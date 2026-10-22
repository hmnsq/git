#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "sparse-index.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം *pgm;
സ്ഥിതം പൂർണ്ണസംഖ്യ one_shot, quiet;
സ്ഥിതം പൂർണ്ണസംഖ്യ err;

സ്ഥിതം പൂർണ്ണസംഖ്യ merge_entry(പൂർണ്ണസംഖ്യ pos, സ്ഥിരം അക്ഷരം *path)
{
	പൂർണ്ണസംഖ്യ found;
	സ്ഥിരം അക്ഷരം *arguments[] = { pgm, "", "", "", path, "", "", "", ശൂന്യ };
	അക്ഷരം hexbuf[4][GIT_MAX_HEXSZ + 1];
	അക്ഷരം ownbuf[4][60];
	ഘടന child_process cmd = CHILD_PROCESS_INIT;

	എങ്കിൽ (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	ചെയ്യുക {
		സ്ഥിരം ഘടന cache_entry *ce = the_repository->index->cache[pos];
		പൂർണ്ണസംഖ്യ stage = ce_stage(ce);

		എങ്കിൽ (strcmp(ce->name, path))
			നിർത്തുക;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], വലുപ്പം(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} ആകുമ്പോൾ (++pos < the_repository->index->cache_nr);
	എങ്കിൽ (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	എങ്കിൽ (run_command(&cmd)) {
		എങ്കിൽ (one_shot)
			err++;
		അല്ലെങ്കിൽ {
			എങ്കിൽ (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	തിരികെ found;
}

സ്ഥിതം ശൂന്യം merge_one_path(സ്ഥിരം അക്ഷരം *path)
{
	പൂർണ്ണസംഖ്യ pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	എങ്കിൽ (pos < 0)
		merge_entry(-pos-1, path);
}

സ്ഥിതം ശൂന്യം merge_all(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	വേണ്ടി (i = 0; i < the_repository->index->cache_nr; i++) {
		സ്ഥിരം ഘടന cache_entry *ce = the_repository->index->cache[i];
		എങ്കിൽ (!ce_stage(ce))
			തുടരുക;
		i += merge_entry(i, ce->name)-1;
	}
}

സ്ഥിതം സ്ഥിരം അക്ഷരം usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

പൂർണ്ണസംഖ്യ cmd_merge_index(പൂർണ്ണസംഖ്യ argc,
		    സ്ഥിരം അക്ഷരം **argv,
		    സ്ഥിരം അക്ഷരം *prefix UNUSED,
		    ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	സൂചന(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	എങ്കിൽ (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	എങ്കിൽ (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	എങ്കിൽ (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	വേണ്ടി (; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];
		എങ്കിൽ (!force_file && *arg == '-') {
			എങ്കിൽ (!strcmp(arg, "--")) {
				force_file = 1;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "-a")) {
				merge_all();
				തുടരുക;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	എങ്കിൽ (err && !quiet)
		die("merge program failed");
	തിരികെ err;
}
