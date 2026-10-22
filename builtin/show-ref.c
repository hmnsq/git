#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "refs/refs-internal.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "object.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "parse-options.h"

સ્થિર અચલ અક્ષર * અચલ show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	ખાલી
};

સંરચના show_one_options {
	પૂર્ણાંક quiet;
	પૂર્ણાંક hash_only;
	પૂર્ણાંક abbrev;
	પૂર્ણાંક deref_tags;
};

સ્થિર શૂન્ય show_one(અચલ સંરચના show_one_options *opts,
		     અચલ સંરચના reference *ref)
{
	અચલ અક્ષર *hex;
	સંરચના object_id peeled;

	જો (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_to_hex(ref->oid));

	જો (opts->quiet)
		પાછા;

	hex = repo_find_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	જો (opts->hash_only)
		printf("%s\n", hex);
	નહીંતર
		printf("%s %s\n", hex, ref->name);

	જો (!opts->deref_tags)
		પાછા;

	જો (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_find_unique_abbrev(the_repository, &peeled, opts->abbrev);
		printf("%s %s^{}\n", hex, ref->name);
	}
}

સંરચના show_ref_data {
	અચલ સંરચના show_one_options *show_one_opts;
	અચલ અક્ષર **patterns;
	પૂર્ણાંક found_match;
	પૂર્ણાંક show_head;
};

સ્થિર પૂર્ણાંક show_ref(અચલ સંરચના reference *ref, શૂન્ય *cbdata)
{
	સંરચના show_ref_data *data = cbdata;

	જો (data->show_head && !strcmp(ref->name, "HEAD"))
		જાઓ match;

	જો (data->patterns) {
		પૂર્ણાંક reflen = strlen(ref->name);
		અચલ અક્ષર **p = data->patterns, *m;
		જ્યારે ((m = *p++) != ખાલી) {
			પૂર્ણાંક len = strlen(m);
			જો (len > reflen)
				ચાલુ;
			જો (memcmp(m, ref->name + reflen - len, len))
				ચાલુ;
			જો (len == reflen)
				જાઓ match;
			જો (ref->name[reflen - len - 1] == '/')
				જાઓ match;
		}
		પાછા 0;
	}

match:
	data->found_match++;

	show_one(data->show_one_opts, ref);

	પાછા 0;
}

સ્થિર પૂર્ણાંક add_existing(અચલ સંરચના reference *ref, શૂન્ય *cbdata)
{
	સંરચના string_list *list = (સંરચના string_list *)cbdata;
	string_list_ઉમેરો(list, ref->name);
	પાછા 0;
}

સંરચના exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	પૂર્ણાંક enabled;
	અચલ અક્ષર *pattern;
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
સ્થિર પૂર્ણાંક cmd_show_ref__exclude_existing(અચલ સંરચના exclude_existing_options *opts)
{
	સંરચના string_list existing_refs = STRING_LIST_INIT_DUP;
	અક્ષર buf[1024];
	પૂર્ણાંક patternlen = opts->pattern ? strlen(opts->pattern) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), add_existing,
			  &existing_refs);
	જ્યારે (fgets(buf, કદ(buf), stdin)) {
		અક્ષર *ref;
		પૂર્ણાંક len = strlen(buf);

		જો (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		જો (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		માટે (ref = buf + len; buf < ref; ref--)
			જો (isspace(ref[-1]))
				તોડો;
		જો (opts->pattern) {
			પૂર્ણાંક reflen = buf + len - ref;
			જો (reflen < patternlen)
				ચાલુ;
			જો (strncmp(ref, opts->pattern, patternlen))
				ચાલુ;
		}
		જો (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			ચાલુ;
		}
		જો (!string_list_has_string(&existing_refs, ref)) {
			printf("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	પાછા 0;
}

સ્થિર પૂર્ણાંક cmd_show_ref__verify(અચલ સંરચના show_one_options *show_one_opts,
				અચલ અક્ષર **refs)
{
	જો (!refs || !*refs)
		die("--verify requires a reference");

	જ્યારે (*refs) {
		સંરચના object_id oid;

		જો ((starts_with(*refs, "refs/") || refname_is_safe(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			સંરચના reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} નહીંતર જો (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} નહીંતર {
			પાછા 1;
		}

		refs++;
	}

	પાછા 0;
}

સંરચના patterns_options {
	પૂર્ણાંક show_head;
	પૂર્ણાંક branches_only;
	પૂર્ણાંક tags_only;
};

સ્થિર પૂર્ણાંક cmd_show_ref__patterns(અચલ સંરચના patterns_options *opts,
				  અચલ સંરચના show_one_options *show_one_opts,
				  અચલ અક્ષર **patterns)
{
	સંરચના show_ref_data show_ref_data = {
		.show_one_opts = show_one_opts,
		.show_head = opts->show_head,
	};

	જો (patterns && *patterns)
		show_ref_data.patterns = patterns;

	જો (opts->show_head)
		refs_head_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_data);
	જો (opts->branches_only || opts->tags_only) {
		સંરચના refs_for_each_ref_options for_each_ref_opts = { 0 };

		જો (opts->branches_only) {
			for_each_ref_opts.prefix = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}

		જો (opts->tags_only) {
			for_each_ref_opts.prefix = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_data, &for_each_ref_opts);
		}
	} નહીંતર {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_data);
	}
	જો (!show_ref_data.found_match)
		પાછા 1;

	પાછા 0;
}

સ્થિર પૂર્ણાંક cmd_show_ref__exists(અચલ અક્ષર **refs)
{
	સંરચના strbuf unused_referent = STRBUF_INIT;
	સંરચના object_id unused_oid;
	unsigned પૂર્ણાંક unused_type;
	પૂર્ણાંક failure_errno = 0;
	અચલ અક્ષર *ref;
	પૂર્ણાંક ret = 0;

	જો (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	જો (*refs)
		die("--exists requires exactly one reference");

	જો (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		જો (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} નહીંતર {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		જાઓ out;
	}

out:
	strbuf_release(&unused_referent);
	પાછા ret;
}

સ્થિર પૂર્ણાંક hash_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના show_one_options *opts = opt->કિંમત;
	સંરચના option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	જો (!arg)
		પાછા 0;

	abbrev_opt.કિંમત = &opts->abbrev;
	પાછા parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

સ્થિર પૂર્ણાંક exclude_existing_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
				     પૂર્ણાંક unset)
{
	સંરચના exclude_existing_options *opts = opt->કિંમત;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->pattern = arg;
	પાછા 0;
}

પૂર્ણાંક cmd_show_ref(પૂર્ણાંક argc,
અચલ અક્ષર **argv,
અચલ અક્ષર *prefix,
સંરચના repository *repo UNUSED)
{
	સંરચના exclude_existing_options exclude_existing_opts = {0};
	સંરચના patterns_options patterns_opts = {0};
	સંરચના show_one_options show_one_opts = {0};
	પૂર્ણાંક verify = 0, exists = 0;
	અચલ સંરચના option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.tags_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', ખાલી, &patterns_opts.show_head,
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

	repo_config(the_repository, git_default_config, ખાલી);

	argc = parse_options(argc, argv, prefix, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	જો (exclude_existing_opts.enabled)
		પાછા cmd_show_ref__exclude_existing(&exclude_existing_opts);
	નહીંતર જો (verify)
		પાછા cmd_show_ref__verify(&show_one_opts, argv);
	નહીંતર જો (exists)
		પાછા cmd_show_ref__exists(argv);
	નહીંતર
		પાછા cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
