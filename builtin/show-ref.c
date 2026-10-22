#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "refs/refs-internal.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "parse-options.h"

स्थिर ध्रुव वर्ण * ध्रुव show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	रिक्त
};

संरचना show_one_options {
	पूर्णाङ्क quiet;
	पूर्णाङ्क hash_only;
	पूर्णाङ्क abbrev;
	पूर्णाङ्क deref_tags;
};

स्थिर शून्य show_one(ध्रुव संरचना show_one_options *opts,
		     ध्रुव संरचना reference *ref)
{
	ध्रुव वर्ण *hex;
	संरचना object_id peeled;

	यदि (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	यदि (opts->quiet)
		प्रत्यागम;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	यदि (opts->hash_only)
		म_लिखो("%s\n", hex);
	अन्यथा
		म_लिखो("%s %s\n", hex, ref->name);

	यदि (!opts->deref_tags)
		प्रत्यागम;

	यदि (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		म_लिखो("%s %s^{}\n", hex, ref->name);
	}
}

संरचना show_ref_data {
	ध्रुव संरचना show_one_options *show_one_opts;
	ध्रुव वर्ण **patterns;
	पूर्णाङ्क found_match;
	पूर्णाङ्क show_head;
};

स्थिर पूर्णाङ्क show_ref(ध्रुव संरचना reference *ref, शून्य *cbdata)
{
	संरचना show_ref_data *data = cbdata;

	यदि (data->show_head && !म_भेद(ref->name, "HEAD"))
		गच्छ match;

	यदि (data->patterns) {
		पूर्णाङ्क reflen = म_माप(ref->name);
		ध्रुव वर्ण **p = data->patterns, *m;
		यावत् ((m = *p++) != रिक्त) {
			पूर्णाङ्क len = म_माप(m);
			यदि (len > reflen)
				अनुवर्त;
			यदि (स_भेद(m, ref->name + reflen - len, len))
				अनुवर्त;
			यदि (len == reflen)
				गच्छ match;
			यदि (ref->name[reflen - len - 1] == '/')
				गच्छ match;
		}
		प्रत्यागम 0;
	}

match:
	data->found_match++;

	show_one(data->show_one_opts, ref);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क add_existing(ध्रुव संरचना reference *ref, शून्य *cbdata)
{
	संरचना string_list *list = (संरचना string_list *)cbdata;
	string_list_जोड़ो(list, ref->name);
	प्रत्यागम 0;
}

संरचना exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	पूर्णाङ्क enabled;
	ध्रुव वर्ण *pattern;
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
स्थिर पूर्णाङ्क cmd_show_ref__exclude_existing(ध्रुव संरचना exclude_existing_options *opts)
{
	संरचना string_list existing_refs = STRING_LIST_INIT_DUP;
	वर्ण buf[1024];
	पूर्णाङ्क patternlen = opts->pattern ? म_माप(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	यावत् (ख_माला_लो(buf, परिमाण(buf), मानक_निवेश)) {
		वर्ण *ref;
		पूर्णाङ्क len = म_माप(buf);

		यदि (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		यदि (3 <= len && !म_भेद(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		प्रति (ref = buf + len; buf < ref; ref--)
			यदि (है_खाली(ref[-1]))
				भङ्ग;
		यदि (opts->pattern) {
			पूर्णाङ्क reflen = buf + len - ref;
			यदि (reflen < patternlen)
				अनुवर्त;
			यदि (म_भेदन(ref, opts->pattern, patternlen))
				अनुवर्त;
		}
		यदि (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			अनुवर्त;
		}
		यदि (!string_list_has_string(&existing_refs, ref)) {
			म_लिखो("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cmd_show_ref__verify(ध्रुव संरचना show_one_options *show_one_opts,
				ध्रुव वर्ण **refs)
{
	यदि (!refs || !*refs)
		die("--verify requires a reference");

	यावत् (*refs) {
		संरचना object_id oid;

		यदि ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			संरचना reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} अन्यथा यदि (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} अन्यथा {
			प्रत्यागम 1;
		}

		refs++;
	}

	प्रत्यागम 0;
}

संरचना patterns_options {
	पूर्णाङ्क show_head;
	पूर्णाङ्क branches_only;
	पूर्णाङ्क tags_only;
};

स्थिर पूर्णाङ्क cmd_show_ref__patterns(ध्रुव संरचना patterns_options *opts,
				  ध्रुव संरचना show_one_options *show_one_opts,
				  ध्रुव वर्ण **patterns)
{
	संरचना show_ref_data show_ref_data = {
		.show_one_opts = show_one_opts,
		.show_head = opts->show_head,
	};

	यदि (patterns && *patterns)
		show_ref_data.patterns = patterns;

	यदि (opts->show_head)
		refs_head_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_data);
	यदि (opts->branches_only || opts->tags_only) {
		संरचना refs_for_each_ref_options for_each_ref_opts = { 0 };

		यदि (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}

		यदि (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}
	} अन्यथा {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_data);
	}
	यदि (!show_ref_data.found_match)
		प्रत्यागम 1;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cmd_show_ref__exists(ध्रुव वर्ण **refs)
{
	संरचना strbuf unused_referent = STRBUF_INIT;
	संरचना object_id unused_oid;
	अचिन्हित पूर्णाङ्क unused_type;
	पूर्णाङ्क failure_errno = 0;
	ध्रुव वर्ण *ref;
	पूर्णाङ्क ret = 0;

	यदि (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	यदि (*refs)
		die("--exists requires exactly one reference");

	यदि (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		यदि (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} अन्यथा {
			त्रुटि_सं = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		गच्छ out;
	}

out:
	strbuf_release(&unused_referent);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क hash_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना show_one_options *opts = opt->मूल्य;
	संरचना option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	यदि (!arg)
		प्रत्यागम 0;

	abbrev_opt.मूल्य = &opts->abbrev;
	प्रत्यागम parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

स्थिर पूर्णाङ्क exclude_existing_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				     पूर्णाङ्क unset)
{
	संरचना exclude_existing_options *opts = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_show_ref(पूर्णाङ्क argc,
ध्रुव वर्ण **argv,
ध्रुव वर्ण *prefix,
संरचना repository *repo UNUSED)
{
	संरचना exclude_existing_options exclude_existing_opts = {0};
	संरचना patterns_options patterns_opts = {0};
	संरचना show_one_options show_one_opts = {0};
	पूर्णाङ्क verify = 0, exists = 0;
	ध्रुव संरचना option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.tags_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', रिक्त, &patterns_opts.show_head,
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

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, prefix, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	यदि (exclude_existing_opts.enabled)
		प्रत्यागम cmd_show_ref__exclude_existing(&exclude_existing_opts);
	अन्यथा यदि (verify)
		प्रत्यागम cmd_show_ref__verify(&show_one_opts, argv);
	अन्यथा यदि (exists)
		प्रत्यागम cmd_show_ref__exists(argv);
	अन्यथा
		प्रत्यागम cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
