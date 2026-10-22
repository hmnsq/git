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

static 정수 line_termination = '\n';

struct merge_list {
	struct merge_list *next;
	struct merge_list *link;	/* other stages for this object */

	unsigned 정수 stage : 2;
	unsigned 정수 mode;
	const char *path;
	struct blob *blob;
};

static struct merge_list *merge_result, **merge_result_end = &merge_result;

static void add_merge_entry(struct merge_list *entry)
{
	*merge_result_end = entry;
	merge_result_end = &entry->next;
}

static void trivial_merge_trees(struct tree_desc t[3], const char *base);

static const char *explanation(struct merge_list *entry)
{
	switch (entry->stage) {
	case 0:
		반환 "merged";
	case 3:
		반환 "added in remote";
	case 2:
		만약 (entry->link)
			반환 "added in both";
		반환 "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	만약 (!entry)
		반환 "removed in both";

	만약 (entry->link)
		반환 "changed in both";

	만약 (entry->stage == 3)
		반환 "removed in local";
	반환 "removed in remote";
}

static void *result(struct merge_list *entry, size_t *size)
{
	enum object_type type;
	struct blob *base, *our, *their;
	const char *path = entry->path;

	만약 (!entry->stage)
		반환 odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       size);
	base = NULL;
	만약 (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = NULL;
	만약 (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = NULL;
	만약 (entry)
		their = entry->blob;
	반환 merge_blobs(the_repository->index, path,
			   base, our, their, size);
}

static void *origin(struct merge_list *entry, size_t *size)
{
	enum object_type type;
	동안 (entry) {
		만약 (entry->stage == 2)
			반환 odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, size);
		entry = entry->link;
	}
	반환 NULL;
}

static 정수 show_outf(void *priv UNUSED, mmfile_t *mb, 정수 nbuf)
{
	정수 i;
	위해 (i = 0; i < nbuf; i++)
		printf("%.*s", (정수) mb[i].size, mb[i].ptr);
	반환 0;
}

static void show_diff(struct merge_list *entry)
{
	size_t size;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_line = show_outf };

	memset(&xpp, 0, sizeof(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, sizeof(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &size);
	만약 (!src.ptr)
		size = 0;
	src.size = size;
	dst.ptr = result(entry, &size);
	만약 (!dst.ptr)
		size = 0;
	dst.size = size;
	만약 (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	free(src.ptr);
	free(dst.ptr);
}

static void show_result_list(struct merge_list *entry)
{
	printf("%s\n", explanation(entry));
	do {
		struct merge_list *link = entry->link;
		static const char *desc[4] = { "result", "base", "our", "their" };
		printf("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} 동안 (entry);
}

static void show_result(void)
{
	struct merge_list *walk;

	walk = merge_result;
	동안 (walk) {
		show_result_list(walk);
		show_diff(walk);
		walk = walk->next;
	}
}

/* An empty entry never compares same, not even to another empty entry */
static 정수 same_entry(struct name_entry *a, struct name_entry *b)
{
	반환	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

static 정수 both_empty(struct name_entry *a, struct name_entry *b)
{
	반환 is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

static struct merge_list *create_entry(unsigned stage, unsigned mode, const struct object_id *oid, const char *path)
{
	struct merge_list *res = xcalloc(1, sizeof(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	반환 res;
}

static char *traverse_path(const struct traverse_info *info, const struct name_entry *n)
{
	struct strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	반환 strbuf_detach(&buf, NULL);
}

static void resolve(const struct traverse_info *info, struct name_entry *ours, struct name_entry *result)
{
	struct merge_list *orig, *final;
	const char *path;

	/* If it's already ours, don't bother showing it */
	만약 (!ours)
		반환;

	path = traverse_path(info, result);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, result->mode, &result->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

static void unresolved_directory(const struct traverse_info *info,
				 struct name_entry n[3])
{
	struct repository *r = the_repository;
	char *newbase;
	struct name_entry *p;
	struct tree_desc t[3];
	void *buf0, *buf1, *buf2;

	위해 (p = n; p < n + 3; p++) {
		만약 (p->mode && S_ISDIR(p->mode))
			break;
	}
	만약 (n + 3 <= p)
		반환; /* there is no tree here */

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


static struct merge_list *link_entry(unsigned stage, const struct traverse_info *info, struct name_entry *n, struct merge_list *entry)
{
	const char *path;
	struct merge_list *link;

	만약 (!n->mode)
		반환 entry;
	만약 (entry)
		path = entry->path;
	아니면
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	반환 link;
}

static void unresolved(const struct traverse_info *info, struct name_entry n[3])
{
	struct merge_list *entry = NULL;
	정수 i;
	unsigned dirmask = 0, mask = 0;

	위해 (i = 0; i < 3; i++) {
		mask |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		만약 (!n[i].mode || S_ISDIR(n[i].mode))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	만약 (dirmask == mask)
		반환;

	만약 (n[2].mode && !S_ISDIR(n[2].mode))
		entry = link_entry(3, info, n + 2, entry);
	만약 (n[1].mode && !S_ISDIR(n[1].mode))
		entry = link_entry(2, info, n + 1, entry);
	만약 (n[0].mode && !S_ISDIR(n[0].mode))
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
static 정수 threeway_callback(정수 n UNUSED, unsigned long mask,
			     unsigned long dirmask UNUSED,
			     struct name_entry *entry, struct traverse_info *info)
{
	/* Same in both? */
	만약 (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, NULL, entry+1);
		반환 mask;
	}

	만약 (same_entry(entry+0, entry+1)) {
		만약 (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			반환 mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	만약 (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, NULL, entry+1);
		반환 mask;
	}

	unresolved(info, entry);
	반환 mask;
}

static void trivial_merge_trees(struct tree_desc t[3], const char *base)
{
	struct traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->index, 3, t, &info);
}

static void *get_tree_descriptor(struct repository *r,
				 struct tree_desc *desc,
				 const char *rev)
{
	struct object_id oid;
	void *buf;

	만약 (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_tree_descriptor(r, desc, &oid);
	만약 (!buf)
		die("%s is not a tree", rev);
	반환 buf;
}

static 정수 trivial_merge(const char *base,
			 const char *branch1,
			 const char *branch2)
{
	struct repository *r = the_repository;
	struct tree_desc t[3];
	void *buf1, *buf2, *buf3;

	buf1 = get_tree_descriptor(r, t+0, base);
	buf2 = get_tree_descriptor(r, t+1, branch1);
	buf3 = get_tree_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	free(buf1);
	free(buf2);
	free(buf3);

	show_result();
	반환 0;
}

enum mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

struct merge_tree_options {
	정수 mode;
	정수 allow_unrelated_histories;
	정수 show_messages;
	정수 name_only;
	정수 use_stdin;
	struct merge_options merge_options;
};

static 정수 real_merge(struct merge_tree_options *o,
		      const char *merge_base,
		      const char *branch1, const char *branch2,
		      const char *prefix)
{
	struct commit *parent1, *parent2;
	struct commit_list *merge_bases = NULL;
	struct merge_result result = { 0 };
	정수 show_messages = o->show_messages;
	struct merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	만약 (merge_base) {
		struct tree *base_tree, *parent1_tree, *parent2_tree;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		struct object_id base_oid, head_oid, merge_oid;

		만약 (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_tree = repo_parse_tree_indirect(the_repository,
						     &base_oid);
		만약 (!base_tree)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		만약 (repo_get_oid_treeish(the_repository, branch1, &head_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_tree = repo_parse_tree_indirect(the_repository,
							&head_oid);
		만약 (!parent1_tree)
			die(_("unable to read tree (%s)"), oid_to_hex(&head_oid));
		만약 (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_tree = repo_parse_tree_indirect(the_repository,
							&merge_oid);
		만약 (!parent2_tree)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_tree, parent1_tree, parent2_tree, &result);
	} 아니면 {
		parent1 = get_merge_parent(branch1);
		만약 (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		만약 (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		만약 (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			exit(128);
		만약 (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &result);
		commit_list_free(merge_bases);
	}

	만약 (result.clean < 0)
		die(_("failure to merge"));

	만약 (o->merge_options.mergeability_only)
		goto cleanup;

	만약 (show_messages == -1)
		show_messages = !result.clean;

	만약 (o->use_stdin)
		printf("%d%c", result.clean, line_termination);
	printf("%s%c", oid_to_hex(&result.tree->object.oid), line_termination);
	만약 (!result.clean) {
		struct string_list conflicted_files = STRING_LIST_INIT_NODUP;
		const char *last = NULL;

		merge_get_conflicted_files(&result, &conflicted_files);
		위해 (size_t i = 0; i < conflicted_files.nr; i++) {
			const char *name = conflicted_files.items[i].string;
			struct stage_info *c = conflicted_files.items[i].util;
			만약 (!o->name_only)
				printf("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			아니면 만약 (last && !strcmp(last, name))
				continue;
			write_name_quoted_relative(
				name, prefix, stdout, line_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	만약 (show_messages) {
		putchar(line_termination);
		merge_display_update_messages(&opt, line_termination == '\0',
					      &result);
	}
	만약 (o->use_stdin)
		putchar(line_termination);

cleanup:
	merge_finalize(&opt, &result);
	clear_merge_options(&opt);
	반환 !result.clean; /* result.clean < 0 handled above */
}

정수 cmd_merge_tree(정수 argc,
		   const char **argv,
		   const char *prefix,
		   struct repository *repo UNUSED)
{
	struct merge_tree_options o = { .show_messages = -1 };
	struct strvec xopts = STRVEC_INIT;
	정수 expected_remaining_argc;
	정수 original_argc;
	const char *merge_base = NULL;
	정수 ret;
	정수 quiet = 0;

	const char * const merge_tree_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		NULL
	};
	struct option mt_options[] = {
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

	만약 (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !line_termination, "-z");

	만약 (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	위해 (size_t x = 0; x < xopts.nr; x++)
		만약 (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	만약 (o.use_stdin) {
		struct strbuf buf = STRBUF_INIT;

		만약 (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		만약 (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		line_termination = '\0';
		동안 (strbuf_getline_lf(&buf, stdin) != EOF) {
			struct string_list split = STRING_LIST_INIT_NODUP;
			const char *input_merge_base = NULL;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			만약 (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			만약 (!strcmp(split.items[1].string, "--")) {
				input_merge_base = split.items[0].string;
			}

			만약 (input_merge_base && split.nr == 4) {
				real_merge(&o, input_merge_base,
					   split.items[2].string, split.items[3].string,
					   prefix);
			} 아니면 만약 (!input_merge_base && split.nr == 2) {
				real_merge(&o, NULL,
					   split.items[0].string, split.items[1].string,
					   prefix);
			} 아니면 {
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
	case MODE_UNKNOWN:
		switch (argc) {
		default:
			usage_with_options(merge_tree_usage, mt_options);
		case 2:
			o.mode = MODE_REAL;
			break;
		case 3:
			o.mode = MODE_TRIVIAL;
			break;
		}
		expected_remaining_argc = argc;
		break;
	case MODE_REAL:
		expected_remaining_argc = 2;
		break;
	case MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		break;
	}
	만약 (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	만약 (argc != expected_remaining_argc)
		usage_with_options(merge_tree_usage, mt_options);

	repo_config(the_repository, git_default_config, NULL);

	/* Do the relevant type of merge */
	만약 (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	아니면
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	반환 ret;
}
