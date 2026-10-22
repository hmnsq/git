#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "pkt-line.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "connect.h"
#അന്തര്ഭാവ "send-pack.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "transport.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "protocol.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "write-or-die.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	ശൂന്യ,
};

സ്ഥിതം ഘടന send_pack_args args;

സ്ഥിതം ശൂന്യം print_helper_status(ഘടന ref *ref)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന ref_തള്ളുക_report *report;

	വേണ്ടി (; ref; ref = ref->next) {
		സ്ഥിരം അക്ഷരം *സന്ദേശം = ശൂന്യ;
		സ്ഥിരം അക്ഷരം *res;
		പൂർണ്ണസംഖ്യ എണ്ണം = 0;

		തിരഞ്ഞെടുപ്പ്(ref->status) {
		അവസ്ഥ REF_STATUS_NONE:
			res = "error";
			സന്ദേശം = "no match";
			നിർത്തുക;

		അവസ്ഥ REF_STATUS_OK:
			res = "ok";
			നിർത്തുക;

		അവസ്ഥ REF_STATUS_UPTODATE:
			res = "ok";
			സന്ദേശം = "up to date";
			നിർത്തുക;

		അവസ്ഥ REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			സന്ദേശം = "non-fast forward";
			നിർത്തുക;

		അവസ്ഥ REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			സന്ദേശം = "fetch first";
			നിർത്തുക;

		അവസ്ഥ REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			സന്ദേശം = "needs force";
			നിർത്തുക;

		അവസ്ഥ REF_STATUS_REJECT_STALE:
			res = "error";
			സന്ദേശം = "stale info";
			നിർത്തുക;

		അവസ്ഥ REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			സന്ദേശം = "remote ref updated since checkout";
			നിർത്തുക;

		അവസ്ഥ REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			സന്ദേശം = "already exists";
			നിർത്തുക;

		അവസ്ഥ REF_STATUS_REJECT_NODELETE:
		അവസ്ഥ REF_STATUS_REMOTE_REJECT:
			res = "error";
			നിർത്തുക;

		അവസ്ഥ REF_STATUS_EXPECTING_REPORT:
			res = "error";
			സന്ദേശം = "expecting report";
			നിർത്തുക;

		സ്ഥിരസ്ഥിതി:
			തുടരുക;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		എങ്കിൽ (ref->remote_status)
			സന്ദേശം = ref->remote_status;
		എങ്കിൽ (സന്ദേശം) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", സന്ദേശം, 0);
		}
		strbuf_addch(&buf, '\n');

		എങ്കിൽ (ref->status == REF_STATUS_OK) {
			വേണ്ടി (report = ref->report; report; report = report->next) {
				എങ്കിൽ (എണ്ണം++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				എങ്കിൽ (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				എങ്കിൽ (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_to_hex(report->old_oid));
				എങ്കിൽ (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_to_hex(report->new_oid));
				എങ്കിൽ (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ send_pack_config(സ്ഥിരം അക്ഷരം *k, സ്ഥിരം അക്ഷരം *v,
			    സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(k, "push.gpgsign")) {
		തിരഞ്ഞെടുപ്പ് (git_parse_maybe_bool(v)) {
		അവസ്ഥ 0:
			args.തള്ളുക_cert = SEND_PACK_തള്ളുക_CERT_NEVER;
			നിർത്തുക;
		അവസ്ഥ 1:
			args.തള്ളുക_cert = SEND_PACK_തള്ളുക_CERT_ALWAYS;
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			എങ്കിൽ (!strcasecmp(v, "if-asked"))
				args.തള്ളുക_cert = SEND_PACK_തള്ളുക_CERT_IF_ASKED;
			അല്ലെങ്കിൽ
				തിരികെ error(_("invalid value for '%s'"), k);
		}
	}
	തിരികെ git_default_config(k, v, ctx, cb);
}

പൂർണ്ണസംഖ്യ cmd_send_pack(പൂർണ്ണസംഖ്യ argc,
		  സ്ഥിരം അക്ഷരം **argv,
		  സ്ഥിരം അക്ഷരം *prefix,
		  ഘടന repository *repo)
{
	ഘടന refspec rs;
	സ്ഥിരം അക്ഷരം *remote_name = ശൂന്യ;
	ഘടന remote *remote = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *dest = ശൂന്യ;
	പൂർണ്ണസംഖ്യ fd[2];
	ഘടന child_process *conn;
	ഘടന oid_array extra_have = OID_ARRAY_INIT;
	ഘടന oid_array shallow = OID_ARRAY_INIT;
	ഘടന ref *remote_refs, *local_refs;
	പൂർണ്ണസംഖ്യ ret;
	പൂർണ്ണസംഖ്യ helper_status = 0;
	പൂർണ്ണസംഖ്യ send_all = 0;
	പൂർണ്ണസംഖ്യ verbose = 0;
	സ്ഥിരം അക്ഷരം *receivepack = "git-receive-pack";
	unsigned dry_run = 0;
	unsigned send_mirror = 0;
	unsigned force_update = 0;
	unsigned quiet = 0;
	പൂർണ്ണസംഖ്യ തള്ളുക_cert = 0;
	ഘടന string_list തള്ളുക_options = STRING_LIST_INIT_NODUP;
	unsigned use_thin_pack = 0;
	unsigned atomic = 0;
	unsigned stateless_rpc = 0;
	പൂർണ്ണസംഖ്യ flags;
	unsigned പൂർണ്ണസംഖ്യ reject_reasons;
	പൂർണ്ണസംഖ്യ progress = -1;
	പൂർണ്ണസംഖ്യ from_stdin = 0;
	ഘടന തള്ളുക_cas_option cas = {0};
	പൂർണ്ണസംഖ്യ force_if_includes = 0;
	ഘടന packet_reader reader;

	ഘടന option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_run, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &തള്ളുക_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_തള്ളുക_signed),
		OPT_STRING_LIST(0, "push-option", &തള്ളുക_options,
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
		  PARSE_OPT_OPTARG, parseopt_തള്ളുക_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_END()
	};

	repo_config(repo, send_pack_config, ശൂന്യ);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_തള്ളുക(&rs, repo->hash_algo);

	എങ്കിൽ (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	എങ്കിൽ (!dest)
		usage_with_options(send_pack_usage, options);

	args.verbose = verbose;
	args.dry_run = dry_run;
	args.send_mirror = send_mirror;
	args.force_update = force_update;
	args.quiet = quiet;
	args.തള്ളുക_cert = തള്ളുക_cert;
	args.progress = progress;
	args.use_thin_pack = use_thin_pack;
	args.atomic = atomic;
	args.stateless_rpc = stateless_rpc;
	args.തള്ളുക_options = തള്ളുക_options.nr ? &തള്ളുക_options : ശൂന്യ;
	args.url = dest;

	എങ്കിൽ (from_stdin) {
		എങ്കിൽ (args.stateless_rpc) {
			സ്ഥിരം അക്ഷരം *buf;
			ആകുമ്പോൾ ((buf = packet_read_വരി(0, ശൂന്യ)))
				refspec_append(&rs, buf);
		} അല്ലെങ്കിൽ {
			ഘടന strbuf വരി = STRBUF_INIT;
			ആകുമ്പോൾ (strbuf_getline(&വരി, stdin) != EOF)
				refspec_append(&rs, വരി.buf);
			strbuf_release(&വരി);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	എങ്കിൽ ((rs.nr > 0 && (send_all || args.send_mirror)) ||
	    (send_all && args.send_mirror))
		usage_with_options(send_pack_usage, options);

	എങ്കിൽ (remote_name) {
		remote = remote_get(remote_name);
		എങ്കിൽ (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	എങ്കിൽ (progress == -1)
		progress = !args.quiet && isatty(2);
	args.progress = progress;

	എങ്കിൽ (args.stateless_rpc) {
		conn = ശൂന്യ;
		fd[0] = 0;
		fd[1] = 1;
	} അല്ലെങ്കിൽ {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   args.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], ശൂന്യ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	തിരഞ്ഞെടുപ്പ് (discover_version(&reader)) {
	അവസ്ഥ protocol_v2:
		die("support for protocol v2 not implemented yet");
		നിർത്തുക;
	അവസ്ഥ protocol_v1:
	അവസ്ഥ protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		നിർത്തുക;
	അവസ്ഥ protocol_unknown_version:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = MATCH_REFS_NONE;

	എങ്കിൽ (send_all)
		flags |= MATCH_REFS_ALL;
	എങ്കിൽ (args.send_mirror)
		flags |= MATCH_REFS_MIRROR;

	/* match them up */
	എങ്കിൽ (match_തള്ളുക_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		പോകുക cleanup;
	}
	എങ്കിൽ (!is_empty_cas(&cas))
		apply_തള്ളുക_cas(&cas, remote, remote_refs);

	എങ്കിൽ (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_തള്ളുക(remote_refs, args.send_mirror,
		args.force_update);

	ret = send_pack(repo, &args, fd, conn, remote_refs, &extra_have);

	എങ്കിൽ (helper_status)
		print_helper_status(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= finish_connect(conn);

	എങ്കിൽ (!helper_status)
		transport_print_തള്ളുക_status(dest, remote_refs, args.verbose, 0, &reject_reasons);

	എങ്കിൽ (!args.dry_run && remote) {
		ഘടന ref *ref;
		വേണ്ടി (ref = remote_refs; ref; ref = ref->next)
			transport_update_tracking_ref(remote, ref, args.verbose);
	}

	എങ്കിൽ (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&തള്ളുക_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	തിരികെ ret;
}
