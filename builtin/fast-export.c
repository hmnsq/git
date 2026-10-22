/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ;
ಸ್ಥಿರ ಎಣಿಕೆ sign_ಬಹುಲಕ signed_ಟ್ಯಾಗ್_ಬಹುಲಕ = SIGN_ABORT;
ಸ್ಥಿರ ಎಣಿಕೆ sign_ಬಹುಲಕ signed_commit_ಬಹುಲಕ = SIGN_STRIP;
ಸ್ಥಿರ ಎಣಿಕೆ ಟ್ಯಾಗ್_ದ_filtered_ಬಹುಲಕ { ಟ್ಯಾಗ್_FILTERING_ABORT, DROP, REWRITE } ಟ್ಯಾಗ್_ದ_filtered_ಬಹುಲಕ = ಟ್ಯಾಗ್_FILTERING_ABORT;
ಸ್ಥಿರ ಎಣಿಕೆ reencode_ಬಹುಲಕ { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_ಬಹುಲಕ = REENCODE_ABORT;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fake_missing_tagger;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_ಮುಗಿದ_feature;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ no_ದತ್ತಾಂಶ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ತುಂಬಿದ_ವೃಕ್ಷ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reference_excluded_commits;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_original_ids;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_ಟ್ಯಾಗುಗಳು;
ಸ್ಥಿರ ರಚನೆ string_list extra_refs = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ ರಚನೆ string_list ಟ್ಯಾಗ್_refs = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ ರಚನೆ refspec refspecs;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ anonymize;
ಸ್ಥಿರ ರಚನೆ hashmap anonymized_seeds;
ಸ್ಥಿರ ರಚನೆ revision_sources revision_sources;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_sign_ಬಹುಲಕ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಎಣಿಕೆ sign_ಬಹುಲಕ *val = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (parse_sign_ಬಹುಲಕ(arg, val, ಖಾಲಿ) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		ಹಿಂತಿರುಗಿ error(_("unknown %s mode: %s"), opt->long_name, arg);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_ಟ್ಯಾಗ್_ದ_filtered_ಬಹುಲಕ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
					  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಎಣಿಕೆ ಟ್ಯಾಗ್_ದ_filtered_ಬಹುಲಕ *val = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset || !strcmp(arg, "abort"))
		*val = ಟ್ಯಾಗ್_FILTERING_ABORT;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "drop"))
		*val = DROP;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("unknown tag-of-filtered mode: %s"), arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_reencode_ಬಹುಲಕ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಎಣಿಕೆ reencode_ಬಹುಲಕ *val = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset) {
		*val = REENCODE_ABORT;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆಯ್ಕೆ (git_parse_maybe_bool(arg)) {
	ಸ್ಥಿತಿ 0:
		*val = REENCODE_NO;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ 1:
		*val = REENCODE_YES;
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಆದರೆ (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		ಇಲ್ಲದಿದ್ದರೆ
			ಹಿಂತಿರುಗಿ error(_("unknown reencoding mode: %s"), arg);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ decoration idnums;
ಸ್ಥಿರ uint32_t ಕೊನೆಯ_idnum;
ರಚನೆ anonymized_ನಮೂದು {
	ರಚನೆ hashmap_ನಮೂದು hash;
	ಅಕ್ಷರ *anon;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ orig[FLEX_ARRAY];
};

ರಚನೆ anonymized_ನಮೂದು_ಕೀಲಿ {
	ರಚನೆ hashmap_ನಮೂದು hash;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *orig;
	ಗಾತ್ರ_t orig_len;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ anonymized_ನಮೂದು_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *cmp_ದತ್ತಾಂಶ ಬಳಸದ,
				ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *eptr,
				ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *ನಮೂದು_or_ಕೀಲಿ,
				ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *keydata)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ anonymized_ನಮೂದು *a, *b;

	a = container_ದ(eptr, ಸ್ಥಿರಾಂಕ ರಚನೆ anonymized_ನಮೂದು, hash);
	ಆದರೆ (keydata) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ anonymized_ನಮೂದು_ಕೀಲಿ *ಕೀಲಿ = keydata;
		ಪೂರ್ಣಾಂಕ equal = !xstrncmpz(a->orig, ಕೀಲಿ->orig, ಕೀಲಿ->orig_len);
		ಹಿಂತಿರುಗಿ !equal;
	}

	b = container_ದ(ನಮೂದು_or_ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ರಚನೆ anonymized_ನಮೂದು, hash);
	ಹಿಂತಿರುಗಿ strcmp(a->orig, b->orig);
}

ಸ್ಥಿರ ರಚನೆ anonymized_ನಮೂದು *ಕೂಡಿಸು_anonymized_ನಮೂದು(ರಚನೆ hashmap *map,
						     ಚಿಹ್ನೆರಹಿತ hash,
						     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *orig, ಗಾತ್ರ_t len,
						     ಅಕ್ಷರ *anon)
{
	ರಚನೆ anonymized_ನಮೂದು *ret, *old;

	ಆದರೆ (!map->cmpfn)
		hashmap_init(map, anonymized_ನಮೂದು_cmp, ಖಾಲಿ, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_ನಮೂದು_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_ನಮೂದು(map, ret, hash);

	ಆದರೆ (old) {
		ಬಿಡುಗಡೆ(old->anon);
		ಬಿಡುಗಡೆ(old);
	}

	ಹಿಂತಿರುಗಿ ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *anonymize_str(ರಚನೆ hashmap *map,
				 ಅಕ್ಷರ *(*generate)(ಶೂನ್ಯ),
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *orig, ಗಾತ್ರ_t len)
{
	ರಚನೆ anonymized_ನಮೂದು_ಕೀಲಿ ಕೀಲಿ;
	ರಚನೆ anonymized_ನಮೂದು *ret;

	hashmap_ನಮೂದು_init(&ಕೀಲಿ.hash, memhash(orig, len));
	ಕೀಲಿ.orig = orig;
	ಕೀಲಿ.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_ನಮೂದು(&anonymized_seeds, &ಕೀಲಿ, hash, &ಕೀಲಿ);

	/* ...otherwise check if we've already seen it in this context... */
	ಆದರೆ (!ret)
		ret = hashmap_get_ನಮೂದು(map, &ಕೀಲಿ, hash, &ಕೀಲಿ);

	/* ...and finally generate a new mapping if necessary */
	ಆದರೆ (!ret)
		ret = ಕೂಡಿಸು_anonymized_ನಮೂದು(map, ಕೀಲಿ.hash.hash,
					   orig, len, generate());

	ಹಿಂತಿರುಗಿ ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
ಸ್ಥಿರ ಶೂನ್ಯ anonymize_ಮಾರ್ಗ(ರಚನೆ strbuf *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
			   ರಚನೆ hashmap *map,
			   ಅಕ್ಷರ *(*generate)(ಶೂನ್ಯ))
{
	ಆಗ (*ಮಾರ್ಗ) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ_ದ_component = strchrnul(ಮಾರ್ಗ, '/');
		ಗಾತ್ರ_t len = ಕೊನೆ_ದ_component - ಮಾರ್ಗ;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *c = anonymize_str(map, generate, ಮಾರ್ಗ, len);
		strbuf_addstr(ಹೊರಹೋಗು, c);
		ಮಾರ್ಗ = ಕೊನೆ_ದ_component;
		ಆದರೆ (*ಮಾರ್ಗ)
			strbuf_addch(ಹೊರಹೋಗು, *ಮಾರ್ಗ++);
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ *mark_ಗಮ್ಯ_ptr(uint32_t mark)
{
	ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ *)(uintptr_t)mark;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ uint32_t ptr_ಗಮ್ಯ_mark(ಶೂನ್ಯ * mark)
{
	ಹಿಂತಿರುಗಿ (uint32_t)(uintptr_t)mark;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ mark_object(ರಚನೆ object *object, uint32_t mark)
{
	ಕೂಡಿಸು_decoration(&idnums, object, mark_ಗಮ್ಯ_ptr(mark));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ mark_ಮುಂದಿನ_object(ರಚನೆ object *object)
{
	mark_object(object, ++ಕೊನೆಯ_idnum);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_object_mark(ರಚನೆ object *object)
{
	ಶೂನ್ಯ *decoration = lookup_decoration(&idnums, object);
	ಆದರೆ (!decoration)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ ptr_ಗಮ್ಯ_mark(decoration);
}

ಸ್ಥಿರ ರಚನೆ commit *rewrite_commit(ರಚನೆ commit *p)
{
	ಗಾಗಿ (;;) {
		ಆದರೆ (p->parents && p->parents->ಮುಂದಿನ)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (p->object.flags & UNINTERESTING)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (!(p->object.flags & TREESAME))
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (!p->parents)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		p = p->parents->ಅಂಶ;
	}
	ಹಿಂತಿರುಗಿ p;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ಪ್ರಗತಿ(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ counter = 0;
	ಆದರೆ (!ಪ್ರಗತಿ)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ ((++counter % ಪ್ರಗತಿ) == 0)
		ಬರೆ("progress %d objects\n", counter);
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
ಸ್ಥಿರ ಅಕ್ಷರ *anonymize_blob(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *ಗಾತ್ರ_2)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ counter;
	ರಚನೆ strbuf ಹೊರಹೋಗು = STRBUF_INIT;
	strbuf_addf(&ಹೊರಹೋಗು, "anonymous blob %d", counter++);
	*ಗಾತ್ರ_2 = ಹೊರಹೋಗು.len;
	ಹಿಂತಿರುಗಿ strbuf_detach(&ಹೊರಹೋಗು, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ export_blob(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;
	ಎಣಿಕೆ object_type type;
	ಅಕ್ಷರ *buf;
	ರಚನೆ object *object;
	ಪೂರ್ಣಾಂಕ eaten;

	ಆದರೆ (no_ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (is_null_oid(oid))
		ಹಿಂತಿರುಗಿ;

	object = lookup_object(the_repository, oid);
	ಆದರೆ (object && object->flags & SHOWN)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (anonymize) {
		buf = anonymize_blob(&ಗಾತ್ರ_2);
		object = (ರಚನೆ object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಗಾತ್ರ_t ಗಾತ್ರ_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &ಗಾತ್ರ_st);
		ಗಾತ್ರ_2 = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(ಗಾತ್ರ_st);
		ಆದರೆ (!buf)
			die(_("could not read blob %s"), oid_ಗಮ್ಯ_hex(oid));
		ಆದರೆ (check_object_signature(the_repository, oid, buf, ಗಾತ್ರ_2,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_ಗಮ್ಯ_hex(oid));
		object = parse_object_ಬಫರ್(the_repository, oid, type,
					     ಗಾತ್ರ_2, buf, &eaten);
	}

	ಆದರೆ (!object)
		die(_("could not read blob %s"), oid_ಗಮ್ಯ_hex(oid));

	mark_ಮುಂದಿನ_object(object);

	ಬರೆ("blob\nmark :%"PRIu32"\n", ಕೊನೆಯ_idnum);
	ಆದರೆ (show_original_ids)
		ಬರೆ("original-oid %s\n", oid_ಗಮ್ಯ_hex(oid));
	ಬರೆ("data %"PRIuMAX"\n", (uintmax_t)ಗಾತ್ರ_2);
	ಆದರೆ (ಗಾತ್ರ_2 && fwrite(buf, ಗಾತ್ರ_2, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_ಗಮ್ಯ_hex(oid));
	ಬರೆ("\n");

	show_ಪ್ರಗತಿ();

	object->flags |= SHOWN;
	ಆದರೆ (!eaten)
		ಬಿಡುಗಡೆ(buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಆಳ_ಮೊದಲ(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a_, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b_)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ diff_filepair *a = *((ಸ್ಥಿರಾಂಕ ರಚನೆ diff_filepair **)a_);
	ಸ್ಥಿರಾಂಕ ರಚನೆ diff_filepair *b = *((ಸ್ಥಿರಾಂಕ ರಚನೆ diff_filepair **)b_);
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name_a, *name_b;
	ಪೂರ್ಣಾಂಕ len_a, len_b, len;
	ಪೂರ್ಣಾಂಕ cmp;

	name_a = a->one ? a->one->ಮಾರ್ಗ : a->two->ಮಾರ್ಗ;
	name_b = b->one ? b->one->ಮಾರ್ಗ : b->two->ಮಾರ್ಗ;

	len_a = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name_a);
	len_b = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	ಆದರೆ (cmp)
		ಹಿಂತಿರುಗಿ cmp;
	cmp = len_b - len_a;
	ಆದರೆ (cmp)
		ಹಿಂತಿರುಗಿ cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	ಹಿಂತಿರುಗಿ (a->ಸ್ಥಿತಿ_2 == 'R') - (b->ಸ್ಥಿತಿ_2 == 'R');
}

ಸ್ಥಿರ ಶೂನ್ಯ print_ಮಾರ್ಗ_1(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಪೂರ್ಣಾಂಕ need_quote = quote_c_style(ಮಾರ್ಗ, ಖಾಲಿ, ಖಾಲಿ, 0);
	ಆದರೆ (need_quote)
		quote_c_style(ಮಾರ್ಗ, ಖಾಲಿ, stdout, 0);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strchr(ಮಾರ್ಗ, ' '))
		ಬರೆ("\"%s\"", ಮಾರ್ಗ);
	ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ("%s", ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಅಕ್ಷರ *anonymize_ಮಾರ್ಗ_component(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ counter;
	ರಚನೆ strbuf ಹೊರಹೋಗು = STRBUF_INIT;
	strbuf_addf(&ಹೊರಹೋಗು, "path%d", counter++);
	ಹಿಂತಿರುಗಿ strbuf_detach(&ಹೊರಹೋಗು, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಆದರೆ (!anonymize)
		print_ಮಾರ್ಗ_1(ಮಾರ್ಗ);
	ಇಲ್ಲದಿದ್ದರೆ {
		ಸ್ಥಿರ ರಚನೆ hashmap paths;
		ಸ್ಥಿರ ರಚನೆ strbuf anon = STRBUF_INIT;

		anonymize_ಮಾರ್ಗ(&anon, ಮಾರ್ಗ, &paths, anonymize_ಮಾರ್ಗ_component);
		print_ಮಾರ್ಗ_1(anon.buf);
		strbuf_reset(&anon);
	}
}

ಸ್ಥಿರ ಅಕ್ಷರ *generate_fake_oid(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ uint32_t counter = 1; /* avoid null oid */
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ hashsz = the_hash_algo->rawsz;
	ರಚನೆ object_id oid;
	ಅಕ್ಷರ *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	ಹಿಂತಿರುಗಿ oid_ಗಮ್ಯ_hex_r(hex, &oid);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *anonymize_oid(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *oid_hex)
{
	ಸ್ಥಿರ ರಚನೆ hashmap objs;
	ಗಾತ್ರ_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(oid_hex);
	ಹಿಂತಿರುಗಿ anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_filemodify(ರಚನೆ diff_ಸರತಿ_struct *q,
			    ರಚನೆ diff_options *options ಬಳಸದ, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ string_list *ಬದಲಾಯಿತು = ದತ್ತಾಂಶ;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->ಸರತಿ, q->nr, ಆಳ_ಮೊದಲ);

	ಗಾಗಿ (i = 0; i < q->nr; i++) {
		ರಚನೆ diff_filespec *ospec = q->ಸರತಿ[i]->one;
		ರಚನೆ diff_filespec *spec = q->ಸರತಿ[i]->two;

		ಆಯ್ಕೆ (q->ಸರತಿ[i]->ಸ್ಥಿತಿ_2) {
		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_DELETED:
			ಬರೆ("D ");
			print_ಮಾರ್ಗ(spec->ಮಾರ್ಗ);
			string_list_ಸೇರಿಸು(ಬದಲಾಯಿತು, spec->ಮಾರ್ಗ);
			ಅಕ್ಷರ_ಬರೆ('\n');
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_COPIED:
		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			ಆದರೆ (!string_list_has_string(ಬದಲಾಯಿತು, ospec->ಮಾರ್ಗ)) {
				ಬರೆ("%c ", q->ಸರತಿ[i]->ಸ್ಥಿತಿ_2);
				print_ಮಾರ್ಗ(ospec->ಮಾರ್ಗ);
				ಅಕ್ಷರ_ಬರೆ(' ');
				print_ಮಾರ್ಗ(spec->ಮಾರ್ಗ);
				string_list_ಸೇರಿಸು(ಬದಲಾಯಿತು, spec->ಮಾರ್ಗ);
				ಅಕ್ಷರ_ಬರೆ('\n');

				ಆದರೆ (oideq(&ospec->oid, &spec->oid) &&
				    ospec->ಬಹುಲಕ == spec->ಬಹುಲಕ)
					ನಿಲ್ಲಿಸು;
			}
			/* fallthrough */

		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_TYPE_ಬದಲಾಯಿತು:
		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_MODIFIED:
		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			ಆದರೆ (no_ದತ್ತಾಂಶ || S_ISGITLINK(spec->ಬಹುಲಕ))
				ಬರೆ("M %06o %s ", spec->ಬಹುಲಕ,
				       anonymize ?
				       anonymize_oid(oid_ಗಮ್ಯ_hex(&spec->oid)) :
				       oid_ಗಮ್ಯ_hex(&spec->oid));
			ಇಲ್ಲದಿದ್ದರೆ {
				ರಚನೆ object *object = lookup_object(the_repository,
								      &spec->oid);
				ಬರೆ("M %06o :%d ", spec->ಬಹುಲಕ,
				       get_object_mark(object));
			}
			print_ಮಾರ್ಗ(spec->ಮಾರ್ಗ);
			string_list_ಸೇರಿಸು(ಬದಲಾಯಿತು, spec->ಮಾರ್ಗ);
			ಅಕ್ಷರ_ಬರೆ('\n');
			ನಿಲ್ಲಿಸು;

		ಪೂರ್ವನಿಯೋಜಿತ:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->ಸರತಿ[i]->ಸ್ಥಿತಿ_2,
			    ospec->ಮಾರ್ಗ ? ospec->ಮಾರ್ಗ : _("none"),
			    spec->ಮಾರ್ಗ ? spec->ಮಾರ್ಗ : _("none"));
		}
	}
}

ಸ್ಥಿರ ಅಕ್ಷರ *anonymize_ref_component(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ counter;
	ರಚನೆ strbuf ಹೊರಹೋಗು = STRBUF_INIT;
	strbuf_addf(&ಹೊರಹೋಗು, "ref%d", counter++);
	ಹಿಂತಿರುಗಿ strbuf_detach(&ಹೊರಹೋಗು, ಖಾಲಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *anonymize_refname(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	ಸ್ಥಿರ ರಚನೆ hashmap refs;
	ಸ್ಥಿರ ರಚನೆ strbuf anon = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ i;

	strbuf_reset(&anon);
	ಗಾಗಿ (i = 0; i < ARRAY_ಗಾತ್ರ(prefixes); i++) {
		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			ನಿಲ್ಲಿಸು;
		}
	}

	anonymize_ಮಾರ್ಗ(&anon, refname, &refs, anonymize_ref_component);
	ಹಿಂತಿರುಗಿ anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
ಸ್ಥಿರ ಅಕ್ಷರ *anonymize_commit_ಸಂದೇಶ(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ counter;
	ಹಿಂತಿರುಗಿ xstrfmt("subject %d\n\nbody\n", counter++);
}

ಸ್ಥಿರ ಅಕ್ಷರ *anonymize_ident(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ counter;
	ರಚನೆ strbuf ಹೊರಹೋಗು = STRBUF_INIT;
	strbuf_addf(&ಹೊರಹೋಗು, "User %d <user%d@example.com>", counter, counter);
	counter++;
	ಹಿಂತಿರುಗಿ strbuf_detach(&ಹೊರಹೋಗು, ಖಾಲಿ);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
ಸ್ಥಿರ ಶೂನ್ಯ anonymize_ident_ಸಾಲು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **beg, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಕೊನೆ)
{
	ಸ್ಥಿರ ರಚನೆ hashmap idents;
	ಸ್ಥಿರ ರಚನೆ strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ which_ಬಫರ್;

	ರಚನೆ strbuf *ಹೊರಹೋಗು;
	ರಚನೆ ident_ವಿಭಜಿಸು ವಿಭಜಿಸು;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ_ದ_ಶೀರ್ಷಿಕೆ;

	ಹೊರಹೋಗು = &buffers[which_ಬಫರ್++];
	which_ಬಫರ್ %= ARRAY_ಗಾತ್ರ(buffers);
	strbuf_reset(ಹೊರಹೋಗು);

	/* skip "committer", "author", "tagger", etc */
	ಕೊನೆ_ದ_ಶೀರ್ಷಿಕೆ = strchr(*beg, ' ');
	ಆದರೆ (!ಕೊನೆ_ದ_ಶೀರ್ಷಿಕೆ)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (ಪೂರ್ಣಾಂಕ)(*ಕೊನೆ - *beg), *beg);
	ಕೊನೆ_ದ_ಶೀರ್ಷಿಕೆ++;
	strbuf_ಕೂಡಿಸು(ಹೊರಹೋಗು, *beg, ಕೊನೆ_ದ_ಶೀರ್ಷಿಕೆ - *beg);

	ಆದರೆ (!ವಿಭಜಿಸು_ident_ಸಾಲು(&ವಿಭಜಿಸು, ಕೊನೆ_ದ_ಶೀರ್ಷಿಕೆ, *ಕೊನೆ - ಕೊನೆ_ದ_ಶೀರ್ಷಿಕೆ) &&
	    ವಿಭಜಿಸು.date_begin) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ident;
		ಗಾತ್ರ_t len;

		len = ವಿಭಜಿಸು.mail_ಕೊನೆ - ವಿಭಜಿಸು.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      ವಿಭಜಿಸು.name_begin, len);
		strbuf_addstr(ಹೊರಹೋಗು, ident);
		strbuf_addch(ಹೊರಹೋಗು, ' ');
		strbuf_ಕೂಡಿಸು(ಹೊರಹೋಗು, ವಿಭಜಿಸು.date_begin, ವಿಭಜಿಸು.tz_ಕೊನೆ - ವಿಭಜಿಸು.date_begin);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addstr(ಹೊರಹೋಗು, "Malformed Ident <malformed@example.com> 0 -0000");
	}

	*beg = ಹೊರಹೋಗು->buf;
	*ಕೊನೆ = ಹೊರಹೋಗು->buf + ಹೊರಹೋಗು->len;
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
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕಂಡುಹಿಡಿ_commit_multiline_ಶೀರ್ಷಿಕೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ,
						ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ,
						ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಕೊನೆ)
{
	ರಚನೆ strbuf val = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bol, *eol;
	ಗಾತ್ರ_t len;

	bol = ಕಂಡುಹಿಡಿ_commit_ಶೀರ್ಷಿಕೆ(ಸಂದೇಶ, ಕೀಲಿ, &len);
	ಆದರೆ (!bol)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	eol = bol + len;
	strbuf_ಕೂಡಿಸು(&val, bol, len);

	ಆಗ (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_ಕೂಡಿಸು(&val, bol, eol - bol);
	}

	*ಕೊನೆ = eol;
	ಹಿಂತಿರುಗಿ strbuf_detach(&val, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_signature(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *signature, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object_hash)
{
	ಆದರೆ (!signature)
		ಹಿಂತಿರುಗಿ;

	ಬರೆ("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (ಚಿಹ್ನೆರಹಿತ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(signature),
	       signature);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *append_signatures_for_ಶೀರ್ಷಿಕೆ(ರಚನೆ string_list *signatures,
						ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pos,
						ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಶೀರ್ಷಿಕೆ,
						ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object_hash)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *signature;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಆರಂಭ = pos;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ = pos;

	ಆಗ ((signature = ಕಂಡುಹಿಡಿ_commit_multiline_ಶೀರ್ಷಿಕೆ(ಆರಂಭ + 1,
							 ಶೀರ್ಷಿಕೆ,
							 &ಕೊನೆ))) {
		string_list_append(signatures, signature)->util = (ಶೂನ್ಯ *)object_hash;
		ಬಿಡುಗಡೆ((ಅಕ್ಷರ *)signature);
		ಆರಂಭ = ಕೊನೆ;
	}

	ಹಿಂತಿರುಗಿ ಕೊನೆ;
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_commit(ರಚನೆ commit *commit, ರಚನೆ rev_info *rev,
			  ರಚನೆ string_list *paths_ದ_ಬದಲಾಯಿತು_objects)
{
	ಪೂರ್ಣಾಂಕ saved_output_format = rev->diffopt.output_format;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *commit_ಬಫರ್, *commit_ಬಫರ್_cursor;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *author, *author_ಕೊನೆ, *committer, *committer_ಕೊನೆ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *encoding = ಖಾಲಿ;
	ಗಾತ್ರ_t encoding_len;
	ರಚನೆ string_list signatures = STRING_LIST_INIT_DUP;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2;
	ಅಕ್ಷರ *reencoded = ಖಾಲಿ;
	ರಚನೆ commit_list *p;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname;
	ಪೂರ್ಣಾಂಕ i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_ಬಫರ್_cursor = commit_ಬಫರ್ = repo_get_commit_ಬಫರ್(the_repository, commit, ಖಾಲಿ);

	author = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(commit_ಬಫರ್_cursor, "\nauthor ");
	ಆದರೆ (!author)
		die(_("could not find author in commit %s"),
		    oid_ಗಮ್ಯ_hex(&commit->object.oid));
	author++;
	commit_ಬಫರ್_cursor = author_ಕೊನೆ = strchrnul(author, '\n');

	committer = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(commit_ಬಫರ್_cursor, "\ncommitter ");
	ಆದರೆ (!committer)
		die(_("could not find committer in commit %s"),
		    oid_ಗಮ್ಯ_hex(&commit->object.oid));
	committer++;
	commit_ಬಫರ್_cursor = committer_ಕೊನೆ = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	ಆದರೆ (*commit_ಬಫರ್_cursor == '\n') {
		encoding = ಕಂಡುಹಿಡಿ_commit_ಶೀರ್ಷಿಕೆ(commit_ಬಫರ್_cursor + 1, "encoding", &encoding_len);
		ಆದರೆ (encoding)
			commit_ಬಫರ್_cursor = encoding + encoding_len;
	}

	ಆದರೆ (*commit_ಬಫರ್_cursor == '\n') {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *after_sha1 = append_signatures_for_ಶೀರ್ಷಿಕೆ(&signatures, commit_ಬಫರ್_cursor,
								      "gpgsig", "sha1");
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *after_sha256 = append_signatures_for_ಶೀರ್ಷಿಕೆ(&signatures, commit_ಬಫರ್_cursor,
									"gpgsig-sha256", "sha256");
		commit_ಬಫರ್_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	ಸಂದೇಶ_2 = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(commit_ಬಫರ್_cursor, "\n\n");
	ಆದರೆ (ಸಂದೇಶ_2)
		ಸಂದೇಶ_2 += 2;

	ಆದರೆ (commit->parents &&
	    (get_object_mark(&commit->parents->ಅಂಶ->object) != 0 ||
	     reference_excluded_commits) &&
	    !ತುಂಬಿದ_ವೃಕ್ಷ) {
		parse_commit_or_die(commit->parents->ಅಂಶ);
		diff_ವೃಕ್ಷ_oid(get_commit_ವೃಕ್ಷ_oid(commit->parents->ಅಂಶ),
			      get_commit_ವೃಕ್ಷ_oid(commit), "", &rev->diffopt);
	}
	ಇಲ್ಲದಿದ್ದರೆ
		diff_ಬೇರು_ವೃಕ್ಷ_oid(get_commit_ವೃಕ್ಷ_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	ಗಾಗಿ (i = 0; i < diff_queued_diff.nr; i++)
		ಆದರೆ (!S_ISGITLINK(diff_queued_diff.ಸರತಿ[i]->two->ಬಹುಲಕ))
			export_blob(&diff_queued_diff.ಸರತಿ[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	ಆದರೆ (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_ಸಾಲು(&committer, &committer_ಕೊನೆ);
		anonymize_ident_ಸಾಲು(&author, &author_ಕೊನೆ);
	}

	mark_ಮುಂದಿನ_object(&commit->object);
	ಆದರೆ (anonymize) {
		reencoded = anonymize_commit_ಸಂದೇಶ();
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (encoding) {
		ಅಕ್ಷರ *buf;
		ಆಯ್ಕೆ (reencode_ಬಹುಲಕ) {
		ಸ್ಥಿತಿ REENCODE_YES:
			buf = xstrfmt("%.*s", (ಪೂರ್ಣಾಂಕ)encoding_len, encoding);
			reencoded = reencode_string(ಸಂದೇಶ_2, "UTF-8", buf);
			ಬಿಡುಗಡೆ(buf);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ REENCODE_NO:
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (ಪೂರ್ಣಾಂಕ)encoding_len, encoding,
			    oid_ಗಮ್ಯ_hex(&commit->object.oid));
		}
	}
	ಆದರೆ (!commit->parents)
		ಬರೆ("reset %s\n", refname);
	ಬರೆ("commit %s\nmark :%"PRIu32"\n", refname, ಕೊನೆಯ_idnum);
	ಆದರೆ (show_original_ids)
		ಬರೆ("original-oid %s\n", oid_ಗಮ್ಯ_hex(&commit->object.oid));
	ಬರೆ("%.*s\n%.*s\n",
	       (ಪೂರ್ಣಾಂಕ)(author_ಕೊನೆ - author), author,
	       (ಪೂರ್ಣಾಂಕ)(committer_ಕೊನೆ - committer), committer);
	ಆದರೆ (signatures.nr) {
		ಆಯ್ಕೆ (signed_commit_ಬಹುಲಕ) {
		/* Exporting modes */
		ಸ್ಥಿತಿ SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_ಗಮ್ಯ_hex(&commit->object.oid));
			/* fallthru */
		ಸ್ಥಿತಿ SIGN_VERBATIM:
			ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < signatures.nr; i++) {
				ರಚನೆ string_list_ಅಂಶ *ಅಂಶ = &signatures.ವಸ್ತುಗಳು[i];
				print_signature(ಅಂಶ->string, ಅಂಶ->util);
			}
			ನಿಲ್ಲಿಸು;

		/* Stripping modes */
		ಸ್ಥಿತಿ SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_ಗಮ್ಯ_hex(&commit->object.oid));
			/* fallthru */
		ಸ್ಥಿತಿ SIGN_STRIP:
			ನಿಲ್ಲಿಸು;

		/* Aborting modes */
		ಸ್ಥಿತಿ SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_ಗಮ್ಯ_hex(&commit->object.oid));
		ಪೂರ್ವನಿಯೋಜಿತ:
			BUG("invalid signed_commit_mode value %d", signed_commit_ಬಹುಲಕ);
		}
		string_list_clear(&signatures, 0);
	}
	ಆದರೆ (!reencoded && encoding)
		ಬರೆ("encoding %.*s\n", (ಪೂರ್ಣಾಂಕ)encoding_len, encoding);
	ಬರೆ("data %u\n%s",
	       (ಚಿಹ್ನೆರಹಿತ)(reencoded
			  ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(reencoded) : ಸಂದೇಶ_2
			  ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಸಂದೇಶ_2) : 0),
	       reencoded ? reencoded : ಸಂದೇಶ_2 ? ಸಂದೇಶ_2 : "");
	ಬಿಡುಗಡೆ(reencoded);
	repo_unuse_commit_ಬಫರ್(the_repository, commit, commit_ಬಫರ್);

	ಗಾಗಿ (i = 0, p = commit->parents; p; p = p->ಮುಂದಿನ) {
		ರಚನೆ object *obj = &p->ಅಂಶ->object;
		ಪೂರ್ಣಾಂಕ mark = get_object_mark(obj);

		ಆದರೆ (!mark && !reference_excluded_commits)
			ಮುಂದುವರಿ;
		ಆದರೆ (i == 0)
			ಬರೆ("from ");
		ಇಲ್ಲದಿದ್ದರೆ
			ಬರೆ("merge ");
		ಆದರೆ (mark)
			ಬರೆ(":%d\n", mark);
		ಇಲ್ಲದಿದ್ದರೆ
			ಬರೆ("%s\n",
			       anonymize ?
			       anonymize_oid(oid_ಗಮ್ಯ_hex(&obj->oid)) :
			       oid_ಗಮ್ಯ_hex(&obj->oid));
		i++;
	}

	ಆದರೆ (ತುಂಬಿದ_ವೃಕ್ಷ)
		ಬರೆ("deleteall\n");
	log_ವೃಕ್ಷ_diff_flush(rev);
	string_list_clear(paths_ದ_ಬದಲಾಯಿತು_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	ಬರೆ("\n");

	show_ಪ್ರಗತಿ();
}

ಸ್ಥಿರ ಅಕ್ಷರ *anonymize_ಟ್ಯಾಗ್(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ counter;
	ರಚನೆ strbuf ಹೊರಹೋಗು = STRBUF_INIT;
	strbuf_addf(&ಹೊರಹೋಗು, "tag message %d", counter++);
	ಹಿಂತಿರುಗಿ strbuf_detach(&ಹೊರಹೋಗು, ಖಾಲಿ);
}


ಸ್ಥಿರ ಶೂನ್ಯ handle_ಟ್ಯಾಗ್(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ರಚನೆ ಟ್ಯಾಗ್ *ಟ್ಯಾಗ್)
{
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
	ಎಣಿಕೆ object_type type;
	ಅಕ್ಷರ *buf;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tagger, *tagger_ಕೊನೆ, *ಸಂದೇಶ_2;
	ಗಾತ್ರ_t ಸಂದೇಶ_ಗಾತ್ರ = 0;
	ರಚನೆ object *tagged;
	ಪೂರ್ಣಾಂಕ tagged_mark;
	ರಚನೆ commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = ಟ್ಯಾಗ್->tagged;
	ಆಗ (tagged->type == OBJ_ಟ್ಯಾಗ್) {
		tagged = ((ರಚನೆ ಟ್ಯಾಗ್ *)tagged)->tagged;
	}
	ಆದರೆ (tagged->type == OBJ_ವೃಕ್ಷ) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_ಗಮ್ಯ_hex(&ಟ್ಯಾಗ್->object.oid));
		ಹಿಂತಿರುಗಿ;
	}

	buf = odb_read_object(the_repository->objects, &ಟ್ಯಾಗ್->object.oid,
			      &type, &ಗಾತ್ರ_2);
	ಆದರೆ (!buf)
		die(_("could not read tag %s"), oid_ಗಮ್ಯ_hex(&ಟ್ಯಾಗ್->object.oid));
	ಸಂದೇಶ_2 = memmem(buf, ಗಾತ್ರ_2, "\n\n", 2);
	ಆದರೆ (ಸಂದೇಶ_2) {
		ಸಂದೇಶ_2 += 2;
		ಸಂದೇಶ_ಗಾತ್ರ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಸಂದೇಶ_2);
	}
	tagger = memmem(buf, ಸಂದೇಶ_2 ? ಸಂದೇಶ_2 - buf : ಗಾತ್ರ_2, "\ntagger ", 8);
	ಆದರೆ (!tagger) {
		ಆದರೆ (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		ಇಲ್ಲದಿದ್ದರೆ
			tagger = "";
		tagger_ಕೊನೆ = tagger + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(tagger);
	} ಇಲ್ಲದಿದ್ದರೆ {
		tagger++;
		tagger_ಕೊನೆ = strchrnul(tagger, '\n');
		ಆದರೆ (anonymize)
			anonymize_ident_ಸಾಲು(&tagger, &tagger_ಕೊನೆ);
	}

	ಆದರೆ (anonymize) {
		name = anonymize_refname(name);
		ಆದರೆ (ಸಂದೇಶ_2) {
			ಸ್ಥಿರ ರಚನೆ hashmap ಟ್ಯಾಗುಗಳು;
			ಸಂದೇಶ_2 = anonymize_str(&ಟ್ಯಾಗುಗಳು, anonymize_ಟ್ಯಾಗ್,
						ಸಂದೇಶ_2, ಸಂದೇಶ_ಗಾತ್ರ);
			ಸಂದೇಶ_ಗಾತ್ರ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಸಂದೇಶ_2);
		}
	}

	/* handle signed tags */
	ಆದರೆ (ಸಂದೇಶ_2) {
		ಗಾತ್ರ_t sig_ಸ್ಥಾನಾಂತರ = parse_signed_ಬಫರ್(ಸಂದೇಶ_2, ಸಂದೇಶ_ಗಾತ್ರ);
		ಆದರೆ (sig_ಸ್ಥಾನಾಂತರ < ಸಂದೇಶ_ಗಾತ್ರ)
			ಆಯ್ಕೆ (signed_ಟ್ಯಾಗ್_ಬಹುಲಕ) {
			/* Exporting modes */
			ಸ್ಥಿತಿ SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_ಗಮ್ಯ_hex(&ಟ್ಯಾಗ್->object.oid));
				/* fallthru */
			ಸ್ಥಿತಿ SIGN_VERBATIM:
				ನಿಲ್ಲಿಸು;

			/* Stripping modes */
			ಸ್ಥಿತಿ SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_ಗಮ್ಯ_hex(&ಟ್ಯಾಗ್->object.oid));
				/* fallthru */
			ಸ್ಥಿತಿ SIGN_STRIP:
				ಸಂದೇಶ_ಗಾತ್ರ = sig_ಸ್ಥಾನಾಂತರ;
				ನಿಲ್ಲಿಸು;

			/* Aborting modes */
			ಸ್ಥಿತಿ SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_ಗಮ್ಯ_hex(&ಟ್ಯಾಗ್->object.oid));
			ಪೂರ್ವನಿಯೋಜಿತ:
				BUG("invalid signed_commit_mode value %d", signed_commit_ಬಹುಲಕ);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = ಟ್ಯಾಗ್->tagged;
	tagged_mark = get_object_mark(tagged);
	ಆದರೆ (!tagged_mark) {
		ಆಯ್ಕೆ (ಟ್ಯಾಗ್_ದ_filtered_ಬಹುಲಕ) {
		ಸ್ಥಿತಿ ಟ್ಯಾಗ್_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_ಗಮ್ಯ_hex(&ಟ್ಯಾಗ್->object.oid));
		ಸ್ಥಿತಿ DROP:
			/* Ignore this tag altogether */
			ಬಿಡುಗಡೆ(buf);
			ಹಿಂತಿರುಗಿ;
		ಸ್ಥಿತಿ REWRITE:
			ಆದರೆ (tagged->type == OBJ_ಟ್ಯಾಗ್ && !mark_ಟ್ಯಾಗುಗಳು) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((ರಚನೆ commit *)tagged);
				ಆದರೆ (!p) {
					ಬರೆ("reset %s\nfrom %s\n\n",
					       name, oid_ಗಮ್ಯ_hex(null_oid(the_hash_algo)));
					ಬಿಡುಗಡೆ(buf);
					ಹಿಂತಿರುಗಿ;
				}
				tagged_mark = get_object_mark(&p->object);
			} ಇಲ್ಲದಿದ್ದರೆ {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	ಆದರೆ (tagged->type == OBJ_ಟ್ಯಾಗ್) {
		ಬರೆ("reset %s\nfrom %s\n\n",
		       name, oid_ಗಮ್ಯ_hex(null_oid(the_hash_algo)));
	}
	skip_ಪೂರ್ವಪ್ರತ್ಯಯ(name, "refs/tags/", &name);
	ಬರೆ("tag %s\n", name);
	ಆದರೆ (mark_ಟ್ಯಾಗುಗಳು) {
		mark_ಮುಂದಿನ_object(&ಟ್ಯಾಗ್->object);
		ಬರೆ("mark :%"PRIu32"\n", ಕೊನೆಯ_idnum);
	}
	ಆದರೆ (tagged_mark)
		ಬರೆ("from :%d\n", tagged_mark);
	ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ("from %s\n", oid_ಗಮ್ಯ_hex(&tagged->oid));

	ಆದರೆ (show_original_ids)
		ಬರೆ("original-oid %s\n", oid_ಗಮ್ಯ_hex(&ಟ್ಯಾಗ್->object.oid));
	ಬರೆ("%.*s%sdata %d\n%.*s\n",
	       (ಪೂರ್ಣಾಂಕ)(tagger_ಕೊನೆ - tagger), tagger,
	       tagger == tagger_ಕೊನೆ ? "" : "\n",
	       (ಪೂರ್ಣಾಂಕ)ಸಂದೇಶ_ಗಾತ್ರ, (ಪೂರ್ಣಾಂಕ)ಸಂದೇಶ_ಗಾತ್ರ, ಸಂದೇಶ_2 ? ಸಂದೇಶ_2 : "");
	ಬಿಡುಗಡೆ(buf);
}

ಸ್ಥಿರ ರಚನೆ commit *get_commit(ರಚನೆ rev_cmdline_ನಮೂದು *e, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತುಂಬಿದ_name)
{
	ಆಯ್ಕೆ (e->ಅಂಶ->type) {
	ಸ್ಥಿತಿ OBJ_COMMIT:
		ಹಿಂತಿರುಗಿ (ರಚನೆ commit *)e->ಅಂಶ;
	ಸ್ಥಿತಿ OBJ_ಟ್ಯಾಗ್: {
		ರಚನೆ ಟ್ಯಾಗ್ *ಟ್ಯಾಗ್ = (ರಚನೆ ಟ್ಯಾಗ್ *)e->ಅಂಶ;

		/* handle nested tags */
		ಆಗ (ಟ್ಯಾಗ್ && ಟ್ಯಾಗ್->object.type == OBJ_ಟ್ಯಾಗ್) {
			parse_object(the_repository, &ಟ್ಯಾಗ್->object.oid);
			string_list_append(&ಟ್ಯಾಗ್_refs, ತುಂಬಿದ_name)->util = ಟ್ಯಾಗ್;
			ಟ್ಯಾಗ್ = (ರಚನೆ ಟ್ಯಾಗ್ *)ಟ್ಯಾಗ್->tagged;
		}
		ಆದರೆ (!ಟ್ಯಾಗ್)
			die(_("tag %s points nowhere?"), e->name);
		ಹಿಂತಿರುಗಿ (ರಚನೆ commit *)ಟ್ಯಾಗ್;
	}
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ get_ಟ್ಯಾಗುಗಳು_and_duplicates(ರಚನೆ rev_cmdline_info *info)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < info->nr; i++) {
		ರಚನೆ rev_cmdline_ನಮೂದು *e = info->rev + i;
		ರಚನೆ object_id oid;
		ರಚನೆ commit *commit;
		ಅಕ್ಷರ *ತುಂಬಿದ_name = ಖಾಲಿ;

		ಆದರೆ (e->flags & UNINTERESTING)
			ಮುಂದುವರಿ;

		ಆದರೆ (repo_dwim_ref(the_repository, e->name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(e->name),
				  &oid, &ತುಂಬಿದ_name, 0) != 1) {
			ಬಿಡುಗಡೆ(ತುಂಬಿದ_name);
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (refspecs.nr) {
			ಅಕ್ಷರ *ಖಾಸಗಿ;
			ಖಾಸಗಿ = apply_refspecs(&refspecs, ತುಂಬಿದ_name);
			ಆದರೆ (ಖಾಸಗಿ) {
				ಬಿಡುಗಡೆ(ತುಂಬಿದ_name);
				ತುಂಬಿದ_name = ಖಾಸಗಿ;
			}
		}

		commit = get_commit(e, ತುಂಬಿದ_name);
		ಆದರೆ (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->ಅಂಶ->type));
			ಬಿಡುಗಡೆ(ತುಂಬಿದ_name);
			ಮುಂದುವರಿ;
		}

		ಆಯ್ಕೆ (commit->object.type) {
		ಸ್ಥಿತಿ OBJ_COMMIT:
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ OBJ_BLOB:
			export_blob(&commit->object.oid);
			ಬಿಡುಗಡೆ(ತುಂಬಿದ_name);
			ಮುಂದುವರಿ;
		ಪೂರ್ವನಿಯೋಜಿತ: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			ಬಿಡುಗಡೆ(ತುಂಬಿದ_name);
			ಮುಂದುವರಿ;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		ಆದರೆ (e->ಅಂಶ->type != OBJ_ಟ್ಯಾಗ್)
			string_list_append(&extra_refs, ತುಂಬಿದ_name)->util = commit;

		ಆದರೆ (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = ತುಂಬಿದ_name;
		ಇಲ್ಲದಿದ್ದರೆ
			ಬಿಡುಗಡೆ(ತುಂಬಿದ_name);
	}

	string_list_ವಿಂಗಡಿಸು_u(&extra_refs, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_ಟ್ಯಾಗುಗಳು_and_duplicates(ರಚನೆ string_list *extras)
{
	ರಚನೆ commit *commit;
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = extras->nr - 1; i >= 0; i--) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = extras->ವಸ್ತುಗಳು[i].string;
		ರಚನೆ object *object = extras->ವಸ್ತುಗಳು[i].util;
		ಪೂರ್ಣಾಂಕ mark;

		ಆಯ್ಕೆ (object->type) {
		ಸ್ಥಿತಿ OBJ_ಟ್ಯಾಗ್:
			handle_ಟ್ಯಾಗ್(name, (ರಚನೆ ಟ್ಯಾಗ್ *)object);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ OBJ_COMMIT:
			ಆದರೆ (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((ರಚನೆ commit *)object);
			ಆದರೆ (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				ಬರೆ("reset %s\nfrom %s\n\n",
				       name, oid_ಗಮ್ಯ_hex(null_oid(the_hash_algo)));
				ಮುಂದುವರಿ;
			}

			mark = get_object_mark(&commit->object);
			ಆದರೆ (!mark) {
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
				ಆದರೆ (!reference_excluded_commits) {
					/* delete the ref */
					ಬರೆ("reset %s\nfrom %s\n\n",
					       name, oid_ಗಮ್ಯ_hex(null_oid(the_hash_algo)));
					ಮುಂದುವರಿ;
				}
				/* set ref to commit using oid, not mark */
				ಬರೆ("reset %s\nfrom %s\n\n", name,
				       oid_ಗಮ್ಯ_hex(&commit->object.oid));
				ಮುಂದುವರಿ;
			}

			ಬರೆ("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_ಪ್ರಗತಿ();
			ನಿಲ್ಲಿಸು;
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ export_marks(ಅಕ್ಷರ *file)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;
	uint32_t mark;
	ರಚನೆ decoration_ನಮೂದು *deco = idnums.entries;
	FILE *f;
	ಪೂರ್ಣಾಂಕ e = 0;

	f = fopen_for_writing(file);
	ಆದರೆ (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	ಗಾಗಿ (i = 0; i < idnums.ಗಾತ್ರ_2; i++) {
		ಆದರೆ (deco->ತಳ && deco->ತಳ->type == 1) {
			mark = ptr_ಗಮ್ಯ_mark(deco->decoration);
			ಆದರೆ (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_ಗಮ್ಯ_hex(&deco->ತಳ->oid)) < 0) {
			    e = 1;
			    ನಿಲ್ಲಿಸು;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	ಆದರೆ (e)
		error(_("unable to write marks file %s."), file);
}

ಸ್ಥಿರ ಶೂನ್ಯ import_marks(ಅಕ್ಷರ *ಒಳಹರಿವು_file, ಪೂರ್ಣಾಂಕ check_exists)
{
	ಅಕ್ಷರ ಸಾಲು[512];
	FILE *f;
	ರಚನೆ stat sb;

	ಆದರೆ (check_exists && stat(ಒಳಹರಿವು_file, &sb))
		ಹಿಂತಿರುಗಿ;

	f = xfopen(ಒಳಹರಿವು_file, "r");
	ಆಗ (fgets(ಸಾಲು, ಗಾತ್ರ(ಸಾಲು), f)) {
		uint32_t mark;
		ಅಕ್ಷರ *ಸಾಲು_ಕೊನೆ, *mark_ಕೊನೆ;
		ರಚನೆ object_id oid;
		ರಚನೆ object *object;
		ರಚನೆ commit *commit;
		ಎಣಿಕೆ object_type type;

		ಸಾಲು_ಕೊನೆ = strchr(ಸಾಲು, '\n');
		ಆದರೆ (ಸಾಲು[0] != ':' || !ಸಾಲು_ಕೊನೆ)
			die(_("corrupt mark line: %s"), ಸಾಲು);
		*ಸಾಲು_ಕೊನೆ = '\0';

		mark = strtoumax(ಸಾಲು + 1, &mark_ಕೊನೆ, 10);
		ಆದರೆ (!mark || mark_ಕೊನೆ == ಸಾಲು + 1
			|| *mark_ಕೊನೆ != ' ' || get_oid_hex(mark_ಕೊನೆ + 1, &oid))
			die(_("corrupt mark line: %s"), ಸಾಲು);

		ಆದರೆ (ಕೊನೆಯ_idnum < mark)
			ಕೊನೆಯ_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, ಖಾಲಿ);
		ಆದರೆ (type < 0)
			die(_("object not found: %s"), oid_ಗಮ್ಯ_hex(&oid));

		ಆದರೆ (type != OBJ_COMMIT)
			/* only commits */
			ಮುಂದುವರಿ;

		commit = lookup_commit(the_repository, &oid);
		ಆದರೆ (!commit)
			die(_("not a commit? can't happen: %s"), oid_ಗಮ್ಯ_hex(&oid));

		object = &commit->object;

		ಆದರೆ (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_ಗಮ್ಯ_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_deletes(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i;
	ಗಾಗಿ (i = 0; i < refspecs.nr; i++) {
		ರಚನೆ refspec_ಅಂಶ *refspec = &refspecs.ವಸ್ತುಗಳು[i];
		ಆದರೆ (*refspec->src)
			ಮುಂದುವರಿ;

		ಬರೆ("reset %s\nfrom %s\n\n",
				refspec->dst, oid_ಗಮ್ಯ_hex(null_oid(the_hash_algo)));
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_anonymize_map(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ hashmap *map = opt->ಮೌಲ್ಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *delim, *ಮೌಲ್ಯ;
	ಗಾತ್ರ_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	ಆದರೆ (delim) {
		keylen = delim - arg;
		ಮೌಲ್ಯ = delim + 1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		keylen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(arg);
		ಮೌಲ್ಯ = arg;
	}

	ಆದರೆ (!keylen || !*ಮೌಲ್ಯ)
		ಹಿಂತಿರುಗಿ error(_("--anonymize-map token cannot be empty"));

	ಕೂಡಿಸು_anonymized_ನಮೂದು(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(ಮೌಲ್ಯ));

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_fast_export(ಪೂರ್ಣಾಂಕ argc,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		    ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ rev_info revs;
	ರಚನೆ commit *commit;
	ಅಕ್ಷರ *export_filename = ಖಾಲಿ,
	     *import_filename = ಖಾಲಿ,
	     *import_filename_if_exists = ಖಾಲಿ;
	uint32_t lastimportid;
	ರಚನೆ string_list refspecs_list = STRING_LIST_INIT_NODUP;
	ರಚನೆ string_list paths_ದ_ಬದಲಾಯಿತು_objects = STRING_LIST_INIT_DUP;
	ರಚನೆ option options[] = {
		OPT_INTEGER(0, "progress", &ಪ್ರಗತಿ,
			    N_("show progress after <n> objects")),
		OPT_CALLBACK(0, "signed-tags", &signed_ಟ್ಯಾಗ್_ಬಹುಲಕ, N_("mode"),
			     N_("select handling of signed tags"),
			     parse_opt_sign_ಬಹುಲಕ),
		OPT_CALLBACK(0, "signed-commits", &signed_commit_ಬಹುಲಕ, N_("mode"),
			     N_("select handling of signed commits"),
			     parse_opt_sign_ಬಹುಲಕ),
		OPT_CALLBACK(0, "tag-of-filtered-object", &ಟ್ಯಾಗ್_ದ_filtered_ಬಹುಲಕ, N_("mode"),
			     N_("select handling of tags that tag filtered objects"),
			     parse_opt_ಟ್ಯಾಗ್_ದ_filtered_ಬಹುಲಕ),
		OPT_CALLBACK(0, "reencode", &reencode_ಬಹುಲಕ, N_("mode"),
			     N_("select handling of commit messages in an alternate encoding"),
			     parse_opt_reencode_ಬಹುಲಕ),
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
		OPT_BOOL(0, "full-tree", &ತುಂಬಿದ_ವೃಕ್ಷ,
			 N_("output full tree for each commit")),
		OPT_BOOL(0, "use-done-feature", &use_ಮುಗಿದ_feature,
			     N_("use the done feature to terminate the stream")),
		OPT_BOOL(0, "no-data", &no_ದತ್ತಾಂಶ, N_("skip output of blob data")),
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
		OPT_BOOL(0, "mark-tags", &mark_ಟ್ಯಾಗುಗಳು,
			    N_("label tags with mark ids")),

		OPT_ಕೊನೆ()
	};

	ಆದರೆ (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, ಪೂರ್ವಪ್ರತ್ಯಯ);
	init_revision_sources(&revision_sources);
	revs.topo_ಕ್ರಮ = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = ಸಿದ್ಧತೆ_revisions(argc, argv, &revs, ಖಾಲಿ);
	ಆದರೆ (argc > 1)
		usage_with_options (fast_export_usage, options);

	ಆದರೆ (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	ಆದರೆ (refspecs_list.nr) {
		ಪೂರ್ಣಾಂಕ i;

		ಗಾಗಿ (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.ವಸ್ತುಗಳು[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	ಆದರೆ (use_ಮುಗಿದ_feature)
		ಬರೆ("feature done\n");

	ಆದರೆ (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	ಆದರೆ (import_filename)
		import_marks(import_filename, 0);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = ಕೊನೆಯ_idnum;

	ಆದರೆ (import_filename && revs.prune_ದತ್ತಾಂಶ.nr)
		ತುಂಬಿದ_ವೃಕ್ಷ = 1;

	get_ಟ್ಯಾಗುಗಳು_and_duplicates(&revs.cmdline);

	ಆದರೆ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.ಹಿಮ್ಮುಖ = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_ದತ್ತಾಂಶ = &paths_ದ_ಬದಲಾಯಿತು_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	ಆಗ ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_ದ_ಬದಲಾಯಿತು_objects);
	revs.diffopt.no_free = 0;

	handle_ಟ್ಯಾಗುಗಳು_and_duplicates(&extra_refs);
	handle_ಟ್ಯಾಗುಗಳು_and_duplicates(&ಟ್ಯಾಗ್_refs);
	handle_deletes();

	ಆದರೆ (export_filename && lastimportid != ಕೊನೆಯ_idnum)
		export_marks(export_filename);

	ಆದರೆ (use_ಮುಗಿದ_feature)
		ಬರೆ("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	ಹಿಂತಿರುಗಿ 0;
}
