#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "exec-cmd.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "pkt-line.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "protocol.h"
#അന്തര്ഭാവ "replace-object.h"
#അന്തര്ഭാവ "upload-pack.h"
#അന്തര്ഭാവ "serve.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "environment.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	ശൂന്യ
};

പൂർണ്ണസംഖ്യ cmd_upload_pack(പൂർണ്ണസംഖ്യ argc,
		    സ്ഥിരം അക്ഷരം **argv,
		    സ്ഥിരം അക്ഷരം *prefix,
		    ഘടന repository *repo UNUSED)
{
	സ്ഥിരം അക്ഷരം *dir;
	പൂർണ്ണസംഖ്യ strict = 0;
	പൂർണ്ണസംഖ്യ advertise_refs = 0;
	പൂർണ്ണസംഖ്യ stateless_rpc = 0;
	പൂർണ്ണസംഖ്യ timeout = 0;
	ഘടന option options[] = {
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

	എങ്കിൽ (argc != 1)
		usage_with_options(upload_pack_usage, options);

	setup_path();

	dir = argv[0];

	എങ്കിൽ (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	എങ്കിൽ (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	തിരഞ്ഞെടുപ്പ് (determine_protocol_version_server()) {
	അവസ്ഥ protocol_v2:
		എങ്കിൽ (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		അല്ലെങ്കിൽ
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		നിർത്തുക;
	അവസ്ഥ protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		എങ്കിൽ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	അവസ്ഥ protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		നിർത്തുക;
	അവസ്ഥ protocol_unknown_version:
		BUG("unknown protocol version");
	}

	തിരികെ 0;
}
