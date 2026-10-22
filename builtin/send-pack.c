#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "pkt-line.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "connect.h"
#अन्तर्भाव "send-pack.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "transport.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "protocol.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "write-or-die.h"

स्थिर अचर वर्ण * अचर send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	रिक्त,
};

स्थिर संरचना send_pack_args args;

स्थिर शून्य print_helper_status(संरचना ref *ref)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना ref_थप_report *report;

	लागि (; ref; ref = ref->next) {
		अचर वर्ण *सन्देश = रिक्त;
		अचर वर्ण *res;
		पूर्णाङ्क गन्ती = 0;

		विकल्प(ref->status) {
		पक्ष REF_STATUS_NONE:
			res = "error";
			सन्देश = "no match";
			रोक;

		पक्ष REF_STATUS_OK:
			res = "ok";
			रोक;

		पक्ष REF_STATUS_UPTODATE:
			res = "ok";
			सन्देश = "up to date";
			रोक;

		पक्ष REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			सन्देश = "non-fast forward";
			रोक;

		पक्ष REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			सन्देश = "fetch first";
			रोक;

		पक्ष REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			सन्देश = "needs force";
			रोक;

		पक्ष REF_STATUS_REJECT_STALE:
			res = "error";
			सन्देश = "stale info";
			रोक;

		पक्ष REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			सन्देश = "remote ref updated since checkout";
			रोक;

		पक्ष REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			सन्देश = "already exists";
			रोक;

		पक्ष REF_STATUS_REJECT_NODELETE:
		पक्ष REF_STATUS_REMOTE_REJECT:
			res = "error";
			रोक;

		पक्ष REF_STATUS_EXPECTING_REPORT:
			res = "error";
			सन्देश = "expecting report";
			रोक;

		स्वभाव:
			जारी;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		यदि (ref->remote_status)
			सन्देश = ref->remote_status;
		यदि (सन्देश) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", सन्देश, 0);
		}
		strbuf_addch(&buf, '\n');

		यदि (ref->status == REF_STATUS_OK) {
			लागि (report = ref->report; report; report = report->next) {
				यदि (गन्ती++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				यदि (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				यदि (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_to_hex(report->old_oid));
				यदि (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_to_hex(report->new_oid));
				यदि (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

स्थिर पूर्णाङ्क send_pack_config(अचर वर्ण *k, अचर वर्ण *v,
			    अचर संरचना config_context *ctx, शून्य *cb)
{
	यदि (!म_भेद(k, "push.gpgsign")) {
		विकल्प (git_parse_maybe_bool(v)) {
		पक्ष 0:
			args.थप_cert = SEND_PACK_थप_CERT_NEVER;
			रोक;
		पक्ष 1:
			args.थप_cert = SEND_PACK_थप_CERT_ALWAYS;
			रोक;
		स्वभाव:
			यदि (!strcasecmp(v, "if-asked"))
				args.थप_cert = SEND_PACK_थप_CERT_IF_ASKED;
			अन्यथा
				फर्काऊ error(_("invalid value for '%s'"), k);
		}
	}
	फर्काऊ git_default_config(k, v, ctx, cb);
}

पूर्णाङ्क cmd_send_pack(पूर्णाङ्क argc,
		  अचर वर्ण **argv,
		  अचर वर्ण *prefix,
		  संरचना repository *repo)
{
	संरचना refspec rs;
	अचर वर्ण *remote_name = रिक्त;
	संरचना remote *remote = रिक्त;
	अचर वर्ण *dest = रिक्त;
	पूर्णाङ्क fd[2];
	संरचना child_process *conn;
	संरचना oid_array extra_have = OID_ARRAY_INIT;
	संरचना oid_array shallow = OID_ARRAY_INIT;
	संरचना ref *remote_refs, *local_refs;
	पूर्णाङ्क ret;
	पूर्णाङ्क helper_status = 0;
	पूर्णाङ्क send_all = 0;
	पूर्णाङ्क verbose = 0;
	अचर वर्ण *receivepack = "git-receive-pack";
	अचिन्हित dry_run = 0;
	अचिन्हित send_mirror = 0;
	अचिन्हित force_update = 0;
	अचिन्हित quiet = 0;
	पूर्णाङ्क थप_cert = 0;
	संरचना string_list थप_options = STRING_LIST_INIT_NODUP;
	अचिन्हित use_thin_pack = 0;
	अचिन्हित atomic = 0;
	अचिन्हित stateless_rpc = 0;
	पूर्णाङ्क flags;
	अचिन्हित पूर्णाङ्क reject_reasons;
	पूर्णाङ्क progress = -1;
	पूर्णाङ्क from_stdin = 0;
	संरचना थप_cas_option cas = {0};
	पूर्णाङ्क force_if_includes = 0;
	संरचना packet_reader reader;

	संरचना option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_run, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &थप_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_थप_signed),
		OPT_STRING_LIST(0, "push-option", &थप_options,
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
		  PARSE_OPT_OPTARG, parseopt_थप_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_END()
	};

	repo_config(repo, send_pack_config, रिक्त);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_थप(&rs, repo->hash_algo);

	यदि (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	यदि (!dest)
		usage_with_options(send_pack_usage, options);

	args.verbose = verbose;
	args.dry_run = dry_run;
	args.send_mirror = send_mirror;
	args.force_update = force_update;
	args.quiet = quiet;
	args.थप_cert = थप_cert;
	args.progress = progress;
	args.use_thin_pack = use_thin_pack;
	args.atomic = atomic;
	args.stateless_rpc = stateless_rpc;
	args.थप_options = थप_options.nr ? &थप_options : रिक्त;
	args.url = dest;

	यदि (from_stdin) {
		यदि (args.stateless_rpc) {
			अचर वर्ण *buf;
			जबसम्म ((buf = packet_read_पङ्क्ति(0, रिक्त)))
				refspec_append(&rs, buf);
		} अन्यथा {
			संरचना strbuf पङ्क्ति = STRBUF_INIT;
			जबसम्म (strbuf_getline(&पङ्क्ति, मानक_निवेश) != खातापूर्ण)
				refspec_append(&rs, पङ्क्ति.buf);
			strbuf_release(&पङ्क्ति);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	यदि ((rs.nr > 0 && (send_all || args.send_mirror)) ||
	    (send_all && args.send_mirror))
		usage_with_options(send_pack_usage, options);

	यदि (remote_name) {
		remote = remote_get(remote_name);
		यदि (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	यदि (progress == -1)
		progress = !args.quiet && isatty(2);
	args.progress = progress;

	यदि (args.stateless_rpc) {
		conn = रिक्त;
		fd[0] = 0;
		fd[1] = 1;
	} अन्यथा {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   args.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], रिक्त, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	विकल्प (discover_version(&reader)) {
	पक्ष protocol_v2:
		die("support for protocol v2 not implemented yet");
		रोक;
	पक्ष protocol_v1:
	पक्ष protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		रोक;
	पक्ष protocol_unknown_version:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = MATCH_REFS_NONE;

	यदि (send_all)
		flags |= MATCH_REFS_ALL;
	यदि (args.send_mirror)
		flags |= MATCH_REFS_MIRROR;

	/* match them up */
	यदि (match_थप_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		गच्छ cleanup;
	}
	यदि (!is_empty_cas(&cas))
		apply_थप_cas(&cas, remote, remote_refs);

	यदि (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_थप(remote_refs, args.send_mirror,
		args.force_update);

	ret = send_pack(repo, &args, fd, conn, remote_refs, &extra_have);

	यदि (helper_status)
		print_helper_status(remote_refs);

	बंद(fd[1]);
	बंद(fd[0]);

	ret |= finish_connect(conn);

	यदि (!helper_status)
		transport_print_थप_status(dest, remote_refs, args.verbose, 0, &reject_reasons);

	यदि (!args.dry_run && remote) {
		संरचना ref *ref;
		लागि (ref = remote_refs; ref; ref = ref->next)
			transport_update_tracking_ref(remote, ref, args.verbose);
	}

	यदि (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		ख_लिखो(मानक_त्रुटि, "Everything up-to-date\n");

cleanup:
	string_list_clear(&थप_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	फर्काऊ ret;
}
