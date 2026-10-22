#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "strbuf.h"

நிலையான மாறிலி எழுத்து * மாறிலி git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	வெற்று
};

நிலையான முழு check_symref(மாறிலி எழுத்து *HEAD, முழு quiet, முழு shorten, முழு recurse, முழு print)
{
	முழு resolve_flags, flag;
	மாறிலி எழுத்து *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  HEAD, resolve_flags, வெற்று, &flag);

	எனில் (!refname)
		die("No such ref: %s", HEAD);
	இல்லையெனில் எனில் (!(flag & REF_ISSYMREF)) {
		எனில் (!quiet)
			die("ref %s is not a symbolic ref", HEAD);
		இல்லையெனில்
			திரும்பு 1;
	}
	எனில் (print) {
		எழுத்து *to_free = வெற்று;
		எனில் (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		விடுவி(to_free);
	}
	திரும்பு 0;
}

முழு cmd_symbolic_ref(முழு argc,
		     மாறிலி எழுத்து **argv,
		     மாறிலி எழுத்து *prefix,
		     கட்டமைப்பு repository *repo UNUSED)
{
	முழு quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	மாறிலி எழுத்து *செய்தி = வெற்று;
	கட்டமைப்பு option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', வெற்று, &செய்தி, N_("reason"), N_("reason of the update")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, வெற்று);
	argc = parse_options(argc, argv, prefix, options,
			     git_symbolic_ref_usage, 0);
	எனில் (செய்தி && !*செய்தி)
		die("Refusing to perform update with empty message");

	எனில் (delete) {
		எனில் (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		எனில் (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		எனில் (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		திரும்பு refs_delete_ref(get_main_ref_store(the_repository),
				       வெற்று, argv[0], வெற்று, REF_NO_DEREF);
	}

	தேர்வு (argc) {
	நிலை 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		நிறுத்து;
	நிலை 2:
		எனில் (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		எனில் (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], செய்தி);
		நிறுத்து;
	இயல்பு:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	திரும்பு ret;
}
