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

ساكن بنية send_pack_دلیلونه دلیلونه;

ساكن فراغ print_helper_حالت(بنية ref *ref)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية ref_ټېل_report *report;

	لكل (; ref; ref = ref->بل) {
		ثابت حرف *پیغام = عدم;
		ثابت حرف *res;
		صحيح شمېر = 0;

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

		حالة REF_حالت_REJECT_FETCH_لومړی:
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
			لكل (report = ref->report; report; report = report->بل) {
				إذا (شمېر++ > 0)
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
			دلیلونه.ټېل_cert = SEND_PACK_ټېل_CERT_NEVER;
			اكسر;
		حالة 1:
			دلیلونه.ټېل_cert = SEND_PACK_ټېل_CERT_ALWAYS;
			اكسر;
		افتراضي:
			إذا (!strcasecmp(v, "if-asked"))
				دلیلونه.ټېل_cert = SEND_PACK_ټېل_CERT_IF_ASKED;
			وإلا
				أرجع error(_("invalid value for '%s'"), k);
		}
	}
	أرجع git_default_config(k, v, ctx, cb);
}

صحيح cmd_send_pack(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *مختاړی,
		  بنية repository *repo)
{
	بنية refspec rs;
	ثابت حرف *remote_name = عدم;
	بنية remote *remote = عدم;
	ثابت حرف *dest = عدم;
	صحيح fd[2];
	بنية ماشوم_process *conn;
	بنية oid_array extra_have = OID_ARRAY_INIT;
	بنية oid_array shallow = OID_ARRAY_INIT;
	بنية ref *remote_refs, *local_refs;
	صحيح ret;
	صحيح helper_حالت = 0;
	صحيح send_all = 0;
	صحيح verbose = 0;
	ثابت حرف *receivepack = "git-receive-pack";
	بینښه dry_پرلپسې = 0;
	بینښه send_mirror = 0;
	بینښه force_update = 0;
	بینښه quiet = 0;
	صحيح ټېل_cert = 0;
	بنية string_list ټېل_options = STRING_LIST_INIT_NODUP;
	بینښه use_thin_pack = 0;
	بینښه atomic = 0;
	بینښه stateless_rpc = 0;
	صحيح flags;
	بینښه صحيح reject_reasons;
	صحيح پرمختګ = -1;
	صحيح from_stdin = 0;
	بنية ټېل_cas_option cas = {0};
	صحيح force_if_includes = 0;
	بنية packet_reader reader;

	بنية option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_پرلپسې, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &ټېل_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_ټېل_signed),
		OPT_STRING_LIST(0, "push-option", &ټېل_options,
				N_("server-specific"),
				N_("option to transmit")),
		OPT_BOOL(0, "progress", &پرمختګ, N_("force progress reporting")),
		OPT_BOOL(0, "thin", &use_thin_pack, N_("use thin pack")),
		OPT_BOOL(0, "atomic", &atomic, N_("request atomic transaction on remote side")),
		OPT_BOOL(0, "stateless-rpc", &stateless_rpc, N_("use stateless RPC protocol")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read refs from stdin")),
		OPT_BOOL(0, "helper-status", &helper_حالت, N_("print status from remote helper")),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
		  N_("require old value of ref to be at this value"),
		  PARSE_OPT_OPTARG, parseopt_ټېل_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_پای_ټکی()
	};

	repo_config(repo, send_pack_config, عدم);
	argc = parse_options(argc, argv, مختاړی, options, send_pack_usage, 0);

	refspec_init_ټېل(&rs, repo->hash_algo);

	إذا (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	إذا (!dest)
		usage_with_options(send_pack_usage, options);

	دلیلونه.verbose = verbose;
	دلیلونه.dry_پرلپسې = dry_پرلپسې;
	دلیلونه.send_mirror = send_mirror;
	دلیلونه.force_update = force_update;
	دلیلونه.quiet = quiet;
	دلیلونه.ټېل_cert = ټېل_cert;
	دلیلونه.پرمختګ = پرمختګ;
	دلیلونه.use_thin_pack = use_thin_pack;
	دلیلونه.atomic = atomic;
	دلیلونه.stateless_rpc = stateless_rpc;
	دلیلونه.ټېل_options = ټېل_options.nr ? &ټېل_options : عدم;
	دلیلونه.url = dest;

	إذا (from_stdin) {
		إذا (دلیلونه.stateless_rpc) {
			ثابت حرف *buf;
			طالما ((buf = packet_read_کرښه(0, عدم)))
				refspec_append(&rs, buf);
		} وإلا {
			بنية strbuf کرښه = STRBUF_INIT;
			طالما (strbuf_getline(&کرښه, stdin) != EOF)
				refspec_append(&rs, کرښه.buf);
			strbuf_release(&کرښه);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	إذا ((rs.nr > 0 && (send_all || دلیلونه.send_mirror)) ||
	    (send_all && دلیلونه.send_mirror))
		usage_with_options(send_pack_usage, options);

	إذا (remote_name) {
		remote = remote_get(remote_name);
		إذا (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	إذا (پرمختګ == -1)
		پرمختګ = !دلیلونه.quiet && isatty(2);
	دلیلونه.پرمختګ = پرمختګ;

	إذا (دلیلونه.stateless_rpc) {
		conn = عدم;
		fd[0] = 0;
		fd[1] = 1;
	} وإلا {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   دلیلونه.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	بدل (discover_نسخه(&reader)) {
	حالة protocol_v2:
		die("support for protocol v2 not implemented yet");
		اكسر;
	حالة protocol_v1:
	حالة protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_نورمال,
				 &extra_have, &shallow);
		اكسر;
	حالة protocol_unknown_نسخه:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = سمون_REFS_NONE;

	إذا (send_all)
		flags |= سمون_REFS_ALL;
	إذا (دلیلونه.send_mirror)
		flags |= سمون_REFS_MIRROR;

	/* match them up */
	إذا (سمون_ټېل_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		اذهب cleanup;
	}
	إذا (!is_تشه_cas(&cas))
		apply_ټېل_cas(&cas, remote, remote_refs);

	إذا (!is_تشه_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	سېټ_ref_حالت_for_ټېل(remote_refs, دلیلونه.send_mirror,
		دلیلونه.force_update);

	ret = send_pack(repo, &دلیلونه, fd, conn, remote_refs, &extra_have);

	إذا (helper_حالت)
		print_helper_حالت(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= پای_connect(conn);

	إذا (!helper_حالت)
		transport_print_ټېل_حالت(dest, remote_refs, دلیلونه.verbose, 0, &reject_reasons);

	إذا (!دلیلونه.dry_پرلپسې && remote) {
		بنية ref *ref;
		لكل (ref = remote_refs; ref; ref = ref->بل)
			transport_update_tracking_ref(remote, ref, دلیلونه.verbose);
	}

	إذا (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&ټېل_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	أرجع ret;
}
