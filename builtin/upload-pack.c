#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "exec-cmd.h"
#include "gettext.h"
#include "pkt-line.h"
#include "parse-options.h"
#include "protocol.h"
#include "replace-object.h"
#include "upload-pack.h"
#include "serve.h"
#include "setup.h"
#include "commit.h"
#include "environment.h"

static const var_na * const upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	NULL
};

ကိန်းပြည့် cmd_upload_pack(ကိန်းပြည့် argc,
		    const var_na **argv,
		    const var_na *prefix,
		    sa_nracanaa repository *repo UNUSED)
{
	const var_na *dir;
	ကိန်းပြည့် strict = 0;
	ကိန်းပြည့် advertise_refs = 0;
	ကိန်းပြည့် stateless_rpc = 0;
	ကိန်းပြည့် timeout = 0;
	sa_nracanaa option options[] = {
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

	အကယ်၍ (argc != 1)
		usage_with_options(upload_pack_usage, options);

	setup_path();

	dir = argv[0];

	အကယ်၍ (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	အကယ်၍ (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	switch (determine_protocol_version_server()) {
	pak_sa protocol_v2:
		အကယ်၍ (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		မဟုတ်ပါက
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		bha_nga;
	pak_sa protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		အကယ်၍ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	pak_sa protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		bha_nga;
	pak_sa protocol_unknown_version:
		BUG("unknown protocol version");
	}

	ပြန်ပေး 0;
}
