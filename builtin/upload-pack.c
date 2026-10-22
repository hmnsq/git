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

puur_naa_nka cmd_upload_pack(puur_naa_nka argc,
		    const var_na **argv,
		    const var_na *prefix,
		    sa_nracanaa repository *repo UNUSED)
{
	const var_na *dir;
	puur_naa_nka strict = 0;
	puur_naa_nka advertise_refs = 0;
	puur_naa_nka stateless_rpc = 0;
	puur_naa_nka timeout = 0;
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

	ᰀᰧ (argc != 1)
		usage_with_options(upload_pack_usage, options);

	setup_path();

	dir = argv[0];

	ᰀᰧ (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	ᰀᰧ (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	switch (determine_protocol_version_server()) {
	pak_sa protocol_v2:
		ᰀᰧ (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		ᰀᰧᰀ
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		bha_nga;
	pak_sa protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ᰀᰧ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	pak_sa protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		bha_nga;
	pak_sa protocol_unknown_version:
		BUG("unknown protocol version");
	}

	ᰀᰧᰌ 0;
}
