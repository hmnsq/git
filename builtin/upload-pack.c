#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "exec-cmd.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "pkt-line.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "protocol.h"
#ಅನ್ತರ್ಭಾವ "replace-object.h"
#ಅನ್ತರ್ಭಾವ "upload-pack.h"
#ಅನ್ತರ್ಭಾವ "serve.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "environment.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	ಖಾಲಿ
};

ಪೂರ್ಣಾಂಕ cmd_upload_pack(ಪೂರ್ಣಾಂಕ argc,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		    ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dir;
	ಪೂರ್ಣಾಂಕ strict = 0;
	ಪೂರ್ಣಾಂಕ advertise_refs = 0;
	ಪೂರ್ಣಾಂಕ stateless_rpc = 0;
	ಪೂರ್ಣಾಂಕ timeout = 0;
	ರಚನೆ option options[] = {
		OPT_BOOL(0, "stateless-rpc", &stateless_rpc,
			 N_("quit after a single request/response exchange")),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs,
				N_("serve up the info/refs for git-http-backend")),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_BOOL(0, "strict", &strict,
			 N_("do not try <directory>/.git/ if <directory> is no Git directory")),
		OPT_INTEGER(0, "timeout", &timeout,
			    N_("interrupt transfer after <n> seconds of inactivity")),
		OPT_ಕೊನೆ()
	};
	ಚಿಹ್ನೆರಹಿತ enter_repo_flags = ENTER_REPO_ANY_OWNER_OK;

	packet_trace_ತಟಸ್ಥ("upload-pack");
	disable_replace_refs();
	save_commit_ಬಫರ್ = 0;
	xsetenv(NO_LAZY_FETCH_ENVIRONMENT, "1", 0);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, upload_pack_usage, 0);

	ಆದರೆ (argc != 1)
		usage_with_options(upload_pack_usage, options);

	ಸಿದ್ಧತೆ_ಮಾರ್ಗ();

	dir = argv[0];

	ಆದರೆ (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	ಆದರೆ (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	ಆಯ್ಕೆ (determine_protocol_ಆವೃತ್ತಿ_server()) {
	ಸ್ಥಿತಿ protocol_v2:
		ಆದರೆ (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		ಇಲ್ಲದಿದ್ದರೆ
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ಆದರೆ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	ಸ್ಥಿತಿ protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ protocol_unknown_ಆವೃತ್ತಿ:
		BUG("unknown protocol version");
	}

	ಹಿಂತಿರುಗಿ 0;
}
