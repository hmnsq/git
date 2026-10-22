#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "refs/refs-internal.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "object.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "parse-options.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	ശൂന്യ
};

ഘടന show_one_options {
	പൂർണ്ണസംഖ്യ quiet;
	പൂർണ്ണസംഖ്യ hash_only;
	പൂർണ്ണസംഖ്യ abbrev;
	പൂർണ്ണസംഖ്യ deref_tags;
};

സ്ഥിതം ശൂന്യം show_one(സ്ഥിരം ഘടന show_one_options *opts,
		     സ്ഥിരം ഘടന reference *ref)
{
	സ്ഥിരം അക്ഷരം *hex;
	ഘടന object_id peeled;

	എങ്കിൽ (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	എങ്കിൽ (opts->quiet)
		തിരികെ;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	എങ്കിൽ (opts->hash_only)
		printf("%s\n", hex);
	അല്ലെങ്കിൽ
		printf("%s %s\n", hex, ref->name);

	എങ്കിൽ (!opts->deref_tags)
		തിരികെ;

	എങ്കിൽ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		printf("%s %s^{}\n", hex, ref->name);
	}
}

ഘടന show_ref_data {
	സ്ഥിരം ഘടന show_one_options *show_one_opts;
	സ്ഥിരം അക്ഷരം **patterns;
	പൂർണ്ണസംഖ്യ found_match;
	പൂർണ്ണസംഖ്യ show_head;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ show_ref(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cbdata)
{
	ഘടന show_ref_data *data = cbdata;

	എങ്കിൽ (data->show_head && !strcmp(ref->name, "HEAD"))
		പോകുക match;

	എങ്കിൽ (data->patterns) {
		പൂർണ്ണസംഖ്യ reflen = strlen(ref->name);
		സ്ഥിരം അക്ഷരം **p = data->patterns, *m;
		ആകുമ്പോൾ ((m = *p++) != ശൂന്യ) {
			പൂർണ്ണസംഖ്യ len = strlen(m);
			എങ്കിൽ (len > reflen)
				തുടരുക;
			എങ്കിൽ (memcmp(m, ref->name + reflen - len, len))
				തുടരുക;
			എങ്കിൽ (len == reflen)
				പോകുക match;
			എങ്കിൽ (ref->name[reflen - len - 1] == '/')
				പോകുക match;
		}
		തിരികെ 0;
	}

match:
	data->found_match++;

	show_one(data->show_one_opts, ref);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_existing(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cbdata)
{
	ഘടന string_list *list = (ഘടന string_list *)cbdata;
	string_list_തിരുകുക(list, ref->name);
	തിരികെ 0;
}

ഘടന exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	പൂർണ്ണസംഖ്യ enabled;
	സ്ഥിരം അക്ഷരം *pattern;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_show_ref__exclude_existing(സ്ഥിരം ഘടന exclude_existing_options *opts)
{
	ഘടന string_list existing_refs = STRING_LIST_INIT_DUP;
	അക്ഷരം buf[1024];
	പൂർണ്ണസംഖ്യ patternlen = opts->pattern ? strlen(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	ആകുമ്പോൾ (fgets(buf, വലുപ്പം(buf), stdin)) {
		അക്ഷരം *ref;
		പൂർണ്ണസംഖ്യ len = strlen(buf);

		എങ്കിൽ (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		എങ്കിൽ (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		വേണ്ടി (ref = buf + len; buf < ref; ref--)
			എങ്കിൽ (isspace(ref[-1]))
				നിർത്തുക;
		എങ്കിൽ (opts->pattern) {
			പൂർണ്ണസംഖ്യ reflen = buf + len - ref;
			എങ്കിൽ (reflen < patternlen)
				തുടരുക;
			എങ്കിൽ (strncmp(ref, opts->pattern, patternlen))
				തുടരുക;
		}
		എങ്കിൽ (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			തുടരുക;
		}
		എങ്കിൽ (!string_list_has_string(&existing_refs, ref)) {
			printf("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_show_ref__verify(സ്ഥിരം ഘടന show_one_options *show_one_opts,
				സ്ഥിരം അക്ഷരം **refs)
{
	എങ്കിൽ (!refs || !*refs)
		die("--verify requires a reference");

	ആകുമ്പോൾ (*refs) {
		ഘടന object_id oid;

		എങ്കിൽ ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			ഘടന reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} അല്ലെങ്കിൽ എങ്കിൽ (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} അല്ലെങ്കിൽ {
			തിരികെ 1;
		}

		refs++;
	}

	തിരികെ 0;
}

ഘടന patterns_options {
	പൂർണ്ണസംഖ്യ show_head;
	പൂർണ്ണസംഖ്യ branches_only;
	പൂർണ്ണസംഖ്യ tags_only;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_show_ref__patterns(സ്ഥിരം ഘടന patterns_options *opts,
				  സ്ഥിരം ഘടന show_one_options *show_one_opts,
				  സ്ഥിരം അക്ഷരം **patterns)
{
	ഘടന show_ref_data show_ref_data = {
		.show_one_opts = show_one_opts,
		.show_head = opts->show_head,
	};

	എങ്കിൽ (patterns && *patterns)
		show_ref_data.patterns = patterns;

	എങ്കിൽ (opts->show_head)
		refs_head_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_data);
	എങ്കിൽ (opts->branches_only || opts->tags_only) {
		ഘടന refs_for_each_ref_options for_each_ref_opts = { 0 };

		എങ്കിൽ (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}

		എങ്കിൽ (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}
	} അല്ലെങ്കിൽ {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_data);
	}
	എങ്കിൽ (!show_ref_data.found_match)
		തിരികെ 1;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_show_ref__exists(സ്ഥിരം അക്ഷരം **refs)
{
	ഘടന strbuf unused_referent = STRBUF_INIT;
	ഘടന object_id unused_oid;
	unsigned പൂർണ്ണസംഖ്യ unused_type;
	പൂർണ്ണസംഖ്യ failure_errno = 0;
	സ്ഥിരം അക്ഷരം *ref;
	പൂർണ്ണസംഖ്യ ret = 0;

	എങ്കിൽ (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	എങ്കിൽ (*refs)
		die("--exists requires exactly one reference");

	എങ്കിൽ (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		എങ്കിൽ (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} അല്ലെങ്കിൽ {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		പോകുക out;
	}

out:
	strbuf_release(&unused_referent);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ hash_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന show_one_options *opts = opt->മൂല്യം;
	ഘടന option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	എങ്കിൽ (!arg)
		തിരികെ 0;

	abbrev_opt.മൂല്യം = &opts->abbrev;
	തിരികെ parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ exclude_existing_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
				     പൂർണ്ണസംഖ്യ unset)
{
	ഘടന exclude_existing_options *opts = opt->മൂല്യം;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_show_ref(പൂർണ്ണസംഖ്യ argc,
സ്ഥിരം അക്ഷരം **argv,
സ്ഥിരം അക്ഷരം *prefix,
ഘടന repository *repo UNUSED)
{
	ഘടന exclude_existing_options exclude_existing_opts = {0};
	ഘടന patterns_options patterns_opts = {0};
	ഘടന show_one_options show_one_opts = {0};
	പൂർണ്ണസംഖ്യ verify = 0, exists = 0;
	സ്ഥിരം ഘടന option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.tags_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', ശൂന്യ, &patterns_opts.show_head,
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

	repo_config(the_repository, git_default_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	എങ്കിൽ (exclude_existing_opts.enabled)
		തിരികെ cmd_show_ref__exclude_existing(&exclude_existing_opts);
	അല്ലെങ്കിൽ എങ്കിൽ (verify)
		തിരികെ cmd_show_ref__verify(&show_one_opts, argv);
	അല്ലെങ്കിൽ എങ്കിൽ (exists)
		തിരികെ cmd_show_ref__exists(argv);
	അല്ലെങ്കിൽ
		തിരികെ cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
