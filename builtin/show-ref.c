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

स्थिर स्थिरांक अक्षर * स्थिरांक show_ref_usage[] = {
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
	पूर्णांक quiet;
	पूर्णांक hash_only;
	पूर्णांक abbrev;
	पूर्णांक deref_tags;
};

स्थिर शून्य show_one(स्थिरांक संरचना show_one_options *opts,
		     स्थिरांक संरचना reference *ref)
{
	स्थिरांक अक्षर *hex;
	संरचना object_id peeled;

	जर (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	जर (opts->quiet)
		परत;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	जर (opts->hash_only)
		म_लिखो("%s\n", hex);
	अन्यथा
		म_लिखो("%s %s\n", hex, ref->name);

	जर (!opts->deref_tags)
		परत;

	जर (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		म_लिखो("%s %s^{}\n", hex, ref->name);
	}
}

संरचना show_ref_data {
	स्थिरांक संरचना show_one_options *show_one_opts;
	स्थिरांक अक्षर **patterns;
	पूर्णांक found_match;
	पूर्णांक show_head;
};

स्थिर पूर्णांक show_ref(स्थिरांक संरचना reference *ref, शून्य *cbdata)
{
	संरचना show_ref_data *data = cbdata;

	जर (data->show_head && !म_भेद(ref->name, "HEAD"))
		जा match;

	जर (data->patterns) {
		पूर्णांक reflen = म_माप(ref->name);
		स्थिरांक अक्षर **p = data->patterns, *m;
		पर्यंत ((m = *p++) != रिक्त) {
			पूर्णांक len = म_माप(m);
			जर (len > reflen)
				सुरूठेवा;
			जर (स_भेद(m, ref->name + reflen - len, len))
				सुरूठेवा;
			जर (len == reflen)
				जा match;
			जर (ref->name[reflen - len - 1] == '/')
				जा match;
		}
		परत 0;
	}

match:
	data->found_match++;

	show_one(data->show_one_opts, ref);

	परत 0;
}

स्थिर पूर्णांक add_existing(स्थिरांक संरचना reference *ref, शून्य *cbdata)
{
	संरचना string_list *list = (संरचना string_list *)cbdata;
	string_list_घाला(list, ref->name);
	परत 0;
}

संरचना exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	पूर्णांक enabled;
	स्थिरांक अक्षर *pattern;
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
स्थिर पूर्णांक cmd_show_ref__exclude_existing(स्थिरांक संरचना exclude_existing_options *opts)
{
	संरचना string_list existing_refs = STRING_LIST_INIT_DUP;
	अक्षर buf[1024];
	पूर्णांक patternlen = opts->pattern ? म_माप(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	पर्यंत (ख_माला_लो(buf, आकार(buf), मानक_निवेश)) {
		अक्षर *ref;
		पूर्णांक len = म_माप(buf);

		जर (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		जर (3 <= len && !म_भेद(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		साठी (ref = buf + len; buf < ref; ref--)
			जर (है_खाली(ref[-1]))
				थांबा;
		जर (opts->pattern) {
			पूर्णांक reflen = buf + len - ref;
			जर (reflen < patternlen)
				सुरूठेवा;
			जर (म_भेदन(ref, opts->pattern, patternlen))
				सुरूठेवा;
		}
		जर (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			सुरूठेवा;
		}
		जर (!string_list_has_string(&existing_refs, ref)) {
			म_लिखो("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	परत 0;
}

स्थिर पूर्णांक cmd_show_ref__verify(स्थिरांक संरचना show_one_options *show_one_opts,
				स्थिरांक अक्षर **refs)
{
	जर (!refs || !*refs)
		die("--verify requires a reference");

	पर्यंत (*refs) {
		संरचना object_id oid;

		जर ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			संरचना reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} अन्यथा जर (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} अन्यथा {
			परत 1;
		}

		refs++;
	}

	परत 0;
}

संरचना patterns_options {
	पूर्णांक show_head;
	पूर्णांक branches_only;
	पूर्णांक tags_only;
};

स्थिर पूर्णांक cmd_show_ref__patterns(स्थिरांक संरचना patterns_options *opts,
				  स्थिरांक संरचना show_one_options *show_one_opts,
				  स्थिरांक अक्षर **patterns)
{
	संरचना show_ref_data show_ref_data = {
		.show_one_opts = show_one_opts,
		.show_head = opts->show_head,
	};

	जर (patterns && *patterns)
		show_ref_data.patterns = patterns;

	जर (opts->show_head)
		refs_head_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_data);
	जर (opts->branches_only || opts->tags_only) {
		संरचना refs_for_each_ref_options for_each_ref_opts = { 0 };

		जर (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}

		जर (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}
	} अन्यथा {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_data);
	}
	जर (!show_ref_data.found_match)
		परत 1;

	परत 0;
}

स्थिर पूर्णांक cmd_show_ref__exists(स्थिरांक अक्षर **refs)
{
	संरचना strbuf unused_referent = STRBUF_INIT;
	संरचना object_id unused_oid;
	अचिन्हित पूर्णांक unused_type;
	पूर्णांक failure_errno = 0;
	स्थिरांक अक्षर *ref;
	पूर्णांक ret = 0;

	जर (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	जर (*refs)
		die("--exists requires exactly one reference");

	जर (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		जर (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} अन्यथा {
			त्रुटि_सं = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		जा out;
	}

out:
	strbuf_release(&unused_referent);
	परत ret;
}

स्थिर पूर्णांक hash_callback(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना show_one_options *opts = opt->मूल्य;
	संरचना option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	जर (!arg)
		परत 0;

	abbrev_opt.मूल्य = &opts->abbrev;
	परत parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

स्थिर पूर्णांक exclude_existing_callback(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg,
				     पूर्णांक unset)
{
	संरचना exclude_existing_options *opts = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	परत 0;
}

पूर्णांक cmd_show_ref(पूर्णांक argc,
स्थिरांक अक्षर **argv,
स्थिरांक अक्षर *prefix,
संरचना repository *repo UNUSED)
{
	संरचना exclude_existing_options exclude_existing_opts = {0};
	संरचना patterns_options patterns_opts = {0};
	संरचना show_one_options show_one_opts = {0};
	पूर्णांक verify = 0, exists = 0;
	स्थिरांक संरचना option show_ref_options[] = {
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

	जर (exclude_existing_opts.enabled)
		परत cmd_show_ref__exclude_existing(&exclude_existing_opts);
	अन्यथा जर (verify)
		परत cmd_show_ref__verify(&show_one_opts, argv);
	अन्यथा जर (exists)
		परत cmd_show_ref__exists(argv);
	अन्यथा
		परत cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
