#تعریف_کن USE_THE_REPOSITORY_VARIABLE

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

ساكن صحيح سطر_termination = '\n';

بنية ادغام_list {
	بنية ادغام_list *بعدی;
	بنية ادغام_list *link;	/* other stages for this object */

	بیعلامت صحيح stage : 2;
	بیعلامت صحيح نما;
	ثابت حرف *مسیر;
	بنية blob *blob;
};

ساكن بنية ادغام_list *ادغام_نتیجه, **ادغام_نتیجه_انتها = &ادغام_نتیجه;

ساكن فراغ جمع_کن_ادغام_مدخل(بنية ادغام_list *مدخل)
{
	*ادغام_نتیجه_انتها = مدخل;
	ادغام_نتیجه_انتها = &مدخل->بعدی;
}

ساكن فراغ trivial_ادغام_trees(بنية درخت_desc t[3], ثابت حرف *پایه);

ساكن ثابت حرف *explanation(بنية ادغام_list *مدخل)
{
	بدل (مدخل->stage) {
	حالة 0:
		أرجع "merged";
	حالة 3:
		أرجع "added in remote";
	حالة 2:
		إذا (مدخل->link)
			أرجع "added in both";
		أرجع "added in local";
	}

	/* Existed in base */
	مدخل = مدخل->link;
	إذا (!مدخل)
		أرجع "removed in both";

	إذا (مدخل->link)
		أرجع "changed in both";

	إذا (مدخل->stage == 3)
		أرجع "removed in local";
	أرجع "removed in remote";
}

ساكن فراغ *نتیجه(بنية ادغام_list *مدخل, اندازه_t *اندازه)
{
	تعداد object_type type;
	بنية blob *پایه, *our, *their;
	ثابت حرف *مسیر = مدخل->مسیر;

	إذا (!مدخل->stage)
		أرجع odb_read_object(the_repository->objects,
				       &مدخل->blob->object.oid, &type,
				       اندازه);
	پایه = عدم;
	إذا (مدخل->stage == 1) {
		پایه = مدخل->blob;
		مدخل = مدخل->link;
	}
	our = عدم;
	إذا (مدخل && مدخل->stage == 2) {
		our = مدخل->blob;
		مدخل = مدخل->link;
	}
	their = عدم;
	إذا (مدخل)
		their = مدخل->blob;
	أرجع ادغام_blobs(the_repository->شاخص, مسیر,
			   پایه, our, their, اندازه);
}

ساكن فراغ *origin(بنية ادغام_list *مدخل, اندازه_t *اندازه)
{
	تعداد object_type type;
	طالما (مدخل) {
		إذا (مدخل->stage == 2)
			أرجع odb_read_object(the_repository->objects,
					       &مدخل->blob->object.oid,
					       &type, اندازه);
		مدخل = مدخل->link;
	}
	أرجع عدم;
}

ساكن صحيح show_outf(فراغ *priv استفادهنشده, mmfile_t *mb, صحيح nbuf)
{
	صحيح i;
	لكل (i = 0; i < nbuf; i++)
		چاپ_کن("%.*s", (صحيح) mb[i].اندازه, mb[i].ptr);
	أرجع 0;
}

ساكن فراغ show_diff(بنية ادغام_list *مدخل)
{
	اندازه_t اندازه;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .بیرون_سطر = show_outf };

	memset(&xpp, 0, حجم(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, حجم(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(مدخل, &اندازه);
	إذا (!src.ptr)
		اندازه = 0;
	src.اندازه = اندازه;
	dst.ptr = نتیجه(مدخل, &اندازه);
	إذا (!dst.ptr)
		اندازه = 0;
	dst.اندازه = اندازه;
	إذا (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	حرر(src.ptr);
	حرر(dst.ptr);
}

ساكن فراغ show_نتیجه_list(بنية ادغام_list *مدخل)
{
	چاپ_کن("%s\n", explanation(مدخل));
	افعل {
		بنية ادغام_list *link = مدخل->link;
		ساكن ثابت حرف *desc[4] = { "result", "base", "our", "their" };
		چاپ_کن("  %-6s %o %s %s\n", desc[مدخل->stage], مدخل->نما, oid_مقصد_hex(&مدخل->blob->object.oid), مدخل->مسیر);
		مدخل = link;
	} طالما (مدخل);
}

ساكن فراغ show_نتیجه(فراغ)
{
	بنية ادغام_list *walk;

	walk = ادغام_نتیجه;
	طالما (walk) {
		show_نتیجه_list(walk);
		show_diff(walk);
		walk = walk->بعدی;
	}
}

/* An empty entry never compares same, not even to another empty entry */
ساكن صحيح same_مدخل(بنية name_مدخل *a, بنية name_مدخل *b)
{
	أرجع	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->نما == b->نما;
}

ساكن صحيح both_خالی(بنية name_مدخل *a, بنية name_مدخل *b)
{
	أرجع is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

ساكن بنية ادغام_list *create_مدخل(بیعلامت stage, بیعلامت نما, ثابت بنية object_id *oid, ثابت حرف *مسیر)
{
	بنية ادغام_list *res = xcalloc(1, حجم(*res));

	res->stage = stage;
	res->مسیر = مسیر;
	res->نما = نما;
	res->blob = lookup_blob(the_repository, oid);
	أرجع res;
}

ساكن حرف *traverse_مسیر(ثابت بنية traverse_info *info, ثابت بنية name_مدخل *n)
{
	بنية strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_مسیر(&buf, info, n->مسیر, n->pathlen);
	أرجع strbuf_detach(&buf, عدم);
}

ساكن فراغ resolve(ثابت بنية traverse_info *info, بنية name_مدخل *ours, بنية name_مدخل *نتیجه)
{
	بنية ادغام_list *orig, *final;
	ثابت حرف *مسیر;

	/* If it's already ours, don't bother showing it */
	إذا (!ours)
		أرجع;

	مسیر = traverse_مسیر(info, نتیجه);
	orig = create_مدخل(2, ours->نما, &ours->oid, مسیر);
	final = create_مدخل(0, نتیجه->نما, &نتیجه->oid, مسیر);

	final->link = orig;

	جمع_کن_ادغام_مدخل(final);
}

ساكن فراغ unresolved_directory(ثابت بنية traverse_info *info,
				 بنية name_مدخل n[3])
{
	بنية repository *r = the_repository;
	حرف *newbase;
	بنية name_مدخل *p;
	بنية درخت_desc t[3];
	فراغ *buf0, *buf1, *buf2;

	لكل (p = n; p < n + 3; p++) {
		إذا (p->نما && S_ISDIR(p->نما))
			اكسر;
	}
	إذا (n + 3 <= p)
		أرجع; /* there is no tree here */

	newbase = traverse_مسیر(info, p);

#تعریف_کن مدخل_OID(e) (((e)->نما && S_ISDIR((e)->نما)) ? &(e)->oid : عدم)
	buf0 = fill_درخت_descriptor(r, t + 0, مدخل_OID(n + 0));
	buf1 = fill_درخت_descriptor(r, t + 1, مدخل_OID(n + 1));
	buf2 = fill_درخت_descriptor(r, t + 2, مدخل_OID(n + 2));
#لغو_تعریف مدخل_OID

	trivial_ادغام_trees(t, newbase);

	حرر(buf0);
	حرر(buf1);
	حرر(buf2);
	حرر(newbase);
}


ساكن بنية ادغام_list *link_مدخل(بیعلامت stage, ثابت بنية traverse_info *info, بنية name_مدخل *n, بنية ادغام_list *مدخل)
{
	ثابت حرف *مسیر;
	بنية ادغام_list *link;

	إذا (!n->نما)
		أرجع مدخل;
	إذا (مدخل)
		مسیر = مدخل->مسیر;
	وإلا
		مسیر = traverse_مسیر(info, n);
	link = create_مدخل(stage, n->نما, &n->oid, مسیر);
	link->link = مدخل;
	أرجع link;
}

ساكن فراغ unresolved(ثابت بنية traverse_info *info, بنية name_مدخل n[3])
{
	بنية ادغام_list *مدخل = عدم;
	صحيح i;
	بیعلامت dirmask = 0, ماسک = 0;

	لكل (i = 0; i < 3; i++) {
		ماسک |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		إذا (!n[i].نما || S_ISDIR(n[i].نما))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	إذا (dirmask == ماسک)
		أرجع;

	إذا (n[2].نما && !S_ISDIR(n[2].نما))
		مدخل = link_مدخل(3, info, n + 2, مدخل);
	إذا (n[1].نما && !S_ISDIR(n[1].نما))
		مدخل = link_مدخل(2, info, n + 1, مدخل);
	إذا (n[0].نما && !S_ISDIR(n[0].نما))
		مدخل = link_مدخل(1, info, n + 0, مدخل);

	جمع_کن_ادغام_مدخل(مدخل);
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
ساكن صحيح threeway_callback(صحيح n استفادهنشده, بیعلامت دراز ماسک,
			     بیعلامت دراز dirmask استفادهنشده,
			     بنية name_مدخل *مدخل, بنية traverse_info *info)
{
	/* Same in both? */
	إذا (same_مدخل(مدخل+1, مدخل+2) || both_خالی(مدخل+1, مدخل+2)) {
		/* Modified, added or removed identically */
		resolve(info, عدم, مدخل+1);
		أرجع ماسک;
	}

	إذا (same_مدخل(مدخل+0, مدخل+1)) {
		إذا (!is_null_oid(&مدخل[2].oid) && !S_ISDIR(مدخل[2].نما)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, مدخل+1, مدخل+2);
			أرجع ماسک;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	إذا (same_مدخل(مدخل+0, مدخل+2) || both_خالی(مدخل+0, مدخل+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, عدم, مدخل+1);
		أرجع ماسک;
	}

	unresolved(info, مدخل);
	أرجع ماسک;
}

ساكن فراغ trivial_ادغام_trees(بنية درخت_desc t[3], ثابت حرف *پایه)
{
	بنية traverse_info info;

	راهاندازی_traverse_info(&info, پایه);
	info.fn = threeway_callback;
	traverse_trees(the_repository->شاخص, 3, t, &info);
}

ساكن فراغ *get_درخت_descriptor(بنية repository *r,
				 بنية درخت_desc *desc,
				 ثابت حرف *rev)
{
	بنية object_id oid;
	فراغ *buf;

	إذا (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_درخت_descriptor(r, desc, &oid);
	إذا (!buf)
		die("%s is not a tree", rev);
	أرجع buf;
}

ساكن صحيح trivial_ادغام(ثابت حرف *پایه,
			 ثابت حرف *branch1,
			 ثابت حرف *branch2)
{
	بنية repository *r = the_repository;
	بنية درخت_desc t[3];
	فراغ *buf1, *buf2, *buf3;

	buf1 = get_درخت_descriptor(r, t+0, پایه);
	buf2 = get_درخت_descriptor(r, t+1, branch1);
	buf3 = get_درخت_descriptor(r, t+2, branch2);
	trivial_ادغام_trees(t, "");
	حرر(buf1);
	حرر(buf2);
	حرر(buf3);

	show_نتیجه();
	أرجع 0;
}

تعداد نما {
	نما_UNKNOWN,
	نما_TRIVIAL,
	نما_REAL,
};

بنية ادغام_درخت_options {
	صحيح نما;
	صحيح allow_unrelated_histories;
	صحيح show_messages;
	صحيح name_only;
	صحيح use_stdin;
	بنية ادغام_options ادغام_options;
};

ساكن صحيح real_ادغام(بنية ادغام_درخت_options *o,
		      ثابت حرف *ادغام_پایه,
		      ثابت حرف *branch1, ثابت حرف *branch2,
		      ثابت حرف *پیشوند)
{
	بنية commit *parent1, *parent2;
	بنية commit_list *ادغام_bases = عدم;
	بنية ادغام_نتیجه نتیجه = { 0 };
	صحيح show_messages = o->show_messages;
	بنية ادغام_options opt;

	copy_ادغام_options(&opt, &o->ادغام_options);
	opt.show_rename_پیشرفت = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	إذا (ادغام_پایه) {
		بنية درخت *پایه_درخت, *parent1_درخت, *parent2_درخت;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		بنية object_id پایه_oid, سر_oid, ادغام_oid;

		إذا (repo_get_oid_treeish(the_repository, ادغام_پایه, &پایه_oid))
			die(_("could not parse as tree '%s'"), ادغام_پایه);
		پایه_درخت = repo_parse_درخت_غیرمستقیم(the_repository,
						     &پایه_oid);
		إذا (!پایه_درخت)
			die(_("unable to read tree (%s)"), oid_مقصد_hex(&پایه_oid));
		إذا (repo_get_oid_treeish(the_repository, branch1, &سر_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_درخت = repo_parse_درخت_غیرمستقیم(the_repository,
							&سر_oid);
		إذا (!parent1_درخت)
			die(_("unable to read tree (%s)"), oid_مقصد_hex(&سر_oid));
		إذا (repo_get_oid_treeish(the_repository, branch2, &ادغام_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_درخت = repo_parse_درخت_غیرمستقیم(the_repository,
							&ادغام_oid);
		إذا (!parent2_درخت)
			die(_("unable to read tree (%s)"), oid_مقصد_hex(&ادغام_oid));

		opt.ancestor = ادغام_پایه;
		ادغام_incore_nonrecursive(&opt, پایه_درخت, parent1_درخت, parent2_درخت, &نتیجه);
	} وإلا {
		parent1 = get_ادغام_والد(branch1);
		إذا (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_ادغام_والد(branch2);
		إذا (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		إذا (repo_get_ادغام_bases(the_repository, parent1,
					 parent2, &ادغام_bases) < 0)
			خروج(128);
		إذا (!ادغام_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		ادغام_bases = commit_list_معکوس(ادغام_bases);
		ادغام_incore_recursive(&opt, ادغام_bases, parent1, parent2, &نتیجه);
		commit_list_free(ادغام_bases);
	}

	إذا (نتیجه.clean < 0)
		die(_("failure to merge"));

	إذا (o->ادغام_options.mergeability_only)
		اذهب cleanup;

	إذا (show_messages == -1)
		show_messages = !نتیجه.clean;

	إذا (o->use_stdin)
		چاپ_کن("%d%c", نتیجه.clean, سطر_termination);
	چاپ_کن("%s%c", oid_مقصد_hex(&نتیجه.درخت->object.oid), سطر_termination);
	إذا (!نتیجه.clean) {
		بنية string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ثابت حرف *آخرین = عدم;

		ادغام_get_conflicted_files(&نتیجه, &conflicted_files);
		لكل (اندازه_t i = 0; i < conflicted_files.nr; i++) {
			ثابت حرف *name = conflicted_files.اقلام[i].string;
			بنية stage_info *c = conflicted_files.اقلام[i].util;
			إذا (!o->name_only)
				چاپ_کن("%06o %s %d\t",
				       c->نما, oid_مقصد_hex(&c->oid), c->stage);
			وإلا إذا (آخرین && !strcmp(آخرین, name))
				استمر;
			write_name_quoted_relative(
				name, پیشوند, stdout, سطر_termination);
			آخرین = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	إذا (show_messages) {
		حرف_چاپ_کن(سطر_termination);
		ادغام_display_update_messages(&opt, سطر_termination == '\0',
					      &نتیجه);
	}
	إذا (o->use_stdin)
		حرف_چاپ_کن(سطر_termination);

cleanup:
	ادغام_finalize(&opt, &نتیجه);
	clear_ادغام_options(&opt);
	أرجع !نتیجه.clean; /* result.clean < 0 handled above */
}

صحيح cmd_ادغام_درخت(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *پیشوند,
		   بنية repository *repo استفادهنشده)
{
	بنية ادغام_درخت_options o = { .show_messages = -1 };
	بنية strvec xopts = STRVEC_INIT;
	صحيح expected_مانده_argc;
	صحيح original_argc;
	ثابت حرف *ادغام_پایه = عدم;
	صحيح ret;
	صحيح quiet = 0;

	ثابت حرف * ثابت ادغام_درخت_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		عدم
	};
	بنية option mt_options[] = {
		OPT_CMDMODE(0, "write-tree", &o.نما,
			    N_("do a real merge instead of a trivial merge"),
			    نما_REAL),
		OPT_CMDMODE(0, "trivial-merge", &o.نما,
			    N_("do a trivial merge only"), نما_TRIVIAL),
		OPT_BOOL(0, "messages", &o.show_messages,
			 N_("also show informational/conflict messages")),
		OPT_BOOL_F(0, "quiet",
			   &quiet,
			   N_("suppress all output; only exit status wanted"),
			   PARSE_OPT_NONEG),
		OPT_مجموعه_INT('z', عدم, &سطر_termination,
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
			   &ادغام_پایه,
			   N_("tree-ish"),
			   N_("specify a merge-base for the merge")),
		OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
			N_("option for selected merge strategy")),
		OPT_انتها()
	};

	/* Init merge options */
	init_basic_ادغام_options(&o.ادغام_options, the_repository);

	/* Parse arguments */
	original_argc = argc - 1; /* ignoring argv[0] */
	argc = parse_options(argc, argv, پیشوند, mt_options,
			     ادغام_درخت_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.ادغام_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !سطر_termination, "-z");

	إذا (xopts.nr && o.نما == نما_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	لكل (اندازه_t x = 0; x < xopts.nr; x++)
		إذا (parse_ادغام_opt(&o.ادغام_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	إذا (o.use_stdin) {
		بنية strbuf buf = STRBUF_INIT;

		إذا (o.نما == نما_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		إذا (ادغام_پایه)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		سطر_termination = '\0';
		طالما (strbuf_getline_lf(&buf, stdin) != EOF) {
			بنية string_list تقسیم = STRING_LIST_INIT_NODUP;
			ثابت حرف *درون_داد_ادغام_پایه = عدم;

			string_list_تقسیم_درون_قراربده_f(&تقسیم, buf.buf, " ", -1,
						     STRING_LIST_تقسیم_TRIM);

			إذا (تقسیم.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			إذا (!strcmp(تقسیم.اقلام[1].string, "--")) {
				درون_داد_ادغام_پایه = تقسیم.اقلام[0].string;
			}

			إذا (درون_داد_ادغام_پایه && تقسیم.nr == 4) {
				real_ادغام(&o, درون_داد_ادغام_پایه,
					   تقسیم.اقلام[2].string, تقسیم.اقلام[3].string,
					   پیشوند);
			} وإلا إذا (!درون_داد_ادغام_پایه && تقسیم.nr == 2) {
				real_ادغام(&o, عدم,
					   تقسیم.اقلام[0].string, تقسیم.اقلام[1].string,
					   پیشوند);
			} وإلا {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&تقسیم, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		اذهب بیرون;
	}

	/* Figure out which mode to use */
	بدل (o.نما) {
	افتراضي:
		BUG("unexpected command mode %d", o.نما);
	حالة نما_UNKNOWN:
		بدل (argc) {
		افتراضي:
			usage_with_options(ادغام_درخت_usage, mt_options);
		حالة 2:
			o.نما = نما_REAL;
			اكسر;
		حالة 3:
			o.نما = نما_TRIVIAL;
			اكسر;
		}
		expected_مانده_argc = argc;
		اكسر;
	حالة نما_REAL:
		expected_مانده_argc = 2;
		اكسر;
	حالة نما_TRIVIAL:
		expected_مانده_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		اكسر;
	}
	إذا (o.نما == نما_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	إذا (argc != expected_مانده_argc)
		usage_with_options(ادغام_درخت_usage, mt_options);

	repo_config(the_repository, git_default_config, عدم);

	/* Do the relevant type of merge */
	إذا (o.نما == نما_REAL)
		ret = real_ادغام(&o, ادغام_پایه, argv[0], argv[1], پیشوند);
	وإلا
		ret = trivial_ادغام(argv[0], argv[1], argv[2]);

بیرون:
	strvec_clear(&xopts);
	أرجع ret;
}
