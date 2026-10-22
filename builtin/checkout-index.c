/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

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

#परिभाषय CHECKOUT_ALL 4
स्थिर पूर्णाङ्क nul_पदांशः_पङ्क्तिः;
स्थिर पूर्णाङ्क checkout_stage; /* default to checkout stage0 */
स्थिर पूर्णाङ्क ignore_skip_worktree; /* default to 0 */
स्थिर पूर्णाङ्क गन्तव्यम्_tempfile = -1;
स्थिर वर्ण topath[4][TEMPORARY_FILENAME_दीर्घता + 1];

स्थिर संरचना checkout state = CHECKOUT_INIT;

स्थिर शून्य write_tempfile_लेखः(ध्रुव वर्ण *name, ध्रुव वर्ण *उपसर्गः)
{
	पूर्णाङ्क i;
	पूर्णाङ्क have_tempname = 0;

	यदि (CHECKOUT_ALL == checkout_stage) {
		प्रति (i = 1; i < 4; i++)
			यदि (topath[i][0]) {
				have_tempname = 1;
				भङ्ग;
			}

		यदि (have_tempname) {
			प्रति (i = 1; i < 4; i++) {
				यदि (i > 1)
					अक्षरं_लिख(' ');
				यदि (topath[i][0])
					fputs(topath[i], stdout);
				अन्यथा
					अक्षरं_लिख('.');
			}
		}
	} अन्यथा यदि (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	यदि (have_tempname) {
		अक्षरं_लिख('\t');
		write_name_quoted_relative(name, उपसर्गः, stdout,
					   nul_पदांशः_पङ्क्तिः ? '\0' : '\n');
	}

	प्रति (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

स्थिर पूर्णाङ्क checkout_file(संरचना सूचकांक_state *सूचकांक, ध्रुव वर्ण *name, ध्रुव वर्ण *उपसर्गः)
{
	पूर्णाङ्क namelen = वाक्यदैर्घ्यम्(name);
	पूर्णाङ्क pos = सूचकांक_name_pos(सूचकांक, name, namelen);
	पूर्णाङ्क has_same_name = 0;
	पूर्णाङ्क is_file = 0;
	पूर्णाङ्क is_skipped = 1;
	पूर्णाङ्क did_checkout = 0;
	पूर्णाङ्क errs = 0;

	यदि (pos < 0)
		pos = -pos - 1;

	यावत् (pos < सूचकांक->cache_nr) {
		संरचना cache_प्रविष्टिः *ce = सूचकांक->cache[pos];
		यदि (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			भङ्ग;
		has_same_name = 1;
		pos++;
		यदि (S_ISSPARSEDIR(ce->ce_बहुलकम्))
			भङ्ग;
		is_file = 1;
		यदि (!ignore_skip_worktree && ce_skip_worktree(ce))
			भङ्ग;
		is_skipped = 0;
		यदि (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			अनुवर्त;
		did_checkout = 1;
		यदि (checkout_प्रविष्टिः(ce, &state,
				   गन्तव्यम्_tempfile ? topath[ce_stage(ce)] : रिक्त,
				   रिक्त) < 0)
			errs++;
	}

	यदि (did_checkout) {
		यदि (गन्तव्यम्_tempfile)
			write_tempfile_लेखः(name, उपसर्गः);
		प्रत्यागम errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	यदि (has_same_name && checkout_stage == CHECKOUT_ALL)
		प्रत्यागम 0;

	यदि (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		यदि (!has_same_name)
			fprintf(stderr, "is not in the cache");
		अन्यथा यदि (!is_file)
			fprintf(stderr, "is a sparse directory");
		अन्यथा यदि (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		अन्यथा यदि (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		अन्यथा
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	प्रत्यागम -1;
}

स्थिर पूर्णाङ्क checkout_all(संरचना सूचकांक_state *सूचकांक, ध्रुव वर्ण *उपसर्गः, पूर्णाङ्क उपसर्गः_दीर्घता)
{
	पूर्णाङ्क i, errs = 0;
	संरचना cache_प्रविष्टिः *चरमः_ce = रिक्त;

	प्रति (i = 0; i < सूचकांक->cache_nr ; i++) {
		संरचना cache_प्रविष्टिः *ce = सूचकांक->cache[i];

		यदि (S_ISSPARSEDIR(ce->ce_बहुलकम्)) {
			यदि (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			यदि (ignore_skip_worktree) {
				ensure_पूर्णम्_सूचकांक(सूचकांक);
				ce = सूचकांक->cache[i];
			}
		}

		यदि (!ignore_skip_worktree && ce_skip_worktree(ce))
			अनुवर्त;
		यदि (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			अनुवर्त;
		यदि (उपसर्गः && *उपसर्गः &&
		    (ce_namelen(ce) <= उपसर्गः_दीर्घता ||
		     memcmp(उपसर्गः, ce->name, उपसर्गः_दीर्घता)))
			अनुवर्त;
		यदि (चरमः_ce && गन्तव्यम्_tempfile) {
			यदि (ce_namelen(चरमः_ce) != ce_namelen(ce)
			    || memcmp(चरमः_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_लेखः(चरमः_ce->name, उपसर्गः);
		}
		यदि (checkout_प्रविष्टिः(ce, &state,
				   गन्तव्यम्_tempfile ? topath[ce_stage(ce)] : रिक्त,
				   रिक्त) < 0)
			errs++;
		चरमः_ce = ce;
	}
	यदि (चरमः_ce && गन्तव्यम्_tempfile)
		write_tempfile_लेखः(चरमः_ce->name, उपसर्गः);
	प्रत्यागम !!errs;
}

स्थिर ध्रुव वर्ण * ध्रुव builtin_checkout_सूचकांक_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	रिक्त
};

स्थिर पूर्णाङ्क option_parse_stage(ध्रुव संरचना option *opt,
			      ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *stage = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	यदि (!वाक्यं_तुलय(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} अन्यथा {
		पूर्णाङ्क ch = arg[0];
		यदि ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		अन्यथा
			die(_("stage should be between 1 and 3 or all"));
	}
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_checkout_सूचकांक(पूर्णाङ्क argc,
		       ध्रुव वर्ण **argv,
		       ध्रुव वर्ण *उपसर्गः,
		       संरचना repository *repo)
{
	पूर्णाङ्क i;
	संरचना तालकम्_file तालकम्_file = तालकम्_INIT;
	पूर्णाङ्क all = 0;
	पूर्णाङ्क read_from_stdin = 0;
	पूर्णाङ्क उपसर्गः_दीर्घता;
	पूर्णाङ्क force = 0, quiet = 0, not_new = 0;
	पूर्णाङ्क सूचकांक_opt = 0;
	पूर्णाङ्क err = 0;
	पूर्णाङ्क pc_workers, pc_देहली;
	संरचना option builtin_checkout_सूचकांक_options[] = {
		OPT_BOOL('a', "all", &all,
			N_("check out all files in the index")),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &ignore_skip_worktree,
			N_("do not skip files with skip-worktree set")),
		OPT__FORCE(&force, N_("force overwrite of existing files"), 0),
		OPT__QUIET(&quiet,
			N_("no warning for existing files and files not in index")),
		OPT_BOOL('n', "no-create", &not_new,
			N_("don't checkout new files")),
		OPT_BOOL('u', "index", &सूचकांक_opt,
			 N_("update stat information in the index file")),
		OPT_BOOL('z', रिक्त, &nul_पदांशः_पङ्क्तिः,
			N_("paths are separated with NUL character")),
		OPT_BOOL(0, "stdin", &read_from_stdin,
			N_("read list of paths from the standard input")),
		OPT_BOOL(0, "temp", &गन्तव्यम्_tempfile,
			N_("write the content to temporary files")),
		OPT_STRING(0, "prefix", &state.आधारः_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_CALLBACK_F(0, "stage", &checkout_stage, "(1|2|3|all)",
			N_("copy out the files from named stage"),
			PARSE_OPT_NONEG, option_parse_stage),
		OPT_अन्तः()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_checkout_सूचकांक_usage,
					 builtin_checkout_सूचकांक_options);
	repo_config(repo, git_default_config, रिक्त);
	उपसर्गः_दीर्घता = उपसर्गः ? वाक्यदैर्घ्यम्(उपसर्गः) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_पूर्णम्_सूचकांक = 0;

	यदि (repo_read_सूचकांक(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, उपसर्गः, builtin_checkout_सूचकांक_options,
			builtin_checkout_सूचकांक_usage, 0);
	state.istate = repo->सूचकांक;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	यदि (!state.आधारः_dir)
		state.आधारः_dir = "";
	state.आधारः_dir_len = वाक्यदैर्घ्यम्(state.आधारः_dir);

	यदि (गन्तव्यम्_tempfile < 0)
		गन्तव्यम्_tempfile = (checkout_stage == CHECKOUT_ALL);
	यदि (!गन्तव्यम्_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	यदि (सूचकांक_opt && !state.आधारः_dir_len && !गन्तव्यम्_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->सूचकांक;
		repo_hold_locked_सूचकांक(repo, &तालकम्_file,
				       तालकम्_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_देहली);
	यदि (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	प्रति (i = 0; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		वर्ण *p;

		यदि (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		यदि (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = उपसर्गः_मार्गः(repo, उपसर्गः, उपसर्गः_दीर्घता, arg);
		err |= checkout_file(repo->सूचकांक, p, उपसर्गः);
		मुक्त(p);
	}

	यदि (read_from_stdin) {
		संरचना strbuf buf = STRBUF_INIT;
		संरचना strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		यदि (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_पदांशः_पङ्क्तिः ? strbuf_getline_nul : strbuf_getline_lf;
		यावत् (getline_fn(&buf, stdin) != EOF) {
			वर्ण *p;
			यदि (!nul_पदांशः_पङ्क्तिः && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				यदि (unquote_c_style(&unquoted, buf.buf, रिक्त))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = उपसर्गः_मार्गः(repo, उपसर्गः, उपसर्गः_दीर्घता, buf.buf);
			err |= checkout_file(repo->सूचकांक, p, उपसर्गः);
			मुक्त(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	यदि (all)
		err |= checkout_all(repo->सूचकांक, उपसर्गः, उपसर्गः_दीर्घता);

	यदि (pc_workers > 1)
		err |= सततम्_parallel_checkout(&state, pc_workers, pc_देहली,
					     रिक्त, रिक्त);

	यदि (err)
		प्रत्यागम 1;

	यदि (is_तालकम्_file_locked(&तालकम्_file) &&
	    write_locked_सूचकांक(repo->सूचकांक, &तालकम्_file, COMMIT_तालकम्))
		die("Unable to write new index file");
	प्रत्यागम 0;
}
