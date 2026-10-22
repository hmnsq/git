/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "cache-tree.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "entry.h"
#അന്തര്ഭാവ "parallel-checkout.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "sparse-index.h"

#define CHECKOUT_ALL 4
സ്ഥിതം പൂർണ്ണസംഖ്യ nul_term_വരി;
സ്ഥിതം പൂർണ്ണസംഖ്യ checkout_stage; /* default to checkout stage0 */
സ്ഥിതം പൂർണ്ണസംഖ്യ ignore_skip_worktree; /* default to 0 */
സ്ഥിതം പൂർണ്ണസംഖ്യ to_tempfile = -1;
സ്ഥിതം അക്ഷരം topath[4][TEMPORARY_FILENAME_LENGTH + 1];

സ്ഥിതം ഘടന checkout state = CHECKOUT_INIT;

സ്ഥിതം ശൂന്യം write_tempfile_record(സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *prefix)
{
	പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ have_tempname = 0;

	എങ്കിൽ (CHECKOUT_ALL == checkout_stage) {
		വേണ്ടി (i = 1; i < 4; i++)
			എങ്കിൽ (topath[i][0]) {
				have_tempname = 1;
				നിർത്തുക;
			}

		എങ്കിൽ (have_tempname) {
			വേണ്ടി (i = 1; i < 4; i++) {
				എങ്കിൽ (i > 1)
					putchar(' ');
				എങ്കിൽ (topath[i][0])
					fputs(topath[i], stdout);
				അല്ലെങ്കിൽ
					putchar('.');
			}
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	എങ്കിൽ (have_tempname) {
		putchar('\t');
		write_name_quoted_relative(name, prefix, stdout,
					   nul_term_വരി ? '\0' : '\n');
	}

	വേണ്ടി (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ checkout_file(ഘടന index_state *index, സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *prefix)
{
	പൂർണ്ണസംഖ്യ namelen = strlen(name);
	പൂർണ്ണസംഖ്യ pos = index_name_pos(index, name, namelen);
	പൂർണ്ണസംഖ്യ has_same_name = 0;
	പൂർണ്ണസംഖ്യ is_file = 0;
	പൂർണ്ണസംഖ്യ is_skipped = 1;
	പൂർണ്ണസംഖ്യ did_checkout = 0;
	പൂർണ്ണസംഖ്യ errs = 0;

	എങ്കിൽ (pos < 0)
		pos = -pos - 1;

	ആകുമ്പോൾ (pos < index->cache_nr) {
		ഘടന cache_entry *ce = index->cache[pos];
		എങ്കിൽ (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			നിർത്തുക;
		has_same_name = 1;
		pos++;
		എങ്കിൽ (S_ISSPARSEDIR(ce->ce_mode))
			നിർത്തുക;
		is_file = 1;
		എങ്കിൽ (!ignore_skip_worktree && ce_skip_worktree(ce))
			നിർത്തുക;
		is_skipped = 0;
		എങ്കിൽ (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			തുടരുക;
		did_checkout = 1;
		എങ്കിൽ (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : ശൂന്യ,
				   ശൂന്യ) < 0)
			errs++;
	}

	എങ്കിൽ (did_checkout) {
		എങ്കിൽ (to_tempfile)
			write_tempfile_record(name, prefix);
		തിരികെ errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	എങ്കിൽ (has_same_name && checkout_stage == CHECKOUT_ALL)
		തിരികെ 0;

	എങ്കിൽ (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		എങ്കിൽ (!has_same_name)
			fprintf(stderr, "is not in the cache");
		അല്ലെങ്കിൽ എങ്കിൽ (!is_file)
			fprintf(stderr, "is a sparse directory");
		അല്ലെങ്കിൽ എങ്കിൽ (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		അല്ലെങ്കിൽ എങ്കിൽ (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		അല്ലെങ്കിൽ
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	തിരികെ -1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ checkout_all(ഘടന index_state *index, സ്ഥിരം അക്ഷരം *prefix, പൂർണ്ണസംഖ്യ prefix_length)
{
	പൂർണ്ണസംഖ്യ i, errs = 0;
	ഘടന cache_entry *last_ce = ശൂന്യ;

	വേണ്ടി (i = 0; i < index->cache_nr ; i++) {
		ഘടന cache_entry *ce = index->cache[i];

		എങ്കിൽ (S_ISSPARSEDIR(ce->ce_mode)) {
			എങ്കിൽ (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			എങ്കിൽ (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		എങ്കിൽ (!ignore_skip_worktree && ce_skip_worktree(ce))
			തുടരുക;
		എങ്കിൽ (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			തുടരുക;
		എങ്കിൽ (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     memcmp(prefix, ce->name, prefix_length)))
			തുടരുക;
		എങ്കിൽ (last_ce && to_tempfile) {
			എങ്കിൽ (ce_namelen(last_ce) != ce_namelen(ce)
			    || memcmp(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		എങ്കിൽ (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : ശൂന്യ,
				   ശൂന്യ) < 0)
			errs++;
		last_ce = ce;
	}
	എങ്കിൽ (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	തിരികെ !!errs;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_stage(സ്ഥിരം ഘടന option *opt,
			      സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *stage = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} അല്ലെങ്കിൽ {
		പൂർണ്ണസംഖ്യ ch = arg[0];
		എങ്കിൽ ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		അല്ലെങ്കിൽ
			die(_("stage should be between 1 and 3 or all"));
	}
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_checkout_index(പൂർണ്ണസംഖ്യ argc,
		       സ്ഥിരം അക്ഷരം **argv,
		       സ്ഥിരം അക്ഷരം *prefix,
		       ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന lock_file lock_file = LOCK_INIT;
	പൂർണ്ണസംഖ്യ all = 0;
	പൂർണ്ണസംഖ്യ read_from_stdin = 0;
	പൂർണ്ണസംഖ്യ prefix_length;
	പൂർണ്ണസംഖ്യ force = 0, quiet = 0, not_new = 0;
	പൂർണ്ണസംഖ്യ index_opt = 0;
	പൂർണ്ണസംഖ്യ err = 0;
	പൂർണ്ണസംഖ്യ pc_workers, pc_threshold;
	ഘടന option builtin_checkout_index_options[] = {
		OPT_BOOL('a', "all", &all,
			N_("check out all files in the index")),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &ignore_skip_worktree,
			N_("do not skip files with skip-worktree set")),
		OPT__FORCE(&force, N_("force overwrite of existing files"), 0),
		OPT__QUIET(&quiet,
			N_("no warning for existing files and files not in index")),
		OPT_BOOL('n', "no-create", &not_new,
			N_("don't checkout new files")),
		OPT_BOOL('u', "index", &index_opt,
			 N_("update stat information in the index file")),
		OPT_BOOL('z', ശൂന്യ, &nul_term_വരി,
			N_("paths are separated with NUL character")),
		OPT_BOOL(0, "stdin", &read_from_stdin,
			N_("read list of paths from the standard input")),
		OPT_BOOL(0, "temp", &to_tempfile,
			N_("write the content to temporary files")),
		OPT_STRING(0, "prefix", &state.base_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_CALLBACK_F(0, "stage", &checkout_stage, "(1|2|3|all)",
			N_("copy out the files from named stage"),
			PARSE_OPT_NONEG, option_parse_stage),
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_checkout_index_usage,
					 builtin_checkout_index_options);
	repo_config(repo, git_default_config, ശൂന്യ);
	prefix_length = prefix ? strlen(prefix) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	എങ്കിൽ (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	എങ്കിൽ (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = strlen(state.base_dir);

	എങ്കിൽ (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	എങ്കിൽ (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	എങ്കിൽ (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	എങ്കിൽ (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	വേണ്ടി (i = 0; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];
		അക്ഷരം *p;

		എങ്കിൽ (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		എങ്കിൽ (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		സ്വതന്ത്രം(p);
	}

	എങ്കിൽ (read_from_stdin) {
		ഘടന strbuf buf = STRBUF_INIT;
		ഘടന strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		എങ്കിൽ (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_വരി ? strbuf_getline_nul : strbuf_getline_lf;
		ആകുമ്പോൾ (getline_fn(&buf, stdin) != EOF) {
			അക്ഷരം *p;
			എങ്കിൽ (!nul_term_വരി && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				എങ്കിൽ (unquote_c_style(&unquoted, buf.buf, ശൂന്യ))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(repo, prefix, prefix_length, buf.buf);
			err |= checkout_file(repo->index, p, prefix);
			സ്വതന്ത്രം(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	എങ്കിൽ (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	എങ്കിൽ (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     ശൂന്യ, ശൂന്യ);

	എങ്കിൽ (err)
		തിരികെ 1;

	എങ്കിൽ (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	തിരികെ 0;
}
