#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "exec-cmd.h"
#समावेश "gettext.h"
#समावेश "pkt-line.h"
#समावेश "parse-options.h"
#समावेश "protocol.h"
#समावेश "replace-object.h"
#समावेश "upload-pack.h"
#समावेश "serve.h"
#समावेश "setup.h"
#समावेश "commit.h"
#समावेश "environment.h"

अटल स्थिर अक्षर * स्थिर upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	शून्य
};

पूर्णांक cmd_upload_pack(पूर्णांक argc,
		    स्थिर अक्षर **argv,
		    स्थिर अक्षर *prefix,
		    काष्ठा repository *repo UNUSED)
{
	स्थिर अक्षर *dir;
	पूर्णांक strict = 0;
	पूर्णांक advertise_refs = 0;
	पूर्णांक stateless_rpc = 0;
	पूर्णांक timeout = 0;
	काष्ठा option options[] = {
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
	save_commit_बफर = 0;
	xsetenv(NO_LAZY_FETCH_ENVIRONMENT, "1", 0);

	argc = parse_options(argc, argv, prefix, options, upload_pack_usage, 0);

	अगर (argc != 1)
		usage_with_options(upload_pack_usage, options);

	setup_path();

	dir = argv[0];

	अगर (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	अगर (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	चयन (determine_protocol_version_server()) {
	हाल protocol_v2:
		अगर (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		अन्यथा
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		अवरोध;
	हाल protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		अगर (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	हाल protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		अवरोध;
	हाल protocol_unknown_version:
		BUG("unknown protocol version");
	}

	वापस 0;
}
