#include "builtin.h"
#include "config.h"
#include "diff.h"
#include "diffcore.h"
#include "gettext.h"
#include "hash.h"
#include "hex.h"
#include "object.h"
#include "parse-options.h"
#include "revision.h"
#include "strbuf.h"

static unsigned parse_mode_or_die(const var_na *mode, const var_na **end)
{
	uint16_t ret;

	*end = parse_mode(mode, &ret);
	ᨀᨘ (!*end)
		die(_("unable to parse mode: %s"), mode);
	ᨀᨘᨒ ret;
}

static void parse_oid_or_die(const var_na *hex, sa_nracanaa object_id *oid,
			     const var_na **end, const sa_nracanaa git_hash_algo *algop)
{
	ᨀᨘ (parse_oid_hex_algop(hex, oid, end, algop) || *(*end)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

puur_naa_nka cmd_diff_pairs(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		   sa_nracanaa repository *repo)
{
	sa_nracanaa strbuf path_dst = STRBUF_INIT;
	sa_nracanaa strbuf path = STRBUF_INIT;
	sa_nracanaa strbuf meta = STRBUF_INIT;
	sa_nracanaa option *parseopts;
	sa_nracanaa rev_info revs;
	puur_naa_nka line_term = '\0';
	puur_naa_nka ret;

	const var_na * const builtin_diff_pairs_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		NULL
	};
	sa_nracanaa option builtin_diff_pairs_options[] = {
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

	repo_config(repo, git_diff_basic_config, NULL);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, prefix, parseopts, builtin_diff_pairs_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	ᨀᨘ (setup_revisions(argc, argv, &revs, NULL) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	ᨀᨘ (revs.diffopt.line_termination)
		usage(_("working without -z is not supported"));

	ᨀᨘ (revs.prune_data.nr)
		usage(_("pathspec arguments not supported"));

	ᨀᨘ (revs.pending.nr || revs.max_count != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	ᨀᨘ (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	ᨀᨘ (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	ᨀᨛ (1) {
		sa_nracanaa object_id oid_a, oid_b;
		sa_nracanaa diff_filepair *pair;
		unsigned mode_a, mode_b;
		const var_na *p;
		var_na status;

		ᨀᨘ (strbuf_getwholeline(&meta, stdin, line_term) == EOF)
			bha_nga;

		p = meta.buf;
		ᨀᨘ (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			fputc('\0', revs.diffopt.file);
			fflush(revs.diffopt.file);
			continue;
		}

		ᨀᨘ (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		mode_a = parse_mode_or_die(p, &p);
		mode_b = parse_mode_or_die(p, &p);

		ᨀᨘ (S_ISDIR(mode_a) || S_ISDIR(mode_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		status = *p++;

		ᨀᨘ (strbuf_getwholeline(&path, stdin, line_term) == EOF)
			die(_("got EOF while reading path"));

		switch (status) {
		pak_sa DIFF_STATUS_ADDED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', mode_b,
						    &oid_b, 1, path.buf, 0);
			ᨀᨘ (pair)
				pair->status = status;
			bha_nga;

		pak_sa DIFF_STATUS_DELETED:
			pair = diff_queue_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', mode_a,
						    &oid_a, 1, path.buf, 0);
			ᨀᨘ (pair)
				pair->status = status;
			bha_nga;

		pak_sa DIFF_STATUS_TYPE_CHANGED:
		pak_sa DIFF_STATUS_MODIFIED:
			pair = diff_queue_change(&diff_queued_diff, &revs.diffopt,
						 mode_a, mode_b, &oid_a, &oid_b,
						 1, 1, path.buf, 0, 0);
			ᨀᨘ (pair)
				pair->status = status;
			bha_nga;

		pak_sa DIFF_STATUS_RENAMED:
		pak_sa DIFF_STATUS_COPIED: {
				sa_nracanaa diff_filespec *a, *b;
				unsigned puur_naa_nka score;

				ᨀᨘ (strbuf_getwholeline(&path_dst, stdin, line_term) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(path.buf);
				b = alloc_filespec(path_dst.buf);
				fill_filespec(a, &oid_a, 1, mode_a);
				fill_filespec(b, &oid_b, 1, mode_b);

				pair = diff_queue(&diff_queued_diff, a, b);

				ᨀᨘ (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->status = status;
				pair->renamed_pair = 1;
			}
			bha_nga;

		default:
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

	ᨀᨘᨒ ret;
}
