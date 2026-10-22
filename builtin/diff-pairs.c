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

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ parse_ಬಹುಲಕ_or_die(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಕೊನೆ)
{
	uint16_t ret;

	*ಕೊನೆ = parse_ಬಹುಲಕ(ಬಹುಲಕ, &ret);
	ಆದರೆ (!*ಕೊನೆ)
		die(_("unable to parse mode: %s"), ಬಹುಲಕ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_oid_or_die(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hex, ರಚನೆ object_id *oid,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಕೊನೆ, ಸ್ಥಿರಾಂಕ ರಚನೆ git_hash_algo *algop)
{
	ಆದರೆ (parse_oid_hex_algop(hex, oid, ಕೊನೆ, algop) || *(*ಕೊನೆ)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

ಪೂರ್ಣಾಂಕ cmd_diff_ಜೋಡಿಗಳು(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo)
{
	ರಚನೆ strbuf ಮಾರ್ಗ_dst = STRBUF_INIT;
	ರಚನೆ strbuf ಮಾರ್ಗ = STRBUF_INIT;
	ರಚನೆ strbuf meta = STRBUF_INIT;
	ರಚನೆ option *parseopts;
	ರಚನೆ rev_info revs;
	ಪೂರ್ಣಾಂಕ ಸಾಲು_ಪದ = '\0';
	ಪೂರ್ಣಾಂಕ ret;

	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_diff_ಜೋಡಿಗಳು_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		ಖಾಲಿ
	};
	ರಚನೆ option builtin_diff_ಜೋಡಿಗಳು_options[] = {
		OPT_ಕೊನೆ()
	};

	repo_init_revisions(repo, &revs, ಪೂರ್ವಪ್ರತ್ಯಯ);

	/*
	 * Diff options are usually parsed implicitly as part of
	 * setup_revisions(). Explicitly handle parsing to ensure options are
	 * printed in the usage message.
	 */
	parseopts = ಕೂಡಿಸು_diff_options(builtin_diff_ಜೋಡಿಗಳು_options, &revs.diffopt);
	show_usage_with_options_if_asked(argc, argv, builtin_diff_ಜೋಡಿಗಳು_usage, parseopts);

	repo_config(repo, git_diff_basic_config, ಖಾಲಿ);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, parseopts, builtin_diff_ಜೋಡಿಗಳು_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	ಆದರೆ (ಸಿದ್ಧತೆ_revisions(argc, argv, &revs, ಖಾಲಿ) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	ಆದರೆ (revs.diffopt.ಸಾಲು_termination)
		usage(_("working without -z is not supported"));

	ಆದರೆ (revs.prune_ದತ್ತಾಂಶ.nr)
		usage(_("pathspec arguments not supported"));

	ಆದರೆ (revs.pending.nr || revs.max_ಎಣಿಕೆ != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	ಆದರೆ (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	ಆದರೆ (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	ಆಗ (1) {
		ರಚನೆ object_id oid_a, oid_b;
		ರಚನೆ diff_filepair *pair;
		ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ_a, ಬಹುಲಕ_b;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
		ಅಕ್ಷರ ಸ್ಥಿತಿ_2;

		ಆದರೆ (strbuf_getwholeline(&meta, stdin, ಸಾಲು_ಪದ) == EOF)
			ನಿಲ್ಲಿಸು;

		p = meta.buf;
		ಆದರೆ (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			fputc('\0', revs.diffopt.file);
			fflush(revs.diffopt.file);
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		ಬಹುಲಕ_a = parse_ಬಹುಲಕ_or_die(p, &p);
		ಬಹುಲಕ_b = parse_ಬಹುಲಕ_or_die(p, &p);

		ಆದರೆ (S_ISDIR(ಬಹುಲಕ_a) || S_ISDIR(ಬಹುಲಕ_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		ಸ್ಥಿತಿ_2 = *p++;

		ಆದರೆ (strbuf_getwholeline(&ಮಾರ್ಗ, stdin, ಸಾಲು_ಪದ) == EOF)
			die(_("got EOF while reading path"));

		ಆಯ್ಕೆ (ಸ್ಥಿತಿ_2) {
		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_ADDED:
			pair = diff_ಸರತಿ_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', ಬಹುಲಕ_b,
						    &oid_b, 1, ಮಾರ್ಗ.buf, 0);
			ಆದರೆ (pair)
				pair->ಸ್ಥಿತಿ_2 = ಸ್ಥಿತಿ_2;
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_DELETED:
			pair = diff_ಸರತಿ_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', ಬಹುಲಕ_a,
						    &oid_a, 1, ಮಾರ್ಗ.buf, 0);
			ಆದರೆ (pair)
				pair->ಸ್ಥಿತಿ_2 = ಸ್ಥಿತಿ_2;
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_TYPE_ಬದಲಾಯಿತು:
		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_MODIFIED:
			pair = diff_ಸರತಿ_change(&diff_queued_diff, &revs.diffopt,
						 ಬಹುಲಕ_a, ಬಹುಲಕ_b, &oid_a, &oid_b,
						 1, 1, ಮಾರ್ಗ.buf, 0, 0);
			ಆದರೆ (pair)
				pair->ಸ್ಥಿತಿ_2 = ಸ್ಥಿತಿ_2;
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_RENAMED:
		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_COPIED: {
				ರಚನೆ diff_filespec *a, *b;
				ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ score;

				ಆದರೆ (strbuf_getwholeline(&ಮಾರ್ಗ_dst, stdin, ಸಾಲು_ಪದ) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(ಮಾರ್ಗ.buf);
				b = alloc_filespec(ಮಾರ್ಗ_dst.buf);
				fill_filespec(a, &oid_a, 1, ಬಹುಲಕ_a);
				fill_filespec(b, &oid_b, 1, ಬಹುಲಕ_b);

				pair = diff_ಸರತಿ(&diff_queued_diff, a, b);

				ಆದರೆ (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->ಸ್ಥಿತಿ_2 = ಸ್ಥಿತಿ_2;
				pair->renamed_pair = 1;
			}
			ನಿಲ್ಲಿಸು;

		ಪೂರ್ವನಿಯೋಜಿತ:
			die(_("unknown diff status: %c"), ಸ್ಥಿತಿ_2);
		}
	}

	revs.diffopt.no_free = 0;
	diffcore_std(&revs.diffopt);
	diff_flush(&revs.diffopt);
	ret = diff_ಫಲಿತಾಂಶ_ಸಂಕೇತ(&revs);

	strbuf_release(&ಮಾರ್ಗ_dst);
	strbuf_release(&ಮಾರ್ಗ);
	strbuf_release(&meta);
	release_revisions(&revs);
	FREE_AND_NULL(parseopts);

	ಹಿಂತಿರುಗಿ ret;
}
