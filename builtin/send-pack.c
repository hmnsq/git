#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "pkt-line.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "connect.h"
#ಅನ್ತರ್ಭಾವ "send-pack.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "transport.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "protocol.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	ರಿಕ್ತ,
};

ಸ್ಥಿರ ಸಂರಚನಾ send_pack_args args;

ಸ್ಥಿರ ಶೂನ್ಯ print_helper_status(ಸಂರಚನಾ ref *ref)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ ref_ತಳ್ಳು_report *report;

	ಪ್ರತಿ (; ref; ref = ref->next) {
		ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ = ರಿಕ್ತ;
		ಧ್ರುವ ವರ್ಣ *res;
		ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;

		ವಿಕಲ್ಪ(ref->status) {
		ಪಕ್ಷ REF_STATUS_NONE:
			res = "error";
			ಸಂದೇಶ = "no match";
			ಭಙ್ಗ;

		ಪಕ್ಷ REF_STATUS_OK:
			res = "ok";
			ಭಙ್ಗ;

		ಪಕ್ಷ REF_STATUS_UPTODATE:
			res = "ok";
			ಸಂದೇಶ = "up to date";
			ಭಙ್ಗ;

		ಪಕ್ಷ REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			ಸಂದೇಶ = "non-fast forward";
			ಭಙ್ಗ;

		ಪಕ್ಷ REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			ಸಂದೇಶ = "fetch first";
			ಭಙ್ಗ;

		ಪಕ್ಷ REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			ಸಂದೇಶ = "needs force";
			ಭಙ್ಗ;

		ಪಕ್ಷ REF_STATUS_REJECT_STALE:
			res = "error";
			ಸಂದೇಶ = "stale info";
			ಭಙ್ಗ;

		ಪಕ್ಷ REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			ಸಂದೇಶ = "remote ref updated since checkout";
			ಭಙ್ಗ;

		ಪಕ್ಷ REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			ಸಂದೇಶ = "already exists";
			ಭಙ್ಗ;

		ಪಕ್ಷ REF_STATUS_REJECT_NODELETE:
		ಪಕ್ಷ REF_STATUS_REMOTE_REJECT:
			res = "error";
			ಭಙ್ಗ;

		ಪಕ್ಷ REF_STATUS_EXPECTING_REPORT:
			res = "error";
			ಸಂದೇಶ = "expecting report";
			ಭಙ್ಗ;

		ಸ್ವಭಾವ:
			ಅನುವರ್ತ;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		ಯದಿ (ref->remote_status)
			ಸಂದೇಶ = ref->remote_status;
		ಯದಿ (ಸಂದೇಶ) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", ಸಂದೇಶ, 0);
		}
		strbuf_addch(&buf, '\n');

		ಯದಿ (ref->status == REF_STATUS_OK) {
			ಪ್ರತಿ (report = ref->report; report; report = report->next) {
				ಯದಿ (ಎಣಿಕೆ++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				ಯದಿ (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				ಯದಿ (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_to_hex(report->old_oid));
				ಯದಿ (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_to_hex(report->new_oid));
				ಯದಿ (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ send_pack_config(ಧ್ರುವ ವರ್ಣ *k, ಧ್ರುವ ವರ್ಣ *v,
			    ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(k, "push.gpgsign")) {
		ವಿಕಲ್ಪ (git_parse_maybe_bool(v)) {
		ಪಕ್ಷ 0:
			args.ತಳ್ಳು_cert = SEND_PACK_ತಳ್ಳು_CERT_NEVER;
			ಭಙ್ಗ;
		ಪಕ್ಷ 1:
			args.ತಳ್ಳು_cert = SEND_PACK_ತಳ್ಳು_CERT_ALWAYS;
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			ಯದಿ (!strcasecmp(v, "if-asked"))
				args.ತಳ್ಳು_cert = SEND_PACK_ತಳ್ಳು_CERT_IF_ASKED;
			ಅನ್ಯಥಾ
				ಪ್ರತ್ಯಾಗಮ error(_("invalid value for '%s'"), k);
		}
	}
	ಪ್ರತ್ಯಾಗಮ git_default_config(k, v, ctx, cb);
}

ಪೂರ್ಣಾಙ್ಕ cmd_send_pack(ಪೂರ್ಣಾಙ್ಕ argc,
		  ಧ್ರುವ ವರ್ಣ **argv,
		  ಧ್ರುವ ವರ್ಣ *prefix,
		  ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ refspec rs;
	ಧ್ರುವ ವರ್ಣ *remote_name = ರಿಕ್ತ;
	ಸಂರಚನಾ remote *remote = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *dest = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ fd[2];
	ಸಂರಚನಾ child_process *conn;
	ಸಂರಚನಾ oid_array extra_have = OID_ARRAY_INIT;
	ಸಂರಚನಾ oid_array shallow = OID_ARRAY_INIT;
	ಸಂರಚನಾ ref *remote_refs, *local_refs;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಪೂರ್ಣಾಙ್ಕ helper_status = 0;
	ಪೂರ್ಣಾಙ್ಕ send_all = 0;
	ಪೂರ್ಣಾಙ್ಕ verbose = 0;
	ಧ್ರುವ ವರ್ಣ *receivepack = "git-receive-pack";
	unsigned dry_run = 0;
	unsigned send_mirror = 0;
	unsigned force_update = 0;
	unsigned quiet = 0;
	ಪೂರ್ಣಾಙ್ಕ ತಳ್ಳು_cert = 0;
	ಸಂರಚನಾ string_list ತಳ್ಳು_options = STRING_LIST_INIT_NODUP;
	unsigned use_thin_pack = 0;
	unsigned atomic = 0;
	unsigned stateless_rpc = 0;
	ಪೂರ್ಣಾಙ್ಕ flags;
	unsigned ಪೂರ್ಣಾಙ್ಕ reject_reasons;
	ಪೂರ್ಣಾಙ್ಕ progress = -1;
	ಪೂರ್ಣಾಙ್ಕ from_stdin = 0;
	ಸಂರಚನಾ ತಳ್ಳು_cas_option cas = {0};
	ಪೂರ್ಣಾಙ್ಕ force_if_includes = 0;
	ಸಂರಚನಾ packet_reader reader;

	ಸಂರಚನಾ option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_run, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &ತಳ್ಳು_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_ತಳ್ಳು_signed),
		OPT_STRING_LIST(0, "push-option", &ತಳ್ಳು_options,
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
		  PARSE_OPT_OPTARG, parseopt_ತಳ್ಳು_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_END()
	};

	repo_config(repo, send_pack_config, ರಿಕ್ತ);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_ತಳ್ಳು(&rs, repo->hash_algo);

	ಯದಿ (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	ಯದಿ (!dest)
		usage_with_options(send_pack_usage, options);

	args.verbose = verbose;
	args.dry_run = dry_run;
	args.send_mirror = send_mirror;
	args.force_update = force_update;
	args.quiet = quiet;
	args.ತಳ್ಳು_cert = ತಳ್ಳು_cert;
	args.progress = progress;
	args.use_thin_pack = use_thin_pack;
	args.atomic = atomic;
	args.stateless_rpc = stateless_rpc;
	args.ತಳ್ಳು_options = ತಳ್ಳು_options.nr ? &ತಳ್ಳು_options : ರಿಕ್ತ;
	args.url = dest;

	ಯದಿ (from_stdin) {
		ಯದಿ (args.stateless_rpc) {
			ಧ್ರುವ ವರ್ಣ *buf;
			ಯಾವತ್ ((buf = packet_read_ಸಾಲು(0, ರಿಕ್ತ)))
				refspec_append(&rs, buf);
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ strbuf ಸಾಲು = STRBUF_INIT;
			ಯಾವತ್ (strbuf_getline(&ಸಾಲು, stdin) != EOF)
				refspec_append(&rs, ಸಾಲು.buf);
			strbuf_release(&ಸಾಲು);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	ಯದಿ ((rs.nr > 0 && (send_all || args.send_mirror)) ||
	    (send_all && args.send_mirror))
		usage_with_options(send_pack_usage, options);

	ಯದಿ (remote_name) {
		remote = remote_get(remote_name);
		ಯದಿ (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	ಯದಿ (progress == -1)
		progress = !args.quiet && isatty(2);
	args.progress = progress;

	ಯದಿ (args.stateless_rpc) {
		conn = ರಿಕ್ತ;
		fd[0] = 0;
		fd[1] = 1;
	} ಅನ್ಯಥಾ {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   args.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], ರಿಕ್ತ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ವಿಕಲ್ಪ (discover_version(&reader)) {
	ಪಕ್ಷ protocol_v2:
		die("support for protocol v2 not implemented yet");
		ಭಙ್ಗ;
	ಪಕ್ಷ protocol_v1:
	ಪಕ್ಷ protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		ಭಙ್ಗ;
	ಪಕ್ಷ protocol_unknown_version:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = MATCH_REFS_NONE;

	ಯದಿ (send_all)
		flags |= MATCH_REFS_ALL;
	ಯದಿ (args.send_mirror)
		flags |= MATCH_REFS_MIRROR;

	/* match them up */
	ಯದಿ (match_ತಳ್ಳು_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		ಗಚ್ಛ cleanup;
	}
	ಯದಿ (!is_empty_cas(&cas))
		apply_ತಳ್ಳು_cas(&cas, remote, remote_refs);

	ಯದಿ (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_ತಳ್ಳು(remote_refs, args.send_mirror,
		args.force_update);

	ret = send_pack(repo, &args, fd, conn, remote_refs, &extra_have);

	ಯದಿ (helper_status)
		print_helper_status(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= finish_connect(conn);

	ಯದಿ (!helper_status)
		transport_print_ತಳ್ಳು_status(dest, remote_refs, args.verbose, 0, &reject_reasons);

	ಯದಿ (!args.dry_run && remote) {
		ಸಂರಚನಾ ref *ref;
		ಪ್ರತಿ (ref = remote_refs; ref; ref = ref->next)
			transport_update_tracking_ref(remote, ref, args.verbose);
	}

	ಯದಿ (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&ತಳ್ಳು_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	ಪ್ರತ್ಯಾಗಮ ret;
}
