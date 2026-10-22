#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "refs/refs-internal.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "object.h"
#ضمن "string-list.h"
#ضمن "parse-options.h"

ساكن ثابت حرف * ثابت show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	عدم
};

بنية show_one_options {
	صحيح quiet;
	صحيح hash_only;
	صحيح abbrev;
	صحيح deref_ٹیگ_سوچی;
};

ساكن فراغ show_one(ثابت بنية show_one_options *opts,
		     ثابت بنية reference *ref)
{
	ثابت حرف *hex;
	بنية object_id peeled;

	إذا (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_منزل_hex(ref->oid));

	إذا (opts->quiet)
		أرجع;

	hex = repo_لبھو_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	إذا (opts->hash_only)
		لکھو("%s\n", hex);
	وإلا
		لکھو("%s %s\n", hex, ref->name);

	إذا (!opts->deref_ٹیگ_سوچی)
		أرجع;

	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_لبھو_unique_abbrev(the_repository, &peeled, opts->abbrev);
		لکھو("%s %s^{}\n", hex, ref->name);
	}
}

بنية show_ref_انکڑے {
	ثابت بنية show_one_options *show_one_opts;
	ثابت حرف **patterns;
	صحيح لبھیا_میل;
	صحيح show_سر;
};

ساكن صحيح show_ref(ثابت بنية reference *ref, فراغ *cbdata)
{
	بنية show_ref_انکڑے *انکڑے = cbdata;

	إذا (انکڑے->show_سر && !strcmp(ref->name, "HEAD"))
		اذهب میل;

	إذا (انکڑے->patterns) {
		صحيح reflen = لڑی_لمبائی(ref->name);
		ثابت حرف **p = انکڑے->patterns, *m;
		طالما ((m = *p++) != عدم) {
			صحيح len = لڑی_لمبائی(m);
			إذا (len > reflen)
				استمر;
			إذا (memcmp(m, ref->name + reflen - len, len))
				استمر;
			إذا (len == reflen)
				اذهب میل;
			إذا (ref->name[reflen - len - 1] == '/')
				اذهب میل;
		}
		أرجع 0;
	}

میل:
	انکڑے->لبھیا_میل++;

	show_one(انکڑے->show_one_opts, ref);

	أرجع 0;
}

ساكن صحيح جوڑو_existing(ثابت بنية reference *ref, فراغ *cbdata)
{
	بنية string_list *list = (بنية string_list *)cbdata;
	string_list_داخل_کرو(list, ref->name);
	أرجع 0;
}

بنية exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	صحيح enabled;
	ثابت حرف *نمونہ;
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
ساكن صحيح cmd_show_ref__exclude_existing(ثابت بنية exclude_existing_options *opts)
{
	بنية string_list existing_refs = STRING_LIST_INIT_DUP;
	حرف buf[1024];
	صحيح patternlen = opts->نمونہ ? لڑی_لمبائی(opts->نمونہ) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), جوڑو_existing,
			  &existing_refs);
	طالما (fgets(buf, حجم(buf), stdin)) {
		حرف *ref;
		صحيح len = لڑی_لمبائی(buf);

		إذا (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		إذا (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		لكل (ref = buf + len; buf < ref; ref--)
			إذا (خلا_اے(ref[-1]))
				اكسر;
		إذا (opts->نمونہ) {
			صحيح reflen = buf + len - ref;
			إذا (reflen < patternlen)
				استمر;
			إذا (لڑی_مقابلہ(ref, opts->نمونہ, patternlen))
				استمر;
		}
		إذا (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			استمر;
		}
		إذا (!string_list_has_string(&existing_refs, ref)) {
			لکھو("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	أرجع 0;
}

ساكن صحيح cmd_show_ref__verify(ثابت بنية show_one_options *show_one_opts,
				ثابت حرف **refs)
{
	إذا (!refs || !*refs)
		die("--verify requires a reference");

	طالما (*refs) {
		بنية object_id oid;

		إذا ((starts_with(*refs, "refs/") || refname_is_محفوظ(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			بنية reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} وإلا إذا (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} وإلا {
			أرجع 1;
		}

		refs++;
	}

	أرجع 0;
}

بنية patterns_options {
	صحيح show_سر;
	صحيح branches_only;
	صحيح ٹیگ_سوچی_only;
};

ساكن صحيح cmd_show_ref__patterns(ثابت بنية patterns_options *opts,
				  ثابت بنية show_one_options *show_one_opts,
				  ثابت حرف **patterns)
{
	بنية show_ref_انکڑے show_ref_انکڑے = {
		.show_one_opts = show_one_opts,
		.show_سر = opts->show_سر,
	};

	إذا (patterns && *patterns)
		show_ref_انکڑے.patterns = patterns;

	إذا (opts->show_سر)
		refs_سر_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_انکڑے);
	إذا (opts->branches_only || opts->ٹیگ_سوچی_only) {
		بنية refs_for_each_ref_options for_each_ref_opts = { 0 };

		إذا (opts->branches_only) {
			for_each_ref_opts.اگیتر = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_انکڑے, &for_each_ref_opts);
		}

		إذا (opts->ٹیگ_سوچی_only) {
			for_each_ref_opts.اگیتر = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_انکڑے, &for_each_ref_opts);
		}
	} وإلا {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_انکڑے);
	}
	إذا (!show_ref_انکڑے.لبھیا_میل)
		أرجع 1;

	أرجع 0;
}

ساكن صحيح cmd_show_ref__exists(ثابت حرف **refs)
{
	بنية strbuf بن_ورتیا_referent = STRBUF_INIT;
	بنية object_id بن_ورتیا_oid;
	بنانشانی صحيح بن_ورتیا_type;
	صحيح failure_errno = 0;
	ثابت حرف *ref;
	صحيح ret = 0;

	إذا (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	إذا (*refs)
		die("--exists requires exactly one reference");

	إذا (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &بن_ورتیا_oid, &بن_ورتیا_referent, &بن_ورتیا_type,
			      &failure_errno)) {
		إذا (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} وإلا {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		اذهب باہرلا;
	}

باہرلا:
	strbuf_release(&بن_ورتیا_referent);
	أرجع ret;
}

ساكن صحيح hash_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية show_one_options *opts = opt->قدر;
	بنية option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	إذا (!arg)
		أرجع 0;

	abbrev_opt.قدر = &opts->abbrev;
	أرجع parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

ساكن صحيح exclude_existing_callback(ثابت بنية option *opt, ثابت حرف *arg,
				     صحيح unset)
{
	بنية exclude_existing_options *opts = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->نمونہ = arg;
	أرجع 0;
}

صحيح cmd_show_ref(صحيح argc,
ثابت حرف **argv,
ثابت حرف *اگیتر,
بنية repository *repo بن_ورتیا)
{
	بنية exclude_existing_options exclude_existing_opts = {0};
	بنية patterns_options patterns_opts = {0};
	بنية show_one_options show_one_opts = {0};
	صحيح verify = 0, exists = 0;
	ثابت بنية option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.ٹیگ_سوچی_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', عدم, &patterns_opts.show_سر,
				N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL(0, "head", &patterns_opts.show_سر,
		  N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL('d', "dereference", &show_one_opts.deref_ٹیگ_سوچی,
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
		OPT_سرا()
	};

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, اگیتر, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	إذا (exclude_existing_opts.enabled)
		أرجع cmd_show_ref__exclude_existing(&exclude_existing_opts);
	وإلا إذا (verify)
		أرجع cmd_show_ref__verify(&show_one_opts, argv);
	وإلا إذا (exists)
		أرجع cmd_show_ref__exists(argv);
	وإلا
		أرجع cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
