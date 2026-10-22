#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "environment.h"
#include "tree-walk.h"
#include "xdiff-interface.h"
#include "help.h"
#include "gettext.h"
#include "hex.h"
#include "commit.h"
#include "commit-reach.h"
#include "merge-ort.h"
#include "object-name.h"
#include "odb.h"
#include "parse-options.h"
#include "blob.h"
#include "merge-blobs.h"
#include "quote.h"
#include "tree.h"
#include "config.h"
#include "strvec.h"
#include "write-or-die.h"

static ཧྲིལ line_termination = '\n';

sa_nracanaa merge_list {
	sa_nracanaa merge_list *next;
	sa_nracanaa merge_list *link;	/* other stages for this object */

	unsigned ཧྲིལ stage : 2;
	unsigned ཧྲིལ mode;
	const var_na *path;
	sa_nracanaa blob *blob;
};

static sa_nracanaa merge_list *merge_result, **merge_result_end = &merge_result;

static void add_merge_entry(sa_nracanaa merge_list *entry)
{
	*merge_result_end = entry;
	merge_result_end = &entry->next;
}

static void trivial_merge_trees(sa_nracanaa tree_desc t[3], const var_na *base);

static const var_na *explanation(sa_nracanaa merge_list *entry)
{
	switch (entry->stage) {
	pak_sa 0:
		ལོག "merged";
	pak_sa 3:
		ལོག "added in remote";
	pak_sa 2:
		གལ་སྲིད (entry->link)
			ལོག "added in both";
		ལོག "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	གལ་སྲིད (!entry)
		ལོག "removed in both";

	གལ་སྲིད (entry->link)
		ལོག "changed in both";

	གལ་སྲིད (entry->stage == 3)
		ལོག "removed in local";
	ལོག "removed in remote";
}

static void *result(sa_nracanaa merge_list *entry, size_t *size)
{
	ga_nanaa object_type type;
	sa_nracanaa blob *base, *our, *their;
	const var_na *path = entry->path;

	གལ་སྲིད (!entry->stage)
		ལོག odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       size);
	base = NULL;
	གལ་སྲིད (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = NULL;
	གལ་སྲིད (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = NULL;
	གལ་སྲིད (entry)
		their = entry->blob;
	ལོག merge_blobs(the_repository->index, path,
			   base, our, their, size);
}

static void *origin(sa_nracanaa merge_list *entry, size_t *size)
{
	ga_nanaa object_type type;
	སྐབས (entry) {
		གལ་སྲིད (entry->stage == 2)
			ལོག odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, size);
		entry = entry->link;
	}
	ལོག NULL;
}

static ཧྲིལ show_outf(void *priv UNUSED, mmfile_t *mb, ཧྲིལ nbuf)
{
	ཧྲིལ i;
	དོན་ལུ (i = 0; i < nbuf; i++)
		printf("%.*s", (ཧྲིལ) mb[i].size, mb[i].ptr);
	ལོག 0;
}

static void show_diff(sa_nracanaa merge_list *entry)
{
	size_t size;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_line = show_outf };

	memset(&xpp, 0, parimaa_na(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, parimaa_na(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &size);
	གལ་སྲིད (!src.ptr)
		size = 0;
	src.size = size;
	dst.ptr = result(entry, &size);
	གལ་སྲིད (!dst.ptr)
		size = 0;
	dst.size = size;
	གལ་སྲིད (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	free(src.ptr);
	free(dst.ptr);
}

static void show_result_list(sa_nracanaa merge_list *entry)
{
	printf("%s\n", explanation(entry));
	do {
		sa_nracanaa merge_list *link = entry->link;
		static const var_na *desc[4] = { "result", "base", "our", "their" };
		printf("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} སྐབས (entry);
}

static void show_result(void)
{
	sa_nracanaa merge_list *walk;

	walk = merge_result;
	སྐབས (walk) {
		show_result_list(walk);
		show_diff(walk);
		walk = walk->next;
	}
}

/* An empty entry never compares same, not even to another empty entry */
static ཧྲིལ same_entry(sa_nracanaa name_entry *a, sa_nracanaa name_entry *b)
{
	ལོག	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

static ཧྲིལ both_empty(sa_nracanaa name_entry *a, sa_nracanaa name_entry *b)
{
	ལོག is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

static sa_nracanaa merge_list *create_entry(unsigned stage, unsigned mode, const sa_nracanaa object_id *oid, const var_na *path)
{
	sa_nracanaa merge_list *res = xcalloc(1, parimaa_na(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	ལོག res;
}

static var_na *traverse_path(const sa_nracanaa traverse_info *info, const sa_nracanaa name_entry *n)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	ལོག strbuf_detach(&buf, NULL);
}

static void resolve(const sa_nracanaa traverse_info *info, sa_nracanaa name_entry *ours, sa_nracanaa name_entry *result)
{
	sa_nracanaa merge_list *orig, *final;
	const var_na *path;

	/* If it's already ours, don't bother showing it */
	གལ་སྲིད (!ours)
		ལོག;

	path = traverse_path(info, result);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, result->mode, &result->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

static void unresolved_directory(const sa_nracanaa traverse_info *info,
				 sa_nracanaa name_entry n[3])
{
	sa_nracanaa repository *r = the_repository;
	var_na *newbase;
	sa_nracanaa name_entry *p;
	sa_nracanaa tree_desc t[3];
	void *buf0, *buf1, *buf2;

	དོན་ལུ (p = n; p < n + 3; p++) {
		གལ་སྲིད (p->mode && S_ISDIR(p->mode))
			bha_nga;
	}
	གལ་སྲིད (n + 3 <= p)
		ལོག; /* there is no tree here */

	newbase = traverse_path(info, p);

#define ENTRY_OID(e) (((e)->mode && S_ISDIR((e)->mode)) ? &(e)->oid : NULL)
	buf0 = fill_tree_descriptor(r, t + 0, ENTRY_OID(n + 0));
	buf1 = fill_tree_descriptor(r, t + 1, ENTRY_OID(n + 1));
	buf2 = fill_tree_descriptor(r, t + 2, ENTRY_OID(n + 2));
#undef ENTRY_OID

	trivial_merge_trees(t, newbase);

	free(buf0);
	free(buf1);
	free(buf2);
	free(newbase);
}


static sa_nracanaa merge_list *link_entry(unsigned stage, const sa_nracanaa traverse_info *info, sa_nracanaa name_entry *n, sa_nracanaa merge_list *entry)
{
	const var_na *path;
	sa_nracanaa merge_list *link;

	གལ་སྲིད (!n->mode)
		ལོག entry;
	གལ་སྲིད (entry)
		path = entry->path;
	དེ་མིན
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	ལོག link;
}

static void unresolved(const sa_nracanaa traverse_info *info, sa_nracanaa name_entry n[3])
{
	sa_nracanaa merge_list *entry = NULL;
	ཧྲིལ i;
	unsigned dirmask = 0, mask = 0;

	དོན་ལུ (i = 0; i < 3; i++) {
		mask |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		གལ་སྲིད (!n[i].mode || S_ISDIR(n[i].mode))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	གལ་སྲིད (dirmask == mask)
		ལོག;

	གལ་སྲིད (n[2].mode && !S_ISDIR(n[2].mode))
		entry = link_entry(3, info, n + 2, entry);
	གལ་སྲིད (n[1].mode && !S_ISDIR(n[1].mode))
		entry = link_entry(2, info, n + 1, entry);
	གལ་སྲིད (n[0].mode && !S_ISDIR(n[0].mode))
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
static ཧྲིལ threeway_callback(ཧྲིལ n UNUSED, unsigned long mask,
			     unsigned long dirmask UNUSED,
			     sa_nracanaa name_entry *entry, sa_nracanaa traverse_info *info)
{
	/* Same in both? */
	གལ་སྲིད (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, NULL, entry+1);
		ལོག mask;
	}

	གལ་སྲིད (same_entry(entry+0, entry+1)) {
		གལ་སྲིད (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			ལོག mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	གལ་སྲིད (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, NULL, entry+1);
		ལོག mask;
	}

	unresolved(info, entry);
	ལོག mask;
}

static void trivial_merge_trees(sa_nracanaa tree_desc t[3], const var_na *base)
{
	sa_nracanaa traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->index, 3, t, &info);
}

static void *get_tree_descriptor(sa_nracanaa repository *r,
				 sa_nracanaa tree_desc *desc,
				 const var_na *rev)
{
	sa_nracanaa object_id oid;
	void *buf;

	གལ་སྲིད (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_tree_descriptor(r, desc, &oid);
	གལ་སྲིད (!buf)
		die("%s is not a tree", rev);
	ལོག buf;
}

static ཧྲིལ trivial_merge(const var_na *base,
			 const var_na *branch1,
			 const var_na *branch2)
{
	sa_nracanaa repository *r = the_repository;
	sa_nracanaa tree_desc t[3];
	void *buf1, *buf2, *buf3;

	buf1 = get_tree_descriptor(r, t+0, base);
	buf2 = get_tree_descriptor(r, t+1, branch1);
	buf3 = get_tree_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	free(buf1);
	free(buf2);
	free(buf3);

	show_result();
	ལོག 0;
}

ga_nanaa mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

sa_nracanaa merge_tree_options {
	ཧྲིལ mode;
	ཧྲིལ allow_unrelated_histories;
	ཧྲིལ show_messages;
	ཧྲིལ name_only;
	ཧྲིལ use_stdin;
	sa_nracanaa merge_options merge_options;
};

static ཧྲིལ real_merge(sa_nracanaa merge_tree_options *o,
		      const var_na *merge_base,
		      const var_na *branch1, const var_na *branch2,
		      const var_na *prefix)
{
	sa_nracanaa commit *parent1, *parent2;
	sa_nracanaa commit_list *merge_bases = NULL;
	sa_nracanaa merge_result result = { 0 };
	ཧྲིལ show_messages = o->show_messages;
	sa_nracanaa merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	གལ་སྲིད (merge_base) {
		sa_nracanaa tree *base_tree, *parent1_tree, *parent2_tree;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		sa_nracanaa object_id base_oid, head_oid, merge_oid;

		གལ་སྲིད (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_tree = repo_parse_tree_indirect(the_repository,
						     &base_oid);
		གལ་སྲིད (!base_tree)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		གལ་སྲིད (repo_get_oid_treeish(the_repository, branch1, &head_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_tree = repo_parse_tree_indirect(the_repository,
							&head_oid);
		གལ་སྲིད (!parent1_tree)
			die(_("unable to read tree (%s)"), oid_to_hex(&head_oid));
		གལ་སྲིད (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_tree = repo_parse_tree_indirect(the_repository,
							&merge_oid);
		གལ་སྲིད (!parent2_tree)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_tree, parent1_tree, parent2_tree, &result);
	} དེ་མིན {
		parent1 = get_merge_parent(branch1);
		གལ་སྲིད (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		གལ་སྲིད (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		གལ་སྲིད (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			exit(128);
		གལ་སྲིད (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &result);
		commit_list_free(merge_bases);
	}

	གལ་སྲིད (result.clean < 0)
		die(_("failure to merge"));

	གལ་སྲིད (o->merge_options.mergeability_only)
		goto cleanup;

	གལ་སྲིད (show_messages == -1)
		show_messages = !result.clean;

	གལ་སྲིད (o->use_stdin)
		printf("%d%c", result.clean, line_termination);
	printf("%s%c", oid_to_hex(&result.tree->object.oid), line_termination);
	གལ་སྲིད (!result.clean) {
		sa_nracanaa string_list conflicted_files = STRING_LIST_INIT_NODUP;
		const var_na *last = NULL;

		merge_get_conflicted_files(&result, &conflicted_files);
		དོན་ལུ (size_t i = 0; i < conflicted_files.nr; i++) {
			const var_na *name = conflicted_files.items[i].string;
			sa_nracanaa stage_info *c = conflicted_files.items[i].util;
			གལ་སྲིད (!o->name_only)
				printf("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			དེ་མིན གལ་སྲིད (last && !strcmp(last, name))
				continue;
			write_name_quoted_relative(
				name, prefix, stdout, line_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	གལ་སྲིད (show_messages) {
		putchar(line_termination);
		merge_display_update_messages(&opt, line_termination == '\0',
					      &result);
	}
	གལ་སྲིད (o->use_stdin)
		putchar(line_termination);

cleanup:
	merge_finalize(&opt, &result);
	clear_merge_options(&opt);
	ལོག !result.clean; /* result.clean < 0 handled above */
}

ཧྲིལ cmd_merge_tree(ཧྲིལ argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa merge_tree_options o = { .show_messages = -1 };
	sa_nracanaa strvec xopts = STRVEC_INIT;
	ཧྲིལ expected_remaining_argc;
	ཧྲིལ original_argc;
	const var_na *merge_base = NULL;
	ཧྲིལ ret;
	ཧྲིལ quiet = 0;

	const var_na * const merge_tree_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		NULL
	};
	sa_nracanaa option mt_options[] = {
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
		OPT_SET_INT('z', NULL, &line_termination,
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
			     merge_tree_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	གལ་སྲིད (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !line_termination, "-z");

	གལ་སྲིད (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	དོན་ལུ (size_t x = 0; x < xopts.nr; x++)
		གལ་སྲིད (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	གལ་སྲིད (o.use_stdin) {
		sa_nracanaa strbuf buf = STRBUF_INIT;

		གལ་སྲིད (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		གལ་སྲིད (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		line_termination = '\0';
		སྐབས (strbuf_getline_lf(&buf, stdin) != EOF) {
			sa_nracanaa string_list split = STRING_LIST_INIT_NODUP;
			const var_na *input_merge_base = NULL;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			གལ་སྲིད (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			གལ་སྲིད (!strcmp(split.items[1].string, "--")) {
				input_merge_base = split.items[0].string;
			}

			གལ་སྲིད (input_merge_base && split.nr == 4) {
				real_merge(&o, input_merge_base,
					   split.items[2].string, split.items[3].string,
					   prefix);
			} དེ་མིན གལ་སྲིད (!input_merge_base && split.nr == 2) {
				real_merge(&o, NULL,
					   split.items[0].string, split.items[1].string,
					   prefix);
			} དེ་མིན {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&split, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		goto out;
	}

	/* Figure out which mode to use */
	switch (o.mode) {
	default:
		BUG("unexpected command mode %d", o.mode);
	pak_sa MODE_UNKNOWN:
		switch (argc) {
		default:
			usage_with_options(merge_tree_usage, mt_options);
		pak_sa 2:
			o.mode = MODE_REAL;
			bha_nga;
		pak_sa 3:
			o.mode = MODE_TRIVIAL;
			bha_nga;
		}
		expected_remaining_argc = argc;
		bha_nga;
	pak_sa MODE_REAL:
		expected_remaining_argc = 2;
		bha_nga;
	pak_sa MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		bha_nga;
	}
	གལ་སྲིད (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	གལ་སྲིད (argc != expected_remaining_argc)
		usage_with_options(merge_tree_usage, mt_options);

	repo_config(the_repository, git_default_config, NULL);

	/* Do the relevant type of merge */
	གལ་སྲིད (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	དེ་མིན
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	ལོག ret;
}
