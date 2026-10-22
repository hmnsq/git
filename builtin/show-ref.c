#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "refs/refs-internal.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "parse-options.h"

நிலையான மாறிலி எழுத்து * மாறிலி show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	வெற்று
};

கட்டமைப்பு show_one_options {
	முழு quiet;
	முழு hash_only;
	முழு abbrev;
	முழு deref_tags;
};

நிலையான வெறுமை show_one(மாறிலி கட்டமைப்பு show_one_options *opts,
		     மாறிலி கட்டமைப்பு reference *ref)
{
	மாறிலி எழுத்து *hex;
	கட்டமைப்பு object_id peeled;

	எனில் (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	எனில் (opts->quiet)
		திரும்பு;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	எனில் (opts->hash_only)
		printf("%s\n", hex);
	இல்லையெனில்
		printf("%s %s\n", hex, ref->name);

	எனில் (!opts->deref_tags)
		திரும்பு;

	எனில் (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		printf("%s %s^{}\n", hex, ref->name);
	}
}

கட்டமைப்பு show_ref_data {
	மாறிலி கட்டமைப்பு show_one_options *show_one_opts;
	மாறிலி எழுத்து **patterns;
	முழு found_match;
	முழு show_head;
};

நிலையான முழு show_ref(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cbdata)
{
	கட்டமைப்பு show_ref_data *data = cbdata;

	எனில் (data->show_head && !strcmp(ref->name, "HEAD"))
		செல் match;

	எனில் (data->patterns) {
		முழு reflen = strlen(ref->name);
		மாறிலி எழுத்து **p = data->patterns, *m;
		போது ((m = *p++) != வெற்று) {
			முழு len = strlen(m);
			எனில் (len > reflen)
				தொடர்;
			எனில் (memcmp(m, ref->name + reflen - len, len))
				தொடர்;
			எனில் (len == reflen)
				செல் match;
			எனில் (ref->name[reflen - len - 1] == '/')
				செல் match;
		}
		திரும்பு 0;
	}

match:
	data->found_match++;

	show_one(data->show_one_opts, ref);

	திரும்பு 0;
}

நிலையான முழு add_existing(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cbdata)
{
	கட்டமைப்பு string_list *list = (கட்டமைப்பு string_list *)cbdata;
	string_list_செருகு(list, ref->name);
	திரும்பு 0;
}

கட்டமைப்பு exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	முழு enabled;
	மாறிலி எழுத்து *pattern;
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
நிலையான முழு cmd_show_ref__exclude_existing(மாறிலி கட்டமைப்பு exclude_existing_options *opts)
{
	கட்டமைப்பு string_list existing_refs = STRING_LIST_INIT_DUP;
	எழுத்து buf[1024];
	முழு patternlen = opts->pattern ? strlen(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	போது (fgets(buf, அளவு(buf), stdin)) {
		எழுத்து *ref;
		முழு len = strlen(buf);

		எனில் (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		எனில் (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		க்கு (ref = buf + len; buf < ref; ref--)
			எனில் (isspace(ref[-1]))
				நிறுத்து;
		எனில் (opts->pattern) {
			முழு reflen = buf + len - ref;
			எனில் (reflen < patternlen)
				தொடர்;
			எனில் (strncmp(ref, opts->pattern, patternlen))
				தொடர்;
		}
		எனில் (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			தொடர்;
		}
		எனில் (!string_list_has_string(&existing_refs, ref)) {
			printf("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	திரும்பு 0;
}

நிலையான முழு cmd_show_ref__verify(மாறிலி கட்டமைப்பு show_one_options *show_one_opts,
				மாறிலி எழுத்து **refs)
{
	எனில் (!refs || !*refs)
		die("--verify requires a reference");

	போது (*refs) {
		கட்டமைப்பு object_id oid;

		எனில் ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			கட்டமைப்பு reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} இல்லையெனில் எனில் (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} இல்லையெனில் {
			திரும்பு 1;
		}

		refs++;
	}

	திரும்பு 0;
}

கட்டமைப்பு patterns_options {
	முழு show_head;
	முழு branches_only;
	முழு tags_only;
};

நிலையான முழு cmd_show_ref__patterns(மாறிலி கட்டமைப்பு patterns_options *opts,
				  மாறிலி கட்டமைப்பு show_one_options *show_one_opts,
				  மாறிலி எழுத்து **patterns)
{
	கட்டமைப்பு show_ref_data show_ref_data = {
		.show_one_opts = show_one_opts,
		.show_head = opts->show_head,
	};

	எனில் (patterns && *patterns)
		show_ref_data.patterns = patterns;

	எனில் (opts->show_head)
		refs_head_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_data);
	எனில் (opts->branches_only || opts->tags_only) {
		கட்டமைப்பு refs_for_each_ref_options for_each_ref_opts = { 0 };

		எனில் (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}

		எனில் (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}
	} இல்லையெனில் {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_data);
	}
	எனில் (!show_ref_data.found_match)
		திரும்பு 1;

	திரும்பு 0;
}

நிலையான முழு cmd_show_ref__exists(மாறிலி எழுத்து **refs)
{
	கட்டமைப்பு strbuf unused_referent = STRBUF_INIT;
	கட்டமைப்பு object_id unused_oid;
	unsigned முழு unused_type;
	முழு failure_errno = 0;
	மாறிலி எழுத்து *ref;
	முழு ret = 0;

	எனில் (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	எனில் (*refs)
		die("--exists requires exactly one reference");

	எனில் (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		எனில் (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} இல்லையெனில் {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		செல் out;
	}

out:
	strbuf_release(&unused_referent);
	திரும்பு ret;
}

நிலையான முழு hash_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு show_one_options *opts = opt->மதிப்பு;
	கட்டமைப்பு option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	எனில் (!arg)
		திரும்பு 0;

	abbrev_opt.மதிப்பு = &opts->abbrev;
	திரும்பு parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

நிலையான முழு exclude_existing_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
				     முழு unset)
{
	கட்டமைப்பு exclude_existing_options *opts = opt->மதிப்பு;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	திரும்பு 0;
}

முழு cmd_show_ref(முழு argc,
மாறிலி எழுத்து **argv,
மாறிலி எழுத்து *prefix,
கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு exclude_existing_options exclude_existing_opts = {0};
	கட்டமைப்பு patterns_options patterns_opts = {0};
	கட்டமைப்பு show_one_options show_one_opts = {0};
	முழு verify = 0, exists = 0;
	மாறிலி கட்டமைப்பு option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.tags_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', வெற்று, &patterns_opts.show_head,
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

	repo_config(the_repository, git_default_config, வெற்று);

	argc = parse_options(argc, argv, prefix, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	எனில் (exclude_existing_opts.enabled)
		திரும்பு cmd_show_ref__exclude_existing(&exclude_existing_opts);
	இல்லையெனில் எனில் (verify)
		திரும்பு cmd_show_ref__verify(&show_one_opts, argv);
	இல்லையெனில் எனில் (exists)
		திரும்பு cmd_show_ref__exists(argv);
	இல்லையெனில்
		திரும்பு cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
