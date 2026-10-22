#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "refs/refs-internal.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "object.h"
#समावेश "string-list.h"
#समावेश "parse-options.h"

अटल स्थिर अक्षर * स्थिर show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	शून्य
};

काष्ठा show_one_options {
	पूर्णांक quiet;
	पूर्णांक hash_only;
	पूर्णांक abbrev;
	पूर्णांक deref_tags;
};

अटल व्योम show_one(स्थिर काष्ठा show_one_options *opts,
		     स्थिर काष्ठा reference *ref)
{
	स्थिर अक्षर *hex;
	काष्ठा object_id peeled;

	अगर (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	अगर (opts->quiet)
		वापस;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	अगर (opts->hash_only)
		म_लिखो("%s\n", hex);
	अन्यथा
		म_लिखो("%s %s\n", hex, ref->name);

	अगर (!opts->deref_tags)
		वापस;

	अगर (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		म_लिखो("%s %s^{}\n", hex, ref->name);
	}
}

काष्ठा show_ref_तथ्य {
	स्थिर काष्ठा show_one_options *show_one_opts;
	स्थिर अक्षर **patterns;
	पूर्णांक found_match;
	पूर्णांक show_शीर्ष;
};

अटल पूर्णांक show_ref(स्थिर काष्ठा reference *ref, व्योम *cbdata)
{
	काष्ठा show_ref_तथ्य *तथ्य = cbdata;

	अगर (तथ्य->show_शीर्ष && !म_भेद(ref->name, "HEAD"))
		जाओ match;

	अगर (तथ्य->patterns) {
		पूर्णांक reflen = म_माप(ref->name);
		स्थिर अक्षर **p = तथ्य->patterns, *m;
		जबतक ((m = *p++) != शून्य) {
			पूर्णांक len = म_माप(m);
			अगर (len > reflen)
				जारी;
			अगर (स_भेद(m, ref->name + reflen - len, len))
				जारी;
			अगर (len == reflen)
				जाओ match;
			अगर (ref->name[reflen - len - 1] == '/')
				जाओ match;
		}
		वापस 0;
	}

match:
	तथ्य->found_match++;

	show_one(तथ्य->show_one_opts, ref);

	वापस 0;
}

अटल पूर्णांक add_existing(स्थिर काष्ठा reference *ref, व्योम *cbdata)
{
	काष्ठा string_list *list = (काष्ठा string_list *)cbdata;
	string_list_जोड़ो(list, ref->name);
	वापस 0;
}

काष्ठा exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	पूर्णांक enabled;
	स्थिर अक्षर *pattern;
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
अटल पूर्णांक cmd_show_ref__exclude_existing(स्थिर काष्ठा exclude_existing_options *opts)
{
	काष्ठा string_list existing_refs = STRING_LIST_INIT_DUP;
	अक्षर buf[1024];
	पूर्णांक patternlen = opts->pattern ? म_माप(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	जबतक (ख_माला_लो(buf, माप(buf), मानक_निवेश)) {
		अक्षर *ref;
		पूर्णांक len = म_माप(buf);

		अगर (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		अगर (3 <= len && !म_भेद(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		क्रम (ref = buf + len; buf < ref; ref--)
			अगर (है_खाली(ref[-1]))
				अवरोध;
		अगर (opts->pattern) {
			पूर्णांक reflen = buf + len - ref;
			अगर (reflen < patternlen)
				जारी;
			अगर (म_भेदन(ref, opts->pattern, patternlen))
				जारी;
		}
		अगर (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			जारी;
		}
		अगर (!string_list_has_string(&existing_refs, ref)) {
			म_लिखो("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	वापस 0;
}

अटल पूर्णांक cmd_show_ref__verify(स्थिर काष्ठा show_one_options *show_one_opts,
				स्थिर अक्षर **refs)
{
	अगर (!refs || !*refs)
		die("--verify requires a reference");

	जबतक (*refs) {
		काष्ठा object_id oid;

		अगर ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			काष्ठा reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} अन्यथा अगर (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} अन्यथा {
			वापस 1;
		}

		refs++;
	}

	वापस 0;
}

काष्ठा patterns_options {
	पूर्णांक show_शीर्ष;
	पूर्णांक branches_only;
	पूर्णांक tags_only;
};

अटल पूर्णांक cmd_show_ref__patterns(स्थिर काष्ठा patterns_options *opts,
				  स्थिर काष्ठा show_one_options *show_one_opts,
				  स्थिर अक्षर **patterns)
{
	काष्ठा show_ref_तथ्य show_ref_तथ्य = {
		.show_one_opts = show_one_opts,
		.show_शीर्ष = opts->show_शीर्ष,
	};

	अगर (patterns && *patterns)
		show_ref_तथ्य.patterns = patterns;

	अगर (opts->show_शीर्ष)
		refs_शीर्ष_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_तथ्य);
	अगर (opts->branches_only || opts->tags_only) {
		काष्ठा refs_for_each_ref_options for_each_ref_opts = { 0 };

		अगर (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_तथ्य, &for_each_ref_opts);
		}

		अगर (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_तथ्य, &for_each_ref_opts);
		}
	} अन्यथा {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_तथ्य);
	}
	अगर (!show_ref_तथ्य.found_match)
		वापस 1;

	वापस 0;
}

अटल पूर्णांक cmd_show_ref__exists(स्थिर अक्षर **refs)
{
	काष्ठा strbuf unused_referent = STRBUF_INIT;
	काष्ठा object_id unused_oid;
	अचिन्हित पूर्णांक unused_type;
	पूर्णांक failure_errno = 0;
	स्थिर अक्षर *ref;
	पूर्णांक ret = 0;

	अगर (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	अगर (*refs)
		die("--exists requires exactly one reference");

	अगर (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		अगर (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} अन्यथा {
			त्रुटि_सं = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		जाओ out;
	}

out:
	strbuf_release(&unused_referent);
	वापस ret;
}

अटल पूर्णांक hash_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा show_one_options *opts = opt->मूल्य;
	काष्ठा option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	अगर (!arg)
		वापस 0;

	abbrev_opt.मूल्य = &opts->abbrev;
	वापस parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

अटल पूर्णांक exclude_existing_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
				     पूर्णांक unset)
{
	काष्ठा exclude_existing_options *opts = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	वापस 0;
}

पूर्णांक cmd_show_ref(पूर्णांक argc,
स्थिर अक्षर **argv,
स्थिर अक्षर *prefix,
काष्ठा repository *repo UNUSED)
{
	काष्ठा exclude_existing_options exclude_existing_opts = {0};
	काष्ठा patterns_options patterns_opts = {0};
	काष्ठा show_one_options show_one_opts = {0};
	पूर्णांक verify = 0, exists = 0;
	स्थिर काष्ठा option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.tags_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', शून्य, &patterns_opts.show_शीर्ष,
				N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL(0, "head", &patterns_opts.show_शीर्ष,
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

	repo_config(the_repository, git_default_config, शून्य);

	argc = parse_options(argc, argv, prefix, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	अगर (exclude_existing_opts.enabled)
		वापस cmd_show_ref__exclude_existing(&exclude_existing_opts);
	अन्यथा अगर (verify)
		वापस cmd_show_ref__verify(&show_one_opts, argv);
	अन्यथा अगर (exists)
		वापस cmd_show_ref__exists(argv);
	अन्यथा
		वापस cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
