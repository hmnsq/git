#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "pkt-line.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "connect.h"
#ଅନ୍ତର୍ଭାଵ "send-pack.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "transport.h"
#ଅନ୍ତର୍ଭାଵ "oid-array.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "protocol.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	ଖାଲି,
};

ସ୍ଥିର ସଂରଚନା send_pack_args args;

ସ୍ଥିର ଶୂନ୍ୟ print_helper_status(ସଂରଚନା ref *ref)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା ref_ଠେଲ_report *report;

	ପାଇଁ (; ref; ref = ref->next) {
		ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ = ଖାଲି;
		ଅଚଳ ଅକ୍ଷର *res;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;

		ବାଛ(ref->status) {
		ସ୍ଥିତି REF_STATUS_NONE:
			res = "error";
			ସନ୍ଦେଶ = "no match";
			ଭାଙ୍ଗ;

		ସ୍ଥିତି REF_STATUS_OK:
			res = "ok";
			ଭାଙ୍ଗ;

		ସ୍ଥିତି REF_STATUS_UPTODATE:
			res = "ok";
			ସନ୍ଦେଶ = "up to date";
			ଭାଙ୍ଗ;

		ସ୍ଥିତି REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			ସନ୍ଦେଶ = "non-fast forward";
			ଭାଙ୍ଗ;

		ସ୍ଥିତି REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			ସନ୍ଦେଶ = "fetch first";
			ଭାଙ୍ଗ;

		ସ୍ଥିତି REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			ସନ୍ଦେଶ = "needs force";
			ଭାଙ୍ଗ;

		ସ୍ଥିତି REF_STATUS_REJECT_STALE:
			res = "error";
			ସନ୍ଦେଶ = "stale info";
			ଭାଙ୍ଗ;

		ସ୍ଥିତି REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			ସନ୍ଦେଶ = "remote ref updated since checkout";
			ଭାଙ୍ଗ;

		ସ୍ଥିତି REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			ସନ୍ଦେଶ = "already exists";
			ଭାଙ୍ଗ;

		ସ୍ଥିତି REF_STATUS_REJECT_NODELETE:
		ସ୍ଥିତି REF_STATUS_REMOTE_REJECT:
			res = "error";
			ଭାଙ୍ଗ;

		ସ୍ଥିତି REF_STATUS_EXPECTING_REPORT:
			res = "error";
			ସନ୍ଦେଶ = "expecting report";
			ଭାଙ୍ଗ;

		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			ଜାରି;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		ଯଦି (ref->remote_status)
			ସନ୍ଦେଶ = ref->remote_status;
		ଯଦି (ସନ୍ଦେଶ) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", ସନ୍ଦେଶ, 0);
		}
		strbuf_addch(&buf, '\n');

		ଯଦି (ref->status == REF_STATUS_OK) {
			ପାଇଁ (report = ref->report; report; report = report->next) {
				ଯଦି (ଗଣନା_2++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				ଯଦି (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				ଯଦି (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_to_hex(report->old_oid));
				ଯଦି (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_to_hex(report->new_oid));
				ଯଦି (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା send_pack_config(ଅଚଳ ଅକ୍ଷର *k, ଅଚଳ ଅକ୍ଷର *v,
			    ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(k, "push.gpgsign")) {
		ବାଛ (git_parse_maybe_bool(v)) {
		ସ୍ଥିତି 0:
			args.ଠେଲ_cert = SEND_PACK_ଠେଲ_CERT_NEVER;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 1:
			args.ଠେଲ_cert = SEND_PACK_ଠେଲ_CERT_ALWAYS;
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			ଯଦି (!strcasecmp(v, "if-asked"))
				args.ଠେଲ_cert = SEND_PACK_ଠେଲ_CERT_IF_ASKED;
			ଅନ୍ୟଥା
				ଫେରନ୍ତୁ error(_("invalid value for '%s'"), k);
		}
	}
	ଫେରନ୍ତୁ git_default_config(k, v, ctx, cb);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_send_pack(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		  ଅଚଳ ଅକ୍ଷର **argv,
		  ଅଚଳ ଅକ୍ଷର *prefix,
		  ସଂରଚନା repository *repo)
{
	ସଂରଚନା refspec rs;
	ଅଚଳ ଅକ୍ଷର *remote_name = ଖାଲି;
	ସଂରଚନା remote *remote = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *dest = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd[2];
	ସଂରଚନା child_process *conn;
	ସଂରଚନା oid_array extra_have = OID_ARRAY_INIT;
	ସଂରଚନା oid_array shallow = OID_ARRAY_INIT;
	ସଂରଚନା ref *remote_refs, *local_refs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା helper_status = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା send_all = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose = 0;
	ଅଚଳ ଅକ୍ଷର *receivepack = "git-receive-pack";
	unsigned dry_run = 0;
	unsigned send_mirror = 0;
	unsigned force_update = 0;
	unsigned quiet = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_cert = 0;
	ସଂରଚନା string_list ଠେଲ_options = STRING_LIST_INIT_NODUP;
	unsigned use_thin_pack = 0;
	unsigned atomic = 0;
	unsigned stateless_rpc = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା reject_reasons;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା from_stdin = 0;
	ସଂରଚନା ଠେଲ_cas_option cas = {0};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_if_includes = 0;
	ସଂରଚନା packet_reader reader;

	ସଂରଚନା option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_run, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &ଠେଲ_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_ଠେଲ_signed),
		OPT_STRING_LIST(0, "push-option", &ଠେଲ_options,
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
		  PARSE_OPT_OPTARG, parseopt_ଠେଲ_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_END()
	};

	repo_config(repo, send_pack_config, ଖାଲି);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_ଠେଲ(&rs, repo->hash_algo);

	ଯଦି (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	ଯଦି (!dest)
		usage_with_options(send_pack_usage, options);

	args.verbose = verbose;
	args.dry_run = dry_run;
	args.send_mirror = send_mirror;
	args.force_update = force_update;
	args.quiet = quiet;
	args.ଠେଲ_cert = ଠେଲ_cert;
	args.progress = progress;
	args.use_thin_pack = use_thin_pack;
	args.atomic = atomic;
	args.stateless_rpc = stateless_rpc;
	args.ଠେଲ_options = ଠେଲ_options.nr ? &ଠେଲ_options : ଖାଲି;
	args.url = dest;

	ଯଦି (from_stdin) {
		ଯଦି (args.stateless_rpc) {
			ଅଚଳ ଅକ୍ଷର *buf;
			ଯେତେବେଳେ ((buf = packet_read_ଧାଡ଼ି(0, ଖାଲି)))
				refspec_append(&rs, buf);
		} ଅନ୍ୟଥା {
			ସଂରଚନା strbuf ଧାଡ଼ି = STRBUF_INIT;
			ଯେତେବେଳେ (strbuf_getline(&ଧାଡ଼ି, stdin) != EOF)
				refspec_append(&rs, ଧାଡ଼ି.buf);
			strbuf_release(&ଧାଡ଼ି);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	ଯଦି ((rs.nr > 0 && (send_all || args.send_mirror)) ||
	    (send_all && args.send_mirror))
		usage_with_options(send_pack_usage, options);

	ଯଦି (remote_name) {
		remote = remote_get(remote_name);
		ଯଦି (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	ଯଦି (progress == -1)
		progress = !args.quiet && isatty(2);
	args.progress = progress;

	ଯଦି (args.stateless_rpc) {
		conn = ଖାଲି;
		fd[0] = 0;
		fd[1] = 1;
	} ଅନ୍ୟଥା {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   args.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], ଖାଲି, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ବାଛ (discover_version(&reader)) {
	ସ୍ଥିତି protocol_v2:
		die("support for protocol v2 not implemented yet");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି protocol_v1:
	ସ୍ଥିତି protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି protocol_unknown_version:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = MATCH_REFS_NONE;

	ଯଦି (send_all)
		flags |= MATCH_REFS_ALL;
	ଯଦି (args.send_mirror)
		flags |= MATCH_REFS_MIRROR;

	/* match them up */
	ଯଦି (match_ଠେଲ_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		ଯାଅ cleanup;
	}
	ଯଦି (!is_empty_cas(&cas))
		apply_ଠେଲ_cas(&cas, remote, remote_refs);

	ଯଦି (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_ଠେଲ(remote_refs, args.send_mirror,
		args.force_update);

	ret = send_pack(repo, &args, fd, conn, remote_refs, &extra_have);

	ଯଦି (helper_status)
		print_helper_status(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= finish_connect(conn);

	ଯଦି (!helper_status)
		transport_print_ଠେଲ_status(dest, remote_refs, args.verbose, 0, &reject_reasons);

	ଯଦି (!args.dry_run && remote) {
		ସଂରଚନା ref *ref;
		ପାଇଁ (ref = remote_refs; ref; ref = ref->next)
			transport_update_tracking_ref(remote, ref, args.verbose);
	}

	ଯଦି (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&ଠେଲ_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	ଫେରନ୍ତୁ ret;
}
