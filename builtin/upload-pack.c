#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "exec-cmd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pkt-line.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "protocol.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "replace-object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "upload-pack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "serve.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	ਨਿਰਾ
};

ਪੂਰਨ cmd_upload_pack(ਪੂਰਨ argc,
		    ਅਚਲ ਅੱਖਰ **argv,
		    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *dir;
	ਪੂਰਨ strict = 0;
	ਪੂਰਨ advertise_refs = 0;
	ਪੂਰਨ stateless_rpc = 0;
	ਪੂਰਨ timeout = 0;
	ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "stateless-rpc", &stateless_rpc,
			 N_("quit after a single request/response exchange")),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs,
				N_("serve up the info/refs for git-http-backend")),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_BOOL(0, "strict", &strict,
			 N_("do not try <directory>/.git/ if <directory> is no Git directory")),
		OPT_INTEGER(0, "timeout", &timeout,
			    N_("interrupt transfer after <n> seconds of inactivity")),
		OPT_ਸਿਰਾ()
	};
	ਬਿਨਾਨਿਸ਼ਾਨੀ enter_repo_flags = ENTER_REPO_ANY_OWNER_OK;

	packet_trace_ਪਛਾਣ_ਤੱਤ("upload-pack");
	disable_replace_refs();
	save_commit_ਬਫਰ = 0;
	xsetenv(NO_LAZY_FETCH_ENVIRONMENT, "1", 0);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, upload_pack_usage, 0);

	ਜੇ (argc != 1)
		usage_with_options(upload_pack_usage, options);

	ਤਿਆਰੀ_ਰਸਤਾ();

	dir = argv[0];

	ਜੇ (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	ਜੇ (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	ਚੋਣ (determine_protocol_ਸੰਸਕਰਣ_server()) {
	ਹਾਲਤ protocol_v2:
		ਜੇ (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		ਨਹੀਂਤਾਂ
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		ਰੋਕੋ;
	ਹਾਲਤ protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ਜੇ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	ਹਾਲਤ protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		ਰੋਕੋ;
	ਹਾਲਤ protocol_unknown_ਸੰਸਕਰਣ:
		BUG("unknown protocol version");
	}

	ਵਾਪਸ 0;
}
