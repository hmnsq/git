#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "hex.h"
#include "pkt-line.h"
#include "run-command.h"
#include "remote.h"
#include "connect.h"
#include "send-pack.h"
#include "quote.h"
#include "transport.h"
#include "oid-array.h"
#include "gettext.h"
#include "protocol.h"
#include "parse-options.h"
#include "write-or-die.h"

static const var_na * const send_pack_usage[] = {
	N_("git send-pack [--mirror] [--dry-run] [--force]\n"
	   "              [--receive-pack=<git-receive-pack>]\n"
	   "              [--verbose] [--thin] [--atomic]\n"
	   "              [--[no-]signed | --signed=(true|false|if-asked)]\n"
	   "              [<host>:]<directory> (--all | <ref>...)"),
	NULL,
};

static sa_nracanaa send_pack_args args;

static void print_helper_status(sa_nracanaa ref *ref)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa ref_push_report *report;

	for (; ref; ref = ref->next) {
		const var_na *msg = NULL;
		const var_na *res;
		puur_naa_nka count = 0;

		switch(ref->status) {
		pak_sa REF_STATUS_NONE:
			res = "error";
			msg = "no match";
			bha_nga;

		pak_sa REF_STATUS_OK:
			res = "ok";
			bha_nga;

		pak_sa REF_STATUS_UPTODATE:
			res = "ok";
			msg = "up to date";
			bha_nga;

		pak_sa REF_STATUS_REJECT_NONFASTFORWARD:
			res = "error";
			msg = "non-fast forward";
			bha_nga;

		pak_sa REF_STATUS_REJECT_FETCH_FIRST:
			res = "error";
			msg = "fetch first";
			bha_nga;

		pak_sa REF_STATUS_REJECT_NEEDS_FORCE:
			res = "error";
			msg = "needs force";
			bha_nga;

		pak_sa REF_STATUS_REJECT_STALE:
			res = "error";
			msg = "stale info";
			bha_nga;

		pak_sa REF_STATUS_REJECT_REMOTE_UPDATED:
			res = "error";
			msg = "remote ref updated since checkout";
			bha_nga;

		pak_sa REF_STATUS_REJECT_ALREADY_EXISTS:
			res = "error";
			msg = "already exists";
			bha_nga;

		pak_sa REF_STATUS_REJECT_NODELETE:
		pak_sa REF_STATUS_REMOTE_REJECT:
			res = "error";
			bha_nga;

		pak_sa REF_STATUS_EXPECTING_REPORT:
			res = "error";
			msg = "expecting report";
			bha_nga;

		default:
			continue;
		}

		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s %s", res, ref->name);
		ᤀᤡ᤺ (ref->remote_status)
			msg = ref->remote_status;
		ᤀᤡ᤺ (msg) {
			strbuf_addch(&buf, ' ');
			quote_two_c_style(&buf, "", msg, 0);
		}
		strbuf_addch(&buf, '\n');

		ᤀᤡ᤺ (ref->status == REF_STATUS_OK) {
			for (report = ref->report; report; report = report->next) {
				ᤀᤡ᤺ (count++ > 0)
					strbuf_addf(&buf, "ok %s\n", ref->name);
				ᤀᤡ᤺ (report->ref_name)
					strbuf_addf(&buf, "option refname %s\n",
						report->ref_name);
				ᤀᤡ᤺ (report->old_oid)
					strbuf_addf(&buf, "option old-oid %s\n",
						oid_to_hex(report->old_oid));
				ᤀᤡ᤺ (report->new_oid)
					strbuf_addf(&buf, "option new-oid %s\n",
						oid_to_hex(report->new_oid));
				ᤀᤡ᤺ (report->forced_update)
					strbuf_addstr(&buf, "option forced-update\n");
			}
		}
		write_or_die(1, buf.buf, buf.len);
	}
	strbuf_release(&buf);
}

static puur_naa_nka send_pack_config(const var_na *k, const var_na *v,
			    const sa_nracanaa config_context *ctx, void *cb)
{
	ᤀᤡ᤺ (!strcmp(k, "push.gpgsign")) {
		switch (git_parse_maybe_bool(v)) {
		pak_sa 0:
			args.push_cert = SEND_PACK_PUSH_CERT_NEVER;
			bha_nga;
		pak_sa 1:
			args.push_cert = SEND_PACK_PUSH_CERT_ALWAYS;
			bha_nga;
		default:
			ᤀᤡ᤺ (!strcasecmp(v, "if-asked"))
				args.push_cert = SEND_PACK_PUSH_CERT_IF_ASKED;
			ᤀᤡ᤺ᤆᤡ
				ᤀᤡᤴ error(_("invalid value for '%s'"), k);
		}
	}
	ᤀᤡᤴ git_default_config(k, v, ctx, cb);
}

puur_naa_nka cmd_send_pack(puur_naa_nka argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo)
{
	sa_nracanaa refspec rs;
	const var_na *remote_name = NULL;
	sa_nracanaa remote *remote = NULL;
	const var_na *dest = NULL;
	puur_naa_nka fd[2];
	sa_nracanaa child_process *conn;
	sa_nracanaa oid_array extra_have = OID_ARRAY_INIT;
	sa_nracanaa oid_array shallow = OID_ARRAY_INIT;
	sa_nracanaa ref *remote_refs, *local_refs;
	puur_naa_nka ret;
	puur_naa_nka helper_status = 0;
	puur_naa_nka send_all = 0;
	puur_naa_nka verbose = 0;
	const var_na *receivepack = "git-receive-pack";
	unsigned dry_run = 0;
	unsigned send_mirror = 0;
	unsigned force_update = 0;
	unsigned quiet = 0;
	puur_naa_nka push_cert = 0;
	sa_nracanaa string_list push_options = STRING_LIST_INIT_NODUP;
	unsigned use_thin_pack = 0;
	unsigned atomic = 0;
	unsigned stateless_rpc = 0;
	puur_naa_nka flags;
	unsigned puur_naa_nka reject_reasons;
	puur_naa_nka progress = -1;
	puur_naa_nka from_stdin = 0;
	sa_nracanaa push_cas_option cas = {0};
	puur_naa_nka force_if_includes = 0;
	sa_nracanaa packet_reader reader;

	sa_nracanaa option options[] = {
		OPT__VERBOSITY(&verbose),
		OPT_STRING(0, "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING(0, "remote", &remote_name, "remote", N_("remote name")),
		OPT_BOOL(0, "all", &send_all, N_("push all refs")),
		OPT_BOOL('n' , "dry-run", &dry_run, N_("dry run")),
		OPT_BOOL(0, "mirror", &send_mirror, N_("mirror all refs")),
		OPT_BOOL('f', "force", &force_update, N_("force updates")),
		OPT_CALLBACK_F(0, "signed", &push_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
		  PARSE_OPT_OPTARG, option_parse_push_signed),
		OPT_STRING_LIST(0, "push-option", &push_options,
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
		  PARSE_OPT_OPTARG, parseopt_push_cas_option),
		OPT_BOOL(0, TRANS_OPT_FORCE_IF_INCLUDES, &force_if_includes,
			 N_("require remote updates to be integrated locally")),
		OPT_END()
	};

	repo_config(repo, send_pack_config, NULL);
	argc = parse_options(argc, argv, prefix, options, send_pack_usage, 0);

	refspec_init_push(&rs, repo->hash_algo);

	ᤀᤡ᤺ (argc > 0) {
		dest = argv[0];
		refspec_appendn(&rs, argv + 1, argc - 1);
	}

	ᤀᤡ᤺ (!dest)
		usage_with_options(send_pack_usage, options);

	args.verbose = verbose;
	args.dry_run = dry_run;
	args.send_mirror = send_mirror;
	args.force_update = force_update;
	args.quiet = quiet;
	args.push_cert = push_cert;
	args.progress = progress;
	args.use_thin_pack = use_thin_pack;
	args.atomic = atomic;
	args.stateless_rpc = stateless_rpc;
	args.push_options = push_options.nr ? &push_options : NULL;
	args.url = dest;

	ᤀᤡ᤺ (from_stdin) {
		ᤀᤡ᤺ (args.stateless_rpc) {
			const var_na *buf;
			ᤀᤠ᤺ ((buf = packet_read_line(0, NULL)))
				refspec_append(&rs, buf);
		} ᤀᤡ᤺ᤆᤡ {
			sa_nracanaa strbuf line = STRBUF_INIT;
			ᤀᤠ᤺ (strbuf_getline(&line, stdin) != EOF)
				refspec_append(&rs, line.buf);
			strbuf_release(&line);
		}
	}

	/*
	 * --all and --mirror are incompatible; neither makes sense
	 * with any refspecs.
	 */
	ᤀᤡ᤺ ((rs.nr > 0 && (send_all || args.send_mirror)) ||
	    (send_all && args.send_mirror))
		usage_with_options(send_pack_usage, options);

	ᤀᤡ᤺ (remote_name) {
		remote = remote_get(remote_name);
		ᤀᤡ᤺ (!remote_has_url(remote, dest)) {
			die("Destination %s is not a uri for %s",
			    dest, remote_name);
		}
	}

	ᤀᤡ᤺ (progress == -1)
		progress = !args.quiet && isatty(2);
	args.progress = progress;

	ᤀᤡ᤺ (args.stateless_rpc) {
		conn = NULL;
		fd[0] = 0;
		fd[1] = 1;
	} ᤀᤡ᤺ᤆᤡ {
		conn = git_connect(fd, dest, GIT_CONNECT_RECEIVE_PACK,
				   receivepack,
				   args.verbose ? CONNECT_VERBOSE : 0);
	}

	packet_reader_init(&reader, fd[0], NULL, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	switch (discover_version(&reader)) {
	pak_sa protocol_v2:
		die("support for protocol v2 not implemented yet");
		bha_nga;
	pak_sa protocol_v1:
	pak_sa protocol_v0:
		get_remote_heads(&reader, &remote_refs, REF_NORMAL,
				 &extra_have, &shallow);
		bha_nga;
	pak_sa protocol_unknown_version:
		BUG("unknown protocol version");
	}

	local_refs = get_local_heads();

	flags = MATCH_REFS_NONE;

	ᤀᤡ᤺ (send_all)
		flags |= MATCH_REFS_ALL;
	ᤀᤡ᤺ (args.send_mirror)
		flags |= MATCH_REFS_MIRROR;

	/* match them up */
	ᤀᤡ᤺ (match_push_refs(local_refs, &remote_refs, &rs, flags)) {
		ret = -1;
		goto cleanup;
	}
	ᤀᤡ᤺ (!is_empty_cas(&cas))
		apply_push_cas(&cas, remote, remote_refs);

	ᤀᤡ᤺ (!is_empty_cas(&cas) && force_if_includes)
		cas.use_force_if_includes = 1;

	set_ref_status_for_push(remote_refs, args.send_mirror,
		args.force_update);

	ret = send_pack(repo, &args, fd, conn, remote_refs, &extra_have);

	ᤀᤡ᤺ (helper_status)
		print_helper_status(remote_refs);

	close(fd[1]);
	close(fd[0]);

	ret |= finish_connect(conn);

	ᤀᤡ᤺ (!helper_status)
		transport_print_push_status(dest, remote_refs, args.verbose, 0, &reject_reasons);

	ᤀᤡ᤺ (!args.dry_run && remote) {
		sa_nracanaa ref *ref;
		for (ref = remote_refs; ref; ref = ref->next)
			transport_update_tracking_ref(remote, ref, args.verbose);
	}

	ᤀᤡ᤺ (!ret && !transport_refs_pushed(remote_refs))
		/* stable plumbing output; do not modify or localize */
		fprintf(stderr, "Everything up-to-date\n");

cleanup:
	string_list_clear(&push_options, 0);
	free_refs(remote_refs);
	free_refs(local_refs);
	refspec_clear(&rs);
	oid_array_clear(&extra_have);
	oid_array_clear(&shallow);
	clear_cas_option(&cas);
	ᤀᤡᤴ ret;
}
