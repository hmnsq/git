#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "exec-cmd.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "pkt-line.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "protocol.h"
#அந்தர்பாவ "replace-object.h"
#அந்தர்பாவ "upload-pack.h"
#அந்தர்பாவ "serve.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "environment.h"

நிலையான மாறிலி எழுத்து * மாறிலி upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	வெற்று
};

முழு cmd_upload_pack(முழு argc,
		    மாறிலி எழுத்து **argv,
		    மாறிலி எழுத்து *prefix,
		    கட்டமைப்பு repository *repo UNUSED)
{
	மாறிலி எழுத்து *dir;
	முழு strict = 0;
	முழு advertise_refs = 0;
	முழு stateless_rpc = 0;
	முழு timeout = 0;
	கட்டமைப்பு option options[] = {
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

	எனில் (argc != 1)
		usage_with_options(upload_pack_usage, options);

	setup_path();

	dir = argv[0];

	எனில் (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	எனில் (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	தேர்வு (determine_protocol_version_server()) {
	நிலை protocol_v2:
		எனில் (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		இல்லையெனில்
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		நிறுத்து;
	நிலை protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		எனில் (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	நிலை protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		நிறுத்து;
	நிலை protocol_unknown_version:
		BUG("unknown protocol version");
	}

	திரும்பு 0;
}
