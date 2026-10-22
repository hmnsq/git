#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "commit.h"
#include "diff.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "revision.h"
#include "list-objects.h"
#include "list-objects-filter-options.h"
#include "object.h"
#include "object-name.h"
#include "object-file.h"
#include "odb.h"
#include "pack-bitmap.h"
#include "parse-options.h"
#include "log-tree.h"
#include "graph.h"
#include "bisect.h"
#include "progress.h"
#include "reflog-walk.h"
#include "oidset.h"
#include "oidmap.h"
#include "packfile.h"
#include "commit-reach.h"
#include "quote.h"
#include "strbuf.h"

struct rev_list_info {
	struct rev_info *revs;
	целое flags;
	целое show_timestamp;
	целое hdr_termination;
	const char *header_prefix;
};

static const char rev_list_usage[] =
"git rev-list [<options>] <commit>... [--] [<path>...]\n"
"\n"
"  limiting output:\n"
"    --max-count=<n>\n"
"    --max-age=<epoch>\n"
"    --min-age=<epoch>\n"
"    --sparse\n"
"    --no-merges\n"
"    --min-parents=<n>\n"
"    --no-min-parents\n"
"    --max-parents=<n>\n"
"    --no-max-parents\n"
"    --remove-empty\n"
"    --all\n"
"    --branches\n"
"    --tags\n"
"    --remotes\n"
"    --stdin\n"
"    --exclude-hidden=[fetch|receive|uploadpack]\n"
"    --quiet\n"
"  ordering output:\n"
"    --topo-order\n"
"    --date-order\n"
"    --reverse\n"
"  formatting output:\n"
"    --parents\n"
"    --children\n"
"    --objects | --objects-edge\n"
"    --disk-usage[=human]\n"
"    --unpacked\n"
"    --header | --pretty\n"
"    --[no-]object-names\n"
"    --abbrev=<n> | --no-abbrev\n"
"    --abbrev-commit\n"
"    --left-right\n"
"    --count\n"
"    -z\n"
"  special purpose:\n"
"    --bisect\n"
"    --bisect-vars\n"
"    --bisect-all"
;

static struct progress *progress;
static unsigned progress_counter;

static struct oidset omitted_objects;
static целое arg_print_omitted; /* print objects omitted by filter */

struct missing_objects_map_entry {
	struct oidmap_entry entry;
	char *path;
	unsigned type;
};

static void missing_objects_map_entry_free(void *e)
{
	struct missing_objects_map_entry *entry =
		container_of(e, struct missing_objects_map_entry, entry);

	free(entry->path);
	free(entry);
}

static struct oidmap missing_objects;
enum missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
static enum missing_action arg_missing_action;
static целое arg_missing_only;

static inline целое should_collect_missing(void)
{
	вернуть arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
static целое arg_show_object_names = 1;

#define DEFAULT_OIDSET_SIZE     (16*1024)

static char line_term = '\n';
static char info_term = ' ';

static целое show_disk_usage;
static off_t total_disk_usage;
static целое human_readable;

static off_t get_object_disk_usage(struct object *obj)
{
	off_t size;
	struct object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &size;
	если (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_to_hex(&obj->oid));
	вернуть size;
}

static void add_missing_object_entry(struct object_id *oid, const char *path,
				     unsigned type)
{
	struct missing_objects_map_entry *entry;

	если (oidmap_get(&missing_objects, oid))
		вернуть;

	CALLOC_ARRAY(entry, 1);
	entry->entry.oid = *oid;
	entry->type = type;
	если (path)
		entry->path = xstrdup(path);
	oidmap_put(&missing_objects, entry);
}

static void print_missing_object(struct missing_objects_map_entry *entry,
				 целое print_missing_info)
{
	struct strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	если (arg_missing_only && line_term)
		printf("%s", oid_to_hex(&entry->entry.oid));
	иначе если (line_term)
		printf("?%s", oid_to_hex(&entry->entry.oid));
	иначе
		printf("%s%cmissing=yes", oid_to_hex(&entry->entry.oid),
		       info_term);

	если (!print_missing_info) {
		putchar(line_term);
		вернуть;
	}

	если (entry->path && *entry->path) {
		strbuf_addf(&sb, "%cpath=", info_term);

		если (line_term) {
			struct strbuf path = STRBUF_INIT;

			quote_path(entry->path, NULL, &path, QUOTE_PATH_QUOTE_SP);
			strbuf_addbuf(&sb, &path);

			strbuf_release(&path);
		} иначе {
			strbuf_addstr(&sb, entry->path);
		}
	}
	если (entry->type)
		strbuf_addf(&sb, "%ctype=%s", info_term, type_name(entry->type));

	fwrite(sb.buf, sizeof(char), sb.len, stdout);
	putchar(line_term);

	strbuf_release(&sb);
}

static inline void finish_object__ma(struct object *obj, const char *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	switch (arg_missing_action) {
	case MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		вернуть;

	case MA_ALLOW_ANY:
		вернуть;

	case MA_PRINT:
	case MA_PRINT_INFO:
		add_missing_object_entry(&obj->oid, name, obj->type);
		вернуть;

	case MA_ALLOW_PROMISOR:
		если (is_promisor_object(the_repository, &obj->oid))
			вернуть;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		вернуть;

	default:
		BUG("unhandled missing_action");
		вернуть;
	}
}

static void finish_commit(struct commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = NULL;
	free_commit_buffer(the_repository->parsed_objects,
			   commit);
}

static void show_commit(struct commit *commit, void *data)
{
	struct rev_list_info *info = data;
	struct rev_info *revs = info->revs;

	display_progress(progress, ++progress_counter);

	если (revs->do_not_die_on_missing_objects &&
	    oidset_contains(&revs->missing_commits, &commit->object.oid)) {
		finish_object__ma(&commit->object, NULL);
		вернуть;
	}

	если (arg_missing_only) {
		finish_commit(commit);
		вернуть;
	}

	если (show_disk_usage)
		total_disk_usage += get_object_disk_usage(&commit->object);

	если (info->flags & REV_LIST_QUIET) {
		finish_commit(commit);
		вернуть;
	}

	graph_show_commit(revs->graph);

	если (revs->count) {
		если (commit->object.flags & PATCHSAME)
			revs->count_same++;
		иначе если (commit->object.flags & SYMMETRIC_LEFT)
			revs->count_left++;
		иначе
			revs->count_right++;
		finish_commit(commit);
		вернуть;
	}

	если (info->show_timestamp)
		printf("%"PRItime" ", commit->date);
	если (info->header_prefix)
		fputs(info->header_prefix, stdout);

	если (revs->include_header) {
		если (!revs->graph && line_term)
			fputs(get_revision_mark(revs, commit), stdout);
		если (revs->abbrev_commit && revs->abbrev)
			fputs(repo_find_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		иначе
			fputs(oid_to_hex(&commit->object.oid), stdout);

		если (!line_term) {
			если (commit->object.flags & BOUNDARY)
				printf("%cboundary=yes", info_term);
		}
	}
	если (revs->print_parents) {
		struct commit_list *parents = commit->parents;
		пока (parents) {
			printf(" %s", oid_to_hex(&parents->item->object.oid));
			parents = parents->next;
		}
	}
	если (revs->children.name) {
		struct commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		пока (children) {
			printf(" %s", oid_to_hex(&children->item->object.oid));
			children = children->next;
		}
	}
	show_decorations(revs, commit);
	если (revs->commit_format == CMIT_FMT_ONELINE)
		putchar(' ');
	иначе если (revs->include_header)
		putchar(line_term);

	если (revs->verbose_header) {
		struct strbuf buf = STRBUF_INIT;
		struct pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_mode = revs->date_mode;
		ctx.date_mode_explicit = revs->date_mode_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		если (buf.len) {
			если (revs->commit_format != CMIT_FMT_ONELINE)
				graph_show_oneline(revs->graph);

			graph_show_commit_msg(revs->graph, stdout, &buf);

			/*
			 * Add a newline after the commit message.
			 *
			 * Usually, this newline produces a blank
			 * padding line between entries, in which case
			 * we need to add graph padding on this line.
			 *
			 * However, the commit message may not end in a
			 * newline.  In this case the newline simply
			 * ends the last line of the commit message,
			 * and we don't need any graph output.  (This
			 * always happens with CMIT_FMT_ONELINE, and it
			 * happens with CMIT_FMT_USERFORMAT when the
			 * format doesn't explicitly end in a newline.)
			 */
			если (buf.len && buf.buf[buf.len - 1] == '\n')
				graph_show_padding(revs->graph);
			putchar(info->hdr_termination);
		} иначе {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			если (graph_show_remainder(revs->graph))
				putchar('\n');
			если (revs->commit_format == CMIT_FMT_ONELINE)
				putchar('\n');
		}
		strbuf_release(&buf);
	} иначе {
		если (graph_show_remainder(revs->graph))
			putchar('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	finish_commit(commit);
}

static целое finish_object(struct object *obj, const char *name, void *cb_data)
{
	struct rev_list_info *info = cb_data;
	если (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, NULL, 0) < 0) {
		finish_object__ma(obj, name);
		вернуть 1;
	}
	если (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	вернуть 0;
}

static void show_object(struct object *obj, const char *name, void *cb_data)
{
	struct rev_list_info *info = cb_data;
	struct rev_info *revs = info->revs;

	если (finish_object(obj, name, cb_data))
		вернуть;
	display_progress(progress, ++progress_counter);
	если (arg_missing_only)
		вернуть;
	если (show_disk_usage)
		total_disk_usage += get_object_disk_usage(obj);
	если (info->flags & REV_LIST_QUIET)
		вернуть;

	если (revs->count) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->count_right++;
		вернуть;
	}

	printf("%s", oid_to_hex(&obj->oid));

	если (arg_show_object_names) {
		если (line_term) {
			putchar(info_term);
			для (const char *p = name; *p && *p != '\n'; p++)
				putchar(*p);
		} иначе если (*name) {
			printf("%cpath=%s", info_term, name);
		}
	}

	putchar(line_term);
}

static void show_edge(struct commit *commit)
{
	printf("-%s\n", oid_to_hex(&commit->object.oid));
}

static void print_var_str(const char *var, const char *val)
{
	printf("%s='%s'\n", var, val);
}

static void print_var_int(const char *var, целое val)
{
	printf("%s=%d\n", var, val);
}

static целое show_bisect_vars(struct rev_list_info *info, целое reaches, целое all)
{
	целое cnt, flags = info->flags;
	char hex[GIT_MAX_HEXSZ + 1] = "";
	struct commit_list *tried;
	struct rev_info *revs = info->revs;

	если (!revs->commits)
		вернуть 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       NULL, NULL);

	/*
	 * revs->commits can reach "reaches" commits among
	 * "all" commits.  If it is good, then there are
	 * (all-reaches) commits left to be bisected.
	 * On the other hand, if it is bad, then the set
	 * to bisect is "reaches".
	 * A bisect set of size N has (N-1) commits further
	 * to test, as we already know one bad one.
	 */
	cnt = all - reaches;
	если (cnt < reaches)
		cnt = reaches;

	если (revs->commits)
		oid_to_hex_r(hex, &revs->commits->item->object.oid);

	если (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		printf("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_steps(all));

	вернуть 0;
}

static целое show_object_fast(
	const struct object_id *oid,
	enum object_type type UNUSED,
	целое exclude UNUSED,
	uint32_t name_hash UNUSED,
	struct packed_git *found_pack UNUSED,
	off_t found_offset UNUSED,
	void *payload UNUSED)
{
	fprintf(stdout, "%s\n", oid_to_hex(oid));
	вернуть 0;
}

static void print_disk_usage(off_t size)
{
	struct strbuf sb = STRBUF_INIT;
	если (human_readable)
		strbuf_humanise_bytes(&sb, size);
	иначе
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)size);
	puts(sb.buf);
	strbuf_release(&sb);
}

static inline целое parse_missing_action_value(struct repository *repo,
					     const char *value)
{
	если (!strcmp(value, "error")) {
		arg_missing_action = MA_ERROR;
		вернуть 1;
	}

	если (!strcmp(value, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		вернуть 1;
	}

	если (!strcmp(value, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		вернуть 1;
	}

	если (!strcmp(value, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		вернуть 1;
	}

	если (!strcmp(value, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		вернуть 1;
	}

	вернуть 0;
}

static целое try_bitmap_count(struct rev_info *revs,
			    целое filter_provided_objects)
{
	uint32_t commit_count = 0,
		 tag_count = 0,
		 tree_count = 0,
		 blob_count = 0;
	целое max_count;
	struct bitmap_index *bitmap_git;

	/* This function only handles counting, not general traversal. */
	если (!revs->count)
		вернуть -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	если (revs->left_right || revs->cherry_mark)
		вернуть -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	если (revs->max_count >= 0 &&
	    (revs->tag_objects || revs->tree_objects || revs->blob_objects))
		вернуть -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_count = revs->max_count;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	если (!bitmap_git)
		вернуть -1;

	count_bitmap_commit_list(bitmap_git, &commit_count,
				 revs->tree_objects ? &tree_count : NULL,
				 revs->blob_objects ? &blob_count : NULL,
				 revs->tag_objects ? &tag_count : NULL);
	если (max_count >= 0 && max_count < commit_count)
		commit_count = max_count;

	printf("%d\n", commit_count + tree_count + blob_count + tag_count);
	free_bitmap_index(bitmap_git);
	вернуть 0;
}

static целое try_bitmap_traversal(struct rev_info *revs,
				целое filter_provided_objects)
{
	struct bitmap_index *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	если (revs->max_count >= 0)
		вернуть -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	если (revs->left_right)
		вернуть -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	если (!bitmap_git)
		вернуть -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_index(bitmap_git);
	вернуть 0;
}

static целое try_bitmap_disk_usage(struct rev_info *revs,
				 целое filter_provided_objects)
{
	struct bitmap_index *bitmap_git;
	off_t size_from_bitmap;

	если (!show_disk_usage)
		вернуть -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	если (!bitmap_git)
		вернуть -1;

	size_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(size_from_bitmap);

	free_bitmap_index(bitmap_git);
	вернуть 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
static void prepare_maximal_independent(struct rev_info *revs)
{
	struct commit_list *c;

	если (!revs->maximal_only)
		вернуть;

	для (c = revs->commits; c; c = c->next) {
		если (c->item->object.flags & UNINTERESTING)
			вернуть;
	}

	если (revs->limited ||
	    revs->topo_order ||
	    revs->first_parent_only ||
	    revs->reverse ||
	    revs->max_count >= 0 ||
	    revs->skip_count >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_data.nr ||
	    revs->count ||
	    revs->left_right ||
	    revs->boundary ||
	    revs->tag_objects ||
	    revs->tree_objects ||
	    revs->blob_objects ||
	    revs->filter.choice ||
	    revs->reflog_info ||
	    revs->diff ||
	    revs->grep_filter.pattern_list ||
	    revs->grep_filter.header_list ||
	    revs->verbose_header ||
	    revs->print_parents ||
	    revs->edge_hint ||
	    revs->unpacked ||
	    revs->no_kept_objects ||
	    revs->line_level_traverse)
		вернуть;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

целое cmd_rev_list(целое argc,
		 const char **argv,
		 const char *prefix,
		 struct repository *repo)
{
	struct rev_info revs;
	struct rev_list_info info;
	struct setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	целое i;
	целое bisect_list = 0;
	целое bisect_show_vars = 0;
	целое bisect_find_all = 0;
	целое use_bitmap_index = 0;
	целое filter_provided_objects = 0;
	const char *show_progress = NULL;
	целое ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, NULL);
	repo_init_revisions(the_repository, &revs, prefix);
	revs.abbrev = DEFAULT_ABBREV;
	revs.commit_format = CMIT_FMT_UNSPECIFIED;
	revs.include_header = 1;

	/*
	 * Scan the argument list before invoking setup_revisions(), so that we
	 * know if fetch_if_missing needs to be set to 0.
	 *
	 * "--exclude-promisor-objects" acts as a pre-filter on missing objects
	 * by not crossing the boundary from realized objects to promisor
	 * objects.
	 *
	 * Let "--missing" to conditionally set fetch_if_missing.
	 */

	/*
	 * NEEDSWORK: The next loop is utterly broken.  It tries to
	 * notice an option is used, but without understanding if each
	 * option takes an argument, which fundamentally would not
	 * work.  It would not know "--grep
	 * --exclude-promisor-objects" is not triggering
	 * "--exclude-promisor-objects" option, for example.
	 *
	 * We really need setup_revisions() to have a mechanism to
	 * allow and disallow some sets of options for different
	 * commands (like rev-list, replay, etc). Such a mechanism
	 * should do an early parsing of options and be able to manage
	 * the `--missing=...` and `--exclude-promisor-objects`
	 * options below.
	 */
	для (i = 1; i < argc; i++) {
		const char *arg = argv[i];
		если (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} иначе если (skip_prefix(arg, "--missing=", &arg)) {
			parse_missing_action_value(repo, arg);
		} иначе если (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} иначе если (!strcmp(arg, "-z")) {
			line_term = '\0';
			info_term = '\0';
		}
	}

	если (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	если (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = setup_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, sizeof(info));
	info.revs = &revs;
	если (revs.bisect)
		bisect_list = 1;

	если (revs.diffopt.flags.quick)
		info.flags |= REV_LIST_QUIET;
	для (i = 1 ; i < argc; i++) {
		const char *arg = argv[i];

		если (!strcmp(arg, "--header")) {
			revs.verbose_header = 1;
			continue;
		}
		если (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			continue;
		}
		если (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			continue;
		}
		если (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_find_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			continue;
		}
		если (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			continue;
		}
		если (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_index = 1;
			continue;
		}
		если (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			goto cleanup;
		}
		если (skip_prefix(arg, "--progress=", &arg)) {
			show_progress = arg;
			continue;
		}
		если (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			continue;
		}
		если (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			continue;
		}

		если (!strcmp(arg, "--exclude-promisor-objects"))
			continue; /* already handled above */
		если (skip_prefix(arg, "--missing=", &arg))
			continue; /* already handled above */

		если (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_names = 0;
			continue;
		}

		если (!strcmp(arg, ("--object-names"))) {
			arg_show_object_names = 1;
			continue;
		}

		если (!strcmp(arg, ("--commit-header"))) {
			revs.include_header = 1;
			continue;
		}

		если (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_header = 0;
			continue;
		}

		если (skip_prefix(arg, "--disk-usage", &arg)) {
			если (*arg == '=') {
				если (!strcmp(++arg, "human")) {
					human_readable = 1;
				} иначе
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} иначе если (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			continue;
		}

		если (!strcmp(arg, "--missing-only"))
			continue;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	если (!line_term) {
		если (revs.graph || revs.verbose_header || show_disk_usage ||
		    info.show_timestamp || info.header_prefix || bisect_list ||
		    use_bitmap_index || revs.edge_hint || revs.left_right ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	если (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_header = 1;
	если (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		если (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_header)
			info.header_prefix = "";
		иначе
			info.header_prefix = "commit ";
	}
	иначе если (revs.verbose_header)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	если ((!revs.commits && reflog_walk_empty(revs.reflog_info) &&
	     (!(revs.tag_objects || revs.tree_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_input_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	если (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	если (revs.count &&
	    (revs.tag_objects || revs.tree_objects || revs.blob_objects) &&
	    (revs.left_right || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.count, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_buffer = (revs.verbose_header ||
			      revs.grep_filter.pattern_list ||
			      revs.grep_filter.header_list);
	если (bisect_list)
		revs.limited = 1;

	если (show_progress)
		progress = start_delayed_progress(the_repository,
						  show_progress, 0);

	если (use_bitmap_index) {
		если (!try_bitmap_count(&revs, filter_provided_objects))
			goto cleanup;
		если (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			goto cleanup;
		если (!try_bitmap_traversal(&revs, filter_provided_objects))
			goto cleanup;
	}

	если (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	если (revs.tree_objects)
		mark_edges_uninteresting(&revs, show_edge, 0);

	если (bisect_list) {
		целое reaches, all;
		unsigned bisect_flags = 0;

		если (bisect_find_all)
			bisect_flags |= FIND_BISECTION_ALL;

		если (revs.first_parent_only)
			bisect_flags |= FIND_BISECTION_FIRST_PARENT_ONLY;

		find_bisection(&revs.commits, &reaches, &all, bisect_flags);

		если (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			goto cleanup;
		}
	}

	если (filter_provided_objects) {
		struct commit_list *c;
		для (i = 0; i < revs.pending.nr; i++) {
			struct object_array_entry *pending = revs.pending.objects + i;
			pending->item->flags |= NOT_USER_GIVEN;
		}
		для (c = revs.commits; c; c = c->next)
			c->item->object.flags |= NOT_USER_GIVEN;
	}

	если (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_SIZE);
	если (should_collect_missing()) {
		struct oidset_iter iter;
		struct object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_SIZE);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		пока ((oid = oidset_iter_next(&iter)))
			add_missing_object_entry(oid, NULL, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : NULL));

	если (arg_print_omitted) {
		struct oidset_iter iter;
		struct object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		пока ((oid = oidset_iter_next(&iter)))
			printf("~%s\n", oid_to_hex(oid));
		oidset_clear(&omitted_objects);
	}
	если (should_collect_missing()) {
		struct missing_objects_map_entry *entry;
		struct oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		пока ((entry = oidmap_iter_next(&iter))) {
			print_missing_object(entry, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_entry_free);
	}

	stop_progress(&progress);

	если (revs.count) {
		если (revs.left_right && revs.cherry_mark)
			printf("%d\t%d\t%d\n", revs.count_left, revs.count_right, revs.count_same);
		иначе если (revs.left_right)
			printf("%d\t%d\n", revs.count_left, revs.count_right);
		иначе если (revs.cherry_mark)
			printf("%d\t%d\n", revs.count_left + revs.count_right, revs.count_same);
		иначе
			printf("%d\n", revs.count_left + revs.count_right);
	}

	если (show_disk_usage)
		print_disk_usage(total_disk_usage);

cleanup:
	release_revisions(&revs);
	вернуть ret;
}
