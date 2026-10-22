/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diffcore.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "log-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "decorate.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blob.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gpg-interface.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ ਤਰੱਕੀ;
ਸਥਿਰ ਗਿਣਤੀ sign_ਬਹੁਲਕ signed_ਟੈਗ_ਬਹੁਲਕ = SIGN_ABORT;
ਸਥਿਰ ਗਿਣਤੀ sign_ਬਹੁਲਕ signed_commit_ਬਹੁਲਕ = SIGN_STRIP;
ਸਥਿਰ ਗਿਣਤੀ ਟੈਗ_ਦਾ_filtered_ਬਹੁਲਕ { ਟੈਗ_FILTERING_ABORT, DROP, REWRITE } ਟੈਗ_ਦਾ_filtered_ਬਹੁਲਕ = ਟੈਗ_FILTERING_ABORT;
ਸਥਿਰ ਗਿਣਤੀ reencode_ਬਹੁਲਕ { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_ਬਹੁਲਕ = REENCODE_ABORT;
ਸਥਿਰ ਪੂਰਨ fake_missing_tagger;
ਸਥਿਰ ਪੂਰਨ use_ਪੂਰਾ_feature;
ਸਥਿਰ ਪੂਰਨ no_ਅੰਕੜੇ;
ਸਥਿਰ ਪੂਰਨ ਭਰਿਆ_ਰੁੱਖ;
ਸਥਿਰ ਪੂਰਨ reference_excluded_commits;
ਸਥਿਰ ਪੂਰਨ show_original_ids;
ਸਥਿਰ ਪੂਰਨ mark_ਟੈਗ_ਸੂਚੀ;
ਸਥਿਰ ਬਣਤਰ string_list extra_refs = STRING_LIST_INIT_DUP;
ਸਥਿਰ ਬਣਤਰ string_list ਟੈਗ_refs = STRING_LIST_INIT_DUP;
ਸਥਿਰ ਬਣਤਰ refspec refspecs;
ਸਥਿਰ ਪੂਰਨ anonymize;
ਸਥਿਰ ਬਣਤਰ hashmap anonymized_seeds;
ਸਥਿਰ ਬਣਤਰ revision_sources revision_sources;

ਸਥਿਰ ਪੂਰਨ parse_opt_sign_ਬਹੁਲਕ(ਅਚਲ ਬਣਤਰ option *opt,
			       ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਗਿਣਤੀ sign_ਬਹੁਲਕ *val = opt->ਮੁੱਲ;

	ਜੇ (unset)
		ਵਾਪਸ 0;

	ਜੇ (parse_sign_ਬਹੁਲਕ(arg, val, ਨਿਰਾ) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		ਵਾਪਸ error(_("unknown %s mode: %s"), opt->long_name, arg);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_opt_ਟੈਗ_ਦਾ_filtered_ਬਹੁਲਕ(ਅਚਲ ਬਣਤਰ option *opt,
					  ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਗਿਣਤੀ ਟੈਗ_ਦਾ_filtered_ਬਹੁਲਕ *val = opt->ਮੁੱਲ;

	ਜੇ (unset || !strcmp(arg, "abort"))
		*val = ਟੈਗ_FILTERING_ABORT;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "drop"))
		*val = DROP;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("unknown tag-of-filtered mode: %s"), arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_opt_reencode_ਬਹੁਲਕ(ਅਚਲ ਬਣਤਰ option *opt,
				   ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਗਿਣਤੀ reencode_ਬਹੁਲਕ *val = opt->ਮੁੱਲ;

	ਜੇ (unset) {
		*val = REENCODE_ABORT;
		ਵਾਪਸ 0;
	}

	ਚੋਣ (git_parse_maybe_bool(arg)) {
	ਹਾਲਤ 0:
		*val = REENCODE_NO;
		ਰੋਕੋ;
	ਹਾਲਤ 1:
		*val = REENCODE_YES;
		ਰੋਕੋ;
	ਮੂਲ:
		ਜੇ (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		ਨਹੀਂਤਾਂ
			ਵਾਪਸ error(_("unknown reencoding mode: %s"), arg);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ decoration idnums;
ਸਥਿਰ uint32_t ਆਖਰੀ_idnum;
ਬਣਤਰ anonymized_ਇੰਦਰਾਜ {
	ਬਣਤਰ hashmap_ਇੰਦਰਾਜ hash;
	ਅੱਖਰ *anon;
	ਅਚਲ ਅੱਖਰ orig[FLEX_ARRAY];
};

ਬਣਤਰ anonymized_ਇੰਦਰਾਜ_ਕੁੰਜੀ {
	ਬਣਤਰ hashmap_ਇੰਦਰਾਜ hash;
	ਅਚਲ ਅੱਖਰ *orig;
	ਮਾਪ_t orig_len;
};

ਸਥਿਰ ਪੂਰਨ anonymized_ਇੰਦਰਾਜ_cmp(ਅਚਲ ਖਾਲੀ *cmp_ਅੰਕੜੇ ਅਣਵਰਤਿਆ,
				ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *eptr,
				ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ_or_ਕੁੰਜੀ,
				ਅਚਲ ਖਾਲੀ *keydata)
{
	ਅਚਲ ਬਣਤਰ anonymized_ਇੰਦਰਾਜ *a, *b;

	a = container_ਦਾ(eptr, ਅਚਲ ਬਣਤਰ anonymized_ਇੰਦਰਾਜ, hash);
	ਜੇ (keydata) {
		ਅਚਲ ਬਣਤਰ anonymized_ਇੰਦਰਾਜ_ਕੁੰਜੀ *ਕੁੰਜੀ = keydata;
		ਪੂਰਨ equal = !xstrncmpz(a->orig, ਕੁੰਜੀ->orig, ਕੁੰਜੀ->orig_len);
		ਵਾਪਸ !equal;
	}

	b = container_ਦਾ(ਇੰਦਰਾਜ_or_ਕੁੰਜੀ, ਅਚਲ ਬਣਤਰ anonymized_ਇੰਦਰਾਜ, hash);
	ਵਾਪਸ strcmp(a->orig, b->orig);
}

ਸਥਿਰ ਬਣਤਰ anonymized_ਇੰਦਰਾਜ *ਜੋੜੋ_anonymized_ਇੰਦਰਾਜ(ਬਣਤਰ hashmap *map,
						     ਬਿਨਾਨਿਸ਼ਾਨੀ hash,
						     ਅਚਲ ਅੱਖਰ *orig, ਮਾਪ_t len,
						     ਅੱਖਰ *anon)
{
	ਬਣਤਰ anonymized_ਇੰਦਰਾਜ *ret, *old;

	ਜੇ (!map->cmpfn)
		hashmap_init(map, anonymized_ਇੰਦਰਾਜ_cmp, ਨਿਰਾ, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_ਇੰਦਰਾਜ_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_ਇੰਦਰਾਜ(map, ret, hash);

	ਜੇ (old) {
		ਮੁਕਤ(old->anon);
		ਮੁਕਤ(old);
	}

	ਵਾਪਸ ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *anonymize_str(ਬਣਤਰ hashmap *map,
				 ਅੱਖਰ *(*generate)(ਖਾਲੀ),
				 ਅਚਲ ਅੱਖਰ *orig, ਮਾਪ_t len)
{
	ਬਣਤਰ anonymized_ਇੰਦਰਾਜ_ਕੁੰਜੀ ਕੁੰਜੀ;
	ਬਣਤਰ anonymized_ਇੰਦਰਾਜ *ret;

	hashmap_ਇੰਦਰਾਜ_init(&ਕੁੰਜੀ.hash, memhash(orig, len));
	ਕੁੰਜੀ.orig = orig;
	ਕੁੰਜੀ.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_ਇੰਦਰਾਜ(&anonymized_seeds, &ਕੁੰਜੀ, hash, &ਕੁੰਜੀ);

	/* ...otherwise check if we've already seen it in this context... */
	ਜੇ (!ret)
		ret = hashmap_get_ਇੰਦਰਾਜ(map, &ਕੁੰਜੀ, hash, &ਕੁੰਜੀ);

	/* ...and finally generate a new mapping if necessary */
	ਜੇ (!ret)
		ret = ਜੋੜੋ_anonymized_ਇੰਦਰਾਜ(map, ਕੁੰਜੀ.hash.hash,
					   orig, len, generate());

	ਵਾਪਸ ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
ਸਥਿਰ ਖਾਲੀ anonymize_ਰਸਤਾ(ਬਣਤਰ strbuf *ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
			   ਬਣਤਰ hashmap *map,
			   ਅੱਖਰ *(*generate)(ਖਾਲੀ))
{
	ਜਦੋਂ (*ਰਸਤਾ) {
		ਅਚਲ ਅੱਖਰ *ਸਿਰਾ_ਦਾ_component = strchrnul(ਰਸਤਾ, '/');
		ਮਾਪ_t len = ਸਿਰਾ_ਦਾ_component - ਰਸਤਾ;
		ਅਚਲ ਅੱਖਰ *c = anonymize_str(map, generate, ਰਸਤਾ, len);
		strbuf_addstr(ਬਾਹਰਲਾ, c);
		ਰਸਤਾ = ਸਿਰਾ_ਦਾ_component;
		ਜੇ (*ਰਸਤਾ)
			strbuf_addch(ਬਾਹਰਲਾ, *ਰਸਤਾ++);
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ *mark_ਮੰਜ਼ਿਲ_ptr(uint32_t mark)
{
	ਵਾਪਸ (ਖਾਲੀ *)(uintptr_t)mark;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t ptr_ਮੰਜ਼ਿਲ_mark(ਖਾਲੀ * mark)
{
	ਵਾਪਸ (uint32_t)(uintptr_t)mark;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ mark_object(ਬਣਤਰ object *object, uint32_t mark)
{
	ਜੋੜੋ_decoration(&idnums, object, mark_ਮੰਜ਼ਿਲ_ptr(mark));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ mark_ਅਗਲਾ_object(ਬਣਤਰ object *object)
{
	mark_object(object, ++ਆਖਰੀ_idnum);
}

ਸਥਿਰ ਪੂਰਨ get_object_mark(ਬਣਤਰ object *object)
{
	ਖਾਲੀ *decoration = lookup_decoration(&idnums, object);
	ਜੇ (!decoration)
		ਵਾਪਸ 0;
	ਵਾਪਸ ptr_ਮੰਜ਼ਿਲ_mark(decoration);
}

ਸਥਿਰ ਬਣਤਰ commit *rewrite_commit(ਬਣਤਰ commit *p)
{
	ਲਈ (;;) {
		ਜੇ (p->parents && p->parents->ਅਗਲਾ)
			ਰੋਕੋ;
		ਜੇ (p->object.flags & UNINTERESTING)
			ਰੋਕੋ;
		ਜੇ (!(p->object.flags & TREESAME))
			ਰੋਕੋ;
		ਜੇ (!p->parents)
			ਵਾਪਸ ਨਿਰਾ;
		p = p->parents->ਵਸਤੂ;
	}
	ਵਾਪਸ p;
}

ਸਥਿਰ ਖਾਲੀ show_ਤਰੱਕੀ(ਖਾਲੀ)
{
	ਸਥਿਰ ਪੂਰਨ counter = 0;
	ਜੇ (!ਤਰੱਕੀ)
		ਵਾਪਸ;
	ਜੇ ((++counter % ਤਰੱਕੀ) == 0)
		ਲਿਖੋ("progress %d objects\n", counter);
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
ਸਥਿਰ ਅੱਖਰ *anonymize_blob(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *ਮਾਪ)
{
	ਸਥਿਰ ਪੂਰਨ counter;
	ਬਣਤਰ strbuf ਬਾਹਰਲਾ = STRBUF_INIT;
	strbuf_addf(&ਬਾਹਰਲਾ, "anonymous blob %d", counter++);
	*ਮਾਪ = ਬਾਹਰਲਾ.len;
	ਵਾਪਸ strbuf_detach(&ਬਾਹਰਲਾ, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ export_blob(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;
	ਗਿਣਤੀ object_type type;
	ਅੱਖਰ *buf;
	ਬਣਤਰ object *object;
	ਪੂਰਨ eaten;

	ਜੇ (no_ਅੰਕੜੇ)
		ਵਾਪਸ;

	ਜੇ (is_null_oid(oid))
		ਵਾਪਸ;

	object = lookup_object(the_repository, oid);
	ਜੇ (object && object->flags & SHOWN)
		ਵਾਪਸ;

	ਜੇ (anonymize) {
		buf = anonymize_blob(&ਮਾਪ);
		object = (ਬਣਤਰ object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} ਨਹੀਂਤਾਂ {
		ਮਾਪ_t ਮਾਪ_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &ਮਾਪ_st);
		ਮਾਪ = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(ਮਾਪ_st);
		ਜੇ (!buf)
			die(_("could not read blob %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		ਜੇ (check_object_signature(the_repository, oid, buf, ਮਾਪ,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		object = parse_object_ਬਫਰ(the_repository, oid, type,
					     ਮਾਪ, buf, &eaten);
	}

	ਜੇ (!object)
		die(_("could not read blob %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));

	mark_ਅਗਲਾ_object(object);

	ਲਿਖੋ("blob\nmark :%"PRIu32"\n", ਆਖਰੀ_idnum);
	ਜੇ (show_original_ids)
		ਲਿਖੋ("original-oid %s\n", oid_ਮੰਜ਼ਿਲ_hex(oid));
	ਲਿਖੋ("data %"PRIuMAX"\n", (uintmax_t)ਮਾਪ);
	ਜੇ (ਮਾਪ && fwrite(buf, ਮਾਪ, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_ਮੰਜ਼ਿਲ_hex(oid));
	ਲਿਖੋ("\n");

	show_ਤਰੱਕੀ();

	object->flags |= SHOWN;
	ਜੇ (!eaten)
		ਮੁਕਤ(buf);
}

ਸਥਿਰ ਪੂਰਨ ਡੂੰਘਾਈ_ਪਹਿਲਾ(ਅਚਲ ਖਾਲੀ *a_, ਅਚਲ ਖਾਲੀ *b_)
{
	ਅਚਲ ਬਣਤਰ diff_filepair *a = *((ਅਚਲ ਬਣਤਰ diff_filepair **)a_);
	ਅਚਲ ਬਣਤਰ diff_filepair *b = *((ਅਚਲ ਬਣਤਰ diff_filepair **)b_);
	ਅਚਲ ਅੱਖਰ *name_a, *name_b;
	ਪੂਰਨ len_a, len_b, len;
	ਪੂਰਨ cmp;

	name_a = a->one ? a->one->ਰਸਤਾ : a->two->ਰਸਤਾ;
	name_b = b->one ? b->one->ਰਸਤਾ : b->two->ਰਸਤਾ;

	len_a = ਲੜੀ_ਲੰਬਾਈ(name_a);
	len_b = ਲੜੀ_ਲੰਬਾਈ(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	ਜੇ (cmp)
		ਵਾਪਸ cmp;
	cmp = len_b - len_a;
	ਜੇ (cmp)
		ਵਾਪਸ cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	ਵਾਪਸ (a->ਸਥਿਤੀ == 'R') - (b->ਸਥਿਤੀ == 'R');
}

ਸਥਿਰ ਖਾਲੀ print_ਰਸਤਾ_1(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਪੂਰਨ need_quote = quote_c_style(ਰਸਤਾ, ਨਿਰਾ, ਨਿਰਾ, 0);
	ਜੇ (need_quote)
		quote_c_style(ਰਸਤਾ, ਨਿਰਾ, stdout, 0);
	ਨਹੀਂਤਾਂ ਜੇ (strchr(ਰਸਤਾ, ' '))
		ਲਿਖੋ("\"%s\"", ਰਸਤਾ);
	ਨਹੀਂਤਾਂ
		ਲਿਖੋ("%s", ਰਸਤਾ);
}

ਸਥਿਰ ਅੱਖਰ *anonymize_ਰਸਤਾ_component(ਖਾਲੀ)
{
	ਸਥਿਰ ਪੂਰਨ counter;
	ਬਣਤਰ strbuf ਬਾਹਰਲਾ = STRBUF_INIT;
	strbuf_addf(&ਬਾਹਰਲਾ, "path%d", counter++);
	ਵਾਪਸ strbuf_detach(&ਬਾਹਰਲਾ, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ print_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਜੇ (!anonymize)
		print_ਰਸਤਾ_1(ਰਸਤਾ);
	ਨਹੀਂਤਾਂ {
		ਸਥਿਰ ਬਣਤਰ hashmap paths;
		ਸਥਿਰ ਬਣਤਰ strbuf anon = STRBUF_INIT;

		anonymize_ਰਸਤਾ(&anon, ਰਸਤਾ, &paths, anonymize_ਰਸਤਾ_component);
		print_ਰਸਤਾ_1(anon.buf);
		strbuf_reset(&anon);
	}
}

ਸਥਿਰ ਅੱਖਰ *generate_fake_oid(ਖਾਲੀ)
{
	ਸਥਿਰ uint32_t counter = 1; /* avoid null oid */
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ hashsz = the_hash_algo->rawsz;
	ਬਣਤਰ object_id oid;
	ਅੱਖਰ *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	ਵਾਪਸ oid_ਮੰਜ਼ਿਲ_hex_r(hex, &oid);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *anonymize_oid(ਅਚਲ ਅੱਖਰ *oid_hex)
{
	ਸਥਿਰ ਬਣਤਰ hashmap objs;
	ਮਾਪ_t len = ਲੜੀ_ਲੰਬਾਈ(oid_hex);
	ਵਾਪਸ anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

ਸਥਿਰ ਖਾਲੀ show_filemodify(ਬਣਤਰ diff_ਕਤਾਰ_struct *q,
			    ਬਣਤਰ diff_options *options ਅਣਵਰਤਿਆ, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਪੂਰਨ i;
	ਬਣਤਰ string_list *ਬਦਲਿਆ = ਅੰਕੜੇ;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->ਕਤਾਰ, q->nr, ਡੂੰਘਾਈ_ਪਹਿਲਾ);

	ਲਈ (i = 0; i < q->nr; i++) {
		ਬਣਤਰ diff_filespec *ospec = q->ਕਤਾਰ[i]->one;
		ਬਣਤਰ diff_filespec *spec = q->ਕਤਾਰ[i]->two;

		ਚੋਣ (q->ਕਤਾਰ[i]->ਸਥਿਤੀ) {
		ਹਾਲਤ DIFF_ਸਥਿਤੀ_DELETED:
			ਲਿਖੋ("D ");
			print_ਰਸਤਾ(spec->ਰਸਤਾ);
			string_list_ਪਾਓ(ਬਦਲਿਆ, spec->ਰਸਤਾ);
			ਅੱਖਰ_ਲਿਖੋ('\n');
			ਰੋਕੋ;

		ਹਾਲਤ DIFF_ਸਥਿਤੀ_COPIED:
		ਹਾਲਤ DIFF_ਸਥਿਤੀ_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			ਜੇ (!string_list_has_string(ਬਦਲਿਆ, ospec->ਰਸਤਾ)) {
				ਲਿਖੋ("%c ", q->ਕਤਾਰ[i]->ਸਥਿਤੀ);
				print_ਰਸਤਾ(ospec->ਰਸਤਾ);
				ਅੱਖਰ_ਲਿਖੋ(' ');
				print_ਰਸਤਾ(spec->ਰਸਤਾ);
				string_list_ਪਾਓ(ਬਦਲਿਆ, spec->ਰਸਤਾ);
				ਅੱਖਰ_ਲਿਖੋ('\n');

				ਜੇ (oideq(&ospec->oid, &spec->oid) &&
				    ospec->ਬਹੁਲਕ == spec->ਬਹੁਲਕ)
					ਰੋਕੋ;
			}
			/* fallthrough */

		ਹਾਲਤ DIFF_ਸਥਿਤੀ_TYPE_ਬਦਲਿਆ:
		ਹਾਲਤ DIFF_ਸਥਿਤੀ_MODIFIED:
		ਹਾਲਤ DIFF_ਸਥਿਤੀ_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			ਜੇ (no_ਅੰਕੜੇ || S_ISGITLINK(spec->ਬਹੁਲਕ))
				ਲਿਖੋ("M %06o %s ", spec->ਬਹੁਲਕ,
				       anonymize ?
				       anonymize_oid(oid_ਮੰਜ਼ਿਲ_hex(&spec->oid)) :
				       oid_ਮੰਜ਼ਿਲ_hex(&spec->oid));
			ਨਹੀਂਤਾਂ {
				ਬਣਤਰ object *object = lookup_object(the_repository,
								      &spec->oid);
				ਲਿਖੋ("M %06o :%d ", spec->ਬਹੁਲਕ,
				       get_object_mark(object));
			}
			print_ਰਸਤਾ(spec->ਰਸਤਾ);
			string_list_ਪਾਓ(ਬਦਲਿਆ, spec->ਰਸਤਾ);
			ਅੱਖਰ_ਲਿਖੋ('\n');
			ਰੋਕੋ;

		ਮੂਲ:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->ਕਤਾਰ[i]->ਸਥਿਤੀ,
			    ospec->ਰਸਤਾ ? ospec->ਰਸਤਾ : _("none"),
			    spec->ਰਸਤਾ ? spec->ਰਸਤਾ : _("none"));
		}
	}
}

ਸਥਿਰ ਅੱਖਰ *anonymize_ref_component(ਖਾਲੀ)
{
	ਸਥਿਰ ਪੂਰਨ counter;
	ਬਣਤਰ strbuf ਬਾਹਰਲਾ = STRBUF_INIT;
	strbuf_addf(&ਬਾਹਰਲਾ, "ref%d", counter++);
	ਵਾਪਸ strbuf_detach(&ਬਾਹਰਲਾ, ਨਿਰਾ);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *anonymize_refname(ਅਚਲ ਅੱਖਰ *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	ਸਥਿਰ ਬਣਤਰ hashmap refs;
	ਸਥਿਰ ਬਣਤਰ strbuf anon = STRBUF_INIT;
	ਪੂਰਨ i;

	strbuf_reset(&anon);
	ਲਈ (i = 0; i < ARRAY_ਮਾਪ(prefixes); i++) {
		ਜੇ (skip_ਅਗੇਤਰ(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			ਰੋਕੋ;
		}
	}

	anonymize_ਰਸਤਾ(&anon, refname, &refs, anonymize_ref_component);
	ਵਾਪਸ anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
ਸਥਿਰ ਅੱਖਰ *anonymize_commit_ਸੁਨੇਹਾ(ਖਾਲੀ)
{
	ਸਥਿਰ ਪੂਰਨ counter;
	ਵਾਪਸ xstrfmt("subject %d\n\nbody\n", counter++);
}

ਸਥਿਰ ਅੱਖਰ *anonymize_ident(ਖਾਲੀ)
{
	ਸਥਿਰ ਪੂਰਨ counter;
	ਬਣਤਰ strbuf ਬਾਹਰਲਾ = STRBUF_INIT;
	strbuf_addf(&ਬਾਹਰਲਾ, "User %d <user%d@example.com>", counter, counter);
	counter++;
	ਵਾਪਸ strbuf_detach(&ਬਾਹਰਲਾ, ਨਿਰਾ);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
ਸਥਿਰ ਖਾਲੀ anonymize_ident_ਸਤਰ(ਅਚਲ ਅੱਖਰ **beg, ਅਚਲ ਅੱਖਰ **ਸਿਰਾ)
{
	ਸਥਿਰ ਬਣਤਰ hashmap idents;
	ਸਥਿਰ ਬਣਤਰ strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ which_ਬਫਰ;

	ਬਣਤਰ strbuf *ਬਾਹਰਲਾ;
	ਬਣਤਰ ident_ਵੰਡੋ ਵੰਡੋ;
	ਅਚਲ ਅੱਖਰ *ਸਿਰਾ_ਦਾ_ਸਿਰਲੇਖ;

	ਬਾਹਰਲਾ = &buffers[which_ਬਫਰ++];
	which_ਬਫਰ %= ARRAY_ਮਾਪ(buffers);
	strbuf_reset(ਬਾਹਰਲਾ);

	/* skip "committer", "author", "tagger", etc */
	ਸਿਰਾ_ਦਾ_ਸਿਰਲੇਖ = strchr(*beg, ' ');
	ਜੇ (!ਸਿਰਾ_ਦਾ_ਸਿਰਲੇਖ)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (ਪੂਰਨ)(*ਸਿਰਾ - *beg), *beg);
	ਸਿਰਾ_ਦਾ_ਸਿਰਲੇਖ++;
	strbuf_ਜੋੜੋ(ਬਾਹਰਲਾ, *beg, ਸਿਰਾ_ਦਾ_ਸਿਰਲੇਖ - *beg);

	ਜੇ (!ਵੰਡੋ_ident_ਸਤਰ(&ਵੰਡੋ, ਸਿਰਾ_ਦਾ_ਸਿਰਲੇਖ, *ਸਿਰਾ - ਸਿਰਾ_ਦਾ_ਸਿਰਲੇਖ) &&
	    ਵੰਡੋ.date_begin) {
		ਅਚਲ ਅੱਖਰ *ident;
		ਮਾਪ_t len;

		len = ਵੰਡੋ.mail_ਸਿਰਾ - ਵੰਡੋ.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      ਵੰਡੋ.name_begin, len);
		strbuf_addstr(ਬਾਹਰਲਾ, ident);
		strbuf_addch(ਬਾਹਰਲਾ, ' ');
		strbuf_ਜੋੜੋ(ਬਾਹਰਲਾ, ਵੰਡੋ.date_begin, ਵੰਡੋ.tz_ਸਿਰਾ - ਵੰਡੋ.date_begin);
	} ਨਹੀਂਤਾਂ {
		strbuf_addstr(ਬਾਹਰਲਾ, "Malformed Ident <malformed@example.com> 0 -0000");
	}

	*beg = ਬਾਹਰਲਾ->buf;
	*ਸਿਰਾ = ਬਾਹਰਲਾ->buf + ਬਾਹਰਲਾ->len;
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
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਲੱਭੋ_commit_multiline_ਸਿਰਲੇਖ(ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ,
						ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ,
						ਅਚਲ ਅੱਖਰ **ਸਿਰਾ)
{
	ਬਣਤਰ strbuf val = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *bol, *eol;
	ਮਾਪ_t len;

	bol = ਲੱਭੋ_commit_ਸਿਰਲੇਖ(ਸੁਨੇਹਾ, ਕੁੰਜੀ, &len);
	ਜੇ (!bol)
		ਵਾਪਸ ਨਿਰਾ;
	eol = bol + len;
	strbuf_ਜੋੜੋ(&val, bol, len);

	ਜਦੋਂ (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_ਜੋੜੋ(&val, bol, eol - bol);
	}

	*ਸਿਰਾ = eol;
	ਵਾਪਸ strbuf_detach(&val, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ print_signature(ਅਚਲ ਅੱਖਰ *signature, ਅਚਲ ਅੱਖਰ *object_hash)
{
	ਜੇ (!signature)
		ਵਾਪਸ;

	ਲਿਖੋ("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਲੜੀ_ਲੰਬਾਈ(signature),
	       signature);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *append_signatures_for_ਸਿਰਲੇਖ(ਬਣਤਰ string_list *signatures,
						ਅਚਲ ਅੱਖਰ *pos,
						ਅਚਲ ਅੱਖਰ *ਸਿਰਲੇਖ,
						ਅਚਲ ਅੱਖਰ *object_hash)
{
	ਅਚਲ ਅੱਖਰ *signature;
	ਅਚਲ ਅੱਖਰ *ਸ਼ੁਰੂ = pos;
	ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = pos;

	ਜਦੋਂ ((signature = ਲੱਭੋ_commit_multiline_ਸਿਰਲੇਖ(ਸ਼ੁਰੂ + 1,
							 ਸਿਰਲੇਖ,
							 &ਸਿਰਾ))) {
		string_list_append(signatures, signature)->util = (ਖਾਲੀ *)object_hash;
		ਮੁਕਤ((ਅੱਖਰ *)signature);
		ਸ਼ੁਰੂ = ਸਿਰਾ;
	}

	ਵਾਪਸ ਸਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ handle_commit(ਬਣਤਰ commit *commit, ਬਣਤਰ rev_info *rev,
			  ਬਣਤਰ string_list *paths_ਦਾ_ਬਦਲਿਆ_objects)
{
	ਪੂਰਨ saved_output_format = rev->diffopt.output_format;
	ਅਚਲ ਅੱਖਰ *commit_ਬਫਰ, *commit_ਬਫਰ_cursor;
	ਅਚਲ ਅੱਖਰ *author, *author_ਸਿਰਾ, *committer, *committer_ਸਿਰਾ;
	ਅਚਲ ਅੱਖਰ *encoding = ਨਿਰਾ;
	ਮਾਪ_t encoding_len;
	ਬਣਤਰ string_list signatures = STRING_LIST_INIT_DUP;
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2;
	ਅੱਖਰ *reencoded = ਨਿਰਾ;
	ਬਣਤਰ commit_list *p;
	ਅਚਲ ਅੱਖਰ *refname;
	ਪੂਰਨ i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_ਬਫਰ_cursor = commit_ਬਫਰ = repo_get_commit_ਬਫਰ(the_repository, commit, ਨਿਰਾ);

	author = ਲੜੀ_ਖੋਜ(commit_ਬਫਰ_cursor, "\nauthor ");
	ਜੇ (!author)
		die(_("could not find author in commit %s"),
		    oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
	author++;
	commit_ਬਫਰ_cursor = author_ਸਿਰਾ = strchrnul(author, '\n');

	committer = ਲੜੀ_ਖੋਜ(commit_ਬਫਰ_cursor, "\ncommitter ");
	ਜੇ (!committer)
		die(_("could not find committer in commit %s"),
		    oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
	committer++;
	commit_ਬਫਰ_cursor = committer_ਸਿਰਾ = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	ਜੇ (*commit_ਬਫਰ_cursor == '\n') {
		encoding = ਲੱਭੋ_commit_ਸਿਰਲੇਖ(commit_ਬਫਰ_cursor + 1, "encoding", &encoding_len);
		ਜੇ (encoding)
			commit_ਬਫਰ_cursor = encoding + encoding_len;
	}

	ਜੇ (*commit_ਬਫਰ_cursor == '\n') {
		ਅਚਲ ਅੱਖਰ *after_sha1 = append_signatures_for_ਸਿਰਲੇਖ(&signatures, commit_ਬਫਰ_cursor,
								      "gpgsig", "sha1");
		ਅਚਲ ਅੱਖਰ *after_sha256 = append_signatures_for_ਸਿਰਲੇਖ(&signatures, commit_ਬਫਰ_cursor,
									"gpgsig-sha256", "sha256");
		commit_ਬਫਰ_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	ਸੁਨੇਹਾ_2 = ਲੜੀ_ਖੋਜ(commit_ਬਫਰ_cursor, "\n\n");
	ਜੇ (ਸੁਨੇਹਾ_2)
		ਸੁਨੇਹਾ_2 += 2;

	ਜੇ (commit->parents &&
	    (get_object_mark(&commit->parents->ਵਸਤੂ->object) != 0 ||
	     reference_excluded_commits) &&
	    !ਭਰਿਆ_ਰੁੱਖ) {
		parse_commit_or_die(commit->parents->ਵਸਤੂ);
		diff_ਰੁੱਖ_oid(get_commit_ਰੁੱਖ_oid(commit->parents->ਵਸਤੂ),
			      get_commit_ਰੁੱਖ_oid(commit), "", &rev->diffopt);
	}
	ਨਹੀਂਤਾਂ
		diff_ਜੜ੍ਹ_ਰੁੱਖ_oid(get_commit_ਰੁੱਖ_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	ਲਈ (i = 0; i < diff_queued_diff.nr; i++)
		ਜੇ (!S_ISGITLINK(diff_queued_diff.ਕਤਾਰ[i]->two->ਬਹੁਲਕ))
			export_blob(&diff_queued_diff.ਕਤਾਰ[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	ਜੇ (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_ਸਤਰ(&committer, &committer_ਸਿਰਾ);
		anonymize_ident_ਸਤਰ(&author, &author_ਸਿਰਾ);
	}

	mark_ਅਗਲਾ_object(&commit->object);
	ਜੇ (anonymize) {
		reencoded = anonymize_commit_ਸੁਨੇਹਾ();
	} ਨਹੀਂਤਾਂ ਜੇ (encoding) {
		ਅੱਖਰ *buf;
		ਚੋਣ (reencode_ਬਹੁਲਕ) {
		ਹਾਲਤ REENCODE_YES:
			buf = xstrfmt("%.*s", (ਪੂਰਨ)encoding_len, encoding);
			reencoded = reencode_string(ਸੁਨੇਹਾ_2, "UTF-8", buf);
			ਮੁਕਤ(buf);
			ਰੋਕੋ;
		ਹਾਲਤ REENCODE_NO:
			ਰੋਕੋ;
		ਹਾਲਤ REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (ਪੂਰਨ)encoding_len, encoding,
			    oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
		}
	}
	ਜੇ (!commit->parents)
		ਲਿਖੋ("reset %s\n", refname);
	ਲਿਖੋ("commit %s\nmark :%"PRIu32"\n", refname, ਆਖਰੀ_idnum);
	ਜੇ (show_original_ids)
		ਲਿਖੋ("original-oid %s\n", oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
	ਲਿਖੋ("%.*s\n%.*s\n",
	       (ਪੂਰਨ)(author_ਸਿਰਾ - author), author,
	       (ਪੂਰਨ)(committer_ਸਿਰਾ - committer), committer);
	ਜੇ (signatures.nr) {
		ਚੋਣ (signed_commit_ਬਹੁਲਕ) {
		/* Exporting modes */
		ਹਾਲਤ SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
			/* fallthru */
		ਹਾਲਤ SIGN_VERBATIM:
			ਲਈ (ਮਾਪ_t i = 0; i < signatures.nr; i++) {
				ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ = &signatures.ਚੀਜ਼ਾਂ[i];
				print_signature(ਵਸਤੂ->string, ਵਸਤੂ->util);
			}
			ਰੋਕੋ;

		/* Stripping modes */
		ਹਾਲਤ SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
			/* fallthru */
		ਹਾਲਤ SIGN_STRIP:
			ਰੋਕੋ;

		/* Aborting modes */
		ਹਾਲਤ SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
		ਮੂਲ:
			BUG("invalid signed_commit_mode value %d", signed_commit_ਬਹੁਲਕ);
		}
		string_list_clear(&signatures, 0);
	}
	ਜੇ (!reencoded && encoding)
		ਲਿਖੋ("encoding %.*s\n", (ਪੂਰਨ)encoding_len, encoding);
	ਲਿਖੋ("data %u\n%s",
	       (ਬਿਨਾਨਿਸ਼ਾਨੀ)(reencoded
			  ? ਲੜੀ_ਲੰਬਾਈ(reencoded) : ਸੁਨੇਹਾ_2
			  ? ਲੜੀ_ਲੰਬਾਈ(ਸੁਨੇਹਾ_2) : 0),
	       reencoded ? reencoded : ਸੁਨੇਹਾ_2 ? ਸੁਨੇਹਾ_2 : "");
	ਮੁਕਤ(reencoded);
	repo_unuse_commit_ਬਫਰ(the_repository, commit, commit_ਬਫਰ);

	ਲਈ (i = 0, p = commit->parents; p; p = p->ਅਗਲਾ) {
		ਬਣਤਰ object *obj = &p->ਵਸਤੂ->object;
		ਪੂਰਨ mark = get_object_mark(obj);

		ਜੇ (!mark && !reference_excluded_commits)
			ਜਾਰੀ;
		ਜੇ (i == 0)
			ਲਿਖੋ("from ");
		ਨਹੀਂਤਾਂ
			ਲਿਖੋ("merge ");
		ਜੇ (mark)
			ਲਿਖੋ(":%d\n", mark);
		ਨਹੀਂਤਾਂ
			ਲਿਖੋ("%s\n",
			       anonymize ?
			       anonymize_oid(oid_ਮੰਜ਼ਿਲ_hex(&obj->oid)) :
			       oid_ਮੰਜ਼ਿਲ_hex(&obj->oid));
		i++;
	}

	ਜੇ (ਭਰਿਆ_ਰੁੱਖ)
		ਲਿਖੋ("deleteall\n");
	log_ਰੁੱਖ_diff_flush(rev);
	string_list_clear(paths_ਦਾ_ਬਦਲਿਆ_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	ਲਿਖੋ("\n");

	show_ਤਰੱਕੀ();
}

ਸਥਿਰ ਅੱਖਰ *anonymize_ਟੈਗ(ਖਾਲੀ)
{
	ਸਥਿਰ ਪੂਰਨ counter;
	ਬਣਤਰ strbuf ਬਾਹਰਲਾ = STRBUF_INIT;
	strbuf_addf(&ਬਾਹਰਲਾ, "tag message %d", counter++);
	ਵਾਪਸ strbuf_detach(&ਬਾਹਰਲਾ, ਨਿਰਾ);
}


ਸਥਿਰ ਖਾਲੀ handle_ਟੈਗ(ਅਚਲ ਅੱਖਰ *name, ਬਣਤਰ ਟੈਗ *ਟੈਗ)
{
	ਮਾਪ_t ਮਾਪ;
	ਗਿਣਤੀ object_type type;
	ਅੱਖਰ *buf;
	ਅਚਲ ਅੱਖਰ *tagger, *tagger_ਸਿਰਾ, *ਸੁਨੇਹਾ_2;
	ਮਾਪ_t ਸੁਨੇਹਾ_ਮਾਪ = 0;
	ਬਣਤਰ object *tagged;
	ਪੂਰਨ tagged_mark;
	ਬਣਤਰ commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = ਟੈਗ->tagged;
	ਜਦੋਂ (tagged->type == OBJ_ਟੈਗ) {
		tagged = ((ਬਣਤਰ ਟੈਗ *)tagged)->tagged;
	}
	ਜੇ (tagged->type == OBJ_ਰੁੱਖ) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_ਮੰਜ਼ਿਲ_hex(&ਟੈਗ->object.oid));
		ਵਾਪਸ;
	}

	buf = odb_read_object(the_repository->objects, &ਟੈਗ->object.oid,
			      &type, &ਮਾਪ);
	ਜੇ (!buf)
		die(_("could not read tag %s"), oid_ਮੰਜ਼ਿਲ_hex(&ਟੈਗ->object.oid));
	ਸੁਨੇਹਾ_2 = memmem(buf, ਮਾਪ, "\n\n", 2);
	ਜੇ (ਸੁਨੇਹਾ_2) {
		ਸੁਨੇਹਾ_2 += 2;
		ਸੁਨੇਹਾ_ਮਾਪ = ਲੜੀ_ਲੰਬਾਈ(ਸੁਨੇਹਾ_2);
	}
	tagger = memmem(buf, ਸੁਨੇਹਾ_2 ? ਸੁਨੇਹਾ_2 - buf : ਮਾਪ, "\ntagger ", 8);
	ਜੇ (!tagger) {
		ਜੇ (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		ਨਹੀਂਤਾਂ
			tagger = "";
		tagger_ਸਿਰਾ = tagger + ਲੜੀ_ਲੰਬਾਈ(tagger);
	} ਨਹੀਂਤਾਂ {
		tagger++;
		tagger_ਸਿਰਾ = strchrnul(tagger, '\n');
		ਜੇ (anonymize)
			anonymize_ident_ਸਤਰ(&tagger, &tagger_ਸਿਰਾ);
	}

	ਜੇ (anonymize) {
		name = anonymize_refname(name);
		ਜੇ (ਸੁਨੇਹਾ_2) {
			ਸਥਿਰ ਬਣਤਰ hashmap ਟੈਗ_ਸੂਚੀ;
			ਸੁਨੇਹਾ_2 = anonymize_str(&ਟੈਗ_ਸੂਚੀ, anonymize_ਟੈਗ,
						ਸੁਨੇਹਾ_2, ਸੁਨੇਹਾ_ਮਾਪ);
			ਸੁਨੇਹਾ_ਮਾਪ = ਲੜੀ_ਲੰਬਾਈ(ਸੁਨੇਹਾ_2);
		}
	}

	/* handle signed tags */
	ਜੇ (ਸੁਨੇਹਾ_2) {
		ਮਾਪ_t sig_ਵਿੱਥ = parse_signed_ਬਫਰ(ਸੁਨੇਹਾ_2, ਸੁਨੇਹਾ_ਮਾਪ);
		ਜੇ (sig_ਵਿੱਥ < ਸੁਨੇਹਾ_ਮਾਪ)
			ਚੋਣ (signed_ਟੈਗ_ਬਹੁਲਕ) {
			/* Exporting modes */
			ਹਾਲਤ SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_ਮੰਜ਼ਿਲ_hex(&ਟੈਗ->object.oid));
				/* fallthru */
			ਹਾਲਤ SIGN_VERBATIM:
				ਰੋਕੋ;

			/* Stripping modes */
			ਹਾਲਤ SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_ਮੰਜ਼ਿਲ_hex(&ਟੈਗ->object.oid));
				/* fallthru */
			ਹਾਲਤ SIGN_STRIP:
				ਸੁਨੇਹਾ_ਮਾਪ = sig_ਵਿੱਥ;
				ਰੋਕੋ;

			/* Aborting modes */
			ਹਾਲਤ SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_ਮੰਜ਼ਿਲ_hex(&ਟੈਗ->object.oid));
			ਮੂਲ:
				BUG("invalid signed_commit_mode value %d", signed_commit_ਬਹੁਲਕ);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = ਟੈਗ->tagged;
	tagged_mark = get_object_mark(tagged);
	ਜੇ (!tagged_mark) {
		ਚੋਣ (ਟੈਗ_ਦਾ_filtered_ਬਹੁਲਕ) {
		ਹਾਲਤ ਟੈਗ_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_ਮੰਜ਼ਿਲ_hex(&ਟੈਗ->object.oid));
		ਹਾਲਤ DROP:
			/* Ignore this tag altogether */
			ਮੁਕਤ(buf);
			ਵਾਪਸ;
		ਹਾਲਤ REWRITE:
			ਜੇ (tagged->type == OBJ_ਟੈਗ && !mark_ਟੈਗ_ਸੂਚੀ) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} ਨਹੀਂਤਾਂ ਜੇ (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((ਬਣਤਰ commit *)tagged);
				ਜੇ (!p) {
					ਲਿਖੋ("reset %s\nfrom %s\n\n",
					       name, oid_ਮੰਜ਼ਿਲ_hex(null_oid(the_hash_algo)));
					ਮੁਕਤ(buf);
					ਵਾਪਸ;
				}
				tagged_mark = get_object_mark(&p->object);
			} ਨਹੀਂਤਾਂ {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	ਜੇ (tagged->type == OBJ_ਟੈਗ) {
		ਲਿਖੋ("reset %s\nfrom %s\n\n",
		       name, oid_ਮੰਜ਼ਿਲ_hex(null_oid(the_hash_algo)));
	}
	skip_ਅਗੇਤਰ(name, "refs/tags/", &name);
	ਲਿਖੋ("tag %s\n", name);
	ਜੇ (mark_ਟੈਗ_ਸੂਚੀ) {
		mark_ਅਗਲਾ_object(&ਟੈਗ->object);
		ਲਿਖੋ("mark :%"PRIu32"\n", ਆਖਰੀ_idnum);
	}
	ਜੇ (tagged_mark)
		ਲਿਖੋ("from :%d\n", tagged_mark);
	ਨਹੀਂਤਾਂ
		ਲਿਖੋ("from %s\n", oid_ਮੰਜ਼ਿਲ_hex(&tagged->oid));

	ਜੇ (show_original_ids)
		ਲਿਖੋ("original-oid %s\n", oid_ਮੰਜ਼ਿਲ_hex(&ਟੈਗ->object.oid));
	ਲਿਖੋ("%.*s%sdata %d\n%.*s\n",
	       (ਪੂਰਨ)(tagger_ਸਿਰਾ - tagger), tagger,
	       tagger == tagger_ਸਿਰਾ ? "" : "\n",
	       (ਪੂਰਨ)ਸੁਨੇਹਾ_ਮਾਪ, (ਪੂਰਨ)ਸੁਨੇਹਾ_ਮਾਪ, ਸੁਨੇਹਾ_2 ? ਸੁਨੇਹਾ_2 : "");
	ਮੁਕਤ(buf);
}

ਸਥਿਰ ਬਣਤਰ commit *get_commit(ਬਣਤਰ rev_cmdline_ਇੰਦਰਾਜ *e, ਅਚਲ ਅੱਖਰ *ਭਰਿਆ_name)
{
	ਚੋਣ (e->ਵਸਤੂ->type) {
	ਹਾਲਤ OBJ_COMMIT:
		ਵਾਪਸ (ਬਣਤਰ commit *)e->ਵਸਤੂ;
	ਹਾਲਤ OBJ_ਟੈਗ: {
		ਬਣਤਰ ਟੈਗ *ਟੈਗ = (ਬਣਤਰ ਟੈਗ *)e->ਵਸਤੂ;

		/* handle nested tags */
		ਜਦੋਂ (ਟੈਗ && ਟੈਗ->object.type == OBJ_ਟੈਗ) {
			parse_object(the_repository, &ਟੈਗ->object.oid);
			string_list_append(&ਟੈਗ_refs, ਭਰਿਆ_name)->util = ਟੈਗ;
			ਟੈਗ = (ਬਣਤਰ ਟੈਗ *)ਟੈਗ->tagged;
		}
		ਜੇ (!ਟੈਗ)
			die(_("tag %s points nowhere?"), e->name);
		ਵਾਪਸ (ਬਣਤਰ commit *)ਟੈਗ;
	}
	ਮੂਲ:
		ਵਾਪਸ ਨਿਰਾ;
	}
}

ਸਥਿਰ ਖਾਲੀ get_ਟੈਗ_ਸੂਚੀ_and_duplicates(ਬਣਤਰ rev_cmdline_info *info)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < info->nr; i++) {
		ਬਣਤਰ rev_cmdline_ਇੰਦਰਾਜ *e = info->rev + i;
		ਬਣਤਰ object_id oid;
		ਬਣਤਰ commit *commit;
		ਅੱਖਰ *ਭਰਿਆ_name = ਨਿਰਾ;

		ਜੇ (e->flags & UNINTERESTING)
			ਜਾਰੀ;

		ਜੇ (repo_dwim_ref(the_repository, e->name, ਲੜੀ_ਲੰਬਾਈ(e->name),
				  &oid, &ਭਰਿਆ_name, 0) != 1) {
			ਮੁਕਤ(ਭਰਿਆ_name);
			ਜਾਰੀ;
		}

		ਜੇ (refspecs.nr) {
			ਅੱਖਰ *ਨਿਜੀ;
			ਨਿਜੀ = apply_refspecs(&refspecs, ਭਰਿਆ_name);
			ਜੇ (ਨਿਜੀ) {
				ਮੁਕਤ(ਭਰਿਆ_name);
				ਭਰਿਆ_name = ਨਿਜੀ;
			}
		}

		commit = get_commit(e, ਭਰਿਆ_name);
		ਜੇ (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->ਵਸਤੂ->type));
			ਮੁਕਤ(ਭਰਿਆ_name);
			ਜਾਰੀ;
		}

		ਚੋਣ (commit->object.type) {
		ਹਾਲਤ OBJ_COMMIT:
			ਰੋਕੋ;
		ਹਾਲਤ OBJ_BLOB:
			export_blob(&commit->object.oid);
			ਮੁਕਤ(ਭਰਿਆ_name);
			ਜਾਰੀ;
		ਮੂਲ: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			ਮੁਕਤ(ਭਰਿਆ_name);
			ਜਾਰੀ;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		ਜੇ (e->ਵਸਤੂ->type != OBJ_ਟੈਗ)
			string_list_append(&extra_refs, ਭਰਿਆ_name)->util = commit;

		ਜੇ (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = ਭਰਿਆ_name;
		ਨਹੀਂਤਾਂ
			ਮੁਕਤ(ਭਰਿਆ_name);
	}

	string_list_ਛਾਂਟੋ_u(&extra_refs, 0);
}

ਸਥਿਰ ਖਾਲੀ handle_ਟੈਗ_ਸੂਚੀ_and_duplicates(ਬਣਤਰ string_list *extras)
{
	ਬਣਤਰ commit *commit;
	ਪੂਰਨ i;

	ਲਈ (i = extras->nr - 1; i >= 0; i--) {
		ਅਚਲ ਅੱਖਰ *name = extras->ਚੀਜ਼ਾਂ[i].string;
		ਬਣਤਰ object *object = extras->ਚੀਜ਼ਾਂ[i].util;
		ਪੂਰਨ mark;

		ਚੋਣ (object->type) {
		ਹਾਲਤ OBJ_ਟੈਗ:
			handle_ਟੈਗ(name, (ਬਣਤਰ ਟੈਗ *)object);
			ਰੋਕੋ;
		ਹਾਲਤ OBJ_COMMIT:
			ਜੇ (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((ਬਣਤਰ commit *)object);
			ਜੇ (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				ਲਿਖੋ("reset %s\nfrom %s\n\n",
				       name, oid_ਮੰਜ਼ਿਲ_hex(null_oid(the_hash_algo)));
				ਜਾਰੀ;
			}

			mark = get_object_mark(&commit->object);
			ਜੇ (!mark) {
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
				ਜੇ (!reference_excluded_commits) {
					/* delete the ref */
					ਲਿਖੋ("reset %s\nfrom %s\n\n",
					       name, oid_ਮੰਜ਼ਿਲ_hex(null_oid(the_hash_algo)));
					ਜਾਰੀ;
				}
				/* set ref to commit using oid, not mark */
				ਲਿਖੋ("reset %s\nfrom %s\n\n", name,
				       oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
				ਜਾਰੀ;
			}

			ਲਿਖੋ("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_ਤਰੱਕੀ();
			ਰੋਕੋ;
		}
	}
}

ਸਥਿਰ ਖਾਲੀ export_marks(ਅੱਖਰ *file)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
	uint32_t mark;
	ਬਣਤਰ decoration_ਇੰਦਰਾਜ *deco = idnums.entries;
	FILE *f;
	ਪੂਰਨ e = 0;

	f = fopen_for_writing(file);
	ਜੇ (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	ਲਈ (i = 0; i < idnums.ਮਾਪ; i++) {
		ਜੇ (deco->ਅਧਾਰ && deco->ਅਧਾਰ->type == 1) {
			mark = ptr_ਮੰਜ਼ਿਲ_mark(deco->decoration);
			ਜੇ (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_ਮੰਜ਼ਿਲ_hex(&deco->ਅਧਾਰ->oid)) < 0) {
			    e = 1;
			    ਰੋਕੋ;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	ਜੇ (e)
		error(_("unable to write marks file %s."), file);
}

ਸਥਿਰ ਖਾਲੀ import_marks(ਅੱਖਰ *ਨਿਵੇਸ਼_file, ਪੂਰਨ check_exists)
{
	ਅੱਖਰ ਸਤਰ[512];
	FILE *f;
	ਬਣਤਰ stat sb;

	ਜੇ (check_exists && stat(ਨਿਵੇਸ਼_file, &sb))
		ਵਾਪਸ;

	f = xfopen(ਨਿਵੇਸ਼_file, "r");
	ਜਦੋਂ (fgets(ਸਤਰ, ਆਕਾਰ(ਸਤਰ), f)) {
		uint32_t mark;
		ਅੱਖਰ *ਸਤਰ_ਸਿਰਾ, *mark_ਸਿਰਾ;
		ਬਣਤਰ object_id oid;
		ਬਣਤਰ object *object;
		ਬਣਤਰ commit *commit;
		ਗਿਣਤੀ object_type type;

		ਸਤਰ_ਸਿਰਾ = strchr(ਸਤਰ, '\n');
		ਜੇ (ਸਤਰ[0] != ':' || !ਸਤਰ_ਸਿਰਾ)
			die(_("corrupt mark line: %s"), ਸਤਰ);
		*ਸਤਰ_ਸਿਰਾ = '\0';

		mark = strtoumax(ਸਤਰ + 1, &mark_ਸਿਰਾ, 10);
		ਜੇ (!mark || mark_ਸਿਰਾ == ਸਤਰ + 1
			|| *mark_ਸਿਰਾ != ' ' || get_oid_hex(mark_ਸਿਰਾ + 1, &oid))
			die(_("corrupt mark line: %s"), ਸਤਰ);

		ਜੇ (ਆਖਰੀ_idnum < mark)
			ਆਖਰੀ_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, ਨਿਰਾ);
		ਜੇ (type < 0)
			die(_("object not found: %s"), oid_ਮੰਜ਼ਿਲ_hex(&oid));

		ਜੇ (type != OBJ_COMMIT)
			/* only commits */
			ਜਾਰੀ;

		commit = lookup_commit(the_repository, &oid);
		ਜੇ (!commit)
			die(_("not a commit? can't happen: %s"), oid_ਮੰਜ਼ਿਲ_hex(&oid));

		object = &commit->object;

		ਜੇ (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_ਮੰਜ਼ਿਲ_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

ਸਥਿਰ ਖਾਲੀ handle_deletes(ਖਾਲੀ)
{
	ਪੂਰਨ i;
	ਲਈ (i = 0; i < refspecs.nr; i++) {
		ਬਣਤਰ refspec_ਵਸਤੂ *refspec = &refspecs.ਚੀਜ਼ਾਂ[i];
		ਜੇ (*refspec->src)
			ਜਾਰੀ;

		ਲਿਖੋ("reset %s\nfrom %s\n\n",
				refspec->dst, oid_ਮੰਜ਼ਿਲ_hex(null_oid(the_hash_algo)));
	}
}

ਸਥਿਰ ਪੂਰਨ parse_opt_anonymize_map(ਅਚਲ ਬਣਤਰ option *opt,
				   ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ hashmap *map = opt->ਮੁੱਲ;
	ਅਚਲ ਅੱਖਰ *delim, *ਮੁੱਲ;
	ਮਾਪ_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	ਜੇ (delim) {
		keylen = delim - arg;
		ਮੁੱਲ = delim + 1;
	} ਨਹੀਂਤਾਂ {
		keylen = ਲੜੀ_ਲੰਬਾਈ(arg);
		ਮੁੱਲ = arg;
	}

	ਜੇ (!keylen || !*ਮੁੱਲ)
		ਵਾਪਸ error(_("--anonymize-map token cannot be empty"));

	ਜੋੜੋ_anonymized_ਇੰਦਰਾਜ(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(ਮੁੱਲ));

	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_fast_export(ਪੂਰਨ argc,
		    ਅਚਲ ਅੱਖਰ **argv,
		    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ rev_info revs;
	ਬਣਤਰ commit *commit;
	ਅੱਖਰ *export_filename = ਨਿਰਾ,
	     *import_filename = ਨਿਰਾ,
	     *import_filename_if_exists = ਨਿਰਾ;
	uint32_t lastimportid;
	ਬਣਤਰ string_list refspecs_list = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ string_list paths_ਦਾ_ਬਦਲਿਆ_objects = STRING_LIST_INIT_DUP;
	ਬਣਤਰ option options[] = {
		OPT_INTEGER(0, "progress", &ਤਰੱਕੀ,
			    N_("show progress after <n> objects")),
		OPT_CALLBACK(0, "signed-tags", &signed_ਟੈਗ_ਬਹੁਲਕ, N_("mode"),
			     N_("select handling of signed tags"),
			     parse_opt_sign_ਬਹੁਲਕ),
		OPT_CALLBACK(0, "signed-commits", &signed_commit_ਬਹੁਲਕ, N_("mode"),
			     N_("select handling of signed commits"),
			     parse_opt_sign_ਬਹੁਲਕ),
		OPT_CALLBACK(0, "tag-of-filtered-object", &ਟੈਗ_ਦਾ_filtered_ਬਹੁਲਕ, N_("mode"),
			     N_("select handling of tags that tag filtered objects"),
			     parse_opt_ਟੈਗ_ਦਾ_filtered_ਬਹੁਲਕ),
		OPT_CALLBACK(0, "reencode", &reencode_ਬਹੁਲਕ, N_("mode"),
			     N_("select handling of commit messages in an alternate encoding"),
			     parse_opt_reencode_ਬਹੁਲਕ),
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
		OPT_BOOL(0, "full-tree", &ਭਰਿਆ_ਰੁੱਖ,
			 N_("output full tree for each commit")),
		OPT_BOOL(0, "use-done-feature", &use_ਪੂਰਾ_feature,
			     N_("use the done feature to terminate the stream")),
		OPT_BOOL(0, "no-data", &no_ਅੰਕੜੇ, N_("skip output of blob data")),
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
		OPT_BOOL(0, "mark-tags", &mark_ਟੈਗ_ਸੂਚੀ,
			    N_("label tags with mark ids")),

		OPT_ਸਿਰਾ()
	};

	ਜੇ (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, ਅਗੇਤਰ);
	init_revision_sources(&revision_sources);
	revs.topo_ਕ੍ਰਮ = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = ਤਿਆਰੀ_revisions(argc, argv, &revs, ਨਿਰਾ);
	ਜੇ (argc > 1)
		usage_with_options (fast_export_usage, options);

	ਜੇ (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	ਜੇ (refspecs_list.nr) {
		ਪੂਰਨ i;

		ਲਈ (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.ਚੀਜ਼ਾਂ[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	ਜੇ (use_ਪੂਰਾ_feature)
		ਲਿਖੋ("feature done\n");

	ਜੇ (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	ਜੇ (import_filename)
		import_marks(import_filename, 0);
	ਨਹੀਂਤਾਂ ਜੇ (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = ਆਖਰੀ_idnum;

	ਜੇ (import_filename && revs.prune_ਅੰਕੜੇ.nr)
		ਭਰਿਆ_ਰੁੱਖ = 1;

	get_ਟੈਗ_ਸੂਚੀ_and_duplicates(&revs.cmdline);

	ਜੇ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.ਉਲਟ = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_ਅੰਕੜੇ = &paths_ਦਾ_ਬਦਲਿਆ_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	ਜਦੋਂ ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_ਦਾ_ਬਦਲਿਆ_objects);
	revs.diffopt.no_free = 0;

	handle_ਟੈਗ_ਸੂਚੀ_and_duplicates(&extra_refs);
	handle_ਟੈਗ_ਸੂਚੀ_and_duplicates(&ਟੈਗ_refs);
	handle_deletes();

	ਜੇ (export_filename && lastimportid != ਆਖਰੀ_idnum)
		export_marks(export_filename);

	ਜੇ (use_ਪੂਰਾ_feature)
		ਲਿਖੋ("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	ਵਾਪਸ 0;
}
