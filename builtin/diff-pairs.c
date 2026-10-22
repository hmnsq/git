#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "diffcore.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "strbuf.h"

स्थिर अचिन्हित parse_mode_or_die(ध्रुव वर्ण *mode, ध्रुव वर्ण **end)
{
	uint16_t ret;

	*end = parse_mode(mode, &ret);
	यदि (!*end)
		die(_("unable to parse mode: %s"), mode);
	प्रत्यागम ret;
}

स्थिर शून्य parse_oid_or_die(ध्रुव वर्ण *hex, संरचना object_id *oid,
			     ध्रुव वर्ण **end, ध्रुव संरचना git_hash_algo *algop)
{
	यदि (parse_oid_hex_algop(hex, oid, end, algop) || *(*end)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

पूर्णाङ्क cmd_diff_pairs(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		   संरचना repository *repo)
{
	संरचना strbuf path_dst = STRBUF_INIT;
	संरचना strbuf path = STRBUF_INIT;
	संरचना strbuf meta = STRBUF_INIT;
	संरचना option *parseopts;
	संरचना rev_info revs;
	पूर्णाङ्क पंक्ति_term = '\0';
	पूर्णाङ्क ret;

	ध्रुव वर्ण * ध्रुव builtin_diff_pairs_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		रिक्त
	};
	संरचना option builtin_diff_pairs_options[] = {
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

	repo_config(repo, git_diff_basic_config, रिक्त);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, prefix, parseopts, builtin_diff_pairs_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	यदि (setup_revisions(argc, argv, &revs, रिक्त) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	यदि (revs.diffopt.पंक्ति_termination)
		usage(_("working without -z is not supported"));

	यदि (revs.prune_data.nr)
		usage(_("pathspec arguments not supported"));

	यदि (revs.pending.nr || revs.max_गिनती != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	यदि (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	यदि (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	यावत् (1) {
		संरचना object_id oid_a, oid_b;
		संरचना diff_filepair *pair;
		अचिन्हित mode_a, mode_b;
		ध्रुव वर्ण *p;
		वर्ण status;

		यदि (strbuf_getwholeline(&meta, मानक_निवेश, पंक्ति_term) == खातापूर्ण)
			भङ्ग;

		p = meta.buf;
		यदि (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			ख_अक्षर_दो('\0', revs.diffopt.file);
			ख_साफ(revs.diffopt.file);
			अनुवर्त;
		}

		यदि (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		mode_a = parse_mode_or_die(p, &p);
		mode_b = parse_mode_or_die(p, &p);

		यदि (S_ISDIR(mode_a) || S_ISDIR(mode_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		status = *p++;

		यदि (strbuf_getwholeline(&path, मानक_निवेश, पंक्ति_term) == खातापूर्ण)
			die(_("got EOF while reading path"));

		विकल्प (status) {
		पक्ष DIFF_STATUS_ADDED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', mode_b,
						    &oid_b, 1, path.buf, 0);
			यदि (pair)
				pair->status = status;
			भङ्ग;

		पक्ष DIFF_STATUS_DELETED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', mode_a,
						    &oid_a, 1, path.buf, 0);
			यदि (pair)
				pair->status = status;
			भङ्ग;

		पक्ष DIFF_STATUS_TYPE_CHANGED:
		पक्ष DIFF_STATUS_MODIFIED:
			pair = diff_queue_change(&diff_queued_diff, &revs.diffopt,
						 mode_a, mode_b, &oid_a, &oid_b,
						 1, 1, path.buf, 0, 0);
			यदि (pair)
				pair->status = status;
			भङ्ग;

		पक्ष DIFF_STATUS_RENAMED:
		पक्ष DIFF_STATUS_COPIED: {
				संरचना diff_filespec *a, *b;
				अचिन्हित पूर्णाङ्क score;

				यदि (strbuf_getwholeline(&path_dst, मानक_निवेश, पंक्ति_term) == खातापूर्ण)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(path.buf);
				b = alloc_filespec(path_dst.buf);
				fill_filespec(a, &oid_a, 1, mode_a);
				fill_filespec(b, &oid_b, 1, mode_b);

				pair = diff_queue(&diff_queued_diff, a, b);

				यदि (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->status = status;
				pair->renamed_pair = 1;
			}
			भङ्ग;

		स्वभाव:
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

	प्रत्यागम ret;
}
