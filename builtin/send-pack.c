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

ساكن بنية send_pack_دلیلاں دلیلاں;

ساكن فراغ print_helper_حالت(بنية ref *ref)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية ref_ڈالو_report *report;

	لكل (; ref; ref = ref->اگلا) {
		ثابت حرف *پیغام = عدم;
		ثابت حرف *res;
		صحيح گنتی = 0;

		بدل(ref->حالت) {
		حالة REF_حالت_NONE:
			res = "error";
			پیغام = "no match";
			اكسر;

		حالة REF_حالت_OK:
			res = "ok";
			اكسر;

		حالة REF_حالت_UPTODATE:
			res = "ok";
			پیغام = "up to date";
			اكسر;

		حالة REF_حالت_REJECT_NONFASTFORWARD:
			res = "error";
			پیغام = "non-fast forward";
			اكسر;

		حالة REF_حالت_REJECT_FETCH_پہلا:
			res = "error";
			پیغام = "fetch first";
			اكسر;

		حالة REF_حالت_REJECT_NEEDS_FORCE:
			res = "error";
			پیغام = "needs force";
			اكسر;

		حالة REF_حالت_REJECT_STALE:
			res = "error";
			پیغام = "stale info";
			اكسر;

		حالة REF_حالت_REJECT_REMOTE_UPDATED:
			res = "error";
			پیغام = "remote ref updated since checkout";
			اكسر;

		حالة REF_حالت_REJECT_ALREADY_EXISTS:
			res = "error";
			پیغام = "already exists";
			اكسر;

		حالة REF_حالت_REJECT_NODELETE:
		حالة REF_حالت_REMOTE_REJECT:
			res = "error";
			اكسر;

		حالة REF_حالت_EXPECTING_REPORT:
			res = "error";
			پیغام = "expecting report";
			اكسر;

		افتراضي:
			استمر;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		إذا (ref->remote_حالت)
			پیغام = ref->remote_حالت;
		إذا (پیغام) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", پیغام, 0);
		}
		strbuf_addch(&buf, '\n');

		إذا (ref->حالت == REF_حالت_OK) {
			لكل (report = ref->report; report; report = report->اگلا) {
				إذا (گنتی++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				إذا (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				إذا (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_منزل_hex(report->old_oid));
				إذا (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_منزل_hex(report->new_oid));
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
			دلیلاں.ڈالو_cert = SEND_PACK_ڈالو_CERT_NEVER;
			اكسر;
		حالة 1:
			دلیلاں.ڈالو_cert = SEND_PACK_ڈالو_CERT_ALWAYS;
			اكسر;
		افتراضي:
			إذا (!strcasecmp(v, "if-asked"))
				دلیلاں.ڈالو_cert = SEND_PACK_ڈالو_CERT_IF_ASKED;
			وإلا
				أرجع error(_("invalid value for '%s'"), k);
		}
	}
	أرجع git_default_config(k, v, ctx, cb);
}

صحيح cmd_send_pack(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *اگیتر,
		  بنية repository *repo)
{
	بنية refspec rs;
	ثابت حرف *remote_name = عدم;
	بنية remote *remote = عدم;
	ثابت حرف *dest = عدم;
	صحيح fd[2];
	بنية بچہ_process *conn;
	بنية oid_array extra_have = OID_ARRAY_INIT;
	بنية oid_array shallow = OID_ARRAY_INIT;
	بنية ref *remote_refs, *local_refs;
	صحيح ret;
	صحيح helper_حالت = 0;
	صحيح send_all = 0;
	صحيح verbose = 0;
	ثابت حرف *receivepack = "git-receive-pack";
	بنانشانی dry_لگاتار = 0;
	بنانشانی send_mirror = 0;
	بنانشانی force_update = 0;
	بنانشانی quiet = 0;
	صحيح ڈالو_cert = 0;
	بنية string_list ڈالو_options = STRING_LIST_INIT_NODUP;
	بنانشانی use_thin_pack = 0;
	بنانشانی atomic = 0;
	بنانشانی stateless_rpc = 0;
	صحيح flags;
	بنانشانی صحيح reject_reasons;
	صحيح ترقی = -1;
	صحيح from_stdin = 0;
	بنية ڈالو_cas_option cas = {0};
	صحيح force_if_includes = 0;
	بنية packet_reader reader;

	بنية option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_لگاتار, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &ڈالو_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_ڈالو_signed),
		OPT_STRING_LIST(0, "push-option", &ڈالو_options,
				N_("server-specific"),
				N_("option to transmit")),
		OPT_BOOL(0, "progress", &ترقی, N_("force progress reporting")),
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
		OPT_سرا()
	};

	repo_config(repo, send_pack_config, عدم);
	argc = parse_options(argc, argv, اگیتر, options, send_pack_usage, 0);

	refspec_init_ڈالو(&rs, repo->hash_algo);

	إذا (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	إذا (!dest)
		usage_with_options(send_pack_usage, options);

	دلیلاں.verbose = verbose;
	دلیلاں.dry_لگاتار = dry_لگاتار;
	دلیلاں.send_mirror = send_mirror;
	دلیلاں.force_update = force_update;
	دلیلاں.quiet = quiet;
	دلیلاں.ڈالو_cert = ڈالو_cert;
	دلیلاں.ترقی = ترقی;
	دلیلاں.use_thin_pack = use_thin_pack;
	دلیلاں.atomic = atomic;
	دلیلاں.stateless_rpc = stateless_rpc;
	دلیلاں.ڈالو_options = ڈالو_options.nr ? &ڈالو_options : عدم;
	دلیلاں.url = dest;

	إذا (from_stdin) {
		إذا (دلیلاں.stateless_rpc) {
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
	إذا ((rs.nr > 0 && (send_all || دلیلاں.send_mirror)) ||
	    (send_all && دلیلاں.send_mirror))
		usage_with_options(send_pack_usage, options);

	إذا (remote_name) {
		remote = remote_get(remote_name);
		إذا (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	إذا (ترقی == -1)
		ترقی = !دلیلاں.quiet && isatty(2);
	دلیلاں.ترقی = ترقی;

	إذا (دلیلاں.stateless_rpc) {
		conn = عدم;
		fd[0] = 0;
		fd[1] = 1;
	} وإلا {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   دلیلاں.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	بدل (discover_نسخہ(&reader)) {
	حالة protocol_v2:
		die("support for protocol v2 not implemented yet");
		اكسر;
	حالة protocol_v1:
	حالة protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_عام,
				 &extra_have, &shallow);
		اكسر;
	حالة protocol_unknown_نسخہ:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = میل_REFS_NONE;

	إذا (send_all)
		flags |= میل_REFS_ALL;
	إذا (دلیلاں.send_mirror)
		flags |= میل_REFS_MIRROR;

	/* match them up */
	إذا (میل_ڈالو_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		اذهب cleanup;
	}
	إذا (!is_خالی_تھاواں_cas(&cas))
		apply_ڈالو_cas(&cas, remote, remote_refs);

	إذا (!is_خالی_تھاواں_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	سیٹ_ref_حالت_for_ڈالو(remote_refs, دلیلاں.send_mirror,
		دلیلاں.force_update);

	ret = send_pack(repo, &دلیلاں, fd, conn, remote_refs, &extra_have);

	إذا (helper_حالت)
		print_helper_حالت(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= انت_connect(conn);

	إذا (!helper_حالت)
		transport_print_ڈالو_حالت(dest, remote_refs, دلیلاں.verbose, 0, &reject_reasons);

	إذا (!دلیلاں.dry_لگاتار && remote) {
		بنية ref *ref;
		لكل (ref = remote_refs; ref; ref = ref->اگلا)
			transport_update_tracking_ref(remote, ref, دلیلاں.verbose);
	}

	إذا (!ret && !transport_refs_pushed(remote_refs))
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
	أرجع ret;
}
