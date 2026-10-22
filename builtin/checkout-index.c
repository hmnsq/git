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
static ཧྲིལ nul_term_line;
static ཧྲིལ checkout_stage; /* default to checkout stage0 */
static ཧྲིལ ignore_skip_worktree; /* default to 0 */
static ཧྲིལ to_tempfile = -1;
static var_na topath[4][TEMPORARY_FILENAME_LENGTH + 1];

static sa_nracanaa checkout state = CHECKOUT_INIT;

static void write_tempfile_record(const var_na *name, const var_na *prefix)
{
	ཧྲིལ i;
	ཧྲིལ have_tempname = 0;

	གལ་སྲིད (CHECKOUT_ALL == checkout_stage) {
		དོན་ལུ (i = 1; i < 4; i++)
			གལ་སྲིད (topath[i][0]) {
				have_tempname = 1;
				bha_nga;
			}

		གལ་སྲིད (have_tempname) {
			དོན་ལུ (i = 1; i < 4; i++) {
				གལ་སྲིད (i > 1)
					putchar(' ');
				གལ་སྲིད (topath[i][0])
					fputs(topath[i], stdout);
				དེ་མིན
					putchar('.');
			}
		}
	} དེ་མིན གལ་སྲིད (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	གལ་སྲིད (have_tempname) {
		putchar('\t');
		write_name_quoted_relative(name, prefix, stdout,
					   nul_term_line ? '\0' : '\n');
	}

	དོན་ལུ (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

static ཧྲིལ checkout_file(sa_nracanaa index_state *index, const var_na *name, const var_na *prefix)
{
	ཧྲིལ namelen = strlen(name);
	ཧྲིལ pos = index_name_pos(index, name, namelen);
	ཧྲིལ has_same_name = 0;
	ཧྲིལ is_file = 0;
	ཧྲིལ is_skipped = 1;
	ཧྲིལ did_checkout = 0;
	ཧྲིལ errs = 0;

	གལ་སྲིད (pos < 0)
		pos = -pos - 1;

	སྐབས (pos < index->cache_nr) {
		sa_nracanaa cache_entry *ce = index->cache[pos];
		གལ་སྲིད (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			bha_nga;
		has_same_name = 1;
		pos++;
		གལ་སྲིད (S_ISSPARSEDIR(ce->ce_mode))
			bha_nga;
		is_file = 1;
		གལ་སྲིད (!ignore_skip_worktree && ce_skip_worktree(ce))
			bha_nga;
		is_skipped = 0;
		གལ་སྲིད (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			continue;
		did_checkout = 1;
		གལ་སྲིད (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : NULL,
				   NULL) < 0)
			errs++;
	}

	གལ་སྲིད (did_checkout) {
		གལ་སྲིད (to_tempfile)
			write_tempfile_record(name, prefix);
		ལོག errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	གལ་སྲིད (has_same_name && checkout_stage == CHECKOUT_ALL)
		ལོག 0;

	གལ་སྲིད (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		གལ་སྲིད (!has_same_name)
			fprintf(stderr, "is not in the cache");
		དེ་མིན གལ་སྲིད (!is_file)
			fprintf(stderr, "is a sparse directory");
		དེ་མིན གལ་སྲིད (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		དེ་མིན གལ་སྲིད (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		དེ་མིན
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	ལོག -1;
}

static ཧྲིལ checkout_all(sa_nracanaa index_state *index, const var_na *prefix, ཧྲིལ prefix_length)
{
	ཧྲིལ i, errs = 0;
	sa_nracanaa cache_entry *last_ce = NULL;

	དོན་ལུ (i = 0; i < index->cache_nr ; i++) {
		sa_nracanaa cache_entry *ce = index->cache[i];

		གལ་སྲིད (S_ISSPARSEDIR(ce->ce_mode)) {
			གལ་སྲིད (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			གལ་སྲིད (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		གལ་སྲིད (!ignore_skip_worktree && ce_skip_worktree(ce))
			continue;
		གལ་སྲིད (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			continue;
		གལ་སྲིད (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     memcmp(prefix, ce->name, prefix_length)))
			continue;
		གལ་སྲིད (last_ce && to_tempfile) {
			གལ་སྲིད (ce_namelen(last_ce) != ce_namelen(ce)
			    || memcmp(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		གལ་སྲིད (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : NULL,
				   NULL) < 0)
			errs++;
		last_ce = ce;
	}
	གལ་སྲིད (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	ལོག !!errs;
}

static const var_na * const builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	NULL
};

static ཧྲིལ option_parse_stage(const sa_nracanaa option *opt,
			      const var_na *arg, ཧྲིལ unset)
{
	ཧྲིལ *stage = opt->value;

	BUG_ON_OPT_NEG(unset);

	གལ་སྲིད (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} དེ་མིན {
		ཧྲིལ ch = arg[0];
		གལ་སྲིད ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		དེ་མིན
			die(_("stage should be between 1 and 3 or all"));
	}
	ལོག 0;
}

ཧྲིལ cmd_checkout_index(ཧྲིལ argc,
		       const var_na **argv,
		       const var_na *prefix,
		       sa_nracanaa repository *repo)
{
	ཧྲིལ i;
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	ཧྲིལ all = 0;
	ཧྲིལ read_from_stdin = 0;
	ཧྲིལ prefix_length;
	ཧྲིལ force = 0, quiet = 0, not_new = 0;
	ཧྲིལ index_opt = 0;
	ཧྲིལ err = 0;
	ཧྲིལ pc_workers, pc_threshold;
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

	གལ་སྲིད (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	གལ་སྲིད (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = strlen(state.base_dir);

	གལ་སྲིད (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	གལ་སྲིད (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	གལ་སྲིད (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	གལ་སྲིད (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	དོན་ལུ (i = 0; i < argc; i++) {
		const var_na *arg = argv[i];
		var_na *p;

		གལ་སྲིད (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		གལ་སྲིད (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		free(p);
	}

	གལ་སྲིད (read_from_stdin) {
		sa_nracanaa strbuf buf = STRBUF_INIT;
		sa_nracanaa strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		གལ་སྲིད (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;
		སྐབས (getline_fn(&buf, stdin) != EOF) {
			var_na *p;
			གལ་སྲིད (!nul_term_line && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				གལ་སྲིད (unquote_c_style(&unquoted, buf.buf, NULL))
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

	གལ་སྲིད (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	གལ་སྲིད (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     NULL, NULL);

	གལ་སྲིད (err)
		ལོག 1;

	གལ་སྲིད (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	ལོག 0;
}
