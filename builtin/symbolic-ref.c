#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "strbuf.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ check_symref(സ്ഥിരം അക്ഷരം *HEAD, പൂർണ്ണസംഖ്യ quiet, പൂർണ്ണസംഖ്യ shorten, പൂർണ്ണസംഖ്യ recurse, പൂർണ്ണസംഖ്യ print)
{
	പൂർണ്ണസംഖ്യ resolve_flags, flag;
	സ്ഥിരം അക്ഷരം *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  HEAD, resolve_flags, ശൂന്യ, &flag);

	എങ്കിൽ (!refname)
		die("No such ref: %s", HEAD);
	അല്ലെങ്കിൽ എങ്കിൽ (!(flag & REF_ISSYMREF)) {
		എങ്കിൽ (!quiet)
			die("ref %s is not a symbolic ref", HEAD);
		അല്ലെങ്കിൽ
			തിരികെ 1;
	}
	എങ്കിൽ (print) {
		അക്ഷരം *to_free = ശൂന്യ;
		എങ്കിൽ (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		സ്വതന്ത്രം(to_free);
	}
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_symbolic_ref(പൂർണ്ണസംഖ്യ argc,
		     സ്ഥിരം അക്ഷരം **argv,
		     സ്ഥിരം അക്ഷരം *prefix,
		     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	സ്ഥിരം അക്ഷരം *സന്ദേശം = ശൂന്യ;
	ഘടന option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', ശൂന്യ, &സന്ദേശം, N_("reason"), N_("reason of the update")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ശൂന്യ);
	argc = parse_options(argc, argv, prefix, options,
			     git_symbolic_ref_usage, 0);
	എങ്കിൽ (സന്ദേശം && !*സന്ദേശം)
		die("Refusing to perform update with empty message");

	എങ്കിൽ (delete) {
		എങ്കിൽ (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		എങ്കിൽ (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		എങ്കിൽ (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		തിരികെ refs_delete_ref(get_main_ref_store(the_repository),
				       ശൂന്യ, argv[0], ശൂന്യ, REF_NO_DEREF);
	}

	തിരഞ്ഞെടുപ്പ് (argc) {
	അവസ്ഥ 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		നിർത്തുക;
	അവസ്ഥ 2:
		എങ്കിൽ (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		എങ്കിൽ (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], സന്ദേശം);
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	തിരികെ ret;
}
