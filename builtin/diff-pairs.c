#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "diffcore.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "object.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "strbuf.h"

સ્થિર unsigned parse_mode_or_die(અચલ અક્ષર *mode, અચલ અક્ષર **end)
{
	uint16_t ret;

	*end = parse_mode(mode, &ret);
	જો (!*end)
		die(_("unable to parse mode: %s"), mode);
	પાછા ret;
}

સ્થિર શૂન્ય parse_oid_or_die(અચલ અક્ષર *hex, સંરચના object_id *oid,
			     અચલ અક્ષર **end, અચલ સંરચના git_hash_algo *algop)
{
	જો (parse_oid_hex_algop(hex, oid, end, algop) || *(*end)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

પૂર્ણાંક cmd_diff_pairs(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		   સંરચના repository *repo)
{
	સંરચના strbuf path_dst = STRBUF_INIT;
	સંરચના strbuf path = STRBUF_INIT;
	સંરચના strbuf meta = STRBUF_INIT;
	સંરચના option *parseopts;
	સંરચના rev_info revs;
	પૂર્ણાંક લીટી_term = '\0';
	પૂર્ણાંક ret;

	અચલ અક્ષર * અચલ builtin_diff_pairs_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		ખાલી
	};
	સંરચના option builtin_diff_pairs_options[] = {
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

	repo_config(repo, git_diff_basic_config, ખાલી);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, prefix, parseopts, builtin_diff_pairs_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	જો (setup_revisions(argc, argv, &revs, ખાલી) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	જો (revs.diffopt.લીટી_termination)
		usage(_("working without -z is not supported"));

	જો (revs.prune_data.nr)
		usage(_("pathspec arguments not supported"));

	જો (revs.pending.nr || revs.max_ગણતરી != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	જો (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	જો (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	જ્યારે (1) {
		સંરચના object_id oid_a, oid_b;
		સંરચના diff_filepair *pair;
		unsigned mode_a, mode_b;
		અચલ અક્ષર *p;
		અક્ષર status;

		જો (strbuf_getwholeline(&meta, stdin, લીટી_term) == EOF)
			તોડો;

		p = meta.buf;
		જો (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			fputc('\0', revs.diffopt.file);
			fflush(revs.diffopt.file);
			ચાલુ;
		}

		જો (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		mode_a = parse_mode_or_die(p, &p);
		mode_b = parse_mode_or_die(p, &p);

		જો (S_ISDIR(mode_a) || S_ISDIR(mode_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		status = *p++;

		જો (strbuf_getwholeline(&path, stdin, લીટી_term) == EOF)
			die(_("got EOF while reading path"));

		પસંદગી (status) {
		સ્થિતિ DIFF_STATUS_ADDED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', mode_b,
						    &oid_b, 1, path.buf, 0);
			જો (pair)
				pair->status = status;
			તોડો;

		સ્થિતિ DIFF_STATUS_DELETED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', mode_a,
						    &oid_a, 1, path.buf, 0);
			જો (pair)
				pair->status = status;
			તોડો;

		સ્થિતિ DIFF_STATUS_TYPE_CHANGED:
		સ્થિતિ DIFF_STATUS_MODIFIED:
			pair = diff_queue_change(&diff_queued_diff, &revs.diffopt,
						 mode_a, mode_b, &oid_a, &oid_b,
						 1, 1, path.buf, 0, 0);
			જો (pair)
				pair->status = status;
			તોડો;

		સ્થિતિ DIFF_STATUS_RENAMED:
		સ્થિતિ DIFF_STATUS_COPIED: {
				સંરચના diff_filespec *a, *b;
				unsigned પૂર્ણાંક score;

				જો (strbuf_getwholeline(&path_dst, stdin, લીટી_term) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(path.buf);
				b = alloc_filespec(path_dst.buf);
				fill_filespec(a, &oid_a, 1, mode_a);
				fill_filespec(b, &oid_b, 1, mode_b);

				pair = diff_queue(&diff_queued_diff, a, b);

				જો (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->status = status;
				pair->renamed_pair = 1;
			}
			તોડો;

		મૂળભૂત:
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

	પાછા ret;
}
