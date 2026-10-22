#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "environment.h"
#ضمن "tree-walk.h"
#ضمن "xdiff-interface.h"
#ضمن "help.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "commit.h"
#ضمن "commit-reach.h"
#ضمن "merge-ort.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "parse-options.h"
#ضمن "blob.h"
#ضمن "merge-blobs.h"
#ضمن "quote.h"
#ضمن "tree.h"
#ضمن "config.h"
#ضمن "strvec.h"
#ضمن "write-or-die.h"

ساكن صحيح کرښه_termination = '\n';

بنية یوځای_list {
	بنية یوځای_list *بل;
	بنية یوځای_list *link;	/* other stages for this object */

	بینښه صحيح stage : 2;
	بینښه صحيح ډېرپېښ;
	ثابت حرف *لار;
	بنية blob *blob;
};

ساكن بنية یوځای_list *یوځای_پایله, **یوځای_پایله_پای_ټکی = &یوځای_پایله;

ساكن فراغ جمع_کړه_یوځای_ثبت(بنية یوځای_list *ثبت)
{
	*یوځای_پایله_پای_ټکی = ثبت;
	یوځای_پایله_پای_ټکی = &ثبت->بل;
}

ساكن فراغ trivial_یوځای_trees(بنية ونه_desc t[3], ثابت حرف *بنسټ);

ساكن ثابت حرف *explanation(بنية یوځای_list *ثبت)
{
	بدل (ثبت->stage) {
	حالة 0:
		أرجع "merged";
	حالة 3:
		أرجع "added in remote";
	حالة 2:
		إذا (ثبت->link)
			أرجع "added in both";
		أرجع "added in local";
	}

	/* Existed in base */
	ثبت = ثبت->link;
	إذا (!ثبت)
		أرجع "removed in both";

	إذا (ثبت->link)
		أرجع "changed in both";

	إذا (ثبت->stage == 3)
		أرجع "removed in local";
	أرجع "removed in remote";
}

ساكن فراغ *پایله(بنية یوځای_list *ثبت, اندازه_t *اندازه)
{
	تعداد object_type type;
	بنية blob *بنسټ, *our, *their;
	ثابت حرف *لار = ثبت->لار;

	إذا (!ثبت->stage)
		أرجع odb_read_object(the_repository->objects,
				       &ثبت->blob->object.oid, &type,
				       اندازه);
	بنسټ = عدم;
	إذا (ثبت->stage == 1) {
		بنسټ = ثبت->blob;
		ثبت = ثبت->link;
	}
	our = عدم;
	إذا (ثبت && ثبت->stage == 2) {
		our = ثبت->blob;
		ثبت = ثبت->link;
	}
	their = عدم;
	إذا (ثبت)
		their = ثبت->blob;
	أرجع یوځای_blobs(the_repository->شاخص, لار,
			   بنسټ, our, their, اندازه);
}

ساكن فراغ *origin(بنية یوځای_list *ثبت, اندازه_t *اندازه)
{
	تعداد object_type type;
	طالما (ثبت) {
		إذا (ثبت->stage == 2)
			أرجع odb_read_object(the_repository->objects,
					       &ثبت->blob->object.oid,
					       &type, اندازه);
		ثبت = ثبت->link;
	}
	أرجع عدم;
}

ساكن صحيح show_outf(فراغ *priv ناکارول_شوی, mmfile_t *mb, صحيح nbuf)
{
	صحيح i;
	لكل (i = 0; i < nbuf; i++)
		چاپ_کړه("%.*s", (صحيح) mb[i].اندازه, mb[i].ptr);
	أرجع 0;
}

ساكن فراغ show_diff(بنية یوځای_list *ثبت)
{
	اندازه_t اندازه;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .بهر_کرښه = show_outf };

	memset(&xpp, 0, حجم(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, حجم(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(ثبت, &اندازه);
	إذا (!src.ptr)
		اندازه = 0;
	src.اندازه = اندازه;
	dst.ptr = پایله(ثبت, &اندازه);
	إذا (!dst.ptr)
		اندازه = 0;
	dst.اندازه = اندازه;
	إذا (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	حرر(src.ptr);
	حرر(dst.ptr);
}

ساكن فراغ show_پایله_list(بنية یوځای_list *ثبت)
{
	چاپ_کړه("%s\n", explanation(ثبت));
	افعل {
		بنية یوځای_list *link = ثبت->link;
		ساكن ثابت حرف *desc[4] = { "result", "base", "our", "their" };
		چاپ_کړه("  %-6s %o %s %s\n", desc[ثبت->stage], ثبت->ډېرپېښ, oid_منزل_hex(&ثبت->blob->object.oid), ثبت->لار);
		ثبت = link;
	} طالما (ثبت);
}

ساكن فراغ show_پایله(فراغ)
{
	بنية یوځای_list *walk;

	walk = یوځای_پایله;
	طالما (walk) {
		show_پایله_list(walk);
		show_diff(walk);
		walk = walk->بل;
	}
}

/* An empty entry never compares same, not even to another empty entry */
ساكن صحيح same_ثبت(بنية name_ثبت *a, بنية name_ثبت *b)
{
	أرجع	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->ډېرپېښ == b->ډېرپېښ;
}

ساكن صحيح both_تشه(بنية name_ثبت *a, بنية name_ثبت *b)
{
	أرجع is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

ساكن بنية یوځای_list *create_ثبت(بینښه stage, بینښه ډېرپېښ, ثابت بنية object_id *oid, ثابت حرف *لار)
{
	بنية یوځای_list *res = xcalloc(1, حجم(*res));

	res->stage = stage;
	res->لار = لار;
	res->ډېرپېښ = ډېرپېښ;
	res->blob = lookup_blob(the_repository, oid);
	أرجع res;
}

ساكن حرف *traverse_لار(ثابت بنية traverse_info *info, ثابت بنية name_ثبت *n)
{
	بنية strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_لار(&buf, info, n->لار, n->pathlen);
	أرجع strbuf_detach(&buf, عدم);
}

ساكن فراغ resolve(ثابت بنية traverse_info *info, بنية name_ثبت *ours, بنية name_ثبت *پایله)
{
	بنية یوځای_list *orig, *final;
	ثابت حرف *لار;

	/* If it's already ours, don't bother showing it */
	إذا (!ours)
		أرجع;

	لار = traverse_لار(info, پایله);
	orig = create_ثبت(2, ours->ډېرپېښ, &ours->oid, لار);
	final = create_ثبت(0, پایله->ډېرپېښ, &پایله->oid, لار);

	final->link = orig;

	جمع_کړه_یوځای_ثبت(final);
}

ساكن فراغ unresolved_directory(ثابت بنية traverse_info *info,
				 بنية name_ثبت n[3])
{
	بنية repository *r = the_repository;
	حرف *newbase;
	بنية name_ثبت *p;
	بنية ونه_desc t[3];
	فراغ *buf0, *buf1, *buf2;

	لكل (p = n; p < n + 3; p++) {
		إذا (p->ډېرپېښ && S_ISDIR(p->ډېرپېښ))
			اكسر;
	}
	إذا (n + 3 <= p)
		أرجع; /* there is no tree here */

	newbase = traverse_لار(info, p);

#تعریف_کړه ثبت_OID(e) (((e)->ډېرپېښ && S_ISDIR((e)->ډېرپېښ)) ? &(e)->oid : عدم)
	buf0 = fill_ونه_descriptor(r, t + 0, ثبت_OID(n + 0));
	buf1 = fill_ونه_descriptor(r, t + 1, ثبت_OID(n + 1));
	buf2 = fill_ونه_descriptor(r, t + 2, ثبت_OID(n + 2));
#تعریف_لغوه ثبت_OID

	trivial_یوځای_trees(t, newbase);

	حرر(buf0);
	حرر(buf1);
	حرر(buf2);
	حرر(newbase);
}


ساكن بنية یوځای_list *link_ثبت(بینښه stage, ثابت بنية traverse_info *info, بنية name_ثبت *n, بنية یوځای_list *ثبت)
{
	ثابت حرف *لار;
	بنية یوځای_list *link;

	إذا (!n->ډېرپېښ)
		أرجع ثبت;
	إذا (ثبت)
		لار = ثبت->لار;
	وإلا
		لار = traverse_لار(info, n);
	link = create_ثبت(stage, n->ډېرپېښ, &n->oid, لار);
	link->link = ثبت;
	أرجع link;
}

ساكن فراغ unresolved(ثابت بنية traverse_info *info, بنية name_ثبت n[3])
{
	بنية یوځای_list *ثبت = عدم;
	صحيح i;
	بینښه dirmask = 0, ماسک = 0;

	لكل (i = 0; i < 3; i++) {
		ماسک |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		إذا (!n[i].ډېرپېښ || S_ISDIR(n[i].ډېرپېښ))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	إذا (dirmask == ماسک)
		أرجع;

	إذا (n[2].ډېرپېښ && !S_ISDIR(n[2].ډېرپېښ))
		ثبت = link_ثبت(3, info, n + 2, ثبت);
	إذا (n[1].ډېرپېښ && !S_ISDIR(n[1].ډېرپېښ))
		ثبت = link_ثبت(2, info, n + 1, ثبت);
	إذا (n[0].ډېرپېښ && !S_ISDIR(n[0].ډېرپېښ))
		ثبت = link_ثبت(1, info, n + 0, ثبت);

	جمع_کړه_یوځای_ثبت(ثبت);
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
ساكن صحيح threeway_callback(صحيح n ناکارول_شوی, بینښه اوږد ماسک,
			     بینښه اوږد dirmask ناکارول_شوی,
			     بنية name_ثبت *ثبت, بنية traverse_info *info)
{
	/* Same in both? */
	إذا (same_ثبت(ثبت+1, ثبت+2) || both_تشه(ثبت+1, ثبت+2)) {
		/* Modified, added or removed identically */
		resolve(info, عدم, ثبت+1);
		أرجع ماسک;
	}

	إذا (same_ثبت(ثبت+0, ثبت+1)) {
		إذا (!is_null_oid(&ثبت[2].oid) && !S_ISDIR(ثبت[2].ډېرپېښ)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, ثبت+1, ثبت+2);
			أرجع ماسک;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	إذا (same_ثبت(ثبت+0, ثبت+2) || both_تشه(ثبت+0, ثبت+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, عدم, ثبت+1);
		أرجع ماسک;
	}

	unresolved(info, ثبت);
	أرجع ماسک;
}

ساكن فراغ trivial_یوځای_trees(بنية ونه_desc t[3], ثابت حرف *بنسټ)
{
	بنية traverse_info info;

	تیاری_traverse_info(&info, بنسټ);
	info.fn = threeway_callback;
	traverse_trees(the_repository->شاخص, 3, t, &info);
}

ساكن فراغ *get_ونه_descriptor(بنية repository *r,
				 بنية ونه_desc *desc,
				 ثابت حرف *rev)
{
	بنية object_id oid;
	فراغ *buf;

	إذا (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_ونه_descriptor(r, desc, &oid);
	إذا (!buf)
		die("%s is not a tree", rev);
	أرجع buf;
}

ساكن صحيح trivial_یوځای(ثابت حرف *بنسټ,
			 ثابت حرف *branch1,
			 ثابت حرف *branch2)
{
	بنية repository *r = the_repository;
	بنية ونه_desc t[3];
	فراغ *buf1, *buf2, *buf3;

	buf1 = get_ونه_descriptor(r, t+0, بنسټ);
	buf2 = get_ونه_descriptor(r, t+1, branch1);
	buf3 = get_ونه_descriptor(r, t+2, branch2);
	trivial_یوځای_trees(t, "");
	حرر(buf1);
	حرر(buf2);
	حرر(buf3);

	show_پایله();
	أرجع 0;
}

تعداد ډېرپېښ {
	ډېرپېښ_UNKNOWN,
	ډېرپېښ_TRIVIAL,
	ډېرپېښ_REAL,
};

بنية یوځای_ونه_options {
	صحيح ډېرپېښ;
	صحيح allow_unrelated_histories;
	صحيح show_messages;
	صحيح name_only;
	صحيح use_stdin;
	بنية یوځای_options یوځای_options;
};

ساكن صحيح real_یوځای(بنية یوځای_ونه_options *o,
		      ثابت حرف *یوځای_بنسټ,
		      ثابت حرف *branch1, ثابت حرف *branch2,
		      ثابت حرف *مختاړی)
{
	بنية commit *parent1, *parent2;
	بنية commit_list *یوځای_bases = عدم;
	بنية یوځای_پایله پایله = { 0 };
	صحيح show_messages = o->show_messages;
	بنية یوځای_options opt;

	copy_یوځای_options(&opt, &o->یوځای_options);
	opt.show_rename_پرمختګ = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	إذا (یوځای_بنسټ) {
		بنية ونه *بنسټ_ونه, *parent1_ونه, *parent2_ونه;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		بنية object_id بنسټ_oid, سر_oid, یوځای_oid;

		إذا (repo_get_oid_treeish(the_repository, یوځای_بنسټ, &بنسټ_oid))
			die(_("could not parse as tree '%s'"), یوځای_بنسټ);
		بنسټ_ونه = repo_parse_ونه_غیرمستقیم(the_repository,
						     &بنسټ_oid);
		إذا (!بنسټ_ونه)
			die(_("unable to read tree (%s)"), oid_منزل_hex(&بنسټ_oid));
		إذا (repo_get_oid_treeish(the_repository, branch1, &سر_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_ونه = repo_parse_ونه_غیرمستقیم(the_repository,
							&سر_oid);
		إذا (!parent1_ونه)
			die(_("unable to read tree (%s)"), oid_منزل_hex(&سر_oid));
		إذا (repo_get_oid_treeish(the_repository, branch2, &یوځای_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_ونه = repo_parse_ونه_غیرمستقیم(the_repository,
							&یوځای_oid);
		إذا (!parent2_ونه)
			die(_("unable to read tree (%s)"), oid_منزل_hex(&یوځای_oid));

		opt.ancestor = یوځای_بنسټ;
		یوځای_incore_nonrecursive(&opt, بنسټ_ونه, parent1_ونه, parent2_ونه, &پایله);
	} وإلا {
		parent1 = get_یوځای_پلار(branch1);
		إذا (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_یوځای_پلار(branch2);
		إذا (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		إذا (repo_get_یوځای_bases(the_repository, parent1,
					 parent2, &یوځای_bases) < 0)
			وتل(128);
		إذا (!یوځای_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		یوځای_bases = commit_list_سرچپه(یوځای_bases);
		یوځای_incore_recursive(&opt, یوځای_bases, parent1, parent2, &پایله);
		commit_list_free(یوځای_bases);
	}

	إذا (پایله.clean < 0)
		die(_("failure to merge"));

	إذا (o->یوځای_options.mergeability_only)
		اذهب cleanup;

	إذا (show_messages == -1)
		show_messages = !پایله.clean;

	إذا (o->use_stdin)
		چاپ_کړه("%d%c", پایله.clean, کرښه_termination);
	چاپ_کړه("%s%c", oid_منزل_hex(&پایله.ونه->object.oid), کرښه_termination);
	إذا (!پایله.clean) {
		بنية string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ثابت حرف *وروستی = عدم;

		یوځای_get_conflicted_files(&پایله, &conflicted_files);
		لكل (اندازه_t i = 0; i < conflicted_files.nr; i++) {
			ثابت حرف *name = conflicted_files.توکي[i].string;
			بنية stage_info *c = conflicted_files.توکي[i].util;
			إذا (!o->name_only)
				چاپ_کړه("%06o %s %d\t",
				       c->ډېرپېښ, oid_منزل_hex(&c->oid), c->stage);
			وإلا إذا (وروستی && !strcmp(وروستی, name))
				استمر;
			write_name_quoted_relative(
				name, مختاړی, stdout, کرښه_termination);
			وروستی = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	إذا (show_messages) {
		توری_چاپ_کړه(کرښه_termination);
		یوځای_display_update_messages(&opt, کرښه_termination == '\0',
					      &پایله);
	}
	إذا (o->use_stdin)
		توری_چاپ_کړه(کرښه_termination);

cleanup:
	یوځای_finalize(&opt, &پایله);
	clear_یوځای_options(&opt);
	أرجع !پایله.clean; /* result.clean < 0 handled above */
}

صحيح cmd_یوځای_ونه(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *مختاړی,
		   بنية repository *repo ناکارول_شوی)
{
	بنية یوځای_ونه_options o = { .show_messages = -1 };
	بنية strvec xopts = STRVEC_INIT;
	صحيح expected_پاتې_شوی_argc;
	صحيح original_argc;
	ثابت حرف *یوځای_بنسټ = عدم;
	صحيح ret;
	صحيح quiet = 0;

	ثابت حرف * ثابت یوځای_ونه_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		عدم
	};
	بنية option mt_options[] = {
		OPT_CMDMODE(0, "write-tree", &o.ډېرپېښ,
			    N_("do a real merge instead of a trivial merge"),
			    ډېرپېښ_REAL),
		OPT_CMDMODE(0, "trivial-merge", &o.ډېرپېښ,
			    N_("do a trivial merge only"), ډېرپېښ_TRIVIAL),
		OPT_BOOL(0, "messages", &o.show_messages,
			 N_("also show informational/conflict messages")),
		OPT_BOOL_F(0, "quiet",
			   &quiet,
			   N_("suppress all output; only exit status wanted"),
			   PARSE_OPT_NONEG),
		OPT_سېټ_INT('z', عدم, &کرښه_termination,
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
			   &یوځای_بنسټ,
			   N_("tree-ish"),
			   N_("specify a merge-base for the merge")),
		OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
			N_("option for selected merge strategy")),
		OPT_پای_ټکی()
	};

	/* Init merge options */
	init_basic_یوځای_options(&o.یوځای_options, the_repository);

	/* Parse arguments */
	original_argc = argc - 1; /* ignoring argv[0] */
	argc = parse_options(argc, argv, مختاړی, mt_options,
			     یوځای_ونه_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.یوځای_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !کرښه_termination, "-z");

	إذا (xopts.nr && o.ډېرپېښ == ډېرپېښ_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	لكل (اندازه_t x = 0; x < xopts.nr; x++)
		إذا (parse_یوځای_opt(&o.یوځای_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	إذا (o.use_stdin) {
		بنية strbuf buf = STRBUF_INIT;

		إذا (o.ډېرپېښ == ډېرپېښ_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		إذا (یوځای_بنسټ)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		کرښه_termination = '\0';
		طالما (strbuf_getline_lf(&buf, stdin) != EOF) {
			بنية string_list وېشل = STRING_LIST_INIT_NODUP;
			ثابت حرف *ننوت_یوځای_بنسټ = عدم;

			string_list_وېشل_دننه_ځای_پرځای_کړه_f(&وېشل, buf.buf, " ", -1,
						     STRING_LIST_وېشل_TRIM);

			إذا (وېشل.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			إذا (!strcmp(وېشل.توکي[1].string, "--")) {
				ننوت_یوځای_بنسټ = وېشل.توکي[0].string;
			}

			إذا (ننوت_یوځای_بنسټ && وېشل.nr == 4) {
				real_یوځای(&o, ننوت_یوځای_بنسټ,
					   وېشل.توکي[2].string, وېشل.توکي[3].string,
					   مختاړی);
			} وإلا إذا (!ننوت_یوځای_بنسټ && وېشل.nr == 2) {
				real_یوځای(&o, عدم,
					   وېشل.توکي[0].string, وېشل.توکي[1].string,
					   مختاړی);
			} وإلا {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&وېشل, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		اذهب بهر;
	}

	/* Figure out which mode to use */
	بدل (o.ډېرپېښ) {
	افتراضي:
		BUG("unexpected command mode %d", o.ډېرپېښ);
	حالة ډېرپېښ_UNKNOWN:
		بدل (argc) {
		افتراضي:
			usage_with_options(یوځای_ونه_usage, mt_options);
		حالة 2:
			o.ډېرپېښ = ډېرپېښ_REAL;
			اكسر;
		حالة 3:
			o.ډېرپېښ = ډېرپېښ_TRIVIAL;
			اكسر;
		}
		expected_پاتې_شوی_argc = argc;
		اكسر;
	حالة ډېرپېښ_REAL:
		expected_پاتې_شوی_argc = 2;
		اكسر;
	حالة ډېرپېښ_TRIVIAL:
		expected_پاتې_شوی_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		اكسر;
	}
	إذا (o.ډېرپېښ == ډېرپېښ_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	إذا (argc != expected_پاتې_شوی_argc)
		usage_with_options(یوځای_ونه_usage, mt_options);

	repo_config(the_repository, git_default_config, عدم);

	/* Do the relevant type of merge */
	إذا (o.ډېرپېښ == ډېرپېښ_REAL)
		ret = real_یوځای(&o, یوځای_بنسټ, argv[0], argv[1], مختاړی);
	وإلا
		ret = trivial_یوځای(argv[0], argv[1], argv[2]);

بهر:
	strvec_clear(&xopts);
	أرجع ret;
}
