#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "list-objects.h"
#ಅನ್ತರ್ಭಾವ "list-objects-filter-options.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "pack-bitmap.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "log-tree.h"
#ಅನ್ತರ್ಭಾವ "graph.h"
#ಅನ್ತರ್ಭಾವ "bisect.h"
#ಅನ್ತರ್ಭಾವ "progress.h"
#ಅನ್ತರ್ಭಾವ "reflog-walk.h"
#ಅನ್ತರ್ಭಾವ "oidset.h"
#ಅನ್ತರ್ಭಾವ "oidmap.h"
#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"

ಸಂರಚನಾ rev_list_info {
	ಸಂರಚನಾ rev_info *revs;
	ಪೂರ್ಣಾಙ್ಕ flags;
	ಪೂರ್ಣಾಙ್ಕ show_timestamp;
	ಪೂರ್ಣಾಙ್ಕ hdr_termination;
	ಧ್ರುವ ವರ್ಣ *header_prefix;
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ rev_list_usage[] =
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

ಸ್ಥಿರ ಸಂರಚನಾ progress *progress;
ಸ್ಥಿರ unsigned progress_counter;

ಸ್ಥಿರ ಸಂರಚನಾ oidset omitted_objects;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ arg_print_omitted; /* print objects omitted by filter */

ಸಂರಚನಾ missing_objects_map_entry {
	ಸಂರಚನಾ oidmap_entry entry;
	ವರ್ಣ *path;
	unsigned type;
};

ಸ್ಥಿರ ಶೂನ್ಯ missing_objects_map_entry_free(ಶೂನ್ಯ *e)
{
	ಸಂರಚನಾ missing_objects_map_entry *entry =
		container_of(e, ಸಂರಚನಾ missing_objects_map_entry, entry);

	ಮುಕ್ತ(entry->path);
	ಮುಕ್ತ(entry);
}

ಸ್ಥಿರ ಸಂರಚನಾ oidmap missing_objects;
ಗಣನಾ missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ಸ್ಥಿರ ಗಣನಾ missing_action arg_missing_action;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ arg_missing_only;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ should_collect_missing(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ arg_show_object_names = 1;

#define DEFAULT_OIDSET_SIZE     (16*1024)

ಸ್ಥಿರ ವರ್ಣ ಸಾಲು_term = '\n';
ಸ್ಥಿರ ವರ್ಣ info_term = ' ';

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_disk_usage;
ಸ್ಥಿರ off_t total_disk_usage;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ human_readable;

ಸ್ಥಿರ off_t get_object_disk_usage(ಸಂರಚನಾ object *obj)
{
	off_t size;
	ಸಂರಚನಾ object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &size;
	ಯದಿ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_to_hex(&obj->oid));
	ಪ್ರತ್ಯಾಗಮ size;
}

ಸ್ಥಿರ ಶೂನ್ಯ add_missing_object_entry(ಸಂರಚನಾ object_id *oid, ಧ್ರುವ ವರ್ಣ *path,
				     unsigned type)
{
	ಸಂರಚನಾ missing_objects_map_entry *entry;

	ಯದಿ (oidmap_get(&missing_objects, oid))
		ಪ್ರತ್ಯಾಗಮ;

	CALLOC_ARRAY(entry, 1);
	entry->entry.oid = *oid;
	entry->type = type;
	ಯದಿ (path)
		entry->path = xstrdup(path);
	oidmap_put(&missing_objects, entry);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_missing_object(ಸಂರಚನಾ missing_objects_map_entry *entry,
				 ಪೂರ್ಣಾಙ್ಕ print_missing_info)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	ಯದಿ (arg_missing_only && ಸಾಲು_term)
		printf("%s", oid_to_hex(&entry->entry.oid));
	ಅನ್ಯಥಾ ಯದಿ (ಸಾಲು_term)
		printf("?%s", oid_to_hex(&entry->entry.oid));
	ಅನ್ಯಥಾ
		printf("%s%cmissing=yes", oid_to_hex(&entry->entry.oid),
		       info_term);

	ಯದಿ (!print_missing_info) {
		putchar(ಸಾಲು_term);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (entry->path && *entry->path) {
		strbuf_addf(&sb, "%cpath=", info_term);

		ಯದಿ (ಸಾಲು_term) {
			ಸಂರಚನಾ strbuf path = STRBUF_INIT;

			quote_path(entry->path, ರಿಕ್ತ, &path, QUOTE_PATH_QUOTE_SP);
			strbuf_addbuf(&sb, &path);

			strbuf_release(&path);
		} ಅನ್ಯಥಾ {
			strbuf_addstr(&sb, entry->path);
		}
	}
	ಯದಿ (entry->type)
		strbuf_addf(&sb, "%ctype=%s", info_term, type_name(entry->type));

	fwrite(sb.buf, ಪರಿಮಾಣ(ವರ್ಣ), sb.len, stdout);
	putchar(ಸಾಲು_term);

	strbuf_release(&sb);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ finish_object__ma(ಸಂರಚನಾ object *obj, ಧ್ರುವ ವರ್ಣ *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	ವಿಕಲ್ಪ (arg_missing_action) {
	ಪಕ್ಷ MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		ಪ್ರತ್ಯಾಗಮ;

	ಪಕ್ಷ MA_ALLOW_ANY:
		ಪ್ರತ್ಯಾಗಮ;

	ಪಕ್ಷ MA_PRINT:
	ಪಕ್ಷ MA_PRINT_INFO:
		add_missing_object_entry(&obj->oid, name, obj->type);
		ಪ್ರತ್ಯಾಗಮ;

	ಪಕ್ಷ MA_ALLOW_PROMISOR:
		ಯದಿ (is_promisor_object(the_repository, &obj->oid))
			ಪ್ರತ್ಯಾಗಮ;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		ಪ್ರತ್ಯಾಗಮ;

	ಸ್ವಭಾವ:
		BUG("unhandled missing_action");
		ಪ್ರತ್ಯಾಗಮ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ finish_commit(ಸಂರಚನಾ commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = ರಿಕ್ತ;
	free_commit_buffer(the_repository->parsed_objects,
			   commit);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_commit(ಸಂರಚನಾ commit *commit, ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ rev_list_info *info = data;
	ಸಂರಚನಾ rev_info *revs = info->revs;

	display_progress(progress, ++progress_counter);

	ಯದಿ (revs->do_not_die_on_missing_objects &&
	    oidset_contains(&revs->missing_commits, &commit->object.oid)) {
		finish_object__ma(&commit->object, ರಿಕ್ತ);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (arg_missing_only) {
		finish_commit(commit);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (show_disk_usage)
		total_disk_usage += get_object_disk_usage(&commit->object);

	ಯದಿ (info->flags & REV_LIST_QUIET) {
		finish_commit(commit);
		ಪ್ರತ್ಯಾಗಮ;
	}

	graph_show_commit(revs->graph);

	ಯದಿ (revs->ಎಣಿಕೆ) {
		ಯದಿ (commit->object.flags & PATCHSAME)
			revs->ಎಣಿಕೆ_same++;
		ಅನ್ಯಥಾ ಯದಿ (commit->object.flags & SYMMETRIC_ಎಡ)
			revs->ಎಣಿಕೆ_ಎಡ++;
		ಅನ್ಯಥಾ
			revs->ಎಣಿಕೆ_ಬಲ++;
		finish_commit(commit);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (info->show_timestamp)
		printf("%"PRItime" ", commit->date);
	ಯದಿ (info->header_prefix)
		fputs(info->header_prefix, stdout);

	ಯದಿ (revs->include_header) {
		ಯದಿ (!revs->graph && ಸಾಲು_term)
			fputs(get_revision_mark(revs, commit), stdout);
		ಯದಿ (revs->abbrev_commit && revs->abbrev)
			fputs(repo_find_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		ಅನ್ಯಥಾ
			fputs(oid_to_hex(&commit->object.oid), stdout);

		ಯದಿ (!ಸಾಲು_term) {
			ಯದಿ (commit->object.flags & BOUNDARY)
				printf("%cboundary=yes", info_term);
		}
	}
	ಯದಿ (revs->print_parents) {
		ಸಂರಚನಾ commit_list *parents = commit->parents;
		ಯಾವತ್ (parents) {
			printf(" %s", oid_to_hex(&parents->item->object.oid));
			parents = parents->next;
		}
	}
	ಯದಿ (revs->children.name) {
		ಸಂರಚನಾ commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		ಯಾವತ್ (children) {
			printf(" %s", oid_to_hex(&children->item->object.oid));
			children = children->next;
		}
	}
	show_decorations(revs, commit);
	ಯದಿ (revs->commit_format == CMIT_FMT_ONELINE)
		putchar(' ');
	ಅನ್ಯಥಾ ಯದಿ (revs->include_header)
		putchar(ಸಾಲು_term);

	ಯದಿ (revs->verbose_header) {
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
		ಸಂರಚನಾ pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_mode = revs->date_mode;
		ctx.date_mode_explicit = revs->date_mode_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		ಯದಿ (buf.len) {
			ಯದಿ (revs->commit_format != CMIT_FMT_ONELINE)
				graph_show_oneline(revs->graph);

			graph_show_commit_ಸಂದೇಶ(revs->graph, stdout, &buf);

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
			ಯದಿ (buf.len && buf.buf[buf.len - 1] == '\n')
				graph_show_padding(revs->graph);
			putchar(info->hdr_termination);
		} ಅನ್ಯಥಾ {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			ಯದಿ (graph_show_remainder(revs->graph))
				putchar('\n');
			ಯದಿ (revs->commit_format == CMIT_FMT_ONELINE)
				putchar('\n');
		}
		strbuf_release(&buf);
	} ಅನ್ಯಥಾ {
		ಯದಿ (graph_show_remainder(revs->graph))
			putchar('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	finish_commit(commit);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ finish_object(ಸಂರಚನಾ object *obj, ಧ್ರುವ ವರ್ಣ *name, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ rev_list_info *info = cb_data;
	ಯದಿ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, ರಿಕ್ತ, 0) < 0) {
		finish_object__ma(obj, name);
		ಪ್ರತ್ಯಾಗಮ 1;
	}
	ಯದಿ (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_object(ಸಂರಚನಾ object *obj, ಧ್ರುವ ವರ್ಣ *name, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ rev_list_info *info = cb_data;
	ಸಂರಚನಾ rev_info *revs = info->revs;

	ಯದಿ (finish_object(obj, name, cb_data))
		ಪ್ರತ್ಯಾಗಮ;
	display_progress(progress, ++progress_counter);
	ಯದಿ (arg_missing_only)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (show_disk_usage)
		total_disk_usage += get_object_disk_usage(obj);
	ಯದಿ (info->flags & REV_LIST_QUIET)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (revs->ಎಣಿಕೆ) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->ಎಣಿಕೆ_ಬಲ++;
		ಪ್ರತ್ಯಾಗಮ;
	}

	printf("%s", oid_to_hex(&obj->oid));

	ಯದಿ (arg_show_object_names) {
		ಯದಿ (ಸಾಲು_term) {
			putchar(info_term);
			ಪ್ರತಿ (ಧ್ರುವ ವರ್ಣ *p = name; *p && *p != '\n'; p++)
				putchar(*p);
		} ಅನ್ಯಥಾ ಯದಿ (*name) {
			printf("%cpath=%s", info_term, name);
		}
	}

	putchar(ಸಾಲು_term);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_edge(ಸಂರಚನಾ commit *commit)
{
	printf("-%s\n", oid_to_hex(&commit->object.oid));
}

ಸ್ಥಿರ ಶೂನ್ಯ print_var_str(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *val)
{
	printf("%s='%s'\n", var, val);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_var_int(ಧ್ರುವ ವರ್ಣ *var, ಪೂರ್ಣಾಙ್ಕ val)
{
	printf("%s=%d\n", var, val);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_bisect_vars(ಸಂರಚನಾ rev_list_info *info, ಪೂರ್ಣಾಙ್ಕ reaches, ಪೂರ್ಣಾಙ್ಕ all)
{
	ಪೂರ್ಣಾಙ್ಕ cnt, flags = info->flags;
	ವರ್ಣ hex[GIT_MAX_HEXSZ + 1] = "";
	ಸಂರಚನಾ commit_list *tried;
	ಸಂರಚನಾ rev_info *revs = info->revs;

	ಯದಿ (!revs->commits)
		ಪ್ರತ್ಯಾಗಮ 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       ರಿಕ್ತ, ರಿಕ್ತ);

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
	ಯದಿ (cnt < reaches)
		cnt = reaches;

	ಯದಿ (revs->commits)
		oid_to_hex_r(hex, &revs->commits->item->object.oid);

	ಯದಿ (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		printf("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_steps(all));

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_object_fast(
	ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
	ಗಣನಾ object_type type UNUSED,
	ಪೂರ್ಣಾಙ್ಕ exclude UNUSED,
	uint32_t name_hash UNUSED,
	ಸಂರಚನಾ packed_git *found_pack UNUSED,
	off_t found_offset UNUSED,
	ಶೂನ್ಯ *payload UNUSED)
{
	fprintf(stdout, "%s\n", oid_to_hex(oid));
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_disk_usage(off_t size)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಯದಿ (human_readable)
		strbuf_humanise_bytes(&sb, size);
	ಅನ್ಯಥಾ
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)size);
	puts(sb.buf);
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ parse_missing_action_ಮೌಲ್ಯ(ಸಂರಚನಾ repository *repo,
					     ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ಯದಿ (!strcmp(ಮೌಲ್ಯ, "error")) {
		arg_missing_action = MA_ERROR;
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಯದಿ (!strcmp(ಮೌಲ್ಯ, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಯದಿ (!strcmp(ಮೌಲ್ಯ, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಯದಿ (!strcmp(ಮೌಲ್ಯ, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಯದಿ (!strcmp(ಮೌಲ್ಯ, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ try_bitmap_ಎಣಿಕೆ(ಸಂರಚನಾ rev_info *revs,
			    ಪೂರ್ಣಾಙ್ಕ filter_provided_objects)
{
	uint32_t commit_ಎಣಿಕೆ = 0,
		 tag_ಎಣಿಕೆ = 0,
		 ವೃಕ್ಷ_ಎಣಿಕೆ = 0,
		 blob_ಎಣಿಕೆ = 0;
	ಪೂರ್ಣಾಙ್ಕ max_ಎಣಿಕೆ;
	ಸಂರಚನಾ bitmap_index *bitmap_git;

	/* This function only handles counting, not general traversal. */
	ಯದಿ (!revs->ಎಣಿಕೆ)
		ಪ್ರತ್ಯಾಗಮ -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	ಯದಿ (revs->ಎಡ_ಬಲ || revs->cherry_mark)
		ಪ್ರತ್ಯಾಗಮ -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	ಯದಿ (revs->max_ಎಣಿಕೆ >= 0 &&
	    (revs->tag_objects || revs->ವೃಕ್ಷ_objects || revs->blob_objects))
		ಪ್ರತ್ಯಾಗಮ -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_ಎಣಿಕೆ = revs->max_ಎಣಿಕೆ;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ಯದಿ (!bitmap_git)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಎಣಿಕೆ_bitmap_commit_list(bitmap_git, &commit_ಎಣಿಕೆ,
				 revs->ವೃಕ್ಷ_objects ? &ವೃಕ್ಷ_ಎಣಿಕೆ : ರಿಕ್ತ,
				 revs->blob_objects ? &blob_ಎಣಿಕೆ : ರಿಕ್ತ,
				 revs->tag_objects ? &tag_ಎಣಿಕೆ : ರಿಕ್ತ);
	ಯದಿ (max_ಎಣಿಕೆ >= 0 && max_ಎಣಿಕೆ < commit_ಎಣಿಕೆ)
		commit_ಎಣಿಕೆ = max_ಎಣಿಕೆ;

	printf("%d\n", commit_ಎಣಿಕೆ + ವೃಕ್ಷ_ಎಣಿಕೆ + blob_ಎಣಿಕೆ + tag_ಎಣಿಕೆ);
	free_bitmap_index(bitmap_git);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ try_bitmap_traversal(ಸಂರಚನಾ rev_info *revs,
				ಪೂರ್ಣಾಙ್ಕ filter_provided_objects)
{
	ಸಂರಚನಾ bitmap_index *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	ಯದಿ (revs->max_ಎಣಿಕೆ >= 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	ಯದಿ (revs->ಎಡ_ಬಲ)
		ಪ್ರತ್ಯಾಗಮ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ಯದಿ (!bitmap_git)
		ಪ್ರತ್ಯಾಗಮ -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_index(bitmap_git);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ try_bitmap_disk_usage(ಸಂರಚನಾ rev_info *revs,
				 ಪೂರ್ಣಾಙ್ಕ filter_provided_objects)
{
	ಸಂರಚನಾ bitmap_index *bitmap_git;
	off_t size_from_bitmap;

	ಯದಿ (!show_disk_usage)
		ಪ್ರತ್ಯಾಗಮ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ಯದಿ (!bitmap_git)
		ಪ್ರತ್ಯಾಗಮ -1;

	size_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(size_from_bitmap);

	free_bitmap_index(bitmap_git);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
ಸ್ಥಿರ ಶೂನ್ಯ prepare_maximal_independent(ಸಂರಚನಾ rev_info *revs)
{
	ಸಂರಚನಾ commit_list *c;

	ಯದಿ (!revs->maximal_only)
		ಪ್ರತ್ಯಾಗಮ;

	ಪ್ರತಿ (c = revs->commits; c; c = c->next) {
		ಯದಿ (c->item->object.flags & UNINTERESTING)
			ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (revs->limited ||
	    revs->topo_order ||
	    revs->first_parent_only ||
	    revs->reverse ||
	    revs->max_ಎಣಿಕೆ >= 0 ||
	    revs->skip_ಎಣಿಕೆ >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_data.nr ||
	    revs->ಎಣಿಕೆ ||
	    revs->ಎಡ_ಬಲ ||
	    revs->boundary ||
	    revs->tag_objects ||
	    revs->ವೃಕ್ಷ_objects ||
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
	    revs->ಸಾಲು_level_traverse)
		ಪ್ರತ್ಯಾಗಮ;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

ಪೂರ್ಣಾಙ್ಕ cmd_rev_list(ಪೂರ್ಣಾಙ್ಕ argc,
		 ಧ್ರುವ ವರ್ಣ **argv,
		 ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ rev_info revs;
	ಸಂರಚನಾ rev_list_info info;
	ಸಂರಚನಾ setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ bisect_list = 0;
	ಪೂರ್ಣಾಙ್ಕ bisect_show_vars = 0;
	ಪೂರ್ಣಾಙ್ಕ bisect_find_all = 0;
	ಪೂರ್ಣಾಙ್ಕ use_bitmap_index = 0;
	ಪೂರ್ಣಾಙ್ಕ filter_provided_objects = 0;
	ಧ್ರುವ ವರ್ಣ *show_progress = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
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
	ಪ್ರತಿ (i = 1; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[i];
		ಯದಿ (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(arg, "--missing=", &arg)) {
			parse_missing_action_ಮೌಲ್ಯ(repo, arg);
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "-z")) {
			ಸಾಲು_term = '\0';
			info_term = '\0';
		}
	}

	ಯದಿ (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	ಯದಿ (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = setup_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, ಪರಿಮಾಣ(info));
	info.revs = &revs;
	ಯದಿ (revs.bisect)
		bisect_list = 1;

	ಯದಿ (revs.diffopt.flags.quick)
		info.flags |= REV_LIST_QUIET;
	ಪ್ರತಿ (i = 1 ; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[i];

		ಯದಿ (!strcmp(arg, "--header")) {
			revs.verbose_header = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_find_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_index = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			ಗಚ್ಛ cleanup;
		}
		ಯದಿ (skip_prefix(arg, "--progress=", &arg)) {
			show_progress = arg;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			ಅನುವರ್ತ;
		}

		ಯದಿ (!strcmp(arg, "--exclude-promisor-objects"))
			ಅನುವರ್ತ; /* already handled above */
		ಯದಿ (skip_prefix(arg, "--missing=", &arg))
			ಅನುವರ್ತ; /* already handled above */

		ಯದಿ (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_names = 0;
			ಅನುವರ್ತ;
		}

		ಯದಿ (!strcmp(arg, ("--object-names"))) {
			arg_show_object_names = 1;
			ಅನುವರ್ತ;
		}

		ಯದಿ (!strcmp(arg, ("--commit-header"))) {
			revs.include_header = 1;
			ಅನುವರ್ತ;
		}

		ಯದಿ (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_header = 0;
			ಅನುವರ್ತ;
		}

		ಯದಿ (skip_prefix(arg, "--disk-usage", &arg)) {
			ಯದಿ (*arg == '=') {
				ಯದಿ (!strcmp(++arg, "human")) {
					human_readable = 1;
				} ಅನ್ಯಥಾ
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} ಅನ್ಯಥಾ ಯದಿ (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			ಅನುವರ್ತ;
		}

		ಯದಿ (!strcmp(arg, "--missing-only"))
			ಅನುವರ್ತ;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	ಯದಿ (!ಸಾಲು_term) {
		ಯದಿ (revs.graph || revs.verbose_header || show_disk_usage ||
		    info.show_timestamp || info.header_prefix || bisect_list ||
		    use_bitmap_index || revs.edge_hint || revs.ಎಡ_ಬಲ ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	ಯದಿ (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_header = 1;
	ಯದಿ (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		ಯದಿ (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_header)
			info.header_prefix = "";
		ಅನ್ಯಥಾ
			info.header_prefix = "commit ";
	}
	ಅನ್ಯಥಾ ಯದಿ (revs.verbose_header)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	ಯದಿ ((!revs.commits && reflog_walk_empty(revs.reflog_info) &&
	     (!(revs.tag_objects || revs.ವೃಕ್ಷ_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_ಒಳಹರಿವು_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	ಯದಿ (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	ಯದಿ (revs.ಎಣಿಕೆ &&
	    (revs.tag_objects || revs.ವೃಕ್ಷ_objects || revs.blob_objects) &&
	    (revs.ಎಡ_ಬಲ || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.ಎಣಿಕೆ, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_buffer = (revs.verbose_header ||
			      revs.grep_filter.pattern_list ||
			      revs.grep_filter.header_list);
	ಯದಿ (bisect_list)
		revs.limited = 1;

	ಯದಿ (show_progress)
		progress = start_delayed_progress(the_repository,
						  show_progress, 0);

	ಯದಿ (use_bitmap_index) {
		ಯದಿ (!try_bitmap_ಎಣಿಕೆ(&revs, filter_provided_objects))
			ಗಚ್ಛ cleanup;
		ಯದಿ (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			ಗಚ್ಛ cleanup;
		ಯದಿ (!try_bitmap_traversal(&revs, filter_provided_objects))
			ಗಚ್ಛ cleanup;
	}

	ಯದಿ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	ಯದಿ (revs.ವೃಕ್ಷ_objects)
		mark_edges_uninteresting(&revs, show_edge, 0);

	ಯದಿ (bisect_list) {
		ಪೂರ್ಣಾಙ್ಕ reaches, all;
		unsigned bisect_flags = 0;

		ಯದಿ (bisect_find_all)
			bisect_flags |= FIND_BISECTION_ALL;

		ಯದಿ (revs.first_parent_only)
			bisect_flags |= FIND_BISECTION_FIRST_PARENT_ONLY;

		find_bisection(&revs.commits, &reaches, &all, bisect_flags);

		ಯದಿ (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			ಗಚ್ಛ cleanup;
		}
	}

	ಯದಿ (filter_provided_objects) {
		ಸಂರಚನಾ commit_list *c;
		ಪ್ರತಿ (i = 0; i < revs.pending.nr; i++) {
			ಸಂರಚನಾ object_array_entry *pending = revs.pending.objects + i;
			pending->item->flags |= NOT_USER_GIVEN;
		}
		ಪ್ರತಿ (c = revs.commits; c; c = c->next)
			c->item->object.flags |= NOT_USER_GIVEN;
	}

	ಯದಿ (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_SIZE);
	ಯದಿ (should_collect_missing()) {
		ಸಂರಚನಾ oidset_iter iter;
		ಸಂರಚನಾ object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_SIZE);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		ಯಾವತ್ ((oid = oidset_iter_next(&iter)))
			add_missing_object_entry(oid, ರಿಕ್ತ, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : ರಿಕ್ತ));

	ಯದಿ (arg_print_omitted) {
		ಸಂರಚನಾ oidset_iter iter;
		ಸಂರಚನಾ object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		ಯಾವತ್ ((oid = oidset_iter_next(&iter)))
			printf("~%s\n", oid_to_hex(oid));
		oidset_clear(&omitted_objects);
	}
	ಯದಿ (should_collect_missing()) {
		ಸಂರಚನಾ missing_objects_map_entry *entry;
		ಸಂರಚನಾ oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		ಯಾವತ್ ((entry = oidmap_iter_next(&iter))) {
			print_missing_object(entry, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_entry_free);
	}

	stop_progress(&progress);

	ಯದಿ (revs.ಎಣಿಕೆ) {
		ಯದಿ (revs.ಎಡ_ಬಲ && revs.cherry_mark)
			printf("%d\t%d\t%d\n", revs.ಎಣಿಕೆ_ಎಡ, revs.ಎಣಿಕೆ_ಬಲ, revs.ಎಣಿಕೆ_same);
		ಅನ್ಯಥಾ ಯದಿ (revs.ಎಡ_ಬಲ)
			printf("%d\t%d\n", revs.ಎಣಿಕೆ_ಎಡ, revs.ಎಣಿಕೆ_ಬಲ);
		ಅನ್ಯಥಾ ಯದಿ (revs.cherry_mark)
			printf("%d\t%d\n", revs.ಎಣಿಕೆ_ಎಡ + revs.ಎಣಿಕೆ_ಬಲ, revs.ಎಣಿಕೆ_same);
		ಅನ್ಯಥಾ
			printf("%d\n", revs.ಎಣಿಕೆ_ಎಡ + revs.ಎಣಿಕೆ_ಬಲ);
	}

	ಯದಿ (show_disk_usage)
		print_disk_usage(total_disk_usage);

cleanup:
	release_revisions(&revs);
	ಪ್ರತ್ಯಾಗಮ ret;
}
