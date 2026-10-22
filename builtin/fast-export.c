/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن صحيح ترقي;
ساكن تعداد sign_گهڻائي signed_ٽيگ_گهڻائي = SIGN_ABORT;
ساكن تعداد sign_گهڻائي signed_commit_گهڻائي = SIGN_STRIP;
ساكن تعداد ٽيگ_جو_filtered_گهڻائي { ٽيگ_FILTERING_ABORT, DROP, REWRITE } ٽيگ_جو_filtered_گهڻائي = ٽيگ_FILTERING_ABORT;
ساكن تعداد reencode_گهڻائي { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_گهڻائي = REENCODE_ABORT;
ساكن صحيح fake_missing_tagger;
ساكن صحيح use_پورو_feature;
ساكن صحيح no_معلومات;
ساكن صحيح ڀريل_وڻ;
ساكن صحيح reference_excluded_commits;
ساكن صحيح show_original_ids;
ساكن صحيح mark_ٽيگ_فهرست;
ساكن بنية string_list extra_refs = STRING_LIST_INIT_DUP;
ساكن بنية string_list ٽيگ_refs = STRING_LIST_INIT_DUP;
ساكن بنية refspec refspecs;
ساكن صحيح anonymize;
ساكن بنية hashmap anonymized_seeds;
ساكن بنية revision_sources revision_sources;

ساكن صحيح parse_opt_sign_گهڻائي(ثابت بنية option *opt,
			       ثابت حرف *arg, صحيح unset)
{
	تعداد sign_گهڻائي *val = opt->قدر;

	إذا (unset)
		أرجع 0;

	إذا (parse_sign_گهڻائي(arg, val, عدم) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		أرجع error(_("unknown %s mode: %s"), opt->long_name, arg);

	أرجع 0;
}

ساكن صحيح parse_opt_ٽيگ_جو_filtered_گهڻائي(ثابت بنية option *opt,
					  ثابت حرف *arg, صحيح unset)
{
	تعداد ٽيگ_جو_filtered_گهڻائي *val = opt->قدر;

	إذا (unset || !strcmp(arg, "abort"))
		*val = ٽيگ_FILTERING_ABORT;
	وإلا إذا (!strcmp(arg, "drop"))
		*val = DROP;
	وإلا إذا (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	وإلا
		أرجع error(_("unknown tag-of-filtered mode: %s"), arg);
	أرجع 0;
}

ساكن صحيح parse_opt_reencode_گهڻائي(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	تعداد reencode_گهڻائي *val = opt->قدر;

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
ساكن uint32_t آخري_idnum;
بنية anonymized_داخلا {
	بنية hashmap_داخلا hash;
	حرف *anon;
	ثابت حرف orig[FLEX_ARRAY];
};

بنية anonymized_داخلا_ڪنجي {
	بنية hashmap_داخلا hash;
	ثابت حرف *orig;
	سائيز_t orig_len;
};

ساكن صحيح anonymized_داخلا_cmp(ثابت فراغ *cmp_معلومات اڻ_استعمال_ٿيل,
				ثابت بنية hashmap_داخلا *eptr,
				ثابت بنية hashmap_داخلا *داخلا_or_ڪنجي,
				ثابت فراغ *keydata)
{
	ثابت بنية anonymized_داخلا *a, *b;

	a = container_جو(eptr, ثابت بنية anonymized_داخلا, hash);
	إذا (keydata) {
		ثابت بنية anonymized_داخلا_ڪنجي *ڪنجي = keydata;
		صحيح equal = !xstrncmpz(a->orig, ڪنجي->orig, ڪنجي->orig_len);
		أرجع !equal;
	}

	b = container_جو(داخلا_or_ڪنجي, ثابت بنية anonymized_داخلا, hash);
	أرجع strcmp(a->orig, b->orig);
}

ساكن بنية anonymized_داخلا *جوڙيو_anonymized_داخلا(بنية hashmap *map,
						     بنانشان hash,
						     ثابت حرف *orig, سائيز_t len,
						     حرف *anon)
{
	بنية anonymized_داخلا *ret, *old;

	إذا (!map->cmpfn)
		hashmap_init(map, anonymized_داخلا_cmp, عدم, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_داخلا_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_داخلا(map, ret, hash);

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
				 ثابت حرف *orig, سائيز_t len)
{
	بنية anonymized_داخلا_ڪنجي ڪنجي;
	بنية anonymized_داخلا *ret;

	hashmap_داخلا_init(&ڪنجي.hash, memhash(orig, len));
	ڪنجي.orig = orig;
	ڪنجي.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_داخلا(&anonymized_seeds, &ڪنجي, hash, &ڪنجي);

	/* ...otherwise check if we've already seen it in this context... */
	إذا (!ret)
		ret = hashmap_get_داخلا(map, &ڪنجي, hash, &ڪنجي);

	/* ...and finally generate a new mapping if necessary */
	إذا (!ret)
		ret = جوڙيو_anonymized_داخلا(map, ڪنجي.hash.hash,
					   orig, len, generate());

	أرجع ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
ساكن فراغ anonymize_رستو(بنية strbuf *ٻاهريون, ثابت حرف *رستو,
			   بنية hashmap *map,
			   حرف *(*generate)(فراغ))
{
	طالما (*رستو) {
		ثابت حرف *پڇاڙي_جو_component = strchrnul(رستو, '/');
		سائيز_t len = پڇاڙي_جو_component - رستو;
		ثابت حرف *c = anonymize_str(map, generate, رستو, len);
		strbuf_addstr(ٻاهريون, c);
		رستو = پڇاڙي_جو_component;
		إذا (*رستو)
			strbuf_addch(ٻاهريون, *رستو++);
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
	جوڙيو_decoration(&idnums, object, mark_منزل_ptr(mark));
}

ساكن مضمن فراغ mark_ايندڙ_object(بنية object *object)
{
	mark_object(object, ++آخري_idnum);
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
		إذا (p->parents && p->parents->ايندڙ)
			اكسر;
		إذا (p->object.flags & UNINTERESTING)
			اكسر;
		إذا (!(p->object.flags & TREESAME))
			اكسر;
		إذا (!p->parents)
			أرجع عدم;
		p = p->parents->شي;
	}
	أرجع p;
}

ساكن فراغ show_ترقي(فراغ)
{
	ساكن صحيح counter = 0;
	إذا (!ترقي)
		أرجع;
	إذا ((++counter % ترقي) == 0)
		لکو("progress %d objects\n", counter);
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
ساكن حرف *anonymize_blob(بنانشان ڊگهو *سائيز)
{
	ساكن صحيح counter;
	بنية strbuf ٻاهريون = STRBUF_INIT;
	strbuf_addf(&ٻاهريون, "anonymous blob %d", counter++);
	*سائيز = ٻاهريون.len;
	أرجع strbuf_detach(&ٻاهريون, عدم);
}

ساكن فراغ export_blob(ثابت بنية object_id *oid)
{
	بنانشان ڊگهو سائيز;
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
		buf = anonymize_blob(&سائيز);
		object = (بنية object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} وإلا {
		سائيز_t سائيز_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &سائيز_st);
		سائيز = cast_سائيز_t_منزل_ulong(سائيز_st);
		إذا (!buf)
			die(_("could not read blob %s"), oid_منزل_hex(oid));
		إذا (check_object_signature(the_repository, oid, buf, سائيز,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_منزل_hex(oid));
		object = parse_object_بفر(the_repository, oid, type,
					     سائيز, buf, &eaten);
	}

	إذا (!object)
		die(_("could not read blob %s"), oid_منزل_hex(oid));

	mark_ايندڙ_object(object);

	لکو("blob\nmark :%"PRIu32"\n", آخري_idnum);
	إذا (show_original_ids)
		لکو("original-oid %s\n", oid_منزل_hex(oid));
	لکو("data %"PRIuMAX"\n", (uintmax_t)سائيز);
	إذا (سائيز && fwrite(buf, سائيز, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_منزل_hex(oid));
	لکو("\n");

	show_ترقي();

	object->flags |= SHOWN;
	إذا (!eaten)
		حرر(buf);
}

ساكن صحيح اونهائي_پهريون(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت بنية diff_filepair *a = *((ثابت بنية diff_filepair **)a_);
	ثابت بنية diff_filepair *b = *((ثابت بنية diff_filepair **)b_);
	ثابت حرف *name_a, *name_b;
	صحيح len_a, len_b, len;
	صحيح cmp;

	name_a = a->one ? a->one->رستو : a->two->رستو;
	name_b = b->one ? b->one->رستو : b->two->رستو;

	len_a = لڙي_ڊيگهه(name_a);
	len_b = لڙي_ڊيگهه(name_b);
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

ساكن فراغ print_رستو_1(ثابت حرف *رستو)
{
	صحيح need_quote = quote_c_style(رستو, عدم, عدم, 0);
	إذا (need_quote)
		quote_c_style(رستو, عدم, stdout, 0);
	وإلا إذا (strchr(رستو, ' '))
		لکو("\"%s\"", رستو);
	وإلا
		لکو("%s", رستو);
}

ساكن حرف *anonymize_رستو_component(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf ٻاهريون = STRBUF_INIT;
	strbuf_addf(&ٻاهريون, "path%d", counter++);
	أرجع strbuf_detach(&ٻاهريون, عدم);
}

ساكن فراغ print_رستو(ثابت حرف *رستو)
{
	إذا (!anonymize)
		print_رستو_1(رستو);
	وإلا {
		ساكن بنية hashmap paths;
		ساكن بنية strbuf anon = STRBUF_INIT;

		anonymize_رستو(&anon, رستو, &paths, anonymize_رستو_component);
		print_رستو_1(anon.buf);
		strbuf_reset(&anon);
	}
}

ساكن حرف *generate_fake_oid(فراغ)
{
	ساكن uint32_t counter = 1; /* avoid null oid */
	ثابت بنانشان hashsz = the_hash_algo->rawsz;
	بنية object_id oid;
	حرف *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	أرجع oid_منزل_hex_r(hex, &oid);
}

ساكن ثابت حرف *anonymize_oid(ثابت حرف *oid_hex)
{
	ساكن بنية hashmap objs;
	سائيز_t len = لڙي_ڊيگهه(oid_hex);
	أرجع anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

ساكن فراغ show_filemodify(بنية diff_قطار_struct *q,
			    بنية diff_options *options اڻ_استعمال_ٿيل, فراغ *معلومات)
{
	صحيح i;
	بنية string_list *بدليو = معلومات;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->قطار, q->nr, اونهائي_پهريون);

	لكل (i = 0; i < q->nr; i++) {
		بنية diff_filespec *ospec = q->قطار[i]->one;
		بنية diff_filespec *spec = q->قطار[i]->two;

		بدل (q->قطار[i]->حالت) {
		حالة DIFF_حالت_DELETED:
			لکو("D ");
			print_رستو(spec->رستو);
			string_list_وجهو(بدليو, spec->رستو);
			اکر_لکو('\n');
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
			إذا (!string_list_has_string(بدليو, ospec->رستو)) {
				لکو("%c ", q->قطار[i]->حالت);
				print_رستو(ospec->رستو);
				اکر_لکو(' ');
				print_رستو(spec->رستو);
				string_list_وجهو(بدليو, spec->رستو);
				اکر_لکو('\n');

				إذا (oideq(&ospec->oid, &spec->oid) &&
				    ospec->گهڻائي == spec->گهڻائي)
					اكسر;
			}
			/* fallthrough */

		حالة DIFF_حالت_TYPE_بدليو:
		حالة DIFF_حالت_MODIFIED:
		حالة DIFF_حالت_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			إذا (no_معلومات || S_ISGITLINK(spec->گهڻائي))
				لکو("M %06o %s ", spec->گهڻائي,
				       anonymize ?
				       anonymize_oid(oid_منزل_hex(&spec->oid)) :
				       oid_منزل_hex(&spec->oid));
			وإلا {
				بنية object *object = lookup_object(the_repository,
								      &spec->oid);
				لکو("M %06o :%d ", spec->گهڻائي,
				       get_object_mark(object));
			}
			print_رستو(spec->رستو);
			string_list_وجهو(بدليو, spec->رستو);
			اکر_لکو('\n');
			اكسر;

		افتراضي:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->قطار[i]->حالت,
			    ospec->رستو ? ospec->رستو : _("none"),
			    spec->رستو ? spec->رستو : _("none"));
		}
	}
}

ساكن حرف *anonymize_ref_component(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf ٻاهريون = STRBUF_INIT;
	strbuf_addf(&ٻاهريون, "ref%d", counter++);
	أرجع strbuf_detach(&ٻاهريون, عدم);
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
	لكل (i = 0; i < ARRAY_سائيز(prefixes); i++) {
		إذا (skip_اڳياڙي(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			اكسر;
		}
	}

	anonymize_رستو(&anon, refname, &refs, anonymize_ref_component);
	أرجع anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
ساكن حرف *anonymize_commit_پيغام(فراغ)
{
	ساكن صحيح counter;
	أرجع xstrfmt("subject %d\n\nbody\n", counter++);
}

ساكن حرف *anonymize_ident(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf ٻاهريون = STRBUF_INIT;
	strbuf_addf(&ٻاهريون, "User %d <user%d@example.com>", counter, counter);
	counter++;
	أرجع strbuf_detach(&ٻاهريون, عدم);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
ساكن فراغ anonymize_ident_سٽ(ثابت حرف **beg, ثابت حرف **پڇاڙي)
{
	ساكن بنية hashmap idents;
	ساكن بنية strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	ساكن بنانشان which_بفر;

	بنية strbuf *ٻاهريون;
	بنية ident_ورهايو ورهايو;
	ثابت حرف *پڇاڙي_جو_سرو;

	ٻاهريون = &buffers[which_بفر++];
	which_بفر %= ARRAY_سائيز(buffers);
	strbuf_reset(ٻاهريون);

	/* skip "committer", "author", "tagger", etc */
	پڇاڙي_جو_سرو = strchr(*beg, ' ');
	إذا (!پڇاڙي_جو_سرو)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (صحيح)(*پڇاڙي - *beg), *beg);
	پڇاڙي_جو_سرو++;
	strbuf_جوڙيو(ٻاهريون, *beg, پڇاڙي_جو_سرو - *beg);

	إذا (!ورهايو_ident_سٽ(&ورهايو, پڇاڙي_جو_سرو, *پڇاڙي - پڇاڙي_جو_سرو) &&
	    ورهايو.date_begin) {
		ثابت حرف *ident;
		سائيز_t len;

		len = ورهايو.mail_پڇاڙي - ورهايو.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      ورهايو.name_begin, len);
		strbuf_addstr(ٻاهريون, ident);
		strbuf_addch(ٻاهريون, ' ');
		strbuf_جوڙيو(ٻاهريون, ورهايو.date_begin, ورهايو.tz_پڇاڙي - ورهايو.date_begin);
	} وإلا {
		strbuf_addstr(ٻاهريون, "Malformed Ident <malformed@example.com> 0 -0000");
	}

	*beg = ٻاهريون->buf;
	*پڇاڙي = ٻاهريون->buf + ٻاهريون->len;
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
ساكن ثابت حرف *ڳوليو_commit_multiline_سرو(ثابت حرف *نياپو,
						ثابت حرف *ڪنجي,
						ثابت حرف **پڇاڙي)
{
	بنية strbuf val = STRBUF_INIT;
	ثابت حرف *bol, *eol;
	سائيز_t len;

	bol = ڳوليو_commit_سرو(نياپو, ڪنجي, &len);
	إذا (!bol)
		أرجع عدم;
	eol = bol + len;
	strbuf_جوڙيو(&val, bol, len);

	طالما (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_جوڙيو(&val, bol, eol - bol);
	}

	*پڇاڙي = eol;
	أرجع strbuf_detach(&val, عدم);
}

ساكن فراغ print_signature(ثابت حرف *signature, ثابت حرف *object_hash)
{
	إذا (!signature)
		أرجع;

	لکو("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (بنانشان)لڙي_ڊيگهه(signature),
	       signature);
}

ساكن ثابت حرف *append_signatures_for_سرو(بنية string_list *signatures,
						ثابت حرف *pos,
						ثابت حرف *سرو,
						ثابت حرف *object_hash)
{
	ثابت حرف *signature;
	ثابت حرف *شروعات = pos;
	ثابت حرف *پڇاڙي = pos;

	طالما ((signature = ڳوليو_commit_multiline_سرو(شروعات + 1,
							 سرو,
							 &پڇاڙي))) {
		string_list_append(signatures, signature)->util = (فراغ *)object_hash;
		حرر((حرف *)signature);
		شروعات = پڇاڙي;
	}

	أرجع پڇاڙي;
}

ساكن فراغ handle_commit(بنية commit *commit, بنية rev_info *rev,
			  بنية string_list *paths_جو_بدليو_objects)
{
	صحيح saved_output_format = rev->diffopt.output_format;
	ثابت حرف *commit_بفر, *commit_بفر_cursor;
	ثابت حرف *author, *author_پڇاڙي, *committer, *committer_پڇاڙي;
	ثابت حرف *encoding = عدم;
	سائيز_t encoding_len;
	بنية string_list signatures = STRING_LIST_INIT_DUP;
	ثابت حرف *پيغام;
	حرف *reencoded = عدم;
	بنية commit_list *p;
	ثابت حرف *refname;
	صحيح i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_بفر_cursor = commit_بفر = repo_get_commit_بفر(the_repository, commit, عدم);

	author = لڙي_ڳولا(commit_بفر_cursor, "\nauthor ");
	إذا (!author)
		die(_("could not find author in commit %s"),
		    oid_منزل_hex(&commit->object.oid));
	author++;
	commit_بفر_cursor = author_پڇاڙي = strchrnul(author, '\n');

	committer = لڙي_ڳولا(commit_بفر_cursor, "\ncommitter ");
	إذا (!committer)
		die(_("could not find committer in commit %s"),
		    oid_منزل_hex(&commit->object.oid));
	committer++;
	commit_بفر_cursor = committer_پڇاڙي = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	إذا (*commit_بفر_cursor == '\n') {
		encoding = ڳوليو_commit_سرو(commit_بفر_cursor + 1, "encoding", &encoding_len);
		إذا (encoding)
			commit_بفر_cursor = encoding + encoding_len;
	}

	إذا (*commit_بفر_cursor == '\n') {
		ثابت حرف *after_sha1 = append_signatures_for_سرو(&signatures, commit_بفر_cursor,
								      "gpgsig", "sha1");
		ثابت حرف *after_sha256 = append_signatures_for_سرو(&signatures, commit_بفر_cursor,
									"gpgsig-sha256", "sha256");
		commit_بفر_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	پيغام = لڙي_ڳولا(commit_بفر_cursor, "\n\n");
	إذا (پيغام)
		پيغام += 2;

	إذا (commit->parents &&
	    (get_object_mark(&commit->parents->شي->object) != 0 ||
	     reference_excluded_commits) &&
	    !ڀريل_وڻ) {
		parse_commit_or_die(commit->parents->شي);
		diff_وڻ_oid(get_commit_وڻ_oid(commit->parents->شي),
			      get_commit_وڻ_oid(commit), "", &rev->diffopt);
	}
	وإلا
		diff_پاڙ_وڻ_oid(get_commit_وڻ_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	لكل (i = 0; i < diff_queued_diff.nr; i++)
		إذا (!S_ISGITLINK(diff_queued_diff.قطار[i]->two->گهڻائي))
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
		anonymize_ident_سٽ(&committer, &committer_پڇاڙي);
		anonymize_ident_سٽ(&author, &author_پڇاڙي);
	}

	mark_ايندڙ_object(&commit->object);
	إذا (anonymize) {
		reencoded = anonymize_commit_پيغام();
	} وإلا إذا (encoding) {
		حرف *buf;
		بدل (reencode_گهڻائي) {
		حالة REENCODE_YES:
			buf = xstrfmt("%.*s", (صحيح)encoding_len, encoding);
			reencoded = reencode_string(پيغام, "UTF-8", buf);
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
		لکو("reset %s\n", refname);
	لکو("commit %s\nmark :%"PRIu32"\n", refname, آخري_idnum);
	إذا (show_original_ids)
		لکو("original-oid %s\n", oid_منزل_hex(&commit->object.oid));
	لکو("%.*s\n%.*s\n",
	       (صحيح)(author_پڇاڙي - author), author,
	       (صحيح)(committer_پڇاڙي - committer), committer);
	إذا (signatures.nr) {
		بدل (signed_commit_گهڻائي) {
		/* Exporting modes */
		حالة SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_منزل_hex(&commit->object.oid));
			/* fallthru */
		حالة SIGN_VERBATIM:
			لكل (سائيز_t i = 0; i < signatures.nr; i++) {
				بنية string_list_شي *شي = &signatures.شيون[i];
				print_signature(شي->string, شي->util);
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
			BUG("invalid signed_commit_mode value %d", signed_commit_گهڻائي);
		}
		string_list_clear(&signatures, 0);
	}
	إذا (!reencoded && encoding)
		لکو("encoding %.*s\n", (صحيح)encoding_len, encoding);
	لکو("data %u\n%s",
	       (بنانشان)(reencoded
			  ? لڙي_ڊيگهه(reencoded) : پيغام
			  ? لڙي_ڊيگهه(پيغام) : 0),
	       reencoded ? reencoded : پيغام ? پيغام : "");
	حرر(reencoded);
	repo_unuse_commit_بفر(the_repository, commit, commit_بفر);

	لكل (i = 0, p = commit->parents; p; p = p->ايندڙ) {
		بنية object *obj = &p->شي->object;
		صحيح mark = get_object_mark(obj);

		إذا (!mark && !reference_excluded_commits)
			استمر;
		إذا (i == 0)
			لکو("from ");
		وإلا
			لکو("merge ");
		إذا (mark)
			لکو(":%d\n", mark);
		وإلا
			لکو("%s\n",
			       anonymize ?
			       anonymize_oid(oid_منزل_hex(&obj->oid)) :
			       oid_منزل_hex(&obj->oid));
		i++;
	}

	إذا (ڀريل_وڻ)
		لکو("deleteall\n");
	log_وڻ_diff_flush(rev);
	string_list_clear(paths_جو_بدليو_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	لکو("\n");

	show_ترقي();
}

ساكن حرف *anonymize_ٽيگ(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf ٻاهريون = STRBUF_INIT;
	strbuf_addf(&ٻاهريون, "tag message %d", counter++);
	أرجع strbuf_detach(&ٻاهريون, عدم);
}


ساكن فراغ handle_ٽيگ(ثابت حرف *name, بنية ٽيگ *ٽيگ)
{
	سائيز_t سائيز;
	تعداد object_type type;
	حرف *buf;
	ثابت حرف *tagger, *tagger_پڇاڙي, *پيغام;
	سائيز_t پيغام_سائيز = 0;
	بنية object *tagged;
	صحيح tagged_mark;
	بنية commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = ٽيگ->tagged;
	طالما (tagged->type == OBJ_ٽيگ) {
		tagged = ((بنية ٽيگ *)tagged)->tagged;
	}
	إذا (tagged->type == OBJ_وڻ) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_منزل_hex(&ٽيگ->object.oid));
		أرجع;
	}

	buf = odb_read_object(the_repository->objects, &ٽيگ->object.oid,
			      &type, &سائيز);
	إذا (!buf)
		die(_("could not read tag %s"), oid_منزل_hex(&ٽيگ->object.oid));
	پيغام = memmem(buf, سائيز, "\n\n", 2);
	إذا (پيغام) {
		پيغام += 2;
		پيغام_سائيز = لڙي_ڊيگهه(پيغام);
	}
	tagger = memmem(buf, پيغام ? پيغام - buf : سائيز, "\ntagger ", 8);
	إذا (!tagger) {
		إذا (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		وإلا
			tagger = "";
		tagger_پڇاڙي = tagger + لڙي_ڊيگهه(tagger);
	} وإلا {
		tagger++;
		tagger_پڇاڙي = strchrnul(tagger, '\n');
		إذا (anonymize)
			anonymize_ident_سٽ(&tagger, &tagger_پڇاڙي);
	}

	إذا (anonymize) {
		name = anonymize_refname(name);
		إذا (پيغام) {
			ساكن بنية hashmap ٽيگ_فهرست;
			پيغام = anonymize_str(&ٽيگ_فهرست, anonymize_ٽيگ,
						پيغام, پيغام_سائيز);
			پيغام_سائيز = لڙي_ڊيگهه(پيغام);
		}
	}

	/* handle signed tags */
	إذا (پيغام) {
		سائيز_t sig_وٿي = parse_signed_بفر(پيغام, پيغام_سائيز);
		إذا (sig_وٿي < پيغام_سائيز)
			بدل (signed_ٽيگ_گهڻائي) {
			/* Exporting modes */
			حالة SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_منزل_hex(&ٽيگ->object.oid));
				/* fallthru */
			حالة SIGN_VERBATIM:
				اكسر;

			/* Stripping modes */
			حالة SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_منزل_hex(&ٽيگ->object.oid));
				/* fallthru */
			حالة SIGN_STRIP:
				پيغام_سائيز = sig_وٿي;
				اكسر;

			/* Aborting modes */
			حالة SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_منزل_hex(&ٽيگ->object.oid));
			افتراضي:
				BUG("invalid signed_commit_mode value %d", signed_commit_گهڻائي);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = ٽيگ->tagged;
	tagged_mark = get_object_mark(tagged);
	إذا (!tagged_mark) {
		بدل (ٽيگ_جو_filtered_گهڻائي) {
		حالة ٽيگ_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_منزل_hex(&ٽيگ->object.oid));
		حالة DROP:
			/* Ignore this tag altogether */
			حرر(buf);
			أرجع;
		حالة REWRITE:
			إذا (tagged->type == OBJ_ٽيگ && !mark_ٽيگ_فهرست) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} وإلا إذا (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((بنية commit *)tagged);
				إذا (!p) {
					لکو("reset %s\nfrom %s\n\n",
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

	إذا (tagged->type == OBJ_ٽيگ) {
		لکو("reset %s\nfrom %s\n\n",
		       name, oid_منزل_hex(null_oid(the_hash_algo)));
	}
	skip_اڳياڙي(name, "refs/tags/", &name);
	لکو("tag %s\n", name);
	إذا (mark_ٽيگ_فهرست) {
		mark_ايندڙ_object(&ٽيگ->object);
		لکو("mark :%"PRIu32"\n", آخري_idnum);
	}
	إذا (tagged_mark)
		لکو("from :%d\n", tagged_mark);
	وإلا
		لکو("from %s\n", oid_منزل_hex(&tagged->oid));

	إذا (show_original_ids)
		لکو("original-oid %s\n", oid_منزل_hex(&ٽيگ->object.oid));
	لکو("%.*s%sdata %d\n%.*s\n",
	       (صحيح)(tagger_پڇاڙي - tagger), tagger,
	       tagger == tagger_پڇاڙي ? "" : "\n",
	       (صحيح)پيغام_سائيز, (صحيح)پيغام_سائيز, پيغام ? پيغام : "");
	حرر(buf);
}

ساكن بنية commit *get_commit(بنية rev_cmdline_داخلا *e, ثابت حرف *ڀريل_name)
{
	بدل (e->شي->type) {
	حالة OBJ_COMMIT:
		أرجع (بنية commit *)e->شي;
	حالة OBJ_ٽيگ: {
		بنية ٽيگ *ٽيگ = (بنية ٽيگ *)e->شي;

		/* handle nested tags */
		طالما (ٽيگ && ٽيگ->object.type == OBJ_ٽيگ) {
			parse_object(the_repository, &ٽيگ->object.oid);
			string_list_append(&ٽيگ_refs, ڀريل_name)->util = ٽيگ;
			ٽيگ = (بنية ٽيگ *)ٽيگ->tagged;
		}
		إذا (!ٽيگ)
			die(_("tag %s points nowhere?"), e->name);
		أرجع (بنية commit *)ٽيگ;
	}
	افتراضي:
		أرجع عدم;
	}
}

ساكن فراغ get_ٽيگ_فهرست_and_duplicates(بنية rev_cmdline_info *info)
{
	صحيح i;

	لكل (i = 0; i < info->nr; i++) {
		بنية rev_cmdline_داخلا *e = info->rev + i;
		بنية object_id oid;
		بنية commit *commit;
		حرف *ڀريل_name = عدم;

		إذا (e->flags & UNINTERESTING)
			استمر;

		إذا (repo_dwim_ref(the_repository, e->name, لڙي_ڊيگهه(e->name),
				  &oid, &ڀريل_name, 0) != 1) {
			حرر(ڀريل_name);
			استمر;
		}

		إذا (refspecs.nr) {
			حرف *خاص;
			خاص = apply_refspecs(&refspecs, ڀريل_name);
			إذا (خاص) {
				حرر(ڀريل_name);
				ڀريل_name = خاص;
			}
		}

		commit = get_commit(e, ڀريل_name);
		إذا (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->شي->type));
			حرر(ڀريل_name);
			استمر;
		}

		بدل (commit->object.type) {
		حالة OBJ_COMMIT:
			اكسر;
		حالة OBJ_BLOB:
			export_blob(&commit->object.oid);
			حرر(ڀريل_name);
			استمر;
		افتراضي: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			حرر(ڀريل_name);
			استمر;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		إذا (e->شي->type != OBJ_ٽيگ)
			string_list_append(&extra_refs, ڀريل_name)->util = commit;

		إذا (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = ڀريل_name;
		وإلا
			حرر(ڀريل_name);
	}

	string_list_ترتيب_ڏيو_u(&extra_refs, 0);
}

ساكن فراغ handle_ٽيگ_فهرست_and_duplicates(بنية string_list *extras)
{
	بنية commit *commit;
	صحيح i;

	لكل (i = extras->nr - 1; i >= 0; i--) {
		ثابت حرف *name = extras->شيون[i].string;
		بنية object *object = extras->شيون[i].util;
		صحيح mark;

		بدل (object->type) {
		حالة OBJ_ٽيگ:
			handle_ٽيگ(name, (بنية ٽيگ *)object);
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
				لکو("reset %s\nfrom %s\n\n",
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
					لکو("reset %s\nfrom %s\n\n",
					       name, oid_منزل_hex(null_oid(the_hash_algo)));
					استمر;
				}
				/* set ref to commit using oid, not mark */
				لکو("reset %s\nfrom %s\n\n", name,
				       oid_منزل_hex(&commit->object.oid));
				استمر;
			}

			لکو("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_ترقي();
			اكسر;
		}
	}
}

ساكن فراغ export_marks(حرف *file)
{
	بنانشان صحيح i;
	uint32_t mark;
	بنية decoration_داخلا *deco = idnums.entries;
	FILE *f;
	صحيح e = 0;

	f = fopen_for_writing(file);
	إذا (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	لكل (i = 0; i < idnums.سائيز; i++) {
		إذا (deco->بنياد && deco->بنياد->type == 1) {
			mark = ptr_منزل_mark(deco->decoration);
			إذا (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_منزل_hex(&deco->بنياد->oid)) < 0) {
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

ساكن فراغ import_marks(حرف *داخلا_file, صحيح check_exists)
{
	حرف سٽ[512];
	FILE *f;
	بنية stat sb;

	إذا (check_exists && stat(داخلا_file, &sb))
		أرجع;

	f = xfopen(داخلا_file, "r");
	طالما (fgets(سٽ, حجم(سٽ), f)) {
		uint32_t mark;
		حرف *سٽ_پڇاڙي, *mark_پڇاڙي;
		بنية object_id oid;
		بنية object *object;
		بنية commit *commit;
		تعداد object_type type;

		سٽ_پڇاڙي = strchr(سٽ, '\n');
		إذا (سٽ[0] != ':' || !سٽ_پڇاڙي)
			die(_("corrupt mark line: %s"), سٽ);
		*سٽ_پڇاڙي = '\0';

		mark = strtoumax(سٽ + 1, &mark_پڇاڙي, 10);
		إذا (!mark || mark_پڇاڙي == سٽ + 1
			|| *mark_پڇاڙي != ' ' || get_oid_hex(mark_پڇاڙي + 1, &oid))
			die(_("corrupt mark line: %s"), سٽ);

		إذا (آخري_idnum < mark)
			آخري_idnum = mark;

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
		بنية refspec_شي *refspec = &refspecs.شيون[i];
		إذا (*refspec->src)
			استمر;

		لکو("reset %s\nfrom %s\n\n",
				refspec->dst, oid_منزل_hex(null_oid(the_hash_algo)));
	}
}

ساكن صحيح parse_opt_anonymize_map(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	بنية hashmap *map = opt->قدر;
	ثابت حرف *delim, *قدر;
	سائيز_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	إذا (delim) {
		keylen = delim - arg;
		قدر = delim + 1;
	} وإلا {
		keylen = لڙي_ڊيگهه(arg);
		قدر = arg;
	}

	إذا (!keylen || !*قدر)
		أرجع error(_("--anonymize-map token cannot be empty"));

	جوڙيو_anonymized_داخلا(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(قدر));

	أرجع 0;
}

صحيح cmd_fast_export(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *اڳياڙي,
		    بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية rev_info revs;
	بنية commit *commit;
	حرف *export_filename = عدم,
	     *import_filename = عدم,
	     *import_filename_if_exists = عدم;
	uint32_t lastimportid;
	بنية string_list refspecs_list = STRING_LIST_INIT_NODUP;
	بنية string_list paths_جو_بدليو_objects = STRING_LIST_INIT_DUP;
	بنية option options[] = {
		OPT_INTEGER(0, "progress", &ترقي,
			    N_("show progress after <n> objects")),
		OPT_CALLBACK(0, "signed-tags", &signed_ٽيگ_گهڻائي, N_("mode"),
			     N_("select handling of signed tags"),
			     parse_opt_sign_گهڻائي),
		OPT_CALLBACK(0, "signed-commits", &signed_commit_گهڻائي, N_("mode"),
			     N_("select handling of signed commits"),
			     parse_opt_sign_گهڻائي),
		OPT_CALLBACK(0, "tag-of-filtered-object", &ٽيگ_جو_filtered_گهڻائي, N_("mode"),
			     N_("select handling of tags that tag filtered objects"),
			     parse_opt_ٽيگ_جو_filtered_گهڻائي),
		OPT_CALLBACK(0, "reencode", &reencode_گهڻائي, N_("mode"),
			     N_("select handling of commit messages in an alternate encoding"),
			     parse_opt_reencode_گهڻائي),
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
		OPT_BOOL(0, "full-tree", &ڀريل_وڻ,
			 N_("output full tree for each commit")),
		OPT_BOOL(0, "use-done-feature", &use_پورو_feature,
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
		OPT_BOOL(0, "mark-tags", &mark_ٽيگ_فهرست,
			    N_("label tags with mark ids")),

		OPT_پڇاڙي()
	};

	إذا (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, عدم);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, اڳياڙي);
	init_revision_sources(&revision_sources);
	revs.topo_ترتيب = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, اڳياڙي, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = تياري_revisions(argc, argv, &revs, عدم);
	إذا (argc > 1)
		usage_with_options (fast_export_usage, options);

	إذا (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	إذا (refspecs_list.nr) {
		صحيح i;

		لكل (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.شيون[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	إذا (use_پورو_feature)
		لکو("feature done\n");

	إذا (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	إذا (import_filename)
		import_marks(import_filename, 0);
	وإلا إذا (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = آخري_idnum;

	إذا (import_filename && revs.prune_معلومات.nr)
		ڀريل_وڻ = 1;

	get_ٽيگ_فهرست_and_duplicates(&revs.cmdline);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.ابتو = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_معلومات = &paths_جو_بدليو_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	طالما ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_جو_بدليو_objects);
	revs.diffopt.no_free = 0;

	handle_ٽيگ_فهرست_and_duplicates(&extra_refs);
	handle_ٽيگ_فهرست_and_duplicates(&ٽيگ_refs);
	handle_deletes();

	إذا (export_filename && lastimportid != آخري_idnum)
		export_marks(export_filename);

	إذا (use_پورو_feature)
		لکو("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	أرجع 0;
}
