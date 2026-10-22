#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs/refs-internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	ਨਿਰਾ
};

ਬਣਤਰ show_one_options {
	ਪੂਰਨ quiet;
	ਪੂਰਨ hash_only;
	ਪੂਰਨ abbrev;
	ਪੂਰਨ deref_ਟੈਗ_ਸੂਚੀ;
};

ਸਥਿਰ ਖਾਲੀ show_one(ਅਚਲ ਬਣਤਰ show_one_options *opts,
		     ਅਚਲ ਬਣਤਰ reference *ref)
{
	ਅਚਲ ਅੱਖਰ *hex;
	ਬਣਤਰ object_id peeled;

	ਜੇ (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_ਮੰਜ਼ਿਲ_hex(ref->oid));

	ਜੇ (opts->quiet)
		ਵਾਪਸ;

	hex = repo_ਲੱਭੋ_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	ਜੇ (opts->hash_only)
		ਲਿਖੋ("%s\n", hex);
	ਨਹੀਂਤਾਂ
		ਲਿਖੋ("%s %s\n", hex, ref->name);

	ਜੇ (!opts->deref_ਟੈਗ_ਸੂਚੀ)
		ਵਾਪਸ;

	ਜੇ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_ਲੱਭੋ_unique_abbrev(the_repository, &peeled, opts->abbrev);
		ਲਿਖੋ("%s %s^{}\n", hex, ref->name);
	}
}

ਬਣਤਰ show_ref_ਅੰਕੜੇ {
	ਅਚਲ ਬਣਤਰ show_one_options *show_one_opts;
	ਅਚਲ ਅੱਖਰ **patterns;
	ਪੂਰਨ ਮਿਲਿਆ_ਮੇਲ;
	ਪੂਰਨ show_ਸਿਰ;
};

ਸਥਿਰ ਪੂਰਨ show_ref(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cbdata)
{
	ਬਣਤਰ show_ref_ਅੰਕੜੇ *ਅੰਕੜੇ = cbdata;

	ਜੇ (ਅੰਕੜੇ->show_ਸਿਰ && !strcmp(ref->name, "HEAD"))
		ਜਾਓ ਮੇਲ;

	ਜੇ (ਅੰਕੜੇ->patterns) {
		ਪੂਰਨ reflen = ਲੜੀ_ਲੰਬਾਈ(ref->name);
		ਅਚਲ ਅੱਖਰ **p = ਅੰਕੜੇ->patterns, *m;
		ਜਦੋਂ ((m = *p++) != ਨਿਰਾ) {
			ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(m);
			ਜੇ (len > reflen)
				ਜਾਰੀ;
			ਜੇ (memcmp(m, ref->name + reflen - len, len))
				ਜਾਰੀ;
			ਜੇ (len == reflen)
				ਜਾਓ ਮੇਲ;
			ਜੇ (ref->name[reflen - len - 1] == '/')
				ਜਾਓ ਮੇਲ;
		}
		ਵਾਪਸ 0;
	}

ਮੇਲ:
	ਅੰਕੜੇ->ਮਿਲਿਆ_ਮੇਲ++;

	show_one(ਅੰਕੜੇ->show_one_opts, ref);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_existing(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cbdata)
{
	ਬਣਤਰ string_list *list = (ਬਣਤਰ string_list *)cbdata;
	string_list_ਪਾਓ(list, ref->name);
	ਵਾਪਸ 0;
}

ਬਣਤਰ exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	ਪੂਰਨ enabled;
	ਅਚਲ ਅੱਖਰ *ਨਮੂਨਾ;
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
ਸਥਿਰ ਪੂਰਨ cmd_show_ref__exclude_existing(ਅਚਲ ਬਣਤਰ exclude_existing_options *opts)
{
	ਬਣਤਰ string_list existing_refs = STRING_LIST_INIT_DUP;
	ਅੱਖਰ buf[1024];
	ਪੂਰਨ patternlen = opts->ਨਮੂਨਾ ? ਲੜੀ_ਲੰਬਾਈ(opts->ਨਮੂਨਾ) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), ਜੋੜੋ_existing,
			  &existing_refs);
	ਜਦੋਂ (fgets(buf, ਆਕਾਰ(buf), stdin)) {
		ਅੱਖਰ *ref;
		ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(buf);

		ਜੇ (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		ਜੇ (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		ਲਈ (ref = buf + len; buf < ref; ref--)
			ਜੇ (ਖਾਲੀ_ਥਾਂ_ਹੈ(ref[-1]))
				ਰੋਕੋ;
		ਜੇ (opts->ਨਮੂਨਾ) {
			ਪੂਰਨ reflen = buf + len - ref;
			ਜੇ (reflen < patternlen)
				ਜਾਰੀ;
			ਜੇ (ਲੜੀ_ਤੁਲਨਾ(ref, opts->ਨਮੂਨਾ, patternlen))
				ਜਾਰੀ;
		}
		ਜੇ (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			ਜਾਰੀ;
		}
		ਜੇ (!string_list_has_string(&existing_refs, ref)) {
			ਲਿਖੋ("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cmd_show_ref__verify(ਅਚਲ ਬਣਤਰ show_one_options *show_one_opts,
				ਅਚਲ ਅੱਖਰ **refs)
{
	ਜੇ (!refs || !*refs)
		die("--verify requires a reference");

	ਜਦੋਂ (*refs) {
		ਬਣਤਰ object_id oid;

		ਜੇ ((starts_with(*refs, "refs/") || refname_is_ਸੁਰੱਖਿਅਤ(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			ਬਣਤਰ reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} ਨਹੀਂਤਾਂ ਜੇ (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} ਨਹੀਂਤਾਂ {
			ਵਾਪਸ 1;
		}

		refs++;
	}

	ਵਾਪਸ 0;
}

ਬਣਤਰ patterns_options {
	ਪੂਰਨ show_ਸਿਰ;
	ਪੂਰਨ branches_only;
	ਪੂਰਨ ਟੈਗ_ਸੂਚੀ_only;
};

ਸਥਿਰ ਪੂਰਨ cmd_show_ref__patterns(ਅਚਲ ਬਣਤਰ patterns_options *opts,
				  ਅਚਲ ਬਣਤਰ show_one_options *show_one_opts,
				  ਅਚਲ ਅੱਖਰ **patterns)
{
	ਬਣਤਰ show_ref_ਅੰਕੜੇ show_ref_ਅੰਕੜੇ = {
		.show_one_opts = show_one_opts,
		.show_ਸਿਰ = opts->show_ਸਿਰ,
	};

	ਜੇ (patterns && *patterns)
		show_ref_ਅੰਕੜੇ.patterns = patterns;

	ਜੇ (opts->show_ਸਿਰ)
		refs_ਸਿਰ_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_ਅੰਕੜੇ);
	ਜੇ (opts->branches_only || opts->ਟੈਗ_ਸੂਚੀ_only) {
		ਬਣਤਰ refs_for_each_ref_options for_each_ref_opts = { 0 };

		ਜੇ (opts->branches_only) {
			for_each_ref_opts.ਅਗੇਤਰ = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_ਅੰਕੜੇ, &for_each_ref_opts);
		}

		ਜੇ (opts->ਟੈਗ_ਸੂਚੀ_only) {
			for_each_ref_opts.ਅਗੇਤਰ = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_ਅੰਕੜੇ, &for_each_ref_opts);
		}
	} ਨਹੀਂਤਾਂ {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_ਅੰਕੜੇ);
	}
	ਜੇ (!show_ref_ਅੰਕੜੇ.ਮਿਲਿਆ_ਮੇਲ)
		ਵਾਪਸ 1;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cmd_show_ref__exists(ਅਚਲ ਅੱਖਰ **refs)
{
	ਬਣਤਰ strbuf ਅਣਵਰਤਿਆ_referent = STRBUF_INIT;
	ਬਣਤਰ object_id ਅਣਵਰਤਿਆ_oid;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਣਵਰਤਿਆ_type;
	ਪੂਰਨ failure_errno = 0;
	ਅਚਲ ਅੱਖਰ *ref;
	ਪੂਰਨ ret = 0;

	ਜੇ (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	ਜੇ (*refs)
		die("--exists requires exactly one reference");

	ਜੇ (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &ਅਣਵਰਤਿਆ_oid, &ਅਣਵਰਤਿਆ_referent, &ਅਣਵਰਤਿਆ_type,
			      &failure_errno)) {
		ਜੇ (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} ਨਹੀਂਤਾਂ {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		ਜਾਓ ਬਾਹਰਲਾ;
	}

ਬਾਹਰਲਾ:
	strbuf_release(&ਅਣਵਰਤਿਆ_referent);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ hash_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ show_one_options *opts = opt->ਮੁੱਲ;
	ਬਣਤਰ option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	ਜੇ (!arg)
		ਵਾਪਸ 0;

	abbrev_opt.ਮੁੱਲ = &opts->abbrev;
	ਵਾਪਸ parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

ਸਥਿਰ ਪੂਰਨ exclude_existing_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
				     ਪੂਰਨ unset)
{
	ਬਣਤਰ exclude_existing_options *opts = opt->ਮੁੱਲ;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->ਨਮੂਨਾ = arg;
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_show_ref(ਪੂਰਨ argc,
ਅਚਲ ਅੱਖਰ **argv,
ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ exclude_existing_options exclude_existing_opts = {0};
	ਬਣਤਰ patterns_options patterns_opts = {0};
	ਬਣਤਰ show_one_options show_one_opts = {0};
	ਪੂਰਨ verify = 0, exists = 0;
	ਅਚਲ ਬਣਤਰ option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.ਟੈਗ_ਸੂਚੀ_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', ਨਿਰਾ, &patterns_opts.show_ਸਿਰ,
				N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL(0, "head", &patterns_opts.show_ਸਿਰ,
		  N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL('d', "dereference", &show_one_opts.deref_ਟੈਗ_ਸੂਚੀ,
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
		OPT_ਸਿਰਾ()
	};

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	ਜੇ (exclude_existing_opts.enabled)
		ਵਾਪਸ cmd_show_ref__exclude_existing(&exclude_existing_opts);
	ਨਹੀਂਤਾਂ ਜੇ (verify)
		ਵਾਪਸ cmd_show_ref__verify(&show_one_opts, argv);
	ਨਹੀਂਤਾਂ ਜੇ (exists)
		ਵਾਪਸ cmd_show_ref__exists(argv);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
