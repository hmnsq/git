#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

بنية ملاؤ_list {
	بنية ملاؤ_list *اگلا;
	بنية ملاؤ_list *link;	/* other stages for this object */

	بنانشانی صحيح stage : 2;
	بنانشانی صحيح منوال;
	ثابت حرف *رستہ;
	بنية blob *blob;
};

ساكن بنية ملاؤ_list *ملاؤ_نتیجہ, **ملاؤ_نتیجہ_سرا = &ملاؤ_نتیجہ;

ساكن فراغ جوڑو_ملاؤ_اندراج(بنية ملاؤ_list *اندراج)
{
	*ملاؤ_نتیجہ_سرا = اندراج;
	ملاؤ_نتیجہ_سرا = &اندراج->اگلا;
}

ساكن فراغ trivial_ملاؤ_trees(بنية درخت_desc t[3], ثابت حرف *ادھار);

ساكن ثابت حرف *explanation(بنية ملاؤ_list *اندراج)
{
	بدل (اندراج->stage) {
	حالة 0:
		أرجع "merged";
	حالة 3:
		أرجع "added in remote";
	حالة 2:
		إذا (اندراج->link)
			أرجع "added in both";
		أرجع "added in local";
	}

	/* Existed in base */
	اندراج = اندراج->link;
	إذا (!اندراج)
		أرجع "removed in both";

	إذا (اندراج->link)
		أرجع "changed in both";

	إذا (اندراج->stage == 3)
		أرجع "removed in local";
	أرجع "removed in remote";
}

ساكن فراغ *نتیجہ(بنية ملاؤ_list *اندراج, ناپ_t *ناپ)
{
	تعداد object_type type;
	بنية blob *ادھار, *our, *their;
	ثابت حرف *رستہ = اندراج->رستہ;

	إذا (!اندراج->stage)
		أرجع odb_read_object(the_repository->objects,
				       &اندراج->blob->object.oid, &type,
				       ناپ);
	ادھار = عدم;
	إذا (اندراج->stage == 1) {
		ادھار = اندراج->blob;
		اندراج = اندراج->link;
	}
	our = عدم;
	إذا (اندراج && اندراج->stage == 2) {
		our = اندراج->blob;
		اندراج = اندراج->link;
	}
	their = عدم;
	إذا (اندراج)
		their = اندراج->blob;
	أرجع ملاؤ_blobs(the_repository->اشاریہ, رستہ,
			   ادھار, our, their, ناپ);
}

ساكن فراغ *origin(بنية ملاؤ_list *اندراج, ناپ_t *ناپ)
{
	تعداد object_type type;
	طالما (اندراج) {
		إذا (اندراج->stage == 2)
			أرجع odb_read_object(the_repository->objects,
					       &اندراج->blob->object.oid,
					       &type, ناپ);
		اندراج = اندراج->link;
	}
	أرجع عدم;
}

ساكن صحيح show_outf(فراغ *priv بن_ورتیا, mmfile_t *mb, صحيح nbuf)
{
	صحيح i;
	لكل (i = 0; i < nbuf; i++)
		لکھو("%.*s", (صحيح) mb[i].ناپ, mb[i].ptr);
	أرجع 0;
}

ساكن فراغ show_diff(بنية ملاؤ_list *اندراج)
{
	ناپ_t ناپ;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .باہرلا_سطر = show_outf };

	memset(&xpp, 0, حجم(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, حجم(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(اندراج, &ناپ);
	إذا (!src.ptr)
		ناپ = 0;
	src.ناپ = ناپ;
	dst.ptr = نتیجہ(اندراج, &ناپ);
	إذا (!dst.ptr)
		ناپ = 0;
	dst.ناپ = ناپ;
	إذا (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	حرر(src.ptr);
	حرر(dst.ptr);
}

ساكن فراغ show_نتیجہ_list(بنية ملاؤ_list *اندراج)
{
	لکھو("%s\n", explanation(اندراج));
	افعل {
		بنية ملاؤ_list *link = اندراج->link;
		ساكن ثابت حرف *desc[4] = { "result", "base", "our", "their" };
		لکھو("  %-6s %o %s %s\n", desc[اندراج->stage], اندراج->منوال, oid_منزل_hex(&اندراج->blob->object.oid), اندراج->رستہ);
		اندراج = link;
	} طالما (اندراج);
}

ساكن فراغ show_نتیجہ(فراغ)
{
	بنية ملاؤ_list *walk;

	walk = ملاؤ_نتیجہ;
	طالما (walk) {
		show_نتیجہ_list(walk);
		show_diff(walk);
		walk = walk->اگلا;
	}
}

/* An empty entry never compares same, not even to another empty entry */
ساكن صحيح same_اندراج(بنية name_اندراج *a, بنية name_اندراج *b)
{
	أرجع	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->منوال == b->منوال;
}

ساكن صحيح both_خالی_تھاواں(بنية name_اندراج *a, بنية name_اندراج *b)
{
	أرجع is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

ساكن بنية ملاؤ_list *create_اندراج(بنانشانی stage, بنانشانی منوال, ثابت بنية object_id *oid, ثابت حرف *رستہ)
{
	بنية ملاؤ_list *res = xcalloc(1, حجم(*res));

	res->stage = stage;
	res->رستہ = رستہ;
	res->منوال = منوال;
	res->blob = lookup_blob(the_repository, oid);
	أرجع res;
}

ساكن حرف *traverse_رستہ(ثابت بنية traverse_info *info, ثابت بنية name_اندراج *n)
{
	بنية strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_رستہ(&buf, info, n->رستہ, n->pathlen);
	أرجع strbuf_detach(&buf, عدم);
}

ساكن فراغ resolve(ثابت بنية traverse_info *info, بنية name_اندراج *ours, بنية name_اندراج *نتیجہ)
{
	بنية ملاؤ_list *orig, *final;
	ثابت حرف *رستہ;

	/* If it's already ours, don't bother showing it */
	إذا (!ours)
		أرجع;

	رستہ = traverse_رستہ(info, نتیجہ);
	orig = create_اندراج(2, ours->منوال, &ours->oid, رستہ);
	final = create_اندراج(0, نتیجہ->منوال, &نتیجہ->oid, رستہ);

	final->link = orig;

	جوڑو_ملاؤ_اندراج(final);
}

ساكن فراغ unresolved_directory(ثابت بنية traverse_info *info,
				 بنية name_اندراج n[3])
{
	بنية repository *r = the_repository;
	حرف *newbase;
	بنية name_اندراج *p;
	بنية درخت_desc t[3];
	فراغ *buf0, *buf1, *buf2;

	لكل (p = n; p < n + 3; p++) {
		إذا (p->منوال && S_ISDIR(p->منوال))
			اكسر;
	}
	إذا (n + 3 <= p)
		أرجع; /* there is no tree here */

	newbase = traverse_رستہ(info, p);

#تعریف_کرو اندراج_OID(e) (((e)->منوال && S_ISDIR((e)->منوال)) ? &(e)->oid : عدم)
	buf0 = fill_درخت_descriptor(r, t + 0, اندراج_OID(n + 0));
	buf1 = fill_درخت_descriptor(r, t + 1, اندراج_OID(n + 1));
	buf2 = fill_درخت_descriptor(r, t + 2, اندراج_OID(n + 2));
#تعریف_رد اندراج_OID

	trivial_ملاؤ_trees(t, newbase);

	حرر(buf0);
	حرر(buf1);
	حرر(buf2);
	حرر(newbase);
}


ساكن بنية ملاؤ_list *link_اندراج(بنانشانی stage, ثابت بنية traverse_info *info, بنية name_اندراج *n, بنية ملاؤ_list *اندراج)
{
	ثابت حرف *رستہ;
	بنية ملاؤ_list *link;

	إذا (!n->منوال)
		أرجع اندراج;
	إذا (اندراج)
		رستہ = اندراج->رستہ;
	وإلا
		رستہ = traverse_رستہ(info, n);
	link = create_اندراج(stage, n->منوال, &n->oid, رستہ);
	link->link = اندراج;
	أرجع link;
}

ساكن فراغ unresolved(ثابت بنية traverse_info *info, بنية name_اندراج n[3])
{
	بنية ملاؤ_list *اندراج = عدم;
	صحيح i;
	بنانشانی dirmask = 0, نقاب = 0;

	لكل (i = 0; i < 3; i++) {
		نقاب |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		إذا (!n[i].منوال || S_ISDIR(n[i].منوال))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	إذا (dirmask == نقاب)
		أرجع;

	إذا (n[2].منوال && !S_ISDIR(n[2].منوال))
		اندراج = link_اندراج(3, info, n + 2, اندراج);
	إذا (n[1].منوال && !S_ISDIR(n[1].منوال))
		اندراج = link_اندراج(2, info, n + 1, اندراج);
	إذا (n[0].منوال && !S_ISDIR(n[0].منوال))
		اندراج = link_اندراج(1, info, n + 0, اندراج);

	جوڑو_ملاؤ_اندراج(اندراج);
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
ساكن صحيح threeway_callback(صحيح n بن_ورتیا, بنانشانی لما نقاب,
			     بنانشانی لما dirmask بن_ورتیا,
			     بنية name_اندراج *اندراج, بنية traverse_info *info)
{
	/* Same in both? */
	إذا (same_اندراج(اندراج+1, اندراج+2) || both_خالی_تھاواں(اندراج+1, اندراج+2)) {
		/* Modified, added or removed identically */
		resolve(info, عدم, اندراج+1);
		أرجع نقاب;
	}

	إذا (same_اندراج(اندراج+0, اندراج+1)) {
		إذا (!is_null_oid(&اندراج[2].oid) && !S_ISDIR(اندراج[2].منوال)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, اندراج+1, اندراج+2);
			أرجع نقاب;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	إذا (same_اندراج(اندراج+0, اندراج+2) || both_خالی_تھاواں(اندراج+0, اندراج+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, عدم, اندراج+1);
		أرجع نقاب;
	}

	unresolved(info, اندراج);
	أرجع نقاب;
}

ساكن فراغ trivial_ملاؤ_trees(بنية درخت_desc t[3], ثابت حرف *ادھار)
{
	بنية traverse_info info;

	تیاری_traverse_info(&info, ادھار);
	info.fn = threeway_callback;
	traverse_trees(the_repository->اشاریہ, 3, t, &info);
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

ساكن صحيح trivial_ملاؤ(ثابت حرف *ادھار,
			 ثابت حرف *branch1,
			 ثابت حرف *branch2)
{
	بنية repository *r = the_repository;
	بنية درخت_desc t[3];
	فراغ *buf1, *buf2, *buf3;

	buf1 = get_درخت_descriptor(r, t+0, ادھار);
	buf2 = get_درخت_descriptor(r, t+1, branch1);
	buf3 = get_درخت_descriptor(r, t+2, branch2);
	trivial_ملاؤ_trees(t, "");
	حرر(buf1);
	حرر(buf2);
	حرر(buf3);

	show_نتیجہ();
	أرجع 0;
}

تعداد منوال {
	منوال_UNKNOWN,
	منوال_TRIVIAL,
	منوال_REAL,
};

بنية ملاؤ_درخت_options {
	صحيح منوال;
	صحيح allow_unrelated_histories;
	صحيح show_messages;
	صحيح name_only;
	صحيح use_stdin;
	بنية ملاؤ_options ملاؤ_options;
};

ساكن صحيح real_ملاؤ(بنية ملاؤ_درخت_options *o,
		      ثابت حرف *ملاؤ_ادھار,
		      ثابت حرف *branch1, ثابت حرف *branch2,
		      ثابت حرف *اگیتر)
{
	بنية commit *parent1, *parent2;
	بنية commit_list *ملاؤ_bases = عدم;
	بنية ملاؤ_نتیجہ نتیجہ = { 0 };
	صحيح show_messages = o->show_messages;
	بنية ملاؤ_options opt;

	copy_ملاؤ_options(&opt, &o->ملاؤ_options);
	opt.show_rename_ترقی = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	إذا (ملاؤ_ادھار) {
		بنية درخت *ادھار_درخت, *parent1_درخت, *parent2_درخت;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		بنية object_id ادھار_oid, سر_oid, ملاؤ_oid;

		إذا (repo_get_oid_treeish(the_repository, ملاؤ_ادھار, &ادھار_oid))
			die(_("could not parse as tree '%s'"), ملاؤ_ادھار);
		ادھار_درخت = repo_parse_درخت_اسدھا(the_repository,
						     &ادھار_oid);
		إذا (!ادھار_درخت)
			die(_("unable to read tree (%s)"), oid_منزل_hex(&ادھار_oid));
		إذا (repo_get_oid_treeish(the_repository, branch1, &سر_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_درخت = repo_parse_درخت_اسدھا(the_repository,
							&سر_oid);
		إذا (!parent1_درخت)
			die(_("unable to read tree (%s)"), oid_منزل_hex(&سر_oid));
		إذا (repo_get_oid_treeish(the_repository, branch2, &ملاؤ_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_درخت = repo_parse_درخت_اسدھا(the_repository,
							&ملاؤ_oid);
		إذا (!parent2_درخت)
			die(_("unable to read tree (%s)"), oid_منزل_hex(&ملاؤ_oid));

		opt.ancestor = ملاؤ_ادھار;
		ملاؤ_incore_nonrecursive(&opt, ادھار_درخت, parent1_درخت, parent2_درخت, &نتیجہ);
	} وإلا {
		parent1 = get_ملاؤ_ماپا(branch1);
		إذا (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_ملاؤ_ماپا(branch2);
		إذا (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		إذا (repo_get_ملاؤ_bases(the_repository, parent1,
					 parent2, &ملاؤ_bases) < 0)
			باہر_نکلو(128);
		إذا (!ملاؤ_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		ملاؤ_bases = commit_list_الٹ(ملاؤ_bases);
		ملاؤ_incore_recursive(&opt, ملاؤ_bases, parent1, parent2, &نتیجہ);
		commit_list_free(ملاؤ_bases);
	}

	إذا (نتیجہ.clean < 0)
		die(_("failure to merge"));

	إذا (o->ملاؤ_options.mergeability_only)
		اذهب cleanup;

	إذا (show_messages == -1)
		show_messages = !نتیجہ.clean;

	إذا (o->use_stdin)
		لکھو("%d%c", نتیجہ.clean, سطر_termination);
	لکھو("%s%c", oid_منزل_hex(&نتیجہ.درخت->object.oid), سطر_termination);
	إذا (!نتیجہ.clean) {
		بنية string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ثابت حرف *آخری = عدم;

		ملاؤ_get_conflicted_files(&نتیجہ, &conflicted_files);
		لكل (ناپ_t i = 0; i < conflicted_files.nr; i++) {
			ثابت حرف *name = conflicted_files.اشیاء[i].string;
			بنية stage_info *c = conflicted_files.اشیاء[i].util;
			إذا (!o->name_only)
				لکھو("%06o %s %d\t",
				       c->منوال, oid_منزل_hex(&c->oid), c->stage);
			وإلا إذا (آخری && !strcmp(آخری, name))
				استمر;
			write_name_quoted_relative(
				name, اگیتر, stdout, سطر_termination);
			آخری = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	إذا (show_messages) {
		حرف_لکھو(سطر_termination);
		ملاؤ_display_update_messages(&opt, سطر_termination == '\0',
					      &نتیجہ);
	}
	إذا (o->use_stdin)
		حرف_لکھو(سطر_termination);

cleanup:
	ملاؤ_finalize(&opt, &نتیجہ);
	clear_ملاؤ_options(&opt);
	أرجع !نتیجہ.clean; /* result.clean < 0 handled above */
}

صحيح cmd_ملاؤ_درخت(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *اگیتر,
		   بنية repository *repo بن_ورتیا)
{
	بنية ملاؤ_درخت_options o = { .show_messages = -1 };
	بنية strvec xopts = STRVEC_INIT;
	صحيح expected_بچیا_argc;
	صحيح original_argc;
	ثابت حرف *ملاؤ_ادھار = عدم;
	صحيح ret;
	صحيح quiet = 0;

	ثابت حرف * ثابت ملاؤ_درخت_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		عدم
	};
	بنية option mt_options[] = {
		OPT_CMDMODE(0, "write-tree", &o.منوال,
			    N_("do a real merge instead of a trivial merge"),
			    منوال_REAL),
		OPT_CMDMODE(0, "trivial-merge", &o.منوال,
			    N_("do a trivial merge only"), منوال_TRIVIAL),
		OPT_BOOL(0, "messages", &o.show_messages,
			 N_("also show informational/conflict messages")),
		OPT_BOOL_F(0, "quiet",
			   &quiet,
			   N_("suppress all output; only exit status wanted"),
			   PARSE_OPT_NONEG),
		OPT_سیٹ_INT('z', عدم, &سطر_termination,
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
			   &ملاؤ_ادھار,
			   N_("tree-ish"),
			   N_("specify a merge-base for the merge")),
		OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
			N_("option for selected merge strategy")),
		OPT_سرا()
	};

	/* Init merge options */
	init_basic_ملاؤ_options(&o.ملاؤ_options, the_repository);

	/* Parse arguments */
	original_argc = argc - 1; /* ignoring argv[0] */
	argc = parse_options(argc, argv, اگیتر, mt_options,
			     ملاؤ_درخت_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.ملاؤ_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !سطر_termination, "-z");

	إذا (xopts.nr && o.منوال == منوال_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	لكل (ناپ_t x = 0; x < xopts.nr; x++)
		إذا (parse_ملاؤ_opt(&o.ملاؤ_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	إذا (o.use_stdin) {
		بنية strbuf buf = STRBUF_INIT;

		إذا (o.منوال == منوال_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		إذا (ملاؤ_ادھار)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		سطر_termination = '\0';
		طالما (strbuf_getline_lf(&buf, stdin) != EOF) {
			بنية string_list ونڈو = STRING_LIST_INIT_NODUP;
			ثابت حرف *داخلہ_ملاؤ_ادھار = عدم;

			string_list_ونڈو_وچ_رکھو_f(&ونڈو, buf.buf, " ", -1,
						     STRING_LIST_ونڈو_TRIM);

			إذا (ونڈو.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			إذا (!strcmp(ونڈو.اشیاء[1].string, "--")) {
				داخلہ_ملاؤ_ادھار = ونڈو.اشیاء[0].string;
			}

			إذا (داخلہ_ملاؤ_ادھار && ونڈو.nr == 4) {
				real_ملاؤ(&o, داخلہ_ملاؤ_ادھار,
					   ونڈو.اشیاء[2].string, ونڈو.اشیاء[3].string,
					   اگیتر);
			} وإلا إذا (!داخلہ_ملاؤ_ادھار && ونڈو.nr == 2) {
				real_ملاؤ(&o, عدم,
					   ونڈو.اشیاء[0].string, ونڈو.اشیاء[1].string,
					   اگیتر);
			} وإلا {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&ونڈو, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		اذهب باہرلا;
	}

	/* Figure out which mode to use */
	بدل (o.منوال) {
	افتراضي:
		BUG("unexpected command mode %d", o.منوال);
	حالة منوال_UNKNOWN:
		بدل (argc) {
		افتراضي:
			usage_with_options(ملاؤ_درخت_usage, mt_options);
		حالة 2:
			o.منوال = منوال_REAL;
			اكسر;
		حالة 3:
			o.منوال = منوال_TRIVIAL;
			اكسر;
		}
		expected_بچیا_argc = argc;
		اكسر;
	حالة منوال_REAL:
		expected_بچیا_argc = 2;
		اكسر;
	حالة منوال_TRIVIAL:
		expected_بچیا_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		اكسر;
	}
	إذا (o.منوال == منوال_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	إذا (argc != expected_بچیا_argc)
		usage_with_options(ملاؤ_درخت_usage, mt_options);

	repo_config(the_repository, git_default_config, عدم);

	/* Do the relevant type of merge */
	إذا (o.منوال == منوال_REAL)
		ret = real_ملاؤ(&o, ملاؤ_ادھار, argv[0], argv[1], اگیتر);
	وإلا
		ret = trivial_ملاؤ(argv[0], argv[1], argv[2]);

باہرلا:
	strvec_clear(&xopts);
	أرجع ret;
}
