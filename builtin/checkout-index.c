/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "cache-tree.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "entry.h"
#અન્તર્ભાવ "parallel-checkout.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "sparse-index.h"

#define CHECKOUT_ALL 4
સ્થિર પૂર્ણાંક nul_term_લીટી;
સ્થિર પૂર્ણાંક checkout_stage; /* default to checkout stage0 */
સ્થિર પૂર્ણાંક ignore_skip_worktree; /* default to 0 */
સ્થિર પૂર્ણાંક to_tempfile = -1;
સ્થિર અક્ષર topath[4][TEMPORARY_FILENAME_LENGTH + 1];

સ્થિર સંરચના checkout state = CHECKOUT_INIT;

સ્થિર શૂન્ય write_tempfile_record(અચલ અક્ષર *name, અચલ અક્ષર *prefix)
{
	પૂર્ણાંક i;
	પૂર્ણાંક have_tempname = 0;

	જો (CHECKOUT_ALL == checkout_stage) {
		માટે (i = 1; i < 4; i++)
			જો (topath[i][0]) {
				have_tempname = 1;
				તોડો;
			}

		જો (have_tempname) {
			માટે (i = 1; i < 4; i++) {
				જો (i > 1)
					putchar(' ');
				જો (topath[i][0])
					fputs(topath[i], stdout);
				નહીંતર
					putchar('.');
			}
		}
	} નહીંતર જો (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	જો (have_tempname) {
		putchar('\t');
		write_name_quoted_relative(name, prefix, stdout,
					   nul_term_લીટી ? '\0' : '\n');
	}

	માટે (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

સ્થિર પૂર્ણાંક checkout_file(સંરચના index_state *index, અચલ અક્ષર *name, અચલ અક્ષર *prefix)
{
	પૂર્ણાંક namelen = strlen(name);
	પૂર્ણાંક pos = index_name_pos(index, name, namelen);
	પૂર્ણાંક has_same_name = 0;
	પૂર્ણાંક is_file = 0;
	પૂર્ણાંક is_skipped = 1;
	પૂર્ણાંક did_checkout = 0;
	પૂર્ણાંક errs = 0;

	જો (pos < 0)
		pos = -pos - 1;

	જ્યારે (pos < index->cache_nr) {
		સંરચના cache_entry *ce = index->cache[pos];
		જો (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			તોડો;
		has_same_name = 1;
		pos++;
		જો (S_ISSPARSEDIR(ce->ce_mode))
			તોડો;
		is_file = 1;
		જો (!ignore_skip_worktree && ce_skip_worktree(ce))
			તોડો;
		is_skipped = 0;
		જો (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			ચાલુ;
		did_checkout = 1;
		જો (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : ખાલી,
				   ખાલી) < 0)
			errs++;
	}

	જો (did_checkout) {
		જો (to_tempfile)
			write_tempfile_record(name, prefix);
		પાછા errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	જો (has_same_name && checkout_stage == CHECKOUT_ALL)
		પાછા 0;

	જો (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		જો (!has_same_name)
			fprintf(stderr, "is not in the cache");
		નહીંતર જો (!is_file)
			fprintf(stderr, "is a sparse directory");
		નહીંતર જો (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		નહીંતર જો (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		નહીંતર
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	પાછા -1;
}

સ્થિર પૂર્ણાંક checkout_all(સંરચના index_state *index, અચલ અક્ષર *prefix, પૂર્ણાંક prefix_length)
{
	પૂર્ણાંક i, errs = 0;
	સંરચના cache_entry *last_ce = ખાલી;

	માટે (i = 0; i < index->cache_nr ; i++) {
		સંરચના cache_entry *ce = index->cache[i];

		જો (S_ISSPARSEDIR(ce->ce_mode)) {
			જો (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			જો (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		જો (!ignore_skip_worktree && ce_skip_worktree(ce))
			ચાલુ;
		જો (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			ચાલુ;
		જો (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     memcmp(prefix, ce->name, prefix_length)))
			ચાલુ;
		જો (last_ce && to_tempfile) {
			જો (ce_namelen(last_ce) != ce_namelen(ce)
			    || memcmp(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		જો (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : ખાલી,
				   ખાલી) < 0)
			errs++;
		last_ce = ce;
	}
	જો (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	પાછા !!errs;
}

સ્થિર અચલ અક્ષર * અચલ builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	ખાલી
};

સ્થિર પૂર્ણાંક option_parse_stage(અચલ સંરચના option *opt,
			      અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	પૂર્ણાંક *stage = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} નહીંતર {
		પૂર્ણાંક ch = arg[0];
		જો ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		નહીંતર
			die(_("stage should be between 1 and 3 or all"));
	}
	પાછા 0;
}

પૂર્ણાંક cmd_checkout_index(પૂર્ણાંક argc,
		       અચલ અક્ષર **argv,
		       અચલ અક્ષર *prefix,
		       સંરચના repository *repo)
{
	પૂર્ણાંક i;
	સંરચના lock_file lock_file = LOCK_INIT;
	પૂર્ણાંક all = 0;
	પૂર્ણાંક read_from_stdin = 0;
	પૂર્ણાંક prefix_length;
	પૂર્ણાંક force = 0, quiet = 0, not_new = 0;
	પૂર્ણાંક index_opt = 0;
	પૂર્ણાંક err = 0;
	પૂર્ણાંક pc_workers, pc_threshold;
	સંરચના option builtin_checkout_index_options[] = {
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
		OPT_BOOL('z', ખાલી, &nul_term_લીટી,
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
	repo_config(repo, git_default_config, ખાલી);
	prefix_length = prefix ? strlen(prefix) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	જો (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	જો (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = strlen(state.base_dir);

	જો (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	જો (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	જો (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	જો (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	માટે (i = 0; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];
		અક્ષર *p;

		જો (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		જો (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		મુક્ત(p);
	}

	જો (read_from_stdin) {
		સંરચના strbuf buf = STRBUF_INIT;
		સંરચના strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		જો (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_લીટી ? strbuf_getline_nul : strbuf_getline_lf;
		જ્યારે (getline_fn(&buf, stdin) != EOF) {
			અક્ષર *p;
			જો (!nul_term_લીટી && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				જો (unquote_c_style(&unquoted, buf.buf, ખાલી))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(repo, prefix, prefix_length, buf.buf);
			err |= checkout_file(repo->index, p, prefix);
			મુક્ત(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	જો (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	જો (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     ખાલી, ખાલી);

	જો (err)
		પાછા 1;

	જો (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	પાછા 0;
}
