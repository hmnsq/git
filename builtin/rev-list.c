#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

ರಚನೆ rev_list_info {
	ರಚನೆ rev_info *revs;
	ಪೂರ್ಣಾಂಕ flags;
	ಪೂರ್ಣಾಂಕ show_timestamp;
	ಪೂರ್ಣಾಂಕ hdr_termination;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಶೀರ್ಷಿಕೆ_ಪೂರ್ವಪ್ರತ್ಯಯ;
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ rev_list_usage[] =
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

ಸ್ಥಿರ ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪ್ರಗತಿ_counter;

ಸ್ಥಿರ ರಚನೆ oidset omitted_objects;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ arg_print_omitted; /* print objects omitted by filter */

ರಚನೆ missing_objects_map_ನಮೂದು {
	ರಚನೆ oidmap_ನಮೂದು ನಮೂದು;
	ಅಕ್ಷರ *ಮಾರ್ಗ;
	ಚಿಹ್ನೆರಹಿತ type;
};

ಸ್ಥಿರ ಶೂನ್ಯ missing_objects_map_ನಮೂದು_free(ಶೂನ್ಯ *e)
{
	ರಚನೆ missing_objects_map_ನಮೂದು *ನಮೂದು =
		container_ದ(e, ರಚನೆ missing_objects_map_ನಮೂದು, ನಮೂದು);

	ಬಿಡುಗಡೆ(ನಮೂದು->ಮಾರ್ಗ);
	ಬಿಡುಗಡೆ(ನಮೂದು);
}

ಸ್ಥಿರ ರಚನೆ oidmap missing_objects;
ಎಣಿಕೆ missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ಸ್ಥಿರ ಎಣಿಕೆ missing_action arg_missing_action;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ arg_missing_only;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ should_collect_missing(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ arg_show_object_ಹೆಸರುಗಳು = 1;

#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_OIDSET_ಗಾತ್ರ     (16*1024)

ಸ್ಥಿರ ಅಕ್ಷರ ಸಾಲು_ಪದ = '\n';
ಸ್ಥಿರ ಅಕ್ಷರ info_ಪದ = ' ';

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_disk_usage;
ಸ್ಥಿರ off_t ಒಟ್ಟು_disk_usage;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ human_readable;

ಸ್ಥಿರ off_t get_object_disk_usage(ರಚನೆ object *obj)
{
	off_t ಗಾತ್ರ_2;
	ರಚನೆ object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &ಗಾತ್ರ_2;
	ಆದರೆ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_ಗಮ್ಯ_hex(&obj->oid));
	ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_missing_object_ನಮೂದು(ರಚನೆ object_id *oid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
				     ಚಿಹ್ನೆರಹಿತ type)
{
	ರಚನೆ missing_objects_map_ನಮೂದು *ನಮೂದು;

	ಆದರೆ (oidmap_get(&missing_objects, oid))
		ಹಿಂತಿರುಗಿ;

	CALLOC_ARRAY(ನಮೂದು, 1);
	ನಮೂದು->ನಮೂದು.oid = *oid;
	ನಮೂದು->type = type;
	ಆದರೆ (ಮಾರ್ಗ)
		ನಮೂದು->ಮಾರ್ಗ = xstrdup(ಮಾರ್ಗ);
	oidmap_put(&missing_objects, ನಮೂದು);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_missing_object(ರಚನೆ missing_objects_map_ನಮೂದು *ನಮೂದು,
				 ಪೂರ್ಣಾಂಕ print_missing_info)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	ಆದರೆ (arg_missing_only && ಸಾಲು_ಪದ)
		ಬರೆ("%s", oid_ಗಮ್ಯ_hex(&ನಮೂದು->ನಮೂದು.oid));
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸಾಲು_ಪದ)
		ಬರೆ("?%s", oid_ಗಮ್ಯ_hex(&ನಮೂದು->ನಮೂದು.oid));
	ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ("%s%cmissing=yes", oid_ಗಮ್ಯ_hex(&ನಮೂದು->ನಮೂದು.oid),
		       info_ಪದ);

	ಆದರೆ (!print_missing_info) {
		ಅಕ್ಷರ_ಬರೆ(ಸಾಲು_ಪದ);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (ನಮೂದು->ಮಾರ್ಗ && *ನಮೂದು->ಮಾರ್ಗ) {
		strbuf_addf(&sb, "%cpath=", info_ಪದ);

		ಆದರೆ (ಸಾಲು_ಪದ) {
			ರಚನೆ strbuf ಮಾರ್ಗ = STRBUF_INIT;

			quote_ಮಾರ್ಗ(ನಮೂದು->ಮಾರ್ಗ, ಖಾಲಿ, &ಮಾರ್ಗ, QUOTE_ಮಾರ್ಗ_QUOTE_SP);
			strbuf_addbuf(&sb, &ಮಾರ್ಗ);

			strbuf_release(&ಮಾರ್ಗ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			strbuf_addstr(&sb, ನಮೂದು->ಮಾರ್ಗ);
		}
	}
	ಆದರೆ (ನಮೂದು->type)
		strbuf_addf(&sb, "%ctype=%s", info_ಪದ, type_name(ನಮೂದು->type));

	fwrite(sb.buf, ಗಾತ್ರ(ಅಕ್ಷರ), sb.len, stdout);
	ಅಕ್ಷರ_ಬರೆ(ಸಾಲು_ಪದ);

	strbuf_release(&sb);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಮುಕ್ತಾಯ_object_ma(ರಚನೆ object *obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	ಆಯ್ಕೆ (arg_missing_action) {
	ಸ್ಥಿತಿ MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_ಗಮ್ಯ_hex(&obj->oid));
		ಹಿಂತಿರುಗಿ;

	ಸ್ಥಿತಿ MA_ALLOW_ANY:
		ಹಿಂತಿರುಗಿ;

	ಸ್ಥಿತಿ MA_PRINT:
	ಸ್ಥಿತಿ MA_PRINT_INFO:
		ಕೂಡಿಸು_missing_object_ನಮೂದು(&obj->oid, name, obj->type);
		ಹಿಂತಿರುಗಿ;

	ಸ್ಥಿತಿ MA_ALLOW_PROMISOR:
		ಆದರೆ (is_promisor_object(the_repository, &obj->oid))
			ಹಿಂತಿರುಗಿ;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_ಗಮ್ಯ_hex(&obj->oid));
		ಹಿಂತಿರುಗಿ;

	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unhandled missing_action");
		ಹಿಂತಿರುಗಿ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಮುಕ್ತಾಯ_commit(ರಚನೆ commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = ಖಾಲಿ;
	free_commit_ಬಫರ್(the_repository->parsed_objects,
			   commit);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_commit(ರಚನೆ commit *commit, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ rev_list_info *info = ದತ್ತಾಂಶ;
	ರಚನೆ rev_info *revs = info->revs;

	display_ಪ್ರಗತಿ(ಪ್ರಗತಿ, ++ಪ್ರಗತಿ_counter);

	ಆದರೆ (revs->do_not_die_on_missing_objects &&
	    oidset_ಒಳಗೊಂಡಿದೆ(&revs->missing_commits, &commit->object.oid)) {
		ಮುಕ್ತಾಯ_object_ma(&commit->object, ಖಾಲಿ);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (arg_missing_only) {
		ಮುಕ್ತಾಯ_commit(commit);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (show_disk_usage)
		ಒಟ್ಟು_disk_usage += get_object_disk_usage(&commit->object);

	ಆದರೆ (info->flags & REV_LIST_QUIET) {
		ಮುಕ್ತಾಯ_commit(commit);
		ಹಿಂತಿರುಗಿ;
	}

	ನಕ್ಷೆ_show_commit(revs->ನಕ್ಷೆ);

	ಆದರೆ (revs->ಎಣಿಕೆ_2) {
		ಆದರೆ (commit->object.flags & PATCHSAME)
			revs->ಎಣಿಕೆ_same++;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (commit->object.flags & ಸಮಮಿತ_ಎಡ)
			revs->ಎಣಿಕೆ_ಎಡ++;
		ಇಲ್ಲದಿದ್ದರೆ
			revs->ಎಣಿಕೆ_ಬಲ++;
		ಮುಕ್ತಾಯ_commit(commit);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (info->show_timestamp)
		ಬರೆ("%"PRItime" ", commit->date);
	ಆದರೆ (info->ಶೀರ್ಷಿಕೆ_ಪೂರ್ವಪ್ರತ್ಯಯ)
		fputs(info->ಶೀರ್ಷಿಕೆ_ಪೂರ್ವಪ್ರತ್ಯಯ, stdout);

	ಆದರೆ (revs->include_ಶೀರ್ಷಿಕೆ) {
		ಆದರೆ (!revs->ನಕ್ಷೆ && ಸಾಲು_ಪದ)
			fputs(get_revision_mark(revs, commit), stdout);
		ಆದರೆ (revs->abbrev_commit && revs->abbrev)
			fputs(repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		ಇಲ್ಲದಿದ್ದರೆ
			fputs(oid_ಗಮ್ಯ_hex(&commit->object.oid), stdout);

		ಆದರೆ (!ಸಾಲು_ಪದ) {
			ಆದರೆ (commit->object.flags & BOUNDARY)
				ಬರೆ("%cboundary=yes", info_ಪದ);
		}
	}
	ಆದರೆ (revs->print_parents) {
		ರಚನೆ commit_list *parents = commit->parents;
		ಆಗ (parents) {
			ಬರೆ(" %s", oid_ಗಮ್ಯ_hex(&parents->ಅಂಶ->object.oid));
			parents = parents->ಮುಂದಿನ;
		}
	}
	ಆದರೆ (revs->children.name) {
		ರಚನೆ commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		ಆಗ (children) {
			ಬರೆ(" %s", oid_ಗಮ್ಯ_hex(&children->ಅಂಶ->object.oid));
			children = children->ಮುಂದಿನ;
		}
	}
	show_decorations(revs, commit);
	ಆದರೆ (revs->commit_format == CMIT_FMT_ONELINE)
		ಅಕ್ಷರ_ಬರೆ(' ');
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (revs->include_ಶೀರ್ಷಿಕೆ)
		ಅಕ್ಷರ_ಬರೆ(ಸಾಲು_ಪದ);

	ಆದರೆ (revs->verbose_ಶೀರ್ಷಿಕೆ) {
		ರಚನೆ strbuf buf = STRBUF_INIT;
		ರಚನೆ pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_ಬಹುಲಕ = revs->date_ಬಹುಲಕ;
		ctx.date_ಬಹುಲಕ_explicit = revs->date_ಬಹುಲಕ_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		ಆದರೆ (buf.len) {
			ಆದರೆ (revs->commit_format != CMIT_FMT_ONELINE)
				ನಕ್ಷೆ_show_oneline(revs->ನಕ್ಷೆ);

			ನಕ್ಷೆ_show_commit_ಸಂದೇಶ(revs->ನಕ್ಷೆ, stdout, &buf);

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
			ಆದರೆ (buf.len && buf.buf[buf.len - 1] == '\n')
				ನಕ್ಷೆ_show_padding(revs->ನಕ್ಷೆ);
			ಅಕ್ಷರ_ಬರೆ(info->hdr_termination);
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			ಆದರೆ (ನಕ್ಷೆ_show_remainder(revs->ನಕ್ಷೆ))
				ಅಕ್ಷರ_ಬರೆ('\n');
			ಆದರೆ (revs->commit_format == CMIT_FMT_ONELINE)
				ಅಕ್ಷರ_ಬರೆ('\n');
		}
		strbuf_release(&buf);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ನಕ್ಷೆ_show_remainder(revs->ನಕ್ಷೆ))
			ಅಕ್ಷರ_ಬರೆ('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	ಮುಕ್ತಾಯ_commit(commit);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮುಕ್ತಾಯ_object(ರಚನೆ object *obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ rev_list_info *info = cb_ದತ್ತಾಂಶ;
	ಆದರೆ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, ಖಾಲಿ, 0) < 0) {
		ಮುಕ್ತಾಯ_object_ma(obj, name);
		ಹಿಂತಿರುಗಿ 1;
	}
	ಆದರೆ (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_object(ರಚನೆ object *obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ rev_list_info *info = cb_ದತ್ತಾಂಶ;
	ರಚನೆ rev_info *revs = info->revs;

	ಆದರೆ (ಮುಕ್ತಾಯ_object(obj, name, cb_ದತ್ತಾಂಶ))
		ಹಿಂತಿರುಗಿ;
	display_ಪ್ರಗತಿ(ಪ್ರಗತಿ, ++ಪ್ರಗತಿ_counter);
	ಆದರೆ (arg_missing_only)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (show_disk_usage)
		ಒಟ್ಟು_disk_usage += get_object_disk_usage(obj);
	ಆದರೆ (info->flags & REV_LIST_QUIET)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (revs->ಎಣಿಕೆ_2) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->ಎಣಿಕೆ_ಬಲ++;
		ಹಿಂತಿರುಗಿ;
	}

	ಬರೆ("%s", oid_ಗಮ್ಯ_hex(&obj->oid));

	ಆದರೆ (arg_show_object_ಹೆಸರುಗಳು) {
		ಆದರೆ (ಸಾಲು_ಪದ) {
			ಅಕ್ಷರ_ಬರೆ(info_ಪದ);
			ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p = name; *p && *p != '\n'; p++)
				ಅಕ್ಷರ_ಬರೆ(*p);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*name) {
			ಬರೆ("%cpath=%s", info_ಪದ, name);
		}
	}

	ಅಕ್ಷರ_ಬರೆ(ಸಾಲು_ಪದ);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ಅಂಚು(ರಚನೆ commit *commit)
{
	ಬರೆ("-%s\n", oid_ಗಮ್ಯ_hex(&commit->object.oid));
}

ಸ್ಥಿರ ಶೂನ್ಯ print_var_str(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *val)
{
	ಬರೆ("%s='%s'\n", var, val);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_var_int(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಪೂರ್ಣಾಂಕ val)
{
	ಬರೆ("%s=%d\n", var, val);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_bisect_vars(ರಚನೆ rev_list_info *info, ಪೂರ್ಣಾಂಕ reaches, ಪೂರ್ಣಾಂಕ all)
{
	ಪೂರ್ಣಾಂಕ cnt, flags = info->flags;
	ಅಕ್ಷರ hex[GIT_MAX_HEXSZ + 1] = "";
	ರಚನೆ commit_list *tried;
	ರಚನೆ rev_info *revs = info->revs;

	ಆದರೆ (!revs->commits)
		ಹಿಂತಿರುಗಿ 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       ಖಾಲಿ, ಖಾಲಿ);

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
	ಆದರೆ (cnt < reaches)
		cnt = reaches;

	ಆದರೆ (revs->commits)
		oid_ಗಮ್ಯ_hex_r(hex, &revs->commits->ಅಂಶ->object.oid);

	ಆದರೆ (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		ಬರೆ("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_ಹೆಜ್ಜೆಗಳು(all));

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_object_fast(
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
	ಎಣಿಕೆ object_type type ಬಳಸದ,
	ಪೂರ್ಣಾಂಕ exclude ಬಳಸದ,
	uint32_t name_hash ಬಳಸದ,
	ರಚನೆ packed_git *ಸಿಕ್ಕಿತು_pack ಬಳಸದ,
	off_t ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ ಬಳಸದ,
	ಶೂನ್ಯ *payload ಬಳಸದ)
{
	fprintf(stdout, "%s\n", oid_ಗಮ್ಯ_hex(oid));
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_disk_usage(off_t ಗಾತ್ರ_2)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಆದರೆ (human_readable)
		strbuf_humanise_ಬೈಟುಗಳು(&sb, ಗಾತ್ರ_2);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)ಗಾತ್ರ_2);
	puts(sb.buf);
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ parse_missing_action_ಮೌಲ್ಯ(ರಚನೆ repository *repo,
					     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "error")) {
		arg_missing_action = MA_ERROR;
		ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ try_bitmap_ಎಣಿಕೆ(ರಚನೆ rev_info *revs,
			    ಪೂರ್ಣಾಂಕ filter_provided_objects)
{
	uint32_t commit_ಎಣಿಕೆ = 0,
		 ಟ್ಯಾಗ್_ಎಣಿಕೆ = 0,
		 ವೃಕ್ಷ_ಎಣಿಕೆ = 0,
		 blob_ಎಣಿಕೆ = 0;
	ಪೂರ್ಣಾಂಕ max_ಎಣಿಕೆ;
	ರಚನೆ bitmap_ಸೂಚ್ಯಂಕ *bitmap_git;

	/* This function only handles counting, not general traversal. */
	ಆದರೆ (!revs->ಎಣಿಕೆ_2)
		ಹಿಂತಿರುಗಿ -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	ಆದರೆ (revs->ಎಡ_ಬಲ || revs->cherry_mark)
		ಹಿಂತಿರುಗಿ -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	ಆದರೆ (revs->max_ಎಣಿಕೆ >= 0 &&
	    (revs->ಟ್ಯಾಗ್_objects || revs->ವೃಕ್ಷ_objects || revs->blob_objects))
		ಹಿಂತಿರುಗಿ -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_ಎಣಿಕೆ = revs->max_ಎಣಿಕೆ;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ಆದರೆ (!bitmap_git)
		ಹಿಂತಿರುಗಿ -1;

	ಎಣಿಕೆ_bitmap_commit_list(bitmap_git, &commit_ಎಣಿಕೆ,
				 revs->ವೃಕ್ಷ_objects ? &ವೃಕ್ಷ_ಎಣಿಕೆ : ಖಾಲಿ,
				 revs->blob_objects ? &blob_ಎಣಿಕೆ : ಖಾಲಿ,
				 revs->ಟ್ಯಾಗ್_objects ? &ಟ್ಯಾಗ್_ಎಣಿಕೆ : ಖಾಲಿ);
	ಆದರೆ (max_ಎಣಿಕೆ >= 0 && max_ಎಣಿಕೆ < commit_ಎಣಿಕೆ)
		commit_ಎಣಿಕೆ = max_ಎಣಿಕೆ;

	ಬರೆ("%d\n", commit_ಎಣಿಕೆ + ವೃಕ್ಷ_ಎಣಿಕೆ + blob_ಎಣಿಕೆ + ಟ್ಯಾಗ್_ಎಣಿಕೆ);
	free_bitmap_ಸೂಚ್ಯಂಕ(bitmap_git);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ try_bitmap_traversal(ರಚನೆ rev_info *revs,
				ಪೂರ್ಣಾಂಕ filter_provided_objects)
{
	ರಚನೆ bitmap_ಸೂಚ್ಯಂಕ *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	ಆದರೆ (revs->max_ಎಣಿಕೆ >= 0)
		ಹಿಂತಿರುಗಿ -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	ಆದರೆ (revs->ಎಡ_ಬಲ)
		ಹಿಂತಿರುಗಿ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ಆದರೆ (!bitmap_git)
		ಹಿಂತಿರುಗಿ -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_ಸೂಚ್ಯಂಕ(bitmap_git);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ try_bitmap_disk_usage(ರಚನೆ rev_info *revs,
				 ಪೂರ್ಣಾಂಕ filter_provided_objects)
{
	ರಚನೆ bitmap_ಸೂಚ್ಯಂಕ *bitmap_git;
	off_t ಗಾತ್ರ_from_bitmap;

	ಆದರೆ (!show_disk_usage)
		ಹಿಂತಿರುಗಿ -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ಆದರೆ (!bitmap_git)
		ಹಿಂತಿರುಗಿ -1;

	ಗಾತ್ರ_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(ಗಾತ್ರ_from_bitmap);

	free_bitmap_ಸೂಚ್ಯಂಕ(bitmap_git);
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
ಸ್ಥಿರ ಶೂನ್ಯ prepare_maximal_independent(ರಚನೆ rev_info *revs)
{
	ರಚನೆ commit_list *c;

	ಆದರೆ (!revs->maximal_only)
		ಹಿಂತಿರುಗಿ;

	ಗಾಗಿ (c = revs->commits; c; c = c->ಮುಂದಿನ) {
		ಆದರೆ (c->ಅಂಶ->object.flags & UNINTERESTING)
			ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (revs->limited ||
	    revs->topo_ಕ್ರಮ ||
	    revs->ಮೊದಲ_ಪೋಷಕ_only ||
	    revs->ಹಿಮ್ಮುಖ ||
	    revs->max_ಎಣಿಕೆ >= 0 ||
	    revs->skip_ಎಣಿಕೆ >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_ದತ್ತಾಂಶ.nr ||
	    revs->ಎಣಿಕೆ_2 ||
	    revs->ಎಡ_ಬಲ ||
	    revs->boundary ||
	    revs->ಟ್ಯಾಗ್_objects ||
	    revs->ವೃಕ್ಷ_objects ||
	    revs->blob_objects ||
	    revs->filter.choice ||
	    revs->reflog_info ||
	    revs->diff ||
	    revs->grep_filter.ನಮೂನೆ_list ||
	    revs->grep_filter.ಶೀರ್ಷಿಕೆ_list ||
	    revs->verbose_ಶೀರ್ಷಿಕೆ ||
	    revs->print_parents ||
	    revs->ಅಂಚು_hint ||
	    revs->unpacked ||
	    revs->no_kept_objects ||
	    revs->ಸಾಲು_level_traverse)
		ಹಿಂತಿರುಗಿ;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

ಪೂರ್ಣಾಂಕ cmd_rev_list(ಪೂರ್ಣಾಂಕ argc,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo)
{
	ರಚನೆ rev_info revs;
	ರಚನೆ rev_list_info info;
	ರಚನೆ ಸಿದ್ಧತೆ_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ bisect_list = 0;
	ಪೂರ್ಣಾಂಕ bisect_show_vars = 0;
	ಪೂರ್ಣಾಂಕ bisect_ಕಂಡುಹಿಡಿ_all = 0;
	ಪೂರ್ಣಾಂಕ use_bitmap_ಸೂಚ್ಯಂಕ = 0;
	ಪೂರ್ಣಾಂಕ filter_provided_objects = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *show_ಪ್ರಗತಿ = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, ಖಾಲಿ);
	repo_init_revisions(the_repository, &revs, ಪೂರ್ವಪ್ರತ್ಯಯ);
	revs.abbrev = DEFAULT_ABBREV;
	revs.commit_format = CMIT_FMT_UNSPECIFIED;
	revs.include_ಶೀರ್ಷಿಕೆ = 1;

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
	ಗಾಗಿ (i = 1; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];
		ಆದರೆ (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--missing=", &arg)) {
			parse_missing_action_ಮೌಲ್ಯ(repo, arg);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "-z")) {
			ಸಾಲು_ಪದ = '\0';
			info_ಪದ = '\0';
		}
	}

	ಆದರೆ (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	ಆದರೆ (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = ಸಿದ್ಧತೆ_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, ಗಾತ್ರ(info));
	info.revs = &revs;
	ಆದರೆ (revs.bisect)
		bisect_list = 1;

	ಆದರೆ (revs.diffopt.flags.ಶೀಘ್ರ)
		info.flags |= REV_LIST_QUIET;
	ಗಾಗಿ (i = 1 ; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];

		ಆದರೆ (!strcmp(arg, "--header")) {
			revs.verbose_ಶೀರ್ಷಿಕೆ = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_ಕಂಡುಹಿಡಿ_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_ಸೂಚ್ಯಂಕ = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			ಹೋಗು cleanup;
		}
		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--progress=", &arg)) {
			show_ಪ್ರಗತಿ = arg;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (!strcmp(arg, "--exclude-promisor-objects"))
			ಮುಂದುವರಿ; /* already handled above */
		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--missing=", &arg))
			ಮುಂದುವರಿ; /* already handled above */

		ಆದರೆ (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_ಹೆಸರುಗಳು = 0;
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (!strcmp(arg, ("--object-names"))) {
			arg_show_object_ಹೆಸರುಗಳು = 1;
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (!strcmp(arg, ("--commit-header"))) {
			revs.include_ಶೀರ್ಷಿಕೆ = 1;
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_ಶೀರ್ಷಿಕೆ = 0;
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--disk-usage", &arg)) {
			ಆದರೆ (*arg == '=') {
				ಆದರೆ (!strcmp(++arg, "human")) {
					human_readable = 1;
				} ಇಲ್ಲದಿದ್ದರೆ
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (!strcmp(arg, "--missing-only"))
			ಮುಂದುವರಿ;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	ಆದರೆ (!ಸಾಲು_ಪದ) {
		ಆದರೆ (revs.ನಕ್ಷೆ || revs.verbose_ಶೀರ್ಷಿಕೆ || show_disk_usage ||
		    info.show_timestamp || info.ಶೀರ್ಷಿಕೆ_ಪೂರ್ವಪ್ರತ್ಯಯ || bisect_list ||
		    use_bitmap_ಸೂಚ್ಯಂಕ || revs.ಅಂಚು_hint || revs.ಎಡ_ಬಲ ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	ಆದರೆ (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_ಶೀರ್ಷಿಕೆ = 1;
	ಆದರೆ (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		ಆದರೆ (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_ಶೀರ್ಷಿಕೆ)
			info.ಶೀರ್ಷಿಕೆ_ಪೂರ್ವಪ್ರತ್ಯಯ = "";
		ಇಲ್ಲದಿದ್ದರೆ
			info.ಶೀರ್ಷಿಕೆ_ಪೂರ್ವಪ್ರತ್ಯಯ = "commit ";
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (revs.verbose_ಶೀರ್ಷಿಕೆ)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	ಆದರೆ ((!revs.commits && reflog_walk_ಖಾಲಿಗಳು(revs.reflog_info) &&
	     (!(revs.ಟ್ಯಾಗ್_objects || revs.ವೃಕ್ಷ_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_ಒಳಹರಿವು_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	ಆದರೆ (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	ಆದರೆ (revs.ಎಣಿಕೆ_2 &&
	    (revs.ಟ್ಯಾಗ್_objects || revs.ವೃಕ್ಷ_objects || revs.blob_objects) &&
	    (revs.ಎಡ_ಬಲ || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.ಎಣಿಕೆ_2, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_ಬಫರ್ = (revs.verbose_ಶೀರ್ಷಿಕೆ ||
			      revs.grep_filter.ನಮೂನೆ_list ||
			      revs.grep_filter.ಶೀರ್ಷಿಕೆ_list);
	ಆದರೆ (bisect_list)
		revs.limited = 1;

	ಆದರೆ (show_ಪ್ರಗತಿ)
		ಪ್ರಗತಿ = ಆರಂಭ_delayed_ಪ್ರಗತಿ(the_repository,
						  show_ಪ್ರಗತಿ, 0);

	ಆದರೆ (use_bitmap_ಸೂಚ್ಯಂಕ) {
		ಆದರೆ (!try_bitmap_ಎಣಿಕೆ(&revs, filter_provided_objects))
			ಹೋಗು cleanup;
		ಆದರೆ (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			ಹೋಗು cleanup;
		ಆದರೆ (!try_bitmap_traversal(&revs, filter_provided_objects))
			ಹೋಗು cleanup;
	}

	ಆದರೆ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	ಆದರೆ (revs.ವೃಕ್ಷ_objects)
		mark_ಅಂಚುಗಳು_uninteresting(&revs, show_ಅಂಚು, 0);

	ಆದರೆ (bisect_list) {
		ಪೂರ್ಣಾಂಕ reaches, all;
		ಚಿಹ್ನೆರಹಿತ bisect_flags = 0;

		ಆದರೆ (bisect_ಕಂಡುಹಿಡಿ_all)
			bisect_flags |= ಕಂಡುಹಿಡಿ_BISECTION_ALL;

		ಆದರೆ (revs.ಮೊದಲ_ಪೋಷಕ_only)
			bisect_flags |= ಕಂಡುಹಿಡಿ_BISECTION_ಮೊದಲ_ಪೋಷಕ_ONLY;

		ಕಂಡುಹಿಡಿ_bisection(&revs.commits, &reaches, &all, bisect_flags);

		ಆದರೆ (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			ಹೋಗು cleanup;
		}
	}

	ಆದರೆ (filter_provided_objects) {
		ರಚನೆ commit_list *c;
		ಗಾಗಿ (i = 0; i < revs.pending.nr; i++) {
			ರಚನೆ object_array_ನಮೂದು *pending = revs.pending.objects + i;
			pending->ಅಂಶ->flags |= NOT_USER_GIVEN;
		}
		ಗಾಗಿ (c = revs.commits; c; c = c->ಮುಂದಿನ)
			c->ಅಂಶ->object.flags |= NOT_USER_GIVEN;
	}

	ಆದರೆ (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_ಗಾತ್ರ);
	ಆದರೆ (should_collect_missing()) {
		ರಚನೆ oidset_iter iter;
		ರಚನೆ object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_ಗಾತ್ರ);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		ಆಗ ((oid = oidset_iter_ಮುಂದಿನ(&iter)))
			ಕೂಡಿಸು_missing_object_ನಮೂದು(oid, ಖಾಲಿ, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : ಖಾಲಿ));

	ಆದರೆ (arg_print_omitted) {
		ರಚನೆ oidset_iter iter;
		ರಚನೆ object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		ಆಗ ((oid = oidset_iter_ಮುಂದಿನ(&iter)))
			ಬರೆ("~%s\n", oid_ಗಮ್ಯ_hex(oid));
		oidset_clear(&omitted_objects);
	}
	ಆದರೆ (should_collect_missing()) {
		ರಚನೆ missing_objects_map_ನಮೂದು *ನಮೂದು;
		ರಚನೆ oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		ಆಗ ((ನಮೂದು = oidmap_iter_ಮುಂದಿನ(&iter))) {
			print_missing_object(ನಮೂದು, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_ನಮೂದು_free);
	}

	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ);

	ಆದರೆ (revs.ಎಣಿಕೆ_2) {
		ಆದರೆ (revs.ಎಡ_ಬಲ && revs.cherry_mark)
			ಬರೆ("%d\t%d\t%d\n", revs.ಎಣಿಕೆ_ಎಡ, revs.ಎಣಿಕೆ_ಬಲ, revs.ಎಣಿಕೆ_same);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (revs.ಎಡ_ಬಲ)
			ಬರೆ("%d\t%d\n", revs.ಎಣಿಕೆ_ಎಡ, revs.ಎಣಿಕೆ_ಬಲ);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (revs.cherry_mark)
			ಬರೆ("%d\t%d\n", revs.ಎಣಿಕೆ_ಎಡ + revs.ಎಣಿಕೆ_ಬಲ, revs.ಎಣಿಕೆ_same);
		ಇಲ್ಲದಿದ್ದರೆ
			ಬರೆ("%d\n", revs.ಎಣಿಕೆ_ಎಡ + revs.ಎಣಿಕೆ_ಬಲ);
	}

	ಆದರೆ (show_disk_usage)
		print_disk_usage(ಒಟ್ಟು_disk_usage);

cleanup:
	release_revisions(&revs);
	ಹಿಂತಿರುಗಿ ret;
}
