#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "commit.h"
#شامل "diff.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "revision.h"
#شامل "list-objects.h"
#شامل "list-objects-filter-options.h"
#شامل "object.h"
#شامل "object-name.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "pack-bitmap.h"
#شامل "parse-options.h"
#شامل "log-tree.h"
#شامل "graph.h"
#شامل "bisect.h"
#شامل "progress.h"
#شامل "reflog-walk.h"
#شامل "oidset.h"
#شامل "oidmap.h"
#شامل "packfile.h"
#شامل "commit-reach.h"
#شامل "quote.h"
#شامل "strbuf.h"

ساخت rev_list_info {
	ساخت rev_info *revs;
	صحیح flags;
	صحیح show_timestamp;
	صحیح hdr_termination;
	ثابت حرف *سرخی_سابقہ;
};

ساکن ثابت حرف rev_list_usage[] =
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

ساکن ساخت پیش_رفت *پیش_رفت;
ساکن غیرعلامتی پیش_رفت_شمار;

ساکن ساخت oidset omitted_objects;
ساکن صحیح arg_print_omitted; /* print objects omitted by filter */

ساخت missing_objects_map_اندراج {
	ساخت oidmap_اندراج اندراج_2;
	حرف *راستہ;
	غیرعلامتی type;
};

ساکن خالی missing_objects_map_اندراج_free(خالی *e)
{
	ساخت missing_objects_map_اندراج *اندراج_2 =
		container_کا(e, ساخت missing_objects_map_اندراج, اندراج_2);

	آزاد(اندراج_2->راستہ);
	آزاد(اندراج_2);
}

ساکن ساخت oidmap missing_objects;
شمار missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ساکن شمار missing_action arg_missing_action;
ساکن صحیح arg_missing_only;

ساکن اندرونی صحیح should_collect_missing(خالی)
{
	واپس arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
ساکن صحیح arg_show_object_نام = 1;

#تعریف_کرو DEFAULT_OIDSET_سائز     (16*1024)

ساکن حرف سطر_رکن = '\n';
ساکن حرف info_رکن = ' ';

ساکن صحیح show_disk_usage;
ساکن off_t کل_disk_usage;
ساکن صحیح human_readable;

ساکن off_t get_object_disk_usage(ساخت object *obj)
{
	off_t سائز;
	ساخت object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &سائز;
	اگر (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_مقصد_hex(&obj->oid));
	واپس سائز;
}

ساکن خالی جوڑو_missing_object_اندراج(ساخت object_id *oid, ثابت حرف *راستہ,
				     غیرعلامتی type)
{
	ساخت missing_objects_map_اندراج *اندراج_2;

	اگر (oidmap_get(&missing_objects, oid))
		واپس;

	CALLOC_ARRAY(اندراج_2, 1);
	اندراج_2->اندراج_2.oid = *oid;
	اندراج_2->type = type;
	اگر (راستہ)
		اندراج_2->راستہ = xstrdup(راستہ);
	oidmap_put(&missing_objects, اندراج_2);
}

ساکن خالی print_missing_object(ساخت missing_objects_map_اندراج *اندراج_2,
				 صحیح print_missing_info)
{
	ساخت strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	اگر (arg_missing_only && سطر_رکن)
		لکھو("%s", oid_مقصد_hex(&اندراج_2->اندراج_2.oid));
	ورنہ اگر (سطر_رکن)
		لکھو("?%s", oid_مقصد_hex(&اندراج_2->اندراج_2.oid));
	ورنہ
		لکھو("%s%cmissing=yes", oid_مقصد_hex(&اندراج_2->اندراج_2.oid),
		       info_رکن);

	اگر (!print_missing_info) {
		حرف_لکھو(سطر_رکن);
		واپس;
	}

	اگر (اندراج_2->راستہ && *اندراج_2->راستہ) {
		strbuf_addf(&sb, "%cpath=", info_رکن);

		اگر (سطر_رکن) {
			ساخت strbuf راستہ = STRBUF_INIT;

			quote_راستہ(اندراج_2->راستہ, تہی, &راستہ, QUOTE_راستہ_QUOTE_SP);
			strbuf_addbuf(&sb, &راستہ);

			strbuf_release(&راستہ);
		} ورنہ {
			strbuf_addstr(&sb, اندراج_2->راستہ);
		}
	}
	اگر (اندراج_2->type)
		strbuf_addf(&sb, "%ctype=%s", info_رکن, type_name(اندراج_2->type));

	fwrite(sb.buf, حجم(حرف), sb.len, stdout);
	حرف_لکھو(سطر_رکن);

	strbuf_release(&sb);
}

ساکن اندرونی خالی اختتام_object_ma(ساخت object *obj, ثابت حرف *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	انتخاب (arg_missing_action) {
	حالت MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_مقصد_hex(&obj->oid));
		واپس;

	حالت MA_ALLOW_ANY:
		واپس;

	حالت MA_PRINT:
	حالت MA_PRINT_INFO:
		جوڑو_missing_object_اندراج(&obj->oid, name, obj->type);
		واپس;

	حالت MA_ALLOW_PROMISOR:
		اگر (is_promisor_object(the_repository, &obj->oid))
			واپس;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_مقصد_hex(&obj->oid));
		واپس;

	طےشدہ:
		BUG("unhandled missing_action");
		واپس;
	}
}

ساکن خالی اختتام_commit(ساخت commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = تہی;
	free_commit_بفر(the_repository->parsed_objects,
			   commit);
}

ساکن خالی show_commit(ساخت commit *commit, خالی *ڈیٹا)
{
	ساخت rev_list_info *info = ڈیٹا;
	ساخت rev_info *revs = info->revs;

	display_پیش_رفت(پیش_رفت, ++پیش_رفت_شمار);

	اگر (revs->do_not_die_on_missing_objects &&
	    oidset_شامل_ہے(&revs->missing_commits, &commit->object.oid)) {
		اختتام_object_ma(&commit->object, تہی);
		واپس;
	}

	اگر (arg_missing_only) {
		اختتام_commit(commit);
		واپس;
	}

	اگر (show_disk_usage)
		کل_disk_usage += get_object_disk_usage(&commit->object);

	اگر (info->flags & REV_LIST_QUIET) {
		اختتام_commit(commit);
		واپس;
	}

	گراف_show_commit(revs->گراف);

	اگر (revs->گنتی) {
		اگر (commit->object.flags & PATCHSAME)
			revs->گنتی_same++;
		ورنہ اگر (commit->object.flags & متشاکل_بایاں)
			revs->گنتی_بایاں++;
		ورنہ
			revs->گنتی_دایاں++;
		اختتام_commit(commit);
		واپس;
	}

	اگر (info->show_timestamp)
		لکھو("%"PRItime" ", commit->date);
	اگر (info->سرخی_سابقہ)
		fputs(info->سرخی_سابقہ, stdout);

	اگر (revs->include_سرخی) {
		اگر (!revs->گراف && سطر_رکن)
			fputs(get_revision_mark(revs, commit), stdout);
		اگر (revs->abbrev_commit && revs->abbrev)
			fputs(repo_ڈھونڈو_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		ورنہ
			fputs(oid_مقصد_hex(&commit->object.oid), stdout);

		اگر (!سطر_رکن) {
			اگر (commit->object.flags & BOUNDARY)
				لکھو("%cboundary=yes", info_رکن);
		}
	}
	اگر (revs->print_parents) {
		ساخت commit_list *parents = commit->parents;
		جب (parents) {
			لکھو(" %s", oid_مقصد_hex(&parents->شے->object.oid));
			parents = parents->اگلا;
		}
	}
	اگر (revs->children.name) {
		ساخت commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		جب (children) {
			لکھو(" %s", oid_مقصد_hex(&children->شے->object.oid));
			children = children->اگلا;
		}
	}
	show_decorations(revs, commit);
	اگر (revs->commit_format == CMIT_FMT_ONELINE)
		حرف_لکھو(' ');
	ورنہ اگر (revs->include_سرخی)
		حرف_لکھو(سطر_رکن);

	اگر (revs->verbose_سرخی) {
		ساخت strbuf buf = STRBUF_INIT;
		ساخت pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_منوال = revs->date_منوال;
		ctx.date_منوال_explicit = revs->date_منوال_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		اگر (buf.len) {
			اگر (revs->commit_format != CMIT_FMT_ONELINE)
				گراف_show_oneline(revs->گراف);

			گراف_show_commit_پیغام(revs->گراف, stdout, &buf);

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
			اگر (buf.len && buf.buf[buf.len - 1] == '\n')
				گراف_show_padding(revs->گراف);
			حرف_لکھو(info->hdr_termination);
		} ورنہ {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			اگر (گراف_show_remainder(revs->گراف))
				حرف_لکھو('\n');
			اگر (revs->commit_format == CMIT_FMT_ONELINE)
				حرف_لکھو('\n');
		}
		strbuf_release(&buf);
	} ورنہ {
		اگر (گراف_show_remainder(revs->گراف))
			حرف_لکھو('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	اختتام_commit(commit);
}

ساکن صحیح اختتام_object(ساخت object *obj, ثابت حرف *name, خالی *cb_ڈیٹا)
{
	ساخت rev_list_info *info = cb_ڈیٹا;
	اگر (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, تہی, 0) < 0) {
		اختتام_object_ma(obj, name);
		واپس 1;
	}
	اگر (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	واپس 0;
}

ساکن خالی show_object(ساخت object *obj, ثابت حرف *name, خالی *cb_ڈیٹا)
{
	ساخت rev_list_info *info = cb_ڈیٹا;
	ساخت rev_info *revs = info->revs;

	اگر (اختتام_object(obj, name, cb_ڈیٹا))
		واپس;
	display_پیش_رفت(پیش_رفت, ++پیش_رفت_شمار);
	اگر (arg_missing_only)
		واپس;
	اگر (show_disk_usage)
		کل_disk_usage += get_object_disk_usage(obj);
	اگر (info->flags & REV_LIST_QUIET)
		واپس;

	اگر (revs->گنتی) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->گنتی_دایاں++;
		واپس;
	}

	لکھو("%s", oid_مقصد_hex(&obj->oid));

	اگر (arg_show_object_نام) {
		اگر (سطر_رکن) {
			حرف_لکھو(info_رکن);
			برائے (ثابت حرف *p = name; *p && *p != '\n'; p++)
				حرف_لکھو(*p);
		} ورنہ اگر (*name) {
			لکھو("%cpath=%s", info_رکن, name);
		}
	}

	حرف_لکھو(سطر_رکن);
}

ساکن خالی show_کنارہ(ساخت commit *commit)
{
	لکھو("-%s\n", oid_مقصد_hex(&commit->object.oid));
}

ساکن خالی print_var_str(ثابت حرف *var, ثابت حرف *val)
{
	لکھو("%s='%s'\n", var, val);
}

ساکن خالی print_var_int(ثابت حرف *var, صحیح val)
{
	لکھو("%s=%d\n", var, val);
}

ساکن صحیح show_bisect_vars(ساخت rev_list_info *info, صحیح reaches, صحیح all)
{
	صحیح cnt, flags = info->flags;
	حرف hex[GIT_MAX_HEXSZ + 1] = "";
	ساخت commit_list *tried;
	ساخت rev_info *revs = info->revs;

	اگر (!revs->commits)
		واپس 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       تہی, تہی);

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
	اگر (cnt < reaches)
		cnt = reaches;

	اگر (revs->commits)
		oid_مقصد_hex_r(hex, &revs->commits->شے->object.oid);

	اگر (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		لکھو("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_اقدام(all));

	واپس 0;
}

ساکن صحیح show_object_fast(
	ثابت ساخت object_id *oid,
	شمار object_type type غیر_مستعمل,
	صحیح exclude غیر_مستعمل,
	uint32_t name_hash غیر_مستعمل,
	ساخت packed_git *ملا_pack غیر_مستعمل,
	off_t ملا_انحراف غیر_مستعمل,
	خالی *payload غیر_مستعمل)
{
	fprintf(stdout, "%s\n", oid_مقصد_hex(oid));
	واپس 0;
}

ساکن خالی print_disk_usage(off_t سائز)
{
	ساخت strbuf sb = STRBUF_INIT;
	اگر (human_readable)
		strbuf_humanise_بائٹیں(&sb, سائز);
	ورنہ
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)سائز);
	puts(sb.buf);
	strbuf_release(&sb);
}

ساکن اندرونی صحیح parse_missing_action_قدر(ساخت repository *repo,
					     ثابت حرف *قدر)
{
	اگر (!strcmp(قدر, "error")) {
		arg_missing_action = MA_ERROR;
		واپس 1;
	}

	اگر (!strcmp(قدر, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		واپس 1;
	}

	اگر (!strcmp(قدر, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		واپس 1;
	}

	اگر (!strcmp(قدر, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		واپس 1;
	}

	اگر (!strcmp(قدر, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		واپس 1;
	}

	واپس 0;
}

ساکن صحیح try_bitmap_گنتی(ساخت rev_info *revs,
			    صحیح filter_provided_objects)
{
	uint32_t commit_گنتی = 0,
		 لیبل_گنتی = 0,
		 درخت_گنتی = 0,
		 blob_گنتی = 0;
	صحیح max_گنتی;
	ساخت bitmap_اشاریہ *bitmap_git;

	/* This function only handles counting, not general traversal. */
	اگر (!revs->گنتی)
		واپس -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	اگر (revs->بایاں_دایاں || revs->cherry_mark)
		واپس -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	اگر (revs->max_گنتی >= 0 &&
	    (revs->لیبل_objects || revs->درخت_objects || revs->blob_objects))
		واپس -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_گنتی = revs->max_گنتی;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	اگر (!bitmap_git)
		واپس -1;

	گنتی_bitmap_commit_list(bitmap_git, &commit_گنتی,
				 revs->درخت_objects ? &درخت_گنتی : تہی,
				 revs->blob_objects ? &blob_گنتی : تہی,
				 revs->لیبل_objects ? &لیبل_گنتی : تہی);
	اگر (max_گنتی >= 0 && max_گنتی < commit_گنتی)
		commit_گنتی = max_گنتی;

	لکھو("%d\n", commit_گنتی + درخت_گنتی + blob_گنتی + لیبل_گنتی);
	free_bitmap_اشاریہ(bitmap_git);
	واپس 0;
}

ساکن صحیح try_bitmap_traversal(ساخت rev_info *revs,
				صحیح filter_provided_objects)
{
	ساخت bitmap_اشاریہ *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	اگر (revs->max_گنتی >= 0)
		واپس -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	اگر (revs->بایاں_دایاں)
		واپس -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	اگر (!bitmap_git)
		واپس -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_اشاریہ(bitmap_git);
	واپس 0;
}

ساکن صحیح try_bitmap_disk_usage(ساخت rev_info *revs,
				 صحیح filter_provided_objects)
{
	ساخت bitmap_اشاریہ *bitmap_git;
	off_t سائز_from_bitmap;

	اگر (!show_disk_usage)
		واپس -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	اگر (!bitmap_git)
		واپس -1;

	سائز_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(سائز_from_bitmap);

	free_bitmap_اشاریہ(bitmap_git);
	واپس 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
ساکن خالی prepare_maximal_independent(ساخت rev_info *revs)
{
	ساخت commit_list *c;

	اگر (!revs->maximal_only)
		واپس;

	برائے (c = revs->commits; c; c = c->اگلا) {
		اگر (c->شے->object.flags & UNINTERESTING)
			واپس;
	}

	اگر (revs->limited ||
	    revs->topo_ترتیب ||
	    revs->پہلا_والد_only ||
	    revs->الٹ ||
	    revs->max_گنتی >= 0 ||
	    revs->skip_گنتی >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_ڈیٹا.nr ||
	    revs->گنتی ||
	    revs->بایاں_دایاں ||
	    revs->boundary ||
	    revs->لیبل_objects ||
	    revs->درخت_objects ||
	    revs->blob_objects ||
	    revs->filter.choice ||
	    revs->reflog_info ||
	    revs->diff ||
	    revs->grep_filter.نمونہ_list ||
	    revs->grep_filter.سرخی_list ||
	    revs->verbose_سرخی ||
	    revs->print_parents ||
	    revs->کنارہ_hint ||
	    revs->unpacked ||
	    revs->no_kept_objects ||
	    revs->سطر_level_traverse)
		واپس;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

صحیح cmd_rev_list(صحیح argc,
		 ثابت حرف **argv,
		 ثابت حرف *سابقہ,
		 ساخت repository *repo)
{
	ساخت rev_info revs;
	ساخت rev_list_info info;
	ساخت تیاری_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	صحیح i;
	صحیح bisect_list = 0;
	صحیح bisect_show_vars = 0;
	صحیح bisect_ڈھونڈو_all = 0;
	صحیح use_bitmap_اشاریہ = 0;
	صحیح filter_provided_objects = 0;
	ثابت حرف *show_پیش_رفت = تہی;
	صحیح ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, تہی);
	repo_init_revisions(the_repository, &revs, سابقہ);
	revs.abbrev = DEFAULT_ABBREV;
	revs.commit_format = CMIT_FMT_UNSPECIFIED;
	revs.include_سرخی = 1;

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
	برائے (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		اگر (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} ورنہ اگر (skip_سابقہ(arg, "--missing=", &arg)) {
			parse_missing_action_قدر(repo, arg);
		} ورنہ اگر (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} ورنہ اگر (!strcmp(arg, "-z")) {
			سطر_رکن = '\0';
			info_رکن = '\0';
		}
	}

	اگر (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	اگر (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = تیاری_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, حجم(info));
	info.revs = &revs;
	اگر (revs.bisect)
		bisect_list = 1;

	اگر (revs.diffopt.flags.تیز)
		info.flags |= REV_LIST_QUIET;
	برائے (i = 1 ; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		اگر (!strcmp(arg, "--header")) {
			revs.verbose_سرخی = 1;
			جاری;
		}
		اگر (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			جاری;
		}
		اگر (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			جاری;
		}
		اگر (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_ڈھونڈو_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			جاری;
		}
		اگر (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			جاری;
		}
		اگر (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_اشاریہ = 1;
			جاری;
		}
		اگر (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			جاؤ cleanup;
		}
		اگر (skip_سابقہ(arg, "--progress=", &arg)) {
			show_پیش_رفت = arg;
			جاری;
		}
		اگر (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			جاری;
		}
		اگر (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			جاری;
		}

		اگر (!strcmp(arg, "--exclude-promisor-objects"))
			جاری; /* already handled above */
		اگر (skip_سابقہ(arg, "--missing=", &arg))
			جاری; /* already handled above */

		اگر (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_نام = 0;
			جاری;
		}

		اگر (!strcmp(arg, ("--object-names"))) {
			arg_show_object_نام = 1;
			جاری;
		}

		اگر (!strcmp(arg, ("--commit-header"))) {
			revs.include_سرخی = 1;
			جاری;
		}

		اگر (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_سرخی = 0;
			جاری;
		}

		اگر (skip_سابقہ(arg, "--disk-usage", &arg)) {
			اگر (*arg == '=') {
				اگر (!strcmp(++arg, "human")) {
					human_readable = 1;
				} ورنہ
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} ورنہ اگر (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			جاری;
		}

		اگر (!strcmp(arg, "--missing-only"))
			جاری;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	اگر (!سطر_رکن) {
		اگر (revs.گراف || revs.verbose_سرخی || show_disk_usage ||
		    info.show_timestamp || info.سرخی_سابقہ || bisect_list ||
		    use_bitmap_اشاریہ || revs.کنارہ_hint || revs.بایاں_دایاں ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	اگر (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_سرخی = 1;
	اگر (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		اگر (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_سرخی)
			info.سرخی_سابقہ = "";
		ورنہ
			info.سرخی_سابقہ = "commit ";
	}
	ورنہ اگر (revs.verbose_سرخی)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	اگر ((!revs.commits && reflog_walk_خالی_خانے(revs.reflog_info) &&
	     (!(revs.لیبل_objects || revs.درخت_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_داخلہ_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	اگر (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	اگر (revs.گنتی &&
	    (revs.لیبل_objects || revs.درخت_objects || revs.blob_objects) &&
	    (revs.بایاں_دایاں || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.گنتی, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_بفر = (revs.verbose_سرخی ||
			      revs.grep_filter.نمونہ_list ||
			      revs.grep_filter.سرخی_list);
	اگر (bisect_list)
		revs.limited = 1;

	اگر (show_پیش_رفت)
		پیش_رفت = آغاز_delayed_پیش_رفت(the_repository,
						  show_پیش_رفت, 0);

	اگر (use_bitmap_اشاریہ) {
		اگر (!try_bitmap_گنتی(&revs, filter_provided_objects))
			جاؤ cleanup;
		اگر (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			جاؤ cleanup;
		اگر (!try_bitmap_traversal(&revs, filter_provided_objects))
			جاؤ cleanup;
	}

	اگر (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	اگر (revs.درخت_objects)
		mark_کنارے_uninteresting(&revs, show_کنارہ, 0);

	اگر (bisect_list) {
		صحیح reaches, all;
		غیرعلامتی bisect_flags = 0;

		اگر (bisect_ڈھونڈو_all)
			bisect_flags |= ڈھونڈو_BISECTION_ALL;

		اگر (revs.پہلا_والد_only)
			bisect_flags |= ڈھونڈو_BISECTION_پہلا_والد_ONLY;

		ڈھونڈو_bisection(&revs.commits, &reaches, &all, bisect_flags);

		اگر (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			جاؤ cleanup;
		}
	}

	اگر (filter_provided_objects) {
		ساخت commit_list *c;
		برائے (i = 0; i < revs.pending.nr; i++) {
			ساخت object_array_اندراج *pending = revs.pending.objects + i;
			pending->شے->flags |= NOT_USER_GIVEN;
		}
		برائے (c = revs.commits; c; c = c->اگلا)
			c->شے->object.flags |= NOT_USER_GIVEN;
	}

	اگر (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_سائز);
	اگر (should_collect_missing()) {
		ساخت oidset_iter iter;
		ساخت object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_سائز);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		جب ((oid = oidset_iter_اگلا(&iter)))
			جوڑو_missing_object_اندراج(oid, تہی, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : تہی));

	اگر (arg_print_omitted) {
		ساخت oidset_iter iter;
		ساخت object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		جب ((oid = oidset_iter_اگلا(&iter)))
			لکھو("~%s\n", oid_مقصد_hex(oid));
		oidset_clear(&omitted_objects);
	}
	اگر (should_collect_missing()) {
		ساخت missing_objects_map_اندراج *اندراج_2;
		ساخت oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		جب ((اندراج_2 = oidmap_iter_اگلا(&iter))) {
			print_missing_object(اندراج_2, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_اندراج_free);
	}

	stop_پیش_رفت(&پیش_رفت);

	اگر (revs.گنتی) {
		اگر (revs.بایاں_دایاں && revs.cherry_mark)
			لکھو("%d\t%d\t%d\n", revs.گنتی_بایاں, revs.گنتی_دایاں, revs.گنتی_same);
		ورنہ اگر (revs.بایاں_دایاں)
			لکھو("%d\t%d\n", revs.گنتی_بایاں, revs.گنتی_دایاں);
		ورنہ اگر (revs.cherry_mark)
			لکھو("%d\t%d\n", revs.گنتی_بایاں + revs.گنتی_دایاں, revs.گنتی_same);
		ورنہ
			لکھو("%d\n", revs.گنتی_بایاں + revs.گنتی_دایاں);
	}

	اگر (show_disk_usage)
		print_disk_usage(کل_disk_usage);

cleanup:
	release_revisions(&revs);
	واپس ret;
}
