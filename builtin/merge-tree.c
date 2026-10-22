#define USE_THE_REPOSITORY_VARIABLE

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

ساكن صحيح باريس_termination = '\n';

بنية merge_list {
	بنية merge_list *next;
	بنية merge_list *link;	/* other stages for this object */

	unsigned صحيح stage : 2;
	unsigned صحيح mode;
	ثابت حرف *path;
	بنية blob *blob;
};

ساكن بنية merge_list *merge_result, **merge_result_end = &merge_result;

ساكن فراغ add_merge_entry(بنية merge_list *entry)
{
	*merge_result_end = entry;
	merge_result_end = &entry->next;
}

ساكن فراغ trivial_merge_trees(بنية ڤوکوق_desc t[3], ثابت حرف *base);

ساكن ثابت حرف *explanation(بنية merge_list *entry)
{
	بدل (entry->stage) {
	حالة 0:
		أرجع "merged";
	حالة 3:
		أرجع "added in remote";
	حالة 2:
		إذا (entry->link)
			أرجع "added in both";
		أرجع "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	إذا (!entry)
		أرجع "removed in both";

	إذا (entry->link)
		أرجع "changed in both";

	إذا (entry->stage == 3)
		أرجع "removed in local";
	أرجع "removed in remote";
}

ساكن فراغ *result(بنية merge_list *entry, size_t *size)
{
	تعداد object_type type;
	بنية blob *base, *our, *their;
	ثابت حرف *path = entry->path;

	إذا (!entry->stage)
		أرجع odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       size);
	base = عدم;
	إذا (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = عدم;
	إذا (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = عدم;
	إذا (entry)
		their = entry->blob;
	أرجع merge_blobs(the_repository->index, path,
			   base, our, their, size);
}

ساكن فراغ *origin(بنية merge_list *entry, size_t *size)
{
	تعداد object_type type;
	طالما (entry) {
		إذا (entry->stage == 2)
			أرجع odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, size);
		entry = entry->link;
	}
	أرجع عدم;
}

ساكن صحيح show_outf(فراغ *priv UNUSED, mmfile_t *mb, صحيح nbuf)
{
	صحيح i;
	لكل (i = 0; i < nbuf; i++)
		printf("%.*s", (صحيح) mb[i].size, mb[i].ptr);
	أرجع 0;
}

ساكن فراغ show_diff(بنية merge_list *entry)
{
	size_t size;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_باريس = show_outf };

	memset(&xpp, 0, حجم(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, حجم(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &size);
	إذا (!src.ptr)
		size = 0;
	src.size = size;
	dst.ptr = result(entry, &size);
	إذا (!dst.ptr)
		size = 0;
	dst.size = size;
	إذا (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	حرر(src.ptr);
	حرر(dst.ptr);
}

ساكن فراغ show_result_list(بنية merge_list *entry)
{
	printf("%s\n", explanation(entry));
	افعل {
		بنية merge_list *link = entry->link;
		ساكن ثابت حرف *desc[4] = { "result", "base", "our", "their" };
		printf("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} طالما (entry);
}

ساكن فراغ show_result(فراغ)
{
	بنية merge_list *walk;

	walk = merge_result;
	طالما (walk) {
		show_result_list(walk);
		show_diff(walk);
		walk = walk->next;
	}
}

/* An empty entry never compares same, not even to another empty entry */
ساكن صحيح same_entry(بنية name_entry *a, بنية name_entry *b)
{
	أرجع	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

ساكن صحيح both_empty(بنية name_entry *a, بنية name_entry *b)
{
	أرجع is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

ساكن بنية merge_list *create_entry(unsigned stage, unsigned mode, ثابت بنية object_id *oid, ثابت حرف *path)
{
	بنية merge_list *res = xcalloc(1, حجم(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	أرجع res;
}

ساكن حرف *traverse_path(ثابت بنية traverse_info *info, ثابت بنية name_entry *n)
{
	بنية strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	أرجع strbuf_detach(&buf, عدم);
}

ساكن فراغ resolve(ثابت بنية traverse_info *info, بنية name_entry *ours, بنية name_entry *result)
{
	بنية merge_list *orig, *final;
	ثابت حرف *path;

	/* If it's already ours, don't bother showing it */
	إذا (!ours)
		أرجع;

	path = traverse_path(info, result);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, result->mode, &result->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

ساكن فراغ unresolved_directory(ثابت بنية traverse_info *info,
				 بنية name_entry n[3])
{
	بنية repository *r = the_repository;
	حرف *newbase;
	بنية name_entry *p;
	بنية ڤوکوق_desc t[3];
	فراغ *buf0, *buf1, *buf2;

	لكل (p = n; p < n + 3; p++) {
		إذا (p->mode && S_ISDIR(p->mode))
			اكسر;
	}
	إذا (n + 3 <= p)
		أرجع; /* there is no tree here */

	newbase = traverse_path(info, p);

#define ENTRY_OID(e) (((e)->mode && S_ISDIR((e)->mode)) ? &(e)->oid : عدم)
	buf0 = fill_ڤوکوق_descriptor(r, t + 0, ENTRY_OID(n + 0));
	buf1 = fill_ڤوکوق_descriptor(r, t + 1, ENTRY_OID(n + 1));
	buf2 = fill_ڤوکوق_descriptor(r, t + 2, ENTRY_OID(n + 2));
#undef ENTRY_OID

	trivial_merge_trees(t, newbase);

	حرر(buf0);
	حرر(buf1);
	حرر(buf2);
	حرر(newbase);
}


ساكن بنية merge_list *link_entry(unsigned stage, ثابت بنية traverse_info *info, بنية name_entry *n, بنية merge_list *entry)
{
	ثابت حرف *path;
	بنية merge_list *link;

	إذا (!n->mode)
		أرجع entry;
	إذا (entry)
		path = entry->path;
	وإلا
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	أرجع link;
}

ساكن فراغ unresolved(ثابت بنية traverse_info *info, بنية name_entry n[3])
{
	بنية merge_list *entry = عدم;
	صحيح i;
	unsigned dirmask = 0, mask = 0;

	لكل (i = 0; i < 3; i++) {
		mask |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		إذا (!n[i].mode || S_ISDIR(n[i].mode))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	إذا (dirmask == mask)
		أرجع;

	إذا (n[2].mode && !S_ISDIR(n[2].mode))
		entry = link_entry(3, info, n + 2, entry);
	إذا (n[1].mode && !S_ISDIR(n[1].mode))
		entry = link_entry(2, info, n + 1, entry);
	إذا (n[0].mode && !S_ISDIR(n[0].mode))
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
ساكن صحيح threeway_callback(صحيح n UNUSED, unsigned long mask,
			     unsigned long dirmask UNUSED,
			     بنية name_entry *entry, بنية traverse_info *info)
{
	/* Same in both? */
	إذا (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, عدم, entry+1);
		أرجع mask;
	}

	إذا (same_entry(entry+0, entry+1)) {
		إذا (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			أرجع mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	إذا (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, عدم, entry+1);
		أرجع mask;
	}

	unresolved(info, entry);
	أرجع mask;
}

ساكن فراغ trivial_merge_trees(بنية ڤوکوق_desc t[3], ثابت حرف *base)
{
	بنية traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->index, 3, t, &info);
}

ساكن فراغ *get_ڤوکوق_descriptor(بنية repository *r,
				 بنية ڤوکوق_desc *desc,
				 ثابت حرف *rev)
{
	بنية object_id oid;
	فراغ *buf;

	إذا (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_ڤوکوق_descriptor(r, desc, &oid);
	إذا (!buf)
		die("%s is not a tree", rev);
	أرجع buf;
}

ساكن صحيح trivial_merge(ثابت حرف *base,
			 ثابت حرف *branch1,
			 ثابت حرف *branch2)
{
	بنية repository *r = the_repository;
	بنية ڤوکوق_desc t[3];
	فراغ *buf1, *buf2, *buf3;

	buf1 = get_ڤوکوق_descriptor(r, t+0, base);
	buf2 = get_ڤوکوق_descriptor(r, t+1, branch1);
	buf3 = get_ڤوکوق_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	حرر(buf1);
	حرر(buf2);
	حرر(buf3);

	show_result();
	أرجع 0;
}

تعداد mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

بنية merge_ڤوکوق_options {
	صحيح mode;
	صحيح allow_unrelated_histories;
	صحيح show_messages;
	صحيح name_only;
	صحيح use_stdin;
	بنية merge_options merge_options;
};

ساكن صحيح real_merge(بنية merge_ڤوکوق_options *o,
		      ثابت حرف *merge_base,
		      ثابت حرف *branch1, ثابت حرف *branch2,
		      ثابت حرف *prefix)
{
	بنية commit *parent1, *parent2;
	بنية commit_list *merge_bases = عدم;
	بنية merge_result result = { 0 };
	صحيح show_messages = o->show_messages;
	بنية merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	إذا (merge_base) {
		بنية ڤوکوق *base_ڤوکوق, *parent1_ڤوکوق, *parent2_ڤوکوق;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		بنية object_id base_oid, head_oid, merge_oid;

		إذا (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_ڤوکوق = repo_parse_ڤوکوق_indirect(the_repository,
						     &base_oid);
		إذا (!base_ڤوکوق)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		إذا (repo_get_oid_treeish(the_repository, branch1, &head_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_ڤوکوق = repo_parse_ڤوکوق_indirect(the_repository,
							&head_oid);
		إذا (!parent1_ڤوکوق)
			die(_("unable to read tree (%s)"), oid_to_hex(&head_oid));
		إذا (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_ڤوکوق = repo_parse_ڤوکوق_indirect(the_repository,
							&merge_oid);
		إذا (!parent2_ڤوکوق)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_ڤوکوق, parent1_ڤوکوق, parent2_ڤوکوق, &result);
	} وإلا {
		parent1 = get_merge_parent(branch1);
		إذا (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		إذا (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		إذا (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			exit(128);
		إذا (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &result);
		commit_list_free(merge_bases);
	}

	إذا (result.clean < 0)
		die(_("failure to merge"));

	إذا (o->merge_options.mergeability_only)
		اذهب cleanup;

	إذا (show_messages == -1)
		show_messages = !result.clean;

	إذا (o->use_stdin)
		printf("%d%c", result.clean, باريس_termination);
	printf("%s%c", oid_to_hex(&result.ڤوکوق->object.oid), باريس_termination);
	إذا (!result.clean) {
		بنية string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ثابت حرف *last = عدم;

		merge_get_conflicted_files(&result, &conflicted_files);
		لكل (size_t i = 0; i < conflicted_files.nr; i++) {
			ثابت حرف *name = conflicted_files.بارڠن[i].string;
			بنية stage_info *c = conflicted_files.بارڠن[i].util;
			إذا (!o->name_only)
				printf("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			وإلا إذا (last && !strcmp(last, name))
				استمر;
			write_name_quoted_relative(
				name, prefix, stdout, باريس_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	إذا (show_messages) {
		putchar(باريس_termination);
		merge_display_update_messages(&opt, باريس_termination == '\0',
					      &result);
	}
	إذا (o->use_stdin)
		putchar(باريس_termination);

cleanup:
	merge_finalize(&opt, &result);
	clear_merge_options(&opt);
	أرجع !result.clean; /* result.clean < 0 handled above */
}

صحيح cmd_merge_ڤوکوق(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *prefix,
		   بنية repository *repo UNUSED)
{
	بنية merge_ڤوکوق_options o = { .show_messages = -1 };
	بنية strvec xopts = STRVEC_INIT;
	صحيح expected_remaining_argc;
	صحيح original_argc;
	ثابت حرف *merge_base = عدم;
	صحيح ret;
	صحيح quiet = 0;

	ثابت حرف * ثابت merge_ڤوکوق_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		عدم
	};
	بنية option mt_options[] = {
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
		OPT_SET_INT('z', عدم, &باريس_termination,
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
			     merge_ڤوکوق_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !باريس_termination, "-z");

	إذا (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	لكل (size_t x = 0; x < xopts.nr; x++)
		إذا (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	إذا (o.use_stdin) {
		بنية strbuf buf = STRBUF_INIT;

		إذا (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		إذا (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		باريس_termination = '\0';
		طالما (strbuf_getline_lf(&buf, stdin) != EOF) {
			بنية string_list split = STRING_LIST_INIT_NODUP;
			ثابت حرف *ماسوقن_merge_base = عدم;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			إذا (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			إذا (!strcmp(split.بارڠن[1].string, "--")) {
				ماسوقن_merge_base = split.بارڠن[0].string;
			}

			إذا (ماسوقن_merge_base && split.nr == 4) {
				real_merge(&o, ماسوقن_merge_base,
					   split.بارڠن[2].string, split.بارڠن[3].string,
					   prefix);
			} وإلا إذا (!ماسوقن_merge_base && split.nr == 2) {
				real_merge(&o, عدم,
					   split.بارڠن[0].string, split.بارڠن[1].string,
					   prefix);
			} وإلا {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&split, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		اذهب out;
	}

	/* Figure out which mode to use */
	بدل (o.mode) {
	افتراضي:
		BUG("unexpected command mode %d", o.mode);
	حالة MODE_UNKNOWN:
		بدل (argc) {
		افتراضي:
			usage_with_options(merge_ڤوکوق_usage, mt_options);
		حالة 2:
			o.mode = MODE_REAL;
			اكسر;
		حالة 3:
			o.mode = MODE_TRIVIAL;
			اكسر;
		}
		expected_remaining_argc = argc;
		اكسر;
	حالة MODE_REAL:
		expected_remaining_argc = 2;
		اكسر;
	حالة MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		اكسر;
	}
	إذا (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	إذا (argc != expected_remaining_argc)
		usage_with_options(merge_ڤوکوق_usage, mt_options);

	repo_config(the_repository, git_default_config, عدم);

	/* Do the relevant type of merge */
	إذا (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	وإلا
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	أرجع ret;
}
