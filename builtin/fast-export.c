/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "refs.h"
#شامل "refspec.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "commit.h"
#شامل "object.h"
#شامل "tag.h"
#شامل "diff.h"
#شامل "diffcore.h"
#شامل "log-tree.h"
#شامل "revision.h"
#شامل "decorate.h"
#شامل "string-list.h"
#شامل "utf8.h"
#شامل "parse-options.h"
#شامل "quote.h"
#شامل "remote.h"
#شامل "blob.h"
#شامل "gpg-interface.h"

ساکن ثابت حرف *ثابت fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	تہی
};

ساکن صحیح پیش_رفت;
ساکن شمار sign_منوال signed_لیبل_منوال = SIGN_ABORT;
ساکن شمار sign_منوال signed_commit_منوال = SIGN_STRIP;
ساکن شمار لیبل_کا_filtered_منوال { لیبل_FILTERING_ABORT, DROP, REWRITE } لیبل_کا_filtered_منوال = لیبل_FILTERING_ABORT;
ساکن شمار reencode_منوال { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_منوال = REENCODE_ABORT;
ساکن صحیح fake_missing_tagger;
ساکن صحیح use_مکمل_feature;
ساکن صحیح no_ڈیٹا;
ساکن صحیح بھرا_درخت;
ساکن صحیح reference_excluded_commits;
ساکن صحیح show_original_ids;
ساکن صحیح mark_لیبل_فہرست;
ساکن ساخت string_list extra_refs = STRING_LIST_INIT_DUP;
ساکن ساخت string_list لیبل_refs = STRING_LIST_INIT_DUP;
ساکن ساخت refspec refspecs;
ساکن صحیح anonymize;
ساکن ساخت hashmap anonymized_seeds;
ساکن ساخت revision_sources revision_sources;

ساکن صحیح parse_opt_sign_منوال(ثابت ساخت option *opt,
			       ثابت حرف *arg, صحیح unset)
{
	شمار sign_منوال *val = opt->قدر;

	اگر (unset)
		واپس 0;

	اگر (parse_sign_منوال(arg, val, تہی) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		واپس error(_("unknown %s mode: %s"), opt->long_name, arg);

	واپس 0;
}

ساکن صحیح parse_opt_لیبل_کا_filtered_منوال(ثابت ساخت option *opt,
					  ثابت حرف *arg, صحیح unset)
{
	شمار لیبل_کا_filtered_منوال *val = opt->قدر;

	اگر (unset || !strcmp(arg, "abort"))
		*val = لیبل_FILTERING_ABORT;
	ورنہ اگر (!strcmp(arg, "drop"))
		*val = DROP;
	ورنہ اگر (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	ورنہ
		واپس error(_("unknown tag-of-filtered mode: %s"), arg);
	واپس 0;
}

ساکن صحیح parse_opt_reencode_منوال(ثابت ساخت option *opt,
				   ثابت حرف *arg, صحیح unset)
{
	شمار reencode_منوال *val = opt->قدر;

	اگر (unset) {
		*val = REENCODE_ABORT;
		واپس 0;
	}

	انتخاب (git_parse_maybe_bool(arg)) {
	حالت 0:
		*val = REENCODE_NO;
		توڑو;
	حالت 1:
		*val = REENCODE_YES;
		توڑو;
	طےشدہ:
		اگر (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		ورنہ
			واپس error(_("unknown reencoding mode: %s"), arg);
	}

	واپس 0;
}

ساکن ساخت decoration idnums;
ساکن uint32_t آخری_idnum;
ساخت anonymized_اندراج {
	ساخت hashmap_اندراج hash;
	حرف *anon;
	ثابت حرف orig[FLEX_ARRAY];
};

ساخت anonymized_اندراج_کلید {
	ساخت hashmap_اندراج hash;
	ثابت حرف *orig;
	سائز_t orig_len;
};

ساکن صحیح anonymized_اندراج_cmp(ثابت خالی *cmp_ڈیٹا غیر_مستعمل,
				ثابت ساخت hashmap_اندراج *eptr,
				ثابت ساخت hashmap_اندراج *اندراج_or_کلید,
				ثابت خالی *keydata)
{
	ثابت ساخت anonymized_اندراج *a, *b;

	a = container_کا(eptr, ثابت ساخت anonymized_اندراج, hash);
	اگر (keydata) {
		ثابت ساخت anonymized_اندراج_کلید *کلید = keydata;
		صحیح equal = !xstrncmpz(a->orig, کلید->orig, کلید->orig_len);
		واپس !equal;
	}

	b = container_کا(اندراج_or_کلید, ثابت ساخت anonymized_اندراج, hash);
	واپس strcmp(a->orig, b->orig);
}

ساکن ساخت anonymized_اندراج *جوڑو_anonymized_اندراج(ساخت hashmap *map,
						     غیرعلامتی hash,
						     ثابت حرف *orig, سائز_t len,
						     حرف *anon)
{
	ساخت anonymized_اندراج *ret, *old;

	اگر (!map->cmpfn)
		hashmap_init(map, anonymized_اندراج_cmp, تہی, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_اندراج_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_اندراج(map, ret, hash);

	اگر (old) {
		آزاد(old->anon);
		آزاد(old);
	}

	واپس ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
ساکن ثابت حرف *anonymize_str(ساخت hashmap *map,
				 حرف *(*generate)(خالی),
				 ثابت حرف *orig, سائز_t len)
{
	ساخت anonymized_اندراج_کلید کلید;
	ساخت anonymized_اندراج *ret;

	hashmap_اندراج_init(&کلید.hash, memhash(orig, len));
	کلید.orig = orig;
	کلید.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_اندراج(&anonymized_seeds, &کلید, hash, &کلید);

	/* ...otherwise check if we've already seen it in this context... */
	اگر (!ret)
		ret = hashmap_get_اندراج(map, &کلید, hash, &کلید);

	/* ...and finally generate a new mapping if necessary */
	اگر (!ret)
		ret = جوڑو_anonymized_اندراج(map, کلید.hash.hash,
					   orig, len, generate());

	واپس ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
ساکن خالی anonymize_راستہ(ساخت strbuf *برآمد, ثابت حرف *راستہ,
			   ساخت hashmap *map,
			   حرف *(*generate)(خالی))
{
	جب (*راستہ) {
		ثابت حرف *انتہا_کا_component = strchrnul(راستہ, '/');
		سائز_t len = انتہا_کا_component - راستہ;
		ثابت حرف *c = anonymize_str(map, generate, راستہ, len);
		strbuf_addstr(برآمد, c);
		راستہ = انتہا_کا_component;
		اگر (*راستہ)
			strbuf_addch(برآمد, *راستہ++);
	}
}

ساکن اندرونی خالی *mark_مقصد_ptr(uint32_t mark)
{
	واپس (خالی *)(uintptr_t)mark;
}

ساکن اندرونی uint32_t ptr_مقصد_mark(خالی * mark)
{
	واپس (uint32_t)(uintptr_t)mark;
}

ساکن اندرونی خالی mark_object(ساخت object *object, uint32_t mark)
{
	جوڑو_decoration(&idnums, object, mark_مقصد_ptr(mark));
}

ساکن اندرونی خالی mark_اگلا_object(ساخت object *object)
{
	mark_object(object, ++آخری_idnum);
}

ساکن صحیح get_object_mark(ساخت object *object)
{
	خالی *decoration = lookup_decoration(&idnums, object);
	اگر (!decoration)
		واپس 0;
	واپس ptr_مقصد_mark(decoration);
}

ساکن ساخت commit *rewrite_commit(ساخت commit *p)
{
	برائے (;;) {
		اگر (p->parents && p->parents->اگلا)
			توڑو;
		اگر (p->object.flags & UNINTERESTING)
			توڑو;
		اگر (!(p->object.flags & TREESAME))
			توڑو;
		اگر (!p->parents)
			واپس تہی;
		p = p->parents->شے;
	}
	واپس p;
}

ساکن خالی show_پیش_رفت(خالی)
{
	ساکن صحیح شمار_2 = 0;
	اگر (!پیش_رفت)
		واپس;
	اگر ((++شمار_2 % پیش_رفت) == 0)
		لکھو("progress %d objects\n", شمار_2);
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
ساکن حرف *anonymize_blob(غیرعلامتی طویل *سائز)
{
	ساکن صحیح شمار_2;
	ساخت strbuf برآمد = STRBUF_INIT;
	strbuf_addf(&برآمد, "anonymous blob %d", شمار_2++);
	*سائز = برآمد.len;
	واپس strbuf_detach(&برآمد, تہی);
}

ساکن خالی export_blob(ثابت ساخت object_id *oid)
{
	غیرعلامتی طویل سائز;
	شمار object_type type;
	حرف *buf;
	ساخت object *object;
	صحیح eaten;

	اگر (no_ڈیٹا)
		واپس;

	اگر (is_null_oid(oid))
		واپس;

	object = lookup_object(the_repository, oid);
	اگر (object && object->flags & SHOWN)
		واپس;

	اگر (anonymize) {
		buf = anonymize_blob(&سائز);
		object = (ساخت object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} ورنہ {
		سائز_t سائز_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &سائز_st);
		سائز = cast_سائز_t_مقصد_ulong(سائز_st);
		اگر (!buf)
			die(_("could not read blob %s"), oid_مقصد_hex(oid));
		اگر (check_object_signature(the_repository, oid, buf, سائز,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_مقصد_hex(oid));
		object = parse_object_بفر(the_repository, oid, type,
					     سائز, buf, &eaten);
	}

	اگر (!object)
		die(_("could not read blob %s"), oid_مقصد_hex(oid));

	mark_اگلا_object(object);

	لکھو("blob\nmark :%"PRIu32"\n", آخری_idnum);
	اگر (show_original_ids)
		لکھو("original-oid %s\n", oid_مقصد_hex(oid));
	لکھو("data %"PRIuMAX"\n", (uintmax_t)سائز);
	اگر (سائز && fwrite(buf, سائز, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_مقصد_hex(oid));
	لکھو("\n");

	show_پیش_رفت();

	object->flags |= SHOWN;
	اگر (!eaten)
		آزاد(buf);
}

ساکن صحیح گہرائی_پہلا(ثابت خالی *a_, ثابت خالی *b_)
{
	ثابت ساخت diff_filepair *a = *((ثابت ساخت diff_filepair **)a_);
	ثابت ساخت diff_filepair *b = *((ثابت ساخت diff_filepair **)b_);
	ثابت حرف *name_a, *name_b;
	صحیح len_a, len_b, len;
	صحیح cmp;

	name_a = a->one ? a->one->راستہ : a->two->راستہ;
	name_b = b->one ? b->one->راستہ : b->two->راستہ;

	len_a = لڑی_لمبائی(name_a);
	len_b = لڑی_لمبائی(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	اگر (cmp)
		واپس cmp;
	cmp = len_b - len_a;
	اگر (cmp)
		واپس cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	واپس (a->حالت_2 == 'R') - (b->حالت_2 == 'R');
}

ساکن خالی print_راستہ_1(ثابت حرف *راستہ)
{
	صحیح need_quote = quote_c_style(راستہ, تہی, تہی, 0);
	اگر (need_quote)
		quote_c_style(راستہ, تہی, stdout, 0);
	ورنہ اگر (strchr(راستہ, ' '))
		لکھو("\"%s\"", راستہ);
	ورنہ
		لکھو("%s", راستہ);
}

ساکن حرف *anonymize_راستہ_component(خالی)
{
	ساکن صحیح شمار_2;
	ساخت strbuf برآمد = STRBUF_INIT;
	strbuf_addf(&برآمد, "path%d", شمار_2++);
	واپس strbuf_detach(&برآمد, تہی);
}

ساکن خالی print_راستہ(ثابت حرف *راستہ)
{
	اگر (!anonymize)
		print_راستہ_1(راستہ);
	ورنہ {
		ساکن ساخت hashmap paths;
		ساکن ساخت strbuf anon = STRBUF_INIT;

		anonymize_راستہ(&anon, راستہ, &paths, anonymize_راستہ_component);
		print_راستہ_1(anon.buf);
		strbuf_reset(&anon);
	}
}

ساکن حرف *generate_fake_oid(خالی)
{
	ساکن uint32_t شمار_2 = 1; /* avoid null oid */
	ثابت غیرعلامتی hashsz = the_hash_algo->rawsz;
	ساخت object_id oid;
	حرف *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, شمار_2++);
	واپس oid_مقصد_hex_r(hex, &oid);
}

ساکن ثابت حرف *anonymize_oid(ثابت حرف *oid_hex)
{
	ساکن ساخت hashmap objs;
	سائز_t len = لڑی_لمبائی(oid_hex);
	واپس anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

ساکن خالی show_filemodify(ساخت diff_قطار_struct *q,
			    ساخت diff_options *options غیر_مستعمل, خالی *ڈیٹا)
{
	صحیح i;
	ساخت string_list *بدلا = ڈیٹا;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->قطار, q->nr, گہرائی_پہلا);

	برائے (i = 0; i < q->nr; i++) {
		ساخت diff_filespec *ospec = q->قطار[i]->one;
		ساخت diff_filespec *spec = q->قطار[i]->two;

		انتخاب (q->قطار[i]->حالت_2) {
		حالت DIFF_حالت_DELETED:
			لکھو("D ");
			print_راستہ(spec->راستہ);
			string_list_داخل_کرو(بدلا, spec->راستہ);
			حرف_لکھو('\n');
			توڑو;

		حالت DIFF_حالت_COPIED:
		حالت DIFF_حالت_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			اگر (!string_list_has_string(بدلا, ospec->راستہ)) {
				لکھو("%c ", q->قطار[i]->حالت_2);
				print_راستہ(ospec->راستہ);
				حرف_لکھو(' ');
				print_راستہ(spec->راستہ);
				string_list_داخل_کرو(بدلا, spec->راستہ);
				حرف_لکھو('\n');

				اگر (oideq(&ospec->oid, &spec->oid) &&
				    ospec->منوال == spec->منوال)
					توڑو;
			}
			/* fallthrough */

		حالت DIFF_حالت_TYPE_بدلا:
		حالت DIFF_حالت_MODIFIED:
		حالت DIFF_حالت_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			اگر (no_ڈیٹا || S_ISGITLINK(spec->منوال))
				لکھو("M %06o %s ", spec->منوال,
				       anonymize ?
				       anonymize_oid(oid_مقصد_hex(&spec->oid)) :
				       oid_مقصد_hex(&spec->oid));
			ورنہ {
				ساخت object *object = lookup_object(the_repository,
								      &spec->oid);
				لکھو("M %06o :%d ", spec->منوال,
				       get_object_mark(object));
			}
			print_راستہ(spec->راستہ);
			string_list_داخل_کرو(بدلا, spec->راستہ);
			حرف_لکھو('\n');
			توڑو;

		طےشدہ:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->قطار[i]->حالت_2,
			    ospec->راستہ ? ospec->راستہ : _("none"),
			    spec->راستہ ? spec->راستہ : _("none"));
		}
	}
}

ساکن حرف *anonymize_ref_component(خالی)
{
	ساکن صحیح شمار_2;
	ساخت strbuf برآمد = STRBUF_INIT;
	strbuf_addf(&برآمد, "ref%d", شمار_2++);
	واپس strbuf_detach(&برآمد, تہی);
}

ساکن ثابت حرف *anonymize_refname(ثابت حرف *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	ساکن ثابت حرف *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	ساکن ساخت hashmap refs;
	ساکن ساخت strbuf anon = STRBUF_INIT;
	صحیح i;

	strbuf_reset(&anon);
	برائے (i = 0; i < ARRAY_سائز(prefixes); i++) {
		اگر (skip_سابقہ(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			توڑو;
		}
	}

	anonymize_راستہ(&anon, refname, &refs, anonymize_ref_component);
	واپس anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
ساکن حرف *anonymize_commit_پیغام(خالی)
{
	ساکن صحیح شمار_2;
	واپس xstrfmt("subject %d\n\nbody\n", شمار_2++);
}

ساکن حرف *anonymize_ident(خالی)
{
	ساکن صحیح شمار_2;
	ساخت strbuf برآمد = STRBUF_INIT;
	strbuf_addf(&برآمد, "User %d <user%d@example.com>", شمار_2, شمار_2);
	شمار_2++;
	واپس strbuf_detach(&برآمد, تہی);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
ساکن خالی anonymize_ident_سطر(ثابت حرف **beg, ثابت حرف **انتہا)
{
	ساکن ساخت hashmap idents;
	ساکن ساخت strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	ساکن غیرعلامتی which_بفر;

	ساخت strbuf *برآمد;
	ساخت ident_بانٹو بانٹو;
	ثابت حرف *انتہا_کا_سرخی;

	برآمد = &buffers[which_بفر++];
	which_بفر %= ARRAY_سائز(buffers);
	strbuf_reset(برآمد);

	/* skip "committer", "author", "tagger", etc */
	انتہا_کا_سرخی = strchr(*beg, ' ');
	اگر (!انتہا_کا_سرخی)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (صحیح)(*انتہا - *beg), *beg);
	انتہا_کا_سرخی++;
	strbuf_جوڑو(برآمد, *beg, انتہا_کا_سرخی - *beg);

	اگر (!بانٹو_ident_سطر(&بانٹو, انتہا_کا_سرخی, *انتہا - انتہا_کا_سرخی) &&
	    بانٹو.date_begin) {
		ثابت حرف *ident;
		سائز_t len;

		len = بانٹو.mail_انتہا - بانٹو.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      بانٹو.name_begin, len);
		strbuf_addstr(برآمد, ident);
		strbuf_addch(برآمد, ' ');
		strbuf_جوڑو(برآمد, بانٹو.date_begin, بانٹو.tz_انتہا - بانٹو.date_begin);
	} ورنہ {
		strbuf_addstr(برآمد, "Malformed Ident <malformed@example.com> 0 -0000");
	}

	*beg = برآمد->buf;
	*انتہا = برآمد->buf + برآمد->len;
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
ساکن ثابت حرف *ڈھونڈو_commit_multiline_سرخی(ثابت حرف *پیغام,
						ثابت حرف *کلید,
						ثابت حرف **انتہا)
{
	ساخت strbuf val = STRBUF_INIT;
	ثابت حرف *bol, *eol;
	سائز_t len;

	bol = ڈھونڈو_commit_سرخی(پیغام, کلید, &len);
	اگر (!bol)
		واپس تہی;
	eol = bol + len;
	strbuf_جوڑو(&val, bol, len);

	جب (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_جوڑو(&val, bol, eol - bol);
	}

	*انتہا = eol;
	واپس strbuf_detach(&val, تہی);
}

ساکن خالی print_signature(ثابت حرف *signature, ثابت حرف *object_hash)
{
	اگر (!signature)
		واپس;

	لکھو("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (غیرعلامتی)لڑی_لمبائی(signature),
	       signature);
}

ساکن ثابت حرف *append_signatures_for_سرخی(ساخت string_list *signatures,
						ثابت حرف *pos,
						ثابت حرف *سرخی,
						ثابت حرف *object_hash)
{
	ثابت حرف *signature;
	ثابت حرف *آغاز = pos;
	ثابت حرف *انتہا = pos;

	جب ((signature = ڈھونڈو_commit_multiline_سرخی(آغاز + 1,
							 سرخی,
							 &انتہا))) {
		string_list_append(signatures, signature)->util = (خالی *)object_hash;
		آزاد((حرف *)signature);
		آغاز = انتہا;
	}

	واپس انتہا;
}

ساکن خالی handle_commit(ساخت commit *commit, ساخت rev_info *rev,
			  ساخت string_list *paths_کا_بدلا_objects)
{
	صحیح saved_output_format = rev->diffopt.output_format;
	ثابت حرف *commit_بفر, *commit_بفر_cursor;
	ثابت حرف *author, *author_انتہا, *committer, *committer_انتہا;
	ثابت حرف *encoding = تہی;
	سائز_t encoding_len;
	ساخت string_list signatures = STRING_LIST_INIT_DUP;
	ثابت حرف *پیغام_2;
	حرف *reencoded = تہی;
	ساخت commit_list *p;
	ثابت حرف *refname;
	صحیح i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_بفر_cursor = commit_بفر = repo_get_commit_بفر(the_repository, commit, تہی);

	author = لڑی_تلاش(commit_بفر_cursor, "\nauthor ");
	اگر (!author)
		die(_("could not find author in commit %s"),
		    oid_مقصد_hex(&commit->object.oid));
	author++;
	commit_بفر_cursor = author_انتہا = strchrnul(author, '\n');

	committer = لڑی_تلاش(commit_بفر_cursor, "\ncommitter ");
	اگر (!committer)
		die(_("could not find committer in commit %s"),
		    oid_مقصد_hex(&commit->object.oid));
	committer++;
	commit_بفر_cursor = committer_انتہا = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	اگر (*commit_بفر_cursor == '\n') {
		encoding = ڈھونڈو_commit_سرخی(commit_بفر_cursor + 1, "encoding", &encoding_len);
		اگر (encoding)
			commit_بفر_cursor = encoding + encoding_len;
	}

	اگر (*commit_بفر_cursor == '\n') {
		ثابت حرف *after_sha1 = append_signatures_for_سرخی(&signatures, commit_بفر_cursor,
								      "gpgsig", "sha1");
		ثابت حرف *after_sha256 = append_signatures_for_سرخی(&signatures, commit_بفر_cursor,
									"gpgsig-sha256", "sha256");
		commit_بفر_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	پیغام_2 = لڑی_تلاش(commit_بفر_cursor, "\n\n");
	اگر (پیغام_2)
		پیغام_2 += 2;

	اگر (commit->parents &&
	    (get_object_mark(&commit->parents->شے->object) != 0 ||
	     reference_excluded_commits) &&
	    !بھرا_درخت) {
		parse_commit_or_die(commit->parents->شے);
		diff_درخت_oid(get_commit_درخت_oid(commit->parents->شے),
			      get_commit_درخت_oid(commit), "", &rev->diffopt);
	}
	ورنہ
		diff_جڑ_درخت_oid(get_commit_درخت_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	برائے (i = 0; i < diff_queued_diff.nr; i++)
		اگر (!S_ISGITLINK(diff_queued_diff.قطار[i]->two->منوال))
			export_blob(&diff_queued_diff.قطار[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	اگر (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_سطر(&committer, &committer_انتہا);
		anonymize_ident_سطر(&author, &author_انتہا);
	}

	mark_اگلا_object(&commit->object);
	اگر (anonymize) {
		reencoded = anonymize_commit_پیغام();
	} ورنہ اگر (encoding) {
		حرف *buf;
		انتخاب (reencode_منوال) {
		حالت REENCODE_YES:
			buf = xstrfmt("%.*s", (صحیح)encoding_len, encoding);
			reencoded = reencode_string(پیغام_2, "UTF-8", buf);
			آزاد(buf);
			توڑو;
		حالت REENCODE_NO:
			توڑو;
		حالت REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (صحیح)encoding_len, encoding,
			    oid_مقصد_hex(&commit->object.oid));
		}
	}
	اگر (!commit->parents)
		لکھو("reset %s\n", refname);
	لکھو("commit %s\nmark :%"PRIu32"\n", refname, آخری_idnum);
	اگر (show_original_ids)
		لکھو("original-oid %s\n", oid_مقصد_hex(&commit->object.oid));
	لکھو("%.*s\n%.*s\n",
	       (صحیح)(author_انتہا - author), author,
	       (صحیح)(committer_انتہا - committer), committer);
	اگر (signatures.nr) {
		انتخاب (signed_commit_منوال) {
		/* Exporting modes */
		حالت SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_مقصد_hex(&commit->object.oid));
			/* fallthru */
		حالت SIGN_VERBATIM:
			برائے (سائز_t i = 0; i < signatures.nr; i++) {
				ساخت string_list_شے *شے = &signatures.اشیاء[i];
				print_signature(شے->string, شے->util);
			}
			توڑو;

		/* Stripping modes */
		حالت SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_مقصد_hex(&commit->object.oid));
			/* fallthru */
		حالت SIGN_STRIP:
			توڑو;

		/* Aborting modes */
		حالت SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_مقصد_hex(&commit->object.oid));
		طےشدہ:
			BUG("invalid signed_commit_mode value %d", signed_commit_منوال);
		}
		string_list_clear(&signatures, 0);
	}
	اگر (!reencoded && encoding)
		لکھو("encoding %.*s\n", (صحیح)encoding_len, encoding);
	لکھو("data %u\n%s",
	       (غیرعلامتی)(reencoded
			  ? لڑی_لمبائی(reencoded) : پیغام_2
			  ? لڑی_لمبائی(پیغام_2) : 0),
	       reencoded ? reencoded : پیغام_2 ? پیغام_2 : "");
	آزاد(reencoded);
	repo_unuse_commit_بفر(the_repository, commit, commit_بفر);

	برائے (i = 0, p = commit->parents; p; p = p->اگلا) {
		ساخت object *obj = &p->شے->object;
		صحیح mark = get_object_mark(obj);

		اگر (!mark && !reference_excluded_commits)
			جاری;
		اگر (i == 0)
			لکھو("from ");
		ورنہ
			لکھو("merge ");
		اگر (mark)
			لکھو(":%d\n", mark);
		ورنہ
			لکھو("%s\n",
			       anonymize ?
			       anonymize_oid(oid_مقصد_hex(&obj->oid)) :
			       oid_مقصد_hex(&obj->oid));
		i++;
	}

	اگر (بھرا_درخت)
		لکھو("deleteall\n");
	log_درخت_diff_flush(rev);
	string_list_clear(paths_کا_بدلا_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	لکھو("\n");

	show_پیش_رفت();
}

ساکن حرف *anonymize_لیبل(خالی)
{
	ساکن صحیح شمار_2;
	ساخت strbuf برآمد = STRBUF_INIT;
	strbuf_addf(&برآمد, "tag message %d", شمار_2++);
	واپس strbuf_detach(&برآمد, تہی);
}


ساکن خالی handle_لیبل(ثابت حرف *name, ساخت لیبل *لیبل)
{
	سائز_t سائز;
	شمار object_type type;
	حرف *buf;
	ثابت حرف *tagger, *tagger_انتہا, *پیغام_2;
	سائز_t پیغام_سائز = 0;
	ساخت object *tagged;
	صحیح tagged_mark;
	ساخت commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = لیبل->tagged;
	جب (tagged->type == OBJ_لیبل) {
		tagged = ((ساخت لیبل *)tagged)->tagged;
	}
	اگر (tagged->type == OBJ_درخت) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_مقصد_hex(&لیبل->object.oid));
		واپس;
	}

	buf = odb_read_object(the_repository->objects, &لیبل->object.oid,
			      &type, &سائز);
	اگر (!buf)
		die(_("could not read tag %s"), oid_مقصد_hex(&لیبل->object.oid));
	پیغام_2 = memmem(buf, سائز, "\n\n", 2);
	اگر (پیغام_2) {
		پیغام_2 += 2;
		پیغام_سائز = لڑی_لمبائی(پیغام_2);
	}
	tagger = memmem(buf, پیغام_2 ? پیغام_2 - buf : سائز, "\ntagger ", 8);
	اگر (!tagger) {
		اگر (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		ورنہ
			tagger = "";
		tagger_انتہا = tagger + لڑی_لمبائی(tagger);
	} ورنہ {
		tagger++;
		tagger_انتہا = strchrnul(tagger, '\n');
		اگر (anonymize)
			anonymize_ident_سطر(&tagger, &tagger_انتہا);
	}

	اگر (anonymize) {
		name = anonymize_refname(name);
		اگر (پیغام_2) {
			ساکن ساخت hashmap لیبل_فہرست;
			پیغام_2 = anonymize_str(&لیبل_فہرست, anonymize_لیبل,
						پیغام_2, پیغام_سائز);
			پیغام_سائز = لڑی_لمبائی(پیغام_2);
		}
	}

	/* handle signed tags */
	اگر (پیغام_2) {
		سائز_t sig_انحراف = parse_signed_بفر(پیغام_2, پیغام_سائز);
		اگر (sig_انحراف < پیغام_سائز)
			انتخاب (signed_لیبل_منوال) {
			/* Exporting modes */
			حالت SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_مقصد_hex(&لیبل->object.oid));
				/* fallthru */
			حالت SIGN_VERBATIM:
				توڑو;

			/* Stripping modes */
			حالت SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_مقصد_hex(&لیبل->object.oid));
				/* fallthru */
			حالت SIGN_STRIP:
				پیغام_سائز = sig_انحراف;
				توڑو;

			/* Aborting modes */
			حالت SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_مقصد_hex(&لیبل->object.oid));
			طےشدہ:
				BUG("invalid signed_commit_mode value %d", signed_commit_منوال);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = لیبل->tagged;
	tagged_mark = get_object_mark(tagged);
	اگر (!tagged_mark) {
		انتخاب (لیبل_کا_filtered_منوال) {
		حالت لیبل_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_مقصد_hex(&لیبل->object.oid));
		حالت DROP:
			/* Ignore this tag altogether */
			آزاد(buf);
			واپس;
		حالت REWRITE:
			اگر (tagged->type == OBJ_لیبل && !mark_لیبل_فہرست) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} ورنہ اگر (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((ساخت commit *)tagged);
				اگر (!p) {
					لکھو("reset %s\nfrom %s\n\n",
					       name, oid_مقصد_hex(null_oid(the_hash_algo)));
					آزاد(buf);
					واپس;
				}
				tagged_mark = get_object_mark(&p->object);
			} ورنہ {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	اگر (tagged->type == OBJ_لیبل) {
		لکھو("reset %s\nfrom %s\n\n",
		       name, oid_مقصد_hex(null_oid(the_hash_algo)));
	}
	skip_سابقہ(name, "refs/tags/", &name);
	لکھو("tag %s\n", name);
	اگر (mark_لیبل_فہرست) {
		mark_اگلا_object(&لیبل->object);
		لکھو("mark :%"PRIu32"\n", آخری_idnum);
	}
	اگر (tagged_mark)
		لکھو("from :%d\n", tagged_mark);
	ورنہ
		لکھو("from %s\n", oid_مقصد_hex(&tagged->oid));

	اگر (show_original_ids)
		لکھو("original-oid %s\n", oid_مقصد_hex(&لیبل->object.oid));
	لکھو("%.*s%sdata %d\n%.*s\n",
	       (صحیح)(tagger_انتہا - tagger), tagger,
	       tagger == tagger_انتہا ? "" : "\n",
	       (صحیح)پیغام_سائز, (صحیح)پیغام_سائز, پیغام_2 ? پیغام_2 : "");
	آزاد(buf);
}

ساکن ساخت commit *get_commit(ساخت rev_cmdline_اندراج *e, ثابت حرف *بھرا_name)
{
	انتخاب (e->شے->type) {
	حالت OBJ_COMMIT:
		واپس (ساخت commit *)e->شے;
	حالت OBJ_لیبل: {
		ساخت لیبل *لیبل = (ساخت لیبل *)e->شے;

		/* handle nested tags */
		جب (لیبل && لیبل->object.type == OBJ_لیبل) {
			parse_object(the_repository, &لیبل->object.oid);
			string_list_append(&لیبل_refs, بھرا_name)->util = لیبل;
			لیبل = (ساخت لیبل *)لیبل->tagged;
		}
		اگر (!لیبل)
			die(_("tag %s points nowhere?"), e->name);
		واپس (ساخت commit *)لیبل;
	}
	طےشدہ:
		واپس تہی;
	}
}

ساکن خالی get_لیبل_فہرست_and_duplicates(ساخت rev_cmdline_info *info)
{
	صحیح i;

	برائے (i = 0; i < info->nr; i++) {
		ساخت rev_cmdline_اندراج *e = info->rev + i;
		ساخت object_id oid;
		ساخت commit *commit;
		حرف *بھرا_name = تہی;

		اگر (e->flags & UNINTERESTING)
			جاری;

		اگر (repo_dwim_ref(the_repository, e->name, لڑی_لمبائی(e->name),
				  &oid, &بھرا_name, 0) != 1) {
			آزاد(بھرا_name);
			جاری;
		}

		اگر (refspecs.nr) {
			حرف *نجی;
			نجی = apply_refspecs(&refspecs, بھرا_name);
			اگر (نجی) {
				آزاد(بھرا_name);
				بھرا_name = نجی;
			}
		}

		commit = get_commit(e, بھرا_name);
		اگر (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->شے->type));
			آزاد(بھرا_name);
			جاری;
		}

		انتخاب (commit->object.type) {
		حالت OBJ_COMMIT:
			توڑو;
		حالت OBJ_BLOB:
			export_blob(&commit->object.oid);
			آزاد(بھرا_name);
			جاری;
		طےشدہ: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			آزاد(بھرا_name);
			جاری;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		اگر (e->شے->type != OBJ_لیبل)
			string_list_append(&extra_refs, بھرا_name)->util = commit;

		اگر (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = بھرا_name;
		ورنہ
			آزاد(بھرا_name);
	}

	string_list_ترتیب_دو_u(&extra_refs, 0);
}

ساکن خالی handle_لیبل_فہرست_and_duplicates(ساخت string_list *extras)
{
	ساخت commit *commit;
	صحیح i;

	برائے (i = extras->nr - 1; i >= 0; i--) {
		ثابت حرف *name = extras->اشیاء[i].string;
		ساخت object *object = extras->اشیاء[i].util;
		صحیح mark;

		انتخاب (object->type) {
		حالت OBJ_لیبل:
			handle_لیبل(name, (ساخت لیبل *)object);
			توڑو;
		حالت OBJ_COMMIT:
			اگر (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((ساخت commit *)object);
			اگر (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				لکھو("reset %s\nfrom %s\n\n",
				       name, oid_مقصد_hex(null_oid(the_hash_algo)));
				جاری;
			}

			mark = get_object_mark(&commit->object);
			اگر (!mark) {
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
				اگر (!reference_excluded_commits) {
					/* delete the ref */
					لکھو("reset %s\nfrom %s\n\n",
					       name, oid_مقصد_hex(null_oid(the_hash_algo)));
					جاری;
				}
				/* set ref to commit using oid, not mark */
				لکھو("reset %s\nfrom %s\n\n", name,
				       oid_مقصد_hex(&commit->object.oid));
				جاری;
			}

			لکھو("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_پیش_رفت();
			توڑو;
		}
	}
}

ساکن خالی export_marks(حرف *file)
{
	غیرعلامتی صحیح i;
	uint32_t mark;
	ساخت decoration_اندراج *deco = idnums.entries;
	FILE *f;
	صحیح e = 0;

	f = fopen_for_writing(file);
	اگر (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	برائے (i = 0; i < idnums.سائز; i++) {
		اگر (deco->بنیاد && deco->بنیاد->type == 1) {
			mark = ptr_مقصد_mark(deco->decoration);
			اگر (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_مقصد_hex(&deco->بنیاد->oid)) < 0) {
			    e = 1;
			    توڑو;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	اگر (e)
		error(_("unable to write marks file %s."), file);
}

ساکن خالی import_marks(حرف *داخلہ_file, صحیح check_exists)
{
	حرف سطر[512];
	FILE *f;
	ساخت stat sb;

	اگر (check_exists && stat(داخلہ_file, &sb))
		واپس;

	f = xfopen(داخلہ_file, "r");
	جب (fgets(سطر, حجم(سطر), f)) {
		uint32_t mark;
		حرف *سطر_انتہا, *mark_انتہا;
		ساخت object_id oid;
		ساخت object *object;
		ساخت commit *commit;
		شمار object_type type;

		سطر_انتہا = strchr(سطر, '\n');
		اگر (سطر[0] != ':' || !سطر_انتہا)
			die(_("corrupt mark line: %s"), سطر);
		*سطر_انتہا = '\0';

		mark = strtoumax(سطر + 1, &mark_انتہا, 10);
		اگر (!mark || mark_انتہا == سطر + 1
			|| *mark_انتہا != ' ' || get_oid_hex(mark_انتہا + 1, &oid))
			die(_("corrupt mark line: %s"), سطر);

		اگر (آخری_idnum < mark)
			آخری_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, تہی);
		اگر (type < 0)
			die(_("object not found: %s"), oid_مقصد_hex(&oid));

		اگر (type != OBJ_COMMIT)
			/* only commits */
			جاری;

		commit = lookup_commit(the_repository, &oid);
		اگر (!commit)
			die(_("not a commit? can't happen: %s"), oid_مقصد_hex(&oid));

		object = &commit->object;

		اگر (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_مقصد_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

ساکن خالی handle_deletes(خالی)
{
	صحیح i;
	برائے (i = 0; i < refspecs.nr; i++) {
		ساخت refspec_شے *refspec = &refspecs.اشیاء[i];
		اگر (*refspec->src)
			جاری;

		لکھو("reset %s\nfrom %s\n\n",
				refspec->dst, oid_مقصد_hex(null_oid(the_hash_algo)));
	}
}

ساکن صحیح parse_opt_anonymize_map(ثابت ساخت option *opt,
				   ثابت حرف *arg, صحیح unset)
{
	ساخت hashmap *map = opt->قدر;
	ثابت حرف *delim, *قدر;
	سائز_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	اگر (delim) {
		keylen = delim - arg;
		قدر = delim + 1;
	} ورنہ {
		keylen = لڑی_لمبائی(arg);
		قدر = arg;
	}

	اگر (!keylen || !*قدر)
		واپس error(_("--anonymize-map token cannot be empty"));

	جوڑو_anonymized_اندراج(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(قدر));

	واپس 0;
}

صحیح cmd_fast_export(صحیح argc,
		    ثابت حرف **argv,
		    ثابت حرف *سابقہ,
		    ساخت repository *repo غیر_مستعمل)
{
	ساخت rev_info revs;
	ساخت commit *commit;
	حرف *export_filename = تہی,
	     *import_filename = تہی,
	     *import_filename_if_exists = تہی;
	uint32_t lastimportid;
	ساخت string_list refspecs_list = STRING_LIST_INIT_NODUP;
	ساخت string_list paths_کا_بدلا_objects = STRING_LIST_INIT_DUP;
	ساخت option options[] = {
		OPT_INTEGER(0, "progress", &پیش_رفت,
			    N_("show progress after <n> objects")),
		OPT_CALLBACK(0, "signed-tags", &signed_لیبل_منوال, N_("mode"),
			     N_("select handling of signed tags"),
			     parse_opt_sign_منوال),
		OPT_CALLBACK(0, "signed-commits", &signed_commit_منوال, N_("mode"),
			     N_("select handling of signed commits"),
			     parse_opt_sign_منوال),
		OPT_CALLBACK(0, "tag-of-filtered-object", &لیبل_کا_filtered_منوال, N_("mode"),
			     N_("select handling of tags that tag filtered objects"),
			     parse_opt_لیبل_کا_filtered_منوال),
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
		OPT_BOOL(0, "full-tree", &بھرا_درخت,
			 N_("output full tree for each commit")),
		OPT_BOOL(0, "use-done-feature", &use_مکمل_feature,
			     N_("use the done feature to terminate the stream")),
		OPT_BOOL(0, "no-data", &no_ڈیٹا, N_("skip output of blob data")),
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
		OPT_BOOL(0, "mark-tags", &mark_لیبل_فہرست,
			    N_("label tags with mark ids")),

		OPT_انتہا()
	};

	اگر (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, تہی);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, سابقہ);
	init_revision_sources(&revision_sources);
	revs.topo_ترتیب = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, سابقہ, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = تیاری_revisions(argc, argv, &revs, تہی);
	اگر (argc > 1)
		usage_with_options (fast_export_usage, options);

	اگر (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	اگر (refspecs_list.nr) {
		صحیح i;

		برائے (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.اشیاء[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	اگر (use_مکمل_feature)
		لکھو("feature done\n");

	اگر (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	اگر (import_filename)
		import_marks(import_filename, 0);
	ورنہ اگر (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = آخری_idnum;

	اگر (import_filename && revs.prune_ڈیٹا.nr)
		بھرا_درخت = 1;

	get_لیبل_فہرست_and_duplicates(&revs.cmdline);

	اگر (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.الٹ = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_ڈیٹا = &paths_کا_بدلا_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	جب ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_کا_بدلا_objects);
	revs.diffopt.no_free = 0;

	handle_لیبل_فہرست_and_duplicates(&extra_refs);
	handle_لیبل_فہرست_and_duplicates(&لیبل_refs);
	handle_deletes();

	اگر (export_filename && lastimportid != آخری_idnum)
		export_marks(export_filename);

	اگر (use_مکمل_feature)
		لکھو("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	واپس 0;
}
