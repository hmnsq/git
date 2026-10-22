#شامل "builtin.h"
#شامل "config.h"
#شامل "diff.h"
#شامل "diffcore.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "hex.h"
#شامل "object.h"
#شامل "parse-options.h"
#شامل "revision.h"
#شامل "strbuf.h"

ایستا unsigned parse_mode_or_die(ثابت نویسه *mode, ثابت نویسه **end)
{
	uint16_t ret;

	*end = parse_mode(mode, &ret);
	اگر (!*end)
		die(_("unable to parse mode: %s"), mode);
	بازگشت ret;
}

ایستا تهی parse_oid_or_die(ثابت نویسه *hex, ساختار object_id *oid,
			     ثابت نویسه **end, ثابت ساختار git_hash_algo *algop)
{
	اگر (parse_oid_hex_algop(hex, oid, end, algop) || *(*end)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

صحیح cmd_diff_pairs(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		   ساختار repository *repo)
{
	ساختار strbuf path_dst = STRBUF_INIT;
	ساختار strbuf path = STRBUF_INIT;
	ساختار strbuf meta = STRBUF_INIT;
	ساختار option *parseopts;
	ساختار rev_info revs;
	صحیح سطر_term = '\0';
	صحیح ret;

	ثابت نویسه * ثابت builtin_diff_pairs_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		پوچ
	};
	ساختار option builtin_diff_pairs_options[] = {
		OPT_END()
	};

	repo_init_revisions(repo, &revs, prefix);

	/*
	 * Diff options are usually parsed implicitly as part of
	 * setup_revisions(). Explicitly handle parsing to ensure options are
	 * printed in the usage message.
	 */
	parseopts = add_diff_options(builtin_diff_pairs_options, &revs.diffopt);
	show_usage_with_options_if_asked(argc, argv, builtin_diff_pairs_usage, parseopts);

	repo_config(repo, git_diff_basic_config, پوچ);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, prefix, parseopts, builtin_diff_pairs_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	اگر (setup_revisions(argc, argv, &revs, پوچ) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	اگر (revs.diffopt.سطر_termination)
		usage(_("working without -z is not supported"));

	اگر (revs.prune_data.nr)
		usage(_("pathspec arguments not supported"));

	اگر (revs.pending.nr || revs.max_شمار != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	اگر (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	اگر (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	وقتی (1) {
		ساختار object_id oid_a, oid_b;
		ساختار diff_filepair *pair;
		unsigned mode_a, mode_b;
		ثابت نویسه *p;
		نویسه status;

		اگر (strbuf_getwholeline(&meta, stdin, سطر_term) == EOF)
			بشکن;

		p = meta.buf;
		اگر (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			fputc('\0', revs.diffopt.file);
			fflush(revs.diffopt.file);
			ادامه;
		}

		اگر (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		mode_a = parse_mode_or_die(p, &p);
		mode_b = parse_mode_or_die(p, &p);

		اگر (S_ISDIR(mode_a) || S_ISDIR(mode_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		status = *p++;

		اگر (strbuf_getwholeline(&path, stdin, سطر_term) == EOF)
			die(_("got EOF while reading path"));

		انتخاب (status) {
		حالت DIFF_STATUS_ADDED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', mode_b,
						    &oid_b, 1, path.buf, 0);
			اگر (pair)
				pair->status = status;
			بشکن;

		حالت DIFF_STATUS_DELETED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', mode_a,
						    &oid_a, 1, path.buf, 0);
			اگر (pair)
				pair->status = status;
			بشکن;

		حالت DIFF_STATUS_TYPE_CHANGED:
		حالت DIFF_STATUS_MODIFIED:
			pair = diff_queue_change(&diff_queued_diff, &revs.diffopt,
						 mode_a, mode_b, &oid_a, &oid_b,
						 1, 1, path.buf, 0, 0);
			اگر (pair)
				pair->status = status;
			بشکن;

		حالت DIFF_STATUS_RENAMED:
		حالت DIFF_STATUS_COPIED: {
				ساختار diff_filespec *a, *b;
				unsigned صحیح score;

				اگر (strbuf_getwholeline(&path_dst, stdin, سطر_term) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(path.buf);
				b = alloc_filespec(path_dst.buf);
				fill_filespec(a, &oid_a, 1, mode_a);
				fill_filespec(b, &oid_b, 1, mode_b);

				pair = diff_queue(&diff_queued_diff, a, b);

				اگر (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->status = status;
				pair->renamed_pair = 1;
			}
			بشکن;

		پیشفرض:
			die(_("unknown diff status: %c"), status);
		}
	}

	revs.diffopt.no_free = 0;
	diffcore_std(&revs.diffopt);
	diff_flush(&revs.diffopt);
	ret = diff_result_code(&revs);

	strbuf_release(&path_dst);
	strbuf_release(&path);
	strbuf_release(&meta);
	release_revisions(&revs);
	FREE_AND_NULL(parseopts);

	بازگشت ret;
}
