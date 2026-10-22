#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pkt-line.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "connect.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "send-pack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "transport.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "protocol.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	ਨਿਰਾ,
};

ਸਥਿਰ ਬਣਤਰ send_pack_ਦਲੀਲਾਂ ਦਲੀਲਾਂ;

ਸਥਿਰ ਖਾਲੀ print_helper_ਸਥਿਤੀ(ਬਣਤਰ ref *ref)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ ref_ਧੱਕੋ_report *report;

	ਲਈ (; ref; ref = ref->ਅਗਲਾ) {
		ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ = ਨਿਰਾ;
		ਅਚਲ ਅੱਖਰ *res;
		ਪੂਰਨ ਗਿਣਤੀ_2 = 0;

		ਚੋਣ(ref->ਸਥਿਤੀ) {
		ਹਾਲਤ REF_ਸਥਿਤੀ_NONE:
			res = "error";
			ਸੁਨੇਹਾ = "no match";
			ਰੋਕੋ;

		ਹਾਲਤ REF_ਸਥਿਤੀ_OK:
			res = "ok";
			ਰੋਕੋ;

		ਹਾਲਤ REF_ਸਥਿਤੀ_UPTODATE:
			res = "ok";
			ਸੁਨੇਹਾ = "up to date";
			ਰੋਕੋ;

		ਹਾਲਤ REF_ਸਥਿਤੀ_REJECT_NONFASTFORWARD:
			res = "error";
			ਸੁਨੇਹਾ = "non-fast forward";
			ਰੋਕੋ;

		ਹਾਲਤ REF_ਸਥਿਤੀ_REJECT_FETCH_ਪਹਿਲਾ:
			res = "error";
			ਸੁਨੇਹਾ = "fetch first";
			ਰੋਕੋ;

		ਹਾਲਤ REF_ਸਥਿਤੀ_REJECT_NEEDS_FORCE:
			res = "error";
			ਸੁਨੇਹਾ = "needs force";
			ਰੋਕੋ;

		ਹਾਲਤ REF_ਸਥਿਤੀ_REJECT_STALE:
			res = "error";
			ਸੁਨੇਹਾ = "stale info";
			ਰੋਕੋ;

		ਹਾਲਤ REF_ਸਥਿਤੀ_REJECT_REMOTE_UPDATED:
			res = "error";
			ਸੁਨੇਹਾ = "remote ref updated since checkout";
			ਰੋਕੋ;

		ਹਾਲਤ REF_ਸਥਿਤੀ_REJECT_ALREADY_EXISTS:
			res = "error";
			ਸੁਨੇਹਾ = "already exists";
			ਰੋਕੋ;

		ਹਾਲਤ REF_ਸਥਿਤੀ_REJECT_NODELETE:
		ਹਾਲਤ REF_ਸਥਿਤੀ_REMOTE_REJECT:
			res = "error";
			ਰੋਕੋ;

		ਹਾਲਤ REF_ਸਥਿਤੀ_EXPECTING_REPORT:
			res = "error";
			ਸੁਨੇਹਾ = "expecting report";
			ਰੋਕੋ;

		ਮੂਲ:
			ਜਾਰੀ;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		ਜੇ (ref->remote_ਸਥਿਤੀ)
			ਸੁਨੇਹਾ = ref->remote_ਸਥਿਤੀ;
		ਜੇ (ਸੁਨੇਹਾ) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", ਸੁਨੇਹਾ, 0);
		}
		strbuf_addch(&buf, '\n');

		ਜੇ (ref->ਸਥਿਤੀ == REF_ਸਥਿਤੀ_OK) {
			ਲਈ (report = ref->report; report; report = report->ਅਗਲਾ) {
				ਜੇ (ਗਿਣਤੀ_2++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				ਜੇ (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				ਜੇ (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_ਮੰਜ਼ਿਲ_hex(report->old_oid));
				ਜੇ (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_ਮੰਜ਼ਿਲ_hex(report->new_oid));
				ਜੇ (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

ਸਥਿਰ ਪੂਰਨ send_pack_config(ਅਚਲ ਅੱਖਰ *k, ਅਚਲ ਅੱਖਰ *v,
			    ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(k, "push.gpgsign")) {
		ਚੋਣ (git_parse_maybe_bool(v)) {
		ਹਾਲਤ 0:
			ਦਲੀਲਾਂ.ਧੱਕੋ_cert = SEND_PACK_ਧੱਕੋ_CERT_NEVER;
			ਰੋਕੋ;
		ਹਾਲਤ 1:
			ਦਲੀਲਾਂ.ਧੱਕੋ_cert = SEND_PACK_ਧੱਕੋ_CERT_ALWAYS;
			ਰੋਕੋ;
		ਮੂਲ:
			ਜੇ (!strcasecmp(v, "if-asked"))
				ਦਲੀਲਾਂ.ਧੱਕੋ_cert = SEND_PACK_ਧੱਕੋ_CERT_IF_ASKED;
			ਨਹੀਂਤਾਂ
				ਵਾਪਸ error(_("invalid value for '%s'"), k);
		}
	}
	ਵਾਪਸ git_default_config(k, v, ctx, cb);
}

ਪੂਰਨ cmd_send_pack(ਪੂਰਨ argc,
		  ਅਚਲ ਅੱਖਰ **argv,
		  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		  ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ refspec rs;
	ਅਚਲ ਅੱਖਰ *remote_name = ਨਿਰਾ;
	ਬਣਤਰ remote *remote = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *dest = ਨਿਰਾ;
	ਪੂਰਨ fd[2];
	ਬਣਤਰ ਬੱਚਾ_process *conn;
	ਬਣਤਰ oid_array extra_have = OID_ARRAY_INIT;
	ਬਣਤਰ oid_array shallow = OID_ARRAY_INIT;
	ਬਣਤਰ ref *remote_refs, *local_refs;
	ਪੂਰਨ ret;
	ਪੂਰਨ helper_ਸਥਿਤੀ = 0;
	ਪੂਰਨ send_all = 0;
	ਪੂਰਨ verbose = 0;
	ਅਚਲ ਅੱਖਰ *receivepack = "git-receive-pack";
	ਬਿਨਾਨਿਸ਼ਾਨੀ dry_ਲੜੀਵਾਰ = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ send_mirror = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ force_update = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ quiet = 0;
	ਪੂਰਨ ਧੱਕੋ_cert = 0;
	ਬਣਤਰ string_list ਧੱਕੋ_options = STRING_LIST_INIT_NODUP;
	ਬਿਨਾਨਿਸ਼ਾਨੀ use_thin_pack = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ atomic = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ stateless_rpc = 0;
	ਪੂਰਨ flags;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ reject_reasons;
	ਪੂਰਨ ਤਰੱਕੀ = -1;
	ਪੂਰਨ from_stdin = 0;
	ਬਣਤਰ ਧੱਕੋ_cas_option cas = {0};
	ਪੂਰਨ force_if_includes = 0;
	ਬਣਤਰ packet_reader reader;

	ਬਣਤਰ option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_ਲੜੀਵਾਰ, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &ਧੱਕੋ_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_ਧੱਕੋ_signed),
		OPT_STRING_LIST(0, "push-option", &ਧੱਕੋ_options,
				N_("server-specific"),
				N_("option to transmit")),
		OPT_BOOL(0, "progress", &ਤਰੱਕੀ, N_("force progress reporting")),
		OPT_BOOL(0, "thin", &use_thin_pack, N_("use thin pack")),
		OPT_BOOL(0, "atomic", &atomic, N_("request atomic transaction on remote side")),
		OPT_BOOL(0, "stateless-rpc", &stateless_rpc, N_("use stateless RPC protocol")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read refs from stdin")),
		OPT_BOOL(0, "helper-status", &helper_ਸਥਿਤੀ, N_("print status from remote helper")),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
		  N_("require old value of ref to be at this value"),
		  PARSE_OPT_OPTARG, parseopt_ਧੱਕੋ_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_ਸਿਰਾ()
	};

	repo_config(repo, send_pack_config, ਨਿਰਾ);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, send_pack_usage, 0);

	refspec_init_ਧੱਕੋ(&rs, repo->hash_algo);

	ਜੇ (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	ਜੇ (!dest)
		usage_with_options(send_pack_usage, options);

	ਦਲੀਲਾਂ.verbose = verbose;
	ਦਲੀਲਾਂ.dry_ਲੜੀਵਾਰ = dry_ਲੜੀਵਾਰ;
	ਦਲੀਲਾਂ.send_mirror = send_mirror;
	ਦਲੀਲਾਂ.force_update = force_update;
	ਦਲੀਲਾਂ.quiet = quiet;
	ਦਲੀਲਾਂ.ਧੱਕੋ_cert = ਧੱਕੋ_cert;
	ਦਲੀਲਾਂ.ਤਰੱਕੀ = ਤਰੱਕੀ;
	ਦਲੀਲਾਂ.use_thin_pack = use_thin_pack;
	ਦਲੀਲਾਂ.atomic = atomic;
	ਦਲੀਲਾਂ.stateless_rpc = stateless_rpc;
	ਦਲੀਲਾਂ.ਧੱਕੋ_options = ਧੱਕੋ_options.nr ? &ਧੱਕੋ_options : ਨਿਰਾ;
	ਦਲੀਲਾਂ.url = dest;

	ਜੇ (from_stdin) {
		ਜੇ (ਦਲੀਲਾਂ.stateless_rpc) {
			ਅਚਲ ਅੱਖਰ *buf;
			ਜਦੋਂ ((buf = packet_read_ਸਤਰ(0, ਨਿਰਾ)))
				refspec_append(&rs, buf);
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;
			ਜਦੋਂ (strbuf_getline(&ਸਤਰ, stdin) != EOF)
				refspec_append(&rs, ਸਤਰ.buf);
			strbuf_release(&ਸਤਰ);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	ਜੇ ((rs.nr > 0 && (send_all || ਦਲੀਲਾਂ.send_mirror)) ||
	    (send_all && ਦਲੀਲਾਂ.send_mirror))
		usage_with_options(send_pack_usage, options);

	ਜੇ (remote_name) {
		remote = remote_get(remote_name);
		ਜੇ (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	ਜੇ (ਤਰੱਕੀ == -1)
		ਤਰੱਕੀ = !ਦਲੀਲਾਂ.quiet && isatty(2);
	ਦਲੀਲਾਂ.ਤਰੱਕੀ = ਤਰੱਕੀ;

	ਜੇ (ਦਲੀਲਾਂ.stateless_rpc) {
		conn = ਨਿਰਾ;
		fd[0] = 0;
		fd[1] = 1;
	} ਨਹੀਂਤਾਂ {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   ਦਲੀਲਾਂ.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], ਨਿਰਾ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ਚੋਣ (discover_ਸੰਸਕਰਣ(&reader)) {
	ਹਾਲਤ protocol_v2:
		die("support for protocol v2 not implemented yet");
		ਰੋਕੋ;
	ਹਾਲਤ protocol_v1:
	ਹਾਲਤ protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_ਸਾਧਾਰਨ,
				 &extra_have, &shallow);
		ਰੋਕੋ;
	ਹਾਲਤ protocol_unknown_ਸੰਸਕਰਣ:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = ਮੇਲ_REFS_NONE;

	ਜੇ (send_all)
		flags |= ਮੇਲ_REFS_ALL;
	ਜੇ (ਦਲੀਲਾਂ.send_mirror)
		flags |= ਮੇਲ_REFS_MIRROR;

	/* match them up */
	ਜੇ (ਮੇਲ_ਧੱਕੋ_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		ਜਾਓ cleanup;
	}
	ਜੇ (!is_ਖਾਲੀ_ਥਾਵਾਂ_cas(&cas))
		apply_ਧੱਕੋ_cas(&cas, remote, remote_refs);

	ਜੇ (!is_ਖਾਲੀ_ਥਾਵਾਂ_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	ਸੈੱਟ_ref_ਸਥਿਤੀ_for_ਧੱਕੋ(remote_refs, ਦਲੀਲਾਂ.send_mirror,
		ਦਲੀਲਾਂ.force_update);

	ret = send_pack(repo, &ਦਲੀਲਾਂ, fd, conn, remote_refs, &extra_have);

	ਜੇ (helper_ਸਥਿਤੀ)
		print_helper_ਸਥਿਤੀ(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= ਅੰਤ_connect(conn);

	ਜੇ (!helper_ਸਥਿਤੀ)
		transport_print_ਧੱਕੋ_ਸਥਿਤੀ(dest, remote_refs, ਦਲੀਲਾਂ.verbose, 0, &reject_reasons);

	ਜੇ (!ਦਲੀਲਾਂ.dry_ਲੜੀਵਾਰ && remote) {
		ਬਣਤਰ ref *ref;
		ਲਈ (ref = remote_refs; ref; ref = ref->ਅਗਲਾ)
			transport_update_tracking_ref(remote, ref, ਦਲੀਲਾਂ.verbose);
	}

	ਜੇ (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&ਧੱਕੋ_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	ਵਾਪਸ ret;
}
