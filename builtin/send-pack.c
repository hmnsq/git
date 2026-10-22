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

स्थिर स्थिरांक अक्षर * स्थिरांक send_pack_usage[] = {
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
	संरचना ref_ढकला_report *report;

	साठी (; ref; ref = ref->next) {
		स्थिरांक अक्षर *संदेश = रिक्त;
		स्थिरांक अक्षर *res;
		पूर्णांक मोजणी = 0;

		निवड(ref->status) {
		स्थिती REF_STATUS_NONE:
			res = "error";
			संदेश = "no match";
			थांबा;

		स्थिती REF_STATUS_OK:
			res = "ok";
			थांबा;

		स्थिती REF_STATUS_UPTODATE:
			res = "ok";
			संदेश = "up to date";
			थांबा;

		स्थिती REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			संदेश = "non-fast forward";
			थांबा;

		स्थिती REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			संदेश = "fetch first";
			थांबा;

		स्थिती REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			संदेश = "needs force";
			थांबा;

		स्थिती REF_STATUS_REJECT_STALE:
			res = "error";
			संदेश = "stale info";
			थांबा;

		स्थिती REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			संदेश = "remote ref updated since checkout";
			थांबा;

		स्थिती REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			संदेश = "already exists";
			थांबा;

		स्थिती REF_STATUS_REJECT_NODELETE:
		स्थिती REF_STATUS_REMOTE_REJECT:
			res = "error";
			थांबा;

		स्थिती REF_STATUS_EXPECTING_REPORT:
			res = "error";
			संदेश = "expecting report";
			थांबा;

		पूर्वनिर्धारित:
			सुरूठेवा;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		जर (ref->remote_status)
			संदेश = ref->remote_status;
		जर (संदेश) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", संदेश, 0);
		}
		strbuf_addch(&buf, '\n');

		जर (ref->status == REF_STATUS_OK) {
			साठी (report = ref->report; report; report = report->next) {
				जर (मोजणी++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				जर (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				जर (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_to_hex(report->old_oid));
				जर (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_to_hex(report->new_oid));
				जर (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

स्थिर पूर्णांक send_pack_config(स्थिरांक अक्षर *k, स्थिरांक अक्षर *v,
			    स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	जर (!म_भेद(k, "push.gpgsign")) {
		निवड (git_parse_maybe_bool(v)) {
		स्थिती 0:
			args.ढकला_cert = SEND_PACK_ढकला_CERT_NEVER;
			थांबा;
		स्थिती 1:
			args.ढकला_cert = SEND_PACK_ढकला_CERT_ALWAYS;
			थांबा;
		पूर्वनिर्धारित:
			जर (!strcasecmp(v, "if-asked"))
				args.ढकला_cert = SEND_PACK_ढकला_CERT_IF_ASKED;
			अन्यथा
				परत error(_("invalid value for '%s'"), k);
		}
	}
	परत git_default_config(k, v, ctx, cb);
}

पूर्णांक cmd_send_pack(पूर्णांक argc,
		  स्थिरांक अक्षर **argv,
		  स्थिरांक अक्षर *prefix,
		  संरचना repository *repo)
{
	संरचना refspec rs;
	स्थिरांक अक्षर *remote_name = रिक्त;
	संरचना remote *remote = रिक्त;
	स्थिरांक अक्षर *dest = रिक्त;
	पूर्णांक fd[2];
	संरचना child_process *conn;
	संरचना oid_array extra_have = OID_ARRAY_INIT;
	संरचना oid_array shallow = OID_ARRAY_INIT;
	संरचना ref *remote_refs, *local_refs;
	पूर्णांक ret;
	पूर्णांक helper_status = 0;
	पूर्णांक send_all = 0;
	पूर्णांक verbose = 0;
	स्थिरांक अक्षर *receivepack = "git-receive-pack";
	अचिन्हित dry_run = 0;
	अचिन्हित send_mirror = 0;
	अचिन्हित force_update = 0;
	अचिन्हित quiet = 0;
	पूर्णांक ढकला_cert = 0;
	संरचना string_list ढकला_options = STRING_LIST_INIT_NODUP;
	अचिन्हित use_thin_pack = 0;
	अचिन्हित atomic = 0;
	अचिन्हित stateless_rpc = 0;
	पूर्णांक flags;
	अचिन्हित पूर्णांक reject_reasons;
	पूर्णांक progress = -1;
	पूर्णांक from_stdin = 0;
	संरचना ढकला_cas_option cas = {0};
	पूर्णांक force_if_includes = 0;
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
		OPT_CALLBACK_F(0, "signed", &ढकला_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_ढकला_signed),
		OPT_STRING_LIST(0, "push-option", &ढकला_options,
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
		  PARSE_OPT_OPTARG, parseopt_ढकला_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_END()
	};

	repo_config(repo, send_pack_config, रिक्त);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_ढकला(&rs, repo->hash_algo);

	जर (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	जर (!dest)
		usage_with_options(send_pack_usage, options);

	args.verbose = verbose;
	args.dry_run = dry_run;
	args.send_mirror = send_mirror;
	args.force_update = force_update;
	args.quiet = quiet;
	args.ढकला_cert = ढकला_cert;
	args.progress = progress;
	args.use_thin_pack = use_thin_pack;
	args.atomic = atomic;
	args.stateless_rpc = stateless_rpc;
	args.ढकला_options = ढकला_options.nr ? &ढकला_options : रिक्त;
	args.url = dest;

	जर (from_stdin) {
		जर (args.stateless_rpc) {
			स्थिरांक अक्षर *buf;
			पर्यंत ((buf = packet_read_ओळ(0, रिक्त)))
				refspec_append(&rs, buf);
		} अन्यथा {
			संरचना strbuf ओळ = STRBUF_INIT;
			पर्यंत (strbuf_getline(&ओळ, मानक_निवेश) != खातापूर्ण)
				refspec_append(&rs, ओळ.buf);
			strbuf_release(&ओळ);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	जर ((rs.nr > 0 && (send_all || args.send_mirror)) ||
	    (send_all && args.send_mirror))
		usage_with_options(send_pack_usage, options);

	जर (remote_name) {
		remote = remote_get(remote_name);
		जर (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	जर (progress == -1)
		progress = !args.quiet && isatty(2);
	args.progress = progress;

	जर (args.stateless_rpc) {
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

	निवड (discover_version(&reader)) {
	स्थिती protocol_v2:
		die("support for protocol v2 not implemented yet");
		थांबा;
	स्थिती protocol_v1:
	स्थिती protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		थांबा;
	स्थिती protocol_unknown_version:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = MATCH_REFS_NONE;

	जर (send_all)
		flags |= MATCH_REFS_ALL;
	जर (args.send_mirror)
		flags |= MATCH_REFS_MIRROR;

	/* match them up */
	जर (match_ढकला_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		जा cleanup;
	}
	जर (!is_empty_cas(&cas))
		apply_ढकला_cas(&cas, remote, remote_refs);

	जर (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_ढकला(remote_refs, args.send_mirror,
		args.force_update);

	ret = send_pack(repo, &args, fd, conn, remote_refs, &extra_have);

	जर (helper_status)
		print_helper_status(remote_refs);

	बंद(fd[1]);
	बंद(fd[0]);

	ret |= finish_connect(conn);

	जर (!helper_status)
		transport_print_ढकला_status(dest, remote_refs, args.verbose, 0, &reject_reasons);

	जर (!args.dry_run && remote) {
		संरचना ref *ref;
		साठी (ref = remote_refs; ref; ref = ref->next)
			transport_update_tracking_ref(remote, ref, args.verbose);
	}

	जर (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		ख_लिखो(मानक_त्रुटि, "Everything up-to-date\n");

cleanup:
	string_list_clear(&ढकला_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	परत ret;
}
