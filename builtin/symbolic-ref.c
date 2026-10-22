#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "strbuf.h"

સ્થિર અચલ અક્ષર * અચલ git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	ખાલી
};

સ્થિર પૂર્ણાંક check_symref(અચલ અક્ષર *HEAD, પૂર્ણાંક quiet, પૂર્ણાંક shorten, પૂર્ણાંક recurse, પૂર્ણાંક print)
{
	પૂર્ણાંક resolve_flags, flag;
	અચલ અક્ષર *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  HEAD, resolve_flags, ખાલી, &flag);

	જો (!refname)
		die("No such ref: %s", HEAD);
	નહીંતર જો (!(flag & REF_ISSYMREF)) {
		જો (!quiet)
			die("ref %s is not a symbolic ref", HEAD);
		નહીંતર
			પાછા 1;
	}
	જો (print) {
		અક્ષર *to_free = ખાલી;
		જો (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		મુક્ત(to_free);
	}
	પાછા 0;
}

પૂર્ણાંક cmd_symbolic_ref(પૂર્ણાંક argc,
		     અચલ અક્ષર **argv,
		     અચલ અક્ષર *prefix,
		     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	અચલ અક્ષર *સંદેશ = ખાલી;
	સંરચના option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', ખાલી, &સંદેશ, N_("reason"), N_("reason of the update")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ખાલી);
	argc = parse_options(argc, argv, prefix, options,
			     git_symbolic_ref_usage, 0);
	જો (સંદેશ && !*સંદેશ)
		die("Refusing to perform update with empty message");

	જો (delete) {
		જો (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		જો (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		જો (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		પાછા refs_delete_ref(get_main_ref_store(the_repository),
				       ખાલી, argv[0], ખાલી, REF_NO_DEREF);
	}

	પસંદગી (argc) {
	સ્થિતિ 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		તોડો;
	સ્થિતિ 2:
		જો (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		જો (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], સંદેશ);
		તોડો;
	મૂળભૂત:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	પાછા ret;
}
