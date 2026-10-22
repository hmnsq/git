#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "diffcore.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "object.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "strbuf.h"

സ്ഥിതം unsigned parse_mode_or_die(സ്ഥിരം അക്ഷരം *mode, സ്ഥിരം അക്ഷരം **end)
{
	uint16_t ret;

	*end = parse_mode(mode, &ret);
	എങ്കിൽ (!*end)
		die(_("unable to parse mode: %s"), mode);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം parse_oid_or_die(സ്ഥിരം അക്ഷരം *hex, ഘടന object_id *oid,
			     സ്ഥിരം അക്ഷരം **end, സ്ഥിരം ഘടന git_hash_algo *algop)
{
	എങ്കിൽ (parse_oid_hex_algop(hex, oid, end, algop) || *(*end)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

പൂർണ്ണസംഖ്യ cmd_diff_pairs(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo)
{
	ഘടന strbuf path_dst = STRBUF_INIT;
	ഘടന strbuf path = STRBUF_INIT;
	ഘടന strbuf meta = STRBUF_INIT;
	ഘടന option *parseopts;
	ഘടന rev_info revs;
	പൂർണ്ണസംഖ്യ വരി_term = '\0';
	പൂർണ്ണസംഖ്യ ret;

	സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_diff_pairs_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		ശൂന്യ
	};
	ഘടന option builtin_diff_pairs_options[] = {
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

	repo_config(repo, git_diff_basic_config, ശൂന്യ);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, prefix, parseopts, builtin_diff_pairs_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	എങ്കിൽ (setup_revisions(argc, argv, &revs, ശൂന്യ) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	എങ്കിൽ (revs.diffopt.വരി_termination)
		usage(_("working without -z is not supported"));

	എങ്കിൽ (revs.prune_data.nr)
		usage(_("pathspec arguments not supported"));

	എങ്കിൽ (revs.pending.nr || revs.max_എണ്ണം != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	എങ്കിൽ (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	എങ്കിൽ (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	ആകുമ്പോൾ (1) {
		ഘടന object_id oid_a, oid_b;
		ഘടന diff_filepair *pair;
		unsigned mode_a, mode_b;
		സ്ഥിരം അക്ഷരം *p;
		അക്ഷരം status;

		എങ്കിൽ (strbuf_getwholeline(&meta, stdin, വരി_term) == EOF)
			നിർത്തുക;

		p = meta.buf;
		എങ്കിൽ (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			fputc('\0', revs.diffopt.file);
			fflush(revs.diffopt.file);
			തുടരുക;
		}

		എങ്കിൽ (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		mode_a = parse_mode_or_die(p, &p);
		mode_b = parse_mode_or_die(p, &p);

		എങ്കിൽ (S_ISDIR(mode_a) || S_ISDIR(mode_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		status = *p++;

		എങ്കിൽ (strbuf_getwholeline(&path, stdin, വരി_term) == EOF)
			die(_("got EOF while reading path"));

		തിരഞ്ഞെടുപ്പ് (status) {
		അവസ്ഥ DIFF_STATUS_ADDED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', mode_b,
						    &oid_b, 1, path.buf, 0);
			എങ്കിൽ (pair)
				pair->status = status;
			നിർത്തുക;

		അവസ്ഥ DIFF_STATUS_DELETED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', mode_a,
						    &oid_a, 1, path.buf, 0);
			എങ്കിൽ (pair)
				pair->status = status;
			നിർത്തുക;

		അവസ്ഥ DIFF_STATUS_TYPE_CHANGED:
		അവസ്ഥ DIFF_STATUS_MODIFIED:
			pair = diff_queue_change(&diff_queued_diff, &revs.diffopt,
						 mode_a, mode_b, &oid_a, &oid_b,
						 1, 1, path.buf, 0, 0);
			എങ്കിൽ (pair)
				pair->status = status;
			നിർത്തുക;

		അവസ്ഥ DIFF_STATUS_RENAMED:
		അവസ്ഥ DIFF_STATUS_COPIED: {
				ഘടന diff_filespec *a, *b;
				unsigned പൂർണ്ണസംഖ്യ score;

				എങ്കിൽ (strbuf_getwholeline(&path_dst, stdin, വരി_term) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(path.buf);
				b = alloc_filespec(path_dst.buf);
				fill_filespec(a, &oid_a, 1, mode_a);
				fill_filespec(b, &oid_b, 1, mode_b);

				pair = diff_queue(&diff_queued_diff, a, b);

				എങ്കിൽ (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->status = status;
				pair->renamed_pair = 1;
			}
			നിർത്തുക;

		സ്ഥിരസ്ഥിതി:
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

	തിരികെ ret;
}
