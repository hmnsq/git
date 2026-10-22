#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "tree-walk.h"
#অন্তর্ভাব "xdiff-interface.h"
#অন্তর্ভাব "help.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "merge-ort.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "blob.h"
#অন্তর্ভাব "merge-blobs.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "tree.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "write-or-die.h"

স্থির পূর্ণসংখ্যা সারি_termination = '\n';

কাঠামো merge_list {
	কাঠামো merge_list *পরবর্তী;
	কাঠামো merge_list *link;	/* other stages for this object */

	unsigned পূর্ণসংখ্যা stage : 2;
	unsigned পূর্ণসংখ্যা mode;
	ধ্রুবক অক্ষর *path;
	কাঠামো blob *blob;
};

স্থির কাঠামো merge_list *merge_ফলাফল, **merge_ফলাফল_end = &merge_ফলাফল;

স্থির শূন্য add_merge_entry(কাঠামো merge_list *entry)
{
	*merge_ফলাফল_end = entry;
	merge_ফলাফল_end = &entry->পরবর্তী;
}

স্থির শূন্য trivial_merge_trees(কাঠামো বৃক্ষ_desc t[3], ধ্রুবক অক্ষর *base);

স্থির ধ্রুবক অক্ষর *explanation(কাঠামো merge_list *entry)
{
	নির্বাচন (entry->stage) {
	ক্ষেত্র 0:
		ফেরত "merged";
	ক্ষেত্র 3:
		ফেরত "added in remote";
	ক্ষেত্র 2:
		যদি (entry->link)
			ফেরত "added in both";
		ফেরত "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	যদি (!entry)
		ফেরত "removed in both";

	যদি (entry->link)
		ফেরত "changed in both";

	যদি (entry->stage == 3)
		ফেরত "removed in local";
	ফেরত "removed in remote";
}

স্থির শূন্য *ফলাফল(কাঠামো merge_list *entry, আকার_t *আকার_2)
{
	গণনা object_type type;
	কাঠামো blob *base, *our, *their;
	ধ্রুবক অক্ষর *path = entry->path;

	যদি (!entry->stage)
		ফেরত odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       আকার_2);
	base = ফাঁকা;
	যদি (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = ফাঁকা;
	যদি (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = ফাঁকা;
	যদি (entry)
		their = entry->blob;
	ফেরত merge_blobs(the_repository->সূচক, path,
			   base, our, their, আকার_2);
}

স্থির শূন্য *origin(কাঠামো merge_list *entry, আকার_t *আকার_2)
{
	গণনা object_type type;
	যখন (entry) {
		যদি (entry->stage == 2)
			ফেরত odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, আকার_2);
		entry = entry->link;
	}
	ফেরত ফাঁকা;
}

স্থির পূর্ণসংখ্যা show_outf(শূন্য *priv UNUSED, mmfile_t *mb, পূর্ণসংখ্যা nbuf)
{
	পূর্ণসংখ্যা i;
	জন্য (i = 0; i < nbuf; i++)
		printf("%.*s", (পূর্ণসংখ্যা) mb[i].আকার_2, mb[i].ptr);
	ফেরত 0;
}

স্থির শূন্য show_diff(কাঠামো merge_list *entry)
{
	আকার_t আকার_2;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_সারি = show_outf };

	memset(&xpp, 0, আকার(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, আকার(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &আকার_2);
	যদি (!src.ptr)
		আকার_2 = 0;
	src.আকার_2 = আকার_2;
	dst.ptr = ফলাফল(entry, &আকার_2);
	যদি (!dst.ptr)
		আকার_2 = 0;
	dst.আকার_2 = আকার_2;
	যদি (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	মুক্ত(src.ptr);
	মুক্ত(dst.ptr);
}

স্থির শূন্য show_ফলাফল_list(কাঠামো merge_list *entry)
{
	printf("%s\n", explanation(entry));
	করো {
		কাঠামো merge_list *link = entry->link;
		স্থির ধ্রুবক অক্ষর *desc[4] = { "result", "base", "our", "their" };
		printf("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} যখন (entry);
}

স্থির শূন্য show_ফলাফল(শূন্য)
{
	কাঠামো merge_list *walk;

	walk = merge_ফলাফল;
	যখন (walk) {
		show_ফলাফল_list(walk);
		show_diff(walk);
		walk = walk->পরবর্তী;
	}
}

/* An empty entry never compares same, not even to another empty entry */
স্থির পূর্ণসংখ্যা same_entry(কাঠামো name_entry *a, কাঠামো name_entry *b)
{
	ফেরত	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

স্থির পূর্ণসংখ্যা both_empty(কাঠামো name_entry *a, কাঠামো name_entry *b)
{
	ফেরত is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

স্থির কাঠামো merge_list *create_entry(unsigned stage, unsigned mode, ধ্রুবক কাঠামো object_id *oid, ধ্রুবক অক্ষর *path)
{
	কাঠামো merge_list *res = xcalloc(1, আকার(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	ফেরত res;
}

স্থির অক্ষর *traverse_path(ধ্রুবক কাঠামো traverse_info *info, ধ্রুবক কাঠামো name_entry *n)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	ফেরত strbuf_detach(&buf, ফাঁকা);
}

স্থির শূন্য resolve(ধ্রুবক কাঠামো traverse_info *info, কাঠামো name_entry *ours, কাঠামো name_entry *ফলাফল)
{
	কাঠামো merge_list *orig, *final;
	ধ্রুবক অক্ষর *path;

	/* If it's already ours, don't bother showing it */
	যদি (!ours)
		ফেরত;

	path = traverse_path(info, ফলাফল);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, ফলাফল->mode, &ফলাফল->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

স্থির শূন্য unresolved_directory(ধ্রুবক কাঠামো traverse_info *info,
				 কাঠামো name_entry n[3])
{
	কাঠামো repository *r = the_repository;
	অক্ষর *newbase;
	কাঠামো name_entry *p;
	কাঠামো বৃক্ষ_desc t[3];
	শূন্য *buf0, *buf1, *buf2;

	জন্য (p = n; p < n + 3; p++) {
		যদি (p->mode && S_ISDIR(p->mode))
			বিরতি;
	}
	যদি (n + 3 <= p)
		ফেরত; /* there is no tree here */

	newbase = traverse_path(info, p);

#define ENTRY_OID(e) (((e)->mode && S_ISDIR((e)->mode)) ? &(e)->oid : ফাঁকা)
	buf0 = fill_বৃক্ষ_descriptor(r, t + 0, ENTRY_OID(n + 0));
	buf1 = fill_বৃক্ষ_descriptor(r, t + 1, ENTRY_OID(n + 1));
	buf2 = fill_বৃক্ষ_descriptor(r, t + 2, ENTRY_OID(n + 2));
#undef ENTRY_OID

	trivial_merge_trees(t, newbase);

	মুক্ত(buf0);
	মুক্ত(buf1);
	মুক্ত(buf2);
	মুক্ত(newbase);
}


স্থির কাঠামো merge_list *link_entry(unsigned stage, ধ্রুবক কাঠামো traverse_info *info, কাঠামো name_entry *n, কাঠামো merge_list *entry)
{
	ধ্রুবক অক্ষর *path;
	কাঠামো merge_list *link;

	যদি (!n->mode)
		ফেরত entry;
	যদি (entry)
		path = entry->path;
	অন্যথা
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	ফেরত link;
}

স্থির শূন্য unresolved(ধ্রুবক কাঠামো traverse_info *info, কাঠামো name_entry n[3])
{
	কাঠামো merge_list *entry = ফাঁকা;
	পূর্ণসংখ্যা i;
	unsigned dirmask = 0, mask = 0;

	জন্য (i = 0; i < 3; i++) {
		mask |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		যদি (!n[i].mode || S_ISDIR(n[i].mode))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	যদি (dirmask == mask)
		ফেরত;

	যদি (n[2].mode && !S_ISDIR(n[2].mode))
		entry = link_entry(3, info, n + 2, entry);
	যদি (n[1].mode && !S_ISDIR(n[1].mode))
		entry = link_entry(2, info, n + 1, entry);
	যদি (n[0].mode && !S_ISDIR(n[0].mode))
		entry = link_entry(1, info, n + 0, entry);

	add_merge_entry(entry);
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
স্থির পূর্ণসংখ্যা threeway_callback(পূর্ণসংখ্যা n UNUSED, unsigned long mask,
			     unsigned long dirmask UNUSED,
			     কাঠামো name_entry *entry, কাঠামো traverse_info *info)
{
	/* Same in both? */
	যদি (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, ফাঁকা, entry+1);
		ফেরত mask;
	}

	যদি (same_entry(entry+0, entry+1)) {
		যদি (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			ফেরত mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	যদি (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, ফাঁকা, entry+1);
		ফেরত mask;
	}

	unresolved(info, entry);
	ফেরত mask;
}

স্থির শূন্য trivial_merge_trees(কাঠামো বৃক্ষ_desc t[3], ধ্রুবক অক্ষর *base)
{
	কাঠামো traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->সূচক, 3, t, &info);
}

স্থির শূন্য *get_বৃক্ষ_descriptor(কাঠামো repository *r,
				 কাঠামো বৃক্ষ_desc *desc,
				 ধ্রুবক অক্ষর *rev)
{
	কাঠামো object_id oid;
	শূন্য *buf;

	যদি (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_বৃক্ষ_descriptor(r, desc, &oid);
	যদি (!buf)
		die("%s is not a tree", rev);
	ফেরত buf;
}

স্থির পূর্ণসংখ্যা trivial_merge(ধ্রুবক অক্ষর *base,
			 ধ্রুবক অক্ষর *branch1,
			 ধ্রুবক অক্ষর *branch2)
{
	কাঠামো repository *r = the_repository;
	কাঠামো বৃক্ষ_desc t[3];
	শূন্য *buf1, *buf2, *buf3;

	buf1 = get_বৃক্ষ_descriptor(r, t+0, base);
	buf2 = get_বৃক্ষ_descriptor(r, t+1, branch1);
	buf3 = get_বৃক্ষ_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	মুক্ত(buf1);
	মুক্ত(buf2);
	মুক্ত(buf3);

	show_ফলাফল();
	ফেরত 0;
}

গণনা mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

কাঠামো merge_বৃক্ষ_options {
	পূর্ণসংখ্যা mode;
	পূর্ণসংখ্যা allow_unrelated_histories;
	পূর্ণসংখ্যা show_messages;
	পূর্ণসংখ্যা name_only;
	পূর্ণসংখ্যা use_stdin;
	কাঠামো merge_options merge_options;
};

স্থির পূর্ণসংখ্যা real_merge(কাঠামো merge_বৃক্ষ_options *o,
		      ধ্রুবক অক্ষর *merge_base,
		      ধ্রুবক অক্ষর *branch1, ধ্রুবক অক্ষর *branch2,
		      ধ্রুবক অক্ষর *prefix)
{
	কাঠামো commit *parent1, *parent2;
	কাঠামো commit_list *merge_bases = ফাঁকা;
	কাঠামো merge_ফলাফল ফলাফল = { 0 };
	পূর্ণসংখ্যা show_messages = o->show_messages;
	কাঠামো merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	যদি (merge_base) {
		কাঠামো বৃক্ষ *base_বৃক্ষ, *parent1_বৃক্ষ, *parent2_বৃক্ষ;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		কাঠামো object_id base_oid, শীর্ষ_oid, merge_oid;

		যদি (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository,
						     &base_oid);
		যদি (!base_বৃক্ষ)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		যদি (repo_get_oid_treeish(the_repository, branch1, &শীর্ষ_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository,
							&শীর্ষ_oid);
		যদি (!parent1_বৃক্ষ)
			die(_("unable to read tree (%s)"), oid_to_hex(&শীর্ষ_oid));
		যদি (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository,
							&merge_oid);
		যদি (!parent2_বৃক্ষ)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_বৃক্ষ, parent1_বৃক্ষ, parent2_বৃক্ষ, &ফলাফল);
	} অন্যথা {
		parent1 = get_merge_parent(branch1);
		যদি (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		যদি (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		যদি (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			exit(128);
		যদি (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &ফলাফল);
		commit_list_free(merge_bases);
	}

	যদি (ফলাফল.clean < 0)
		die(_("failure to merge"));

	যদি (o->merge_options.mergeability_only)
		যাও cleanup;

	যদি (show_messages == -1)
		show_messages = !ফলাফল.clean;

	যদি (o->use_stdin)
		printf("%d%c", ফলাফল.clean, সারি_termination);
	printf("%s%c", oid_to_hex(&ফলাফল.বৃক্ষ->object.oid), সারি_termination);
	যদি (!ফলাফল.clean) {
		কাঠামো string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ধ্রুবক অক্ষর *last = ফাঁকা;

		merge_get_conflicted_files(&ফলাফল, &conflicted_files);
		জন্য (আকার_t i = 0; i < conflicted_files.nr; i++) {
			ধ্রুবক অক্ষর *name = conflicted_files.বস্তুসমূহ[i].string;
			কাঠামো stage_info *c = conflicted_files.বস্তুসমূহ[i].util;
			যদি (!o->name_only)
				printf("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			অন্যথা যদি (last && !strcmp(last, name))
				চালু;
			write_name_quoted_relative(
				name, prefix, stdout, সারি_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	যদি (show_messages) {
		putchar(সারি_termination);
		merge_display_update_messages(&opt, সারি_termination == '\0',
					      &ফলাফল);
	}
	যদি (o->use_stdin)
		putchar(সারি_termination);

cleanup:
	merge_finalize(&opt, &ফলাফল);
	clear_merge_options(&opt);
	ফেরত !ফলাফল.clean; /* result.clean < 0 handled above */
}

পূর্ণসংখ্যা cmd_merge_বৃক্ষ(পূর্ণসংখ্যা argc,
		   ধ্রুবক অক্ষর **argv,
		   ধ্রুবক অক্ষর *prefix,
		   কাঠামো repository *repo UNUSED)
{
	কাঠামো merge_বৃক্ষ_options o = { .show_messages = -1 };
	কাঠামো strvec xopts = STRVEC_INIT;
	পূর্ণসংখ্যা expected_remaining_argc;
	পূর্ণসংখ্যা original_argc;
	ধ্রুবক অক্ষর *merge_base = ফাঁকা;
	পূর্ণসংখ্যা ret;
	পূর্ণসংখ্যা quiet = 0;

	ধ্রুবক অক্ষর * ধ্রুবক merge_বৃক্ষ_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		ফাঁকা
	};
	কাঠামো option mt_options[] = {
		OPT_CMDMODE(0, "write-tree", &o.mode,
			    N_("do a real merge instead of a trivial merge"),
			    MODE_REAL),
		OPT_CMDMODE(0, "trivial-merge", &o.mode,
			    N_("do a trivial merge only"), MODE_TRIVIAL),
		OPT_BOOL(0, "messages", &o.show_messages,
			 N_("also show informational/conflict messages")),
		OPT_BOOL_F(0, "quiet",
			   &quiet,
			   N_("suppress all output; only exit status wanted"),
			   PARSE_OPT_NONEG),
		OPT_SET_INT('z', ফাঁকা, &সারি_termination,
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
			   &merge_base,
			   N_("tree-ish"),
			   N_("specify a merge-base for the merge")),
		OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
			N_("option for selected merge strategy")),
		OPT_END()
	};

	/* Init merge options */
	init_basic_merge_options(&o.merge_options, the_repository);

	/* Parse arguments */
	original_argc = argc - 1; /* ignoring argv[0] */
	argc = parse_options(argc, argv, prefix, mt_options,
			     merge_বৃক্ষ_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	যদি (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !সারি_termination, "-z");

	যদি (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	জন্য (আকার_t x = 0; x < xopts.nr; x++)
		যদি (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	যদি (o.use_stdin) {
		কাঠামো strbuf buf = STRBUF_INIT;

		যদি (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		যদি (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		সারি_termination = '\0';
		যখন (strbuf_getline_lf(&buf, stdin) != EOF) {
			কাঠামো string_list split = STRING_LIST_INIT_NODUP;
			ধ্রুবক অক্ষর *প্রবেশ_merge_base = ফাঁকা;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			যদি (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			যদি (!strcmp(split.বস্তুসমূহ[1].string, "--")) {
				প্রবেশ_merge_base = split.বস্তুসমূহ[0].string;
			}

			যদি (প্রবেশ_merge_base && split.nr == 4) {
				real_merge(&o, প্রবেশ_merge_base,
					   split.বস্তুসমূহ[2].string, split.বস্তুসমূহ[3].string,
					   prefix);
			} অন্যথা যদি (!প্রবেশ_merge_base && split.nr == 2) {
				real_merge(&o, ফাঁকা,
					   split.বস্তুসমূহ[0].string, split.বস্তুসমূহ[1].string,
					   prefix);
			} অন্যথা {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&split, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		যাও out;
	}

	/* Figure out which mode to use */
	নির্বাচন (o.mode) {
	পূর্বনির্ধারিত:
		BUG("unexpected command mode %d", o.mode);
	ক্ষেত্র MODE_UNKNOWN:
		নির্বাচন (argc) {
		পূর্বনির্ধারিত:
			usage_with_options(merge_বৃক্ষ_usage, mt_options);
		ক্ষেত্র 2:
			o.mode = MODE_REAL;
			বিরতি;
		ক্ষেত্র 3:
			o.mode = MODE_TRIVIAL;
			বিরতি;
		}
		expected_remaining_argc = argc;
		বিরতি;
	ক্ষেত্র MODE_REAL:
		expected_remaining_argc = 2;
		বিরতি;
	ক্ষেত্র MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		বিরতি;
	}
	যদি (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	যদি (argc != expected_remaining_argc)
		usage_with_options(merge_বৃক্ষ_usage, mt_options);

	repo_config(the_repository, git_default_config, ফাঁকা);

	/* Do the relevant type of merge */
	যদি (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	অন্যথা
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	ফেরত ret;
}
