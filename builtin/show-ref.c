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
	จำนวนเต็ม quiet;
	จำนวนเต็ม hash_only;
	จำนวนเต็ม abbrev;
	จำนวนเต็ม deref_tags;
};

static void show_one(const sa_nracanaa show_one_options *opts,
		     const sa_nracanaa reference *ref)
{
	const var_na *hex;
	sa_nracanaa object_id peeled;

	ถ้า (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	ถ้า (opts->quiet)
		คืน;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	ถ้า (opts->hash_only)
		printf("%s\n", hex);
	มิฉะนั้น
		printf("%s %s\n", hex, ref->name);

	ถ้า (!opts->deref_tags)
		คืน;

	ถ้า (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		printf("%s %s^{}\n", hex, ref->name);
	}
}

sa_nracanaa show_ref_data {
	const sa_nracanaa show_one_options *show_one_opts;
	const var_na **patterns;
	จำนวนเต็ม found_match;
	จำนวนเต็ม show_head;
};

static จำนวนเต็ม show_ref(const sa_nracanaa reference *ref, void *cbdata)
{
	sa_nracanaa show_ref_data *data = cbdata;

	ถ้า (data->show_head && !strcmp(ref->name, "HEAD"))
		goto match;

	ถ้า (data->patterns) {
		จำนวนเต็ม reflen = strlen(ref->name);
		const var_na **p = data->patterns, *m;
		ขณะ ((m = *p++) != NULL) {
			จำนวนเต็ม len = strlen(m);
			ถ้า (len > reflen)
				continue;
			ถ้า (memcmp(m, ref->name + reflen - len, len))
				continue;
			ถ้า (len == reflen)
				goto match;
			ถ้า (ref->name[reflen - len - 1] == '/')
				goto match;
		}
		คืน 0;
	}

match:
	data->found_match++;

	show_one(data->show_one_opts, ref);

	คืน 0;
}

static จำนวนเต็ม add_existing(const sa_nracanaa reference *ref, void *cbdata)
{
	sa_nracanaa string_list *list = (sa_nracanaa string_list *)cbdata;
	string_list_insert(list, ref->name);
	คืน 0;
}

sa_nracanaa exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	จำนวนเต็ม enabled;
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
static จำนวนเต็ม cmd_show_ref__exclude_existing(const sa_nracanaa exclude_existing_options *opts)
{
	sa_nracanaa string_list existing_refs = STRING_LIST_INIT_DUP;
	var_na buf[1024];
	จำนวนเต็ม patternlen = opts->pattern ? strlen(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	ขณะ (fgets(buf, parimaa_na(buf), stdin)) {
		var_na *ref;
		จำนวนเต็ม len = strlen(buf);

		ถ้า (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		ถ้า (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		สำหรับ (ref = buf + len; buf < ref; ref--)
			ถ้า (isspace(ref[-1]))
				bha_nga;
		ถ้า (opts->pattern) {
			จำนวนเต็ม reflen = buf + len - ref;
			ถ้า (reflen < patternlen)
				continue;
			ถ้า (strncmp(ref, opts->pattern, patternlen))
				continue;
		}
		ถ้า (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			continue;
		}
		ถ้า (!string_list_has_string(&existing_refs, ref)) {
			printf("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	คืน 0;
}

static จำนวนเต็ม cmd_show_ref__verify(const sa_nracanaa show_one_options *show_one_opts,
				const var_na **refs)
{
	ถ้า (!refs || !*refs)
		die("--verify requires a reference");

	ขณะ (*refs) {
		sa_nracanaa object_id oid;

		ถ้า ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			sa_nracanaa reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} มิฉะนั้น ถ้า (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} มิฉะนั้น {
			คืน 1;
		}

		refs++;
	}

	คืน 0;
}

sa_nracanaa patterns_options {
	จำนวนเต็ม show_head;
	จำนวนเต็ม branches_only;
	จำนวนเต็ม tags_only;
};

static จำนวนเต็ม cmd_show_ref__patterns(const sa_nracanaa patterns_options *opts,
				  const sa_nracanaa show_one_options *show_one_opts,
				  const var_na **patterns)
{
	sa_nracanaa show_ref_data show_ref_data = {
		.show_one_opts = show_one_opts,
		.show_head = opts->show_head,
	};

	ถ้า (patterns && *patterns)
		show_ref_data.patterns = patterns;

	ถ้า (opts->show_head)
		refs_head_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_data);
	ถ้า (opts->branches_only || opts->tags_only) {
		sa_nracanaa refs_for_each_ref_options for_each_ref_opts = { 0 };

		ถ้า (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}

		ถ้า (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}
	} มิฉะนั้น {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_data);
	}
	ถ้า (!show_ref_data.found_match)
		คืน 1;

	คืน 0;
}

static จำนวนเต็ม cmd_show_ref__exists(const var_na **refs)
{
	sa_nracanaa strbuf unused_referent = STRBUF_INIT;
	sa_nracanaa object_id unused_oid;
	unsigned จำนวนเต็ม unused_type;
	จำนวนเต็ม failure_errno = 0;
	const var_na *ref;
	จำนวนเต็ม ret = 0;

	ถ้า (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	ถ้า (*refs)
		die("--exists requires exactly one reference");

	ถ้า (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		ถ้า (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} มิฉะนั้น {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		goto out;
	}

out:
	strbuf_release(&unused_referent);
	คืน ret;
}

static จำนวนเต็ม hash_callback(const sa_nracanaa option *opt, const var_na *arg, จำนวนเต็ม unset)
{
	sa_nracanaa show_one_options *opts = opt->value;
	sa_nracanaa option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	ถ้า (!arg)
		คืน 0;

	abbrev_opt.value = &opts->abbrev;
	คืน parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

static จำนวนเต็ม exclude_existing_callback(const sa_nracanaa option *opt, const var_na *arg,
				     จำนวนเต็ม unset)
{
	sa_nracanaa exclude_existing_options *opts = opt->value;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	คืน 0;
}

จำนวนเต็ม cmd_show_ref(จำนวนเต็ม argc,
const var_na **argv,
const var_na *prefix,
sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa exclude_existing_options exclude_existing_opts = {0};
	sa_nracanaa patterns_options patterns_opts = {0};
	sa_nracanaa show_one_options show_one_opts = {0};
	จำนวนเต็ม verify = 0, exists = 0;
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

	ถ้า (exclude_existing_opts.enabled)
		คืน cmd_show_ref__exclude_existing(&exclude_existing_opts);
	มิฉะนั้น ถ้า (verify)
		คืน cmd_show_ref__verify(&show_one_opts, argv);
	มิฉะนั้น ถ้า (exists)
		คืน cmd_show_ref__exists(argv);
	มิฉะนั้น
		คืน cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
