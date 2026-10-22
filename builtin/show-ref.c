#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "refs/refs-internal.h"
#include "object-name.h"
#include "odb.h"
#include "object.h"
#include "string-list.h"
#include "parse-options.h"

static const var_na * const show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	NULL
};

sa_nracanaa show_one_options {
	පූර්ණ_සංඛ්‍යා quiet;
	පූර්ණ_සංඛ්‍යා hash_only;
	පූර්ණ_සංඛ්‍යා abbrev;
	පූර්ණ_සංඛ්‍යා deref_tags;
};

static void show_one(const sa_nracanaa show_one_options *opts,
		     const sa_nracanaa reference *ref)
{
	const var_na *hex;
	sa_nracanaa object_id peeled;

	නම් (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	නම් (opts->quiet)
		ආපසු;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	නම් (opts->hash_only)
		printf("%s\n", hex);
	නැතහොත්
		printf("%s %s\n", hex, ref->name);

	නම් (!opts->deref_tags)
		ආපසු;

	නම් (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		printf("%s %s^{}\n", hex, ref->name);
	}
}

sa_nracanaa show_ref_data {
	const sa_nracanaa show_one_options *show_one_opts;
	const var_na **patterns;
	පූර්ණ_සංඛ්‍යා found_match;
	පූර්ණ_සංඛ්‍යා show_head;
};

static පූර්ණ_සංඛ්‍යා show_ref(const sa_nracanaa reference *ref, void *cbdata)
{
	sa_nracanaa show_ref_data *data = cbdata;

	නම් (data->show_head && !strcmp(ref->name, "HEAD"))
		goto match;

	නම් (data->patterns) {
		පූර්ණ_සංඛ්‍යා reflen = strlen(ref->name);
		const var_na **p = data->patterns, *m;
		අතරතුර ((m = *p++) != NULL) {
			පූර්ණ_සංඛ්‍යා len = strlen(m);
			නම් (len > reflen)
				continue;
			නම් (memcmp(m, ref->name + reflen - len, len))
				continue;
			නම් (len == reflen)
				goto match;
			නම් (ref->name[reflen - len - 1] == '/')
				goto match;
		}
		ආපසු 0;
	}

match:
	data->found_match++;

	show_one(data->show_one_opts, ref);

	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා add_existing(const sa_nracanaa reference *ref, void *cbdata)
{
	sa_nracanaa string_list *list = (sa_nracanaa string_list *)cbdata;
	string_list_insert(list, ref->name);
	ආපසු 0;
}

sa_nracanaa exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	පූර්ණ_සංඛ්‍යා enabled;
	const var_na *pattern;
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
static පූර්ණ_සංඛ්‍යා cmd_show_ref__exclude_existing(const sa_nracanaa exclude_existing_options *opts)
{
	sa_nracanaa string_list existing_refs = STRING_LIST_INIT_DUP;
	var_na buf[1024];
	පූර්ණ_සංඛ්‍යා patternlen = opts->pattern ? strlen(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	අතරතුර (fgets(buf, parimaa_na(buf), stdin)) {
		var_na *ref;
		පූර්ණ_සංඛ්‍යා len = strlen(buf);

		නම් (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		නම් (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		සඳහා (ref = buf + len; buf < ref; ref--)
			නම් (isspace(ref[-1]))
				bha_nga;
		නම් (opts->pattern) {
			පූර්ණ_සංඛ්‍යා reflen = buf + len - ref;
			නම් (reflen < patternlen)
				continue;
			නම් (strncmp(ref, opts->pattern, patternlen))
				continue;
		}
		නම් (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			continue;
		}
		නම් (!string_list_has_string(&existing_refs, ref)) {
			printf("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා cmd_show_ref__verify(const sa_nracanaa show_one_options *show_one_opts,
				const var_na **refs)
{
	නම් (!refs || !*refs)
		die("--verify requires a reference");

	අතරතුර (*refs) {
		sa_nracanaa object_id oid;

		නම් ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			sa_nracanaa reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} නැතහොත් නම් (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} නැතහොත් {
			ආපසු 1;
		}

		refs++;
	}

	ආපසු 0;
}

sa_nracanaa patterns_options {
	පූර්ණ_සංඛ්‍යා show_head;
	පූර්ණ_සංඛ්‍යා branches_only;
	පූර්ණ_සංඛ්‍යා tags_only;
};

static පූර්ණ_සංඛ්‍යා cmd_show_ref__patterns(const sa_nracanaa patterns_options *opts,
				  const sa_nracanaa show_one_options *show_one_opts,
				  const var_na **patterns)
{
	sa_nracanaa show_ref_data show_ref_data = {
		.show_one_opts = show_one_opts,
		.show_head = opts->show_head,
	};

	නම් (patterns && *patterns)
		show_ref_data.patterns = patterns;

	නම් (opts->show_head)
		refs_head_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_data);
	නම් (opts->branches_only || opts->tags_only) {
		sa_nracanaa refs_for_each_ref_options for_each_ref_opts = { 0 };

		නම් (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}

		නම් (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}
	} නැතහොත් {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_data);
	}
	නම් (!show_ref_data.found_match)
		ආපසු 1;

	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා cmd_show_ref__exists(const var_na **refs)
{
	sa_nracanaa strbuf unused_referent = STRBUF_INIT;
	sa_nracanaa object_id unused_oid;
	unsigned පූර්ණ_සංඛ්‍යා unused_type;
	පූර්ණ_සංඛ්‍යා failure_errno = 0;
	const var_na *ref;
	පූර්ණ_සංඛ්‍යා ret = 0;

	නම් (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	නම් (*refs)
		die("--exists requires exactly one reference");

	නම් (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		නම් (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} නැතහොත් {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		goto out;
	}

out:
	strbuf_release(&unused_referent);
	ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා hash_callback(const sa_nracanaa option *opt, const var_na *arg, පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa show_one_options *opts = opt->value;
	sa_nracanaa option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	නම් (!arg)
		ආපසු 0;

	abbrev_opt.value = &opts->abbrev;
	ආපසු parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

static පූර්ණ_සංඛ්‍යා exclude_existing_callback(const sa_nracanaa option *opt, const var_na *arg,
				     පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa exclude_existing_options *opts = opt->value;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	ආපසු 0;
}

පූර්ණ_සංඛ්‍යා cmd_show_ref(පූර්ණ_සංඛ්‍යා argc,
const var_na **argv,
const var_na *prefix,
sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa exclude_existing_options exclude_existing_opts = {0};
	sa_nracanaa patterns_options patterns_opts = {0};
	sa_nracanaa show_one_options show_one_opts = {0};
	පූර්ණ_සංඛ්‍යා verify = 0, exists = 0;
	const sa_nracanaa option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.tags_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', NULL, &patterns_opts.show_head,
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

	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	නම් (exclude_existing_opts.enabled)
		ආපසු cmd_show_ref__exclude_existing(&exclude_existing_opts);
	නැතහොත් නම් (verify)
		ආපසු cmd_show_ref__verify(&show_one_opts, argv);
	නැතහොත් නම් (exists)
		ආපසු cmd_show_ref__exists(argv);
	නැතහොත්
		ආපසු cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
