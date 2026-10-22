#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "pkt-line.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "connect.h"
#అన్తర్భావ "send-pack.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "transport.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "protocol.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "write-or-die.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	ఖాళీ,
};

స్థిర నిర్మాణం send_pack_వాదనలు వాదనలు;

స్థిర శూన్య print_helper_స్థితి(నిర్మాణం ref *ref)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం ref_నెట్టు_report *report;

	కొరకు (; ref; ref = ref->తదుపరి) {
		స్థిరరాశి అక్షరం *సందేశం = ఖాళీ;
		స్థిరరాశి అక్షరం *res;
		పూర్ణాంకం లెక్క = 0;

		ఎంపిక(ref->స్థితి_2) {
		స్థితి REF_స్థితి_NONE:
			res = "error";
			సందేశం = "no match";
			ఆపు;

		స్థితి REF_స్థితి_OK:
			res = "ok";
			ఆపు;

		స్థితి REF_స్థితి_UPTODATE:
			res = "ok";
			సందేశం = "up to date";
			ఆపు;

		స్థితి REF_స్థితి_REJECT_NONFASTFORWARD:
			res = "error";
			సందేశం = "non-fast forward";
			ఆపు;

		స్థితి REF_స్థితి_REJECT_FETCH_మొదటి:
			res = "error";
			సందేశం = "fetch first";
			ఆపు;

		స్థితి REF_స్థితి_REJECT_NEEDS_FORCE:
			res = "error";
			సందేశం = "needs force";
			ఆపు;

		స్థితి REF_స్థితి_REJECT_STALE:
			res = "error";
			సందేశం = "stale info";
			ఆపు;

		స్థితి REF_స్థితి_REJECT_REMOTE_UPDATED:
			res = "error";
			సందేశం = "remote ref updated since checkout";
			ఆపు;

		స్థితి REF_స్థితి_REJECT_ALREADY_EXISTS:
			res = "error";
			సందేశం = "already exists";
			ఆపు;

		స్థితి REF_స్థితి_REJECT_NODELETE:
		స్థితి REF_స్థితి_REMOTE_REJECT:
			res = "error";
			ఆపు;

		స్థితి REF_స్థితి_EXPECTING_REPORT:
			res = "error";
			సందేశం = "expecting report";
			ఆపు;

		అప్రమేయ:
			కొనసాగు;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		ఒకవేళ (ref->remote_స్థితి)
			సందేశం = ref->remote_స్థితి;
		ఒకవేళ (సందేశం) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", సందేశం, 0);
		}
		strbuf_addch(&buf, '\n');

		ఒకవేళ (ref->స్థితి_2 == REF_స్థితి_OK) {
			కొరకు (report = ref->report; report; report = report->తదుపరి) {
				ఒకవేళ (లెక్క++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				ఒకవేళ (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				ఒకవేళ (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_గమ్యం_hex(report->old_oid));
				ఒకవేళ (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_గమ్యం_hex(report->new_oid));
				ఒకవేళ (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

స్థిర పూర్ణాంకం send_pack_config(స్థిరరాశి అక్షరం *k, స్థిరరాశి అక్షరం *v,
			    స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(k, "push.gpgsign")) {
		ఎంపిక (git_parse_maybe_bool(v)) {
		స్థితి 0:
			వాదనలు.నెట్టు_cert = SEND_PACK_నెట్టు_CERT_NEVER;
			ఆపు;
		స్థితి 1:
			వాదనలు.నెట్టు_cert = SEND_PACK_నెట్టు_CERT_ALWAYS;
			ఆపు;
		అప్రమేయ:
			ఒకవేళ (!strcasecmp(v, "if-asked"))
				వాదనలు.నెట్టు_cert = SEND_PACK_నెట్టు_CERT_IF_ASKED;
			లేకపోతే
				తిరిగి error(_("invalid value for '%s'"), k);
		}
	}
	తిరిగి git_default_config(k, v, ctx, cb);
}

పూర్ణాంకం cmd_send_pack(పూర్ణాంకం argc,
		  స్థిరరాశి అక్షరం **argv,
		  స్థిరరాశి అక్షరం *ఉపసర్గ,
		  నిర్మాణం repository *repo)
{
	నిర్మాణం refspec rs;
	స్థిరరాశి అక్షరం *remote_name = ఖాళీ;
	నిర్మాణం remote *remote = ఖాళీ;
	స్థిరరాశి అక్షరం *dest = ఖాళీ;
	పూర్ణాంకం fd[2];
	నిర్మాణం బిడ్డ_process *conn;
	నిర్మాణం oid_array extra_have = OID_ARRAY_INIT;
	నిర్మాణం oid_array shallow = OID_ARRAY_INIT;
	నిర్మాణం ref *remote_refs, *local_refs;
	పూర్ణాంకం ret;
	పూర్ణాంకం helper_స్థితి = 0;
	పూర్ణాంకం send_all = 0;
	పూర్ణాంకం verbose = 0;
	స్థిరరాశి అక్షరం *receivepack = "git-receive-pack";
	గుర్తులేని dry_నిరంతరం = 0;
	గుర్తులేని send_mirror = 0;
	గుర్తులేని force_update = 0;
	గుర్తులేని quiet = 0;
	పూర్ణాంకం నెట్టు_cert = 0;
	నిర్మాణం string_list నెట్టు_options = STRING_LIST_INIT_NODUP;
	గుర్తులేని use_thin_pack = 0;
	గుర్తులేని atomic = 0;
	గుర్తులేని stateless_rpc = 0;
	పూర్ణాంకం flags;
	గుర్తులేని పూర్ణాంకం reject_reasons;
	పూర్ణాంకం పురోగతి = -1;
	పూర్ణాంకం from_stdin = 0;
	నిర్మాణం నెట్టు_cas_option cas = {0};
	పూర్ణాంకం force_if_includes = 0;
	నిర్మాణం packet_reader reader;

	నిర్మాణం option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_నిరంతరం, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &నెట్టు_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_నెట్టు_signed),
		OPT_STRING_LIST(0, "push-option", &నెట్టు_options,
				N_("server-specific"),
				N_("option to transmit")),
		OPT_BOOL(0, "progress", &పురోగతి, N_("force progress reporting")),
		OPT_BOOL(0, "thin", &use_thin_pack, N_("use thin pack")),
		OPT_BOOL(0, "atomic", &atomic, N_("request atomic transaction on remote side")),
		OPT_BOOL(0, "stateless-rpc", &stateless_rpc, N_("use stateless RPC protocol")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read refs from stdin")),
		OPT_BOOL(0, "helper-status", &helper_స్థితి, N_("print status from remote helper")),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
		  N_("require old value of ref to be at this value"),
		  PARSE_OPT_OPTARG, parseopt_నెట్టు_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_చివర()
	};

	repo_config(repo, send_pack_config, ఖాళీ);
	argc = parse_options(argc, argv, ఉపసర్గ, options, send_pack_usage, 0);

	refspec_init_నెట్టు(&rs, repo->hash_algo);

	ఒకవేళ (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	ఒకవేళ (!dest)
		usage_with_options(send_pack_usage, options);

	వాదనలు.verbose = verbose;
	వాదనలు.dry_నిరంతరం = dry_నిరంతరం;
	వాదనలు.send_mirror = send_mirror;
	వాదనలు.force_update = force_update;
	వాదనలు.quiet = quiet;
	వాదనలు.నెట్టు_cert = నెట్టు_cert;
	వాదనలు.పురోగతి = పురోగతి;
	వాదనలు.use_thin_pack = use_thin_pack;
	వాదనలు.atomic = atomic;
	వాదనలు.stateless_rpc = stateless_rpc;
	వాదనలు.నెట్టు_options = నెట్టు_options.nr ? &నెట్టు_options : ఖాళీ;
	వాదనలు.url = dest;

	ఒకవేళ (from_stdin) {
		ఒకవేళ (వాదనలు.stateless_rpc) {
			స్థిరరాశి అక్షరం *buf;
			అయితే ((buf = packet_read_పంక్తి(0, ఖాళీ)))
				refspec_append(&rs, buf);
		} లేకపోతే {
			నిర్మాణం strbuf పంక్తి = STRBUF_INIT;
			అయితే (strbuf_getline(&పంక్తి, stdin) != EOF)
				refspec_append(&rs, పంక్తి.buf);
			strbuf_release(&పంక్తి);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	ఒకవేళ ((rs.nr > 0 && (send_all || వాదనలు.send_mirror)) ||
	    (send_all && వాదనలు.send_mirror))
		usage_with_options(send_pack_usage, options);

	ఒకవేళ (remote_name) {
		remote = remote_get(remote_name);
		ఒకవేళ (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	ఒకవేళ (పురోగతి == -1)
		పురోగతి = !వాదనలు.quiet && isatty(2);
	వాదనలు.పురోగతి = పురోగతి;

	ఒకవేళ (వాదనలు.stateless_rpc) {
		conn = ఖాళీ;
		fd[0] = 0;
		fd[1] = 1;
	} లేకపోతే {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   వాదనలు.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], ఖాళీ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ఎంపిక (discover_సంస్కరణ(&reader)) {
	స్థితి protocol_v2:
		die("support for protocol v2 not implemented yet");
		ఆపు;
	స్థితి protocol_v1:
	స్థితి protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_సాధారణ,
				 &extra_have, &shallow);
		ఆపు;
	స్థితి protocol_unknown_సంస్కరణ:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = సరిపోలిక_REFS_NONE;

	ఒకవేళ (send_all)
		flags |= సరిపోలిక_REFS_ALL;
	ఒకవేళ (వాదనలు.send_mirror)
		flags |= సరిపోలిక_REFS_MIRROR;

	/* match them up */
	ఒకవేళ (సరిపోలిక_నెట్టు_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		వెళ్ళు cleanup;
	}
	ఒకవేళ (!is_ఖాళీలు_cas(&cas))
		apply_నెట్టు_cas(&cas, remote, remote_refs);

	ఒకవేళ (!is_ఖాళీలు_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	సమితి_ref_స్థితి_for_నెట్టు(remote_refs, వాదనలు.send_mirror,
		వాదనలు.force_update);

	ret = send_pack(repo, &వాదనలు, fd, conn, remote_refs, &extra_have);

	ఒకవేళ (helper_స్థితి)
		print_helper_స్థితి(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= ముగింపు_connect(conn);

	ఒకవేళ (!helper_స్థితి)
		transport_print_నెట్టు_స్థితి(dest, remote_refs, వాదనలు.verbose, 0, &reject_reasons);

	ఒకవేళ (!వాదనలు.dry_నిరంతరం && remote) {
		నిర్మాణం ref *ref;
		కొరకు (ref = remote_refs; ref; ref = ref->తదుపరి)
			transport_update_tracking_ref(remote, ref, వాదనలు.verbose);
	}

	ఒకవేళ (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&నెట్టు_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	తిరిగి ret;
}
