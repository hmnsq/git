#define USE_THE_REPOSITORY_VARIABLE

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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	ರಿಕ್ತ
};

ಪೂರ್ಣಾಙ್ಕ cmd_upload_pack(ಪೂರ್ಣಾಙ್ಕ argc,
		    ಧ್ರುವ ವರ್ಣ **argv,
		    ಧ್ರುವ ವರ್ಣ *prefix,
		    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಧ್ರುವ ವರ್ಣ *dir;
	ಪೂರ್ಣಾಙ್ಕ strict = 0;
	ಪೂರ್ಣಾಙ್ಕ advertise_refs = 0;
	ಪೂರ್ಣಾಙ್ಕ stateless_rpc = 0;
	ಪೂರ್ಣಾಙ್ಕ timeout = 0;
	ಸಂರಚನಾ option options[] = {
		OPT_BOOL(0, "stateless-rpc", &stateless_rpc,
			 N_("quit after a single request/response exchange")),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs,
				N_("serve up the info/refs for git-http-backend")),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_BOOL(0, "strict", &strict,
			 N_("do not try <directory>/.git/ if <directory> is no Git directory")),
		OPT_INTEGER(0, "timeout", &timeout,
			    N_("interrupt transfer after <n> seconds of inactivity")),
		OPT_END()
	};
	unsigned enter_repo_flags = ENTER_REPO_ANY_OWNER_OK;

	packet_trace_identity("upload-pack");
	disable_replace_refs();
	save_commit_buffer = 0;
	xsetenv(NO_LAZY_FETCH_ENVIRONMENT, "1", 0);

	argc = parse_options(argc, argv, prefix, options, upload_pack_usage, 0);

	ಯದಿ (argc != 1)
		usage_with_options(upload_pack_usage, options);

	setup_path();

	dir = argv[0];

	ಯದಿ (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	ಯದಿ (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	ವಿಕಲ್ಪ (determine_protocol_version_server()) {
	ಪಕ್ಷ protocol_v2:
		ಯದಿ (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		ಅನ್ಯಥಾ
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		ಭಙ್ಗ;
	ಪಕ್ಷ protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ಯದಿ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	ಪಕ್ಷ protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		ಭಙ್ಗ;
	ಪಕ್ಷ protocol_unknown_version:
		BUG("unknown protocol version");
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}
