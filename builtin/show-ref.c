#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "refs/refs-internal.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "parse-options.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి show_ref_usage[] = {
	N_("git show-ref [--head] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]] [--branches] [--tags]\n"
	   "             [--] [<pattern>...]"),
	N_("git show-ref --verify [-q | --quiet] [-d | --dereference]\n"
	   "             [-s | --hash[=<n>]] [--abbrev[=<n>]]\n"
	   "             [--] [<ref>...]"),
	N_("git show-ref --exclude-existing[=<pattern>]"),
	N_("git show-ref --exists <ref>"),
	ఖాళీ
};

నిర్మాణం show_one_options {
	పూర్ణాంకం quiet;
	పూర్ణాంకం hash_only;
	పూర్ణాంకం abbrev;
	పూర్ణాంకం deref_ట్యాగులు;
};

స్థిర శూన్య show_one(స్థిరరాశి నిర్మాణం show_one_options *opts,
		     స్థిరరాశి నిర్మాణం reference *ref)
{
	స్థిరరాశి అక్షరం *hex;
	నిర్మాణం object_id peeled;

	ఒకవేళ (!odb_has_object(the_repository->objects, ref->oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die("git show-ref: bad ref %s (%s)", ref->name,
		    oid_గమ్యం_hex(ref->oid));

	ఒకవేళ (opts->quiet)
		తిరిగి;

	hex = repo_కనుగొను_unique_abbrev(the_repository, ref->oid, opts->abbrev);
	ఒకవేళ (opts->hash_only)
		రాయి("%s\n", hex);
	లేకపోతే
		రాయి("%s %s\n", hex, ref->name);

	ఒకవేళ (!opts->deref_ట్యాగులు)
		తిరిగి;

	ఒకవేళ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		hex = repo_కనుగొను_unique_abbrev(the_repository, &peeled, opts->abbrev);
		రాయి("%s %s^{}\n", hex, ref->name);
	}
}

నిర్మాణం show_ref_దత్తాంశం {
	స్థిరరాశి నిర్మాణం show_one_options *show_one_opts;
	స్థిరరాశి అక్షరం **patterns;
	పూర్ణాంకం దొరికింది_సరిపోలిక;
	పూర్ణాంకం show_తల;
};

స్థిర పూర్ణాంకం show_ref(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cbdata)
{
	నిర్మాణం show_ref_దత్తాంశం *దత్తాంశం = cbdata;

	ఒకవేళ (దత్తాంశం->show_తల && !strcmp(ref->name, "HEAD"))
		వెళ్ళు సరిపోలిక;

	ఒకవేళ (దత్తాంశం->patterns) {
		పూర్ణాంకం reflen = అక్షరశ్రేణి_పొడవు(ref->name);
		స్థిరరాశి అక్షరం **p = దత్తాంశం->patterns, *m;
		అయితే ((m = *p++) != ఖాళీ) {
			పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(m);
			ఒకవేళ (len > reflen)
				కొనసాగు;
			ఒకవేళ (memcmp(m, ref->name + reflen - len, len))
				కొనసాగు;
			ఒకవేళ (len == reflen)
				వెళ్ళు సరిపోలిక;
			ఒకవేళ (ref->name[reflen - len - 1] == '/')
				వెళ్ళు సరిపోలిక;
		}
		తిరిగి 0;
	}

సరిపోలిక:
	దత్తాంశం->దొరికింది_సరిపోలిక++;

	show_one(దత్తాంశం->show_one_opts, ref);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం కూడు_existing(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cbdata)
{
	నిర్మాణం string_list *list = (నిర్మాణం string_list *)cbdata;
	string_list_చొప్పించు(list, ref->name);
	తిరిగి 0;
}

నిర్మాణం exclude_existing_options {
	/*
	 * We need an explicit `enabled` field because it is perfectly valid
	 * for `pattern` to be `NULL` even if `--exclude-existing` was given.
	 */
	పూర్ణాంకం enabled;
	స్థిరరాశి అక్షరం *నమూనా;
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
స్థిర పూర్ణాంకం cmd_show_ref__exclude_existing(స్థిరరాశి నిర్మాణం exclude_existing_options *opts)
{
	నిర్మాణం string_list existing_refs = STRING_LIST_INIT_DUP;
	అక్షరం buf[1024];
	పూర్ణాంకం patternlen = opts->నమూనా ? అక్షరశ్రేణి_పొడవు(opts->నమూనా) : 0;

	refs_for_each_ref(get_main_ref_store(the_repository), కూడు_existing,
			  &existing_refs);
	అయితే (fgets(buf, పరిమాణం(buf), stdin)) {
		అక్షరం *ref;
		పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(buf);

		ఒకవేళ (len > 0 && buf[len - 1] == '\n')
			buf[--len] = '\0';
		ఒకవేళ (3 <= len && !strcmp(buf + len - 3, "^{}")) {
			len -= 3;
			buf[len] = '\0';
		}
		కొరకు (ref = buf + len; buf < ref; ref--)
			ఒకవేళ (ఖాళీనా(ref[-1]))
				ఆపు;
		ఒకవేళ (opts->నమూనా) {
			పూర్ణాంకం reflen = buf + len - ref;
			ఒకవేళ (reflen < patternlen)
				కొనసాగు;
			ఒకవేళ (అక్షరశ్రేణి_పోలిక(ref, opts->నమూనా, patternlen))
				కొనసాగు;
		}
		ఒకవేళ (check_refname_format(ref, 0)) {
			warning("ref '%s' ignored", ref);
			కొనసాగు;
		}
		ఒకవేళ (!string_list_has_string(&existing_refs, ref)) {
			రాయి("%s\n", buf);
		}
	}

	string_list_clear(&existing_refs, 0);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం cmd_show_ref__verify(స్థిరరాశి నిర్మాణం show_one_options *show_one_opts,
				స్థిరరాశి అక్షరం **refs)
{
	ఒకవేళ (!refs || !*refs)
		die("--verify requires a reference");

	అయితే (*refs) {
		నిర్మాణం object_id oid;

		ఒకవేళ ((starts_with(*refs, "refs/") || refname_is_సురక్షిత(*refs)) &&
		    !refs_read_ref(get_main_ref_store(the_repository), *refs, &oid)) {
			నిర్మాణం reference ref = {
				.name = *refs,
				.oid = &oid,
			};

			show_one(show_one_opts, &ref);
		} లేకపోతే ఒకవేళ (!show_one_opts->quiet) {
			die("'%s' - not a valid ref", *refs);
		} లేకపోతే {
			తిరిగి 1;
		}

		refs++;
	}

	తిరిగి 0;
}

నిర్మాణం patterns_options {
	పూర్ణాంకం show_తల;
	పూర్ణాంకం branches_only;
	పూర్ణాంకం ట్యాగులు_only;
};

స్థిర పూర్ణాంకం cmd_show_ref__patterns(స్థిరరాశి నిర్మాణం patterns_options *opts,
				  స్థిరరాశి నిర్మాణం show_one_options *show_one_opts,
				  స్థిరరాశి అక్షరం **patterns)
{
	నిర్మాణం show_ref_దత్తాంశం show_ref_దత్తాంశం = {
		.show_one_opts = show_one_opts,
		.show_తల = opts->show_తల,
	};

	ఒకవేళ (patterns && *patterns)
		show_ref_దత్తాంశం.patterns = patterns;

	ఒకవేళ (opts->show_తల)
		refs_తల_ref(get_main_ref_store(the_repository), show_ref,
			      &show_ref_దత్తాంశం);
	ఒకవేళ (opts->branches_only || opts->ట్యాగులు_only) {
		నిర్మాణం refs_for_each_ref_options for_each_ref_opts = { 0 };

		ఒకవేళ (opts->branches_only) {
			for_each_ref_opts.ఉపసర్గ = "refs/heads/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_దత్తాంశం, &for_each_ref_opts);
		}

		ఒకవేళ (opts->ట్యాగులు_only) {
			for_each_ref_opts.ఉపసర్గ = "refs/tags/";
			refs_for_each_ref_ext(get_main_ref_store(the_repository),
					      show_ref, &show_ref_దత్తాంశం, &for_each_ref_opts);
		}
	} లేకపోతే {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  show_ref, &show_ref_దత్తాంశం);
	}
	ఒకవేళ (!show_ref_దత్తాంశం.దొరికింది_సరిపోలిక)
		తిరిగి 1;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం cmd_show_ref__exists(స్థిరరాశి అక్షరం **refs)
{
	నిర్మాణం strbuf వాడని_referent = STRBUF_INIT;
	నిర్మాణం object_id వాడని_oid;
	గుర్తులేని పూర్ణాంకం వాడని_type;
	పూర్ణాంకం failure_errno = 0;
	స్థిరరాశి అక్షరం *ref;
	పూర్ణాంకం ret = 0;

	ఒకవేళ (!refs || !*refs)
		die("--exists requires a reference");
	ref = *refs++;
	ఒకవేళ (*refs)
		die("--exists requires exactly one reference");

	ఒకవేళ (refs_read_raw_ref(get_main_ref_store(the_repository), ref,
			      &వాడని_oid, &వాడని_referent, &వాడని_type,
			      &failure_errno)) {
		ఒకవేళ (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} లేకపోతే {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		వెళ్ళు బహిర్గతం;
	}

బహిర్గతం:
	strbuf_release(&వాడని_referent);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం hash_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం show_one_options *opts = opt->విలువ;
	నిర్మాణం option abbrev_opt = *opt;

	opts->hash_only = 1;
	/* Use full length SHA1 if no argument */
	ఒకవేళ (!arg)
		తిరిగి 0;

	abbrev_opt.విలువ = &opts->abbrev;
	తిరిగి parse_opt_abbrev_cb(&abbrev_opt, arg, unset);
}

స్థిర పూర్ణాంకం exclude_existing_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
				     పూర్ణాంకం unset)
{
	నిర్మాణం exclude_existing_options *opts = opt->విలువ;
	BUG_ON_OPT_NEG(unset);
	opts->enabled = 1;
	opts->నమూనా = arg;
	తిరిగి 0;
}

పూర్ణాంకం cmd_show_ref(పూర్ణాంకం argc,
స్థిరరాశి అక్షరం **argv,
స్థిరరాశి అక్షరం *ఉపసర్గ,
నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం exclude_existing_options exclude_existing_opts = {0};
	నిర్మాణం patterns_options patterns_opts = {0};
	నిర్మాణం show_one_options show_one_opts = {0};
	పూర్ణాంకం verify = 0, exists = 0;
	స్థిరరాశి నిర్మాణం option show_ref_options[] = {
		OPT_BOOL(0, "tags", &patterns_opts.ట్యాగులు_only, N_("only show tags (can be combined with --branches)")),
		OPT_BOOL(0, "branches", &patterns_opts.branches_only, N_("only show branches (can be combined with --tags)")),
		OPT_HIDDEN_BOOL(0, "heads", &patterns_opts.branches_only,
				N_("deprecated synonym for --branches")),
		OPT_BOOL(0, "exists", &exists, N_("check for reference existence without resolving")),
		OPT_BOOL(0, "verify", &verify, N_("stricter reference checking, "
			    "requires exact ref path")),
		OPT_HIDDEN_BOOL('h', ఖాళీ, &patterns_opts.show_తల,
				N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL(0, "head", &patterns_opts.show_తల,
		  N_("show the HEAD reference, even if it would be filtered out")),
		OPT_BOOL('d', "dereference", &show_one_opts.deref_ట్యాగులు,
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
		OPT_చివర()
	};

	repo_config(the_repository, git_default_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, show_ref_options,
			     show_ref_usage, 0);

	die_for_incompatible_opt3(exclude_existing_opts.enabled, "--exclude-existing",
				  verify, "--verify",
				  exists, "--exists");

	ఒకవేళ (exclude_existing_opts.enabled)
		తిరిగి cmd_show_ref__exclude_existing(&exclude_existing_opts);
	లేకపోతే ఒకవేళ (verify)
		తిరిగి cmd_show_ref__verify(&show_one_opts, argv);
	లేకపోతే ఒకవేళ (exists)
		తిరిగి cmd_show_ref__exists(argv);
	లేకపోతే
		తిరిగి cmd_show_ref__patterns(&patterns_opts, &show_one_opts, argv);
}
