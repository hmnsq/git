#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "diffcore.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"

ಸ್ಥಿರ unsigned parse_mode_or_die(ಧ್ರುವ ವರ್ಣ *mode, ಧ್ರುವ ವರ್ಣ **end)
{
	uint16_t ret;

	*end = parse_mode(mode, &ret);
	ಯದಿ (!*end)
		die(_("unable to parse mode: %s"), mode);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_oid_or_die(ಧ್ರುವ ವರ್ಣ *hex, ಸಂರಚನಾ object_id *oid,
			     ಧ್ರುವ ವರ್ಣ **end, ಧ್ರುವ ಸಂರಚನಾ git_hash_algo *algop)
{
	ಯದಿ (parse_oid_hex_algop(hex, oid, end, algop) || *(*end)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

ಪೂರ್ಣಾಙ್ಕ cmd_diff_pairs(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		   ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ strbuf path_dst = STRBUF_INIT;
	ಸಂರಚನಾ strbuf path = STRBUF_INIT;
	ಸಂರಚನಾ strbuf meta = STRBUF_INIT;
	ಸಂರಚನಾ option *parseopts;
	ಸಂರಚನಾ rev_info revs;
	ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_term = '\0';
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_diff_pairs_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		ರಿಕ್ತ
	};
	ಸಂರಚನಾ option builtin_diff_pairs_options[] = {
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

	repo_config(repo, git_diff_basic_config, ರಿಕ್ತ);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, prefix, parseopts, builtin_diff_pairs_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	ಯದಿ (setup_revisions(argc, argv, &revs, ರಿಕ್ತ) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	ಯದಿ (revs.diffopt.ಸಾಲು_termination)
		usage(_("working without -z is not supported"));

	ಯದಿ (revs.prune_data.nr)
		usage(_("pathspec arguments not supported"));

	ಯದಿ (revs.pending.nr || revs.max_ಎಣಿಕೆ != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	ಯದಿ (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	ಯದಿ (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	ಯಾವತ್ (1) {
		ಸಂರಚನಾ object_id oid_a, oid_b;
		ಸಂರಚನಾ diff_filepair *pair;
		unsigned mode_a, mode_b;
		ಧ್ರುವ ವರ್ಣ *p;
		ವರ್ಣ status;

		ಯದಿ (strbuf_getwholeline(&meta, stdin, ಸಾಲು_term) == EOF)
			ಭಙ್ಗ;

		p = meta.buf;
		ಯದಿ (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			fputc('\0', revs.diffopt.file);
			fflush(revs.diffopt.file);
			ಅನುವರ್ತ;
		}

		ಯದಿ (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		mode_a = parse_mode_or_die(p, &p);
		mode_b = parse_mode_or_die(p, &p);

		ಯದಿ (S_ISDIR(mode_a) || S_ISDIR(mode_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		status = *p++;

		ಯದಿ (strbuf_getwholeline(&path, stdin, ಸಾಲು_term) == EOF)
			die(_("got EOF while reading path"));

		ವಿಕಲ್ಪ (status) {
		ಪಕ್ಷ DIFF_STATUS_ADDED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', mode_b,
						    &oid_b, 1, path.buf, 0);
			ಯದಿ (pair)
				pair->status = status;
			ಭಙ್ಗ;

		ಪಕ್ಷ DIFF_STATUS_DELETED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', mode_a,
						    &oid_a, 1, path.buf, 0);
			ಯದಿ (pair)
				pair->status = status;
			ಭಙ್ಗ;

		ಪಕ್ಷ DIFF_STATUS_TYPE_CHANGED:
		ಪಕ್ಷ DIFF_STATUS_MODIFIED:
			pair = diff_queue_change(&diff_queued_diff, &revs.diffopt,
						 mode_a, mode_b, &oid_a, &oid_b,
						 1, 1, path.buf, 0, 0);
			ಯದಿ (pair)
				pair->status = status;
			ಭಙ್ಗ;

		ಪಕ್ಷ DIFF_STATUS_RENAMED:
		ಪಕ್ಷ DIFF_STATUS_COPIED: {
				ಸಂರಚನಾ diff_filespec *a, *b;
				unsigned ಪೂರ್ಣಾಙ್ಕ score;

				ಯದಿ (strbuf_getwholeline(&path_dst, stdin, ಸಾಲು_term) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(path.buf);
				b = alloc_filespec(path_dst.buf);
				fill_filespec(a, &oid_a, 1, mode_a);
				fill_filespec(b, &oid_b, 1, mode_b);

				pair = diff_queue(&diff_queued_diff, a, b);

				ಯದಿ (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->status = status;
				pair->renamed_pair = 1;
			}
			ಭಙ್ಗ;

		ಸ್ವಭಾವ:
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

	ಪ್ರತ್ಯಾಗಮ ret;
}
