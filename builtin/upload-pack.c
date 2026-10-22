#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "exec-cmd.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "pkt-line.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "protocol.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "upload-pack.h"
#अन्तर्भाव "serve.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "environment.h"

स्थिर स्थिरांक अक्षर * स्थिरांक upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	रिक्त
};

पूर्णांक cmd_upload_pack(पूर्णांक argc,
		    स्थिरांक अक्षर **argv,
		    स्थिरांक अक्षर *prefix,
		    संरचना repository *repo UNUSED)
{
	स्थिरांक अक्षर *dir;
	पूर्णांक strict = 0;
	पूर्णांक advertise_refs = 0;
	पूर्णांक stateless_rpc = 0;
	पूर्णांक timeout = 0;
	संरचना option options[] = {
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
	अचिन्हित enter_repo_flags = ENTER_REPO_ANY_OWNER_OK;

	packet_trace_identity("upload-pack");
	disable_replace_refs();
	save_commit_buffer = 0;
	xsetenv(NO_LAZY_FETCH_ENVIRONMENT, "1", 0);

	argc = parse_options(argc, argv, prefix, options, upload_pack_usage, 0);

	जर (argc != 1)
		usage_with_options(upload_pack_usage, options);

	setup_path();

	dir = argv[0];

	जर (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	जर (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	निवड (determine_protocol_version_server()) {
	स्थिती protocol_v2:
		जर (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		अन्यथा
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		थांबा;
	स्थिती protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		जर (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	स्थिती protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		थांबा;
	स्थिती protocol_unknown_version:
		BUG("unknown protocol version");
	}

	परत 0;
}
