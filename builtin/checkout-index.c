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
static ціле nul_term_line;
static ціле checkout_stage; /* default to checkout stage0 */
static ціле ignore_skip_worktree; /* default to 0 */
static ціле to_tempfile = -1;
static char topath[4][TEMPORARY_FILENAME_LENGTH + 1];

static struct checkout state = CHECKOUT_INIT;

static void write_tempfile_record(const char *name, const char *prefix)
{
	ціле i;
	ціле have_tempname = 0;

	якщо (CHECKOUT_ALL == checkout_stage) {
		для (i = 1; i < 4; i++)
			якщо (topath[i][0]) {
				have_tempname = 1;
				break;
			}

		якщо (have_tempname) {
			для (i = 1; i < 4; i++) {
				якщо (i > 1)
					putchar(' ');
				якщо (topath[i][0])
					fputs(topath[i], stdout);
				інакше
					putchar('.');
			}
		}
	} інакше якщо (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	якщо (have_tempname) {
		putchar('\t');
		write_name_quoted_relative(name, prefix, stdout,
					   nul_term_line ? '\0' : '\n');
	}

	для (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

static ціле checkout_file(struct index_state *index, const char *name, const char *prefix)
{
	ціле namelen = strlen(name);
	ціле pos = index_name_pos(index, name, namelen);
	ціле has_same_name = 0;
	ціле is_file = 0;
	ціле is_skipped = 1;
	ціле did_checkout = 0;
	ціле errs = 0;

	якщо (pos < 0)
		pos = -pos - 1;

	поки (pos < index->cache_nr) {
		struct cache_entry *ce = index->cache[pos];
		якщо (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			break;
		has_same_name = 1;
		pos++;
		якщо (S_ISSPARSEDIR(ce->ce_mode))
			break;
		is_file = 1;
		якщо (!ignore_skip_worktree && ce_skip_worktree(ce))
			break;
		is_skipped = 0;
		якщо (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			continue;
		did_checkout = 1;
		якщо (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : NULL,
				   NULL) < 0)
			errs++;
	}

	якщо (did_checkout) {
		якщо (to_tempfile)
			write_tempfile_record(name, prefix);
		повернути errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	якщо (has_same_name && checkout_stage == CHECKOUT_ALL)
		повернути 0;

	якщо (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		якщо (!has_same_name)
			fprintf(stderr, "is not in the cache");
		інакше якщо (!is_file)
			fprintf(stderr, "is a sparse directory");
		інакше якщо (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		інакше якщо (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		інакше
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	повернути -1;
}

static ціле checkout_all(struct index_state *index, const char *prefix, ціле prefix_length)
{
	ціле i, errs = 0;
	struct cache_entry *last_ce = NULL;

	для (i = 0; i < index->cache_nr ; i++) {
		struct cache_entry *ce = index->cache[i];

		якщо (S_ISSPARSEDIR(ce->ce_mode)) {
			якщо (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			якщо (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		якщо (!ignore_skip_worktree && ce_skip_worktree(ce))
			continue;
		якщо (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			continue;
		якщо (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     memcmp(prefix, ce->name, prefix_length)))
			continue;
		якщо (last_ce && to_tempfile) {
			якщо (ce_namelen(last_ce) != ce_namelen(ce)
			    || memcmp(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		якщо (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : NULL,
				   NULL) < 0)
			errs++;
		last_ce = ce;
	}
	якщо (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	повернути !!errs;
}

static const char * const builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	NULL
};

static ціле option_parse_stage(const struct option *opt,
			      const char *arg, ціле unset)
{
	ціле *stage = opt->value;

	BUG_ON_OPT_NEG(unset);

	якщо (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} інакше {
		ціле ch = arg[0];
		якщо ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		інакше
			die(_("stage should be between 1 and 3 or all"));
	}
	повернути 0;
}

ціле cmd_checkout_index(ціле argc,
		       const char **argv,
		       const char *prefix,
		       struct repository *repo)
{
	ціле i;
	struct lock_file lock_file = LOCK_INIT;
	ціле all = 0;
	ціле read_from_stdin = 0;
	ціле prefix_length;
	ціле force = 0, quiet = 0, not_new = 0;
	ціле index_opt = 0;
	ціле err = 0;
	ціле pc_workers, pc_threshold;
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

	якщо (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	якщо (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = strlen(state.base_dir);

	якщо (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	якщо (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	якщо (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	якщо (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	для (i = 0; i < argc; i++) {
		const char *arg = argv[i];
		char *p;

		якщо (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		якщо (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		free(p);
	}

	якщо (read_from_stdin) {
		struct strbuf buf = STRBUF_INIT;
		struct strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		якщо (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;
		поки (getline_fn(&buf, stdin) != EOF) {
			char *p;
			якщо (!nul_term_line && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				якщо (unquote_c_style(&unquoted, buf.buf, NULL))
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

	якщо (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	якщо (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     NULL, NULL);

	якщо (err)
		повернути 1;

	якщо (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	повернути 0;
}
