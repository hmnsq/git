#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "pkt-line.h"
#অন্তর্ভাৱ "run-command.h"
#অন্তর্ভাৱ "remote.h"
#অন্তর্ভাৱ "connect.h"
#অন্তর্ভাৱ "send-pack.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "transport.h"
#অন্তর্ভাৱ "oid-array.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "protocol.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "write-or-die.h"

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	রিক্ত,
};

স্থির সংরচনা send_pack_args args;

স্থির শূন্য print_helper_status(সংরচনা ref *ref)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	সংরচনা ref_ঠেলো_report *report;

	বাবে (; ref; ref = ref->next) {
		ধ্রুৱ ৱর্ণ *বাৰ্তা = রিক্ত;
		ধ্রুৱ ৱর্ণ *res;
		পূর্ণাঙ্ক গুনতি = 0;

		ৱিকল্প(ref->status) {
		পক্ষ REF_STATUS_NONE:
			res = "error";
			বাৰ্তা = "no match";
			ভঙ্গ;

		পক্ষ REF_STATUS_OK:
			res = "ok";
			ভঙ্গ;

		পক্ষ REF_STATUS_UPTODATE:
			res = "ok";
			বাৰ্তা = "up to date";
			ভঙ্গ;

		পক্ষ REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			বাৰ্তা = "non-fast forward";
			ভঙ্গ;

		পক্ষ REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			বাৰ্তা = "fetch first";
			ভঙ্গ;

		পক্ষ REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			বাৰ্তা = "needs force";
			ভঙ্গ;

		পক্ষ REF_STATUS_REJECT_STALE:
			res = "error";
			বাৰ্তা = "stale info";
			ভঙ্গ;

		পক্ষ REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			বাৰ্তা = "remote ref updated since checkout";
			ভঙ্গ;

		পক্ষ REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			বাৰ্তা = "already exists";
			ভঙ্গ;

		পক্ষ REF_STATUS_REJECT_NODELETE:
		পক্ষ REF_STATUS_REMOTE_REJECT:
			res = "error";
			ভঙ্গ;

		পক্ষ REF_STATUS_EXPECTING_REPORT:
			res = "error";
			বাৰ্তা = "expecting report";
			ভঙ্গ;

		স্ৱভাৱ:
			অনুৱর্ত;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		যদি (ref->remote_status)
			বাৰ্তা = ref->remote_status;
		যদি (বাৰ্তা) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", বাৰ্তা, 0);
		}
		strbuf_addch(&buf, '\n');

		যদি (ref->status == REF_STATUS_OK) {
			বাবে (report = ref->report; report; report = report->next) {
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

স্থির পূর্ণাঙ্ক send_pack_config(ধ্রুৱ ৱর্ণ *k, ধ্রুৱ ৱর্ণ *v,
			    ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(k, "push.gpgsign")) {
		ৱিকল্প (git_parse_maybe_bool(v)) {
		পক্ষ 0:
			args.ঠেলো_cert = SEND_PACK_ঠেলো_CERT_NEVER;
			ভঙ্গ;
		পক্ষ 1:
			args.ঠেলো_cert = SEND_PACK_ঠেলো_CERT_ALWAYS;
			ভঙ্গ;
		স্ৱভাৱ:
			যদি (!strcasecmp(v, "if-asked"))
				args.ঠেলো_cert = SEND_PACK_ঠেলো_CERT_IF_ASKED;
			অন্যথা
				ঘূৰাই error(_("invalid value for '%s'"), k);
		}
	}
	ঘূৰাই git_default_config(k, v, ctx, cb);
}

পূর্ণাঙ্ক cmd_send_pack(পূর্ণাঙ্ক argc,
		  ধ্রুৱ ৱর্ণ **argv,
		  ধ্রুৱ ৱর্ণ *prefix,
		  সংরচনা repository *repo)
{
	সংরচনা refspec rs;
	ধ্রুৱ ৱর্ণ *remote_name = রিক্ত;
	সংরচনা remote *remote = রিক্ত;
	ধ্রুৱ ৱর্ণ *dest = রিক্ত;
	পূর্ণাঙ্ক fd[2];
	সংরচনা child_process *conn;
	সংরচনা oid_array extra_have = OID_ARRAY_INIT;
	সংরচনা oid_array shallow = OID_ARRAY_INIT;
	সংরচনা ref *remote_refs, *local_refs;
	পূর্ণাঙ্ক ret;
	পূর্ণাঙ্ক helper_status = 0;
	পূর্ণাঙ্ক send_all = 0;
	পূর্ণাঙ্ক verbose = 0;
	ধ্রুৱ ৱর্ণ *receivepack = "git-receive-pack";
	unsigned dry_run = 0;
	unsigned send_mirror = 0;
	unsigned force_update = 0;
	unsigned quiet = 0;
	পূর্ণাঙ্ক ঠেলো_cert = 0;
	সংরচনা string_list ঠেলো_options = STRING_LIST_INIT_NODUP;
	unsigned use_thin_pack = 0;
	unsigned atomic = 0;
	unsigned stateless_rpc = 0;
	পূর্ণাঙ্ক flags;
	unsigned পূর্ণাঙ্ক reject_reasons;
	পূর্ণাঙ্ক progress = -1;
	পূর্ণাঙ্ক from_stdin = 0;
	সংরচনা ঠেলো_cas_option cas = {0};
	পূর্ণাঙ্ক force_if_includes = 0;
	সংরচনা packet_reader reader;

	সংরচনা option options[] = {
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

	repo_config(repo, send_pack_config, রিক্ত);
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
	args.ঠেলো_options = ঠেলো_options.nr ? &ঠেলো_options : রিক্ত;
	args.url = dest;

	যদি (from_stdin) {
		যদি (args.stateless_rpc) {
			ধ্রুৱ ৱর্ণ *buf;
			যেতিয়ালৈ ((buf = packet_read_সাৰি(0, রিক্ত)))
				refspec_append(&rs, buf);
		} অন্যথা {
			সংরচনা strbuf সাৰি = STRBUF_INIT;
			যেতিয়ালৈ (strbuf_getline(&সাৰি, stdin) != EOF)
				refspec_append(&rs, সাৰি.buf);
			strbuf_release(&সাৰি);
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
		conn = রিক্ত;
		fd[0] = 0;
		fd[1] = 1;
	} অন্যথা {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   args.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], রিক্ত, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ৱিকল্প (discover_version(&reader)) {
	পক্ষ protocol_v2:
		die("support for protocol v2 not implemented yet");
		ভঙ্গ;
	পক্ষ protocol_v1:
	পক্ষ protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		ভঙ্গ;
	পক্ষ protocol_unknown_version:
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
		গচ্ছ cleanup;
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
		সংরচনা ref *ref;
		বাবে (ref = remote_refs; ref; ref = ref->next)
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
	ঘূৰাই ret;
}
