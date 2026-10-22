#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "commit.h"
#ضمن "diff.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "revision.h"
#ضمن "list-objects.h"
#ضمن "list-objects-filter-options.h"
#ضمن "object.h"
#ضمن "object-name.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "pack-bitmap.h"
#ضمن "parse-options.h"
#ضمن "log-tree.h"
#ضمن "graph.h"
#ضمن "bisect.h"
#ضمن "progress.h"
#ضمن "reflog-walk.h"
#ضمن "oidset.h"
#ضمن "oidmap.h"
#ضمن "packfile.h"
#ضمن "commit-reach.h"
#ضمن "quote.h"
#ضمن "strbuf.h"

بنية rev_list_info {
	بنية rev_info *revs;
	صحيح flags;
	صحيح show_timestamp;
	صحيح hdr_termination;
	ثابت حرف *header_prefix;
};

ساكن ثابت حرف rev_list_usage[] =
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

ساكن بنية progress *progress;
ساكن unsigned progress_counter;

ساكن بنية oidset omitted_objects;
ساكن صحيح arg_print_omitted; /* print objects omitted by filter */

بنية missing_objects_map_entry {
	بنية oidmap_entry entry;
	حرف *path;
	unsigned type;
};

ساكن فراغ missing_objects_map_entry_free(فراغ *e)
{
	بنية missing_objects_map_entry *entry =
		container_of(e, بنية missing_objects_map_entry, entry);

	حرر(entry->path);
	حرر(entry);
}

ساكن بنية oidmap missing_objects;
تعداد missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ساكن تعداد missing_action arg_missing_action;
ساكن صحيح arg_missing_only;

ساكن مضمن صحيح should_collect_missing(فراغ)
{
	ܦܢܐ arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
ساكن صحيح arg_show_object_names = 1;

#define DEFAULT_OIDSET_SIZE     (16*1024)

ساكن حرف line_term = '\n';
ساكن حرف info_term = ' ';

ساكن صحيح show_disk_usage;
ساكن off_t total_disk_usage;
ساكن صحيح human_readable;

ساكن off_t get_object_disk_usage(بنية object *obj)
{
	off_t size;
	بنية object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &size;
	ܐܢ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_to_hex(&obj->oid));
	ܦܢܐ size;
}

ساكن فراغ add_missing_object_entry(بنية object_id *oid, ثابت حرف *path,
				     unsigned type)
{
	بنية missing_objects_map_entry *entry;

	ܐܢ (oidmap_get(&missing_objects, oid))
		ܦܢܐ;

	CALLOC_ARRAY(entry, 1);
	entry->entry.oid = *oid;
	entry->type = type;
	ܐܢ (path)
		entry->path = xstrdup(path);
	oidmap_put(&missing_objects, entry);
}

ساكن فراغ print_missing_object(بنية missing_objects_map_entry *entry,
				 صحيح print_missing_info)
{
	بنية strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	ܐܢ (arg_missing_only && line_term)
		printf("%s", oid_to_hex(&entry->entry.oid));
	ܐܚܪܢܐ ܐܢ (line_term)
		printf("?%s", oid_to_hex(&entry->entry.oid));
	ܐܚܪܢܐ
		printf("%s%cmissing=yes", oid_to_hex(&entry->entry.oid),
		       info_term);

	ܐܢ (!print_missing_info) {
		putchar(line_term);
		ܦܢܐ;
	}

	ܐܢ (entry->path && *entry->path) {
		strbuf_addf(&sb, "%cpath=", info_term);

		ܐܢ (line_term) {
			بنية strbuf path = STRBUF_INIT;

			quote_path(entry->path, عدم, &path, QUOTE_PATH_QUOTE_SP);
			strbuf_addbuf(&sb, &path);

			strbuf_release(&path);
		} ܐܚܪܢܐ {
			strbuf_addstr(&sb, entry->path);
		}
	}
	ܐܢ (entry->type)
		strbuf_addf(&sb, "%ctype=%s", info_term, type_name(entry->type));

	fwrite(sb.buf, حجم(حرف), sb.len, stdout);
	putchar(line_term);

	strbuf_release(&sb);
}

ساكن مضمن فراغ finish_object__ma(بنية object *obj, ثابت حرف *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	بدل (arg_missing_action) {
	حالة MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		ܦܢܐ;

	حالة MA_ALLOW_ANY:
		ܦܢܐ;

	حالة MA_PRINT:
	حالة MA_PRINT_INFO:
		add_missing_object_entry(&obj->oid, name, obj->type);
		ܦܢܐ;

	حالة MA_ALLOW_PROMISOR:
		ܐܢ (is_promisor_object(the_repository, &obj->oid))
			ܦܢܐ;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		ܦܢܐ;

	افتراضي:
		BUG("unhandled missing_action");
		ܦܢܐ;
	}
}

ساكن فراغ finish_commit(بنية commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = عدم;
	free_commit_buffer(the_repository->parsed_objects,
			   commit);
}

ساكن فراغ show_commit(بنية commit *commit, فراغ *data)
{
	بنية rev_list_info *info = data;
	بنية rev_info *revs = info->revs;

	display_progress(progress, ++progress_counter);

	ܐܢ (revs->do_not_die_on_missing_objects &&
	    oidset_contains(&revs->missing_commits, &commit->object.oid)) {
		finish_object__ma(&commit->object, عدم);
		ܦܢܐ;
	}

	ܐܢ (arg_missing_only) {
		finish_commit(commit);
		ܦܢܐ;
	}

	ܐܢ (show_disk_usage)
		total_disk_usage += get_object_disk_usage(&commit->object);

	ܐܢ (info->flags & REV_LIST_QUIET) {
		finish_commit(commit);
		ܦܢܐ;
	}

	graph_show_commit(revs->graph);

	ܐܢ (revs->count) {
		ܐܢ (commit->object.flags & PATCHSAME)
			revs->count_same++;
		ܐܚܪܢܐ ܐܢ (commit->object.flags & SYMMETRIC_LEFT)
			revs->count_left++;
		ܐܚܪܢܐ
			revs->count_right++;
		finish_commit(commit);
		ܦܢܐ;
	}

	ܐܢ (info->show_timestamp)
		printf("%"PRItime" ", commit->date);
	ܐܢ (info->header_prefix)
		fputs(info->header_prefix, stdout);

	ܐܢ (revs->include_header) {
		ܐܢ (!revs->graph && line_term)
			fputs(get_revision_mark(revs, commit), stdout);
		ܐܢ (revs->abbrev_commit && revs->abbrev)
			fputs(repo_find_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		ܐܚܪܢܐ
			fputs(oid_to_hex(&commit->object.oid), stdout);

		ܐܢ (!line_term) {
			ܐܢ (commit->object.flags & BOUNDARY)
				printf("%cboundary=yes", info_term);
		}
	}
	ܐܢ (revs->print_parents) {
		بنية commit_list *parents = commit->parents;
		ܟܡܐ (parents) {
			printf(" %s", oid_to_hex(&parents->item->object.oid));
			parents = parents->next;
		}
	}
	ܐܢ (revs->children.name) {
		بنية commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		ܟܡܐ (children) {
			printf(" %s", oid_to_hex(&children->item->object.oid));
			children = children->next;
		}
	}
	show_decorations(revs, commit);
	ܐܢ (revs->commit_format == CMIT_FMT_ONELINE)
		putchar(' ');
	ܐܚܪܢܐ ܐܢ (revs->include_header)
		putchar(line_term);

	ܐܢ (revs->verbose_header) {
		بنية strbuf buf = STRBUF_INIT;
		بنية pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_mode = revs->date_mode;
		ctx.date_mode_explicit = revs->date_mode_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		ܐܢ (buf.len) {
			ܐܢ (revs->commit_format != CMIT_FMT_ONELINE)
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
			ܐܢ (buf.len && buf.buf[buf.len - 1] == '\n')
				graph_show_padding(revs->graph);
			putchar(info->hdr_termination);
		} ܐܚܪܢܐ {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			ܐܢ (graph_show_remainder(revs->graph))
				putchar('\n');
			ܐܢ (revs->commit_format == CMIT_FMT_ONELINE)
				putchar('\n');
		}
		strbuf_release(&buf);
	} ܐܚܪܢܐ {
		ܐܢ (graph_show_remainder(revs->graph))
			putchar('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	finish_commit(commit);
}

ساكن صحيح finish_object(بنية object *obj, ثابت حرف *name, فراغ *cb_data)
{
	بنية rev_list_info *info = cb_data;
	ܐܢ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, عدم, 0) < 0) {
		finish_object__ma(obj, name);
		ܦܢܐ 1;
	}
	ܐܢ (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	ܦܢܐ 0;
}

ساكن فراغ show_object(بنية object *obj, ثابت حرف *name, فراغ *cb_data)
{
	بنية rev_list_info *info = cb_data;
	بنية rev_info *revs = info->revs;

	ܐܢ (finish_object(obj, name, cb_data))
		ܦܢܐ;
	display_progress(progress, ++progress_counter);
	ܐܢ (arg_missing_only)
		ܦܢܐ;
	ܐܢ (show_disk_usage)
		total_disk_usage += get_object_disk_usage(obj);
	ܐܢ (info->flags & REV_LIST_QUIET)
		ܦܢܐ;

	ܐܢ (revs->count) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->count_right++;
		ܦܢܐ;
	}

	printf("%s", oid_to_hex(&obj->oid));

	ܐܢ (arg_show_object_names) {
		ܐܢ (line_term) {
			putchar(info_term);
			لكل (ثابت حرف *p = name; *p && *p != '\n'; p++)
				putchar(*p);
		} ܐܚܪܢܐ ܐܢ (*name) {
			printf("%cpath=%s", info_term, name);
		}
	}

	putchar(line_term);
}

ساكن فراغ show_edge(بنية commit *commit)
{
	printf("-%s\n", oid_to_hex(&commit->object.oid));
}

ساكن فراغ print_var_str(ثابت حرف *var, ثابت حرف *val)
{
	printf("%s='%s'\n", var, val);
}

ساكن فراغ print_var_int(ثابت حرف *var, صحيح val)
{
	printf("%s=%d\n", var, val);
}

ساكن صحيح show_bisect_vars(بنية rev_list_info *info, صحيح reaches, صحيح all)
{
	صحيح cnt, flags = info->flags;
	حرف hex[GIT_MAX_HEXSZ + 1] = "";
	بنية commit_list *tried;
	بنية rev_info *revs = info->revs;

	ܐܢ (!revs->commits)
		ܦܢܐ 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       عدم, عدم);

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
	ܐܢ (cnt < reaches)
		cnt = reaches;

	ܐܢ (revs->commits)
		oid_to_hex_r(hex, &revs->commits->item->object.oid);

	ܐܢ (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		printf("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_steps(all));

	ܦܢܐ 0;
}

ساكن صحيح show_object_fast(
	ثابت بنية object_id *oid,
	تعداد object_type type UNUSED,
	صحيح exclude UNUSED,
	uint32_t name_hash UNUSED,
	بنية packed_git *found_pack UNUSED,
	off_t found_offset UNUSED,
	فراغ *payload UNUSED)
{
	fprintf(stdout, "%s\n", oid_to_hex(oid));
	ܦܢܐ 0;
}

ساكن فراغ print_disk_usage(off_t size)
{
	بنية strbuf sb = STRBUF_INIT;
	ܐܢ (human_readable)
		strbuf_humanise_bytes(&sb, size);
	ܐܚܪܢܐ
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)size);
	puts(sb.buf);
	strbuf_release(&sb);
}

ساكن مضمن صحيح parse_missing_action_value(بنية repository *repo,
					     ثابت حرف *value)
{
	ܐܢ (!strcmp(value, "error")) {
		arg_missing_action = MA_ERROR;
		ܦܢܐ 1;
	}

	ܐܢ (!strcmp(value, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		ܦܢܐ 1;
	}

	ܐܢ (!strcmp(value, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		ܦܢܐ 1;
	}

	ܐܢ (!strcmp(value, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		ܦܢܐ 1;
	}

	ܐܢ (!strcmp(value, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		ܦܢܐ 1;
	}

	ܦܢܐ 0;
}

ساكن صحيح try_bitmap_count(بنية rev_info *revs,
			    صحيح filter_provided_objects)
{
	uint32_t commit_count = 0,
		 tag_count = 0,
		 tree_count = 0,
		 blob_count = 0;
	صحيح max_count;
	بنية bitmap_index *bitmap_git;

	/* This function only handles counting, not general traversal. */
	ܐܢ (!revs->count)
		ܦܢܐ -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	ܐܢ (revs->left_right || revs->cherry_mark)
		ܦܢܐ -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	ܐܢ (revs->max_count >= 0 &&
	    (revs->tag_objects || revs->tree_objects || revs->blob_objects))
		ܦܢܐ -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_count = revs->max_count;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ܐܢ (!bitmap_git)
		ܦܢܐ -1;

	count_bitmap_commit_list(bitmap_git, &commit_count,
				 revs->tree_objects ? &tree_count : عدم,
				 revs->blob_objects ? &blob_count : عدم,
				 revs->tag_objects ? &tag_count : عدم);
	ܐܢ (max_count >= 0 && max_count < commit_count)
		commit_count = max_count;

	printf("%d\n", commit_count + tree_count + blob_count + tag_count);
	free_bitmap_index(bitmap_git);
	ܦܢܐ 0;
}

ساكن صحيح try_bitmap_traversal(بنية rev_info *revs,
				صحيح filter_provided_objects)
{
	بنية bitmap_index *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	ܐܢ (revs->max_count >= 0)
		ܦܢܐ -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	ܐܢ (revs->left_right)
		ܦܢܐ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ܐܢ (!bitmap_git)
		ܦܢܐ -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_index(bitmap_git);
	ܦܢܐ 0;
}

ساكن صحيح try_bitmap_disk_usage(بنية rev_info *revs,
				 صحيح filter_provided_objects)
{
	بنية bitmap_index *bitmap_git;
	off_t size_from_bitmap;

	ܐܢ (!show_disk_usage)
		ܦܢܐ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ܐܢ (!bitmap_git)
		ܦܢܐ -1;

	size_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(size_from_bitmap);

	free_bitmap_index(bitmap_git);
	ܦܢܐ 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
ساكن فراغ prepare_maximal_independent(بنية rev_info *revs)
{
	بنية commit_list *c;

	ܐܢ (!revs->maximal_only)
		ܦܢܐ;

	لكل (c = revs->commits; c; c = c->next) {
		ܐܢ (c->item->object.flags & UNINTERESTING)
			ܦܢܐ;
	}

	ܐܢ (revs->limited ||
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
		ܦܢܐ;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

صحيح cmd_rev_list(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *prefix,
		 بنية repository *repo)
{
	بنية rev_info revs;
	بنية rev_list_info info;
	بنية setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	صحيح i;
	صحيح bisect_list = 0;
	صحيح bisect_show_vars = 0;
	صحيح bisect_find_all = 0;
	صحيح use_bitmap_index = 0;
	صحيح filter_provided_objects = 0;
	ثابت حرف *show_progress = عدم;
	صحيح ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, عدم);
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
	لكل (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		ܐܢ (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} ܐܚܪܢܐ ܐܢ (skip_prefix(arg, "--missing=", &arg)) {
			parse_missing_action_value(repo, arg);
		} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "-z")) {
			line_term = '\0';
			info_term = '\0';
		}
	}

	ܐܢ (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	ܐܢ (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = setup_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, حجم(info));
	info.revs = &revs;
	ܐܢ (revs.bisect)
		bisect_list = 1;

	ܐܢ (revs.diffopt.flags.quick)
		info.flags |= REV_LIST_QUIET;
	لكل (i = 1 ; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		ܐܢ (!strcmp(arg, "--header")) {
			revs.verbose_header = 1;
			استمر;
		}
		ܐܢ (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			استمر;
		}
		ܐܢ (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			استمر;
		}
		ܐܢ (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_find_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			استمر;
		}
		ܐܢ (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			استمر;
		}
		ܐܢ (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_index = 1;
			استمر;
		}
		ܐܢ (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			اذهب cleanup;
		}
		ܐܢ (skip_prefix(arg, "--progress=", &arg)) {
			show_progress = arg;
			استمر;
		}
		ܐܢ (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			استمر;
		}
		ܐܢ (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			استمر;
		}

		ܐܢ (!strcmp(arg, "--exclude-promisor-objects"))
			استمر; /* already handled above */
		ܐܢ (skip_prefix(arg, "--missing=", &arg))
			استمر; /* already handled above */

		ܐܢ (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_names = 0;
			استمر;
		}

		ܐܢ (!strcmp(arg, ("--object-names"))) {
			arg_show_object_names = 1;
			استمر;
		}

		ܐܢ (!strcmp(arg, ("--commit-header"))) {
			revs.include_header = 1;
			استمر;
		}

		ܐܢ (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_header = 0;
			استمر;
		}

		ܐܢ (skip_prefix(arg, "--disk-usage", &arg)) {
			ܐܢ (*arg == '=') {
				ܐܢ (!strcmp(++arg, "human")) {
					human_readable = 1;
				} ܐܚܪܢܐ
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} ܐܚܪܢܐ ܐܢ (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			استمر;
		}

		ܐܢ (!strcmp(arg, "--missing-only"))
			استمر;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	ܐܢ (!line_term) {
		ܐܢ (revs.graph || revs.verbose_header || show_disk_usage ||
		    info.show_timestamp || info.header_prefix || bisect_list ||
		    use_bitmap_index || revs.edge_hint || revs.left_right ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	ܐܢ (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_header = 1;
	ܐܢ (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		ܐܢ (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_header)
			info.header_prefix = "";
		ܐܚܪܢܐ
			info.header_prefix = "commit ";
	}
	ܐܚܪܢܐ ܐܢ (revs.verbose_header)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	ܐܢ ((!revs.commits && reflog_walk_empty(revs.reflog_info) &&
	     (!(revs.tag_objects || revs.tree_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_input_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	ܐܢ (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	ܐܢ (revs.count &&
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
	ܐܢ (bisect_list)
		revs.limited = 1;

	ܐܢ (show_progress)
		progress = start_delayed_progress(the_repository,
						  show_progress, 0);

	ܐܢ (use_bitmap_index) {
		ܐܢ (!try_bitmap_count(&revs, filter_provided_objects))
			اذهب cleanup;
		ܐܢ (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			اذهب cleanup;
		ܐܢ (!try_bitmap_traversal(&revs, filter_provided_objects))
			اذهب cleanup;
	}

	ܐܢ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	ܐܢ (revs.tree_objects)
		mark_edges_uninteresting(&revs, show_edge, 0);

	ܐܢ (bisect_list) {
		صحيح reaches, all;
		unsigned bisect_flags = 0;

		ܐܢ (bisect_find_all)
			bisect_flags |= FIND_BISECTION_ALL;

		ܐܢ (revs.first_parent_only)
			bisect_flags |= FIND_BISECTION_FIRST_PARENT_ONLY;

		find_bisection(&revs.commits, &reaches, &all, bisect_flags);

		ܐܢ (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			اذهب cleanup;
		}
	}

	ܐܢ (filter_provided_objects) {
		بنية commit_list *c;
		لكل (i = 0; i < revs.pending.nr; i++) {
			بنية object_array_entry *pending = revs.pending.objects + i;
			pending->item->flags |= NOT_USER_GIVEN;
		}
		لكل (c = revs.commits; c; c = c->next)
			c->item->object.flags |= NOT_USER_GIVEN;
	}

	ܐܢ (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_SIZE);
	ܐܢ (should_collect_missing()) {
		بنية oidset_iter iter;
		بنية object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_SIZE);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		ܟܡܐ ((oid = oidset_iter_next(&iter)))
			add_missing_object_entry(oid, عدم, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : عدم));

	ܐܢ (arg_print_omitted) {
		بنية oidset_iter iter;
		بنية object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		ܟܡܐ ((oid = oidset_iter_next(&iter)))
			printf("~%s\n", oid_to_hex(oid));
		oidset_clear(&omitted_objects);
	}
	ܐܢ (should_collect_missing()) {
		بنية missing_objects_map_entry *entry;
		بنية oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		ܟܡܐ ((entry = oidmap_iter_next(&iter))) {
			print_missing_object(entry, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_entry_free);
	}

	stop_progress(&progress);

	ܐܢ (revs.count) {
		ܐܢ (revs.left_right && revs.cherry_mark)
			printf("%d\t%d\t%d\n", revs.count_left, revs.count_right, revs.count_same);
		ܐܚܪܢܐ ܐܢ (revs.left_right)
			printf("%d\t%d\n", revs.count_left, revs.count_right);
		ܐܚܪܢܐ ܐܢ (revs.cherry_mark)
			printf("%d\t%d\n", revs.count_left + revs.count_right, revs.count_same);
		ܐܚܪܢܐ
			printf("%d\n", revs.count_left + revs.count_right);
	}

	ܐܢ (show_disk_usage)
		print_disk_usage(total_disk_usage);

cleanup:
	release_revisions(&revs);
	ܦܢܐ ret;
}
