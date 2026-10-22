#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "list-objects.h"
#অন্তর্ভাব "list-objects-filter-options.h"
#অন্তর্ভাব "object.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "pack-bitmap.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "log-tree.h"
#অন্তর্ভাব "graph.h"
#অন্তর্ভাব "bisect.h"
#অন্তর্ভাব "progress.h"
#অন্তর্ভাব "reflog-walk.h"
#অন্তর্ভাব "oidset.h"
#অন্তর্ভাব "oidmap.h"
#অন্তর্ভাব "packfile.h"
#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "strbuf.h"

কাঠামো rev_list_info {
	কাঠামো rev_info *revs;
	পূর্ণসংখ্যা flags;
	পূর্ণসংখ্যা show_timestamp;
	পূর্ণসংখ্যা hdr_termination;
	ধ্রুবক অক্ষর *header_prefix;
};

স্থির ধ্রুবক অক্ষর rev_list_usage[] =
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

স্থির কাঠামো progress *progress;
স্থির unsigned progress_গণক;

স্থির কাঠামো oidset omitted_objects;
স্থির পূর্ণসংখ্যা arg_print_omitted; /* print objects omitted by filter */

কাঠামো missing_objects_map_entry {
	কাঠামো oidmap_entry entry;
	অক্ষর *path;
	unsigned type;
};

স্থির শূন্য missing_objects_map_entry_free(শূন্য *e)
{
	কাঠামো missing_objects_map_entry *entry =
		container_of(e, কাঠামো missing_objects_map_entry, entry);

	মুক্ত(entry->path);
	মুক্ত(entry);
}

স্থির কাঠামো oidmap missing_objects;
গণনা missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
স্থির গণনা missing_action arg_missing_action;
স্থির পূর্ণসংখ্যা arg_missing_only;

স্থির অন্তঃরেখ পূর্ণসংখ্যা should_collect_missing(শূন্য)
{
	ফেরত arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
স্থির পূর্ণসংখ্যা arg_show_object_names = 1;

#define DEFAULT_OIDSET_আকার     (16*1024)

স্থির অক্ষর সারি_term = '\n';
স্থির অক্ষর info_term = ' ';

স্থির পূর্ণসংখ্যা show_disk_usage;
স্থির off_t যোগ_disk_usage;
স্থির পূর্ণসংখ্যা human_readable;

স্থির off_t get_object_disk_usage(কাঠামো object *obj)
{
	off_t আকার_2;
	কাঠামো object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &আকার_2;
	যদি (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_to_hex(&obj->oid));
	ফেরত আকার_2;
}

স্থির শূন্য add_missing_object_entry(কাঠামো object_id *oid, ধ্রুবক অক্ষর *path,
				     unsigned type)
{
	কাঠামো missing_objects_map_entry *entry;

	যদি (oidmap_get(&missing_objects, oid))
		ফেরত;

	CALLOC_ARRAY(entry, 1);
	entry->entry.oid = *oid;
	entry->type = type;
	যদি (path)
		entry->path = xstrdup(path);
	oidmap_put(&missing_objects, entry);
}

স্থির শূন্য print_missing_object(কাঠামো missing_objects_map_entry *entry,
				 পূর্ণসংখ্যা print_missing_info)
{
	কাঠামো strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	যদি (arg_missing_only && সারি_term)
		printf("%s", oid_to_hex(&entry->entry.oid));
	অন্যথা যদি (সারি_term)
		printf("?%s", oid_to_hex(&entry->entry.oid));
	অন্যথা
		printf("%s%cmissing=yes", oid_to_hex(&entry->entry.oid),
		       info_term);

	যদি (!print_missing_info) {
		putchar(সারি_term);
		ফেরত;
	}

	যদি (entry->path && *entry->path) {
		strbuf_addf(&sb, "%cpath=", info_term);

		যদি (সারি_term) {
			কাঠামো strbuf path = STRBUF_INIT;

			quote_path(entry->path, ফাঁকা, &path, QUOTE_PATH_QUOTE_SP);
			strbuf_addbuf(&sb, &path);

			strbuf_release(&path);
		} অন্যথা {
			strbuf_addstr(&sb, entry->path);
		}
	}
	যদি (entry->type)
		strbuf_addf(&sb, "%ctype=%s", info_term, type_name(entry->type));

	fwrite(sb.buf, আকার(অক্ষর), sb.len, stdout);
	putchar(সারি_term);

	strbuf_release(&sb);
}

স্থির অন্তঃরেখ শূন্য finish_object__ma(কাঠামো object *obj, ধ্রুবক অক্ষর *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	নির্বাচন (arg_missing_action) {
	ক্ষেত্র MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		ফেরত;

	ক্ষেত্র MA_ALLOW_ANY:
		ফেরত;

	ক্ষেত্র MA_PRINT:
	ক্ষেত্র MA_PRINT_INFO:
		add_missing_object_entry(&obj->oid, name, obj->type);
		ফেরত;

	ক্ষেত্র MA_ALLOW_PROMISOR:
		যদি (is_promisor_object(the_repository, &obj->oid))
			ফেরত;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		ফেরত;

	পূর্বনির্ধারিত:
		BUG("unhandled missing_action");
		ফেরত;
	}
}

স্থির শূন্য finish_commit(কাঠামো commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = ফাঁকা;
	free_commit_বাফার(the_repository->parsed_objects,
			   commit);
}

স্থির শূন্য show_commit(কাঠামো commit *commit, শূন্য *তথ্য)
{
	কাঠামো rev_list_info *info = তথ্য;
	কাঠামো rev_info *revs = info->revs;

	display_progress(progress, ++progress_গণক);

	যদি (revs->do_not_die_on_missing_objects &&
	    oidset_contains(&revs->missing_commits, &commit->object.oid)) {
		finish_object__ma(&commit->object, ফাঁকা);
		ফেরত;
	}

	যদি (arg_missing_only) {
		finish_commit(commit);
		ফেরত;
	}

	যদি (show_disk_usage)
		যোগ_disk_usage += get_object_disk_usage(&commit->object);

	যদি (info->flags & REV_LIST_QUIET) {
		finish_commit(commit);
		ফেরত;
	}

	graph_show_commit(revs->graph);

	যদি (revs->গুনতি) {
		যদি (commit->object.flags & PATCHSAME)
			revs->গুনতি_same++;
		অন্যথা যদি (commit->object.flags & SYMMETRIC_বাম)
			revs->গুনতি_বাম++;
		অন্যথা
			revs->গুনতি_ডান++;
		finish_commit(commit);
		ফেরত;
	}

	যদি (info->show_timestamp)
		printf("%"PRItime" ", commit->date);
	যদি (info->header_prefix)
		fputs(info->header_prefix, stdout);

	যদি (revs->include_header) {
		যদি (!revs->graph && সারি_term)
			fputs(get_revision_mark(revs, commit), stdout);
		যদি (revs->abbrev_commit && revs->abbrev)
			fputs(repo_find_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		অন্যথা
			fputs(oid_to_hex(&commit->object.oid), stdout);

		যদি (!সারি_term) {
			যদি (commit->object.flags & BOUNDARY)
				printf("%cboundary=yes", info_term);
		}
	}
	যদি (revs->print_parents) {
		কাঠামো commit_list *parents = commit->parents;
		যখন (parents) {
			printf(" %s", oid_to_hex(&parents->item->object.oid));
			parents = parents->পরবর্তী;
		}
	}
	যদি (revs->children.name) {
		কাঠামো commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		যখন (children) {
			printf(" %s", oid_to_hex(&children->item->object.oid));
			children = children->পরবর্তী;
		}
	}
	show_decorations(revs, commit);
	যদি (revs->commit_format == CMIT_FMT_ONELINE)
		putchar(' ');
	অন্যথা যদি (revs->include_header)
		putchar(সারি_term);

	যদি (revs->verbose_header) {
		কাঠামো strbuf buf = STRBUF_INIT;
		কাঠামো pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_mode = revs->date_mode;
		ctx.date_mode_explicit = revs->date_mode_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		যদি (buf.len) {
			যদি (revs->commit_format != CMIT_FMT_ONELINE)
				graph_show_oneline(revs->graph);

			graph_show_commit_বার্তা(revs->graph, stdout, &buf);

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
			যদি (buf.len && buf.buf[buf.len - 1] == '\n')
				graph_show_padding(revs->graph);
			putchar(info->hdr_termination);
		} অন্যথা {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			যদি (graph_show_remainder(revs->graph))
				putchar('\n');
			যদি (revs->commit_format == CMIT_FMT_ONELINE)
				putchar('\n');
		}
		strbuf_release(&buf);
	} অন্যথা {
		যদি (graph_show_remainder(revs->graph))
			putchar('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	finish_commit(commit);
}

স্থির পূর্ণসংখ্যা finish_object(কাঠামো object *obj, ধ্রুবক অক্ষর *name, শূন্য *cb_তথ্য)
{
	কাঠামো rev_list_info *info = cb_তথ্য;
	যদি (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, ফাঁকা, 0) < 0) {
		finish_object__ma(obj, name);
		ফেরত 1;
	}
	যদি (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	ফেরত 0;
}

স্থির শূন্য show_object(কাঠামো object *obj, ধ্রুবক অক্ষর *name, শূন্য *cb_তথ্য)
{
	কাঠামো rev_list_info *info = cb_তথ্য;
	কাঠামো rev_info *revs = info->revs;

	যদি (finish_object(obj, name, cb_তথ্য))
		ফেরত;
	display_progress(progress, ++progress_গণক);
	যদি (arg_missing_only)
		ফেরত;
	যদি (show_disk_usage)
		যোগ_disk_usage += get_object_disk_usage(obj);
	যদি (info->flags & REV_LIST_QUIET)
		ফেরত;

	যদি (revs->গুনতি) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->গুনতি_ডান++;
		ফেরত;
	}

	printf("%s", oid_to_hex(&obj->oid));

	যদি (arg_show_object_names) {
		যদি (সারি_term) {
			putchar(info_term);
			জন্য (ধ্রুবক অক্ষর *p = name; *p && *p != '\n'; p++)
				putchar(*p);
		} অন্যথা যদি (*name) {
			printf("%cpath=%s", info_term, name);
		}
	}

	putchar(সারি_term);
}

স্থির শূন্য show_edge(কাঠামো commit *commit)
{
	printf("-%s\n", oid_to_hex(&commit->object.oid));
}

স্থির শূন্য print_var_str(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *val)
{
	printf("%s='%s'\n", var, val);
}

স্থির শূন্য print_var_int(ধ্রুবক অক্ষর *var, পূর্ণসংখ্যা val)
{
	printf("%s=%d\n", var, val);
}

স্থির পূর্ণসংখ্যা show_bisect_vars(কাঠামো rev_list_info *info, পূর্ণসংখ্যা reaches, পূর্ণসংখ্যা all)
{
	পূর্ণসংখ্যা cnt, flags = info->flags;
	অক্ষর hex[GIT_MAX_HEXSZ + 1] = "";
	কাঠামো commit_list *tried;
	কাঠামো rev_info *revs = info->revs;

	যদি (!revs->commits)
		ফেরত 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       ফাঁকা, ফাঁকা);

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
	যদি (cnt < reaches)
		cnt = reaches;

	যদি (revs->commits)
		oid_to_hex_r(hex, &revs->commits->item->object.oid);

	যদি (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		printf("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_steps(all));

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা show_object_fast(
	ধ্রুবক কাঠামো object_id *oid,
	গণনা object_type type UNUSED,
	পূর্ণসংখ্যা exclude UNUSED,
	uint32_t name_hash UNUSED,
	কাঠামো packed_git *found_pack UNUSED,
	off_t found_offset UNUSED,
	শূন্য *payload UNUSED)
{
	fprintf(stdout, "%s\n", oid_to_hex(oid));
	ফেরত 0;
}

স্থির শূন্য print_disk_usage(off_t আকার_2)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	যদি (human_readable)
		strbuf_humanise_bytes(&sb, আকার_2);
	অন্যথা
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)আকার_2);
	puts(sb.buf);
	strbuf_release(&sb);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা parse_missing_action_মান(কাঠামো repository *repo,
					     ধ্রুবক অক্ষর *মান)
{
	যদি (!strcmp(মান, "error")) {
		arg_missing_action = MA_ERROR;
		ফেরত 1;
	}

	যদি (!strcmp(মান, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		ফেরত 1;
	}

	যদি (!strcmp(মান, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		ফেরত 1;
	}

	যদি (!strcmp(মান, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		ফেরত 1;
	}

	যদি (!strcmp(মান, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		ফেরত 1;
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা try_bitmap_গুনতি(কাঠামো rev_info *revs,
			    পূর্ণসংখ্যা filter_provided_objects)
{
	uint32_t commit_গুনতি = 0,
		 tag_গুনতি = 0,
		 বৃক্ষ_গুনতি = 0,
		 blob_গুনতি = 0;
	পূর্ণসংখ্যা max_গুনতি;
	কাঠামো bitmap_সূচক *bitmap_git;

	/* This function only handles counting, not general traversal. */
	যদি (!revs->গুনতি)
		ফেরত -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	যদি (revs->বাম_ডান || revs->cherry_mark)
		ফেরত -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	যদি (revs->max_গুনতি >= 0 &&
	    (revs->tag_objects || revs->বৃক্ষ_objects || revs->blob_objects))
		ফেরত -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_গুনতি = revs->max_গুনতি;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	যদি (!bitmap_git)
		ফেরত -1;

	গুনতি_bitmap_commit_list(bitmap_git, &commit_গুনতি,
				 revs->বৃক্ষ_objects ? &বৃক্ষ_গুনতি : ফাঁকা,
				 revs->blob_objects ? &blob_গুনতি : ফাঁকা,
				 revs->tag_objects ? &tag_গুনতি : ফাঁকা);
	যদি (max_গুনতি >= 0 && max_গুনতি < commit_গুনতি)
		commit_গুনতি = max_গুনতি;

	printf("%d\n", commit_গুনতি + বৃক্ষ_গুনতি + blob_গুনতি + tag_গুনতি);
	free_bitmap_সূচক(bitmap_git);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা try_bitmap_traversal(কাঠামো rev_info *revs,
				পূর্ণসংখ্যা filter_provided_objects)
{
	কাঠামো bitmap_সূচক *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	যদি (revs->max_গুনতি >= 0)
		ফেরত -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	যদি (revs->বাম_ডান)
		ফেরত -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	যদি (!bitmap_git)
		ফেরত -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_সূচক(bitmap_git);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা try_bitmap_disk_usage(কাঠামো rev_info *revs,
				 পূর্ণসংখ্যা filter_provided_objects)
{
	কাঠামো bitmap_সূচক *bitmap_git;
	off_t আকার_from_bitmap;

	যদি (!show_disk_usage)
		ফেরত -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	যদি (!bitmap_git)
		ফেরত -1;

	আকার_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(আকার_from_bitmap);

	free_bitmap_সূচক(bitmap_git);
	ফেরত 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
স্থির শূন্য prepare_maximal_independent(কাঠামো rev_info *revs)
{
	কাঠামো commit_list *c;

	যদি (!revs->maximal_only)
		ফেরত;

	জন্য (c = revs->commits; c; c = c->পরবর্তী) {
		যদি (c->item->object.flags & UNINTERESTING)
			ফেরত;
	}

	যদি (revs->limited ||
	    revs->topo_order ||
	    revs->first_parent_only ||
	    revs->reverse ||
	    revs->max_গুনতি >= 0 ||
	    revs->skip_গুনতি >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_তথ্য.nr ||
	    revs->গুনতি ||
	    revs->বাম_ডান ||
	    revs->boundary ||
	    revs->tag_objects ||
	    revs->বৃক্ষ_objects ||
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
	    revs->সারি_level_traverse)
		ফেরত;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

পূর্ণসংখ্যা cmd_rev_list(পূর্ণসংখ্যা argc,
		 ধ্রুবক অক্ষর **argv,
		 ধ্রুবক অক্ষর *prefix,
		 কাঠামো repository *repo)
{
	কাঠামো rev_info revs;
	কাঠামো rev_list_info info;
	কাঠামো setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	পূর্ণসংখ্যা i;
	পূর্ণসংখ্যা bisect_list = 0;
	পূর্ণসংখ্যা bisect_show_vars = 0;
	পূর্ণসংখ্যা bisect_find_all = 0;
	পূর্ণসংখ্যা use_bitmap_সূচক = 0;
	পূর্ণসংখ্যা filter_provided_objects = 0;
	ধ্রুবক অক্ষর *show_progress = ফাঁকা;
	পূর্ণসংখ্যা ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, ফাঁকা);
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
	জন্য (i = 1; i < argc; i++) {
		ধ্রুবক অক্ষর *arg = argv[i];
		যদি (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} অন্যথা যদি (skip_prefix(arg, "--missing=", &arg)) {
			parse_missing_action_মান(repo, arg);
		} অন্যথা যদি (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} অন্যথা যদি (!strcmp(arg, "-z")) {
			সারি_term = '\0';
			info_term = '\0';
		}
	}

	যদি (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	যদি (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = setup_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, আকার(info));
	info.revs = &revs;
	যদি (revs.bisect)
		bisect_list = 1;

	যদি (revs.diffopt.flags.quick)
		info.flags |= REV_LIST_QUIET;
	জন্য (i = 1 ; i < argc; i++) {
		ধ্রুবক অক্ষর *arg = argv[i];

		যদি (!strcmp(arg, "--header")) {
			revs.verbose_header = 1;
			চালু;
		}
		যদি (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			চালু;
		}
		যদি (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			চালু;
		}
		যদি (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_find_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			চালু;
		}
		যদি (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			চালু;
		}
		যদি (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_সূচক = 1;
			চালু;
		}
		যদি (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			যাও cleanup;
		}
		যদি (skip_prefix(arg, "--progress=", &arg)) {
			show_progress = arg;
			চালু;
		}
		যদি (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			চালু;
		}
		যদি (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			চালু;
		}

		যদি (!strcmp(arg, "--exclude-promisor-objects"))
			চালু; /* already handled above */
		যদি (skip_prefix(arg, "--missing=", &arg))
			চালু; /* already handled above */

		যদি (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_names = 0;
			চালু;
		}

		যদি (!strcmp(arg, ("--object-names"))) {
			arg_show_object_names = 1;
			চালু;
		}

		যদি (!strcmp(arg, ("--commit-header"))) {
			revs.include_header = 1;
			চালু;
		}

		যদি (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_header = 0;
			চালু;
		}

		যদি (skip_prefix(arg, "--disk-usage", &arg)) {
			যদি (*arg == '=') {
				যদি (!strcmp(++arg, "human")) {
					human_readable = 1;
				} অন্যথা
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} অন্যথা যদি (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			চালু;
		}

		যদি (!strcmp(arg, "--missing-only"))
			চালু;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	যদি (!সারি_term) {
		যদি (revs.graph || revs.verbose_header || show_disk_usage ||
		    info.show_timestamp || info.header_prefix || bisect_list ||
		    use_bitmap_সূচক || revs.edge_hint || revs.বাম_ডান ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	যদি (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_header = 1;
	যদি (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		যদি (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_header)
			info.header_prefix = "";
		অন্যথা
			info.header_prefix = "commit ";
	}
	অন্যথা যদি (revs.verbose_header)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	যদি ((!revs.commits && reflog_walk_empty(revs.reflog_info) &&
	     (!(revs.tag_objects || revs.বৃক্ষ_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_প্রবেশ_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	যদি (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	যদি (revs.গুনতি &&
	    (revs.tag_objects || revs.বৃক্ষ_objects || revs.blob_objects) &&
	    (revs.বাম_ডান || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.গুনতি, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_বাফার = (revs.verbose_header ||
			      revs.grep_filter.pattern_list ||
			      revs.grep_filter.header_list);
	যদি (bisect_list)
		revs.limited = 1;

	যদি (show_progress)
		progress = start_delayed_progress(the_repository,
						  show_progress, 0);

	যদি (use_bitmap_সূচক) {
		যদি (!try_bitmap_গুনতি(&revs, filter_provided_objects))
			যাও cleanup;
		যদি (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			যাও cleanup;
		যদি (!try_bitmap_traversal(&revs, filter_provided_objects))
			যাও cleanup;
	}

	যদি (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	যদি (revs.বৃক্ষ_objects)
		mark_edges_uninteresting(&revs, show_edge, 0);

	যদি (bisect_list) {
		পূর্ণসংখ্যা reaches, all;
		unsigned bisect_flags = 0;

		যদি (bisect_find_all)
			bisect_flags |= FIND_BISECTION_ALL;

		যদি (revs.first_parent_only)
			bisect_flags |= FIND_BISECTION_FIRST_PARENT_ONLY;

		find_bisection(&revs.commits, &reaches, &all, bisect_flags);

		যদি (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			যাও cleanup;
		}
	}

	যদি (filter_provided_objects) {
		কাঠামো commit_list *c;
		জন্য (i = 0; i < revs.pending.nr; i++) {
			কাঠামো object_array_entry *pending = revs.pending.objects + i;
			pending->item->flags |= NOT_USER_GIVEN;
		}
		জন্য (c = revs.commits; c; c = c->পরবর্তী)
			c->item->object.flags |= NOT_USER_GIVEN;
	}

	যদি (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_আকার);
	যদি (should_collect_missing()) {
		কাঠামো oidset_iter iter;
		কাঠামো object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_আকার);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		যখন ((oid = oidset_iter_পরবর্তী(&iter)))
			add_missing_object_entry(oid, ফাঁকা, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : ফাঁকা));

	যদি (arg_print_omitted) {
		কাঠামো oidset_iter iter;
		কাঠামো object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		যখন ((oid = oidset_iter_পরবর্তী(&iter)))
			printf("~%s\n", oid_to_hex(oid));
		oidset_clear(&omitted_objects);
	}
	যদি (should_collect_missing()) {
		কাঠামো missing_objects_map_entry *entry;
		কাঠামো oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		যখন ((entry = oidmap_iter_পরবর্তী(&iter))) {
			print_missing_object(entry, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_entry_free);
	}

	stop_progress(&progress);

	যদি (revs.গুনতি) {
		যদি (revs.বাম_ডান && revs.cherry_mark)
			printf("%d\t%d\t%d\n", revs.গুনতি_বাম, revs.গুনতি_ডান, revs.গুনতি_same);
		অন্যথা যদি (revs.বাম_ডান)
			printf("%d\t%d\n", revs.গুনতি_বাম, revs.গুনতি_ডান);
		অন্যথা যদি (revs.cherry_mark)
			printf("%d\t%d\n", revs.গুনতি_বাম + revs.গুনতি_ডান, revs.গুনতি_same);
		অন্যথা
			printf("%d\n", revs.গুনতি_বাম + revs.গুনতি_ডান);
	}

	যদি (show_disk_usage)
		print_disk_usage(যোগ_disk_usage);

cleanup:
	release_revisions(&revs);
	ফেরত ret;
}
