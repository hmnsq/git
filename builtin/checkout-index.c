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
स्थिर पूर्णांक nul_term_ओळ;
स्थिर पूर्णांक checkout_stage; /* default to checkout stage0 */
स्थिर पूर्णांक ignore_skip_worktree; /* default to 0 */
स्थिर पूर्णांक to_tempfile = -1;
स्थिर अक्षर topath[4][TEMPORARY_FILENAME_LENGTH + 1];

स्थिर संरचना checkout state = CHECKOUT_INIT;

स्थिर शून्य write_tempfile_record(स्थिरांक अक्षर *name, स्थिरांक अक्षर *prefix)
{
	पूर्णांक i;
	पूर्णांक have_tempname = 0;

	जर (CHECKOUT_ALL == checkout_stage) {
		साठी (i = 1; i < 4; i++)
			जर (topath[i][0]) {
				have_tempname = 1;
				थांबा;
			}

		जर (have_tempname) {
			साठी (i = 1; i < 4; i++) {
				जर (i > 1)
					अक्षर_दो(' ');
				जर (topath[i][0])
					ख_माला_दो(topath[i], मानक_निकास);
				अन्यथा
					अक्षर_दो('.');
			}
		}
	} अन्यथा जर (topath[checkout_stage][0]) {
		have_tempname = 1;
		ख_माला_दो(topath[checkout_stage], मानक_निकास);
	}

	जर (have_tempname) {
		अक्षर_दो('\t');
		write_name_quoted_relative(name, prefix, मानक_निकास,
					   nul_term_ओळ ? '\0' : '\n');
	}

	साठी (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

स्थिर पूर्णांक checkout_file(संरचना index_state *index, स्थिरांक अक्षर *name, स्थिरांक अक्षर *prefix)
{
	पूर्णांक namelen = म_माप(name);
	पूर्णांक pos = index_name_pos(index, name, namelen);
	पूर्णांक has_same_name = 0;
	पूर्णांक is_file = 0;
	पूर्णांक is_skipped = 1;
	पूर्णांक did_checkout = 0;
	पूर्णांक errs = 0;

	जर (pos < 0)
		pos = -pos - 1;

	पर्यंत (pos < index->cache_nr) {
		संरचना cache_entry *ce = index->cache[pos];
		जर (ce_namelen(ce) != namelen ||
		    स_भेद(ce->name, name, namelen))
			थांबा;
		has_same_name = 1;
		pos++;
		जर (S_ISSPARSEDIR(ce->ce_mode))
			थांबा;
		is_file = 1;
		जर (!ignore_skip_worktree && ce_skip_worktree(ce))
			थांबा;
		is_skipped = 0;
		जर (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			सुरूठेवा;
		did_checkout = 1;
		जर (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : रिक्त,
				   रिक्त) < 0)
			errs++;
	}

	जर (did_checkout) {
		जर (to_tempfile)
			write_tempfile_record(name, prefix);
		परत errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	जर (has_same_name && checkout_stage == CHECKOUT_ALL)
		परत 0;

	जर (!state.quiet) {
		ख_लिखो(मानक_त्रुटि, "git checkout-index: %s ", name);
		जर (!has_same_name)
			ख_लिखो(मानक_त्रुटि, "is not in the cache");
		अन्यथा जर (!is_file)
			ख_लिखो(मानक_त्रुटि, "is a sparse directory");
		अन्यथा जर (is_skipped)
			ख_लिखो(मानक_त्रुटि, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		अन्यथा जर (checkout_stage)
			ख_लिखो(मानक_त्रुटि, "does not exist at stage %d",
				checkout_stage);
		अन्यथा
			ख_लिखो(मानक_त्रुटि, "is unmerged");
		ख_अक्षर_दो('\n', मानक_त्रुटि);
	}
	परत -1;
}

स्थिर पूर्णांक checkout_all(संरचना index_state *index, स्थिरांक अक्षर *prefix, पूर्णांक prefix_length)
{
	पूर्णांक i, errs = 0;
	संरचना cache_entry *last_ce = रिक्त;

	साठी (i = 0; i < index->cache_nr ; i++) {
		संरचना cache_entry *ce = index->cache[i];

		जर (S_ISSPARSEDIR(ce->ce_mode)) {
			जर (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			जर (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		जर (!ignore_skip_worktree && ce_skip_worktree(ce))
			सुरूठेवा;
		जर (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			सुरूठेवा;
		जर (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     स_भेद(prefix, ce->name, prefix_length)))
			सुरूठेवा;
		जर (last_ce && to_tempfile) {
			जर (ce_namelen(last_ce) != ce_namelen(ce)
			    || स_भेद(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		जर (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : रिक्त,
				   रिक्त) < 0)
			errs++;
		last_ce = ce;
	}
	जर (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	परत !!errs;
}

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	रिक्त
};

स्थिर पूर्णांक option_parse_stage(स्थिरांक संरचना option *opt,
			      स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *stage = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	जर (!म_भेद(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} अन्यथा {
		पूर्णांक ch = arg[0];
		जर ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		अन्यथा
			die(_("stage should be between 1 and 3 or all"));
	}
	परत 0;
}

पूर्णांक cmd_checkout_index(पूर्णांक argc,
		       स्थिरांक अक्षर **argv,
		       स्थिरांक अक्षर *prefix,
		       संरचना repository *repo)
{
	पूर्णांक i;
	संरचना lock_file lock_file = LOCK_INIT;
	पूर्णांक all = 0;
	पूर्णांक read_from_stdin = 0;
	पूर्णांक prefix_length;
	पूर्णांक force = 0, quiet = 0, not_new = 0;
	पूर्णांक index_opt = 0;
	पूर्णांक err = 0;
	पूर्णांक pc_workers, pc_threshold;
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
		OPT_BOOL('z', रिक्त, &nul_term_ओळ,
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
	prefix_length = prefix ? म_माप(prefix) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	जर (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	जर (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = म_माप(state.base_dir);

	जर (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	जर (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	जर (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	जर (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	साठी (i = 0; i < argc; i++) {
		स्थिरांक अक्षर *arg = argv[i];
		अक्षर *p;

		जर (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		जर (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		मुक्त(p);
	}

	जर (read_from_stdin) {
		संरचना strbuf buf = STRBUF_INIT;
		संरचना strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		जर (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_ओळ ? strbuf_getline_nul : strbuf_getline_lf;
		पर्यंत (getline_fn(&buf, मानक_निवेश) != खातापूर्ण) {
			अक्षर *p;
			जर (!nul_term_ओळ && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				जर (unquote_c_style(&unquoted, buf.buf, रिक्त))
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

	जर (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	जर (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     रिक्त, रिक्त);

	जर (err)
		परत 1;

	जर (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	परत 0;
}
