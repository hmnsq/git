/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "refspec.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "diffcore.h"
#అన్తర్భావ "log-tree.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "decorate.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "utf8.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "blob.h"
#అన్తర్భావ "gpg-interface.h"

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	ఖాళీ
};

స్థిర పూర్ణాంకం పురోగతి;
స్థిర గణన sign_బహుళకం signed_ట్యాగ్_బహుళకం = SIGN_ABORT;
స్థిర గణన sign_బహుళకం signed_commit_బహుళకం = SIGN_STRIP;
స్థిర గణన ట్యాగ్_యొక్క_filtered_బహుళకం { ట్యాగ్_FILTERING_ABORT, DROP, REWRITE } ట్యాగ్_యొక్క_filtered_బహుళకం = ట్యాగ్_FILTERING_ABORT;
స్థిర గణన reencode_బహుళకం { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_బహుళకం = REENCODE_ABORT;
స్థిర పూర్ణాంకం fake_missing_tagger;
స్థిర పూర్ణాంకం use_పూర్తి_feature;
స్థిర పూర్ణాంకం no_దత్తాంశం;
స్థిర పూర్ణాంకం నిండిన_వృక్షం;
స్థిర పూర్ణాంకం reference_excluded_commits;
స్థిర పూర్ణాంకం show_original_ids;
స్థిర పూర్ణాంకం mark_ట్యాగులు;
స్థిర నిర్మాణం string_list extra_refs = STRING_LIST_INIT_DUP;
స్థిర నిర్మాణం string_list ట్యాగ్_refs = STRING_LIST_INIT_DUP;
స్థిర నిర్మాణం refspec refspecs;
స్థిర పూర్ణాంకం anonymize;
స్థిర నిర్మాణం hashmap anonymized_seeds;
స్థిర నిర్మాణం revision_sources revision_sources;

స్థిర పూర్ణాంకం parse_opt_sign_బహుళకం(స్థిరరాశి నిర్మాణం option *opt,
			       స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	గణన sign_బహుళకం *val = opt->విలువ;

	ఒకవేళ (unset)
		తిరిగి 0;

	ఒకవేళ (parse_sign_బహుళకం(arg, val, ఖాళీ) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		తిరిగి error(_("unknown %s mode: %s"), opt->long_name, arg);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_opt_ట్యాగ్_యొక్క_filtered_బహుళకం(స్థిరరాశి నిర్మాణం option *opt,
					  స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	గణన ట్యాగ్_యొక్క_filtered_బహుళకం *val = opt->విలువ;

	ఒకవేళ (unset || !strcmp(arg, "abort"))
		*val = ట్యాగ్_FILTERING_ABORT;
	లేకపోతే ఒకవేళ (!strcmp(arg, "drop"))
		*val = DROP;
	లేకపోతే ఒకవేళ (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	లేకపోతే
		తిరిగి error(_("unknown tag-of-filtered mode: %s"), arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_opt_reencode_బహుళకం(స్థిరరాశి నిర్మాణం option *opt,
				   స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	గణన reencode_బహుళకం *val = opt->విలువ;

	ఒకవేళ (unset) {
		*val = REENCODE_ABORT;
		తిరిగి 0;
	}

	ఎంపిక (git_parse_maybe_bool(arg)) {
	స్థితి 0:
		*val = REENCODE_NO;
		ఆపు;
	స్థితి 1:
		*val = REENCODE_YES;
		ఆపు;
	అప్రమేయ:
		ఒకవేళ (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		లేకపోతే
			తిరిగి error(_("unknown reencoding mode: %s"), arg);
	}

	తిరిగి 0;
}

స్థిర నిర్మాణం decoration idnums;
స్థిర uint32_t చివరి_idnum;
నిర్మాణం anonymized_నమోదు {
	నిర్మాణం hashmap_నమోదు hash;
	అక్షరం *anon;
	స్థిరరాశి అక్షరం orig[FLEX_ARRAY];
};

నిర్మాణం anonymized_నమోదు_కీలకం {
	నిర్మాణం hashmap_నమోదు hash;
	స్థిరరాశి అక్షరం *orig;
	కొలత_t orig_len;
};

స్థిర పూర్ణాంకం anonymized_నమోదు_cmp(స్థిరరాశి శూన్య *cmp_దత్తాంశం వాడని,
				స్థిరరాశి నిర్మాణం hashmap_నమోదు *eptr,
				స్థిరరాశి నిర్మాణం hashmap_నమోదు *నమోదు_or_కీలకం,
				స్థిరరాశి శూన్య *keydata)
{
	స్థిరరాశి నిర్మాణం anonymized_నమోదు *a, *b;

	a = container_యొక్క(eptr, స్థిరరాశి నిర్మాణం anonymized_నమోదు, hash);
	ఒకవేళ (keydata) {
		స్థిరరాశి నిర్మాణం anonymized_నమోదు_కీలకం *కీలకం = keydata;
		పూర్ణాంకం equal = !xstrncmpz(a->orig, కీలకం->orig, కీలకం->orig_len);
		తిరిగి !equal;
	}

	b = container_యొక్క(నమోదు_or_కీలకం, స్థిరరాశి నిర్మాణం anonymized_నమోదు, hash);
	తిరిగి strcmp(a->orig, b->orig);
}

స్థిర నిర్మాణం anonymized_నమోదు *కూడు_anonymized_నమోదు(నిర్మాణం hashmap *map,
						     గుర్తులేని hash,
						     స్థిరరాశి అక్షరం *orig, కొలత_t len,
						     అక్షరం *anon)
{
	నిర్మాణం anonymized_నమోదు *ret, *old;

	ఒకవేళ (!map->cmpfn)
		hashmap_init(map, anonymized_నమోదు_cmp, ఖాళీ, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_నమోదు_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_నమోదు(map, ret, hash);

	ఒకవేళ (old) {
		విడుదల(old->anon);
		విడుదల(old);
	}

	తిరిగి ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
స్థిర స్థిరరాశి అక్షరం *anonymize_str(నిర్మాణం hashmap *map,
				 అక్షరం *(*generate)(శూన్య),
				 స్థిరరాశి అక్షరం *orig, కొలత_t len)
{
	నిర్మాణం anonymized_నమోదు_కీలకం కీలకం;
	నిర్మాణం anonymized_నమోదు *ret;

	hashmap_నమోదు_init(&కీలకం.hash, memhash(orig, len));
	కీలకం.orig = orig;
	కీలకం.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_నమోదు(&anonymized_seeds, &కీలకం, hash, &కీలకం);

	/* ...otherwise check if we've already seen it in this context... */
	ఒకవేళ (!ret)
		ret = hashmap_get_నమోదు(map, &కీలకం, hash, &కీలకం);

	/* ...and finally generate a new mapping if necessary */
	ఒకవేళ (!ret)
		ret = కూడు_anonymized_నమోదు(map, కీలకం.hash.hash,
					   orig, len, generate());

	తిరిగి ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
స్థిర శూన్య anonymize_మార్గం(నిర్మాణం strbuf *బహిర్గతం, స్థిరరాశి అక్షరం *మార్గం,
			   నిర్మాణం hashmap *map,
			   అక్షరం *(*generate)(శూన్య))
{
	అయితే (*మార్గం) {
		స్థిరరాశి అక్షరం *చివర_యొక్క_component = strchrnul(మార్గం, '/');
		కొలత_t len = చివర_యొక్క_component - మార్గం;
		స్థిరరాశి అక్షరం *c = anonymize_str(map, generate, మార్గం, len);
		strbuf_addstr(బహిర్గతం, c);
		మార్గం = చివర_యొక్క_component;
		ఒకవేళ (*మార్గం)
			strbuf_addch(బహిర్గతం, *మార్గం++);
	}
}

స్థిర అంతఃవరుస శూన్య *mark_గమ్యం_ptr(uint32_t mark)
{
	తిరిగి (శూన్య *)(uintptr_t)mark;
}

స్థిర అంతఃవరుస uint32_t ptr_గమ్యం_mark(శూన్య * mark)
{
	తిరిగి (uint32_t)(uintptr_t)mark;
}

స్థిర అంతఃవరుస శూన్య mark_object(నిర్మాణం object *object, uint32_t mark)
{
	కూడు_decoration(&idnums, object, mark_గమ్యం_ptr(mark));
}

స్థిర అంతఃవరుస శూన్య mark_తదుపరి_object(నిర్మాణం object *object)
{
	mark_object(object, ++చివరి_idnum);
}

స్థిర పూర్ణాంకం get_object_mark(నిర్మాణం object *object)
{
	శూన్య *decoration = lookup_decoration(&idnums, object);
	ఒకవేళ (!decoration)
		తిరిగి 0;
	తిరిగి ptr_గమ్యం_mark(decoration);
}

స్థిర నిర్మాణం commit *rewrite_commit(నిర్మాణం commit *p)
{
	కొరకు (;;) {
		ఒకవేళ (p->parents && p->parents->తదుపరి)
			ఆపు;
		ఒకవేళ (p->object.flags & UNINTERESTING)
			ఆపు;
		ఒకవేళ (!(p->object.flags & TREESAME))
			ఆపు;
		ఒకవేళ (!p->parents)
			తిరిగి ఖాళీ;
		p = p->parents->అంశం;
	}
	తిరిగి p;
}

స్థిర శూన్య show_పురోగతి(శూన్య)
{
	స్థిర పూర్ణాంకం counter = 0;
	ఒకవేళ (!పురోగతి)
		తిరిగి;
	ఒకవేళ ((++counter % పురోగతి) == 0)
		రాయి("progress %d objects\n", counter);
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
స్థిర అక్షరం *anonymize_blob(గుర్తులేని దీర్ఘ *కొలత)
{
	స్థిర పూర్ణాంకం counter;
	నిర్మాణం strbuf బహిర్గతం = STRBUF_INIT;
	strbuf_addf(&బహిర్గతం, "anonymous blob %d", counter++);
	*కొలత = బహిర్గతం.len;
	తిరిగి strbuf_detach(&బహిర్గతం, ఖాళీ);
}

స్థిర శూన్య export_blob(స్థిరరాశి నిర్మాణం object_id *oid)
{
	గుర్తులేని దీర్ఘ కొలత;
	గణన object_type type;
	అక్షరం *buf;
	నిర్మాణం object *object;
	పూర్ణాంకం eaten;

	ఒకవేళ (no_దత్తాంశం)
		తిరిగి;

	ఒకవేళ (is_null_oid(oid))
		తిరిగి;

	object = lookup_object(the_repository, oid);
	ఒకవేళ (object && object->flags & SHOWN)
		తిరిగి;

	ఒకవేళ (anonymize) {
		buf = anonymize_blob(&కొలత);
		object = (నిర్మాణం object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} లేకపోతే {
		కొలత_t కొలత_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &కొలత_st);
		కొలత = cast_కొలత_t_గమ్యం_ulong(కొలత_st);
		ఒకవేళ (!buf)
			die(_("could not read blob %s"), oid_గమ్యం_hex(oid));
		ఒకవేళ (check_object_signature(the_repository, oid, buf, కొలత,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_గమ్యం_hex(oid));
		object = parse_object_బఫరు(the_repository, oid, type,
					     కొలత, buf, &eaten);
	}

	ఒకవేళ (!object)
		die(_("could not read blob %s"), oid_గమ్యం_hex(oid));

	mark_తదుపరి_object(object);

	రాయి("blob\nmark :%"PRIu32"\n", చివరి_idnum);
	ఒకవేళ (show_original_ids)
		రాయి("original-oid %s\n", oid_గమ్యం_hex(oid));
	రాయి("data %"PRIuMAX"\n", (uintmax_t)కొలత);
	ఒకవేళ (కొలత && fwrite(buf, కొలత, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_గమ్యం_hex(oid));
	రాయి("\n");

	show_పురోగతి();

	object->flags |= SHOWN;
	ఒకవేళ (!eaten)
		విడుదల(buf);
}

స్థిర పూర్ణాంకం లోతు_మొదటి(స్థిరరాశి శూన్య *a_, స్థిరరాశి శూన్య *b_)
{
	స్థిరరాశి నిర్మాణం diff_filepair *a = *((స్థిరరాశి నిర్మాణం diff_filepair **)a_);
	స్థిరరాశి నిర్మాణం diff_filepair *b = *((స్థిరరాశి నిర్మాణం diff_filepair **)b_);
	స్థిరరాశి అక్షరం *name_a, *name_b;
	పూర్ణాంకం len_a, len_b, len;
	పూర్ణాంకం cmp;

	name_a = a->one ? a->one->మార్గం : a->two->మార్గం;
	name_b = b->one ? b->one->మార్గం : b->two->మార్గం;

	len_a = అక్షరశ్రేణి_పొడవు(name_a);
	len_b = అక్షరశ్రేణి_పొడవు(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	ఒకవేళ (cmp)
		తిరిగి cmp;
	cmp = len_b - len_a;
	ఒకవేళ (cmp)
		తిరిగి cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	తిరిగి (a->స్థితి_2 == 'R') - (b->స్థితి_2 == 'R');
}

స్థిర శూన్య print_మార్గం_1(స్థిరరాశి అక్షరం *మార్గం)
{
	పూర్ణాంకం need_quote = quote_c_style(మార్గం, ఖాళీ, ఖాళీ, 0);
	ఒకవేళ (need_quote)
		quote_c_style(మార్గం, ఖాళీ, stdout, 0);
	లేకపోతే ఒకవేళ (strchr(మార్గం, ' '))
		రాయి("\"%s\"", మార్గం);
	లేకపోతే
		రాయి("%s", మార్గం);
}

స్థిర అక్షరం *anonymize_మార్గం_component(శూన్య)
{
	స్థిర పూర్ణాంకం counter;
	నిర్మాణం strbuf బహిర్గతం = STRBUF_INIT;
	strbuf_addf(&బహిర్గతం, "path%d", counter++);
	తిరిగి strbuf_detach(&బహిర్గతం, ఖాళీ);
}

స్థిర శూన్య print_మార్గం(స్థిరరాశి అక్షరం *మార్గం)
{
	ఒకవేళ (!anonymize)
		print_మార్గం_1(మార్గం);
	లేకపోతే {
		స్థిర నిర్మాణం hashmap paths;
		స్థిర నిర్మాణం strbuf anon = STRBUF_INIT;

		anonymize_మార్గం(&anon, మార్గం, &paths, anonymize_మార్గం_component);
		print_మార్గం_1(anon.buf);
		strbuf_reset(&anon);
	}
}

స్థిర అక్షరం *generate_fake_oid(శూన్య)
{
	స్థిర uint32_t counter = 1; /* avoid null oid */
	స్థిరరాశి గుర్తులేని hashsz = the_hash_algo->rawsz;
	నిర్మాణం object_id oid;
	అక్షరం *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	తిరిగి oid_గమ్యం_hex_r(hex, &oid);
}

స్థిర స్థిరరాశి అక్షరం *anonymize_oid(స్థిరరాశి అక్షరం *oid_hex)
{
	స్థిర నిర్మాణం hashmap objs;
	కొలత_t len = అక్షరశ్రేణి_పొడవు(oid_hex);
	తిరిగి anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

స్థిర శూన్య show_filemodify(నిర్మాణం diff_వరుస_struct *q,
			    నిర్మాణం diff_options *options వాడని, శూన్య *దత్తాంశం)
{
	పూర్ణాంకం i;
	నిర్మాణం string_list *మారింది = దత్తాంశం;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->వరుస, q->nr, లోతు_మొదటి);

	కొరకు (i = 0; i < q->nr; i++) {
		నిర్మాణం diff_filespec *ospec = q->వరుస[i]->one;
		నిర్మాణం diff_filespec *spec = q->వరుస[i]->two;

		ఎంపిక (q->వరుస[i]->స్థితి_2) {
		స్థితి DIFF_స్థితి_DELETED:
			రాయి("D ");
			print_మార్గం(spec->మార్గం);
			string_list_చొప్పించు(మారింది, spec->మార్గం);
			అక్షరం_రాయి('\n');
			ఆపు;

		స్థితి DIFF_స్థితి_COPIED:
		స్థితి DIFF_స్థితి_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			ఒకవేళ (!string_list_has_string(మారింది, ospec->మార్గం)) {
				రాయి("%c ", q->వరుస[i]->స్థితి_2);
				print_మార్గం(ospec->మార్గం);
				అక్షరం_రాయి(' ');
				print_మార్గం(spec->మార్గం);
				string_list_చొప్పించు(మారింది, spec->మార్గం);
				అక్షరం_రాయి('\n');

				ఒకవేళ (oideq(&ospec->oid, &spec->oid) &&
				    ospec->బహుళకం == spec->బహుళకం)
					ఆపు;
			}
			/* fallthrough */

		స్థితి DIFF_స్థితి_TYPE_మారింది:
		స్థితి DIFF_స్థితి_MODIFIED:
		స్థితి DIFF_స్థితి_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			ఒకవేళ (no_దత్తాంశం || S_ISGITLINK(spec->బహుళకం))
				రాయి("M %06o %s ", spec->బహుళకం,
				       anonymize ?
				       anonymize_oid(oid_గమ్యం_hex(&spec->oid)) :
				       oid_గమ్యం_hex(&spec->oid));
			లేకపోతే {
				నిర్మాణం object *object = lookup_object(the_repository,
								      &spec->oid);
				రాయి("M %06o :%d ", spec->బహుళకం,
				       get_object_mark(object));
			}
			print_మార్గం(spec->మార్గం);
			string_list_చొప్పించు(మారింది, spec->మార్గం);
			అక్షరం_రాయి('\n');
			ఆపు;

		అప్రమేయ:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->వరుస[i]->స్థితి_2,
			    ospec->మార్గం ? ospec->మార్గం : _("none"),
			    spec->మార్గం ? spec->మార్గం : _("none"));
		}
	}
}

స్థిర అక్షరం *anonymize_ref_component(శూన్య)
{
	స్థిర పూర్ణాంకం counter;
	నిర్మాణం strbuf బహిర్గతం = STRBUF_INIT;
	strbuf_addf(&బహిర్గతం, "ref%d", counter++);
	తిరిగి strbuf_detach(&బహిర్గతం, ఖాళీ);
}

స్థిర స్థిరరాశి అక్షరం *anonymize_refname(స్థిరరాశి అక్షరం *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	స్థిర స్థిరరాశి అక్షరం *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	స్థిర నిర్మాణం hashmap refs;
	స్థిర నిర్మాణం strbuf anon = STRBUF_INIT;
	పూర్ణాంకం i;

	strbuf_reset(&anon);
	కొరకు (i = 0; i < ARRAY_కొలత(prefixes); i++) {
		ఒకవేళ (skip_ఉపసర్గ(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			ఆపు;
		}
	}

	anonymize_మార్గం(&anon, refname, &refs, anonymize_ref_component);
	తిరిగి anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
స్థిర అక్షరం *anonymize_commit_సందేశం(శూన్య)
{
	స్థిర పూర్ణాంకం counter;
	తిరిగి xstrfmt("subject %d\n\nbody\n", counter++);
}

స్థిర అక్షరం *anonymize_ident(శూన్య)
{
	స్థిర పూర్ణాంకం counter;
	నిర్మాణం strbuf బహిర్గతం = STRBUF_INIT;
	strbuf_addf(&బహిర్గతం, "User %d <user%d@example.com>", counter, counter);
	counter++;
	తిరిగి strbuf_detach(&బహిర్గతం, ఖాళీ);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
స్థిర శూన్య anonymize_ident_పంక్తి(స్థిరరాశి అక్షరం **beg, స్థిరరాశి అక్షరం **చివర)
{
	స్థిర నిర్మాణం hashmap idents;
	స్థిర నిర్మాణం strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	స్థిర గుర్తులేని which_బఫరు;

	నిర్మాణం strbuf *బహిర్గతం;
	నిర్మాణం ident_విభజించు విభజించు;
	స్థిరరాశి అక్షరం *చివర_యొక్క_శీర్షిక;

	బహిర్గతం = &buffers[which_బఫరు++];
	which_బఫరు %= ARRAY_కొలత(buffers);
	strbuf_reset(బహిర్గతం);

	/* skip "committer", "author", "tagger", etc */
	చివర_యొక్క_శీర్షిక = strchr(*beg, ' ');
	ఒకవేళ (!చివర_యొక్క_శీర్షిక)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (పూర్ణాంకం)(*చివర - *beg), *beg);
	చివర_యొక్క_శీర్షిక++;
	strbuf_కూడు(బహిర్గతం, *beg, చివర_యొక్క_శీర్షిక - *beg);

	ఒకవేళ (!విభజించు_ident_పంక్తి(&విభజించు, చివర_యొక్క_శీర్షిక, *చివర - చివర_యొక్క_శీర్షిక) &&
	    విభజించు.date_begin) {
		స్థిరరాశి అక్షరం *ident;
		కొలత_t len;

		len = విభజించు.mail_చివర - విభజించు.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      విభజించు.name_begin, len);
		strbuf_addstr(బహిర్గతం, ident);
		strbuf_addch(బహిర్గతం, ' ');
		strbuf_కూడు(బహిర్గతం, విభజించు.date_begin, విభజించు.tz_చివర - విభజించు.date_begin);
	} లేకపోతే {
		strbuf_addstr(బహిర్గతం, "Malformed Ident <malformed@example.com> 0 -0000");
	}

	*beg = బహిర్గతం->buf;
	*చివర = బహిర్గతం->buf + బహిర్గతం->len;
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
స్థిర స్థిరరాశి అక్షరం *కనుగొను_commit_multiline_శీర్షిక(స్థిరరాశి అక్షరం *సందేశం,
						స్థిరరాశి అక్షరం *కీలకం,
						స్థిరరాశి అక్షరం **చివర)
{
	నిర్మాణం strbuf val = STRBUF_INIT;
	స్థిరరాశి అక్షరం *bol, *eol;
	కొలత_t len;

	bol = కనుగొను_commit_శీర్షిక(సందేశం, కీలకం, &len);
	ఒకవేళ (!bol)
		తిరిగి ఖాళీ;
	eol = bol + len;
	strbuf_కూడు(&val, bol, len);

	అయితే (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_కూడు(&val, bol, eol - bol);
	}

	*చివర = eol;
	తిరిగి strbuf_detach(&val, ఖాళీ);
}

స్థిర శూన్య print_signature(స్థిరరాశి అక్షరం *signature, స్థిరరాశి అక్షరం *object_hash)
{
	ఒకవేళ (!signature)
		తిరిగి;

	రాయి("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (గుర్తులేని)అక్షరశ్రేణి_పొడవు(signature),
	       signature);
}

స్థిర స్థిరరాశి అక్షరం *append_signatures_for_శీర్షిక(నిర్మాణం string_list *signatures,
						స్థిరరాశి అక్షరం *pos,
						స్థిరరాశి అక్షరం *శీర్షిక,
						స్థిరరాశి అక్షరం *object_hash)
{
	స్థిరరాశి అక్షరం *signature;
	స్థిరరాశి అక్షరం *ప్రారంభం = pos;
	స్థిరరాశి అక్షరం *చివర = pos;

	అయితే ((signature = కనుగొను_commit_multiline_శీర్షిక(ప్రారంభం + 1,
							 శీర్షిక,
							 &చివర))) {
		string_list_append(signatures, signature)->util = (శూన్య *)object_hash;
		విడుదల((అక్షరం *)signature);
		ప్రారంభం = చివర;
	}

	తిరిగి చివర;
}

స్థిర శూన్య handle_commit(నిర్మాణం commit *commit, నిర్మాణం rev_info *rev,
			  నిర్మాణం string_list *paths_యొక్క_మారింది_objects)
{
	పూర్ణాంకం saved_output_format = rev->diffopt.output_format;
	స్థిరరాశి అక్షరం *commit_బఫరు, *commit_బఫరు_cursor;
	స్థిరరాశి అక్షరం *author, *author_చివర, *committer, *committer_చివర;
	స్థిరరాశి అక్షరం *encoding = ఖాళీ;
	కొలత_t encoding_len;
	నిర్మాణం string_list signatures = STRING_LIST_INIT_DUP;
	స్థిరరాశి అక్షరం *సందేశం_2;
	అక్షరం *reencoded = ఖాళీ;
	నిర్మాణం commit_list *p;
	స్థిరరాశి అక్షరం *refname;
	పూర్ణాంకం i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_బఫరు_cursor = commit_బఫరు = repo_get_commit_బఫరు(the_repository, commit, ఖాళీ);

	author = అక్షరశ్రేణి_శోధన(commit_బఫరు_cursor, "\nauthor ");
	ఒకవేళ (!author)
		die(_("could not find author in commit %s"),
		    oid_గమ్యం_hex(&commit->object.oid));
	author++;
	commit_బఫరు_cursor = author_చివర = strchrnul(author, '\n');

	committer = అక్షరశ్రేణి_శోధన(commit_బఫరు_cursor, "\ncommitter ");
	ఒకవేళ (!committer)
		die(_("could not find committer in commit %s"),
		    oid_గమ్యం_hex(&commit->object.oid));
	committer++;
	commit_బఫరు_cursor = committer_చివర = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	ఒకవేళ (*commit_బఫరు_cursor == '\n') {
		encoding = కనుగొను_commit_శీర్షిక(commit_బఫరు_cursor + 1, "encoding", &encoding_len);
		ఒకవేళ (encoding)
			commit_బఫరు_cursor = encoding + encoding_len;
	}

	ఒకవేళ (*commit_బఫరు_cursor == '\n') {
		స్థిరరాశి అక్షరం *after_sha1 = append_signatures_for_శీర్షిక(&signatures, commit_బఫరు_cursor,
								      "gpgsig", "sha1");
		స్థిరరాశి అక్షరం *after_sha256 = append_signatures_for_శీర్షిక(&signatures, commit_బఫరు_cursor,
									"gpgsig-sha256", "sha256");
		commit_బఫరు_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	సందేశం_2 = అక్షరశ్రేణి_శోధన(commit_బఫరు_cursor, "\n\n");
	ఒకవేళ (సందేశం_2)
		సందేశం_2 += 2;

	ఒకవేళ (commit->parents &&
	    (get_object_mark(&commit->parents->అంశం->object) != 0 ||
	     reference_excluded_commits) &&
	    !నిండిన_వృక్షం) {
		parse_commit_or_die(commit->parents->అంశం);
		diff_వృక్షం_oid(get_commit_వృక్షం_oid(commit->parents->అంశం),
			      get_commit_వృక్షం_oid(commit), "", &rev->diffopt);
	}
	లేకపోతే
		diff_మూలం_వృక్షం_oid(get_commit_వృక్షం_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	కొరకు (i = 0; i < diff_queued_diff.nr; i++)
		ఒకవేళ (!S_ISGITLINK(diff_queued_diff.వరుస[i]->two->బహుళకం))
			export_blob(&diff_queued_diff.వరుస[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	ఒకవేళ (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_పంక్తి(&committer, &committer_చివర);
		anonymize_ident_పంక్తి(&author, &author_చివర);
	}

	mark_తదుపరి_object(&commit->object);
	ఒకవేళ (anonymize) {
		reencoded = anonymize_commit_సందేశం();
	} లేకపోతే ఒకవేళ (encoding) {
		అక్షరం *buf;
		ఎంపిక (reencode_బహుళకం) {
		స్థితి REENCODE_YES:
			buf = xstrfmt("%.*s", (పూర్ణాంకం)encoding_len, encoding);
			reencoded = reencode_string(సందేశం_2, "UTF-8", buf);
			విడుదల(buf);
			ఆపు;
		స్థితి REENCODE_NO:
			ఆపు;
		స్థితి REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (పూర్ణాంకం)encoding_len, encoding,
			    oid_గమ్యం_hex(&commit->object.oid));
		}
	}
	ఒకవేళ (!commit->parents)
		రాయి("reset %s\n", refname);
	రాయి("commit %s\nmark :%"PRIu32"\n", refname, చివరి_idnum);
	ఒకవేళ (show_original_ids)
		రాయి("original-oid %s\n", oid_గమ్యం_hex(&commit->object.oid));
	రాయి("%.*s\n%.*s\n",
	       (పూర్ణాంకం)(author_చివర - author), author,
	       (పూర్ణాంకం)(committer_చివర - committer), committer);
	ఒకవేళ (signatures.nr) {
		ఎంపిక (signed_commit_బహుళకం) {
		/* Exporting modes */
		స్థితి SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_గమ్యం_hex(&commit->object.oid));
			/* fallthru */
		స్థితి SIGN_VERBATIM:
			కొరకు (కొలత_t i = 0; i < signatures.nr; i++) {
				నిర్మాణం string_list_అంశం *అంశం = &signatures.వస్తువులు[i];
				print_signature(అంశం->string, అంశం->util);
			}
			ఆపు;

		/* Stripping modes */
		స్థితి SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_గమ్యం_hex(&commit->object.oid));
			/* fallthru */
		స్థితి SIGN_STRIP:
			ఆపు;

		/* Aborting modes */
		స్థితి SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_గమ్యం_hex(&commit->object.oid));
		అప్రమేయ:
			BUG("invalid signed_commit_mode value %d", signed_commit_బహుళకం);
		}
		string_list_clear(&signatures, 0);
	}
	ఒకవేళ (!reencoded && encoding)
		రాయి("encoding %.*s\n", (పూర్ణాంకం)encoding_len, encoding);
	రాయి("data %u\n%s",
	       (గుర్తులేని)(reencoded
			  ? అక్షరశ్రేణి_పొడవు(reencoded) : సందేశం_2
			  ? అక్షరశ్రేణి_పొడవు(సందేశం_2) : 0),
	       reencoded ? reencoded : సందేశం_2 ? సందేశం_2 : "");
	విడుదల(reencoded);
	repo_unuse_commit_బఫరు(the_repository, commit, commit_బఫరు);

	కొరకు (i = 0, p = commit->parents; p; p = p->తదుపరి) {
		నిర్మాణం object *obj = &p->అంశం->object;
		పూర్ణాంకం mark = get_object_mark(obj);

		ఒకవేళ (!mark && !reference_excluded_commits)
			కొనసాగు;
		ఒకవేళ (i == 0)
			రాయి("from ");
		లేకపోతే
			రాయి("merge ");
		ఒకవేళ (mark)
			రాయి(":%d\n", mark);
		లేకపోతే
			రాయి("%s\n",
			       anonymize ?
			       anonymize_oid(oid_గమ్యం_hex(&obj->oid)) :
			       oid_గమ్యం_hex(&obj->oid));
		i++;
	}

	ఒకవేళ (నిండిన_వృక్షం)
		రాయి("deleteall\n");
	log_వృక్షం_diff_flush(rev);
	string_list_clear(paths_యొక్క_మారింది_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	రాయి("\n");

	show_పురోగతి();
}

స్థిర అక్షరం *anonymize_ట్యాగ్(శూన్య)
{
	స్థిర పూర్ణాంకం counter;
	నిర్మాణం strbuf బహిర్గతం = STRBUF_INIT;
	strbuf_addf(&బహిర్గతం, "tag message %d", counter++);
	తిరిగి strbuf_detach(&బహిర్గతం, ఖాళీ);
}


స్థిర శూన్య handle_ట్యాగ్(స్థిరరాశి అక్షరం *name, నిర్మాణం ట్యాగ్ *ట్యాగ్)
{
	కొలత_t కొలత;
	గణన object_type type;
	అక్షరం *buf;
	స్థిరరాశి అక్షరం *tagger, *tagger_చివర, *సందేశం_2;
	కొలత_t సందేశం_కొలత = 0;
	నిర్మాణం object *tagged;
	పూర్ణాంకం tagged_mark;
	నిర్మాణం commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = ట్యాగ్->tagged;
	అయితే (tagged->type == OBJ_ట్యాగ్) {
		tagged = ((నిర్మాణం ట్యాగ్ *)tagged)->tagged;
	}
	ఒకవేళ (tagged->type == OBJ_వృక్షం) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_గమ్యం_hex(&ట్యాగ్->object.oid));
		తిరిగి;
	}

	buf = odb_read_object(the_repository->objects, &ట్యాగ్->object.oid,
			      &type, &కొలత);
	ఒకవేళ (!buf)
		die(_("could not read tag %s"), oid_గమ్యం_hex(&ట్యాగ్->object.oid));
	సందేశం_2 = memmem(buf, కొలత, "\n\n", 2);
	ఒకవేళ (సందేశం_2) {
		సందేశం_2 += 2;
		సందేశం_కొలత = అక్షరశ్రేణి_పొడవు(సందేశం_2);
	}
	tagger = memmem(buf, సందేశం_2 ? సందేశం_2 - buf : కొలత, "\ntagger ", 8);
	ఒకవేళ (!tagger) {
		ఒకవేళ (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		లేకపోతే
			tagger = "";
		tagger_చివర = tagger + అక్షరశ్రేణి_పొడవు(tagger);
	} లేకపోతే {
		tagger++;
		tagger_చివర = strchrnul(tagger, '\n');
		ఒకవేళ (anonymize)
			anonymize_ident_పంక్తి(&tagger, &tagger_చివర);
	}

	ఒకవేళ (anonymize) {
		name = anonymize_refname(name);
		ఒకవేళ (సందేశం_2) {
			స్థిర నిర్మాణం hashmap ట్యాగులు;
			సందేశం_2 = anonymize_str(&ట్యాగులు, anonymize_ట్యాగ్,
						సందేశం_2, సందేశం_కొలత);
			సందేశం_కొలత = అక్షరశ్రేణి_పొడవు(సందేశం_2);
		}
	}

	/* handle signed tags */
	ఒకవేళ (సందేశం_2) {
		కొలత_t sig_స్థానభ్రంశం = parse_signed_బఫరు(సందేశం_2, సందేశం_కొలత);
		ఒకవేళ (sig_స్థానభ్రంశం < సందేశం_కొలత)
			ఎంపిక (signed_ట్యాగ్_బహుళకం) {
			/* Exporting modes */
			స్థితి SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_గమ్యం_hex(&ట్యాగ్->object.oid));
				/* fallthru */
			స్థితి SIGN_VERBATIM:
				ఆపు;

			/* Stripping modes */
			స్థితి SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_గమ్యం_hex(&ట్యాగ్->object.oid));
				/* fallthru */
			స్థితి SIGN_STRIP:
				సందేశం_కొలత = sig_స్థానభ్రంశం;
				ఆపు;

			/* Aborting modes */
			స్థితి SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_గమ్యం_hex(&ట్యాగ్->object.oid));
			అప్రమేయ:
				BUG("invalid signed_commit_mode value %d", signed_commit_బహుళకం);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = ట్యాగ్->tagged;
	tagged_mark = get_object_mark(tagged);
	ఒకవేళ (!tagged_mark) {
		ఎంపిక (ట్యాగ్_యొక్క_filtered_బహుళకం) {
		స్థితి ట్యాగ్_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_గమ్యం_hex(&ట్యాగ్->object.oid));
		స్థితి DROP:
			/* Ignore this tag altogether */
			విడుదల(buf);
			తిరిగి;
		స్థితి REWRITE:
			ఒకవేళ (tagged->type == OBJ_ట్యాగ్ && !mark_ట్యాగులు) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} లేకపోతే ఒకవేళ (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((నిర్మాణం commit *)tagged);
				ఒకవేళ (!p) {
					రాయి("reset %s\nfrom %s\n\n",
					       name, oid_గమ్యం_hex(null_oid(the_hash_algo)));
					విడుదల(buf);
					తిరిగి;
				}
				tagged_mark = get_object_mark(&p->object);
			} లేకపోతే {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	ఒకవేళ (tagged->type == OBJ_ట్యాగ్) {
		రాయి("reset %s\nfrom %s\n\n",
		       name, oid_గమ్యం_hex(null_oid(the_hash_algo)));
	}
	skip_ఉపసర్గ(name, "refs/tags/", &name);
	రాయి("tag %s\n", name);
	ఒకవేళ (mark_ట్యాగులు) {
		mark_తదుపరి_object(&ట్యాగ్->object);
		రాయి("mark :%"PRIu32"\n", చివరి_idnum);
	}
	ఒకవేళ (tagged_mark)
		రాయి("from :%d\n", tagged_mark);
	లేకపోతే
		రాయి("from %s\n", oid_గమ్యం_hex(&tagged->oid));

	ఒకవేళ (show_original_ids)
		రాయి("original-oid %s\n", oid_గమ్యం_hex(&ట్యాగ్->object.oid));
	రాయి("%.*s%sdata %d\n%.*s\n",
	       (పూర్ణాంకం)(tagger_చివర - tagger), tagger,
	       tagger == tagger_చివర ? "" : "\n",
	       (పూర్ణాంకం)సందేశం_కొలత, (పూర్ణాంకం)సందేశం_కొలత, సందేశం_2 ? సందేశం_2 : "");
	విడుదల(buf);
}

స్థిర నిర్మాణం commit *get_commit(నిర్మాణం rev_cmdline_నమోదు *e, స్థిరరాశి అక్షరం *నిండిన_name)
{
	ఎంపిక (e->అంశం->type) {
	స్థితి OBJ_COMMIT:
		తిరిగి (నిర్మాణం commit *)e->అంశం;
	స్థితి OBJ_ట్యాగ్: {
		నిర్మాణం ట్యాగ్ *ట్యాగ్ = (నిర్మాణం ట్యాగ్ *)e->అంశం;

		/* handle nested tags */
		అయితే (ట్యాగ్ && ట్యాగ్->object.type == OBJ_ట్యాగ్) {
			parse_object(the_repository, &ట్యాగ్->object.oid);
			string_list_append(&ట్యాగ్_refs, నిండిన_name)->util = ట్యాగ్;
			ట్యాగ్ = (నిర్మాణం ట్యాగ్ *)ట్యాగ్->tagged;
		}
		ఒకవేళ (!ట్యాగ్)
			die(_("tag %s points nowhere?"), e->name);
		తిరిగి (నిర్మాణం commit *)ట్యాగ్;
	}
	అప్రమేయ:
		తిరిగి ఖాళీ;
	}
}

స్థిర శూన్య get_ట్యాగులు_and_duplicates(నిర్మాణం rev_cmdline_info *info)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < info->nr; i++) {
		నిర్మాణం rev_cmdline_నమోదు *e = info->rev + i;
		నిర్మాణం object_id oid;
		నిర్మాణం commit *commit;
		అక్షరం *నిండిన_name = ఖాళీ;

		ఒకవేళ (e->flags & UNINTERESTING)
			కొనసాగు;

		ఒకవేళ (repo_dwim_ref(the_repository, e->name, అక్షరశ్రేణి_పొడవు(e->name),
				  &oid, &నిండిన_name, 0) != 1) {
			విడుదల(నిండిన_name);
			కొనసాగు;
		}

		ఒకవేళ (refspecs.nr) {
			అక్షరం *అంతరంగ;
			అంతరంగ = apply_refspecs(&refspecs, నిండిన_name);
			ఒకవేళ (అంతరంగ) {
				విడుదల(నిండిన_name);
				నిండిన_name = అంతరంగ;
			}
		}

		commit = get_commit(e, నిండిన_name);
		ఒకవేళ (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->అంశం->type));
			విడుదల(నిండిన_name);
			కొనసాగు;
		}

		ఎంపిక (commit->object.type) {
		స్థితి OBJ_COMMIT:
			ఆపు;
		స్థితి OBJ_BLOB:
			export_blob(&commit->object.oid);
			విడుదల(నిండిన_name);
			కొనసాగు;
		అప్రమేయ: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			విడుదల(నిండిన_name);
			కొనసాగు;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		ఒకవేళ (e->అంశం->type != OBJ_ట్యాగ్)
			string_list_append(&extra_refs, నిండిన_name)->util = commit;

		ఒకవేళ (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = నిండిన_name;
		లేకపోతే
			విడుదల(నిండిన_name);
	}

	string_list_క్రమీకరించు_u(&extra_refs, 0);
}

స్థిర శూన్య handle_ట్యాగులు_and_duplicates(నిర్మాణం string_list *extras)
{
	నిర్మాణం commit *commit;
	పూర్ణాంకం i;

	కొరకు (i = extras->nr - 1; i >= 0; i--) {
		స్థిరరాశి అక్షరం *name = extras->వస్తువులు[i].string;
		నిర్మాణం object *object = extras->వస్తువులు[i].util;
		పూర్ణాంకం mark;

		ఎంపిక (object->type) {
		స్థితి OBJ_ట్యాగ్:
			handle_ట్యాగ్(name, (నిర్మాణం ట్యాగ్ *)object);
			ఆపు;
		స్థితి OBJ_COMMIT:
			ఒకవేళ (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((నిర్మాణం commit *)object);
			ఒకవేళ (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				రాయి("reset %s\nfrom %s\n\n",
				       name, oid_గమ్యం_hex(null_oid(the_hash_algo)));
				కొనసాగు;
			}

			mark = get_object_mark(&commit->object);
			ఒకవేళ (!mark) {
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
				ఒకవేళ (!reference_excluded_commits) {
					/* delete the ref */
					రాయి("reset %s\nfrom %s\n\n",
					       name, oid_గమ్యం_hex(null_oid(the_hash_algo)));
					కొనసాగు;
				}
				/* set ref to commit using oid, not mark */
				రాయి("reset %s\nfrom %s\n\n", name,
				       oid_గమ్యం_hex(&commit->object.oid));
				కొనసాగు;
			}

			రాయి("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_పురోగతి();
			ఆపు;
		}
	}
}

స్థిర శూన్య export_marks(అక్షరం *file)
{
	గుర్తులేని పూర్ణాంకం i;
	uint32_t mark;
	నిర్మాణం decoration_నమోదు *deco = idnums.entries;
	FILE *f;
	పూర్ణాంకం e = 0;

	f = fopen_for_writing(file);
	ఒకవేళ (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	కొరకు (i = 0; i < idnums.కొలత; i++) {
		ఒకవేళ (deco->పునాది && deco->పునాది->type == 1) {
			mark = ptr_గమ్యం_mark(deco->decoration);
			ఒకవేళ (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_గమ్యం_hex(&deco->పునాది->oid)) < 0) {
			    e = 1;
			    ఆపు;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	ఒకవేళ (e)
		error(_("unable to write marks file %s."), file);
}

స్థిర శూన్య import_marks(అక్షరం *ఆదానం_file, పూర్ణాంకం check_exists)
{
	అక్షరం పంక్తి[512];
	FILE *f;
	నిర్మాణం stat sb;

	ఒకవేళ (check_exists && stat(ఆదానం_file, &sb))
		తిరిగి;

	f = xfopen(ఆదానం_file, "r");
	అయితే (fgets(పంక్తి, పరిమాణం(పంక్తి), f)) {
		uint32_t mark;
		అక్షరం *పంక్తి_చివర, *mark_చివర;
		నిర్మాణం object_id oid;
		నిర్మాణం object *object;
		నిర్మాణం commit *commit;
		గణన object_type type;

		పంక్తి_చివర = strchr(పంక్తి, '\n');
		ఒకవేళ (పంక్తి[0] != ':' || !పంక్తి_చివర)
			die(_("corrupt mark line: %s"), పంక్తి);
		*పంక్తి_చివర = '\0';

		mark = strtoumax(పంక్తి + 1, &mark_చివర, 10);
		ఒకవేళ (!mark || mark_చివర == పంక్తి + 1
			|| *mark_చివర != ' ' || get_oid_hex(mark_చివర + 1, &oid))
			die(_("corrupt mark line: %s"), పంక్తి);

		ఒకవేళ (చివరి_idnum < mark)
			చివరి_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, ఖాళీ);
		ఒకవేళ (type < 0)
			die(_("object not found: %s"), oid_గమ్యం_hex(&oid));

		ఒకవేళ (type != OBJ_COMMIT)
			/* only commits */
			కొనసాగు;

		commit = lookup_commit(the_repository, &oid);
		ఒకవేళ (!commit)
			die(_("not a commit? can't happen: %s"), oid_గమ్యం_hex(&oid));

		object = &commit->object;

		ఒకవేళ (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_గమ్యం_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

స్థిర శూన్య handle_deletes(శూన్య)
{
	పూర్ణాంకం i;
	కొరకు (i = 0; i < refspecs.nr; i++) {
		నిర్మాణం refspec_అంశం *refspec = &refspecs.వస్తువులు[i];
		ఒకవేళ (*refspec->src)
			కొనసాగు;

		రాయి("reset %s\nfrom %s\n\n",
				refspec->dst, oid_గమ్యం_hex(null_oid(the_hash_algo)));
	}
}

స్థిర పూర్ణాంకం parse_opt_anonymize_map(స్థిరరాశి నిర్మాణం option *opt,
				   స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం hashmap *map = opt->విలువ;
	స్థిరరాశి అక్షరం *delim, *విలువ;
	కొలత_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	ఒకవేళ (delim) {
		keylen = delim - arg;
		విలువ = delim + 1;
	} లేకపోతే {
		keylen = అక్షరశ్రేణి_పొడవు(arg);
		విలువ = arg;
	}

	ఒకవేళ (!keylen || !*విలువ)
		తిరిగి error(_("--anonymize-map token cannot be empty"));

	కూడు_anonymized_నమోదు(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(విలువ));

	తిరిగి 0;
}

పూర్ణాంకం cmd_fast_export(పూర్ణాంకం argc,
		    స్థిరరాశి అక్షరం **argv,
		    స్థిరరాశి అక్షరం *ఉపసర్గ,
		    నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం rev_info revs;
	నిర్మాణం commit *commit;
	అక్షరం *export_filename = ఖాళీ,
	     *import_filename = ఖాళీ,
	     *import_filename_if_exists = ఖాళీ;
	uint32_t lastimportid;
	నిర్మాణం string_list refspecs_list = STRING_LIST_INIT_NODUP;
	నిర్మాణం string_list paths_యొక్క_మారింది_objects = STRING_LIST_INIT_DUP;
	నిర్మాణం option options[] = {
		OPT_INTEGER(0, "progress", &పురోగతి,
			    N_("show progress after <n> objects")),
		OPT_CALLBACK(0, "signed-tags", &signed_ట్యాగ్_బహుళకం, N_("mode"),
			     N_("select handling of signed tags"),
			     parse_opt_sign_బహుళకం),
		OPT_CALLBACK(0, "signed-commits", &signed_commit_బహుళకం, N_("mode"),
			     N_("select handling of signed commits"),
			     parse_opt_sign_బహుళకం),
		OPT_CALLBACK(0, "tag-of-filtered-object", &ట్యాగ్_యొక్క_filtered_బహుళకం, N_("mode"),
			     N_("select handling of tags that tag filtered objects"),
			     parse_opt_ట్యాగ్_యొక్క_filtered_బహుళకం),
		OPT_CALLBACK(0, "reencode", &reencode_బహుళకం, N_("mode"),
			     N_("select handling of commit messages in an alternate encoding"),
			     parse_opt_reencode_బహుళకం),
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
		OPT_BOOL(0, "full-tree", &నిండిన_వృక్షం,
			 N_("output full tree for each commit")),
		OPT_BOOL(0, "use-done-feature", &use_పూర్తి_feature,
			     N_("use the done feature to terminate the stream")),
		OPT_BOOL(0, "no-data", &no_దత్తాంశం, N_("skip output of blob data")),
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
		OPT_BOOL(0, "mark-tags", &mark_ట్యాగులు,
			    N_("label tags with mark ids")),

		OPT_చివర()
	};

	ఒకవేళ (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, ఖాళీ);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, ఉపసర్గ);
	init_revision_sources(&revision_sources);
	revs.topo_క్రమం = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, ఉపసర్గ, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = అమరిక_revisions(argc, argv, &revs, ఖాళీ);
	ఒకవేళ (argc > 1)
		usage_with_options (fast_export_usage, options);

	ఒకవేళ (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	ఒకవేళ (refspecs_list.nr) {
		పూర్ణాంకం i;

		కొరకు (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.వస్తువులు[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	ఒకవేళ (use_పూర్తి_feature)
		రాయి("feature done\n");

	ఒకవేళ (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	ఒకవేళ (import_filename)
		import_marks(import_filename, 0);
	లేకపోతే ఒకవేళ (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = చివరి_idnum;

	ఒకవేళ (import_filename && revs.prune_దత్తాంశం.nr)
		నిండిన_వృక్షం = 1;

	get_ట్యాగులు_and_duplicates(&revs.cmdline);

	ఒకవేళ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.తిరగేయి = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_దత్తాంశం = &paths_యొక్క_మారింది_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	అయితే ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_యొక్క_మారింది_objects);
	revs.diffopt.no_free = 0;

	handle_ట్యాగులు_and_duplicates(&extra_refs);
	handle_ట్యాగులు_and_duplicates(&ట్యాగ్_refs);
	handle_deletes();

	ఒకవేళ (export_filename && lastimportid != చివరి_idnum)
		export_marks(export_filename);

	ఒకవేళ (use_పూర్తి_feature)
		రాయి("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	తిరిగి 0;
}
