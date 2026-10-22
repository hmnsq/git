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

ایستا ثابت نویسه * ثابت send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	پوچ,
};

ایستا ساختار send_pack_args args;

ایستا تهی print_helper_status(ساختار ref *ref)
{
	ساختار strbuf buf = STRBUF_INIT;
	ساختار ref_افزودن_report *report;

	برای (; ref; ref = ref->next) {
		ثابت نویسه *پیام = پوچ;
		ثابت نویسه *res;
		صحیح شمار = 0;

		انتخاب(ref->status) {
		حالت REF_STATUS_NONE:
			res = "error";
			پیام = "no match";
			بشکن;

		حالت REF_STATUS_OK:
			res = "ok";
			بشکن;

		حالت REF_STATUS_UPTODATE:
			res = "ok";
			پیام = "up to date";
			بشکن;

		حالت REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			پیام = "non-fast forward";
			بشکن;

		حالت REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			پیام = "fetch first";
			بشکن;

		حالت REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			پیام = "needs force";
			بشکن;

		حالت REF_STATUS_REJECT_STALE:
			res = "error";
			پیام = "stale info";
			بشکن;

		حالت REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			پیام = "remote ref updated since checkout";
			بشکن;

		حالت REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			پیام = "already exists";
			بشکن;

		حالت REF_STATUS_REJECT_NODELETE:
		حالت REF_STATUS_REMOTE_REJECT:
			res = "error";
			بشکن;

		حالت REF_STATUS_EXPECTING_REPORT:
			res = "error";
			پیام = "expecting report";
			بشکن;

		پیشفرض:
			ادامه;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		اگر (ref->remote_status)
			پیام = ref->remote_status;
		اگر (پیام) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", پیام, 0);
		}
		strbuf_addch(&buf, '\n');

		اگر (ref->status == REF_STATUS_OK) {
			برای (report = ref->report; report; report = report->next) {
				اگر (شمار++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				اگر (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				اگر (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_to_hex(report->old_oid));
				اگر (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_to_hex(report->new_oid));
				اگر (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

ایستا صحیح send_pack_config(ثابت نویسه *k, ثابت نویسه *v,
			    ثابت ساختار config_context *ctx, تهی *cb)
{
	اگر (!strcmp(k, "push.gpgsign")) {
		انتخاب (git_parse_maybe_bool(v)) {
		حالت 0:
			args.افزودن_cert = SEND_PACK_افزودن_CERT_NEVER;
			بشکن;
		حالت 1:
			args.افزودن_cert = SEND_PACK_افزودن_CERT_ALWAYS;
			بشکن;
		پیشفرض:
			اگر (!strcasecmp(v, "if-asked"))
				args.افزودن_cert = SEND_PACK_افزودن_CERT_IF_ASKED;
			وگرنه
				بازگشت error(_("invalid value for '%s'"), k);
		}
	}
	بازگشت git_default_config(k, v, ctx, cb);
}

صحیح cmd_send_pack(صحیح argc,
		  ثابت نویسه **argv,
		  ثابت نویسه *prefix,
		  ساختار repository *repo)
{
	ساختار refspec rs;
	ثابت نویسه *remote_name = پوچ;
	ساختار remote *remote = پوچ;
	ثابت نویسه *dest = پوچ;
	صحیح fd[2];
	ساختار child_process *conn;
	ساختار oid_array extra_have = OID_ARRAY_INIT;
	ساختار oid_array shallow = OID_ARRAY_INIT;
	ساختار ref *remote_refs, *local_refs;
	صحیح ret;
	صحیح helper_status = 0;
	صحیح send_all = 0;
	صحیح verbose = 0;
	ثابت نویسه *receivepack = "git-receive-pack";
	unsigned dry_run = 0;
	unsigned send_mirror = 0;
	unsigned force_update = 0;
	unsigned quiet = 0;
	صحیح افزودن_cert = 0;
	ساختار string_list افزودن_options = STRING_LIST_INIT_NODUP;
	unsigned use_thin_pack = 0;
	unsigned atomic = 0;
	unsigned stateless_rpc = 0;
	صحیح flags;
	unsigned صحیح reject_reasons;
	صحیح progress = -1;
	صحیح from_stdin = 0;
	ساختار افزودن_cas_option cas = {0};
	صحیح force_if_includes = 0;
	ساختار packet_reader reader;

	ساختار option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_run, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &افزودن_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_افزودن_signed),
		OPT_STRING_LIST(0, "push-option", &افزودن_options,
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
		  PARSE_OPT_OPTARG, parseopt_افزودن_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_END()
	};

	repo_config(repo, send_pack_config, پوچ);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_افزودن(&rs, repo->hash_algo);

	اگر (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	اگر (!dest)
		usage_with_options(send_pack_usage, options);

	args.verbose = verbose;
	args.dry_run = dry_run;
	args.send_mirror = send_mirror;
	args.force_update = force_update;
	args.quiet = quiet;
	args.افزودن_cert = افزودن_cert;
	args.progress = progress;
	args.use_thin_pack = use_thin_pack;
	args.atomic = atomic;
	args.stateless_rpc = stateless_rpc;
	args.افزودن_options = افزودن_options.nr ? &افزودن_options : پوچ;
	args.url = dest;

	اگر (from_stdin) {
		اگر (args.stateless_rpc) {
			ثابت نویسه *buf;
			وقتی ((buf = packet_read_سطر(0, پوچ)))
				refspec_append(&rs, buf);
		} وگرنه {
			ساختار strbuf سطر = STRBUF_INIT;
			وقتی (strbuf_getline(&سطر, stdin) != EOF)
				refspec_append(&rs, سطر.buf);
			strbuf_release(&سطر);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	اگر ((rs.nr > 0 && (send_all || args.send_mirror)) ||
	    (send_all && args.send_mirror))
		usage_with_options(send_pack_usage, options);

	اگر (remote_name) {
		remote = remote_get(remote_name);
		اگر (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	اگر (progress == -1)
		progress = !args.quiet && isatty(2);
	args.progress = progress;

	اگر (args.stateless_rpc) {
		conn = پوچ;
		fd[0] = 0;
		fd[1] = 1;
	} وگرنه {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   args.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], پوچ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	انتخاب (discover_version(&reader)) {
	حالت protocol_v2:
		die("support for protocol v2 not implemented yet");
		بشکن;
	حالت protocol_v1:
	حالت protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		بشکن;
	حالت protocol_unknown_version:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = MATCH_REFS_NONE;

	اگر (send_all)
		flags |= MATCH_REFS_ALL;
	اگر (args.send_mirror)
		flags |= MATCH_REFS_MIRROR;

	/* match them up */
	اگر (match_افزودن_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		برو cleanup;
	}
	اگر (!is_empty_cas(&cas))
		apply_افزودن_cas(&cas, remote, remote_refs);

	اگر (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_افزودن(remote_refs, args.send_mirror,
		args.force_update);

	ret = send_pack(repo, &args, fd, conn, remote_refs, &extra_have);

	اگر (helper_status)
		print_helper_status(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= finish_connect(conn);

	اگر (!helper_status)
		transport_print_افزودن_status(dest, remote_refs, args.verbose, 0, &reject_reasons);

	اگر (!args.dry_run && remote) {
		ساختار ref *ref;
		برای (ref = remote_refs; ref; ref = ref->next)
			transport_update_tracking_ref(remote, ref, args.verbose);
	}

	اگر (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&افزودن_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	بازگشت ret;
}
