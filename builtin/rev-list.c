#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "list-objects.h"
#अन्तर्भाव "list-objects-filter-options.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "pack-bitmap.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "log-tree.h"
#अन्तर्भाव "graph.h"
#अन्तर्भाव "bisect.h"
#अन्तर्भाव "progress.h"
#अन्तर्भाव "reflog-walk.h"
#अन्तर्भाव "oidset.h"
#अन्तर्भाव "oidmap.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "strbuf.h"

संरचना rev_list_info {
	संरचना rev_info *revs;
	पूर्णाङ्क flags;
	पूर्णाङ्क show_timestamp;
	पूर्णाङ्क hdr_termination;
	ध्रुव वर्ण *header_prefix;
};

स्थिर ध्रुव वर्ण rev_list_usage[] =
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

स्थिर संरचना progress *progress;
स्थिर अचिन्हित progress_counter;

स्थिर संरचना oidset omitted_objects;
स्थिर पूर्णाङ्क arg_print_omitted; /* print objects omitted by filter */

संरचना missing_objects_map_entry {
	संरचना oidmap_entry entry;
	वर्ण *path;
	अचिन्हित type;
};

स्थिर शून्य missing_objects_map_entry_free(शून्य *e)
{
	संरचना missing_objects_map_entry *entry =
		container_of(e, संरचना missing_objects_map_entry, entry);

	मुक्त(entry->path);
	मुक्त(entry);
}

स्थिर संरचना oidmap missing_objects;
गणना missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
स्थिर गणना missing_action arg_missing_action;
स्थिर पूर्णाङ्क arg_missing_only;

स्थिर अन्तःस्थ पूर्णाङ्क should_collect_missing(शून्य)
{
	प्रत्यागम arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
स्थिर पूर्णाङ्क arg_show_object_names = 1;

#define DEFAULT_OIDSET_SIZE     (16*1024)

स्थिर वर्ण पंक्ति_term = '\n';
स्थिर वर्ण info_term = ' ';

स्थिर पूर्णाङ्क show_disk_usage;
स्थिर off_t total_disk_usage;
स्थिर पूर्णाङ्क human_readable;

स्थिर off_t get_object_disk_usage(संरचना object *obj)
{
	off_t size;
	संरचना object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &size;
	यदि (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_to_hex(&obj->oid));
	प्रत्यागम size;
}

स्थिर शून्य add_missing_object_entry(संरचना object_id *oid, ध्रुव वर्ण *path,
				     अचिन्हित type)
{
	संरचना missing_objects_map_entry *entry;

	यदि (oidmap_get(&missing_objects, oid))
		प्रत्यागम;

	CALLOC_ARRAY(entry, 1);
	entry->entry.oid = *oid;
	entry->type = type;
	यदि (path)
		entry->path = xstrdup(path);
	oidmap_put(&missing_objects, entry);
}

स्थिर शून्य print_missing_object(संरचना missing_objects_map_entry *entry,
				 पूर्णाङ्क print_missing_info)
{
	संरचना strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	यदि (arg_missing_only && पंक्ति_term)
		म_लिखो("%s", oid_to_hex(&entry->entry.oid));
	अन्यथा यदि (पंक्ति_term)
		म_लिखो("?%s", oid_to_hex(&entry->entry.oid));
	अन्यथा
		म_लिखो("%s%cmissing=yes", oid_to_hex(&entry->entry.oid),
		       info_term);

	यदि (!print_missing_info) {
		अक्षर_दो(पंक्ति_term);
		प्रत्यागम;
	}

	यदि (entry->path && *entry->path) {
		strbuf_addf(&sb, "%cpath=", info_term);

		यदि (पंक्ति_term) {
			संरचना strbuf path = STRBUF_INIT;

			quote_path(entry->path, रिक्त, &path, QUOTE_PATH_QUOTE_SP);
			strbuf_addbuf(&sb, &path);

			strbuf_release(&path);
		} अन्यथा {
			strbuf_addstr(&sb, entry->path);
		}
	}
	यदि (entry->type)
		strbuf_addf(&sb, "%ctype=%s", info_term, type_name(entry->type));

	ख_डालो(sb.buf, परिमाण(वर्ण), sb.len, मानक_निकास);
	अक्षर_दो(पंक्ति_term);

	strbuf_release(&sb);
}

स्थिर अन्तःस्थ शून्य finish_object__ma(संरचना object *obj, ध्रुव वर्ण *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	विकल्प (arg_missing_action) {
	पक्ष MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		प्रत्यागम;

	पक्ष MA_ALLOW_ANY:
		प्रत्यागम;

	पक्ष MA_PRINT:
	पक्ष MA_PRINT_INFO:
		add_missing_object_entry(&obj->oid, name, obj->type);
		प्रत्यागम;

	पक्ष MA_ALLOW_PROMISOR:
		यदि (is_promisor_object(the_repository, &obj->oid))
			प्रत्यागम;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		प्रत्यागम;

	स्वभाव:
		BUG("unhandled missing_action");
		प्रत्यागम;
	}
}

स्थिर शून्य finish_commit(संरचना commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = रिक्त;
	free_commit_buffer(the_repository->parsed_objects,
			   commit);
}

स्थिर शून्य show_commit(संरचना commit *commit, शून्य *data)
{
	संरचना rev_list_info *info = data;
	संरचना rev_info *revs = info->revs;

	display_progress(progress, ++progress_counter);

	यदि (revs->do_not_die_on_missing_objects &&
	    oidset_contains(&revs->missing_commits, &commit->object.oid)) {
		finish_object__ma(&commit->object, रिक्त);
		प्रत्यागम;
	}

	यदि (arg_missing_only) {
		finish_commit(commit);
		प्रत्यागम;
	}

	यदि (show_disk_usage)
		total_disk_usage += get_object_disk_usage(&commit->object);

	यदि (info->flags & REV_LIST_QUIET) {
		finish_commit(commit);
		प्रत्यागम;
	}

	graph_show_commit(revs->graph);

	यदि (revs->गिनती) {
		यदि (commit->object.flags & PATCHSAME)
			revs->गिनती_same++;
		अन्यथा यदि (commit->object.flags & SYMMETRIC_बायाँ)
			revs->गिनती_बायाँ++;
		अन्यथा
			revs->गिनती_दायाँ++;
		finish_commit(commit);
		प्रत्यागम;
	}

	यदि (info->show_timestamp)
		म_लिखो("%"PRItime" ", commit->date);
	यदि (info->header_prefix)
		ख_माला_दो(info->header_prefix, मानक_निकास);

	यदि (revs->include_header) {
		यदि (!revs->graph && पंक्ति_term)
			ख_माला_दो(get_revision_mark(revs, commit), मानक_निकास);
		यदि (revs->abbrev_commit && revs->abbrev)
			ख_माला_दो(repo_find_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      मानक_निकास);
		अन्यथा
			ख_माला_दो(oid_to_hex(&commit->object.oid), मानक_निकास);

		यदि (!पंक्ति_term) {
			यदि (commit->object.flags & BOUNDARY)
				म_लिखो("%cboundary=yes", info_term);
		}
	}
	यदि (revs->print_parents) {
		संरचना commit_list *parents = commit->parents;
		यावत् (parents) {
			म_लिखो(" %s", oid_to_hex(&parents->item->object.oid));
			parents = parents->next;
		}
	}
	यदि (revs->children.name) {
		संरचना commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		यावत् (children) {
			म_लिखो(" %s", oid_to_hex(&children->item->object.oid));
			children = children->next;
		}
	}
	show_decorations(revs, commit);
	यदि (revs->commit_format == CMIT_FMT_ONELINE)
		अक्षर_दो(' ');
	अन्यथा यदि (revs->include_header)
		अक्षर_दो(पंक्ति_term);

	यदि (revs->verbose_header) {
		संरचना strbuf buf = STRBUF_INIT;
		संरचना pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_mode = revs->date_mode;
		ctx.date_mode_explicit = revs->date_mode_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		यदि (buf.len) {
			यदि (revs->commit_format != CMIT_FMT_ONELINE)
				graph_show_oneline(revs->graph);

			graph_show_commit_संदेश(revs->graph, मानक_निकास, &buf);

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
			यदि (buf.len && buf.buf[buf.len - 1] == '\n')
				graph_show_padding(revs->graph);
			अक्षर_दो(info->hdr_termination);
		} अन्यथा {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			यदि (graph_show_remainder(revs->graph))
				अक्षर_दो('\n');
			यदि (revs->commit_format == CMIT_FMT_ONELINE)
				अक्षर_दो('\n');
		}
		strbuf_release(&buf);
	} अन्यथा {
		यदि (graph_show_remainder(revs->graph))
			अक्षर_दो('\n');
	}
	maybe_flush_or_die(मानक_निकास, "stdout");
	finish_commit(commit);
}

स्थिर पूर्णाङ्क finish_object(संरचना object *obj, ध्रुव वर्ण *name, शून्य *cb_data)
{
	संरचना rev_list_info *info = cb_data;
	यदि (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, रिक्त, 0) < 0) {
		finish_object__ma(obj, name);
		प्रत्यागम 1;
	}
	यदि (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	प्रत्यागम 0;
}

स्थिर शून्य show_object(संरचना object *obj, ध्रुव वर्ण *name, शून्य *cb_data)
{
	संरचना rev_list_info *info = cb_data;
	संरचना rev_info *revs = info->revs;

	यदि (finish_object(obj, name, cb_data))
		प्रत्यागम;
	display_progress(progress, ++progress_counter);
	यदि (arg_missing_only)
		प्रत्यागम;
	यदि (show_disk_usage)
		total_disk_usage += get_object_disk_usage(obj);
	यदि (info->flags & REV_LIST_QUIET)
		प्रत्यागम;

	यदि (revs->गिनती) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->गिनती_दायाँ++;
		प्रत्यागम;
	}

	म_लिखो("%s", oid_to_hex(&obj->oid));

	यदि (arg_show_object_names) {
		यदि (पंक्ति_term) {
			अक्षर_दो(info_term);
			प्रति (ध्रुव वर्ण *p = name; *p && *p != '\n'; p++)
				अक्षर_दो(*p);
		} अन्यथा यदि (*name) {
			म_लिखो("%cpath=%s", info_term, name);
		}
	}

	अक्षर_दो(पंक्ति_term);
}

स्थिर शून्य show_edge(संरचना commit *commit)
{
	म_लिखो("-%s\n", oid_to_hex(&commit->object.oid));
}

स्थिर शून्य print_var_str(ध्रुव वर्ण *var, ध्रुव वर्ण *val)
{
	म_लिखो("%s='%s'\n", var, val);
}

स्थिर शून्य print_var_int(ध्रुव वर्ण *var, पूर्णाङ्क val)
{
	म_लिखो("%s=%d\n", var, val);
}

स्थिर पूर्णाङ्क show_bisect_vars(संरचना rev_list_info *info, पूर्णाङ्क reaches, पूर्णाङ्क all)
{
	पूर्णाङ्क cnt, flags = info->flags;
	वर्ण hex[GIT_MAX_HEXSZ + 1] = "";
	संरचना commit_list *tried;
	संरचना rev_info *revs = info->revs;

	यदि (!revs->commits)
		प्रत्यागम 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       रिक्त, रिक्त);

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
	यदि (cnt < reaches)
		cnt = reaches;

	यदि (revs->commits)
		oid_to_hex_r(hex, &revs->commits->item->object.oid);

	यदि (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		म_लिखो("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_steps(all));

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_object_fast(
	ध्रुव संरचना object_id *oid,
	गणना object_type type UNUSED,
	पूर्णाङ्क exclude UNUSED,
	uint32_t name_hash UNUSED,
	संरचना packed_git *found_pack UNUSED,
	off_t found_offset UNUSED,
	शून्य *payload UNUSED)
{
	ख_लिखो(मानक_निकास, "%s\n", oid_to_hex(oid));
	प्रत्यागम 0;
}

स्थिर शून्य print_disk_usage(off_t size)
{
	संरचना strbuf sb = STRBUF_INIT;
	यदि (human_readable)
		strbuf_humanise_bytes(&sb, size);
	अन्यथा
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)size);
	माला_दो(sb.buf);
	strbuf_release(&sb);
}

स्थिर अन्तःस्थ पूर्णाङ्क parse_missing_action_मूल्य(संरचना repository *repo,
					     ध्रुव वर्ण *मूल्य)
{
	यदि (!म_भेद(मूल्य, "error")) {
		arg_missing_action = MA_ERROR;
		प्रत्यागम 1;
	}

	यदि (!म_भेद(मूल्य, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		प्रत्यागम 1;
	}

	यदि (!म_भेद(मूल्य, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		प्रत्यागम 1;
	}

	यदि (!म_भेद(मूल्य, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		प्रत्यागम 1;
	}

	यदि (!म_भेद(मूल्य, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		प्रत्यागम 1;
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क try_bitmap_गिनती(संरचना rev_info *revs,
			    पूर्णाङ्क filter_provided_objects)
{
	uint32_t commit_गिनती = 0,
		 tag_गिनती = 0,
		 वृक्ष_गिनती = 0,
		 blob_गिनती = 0;
	पूर्णाङ्क max_गिनती;
	संरचना bitmap_index *bitmap_git;

	/* This function only handles counting, not general traversal. */
	यदि (!revs->गिनती)
		प्रत्यागम -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	यदि (revs->बायाँ_दायाँ || revs->cherry_mark)
		प्रत्यागम -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	यदि (revs->max_गिनती >= 0 &&
	    (revs->tag_objects || revs->वृक्ष_objects || revs->blob_objects))
		प्रत्यागम -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_गिनती = revs->max_गिनती;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	यदि (!bitmap_git)
		प्रत्यागम -1;

	गिनती_bitmap_commit_list(bitmap_git, &commit_गिनती,
				 revs->वृक्ष_objects ? &वृक्ष_गिनती : रिक्त,
				 revs->blob_objects ? &blob_गिनती : रिक्त,
				 revs->tag_objects ? &tag_गिनती : रिक्त);
	यदि (max_गिनती >= 0 && max_गिनती < commit_गिनती)
		commit_गिनती = max_गिनती;

	म_लिखो("%d\n", commit_गिनती + वृक्ष_गिनती + blob_गिनती + tag_गिनती);
	free_bitmap_index(bitmap_git);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क try_bitmap_traversal(संरचना rev_info *revs,
				पूर्णाङ्क filter_provided_objects)
{
	संरचना bitmap_index *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	यदि (revs->max_गिनती >= 0)
		प्रत्यागम -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	यदि (revs->बायाँ_दायाँ)
		प्रत्यागम -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	यदि (!bitmap_git)
		प्रत्यागम -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_index(bitmap_git);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क try_bitmap_disk_usage(संरचना rev_info *revs,
				 पूर्णाङ्क filter_provided_objects)
{
	संरचना bitmap_index *bitmap_git;
	off_t size_from_bitmap;

	यदि (!show_disk_usage)
		प्रत्यागम -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	यदि (!bitmap_git)
		प्रत्यागम -1;

	size_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(size_from_bitmap);

	free_bitmap_index(bitmap_git);
	प्रत्यागम 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
स्थिर शून्य prepare_maximal_independent(संरचना rev_info *revs)
{
	संरचना commit_list *c;

	यदि (!revs->maximal_only)
		प्रत्यागम;

	प्रति (c = revs->commits; c; c = c->next) {
		यदि (c->item->object.flags & UNINTERESTING)
			प्रत्यागम;
	}

	यदि (revs->limited ||
	    revs->topo_order ||
	    revs->first_parent_only ||
	    revs->reverse ||
	    revs->max_गिनती >= 0 ||
	    revs->skip_गिनती >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_data.nr ||
	    revs->गिनती ||
	    revs->बायाँ_दायाँ ||
	    revs->boundary ||
	    revs->tag_objects ||
	    revs->वृक्ष_objects ||
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
	    revs->पंक्ति_level_traverse)
		प्रत्यागम;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

पूर्णाङ्क cmd_rev_list(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *prefix,
		 संरचना repository *repo)
{
	संरचना rev_info revs;
	संरचना rev_list_info info;
	संरचना setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	पूर्णाङ्क i;
	पूर्णाङ्क bisect_list = 0;
	पूर्णाङ्क bisect_show_vars = 0;
	पूर्णाङ्क bisect_find_all = 0;
	पूर्णाङ्क use_bitmap_index = 0;
	पूर्णाङ्क filter_provided_objects = 0;
	ध्रुव वर्ण *show_progress = रिक्त;
	पूर्णाङ्क ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, रिक्त);
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
	प्रति (i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		यदि (!म_भेद(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} अन्यथा यदि (skip_prefix(arg, "--missing=", &arg)) {
			parse_missing_action_मूल्य(repo, arg);
		} अन्यथा यदि (!म_भेद(arg, "--missing-only")) {
			arg_missing_only = 1;
		} अन्यथा यदि (!म_भेद(arg, "-z")) {
			पंक्ति_term = '\0';
			info_term = '\0';
		}
	}

	यदि (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	यदि (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = setup_revisions(argc, argv, &revs, &s_r_opt);

	स_रखो(&info, 0, परिमाण(info));
	info.revs = &revs;
	यदि (revs.bisect)
		bisect_list = 1;

	यदि (revs.diffopt.flags.quick)
		info.flags |= REV_LIST_QUIET;
	प्रति (i = 1 ; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];

		यदि (!म_भेद(arg, "--header")) {
			revs.verbose_header = 1;
			अनुवर्त;
		}
		यदि (!म_भेद(arg, "--timestamp")) {
			info.show_timestamp = 1;
			अनुवर्त;
		}
		यदि (!म_भेद(arg, "--bisect")) {
			bisect_list = 1;
			अनुवर्त;
		}
		यदि (!म_भेद(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_find_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			अनुवर्त;
		}
		यदि (!म_भेद(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			अनुवर्त;
		}
		यदि (!म_भेद(arg, "--use-bitmap-index")) {
			use_bitmap_index = 1;
			अनुवर्त;
		}
		यदि (!म_भेद(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			गच्छ cleanup;
		}
		यदि (skip_prefix(arg, "--progress=", &arg)) {
			show_progress = arg;
			अनुवर्त;
		}
		यदि (!म_भेद(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			अनुवर्त;
		}
		यदि (!म_भेद(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			अनुवर्त;
		}

		यदि (!म_भेद(arg, "--exclude-promisor-objects"))
			अनुवर्त; /* already handled above */
		यदि (skip_prefix(arg, "--missing=", &arg))
			अनुवर्त; /* already handled above */

		यदि (!म_भेद(arg, ("--no-object-names"))) {
			arg_show_object_names = 0;
			अनुवर्त;
		}

		यदि (!म_भेद(arg, ("--object-names"))) {
			arg_show_object_names = 1;
			अनुवर्त;
		}

		यदि (!म_भेद(arg, ("--commit-header"))) {
			revs.include_header = 1;
			अनुवर्त;
		}

		यदि (!म_भेद(arg, ("--no-commit-header"))) {
			revs.include_header = 0;
			अनुवर्त;
		}

		यदि (skip_prefix(arg, "--disk-usage", &arg)) {
			यदि (*arg == '=') {
				यदि (!म_भेद(++arg, "human")) {
					human_readable = 1;
				} अन्यथा
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} अन्यथा यदि (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			अनुवर्त;
		}

		यदि (!म_भेद(arg, "--missing-only"))
			अनुवर्त;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	यदि (!पंक्ति_term) {
		यदि (revs.graph || revs.verbose_header || show_disk_usage ||
		    info.show_timestamp || info.header_prefix || bisect_list ||
		    use_bitmap_index || revs.edge_hint || revs.बायाँ_दायाँ ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	यदि (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_header = 1;
	यदि (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		यदि (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_header)
			info.header_prefix = "";
		अन्यथा
			info.header_prefix = "commit ";
	}
	अन्यथा यदि (revs.verbose_header)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	यदि ((!revs.commits && reflog_walk_empty(revs.reflog_info) &&
	     (!(revs.tag_objects || revs.वृक्ष_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_निवेश_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	यदि (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	यदि (revs.गिनती &&
	    (revs.tag_objects || revs.वृक्ष_objects || revs.blob_objects) &&
	    (revs.बायाँ_दायाँ || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.गिनती, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_buffer = (revs.verbose_header ||
			      revs.grep_filter.pattern_list ||
			      revs.grep_filter.header_list);
	यदि (bisect_list)
		revs.limited = 1;

	यदि (show_progress)
		progress = start_delayed_progress(the_repository,
						  show_progress, 0);

	यदि (use_bitmap_index) {
		यदि (!try_bitmap_गिनती(&revs, filter_provided_objects))
			गच्छ cleanup;
		यदि (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			गच्छ cleanup;
		यदि (!try_bitmap_traversal(&revs, filter_provided_objects))
			गच्छ cleanup;
	}

	यदि (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	यदि (revs.वृक्ष_objects)
		mark_edges_uninteresting(&revs, show_edge, 0);

	यदि (bisect_list) {
		पूर्णाङ्क reaches, all;
		अचिन्हित bisect_flags = 0;

		यदि (bisect_find_all)
			bisect_flags |= FIND_BISECTION_ALL;

		यदि (revs.first_parent_only)
			bisect_flags |= FIND_BISECTION_FIRST_PARENT_ONLY;

		find_bisection(&revs.commits, &reaches, &all, bisect_flags);

		यदि (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			गच्छ cleanup;
		}
	}

	यदि (filter_provided_objects) {
		संरचना commit_list *c;
		प्रति (i = 0; i < revs.pending.nr; i++) {
			संरचना object_array_entry *pending = revs.pending.objects + i;
			pending->item->flags |= NOT_USER_GIVEN;
		}
		प्रति (c = revs.commits; c; c = c->next)
			c->item->object.flags |= NOT_USER_GIVEN;
	}

	यदि (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_SIZE);
	यदि (should_collect_missing()) {
		संरचना oidset_iter iter;
		संरचना object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_SIZE);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		यावत् ((oid = oidset_iter_next(&iter)))
			add_missing_object_entry(oid, रिक्त, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : रिक्त));

	यदि (arg_print_omitted) {
		संरचना oidset_iter iter;
		संरचना object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		यावत् ((oid = oidset_iter_next(&iter)))
			म_लिखो("~%s\n", oid_to_hex(oid));
		oidset_clear(&omitted_objects);
	}
	यदि (should_collect_missing()) {
		संरचना missing_objects_map_entry *entry;
		संरचना oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		यावत् ((entry = oidmap_iter_next(&iter))) {
			print_missing_object(entry, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_entry_free);
	}

	stop_progress(&progress);

	यदि (revs.गिनती) {
		यदि (revs.बायाँ_दायाँ && revs.cherry_mark)
			म_लिखो("%d\t%d\t%d\n", revs.गिनती_बायाँ, revs.गिनती_दायाँ, revs.गिनती_same);
		अन्यथा यदि (revs.बायाँ_दायाँ)
			म_लिखो("%d\t%d\n", revs.गिनती_बायाँ, revs.गिनती_दायाँ);
		अन्यथा यदि (revs.cherry_mark)
			म_लिखो("%d\t%d\n", revs.गिनती_बायाँ + revs.गिनती_दायाँ, revs.गिनती_same);
		अन्यथा
			म_लिखो("%d\n", revs.गिनती_बायाँ + revs.गिनती_दायाँ);
	}

	यदि (show_disk_usage)
		print_disk_usage(total_disk_usage);

cleanup:
	release_revisions(&revs);
	प्रत्यागम ret;
}
