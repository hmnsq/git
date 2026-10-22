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

static const char * const show_ref_usage[] = {
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

struct show_one_options {
	整数 quiet;
	整数 hash_only;
	整数 abbrev;
	整数 deref_tags;
};

static void show_one(const struct show_one_options *opts,
		     const struct reference *ref)
{
	const char *hex;
	struct object_id peeled;

	如果 (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	如果 (opts->quiet)
		返回;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	如果 (opts->hash_only)
		printf("%s\n", hex);
	否则
		printf("%s %s\n", hex, ref->name);

	如果 (!opts->deref_tags)
		返回;

	如果 (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		printf("%s %s^{}\n", hex, ref->name);
	}
}

struct show_ref_data {
	const struct show_one_options *show_one_opts;
	const char **patterns;
	整数 found_match;
	整数 show_head;
};

static 整数 show_ref(const struct reference *ref, void *cbdata)
{
	struct show_ref_data *data = cbdata;

	如果 (data->show_head && !strcmp(ref->name, "HEAD"))
		goto match;

	如果 (data->patterns) {
		整数 reflen = strlen(ref->name);
		const char **p = data->patterns, *m;
		当 ((m = *p++) != NULL) {
			整数 len = strlen(m);
			如果 (len > reflen)
				continue;
			如果 (memcmp(m, ref->name + reflen - len, len))
				continue;
			如果 (len == reflen)
				goto match;
			如果 (ref->name[reflen - len - 1] == '/')
				goto match;
		}
		返回 0;
	}

match:
	data->found_match++;

	show_one(data->show_one_opts, ref);

	返回 0;
}

static 整数 add_existing(const struct reference *ref, void *cbdata)
{
	struct string_list *list = (struct string_list *)cbdata;
	string_list_insert(list, ref->name);
	返回 0;
}

struct exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	整数 enabled;
	const char *pattern;
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
static 整数 cmd_show_ref__exclude_existing(const struct exclude_existing_options *opts)
{
	struct string_list existing_refs = STRING_LIST_INIT_DUP;
	char buf[1024];
	整数 patternlen = opts->pattern ? strlen(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	当 (fgets(buf, sizeof(buf), stdin)) {
		char *ref;
		整数 len = strlen(buf);

		如果 (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		如果 (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		为 (ref = buf + len; buf < ref; ref--)
			如果 (isspace(ref[-1]))
				break;
		如果 (opts->pattern) {
			整数 reflen = buf + len - ref;
			如果 (reflen < patternlen)
				continue;
			如果 (strncmp(ref, opts->pattern, patternlen))
				continue;
		}
		如果 (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			continue;
		}
		如果 (!string_list_has_string(&existing_refs, ref)) {
			printf("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	返回 0;
}

static 整数 cmd_show_ref__verify(const struct show_one_options *show_one_opts,
				const char **refs)
{
	如果 (!refs || !*refs)
		die("--verify requires a reference");

	当 (*refs) {
		struct object_id oid;

		如果 ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			struct reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} 否则 如果 (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} 否则 {
			返回 1;
		}

		refs++;
	}

	返回 0;
}

struct patterns_options {
	整数 show_head;
	整数 branches_only;
	整数 tags_only;
};

static 整数 cmd_show_ref__patterns(const struct patterns_options *opts,
				  const struct show_one_options *show_one_opts,
				  const char **patterns)
{
	struct show_ref_data show_ref_data = {
		.show_one_opts = show_one_opts,
		.show_head = opts->show_head,
	};

	如果 (patterns && *patterns)
		show_ref_data.patterns = patterns;

	如果 (opts->show_head)
		refs_head_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_data);
	如果 (opts->branches_only || opts->tags_only) {
		struct refs_for_each_ref_options for_each_ref_opts = { 0 };

		如果 (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}

		如果 (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}
	} 否则 {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_data);
	}
	如果 (!show_ref_data.found_match)
		返回 1;

	返回 0;
}

static 整数 cmd_show_ref__exists(const char **refs)
{
	struct strbuf unused_referent = STRBUF_INIT;
	struct object_id unused_oid;
	unsigned 整数 unused_type;
	整数 failure_errno = 0;
	const char *ref;
	整数 ret = 0;

	如果 (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	如果 (*refs)
		die("--exists requires exactly one reference");

	如果 (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		如果 (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} 否则 {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		goto out;
	}

out:
	strbuf_release(&unused_referent);
	返回 ret;
}

static 整数 hash_callback(const struct option *opt, const char *arg, 整数 unset)
{
	struct show_one_options *opts = opt->value;
	struct option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	如果 (!arg)
		返回 0;

	abbrev_opt.value = &opts->abbrev;
	返回 parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

static 整数 exclude_existing_callback(const struct option *opt, const char *arg,
				     整数 unset)
{
	struct exclude_existing_options *opts = opt->value;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	返回 0;
}

整数 cmd_show_ref(整数 argc,
const char **argv,
const char *prefix,
struct repository *repo UNUSED)
{
	struct exclude_existing_options exclude_existing_opts = {0};
	struct patterns_options patterns_opts = {0};
	struct show_one_options show_one_opts = {0};
	整数 verify = 0, exists = 0;
	const struct option show_ref_options[] = {
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

	如果 (exclude_existing_opts.enabled)
		返回 cmd_show_ref__exclude_existing(&exclude_existing_opts);
	否则 如果 (verify)
		返回 cmd_show_ref__verify(&show_one_opts, argv);
	否则 如果 (exists)
		返回 cmd_show_ref__exists(argv);
	否则
		返回 cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
