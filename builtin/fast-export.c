/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "refs.h"
#include "refspec.h"
#include "object-file.h"
#include "odb.h"
#include "commit.h"
#include "object.h"
#include "tag.h"
#include "diff.h"
#include "diffcore.h"
#include "log-tree.h"
#include "revision.h"
#include "decorate.h"
#include "string-list.h"
#include "utf8.h"
#include "parse-options.h"
#include "quote.h"
#include "remote.h"
#include "blob.h"
#include "gpg-interface.h"

static const var_na *const fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	NULL
};

static puur_naa_nka progress;
static ga_nanaa sign_mode signed_tag_mode = SIGN_ABORT;
static ga_nanaa sign_mode signed_commit_mode = SIGN_STRIP;
static ga_nanaa tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
static ga_nanaa reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
static puur_naa_nka fake_missing_tagger;
static puur_naa_nka use_done_feature;
static puur_naa_nka no_data;
static puur_naa_nka full_tree;
static puur_naa_nka reference_excluded_commits;
static puur_naa_nka show_original_ids;
static puur_naa_nka mark_tags;
static sa_nracanaa string_list extra_refs = STRING_LIST_INIT_DUP;
static sa_nracanaa string_list tag_refs = STRING_LIST_INIT_DUP;
static sa_nracanaa refspec refspecs;
static puur_naa_nka anonymize;
static sa_nracanaa hashmap anonymized_seeds;
static sa_nracanaa revision_sources revision_sources;

static puur_naa_nka parse_opt_sign_mode(const sa_nracanaa option *opt,
			       const var_na *arg, puur_naa_nka unset)
{
	ga_nanaa sign_mode *val = opt->value;

	ပေႃး (unset)
		ပွၵ်ႈ 0;

	ပေႃး (parse_sign_mode(arg, val, NULL) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		ပွၵ်ႈ error(_("unknown %s mode: %s"), opt->long_name, arg);

	ပွၵ်ႈ 0;
}

static puur_naa_nka parse_opt_tag_of_filtered_mode(const sa_nracanaa option *opt,
					  const var_na *arg, puur_naa_nka unset)
{
	ga_nanaa tag_of_filtered_mode *val = opt->value;

	ပေႃး (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	ပေႃးဢမ်ႇ ပေႃး (!strcmp(arg, "drop"))
		*val = DROP;
	ပေႃးဢမ်ႇ ပေႃး (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	ပေႃးဢမ်ႇ
		ပွၵ်ႈ error(_("unknown tag-of-filtered mode: %s"), arg);
	ပွၵ်ႈ 0;
}

static puur_naa_nka parse_opt_reencode_mode(const sa_nracanaa option *opt,
				   const var_na *arg, puur_naa_nka unset)
{
	ga_nanaa reencode_mode *val = opt->value;

	ပေႃး (unset) {
		*val = REENCODE_ABORT;
		ပွၵ်ႈ 0;
	}

	switch (git_parse_maybe_bool(arg)) {
	pak_sa 0:
		*val = REENCODE_NO;
		bha_nga;
	pak_sa 1:
		*val = REENCODE_YES;
		bha_nga;
	default:
		ပေႃး (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		ပေႃးဢမ်ႇ
			ပွၵ်ႈ error(_("unknown reencoding mode: %s"), arg);
	}

	ပွၵ်ႈ 0;
}

static sa_nracanaa decoration idnums;
static uint32_t last_idnum;
sa_nracanaa anonymized_entry {
	sa_nracanaa hashmap_entry hash;
	var_na *anon;
	const var_na orig[FLEX_ARRAY];
};

sa_nracanaa anonymized_entry_key {
	sa_nracanaa hashmap_entry hash;
	const var_na *orig;
	size_t orig_len;
};

static puur_naa_nka anonymized_entry_cmp(const void *cmp_data UNUSED,
				const sa_nracanaa hashmap_entry *eptr,
				const sa_nracanaa hashmap_entry *entry_or_key,
				const void *keydata)
{
	const sa_nracanaa anonymized_entry *a, *b;

	a = container_of(eptr, const sa_nracanaa anonymized_entry, hash);
	ပေႃး (keydata) {
		const sa_nracanaa anonymized_entry_key *key = keydata;
		puur_naa_nka equal = !xstrncmpz(a->orig, key->orig, key->orig_len);
		ပွၵ်ႈ !equal;
	}

	b = container_of(entry_or_key, const sa_nracanaa anonymized_entry, hash);
	ပွၵ်ႈ strcmp(a->orig, b->orig);
}

static sa_nracanaa anonymized_entry *add_anonymized_entry(sa_nracanaa hashmap *map,
						     unsigned hash,
						     const var_na *orig, size_t len,
						     var_na *anon)
{
	sa_nracanaa anonymized_entry *ret, *old;

	ပေႃး (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, NULL, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	ပေႃး (old) {
		free(old->anon);
		free(old);
	}

	ပွၵ်ႈ ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
static const var_na *anonymize_str(sa_nracanaa hashmap *map,
				 var_na *(*generate)(void),
				 const var_na *orig, size_t len)
{
	sa_nracanaa anonymized_entry_key key;
	sa_nracanaa anonymized_entry *ret;

	hashmap_entry_init(&key.hash, memhash(orig, len));
	key.orig = orig;
	key.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &key, hash, &key);

	/* ...otherwise check if we've already seen it in this context... */
	ပေႃး (!ret)
		ret = hashmap_get_entry(map, &key, hash, &key);

	/* ...and finally generate a new mapping if necessary */
	ပေႃး (!ret)
		ret = add_anonymized_entry(map, key.hash.hash,
					   orig, len, generate());

	ပွၵ်ႈ ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
static void anonymize_path(sa_nracanaa strbuf *out, const var_na *path,
			   sa_nracanaa hashmap *map,
			   var_na *(*generate)(void))
{
	ၶၢဝ်း (*path) {
		const var_na *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		const var_na *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		ပေႃး (*path)
			strbuf_addch(out, *path++);
	}
}

static anta_hstha void *mark_to_ptr(uint32_t mark)
{
	ပွၵ်ႈ (void *)(uintptr_t)mark;
}

static anta_hstha uint32_t ptr_to_mark(void * mark)
{
	ပွၵ်ႈ (uint32_t)(uintptr_t)mark;
}

static anta_hstha void mark_object(sa_nracanaa object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

static anta_hstha void mark_next_object(sa_nracanaa object *object)
{
	mark_object(object, ++last_idnum);
}

static puur_naa_nka get_object_mark(sa_nracanaa object *object)
{
	void *decoration = lookup_decoration(&idnums, object);
	ပေႃး (!decoration)
		ပွၵ်ႈ 0;
	ပွၵ်ႈ ptr_to_mark(decoration);
}

static sa_nracanaa commit *rewrite_commit(sa_nracanaa commit *p)
{
	for (;;) {
		ပေႃး (p->parents && p->parents->next)
			bha_nga;
		ပေႃး (p->object.flags & UNINTERESTING)
			bha_nga;
		ပေႃး (!(p->object.flags & TREESAME))
			bha_nga;
		ပေႃး (!p->parents)
			ပွၵ်ႈ NULL;
		p = p->parents->item;
	}
	ပွၵ်ႈ p;
}

static void show_progress(void)
{
	static puur_naa_nka counter = 0;
	ပေႃး (!progress)
		ပွၵ်ႈ;
	ပေႃး ((++counter % progress) == 0)
		printf("progress %d objects\n", counter);
}

/*
 * Ideally we would want some transformation of the blob data here
 * that is unreversible, but would still be the same size and have
 * the same data relationship to other blobs (so that we get the same
 * delta and packing behavior as the original). But the first and last
 * requirements there are probably mutually exclusive, so let's take
 * the easy way out for now, and just generate arbitrary content.
 *
 * There's no need to cache this result with anonymize_mem, since
 * we already handle blob content caching with marks.
 */
static var_na *anonymize_blob(unsigned long *size)
{
	static puur_naa_nka counter;
	sa_nracanaa strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	ပွၵ်ႈ strbuf_detach(&out, NULL);
}

static void export_blob(const sa_nracanaa object_id *oid)
{
	unsigned long size;
	ga_nanaa object_type type;
	var_na *buf;
	sa_nracanaa object *object;
	puur_naa_nka eaten;

	ပေႃး (no_data)
		ပွၵ်ႈ;

	ပေႃး (is_null_oid(oid))
		ပွၵ်ႈ;

	object = lookup_object(the_repository, oid);
	ပေႃး (object && object->flags & SHOWN)
		ပွၵ်ႈ;

	ပေႃး (anonymize) {
		buf = anonymize_blob(&size);
		object = (sa_nracanaa object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} ပေႃးဢမ်ႇ {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		ပေႃး (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		ပေႃး (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	ပေႃး (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	ပေႃး (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	ပေႃး (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	ပေႃး (!eaten)
		free(buf);
}

static puur_naa_nka depth_first(const void *a_, const void *b_)
{
	const sa_nracanaa diff_filepair *a = *((const sa_nracanaa diff_filepair **)a_);
	const sa_nracanaa diff_filepair *b = *((const sa_nracanaa diff_filepair **)b_);
	const var_na *name_a, *name_b;
	puur_naa_nka len_a, len_b, len;
	puur_naa_nka cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	ပေႃး (cmp)
		ပွၵ်ႈ cmp;
	cmp = len_b - len_a;
	ပေႃး (cmp)
		ပွၵ်ႈ cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	ပွၵ်ႈ (a->status == 'R') - (b->status == 'R');
}

static void print_path_1(const var_na *path)
{
	puur_naa_nka need_quote = quote_c_style(path, NULL, NULL, 0);
	ပေႃး (need_quote)
		quote_c_style(path, NULL, stdout, 0);
	ပေႃးဢမ်ႇ ပေႃး (strchr(path, ' '))
		printf("\"%s\"", path);
	ပေႃးဢမ်ႇ
		printf("%s", path);
}

static var_na *anonymize_path_component(void)
{
	static puur_naa_nka counter;
	sa_nracanaa strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	ပွၵ်ႈ strbuf_detach(&out, NULL);
}

static void print_path(const var_na *path)
{
	ပေႃး (!anonymize)
		print_path_1(path);
	ပေႃးဢမ်ႇ {
		static sa_nracanaa hashmap paths;
		static sa_nracanaa strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

static var_na *generate_fake_oid(void)
{
	static uint32_t counter = 1; /* avoid null oid */
	const unsigned hashsz = the_hash_algo->rawsz;
	sa_nracanaa object_id oid;
	var_na *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	ပွၵ်ႈ oid_to_hex_r(hex, &oid);
}

static const var_na *anonymize_oid(const var_na *oid_hex)
{
	static sa_nracanaa hashmap objs;
	size_t len = strlen(oid_hex);
	ပွၵ်ႈ anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

static void show_filemodify(sa_nracanaa diff_queue_struct *q,
			    sa_nracanaa diff_options *options UNUSED, void *data)
{
	puur_naa_nka i;
	sa_nracanaa string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	for (i = 0; i < q->nr; i++) {
		sa_nracanaa diff_filespec *ospec = q->queue[i]->one;
		sa_nracanaa diff_filespec *spec = q->queue[i]->two;

		switch (q->queue[i]->status) {
		pak_sa DIFF_STATUS_DELETED:
			printf("D ");
			print_path(spec->path);
			string_list_insert(changed, spec->path);
			putchar('\n');
			bha_nga;

		pak_sa DIFF_STATUS_COPIED:
		pak_sa DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			ပေႃး (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				print_path(ospec->path);
				putchar(' ');
				print_path(spec->path);
				string_list_insert(changed, spec->path);
				putchar('\n');

				ပေႃး (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					bha_nga;
			}
			/* fallthrough */

		pak_sa DIFF_STATUS_TYPE_CHANGED:
		pak_sa DIFF_STATUS_MODIFIED:
		pak_sa DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			ပေႃး (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			ပေႃးဢမ်ႇ {
				sa_nracanaa object *object = lookup_object(the_repository,
								      &spec->oid);
				printf("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_insert(changed, spec->path);
			putchar('\n');
			bha_nga;

		default:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

static var_na *anonymize_ref_component(void)
{
	static puur_naa_nka counter;
	sa_nracanaa strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	ပွၵ်ႈ strbuf_detach(&out, NULL);
}

static const var_na *anonymize_refname(const var_na *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	static const var_na *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	static sa_nracanaa hashmap refs;
	static sa_nracanaa strbuf anon = STRBUF_INIT;
	puur_naa_nka i;

	strbuf_reset(&anon);
	for (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		ပေႃး (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			bha_nga;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	ပွၵ်ႈ anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
static var_na *anonymize_commit_message(void)
{
	static puur_naa_nka counter;
	ပွၵ်ႈ xstrfmt("subject %d\n\nbody\n", counter++);
}

static var_na *anonymize_ident(void)
{
	static puur_naa_nka counter;
	sa_nracanaa strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	ပွၵ်ႈ strbuf_detach(&out, NULL);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
static void anonymize_ident_line(const var_na **beg, const var_na **end)
{
	static sa_nracanaa hashmap idents;
	static sa_nracanaa strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	static unsigned which_buffer;

	sa_nracanaa strbuf *out;
	sa_nracanaa ident_split split;
	const var_na *end_of_header;

	out = &buffers[which_buffer++];
	which_buffer %= ARRAY_SIZE(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = strchr(*beg, ' ');
	ပေႃး (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (puur_naa_nka)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	ပေႃး (!split_ident_line(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		const var_na *ident;
		size_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} ပေႃးဢမ်ႇ {
		strbuf_addstr(out, "Malformed Ident <malformed@example.com> 0 -0000");
	}

	*beg = out->buf;
	*end = out->buf + out->len;
}

/*
 * find_commit_multiline_header is similar to find_commit_header,
 * except that it handles multi-line headers, rather than simply
 * returning the first line of the header.
 *
 * The returned string has had the ' ' line continuation markers
 * removed, and points to allocated memory that must be free()d (not
 * to memory within 'msg').
 *
 * If the header is found, then *end is set to point at the '\n' in
 * msg that immediately follows the header value.
 */
static const var_na *find_commit_multiline_header(const var_na *msg,
						const var_na *key,
						const var_na **end)
{
	sa_nracanaa strbuf val = STRBUF_INIT;
	const var_na *bol, *eol;
	size_t len;

	bol = find_commit_header(msg, key, &len);
	ပေႃး (!bol)
		ပွၵ်ႈ NULL;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	ၶၢဝ်း (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	ပွၵ်ႈ strbuf_detach(&val, NULL);
}

static void print_signature(const var_na *signature, const var_na *object_hash)
{
	ပေႃး (!signature)
		ပွၵ်ႈ;

	printf("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (unsigned)strlen(signature),
	       signature);
}

static const var_na *append_signatures_for_header(sa_nracanaa string_list *signatures,
						const var_na *pos,
						const var_na *header,
						const var_na *object_hash)
{
	const var_na *signature;
	const var_na *start = pos;
	const var_na *end = pos;

	ၶၢဝ်း ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (void *)object_hash;
		free((var_na *)signature);
		start = end;
	}

	ပွၵ်ႈ end;
}

static void handle_commit(sa_nracanaa commit *commit, sa_nracanaa rev_info *rev,
			  sa_nracanaa string_list *paths_of_changed_objects)
{
	puur_naa_nka saved_output_format = rev->diffopt.output_format;
	const var_na *commit_buffer, *commit_buffer_cursor;
	const var_na *author, *author_end, *committer, *committer_end;
	const var_na *encoding = NULL;
	size_t encoding_len;
	sa_nracanaa string_list signatures = STRING_LIST_INIT_DUP;
	const var_na *message;
	var_na *reencoded = NULL;
	sa_nracanaa commit_list *p;
	const var_na *refname;
	puur_naa_nka i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, NULL);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	ပေႃး (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	ပေႃး (!committer)
		die(_("could not find committer in commit %s"),
		    oid_to_hex(&commit->object.oid));
	committer++;
	commit_buffer_cursor = committer_end = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	ပေႃး (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		ပေႃး (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	ပေႃး (*commit_buffer_cursor == '\n') {
		const var_na *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		const var_na *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
	ပေႃး (message)
		message += 2;

	ပေႃး (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_tree) {
		parse_commit_or_die(commit->parents->item);
		diff_tree_oid(get_commit_tree_oid(commit->parents->item),
			      get_commit_tree_oid(commit), "", &rev->diffopt);
	}
	ပေႃးဢမ်ႇ
		diff_root_tree_oid(get_commit_tree_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	for (i = 0; i < diff_queued_diff.nr; i++)
		ပေႃး (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	ပေႃး (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_line(&committer, &committer_end);
		anonymize_ident_line(&author, &author_end);
	}

	mark_next_object(&commit->object);
	ပေႃး (anonymize) {
		reencoded = anonymize_commit_message();
	} ပေႃးဢမ်ႇ ပေႃး (encoding) {
		var_na *buf;
		switch (reencode_mode) {
		pak_sa REENCODE_YES:
			buf = xstrfmt("%.*s", (puur_naa_nka)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			free(buf);
			bha_nga;
		pak_sa REENCODE_NO:
			bha_nga;
		pak_sa REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (puur_naa_nka)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	ပေႃး (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	ပေႃး (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (puur_naa_nka)(author_end - author), author,
	       (puur_naa_nka)(committer_end - committer), committer);
	ပေႃး (signatures.nr) {
		switch (signed_commit_mode) {
		/* Exporting modes */
		pak_sa SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		pak_sa SIGN_VERBATIM:
			for (size_t i = 0; i < signatures.nr; i++) {
				sa_nracanaa string_list_item *item = &signatures.items[i];
				print_signature(item->string, item->util);
			}
			bha_nga;

		/* Stripping modes */
		pak_sa SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		pak_sa SIGN_STRIP:
			bha_nga;

		/* Aborting modes */
		pak_sa SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		default:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	ပေႃး (!reencoded && encoding)
		printf("encoding %.*s\n", (puur_naa_nka)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	free(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	for (i = 0, p = commit->parents; p; p = p->next) {
		sa_nracanaa object *obj = &p->item->object;
		puur_naa_nka mark = get_object_mark(obj);

		ပေႃး (!mark && !reference_excluded_commits)
			continue;
		ပေႃး (i == 0)
			printf("from ");
		ပေႃးဢမ်ႇ
			printf("merge ");
		ပေႃး (mark)
			printf(":%d\n", mark);
		ပေႃးဢမ်ႇ
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	ပေႃး (full_tree)
		printf("deleteall\n");
	log_tree_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

static var_na *anonymize_tag(void)
{
	static puur_naa_nka counter;
	sa_nracanaa strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	ပွၵ်ႈ strbuf_detach(&out, NULL);
}


static void handle_tag(const var_na *name, sa_nracanaa tag *tag)
{
	size_t size;
	ga_nanaa object_type type;
	var_na *buf;
	const var_na *tagger, *tagger_end, *message;
	size_t message_size = 0;
	sa_nracanaa object *tagged;
	puur_naa_nka tagged_mark;
	sa_nracanaa commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	ၶၢဝ်း (tagged->type == OBJ_TAG) {
		tagged = ((sa_nracanaa tag *)tagged)->tagged;
	}
	ပေႃး (tagged->type == OBJ_TREE) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		ပွၵ်ႈ;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	ပေႃး (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	ပေႃး (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	ပေႃး (!tagger) {
		ပေႃး (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		ပေႃးဢမ်ႇ
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} ပေႃးဢမ်ႇ {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		ပေႃး (anonymize)
			anonymize_ident_line(&tagger, &tagger_end);
	}

	ပေႃး (anonymize) {
		name = anonymize_refname(name);
		ပေႃး (message) {
			static sa_nracanaa hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	ပေႃး (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		ပေႃး (sig_offset < message_size)
			switch (signed_tag_mode) {
			/* Exporting modes */
			pak_sa SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			pak_sa SIGN_VERBATIM:
				bha_nga;

			/* Stripping modes */
			pak_sa SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			pak_sa SIGN_STRIP:
				message_size = sig_offset;
				bha_nga;

			/* Aborting modes */
			pak_sa SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			default:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	ပေႃး (!tagged_mark) {
		switch (tag_of_filtered_mode) {
		pak_sa TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		pak_sa DROP:
			/* Ignore this tag altogether */
			free(buf);
			ပွၵ်ႈ;
		pak_sa REWRITE:
			ပေႃး (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} ပေႃးဢမ်ႇ ပေႃး (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((sa_nracanaa commit *)tagged);
				ပေႃး (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					free(buf);
					ပွၵ်ႈ;
				}
				tagged_mark = get_object_mark(&p->object);
			} ပေႃးဢမ်ႇ {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	ပေႃး (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	ပေႃး (mark_tags) {
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	ပေႃး (tagged_mark)
		printf("from :%d\n", tagged_mark);
	ပေႃးဢမ်ႇ
		printf("from %s\n", oid_to_hex(&tagged->oid));

	ပေႃး (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (puur_naa_nka)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (puur_naa_nka)message_size, (puur_naa_nka)message_size, message ? message : "");
	free(buf);
}

static sa_nracanaa commit *get_commit(sa_nracanaa rev_cmdline_entry *e, const var_na *full_name)
{
	switch (e->item->type) {
	pak_sa OBJ_COMMIT:
		ပွၵ်ႈ (sa_nracanaa commit *)e->item;
	pak_sa OBJ_TAG: {
		sa_nracanaa tag *tag = (sa_nracanaa tag *)e->item;

		/* handle nested tags */
		ၶၢဝ်း (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (sa_nracanaa tag *)tag->tagged;
		}
		ပေႃး (!tag)
			die(_("tag %s points nowhere?"), e->name);
		ပွၵ်ႈ (sa_nracanaa commit *)tag;
	}
	default:
		ပွၵ်ႈ NULL;
	}
}

static void get_tags_and_duplicates(sa_nracanaa rev_cmdline_info *info)
{
	puur_naa_nka i;

	for (i = 0; i < info->nr; i++) {
		sa_nracanaa rev_cmdline_entry *e = info->rev + i;
		sa_nracanaa object_id oid;
		sa_nracanaa commit *commit;
		var_na *full_name = NULL;

		ပေႃး (e->flags & UNINTERESTING)
			continue;

		ပေႃး (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			free(full_name);
			continue;
		}

		ပေႃး (refspecs.nr) {
			var_na *private;
			private = apply_refspecs(&refspecs, full_name);
			ပေႃး (private) {
				free(full_name);
				full_name = private;
			}
		}

		commit = get_commit(e, full_name);
		ပေႃး (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			free(full_name);
			continue;
		}

		switch (commit->object.type) {
		pak_sa OBJ_COMMIT:
			bha_nga;
		pak_sa OBJ_BLOB:
			export_blob(&commit->object.oid);
			free(full_name);
			continue;
		default: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			free(full_name);
			continue;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		ပေႃး (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		ပေႃး (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		ပေႃးဢမ်ႇ
			free(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

static void handle_tags_and_duplicates(sa_nracanaa string_list *extras)
{
	sa_nracanaa commit *commit;
	puur_naa_nka i;

	for (i = extras->nr - 1; i >= 0; i--) {
		const var_na *name = extras->items[i].string;
		sa_nracanaa object *object = extras->items[i].util;
		puur_naa_nka mark;

		switch (object->type) {
		pak_sa OBJ_TAG:
			handle_tag(name, (sa_nracanaa tag *)object);
			bha_nga;
		pak_sa OBJ_COMMIT:
			ပေႃး (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((sa_nracanaa commit *)object);
			ပေႃး (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				printf("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				continue;
			}

			mark = get_object_mark(&commit->object);
			ပေႃး (!mark) {
				/*
				 * Getting here means we have a commit which
				 * was excluded by a negative refspec (e.g.
				 * fast-export ^HEAD HEAD).  If we are
				 * referencing excluded commits, set the ref
				 * to the exact commit.  Otherwise, the user
				 * wants the branch exported but every commit
				 * in its history to be deleted, which basically
				 * just means deletion of the ref.
				 */
				ပေႃး (!reference_excluded_commits) {
					/* delete the ref */
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					continue;
				}
				/* set ref to commit using oid, not mark */
				printf("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				continue;
			}

			printf("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			bha_nga;
		}
	}
}

static void export_marks(var_na *file)
{
	unsigned puur_naa_nka i;
	uint32_t mark;
	sa_nracanaa decoration_entry *deco = idnums.entries;
	FILE *f;
	puur_naa_nka e = 0;

	f = fopen_for_writing(file);
	ပေႃး (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	for (i = 0; i < idnums.size; i++) {
		ပေႃး (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			ပေႃး (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    bha_nga;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	ပေႃး (e)
		error(_("unable to write marks file %s."), file);
}

static void import_marks(var_na *input_file, puur_naa_nka check_exists)
{
	var_na line[512];
	FILE *f;
	sa_nracanaa stat sb;

	ပေႃး (check_exists && stat(input_file, &sb))
		ပွၵ်ႈ;

	f = xfopen(input_file, "r");
	ၶၢဝ်း (fgets(line, parimaa_na(line), f)) {
		uint32_t mark;
		var_na *line_end, *mark_end;
		sa_nracanaa object_id oid;
		sa_nracanaa object *object;
		sa_nracanaa commit *commit;
		ga_nanaa object_type type;

		line_end = strchr(line, '\n');
		ပေႃး (line[0] != ':' || !line_end)
			die(_("corrupt mark line: %s"), line);
		*line_end = '\0';

		mark = strtoumax(line + 1, &mark_end, 10);
		ပေႃး (!mark || mark_end == line + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), line);

		ပေႃး (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, NULL);
		ပေႃး (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		ပေႃး (type != OBJ_COMMIT)
			/* only commits */
			continue;

		commit = lookup_commit(the_repository, &oid);
		ပေႃး (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		ပေႃး (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

static void handle_deletes(void)
{
	puur_naa_nka i;
	for (i = 0; i < refspecs.nr; i++) {
		sa_nracanaa refspec_item *refspec = &refspecs.items[i];
		ပေႃး (*refspec->src)
			continue;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

static puur_naa_nka parse_opt_anonymize_map(const sa_nracanaa option *opt,
				   const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa hashmap *map = opt->value;
	const var_na *delim, *value;
	size_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	ပေႃး (delim) {
		keylen = delim - arg;
		value = delim + 1;
	} ပေႃးဢမ်ႇ {
		keylen = strlen(arg);
		value = arg;
	}

	ပေႃး (!keylen || !*value)
		ပွၵ်ႈ error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(value));

	ပွၵ်ႈ 0;
}

puur_naa_nka cmd_fast_export(puur_naa_nka argc,
		    const var_na **argv,
		    const var_na *prefix,
		    sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa rev_info revs;
	sa_nracanaa commit *commit;
	var_na *export_filename = NULL,
	     *import_filename = NULL,
	     *import_filename_if_exists = NULL;
	uint32_t lastimportid;
	sa_nracanaa string_list refspecs_list = STRING_LIST_INIT_NODUP;
	sa_nracanaa string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	sa_nracanaa option options[] = {
		OPT_INTEGER(0, "progress", &progress,
			    N_("show progress after <n> objects")),
		OPT_CALLBACK(0, "signed-tags", &signed_tag_mode, N_("mode"),
			     N_("select handling of signed tags"),
			     parse_opt_sign_mode),
		OPT_CALLBACK(0, "signed-commits", &signed_commit_mode, N_("mode"),
			     N_("select handling of signed commits"),
			     parse_opt_sign_mode),
		OPT_CALLBACK(0, "tag-of-filtered-object", &tag_of_filtered_mode, N_("mode"),
			     N_("select handling of tags that tag filtered objects"),
			     parse_opt_tag_of_filtered_mode),
		OPT_CALLBACK(0, "reencode", &reencode_mode, N_("mode"),
			     N_("select handling of commit messages in an alternate encoding"),
			     parse_opt_reencode_mode),
		OPT_STRING(0, "export-marks", &export_filename, N_("file"),
			     N_("dump marks to this file")),
		OPT_STRING(0, "import-marks", &import_filename, N_("file"),
			     N_("import marks from this file")),
		OPT_STRING(0, "import-marks-if-exists",
			     &import_filename_if_exists,
			     N_("file"),
			     N_("import marks from this file if it exists")),
		OPT_BOOL(0, "fake-missing-tagger", &fake_missing_tagger,
			 N_("fake a tagger when tags lack one")),
		OPT_BOOL(0, "full-tree", &full_tree,
			 N_("output full tree for each commit")),
		OPT_BOOL(0, "use-done-feature", &use_done_feature,
			     N_("use the done feature to terminate the stream")),
		OPT_BOOL(0, "no-data", &no_data, N_("skip output of blob data")),
		OPT_STRING_LIST(0, "refspec", &refspecs_list, N_("refspec"),
			     N_("apply refspec to exported refs")),
		OPT_BOOL(0, "anonymize", &anonymize, N_("anonymize output")),
		OPT_CALLBACK_F(0, "anonymize-map", &anonymized_seeds, N_("from:to"),
			       N_("convert <from> to <to> in anonymized output"),
			       PARSE_OPT_NONEG, parse_opt_anonymize_map),
		OPT_BOOL(0, "reference-excluded-parents",
			 &reference_excluded_commits, N_("reference parents which are not in fast-export stream by object id")),
		OPT_BOOL(0, "show-original-ids", &show_original_ids,
			    N_("show original object ids of blobs/commits")),
		OPT_BOOL(0, "mark-tags", &mark_tags,
			    N_("label tags with mark ids")),

		OPT_END()
	};

	ပေႃး (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, NULL);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, prefix);
	init_revision_sources(&revision_sources);
	revs.topo_order = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, prefix, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = setup_revisions(argc, argv, &revs, NULL);
	ပေႃး (argc > 1)
		usage_with_options (fast_export_usage, options);

	ပေႃး (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	ပေႃး (refspecs_list.nr) {
		puur_naa_nka i;

		for (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.items[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	ပေႃး (use_done_feature)
		printf("feature done\n");

	ပေႃး (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	ပေႃး (import_filename)
		import_marks(import_filename, 0);
	ပေႃးဢမ်ႇ ပေႃး (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	ပေႃး (import_filename && revs.prune_data.nr)
		full_tree = 1;

	get_tags_and_duplicates(&revs.cmdline);

	ပေႃး (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	ၶၢဝ်း ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	ပေႃး (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	ပေႃး (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	ပွၵ်ႈ 0;
}
