/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "cache-tree.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "entry.h"
#அந்தர்பாவ "parallel-checkout.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "sparse-index.h"

#define CHECKOUT_ALL 4
நிலையான முழு nul_term_வரி;
நிலையான முழு checkout_stage; /* default to checkout stage0 */
நிலையான முழு ignore_skip_worktree; /* default to 0 */
நிலையான முழு to_tempfile = -1;
நிலையான எழுத்து topath[4][TEMPORARY_FILENAME_LENGTH + 1];

நிலையான கட்டமைப்பு checkout state = CHECKOUT_INIT;

நிலையான வெறுமை write_tempfile_record(மாறிலி எழுத்து *name, மாறிலி எழுத்து *prefix)
{
	முழு i;
	முழு have_tempname = 0;

	எனில் (CHECKOUT_ALL == checkout_stage) {
		க்கு (i = 1; i < 4; i++)
			எனில் (topath[i][0]) {
				have_tempname = 1;
				நிறுத்து;
			}

		எனில் (have_tempname) {
			க்கு (i = 1; i < 4; i++) {
				எனில் (i > 1)
					putchar(' ');
				எனில் (topath[i][0])
					fputs(topath[i], stdout);
				இல்லையெனில்
					putchar('.');
			}
		}
	} இல்லையெனில் எனில் (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	எனில் (have_tempname) {
		putchar('\t');
		write_name_quoted_relative(name, prefix, stdout,
					   nul_term_வரி ? '\0' : '\n');
	}

	க்கு (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

நிலையான முழு checkout_file(கட்டமைப்பு index_state *index, மாறிலி எழுத்து *name, மாறிலி எழுத்து *prefix)
{
	முழு namelen = strlen(name);
	முழு pos = index_name_pos(index, name, namelen);
	முழு has_same_name = 0;
	முழு is_file = 0;
	முழு is_skipped = 1;
	முழு did_checkout = 0;
	முழு errs = 0;

	எனில் (pos < 0)
		pos = -pos - 1;

	போது (pos < index->cache_nr) {
		கட்டமைப்பு cache_entry *ce = index->cache[pos];
		எனில் (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			நிறுத்து;
		has_same_name = 1;
		pos++;
		எனில் (S_ISSPARSEDIR(ce->ce_mode))
			நிறுத்து;
		is_file = 1;
		எனில் (!ignore_skip_worktree && ce_skip_worktree(ce))
			நிறுத்து;
		is_skipped = 0;
		எனில் (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			தொடர்;
		did_checkout = 1;
		எனில் (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : வெற்று,
				   வெற்று) < 0)
			errs++;
	}

	எனில் (did_checkout) {
		எனில் (to_tempfile)
			write_tempfile_record(name, prefix);
		திரும்பு errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	எனில் (has_same_name && checkout_stage == CHECKOUT_ALL)
		திரும்பு 0;

	எனில் (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		எனில் (!has_same_name)
			fprintf(stderr, "is not in the cache");
		இல்லையெனில் எனில் (!is_file)
			fprintf(stderr, "is a sparse directory");
		இல்லையெனில் எனில் (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		இல்லையெனில் எனில் (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		இல்லையெனில்
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	திரும்பு -1;
}

நிலையான முழு checkout_all(கட்டமைப்பு index_state *index, மாறிலி எழுத்து *prefix, முழு prefix_length)
{
	முழு i, errs = 0;
	கட்டமைப்பு cache_entry *last_ce = வெற்று;

	க்கு (i = 0; i < index->cache_nr ; i++) {
		கட்டமைப்பு cache_entry *ce = index->cache[i];

		எனில் (S_ISSPARSEDIR(ce->ce_mode)) {
			எனில் (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			எனில் (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		எனில் (!ignore_skip_worktree && ce_skip_worktree(ce))
			தொடர்;
		எனில் (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			தொடர்;
		எனில் (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     memcmp(prefix, ce->name, prefix_length)))
			தொடர்;
		எனில் (last_ce && to_tempfile) {
			எனில் (ce_namelen(last_ce) != ce_namelen(ce)
			    || memcmp(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		எனில் (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : வெற்று,
				   வெற்று) < 0)
			errs++;
		last_ce = ce;
	}
	எனில் (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	திரும்பு !!errs;
}

நிலையான மாறிலி எழுத்து * மாறிலி builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	வெற்று
};

நிலையான முழு option_parse_stage(மாறிலி கட்டமைப்பு option *opt,
			      மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *stage = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} இல்லையெனில் {
		முழு ch = arg[0];
		எனில் ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		இல்லையெனில்
			die(_("stage should be between 1 and 3 or all"));
	}
	திரும்பு 0;
}

முழு cmd_checkout_index(முழு argc,
		       மாறிலி எழுத்து **argv,
		       மாறிலி எழுத்து *prefix,
		       கட்டமைப்பு repository *repo)
{
	முழு i;
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	முழு all = 0;
	முழு read_from_stdin = 0;
	முழு prefix_length;
	முழு force = 0, quiet = 0, not_new = 0;
	முழு index_opt = 0;
	முழு err = 0;
	முழு pc_workers, pc_threshold;
	கட்டமைப்பு option builtin_checkout_index_options[] = {
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
		OPT_BOOL('z', வெற்று, &nul_term_வரி,
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
	repo_config(repo, git_default_config, வெற்று);
	prefix_length = prefix ? strlen(prefix) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	எனில் (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	எனில் (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = strlen(state.base_dir);

	எனில் (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	எனில் (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	எனில் (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	எனில் (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	க்கு (i = 0; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];
		எழுத்து *p;

		எனில் (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		எனில் (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		விடுவி(p);
	}

	எனில் (read_from_stdin) {
		கட்டமைப்பு strbuf buf = STRBUF_INIT;
		கட்டமைப்பு strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		எனில் (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_வரி ? strbuf_getline_nul : strbuf_getline_lf;
		போது (getline_fn(&buf, stdin) != EOF) {
			எழுத்து *p;
			எனில் (!nul_term_வரி && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				எனில் (unquote_c_style(&unquoted, buf.buf, வெற்று))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(repo, prefix, prefix_length, buf.buf);
			err |= checkout_file(repo->index, p, prefix);
			விடுவி(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	எனில் (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	எனில் (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     வெற்று, வெற்று);

	எனில் (err)
		திரும்பு 1;

	எனில் (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	திரும்பு 0;
}
