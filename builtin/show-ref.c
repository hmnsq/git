#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "refs/refs-internal.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "object.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	ଖାଲି
};

ସଂରଚନା show_one_options {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash_only;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା abbrev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା deref_tags;
};

ସ୍ଥିର ଶୂନ୍ୟ show_one(ଅଚଳ ସଂରଚନା show_one_options *opts,
		     ଅଚଳ ସଂରଚନା reference *ref)
{
	ଅଚଳ ଅକ୍ଷର *hex;
	ସଂରଚନା object_id peeled;

	ଯଦି (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	ଯଦି (opts->quiet)
		ଫେରନ୍ତୁ;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	ଯଦି (opts->hash_only)
		printf("%s\n", hex);
	ଅନ୍ୟଥା
		printf("%s %s\n", hex, ref->name);

	ଯଦି (!opts->deref_tags)
		ଫେରନ୍ତୁ;

	ଯଦି (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		printf("%s %s^{}\n", hex, ref->name);
	}
}

ସଂରଚନା show_ref_data {
	ଅଚଳ ସଂରଚନା show_one_options *show_one_opts;
	ଅଚଳ ଅକ୍ଷର **patterns;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା found_match;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_head;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ref(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cbdata)
{
	ସଂରଚନା show_ref_data *data = cbdata;

	ଯଦି (data->show_head && !strcmp(ref->name, "HEAD"))
		ଯାଅ match;

	ଯଦି (data->patterns) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା reflen = strlen(ref->name);
		ଅଚଳ ଅକ୍ଷର **p = data->patterns, *m;
		ଯେତେବେଳେ ((m = *p++) != ଖାଲି) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = strlen(m);
			ଯଦି (len > reflen)
				ଜାରି;
			ଯଦି (memcmp(m, ref->name + reflen - len, len))
				ଜାରି;
			ଯଦି (len == reflen)
				ଯାଅ match;
			ଯଦି (ref->name[reflen - len - 1] == '/')
				ଯାଅ match;
		}
		ଫେରନ୍ତୁ 0;
	}

match:
	data->found_match++;

	show_one(data->show_one_opts, ref);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_existing(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cbdata)
{
	ସଂରଚନା string_list *list = (ସଂରଚନା string_list *)cbdata;
	string_list_ଭର୍ତ୍ତି(list, ref->name);
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା enabled;
	ଅଚଳ ଅକ୍ଷର *pattern;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_show_ref__exclude_existing(ଅଚଳ ସଂରଚନା exclude_existing_options *opts)
{
	ସଂରଚନା string_list existing_refs = STRING_LIST_INIT_DUP;
	ଅକ୍ଷର buf[1024];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା patternlen = opts->pattern ? strlen(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	ଯେତେବେଳେ (fgets(buf, ଆକାର(buf), stdin)) {
		ଅକ୍ଷର *ref;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = strlen(buf);

		ଯଦି (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		ଯଦି (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		ପାଇଁ (ref = buf + len; buf < ref; ref--)
			ଯଦି (isspace(ref[-1]))
				ଭାଙ୍ଗ;
		ଯଦି (opts->pattern) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା reflen = buf + len - ref;
			ଯଦି (reflen < patternlen)
				ଜାରି;
			ଯଦି (strncmp(ref, opts->pattern, patternlen))
				ଜାରି;
		}
		ଯଦି (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			ଜାରି;
		}
		ଯଦି (!string_list_has_string(&existing_refs, ref)) {
			printf("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_show_ref__verify(ଅଚଳ ସଂରଚନା show_one_options *show_one_opts,
				ଅଚଳ ଅକ୍ଷର **refs)
{
	ଯଦି (!refs || !*refs)
		die("--verify requires a reference");

	ଯେତେବେଳେ (*refs) {
		ସଂରଚନା object_id oid;

		ଯଦି ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			ସଂରଚନା reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} ଅନ୍ୟଥା ଯଦି (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} ଅନ୍ୟଥା {
			ଫେରନ୍ତୁ 1;
		}

		refs++;
	}

	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା patterns_options {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_head;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା branches_only;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା tags_only;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_show_ref__patterns(ଅଚଳ ସଂରଚନା patterns_options *opts,
				  ଅଚଳ ସଂରଚନା show_one_options *show_one_opts,
				  ଅଚଳ ଅକ୍ଷର **patterns)
{
	ସଂରଚନା show_ref_data show_ref_data = {
		.show_one_opts = show_one_opts,
		.show_head = opts->show_head,
	};

	ଯଦି (patterns && *patterns)
		show_ref_data.patterns = patterns;

	ଯଦି (opts->show_head)
		refs_head_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_data);
	ଯଦି (opts->branches_only || opts->tags_only) {
		ସଂରଚନା refs_for_each_ref_options for_each_ref_opts = { 0 };

		ଯଦି (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}

		ଯଦି (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}
	} ଅନ୍ୟଥା {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_data);
	}
	ଯଦି (!show_ref_data.found_match)
		ଫେରନ୍ତୁ 1;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_show_ref__exists(ଅଚଳ ଅକ୍ଷର **refs)
{
	ସଂରଚନା strbuf unused_referent = STRBUF_INIT;
	ସଂରଚନା object_id unused_oid;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା unused_type;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା failure_errno = 0;
	ଅଚଳ ଅକ୍ଷର *ref;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ଯଦି (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	ଯଦି (*refs)
		die("--exists requires exactly one reference");

	ଯଦି (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		ଯଦି (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} ଅନ୍ୟଥା {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		ଯାଅ out;
	}

out:
	strbuf_release(&unused_referent);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା show_one_options *opts = opt->ମୂଲ୍ୟ;
	ସଂରଚନା option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	ଯଦି (!arg)
		ଫେରନ୍ତୁ 0;

	abbrev_opt.ମୂଲ୍ୟ = &opts->abbrev;
	ଫେରନ୍ତୁ parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude_existing_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା exclude_existing_options *opts = opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_show_ref(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
ଅଚଳ ଅକ୍ଷର **argv,
ଅଚଳ ଅକ୍ଷର *prefix,
ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା exclude_existing_options exclude_existing_opts = {0};
	ସଂରଚନା patterns_options patterns_opts = {0};
	ସଂରଚନା show_one_options show_one_opts = {0};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା verify = 0, exists = 0;
	ଅଚଳ ସଂରଚନା option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.tags_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', ଖାଲି, &patterns_opts.show_head,
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

	repo_config(the_repository, git_default_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	ଯଦି (exclude_existing_opts.enabled)
		ଫେରନ୍ତୁ cmd_show_ref__exclude_existing(&exclude_existing_opts);
	ଅନ୍ୟଥା ଯଦି (verify)
		ଫେରନ୍ତୁ cmd_show_ref__verify(&show_one_opts, argv);
	ଅନ୍ୟଥା ଯଦି (exists)
		ଫେରନ୍ତୁ cmd_show_ref__exists(argv);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
