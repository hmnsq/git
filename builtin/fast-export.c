/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "refspec.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "diffcore.h"
#ಅನ್ತರ್ಭಾವ "log-tree.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "decorate.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "blob.h"
#ಅನ್ತರ್ಭಾವ "gpg-interface.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ progress;
ಸ್ಥಿರ ಗಣನಾ sign_mode signed_tag_mode = SIGN_ABORT;
ಸ್ಥಿರ ಗಣನಾ sign_mode signed_commit_mode = SIGN_STRIP;
ಸ್ಥಿರ ಗಣನಾ tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
ಸ್ಥಿರ ಗಣನಾ reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fake_missing_tagger;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_done_feature;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ no_data;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ full_ವೃಕ್ಷ;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reference_excluded_commits;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_original_ids;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_tags;
ಸ್ಥಿರ ಸಂರಚನಾ string_list extra_refs = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ ಸಂರಚನಾ string_list tag_refs = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ ಸಂರಚನಾ refspec refspecs;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ anonymize;
ಸ್ಥಿರ ಸಂರಚನಾ hashmap anonymized_seeds;
ಸ್ಥಿರ ಸಂರಚನಾ revision_sources revision_sources;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_sign_mode(ಧ್ರುವ ಸಂರಚನಾ option *opt,
			       ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಗಣನಾ sign_mode *val = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (parse_sign_mode(arg, val, ರಿಕ್ತ) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		ಪ್ರತ್ಯಾಗಮ error(_("unknown %s mode: %s"), opt->long_name, arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_tag_of_filtered_mode(ಧ್ರುವ ಸಂರಚನಾ option *opt,
					  ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಗಣನಾ tag_of_filtered_mode *val = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "drop"))
		*val = DROP;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ error(_("unknown tag-of-filtered mode: %s"), arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_reencode_mode(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				   ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಗಣನಾ reencode_mode *val = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset) {
		*val = REENCODE_ABORT;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ವಿಕಲ್ಪ (git_parse_maybe_bool(arg)) {
	ಪಕ್ಷ 0:
		*val = REENCODE_NO;
		ಭಙ್ಗ;
	ಪಕ್ಷ 1:
		*val = REENCODE_YES;
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		ಯದಿ (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		ಅನ್ಯಥಾ
			ಪ್ರತ್ಯಾಗಮ error(_("unknown reencoding mode: %s"), arg);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ decoration idnums;
ಸ್ಥಿರ uint32_t last_idnum;
ಸಂರಚನಾ anonymized_entry {
	ಸಂರಚನಾ hashmap_entry hash;
	ವರ್ಣ *anon;
	ಧ್ರುವ ವರ್ಣ orig[FLEX_ARRAY];
};

ಸಂರಚನಾ anonymized_entry_ಕೀಲಿ {
	ಸಂರಚನಾ hashmap_entry hash;
	ಧ್ರುವ ವರ್ಣ *orig;
	size_t orig_len;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ anonymized_entry_cmp(ಧ್ರುವ ಶೂನ್ಯ *cmp_data UNUSED,
				ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *eptr,
				ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *entry_or_ಕೀಲಿ,
				ಧ್ರುವ ಶೂನ್ಯ *keydata)
{
	ಧ್ರುವ ಸಂರಚನಾ anonymized_entry *a, *b;

	a = container_of(eptr, ಧ್ರುವ ಸಂರಚನಾ anonymized_entry, hash);
	ಯದಿ (keydata) {
		ಧ್ರುವ ಸಂರಚನಾ anonymized_entry_ಕೀಲಿ *ಕೀಲಿ = keydata;
		ಪೂರ್ಣಾಙ್ಕ equal = !xstrncmpz(a->orig, ಕೀಲಿ->orig, ಕೀಲಿ->orig_len);
		ಪ್ರತ್ಯಾಗಮ !equal;
	}

	b = container_of(entry_or_ಕೀಲಿ, ಧ್ರುವ ಸಂರಚನಾ anonymized_entry, hash);
	ಪ್ರತ್ಯಾಗಮ strcmp(a->orig, b->orig);
}

ಸ್ಥಿರ ಸಂರಚನಾ anonymized_entry *add_anonymized_entry(ಸಂರಚನಾ hashmap *map,
						     unsigned hash,
						     ಧ್ರುವ ವರ್ಣ *orig, size_t len,
						     ವರ್ಣ *anon)
{
	ಸಂರಚನಾ anonymized_entry *ret, *old;

	ಯದಿ (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, ರಿಕ್ತ, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	ಯದಿ (old) {
		ಮುಕ್ತ(old->anon);
		ಮುಕ್ತ(old);
	}

	ಪ್ರತ್ಯಾಗಮ ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *anonymize_str(ಸಂರಚನಾ hashmap *map,
				 ವರ್ಣ *(*generate)(ಶೂನ್ಯ),
				 ಧ್ರುವ ವರ್ಣ *orig, size_t len)
{
	ಸಂರಚನಾ anonymized_entry_ಕೀಲಿ ಕೀಲಿ;
	ಸಂರಚನಾ anonymized_entry *ret;

	hashmap_entry_init(&ಕೀಲಿ.hash, memhash(orig, len));
	ಕೀಲಿ.orig = orig;
	ಕೀಲಿ.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &ಕೀಲಿ, hash, &ಕೀಲಿ);

	/* ...otherwise check if we've already seen it in this context... */
	ಯದಿ (!ret)
		ret = hashmap_get_entry(map, &ಕೀಲಿ, hash, &ಕೀಲಿ);

	/* ...and finally generate a new mapping if necessary */
	ಯದಿ (!ret)
		ret = add_anonymized_entry(map, ಕೀಲಿ.hash.hash,
					   orig, len, generate());

	ಪ್ರತ್ಯಾಗಮ ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
ಸ್ಥಿರ ಶೂನ್ಯ anonymize_path(ಸಂರಚನಾ strbuf *out, ಧ್ರುವ ವರ್ಣ *path,
			   ಸಂರಚನಾ hashmap *map,
			   ವರ್ಣ *(*generate)(ಶೂನ್ಯ))
{
	ಯಾವತ್ (*path) {
		ಧ್ರುವ ವರ್ಣ *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		ಧ್ರುವ ವರ್ಣ *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		ಯದಿ (*path)
			strbuf_addch(out, *path++);
	}
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ *mark_to_ptr(uint32_t mark)
{
	ಪ್ರತ್ಯಾಗಮ (ಶೂನ್ಯ *)(uintptr_t)mark;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint32_t ptr_to_mark(ಶೂನ್ಯ * mark)
{
	ಪ್ರತ್ಯಾಗಮ (uint32_t)(uintptr_t)mark;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ mark_object(ಸಂರಚನಾ object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ mark_next_object(ಸಂರಚನಾ object *object)
{
	mark_object(object, ++last_idnum);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_object_mark(ಸಂರಚನಾ object *object)
{
	ಶೂನ್ಯ *decoration = lookup_decoration(&idnums, object);
	ಯದಿ (!decoration)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ ptr_to_mark(decoration);
}

ಸ್ಥಿರ ಸಂರಚನಾ commit *rewrite_commit(ಸಂರಚನಾ commit *p)
{
	ಪ್ರತಿ (;;) {
		ಯದಿ (p->parents && p->parents->next)
			ಭಙ್ಗ;
		ಯದಿ (p->object.flags & UNINTERESTING)
			ಭಙ್ಗ;
		ಯದಿ (!(p->object.flags & TREESAME))
			ಭಙ್ಗ;
		ಯದಿ (!p->parents)
			ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
		p = p->parents->item;
	}
	ಪ್ರತ್ಯಾಗಮ p;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_progress(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ counter = 0;
	ಯದಿ (!progress)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ ((++counter % progress) == 0)
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
ಸ್ಥಿರ ವರ್ಣ *anonymize_blob(unsigned long *size)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ counter;
	ಸಂರಚನಾ strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&out, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ export_blob(ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	unsigned long size;
	ಗಣನಾ object_type type;
	ವರ್ಣ *buf;
	ಸಂರಚನಾ object *object;
	ಪೂರ್ಣಾಙ್ಕ eaten;

	ಯದಿ (no_data)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (is_null_oid(oid))
		ಪ್ರತ್ಯಾಗಮ;

	object = lookup_object(the_repository, oid);
	ಯದಿ (object && object->flags & SHOWN)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (anonymize) {
		buf = anonymize_blob(&size);
		object = (ಸಂರಚನಾ object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} ಅನ್ಯಥಾ {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		ಯದಿ (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		ಯದಿ (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	ಯದಿ (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	ಯದಿ (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	ಯದಿ (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	ಯದಿ (!eaten)
		ಮುಕ್ತ(buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ depth_first(ಧ್ರುವ ಶೂನ್ಯ *a_, ಧ್ರುವ ಶೂನ್ಯ *b_)
{
	ಧ್ರುವ ಸಂರಚನಾ diff_filepair *a = *((ಧ್ರುವ ಸಂರಚನಾ diff_filepair **)a_);
	ಧ್ರುವ ಸಂರಚನಾ diff_filepair *b = *((ಧ್ರುವ ಸಂರಚನಾ diff_filepair **)b_);
	ಧ್ರುವ ವರ್ಣ *name_a, *name_b;
	ಪೂರ್ಣಾಙ್ಕ len_a, len_b, len;
	ಪೂರ್ಣಾಙ್ಕ cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	ಯದಿ (cmp)
		ಪ್ರತ್ಯಾಗಮ cmp;
	cmp = len_b - len_a;
	ಯದಿ (cmp)
		ಪ್ರತ್ಯಾಗಮ cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	ಪ್ರತ್ಯಾಗಮ (a->status == 'R') - (b->status == 'R');
}

ಸ್ಥಿರ ಶೂನ್ಯ print_path_1(ಧ್ರುವ ವರ್ಣ *path)
{
	ಪೂರ್ಣಾಙ್ಕ need_quote = quote_c_style(path, ರಿಕ್ತ, ರಿಕ್ತ, 0);
	ಯದಿ (need_quote)
		quote_c_style(path, ರಿಕ್ತ, stdout, 0);
	ಅನ್ಯಥಾ ಯದಿ (strchr(path, ' '))
		printf("\"%s\"", path);
	ಅನ್ಯಥಾ
		printf("%s", path);
}

ಸ್ಥಿರ ವರ್ಣ *anonymize_path_component(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ counter;
	ಸಂರಚನಾ strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&out, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_path(ಧ್ರುವ ವರ್ಣ *path)
{
	ಯದಿ (!anonymize)
		print_path_1(path);
	ಅನ್ಯಥಾ {
		ಸ್ಥಿರ ಸಂರಚನಾ hashmap paths;
		ಸ್ಥಿರ ಸಂರಚನಾ strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

ಸ್ಥಿರ ವರ್ಣ *generate_fake_oid(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ uint32_t counter = 1; /* avoid null oid */
	ಧ್ರುವ unsigned hashsz = the_hash_algo->rawsz;
	ಸಂರಚನಾ object_id oid;
	ವರ್ಣ *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	ಪ್ರತ್ಯಾಗಮ oid_to_hex_r(hex, &oid);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *anonymize_oid(ಧ್ರುವ ವರ್ಣ *oid_hex)
{
	ಸ್ಥಿರ ಸಂರಚನಾ hashmap objs;
	size_t len = strlen(oid_hex);
	ಪ್ರತ್ಯಾಗಮ anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_filemodify(ಸಂರಚನಾ diff_queue_struct *q,
			    ಸಂರಚನಾ diff_options *options UNUSED, ಶೂನ್ಯ *data)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	ಪ್ರತಿ (i = 0; i < q->nr; i++) {
		ಸಂರಚನಾ diff_filespec *ospec = q->queue[i]->one;
		ಸಂರಚನಾ diff_filespec *spec = q->queue[i]->two;

		ವಿಕಲ್ಪ (q->queue[i]->status) {
		ಪಕ್ಷ DIFF_STATUS_DELETED:
			printf("D ");
			print_path(spec->path);
			string_list_ಸೇರಿಸು(changed, spec->path);
			putchar('\n');
			ಭಙ್ಗ;

		ಪಕ್ಷ DIFF_STATUS_COPIED:
		ಪಕ್ಷ DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			ಯದಿ (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				print_path(ospec->path);
				putchar(' ');
				print_path(spec->path);
				string_list_ಸೇರಿಸು(changed, spec->path);
				putchar('\n');

				ಯದಿ (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					ಭಙ್ಗ;
			}
			/* fallthrough */

		ಪಕ್ಷ DIFF_STATUS_TYPE_CHANGED:
		ಪಕ್ಷ DIFF_STATUS_MODIFIED:
		ಪಕ್ಷ DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			ಯದಿ (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			ಅನ್ಯಥಾ {
				ಸಂರಚನಾ object *object = lookup_object(the_repository,
								      &spec->oid);
				printf("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_ಸೇರಿಸು(changed, spec->path);
			putchar('\n');
			ಭಙ್ಗ;

		ಸ್ವಭಾವ:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

ಸ್ಥಿರ ವರ್ಣ *anonymize_ref_component(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ counter;
	ಸಂರಚನಾ strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&out, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *anonymize_refname(ಧ್ರುವ ವರ್ಣ *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	ಸ್ಥಿರ ಸಂರಚನಾ hashmap refs;
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf anon = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ i;

	strbuf_reset(&anon);
	ಪ್ರತಿ (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		ಯದಿ (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			ಭಙ್ಗ;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	ಪ್ರತ್ಯಾಗಮ anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
ಸ್ಥಿರ ವರ್ಣ *anonymize_commit_message(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ counter;
	ಪ್ರತ್ಯಾಗಮ xstrfmt("subject %d\n\nbody\n", counter++);
}

ಸ್ಥಿರ ವರ್ಣ *anonymize_ident(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ counter;
	ಸಂರಚನಾ strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&out, ರಿಕ್ತ);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
ಸ್ಥಿರ ಶೂನ್ಯ anonymize_ident_ಸಾಲು(ಧ್ರುವ ವರ್ಣ **beg, ಧ್ರುವ ವರ್ಣ **end)
{
	ಸ್ಥಿರ ಸಂರಚನಾ hashmap idents;
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	ಸ್ಥಿರ unsigned which_buffer;

	ಸಂರಚನಾ strbuf *out;
	ಸಂರಚನಾ ident_split split;
	ಧ್ರುವ ವರ್ಣ *end_of_header;

	out = &buffers[which_buffer++];
	which_buffer %= ARRAY_SIZE(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = strchr(*beg, ' ');
	ಯದಿ (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (ಪೂರ್ಣಾಙ್ಕ)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	ಯದಿ (!split_ident_ಸಾಲು(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		ಧ್ರುವ ವರ್ಣ *ident;
		size_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} ಅನ್ಯಥಾ {
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
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *find_commit_multiline_header(ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ,
						ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ,
						ಧ್ರುವ ವರ್ಣ **end)
{
	ಸಂರಚನಾ strbuf val = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *bol, *eol;
	size_t len;

	bol = find_commit_header(ಸಂದೇಶ, ಕೀಲಿ, &len);
	ಯದಿ (!bol)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	ಯಾವತ್ (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&val, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_signature(ಧ್ರುವ ವರ್ಣ *signature, ಧ್ರುವ ವರ್ಣ *object_hash)
{
	ಯದಿ (!signature)
		ಪ್ರತ್ಯಾಗಮ;

	printf("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (unsigned)strlen(signature),
	       signature);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *append_signatures_for_header(ಸಂರಚನಾ string_list *signatures,
						ಧ್ರುವ ವರ್ಣ *pos,
						ಧ್ರುವ ವರ್ಣ *header,
						ಧ್ರುವ ವರ್ಣ *object_hash)
{
	ಧ್ರುವ ವರ್ಣ *signature;
	ಧ್ರುವ ವರ್ಣ *start = pos;
	ಧ್ರುವ ವರ್ಣ *end = pos;

	ಯಾವತ್ ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (ಶೂನ್ಯ *)object_hash;
		ಮುಕ್ತ((ವರ್ಣ *)signature);
		start = end;
	}

	ಪ್ರತ್ಯಾಗಮ end;
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_commit(ಸಂರಚನಾ commit *commit, ಸಂರಚನಾ rev_info *rev,
			  ಸಂರಚನಾ string_list *paths_of_changed_objects)
{
	ಪೂರ್ಣಾಙ್ಕ saved_output_format = rev->diffopt.output_format;
	ಧ್ರುವ ವರ್ಣ *commit_buffer, *commit_buffer_cursor;
	ಧ್ರುವ ವರ್ಣ *author, *author_end, *committer, *committer_end;
	ಧ್ರುವ ವರ್ಣ *encoding = ರಿಕ್ತ;
	size_t encoding_len;
	ಸಂರಚನಾ string_list signatures = STRING_LIST_INIT_DUP;
	ಧ್ರುವ ವರ್ಣ *message;
	ವರ್ಣ *reencoded = ರಿಕ್ತ;
	ಸಂರಚನಾ commit_list *p;
	ಧ್ರುವ ವರ್ಣ *refname;
	ಪೂರ್ಣಾಙ್ಕ i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, ರಿಕ್ತ);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	ಯದಿ (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	ಯದಿ (!committer)
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

	ಯದಿ (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		ಯದಿ (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	ಯದಿ (*commit_buffer_cursor == '\n') {
		ಧ್ರುವ ವರ್ಣ *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		ಧ್ರುವ ವರ್ಣ *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
	ಯದಿ (message)
		message += 2;

	ಯದಿ (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_ವೃಕ್ಷ) {
		parse_commit_or_die(commit->parents->item);
		diff_ವೃಕ್ಷ_oid(get_commit_ವೃಕ್ಷ_oid(commit->parents->item),
			      get_commit_ವೃಕ್ಷ_oid(commit), "", &rev->diffopt);
	}
	ಅನ್ಯಥಾ
		diff_ಬೇರು_ವೃಕ್ಷ_oid(get_commit_ವೃಕ್ಷ_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	ಪ್ರತಿ (i = 0; i < diff_queued_diff.nr; i++)
		ಯದಿ (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	ಯದಿ (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_ಸಾಲು(&committer, &committer_end);
		anonymize_ident_ಸಾಲು(&author, &author_end);
	}

	mark_next_object(&commit->object);
	ಯದಿ (anonymize) {
		reencoded = anonymize_commit_message();
	} ಅನ್ಯಥಾ ಯದಿ (encoding) {
		ವರ್ಣ *buf;
		ವಿಕಲ್ಪ (reencode_mode) {
		ಪಕ್ಷ REENCODE_YES:
			buf = xstrfmt("%.*s", (ಪೂರ್ಣಾಙ್ಕ)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			ಮುಕ್ತ(buf);
			ಭಙ್ಗ;
		ಪಕ್ಷ REENCODE_NO:
			ಭಙ್ಗ;
		ಪಕ್ಷ REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (ಪೂರ್ಣಾಙ್ಕ)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	ಯದಿ (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	ಯದಿ (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (ಪೂರ್ಣಾಙ್ಕ)(author_end - author), author,
	       (ಪೂರ್ಣಾಙ್ಕ)(committer_end - committer), committer);
	ಯದಿ (signatures.nr) {
		ವಿಕಲ್ಪ (signed_commit_mode) {
		/* Exporting modes */
		ಪಕ್ಷ SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		ಪಕ್ಷ SIGN_VERBATIM:
			ಪ್ರತಿ (size_t i = 0; i < signatures.nr; i++) {
				ಸಂರಚನಾ string_list_item *item = &signatures.ವಸ್ತುಗಳು[i];
				print_signature(item->string, item->util);
			}
			ಭಙ್ಗ;

		/* Stripping modes */
		ಪಕ್ಷ SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		ಪಕ್ಷ SIGN_STRIP:
			ಭಙ್ಗ;

		/* Aborting modes */
		ಪಕ್ಷ SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		ಸ್ವಭಾವ:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	ಯದಿ (!reencoded && encoding)
		printf("encoding %.*s\n", (ಪೂರ್ಣಾಙ್ಕ)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	ಮುಕ್ತ(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	ಪ್ರತಿ (i = 0, p = commit->parents; p; p = p->next) {
		ಸಂರಚನಾ object *obj = &p->item->object;
		ಪೂರ್ಣಾಙ್ಕ mark = get_object_mark(obj);

		ಯದಿ (!mark && !reference_excluded_commits)
			ಅನುವರ್ತ;
		ಯದಿ (i == 0)
			printf("from ");
		ಅನ್ಯಥಾ
			printf("merge ");
		ಯದಿ (mark)
			printf(":%d\n", mark);
		ಅನ್ಯಥಾ
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	ಯದಿ (full_ವೃಕ್ಷ)
		printf("deleteall\n");
	log_ವೃಕ್ಷ_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

ಸ್ಥಿರ ವರ್ಣ *anonymize_tag(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ counter;
	ಸಂರಚನಾ strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&out, ರಿಕ್ತ);
}


ಸ್ಥಿರ ಶೂನ್ಯ handle_tag(ಧ್ರುವ ವರ್ಣ *name, ಸಂರಚನಾ tag *tag)
{
	size_t size;
	ಗಣನಾ object_type type;
	ವರ್ಣ *buf;
	ಧ್ರುವ ವರ್ಣ *tagger, *tagger_end, *message;
	size_t message_size = 0;
	ಸಂರಚನಾ object *tagged;
	ಪೂರ್ಣಾಙ್ಕ tagged_mark;
	ಸಂರಚನಾ commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	ಯಾವತ್ (tagged->type == OBJ_TAG) {
		tagged = ((ಸಂರಚನಾ tag *)tagged)->tagged;
	}
	ಯದಿ (tagged->type == OBJ_ವೃಕ್ಷ) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		ಪ್ರತ್ಯಾಗಮ;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	ಯದಿ (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	ಯದಿ (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	ಯದಿ (!tagger) {
		ಯದಿ (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		ಅನ್ಯಥಾ
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} ಅನ್ಯಥಾ {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		ಯದಿ (anonymize)
			anonymize_ident_ಸಾಲು(&tagger, &tagger_end);
	}

	ಯದಿ (anonymize) {
		name = anonymize_refname(name);
		ಯದಿ (message) {
			ಸ್ಥಿರ ಸಂರಚನಾ hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	ಯದಿ (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		ಯದಿ (sig_offset < message_size)
			ವಿಕಲ್ಪ (signed_tag_mode) {
			/* Exporting modes */
			ಪಕ್ಷ SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			ಪಕ್ಷ SIGN_VERBATIM:
				ಭಙ್ಗ;

			/* Stripping modes */
			ಪಕ್ಷ SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			ಪಕ್ಷ SIGN_STRIP:
				message_size = sig_offset;
				ಭಙ್ಗ;

			/* Aborting modes */
			ಪಕ್ಷ SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			ಸ್ವಭಾವ:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	ಯದಿ (!tagged_mark) {
		ವಿಕಲ್ಪ (tag_of_filtered_mode) {
		ಪಕ್ಷ TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		ಪಕ್ಷ DROP:
			/* Ignore this tag altogether */
			ಮುಕ್ತ(buf);
			ಪ್ರತ್ಯಾಗಮ;
		ಪಕ್ಷ REWRITE:
			ಯದಿ (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} ಅನ್ಯಥಾ ಯದಿ (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((ಸಂರಚನಾ commit *)tagged);
				ಯದಿ (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					ಮುಕ್ತ(buf);
					ಪ್ರತ್ಯಾಗಮ;
				}
				tagged_mark = get_object_mark(&p->object);
			} ಅನ್ಯಥಾ {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	ಯದಿ (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	ಯದಿ (mark_tags) {
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	ಯದಿ (tagged_mark)
		printf("from :%d\n", tagged_mark);
	ಅನ್ಯಥಾ
		printf("from %s\n", oid_to_hex(&tagged->oid));

	ಯದಿ (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (ಪೂರ್ಣಾಙ್ಕ)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (ಪೂರ್ಣಾಙ್ಕ)message_size, (ಪೂರ್ಣಾಙ್ಕ)message_size, message ? message : "");
	ಮುಕ್ತ(buf);
}

ಸ್ಥಿರ ಸಂರಚನಾ commit *get_commit(ಸಂರಚನಾ rev_cmdline_entry *e, ಧ್ರುವ ವರ್ಣ *full_name)
{
	ವಿಕಲ್ಪ (e->item->type) {
	ಪಕ್ಷ OBJ_COMMIT:
		ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ commit *)e->item;
	ಪಕ್ಷ OBJ_TAG: {
		ಸಂರಚನಾ tag *tag = (ಸಂರಚನಾ tag *)e->item;

		/* handle nested tags */
		ಯಾವತ್ (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (ಸಂರಚನಾ tag *)tag->tagged;
		}
		ಯದಿ (!tag)
			die(_("tag %s points nowhere?"), e->name);
		ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ commit *)tag;
	}
	ಸ್ವಭಾವ:
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ get_tags_and_duplicates(ಸಂರಚನಾ rev_cmdline_info *info)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < info->nr; i++) {
		ಸಂರಚನಾ rev_cmdline_entry *e = info->rev + i;
		ಸಂರಚನಾ object_id oid;
		ಸಂರಚನಾ commit *commit;
		ವರ್ಣ *full_name = ರಿಕ್ತ;

		ಯದಿ (e->flags & UNINTERESTING)
			ಅನುವರ್ತ;

		ಯದಿ (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			ಮುಕ್ತ(full_name);
			ಅನುವರ್ತ;
		}

		ಯದಿ (refspecs.nr) {
			ವರ್ಣ *ಗುಪ್ತ;
			ಗುಪ್ತ = apply_refspecs(&refspecs, full_name);
			ಯದಿ (ಗುಪ್ತ) {
				ಮುಕ್ತ(full_name);
				full_name = ಗುಪ್ತ;
			}
		}

		commit = get_commit(e, full_name);
		ಯದಿ (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			ಮುಕ್ತ(full_name);
			ಅನುವರ್ತ;
		}

		ವಿಕಲ್ಪ (commit->object.type) {
		ಪಕ್ಷ OBJ_COMMIT:
			ಭಙ್ಗ;
		ಪಕ್ಷ OBJ_BLOB:
			export_blob(&commit->object.oid);
			ಮುಕ್ತ(full_name);
			ಅನುವರ್ತ;
		ಸ್ವಭಾವ: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			ಮುಕ್ತ(full_name);
			ಅನುವರ್ತ;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		ಯದಿ (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		ಯದಿ (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		ಅನ್ಯಥಾ
			ಮುಕ್ತ(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_tags_and_duplicates(ಸಂರಚನಾ string_list *extras)
{
	ಸಂರಚನಾ commit *commit;
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = extras->nr - 1; i >= 0; i--) {
		ಧ್ರುವ ವರ್ಣ *name = extras->ವಸ್ತುಗಳು[i].string;
		ಸಂರಚನಾ object *object = extras->ವಸ್ತುಗಳು[i].util;
		ಪೂರ್ಣಾಙ್ಕ mark;

		ವಿಕಲ್ಪ (object->type) {
		ಪಕ್ಷ OBJ_TAG:
			handle_tag(name, (ಸಂರಚನಾ tag *)object);
			ಭಙ್ಗ;
		ಪಕ್ಷ OBJ_COMMIT:
			ಯದಿ (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((ಸಂರಚನಾ commit *)object);
			ಯದಿ (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				printf("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				ಅನುವರ್ತ;
			}

			mark = get_object_mark(&commit->object);
			ಯದಿ (!mark) {
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
				ಯದಿ (!reference_excluded_commits) {
					/* delete the ref */
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					ಅನುವರ್ತ;
				}
				/* set ref to commit using oid, not mark */
				printf("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				ಅನುವರ್ತ;
			}

			printf("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			ಭಙ್ಗ;
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ export_marks(ವರ್ಣ *file)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ i;
	uint32_t mark;
	ಸಂರಚನಾ decoration_entry *deco = idnums.entries;
	FILE *f;
	ಪೂರ್ಣಾಙ್ಕ e = 0;

	f = fopen_for_writing(file);
	ಯದಿ (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	ಪ್ರತಿ (i = 0; i < idnums.size; i++) {
		ಯದಿ (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			ಯದಿ (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    ಭಙ್ಗ;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	ಯದಿ (e)
		error(_("unable to write marks file %s."), file);
}

ಸ್ಥಿರ ಶೂನ್ಯ import_marks(ವರ್ಣ *ಒಳಹರಿವು_file, ಪೂರ್ಣಾಙ್ಕ check_exists)
{
	ವರ್ಣ ಸಾಲು[512];
	FILE *f;
	ಸಂರಚನಾ stat sb;

	ಯದಿ (check_exists && stat(ಒಳಹರಿವು_file, &sb))
		ಪ್ರತ್ಯಾಗಮ;

	f = xfopen(ಒಳಹರಿವು_file, "r");
	ಯಾವತ್ (fgets(ಸಾಲು, ಪರಿಮಾಣ(ಸಾಲು), f)) {
		uint32_t mark;
		ವರ್ಣ *ಸಾಲು_end, *mark_end;
		ಸಂರಚನಾ object_id oid;
		ಸಂರಚನಾ object *object;
		ಸಂರಚನಾ commit *commit;
		ಗಣನಾ object_type type;

		ಸಾಲು_end = strchr(ಸಾಲು, '\n');
		ಯದಿ (ಸಾಲು[0] != ':' || !ಸಾಲು_end)
			die(_("corrupt mark line: %s"), ಸಾಲು);
		*ಸಾಲು_end = '\0';

		mark = strtoumax(ಸಾಲು + 1, &mark_end, 10);
		ಯದಿ (!mark || mark_end == ಸಾಲು + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), ಸಾಲು);

		ಯದಿ (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, ರಿಕ್ತ);
		ಯದಿ (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		ಯದಿ (type != OBJ_COMMIT)
			/* only commits */
			ಅನುವರ್ತ;

		commit = lookup_commit(the_repository, &oid);
		ಯದಿ (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		ಯದಿ (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_deletes(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪ್ರತಿ (i = 0; i < refspecs.nr; i++) {
		ಸಂರಚನಾ refspec_item *refspec = &refspecs.ವಸ್ತುಗಳು[i];
		ಯದಿ (*refspec->src)
			ಅನುವರ್ತ;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_anonymize_map(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				   ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ hashmap *map = opt->ಮೌಲ್ಯ;
	ಧ್ರುವ ವರ್ಣ *delim, *ಮೌಲ್ಯ;
	size_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	ಯದಿ (delim) {
		keylen = delim - arg;
		ಮೌಲ್ಯ = delim + 1;
	} ಅನ್ಯಥಾ {
		keylen = strlen(arg);
		ಮೌಲ್ಯ = arg;
	}

	ಯದಿ (!keylen || !*ಮೌಲ್ಯ)
		ಪ್ರತ್ಯಾಗಮ error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(ಮೌಲ್ಯ));

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_fast_export(ಪೂರ್ಣಾಙ್ಕ argc,
		    ಧ್ರುವ ವರ್ಣ **argv,
		    ಧ್ರುವ ವರ್ಣ *prefix,
		    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ rev_info revs;
	ಸಂರಚನಾ commit *commit;
	ವರ್ಣ *export_filename = ರಿಕ್ತ,
	     *import_filename = ರಿಕ್ತ,
	     *import_filename_if_exists = ರಿಕ್ತ;
	uint32_t lastimportid;
	ಸಂರಚನಾ string_list refspecs_list = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ option options[] = {
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
		OPT_BOOL(0, "full-tree", &full_ವೃಕ್ಷ,
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

	ಯದಿ (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, prefix);
	init_revision_sources(&revision_sources);
	revs.topo_order = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, prefix, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = setup_revisions(argc, argv, &revs, ರಿಕ್ತ);
	ಯದಿ (argc > 1)
		usage_with_options (fast_export_usage, options);

	ಯದಿ (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	ಯದಿ (refspecs_list.nr) {
		ಪೂರ್ಣಾಙ್ಕ i;

		ಪ್ರತಿ (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.ವಸ್ತುಗಳು[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	ಯದಿ (use_done_feature)
		printf("feature done\n");

	ಯದಿ (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	ಯದಿ (import_filename)
		import_marks(import_filename, 0);
	ಅನ್ಯಥಾ ಯದಿ (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	ಯದಿ (import_filename && revs.prune_data.nr)
		full_ವೃಕ್ಷ = 1;

	get_tags_and_duplicates(&revs.cmdline);

	ಯದಿ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	ಯಾವತ್ ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	ಯದಿ (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	ಯದಿ (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	ಪ್ರತ್ಯಾಗಮ 0;
}
