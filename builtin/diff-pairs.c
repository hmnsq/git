#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "diffcore.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "strbuf.h"

స్థిర గుర్తులేని parse_బహుళకం_or_die(స్థిరరాశి అక్షరం *బహుళకం, స్థిరరాశి అక్షరం **చివర)
{
	uint16_t ret;

	*చివర = parse_బహుళకం(బహుళకం, &ret);
	ఒకవేళ (!*చివర)
		die(_("unable to parse mode: %s"), బహుళకం);
	తిరిగి ret;
}

స్థిర శూన్య parse_oid_or_die(స్థిరరాశి అక్షరం *hex, నిర్మాణం object_id *oid,
			     స్థిరరాశి అక్షరం **చివర, స్థిరరాశి నిర్మాణం git_hash_algo *algop)
{
	ఒకవేళ (parse_oid_hex_algop(hex, oid, చివర, algop) || *(*చివర)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

పూర్ణాంకం cmd_diff_జతలు(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo)
{
	నిర్మాణం strbuf మార్గం_dst = STRBUF_INIT;
	నిర్మాణం strbuf మార్గం = STRBUF_INIT;
	నిర్మాణం strbuf meta = STRBUF_INIT;
	నిర్మాణం option *parseopts;
	నిర్మాణం rev_info revs;
	పూర్ణాంకం పంక్తి_పదం = '\0';
	పూర్ణాంకం ret;

	స్థిరరాశి అక్షరం * స్థిరరాశి builtin_diff_జతలు_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		ఖాళీ
	};
	నిర్మాణం option builtin_diff_జతలు_options[] = {
		OPT_చివర()
	};

	repo_init_revisions(repo, &revs, ఉపసర్గ);

	/*
	 * Diff options are usually parsed implicitly as part of
	 * setup_revisions(). Explicitly handle parsing to ensure options are
	 * printed in the usage message.
	 */
	parseopts = కూడు_diff_options(builtin_diff_జతలు_options, &revs.diffopt);
	show_usage_with_options_if_asked(argc, argv, builtin_diff_జతలు_usage, parseopts);

	repo_config(repo, git_diff_basic_config, ఖాళీ);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, ఉపసర్గ, parseopts, builtin_diff_జతలు_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	ఒకవేళ (అమరిక_revisions(argc, argv, &revs, ఖాళీ) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	ఒకవేళ (revs.diffopt.పంక్తి_termination)
		usage(_("working without -z is not supported"));

	ఒకవేళ (revs.prune_దత్తాంశం.nr)
		usage(_("pathspec arguments not supported"));

	ఒకవేళ (revs.pending.nr || revs.max_లెక్క != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	ఒకవేళ (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	ఒకవేళ (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	అయితే (1) {
		నిర్మాణం object_id oid_a, oid_b;
		నిర్మాణం diff_filepair *pair;
		గుర్తులేని బహుళకం_a, బహుళకం_b;
		స్థిరరాశి అక్షరం *p;
		అక్షరం స్థితి_2;

		ఒకవేళ (strbuf_getwholeline(&meta, stdin, పంక్తి_పదం) == EOF)
			ఆపు;

		p = meta.buf;
		ఒకవేళ (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			fputc('\0', revs.diffopt.file);
			fflush(revs.diffopt.file);
			కొనసాగు;
		}

		ఒకవేళ (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		బహుళకం_a = parse_బహుళకం_or_die(p, &p);
		బహుళకం_b = parse_బహుళకం_or_die(p, &p);

		ఒకవేళ (S_ISDIR(బహుళకం_a) || S_ISDIR(బహుళకం_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		స్థితి_2 = *p++;

		ఒకవేళ (strbuf_getwholeline(&మార్గం, stdin, పంక్తి_పదం) == EOF)
			die(_("got EOF while reading path"));

		ఎంపిక (స్థితి_2) {
		స్థితి DIFF_స్థితి_ADDED:
			pair = diff_వరుస_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', బహుళకం_b,
						    &oid_b, 1, మార్గం.buf, 0);
			ఒకవేళ (pair)
				pair->స్థితి_2 = స్థితి_2;
			ఆపు;

		స్థితి DIFF_స్థితి_DELETED:
			pair = diff_వరుస_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', బహుళకం_a,
						    &oid_a, 1, మార్గం.buf, 0);
			ఒకవేళ (pair)
				pair->స్థితి_2 = స్థితి_2;
			ఆపు;

		స్థితి DIFF_స్థితి_TYPE_మారింది:
		స్థితి DIFF_స్థితి_MODIFIED:
			pair = diff_వరుస_change(&diff_queued_diff, &revs.diffopt,
						 బహుళకం_a, బహుళకం_b, &oid_a, &oid_b,
						 1, 1, మార్గం.buf, 0, 0);
			ఒకవేళ (pair)
				pair->స్థితి_2 = స్థితి_2;
			ఆపు;

		స్థితి DIFF_స్థితి_RENAMED:
		స్థితి DIFF_స్థితి_COPIED: {
				నిర్మాణం diff_filespec *a, *b;
				గుర్తులేని పూర్ణాంకం score;

				ఒకవేళ (strbuf_getwholeline(&మార్గం_dst, stdin, పంక్తి_పదం) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(మార్గం.buf);
				b = alloc_filespec(మార్గం_dst.buf);
				fill_filespec(a, &oid_a, 1, బహుళకం_a);
				fill_filespec(b, &oid_b, 1, బహుళకం_b);

				pair = diff_వరుస(&diff_queued_diff, a, b);

				ఒకవేళ (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->స్థితి_2 = స్థితి_2;
				pair->renamed_pair = 1;
			}
			ఆపు;

		అప్రమేయ:
			die(_("unknown diff status: %c"), స్థితి_2);
		}
	}

	revs.diffopt.no_free = 0;
	diffcore_std(&revs.diffopt);
	diff_flush(&revs.diffopt);
	ret = diff_ఫలితం_సంకేతం(&revs);

	strbuf_release(&మార్గం_dst);
	strbuf_release(&మార్గం);
	strbuf_release(&meta);
	release_revisions(&revs);
	FREE_AND_NULL(parseopts);

	తిరిగి ret;
}
