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

स्थिर ध्रुव वर्ण * ध्रुव send_pack_usage[] = {
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
	संरचना ref_धकेलो_report *report;

	प्रति (; ref; ref = ref->next) {
		ध्रुव वर्ण *संदेश = रिक्त;
		ध्रुव वर्ण *res;
		पूर्णाङ्क गिनती = 0;

		विकल्प(ref->status) {
		पक्ष REF_STATUS_NONE:
			res = "error";
			संदेश = "no match";
			भङ्ग;

		पक्ष REF_STATUS_OK:
			res = "ok";
			भङ्ग;

		पक्ष REF_STATUS_UPTODATE:
			res = "ok";
			संदेश = "up to date";
			भङ्ग;

		पक्ष REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			संदेश = "non-fast forward";
			भङ्ग;

		पक्ष REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			संदेश = "fetch first";
			भङ्ग;

		पक्ष REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			संदेश = "needs force";
			भङ्ग;

		पक्ष REF_STATUS_REJECT_STALE:
			res = "error";
			संदेश = "stale info";
			भङ्ग;

		पक्ष REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			संदेश = "remote ref updated since checkout";
			भङ्ग;

		पक्ष REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			संदेश = "already exists";
			भङ्ग;

		पक्ष REF_STATUS_REJECT_NODELETE:
		पक्ष REF_STATUS_REMOTE_REJECT:
			res = "error";
			भङ्ग;

		पक्ष REF_STATUS_EXPECTING_REPORT:
			res = "error";
			संदेश = "expecting report";
			भङ्ग;

		स्वभाव:
			अनुवर्त;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		यदि (ref->remote_status)
			संदेश = ref->remote_status;
		यदि (संदेश) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", संदेश, 0);
		}
		strbuf_addch(&buf, '\n');

		यदि (ref->status == REF_STATUS_OK) {
			प्रति (report = ref->report; report; report = report->next) {
				यदि (गिनती++ > 0)
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

स्थिर पूर्णाङ्क send_pack_config(ध्रुव वर्ण *k, ध्रुव वर्ण *v,
			    ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (!strcmp(k, "push.gpgsign")) {
		विकल्प (git_parse_maybe_bool(v)) {
		पक्ष 0:
			args.धकेलो_cert = SEND_PACK_धकेलो_CERT_NEVER;
			भङ्ग;
		पक्ष 1:
			args.धकेलो_cert = SEND_PACK_धकेलो_CERT_ALWAYS;
			भङ्ग;
		स्वभाव:
			यदि (!strcasecmp(v, "if-asked"))
				args.धकेलो_cert = SEND_PACK_धकेलो_CERT_IF_ASKED;
			अन्यथा
				प्रत्यागम error(_("invalid value for '%s'"), k);
		}
	}
	प्रत्यागम git_default_config(k, v, ctx, cb);
}

पूर्णाङ्क cmd_send_pack(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *prefix,
		  संरचना repository *repo)
{
	संरचना refspec rs;
	ध्रुव वर्ण *remote_name = रिक्त;
	संरचना remote *remote = रिक्त;
	ध्रुव वर्ण *dest = रिक्त;
	पूर्णाङ्क fd[2];
	संरचना child_process *conn;
	संरचना oid_array extra_have = OID_ARRAY_INIT;
	संरचना oid_array shallow = OID_ARRAY_INIT;
	संरचना ref *remote_refs, *local_refs;
	पूर्णाङ्क ret;
	पूर्णाङ्क helper_status = 0;
	पूर्णाङ्क send_all = 0;
	पूर्णाङ्क verbose = 0;
	ध्रुव वर्ण *receivepack = "git-receive-pack";
	unsigned dry_run = 0;
	unsigned send_mirror = 0;
	unsigned force_update = 0;
	unsigned quiet = 0;
	पूर्णाङ्क धकेलो_cert = 0;
	संरचना string_list धकेलो_options = STRING_LIST_INIT_NODUP;
	unsigned use_thin_pack = 0;
	unsigned atomic = 0;
	unsigned stateless_rpc = 0;
	पूर्णाङ्क flags;
	unsigned पूर्णाङ्क reject_reasons;
	पूर्णाङ्क progress = -1;
	पूर्णाङ्क from_stdin = 0;
	संरचना धकेलो_cas_option cas = {0};
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

	repo_config(repo, send_pack_config, रिक्त);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_धकेलो(&rs, repo->hash_algo);

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
	args.धकेलो_cert = धकेलो_cert;
	args.progress = progress;
	args.use_thin_pack = use_thin_pack;
	args.atomic = atomic;
	args.stateless_rpc = stateless_rpc;
	args.धकेलो_options = धकेलो_options.nr ? &धकेलो_options : रिक्त;
	args.url = dest;

	यदि (from_stdin) {
		यदि (args.stateless_rpc) {
			ध्रुव वर्ण *buf;
			यावत् ((buf = packet_read_पंक्ति(0, रिक्त)))
				refspec_append(&rs, buf);
		} अन्यथा {
			संरचना strbuf पंक्ति = STRBUF_INIT;
			यावत् (strbuf_getline(&पंक्ति, stdin) != EOF)
				refspec_append(&rs, पंक्ति.buf);
			strbuf_release(&पंक्ति);
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
		भङ्ग;
	पक्ष protocol_v1:
	पक्ष protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		भङ्ग;
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
	यदि (match_धकेलो_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		गच्छ cleanup;
	}
	यदि (!is_empty_cas(&cas))
		apply_धकेलो_cas(&cas, remote, remote_refs);

	यदि (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_धकेलो(remote_refs, args.send_mirror,
		args.force_update);

	ret = send_pack(repo, &args, fd, conn, remote_refs, &extra_have);

	यदि (helper_status)
		print_helper_status(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= finish_connect(conn);

	यदि (!helper_status)
		transport_print_धकेलो_status(dest, remote_refs, args.verbose, 0, &reject_reasons);

	यदि (!args.dry_run && remote) {
		संरचना ref *ref;
		प्रति (ref = remote_refs; ref; ref = ref->next)
			transport_update_tracking_ref(remote, ref, args.verbose);
	}

	यदि (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&धकेलो_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	प्रत्यागम ret;
}
