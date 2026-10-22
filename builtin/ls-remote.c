#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "transport.h"
#అన్తర్భావ "pkt-line.h"
#అన్తర్భావ "ref-filter.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "wildmatch.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి ls_remote_usage[] = {
	N_("git ls-remote [--branches] [--tags] [--refs] [--upload-pack=<exec>]\n"
	   "              [-q | --quiet] [--exit-code] [--get-url] [--sort=<key>]\n"
	   "              [--symref] [<repository> [<patterns>...]]"),
	ఖాళీ
};

/*
 * Is there one among the list of patterns that match the tail part
 * of the path?
 */
స్థిర పూర్ణాంకం తోక_సరిపోలిక(స్థిరరాశి నిర్మాణం strvec *నమూనా, స్థిరరాశి అక్షరం *మార్గం)
{
	అక్షరం *pathbuf;

	ఒకవేళ (!నమూనా->nr)
		తిరిగి 1; /* no restriction */

	pathbuf = xstrfmt("/%s", మార్గం);
	కొరకు (కొలత_t i = 0; i < నమూనా->nr; i++) {
		ఒకవేళ (!wildmatch(నమూనా->v[i], pathbuf, 0)) {
			విడుదల(pathbuf);
			తిరిగి 1;
		}
	}
	విడుదల(pathbuf);
	తిరిగి 0;
}

పూర్ణాంకం cmd_ls_remote(పూర్ణాంకం argc,
		  స్థిరరాశి అక్షరం **argv,
		  స్థిరరాశి అక్షరం *ఉపసర్గ,
		  నిర్మాణం repository *repo వాడని)
{
	స్థిరరాశి అక్షరం *dest = ఖాళీ;
	గుర్తులేని flags = 0;
	పూర్ణాంకం get_url = 0;
	పూర్ణాంకం quiet = 0;
	పూర్ణాంకం స్థితి_2 = 0;
	పూర్ణాంకం show_symref_లక్ష్యం = 0;
	స్థిరరాశి అక్షరం *uploadpack = ఖాళీ;
	నిర్మాణం strvec నమూనా = STRVEC_INIT;
	నిర్మాణం transport_ls_refs_options transport_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	పూర్ణాంకం i;
	నిర్మాణం string_list server_options = STRING_LIST_INIT_DUP;

	నిర్మాణం remote *remote;
	నిర్మాణం transport *transport;
	స్థిరరాశి నిర్మాణం ref *ref;
	నిర్మాణం ref_array ref_array;
	నిర్మాణం ref_క్రమీకరణ *క్రమీకరణ;
	నిర్మాణం string_list క్రమీకరణ_options = STRING_LIST_INIT_DUP;

	నిర్మాణం option options[] = {
		OPT__QUIET(&quiet, N_("do not print remote URL")),
		OPT_STRING(0, "upload-pack", &uploadpack, N_("exec"),
			   N_("path of git-upload-pack on the remote host")),
		{
			.type = OPTION_STRING,
			.long_name = "exec",
			.విలువ = &uploadpack,
			.argh = N_("exec"),
			.help = N_("path of git-upload-pack on the remote host"),
			.flags = PARSE_OPT_HIDDEN,
		},
		OPT_బిట్టు('t', "tags", &flags, N_("limit to tags"), REF_ట్యాగులు),
		OPT_బిట్టు('b', "branches", &flags, N_("limit to branches"), REF_BRANCHES),
		OPT_బిట్టు_F('h', "heads", &flags,
			  N_("deprecated synonym for --branches"), REF_BRANCHES,
			  PARSE_OPT_HIDDEN),
		OPT_బిట్టు(0, "refs", &flags, N_("do not show peeled tags"), REF_సాధారణ),
		OPT_BOOL(0, "get-url", &get_url,
			 N_("take url.<base>.insteadOf into account")),
		OPT_REF_క్రమీకరించు(&క్రమీకరణ_options),
		OPT_సమితి_INT_F(0, "exit-code", &స్థితి_2,
			      N_("exit with exit code 2 if no matching refs are found"),
			      2, PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "symref", &show_symref_లక్ష్యం,
			 N_("show underlying ref in addition to the object pointed by it")),
		OPT_STRING_LIST('o', "server-option", &server_options, N_("server-specific"), N_("option to transmit")),
		OPT_చివర()
	};

	memset(&ref_array, 0, పరిమాణం(ref_array));

	argc = parse_options(argc, argv, ఉపసర్గ, options, ls_remote_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	dest = argv[0];

	/*
	 * TODO: This is buggy, but required for transport helpers. When a
	 * transport helper advertises a "refspec", then we'd add that to a
	 * list of refspecs via `refspec_append()`, which transitively depends
	 * on `the_hash_algo`. Thus, when the hash algorithm isn't properly set
	 * up, this would lead to a segfault.
	 *
	 * We really should fix this in the transport helper logic such that we
	 * lazily parse refspec capabilities _after_ we have learned about the
	 * remote's object format. Otherwise, we may end up misparsing refspecs
	 * depending on what object hash the remote uses.
	 */
	ఒకవేళ (!the_repository->hash_algo)
		repo_సమితి_hash_algo(the_repository, GIT_HASH_DEFAULT);

	packet_trace_తటస్థం("ls-remote");

	కొరకు (పూర్ణాంకం i = 1; i < argc; i++)
		strvec_pushf(&నమూనా, "*/%s", argv[i]);

	ఒకవేళ (flags & REF_ట్యాగులు)
		strvec_నెట్టు(&transport_options.ref_prefixes, "refs/tags/");
	ఒకవేళ (flags & REF_BRANCHES)
		strvec_నెట్టు(&transport_options.ref_prefixes, "refs/heads/");

	remote = remote_get(dest);
	ఒకవేళ (!remote) {
		ఒకవేళ (dest)
			die("bad repository '%s'", dest);
		die("No remote configured to list refs from.");
	}

	ఒకవేళ (get_url) {
		రాయి("%s\n", remote->url.v[0]);
		తిరిగి 0;
	}

	transport = transport_get(remote, ఖాళీ);
	ఒకవేళ (uploadpack)
		transport_సమితి_option(transport, TRANS_OPT_UPLOADPACK, uploadpack);
	ఒకవేళ (server_options.nr)
		transport->server_options = &server_options;

	ref = transport_get_remote_refs(transport, &transport_options);
	ఒకవేళ (ref) {
		పూర్ణాంకం hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
		repo_సమితి_hash_algo(the_repository, hash_algo);
	}

	ఒకవేళ (!dest && !quiet)
		fprintf(stderr, "From %s\n", remote->url.v[0]);
	కొరకు ( ; ref; ref = ref->తదుపరి) {
		నిర్మాణం ref_array_అంశం *అంశం;
		ఒకవేళ (!check_ref_type(ref, flags))
			కొనసాగు;
		ఒకవేళ (!తోక_సరిపోలిక(&నమూనా, ref->name))
			కొనసాగు;
		అంశం = ref_array_నెట్టు(&ref_array, ref->name, &ref->old_oid, ఖాళీ);
		అంశం->symref = xstrdup_or_null(ref->symref);
	}

	క్రమీకరణ = ref_క్రమీకరణ_options(&క్రమీకరణ_options);
	ref_array_క్రమీకరించు(క్రమీకరణ, &ref_array);

	కొరకు (i = 0; i < ref_array.nr; i++) {
		స్థిరరాశి నిర్మాణం ref_array_అంశం *ref = ref_array.వస్తువులు[i];
		ఒకవేళ (show_symref_లక్ష్యం && ref->symref)
			రాయి("ref: %s\t%s\n", ref->symref, ref->refname);
		రాయి("%s\t%s\n", oid_గమ్యం_hex(&ref->objectname), ref->refname);
		స్థితి_2 = 0; /* we found something */
	}

	string_list_clear(&server_options, 0);
	ref_క్రమీకరణ_release(క్రమీకరణ);
	ref_array_clear(&ref_array);
	ఒకవేళ (transport_disconnect(transport))
		స్థితి_2 = 1;
	transport_ls_refs_options_release(&transport_options);

	strvec_clear(&నమూనా);
	string_list_clear(&server_options, 0);
	తిరిగి స్థితి_2;
}
