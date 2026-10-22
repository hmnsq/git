#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "exec-cmd.h"
#شامل "gettext.h"
#شامل "pkt-line.h"
#شامل "parse-options.h"
#شامل "protocol.h"
#شامل "replace-object.h"
#شامل "upload-pack.h"
#شامل "serve.h"
#شامل "setup.h"
#شامل "commit.h"
#شامل "environment.h"

ایستا ثابت نویسه * ثابت upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	پوچ
};

صحیح cmd_upload_pack(صحیح argc,
		    ثابت نویسه **argv,
		    ثابت نویسه *prefix,
		    ساختار repository *repo UNUSED)
{
	ثابت نویسه *dir;
	صحیح strict = 0;
	صحیح advertise_refs = 0;
	صحیح stateless_rpc = 0;
	صحیح timeout = 0;
	ساختار option options[] = {
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

	اگر (argc != 1)
		usage_with_options(upload_pack_usage, options);

	setup_path();

	dir = argv[0];

	اگر (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	اگر (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	انتخاب (determine_protocol_version_server()) {
	حالت protocol_v2:
		اگر (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		وگرنه
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		بشکن;
	حالت protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		اگر (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	حالت protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		بشکن;
	حالت protocol_unknown_version:
		BUG("unknown protocol version");
	}

	بازگشت 0;
}
