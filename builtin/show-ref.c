#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "refs/refs-internal.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	ರಿಕ್ತ
};

ಸಂರಚನಾ show_one_options {
	ಪೂರ್ಣಾಙ್ಕ quiet;
	ಪೂರ್ಣಾಙ್ಕ hash_only;
	ಪೂರ್ಣಾಙ್ಕ abbrev;
	ಪೂರ್ಣಾಙ್ಕ deref_tags;
};

ಸ್ಥಿರ ಶೂನ್ಯ show_one(ಧ್ರುವ ಸಂರಚನಾ show_one_options *opts,
		     ಧ್ರುವ ಸಂರಚನಾ reference *ref)
{
	ಧ್ರುವ ವರ್ಣ *hex;
	ಸಂರಚನಾ object_id peeled;

	ಯದಿ (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	ಯದಿ (opts->quiet)
		ಪ್ರತ್ಯಾಗಮ;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	ಯದಿ (opts->hash_only)
		printf("%s\n", hex);
	ಅನ್ಯಥಾ
		printf("%s %s\n", hex, ref->name);

	ಯದಿ (!opts->deref_tags)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		printf("%s %s^{}\n", hex, ref->name);
	}
}

ಸಂರಚನಾ show_ref_data {
	ಧ್ರುವ ಸಂರಚನಾ show_one_options *show_one_opts;
	ಧ್ರುವ ವರ್ಣ **patterns;
	ಪೂರ್ಣಾಙ್ಕ found_match;
	ಪೂರ್ಣಾಙ್ಕ show_head;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_ref(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cbdata)
{
	ಸಂರಚನಾ show_ref_data *data = cbdata;

	ಯದಿ (data->show_head && !strcmp(ref->name, "HEAD"))
		ಗಚ್ಛ match;

	ಯದಿ (data->patterns) {
		ಪೂರ್ಣಾಙ್ಕ reflen = strlen(ref->name);
		ಧ್ರುವ ವರ್ಣ **p = data->patterns, *m;
		ಯಾವತ್ ((m = *p++) != ರಿಕ್ತ) {
			ಪೂರ್ಣಾಙ್ಕ len = strlen(m);
			ಯದಿ (len > reflen)
				ಅನುವರ್ತ;
			ಯದಿ (memcmp(m, ref->name + reflen - len, len))
				ಅನುವರ್ತ;
			ಯದಿ (len == reflen)
				ಗಚ್ಛ match;
			ಯದಿ (ref->name[reflen - len - 1] == '/')
				ಗಚ್ಛ match;
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}

match:
	data->found_match++;

	show_one(data->show_one_opts, ref);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_existing(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cbdata)
{
	ಸಂರಚನಾ string_list *list = (ಸಂರಚನಾ string_list *)cbdata;
	string_list_ಸೇರಿಸು(list, ref->name);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	ಪೂರ್ಣಾಙ್ಕ enabled;
	ಧ್ರುವ ವರ್ಣ *pattern;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_show_ref__exclude_existing(ಧ್ರುವ ಸಂರಚನಾ exclude_existing_options *opts)
{
	ಸಂರಚನಾ string_list existing_refs = STRING_LIST_INIT_DUP;
	ವರ್ಣ buf[1024];
	ಪೂರ್ಣಾಙ್ಕ patternlen = opts->pattern ? strlen(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	ಯಾವತ್ (fgets(buf, ಪರಿಮಾಣ(buf), stdin)) {
		ವರ್ಣ *ref;
		ಪೂರ್ಣಾಙ್ಕ len = strlen(buf);

		ಯದಿ (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		ಯದಿ (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		ಪ್ರತಿ (ref = buf + len; buf < ref; ref--)
			ಯದಿ (isspace(ref[-1]))
				ಭಙ್ಗ;
		ಯದಿ (opts->pattern) {
			ಪೂರ್ಣಾಙ್ಕ reflen = buf + len - ref;
			ಯದಿ (reflen < patternlen)
				ಅನುವರ್ತ;
			ಯದಿ (strncmp(ref, opts->pattern, patternlen))
				ಅನುವರ್ತ;
		}
		ಯದಿ (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			ಅನುವರ್ತ;
		}
		ಯದಿ (!string_list_has_string(&existing_refs, ref)) {
			printf("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_show_ref__verify(ಧ್ರುವ ಸಂರಚನಾ show_one_options *show_one_opts,
				ಧ್ರುವ ವರ್ಣ **refs)
{
	ಯದಿ (!refs || !*refs)
		die("--verify requires a reference");

	ಯಾವತ್ (*refs) {
		ಸಂರಚನಾ object_id oid;

		ಯದಿ ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			ಸಂರಚನಾ reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} ಅನ್ಯಥಾ ಯದಿ (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} ಅನ್ಯಥಾ {
			ಪ್ರತ್ಯಾಗಮ 1;
		}

		refs++;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ patterns_options {
	ಪೂರ್ಣಾಙ್ಕ show_head;
	ಪೂರ್ಣಾಙ್ಕ branches_only;
	ಪೂರ್ಣಾಙ್ಕ tags_only;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_show_ref__patterns(ಧ್ರುವ ಸಂರಚನಾ patterns_options *opts,
				  ಧ್ರುವ ಸಂರಚನಾ show_one_options *show_one_opts,
				  ಧ್ರುವ ವರ್ಣ **patterns)
{
	ಸಂರಚನಾ show_ref_data show_ref_data = {
		.show_one_opts = show_one_opts,
		.show_head = opts->show_head,
	};

	ಯದಿ (patterns && *patterns)
		show_ref_data.patterns = patterns;

	ಯದಿ (opts->show_head)
		refs_head_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_data);
	ಯದಿ (opts->branches_only || opts->tags_only) {
		ಸಂರಚನಾ refs_for_each_ref_options for_each_ref_opts = { 0 };

		ಯದಿ (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}

		ಯದಿ (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}
	} ಅನ್ಯಥಾ {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_data);
	}
	ಯದಿ (!show_ref_data.found_match)
		ಪ್ರತ್ಯಾಗಮ 1;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_show_ref__exists(ಧ್ರುವ ವರ್ಣ **refs)
{
	ಸಂರಚನಾ strbuf unused_referent = STRBUF_INIT;
	ಸಂರಚನಾ object_id unused_oid;
	unsigned ಪೂರ್ಣಾಙ್ಕ unused_type;
	ಪೂರ್ಣಾಙ್ಕ failure_errno = 0;
	ಧ್ರುವ ವರ್ಣ *ref;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಯದಿ (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	ಯದಿ (*refs)
		die("--exists requires exactly one reference");

	ಯದಿ (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		ಯದಿ (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} ಅನ್ಯಥಾ {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		ಗಚ್ಛ out;
	}

out:
	strbuf_release(&unused_referent);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ hash_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ show_one_options *opts = opt->ಮೌಲ್ಯ;
	ಸಂರಚನಾ option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	ಯದಿ (!arg)
		ಪ್ರತ್ಯಾಗಮ 0;

	abbrev_opt.ಮೌಲ್ಯ = &opts->abbrev;
	ಪ್ರತ್ಯಾಗಮ parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ exclude_existing_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
				     ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ exclude_existing_options *opts = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_show_ref(ಪೂರ್ಣಾಙ್ಕ argc,
ಧ್ರುವ ವರ್ಣ **argv,
ಧ್ರುವ ವರ್ಣ *prefix,
ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ exclude_existing_options exclude_existing_opts = {0};
	ಸಂರಚನಾ patterns_options patterns_opts = {0};
	ಸಂರಚನಾ show_one_options show_one_opts = {0};
	ಪೂರ್ಣಾಙ್ಕ verify = 0, exists = 0;
	ಧ್ರುವ ಸಂರಚನಾ option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.tags_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', ರಿಕ್ತ, &patterns_opts.show_head,
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

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	ಯದಿ (exclude_existing_opts.enabled)
		ಪ್ರತ್ಯಾಗಮ cmd_show_ref__exclude_existing(&exclude_existing_opts);
	ಅನ್ಯಥಾ ಯದಿ (verify)
		ಪ್ರತ್ಯಾಗಮ cmd_show_ref__verify(&show_one_opts, argv);
	ಅನ್ಯಥಾ ಯದಿ (exists)
		ಪ್ರತ್ಯಾಗಮ cmd_show_ref__exists(argv);
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
