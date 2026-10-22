#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "list-objects.h"
#അന്തര്ഭാവ "list-objects-filter-options.h"
#അന്തര്ഭാവ "object.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "pack-bitmap.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "log-tree.h"
#അന്തര്ഭാവ "graph.h"
#അന്തര്ഭാവ "bisect.h"
#അന്തര്ഭാവ "progress.h"
#അന്തര്ഭാവ "reflog-walk.h"
#അന്തര്ഭാവ "oidset.h"
#അന്തര്ഭാവ "oidmap.h"
#അന്തര്ഭാവ "packfile.h"
#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "strbuf.h"

ഘടന rev_list_info {
	ഘടന rev_info *revs;
	പൂർണ്ണസംഖ്യ flags;
	പൂർണ്ണസംഖ്യ show_timestamp;
	പൂർണ്ണസംഖ്യ hdr_termination;
	സ്ഥിരം അക്ഷരം *header_prefix;
};

സ്ഥിതം സ്ഥിരം അക്ഷരം rev_list_usage[] =
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

സ്ഥിതം ഘടന progress *progress;
സ്ഥിതം unsigned progress_counter;

സ്ഥിതം ഘടന oidset omitted_objects;
സ്ഥിതം പൂർണ്ണസംഖ്യ arg_print_omitted; /* print objects omitted by filter */

ഘടന missing_objects_map_entry {
	ഘടന oidmap_entry entry;
	അക്ഷരം *path;
	unsigned type;
};

സ്ഥിതം ശൂന്യം missing_objects_map_entry_free(ശൂന്യം *e)
{
	ഘടന missing_objects_map_entry *entry =
		container_of(e, ഘടന missing_objects_map_entry, entry);

	സ്വതന്ത്രം(entry->path);
	സ്വതന്ത്രം(entry);
}

സ്ഥിതം ഘടന oidmap missing_objects;
എണ്ണൽ missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
സ്ഥിതം എണ്ണൽ missing_action arg_missing_action;
സ്ഥിതം പൂർണ്ണസംഖ്യ arg_missing_only;

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ should_collect_missing(ശൂന്യം)
{
	തിരികെ arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
സ്ഥിതം പൂർണ്ണസംഖ്യ arg_show_object_names = 1;

#define DEFAULT_OIDSET_SIZE     (16*1024)

സ്ഥിതം അക്ഷരം വരി_term = '\n';
സ്ഥിതം അക്ഷരം info_term = ' ';

സ്ഥിതം പൂർണ്ണസംഖ്യ show_disk_usage;
സ്ഥിതം off_t total_disk_usage;
സ്ഥിതം പൂർണ്ണസംഖ്യ human_readable;

സ്ഥിതം off_t get_object_disk_usage(ഘടന object *obj)
{
	off_t size;
	ഘടന object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &size;
	എങ്കിൽ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_to_hex(&obj->oid));
	തിരികെ size;
}

സ്ഥിതം ശൂന്യം add_missing_object_entry(ഘടന object_id *oid, സ്ഥിരം അക്ഷരം *path,
				     unsigned type)
{
	ഘടന missing_objects_map_entry *entry;

	എങ്കിൽ (oidmap_get(&missing_objects, oid))
		തിരികെ;

	CALLOC_ARRAY(entry, 1);
	entry->entry.oid = *oid;
	entry->type = type;
	എങ്കിൽ (path)
		entry->path = xstrdup(path);
	oidmap_put(&missing_objects, entry);
}

സ്ഥിതം ശൂന്യം print_missing_object(ഘടന missing_objects_map_entry *entry,
				 പൂർണ്ണസംഖ്യ print_missing_info)
{
	ഘടന strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	എങ്കിൽ (arg_missing_only && വരി_term)
		printf("%s", oid_to_hex(&entry->entry.oid));
	അല്ലെങ്കിൽ എങ്കിൽ (വരി_term)
		printf("?%s", oid_to_hex(&entry->entry.oid));
	അല്ലെങ്കിൽ
		printf("%s%cmissing=yes", oid_to_hex(&entry->entry.oid),
		       info_term);

	എങ്കിൽ (!print_missing_info) {
		putchar(വരി_term);
		തിരികെ;
	}

	എങ്കിൽ (entry->path && *entry->path) {
		strbuf_addf(&sb, "%cpath=", info_term);

		എങ്കിൽ (വരി_term) {
			ഘടന strbuf path = STRBUF_INIT;

			quote_path(entry->path, ശൂന്യ, &path, QUOTE_PATH_QUOTE_SP);
			strbuf_addbuf(&sb, &path);

			strbuf_release(&path);
		} അല്ലെങ്കിൽ {
			strbuf_addstr(&sb, entry->path);
		}
	}
	എങ്കിൽ (entry->type)
		strbuf_addf(&sb, "%ctype=%s", info_term, type_name(entry->type));

	fwrite(sb.buf, വലുപ്പം(അക്ഷരം), sb.len, stdout);
	putchar(വരി_term);

	strbuf_release(&sb);
}

സ്ഥിതം വരിയിൽ ശൂന്യം finish_object__ma(ഘടന object *obj, സ്ഥിരം അക്ഷരം *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	തിരഞ്ഞെടുപ്പ് (arg_missing_action) {
	അവസ്ഥ MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		തിരികെ;

	അവസ്ഥ MA_ALLOW_ANY:
		തിരികെ;

	അവസ്ഥ MA_PRINT:
	അവസ്ഥ MA_PRINT_INFO:
		add_missing_object_entry(&obj->oid, name, obj->type);
		തിരികെ;

	അവസ്ഥ MA_ALLOW_PROMISOR:
		എങ്കിൽ (is_promisor_object(the_repository, &obj->oid))
			തിരികെ;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		തിരികെ;

	സ്ഥിരസ്ഥിതി:
		BUG("unhandled missing_action");
		തിരികെ;
	}
}

സ്ഥിതം ശൂന്യം finish_commit(ഘടന commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = ശൂന്യ;
	free_commit_buffer(the_repository->parsed_objects,
			   commit);
}

സ്ഥിതം ശൂന്യം show_commit(ഘടന commit *commit, ശൂന്യം *data)
{
	ഘടന rev_list_info *info = data;
	ഘടന rev_info *revs = info->revs;

	display_progress(progress, ++progress_counter);

	എങ്കിൽ (revs->do_not_die_on_missing_objects &&
	    oidset_contains(&revs->missing_commits, &commit->object.oid)) {
		finish_object__ma(&commit->object, ശൂന്യ);
		തിരികെ;
	}

	എങ്കിൽ (arg_missing_only) {
		finish_commit(commit);
		തിരികെ;
	}

	എങ്കിൽ (show_disk_usage)
		total_disk_usage += get_object_disk_usage(&commit->object);

	എങ്കിൽ (info->flags & REV_LIST_QUIET) {
		finish_commit(commit);
		തിരികെ;
	}

	graph_show_commit(revs->graph);

	എങ്കിൽ (revs->എണ്ണം) {
		എങ്കിൽ (commit->object.flags & PATCHSAME)
			revs->എണ്ണം_same++;
		അല്ലെങ്കിൽ എങ്കിൽ (commit->object.flags & SYMMETRIC_ഇടത്)
			revs->എണ്ണം_ഇടത്++;
		അല്ലെങ്കിൽ
			revs->എണ്ണം_വലത്++;
		finish_commit(commit);
		തിരികെ;
	}

	എങ്കിൽ (info->show_timestamp)
		printf("%"PRItime" ", commit->date);
	എങ്കിൽ (info->header_prefix)
		fputs(info->header_prefix, stdout);

	എങ്കിൽ (revs->include_header) {
		എങ്കിൽ (!revs->graph && വരി_term)
			fputs(get_revision_mark(revs, commit), stdout);
		എങ്കിൽ (revs->abbrev_commit && revs->abbrev)
			fputs(repo_find_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		അല്ലെങ്കിൽ
			fputs(oid_to_hex(&commit->object.oid), stdout);

		എങ്കിൽ (!വരി_term) {
			എങ്കിൽ (commit->object.flags & BOUNDARY)
				printf("%cboundary=yes", info_term);
		}
	}
	എങ്കിൽ (revs->print_parents) {
		ഘടന commit_list *parents = commit->parents;
		ആകുമ്പോൾ (parents) {
			printf(" %s", oid_to_hex(&parents->item->object.oid));
			parents = parents->next;
		}
	}
	എങ്കിൽ (revs->children.name) {
		ഘടന commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		ആകുമ്പോൾ (children) {
			printf(" %s", oid_to_hex(&children->item->object.oid));
			children = children->next;
		}
	}
	show_decorations(revs, commit);
	എങ്കിൽ (revs->commit_format == CMIT_FMT_ONELINE)
		putchar(' ');
	അല്ലെങ്കിൽ എങ്കിൽ (revs->include_header)
		putchar(വരി_term);

	എങ്കിൽ (revs->verbose_header) {
		ഘടന strbuf buf = STRBUF_INIT;
		ഘടന pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_mode = revs->date_mode;
		ctx.date_mode_explicit = revs->date_mode_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		എങ്കിൽ (buf.len) {
			എങ്കിൽ (revs->commit_format != CMIT_FMT_ONELINE)
				graph_show_oneline(revs->graph);

			graph_show_commit_സന്ദേശം(revs->graph, stdout, &buf);

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
			എങ്കിൽ (buf.len && buf.buf[buf.len - 1] == '\n')
				graph_show_padding(revs->graph);
			putchar(info->hdr_termination);
		} അല്ലെങ്കിൽ {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			എങ്കിൽ (graph_show_remainder(revs->graph))
				putchar('\n');
			എങ്കിൽ (revs->commit_format == CMIT_FMT_ONELINE)
				putchar('\n');
		}
		strbuf_release(&buf);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (graph_show_remainder(revs->graph))
			putchar('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	finish_commit(commit);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ finish_object(ഘടന object *obj, സ്ഥിരം അക്ഷരം *name, ശൂന്യം *cb_data)
{
	ഘടന rev_list_info *info = cb_data;
	എങ്കിൽ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, ശൂന്യ, 0) < 0) {
		finish_object__ma(obj, name);
		തിരികെ 1;
	}
	എങ്കിൽ (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം show_object(ഘടന object *obj, സ്ഥിരം അക്ഷരം *name, ശൂന്യം *cb_data)
{
	ഘടന rev_list_info *info = cb_data;
	ഘടന rev_info *revs = info->revs;

	എങ്കിൽ (finish_object(obj, name, cb_data))
		തിരികെ;
	display_progress(progress, ++progress_counter);
	എങ്കിൽ (arg_missing_only)
		തിരികെ;
	എങ്കിൽ (show_disk_usage)
		total_disk_usage += get_object_disk_usage(obj);
	എങ്കിൽ (info->flags & REV_LIST_QUIET)
		തിരികെ;

	എങ്കിൽ (revs->എണ്ണം) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->എണ്ണം_വലത്++;
		തിരികെ;
	}

	printf("%s", oid_to_hex(&obj->oid));

	എങ്കിൽ (arg_show_object_names) {
		എങ്കിൽ (വരി_term) {
			putchar(info_term);
			വേണ്ടി (സ്ഥിരം അക്ഷരം *p = name; *p && *p != '\n'; p++)
				putchar(*p);
		} അല്ലെങ്കിൽ എങ്കിൽ (*name) {
			printf("%cpath=%s", info_term, name);
		}
	}

	putchar(വരി_term);
}

സ്ഥിതം ശൂന്യം show_edge(ഘടന commit *commit)
{
	printf("-%s\n", oid_to_hex(&commit->object.oid));
}

സ്ഥിതം ശൂന്യം print_var_str(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *val)
{
	printf("%s='%s'\n", var, val);
}

സ്ഥിതം ശൂന്യം print_var_int(സ്ഥിരം അക്ഷരം *var, പൂർണ്ണസംഖ്യ val)
{
	printf("%s=%d\n", var, val);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_bisect_vars(ഘടന rev_list_info *info, പൂർണ്ണസംഖ്യ reaches, പൂർണ്ണസംഖ്യ all)
{
	പൂർണ്ണസംഖ്യ cnt, flags = info->flags;
	അക്ഷരം hex[GIT_MAX_HEXSZ + 1] = "";
	ഘടന commit_list *tried;
	ഘടന rev_info *revs = info->revs;

	എങ്കിൽ (!revs->commits)
		തിരികെ 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       ശൂന്യ, ശൂന്യ);

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
	എങ്കിൽ (cnt < reaches)
		cnt = reaches;

	എങ്കിൽ (revs->commits)
		oid_to_hex_r(hex, &revs->commits->item->object.oid);

	എങ്കിൽ (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		printf("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_steps(all));

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_object_fast(
	സ്ഥിരം ഘടന object_id *oid,
	എണ്ണൽ object_type type UNUSED,
	പൂർണ്ണസംഖ്യ exclude UNUSED,
	uint32_t name_hash UNUSED,
	ഘടന packed_git *found_pack UNUSED,
	off_t found_offset UNUSED,
	ശൂന്യം *payload UNUSED)
{
	fprintf(stdout, "%s\n", oid_to_hex(oid));
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം print_disk_usage(off_t size)
{
	ഘടന strbuf sb = STRBUF_INIT;
	എങ്കിൽ (human_readable)
		strbuf_humanise_bytes(&sb, size);
	അല്ലെങ്കിൽ
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)size);
	puts(sb.buf);
	strbuf_release(&sb);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ parse_missing_action_മൂല്യം(ഘടന repository *repo,
					     സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	എങ്കിൽ (!strcmp(മൂല്യം, "error")) {
		arg_missing_action = MA_ERROR;
		തിരികെ 1;
	}

	എങ്കിൽ (!strcmp(മൂല്യം, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		തിരികെ 1;
	}

	എങ്കിൽ (!strcmp(മൂല്യം, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		തിരികെ 1;
	}

	എങ്കിൽ (!strcmp(മൂല്യം, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		തിരികെ 1;
	}

	എങ്കിൽ (!strcmp(മൂല്യം, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		തിരികെ 1;
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ try_bitmap_എണ്ണം(ഘടന rev_info *revs,
			    പൂർണ്ണസംഖ്യ filter_provided_objects)
{
	uint32_t commit_എണ്ണം = 0,
		 tag_എണ്ണം = 0,
		 വൃക്ഷം_എണ്ണം = 0,
		 blob_എണ്ണം = 0;
	പൂർണ്ണസംഖ്യ max_എണ്ണം;
	ഘടന bitmap_index *bitmap_git;

	/* This function only handles counting, not general traversal. */
	എങ്കിൽ (!revs->എണ്ണം)
		തിരികെ -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	എങ്കിൽ (revs->ഇടത്_വലത് || revs->cherry_mark)
		തിരികെ -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	എങ്കിൽ (revs->max_എണ്ണം >= 0 &&
	    (revs->tag_objects || revs->വൃക്ഷം_objects || revs->blob_objects))
		തിരികെ -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_എണ്ണം = revs->max_എണ്ണം;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	എങ്കിൽ (!bitmap_git)
		തിരികെ -1;

	എണ്ണം_bitmap_commit_list(bitmap_git, &commit_എണ്ണം,
				 revs->വൃക്ഷം_objects ? &വൃക്ഷം_എണ്ണം : ശൂന്യ,
				 revs->blob_objects ? &blob_എണ്ണം : ശൂന്യ,
				 revs->tag_objects ? &tag_എണ്ണം : ശൂന്യ);
	എങ്കിൽ (max_എണ്ണം >= 0 && max_എണ്ണം < commit_എണ്ണം)
		commit_എണ്ണം = max_എണ്ണം;

	printf("%d\n", commit_എണ്ണം + വൃക്ഷം_എണ്ണം + blob_എണ്ണം + tag_എണ്ണം);
	free_bitmap_index(bitmap_git);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ try_bitmap_traversal(ഘടന rev_info *revs,
				പൂർണ്ണസംഖ്യ filter_provided_objects)
{
	ഘടന bitmap_index *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	എങ്കിൽ (revs->max_എണ്ണം >= 0)
		തിരികെ -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	എങ്കിൽ (revs->ഇടത്_വലത്)
		തിരികെ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	എങ്കിൽ (!bitmap_git)
		തിരികെ -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_index(bitmap_git);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ try_bitmap_disk_usage(ഘടന rev_info *revs,
				 പൂർണ്ണസംഖ്യ filter_provided_objects)
{
	ഘടന bitmap_index *bitmap_git;
	off_t size_from_bitmap;

	എങ്കിൽ (!show_disk_usage)
		തിരികെ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	എങ്കിൽ (!bitmap_git)
		തിരികെ -1;

	size_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(size_from_bitmap);

	free_bitmap_index(bitmap_git);
	തിരികെ 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
സ്ഥിതം ശൂന്യം prepare_maximal_independent(ഘടന rev_info *revs)
{
	ഘടന commit_list *c;

	എങ്കിൽ (!revs->maximal_only)
		തിരികെ;

	വേണ്ടി (c = revs->commits; c; c = c->next) {
		എങ്കിൽ (c->item->object.flags & UNINTERESTING)
			തിരികെ;
	}

	എങ്കിൽ (revs->limited ||
	    revs->topo_order ||
	    revs->first_parent_only ||
	    revs->reverse ||
	    revs->max_എണ്ണം >= 0 ||
	    revs->skip_എണ്ണം >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_data.nr ||
	    revs->എണ്ണം ||
	    revs->ഇടത്_വലത് ||
	    revs->boundary ||
	    revs->tag_objects ||
	    revs->വൃക്ഷം_objects ||
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
	    revs->വരി_level_traverse)
		തിരികെ;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

പൂർണ്ണസംഖ്യ cmd_rev_list(പൂർണ്ണസംഖ്യ argc,
		 സ്ഥിരം അക്ഷരം **argv,
		 സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo)
{
	ഘടന rev_info revs;
	ഘടന rev_list_info info;
	ഘടന setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ bisect_list = 0;
	പൂർണ്ണസംഖ്യ bisect_show_vars = 0;
	പൂർണ്ണസംഖ്യ bisect_find_all = 0;
	പൂർണ്ണസംഖ്യ use_bitmap_index = 0;
	പൂർണ്ണസംഖ്യ filter_provided_objects = 0;
	സ്ഥിരം അക്ഷരം *show_progress = ശൂന്യ;
	പൂർണ്ണസംഖ്യ ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, ശൂന്യ);
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
	വേണ്ടി (i = 1; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];
		എങ്കിൽ (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(arg, "--missing=", &arg)) {
			parse_missing_action_മൂല്യം(repo, arg);
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "-z")) {
			വരി_term = '\0';
			info_term = '\0';
		}
	}

	എങ്കിൽ (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	എങ്കിൽ (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = setup_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, വലുപ്പം(info));
	info.revs = &revs;
	എങ്കിൽ (revs.bisect)
		bisect_list = 1;

	എങ്കിൽ (revs.diffopt.flags.quick)
		info.flags |= REV_LIST_QUIET;
	വേണ്ടി (i = 1 ; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];

		എങ്കിൽ (!strcmp(arg, "--header")) {
			revs.verbose_header = 1;
			തുടരുക;
		}
		എങ്കിൽ (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			തുടരുക;
		}
		എങ്കിൽ (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			തുടരുക;
		}
		എങ്കിൽ (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_find_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			തുടരുക;
		}
		എങ്കിൽ (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			തുടരുക;
		}
		എങ്കിൽ (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_index = 1;
			തുടരുക;
		}
		എങ്കിൽ (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			പോകുക cleanup;
		}
		എങ്കിൽ (skip_prefix(arg, "--progress=", &arg)) {
			show_progress = arg;
			തുടരുക;
		}
		എങ്കിൽ (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			തുടരുക;
		}
		എങ്കിൽ (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			തുടരുക;
		}

		എങ്കിൽ (!strcmp(arg, "--exclude-promisor-objects"))
			തുടരുക; /* already handled above */
		എങ്കിൽ (skip_prefix(arg, "--missing=", &arg))
			തുടരുക; /* already handled above */

		എങ്കിൽ (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_names = 0;
			തുടരുക;
		}

		എങ്കിൽ (!strcmp(arg, ("--object-names"))) {
			arg_show_object_names = 1;
			തുടരുക;
		}

		എങ്കിൽ (!strcmp(arg, ("--commit-header"))) {
			revs.include_header = 1;
			തുടരുക;
		}

		എങ്കിൽ (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_header = 0;
			തുടരുക;
		}

		എങ്കിൽ (skip_prefix(arg, "--disk-usage", &arg)) {
			എങ്കിൽ (*arg == '=') {
				എങ്കിൽ (!strcmp(++arg, "human")) {
					human_readable = 1;
				} അല്ലെങ്കിൽ
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} അല്ലെങ്കിൽ എങ്കിൽ (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			തുടരുക;
		}

		എങ്കിൽ (!strcmp(arg, "--missing-only"))
			തുടരുക;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	എങ്കിൽ (!വരി_term) {
		എങ്കിൽ (revs.graph || revs.verbose_header || show_disk_usage ||
		    info.show_timestamp || info.header_prefix || bisect_list ||
		    use_bitmap_index || revs.edge_hint || revs.ഇടത്_വലത് ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	എങ്കിൽ (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_header = 1;
	എങ്കിൽ (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		എങ്കിൽ (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_header)
			info.header_prefix = "";
		അല്ലെങ്കിൽ
			info.header_prefix = "commit ";
	}
	അല്ലെങ്കിൽ എങ്കിൽ (revs.verbose_header)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	എങ്കിൽ ((!revs.commits && reflog_walk_empty(revs.reflog_info) &&
	     (!(revs.tag_objects || revs.വൃക്ഷം_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_നിവേശം_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	എങ്കിൽ (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	എങ്കിൽ (revs.എണ്ണം &&
	    (revs.tag_objects || revs.വൃക്ഷം_objects || revs.blob_objects) &&
	    (revs.ഇടത്_വലത് || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.എണ്ണം, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_buffer = (revs.verbose_header ||
			      revs.grep_filter.pattern_list ||
			      revs.grep_filter.header_list);
	എങ്കിൽ (bisect_list)
		revs.limited = 1;

	എങ്കിൽ (show_progress)
		progress = start_delayed_progress(the_repository,
						  show_progress, 0);

	എങ്കിൽ (use_bitmap_index) {
		എങ്കിൽ (!try_bitmap_എണ്ണം(&revs, filter_provided_objects))
			പോകുക cleanup;
		എങ്കിൽ (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			പോകുക cleanup;
		എങ്കിൽ (!try_bitmap_traversal(&revs, filter_provided_objects))
			പോകുക cleanup;
	}

	എങ്കിൽ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	എങ്കിൽ (revs.വൃക്ഷം_objects)
		mark_edges_uninteresting(&revs, show_edge, 0);

	എങ്കിൽ (bisect_list) {
		പൂർണ്ണസംഖ്യ reaches, all;
		unsigned bisect_flags = 0;

		എങ്കിൽ (bisect_find_all)
			bisect_flags |= FIND_BISECTION_ALL;

		എങ്കിൽ (revs.first_parent_only)
			bisect_flags |= FIND_BISECTION_FIRST_PARENT_ONLY;

		find_bisection(&revs.commits, &reaches, &all, bisect_flags);

		എങ്കിൽ (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			പോകുക cleanup;
		}
	}

	എങ്കിൽ (filter_provided_objects) {
		ഘടന commit_list *c;
		വേണ്ടി (i = 0; i < revs.pending.nr; i++) {
			ഘടന object_array_entry *pending = revs.pending.objects + i;
			pending->item->flags |= NOT_USER_GIVEN;
		}
		വേണ്ടി (c = revs.commits; c; c = c->next)
			c->item->object.flags |= NOT_USER_GIVEN;
	}

	എങ്കിൽ (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_SIZE);
	എങ്കിൽ (should_collect_missing()) {
		ഘടന oidset_iter iter;
		ഘടന object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_SIZE);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		ആകുമ്പോൾ ((oid = oidset_iter_next(&iter)))
			add_missing_object_entry(oid, ശൂന്യ, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : ശൂന്യ));

	എങ്കിൽ (arg_print_omitted) {
		ഘടന oidset_iter iter;
		ഘടന object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		ആകുമ്പോൾ ((oid = oidset_iter_next(&iter)))
			printf("~%s\n", oid_to_hex(oid));
		oidset_clear(&omitted_objects);
	}
	എങ്കിൽ (should_collect_missing()) {
		ഘടന missing_objects_map_entry *entry;
		ഘടന oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		ആകുമ്പോൾ ((entry = oidmap_iter_next(&iter))) {
			print_missing_object(entry, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_entry_free);
	}

	stop_progress(&progress);

	എങ്കിൽ (revs.എണ്ണം) {
		എങ്കിൽ (revs.ഇടത്_വലത് && revs.cherry_mark)
			printf("%d\t%d\t%d\n", revs.എണ്ണം_ഇടത്, revs.എണ്ണം_വലത്, revs.എണ്ണം_same);
		അല്ലെങ്കിൽ എങ്കിൽ (revs.ഇടത്_വലത്)
			printf("%d\t%d\n", revs.എണ്ണം_ഇടത്, revs.എണ്ണം_വലത്);
		അല്ലെങ്കിൽ എങ്കിൽ (revs.cherry_mark)
			printf("%d\t%d\n", revs.എണ്ണം_ഇടത് + revs.എണ്ണം_വലത്, revs.എണ്ണം_same);
		അല്ലെങ്കിൽ
			printf("%d\n", revs.എണ്ണം_ഇടത് + revs.എണ്ണം_വലത്);
	}

	എങ്കിൽ (show_disk_usage)
		print_disk_usage(total_disk_usage);

cleanup:
	release_revisions(&revs);
	തിരികെ ret;
}
