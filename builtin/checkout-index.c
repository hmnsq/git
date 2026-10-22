/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "cache-tree.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "entry.h"
#অন্তর্ভাব "parallel-checkout.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "sparse-index.h"

#define CHECKOUT_ALL 4
স্থির পূর্ণাঙ্ক nul_term_সারি;
স্থির পূর্ণাঙ্ক checkout_stage; /* default to checkout stage0 */
স্থির পূর্ণাঙ্ক ignore_skip_worktree; /* default to 0 */
স্থির পূর্ণাঙ্ক to_tempfile = -1;
স্থির বর্ণ topath[4][TEMPORARY_FILENAME_LENGTH + 1];

স্থির সংরচনা checkout state = CHECKOUT_INIT;

স্থির শূন্য write_tempfile_record(ধ্রুব বর্ণ *name, ধ্রুব বর্ণ *prefix)
{
	পূর্ণাঙ্ক i;
	পূর্ণাঙ্ক have_tempname = 0;

	যদি (CHECKOUT_ALL == checkout_stage) {
		প্রতি (i = 1; i < 4; i++)
			যদি (topath[i][0]) {
				have_tempname = 1;
				ভঙ্গ;
			}

		যদি (have_tempname) {
			প্রতি (i = 1; i < 4; i++) {
				যদি (i > 1)
					putchar(' ');
				যদি (topath[i][0])
					fputs(topath[i], stdout);
				অন্যথা
					putchar('.');
			}
		}
	} অন্যথা যদি (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	যদি (have_tempname) {
		putchar('\t');
		write_name_quoted_relative(name, prefix, stdout,
					   nul_term_সারি ? '\0' : '\n');
	}

	প্রতি (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

স্থির পূর্ণাঙ্ক checkout_file(সংরচনা index_state *index, ধ্রুব বর্ণ *name, ধ্রুব বর্ণ *prefix)
{
	পূর্ণাঙ্ক namelen = strlen(name);
	পূর্ণাঙ্ক pos = index_name_pos(index, name, namelen);
	পূর্ণাঙ্ক has_same_name = 0;
	পূর্ণাঙ্ক is_file = 0;
	পূর্ণাঙ্ক is_skipped = 1;
	পূর্ণাঙ্ক did_checkout = 0;
	পূর্ণাঙ্ক errs = 0;

	যদি (pos < 0)
		pos = -pos - 1;

	যাবত্ (pos < index->cache_nr) {
		সংরচনা cache_entry *ce = index->cache[pos];
		যদি (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			ভঙ্গ;
		has_same_name = 1;
		pos++;
		যদি (S_ISSPARSEDIR(ce->ce_mode))
			ভঙ্গ;
		is_file = 1;
		যদি (!ignore_skip_worktree && ce_skip_worktree(ce))
			ভঙ্গ;
		is_skipped = 0;
		যদি (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			অনুবর্ত;
		did_checkout = 1;
		যদি (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : রিক্ত,
				   রিক্ত) < 0)
			errs++;
	}

	যদি (did_checkout) {
		যদি (to_tempfile)
			write_tempfile_record(name, prefix);
		প্রত্যাগম errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	যদি (has_same_name && checkout_stage == CHECKOUT_ALL)
		প্রত্যাগম 0;

	যদি (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		যদি (!has_same_name)
			fprintf(stderr, "is not in the cache");
		অন্যথা যদি (!is_file)
			fprintf(stderr, "is a sparse directory");
		অন্যথা যদি (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		অন্যথা যদি (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		অন্যথা
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	প্রত্যাগম -1;
}

স্থির পূর্ণাঙ্ক checkout_all(সংরচনা index_state *index, ধ্রুব বর্ণ *prefix, পূর্ণাঙ্ক prefix_length)
{
	পূর্ণাঙ্ক i, errs = 0;
	সংরচনা cache_entry *last_ce = রিক্ত;

	প্রতি (i = 0; i < index->cache_nr ; i++) {
		সংরচনা cache_entry *ce = index->cache[i];

		যদি (S_ISSPARSEDIR(ce->ce_mode)) {
			যদি (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			যদি (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		যদি (!ignore_skip_worktree && ce_skip_worktree(ce))
			অনুবর্ত;
		যদি (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			অনুবর্ত;
		যদি (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     memcmp(prefix, ce->name, prefix_length)))
			অনুবর্ত;
		যদি (last_ce && to_tempfile) {
			যদি (ce_namelen(last_ce) != ce_namelen(ce)
			    || memcmp(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		যদি (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : রিক্ত,
				   রিক্ত) < 0)
			errs++;
		last_ce = ce;
	}
	যদি (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	প্রত্যাগম !!errs;
}

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	রিক্ত
};

স্থির পূর্ণাঙ্ক option_parse_stage(ধ্রুব সংরচনা option *opt,
			      ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	পূর্ণাঙ্ক *stage = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} অন্যথা {
		পূর্ণাঙ্ক ch = arg[0];
		যদি ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		অন্যথা
			die(_("stage should be between 1 and 3 or all"));
	}
	প্রত্যাগম 0;
}

পূর্ণাঙ্ক cmd_checkout_index(পূর্ণাঙ্ক argc,
		       ধ্রুব বর্ণ **argv,
		       ধ্রুব বর্ণ *prefix,
		       সংরচনা repository *repo)
{
	পূর্ণাঙ্ক i;
	সংরচনা lock_file lock_file = LOCK_INIT;
	পূর্ণাঙ্ক all = 0;
	পূর্ণাঙ্ক read_from_stdin = 0;
	পূর্ণাঙ্ক prefix_length;
	পূর্ণাঙ্ক force = 0, quiet = 0, not_new = 0;
	পূর্ণাঙ্ক index_opt = 0;
	পূর্ণাঙ্ক err = 0;
	পূর্ণাঙ্ক pc_workers, pc_threshold;
	সংরচনা option builtin_checkout_index_options[] = {
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
		OPT_BOOL('z', রিক্ত, &nul_term_সারি,
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
	repo_config(repo, git_default_config, রিক্ত);
	prefix_length = prefix ? strlen(prefix) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	যদি (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	যদি (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = strlen(state.base_dir);

	যদি (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	যদি (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	যদি (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	যদি (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	প্রতি (i = 0; i < argc; i++) {
		ধ্রুব বর্ণ *arg = argv[i];
		বর্ণ *p;

		যদি (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		যদি (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		মুক্ত(p);
	}

	যদি (read_from_stdin) {
		সংরচনা strbuf buf = STRBUF_INIT;
		সংরচনা strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		যদি (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_সারি ? strbuf_getline_nul : strbuf_getline_lf;
		যাবত্ (getline_fn(&buf, stdin) != EOF) {
			বর্ণ *p;
			যদি (!nul_term_সারি && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				যদি (unquote_c_style(&unquoted, buf.buf, রিক্ত))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(repo, prefix, prefix_length, buf.buf);
			err |= checkout_file(repo->index, p, prefix);
			মুক্ত(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	যদি (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	যদি (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     রিক্ত, রিক্ত);

	যদি (err)
		প্রত্যাগম 1;

	যদি (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	প্রত্যাগম 0;
}
