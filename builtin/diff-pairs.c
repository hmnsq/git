#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "diffcore.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "strbuf.h"

நிலையான unsigned parse_mode_or_die(மாறிலி எழுத்து *mode, மாறிலி எழுத்து **end)
{
	uint16_t ret;

	*end = parse_mode(mode, &ret);
	எனில் (!*end)
		die(_("unable to parse mode: %s"), mode);
	திரும்பு ret;
}

நிலையான வெறுமை parse_oid_or_die(மாறிலி எழுத்து *hex, கட்டமைப்பு object_id *oid,
			     மாறிலி எழுத்து **end, மாறிலி கட்டமைப்பு git_hash_algo *algop)
{
	எனில் (parse_oid_hex_algop(hex, oid, end, algop) || *(*end)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

முழு cmd_diff_pairs(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு strbuf path_dst = STRBUF_INIT;
	கட்டமைப்பு strbuf path = STRBUF_INIT;
	கட்டமைப்பு strbuf meta = STRBUF_INIT;
	கட்டமைப்பு option *parseopts;
	கட்டமைப்பு rev_info revs;
	முழு வரி_term = '\0';
	முழு ret;

	மாறிலி எழுத்து * மாறிலி builtin_diff_pairs_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		வெற்று
	};
	கட்டமைப்பு option builtin_diff_pairs_options[] = {
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

	repo_config(repo, git_diff_basic_config, வெற்று);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, prefix, parseopts, builtin_diff_pairs_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	எனில் (setup_revisions(argc, argv, &revs, வெற்று) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	எனில் (revs.diffopt.வரி_termination)
		usage(_("working without -z is not supported"));

	எனில் (revs.prune_data.nr)
		usage(_("pathspec arguments not supported"));

	எனில் (revs.pending.nr || revs.max_மொத்தம் != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	எனில் (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	எனில் (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	போது (1) {
		கட்டமைப்பு object_id oid_a, oid_b;
		கட்டமைப்பு diff_filepair *pair;
		unsigned mode_a, mode_b;
		மாறிலி எழுத்து *p;
		எழுத்து status;

		எனில் (strbuf_getwholeline(&meta, stdin, வரி_term) == EOF)
			நிறுத்து;

		p = meta.buf;
		எனில் (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			fputc('\0', revs.diffopt.file);
			fflush(revs.diffopt.file);
			தொடர்;
		}

		எனில் (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		mode_a = parse_mode_or_die(p, &p);
		mode_b = parse_mode_or_die(p, &p);

		எனில் (S_ISDIR(mode_a) || S_ISDIR(mode_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		status = *p++;

		எனில் (strbuf_getwholeline(&path, stdin, வரி_term) == EOF)
			die(_("got EOF while reading path"));

		தேர்வு (status) {
		நிலை DIFF_STATUS_ADDED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', mode_b,
						    &oid_b, 1, path.buf, 0);
			எனில் (pair)
				pair->status = status;
			நிறுத்து;

		நிலை DIFF_STATUS_DELETED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', mode_a,
						    &oid_a, 1, path.buf, 0);
			எனில் (pair)
				pair->status = status;
			நிறுத்து;

		நிலை DIFF_STATUS_TYPE_CHANGED:
		நிலை DIFF_STATUS_MODIFIED:
			pair = diff_queue_change(&diff_queued_diff, &revs.diffopt,
						 mode_a, mode_b, &oid_a, &oid_b,
						 1, 1, path.buf, 0, 0);
			எனில் (pair)
				pair->status = status;
			நிறுத்து;

		நிலை DIFF_STATUS_RENAMED:
		நிலை DIFF_STATUS_COPIED: {
				கட்டமைப்பு diff_filespec *a, *b;
				unsigned முழு score;

				எனில் (strbuf_getwholeline(&path_dst, stdin, வரி_term) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(path.buf);
				b = alloc_filespec(path_dst.buf);
				fill_filespec(a, &oid_a, 1, mode_a);
				fill_filespec(b, &oid_b, 1, mode_b);

				pair = diff_queue(&diff_queued_diff, a, b);

				எனில் (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->status = status;
				pair->renamed_pair = 1;
			}
			நிறுத்து;

		இயல்பு:
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

	திரும்பு ret;
}
