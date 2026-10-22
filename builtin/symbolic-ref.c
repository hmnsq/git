#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_symref(ಧ್ರುವ ವರ್ಣ *HEAD, ಪೂರ್ಣಾಙ್ಕ quiet, ಪೂರ್ಣಾಙ್ಕ shorten, ಪೂರ್ಣಾಙ್ಕ recurse, ಪೂರ್ಣಾಙ್ಕ print)
{
	ಪೂರ್ಣಾಙ್ಕ resolve_flags, flag;
	ಧ್ರುವ ವರ್ಣ *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  HEAD, resolve_flags, ರಿಕ್ತ, &flag);

	ಯದಿ (!refname)
		die("No such ref: %s", HEAD);
	ಅನ್ಯಥಾ ಯದಿ (!(flag & REF_ISSYMREF)) {
		ಯದಿ (!quiet)
			die("ref %s is not a symbolic ref", HEAD);
		ಅನ್ಯಥಾ
			ಪ್ರತ್ಯಾಗಮ 1;
	}
	ಯದಿ (print) {
		ವರ್ಣ *to_free = ರಿಕ್ತ;
		ಯದಿ (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		ಮುಕ್ತ(to_free);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_symbolic_ref(ಪೂರ್ಣಾಙ್ಕ argc,
		     ಧ್ರುವ ವರ್ಣ **argv,
		     ಧ್ರುವ ವರ್ಣ *prefix,
		     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', ರಿಕ್ತ, &ಸಂದೇಶ, N_("reason"), N_("reason of the update")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
	argc = parse_options(argc, argv, prefix, options,
			     git_symbolic_ref_usage, 0);
	ಯದಿ (ಸಂದೇಶ && !*ಸಂದೇಶ)
		die("Refusing to perform update with empty message");

	ಯದಿ (delete) {
		ಯದಿ (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		ಯದಿ (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		ಯದಿ (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		ಪ್ರತ್ಯಾಗಮ refs_delete_ref(get_main_ref_store(the_repository),
				       ರಿಕ್ತ, argv[0], ರಿಕ್ತ, REF_NO_DEREF);
	}

	ವಿಕಲ್ಪ (argc) {
	ಪಕ್ಷ 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		ಭಙ್ಗ;
	ಪಕ್ಷ 2:
		ಯದಿ (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		ಯದಿ (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], ಸಂದೇಶ);
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}
