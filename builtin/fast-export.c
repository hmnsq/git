/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "refspec.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "diffcore.h"
#अन्तर्भाव "log-tree.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "decorate.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "utf8.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "gpg-interface.h"

स्थिर ध्रुव वर्ण *ध्रुव fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	रिक्त
};

स्थिर पूर्णाङ्क प्रगतिः;
स्थिर गणना sign_बहुलकम् signed_चिह्नकम्_बहुलकम् = SIGN_ABORT;
स्थिर गणना sign_बहुलकम् signed_commit_बहुलकम् = SIGN_STRIP;
स्थिर गणना चिह्नकम्_सम्बन्धि_filtered_बहुलकम् { चिह्नकम्_FILTERING_ABORT, DROP, REWRITE } चिह्नकम्_सम्बन्धि_filtered_बहुलकम् = चिह्नकम्_FILTERING_ABORT;
स्थिर गणना reencode_बहुलकम् { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_बहुलकम् = REENCODE_ABORT;
स्थिर पूर्णाङ्क fake_missing_tagger;
स्थिर पूर्णाङ्क use_समाप्तम्_feature;
स्थिर पूर्णाङ्क no_दत्त;
स्थिर पूर्णाङ्क पूर्णम्_वृक्षः;
स्थिर पूर्णाङ्क reference_excluded_commits;
स्थिर पूर्णाङ्क show_original_ids;
स्थिर पूर्णाङ्क mark_चिह्नकानि;
स्थिर संरचना string_list extra_refs = STRING_LIST_INIT_DUP;
स्थिर संरचना string_list चिह्नकम्_refs = STRING_LIST_INIT_DUP;
स्थिर संरचना refspec refspecs;
स्थिर पूर्णाङ्क anonymize;
स्थिर संरचना hashmap anonymized_seeds;
स्थिर संरचना revision_sources revision_sources;

स्थिर पूर्णाङ्क parse_opt_sign_बहुलकम्(ध्रुव संरचना option *opt,
			       ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	गणना sign_बहुलकम् *val = opt->मानम्;

	यदि (unset)
		प्रत्यागम 0;

	यदि (parse_sign_बहुलकम्(arg, val, रिक्त) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		प्रत्यागम error(_("unknown %s mode: %s"), opt->long_name, arg);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_opt_चिह्नकम्_सम्बन्धि_filtered_बहुलकम्(ध्रुव संरचना option *opt,
					  ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	गणना चिह्नकम्_सम्बन्धि_filtered_बहुलकम् *val = opt->मानम्;

	यदि (unset || !वाक्यं_तुलय(arg, "abort"))
		*val = चिह्नकम्_FILTERING_ABORT;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "drop"))
		*val = DROP;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "rewrite"))
		*val = REWRITE;
	अन्यथा
		प्रत्यागम error(_("unknown tag-of-filtered mode: %s"), arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_opt_reencode_बहुलकम्(ध्रुव संरचना option *opt,
				   ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	गणना reencode_बहुलकम् *val = opt->मानम्;

	यदि (unset) {
		*val = REENCODE_ABORT;
		प्रत्यागम 0;
	}

	विकल्प (git_parse_maybe_bool(arg)) {
	पक्ष 0:
		*val = REENCODE_NO;
		भङ्ग;
	पक्ष 1:
		*val = REENCODE_YES;
		भङ्ग;
	स्वभाव:
		यदि (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		अन्यथा
			प्रत्यागम error(_("unknown reencoding mode: %s"), arg);
	}

	प्रत्यागम 0;
}

स्थिर संरचना decoration idnums;
स्थिर uint32_t चरमः_idnum;
संरचना anonymized_प्रविष्टिः {
	संरचना hashmap_प्रविष्टिः hash;
	वर्ण *anon;
	ध्रुव वर्ण orig[FLEX_ARRAY];
};

संरचना anonymized_प्रविष्टिः_कुञ्चिका {
	संरचना hashmap_प्रविष्टिः hash;
	ध्रुव वर्ण *orig;
	आकार_t orig_len;
};

स्थिर पूर्णाङ्क anonymized_प्रविष्टिः_cmp(ध्रुव शून्य *cmp_दत्त अप्रयुक्तम्,
				ध्रुव संरचना hashmap_प्रविष्टिः *eptr,
				ध्रुव संरचना hashmap_प्रविष्टिः *प्रविष्टिः_or_कुञ्चिका,
				ध्रुव शून्य *keydata)
{
	ध्रुव संरचना anonymized_प्रविष्टिः *a, *b;

	a = container_सम्बन्धि(eptr, ध्रुव संरचना anonymized_प्रविष्टिः, hash);
	यदि (keydata) {
		ध्रुव संरचना anonymized_प्रविष्टिः_कुञ्चिका *कुञ्चिका = keydata;
		पूर्णाङ्क equal = !xstrncmpz(a->orig, कुञ्चिका->orig, कुञ्चिका->orig_len);
		प्रत्यागम !equal;
	}

	b = container_सम्बन्धि(प्रविष्टिः_or_कुञ्चिका, ध्रुव संरचना anonymized_प्रविष्टिः, hash);
	प्रत्यागम वाक्यं_तुलय(a->orig, b->orig);
}

स्थिर संरचना anonymized_प्रविष्टिः *सङ्कलय_anonymized_प्रविष्टिः(संरचना hashmap *map,
						     अचिह्नित hash,
						     ध्रुव वर्ण *orig, आकार_t len,
						     वर्ण *anon)
{
	संरचना anonymized_प्रविष्टिः *ret, *old;

	यदि (!map->cmpfn)
		hashmap_init(map, anonymized_प्रविष्टिः_cmp, रिक्त, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_प्रविष्टिः_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_प्रविष्टिः(map, ret, hash);

	यदि (old) {
		मुक्त(old->anon);
		मुक्त(old);
	}

	प्रत्यागम ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
स्थिर ध्रुव वर्ण *anonymize_str(संरचना hashmap *map,
				 वर्ण *(*generate)(शून्य),
				 ध्रुव वर्ण *orig, आकार_t len)
{
	संरचना anonymized_प्रविष्टिः_कुञ्चिका कुञ्चिका;
	संरचना anonymized_प्रविष्टिः *ret;

	hashmap_प्रविष्टिः_init(&कुञ्चिका.hash, memhash(orig, len));
	कुञ्चिका.orig = orig;
	कुञ्चिका.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_प्रविष्टिः(&anonymized_seeds, &कुञ्चिका, hash, &कुञ्चिका);

	/* ...otherwise check if we've already seen it in this context... */
	यदि (!ret)
		ret = hashmap_get_प्रविष्टिः(map, &कुञ्चिका, hash, &कुञ्चिका);

	/* ...and finally generate a new mapping if necessary */
	यदि (!ret)
		ret = सङ्कलय_anonymized_प्रविष्टिः(map, कुञ्चिका.hash.hash,
					   orig, len, generate());

	प्रत्यागम ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
स्थिर शून्य anonymize_मार्गः(संरचना strbuf *बहिर्गमनम्, ध्रुव वर्ण *मार्गः,
			   संरचना hashmap *map,
			   वर्ण *(*generate)(शून्य))
{
	यावत् (*मार्गः) {
		ध्रुव वर्ण *अन्तः_सम्बन्धि_component = strchrnul(मार्गः, '/');
		आकार_t len = अन्तः_सम्बन्धि_component - मार्गः;
		ध्रुव वर्ण *c = anonymize_str(map, generate, मार्गः, len);
		strbuf_addstr(बहिर्गमनम्, c);
		मार्गः = अन्तः_सम्बन्धि_component;
		यदि (*मार्गः)
			strbuf_addch(बहिर्गमनम्, *मार्गः++);
	}
}

स्थिर अन्तःस्थ शून्य *mark_गन्तव्यम्_ptr(uint32_t mark)
{
	प्रत्यागम (शून्य *)(uintptr_t)mark;
}

स्थिर अन्तःस्थ uint32_t ptr_गन्तव्यम्_mark(शून्य * mark)
{
	प्रत्यागम (uint32_t)(uintptr_t)mark;
}

स्थिर अन्तःस्थ शून्य mark_object(संरचना object *object, uint32_t mark)
{
	सङ्कलय_decoration(&idnums, object, mark_गन्तव्यम्_ptr(mark));
}

स्थिर अन्तःस्थ शून्य mark_अग्र_object(संरचना object *object)
{
	mark_object(object, ++चरमः_idnum);
}

स्थिर पूर्णाङ्क get_object_mark(संरचना object *object)
{
	शून्य *decoration = lookup_decoration(&idnums, object);
	यदि (!decoration)
		प्रत्यागम 0;
	प्रत्यागम ptr_गन्तव्यम्_mark(decoration);
}

स्थिर संरचना commit *rewrite_commit(संरचना commit *p)
{
	प्रति (;;) {
		यदि (p->parents && p->parents->अग्र)
			भङ्ग;
		यदि (p->object.flags & UNINTERESTING)
			भङ्ग;
		यदि (!(p->object.flags & TREESAME))
			भङ्ग;
		यदि (!p->parents)
			प्रत्यागम रिक्त;
		p = p->parents->item;
	}
	प्रत्यागम p;
}

स्थिर शून्य show_प्रगतिः(शून्य)
{
	स्थिर पूर्णाङ्क गणक = 0;
	यदि (!प्रगतिः)
		प्रत्यागम;
	यदि ((++गणक % प्रगतिः) == 0)
		प्रारूप_लिख("progress %d objects\n", गणक);
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
स्थिर वर्ण *anonymize_blob(अचिह्नित दीर्घ *आकार)
{
	स्थिर पूर्णाङ्क गणक;
	संरचना strbuf बहिर्गमनम् = STRBUF_INIT;
	strbuf_addf(&बहिर्गमनम्, "anonymous blob %d", गणक++);
	*आकार = बहिर्गमनम्.len;
	प्रत्यागम strbuf_detach(&बहिर्गमनम्, रिक्त);
}

स्थिर शून्य export_blob(ध्रुव संरचना object_id *oid)
{
	अचिह्नित दीर्घ आकार;
	गणना object_type type;
	वर्ण *buf;
	संरचना object *object;
	पूर्णाङ्क eaten;

	यदि (no_दत्त)
		प्रत्यागम;

	यदि (is_null_oid(oid))
		प्रत्यागम;

	object = lookup_object(the_repository, oid);
	यदि (object && object->flags & SHOWN)
		प्रत्यागम;

	यदि (anonymize) {
		buf = anonymize_blob(&आकार);
		object = (संरचना object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} अन्यथा {
		आकार_t आकार_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &आकार_st);
		आकार = cast_आकार_t_गन्तव्यम्_ulong(आकार_st);
		यदि (!buf)
			die(_("could not read blob %s"), oid_गन्तव्यम्_hex(oid));
		यदि (check_object_signature(the_repository, oid, buf, आकार,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_गन्तव्यम्_hex(oid));
		object = parse_object_बफर(the_repository, oid, type,
					     आकार, buf, &eaten);
	}

	यदि (!object)
		die(_("could not read blob %s"), oid_गन्तव्यम्_hex(oid));

	mark_अग्र_object(object);

	प्रारूप_लिख("blob\nmark :%"PRIu32"\n", चरमः_idnum);
	यदि (show_original_ids)
		प्रारूप_लिख("original-oid %s\n", oid_गन्तव्यम्_hex(oid));
	प्रारूप_लिख("data %"PRIuMAX"\n", (uintmax_t)आकार);
	यदि (आकार && fwrite(buf, आकार, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_गन्तव्यम्_hex(oid));
	प्रारूप_लिख("\n");

	show_प्रगतिः();

	object->flags |= SHOWN;
	यदि (!eaten)
		मुक्त(buf);
}

स्थिर पूर्णाङ्क गभीरता_first(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	ध्रुव संरचना diff_filepair *a = *((ध्रुव संरचना diff_filepair **)a_);
	ध्रुव संरचना diff_filepair *b = *((ध्रुव संरचना diff_filepair **)b_);
	ध्रुव वर्ण *name_a, *name_b;
	पूर्णाङ्क len_a, len_b, len;
	पूर्णाङ्क cmp;

	name_a = a->one ? a->one->मार्गः : a->two->मार्गः;
	name_b = b->one ? b->one->मार्गः : b->two->मार्गः;

	len_a = वाक्यदैर्घ्यम्(name_a);
	len_b = वाक्यदैर्घ्यम्(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	यदि (cmp)
		प्रत्यागम cmp;
	cmp = len_b - len_a;
	यदि (cmp)
		प्रत्यागम cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	प्रत्यागम (a->स्थितिः == 'R') - (b->स्थितिः == 'R');
}

स्थिर शून्य print_मार्गः_1(ध्रुव वर्ण *मार्गः)
{
	पूर्णाङ्क need_quote = quote_c_style(मार्गः, रिक्त, रिक्त, 0);
	यदि (need_quote)
		quote_c_style(मार्गः, रिक्त, stdout, 0);
	अन्यथा यदि (strchr(मार्गः, ' '))
		प्रारूप_लिख("\"%s\"", मार्गः);
	अन्यथा
		प्रारूप_लिख("%s", मार्गः);
}

स्थिर वर्ण *anonymize_मार्गः_component(शून्य)
{
	स्थिर पूर्णाङ्क गणक;
	संरचना strbuf बहिर्गमनम् = STRBUF_INIT;
	strbuf_addf(&बहिर्गमनम्, "path%d", गणक++);
	प्रत्यागम strbuf_detach(&बहिर्गमनम्, रिक्त);
}

स्थिर शून्य print_मार्गः(ध्रुव वर्ण *मार्गः)
{
	यदि (!anonymize)
		print_मार्गः_1(मार्गः);
	अन्यथा {
		स्थिर संरचना hashmap paths;
		स्थिर संरचना strbuf anon = STRBUF_INIT;

		anonymize_मार्गः(&anon, मार्गः, &paths, anonymize_मार्गः_component);
		print_मार्गः_1(anon.buf);
		strbuf_reset(&anon);
	}
}

स्थिर वर्ण *generate_fake_oid(शून्य)
{
	स्थिर uint32_t गणक = 1; /* avoid null oid */
	ध्रुव अचिह्नित hashsz = the_hash_algo->rawsz;
	संरचना object_id oid;
	वर्ण *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, गणक++);
	प्रत्यागम oid_गन्तव्यम्_hex_r(hex, &oid);
}

स्थिर ध्रुव वर्ण *anonymize_oid(ध्रुव वर्ण *oid_hex)
{
	स्थिर संरचना hashmap objs;
	आकार_t len = वाक्यदैर्घ्यम्(oid_hex);
	प्रत्यागम anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

स्थिर शून्य show_filemodify(संरचना diff_queue_struct *q,
			    संरचना diff_options *options अप्रयुक्तम्, शून्य *दत्त)
{
	पूर्णाङ्क i;
	संरचना string_list *परिवर्तितम् = दत्त;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, गभीरता_first);

	प्रति (i = 0; i < q->nr; i++) {
		संरचना diff_filespec *ospec = q->queue[i]->one;
		संरचना diff_filespec *spec = q->queue[i]->two;

		विकल्प (q->queue[i]->स्थितिः) {
		पक्ष DIFF_स्थितिः_DELETED:
			प्रारूप_लिख("D ");
			print_मार्गः(spec->मार्गः);
			string_list_निवेशय(परिवर्तितम्, spec->मार्गः);
			अक्षरं_लिख('\n');
			भङ्ग;

		पक्ष DIFF_स्थितिः_COPIED:
		पक्ष DIFF_स्थितिः_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			यदि (!string_list_has_string(परिवर्तितम्, ospec->मार्गः)) {
				प्रारूप_लिख("%c ", q->queue[i]->स्थितिः);
				print_मार्गः(ospec->मार्गः);
				अक्षरं_लिख(' ');
				print_मार्गः(spec->मार्गः);
				string_list_निवेशय(परिवर्तितम्, spec->मार्गः);
				अक्षरं_लिख('\n');

				यदि (oideq(&ospec->oid, &spec->oid) &&
				    ospec->बहुलकम् == spec->बहुलकम्)
					भङ्ग;
			}
			/* fallthrough */

		पक्ष DIFF_स्थितिः_TYPE_परिवर्तितम्:
		पक्ष DIFF_स्थितिः_MODIFIED:
		पक्ष DIFF_स्थितिः_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			यदि (no_दत्त || S_ISGITLINK(spec->बहुलकम्))
				प्रारूप_लिख("M %06o %s ", spec->बहुलकम्,
				       anonymize ?
				       anonymize_oid(oid_गन्तव्यम्_hex(&spec->oid)) :
				       oid_गन्तव्यम्_hex(&spec->oid));
			अन्यथा {
				संरचना object *object = lookup_object(the_repository,
								      &spec->oid);
				प्रारूप_लिख("M %06o :%d ", spec->बहुलकम्,
				       get_object_mark(object));
			}
			print_मार्गः(spec->मार्गः);
			string_list_निवेशय(परिवर्तितम्, spec->मार्गः);
			अक्षरं_लिख('\n');
			भङ्ग;

		स्वभाव:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->स्थितिः,
			    ospec->मार्गः ? ospec->मार्गः : _("none"),
			    spec->मार्गः ? spec->मार्गः : _("none"));
		}
	}
}

स्थिर वर्ण *anonymize_ref_component(शून्य)
{
	स्थिर पूर्णाङ्क गणक;
	संरचना strbuf बहिर्गमनम् = STRBUF_INIT;
	strbuf_addf(&बहिर्गमनम्, "ref%d", गणक++);
	प्रत्यागम strbuf_detach(&बहिर्गमनम्, रिक्त);
}

स्थिर ध्रुव वर्ण *anonymize_refname(ध्रुव वर्ण *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	स्थिर ध्रुव वर्ण *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	स्थिर संरचना hashmap refs;
	स्थिर संरचना strbuf anon = STRBUF_INIT;
	पूर्णाङ्क i;

	strbuf_reset(&anon);
	प्रति (i = 0; i < ARRAY_आकार(prefixes); i++) {
		यदि (skip_उपसर्गः(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			भङ्ग;
		}
	}

	anonymize_मार्गः(&anon, refname, &refs, anonymize_ref_component);
	प्रत्यागम anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
स्थिर वर्ण *anonymize_commit_सन्देशः(शून्य)
{
	स्थिर पूर्णाङ्क गणक;
	प्रत्यागम xstrfmt("subject %d\n\nbody\n", गणक++);
}

स्थिर वर्ण *anonymize_ident(शून्य)
{
	स्थिर पूर्णाङ्क गणक;
	संरचना strbuf बहिर्गमनम् = STRBUF_INIT;
	strbuf_addf(&बहिर्गमनम्, "User %d <user%d@example.com>", गणक, गणक);
	गणक++;
	प्रत्यागम strbuf_detach(&बहिर्गमनम्, रिक्त);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
स्थिर शून्य anonymize_ident_पङ्क्तिः(ध्रुव वर्ण **beg, ध्रुव वर्ण **अन्तः)
{
	स्थिर संरचना hashmap idents;
	स्थिर संरचना strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	स्थिर अचिह्नित which_बफर;

	संरचना strbuf *बहिर्गमनम्;
	संरचना ident_विभज विभज;
	ध्रुव वर्ण *अन्तः_सम्बन्धि_शीर्षकम्;

	बहिर्गमनम् = &buffers[which_बफर++];
	which_बफर %= ARRAY_आकार(buffers);
	strbuf_reset(बहिर्गमनम्);

	/* skip "committer", "author", "tagger", etc */
	अन्तः_सम्बन्धि_शीर्षकम् = strchr(*beg, ' ');
	यदि (!अन्तः_सम्बन्धि_शीर्षकम्)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (पूर्णाङ्क)(*अन्तः - *beg), *beg);
	अन्तः_सम्बन्धि_शीर्षकम्++;
	strbuf_सङ्कलय(बहिर्गमनम्, *beg, अन्तः_सम्बन्धि_शीर्षकम् - *beg);

	यदि (!विभज_ident_पङ्क्तिः(&विभज, अन्तः_सम्बन्धि_शीर्षकम्, *अन्तः - अन्तः_सम्बन्धि_शीर्षकम्) &&
	    विभज.date_begin) {
		ध्रुव वर्ण *ident;
		आकार_t len;

		len = विभज.mail_अन्तः - विभज.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      विभज.name_begin, len);
		strbuf_addstr(बहिर्गमनम्, ident);
		strbuf_addch(बहिर्गमनम्, ' ');
		strbuf_सङ्कलय(बहिर्गमनम्, विभज.date_begin, विभज.tz_अन्तः - विभज.date_begin);
	} अन्यथा {
		strbuf_addstr(बहिर्गमनम्, "Malformed Ident <malformed@example.com> 0 -0000");
	}

	*beg = बहिर्गमनम्->buf;
	*अन्तः = बहिर्गमनम्->buf + बहिर्गमनम्->len;
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
स्थिर ध्रुव वर्ण *find_commit_multiline_शीर्षकम्(ध्रुव वर्ण *सन्देशः,
						ध्रुव वर्ण *कुञ्चिका,
						ध्रुव वर्ण **अन्तः)
{
	संरचना strbuf val = STRBUF_INIT;
	ध्रुव वर्ण *bol, *eol;
	आकार_t len;

	bol = find_commit_शीर्षकम्(सन्देशः, कुञ्चिका, &len);
	यदि (!bol)
		प्रत्यागम रिक्त;
	eol = bol + len;
	strbuf_सङ्कलय(&val, bol, len);

	यावत् (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_सङ्कलय(&val, bol, eol - bol);
	}

	*अन्तः = eol;
	प्रत्यागम strbuf_detach(&val, रिक्त);
}

स्थिर शून्य print_signature(ध्रुव वर्ण *signature, ध्रुव वर्ण *object_hash)
{
	यदि (!signature)
		प्रत्यागम;

	प्रारूप_लिख("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (अचिह्नित)वाक्यदैर्घ्यम्(signature),
	       signature);
}

स्थिर ध्रुव वर्ण *append_signatures_for_शीर्षकम्(संरचना string_list *signatures,
						ध्रुव वर्ण *pos,
						ध्रुव वर्ण *शीर्षकम्,
						ध्रुव वर्ण *object_hash)
{
	ध्रुव वर्ण *signature;
	ध्रुव वर्ण *start = pos;
	ध्रुव वर्ण *अन्तः = pos;

	यावत् ((signature = find_commit_multiline_शीर्षकम्(start + 1,
							 शीर्षकम्,
							 &अन्तः))) {
		string_list_append(signatures, signature)->util = (शून्य *)object_hash;
		मुक्त((वर्ण *)signature);
		start = अन्तः;
	}

	प्रत्यागम अन्तः;
}

स्थिर शून्य handle_commit(संरचना commit *commit, संरचना rev_info *rev,
			  संरचना string_list *paths_सम्बन्धि_परिवर्तितम्_objects)
{
	पूर्णाङ्क saved_output_format = rev->diffopt.output_format;
	ध्रुव वर्ण *commit_बफर, *commit_बफर_cursor;
	ध्रुव वर्ण *author, *author_अन्तः, *committer, *committer_अन्तः;
	ध्रुव वर्ण *encoding = रिक्त;
	आकार_t encoding_len;
	संरचना string_list signatures = STRING_LIST_INIT_DUP;
	ध्रुव वर्ण *सन्देशः_2;
	वर्ण *reencoded = रिक्त;
	संरचना commit_list *p;
	ध्रुव वर्ण *refname;
	पूर्णाङ्क i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_बफर_cursor = commit_बफर = repo_get_commit_बफर(the_repository, commit, रिक्त);

	author = वाक्ये_अन्विष(commit_बफर_cursor, "\nauthor ");
	यदि (!author)
		die(_("could not find author in commit %s"),
		    oid_गन्तव्यम्_hex(&commit->object.oid));
	author++;
	commit_बफर_cursor = author_अन्तः = strchrnul(author, '\n');

	committer = वाक्ये_अन्विष(commit_बफर_cursor, "\ncommitter ");
	यदि (!committer)
		die(_("could not find committer in commit %s"),
		    oid_गन्तव्यम्_hex(&commit->object.oid));
	committer++;
	commit_बफर_cursor = committer_अन्तः = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	यदि (*commit_बफर_cursor == '\n') {
		encoding = find_commit_शीर्षकम्(commit_बफर_cursor + 1, "encoding", &encoding_len);
		यदि (encoding)
			commit_बफर_cursor = encoding + encoding_len;
	}

	यदि (*commit_बफर_cursor == '\n') {
		ध्रुव वर्ण *after_sha1 = append_signatures_for_शीर्षकम्(&signatures, commit_बफर_cursor,
								      "gpgsig", "sha1");
		ध्रुव वर्ण *after_sha256 = append_signatures_for_शीर्षकम्(&signatures, commit_बफर_cursor,
									"gpgsig-sha256", "sha256");
		commit_बफर_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	सन्देशः_2 = वाक्ये_अन्विष(commit_बफर_cursor, "\n\n");
	यदि (सन्देशः_2)
		सन्देशः_2 += 2;

	यदि (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !पूर्णम्_वृक्षः) {
		parse_commit_or_die(commit->parents->item);
		diff_वृक्षः_oid(get_commit_वृक्षः_oid(commit->parents->item),
			      get_commit_वृक्षः_oid(commit), "", &rev->diffopt);
	}
	अन्यथा
		diff_मूलम्_वृक्षः_oid(get_commit_वृक्षः_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	प्रति (i = 0; i < diff_queued_diff.nr; i++)
		यदि (!S_ISGITLINK(diff_queued_diff.queue[i]->two->बहुलकम्))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	यदि (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_पङ्क्तिः(&committer, &committer_अन्तः);
		anonymize_ident_पङ्क्तिः(&author, &author_अन्तः);
	}

	mark_अग्र_object(&commit->object);
	यदि (anonymize) {
		reencoded = anonymize_commit_सन्देशः();
	} अन्यथा यदि (encoding) {
		वर्ण *buf;
		विकल्प (reencode_बहुलकम्) {
		पक्ष REENCODE_YES:
			buf = xstrfmt("%.*s", (पूर्णाङ्क)encoding_len, encoding);
			reencoded = reencode_string(सन्देशः_2, "UTF-8", buf);
			मुक्त(buf);
			भङ्ग;
		पक्ष REENCODE_NO:
			भङ्ग;
		पक्ष REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (पूर्णाङ्क)encoding_len, encoding,
			    oid_गन्तव्यम्_hex(&commit->object.oid));
		}
	}
	यदि (!commit->parents)
		प्रारूप_लिख("reset %s\n", refname);
	प्रारूप_लिख("commit %s\nmark :%"PRIu32"\n", refname, चरमः_idnum);
	यदि (show_original_ids)
		प्रारूप_लिख("original-oid %s\n", oid_गन्तव्यम्_hex(&commit->object.oid));
	प्रारूप_लिख("%.*s\n%.*s\n",
	       (पूर्णाङ्क)(author_अन्तः - author), author,
	       (पूर्णाङ्क)(committer_अन्तः - committer), committer);
	यदि (signatures.nr) {
		विकल्प (signed_commit_बहुलकम्) {
		/* Exporting modes */
		पक्ष SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_गन्तव्यम्_hex(&commit->object.oid));
			/* fallthru */
		पक्ष SIGN_VERBATIM:
			प्रति (आकार_t i = 0; i < signatures.nr; i++) {
				संरचना string_list_item *item = &signatures.वस्तूनि[i];
				print_signature(item->string, item->util);
			}
			भङ्ग;

		/* Stripping modes */
		पक्ष SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_गन्तव्यम्_hex(&commit->object.oid));
			/* fallthru */
		पक्ष SIGN_STRIP:
			भङ्ग;

		/* Aborting modes */
		पक्ष SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_गन्तव्यम्_hex(&commit->object.oid));
		स्वभाव:
			BUG("invalid signed_commit_mode value %d", signed_commit_बहुलकम्);
		}
		string_list_clear(&signatures, 0);
	}
	यदि (!reencoded && encoding)
		प्रारूप_लिख("encoding %.*s\n", (पूर्णाङ्क)encoding_len, encoding);
	प्रारूप_लिख("data %u\n%s",
	       (अचिह्नित)(reencoded
			  ? वाक्यदैर्घ्यम्(reencoded) : सन्देशः_2
			  ? वाक्यदैर्घ्यम्(सन्देशः_2) : 0),
	       reencoded ? reencoded : सन्देशः_2 ? सन्देशः_2 : "");
	मुक्त(reencoded);
	repo_unuse_commit_बफर(the_repository, commit, commit_बफर);

	प्रति (i = 0, p = commit->parents; p; p = p->अग्र) {
		संरचना object *obj = &p->item->object;
		पूर्णाङ्क mark = get_object_mark(obj);

		यदि (!mark && !reference_excluded_commits)
			अनुवर्त;
		यदि (i == 0)
			प्रारूप_लिख("from ");
		अन्यथा
			प्रारूप_लिख("merge ");
		यदि (mark)
			प्रारूप_लिख(":%d\n", mark);
		अन्यथा
			प्रारूप_लिख("%s\n",
			       anonymize ?
			       anonymize_oid(oid_गन्तव्यम्_hex(&obj->oid)) :
			       oid_गन्तव्यम्_hex(&obj->oid));
		i++;
	}

	यदि (पूर्णम्_वृक्षः)
		प्रारूप_लिख("deleteall\n");
	log_वृक्षः_diff_flush(rev);
	string_list_clear(paths_सम्बन्धि_परिवर्तितम्_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	प्रारूप_लिख("\n");

	show_प्रगतिः();
}

स्थिर वर्ण *anonymize_चिह्नकम्(शून्य)
{
	स्थिर पूर्णाङ्क गणक;
	संरचना strbuf बहिर्गमनम् = STRBUF_INIT;
	strbuf_addf(&बहिर्गमनम्, "tag message %d", गणक++);
	प्रत्यागम strbuf_detach(&बहिर्गमनम्, रिक्त);
}


स्थिर शून्य handle_चिह्नकम्(ध्रुव वर्ण *name, संरचना चिह्नकम् *चिह्नकम्)
{
	आकार_t आकार;
	गणना object_type type;
	वर्ण *buf;
	ध्रुव वर्ण *tagger, *tagger_अन्तः, *सन्देशः_2;
	आकार_t सन्देशः_आकार = 0;
	संरचना object *tagged;
	पूर्णाङ्क tagged_mark;
	संरचना commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = चिह्नकम्->tagged;
	यावत् (tagged->type == OBJ_चिह्नकम्) {
		tagged = ((संरचना चिह्नकम् *)tagged)->tagged;
	}
	यदि (tagged->type == OBJ_वृक्षः) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_गन्तव्यम्_hex(&चिह्नकम्->object.oid));
		प्रत्यागम;
	}

	buf = odb_read_object(the_repository->objects, &चिह्नकम्->object.oid,
			      &type, &आकार);
	यदि (!buf)
		die(_("could not read tag %s"), oid_गन्तव्यम्_hex(&चिह्नकम्->object.oid));
	सन्देशः_2 = memmem(buf, आकार, "\n\n", 2);
	यदि (सन्देशः_2) {
		सन्देशः_2 += 2;
		सन्देशः_आकार = वाक्यदैर्घ्यम्(सन्देशः_2);
	}
	tagger = memmem(buf, सन्देशः_2 ? सन्देशः_2 - buf : आकार, "\ntagger ", 8);
	यदि (!tagger) {
		यदि (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		अन्यथा
			tagger = "";
		tagger_अन्तः = tagger + वाक्यदैर्घ्यम्(tagger);
	} अन्यथा {
		tagger++;
		tagger_अन्तः = strchrnul(tagger, '\n');
		यदि (anonymize)
			anonymize_ident_पङ्क्तिः(&tagger, &tagger_अन्तः);
	}

	यदि (anonymize) {
		name = anonymize_refname(name);
		यदि (सन्देशः_2) {
			स्थिर संरचना hashmap चिह्नकानि;
			सन्देशः_2 = anonymize_str(&चिह्नकानि, anonymize_चिह्नकम्,
						सन्देशः_2, सन्देशः_आकार);
			सन्देशः_आकार = वाक्यदैर्घ्यम्(सन्देशः_2);
		}
	}

	/* handle signed tags */
	यदि (सन्देशः_2) {
		आकार_t sig_विस्थापनम् = parse_signed_बफर(सन्देशः_2, सन्देशः_आकार);
		यदि (sig_विस्थापनम् < सन्देशः_आकार)
			विकल्प (signed_चिह्नकम्_बहुलकम्) {
			/* Exporting modes */
			पक्ष SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_गन्तव्यम्_hex(&चिह्नकम्->object.oid));
				/* fallthru */
			पक्ष SIGN_VERBATIM:
				भङ्ग;

			/* Stripping modes */
			पक्ष SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_गन्तव्यम्_hex(&चिह्नकम्->object.oid));
				/* fallthru */
			पक्ष SIGN_STRIP:
				सन्देशः_आकार = sig_विस्थापनम्;
				भङ्ग;

			/* Aborting modes */
			पक्ष SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_गन्तव्यम्_hex(&चिह्नकम्->object.oid));
			स्वभाव:
				BUG("invalid signed_commit_mode value %d", signed_commit_बहुलकम्);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = चिह्नकम्->tagged;
	tagged_mark = get_object_mark(tagged);
	यदि (!tagged_mark) {
		विकल्प (चिह्नकम्_सम्बन्धि_filtered_बहुलकम्) {
		पक्ष चिह्नकम्_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_गन्तव्यम्_hex(&चिह्नकम्->object.oid));
		पक्ष DROP:
			/* Ignore this tag altogether */
			मुक्त(buf);
			प्रत्यागम;
		पक्ष REWRITE:
			यदि (tagged->type == OBJ_चिह्नकम् && !mark_चिह्नकानि) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} अन्यथा यदि (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((संरचना commit *)tagged);
				यदि (!p) {
					प्रारूप_लिख("reset %s\nfrom %s\n\n",
					       name, oid_गन्तव्यम्_hex(null_oid(the_hash_algo)));
					मुक्त(buf);
					प्रत्यागम;
				}
				tagged_mark = get_object_mark(&p->object);
			} अन्यथा {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	यदि (tagged->type == OBJ_चिह्नकम्) {
		प्रारूप_लिख("reset %s\nfrom %s\n\n",
		       name, oid_गन्तव्यम्_hex(null_oid(the_hash_algo)));
	}
	skip_उपसर्गः(name, "refs/tags/", &name);
	प्रारूप_लिख("tag %s\n", name);
	यदि (mark_चिह्नकानि) {
		mark_अग्र_object(&चिह्नकम्->object);
		प्रारूप_लिख("mark :%"PRIu32"\n", चरमः_idnum);
	}
	यदि (tagged_mark)
		प्रारूप_लिख("from :%d\n", tagged_mark);
	अन्यथा
		प्रारूप_लिख("from %s\n", oid_गन्तव्यम्_hex(&tagged->oid));

	यदि (show_original_ids)
		प्रारूप_लिख("original-oid %s\n", oid_गन्तव्यम्_hex(&चिह्नकम्->object.oid));
	प्रारूप_लिख("%.*s%sdata %d\n%.*s\n",
	       (पूर्णाङ्क)(tagger_अन्तः - tagger), tagger,
	       tagger == tagger_अन्तः ? "" : "\n",
	       (पूर्णाङ्क)सन्देशः_आकार, (पूर्णाङ्क)सन्देशः_आकार, सन्देशः_2 ? सन्देशः_2 : "");
	मुक्त(buf);
}

स्थिर संरचना commit *get_commit(संरचना rev_cmdline_प्रविष्टिः *e, ध्रुव वर्ण *पूर्णम्_name)
{
	विकल्प (e->item->type) {
	पक्ष OBJ_COMMIT:
		प्रत्यागम (संरचना commit *)e->item;
	पक्ष OBJ_चिह्नकम्: {
		संरचना चिह्नकम् *चिह्नकम् = (संरचना चिह्नकम् *)e->item;

		/* handle nested tags */
		यावत् (चिह्नकम् && चिह्नकम्->object.type == OBJ_चिह्नकम्) {
			parse_object(the_repository, &चिह्नकम्->object.oid);
			string_list_append(&चिह्नकम्_refs, पूर्णम्_name)->util = चिह्नकम्;
			चिह्नकम् = (संरचना चिह्नकम् *)चिह्नकम्->tagged;
		}
		यदि (!चिह्नकम्)
			die(_("tag %s points nowhere?"), e->name);
		प्रत्यागम (संरचना commit *)चिह्नकम्;
	}
	स्वभाव:
		प्रत्यागम रिक्त;
	}
}

स्थिर शून्य get_चिह्नकानि_and_duplicates(संरचना rev_cmdline_info *info)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < info->nr; i++) {
		संरचना rev_cmdline_प्रविष्टिः *e = info->rev + i;
		संरचना object_id oid;
		संरचना commit *commit;
		वर्ण *पूर्णम्_name = रिक्त;

		यदि (e->flags & UNINTERESTING)
			अनुवर्त;

		यदि (repo_dwim_ref(the_repository, e->name, वाक्यदैर्घ्यम्(e->name),
				  &oid, &पूर्णम्_name, 0) != 1) {
			मुक्त(पूर्णम्_name);
			अनुवर्त;
		}

		यदि (refspecs.nr) {
			वर्ण *गुप्त;
			गुप्त = apply_refspecs(&refspecs, पूर्णम्_name);
			यदि (गुप्त) {
				मुक्त(पूर्णम्_name);
				पूर्णम्_name = गुप्त;
			}
		}

		commit = get_commit(e, पूर्णम्_name);
		यदि (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			मुक्त(पूर्णम्_name);
			अनुवर्त;
		}

		विकल्प (commit->object.type) {
		पक्ष OBJ_COMMIT:
			भङ्ग;
		पक्ष OBJ_BLOB:
			export_blob(&commit->object.oid);
			मुक्त(पूर्णम्_name);
			अनुवर्त;
		स्वभाव: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			मुक्त(पूर्णम्_name);
			अनुवर्त;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		यदि (e->item->type != OBJ_चिह्नकम्)
			string_list_append(&extra_refs, पूर्णम्_name)->util = commit;

		यदि (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = पूर्णम्_name;
		अन्यथा
			मुक्त(पूर्णम्_name);
	}

	string_list_क्रमय_u(&extra_refs, 0);
}

स्थिर शून्य handle_चिह्नकानि_and_duplicates(संरचना string_list *extras)
{
	संरचना commit *commit;
	पूर्णाङ्क i;

	प्रति (i = extras->nr - 1; i >= 0; i--) {
		ध्रुव वर्ण *name = extras->वस्तूनि[i].string;
		संरचना object *object = extras->वस्तूनि[i].util;
		पूर्णाङ्क mark;

		विकल्प (object->type) {
		पक्ष OBJ_चिह्नकम्:
			handle_चिह्नकम्(name, (संरचना चिह्नकम् *)object);
			भङ्ग;
		पक्ष OBJ_COMMIT:
			यदि (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((संरचना commit *)object);
			यदि (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				प्रारूप_लिख("reset %s\nfrom %s\n\n",
				       name, oid_गन्तव्यम्_hex(null_oid(the_hash_algo)));
				अनुवर्त;
			}

			mark = get_object_mark(&commit->object);
			यदि (!mark) {
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
				यदि (!reference_excluded_commits) {
					/* delete the ref */
					प्रारूप_लिख("reset %s\nfrom %s\n\n",
					       name, oid_गन्तव्यम्_hex(null_oid(the_hash_algo)));
					अनुवर्त;
				}
				/* set ref to commit using oid, not mark */
				प्रारूप_लिख("reset %s\nfrom %s\n\n", name,
				       oid_गन्तव्यम्_hex(&commit->object.oid));
				अनुवर्त;
			}

			प्रारूप_लिख("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_प्रगतिः();
			भङ्ग;
		}
	}
}

स्थिर शून्य export_marks(वर्ण *file)
{
	अचिह्नित पूर्णाङ्क i;
	uint32_t mark;
	संरचना decoration_प्रविष्टिः *deco = idnums.entries;
	FILE *f;
	पूर्णाङ्क e = 0;

	f = fopen_for_writing(file);
	यदि (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	प्रति (i = 0; i < idnums.आकार; i++) {
		यदि (deco->आधारः && deco->आधारः->type == 1) {
			mark = ptr_गन्तव्यम्_mark(deco->decoration);
			यदि (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_गन्तव्यम्_hex(&deco->आधारः->oid)) < 0) {
			    e = 1;
			    भङ्ग;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	यदि (e)
		error(_("unable to write marks file %s."), file);
}

स्थिर शून्य import_marks(वर्ण *निवेशः_file, पूर्णाङ्क check_exists)
{
	वर्ण पङ्क्तिः[512];
	FILE *f;
	संरचना stat sb;

	यदि (check_exists && stat(निवेशः_file, &sb))
		प्रत्यागम;

	f = xfopen(निवेशः_file, "r");
	यावत् (fgets(पङ्क्तिः, परिमाण(पङ्क्तिः), f)) {
		uint32_t mark;
		वर्ण *पङ्क्तिः_अन्तः, *mark_अन्तः;
		संरचना object_id oid;
		संरचना object *object;
		संरचना commit *commit;
		गणना object_type type;

		पङ्क्तिः_अन्तः = strchr(पङ्क्तिः, '\n');
		यदि (पङ्क्तिः[0] != ':' || !पङ्क्तिः_अन्तः)
			die(_("corrupt mark line: %s"), पङ्क्तिः);
		*पङ्क्तिः_अन्तः = '\0';

		mark = strtoumax(पङ्क्तिः + 1, &mark_अन्तः, 10);
		यदि (!mark || mark_अन्तः == पङ्क्तिः + 1
			|| *mark_अन्तः != ' ' || get_oid_hex(mark_अन्तः + 1, &oid))
			die(_("corrupt mark line: %s"), पङ्क्तिः);

		यदि (चरमः_idnum < mark)
			चरमः_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, रिक्त);
		यदि (type < 0)
			die(_("object not found: %s"), oid_गन्तव्यम्_hex(&oid));

		यदि (type != OBJ_COMMIT)
			/* only commits */
			अनुवर्त;

		commit = lookup_commit(the_repository, &oid);
		यदि (!commit)
			die(_("not a commit? can't happen: %s"), oid_गन्तव्यम्_hex(&oid));

		object = &commit->object;

		यदि (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_गन्तव्यम्_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

स्थिर शून्य handle_deletes(शून्य)
{
	पूर्णाङ्क i;
	प्रति (i = 0; i < refspecs.nr; i++) {
		संरचना refspec_item *refspec = &refspecs.वस्तूनि[i];
		यदि (*refspec->src)
			अनुवर्त;

		प्रारूप_लिख("reset %s\nfrom %s\n\n",
				refspec->dst, oid_गन्तव्यम्_hex(null_oid(the_hash_algo)));
	}
}

स्थिर पूर्णाङ्क parse_opt_anonymize_map(ध्रुव संरचना option *opt,
				   ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना hashmap *map = opt->मानम्;
	ध्रुव वर्ण *delim, *मानम्;
	आकार_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	यदि (delim) {
		keylen = delim - arg;
		मानम् = delim + 1;
	} अन्यथा {
		keylen = वाक्यदैर्घ्यम्(arg);
		मानम् = arg;
	}

	यदि (!keylen || !*मानम्)
		प्रत्यागम error(_("--anonymize-map token cannot be empty"));

	सङ्कलय_anonymized_प्रविष्टिः(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(मानम्));

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_fast_export(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *उपसर्गः,
		    संरचना repository *repo अप्रयुक्तम्)
{
	संरचना rev_info revs;
	संरचना commit *commit;
	वर्ण *export_filename = रिक्त,
	     *import_filename = रिक्त,
	     *import_filename_if_exists = रिक्त;
	uint32_t lastimportid;
	संरचना string_list refspecs_list = STRING_LIST_INIT_NODUP;
	संरचना string_list paths_सम्बन्धि_परिवर्तितम्_objects = STRING_LIST_INIT_DUP;
	संरचना option options[] = {
		OPT_INTEGER(0, "progress", &प्रगतिः,
			    N_("show progress after <n> objects")),
		OPT_CALLBACK(0, "signed-tags", &signed_चिह्नकम्_बहुलकम्, N_("mode"),
			     N_("select handling of signed tags"),
			     parse_opt_sign_बहुलकम्),
		OPT_CALLBACK(0, "signed-commits", &signed_commit_बहुलकम्, N_("mode"),
			     N_("select handling of signed commits"),
			     parse_opt_sign_बहुलकम्),
		OPT_CALLBACK(0, "tag-of-filtered-object", &चिह्नकम्_सम्बन्धि_filtered_बहुलकम्, N_("mode"),
			     N_("select handling of tags that tag filtered objects"),
			     parse_opt_चिह्नकम्_सम्बन्धि_filtered_बहुलकम्),
		OPT_CALLBACK(0, "reencode", &reencode_बहुलकम्, N_("mode"),
			     N_("select handling of commit messages in an alternate encoding"),
			     parse_opt_reencode_बहुलकम्),
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
		OPT_BOOL(0, "full-tree", &पूर्णम्_वृक्षः,
			 N_("output full tree for each commit")),
		OPT_BOOL(0, "use-done-feature", &use_समाप्तम्_feature,
			     N_("use the done feature to terminate the stream")),
		OPT_BOOL(0, "no-data", &no_दत्त, N_("skip output of blob data")),
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
		OPT_BOOL(0, "mark-tags", &mark_चिह्नकानि,
			    N_("label tags with mark ids")),

		OPT_अन्तः()
	};

	यदि (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, रिक्त);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, उपसर्गः);
	init_revision_sources(&revision_sources);
	revs.topo_क्रमः = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, उपसर्गः, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = सज्जता_revisions(argc, argv, &revs, रिक्त);
	यदि (argc > 1)
		usage_with_options (fast_export_usage, options);

	यदि (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	यदि (refspecs_list.nr) {
		पूर्णाङ्क i;

		प्रति (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.वस्तूनि[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	यदि (use_समाप्तम्_feature)
		प्रारूप_लिख("feature done\n");

	यदि (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	यदि (import_filename)
		import_marks(import_filename, 0);
	अन्यथा यदि (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = चरमः_idnum;

	यदि (import_filename && revs.prune_दत्त.nr)
		पूर्णम्_वृक्षः = 1;

	get_चिह्नकानि_and_duplicates(&revs.cmdline);

	यदि (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_दत्त = &paths_सम्बन्धि_परिवर्तितम्_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	यावत् ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_सम्बन्धि_परिवर्तितम्_objects);
	revs.diffopt.no_free = 0;

	handle_चिह्नकानि_and_duplicates(&extra_refs);
	handle_चिह्नकानि_and_duplicates(&चिह्नकम्_refs);
	handle_deletes();

	यदि (export_filename && lastimportid != चरमः_idnum)
		export_marks(export_filename);

	यदि (use_समाप्तम्_feature)
		प्रारूप_लिख("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	प्रत्यागम 0;
}
