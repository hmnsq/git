#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	ಖಾಲಿ
};

ರಚನೆ show_one_options {
	ಪೂರ್ಣಾಂಕ quiet;
	ಪೂರ್ಣಾಂಕ hash_only;
	ಪೂರ್ಣಾಂಕ abbrev;
	ಪೂರ್ಣಾಂಕ deref_ಟ್ಯಾಗುಗಳು;
};

ಸ್ಥಿರ ಶೂನ್ಯ show_one(ಸ್ಥಿರಾಂಕ ರಚನೆ show_one_options *opts,
		     ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hex;
	ರಚನೆ object_id peeled;

	ಆದರೆ (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_ಗಮ್ಯ_hex(ref->oid));

	ಆದರೆ (opts->quiet)
		ಹಿಂತಿರುಗಿ;

	hex = repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	ಆದರೆ (opts->hash_only)
		ಬರೆ("%s\n", hex);
	ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ("%s %s\n", hex, ref->name);

	ಆದರೆ (!opts->deref_ಟ್ಯಾಗುಗಳು)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &peeled, opts->abbrev);
		ಬರೆ("%s %s^{}\n", hex, ref->name);
	}
}

ರಚನೆ show_ref_ದತ್ತಾಂಶ {
	ಸ್ಥಿರಾಂಕ ರಚನೆ show_one_options *show_one_opts;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **patterns;
	ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು_ಹೊಂದಿಕೆ;
	ಪೂರ್ಣಾಂಕ show_ತಲೆ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cbdata)
{
	ರಚನೆ show_ref_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cbdata;

	ಆದರೆ (ದತ್ತಾಂಶ->show_ತಲೆ && !strcmp(ref->name, "HEAD"))
		ಹೋಗು ಹೊಂದಿಕೆ;

	ಆದರೆ (ದತ್ತಾಂಶ->patterns) {
		ಪೂರ್ಣಾಂಕ reflen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ref->name);
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **p = ದತ್ತಾಂಶ->patterns, *m;
		ಆಗ ((m = *p++) != ಖಾಲಿ) {
			ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(m);
			ಆದರೆ (len > reflen)
				ಮುಂದುವರಿ;
			ಆದರೆ (memcmp(m, ref->name + reflen - len, len))
				ಮುಂದುವರಿ;
			ಆದರೆ (len == reflen)
				ಹೋಗು ಹೊಂದಿಕೆ;
			ಆದರೆ (ref->name[reflen - len - 1] == '/')
				ಹೋಗು ಹೊಂದಿಕೆ;
		}
		ಹಿಂತಿರುಗಿ 0;
	}

ಹೊಂದಿಕೆ:
	ದತ್ತಾಂಶ->ಸಿಕ್ಕಿತು_ಹೊಂದಿಕೆ++;

	show_one(ದತ್ತಾಂಶ->show_one_opts, ref);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_existing(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cbdata)
{
	ರಚನೆ string_list *list = (ರಚನೆ string_list *)cbdata;
	string_list_ಸೇರಿಸು(list, ref->name);
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	ಪೂರ್ಣಾಂಕ enabled;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ನಮೂನೆ;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_show_ref__exclude_existing(ಸ್ಥಿರಾಂಕ ರಚನೆ exclude_existing_options *opts)
{
	ರಚನೆ string_list existing_refs = STRING_LIST_INIT_DUP;
	ಅಕ್ಷರ buf[1024];
	ಪೂರ್ಣಾಂಕ patternlen = opts->ನಮೂನೆ ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(opts->ನಮೂನೆ) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), ಕೂಡಿಸು_existing,
			  &existing_refs);
	ಆಗ (fgets(buf, ಗಾತ್ರ(buf), stdin)) {
		ಅಕ್ಷರ *ref;
		ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(buf);

		ಆದರೆ (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		ಆದರೆ (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		ಗಾಗಿ (ref = buf + len; buf < ref; ref--)
			ಆದರೆ (ಖಾಲಿಯೇ(ref[-1]))
				ನಿಲ್ಲಿಸು;
		ಆದರೆ (opts->ನಮೂನೆ) {
			ಪೂರ್ಣಾಂಕ reflen = buf + len - ref;
			ಆದರೆ (reflen < patternlen)
				ಮುಂದುವರಿ;
			ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(ref, opts->ನಮೂನೆ, patternlen))
				ಮುಂದುವರಿ;
		}
		ಆದರೆ (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!string_list_has_string(&existing_refs, ref)) {
			ಬರೆ("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_show_ref__verify(ಸ್ಥಿರಾಂಕ ರಚನೆ show_one_options *show_one_opts,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **refs)
{
	ಆದರೆ (!refs || !*refs)
		die("--verify requires a reference");

	ಆಗ (*refs) {
		ರಚನೆ object_id oid;

		ಆದರೆ ((starts_with(*refs, "refs/") || refname_is_ಸುರಕ್ಷಿತ(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			ರಚನೆ reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಹಿಂತಿರುಗಿ 1;
		}

		refs++;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ patterns_options {
	ಪೂರ್ಣಾಂಕ show_ತಲೆ;
	ಪೂರ್ಣಾಂಕ branches_only;
	ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗುಗಳು_only;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_show_ref__patterns(ಸ್ಥಿರಾಂಕ ರಚನೆ patterns_options *opts,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ show_one_options *show_one_opts,
				  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **patterns)
{
	ರಚನೆ show_ref_ದತ್ತಾಂಶ show_ref_ದತ್ತಾಂಶ = {
		.show_one_opts = show_one_opts,
		.show_ತಲೆ = opts->show_ತಲೆ,
	};

	ಆದರೆ (patterns && *patterns)
		show_ref_ದತ್ತಾಂಶ.patterns = patterns;

	ಆದರೆ (opts->show_ತಲೆ)
		refs_ತಲೆ_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_ದತ್ತಾಂಶ);
	ಆದರೆ (opts->branches_only || opts->ಟ್ಯಾಗುಗಳು_only) {
		ರಚನೆ refs_for_each_ref_options for_each_ref_opts = { 0 };

		ಆದರೆ (opts->branches_only) {
			for_each_ref_opts.ಪೂರ್ವಪ್ರತ್ಯಯ = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_ದತ್ತಾಂಶ, &for_each_ref_opts);
		}

		ಆದರೆ (opts->ಟ್ಯಾಗುಗಳು_only) {
			for_each_ref_opts.ಪೂರ್ವಪ್ರತ್ಯಯ = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_ದತ್ತಾಂಶ, &for_each_ref_opts);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_ದತ್ತಾಂಶ);
	}
	ಆದರೆ (!show_ref_ದತ್ತಾಂಶ.ಸಿಕ್ಕಿತು_ಹೊಂದಿಕೆ)
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_show_ref__exists(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **refs)
{
	ರಚನೆ strbuf ಬಳಸದ_referent = STRBUF_INIT;
	ರಚನೆ object_id ಬಳಸದ_oid;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಳಸದ_type;
	ಪೂರ್ಣಾಂಕ failure_errno = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಆದರೆ (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	ಆದರೆ (*refs)
		die("--exists requires exactly one reference");

	ಆದರೆ (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &ಬಳಸದ_oid, &ಬಳಸದ_referent, &ಬಳಸದ_type,
			      &failure_errno)) {
		ಆದರೆ (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} ಇಲ್ಲದಿದ್ದರೆ {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		ಹೋಗು ಹೊರಹೋಗು;
	}

ಹೊರಹೋಗು:
	strbuf_release(&ಬಳಸದ_referent);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ hash_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ show_one_options *opts = opt->ಮೌಲ್ಯ;
	ರಚನೆ option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	ಆದರೆ (!arg)
		ಹಿಂತಿರುಗಿ 0;

	abbrev_opt.ಮೌಲ್ಯ = &opts->abbrev;
	ಹಿಂತಿರುಗಿ parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ exclude_existing_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				     ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ exclude_existing_options *opts = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->ನಮೂನೆ = arg;
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_show_ref(ಪೂರ್ಣಾಂಕ argc,
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ exclude_existing_options exclude_existing_opts = {0};
	ರಚನೆ patterns_options patterns_opts = {0};
	ರಚನೆ show_one_options show_one_opts = {0};
	ಪೂರ್ಣಾಂಕ verify = 0, exists = 0;
	ಸ್ಥಿರಾಂಕ ರಚನೆ option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.ಟ್ಯಾಗುಗಳು_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', ಖಾಲಿ, &patterns_opts.show_ತಲೆ,
				N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL(0, "head", &patterns_opts.show_ತಲೆ,
		  N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL('d', "dereference", &show_one_opts.deref_ಟ್ಯಾಗುಗಳು,
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
		OPT_ಕೊನೆ()
	};

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	ಆದರೆ (exclude_existing_opts.enabled)
		ಹಿಂತಿರುಗಿ cmd_show_ref__exclude_existing(&exclude_existing_opts);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (verify)
		ಹಿಂತಿರುಗಿ cmd_show_ref__verify(&show_one_opts, argv);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (exists)
		ಹಿಂತಿರುಗಿ cmd_show_ref__exists(argv);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
