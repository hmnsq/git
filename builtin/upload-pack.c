#परिभाषय USE_THE_REPOSITORY_VARIABLE

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

स्थिर ध्रुव वर्ण * ध्रुव upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	रिक्त
};

पूर्णाङ्क cmd_upload_pack(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *उपसर्गः,
		    संरचना repository *repo अप्रयुक्तम्)
{
	ध्रुव वर्ण *dir;
	पूर्णाङ्क strict = 0;
	पूर्णाङ्क advertise_refs = 0;
	पूर्णाङ्क stateless_rpc = 0;
	पूर्णाङ्क timeout = 0;
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
		OPT_अन्तः()
	};
	अचिह्नित enter_repo_flags = ENTER_REPO_ANY_OWNER_OK;

	packet_trace_तटस्थः("upload-pack");
	disable_replace_refs();
	save_commit_बफर = 0;
	xsetenv(NO_LAZY_FETCH_ENVIRONMENT, "1", 0);

	argc = parse_options(argc, argv, उपसर्गः, options, upload_pack_usage, 0);

	यदि (argc != 1)
		usage_with_options(upload_pack_usage, options);

	सज्जता_मार्गः();

	dir = argv[0];

	यदि (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	यदि (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	विकल्प (determine_protocol_संस्करणम्_server()) {
	पक्ष protocol_v2:
		यदि (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		अन्यथा
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		भङ्ग;
	पक्ष protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		यदि (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	पक्ष protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		भङ्ग;
	पक्ष protocol_unknown_संस्करणम्:
		BUG("unknown protocol version");
	}

	प्रत्यागम 0;
}
