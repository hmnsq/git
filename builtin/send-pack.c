#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "hex.h"
#समावेश "pkt-line.h"
#समावेश "run-command.h"
#समावेश "remote.h"
#समावेश "connect.h"
#समावेश "send-pack.h"
#समावेश "quote.h"
#समावेश "transport.h"
#समावेश "oid-array.h"
#समावेश "gettext.h"
#समावेश "protocol.h"
#समावेश "parse-options.h"
#समावेश "write-or-die.h"

अटल स्थिर अक्षर * स्थिर send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	शून्य,
};

अटल काष्ठा send_pack_args args;

अटल व्योम print_helper_status(काष्ठा ref *ref)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा ref_धकेलो_report *report;

	क्रम (; ref; ref = ref->अग्र) {
		स्थिर अक्षर *संदेश = शून्य;
		स्थिर अक्षर *res;
		पूर्णांक गिनती = 0;

		चयन(ref->status) {
		हाल REF_STATUS_NONE:
			res = "error";
			संदेश = "no match";
			अवरोध;

		हाल REF_STATUS_OK:
			res = "ok";
			अवरोध;

		हाल REF_STATUS_UPTODATE:
			res = "ok";
			संदेश = "up to date";
			अवरोध;

		हाल REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			संदेश = "non-fast forward";
			अवरोध;

		हाल REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			संदेश = "fetch first";
			अवरोध;

		हाल REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			संदेश = "needs force";
			अवरोध;

		हाल REF_STATUS_REJECT_STALE:
			res = "error";
			संदेश = "stale info";
			अवरोध;

		हाल REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			संदेश = "remote ref updated since checkout";
			अवरोध;

		हाल REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			संदेश = "already exists";
			अवरोध;

		हाल REF_STATUS_REJECT_NODELETE:
		हाल REF_STATUS_REMOTE_REJECT:
			res = "error";
			अवरोध;

		हाल REF_STATUS_EXPECTING_REPORT:
			res = "error";
			संदेश = "expecting report";
			अवरोध;

		शेष:
			जारी;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		अगर (ref->remote_status)
			संदेश = ref->remote_status;
		अगर (संदेश) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", संदेश, 0);
		}
		strbuf_addch(&buf, '\n');

		अगर (ref->status == REF_STATUS_OK) {
			क्रम (report = ref->report; report; report = report->अग्र) {
				अगर (गिनती++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				अगर (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				अगर (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_to_hex(report->old_oid));
				अगर (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_to_hex(report->new_oid));
				अगर (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

अटल पूर्णांक send_pack_config(स्थिर अक्षर *k, स्थिर अक्षर *v,
			    स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(k, "push.gpgsign")) {
		चयन (git_parse_maybe_bool(v)) {
		हाल 0:
			args.धकेलो_cert = SEND_PACK_धकेलो_CERT_NEVER;
			अवरोध;
		हाल 1:
			args.धकेलो_cert = SEND_PACK_धकेलो_CERT_ALWAYS;
			अवरोध;
		शेष:
			अगर (!strcasecmp(v, "if-asked"))
				args.धकेलो_cert = SEND_PACK_धकेलो_CERT_IF_ASKED;
			अन्यथा
				वापस error(_("invalid value for '%s'"), k);
		}
	}
	वापस git_default_config(k, v, ctx, cb);
}

पूर्णांक cmd_send_pack(पूर्णांक argc,
		  स्थिर अक्षर **argv,
		  स्थिर अक्षर *prefix,
		  काष्ठा repository *repo)
{
	काष्ठा refspec rs;
	स्थिर अक्षर *remote_name = शून्य;
	काष्ठा remote *remote = शून्य;
	स्थिर अक्षर *dest = शून्य;
	पूर्णांक fd[2];
	काष्ठा child_process *conn;
	काष्ठा oid_array extra_have = OID_ARRAY_INIT;
	काष्ठा oid_array shallow = OID_ARRAY_INIT;
	काष्ठा ref *remote_refs, *local_refs;
	पूर्णांक ret;
	पूर्णांक helper_status = 0;
	पूर्णांक send_all = 0;
	पूर्णांक verbose = 0;
	स्थिर अक्षर *receivepack = "git-receive-pack";
	अचिन्हित dry_run = 0;
	अचिन्हित send_mirror = 0;
	अचिन्हित force_update = 0;
	अचिन्हित quiet = 0;
	पूर्णांक धकेलो_cert = 0;
	काष्ठा string_list धकेलो_options = STRING_LIST_INIT_NODUP;
	अचिन्हित use_thin_pack = 0;
	अचिन्हित atomic = 0;
	अचिन्हित stateless_rpc = 0;
	पूर्णांक flags;
	अचिन्हित पूर्णांक reject_reasons;
	पूर्णांक progress = -1;
	पूर्णांक from_stdin = 0;
	काष्ठा धकेलो_cas_option cas = {0};
	पूर्णांक force_if_includes = 0;
	काष्ठा packet_reader reader;

	काष्ठा option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_run, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &धकेलो_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_धकेलो_signed),
		OPT_STRING_LIST(0, "push-option", &धकेलो_options,
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
		  PARSE_OPT_OPTARG, parseopt_धकेलो_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_END()
	};

	repo_config(repo, send_pack_config, शून्य);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_धकेलो(&rs, repo->hash_algo);

	अगर (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	अगर (!dest)
		usage_with_options(send_pack_usage, options);

	args.verbose = verbose;
	args.dry_run = dry_run;
	args.send_mirror = send_mirror;
	args.force_update = force_update;
	args.quiet = quiet;
	args.धकेलो_cert = धकेलो_cert;
	args.progress = progress;
	args.use_thin_pack = use_thin_pack;
	args.atomic = atomic;
	args.stateless_rpc = stateless_rpc;
	args.धकेलो_options = धकेलो_options.nr ? &धकेलो_options : शून्य;
	args.url = dest;

	अगर (from_stdin) {
		अगर (args.stateless_rpc) {
			स्थिर अक्षर *buf;
			जबतक ((buf = packet_read_पंक्ति(0, शून्य)))
				refspec_append(&rs, buf);
		} अन्यथा {
			काष्ठा strbuf पंक्ति = STRBUF_INIT;
			जबतक (strbuf_getline(&पंक्ति, मानक_निवेश) != खातापूर्ण)
				refspec_append(&rs, पंक्ति.buf);
			strbuf_release(&पंक्ति);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	अगर ((rs.nr > 0 && (send_all || args.send_mirror)) ||
	    (send_all && args.send_mirror))
		usage_with_options(send_pack_usage, options);

	अगर (remote_name) {
		remote = remote_get(remote_name);
		अगर (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	अगर (progress == -1)
		progress = !args.quiet && isatty(2);
	args.progress = progress;

	अगर (args.stateless_rpc) {
		conn = शून्य;
		fd[0] = 0;
		fd[1] = 1;
	} अन्यथा {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   args.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], शून्य, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	चयन (discover_version(&reader)) {
	हाल protocol_v2:
		die("support for protocol v2 not implemented yet");
		अवरोध;
	हाल protocol_v1:
	हाल protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		अवरोध;
	हाल protocol_unknown_version:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = MATCH_REFS_NONE;

	अगर (send_all)
		flags |= MATCH_REFS_ALL;
	अगर (args.send_mirror)
		flags |= MATCH_REFS_MIRROR;

	/* match them up */
	अगर (match_धकेलो_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		जाओ cleanup;
	}
	अगर (!is_empty_cas(&cas))
		apply_धकेलो_cas(&cas, remote, remote_refs);

	अगर (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_धकेलो(remote_refs, args.send_mirror,
		args.force_update);

	ret = send_pack(repo, &args, fd, conn, remote_refs, &extra_have);

	अगर (helper_status)
		print_helper_status(remote_refs);

	बंद(fd[1]);
	बंद(fd[0]);

	ret |= finish_connect(conn);

	अगर (!helper_status)
		transport_print_धकेलो_status(dest, remote_refs, args.verbose, 0, &reject_reasons);

	अगर (!args.dry_run && remote) {
		काष्ठा ref *ref;
		क्रम (ref = remote_refs; ref; ref = ref->अग्र)
			transport_update_tracking_ref(remote, ref, args.verbose);
	}

	अगर (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		ख_लिखो(मानक_त्रुटि, "Everything up-to-date\n");

cleanup:
	string_list_clear(&धकेलो_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	वापस ret;
}
