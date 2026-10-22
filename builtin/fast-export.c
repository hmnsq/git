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

static ကိန်းပြည့် progress;
static ga_nanaa sign_mode signed_tag_mode = SIGN_ABORT;
static ga_nanaa sign_mode signed_commit_mode = SIGN_STRIP;
static ga_nanaa tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
static ga_nanaa reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
static ကိန်းပြည့် fake_missing_tagger;
static ကိန်းပြည့် use_done_feature;
static ကိန်းပြည့် no_data;
static ကိန်းပြည့် full_tree;
static ကိန်းပြည့် reference_excluded_commits;
static ကိန်းပြည့် show_original_ids;
static ကိန်းပြည့် mark_tags;
static sa_nracanaa string_list extra_refs = STRING_LIST_INIT_DUP;
static sa_nracanaa string_list tag_refs = STRING_LIST_INIT_DUP;
static sa_nracanaa refspec refspecs;
static ကိန်းပြည့် anonymize;
static sa_nracanaa hashmap anonymized_seeds;
static sa_nracanaa revision_sources revision_sources;

static ကိန်းပြည့် parse_opt_sign_mode(const sa_nracanaa option *opt,
			       const var_na *arg, ကိန်းပြည့် unset)
{
	ga_nanaa sign_mode *val = opt->value;

	အကယ်၍ (unset)
		ပြန်ပေး 0;

	အကယ်၍ (parse_sign_mode(arg, val, NULL) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		ပြန်ပေး error(_("unknown %s mode: %s"), opt->long_name, arg);

	ပြန်ပေး 0;
}

static ကိန်းပြည့် parse_opt_tag_of_filtered_mode(const sa_nracanaa option *opt,
					  const var_na *arg, ကိန်းပြည့် unset)
{
	ga_nanaa tag_of_filtered_mode *val = opt->value;

	အကယ်၍ (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	မဟုတ်ပါက အကယ်၍ (!strcmp(arg, "drop"))
		*val = DROP;
	မဟုတ်ပါက အကယ်၍ (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	မဟုတ်ပါက
		ပြန်ပေး error(_("unknown tag-of-filtered mode: %s"), arg);
	ပြန်ပေး 0;
}

static ကိန်းပြည့် parse_opt_reencode_mode(const sa_nracanaa option *opt,
				   const var_na *arg, ကိန်းပြည့် unset)
{
	ga_nanaa reencode_mode *val = opt->value;

	အကယ်၍ (unset) {
		*val = REENCODE_ABORT;
		ပြန်ပေး 0;
	}

	switch (git_parse_maybe_bool(arg)) {
	pak_sa 0:
		*val = REENCODE_NO;
		bha_nga;
	pak_sa 1:
		*val = REENCODE_YES;
		bha_nga;
	default:
		အကယ်၍ (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		မဟုတ်ပါက
			ပြန်ပေး error(_("unknown reencoding mode: %s"), arg);
	}

	ပြန်ပေး 0;
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

static ကိန်းပြည့် anonymized_entry_cmp(const void *cmp_data UNUSED,
				const sa_nracanaa hashmap_entry *eptr,
				const sa_nracanaa hashmap_entry *entry_or_key,
				const void *keydata)
{
	const sa_nracanaa anonymized_entry *a, *b;

	a = container_of(eptr, const sa_nracanaa anonymized_entry, hash);
	အကယ်၍ (keydata) {
		const sa_nracanaa anonymized_entry_key *key = keydata;
		ကိန်းပြည့် equal = !xstrncmpz(a->orig, key->orig, key->orig_len);
		ပြန်ပေး !equal;
	}

	b = container_of(entry_or_key, const sa_nracanaa anonymized_entry, hash);
	ပြန်ပေး strcmp(a->orig, b->orig);
}

static sa_nracanaa anonymized_entry *add_anonymized_entry(sa_nracanaa hashmap *map,
						     unsigned hash,
						     const var_na *orig, size_t len,
						     var_na *anon)
{
	sa_nracanaa anonymized_entry *ret, *old;

	အကယ်၍ (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, NULL, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	အကယ်၍ (old) {
		free(old->anon);
		free(old);
	}

	ပြန်ပေး ret;
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
	အကယ်၍ (!ret)
		ret = hashmap_get_entry(map, &key, hash, &key);

	/* ...and finally generate a new mapping if necessary */
	အကယ်၍ (!ret)
		ret = add_anonymized_entry(map, key.hash.hash,
					   orig, len, generate());

	ပြန်ပေး ret->anon;
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
	နေစဉ် (*path) {
		const var_na *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		const var_na *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		အကယ်၍ (*path)
			strbuf_addch(out, *path++);
	}
}

static anta_hstha void *mark_to_ptr(uint32_t mark)
{
	ပြန်ပေး (void *)(uintptr_t)mark;
}

static anta_hstha uint32_t ptr_to_mark(void * mark)
{
	ပြန်ပေး (uint32_t)(uintptr_t)mark;
}

static anta_hstha void mark_object(sa_nracanaa object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

static anta_hstha void mark_next_object(sa_nracanaa object *object)
{
	mark_object(object, ++last_idnum);
}

static ကိန်းပြည့် get_object_mark(sa_nracanaa object *object)
{
	void *decoration = lookup_decoration(&idnums, object);
	အကယ်၍ (!decoration)
		ပြန်ပေး 0;
	ပြန်ပေး ptr_to_mark(decoration);
}

static sa_nracanaa commit *rewrite_commit(sa_nracanaa commit *p)
{
	အတွက် (;;) {
		အကယ်၍ (p->parents && p->parents->next)
			bha_nga;
		အကယ်၍ (p->object.flags & UNINTERESTING)
			bha_nga;
		အကယ်၍ (!(p->object.flags & TREESAME))
			bha_nga;
		အကယ်၍ (!p->parents)
			ပြန်ပေး NULL;
		p = p->parents->item;
	}
	ပြန်ပေး p;
}

static void show_progress(void)
{
	static ကိန်းပြည့် counter = 0;
	အကယ်၍ (!progress)
		ပြန်ပေး;
	အကယ်၍ ((++counter % progress) == 0)
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
	static ကိန်းပြည့် counter;
	sa_nracanaa strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	ပြန်ပေး strbuf_detach(&out, NULL);
}

static void export_blob(const sa_nracanaa object_id *oid)
{
	unsigned long size;
	ga_nanaa object_type type;
	var_na *buf;
	sa_nracanaa object *object;
	ကိန်းပြည့် eaten;

	အကယ်၍ (no_data)
		ပြန်ပေး;

	အကယ်၍ (is_null_oid(oid))
		ပြန်ပေး;

	object = lookup_object(the_repository, oid);
	အကယ်၍ (object && object->flags & SHOWN)
		ပြန်ပေး;

	အကယ်၍ (anonymize) {
		buf = anonymize_blob(&size);
		object = (sa_nracanaa object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} မဟုတ်ပါက {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		အကယ်၍ (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		အကယ်၍ (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	အကယ်၍ (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	အကယ်၍ (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	အကယ်၍ (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	အကယ်၍ (!eaten)
		free(buf);
}

static ကိန်းပြည့် depth_first(const void *a_, const void *b_)
{
	const sa_nracanaa diff_filepair *a = *((const sa_nracanaa diff_filepair **)a_);
	const sa_nracanaa diff_filepair *b = *((const sa_nracanaa diff_filepair **)b_);
	const var_na *name_a, *name_b;
	ကိန်းပြည့် len_a, len_b, len;
	ကိန်းပြည့် cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	အကယ်၍ (cmp)
		ပြန်ပေး cmp;
	cmp = len_b - len_a;
	အကယ်၍ (cmp)
		ပြန်ပေး cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	ပြန်ပေး (a->status == 'R') - (b->status == 'R');
}

static void print_path_1(const var_na *path)
{
	ကိန်းပြည့် need_quote = quote_c_style(path, NULL, NULL, 0);
	အကယ်၍ (need_quote)
		quote_c_style(path, NULL, stdout, 0);
	မဟုတ်ပါက အကယ်၍ (strchr(path, ' '))
		printf("\"%s\"", path);
	မဟုတ်ပါက
		printf("%s", path);
}

static var_na *anonymize_path_component(void)
{
	static ကိန်းပြည့် counter;
	sa_nracanaa strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	ပြန်ပေး strbuf_detach(&out, NULL);
}

static void print_path(const var_na *path)
{
	အကယ်၍ (!anonymize)
		print_path_1(path);
	မဟုတ်ပါက {
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
	ပြန်ပေး oid_to_hex_r(hex, &oid);
}

static const var_na *anonymize_oid(const var_na *oid_hex)
{
	static sa_nracanaa hashmap objs;
	size_t len = strlen(oid_hex);
	ပြန်ပေး anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

static void show_filemodify(sa_nracanaa diff_queue_struct *q,
			    sa_nracanaa diff_options *options UNUSED, void *data)
{
	ကိန်းပြည့် i;
	sa_nracanaa string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	အတွက် (i = 0; i < q->nr; i++) {
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
			အကယ်၍ (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				print_path(ospec->path);
				putchar(' ');
				print_path(spec->path);
				string_list_insert(changed, spec->path);
				putchar('\n');

				အကယ်၍ (oideq(&ospec->oid, &spec->oid) &&
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
			အကယ်၍ (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			မဟုတ်ပါက {
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
	static ကိန်းပြည့် counter;
	sa_nracanaa strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	ပြန်ပေး strbuf_detach(&out, NULL);
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
	ကိန်းပြည့် i;

	strbuf_reset(&anon);
	အတွက် (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		အကယ်၍ (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			bha_nga;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	ပြန်ပေး anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
static var_na *anonymize_commit_message(void)
{
	static ကိန်းပြည့် counter;
	ပြန်ပေး xstrfmt("subject %d\n\nbody\n", counter++);
}

static var_na *anonymize_ident(void)
{
	static ကိန်းပြည့် counter;
	sa_nracanaa strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	ပြန်ပေး strbuf_detach(&out, NULL);
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
	အကယ်၍ (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (ကိန်းပြည့်)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	အကယ်၍ (!split_ident_line(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		const var_na *ident;
		size_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} မဟုတ်ပါက {
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
	အကယ်၍ (!bol)
		ပြန်ပေး NULL;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	နေစဉ် (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	ပြန်ပေး strbuf_detach(&val, NULL);
}

static void print_signature(const var_na *signature, const var_na *object_hash)
{
	အကယ်၍ (!signature)
		ပြန်ပေး;

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

	နေစဉ် ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (void *)object_hash;
		free((var_na *)signature);
		start = end;
	}

	ပြန်ပေး end;
}

static void handle_commit(sa_nracanaa commit *commit, sa_nracanaa rev_info *rev,
			  sa_nracanaa string_list *paths_of_changed_objects)
{
	ကိန်းပြည့် saved_output_format = rev->diffopt.output_format;
	const var_na *commit_buffer, *commit_buffer_cursor;
	const var_na *author, *author_end, *committer, *committer_end;
	const var_na *encoding = NULL;
	size_t encoding_len;
	sa_nracanaa string_list signatures = STRING_LIST_INIT_DUP;
	const var_na *message;
	var_na *reencoded = NULL;
	sa_nracanaa commit_list *p;
	const var_na *refname;
	ကိန်းပြည့် i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, NULL);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	အကယ်၍ (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	အကယ်၍ (!committer)
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

	အကယ်၍ (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		အကယ်၍ (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	အကယ်၍ (*commit_buffer_cursor == '\n') {
		const var_na *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		const var_na *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
	အကယ်၍ (message)
		message += 2;

	အကယ်၍ (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_tree) {
		parse_commit_or_die(commit->parents->item);
		diff_tree_oid(get_commit_tree_oid(commit->parents->item),
			      get_commit_tree_oid(commit), "", &rev->diffopt);
	}
	မဟုတ်ပါက
		diff_root_tree_oid(get_commit_tree_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	အတွက် (i = 0; i < diff_queued_diff.nr; i++)
		အကယ်၍ (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	အကယ်၍ (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_line(&committer, &committer_end);
		anonymize_ident_line(&author, &author_end);
	}

	mark_next_object(&commit->object);
	အကယ်၍ (anonymize) {
		reencoded = anonymize_commit_message();
	} မဟုတ်ပါက အကယ်၍ (encoding) {
		var_na *buf;
		switch (reencode_mode) {
		pak_sa REENCODE_YES:
			buf = xstrfmt("%.*s", (ကိန်းပြည့်)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			free(buf);
			bha_nga;
		pak_sa REENCODE_NO:
			bha_nga;
		pak_sa REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (ကိန်းပြည့်)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	အကယ်၍ (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	အကယ်၍ (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (ကိန်းပြည့်)(author_end - author), author,
	       (ကိန်းပြည့်)(committer_end - committer), committer);
	အကယ်၍ (signatures.nr) {
		switch (signed_commit_mode) {
		/* Exporting modes */
		pak_sa SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		pak_sa SIGN_VERBATIM:
			အတွက် (size_t i = 0; i < signatures.nr; i++) {
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
	အကယ်၍ (!reencoded && encoding)
		printf("encoding %.*s\n", (ကိန်းပြည့်)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	free(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	အတွက် (i = 0, p = commit->parents; p; p = p->next) {
		sa_nracanaa object *obj = &p->item->object;
		ကိန်းပြည့် mark = get_object_mark(obj);

		အကယ်၍ (!mark && !reference_excluded_commits)
			continue;
		အကယ်၍ (i == 0)
			printf("from ");
		မဟုတ်ပါက
			printf("merge ");
		အကယ်၍ (mark)
			printf(":%d\n", mark);
		မဟုတ်ပါက
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	အကယ်၍ (full_tree)
		printf("deleteall\n");
	log_tree_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

static var_na *anonymize_tag(void)
{
	static ကိန်းပြည့် counter;
	sa_nracanaa strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	ပြန်ပေး strbuf_detach(&out, NULL);
}


static void handle_tag(const var_na *name, sa_nracanaa tag *tag)
{
	size_t size;
	ga_nanaa object_type type;
	var_na *buf;
	const var_na *tagger, *tagger_end, *message;
	size_t message_size = 0;
	sa_nracanaa object *tagged;
	ကိန်းပြည့် tagged_mark;
	sa_nracanaa commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	နေစဉ် (tagged->type == OBJ_TAG) {
		tagged = ((sa_nracanaa tag *)tagged)->tagged;
	}
	အကယ်၍ (tagged->type == OBJ_TREE) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		ပြန်ပေး;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	အကယ်၍ (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	အကယ်၍ (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	အကယ်၍ (!tagger) {
		အကယ်၍ (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		မဟုတ်ပါက
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} မဟုတ်ပါက {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		အကယ်၍ (anonymize)
			anonymize_ident_line(&tagger, &tagger_end);
	}

	အကယ်၍ (anonymize) {
		name = anonymize_refname(name);
		အကယ်၍ (message) {
			static sa_nracanaa hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	အကယ်၍ (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		အကယ်၍ (sig_offset < message_size)
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
	အကယ်၍ (!tagged_mark) {
		switch (tag_of_filtered_mode) {
		pak_sa TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		pak_sa DROP:
			/* Ignore this tag altogether */
			free(buf);
			ပြန်ပေး;
		pak_sa REWRITE:
			အကယ်၍ (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} မဟုတ်ပါက အကယ်၍ (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((sa_nracanaa commit *)tagged);
				အကယ်၍ (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					free(buf);
					ပြန်ပေး;
				}
				tagged_mark = get_object_mark(&p->object);
			} မဟုတ်ပါက {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	အကယ်၍ (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	အကယ်၍ (mark_tags) {
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	အကယ်၍ (tagged_mark)
		printf("from :%d\n", tagged_mark);
	မဟုတ်ပါက
		printf("from %s\n", oid_to_hex(&tagged->oid));

	အကယ်၍ (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (ကိန်းပြည့်)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (ကိန်းပြည့်)message_size, (ကိန်းပြည့်)message_size, message ? message : "");
	free(buf);
}

static sa_nracanaa commit *get_commit(sa_nracanaa rev_cmdline_entry *e, const var_na *full_name)
{
	switch (e->item->type) {
	pak_sa OBJ_COMMIT:
		ပြန်ပေး (sa_nracanaa commit *)e->item;
	pak_sa OBJ_TAG: {
		sa_nracanaa tag *tag = (sa_nracanaa tag *)e->item;

		/* handle nested tags */
		နေစဉ် (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (sa_nracanaa tag *)tag->tagged;
		}
		အကယ်၍ (!tag)
			die(_("tag %s points nowhere?"), e->name);
		ပြန်ပေး (sa_nracanaa commit *)tag;
	}
	default:
		ပြန်ပေး NULL;
	}
}

static void get_tags_and_duplicates(sa_nracanaa rev_cmdline_info *info)
{
	ကိန်းပြည့် i;

	အတွက် (i = 0; i < info->nr; i++) {
		sa_nracanaa rev_cmdline_entry *e = info->rev + i;
		sa_nracanaa object_id oid;
		sa_nracanaa commit *commit;
		var_na *full_name = NULL;

		အကယ်၍ (e->flags & UNINTERESTING)
			continue;

		အကယ်၍ (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			free(full_name);
			continue;
		}

		အကယ်၍ (refspecs.nr) {
			var_na *private;
			private = apply_refspecs(&refspecs, full_name);
			အကယ်၍ (private) {
				free(full_name);
				full_name = private;
			}
		}

		commit = get_commit(e, full_name);
		အကယ်၍ (!commit) {
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
		အကယ်၍ (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		အကယ်၍ (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		မဟုတ်ပါက
			free(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

static void handle_tags_and_duplicates(sa_nracanaa string_list *extras)
{
	sa_nracanaa commit *commit;
	ကိန်းပြည့် i;

	အတွက် (i = extras->nr - 1; i >= 0; i--) {
		const var_na *name = extras->items[i].string;
		sa_nracanaa object *object = extras->items[i].util;
		ကိန်းပြည့် mark;

		switch (object->type) {
		pak_sa OBJ_TAG:
			handle_tag(name, (sa_nracanaa tag *)object);
			bha_nga;
		pak_sa OBJ_COMMIT:
			အကယ်၍ (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((sa_nracanaa commit *)object);
			အကယ်၍ (!commit) {
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
			အကယ်၍ (!mark) {
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
				အကယ်၍ (!reference_excluded_commits) {
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
	unsigned ကိန်းပြည့် i;
	uint32_t mark;
	sa_nracanaa decoration_entry *deco = idnums.entries;
	FILE *f;
	ကိန်းပြည့် e = 0;

	f = fopen_for_writing(file);
	အကယ်၍ (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	အတွက် (i = 0; i < idnums.size; i++) {
		အကယ်၍ (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			အကယ်၍ (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    bha_nga;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	အကယ်၍ (e)
		error(_("unable to write marks file %s."), file);
}

static void import_marks(var_na *input_file, ကိန်းပြည့် check_exists)
{
	var_na line[512];
	FILE *f;
	sa_nracanaa stat sb;

	အကယ်၍ (check_exists && stat(input_file, &sb))
		ပြန်ပေး;

	f = xfopen(input_file, "r");
	နေစဉ် (fgets(line, parimaa_na(line), f)) {
		uint32_t mark;
		var_na *line_end, *mark_end;
		sa_nracanaa object_id oid;
		sa_nracanaa object *object;
		sa_nracanaa commit *commit;
		ga_nanaa object_type type;

		line_end = strchr(line, '\n');
		အကယ်၍ (line[0] != ':' || !line_end)
			die(_("corrupt mark line: %s"), line);
		*line_end = '\0';

		mark = strtoumax(line + 1, &mark_end, 10);
		အကယ်၍ (!mark || mark_end == line + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), line);

		အကယ်၍ (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, NULL);
		အကယ်၍ (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		အကယ်၍ (type != OBJ_COMMIT)
			/* only commits */
			continue;

		commit = lookup_commit(the_repository, &oid);
		အကယ်၍ (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		အကယ်၍ (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

static void handle_deletes(void)
{
	ကိန်းပြည့် i;
	အတွက် (i = 0; i < refspecs.nr; i++) {
		sa_nracanaa refspec_item *refspec = &refspecs.items[i];
		အကယ်၍ (*refspec->src)
			continue;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

static ကိန်းပြည့် parse_opt_anonymize_map(const sa_nracanaa option *opt,
				   const var_na *arg, ကိန်းပြည့် unset)
{
	sa_nracanaa hashmap *map = opt->value;
	const var_na *delim, *value;
	size_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	အကယ်၍ (delim) {
		keylen = delim - arg;
		value = delim + 1;
	} မဟုတ်ပါက {
		keylen = strlen(arg);
		value = arg;
	}

	အကယ်၍ (!keylen || !*value)
		ပြန်ပေး error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(value));

	ပြန်ပေး 0;
}

ကိန်းပြည့် cmd_fast_export(ကိန်းပြည့် argc,
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

	အကယ်၍ (argc == 1)
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
	အကယ်၍ (argc > 1)
		usage_with_options (fast_export_usage, options);

	အကယ်၍ (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	အကယ်၍ (refspecs_list.nr) {
		ကိန်းပြည့် i;

		အတွက် (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.items[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	အကယ်၍ (use_done_feature)
		printf("feature done\n");

	အကယ်၍ (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	အကယ်၍ (import_filename)
		import_marks(import_filename, 0);
	မဟုတ်ပါက အကယ်၍ (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	အကယ်၍ (import_filename && revs.prune_data.nr)
		full_tree = 1;

	get_tags_and_duplicates(&revs.cmdline);

	အကယ်၍ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	နေစဉ် ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	အကယ်၍ (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	အကယ်၍ (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	ပြန်ပေး 0;
}
