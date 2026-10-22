#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "sparse-index.h"

સ્થિર અચલ અક્ષર *pgm;
સ્થિર પૂર્ણાંક one_shot, quiet;
સ્થિર પૂર્ણાંક err;

સ્થિર પૂર્ણાંક merge_entry(પૂર્ણાંક pos, અચલ અક્ષર *path)
{
	પૂર્ણાંક found;
	અચલ અક્ષર *arguments[] = { pgm, "", "", "", path, "", "", "", ખાલી };
	અક્ષર hexbuf[4][GIT_MAX_HEXSZ + 1];
	અક્ષર ownbuf[4][60];
	સંરચના child_process cmd = CHILD_PROCESS_INIT;

	જો (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	કરો {
		અચલ સંરચના cache_entry *ce = the_repository->index->cache[pos];
		પૂર્ણાંક stage = ce_stage(ce);

		જો (strcmp(ce->name, path))
			તોડો;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], કદ(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} જ્યારે (++pos < the_repository->index->cache_nr);
	જો (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	જો (run_command(&cmd)) {
		જો (one_shot)
			err++;
		નહીંતર {
			જો (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	પાછા found;
}

સ્થિર શૂન્ય merge_one_path(અચલ અક્ષર *path)
{
	પૂર્ણાંક pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	જો (pos < 0)
		merge_entry(-pos-1, path);
}

સ્થિર શૂન્ય merge_all(શૂન્ય)
{
	પૂર્ણાંક i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	માટે (i = 0; i < the_repository->index->cache_nr; i++) {
		અચલ સંરચના cache_entry *ce = the_repository->index->cache[i];
		જો (!ce_stage(ce))
			ચાલુ;
		i += merge_entry(i, ce->name)-1;
	}
}

સ્થિર અચલ અક્ષર usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

પૂર્ણાંક cmd_merge_index(પૂર્ણાંક argc,
		    અચલ અક્ષર **argv,
		    અચલ અક્ષર *prefix UNUSED,
		    સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	સંકેત(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	જો (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	જો (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	જો (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	માટે (; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];
		જો (!force_file && *arg == '-') {
			જો (!strcmp(arg, "--")) {
				force_file = 1;
				ચાલુ;
			}
			જો (!strcmp(arg, "-a")) {
				merge_all();
				ચાલુ;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	જો (err && !quiet)
		die("merge program failed");
	પાછા err;
}
