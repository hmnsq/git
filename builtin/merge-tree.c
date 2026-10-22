#define USE_THE_REPOSITORY_VARIABLE

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

ایستا صحیح سطر_termination = '\n';

ساختار merge_list {
	ساختار merge_list *next;
	ساختار merge_list *link;	/* other stages for this object */

	unsigned صحیح stage : 2;
	unsigned صحیح mode;
	ثابت نویسه *path;
	ساختار blob *blob;
};

ایستا ساختار merge_list *merge_result, **merge_result_end = &merge_result;

ایستا تهی add_merge_entry(ساختار merge_list *entry)
{
	*merge_result_end = entry;
	merge_result_end = &entry->next;
}

ایستا تهی trivial_merge_trees(ساختار درخت_desc t[3], ثابت نویسه *base);

ایستا ثابت نویسه *explanation(ساختار merge_list *entry)
{
	انتخاب (entry->stage) {
	حالت 0:
		بازگشت "merged";
	حالت 3:
		بازگشت "added in remote";
	حالت 2:
		اگر (entry->link)
			بازگشت "added in both";
		بازگشت "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	اگر (!entry)
		بازگشت "removed in both";

	اگر (entry->link)
		بازگشت "changed in both";

	اگر (entry->stage == 3)
		بازگشت "removed in local";
	بازگشت "removed in remote";
}

ایستا تهی *result(ساختار merge_list *entry, size_t *size)
{
	شمارش object_type type;
	ساختار blob *base, *our, *their;
	ثابت نویسه *path = entry->path;

	اگر (!entry->stage)
		بازگشت odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       size);
	base = پوچ;
	اگر (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = پوچ;
	اگر (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = پوچ;
	اگر (entry)
		their = entry->blob;
	بازگشت merge_blobs(the_repository->index, path,
			   base, our, their, size);
}

ایستا تهی *origin(ساختار merge_list *entry, size_t *size)
{
	شمارش object_type type;
	وقتی (entry) {
		اگر (entry->stage == 2)
			بازگشت odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, size);
		entry = entry->link;
	}
	بازگشت پوچ;
}

ایستا صحیح show_outf(تهی *priv UNUSED, mmfile_t *mb, صحیح nbuf)
{
	صحیح i;
	برای (i = 0; i < nbuf; i++)
		printf("%.*s", (صحیح) mb[i].size, mb[i].ptr);
	بازگشت 0;
}

ایستا تهی show_diff(ساختار merge_list *entry)
{
	size_t size;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_سطر = show_outf };

	memset(&xpp, 0, اندازه(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, اندازه(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &size);
	اگر (!src.ptr)
		size = 0;
	src.size = size;
	dst.ptr = result(entry, &size);
	اگر (!dst.ptr)
		size = 0;
	dst.size = size;
	اگر (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	آزاد(src.ptr);
	آزاد(dst.ptr);
}

ایستا تهی show_result_list(ساختار merge_list *entry)
{
	printf("%s\n", explanation(entry));
	انجامبده {
		ساختار merge_list *link = entry->link;
		ایستا ثابت نویسه *desc[4] = { "result", "base", "our", "their" };
		printf("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} وقتی (entry);
}

ایستا تهی show_result(تهی)
{
	ساختار merge_list *walk;

	walk = merge_result;
	وقتی (walk) {
		show_result_list(walk);
		show_diff(walk);
		walk = walk->next;
	}
}

/* An empty entry never compares same, not even to another empty entry */
ایستا صحیح same_entry(ساختار name_entry *a, ساختار name_entry *b)
{
	بازگشت	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

ایستا صحیح both_empty(ساختار name_entry *a, ساختار name_entry *b)
{
	بازگشت is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

ایستا ساختار merge_list *create_entry(unsigned stage, unsigned mode, ثابت ساختار object_id *oid, ثابت نویسه *path)
{
	ساختار merge_list *res = xcalloc(1, اندازه(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	بازگشت res;
}

ایستا نویسه *traverse_path(ثابت ساختار traverse_info *info, ثابت ساختار name_entry *n)
{
	ساختار strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	بازگشت strbuf_detach(&buf, پوچ);
}

ایستا تهی resolve(ثابت ساختار traverse_info *info, ساختار name_entry *ours, ساختار name_entry *result)
{
	ساختار merge_list *orig, *final;
	ثابت نویسه *path;

	/* If it's already ours, don't bother showing it */
	اگر (!ours)
		بازگشت;

	path = traverse_path(info, result);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, result->mode, &result->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

ایستا تهی unresolved_directory(ثابت ساختار traverse_info *info,
				 ساختار name_entry n[3])
{
	ساختار repository *r = the_repository;
	نویسه *newbase;
	ساختار name_entry *p;
	ساختار درخت_desc t[3];
	تهی *buf0, *buf1, *buf2;

	برای (p = n; p < n + 3; p++) {
		اگر (p->mode && S_ISDIR(p->mode))
			بشکن;
	}
	اگر (n + 3 <= p)
		بازگشت; /* there is no tree here */

	newbase = traverse_path(info, p);

#define ENTRY_OID(e) (((e)->mode && S_ISDIR((e)->mode)) ? &(e)->oid : پوچ)
	buf0 = fill_درخت_descriptor(r, t + 0, ENTRY_OID(n + 0));
	buf1 = fill_درخت_descriptor(r, t + 1, ENTRY_OID(n + 1));
	buf2 = fill_درخت_descriptor(r, t + 2, ENTRY_OID(n + 2));
#undef ENTRY_OID

	trivial_merge_trees(t, newbase);

	آزاد(buf0);
	آزاد(buf1);
	آزاد(buf2);
	آزاد(newbase);
}


ایستا ساختار merge_list *link_entry(unsigned stage, ثابت ساختار traverse_info *info, ساختار name_entry *n, ساختار merge_list *entry)
{
	ثابت نویسه *path;
	ساختار merge_list *link;

	اگر (!n->mode)
		بازگشت entry;
	اگر (entry)
		path = entry->path;
	وگرنه
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	بازگشت link;
}

ایستا تهی unresolved(ثابت ساختار traverse_info *info, ساختار name_entry n[3])
{
	ساختار merge_list *entry = پوچ;
	صحیح i;
	unsigned dirmask = 0, mask = 0;

	برای (i = 0; i < 3; i++) {
		mask |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		اگر (!n[i].mode || S_ISDIR(n[i].mode))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	اگر (dirmask == mask)
		بازگشت;

	اگر (n[2].mode && !S_ISDIR(n[2].mode))
		entry = link_entry(3, info, n + 2, entry);
	اگر (n[1].mode && !S_ISDIR(n[1].mode))
		entry = link_entry(2, info, n + 1, entry);
	اگر (n[0].mode && !S_ISDIR(n[0].mode))
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
ایستا صحیح threeway_callback(صحیح n UNUSED, unsigned long mask,
			     unsigned long dirmask UNUSED,
			     ساختار name_entry *entry, ساختار traverse_info *info)
{
	/* Same in both? */
	اگر (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, پوچ, entry+1);
		بازگشت mask;
	}

	اگر (same_entry(entry+0, entry+1)) {
		اگر (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			بازگشت mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	اگر (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, پوچ, entry+1);
		بازگشت mask;
	}

	unresolved(info, entry);
	بازگشت mask;
}

ایستا تهی trivial_merge_trees(ساختار درخت_desc t[3], ثابت نویسه *base)
{
	ساختار traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->index, 3, t, &info);
}

ایستا تهی *get_درخت_descriptor(ساختار repository *r,
				 ساختار درخت_desc *desc,
				 ثابت نویسه *rev)
{
	ساختار object_id oid;
	تهی *buf;

	اگر (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_درخت_descriptor(r, desc, &oid);
	اگر (!buf)
		die("%s is not a tree", rev);
	بازگشت buf;
}

ایستا صحیح trivial_merge(ثابت نویسه *base,
			 ثابت نویسه *branch1,
			 ثابت نویسه *branch2)
{
	ساختار repository *r = the_repository;
	ساختار درخت_desc t[3];
	تهی *buf1, *buf2, *buf3;

	buf1 = get_درخت_descriptor(r, t+0, base);
	buf2 = get_درخت_descriptor(r, t+1, branch1);
	buf3 = get_درخت_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	آزاد(buf1);
	آزاد(buf2);
	آزاد(buf3);

	show_result();
	بازگشت 0;
}

شمارش mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

ساختار merge_درخت_options {
	صحیح mode;
	صحیح allow_unrelated_histories;
	صحیح show_messages;
	صحیح name_only;
	صحیح use_stdin;
	ساختار merge_options merge_options;
};

ایستا صحیح real_merge(ساختار merge_درخت_options *o,
		      ثابت نویسه *merge_base,
		      ثابت نویسه *branch1, ثابت نویسه *branch2,
		      ثابت نویسه *prefix)
{
	ساختار commit *parent1, *parent2;
	ساختار commit_list *merge_bases = پوچ;
	ساختار merge_result result = { 0 };
	صحیح show_messages = o->show_messages;
	ساختار merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	اگر (merge_base) {
		ساختار درخت *base_درخت, *parent1_درخت, *parent2_درخت;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		ساختار object_id base_oid, head_oid, merge_oid;

		اگر (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_درخت = repo_parse_درخت_indirect(the_repository,
						     &base_oid);
		اگر (!base_درخت)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		اگر (repo_get_oid_treeish(the_repository, branch1, &head_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_درخت = repo_parse_درخت_indirect(the_repository,
							&head_oid);
		اگر (!parent1_درخت)
			die(_("unable to read tree (%s)"), oid_to_hex(&head_oid));
		اگر (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_درخت = repo_parse_درخت_indirect(the_repository,
							&merge_oid);
		اگر (!parent2_درخت)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_درخت, parent1_درخت, parent2_درخت, &result);
	} وگرنه {
		parent1 = get_merge_parent(branch1);
		اگر (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		اگر (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		اگر (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			exit(128);
		اگر (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &result);
		commit_list_free(merge_bases);
	}

	اگر (result.clean < 0)
		die(_("failure to merge"));

	اگر (o->merge_options.mergeability_only)
		برو cleanup;

	اگر (show_messages == -1)
		show_messages = !result.clean;

	اگر (o->use_stdin)
		printf("%d%c", result.clean, سطر_termination);
	printf("%s%c", oid_to_hex(&result.درخت->object.oid), سطر_termination);
	اگر (!result.clean) {
		ساختار string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ثابت نویسه *last = پوچ;

		merge_get_conflicted_files(&result, &conflicted_files);
		برای (size_t i = 0; i < conflicted_files.nr; i++) {
			ثابت نویسه *name = conflicted_files.اقلام[i].string;
			ساختار stage_info *c = conflicted_files.اقلام[i].util;
			اگر (!o->name_only)
				printf("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			وگرنه اگر (last && !strcmp(last, name))
				ادامه;
			write_name_quoted_relative(
				name, prefix, stdout, سطر_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	اگر (show_messages) {
		putchar(سطر_termination);
		merge_display_update_messages(&opt, سطر_termination == '\0',
					      &result);
	}
	اگر (o->use_stdin)
		putchar(سطر_termination);

cleanup:
	merge_finalize(&opt, &result);
	clear_merge_options(&opt);
	بازگشت !result.clean; /* result.clean < 0 handled above */
}

صحیح cmd_merge_درخت(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix,
		   ساختار repository *repo UNUSED)
{
	ساختار merge_درخت_options o = { .show_messages = -1 };
	ساختار strvec xopts = STRVEC_INIT;
	صحیح expected_remaining_argc;
	صحیح original_argc;
	ثابت نویسه *merge_base = پوچ;
	صحیح ret;
	صحیح quiet = 0;

	ثابت نویسه * ثابت merge_درخت_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		پوچ
	};
	ساختار option mt_options[] = {
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
		OPT_SET_INT('z', پوچ, &سطر_termination,
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
			     merge_درخت_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	اگر (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !سطر_termination, "-z");

	اگر (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	برای (size_t x = 0; x < xopts.nr; x++)
		اگر (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	اگر (o.use_stdin) {
		ساختار strbuf buf = STRBUF_INIT;

		اگر (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		اگر (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		سطر_termination = '\0';
		وقتی (strbuf_getline_lf(&buf, stdin) != EOF) {
			ساختار string_list split = STRING_LIST_INIT_NODUP;
			ثابت نویسه *درون_داد_merge_base = پوچ;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			اگر (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			اگر (!strcmp(split.اقلام[1].string, "--")) {
				درون_داد_merge_base = split.اقلام[0].string;
			}

			اگر (درون_داد_merge_base && split.nr == 4) {
				real_merge(&o, درون_داد_merge_base,
					   split.اقلام[2].string, split.اقلام[3].string,
					   prefix);
			} وگرنه اگر (!درون_داد_merge_base && split.nr == 2) {
				real_merge(&o, پوچ,
					   split.اقلام[0].string, split.اقلام[1].string,
					   prefix);
			} وگرنه {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&split, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		برو out;
	}

	/* Figure out which mode to use */
	انتخاب (o.mode) {
	پیشفرض:
		BUG("unexpected command mode %d", o.mode);
	حالت MODE_UNKNOWN:
		انتخاب (argc) {
		پیشفرض:
			usage_with_options(merge_درخت_usage, mt_options);
		حالت 2:
			o.mode = MODE_REAL;
			بشکن;
		حالت 3:
			o.mode = MODE_TRIVIAL;
			بشکن;
		}
		expected_remaining_argc = argc;
		بشکن;
	حالت MODE_REAL:
		expected_remaining_argc = 2;
		بشکن;
	حالت MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		بشکن;
	}
	اگر (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	اگر (argc != expected_remaining_argc)
		usage_with_options(merge_درخت_usage, mt_options);

	repo_config(the_repository, git_default_config, پوچ);

	/* Do the relevant type of merge */
	اگر (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	وگرنه
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	بازگشت ret;
}
