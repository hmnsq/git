#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diffcore.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ parse_ਬਹੁਲਕ_or_die(ਅਚਲ ਅੱਖਰ *ਬਹੁਲਕ, ਅਚਲ ਅੱਖਰ **ਸਿਰਾ)
{
	uint16_t ret;

	*ਸਿਰਾ = parse_ਬਹੁਲਕ(ਬਹੁਲਕ, &ret);
	ਜੇ (!*ਸਿਰਾ)
		die(_("unable to parse mode: %s"), ਬਹੁਲਕ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ parse_oid_or_die(ਅਚਲ ਅੱਖਰ *hex, ਬਣਤਰ object_id *oid,
			     ਅਚਲ ਅੱਖਰ **ਸਿਰਾ, ਅਚਲ ਬਣਤਰ git_hash_algo *algop)
{
	ਜੇ (parse_oid_hex_algop(hex, oid, ਸਿਰਾ, algop) || *(*ਸਿਰਾ)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

ਪੂਰਨ cmd_diff_ਜੋੜੇ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ strbuf ਰਸਤਾ_dst = STRBUF_INIT;
	ਬਣਤਰ strbuf ਰਸਤਾ = STRBUF_INIT;
	ਬਣਤਰ strbuf meta = STRBUF_INIT;
	ਬਣਤਰ option *parseopts;
	ਬਣਤਰ rev_info revs;
	ਪੂਰਨ ਸਤਰ_ਪਦ = '\0';
	ਪੂਰਨ ret;

	ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_diff_ਜੋੜੇ_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		ਨਿਰਾ
	};
	ਬਣਤਰ option builtin_diff_ਜੋੜੇ_options[] = {
		OPT_ਸਿਰਾ()
	};

	repo_init_revisions(repo, &revs, ਅਗੇਤਰ);

	/*
	 * Diff options are usually parsed implicitly as part of
	 * setup_revisions(). Explicitly handle parsing to ensure options are
	 * printed in the usage message.
	 */
	parseopts = ਜੋੜੋ_diff_options(builtin_diff_ਜੋੜੇ_options, &revs.diffopt);
	show_usage_with_options_if_asked(argc, argv, builtin_diff_ਜੋੜੇ_usage, parseopts);

	repo_config(repo, git_diff_basic_config, ਨਿਰਾ);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, parseopts, builtin_diff_ਜੋੜੇ_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	ਜੇ (ਤਿਆਰੀ_revisions(argc, argv, &revs, ਨਿਰਾ) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	ਜੇ (revs.diffopt.ਸਤਰ_termination)
		usage(_("working without -z is not supported"));

	ਜੇ (revs.prune_ਅੰਕੜੇ.nr)
		usage(_("pathspec arguments not supported"));

	ਜੇ (revs.pending.nr || revs.max_ਗਿਣਤੀ != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	ਜੇ (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	ਜੇ (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	ਜਦੋਂ (1) {
		ਬਣਤਰ object_id oid_a, oid_b;
		ਬਣਤਰ diff_filepair *pair;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ_a, ਬਹੁਲਕ_b;
		ਅਚਲ ਅੱਖਰ *p;
		ਅੱਖਰ ਸਥਿਤੀ;

		ਜੇ (strbuf_getwholeline(&meta, stdin, ਸਤਰ_ਪਦ) == EOF)
			ਰੋਕੋ;

		p = meta.buf;
		ਜੇ (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			fputc('\0', revs.diffopt.file);
			fflush(revs.diffopt.file);
			ਜਾਰੀ;
		}

		ਜੇ (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		ਬਹੁਲਕ_a = parse_ਬਹੁਲਕ_or_die(p, &p);
		ਬਹੁਲਕ_b = parse_ਬਹੁਲਕ_or_die(p, &p);

		ਜੇ (S_ISDIR(ਬਹੁਲਕ_a) || S_ISDIR(ਬਹੁਲਕ_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		ਸਥਿਤੀ = *p++;

		ਜੇ (strbuf_getwholeline(&ਰਸਤਾ, stdin, ਸਤਰ_ਪਦ) == EOF)
			die(_("got EOF while reading path"));

		ਚੋਣ (ਸਥਿਤੀ) {
		ਹਾਲਤ DIFF_ਸਥਿਤੀ_ADDED:
			pair = diff_ਕਤਾਰ_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', ਬਹੁਲਕ_b,
						    &oid_b, 1, ਰਸਤਾ.buf, 0);
			ਜੇ (pair)
				pair->ਸਥਿਤੀ = ਸਥਿਤੀ;
			ਰੋਕੋ;

		ਹਾਲਤ DIFF_ਸਥਿਤੀ_DELETED:
			pair = diff_ਕਤਾਰ_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', ਬਹੁਲਕ_a,
						    &oid_a, 1, ਰਸਤਾ.buf, 0);
			ਜੇ (pair)
				pair->ਸਥਿਤੀ = ਸਥਿਤੀ;
			ਰੋਕੋ;

		ਹਾਲਤ DIFF_ਸਥਿਤੀ_TYPE_ਬਦਲਿਆ:
		ਹਾਲਤ DIFF_ਸਥਿਤੀ_MODIFIED:
			pair = diff_ਕਤਾਰ_change(&diff_queued_diff, &revs.diffopt,
						 ਬਹੁਲਕ_a, ਬਹੁਲਕ_b, &oid_a, &oid_b,
						 1, 1, ਰਸਤਾ.buf, 0, 0);
			ਜੇ (pair)
				pair->ਸਥਿਤੀ = ਸਥਿਤੀ;
			ਰੋਕੋ;

		ਹਾਲਤ DIFF_ਸਥਿਤੀ_RENAMED:
		ਹਾਲਤ DIFF_ਸਥਿਤੀ_COPIED: {
				ਬਣਤਰ diff_filespec *a, *b;
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ score;

				ਜੇ (strbuf_getwholeline(&ਰਸਤਾ_dst, stdin, ਸਤਰ_ਪਦ) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(ਰਸਤਾ.buf);
				b = alloc_filespec(ਰਸਤਾ_dst.buf);
				fill_filespec(a, &oid_a, 1, ਬਹੁਲਕ_a);
				fill_filespec(b, &oid_b, 1, ਬਹੁਲਕ_b);

				pair = diff_ਕਤਾਰ(&diff_queued_diff, a, b);

				ਜੇ (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->ਸਥਿਤੀ = ਸਥਿਤੀ;
				pair->renamed_pair = 1;
			}
			ਰੋਕੋ;

		ਮੂਲ:
			die(_("unknown diff status: %c"), ਸਥਿਤੀ);
		}
	}

	revs.diffopt.no_free = 0;
	diffcore_std(&revs.diffopt);
	diff_flush(&revs.diffopt);
	ret = diff_ਨਤੀਜਾ_ਕੋਡ(&revs);

	strbuf_release(&ਰਸਤਾ_dst);
	strbuf_release(&ਰਸਤਾ);
	strbuf_release(&meta);
	release_revisions(&revs);
	FREE_AND_NULL(parseopts);

	ਵਾਪਸ ret;
}
