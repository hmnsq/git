#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list-objects.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list-objects-filter-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack-bitmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "log-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "graph.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "bisect.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "progress.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "reflog-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oidset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oidmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"

ਬਣਤਰ rev_list_info {
	ਬਣਤਰ rev_info *revs;
	ਪੂਰਨ flags;
	ਪੂਰਨ show_timestamp;
	ਪੂਰਨ hdr_termination;
	ਅਚਲ ਅੱਖਰ *ਸਿਰਲੇਖ_ਅਗੇਤਰ;
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ rev_list_usage[] =
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

ਸਥਿਰ ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਤਰੱਕੀ_counter;

ਸਥਿਰ ਬਣਤਰ oidset omitted_objects;
ਸਥਿਰ ਪੂਰਨ arg_print_omitted; /* print objects omitted by filter */

ਬਣਤਰ missing_objects_map_ਇੰਦਰਾਜ {
	ਬਣਤਰ oidmap_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
	ਅੱਖਰ *ਰਸਤਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ type;
};

ਸਥਿਰ ਖਾਲੀ missing_objects_map_ਇੰਦਰਾਜ_free(ਖਾਲੀ *e)
{
	ਬਣਤਰ missing_objects_map_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ =
		container_ਦਾ(e, ਬਣਤਰ missing_objects_map_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);

	ਮੁਕਤ(ਇੰਦਰਾਜ->ਰਸਤਾ);
	ਮੁਕਤ(ਇੰਦਰਾਜ);
}

ਸਥਿਰ ਬਣਤਰ oidmap missing_objects;
ਗਿਣਤੀ missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ਸਥਿਰ ਗਿਣਤੀ missing_action arg_missing_action;
ਸਥਿਰ ਪੂਰਨ arg_missing_only;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ should_collect_missing(ਖਾਲੀ)
{
	ਵਾਪਸ arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
ਸਥਿਰ ਪੂਰਨ arg_show_object_ਨਾਂ = 1;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_OIDSET_ਮਾਪ     (16*1024)

ਸਥਿਰ ਅੱਖਰ ਸਤਰ_ਪਦ = '\n';
ਸਥਿਰ ਅੱਖਰ info_ਪਦ = ' ';

ਸਥਿਰ ਪੂਰਨ show_disk_usage;
ਸਥਿਰ off_t ਕੁੱਲ_disk_usage;
ਸਥਿਰ ਪੂਰਨ human_readable;

ਸਥਿਰ off_t get_object_disk_usage(ਬਣਤਰ object *obj)
{
	off_t ਮਾਪ;
	ਬਣਤਰ object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &ਮਾਪ;
	ਜੇ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_ਮੰਜ਼ਿਲ_hex(&obj->oid));
	ਵਾਪਸ ਮਾਪ;
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_missing_object_ਇੰਦਰਾਜ(ਬਣਤਰ object_id *oid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ type)
{
	ਬਣਤਰ missing_objects_map_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;

	ਜੇ (oidmap_get(&missing_objects, oid))
		ਵਾਪਸ;

	CALLOC_ARRAY(ਇੰਦਰਾਜ, 1);
	ਇੰਦਰਾਜ->ਇੰਦਰਾਜ.oid = *oid;
	ਇੰਦਰਾਜ->type = type;
	ਜੇ (ਰਸਤਾ)
		ਇੰਦਰਾਜ->ਰਸਤਾ = xstrdup(ਰਸਤਾ);
	oidmap_put(&missing_objects, ਇੰਦਰਾਜ);
}

ਸਥਿਰ ਖਾਲੀ print_missing_object(ਬਣਤਰ missing_objects_map_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
				 ਪੂਰਨ print_missing_info)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	ਜੇ (arg_missing_only && ਸਤਰ_ਪਦ)
		ਲਿਖੋ("%s", oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->ਇੰਦਰਾਜ.oid));
	ਨਹੀਂਤਾਂ ਜੇ (ਸਤਰ_ਪਦ)
		ਲਿਖੋ("?%s", oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->ਇੰਦਰਾਜ.oid));
	ਨਹੀਂਤਾਂ
		ਲਿਖੋ("%s%cmissing=yes", oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->ਇੰਦਰਾਜ.oid),
		       info_ਪਦ);

	ਜੇ (!print_missing_info) {
		ਅੱਖਰ_ਲਿਖੋ(ਸਤਰ_ਪਦ);
		ਵਾਪਸ;
	}

	ਜੇ (ਇੰਦਰਾਜ->ਰਸਤਾ && *ਇੰਦਰਾਜ->ਰਸਤਾ) {
		strbuf_addf(&sb, "%cpath=", info_ਪਦ);

		ਜੇ (ਸਤਰ_ਪਦ) {
			ਬਣਤਰ strbuf ਰਸਤਾ = STRBUF_INIT;

			quote_ਰਸਤਾ(ਇੰਦਰਾਜ->ਰਸਤਾ, ਨਿਰਾ, &ਰਸਤਾ, QUOTE_ਰਸਤਾ_QUOTE_SP);
			strbuf_addbuf(&sb, &ਰਸਤਾ);

			strbuf_release(&ਰਸਤਾ);
		} ਨਹੀਂਤਾਂ {
			strbuf_addstr(&sb, ਇੰਦਰਾਜ->ਰਸਤਾ);
		}
	}
	ਜੇ (ਇੰਦਰਾਜ->type)
		strbuf_addf(&sb, "%ctype=%s", info_ਪਦ, type_name(ਇੰਦਰਾਜ->type));

	fwrite(sb.buf, ਆਕਾਰ(ਅੱਖਰ), sb.len, stdout);
	ਅੱਖਰ_ਲਿਖੋ(ਸਤਰ_ਪਦ);

	strbuf_release(&sb);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਅੰਤ_object_ma(ਬਣਤਰ object *obj, ਅਚਲ ਅੱਖਰ *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	ਚੋਣ (arg_missing_action) {
	ਹਾਲਤ MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_ਮੰਜ਼ਿਲ_hex(&obj->oid));
		ਵਾਪਸ;

	ਹਾਲਤ MA_ALLOW_ANY:
		ਵਾਪਸ;

	ਹਾਲਤ MA_PRINT:
	ਹਾਲਤ MA_PRINT_INFO:
		ਜੋੜੋ_missing_object_ਇੰਦਰਾਜ(&obj->oid, name, obj->type);
		ਵਾਪਸ;

	ਹਾਲਤ MA_ALLOW_PROMISOR:
		ਜੇ (is_promisor_object(the_repository, &obj->oid))
			ਵਾਪਸ;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_ਮੰਜ਼ਿਲ_hex(&obj->oid));
		ਵਾਪਸ;

	ਮੂਲ:
		BUG("unhandled missing_action");
		ਵਾਪਸ;
	}
}

ਸਥਿਰ ਖਾਲੀ ਅੰਤ_commit(ਬਣਤਰ commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = ਨਿਰਾ;
	free_commit_ਬਫਰ(the_repository->parsed_objects,
			   commit);
}

ਸਥਿਰ ਖਾਲੀ show_commit(ਬਣਤਰ commit *commit, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ rev_list_info *info = ਅੰਕੜੇ;
	ਬਣਤਰ rev_info *revs = info->revs;

	display_ਤਰੱਕੀ(ਤਰੱਕੀ, ++ਤਰੱਕੀ_counter);

	ਜੇ (revs->do_not_die_on_missing_objects &&
	    oidset_ਸ਼ਾਮਲ_ਹੈ(&revs->missing_commits, &commit->object.oid)) {
		ਅੰਤ_object_ma(&commit->object, ਨਿਰਾ);
		ਵਾਪਸ;
	}

	ਜੇ (arg_missing_only) {
		ਅੰਤ_commit(commit);
		ਵਾਪਸ;
	}

	ਜੇ (show_disk_usage)
		ਕੁੱਲ_disk_usage += get_object_disk_usage(&commit->object);

	ਜੇ (info->flags & REV_LIST_QUIET) {
		ਅੰਤ_commit(commit);
		ਵਾਪਸ;
	}

	ਗ੍ਰਾਫ_show_commit(revs->ਗ੍ਰਾਫ);

	ਜੇ (revs->ਗਿਣਤੀ_2) {
		ਜੇ (commit->object.flags & PATCHSAME)
			revs->ਗਿਣਤੀ_same++;
		ਨਹੀਂਤਾਂ ਜੇ (commit->object.flags & ਸਮਮਿਤ_ਖੱਬਾ)
			revs->ਗਿਣਤੀ_ਖੱਬਾ++;
		ਨਹੀਂਤਾਂ
			revs->ਗਿਣਤੀ_ਸੱਜਾ++;
		ਅੰਤ_commit(commit);
		ਵਾਪਸ;
	}

	ਜੇ (info->show_timestamp)
		ਲਿਖੋ("%"PRItime" ", commit->date);
	ਜੇ (info->ਸਿਰਲੇਖ_ਅਗੇਤਰ)
		fputs(info->ਸਿਰਲੇਖ_ਅਗੇਤਰ, stdout);

	ਜੇ (revs->include_ਸਿਰਲੇਖ) {
		ਜੇ (!revs->ਗ੍ਰਾਫ && ਸਤਰ_ਪਦ)
			fputs(get_revision_mark(revs, commit), stdout);
		ਜੇ (revs->abbrev_commit && revs->abbrev)
			fputs(repo_ਲੱਭੋ_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		ਨਹੀਂਤਾਂ
			fputs(oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid), stdout);

		ਜੇ (!ਸਤਰ_ਪਦ) {
			ਜੇ (commit->object.flags & BOUNDARY)
				ਲਿਖੋ("%cboundary=yes", info_ਪਦ);
		}
	}
	ਜੇ (revs->print_parents) {
		ਬਣਤਰ commit_list *parents = commit->parents;
		ਜਦੋਂ (parents) {
			ਲਿਖੋ(" %s", oid_ਮੰਜ਼ਿਲ_hex(&parents->ਵਸਤੂ->object.oid));
			parents = parents->ਅਗਲਾ;
		}
	}
	ਜੇ (revs->children.name) {
		ਬਣਤਰ commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		ਜਦੋਂ (children) {
			ਲਿਖੋ(" %s", oid_ਮੰਜ਼ਿਲ_hex(&children->ਵਸਤੂ->object.oid));
			children = children->ਅਗਲਾ;
		}
	}
	show_decorations(revs, commit);
	ਜੇ (revs->commit_format == CMIT_FMT_ONELINE)
		ਅੱਖਰ_ਲਿਖੋ(' ');
	ਨਹੀਂਤਾਂ ਜੇ (revs->include_ਸਿਰਲੇਖ)
		ਅੱਖਰ_ਲਿਖੋ(ਸਤਰ_ਪਦ);

	ਜੇ (revs->verbose_ਸਿਰਲੇਖ) {
		ਬਣਤਰ strbuf buf = STRBUF_INIT;
		ਬਣਤਰ pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_ਬਹੁਲਕ = revs->date_ਬਹੁਲਕ;
		ctx.date_ਬਹੁਲਕ_explicit = revs->date_ਬਹੁਲਕ_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		ਜੇ (buf.len) {
			ਜੇ (revs->commit_format != CMIT_FMT_ONELINE)
				ਗ੍ਰਾਫ_show_oneline(revs->ਗ੍ਰਾਫ);

			ਗ੍ਰਾਫ_show_commit_ਸੁਨੇਹਾ(revs->ਗ੍ਰਾਫ, stdout, &buf);

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
			ਜੇ (buf.len && buf.buf[buf.len - 1] == '\n')
				ਗ੍ਰਾਫ_show_padding(revs->ਗ੍ਰਾਫ);
			ਅੱਖਰ_ਲਿਖੋ(info->hdr_termination);
		} ਨਹੀਂਤਾਂ {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			ਜੇ (ਗ੍ਰਾਫ_show_remainder(revs->ਗ੍ਰਾਫ))
				ਅੱਖਰ_ਲਿਖੋ('\n');
			ਜੇ (revs->commit_format == CMIT_FMT_ONELINE)
				ਅੱਖਰ_ਲਿਖੋ('\n');
		}
		strbuf_release(&buf);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (ਗ੍ਰਾਫ_show_remainder(revs->ਗ੍ਰਾਫ))
			ਅੱਖਰ_ਲਿਖੋ('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	ਅੰਤ_commit(commit);
}

ਸਥਿਰ ਪੂਰਨ ਅੰਤ_object(ਬਣਤਰ object *obj, ਅਚਲ ਅੱਖਰ *name, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ rev_list_info *info = cb_ਅੰਕੜੇ;
	ਜੇ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, ਨਿਰਾ, 0) < 0) {
		ਅੰਤ_object_ma(obj, name);
		ਵਾਪਸ 1;
	}
	ਜੇ (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ show_object(ਬਣਤਰ object *obj, ਅਚਲ ਅੱਖਰ *name, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ rev_list_info *info = cb_ਅੰਕੜੇ;
	ਬਣਤਰ rev_info *revs = info->revs;

	ਜੇ (ਅੰਤ_object(obj, name, cb_ਅੰਕੜੇ))
		ਵਾਪਸ;
	display_ਤਰੱਕੀ(ਤਰੱਕੀ, ++ਤਰੱਕੀ_counter);
	ਜੇ (arg_missing_only)
		ਵਾਪਸ;
	ਜੇ (show_disk_usage)
		ਕੁੱਲ_disk_usage += get_object_disk_usage(obj);
	ਜੇ (info->flags & REV_LIST_QUIET)
		ਵਾਪਸ;

	ਜੇ (revs->ਗਿਣਤੀ_2) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->ਗਿਣਤੀ_ਸੱਜਾ++;
		ਵਾਪਸ;
	}

	ਲਿਖੋ("%s", oid_ਮੰਜ਼ਿਲ_hex(&obj->oid));

	ਜੇ (arg_show_object_ਨਾਂ) {
		ਜੇ (ਸਤਰ_ਪਦ) {
			ਅੱਖਰ_ਲਿਖੋ(info_ਪਦ);
			ਲਈ (ਅਚਲ ਅੱਖਰ *p = name; *p && *p != '\n'; p++)
				ਅੱਖਰ_ਲਿਖੋ(*p);
		} ਨਹੀਂਤਾਂ ਜੇ (*name) {
			ਲਿਖੋ("%cpath=%s", info_ਪਦ, name);
		}
	}

	ਅੱਖਰ_ਲਿਖੋ(ਸਤਰ_ਪਦ);
}

ਸਥਿਰ ਖਾਲੀ show_ਕਿਨਾਰਾ(ਬਣਤਰ commit *commit)
{
	ਲਿਖੋ("-%s\n", oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
}

ਸਥਿਰ ਖਾਲੀ print_var_str(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *val)
{
	ਲਿਖੋ("%s='%s'\n", var, val);
}

ਸਥਿਰ ਖਾਲੀ print_var_int(ਅਚਲ ਅੱਖਰ *var, ਪੂਰਨ val)
{
	ਲਿਖੋ("%s=%d\n", var, val);
}

ਸਥਿਰ ਪੂਰਨ show_bisect_vars(ਬਣਤਰ rev_list_info *info, ਪੂਰਨ reaches, ਪੂਰਨ all)
{
	ਪੂਰਨ cnt, flags = info->flags;
	ਅੱਖਰ hex[GIT_MAX_HEXSZ + 1] = "";
	ਬਣਤਰ commit_list *tried;
	ਬਣਤਰ rev_info *revs = info->revs;

	ਜੇ (!revs->commits)
		ਵਾਪਸ 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       ਨਿਰਾ, ਨਿਰਾ);

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
	ਜੇ (cnt < reaches)
		cnt = reaches;

	ਜੇ (revs->commits)
		oid_ਮੰਜ਼ਿਲ_hex_r(hex, &revs->commits->ਵਸਤੂ->object.oid);

	ਜੇ (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		ਲਿਖੋ("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_ਕਦਮਾਂ(all));

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ show_object_fast(
	ਅਚਲ ਬਣਤਰ object_id *oid,
	ਗਿਣਤੀ object_type type ਅਣਵਰਤਿਆ,
	ਪੂਰਨ exclude ਅਣਵਰਤਿਆ,
	uint32_t name_hash ਅਣਵਰਤਿਆ,
	ਬਣਤਰ packed_git *ਮਿਲਿਆ_pack ਅਣਵਰਤਿਆ,
	off_t ਮਿਲਿਆ_ਵਿੱਥ ਅਣਵਰਤਿਆ,
	ਖਾਲੀ *payload ਅਣਵਰਤਿਆ)
{
	fprintf(stdout, "%s\n", oid_ਮੰਜ਼ਿਲ_hex(oid));
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ print_disk_usage(off_t ਮਾਪ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਜੇ (human_readable)
		strbuf_humanise_ਬਾਈਟਾਂ(&sb, ਮਾਪ);
	ਨਹੀਂਤਾਂ
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)ਮਾਪ);
	puts(sb.buf);
	strbuf_release(&sb);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ parse_missing_action_ਮੁੱਲ(ਬਣਤਰ repository *repo,
					     ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਜੇ (!strcmp(ਮੁੱਲ, "error")) {
		arg_missing_action = MA_ERROR;
		ਵਾਪਸ 1;
	}

	ਜੇ (!strcmp(ਮੁੱਲ, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		ਵਾਪਸ 1;
	}

	ਜੇ (!strcmp(ਮੁੱਲ, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		ਵਾਪਸ 1;
	}

	ਜੇ (!strcmp(ਮੁੱਲ, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		ਵਾਪਸ 1;
	}

	ਜੇ (!strcmp(ਮੁੱਲ, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		ਵਾਪਸ 1;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ try_bitmap_ਗਿਣਤੀ(ਬਣਤਰ rev_info *revs,
			    ਪੂਰਨ filter_provided_objects)
{
	uint32_t commit_ਗਿਣਤੀ = 0,
		 ਟੈਗ_ਗਿਣਤੀ = 0,
		 ਰੁੱਖ_ਗਿਣਤੀ = 0,
		 blob_ਗਿਣਤੀ = 0;
	ਪੂਰਨ max_ਗਿਣਤੀ;
	ਬਣਤਰ bitmap_ਸੂਚਕਾਂਕ *bitmap_git;

	/* This function only handles counting, not general traversal. */
	ਜੇ (!revs->ਗਿਣਤੀ_2)
		ਵਾਪਸ -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	ਜੇ (revs->ਖੱਬਾ_ਸੱਜਾ || revs->cherry_mark)
		ਵਾਪਸ -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	ਜੇ (revs->max_ਗਿਣਤੀ >= 0 &&
	    (revs->ਟੈਗ_objects || revs->ਰੁੱਖ_objects || revs->blob_objects))
		ਵਾਪਸ -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_ਗਿਣਤੀ = revs->max_ਗਿਣਤੀ;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ਜੇ (!bitmap_git)
		ਵਾਪਸ -1;

	ਗਿਣਤੀ_bitmap_commit_list(bitmap_git, &commit_ਗਿਣਤੀ,
				 revs->ਰੁੱਖ_objects ? &ਰੁੱਖ_ਗਿਣਤੀ : ਨਿਰਾ,
				 revs->blob_objects ? &blob_ਗਿਣਤੀ : ਨਿਰਾ,
				 revs->ਟੈਗ_objects ? &ਟੈਗ_ਗਿਣਤੀ : ਨਿਰਾ);
	ਜੇ (max_ਗਿਣਤੀ >= 0 && max_ਗਿਣਤੀ < commit_ਗਿਣਤੀ)
		commit_ਗਿਣਤੀ = max_ਗਿਣਤੀ;

	ਲਿਖੋ("%d\n", commit_ਗਿਣਤੀ + ਰੁੱਖ_ਗਿਣਤੀ + blob_ਗਿਣਤੀ + ਟੈਗ_ਗਿਣਤੀ);
	free_bitmap_ਸੂਚਕਾਂਕ(bitmap_git);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ try_bitmap_traversal(ਬਣਤਰ rev_info *revs,
				ਪੂਰਨ filter_provided_objects)
{
	ਬਣਤਰ bitmap_ਸੂਚਕਾਂਕ *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	ਜੇ (revs->max_ਗਿਣਤੀ >= 0)
		ਵਾਪਸ -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	ਜੇ (revs->ਖੱਬਾ_ਸੱਜਾ)
		ਵਾਪਸ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ਜੇ (!bitmap_git)
		ਵਾਪਸ -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_ਸੂਚਕਾਂਕ(bitmap_git);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ try_bitmap_disk_usage(ਬਣਤਰ rev_info *revs,
				 ਪੂਰਨ filter_provided_objects)
{
	ਬਣਤਰ bitmap_ਸੂਚਕਾਂਕ *bitmap_git;
	off_t ਮਾਪ_from_bitmap;

	ਜੇ (!show_disk_usage)
		ਵਾਪਸ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ਜੇ (!bitmap_git)
		ਵਾਪਸ -1;

	ਮਾਪ_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(ਮਾਪ_from_bitmap);

	free_bitmap_ਸੂਚਕਾਂਕ(bitmap_git);
	ਵਾਪਸ 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
ਸਥਿਰ ਖਾਲੀ prepare_maximal_independent(ਬਣਤਰ rev_info *revs)
{
	ਬਣਤਰ commit_list *c;

	ਜੇ (!revs->maximal_only)
		ਵਾਪਸ;

	ਲਈ (c = revs->commits; c; c = c->ਅਗਲਾ) {
		ਜੇ (c->ਵਸਤੂ->object.flags & UNINTERESTING)
			ਵਾਪਸ;
	}

	ਜੇ (revs->limited ||
	    revs->topo_ਕ੍ਰਮ ||
	    revs->ਪਹਿਲਾ_ਮਾਪਾ_only ||
	    revs->ਉਲਟ ||
	    revs->max_ਗਿਣਤੀ >= 0 ||
	    revs->skip_ਗਿਣਤੀ >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_ਅੰਕੜੇ.nr ||
	    revs->ਗਿਣਤੀ_2 ||
	    revs->ਖੱਬਾ_ਸੱਜਾ ||
	    revs->boundary ||
	    revs->ਟੈਗ_objects ||
	    revs->ਰੁੱਖ_objects ||
	    revs->blob_objects ||
	    revs->filter.choice ||
	    revs->reflog_info ||
	    revs->diff ||
	    revs->grep_filter.ਨਮੂਨਾ_list ||
	    revs->grep_filter.ਸਿਰਲੇਖ_list ||
	    revs->verbose_ਸਿਰਲੇਖ ||
	    revs->print_parents ||
	    revs->ਕਿਨਾਰਾ_hint ||
	    revs->unpacked ||
	    revs->no_kept_objects ||
	    revs->ਸਤਰ_level_traverse)
		ਵਾਪਸ;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

ਪੂਰਨ cmd_rev_list(ਪੂਰਨ argc,
		 ਅਚਲ ਅੱਖਰ **argv,
		 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ rev_info revs;
	ਬਣਤਰ rev_list_info info;
	ਬਣਤਰ ਤਿਆਰੀ_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	ਪੂਰਨ i;
	ਪੂਰਨ bisect_list = 0;
	ਪੂਰਨ bisect_show_vars = 0;
	ਪੂਰਨ bisect_ਲੱਭੋ_all = 0;
	ਪੂਰਨ use_bitmap_ਸੂਚਕਾਂਕ = 0;
	ਪੂਰਨ filter_provided_objects = 0;
	ਅਚਲ ਅੱਖਰ *show_ਤਰੱਕੀ = ਨਿਰਾ;
	ਪੂਰਨ ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	repo_init_revisions(the_repository, &revs, ਅਗੇਤਰ);
	revs.abbrev = DEFAULT_ABBREV;
	revs.commit_format = CMIT_FMT_UNSPECIFIED;
	revs.include_ਸਿਰਲੇਖ = 1;

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
	ਲਈ (i = 1; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];
		ਜੇ (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(arg, "--missing=", &arg)) {
			parse_missing_action_ਮੁੱਲ(repo, arg);
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "-z")) {
			ਸਤਰ_ਪਦ = '\0';
			info_ਪਦ = '\0';
		}
	}

	ਜੇ (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	ਜੇ (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = ਤਿਆਰੀ_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, ਆਕਾਰ(info));
	info.revs = &revs;
	ਜੇ (revs.bisect)
		bisect_list = 1;

	ਜੇ (revs.diffopt.flags.ਤੇਜ਼)
		info.flags |= REV_LIST_QUIET;
	ਲਈ (i = 1 ; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];

		ਜੇ (!strcmp(arg, "--header")) {
			revs.verbose_ਸਿਰਲੇਖ = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_ਲੱਭੋ_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_ਸੂਚਕਾਂਕ = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			ਜਾਓ cleanup;
		}
		ਜੇ (skip_ਅਗੇਤਰ(arg, "--progress=", &arg)) {
			show_ਤਰੱਕੀ = arg;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			ਜਾਰੀ;
		}

		ਜੇ (!strcmp(arg, "--exclude-promisor-objects"))
			ਜਾਰੀ; /* already handled above */
		ਜੇ (skip_ਅਗੇਤਰ(arg, "--missing=", &arg))
			ਜਾਰੀ; /* already handled above */

		ਜੇ (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_ਨਾਂ = 0;
			ਜਾਰੀ;
		}

		ਜੇ (!strcmp(arg, ("--object-names"))) {
			arg_show_object_ਨਾਂ = 1;
			ਜਾਰੀ;
		}

		ਜੇ (!strcmp(arg, ("--commit-header"))) {
			revs.include_ਸਿਰਲੇਖ = 1;
			ਜਾਰੀ;
		}

		ਜੇ (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_ਸਿਰਲੇਖ = 0;
			ਜਾਰੀ;
		}

		ਜੇ (skip_ਅਗੇਤਰ(arg, "--disk-usage", &arg)) {
			ਜੇ (*arg == '=') {
				ਜੇ (!strcmp(++arg, "human")) {
					human_readable = 1;
				} ਨਹੀਂਤਾਂ
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} ਨਹੀਂਤਾਂ ਜੇ (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			ਜਾਰੀ;
		}

		ਜੇ (!strcmp(arg, "--missing-only"))
			ਜਾਰੀ;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	ਜੇ (!ਸਤਰ_ਪਦ) {
		ਜੇ (revs.ਗ੍ਰਾਫ || revs.verbose_ਸਿਰਲੇਖ || show_disk_usage ||
		    info.show_timestamp || info.ਸਿਰਲੇਖ_ਅਗੇਤਰ || bisect_list ||
		    use_bitmap_ਸੂਚਕਾਂਕ || revs.ਕਿਨਾਰਾ_hint || revs.ਖੱਬਾ_ਸੱਜਾ ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	ਜੇ (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_ਸਿਰਲੇਖ = 1;
	ਜੇ (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		ਜੇ (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_ਸਿਰਲੇਖ)
			info.ਸਿਰਲੇਖ_ਅਗੇਤਰ = "";
		ਨਹੀਂਤਾਂ
			info.ਸਿਰਲੇਖ_ਅਗੇਤਰ = "commit ";
	}
	ਨਹੀਂਤਾਂ ਜੇ (revs.verbose_ਸਿਰਲੇਖ)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	ਜੇ ((!revs.commits && reflog_walk_ਖਾਲੀ_ਥਾਵਾਂ(revs.reflog_info) &&
	     (!(revs.ਟੈਗ_objects || revs.ਰੁੱਖ_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_ਨਿਵੇਸ਼_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	ਜੇ (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	ਜੇ (revs.ਗਿਣਤੀ_2 &&
	    (revs.ਟੈਗ_objects || revs.ਰੁੱਖ_objects || revs.blob_objects) &&
	    (revs.ਖੱਬਾ_ਸੱਜਾ || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.ਗਿਣਤੀ_2, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_ਬਫਰ = (revs.verbose_ਸਿਰਲੇਖ ||
			      revs.grep_filter.ਨਮੂਨਾ_list ||
			      revs.grep_filter.ਸਿਰਲੇਖ_list);
	ਜੇ (bisect_list)
		revs.limited = 1;

	ਜੇ (show_ਤਰੱਕੀ)
		ਤਰੱਕੀ = ਸ਼ੁਰੂ_delayed_ਤਰੱਕੀ(the_repository,
						  show_ਤਰੱਕੀ, 0);

	ਜੇ (use_bitmap_ਸੂਚਕਾਂਕ) {
		ਜੇ (!try_bitmap_ਗਿਣਤੀ(&revs, filter_provided_objects))
			ਜਾਓ cleanup;
		ਜੇ (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			ਜਾਓ cleanup;
		ਜੇ (!try_bitmap_traversal(&revs, filter_provided_objects))
			ਜਾਓ cleanup;
	}

	ਜੇ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	ਜੇ (revs.ਰੁੱਖ_objects)
		mark_ਕਿਨਾਰੇ_uninteresting(&revs, show_ਕਿਨਾਰਾ, 0);

	ਜੇ (bisect_list) {
		ਪੂਰਨ reaches, all;
		ਬਿਨਾਨਿਸ਼ਾਨੀ bisect_flags = 0;

		ਜੇ (bisect_ਲੱਭੋ_all)
			bisect_flags |= ਲੱਭੋ_BISECTION_ALL;

		ਜੇ (revs.ਪਹਿਲਾ_ਮਾਪਾ_only)
			bisect_flags |= ਲੱਭੋ_BISECTION_ਪਹਿਲਾ_ਮਾਪਾ_ONLY;

		ਲੱਭੋ_bisection(&revs.commits, &reaches, &all, bisect_flags);

		ਜੇ (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			ਜਾਓ cleanup;
		}
	}

	ਜੇ (filter_provided_objects) {
		ਬਣਤਰ commit_list *c;
		ਲਈ (i = 0; i < revs.pending.nr; i++) {
			ਬਣਤਰ object_array_ਇੰਦਰਾਜ *pending = revs.pending.objects + i;
			pending->ਵਸਤੂ->flags |= NOT_USER_GIVEN;
		}
		ਲਈ (c = revs.commits; c; c = c->ਅਗਲਾ)
			c->ਵਸਤੂ->object.flags |= NOT_USER_GIVEN;
	}

	ਜੇ (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_ਮਾਪ);
	ਜੇ (should_collect_missing()) {
		ਬਣਤਰ oidset_iter iter;
		ਬਣਤਰ object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_ਮਾਪ);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		ਜਦੋਂ ((oid = oidset_iter_ਅਗਲਾ(&iter)))
			ਜੋੜੋ_missing_object_ਇੰਦਰਾਜ(oid, ਨਿਰਾ, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : ਨਿਰਾ));

	ਜੇ (arg_print_omitted) {
		ਬਣਤਰ oidset_iter iter;
		ਬਣਤਰ object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		ਜਦੋਂ ((oid = oidset_iter_ਅਗਲਾ(&iter)))
			ਲਿਖੋ("~%s\n", oid_ਮੰਜ਼ਿਲ_hex(oid));
		oidset_clear(&omitted_objects);
	}
	ਜੇ (should_collect_missing()) {
		ਬਣਤਰ missing_objects_map_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
		ਬਣਤਰ oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		ਜਦੋਂ ((ਇੰਦਰਾਜ = oidmap_iter_ਅਗਲਾ(&iter))) {
			print_missing_object(ਇੰਦਰਾਜ, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_ਇੰਦਰਾਜ_free);
	}

	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ);

	ਜੇ (revs.ਗਿਣਤੀ_2) {
		ਜੇ (revs.ਖੱਬਾ_ਸੱਜਾ && revs.cherry_mark)
			ਲਿਖੋ("%d\t%d\t%d\n", revs.ਗਿਣਤੀ_ਖੱਬਾ, revs.ਗਿਣਤੀ_ਸੱਜਾ, revs.ਗਿਣਤੀ_same);
		ਨਹੀਂਤਾਂ ਜੇ (revs.ਖੱਬਾ_ਸੱਜਾ)
			ਲਿਖੋ("%d\t%d\n", revs.ਗਿਣਤੀ_ਖੱਬਾ, revs.ਗਿਣਤੀ_ਸੱਜਾ);
		ਨਹੀਂਤਾਂ ਜੇ (revs.cherry_mark)
			ਲਿਖੋ("%d\t%d\n", revs.ਗਿਣਤੀ_ਖੱਬਾ + revs.ਗਿਣਤੀ_ਸੱਜਾ, revs.ਗਿਣਤੀ_same);
		ਨਹੀਂਤਾਂ
			ਲਿਖੋ("%d\n", revs.ਗਿਣਤੀ_ਖੱਬਾ + revs.ਗਿਣਤੀ_ਸੱਜਾ);
	}

	ਜੇ (show_disk_usage)
		print_disk_usage(ਕੁੱਲ_disk_usage);

cleanup:
	release_revisions(&revs);
	ਵਾਪਸ ret;
}
