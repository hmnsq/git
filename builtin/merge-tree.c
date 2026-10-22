#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "tree-walk.h"
#అన్తర్భావ "xdiff-interface.h"
#అన్తర్భావ "help.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "merge-ort.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "blob.h"
#అన్తర్భావ "merge-blobs.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "write-or-die.h"

స్థిర పూర్ణాంకం పంక్తి_termination = '\n';

నిర్మాణం విలీనం_list {
	నిర్మాణం విలీనం_list *తదుపరి;
	నిర్మాణం విలీనం_list *link;	/* other stages for this object */

	గుర్తులేని పూర్ణాంకం stage : 2;
	గుర్తులేని పూర్ణాంకం బహుళకం;
	స్థిరరాశి అక్షరం *మార్గం;
	నిర్మాణం blob *blob;
};

స్థిర నిర్మాణం విలీనం_list *విలీనం_ఫలితం, **విలీనం_ఫలితం_చివర = &విలీనం_ఫలితం;

స్థిర శూన్య కూడు_విలీనం_నమోదు(నిర్మాణం విలీనం_list *నమోదు_2)
{
	*విలీనం_ఫలితం_చివర = నమోదు_2;
	విలీనం_ఫలితం_చివర = &నమోదు_2->తదుపరి;
}

స్థిర శూన్య trivial_విలీనం_trees(నిర్మాణం వృక్షం_desc t[3], స్థిరరాశి అక్షరం *పునాది);

స్థిర స్థిరరాశి అక్షరం *explanation(నిర్మాణం విలీనం_list *నమోదు_2)
{
	ఎంపిక (నమోదు_2->stage) {
	స్థితి 0:
		తిరిగి "merged";
	స్థితి 3:
		తిరిగి "added in remote";
	స్థితి 2:
		ఒకవేళ (నమోదు_2->link)
			తిరిగి "added in both";
		తిరిగి "added in local";
	}

	/* Existed in base */
	నమోదు_2 = నమోదు_2->link;
	ఒకవేళ (!నమోదు_2)
		తిరిగి "removed in both";

	ఒకవేళ (నమోదు_2->link)
		తిరిగి "changed in both";

	ఒకవేళ (నమోదు_2->stage == 3)
		తిరిగి "removed in local";
	తిరిగి "removed in remote";
}

స్థిర శూన్య *ఫలితం(నిర్మాణం విలీనం_list *నమోదు_2, కొలత_t *కొలత)
{
	గణన object_type type;
	నిర్మాణం blob *పునాది, *our, *their;
	స్థిరరాశి అక్షరం *మార్గం = నమోదు_2->మార్గం;

	ఒకవేళ (!నమోదు_2->stage)
		తిరిగి odb_read_object(the_repository->objects,
				       &నమోదు_2->blob->object.oid, &type,
				       కొలత);
	పునాది = ఖాళీ;
	ఒకవేళ (నమోదు_2->stage == 1) {
		పునాది = నమోదు_2->blob;
		నమోదు_2 = నమోదు_2->link;
	}
	our = ఖాళీ;
	ఒకవేళ (నమోదు_2 && నమోదు_2->stage == 2) {
		our = నమోదు_2->blob;
		నమోదు_2 = నమోదు_2->link;
	}
	their = ఖాళీ;
	ఒకవేళ (నమోదు_2)
		their = నమోదు_2->blob;
	తిరిగి విలీనం_blobs(the_repository->సూచీ, మార్గం,
			   పునాది, our, their, కొలత);
}

స్థిర శూన్య *origin(నిర్మాణం విలీనం_list *నమోదు_2, కొలత_t *కొలత)
{
	గణన object_type type;
	అయితే (నమోదు_2) {
		ఒకవేళ (నమోదు_2->stage == 2)
			తిరిగి odb_read_object(the_repository->objects,
					       &నమోదు_2->blob->object.oid,
					       &type, కొలత);
		నమోదు_2 = నమోదు_2->link;
	}
	తిరిగి ఖాళీ;
}

స్థిర పూర్ణాంకం show_outf(శూన్య *priv వాడని, mmfile_t *mb, పూర్ణాంకం nbuf)
{
	పూర్ణాంకం i;
	కొరకు (i = 0; i < nbuf; i++)
		రాయి("%.*s", (పూర్ణాంకం) mb[i].కొలత, mb[i].ptr);
	తిరిగి 0;
}

స్థిర శూన్య show_diff(నిర్మాణం విలీనం_list *నమోదు_2)
{
	కొలత_t కొలత;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .బహిర్గతం_పంక్తి = show_outf };

	memset(&xpp, 0, పరిమాణం(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, పరిమాణం(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(నమోదు_2, &కొలత);
	ఒకవేళ (!src.ptr)
		కొలత = 0;
	src.కొలత = కొలత;
	dst.ptr = ఫలితం(నమోదు_2, &కొలత);
	ఒకవేళ (!dst.ptr)
		కొలత = 0;
	dst.కొలత = కొలత;
	ఒకవేళ (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	విడుదల(src.ptr);
	విడుదల(dst.ptr);
}

స్థిర శూన్య show_ఫలితం_list(నిర్మాణం విలీనం_list *నమోదు_2)
{
	రాయి("%s\n", explanation(నమోదు_2));
	చేయి {
		నిర్మాణం విలీనం_list *link = నమోదు_2->link;
		స్థిర స్థిరరాశి అక్షరం *desc[4] = { "result", "base", "our", "their" };
		రాయి("  %-6s %o %s %s\n", desc[నమోదు_2->stage], నమోదు_2->బహుళకం, oid_గమ్యం_hex(&నమోదు_2->blob->object.oid), నమోదు_2->మార్గం);
		నమోదు_2 = link;
	} అయితే (నమోదు_2);
}

స్థిర శూన్య show_ఫలితం(శూన్య)
{
	నిర్మాణం విలీనం_list *walk;

	walk = విలీనం_ఫలితం;
	అయితే (walk) {
		show_ఫలితం_list(walk);
		show_diff(walk);
		walk = walk->తదుపరి;
	}
}

/* An empty entry never compares same, not even to another empty entry */
స్థిర పూర్ణాంకం same_నమోదు(నిర్మాణం name_నమోదు *a, నిర్మాణం name_నమోదు *b)
{
	తిరిగి	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->బహుళకం == b->బహుళకం;
}

స్థిర పూర్ణాంకం both_ఖాళీలు(నిర్మాణం name_నమోదు *a, నిర్మాణం name_నమోదు *b)
{
	తిరిగి is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

స్థిర నిర్మాణం విలీనం_list *create_నమోదు(గుర్తులేని stage, గుర్తులేని బహుళకం, స్థిరరాశి నిర్మాణం object_id *oid, స్థిరరాశి అక్షరం *మార్గం)
{
	నిర్మాణం విలీనం_list *res = xcalloc(1, పరిమాణం(*res));

	res->stage = stage;
	res->మార్గం = మార్గం;
	res->బహుళకం = బహుళకం;
	res->blob = lookup_blob(the_repository, oid);
	తిరిగి res;
}

స్థిర అక్షరం *traverse_మార్గం(స్థిరరాశి నిర్మాణం traverse_info *info, స్థిరరాశి నిర్మాణం name_నమోదు *n)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_మార్గం(&buf, info, n->మార్గం, n->pathlen);
	తిరిగి strbuf_detach(&buf, ఖాళీ);
}

స్థిర శూన్య resolve(స్థిరరాశి నిర్మాణం traverse_info *info, నిర్మాణం name_నమోదు *ours, నిర్మాణం name_నమోదు *ఫలితం)
{
	నిర్మాణం విలీనం_list *orig, *final;
	స్థిరరాశి అక్షరం *మార్గం;

	/* If it's already ours, don't bother showing it */
	ఒకవేళ (!ours)
		తిరిగి;

	మార్గం = traverse_మార్గం(info, ఫలితం);
	orig = create_నమోదు(2, ours->బహుళకం, &ours->oid, మార్గం);
	final = create_నమోదు(0, ఫలితం->బహుళకం, &ఫలితం->oid, మార్గం);

	final->link = orig;

	కూడు_విలీనం_నమోదు(final);
}

స్థిర శూన్య unresolved_directory(స్థిరరాశి నిర్మాణం traverse_info *info,
				 నిర్మాణం name_నమోదు n[3])
{
	నిర్మాణం repository *r = the_repository;
	అక్షరం *newbase;
	నిర్మాణం name_నమోదు *p;
	నిర్మాణం వృక్షం_desc t[3];
	శూన్య *buf0, *buf1, *buf2;

	కొరకు (p = n; p < n + 3; p++) {
		ఒకవేళ (p->బహుళకం && S_ISDIR(p->బహుళకం))
			ఆపు;
	}
	ఒకవేళ (n + 3 <= p)
		తిరిగి; /* there is no tree here */

	newbase = traverse_మార్గం(info, p);

#నిర్వచించు నమోదు_OID(e) (((e)->బహుళకం && S_ISDIR((e)->బహుళకం)) ? &(e)->oid : ఖాళీ)
	buf0 = fill_వృక్షం_descriptor(r, t + 0, నమోదు_OID(n + 0));
	buf1 = fill_వృక్షం_descriptor(r, t + 1, నమోదు_OID(n + 1));
	buf2 = fill_వృక్షం_descriptor(r, t + 2, నమోదు_OID(n + 2));
#నిర్వచనం_రద్దు నమోదు_OID

	trivial_విలీనం_trees(t, newbase);

	విడుదల(buf0);
	విడుదల(buf1);
	విడుదల(buf2);
	విడుదల(newbase);
}


స్థిర నిర్మాణం విలీనం_list *link_నమోదు(గుర్తులేని stage, స్థిరరాశి నిర్మాణం traverse_info *info, నిర్మాణం name_నమోదు *n, నిర్మాణం విలీనం_list *నమోదు_2)
{
	స్థిరరాశి అక్షరం *మార్గం;
	నిర్మాణం విలీనం_list *link;

	ఒకవేళ (!n->బహుళకం)
		తిరిగి నమోదు_2;
	ఒకవేళ (నమోదు_2)
		మార్గం = నమోదు_2->మార్గం;
	లేకపోతే
		మార్గం = traverse_మార్గం(info, n);
	link = create_నమోదు(stage, n->బహుళకం, &n->oid, మార్గం);
	link->link = నమోదు_2;
	తిరిగి link;
}

స్థిర శూన్య unresolved(స్థిరరాశి నిర్మాణం traverse_info *info, నిర్మాణం name_నమోదు n[3])
{
	నిర్మాణం విలీనం_list *నమోదు_2 = ఖాళీ;
	పూర్ణాంకం i;
	గుర్తులేని dirmask = 0, ముసుగు = 0;

	కొరకు (i = 0; i < 3; i++) {
		ముసుగు |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		ఒకవేళ (!n[i].బహుళకం || S_ISDIR(n[i].బహుళకం))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	ఒకవేళ (dirmask == ముసుగు)
		తిరిగి;

	ఒకవేళ (n[2].బహుళకం && !S_ISDIR(n[2].బహుళకం))
		నమోదు_2 = link_నమోదు(3, info, n + 2, నమోదు_2);
	ఒకవేళ (n[1].బహుళకం && !S_ISDIR(n[1].బహుళకం))
		నమోదు_2 = link_నమోదు(2, info, n + 1, నమోదు_2);
	ఒకవేళ (n[0].బహుళకం && !S_ISDIR(n[0].బహుళకం))
		నమోదు_2 = link_నమోదు(1, info, n + 0, నమోదు_2);

	కూడు_విలీనం_నమోదు(నమోదు_2);
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
స్థిర పూర్ణాంకం threeway_callback(పూర్ణాంకం n వాడని, గుర్తులేని దీర్ఘ ముసుగు,
			     గుర్తులేని దీర్ఘ dirmask వాడని,
			     నిర్మాణం name_నమోదు *నమోదు_2, నిర్మాణం traverse_info *info)
{
	/* Same in both? */
	ఒకవేళ (same_నమోదు(నమోదు_2+1, నమోదు_2+2) || both_ఖాళీలు(నమోదు_2+1, నమోదు_2+2)) {
		/* Modified, added or removed identically */
		resolve(info, ఖాళీ, నమోదు_2+1);
		తిరిగి ముసుగు;
	}

	ఒకవేళ (same_నమోదు(నమోదు_2+0, నమోదు_2+1)) {
		ఒకవేళ (!is_null_oid(&నమోదు_2[2].oid) && !S_ISDIR(నమోదు_2[2].బహుళకం)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, నమోదు_2+1, నమోదు_2+2);
			తిరిగి ముసుగు;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	ఒకవేళ (same_నమోదు(నమోదు_2+0, నమోదు_2+2) || both_ఖాళీలు(నమోదు_2+0, నమోదు_2+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, ఖాళీ, నమోదు_2+1);
		తిరిగి ముసుగు;
	}

	unresolved(info, నమోదు_2);
	తిరిగి ముసుగు;
}

స్థిర శూన్య trivial_విలీనం_trees(నిర్మాణం వృక్షం_desc t[3], స్థిరరాశి అక్షరం *పునాది)
{
	నిర్మాణం traverse_info info;

	అమరిక_traverse_info(&info, పునాది);
	info.fn = threeway_callback;
	traverse_trees(the_repository->సూచీ, 3, t, &info);
}

స్థిర శూన్య *get_వృక్షం_descriptor(నిర్మాణం repository *r,
				 నిర్మాణం వృక్షం_desc *desc,
				 స్థిరరాశి అక్షరం *rev)
{
	నిర్మాణం object_id oid;
	శూన్య *buf;

	ఒకవేళ (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_వృక్షం_descriptor(r, desc, &oid);
	ఒకవేళ (!buf)
		die("%s is not a tree", rev);
	తిరిగి buf;
}

స్థిర పూర్ణాంకం trivial_విలీనం(స్థిరరాశి అక్షరం *పునాది,
			 స్థిరరాశి అక్షరం *branch1,
			 స్థిరరాశి అక్షరం *branch2)
{
	నిర్మాణం repository *r = the_repository;
	నిర్మాణం వృక్షం_desc t[3];
	శూన్య *buf1, *buf2, *buf3;

	buf1 = get_వృక్షం_descriptor(r, t+0, పునాది);
	buf2 = get_వృక్షం_descriptor(r, t+1, branch1);
	buf3 = get_వృక్షం_descriptor(r, t+2, branch2);
	trivial_విలీనం_trees(t, "");
	విడుదల(buf1);
	విడుదల(buf2);
	విడుదల(buf3);

	show_ఫలితం();
	తిరిగి 0;
}

గణన బహుళకం {
	బహుళకం_UNKNOWN,
	బహుళకం_TRIVIAL,
	బహుళకం_REAL,
};

నిర్మాణం విలీనం_వృక్షం_options {
	పూర్ణాంకం బహుళకం;
	పూర్ణాంకం allow_unrelated_histories;
	పూర్ణాంకం show_messages;
	పూర్ణాంకం name_only;
	పూర్ణాంకం use_stdin;
	నిర్మాణం విలీనం_options విలీనం_options;
};

స్థిర పూర్ణాంకం real_విలీనం(నిర్మాణం విలీనం_వృక్షం_options *o,
		      స్థిరరాశి అక్షరం *విలీనం_పునాది,
		      స్థిరరాశి అక్షరం *branch1, స్థిరరాశి అక్షరం *branch2,
		      స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	నిర్మాణం commit *parent1, *parent2;
	నిర్మాణం commit_list *విలీనం_bases = ఖాళీ;
	నిర్మాణం విలీనం_ఫలితం ఫలితం = { 0 };
	పూర్ణాంకం show_messages = o->show_messages;
	నిర్మాణం విలీనం_options opt;

	copy_విలీనం_options(&opt, &o->విలీనం_options);
	opt.show_rename_పురోగతి = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	ఒకవేళ (విలీనం_పునాది) {
		నిర్మాణం వృక్షం *పునాది_వృక్షం, *parent1_వృక్షం, *parent2_వృక్షం;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		నిర్మాణం object_id పునాది_oid, తల_oid, విలీనం_oid;

		ఒకవేళ (repo_get_oid_treeish(the_repository, విలీనం_పునాది, &పునాది_oid))
			die(_("could not parse as tree '%s'"), విలీనం_పునాది);
		పునాది_వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository,
						     &పునాది_oid);
		ఒకవేళ (!పునాది_వృక్షం)
			die(_("unable to read tree (%s)"), oid_గమ్యం_hex(&పునాది_oid));
		ఒకవేళ (repo_get_oid_treeish(the_repository, branch1, &తల_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository,
							&తల_oid);
		ఒకవేళ (!parent1_వృక్షం)
			die(_("unable to read tree (%s)"), oid_గమ్యం_hex(&తల_oid));
		ఒకవేళ (repo_get_oid_treeish(the_repository, branch2, &విలీనం_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository,
							&విలీనం_oid);
		ఒకవేళ (!parent2_వృక్షం)
			die(_("unable to read tree (%s)"), oid_గమ్యం_hex(&విలీనం_oid));

		opt.ancestor = విలీనం_పునాది;
		విలీనం_incore_nonrecursive(&opt, పునాది_వృక్షం, parent1_వృక్షం, parent2_వృక్షం, &ఫలితం);
	} లేకపోతే {
		parent1 = get_విలీనం_మాతృ(branch1);
		ఒకవేళ (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_విలీనం_మాతృ(branch2);
		ఒకవేళ (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		ఒకవేళ (repo_get_విలీనం_bases(the_repository, parent1,
					 parent2, &విలీనం_bases) < 0)
			నిష్క్రమించు(128);
		ఒకవేళ (!విలీనం_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		విలీనం_bases = commit_list_తిరగేయి(విలీనం_bases);
		విలీనం_incore_recursive(&opt, విలీనం_bases, parent1, parent2, &ఫలితం);
		commit_list_free(విలీనం_bases);
	}

	ఒకవేళ (ఫలితం.clean < 0)
		die(_("failure to merge"));

	ఒకవేళ (o->విలీనం_options.mergeability_only)
		వెళ్ళు cleanup;

	ఒకవేళ (show_messages == -1)
		show_messages = !ఫలితం.clean;

	ఒకవేళ (o->use_stdin)
		రాయి("%d%c", ఫలితం.clean, పంక్తి_termination);
	రాయి("%s%c", oid_గమ్యం_hex(&ఫలితం.వృక్షం->object.oid), పంక్తి_termination);
	ఒకవేళ (!ఫలితం.clean) {
		నిర్మాణం string_list conflicted_files = STRING_LIST_INIT_NODUP;
		స్థిరరాశి అక్షరం *చివరి = ఖాళీ;

		విలీనం_get_conflicted_files(&ఫలితం, &conflicted_files);
		కొరకు (కొలత_t i = 0; i < conflicted_files.nr; i++) {
			స్థిరరాశి అక్షరం *name = conflicted_files.వస్తువులు[i].string;
			నిర్మాణం stage_info *c = conflicted_files.వస్తువులు[i].util;
			ఒకవేళ (!o->name_only)
				రాయి("%06o %s %d\t",
				       c->బహుళకం, oid_గమ్యం_hex(&c->oid), c->stage);
			లేకపోతే ఒకవేళ (చివరి && !strcmp(చివరి, name))
				కొనసాగు;
			write_name_quoted_relative(
				name, ఉపసర్గ, stdout, పంక్తి_termination);
			చివరి = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	ఒకవేళ (show_messages) {
		అక్షరం_రాయి(పంక్తి_termination);
		విలీనం_display_update_messages(&opt, పంక్తి_termination == '\0',
					      &ఫలితం);
	}
	ఒకవేళ (o->use_stdin)
		అక్షరం_రాయి(పంక్తి_termination);

cleanup:
	విలీనం_finalize(&opt, &ఫలితం);
	clear_విలీనం_options(&opt);
	తిరిగి !ఫలితం.clean; /* result.clean < 0 handled above */
}

పూర్ణాంకం cmd_విలీనం_వృక్షం(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం విలీనం_వృక్షం_options o = { .show_messages = -1 };
	నిర్మాణం strvec xopts = STRVEC_INIT;
	పూర్ణాంకం expected_మిగిలిన_argc;
	పూర్ణాంకం original_argc;
	స్థిరరాశి అక్షరం *విలీనం_పునాది = ఖాళీ;
	పూర్ణాంకం ret;
	పూర్ణాంకం quiet = 0;

	స్థిరరాశి అక్షరం * స్థిరరాశి విలీనం_వృక్షం_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		ఖాళీ
	};
	నిర్మాణం option mt_options[] = {
		OPT_CMDMODE(0, "write-tree", &o.బహుళకం,
			    N_("do a real merge instead of a trivial merge"),
			    బహుళకం_REAL),
		OPT_CMDMODE(0, "trivial-merge", &o.బహుళకం,
			    N_("do a trivial merge only"), బహుళకం_TRIVIAL),
		OPT_BOOL(0, "messages", &o.show_messages,
			 N_("also show informational/conflict messages")),
		OPT_BOOL_F(0, "quiet",
			   &quiet,
			   N_("suppress all output; only exit status wanted"),
			   PARSE_OPT_NONEG),
		OPT_సమితి_INT('z', ఖాళీ, &పంక్తి_termination,
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
			   &విలీనం_పునాది,
			   N_("tree-ish"),
			   N_("specify a merge-base for the merge")),
		OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
			N_("option for selected merge strategy")),
		OPT_చివర()
	};

	/* Init merge options */
	init_basic_విలీనం_options(&o.విలీనం_options, the_repository);

	/* Parse arguments */
	original_argc = argc - 1; /* ignoring argv[0] */
	argc = parse_options(argc, argv, ఉపసర్గ, mt_options,
			     విలీనం_వృక్షం_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	ఒకవేళ (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.విలీనం_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !పంక్తి_termination, "-z");

	ఒకవేళ (xopts.nr && o.బహుళకం == బహుళకం_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	కొరకు (కొలత_t x = 0; x < xopts.nr; x++)
		ఒకవేళ (parse_విలీనం_opt(&o.విలీనం_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	ఒకవేళ (o.use_stdin) {
		నిర్మాణం strbuf buf = STRBUF_INIT;

		ఒకవేళ (o.బహుళకం == బహుళకం_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		ఒకవేళ (విలీనం_పునాది)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		పంక్తి_termination = '\0';
		అయితే (strbuf_getline_lf(&buf, stdin) != EOF) {
			నిర్మాణం string_list విభజించు = STRING_LIST_INIT_NODUP;
			స్థిరరాశి అక్షరం *ఆదానం_విలీనం_పునాది = ఖాళీ;

			string_list_విభజించు_లోపల_ఉంచు_f(&విభజించు, buf.buf, " ", -1,
						     STRING_LIST_విభజించు_TRIM);

			ఒకవేళ (విభజించు.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			ఒకవేళ (!strcmp(విభజించు.వస్తువులు[1].string, "--")) {
				ఆదానం_విలీనం_పునాది = విభజించు.వస్తువులు[0].string;
			}

			ఒకవేళ (ఆదానం_విలీనం_పునాది && విభజించు.nr == 4) {
				real_విలీనం(&o, ఆదానం_విలీనం_పునాది,
					   విభజించు.వస్తువులు[2].string, విభజించు.వస్తువులు[3].string,
					   ఉపసర్గ);
			} లేకపోతే ఒకవేళ (!ఆదానం_విలీనం_పునాది && విభజించు.nr == 2) {
				real_విలీనం(&o, ఖాళీ,
					   విభజించు.వస్తువులు[0].string, విభజించు.వస్తువులు[1].string,
					   ఉపసర్గ);
			} లేకపోతే {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&విభజించు, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		వెళ్ళు బహిర్గతం;
	}

	/* Figure out which mode to use */
	ఎంపిక (o.బహుళకం) {
	అప్రమేయ:
		BUG("unexpected command mode %d", o.బహుళకం);
	స్థితి బహుళకం_UNKNOWN:
		ఎంపిక (argc) {
		అప్రమేయ:
			usage_with_options(విలీనం_వృక్షం_usage, mt_options);
		స్థితి 2:
			o.బహుళకం = బహుళకం_REAL;
			ఆపు;
		స్థితి 3:
			o.బహుళకం = బహుళకం_TRIVIAL;
			ఆపు;
		}
		expected_మిగిలిన_argc = argc;
		ఆపు;
	స్థితి బహుళకం_REAL:
		expected_మిగిలిన_argc = 2;
		ఆపు;
	స్థితి బహుళకం_TRIVIAL:
		expected_మిగిలిన_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		ఆపు;
	}
	ఒకవేళ (o.బహుళకం == బహుళకం_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	ఒకవేళ (argc != expected_మిగిలిన_argc)
		usage_with_options(విలీనం_వృక్షం_usage, mt_options);

	repo_config(the_repository, git_default_config, ఖాళీ);

	/* Do the relevant type of merge */
	ఒకవేళ (o.బహుళకం == బహుళకం_REAL)
		ret = real_విలీనం(&o, విలీనం_పునాది, argv[0], argv[1], ఉపసర్గ);
	లేకపోతే
		ret = trivial_విలీనం(argv[0], argv[1], argv[2]);

బహిర్గతం:
	strvec_clear(&xopts);
	తిరిగి ret;
}
