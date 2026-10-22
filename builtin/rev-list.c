#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "commit.h"
#شامل "diff.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "revision.h"
#شامل "list-objects.h"
#شامل "list-objects-filter-options.h"
#شامل "object.h"
#شامل "object-name.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "pack-bitmap.h"
#شامل "parse-options.h"
#شامل "log-tree.h"
#شامل "graph.h"
#شامل "bisect.h"
#شامل "progress.h"
#شامل "reflog-walk.h"
#شامل "oidset.h"
#شامل "oidmap.h"
#شامل "packfile.h"
#شامل "commit-reach.h"
#شامل "quote.h"
#شامل "strbuf.h"

ساختار rev_list_info {
	ساختار rev_info *revs;
	صحیح flags;
	صحیح show_timestamp;
	صحیح hdr_termination;
	ثابت نویسه *header_prefix;
};

ایستا ثابت نویسه rev_list_usage[] =
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

ایستا ساختار progress *progress;
ایستا unsigned progress_counter;

ایستا ساختار oidset omitted_objects;
ایستا صحیح arg_print_omitted; /* print objects omitted by filter */

ساختار missing_objects_map_entry {
	ساختار oidmap_entry entry;
	نویسه *path;
	unsigned type;
};

ایستا تهی missing_objects_map_entry_free(تهی *e)
{
	ساختار missing_objects_map_entry *entry =
		container_of(e, ساختار missing_objects_map_entry, entry);

	آزاد(entry->path);
	آزاد(entry);
}

ایستا ساختار oidmap missing_objects;
شمارش missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ایستا شمارش missing_action arg_missing_action;
ایستا صحیح arg_missing_only;

ایستا درونخطی صحیح should_collect_missing(تهی)
{
	بازگشت arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
ایستا صحیح arg_show_object_names = 1;

#define DEFAULT_OIDSET_SIZE     (16*1024)

ایستا نویسه سطر_term = '\n';
ایستا نویسه info_term = ' ';

ایستا صحیح show_disk_usage;
ایستا off_t total_disk_usage;
ایستا صحیح human_readable;

ایستا off_t get_object_disk_usage(ساختار object *obj)
{
	off_t size;
	ساختار object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &size;
	اگر (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_to_hex(&obj->oid));
	بازگشت size;
}

ایستا تهی add_missing_object_entry(ساختار object_id *oid, ثابت نویسه *path,
				     unsigned type)
{
	ساختار missing_objects_map_entry *entry;

	اگر (oidmap_get(&missing_objects, oid))
		بازگشت;

	CALLOC_ARRAY(entry, 1);
	entry->entry.oid = *oid;
	entry->type = type;
	اگر (path)
		entry->path = xstrdup(path);
	oidmap_put(&missing_objects, entry);
}

ایستا تهی print_missing_object(ساختار missing_objects_map_entry *entry,
				 صحیح print_missing_info)
{
	ساختار strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	اگر (arg_missing_only && سطر_term)
		printf("%s", oid_to_hex(&entry->entry.oid));
	وگرنه اگر (سطر_term)
		printf("?%s", oid_to_hex(&entry->entry.oid));
	وگرنه
		printf("%s%cmissing=yes", oid_to_hex(&entry->entry.oid),
		       info_term);

	اگر (!print_missing_info) {
		putchar(سطر_term);
		بازگشت;
	}

	اگر (entry->path && *entry->path) {
		strbuf_addf(&sb, "%cpath=", info_term);

		اگر (سطر_term) {
			ساختار strbuf path = STRBUF_INIT;

			quote_path(entry->path, پوچ, &path, QUOTE_PATH_QUOTE_SP);
			strbuf_addbuf(&sb, &path);

			strbuf_release(&path);
		} وگرنه {
			strbuf_addstr(&sb, entry->path);
		}
	}
	اگر (entry->type)
		strbuf_addf(&sb, "%ctype=%s", info_term, type_name(entry->type));

	fwrite(sb.buf, اندازه(نویسه), sb.len, stdout);
	putchar(سطر_term);

	strbuf_release(&sb);
}

ایستا درونخطی تهی finish_object__ma(ساختار object *obj, ثابت نویسه *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	انتخاب (arg_missing_action) {
	حالت MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		بازگشت;

	حالت MA_ALLOW_ANY:
		بازگشت;

	حالت MA_PRINT:
	حالت MA_PRINT_INFO:
		add_missing_object_entry(&obj->oid, name, obj->type);
		بازگشت;

	حالت MA_ALLOW_PROMISOR:
		اگر (is_promisor_object(the_repository, &obj->oid))
			بازگشت;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		بازگشت;

	پیشفرض:
		BUG("unhandled missing_action");
		بازگشت;
	}
}

ایستا تهی finish_commit(ساختار commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = پوچ;
	free_commit_buffer(the_repository->parsed_objects,
			   commit);
}

ایستا تهی show_commit(ساختار commit *commit, تهی *data)
{
	ساختار rev_list_info *info = data;
	ساختار rev_info *revs = info->revs;

	display_progress(progress, ++progress_counter);

	اگر (revs->do_not_die_on_missing_objects &&
	    oidset_contains(&revs->missing_commits, &commit->object.oid)) {
		finish_object__ma(&commit->object, پوچ);
		بازگشت;
	}

	اگر (arg_missing_only) {
		finish_commit(commit);
		بازگشت;
	}

	اگر (show_disk_usage)
		total_disk_usage += get_object_disk_usage(&commit->object);

	اگر (info->flags & REV_LIST_QUIET) {
		finish_commit(commit);
		بازگشت;
	}

	graph_show_commit(revs->graph);

	اگر (revs->شمار) {
		اگر (commit->object.flags & PATCHSAME)
			revs->شمار_same++;
		وگرنه اگر (commit->object.flags & SYMMETRIC_چپ)
			revs->شمار_چپ++;
		وگرنه
			revs->شمار_راست++;
		finish_commit(commit);
		بازگشت;
	}

	اگر (info->show_timestamp)
		printf("%"PRItime" ", commit->date);
	اگر (info->header_prefix)
		fputs(info->header_prefix, stdout);

	اگر (revs->include_header) {
		اگر (!revs->graph && سطر_term)
			fputs(get_revision_mark(revs, commit), stdout);
		اگر (revs->abbrev_commit && revs->abbrev)
			fputs(repo_find_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		وگرنه
			fputs(oid_to_hex(&commit->object.oid), stdout);

		اگر (!سطر_term) {
			اگر (commit->object.flags & BOUNDARY)
				printf("%cboundary=yes", info_term);
		}
	}
	اگر (revs->print_parents) {
		ساختار commit_list *parents = commit->parents;
		وقتی (parents) {
			printf(" %s", oid_to_hex(&parents->item->object.oid));
			parents = parents->next;
		}
	}
	اگر (revs->children.name) {
		ساختار commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		وقتی (children) {
			printf(" %s", oid_to_hex(&children->item->object.oid));
			children = children->next;
		}
	}
	show_decorations(revs, commit);
	اگر (revs->commit_format == CMIT_FMT_ONELINE)
		putchar(' ');
	وگرنه اگر (revs->include_header)
		putchar(سطر_term);

	اگر (revs->verbose_header) {
		ساختار strbuf buf = STRBUF_INIT;
		ساختار pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_mode = revs->date_mode;
		ctx.date_mode_explicit = revs->date_mode_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		اگر (buf.len) {
			اگر (revs->commit_format != CMIT_FMT_ONELINE)
				graph_show_oneline(revs->graph);

			graph_show_commit_پیام(revs->graph, stdout, &buf);

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
			اگر (buf.len && buf.buf[buf.len - 1] == '\n')
				graph_show_padding(revs->graph);
			putchar(info->hdr_termination);
		} وگرنه {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			اگر (graph_show_remainder(revs->graph))
				putchar('\n');
			اگر (revs->commit_format == CMIT_FMT_ONELINE)
				putchar('\n');
		}
		strbuf_release(&buf);
	} وگرنه {
		اگر (graph_show_remainder(revs->graph))
			putchar('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	finish_commit(commit);
}

ایستا صحیح finish_object(ساختار object *obj, ثابت نویسه *name, تهی *cb_data)
{
	ساختار rev_list_info *info = cb_data;
	اگر (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, پوچ, 0) < 0) {
		finish_object__ma(obj, name);
		بازگشت 1;
	}
	اگر (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	بازگشت 0;
}

ایستا تهی show_object(ساختار object *obj, ثابت نویسه *name, تهی *cb_data)
{
	ساختار rev_list_info *info = cb_data;
	ساختار rev_info *revs = info->revs;

	اگر (finish_object(obj, name, cb_data))
		بازگشت;
	display_progress(progress, ++progress_counter);
	اگر (arg_missing_only)
		بازگشت;
	اگر (show_disk_usage)
		total_disk_usage += get_object_disk_usage(obj);
	اگر (info->flags & REV_LIST_QUIET)
		بازگشت;

	اگر (revs->شمار) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->شمار_راست++;
		بازگشت;
	}

	printf("%s", oid_to_hex(&obj->oid));

	اگر (arg_show_object_names) {
		اگر (سطر_term) {
			putchar(info_term);
			برای (ثابت نویسه *p = name; *p && *p != '\n'; p++)
				putchar(*p);
		} وگرنه اگر (*name) {
			printf("%cpath=%s", info_term, name);
		}
	}

	putchar(سطر_term);
}

ایستا تهی show_edge(ساختار commit *commit)
{
	printf("-%s\n", oid_to_hex(&commit->object.oid));
}

ایستا تهی print_var_str(ثابت نویسه *var, ثابت نویسه *val)
{
	printf("%s='%s'\n", var, val);
}

ایستا تهی print_var_int(ثابت نویسه *var, صحیح val)
{
	printf("%s=%d\n", var, val);
}

ایستا صحیح show_bisect_vars(ساختار rev_list_info *info, صحیح reaches, صحیح all)
{
	صحیح cnt, flags = info->flags;
	نویسه hex[GIT_MAX_HEXSZ + 1] = "";
	ساختار commit_list *tried;
	ساختار rev_info *revs = info->revs;

	اگر (!revs->commits)
		بازگشت 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       پوچ, پوچ);

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
	اگر (cnt < reaches)
		cnt = reaches;

	اگر (revs->commits)
		oid_to_hex_r(hex, &revs->commits->item->object.oid);

	اگر (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		printf("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_steps(all));

	بازگشت 0;
}

ایستا صحیح show_object_fast(
	ثابت ساختار object_id *oid,
	شمارش object_type type UNUSED,
	صحیح exclude UNUSED,
	uint32_t name_hash UNUSED,
	ساختار packed_git *found_pack UNUSED,
	off_t found_offset UNUSED,
	تهی *payload UNUSED)
{
	fprintf(stdout, "%s\n", oid_to_hex(oid));
	بازگشت 0;
}

ایستا تهی print_disk_usage(off_t size)
{
	ساختار strbuf sb = STRBUF_INIT;
	اگر (human_readable)
		strbuf_humanise_bytes(&sb, size);
	وگرنه
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)size);
	puts(sb.buf);
	strbuf_release(&sb);
}

ایستا درونخطی صحیح parse_missing_action_مقدار(ساختار repository *repo,
					     ثابت نویسه *مقدار)
{
	اگر (!strcmp(مقدار, "error")) {
		arg_missing_action = MA_ERROR;
		بازگشت 1;
	}

	اگر (!strcmp(مقدار, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		بازگشت 1;
	}

	اگر (!strcmp(مقدار, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		بازگشت 1;
	}

	اگر (!strcmp(مقدار, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		بازگشت 1;
	}

	اگر (!strcmp(مقدار, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		بازگشت 1;
	}

	بازگشت 0;
}

ایستا صحیح try_bitmap_شمار(ساختار rev_info *revs,
			    صحیح filter_provided_objects)
{
	uint32_t commit_شمار = 0,
		 tag_شمار = 0,
		 درخت_شمار = 0,
		 blob_شمار = 0;
	صحیح max_شمار;
	ساختار bitmap_index *bitmap_git;

	/* This function only handles counting, not general traversal. */
	اگر (!revs->شمار)
		بازگشت -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	اگر (revs->چپ_راست || revs->cherry_mark)
		بازگشت -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	اگر (revs->max_شمار >= 0 &&
	    (revs->tag_objects || revs->درخت_objects || revs->blob_objects))
		بازگشت -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_شمار = revs->max_شمار;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	اگر (!bitmap_git)
		بازگشت -1;

	شمار_bitmap_commit_list(bitmap_git, &commit_شمار,
				 revs->درخت_objects ? &درخت_شمار : پوچ,
				 revs->blob_objects ? &blob_شمار : پوچ,
				 revs->tag_objects ? &tag_شمار : پوچ);
	اگر (max_شمار >= 0 && max_شمار < commit_شمار)
		commit_شمار = max_شمار;

	printf("%d\n", commit_شمار + درخت_شمار + blob_شمار + tag_شمار);
	free_bitmap_index(bitmap_git);
	بازگشت 0;
}

ایستا صحیح try_bitmap_traversal(ساختار rev_info *revs,
				صحیح filter_provided_objects)
{
	ساختار bitmap_index *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	اگر (revs->max_شمار >= 0)
		بازگشت -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	اگر (revs->چپ_راست)
		بازگشت -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	اگر (!bitmap_git)
		بازگشت -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_index(bitmap_git);
	بازگشت 0;
}

ایستا صحیح try_bitmap_disk_usage(ساختار rev_info *revs,
				 صحیح filter_provided_objects)
{
	ساختار bitmap_index *bitmap_git;
	off_t size_from_bitmap;

	اگر (!show_disk_usage)
		بازگشت -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	اگر (!bitmap_git)
		بازگشت -1;

	size_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(size_from_bitmap);

	free_bitmap_index(bitmap_git);
	بازگشت 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
ایستا تهی prepare_maximal_independent(ساختار rev_info *revs)
{
	ساختار commit_list *c;

	اگر (!revs->maximal_only)
		بازگشت;

	برای (c = revs->commits; c; c = c->next) {
		اگر (c->item->object.flags & UNINTERESTING)
			بازگشت;
	}

	اگر (revs->limited ||
	    revs->topo_order ||
	    revs->first_parent_only ||
	    revs->reverse ||
	    revs->max_شمار >= 0 ||
	    revs->skip_شمار >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_data.nr ||
	    revs->شمار ||
	    revs->چپ_راست ||
	    revs->boundary ||
	    revs->tag_objects ||
	    revs->درخت_objects ||
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
	    revs->سطر_level_traverse)
		بازگشت;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

صحیح cmd_rev_list(صحیح argc,
		 ثابت نویسه **argv,
		 ثابت نویسه *prefix,
		 ساختار repository *repo)
{
	ساختار rev_info revs;
	ساختار rev_list_info info;
	ساختار setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	صحیح i;
	صحیح bisect_list = 0;
	صحیح bisect_show_vars = 0;
	صحیح bisect_find_all = 0;
	صحیح use_bitmap_index = 0;
	صحیح filter_provided_objects = 0;
	ثابت نویسه *show_progress = پوچ;
	صحیح ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, پوچ);
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
	برای (i = 1; i < argc; i++) {
		ثابت نویسه *arg = argv[i];
		اگر (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} وگرنه اگر (skip_prefix(arg, "--missing=", &arg)) {
			parse_missing_action_مقدار(repo, arg);
		} وگرنه اگر (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} وگرنه اگر (!strcmp(arg, "-z")) {
			سطر_term = '\0';
			info_term = '\0';
		}
	}

	اگر (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	اگر (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = setup_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, اندازه(info));
	info.revs = &revs;
	اگر (revs.bisect)
		bisect_list = 1;

	اگر (revs.diffopt.flags.quick)
		info.flags |= REV_LIST_QUIET;
	برای (i = 1 ; i < argc; i++) {
		ثابت نویسه *arg = argv[i];

		اگر (!strcmp(arg, "--header")) {
			revs.verbose_header = 1;
			ادامه;
		}
		اگر (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			ادامه;
		}
		اگر (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			ادامه;
		}
		اگر (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_find_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			ادامه;
		}
		اگر (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			ادامه;
		}
		اگر (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_index = 1;
			ادامه;
		}
		اگر (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			برو cleanup;
		}
		اگر (skip_prefix(arg, "--progress=", &arg)) {
			show_progress = arg;
			ادامه;
		}
		اگر (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			ادامه;
		}
		اگر (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			ادامه;
		}

		اگر (!strcmp(arg, "--exclude-promisor-objects"))
			ادامه; /* already handled above */
		اگر (skip_prefix(arg, "--missing=", &arg))
			ادامه; /* already handled above */

		اگر (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_names = 0;
			ادامه;
		}

		اگر (!strcmp(arg, ("--object-names"))) {
			arg_show_object_names = 1;
			ادامه;
		}

		اگر (!strcmp(arg, ("--commit-header"))) {
			revs.include_header = 1;
			ادامه;
		}

		اگر (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_header = 0;
			ادامه;
		}

		اگر (skip_prefix(arg, "--disk-usage", &arg)) {
			اگر (*arg == '=') {
				اگر (!strcmp(++arg, "human")) {
					human_readable = 1;
				} وگرنه
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} وگرنه اگر (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			ادامه;
		}

		اگر (!strcmp(arg, "--missing-only"))
			ادامه;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	اگر (!سطر_term) {
		اگر (revs.graph || revs.verbose_header || show_disk_usage ||
		    info.show_timestamp || info.header_prefix || bisect_list ||
		    use_bitmap_index || revs.edge_hint || revs.چپ_راست ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	اگر (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_header = 1;
	اگر (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		اگر (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_header)
			info.header_prefix = "";
		وگرنه
			info.header_prefix = "commit ";
	}
	وگرنه اگر (revs.verbose_header)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	اگر ((!revs.commits && reflog_walk_empty(revs.reflog_info) &&
	     (!(revs.tag_objects || revs.درخت_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_درون_داد_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	اگر (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	اگر (revs.شمار &&
	    (revs.tag_objects || revs.درخت_objects || revs.blob_objects) &&
	    (revs.چپ_راست || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.شمار, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_buffer = (revs.verbose_header ||
			      revs.grep_filter.pattern_list ||
			      revs.grep_filter.header_list);
	اگر (bisect_list)
		revs.limited = 1;

	اگر (show_progress)
		progress = start_delayed_progress(the_repository,
						  show_progress, 0);

	اگر (use_bitmap_index) {
		اگر (!try_bitmap_شمار(&revs, filter_provided_objects))
			برو cleanup;
		اگر (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			برو cleanup;
		اگر (!try_bitmap_traversal(&revs, filter_provided_objects))
			برو cleanup;
	}

	اگر (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	اگر (revs.درخت_objects)
		mark_edges_uninteresting(&revs, show_edge, 0);

	اگر (bisect_list) {
		صحیح reaches, all;
		unsigned bisect_flags = 0;

		اگر (bisect_find_all)
			bisect_flags |= FIND_BISECTION_ALL;

		اگر (revs.first_parent_only)
			bisect_flags |= FIND_BISECTION_FIRST_PARENT_ONLY;

		find_bisection(&revs.commits, &reaches, &all, bisect_flags);

		اگر (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			برو cleanup;
		}
	}

	اگر (filter_provided_objects) {
		ساختار commit_list *c;
		برای (i = 0; i < revs.pending.nr; i++) {
			ساختار object_array_entry *pending = revs.pending.objects + i;
			pending->item->flags |= NOT_USER_GIVEN;
		}
		برای (c = revs.commits; c; c = c->next)
			c->item->object.flags |= NOT_USER_GIVEN;
	}

	اگر (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_SIZE);
	اگر (should_collect_missing()) {
		ساختار oidset_iter iter;
		ساختار object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_SIZE);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		وقتی ((oid = oidset_iter_next(&iter)))
			add_missing_object_entry(oid, پوچ, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : پوچ));

	اگر (arg_print_omitted) {
		ساختار oidset_iter iter;
		ساختار object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		وقتی ((oid = oidset_iter_next(&iter)))
			printf("~%s\n", oid_to_hex(oid));
		oidset_clear(&omitted_objects);
	}
	اگر (should_collect_missing()) {
		ساختار missing_objects_map_entry *entry;
		ساختار oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		وقتی ((entry = oidmap_iter_next(&iter))) {
			print_missing_object(entry, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_entry_free);
	}

	stop_progress(&progress);

	اگر (revs.شمار) {
		اگر (revs.چپ_راست && revs.cherry_mark)
			printf("%d\t%d\t%d\n", revs.شمار_چپ, revs.شمار_راست, revs.شمار_same);
		وگرنه اگر (revs.چپ_راست)
			printf("%d\t%d\n", revs.شمار_چپ, revs.شمار_راست);
		وگرنه اگر (revs.cherry_mark)
			printf("%d\t%d\n", revs.شمار_چپ + revs.شمار_راست, revs.شمار_same);
		وگرنه
			printf("%d\n", revs.شمار_چپ + revs.شمار_راست);
	}

	اگر (show_disk_usage)
		print_disk_usage(total_disk_usage);

cleanup:
	release_revisions(&revs);
	بازگشت ret;
}
