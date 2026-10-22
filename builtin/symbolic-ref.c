#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ check_symref(ਅਚਲ ਅੱਖਰ *ਸਿਰ_2, ਪੂਰਨ quiet, ਪੂਰਨ shorten, ਪੂਰਨ recurse, ਪੂਰਨ print)
{
	ਪੂਰਨ resolve_flags, flag;
	ਅਚਲ ਅੱਖਰ *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  ਸਿਰ_2, resolve_flags, ਨਿਰਾ, &flag);

	ਜੇ (!refname)
		die("No such ref: %s", ਸਿਰ_2);
	ਨਹੀਂਤਾਂ ਜੇ (!(flag & REF_ISSYMREF)) {
		ਜੇ (!quiet)
			die("ref %s is not a symbolic ref", ਸਿਰ_2);
		ਨਹੀਂਤਾਂ
			ਵਾਪਸ 1;
	}
	ਜੇ (print) {
		ਅੱਖਰ *ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
		ਜੇ (shorten)
			refname = ਮੰਜ਼ਿਲ_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
	}
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_symbolic_ref(ਪੂਰਨ argc,
		     ਅਚਲ ਅੱਖਰ **argv,
		     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', ਨਿਰਾ, &ਸੁਨੇਹਾ, N_("reason"), N_("reason of the update")),
		OPT_ਸਿਰਾ(),
	};

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_symbolic_ref_usage, 0);
	ਜੇ (ਸੁਨੇਹਾ && !*ਸੁਨੇਹਾ)
		die("Refusing to perform update with empty message");

	ਜੇ (delete) {
		ਜੇ (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		ਜੇ (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		ਜੇ (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		ਵਾਪਸ refs_delete_ref(get_main_ref_store(the_repository),
				       ਨਿਰਾ, argv[0], ਨਿਰਾ, REF_NO_DEREF);
	}

	ਚੋਣ (argc) {
	ਹਾਲਤ 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		ਰੋਕੋ;
	ਹਾਲਤ 2:
		ਜੇ (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		ਜੇ (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], ਸੁਨੇਹਾ);
		ਰੋਕੋ;
	ਮੂਲ:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	ਵਾਪਸ ret;
}
