#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VERIFY_PACK_VERBOSE 01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VERIFY_PACK_STAT_ONLY 02

ਸਥਿਰ ਪੂਰਨ verify_one_pack(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags, ਅਚਲ ਅੱਖਰ *hash_algo)
{
	ਬਣਤਰ ਬੱਚਾ_process ਸੂਚਕਾਂਕ_pack = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ strvec *argv = &ਸੂਚਕਾਂਕ_pack.ਦਲੀਲਾਂ;
	ਬਣਤਰ strbuf arg = STRBUF_INIT;
	ਪੂਰਨ verbose = flags & VERIFY_PACK_VERBOSE;
	ਪੂਰਨ stat_only = flags & VERIFY_PACK_STAT_ONLY;
	ਪੂਰਨ err;

	strvec_ਧੱਕੋ(argv, "index-pack");

	ਜੇ (stat_only)
		strvec_ਧੱਕੋ(argv, "--verify-stat-only");
	ਨਹੀਂਤਾਂ ਜੇ (verbose)
		strvec_ਧੱਕੋ(argv, "--verify-stat");
	ਨਹੀਂਤਾਂ
		strvec_ਧੱਕੋ(argv, "--verify");

	ਜੇ (hash_algo)
		strvec_pushf(argv, "--object-format=%s", hash_algo);

	/*
	 * In addition to "foo.pack" we accept "foo.idx" and "foo";
	 * normalize these forms to "foo.pack" for "index-pack --verify".
	 */
	strbuf_addstr(&arg, ਰਸਤਾ);
	ਜੇ (strbuf_strip_suffix(&arg, ".idx") ||
	    !ਸਿਰੇ_with(arg.buf, ".pack"))
		strbuf_addstr(&arg, ".pack");
	strvec_ਧੱਕੋ(argv, arg.buf);

	ਸੂਚਕਾਂਕ_pack.git_cmd = 1;

	err = ਲੜੀਵਾਰ_command(&ਸੂਚਕਾਂਕ_pack);

	ਜੇ (verbose || stat_only) {
		ਜੇ (err)
			ਲਿਖੋ("%s: bad\n", arg.buf);
		ਨਹੀਂਤਾਂ {
			ਜੇ (!stat_only)
				ਲਿਖੋ("%s: ok\n", arg.buf);
		}
	}
	strbuf_release(&arg);

	ਵਾਪਸ err;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ verify_pack_usage[] = {
	N_("git verify-pack [-v | --verbose] [-s | --stat-only] [--] <pack>.idx..."),
	ਨਿਰਾ
};

ਪੂਰਨ cmd_verify_pack(ਪੂਰਨ argc,
		    ਅਚਲ ਅੱਖਰ **argv,
		    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ err = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = 0;
	ਅਚਲ ਅੱਖਰ *object_format = ਨਿਰਾ;
	ਪੂਰਨ i;
	ਅਚਲ ਬਣਤਰ option verify_pack_options[] = {
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('v', "verbose", &flags, N_("verbose"),
			VERIFY_PACK_VERBOSE),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('s', "stat-only", &flags, N_("show statistics only"),
			VERIFY_PACK_STAT_ONLY),
		OPT_STRING(0, "object-format", &object_format, N_("hash"),
			   N_("specify the hash algorithm to use")),
		OPT_ਸਿਰਾ()
	};

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, verify_pack_options,
			     verify_pack_usage, 0);
	ਜੇ (argc < 1)
		usage_with_options(verify_pack_usage, verify_pack_options);
	ਲਈ (i = 0; i < argc; i++) {
		ਜੇ (verify_one_pack(argv[i], flags, object_format))
			err = 1;
	}

	ਵਾਪਸ err;
}
