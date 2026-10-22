#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "list-objects.h"
#અન્તર્ભાવ "list-objects-filter-options.h"
#અન્તર્ભાવ "object.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "pack-bitmap.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "log-tree.h"
#અન્તર્ભાવ "graph.h"
#અન્તર્ભાવ "bisect.h"
#અન્તર્ભાવ "progress.h"
#અન્તર્ભાવ "reflog-walk.h"
#અન્તર્ભાવ "oidset.h"
#અન્તર્ભાવ "oidmap.h"
#અન્તર્ભાવ "packfile.h"
#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "strbuf.h"

સંરચના rev_list_info {
	સંરચના rev_info *revs;
	પૂર્ણાંક flags;
	પૂર્ણાંક show_timestamp;
	પૂર્ણાંક hdr_termination;
	અચલ અક્ષર *header_prefix;
};

સ્થિર અચલ અક્ષર rev_list_usage[] =
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

સ્થિર સંરચના progress *progress;
સ્થિર unsigned progress_counter;

સ્થિર સંરચના oidset omitted_objects;
સ્થિર પૂર્ણાંક arg_print_omitted; /* print objects omitted by filter */

સંરચના missing_objects_map_entry {
	સંરચના oidmap_entry entry;
	અક્ષર *path;
	unsigned type;
};

સ્થિર શૂન્ય missing_objects_map_entry_free(શૂન્ય *e)
{
	સંરચના missing_objects_map_entry *entry =
		container_of(e, સંરચના missing_objects_map_entry, entry);

	મુક્ત(entry->path);
	મુક્ત(entry);
}

સ્થિર સંરચના oidmap missing_objects;
ગણના missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
સ્થિર ગણના missing_action arg_missing_action;
સ્થિર પૂર્ણાંક arg_missing_only;

સ્થિર અંતઃરેખ પૂર્ણાંક should_collect_missing(શૂન્ય)
{
	પાછા arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
સ્થિર પૂર્ણાંક arg_show_object_names = 1;

#define DEFAULT_OIDSET_SIZE     (16*1024)

સ્થિર અક્ષર લીટી_term = '\n';
સ્થિર અક્ષર info_term = ' ';

સ્થિર પૂર્ણાંક show_disk_usage;
સ્થિર off_t total_disk_usage;
સ્થિર પૂર્ણાંક human_readable;

સ્થિર off_t get_object_disk_usage(સંરચના object *obj)
{
	off_t size;
	સંરચના object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &size;
	જો (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_to_hex(&obj->oid));
	પાછા size;
}

સ્થિર શૂન્ય add_missing_object_entry(સંરચના object_id *oid, અચલ અક્ષર *path,
				     unsigned type)
{
	સંરચના missing_objects_map_entry *entry;

	જો (oidmap_get(&missing_objects, oid))
		પાછા;

	CALLOC_ARRAY(entry, 1);
	entry->entry.oid = *oid;
	entry->type = type;
	જો (path)
		entry->path = xstrdup(path);
	oidmap_put(&missing_objects, entry);
}

સ્થિર શૂન્ય print_missing_object(સંરચના missing_objects_map_entry *entry,
				 પૂર્ણાંક print_missing_info)
{
	સંરચના strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	જો (arg_missing_only && લીટી_term)
		printf("%s", oid_to_hex(&entry->entry.oid));
	નહીંતર જો (લીટી_term)
		printf("?%s", oid_to_hex(&entry->entry.oid));
	નહીંતર
		printf("%s%cmissing=yes", oid_to_hex(&entry->entry.oid),
		       info_term);

	જો (!print_missing_info) {
		putchar(લીટી_term);
		પાછા;
	}

	જો (entry->path && *entry->path) {
		strbuf_addf(&sb, "%cpath=", info_term);

		જો (લીટી_term) {
			સંરચના strbuf path = STRBUF_INIT;

			quote_path(entry->path, ખાલી, &path, QUOTE_PATH_QUOTE_SP);
			strbuf_addbuf(&sb, &path);

			strbuf_release(&path);
		} નહીંતર {
			strbuf_addstr(&sb, entry->path);
		}
	}
	જો (entry->type)
		strbuf_addf(&sb, "%ctype=%s", info_term, type_name(entry->type));

	fwrite(sb.buf, કદ(અક્ષર), sb.len, stdout);
	putchar(લીટી_term);

	strbuf_release(&sb);
}

સ્થિર અંતઃરેખ શૂન્ય finish_object__ma(સંરચના object *obj, અચલ અક્ષર *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	પસંદગી (arg_missing_action) {
	સ્થિતિ MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		પાછા;

	સ્થિતિ MA_ALLOW_ANY:
		પાછા;

	સ્થિતિ MA_PRINT:
	સ્થિતિ MA_PRINT_INFO:
		add_missing_object_entry(&obj->oid, name, obj->type);
		પાછા;

	સ્થિતિ MA_ALLOW_PROMISOR:
		જો (is_promisor_object(the_repository, &obj->oid))
			પાછા;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		પાછા;

	મૂળભૂત:
		BUG("unhandled missing_action");
		પાછા;
	}
}

સ્થિર શૂન્ય finish_commit(સંરચના commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = ખાલી;
	free_commit_buffer(the_repository->parsed_objects,
			   commit);
}

સ્થિર શૂન્ય show_commit(સંરચના commit *commit, શૂન્ય *data)
{
	સંરચના rev_list_info *info = data;
	સંરચના rev_info *revs = info->revs;

	display_progress(progress, ++progress_counter);

	જો (revs->do_not_die_on_missing_objects &&
	    oidset_contains(&revs->missing_commits, &commit->object.oid)) {
		finish_object__ma(&commit->object, ખાલી);
		પાછા;
	}

	જો (arg_missing_only) {
		finish_commit(commit);
		પાછા;
	}

	જો (show_disk_usage)
		total_disk_usage += get_object_disk_usage(&commit->object);

	જો (info->flags & REV_LIST_QUIET) {
		finish_commit(commit);
		પાછા;
	}

	graph_show_commit(revs->graph);

	જો (revs->ગણતરી) {
		જો (commit->object.flags & PATCHSAME)
			revs->ગણતરી_same++;
		નહીંતર જો (commit->object.flags & SYMMETRIC_ડાબું)
			revs->ગણતરી_ડાબું++;
		નહીંતર
			revs->ગણતરી_જમણું++;
		finish_commit(commit);
		પાછા;
	}

	જો (info->show_timestamp)
		printf("%"PRItime" ", commit->date);
	જો (info->header_prefix)
		fputs(info->header_prefix, stdout);

	જો (revs->include_header) {
		જો (!revs->graph && લીટી_term)
			fputs(get_revision_mark(revs, commit), stdout);
		જો (revs->abbrev_commit && revs->abbrev)
			fputs(repo_find_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		નહીંતર
			fputs(oid_to_hex(&commit->object.oid), stdout);

		જો (!લીટી_term) {
			જો (commit->object.flags & BOUNDARY)
				printf("%cboundary=yes", info_term);
		}
	}
	જો (revs->print_parents) {
		સંરચના commit_list *parents = commit->parents;
		જ્યારે (parents) {
			printf(" %s", oid_to_hex(&parents->item->object.oid));
			parents = parents->next;
		}
	}
	જો (revs->children.name) {
		સંરચના commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		જ્યારે (children) {
			printf(" %s", oid_to_hex(&children->item->object.oid));
			children = children->next;
		}
	}
	show_decorations(revs, commit);
	જો (revs->commit_format == CMIT_FMT_ONELINE)
		putchar(' ');
	નહીંતર જો (revs->include_header)
		putchar(લીટી_term);

	જો (revs->verbose_header) {
		સંરચના strbuf buf = STRBUF_INIT;
		સંરચના pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_mode = revs->date_mode;
		ctx.date_mode_explicit = revs->date_mode_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		જો (buf.len) {
			જો (revs->commit_format != CMIT_FMT_ONELINE)
				graph_show_oneline(revs->graph);

			graph_show_commit_સંદેશ(revs->graph, stdout, &buf);

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
			જો (buf.len && buf.buf[buf.len - 1] == '\n')
				graph_show_padding(revs->graph);
			putchar(info->hdr_termination);
		} નહીંતર {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			જો (graph_show_remainder(revs->graph))
				putchar('\n');
			જો (revs->commit_format == CMIT_FMT_ONELINE)
				putchar('\n');
		}
		strbuf_release(&buf);
	} નહીંતર {
		જો (graph_show_remainder(revs->graph))
			putchar('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	finish_commit(commit);
}

સ્થિર પૂર્ણાંક finish_object(સંરચના object *obj, અચલ અક્ષર *name, શૂન્ય *cb_data)
{
	સંરચના rev_list_info *info = cb_data;
	જો (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, ખાલી, 0) < 0) {
		finish_object__ma(obj, name);
		પાછા 1;
	}
	જો (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	પાછા 0;
}

સ્થિર શૂન્ય show_object(સંરચના object *obj, અચલ અક્ષર *name, શૂન્ય *cb_data)
{
	સંરચના rev_list_info *info = cb_data;
	સંરચના rev_info *revs = info->revs;

	જો (finish_object(obj, name, cb_data))
		પાછા;
	display_progress(progress, ++progress_counter);
	જો (arg_missing_only)
		પાછા;
	જો (show_disk_usage)
		total_disk_usage += get_object_disk_usage(obj);
	જો (info->flags & REV_LIST_QUIET)
		પાછા;

	જો (revs->ગણતરી) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->ગણતરી_જમણું++;
		પાછા;
	}

	printf("%s", oid_to_hex(&obj->oid));

	જો (arg_show_object_names) {
		જો (લીટી_term) {
			putchar(info_term);
			માટે (અચલ અક્ષર *p = name; *p && *p != '\n'; p++)
				putchar(*p);
		} નહીંતર જો (*name) {
			printf("%cpath=%s", info_term, name);
		}
	}

	putchar(લીટી_term);
}

સ્થિર શૂન્ય show_edge(સંરચના commit *commit)
{
	printf("-%s\n", oid_to_hex(&commit->object.oid));
}

સ્થિર શૂન્ય print_var_str(અચલ અક્ષર *var, અચલ અક્ષર *val)
{
	printf("%s='%s'\n", var, val);
}

સ્થિર શૂન્ય print_var_int(અચલ અક્ષર *var, પૂર્ણાંક val)
{
	printf("%s=%d\n", var, val);
}

સ્થિર પૂર્ણાંક show_bisect_vars(સંરચના rev_list_info *info, પૂર્ણાંક reaches, પૂર્ણાંક all)
{
	પૂર્ણાંક cnt, flags = info->flags;
	અક્ષર hex[GIT_MAX_HEXSZ + 1] = "";
	સંરચના commit_list *tried;
	સંરચના rev_info *revs = info->revs;

	જો (!revs->commits)
		પાછા 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       ખાલી, ખાલી);

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
	જો (cnt < reaches)
		cnt = reaches;

	જો (revs->commits)
		oid_to_hex_r(hex, &revs->commits->item->object.oid);

	જો (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		printf("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_steps(all));

	પાછા 0;
}

સ્થિર પૂર્ણાંક show_object_fast(
	અચલ સંરચના object_id *oid,
	ગણના object_type type UNUSED,
	પૂર્ણાંક exclude UNUSED,
	uint32_t name_hash UNUSED,
	સંરચના packed_git *found_pack UNUSED,
	off_t found_offset UNUSED,
	શૂન્ય *payload UNUSED)
{
	fprintf(stdout, "%s\n", oid_to_hex(oid));
	પાછા 0;
}

સ્થિર શૂન્ય print_disk_usage(off_t size)
{
	સંરચના strbuf sb = STRBUF_INIT;
	જો (human_readable)
		strbuf_humanise_bytes(&sb, size);
	નહીંતર
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)size);
	puts(sb.buf);
	strbuf_release(&sb);
}

સ્થિર અંતઃરેખ પૂર્ણાંક parse_missing_action_કિંમત(સંરચના repository *repo,
					     અચલ અક્ષર *કિંમત)
{
	જો (!strcmp(કિંમત, "error")) {
		arg_missing_action = MA_ERROR;
		પાછા 1;
	}

	જો (!strcmp(કિંમત, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		પાછા 1;
	}

	જો (!strcmp(કિંમત, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		પાછા 1;
	}

	જો (!strcmp(કિંમત, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		પાછા 1;
	}

	જો (!strcmp(કિંમત, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		પાછા 1;
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક try_bitmap_ગણતરી(સંરચના rev_info *revs,
			    પૂર્ણાંક filter_provided_objects)
{
	uint32_t commit_ગણતરી = 0,
		 tag_ગણતરી = 0,
		 વૃક્ષ_ગણતરી = 0,
		 blob_ગણતરી = 0;
	પૂર્ણાંક max_ગણતરી;
	સંરચના bitmap_index *bitmap_git;

	/* This function only handles counting, not general traversal. */
	જો (!revs->ગણતરી)
		પાછા -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	જો (revs->ડાબું_જમણું || revs->cherry_mark)
		પાછા -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	જો (revs->max_ગણતરી >= 0 &&
	    (revs->tag_objects || revs->વૃક્ષ_objects || revs->blob_objects))
		પાછા -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_ગણતરી = revs->max_ગણતરી;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	જો (!bitmap_git)
		પાછા -1;

	ગણતરી_bitmap_commit_list(bitmap_git, &commit_ગણતરી,
				 revs->વૃક્ષ_objects ? &વૃક્ષ_ગણતરી : ખાલી,
				 revs->blob_objects ? &blob_ગણતરી : ખાલી,
				 revs->tag_objects ? &tag_ગણતરી : ખાલી);
	જો (max_ગણતરી >= 0 && max_ગણતરી < commit_ગણતરી)
		commit_ગણતરી = max_ગણતરી;

	printf("%d\n", commit_ગણતરી + વૃક્ષ_ગણતરી + blob_ગણતરી + tag_ગણતરી);
	free_bitmap_index(bitmap_git);
	પાછા 0;
}

સ્થિર પૂર્ણાંક try_bitmap_traversal(સંરચના rev_info *revs,
				પૂર્ણાંક filter_provided_objects)
{
	સંરચના bitmap_index *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	જો (revs->max_ગણતરી >= 0)
		પાછા -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	જો (revs->ડાબું_જમણું)
		પાછા -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	જો (!bitmap_git)
		પાછા -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_index(bitmap_git);
	પાછા 0;
}

સ્થિર પૂર્ણાંક try_bitmap_disk_usage(સંરચના rev_info *revs,
				 પૂર્ણાંક filter_provided_objects)
{
	સંરચના bitmap_index *bitmap_git;
	off_t size_from_bitmap;

	જો (!show_disk_usage)
		પાછા -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	જો (!bitmap_git)
		પાછા -1;

	size_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(size_from_bitmap);

	free_bitmap_index(bitmap_git);
	પાછા 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
સ્થિર શૂન્ય prepare_maximal_independent(સંરચના rev_info *revs)
{
	સંરચના commit_list *c;

	જો (!revs->maximal_only)
		પાછા;

	માટે (c = revs->commits; c; c = c->next) {
		જો (c->item->object.flags & UNINTERESTING)
			પાછા;
	}

	જો (revs->limited ||
	    revs->topo_order ||
	    revs->first_parent_only ||
	    revs->reverse ||
	    revs->max_ગણતરી >= 0 ||
	    revs->skip_ગણતરી >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_data.nr ||
	    revs->ગણતરી ||
	    revs->ડાબું_જમણું ||
	    revs->boundary ||
	    revs->tag_objects ||
	    revs->વૃક્ષ_objects ||
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
	    revs->લીટી_level_traverse)
		પાછા;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

પૂર્ણાંક cmd_rev_list(પૂર્ણાંક argc,
		 અચલ અક્ષર **argv,
		 અચલ અક્ષર *prefix,
		 સંરચના repository *repo)
{
	સંરચના rev_info revs;
	સંરચના rev_list_info info;
	સંરચના setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	પૂર્ણાંક i;
	પૂર્ણાંક bisect_list = 0;
	પૂર્ણાંક bisect_show_vars = 0;
	પૂર્ણાંક bisect_find_all = 0;
	પૂર્ણાંક use_bitmap_index = 0;
	પૂર્ણાંક filter_provided_objects = 0;
	અચલ અક્ષર *show_progress = ખાલી;
	પૂર્ણાંક ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, ખાલી);
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
	માટે (i = 1; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];
		જો (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} નહીંતર જો (skip_prefix(arg, "--missing=", &arg)) {
			parse_missing_action_કિંમત(repo, arg);
		} નહીંતર જો (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} નહીંતર જો (!strcmp(arg, "-z")) {
			લીટી_term = '\0';
			info_term = '\0';
		}
	}

	જો (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	જો (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = setup_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, કદ(info));
	info.revs = &revs;
	જો (revs.bisect)
		bisect_list = 1;

	જો (revs.diffopt.flags.quick)
		info.flags |= REV_LIST_QUIET;
	માટે (i = 1 ; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];

		જો (!strcmp(arg, "--header")) {
			revs.verbose_header = 1;
			ચાલુ;
		}
		જો (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			ચાલુ;
		}
		જો (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			ચાલુ;
		}
		જો (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_find_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			ચાલુ;
		}
		જો (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			ચાલુ;
		}
		જો (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_index = 1;
			ચાલુ;
		}
		જો (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			જાઓ cleanup;
		}
		જો (skip_prefix(arg, "--progress=", &arg)) {
			show_progress = arg;
			ચાલુ;
		}
		જો (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			ચાલુ;
		}
		જો (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			ચાલુ;
		}

		જો (!strcmp(arg, "--exclude-promisor-objects"))
			ચાલુ; /* already handled above */
		જો (skip_prefix(arg, "--missing=", &arg))
			ચાલુ; /* already handled above */

		જો (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_names = 0;
			ચાલુ;
		}

		જો (!strcmp(arg, ("--object-names"))) {
			arg_show_object_names = 1;
			ચાલુ;
		}

		જો (!strcmp(arg, ("--commit-header"))) {
			revs.include_header = 1;
			ચાલુ;
		}

		જો (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_header = 0;
			ચાલુ;
		}

		જો (skip_prefix(arg, "--disk-usage", &arg)) {
			જો (*arg == '=') {
				જો (!strcmp(++arg, "human")) {
					human_readable = 1;
				} નહીંતર
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} નહીંતર જો (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			ચાલુ;
		}

		જો (!strcmp(arg, "--missing-only"))
			ચાલુ;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	જો (!લીટી_term) {
		જો (revs.graph || revs.verbose_header || show_disk_usage ||
		    info.show_timestamp || info.header_prefix || bisect_list ||
		    use_bitmap_index || revs.edge_hint || revs.ડાબું_જમણું ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	જો (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_header = 1;
	જો (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		જો (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_header)
			info.header_prefix = "";
		નહીંતર
			info.header_prefix = "commit ";
	}
	નહીંતર જો (revs.verbose_header)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	જો ((!revs.commits && reflog_walk_empty(revs.reflog_info) &&
	     (!(revs.tag_objects || revs.વૃક્ષ_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_આગત_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	જો (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	જો (revs.ગણતરી &&
	    (revs.tag_objects || revs.વૃક્ષ_objects || revs.blob_objects) &&
	    (revs.ડાબું_જમણું || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.ગણતરી, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_buffer = (revs.verbose_header ||
			      revs.grep_filter.pattern_list ||
			      revs.grep_filter.header_list);
	જો (bisect_list)
		revs.limited = 1;

	જો (show_progress)
		progress = start_delayed_progress(the_repository,
						  show_progress, 0);

	જો (use_bitmap_index) {
		જો (!try_bitmap_ગણતરી(&revs, filter_provided_objects))
			જાઓ cleanup;
		જો (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			જાઓ cleanup;
		જો (!try_bitmap_traversal(&revs, filter_provided_objects))
			જાઓ cleanup;
	}

	જો (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	જો (revs.વૃક્ષ_objects)
		mark_edges_uninteresting(&revs, show_edge, 0);

	જો (bisect_list) {
		પૂર્ણાંક reaches, all;
		unsigned bisect_flags = 0;

		જો (bisect_find_all)
			bisect_flags |= FIND_BISECTION_ALL;

		જો (revs.first_parent_only)
			bisect_flags |= FIND_BISECTION_FIRST_PARENT_ONLY;

		find_bisection(&revs.commits, &reaches, &all, bisect_flags);

		જો (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			જાઓ cleanup;
		}
	}

	જો (filter_provided_objects) {
		સંરચના commit_list *c;
		માટે (i = 0; i < revs.pending.nr; i++) {
			સંરચના object_array_entry *pending = revs.pending.objects + i;
			pending->item->flags |= NOT_USER_GIVEN;
		}
		માટે (c = revs.commits; c; c = c->next)
			c->item->object.flags |= NOT_USER_GIVEN;
	}

	જો (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_SIZE);
	જો (should_collect_missing()) {
		સંરચના oidset_iter iter;
		સંરચના object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_SIZE);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		જ્યારે ((oid = oidset_iter_next(&iter)))
			add_missing_object_entry(oid, ખાલી, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : ખાલી));

	જો (arg_print_omitted) {
		સંરચના oidset_iter iter;
		સંરચના object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		જ્યારે ((oid = oidset_iter_next(&iter)))
			printf("~%s\n", oid_to_hex(oid));
		oidset_clear(&omitted_objects);
	}
	જો (should_collect_missing()) {
		સંરચના missing_objects_map_entry *entry;
		સંરચના oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		જ્યારે ((entry = oidmap_iter_next(&iter))) {
			print_missing_object(entry, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_entry_free);
	}

	stop_progress(&progress);

	જો (revs.ગણતરી) {
		જો (revs.ડાબું_જમણું && revs.cherry_mark)
			printf("%d\t%d\t%d\n", revs.ગણતરી_ડાબું, revs.ગણતરી_જમણું, revs.ગણતરી_same);
		નહીંતર જો (revs.ડાબું_જમણું)
			printf("%d\t%d\n", revs.ગણતરી_ડાબું, revs.ગણતરી_જમણું);
		નહીંતર જો (revs.cherry_mark)
			printf("%d\t%d\n", revs.ગણતરી_ડાબું + revs.ગણતરી_જમણું, revs.ગણતરી_same);
		નહીંતર
			printf("%d\n", revs.ગણતરી_ડાબું + revs.ગણતરી_જમણું);
	}

	જો (show_disk_usage)
		print_disk_usage(total_disk_usage);

cleanup:
	release_revisions(&revs);
	પાછા ret;
}
