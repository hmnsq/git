#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "hex.h"
#ضمن "pkt-line.h"
#ضمن "run-command.h"
#ضمن "remote.h"
#ضمن "connect.h"
#ضمن "send-pack.h"
#ضمن "quote.h"
#ضمن "transport.h"
#ضمن "oid-array.h"
#ضمن "gettext.h"
#ضمن "protocol.h"
#ضمن "parse-options.h"
#ضمن "write-or-die.h"

ساكن ثابت حرف * ثابت send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	عدم,
};

ساكن بنية send_pack_وسائط وسائط;

ساكن فراغ اطبع_helper_status(بنية ref *ref)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية ref_ادفع_report *report;

	لكل (; ref; ref = ref->next) {
		ثابت حرف *رسالة = عدم;
		ثابت حرف *res;
		صحيح عدد = 0;

		بدل(ref->status) {
		حالة REF_STATUS_NONE:
			res = "error";
			رسالة = "no match";
			اكسر;

		حالة REF_STATUS_OK:
			res = "ok";
			اكسر;

		حالة REF_STATUS_UPTODATE:
			res = "ok";
			رسالة = "up to date";
			اكسر;

		حالة REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			رسالة = "non-fast forward";
			اكسر;

		حالة REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			رسالة = "fetch first";
			اكسر;

		حالة REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			رسالة = "needs force";
			اكسر;

		حالة REF_STATUS_REJECT_STALE:
			res = "error";
			رسالة = "stale info";
			اكسر;

		حالة REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			رسالة = "remote ref updated since checkout";
			اكسر;

		حالة REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			رسالة = "already exists";
			اكسر;

		حالة REF_STATUS_REJECT_NODELETE:
		حالة REF_STATUS_REMOTE_REJECT:
			res = "error";
			اكسر;

		حالة REF_STATUS_EXPECTING_REPORT:
			res = "error";
			رسالة = "expecting report";
			اكسر;

		افتراضي:
			استمر;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		إذا (ref->remote_status)
			رسالة = ref->remote_status;
		إذا (رسالة) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", رسالة, 0);
		}
		strbuf_addch(&buf, '\n');

		إذا (ref->status == REF_STATUS_OK) {
			لكل (report = ref->report; report; report = report->next) {
				إذا (عدد++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				إذا (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				إذا (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_to_hex(report->old_oid));
				إذا (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_to_hex(report->new_oid));
				إذا (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

ساكن صحيح send_pack_config(ثابت حرف *k, ثابت حرف *v,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(k, "push.gpgsign")) {
		بدل (git_parse_maybe_bool(v)) {
		حالة 0:
			وسائط.ادفع_cert = SEND_PACK_ادفع_CERT_NEVER;
			اكسر;
		حالة 1:
			وسائط.ادفع_cert = SEND_PACK_ادفع_CERT_ALWAYS;
			اكسر;
		افتراضي:
			إذا (!strcasecmp(v, "if-asked"))
				وسائط.ادفع_cert = SEND_PACK_ادفع_CERT_IF_ASKED;
			وإلا
				أرجع error(_("invalid value for '%s'"), k);
		}
	}
	أرجع git_default_config(k, v, ctx, cb);
}

صحيح cmd_send_pack(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *prefix,
		  بنية repository *repo)
{
	بنية refspec rs;
	ثابت حرف *remote_name = عدم;
	بنية remote *remote = عدم;
	ثابت حرف *dest = عدم;
	صحيح fd[2];
	بنية child_process *conn;
	بنية oid_array extra_have = OID_ARRAY_INIT;
	بنية oid_array shallow = OID_ARRAY_INIT;
	بنية ref *remote_refs, *local_refs;
	صحيح ret;
	صحيح helper_status = 0;
	صحيح send_all = 0;
	صحيح verbose = 0;
	ثابت حرف *receivepack = "git-receive-pack";
	unsigned dry_run = 0;
	unsigned send_mirror = 0;
	unsigned force_update = 0;
	unsigned quiet = 0;
	صحيح ادفع_cert = 0;
	بنية string_list ادفع_options = STRING_LIST_INIT_NODUP;
	unsigned use_thin_pack = 0;
	unsigned atomic = 0;
	unsigned stateless_rpc = 0;
	صحيح flags;
	unsigned صحيح reject_reasons;
	صحيح progress = -1;
	صحيح from_stdin = 0;
	بنية ادفع_cas_option cas = {0};
	صحيح force_if_includes = 0;
	بنية packet_reader reader;

	بنية option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_run, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &ادفع_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_ادفع_signed),
		OPT_STRING_LIST(0, "push-option", &ادفع_options,
				N_("server-specific"),
				N_("option to transmit")),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_BOOL(0, "thin", &use_thin_pack, N_("use thin pack")),
		OPT_BOOL(0, "atomic", &atomic, N_("request atomic transaction on remote side")),
		OPT_BOOL(0, "stateless-rpc", &stateless_rpc, N_("use stateless RPC protocol")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read refs from stdin")),
		OPT_BOOL(0, "helper-status", &helper_status, N_("print status from remote helper")),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
		  N_("require old value of ref to be at this value"),
		  PARSE_OPT_OPTARG, parseopt_ادفع_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_END()
	};

	repo_config(repo, send_pack_config, عدم);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_ادفع(&rs, repo->hash_algo);

	إذا (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	إذا (!dest)
		usage_with_options(send_pack_usage, options);

	وسائط.verbose = verbose;
	وسائط.dry_run = dry_run;
	وسائط.send_mirror = send_mirror;
	وسائط.force_update = force_update;
	وسائط.quiet = quiet;
	وسائط.ادفع_cert = ادفع_cert;
	وسائط.progress = progress;
	وسائط.use_thin_pack = use_thin_pack;
	وسائط.atomic = atomic;
	وسائط.stateless_rpc = stateless_rpc;
	وسائط.ادفع_options = ادفع_options.nr ? &ادفع_options : عدم;
	وسائط.url = dest;

	إذا (from_stdin) {
		إذا (وسائط.stateless_rpc) {
			ثابت حرف *buf;
			طالما ((buf = packet_read_سطر(0, عدم)))
				refspec_append(&rs, buf);
		} وإلا {
			بنية strbuf سطر = STRBUF_INIT;
			طالما (strbuf_getline(&سطر, stdin) != EOF)
				refspec_append(&rs, سطر.buf);
			strbuf_release(&سطر);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	إذا ((rs.nr > 0 && (send_all || وسائط.send_mirror)) ||
	    (send_all && وسائط.send_mirror))
		usage_with_options(send_pack_usage, options);

	إذا (remote_name) {
		remote = remote_get(remote_name);
		إذا (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	إذا (progress == -1)
		progress = !وسائط.quiet && isatty(2);
	وسائط.progress = progress;

	إذا (وسائط.stateless_rpc) {
		conn = عدم;
		fd[0] = 0;
		fd[1] = 1;
	} وإلا {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   وسائط.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	بدل (discover_version(&reader)) {
	حالة protocol_v2:
		die("support for protocol v2 not implemented yet");
		اكسر;
	حالة protocol_v1:
	حالة protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		اكسر;
	حالة protocol_unknown_version:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = MATCH_REFS_NONE;

	إذا (send_all)
		flags |= MATCH_REFS_ALL;
	إذا (وسائط.send_mirror)
		flags |= MATCH_REFS_MIRROR;

	/* match them up */
	إذا (match_ادفع_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		اذهب cleanup;
	}
	إذا (!is_empty_cas(&cas))
		apply_ادفع_cas(&cas, remote, remote_refs);

	إذا (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_ادفع(remote_refs, وسائط.send_mirror,
		وسائط.force_update);

	ret = send_pack(repo, &وسائط, fd, conn, remote_refs, &extra_have);

	إذا (helper_status)
		اطبع_helper_status(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= finish_connect(conn);

	إذا (!helper_status)
		transport_اطبع_ادفع_status(dest, remote_refs, وسائط.verbose, 0, &reject_reasons);

	إذا (!وسائط.dry_run && remote) {
		بنية ref *ref;
		لكل (ref = remote_refs; ref; ref = ref->next)
			transport_update_tracking_ref(remote, ref, وسائط.verbose);
	}

	إذا (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&ادفع_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	أرجع ret;
}
