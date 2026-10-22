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
static 整数 nul_term_line;
static 整数 checkout_stage; /* default to checkout stage0 */
static 整数 ignore_skip_worktree; /* default to 0 */
static 整数 to_tempfile = -1;
static char topath[4][TEMPORARY_FILENAME_LENGTH + 1];

static struct checkout state = CHECKOUT_INIT;

static void write_tempfile_record(const char *name, const char *prefix)
{
	整数 i;
	整数 have_tempname = 0;

	如果 (CHECKOUT_ALL == checkout_stage) {
		为 (i = 1; i < 4; i++)
			如果 (topath[i][0]) {
				have_tempname = 1;
				break;
			}

		如果 (have_tempname) {
			为 (i = 1; i < 4; i++) {
				如果 (i > 1)
					putchar(' ');
				如果 (topath[i][0])
					fputs(topath[i], stdout);
				否则
					putchar('.');
			}
		}
	} 否则 如果 (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	如果 (have_tempname) {
		putchar('\t');
		write_name_quoted_relative(name, prefix, stdout,
					   nul_term_line ? '\0' : '\n');
	}

	为 (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

static 整数 checkout_file(struct index_state *index, const char *name, const char *prefix)
{
	整数 namelen = strlen(name);
	整数 pos = index_name_pos(index, name, namelen);
	整数 has_same_name = 0;
	整数 is_file = 0;
	整数 is_skipped = 1;
	整数 did_checkout = 0;
	整数 errs = 0;

	如果 (pos < 0)
		pos = -pos - 1;

	当 (pos < index->cache_nr) {
		struct cache_entry *ce = index->cache[pos];
		如果 (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			break;
		has_same_name = 1;
		pos++;
		如果 (S_ISSPARSEDIR(ce->ce_mode))
			break;
		is_file = 1;
		如果 (!ignore_skip_worktree && ce_skip_worktree(ce))
			break;
		is_skipped = 0;
		如果 (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			continue;
		did_checkout = 1;
		如果 (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : NULL,
				   NULL) < 0)
			errs++;
	}

	如果 (did_checkout) {
		如果 (to_tempfile)
			write_tempfile_record(name, prefix);
		返回 errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	如果 (has_same_name && checkout_stage == CHECKOUT_ALL)
		返回 0;

	如果 (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		如果 (!has_same_name)
			fprintf(stderr, "is not in the cache");
		否则 如果 (!is_file)
			fprintf(stderr, "is a sparse directory");
		否则 如果 (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		否则 如果 (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		否则
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	返回 -1;
}

static 整数 checkout_all(struct index_state *index, const char *prefix, 整数 prefix_length)
{
	整数 i, errs = 0;
	struct cache_entry *last_ce = NULL;

	为 (i = 0; i < index->cache_nr ; i++) {
		struct cache_entry *ce = index->cache[i];

		如果 (S_ISSPARSEDIR(ce->ce_mode)) {
			如果 (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			如果 (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		如果 (!ignore_skip_worktree && ce_skip_worktree(ce))
			continue;
		如果 (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			continue;
		如果 (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     memcmp(prefix, ce->name, prefix_length)))
			continue;
		如果 (last_ce && to_tempfile) {
			如果 (ce_namelen(last_ce) != ce_namelen(ce)
			    || memcmp(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		如果 (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : NULL,
				   NULL) < 0)
			errs++;
		last_ce = ce;
	}
	如果 (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	返回 !!errs;
}

static const char * const builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	NULL
};

static 整数 option_parse_stage(const struct option *opt,
			      const char *arg, 整数 unset)
{
	整数 *stage = opt->value;

	BUG_ON_OPT_NEG(unset);

	如果 (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} 否则 {
		整数 ch = arg[0];
		如果 ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		否则
			die(_("stage should be between 1 and 3 or all"));
	}
	返回 0;
}

整数 cmd_checkout_index(整数 argc,
		       const char **argv,
		       const char *prefix,
		       struct repository *repo)
{
	整数 i;
	struct lock_file lock_file = LOCK_INIT;
	整数 all = 0;
	整数 read_from_stdin = 0;
	整数 prefix_length;
	整数 force = 0, quiet = 0, not_new = 0;
	整数 index_opt = 0;
	整数 err = 0;
	整数 pc_workers, pc_threshold;
	struct option builtin_checkout_index_options[] = {
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

	如果 (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	如果 (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = strlen(state.base_dir);

	如果 (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	如果 (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	如果 (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	如果 (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	为 (i = 0; i < argc; i++) {
		const char *arg = argv[i];
		char *p;

		如果 (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		如果 (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		free(p);
	}

	如果 (read_from_stdin) {
		struct strbuf buf = STRBUF_INIT;
		struct strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		如果 (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;
		当 (getline_fn(&buf, stdin) != EOF) {
			char *p;
			如果 (!nul_term_line && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				如果 (unquote_c_style(&unquoted, buf.buf, NULL))
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

	如果 (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	如果 (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     NULL, NULL);

	如果 (err)
		返回 1;

	如果 (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	返回 0;
}
