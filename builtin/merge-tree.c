#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "environment.h"
#شامل "tree-walk.h"
#شامل "xdiff-interface.h"
#شامل "help.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "commit.h"
#شامل "commit-reach.h"
#شامل "merge-ort.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "parse-options.h"
#شامل "blob.h"
#شامل "merge-blobs.h"
#شامل "quote.h"
#شامل "tree.h"
#شامل "config.h"
#شامل "strvec.h"
#شامل "write-or-die.h"

ساکن صحیح سطر_termination = '\n';

ساخت ملاؤ_list {
	ساخت ملاؤ_list *اگلا;
	ساخت ملاؤ_list *link;	/* other stages for this object */

	غیرعلامتی صحیح stage : 2;
	غیرعلامتی صحیح منوال;
	ثابت حرف *راستہ;
	ساخت blob *blob;
};

ساکن ساخت ملاؤ_list *ملاؤ_نتیجہ, **ملاؤ_نتیجہ_انتہا = &ملاؤ_نتیجہ;

ساکن خالی جوڑو_ملاؤ_اندراج(ساخت ملاؤ_list *اندراج_2)
{
	*ملاؤ_نتیجہ_انتہا = اندراج_2;
	ملاؤ_نتیجہ_انتہا = &اندراج_2->اگلا;
}

ساکن خالی trivial_ملاؤ_trees(ساخت درخت_desc t[3], ثابت حرف *بنیاد);

ساکن ثابت حرف *explanation(ساخت ملاؤ_list *اندراج_2)
{
	انتخاب (اندراج_2->stage) {
	حالت 0:
		واپس "merged";
	حالت 3:
		واپس "added in remote";
	حالت 2:
		اگر (اندراج_2->link)
			واپس "added in both";
		واپس "added in local";
	}

	/* Existed in base */
	اندراج_2 = اندراج_2->link;
	اگر (!اندراج_2)
		واپس "removed in both";

	اگر (اندراج_2->link)
		واپس "changed in both";

	اگر (اندراج_2->stage == 3)
		واپس "removed in local";
	واپس "removed in remote";
}

ساکن خالی *نتیجہ(ساخت ملاؤ_list *اندراج_2, سائز_t *سائز)
{
	شمار object_type type;
	ساخت blob *بنیاد, *our, *their;
	ثابت حرف *راستہ = اندراج_2->راستہ;

	اگر (!اندراج_2->stage)
		واپس odb_read_object(the_repository->objects,
				       &اندراج_2->blob->object.oid, &type,
				       سائز);
	بنیاد = تہی;
	اگر (اندراج_2->stage == 1) {
		بنیاد = اندراج_2->blob;
		اندراج_2 = اندراج_2->link;
	}
	our = تہی;
	اگر (اندراج_2 && اندراج_2->stage == 2) {
		our = اندراج_2->blob;
		اندراج_2 = اندراج_2->link;
	}
	their = تہی;
	اگر (اندراج_2)
		their = اندراج_2->blob;
	واپس ملاؤ_blobs(the_repository->اشاریہ, راستہ,
			   بنیاد, our, their, سائز);
}

ساکن خالی *origin(ساخت ملاؤ_list *اندراج_2, سائز_t *سائز)
{
	شمار object_type type;
	جب (اندراج_2) {
		اگر (اندراج_2->stage == 2)
			واپس odb_read_object(the_repository->objects,
					       &اندراج_2->blob->object.oid,
					       &type, سائز);
		اندراج_2 = اندراج_2->link;
	}
	واپس تہی;
}

ساکن صحیح show_outf(خالی *priv غیر_مستعمل, mmfile_t *mb, صحیح nbuf)
{
	صحیح i;
	برائے (i = 0; i < nbuf; i++)
		لکھو("%.*s", (صحیح) mb[i].سائز, mb[i].ptr);
	واپس 0;
}

ساکن خالی show_diff(ساخت ملاؤ_list *اندراج_2)
{
	سائز_t سائز;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .برآمد_سطر = show_outf };

	memset(&xpp, 0, حجم(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, حجم(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(اندراج_2, &سائز);
	اگر (!src.ptr)
		سائز = 0;
	src.سائز = سائز;
	dst.ptr = نتیجہ(اندراج_2, &سائز);
	اگر (!dst.ptr)
		سائز = 0;
	dst.سائز = سائز;
	اگر (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	آزاد(src.ptr);
	آزاد(dst.ptr);
}

ساکن خالی show_نتیجہ_list(ساخت ملاؤ_list *اندراج_2)
{
	لکھو("%s\n", explanation(اندراج_2));
	کرو {
		ساخت ملاؤ_list *link = اندراج_2->link;
		ساکن ثابت حرف *desc[4] = { "result", "base", "our", "their" };
		لکھو("  %-6s %o %s %s\n", desc[اندراج_2->stage], اندراج_2->منوال, oid_مقصد_hex(&اندراج_2->blob->object.oid), اندراج_2->راستہ);
		اندراج_2 = link;
	} جب (اندراج_2);
}

ساکن خالی show_نتیجہ(خالی)
{
	ساخت ملاؤ_list *walk;

	walk = ملاؤ_نتیجہ;
	جب (walk) {
		show_نتیجہ_list(walk);
		show_diff(walk);
		walk = walk->اگلا;
	}
}

/* An empty entry never compares same, not even to another empty entry */
ساکن صحیح same_اندراج(ساخت name_اندراج *a, ساخت name_اندراج *b)
{
	واپس	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->منوال == b->منوال;
}

ساکن صحیح both_خالی_خانے(ساخت name_اندراج *a, ساخت name_اندراج *b)
{
	واپس is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

ساکن ساخت ملاؤ_list *create_اندراج(غیرعلامتی stage, غیرعلامتی منوال, ثابت ساخت object_id *oid, ثابت حرف *راستہ)
{
	ساخت ملاؤ_list *res = xcalloc(1, حجم(*res));

	res->stage = stage;
	res->راستہ = راستہ;
	res->منوال = منوال;
	res->blob = lookup_blob(the_repository, oid);
	واپس res;
}

ساکن حرف *traverse_راستہ(ثابت ساخت traverse_info *info, ثابت ساخت name_اندراج *n)
{
	ساخت strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_راستہ(&buf, info, n->راستہ, n->pathlen);
	واپس strbuf_detach(&buf, تہی);
}

ساکن خالی resolve(ثابت ساخت traverse_info *info, ساخت name_اندراج *ours, ساخت name_اندراج *نتیجہ)
{
	ساخت ملاؤ_list *orig, *final;
	ثابت حرف *راستہ;

	/* If it's already ours, don't bother showing it */
	اگر (!ours)
		واپس;

	راستہ = traverse_راستہ(info, نتیجہ);
	orig = create_اندراج(2, ours->منوال, &ours->oid, راستہ);
	final = create_اندراج(0, نتیجہ->منوال, &نتیجہ->oid, راستہ);

	final->link = orig;

	جوڑو_ملاؤ_اندراج(final);
}

ساکن خالی unresolved_directory(ثابت ساخت traverse_info *info,
				 ساخت name_اندراج n[3])
{
	ساخت repository *r = the_repository;
	حرف *newbase;
	ساخت name_اندراج *p;
	ساخت درخت_desc t[3];
	خالی *buf0, *buf1, *buf2;

	برائے (p = n; p < n + 3; p++) {
		اگر (p->منوال && S_ISDIR(p->منوال))
			توڑو;
	}
	اگر (n + 3 <= p)
		واپس; /* there is no tree here */

	newbase = traverse_راستہ(info, p);

#تعریف_کرو اندراج_OID(e) (((e)->منوال && S_ISDIR((e)->منوال)) ? &(e)->oid : تہی)
	buf0 = fill_درخت_descriptor(r, t + 0, اندراج_OID(n + 0));
	buf1 = fill_درخت_descriptor(r, t + 1, اندراج_OID(n + 1));
	buf2 = fill_درخت_descriptor(r, t + 2, اندراج_OID(n + 2));
#تعریف_منسوخ اندراج_OID

	trivial_ملاؤ_trees(t, newbase);

	آزاد(buf0);
	آزاد(buf1);
	آزاد(buf2);
	آزاد(newbase);
}


ساکن ساخت ملاؤ_list *link_اندراج(غیرعلامتی stage, ثابت ساخت traverse_info *info, ساخت name_اندراج *n, ساخت ملاؤ_list *اندراج_2)
{
	ثابت حرف *راستہ;
	ساخت ملاؤ_list *link;

	اگر (!n->منوال)
		واپس اندراج_2;
	اگر (اندراج_2)
		راستہ = اندراج_2->راستہ;
	ورنہ
		راستہ = traverse_راستہ(info, n);
	link = create_اندراج(stage, n->منوال, &n->oid, راستہ);
	link->link = اندراج_2;
	واپس link;
}

ساکن خالی unresolved(ثابت ساخت traverse_info *info, ساخت name_اندراج n[3])
{
	ساخت ملاؤ_list *اندراج_2 = تہی;
	صحیح i;
	غیرعلامتی dirmask = 0, نقاب = 0;

	برائے (i = 0; i < 3; i++) {
		نقاب |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		اگر (!n[i].منوال || S_ISDIR(n[i].منوال))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	اگر (dirmask == نقاب)
		واپس;

	اگر (n[2].منوال && !S_ISDIR(n[2].منوال))
		اندراج_2 = link_اندراج(3, info, n + 2, اندراج_2);
	اگر (n[1].منوال && !S_ISDIR(n[1].منوال))
		اندراج_2 = link_اندراج(2, info, n + 1, اندراج_2);
	اگر (n[0].منوال && !S_ISDIR(n[0].منوال))
		اندراج_2 = link_اندراج(1, info, n + 0, اندراج_2);

	جوڑو_ملاؤ_اندراج(اندراج_2);
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
ساکن صحیح threeway_callback(صحیح n غیر_مستعمل, غیرعلامتی طویل نقاب,
			     غیرعلامتی طویل dirmask غیر_مستعمل,
			     ساخت name_اندراج *اندراج_2, ساخت traverse_info *info)
{
	/* Same in both? */
	اگر (same_اندراج(اندراج_2+1, اندراج_2+2) || both_خالی_خانے(اندراج_2+1, اندراج_2+2)) {
		/* Modified, added or removed identically */
		resolve(info, تہی, اندراج_2+1);
		واپس نقاب;
	}

	اگر (same_اندراج(اندراج_2+0, اندراج_2+1)) {
		اگر (!is_null_oid(&اندراج_2[2].oid) && !S_ISDIR(اندراج_2[2].منوال)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, اندراج_2+1, اندراج_2+2);
			واپس نقاب;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	اگر (same_اندراج(اندراج_2+0, اندراج_2+2) || both_خالی_خانے(اندراج_2+0, اندراج_2+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, تہی, اندراج_2+1);
		واپس نقاب;
	}

	unresolved(info, اندراج_2);
	واپس نقاب;
}

ساکن خالی trivial_ملاؤ_trees(ساخت درخت_desc t[3], ثابت حرف *بنیاد)
{
	ساخت traverse_info info;

	تیاری_traverse_info(&info, بنیاد);
	info.fn = threeway_callback;
	traverse_trees(the_repository->اشاریہ, 3, t, &info);
}

ساکن خالی *get_درخت_descriptor(ساخت repository *r,
				 ساخت درخت_desc *desc,
				 ثابت حرف *rev)
{
	ساخت object_id oid;
	خالی *buf;

	اگر (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_درخت_descriptor(r, desc, &oid);
	اگر (!buf)
		die("%s is not a tree", rev);
	واپس buf;
}

ساکن صحیح trivial_ملاؤ(ثابت حرف *بنیاد,
			 ثابت حرف *branch1,
			 ثابت حرف *branch2)
{
	ساخت repository *r = the_repository;
	ساخت درخت_desc t[3];
	خالی *buf1, *buf2, *buf3;

	buf1 = get_درخت_descriptor(r, t+0, بنیاد);
	buf2 = get_درخت_descriptor(r, t+1, branch1);
	buf3 = get_درخت_descriptor(r, t+2, branch2);
	trivial_ملاؤ_trees(t, "");
	آزاد(buf1);
	آزاد(buf2);
	آزاد(buf3);

	show_نتیجہ();
	واپس 0;
}

شمار منوال {
	منوال_UNKNOWN,
	منوال_TRIVIAL,
	منوال_REAL,
};

ساخت ملاؤ_درخت_options {
	صحیح منوال;
	صحیح allow_unrelated_histories;
	صحیح show_messages;
	صحیح name_only;
	صحیح use_stdin;
	ساخت ملاؤ_options ملاؤ_options;
};

ساکن صحیح real_ملاؤ(ساخت ملاؤ_درخت_options *o,
		      ثابت حرف *ملاؤ_بنیاد,
		      ثابت حرف *branch1, ثابت حرف *branch2,
		      ثابت حرف *سابقہ)
{
	ساخت commit *parent1, *parent2;
	ساخت commit_list *ملاؤ_bases = تہی;
	ساخت ملاؤ_نتیجہ نتیجہ = { 0 };
	صحیح show_messages = o->show_messages;
	ساخت ملاؤ_options opt;

	copy_ملاؤ_options(&opt, &o->ملاؤ_options);
	opt.show_rename_پیش_رفت = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	اگر (ملاؤ_بنیاد) {
		ساخت درخت *بنیاد_درخت, *parent1_درخت, *parent2_درخت;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		ساخت object_id بنیاد_oid, سر_oid, ملاؤ_oid;

		اگر (repo_get_oid_treeish(the_repository, ملاؤ_بنیاد, &بنیاد_oid))
			die(_("could not parse as tree '%s'"), ملاؤ_بنیاد);
		بنیاد_درخت = repo_parse_درخت_بالواسطہ(the_repository,
						     &بنیاد_oid);
		اگر (!بنیاد_درخت)
			die(_("unable to read tree (%s)"), oid_مقصد_hex(&بنیاد_oid));
		اگر (repo_get_oid_treeish(the_repository, branch1, &سر_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_درخت = repo_parse_درخت_بالواسطہ(the_repository,
							&سر_oid);
		اگر (!parent1_درخت)
			die(_("unable to read tree (%s)"), oid_مقصد_hex(&سر_oid));
		اگر (repo_get_oid_treeish(the_repository, branch2, &ملاؤ_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_درخت = repo_parse_درخت_بالواسطہ(the_repository,
							&ملاؤ_oid);
		اگر (!parent2_درخت)
			die(_("unable to read tree (%s)"), oid_مقصد_hex(&ملاؤ_oid));

		opt.ancestor = ملاؤ_بنیاد;
		ملاؤ_incore_nonrecursive(&opt, بنیاد_درخت, parent1_درخت, parent2_درخت, &نتیجہ);
	} ورنہ {
		parent1 = get_ملاؤ_والد(branch1);
		اگر (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_ملاؤ_والد(branch2);
		اگر (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		اگر (repo_get_ملاؤ_bases(the_repository, parent1,
					 parent2, &ملاؤ_bases) < 0)
			باہر_نکلو(128);
		اگر (!ملاؤ_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		ملاؤ_bases = commit_list_الٹ(ملاؤ_bases);
		ملاؤ_incore_recursive(&opt, ملاؤ_bases, parent1, parent2, &نتیجہ);
		commit_list_free(ملاؤ_bases);
	}

	اگر (نتیجہ.clean < 0)
		die(_("failure to merge"));

	اگر (o->ملاؤ_options.mergeability_only)
		جاؤ cleanup;

	اگر (show_messages == -1)
		show_messages = !نتیجہ.clean;

	اگر (o->use_stdin)
		لکھو("%d%c", نتیجہ.clean, سطر_termination);
	لکھو("%s%c", oid_مقصد_hex(&نتیجہ.درخت->object.oid), سطر_termination);
	اگر (!نتیجہ.clean) {
		ساخت string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ثابت حرف *آخری = تہی;

		ملاؤ_get_conflicted_files(&نتیجہ, &conflicted_files);
		برائے (سائز_t i = 0; i < conflicted_files.nr; i++) {
			ثابت حرف *name = conflicted_files.اشیاء[i].string;
			ساخت stage_info *c = conflicted_files.اشیاء[i].util;
			اگر (!o->name_only)
				لکھو("%06o %s %d\t",
				       c->منوال, oid_مقصد_hex(&c->oid), c->stage);
			ورنہ اگر (آخری && !strcmp(آخری, name))
				جاری;
			write_name_quoted_relative(
				name, سابقہ, stdout, سطر_termination);
			آخری = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	اگر (show_messages) {
		حرف_لکھو(سطر_termination);
		ملاؤ_display_update_messages(&opt, سطر_termination == '\0',
					      &نتیجہ);
	}
	اگر (o->use_stdin)
		حرف_لکھو(سطر_termination);

cleanup:
	ملاؤ_finalize(&opt, &نتیجہ);
	clear_ملاؤ_options(&opt);
	واپس !نتیجہ.clean; /* result.clean < 0 handled above */
}

صحیح cmd_ملاؤ_درخت(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *سابقہ,
		   ساخت repository *repo غیر_مستعمل)
{
	ساخت ملاؤ_درخت_options o = { .show_messages = -1 };
	ساخت strvec xopts = STRVEC_INIT;
	صحیح expected_بقایا_argc;
	صحیح original_argc;
	ثابت حرف *ملاؤ_بنیاد = تہی;
	صحیح ret;
	صحیح quiet = 0;

	ثابت حرف * ثابت ملاؤ_درخت_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		تہی
	};
	ساخت option mt_options[] = {
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
		OPT_سیٹ_INT('z', تہی, &سطر_termination,
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
			   &ملاؤ_بنیاد,
			   N_("tree-ish"),
			   N_("specify a merge-base for the merge")),
		OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
			N_("option for selected merge strategy")),
		OPT_انتہا()
	};

	/* Init merge options */
	init_basic_ملاؤ_options(&o.ملاؤ_options, the_repository);

	/* Parse arguments */
	original_argc = argc - 1; /* ignoring argv[0] */
	argc = parse_options(argc, argv, سابقہ, mt_options,
			     ملاؤ_درخت_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	اگر (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.ملاؤ_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !سطر_termination, "-z");

	اگر (xopts.nr && o.منوال == منوال_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	برائے (سائز_t x = 0; x < xopts.nr; x++)
		اگر (parse_ملاؤ_opt(&o.ملاؤ_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	اگر (o.use_stdin) {
		ساخت strbuf buf = STRBUF_INIT;

		اگر (o.منوال == منوال_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		اگر (ملاؤ_بنیاد)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		سطر_termination = '\0';
		جب (strbuf_getline_lf(&buf, stdin) != EOF) {
			ساخت string_list بانٹو = STRING_LIST_INIT_NODUP;
			ثابت حرف *داخلہ_ملاؤ_بنیاد = تہی;

			string_list_بانٹو_اندر_رکھو_f(&بانٹو, buf.buf, " ", -1,
						     STRING_LIST_بانٹو_TRIM);

			اگر (بانٹو.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			اگر (!strcmp(بانٹو.اشیاء[1].string, "--")) {
				داخلہ_ملاؤ_بنیاد = بانٹو.اشیاء[0].string;
			}

			اگر (داخلہ_ملاؤ_بنیاد && بانٹو.nr == 4) {
				real_ملاؤ(&o, داخلہ_ملاؤ_بنیاد,
					   بانٹو.اشیاء[2].string, بانٹو.اشیاء[3].string,
					   سابقہ);
			} ورنہ اگر (!داخلہ_ملاؤ_بنیاد && بانٹو.nr == 2) {
				real_ملاؤ(&o, تہی,
					   بانٹو.اشیاء[0].string, بانٹو.اشیاء[1].string,
					   سابقہ);
			} ورنہ {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&بانٹو, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		جاؤ برآمد;
	}

	/* Figure out which mode to use */
	انتخاب (o.منوال) {
	طےشدہ:
		BUG("unexpected command mode %d", o.منوال);
	حالت منوال_UNKNOWN:
		انتخاب (argc) {
		طےشدہ:
			usage_with_options(ملاؤ_درخت_usage, mt_options);
		حالت 2:
			o.منوال = منوال_REAL;
			توڑو;
		حالت 3:
			o.منوال = منوال_TRIVIAL;
			توڑو;
		}
		expected_بقایا_argc = argc;
		توڑو;
	حالت منوال_REAL:
		expected_بقایا_argc = 2;
		توڑو;
	حالت منوال_TRIVIAL:
		expected_بقایا_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		توڑو;
	}
	اگر (o.منوال == منوال_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	اگر (argc != expected_بقایا_argc)
		usage_with_options(ملاؤ_درخت_usage, mt_options);

	repo_config(the_repository, git_default_config, تہی);

	/* Do the relevant type of merge */
	اگر (o.منوال == منوال_REAL)
		ret = real_ملاؤ(&o, ملاؤ_بنیاد, argv[0], argv[1], سابقہ);
	ورنہ
		ret = trivial_ملاؤ(argv[0], argv[1], argv[2]);

برآمد:
	strvec_clear(&xopts);
	واپس ret;
}
