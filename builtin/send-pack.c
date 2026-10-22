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

स्थिर संरचना send_pack_तर्काः तर्काः;

स्थिर शून्य print_helper_स्थितिः(संरचना ref *ref)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना ref_प्रक्षिप_report *report;

	प्रति (; ref; ref = ref->अग्र) {
		ध्रुव वर्ण *सन्देशः = रिक्त;
		ध्रुव वर्ण *res;
		पूर्णाङ्क गणनम् = 0;

		विकल्प(ref->स्थितिः) {
		पक्ष REF_स्थितिः_NONE:
			res = "error";
			सन्देशः = "no match";
			भङ्ग;

		पक्ष REF_स्थितिः_OK:
			res = "ok";
			भङ्ग;

		पक्ष REF_स्थितिः_UPTODATE:
			res = "ok";
			सन्देशः = "up to date";
			भङ्ग;

		पक्ष REF_स्थितिः_REJECT_NONFASTFORWARD:
			res = "error";
			सन्देशः = "non-fast forward";
			भङ्ग;

		पक्ष REF_स्थितिः_REJECT_FETCH_FIRST:
			res = "error";
			सन्देशः = "fetch first";
			भङ्ग;

		पक्ष REF_स्थितिः_REJECT_NEEDS_FORCE:
			res = "error";
			सन्देशः = "needs force";
			भङ्ग;

		पक्ष REF_स्थितिः_REJECT_STALE:
			res = "error";
			सन्देशः = "stale info";
			भङ्ग;

		पक्ष REF_स्थितिः_REJECT_REMOTE_UPDATED:
			res = "error";
			सन्देशः = "remote ref updated since checkout";
			भङ्ग;

		पक्ष REF_स्थितिः_REJECT_ALREADY_EXISTS:
			res = "error";
			सन्देशः = "already exists";
			भङ्ग;

		पक्ष REF_स्थितिः_REJECT_NODELETE:
		पक्ष REF_स्थितिः_REMOTE_REJECT:
			res = "error";
			भङ्ग;

		पक्ष REF_स्थितिः_EXPECTING_REPORT:
			res = "error";
			सन्देशः = "expecting report";
			भङ्ग;

		स्वभाव:
			अनुवर्त;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		यदि (ref->remote_स्थितिः)
			सन्देशः = ref->remote_स्थितिः;
		यदि (सन्देशः) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", सन्देशः, 0);
		}
		strbuf_addch(&buf, '\n');

		यदि (ref->स्थितिः == REF_स्थितिः_OK) {
			प्रति (report = ref->report; report; report = report->अग्र) {
				यदि (गणनम्++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				यदि (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				यदि (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_गन्तव्यम्_hex(report->old_oid));
				यदि (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_गन्तव्यम्_hex(report->new_oid));
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
	यदि (!वाक्यं_तुलय(k, "push.gpgsign")) {
		विकल्प (git_parse_maybe_bool(v)) {
		पक्ष 0:
			तर्काः.प्रक्षिप_cert = SEND_PACK_प्रक्षिप_CERT_NEVER;
			भङ्ग;
		पक्ष 1:
			तर्काः.प्रक्षिप_cert = SEND_PACK_प्रक्षिप_CERT_ALWAYS;
			भङ्ग;
		स्वभाव:
			यदि (!strcasecmp(v, "if-asked"))
				तर्काः.प्रक्षिप_cert = SEND_PACK_प्रक्षिप_CERT_IF_ASKED;
			अन्यथा
				प्रत्यागम error(_("invalid value for '%s'"), k);
		}
	}
	प्रत्यागम git_default_config(k, v, ctx, cb);
}

पूर्णाङ्क cmd_send_pack(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *उपसर्गः,
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
	पूर्णाङ्क helper_स्थितिः = 0;
	पूर्णाङ्क send_all = 0;
	पूर्णाङ्क verbose = 0;
	ध्रुव वर्ण *receivepack = "git-receive-pack";
	अचिह्नित dry_सततम् = 0;
	अचिह्नित send_mirror = 0;
	अचिह्नित force_update = 0;
	अचिह्नित quiet = 0;
	पूर्णाङ्क प्रक्षिप_cert = 0;
	संरचना string_list प्रक्षिप_options = STRING_LIST_INIT_NODUP;
	अचिह्नित use_thin_pack = 0;
	अचिह्नित atomic = 0;
	अचिह्नित stateless_rpc = 0;
	पूर्णाङ्क flags;
	अचिह्नित पूर्णाङ्क reject_reasons;
	पूर्णाङ्क प्रगतिः = -1;
	पूर्णाङ्क from_stdin = 0;
	संरचना प्रक्षिप_cas_option cas = {0};
	पूर्णाङ्क force_if_includes = 0;
	संरचना packet_reader reader;

	संरचना option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_सततम्, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &प्रक्षिप_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_प्रक्षिप_signed),
		OPT_STRING_LIST(0, "push-option", &प्रक्षिप_options,
				N_("server-specific"),
				N_("option to transmit")),
		OPT_BOOL(0, "progress", &प्रगतिः, N_("force progress reporting")),
		OPT_BOOL(0, "thin", &use_thin_pack, N_("use thin pack")),
		OPT_BOOL(0, "atomic", &atomic, N_("request atomic transaction on remote side")),
		OPT_BOOL(0, "stateless-rpc", &stateless_rpc, N_("use stateless RPC protocol")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read refs from stdin")),
		OPT_BOOL(0, "helper-status", &helper_स्थितिः, N_("print status from remote helper")),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
		  N_("require old value of ref to be at this value"),
		  PARSE_OPT_OPTARG, parseopt_प्रक्षिप_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_अन्तः()
	};

	repo_config(repo, send_pack_config, रिक्त);
	argc = parse_options(argc, argv, उपसर्गः, options, send_pack_usage, 0);

	refspec_init_प्रक्षिप(&rs, repo->hash_algo);

	यदि (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	यदि (!dest)
		usage_with_options(send_pack_usage, options);

	तर्काः.verbose = verbose;
	तर्काः.dry_सततम् = dry_सततम्;
	तर्काः.send_mirror = send_mirror;
	तर्काः.force_update = force_update;
	तर्काः.quiet = quiet;
	तर्काः.प्रक्षिप_cert = प्रक्षिप_cert;
	तर्काः.प्रगतिः = प्रगतिः;
	तर्काः.use_thin_pack = use_thin_pack;
	तर्काः.atomic = atomic;
	तर्काः.stateless_rpc = stateless_rpc;
	तर्काः.प्रक्षिप_options = प्रक्षिप_options.nr ? &प्रक्षिप_options : रिक्त;
	तर्काः.url = dest;

	यदि (from_stdin) {
		यदि (तर्काः.stateless_rpc) {
			ध्रुव वर्ण *buf;
			यावत् ((buf = packet_read_पङ्क्तिः(0, रिक्त)))
				refspec_append(&rs, buf);
		} अन्यथा {
			संरचना strbuf पङ्क्तिः = STRBUF_INIT;
			यावत् (strbuf_getline(&पङ्क्तिः, stdin) != EOF)
				refspec_append(&rs, पङ्क्तिः.buf);
			strbuf_release(&पङ्क्तिः);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	यदि ((rs.nr > 0 && (send_all || तर्काः.send_mirror)) ||
	    (send_all && तर्काः.send_mirror))
		usage_with_options(send_pack_usage, options);

	यदि (remote_name) {
		remote = remote_get(remote_name);
		यदि (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	यदि (प्रगतिः == -1)
		प्रगतिः = !तर्काः.quiet && isatty(2);
	तर्काः.प्रगतिः = प्रगतिः;

	यदि (तर्काः.stateless_rpc) {
		conn = रिक्त;
		fd[0] = 0;
		fd[1] = 1;
	} अन्यथा {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   तर्काः.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], रिक्त, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	विकल्प (discover_संस्करणम्(&reader)) {
	पक्ष protocol_v2:
		die("support for protocol v2 not implemented yet");
		भङ्ग;
	पक्ष protocol_v1:
	पक्ष protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_प्रसामान्यम्,
				 &extra_have, &shallow);
		भङ्ग;
	पक्ष protocol_unknown_संस्करणम्:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = मेलनम्_REFS_NONE;

	यदि (send_all)
		flags |= मेलनम्_REFS_ALL;
	यदि (तर्काः.send_mirror)
		flags |= मेलनम्_REFS_MIRROR;

	/* match them up */
	यदि (मेलनम्_प्रक्षिप_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		गच्छ cleanup;
	}
	यदि (!is_रिक्तानि_cas(&cas))
		apply_प्रक्षिप_cas(&cas, remote, remote_refs);

	यदि (!is_रिक्तानि_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	समुच्चयः_ref_स्थितिः_for_प्रक्षिप(remote_refs, तर्काः.send_mirror,
		तर्काः.force_update);

	ret = send_pack(repo, &तर्काः, fd, conn, remote_refs, &extra_have);

	यदि (helper_स्थितिः)
		print_helper_स्थितिः(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= finish_connect(conn);

	यदि (!helper_स्थितिः)
		transport_print_प्रक्षिप_स्थितिः(dest, remote_refs, तर्काः.verbose, 0, &reject_reasons);

	यदि (!तर्काः.dry_सततम् && remote) {
		संरचना ref *ref;
		प्रति (ref = remote_refs; ref; ref = ref->अग्र)
			transport_update_tracking_ref(remote, ref, तर्काः.verbose);
	}

	यदि (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&प्रक्षिप_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	प्रत्यागम ret;
}
