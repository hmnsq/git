/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "entry.h"
#अन्तर्भाव "parallel-checkout.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "sparse-index.h"

#define CHECKOUT_ALL 4
स्थिर पूर्णाङ्क nul_term_पङ्क्तिः;
स्थिर पूर्णाङ्क checkout_stage; /* default to checkout stage0 */
स्थिर पूर्णाङ्क ignore_skip_worktree; /* default to 0 */
स्थिर पूर्णाङ्क to_tempfile = -1;
स्थिर वर्ण topath[4][TEMPORARY_FILENAME_LENGTH + 1];

स्थिर संरचना checkout state = CHECKOUT_INIT;

स्थिर शून्य write_tempfile_record(ध्रुव वर्ण *name, ध्रुव वर्ण *prefix)
{
	पूर्णाङ्क i;
	पूर्णाङ्क have_tempname = 0;

	जइ (CHECKOUT_ALL == checkout_stage) {
		प्रति (i = 1; i < 4; i++)
			जइ (topath[i][0]) {
				have_tempname = 1;
				भङ्ग;
			}

		जइ (have_tempname) {
			प्रति (i = 1; i < 4; i++) {
				जइ (i > 1)
					putchar(' ');
				जइ (topath[i][0])
					fputs(topath[i], stdout);
				अण्णहा
					putchar('.');
			}
		}
	} अण्णहा जइ (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	जइ (have_tempname) {
		putchar('\t');
		write_name_quoted_relative(name, prefix, stdout,
					   nul_term_पङ्क्तिः ? '\0' : '\n');
	}

	प्रति (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

स्थिर पूर्णाङ्क checkout_file(संरचना index_state *index, ध्रुव वर्ण *name, ध्रुव वर्ण *prefix)
{
	पूर्णाङ्क namelen = strlen(name);
	पूर्णाङ्क pos = index_name_pos(index, name, namelen);
	पूर्णाङ्क has_same_name = 0;
	पूर्णाङ्क is_file = 0;
	पूर्णाङ्क is_skipped = 1;
	पूर्णाङ्क did_checkout = 0;
	पूर्णाङ्क errs = 0;

	जइ (pos < 0)
		pos = -pos - 1;

	जाव (pos < index->cache_nr) {
		संरचना cache_entry *ce = index->cache[pos];
		जइ (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			भङ्ग;
		has_same_name = 1;
		pos++;
		जइ (S_ISSPARSEDIR(ce->ce_mode))
			भङ्ग;
		is_file = 1;
		जइ (!ignore_skip_worktree && ce_skip_worktree(ce))
			भङ्ग;
		is_skipped = 0;
		जइ (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			अनुवर्त;
		did_checkout = 1;
		जइ (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : रिक्त,
				   रिक्त) < 0)
			errs++;
	}

	जइ (did_checkout) {
		जइ (to_tempfile)
			write_tempfile_record(name, prefix);
		प्रत्यागम errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	जइ (has_same_name && checkout_stage == CHECKOUT_ALL)
		प्रत्यागम 0;

	जइ (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		जइ (!has_same_name)
			fprintf(stderr, "is not in the cache");
		अण्णहा जइ (!is_file)
			fprintf(stderr, "is a sparse directory");
		अण्णहा जइ (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		अण्णहा जइ (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		अण्णहा
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	प्रत्यागम -1;
}

स्थिर पूर्णाङ्क checkout_all(संरचना index_state *index, ध्रुव वर्ण *prefix, पूर्णाङ्क prefix_length)
{
	पूर्णाङ्क i, errs = 0;
	संरचना cache_entry *last_ce = रिक्त;

	प्रति (i = 0; i < index->cache_nr ; i++) {
		संरचना cache_entry *ce = index->cache[i];

		जइ (S_ISSPARSEDIR(ce->ce_mode)) {
			जइ (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			जइ (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		जइ (!ignore_skip_worktree && ce_skip_worktree(ce))
			अनुवर्त;
		जइ (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			अनुवर्त;
		जइ (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     memcmp(prefix, ce->name, prefix_length)))
			अनुवर्त;
		जइ (last_ce && to_tempfile) {
			जइ (ce_namelen(last_ce) != ce_namelen(ce)
			    || memcmp(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		जइ (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : रिक्त,
				   रिक्त) < 0)
			errs++;
		last_ce = ce;
	}
	जइ (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	प्रत्यागम !!errs;
}

स्थिर ध्रुव वर्ण * ध्रुव builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	रिक्त
};

स्थिर पूर्णाङ्क option_parse_stage(ध्रुव संरचना option *opt,
			      ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *stage = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	जइ (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} अण्णहा {
		पूर्णाङ्क ch = arg[0];
		जइ ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		अण्णहा
			die(_("stage should be between 1 and 3 or all"));
	}
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_checkout_index(पूर्णाङ्क argc,
		       ध्रुव वर्ण **argv,
		       ध्रुव वर्ण *prefix,
		       संरचना repository *repo)
{
	पूर्णाङ्क i;
	संरचना lock_file lock_file = LOCK_INIT;
	पूर्णाङ्क all = 0;
	पूर्णाङ्क read_from_stdin = 0;
	पूर्णाङ्क prefix_length;
	पूर्णाङ्क force = 0, quiet = 0, not_new = 0;
	पूर्णाङ्क index_opt = 0;
	पूर्णाङ्क err = 0;
	पूर्णाङ्क pc_workers, pc_threshold;
	संरचना option builtin_checkout_index_options[] = {
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
		OPT_BOOL('z', रिक्त, &nul_term_पङ्क्तिः,
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
	repo_config(repo, git_default_config, रिक्त);
	prefix_length = prefix ? strlen(prefix) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	जइ (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	जइ (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = strlen(state.base_dir);

	जइ (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	जइ (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	जइ (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	जइ (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	प्रति (i = 0; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		वर्ण *p;

		जइ (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		जइ (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		मुक्त(p);
	}

	जइ (read_from_stdin) {
		संरचना strbuf buf = STRBUF_INIT;
		संरचना strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		जइ (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_पङ्क्तिः ? strbuf_getline_nul : strbuf_getline_lf;
		जाव (getline_fn(&buf, stdin) != EOF) {
			वर्ण *p;
			जइ (!nul_term_पङ्क्तिः && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				जइ (unquote_c_style(&unquoted, buf.buf, रिक्त))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(repo, prefix, prefix_length, buf.buf);
			err |= checkout_file(repo->index, p, prefix);
			मुक्त(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	जइ (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	जइ (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     रिक्त, रिक्त);

	जइ (err)
		प्रत्यागम 1;

	जइ (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	प्रत्यागम 0;
}
