/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "refspec.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "object.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "diffcore.h"
#ଅନ୍ତର୍ଭାଵ "log-tree.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "decorate.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "blob.h"
#ଅନ୍ତର୍ଭାଵ "gpg-interface.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress;
ସ୍ଥିର ଗଣନା sign_mode signed_tag_mode = SIGN_ABORT;
ସ୍ଥିର ଗଣନା sign_mode signed_commit_mode = SIGN_STRIP;
ସ୍ଥିର ଗଣନା tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
ସ୍ଥିର ଗଣନା reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fake_missing_tagger;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_done_feature;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_data;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା full_ବୃକ୍ଷ;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reference_excluded_commits;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_original_ids;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_tags;
ସ୍ଥିର ସଂରଚନା string_list extra_refs = STRING_LIST_INIT_DUP;
ସ୍ଥିର ସଂରଚନା string_list tag_refs = STRING_LIST_INIT_DUP;
ସ୍ଥିର ସଂରଚନା refspec refspecs;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା anonymize;
ସ୍ଥିର ସଂରଚନା hashmap anonymized_seeds;
ସ୍ଥିର ସଂରଚନା revision_sources revision_sources;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_opt_sign_mode(ଅଚଳ ସଂରଚନା option *opt,
			       ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଗଣନା sign_mode *val = opt->ମୂଲ୍ୟ;

	ଯଦି (unset)
		ଫେରନ୍ତୁ 0;

	ଯଦି (parse_sign_mode(arg, val, ଖାଲି) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		ଫେରନ୍ତୁ error(_("unknown %s mode: %s"), opt->long_name, arg);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_opt_tag_of_filtered_mode(ଅଚଳ ସଂରଚନା option *opt,
					  ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଗଣନା tag_of_filtered_mode *val = opt->ମୂଲ୍ୟ;

	ଯଦି (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "drop"))
		*val = DROP;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("unknown tag-of-filtered mode: %s"), arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_opt_reencode_mode(ଅଚଳ ସଂରଚନା option *opt,
				   ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଗଣନା reencode_mode *val = opt->ମୂଲ୍ୟ;

	ଯଦି (unset) {
		*val = REENCODE_ABORT;
		ଫେରନ୍ତୁ 0;
	}

	ବାଛ (git_parse_maybe_bool(arg)) {
	ସ୍ଥିତି 0:
		*val = REENCODE_NO;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି 1:
		*val = REENCODE_YES;
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		ଯଦି (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		ଅନ୍ୟଥା
			ଫେରନ୍ତୁ error(_("unknown reencoding mode: %s"), arg);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା decoration idnums;
ସ୍ଥିର uint32_t last_idnum;
ସଂରଚନା anonymized_entry {
	ସଂରଚନା hashmap_entry hash;
	ଅକ୍ଷର *anon;
	ଅଚଳ ଅକ୍ଷର orig[FLEX_ARRAY];
};

ସଂରଚନା anonymized_entry_ଚାବି {
	ସଂରଚନା hashmap_entry hash;
	ଅଚଳ ଅକ୍ଷର *orig;
	size_t orig_len;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା anonymized_entry_cmp(ଅଚଳ ଶୂନ୍ୟ *cmp_data UNUSED,
				ଅଚଳ ସଂରଚନା hashmap_entry *eptr,
				ଅଚଳ ସଂରଚନା hashmap_entry *entry_or_ଚାବି,
				ଅଚଳ ଶୂନ୍ୟ *keydata)
{
	ଅଚଳ ସଂରଚନା anonymized_entry *a, *b;

	a = container_of(eptr, ଅଚଳ ସଂରଚନା anonymized_entry, hash);
	ଯଦି (keydata) {
		ଅଚଳ ସଂରଚନା anonymized_entry_ଚାବି *ଚାବି = keydata;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା equal = !xstrncmpz(a->orig, ଚାବି->orig, ଚାବି->orig_len);
		ଫେରନ୍ତୁ !equal;
	}

	b = container_of(entry_or_ଚାବି, ଅଚଳ ସଂରଚନା anonymized_entry, hash);
	ଫେରନ୍ତୁ strcmp(a->orig, b->orig);
}

ସ୍ଥିର ସଂରଚନା anonymized_entry *add_anonymized_entry(ସଂରଚନା hashmap *map,
						     unsigned hash,
						     ଅଚଳ ଅକ୍ଷର *orig, size_t len,
						     ଅକ୍ଷର *anon)
{
	ସଂରଚନା anonymized_entry *ret, *old;

	ଯଦି (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, ଖାଲି, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	ଯଦି (old) {
		ମୁକ୍ତ(old->anon);
		ମୁକ୍ତ(old);
	}

	ଫେରନ୍ତୁ ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *anonymize_str(ସଂରଚନା hashmap *map,
				 ଅକ୍ଷର *(*generate)(ଶୂନ୍ୟ),
				 ଅଚଳ ଅକ୍ଷର *orig, size_t len)
{
	ସଂରଚନା anonymized_entry_ଚାବି ଚାବି;
	ସଂରଚନା anonymized_entry *ret;

	hashmap_entry_init(&ଚାବି.hash, memhash(orig, len));
	ଚାବି.orig = orig;
	ଚାବି.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &ଚାବି, hash, &ଚାବି);

	/* ...otherwise check if we've already seen it in this context... */
	ଯଦି (!ret)
		ret = hashmap_get_entry(map, &ଚାବି, hash, &ଚାବି);

	/* ...and finally generate a new mapping if necessary */
	ଯଦି (!ret)
		ret = add_anonymized_entry(map, ଚାବି.hash.hash,
					   orig, len, generate());

	ଫେରନ୍ତୁ ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
ସ୍ଥିର ଶୂନ୍ୟ anonymize_path(ସଂରଚନା strbuf *out, ଅଚଳ ଅକ୍ଷର *path,
			   ସଂରଚନା hashmap *map,
			   ଅକ୍ଷର *(*generate)(ଶୂନ୍ୟ))
{
	ଯେତେବେଳେ (*path) {
		ଅଚଳ ଅକ୍ଷର *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		ଅଚଳ ଅକ୍ଷର *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		ଯଦି (*path)
			strbuf_addch(out, *path++);
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ *mark_to_ptr(uint32_t mark)
{
	ଫେରନ୍ତୁ (ଶୂନ୍ୟ *)(uintptr_t)mark;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint32_t ptr_to_mark(ଶୂନ୍ୟ * mark)
{
	ଫେରନ୍ତୁ (uint32_t)(uintptr_t)mark;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ mark_object(ସଂରଚନା object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ mark_next_object(ସଂରଚନା object *object)
{
	mark_object(object, ++last_idnum);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_object_mark(ସଂରଚନା object *object)
{
	ଶୂନ୍ୟ *decoration = lookup_decoration(&idnums, object);
	ଯଦି (!decoration)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ ptr_to_mark(decoration);
}

ସ୍ଥିର ସଂରଚନା commit *rewrite_commit(ସଂରଚନା commit *p)
{
	ପାଇଁ (;;) {
		ଯଦି (p->parents && p->parents->next)
			ଭାଙ୍ଗ;
		ଯଦି (p->object.flags & UNINTERESTING)
			ଭାଙ୍ଗ;
		ଯଦି (!(p->object.flags & TREESAME))
			ଭାଙ୍ଗ;
		ଯଦି (!p->parents)
			ଫେରନ୍ତୁ ଖାଲି;
		p = p->parents->item;
	}
	ଫେରନ୍ତୁ p;
}

ସ୍ଥିର ଶୂନ୍ୟ show_progress(ଶୂନ୍ୟ)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା counter = 0;
	ଯଦି (!progress)
		ଫେରନ୍ତୁ;
	ଯଦି ((++counter % progress) == 0)
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
ସ୍ଥିର ଅକ୍ଷର *anonymize_blob(unsigned long *size)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା counter;
	ସଂରଚନା strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	ଫେରନ୍ତୁ strbuf_detach(&out, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ export_blob(ଅଚଳ ସଂରଚନା object_id *oid)
{
	unsigned long size;
	ଗଣନା object_type type;
	ଅକ୍ଷର *buf;
	ସଂରଚନା object *object;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା eaten;

	ଯଦି (no_data)
		ଫେରନ୍ତୁ;

	ଯଦି (is_null_oid(oid))
		ଫେରନ୍ତୁ;

	object = lookup_object(the_repository, oid);
	ଯଦି (object && object->flags & SHOWN)
		ଫେରନ୍ତୁ;

	ଯଦି (anonymize) {
		buf = anonymize_blob(&size);
		object = (ସଂରଚନା object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} ଅନ୍ୟଥା {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		ଯଦି (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		ଯଦି (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	ଯଦି (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	ଯଦି (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	ଯଦି (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	ଯଦି (!eaten)
		ମୁକ୍ତ(buf);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth_first(ଅଚଳ ଶୂନ୍ୟ *a_, ଅଚଳ ଶୂନ୍ୟ *b_)
{
	ଅଚଳ ସଂରଚନା diff_filepair *a = *((ଅଚଳ ସଂରଚନା diff_filepair **)a_);
	ଅଚଳ ସଂରଚନା diff_filepair *b = *((ଅଚଳ ସଂରଚନା diff_filepair **)b_);
	ଅଚଳ ଅକ୍ଷର *name_a, *name_b;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len_a, len_b, len;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	ଯଦି (cmp)
		ଫେରନ୍ତୁ cmp;
	cmp = len_b - len_a;
	ଯଦି (cmp)
		ଫେରନ୍ତୁ cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	ଫେରନ୍ତୁ (a->status == 'R') - (b->status == 'R');
}

ସ୍ଥିର ଶୂନ୍ୟ print_path_1(ଅଚଳ ଅକ୍ଷର *path)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା need_quote = quote_c_style(path, ଖାଲି, ଖାଲି, 0);
	ଯଦି (need_quote)
		quote_c_style(path, ଖାଲି, stdout, 0);
	ଅନ୍ୟଥା ଯଦି (strchr(path, ' '))
		printf("\"%s\"", path);
	ଅନ୍ୟଥା
		printf("%s", path);
}

ସ୍ଥିର ଅକ୍ଷର *anonymize_path_component(ଶୂନ୍ୟ)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା counter;
	ସଂରଚନା strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	ଫେରନ୍ତୁ strbuf_detach(&out, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ print_path(ଅଚଳ ଅକ୍ଷର *path)
{
	ଯଦି (!anonymize)
		print_path_1(path);
	ଅନ୍ୟଥା {
		ସ୍ଥିର ସଂରଚନା hashmap paths;
		ସ୍ଥିର ସଂରଚନା strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

ସ୍ଥିର ଅକ୍ଷର *generate_fake_oid(ଶୂନ୍ୟ)
{
	ସ୍ଥିର uint32_t counter = 1; /* avoid null oid */
	ଅଚଳ unsigned hashsz = the_hash_algo->rawsz;
	ସଂରଚନା object_id oid;
	ଅକ୍ଷର *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	ଫେରନ୍ତୁ oid_to_hex_r(hex, &oid);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *anonymize_oid(ଅଚଳ ଅକ୍ଷର *oid_hex)
{
	ସ୍ଥିର ସଂରଚନା hashmap objs;
	size_t len = strlen(oid_hex);
	ଫେରନ୍ତୁ anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

ସ୍ଥିର ଶୂନ୍ୟ show_filemodify(ସଂରଚନା diff_queue_struct *q,
			    ସଂରଚନା diff_options *options UNUSED, ଶୂନ୍ୟ *data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	ପାଇଁ (i = 0; i < q->nr; i++) {
		ସଂରଚନା diff_filespec *ospec = q->queue[i]->one;
		ସଂରଚନା diff_filespec *spec = q->queue[i]->two;

		ବାଛ (q->queue[i]->status) {
		ସ୍ଥିତି DIFF_STATUS_DELETED:
			printf("D ");
			print_path(spec->path);
			string_list_ଭର୍ତ୍ତି(changed, spec->path);
			putchar('\n');
			ଭାଙ୍ଗ;

		ସ୍ଥିତି DIFF_STATUS_COPIED:
		ସ୍ଥିତି DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			ଯଦି (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				print_path(ospec->path);
				putchar(' ');
				print_path(spec->path);
				string_list_ଭର୍ତ୍ତି(changed, spec->path);
				putchar('\n');

				ଯଦି (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					ଭାଙ୍ଗ;
			}
			/* fallthrough */

		ସ୍ଥିତି DIFF_STATUS_TYPE_CHANGED:
		ସ୍ଥିତି DIFF_STATUS_MODIFIED:
		ସ୍ଥିତି DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			ଯଦି (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			ଅନ୍ୟଥା {
				ସଂରଚନା object *object = lookup_object(the_repository,
								      &spec->oid);
				printf("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_ଭର୍ତ୍ତି(changed, spec->path);
			putchar('\n');
			ଭାଙ୍ଗ;

		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

ସ୍ଥିର ଅକ୍ଷର *anonymize_ref_component(ଶୂନ୍ୟ)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା counter;
	ସଂରଚନା strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	ଫେରନ୍ତୁ strbuf_detach(&out, ଖାଲି);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *anonymize_refname(ଅଚଳ ଅକ୍ଷର *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	ସ୍ଥିର ସଂରଚନା hashmap refs;
	ସ୍ଥିର ସଂରଚନା strbuf anon = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	strbuf_reset(&anon);
	ପାଇଁ (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		ଯଦି (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			ଭାଙ୍ଗ;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	ଫେରନ୍ତୁ anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
ସ୍ଥିର ଅକ୍ଷର *anonymize_commit_message(ଶୂନ୍ୟ)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା counter;
	ଫେରନ୍ତୁ xstrfmt("subject %d\n\nbody\n", counter++);
}

ସ୍ଥିର ଅକ୍ଷର *anonymize_ident(ଶୂନ୍ୟ)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା counter;
	ସଂରଚନା strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	ଫେରନ୍ତୁ strbuf_detach(&out, ଖାଲି);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
ସ୍ଥିର ଶୂନ୍ୟ anonymize_ident_ଧାଡ଼ି(ଅଚଳ ଅକ୍ଷର **beg, ଅଚଳ ଅକ୍ଷର **end)
{
	ସ୍ଥିର ସଂରଚନା hashmap idents;
	ସ୍ଥିର ସଂରଚନା strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	ସ୍ଥିର unsigned which_buffer;

	ସଂରଚନା strbuf *out;
	ସଂରଚନା ident_split split;
	ଅଚଳ ଅକ୍ଷର *end_of_header;

	out = &buffers[which_buffer++];
	which_buffer %= ARRAY_SIZE(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = strchr(*beg, ' ');
	ଯଦି (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	ଯଦି (!split_ident_ଧାଡ଼ି(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		ଅଚଳ ଅକ୍ଷର *ident;
		size_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} ଅନ୍ୟଥା {
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
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *find_commit_multiline_header(ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ,
						ଅଚଳ ଅକ୍ଷର *ଚାବି,
						ଅଚଳ ଅକ୍ଷର **end)
{
	ସଂରଚନା strbuf val = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *bol, *eol;
	size_t len;

	bol = find_commit_header(ସନ୍ଦେଶ, ଚାବି, &len);
	ଯଦି (!bol)
		ଫେରନ୍ତୁ ଖାଲି;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	ଯେତେବେଳେ (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	ଫେରନ୍ତୁ strbuf_detach(&val, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ print_signature(ଅଚଳ ଅକ୍ଷର *signature, ଅଚଳ ଅକ୍ଷର *object_hash)
{
	ଯଦି (!signature)
		ଫେରନ୍ତୁ;

	printf("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (unsigned)strlen(signature),
	       signature);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *append_signatures_for_header(ସଂରଚନା string_list *signatures,
						ଅଚଳ ଅକ୍ଷର *pos,
						ଅଚଳ ଅକ୍ଷର *header,
						ଅଚଳ ଅକ୍ଷର *object_hash)
{
	ଅଚଳ ଅକ୍ଷର *signature;
	ଅଚଳ ଅକ୍ଷର *start = pos;
	ଅଚଳ ଅକ୍ଷର *end = pos;

	ଯେତେବେଳେ ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (ଶୂନ୍ୟ *)object_hash;
		ମୁକ୍ତ((ଅକ୍ଷର *)signature);
		start = end;
	}

	ଫେରନ୍ତୁ end;
}

ସ୍ଥିର ଶୂନ୍ୟ handle_commit(ସଂରଚନା commit *commit, ସଂରଚନା rev_info *rev,
			  ସଂରଚନା string_list *paths_of_changed_objects)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_output_format = rev->diffopt.output_format;
	ଅଚଳ ଅକ୍ଷର *commit_buffer, *commit_buffer_cursor;
	ଅଚଳ ଅକ୍ଷର *author, *author_end, *committer, *committer_end;
	ଅଚଳ ଅକ୍ଷର *encoding = ଖାଲି;
	size_t encoding_len;
	ସଂରଚନା string_list signatures = STRING_LIST_INIT_DUP;
	ଅଚଳ ଅକ୍ଷର *message;
	ଅକ୍ଷର *reencoded = ଖାଲି;
	ସଂରଚନା commit_list *p;
	ଅଚଳ ଅକ୍ଷର *refname;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, ଖାଲି);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	ଯଦି (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	ଯଦି (!committer)
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

	ଯଦି (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		ଯଦି (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	ଯଦି (*commit_buffer_cursor == '\n') {
		ଅଚଳ ଅକ୍ଷର *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		ଅଚଳ ଅକ୍ଷର *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
	ଯଦି (message)
		message += 2;

	ଯଦି (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_ବୃକ୍ଷ) {
		parse_commit_or_die(commit->parents->item);
		diff_ବୃକ୍ଷ_oid(get_commit_ବୃକ୍ଷ_oid(commit->parents->item),
			      get_commit_ବୃକ୍ଷ_oid(commit), "", &rev->diffopt);
	}
	ଅନ୍ୟଥା
		diff_ମୂଳ_ବୃକ୍ଷ_oid(get_commit_ବୃକ୍ଷ_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	ପାଇଁ (i = 0; i < diff_queued_diff.nr; i++)
		ଯଦି (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	ଯଦି (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_ଧାଡ଼ି(&committer, &committer_end);
		anonymize_ident_ଧାଡ଼ି(&author, &author_end);
	}

	mark_next_object(&commit->object);
	ଯଦି (anonymize) {
		reencoded = anonymize_commit_message();
	} ଅନ୍ୟଥା ଯଦି (encoding) {
		ଅକ୍ଷର *buf;
		ବାଛ (reencode_mode) {
		ସ୍ଥିତି REENCODE_YES:
			buf = xstrfmt("%.*s", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			ମୁକ୍ତ(buf);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି REENCODE_NO:
			ଭାଙ୍ଗ;
		ସ୍ଥିତି REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	ଯଦି (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	ଯଦି (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(author_end - author), author,
	       (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(committer_end - committer), committer);
	ଯଦି (signatures.nr) {
		ବାଛ (signed_commit_mode) {
		/* Exporting modes */
		ସ୍ଥିତି SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		ସ୍ଥିତି SIGN_VERBATIM:
			ପାଇଁ (size_t i = 0; i < signatures.nr; i++) {
				ସଂରଚନା string_list_item *item = &signatures.ବସ୍ତୁଗୁଡ଼ିକ[i];
				print_signature(item->string, item->util);
			}
			ଭାଙ୍ଗ;

		/* Stripping modes */
		ସ୍ଥିତି SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		ସ୍ଥିତି SIGN_STRIP:
			ଭାଙ୍ଗ;

		/* Aborting modes */
		ସ୍ଥିତି SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	ଯଦି (!reencoded && encoding)
		printf("encoding %.*s\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	ମୁକ୍ତ(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	ପାଇଁ (i = 0, p = commit->parents; p; p = p->next) {
		ସଂରଚନା object *obj = &p->item->object;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark = get_object_mark(obj);

		ଯଦି (!mark && !reference_excluded_commits)
			ଜାରି;
		ଯଦି (i == 0)
			printf("from ");
		ଅନ୍ୟଥା
			printf("merge ");
		ଯଦି (mark)
			printf(":%d\n", mark);
		ଅନ୍ୟଥା
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	ଯଦି (full_ବୃକ୍ଷ)
		printf("deleteall\n");
	log_ବୃକ୍ଷ_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

ସ୍ଥିର ଅକ୍ଷର *anonymize_tag(ଶୂନ୍ୟ)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା counter;
	ସଂରଚନା strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	ଫେରନ୍ତୁ strbuf_detach(&out, ଖାଲି);
}


ସ୍ଥିର ଶୂନ୍ୟ handle_tag(ଅଚଳ ଅକ୍ଷର *name, ସଂରଚନା tag *tag)
{
	size_t size;
	ଗଣନା object_type type;
	ଅକ୍ଷର *buf;
	ଅଚଳ ଅକ୍ଷର *tagger, *tagger_end, *message;
	size_t message_size = 0;
	ସଂରଚନା object *tagged;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା tagged_mark;
	ସଂରଚନା commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	ଯେତେବେଳେ (tagged->type == OBJ_TAG) {
		tagged = ((ସଂରଚନା tag *)tagged)->tagged;
	}
	ଯଦି (tagged->type == OBJ_ବୃକ୍ଷ) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		ଫେରନ୍ତୁ;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	ଯଦି (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	ଯଦି (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	ଯଦି (!tagger) {
		ଯଦି (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		ଅନ୍ୟଥା
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} ଅନ୍ୟଥା {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		ଯଦି (anonymize)
			anonymize_ident_ଧାଡ଼ି(&tagger, &tagger_end);
	}

	ଯଦି (anonymize) {
		name = anonymize_refname(name);
		ଯଦି (message) {
			ସ୍ଥିର ସଂରଚନା hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	ଯଦି (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		ଯଦି (sig_offset < message_size)
			ବାଛ (signed_tag_mode) {
			/* Exporting modes */
			ସ୍ଥିତି SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			ସ୍ଥିତି SIGN_VERBATIM:
				ଭାଙ୍ଗ;

			/* Stripping modes */
			ସ୍ଥିତି SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			ସ୍ଥିତି SIGN_STRIP:
				message_size = sig_offset;
				ଭାଙ୍ଗ;

			/* Aborting modes */
			ସ୍ଥିତି SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	ଯଦି (!tagged_mark) {
		ବାଛ (tag_of_filtered_mode) {
		ସ୍ଥିତି TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		ସ୍ଥିତି DROP:
			/* Ignore this tag altogether */
			ମୁକ୍ତ(buf);
			ଫେରନ୍ତୁ;
		ସ୍ଥିତି REWRITE:
			ଯଦି (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} ଅନ୍ୟଥା ଯଦି (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((ସଂରଚନା commit *)tagged);
				ଯଦି (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					ମୁକ୍ତ(buf);
					ଫେରନ୍ତୁ;
				}
				tagged_mark = get_object_mark(&p->object);
			} ଅନ୍ୟଥା {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	ଯଦି (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	ଯଦି (mark_tags) {
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	ଯଦି (tagged_mark)
		printf("from :%d\n", tagged_mark);
	ଅନ୍ୟଥା
		printf("from %s\n", oid_to_hex(&tagged->oid));

	ଯଦି (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)message_size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)message_size, message ? message : "");
	ମୁକ୍ତ(buf);
}

ସ୍ଥିର ସଂରଚନା commit *get_commit(ସଂରଚନା rev_cmdline_entry *e, ଅଚଳ ଅକ୍ଷର *full_name)
{
	ବାଛ (e->item->type) {
	ସ୍ଥିତି OBJ_COMMIT:
		ଫେରନ୍ତୁ (ସଂରଚନା commit *)e->item;
	ସ୍ଥିତି OBJ_TAG: {
		ସଂରଚନା tag *tag = (ସଂରଚନା tag *)e->item;

		/* handle nested tags */
		ଯେତେବେଳେ (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (ସଂରଚନା tag *)tag->tagged;
		}
		ଯଦି (!tag)
			die(_("tag %s points nowhere?"), e->name);
		ଫେରନ୍ତୁ (ସଂରଚନା commit *)tag;
	}
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		ଫେରନ୍ତୁ ଖାଲି;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ get_tags_and_duplicates(ସଂରଚନା rev_cmdline_info *info)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < info->nr; i++) {
		ସଂରଚନା rev_cmdline_entry *e = info->rev + i;
		ସଂରଚନା object_id oid;
		ସଂରଚନା commit *commit;
		ଅକ୍ଷର *full_name = ଖାଲି;

		ଯଦି (e->flags & UNINTERESTING)
			ଜାରି;

		ଯଦି (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			ମୁକ୍ତ(full_name);
			ଜାରି;
		}

		ଯଦି (refspecs.nr) {
			ଅକ୍ଷର *ବ୍ୟକ୍ତିଗତ;
			ବ୍ୟକ୍ତିଗତ = apply_refspecs(&refspecs, full_name);
			ଯଦି (ବ୍ୟକ୍ତିଗତ) {
				ମୁକ୍ତ(full_name);
				full_name = ବ୍ୟକ୍ତିଗତ;
			}
		}

		commit = get_commit(e, full_name);
		ଯଦି (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			ମୁକ୍ତ(full_name);
			ଜାରି;
		}

		ବାଛ (commit->object.type) {
		ସ୍ଥିତି OBJ_COMMIT:
			ଭାଙ୍ଗ;
		ସ୍ଥିତି OBJ_BLOB:
			export_blob(&commit->object.oid);
			ମୁକ୍ତ(full_name);
			ଜାରି;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			ମୁକ୍ତ(full_name);
			ଜାରି;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		ଯଦି (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		ଯଦି (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		ଅନ୍ୟଥା
			ମୁକ୍ତ(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ handle_tags_and_duplicates(ସଂରଚନା string_list *extras)
{
	ସଂରଚନା commit *commit;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = extras->nr - 1; i >= 0; i--) {
		ଅଚଳ ଅକ୍ଷର *name = extras->ବସ୍ତୁଗୁଡ଼ିକ[i].string;
		ସଂରଚନା object *object = extras->ବସ୍ତୁଗୁଡ଼ିକ[i].util;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark;

		ବାଛ (object->type) {
		ସ୍ଥିତି OBJ_TAG:
			handle_tag(name, (ସଂରଚନା tag *)object);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି OBJ_COMMIT:
			ଯଦି (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((ସଂରଚନା commit *)object);
			ଯଦି (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				printf("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				ଜାରି;
			}

			mark = get_object_mark(&commit->object);
			ଯଦି (!mark) {
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
				ଯଦି (!reference_excluded_commits) {
					/* delete the ref */
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					ଜାରି;
				}
				/* set ref to commit using oid, not mark */
				printf("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				ଜାରି;
			}

			printf("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			ଭାଙ୍ଗ;
		}
	}
}

ସ୍ଥିର ଶୂନ୍ୟ export_marks(ଅକ୍ଷର *file)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	uint32_t mark;
	ସଂରଚନା decoration_entry *deco = idnums.entries;
	FILE *f;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା e = 0;

	f = fopen_for_writing(file);
	ଯଦି (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	ପାଇଁ (i = 0; i < idnums.size; i++) {
		ଯଦି (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			ଯଦି (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    ଭାଙ୍ଗ;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	ଯଦି (e)
		error(_("unable to write marks file %s."), file);
}

ସ୍ଥିର ଶୂନ୍ୟ import_marks(ଅକ୍ଷର *ନିବେଶ_file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_exists)
{
	ଅକ୍ଷର ଧାଡ଼ି[512];
	FILE *f;
	ସଂରଚନା stat sb;

	ଯଦି (check_exists && stat(ନିବେଶ_file, &sb))
		ଫେରନ୍ତୁ;

	f = xfopen(ନିବେଶ_file, "r");
	ଯେତେବେଳେ (fgets(ଧାଡ଼ି, ଆକାର(ଧାଡ଼ି), f)) {
		uint32_t mark;
		ଅକ୍ଷର *ଧାଡ଼ି_end, *mark_end;
		ସଂରଚନା object_id oid;
		ସଂରଚନା object *object;
		ସଂରଚନା commit *commit;
		ଗଣନା object_type type;

		ଧାଡ଼ି_end = strchr(ଧାଡ଼ି, '\n');
		ଯଦି (ଧାଡ଼ି[0] != ':' || !ଧାଡ଼ି_end)
			die(_("corrupt mark line: %s"), ଧାଡ଼ି);
		*ଧାଡ଼ି_end = '\0';

		mark = strtoumax(ଧାଡ଼ି + 1, &mark_end, 10);
		ଯଦି (!mark || mark_end == ଧାଡ଼ି + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), ଧାଡ଼ି);

		ଯଦି (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, ଖାଲି);
		ଯଦି (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		ଯଦି (type != OBJ_COMMIT)
			/* only commits */
			ଜାରି;

		commit = lookup_commit(the_repository, &oid);
		ଯଦି (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		ଯଦି (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

ସ୍ଥିର ଶୂନ୍ୟ handle_deletes(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପାଇଁ (i = 0; i < refspecs.nr; i++) {
		ସଂରଚନା refspec_item *refspec = &refspecs.ବସ୍ତୁଗୁଡ଼ିକ[i];
		ଯଦି (*refspec->src)
			ଜାରି;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_opt_anonymize_map(ଅଚଳ ସଂରଚନା option *opt,
				   ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା hashmap *map = opt->ମୂଲ୍ୟ;
	ଅଚଳ ଅକ୍ଷର *delim, *ମୂଲ୍ୟ;
	size_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	ଯଦି (delim) {
		keylen = delim - arg;
		ମୂଲ୍ୟ = delim + 1;
	} ଅନ୍ୟଥା {
		keylen = strlen(arg);
		ମୂଲ୍ୟ = arg;
	}

	ଯଦି (!keylen || !*ମୂଲ୍ୟ)
		ଫେରନ୍ତୁ error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(ମୂଲ୍ୟ));

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_fast_export(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		    ଅଚଳ ଅକ୍ଷର **argv,
		    ଅଚଳ ଅକ୍ଷର *prefix,
		    ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା rev_info revs;
	ସଂରଚନା commit *commit;
	ଅକ୍ଷର *export_filename = ଖାଲି,
	     *import_filename = ଖାଲି,
	     *import_filename_if_exists = ଖାଲି;
	uint32_t lastimportid;
	ସଂରଚନା string_list refspecs_list = STRING_LIST_INIT_NODUP;
	ସଂରଚନା string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	ସଂରଚନା option options[] = {
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
		OPT_BOOL(0, "full-tree", &full_ବୃକ୍ଷ,
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

	ଯଦି (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, ଖାଲି);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, prefix);
	init_revision_sources(&revision_sources);
	revs.topo_order = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, prefix, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = setup_revisions(argc, argv, &revs, ଖାଲି);
	ଯଦି (argc > 1)
		usage_with_options (fast_export_usage, options);

	ଯଦି (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	ଯଦି (refspecs_list.nr) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

		ପାଇଁ (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.ବସ୍ତୁଗୁଡ଼ିକ[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	ଯଦି (use_done_feature)
		printf("feature done\n");

	ଯଦି (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	ଯଦି (import_filename)
		import_marks(import_filename, 0);
	ଅନ୍ୟଥା ଯଦି (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	ଯଦି (import_filename && revs.prune_data.nr)
		full_ବୃକ୍ଷ = 1;

	get_tags_and_duplicates(&revs.cmdline);

	ଯଦି (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	ଯେତେବେଳେ ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	ଯଦି (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	ଯଦି (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	ଫେରନ୍ତୁ 0;
}
