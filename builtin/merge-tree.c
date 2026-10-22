#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE

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

ساكن صحيح سٽ_termination = '\n';

بنية ملايو_list {
	بنية ملايو_list *ايندڙ;
	بنية ملايو_list *link;	/* other stages for this object */

	بنانشان صحيح stage : 2;
	بنانشان صحيح گهڻائي;
	ثابت حرف *رستو;
	بنية blob *blob;
};

ساكن بنية ملايو_list *ملايو_نتيجو, **ملايو_نتيجو_پڇاڙي = &ملايو_نتيجو;

ساكن فراغ جوڙيو_ملايو_داخلا(بنية ملايو_list *داخلا)
{
	*ملايو_نتيجو_پڇاڙي = داخلا;
	ملايو_نتيجو_پڇاڙي = &داخلا->ايندڙ;
}

ساكن فراغ trivial_ملايو_trees(بنية وڻ_desc t[3], ثابت حرف *بنياد);

ساكن ثابت حرف *explanation(بنية ملايو_list *داخلا)
{
	بدل (داخلا->stage) {
	حالة 0:
		أرجع "merged";
	حالة 3:
		أرجع "added in remote";
	حالة 2:
		إذا (داخلا->link)
			أرجع "added in both";
		أرجع "added in local";
	}

	/* Existed in base */
	داخلا = داخلا->link;
	إذا (!داخلا)
		أرجع "removed in both";

	إذا (داخلا->link)
		أرجع "changed in both";

	إذا (داخلا->stage == 3)
		أرجع "removed in local";
	أرجع "removed in remote";
}

ساكن فراغ *نتيجو(بنية ملايو_list *داخلا, سائيز_t *سائيز)
{
	تعداد object_type type;
	بنية blob *بنياد, *our, *their;
	ثابت حرف *رستو = داخلا->رستو;

	إذا (!داخلا->stage)
		أرجع odb_read_object(the_repository->objects,
				       &داخلا->blob->object.oid, &type,
				       سائيز);
	بنياد = عدم;
	إذا (داخلا->stage == 1) {
		بنياد = داخلا->blob;
		داخلا = داخلا->link;
	}
	our = عدم;
	إذا (داخلا && داخلا->stage == 2) {
		our = داخلا->blob;
		داخلا = داخلا->link;
	}
	their = عدم;
	إذا (داخلا)
		their = داخلا->blob;
	أرجع ملايو_blobs(the_repository->اشاريو, رستو,
			   بنياد, our, their, سائيز);
}

ساكن فراغ *origin(بنية ملايو_list *داخلا, سائيز_t *سائيز)
{
	تعداد object_type type;
	طالما (داخلا) {
		إذا (داخلا->stage == 2)
			أرجع odb_read_object(the_repository->objects,
					       &داخلا->blob->object.oid,
					       &type, سائيز);
		داخلا = داخلا->link;
	}
	أرجع عدم;
}

ساكن صحيح show_outf(فراغ *priv اڻ_استعمال_ٿيل, mmfile_t *mb, صحيح nbuf)
{
	صحيح i;
	لكل (i = 0; i < nbuf; i++)
		لکو("%.*s", (صحيح) mb[i].سائيز, mb[i].ptr);
	أرجع 0;
}

ساكن فراغ show_diff(بنية ملايو_list *داخلا)
{
	سائيز_t سائيز;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .ٻاهريون_سٽ = show_outf };

	memset(&xpp, 0, حجم(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, حجم(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(داخلا, &سائيز);
	إذا (!src.ptr)
		سائيز = 0;
	src.سائيز = سائيز;
	dst.ptr = نتيجو(داخلا, &سائيز);
	إذا (!dst.ptr)
		سائيز = 0;
	dst.سائيز = سائيز;
	إذا (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	حرر(src.ptr);
	حرر(dst.ptr);
}

ساكن فراغ show_نتيجو_list(بنية ملايو_list *داخلا)
{
	لکو("%s\n", explanation(داخلا));
	افعل {
		بنية ملايو_list *link = داخلا->link;
		ساكن ثابت حرف *desc[4] = { "result", "base", "our", "their" };
		لکو("  %-6s %o %s %s\n", desc[داخلا->stage], داخلا->گهڻائي, oid_منزل_hex(&داخلا->blob->object.oid), داخلا->رستو);
		داخلا = link;
	} طالما (داخلا);
}

ساكن فراغ show_نتيجو(فراغ)
{
	بنية ملايو_list *walk;

	walk = ملايو_نتيجو;
	طالما (walk) {
		show_نتيجو_list(walk);
		show_diff(walk);
		walk = walk->ايندڙ;
	}
}

/* An empty entry never compares same, not even to another empty entry */
ساكن صحيح same_داخلا(بنية name_داخلا *a, بنية name_داخلا *b)
{
	أرجع	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->گهڻائي == b->گهڻائي;
}

ساكن صحيح both_خالي_جايون(بنية name_داخلا *a, بنية name_داخلا *b)
{
	أرجع is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

ساكن بنية ملايو_list *create_داخلا(بنانشان stage, بنانشان گهڻائي, ثابت بنية object_id *oid, ثابت حرف *رستو)
{
	بنية ملايو_list *res = xcalloc(1, حجم(*res));

	res->stage = stage;
	res->رستو = رستو;
	res->گهڻائي = گهڻائي;
	res->blob = lookup_blob(the_repository, oid);
	أرجع res;
}

ساكن حرف *traverse_رستو(ثابت بنية traverse_info *info, ثابت بنية name_داخلا *n)
{
	بنية strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_رستو(&buf, info, n->رستو, n->pathlen);
	أرجع strbuf_detach(&buf, عدم);
}

ساكن فراغ resolve(ثابت بنية traverse_info *info, بنية name_داخلا *ours, بنية name_داخلا *نتيجو)
{
	بنية ملايو_list *orig, *final;
	ثابت حرف *رستو;

	/* If it's already ours, don't bother showing it */
	إذا (!ours)
		أرجع;

	رستو = traverse_رستو(info, نتيجو);
	orig = create_داخلا(2, ours->گهڻائي, &ours->oid, رستو);
	final = create_داخلا(0, نتيجو->گهڻائي, &نتيجو->oid, رستو);

	final->link = orig;

	جوڙيو_ملايو_داخلا(final);
}

ساكن فراغ unresolved_directory(ثابت بنية traverse_info *info,
				 بنية name_داخلا n[3])
{
	بنية repository *r = the_repository;
	حرف *newbase;
	بنية name_داخلا *p;
	بنية وڻ_desc t[3];
	فراغ *buf0, *buf1, *buf2;

	لكل (p = n; p < n + 3; p++) {
		إذا (p->گهڻائي && S_ISDIR(p->گهڻائي))
			اكسر;
	}
	إذا (n + 3 <= p)
		أرجع; /* there is no tree here */

	newbase = traverse_رستو(info, p);

#وصف_ڪريو داخلا_OID(e) (((e)->گهڻائي && S_ISDIR((e)->گهڻائي)) ? &(e)->oid : عدم)
	buf0 = fill_وڻ_descriptor(r, t + 0, داخلا_OID(n + 0));
	buf1 = fill_وڻ_descriptor(r, t + 1, داخلا_OID(n + 1));
	buf2 = fill_وڻ_descriptor(r, t + 2, داخلا_OID(n + 2));
#وصف_رد داخلا_OID

	trivial_ملايو_trees(t, newbase);

	حرر(buf0);
	حرر(buf1);
	حرر(buf2);
	حرر(newbase);
}


ساكن بنية ملايو_list *link_داخلا(بنانشان stage, ثابت بنية traverse_info *info, بنية name_داخلا *n, بنية ملايو_list *داخلا)
{
	ثابت حرف *رستو;
	بنية ملايو_list *link;

	إذا (!n->گهڻائي)
		أرجع داخلا;
	إذا (داخلا)
		رستو = داخلا->رستو;
	وإلا
		رستو = traverse_رستو(info, n);
	link = create_داخلا(stage, n->گهڻائي, &n->oid, رستو);
	link->link = داخلا;
	أرجع link;
}

ساكن فراغ unresolved(ثابت بنية traverse_info *info, بنية name_داخلا n[3])
{
	بنية ملايو_list *داخلا = عدم;
	صحيح i;
	بنانشان dirmask = 0, نقاب = 0;

	لكل (i = 0; i < 3; i++) {
		نقاب |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		إذا (!n[i].گهڻائي || S_ISDIR(n[i].گهڻائي))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	إذا (dirmask == نقاب)
		أرجع;

	إذا (n[2].گهڻائي && !S_ISDIR(n[2].گهڻائي))
		داخلا = link_داخلا(3, info, n + 2, داخلا);
	إذا (n[1].گهڻائي && !S_ISDIR(n[1].گهڻائي))
		داخلا = link_داخلا(2, info, n + 1, داخلا);
	إذا (n[0].گهڻائي && !S_ISDIR(n[0].گهڻائي))
		داخلا = link_داخلا(1, info, n + 0, داخلا);

	جوڙيو_ملايو_داخلا(داخلا);
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
ساكن صحيح threeway_callback(صحيح n اڻ_استعمال_ٿيل, بنانشان ڊگهو نقاب,
			     بنانشان ڊگهو dirmask اڻ_استعمال_ٿيل,
			     بنية name_داخلا *داخلا, بنية traverse_info *info)
{
	/* Same in both? */
	إذا (same_داخلا(داخلا+1, داخلا+2) || both_خالي_جايون(داخلا+1, داخلا+2)) {
		/* Modified, added or removed identically */
		resolve(info, عدم, داخلا+1);
		أرجع نقاب;
	}

	إذا (same_داخلا(داخلا+0, داخلا+1)) {
		إذا (!is_null_oid(&داخلا[2].oid) && !S_ISDIR(داخلا[2].گهڻائي)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, داخلا+1, داخلا+2);
			أرجع نقاب;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	إذا (same_داخلا(داخلا+0, داخلا+2) || both_خالي_جايون(داخلا+0, داخلا+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, عدم, داخلا+1);
		أرجع نقاب;
	}

	unresolved(info, داخلا);
	أرجع نقاب;
}

ساكن فراغ trivial_ملايو_trees(بنية وڻ_desc t[3], ثابت حرف *بنياد)
{
	بنية traverse_info info;

	تياري_traverse_info(&info, بنياد);
	info.fn = threeway_callback;
	traverse_trees(the_repository->اشاريو, 3, t, &info);
}

ساكن فراغ *get_وڻ_descriptor(بنية repository *r,
				 بنية وڻ_desc *desc,
				 ثابت حرف *rev)
{
	بنية object_id oid;
	فراغ *buf;

	إذا (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_وڻ_descriptor(r, desc, &oid);
	إذا (!buf)
		die("%s is not a tree", rev);
	أرجع buf;
}

ساكن صحيح trivial_ملايو(ثابت حرف *بنياد,
			 ثابت حرف *branch1,
			 ثابت حرف *branch2)
{
	بنية repository *r = the_repository;
	بنية وڻ_desc t[3];
	فراغ *buf1, *buf2, *buf3;

	buf1 = get_وڻ_descriptor(r, t+0, بنياد);
	buf2 = get_وڻ_descriptor(r, t+1, branch1);
	buf3 = get_وڻ_descriptor(r, t+2, branch2);
	trivial_ملايو_trees(t, "");
	حرر(buf1);
	حرر(buf2);
	حرر(buf3);

	show_نتيجو();
	أرجع 0;
}

تعداد گهڻائي {
	گهڻائي_UNKNOWN,
	گهڻائي_TRIVIAL,
	گهڻائي_REAL,
};

بنية ملايو_وڻ_options {
	صحيح گهڻائي;
	صحيح allow_unrelated_histories;
	صحيح show_messages;
	صحيح name_only;
	صحيح use_stdin;
	بنية ملايو_options ملايو_options;
};

ساكن صحيح real_ملايو(بنية ملايو_وڻ_options *o,
		      ثابت حرف *ملايو_بنياد,
		      ثابت حرف *branch1, ثابت حرف *branch2,
		      ثابت حرف *اڳياڙي)
{
	بنية commit *parent1, *parent2;
	بنية commit_list *ملايو_bases = عدم;
	بنية ملايو_نتيجو نتيجو = { 0 };
	صحيح show_messages = o->show_messages;
	بنية ملايو_options opt;

	copy_ملايو_options(&opt, &o->ملايو_options);
	opt.show_rename_ترقي = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	إذا (ملايو_بنياد) {
		بنية وڻ *بنياد_وڻ, *parent1_وڻ, *parent2_وڻ;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		بنية object_id بنياد_oid, سر_oid, ملايو_oid;

		إذا (repo_get_oid_treeish(the_repository, ملايو_بنياد, &بنياد_oid))
			die(_("could not parse as tree '%s'"), ملايو_بنياد);
		بنياد_وڻ = repo_parse_وڻ_اڻسڌو(the_repository,
						     &بنياد_oid);
		إذا (!بنياد_وڻ)
			die(_("unable to read tree (%s)"), oid_منزل_hex(&بنياد_oid));
		إذا (repo_get_oid_treeish(the_repository, branch1, &سر_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_وڻ = repo_parse_وڻ_اڻسڌو(the_repository,
							&سر_oid);
		إذا (!parent1_وڻ)
			die(_("unable to read tree (%s)"), oid_منزل_hex(&سر_oid));
		إذا (repo_get_oid_treeish(the_repository, branch2, &ملايو_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_وڻ = repo_parse_وڻ_اڻسڌو(the_repository,
							&ملايو_oid);
		إذا (!parent2_وڻ)
			die(_("unable to read tree (%s)"), oid_منزل_hex(&ملايو_oid));

		opt.ancestor = ملايو_بنياد;
		ملايو_incore_nonrecursive(&opt, بنياد_وڻ, parent1_وڻ, parent2_وڻ, &نتيجو);
	} وإلا {
		parent1 = get_ملايو_والد(branch1);
		إذا (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_ملايو_والد(branch2);
		إذا (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		إذا (repo_get_ملايو_bases(the_repository, parent1,
					 parent2, &ملايو_bases) < 0)
			ٻاهر_نڪرو(128);
		إذا (!ملايو_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		ملايو_bases = commit_list_ابتو(ملايو_bases);
		ملايو_incore_recursive(&opt, ملايو_bases, parent1, parent2, &نتيجو);
		commit_list_free(ملايو_bases);
	}

	إذا (نتيجو.clean < 0)
		die(_("failure to merge"));

	إذا (o->ملايو_options.mergeability_only)
		اذهب cleanup;

	إذا (show_messages == -1)
		show_messages = !نتيجو.clean;

	إذا (o->use_stdin)
		لکو("%d%c", نتيجو.clean, سٽ_termination);
	لکو("%s%c", oid_منزل_hex(&نتيجو.وڻ->object.oid), سٽ_termination);
	إذا (!نتيجو.clean) {
		بنية string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ثابت حرف *آخري = عدم;

		ملايو_get_conflicted_files(&نتيجو, &conflicted_files);
		لكل (سائيز_t i = 0; i < conflicted_files.nr; i++) {
			ثابت حرف *name = conflicted_files.شيون[i].string;
			بنية stage_info *c = conflicted_files.شيون[i].util;
			إذا (!o->name_only)
				لکو("%06o %s %d\t",
				       c->گهڻائي, oid_منزل_hex(&c->oid), c->stage);
			وإلا إذا (آخري && !strcmp(آخري, name))
				استمر;
			write_name_quoted_relative(
				name, اڳياڙي, stdout, سٽ_termination);
			آخري = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	إذا (show_messages) {
		اکر_لکو(سٽ_termination);
		ملايو_display_update_messages(&opt, سٽ_termination == '\0',
					      &نتيجو);
	}
	إذا (o->use_stdin)
		اکر_لکو(سٽ_termination);

cleanup:
	ملايو_finalize(&opt, &نتيجو);
	clear_ملايو_options(&opt);
	أرجع !نتيجو.clean; /* result.clean < 0 handled above */
}

صحيح cmd_ملايو_وڻ(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *اڳياڙي,
		   بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية ملايو_وڻ_options o = { .show_messages = -1 };
	بنية strvec xopts = STRVEC_INIT;
	صحيح expected_بچيل_argc;
	صحيح original_argc;
	ثابت حرف *ملايو_بنياد = عدم;
	صحيح ret;
	صحيح quiet = 0;

	ثابت حرف * ثابت ملايو_وڻ_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		عدم
	};
	بنية option mt_options[] = {
		OPT_CMDMODE(0, "write-tree", &o.گهڻائي,
			    N_("do a real merge instead of a trivial merge"),
			    گهڻائي_REAL),
		OPT_CMDMODE(0, "trivial-merge", &o.گهڻائي,
			    N_("do a trivial merge only"), گهڻائي_TRIVIAL),
		OPT_BOOL(0, "messages", &o.show_messages,
			 N_("also show informational/conflict messages")),
		OPT_BOOL_F(0, "quiet",
			   &quiet,
			   N_("suppress all output; only exit status wanted"),
			   PARSE_OPT_NONEG),
		OPT_سيٽ_INT('z', عدم, &سٽ_termination,
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
			   &ملايو_بنياد,
			   N_("tree-ish"),
			   N_("specify a merge-base for the merge")),
		OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
			N_("option for selected merge strategy")),
		OPT_پڇاڙي()
	};

	/* Init merge options */
	init_basic_ملايو_options(&o.ملايو_options, the_repository);

	/* Parse arguments */
	original_argc = argc - 1; /* ignoring argv[0] */
	argc = parse_options(argc, argv, اڳياڙي, mt_options,
			     ملايو_وڻ_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.ملايو_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !سٽ_termination, "-z");

	إذا (xopts.nr && o.گهڻائي == گهڻائي_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	لكل (سائيز_t x = 0; x < xopts.nr; x++)
		إذا (parse_ملايو_opt(&o.ملايو_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	إذا (o.use_stdin) {
		بنية strbuf buf = STRBUF_INIT;

		إذا (o.گهڻائي == گهڻائي_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		إذا (ملايو_بنياد)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		سٽ_termination = '\0';
		طالما (strbuf_getline_lf(&buf, stdin) != EOF) {
			بنية string_list ورهايو = STRING_LIST_INIT_NODUP;
			ثابت حرف *داخلا_ملايو_بنياد = عدم;

			string_list_ورهايو_اندريون_رکو_f(&ورهايو, buf.buf, " ", -1,
						     STRING_LIST_ورهايو_TRIM);

			إذا (ورهايو.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			إذا (!strcmp(ورهايو.شيون[1].string, "--")) {
				داخلا_ملايو_بنياد = ورهايو.شيون[0].string;
			}

			إذا (داخلا_ملايو_بنياد && ورهايو.nr == 4) {
				real_ملايو(&o, داخلا_ملايو_بنياد,
					   ورهايو.شيون[2].string, ورهايو.شيون[3].string,
					   اڳياڙي);
			} وإلا إذا (!داخلا_ملايو_بنياد && ورهايو.nr == 2) {
				real_ملايو(&o, عدم,
					   ورهايو.شيون[0].string, ورهايو.شيون[1].string,
					   اڳياڙي);
			} وإلا {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&ورهايو, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		اذهب ٻاهريون;
	}

	/* Figure out which mode to use */
	بدل (o.گهڻائي) {
	افتراضي:
		BUG("unexpected command mode %d", o.گهڻائي);
	حالة گهڻائي_UNKNOWN:
		بدل (argc) {
		افتراضي:
			usage_with_options(ملايو_وڻ_usage, mt_options);
		حالة 2:
			o.گهڻائي = گهڻائي_REAL;
			اكسر;
		حالة 3:
			o.گهڻائي = گهڻائي_TRIVIAL;
			اكسر;
		}
		expected_بچيل_argc = argc;
		اكسر;
	حالة گهڻائي_REAL:
		expected_بچيل_argc = 2;
		اكسر;
	حالة گهڻائي_TRIVIAL:
		expected_بچيل_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		اكسر;
	}
	إذا (o.گهڻائي == گهڻائي_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	إذا (argc != expected_بچيل_argc)
		usage_with_options(ملايو_وڻ_usage, mt_options);

	repo_config(the_repository, git_default_config, عدم);

	/* Do the relevant type of merge */
	إذا (o.گهڻائي == گهڻائي_REAL)
		ret = real_ملايو(&o, ملايو_بنياد, argv[0], argv[1], اڳياڙي);
	وإلا
		ret = trivial_ملايو(argv[0], argv[1], argv[2]);

ٻاهريون:
	strvec_clear(&xopts);
	أرجع ret;
}
