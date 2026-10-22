#define USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "exec-cmd.h"
#ضمن "gettext.h"
#ضمن "pkt-line.h"
#ضمن "parse-options.h"
#ضمن "protocol.h"
#ضمن "replace-object.h"
#ضمن "upload-pack.h"
#ضمن "serve.h"
#ضمن "setup.h"
#ضمن "commit.h"
#ضمن "environment.h"

ساكن ثابت حرف * ثابت upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	عدم
};

صحيح cmd_upload_pack(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *prefix,
		    بنية repository *repo UNUSED)
{
	ثابت حرف *dir;
	صحيح strict = 0;
	صحيح advertise_refs = 0;
	صحيح stateless_rpc = 0;
	صحيح timeout = 0;
	بنية option options[] = {
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

	ܐܢ (argc != 1)
		usage_with_options(upload_pack_usage, options);

	setup_path();

	dir = argv[0];

	ܐܢ (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	ܐܢ (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	بدل (determine_protocol_version_server()) {
	حالة protocol_v2:
		ܐܢ (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		ܐܚܪܢܐ
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		اكسر;
	حالة protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ܐܢ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	حالة protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		اكسر;
	حالة protocol_unknown_version:
		BUG("unknown protocol version");
	}

	ܦܢܐ 0;
}
