#परिभाषय USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "xdiff-interface.h"
#अन्तर्भाव "help.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "merge-ort.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "merge-blobs.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "write-or-die.h"

स्थिर पूर्णाङ्क पङ्क्तिः_termination = '\n';

संरचना merge_list {
	संरचना merge_list *अग्र;
	संरचना merge_list *link;	/* other stages for this object */

	अचिह्नित पूर्णाङ्क stage : 2;
	अचिह्नित पूर्णाङ्क बहुलकम्;
	ध्रुव वर्ण *मार्गः;
	संरचना blob *blob;
};

स्थिर संरचना merge_list *merge_परिणाम, **merge_परिणाम_अन्तः = &merge_परिणाम;

स्थिर शून्य सङ्कलय_merge_प्रविष्टिः(संरचना merge_list *प्रविष्टिः)
{
	*merge_परिणाम_अन्तः = प्रविष्टिः;
	merge_परिणाम_अन्तः = &प्रविष्टिः->अग्र;
}

स्थिर शून्य trivial_merge_trees(संरचना वृक्षः_desc t[3], ध्रुव वर्ण *आधारः);

स्थिर ध्रुव वर्ण *explanation(संरचना merge_list *प्रविष्टिः)
{
	विकल्प (प्रविष्टिः->stage) {
	पक्ष 0:
		प्रत्यागम "merged";
	पक्ष 3:
		प्रत्यागम "added in remote";
	पक्ष 2:
		यदि (प्रविष्टिः->link)
			प्रत्यागम "added in both";
		प्रत्यागम "added in local";
	}

	/* Existed in base */
	प्रविष्टिः = प्रविष्टिः->link;
	यदि (!प्रविष्टिः)
		प्रत्यागम "removed in both";

	यदि (प्रविष्टिः->link)
		प्रत्यागम "changed in both";

	यदि (प्रविष्टिः->stage == 3)
		प्रत्यागम "removed in local";
	प्रत्यागम "removed in remote";
}

स्थिर शून्य *परिणाम(संरचना merge_list *प्रविष्टिः, आकार_t *आकार)
{
	गणना object_type type;
	संरचना blob *आधारः, *our, *their;
	ध्रुव वर्ण *मार्गः = प्रविष्टिः->मार्गः;

	यदि (!प्रविष्टिः->stage)
		प्रत्यागम odb_read_object(the_repository->objects,
				       &प्रविष्टिः->blob->object.oid, &type,
				       आकार);
	आधारः = रिक्त;
	यदि (प्रविष्टिः->stage == 1) {
		आधारः = प्रविष्टिः->blob;
		प्रविष्टिः = प्रविष्टिः->link;
	}
	our = रिक्त;
	यदि (प्रविष्टिः && प्रविष्टिः->stage == 2) {
		our = प्रविष्टिः->blob;
		प्रविष्टिः = प्रविष्टिः->link;
	}
	their = रिक्त;
	यदि (प्रविष्टिः)
		their = प्रविष्टिः->blob;
	प्रत्यागम merge_blobs(the_repository->सूचकांक, मार्गः,
			   आधारः, our, their, आकार);
}

स्थिर शून्य *origin(संरचना merge_list *प्रविष्टिः, आकार_t *आकार)
{
	गणना object_type type;
	यावत् (प्रविष्टिः) {
		यदि (प्रविष्टिः->stage == 2)
			प्रत्यागम odb_read_object(the_repository->objects,
					       &प्रविष्टिः->blob->object.oid,
					       &type, आकार);
		प्रविष्टिः = प्रविष्टिः->link;
	}
	प्रत्यागम रिक्त;
}

स्थिर पूर्णाङ्क show_outf(शून्य *priv अप्रयुक्तम्, mmfile_t *mb, पूर्णाङ्क nbuf)
{
	पूर्णाङ्क i;
	प्रति (i = 0; i < nbuf; i++)
		प्रारूप_लिख("%.*s", (पूर्णाङ्क) mb[i].आकार, mb[i].ptr);
	प्रत्यागम 0;
}

स्थिर शून्य show_diff(संरचना merge_list *प्रविष्टिः)
{
	आकार_t आकार;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .बहिर्गमनम्_पङ्क्तिः = show_outf };

	स्मृतिं_पूरय(&xpp, 0, परिमाण(xpp));
	xpp.flags = 0;
	स्मृतिं_पूरय(&xecfg, 0, परिमाण(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(प्रविष्टिः, &आकार);
	यदि (!src.ptr)
		आकार = 0;
	src.आकार = आकार;
	dst.ptr = परिणाम(प्रविष्टिः, &आकार);
	यदि (!dst.ptr)
		आकार = 0;
	dst.आकार = आकार;
	यदि (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	मुक्त(src.ptr);
	मुक्त(dst.ptr);
}

स्थिर शून्य show_परिणाम_list(संरचना merge_list *प्रविष्टिः)
{
	प्रारूप_लिख("%s\n", explanation(प्रविष्टिः));
	कुरु {
		संरचना merge_list *link = प्रविष्टिः->link;
		स्थिर ध्रुव वर्ण *desc[4] = { "result", "base", "our", "their" };
		प्रारूप_लिख("  %-6s %o %s %s\n", desc[प्रविष्टिः->stage], प्रविष्टिः->बहुलकम्, oid_गन्तव्यम्_hex(&प्रविष्टिः->blob->object.oid), प्रविष्टिः->मार्गः);
		प्रविष्टिः = link;
	} यावत् (प्रविष्टिः);
}

स्थिर शून्य show_परिणाम(शून्य)
{
	संरचना merge_list *walk;

	walk = merge_परिणाम;
	यावत् (walk) {
		show_परिणाम_list(walk);
		show_diff(walk);
		walk = walk->अग्र;
	}
}

/* An empty entry never compares same, not even to another empty entry */
स्थिर पूर्णाङ्क same_प्रविष्टिः(संरचना name_प्रविष्टिः *a, संरचना name_प्रविष्टिः *b)
{
	प्रत्यागम	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->बहुलकम् == b->बहुलकम्;
}

स्थिर पूर्णाङ्क both_रिक्तानि(संरचना name_प्रविष्टिः *a, संरचना name_प्रविष्टिः *b)
{
	प्रत्यागम is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

स्थिर संरचना merge_list *create_प्रविष्टिः(अचिह्नित stage, अचिह्नित बहुलकम्, ध्रुव संरचना object_id *oid, ध्रुव वर्ण *मार्गः)
{
	संरचना merge_list *res = xcalloc(1, परिमाण(*res));

	res->stage = stage;
	res->मार्गः = मार्गः;
	res->बहुलकम् = बहुलकम्;
	res->blob = lookup_blob(the_repository, oid);
	प्रत्यागम res;
}

स्थिर वर्ण *traverse_मार्गः(ध्रुव संरचना traverse_info *info, ध्रुव संरचना name_प्रविष्टिः *n)
{
	संरचना strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_मार्गः(&buf, info, n->मार्गः, n->pathlen);
	प्रत्यागम strbuf_detach(&buf, रिक्त);
}

स्थिर शून्य resolve(ध्रुव संरचना traverse_info *info, संरचना name_प्रविष्टिः *ours, संरचना name_प्रविष्टिः *परिणाम)
{
	संरचना merge_list *orig, *final;
	ध्रुव वर्ण *मार्गः;

	/* If it's already ours, don't bother showing it */
	यदि (!ours)
		प्रत्यागम;

	मार्गः = traverse_मार्गः(info, परिणाम);
	orig = create_प्रविष्टिः(2, ours->बहुलकम्, &ours->oid, मार्गः);
	final = create_प्रविष्टिः(0, परिणाम->बहुलकम्, &परिणाम->oid, मार्गः);

	final->link = orig;

	सङ्कलय_merge_प्रविष्टिः(final);
}

स्थिर शून्य unresolved_directory(ध्रुव संरचना traverse_info *info,
				 संरचना name_प्रविष्टिः n[3])
{
	संरचना repository *r = the_repository;
	वर्ण *newbase;
	संरचना name_प्रविष्टिः *p;
	संरचना वृक्षः_desc t[3];
	शून्य *buf0, *buf1, *buf2;

	प्रति (p = n; p < n + 3; p++) {
		यदि (p->बहुलकम् && S_ISDIR(p->बहुलकम्))
			भङ्ग;
	}
	यदि (n + 3 <= p)
		प्रत्यागम; /* there is no tree here */

	newbase = traverse_मार्गः(info, p);

#परिभाषय प्रविष्टिः_OID(e) (((e)->बहुलकम् && S_ISDIR((e)->बहुलकम्)) ? &(e)->oid : रिक्त)
	buf0 = fill_वृक्षः_descriptor(r, t + 0, प्रविष्टिः_OID(n + 0));
	buf1 = fill_वृक्षः_descriptor(r, t + 1, प्रविष्टिः_OID(n + 1));
	buf2 = fill_वृक्षः_descriptor(r, t + 2, प्रविष्टिः_OID(n + 2));
#अपरिभाषय प्रविष्टिः_OID

	trivial_merge_trees(t, newbase);

	मुक्त(buf0);
	मुक्त(buf1);
	मुक्त(buf2);
	मुक्त(newbase);
}


स्थिर संरचना merge_list *link_प्रविष्टिः(अचिह्नित stage, ध्रुव संरचना traverse_info *info, संरचना name_प्रविष्टिः *n, संरचना merge_list *प्रविष्टिः)
{
	ध्रुव वर्ण *मार्गः;
	संरचना merge_list *link;

	यदि (!n->बहुलकम्)
		प्रत्यागम प्रविष्टिः;
	यदि (प्रविष्टिः)
		मार्गः = प्रविष्टिः->मार्गः;
	अन्यथा
		मार्गः = traverse_मार्गः(info, n);
	link = create_प्रविष्टिः(stage, n->बहुलकम्, &n->oid, मार्गः);
	link->link = प्रविष्टिः;
	प्रत्यागम link;
}

स्थिर शून्य unresolved(ध्रुव संरचना traverse_info *info, संरचना name_प्रविष्टिः n[3])
{
	संरचना merge_list *प्रविष्टिः = रिक्त;
	पूर्णाङ्क i;
	अचिह्नित dirmask = 0, आवरणम् = 0;

	प्रति (i = 0; i < 3; i++) {
		आवरणम् |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		यदि (!n[i].बहुलकम् || S_ISDIR(n[i].बहुलकम्))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	यदि (dirmask == आवरणम्)
		प्रत्यागम;

	यदि (n[2].बहुलकम् && !S_ISDIR(n[2].बहुलकम्))
		प्रविष्टिः = link_प्रविष्टिः(3, info, n + 2, प्रविष्टिः);
	यदि (n[1].बहुलकम् && !S_ISDIR(n[1].बहुलकम्))
		प्रविष्टिः = link_प्रविष्टिः(2, info, n + 1, प्रविष्टिः);
	यदि (n[0].बहुलकम् && !S_ISDIR(n[0].बहुलकम्))
		प्रविष्टिः = link_प्रविष्टिः(1, info, n + 0, प्रविष्टिः);

	सङ्कलय_merge_प्रविष्टिः(प्रविष्टिः);
}

/*
 * Merge two trees together (t[1] and t[2]), using a common base (t[0])
 * as the origin.
 *
 * This walks the (sorted) trees in lock-step, checking every possible
 * name. Note that directories automatically sort differently from other
 * files (see "base_name_compare"), so you'll never see file/directory
 * conflicts, because they won't ever compare the same.
 *
 * IOW, if a directory changes to a filename, it will automatically be
 * seen as the directory going away, and the filename being created.
 *
 * Think of this as a three-way diff.
 *
 * The output will be either:
 *  - successful merge
 *	 "0 mode sha1 filename"
 *    NOTE NOTE NOTE! FIXME! We really really need to walk the index
 *    in parallel with this too!
 *
 *  - conflict:
 *	"1 mode sha1 filename"
 *	"2 mode sha1 filename"
 *	"3 mode sha1 filename"
 *    where not all of the 1/2/3 lines may exist, of course.
 *
 * The successful merge rules are the same as for the three-way merge
 * in git-read-tree.
 */
स्थिर पूर्णाङ्क threeway_callback(पूर्णाङ्क n अप्रयुक्तम्, अचिह्नित दीर्घ आवरणम्,
			     अचिह्नित दीर्घ dirmask अप्रयुक्तम्,
			     संरचना name_प्रविष्टिः *प्रविष्टिः, संरचना traverse_info *info)
{
	/* Same in both? */
	यदि (same_प्रविष्टिः(प्रविष्टिः+1, प्रविष्टिः+2) || both_रिक्तानि(प्रविष्टिः+1, प्रविष्टिः+2)) {
		/* Modified, added or removed identically */
		resolve(info, रिक्त, प्रविष्टिः+1);
		प्रत्यागम आवरणम्;
	}

	यदि (same_प्रविष्टिः(प्रविष्टिः+0, प्रविष्टिः+1)) {
		यदि (!is_null_oid(&प्रविष्टिः[2].oid) && !S_ISDIR(प्रविष्टिः[2].बहुलकम्)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, प्रविष्टिः+1, प्रविष्टिः+2);
			प्रत्यागम आवरणम्;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	यदि (same_प्रविष्टिः(प्रविष्टिः+0, प्रविष्टिः+2) || both_रिक्तानि(प्रविष्टिः+0, प्रविष्टिः+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, रिक्त, प्रविष्टिः+1);
		प्रत्यागम आवरणम्;
	}

	unresolved(info, प्रविष्टिः);
	प्रत्यागम आवरणम्;
}

स्थिर शून्य trivial_merge_trees(संरचना वृक्षः_desc t[3], ध्रुव वर्ण *आधारः)
{
	संरचना traverse_info info;

	सज्जता_traverse_info(&info, आधारः);
	info.fn = threeway_callback;
	traverse_trees(the_repository->सूचकांक, 3, t, &info);
}

स्थिर शून्य *get_वृक्षः_descriptor(संरचना repository *r,
				 संरचना वृक्षः_desc *desc,
				 ध्रुव वर्ण *rev)
{
	संरचना object_id oid;
	शून्य *buf;

	यदि (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_वृक्षः_descriptor(r, desc, &oid);
	यदि (!buf)
		die("%s is not a tree", rev);
	प्रत्यागम buf;
}

स्थिर पूर्णाङ्क trivial_merge(ध्रुव वर्ण *आधारः,
			 ध्रुव वर्ण *branch1,
			 ध्रुव वर्ण *branch2)
{
	संरचना repository *r = the_repository;
	संरचना वृक्षः_desc t[3];
	शून्य *buf1, *buf2, *buf3;

	buf1 = get_वृक्षः_descriptor(r, t+0, आधारः);
	buf2 = get_वृक्षः_descriptor(r, t+1, branch1);
	buf3 = get_वृक्षः_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	मुक्त(buf1);
	मुक्त(buf2);
	मुक्त(buf3);

	show_परिणाम();
	प्रत्यागम 0;
}

गणना बहुलकम् {
	बहुलकम्_UNKNOWN,
	बहुलकम्_TRIVIAL,
	बहुलकम्_REAL,
};

संरचना merge_वृक्षः_options {
	पूर्णाङ्क बहुलकम्;
	पूर्णाङ्क allow_unrelated_histories;
	पूर्णाङ्क show_messages;
	पूर्णाङ्क name_only;
	पूर्णाङ्क use_stdin;
	संरचना merge_options merge_options;
};

स्थिर पूर्णाङ्क real_merge(संरचना merge_वृक्षः_options *o,
		      ध्रुव वर्ण *merge_आधारः,
		      ध्रुव वर्ण *branch1, ध्रुव वर्ण *branch2,
		      ध्रुव वर्ण *उपसर्गः)
{
	संरचना commit *parent1, *parent2;
	संरचना commit_list *merge_bases = रिक्त;
	संरचना merge_परिणाम परिणाम = { 0 };
	पूर्णाङ्क show_messages = o->show_messages;
	संरचना merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_प्रगतिः = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	यदि (merge_आधारः) {
		संरचना वृक्षः *आधारः_वृक्षः, *parent1_वृक्षः, *parent2_वृक्षः;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		संरचना object_id आधारः_oid, शीर्ष_oid, merge_oid;

		यदि (repo_get_oid_treeish(the_repository, merge_आधारः, &आधारः_oid))
			die(_("could not parse as tree '%s'"), merge_आधारः);
		आधारः_वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository,
						     &आधारः_oid);
		यदि (!आधारः_वृक्षः)
			die(_("unable to read tree (%s)"), oid_गन्तव्यम्_hex(&आधारः_oid));
		यदि (repo_get_oid_treeish(the_repository, branch1, &शीर्ष_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository,
							&शीर्ष_oid);
		यदि (!parent1_वृक्षः)
			die(_("unable to read tree (%s)"), oid_गन्तव्यम्_hex(&शीर्ष_oid));
		यदि (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository,
							&merge_oid);
		यदि (!parent2_वृक्षः)
			die(_("unable to read tree (%s)"), oid_गन्तव्यम्_hex(&merge_oid));

		opt.ancestor = merge_आधारः;
		merge_incore_nonrecursive(&opt, आधारः_वृक्षः, parent1_वृक्षः, parent2_वृक्षः, &परिणाम);
	} अन्यथा {
		parent1 = get_merge_parent(branch1);
		यदि (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		यदि (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		यदि (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			निर्गच्छ(128);
		यदि (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &परिणाम);
		commit_list_free(merge_bases);
	}

	यदि (परिणाम.clean < 0)
		die(_("failure to merge"));

	यदि (o->merge_options.mergeability_only)
		गच्छ cleanup;

	यदि (show_messages == -1)
		show_messages = !परिणाम.clean;

	यदि (o->use_stdin)
		प्रारूप_लिख("%d%c", परिणाम.clean, पङ्क्तिः_termination);
	प्रारूप_लिख("%s%c", oid_गन्तव्यम्_hex(&परिणाम.वृक्षः->object.oid), पङ्क्तिः_termination);
	यदि (!परिणाम.clean) {
		संरचना string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ध्रुव वर्ण *चरमः = रिक्त;

		merge_get_conflicted_files(&परिणाम, &conflicted_files);
		प्रति (आकार_t i = 0; i < conflicted_files.nr; i++) {
			ध्रुव वर्ण *name = conflicted_files.वस्तूनि[i].string;
			संरचना stage_info *c = conflicted_files.वस्तूनि[i].util;
			यदि (!o->name_only)
				प्रारूप_लिख("%06o %s %d\t",
				       c->बहुलकम्, oid_गन्तव्यम्_hex(&c->oid), c->stage);
			अन्यथा यदि (चरमः && !वाक्यं_तुलय(चरमः, name))
				अनुवर्त;
			write_name_quoted_relative(
				name, उपसर्गः, stdout, पङ्क्तिः_termination);
			चरमः = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	यदि (show_messages) {
		अक्षरं_लिख(पङ्क्तिः_termination);
		merge_display_update_messages(&opt, पङ्क्तिः_termination == '\0',
					      &परिणाम);
	}
	यदि (o->use_stdin)
		अक्षरं_लिख(पङ्क्तिः_termination);

cleanup:
	merge_finalize(&opt, &परिणाम);
	clear_merge_options(&opt);
	प्रत्यागम !परिणाम.clean; /* result.clean < 0 handled above */
}

पूर्णाङ्क cmd_merge_वृक्षः(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *उपसर्गः,
		   संरचना repository *repo अप्रयुक्तम्)
{
	संरचना merge_वृक्षः_options o = { .show_messages = -1 };
	संरचना strvec xopts = STRVEC_INIT;
	पूर्णाङ्क expected_remaining_argc;
	पूर्णाङ्क original_argc;
	ध्रुव वर्ण *merge_आधारः = रिक्त;
	पूर्णाङ्क ret;
	पूर्णाङ्क quiet = 0;

	ध्रुव वर्ण * ध्रुव merge_वृक्षः_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		रिक्त
	};
	संरचना option mt_options[] = {
		OPT_CMDMODE(0, "write-tree", &o.बहुलकम्,
			    N_("do a real merge instead of a trivial merge"),
			    बहुलकम्_REAL),
		OPT_CMDMODE(0, "trivial-merge", &o.बहुलकम्,
			    N_("do a trivial merge only"), बहुलकम्_TRIVIAL),
		OPT_BOOL(0, "messages", &o.show_messages,
			 N_("also show informational/conflict messages")),
		OPT_BOOL_F(0, "quiet",
			   &quiet,
			   N_("suppress all output; only exit status wanted"),
			   PARSE_OPT_NONEG),
		OPT_समुच्चयः_INT('z', रिक्त, &पङ्क्तिः_termination,
			    N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL_F(0, "name-only",
			   &o.name_only,
			   N_("list filenames without modes/oids/stages"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "allow-unrelated-histories",
			   &o.allow_unrelated_histories,
			   N_("allow merging unrelated histories"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin",
			   &o.use_stdin,
			   N_("perform multiple merges, one per line of input"),
			   PARSE_OPT_NONEG),
		OPT_STRING(0, "merge-base",
			   &merge_आधारः,
			   N_("tree-ish"),
			   N_("specify a merge-base for the merge")),
		OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
			N_("option for selected merge strategy")),
		OPT_अन्तः()
	};

	/* Init merge options */
	init_basic_merge_options(&o.merge_options, the_repository);

	/* Parse arguments */
	original_argc = argc - 1; /* ignoring argv[0] */
	argc = parse_options(argc, argv, उपसर्गः, mt_options,
			     merge_वृक्षः_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	यदि (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !पङ्क्तिः_termination, "-z");

	यदि (xopts.nr && o.बहुलकम् == बहुलकम्_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	प्रति (आकार_t x = 0; x < xopts.nr; x++)
		यदि (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	यदि (o.use_stdin) {
		संरचना strbuf buf = STRBUF_INIT;

		यदि (o.बहुलकम् == बहुलकम्_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		यदि (merge_आधारः)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		पङ्क्तिः_termination = '\0';
		यावत् (strbuf_getline_lf(&buf, stdin) != EOF) {
			संरचना string_list विभज = STRING_LIST_INIT_NODUP;
			ध्रुव वर्ण *निवेशः_merge_आधारः = रिक्त;

			string_list_विभज_अन्तर्_place_f(&विभज, buf.buf, " ", -1,
						     STRING_LIST_विभज_TRIM);

			यदि (विभज.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			यदि (!वाक्यं_तुलय(विभज.वस्तूनि[1].string, "--")) {
				निवेशः_merge_आधारः = विभज.वस्तूनि[0].string;
			}

			यदि (निवेशः_merge_आधारः && विभज.nr == 4) {
				real_merge(&o, निवेशः_merge_आधारः,
					   विभज.वस्तूनि[2].string, विभज.वस्तूनि[3].string,
					   उपसर्गः);
			} अन्यथा यदि (!निवेशः_merge_आधारः && विभज.nr == 2) {
				real_merge(&o, रिक्त,
					   विभज.वस्तूनि[0].string, विभज.वस्तूनि[1].string,
					   उपसर्गः);
			} अन्यथा {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&विभज, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		गच्छ बहिर्गमनम्;
	}

	/* Figure out which mode to use */
	विकल्प (o.बहुलकम्) {
	स्वभाव:
		BUG("unexpected command mode %d", o.बहुलकम्);
	पक्ष बहुलकम्_UNKNOWN:
		विकल्प (argc) {
		स्वभाव:
			usage_with_options(merge_वृक्षः_usage, mt_options);
		पक्ष 2:
			o.बहुलकम् = बहुलकम्_REAL;
			भङ्ग;
		पक्ष 3:
			o.बहुलकम् = बहुलकम्_TRIVIAL;
			भङ्ग;
		}
		expected_remaining_argc = argc;
		भङ्ग;
	पक्ष बहुलकम्_REAL:
		expected_remaining_argc = 2;
		भङ्ग;
	पक्ष बहुलकम्_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		भङ्ग;
	}
	यदि (o.बहुलकम् == बहुलकम्_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	यदि (argc != expected_remaining_argc)
		usage_with_options(merge_वृक्षः_usage, mt_options);

	repo_config(the_repository, git_default_config, रिक्त);

	/* Do the relevant type of merge */
	यदि (o.बहुलकम् == बहुलकम्_REAL)
		ret = real_merge(&o, merge_आधारः, argv[0], argv[1], उपसर्गः);
	अन्यथा
		ret = trivial_merge(argv[0], argv[1], argv[2]);

बहिर्गमनम्:
	strvec_clear(&xopts);
	प्रत्यागम ret;
}
