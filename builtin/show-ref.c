#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "refs/refs-internal.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "object.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "parse-options.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	ফাঁকা
};

কাঠামো show_one_options {
	পূর্ণসংখ্যা quiet;
	পূর্ণসংখ্যা hash_only;
	পূর্ণসংখ্যা abbrev;
	পূর্ণসংখ্যা deref_tags;
};

স্থির শূন্য show_one(ধ্রুবক কাঠামো show_one_options *opts,
		     ধ্রুবক কাঠামো reference *ref)
{
	ধ্রুবক অক্ষর *hex;
	কাঠামো object_id peeled;

	যদি (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	যদি (opts->quiet)
		ফেরত;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	যদি (opts->hash_only)
		printf("%s\n", hex);
	অন্যথা
		printf("%s %s\n", hex, ref->name);

	যদি (!opts->deref_tags)
		ফেরত;

	যদি (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		printf("%s %s^{}\n", hex, ref->name);
	}
}

কাঠামো show_ref_তথ্য {
	ধ্রুবক কাঠামো show_one_options *show_one_opts;
	ধ্রুবক অক্ষর **patterns;
	পূর্ণসংখ্যা found_match;
	পূর্ণসংখ্যা show_শীর্ষ;
};

স্থির পূর্ণসংখ্যা show_ref(ধ্রুবক কাঠামো reference *ref, শূন্য *cbdata)
{
	কাঠামো show_ref_তথ্য *তথ্য = cbdata;

	যদি (তথ্য->show_শীর্ষ && !strcmp(ref->name, "HEAD"))
		যাও match;

	যদি (তথ্য->patterns) {
		পূর্ণসংখ্যা reflen = strlen(ref->name);
		ধ্রুবক অক্ষর **p = তথ্য->patterns, *m;
		যখন ((m = *p++) != ফাঁকা) {
			পূর্ণসংখ্যা len = strlen(m);
			যদি (len > reflen)
				চালু;
			যদি (memcmp(m, ref->name + reflen - len, len))
				চালু;
			যদি (len == reflen)
				যাও match;
			যদি (ref->name[reflen - len - 1] == '/')
				যাও match;
		}
		ফেরত 0;
	}

match:
	তথ্য->found_match++;

	show_one(তথ্য->show_one_opts, ref);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা add_existing(ধ্রুবক কাঠামো reference *ref, শূন্য *cbdata)
{
	কাঠামো string_list *list = (কাঠামো string_list *)cbdata;
	string_list_ঢোকাও(list, ref->name);
	ফেরত 0;
}

কাঠামো exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	পূর্ণসংখ্যা enabled;
	ধ্রুবক অক্ষর *pattern;
};

/*
 * read "^(?:<anything>\s)?<refname>(?:\^\{\})?$" from the standard input,
 * and
 * (1) strip "^{}" at the end of line if any;
 * (2) ignore if match is provided and does not head-match refname;
 * (3) warn if refname is not a well-formed refname and skip;
 * (4) ignore if refname is a ref that exists in the local repository;
 * (5) otherwise output the line.
 */
স্থির পূর্ণসংখ্যা cmd_show_ref__exclude_existing(ধ্রুবক কাঠামো exclude_existing_options *opts)
{
	কাঠামো string_list existing_refs = STRING_LIST_INIT_DUP;
	অক্ষর buf[1024];
	পূর্ণসংখ্যা patternlen = opts->pattern ? strlen(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	যখন (fgets(buf, আকার(buf), stdin)) {
		অক্ষর *ref;
		পূর্ণসংখ্যা len = strlen(buf);

		যদি (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		যদি (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		জন্য (ref = buf + len; buf < ref; ref--)
			যদি (isspace(ref[-1]))
				বিরতি;
		যদি (opts->pattern) {
			পূর্ণসংখ্যা reflen = buf + len - ref;
			যদি (reflen < patternlen)
				চালু;
			যদি (strncmp(ref, opts->pattern, patternlen))
				চালু;
		}
		যদি (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			চালু;
		}
		যদি (!string_list_has_string(&existing_refs, ref)) {
			printf("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা cmd_show_ref__verify(ধ্রুবক কাঠামো show_one_options *show_one_opts,
				ধ্রুবক অক্ষর **refs)
{
	যদি (!refs || !*refs)
		die("--verify requires a reference");

	যখন (*refs) {
		কাঠামো object_id oid;

		যদি ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			কাঠামো reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} অন্যথা যদি (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} অন্যথা {
			ফেরত 1;
		}

		refs++;
	}

	ফেরত 0;
}

কাঠামো patterns_options {
	পূর্ণসংখ্যা show_শীর্ষ;
	পূর্ণসংখ্যা branches_only;
	পূর্ণসংখ্যা tags_only;
};

স্থির পূর্ণসংখ্যা cmd_show_ref__patterns(ধ্রুবক কাঠামো patterns_options *opts,
				  ধ্রুবক কাঠামো show_one_options *show_one_opts,
				  ধ্রুবক অক্ষর **patterns)
{
	কাঠামো show_ref_তথ্য show_ref_তথ্য = {
		.show_one_opts = show_one_opts,
		.show_শীর্ষ = opts->show_শীর্ষ,
	};

	যদি (patterns && *patterns)
		show_ref_তথ্য.patterns = patterns;

	যদি (opts->show_শীর্ষ)
		refs_শীর্ষ_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_তথ্য);
	যদি (opts->branches_only || opts->tags_only) {
		কাঠামো refs_for_each_ref_options for_each_ref_opts = { 0 };

		যদি (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_তথ্য, &for_each_ref_opts);
		}

		যদি (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_তথ্য, &for_each_ref_opts);
		}
	} অন্যথা {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_তথ্য);
	}
	যদি (!show_ref_তথ্য.found_match)
		ফেরত 1;

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা cmd_show_ref__exists(ধ্রুবক অক্ষর **refs)
{
	কাঠামো strbuf unused_referent = STRBUF_INIT;
	কাঠামো object_id unused_oid;
	unsigned পূর্ণসংখ্যা unused_type;
	পূর্ণসংখ্যা failure_errno = 0;
	ধ্রুবক অক্ষর *ref;
	পূর্ণসংখ্যা ret = 0;

	যদি (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	যদি (*refs)
		die("--exists requires exactly one reference");

	যদি (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		যদি (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} অন্যথা {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		যাও out;
	}

out:
	strbuf_release(&unused_referent);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা hash_callback(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো show_one_options *opts = opt->মান;
	কাঠামো option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	যদি (!arg)
		ফেরত 0;

	abbrev_opt.মান = &opts->abbrev;
	ফেরত parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

স্থির পূর্ণসংখ্যা exclude_existing_callback(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
				     পূর্ণসংখ্যা unset)
{
	কাঠামো exclude_existing_options *opts = opt->মান;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	ফেরত 0;
}

পূর্ণসংখ্যা cmd_show_ref(পূর্ণসংখ্যা argc,
ধ্রুবক অক্ষর **argv,
ধ্রুবক অক্ষর *prefix,
কাঠামো repository *repo UNUSED)
{
	কাঠামো exclude_existing_options exclude_existing_opts = {0};
	কাঠামো patterns_options patterns_opts = {0};
	কাঠামো show_one_options show_one_opts = {0};
	পূর্ণসংখ্যা verify = 0, exists = 0;
	ধ্রুবক কাঠামো option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.tags_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', ফাঁকা, &patterns_opts.show_শীর্ষ,
				N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL(0, "head", &patterns_opts.show_শীর্ষ,
		  N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL('d', "dereference", &show_one_opts.deref_tags,
			    N_("dereference tags into object IDs")),
		OPT_CALLBACK_F('s', "hash", &show_one_opts, N_("n"),
			       N_("only show SHA1 hash using <n> digits"),
			       PARSE_OPT_OPTARG, &hash_callback),
		OPT__ABBREV(&show_one_opts.abbrev),
		OPT__QUIET(&show_one_opts.quiet,
			   N_("do not print results to stdout (useful with --verify)")),
		OPT_CALLBACK_F(0, "exclude-existing", &exclude_existing_opts,
			       N_("pattern"), N_("show refs from stdin that aren't in local repository"),
			       PARSE_OPT_OPTARG | PARSE_OPT_NONEG, exclude_existing_callback),
		OPT_END()
	};

	repo_config(the_repository, git_default_config, ফাঁকা);

	argc = parse_options(argc, argv, prefix, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	যদি (exclude_existing_opts.enabled)
		ফেরত cmd_show_ref__exclude_existing(&exclude_existing_opts);
	অন্যথা যদি (verify)
		ফেরত cmd_show_ref__verify(&show_one_opts, argv);
	অন্যথা যদি (exists)
		ফেরত cmd_show_ref__exists(argv);
	অন্যথা
		ফেরত cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
