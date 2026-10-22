#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "list-objects.h"
#అన్తర్భావ "list-objects-filter-options.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "pack-bitmap.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "log-tree.h"
#అన్తర్భావ "graph.h"
#అన్తర్భావ "bisect.h"
#అన్తర్భావ "progress.h"
#అన్తర్భావ "reflog-walk.h"
#అన్తర్భావ "oidset.h"
#అన్తర్భావ "oidmap.h"
#అన్తర్భావ "packfile.h"
#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "strbuf.h"

నిర్మాణం rev_list_info {
	నిర్మాణం rev_info *revs;
	పూర్ణాంకం flags;
	పూర్ణాంకం show_timestamp;
	పూర్ణాంకం hdr_termination;
	స్థిరరాశి అక్షరం *శీర్షిక_ఉపసర్గ;
};

స్థిర స్థిరరాశి అక్షరం rev_list_usage[] =
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

స్థిర నిర్మాణం పురోగతి *పురోగతి;
స్థిర గుర్తులేని పురోగతి_counter;

స్థిర నిర్మాణం oidset omitted_objects;
స్థిర పూర్ణాంకం arg_print_omitted; /* print objects omitted by filter */

నిర్మాణం missing_objects_map_నమోదు {
	నిర్మాణం oidmap_నమోదు నమోదు_2;
	అక్షరం *మార్గం;
	గుర్తులేని type;
};

స్థిర శూన్య missing_objects_map_నమోదు_free(శూన్య *e)
{
	నిర్మాణం missing_objects_map_నమోదు *నమోదు_2 =
		container_యొక్క(e, నిర్మాణం missing_objects_map_నమోదు, నమోదు_2);

	విడుదల(నమోదు_2->మార్గం);
	విడుదల(నమోదు_2);
}

స్థిర నిర్మాణం oidmap missing_objects;
గణన missing_action {
	MA_ERROR = 0,    /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,    /* silently allow ALL missing objects */
	MA_PRINT,        /* print ALL missing objects in special section */
	MA_PRINT_INFO,   /* same as MA_PRINT but also prints missing object info */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
స్థిర గణన missing_action arg_missing_action;
స్థిర పూర్ణాంకం arg_missing_only;

స్థిర అంతఃవరుస పూర్ణాంకం should_collect_missing(శూన్య)
{
	తిరిగి arg_missing_action == MA_PRINT ||
	       arg_missing_action == MA_PRINT_INFO;
}

/* display only the oid of each object encountered */
స్థిర పూర్ణాంకం arg_show_object_పేర్లు = 1;

#నిర్వచించు DEFAULT_OIDSET_కొలత     (16*1024)

స్థిర అక్షరం పంక్తి_పదం = '\n';
స్థిర అక్షరం info_పదం = ' ';

స్థిర పూర్ణాంకం show_disk_usage;
స్థిర off_t మొత్తం_disk_usage;
స్థిర పూర్ణాంకం human_readable;

స్థిర off_t get_object_disk_usage(నిర్మాణం object *obj)
{
	off_t కొలత;
	నిర్మాణం object_info oi = OBJECT_INFO_INIT;
	oi.disk_sizep = &కొలత;
	ఒకవేళ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, &oi, 0) < 0)
		die(_("unable to get disk usage of %s"), oid_గమ్యం_hex(&obj->oid));
	తిరిగి కొలత;
}

స్థిర శూన్య కూడు_missing_object_నమోదు(నిర్మాణం object_id *oid, స్థిరరాశి అక్షరం *మార్గం,
				     గుర్తులేని type)
{
	నిర్మాణం missing_objects_map_నమోదు *నమోదు_2;

	ఒకవేళ (oidmap_get(&missing_objects, oid))
		తిరిగి;

	CALLOC_ARRAY(నమోదు_2, 1);
	నమోదు_2->నమోదు_2.oid = *oid;
	నమోదు_2->type = type;
	ఒకవేళ (మార్గం)
		నమోదు_2->మార్గం = xstrdup(మార్గం);
	oidmap_put(&missing_objects, నమోదు_2);
}

స్థిర శూన్య print_missing_object(నిర్మాణం missing_objects_map_నమోదు *నమోదు_2,
				 పూర్ణాంకం print_missing_info)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;

	/*
	 * --missing-only filters present objects out of the walk output.
	 * It still uses the selected --missing= format for missing ones,
	 * except the human "?" prefix is omitted (script-friendly OIDs).
	 */
	ఒకవేళ (arg_missing_only && పంక్తి_పదం)
		రాయి("%s", oid_గమ్యం_hex(&నమోదు_2->నమోదు_2.oid));
	లేకపోతే ఒకవేళ (పంక్తి_పదం)
		రాయి("?%s", oid_గమ్యం_hex(&నమోదు_2->నమోదు_2.oid));
	లేకపోతే
		రాయి("%s%cmissing=yes", oid_గమ్యం_hex(&నమోదు_2->నమోదు_2.oid),
		       info_పదం);

	ఒకవేళ (!print_missing_info) {
		అక్షరం_రాయి(పంక్తి_పదం);
		తిరిగి;
	}

	ఒకవేళ (నమోదు_2->మార్గం && *నమోదు_2->మార్గం) {
		strbuf_addf(&sb, "%cpath=", info_పదం);

		ఒకవేళ (పంక్తి_పదం) {
			నిర్మాణం strbuf మార్గం = STRBUF_INIT;

			quote_మార్గం(నమోదు_2->మార్గం, ఖాళీ, &మార్గం, QUOTE_మార్గం_QUOTE_SP);
			strbuf_addbuf(&sb, &మార్గం);

			strbuf_release(&మార్గం);
		} లేకపోతే {
			strbuf_addstr(&sb, నమోదు_2->మార్గం);
		}
	}
	ఒకవేళ (నమోదు_2->type)
		strbuf_addf(&sb, "%ctype=%s", info_పదం, type_name(నమోదు_2->type));

	fwrite(sb.buf, పరిమాణం(అక్షరం), sb.len, stdout);
	అక్షరం_రాయి(పంక్తి_పదం);

	strbuf_release(&sb);
}

స్థిర అంతఃవరుస శూన్య ముగింపు_object_ma(నిర్మాణం object *obj, స్థిరరాశి అక్షరం *name)
{
	/*
	 * Whether or not we try to dynamically fetch missing objects
	 * from the server, we currently DO NOT have the object.  We
	 * can either print, allow (ignore), or conditionally allow
	 * (ignore) them.
	 */
	ఎంపిక (arg_missing_action) {
	స్థితి MA_ERROR:
		die("missing %s object '%s'",
		    type_name(obj->type), oid_గమ్యం_hex(&obj->oid));
		తిరిగి;

	స్థితి MA_ALLOW_ANY:
		తిరిగి;

	స్థితి MA_PRINT:
	స్థితి MA_PRINT_INFO:
		కూడు_missing_object_నమోదు(&obj->oid, name, obj->type);
		తిరిగి;

	స్థితి MA_ALLOW_PROMISOR:
		ఒకవేళ (is_promisor_object(the_repository, &obj->oid))
			తిరిగి;
		die("unexpected missing %s object '%s'",
		    type_name(obj->type), oid_గమ్యం_hex(&obj->oid));
		తిరిగి;

	అప్రమేయ:
		BUG("unhandled missing_action");
		తిరిగి;
	}
}

స్థిర శూన్య ముగింపు_commit(నిర్మాణం commit *commit)
{
	commit_list_free(commit->parents);
	commit->parents = ఖాళీ;
	free_commit_బఫరు(the_repository->parsed_objects,
			   commit);
}

స్థిర శూన్య show_commit(నిర్మాణం commit *commit, శూన్య *దత్తాంశం)
{
	నిర్మాణం rev_list_info *info = దత్తాంశం;
	నిర్మాణం rev_info *revs = info->revs;

	display_పురోగతి(పురోగతి, ++పురోగతి_counter);

	ఒకవేళ (revs->do_not_die_on_missing_objects &&
	    oidset_కలిగియుంది(&revs->missing_commits, &commit->object.oid)) {
		ముగింపు_object_ma(&commit->object, ఖాళీ);
		తిరిగి;
	}

	ఒకవేళ (arg_missing_only) {
		ముగింపు_commit(commit);
		తిరిగి;
	}

	ఒకవేళ (show_disk_usage)
		మొత్తం_disk_usage += get_object_disk_usage(&commit->object);

	ఒకవేళ (info->flags & REV_LIST_QUIET) {
		ముగింపు_commit(commit);
		తిరిగి;
	}

	లేఖాచిత్రం_show_commit(revs->లేఖాచిత్రం);

	ఒకవేళ (revs->లెక్క) {
		ఒకవేళ (commit->object.flags & PATCHSAME)
			revs->లెక్క_same++;
		లేకపోతే ఒకవేళ (commit->object.flags & సౌష్ఠవ_ఎడమ)
			revs->లెక్క_ఎడమ++;
		లేకపోతే
			revs->లెక్క_కుడి++;
		ముగింపు_commit(commit);
		తిరిగి;
	}

	ఒకవేళ (info->show_timestamp)
		రాయి("%"PRItime" ", commit->date);
	ఒకవేళ (info->శీర్షిక_ఉపసర్గ)
		fputs(info->శీర్షిక_ఉపసర్గ, stdout);

	ఒకవేళ (revs->include_శీర్షిక) {
		ఒకవేళ (!revs->లేఖాచిత్రం && పంక్తి_పదం)
			fputs(get_revision_mark(revs, commit), stdout);
		ఒకవేళ (revs->abbrev_commit && revs->abbrev)
			fputs(repo_కనుగొను_unique_abbrev(the_repository, &commit->object.oid, revs->abbrev),
			      stdout);
		లేకపోతే
			fputs(oid_గమ్యం_hex(&commit->object.oid), stdout);

		ఒకవేళ (!పంక్తి_పదం) {
			ఒకవేళ (commit->object.flags & BOUNDARY)
				రాయి("%cboundary=yes", info_పదం);
		}
	}
	ఒకవేళ (revs->print_parents) {
		నిర్మాణం commit_list *parents = commit->parents;
		అయితే (parents) {
			రాయి(" %s", oid_గమ్యం_hex(&parents->అంశం->object.oid));
			parents = parents->తదుపరి;
		}
	}
	ఒకవేళ (revs->children.name) {
		నిర్మాణం commit_list *children;

		children = lookup_decoration(&revs->children, &commit->object);
		అయితే (children) {
			రాయి(" %s", oid_గమ్యం_hex(&children->అంశం->object.oid));
			children = children->తదుపరి;
		}
	}
	show_decorations(revs, commit);
	ఒకవేళ (revs->commit_format == CMIT_FMT_ONELINE)
		అక్షరం_రాయి(' ');
	లేకపోతే ఒకవేళ (revs->include_శీర్షిక)
		అక్షరం_రాయి(పంక్తి_పదం);

	ఒకవేళ (revs->verbose_శీర్షిక) {
		నిర్మాణం strbuf buf = STRBUF_INIT;
		నిర్మాణం pretty_print_context ctx = {0};
		ctx.abbrev = revs->abbrev;
		ctx.date_బహుళకం = revs->date_బహుళకం;
		ctx.date_బహుళకం_explicit = revs->date_బహుళకం_explicit;
		ctx.fmt = revs->commit_format;
		ctx.output_encoding = get_log_output_encoding();
		ctx.color = revs->diffopt.use_color;
		ctx.rev = revs;
		pretty_print_commit(&ctx, commit, &buf);
		ఒకవేళ (buf.len) {
			ఒకవేళ (revs->commit_format != CMIT_FMT_ONELINE)
				లేఖాచిత్రం_show_oneline(revs->లేఖాచిత్రం);

			లేఖాచిత్రం_show_commit_సందేశం(revs->లేఖాచిత్రం, stdout, &buf);

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
			ఒకవేళ (buf.len && buf.buf[buf.len - 1] == '\n')
				లేఖాచిత్రం_show_padding(revs->లేఖాచిత్రం);
			అక్షరం_రాయి(info->hdr_termination);
		} లేకపోతే {
			/*
			 * If the message buffer is empty, just show
			 * the rest of the graph output for this
			 * commit.
			 */
			ఒకవేళ (లేఖాచిత్రం_show_remainder(revs->లేఖాచిత్రం))
				అక్షరం_రాయి('\n');
			ఒకవేళ (revs->commit_format == CMIT_FMT_ONELINE)
				అక్షరం_రాయి('\n');
		}
		strbuf_release(&buf);
	} లేకపోతే {
		ఒకవేళ (లేఖాచిత్రం_show_remainder(revs->లేఖాచిత్రం))
			అక్షరం_రాయి('\n');
	}
	maybe_flush_or_die(stdout, "stdout");
	ముగింపు_commit(commit);
}

స్థిర పూర్ణాంకం ముగింపు_object(నిర్మాణం object *obj, స్థిరరాశి అక్షరం *name, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం rev_list_info *info = cb_దత్తాంశం;
	ఒకవేళ (odb_read_object_info_extended(the_repository->objects,
					  &obj->oid, ఖాళీ, 0) < 0) {
		ముగింపు_object_ma(obj, name);
		తిరిగి 1;
	}
	ఒకవేళ (info->revs->verify_objects && !obj->parsed && obj->type != OBJ_COMMIT)
		parse_object(the_repository, &obj->oid);
	తిరిగి 0;
}

స్థిర శూన్య show_object(నిర్మాణం object *obj, స్థిరరాశి అక్షరం *name, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం rev_list_info *info = cb_దత్తాంశం;
	నిర్మాణం rev_info *revs = info->revs;

	ఒకవేళ (ముగింపు_object(obj, name, cb_దత్తాంశం))
		తిరిగి;
	display_పురోగతి(పురోగతి, ++పురోగతి_counter);
	ఒకవేళ (arg_missing_only)
		తిరిగి;
	ఒకవేళ (show_disk_usage)
		మొత్తం_disk_usage += get_object_disk_usage(obj);
	ఒకవేళ (info->flags & REV_LIST_QUIET)
		తిరిగి;

	ఒకవేళ (revs->లెక్క) {
		/*
		 * The object count is always accumulated in the .count_right
		 * field for traversal that is not a left-right traversal,
		 * and cmd_rev_list() made sure that a .count request that
		 * wants to count non-commit objects, which is handled by
		 * the show_object() callback, does not ask for .left_right.
		 */
		revs->లెక్క_కుడి++;
		తిరిగి;
	}

	రాయి("%s", oid_గమ్యం_hex(&obj->oid));

	ఒకవేళ (arg_show_object_పేర్లు) {
		ఒకవేళ (పంక్తి_పదం) {
			అక్షరం_రాయి(info_పదం);
			కొరకు (స్థిరరాశి అక్షరం *p = name; *p && *p != '\n'; p++)
				అక్షరం_రాయి(*p);
		} లేకపోతే ఒకవేళ (*name) {
			రాయి("%cpath=%s", info_పదం, name);
		}
	}

	అక్షరం_రాయి(పంక్తి_పదం);
}

స్థిర శూన్య show_అంచు(నిర్మాణం commit *commit)
{
	రాయి("-%s\n", oid_గమ్యం_hex(&commit->object.oid));
}

స్థిర శూన్య print_var_str(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *val)
{
	రాయి("%s='%s'\n", var, val);
}

స్థిర శూన్య print_var_int(స్థిరరాశి అక్షరం *var, పూర్ణాంకం val)
{
	రాయి("%s=%d\n", var, val);
}

స్థిర పూర్ణాంకం show_bisect_vars(నిర్మాణం rev_list_info *info, పూర్ణాంకం reaches, పూర్ణాంకం all)
{
	పూర్ణాంకం cnt, flags = info->flags;
	అక్షరం hex[GIT_MAX_HEXSZ + 1] = "";
	నిర్మాణం commit_list *tried;
	నిర్మాణం rev_info *revs = info->revs;

	ఒకవేళ (!revs->commits)
		తిరిగి 1;

	revs->commits = filter_skipped(revs->commits, &tried,
				       flags & BISECT_SHOW_ALL,
				       ఖాళీ, ఖాళీ);

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
	ఒకవేళ (cnt < reaches)
		cnt = reaches;

	ఒకవేళ (revs->commits)
		oid_గమ్యం_hex_r(hex, &revs->commits->అంశం->object.oid);

	ఒకవేళ (flags & BISECT_SHOW_ALL) {
		traverse_commit_list(revs, show_commit, show_object, info);
		రాయి("------\n");
	}

	print_var_str("bisect_rev", hex);
	print_var_int("bisect_nr", cnt - 1);
	print_var_int("bisect_good", all - reaches - 1);
	print_var_int("bisect_bad", reaches - 1);
	print_var_int("bisect_all", all);
	print_var_int("bisect_steps", estimate_bisect_అడుగులు(all));

	తిరిగి 0;
}

స్థిర పూర్ణాంకం show_object_fast(
	స్థిరరాశి నిర్మాణం object_id *oid,
	గణన object_type type వాడని,
	పూర్ణాంకం exclude వాడని,
	uint32_t name_hash వాడని,
	నిర్మాణం packed_git *దొరికింది_pack వాడని,
	off_t దొరికింది_స్థానభ్రంశం వాడని,
	శూన్య *payload వాడని)
{
	fprintf(stdout, "%s\n", oid_గమ్యం_hex(oid));
	తిరిగి 0;
}

స్థిర శూన్య print_disk_usage(off_t కొలత)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	ఒకవేళ (human_readable)
		strbuf_humanise_బైట్లు(&sb, కొలత);
	లేకపోతే
		strbuf_addf(&sb, "%"PRIuMAX, (uintmax_t)కొలత);
	puts(sb.buf);
	strbuf_release(&sb);
}

స్థిర అంతఃవరుస పూర్ణాంకం parse_missing_action_విలువ(నిర్మాణం repository *repo,
					     స్థిరరాశి అక్షరం *విలువ)
{
	ఒకవేళ (!strcmp(విలువ, "error")) {
		arg_missing_action = MA_ERROR;
		తిరిగి 1;
	}

	ఒకవేళ (!strcmp(విలువ, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		తిరిగి 1;
	}

	ఒకవేళ (!strcmp(విలువ, "print")) {
		arg_missing_action = MA_PRINT;
		repo->fetch_if_missing = 0;
		తిరిగి 1;
	}

	ఒకవేళ (!strcmp(విలువ, "print-info")) {
		arg_missing_action = MA_PRINT_INFO;
		repo->fetch_if_missing = 0;
		తిరిగి 1;
	}

	ఒకవేళ (!strcmp(విలువ, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		తిరిగి 1;
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం try_bitmap_లెక్క(నిర్మాణం rev_info *revs,
			    పూర్ణాంకం filter_provided_objects)
{
	uint32_t commit_లెక్క = 0,
		 ట్యాగ్_లెక్క = 0,
		 వృక్షం_లెక్క = 0,
		 blob_లెక్క = 0;
	పూర్ణాంకం max_లెక్క;
	నిర్మాణం bitmap_సూచీ *bitmap_git;

	/* This function only handles counting, not general traversal. */
	ఒకవేళ (!revs->లెక్క)
		తిరిగి -1;

	/*
	 * A bitmap result can't know left/right, etc, because we don't
	 * actually traverse.
	 */
	ఒకవేళ (revs->ఎడమ_కుడి || revs->cherry_mark)
		తిరిగి -1;

	/*
	 * If we're counting reachable objects, we can't handle a max count of
	 * commits to traverse, since we don't know which objects go with which
	 * commit.
	 */
	ఒకవేళ (revs->max_లెక్క >= 0 &&
	    (revs->ట్యాగ్_objects || revs->వృక్షం_objects || revs->blob_objects))
		తిరిగి -1;

	/*
	 * This must be saved before doing any walking, since the revision
	 * machinery will count it down to zero while traversing.
	 */
	max_లెక్క = revs->max_లెక్క;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ఒకవేళ (!bitmap_git)
		తిరిగి -1;

	లెక్క_bitmap_commit_list(bitmap_git, &commit_లెక్క,
				 revs->వృక్షం_objects ? &వృక్షం_లెక్క : ఖాళీ,
				 revs->blob_objects ? &blob_లెక్క : ఖాళీ,
				 revs->ట్యాగ్_objects ? &ట్యాగ్_లెక్క : ఖాళీ);
	ఒకవేళ (max_లెక్క >= 0 && max_లెక్క < commit_లెక్క)
		commit_లెక్క = max_లెక్క;

	రాయి("%d\n", commit_లెక్క + వృక్షం_లెక్క + blob_లెక్క + ట్యాగ్_లెక్క);
	free_bitmap_సూచీ(bitmap_git);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం try_bitmap_traversal(నిర్మాణం rev_info *revs,
				పూర్ణాంకం filter_provided_objects)
{
	నిర్మాణం bitmap_సూచీ *bitmap_git;

	/*
	 * We can't use a bitmap result with a traversal limit, since the set
	 * of commits we'd get would be essentially random.
	 */
	ఒకవేళ (revs->max_లెక్క >= 0)
		తిరిగి -1;

	/*
	 * We can't know which commits were left/right in a single traversal,
	 * and we don't yet know how to traverse them separately.
	 */
	ఒకవేళ (revs->ఎడమ_కుడి)
		తిరిగి -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ఒకవేళ (!bitmap_git)
		తిరిగి -1;

	traverse_bitmap_commit_list(bitmap_git, revs, &show_object_fast);
	free_bitmap_సూచీ(bitmap_git);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం try_bitmap_disk_usage(నిర్మాణం rev_info *revs,
				 పూర్ణాంకం filter_provided_objects)
{
	నిర్మాణం bitmap_సూచీ *bitmap_git;
	off_t కొలత_from_bitmap;

	ఒకవేళ (!show_disk_usage)
		తిరిగి -1;

	bitmap_git = prepare_bitmap_walk(revs, filter_provided_objects);
	ఒకవేళ (!bitmap_git)
		తిరిగి -1;

	కొలత_from_bitmap = get_disk_usage_from_bitmap(bitmap_git, revs);
	print_disk_usage(కొలత_from_bitmap);

	free_bitmap_సూచీ(bitmap_git);
	తిరిగి 0;
}

/*
 * If revs->maximal_only is set and no other walk modifiers are provided,
 * run a faster computation to filter the independent commits and prepare
 * them for output. Set revs->no_walk to prevent later walking.
 *
 * If this algorithm doesn't apply, then no changes are made to revs.
 */
స్థిర శూన్య prepare_maximal_independent(నిర్మాణం rev_info *revs)
{
	నిర్మాణం commit_list *c;

	ఒకవేళ (!revs->maximal_only)
		తిరిగి;

	కొరకు (c = revs->commits; c; c = c->తదుపరి) {
		ఒకవేళ (c->అంశం->object.flags & UNINTERESTING)
			తిరిగి;
	}

	ఒకవేళ (revs->limited ||
	    revs->topo_క్రమం ||
	    revs->మొదటి_మాతృ_only ||
	    revs->తిరగేయి ||
	    revs->max_లెక్క >= 0 ||
	    revs->skip_లెక్క >= 0 ||
	    revs->min_age != (timestamp_t)-1 ||
	    revs->max_age != (timestamp_t)-1 ||
	    revs->min_parents > 0 ||
	    revs->max_parents >= 0 ||
	    revs->prune_దత్తాంశం.nr ||
	    revs->లెక్క ||
	    revs->ఎడమ_కుడి ||
	    revs->boundary ||
	    revs->ట్యాగ్_objects ||
	    revs->వృక్షం_objects ||
	    revs->blob_objects ||
	    revs->filter.choice ||
	    revs->reflog_info ||
	    revs->diff ||
	    revs->grep_filter.నమూనా_list ||
	    revs->grep_filter.శీర్షిక_list ||
	    revs->verbose_శీర్షిక ||
	    revs->print_parents ||
	    revs->అంచు_hint ||
	    revs->unpacked ||
	    revs->no_kept_objects ||
	    revs->పంక్తి_level_traverse)
		తిరిగి;

	reduce_heads_replace(&revs->commits);

	/* Modify 'revs' to only output this commit list. */
	revs->no_walk = 1;
}

పూర్ణాంకం cmd_rev_list(పూర్ణాంకం argc,
		 స్థిరరాశి అక్షరం **argv,
		 స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo)
{
	నిర్మాణం rev_info revs;
	నిర్మాణం rev_list_info info;
	నిర్మాణం అమరిక_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	పూర్ణాంకం i;
	పూర్ణాంకం bisect_list = 0;
	పూర్ణాంకం bisect_show_vars = 0;
	పూర్ణాంకం bisect_కనుగొను_all = 0;
	పూర్ణాంకం use_bitmap_సూచీ = 0;
	పూర్ణాంకం filter_provided_objects = 0;
	స్థిరరాశి అక్షరం *show_పురోగతి = ఖాళీ;
	పూర్ణాంకం ret = 0;

	show_usage_if_asked(argc, argv, rev_list_usage);

	repo_config(the_repository, git_default_config, ఖాళీ);
	repo_init_revisions(the_repository, &revs, ఉపసర్గ);
	revs.abbrev = DEFAULT_ABBREV;
	revs.commit_format = CMIT_FMT_UNSPECIFIED;
	revs.include_శీర్షిక = 1;

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
	కొరకు (i = 1; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];
		ఒకవేళ (!strcmp(arg, "--exclude-promisor-objects")) {
			repo->fetch_if_missing = 0;
			revs.exclude_promisor_objects = 1;
		} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(arg, "--missing=", &arg)) {
			parse_missing_action_విలువ(repo, arg);
		} లేకపోతే ఒకవేళ (!strcmp(arg, "--missing-only")) {
			arg_missing_only = 1;
		} లేకపోతే ఒకవేళ (!strcmp(arg, "-z")) {
			పంక్తి_పదం = '\0';
			info_పదం = '\0';
		}
	}

	ఒకవేళ (arg_missing_only && !should_collect_missing())
		die(_("--missing-only requires --missing=print or --missing=print-info"));

	die_for_incompatible_opt2(revs.exclude_promisor_objects,
				  "--exclude_promisor_objects",
				  arg_missing_action, "--missing");

	ఒకవేళ (arg_missing_action)
		revs.do_not_die_on_missing_objects = 1;

	argc = అమరిక_revisions(argc, argv, &revs, &s_r_opt);

	memset(&info, 0, పరిమాణం(info));
	info.revs = &revs;
	ఒకవేళ (revs.bisect)
		bisect_list = 1;

	ఒకవేళ (revs.diffopt.flags.శీఘ్ర)
		info.flags |= REV_LIST_QUIET;
	కొరకు (i = 1 ; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];

		ఒకవేళ (!strcmp(arg, "--header")) {
			revs.verbose_శీర్షిక = 1;
			కొనసాగు;
		}
		ఒకవేళ (!strcmp(arg, "--timestamp")) {
			info.show_timestamp = 1;
			కొనసాగు;
		}
		ఒకవేళ (!strcmp(arg, "--bisect")) {
			bisect_list = 1;
			కొనసాగు;
		}
		ఒకవేళ (!strcmp(arg, "--bisect-all")) {
			bisect_list = 1;
			bisect_కనుగొను_all = 1;
			info.flags |= BISECT_SHOW_ALL;
			revs.show_decorations = 1;
			కొనసాగు;
		}
		ఒకవేళ (!strcmp(arg, "--bisect-vars")) {
			bisect_list = 1;
			bisect_show_vars = 1;
			కొనసాగు;
		}
		ఒకవేళ (!strcmp(arg, "--use-bitmap-index")) {
			use_bitmap_సూచీ = 1;
			కొనసాగు;
		}
		ఒకవేళ (!strcmp(arg, "--test-bitmap")) {
			test_bitmap_walk(&revs);
			వెళ్ళు cleanup;
		}
		ఒకవేళ (skip_ఉపసర్గ(arg, "--progress=", &arg)) {
			show_పురోగతి = arg;
			కొనసాగు;
		}
		ఒకవేళ (!strcmp(arg, "--filter-provided-objects")) {
			filter_provided_objects = 1;
			కొనసాగు;
		}
		ఒకవేళ (!strcmp(arg, "--filter-print-omitted")) {
			arg_print_omitted = 1;
			కొనసాగు;
		}

		ఒకవేళ (!strcmp(arg, "--exclude-promisor-objects"))
			కొనసాగు; /* already handled above */
		ఒకవేళ (skip_ఉపసర్గ(arg, "--missing=", &arg))
			కొనసాగు; /* already handled above */

		ఒకవేళ (!strcmp(arg, ("--no-object-names"))) {
			arg_show_object_పేర్లు = 0;
			కొనసాగు;
		}

		ఒకవేళ (!strcmp(arg, ("--object-names"))) {
			arg_show_object_పేర్లు = 1;
			కొనసాగు;
		}

		ఒకవేళ (!strcmp(arg, ("--commit-header"))) {
			revs.include_శీర్షిక = 1;
			కొనసాగు;
		}

		ఒకవేళ (!strcmp(arg, ("--no-commit-header"))) {
			revs.include_శీర్షిక = 0;
			కొనసాగు;
		}

		ఒకవేళ (skip_ఉపసర్గ(arg, "--disk-usage", &arg)) {
			ఒకవేళ (*arg == '=') {
				ఒకవేళ (!strcmp(++arg, "human")) {
					human_readable = 1;
				} లేకపోతే
					die(_("invalid value for '%s': '%s', the only allowed format is '%s'"),
					    "--disk-usage=<format>", arg, "human");
			} లేకపోతే ఒకవేళ (*arg) {
				/*
				 * Arguably should goto a label to continue chain of ifs?
				 * Doesn't matter unless we try to add --disk-usage-foo
				 * afterwards.
				 */
				usage(rev_list_usage);
			}
			show_disk_usage = 1;
			info.flags |= REV_LIST_QUIET;
			కొనసాగు;
		}

		ఒకవేళ (!strcmp(arg, "--missing-only"))
			కొనసాగు;

		usage(rev_list_usage);

	}

	/*
	 * Reject options currently incompatible with -z. For some options, this
	 * is not an inherent limitation and support may be implemented in the
	 * future.
	 */
	ఒకవేళ (!పంక్తి_పదం) {
		ఒకవేళ (revs.లేఖాచిత్రం || revs.verbose_శీర్షిక || show_disk_usage ||
		    info.show_timestamp || info.శీర్షిక_ఉపసర్గ || bisect_list ||
		    use_bitmap_సూచీ || revs.అంచు_hint || revs.ఎడమ_కుడి ||
		    revs.cherry_mark)
			die(_("-z option used with unsupported option"));
	}

	ఒకవేళ (revs.commit_format != CMIT_FMT_USERFORMAT)
		revs.include_శీర్షిక = 1;
	ఒకవేళ (revs.commit_format != CMIT_FMT_UNSPECIFIED) {
		/* The command line has a --pretty  */
		info.hdr_termination = '\n';
		ఒకవేళ (revs.commit_format == CMIT_FMT_ONELINE || !revs.include_శీర్షిక)
			info.శీర్షిక_ఉపసర్గ = "";
		లేకపోతే
			info.శీర్షిక_ఉపసర్గ = "commit ";
	}
	లేకపోతే ఒకవేళ (revs.verbose_శీర్షిక)
		/* Only --header was specified */
		revs.commit_format = CMIT_FMT_RAW;

	ఒకవేళ ((!revs.commits && reflog_walk_ఖాళీలు(revs.reflog_info) &&
	     (!(revs.ట్యాగ్_objects || revs.వృక్షం_objects || revs.blob_objects) &&
	      !revs.pending.nr) &&
	     !revs.rev_ఆదానం_given && !revs.read_from_stdin) ||
	    revs.diff)
		usage(rev_list_usage);

	ఒకవేళ (revs.show_notes)
		die(_("rev-list does not support display of notes"));

	ఒకవేళ (revs.లెక్క &&
	    (revs.ట్యాగ్_objects || revs.వృక్షం_objects || revs.blob_objects) &&
	    (revs.ఎడమ_కుడి || revs.cherry_mark))
		die(_("marked counting and '%s' cannot be used together"), "--objects");

	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  revs.లెక్క, "--count");
	die_for_incompatible_opt2(arg_missing_only, "--missing-only",
				  show_disk_usage, "--disk-usage");

	save_commit_బఫరు = (revs.verbose_శీర్షిక ||
			      revs.grep_filter.నమూనా_list ||
			      revs.grep_filter.శీర్షిక_list);
	ఒకవేళ (bisect_list)
		revs.limited = 1;

	ఒకవేళ (show_పురోగతి)
		పురోగతి = ప్రారంభం_delayed_పురోగతి(the_repository,
						  show_పురోగతి, 0);

	ఒకవేళ (use_bitmap_సూచీ) {
		ఒకవేళ (!try_bitmap_లెక్క(&revs, filter_provided_objects))
			వెళ్ళు cleanup;
		ఒకవేళ (!try_bitmap_disk_usage(&revs, filter_provided_objects))
			వెళ్ళు cleanup;
		ఒకవేళ (!try_bitmap_traversal(&revs, filter_provided_objects))
			వెళ్ళు cleanup;
	}

	ఒకవేళ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	prepare_maximal_independent(&revs);

	ఒకవేళ (revs.వృక్షం_objects)
		mark_అంచులు_uninteresting(&revs, show_అంచు, 0);

	ఒకవేళ (bisect_list) {
		పూర్ణాంకం reaches, all;
		గుర్తులేని bisect_flags = 0;

		ఒకవేళ (bisect_కనుగొను_all)
			bisect_flags |= కనుగొను_BISECTION_ALL;

		ఒకవేళ (revs.మొదటి_మాతృ_only)
			bisect_flags |= కనుగొను_BISECTION_మొదటి_మాతృ_ONLY;

		కనుగొను_bisection(&revs.commits, &reaches, &all, bisect_flags);

		ఒకవేళ (bisect_show_vars) {
			ret = show_bisect_vars(&info, reaches, all);
			వెళ్ళు cleanup;
		}
	}

	ఒకవేళ (filter_provided_objects) {
		నిర్మాణం commit_list *c;
		కొరకు (i = 0; i < revs.pending.nr; i++) {
			నిర్మాణం object_array_నమోదు *pending = revs.pending.objects + i;
			pending->అంశం->flags |= NOT_USER_GIVEN;
		}
		కొరకు (c = revs.commits; c; c = c->తదుపరి)
			c->అంశం->object.flags |= NOT_USER_GIVEN;
	}

	ఒకవేళ (arg_print_omitted)
		oidset_init(&omitted_objects, DEFAULT_OIDSET_కొలత);
	ఒకవేళ (should_collect_missing()) {
		నిర్మాణం oidset_iter iter;
		నిర్మాణం object_id *oid;

		oidmap_init(&missing_objects, DEFAULT_OIDSET_కొలత);
		oidset_iter_init(&revs.missing_commits, &iter);

		/* Add missing tips */
		అయితే ((oid = oidset_iter_తదుపరి(&iter)))
			కూడు_missing_object_నమోదు(oid, ఖాళీ, 0);

		oidset_clear(&revs.missing_commits);
	}

	traverse_commit_list_filtered(
		&revs, show_commit, show_object, &info,
		(arg_print_omitted ? &omitted_objects : ఖాళీ));

	ఒకవేళ (arg_print_omitted) {
		నిర్మాణం oidset_iter iter;
		నిర్మాణం object_id *oid;
		oidset_iter_init(&omitted_objects, &iter);
		అయితే ((oid = oidset_iter_తదుపరి(&iter)))
			రాయి("~%s\n", oid_గమ్యం_hex(oid));
		oidset_clear(&omitted_objects);
	}
	ఒకవేళ (should_collect_missing()) {
		నిర్మాణం missing_objects_map_నమోదు *నమోదు_2;
		నిర్మాణం oidmap_iter iter;

		oidmap_iter_init(&missing_objects, &iter);

		అయితే ((నమోదు_2 = oidmap_iter_తదుపరి(&iter))) {
			print_missing_object(నమోదు_2, arg_missing_action ==
							    MA_PRINT_INFO);
		}

		oidmap_clear_with_free(&missing_objects, missing_objects_map_నమోదు_free);
	}

	stop_పురోగతి(&పురోగతి);

	ఒకవేళ (revs.లెక్క) {
		ఒకవేళ (revs.ఎడమ_కుడి && revs.cherry_mark)
			రాయి("%d\t%d\t%d\n", revs.లెక్క_ఎడమ, revs.లెక్క_కుడి, revs.లెక్క_same);
		లేకపోతే ఒకవేళ (revs.ఎడమ_కుడి)
			రాయి("%d\t%d\n", revs.లెక్క_ఎడమ, revs.లెక్క_కుడి);
		లేకపోతే ఒకవేళ (revs.cherry_mark)
			రాయి("%d\t%d\n", revs.లెక్క_ఎడమ + revs.లెక్క_కుడి, revs.లెక్క_same);
		లేకపోతే
			రాయి("%d\n", revs.లెక్క_ఎడమ + revs.లెక్క_కుడి);
	}

	ఒకవేళ (show_disk_usage)
		print_disk_usage(మొత్తం_disk_usage);

cleanup:
	release_revisions(&revs);
	తిరిగి ret;
}
