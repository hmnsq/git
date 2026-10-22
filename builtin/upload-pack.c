#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "exec-cmd.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "pkt-line.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "protocol.h"
#અન્તર્ભાવ "replace-object.h"
#અન્તર્ભાવ "upload-pack.h"
#અન્તર્ભાવ "serve.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "environment.h"

સ્થિર અચલ અક્ષર * અચલ upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	ખાલી
};

પૂર્ણાંક cmd_upload_pack(પૂર્ણાંક argc,
		    અચલ અક્ષર **argv,
		    અચલ અક્ષર *prefix,
		    સંરચના repository *repo UNUSED)
{
	અચલ અક્ષર *dir;
	પૂર્ણાંક strict = 0;
	પૂર્ણાંક advertise_refs = 0;
	પૂર્ણાંક stateless_rpc = 0;
	પૂર્ણાંક timeout = 0;
	સંરચના option options[] = {
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

	જો (argc != 1)
		usage_with_options(upload_pack_usage, options);

	setup_path();

	dir = argv[0];

	જો (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	જો (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	પસંદગી (determine_protocol_version_server()) {
	સ્થિતિ protocol_v2:
		જો (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		નહીંતર
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		તોડો;
	સ્થિતિ protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		જો (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	સ્થિતિ protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		તોડો;
	સ્થિતિ protocol_unknown_version:
		BUG("unknown protocol version");
	}

	પાછા 0;
}
