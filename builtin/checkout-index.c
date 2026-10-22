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
स्थिर पूर्णाङ्क nul_term_पङ्क्ति;
स्थिर पूर्णाङ्क checkout_stage; /* default to checkout stage0 */
स्थिर पूर्णाङ्क ignore_skip_worktree; /* default to 0 */
स्थिर पूर्णाङ्क to_tempfile = -1;
स्थिर वर्ण topath[4][TEMPORARY_FILENAME_LENGTH + 1];

स्थिर संरचना checkout state = CHECKOUT_INIT;

स्थिर शून्य write_tempfile_record(अचर वर्ण *name, अचर वर्ण *prefix)
{
	पूर्णाङ्क i;
	पूर्णाङ्क have_tempname = 0;

	यदि (CHECKOUT_ALL == checkout_stage) {
		लागि (i = 1; i < 4; i++)
			यदि (topath[i][0]) {
				have_tempname = 1;
				रोक;
			}

		यदि (have_tempname) {
			लागि (i = 1; i < 4; i++) {
				यदि (i > 1)
					अक्षर_दो(' ');
				यदि (topath[i][0])
					ख_माला_दो(topath[i], मानक_निकास);
				अन्यथा
					अक्षर_दो('.');
			}
		}
	} अन्यथा यदि (topath[checkout_stage][0]) {
		have_tempname = 1;
		ख_माला_दो(topath[checkout_stage], मानक_निकास);
	}

	यदि (have_tempname) {
		अक्षर_दो('\t');
		write_name_quoted_relative(name, prefix, मानक_निकास,
					   nul_term_पङ्क्ति ? '\0' : '\n');
	}

	लागि (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

स्थिर पूर्णाङ्क checkout_file(संरचना index_state *index, अचर वर्ण *name, अचर वर्ण *prefix)
{
	पूर्णाङ्क namelen = म_माप(name);
	पूर्णाङ्क pos = index_name_pos(index, name, namelen);
	पूर्णाङ्क has_same_name = 0;
	पूर्णाङ्क is_file = 0;
	पूर्णाङ्क is_skipped = 1;
	पूर्णाङ्क did_checkout = 0;
	पूर्णाङ्क errs = 0;

	यदि (pos < 0)
		pos = -pos - 1;

	जबसम्म (pos < index->cache_nr) {
		संरचना cache_entry *ce = index->cache[pos];
		यदि (ce_namelen(ce) != namelen ||
		    स_भेद(ce->name, name, namelen))
			रोक;
		has_same_name = 1;
		pos++;
		यदि (S_ISSPARSEDIR(ce->ce_mode))
			रोक;
		is_file = 1;
		यदि (!ignore_skip_worktree && ce_skip_worktree(ce))
			रोक;
		is_skipped = 0;
		यदि (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			जारी;
		did_checkout = 1;
		यदि (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : रिक्त,
				   रिक्त) < 0)
			errs++;
	}

	यदि (did_checkout) {
		यदि (to_tempfile)
			write_tempfile_record(name, prefix);
		फर्काऊ errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	यदि (has_same_name && checkout_stage == CHECKOUT_ALL)
		फर्काऊ 0;

	यदि (!state.quiet) {
		ख_लिखो(मानक_त्रुटि, "git checkout-index: %s ", name);
		यदि (!has_same_name)
			ख_लिखो(मानक_त्रुटि, "is not in the cache");
		अन्यथा यदि (!is_file)
			ख_लिखो(मानक_त्रुटि, "is a sparse directory");
		अन्यथा यदि (is_skipped)
			ख_लिखो(मानक_त्रुटि, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		अन्यथा यदि (checkout_stage)
			ख_लिखो(मानक_त्रुटि, "does not exist at stage %d",
				checkout_stage);
		अन्यथा
			ख_लिखो(मानक_त्रुटि, "is unmerged");
		ख_अक्षर_दो('\n', मानक_त्रुटि);
	}
	फर्काऊ -1;
}

स्थिर पूर्णाङ्क checkout_all(संरचना index_state *index, अचर वर्ण *prefix, पूर्णाङ्क prefix_length)
{
	पूर्णाङ्क i, errs = 0;
	संरचना cache_entry *last_ce = रिक्त;

	लागि (i = 0; i < index->cache_nr ; i++) {
		संरचना cache_entry *ce = index->cache[i];

		यदि (S_ISSPARSEDIR(ce->ce_mode)) {
			यदि (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			यदि (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		यदि (!ignore_skip_worktree && ce_skip_worktree(ce))
			जारी;
		यदि (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			जारी;
		यदि (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     स_भेद(prefix, ce->name, prefix_length)))
			जारी;
		यदि (last_ce && to_tempfile) {
			यदि (ce_namelen(last_ce) != ce_namelen(ce)
			    || स_भेद(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		यदि (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : रिक्त,
				   रिक्त) < 0)
			errs++;
		last_ce = ce;
	}
	यदि (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	फर्काऊ !!errs;
}

स्थिर अचर वर्ण * अचर builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	रिक्त
};

स्थिर पूर्णाङ्क option_parse_stage(अचर संरचना option *opt,
			      अचर वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *stage = opt->मान;

	BUG_ON_OPT_NEG(unset);

	यदि (!म_भेद(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} अन्यथा {
		पूर्णाङ्क ch = arg[0];
		यदि ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		अन्यथा
			die(_("stage should be between 1 and 3 or all"));
	}
	फर्काऊ 0;
}

पूर्णाङ्क cmd_checkout_index(पूर्णाङ्क argc,
		       अचर वर्ण **argv,
		       अचर वर्ण *prefix,
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
		OPT_BOOL('z', रिक्त, &nul_term_पङ्क्ति,
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

	यदि (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	यदि (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = म_माप(state.base_dir);

	यदि (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	यदि (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	यदि (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	यदि (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	लागि (i = 0; i < argc; i++) {
		अचर वर्ण *arg = argv[i];
		वर्ण *p;

		यदि (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		यदि (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		मुक्त(p);
	}

	यदि (read_from_stdin) {
		संरचना strbuf buf = STRBUF_INIT;
		संरचना strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		यदि (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_पङ्क्ति ? strbuf_getline_nul : strbuf_getline_lf;
		जबसम्म (getline_fn(&buf, मानक_निवेश) != खातापूर्ण) {
			वर्ण *p;
			यदि (!nul_term_पङ्क्ति && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				यदि (unquote_c_style(&unquoted, buf.buf, रिक्त))
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

	यदि (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	यदि (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     रिक्त, रिक्त);

	यदि (err)
		फर्काऊ 1;

	यदि (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	फर्काऊ 0;
}
