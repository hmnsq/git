#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_symref(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_2, ಪೂರ್ಣಾಂಕ quiet, ಪೂರ್ಣಾಂಕ shorten, ಪೂರ್ಣಾಂಕ recurse, ಪೂರ್ಣಾಂಕ print)
{
	ಪೂರ್ಣಾಂಕ resolve_flags, flag;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  ತಲೆ_2, resolve_flags, ಖಾಲಿ, &flag);

	ಆದರೆ (!refname)
		die("No such ref: %s", ತಲೆ_2);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(flag & REF_ISSYMREF)) {
		ಆದರೆ (!quiet)
			die("ref %s is not a symbolic ref", ತಲೆ_2);
		ಇಲ್ಲದಿದ್ದರೆ
			ಹಿಂತಿರುಗಿ 1;
	}
	ಆದರೆ (print) {
		ಅಕ್ಷರ *ಗಮ್ಯ_free = ಖಾಲಿ;
		ಆದರೆ (shorten)
			refname = ಗಮ್ಯ_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_symbolic_ref(ಪೂರ್ಣಾಂಕ argc,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', ಖಾಲಿ, &ಸಂದೇಶ, N_("reason"), N_("reason of the update")),
		OPT_ಕೊನೆ(),
	};

	repo_config(the_repository, git_default_config, ಖಾಲಿ);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_symbolic_ref_usage, 0);
	ಆದರೆ (ಸಂದೇಶ && !*ಸಂದೇಶ)
		die("Refusing to perform update with empty message");

	ಆದರೆ (delete) {
		ಆದರೆ (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		ಆದರೆ (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		ಆದರೆ (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		ಹಿಂತಿರುಗಿ refs_delete_ref(get_main_ref_store(the_repository),
				       ಖಾಲಿ, argv[0], ಖಾಲಿ, REF_NO_DEREF);
	}

	ಆಯ್ಕೆ (argc) {
	ಸ್ಥಿತಿ 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ 2:
		ಆದರೆ (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		ಆದರೆ (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], ಸಂದೇಶ);
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	ಹಿಂತಿರುಗಿ ret;
}
