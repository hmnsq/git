#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "refs/refs-internal.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "object.h"
#شامل "string-list.h"
#شامل "parse-options.h"

ساکن ثابت حرف * ثابت show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	تہی
};

ساخت show_one_options {
	صحیح quiet;
	صحیح hash_only;
	صحیح abbrev;
	صحیح deref_لیبل_فہرست;
};

ساکن خالی show_one(ثابت ساخت show_one_options *opts,
		     ثابت ساخت reference *ref)
{
	ثابت حرف *hex;
	ساخت object_id peeled;

	اگر (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_مقصد_hex(ref->oid));

	اگر (opts->quiet)
		واپس;

	hex = repo_ڈھونڈو_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	اگر (opts->hash_only)
		لکھو("%s\n", hex);
	ورنہ
		لکھو("%s %s\n", hex, ref->name);

	اگر (!opts->deref_لیبل_فہرست)
		واپس;

	اگر (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_ڈھونڈو_unique_abbrev(the_repository, &peeled, opts->abbrev);
		لکھو("%s %s^{}\n", hex, ref->name);
	}
}

ساخت show_ref_ڈیٹا {
	ثابت ساخت show_one_options *show_one_opts;
	ثابت حرف **patterns;
	صحیح ملا_مطابقت;
	صحیح show_سر;
};

ساکن صحیح show_ref(ثابت ساخت reference *ref, خالی *cbdata)
{
	ساخت show_ref_ڈیٹا *ڈیٹا = cbdata;

	اگر (ڈیٹا->show_سر && !strcmp(ref->name, "HEAD"))
		جاؤ مطابقت;

	اگر (ڈیٹا->patterns) {
		صحیح reflen = لڑی_لمبائی(ref->name);
		ثابت حرف **p = ڈیٹا->patterns, *m;
		جب ((m = *p++) != تہی) {
			صحیح len = لڑی_لمبائی(m);
			اگر (len > reflen)
				جاری;
			اگر (memcmp(m, ref->name + reflen - len, len))
				جاری;
			اگر (len == reflen)
				جاؤ مطابقت;
			اگر (ref->name[reflen - len - 1] == '/')
				جاؤ مطابقت;
		}
		واپس 0;
	}

مطابقت:
	ڈیٹا->ملا_مطابقت++;

	show_one(ڈیٹا->show_one_opts, ref);

	واپس 0;
}

ساکن صحیح جوڑو_existing(ثابت ساخت reference *ref, خالی *cbdata)
{
	ساخت string_list *list = (ساخت string_list *)cbdata;
	string_list_داخل_کرو(list, ref->name);
	واپس 0;
}

ساخت exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	صحیح enabled;
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
ساکن صحیح cmd_show_ref__exclude_existing(ثابت ساخت exclude_existing_options *opts)
{
	ساخت string_list existing_refs = STRING_LIST_INIT_DUP;
	حرف buf[1024];
	صحیح patternlen = opts->نمونہ ? لڑی_لمبائی(opts->نمونہ) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), جوڑو_existing,
			  &existing_refs);
	جب (fgets(buf, حجم(buf), stdin)) {
		حرف *ref;
		صحیح len = لڑی_لمبائی(buf);

		اگر (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		اگر (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		برائے (ref = buf + len; buf < ref; ref--)
			اگر (خلا_ہے(ref[-1]))
				توڑو;
		اگر (opts->نمونہ) {
			صحیح reflen = buf + len - ref;
			اگر (reflen < patternlen)
				جاری;
			اگر (لڑی_موازنہ(ref, opts->نمونہ, patternlen))
				جاری;
		}
		اگر (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			جاری;
		}
		اگر (!string_list_has_string(&existing_refs, ref)) {
			لکھو("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	واپس 0;
}

ساکن صحیح cmd_show_ref__verify(ثابت ساخت show_one_options *show_one_opts,
				ثابت حرف **refs)
{
	اگر (!refs || !*refs)
		die("--verify requires a reference");

	جب (*refs) {
		ساخت object_id oid;

		اگر ((starts_with(*refs, "refs/") || refname_is_محفوظ(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			ساخت reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} ورنہ اگر (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} ورنہ {
			واپس 1;
		}

		refs++;
	}

	واپس 0;
}

ساخت patterns_options {
	صحیح show_سر;
	صحیح branches_only;
	صحیح لیبل_فہرست_only;
};

ساکن صحیح cmd_show_ref__patterns(ثابت ساخت patterns_options *opts,
				  ثابت ساخت show_one_options *show_one_opts,
				  ثابت حرف **patterns)
{
	ساخت show_ref_ڈیٹا show_ref_ڈیٹا = {
		.show_one_opts = show_one_opts,
		.show_سر = opts->show_سر,
	};

	اگر (patterns && *patterns)
		show_ref_ڈیٹا.patterns = patterns;

	اگر (opts->show_سر)
		refs_سر_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_ڈیٹا);
	اگر (opts->branches_only || opts->لیبل_فہرست_only) {
		ساخت refs_for_each_ref_options for_each_ref_opts = { 0 };

		اگر (opts->branches_only) {
			for_each_ref_opts.سابقہ = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_ڈیٹا, &for_each_ref_opts);
		}

		اگر (opts->لیبل_فہرست_only) {
			for_each_ref_opts.سابقہ = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_ڈیٹا, &for_each_ref_opts);
		}
	} ورنہ {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_ڈیٹا);
	}
	اگر (!show_ref_ڈیٹا.ملا_مطابقت)
		واپس 1;

	واپس 0;
}

ساکن صحیح cmd_show_ref__exists(ثابت حرف **refs)
{
	ساخت strbuf غیر_مستعمل_referent = STRBUF_INIT;
	ساخت object_id غیر_مستعمل_oid;
	غیرعلامتی صحیح غیر_مستعمل_type;
	صحیح failure_errno = 0;
	ثابت حرف *ref;
	صحیح ret = 0;

	اگر (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	اگر (*refs)
		die("--exists requires exactly one reference");

	اگر (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &غیر_مستعمل_oid, &غیر_مستعمل_referent, &غیر_مستعمل_type,
			      &failure_errno)) {
		اگر (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} ورنہ {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		جاؤ برآمد;
	}

برآمد:
	strbuf_release(&غیر_مستعمل_referent);
	واپس ret;
}

ساکن صحیح hash_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت show_one_options *opts = opt->قدر;
	ساخت option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	اگر (!arg)
		واپس 0;

	abbrev_opt.قدر = &opts->abbrev;
	واپس parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

ساکن صحیح exclude_existing_callback(ثابت ساخت option *opt, ثابت حرف *arg,
				     صحیح unset)
{
	ساخت exclude_existing_options *opts = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->نمونہ = arg;
	واپس 0;
}

صحیح cmd_show_ref(صحیح argc,
ثابت حرف **argv,
ثابت حرف *سابقہ,
ساخت repository *repo غیر_مستعمل)
{
	ساخت exclude_existing_options exclude_existing_opts = {0};
	ساخت patterns_options patterns_opts = {0};
	ساخت show_one_options show_one_opts = {0};
	صحیح verify = 0, exists = 0;
	ثابت ساخت option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.لیبل_فہرست_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', تہی, &patterns_opts.show_سر,
				N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL(0, "head", &patterns_opts.show_سر,
		  N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL('d', "dereference", &show_one_opts.deref_لیبل_فہرست,
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
		OPT_انتہا()
	};

	repo_config(the_repository, git_default_config, تہی);

	argc = parse_options(argc, argv, سابقہ, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	اگر (exclude_existing_opts.enabled)
		واپس cmd_show_ref__exclude_existing(&exclude_existing_opts);
	ورنہ اگر (verify)
		واپس cmd_show_ref__verify(&show_one_opts, argv);
	ورنہ اگر (exists)
		واپس cmd_show_ref__exists(argv);
	ورنہ
		واپس cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
