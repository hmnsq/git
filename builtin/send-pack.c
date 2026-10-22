#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "pkt-line.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "connect.h"
#অন্তর্ভাব "send-pack.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "transport.h"
#অন্তর্ভাব "oid-array.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "protocol.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "write-or-die.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	ফাঁকা,
};

স্থির কাঠামো send_pack_args args;

স্থির শূন্য print_helper_status(কাঠামো ref *ref)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো ref_ঠেলো_report *report;

	জন্য (; ref; ref = ref->পরবর্তী) {
		ধ্রুবক অক্ষর *বার্তা = ফাঁকা;
		ধ্রুবক অক্ষর *res;
		পূর্ণসংখ্যা গুনতি = 0;

		নির্বাচন(ref->status) {
		ক্ষেত্র REF_STATUS_NONE:
			res = "error";
			বার্তা = "no match";
			বিরতি;

		ক্ষেত্র REF_STATUS_OK:
			res = "ok";
			বিরতি;

		ক্ষেত্র REF_STATUS_UPTODATE:
			res = "ok";
			বার্তা = "up to date";
			বিরতি;

		ক্ষেত্র REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			বার্তা = "non-fast forward";
			বিরতি;

		ক্ষেত্র REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			বার্তা = "fetch first";
			বিরতি;

		ক্ষেত্র REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			বার্তা = "needs force";
			বিরতি;

		ক্ষেত্র REF_STATUS_REJECT_STALE:
			res = "error";
			বার্তা = "stale info";
			বিরতি;

		ক্ষেত্র REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			বার্তা = "remote ref updated since checkout";
			বিরতি;

		ক্ষেত্র REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			বার্তা = "already exists";
			বিরতি;

		ক্ষেত্র REF_STATUS_REJECT_NODELETE:
		ক্ষেত্র REF_STATUS_REMOTE_REJECT:
			res = "error";
			বিরতি;

		ক্ষেত্র REF_STATUS_EXPECTING_REPORT:
			res = "error";
			বার্তা = "expecting report";
			বিরতি;

		পূর্বনির্ধারিত:
			চালু;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		যদি (ref->remote_status)
			বার্তা = ref->remote_status;
		যদি (বার্তা) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", বার্তা, 0);
		}
		strbuf_addch(&buf, '\n');

		যদি (ref->status == REF_STATUS_OK) {
			জন্য (report = ref->report; report; report = report->পরবর্তী) {
				যদি (গুনতি++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				যদি (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				যদি (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_to_hex(report->old_oid));
				যদি (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_to_hex(report->new_oid));
				যদি (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

স্থির পূর্ণসংখ্যা send_pack_config(ধ্রুবক অক্ষর *k, ধ্রুবক অক্ষর *v,
			    ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(k, "push.gpgsign")) {
		নির্বাচন (git_parse_maybe_bool(v)) {
		ক্ষেত্র 0:
			args.ঠেলো_cert = SEND_PACK_ঠেলো_CERT_NEVER;
			বিরতি;
		ক্ষেত্র 1:
			args.ঠেলো_cert = SEND_PACK_ঠেলো_CERT_ALWAYS;
			বিরতি;
		পূর্বনির্ধারিত:
			যদি (!strcasecmp(v, "if-asked"))
				args.ঠেলো_cert = SEND_PACK_ঠেলো_CERT_IF_ASKED;
			অন্যথা
				ফেরত error(_("invalid value for '%s'"), k);
		}
	}
	ফেরত git_default_config(k, v, ctx, cb);
}

পূর্ণসংখ্যা cmd_send_pack(পূর্ণসংখ্যা argc,
		  ধ্রুবক অক্ষর **argv,
		  ধ্রুবক অক্ষর *prefix,
		  কাঠামো repository *repo)
{
	কাঠামো refspec rs;
	ধ্রুবক অক্ষর *remote_name = ফাঁকা;
	কাঠামো remote *remote = ফাঁকা;
	ধ্রুবক অক্ষর *dest = ফাঁকা;
	পূর্ণসংখ্যা fd[2];
	কাঠামো child_process *conn;
	কাঠামো oid_array extra_have = OID_ARRAY_INIT;
	কাঠামো oid_array shallow = OID_ARRAY_INIT;
	কাঠামো ref *remote_refs, *local_refs;
	পূর্ণসংখ্যা ret;
	পূর্ণসংখ্যা helper_status = 0;
	পূর্ণসংখ্যা send_all = 0;
	পূর্ণসংখ্যা verbose = 0;
	ধ্রুবক অক্ষর *receivepack = "git-receive-pack";
	unsigned dry_run = 0;
	unsigned send_mirror = 0;
	unsigned force_update = 0;
	unsigned quiet = 0;
	পূর্ণসংখ্যা ঠেলো_cert = 0;
	কাঠামো string_list ঠেলো_options = STRING_LIST_INIT_NODUP;
	unsigned use_thin_pack = 0;
	unsigned atomic = 0;
	unsigned stateless_rpc = 0;
	পূর্ণসংখ্যা flags;
	unsigned পূর্ণসংখ্যা reject_reasons;
	পূর্ণসংখ্যা progress = -1;
	পূর্ণসংখ্যা from_stdin = 0;
	কাঠামো ঠেলো_cas_option cas = {0};
	পূর্ণসংখ্যা force_if_includes = 0;
	কাঠামো packet_reader reader;

	কাঠামো option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_run, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &ঠেলো_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_ঠেলো_signed),
		OPT_STRING_LIST(0, "push-option", &ঠেলো_options,
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
		  PARSE_OPT_OPTARG, parseopt_ঠেলো_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_END()
	};

	repo_config(repo, send_pack_config, ফাঁকা);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_ঠেলো(&rs, repo->hash_algo);

	যদি (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	যদি (!dest)
		usage_with_options(send_pack_usage, options);

	args.verbose = verbose;
	args.dry_run = dry_run;
	args.send_mirror = send_mirror;
	args.force_update = force_update;
	args.quiet = quiet;
	args.ঠেলো_cert = ঠেলো_cert;
	args.progress = progress;
	args.use_thin_pack = use_thin_pack;
	args.atomic = atomic;
	args.stateless_rpc = stateless_rpc;
	args.ঠেলো_options = ঠেলো_options.nr ? &ঠেলো_options : ফাঁকা;
	args.url = dest;

	যদি (from_stdin) {
		যদি (args.stateless_rpc) {
			ধ্রুবক অক্ষর *buf;
			যখন ((buf = packet_read_সারি(0, ফাঁকা)))
				refspec_append(&rs, buf);
		} অন্যথা {
			কাঠামো strbuf সারি = STRBUF_INIT;
			যখন (strbuf_getline(&সারি, stdin) != EOF)
				refspec_append(&rs, সারি.buf);
			strbuf_release(&সারি);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	যদি ((rs.nr > 0 && (send_all || args.send_mirror)) ||
	    (send_all && args.send_mirror))
		usage_with_options(send_pack_usage, options);

	যদি (remote_name) {
		remote = remote_get(remote_name);
		যদি (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	যদি (progress == -1)
		progress = !args.quiet && isatty(2);
	args.progress = progress;

	যদি (args.stateless_rpc) {
		conn = ফাঁকা;
		fd[0] = 0;
		fd[1] = 1;
	} অন্যথা {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   args.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], ফাঁকা, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	নির্বাচন (discover_version(&reader)) {
	ক্ষেত্র protocol_v2:
		die("support for protocol v2 not implemented yet");
		বিরতি;
	ক্ষেত্র protocol_v1:
	ক্ষেত্র protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		বিরতি;
	ক্ষেত্র protocol_unknown_version:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = MATCH_REFS_NONE;

	যদি (send_all)
		flags |= MATCH_REFS_ALL;
	যদি (args.send_mirror)
		flags |= MATCH_REFS_MIRROR;

	/* match them up */
	যদি (match_ঠেলো_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		যাও cleanup;
	}
	যদি (!is_empty_cas(&cas))
		apply_ঠেলো_cas(&cas, remote, remote_refs);

	যদি (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_ঠেলো(remote_refs, args.send_mirror,
		args.force_update);

	ret = send_pack(repo, &args, fd, conn, remote_refs, &extra_have);

	যদি (helper_status)
		print_helper_status(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= finish_connect(conn);

	যদি (!helper_status)
		transport_print_ঠেলো_status(dest, remote_refs, args.verbose, 0, &reject_reasons);

	যদি (!args.dry_run && remote) {
		কাঠামো ref *ref;
		জন্য (ref = remote_refs; ref; ref = ref->পরবর্তী)
			transport_update_tracking_ref(remote, ref, args.verbose);
	}

	যদি (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&ঠেলো_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	ফেরত ret;
}
