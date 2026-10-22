#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "refs/refs-internal.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "object.h"
#অন্তর্ভাৱ "string-list.h"
#অন্তর্ভাৱ "parse-options.h"

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	রিক্ত
};

সংরচনা show_one_options {
	পূর্ণাঙ্ক quiet;
	পূর্ণাঙ্ক hash_only;
	পূর্ণাঙ্ক abbrev;
	পূর্ণাঙ্ক deref_tags;
};

স্থির শূন্য show_one(ধ্রুৱ সংরচনা show_one_options *opts,
		     ধ্রুৱ সংরচনা reference *ref)
{
	ধ্রুৱ ৱর্ণ *hex;
	সংরচনা object_id peeled;

	যদি (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	যদি (opts->quiet)
		ঘূৰাই;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	যদি (opts->hash_only)
		printf("%s\n", hex);
	অন্যথা
		printf("%s %s\n", hex, ref->name);

	যদি (!opts->deref_tags)
		ঘূৰাই;

	যদি (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		printf("%s %s^{}\n", hex, ref->name);
	}
}

সংরচনা show_ref_data {
	ধ্রুৱ সংরচনা show_one_options *show_one_opts;
	ধ্রুৱ ৱর্ণ **patterns;
	পূর্ণাঙ্ক found_match;
	পূর্ণাঙ্ক show_head;
};

স্থির পূর্ণাঙ্ক show_ref(ধ্রুৱ সংরচনা reference *ref, শূন্য *cbdata)
{
	সংরচনা show_ref_data *data = cbdata;

	যদি (data->show_head && !strcmp(ref->name, "HEAD"))
		গচ্ছ match;

	যদি (data->patterns) {
		পূর্ণাঙ্ক reflen = strlen(ref->name);
		ধ্রুৱ ৱর্ণ **p = data->patterns, *m;
		যেতিয়ালৈ ((m = *p++) != রিক্ত) {
			পূর্ণাঙ্ক len = strlen(m);
			যদি (len > reflen)
				অনুৱর্ত;
			যদি (memcmp(m, ref->name + reflen - len, len))
				অনুৱর্ত;
			যদি (len == reflen)
				গচ্ছ match;
			যদি (ref->name[reflen - len - 1] == '/')
				গচ্ছ match;
		}
		ঘূৰাই 0;
	}

match:
	data->found_match++;

	show_one(data->show_one_opts, ref);

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক add_existing(ধ্রুৱ সংরচনা reference *ref, শূন্য *cbdata)
{
	সংরচনা string_list *list = (সংরচনা string_list *)cbdata;
	string_list_ঢোকাও(list, ref->name);
	ঘূৰাই 0;
}

সংরচনা exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	পূর্ণাঙ্ক enabled;
	ধ্রুৱ ৱর্ণ *pattern;
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
স্থির পূর্ণাঙ্ক cmd_show_ref__exclude_existing(ধ্রুৱ সংরচনা exclude_existing_options *opts)
{
	সংরচনা string_list existing_refs = STRING_LIST_INIT_DUP;
	ৱর্ণ buf[1024];
	পূর্ণাঙ্ক patternlen = opts->pattern ? strlen(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	যেতিয়ালৈ (fgets(buf, পরিমাণ(buf), stdin)) {
		ৱর্ণ *ref;
		পূর্ণাঙ্ক len = strlen(buf);

		যদি (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		যদি (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		বাবে (ref = buf + len; buf < ref; ref--)
			যদি (isspace(ref[-1]))
				ভঙ্গ;
		যদি (opts->pattern) {
			পূর্ণাঙ্ক reflen = buf + len - ref;
			যদি (reflen < patternlen)
				অনুৱর্ত;
			যদি (strncmp(ref, opts->pattern, patternlen))
				অনুৱর্ত;
		}
		যদি (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			অনুৱর্ত;
		}
		যদি (!string_list_has_string(&existing_refs, ref)) {
			printf("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক cmd_show_ref__verify(ধ্রুৱ সংরচনা show_one_options *show_one_opts,
				ধ্রুৱ ৱর্ণ **refs)
{
	যদি (!refs || !*refs)
		die("--verify requires a reference");

	যেতিয়ালৈ (*refs) {
		সংরচনা object_id oid;

		যদি ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			সংরচনা reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} অন্যথা যদি (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} অন্যথা {
			ঘূৰাই 1;
		}

		refs++;
	}

	ঘূৰাই 0;
}

সংরচনা patterns_options {
	পূর্ণাঙ্ক show_head;
	পূর্ণাঙ্ক branches_only;
	পূর্ণাঙ্ক tags_only;
};

স্থির পূর্ণাঙ্ক cmd_show_ref__patterns(ধ্রুৱ সংরচনা patterns_options *opts,
				  ধ্রুৱ সংরচনা show_one_options *show_one_opts,
				  ধ্রুৱ ৱর্ণ **patterns)
{
	সংরচনা show_ref_data show_ref_data = {
		.show_one_opts = show_one_opts,
		.show_head = opts->show_head,
	};

	যদি (patterns && *patterns)
		show_ref_data.patterns = patterns;

	যদি (opts->show_head)
		refs_head_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_data);
	যদি (opts->branches_only || opts->tags_only) {
		সংরচনা refs_for_each_ref_options for_each_ref_opts = { 0 };

		যদি (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}

		যদি (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}
	} অন্যথা {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_data);
	}
	যদি (!show_ref_data.found_match)
		ঘূৰাই 1;

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক cmd_show_ref__exists(ধ্রুৱ ৱর্ণ **refs)
{
	সংরচনা strbuf unused_referent = STRBUF_INIT;
	সংরচনা object_id unused_oid;
	unsigned পূর্ণাঙ্ক unused_type;
	পূর্ণাঙ্ক failure_errno = 0;
	ধ্রুৱ ৱর্ণ *ref;
	পূর্ণাঙ্ক ret = 0;

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

		গচ্ছ out;
	}

out:
	strbuf_release(&unused_referent);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক hash_callback(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা show_one_options *opts = opt->মান;
	সংরচনা option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	যদি (!arg)
		ঘূৰাই 0;

	abbrev_opt.মান = &opts->abbrev;
	ঘূৰাই parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

স্থির পূর্ণাঙ্ক exclude_existing_callback(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg,
				     পূর্ণাঙ্ক unset)
{
	সংরচনা exclude_existing_options *opts = opt->মান;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	ঘূৰাই 0;
}

পূর্ণাঙ্ক cmd_show_ref(পূর্ণাঙ্ক argc,
ধ্রুৱ ৱর্ণ **argv,
ধ্রুৱ ৱর্ণ *prefix,
সংরচনা repository *repo UNUSED)
{
	সংরচনা exclude_existing_options exclude_existing_opts = {0};
	সংরচনা patterns_options patterns_opts = {0};
	সংরচনা show_one_options show_one_opts = {0};
	পূর্ণাঙ্ক verify = 0, exists = 0;
	ধ্রুৱ সংরচনা option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.tags_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', রিক্ত, &patterns_opts.show_head,
				N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL(0, "head", &patterns_opts.show_head,
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

	repo_config(the_repository, git_default_config, রিক্ত);

	argc = parse_options(argc, argv, prefix, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	যদি (exclude_existing_opts.enabled)
		ঘূৰাই cmd_show_ref__exclude_existing(&exclude_existing_opts);
	অন্যথা যদি (verify)
		ঘূৰাই cmd_show_ref__verify(&show_one_opts, argv);
	অন্যথা যদি (exists)
		ঘূৰাই cmd_show_ref__exists(argv);
	অন্যথা
		ঘূৰাই cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
