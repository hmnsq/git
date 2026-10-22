#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "exec-cmd.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "pkt-line.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "protocol.h"
#অন্তর্ভাব "replace-object.h"
#অন্তর্ভাব "upload-pack.h"
#অন্তর্ভাব "serve.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "environment.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক upload_pack_usage[] = {
	N_("git-upload-pack [--[no-]strict] [--timeout=<n>] [--stateless-rpc]\n"
	   "                [--advertise-refs] <directory>"),
	ফাঁকা
};

পূর্ণসংখ্যা cmd_upload_pack(পূর্ণসংখ্যা argc,
		    ধ্রুবক অক্ষর **argv,
		    ধ্রুবক অক্ষর *prefix,
		    কাঠামো repository *repo UNUSED)
{
	ধ্রুবক অক্ষর *dir;
	পূর্ণসংখ্যা strict = 0;
	পূর্ণসংখ্যা advertise_refs = 0;
	পূর্ণসংখ্যা stateless_rpc = 0;
	পূর্ণসংখ্যা timeout = 0;
	কাঠামো option options[] = {
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
	save_commit_বাফার = 0;
	xsetenv(NO_LAZY_FETCH_ENVIRONMENT, "1", 0);

	argc = parse_options(argc, argv, prefix, options, upload_pack_usage, 0);

	যদি (argc != 1)
		usage_with_options(upload_pack_usage, options);

	setup_path();

	dir = argv[0];

	যদি (strict)
		enter_repo_flags |= ENTER_REPO_STRICT;
	যদি (!enter_repo(the_repository, dir, enter_repo_flags))
		die("'%s' does not appear to be a git repository", dir);

	নির্বাচন (determine_protocol_version_server()) {
	ক্ষেত্র protocol_v2:
		যদি (advertise_refs)
			protocol_v2_advertise_capabilities(the_repository);
		অন্যথা
			protocol_v2_serve_loop(the_repository, stateless_rpc);
		বিরতি;
	ক্ষেত্র protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		যদি (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	ক্ষেত্র protocol_v0:
		upload_pack(advertise_refs, stateless_rpc, timeout);
		বিরতি;
	ক্ষেত্র protocol_unknown_version:
		BUG("unknown protocol version");
	}

	ফেরত 0;
}
