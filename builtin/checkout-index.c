/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "lockfile.h"
#شامل "quote.h"
#شامل "cache-tree.h"
#شامل "parse-options.h"
#شامل "entry.h"
#شامل "parallel-checkout.h"
#شامل "read-cache-ll.h"
#شامل "setup.h"
#شامل "sparse-index.h"

#define CHECKOUT_ALL 4
ایستا صحیح nul_term_سطر;
ایستا صحیح checkout_stage; /* default to checkout stage0 */
ایستا صحیح ignore_skip_worktree; /* default to 0 */
ایستا صحیح to_tempfile = -1;
ایستا نویسه topath[4][TEMPORARY_FILENAME_LENGTH + 1];

ایستا ساختار checkout state = CHECKOUT_INIT;

ایستا تهی write_tempfile_record(ثابت نویسه *name, ثابت نویسه *prefix)
{
	صحیح i;
	صحیح have_tempname = 0;

	اگر (CHECKOUT_ALL == checkout_stage) {
		برای (i = 1; i < 4; i++)
			اگر (topath[i][0]) {
				have_tempname = 1;
				بشکن;
			}

		اگر (have_tempname) {
			برای (i = 1; i < 4; i++) {
				اگر (i > 1)
					putchar(' ');
				اگر (topath[i][0])
					fputs(topath[i], stdout);
				وگرنه
					putchar('.');
			}
		}
	} وگرنه اگر (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	اگر (have_tempname) {
		putchar('\t');
		write_name_quoted_relative(name, prefix, stdout,
					   nul_term_سطر ? '\0' : '\n');
	}

	برای (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

ایستا صحیح checkout_file(ساختار index_state *index, ثابت نویسه *name, ثابت نویسه *prefix)
{
	صحیح namelen = strlen(name);
	صحیح pos = index_name_pos(index, name, namelen);
	صحیح has_same_name = 0;
	صحیح is_file = 0;
	صحیح is_skipped = 1;
	صحیح did_checkout = 0;
	صحیح errs = 0;

	اگر (pos < 0)
		pos = -pos - 1;

	وقتی (pos < index->cache_nr) {
		ساختار cache_entry *ce = index->cache[pos];
		اگر (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			بشکن;
		has_same_name = 1;
		pos++;
		اگر (S_ISSPARSEDIR(ce->ce_mode))
			بشکن;
		is_file = 1;
		اگر (!ignore_skip_worktree && ce_skip_worktree(ce))
			بشکن;
		is_skipped = 0;
		اگر (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			ادامه;
		did_checkout = 1;
		اگر (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : پوچ,
				   پوچ) < 0)
			errs++;
	}

	اگر (did_checkout) {
		اگر (to_tempfile)
			write_tempfile_record(name, prefix);
		بازگشت errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	اگر (has_same_name && checkout_stage == CHECKOUT_ALL)
		بازگشت 0;

	اگر (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		اگر (!has_same_name)
			fprintf(stderr, "is not in the cache");
		وگرنه اگر (!is_file)
			fprintf(stderr, "is a sparse directory");
		وگرنه اگر (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		وگرنه اگر (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		وگرنه
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	بازگشت -1;
}

ایستا صحیح checkout_all(ساختار index_state *index, ثابت نویسه *prefix, صحیح prefix_length)
{
	صحیح i, errs = 0;
	ساختار cache_entry *last_ce = پوچ;

	برای (i = 0; i < index->cache_nr ; i++) {
		ساختار cache_entry *ce = index->cache[i];

		اگر (S_ISSPARSEDIR(ce->ce_mode)) {
			اگر (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			اگر (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		اگر (!ignore_skip_worktree && ce_skip_worktree(ce))
			ادامه;
		اگر (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			ادامه;
		اگر (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     memcmp(prefix, ce->name, prefix_length)))
			ادامه;
		اگر (last_ce && to_tempfile) {
			اگر (ce_namelen(last_ce) != ce_namelen(ce)
			    || memcmp(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		اگر (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : پوچ,
				   پوچ) < 0)
			errs++;
		last_ce = ce;
	}
	اگر (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	بازگشت !!errs;
}

ایستا ثابت نویسه * ثابت builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	پوچ
};

ایستا صحیح option_parse_stage(ثابت ساختار option *opt,
			      ثابت نویسه *arg, صحیح unset)
{
	صحیح *stage = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} وگرنه {
		صحیح ch = arg[0];
		اگر ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		وگرنه
			die(_("stage should be between 1 and 3 or all"));
	}
	بازگشت 0;
}

صحیح cmd_checkout_index(صحیح argc,
		       ثابت نویسه **argv,
		       ثابت نویسه *prefix,
		       ساختار repository *repo)
{
	صحیح i;
	ساختار lock_file lock_file = LOCK_INIT;
	صحیح all = 0;
	صحیح read_from_stdin = 0;
	صحیح prefix_length;
	صحیح force = 0, quiet = 0, not_new = 0;
	صحیح index_opt = 0;
	صحیح err = 0;
	صحیح pc_workers, pc_threshold;
	ساختار option builtin_checkout_index_options[] = {
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
		OPT_BOOL('z', پوچ, &nul_term_سطر,
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
	repo_config(repo, git_default_config, پوچ);
	prefix_length = prefix ? strlen(prefix) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	اگر (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	اگر (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = strlen(state.base_dir);

	اگر (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	اگر (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	اگر (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	اگر (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	برای (i = 0; i < argc; i++) {
		ثابت نویسه *arg = argv[i];
		نویسه *p;

		اگر (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		اگر (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		آزاد(p);
	}

	اگر (read_from_stdin) {
		ساختار strbuf buf = STRBUF_INIT;
		ساختار strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		اگر (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_سطر ? strbuf_getline_nul : strbuf_getline_lf;
		وقتی (getline_fn(&buf, stdin) != EOF) {
			نویسه *p;
			اگر (!nul_term_سطر && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				اگر (unquote_c_style(&unquoted, buf.buf, پوچ))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(repo, prefix, prefix_length, buf.buf);
			err |= checkout_file(repo->index, p, prefix);
			آزاد(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	اگر (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	اگر (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     پوچ, پوچ);

	اگر (err)
		بازگشت 1;

	اگر (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	بازگشت 0;
}
