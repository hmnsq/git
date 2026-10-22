#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "pkt-line.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "connect.h"
#அந்தர்பாவ "send-pack.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "transport.h"
#அந்தர்பாவ "oid-array.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "protocol.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "write-or-die.h"

நிலையான மாறிலி எழுத்து * மாறிலி send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	வெற்று,
};

நிலையான கட்டமைப்பு send_pack_args args;

நிலையான வெறுமை print_helper_status(கட்டமைப்பு ref *ref)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு ref_தள்ளு_report *report;

	க்கு (; ref; ref = ref->next) {
		மாறிலி எழுத்து *செய்தி = வெற்று;
		மாறிலி எழுத்து *res;
		முழு மொத்தம் = 0;

		தேர்வு(ref->status) {
		நிலை REF_STATUS_NONE:
			res = "error";
			செய்தி = "no match";
			நிறுத்து;

		நிலை REF_STATUS_OK:
			res = "ok";
			நிறுத்து;

		நிலை REF_STATUS_UPTODATE:
			res = "ok";
			செய்தி = "up to date";
			நிறுத்து;

		நிலை REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			செய்தி = "non-fast forward";
			நிறுத்து;

		நிலை REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			செய்தி = "fetch first";
			நிறுத்து;

		நிலை REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			செய்தி = "needs force";
			நிறுத்து;

		நிலை REF_STATUS_REJECT_STALE:
			res = "error";
			செய்தி = "stale info";
			நிறுத்து;

		நிலை REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			செய்தி = "remote ref updated since checkout";
			நிறுத்து;

		நிலை REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			செய்தி = "already exists";
			நிறுத்து;

		நிலை REF_STATUS_REJECT_NODELETE:
		நிலை REF_STATUS_REMOTE_REJECT:
			res = "error";
			நிறுத்து;

		நிலை REF_STATUS_EXPECTING_REPORT:
			res = "error";
			செய்தி = "expecting report";
			நிறுத்து;

		இயல்பு:
			தொடர்;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		எனில் (ref->remote_status)
			செய்தி = ref->remote_status;
		எனில் (செய்தி) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", செய்தி, 0);
		}
		strbuf_addch(&buf, '\n');

		எனில் (ref->status == REF_STATUS_OK) {
			க்கு (report = ref->report; report; report = report->next) {
				எனில் (மொத்தம்++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				எனில் (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				எனில் (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_to_hex(report->old_oid));
				எனில் (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_to_hex(report->new_oid));
				எனில் (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

நிலையான முழு send_pack_config(மாறிலி எழுத்து *k, மாறிலி எழுத்து *v,
			    மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(k, "push.gpgsign")) {
		தேர்வு (git_parse_maybe_bool(v)) {
		நிலை 0:
			args.தள்ளு_cert = SEND_PACK_தள்ளு_CERT_NEVER;
			நிறுத்து;
		நிலை 1:
			args.தள்ளு_cert = SEND_PACK_தள்ளு_CERT_ALWAYS;
			நிறுத்து;
		இயல்பு:
			எனில் (!strcasecmp(v, "if-asked"))
				args.தள்ளு_cert = SEND_PACK_தள்ளு_CERT_IF_ASKED;
			இல்லையெனில்
				திரும்பு error(_("invalid value for '%s'"), k);
		}
	}
	திரும்பு git_default_config(k, v, ctx, cb);
}

முழு cmd_send_pack(முழு argc,
		  மாறிலி எழுத்து **argv,
		  மாறிலி எழுத்து *prefix,
		  கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு refspec rs;
	மாறிலி எழுத்து *remote_name = வெற்று;
	கட்டமைப்பு remote *remote = வெற்று;
	மாறிலி எழுத்து *dest = வெற்று;
	முழு fd[2];
	கட்டமைப்பு child_process *conn;
	கட்டமைப்பு oid_array extra_have = OID_ARRAY_INIT;
	கட்டமைப்பு oid_array shallow = OID_ARRAY_INIT;
	கட்டமைப்பு ref *remote_refs, *local_refs;
	முழு ret;
	முழு helper_status = 0;
	முழு send_all = 0;
	முழு verbose = 0;
	மாறிலி எழுத்து *receivepack = "git-receive-pack";
	unsigned dry_run = 0;
	unsigned send_mirror = 0;
	unsigned force_update = 0;
	unsigned quiet = 0;
	முழு தள்ளு_cert = 0;
	கட்டமைப்பு string_list தள்ளு_options = STRING_LIST_INIT_NODUP;
	unsigned use_thin_pack = 0;
	unsigned atomic = 0;
	unsigned stateless_rpc = 0;
	முழு flags;
	unsigned முழு reject_reasons;
	முழு progress = -1;
	முழு from_stdin = 0;
	கட்டமைப்பு தள்ளு_cas_option cas = {0};
	முழு force_if_includes = 0;
	கட்டமைப்பு packet_reader reader;

	கட்டமைப்பு option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_run, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &தள்ளு_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_தள்ளு_signed),
		OPT_STRING_LIST(0, "push-option", &தள்ளு_options,
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
		  PARSE_OPT_OPTARG, parseopt_தள்ளு_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_END()
	};

	repo_config(repo, send_pack_config, வெற்று);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_தள்ளு(&rs, repo->hash_algo);

	எனில் (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	எனில் (!dest)
		usage_with_options(send_pack_usage, options);

	args.verbose = verbose;
	args.dry_run = dry_run;
	args.send_mirror = send_mirror;
	args.force_update = force_update;
	args.quiet = quiet;
	args.தள்ளு_cert = தள்ளு_cert;
	args.progress = progress;
	args.use_thin_pack = use_thin_pack;
	args.atomic = atomic;
	args.stateless_rpc = stateless_rpc;
	args.தள்ளு_options = தள்ளு_options.nr ? &தள்ளு_options : வெற்று;
	args.url = dest;

	எனில் (from_stdin) {
		எனில் (args.stateless_rpc) {
			மாறிலி எழுத்து *buf;
			போது ((buf = packet_read_வரி(0, வெற்று)))
				refspec_append(&rs, buf);
		} இல்லையெனில் {
			கட்டமைப்பு strbuf வரி = STRBUF_INIT;
			போது (strbuf_getline(&வரி, stdin) != EOF)
				refspec_append(&rs, வரி.buf);
			strbuf_release(&வரி);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	எனில் ((rs.nr > 0 && (send_all || args.send_mirror)) ||
	    (send_all && args.send_mirror))
		usage_with_options(send_pack_usage, options);

	எனில் (remote_name) {
		remote = remote_get(remote_name);
		எனில் (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	எனில் (progress == -1)
		progress = !args.quiet && isatty(2);
	args.progress = progress;

	எனில் (args.stateless_rpc) {
		conn = வெற்று;
		fd[0] = 0;
		fd[1] = 1;
	} இல்லையெனில் {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   args.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], வெற்று, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	தேர்வு (discover_version(&reader)) {
	நிலை protocol_v2:
		die("support for protocol v2 not implemented yet");
		நிறுத்து;
	நிலை protocol_v1:
	நிலை protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		நிறுத்து;
	நிலை protocol_unknown_version:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = MATCH_REFS_NONE;

	எனில் (send_all)
		flags |= MATCH_REFS_ALL;
	எனில் (args.send_mirror)
		flags |= MATCH_REFS_MIRROR;

	/* match them up */
	எனில் (match_தள்ளு_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		செல் cleanup;
	}
	எனில் (!is_empty_cas(&cas))
		apply_தள்ளு_cas(&cas, remote, remote_refs);

	எனில் (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_தள்ளு(remote_refs, args.send_mirror,
		args.force_update);

	ret = send_pack(repo, &args, fd, conn, remote_refs, &extra_have);

	எனில் (helper_status)
		print_helper_status(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= finish_connect(conn);

	எனில் (!helper_status)
		transport_print_தள்ளு_status(dest, remote_refs, args.verbose, 0, &reject_reasons);

	எனில் (!args.dry_run && remote) {
		கட்டமைப்பு ref *ref;
		க்கு (ref = remote_refs; ref; ref = ref->next)
			transport_update_tracking_ref(remote, ref, args.verbose);
	}

	எனில் (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&தள்ளு_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	திரும்பு ret;
}
