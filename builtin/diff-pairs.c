#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "diffcore.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "object.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"

ସ୍ଥିର unsigned parse_mode_or_die(ଅଚଳ ଅକ୍ଷର *mode, ଅଚଳ ଅକ୍ଷର **end)
{
	uint16_t ret;

	*end = parse_mode(mode, &ret);
	ଯଦି (!*end)
		die(_("unable to parse mode: %s"), mode);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ parse_oid_or_die(ଅଚଳ ଅକ୍ଷର *hex, ସଂରଚନା object_id *oid,
			     ଅଚଳ ଅକ୍ଷର **end, ଅଚଳ ସଂରଚନା git_hash_algo *algop)
{
	ଯଦି (parse_oid_hex_algop(hex, oid, end, algop) || *(*end)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_diff_pairs(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo)
{
	ସଂରଚନା strbuf path_dst = STRBUF_INIT;
	ସଂରଚନା strbuf path = STRBUF_INIT;
	ସଂରଚନା strbuf meta = STRBUF_INIT;
	ସଂରଚନା option *parseopts;
	ସଂରଚନା rev_info revs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_term = '\0';
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_diff_pairs_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		ଖାଲି
	};
	ସଂରଚନା option builtin_diff_pairs_options[] = {
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

	repo_config(repo, git_diff_basic_config, ଖାଲି);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, prefix, parseopts, builtin_diff_pairs_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	ଯଦି (setup_revisions(argc, argv, &revs, ଖାଲି) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	ଯଦି (revs.diffopt.ଧାଡ଼ି_termination)
		usage(_("working without -z is not supported"));

	ଯଦି (revs.prune_data.nr)
		usage(_("pathspec arguments not supported"));

	ଯଦି (revs.pending.nr || revs.max_ଗଣନା != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	ଯଦି (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	ଯଦି (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	ଯେତେବେଳେ (1) {
		ସଂରଚନା object_id oid_a, oid_b;
		ସଂରଚନା diff_filepair *pair;
		unsigned mode_a, mode_b;
		ଅଚଳ ଅକ୍ଷର *p;
		ଅକ୍ଷର status;

		ଯଦି (strbuf_getwholeline(&meta, stdin, ଧାଡ଼ି_term) == EOF)
			ଭାଙ୍ଗ;

		p = meta.buf;
		ଯଦି (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			fputc('\0', revs.diffopt.file);
			fflush(revs.diffopt.file);
			ଜାରି;
		}

		ଯଦି (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		mode_a = parse_mode_or_die(p, &p);
		mode_b = parse_mode_or_die(p, &p);

		ଯଦି (S_ISDIR(mode_a) || S_ISDIR(mode_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		status = *p++;

		ଯଦି (strbuf_getwholeline(&path, stdin, ଧାଡ଼ି_term) == EOF)
			die(_("got EOF while reading path"));

		ବାଛ (status) {
		ସ୍ଥିତି DIFF_STATUS_ADDED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', mode_b,
						    &oid_b, 1, path.buf, 0);
			ଯଦି (pair)
				pair->status = status;
			ଭାଙ୍ଗ;

		ସ୍ଥିତି DIFF_STATUS_DELETED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', mode_a,
						    &oid_a, 1, path.buf, 0);
			ଯଦି (pair)
				pair->status = status;
			ଭାଙ୍ଗ;

		ସ୍ଥିତି DIFF_STATUS_TYPE_CHANGED:
		ସ୍ଥିତି DIFF_STATUS_MODIFIED:
			pair = diff_queue_change(&diff_queued_diff, &revs.diffopt,
						 mode_a, mode_b, &oid_a, &oid_b,
						 1, 1, path.buf, 0, 0);
			ଯଦି (pair)
				pair->status = status;
			ଭାଙ୍ଗ;

		ସ୍ଥିତି DIFF_STATUS_RENAMED:
		ସ୍ଥିତି DIFF_STATUS_COPIED: {
				ସଂରଚନା diff_filespec *a, *b;
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା score;

				ଯଦି (strbuf_getwholeline(&path_dst, stdin, ଧାଡ଼ି_term) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(path.buf);
				b = alloc_filespec(path_dst.buf);
				fill_filespec(a, &oid_a, 1, mode_a);
				fill_filespec(b, &oid_b, 1, mode_b);

				pair = diff_queue(&diff_queued_diff, a, b);

				ଯଦି (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->status = status;
				pair->renamed_pair = 1;
			}
			ଭାଙ୍ଗ;

		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
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

	ଫେରନ୍ତୁ ret;
}
