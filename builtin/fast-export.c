/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن صحيح ترقی;
ساكن تعداد sign_منوال signed_ٹیگ_منوال = SIGN_ABORT;
ساكن تعداد sign_منوال signed_commit_منوال = SIGN_STRIP;
ساكن تعداد ٹیگ_دا_filtered_منوال { ٹیگ_FILTERING_ABORT, DROP, REWRITE } ٹیگ_دا_filtered_منوال = ٹیگ_FILTERING_ABORT;
ساكن تعداد reencode_منوال { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_منوال = REENCODE_ABORT;
ساكن صحيح fake_missing_tagger;
ساكن صحيح use_پورا_feature;
ساكن صحيح no_انکڑے;
ساكن صحيح بھریا_درخت;
ساكن صحيح reference_excluded_commits;
ساكن صحيح show_original_ids;
ساكن صحيح mark_ٹیگ_سوچی;
ساكن بنية string_list extra_refs = STRING_LIST_INIT_DUP;
ساكن بنية string_list ٹیگ_refs = STRING_LIST_INIT_DUP;
ساكن بنية refspec refspecs;
ساكن صحيح anonymize;
ساكن بنية hashmap anonymized_seeds;
ساكن بنية revision_sources revision_sources;

ساكن صحيح parse_opt_sign_منوال(ثابت بنية option *opt,
			       ثابت حرف *arg, صحيح unset)
{
	تعداد sign_منوال *val = opt->قدر;

	إذا (unset)
		أرجع 0;

	إذا (parse_sign_منوال(arg, val, عدم) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		أرجع error(_("unknown %s mode: %s"), opt->long_name, arg);

	أرجع 0;
}

ساكن صحيح parse_opt_ٹیگ_دا_filtered_منوال(ثابت بنية option *opt,
					  ثابت حرف *arg, صحيح unset)
{
	تعداد ٹیگ_دا_filtered_منوال *val = opt->قدر;

	إذا (unset || !strcmp(arg, "abort"))
		*val = ٹیگ_FILTERING_ABORT;
	وإلا إذا (!strcmp(arg, "drop"))
		*val = DROP;
	وإلا إذا (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	وإلا
		أرجع error(_("unknown tag-of-filtered mode: %s"), arg);
	أرجع 0;
}

ساكن صحيح parse_opt_reencode_منوال(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	تعداد reencode_منوال *val = opt->قدر;

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
ساكن uint32_t آخری_idnum;
بنية anonymized_اندراج {
	بنية hashmap_اندراج hash;
	حرف *anon;
	ثابت حرف orig[FLEX_ARRAY];
};

بنية anonymized_اندراج_کلید {
	بنية hashmap_اندراج hash;
	ثابت حرف *orig;
	ناپ_t orig_len;
};

ساكن صحيح anonymized_اندراج_cmp(ثابت فراغ *cmp_انکڑے بن_ورتیا,
				ثابت بنية hashmap_اندراج *eptr,
				ثابت بنية hashmap_اندراج *اندراج_or_کلید,
				ثابت فراغ *keydata)
{
	ثابت بنية anonymized_اندراج *a, *b;

	a = container_دا(eptr, ثابت بنية anonymized_اندراج, hash);
	إذا (keydata) {
		ثابت بنية anonymized_اندراج_کلید *کلید = keydata;
		صحيح equal = !xstrncmpz(a->orig, کلید->orig, کلید->orig_len);
		أرجع !equal;
	}

	b = container_دا(اندراج_or_کلید, ثابت بنية anonymized_اندراج, hash);
	أرجع strcmp(a->orig, b->orig);
}

ساكن بنية anonymized_اندراج *جوڑو_anonymized_اندراج(بنية hashmap *map,
						     بنانشانی hash,
						     ثابت حرف *orig, ناپ_t len,
						     حرف *anon)
{
	بنية anonymized_اندراج *ret, *old;

	إذا (!map->cmpfn)
		hashmap_init(map, anonymized_اندراج_cmp, عدم, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_اندراج_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_اندراج(map, ret, hash);

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
				 ثابت حرف *orig, ناپ_t len)
{
	بنية anonymized_اندراج_کلید کلید;
	بنية anonymized_اندراج *ret;

	hashmap_اندراج_init(&کلید.hash, memhash(orig, len));
	کلید.orig = orig;
	کلید.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_اندراج(&anonymized_seeds, &کلید, hash, &کلید);

	/* ...otherwise check if we've already seen it in this context... */
	إذا (!ret)
		ret = hashmap_get_اندراج(map, &کلید, hash, &کلید);

	/* ...and finally generate a new mapping if necessary */
	إذا (!ret)
		ret = جوڑو_anonymized_اندراج(map, کلید.hash.hash,
					   orig, len, generate());

	أرجع ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
ساكن فراغ anonymize_رستہ(بنية strbuf *باہرلا, ثابت حرف *رستہ,
			   بنية hashmap *map,
			   حرف *(*generate)(فراغ))
{
	طالما (*رستہ) {
		ثابت حرف *سرا_دا_component = strchrnul(رستہ, '/');
		ناپ_t len = سرا_دا_component - رستہ;
		ثابت حرف *c = anonymize_str(map, generate, رستہ, len);
		strbuf_addstr(باہرلا, c);
		رستہ = سرا_دا_component;
		إذا (*رستہ)
			strbuf_addch(باہرلا, *رستہ++);
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
	جوڑو_decoration(&idnums, object, mark_منزل_ptr(mark));
}

ساكن مضمن فراغ mark_اگلا_object(بنية object *object)
{
	mark_object(object, ++آخری_idnum);
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
		إذا (p->parents && p->parents->اگلا)
			اكسر;
		إذا (p->object.flags & UNINTERESTING)
			اكسر;
		إذا (!(p->object.flags & TREESAME))
			اكسر;
		إذا (!p->parents)
			أرجع عدم;
		p = p->parents->شے;
	}
	أرجع p;
}

ساكن فراغ show_ترقی(فراغ)
{
	ساكن صحيح counter = 0;
	إذا (!ترقی)
		أرجع;
	إذا ((++counter % ترقی) == 0)
		لکھو("progress %d objects\n", counter);
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
ساكن حرف *anonymize_blob(بنانشانی لما *ناپ)
{
	ساكن صحيح counter;
	بنية strbuf باہرلا = STRBUF_INIT;
	strbuf_addf(&باہرلا, "anonymous blob %d", counter++);
	*ناپ = باہرلا.len;
	أرجع strbuf_detach(&باہرلا, عدم);
}

ساكن فراغ export_blob(ثابت بنية object_id *oid)
{
	بنانشانی لما ناپ;
	تعداد object_type type;
	حرف *buf;
	بنية object *object;
	صحيح eaten;

	إذا (no_انکڑے)
		أرجع;

	إذا (is_null_oid(oid))
		أرجع;

	object = lookup_object(the_repository, oid);
	إذا (object && object->flags & SHOWN)
		أرجع;

	إذا (anonymize) {
		buf = anonymize_blob(&ناپ);
		object = (بنية object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} وإلا {
		ناپ_t ناپ_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &ناپ_st);
		ناپ = cast_ناپ_t_منزل_ulong(ناپ_st);
		إذا (!buf)
			die(_("could not read blob %s"), oid_منزل_hex(oid));
		إذا (check_object_signature(the_repository, oid, buf, ناپ,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_منزل_hex(oid));
		object = parse_object_بفر(the_repository, oid, type,
					     ناپ, buf, &eaten);
	}

	إذا (!object)
		die(_("could not read blob %s"), oid_منزل_hex(oid));

	mark_اگلا_object(object);

	لکھو("blob\nmark :%"PRIu32"\n", آخری_idnum);
	إذا (show_original_ids)
		لکھو("original-oid %s\n", oid_منزل_hex(oid));
	لکھو("data %"PRIuMAX"\n", (uintmax_t)ناپ);
	إذا (ناپ && fwrite(buf, ناپ, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_منزل_hex(oid));
	لکھو("\n");

	show_ترقی();

	object->flags |= SHOWN;
	إذا (!eaten)
		حرر(buf);
}

ساكن صحيح ڈونگھائی_پہلا(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت بنية diff_filepair *a = *((ثابت بنية diff_filepair **)a_);
	ثابت بنية diff_filepair *b = *((ثابت بنية diff_filepair **)b_);
	ثابت حرف *name_a, *name_b;
	صحيح len_a, len_b, len;
	صحيح cmp;

	name_a = a->one ? a->one->رستہ : a->two->رستہ;
	name_b = b->one ? b->one->رستہ : b->two->رستہ;

	len_a = لڑی_لمبائی(name_a);
	len_b = لڑی_لمبائی(name_b);
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

ساكن فراغ print_رستہ_1(ثابت حرف *رستہ)
{
	صحيح need_quote = quote_c_style(رستہ, عدم, عدم, 0);
	إذا (need_quote)
		quote_c_style(رستہ, عدم, stdout, 0);
	وإلا إذا (strchr(رستہ, ' '))
		لکھو("\"%s\"", رستہ);
	وإلا
		لکھو("%s", رستہ);
}

ساكن حرف *anonymize_رستہ_component(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf باہرلا = STRBUF_INIT;
	strbuf_addf(&باہرلا, "path%d", counter++);
	أرجع strbuf_detach(&باہرلا, عدم);
}

ساكن فراغ print_رستہ(ثابت حرف *رستہ)
{
	إذا (!anonymize)
		print_رستہ_1(رستہ);
	وإلا {
		ساكن بنية hashmap paths;
		ساكن بنية strbuf anon = STRBUF_INIT;

		anonymize_رستہ(&anon, رستہ, &paths, anonymize_رستہ_component);
		print_رستہ_1(anon.buf);
		strbuf_reset(&anon);
	}
}

ساكن حرف *generate_fake_oid(فراغ)
{
	ساكن uint32_t counter = 1; /* avoid null oid */
	ثابت بنانشانی hashsz = the_hash_algo->rawsz;
	بنية object_id oid;
	حرف *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	أرجع oid_منزل_hex_r(hex, &oid);
}

ساكن ثابت حرف *anonymize_oid(ثابت حرف *oid_hex)
{
	ساكن بنية hashmap objs;
	ناپ_t len = لڑی_لمبائی(oid_hex);
	أرجع anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

ساكن فراغ show_filemodify(بنية diff_قطار_struct *q,
			    بنية diff_options *options بن_ورتیا, فراغ *انکڑے)
{
	صحيح i;
	بنية string_list *بدلیا = انکڑے;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->قطار, q->nr, ڈونگھائی_پہلا);

	لكل (i = 0; i < q->nr; i++) {
		بنية diff_filespec *ospec = q->قطار[i]->one;
		بنية diff_filespec *spec = q->قطار[i]->two;

		بدل (q->قطار[i]->حالت) {
		حالة DIFF_حالت_DELETED:
			لکھو("D ");
			print_رستہ(spec->رستہ);
			string_list_داخل_کرو(بدلیا, spec->رستہ);
			حرف_لکھو('\n');
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
			إذا (!string_list_has_string(بدلیا, ospec->رستہ)) {
				لکھو("%c ", q->قطار[i]->حالت);
				print_رستہ(ospec->رستہ);
				حرف_لکھو(' ');
				print_رستہ(spec->رستہ);
				string_list_داخل_کرو(بدلیا, spec->رستہ);
				حرف_لکھو('\n');

				إذا (oideq(&ospec->oid, &spec->oid) &&
				    ospec->منوال == spec->منوال)
					اكسر;
			}
			/* fallthrough */

		حالة DIFF_حالت_TYPE_بدلیا:
		حالة DIFF_حالت_MODIFIED:
		حالة DIFF_حالت_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			إذا (no_انکڑے || S_ISGITLINK(spec->منوال))
				لکھو("M %06o %s ", spec->منوال,
				       anonymize ?
				       anonymize_oid(oid_منزل_hex(&spec->oid)) :
				       oid_منزل_hex(&spec->oid));
			وإلا {
				بنية object *object = lookup_object(the_repository,
								      &spec->oid);
				لکھو("M %06o :%d ", spec->منوال,
				       get_object_mark(object));
			}
			print_رستہ(spec->رستہ);
			string_list_داخل_کرو(بدلیا, spec->رستہ);
			حرف_لکھو('\n');
			اكسر;

		افتراضي:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->قطار[i]->حالت,
			    ospec->رستہ ? ospec->رستہ : _("none"),
			    spec->رستہ ? spec->رستہ : _("none"));
		}
	}
}

ساكن حرف *anonymize_ref_component(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf باہرلا = STRBUF_INIT;
	strbuf_addf(&باہرلا, "ref%d", counter++);
	أرجع strbuf_detach(&باہرلا, عدم);
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
	لكل (i = 0; i < ARRAY_ناپ(prefixes); i++) {
		إذا (skip_اگیتر(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			اكسر;
		}
	}

	anonymize_رستہ(&anon, refname, &refs, anonymize_ref_component);
	أرجع anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
ساكن حرف *anonymize_commit_سنیہا(فراغ)
{
	ساكن صحيح counter;
	أرجع xstrfmt("subject %d\n\nbody\n", counter++);
}

ساكن حرف *anonymize_ident(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf باہرلا = STRBUF_INIT;
	strbuf_addf(&باہرلا, "User %d <user%d@example.com>", counter, counter);
	counter++;
	أرجع strbuf_detach(&باہرلا, عدم);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
ساكن فراغ anonymize_ident_سطر(ثابت حرف **beg, ثابت حرف **سرا)
{
	ساكن بنية hashmap idents;
	ساكن بنية strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	ساكن بنانشانی which_بفر;

	بنية strbuf *باہرلا;
	بنية ident_ونڈو ونڈو;
	ثابت حرف *سرا_دا_سرخی;

	باہرلا = &buffers[which_بفر++];
	which_بفر %= ARRAY_ناپ(buffers);
	strbuf_reset(باہرلا);

	/* skip "committer", "author", "tagger", etc */
	سرا_دا_سرخی = strchr(*beg, ' ');
	إذا (!سرا_دا_سرخی)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (صحيح)(*سرا - *beg), *beg);
	سرا_دا_سرخی++;
	strbuf_جوڑو(باہرلا, *beg, سرا_دا_سرخی - *beg);

	إذا (!ونڈو_ident_سطر(&ونڈو, سرا_دا_سرخی, *سرا - سرا_دا_سرخی) &&
	    ونڈو.date_begin) {
		ثابت حرف *ident;
		ناپ_t len;

		len = ونڈو.mail_سرا - ونڈو.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      ونڈو.name_begin, len);
		strbuf_addstr(باہرلا, ident);
		strbuf_addch(باہرلا, ' ');
		strbuf_جوڑو(باہرلا, ونڈو.date_begin, ونڈو.tz_سرا - ونڈو.date_begin);
	} وإلا {
		strbuf_addstr(باہرلا, "Malformed Ident <malformed@example.com> 0 -0000");
	}

	*beg = باہرلا->buf;
	*سرا = باہرلا->buf + باہرلا->len;
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
ساكن ثابت حرف *لبھو_commit_multiline_سرخی(ثابت حرف *پیغام,
						ثابت حرف *کلید,
						ثابت حرف **سرا)
{
	بنية strbuf val = STRBUF_INIT;
	ثابت حرف *bol, *eol;
	ناپ_t len;

	bol = لبھو_commit_سرخی(پیغام, کلید, &len);
	إذا (!bol)
		أرجع عدم;
	eol = bol + len;
	strbuf_جوڑو(&val, bol, len);

	طالما (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_جوڑو(&val, bol, eol - bol);
	}

	*سرا = eol;
	أرجع strbuf_detach(&val, عدم);
}

ساكن فراغ print_signature(ثابت حرف *signature, ثابت حرف *object_hash)
{
	إذا (!signature)
		أرجع;

	لکھو("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (بنانشانی)لڑی_لمبائی(signature),
	       signature);
}

ساكن ثابت حرف *append_signatures_for_سرخی(بنية string_list *signatures,
						ثابت حرف *pos,
						ثابت حرف *سرخی,
						ثابت حرف *object_hash)
{
	ثابت حرف *signature;
	ثابت حرف *شروع = pos;
	ثابت حرف *سرا = pos;

	طالما ((signature = لبھو_commit_multiline_سرخی(شروع + 1,
							 سرخی,
							 &سرا))) {
		string_list_append(signatures, signature)->util = (فراغ *)object_hash;
		حرر((حرف *)signature);
		شروع = سرا;
	}

	أرجع سرا;
}

ساكن فراغ handle_commit(بنية commit *commit, بنية rev_info *rev,
			  بنية string_list *paths_دا_بدلیا_objects)
{
	صحيح saved_output_format = rev->diffopt.output_format;
	ثابت حرف *commit_بفر, *commit_بفر_cursor;
	ثابت حرف *author, *author_سرا, *committer, *committer_سرا;
	ثابت حرف *encoding = عدم;
	ناپ_t encoding_len;
	بنية string_list signatures = STRING_LIST_INIT_DUP;
	ثابت حرف *سنیہا;
	حرف *reencoded = عدم;
	بنية commit_list *p;
	ثابت حرف *refname;
	صحيح i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_بفر_cursor = commit_بفر = repo_get_commit_بفر(the_repository, commit, عدم);

	author = لڑی_کھوج(commit_بفر_cursor, "\nauthor ");
	إذا (!author)
		die(_("could not find author in commit %s"),
		    oid_منزل_hex(&commit->object.oid));
	author++;
	commit_بفر_cursor = author_سرا = strchrnul(author, '\n');

	committer = لڑی_کھوج(commit_بفر_cursor, "\ncommitter ");
	إذا (!committer)
		die(_("could not find committer in commit %s"),
		    oid_منزل_hex(&commit->object.oid));
	committer++;
	commit_بفر_cursor = committer_سرا = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	إذا (*commit_بفر_cursor == '\n') {
		encoding = لبھو_commit_سرخی(commit_بفر_cursor + 1, "encoding", &encoding_len);
		إذا (encoding)
			commit_بفر_cursor = encoding + encoding_len;
	}

	إذا (*commit_بفر_cursor == '\n') {
		ثابت حرف *after_sha1 = append_signatures_for_سرخی(&signatures, commit_بفر_cursor,
								      "gpgsig", "sha1");
		ثابت حرف *after_sha256 = append_signatures_for_سرخی(&signatures, commit_بفر_cursor,
									"gpgsig-sha256", "sha256");
		commit_بفر_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	سنیہا = لڑی_کھوج(commit_بفر_cursor, "\n\n");
	إذا (سنیہا)
		سنیہا += 2;

	إذا (commit->parents &&
	    (get_object_mark(&commit->parents->شے->object) != 0 ||
	     reference_excluded_commits) &&
	    !بھریا_درخت) {
		parse_commit_or_die(commit->parents->شے);
		diff_درخت_oid(get_commit_درخت_oid(commit->parents->شے),
			      get_commit_درخت_oid(commit), "", &rev->diffopt);
	}
	وإلا
		diff_جڑ_درخت_oid(get_commit_درخت_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	لكل (i = 0; i < diff_queued_diff.nr; i++)
		إذا (!S_ISGITLINK(diff_queued_diff.قطار[i]->two->منوال))
			export_blob(&diff_queued_diff.قطار[i]->two->oid);

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
		anonymize_ident_سطر(&committer, &committer_سرا);
		anonymize_ident_سطر(&author, &author_سرا);
	}

	mark_اگلا_object(&commit->object);
	إذا (anonymize) {
		reencoded = anonymize_commit_سنیہا();
	} وإلا إذا (encoding) {
		حرف *buf;
		بدل (reencode_منوال) {
		حالة REENCODE_YES:
			buf = xstrfmt("%.*s", (صحيح)encoding_len, encoding);
			reencoded = reencode_string(سنیہا, "UTF-8", buf);
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
		لکھو("reset %s\n", refname);
	لکھو("commit %s\nmark :%"PRIu32"\n", refname, آخری_idnum);
	إذا (show_original_ids)
		لکھو("original-oid %s\n", oid_منزل_hex(&commit->object.oid));
	لکھو("%.*s\n%.*s\n",
	       (صحيح)(author_سرا - author), author,
	       (صحيح)(committer_سرا - committer), committer);
	إذا (signatures.nr) {
		بدل (signed_commit_منوال) {
		/* Exporting modes */
		حالة SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_منزل_hex(&commit->object.oid));
			/* fallthru */
		حالة SIGN_VERBATIM:
			لكل (ناپ_t i = 0; i < signatures.nr; i++) {
				بنية string_list_شے *شے = &signatures.اشیاء[i];
				print_signature(شے->string, شے->util);
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
			BUG("invalid signed_commit_mode value %d", signed_commit_منوال);
		}
		string_list_clear(&signatures, 0);
	}
	إذا (!reencoded && encoding)
		لکھو("encoding %.*s\n", (صحيح)encoding_len, encoding);
	لکھو("data %u\n%s",
	       (بنانشانی)(reencoded
			  ? لڑی_لمبائی(reencoded) : سنیہا
			  ? لڑی_لمبائی(سنیہا) : 0),
	       reencoded ? reencoded : سنیہا ? سنیہا : "");
	حرر(reencoded);
	repo_unuse_commit_بفر(the_repository, commit, commit_بفر);

	لكل (i = 0, p = commit->parents; p; p = p->اگلا) {
		بنية object *obj = &p->شے->object;
		صحيح mark = get_object_mark(obj);

		إذا (!mark && !reference_excluded_commits)
			استمر;
		إذا (i == 0)
			لکھو("from ");
		وإلا
			لکھو("merge ");
		إذا (mark)
			لکھو(":%d\n", mark);
		وإلا
			لکھو("%s\n",
			       anonymize ?
			       anonymize_oid(oid_منزل_hex(&obj->oid)) :
			       oid_منزل_hex(&obj->oid));
		i++;
	}

	إذا (بھریا_درخت)
		لکھو("deleteall\n");
	log_درخت_diff_flush(rev);
	string_list_clear(paths_دا_بدلیا_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	لکھو("\n");

	show_ترقی();
}

ساكن حرف *anonymize_ٹیگ(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf باہرلا = STRBUF_INIT;
	strbuf_addf(&باہرلا, "tag message %d", counter++);
	أرجع strbuf_detach(&باہرلا, عدم);
}


ساكن فراغ handle_ٹیگ(ثابت حرف *name, بنية ٹیگ *ٹیگ)
{
	ناپ_t ناپ;
	تعداد object_type type;
	حرف *buf;
	ثابت حرف *tagger, *tagger_سرا, *سنیہا;
	ناپ_t سنیہا_ناپ = 0;
	بنية object *tagged;
	صحيح tagged_mark;
	بنية commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = ٹیگ->tagged;
	طالما (tagged->type == OBJ_ٹیگ) {
		tagged = ((بنية ٹیگ *)tagged)->tagged;
	}
	إذا (tagged->type == OBJ_درخت) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_منزل_hex(&ٹیگ->object.oid));
		أرجع;
	}

	buf = odb_read_object(the_repository->objects, &ٹیگ->object.oid,
			      &type, &ناپ);
	إذا (!buf)
		die(_("could not read tag %s"), oid_منزل_hex(&ٹیگ->object.oid));
	سنیہا = memmem(buf, ناپ, "\n\n", 2);
	إذا (سنیہا) {
		سنیہا += 2;
		سنیہا_ناپ = لڑی_لمبائی(سنیہا);
	}
	tagger = memmem(buf, سنیہا ? سنیہا - buf : ناپ, "\ntagger ", 8);
	إذا (!tagger) {
		إذا (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		وإلا
			tagger = "";
		tagger_سرا = tagger + لڑی_لمبائی(tagger);
	} وإلا {
		tagger++;
		tagger_سرا = strchrnul(tagger, '\n');
		إذا (anonymize)
			anonymize_ident_سطر(&tagger, &tagger_سرا);
	}

	إذا (anonymize) {
		name = anonymize_refname(name);
		إذا (سنیہا) {
			ساكن بنية hashmap ٹیگ_سوچی;
			سنیہا = anonymize_str(&ٹیگ_سوچی, anonymize_ٹیگ,
						سنیہا, سنیہا_ناپ);
			سنیہا_ناپ = لڑی_لمبائی(سنیہا);
		}
	}

	/* handle signed tags */
	إذا (سنیہا) {
		ناپ_t sig_وتھ = parse_signed_بفر(سنیہا, سنیہا_ناپ);
		إذا (sig_وتھ < سنیہا_ناپ)
			بدل (signed_ٹیگ_منوال) {
			/* Exporting modes */
			حالة SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_منزل_hex(&ٹیگ->object.oid));
				/* fallthru */
			حالة SIGN_VERBATIM:
				اكسر;

			/* Stripping modes */
			حالة SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_منزل_hex(&ٹیگ->object.oid));
				/* fallthru */
			حالة SIGN_STRIP:
				سنیہا_ناپ = sig_وتھ;
				اكسر;

			/* Aborting modes */
			حالة SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_منزل_hex(&ٹیگ->object.oid));
			افتراضي:
				BUG("invalid signed_commit_mode value %d", signed_commit_منوال);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = ٹیگ->tagged;
	tagged_mark = get_object_mark(tagged);
	إذا (!tagged_mark) {
		بدل (ٹیگ_دا_filtered_منوال) {
		حالة ٹیگ_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_منزل_hex(&ٹیگ->object.oid));
		حالة DROP:
			/* Ignore this tag altogether */
			حرر(buf);
			أرجع;
		حالة REWRITE:
			إذا (tagged->type == OBJ_ٹیگ && !mark_ٹیگ_سوچی) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} وإلا إذا (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((بنية commit *)tagged);
				إذا (!p) {
					لکھو("reset %s\nfrom %s\n\n",
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

	إذا (tagged->type == OBJ_ٹیگ) {
		لکھو("reset %s\nfrom %s\n\n",
		       name, oid_منزل_hex(null_oid(the_hash_algo)));
	}
	skip_اگیتر(name, "refs/tags/", &name);
	لکھو("tag %s\n", name);
	إذا (mark_ٹیگ_سوچی) {
		mark_اگلا_object(&ٹیگ->object);
		لکھو("mark :%"PRIu32"\n", آخری_idnum);
	}
	إذا (tagged_mark)
		لکھو("from :%d\n", tagged_mark);
	وإلا
		لکھو("from %s\n", oid_منزل_hex(&tagged->oid));

	إذا (show_original_ids)
		لکھو("original-oid %s\n", oid_منزل_hex(&ٹیگ->object.oid));
	لکھو("%.*s%sdata %d\n%.*s\n",
	       (صحيح)(tagger_سرا - tagger), tagger,
	       tagger == tagger_سرا ? "" : "\n",
	       (صحيح)سنیہا_ناپ, (صحيح)سنیہا_ناپ, سنیہا ? سنیہا : "");
	حرر(buf);
}

ساكن بنية commit *get_commit(بنية rev_cmdline_اندراج *e, ثابت حرف *بھریا_name)
{
	بدل (e->شے->type) {
	حالة OBJ_COMMIT:
		أرجع (بنية commit *)e->شے;
	حالة OBJ_ٹیگ: {
		بنية ٹیگ *ٹیگ = (بنية ٹیگ *)e->شے;

		/* handle nested tags */
		طالما (ٹیگ && ٹیگ->object.type == OBJ_ٹیگ) {
			parse_object(the_repository, &ٹیگ->object.oid);
			string_list_append(&ٹیگ_refs, بھریا_name)->util = ٹیگ;
			ٹیگ = (بنية ٹیگ *)ٹیگ->tagged;
		}
		إذا (!ٹیگ)
			die(_("tag %s points nowhere?"), e->name);
		أرجع (بنية commit *)ٹیگ;
	}
	افتراضي:
		أرجع عدم;
	}
}

ساكن فراغ get_ٹیگ_سوچی_and_duplicates(بنية rev_cmdline_info *info)
{
	صحيح i;

	لكل (i = 0; i < info->nr; i++) {
		بنية rev_cmdline_اندراج *e = info->rev + i;
		بنية object_id oid;
		بنية commit *commit;
		حرف *بھریا_name = عدم;

		إذا (e->flags & UNINTERESTING)
			استمر;

		إذا (repo_dwim_ref(the_repository, e->name, لڑی_لمبائی(e->name),
				  &oid, &بھریا_name, 0) != 1) {
			حرر(بھریا_name);
			استمر;
		}

		إذا (refspecs.nr) {
			حرف *خاص;
			خاص = apply_refspecs(&refspecs, بھریا_name);
			إذا (خاص) {
				حرر(بھریا_name);
				بھریا_name = خاص;
			}
		}

		commit = get_commit(e, بھریا_name);
		إذا (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->شے->type));
			حرر(بھریا_name);
			استمر;
		}

		بدل (commit->object.type) {
		حالة OBJ_COMMIT:
			اكسر;
		حالة OBJ_BLOB:
			export_blob(&commit->object.oid);
			حرر(بھریا_name);
			استمر;
		افتراضي: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			حرر(بھریا_name);
			استمر;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		إذا (e->شے->type != OBJ_ٹیگ)
			string_list_append(&extra_refs, بھریا_name)->util = commit;

		إذا (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = بھریا_name;
		وإلا
			حرر(بھریا_name);
	}

	string_list_ترتیب_دیو_u(&extra_refs, 0);
}

ساكن فراغ handle_ٹیگ_سوچی_and_duplicates(بنية string_list *extras)
{
	بنية commit *commit;
	صحيح i;

	لكل (i = extras->nr - 1; i >= 0; i--) {
		ثابت حرف *name = extras->اشیاء[i].string;
		بنية object *object = extras->اشیاء[i].util;
		صحيح mark;

		بدل (object->type) {
		حالة OBJ_ٹیگ:
			handle_ٹیگ(name, (بنية ٹیگ *)object);
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
				لکھو("reset %s\nfrom %s\n\n",
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
					لکھو("reset %s\nfrom %s\n\n",
					       name, oid_منزل_hex(null_oid(the_hash_algo)));
					استمر;
				}
				/* set ref to commit using oid, not mark */
				لکھو("reset %s\nfrom %s\n\n", name,
				       oid_منزل_hex(&commit->object.oid));
				استمر;
			}

			لکھو("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_ترقی();
			اكسر;
		}
	}
}

ساكن فراغ export_marks(حرف *file)
{
	بنانشانی صحيح i;
	uint32_t mark;
	بنية decoration_اندراج *deco = idnums.entries;
	FILE *f;
	صحيح e = 0;

	f = fopen_for_writing(file);
	إذا (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	لكل (i = 0; i < idnums.ناپ; i++) {
		إذا (deco->ادھار && deco->ادھار->type == 1) {
			mark = ptr_منزل_mark(deco->decoration);
			إذا (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_منزل_hex(&deco->ادھار->oid)) < 0) {
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

ساكن فراغ import_marks(حرف *داخلہ_file, صحيح check_exists)
{
	حرف سطر[512];
	FILE *f;
	بنية stat sb;

	إذا (check_exists && stat(داخلہ_file, &sb))
		أرجع;

	f = xfopen(داخلہ_file, "r");
	طالما (fgets(سطر, حجم(سطر), f)) {
		uint32_t mark;
		حرف *سطر_سرا, *mark_سرا;
		بنية object_id oid;
		بنية object *object;
		بنية commit *commit;
		تعداد object_type type;

		سطر_سرا = strchr(سطر, '\n');
		إذا (سطر[0] != ':' || !سطر_سرا)
			die(_("corrupt mark line: %s"), سطر);
		*سطر_سرا = '\0';

		mark = strtoumax(سطر + 1, &mark_سرا, 10);
		إذا (!mark || mark_سرا == سطر + 1
			|| *mark_سرا != ' ' || get_oid_hex(mark_سرا + 1, &oid))
			die(_("corrupt mark line: %s"), سطر);

		إذا (آخری_idnum < mark)
			آخری_idnum = mark;

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
		بنية refspec_شے *refspec = &refspecs.اشیاء[i];
		إذا (*refspec->src)
			استمر;

		لکھو("reset %s\nfrom %s\n\n",
				refspec->dst, oid_منزل_hex(null_oid(the_hash_algo)));
	}
}

ساكن صحيح parse_opt_anonymize_map(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	بنية hashmap *map = opt->قدر;
	ثابت حرف *delim, *قدر;
	ناپ_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	إذا (delim) {
		keylen = delim - arg;
		قدر = delim + 1;
	} وإلا {
		keylen = لڑی_لمبائی(arg);
		قدر = arg;
	}

	إذا (!keylen || !*قدر)
		أرجع error(_("--anonymize-map token cannot be empty"));

	جوڑو_anonymized_اندراج(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(قدر));

	أرجع 0;
}

صحيح cmd_fast_export(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *اگیتر,
		    بنية repository *repo بن_ورتیا)
{
	بنية rev_info revs;
	بنية commit *commit;
	حرف *export_filename = عدم,
	     *import_filename = عدم,
	     *import_filename_if_exists = عدم;
	uint32_t lastimportid;
	بنية string_list refspecs_list = STRING_LIST_INIT_NODUP;
	بنية string_list paths_دا_بدلیا_objects = STRING_LIST_INIT_DUP;
	بنية option options[] = {
		OPT_INTEGER(0, "progress", &ترقی,
			    N_("show progress after <n> objects")),
		OPT_CALLBACK(0, "signed-tags", &signed_ٹیگ_منوال, N_("mode"),
			     N_("select handling of signed tags"),
			     parse_opt_sign_منوال),
		OPT_CALLBACK(0, "signed-commits", &signed_commit_منوال, N_("mode"),
			     N_("select handling of signed commits"),
			     parse_opt_sign_منوال),
		OPT_CALLBACK(0, "tag-of-filtered-object", &ٹیگ_دا_filtered_منوال, N_("mode"),
			     N_("select handling of tags that tag filtered objects"),
			     parse_opt_ٹیگ_دا_filtered_منوال),
		OPT_CALLBACK(0, "reencode", &reencode_منوال, N_("mode"),
			     N_("select handling of commit messages in an alternate encoding"),
			     parse_opt_reencode_منوال),
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
		OPT_BOOL(0, "full-tree", &بھریا_درخت,
			 N_("output full tree for each commit")),
		OPT_BOOL(0, "use-done-feature", &use_پورا_feature,
			     N_("use the done feature to terminate the stream")),
		OPT_BOOL(0, "no-data", &no_انکڑے, N_("skip output of blob data")),
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
		OPT_BOOL(0, "mark-tags", &mark_ٹیگ_سوچی,
			    N_("label tags with mark ids")),

		OPT_سرا()
	};

	إذا (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, عدم);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, اگیتر);
	init_revision_sources(&revision_sources);
	revs.topo_ترتیب = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, اگیتر, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = تیاری_revisions(argc, argv, &revs, عدم);
	إذا (argc > 1)
		usage_with_options (fast_export_usage, options);

	إذا (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	إذا (refspecs_list.nr) {
		صحيح i;

		لكل (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.اشیاء[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	إذا (use_پورا_feature)
		لکھو("feature done\n");

	إذا (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	إذا (import_filename)
		import_marks(import_filename, 0);
	وإلا إذا (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = آخری_idnum;

	إذا (import_filename && revs.prune_انکڑے.nr)
		بھریا_درخت = 1;

	get_ٹیگ_سوچی_and_duplicates(&revs.cmdline);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.الٹ = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_انکڑے = &paths_دا_بدلیا_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	طالما ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_دا_بدلیا_objects);
	revs.diffopt.no_free = 0;

	handle_ٹیگ_سوچی_and_duplicates(&extra_refs);
	handle_ٹیگ_سوچی_and_duplicates(&ٹیگ_refs);
	handle_deletes();

	إذا (export_filename && lastimportid != آخری_idnum)
		export_marks(export_filename);

	إذا (use_پورا_feature)
		لکھو("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	أرجع 0;
}
