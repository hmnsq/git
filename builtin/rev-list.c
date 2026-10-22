#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "commit.h"
#ضمن "diff.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "revision.h"
#ضمن "list-objects.h"
#ضمن "list-objects-filter-options.h"
#ضمن "object.h"
#ضمن "object-name.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "pack-bitmap.h"
#ضمن "parse-options.h"
#ضمن "log-tree.h"
#ضمن "graph.h"
#ضمن "bisect.h"
#ضمن "progress.h"
#ضمن "reflog-walk.h"
#ضمن "oidset.h"
#ضمن "oidmap.h"
#ضمن "packfile.h"
#ضمن "commit-reach.h"
#ضمن "quote.h"
#ضمن "strbuf.h"

بنية rev_list_info {
	بنية rev_info *revs;
	صحيح flags;
	صحيح show_timestamp;
	صحيح hdr_termination;
	ثابت حرف *سرخی_اگیتر;
};

ساكن ثابت حرف rev_list_usage[] =
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

ساكن بنية ترقی *ترقی;
ساكن بنانشانی ترقی_counter;

ساكن بنية oidset omitted_objects;
ساكن صحيح arg_print_omitted; /* print objects omitted by filter */

بنية missing_objects_map_اندراج {
	بنية oidmap_اندراج اندراج;
	حرف *رستہ;
	بنانشانی type;
};

ساكن فراغ missing_objects_map_اندراج_free(فراغ *e)
{
	بنية missing_objects_map_اندراج *اندراج =
		container_دا(e, بنية missing_objects_map_اندراج, اندراج);

	حرر(اندراج->رستہ);
	حرر(اندراج);
}

ساكن بنية oidmap missing_objects;
تعداد missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ساكن تعداد missing_action arg_missing_action;
ساكن صحيح arg_missing_only;

ساكن مضمن صحيح should_collect_missing(فراغ)
{
	أرجع arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
ساكن صحيح arg_show_object_ناں = 1;

#تعریف_کرو DEFAULT_OIDSET_ناپ     (16*1024)

ساكن حرف سطر_رکن = '\n';
ساكن حرف info_رکن = ' ';

ساكن صحيح show_disk_usage;
ساكن off_t کل_disk_usage;
ساكن صحيح human_readable;

ساكن off_t get_object_disk_usage(بنية object *obj)
{
	off_t ناپ;
	بنية object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &ناپ;
	إذا (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_منزل_hex(&obj->oid));
	أرجع ناپ;
}

ساكن فراغ جوڑو_missing_object_اندراج(بنية object_id *oid, ثابت حرف *رستہ,
				     بنانشانی type)
{
	بنية missing_objects_map_اندراج *اندراج;

	إذا (oidmap_get(&missing_objects, oid))
		أرجع;

	CALLOC_ARRAY(اندراج, 1);
	اندراج->اندراج.oid = *oid;
	اندراج->type = type;
	إذا (رستہ)
		اندراج->رستہ = xstrdup(رستہ);
	oidmap_put(&missing_objects, اندراج);
}

ساكن فراغ print_missing_object(بنية missing_objects_map_اندراج *اندراج,
				 صحيح print_missing_info)
{
	بنية strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	إذا (arg_missing_only && سطر_رکن)
		لکھو("%s", oid_منزل_hex(&اندراج->اندراج.oid));
	وإلا إذا (سطر_رکن)
		لکھو("?%s", oid_منزل_hex(&اندراج->اندراج.oid));
	وإلا
		لکھو("%s%cmissing=yes", oid_منزل_hex(&اندراج->اندراج.oid),
		       info_رکن);

	إذا (!print_missing_info) {
		حرف_لکھو(سطر_رکن);
		أرجع;
	}

	إذا (اندراج->رستہ && *اندراج->رستہ) {
		strbuf_addf(&sb, "%cpath=", info_رکن);

		إذا (سطر_رکن) {
			بنية strbuf رستہ = STRBUF_INIT;

			quote_رستہ(اندراج->رستہ, عدم, &رستہ, QUOTE_رستہ_QUOTE_SP);
			strbuf_addbuf(&sb, &رستہ);

			strbuf_release(&رستہ);
		} وإلا {
			strbuf_addstr(&sb, اندراج->رستہ);
		}
	}
	إذا (اندراج->type)
		strbuf_addf(&sb, "%ctype=%s", info_رکن, type_name(اندراج->type));

	fwrite(sb.buf, حجم(حرف), sb.len, stdout);
	حرف_لکھو(سطر_رکن);

	strbuf_release(&sb);
}

ساكن مضمن فراغ انت_object_ma(بنية object *obj, ثابت حرف *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	بدل (arg_missing_action) {
	حالة MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_منزل_hex(&obj->oid));
		أرجع;

	حالة MA_ALLOW_ANY:
		أرجع;

	حالة MA_PRINT:
	حالة MA_PRINT_INFO:
		جوڑو_missing_object_اندراج(&obj->oid, name, obj->type);
		أرجع;

	حالة MA_ALLOW_PROMISOR:
		إذا (is_promisor_object(the_repository, &obj->oid))
			أرجع;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_منزل_hex(&obj->oid));
		أرجع;

	افتراضي:
		BUG("unhandled missing_action");
		أرجع;
	}
}

ساكن فراغ انت_commit(بنية commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = عدم;
	free_commit_بفر(the_repository->parsed_objects,
			   commit);
}

ساكن فراغ show_commit(بنية commit *commit, فراغ *انکڑے)
{
	بنية rev_list_info *info = انکڑے;
	بنية rev_info *revs = info->revs;

	display_ترقی(ترقی, ++ترقی_counter);

	إذا (revs->do_not_die_on_missing_objects &&
	    oidset_شامل_اے(&revs->missing_commits, &commit->object.oid)) {
		انت_object_ma(&commit->object, عدم);
		أرجع;
	}

	إذا (arg_missing_only) {
		انت_commit(commit);
		أرجع;
	}

	إذا (show_disk_usage)
		کل_disk_usage += get_object_disk_usage(&commit->object);

	إذا (info->flags & REV_LIST_QUIET) {
		انت_commit(commit);
		أرجع;
	}

	گراف_show_commit(revs->گراف);

	إذا (revs->گنتی) {
		إذا (commit->object.flags & PATCHSAME)
			revs->گنتی_same++;
		وإلا إذا (commit->object.flags & متشاکل_بایاں)
			revs->گنتی_بایاں++;
		وإلا
			revs->گنتی_دایاں++;
		انت_commit(commit);
		أرجع;
	}

	إذا (info->show_timestamp)
		لکھو("%"PRItime" ", commit->date);
	إذا (info->سرخی_اگیتر)
		fputs(info->سرخی_اگیتر, stdout);

	إذا (revs->include_سرخی) {
		إذا (!revs->گراف && سطر_رکن)
			fputs(get_revision_mark(revs, commit), stdout);
		إذا (revs->abbrev_commit && revs->abbrev)
			fputs(repo_لبھو_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		وإلا
			fputs(oid_منزل_hex(&commit->object.oid), stdout);

		إذا (!سطر_رکن) {
			إذا (commit->object.flags & BOUNDARY)
				لکھو("%cboundary=yes", info_رکن);
		}
	}
	إذا (revs->print_parents) {
		بنية commit_list *parents = commit->parents;
		طالما (parents) {
			لکھو(" %s", oid_منزل_hex(&parents->شے->object.oid));
			parents = parents->اگلا;
		}
	}
	إذا (revs->children.name) {
		بنية commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		طالما (children) {
			لکھو(" %s", oid_منزل_hex(&children->شے->object.oid));
			children = children->اگلا;
		}
	}
	show_decorations(revs, commit);
	إذا (revs->commit_format == CMIT_FMT_ONELINE)
		حرف_لکھو(' ');
	وإلا إذا (revs->include_سرخی)
		حرف_لکھو(سطر_رکن);

	إذا (revs->verbose_سرخی) {
		بنية strbuf buf = STRBUF_INIT;
		بنية pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_منوال = revs->date_منوال;
		ctx.date_منوال_explicit = revs->date_منوال_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		إذا (buf.len) {
			إذا (revs->commit_format != CMIT_FMT_ONELINE)
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
			إذا (buf.len && buf.buf[buf.len - 1] == '\n')
				گراف_show_padding(revs->گراف);
			حرف_لکھو(info->hdr_termination);
		} وإلا {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			إذا (گراف_show_remainder(revs->گراف))
				حرف_لکھو('\n');
			إذا (revs->commit_format == CMIT_FMT_ONELINE)
				حرف_لکھو('\n');
		}
		strbuf_release(&buf);
	} وإلا {
		إذا (گراف_show_remainder(revs->گراف))
			حرف_لکھو('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	انت_commit(commit);
}

ساكن صحيح انت_object(بنية object *obj, ثابت حرف *name, فراغ *cb_انکڑے)
{
	بنية rev_list_info *info = cb_انکڑے;
	إذا (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, عدم, 0) < 0) {
		انت_object_ma(obj, name);
		أرجع 1;
	}
	إذا (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	أرجع 0;
}

ساكن فراغ show_object(بنية object *obj, ثابت حرف *name, فراغ *cb_انکڑے)
{
	بنية rev_list_info *info = cb_انکڑے;
	بنية rev_info *revs = info->revs;

	إذا (انت_object(obj, name, cb_انکڑے))
		أرجع;
	display_ترقی(ترقی, ++ترقی_counter);
	إذا (arg_missing_only)
		أرجع;
	إذا (show_disk_usage)
		کل_disk_usage += get_object_disk_usage(obj);
	إذا (info->flags & REV_LIST_QUIET)
		أرجع;

	إذا (revs->گنتی) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->گنتی_دایاں++;
		أرجع;
	}

	لکھو("%s", oid_منزل_hex(&obj->oid));

	إذا (arg_show_object_ناں) {
		إذا (سطر_رکن) {
			حرف_لکھو(info_رکن);
			لكل (ثابت حرف *p = name; *p && *p != '\n'; p++)
				حرف_لکھو(*p);
		} وإلا إذا (*name) {
			لکھو("%cpath=%s", info_رکن, name);
		}
	}

	حرف_لکھو(سطر_رکن);
}

ساكن فراغ show_کنارا(بنية commit *commit)
{
	لکھو("-%s\n", oid_منزل_hex(&commit->object.oid));
}

ساكن فراغ print_var_str(ثابت حرف *var, ثابت حرف *val)
{
	لکھو("%s='%s'\n", var, val);
}

ساكن فراغ print_var_int(ثابت حرف *var, صحيح val)
{
	لکھو("%s=%d\n", var, val);
}

ساكن صحيح show_bisect_vars(بنية rev_list_info *info, صحيح reaches, صحيح all)
{
	صحيح cnt, flags = info->flags;
	حرف hex[GIT_MAX_HEXSZ + 1] = "";
	بنية commit_list *tried;
	بنية rev_info *revs = info->revs;

	إذا (!revs->commits)
		أرجع 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       عدم, عدم);

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
	إذا (cnt < reaches)
		cnt = reaches;

	إذا (revs->commits)
		oid_منزل_hex_r(hex, &revs->commits->شے->object.oid);

	إذا (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		لکھو("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_قدماں(all));

	أرجع 0;
}

ساكن صحيح show_object_fast(
	ثابت بنية object_id *oid,
	تعداد object_type type بن_ورتیا,
	صحيح exclude بن_ورتیا,
	uint32_t name_hash بن_ورتیا,
	بنية packed_git *لبھیا_pack بن_ورتیا,
	off_t لبھیا_وتھ بن_ورتیا,
	فراغ *payload بن_ورتیا)
{
	fprintf(stdout, "%s\n", oid_منزل_hex(oid));
	أرجع 0;
}

ساكن فراغ print_disk_usage(off_t ناپ)
{
	بنية strbuf sb = STRBUF_INIT;
	إذا (human_readable)
		strbuf_humanise_بائٹاں(&sb, ناپ);
	وإلا
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)ناپ);
	puts(sb.buf);
	strbuf_release(&sb);
}

ساكن مضمن صحيح parse_missing_action_قدر(بنية repository *repo,
					     ثابت حرف *قدر)
{
	إذا (!strcmp(قدر, "error")) {
		arg_missing_action = MA_ERROR;
		أرجع 1;
	}

	إذا (!strcmp(قدر, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		أرجع 1;
	}

	إذا (!strcmp(قدر, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		أرجع 1;
	}

	إذا (!strcmp(قدر, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		أرجع 1;
	}

	إذا (!strcmp(قدر, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		أرجع 1;
	}

	أرجع 0;
}

ساكن صحيح try_bitmap_گنتی(بنية rev_info *revs,
			    صحيح filter_provided_objects)
{
	uint32_t commit_گنتی = 0,
		 ٹیگ_گنتی = 0,
		 درخت_گنتی = 0,
		 blob_گنتی = 0;
	صحيح max_گنتی;
	بنية bitmap_اشاریہ *bitmap_git;

	/* This function only handles counting, not general traversal. */
	إذا (!revs->گنتی)
		أرجع -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	إذا (revs->بایاں_دایاں || revs->cherry_mark)
		أرجع -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	إذا (revs->max_گنتی >= 0 &&
	    (revs->ٹیگ_objects || revs->درخت_objects || revs->blob_objects))
		أرجع -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_گنتی = revs->max_گنتی;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	إذا (!bitmap_git)
		أرجع -1;

	گنتی_bitmap_commit_list(bitmap_git, &commit_گنتی,
				 revs->درخت_objects ? &درخت_گنتی : عدم,
				 revs->blob_objects ? &blob_گنتی : عدم,
				 revs->ٹیگ_objects ? &ٹیگ_گنتی : عدم);
	إذا (max_گنتی >= 0 && max_گنتی < commit_گنتی)
		commit_گنتی = max_گنتی;

	لکھو("%d\n", commit_گنتی + درخت_گنتی + blob_گنتی + ٹیگ_گنتی);
	free_bitmap_اشاریہ(bitmap_git);
	أرجع 0;
}

ساكن صحيح try_bitmap_traversal(بنية rev_info *revs,
				صحيح filter_provided_objects)
{
	بنية bitmap_اشاریہ *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	إذا (revs->max_گنتی >= 0)
		أرجع -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	إذا (revs->بایاں_دایاں)
		أرجع -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	إذا (!bitmap_git)
		أرجع -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_اشاریہ(bitmap_git);
	أرجع 0;
}

ساكن صحيح try_bitmap_disk_usage(بنية rev_info *revs,
				 صحيح filter_provided_objects)
{
	بنية bitmap_اشاریہ *bitmap_git;
	off_t ناپ_from_bitmap;

	إذا (!show_disk_usage)
		أرجع -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	إذا (!bitmap_git)
		أرجع -1;

	ناپ_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(ناپ_from_bitmap);

	free_bitmap_اشاریہ(bitmap_git);
	أرجع 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
ساكن فراغ prepare_maximal_independent(بنية rev_info *revs)
{
	بنية commit_list *c;

	إذا (!revs->maximal_only)
		أرجع;

	لكل (c = revs->commits; c; c = c->اگلا) {
		إذا (c->شے->object.flags & UNINTERESTING)
			أرجع;
	}

	إذا (revs->limited ||
	    revs->topo_ترتیب ||
	    revs->پہلا_ماپا_only ||
	    revs->الٹ ||
	    revs->max_گنتی >= 0 ||
	    revs->skip_گنتی >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_انکڑے.nr ||
	    revs->گنتی ||
	    revs->بایاں_دایاں ||
	    revs->boundary ||
	    revs->ٹیگ_objects ||
	    revs->درخت_objects ||
	    revs->blob_objects ||
	    revs->filter.choice ||
	    revs->reflog_info ||
	    revs->diff ||
	    revs->grep_filter.نمونہ_list ||
	    revs->grep_filter.سرخی_list ||
	    revs->verbose_سرخی ||
	    revs->print_parents ||
	    revs->کنارا_hint ||
	    revs->unpacked ||
	    revs->no_kept_objects ||
	    revs->سطر_level_traverse)
		أرجع;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

صحيح cmd_rev_list(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *اگیتر,
		 بنية repository *repo)
{
	بنية rev_info revs;
	بنية rev_list_info info;
	بنية تیاری_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	صحيح i;
	صحيح bisect_list = 0;
	صحيح bisect_show_vars = 0;
	صحيح bisect_لبھو_all = 0;
	صحيح use_bitmap_اشاریہ = 0;
	صحيح filter_provided_objects = 0;
	ثابت حرف *show_ترقی = عدم;
	صحيح ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, عدم);
	repo_init_revisions(the_repository, &revs, اگیتر);
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
	لكل (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		إذا (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} وإلا إذا (skip_اگیتر(arg, "--missing=", &arg)) {
			parse_missing_action_قدر(repo, arg);
		} وإلا إذا (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} وإلا إذا (!strcmp(arg, "-z")) {
			سطر_رکن = '\0';
			info_رکن = '\0';
		}
	}

	إذا (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	إذا (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = تیاری_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, حجم(info));
	info.revs = &revs;
	إذا (revs.bisect)
		bisect_list = 1;

	إذا (revs.diffopt.flags.تیز)
		info.flags |= REV_LIST_QUIET;
	لكل (i = 1 ; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		إذا (!strcmp(arg, "--header")) {
			revs.verbose_سرخی = 1;
			استمر;
		}
		إذا (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			استمر;
		}
		إذا (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			استمر;
		}
		إذا (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_لبھو_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			استمر;
		}
		إذا (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			استمر;
		}
		إذا (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_اشاریہ = 1;
			استمر;
		}
		إذا (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			اذهب cleanup;
		}
		إذا (skip_اگیتر(arg, "--progress=", &arg)) {
			show_ترقی = arg;
			استمر;
		}
		إذا (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			استمر;
		}
		إذا (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			استمر;
		}

		إذا (!strcmp(arg, "--exclude-promisor-objects"))
			استمر; /* already handled above */
		إذا (skip_اگیتر(arg, "--missing=", &arg))
			استمر; /* already handled above */

		إذا (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_ناں = 0;
			استمر;
		}

		إذا (!strcmp(arg, ("--object-names"))) {
			arg_show_object_ناں = 1;
			استمر;
		}

		إذا (!strcmp(arg, ("--commit-header"))) {
			revs.include_سرخی = 1;
			استمر;
		}

		إذا (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_سرخی = 0;
			استمر;
		}

		إذا (skip_اگیتر(arg, "--disk-usage", &arg)) {
			إذا (*arg == '=') {
				إذا (!strcmp(++arg, "human")) {
					human_readable = 1;
				} وإلا
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} وإلا إذا (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			استمر;
		}

		إذا (!strcmp(arg, "--missing-only"))
			استمر;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	إذا (!سطر_رکن) {
		إذا (revs.گراف || revs.verbose_سرخی || show_disk_usage ||
		    info.show_timestamp || info.سرخی_اگیتر || bisect_list ||
		    use_bitmap_اشاریہ || revs.کنارا_hint || revs.بایاں_دایاں ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	إذا (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_سرخی = 1;
	إذا (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		إذا (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_سرخی)
			info.سرخی_اگیتر = "";
		وإلا
			info.سرخی_اگیتر = "commit ";
	}
	وإلا إذا (revs.verbose_سرخی)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	إذا ((!revs.commits && reflog_walk_خالی_تھاواں(revs.reflog_info) &&
	     (!(revs.ٹیگ_objects || revs.درخت_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_داخلہ_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	إذا (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	إذا (revs.گنتی &&
	    (revs.ٹیگ_objects || revs.درخت_objects || revs.blob_objects) &&
	    (revs.بایاں_دایاں || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.گنتی, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_بفر = (revs.verbose_سرخی ||
			      revs.grep_filter.نمونہ_list ||
			      revs.grep_filter.سرخی_list);
	إذا (bisect_list)
		revs.limited = 1;

	إذا (show_ترقی)
		ترقی = شروع_delayed_ترقی(the_repository,
						  show_ترقی, 0);

	إذا (use_bitmap_اشاریہ) {
		إذا (!try_bitmap_گنتی(&revs, filter_provided_objects))
			اذهب cleanup;
		إذا (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			اذهب cleanup;
		إذا (!try_bitmap_traversal(&revs, filter_provided_objects))
			اذهب cleanup;
	}

	إذا (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	إذا (revs.درخت_objects)
		mark_کنارے_uninteresting(&revs, show_کنارا, 0);

	إذا (bisect_list) {
		صحيح reaches, all;
		بنانشانی bisect_flags = 0;

		إذا (bisect_لبھو_all)
			bisect_flags |= لبھو_BISECTION_ALL;

		إذا (revs.پہلا_ماپا_only)
			bisect_flags |= لبھو_BISECTION_پہلا_ماپا_ONLY;

		لبھو_bisection(&revs.commits, &reaches, &all, bisect_flags);

		إذا (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			اذهب cleanup;
		}
	}

	إذا (filter_provided_objects) {
		بنية commit_list *c;
		لكل (i = 0; i < revs.pending.nr; i++) {
			بنية object_array_اندراج *pending = revs.pending.objects + i;
			pending->شے->flags |= NOT_USER_GIVEN;
		}
		لكل (c = revs.commits; c; c = c->اگلا)
			c->شے->object.flags |= NOT_USER_GIVEN;
	}

	إذا (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_ناپ);
	إذا (should_collect_missing()) {
		بنية oidset_iter iter;
		بنية object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_ناپ);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		طالما ((oid = oidset_iter_اگلا(&iter)))
			جوڑو_missing_object_اندراج(oid, عدم, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : عدم));

	إذا (arg_print_omitted) {
		بنية oidset_iter iter;
		بنية object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		طالما ((oid = oidset_iter_اگلا(&iter)))
			لکھو("~%s\n", oid_منزل_hex(oid));
		oidset_clear(&omitted_objects);
	}
	إذا (should_collect_missing()) {
		بنية missing_objects_map_اندراج *اندراج;
		بنية oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		طالما ((اندراج = oidmap_iter_اگلا(&iter))) {
			print_missing_object(اندراج, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_اندراج_free);
	}

	stop_ترقی(&ترقی);

	إذا (revs.گنتی) {
		إذا (revs.بایاں_دایاں && revs.cherry_mark)
			لکھو("%d\t%d\t%d\n", revs.گنتی_بایاں, revs.گنتی_دایاں, revs.گنتی_same);
		وإلا إذا (revs.بایاں_دایاں)
			لکھو("%d\t%d\n", revs.گنتی_بایاں, revs.گنتی_دایاں);
		وإلا إذا (revs.cherry_mark)
			لکھو("%d\t%d\n", revs.گنتی_بایاں + revs.گنتی_دایاں, revs.گنتی_same);
		وإلا
			لکھو("%d\n", revs.گنتی_بایاں + revs.گنتی_دایاں);
	}

	إذا (show_disk_usage)
		print_disk_usage(کل_disk_usage);

cleanup:
	release_revisions(&revs);
	أرجع ret;
}
