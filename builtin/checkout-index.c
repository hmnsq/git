/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "cache-tree.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "entry.h"
#అన్తర్భావ "parallel-checkout.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "sparse-index.h"

#నిర్వచించు CHECKOUT_ALL 4
స్థిర పూర్ణాంకం nul_పదం_పంక్తి;
స్థిర పూర్ణాంకం checkout_stage; /* default to checkout stage0 */
స్థిర పూర్ణాంకం ignore_skip_worktree; /* default to 0 */
స్థిర పూర్ణాంకం గమ్యం_tempfile = -1;
స్థిర అక్షరం topath[4][TEMPORARY_FILENAME_నిడివి + 1];

స్థిర నిర్మాణం checkout state = CHECKOUT_INIT;

స్థిర శూన్య write_tempfile_రికార్డు(స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	పూర్ణాంకం i;
	పూర్ణాంకం have_tempname = 0;

	ఒకవేళ (CHECKOUT_ALL == checkout_stage) {
		కొరకు (i = 1; i < 4; i++)
			ఒకవేళ (topath[i][0]) {
				have_tempname = 1;
				ఆపు;
			}

		ఒకవేళ (have_tempname) {
			కొరకు (i = 1; i < 4; i++) {
				ఒకవేళ (i > 1)
					అక్షరం_రాయి(' ');
				ఒకవేళ (topath[i][0])
					fputs(topath[i], stdout);
				లేకపోతే
					అక్షరం_రాయి('.');
			}
		}
	} లేకపోతే ఒకవేళ (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	ఒకవేళ (have_tempname) {
		అక్షరం_రాయి('\t');
		write_name_quoted_relative(name, ఉపసర్గ, stdout,
					   nul_పదం_పంక్తి ? '\0' : '\n');
	}

	కొరకు (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

స్థిర పూర్ణాంకం checkout_file(నిర్మాణం సూచీ_state *సూచీ, స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	పూర్ణాంకం namelen = అక్షరశ్రేణి_పొడవు(name);
	పూర్ణాంకం pos = సూచీ_name_pos(సూచీ, name, namelen);
	పూర్ణాంకం has_same_name = 0;
	పూర్ణాంకం is_file = 0;
	పూర్ణాంకం is_skipped = 1;
	పూర్ణాంకం did_checkout = 0;
	పూర్ణాంకం errs = 0;

	ఒకవేళ (pos < 0)
		pos = -pos - 1;

	అయితే (pos < సూచీ->cache_nr) {
		నిర్మాణం cache_నమోదు *ce = సూచీ->cache[pos];
		ఒకవేళ (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			ఆపు;
		has_same_name = 1;
		pos++;
		ఒకవేళ (S_ISSPARSEDIR(ce->ce_బహుళకం))
			ఆపు;
		is_file = 1;
		ఒకవేళ (!ignore_skip_worktree && ce_skip_worktree(ce))
			ఆపు;
		is_skipped = 0;
		ఒకవేళ (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			కొనసాగు;
		did_checkout = 1;
		ఒకవేళ (checkout_నమోదు(ce, &state,
				   గమ్యం_tempfile ? topath[ce_stage(ce)] : ఖాళీ,
				   ఖాళీ) < 0)
			errs++;
	}

	ఒకవేళ (did_checkout) {
		ఒకవేళ (గమ్యం_tempfile)
			write_tempfile_రికార్డు(name, ఉపసర్గ);
		తిరిగి errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	ఒకవేళ (has_same_name && checkout_stage == CHECKOUT_ALL)
		తిరిగి 0;

	ఒకవేళ (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		ఒకవేళ (!has_same_name)
			fprintf(stderr, "is not in the cache");
		లేకపోతే ఒకవేళ (!is_file)
			fprintf(stderr, "is a sparse directory");
		లేకపోతే ఒకవేళ (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		లేకపోతే ఒకవేళ (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		లేకపోతే
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	తిరిగి -1;
}

స్థిర పూర్ణాంకం checkout_all(నిర్మాణం సూచీ_state *సూచీ, స్థిరరాశి అక్షరం *ఉపసర్గ, పూర్ణాంకం ఉపసర్గ_నిడివి)
{
	పూర్ణాంకం i, errs = 0;
	నిర్మాణం cache_నమోదు *చివరి_ce = ఖాళీ;

	కొరకు (i = 0; i < సూచీ->cache_nr ; i++) {
		నిర్మాణం cache_నమోదు *ce = సూచీ->cache[i];

		ఒకవేళ (S_ISSPARSEDIR(ce->ce_బహుళకం)) {
			ఒకవేళ (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			ఒకవేళ (ignore_skip_worktree) {
				ensure_నిండిన_సూచీ(సూచీ);
				ce = సూచీ->cache[i];
			}
		}

		ఒకవేళ (!ignore_skip_worktree && ce_skip_worktree(ce))
			కొనసాగు;
		ఒకవేళ (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			కొనసాగు;
		ఒకవేళ (ఉపసర్గ && *ఉపసర్గ &&
		    (ce_namelen(ce) <= ఉపసర్గ_నిడివి ||
		     memcmp(ఉపసర్గ, ce->name, ఉపసర్గ_నిడివి)))
			కొనసాగు;
		ఒకవేళ (చివరి_ce && గమ్యం_tempfile) {
			ఒకవేళ (ce_namelen(చివరి_ce) != ce_namelen(ce)
			    || memcmp(చివరి_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_రికార్డు(చివరి_ce->name, ఉపసర్గ);
		}
		ఒకవేళ (checkout_నమోదు(ce, &state,
				   గమ్యం_tempfile ? topath[ce_stage(ce)] : ఖాళీ,
				   ఖాళీ) < 0)
			errs++;
		చివరి_ce = ce;
	}
	ఒకవేళ (చివరి_ce && గమ్యం_tempfile)
		write_tempfile_రికార్డు(చివరి_ce->name, ఉపసర్గ);
	తిరిగి !!errs;
}

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_checkout_సూచీ_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	ఖాళీ
};

స్థిర పూర్ణాంకం option_parse_stage(స్థిరరాశి నిర్మాణం option *opt,
			      స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *stage = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} లేకపోతే {
		పూర్ణాంకం ch = arg[0];
		ఒకవేళ ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		లేకపోతే
			die(_("stage should be between 1 and 3 or all"));
	}
	తిరిగి 0;
}

పూర్ణాంకం cmd_checkout_సూచీ(పూర్ణాంకం argc,
		       స్థిరరాశి అక్షరం **argv,
		       స్థిరరాశి అక్షరం *ఉపసర్గ,
		       నిర్మాణం repository *repo)
{
	పూర్ణాంకం i;
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	పూర్ణాంకం all = 0;
	పూర్ణాంకం read_from_stdin = 0;
	పూర్ణాంకం ఉపసర్గ_నిడివి;
	పూర్ణాంకం force = 0, quiet = 0, not_new = 0;
	పూర్ణాంకం సూచీ_opt = 0;
	పూర్ణాంకం err = 0;
	పూర్ణాంకం pc_workers, pc_పరిమితి;
	నిర్మాణం option builtin_checkout_సూచీ_options[] = {
		OPT_BOOL('a', "all", &all,
			N_("check out all files in the index")),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &ignore_skip_worktree,
			N_("do not skip files with skip-worktree set")),
		OPT__FORCE(&force, N_("force overwrite of existing files"), 0),
		OPT__QUIET(&quiet,
			N_("no warning for existing files and files not in index")),
		OPT_BOOL('n', "no-create", &not_new,
			N_("don't checkout new files")),
		OPT_BOOL('u', "index", &సూచీ_opt,
			 N_("update stat information in the index file")),
		OPT_BOOL('z', ఖాళీ, &nul_పదం_పంక్తి,
			N_("paths are separated with NUL character")),
		OPT_BOOL(0, "stdin", &read_from_stdin,
			N_("read list of paths from the standard input")),
		OPT_BOOL(0, "temp", &గమ్యం_tempfile,
			N_("write the content to temporary files")),
		OPT_STRING(0, "prefix", &state.పునాది_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_CALLBACK_F(0, "stage", &checkout_stage, "(1|2|3|all)",
			N_("copy out the files from named stage"),
			PARSE_OPT_NONEG, option_parse_stage),
		OPT_చివర()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_checkout_సూచీ_usage,
					 builtin_checkout_సూచీ_options);
	repo_config(repo, git_default_config, ఖాళీ);
	ఉపసర్గ_నిడివి = ఉపసర్గ ? అక్షరశ్రేణి_పొడవు(ఉపసర్గ) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_నిండిన_సూచీ = 0;

	ఒకవేళ (repo_read_సూచీ(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, ఉపసర్గ, builtin_checkout_సూచీ_options,
			builtin_checkout_సూచీ_usage, 0);
	state.istate = repo->సూచీ;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	ఒకవేళ (!state.పునాది_dir)
		state.పునాది_dir = "";
	state.పునాది_dir_len = అక్షరశ్రేణి_పొడవు(state.పునాది_dir);

	ఒకవేళ (గమ్యం_tempfile < 0)
		గమ్యం_tempfile = (checkout_stage == CHECKOUT_ALL);
	ఒకవేళ (!గమ్యం_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	ఒకవేళ (సూచీ_opt && !state.పునాది_dir_len && !గమ్యం_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->సూచీ;
		repo_hold_locked_సూచీ(repo, &తాళం_file,
				       తాళం_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_పరిమితి);
	ఒకవేళ (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	కొరకు (i = 0; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];
		అక్షరం *p;

		ఒకవేళ (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		ఒకవేళ (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = ఉపసర్గ_మార్గం(repo, ఉపసర్గ, ఉపసర్గ_నిడివి, arg);
		err |= checkout_file(repo->సూచీ, p, ఉపసర్గ);
		విడుదల(p);
	}

	ఒకవేళ (read_from_stdin) {
		నిర్మాణం strbuf buf = STRBUF_INIT;
		నిర్మాణం strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		ఒకవేళ (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_పదం_పంక్తి ? strbuf_getline_nul : strbuf_getline_lf;
		అయితే (getline_fn(&buf, stdin) != EOF) {
			అక్షరం *p;
			ఒకవేళ (!nul_పదం_పంక్తి && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				ఒకవేళ (unquote_c_style(&unquoted, buf.buf, ఖాళీ))
					die("line is badly quoted");
				strbuf_మార్పిడి(&buf, &unquoted);
			}
			p = ఉపసర్గ_మార్గం(repo, ఉపసర్గ, ఉపసర్గ_నిడివి, buf.buf);
			err |= checkout_file(repo->సూచీ, p, ఉపసర్గ);
			విడుదల(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	ఒకవేళ (all)
		err |= checkout_all(repo->సూచీ, ఉపసర్గ, ఉపసర్గ_నిడివి);

	ఒకవేళ (pc_workers > 1)
		err |= నిరంతరం_parallel_checkout(&state, pc_workers, pc_పరిమితి,
					     ఖాళీ, ఖాళీ);

	ఒకవేళ (err)
		తిరిగి 1;

	ఒకవేళ (is_తాళం_file_locked(&తాళం_file) &&
	    write_locked_సూచీ(repo->సూచీ, &తాళం_file, COMMIT_తాళం))
		die("Unable to write new index file");
	తిరిగి 0;
}
