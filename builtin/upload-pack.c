#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "exec-cmd.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "pkt-line.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "protocol.h"
#అన్తర్భావ "replace-object.h"
#అన్తర్భావ "upload-pack.h"
#అన్తర్భావ "serve.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "environment.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	ఖాళీ
};

పూర్ణాంకం cmd_upload_pack(పూర్ణాంకం argc,
		    స్థిరరాశి అక్షరం **argv,
		    స్థిరరాశి అక్షరం *ఉపసర్గ,
		    నిర్మాణం repository *repo వాడని)
{
	స్థిరరాశి అక్షరం *dir;
	పూర్ణాంకం strict = 0;
	పూర్ణాంకం advertise_refs = 0;
	పూర్ణాంకం stateless_rpc = 0;
	పూర్ణాంకం timeout = 0;
	నిర్మాణం option options[] = {
		OPT_BOOL(0, "stateless-rpc", &stateless_rpc,
			 N_("quit after a single request/response exchange")),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs,
				N_("serve up the info/refs for git-http-backend")),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_BOOL(0, "strict", &strict,
			 N_("do not try <directory>/.git/ if <directory> is no Git directory")),
		OPT_INTEGER(0, "timeout", &timeout,
			    N_("interrupt transfer after <n> seconds of inactivity")),
		OPT_చివర()
	};
	గుర్తులేని enter_repo_flags = ENTER_REPO_ANY_OWNER_OK;

	packet_trace_తటస్థం("upload-pack");
	disable_replace_refs();
	save_commit_బఫరు = 0;
	xsetenv(NO_LAZY_FETCH_ENVIRONMENT, "1", 0);

	argc = parse_options(argc, argv, ఉపసర్గ, options, upload_pack_usage, 0);

	ఒకవేళ (argc != 1)
		usage_with_options(upload_pack_usage, options);

	అమరిక_మార్గం();

	dir = argv[0];

	ఒకవేళ (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	ఒకవేళ (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	ఎంపిక (determine_protocol_సంస్కరణ_server()) {
	స్థితి protocol_v2:
		ఒకవేళ (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		లేకపోతే
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		ఆపు;
	స్థితి protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ఒకవేళ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	స్థితి protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		ఆపు;
	స్థితి protocol_unknown_సంస్కరణ:
		BUG("unknown protocol version");
	}

	తిరిగి 0;
}
