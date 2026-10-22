/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "refs.h"
#ضمن "refspec.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "commit.h"
#ضمن "object.h"
#ضمن "tag.h"
#ضمن "diff.h"
#ضمن "diffcore.h"
#ضمن "log-tree.h"
#ضمن "revision.h"
#ضمن "decorate.h"
#ضمن "string-list.h"
#ضمن "utf8.h"
#ضمن "parse-options.h"
#ضمن "quote.h"
#ضمن "remote.h"
#ضمن "blob.h"
#ضمن "gpg-interface.h"

ساكن ثابت حرف *ثابت fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	عدم
};

ساكن صحيح پرمختګ;
ساكن تعداد sign_ډېرپېښ signed_نښان_ډېرپېښ = SIGN_ABORT;
ساكن تعداد sign_ډېرپېښ signed_commit_ډېرپېښ = SIGN_STRIP;
ساكن تعداد نښان_د_filtered_ډېرپېښ { نښان_FILTERING_ABORT, DROP, REWRITE } نښان_د_filtered_ډېرپېښ = نښان_FILTERING_ABORT;
ساكن تعداد reencode_ډېرپېښ { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_ډېرپېښ = REENCODE_ABORT;
ساكن صحيح fake_missing_tagger;
ساكن صحيح use_بشپړ_feature;
ساكن صحيح no_معلومات;
ساكن صحيح ډک_ونه;
ساكن صحيح reference_excluded_commits;
ساكن صحيح show_original_ids;
ساكن صحيح mark_نښانونه;
ساكن بنية string_list extra_refs = STRING_LIST_INIT_DUP;
ساكن بنية string_list نښان_refs = STRING_LIST_INIT_DUP;
ساكن بنية refspec refspecs;
ساكن صحيح anonymize;
ساكن بنية hashmap anonymized_seeds;
ساكن بنية revision_sources revision_sources;

ساكن صحيح parse_opt_sign_ډېرپېښ(ثابت بنية option *opt,
			       ثابت حرف *arg, صحيح unset)
{
	تعداد sign_ډېرپېښ *val = opt->ارزښت;

	إذا (unset)
		أرجع 0;

	إذا (parse_sign_ډېرپېښ(arg, val, عدم) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		أرجع error(_("unknown %s mode: %s"), opt->long_name, arg);

	أرجع 0;
}

ساكن صحيح parse_opt_نښان_د_filtered_ډېرپېښ(ثابت بنية option *opt,
					  ثابت حرف *arg, صحيح unset)
{
	تعداد نښان_د_filtered_ډېرپېښ *val = opt->ارزښت;

	إذا (unset || !strcmp(arg, "abort"))
		*val = نښان_FILTERING_ABORT;
	وإلا إذا (!strcmp(arg, "drop"))
		*val = DROP;
	وإلا إذا (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	وإلا
		أرجع error(_("unknown tag-of-filtered mode: %s"), arg);
	أرجع 0;
}

ساكن صحيح parse_opt_reencode_ډېرپېښ(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	تعداد reencode_ډېرپېښ *val = opt->ارزښت;

	إذا (unset) {
		*val = REENCODE_ABORT;
		أرجع 0;
	}

	بدل (git_parse_maybe_bool(arg)) {
	حالة 0:
		*val = REENCODE_NO;
		اكسر;
	حالة 1:
		*val = REENCODE_YES;
		اكسر;
	افتراضي:
		إذا (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		وإلا
			أرجع error(_("unknown reencoding mode: %s"), arg);
	}

	أرجع 0;
}

ساكن بنية decoration idnums;
ساكن uint32_t وروستی_idnum;
بنية anonymized_ثبت {
	بنية hashmap_ثبت hash;
	حرف *anon;
	ثابت حرف orig[FLEX_ARRAY];
};

بنية anonymized_ثبت_کلی {
	بنية hashmap_ثبت hash;
	ثابت حرف *orig;
	اندازه_t orig_len;
};

ساكن صحيح anonymized_ثبت_cmp(ثابت فراغ *cmp_معلومات ناکارول_شوی,
				ثابت بنية hashmap_ثبت *eptr,
				ثابت بنية hashmap_ثبت *ثبت_or_کلی,
				ثابت فراغ *keydata)
{
	ثابت بنية anonymized_ثبت *a, *b;

	a = container_د(eptr, ثابت بنية anonymized_ثبت, hash);
	إذا (keydata) {
		ثابت بنية anonymized_ثبت_کلی *کلی = keydata;
		صحيح equal = !xstrncmpz(a->orig, کلی->orig, کلی->orig_len);
		أرجع !equal;
	}

	b = container_د(ثبت_or_کلی, ثابت بنية anonymized_ثبت, hash);
	أرجع strcmp(a->orig, b->orig);
}

ساكن بنية anonymized_ثبت *جمع_کړه_anonymized_ثبت(بنية hashmap *map,
						     بینښه hash,
						     ثابت حرف *orig, اندازه_t len,
						     حرف *anon)
{
	بنية anonymized_ثبت *ret, *old;

	إذا (!map->cmpfn)
		hashmap_init(map, anonymized_ثبت_cmp, عدم, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_ثبت_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_ثبت(map, ret, hash);

	إذا (old) {
		حرر(old->anon);
		حرر(old);
	}

	أرجع ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
ساكن ثابت حرف *anonymize_str(بنية hashmap *map,
				 حرف *(*generate)(فراغ),
				 ثابت حرف *orig, اندازه_t len)
{
	بنية anonymized_ثبت_کلی کلی;
	بنية anonymized_ثبت *ret;

	hashmap_ثبت_init(&کلی.hash, memhash(orig, len));
	کلی.orig = orig;
	کلی.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_ثبت(&anonymized_seeds, &کلی, hash, &کلی);

	/* ...otherwise check if we've already seen it in this context... */
	إذا (!ret)
		ret = hashmap_get_ثبت(map, &کلی, hash, &کلی);

	/* ...and finally generate a new mapping if necessary */
	إذا (!ret)
		ret = جمع_کړه_anonymized_ثبت(map, کلی.hash.hash,
					   orig, len, generate());

	أرجع ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
ساكن فراغ anonymize_لار(بنية strbuf *بهر, ثابت حرف *لار,
			   بنية hashmap *map,
			   حرف *(*generate)(فراغ))
{
	طالما (*لار) {
		ثابت حرف *پای_ټکی_د_component = strchrnul(لار, '/');
		اندازه_t len = پای_ټکی_د_component - لار;
		ثابت حرف *c = anonymize_str(map, generate, لار, len);
		strbuf_addstr(بهر, c);
		لار = پای_ټکی_د_component;
		إذا (*لار)
			strbuf_addch(بهر, *لار++);
	}
}

ساكن مضمن فراغ *mark_منزل_ptr(uint32_t mark)
{
	أرجع (فراغ *)(uintptr_t)mark;
}

ساكن مضمن uint32_t ptr_منزل_mark(فراغ * mark)
{
	أرجع (uint32_t)(uintptr_t)mark;
}

ساكن مضمن فراغ mark_object(بنية object *object, uint32_t mark)
{
	جمع_کړه_decoration(&idnums, object, mark_منزل_ptr(mark));
}

ساكن مضمن فراغ mark_بل_object(بنية object *object)
{
	mark_object(object, ++وروستی_idnum);
}

ساكن صحيح get_object_mark(بنية object *object)
{
	فراغ *decoration = lookup_decoration(&idnums, object);
	إذا (!decoration)
		أرجع 0;
	أرجع ptr_منزل_mark(decoration);
}

ساكن بنية commit *rewrite_commit(بنية commit *p)
{
	لكل (;;) {
		إذا (p->parents && p->parents->بل)
			اكسر;
		إذا (p->object.flags & UNINTERESTING)
			اكسر;
		إذا (!(p->object.flags & TREESAME))
			اكسر;
		إذا (!p->parents)
			أرجع عدم;
		p = p->parents->توکی;
	}
	أرجع p;
}

ساكن فراغ show_پرمختګ(فراغ)
{
	ساكن صحيح counter = 0;
	إذا (!پرمختګ)
		أرجع;
	إذا ((++counter % پرمختګ) == 0)
		چاپ_کړه("progress %d objects\n", counter);
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
ساكن حرف *anonymize_blob(بینښه اوږد *اندازه)
{
	ساكن صحيح counter;
	بنية strbuf بهر = STRBUF_INIT;
	strbuf_addf(&بهر, "anonymous blob %d", counter++);
	*اندازه = بهر.len;
	أرجع strbuf_detach(&بهر, عدم);
}

ساكن فراغ export_blob(ثابت بنية object_id *oid)
{
	بینښه اوږد اندازه;
	تعداد object_type type;
	حرف *buf;
	بنية object *object;
	صحيح eaten;

	إذا (no_معلومات)
		أرجع;

	إذا (is_null_oid(oid))
		أرجع;

	object = lookup_object(the_repository, oid);
	إذا (object && object->flags & SHOWN)
		أرجع;

	إذا (anonymize) {
		buf = anonymize_blob(&اندازه);
		object = (بنية object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} وإلا {
		اندازه_t اندازه_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &اندازه_st);
		اندازه = cast_اندازه_t_منزل_ulong(اندازه_st);
		إذا (!buf)
			die(_("could not read blob %s"), oid_منزل_hex(oid));
		إذا (check_object_signature(the_repository, oid, buf, اندازه,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_منزل_hex(oid));
		object = parse_object_بفر(the_repository, oid, type,
					     اندازه, buf, &eaten);
	}

	إذا (!object)
		die(_("could not read blob %s"), oid_منزل_hex(oid));

	mark_بل_object(object);

	چاپ_کړه("blob\nmark :%"PRIu32"\n", وروستی_idnum);
	إذا (show_original_ids)
		چاپ_کړه("original-oid %s\n", oid_منزل_hex(oid));
	چاپ_کړه("data %"PRIuMAX"\n", (uintmax_t)اندازه);
	إذا (اندازه && fwrite(buf, اندازه, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_منزل_hex(oid));
	چاپ_کړه("\n");

	show_پرمختګ();

	object->flags |= SHOWN;
	إذا (!eaten)
		حرر(buf);
}

ساكن صحيح ژوروالی_لومړی(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت بنية diff_filepair *a = *((ثابت بنية diff_filepair **)a_);
	ثابت بنية diff_filepair *b = *((ثابت بنية diff_filepair **)b_);
	ثابت حرف *name_a, *name_b;
	صحيح len_a, len_b, len;
	صحيح cmp;

	name_a = a->one ? a->one->لار : a->two->لار;
	name_b = b->one ? b->one->لار : b->two->لار;

	len_a = مزي_اوږدوالی(name_a);
	len_b = مزي_اوږدوالی(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	إذا (cmp)
		أرجع cmp;
	cmp = len_b - len_a;
	إذا (cmp)
		أرجع cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	أرجع (a->حالت == 'R') - (b->حالت == 'R');
}

ساكن فراغ print_لار_1(ثابت حرف *لار)
{
	صحيح need_quote = quote_c_style(لار, عدم, عدم, 0);
	إذا (need_quote)
		quote_c_style(لار, عدم, stdout, 0);
	وإلا إذا (strchr(لار, ' '))
		چاپ_کړه("\"%s\"", لار);
	وإلا
		چاپ_کړه("%s", لار);
}

ساكن حرف *anonymize_لار_component(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf بهر = STRBUF_INIT;
	strbuf_addf(&بهر, "path%d", counter++);
	أرجع strbuf_detach(&بهر, عدم);
}

ساكن فراغ print_لار(ثابت حرف *لار)
{
	إذا (!anonymize)
		print_لار_1(لار);
	وإلا {
		ساكن بنية hashmap paths;
		ساكن بنية strbuf anon = STRBUF_INIT;

		anonymize_لار(&anon, لار, &paths, anonymize_لار_component);
		print_لار_1(anon.buf);
		strbuf_reset(&anon);
	}
}

ساكن حرف *generate_fake_oid(فراغ)
{
	ساكن uint32_t counter = 1; /* avoid null oid */
	ثابت بینښه hashsz = the_hash_algo->rawsz;
	بنية object_id oid;
	حرف *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	أرجع oid_منزل_hex_r(hex, &oid);
}

ساكن ثابت حرف *anonymize_oid(ثابت حرف *oid_hex)
{
	ساكن بنية hashmap objs;
	اندازه_t len = مزي_اوږدوالی(oid_hex);
	أرجع anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

ساكن فراغ show_filemodify(بنية diff_کتار_struct *q,
			    بنية diff_options *options ناکارول_شوی, فراغ *معلومات)
{
	صحيح i;
	بنية string_list *بدل_شوی = معلومات;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->کتار, q->nr, ژوروالی_لومړی);

	لكل (i = 0; i < q->nr; i++) {
		بنية diff_filespec *ospec = q->کتار[i]->one;
		بنية diff_filespec *spec = q->کتار[i]->two;

		بدل (q->کتار[i]->حالت) {
		حالة DIFF_حالت_DELETED:
			چاپ_کړه("D ");
			print_لار(spec->لار);
			string_list_ورننباسه(بدل_شوی, spec->لار);
			توری_چاپ_کړه('\n');
			اكسر;

		حالة DIFF_حالت_COPIED:
		حالة DIFF_حالت_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			إذا (!string_list_has_string(بدل_شوی, ospec->لار)) {
				چاپ_کړه("%c ", q->کتار[i]->حالت);
				print_لار(ospec->لار);
				توری_چاپ_کړه(' ');
				print_لار(spec->لار);
				string_list_ورننباسه(بدل_شوی, spec->لار);
				توری_چاپ_کړه('\n');

				إذا (oideq(&ospec->oid, &spec->oid) &&
				    ospec->ډېرپېښ == spec->ډېرپېښ)
					اكسر;
			}
			/* fallthrough */

		حالة DIFF_حالت_TYPE_بدل_شوی:
		حالة DIFF_حالت_MODIFIED:
		حالة DIFF_حالت_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			إذا (no_معلومات || S_ISGITLINK(spec->ډېرپېښ))
				چاپ_کړه("M %06o %s ", spec->ډېرپېښ,
				       anonymize ?
				       anonymize_oid(oid_منزل_hex(&spec->oid)) :
				       oid_منزل_hex(&spec->oid));
			وإلا {
				بنية object *object = lookup_object(the_repository,
								      &spec->oid);
				چاپ_کړه("M %06o :%d ", spec->ډېرپېښ,
				       get_object_mark(object));
			}
			print_لار(spec->لار);
			string_list_ورننباسه(بدل_شوی, spec->لار);
			توری_چاپ_کړه('\n');
			اكسر;

		افتراضي:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->کتار[i]->حالت,
			    ospec->لار ? ospec->لار : _("none"),
			    spec->لار ? spec->لار : _("none"));
		}
	}
}

ساكن حرف *anonymize_ref_component(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf بهر = STRBUF_INIT;
	strbuf_addf(&بهر, "ref%d", counter++);
	أرجع strbuf_detach(&بهر, عدم);
}

ساكن ثابت حرف *anonymize_refname(ثابت حرف *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	ساكن ثابت حرف *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	ساكن بنية hashmap refs;
	ساكن بنية strbuf anon = STRBUF_INIT;
	صحيح i;

	strbuf_reset(&anon);
	لكل (i = 0; i < ARRAY_اندازه(prefixes); i++) {
		إذا (skip_مختاړی(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			اكسر;
		}
	}

	anonymize_لار(&anon, refname, &refs, anonymize_ref_component);
	أرجع anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
ساكن حرف *anonymize_commit_پیغام(فراغ)
{
	ساكن صحيح counter;
	أرجع xstrfmt("subject %d\n\nbody\n", counter++);
}

ساكن حرف *anonymize_ident(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf بهر = STRBUF_INIT;
	strbuf_addf(&بهر, "User %d <user%d@example.com>", counter, counter);
	counter++;
	أرجع strbuf_detach(&بهر, عدم);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
ساكن فراغ anonymize_ident_کرښه(ثابت حرف **beg, ثابت حرف **پای_ټکی)
{
	ساكن بنية hashmap idents;
	ساكن بنية strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	ساكن بینښه which_بفر;

	بنية strbuf *بهر;
	بنية ident_وېشل وېشل;
	ثابت حرف *پای_ټکی_د_سرلیک;

	بهر = &buffers[which_بفر++];
	which_بفر %= ARRAY_اندازه(buffers);
	strbuf_reset(بهر);

	/* skip "committer", "author", "tagger", etc */
	پای_ټکی_د_سرلیک = strchr(*beg, ' ');
	إذا (!پای_ټکی_د_سرلیک)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (صحيح)(*پای_ټکی - *beg), *beg);
	پای_ټکی_د_سرلیک++;
	strbuf_جمع_کړه(بهر, *beg, پای_ټکی_د_سرلیک - *beg);

	إذا (!وېشل_ident_کرښه(&وېشل, پای_ټکی_د_سرلیک, *پای_ټکی - پای_ټکی_د_سرلیک) &&
	    وېشل.date_begin) {
		ثابت حرف *ident;
		اندازه_t len;

		len = وېشل.mail_پای_ټکی - وېشل.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      وېشل.name_begin, len);
		strbuf_addstr(بهر, ident);
		strbuf_addch(بهر, ' ');
		strbuf_جمع_کړه(بهر, وېشل.date_begin, وېشل.tz_پای_ټکی - وېشل.date_begin);
	} وإلا {
		strbuf_addstr(بهر, "Malformed Ident <malformed@example.com> 0 -0000");
	}

	*beg = بهر->buf;
	*پای_ټکی = بهر->buf + بهر->len;
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
ساكن ثابت حرف *پیدا_کړه_commit_multiline_سرلیک(ثابت حرف *پیغام,
						ثابت حرف *کلی,
						ثابت حرف **پای_ټکی)
{
	بنية strbuf val = STRBUF_INIT;
	ثابت حرف *bol, *eol;
	اندازه_t len;

	bol = پیدا_کړه_commit_سرلیک(پیغام, کلی, &len);
	إذا (!bol)
		أرجع عدم;
	eol = bol + len;
	strbuf_جمع_کړه(&val, bol, len);

	طالما (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_جمع_کړه(&val, bol, eol - bol);
	}

	*پای_ټکی = eol;
	أرجع strbuf_detach(&val, عدم);
}

ساكن فراغ print_signature(ثابت حرف *signature, ثابت حرف *object_hash)
{
	إذا (!signature)
		أرجع;

	چاپ_کړه("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (بینښه)مزي_اوږدوالی(signature),
	       signature);
}

ساكن ثابت حرف *append_signatures_for_سرلیک(بنية string_list *signatures,
						ثابت حرف *pos,
						ثابت حرف *سرلیک,
						ثابت حرف *object_hash)
{
	ثابت حرف *signature;
	ثابت حرف *پیل = pos;
	ثابت حرف *پای_ټکی = pos;

	طالما ((signature = پیدا_کړه_commit_multiline_سرلیک(پیل + 1,
							 سرلیک,
							 &پای_ټکی))) {
		string_list_append(signatures, signature)->util = (فراغ *)object_hash;
		حرر((حرف *)signature);
		پیل = پای_ټکی;
	}

	أرجع پای_ټکی;
}

ساكن فراغ handle_commit(بنية commit *commit, بنية rev_info *rev,
			  بنية string_list *paths_د_بدل_شوی_objects)
{
	صحيح saved_output_format = rev->diffopt.output_format;
	ثابت حرف *commit_بفر, *commit_بفر_cursor;
	ثابت حرف *author, *author_پای_ټکی, *committer, *committer_پای_ټکی;
	ثابت حرف *encoding = عدم;
	اندازه_t encoding_len;
	بنية string_list signatures = STRING_LIST_INIT_DUP;
	ثابت حرف *پیغام_2;
	حرف *reencoded = عدم;
	بنية commit_list *p;
	ثابت حرف *refname;
	صحيح i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_بفر_cursor = commit_بفر = repo_get_commit_بفر(the_repository, commit, عدم);

	author = مزي_پلټنه(commit_بفر_cursor, "\nauthor ");
	إذا (!author)
		die(_("could not find author in commit %s"),
		    oid_منزل_hex(&commit->object.oid));
	author++;
	commit_بفر_cursor = author_پای_ټکی = strchrnul(author, '\n');

	committer = مزي_پلټنه(commit_بفر_cursor, "\ncommitter ");
	إذا (!committer)
		die(_("could not find committer in commit %s"),
		    oid_منزل_hex(&commit->object.oid));
	committer++;
	commit_بفر_cursor = committer_پای_ټکی = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	إذا (*commit_بفر_cursor == '\n') {
		encoding = پیدا_کړه_commit_سرلیک(commit_بفر_cursor + 1, "encoding", &encoding_len);
		إذا (encoding)
			commit_بفر_cursor = encoding + encoding_len;
	}

	إذا (*commit_بفر_cursor == '\n') {
		ثابت حرف *after_sha1 = append_signatures_for_سرلیک(&signatures, commit_بفر_cursor,
								      "gpgsig", "sha1");
		ثابت حرف *after_sha256 = append_signatures_for_سرلیک(&signatures, commit_بفر_cursor,
									"gpgsig-sha256", "sha256");
		commit_بفر_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	پیغام_2 = مزي_پلټنه(commit_بفر_cursor, "\n\n");
	إذا (پیغام_2)
		پیغام_2 += 2;

	إذا (commit->parents &&
	    (get_object_mark(&commit->parents->توکی->object) != 0 ||
	     reference_excluded_commits) &&
	    !ډک_ونه) {
		parse_commit_or_die(commit->parents->توکی);
		diff_ونه_oid(get_commit_ونه_oid(commit->parents->توکی),
			      get_commit_ونه_oid(commit), "", &rev->diffopt);
	}
	وإلا
		diff_ریښه_ونه_oid(get_commit_ونه_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	لكل (i = 0; i < diff_queued_diff.nr; i++)
		إذا (!S_ISGITLINK(diff_queued_diff.کتار[i]->two->ډېرپېښ))
			export_blob(&diff_queued_diff.کتار[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	إذا (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_کرښه(&committer, &committer_پای_ټکی);
		anonymize_ident_کرښه(&author, &author_پای_ټکی);
	}

	mark_بل_object(&commit->object);
	إذا (anonymize) {
		reencoded = anonymize_commit_پیغام();
	} وإلا إذا (encoding) {
		حرف *buf;
		بدل (reencode_ډېرپېښ) {
		حالة REENCODE_YES:
			buf = xstrfmt("%.*s", (صحيح)encoding_len, encoding);
			reencoded = reencode_string(پیغام_2, "UTF-8", buf);
			حرر(buf);
			اكسر;
		حالة REENCODE_NO:
			اكسر;
		حالة REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (صحيح)encoding_len, encoding,
			    oid_منزل_hex(&commit->object.oid));
		}
	}
	إذا (!commit->parents)
		چاپ_کړه("reset %s\n", refname);
	چاپ_کړه("commit %s\nmark :%"PRIu32"\n", refname, وروستی_idnum);
	إذا (show_original_ids)
		چاپ_کړه("original-oid %s\n", oid_منزل_hex(&commit->object.oid));
	چاپ_کړه("%.*s\n%.*s\n",
	       (صحيح)(author_پای_ټکی - author), author,
	       (صحيح)(committer_پای_ټکی - committer), committer);
	إذا (signatures.nr) {
		بدل (signed_commit_ډېرپېښ) {
		/* Exporting modes */
		حالة SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_منزل_hex(&commit->object.oid));
			/* fallthru */
		حالة SIGN_VERBATIM:
			لكل (اندازه_t i = 0; i < signatures.nr; i++) {
				بنية string_list_توکی *توکی = &signatures.توکي[i];
				print_signature(توکی->string, توکی->util);
			}
			اكسر;

		/* Stripping modes */
		حالة SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_منزل_hex(&commit->object.oid));
			/* fallthru */
		حالة SIGN_STRIP:
			اكسر;

		/* Aborting modes */
		حالة SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_منزل_hex(&commit->object.oid));
		افتراضي:
			BUG("invalid signed_commit_mode value %d", signed_commit_ډېرپېښ);
		}
		string_list_clear(&signatures, 0);
	}
	إذا (!reencoded && encoding)
		چاپ_کړه("encoding %.*s\n", (صحيح)encoding_len, encoding);
	چاپ_کړه("data %u\n%s",
	       (بینښه)(reencoded
			  ? مزي_اوږدوالی(reencoded) : پیغام_2
			  ? مزي_اوږدوالی(پیغام_2) : 0),
	       reencoded ? reencoded : پیغام_2 ? پیغام_2 : "");
	حرر(reencoded);
	repo_unuse_commit_بفر(the_repository, commit, commit_بفر);

	لكل (i = 0, p = commit->parents; p; p = p->بل) {
		بنية object *obj = &p->توکی->object;
		صحيح mark = get_object_mark(obj);

		إذا (!mark && !reference_excluded_commits)
			استمر;
		إذا (i == 0)
			چاپ_کړه("from ");
		وإلا
			چاپ_کړه("merge ");
		إذا (mark)
			چاپ_کړه(":%d\n", mark);
		وإلا
			چاپ_کړه("%s\n",
			       anonymize ?
			       anonymize_oid(oid_منزل_hex(&obj->oid)) :
			       oid_منزل_hex(&obj->oid));
		i++;
	}

	إذا (ډک_ونه)
		چاپ_کړه("deleteall\n");
	log_ونه_diff_flush(rev);
	string_list_clear(paths_د_بدل_شوی_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	چاپ_کړه("\n");

	show_پرمختګ();
}

ساكن حرف *anonymize_نښان(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf بهر = STRBUF_INIT;
	strbuf_addf(&بهر, "tag message %d", counter++);
	أرجع strbuf_detach(&بهر, عدم);
}


ساكن فراغ handle_نښان(ثابت حرف *name, بنية نښان *نښان)
{
	اندازه_t اندازه;
	تعداد object_type type;
	حرف *buf;
	ثابت حرف *tagger, *tagger_پای_ټکی, *پیغام_2;
	اندازه_t پیغام_اندازه = 0;
	بنية object *tagged;
	صحيح tagged_mark;
	بنية commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = نښان->tagged;
	طالما (tagged->type == OBJ_نښان) {
		tagged = ((بنية نښان *)tagged)->tagged;
	}
	إذا (tagged->type == OBJ_ونه) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_منزل_hex(&نښان->object.oid));
		أرجع;
	}

	buf = odb_read_object(the_repository->objects, &نښان->object.oid,
			      &type, &اندازه);
	إذا (!buf)
		die(_("could not read tag %s"), oid_منزل_hex(&نښان->object.oid));
	پیغام_2 = memmem(buf, اندازه, "\n\n", 2);
	إذا (پیغام_2) {
		پیغام_2 += 2;
		پیغام_اندازه = مزي_اوږدوالی(پیغام_2);
	}
	tagger = memmem(buf, پیغام_2 ? پیغام_2 - buf : اندازه, "\ntagger ", 8);
	إذا (!tagger) {
		إذا (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		وإلا
			tagger = "";
		tagger_پای_ټکی = tagger + مزي_اوږدوالی(tagger);
	} وإلا {
		tagger++;
		tagger_پای_ټکی = strchrnul(tagger, '\n');
		إذا (anonymize)
			anonymize_ident_کرښه(&tagger, &tagger_پای_ټکی);
	}

	إذا (anonymize) {
		name = anonymize_refname(name);
		إذا (پیغام_2) {
			ساكن بنية hashmap نښانونه;
			پیغام_2 = anonymize_str(&نښانونه, anonymize_نښان,
						پیغام_2, پیغام_اندازه);
			پیغام_اندازه = مزي_اوږدوالی(پیغام_2);
		}
	}

	/* handle signed tags */
	إذا (پیغام_2) {
		اندازه_t sig_دننه_واټن = parse_signed_بفر(پیغام_2, پیغام_اندازه);
		إذا (sig_دننه_واټن < پیغام_اندازه)
			بدل (signed_نښان_ډېرپېښ) {
			/* Exporting modes */
			حالة SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_منزل_hex(&نښان->object.oid));
				/* fallthru */
			حالة SIGN_VERBATIM:
				اكسر;

			/* Stripping modes */
			حالة SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_منزل_hex(&نښان->object.oid));
				/* fallthru */
			حالة SIGN_STRIP:
				پیغام_اندازه = sig_دننه_واټن;
				اكسر;

			/* Aborting modes */
			حالة SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_منزل_hex(&نښان->object.oid));
			افتراضي:
				BUG("invalid signed_commit_mode value %d", signed_commit_ډېرپېښ);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = نښان->tagged;
	tagged_mark = get_object_mark(tagged);
	إذا (!tagged_mark) {
		بدل (نښان_د_filtered_ډېرپېښ) {
		حالة نښان_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_منزل_hex(&نښان->object.oid));
		حالة DROP:
			/* Ignore this tag altogether */
			حرر(buf);
			أرجع;
		حالة REWRITE:
			إذا (tagged->type == OBJ_نښان && !mark_نښانونه) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} وإلا إذا (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((بنية commit *)tagged);
				إذا (!p) {
					چاپ_کړه("reset %s\nfrom %s\n\n",
					       name, oid_منزل_hex(null_oid(the_hash_algo)));
					حرر(buf);
					أرجع;
				}
				tagged_mark = get_object_mark(&p->object);
			} وإلا {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	إذا (tagged->type == OBJ_نښان) {
		چاپ_کړه("reset %s\nfrom %s\n\n",
		       name, oid_منزل_hex(null_oid(the_hash_algo)));
	}
	skip_مختاړی(name, "refs/tags/", &name);
	چاپ_کړه("tag %s\n", name);
	إذا (mark_نښانونه) {
		mark_بل_object(&نښان->object);
		چاپ_کړه("mark :%"PRIu32"\n", وروستی_idnum);
	}
	إذا (tagged_mark)
		چاپ_کړه("from :%d\n", tagged_mark);
	وإلا
		چاپ_کړه("from %s\n", oid_منزل_hex(&tagged->oid));

	إذا (show_original_ids)
		چاپ_کړه("original-oid %s\n", oid_منزل_hex(&نښان->object.oid));
	چاپ_کړه("%.*s%sdata %d\n%.*s\n",
	       (صحيح)(tagger_پای_ټکی - tagger), tagger,
	       tagger == tagger_پای_ټکی ? "" : "\n",
	       (صحيح)پیغام_اندازه, (صحيح)پیغام_اندازه, پیغام_2 ? پیغام_2 : "");
	حرر(buf);
}

ساكن بنية commit *get_commit(بنية rev_cmdline_ثبت *e, ثابت حرف *ډک_name)
{
	بدل (e->توکی->type) {
	حالة OBJ_COMMIT:
		أرجع (بنية commit *)e->توکی;
	حالة OBJ_نښان: {
		بنية نښان *نښان = (بنية نښان *)e->توکی;

		/* handle nested tags */
		طالما (نښان && نښان->object.type == OBJ_نښان) {
			parse_object(the_repository, &نښان->object.oid);
			string_list_append(&نښان_refs, ډک_name)->util = نښان;
			نښان = (بنية نښان *)نښان->tagged;
		}
		إذا (!نښان)
			die(_("tag %s points nowhere?"), e->name);
		أرجع (بنية commit *)نښان;
	}
	افتراضي:
		أرجع عدم;
	}
}

ساكن فراغ get_نښانونه_and_duplicates(بنية rev_cmdline_info *info)
{
	صحيح i;

	لكل (i = 0; i < info->nr; i++) {
		بنية rev_cmdline_ثبت *e = info->rev + i;
		بنية object_id oid;
		بنية commit *commit;
		حرف *ډک_name = عدم;

		إذا (e->flags & UNINTERESTING)
			استمر;

		إذا (repo_dwim_ref(the_repository, e->name, مزي_اوږدوالی(e->name),
				  &oid, &ډک_name, 0) != 1) {
			حرر(ډک_name);
			استمر;
		}

		إذا (refspecs.nr) {
			حرف *خاص;
			خاص = apply_refspecs(&refspecs, ډک_name);
			إذا (خاص) {
				حرر(ډک_name);
				ډک_name = خاص;
			}
		}

		commit = get_commit(e, ډک_name);
		إذا (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->توکی->type));
			حرر(ډک_name);
			استمر;
		}

		بدل (commit->object.type) {
		حالة OBJ_COMMIT:
			اكسر;
		حالة OBJ_BLOB:
			export_blob(&commit->object.oid);
			حرر(ډک_name);
			استمر;
		افتراضي: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			حرر(ډک_name);
			استمر;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		إذا (e->توکی->type != OBJ_نښان)
			string_list_append(&extra_refs, ډک_name)->util = commit;

		إذا (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = ډک_name;
		وإلا
			حرر(ډک_name);
	}

	string_list_ترتیبول_u(&extra_refs, 0);
}

ساكن فراغ handle_نښانونه_and_duplicates(بنية string_list *extras)
{
	بنية commit *commit;
	صحيح i;

	لكل (i = extras->nr - 1; i >= 0; i--) {
		ثابت حرف *name = extras->توکي[i].string;
		بنية object *object = extras->توکي[i].util;
		صحيح mark;

		بدل (object->type) {
		حالة OBJ_نښان:
			handle_نښان(name, (بنية نښان *)object);
			اكسر;
		حالة OBJ_COMMIT:
			إذا (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((بنية commit *)object);
			إذا (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				چاپ_کړه("reset %s\nfrom %s\n\n",
				       name, oid_منزل_hex(null_oid(the_hash_algo)));
				استمر;
			}

			mark = get_object_mark(&commit->object);
			إذا (!mark) {
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
				إذا (!reference_excluded_commits) {
					/* delete the ref */
					چاپ_کړه("reset %s\nfrom %s\n\n",
					       name, oid_منزل_hex(null_oid(the_hash_algo)));
					استمر;
				}
				/* set ref to commit using oid, not mark */
				چاپ_کړه("reset %s\nfrom %s\n\n", name,
				       oid_منزل_hex(&commit->object.oid));
				استمر;
			}

			چاپ_کړه("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_پرمختګ();
			اكسر;
		}
	}
}

ساكن فراغ export_marks(حرف *file)
{
	بینښه صحيح i;
	uint32_t mark;
	بنية decoration_ثبت *deco = idnums.entries;
	FILE *f;
	صحيح e = 0;

	f = fopen_for_writing(file);
	إذا (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	لكل (i = 0; i < idnums.اندازه; i++) {
		إذا (deco->بنسټ && deco->بنسټ->type == 1) {
			mark = ptr_منزل_mark(deco->decoration);
			إذا (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_منزل_hex(&deco->بنسټ->oid)) < 0) {
			    e = 1;
			    اكسر;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	إذا (e)
		error(_("unable to write marks file %s."), file);
}

ساكن فراغ import_marks(حرف *ننوت_file, صحيح check_exists)
{
	حرف کرښه[512];
	FILE *f;
	بنية stat sb;

	إذا (check_exists && stat(ننوت_file, &sb))
		أرجع;

	f = xfopen(ننوت_file, "r");
	طالما (fgets(کرښه, حجم(کرښه), f)) {
		uint32_t mark;
		حرف *کرښه_پای_ټکی, *mark_پای_ټکی;
		بنية object_id oid;
		بنية object *object;
		بنية commit *commit;
		تعداد object_type type;

		کرښه_پای_ټکی = strchr(کرښه, '\n');
		إذا (کرښه[0] != ':' || !کرښه_پای_ټکی)
			die(_("corrupt mark line: %s"), کرښه);
		*کرښه_پای_ټکی = '\0';

		mark = strtoumax(کرښه + 1, &mark_پای_ټکی, 10);
		إذا (!mark || mark_پای_ټکی == کرښه + 1
			|| *mark_پای_ټکی != ' ' || get_oid_hex(mark_پای_ټکی + 1, &oid))
			die(_("corrupt mark line: %s"), کرښه);

		إذا (وروستی_idnum < mark)
			وروستی_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, عدم);
		إذا (type < 0)
			die(_("object not found: %s"), oid_منزل_hex(&oid));

		إذا (type != OBJ_COMMIT)
			/* only commits */
			استمر;

		commit = lookup_commit(the_repository, &oid);
		إذا (!commit)
			die(_("not a commit? can't happen: %s"), oid_منزل_hex(&oid));

		object = &commit->object;

		إذا (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_منزل_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

ساكن فراغ handle_deletes(فراغ)
{
	صحيح i;
	لكل (i = 0; i < refspecs.nr; i++) {
		بنية refspec_توکی *refspec = &refspecs.توکي[i];
		إذا (*refspec->src)
			استمر;

		چاپ_کړه("reset %s\nfrom %s\n\n",
				refspec->dst, oid_منزل_hex(null_oid(the_hash_algo)));
	}
}

ساكن صحيح parse_opt_anonymize_map(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	بنية hashmap *map = opt->ارزښت;
	ثابت حرف *delim, *ارزښت;
	اندازه_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	إذا (delim) {
		keylen = delim - arg;
		ارزښت = delim + 1;
	} وإلا {
		keylen = مزي_اوږدوالی(arg);
		ارزښت = arg;
	}

	إذا (!keylen || !*ارزښت)
		أرجع error(_("--anonymize-map token cannot be empty"));

	جمع_کړه_anonymized_ثبت(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(ارزښت));

	أرجع 0;
}

صحيح cmd_fast_export(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *مختاړی,
		    بنية repository *repo ناکارول_شوی)
{
	بنية rev_info revs;
	بنية commit *commit;
	حرف *export_filename = عدم,
	     *import_filename = عدم,
	     *import_filename_if_exists = عدم;
	uint32_t lastimportid;
	بنية string_list refspecs_list = STRING_LIST_INIT_NODUP;
	بنية string_list paths_د_بدل_شوی_objects = STRING_LIST_INIT_DUP;
	بنية option options[] = {
		OPT_INTEGER(0, "progress", &پرمختګ,
			    N_("show progress after <n> objects")),
		OPT_CALLBACK(0, "signed-tags", &signed_نښان_ډېرپېښ, N_("mode"),
			     N_("select handling of signed tags"),
			     parse_opt_sign_ډېرپېښ),
		OPT_CALLBACK(0, "signed-commits", &signed_commit_ډېرپېښ, N_("mode"),
			     N_("select handling of signed commits"),
			     parse_opt_sign_ډېرپېښ),
		OPT_CALLBACK(0, "tag-of-filtered-object", &نښان_د_filtered_ډېرپېښ, N_("mode"),
			     N_("select handling of tags that tag filtered objects"),
			     parse_opt_نښان_د_filtered_ډېرپېښ),
		OPT_CALLBACK(0, "reencode", &reencode_ډېرپېښ, N_("mode"),
			     N_("select handling of commit messages in an alternate encoding"),
			     parse_opt_reencode_ډېرپېښ),
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
		OPT_BOOL(0, "full-tree", &ډک_ونه,
			 N_("output full tree for each commit")),
		OPT_BOOL(0, "use-done-feature", &use_بشپړ_feature,
			     N_("use the done feature to terminate the stream")),
		OPT_BOOL(0, "no-data", &no_معلومات, N_("skip output of blob data")),
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
		OPT_BOOL(0, "mark-tags", &mark_نښانونه,
			    N_("label tags with mark ids")),

		OPT_پای_ټکی()
	};

	إذا (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, عدم);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, مختاړی);
	init_revision_sources(&revision_sources);
	revs.topo_ترتیب = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, مختاړی, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = تیاری_revisions(argc, argv, &revs, عدم);
	إذا (argc > 1)
		usage_with_options (fast_export_usage, options);

	إذا (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	إذا (refspecs_list.nr) {
		صحيح i;

		لكل (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.توکي[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	إذا (use_بشپړ_feature)
		چاپ_کړه("feature done\n");

	إذا (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	إذا (import_filename)
		import_marks(import_filename, 0);
	وإلا إذا (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = وروستی_idnum;

	إذا (import_filename && revs.prune_معلومات.nr)
		ډک_ونه = 1;

	get_نښانونه_and_duplicates(&revs.cmdline);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.سرچپه = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_معلومات = &paths_د_بدل_شوی_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	طالما ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_د_بدل_شوی_objects);
	revs.diffopt.no_free = 0;

	handle_نښانونه_and_duplicates(&extra_refs);
	handle_نښانونه_and_duplicates(&نښان_refs);
	handle_deletes();

	إذا (export_filename && lastimportid != وروستی_idnum)
		export_marks(export_filename);

	إذا (use_بشپړ_feature)
		چاپ_کړه("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	أرجع 0;
}
