/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "lockfile.h"
#include "quote.h"
#include "cache-tree.h"
#include "parse-options.h"
#include "entry.h"
#include "parallel-checkout.h"
#include "read-cache-ll.h"
#include "setup.h"
#include "sparse-index.h"

#define CHECKOUT_ALL 4
static จำนวนเต็ม nul_term_line;
static จำนวนเต็ม checkout_stage; /* default to checkout stage0 */
static จำนวนเต็ม ignore_skip_worktree; /* default to 0 */
static จำนวนเต็ม to_tempfile = -1;
static var_na topath[4][TEMPORARY_FILENAME_LENGTH + 1];

static sa_nracanaa checkout state = CHECKOUT_INIT;

static void write_tempfile_record(const var_na *name, const var_na *prefix)
{
	จำนวนเต็ม i;
	จำนวนเต็ม have_tempname = 0;

	ถ้า (CHECKOUT_ALL == checkout_stage) {
		สำหรับ (i = 1; i < 4; i++)
			ถ้า (topath[i][0]) {
				have_tempname = 1;
				bha_nga;
			}

		ถ้า (have_tempname) {
			สำหรับ (i = 1; i < 4; i++) {
				ถ้า (i > 1)
					putchar(' ');
				ถ้า (topath[i][0])
					fputs(topath[i], stdout);
				มิฉะนั้น
					putchar('.');
			}
		}
	} มิฉะนั้น ถ้า (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	ถ้า (have_tempname) {
		putchar('\t');
		write_name_quoted_relative(name, prefix, stdout,
					   nul_term_line ? '\0' : '\n');
	}

	สำหรับ (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

static จำนวนเต็ม checkout_file(sa_nracanaa index_state *index, const var_na *name, const var_na *prefix)
{
	จำนวนเต็ม namelen = strlen(name);
	จำนวนเต็ม pos = index_name_pos(index, name, namelen);
	จำนวนเต็ม has_same_name = 0;
	จำนวนเต็ม is_file = 0;
	จำนวนเต็ม is_skipped = 1;
	จำนวนเต็ม did_checkout = 0;
	จำนวนเต็ม errs = 0;

	ถ้า (pos < 0)
		pos = -pos - 1;

	ขณะ (pos < index->cache_nr) {
		sa_nracanaa cache_entry *ce = index->cache[pos];
		ถ้า (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			bha_nga;
		has_same_name = 1;
		pos++;
		ถ้า (S_ISSPARSEDIR(ce->ce_mode))
			bha_nga;
		is_file = 1;
		ถ้า (!ignore_skip_worktree && ce_skip_worktree(ce))
			bha_nga;
		is_skipped = 0;
		ถ้า (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			continue;
		did_checkout = 1;
		ถ้า (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : NULL,
				   NULL) < 0)
			errs++;
	}

	ถ้า (did_checkout) {
		ถ้า (to_tempfile)
			write_tempfile_record(name, prefix);
		คืน errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	ถ้า (has_same_name && checkout_stage == CHECKOUT_ALL)
		คืน 0;

	ถ้า (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		ถ้า (!has_same_name)
			fprintf(stderr, "is not in the cache");
		มิฉะนั้น ถ้า (!is_file)
			fprintf(stderr, "is a sparse directory");
		มิฉะนั้น ถ้า (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		มิฉะนั้น ถ้า (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		มิฉะนั้น
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	คืน -1;
}

static จำนวนเต็ม checkout_all(sa_nracanaa index_state *index, const var_na *prefix, จำนวนเต็ม prefix_length)
{
	จำนวนเต็ม i, errs = 0;
	sa_nracanaa cache_entry *last_ce = NULL;

	สำหรับ (i = 0; i < index->cache_nr ; i++) {
		sa_nracanaa cache_entry *ce = index->cache[i];

		ถ้า (S_ISSPARSEDIR(ce->ce_mode)) {
			ถ้า (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			ถ้า (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		ถ้า (!ignore_skip_worktree && ce_skip_worktree(ce))
			continue;
		ถ้า (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			continue;
		ถ้า (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     memcmp(prefix, ce->name, prefix_length)))
			continue;
		ถ้า (last_ce && to_tempfile) {
			ถ้า (ce_namelen(last_ce) != ce_namelen(ce)
			    || memcmp(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		ถ้า (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : NULL,
				   NULL) < 0)
			errs++;
		last_ce = ce;
	}
	ถ้า (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	คืน !!errs;
}

static const var_na * const builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	NULL
};

static จำนวนเต็ม option_parse_stage(const sa_nracanaa option *opt,
			      const var_na *arg, จำนวนเต็ม unset)
{
	จำนวนเต็ม *stage = opt->value;

	BUG_ON_OPT_NEG(unset);

	ถ้า (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} มิฉะนั้น {
		จำนวนเต็ม ch = arg[0];
		ถ้า ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		มิฉะนั้น
			die(_("stage should be between 1 and 3 or all"));
	}
	คืน 0;
}

จำนวนเต็ม cmd_checkout_index(จำนวนเต็ม argc,
		       const var_na **argv,
		       const var_na *prefix,
		       sa_nracanaa repository *repo)
{
	จำนวนเต็ม i;
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	จำนวนเต็ม all = 0;
	จำนวนเต็ม read_from_stdin = 0;
	จำนวนเต็ม prefix_length;
	จำนวนเต็ม force = 0, quiet = 0, not_new = 0;
	จำนวนเต็ม index_opt = 0;
	จำนวนเต็ม err = 0;
	จำนวนเต็ม pc_workers, pc_threshold;
	sa_nracanaa option builtin_checkout_index_options[] = {
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
		OPT_BOOL('z', NULL, &nul_term_line,
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
	repo_config(repo, git_default_config, NULL);
	prefix_length = prefix ? strlen(prefix) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	ถ้า (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	ถ้า (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = strlen(state.base_dir);

	ถ้า (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	ถ้า (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	ถ้า (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	ถ้า (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	สำหรับ (i = 0; i < argc; i++) {
		const var_na *arg = argv[i];
		var_na *p;

		ถ้า (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		ถ้า (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		free(p);
	}

	ถ้า (read_from_stdin) {
		sa_nracanaa strbuf buf = STRBUF_INIT;
		sa_nracanaa strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		ถ้า (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;
		ขณะ (getline_fn(&buf, stdin) != EOF) {
			var_na *p;
			ถ้า (!nul_term_line && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				ถ้า (unquote_c_style(&unquoted, buf.buf, NULL))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(repo, prefix, prefix_length, buf.buf);
			err |= checkout_file(repo->index, p, prefix);
			free(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	ถ้า (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	ถ้า (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     NULL, NULL);

	ถ้า (err)
		คืน 1;

	ถ้า (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	คืน 0;
}
