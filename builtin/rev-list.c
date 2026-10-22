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

sa_nracanaa rev_list_info {
	sa_nracanaa rev_info *revs;
	ຈຳນວນເຕັມ flags;
	ຈຳນວນເຕັມ show_timestamp;
	ຈຳນວນເຕັມ hdr_termination;
	const var_na *header_prefix;
};

static const var_na rev_list_usage[] =
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

static sa_nracanaa progress *progress;
static unsigned progress_counter;

static sa_nracanaa oidset omitted_objects;
static ຈຳນວນເຕັມ arg_print_omitted; /* print objects omitted by filter */

sa_nracanaa missing_objects_map_entry {
	sa_nracanaa oidmap_entry entry;
	var_na *path;
	unsigned type;
};

static void missing_objects_map_entry_free(void *e)
{
	sa_nracanaa missing_objects_map_entry *entry =
		container_of(e, sa_nracanaa missing_objects_map_entry, entry);

	free(entry->path);
	free(entry);
}

static sa_nracanaa oidmap missing_objects;
ga_nanaa missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
static ga_nanaa missing_action arg_missing_action;
static ຈຳນວນເຕັມ arg_missing_only;

static anta_hstha ຈຳນວນເຕັມ should_collect_missing(void)
{
	ສົ່ງຄືນ arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
static ຈຳນວນເຕັມ arg_show_object_names = 1;

#define DEFAULT_OIDSET_SIZE     (16*1024)

static var_na line_term = '\n';
static var_na info_term = ' ';

static ຈຳນວນເຕັມ show_disk_usage;
static off_t total_disk_usage;
static ຈຳນວນເຕັມ human_readable;

static off_t get_object_disk_usage(sa_nracanaa object *obj)
{
	off_t size;
	sa_nracanaa object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &size;
	ຖ້າ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_to_hex(&obj->oid));
	ສົ່ງຄືນ size;
}

static void add_missing_object_entry(sa_nracanaa object_id *oid, const var_na *path,
				     unsigned type)
{
	sa_nracanaa missing_objects_map_entry *entry;

	ຖ້າ (oidmap_get(&missing_objects, oid))
		ສົ່ງຄືນ;

	CALLOC_ARRAY(entry, 1);
	entry->entry.oid = *oid;
	entry->type = type;
	ຖ້າ (path)
		entry->path = xstrdup(path);
	oidmap_put(&missing_objects, entry);
}

static void print_missing_object(sa_nracanaa missing_objects_map_entry *entry,
				 ຈຳນວນເຕັມ print_missing_info)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	ຖ້າ (arg_missing_only && line_term)
		printf("%s", oid_to_hex(&entry->entry.oid));
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (line_term)
		printf("?%s", oid_to_hex(&entry->entry.oid));
	ບໍ່ດັ່ງນັ້ນ
		printf("%s%cmissing=yes", oid_to_hex(&entry->entry.oid),
		       info_term);

	ຖ້າ (!print_missing_info) {
		putchar(line_term);
		ສົ່ງຄືນ;
	}

	ຖ້າ (entry->path && *entry->path) {
		strbuf_addf(&sb, "%cpath=", info_term);

		ຖ້າ (line_term) {
			sa_nracanaa strbuf path = STRBUF_INIT;

			quote_path(entry->path, NULL, &path, QUOTE_PATH_QUOTE_SP);
			strbuf_addbuf(&sb, &path);

			strbuf_release(&path);
		} ບໍ່ດັ່ງນັ້ນ {
			strbuf_addstr(&sb, entry->path);
		}
	}
	ຖ້າ (entry->type)
		strbuf_addf(&sb, "%ctype=%s", info_term, type_name(entry->type));

	fwrite(sb.buf, parimaa_na(var_na), sb.len, stdout);
	putchar(line_term);

	strbuf_release(&sb);
}

static anta_hstha void finish_object__ma(sa_nracanaa object *obj, const var_na *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	switch (arg_missing_action) {
	pak_sa MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		ສົ່ງຄືນ;

	pak_sa MA_ALLOW_ANY:
		ສົ່ງຄືນ;

	pak_sa MA_PRINT:
	pak_sa MA_PRINT_INFO:
		add_missing_object_entry(&obj->oid, name, obj->type);
		ສົ່ງຄືນ;

	pak_sa MA_ALLOW_PROMISOR:
		ຖ້າ (is_promisor_object(the_repository, &obj->oid))
			ສົ່ງຄືນ;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		ສົ່ງຄືນ;

	default:
		BUG("unhandled missing_action");
		ສົ່ງຄືນ;
	}
}

static void finish_commit(sa_nracanaa commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = NULL;
	free_commit_buffer(the_repository->parsed_objects,
			   commit);
}

static void show_commit(sa_nracanaa commit *commit, void *data)
{
	sa_nracanaa rev_list_info *info = data;
	sa_nracanaa rev_info *revs = info->revs;

	display_progress(progress, ++progress_counter);

	ຖ້າ (revs->do_not_die_on_missing_objects &&
	    oidset_contains(&revs->missing_commits, &commit->object.oid)) {
		finish_object__ma(&commit->object, NULL);
		ສົ່ງຄືນ;
	}

	ຖ້າ (arg_missing_only) {
		finish_commit(commit);
		ສົ່ງຄືນ;
	}

	ຖ້າ (show_disk_usage)
		total_disk_usage += get_object_disk_usage(&commit->object);

	ຖ້າ (info->flags & REV_LIST_QUIET) {
		finish_commit(commit);
		ສົ່ງຄືນ;
	}

	graph_show_commit(revs->graph);

	ຖ້າ (revs->count) {
		ຖ້າ (commit->object.flags & PATCHSAME)
			revs->count_same++;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (commit->object.flags & SYMMETRIC_LEFT)
			revs->count_left++;
		ບໍ່ດັ່ງນັ້ນ
			revs->count_right++;
		finish_commit(commit);
		ສົ່ງຄືນ;
	}

	ຖ້າ (info->show_timestamp)
		printf("%"PRItime" ", commit->date);
	ຖ້າ (info->header_prefix)
		fputs(info->header_prefix, stdout);

	ຖ້າ (revs->include_header) {
		ຖ້າ (!revs->graph && line_term)
			fputs(get_revision_mark(revs, commit), stdout);
		ຖ້າ (revs->abbrev_commit && revs->abbrev)
			fputs(repo_find_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		ບໍ່ດັ່ງນັ້ນ
			fputs(oid_to_hex(&commit->object.oid), stdout);

		ຖ້າ (!line_term) {
			ຖ້າ (commit->object.flags & BOUNDARY)
				printf("%cboundary=yes", info_term);
		}
	}
	ຖ້າ (revs->print_parents) {
		sa_nracanaa commit_list *parents = commit->parents;
		ໃນຂະນະທີ່ (parents) {
			printf(" %s", oid_to_hex(&parents->item->object.oid));
			parents = parents->next;
		}
	}
	ຖ້າ (revs->children.name) {
		sa_nracanaa commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		ໃນຂະນະທີ່ (children) {
			printf(" %s", oid_to_hex(&children->item->object.oid));
			children = children->next;
		}
	}
	show_decorations(revs, commit);
	ຖ້າ (revs->commit_format == CMIT_FMT_ONELINE)
		putchar(' ');
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (revs->include_header)
		putchar(line_term);

	ຖ້າ (revs->verbose_header) {
		sa_nracanaa strbuf buf = STRBUF_INIT;
		sa_nracanaa pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_mode = revs->date_mode;
		ctx.date_mode_explicit = revs->date_mode_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		ຖ້າ (buf.len) {
			ຖ້າ (revs->commit_format != CMIT_FMT_ONELINE)
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
			ຖ້າ (buf.len && buf.buf[buf.len - 1] == '\n')
				graph_show_padding(revs->graph);
			putchar(info->hdr_termination);
		} ບໍ່ດັ່ງນັ້ນ {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			ຖ້າ (graph_show_remainder(revs->graph))
				putchar('\n');
			ຖ້າ (revs->commit_format == CMIT_FMT_ONELINE)
				putchar('\n');
		}
		strbuf_release(&buf);
	} ບໍ່ດັ່ງນັ້ນ {
		ຖ້າ (graph_show_remainder(revs->graph))
			putchar('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	finish_commit(commit);
}

static ຈຳນວນເຕັມ finish_object(sa_nracanaa object *obj, const var_na *name, void *cb_data)
{
	sa_nracanaa rev_list_info *info = cb_data;
	ຖ້າ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, NULL, 0) < 0) {
		finish_object__ma(obj, name);
		ສົ່ງຄືນ 1;
	}
	ຖ້າ (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	ສົ່ງຄືນ 0;
}

static void show_object(sa_nracanaa object *obj, const var_na *name, void *cb_data)
{
	sa_nracanaa rev_list_info *info = cb_data;
	sa_nracanaa rev_info *revs = info->revs;

	ຖ້າ (finish_object(obj, name, cb_data))
		ສົ່ງຄືນ;
	display_progress(progress, ++progress_counter);
	ຖ້າ (arg_missing_only)
		ສົ່ງຄືນ;
	ຖ້າ (show_disk_usage)
		total_disk_usage += get_object_disk_usage(obj);
	ຖ້າ (info->flags & REV_LIST_QUIET)
		ສົ່ງຄືນ;

	ຖ້າ (revs->count) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->count_right++;
		ສົ່ງຄືນ;
	}

	printf("%s", oid_to_hex(&obj->oid));

	ຖ້າ (arg_show_object_names) {
		ຖ້າ (line_term) {
			putchar(info_term);
			ສຳລັບ (const var_na *p = name; *p && *p != '\n'; p++)
				putchar(*p);
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (*name) {
			printf("%cpath=%s", info_term, name);
		}
	}

	putchar(line_term);
}

static void show_edge(sa_nracanaa commit *commit)
{
	printf("-%s\n", oid_to_hex(&commit->object.oid));
}

static void print_var_str(const var_na *var, const var_na *val)
{
	printf("%s='%s'\n", var, val);
}

static void print_var_int(const var_na *var, ຈຳນວນເຕັມ val)
{
	printf("%s=%d\n", var, val);
}

static ຈຳນວນເຕັມ show_bisect_vars(sa_nracanaa rev_list_info *info, ຈຳນວນເຕັມ reaches, ຈຳນວນເຕັມ all)
{
	ຈຳນວນເຕັມ cnt, flags = info->flags;
	var_na hex[GIT_MAX_HEXSZ + 1] = "";
	sa_nracanaa commit_list *tried;
	sa_nracanaa rev_info *revs = info->revs;

	ຖ້າ (!revs->commits)
		ສົ່ງຄືນ 1;

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
	ຖ້າ (cnt < reaches)
		cnt = reaches;

	ຖ້າ (revs->commits)
		oid_to_hex_r(hex, &revs->commits->item->object.oid);

	ຖ້າ (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		printf("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_steps(all));

	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ show_object_fast(
	const sa_nracanaa object_id *oid,
	ga_nanaa object_type type UNUSED,
	ຈຳນວນເຕັມ exclude UNUSED,
	uint32_t name_hash UNUSED,
	sa_nracanaa packed_git *found_pack UNUSED,
	off_t found_offset UNUSED,
	void *payload UNUSED)
{
	fprintf(stdout, "%s\n", oid_to_hex(oid));
	ສົ່ງຄືນ 0;
}

static void print_disk_usage(off_t size)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	ຖ້າ (human_readable)
		strbuf_humanise_bytes(&sb, size);
	ບໍ່ດັ່ງນັ້ນ
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)size);
	puts(sb.buf);
	strbuf_release(&sb);
}

static anta_hstha ຈຳນວນເຕັມ parse_missing_action_value(sa_nracanaa repository *repo,
					     const var_na *value)
{
	ຖ້າ (!strcmp(value, "error")) {
		arg_missing_action = MA_ERROR;
		ສົ່ງຄືນ 1;
	}

	ຖ້າ (!strcmp(value, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		ສົ່ງຄືນ 1;
	}

	ຖ້າ (!strcmp(value, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		ສົ່ງຄືນ 1;
	}

	ຖ້າ (!strcmp(value, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		ສົ່ງຄືນ 1;
	}

	ຖ້າ (!strcmp(value, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		ສົ່ງຄືນ 1;
	}

	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ try_bitmap_count(sa_nracanaa rev_info *revs,
			    ຈຳນວນເຕັມ filter_provided_objects)
{
	uint32_t commit_count = 0,
		 tag_count = 0,
		 tree_count = 0,
		 blob_count = 0;
	ຈຳນວນເຕັມ max_count;
	sa_nracanaa bitmap_index *bitmap_git;

	/* This function only handles counting, not general traversal. */
	ຖ້າ (!revs->count)
		ສົ່ງຄືນ -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	ຖ້າ (revs->left_right || revs->cherry_mark)
		ສົ່ງຄືນ -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	ຖ້າ (revs->max_count >= 0 &&
	    (revs->tag_objects || revs->tree_objects || revs->blob_objects))
		ສົ່ງຄືນ -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_count = revs->max_count;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ຖ້າ (!bitmap_git)
		ສົ່ງຄືນ -1;

	count_bitmap_commit_list(bitmap_git, &commit_count,
				 revs->tree_objects ? &tree_count : NULL,
				 revs->blob_objects ? &blob_count : NULL,
				 revs->tag_objects ? &tag_count : NULL);
	ຖ້າ (max_count >= 0 && max_count < commit_count)
		commit_count = max_count;

	printf("%d\n", commit_count + tree_count + blob_count + tag_count);
	free_bitmap_index(bitmap_git);
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ try_bitmap_traversal(sa_nracanaa rev_info *revs,
				ຈຳນວນເຕັມ filter_provided_objects)
{
	sa_nracanaa bitmap_index *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	ຖ້າ (revs->max_count >= 0)
		ສົ່ງຄືນ -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	ຖ້າ (revs->left_right)
		ສົ່ງຄືນ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ຖ້າ (!bitmap_git)
		ສົ່ງຄືນ -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_index(bitmap_git);
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ try_bitmap_disk_usage(sa_nracanaa rev_info *revs,
				 ຈຳນວນເຕັມ filter_provided_objects)
{
	sa_nracanaa bitmap_index *bitmap_git;
	off_t size_from_bitmap;

	ຖ້າ (!show_disk_usage)
		ສົ່ງຄືນ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ຖ້າ (!bitmap_git)
		ສົ່ງຄືນ -1;

	size_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(size_from_bitmap);

	free_bitmap_index(bitmap_git);
	ສົ່ງຄືນ 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
static void prepare_maximal_independent(sa_nracanaa rev_info *revs)
{
	sa_nracanaa commit_list *c;

	ຖ້າ (!revs->maximal_only)
		ສົ່ງຄືນ;

	ສຳລັບ (c = revs->commits; c; c = c->next) {
		ຖ້າ (c->item->object.flags & UNINTERESTING)
			ສົ່ງຄືນ;
	}

	ຖ້າ (revs->limited ||
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
		ສົ່ງຄືນ;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

ຈຳນວນເຕັມ cmd_rev_list(ຈຳນວນເຕັມ argc,
		 const var_na **argv,
		 const var_na *prefix,
		 sa_nracanaa repository *repo)
{
	sa_nracanaa rev_info revs;
	sa_nracanaa rev_list_info info;
	sa_nracanaa setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	ຈຳນວນເຕັມ i;
	ຈຳນວນເຕັມ bisect_list = 0;
	ຈຳນວນເຕັມ bisect_show_vars = 0;
	ຈຳນວນເຕັມ bisect_find_all = 0;
	ຈຳນວນເຕັມ use_bitmap_index = 0;
	ຈຳນວນເຕັມ filter_provided_objects = 0;
	const var_na *show_progress = NULL;
	ຈຳນວນເຕັມ ret = 0;

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
	ສຳລັບ (i = 1; i < argc; i++) {
		const var_na *arg = argv[i];
		ຖ້າ (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (skip_prefix(arg, "--missing=", &arg)) {
			parse_missing_action_value(repo, arg);
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "-z")) {
			line_term = '\0';
			info_term = '\0';
		}
	}

	ຖ້າ (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	ຖ້າ (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = setup_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, parimaa_na(info));
	info.revs = &revs;
	ຖ້າ (revs.bisect)
		bisect_list = 1;

	ຖ້າ (revs.diffopt.flags.quick)
		info.flags |= REV_LIST_QUIET;
	ສຳລັບ (i = 1 ; i < argc; i++) {
		const var_na *arg = argv[i];

		ຖ້າ (!strcmp(arg, "--header")) {
			revs.verbose_header = 1;
			continue;
		}
		ຖ້າ (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			continue;
		}
		ຖ້າ (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			continue;
		}
		ຖ້າ (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_find_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			continue;
		}
		ຖ້າ (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			continue;
		}
		ຖ້າ (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_index = 1;
			continue;
		}
		ຖ້າ (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			goto cleanup;
		}
		ຖ້າ (skip_prefix(arg, "--progress=", &arg)) {
			show_progress = arg;
			continue;
		}
		ຖ້າ (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			continue;
		}
		ຖ້າ (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			continue;
		}

		ຖ້າ (!strcmp(arg, "--exclude-promisor-objects"))
			continue; /* already handled above */
		ຖ້າ (skip_prefix(arg, "--missing=", &arg))
			continue; /* already handled above */

		ຖ້າ (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_names = 0;
			continue;
		}

		ຖ້າ (!strcmp(arg, ("--object-names"))) {
			arg_show_object_names = 1;
			continue;
		}

		ຖ້າ (!strcmp(arg, ("--commit-header"))) {
			revs.include_header = 1;
			continue;
		}

		ຖ້າ (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_header = 0;
			continue;
		}

		ຖ້າ (skip_prefix(arg, "--disk-usage", &arg)) {
			ຖ້າ (*arg == '=') {
				ຖ້າ (!strcmp(++arg, "human")) {
					human_readable = 1;
				} ບໍ່ດັ່ງນັ້ນ
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} ບໍ່ດັ່ງນັ້ນ ຖ້າ (*arg) {
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

		ຖ້າ (!strcmp(arg, "--missing-only"))
			continue;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	ຖ້າ (!line_term) {
		ຖ້າ (revs.graph || revs.verbose_header || show_disk_usage ||
		    info.show_timestamp || info.header_prefix || bisect_list ||
		    use_bitmap_index || revs.edge_hint || revs.left_right ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	ຖ້າ (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_header = 1;
	ຖ້າ (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		ຖ້າ (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_header)
			info.header_prefix = "";
		ບໍ່ດັ່ງນັ້ນ
			info.header_prefix = "commit ";
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (revs.verbose_header)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	ຖ້າ ((!revs.commits && reflog_walk_empty(revs.reflog_info) &&
	     (!(revs.tag_objects || revs.tree_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_input_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	ຖ້າ (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	ຖ້າ (revs.count &&
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
	ຖ້າ (bisect_list)
		revs.limited = 1;

	ຖ້າ (show_progress)
		progress = start_delayed_progress(the_repository,
						  show_progress, 0);

	ຖ້າ (use_bitmap_index) {
		ຖ້າ (!try_bitmap_count(&revs, filter_provided_objects))
			goto cleanup;
		ຖ້າ (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			goto cleanup;
		ຖ້າ (!try_bitmap_traversal(&revs, filter_provided_objects))
			goto cleanup;
	}

	ຖ້າ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	ຖ້າ (revs.tree_objects)
		mark_edges_uninteresting(&revs, show_edge, 0);

	ຖ້າ (bisect_list) {
		ຈຳນວນເຕັມ reaches, all;
		unsigned bisect_flags = 0;

		ຖ້າ (bisect_find_all)
			bisect_flags |= FIND_BISECTION_ALL;

		ຖ້າ (revs.first_parent_only)
			bisect_flags |= FIND_BISECTION_FIRST_PARENT_ONLY;

		find_bisection(&revs.commits, &reaches, &all, bisect_flags);

		ຖ້າ (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			goto cleanup;
		}
	}

	ຖ້າ (filter_provided_objects) {
		sa_nracanaa commit_list *c;
		ສຳລັບ (i = 0; i < revs.pending.nr; i++) {
			sa_nracanaa object_array_entry *pending = revs.pending.objects + i;
			pending->item->flags |= NOT_USER_GIVEN;
		}
		ສຳລັບ (c = revs.commits; c; c = c->next)
			c->item->object.flags |= NOT_USER_GIVEN;
	}

	ຖ້າ (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_SIZE);
	ຖ້າ (should_collect_missing()) {
		sa_nracanaa oidset_iter iter;
		sa_nracanaa object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_SIZE);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		ໃນຂະນະທີ່ ((oid = oidset_iter_next(&iter)))
			add_missing_object_entry(oid, NULL, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : NULL));

	ຖ້າ (arg_print_omitted) {
		sa_nracanaa oidset_iter iter;
		sa_nracanaa object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		ໃນຂະນະທີ່ ((oid = oidset_iter_next(&iter)))
			printf("~%s\n", oid_to_hex(oid));
		oidset_clear(&omitted_objects);
	}
	ຖ້າ (should_collect_missing()) {
		sa_nracanaa missing_objects_map_entry *entry;
		sa_nracanaa oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		ໃນຂະນະທີ່ ((entry = oidmap_iter_next(&iter))) {
			print_missing_object(entry, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_entry_free);
	}

	stop_progress(&progress);

	ຖ້າ (revs.count) {
		ຖ້າ (revs.left_right && revs.cherry_mark)
			printf("%d\t%d\t%d\n", revs.count_left, revs.count_right, revs.count_same);
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (revs.left_right)
			printf("%d\t%d\n", revs.count_left, revs.count_right);
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (revs.cherry_mark)
			printf("%d\t%d\n", revs.count_left + revs.count_right, revs.count_same);
		ບໍ່ດັ່ງນັ້ນ
			printf("%d\n", revs.count_left + revs.count_right);
	}

	ຖ້າ (show_disk_usage)
		print_disk_usage(total_disk_usage);

cleanup:
	release_revisions(&revs);
	ສົ່ງຄືນ ret;
}
