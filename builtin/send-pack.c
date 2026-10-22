#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "hex.h"
#شامل "pkt-line.h"
#شامل "run-command.h"
#شامل "remote.h"
#شامل "connect.h"
#شامل "send-pack.h"
#شامل "quote.h"
#شامل "transport.h"
#شامل "oid-array.h"
#شامل "gettext.h"
#شامل "protocol.h"
#شامل "parse-options.h"
#شامل "write-or-die.h"

ساکن ثابت حرف * ثابت send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	تہی,
};

ساکن ساخت send_pack_دلائل دلائل;

ساکن خالی print_helper_حالت(ساخت ref *ref)
{
	ساخت strbuf buf = STRBUF_INIT;
	ساخت ref_ڈالو_report *report;

	برائے (; ref; ref = ref->اگلا) {
		ثابت حرف *پیغام = تہی;
		ثابت حرف *res;
		صحیح گنتی = 0;

		انتخاب(ref->حالت_2) {
		حالت REF_حالت_NONE:
			res = "error";
			پیغام = "no match";
			توڑو;

		حالت REF_حالت_OK:
			res = "ok";
			توڑو;

		حالت REF_حالت_UPTODATE:
			res = "ok";
			پیغام = "up to date";
			توڑو;

		حالت REF_حالت_REJECT_NONFASTFORWARD:
			res = "error";
			پیغام = "non-fast forward";
			توڑو;

		حالت REF_حالت_REJECT_FETCH_پہلا:
			res = "error";
			پیغام = "fetch first";
			توڑو;

		حالت REF_حالت_REJECT_NEEDS_FORCE:
			res = "error";
			پیغام = "needs force";
			توڑو;

		حالت REF_حالت_REJECT_STALE:
			res = "error";
			پیغام = "stale info";
			توڑو;

		حالت REF_حالت_REJECT_REMOTE_UPDATED:
			res = "error";
			پیغام = "remote ref updated since checkout";
			توڑو;

		حالت REF_حالت_REJECT_ALREADY_EXISTS:
			res = "error";
			پیغام = "already exists";
			توڑو;

		حالت REF_حالت_REJECT_NODELETE:
		حالت REF_حالت_REMOTE_REJECT:
			res = "error";
			توڑو;

		حالت REF_حالت_EXPECTING_REPORT:
			res = "error";
			پیغام = "expecting report";
			توڑو;

		طےشدہ:
			جاری;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		اگر (ref->remote_حالت)
			پیغام = ref->remote_حالت;
		اگر (پیغام) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", پیغام, 0);
		}
		strbuf_addch(&buf, '\n');

		اگر (ref->حالت_2 == REF_حالت_OK) {
			برائے (report = ref->report; report; report = report->اگلا) {
				اگر (گنتی++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				اگر (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				اگر (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_مقصد_hex(report->old_oid));
				اگر (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_مقصد_hex(report->new_oid));
				اگر (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

ساکن صحیح send_pack_config(ثابت حرف *k, ثابت حرف *v,
			    ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(k, "push.gpgsign")) {
		انتخاب (git_parse_maybe_bool(v)) {
		حالت 0:
			دلائل.ڈالو_cert = SEND_PACK_ڈالو_CERT_NEVER;
			توڑو;
		حالت 1:
			دلائل.ڈالو_cert = SEND_PACK_ڈالو_CERT_ALWAYS;
			توڑو;
		طےشدہ:
			اگر (!strcasecmp(v, "if-asked"))
				دلائل.ڈالو_cert = SEND_PACK_ڈالو_CERT_IF_ASKED;
			ورنہ
				واپس error(_("invalid value for '%s'"), k);
		}
	}
	واپس git_default_config(k, v, ctx, cb);
}

صحیح cmd_send_pack(صحیح argc,
		  ثابت حرف **argv,
		  ثابت حرف *سابقہ,
		  ساخت repository *repo)
{
	ساخت refspec rs;
	ثابت حرف *remote_name = تہی;
	ساخت remote *remote = تہی;
	ثابت حرف *dest = تہی;
	صحیح fd[2];
	ساخت بچہ_process *conn;
	ساخت oid_array extra_have = OID_ARRAY_INIT;
	ساخت oid_array shallow = OID_ARRAY_INIT;
	ساخت ref *remote_refs, *local_refs;
	صحیح ret;
	صحیح helper_حالت = 0;
	صحیح send_all = 0;
	صحیح verbose = 0;
	ثابت حرف *receivepack = "git-receive-pack";
	غیرعلامتی dry_تسلسل = 0;
	غیرعلامتی send_mirror = 0;
	غیرعلامتی force_update = 0;
	غیرعلامتی quiet = 0;
	صحیح ڈالو_cert = 0;
	ساخت string_list ڈالو_options = STRING_LIST_INIT_NODUP;
	غیرعلامتی use_thin_pack = 0;
	غیرعلامتی atomic = 0;
	غیرعلامتی stateless_rpc = 0;
	صحیح flags;
	غیرعلامتی صحیح reject_reasons;
	صحیح پیش_رفت = -1;
	صحیح from_stdin = 0;
	ساخت ڈالو_cas_option cas = {0};
	صحیح force_if_includes = 0;
	ساخت packet_reader reader;

	ساخت option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_تسلسل, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &ڈالو_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_ڈالو_signed),
		OPT_STRING_LIST(0, "push-option", &ڈالو_options,
				N_("server-specific"),
				N_("option to transmit")),
		OPT_BOOL(0, "progress", &پیش_رفت, N_("force progress reporting")),
		OPT_BOOL(0, "thin", &use_thin_pack, N_("use thin pack")),
		OPT_BOOL(0, "atomic", &atomic, N_("request atomic transaction on remote side")),
		OPT_BOOL(0, "stateless-rpc", &stateless_rpc, N_("use stateless RPC protocol")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read refs from stdin")),
		OPT_BOOL(0, "helper-status", &helper_حالت, N_("print status from remote helper")),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
		  N_("require old value of ref to be at this value"),
		  PARSE_OPT_OPTARG, parseopt_ڈالو_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_انتہا()
	};

	repo_config(repo, send_pack_config, تہی);
	argc = parse_options(argc, argv, سابقہ, options, send_pack_usage, 0);

	refspec_init_ڈالو(&rs, repo->hash_algo);

	اگر (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	اگر (!dest)
		usage_with_options(send_pack_usage, options);

	دلائل.verbose = verbose;
	دلائل.dry_تسلسل = dry_تسلسل;
	دلائل.send_mirror = send_mirror;
	دلائل.force_update = force_update;
	دلائل.quiet = quiet;
	دلائل.ڈالو_cert = ڈالو_cert;
	دلائل.پیش_رفت = پیش_رفت;
	دلائل.use_thin_pack = use_thin_pack;
	دلائل.atomic = atomic;
	دلائل.stateless_rpc = stateless_rpc;
	دلائل.ڈالو_options = ڈالو_options.nr ? &ڈالو_options : تہی;
	دلائل.url = dest;

	اگر (from_stdin) {
		اگر (دلائل.stateless_rpc) {
			ثابت حرف *buf;
			جب ((buf = packet_read_سطر(0, تہی)))
				refspec_append(&rs, buf);
		} ورنہ {
			ساخت strbuf سطر = STRBUF_INIT;
			جب (strbuf_getline(&سطر, stdin) != EOF)
				refspec_append(&rs, سطر.buf);
			strbuf_release(&سطر);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	اگر ((rs.nr > 0 && (send_all || دلائل.send_mirror)) ||
	    (send_all && دلائل.send_mirror))
		usage_with_options(send_pack_usage, options);

	اگر (remote_name) {
		remote = remote_get(remote_name);
		اگر (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	اگر (پیش_رفت == -1)
		پیش_رفت = !دلائل.quiet && isatty(2);
	دلائل.پیش_رفت = پیش_رفت;

	اگر (دلائل.stateless_rpc) {
		conn = تہی;
		fd[0] = 0;
		fd[1] = 1;
	} ورنہ {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   دلائل.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], تہی, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	انتخاب (discover_نسخہ(&reader)) {
	حالت protocol_v2:
		die("support for protocol v2 not implemented yet");
		توڑو;
	حالت protocol_v1:
	حالت protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_معمول,
				 &extra_have, &shallow);
		توڑو;
	حالت protocol_unknown_نسخہ:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = مطابقت_REFS_NONE;

	اگر (send_all)
		flags |= مطابقت_REFS_ALL;
	اگر (دلائل.send_mirror)
		flags |= مطابقت_REFS_MIRROR;

	/* match them up */
	اگر (مطابقت_ڈالو_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		جاؤ cleanup;
	}
	اگر (!is_خالی_خانے_cas(&cas))
		apply_ڈالو_cas(&cas, remote, remote_refs);

	اگر (!is_خالی_خانے_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	سیٹ_ref_حالت_for_ڈالو(remote_refs, دلائل.send_mirror,
		دلائل.force_update);

	ret = send_pack(repo, &دلائل, fd, conn, remote_refs, &extra_have);

	اگر (helper_حالت)
		print_helper_حالت(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= اختتام_connect(conn);

	اگر (!helper_حالت)
		transport_print_ڈالو_حالت(dest, remote_refs, دلائل.verbose, 0, &reject_reasons);

	اگر (!دلائل.dry_تسلسل && remote) {
		ساخت ref *ref;
		برائے (ref = remote_refs; ref; ref = ref->اگلا)
			transport_update_tracking_ref(remote, ref, دلائل.verbose);
	}

	اگر (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&ڈالو_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	واپس ret;
}
