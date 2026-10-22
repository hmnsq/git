/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "cache-tree.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "entry.h"
#ଅନ୍ତର୍ଭାଵ "parallel-checkout.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"

#define CHECKOUT_ALL 4
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nul_term_ଧାଡ଼ି;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_stage; /* default to checkout stage0 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_skip_worktree; /* default to 0 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_tempfile = -1;
ସ୍ଥିର ଅକ୍ଷର topath[4][TEMPORARY_FILENAME_LENGTH + 1];

ସ୍ଥିର ସଂରଚନା checkout state = CHECKOUT_INIT;

ସ୍ଥିର ଶୂନ୍ୟ write_tempfile_record(ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *prefix)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା have_tempname = 0;

	ଯଦି (CHECKOUT_ALL == checkout_stage) {
		ପାଇଁ (i = 1; i < 4; i++)
			ଯଦି (topath[i][0]) {
				have_tempname = 1;
				ଭାଙ୍ଗ;
			}

		ଯଦି (have_tempname) {
			ପାଇଁ (i = 1; i < 4; i++) {
				ଯଦି (i > 1)
					putchar(' ');
				ଯଦି (topath[i][0])
					fputs(topath[i], stdout);
				ଅନ୍ୟଥା
					putchar('.');
			}
		}
	} ଅନ୍ୟଥା ଯଦି (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	ଯଦି (have_tempname) {
		putchar('\t');
		write_name_quoted_relative(name, prefix, stdout,
					   nul_term_ଧାଡ଼ି ? '\0' : '\n');
	}

	ପାଇଁ (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_file(ସଂରଚନା index_state *index, ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *prefix)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା namelen = strlen(name);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos = index_name_pos(index, name, namelen);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_same_name = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_file = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_skipped = 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା did_checkout = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା errs = 0;

	ଯଦି (pos < 0)
		pos = -pos - 1;

	ଯେତେବେଳେ (pos < index->cache_nr) {
		ସଂରଚନା cache_entry *ce = index->cache[pos];
		ଯଦି (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			ଭାଙ୍ଗ;
		has_same_name = 1;
		pos++;
		ଯଦି (S_ISSPARSEDIR(ce->ce_mode))
			ଭାଙ୍ଗ;
		is_file = 1;
		ଯଦି (!ignore_skip_worktree && ce_skip_worktree(ce))
			ଭାଙ୍ଗ;
		is_skipped = 0;
		ଯଦି (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			ଜାରି;
		did_checkout = 1;
		ଯଦି (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : ଖାଲି,
				   ଖାଲି) < 0)
			errs++;
	}

	ଯଦି (did_checkout) {
		ଯଦି (to_tempfile)
			write_tempfile_record(name, prefix);
		ଫେରନ୍ତୁ errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	ଯଦି (has_same_name && checkout_stage == CHECKOUT_ALL)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		ଯଦି (!has_same_name)
			fprintf(stderr, "is not in the cache");
		ଅନ୍ୟଥା ଯଦି (!is_file)
			fprintf(stderr, "is a sparse directory");
		ଅନ୍ୟଥା ଯଦି (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		ଅନ୍ୟଥା ଯଦି (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		ଅନ୍ୟଥା
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_all(ସଂରଚନା index_state *index, ଅଚଳ ଅକ୍ଷର *prefix, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prefix_length)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, errs = 0;
	ସଂରଚନା cache_entry *last_ce = ଖାଲି;

	ପାଇଁ (i = 0; i < index->cache_nr ; i++) {
		ସଂରଚନା cache_entry *ce = index->cache[i];

		ଯଦି (S_ISSPARSEDIR(ce->ce_mode)) {
			ଯଦି (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			ଯଦି (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		ଯଦି (!ignore_skip_worktree && ce_skip_worktree(ce))
			ଜାରି;
		ଯଦି (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			ଜାରି;
		ଯଦି (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     memcmp(prefix, ce->name, prefix_length)))
			ଜାରି;
		ଯଦି (last_ce && to_tempfile) {
			ଯଦି (ce_namelen(last_ce) != ce_namelen(ce)
			    || memcmp(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		ଯଦି (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : ଖାଲି,
				   ଖାଲି) < 0)
			errs++;
		last_ce = ce;
	}
	ଯଦି (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	ଫେରନ୍ତୁ !!errs;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_stage(ଅଚଳ ସଂରଚନା option *opt,
			      ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *stage = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	ଯଦି (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} ଅନ୍ୟଥା {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ch = arg[0];
		ଯଦି ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		ଅନ୍ୟଥା
			die(_("stage should be between 1 and 3 or all"));
	}
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_checkout_index(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		       ଅଚଳ ଅକ୍ଷର **argv,
		       ଅଚଳ ଅକ୍ଷର *prefix,
		       ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_from_stdin = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prefix_length;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force = 0, quiet = 0, not_new = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା index_opt = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pc_workers, pc_threshold;
	ସଂରଚନା option builtin_checkout_index_options[] = {
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
		OPT_BOOL('z', ଖାଲି, &nul_term_ଧାଡ଼ି,
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
	repo_config(repo, git_default_config, ଖାଲି);
	prefix_length = prefix ? strlen(prefix) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	ଯଦି (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	ଯଦି (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = strlen(state.base_dir);

	ଯଦି (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	ଯଦି (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	ଯଦି (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	ଯଦି (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	ପାଇଁ (i = 0; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];
		ଅକ୍ଷର *p;

		ଯଦି (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		ଯଦି (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		ମୁକ୍ତ(p);
	}

	ଯଦି (read_from_stdin) {
		ସଂରଚନା strbuf buf = STRBUF_INIT;
		ସଂରଚନା strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		ଯଦି (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_ଧାଡ଼ି ? strbuf_getline_nul : strbuf_getline_lf;
		ଯେତେବେଳେ (getline_fn(&buf, stdin) != EOF) {
			ଅକ୍ଷର *p;
			ଯଦି (!nul_term_ଧାଡ଼ି && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				ଯଦି (unquote_c_style(&unquoted, buf.buf, ଖାଲି))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(repo, prefix, prefix_length, buf.buf);
			err |= checkout_file(repo->index, p, prefix);
			ମୁକ୍ତ(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	ଯଦି (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	ଯଦି (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     ଖାଲି, ଖାଲି);

	ଯଦି (err)
		ଫେରନ୍ତୁ 1;

	ଯଦି (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	ଫେରନ୍ତୁ 0;
}
