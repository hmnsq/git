/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "lockfile.h"
#समावेश "quote.h"
#समावेश "cache-tree.h"
#समावेश "parse-options.h"
#समावेश "entry.h"
#समावेश "parallel-checkout.h"
#समावेश "read-cache-ll.h"
#समावेश "setup.h"
#समावेश "sparse-index.h"

#घोषणा CHECKOUT_ALL 4
अटल पूर्णांक nul_term_पंक्ति;
अटल पूर्णांक checkout_stage; /* default to checkout stage0 */
अटल पूर्णांक ignore_skip_worktree; /* default to 0 */
अटल पूर्णांक to_tempfile = -1;
अटल अक्षर topath[4][TEMPORARY_FILENAME_लंबाई + 1];

अटल काष्ठा checkout state = CHECKOUT_INIT;

अटल व्योम write_tempfile_record(स्थिर अक्षर *name, स्थिर अक्षर *prefix)
{
	पूर्णांक i;
	पूर्णांक have_tempname = 0;

	अगर (CHECKOUT_ALL == checkout_stage) {
		क्रम (i = 1; i < 4; i++)
			अगर (topath[i][0]) {
				have_tempname = 1;
				अवरोध;
			}

		अगर (have_tempname) {
			क्रम (i = 1; i < 4; i++) {
				अगर (i > 1)
					अक्षर_दो(' ');
				अगर (topath[i][0])
					ख_माला_दो(topath[i], मानक_निकास);
				अन्यथा
					अक्षर_दो('.');
			}
		}
	} अन्यथा अगर (topath[checkout_stage][0]) {
		have_tempname = 1;
		ख_माला_दो(topath[checkout_stage], मानक_निकास);
	}

	अगर (have_tempname) {
		अक्षर_दो('\t');
		write_name_quoted_relative(name, prefix, मानक_निकास,
					   nul_term_पंक्ति ? '\0' : '\n');
	}

	क्रम (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

अटल पूर्णांक checkout_file(काष्ठा सूचकांक_state *सूचकांक, स्थिर अक्षर *name, स्थिर अक्षर *prefix)
{
	पूर्णांक namelen = म_माप(name);
	पूर्णांक pos = सूचकांक_name_pos(सूचकांक, name, namelen);
	पूर्णांक has_same_name = 0;
	पूर्णांक is_file = 0;
	पूर्णांक is_skipped = 1;
	पूर्णांक did_checkout = 0;
	पूर्णांक errs = 0;

	अगर (pos < 0)
		pos = -pos - 1;

	जबतक (pos < सूचकांक->cache_nr) {
		काष्ठा cache_entry *ce = सूचकांक->cache[pos];
		अगर (ce_namelen(ce) != namelen ||
		    स_भेद(ce->name, name, namelen))
			अवरोध;
		has_same_name = 1;
		pos++;
		अगर (S_ISSPARSEDIR(ce->ce_mode))
			अवरोध;
		is_file = 1;
		अगर (!ignore_skip_worktree && ce_skip_worktree(ce))
			अवरोध;
		is_skipped = 0;
		अगर (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			जारी;
		did_checkout = 1;
		अगर (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : शून्य,
				   शून्य) < 0)
			errs++;
	}

	अगर (did_checkout) {
		अगर (to_tempfile)
			write_tempfile_record(name, prefix);
		वापस errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	अगर (has_same_name && checkout_stage == CHECKOUT_ALL)
		वापस 0;

	अगर (!state.quiet) {
		ख_लिखो(मानक_त्रुटि, "git checkout-index: %s ", name);
		अगर (!has_same_name)
			ख_लिखो(मानक_त्रुटि, "is not in the cache");
		अन्यथा अगर (!is_file)
			ख_लिखो(मानक_त्रुटि, "is a sparse directory");
		अन्यथा अगर (is_skipped)
			ख_लिखो(मानक_त्रुटि, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		अन्यथा अगर (checkout_stage)
			ख_लिखो(मानक_त्रुटि, "does not exist at stage %d",
				checkout_stage);
		अन्यथा
			ख_लिखो(मानक_त्रुटि, "is unmerged");
		ख_अक्षर_दो('\n', मानक_त्रुटि);
	}
	वापस -1;
}

अटल पूर्णांक checkout_all(काष्ठा सूचकांक_state *सूचकांक, स्थिर अक्षर *prefix, पूर्णांक prefix_लंबाई)
{
	पूर्णांक i, errs = 0;
	काष्ठा cache_entry *last_ce = शून्य;

	क्रम (i = 0; i < सूचकांक->cache_nr ; i++) {
		काष्ठा cache_entry *ce = सूचकांक->cache[i];

		अगर (S_ISSPARSEDIR(ce->ce_mode)) {
			अगर (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			अगर (ignore_skip_worktree) {
				ensure_full_सूचकांक(सूचकांक);
				ce = सूचकांक->cache[i];
			}
		}

		अगर (!ignore_skip_worktree && ce_skip_worktree(ce))
			जारी;
		अगर (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			जारी;
		अगर (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_लंबाई ||
		     स_भेद(prefix, ce->name, prefix_लंबाई)))
			जारी;
		अगर (last_ce && to_tempfile) {
			अगर (ce_namelen(last_ce) != ce_namelen(ce)
			    || स_भेद(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		अगर (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : शून्य,
				   शून्य) < 0)
			errs++;
		last_ce = ce;
	}
	अगर (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	वापस !!errs;
}

अटल स्थिर अक्षर * स्थिर builtin_checkout_सूचकांक_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	शून्य
};

अटल पूर्णांक option_parse_stage(स्थिर काष्ठा option *opt,
			      स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *stage = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (!म_भेद(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} अन्यथा {
		पूर्णांक ch = arg[0];
		अगर ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		अन्यथा
			die(_("stage should be between 1 and 3 or all"));
	}
	वापस 0;
}

पूर्णांक cmd_checkout_सूचकांक(पूर्णांक argc,
		       स्थिर अक्षर **argv,
		       स्थिर अक्षर *prefix,
		       काष्ठा repository *repo)
{
	पूर्णांक i;
	काष्ठा lock_file lock_file = LOCK_INIT;
	पूर्णांक all = 0;
	पूर्णांक read_from_stdin = 0;
	पूर्णांक prefix_लंबाई;
	पूर्णांक force = 0, quiet = 0, not_new = 0;
	पूर्णांक सूचकांक_opt = 0;
	पूर्णांक err = 0;
	पूर्णांक pc_workers, pc_threshold;
	काष्ठा option builtin_checkout_सूचकांक_options[] = {
		OPT_BOOL('a', "all", &all,
			N_("check out all files in the index")),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &ignore_skip_worktree,
			N_("do not skip files with skip-worktree set")),
		OPT__FORCE(&force, N_("force overwrite of existing files"), 0),
		OPT__QUIET(&quiet,
			N_("no warning for existing files and files not in index")),
		OPT_BOOL('n', "no-create", &not_new,
			N_("don't checkout new files")),
		OPT_BOOL('u', "index", &सूचकांक_opt,
			 N_("update stat information in the index file")),
		OPT_BOOL('z', शून्य, &nul_term_पंक्ति,
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
					 builtin_checkout_सूचकांक_usage,
					 builtin_checkout_सूचकांक_options);
	repo_config(repo, git_default_config, शून्य);
	prefix_लंबाई = prefix ? म_माप(prefix) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_सूचकांक = 0;

	अगर (repo_read_सूचकांक(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_सूचकांक_options,
			builtin_checkout_सूचकांक_usage, 0);
	state.istate = repo->सूचकांक;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	अगर (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = म_माप(state.base_dir);

	अगर (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	अगर (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	अगर (सूचकांक_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->सूचकांक;
		repo_hold_locked_सूचकांक(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	अगर (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	क्रम (i = 0; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];
		अक्षर *p;

		अगर (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		अगर (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_लंबाई, arg);
		err |= checkout_file(repo->सूचकांक, p, prefix);
		मुक्त(p);
	}

	अगर (read_from_stdin) {
		काष्ठा strbuf buf = STRBUF_INIT;
		काष्ठा strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		अगर (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_पंक्ति ? strbuf_getline_nul : strbuf_getline_lf;
		जबतक (getline_fn(&buf, मानक_निवेश) != खातापूर्ण) {
			अक्षर *p;
			अगर (!nul_term_पंक्ति && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				अगर (unquote_c_style(&unquoted, buf.buf, शून्य))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(repo, prefix, prefix_लंबाई, buf.buf);
			err |= checkout_file(repo->सूचकांक, p, prefix);
			मुक्त(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	अगर (all)
		err |= checkout_all(repo->सूचकांक, prefix, prefix_लंबाई);

	अगर (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     शून्य, शून्य);

	अगर (err)
		वापस 1;

	अगर (is_lock_file_locked(&lock_file) &&
	    write_locked_सूचकांक(repo->सूचकांक, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	वापस 0;
}
