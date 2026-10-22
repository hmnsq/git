#परिभाषय USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "pathspec.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "write-or-die.h"

स्थिर पूर्णाङ्क quiet, verbose, stdin_paths, show_non_matching, no_सूचकांक;
स्थिर ध्रुव वर्ण * ध्रुव check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
रिक्त
};

स्थिर पूर्णाङ्क nul_पदांशः_पङ्क्तिः;

स्थिर ध्रुव संरचना option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_गणः(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', रिक्त, &nul_पदांशः_पङ्क्तिः,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_सूचकांक,
		 N_("ignore index when checking")),
	OPT_अन्तः()
};

स्थिर शून्य output_आदर्शः(ध्रुव वर्ण *मार्गः, संरचना मार्गः_आदर्शः *आदर्शः)
{
	ध्रुव वर्ण *bang  = (आदर्शः && आदर्शः->flags & आदर्शः_FLAG_NEGATIVE)  ? "!" : "";
	ध्रुव वर्ण *slash = (आदर्शः && आदर्शः->flags & आदर्शः_FLAG_MUSTBEDIR) ? "/" : "";
	यदि (!nul_पदांशः_पङ्क्तिः) {
		यदि (!verbose) {
			write_name_quoted(मार्गः, stdout, '\n');
		} अन्यथा {
			यदि (आदर्शः) {
				quote_c_style(आदर्शः->pl->src, रिक्त, stdout, 0);
				प्रारूप_लिख(":%d:%s%s%s\t",
				       आदर्शः->srcpos,
				       bang, आदर्शः->आदर्शः, slash);
			}
			अन्यथा {
				प्रारूप_लिख("::\t");
			}
			quote_c_style(मार्गः, रिक्त, stdout, 0);
			fputc('\n', stdout);
		}
	} अन्यथा {
		यदि (!verbose) {
			प्रारूप_लिख("%s%c", मार्गः, '\0');
		} अन्यथा {
			यदि (आदर्शः)
				प्रारूप_लिख("%s%c%d%c%s%s%s%c%s%c",
				       आदर्शः->pl->src, '\0',
				       आदर्शः->srcpos, '\0',
				       bang, आदर्शः->आदर्शः, slash, '\0',
				       मार्गः, '\0');
			अन्यथा
				प्रारूप_लिख("%c%c%c%s%c", '\0', '\0', '\0', मार्गः, '\0');
		}
	}
}

स्थिर पूर्णाङ्क check_ignore(संरचना dir_struct *dir,
			ध्रुव वर्ण *उपसर्गः, पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	ध्रुव वर्ण *पूर्णम्_मार्गः;
	वर्ण *आलोकितम्;
	पूर्णाङ्क अङ्कः_ignored = 0, i;
	संरचना मार्गः_आदर्शः *आदर्शः;
	संरचना pathspec pathspec;

	यदि (!argc) {
		यदि (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		प्रत्यागम 0;
	}

	/*
	 * check-ignore just needs paths. Magic beyond :/ is really
	 * irrelevant.
	 */
	parse_pathspec(&pathspec,
		       PATHSPEC_ALL_MAGIC & ~PATHSPEC_FROMTOP,
		       PATHSPEC_SYMLINK_LEADING_मार्गः |
		       PATHSPEC_KEEP_क्रमः,
		       उपसर्गः, argv);

	die_मार्गः_inside_submodule(the_repository->सूचकांक, &pathspec);

	/*
	 * look for pathspecs matching entries in the index, since these
	 * should not be ignored, in order to be consistent with
	 * 'git status', 'git add' etc.
	 */
	आलोकितम् = find_pathspecs_matching_against_सूचकांक(&pathspec, the_repository->सूचकांक,
						     PS_HEED_SKIP_WORKTREE);
	प्रति (i = 0; i < pathspec.nr; i++) {
		पूर्णम्_मार्गः = pathspec.वस्तूनि[i].मेलनम्;
		आदर्शः = रिक्त;
		यदि (!आलोकितम्[i]) {
			पूर्णाङ्क dtype = DT_UNKNOWN;
			आदर्शः = चरमः_matching_आदर्शः(dir, the_repository->सूचकांक,
							पूर्णम्_मार्गः, &dtype);
			यदि (!verbose && आदर्शः &&
			    आदर्शः->flags & आदर्शः_FLAG_NEGATIVE)
				आदर्शः = रिक्त;
		}
		यदि (!quiet && (आदर्शः || show_non_matching))
			output_आदर्शः(pathspec.वस्तूनि[i].original, आदर्शः);
		यदि (आदर्शः)
			अङ्कः_ignored++;
	}
	मुक्त(आलोकितम्);
	clear_pathspec(&pathspec);

	प्रत्यागम अङ्कः_ignored;
}

स्थिर पूर्णाङ्क check_ignore_stdin_paths(संरचना dir_struct *dir, ध्रुव वर्ण *उपसर्गः)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf unquoted = STRBUF_INIT;
	वर्ण *pathspec[2] = { रिक्त, रिक्त };
	strbuf_getline_fn getline_fn;
	पूर्णाङ्क अङ्कः_ignored = 0;

	getline_fn = nul_पदांशः_पङ्क्तिः ? strbuf_getline_nul : strbuf_getline_lf;
	यावत् (getline_fn(&buf, stdin) != EOF) {
		यदि (!nul_पदांशः_पङ्क्तिः && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			यदि (unquote_c_style(&unquoted, buf.buf, रिक्त))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		अङ्कः_ignored += check_ignore(dir, उपसर्गः,
					    1, (ध्रुव वर्ण **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	प्रत्यागम अङ्कः_ignored;
}

पूर्णाङ्क cmd_check_ignore(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *उपसर्गः,
		     संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क अङ्कः_ignored;
	संरचना dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, उपसर्गः, check_ignore_options,
			     check_ignore_usage, 0);

	यदि (stdin_paths) {
		यदि (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} अन्यथा {
		यदि (nul_पदांशः_पङ्क्तिः)
			die(_("-z only makes sense with --stdin"));
		यदि (argc == 0)
			die(_("no path specified"));
	}
	यदि (quiet) {
		यदि (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		यदि (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	यदि (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	यदि (!no_सूचकांक && repo_read_सूचकांक(the_repository) < 0)
		die(_("index file corrupt"));

	सज्जता_standard_excludes(&dir);

	यदि (stdin_paths) {
		अङ्कः_ignored = check_ignore_stdin_paths(&dir, उपसर्गः);
	} अन्यथा {
		अङ्कः_ignored = check_ignore(&dir, उपसर्गः, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	प्रत्यागम !अङ्कः_ignored;
}
