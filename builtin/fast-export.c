/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن صحيح پیشرفت;
ساكن تعداد sign_نما signed_برچسب_نما = SIGN_ABORT;
ساكن تعداد sign_نما signed_commit_نما = SIGN_STRIP;
ساكن تعداد برچسب_از_filtered_نما { برچسب_FILTERING_ABORT, DROP, REWRITE } برچسب_از_filtered_نما = برچسب_FILTERING_ABORT;
ساكن تعداد reencode_نما { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_نما = REENCODE_ABORT;
ساكن صحيح fake_missing_tagger;
ساكن صحيح use_انجامشده_feature;
ساكن صحيح no_داده;
ساكن صحيح پر_درخت;
ساكن صحيح reference_excluded_commits;
ساكن صحيح show_original_ids;
ساكن صحيح mark_برچسبها;
ساكن بنية string_list extra_refs = STRING_LIST_INIT_DUP;
ساكن بنية string_list برچسب_refs = STRING_LIST_INIT_DUP;
ساكن بنية refspec refspecs;
ساكن صحيح anonymize;
ساكن بنية hashmap anonymized_seeds;
ساكن بنية revision_sources revision_sources;

ساكن صحيح parse_opt_sign_نما(ثابت بنية option *opt,
			       ثابت حرف *arg, صحيح unset)
{
	تعداد sign_نما *val = opt->مقدار;

	إذا (unset)
		أرجع 0;

	إذا (parse_sign_نما(arg, val, عدم) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		أرجع error(_("unknown %s mode: %s"), opt->long_name, arg);

	أرجع 0;
}

ساكن صحيح parse_opt_برچسب_از_filtered_نما(ثابت بنية option *opt,
					  ثابت حرف *arg, صحيح unset)
{
	تعداد برچسب_از_filtered_نما *val = opt->مقدار;

	إذا (unset || !strcmp(arg, "abort"))
		*val = برچسب_FILTERING_ABORT;
	وإلا إذا (!strcmp(arg, "drop"))
		*val = DROP;
	وإلا إذا (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	وإلا
		أرجع error(_("unknown tag-of-filtered mode: %s"), arg);
	أرجع 0;
}

ساكن صحيح parse_opt_reencode_نما(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	تعداد reencode_نما *val = opt->مقدار;

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
ساكن uint32_t آخرین_idnum;
بنية anonymized_مدخل {
	بنية hashmap_مدخل hash;
	حرف *anon;
	ثابت حرف orig[FLEX_ARRAY];
};

بنية anonymized_مدخل_کلید {
	بنية hashmap_مدخل hash;
	ثابت حرف *orig;
	اندازه_t orig_len;
};

ساكن صحيح anonymized_مدخل_cmp(ثابت فراغ *cmp_داده استفادهنشده,
				ثابت بنية hashmap_مدخل *eptr,
				ثابت بنية hashmap_مدخل *مدخل_or_کلید,
				ثابت فراغ *keydata)
{
	ثابت بنية anonymized_مدخل *a, *b;

	a = container_از(eptr, ثابت بنية anonymized_مدخل, hash);
	إذا (keydata) {
		ثابت بنية anonymized_مدخل_کلید *کلید = keydata;
		صحيح equal = !xstrncmpz(a->orig, کلید->orig, کلید->orig_len);
		أرجع !equal;
	}

	b = container_از(مدخل_or_کلید, ثابت بنية anonymized_مدخل, hash);
	أرجع strcmp(a->orig, b->orig);
}

ساكن بنية anonymized_مدخل *جمع_کن_anonymized_مدخل(بنية hashmap *map,
						     بیعلامت hash,
						     ثابت حرف *orig, اندازه_t len,
						     حرف *anon)
{
	بنية anonymized_مدخل *ret, *old;

	إذا (!map->cmpfn)
		hashmap_init(map, anonymized_مدخل_cmp, عدم, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_مدخل_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_مدخل(map, ret, hash);

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
	بنية anonymized_مدخل_کلید کلید;
	بنية anonymized_مدخل *ret;

	hashmap_مدخل_init(&کلید.hash, memhash(orig, len));
	کلید.orig = orig;
	کلید.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_مدخل(&anonymized_seeds, &کلید, hash, &کلید);

	/* ...otherwise check if we've already seen it in this context... */
	إذا (!ret)
		ret = hashmap_get_مدخل(map, &کلید, hash, &کلید);

	/* ...and finally generate a new mapping if necessary */
	إذا (!ret)
		ret = جمع_کن_anonymized_مدخل(map, کلید.hash.hash,
					   orig, len, generate());

	أرجع ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
ساكن فراغ anonymize_مسیر(بنية strbuf *بیرون, ثابت حرف *مسیر,
			   بنية hashmap *map,
			   حرف *(*generate)(فراغ))
{
	طالما (*مسیر) {
		ثابت حرف *انتها_از_component = strchrnul(مسیر, '/');
		اندازه_t len = انتها_از_component - مسیر;
		ثابت حرف *c = anonymize_str(map, generate, مسیر, len);
		strbuf_addstr(بیرون, c);
		مسیر = انتها_از_component;
		إذا (*مسیر)
			strbuf_addch(بیرون, *مسیر++);
	}
}

ساكن مضمن فراغ *mark_مقصد_ptr(uint32_t mark)
{
	أرجع (فراغ *)(uintptr_t)mark;
}

ساكن مضمن uint32_t ptr_مقصد_mark(فراغ * mark)
{
	أرجع (uint32_t)(uintptr_t)mark;
}

ساكن مضمن فراغ mark_object(بنية object *object, uint32_t mark)
{
	جمع_کن_decoration(&idnums, object, mark_مقصد_ptr(mark));
}

ساكن مضمن فراغ mark_بعدی_object(بنية object *object)
{
	mark_object(object, ++آخرین_idnum);
}

ساكن صحيح get_object_mark(بنية object *object)
{
	فراغ *decoration = lookup_decoration(&idnums, object);
	إذا (!decoration)
		أرجع 0;
	أرجع ptr_مقصد_mark(decoration);
}

ساكن بنية commit *rewrite_commit(بنية commit *p)
{
	لكل (;;) {
		إذا (p->parents && p->parents->بعدی)
			اكسر;
		إذا (p->object.flags & UNINTERESTING)
			اكسر;
		إذا (!(p->object.flags & TREESAME))
			اكسر;
		إذا (!p->parents)
			أرجع عدم;
		p = p->parents->قلم;
	}
	أرجع p;
}

ساكن فراغ show_پیشرفت(فراغ)
{
	ساكن صحيح counter = 0;
	إذا (!پیشرفت)
		أرجع;
	إذا ((++counter % پیشرفت) == 0)
		چاپ_کن("progress %d objects\n", counter);
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
ساكن حرف *anonymize_blob(بیعلامت دراز *اندازه)
{
	ساكن صحيح counter;
	بنية strbuf بیرون = STRBUF_INIT;
	strbuf_addf(&بیرون, "anonymous blob %d", counter++);
	*اندازه = بیرون.len;
	أرجع strbuf_detach(&بیرون, عدم);
}

ساكن فراغ export_blob(ثابت بنية object_id *oid)
{
	بیعلامت دراز اندازه;
	تعداد object_type type;
	حرف *buf;
	بنية object *object;
	صحيح eaten;

	إذا (no_داده)
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
		اندازه = cast_اندازه_t_مقصد_ulong(اندازه_st);
		إذا (!buf)
			die(_("could not read blob %s"), oid_مقصد_hex(oid));
		إذا (check_object_signature(the_repository, oid, buf, اندازه,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_مقصد_hex(oid));
		object = parse_object_میانگیر(the_repository, oid, type,
					     اندازه, buf, &eaten);
	}

	إذا (!object)
		die(_("could not read blob %s"), oid_مقصد_hex(oid));

	mark_بعدی_object(object);

	چاپ_کن("blob\nmark :%"PRIu32"\n", آخرین_idnum);
	إذا (show_original_ids)
		چاپ_کن("original-oid %s\n", oid_مقصد_hex(oid));
	چاپ_کن("data %"PRIuMAX"\n", (uintmax_t)اندازه);
	إذا (اندازه && fwrite(buf, اندازه, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_مقصد_hex(oid));
	چاپ_کن("\n");

	show_پیشرفت();

	object->flags |= SHOWN;
	إذا (!eaten)
		حرر(buf);
}

ساكن صحيح عمق_اولی(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت بنية diff_filepair *a = *((ثابت بنية diff_filepair **)a_);
	ثابت بنية diff_filepair *b = *((ثابت بنية diff_filepair **)b_);
	ثابت حرف *name_a, *name_b;
	صحيح len_a, len_b, len;
	صحيح cmp;

	name_a = a->one ? a->one->مسیر : a->two->مسیر;
	name_b = b->one ? b->one->مسیر : b->two->مسیر;

	len_a = طول_رشته(name_a);
	len_b = طول_رشته(name_b);
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
	أرجع (a->وضعیت == 'R') - (b->وضعیت == 'R');
}

ساكن فراغ print_مسیر_1(ثابت حرف *مسیر)
{
	صحيح need_quote = quote_c_style(مسیر, عدم, عدم, 0);
	إذا (need_quote)
		quote_c_style(مسیر, عدم, stdout, 0);
	وإلا إذا (strchr(مسیر, ' '))
		چاپ_کن("\"%s\"", مسیر);
	وإلا
		چاپ_کن("%s", مسیر);
}

ساكن حرف *anonymize_مسیر_component(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf بیرون = STRBUF_INIT;
	strbuf_addf(&بیرون, "path%d", counter++);
	أرجع strbuf_detach(&بیرون, عدم);
}

ساكن فراغ print_مسیر(ثابت حرف *مسیر)
{
	إذا (!anonymize)
		print_مسیر_1(مسیر);
	وإلا {
		ساكن بنية hashmap paths;
		ساكن بنية strbuf anon = STRBUF_INIT;

		anonymize_مسیر(&anon, مسیر, &paths, anonymize_مسیر_component);
		print_مسیر_1(anon.buf);
		strbuf_reset(&anon);
	}
}

ساكن حرف *generate_fake_oid(فراغ)
{
	ساكن uint32_t counter = 1; /* avoid null oid */
	ثابت بیعلامت hashsz = the_hash_algo->rawsz;
	بنية object_id oid;
	حرف *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	أرجع oid_مقصد_hex_r(hex, &oid);
}

ساكن ثابت حرف *anonymize_oid(ثابت حرف *oid_hex)
{
	ساكن بنية hashmap objs;
	اندازه_t len = طول_رشته(oid_hex);
	أرجع anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

ساكن فراغ show_filemodify(بنية diff_صف_struct *q,
			    بنية diff_options *options استفادهنشده, فراغ *داده)
{
	صحيح i;
	بنية string_list *تغییریافته = داده;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->صف, q->nr, عمق_اولی);

	لكل (i = 0; i < q->nr; i++) {
		بنية diff_filespec *ospec = q->صف[i]->one;
		بنية diff_filespec *spec = q->صف[i]->two;

		بدل (q->صف[i]->وضعیت) {
		حالة DIFF_وضعیت_DELETED:
			چاپ_کن("D ");
			print_مسیر(spec->مسیر);
			string_list_درج(تغییریافته, spec->مسیر);
			حرف_چاپ_کن('\n');
			اكسر;

		حالة DIFF_وضعیت_COPIED:
		حالة DIFF_وضعیت_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			إذا (!string_list_has_string(تغییریافته, ospec->مسیر)) {
				چاپ_کن("%c ", q->صف[i]->وضعیت);
				print_مسیر(ospec->مسیر);
				حرف_چاپ_کن(' ');
				print_مسیر(spec->مسیر);
				string_list_درج(تغییریافته, spec->مسیر);
				حرف_چاپ_کن('\n');

				إذا (oideq(&ospec->oid, &spec->oid) &&
				    ospec->نما == spec->نما)
					اكسر;
			}
			/* fallthrough */

		حالة DIFF_وضعیت_TYPE_تغییریافته:
		حالة DIFF_وضعیت_MODIFIED:
		حالة DIFF_وضعیت_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			إذا (no_داده || S_ISGITLINK(spec->نما))
				چاپ_کن("M %06o %s ", spec->نما,
				       anonymize ?
				       anonymize_oid(oid_مقصد_hex(&spec->oid)) :
				       oid_مقصد_hex(&spec->oid));
			وإلا {
				بنية object *object = lookup_object(the_repository,
								      &spec->oid);
				چاپ_کن("M %06o :%d ", spec->نما,
				       get_object_mark(object));
			}
			print_مسیر(spec->مسیر);
			string_list_درج(تغییریافته, spec->مسیر);
			حرف_چاپ_کن('\n');
			اكسر;

		افتراضي:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->صف[i]->وضعیت,
			    ospec->مسیر ? ospec->مسیر : _("none"),
			    spec->مسیر ? spec->مسیر : _("none"));
		}
	}
}

ساكن حرف *anonymize_ref_component(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf بیرون = STRBUF_INIT;
	strbuf_addf(&بیرون, "ref%d", counter++);
	أرجع strbuf_detach(&بیرون, عدم);
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
		إذا (skip_پیشوند(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			اكسر;
		}
	}

	anonymize_مسیر(&anon, refname, &refs, anonymize_ref_component);
	أرجع anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
ساكن حرف *anonymize_commit_پیام(فراغ)
{
	ساكن صحيح counter;
	أرجع xstrfmt("subject %d\n\nbody\n", counter++);
}

ساكن حرف *anonymize_ident(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf بیرون = STRBUF_INIT;
	strbuf_addf(&بیرون, "User %d <user%d@example.com>", counter, counter);
	counter++;
	أرجع strbuf_detach(&بیرون, عدم);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
ساكن فراغ anonymize_ident_سطر(ثابت حرف **beg, ثابت حرف **انتها)
{
	ساكن بنية hashmap idents;
	ساكن بنية strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	ساكن بیعلامت which_میانگیر;

	بنية strbuf *بیرون;
	بنية ident_تقسیم تقسیم;
	ثابت حرف *انتها_از_سرآیند;

	بیرون = &buffers[which_میانگیر++];
	which_میانگیر %= ARRAY_اندازه(buffers);
	strbuf_reset(بیرون);

	/* skip "committer", "author", "tagger", etc */
	انتها_از_سرآیند = strchr(*beg, ' ');
	إذا (!انتها_از_سرآیند)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (صحيح)(*انتها - *beg), *beg);
	انتها_از_سرآیند++;
	strbuf_جمع_کن(بیرون, *beg, انتها_از_سرآیند - *beg);

	إذا (!تقسیم_ident_سطر(&تقسیم, انتها_از_سرآیند, *انتها - انتها_از_سرآیند) &&
	    تقسیم.date_begin) {
		ثابت حرف *ident;
		اندازه_t len;

		len = تقسیم.mail_انتها - تقسیم.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      تقسیم.name_begin, len);
		strbuf_addstr(بیرون, ident);
		strbuf_addch(بیرون, ' ');
		strbuf_جمع_کن(بیرون, تقسیم.date_begin, تقسیم.tz_انتها - تقسیم.date_begin);
	} وإلا {
		strbuf_addstr(بیرون, "Malformed Ident <malformed@example.com> 0 -0000");
	}

	*beg = بیرون->buf;
	*انتها = بیرون->buf + بیرون->len;
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
ساكن ثابت حرف *پیدا_کن_commit_multiline_سرآیند(ثابت حرف *پیام,
						ثابت حرف *کلید,
						ثابت حرف **انتها)
{
	بنية strbuf val = STRBUF_INIT;
	ثابت حرف *bol, *eol;
	اندازه_t len;

	bol = پیدا_کن_commit_سرآیند(پیام, کلید, &len);
	إذا (!bol)
		أرجع عدم;
	eol = bol + len;
	strbuf_جمع_کن(&val, bol, len);

	طالما (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_جمع_کن(&val, bol, eol - bol);
	}

	*انتها = eol;
	أرجع strbuf_detach(&val, عدم);
}

ساكن فراغ print_signature(ثابت حرف *signature, ثابت حرف *object_hash)
{
	إذا (!signature)
		أرجع;

	چاپ_کن("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (بیعلامت)طول_رشته(signature),
	       signature);
}

ساكن ثابت حرف *append_signatures_for_سرآیند(بنية string_list *signatures,
						ثابت حرف *pos,
						ثابت حرف *سرآیند,
						ثابت حرف *object_hash)
{
	ثابت حرف *signature;
	ثابت حرف *شروع = pos;
	ثابت حرف *انتها = pos;

	طالما ((signature = پیدا_کن_commit_multiline_سرآیند(شروع + 1,
							 سرآیند,
							 &انتها))) {
		string_list_append(signatures, signature)->util = (فراغ *)object_hash;
		حرر((حرف *)signature);
		شروع = انتها;
	}

	أرجع انتها;
}

ساكن فراغ handle_commit(بنية commit *commit, بنية rev_info *rev,
			  بنية string_list *paths_از_تغییریافته_objects)
{
	صحيح saved_output_format = rev->diffopt.output_format;
	ثابت حرف *commit_میانگیر, *commit_میانگیر_cursor;
	ثابت حرف *author, *author_انتها, *committer, *committer_انتها;
	ثابت حرف *encoding = عدم;
	اندازه_t encoding_len;
	بنية string_list signatures = STRING_LIST_INIT_DUP;
	ثابت حرف *پیام_2;
	حرف *reencoded = عدم;
	بنية commit_list *p;
	ثابت حرف *refname;
	صحيح i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_میانگیر_cursor = commit_میانگیر = repo_get_commit_میانگیر(the_repository, commit, عدم);

	author = جستجوی_رشته(commit_میانگیر_cursor, "\nauthor ");
	إذا (!author)
		die(_("could not find author in commit %s"),
		    oid_مقصد_hex(&commit->object.oid));
	author++;
	commit_میانگیر_cursor = author_انتها = strchrnul(author, '\n');

	committer = جستجوی_رشته(commit_میانگیر_cursor, "\ncommitter ");
	إذا (!committer)
		die(_("could not find committer in commit %s"),
		    oid_مقصد_hex(&commit->object.oid));
	committer++;
	commit_میانگیر_cursor = committer_انتها = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	إذا (*commit_میانگیر_cursor == '\n') {
		encoding = پیدا_کن_commit_سرآیند(commit_میانگیر_cursor + 1, "encoding", &encoding_len);
		إذا (encoding)
			commit_میانگیر_cursor = encoding + encoding_len;
	}

	إذا (*commit_میانگیر_cursor == '\n') {
		ثابت حرف *after_sha1 = append_signatures_for_سرآیند(&signatures, commit_میانگیر_cursor,
								      "gpgsig", "sha1");
		ثابت حرف *after_sha256 = append_signatures_for_سرآیند(&signatures, commit_میانگیر_cursor,
									"gpgsig-sha256", "sha256");
		commit_میانگیر_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	پیام_2 = جستجوی_رشته(commit_میانگیر_cursor, "\n\n");
	إذا (پیام_2)
		پیام_2 += 2;

	إذا (commit->parents &&
	    (get_object_mark(&commit->parents->قلم->object) != 0 ||
	     reference_excluded_commits) &&
	    !پر_درخت) {
		parse_commit_or_die(commit->parents->قلم);
		diff_درخت_oid(get_commit_درخت_oid(commit->parents->قلم),
			      get_commit_درخت_oid(commit), "", &rev->diffopt);
	}
	وإلا
		diff_ریشه_درخت_oid(get_commit_درخت_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	لكل (i = 0; i < diff_queued_diff.nr; i++)
		إذا (!S_ISGITLINK(diff_queued_diff.صف[i]->two->نما))
			export_blob(&diff_queued_diff.صف[i]->two->oid);

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
		anonymize_ident_سطر(&committer, &committer_انتها);
		anonymize_ident_سطر(&author, &author_انتها);
	}

	mark_بعدی_object(&commit->object);
	إذا (anonymize) {
		reencoded = anonymize_commit_پیام();
	} وإلا إذا (encoding) {
		حرف *buf;
		بدل (reencode_نما) {
		حالة REENCODE_YES:
			buf = xstrfmt("%.*s", (صحيح)encoding_len, encoding);
			reencoded = reencode_string(پیام_2, "UTF-8", buf);
			حرر(buf);
			اكسر;
		حالة REENCODE_NO:
			اكسر;
		حالة REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (صحيح)encoding_len, encoding,
			    oid_مقصد_hex(&commit->object.oid));
		}
	}
	إذا (!commit->parents)
		چاپ_کن("reset %s\n", refname);
	چاپ_کن("commit %s\nmark :%"PRIu32"\n", refname, آخرین_idnum);
	إذا (show_original_ids)
		چاپ_کن("original-oid %s\n", oid_مقصد_hex(&commit->object.oid));
	چاپ_کن("%.*s\n%.*s\n",
	       (صحيح)(author_انتها - author), author,
	       (صحيح)(committer_انتها - committer), committer);
	إذا (signatures.nr) {
		بدل (signed_commit_نما) {
		/* Exporting modes */
		حالة SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_مقصد_hex(&commit->object.oid));
			/* fallthru */
		حالة SIGN_VERBATIM:
			لكل (اندازه_t i = 0; i < signatures.nr; i++) {
				بنية string_list_قلم *قلم = &signatures.اقلام[i];
				print_signature(قلم->string, قلم->util);
			}
			اكسر;

		/* Stripping modes */
		حالة SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_مقصد_hex(&commit->object.oid));
			/* fallthru */
		حالة SIGN_STRIP:
			اكسر;

		/* Aborting modes */
		حالة SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_مقصد_hex(&commit->object.oid));
		افتراضي:
			BUG("invalid signed_commit_mode value %d", signed_commit_نما);
		}
		string_list_clear(&signatures, 0);
	}
	إذا (!reencoded && encoding)
		چاپ_کن("encoding %.*s\n", (صحيح)encoding_len, encoding);
	چاپ_کن("data %u\n%s",
	       (بیعلامت)(reencoded
			  ? طول_رشته(reencoded) : پیام_2
			  ? طول_رشته(پیام_2) : 0),
	       reencoded ? reencoded : پیام_2 ? پیام_2 : "");
	حرر(reencoded);
	repo_unuse_commit_میانگیر(the_repository, commit, commit_میانگیر);

	لكل (i = 0, p = commit->parents; p; p = p->بعدی) {
		بنية object *obj = &p->قلم->object;
		صحيح mark = get_object_mark(obj);

		إذا (!mark && !reference_excluded_commits)
			استمر;
		إذا (i == 0)
			چاپ_کن("from ");
		وإلا
			چاپ_کن("merge ");
		إذا (mark)
			چاپ_کن(":%d\n", mark);
		وإلا
			چاپ_کن("%s\n",
			       anonymize ?
			       anonymize_oid(oid_مقصد_hex(&obj->oid)) :
			       oid_مقصد_hex(&obj->oid));
		i++;
	}

	إذا (پر_درخت)
		چاپ_کن("deleteall\n");
	log_درخت_diff_flush(rev);
	string_list_clear(paths_از_تغییریافته_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	چاپ_کن("\n");

	show_پیشرفت();
}

ساكن حرف *anonymize_برچسب(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf بیرون = STRBUF_INIT;
	strbuf_addf(&بیرون, "tag message %d", counter++);
	أرجع strbuf_detach(&بیرون, عدم);
}


ساكن فراغ handle_برچسب(ثابت حرف *name, بنية برچسب *برچسب)
{
	اندازه_t اندازه;
	تعداد object_type type;
	حرف *buf;
	ثابت حرف *tagger, *tagger_انتها, *پیام_2;
	اندازه_t پیام_اندازه = 0;
	بنية object *tagged;
	صحيح tagged_mark;
	بنية commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = برچسب->tagged;
	طالما (tagged->type == OBJ_برچسب) {
		tagged = ((بنية برچسب *)tagged)->tagged;
	}
	إذا (tagged->type == OBJ_درخت) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_مقصد_hex(&برچسب->object.oid));
		أرجع;
	}

	buf = odb_read_object(the_repository->objects, &برچسب->object.oid,
			      &type, &اندازه);
	إذا (!buf)
		die(_("could not read tag %s"), oid_مقصد_hex(&برچسب->object.oid));
	پیام_2 = memmem(buf, اندازه, "\n\n", 2);
	إذا (پیام_2) {
		پیام_2 += 2;
		پیام_اندازه = طول_رشته(پیام_2);
	}
	tagger = memmem(buf, پیام_2 ? پیام_2 - buf : اندازه, "\ntagger ", 8);
	إذا (!tagger) {
		إذا (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		وإلا
			tagger = "";
		tagger_انتها = tagger + طول_رشته(tagger);
	} وإلا {
		tagger++;
		tagger_انتها = strchrnul(tagger, '\n');
		إذا (anonymize)
			anonymize_ident_سطر(&tagger, &tagger_انتها);
	}

	إذا (anonymize) {
		name = anonymize_refname(name);
		إذا (پیام_2) {
			ساكن بنية hashmap برچسبها;
			پیام_2 = anonymize_str(&برچسبها, anonymize_برچسب,
						پیام_2, پیام_اندازه);
			پیام_اندازه = طول_رشته(پیام_2);
		}
	}

	/* handle signed tags */
	إذا (پیام_2) {
		اندازه_t sig_جابه_جایی = parse_signed_میانگیر(پیام_2, پیام_اندازه);
		إذا (sig_جابه_جایی < پیام_اندازه)
			بدل (signed_برچسب_نما) {
			/* Exporting modes */
			حالة SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_مقصد_hex(&برچسب->object.oid));
				/* fallthru */
			حالة SIGN_VERBATIM:
				اكسر;

			/* Stripping modes */
			حالة SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_مقصد_hex(&برچسب->object.oid));
				/* fallthru */
			حالة SIGN_STRIP:
				پیام_اندازه = sig_جابه_جایی;
				اكسر;

			/* Aborting modes */
			حالة SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_مقصد_hex(&برچسب->object.oid));
			افتراضي:
				BUG("invalid signed_commit_mode value %d", signed_commit_نما);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = برچسب->tagged;
	tagged_mark = get_object_mark(tagged);
	إذا (!tagged_mark) {
		بدل (برچسب_از_filtered_نما) {
		حالة برچسب_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_مقصد_hex(&برچسب->object.oid));
		حالة DROP:
			/* Ignore this tag altogether */
			حرر(buf);
			أرجع;
		حالة REWRITE:
			إذا (tagged->type == OBJ_برچسب && !mark_برچسبها) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} وإلا إذا (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((بنية commit *)tagged);
				إذا (!p) {
					چاپ_کن("reset %s\nfrom %s\n\n",
					       name, oid_مقصد_hex(null_oid(the_hash_algo)));
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

	إذا (tagged->type == OBJ_برچسب) {
		چاپ_کن("reset %s\nfrom %s\n\n",
		       name, oid_مقصد_hex(null_oid(the_hash_algo)));
	}
	skip_پیشوند(name, "refs/tags/", &name);
	چاپ_کن("tag %s\n", name);
	إذا (mark_برچسبها) {
		mark_بعدی_object(&برچسب->object);
		چاپ_کن("mark :%"PRIu32"\n", آخرین_idnum);
	}
	إذا (tagged_mark)
		چاپ_کن("from :%d\n", tagged_mark);
	وإلا
		چاپ_کن("from %s\n", oid_مقصد_hex(&tagged->oid));

	إذا (show_original_ids)
		چاپ_کن("original-oid %s\n", oid_مقصد_hex(&برچسب->object.oid));
	چاپ_کن("%.*s%sdata %d\n%.*s\n",
	       (صحيح)(tagger_انتها - tagger), tagger,
	       tagger == tagger_انتها ? "" : "\n",
	       (صحيح)پیام_اندازه, (صحيح)پیام_اندازه, پیام_2 ? پیام_2 : "");
	حرر(buf);
}

ساكن بنية commit *get_commit(بنية rev_cmdline_مدخل *e, ثابت حرف *پر_name)
{
	بدل (e->قلم->type) {
	حالة OBJ_COMMIT:
		أرجع (بنية commit *)e->قلم;
	حالة OBJ_برچسب: {
		بنية برچسب *برچسب = (بنية برچسب *)e->قلم;

		/* handle nested tags */
		طالما (برچسب && برچسب->object.type == OBJ_برچسب) {
			parse_object(the_repository, &برچسب->object.oid);
			string_list_append(&برچسب_refs, پر_name)->util = برچسب;
			برچسب = (بنية برچسب *)برچسب->tagged;
		}
		إذا (!برچسب)
			die(_("tag %s points nowhere?"), e->name);
		أرجع (بنية commit *)برچسب;
	}
	افتراضي:
		أرجع عدم;
	}
}

ساكن فراغ get_برچسبها_and_duplicates(بنية rev_cmdline_info *info)
{
	صحيح i;

	لكل (i = 0; i < info->nr; i++) {
		بنية rev_cmdline_مدخل *e = info->rev + i;
		بنية object_id oid;
		بنية commit *commit;
		حرف *پر_name = عدم;

		إذا (e->flags & UNINTERESTING)
			استمر;

		إذا (repo_dwim_ref(the_repository, e->name, طول_رشته(e->name),
				  &oid, &پر_name, 0) != 1) {
			حرر(پر_name);
			استمر;
		}

		إذا (refspecs.nr) {
			حرف *خاص;
			خاص = apply_refspecs(&refspecs, پر_name);
			إذا (خاص) {
				حرر(پر_name);
				پر_name = خاص;
			}
		}

		commit = get_commit(e, پر_name);
		إذا (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->قلم->type));
			حرر(پر_name);
			استمر;
		}

		بدل (commit->object.type) {
		حالة OBJ_COMMIT:
			اكسر;
		حالة OBJ_BLOB:
			export_blob(&commit->object.oid);
			حرر(پر_name);
			استمر;
		افتراضي: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			حرر(پر_name);
			استمر;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		إذا (e->قلم->type != OBJ_برچسب)
			string_list_append(&extra_refs, پر_name)->util = commit;

		إذا (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = پر_name;
		وإلا
			حرر(پر_name);
	}

	string_list_مرتب_سازی_u(&extra_refs, 0);
}

ساكن فراغ handle_برچسبها_and_duplicates(بنية string_list *extras)
{
	بنية commit *commit;
	صحيح i;

	لكل (i = extras->nr - 1; i >= 0; i--) {
		ثابت حرف *name = extras->اقلام[i].string;
		بنية object *object = extras->اقلام[i].util;
		صحيح mark;

		بدل (object->type) {
		حالة OBJ_برچسب:
			handle_برچسب(name, (بنية برچسب *)object);
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
				چاپ_کن("reset %s\nfrom %s\n\n",
				       name, oid_مقصد_hex(null_oid(the_hash_algo)));
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
					چاپ_کن("reset %s\nfrom %s\n\n",
					       name, oid_مقصد_hex(null_oid(the_hash_algo)));
					استمر;
				}
				/* set ref to commit using oid, not mark */
				چاپ_کن("reset %s\nfrom %s\n\n", name,
				       oid_مقصد_hex(&commit->object.oid));
				استمر;
			}

			چاپ_کن("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_پیشرفت();
			اكسر;
		}
	}
}

ساكن فراغ export_marks(حرف *file)
{
	بیعلامت صحيح i;
	uint32_t mark;
	بنية decoration_مدخل *deco = idnums.entries;
	FILE *f;
	صحيح e = 0;

	f = fopen_for_writing(file);
	إذا (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	لكل (i = 0; i < idnums.اندازه; i++) {
		إذا (deco->پایه && deco->پایه->type == 1) {
			mark = ptr_مقصد_mark(deco->decoration);
			إذا (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_مقصد_hex(&deco->پایه->oid)) < 0) {
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

ساكن فراغ import_marks(حرف *درون_داد_file, صحيح check_exists)
{
	حرف سطر[512];
	FILE *f;
	بنية stat sb;

	إذا (check_exists && stat(درون_داد_file, &sb))
		أرجع;

	f = xfopen(درون_داد_file, "r");
	طالما (fgets(سطر, حجم(سطر), f)) {
		uint32_t mark;
		حرف *سطر_انتها, *mark_انتها;
		بنية object_id oid;
		بنية object *object;
		بنية commit *commit;
		تعداد object_type type;

		سطر_انتها = strchr(سطر, '\n');
		إذا (سطر[0] != ':' || !سطر_انتها)
			die(_("corrupt mark line: %s"), سطر);
		*سطر_انتها = '\0';

		mark = strtoumax(سطر + 1, &mark_انتها, 10);
		إذا (!mark || mark_انتها == سطر + 1
			|| *mark_انتها != ' ' || get_oid_hex(mark_انتها + 1, &oid))
			die(_("corrupt mark line: %s"), سطر);

		إذا (آخرین_idnum < mark)
			آخرین_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, عدم);
		إذا (type < 0)
			die(_("object not found: %s"), oid_مقصد_hex(&oid));

		إذا (type != OBJ_COMMIT)
			/* only commits */
			استمر;

		commit = lookup_commit(the_repository, &oid);
		إذا (!commit)
			die(_("not a commit? can't happen: %s"), oid_مقصد_hex(&oid));

		object = &commit->object;

		إذا (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_مقصد_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

ساكن فراغ handle_deletes(فراغ)
{
	صحيح i;
	لكل (i = 0; i < refspecs.nr; i++) {
		بنية refspec_قلم *refspec = &refspecs.اقلام[i];
		إذا (*refspec->src)
			استمر;

		چاپ_کن("reset %s\nfrom %s\n\n",
				refspec->dst, oid_مقصد_hex(null_oid(the_hash_algo)));
	}
}

ساكن صحيح parse_opt_anonymize_map(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	بنية hashmap *map = opt->مقدار;
	ثابت حرف *delim, *مقدار;
	اندازه_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	إذا (delim) {
		keylen = delim - arg;
		مقدار = delim + 1;
	} وإلا {
		keylen = طول_رشته(arg);
		مقدار = arg;
	}

	إذا (!keylen || !*مقدار)
		أرجع error(_("--anonymize-map token cannot be empty"));

	جمع_کن_anonymized_مدخل(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(مقدار));

	أرجع 0;
}

صحيح cmd_fast_export(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *پیشوند,
		    بنية repository *repo استفادهنشده)
{
	بنية rev_info revs;
	بنية commit *commit;
	حرف *export_filename = عدم,
	     *import_filename = عدم,
	     *import_filename_if_exists = عدم;
	uint32_t lastimportid;
	بنية string_list refspecs_list = STRING_LIST_INIT_NODUP;
	بنية string_list paths_از_تغییریافته_objects = STRING_LIST_INIT_DUP;
	بنية option options[] = {
		OPT_INTEGER(0, "progress", &پیشرفت,
			    N_("show progress after <n> objects")),
		OPT_CALLBACK(0, "signed-tags", &signed_برچسب_نما, N_("mode"),
			     N_("select handling of signed tags"),
			     parse_opt_sign_نما),
		OPT_CALLBACK(0, "signed-commits", &signed_commit_نما, N_("mode"),
			     N_("select handling of signed commits"),
			     parse_opt_sign_نما),
		OPT_CALLBACK(0, "tag-of-filtered-object", &برچسب_از_filtered_نما, N_("mode"),
			     N_("select handling of tags that tag filtered objects"),
			     parse_opt_برچسب_از_filtered_نما),
		OPT_CALLBACK(0, "reencode", &reencode_نما, N_("mode"),
			     N_("select handling of commit messages in an alternate encoding"),
			     parse_opt_reencode_نما),
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
		OPT_BOOL(0, "full-tree", &پر_درخت,
			 N_("output full tree for each commit")),
		OPT_BOOL(0, "use-done-feature", &use_انجامشده_feature,
			     N_("use the done feature to terminate the stream")),
		OPT_BOOL(0, "no-data", &no_داده, N_("skip output of blob data")),
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
		OPT_BOOL(0, "mark-tags", &mark_برچسبها,
			    N_("label tags with mark ids")),

		OPT_انتها()
	};

	إذا (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, عدم);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, پیشوند);
	init_revision_sources(&revision_sources);
	revs.topo_ترتیب = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, پیشوند, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = راهاندازی_revisions(argc, argv, &revs, عدم);
	إذا (argc > 1)
		usage_with_options (fast_export_usage, options);

	إذا (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	إذا (refspecs_list.nr) {
		صحيح i;

		لكل (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.اقلام[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	إذا (use_انجامشده_feature)
		چاپ_کن("feature done\n");

	إذا (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	إذا (import_filename)
		import_marks(import_filename, 0);
	وإلا إذا (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = آخرین_idnum;

	إذا (import_filename && revs.prune_داده.nr)
		پر_درخت = 1;

	get_برچسبها_and_duplicates(&revs.cmdline);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.معکوس = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_داده = &paths_از_تغییریافته_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	طالما ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_از_تغییریافته_objects);
	revs.diffopt.no_free = 0;

	handle_برچسبها_and_duplicates(&extra_refs);
	handle_برچسبها_and_duplicates(&برچسب_refs);
	handle_deletes();

	إذا (export_filename && lastimportid != آخرین_idnum)
		export_marks(export_filename);

	إذا (use_انجامشده_feature)
		چاپ_کن("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	أرجع 0;
}
