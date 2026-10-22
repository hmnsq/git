#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "list-objects.h"
#ଅନ୍ତର୍ଭାଵ "list-objects-filter-options.h"
#ଅନ୍ତର୍ଭାଵ "object.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "pack-bitmap.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "log-tree.h"
#ଅନ୍ତର୍ଭାଵ "graph.h"
#ଅନ୍ତର୍ଭାଵ "bisect.h"
#ଅନ୍ତର୍ଭାଵ "progress.h"
#ଅନ୍ତର୍ଭାଵ "reflog-walk.h"
#ଅନ୍ତର୍ଭାଵ "oidset.h"
#ଅନ୍ତର୍ଭାଵ "oidmap.h"
#ଅନ୍ତର୍ଭାଵ "packfile.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"

ସଂରଚନା rev_list_info {
	ସଂରଚନା rev_info *revs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_timestamp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hdr_termination;
	ଅଚଳ ଅକ୍ଷର *header_prefix;
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର rev_list_usage[] =
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

ସ୍ଥିର ସଂରଚନା progress *progress;
ସ୍ଥିର unsigned progress_counter;

ସ୍ଥିର ସଂରଚନା oidset omitted_objects;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା arg_print_omitted; /* print objects omitted by filter */

ସଂରଚନା missing_objects_map_entry {
	ସଂରଚନା oidmap_entry entry;
	ଅକ୍ଷର *path;
	unsigned type;
};

ସ୍ଥିର ଶୂନ୍ୟ missing_objects_map_entry_free(ଶୂନ୍ୟ *e)
{
	ସଂରଚନା missing_objects_map_entry *entry =
		container_of(e, ସଂରଚନା missing_objects_map_entry, entry);

	ମୁକ୍ତ(entry->path);
	ମୁକ୍ତ(entry);
}

ସ୍ଥିର ସଂରଚନା oidmap missing_objects;
ଗଣନା missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ସ୍ଥିର ଗଣନା missing_action arg_missing_action;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା arg_missing_only;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା should_collect_missing(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା arg_show_object_names = 1;

#define DEFAULT_OIDSET_SIZE     (16*1024)

ସ୍ଥିର ଅକ୍ଷର ଧାଡ଼ି_term = '\n';
ସ୍ଥିର ଅକ୍ଷର info_term = ' ';

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_disk_usage;
ସ୍ଥିର off_t total_disk_usage;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା human_readable;

ସ୍ଥିର off_t get_object_disk_usage(ସଂରଚନା object *obj)
{
	off_t size;
	ସଂରଚନା object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &size;
	ଯଦି (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_to_hex(&obj->oid));
	ଫେରନ୍ତୁ size;
}

ସ୍ଥିର ଶୂନ୍ୟ add_missing_object_entry(ସଂରଚନା object_id *oid, ଅଚଳ ଅକ୍ଷର *path,
				     unsigned type)
{
	ସଂରଚନା missing_objects_map_entry *entry;

	ଯଦି (oidmap_get(&missing_objects, oid))
		ଫେରନ୍ତୁ;

	CALLOC_ARRAY(entry, 1);
	entry->entry.oid = *oid;
	entry->type = type;
	ଯଦି (path)
		entry->path = xstrdup(path);
	oidmap_put(&missing_objects, entry);
}

ସ୍ଥିର ଶୂନ୍ୟ print_missing_object(ସଂରଚନା missing_objects_map_entry *entry,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_missing_info)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	ଯଦି (arg_missing_only && ଧାଡ଼ି_term)
		printf("%s", oid_to_hex(&entry->entry.oid));
	ଅନ୍ୟଥା ଯଦି (ଧାଡ଼ି_term)
		printf("?%s", oid_to_hex(&entry->entry.oid));
	ଅନ୍ୟଥା
		printf("%s%cmissing=yes", oid_to_hex(&entry->entry.oid),
		       info_term);

	ଯଦି (!print_missing_info) {
		putchar(ଧାଡ଼ି_term);
		ଫେରନ୍ତୁ;
	}

	ଯଦି (entry->path && *entry->path) {
		strbuf_addf(&sb, "%cpath=", info_term);

		ଯଦି (ଧାଡ଼ି_term) {
			ସଂରଚନା strbuf path = STRBUF_INIT;

			quote_path(entry->path, ଖାଲି, &path, QUOTE_PATH_QUOTE_SP);
			strbuf_addbuf(&sb, &path);

			strbuf_release(&path);
		} ଅନ୍ୟଥା {
			strbuf_addstr(&sb, entry->path);
		}
	}
	ଯଦି (entry->type)
		strbuf_addf(&sb, "%ctype=%s", info_term, type_name(entry->type));

	fwrite(sb.buf, ଆକାର(ଅକ୍ଷର), sb.len, stdout);
	putchar(ଧାଡ଼ି_term);

	strbuf_release(&sb);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ finish_object__ma(ସଂରଚନା object *obj, ଅଚଳ ଅକ୍ଷର *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	ବାଛ (arg_missing_action) {
	ସ୍ଥିତି MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		ଫେରନ୍ତୁ;

	ସ୍ଥିତି MA_ALLOW_ANY:
		ଫେରନ୍ତୁ;

	ସ୍ଥିତି MA_PRINT:
	ସ୍ଥିତି MA_PRINT_INFO:
		add_missing_object_entry(&obj->oid, name, obj->type);
		ଫେରନ୍ତୁ;

	ସ୍ଥିତି MA_ALLOW_PROMISOR:
		ଯଦି (is_promisor_object(the_repository, &obj->oid))
			ଫେରନ୍ତୁ;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		ଫେରନ୍ତୁ;

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unhandled missing_action");
		ଫେରନ୍ତୁ;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ finish_commit(ସଂରଚନା commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = ଖାଲି;
	free_commit_buffer(the_repository->parsed_objects,
			   commit);
}

ସ୍ଥିର ଶୂନ୍ୟ show_commit(ସଂରଚନା commit *commit, ଶୂନ୍ୟ *data)
{
	ସଂରଚନା rev_list_info *info = data;
	ସଂରଚନା rev_info *revs = info->revs;

	display_progress(progress, ++progress_counter);

	ଯଦି (revs->do_not_die_on_missing_objects &&
	    oidset_contains(&revs->missing_commits, &commit->object.oid)) {
		finish_object__ma(&commit->object, ଖାଲି);
		ଫେରନ୍ତୁ;
	}

	ଯଦି (arg_missing_only) {
		finish_commit(commit);
		ଫେରନ୍ତୁ;
	}

	ଯଦି (show_disk_usage)
		total_disk_usage += get_object_disk_usage(&commit->object);

	ଯଦି (info->flags & REV_LIST_QUIET) {
		finish_commit(commit);
		ଫେରନ୍ତୁ;
	}

	graph_show_commit(revs->graph);

	ଯଦି (revs->ଗଣନା_2) {
		ଯଦି (commit->object.flags & PATCHSAME)
			revs->ଗଣନା_same++;
		ଅନ୍ୟଥା ଯଦି (commit->object.flags & SYMMETRIC_ବାମ)
			revs->ଗଣନା_ବାମ++;
		ଅନ୍ୟଥା
			revs->ଗଣନା_ଡାହାଣ++;
		finish_commit(commit);
		ଫେରନ୍ତୁ;
	}

	ଯଦି (info->show_timestamp)
		printf("%"PRItime" ", commit->date);
	ଯଦି (info->header_prefix)
		fputs(info->header_prefix, stdout);

	ଯଦି (revs->include_header) {
		ଯଦି (!revs->graph && ଧାଡ଼ି_term)
			fputs(get_revision_mark(revs, commit), stdout);
		ଯଦି (revs->abbrev_commit && revs->abbrev)
			fputs(repo_find_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		ଅନ୍ୟଥା
			fputs(oid_to_hex(&commit->object.oid), stdout);

		ଯଦି (!ଧାଡ଼ି_term) {
			ଯଦି (commit->object.flags & BOUNDARY)
				printf("%cboundary=yes", info_term);
		}
	}
	ଯଦି (revs->print_parents) {
		ସଂରଚନା commit_list *parents = commit->parents;
		ଯେତେବେଳେ (parents) {
			printf(" %s", oid_to_hex(&parents->item->object.oid));
			parents = parents->next;
		}
	}
	ଯଦି (revs->children.name) {
		ସଂରଚନା commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		ଯେତେବେଳେ (children) {
			printf(" %s", oid_to_hex(&children->item->object.oid));
			children = children->next;
		}
	}
	show_decorations(revs, commit);
	ଯଦି (revs->commit_format == CMIT_FMT_ONELINE)
		putchar(' ');
	ଅନ୍ୟଥା ଯଦି (revs->include_header)
		putchar(ଧାଡ଼ି_term);

	ଯଦି (revs->verbose_header) {
		ସଂରଚନା strbuf buf = STRBUF_INIT;
		ସଂରଚନା pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_mode = revs->date_mode;
		ctx.date_mode_explicit = revs->date_mode_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		ଯଦି (buf.len) {
			ଯଦି (revs->commit_format != CMIT_FMT_ONELINE)
				graph_show_oneline(revs->graph);

			graph_show_commit_ସନ୍ଦେଶ(revs->graph, stdout, &buf);

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
			ଯଦି (buf.len && buf.buf[buf.len - 1] == '\n')
				graph_show_padding(revs->graph);
			putchar(info->hdr_termination);
		} ଅନ୍ୟଥା {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			ଯଦି (graph_show_remainder(revs->graph))
				putchar('\n');
			ଯଦି (revs->commit_format == CMIT_FMT_ONELINE)
				putchar('\n');
		}
		strbuf_release(&buf);
	} ଅନ୍ୟଥା {
		ଯଦି (graph_show_remainder(revs->graph))
			putchar('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	finish_commit(commit);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା finish_object(ସଂରଚନା object *obj, ଅଚଳ ଅକ୍ଷର *name, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା rev_list_info *info = cb_data;
	ଯଦି (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, ଖାଲି, 0) < 0) {
		finish_object__ma(obj, name);
		ଫେରନ୍ତୁ 1;
	}
	ଯଦି (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ show_object(ସଂରଚନା object *obj, ଅଚଳ ଅକ୍ଷର *name, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା rev_list_info *info = cb_data;
	ସଂରଚନା rev_info *revs = info->revs;

	ଯଦି (finish_object(obj, name, cb_data))
		ଫେରନ୍ତୁ;
	display_progress(progress, ++progress_counter);
	ଯଦି (arg_missing_only)
		ଫେରନ୍ତୁ;
	ଯଦି (show_disk_usage)
		total_disk_usage += get_object_disk_usage(obj);
	ଯଦି (info->flags & REV_LIST_QUIET)
		ଫେରନ୍ତୁ;

	ଯଦି (revs->ଗଣନା_2) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->ଗଣନା_ଡାହାଣ++;
		ଫେରନ୍ତୁ;
	}

	printf("%s", oid_to_hex(&obj->oid));

	ଯଦି (arg_show_object_names) {
		ଯଦି (ଧାଡ଼ି_term) {
			putchar(info_term);
			ପାଇଁ (ଅଚଳ ଅକ୍ଷର *p = name; *p && *p != '\n'; p++)
				putchar(*p);
		} ଅନ୍ୟଥା ଯଦି (*name) {
			printf("%cpath=%s", info_term, name);
		}
	}

	putchar(ଧାଡ଼ି_term);
}

ସ୍ଥିର ଶୂନ୍ୟ show_edge(ସଂରଚନା commit *commit)
{
	printf("-%s\n", oid_to_hex(&commit->object.oid));
}

ସ୍ଥିର ଶୂନ୍ୟ print_var_str(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *val)
{
	printf("%s='%s'\n", var, val);
}

ସ୍ଥିର ଶୂନ୍ୟ print_var_int(ଅଚଳ ଅକ୍ଷର *var, ପୂର୍ଣ୍ଣସଂଖ୍ୟା val)
{
	printf("%s=%d\n", var, val);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_bisect_vars(ସଂରଚନା rev_list_info *info, ପୂର୍ଣ୍ଣସଂଖ୍ୟା reaches, ପୂର୍ଣ୍ଣସଂଖ୍ୟା all)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnt, flags = info->flags;
	ଅକ୍ଷର hex[GIT_MAX_HEXSZ + 1] = "";
	ସଂରଚନା commit_list *tried;
	ସଂରଚନା rev_info *revs = info->revs;

	ଯଦି (!revs->commits)
		ଫେରନ୍ତୁ 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       ଖାଲି, ଖାଲି);

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
	ଯଦି (cnt < reaches)
		cnt = reaches;

	ଯଦି (revs->commits)
		oid_to_hex_r(hex, &revs->commits->item->object.oid);

	ଯଦି (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		printf("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_steps(all));

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_object_fast(
	ଅଚଳ ସଂରଚନା object_id *oid,
	ଗଣନା object_type type UNUSED,
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude UNUSED,
	uint32_t name_hash UNUSED,
	ସଂରଚନା packed_git *found_pack UNUSED,
	off_t found_offset UNUSED,
	ଶୂନ୍ୟ *payload UNUSED)
{
	fprintf(stdout, "%s\n", oid_to_hex(oid));
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ print_disk_usage(off_t size)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଯଦି (human_readable)
		strbuf_humanise_bytes(&sb, size);
	ଅନ୍ୟଥା
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)size);
	puts(sb.buf);
	strbuf_release(&sb);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_missing_action_ମୂଲ୍ୟ(ସଂରଚନା repository *repo,
					     ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ଯଦି (!strcmp(ମୂଲ୍ୟ, "error")) {
		arg_missing_action = MA_ERROR;
		ଫେରନ୍ତୁ 1;
	}

	ଯଦି (!strcmp(ମୂଲ୍ୟ, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		ଫେରନ୍ତୁ 1;
	}

	ଯଦି (!strcmp(ମୂଲ୍ୟ, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		ଫେରନ୍ତୁ 1;
	}

	ଯଦି (!strcmp(ମୂଲ୍ୟ, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		ଫେରନ୍ତୁ 1;
	}

	ଯଦି (!strcmp(ମୂଲ୍ୟ, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		ଫେରନ୍ତୁ 1;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_bitmap_ଗଣନା(ସଂରଚନା rev_info *revs,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା filter_provided_objects)
{
	uint32_t commit_ଗଣନା = 0,
		 tag_ଗଣନା = 0,
		 ବୃକ୍ଷ_ଗଣନା = 0,
		 blob_ଗଣନା = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_ଗଣନା;
	ସଂରଚନା bitmap_index *bitmap_git;

	/* This function only handles counting, not general traversal. */
	ଯଦି (!revs->ଗଣନା_2)
		ଫେରନ୍ତୁ -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	ଯଦି (revs->ବାମ_ଡାହାଣ || revs->cherry_mark)
		ଫେରନ୍ତୁ -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	ଯଦି (revs->max_ଗଣନା >= 0 &&
	    (revs->tag_objects || revs->ବୃକ୍ଷ_objects || revs->blob_objects))
		ଫେରନ୍ତୁ -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_ଗଣନା = revs->max_ଗଣନା;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ଯଦି (!bitmap_git)
		ଫେରନ୍ତୁ -1;

	ଗଣନା_bitmap_commit_list(bitmap_git, &commit_ଗଣନା,
				 revs->ବୃକ୍ଷ_objects ? &ବୃକ୍ଷ_ଗଣନା : ଖାଲି,
				 revs->blob_objects ? &blob_ଗଣନା : ଖାଲି,
				 revs->tag_objects ? &tag_ଗଣନା : ଖାଲି);
	ଯଦି (max_ଗଣନା >= 0 && max_ଗଣନା < commit_ଗଣନା)
		commit_ଗଣନା = max_ଗଣନା;

	printf("%d\n", commit_ଗଣନା + ବୃକ୍ଷ_ଗଣନା + blob_ଗଣନା + tag_ଗଣନା);
	free_bitmap_index(bitmap_git);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_bitmap_traversal(ସଂରଚନା rev_info *revs,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା filter_provided_objects)
{
	ସଂରଚନା bitmap_index *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	ଯଦି (revs->max_ଗଣନା >= 0)
		ଫେରନ୍ତୁ -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	ଯଦି (revs->ବାମ_ଡାହାଣ)
		ଫେରନ୍ତୁ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ଯଦି (!bitmap_git)
		ଫେରନ୍ତୁ -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_index(bitmap_git);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_bitmap_disk_usage(ସଂରଚନା rev_info *revs,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା filter_provided_objects)
{
	ସଂରଚନା bitmap_index *bitmap_git;
	off_t size_from_bitmap;

	ଯଦି (!show_disk_usage)
		ଫେରନ୍ତୁ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ଯଦି (!bitmap_git)
		ଫେରନ୍ତୁ -1;

	size_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(size_from_bitmap);

	free_bitmap_index(bitmap_git);
	ଫେରନ୍ତୁ 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
ସ୍ଥିର ଶୂନ୍ୟ prepare_maximal_independent(ସଂରଚନା rev_info *revs)
{
	ସଂରଚନା commit_list *c;

	ଯଦି (!revs->maximal_only)
		ଫେରନ୍ତୁ;

	ପାଇଁ (c = revs->commits; c; c = c->next) {
		ଯଦି (c->item->object.flags & UNINTERESTING)
			ଫେରନ୍ତୁ;
	}

	ଯଦି (revs->limited ||
	    revs->topo_order ||
	    revs->first_parent_only ||
	    revs->reverse ||
	    revs->max_ଗଣନା >= 0 ||
	    revs->skip_ଗଣନା >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_data.nr ||
	    revs->ଗଣନା_2 ||
	    revs->ବାମ_ଡାହାଣ ||
	    revs->boundary ||
	    revs->tag_objects ||
	    revs->ବୃକ୍ଷ_objects ||
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
	    revs->ଧାଡ଼ି_level_traverse)
		ଫେରନ୍ତୁ;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_rev_list(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		 ଅଚଳ ଅକ୍ଷର **argv,
		 ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo)
{
	ସଂରଚନା rev_info revs;
	ସଂରଚନା rev_list_info info;
	ସଂରଚନା setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_list = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_show_vars = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_find_all = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_bitmap_index = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା filter_provided_objects = 0;
	ଅଚଳ ଅକ୍ଷର *show_progress = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, ଖାଲି);
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
	ପାଇଁ (i = 1; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];
		ଯଦି (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} ଅନ୍ୟଥା ଯଦି (skip_prefix(arg, "--missing=", &arg)) {
			parse_missing_action_ମୂଲ୍ୟ(repo, arg);
		} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "-z")) {
			ଧାଡ଼ି_term = '\0';
			info_term = '\0';
		}
	}

	ଯଦି (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	ଯଦି (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = setup_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, ଆକାର(info));
	info.revs = &revs;
	ଯଦି (revs.bisect)
		bisect_list = 1;

	ଯଦି (revs.diffopt.flags.quick)
		info.flags |= REV_LIST_QUIET;
	ପାଇଁ (i = 1 ; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];

		ଯଦି (!strcmp(arg, "--header")) {
			revs.verbose_header = 1;
			ଜାରି;
		}
		ଯଦି (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			ଜାରି;
		}
		ଯଦି (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			ଜାରି;
		}
		ଯଦି (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_find_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			ଜାରି;
		}
		ଯଦି (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			ଜାରି;
		}
		ଯଦି (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_index = 1;
			ଜାରି;
		}
		ଯଦି (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			ଯାଅ cleanup;
		}
		ଯଦି (skip_prefix(arg, "--progress=", &arg)) {
			show_progress = arg;
			ଜାରି;
		}
		ଯଦି (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			ଜାରି;
		}
		ଯଦି (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			ଜାରି;
		}

		ଯଦି (!strcmp(arg, "--exclude-promisor-objects"))
			ଜାରି; /* already handled above */
		ଯଦି (skip_prefix(arg, "--missing=", &arg))
			ଜାରି; /* already handled above */

		ଯଦି (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_names = 0;
			ଜାରି;
		}

		ଯଦି (!strcmp(arg, ("--object-names"))) {
			arg_show_object_names = 1;
			ଜାରି;
		}

		ଯଦି (!strcmp(arg, ("--commit-header"))) {
			revs.include_header = 1;
			ଜାରି;
		}

		ଯଦି (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_header = 0;
			ଜାରି;
		}

		ଯଦି (skip_prefix(arg, "--disk-usage", &arg)) {
			ଯଦି (*arg == '=') {
				ଯଦି (!strcmp(++arg, "human")) {
					human_readable = 1;
				} ଅନ୍ୟଥା
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} ଅନ୍ୟଥା ଯଦି (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			ଜାରି;
		}

		ଯଦି (!strcmp(arg, "--missing-only"))
			ଜାରି;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	ଯଦି (!ଧାଡ଼ି_term) {
		ଯଦି (revs.graph || revs.verbose_header || show_disk_usage ||
		    info.show_timestamp || info.header_prefix || bisect_list ||
		    use_bitmap_index || revs.edge_hint || revs.ବାମ_ଡାହାଣ ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	ଯଦି (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_header = 1;
	ଯଦି (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		ଯଦି (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_header)
			info.header_prefix = "";
		ଅନ୍ୟଥା
			info.header_prefix = "commit ";
	}
	ଅନ୍ୟଥା ଯଦି (revs.verbose_header)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	ଯଦି ((!revs.commits && reflog_walk_empty(revs.reflog_info) &&
	     (!(revs.tag_objects || revs.ବୃକ୍ଷ_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_ନିବେଶ_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	ଯଦି (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	ଯଦି (revs.ଗଣନା_2 &&
	    (revs.tag_objects || revs.ବୃକ୍ଷ_objects || revs.blob_objects) &&
	    (revs.ବାମ_ଡାହାଣ || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.ଗଣନା_2, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_buffer = (revs.verbose_header ||
			      revs.grep_filter.pattern_list ||
			      revs.grep_filter.header_list);
	ଯଦି (bisect_list)
		revs.limited = 1;

	ଯଦି (show_progress)
		progress = start_delayed_progress(the_repository,
						  show_progress, 0);

	ଯଦି (use_bitmap_index) {
		ଯଦି (!try_bitmap_ଗଣନା(&revs, filter_provided_objects))
			ଯାଅ cleanup;
		ଯଦି (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			ଯାଅ cleanup;
		ଯଦି (!try_bitmap_traversal(&revs, filter_provided_objects))
			ଯାଅ cleanup;
	}

	ଯଦି (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	ଯଦି (revs.ବୃକ୍ଷ_objects)
		mark_edges_uninteresting(&revs, show_edge, 0);

	ଯଦି (bisect_list) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା reaches, all;
		unsigned bisect_flags = 0;

		ଯଦି (bisect_find_all)
			bisect_flags |= FIND_BISECTION_ALL;

		ଯଦି (revs.first_parent_only)
			bisect_flags |= FIND_BISECTION_FIRST_PARENT_ONLY;

		find_bisection(&revs.commits, &reaches, &all, bisect_flags);

		ଯଦି (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			ଯାଅ cleanup;
		}
	}

	ଯଦି (filter_provided_objects) {
		ସଂରଚନା commit_list *c;
		ପାଇଁ (i = 0; i < revs.pending.nr; i++) {
			ସଂରଚନା object_array_entry *pending = revs.pending.objects + i;
			pending->item->flags |= NOT_USER_GIVEN;
		}
		ପାଇଁ (c = revs.commits; c; c = c->next)
			c->item->object.flags |= NOT_USER_GIVEN;
	}

	ଯଦି (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_SIZE);
	ଯଦି (should_collect_missing()) {
		ସଂରଚନା oidset_iter iter;
		ସଂରଚନା object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_SIZE);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		ଯେତେବେଳେ ((oid = oidset_iter_next(&iter)))
			add_missing_object_entry(oid, ଖାଲି, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : ଖାଲି));

	ଯଦି (arg_print_omitted) {
		ସଂରଚନା oidset_iter iter;
		ସଂରଚନା object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		ଯେତେବେଳେ ((oid = oidset_iter_next(&iter)))
			printf("~%s\n", oid_to_hex(oid));
		oidset_clear(&omitted_objects);
	}
	ଯଦି (should_collect_missing()) {
		ସଂରଚନା missing_objects_map_entry *entry;
		ସଂରଚନା oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		ଯେତେବେଳେ ((entry = oidmap_iter_next(&iter))) {
			print_missing_object(entry, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_entry_free);
	}

	stop_progress(&progress);

	ଯଦି (revs.ଗଣନା_2) {
		ଯଦି (revs.ବାମ_ଡାହାଣ && revs.cherry_mark)
			printf("%d\t%d\t%d\n", revs.ଗଣନା_ବାମ, revs.ଗଣନା_ଡାହାଣ, revs.ଗଣନା_same);
		ଅନ୍ୟଥା ଯଦି (revs.ବାମ_ଡାହାଣ)
			printf("%d\t%d\n", revs.ଗଣନା_ବାମ, revs.ଗଣନା_ଡାହାଣ);
		ଅନ୍ୟଥା ଯଦି (revs.cherry_mark)
			printf("%d\t%d\n", revs.ଗଣନା_ବାମ + revs.ଗଣନା_ଡାହାଣ, revs.ଗଣନା_same);
		ଅନ୍ୟଥା
			printf("%d\n", revs.ଗଣନା_ବାମ + revs.ଗଣନା_ଡାହାଣ);
	}

	ଯଦି (show_disk_usage)
		print_disk_usage(total_disk_usage);

cleanup:
	release_revisions(&revs);
	ଫେରନ୍ତୁ ret;
}
