#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "exec-cmd.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "pkt-line.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "protocol.h"
#ଅନ୍ତର୍ଭାଵ "replace-object.h"
#ଅନ୍ତର୍ଭାଵ "upload-pack.h"
#ଅନ୍ତର୍ଭାଵ "serve.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	ଖାଲି
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_upload_pack(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		    ଅଚଳ ଅକ୍ଷର **argv,
		    ଅଚଳ ଅକ୍ଷର *prefix,
		    ସଂରଚନା repository *repo UNUSED)
{
	ଅଚଳ ଅକ୍ଷର *dir;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା strict = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା advertise_refs = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା stateless_rpc = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା timeout = 0;
	ସଂରଚନା option options[] = {
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

	ଯଦି (argc != 1)
		usage_with_options(upload_pack_usage, options);

	setup_path();

	dir = argv[0];

	ଯଦି (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	ଯଦି (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	ବାଛ (determine_protocol_version_server()) {
	ସ୍ଥିତି protocol_v2:
		ଯଦି (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		ଅନ୍ୟଥା
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ଯଦି (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	ସ୍ଥିତି protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି protocol_unknown_version:
		BUG("unknown protocol version");
	}

	ଫେରନ୍ତୁ 0;
}
