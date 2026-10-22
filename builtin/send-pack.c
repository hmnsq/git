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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	ಖಾಲಿ,
};

ಸ್ಥಿರ ರಚನೆ send_pack_ವಾದಗಳು ವಾದಗಳು;

ಸ್ಥಿರ ಶೂನ್ಯ print_helper_ಸ್ಥಿತಿ(ರಚನೆ ref *ref)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ ref_ತಳ್ಳು_report *report;

	ಗಾಗಿ (; ref; ref = ref->ಮುಂದಿನ) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ = ಖಾಲಿ;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *res;
		ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;

		ಆಯ್ಕೆ(ref->ಸ್ಥಿತಿ_2) {
		ಸ್ಥಿತಿ REF_ಸ್ಥಿತಿ_NONE:
			res = "error";
			ಸಂದೇಶ = "no match";
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ REF_ಸ್ಥಿತಿ_OK:
			res = "ok";
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ REF_ಸ್ಥಿತಿ_UPTODATE:
			res = "ok";
			ಸಂದೇಶ = "up to date";
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ REF_ಸ್ಥಿತಿ_REJECT_NONFASTFORWARD:
			res = "error";
			ಸಂದೇಶ = "non-fast forward";
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ REF_ಸ್ಥಿತಿ_REJECT_FETCH_ಮೊದಲ:
			res = "error";
			ಸಂದೇಶ = "fetch first";
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ REF_ಸ್ಥಿತಿ_REJECT_NEEDS_FORCE:
			res = "error";
			ಸಂದೇಶ = "needs force";
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ REF_ಸ್ಥಿತಿ_REJECT_STALE:
			res = "error";
			ಸಂದೇಶ = "stale info";
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ REF_ಸ್ಥಿತಿ_REJECT_REMOTE_UPDATED:
			res = "error";
			ಸಂದೇಶ = "remote ref updated since checkout";
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ REF_ಸ್ಥಿತಿ_REJECT_ALREADY_EXISTS:
			res = "error";
			ಸಂದೇಶ = "already exists";
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ REF_ಸ್ಥಿತಿ_REJECT_NODELETE:
		ಸ್ಥಿತಿ REF_ಸ್ಥಿತಿ_REMOTE_REJECT:
			res = "error";
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ REF_ಸ್ಥಿತಿ_EXPECTING_REPORT:
			res = "error";
			ಸಂದೇಶ = "expecting report";
			ನಿಲ್ಲಿಸು;

		ಪೂರ್ವನಿಯೋಜಿತ:
			ಮುಂದುವರಿ;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		ಆದರೆ (ref->remote_ಸ್ಥಿತಿ)
			ಸಂದೇಶ = ref->remote_ಸ್ಥಿತಿ;
		ಆದರೆ (ಸಂದೇಶ) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", ಸಂದೇಶ, 0);
		}
		strbuf_addch(&buf, '\n');

		ಆದರೆ (ref->ಸ್ಥಿತಿ_2 == REF_ಸ್ಥಿತಿ_OK) {
			ಗಾಗಿ (report = ref->report; report; report = report->ಮುಂದಿನ) {
				ಆದರೆ (ಎಣಿಕೆ_2++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				ಆದರೆ (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				ಆದರೆ (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_ಗಮ್ಯ_hex(report->old_oid));
				ಆದರೆ (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_ಗಮ್ಯ_hex(report->new_oid));
				ಆದರೆ (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ send_pack_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *k, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(k, "push.gpgsign")) {
		ಆಯ್ಕೆ (git_parse_maybe_bool(v)) {
		ಸ್ಥಿತಿ 0:
			ವಾದಗಳು.ತಳ್ಳು_cert = SEND_PACK_ತಳ್ಳು_CERT_NEVER;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 1:
			ವಾದಗಳು.ತಳ್ಳು_cert = SEND_PACK_ತಳ್ಳು_CERT_ALWAYS;
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			ಆದರೆ (!strcasecmp(v, "if-asked"))
				ವಾದಗಳು.ತಳ್ಳು_cert = SEND_PACK_ತಳ್ಳು_CERT_IF_ASKED;
			ಇಲ್ಲದಿದ್ದರೆ
				ಹಿಂತಿರುಗಿ error(_("invalid value for '%s'"), k);
		}
	}
	ಹಿಂತಿರುಗಿ git_default_config(k, v, ctx, cb);
}

ಪೂರ್ಣಾಂಕ cmd_send_pack(ಪೂರ್ಣಾಂಕ argc,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		  ರಚನೆ repository *repo)
{
	ರಚನೆ refspec rs;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote_name = ಖಾಲಿ;
	ರಚನೆ remote *remote = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dest = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ fd[2];
	ರಚನೆ ಮಗು_process *conn;
	ರಚನೆ oid_array extra_have = OID_ARRAY_INIT;
	ರಚನೆ oid_array shallow = OID_ARRAY_INIT;
	ರಚನೆ ref *remote_refs, *local_refs;
	ಪೂರ್ಣಾಂಕ ret;
	ಪೂರ್ಣಾಂಕ helper_ಸ್ಥಿತಿ = 0;
	ಪೂರ್ಣಾಂಕ send_all = 0;
	ಪೂರ್ಣಾಂಕ verbose = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *receivepack = "git-receive-pack";
	ಚಿಹ್ನೆರಹಿತ dry_ಸತತ = 0;
	ಚಿಹ್ನೆರಹಿತ send_mirror = 0;
	ಚಿಹ್ನೆರಹಿತ force_update = 0;
	ಚಿಹ್ನೆರಹಿತ quiet = 0;
	ಪೂರ್ಣಾಂಕ ತಳ್ಳು_cert = 0;
	ರಚನೆ string_list ತಳ್ಳು_options = STRING_LIST_INIT_NODUP;
	ಚಿಹ್ನೆರಹಿತ use_thin_pack = 0;
	ಚಿಹ್ನೆರಹಿತ atomic = 0;
	ಚಿಹ್ನೆರಹಿತ stateless_rpc = 0;
	ಪೂರ್ಣಾಂಕ flags;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ reject_reasons;
	ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ = -1;
	ಪೂರ್ಣಾಂಕ from_stdin = 0;
	ರಚನೆ ತಳ್ಳು_cas_option cas = {0};
	ಪೂರ್ಣಾಂಕ force_if_includes = 0;
	ರಚನೆ packet_reader reader;

	ರಚನೆ option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_ಸತತ, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &ತಳ್ಳು_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_ತಳ್ಳು_signed),
		OPT_STRING_LIST(0, "push-option", &ತಳ್ಳು_options,
				N_("server-specific"),
				N_("option to transmit")),
		OPT_BOOL(0, "progress", &ಪ್ರಗತಿ, N_("force progress reporting")),
		OPT_BOOL(0, "thin", &use_thin_pack, N_("use thin pack")),
		OPT_BOOL(0, "atomic", &atomic, N_("request atomic transaction on remote side")),
		OPT_BOOL(0, "stateless-rpc", &stateless_rpc, N_("use stateless RPC protocol")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read refs from stdin")),
		OPT_BOOL(0, "helper-status", &helper_ಸ್ಥಿತಿ, N_("print status from remote helper")),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
		  N_("require old value of ref to be at this value"),
		  PARSE_OPT_OPTARG, parseopt_ತಳ್ಳು_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_ಕೊನೆ()
	};

	repo_config(repo, send_pack_config, ಖಾಲಿ);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, send_pack_usage, 0);

	refspec_init_ತಳ್ಳು(&rs, repo->hash_algo);

	ಆದರೆ (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	ಆದರೆ (!dest)
		usage_with_options(send_pack_usage, options);

	ವಾದಗಳು.verbose = verbose;
	ವಾದಗಳು.dry_ಸತತ = dry_ಸತತ;
	ವಾದಗಳು.send_mirror = send_mirror;
	ವಾದಗಳು.force_update = force_update;
	ವಾದಗಳು.quiet = quiet;
	ವಾದಗಳು.ತಳ್ಳು_cert = ತಳ್ಳು_cert;
	ವಾದಗಳು.ಪ್ರಗತಿ = ಪ್ರಗತಿ;
	ವಾದಗಳು.use_thin_pack = use_thin_pack;
	ವಾದಗಳು.atomic = atomic;
	ವಾದಗಳು.stateless_rpc = stateless_rpc;
	ವಾದಗಳು.ತಳ್ಳು_options = ತಳ್ಳು_options.nr ? &ತಳ್ಳು_options : ಖಾಲಿ;
	ವಾದಗಳು.url = dest;

	ಆದರೆ (from_stdin) {
		ಆದರೆ (ವಾದಗಳು.stateless_rpc) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf;
			ಆಗ ((buf = packet_read_ಸಾಲು(0, ಖಾಲಿ)))
				refspec_append(&rs, buf);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ strbuf ಸಾಲು = STRBUF_INIT;
			ಆಗ (strbuf_getline(&ಸಾಲು, stdin) != EOF)
				refspec_append(&rs, ಸಾಲು.buf);
			strbuf_release(&ಸಾಲು);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	ಆದರೆ ((rs.nr > 0 && (send_all || ವಾದಗಳು.send_mirror)) ||
	    (send_all && ವಾದಗಳು.send_mirror))
		usage_with_options(send_pack_usage, options);

	ಆದರೆ (remote_name) {
		remote = remote_get(remote_name);
		ಆದರೆ (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	ಆದರೆ (ಪ್ರಗತಿ == -1)
		ಪ್ರಗತಿ = !ವಾದಗಳು.quiet && isatty(2);
	ವಾದಗಳು.ಪ್ರಗತಿ = ಪ್ರಗತಿ;

	ಆದರೆ (ವಾದಗಳು.stateless_rpc) {
		conn = ಖಾಲಿ;
		fd[0] = 0;
		fd[1] = 1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   ವಾದಗಳು.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], ಖಾಲಿ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ಆಯ್ಕೆ (discover_ಆವೃತ್ತಿ(&reader)) {
	ಸ್ಥಿತಿ protocol_v2:
		die("support for protocol v2 not implemented yet");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ protocol_v1:
	ಸ್ಥಿತಿ protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_ಸಾಮಾನ್ಯ,
				 &extra_have, &shallow);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ protocol_unknown_ಆವೃತ್ತಿ:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = ಹೊಂದಿಕೆ_REFS_NONE;

	ಆದರೆ (send_all)
		flags |= ಹೊಂದಿಕೆ_REFS_ALL;
	ಆದರೆ (ವಾದಗಳು.send_mirror)
		flags |= ಹೊಂದಿಕೆ_REFS_MIRROR;

	/* match them up */
	ಆದರೆ (ಹೊಂದಿಕೆ_ತಳ್ಳು_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		ಹೋಗು cleanup;
	}
	ಆದರೆ (!is_ಖಾಲಿಗಳು_cas(&cas))
		apply_ತಳ್ಳು_cas(&cas, remote, remote_refs);

	ಆದರೆ (!is_ಖಾಲಿಗಳು_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	ಗಣ_ref_ಸ್ಥಿತಿ_for_ತಳ್ಳು(remote_refs, ವಾದಗಳು.send_mirror,
		ವಾದಗಳು.force_update);

	ret = send_pack(repo, &ವಾದಗಳು, fd, conn, remote_refs, &extra_have);

	ಆದರೆ (helper_ಸ್ಥಿತಿ)
		print_helper_ಸ್ಥಿತಿ(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= ಮುಕ್ತಾಯ_connect(conn);

	ಆದರೆ (!helper_ಸ್ಥಿತಿ)
		transport_print_ತಳ್ಳು_ಸ್ಥಿತಿ(dest, remote_refs, ವಾದಗಳು.verbose, 0, &reject_reasons);

	ಆದರೆ (!ವಾದಗಳು.dry_ಸತತ && remote) {
		ರಚನೆ ref *ref;
		ಗಾಗಿ (ref = remote_refs; ref; ref = ref->ಮುಂದಿನ)
			transport_update_tracking_ref(remote, ref, ವಾದಗಳು.verbose);
	}

	ಆದರೆ (!ret && !transport_refs_pushed(remote_refs))
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
	ಹಿಂತಿರುಗಿ ret;
}
