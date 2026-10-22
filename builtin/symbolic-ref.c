#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_symref(ଅଚଳ ଅକ୍ଷର *HEAD, ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet, ପୂର୍ଣ୍ଣସଂଖ୍ୟା shorten, ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse, ପୂର୍ଣ୍ଣସଂଖ୍ୟା print)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା resolve_flags, flag;
	ଅଚଳ ଅକ୍ଷର *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  HEAD, resolve_flags, ଖାଲି, &flag);

	ଯଦି (!refname)
		die("No such ref: %s", HEAD);
	ଅନ୍ୟଥା ଯଦି (!(flag & REF_ISSYMREF)) {
		ଯଦି (!quiet)
			die("ref %s is not a symbolic ref", HEAD);
		ଅନ୍ୟଥା
			ଫେରନ୍ତୁ 1;
	}
	ଯଦି (print) {
		ଅକ୍ଷର *to_free = ଖାଲି;
		ଯଦି (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		ମୁକ୍ତ(to_free);
	}
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_symbolic_ref(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		     ଅଚଳ ଅକ୍ଷର **argv,
		     ଅଚଳ ଅକ୍ଷର *prefix,
		     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ = ଖାଲି;
	ସଂରଚନା option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', ଖାଲି, &ସନ୍ଦେଶ, N_("reason"), N_("reason of the update")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ଖାଲି);
	argc = parse_options(argc, argv, prefix, options,
			     git_symbolic_ref_usage, 0);
	ଯଦି (ସନ୍ଦେଶ && !*ସନ୍ଦେଶ)
		die("Refusing to perform update with empty message");

	ଯଦି (delete) {
		ଯଦି (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		ଯଦି (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		ଯଦି (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		ଫେରନ୍ତୁ refs_delete_ref(get_main_ref_store(the_repository),
				       ଖାଲି, argv[0], ଖାଲି, REF_NO_DEREF);
	}

	ବାଛ (argc) {
	ସ୍ଥିତି 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି 2:
		ଯଦି (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		ଯଦି (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], ସନ୍ଦେଶ);
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	ଫେରନ୍ତୁ ret;
}
