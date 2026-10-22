#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "commit.h"
#समावेश "diff.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "revision.h"
#समावेश "list-objects.h"
#समावेश "list-objects-filter-options.h"
#समावेश "object.h"
#समावेश "object-name.h"
#समावेश "object-file.h"
#समावेश "odb.h"
#समावेश "pack-bitmap.h"
#समावेश "parse-options.h"
#समावेश "log-tree.h"
#समावेश "graph.h"
#समावेश "bisect.h"
#समावेश "progress.h"
#समावेश "reflog-walk.h"
#समावेश "oidset.h"
#समावेश "oidmap.h"
#समावेश "packfile.h"
#समावेश "commit-reach.h"
#समावेश "quote.h"
#समावेश "strbuf.h"

काष्ठा rev_list_info {
	काष्ठा rev_info *revs;
	पूर्णांक flags;
	पूर्णांक show_timestamp;
	पूर्णांक hdr_termination;
	स्थिर अक्षर *header_prefix;
};

अटल स्थिर अक्षर rev_list_usage[] =
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

अटल काष्ठा progress *progress;
अटल अचिन्हित progress_गणक;

अटल काष्ठा oidset omitted_objects;
अटल पूर्णांक arg_print_omitted; /* print objects omitted by filter */

काष्ठा missing_objects_map_entry {
	काष्ठा oidmap_entry entry;
	अक्षर *path;
	अचिन्हित type;
};

अटल व्योम missing_objects_map_entry_free(व्योम *e)
{
	काष्ठा missing_objects_map_entry *entry =
		container_of(e, काष्ठा missing_objects_map_entry, entry);

	मुक्त(entry->path);
	मुक्त(entry);
}

अटल काष्ठा oidmap missing_objects;
क्रमागत missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
अटल क्रमागत missing_action arg_missing_action;
अटल पूर्णांक arg_missing_only;

अटल अंतरभूत पूर्णांक should_collect_missing(व्योम)
{
	वापस arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
अटल पूर्णांक arg_show_object_names = 1;

#घोषणा DEFAULT_OIDSET_आकार     (16*1024)

अटल अक्षर पंक्ति_term = '\n';
अटल अक्षर info_term = ' ';

अटल पूर्णांक show_disk_usage;
अटल off_t योग_disk_usage;
अटल पूर्णांक human_readable;

अटल off_t get_object_disk_usage(काष्ठा object *obj)
{
	off_t आकार;
	काष्ठा object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &आकार;
	अगर (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_to_hex(&obj->oid));
	वापस आकार;
}

अटल व्योम add_missing_object_entry(काष्ठा object_id *oid, स्थिर अक्षर *path,
				     अचिन्हित type)
{
	काष्ठा missing_objects_map_entry *entry;

	अगर (oidmap_get(&missing_objects, oid))
		वापस;

	CALLOC_ARRAY(entry, 1);
	entry->entry.oid = *oid;
	entry->type = type;
	अगर (path)
		entry->path = xstrdup(path);
	oidmap_put(&missing_objects, entry);
}

अटल व्योम print_missing_object(काष्ठा missing_objects_map_entry *entry,
				 पूर्णांक print_missing_info)
{
	काष्ठा strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	अगर (arg_missing_only && पंक्ति_term)
		म_लिखो("%s", oid_to_hex(&entry->entry.oid));
	अन्यथा अगर (पंक्ति_term)
		म_लिखो("?%s", oid_to_hex(&entry->entry.oid));
	अन्यथा
		म_लिखो("%s%cmissing=yes", oid_to_hex(&entry->entry.oid),
		       info_term);

	अगर (!print_missing_info) {
		अक्षर_दो(पंक्ति_term);
		वापस;
	}

	अगर (entry->path && *entry->path) {
		strbuf_addf(&sb, "%cpath=", info_term);

		अगर (पंक्ति_term) {
			काष्ठा strbuf path = STRBUF_INIT;

			quote_path(entry->path, शून्य, &path, QUOTE_PATH_QUOTE_SP);
			strbuf_addbuf(&sb, &path);

			strbuf_release(&path);
		} अन्यथा {
			strbuf_addstr(&sb, entry->path);
		}
	}
	अगर (entry->type)
		strbuf_addf(&sb, "%ctype=%s", info_term, type_name(entry->type));

	ख_डालो(sb.buf, माप(अक्षर), sb.len, मानक_निकास);
	अक्षर_दो(पंक्ति_term);

	strbuf_release(&sb);
}

अटल अंतरभूत व्योम finish_object__ma(काष्ठा object *obj, स्थिर अक्षर *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	चयन (arg_missing_action) {
	हाल MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		वापस;

	हाल MA_ALLOW_ANY:
		वापस;

	हाल MA_PRINT:
	हाल MA_PRINT_INFO:
		add_missing_object_entry(&obj->oid, name, obj->type);
		वापस;

	हाल MA_ALLOW_PROMISOR:
		अगर (is_promisor_object(the_repository, &obj->oid))
			वापस;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_to_hex(&obj->oid));
		वापस;

	शेष:
		BUG("unhandled missing_action");
		वापस;
	}
}

अटल व्योम finish_commit(काष्ठा commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = शून्य;
	free_commit_बफर(the_repository->parsed_objects,
			   commit);
}

अटल व्योम show_commit(काष्ठा commit *commit, व्योम *तथ्य)
{
	काष्ठा rev_list_info *info = तथ्य;
	काष्ठा rev_info *revs = info->revs;

	display_progress(progress, ++progress_गणक);

	अगर (revs->do_not_die_on_missing_objects &&
	    oidset_contains(&revs->missing_commits, &commit->object.oid)) {
		finish_object__ma(&commit->object, शून्य);
		वापस;
	}

	अगर (arg_missing_only) {
		finish_commit(commit);
		वापस;
	}

	अगर (show_disk_usage)
		योग_disk_usage += get_object_disk_usage(&commit->object);

	अगर (info->flags & REV_LIST_QUIET) {
		finish_commit(commit);
		वापस;
	}

	graph_show_commit(revs->graph);

	अगर (revs->गिनती) {
		अगर (commit->object.flags & PATCHSAME)
			revs->गिनती_same++;
		अन्यथा अगर (commit->object.flags & SYMMETRIC_बायाँ)
			revs->गिनती_बायाँ++;
		अन्यथा
			revs->गिनती_दायाँ++;
		finish_commit(commit);
		वापस;
	}

	अगर (info->show_timestamp)
		म_लिखो("%"PRItime" ", commit->date);
	अगर (info->header_prefix)
		ख_माला_दो(info->header_prefix, मानक_निकास);

	अगर (revs->include_header) {
		अगर (!revs->graph && पंक्ति_term)
			ख_माला_दो(get_revision_mark(revs, commit), मानक_निकास);
		अगर (revs->abbrev_commit && revs->abbrev)
			ख_माला_दो(repo_find_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      मानक_निकास);
		अन्यथा
			ख_माला_दो(oid_to_hex(&commit->object.oid), मानक_निकास);

		अगर (!पंक्ति_term) {
			अगर (commit->object.flags & BOUNDARY)
				म_लिखो("%cboundary=yes", info_term);
		}
	}
	अगर (revs->print_parents) {
		काष्ठा commit_list *parents = commit->parents;
		जबतक (parents) {
			म_लिखो(" %s", oid_to_hex(&parents->item->object.oid));
			parents = parents->अग्र;
		}
	}
	अगर (revs->children.name) {
		काष्ठा commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		जबतक (children) {
			म_लिखो(" %s", oid_to_hex(&children->item->object.oid));
			children = children->अग्र;
		}
	}
	show_decorations(revs, commit);
	अगर (revs->commit_format == CMIT_FMT_ONELINE)
		अक्षर_दो(' ');
	अन्यथा अगर (revs->include_header)
		अक्षर_दो(पंक्ति_term);

	अगर (revs->verbose_header) {
		काष्ठा strbuf buf = STRBUF_INIT;
		काष्ठा pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_mode = revs->date_mode;
		ctx.date_mode_explicit = revs->date_mode_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		अगर (buf.len) {
			अगर (revs->commit_format != CMIT_FMT_ONELINE)
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
			अगर (buf.len && buf.buf[buf.len - 1] == '\n')
				graph_show_padding(revs->graph);
			अक्षर_दो(info->hdr_termination);
		} अन्यथा {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			अगर (graph_show_remainder(revs->graph))
				अक्षर_दो('\n');
			अगर (revs->commit_format == CMIT_FMT_ONELINE)
				अक्षर_दो('\n');
		}
		strbuf_release(&buf);
	} अन्यथा {
		अगर (graph_show_remainder(revs->graph))
			अक्षर_दो('\n');
	}
	maybe_flush_or_die(मानक_निकास, "stdout");
	finish_commit(commit);
}

अटल पूर्णांक finish_object(काष्ठा object *obj, स्थिर अक्षर *name, व्योम *cb_तथ्य)
{
	काष्ठा rev_list_info *info = cb_तथ्य;
	अगर (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, शून्य, 0) < 0) {
		finish_object__ma(obj, name);
		वापस 1;
	}
	अगर (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	वापस 0;
}

अटल व्योम show_object(काष्ठा object *obj, स्थिर अक्षर *name, व्योम *cb_तथ्य)
{
	काष्ठा rev_list_info *info = cb_तथ्य;
	काष्ठा rev_info *revs = info->revs;

	अगर (finish_object(obj, name, cb_तथ्य))
		वापस;
	display_progress(progress, ++progress_गणक);
	अगर (arg_missing_only)
		वापस;
	अगर (show_disk_usage)
		योग_disk_usage += get_object_disk_usage(obj);
	अगर (info->flags & REV_LIST_QUIET)
		वापस;

	अगर (revs->गिनती) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->गिनती_दायाँ++;
		वापस;
	}

	म_लिखो("%s", oid_to_hex(&obj->oid));

	अगर (arg_show_object_names) {
		अगर (पंक्ति_term) {
			अक्षर_दो(info_term);
			क्रम (स्थिर अक्षर *p = name; *p && *p != '\n'; p++)
				अक्षर_दो(*p);
		} अन्यथा अगर (*name) {
			म_लिखो("%cpath=%s", info_term, name);
		}
	}

	अक्षर_दो(पंक्ति_term);
}

अटल व्योम show_edge(काष्ठा commit *commit)
{
	म_लिखो("-%s\n", oid_to_hex(&commit->object.oid));
}

अटल व्योम print_var_str(स्थिर अक्षर *var, स्थिर अक्षर *val)
{
	म_लिखो("%s='%s'\n", var, val);
}

अटल व्योम print_var_int(स्थिर अक्षर *var, पूर्णांक val)
{
	म_लिखो("%s=%d\n", var, val);
}

अटल पूर्णांक show_bisect_vars(काष्ठा rev_list_info *info, पूर्णांक reaches, पूर्णांक all)
{
	पूर्णांक cnt, flags = info->flags;
	अक्षर hex[GIT_MAX_HEXSZ + 1] = "";
	काष्ठा commit_list *tried;
	काष्ठा rev_info *revs = info->revs;

	अगर (!revs->commits)
		वापस 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       शून्य, शून्य);

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
	अगर (cnt < reaches)
		cnt = reaches;

	अगर (revs->commits)
		oid_to_hex_r(hex, &revs->commits->item->object.oid);

	अगर (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		म_लिखो("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_steps(all));

	वापस 0;
}

अटल पूर्णांक show_object_fast(
	स्थिर काष्ठा object_id *oid,
	क्रमागत object_type type UNUSED,
	पूर्णांक exclude UNUSED,
	uint32_t name_hash UNUSED,
	काष्ठा packed_git *found_pack UNUSED,
	off_t found_offset UNUSED,
	व्योम *payload UNUSED)
{
	ख_लिखो(मानक_निकास, "%s\n", oid_to_hex(oid));
	वापस 0;
}

अटल व्योम print_disk_usage(off_t आकार)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	अगर (human_readable)
		strbuf_humanise_bytes(&sb, आकार);
	अन्यथा
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)आकार);
	माला_दो(sb.buf);
	strbuf_release(&sb);
}

अटल अंतरभूत पूर्णांक parse_missing_action_मूल्य(काष्ठा repository *repo,
					     स्थिर अक्षर *मूल्य)
{
	अगर (!म_भेद(मूल्य, "error")) {
		arg_missing_action = MA_ERROR;
		वापस 1;
	}

	अगर (!म_भेद(मूल्य, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		वापस 1;
	}

	अगर (!म_भेद(मूल्य, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		वापस 1;
	}

	अगर (!म_भेद(मूल्य, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		वापस 1;
	}

	अगर (!म_भेद(मूल्य, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		वापस 1;
	}

	वापस 0;
}

अटल पूर्णांक try_bitmap_गिनती(काष्ठा rev_info *revs,
			    पूर्णांक filter_provided_objects)
{
	uint32_t commit_गिनती = 0,
		 tag_गिनती = 0,
		 वृक्ष_गिनती = 0,
		 blob_गिनती = 0;
	पूर्णांक max_गिनती;
	काष्ठा bitmap_सूचकांक *bitmap_git;

	/* This function only handles counting, not general traversal. */
	अगर (!revs->गिनती)
		वापस -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	अगर (revs->बायाँ_दायाँ || revs->cherry_mark)
		वापस -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	अगर (revs->max_गिनती >= 0 &&
	    (revs->tag_objects || revs->वृक्ष_objects || revs->blob_objects))
		वापस -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_गिनती = revs->max_गिनती;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	अगर (!bitmap_git)
		वापस -1;

	गिनती_bitmap_commit_list(bitmap_git, &commit_गिनती,
				 revs->वृक्ष_objects ? &वृक्ष_गिनती : शून्य,
				 revs->blob_objects ? &blob_गिनती : शून्य,
				 revs->tag_objects ? &tag_गिनती : शून्य);
	अगर (max_गिनती >= 0 && max_गिनती < commit_गिनती)
		commit_गिनती = max_गिनती;

	म_लिखो("%d\n", commit_गिनती + वृक्ष_गिनती + blob_गिनती + tag_गिनती);
	free_bitmap_सूचकांक(bitmap_git);
	वापस 0;
}

अटल पूर्णांक try_bitmap_traversal(काष्ठा rev_info *revs,
				पूर्णांक filter_provided_objects)
{
	काष्ठा bitmap_सूचकांक *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	अगर (revs->max_गिनती >= 0)
		वापस -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	अगर (revs->बायाँ_दायाँ)
		वापस -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	अगर (!bitmap_git)
		वापस -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_सूचकांक(bitmap_git);
	वापस 0;
}

अटल पूर्णांक try_bitmap_disk_usage(काष्ठा rev_info *revs,
				 पूर्णांक filter_provided_objects)
{
	काष्ठा bitmap_सूचकांक *bitmap_git;
	off_t आकार_from_bitmap;

	अगर (!show_disk_usage)
		वापस -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	अगर (!bitmap_git)
		वापस -1;

	आकार_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(आकार_from_bitmap);

	free_bitmap_सूचकांक(bitmap_git);
	वापस 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
अटल व्योम prepare_maximal_independent(काष्ठा rev_info *revs)
{
	काष्ठा commit_list *c;

	अगर (!revs->maximal_only)
		वापस;

	क्रम (c = revs->commits; c; c = c->अग्र) {
		अगर (c->item->object.flags & UNINTERESTING)
			वापस;
	}

	अगर (revs->limited ||
	    revs->topo_order ||
	    revs->first_parent_only ||
	    revs->reverse ||
	    revs->max_गिनती >= 0 ||
	    revs->skip_गिनती >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_तथ्य.nr ||
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
		वापस;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

पूर्णांक cmd_rev_list(पूर्णांक argc,
		 स्थिर अक्षर **argv,
		 स्थिर अक्षर *prefix,
		 काष्ठा repository *repo)
{
	काष्ठा rev_info revs;
	काष्ठा rev_list_info info;
	काष्ठा setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	पूर्णांक i;
	पूर्णांक bisect_list = 0;
	पूर्णांक bisect_show_vars = 0;
	पूर्णांक bisect_find_all = 0;
	पूर्णांक use_bitmap_सूचकांक = 0;
	पूर्णांक filter_provided_objects = 0;
	स्थिर अक्षर *show_progress = शून्य;
	पूर्णांक ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, शून्य);
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
	क्रम (i = 1; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];
		अगर (!म_भेद(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} अन्यथा अगर (skip_prefix(arg, "--missing=", &arg)) {
			parse_missing_action_मूल्य(repo, arg);
		} अन्यथा अगर (!म_भेद(arg, "--missing-only")) {
			arg_missing_only = 1;
		} अन्यथा अगर (!म_भेद(arg, "-z")) {
			पंक्ति_term = '\0';
			info_term = '\0';
		}
	}

	अगर (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	अगर (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = setup_revisions(argc, argv, &revs, &s_r_opt);

	स_रखो(&info, 0, माप(info));
	info.revs = &revs;
	अगर (revs.bisect)
		bisect_list = 1;

	अगर (revs.diffopt.flags.quick)
		info.flags |= REV_LIST_QUIET;
	क्रम (i = 1 ; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];

		अगर (!म_भेद(arg, "--header")) {
			revs.verbose_header = 1;
			जारी;
		}
		अगर (!म_भेद(arg, "--timestamp")) {
			info.show_timestamp = 1;
			जारी;
		}
		अगर (!म_भेद(arg, "--bisect")) {
			bisect_list = 1;
			जारी;
		}
		अगर (!म_भेद(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_find_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			जारी;
		}
		अगर (!म_भेद(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			जारी;
		}
		अगर (!म_भेद(arg, "--use-bitmap-index")) {
			use_bitmap_सूचकांक = 1;
			जारी;
		}
		अगर (!म_भेद(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			जाओ cleanup;
		}
		अगर (skip_prefix(arg, "--progress=", &arg)) {
			show_progress = arg;
			जारी;
		}
		अगर (!म_भेद(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			जारी;
		}
		अगर (!म_भेद(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			जारी;
		}

		अगर (!म_भेद(arg, "--exclude-promisor-objects"))
			जारी; /* already handled above */
		अगर (skip_prefix(arg, "--missing=", &arg))
			जारी; /* already handled above */

		अगर (!म_भेद(arg, ("--no-object-names"))) {
			arg_show_object_names = 0;
			जारी;
		}

		अगर (!म_भेद(arg, ("--object-names"))) {
			arg_show_object_names = 1;
			जारी;
		}

		अगर (!म_भेद(arg, ("--commit-header"))) {
			revs.include_header = 1;
			जारी;
		}

		अगर (!म_भेद(arg, ("--no-commit-header"))) {
			revs.include_header = 0;
			जारी;
		}

		अगर (skip_prefix(arg, "--disk-usage", &arg)) {
			अगर (*arg == '=') {
				अगर (!म_भेद(++arg, "human")) {
					human_readable = 1;
				} अन्यथा
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} अन्यथा अगर (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			जारी;
		}

		अगर (!म_भेद(arg, "--missing-only"))
			जारी;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	अगर (!पंक्ति_term) {
		अगर (revs.graph || revs.verbose_header || show_disk_usage ||
		    info.show_timestamp || info.header_prefix || bisect_list ||
		    use_bitmap_सूचकांक || revs.edge_hint || revs.बायाँ_दायाँ ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	अगर (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_header = 1;
	अगर (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		अगर (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_header)
			info.header_prefix = "";
		अन्यथा
			info.header_prefix = "commit ";
	}
	अन्यथा अगर (revs.verbose_header)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	अगर ((!revs.commits && reflog_walk_empty(revs.reflog_info) &&
	     (!(revs.tag_objects || revs.वृक्ष_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_निवेश_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	अगर (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	अगर (revs.गिनती &&
	    (revs.tag_objects || revs.वृक्ष_objects || revs.blob_objects) &&
	    (revs.बायाँ_दायाँ || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.गिनती, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_बफर = (revs.verbose_header ||
			      revs.grep_filter.pattern_list ||
			      revs.grep_filter.header_list);
	अगर (bisect_list)
		revs.limited = 1;

	अगर (show_progress)
		progress = start_delayed_progress(the_repository,
						  show_progress, 0);

	अगर (use_bitmap_सूचकांक) {
		अगर (!try_bitmap_गिनती(&revs, filter_provided_objects))
			जाओ cleanup;
		अगर (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			जाओ cleanup;
		अगर (!try_bitmap_traversal(&revs, filter_provided_objects))
			जाओ cleanup;
	}

	अगर (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	अगर (revs.वृक्ष_objects)
		mark_edges_uninteresting(&revs, show_edge, 0);

	अगर (bisect_list) {
		पूर्णांक reaches, all;
		अचिन्हित bisect_flags = 0;

		अगर (bisect_find_all)
			bisect_flags |= FIND_BISECTION_ALL;

		अगर (revs.first_parent_only)
			bisect_flags |= FIND_BISECTION_FIRST_PARENT_ONLY;

		find_bisection(&revs.commits, &reaches, &all, bisect_flags);

		अगर (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			जाओ cleanup;
		}
	}

	अगर (filter_provided_objects) {
		काष्ठा commit_list *c;
		क्रम (i = 0; i < revs.pending.nr; i++) {
			काष्ठा object_array_entry *pending = revs.pending.objects + i;
			pending->item->flags |= NOT_USER_GIVEN;
		}
		क्रम (c = revs.commits; c; c = c->अग्र)
			c->item->object.flags |= NOT_USER_GIVEN;
	}

	अगर (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_आकार);
	अगर (should_collect_missing()) {
		काष्ठा oidset_iter iter;
		काष्ठा object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_आकार);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		जबतक ((oid = oidset_iter_अग्र(&iter)))
			add_missing_object_entry(oid, शून्य, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : शून्य));

	अगर (arg_print_omitted) {
		काष्ठा oidset_iter iter;
		काष्ठा object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		जबतक ((oid = oidset_iter_अग्र(&iter)))
			म_लिखो("~%s\n", oid_to_hex(oid));
		oidset_clear(&omitted_objects);
	}
	अगर (should_collect_missing()) {
		काष्ठा missing_objects_map_entry *entry;
		काष्ठा oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		जबतक ((entry = oidmap_iter_अग्र(&iter))) {
			print_missing_object(entry, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_entry_free);
	}

	stop_progress(&progress);

	अगर (revs.गिनती) {
		अगर (revs.बायाँ_दायाँ && revs.cherry_mark)
			म_लिखो("%d\t%d\t%d\n", revs.गिनती_बायाँ, revs.गिनती_दायाँ, revs.गिनती_same);
		अन्यथा अगर (revs.बायाँ_दायाँ)
			म_लिखो("%d\t%d\n", revs.गिनती_बायाँ, revs.गिनती_दायाँ);
		अन्यथा अगर (revs.cherry_mark)
			म_लिखो("%d\t%d\n", revs.गिनती_बायाँ + revs.गिनती_दायाँ, revs.गिनती_same);
		अन्यथा
			म_लिखो("%d\n", revs.गिनती_बायाँ + revs.गिनती_दायाँ);
	}

	अगर (show_disk_usage)
		print_disk_usage(योग_disk_usage);

cleanup:
	release_revisions(&revs);
	वापस ret;
}
