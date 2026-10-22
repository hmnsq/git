#define USE_THE_REPOSITORY_VARIABLE
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

ایستا ثابت نویسه * ثابت show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	پوچ
};

ساختار show_one_options {
	صحیح quiet;
	صحیح hash_only;
	صحیح abbrev;
	صحیح deref_tags;
};

ایستا تهی show_one(ثابت ساختار show_one_options *opts,
		     ثابت ساختار reference *ref)
{
	ثابت نویسه *hex;
	ساختار object_id peeled;

	اگر (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	اگر (opts->quiet)
		بازگشت;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	اگر (opts->hash_only)
		printf("%s\n", hex);
	وگرنه
		printf("%s %s\n", hex, ref->name);

	اگر (!opts->deref_tags)
		بازگشت;

	اگر (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		printf("%s %s^{}\n", hex, ref->name);
	}
}

ساختار show_ref_data {
	ثابت ساختار show_one_options *show_one_opts;
	ثابت نویسه **patterns;
	صحیح found_match;
	صحیح show_head;
};

ایستا صحیح show_ref(ثابت ساختار reference *ref, تهی *cbdata)
{
	ساختار show_ref_data *data = cbdata;

	اگر (data->show_head && !strcmp(ref->name, "HEAD"))
		برو match;

	اگر (data->patterns) {
		صحیح reflen = strlen(ref->name);
		ثابت نویسه **p = data->patterns, *m;
		وقتی ((m = *p++) != پوچ) {
			صحیح len = strlen(m);
			اگر (len > reflen)
				ادامه;
			اگر (memcmp(m, ref->name + reflen - len, len))
				ادامه;
			اگر (len == reflen)
				برو match;
			اگر (ref->name[reflen - len - 1] == '/')
				برو match;
		}
		بازگشت 0;
	}

match:
	data->found_match++;

	show_one(data->show_one_opts, ref);

	بازگشت 0;
}

ایستا صحیح add_existing(ثابت ساختار reference *ref, تهی *cbdata)
{
	ساختار string_list *list = (ساختار string_list *)cbdata;
	string_list_درج(list, ref->name);
	بازگشت 0;
}

ساختار exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	صحیح enabled;
	ثابت نویسه *pattern;
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
ایستا صحیح cmd_show_ref__exclude_existing(ثابت ساختار exclude_existing_options *opts)
{
	ساختار string_list existing_refs = STRING_LIST_INIT_DUP;
	نویسه buf[1024];
	صحیح patternlen = opts->pattern ? strlen(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	وقتی (fgets(buf, اندازه(buf), stdin)) {
		نویسه *ref;
		صحیح len = strlen(buf);

		اگر (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		اگر (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		برای (ref = buf + len; buf < ref; ref--)
			اگر (isspace(ref[-1]))
				بشکن;
		اگر (opts->pattern) {
			صحیح reflen = buf + len - ref;
			اگر (reflen < patternlen)
				ادامه;
			اگر (strncmp(ref, opts->pattern, patternlen))
				ادامه;
		}
		اگر (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			ادامه;
		}
		اگر (!string_list_has_string(&existing_refs, ref)) {
			printf("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	بازگشت 0;
}

ایستا صحیح cmd_show_ref__verify(ثابت ساختار show_one_options *show_one_opts,
				ثابت نویسه **refs)
{
	اگر (!refs || !*refs)
		die("--verify requires a reference");

	وقتی (*refs) {
		ساختار object_id oid;

		اگر ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			ساختار reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} وگرنه اگر (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} وگرنه {
			بازگشت 1;
		}

		refs++;
	}

	بازگشت 0;
}

ساختار patterns_options {
	صحیح show_head;
	صحیح branches_only;
	صحیح tags_only;
};

ایستا صحیح cmd_show_ref__patterns(ثابت ساختار patterns_options *opts,
				  ثابت ساختار show_one_options *show_one_opts,
				  ثابت نویسه **patterns)
{
	ساختار show_ref_data show_ref_data = {
		.show_one_opts = show_one_opts,
		.show_head = opts->show_head,
	};

	اگر (patterns && *patterns)
		show_ref_data.patterns = patterns;

	اگر (opts->show_head)
		refs_head_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_data);
	اگر (opts->branches_only || opts->tags_only) {
		ساختار refs_for_each_ref_options for_each_ref_opts = { 0 };

		اگر (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}

		اگر (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}
	} وگرنه {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_data);
	}
	اگر (!show_ref_data.found_match)
		بازگشت 1;

	بازگشت 0;
}

ایستا صحیح cmd_show_ref__exists(ثابت نویسه **refs)
{
	ساختار strbuf unused_referent = STRBUF_INIT;
	ساختار object_id unused_oid;
	unsigned صحیح unused_type;
	صحیح failure_errno = 0;
	ثابت نویسه *ref;
	صحیح ret = 0;

	اگر (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	اگر (*refs)
		die("--exists requires exactly one reference");

	اگر (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		اگر (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} وگرنه {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		برو out;
	}

out:
	strbuf_release(&unused_referent);
	بازگشت ret;
}

ایستا صحیح hash_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار show_one_options *opts = opt->مقدار;
	ساختار option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	اگر (!arg)
		بازگشت 0;

	abbrev_opt.مقدار = &opts->abbrev;
	بازگشت parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

ایستا صحیح exclude_existing_callback(ثابت ساختار option *opt, ثابت نویسه *arg,
				     صحیح unset)
{
	ساختار exclude_existing_options *opts = opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	بازگشت 0;
}

صحیح cmd_show_ref(صحیح argc,
ثابت نویسه **argv,
ثابت نویسه *prefix,
ساختار repository *repo UNUSED)
{
	ساختار exclude_existing_options exclude_existing_opts = {0};
	ساختار patterns_options patterns_opts = {0};
	ساختار show_one_options show_one_opts = {0};
	صحیح verify = 0, exists = 0;
	ثابت ساختار option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.tags_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', پوچ, &patterns_opts.show_head,
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

	repo_config(the_repository, git_default_config, پوچ);

	argc = parse_options(argc, argv, prefix, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	اگر (exclude_existing_opts.enabled)
		بازگشت cmd_show_ref__exclude_existing(&exclude_existing_opts);
	وگرنه اگر (verify)
		بازگشت cmd_show_ref__verify(&show_one_opts, argv);
	وگرنه اگر (exists)
		بازگشت cmd_show_ref__exists(argv);
	وگرنه
		بازگشت cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
