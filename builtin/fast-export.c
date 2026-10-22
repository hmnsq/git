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

static ціле progress;
static enum sign_mode signed_tag_mode = SIGN_ABORT;
static enum sign_mode signed_commit_mode = SIGN_STRIP;
static enum tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
static enum reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
static ціле fake_missing_tagger;
static ціле use_done_feature;
static ціле no_data;
static ціле full_tree;
static ціле reference_excluded_commits;
static ціле show_original_ids;
static ціле mark_tags;
static struct string_list extra_refs = STRING_LIST_INIT_DUP;
static struct string_list tag_refs = STRING_LIST_INIT_DUP;
static struct refspec refspecs;
static ціле anonymize;
static struct hashmap anonymized_seeds;
static struct revision_sources revision_sources;

static ціле parse_opt_sign_mode(const struct option *opt,
			       const char *arg, ціле unset)
{
	enum sign_mode *val = opt->value;

	якщо (unset)
		повернути 0;

	якщо (parse_sign_mode(arg, val, NULL) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		повернути error(_("unknown %s mode: %s"), opt->long_name, arg);

	повернути 0;
}

static ціле parse_opt_tag_of_filtered_mode(const struct option *opt,
					  const char *arg, ціле unset)
{
	enum tag_of_filtered_mode *val = opt->value;

	якщо (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	інакше якщо (!strcmp(arg, "drop"))
		*val = DROP;
	інакше якщо (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	інакше
		повернути error(_("unknown tag-of-filtered mode: %s"), arg);
	повернути 0;
}

static ціле parse_opt_reencode_mode(const struct option *opt,
				   const char *arg, ціле unset)
{
	enum reencode_mode *val = opt->value;

	якщо (unset) {
		*val = REENCODE_ABORT;
		повернути 0;
	}

	switch (git_parse_maybe_bool(arg)) {
	case 0:
		*val = REENCODE_NO;
		break;
	case 1:
		*val = REENCODE_YES;
		break;
	default:
		якщо (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		інакше
			повернути error(_("unknown reencoding mode: %s"), arg);
	}

	повернути 0;
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

static ціле anonymized_entry_cmp(const void *cmp_data UNUSED,
				const struct hashmap_entry *eptr,
				const struct hashmap_entry *entry_or_key,
				const void *keydata)
{
	const struct anonymized_entry *a, *b;

	a = container_of(eptr, const struct anonymized_entry, hash);
	якщо (keydata) {
		const struct anonymized_entry_key *key = keydata;
		ціле equal = !xstrncmpz(a->orig, key->orig, key->orig_len);
		повернути !equal;
	}

	b = container_of(entry_or_key, const struct anonymized_entry, hash);
	повернути strcmp(a->orig, b->orig);
}

static struct anonymized_entry *add_anonymized_entry(struct hashmap *map,
						     unsigned hash,
						     const char *orig, size_t len,
						     char *anon)
{
	struct anonymized_entry *ret, *old;

	якщо (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, NULL, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	якщо (old) {
		free(old->anon);
		free(old);
	}

	повернути ret;
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
	якщо (!ret)
		ret = hashmap_get_entry(map, &key, hash, &key);

	/* ...and finally generate a new mapping if necessary */
	якщо (!ret)
		ret = add_anonymized_entry(map, key.hash.hash,
					   orig, len, generate());

	повернути ret->anon;
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
	поки (*path) {
		const char *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		const char *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		якщо (*path)
			strbuf_addch(out, *path++);
	}
}

static inline void *mark_to_ptr(uint32_t mark)
{
	повернути (void *)(uintptr_t)mark;
}

static inline uint32_t ptr_to_mark(void * mark)
{
	повернути (uint32_t)(uintptr_t)mark;
}

static inline void mark_object(struct object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

static inline void mark_next_object(struct object *object)
{
	mark_object(object, ++last_idnum);
}

static ціле get_object_mark(struct object *object)
{
	void *decoration = lookup_decoration(&idnums, object);
	якщо (!decoration)
		повернути 0;
	повернути ptr_to_mark(decoration);
}

static struct commit *rewrite_commit(struct commit *p)
{
	для (;;) {
		якщо (p->parents && p->parents->next)
			break;
		якщо (p->object.flags & UNINTERESTING)
			break;
		якщо (!(p->object.flags & TREESAME))
			break;
		якщо (!p->parents)
			повернути NULL;
		p = p->parents->item;
	}
	повернути p;
}

static void show_progress(void)
{
	static ціле counter = 0;
	якщо (!progress)
		повернути;
	якщо ((++counter % progress) == 0)
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
	static ціле counter;
	struct strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	повернути strbuf_detach(&out, NULL);
}

static void export_blob(const struct object_id *oid)
{
	unsigned long size;
	enum object_type type;
	char *buf;
	struct object *object;
	ціле eaten;

	якщо (no_data)
		повернути;

	якщо (is_null_oid(oid))
		повернути;

	object = lookup_object(the_repository, oid);
	якщо (object && object->flags & SHOWN)
		повернути;

	якщо (anonymize) {
		buf = anonymize_blob(&size);
		object = (struct object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} інакше {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		якщо (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		якщо (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	якщо (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	якщо (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	якщо (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	якщо (!eaten)
		free(buf);
}

static ціле depth_first(const void *a_, const void *b_)
{
	const struct diff_filepair *a = *((const struct diff_filepair **)a_);
	const struct diff_filepair *b = *((const struct diff_filepair **)b_);
	const char *name_a, *name_b;
	ціле len_a, len_b, len;
	ціле cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	якщо (cmp)
		повернути cmp;
	cmp = len_b - len_a;
	якщо (cmp)
		повернути cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	повернути (a->status == 'R') - (b->status == 'R');
}

static void print_path_1(const char *path)
{
	ціле need_quote = quote_c_style(path, NULL, NULL, 0);
	якщо (need_quote)
		quote_c_style(path, NULL, stdout, 0);
	інакше якщо (strchr(path, ' '))
		printf("\"%s\"", path);
	інакше
		printf("%s", path);
}

static char *anonymize_path_component(void)
{
	static ціле counter;
	struct strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	повернути strbuf_detach(&out, NULL);
}

static void print_path(const char *path)
{
	якщо (!anonymize)
		print_path_1(path);
	інакше {
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
	повернути oid_to_hex_r(hex, &oid);
}

static const char *anonymize_oid(const char *oid_hex)
{
	static struct hashmap objs;
	size_t len = strlen(oid_hex);
	повернути anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

static void show_filemodify(struct diff_queue_struct *q,
			    struct diff_options *options UNUSED, void *data)
{
	ціле i;
	struct string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	для (i = 0; i < q->nr; i++) {
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
			якщо (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				print_path(ospec->path);
				putchar(' ');
				print_path(spec->path);
				string_list_insert(changed, spec->path);
				putchar('\n');

				якщо (oideq(&ospec->oid, &spec->oid) &&
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
			якщо (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			інакше {
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
	static ціле counter;
	struct strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	повернути strbuf_detach(&out, NULL);
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
	ціле i;

	strbuf_reset(&anon);
	для (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		якщо (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			break;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	повернути anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
static char *anonymize_commit_message(void)
{
	static ціле counter;
	повернути xstrfmt("subject %d\n\nbody\n", counter++);
}

static char *anonymize_ident(void)
{
	static ціле counter;
	struct strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	повернути strbuf_detach(&out, NULL);
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
	якщо (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (ціле)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	якщо (!split_ident_line(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		const char *ident;
		size_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} інакше {
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
	якщо (!bol)
		повернути NULL;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	поки (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	повернути strbuf_detach(&val, NULL);
}

static void print_signature(const char *signature, const char *object_hash)
{
	якщо (!signature)
		повернути;

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

	поки ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (void *)object_hash;
		free((char *)signature);
		start = end;
	}

	повернути end;
}

static void handle_commit(struct commit *commit, struct rev_info *rev,
			  struct string_list *paths_of_changed_objects)
{
	ціле saved_output_format = rev->diffopt.output_format;
	const char *commit_buffer, *commit_buffer_cursor;
	const char *author, *author_end, *committer, *committer_end;
	const char *encoding = NULL;
	size_t encoding_len;
	struct string_list signatures = STRING_LIST_INIT_DUP;
	const char *message;
	char *reencoded = NULL;
	struct commit_list *p;
	const char *refname;
	ціле i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, NULL);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	якщо (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	якщо (!committer)
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

	якщо (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		якщо (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	якщо (*commit_buffer_cursor == '\n') {
		const char *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		const char *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
	якщо (message)
		message += 2;

	якщо (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_tree) {
		parse_commit_or_die(commit->parents->item);
		diff_tree_oid(get_commit_tree_oid(commit->parents->item),
			      get_commit_tree_oid(commit), "", &rev->diffopt);
	}
	інакше
		diff_root_tree_oid(get_commit_tree_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	для (i = 0; i < diff_queued_diff.nr; i++)
		якщо (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	якщо (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_line(&committer, &committer_end);
		anonymize_ident_line(&author, &author_end);
	}

	mark_next_object(&commit->object);
	якщо (anonymize) {
		reencoded = anonymize_commit_message();
	} інакше якщо (encoding) {
		char *buf;
		switch (reencode_mode) {
		case REENCODE_YES:
			buf = xstrfmt("%.*s", (ціле)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			free(buf);
			break;
		case REENCODE_NO:
			break;
		case REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (ціле)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	якщо (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	якщо (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (ціле)(author_end - author), author,
	       (ціле)(committer_end - committer), committer);
	якщо (signatures.nr) {
		switch (signed_commit_mode) {
		/* Exporting modes */
		case SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		case SIGN_VERBATIM:
			для (size_t i = 0; i < signatures.nr; i++) {
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
	якщо (!reencoded && encoding)
		printf("encoding %.*s\n", (ціле)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	free(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	для (i = 0, p = commit->parents; p; p = p->next) {
		struct object *obj = &p->item->object;
		ціле mark = get_object_mark(obj);

		якщо (!mark && !reference_excluded_commits)
			continue;
		якщо (i == 0)
			printf("from ");
		інакше
			printf("merge ");
		якщо (mark)
			printf(":%d\n", mark);
		інакше
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	якщо (full_tree)
		printf("deleteall\n");
	log_tree_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

static char *anonymize_tag(void)
{
	static ціле counter;
	struct strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	повернути strbuf_detach(&out, NULL);
}


static void handle_tag(const char *name, struct tag *tag)
{
	size_t size;
	enum object_type type;
	char *buf;
	const char *tagger, *tagger_end, *message;
	size_t message_size = 0;
	struct object *tagged;
	ціле tagged_mark;
	struct commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	поки (tagged->type == OBJ_TAG) {
		tagged = ((struct tag *)tagged)->tagged;
	}
	якщо (tagged->type == OBJ_TREE) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		повернути;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	якщо (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	якщо (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	якщо (!tagger) {
		якщо (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		інакше
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} інакше {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		якщо (anonymize)
			anonymize_ident_line(&tagger, &tagger_end);
	}

	якщо (anonymize) {
		name = anonymize_refname(name);
		якщо (message) {
			static struct hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	якщо (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		якщо (sig_offset < message_size)
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
	якщо (!tagged_mark) {
		switch (tag_of_filtered_mode) {
		case TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		case DROP:
			/* Ignore this tag altogether */
			free(buf);
			повернути;
		case REWRITE:
			якщо (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} інакше якщо (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((struct commit *)tagged);
				якщо (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					free(buf);
					повернути;
				}
				tagged_mark = get_object_mark(&p->object);
			} інакше {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	якщо (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	якщо (mark_tags) {
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	якщо (tagged_mark)
		printf("from :%d\n", tagged_mark);
	інакше
		printf("from %s\n", oid_to_hex(&tagged->oid));

	якщо (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (ціле)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (ціле)message_size, (ціле)message_size, message ? message : "");
	free(buf);
}

static struct commit *get_commit(struct rev_cmdline_entry *e, const char *full_name)
{
	switch (e->item->type) {
	case OBJ_COMMIT:
		повернути (struct commit *)e->item;
	case OBJ_TAG: {
		struct tag *tag = (struct tag *)e->item;

		/* handle nested tags */
		поки (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (struct tag *)tag->tagged;
		}
		якщо (!tag)
			die(_("tag %s points nowhere?"), e->name);
		повернути (struct commit *)tag;
	}
	default:
		повернути NULL;
	}
}

static void get_tags_and_duplicates(struct rev_cmdline_info *info)
{
	ціле i;

	для (i = 0; i < info->nr; i++) {
		struct rev_cmdline_entry *e = info->rev + i;
		struct object_id oid;
		struct commit *commit;
		char *full_name = NULL;

		якщо (e->flags & UNINTERESTING)
			continue;

		якщо (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			free(full_name);
			continue;
		}

		якщо (refspecs.nr) {
			char *private;
			private = apply_refspecs(&refspecs, full_name);
			якщо (private) {
				free(full_name);
				full_name = private;
			}
		}

		commit = get_commit(e, full_name);
		якщо (!commit) {
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
		якщо (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		якщо (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		інакше
			free(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

static void handle_tags_and_duplicates(struct string_list *extras)
{
	struct commit *commit;
	ціле i;

	для (i = extras->nr - 1; i >= 0; i--) {
		const char *name = extras->items[i].string;
		struct object *object = extras->items[i].util;
		ціле mark;

		switch (object->type) {
		case OBJ_TAG:
			handle_tag(name, (struct tag *)object);
			break;
		case OBJ_COMMIT:
			якщо (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((struct commit *)object);
			якщо (!commit) {
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
			якщо (!mark) {
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
				якщо (!reference_excluded_commits) {
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
	unsigned ціле i;
	uint32_t mark;
	struct decoration_entry *deco = idnums.entries;
	FILE *f;
	ціле e = 0;

	f = fopen_for_writing(file);
	якщо (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	для (i = 0; i < idnums.size; i++) {
		якщо (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			якщо (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    break;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	якщо (e)
		error(_("unable to write marks file %s."), file);
}

static void import_marks(char *input_file, ціле check_exists)
{
	char line[512];
	FILE *f;
	struct stat sb;

	якщо (check_exists && stat(input_file, &sb))
		повернути;

	f = xfopen(input_file, "r");
	поки (fgets(line, sizeof(line), f)) {
		uint32_t mark;
		char *line_end, *mark_end;
		struct object_id oid;
		struct object *object;
		struct commit *commit;
		enum object_type type;

		line_end = strchr(line, '\n');
		якщо (line[0] != ':' || !line_end)
			die(_("corrupt mark line: %s"), line);
		*line_end = '\0';

		mark = strtoumax(line + 1, &mark_end, 10);
		якщо (!mark || mark_end == line + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), line);

		якщо (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, NULL);
		якщо (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		якщо (type != OBJ_COMMIT)
			/* only commits */
			continue;

		commit = lookup_commit(the_repository, &oid);
		якщо (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		якщо (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

static void handle_deletes(void)
{
	ціле i;
	для (i = 0; i < refspecs.nr; i++) {
		struct refspec_item *refspec = &refspecs.items[i];
		якщо (*refspec->src)
			continue;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

static ціле parse_opt_anonymize_map(const struct option *opt,
				   const char *arg, ціле unset)
{
	struct hashmap *map = opt->value;
	const char *delim, *value;
	size_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	якщо (delim) {
		keylen = delim - arg;
		value = delim + 1;
	} інакше {
		keylen = strlen(arg);
		value = arg;
	}

	якщо (!keylen || !*value)
		повернути error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(value));

	повернути 0;
}

ціле cmd_fast_export(ціле argc,
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

	якщо (argc == 1)
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
	якщо (argc > 1)
		usage_with_options (fast_export_usage, options);

	якщо (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	якщо (refspecs_list.nr) {
		ціле i;

		для (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.items[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	якщо (use_done_feature)
		printf("feature done\n");

	якщо (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	якщо (import_filename)
		import_marks(import_filename, 0);
	інакше якщо (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	якщо (import_filename && revs.prune_data.nr)
		full_tree = 1;

	get_tags_and_duplicates(&revs.cmdline);

	якщо (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	поки ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	якщо (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	якщо (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	повернути 0;
}
