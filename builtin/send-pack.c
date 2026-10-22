#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "pkt-line.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "connect.h"
#અન્તર્ભાવ "send-pack.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "transport.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "protocol.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "write-or-die.h"

સ્થિર અચલ અક્ષર * અચલ send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	ખાલી,
};

સ્થિર સંરચના send_pack_args args;

સ્થિર શૂન્ય print_helper_status(સંરચના ref *ref)
{
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના ref_ધકેલો_report *report;

	માટે (; ref; ref = ref->next) {
		અચલ અક્ષર *સંદેશ = ખાલી;
		અચલ અક્ષર *res;
		પૂર્ણાંક ગણતરી = 0;

		પસંદગી(ref->status) {
		સ્થિતિ REF_STATUS_NONE:
			res = "error";
			સંદેશ = "no match";
			તોડો;

		સ્થિતિ REF_STATUS_OK:
			res = "ok";
			તોડો;

		સ્થિતિ REF_STATUS_UPTODATE:
			res = "ok";
			સંદેશ = "up to date";
			તોડો;

		સ્થિતિ REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			સંદેશ = "non-fast forward";
			તોડો;

		સ્થિતિ REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			સંદેશ = "fetch first";
			તોડો;

		સ્થિતિ REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			સંદેશ = "needs force";
			તોડો;

		સ્થિતિ REF_STATUS_REJECT_STALE:
			res = "error";
			સંદેશ = "stale info";
			તોડો;

		સ્થિતિ REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			સંદેશ = "remote ref updated since checkout";
			તોડો;

		સ્થિતિ REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			સંદેશ = "already exists";
			તોડો;

		સ્થિતિ REF_STATUS_REJECT_NODELETE:
		સ્થિતિ REF_STATUS_REMOTE_REJECT:
			res = "error";
			તોડો;

		સ્થિતિ REF_STATUS_EXPECTING_REPORT:
			res = "error";
			સંદેશ = "expecting report";
			તોડો;

		મૂળભૂત:
			ચાલુ;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		જો (ref->remote_status)
			સંદેશ = ref->remote_status;
		જો (સંદેશ) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", સંદેશ, 0);
		}
		strbuf_addch(&buf, '\n');

		જો (ref->status == REF_STATUS_OK) {
			માટે (report = ref->report; report; report = report->next) {
				જો (ગણતરી++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				જો (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				જો (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_to_hex(report->old_oid));
				જો (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_to_hex(report->new_oid));
				જો (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

સ્થિર પૂર્ણાંક send_pack_config(અચલ અક્ષર *k, અચલ અક્ષર *v,
			    અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(k, "push.gpgsign")) {
		પસંદગી (git_parse_maybe_bool(v)) {
		સ્થિતિ 0:
			args.ધકેલો_cert = SEND_PACK_ધકેલો_CERT_NEVER;
			તોડો;
		સ્થિતિ 1:
			args.ધકેલો_cert = SEND_PACK_ધકેલો_CERT_ALWAYS;
			તોડો;
		મૂળભૂત:
			જો (!strcasecmp(v, "if-asked"))
				args.ધકેલો_cert = SEND_PACK_ધકેલો_CERT_IF_ASKED;
			નહીંતર
				પાછા error(_("invalid value for '%s'"), k);
		}
	}
	પાછા git_default_config(k, v, ctx, cb);
}

પૂર્ણાંક cmd_send_pack(પૂર્ણાંક argc,
		  અચલ અક્ષર **argv,
		  અચલ અક્ષર *prefix,
		  સંરચના repository *repo)
{
	સંરચના refspec rs;
	અચલ અક્ષર *remote_name = ખાલી;
	સંરચના remote *remote = ખાલી;
	અચલ અક્ષર *dest = ખાલી;
	પૂર્ણાંક fd[2];
	સંરચના child_process *conn;
	સંરચના oid_array extra_have = OID_ARRAY_INIT;
	સંરચના oid_array shallow = OID_ARRAY_INIT;
	સંરચના ref *remote_refs, *local_refs;
	પૂર્ણાંક ret;
	પૂર્ણાંક helper_status = 0;
	પૂર્ણાંક send_all = 0;
	પૂર્ણાંક verbose = 0;
	અચલ અક્ષર *receivepack = "git-receive-pack";
	unsigned dry_run = 0;
	unsigned send_mirror = 0;
	unsigned force_update = 0;
	unsigned quiet = 0;
	પૂર્ણાંક ધકેલો_cert = 0;
	સંરચના string_list ધકેલો_options = STRING_LIST_INIT_NODUP;
	unsigned use_thin_pack = 0;
	unsigned atomic = 0;
	unsigned stateless_rpc = 0;
	પૂર્ણાંક flags;
	unsigned પૂર્ણાંક reject_reasons;
	પૂર્ણાંક progress = -1;
	પૂર્ણાંક from_stdin = 0;
	સંરચના ધકેલો_cas_option cas = {0};
	પૂર્ણાંક force_if_includes = 0;
	સંરચના packet_reader reader;

	સંરચના option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_run, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &ધકેલો_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_ધકેલો_signed),
		OPT_STRING_LIST(0, "push-option", &ધકેલો_options,
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
		  PARSE_OPT_OPTARG, parseopt_ધકેલો_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_END()
	};

	repo_config(repo, send_pack_config, ખાલી);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_ધકેલો(&rs, repo->hash_algo);

	જો (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	જો (!dest)
		usage_with_options(send_pack_usage, options);

	args.verbose = verbose;
	args.dry_run = dry_run;
	args.send_mirror = send_mirror;
	args.force_update = force_update;
	args.quiet = quiet;
	args.ધકેલો_cert = ધકેલો_cert;
	args.progress = progress;
	args.use_thin_pack = use_thin_pack;
	args.atomic = atomic;
	args.stateless_rpc = stateless_rpc;
	args.ધકેલો_options = ધકેલો_options.nr ? &ધકેલો_options : ખાલી;
	args.url = dest;

	જો (from_stdin) {
		જો (args.stateless_rpc) {
			અચલ અક્ષર *buf;
			જ્યારે ((buf = packet_read_લીટી(0, ખાલી)))
				refspec_append(&rs, buf);
		} નહીંતર {
			સંરચના strbuf લીટી = STRBUF_INIT;
			જ્યારે (strbuf_getline(&લીટી, stdin) != EOF)
				refspec_append(&rs, લીટી.buf);
			strbuf_release(&લીટી);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	જો ((rs.nr > 0 && (send_all || args.send_mirror)) ||
	    (send_all && args.send_mirror))
		usage_with_options(send_pack_usage, options);

	જો (remote_name) {
		remote = remote_get(remote_name);
		જો (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	જો (progress == -1)
		progress = !args.quiet && isatty(2);
	args.progress = progress;

	જો (args.stateless_rpc) {
		conn = ખાલી;
		fd[0] = 0;
		fd[1] = 1;
	} નહીંતર {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   args.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], ખાલી, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	પસંદગી (discover_version(&reader)) {
	સ્થિતિ protocol_v2:
		die("support for protocol v2 not implemented yet");
		તોડો;
	સ્થિતિ protocol_v1:
	સ્થિતિ protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		તોડો;
	સ્થિતિ protocol_unknown_version:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = MATCH_REFS_NONE;

	જો (send_all)
		flags |= MATCH_REFS_ALL;
	જો (args.send_mirror)
		flags |= MATCH_REFS_MIRROR;

	/* match them up */
	જો (match_ધકેલો_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		જાઓ cleanup;
	}
	જો (!is_empty_cas(&cas))
		apply_ધકેલો_cas(&cas, remote, remote_refs);

	જો (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_ધકેલો(remote_refs, args.send_mirror,
		args.force_update);

	ret = send_pack(repo, &args, fd, conn, remote_refs, &extra_have);

	જો (helper_status)
		print_helper_status(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= finish_connect(conn);

	જો (!helper_status)
		transport_print_ધકેલો_status(dest, remote_refs, args.verbose, 0, &reject_reasons);

	જો (!args.dry_run && remote) {
		સંરચના ref *ref;
		માટે (ref = remote_refs; ref; ref = ref->next)
			transport_update_tracking_ref(remote, ref, args.verbose);
	}

	જો (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&ધકેલો_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	પાછા ret;
}
