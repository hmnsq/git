#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "tree-walk.h"
#অন্তর্ভাৱ "xdiff-interface.h"
#অন্তর্ভাৱ "help.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "commit.h"
#অন্তর্ভাৱ "commit-reach.h"
#অন্তর্ভাৱ "merge-ort.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "blob.h"
#অন্তর্ভাৱ "merge-blobs.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "tree.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "strvec.h"
#অন্তর্ভাৱ "write-or-die.h"

স্থির পূর্ণাঙ্ক সাৰি_termination = '\n';

সংরচনা merge_list {
	সংরচনা merge_list *next;
	সংরচনা merge_list *link;	/* other stages for this object */

	unsigned পূর্ণাঙ্ক stage : 2;
	unsigned পূর্ণাঙ্ক mode;
	ধ্রুৱ ৱর্ণ *path;
	সংরচনা blob *blob;
};

স্থির সংরচনা merge_list *merge_result, **merge_result_end = &merge_result;

স্থির শূন্য add_merge_entry(সংরচনা merge_list *entry)
{
	*merge_result_end = entry;
	merge_result_end = &entry->next;
}

স্থির শূন্য trivial_merge_trees(সংরচনা বৃক্ষ_desc t[3], ধ্রুৱ ৱর্ণ *base);

স্থির ধ্রুৱ ৱর্ণ *explanation(সংরচনা merge_list *entry)
{
	ৱিকল্প (entry->stage) {
	পক্ষ 0:
		ঘূৰাই "merged";
	পক্ষ 3:
		ঘূৰাই "added in remote";
	পক্ষ 2:
		যদি (entry->link)
			ঘূৰাই "added in both";
		ঘূৰাই "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	যদি (!entry)
		ঘূৰাই "removed in both";

	যদি (entry->link)
		ঘূৰাই "changed in both";

	যদি (entry->stage == 3)
		ঘূৰাই "removed in local";
	ঘূৰাই "removed in remote";
}

স্থির শূন্য *result(সংরচনা merge_list *entry, size_t *size)
{
	গণনা object_type type;
	সংরচনা blob *base, *our, *their;
	ধ্রুৱ ৱর্ণ *path = entry->path;

	যদি (!entry->stage)
		ঘূৰাই odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       size);
	base = রিক্ত;
	যদি (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = রিক্ত;
	যদি (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = রিক্ত;
	যদি (entry)
		their = entry->blob;
	ঘূৰাই merge_blobs(the_repository->index, path,
			   base, our, their, size);
}

স্থির শূন্য *origin(সংরচনা merge_list *entry, size_t *size)
{
	গণনা object_type type;
	যেতিয়ালৈ (entry) {
		যদি (entry->stage == 2)
			ঘূৰাই odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, size);
		entry = entry->link;
	}
	ঘূৰাই রিক্ত;
}

স্থির পূর্ণাঙ্ক show_outf(শূন্য *priv UNUSED, mmfile_t *mb, পূর্ণাঙ্ক nbuf)
{
	পূর্ণাঙ্ক i;
	বাবে (i = 0; i < nbuf; i++)
		printf("%.*s", (পূর্ণাঙ্ক) mb[i].size, mb[i].ptr);
	ঘূৰাই 0;
}

স্থির শূন্য show_diff(সংরচনা merge_list *entry)
{
	size_t size;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_সাৰি = show_outf };

	memset(&xpp, 0, পরিমাণ(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, পরিমাণ(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &size);
	যদি (!src.ptr)
		size = 0;
	src.size = size;
	dst.ptr = result(entry, &size);
	যদি (!dst.ptr)
		size = 0;
	dst.size = size;
	যদি (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	মুক্ত(src.ptr);
	মুক্ত(dst.ptr);
}

স্থির শূন্য show_result_list(সংরচনা merge_list *entry)
{
	printf("%s\n", explanation(entry));
	কৰা {
		সংরচনা merge_list *link = entry->link;
		স্থির ধ্রুৱ ৱর্ণ *desc[4] = { "result", "base", "our", "their" };
		printf("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} যেতিয়ালৈ (entry);
}

স্থির শূন্য show_result(শূন্য)
{
	সংরচনা merge_list *walk;

	walk = merge_result;
	যেতিয়ালৈ (walk) {
		show_result_list(walk);
		show_diff(walk);
		walk = walk->next;
	}
}

/* An empty entry never compares same, not even to another empty entry */
স্থির পূর্ণাঙ্ক same_entry(সংরচনা name_entry *a, সংরচনা name_entry *b)
{
	ঘূৰাই	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

স্থির পূর্ণাঙ্ক both_empty(সংরচনা name_entry *a, সংরচনা name_entry *b)
{
	ঘূৰাই is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

স্থির সংরচনা merge_list *create_entry(unsigned stage, unsigned mode, ধ্রুৱ সংরচনা object_id *oid, ধ্রুৱ ৱর্ণ *path)
{
	সংরচনা merge_list *res = xcalloc(1, পরিমাণ(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	ঘূৰাই res;
}

স্থির ৱর্ণ *traverse_path(ধ্রুৱ সংরচনা traverse_info *info, ধ্রুৱ সংরচনা name_entry *n)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	ঘূৰাই strbuf_detach(&buf, রিক্ত);
}

স্থির শূন্য resolve(ধ্রুৱ সংরচনা traverse_info *info, সংরচনা name_entry *ours, সংরচনা name_entry *result)
{
	সংরচনা merge_list *orig, *final;
	ধ্রুৱ ৱর্ণ *path;

	/* If it's already ours, don't bother showing it */
	যদি (!ours)
		ঘূৰাই;

	path = traverse_path(info, result);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, result->mode, &result->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

স্থির শূন্য unresolved_directory(ধ্রুৱ সংরচনা traverse_info *info,
				 সংরচনা name_entry n[3])
{
	সংরচনা repository *r = the_repository;
	ৱর্ণ *newbase;
	সংরচনা name_entry *p;
	সংরচনা বৃক্ষ_desc t[3];
	শূন্য *buf0, *buf1, *buf2;

	বাবে (p = n; p < n + 3; p++) {
		যদি (p->mode && S_ISDIR(p->mode))
			ভঙ্গ;
	}
	যদি (n + 3 <= p)
		ঘূৰাই; /* there is no tree here */

	newbase = traverse_path(info, p);

#define ENTRY_OID(e) (((e)->mode && S_ISDIR((e)->mode)) ? &(e)->oid : রিক্ত)
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


স্থির সংরচনা merge_list *link_entry(unsigned stage, ধ্রুৱ সংরচনা traverse_info *info, সংরচনা name_entry *n, সংরচনা merge_list *entry)
{
	ধ্রুৱ ৱর্ণ *path;
	সংরচনা merge_list *link;

	যদি (!n->mode)
		ঘূৰাই entry;
	যদি (entry)
		path = entry->path;
	অন্যথা
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	ঘূৰাই link;
}

স্থির শূন্য unresolved(ধ্রুৱ সংরচনা traverse_info *info, সংরচনা name_entry n[3])
{
	সংরচনা merge_list *entry = রিক্ত;
	পূর্ণাঙ্ক i;
	unsigned dirmask = 0, mask = 0;

	বাবে (i = 0; i < 3; i++) {
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
		ঘূৰাই;

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
স্থির পূর্ণাঙ্ক threeway_callback(পূর্ণাঙ্ক n UNUSED, unsigned long mask,
			     unsigned long dirmask UNUSED,
			     সংরচনা name_entry *entry, সংরচনা traverse_info *info)
{
	/* Same in both? */
	যদি (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, রিক্ত, entry+1);
		ঘূৰাই mask;
	}

	যদি (same_entry(entry+0, entry+1)) {
		যদি (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			ঘূৰাই mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	যদি (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, রিক্ত, entry+1);
		ঘূৰাই mask;
	}

	unresolved(info, entry);
	ঘূৰাই mask;
}

স্থির শূন্য trivial_merge_trees(সংরচনা বৃক্ষ_desc t[3], ধ্রুৱ ৱর্ণ *base)
{
	সংরচনা traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->index, 3, t, &info);
}

স্থির শূন্য *get_বৃক্ষ_descriptor(সংরচনা repository *r,
				 সংরচনা বৃক্ষ_desc *desc,
				 ধ্রুৱ ৱর্ণ *rev)
{
	সংরচনা object_id oid;
	শূন্য *buf;

	যদি (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_বৃক্ষ_descriptor(r, desc, &oid);
	যদি (!buf)
		die("%s is not a tree", rev);
	ঘূৰাই buf;
}

স্থির পূর্ণাঙ্ক trivial_merge(ধ্রুৱ ৱর্ণ *base,
			 ধ্রুৱ ৱর্ণ *branch1,
			 ধ্রুৱ ৱর্ণ *branch2)
{
	সংরচনা repository *r = the_repository;
	সংরচনা বৃক্ষ_desc t[3];
	শূন্য *buf1, *buf2, *buf3;

	buf1 = get_বৃক্ষ_descriptor(r, t+0, base);
	buf2 = get_বৃক্ষ_descriptor(r, t+1, branch1);
	buf3 = get_বৃক্ষ_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	মুক্ত(buf1);
	মুক্ত(buf2);
	মুক্ত(buf3);

	show_result();
	ঘূৰাই 0;
}

গণনা mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

সংরচনা merge_বৃক্ষ_options {
	পূর্ণাঙ্ক mode;
	পূর্ণাঙ্ক allow_unrelated_histories;
	পূর্ণাঙ্ক show_messages;
	পূর্ণাঙ্ক name_only;
	পূর্ণাঙ্ক use_stdin;
	সংরচনা merge_options merge_options;
};

স্থির পূর্ণাঙ্ক real_merge(সংরচনা merge_বৃক্ষ_options *o,
		      ধ্রুৱ ৱর্ণ *merge_base,
		      ধ্রুৱ ৱর্ণ *branch1, ধ্রুৱ ৱর্ণ *branch2,
		      ধ্রুৱ ৱর্ণ *prefix)
{
	সংরচনা commit *parent1, *parent2;
	সংরচনা commit_list *merge_bases = রিক্ত;
	সংরচনা merge_result result = { 0 };
	পূর্ণাঙ্ক show_messages = o->show_messages;
	সংরচনা merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	যদি (merge_base) {
		সংরচনা বৃক্ষ *base_বৃক্ষ, *parent1_বৃক্ষ, *parent2_বৃক্ষ;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		সংরচনা object_id base_oid, head_oid, merge_oid;

		যদি (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository,
						     &base_oid);
		যদি (!base_বৃক্ষ)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		যদি (repo_get_oid_treeish(the_repository, branch1, &head_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository,
							&head_oid);
		যদি (!parent1_বৃক্ষ)
			die(_("unable to read tree (%s)"), oid_to_hex(&head_oid));
		যদি (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository,
							&merge_oid);
		যদি (!parent2_বৃক্ষ)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_বৃক্ষ, parent1_বৃক্ষ, parent2_বৃক্ষ, &result);
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
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &result);
		commit_list_free(merge_bases);
	}

	যদি (result.clean < 0)
		die(_("failure to merge"));

	যদি (o->merge_options.mergeability_only)
		গচ্ছ cleanup;

	যদি (show_messages == -1)
		show_messages = !result.clean;

	যদি (o->use_stdin)
		printf("%d%c", result.clean, সাৰি_termination);
	printf("%s%c", oid_to_hex(&result.বৃক্ষ->object.oid), সাৰি_termination);
	যদি (!result.clean) {
		সংরচনা string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ধ্রুৱ ৱর্ণ *last = রিক্ত;

		merge_get_conflicted_files(&result, &conflicted_files);
		বাবে (size_t i = 0; i < conflicted_files.nr; i++) {
			ধ্রুৱ ৱর্ণ *name = conflicted_files.বস্তুসমূহ[i].string;
			সংরচনা stage_info *c = conflicted_files.বস্তুসমূহ[i].util;
			যদি (!o->name_only)
				printf("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			অন্যথা যদি (last && !strcmp(last, name))
				অনুৱর্ত;
			write_name_quoted_relative(
				name, prefix, stdout, সাৰি_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	যদি (show_messages) {
		putchar(সাৰি_termination);
		merge_display_update_messages(&opt, সাৰি_termination == '\0',
					      &result);
	}
	যদি (o->use_stdin)
		putchar(সাৰি_termination);

cleanup:
	merge_finalize(&opt, &result);
	clear_merge_options(&opt);
	ঘূৰাই !result.clean; /* result.clean < 0 handled above */
}

পূর্ণাঙ্ক cmd_merge_বৃক্ষ(পূর্ণাঙ্ক argc,
		   ধ্রুৱ ৱর্ণ **argv,
		   ধ্রুৱ ৱর্ণ *prefix,
		   সংরচনা repository *repo UNUSED)
{
	সংরচনা merge_বৃক্ষ_options o = { .show_messages = -1 };
	সংরচনা strvec xopts = STRVEC_INIT;
	পূর্ণাঙ্ক expected_remaining_argc;
	পূর্ণাঙ্ক original_argc;
	ধ্রুৱ ৱর্ণ *merge_base = রিক্ত;
	পূর্ণাঙ্ক ret;
	পূর্ণাঙ্ক quiet = 0;

	ধ্রুৱ ৱর্ণ * ধ্রুৱ merge_বৃক্ষ_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		রিক্ত
	};
	সংরচনা option mt_options[] = {
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
		OPT_SET_INT('z', রিক্ত, &সাৰি_termination,
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
	die_for_incompatible_opt2(quiet, "--quiet", !সাৰি_termination, "-z");

	যদি (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	বাবে (size_t x = 0; x < xopts.nr; x++)
		যদি (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	যদি (o.use_stdin) {
		সংরচনা strbuf buf = STRBUF_INIT;

		যদি (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		যদি (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		সাৰি_termination = '\0';
		যেতিয়ালৈ (strbuf_getline_lf(&buf, stdin) != EOF) {
			সংরচনা string_list split = STRING_LIST_INIT_NODUP;
			ধ্রুৱ ৱর্ণ *প্ৰবেশ_merge_base = রিক্ত;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			যদি (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			যদি (!strcmp(split.বস্তুসমূহ[1].string, "--")) {
				প্ৰবেশ_merge_base = split.বস্তুসমূহ[0].string;
			}

			যদি (প্ৰবেশ_merge_base && split.nr == 4) {
				real_merge(&o, প্ৰবেশ_merge_base,
					   split.বস্তুসমূহ[2].string, split.বস্তুসমূহ[3].string,
					   prefix);
			} অন্যথা যদি (!প্ৰবেশ_merge_base && split.nr == 2) {
				real_merge(&o, রিক্ত,
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
		গচ্ছ out;
	}

	/* Figure out which mode to use */
	ৱিকল্প (o.mode) {
	স্ৱভাৱ:
		BUG("unexpected command mode %d", o.mode);
	পক্ষ MODE_UNKNOWN:
		ৱিকল্প (argc) {
		স্ৱভাৱ:
			usage_with_options(merge_বৃক্ষ_usage, mt_options);
		পক্ষ 2:
			o.mode = MODE_REAL;
			ভঙ্গ;
		পক্ষ 3:
			o.mode = MODE_TRIVIAL;
			ভঙ্গ;
		}
		expected_remaining_argc = argc;
		ভঙ্গ;
	পক্ষ MODE_REAL:
		expected_remaining_argc = 2;
		ভঙ্গ;
	পক্ষ MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		ভঙ্গ;
	}
	যদি (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	যদি (argc != expected_remaining_argc)
		usage_with_options(merge_বৃক্ষ_usage, mt_options);

	repo_config(the_repository, git_default_config, রিক্ত);

	/* Do the relevant type of merge */
	যদি (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	অন্যথা
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	ঘূৰাই ret;
}
