#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "list-objects.h"
#அந்தர்பாவ "list-objects-filter-options.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "pack-bitmap.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "log-tree.h"
#அந்தர்பாவ "graph.h"
#அந்தர்பாவ "bisect.h"
#அந்தர்பாவ "progress.h"
#அந்தர்பாவ "reflog-walk.h"
#அந்தர்பாவ "oidset.h"
#அந்தர்பாவ "oidmap.h"
#அந்தர்பாவ "packfile.h"
#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "strbuf.h"

கட்டமைப்பு rev_list_info {
	கட்டமைப்பு rev_info *revs;
	முழு flags;
	முழு show_timestamp;
	முழு hdr_termination;
	மாறிலி எழுத்து *header_prefix;
};

நிலையான மாறிலி எழுத்து rev_list_usage[] =
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

நிலையான கட்டமைப்பு progress *progress;
நிலையான unsigned progress_counter;

நிலையான கட்டமைப்பு oidset omitted_objects;
நிலையான முழு arg_print_omitted; /* print objects omitted by filter */

கட்டமைப்பு missing_objects_map_entry {
	கட்டமைப்பு oidmap_entry entry;
	எழுத்து *path;
	unsigned type;
};

நிலையான வெறுமை missing_objects_map_entry_free(வெறுமை *e)
{
	கட்டமைப்பு missing_objects_map_entry *entry =
		container_of(e, கட்டமைப்பு missing_objects_map_entry, entry);

	விடுவி(entry->path);
	விடுவி(entry);
}

நிலையான கட்டமைப்பு oidmap missing_objects;
எண்ணிக்கை missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
நிலையான எண்ணிக்கை missing_action arg_missing_action;
நிலையான முழு arg_missing_only;

நிலையான உள்வரி முழு should_collect_missing(வெறுமை)
{
	திரும்பு arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
நிலையான முழு arg_show_object_names = 1;

#define DEFAULT_OIDSET_SIZE     (16*1024)

நிலையான எழுத்து வரி_term = '\n';
நிலையான எழுத்து info_term = ' ';

நிலையான முழு show_disk_usage;
நிலையான off_t total_disk_usage;
நிலையான முழு human_readable;

நிலையான off_t get_object_disk_usage(கட்டமைப்பு object *obj)
{
	off_t size;
	கட்டமைப்பு object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &size;
	எனில் (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_to_hex(&obj->oid));
	திரும்பு size;
}

நிலையான வெறுமை add_missing_object_entry(கட்டமைப்பு object_id *oid, மாறிலி எழுத்து *path,
				     unsigned type)
{
	கட்டமைப்பு missing_objects_map_entry *entry;

	எனில் (oidmap_get(&missing_objects, oid))
		திரும்பு;

	CALLOC_ARRAY(entry, 1);
	entry->entry.oid = *oid;
	entry->type = type;
	எனில் (path)
		entry->path = xstrdup(path);
	oidmap_put(&missing_objects, entry);
}

நிலையான வெறுமை print_missing_object(கட்டமைப்பு missing_objects_map_entry *entry,
				 முழு print_missing_info)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	எனில் (arg_missing_only && வரி_term)
		printf("%s", oid_to_hex(&entry->entry.oid));
	இல்லையெனில் எனில் (வரி_term)
		printf("?%s", oid_to_hex(&entry->entry.oid));
	இல்லையெனில்
		printf("%s%cmissing=yes", oid_to_hex(&entry->entry.oid),
		       info_term);

	எனில் (!print_missing_info) {
		putchar(வரி_term);
		திரும்பு;
	}

	எனில் (entry->path && *entry->path) {
		strbuf_addf(&sb, "%cpath=", info_term);

		எனில் (வரி_term) {
			கட்டமைப்பு strbuf path = STRBUF_INIT;

			quote_path(entry->path, வெற்று, &path, QUOTE_PATH_QUOTE_SP);
			strbuf_addbuf(&sb, &path);

			strbuf_release(&path);
		} இல்லையெனில் {
			strbuf_addstr(&sb, entry->path);
		}
	}
	எனில் (entry->type)
		strbuf_addf(&sb, "%ctype=%s", info_term, type_name(entry->type));

	fwrite(sb.buf, அளவு(எழுத்து), sb.len, stdout);
	putchar(வரி_term);

	strbuf_release(&sb);
}

நிலையான உள்வரி வெறுமை finish_object__ma(கட்டமைப்பு object *obj, மாறிலி எழுத்து *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	தேர்வு (arg_missing_action) {
	நிலை MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		திரும்பு;

	நிலை MA_ALLOW_ANY:
		திரும்பு;

	நிலை MA_PRINT:
	நிலை MA_PRINT_INFO:
		add_missing_object_entry(&obj->oid, name, obj->type);
		திரும்பு;

	நிலை MA_ALLOW_PROMISOR:
		எனில் (is_promisor_object(the_repository, &obj->oid))
			திரும்பு;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		திரும்பு;

	இயல்பு:
		BUG("unhandled missing_action");
		திரும்பு;
	}
}

நிலையான வெறுமை finish_commit(கட்டமைப்பு commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = வெற்று;
	free_commit_buffer(the_repository->parsed_objects,
			   commit);
}

நிலையான வெறுமை show_commit(கட்டமைப்பு commit *commit, வெறுமை *data)
{
	கட்டமைப்பு rev_list_info *info = data;
	கட்டமைப்பு rev_info *revs = info->revs;

	display_progress(progress, ++progress_counter);

	எனில் (revs->do_not_die_on_missing_objects &&
	    oidset_contains(&revs->missing_commits, &commit->object.oid)) {
		finish_object__ma(&commit->object, வெற்று);
		திரும்பு;
	}

	எனில் (arg_missing_only) {
		finish_commit(commit);
		திரும்பு;
	}

	எனில் (show_disk_usage)
		total_disk_usage += get_object_disk_usage(&commit->object);

	எனில் (info->flags & REV_LIST_QUIET) {
		finish_commit(commit);
		திரும்பு;
	}

	graph_show_commit(revs->graph);

	எனில் (revs->மொத்தம்) {
		எனில் (commit->object.flags & PATCHSAME)
			revs->மொத்தம்_same++;
		இல்லையெனில் எனில் (commit->object.flags & SYMMETRIC_இடது)
			revs->மொத்தம்_இடது++;
		இல்லையெனில்
			revs->மொத்தம்_வலது++;
		finish_commit(commit);
		திரும்பு;
	}

	எனில் (info->show_timestamp)
		printf("%"PRItime" ", commit->date);
	எனில் (info->header_prefix)
		fputs(info->header_prefix, stdout);

	எனில் (revs->include_header) {
		எனில் (!revs->graph && வரி_term)
			fputs(get_revision_mark(revs, commit), stdout);
		எனில் (revs->abbrev_commit && revs->abbrev)
			fputs(repo_find_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		இல்லையெனில்
			fputs(oid_to_hex(&commit->object.oid), stdout);

		எனில் (!வரி_term) {
			எனில் (commit->object.flags & BOUNDARY)
				printf("%cboundary=yes", info_term);
		}
	}
	எனில் (revs->print_parents) {
		கட்டமைப்பு commit_list *parents = commit->parents;
		போது (parents) {
			printf(" %s", oid_to_hex(&parents->item->object.oid));
			parents = parents->next;
		}
	}
	எனில் (revs->children.name) {
		கட்டமைப்பு commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		போது (children) {
			printf(" %s", oid_to_hex(&children->item->object.oid));
			children = children->next;
		}
	}
	show_decorations(revs, commit);
	எனில் (revs->commit_format == CMIT_FMT_ONELINE)
		putchar(' ');
	இல்லையெனில் எனில் (revs->include_header)
		putchar(வரி_term);

	எனில் (revs->verbose_header) {
		கட்டமைப்பு strbuf buf = STRBUF_INIT;
		கட்டமைப்பு pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_mode = revs->date_mode;
		ctx.date_mode_explicit = revs->date_mode_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		எனில் (buf.len) {
			எனில் (revs->commit_format != CMIT_FMT_ONELINE)
				graph_show_oneline(revs->graph);

			graph_show_commit_செய்தி(revs->graph, stdout, &buf);

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
			எனில் (buf.len && buf.buf[buf.len - 1] == '\n')
				graph_show_padding(revs->graph);
			putchar(info->hdr_termination);
		} இல்லையெனில் {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			எனில் (graph_show_remainder(revs->graph))
				putchar('\n');
			எனில் (revs->commit_format == CMIT_FMT_ONELINE)
				putchar('\n');
		}
		strbuf_release(&buf);
	} இல்லையெனில் {
		எனில் (graph_show_remainder(revs->graph))
			putchar('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	finish_commit(commit);
}

நிலையான முழு finish_object(கட்டமைப்பு object *obj, மாறிலி எழுத்து *name, வெறுமை *cb_data)
{
	கட்டமைப்பு rev_list_info *info = cb_data;
	எனில் (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, வெற்று, 0) < 0) {
		finish_object__ma(obj, name);
		திரும்பு 1;
	}
	எனில் (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	திரும்பு 0;
}

நிலையான வெறுமை show_object(கட்டமைப்பு object *obj, மாறிலி எழுத்து *name, வெறுமை *cb_data)
{
	கட்டமைப்பு rev_list_info *info = cb_data;
	கட்டமைப்பு rev_info *revs = info->revs;

	எனில் (finish_object(obj, name, cb_data))
		திரும்பு;
	display_progress(progress, ++progress_counter);
	எனில் (arg_missing_only)
		திரும்பு;
	எனில் (show_disk_usage)
		total_disk_usage += get_object_disk_usage(obj);
	எனில் (info->flags & REV_LIST_QUIET)
		திரும்பு;

	எனில் (revs->மொத்தம்) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->மொத்தம்_வலது++;
		திரும்பு;
	}

	printf("%s", oid_to_hex(&obj->oid));

	எனில் (arg_show_object_names) {
		எனில் (வரி_term) {
			putchar(info_term);
			க்கு (மாறிலி எழுத்து *p = name; *p && *p != '\n'; p++)
				putchar(*p);
		} இல்லையெனில் எனில் (*name) {
			printf("%cpath=%s", info_term, name);
		}
	}

	putchar(வரி_term);
}

நிலையான வெறுமை show_edge(கட்டமைப்பு commit *commit)
{
	printf("-%s\n", oid_to_hex(&commit->object.oid));
}

நிலையான வெறுமை print_var_str(மாறிலி எழுத்து *var, மாறிலி எழுத்து *val)
{
	printf("%s='%s'\n", var, val);
}

நிலையான வெறுமை print_var_int(மாறிலி எழுத்து *var, முழு val)
{
	printf("%s=%d\n", var, val);
}

நிலையான முழு show_bisect_vars(கட்டமைப்பு rev_list_info *info, முழு reaches, முழு all)
{
	முழு cnt, flags = info->flags;
	எழுத்து hex[GIT_MAX_HEXSZ + 1] = "";
	கட்டமைப்பு commit_list *tried;
	கட்டமைப்பு rev_info *revs = info->revs;

	எனில் (!revs->commits)
		திரும்பு 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       வெற்று, வெற்று);

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
	எனில் (cnt < reaches)
		cnt = reaches;

	எனில் (revs->commits)
		oid_to_hex_r(hex, &revs->commits->item->object.oid);

	எனில் (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		printf("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_steps(all));

	திரும்பு 0;
}

நிலையான முழு show_object_fast(
	மாறிலி கட்டமைப்பு object_id *oid,
	எண்ணிக்கை object_type type UNUSED,
	முழு exclude UNUSED,
	uint32_t name_hash UNUSED,
	கட்டமைப்பு packed_git *found_pack UNUSED,
	off_t found_offset UNUSED,
	வெறுமை *payload UNUSED)
{
	fprintf(stdout, "%s\n", oid_to_hex(oid));
	திரும்பு 0;
}

நிலையான வெறுமை print_disk_usage(off_t size)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	எனில் (human_readable)
		strbuf_humanise_bytes(&sb, size);
	இல்லையெனில்
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)size);
	puts(sb.buf);
	strbuf_release(&sb);
}

நிலையான உள்வரி முழு parse_missing_action_மதிப்பு(கட்டமைப்பு repository *repo,
					     மாறிலி எழுத்து *மதிப்பு)
{
	எனில் (!strcmp(மதிப்பு, "error")) {
		arg_missing_action = MA_ERROR;
		திரும்பு 1;
	}

	எனில் (!strcmp(மதிப்பு, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		திரும்பு 1;
	}

	எனில் (!strcmp(மதிப்பு, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		திரும்பு 1;
	}

	எனில் (!strcmp(மதிப்பு, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		திரும்பு 1;
	}

	எனில் (!strcmp(மதிப்பு, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		திரும்பு 1;
	}

	திரும்பு 0;
}

நிலையான முழு try_bitmap_மொத்தம்(கட்டமைப்பு rev_info *revs,
			    முழு filter_provided_objects)
{
	uint32_t commit_மொத்தம் = 0,
		 tag_மொத்தம் = 0,
		 மரம்_மொத்தம் = 0,
		 blob_மொத்தம் = 0;
	முழு max_மொத்தம்;
	கட்டமைப்பு bitmap_index *bitmap_git;

	/* This function only handles counting, not general traversal. */
	எனில் (!revs->மொத்தம்)
		திரும்பு -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	எனில் (revs->இடது_வலது || revs->cherry_mark)
		திரும்பு -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	எனில் (revs->max_மொத்தம் >= 0 &&
	    (revs->tag_objects || revs->மரம்_objects || revs->blob_objects))
		திரும்பு -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_மொத்தம் = revs->max_மொத்தம்;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	எனில் (!bitmap_git)
		திரும்பு -1;

	மொத்தம்_bitmap_commit_list(bitmap_git, &commit_மொத்தம்,
				 revs->மரம்_objects ? &மரம்_மொத்தம் : வெற்று,
				 revs->blob_objects ? &blob_மொத்தம் : வெற்று,
				 revs->tag_objects ? &tag_மொத்தம் : வெற்று);
	எனில் (max_மொத்தம் >= 0 && max_மொத்தம் < commit_மொத்தம்)
		commit_மொத்தம் = max_மொத்தம்;

	printf("%d\n", commit_மொத்தம் + மரம்_மொத்தம் + blob_மொத்தம் + tag_மொத்தம்);
	free_bitmap_index(bitmap_git);
	திரும்பு 0;
}

நிலையான முழு try_bitmap_traversal(கட்டமைப்பு rev_info *revs,
				முழு filter_provided_objects)
{
	கட்டமைப்பு bitmap_index *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	எனில் (revs->max_மொத்தம் >= 0)
		திரும்பு -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	எனில் (revs->இடது_வலது)
		திரும்பு -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	எனில் (!bitmap_git)
		திரும்பு -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_index(bitmap_git);
	திரும்பு 0;
}

நிலையான முழு try_bitmap_disk_usage(கட்டமைப்பு rev_info *revs,
				 முழு filter_provided_objects)
{
	கட்டமைப்பு bitmap_index *bitmap_git;
	off_t size_from_bitmap;

	எனில் (!show_disk_usage)
		திரும்பு -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	எனில் (!bitmap_git)
		திரும்பு -1;

	size_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(size_from_bitmap);

	free_bitmap_index(bitmap_git);
	திரும்பு 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
நிலையான வெறுமை prepare_maximal_independent(கட்டமைப்பு rev_info *revs)
{
	கட்டமைப்பு commit_list *c;

	எனில் (!revs->maximal_only)
		திரும்பு;

	க்கு (c = revs->commits; c; c = c->next) {
		எனில் (c->item->object.flags & UNINTERESTING)
			திரும்பு;
	}

	எனில் (revs->limited ||
	    revs->topo_order ||
	    revs->first_parent_only ||
	    revs->reverse ||
	    revs->max_மொத்தம் >= 0 ||
	    revs->skip_மொத்தம் >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_data.nr ||
	    revs->மொத்தம் ||
	    revs->இடது_வலது ||
	    revs->boundary ||
	    revs->tag_objects ||
	    revs->மரம்_objects ||
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
	    revs->வரி_level_traverse)
		திரும்பு;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

முழு cmd_rev_list(முழு argc,
		 மாறிலி எழுத்து **argv,
		 மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு rev_info revs;
	கட்டமைப்பு rev_list_info info;
	கட்டமைப்பு setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	முழு i;
	முழு bisect_list = 0;
	முழு bisect_show_vars = 0;
	முழு bisect_find_all = 0;
	முழு use_bitmap_index = 0;
	முழு filter_provided_objects = 0;
	மாறிலி எழுத்து *show_progress = வெற்று;
	முழு ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, வெற்று);
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
	க்கு (i = 1; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];
		எனில் (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} இல்லையெனில் எனில் (skip_prefix(arg, "--missing=", &arg)) {
			parse_missing_action_மதிப்பு(repo, arg);
		} இல்லையெனில் எனில் (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} இல்லையெனில் எனில் (!strcmp(arg, "-z")) {
			வரி_term = '\0';
			info_term = '\0';
		}
	}

	எனில் (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	எனில் (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = setup_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, அளவு(info));
	info.revs = &revs;
	எனில் (revs.bisect)
		bisect_list = 1;

	எனில் (revs.diffopt.flags.quick)
		info.flags |= REV_LIST_QUIET;
	க்கு (i = 1 ; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];

		எனில் (!strcmp(arg, "--header")) {
			revs.verbose_header = 1;
			தொடர்;
		}
		எனில் (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			தொடர்;
		}
		எனில் (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			தொடர்;
		}
		எனில் (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_find_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			தொடர்;
		}
		எனில் (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			தொடர்;
		}
		எனில் (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_index = 1;
			தொடர்;
		}
		எனில் (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			செல் cleanup;
		}
		எனில் (skip_prefix(arg, "--progress=", &arg)) {
			show_progress = arg;
			தொடர்;
		}
		எனில் (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			தொடர்;
		}
		எனில் (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			தொடர்;
		}

		எனில் (!strcmp(arg, "--exclude-promisor-objects"))
			தொடர்; /* already handled above */
		எனில் (skip_prefix(arg, "--missing=", &arg))
			தொடர்; /* already handled above */

		எனில் (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_names = 0;
			தொடர்;
		}

		எனில் (!strcmp(arg, ("--object-names"))) {
			arg_show_object_names = 1;
			தொடர்;
		}

		எனில் (!strcmp(arg, ("--commit-header"))) {
			revs.include_header = 1;
			தொடர்;
		}

		எனில் (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_header = 0;
			தொடர்;
		}

		எனில் (skip_prefix(arg, "--disk-usage", &arg)) {
			எனில் (*arg == '=') {
				எனில் (!strcmp(++arg, "human")) {
					human_readable = 1;
				} இல்லையெனில்
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} இல்லையெனில் எனில் (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			தொடர்;
		}

		எனில் (!strcmp(arg, "--missing-only"))
			தொடர்;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	எனில் (!வரி_term) {
		எனில் (revs.graph || revs.verbose_header || show_disk_usage ||
		    info.show_timestamp || info.header_prefix || bisect_list ||
		    use_bitmap_index || revs.edge_hint || revs.இடது_வலது ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	எனில் (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_header = 1;
	எனில் (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		எனில் (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_header)
			info.header_prefix = "";
		இல்லையெனில்
			info.header_prefix = "commit ";
	}
	இல்லையெனில் எனில் (revs.verbose_header)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	எனில் ((!revs.commits && reflog_walk_empty(revs.reflog_info) &&
	     (!(revs.tag_objects || revs.மரம்_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_உள்ளீடு_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	எனில் (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	எனில் (revs.மொத்தம் &&
	    (revs.tag_objects || revs.மரம்_objects || revs.blob_objects) &&
	    (revs.இடது_வலது || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.மொத்தம், "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_buffer = (revs.verbose_header ||
			      revs.grep_filter.pattern_list ||
			      revs.grep_filter.header_list);
	எனில் (bisect_list)
		revs.limited = 1;

	எனில் (show_progress)
		progress = start_delayed_progress(the_repository,
						  show_progress, 0);

	எனில் (use_bitmap_index) {
		எனில் (!try_bitmap_மொத்தம்(&revs, filter_provided_objects))
			செல் cleanup;
		எனில் (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			செல் cleanup;
		எனில் (!try_bitmap_traversal(&revs, filter_provided_objects))
			செல் cleanup;
	}

	எனில் (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	எனில் (revs.மரம்_objects)
		mark_edges_uninteresting(&revs, show_edge, 0);

	எனில் (bisect_list) {
		முழு reaches, all;
		unsigned bisect_flags = 0;

		எனில் (bisect_find_all)
			bisect_flags |= FIND_BISECTION_ALL;

		எனில் (revs.first_parent_only)
			bisect_flags |= FIND_BISECTION_FIRST_PARENT_ONLY;

		find_bisection(&revs.commits, &reaches, &all, bisect_flags);

		எனில் (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			செல் cleanup;
		}
	}

	எனில் (filter_provided_objects) {
		கட்டமைப்பு commit_list *c;
		க்கு (i = 0; i < revs.pending.nr; i++) {
			கட்டமைப்பு object_array_entry *pending = revs.pending.objects + i;
			pending->item->flags |= NOT_USER_GIVEN;
		}
		க்கு (c = revs.commits; c; c = c->next)
			c->item->object.flags |= NOT_USER_GIVEN;
	}

	எனில் (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_SIZE);
	எனில் (should_collect_missing()) {
		கட்டமைப்பு oidset_iter iter;
		கட்டமைப்பு object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_SIZE);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		போது ((oid = oidset_iter_next(&iter)))
			add_missing_object_entry(oid, வெற்று, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : வெற்று));

	எனில் (arg_print_omitted) {
		கட்டமைப்பு oidset_iter iter;
		கட்டமைப்பு object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		போது ((oid = oidset_iter_next(&iter)))
			printf("~%s\n", oid_to_hex(oid));
		oidset_clear(&omitted_objects);
	}
	எனில் (should_collect_missing()) {
		கட்டமைப்பு missing_objects_map_entry *entry;
		கட்டமைப்பு oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		போது ((entry = oidmap_iter_next(&iter))) {
			print_missing_object(entry, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_entry_free);
	}

	stop_progress(&progress);

	எனில் (revs.மொத்தம்) {
		எனில் (revs.இடது_வலது && revs.cherry_mark)
			printf("%d\t%d\t%d\n", revs.மொத்தம்_இடது, revs.மொத்தம்_வலது, revs.மொத்தம்_same);
		இல்லையெனில் எனில் (revs.இடது_வலது)
			printf("%d\t%d\n", revs.மொத்தம்_இடது, revs.மொத்தம்_வலது);
		இல்லையெனில் எனில் (revs.cherry_mark)
			printf("%d\t%d\n", revs.மொத்தம்_இடது + revs.மொத்தம்_வலது, revs.மொத்தம்_same);
		இல்லையெனில்
			printf("%d\n", revs.மொத்தம்_இடது + revs.மொத்தம்_வலது);
	}

	எனில் (show_disk_usage)
		print_disk_usage(total_disk_usage);

cleanup:
	release_revisions(&revs);
	திரும்பு ret;
}
