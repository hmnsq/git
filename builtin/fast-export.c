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

static const char *const fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	NULL
};

static 整数 progress;
static enum sign_mode signed_tag_mode = SIGN_ABORT;
static enum sign_mode signed_commit_mode = SIGN_STRIP;
static enum tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
static enum reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
static 整数 fake_missing_tagger;
static 整数 use_done_feature;
static 整数 no_data;
static 整数 full_tree;
static 整数 reference_excluded_commits;
static 整数 show_original_ids;
static 整数 mark_tags;
static struct string_list extra_refs = STRING_LIST_INIT_DUP;
static struct string_list tag_refs = STRING_LIST_INIT_DUP;
static struct refspec refspecs;
static 整数 anonymize;
static struct hashmap anonymized_seeds;
static struct revision_sources revision_sources;

static 整数 parse_opt_sign_mode(const struct option *opt,
			       const char *arg, 整数 unset)
{
	enum sign_mode *val = opt->value;

	如果 (unset)
		返回 0;

	如果 (parse_sign_mode(arg, val, NULL) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		返回 error(_("unknown %s mode: %s"), opt->long_name, arg);

	返回 0;
}

static 整数 parse_opt_tag_of_filtered_mode(const struct option *opt,
					  const char *arg, 整数 unset)
{
	enum tag_of_filtered_mode *val = opt->value;

	如果 (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	否则 如果 (!strcmp(arg, "drop"))
		*val = DROP;
	否则 如果 (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	否则
		返回 error(_("unknown tag-of-filtered mode: %s"), arg);
	返回 0;
}

static 整数 parse_opt_reencode_mode(const struct option *opt,
				   const char *arg, 整数 unset)
{
	enum reencode_mode *val = opt->value;

	如果 (unset) {
		*val = REENCODE_ABORT;
		返回 0;
	}

	switch (git_parse_maybe_bool(arg)) {
	case 0:
		*val = REENCODE_NO;
		break;
	case 1:
		*val = REENCODE_YES;
		break;
	default:
		如果 (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		否则
			返回 error(_("unknown reencoding mode: %s"), arg);
	}

	返回 0;
}

static struct decoration idnums;
static uint32_t last_idnum;
struct anonymized_entry {
	struct hashmap_entry hash;
	char *anon;
	const char orig[FLEX_ARRAY];
};

struct anonymized_entry_key {
	struct hashmap_entry hash;
	const char *orig;
	size_t orig_len;
};

static 整数 anonymized_entry_cmp(const void *cmp_data UNUSED,
				const struct hashmap_entry *eptr,
				const struct hashmap_entry *entry_or_key,
				const void *keydata)
{
	const struct anonymized_entry *a, *b;

	a = container_of(eptr, const struct anonymized_entry, hash);
	如果 (keydata) {
		const struct anonymized_entry_key *key = keydata;
		整数 equal = !xstrncmpz(a->orig, key->orig, key->orig_len);
		返回 !equal;
	}

	b = container_of(entry_or_key, const struct anonymized_entry, hash);
	返回 strcmp(a->orig, b->orig);
}

static struct anonymized_entry *add_anonymized_entry(struct hashmap *map,
						     unsigned hash,
						     const char *orig, size_t len,
						     char *anon)
{
	struct anonymized_entry *ret, *old;

	如果 (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, NULL, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	如果 (old) {
		free(old->anon);
		free(old);
	}

	返回 ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
static const char *anonymize_str(struct hashmap *map,
				 char *(*generate)(void),
				 const char *orig, size_t len)
{
	struct anonymized_entry_key key;
	struct anonymized_entry *ret;

	hashmap_entry_init(&key.hash, memhash(orig, len));
	key.orig = orig;
	key.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &key, hash, &key);

	/* ...otherwise check if we've already seen it in this context... */
	如果 (!ret)
		ret = hashmap_get_entry(map, &key, hash, &key);

	/* ...and finally generate a new mapping if necessary */
	如果 (!ret)
		ret = add_anonymized_entry(map, key.hash.hash,
					   orig, len, generate());

	返回 ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
static void anonymize_path(struct strbuf *out, const char *path,
			   struct hashmap *map,
			   char *(*generate)(void))
{
	当 (*path) {
		const char *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		const char *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		如果 (*path)
			strbuf_addch(out, *path++);
	}
}

static inline void *mark_to_ptr(uint32_t mark)
{
	返回 (void *)(uintptr_t)mark;
}

static inline uint32_t ptr_to_mark(void * mark)
{
	返回 (uint32_t)(uintptr_t)mark;
}

static inline void mark_object(struct object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

static inline void mark_next_object(struct object *object)
{
	mark_object(object, ++last_idnum);
}

static 整数 get_object_mark(struct object *object)
{
	void *decoration = lookup_decoration(&idnums, object);
	如果 (!decoration)
		返回 0;
	返回 ptr_to_mark(decoration);
}

static struct commit *rewrite_commit(struct commit *p)
{
	为 (;;) {
		如果 (p->parents && p->parents->next)
			break;
		如果 (p->object.flags & UNINTERESTING)
			break;
		如果 (!(p->object.flags & TREESAME))
			break;
		如果 (!p->parents)
			返回 NULL;
		p = p->parents->item;
	}
	返回 p;
}

static void show_progress(void)
{
	static 整数 counter = 0;
	如果 (!progress)
		返回;
	如果 ((++counter % progress) == 0)
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
static char *anonymize_blob(unsigned long *size)
{
	static 整数 counter;
	struct strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	返回 strbuf_detach(&out, NULL);
}

static void export_blob(const struct object_id *oid)
{
	unsigned long size;
	enum object_type type;
	char *buf;
	struct object *object;
	整数 eaten;

	如果 (no_data)
		返回;

	如果 (is_null_oid(oid))
		返回;

	object = lookup_object(the_repository, oid);
	如果 (object && object->flags & SHOWN)
		返回;

	如果 (anonymize) {
		buf = anonymize_blob(&size);
		object = (struct object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} 否则 {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		如果 (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		如果 (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	如果 (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	如果 (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	如果 (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	如果 (!eaten)
		free(buf);
}

static 整数 depth_first(const void *a_, const void *b_)
{
	const struct diff_filepair *a = *((const struct diff_filepair **)a_);
	const struct diff_filepair *b = *((const struct diff_filepair **)b_);
	const char *name_a, *name_b;
	整数 len_a, len_b, len;
	整数 cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	如果 (cmp)
		返回 cmp;
	cmp = len_b - len_a;
	如果 (cmp)
		返回 cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	返回 (a->status == 'R') - (b->status == 'R');
}

static void print_path_1(const char *path)
{
	整数 need_quote = quote_c_style(path, NULL, NULL, 0);
	如果 (need_quote)
		quote_c_style(path, NULL, stdout, 0);
	否则 如果 (strchr(path, ' '))
		printf("\"%s\"", path);
	否则
		printf("%s", path);
}

static char *anonymize_path_component(void)
{
	static 整数 counter;
	struct strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	返回 strbuf_detach(&out, NULL);
}

static void print_path(const char *path)
{
	如果 (!anonymize)
		print_path_1(path);
	否则 {
		static struct hashmap paths;
		static struct strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

static char *generate_fake_oid(void)
{
	static uint32_t counter = 1; /* avoid null oid */
	const unsigned hashsz = the_hash_algo->rawsz;
	struct object_id oid;
	char *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	返回 oid_to_hex_r(hex, &oid);
}

static const char *anonymize_oid(const char *oid_hex)
{
	static struct hashmap objs;
	size_t len = strlen(oid_hex);
	返回 anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

static void show_filemodify(struct diff_queue_struct *q,
			    struct diff_options *options UNUSED, void *data)
{
	整数 i;
	struct string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	为 (i = 0; i < q->nr; i++) {
		struct diff_filespec *ospec = q->queue[i]->one;
		struct diff_filespec *spec = q->queue[i]->two;

		switch (q->queue[i]->status) {
		case DIFF_STATUS_DELETED:
			printf("D ");
			print_path(spec->path);
			string_list_insert(changed, spec->path);
			putchar('\n');
			break;

		case DIFF_STATUS_COPIED:
		case DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			如果 (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				print_path(ospec->path);
				putchar(' ');
				print_path(spec->path);
				string_list_insert(changed, spec->path);
				putchar('\n');

				如果 (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					break;
			}
			/* fallthrough */

		case DIFF_STATUS_TYPE_CHANGED:
		case DIFF_STATUS_MODIFIED:
		case DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			如果 (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			否则 {
				struct object *object = lookup_object(the_repository,
								      &spec->oid);
				printf("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_insert(changed, spec->path);
			putchar('\n');
			break;

		default:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

static char *anonymize_ref_component(void)
{
	static 整数 counter;
	struct strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	返回 strbuf_detach(&out, NULL);
}

static const char *anonymize_refname(const char *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	static const char *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	static struct hashmap refs;
	static struct strbuf anon = STRBUF_INIT;
	整数 i;

	strbuf_reset(&anon);
	为 (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		如果 (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			break;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	返回 anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
static char *anonymize_commit_message(void)
{
	static 整数 counter;
	返回 xstrfmt("subject %d\n\nbody\n", counter++);
}

static char *anonymize_ident(void)
{
	static 整数 counter;
	struct strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	返回 strbuf_detach(&out, NULL);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
static void anonymize_ident_line(const char **beg, const char **end)
{
	static struct hashmap idents;
	static struct strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	static unsigned which_buffer;

	struct strbuf *out;
	struct ident_split split;
	const char *end_of_header;

	out = &buffers[which_buffer++];
	which_buffer %= ARRAY_SIZE(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = strchr(*beg, ' ');
	如果 (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (整数)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	如果 (!split_ident_line(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		const char *ident;
		size_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} 否则 {
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
static const char *find_commit_multiline_header(const char *msg,
						const char *key,
						const char **end)
{
	struct strbuf val = STRBUF_INIT;
	const char *bol, *eol;
	size_t len;

	bol = find_commit_header(msg, key, &len);
	如果 (!bol)
		返回 NULL;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	当 (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	返回 strbuf_detach(&val, NULL);
}

static void print_signature(const char *signature, const char *object_hash)
{
	如果 (!signature)
		返回;

	printf("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (unsigned)strlen(signature),
	       signature);
}

static const char *append_signatures_for_header(struct string_list *signatures,
						const char *pos,
						const char *header,
						const char *object_hash)
{
	const char *signature;
	const char *start = pos;
	const char *end = pos;

	当 ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (void *)object_hash;
		free((char *)signature);
		start = end;
	}

	返回 end;
}

static void handle_commit(struct commit *commit, struct rev_info *rev,
			  struct string_list *paths_of_changed_objects)
{
	整数 saved_output_format = rev->diffopt.output_format;
	const char *commit_buffer, *commit_buffer_cursor;
	const char *author, *author_end, *committer, *committer_end;
	const char *encoding = NULL;
	size_t encoding_len;
	struct string_list signatures = STRING_LIST_INIT_DUP;
	const char *message;
	char *reencoded = NULL;
	struct commit_list *p;
	const char *refname;
	整数 i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, NULL);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	如果 (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	如果 (!committer)
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

	如果 (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		如果 (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	如果 (*commit_buffer_cursor == '\n') {
		const char *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		const char *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
	如果 (message)
		message += 2;

	如果 (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_tree) {
		parse_commit_or_die(commit->parents->item);
		diff_tree_oid(get_commit_tree_oid(commit->parents->item),
			      get_commit_tree_oid(commit), "", &rev->diffopt);
	}
	否则
		diff_root_tree_oid(get_commit_tree_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	为 (i = 0; i < diff_queued_diff.nr; i++)
		如果 (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	如果 (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_line(&committer, &committer_end);
		anonymize_ident_line(&author, &author_end);
	}

	mark_next_object(&commit->object);
	如果 (anonymize) {
		reencoded = anonymize_commit_message();
	} 否则 如果 (encoding) {
		char *buf;
		switch (reencode_mode) {
		case REENCODE_YES:
			buf = xstrfmt("%.*s", (整数)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			free(buf);
			break;
		case REENCODE_NO:
			break;
		case REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (整数)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	如果 (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	如果 (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (整数)(author_end - author), author,
	       (整数)(committer_end - committer), committer);
	如果 (signatures.nr) {
		switch (signed_commit_mode) {
		/* Exporting modes */
		case SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		case SIGN_VERBATIM:
			为 (size_t i = 0; i < signatures.nr; i++) {
				struct string_list_item *item = &signatures.items[i];
				print_signature(item->string, item->util);
			}
			break;

		/* Stripping modes */
		case SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		case SIGN_STRIP:
			break;

		/* Aborting modes */
		case SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		default:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	如果 (!reencoded && encoding)
		printf("encoding %.*s\n", (整数)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	free(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	为 (i = 0, p = commit->parents; p; p = p->next) {
		struct object *obj = &p->item->object;
		整数 mark = get_object_mark(obj);

		如果 (!mark && !reference_excluded_commits)
			continue;
		如果 (i == 0)
			printf("from ");
		否则
			printf("merge ");
		如果 (mark)
			printf(":%d\n", mark);
		否则
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	如果 (full_tree)
		printf("deleteall\n");
	log_tree_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

static char *anonymize_tag(void)
{
	static 整数 counter;
	struct strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	返回 strbuf_detach(&out, NULL);
}


static void handle_tag(const char *name, struct tag *tag)
{
	size_t size;
	enum object_type type;
	char *buf;
	const char *tagger, *tagger_end, *message;
	size_t message_size = 0;
	struct object *tagged;
	整数 tagged_mark;
	struct commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	当 (tagged->type == OBJ_TAG) {
		tagged = ((struct tag *)tagged)->tagged;
	}
	如果 (tagged->type == OBJ_TREE) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		返回;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	如果 (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	如果 (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	如果 (!tagger) {
		如果 (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		否则
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} 否则 {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		如果 (anonymize)
			anonymize_ident_line(&tagger, &tagger_end);
	}

	如果 (anonymize) {
		name = anonymize_refname(name);
		如果 (message) {
			static struct hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	如果 (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		如果 (sig_offset < message_size)
			switch (signed_tag_mode) {
			/* Exporting modes */
			case SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			case SIGN_VERBATIM:
				break;

			/* Stripping modes */
			case SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			case SIGN_STRIP:
				message_size = sig_offset;
				break;

			/* Aborting modes */
			case SIGN_ABORT:
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
	如果 (!tagged_mark) {
		switch (tag_of_filtered_mode) {
		case TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		case DROP:
			/* Ignore this tag altogether */
			free(buf);
			返回;
		case REWRITE:
			如果 (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} 否则 如果 (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((struct commit *)tagged);
				如果 (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					free(buf);
					返回;
				}
				tagged_mark = get_object_mark(&p->object);
			} 否则 {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	如果 (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	如果 (mark_tags) {
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	如果 (tagged_mark)
		printf("from :%d\n", tagged_mark);
	否则
		printf("from %s\n", oid_to_hex(&tagged->oid));

	如果 (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (整数)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (整数)message_size, (整数)message_size, message ? message : "");
	free(buf);
}

static struct commit *get_commit(struct rev_cmdline_entry *e, const char *full_name)
{
	switch (e->item->type) {
	case OBJ_COMMIT:
		返回 (struct commit *)e->item;
	case OBJ_TAG: {
		struct tag *tag = (struct tag *)e->item;

		/* handle nested tags */
		当 (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (struct tag *)tag->tagged;
		}
		如果 (!tag)
			die(_("tag %s points nowhere?"), e->name);
		返回 (struct commit *)tag;
	}
	default:
		返回 NULL;
	}
}

static void get_tags_and_duplicates(struct rev_cmdline_info *info)
{
	整数 i;

	为 (i = 0; i < info->nr; i++) {
		struct rev_cmdline_entry *e = info->rev + i;
		struct object_id oid;
		struct commit *commit;
		char *full_name = NULL;

		如果 (e->flags & UNINTERESTING)
			continue;

		如果 (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			free(full_name);
			continue;
		}

		如果 (refspecs.nr) {
			char *private;
			private = apply_refspecs(&refspecs, full_name);
			如果 (private) {
				free(full_name);
				full_name = private;
			}
		}

		commit = get_commit(e, full_name);
		如果 (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			free(full_name);
			continue;
		}

		switch (commit->object.type) {
		case OBJ_COMMIT:
			break;
		case OBJ_BLOB:
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
		如果 (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		如果 (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		否则
			free(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

static void handle_tags_and_duplicates(struct string_list *extras)
{
	struct commit *commit;
	整数 i;

	为 (i = extras->nr - 1; i >= 0; i--) {
		const char *name = extras->items[i].string;
		struct object *object = extras->items[i].util;
		整数 mark;

		switch (object->type) {
		case OBJ_TAG:
			handle_tag(name, (struct tag *)object);
			break;
		case OBJ_COMMIT:
			如果 (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((struct commit *)object);
			如果 (!commit) {
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
			如果 (!mark) {
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
				如果 (!reference_excluded_commits) {
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
			break;
		}
	}
}

static void export_marks(char *file)
{
	unsigned 整数 i;
	uint32_t mark;
	struct decoration_entry *deco = idnums.entries;
	FILE *f;
	整数 e = 0;

	f = fopen_for_writing(file);
	如果 (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	为 (i = 0; i < idnums.size; i++) {
		如果 (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			如果 (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    break;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	如果 (e)
		error(_("unable to write marks file %s."), file);
}

static void import_marks(char *input_file, 整数 check_exists)
{
	char line[512];
	FILE *f;
	struct stat sb;

	如果 (check_exists && stat(input_file, &sb))
		返回;

	f = xfopen(input_file, "r");
	当 (fgets(line, sizeof(line), f)) {
		uint32_t mark;
		char *line_end, *mark_end;
		struct object_id oid;
		struct object *object;
		struct commit *commit;
		enum object_type type;

		line_end = strchr(line, '\n');
		如果 (line[0] != ':' || !line_end)
			die(_("corrupt mark line: %s"), line);
		*line_end = '\0';

		mark = strtoumax(line + 1, &mark_end, 10);
		如果 (!mark || mark_end == line + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), line);

		如果 (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, NULL);
		如果 (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		如果 (type != OBJ_COMMIT)
			/* only commits */
			continue;

		commit = lookup_commit(the_repository, &oid);
		如果 (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		如果 (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

static void handle_deletes(void)
{
	整数 i;
	为 (i = 0; i < refspecs.nr; i++) {
		struct refspec_item *refspec = &refspecs.items[i];
		如果 (*refspec->src)
			continue;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

static 整数 parse_opt_anonymize_map(const struct option *opt,
				   const char *arg, 整数 unset)
{
	struct hashmap *map = opt->value;
	const char *delim, *value;
	size_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	如果 (delim) {
		keylen = delim - arg;
		value = delim + 1;
	} 否则 {
		keylen = strlen(arg);
		value = arg;
	}

	如果 (!keylen || !*value)
		返回 error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(value));

	返回 0;
}

整数 cmd_fast_export(整数 argc,
		    const char **argv,
		    const char *prefix,
		    struct repository *repo UNUSED)
{
	struct rev_info revs;
	struct commit *commit;
	char *export_filename = NULL,
	     *import_filename = NULL,
	     *import_filename_if_exists = NULL;
	uint32_t lastimportid;
	struct string_list refspecs_list = STRING_LIST_INIT_NODUP;
	struct string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	struct option options[] = {
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

	如果 (argc == 1)
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
	如果 (argc > 1)
		usage_with_options (fast_export_usage, options);

	如果 (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	如果 (refspecs_list.nr) {
		整数 i;

		为 (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.items[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	如果 (use_done_feature)
		printf("feature done\n");

	如果 (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	如果 (import_filename)
		import_marks(import_filename, 0);
	否则 如果 (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	如果 (import_filename && revs.prune_data.nr)
		full_tree = 1;

	get_tags_and_duplicates(&revs.cmdline);

	如果 (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	当 ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	如果 (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	如果 (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	返回 0;
}
