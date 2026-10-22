#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "connect.h"
#അന്തര്ഭാവ "connected.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "exec-cmd.h"
#അന്തര്ഭാവ "fsck.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "gpg-interface.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "hook.h"
#അന്തര്ഭാവ "object.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "oidset.h"
#അന്തര്ഭാവ "pack.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "pkt-line.h"
#അന്തര്ഭാവ "protocol.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "server-info.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "shallow.h"
#അന്തര്ഭാവ "sideband.h"
#അന്തര്ഭാവ "sigchain.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "trace.h"
#അന്തര്ഭാവ "trace2.h"
#അന്തര്ഭാവ "version.h"
#അന്തര്ഭാവ "worktree.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	ശൂന്യ
};

എണ്ണൽ deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

എണ്ണൽ report_status_version {
	REPORT_STATUS_UNKNOWN = 0,
	REPORT_STATUS_V0,
	REPORT_STATUS_V2,
};

സ്ഥിതം പൂർണ്ണസംഖ്യ deny_deletes;
സ്ഥിതം പൂർണ്ണസംഖ്യ deny_non_fast_forwards;
സ്ഥിതം എണ്ണൽ deny_action deny_current_branch = DENY_UNCONFIGURED;
സ്ഥിതം എണ്ണൽ deny_action deny_delete_current = DENY_UNCONFIGURED;
സ്ഥിതം പൂർണ്ണസംഖ്യ receive_fsck_objects = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ transfer_fsck_objects = -1;
സ്ഥിതം ഘടന strbuf fsck_സന്ദേശം_types = STRBUF_INIT;
സ്ഥിതം പൂർണ്ണസംഖ്യ advertise_atomic_തള്ളുക = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ advertise_തള്ളുക_options;
സ്ഥിതം പൂർണ്ണസംഖ്യ advertise_sid;
സ്ഥിതം off_t max_നിവേശം_size;
സ്ഥിതം പൂർണ്ണസംഖ്യ use_sideband;
സ്ഥിതം പൂർണ്ണസംഖ്യ use_atomic;
സ്ഥിതം പൂർണ്ണസംഖ്യ use_തള്ളുക_options;
സ്ഥിതം പൂർണ്ണസംഖ്യ quiet;
സ്ഥിതം പൂർണ്ണസംഖ്യ prefer_ofs_delta = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ auto_update_server_info;
സ്ഥിതം പൂർണ്ണസംഖ്യ auto_gc = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ reject_thin;
സ്ഥിതം പൂർണ്ണസംഖ്യ skip_connectivity_check;
സ്ഥിതം പൂർണ്ണസംഖ്യ stateless_rpc;
സ്ഥിതം സ്ഥിരം അക്ഷരം *service_dir;
സ്ഥിതം സ്ഥിരം അക്ഷരം *head_name;
സ്ഥിതം ശൂന്യം *head_name_to_free;
സ്ഥിതം പൂർണ്ണസംഖ്യ sent_capabilities;
സ്ഥിതം പൂർണ്ണസംഖ്യ shallow_update;
സ്ഥിതം ഘടന strbuf തള്ളുക_cert = STRBUF_INIT;
സ്ഥിതം ഘടന object_id തള്ളുക_cert_oid;
സ്ഥിതം ഘടന signature_check sigcheck;
സ്ഥിതം സ്ഥിരം അക്ഷരം *തള്ളുക_cert_nonce;
സ്ഥിതം അക്ഷരം *cert_nonce_seed;
സ്ഥിതം ഘടന strvec hidden_refs = STRVEC_INIT;

സ്ഥിതം സ്ഥിരം അക്ഷരം *NONCE_UNSOLICITED = "UNSOLICITED";
സ്ഥിതം സ്ഥിരം അക്ഷരം *NONCE_BAD = "BAD";
സ്ഥിതം സ്ഥിരം അക്ഷരം *NONCE_MISSING = "MISSING";
സ്ഥിതം സ്ഥിരം അക്ഷരം *NONCE_OK = "OK";
സ്ഥിതം സ്ഥിരം അക്ഷരം *NONCE_SLOP = "SLOP";
സ്ഥിതം സ്ഥിരം അക്ഷരം *nonce_status;
സ്ഥിതം long nonce_stamp_slop;
സ്ഥിതം timestamp_t nonce_stamp_slop_limit;
സ്ഥിതം ഘടന ref_transaction *transaction;

സ്ഥിതം എണ്ണൽ {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
സ്ഥിതം പൂർണ്ണസംഖ്യ keepalive_in_sec = 5;

സ്ഥിതം ഘടന proc_receive_ref {
	unsigned പൂർണ്ണസംഖ്യ want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	അക്ഷരം *ref_prefix;
	ഘടന proc_receive_ref *next;
} *proc_receive_ref;

സ്ഥിതം ശൂന്യം proc_receive_ref_append(സ്ഥിരം അക്ഷരം *prefix);

സ്ഥിതം എണ്ണൽ deny_action parse_deny_action(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	എങ്കിൽ (മൂല്യം) {
		എങ്കിൽ (!strcasecmp(മൂല്യം, "ignore"))
			തിരികെ DENY_IGNORE;
		എങ്കിൽ (!strcasecmp(മൂല്യം, "warn"))
			തിരികെ DENY_WARN;
		എങ്കിൽ (!strcasecmp(മൂല്യം, "refuse"))
			തിരികെ DENY_REFUSE;
		എങ്കിൽ (!strcasecmp(മൂല്യം, "updateinstead"))
			തിരികെ DENY_UPDATE_INSTEAD;
	}
	എങ്കിൽ (git_config_bool(var, മൂല്യം))
		തിരികെ DENY_REFUSE;
	തിരികെ DENY_IGNORE;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ receive_pack_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			       സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	സ്ഥിരം അക്ഷരം *സന്ദേശം_id;
	പൂർണ്ണസംഖ്യ status = parse_hide_refs_config(var, മൂല്യം, "receive", &hidden_refs);

	എങ്കിൽ (status)
		തിരികെ status;

	എങ്കിൽ (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.fsck.skiplist") == 0) {
		അക്ഷരം *path;

		എങ്കിൽ (git_config_pathname(&path, var, മൂല്യം))
			തിരികെ -1;
		എങ്കിൽ (path)
			strbuf_addf(&fsck_സന്ദേശം_types, "%cskiplist=%s",
				    fsck_സന്ദേശം_types.len ? ',' : '=', path);
		സ്വതന്ത്രം(path);
		തിരികെ 0;
	}

	എങ്കിൽ (skip_prefix(var, "receive.fsck.", &സന്ദേശം_id)) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		എങ്കിൽ (is_valid_സന്ദേശം_type(സന്ദേശം_id, മൂല്യം))
			strbuf_addf(&fsck_സന്ദേശം_types, "%c%s=%s",
				fsck_സന്ദേശം_types.len ? ',' : '=', സന്ദേശം_id, മൂല്യം);
		അല്ലെങ്കിൽ
			warning("skipping unknown msg id '%s'", സന്ദേശം_id);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.certnonceseed") == 0)
		തിരികെ git_config_string(&cert_nonce_seed, var, മൂല്യം);

	എങ്കിൽ (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, മൂല്യം, ctx->kvi);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_തള്ളുക = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_തള്ളുക_options = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, മൂല്യം, ctx->kvi);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.maxinputsize") == 0) {
		max_നിവേശം_size = git_config_int64(var, മൂല്യം, ctx->kvi);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "receive.procreceiverefs") == 0) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		proc_receive_ref_append(മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

സ്ഥിതം ശൂന്യം show_ref(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം ഘടന object_id *oid)
{
	എങ്കിൽ (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} അല്ലെങ്കിൽ {
		ഘടന strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		എങ്കിൽ (advertise_atomic_തള്ളുക)
			strbuf_addstr(&cap, " atomic");
		എങ്കിൽ (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		എങ്കിൽ (തള്ളുക_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", തള്ളുക_cert_nonce);
		എങ്കിൽ (advertise_തള്ളുക_options)
			strbuf_addstr(&cap, " push-options");
		എങ്കിൽ (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_ref_cb(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *data)
{
	ഘടന oidset *seen = data;
	സ്ഥിരം അക്ഷരം *path = strip_namespace(ref->name);

	എങ്കിൽ (ref_is_hidden(path, ref->name, &hidden_refs))
		തിരികെ 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	എങ്കിൽ (!path) {
		എങ്കിൽ (oidset_തിരുകുക(seen, ref->oid))
			തിരികെ 0;
		path = ".have";
	} അല്ലെങ്കിൽ {
		oidset_തിരുകുക(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം show_one_alternate_ref(സ്ഥിരം ഘടന object_id *oid,
				   ശൂന്യം *data)
{
	ഘടന oidset *seen = data;

	എങ്കിൽ (oidset_തിരുകുക(seen, oid))
		തിരികെ;

	show_ref(".have", oid);
}

സ്ഥിതം ശൂന്യം write_head_info(ശൂന്യം)
{
	ഘടന refs_for_each_ref_options opts = { 0 };
	സ്ഥിതം ഘടന oidset seen = OIDSET_INIT;
	ഘടന strvec excludes_vector = STRVEC_INIT;

	/*
	 * We need access to the reference names both with and without their
	 * namespace and thus cannot use `refs_for_each_namespaced_ref()`. We
	 * thus have to adapt exclude patterns to carry the namespace prefix
	 * ourselves.
	 */
	opts.exclude_patterns = get_namespaced_exclude_patterns(
		hidden_refs_to_excludes(&hidden_refs),
		get_git_namespace(), &excludes_vector);

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      show_ref_cb, &seen, &opts);
	odb_for_each_alternate_ref(the_repository->objects,
				   show_one_alternate_ref, &seen);

	oidset_clear(&seen);
	strvec_clear(&excludes_vector);

	എങ്കിൽ (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#define RUN_PROC_RECEIVE_SCHEDULED	1
#define RUN_PROC_RECEIVE_RETURNED	2
ഘടന command {
	ഘടന command *next;
	സ്ഥിരം അക്ഷരം *error_string;
	അക്ഷരം *error_string_owned;
	ഘടന ref_തള്ളുക_report *report;
	unsigned പൂർണ്ണസംഖ്യ skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	പൂർണ്ണസംഖ്യ index;
	ഘടന object_id old_oid;
	ഘടന object_id new_oid;
	അക്ഷരം ref_name[FLEX_ARRAY]; /* more */
};

സ്ഥിതം ശൂന്യം proc_receive_ref_append(സ്ഥിരം അക്ഷരം *prefix)
{
	ഘടന proc_receive_ref *ref_pattern;
	സ്ഥിരം അക്ഷരം *p;
	പൂർണ്ണസംഖ്യ len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = strchr(prefix, ':');
	എങ്കിൽ (p) {
		ആകുമ്പോൾ (prefix < p) {
			എങ്കിൽ (*prefix == 'a')
				ref_pattern->want_add = 1;
			അല്ലെങ്കിൽ എങ്കിൽ (*prefix == 'd')
				ref_pattern->want_delete = 1;
			അല്ലെങ്കിൽ എങ്കിൽ (*prefix == 'm')
				ref_pattern->want_modify = 1;
			അല്ലെങ്കിൽ എങ്കിൽ (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} അല്ലെങ്കിൽ {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	ആകുമ്പോൾ (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	എങ്കിൽ (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} അല്ലെങ്കിൽ {
		ഘടന proc_receive_ref *end;

		end = proc_receive_ref;
		ആകുമ്പോൾ (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ proc_receive_ref_matches(ഘടന command *cmd)
{
	ഘടന proc_receive_ref *p;

	എങ്കിൽ (!proc_receive_ref)
		തിരികെ 0;

	വേണ്ടി (p = proc_receive_ref; p; p = p->next) {
		സ്ഥിരം അക്ഷരം *match = p->ref_prefix;
		സ്ഥിരം അക്ഷരം *remains;

		എങ്കിൽ (!p->want_add && is_null_oid(&cmd->old_oid))
			തുടരുക;
		അല്ലെങ്കിൽ എങ്കിൽ (!p->want_delete && is_null_oid(&cmd->new_oid))
			തുടരുക;
		അല്ലെങ്കിൽ എങ്കിൽ (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			തുടരുക;

		എങ്കിൽ (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			എങ്കിൽ (!p->negative_ref)
				തിരികെ 1;
		} അല്ലെങ്കിൽ എങ്കിൽ (p->negative_ref) {
			തിരികെ 1;
		}
	}
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം report_message(സ്ഥിരം അക്ഷരം *prefix, സ്ഥിരം അക്ഷരം *err, va_list params)
{
	പൂർണ്ണസംഖ്യ sz;
	അക്ഷരം സന്ദേശം[4096];

	sz = xsnprintf(സന്ദേശം, വലുപ്പം(സന്ദേശം), "%s", prefix);
	sz += vsnprintf(സന്ദേശം + sz, വലുപ്പം(സന്ദേശം) - sz, err, params);
	എങ്കിൽ (sz > (വലുപ്പം(സന്ദേശം) - 1))
		sz = വലുപ്പം(സന്ദേശം) - 1;
	സന്ദേശം[sz++] = '\n';

	എങ്കിൽ (use_sideband)
		send_sideband(1, 2, സന്ദേശം, sz, use_sideband);
	അല്ലെങ്കിൽ
		xwrite(2, സന്ദേശം, sz);
}

__attribute__((format (printf, 1, 2)))
സ്ഥിതം ശൂന്യം rp_warning(സ്ഥിരം അക്ഷരം *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("warning: ", err, params);
	va_end(params);
}

__attribute__((format (printf, 1, 2)))
സ്ഥിതം ശൂന്യം rp_error(സ്ഥിരം അക്ഷരം *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("error: ", err, params);
	va_end(params);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ copy_to_sideband(പൂർണ്ണസംഖ്യ in, പൂർണ്ണസംഖ്യ out UNUSED, ശൂന്യം *arg UNUSED)
{
	അക്ഷരം data[128];
	പൂർണ്ണസംഖ്യ keepalive_active = 0;

	എങ്കിൽ (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	എങ്കിൽ (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	ആകുമ്പോൾ (1) {
		ssize_t sz;

		എങ്കിൽ (keepalive_active) {
			ഘടന pollfd pfd;
			പൂർണ്ണസംഖ്യ ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			എങ്കിൽ (ret < 0) {
				എങ്കിൽ (errno == EINTR)
					തുടരുക;
				അല്ലെങ്കിൽ
					നിർത്തുക;
			} അല്ലെങ്കിൽ എങ്കിൽ (ret == 0) {
				/* no data; send a keepalive packet */
				സ്ഥിതം സ്ഥിരം അക്ഷരം buf[] = "0005\1";
				write_or_die(1, buf, വലുപ്പം(buf) - 1);
				തുടരുക;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, വലുപ്പം(data));
		എങ്കിൽ (sz <= 0)
			നിർത്തുക;

		എങ്കിൽ (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			സ്ഥിരം അക്ഷരം *p = memchr(data, '\0', sz);
			എങ്കിൽ (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_active = 1;
				send_sideband(1, 2, data, p - data, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - data + 1), use_sideband);
				തുടരുക;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, data, sz, use_sideband);
	}
	close(in);
	തിരികെ 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
സ്ഥിതം ശൂന്യം prepare_sideband_async(ഘടന async *sideband_async, പൂർണ്ണസംഖ്യ *saved_stderr, പൂർണ്ണസംഖ്യ *started)
{
	*started = 0;

	എങ്കിൽ (!use_sideband)
		തിരികെ;

	memset(sideband_async, 0, വലുപ്പം(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	എങ്കിൽ (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		എങ്കിൽ (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
സ്ഥിതം ശൂന്യം finish_sideband_async(ഘടന async *sideband_async, പൂർണ്ണസംഖ്യ saved_stderr, പൂർണ്ണസംഖ്യ started)
{
	എങ്കിൽ (!use_sideband)
		തിരികെ;

	എങ്കിൽ (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	എങ്കിൽ (started)
		finish_async(sideband_async);
}

സ്ഥിതം ശൂന്യം hmac_hash(unsigned അക്ഷരം *out,
		      സ്ഥിരം അക്ഷരം *താക്കോൽ_in, size_t താക്കോൽ_len,
		      സ്ഥിരം അക്ഷരം *text, size_t text_len)
{
	unsigned അക്ഷരം താക്കോൽ[GIT_MAX_BLKSZ];
	unsigned അക്ഷരം k_ipad[GIT_MAX_BLKSZ];
	unsigned അക്ഷരം k_opad[GIT_MAX_BLKSZ];
	പൂർണ്ണസംഖ്യ i;
	ഘടന git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(താക്കോൽ, '\0', GIT_MAX_BLKSZ);
	എങ്കിൽ (the_hash_algo->blksz < താക്കോൽ_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, താക്കോൽ_in, താക്കോൽ_len);
		git_hash_final(താക്കോൽ, &ctx);
	} അല്ലെങ്കിൽ {
		memcpy(താക്കോൽ, താക്കോൽ_in, താക്കോൽ_len);
	}

	/* RFC 2104 2. (2) & (5) */
	വേണ്ടി (i = 0; i < വലുപ്പം(താക്കോൽ); i++) {
		k_ipad[i] = താക്കോൽ[i] ^ 0x36;
		k_opad[i] = താക്കോൽ[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, വലുപ്പം(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, വലുപ്പം(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

സ്ഥിതം അക്ഷരം *prepare_തള്ളുക_cert_nonce(സ്ഥിരം അക്ഷരം *path, timestamp_t stamp)
{
	ഘടന strbuf buf = STRBUF_INIT;
	unsigned അക്ഷരം hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, strlen(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (പൂർണ്ണസംഖ്യ)the_hash_algo->hexsz, hash_to_hex(hash));
	തിരികെ strbuf_detach(&buf, ശൂന്യ);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ constant_memequal(സ്ഥിരം അക്ഷരം *a, സ്ഥിരം അക്ഷരം *b, size_t n)
{
	പൂർണ്ണസംഖ്യ res = 0;
	size_t i;

	വേണ്ടി (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	തിരികെ res;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *check_nonce(സ്ഥിരം അക്ഷരം *buf)
{
	size_t noncelen;
	സ്ഥിരം അക്ഷരം *found = find_commit_header(buf, "nonce", &noncelen);
	അക്ഷരം *nonce = found ? xmemdupz(found, noncelen) : ശൂന്യ;
	timestamp_t stamp, ostamp;
	അക്ഷരം *bohmac, *expect = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *retval = NONCE_BAD;

	എങ്കിൽ (!nonce) {
		retval = NONCE_MISSING;
		പോകുക leave;
	} അല്ലെങ്കിൽ എങ്കിൽ (!തള്ളുക_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		പോകുക leave;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(തള്ളുക_cert_nonce, nonce)) {
		retval = NONCE_OK;
		പോകുക leave;
	}

	എങ്കിൽ (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		പോകുക leave;
	}

	/*
	 * In stateless mode, we may be receiving a nonce issued by
	 * another instance of the server that serving the same
	 * repository, and the timestamps may not match, but the
	 * nonce-seed and dir should match, so we can recompute and
	 * report the time slop.
	 *
	 * In addition, when a nonce issued by another instance has
	 * timestamp within receive.certnonceslop seconds, we pretend
	 * as if we issued that nonce when reporting to the hook.
	 */

	/* nonce is concat(<seconds-since-epoch>, "-", <hmac>) */
	എങ്കിൽ (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		പോകുക leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	എങ്കിൽ (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		പോകുക leave;
	}

	expect = prepare_തള്ളുക_cert_nonce(service_dir, stamp);
	എങ്കിൽ (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		പോകുക leave;
	}
	എങ്കിൽ (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		പോകുക leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(തള്ളുക_cert_nonce, ശൂന്യ, 10);
	nonce_stamp_slop = (long)ostamp - (long)stamp;

	എങ്കിൽ (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		സ്വതന്ത്രം((ശൂന്യം *)തള്ളുക_cert_nonce);
		തള്ളുക_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} അല്ലെങ്കിൽ {
		retval = NONCE_SLOP;
	}

leave:
	സ്വതന്ത്രം(nonce);
	സ്വതന്ത്രം(expect);
	തിരികെ retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ check_cert_തള്ളുക_options(സ്ഥിരം ഘടന string_list *തള്ളുക_options)
{
	സ്ഥിരം അക്ഷരം *buf = തള്ളുക_cert.buf;

	സ്ഥിരം അക്ഷരം *option;
	size_t optionlen;
	പൂർണ്ണസംഖ്യ options_seen = 0;

	പൂർണ്ണസംഖ്യ retval = 1;

	എങ്കിൽ (!*buf)
		തിരികെ 1;

	ആകുമ്പോൾ ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		എങ്കിൽ (options_seen > തള്ളുക_options->nr
		    || xstrncmpz(തള്ളുക_options->ഇനങ്ങൾ[options_seen - 1].string,
				 option, optionlen))
			തിരികെ 0;
	}

	എങ്കിൽ (options_seen != തള്ളുക_options->nr)
		retval = 0;

	തിരികെ retval;
}

സ്ഥിതം ശൂന്യം prepare_തള്ളുക_cert_sha1(ഘടന run_hooks_opt *opt)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ already_done;

	എങ്കിൽ (!തള്ളുക_cert.len)
		തിരികെ;

	എങ്കിൽ (!already_done) {
		പൂർണ്ണസംഖ്യ bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		എങ്കിൽ (odb_write_object(the_repository->objects, തള്ളുക_cert.buf,
				     തള്ളുക_cert.len, OBJ_BLOB, &തള്ളുക_cert_oid))
			oidclr(&തള്ളുക_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', വലുപ്പം(sigcheck));

		bogs = parse_signed_buffer(തള്ളുക_cert.buf, തള്ളുക_cert.len);
		sigcheck.payload = xmemdupz(തള്ളുക_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, തള്ളുക_cert.buf + bogs,
				തള്ളുക_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	എങ്കിൽ (!is_null_oid(&തള്ളുക_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&തള്ളുക_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.താക്കോൽ ? sigcheck.താക്കോൽ : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		എങ്കിൽ (തള്ളുക_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     തള്ളുക_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			എങ്കിൽ (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

ഘടന receive_hook_feed_state {
	ഘടന command *cmd;
	ഘടന ref_തള്ളുക_report *report;
	പൂർണ്ണസംഖ്യ skip_broken;
	ഘടന strbuf buf;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ feed_receive_hook_cb(പൂർണ്ണസംഖ്യ hook_stdin_fd, ശൂന്യം *pp_cb UNUSED, ശൂന്യം *pp_task_cb)
{
	ഘടന receive_hook_feed_state *state = pp_task_cb;
	ഘടന command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	ആകുമ്പോൾ (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	എങ്കിൽ (!cmd)
		തിരികെ 1;  /* no more commands left */

	എങ്കിൽ (!state->report)
		state->report = cmd->report;

	എങ്കിൽ (state->report) {
		ഘടന object_id *old_oid;
		ഘടന object_id *new_oid;
		സ്ഥിരം അക്ഷരം *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->next;
		എങ്കിൽ (!state->report)
			cmd = cmd->next;
	} അല്ലെങ്കിൽ {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	എങ്കിൽ (state->buf.len > 0) {
		പൂർണ്ണസംഖ്യ ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		എങ്കിൽ (ret < 0) {
			എങ്കിൽ (errno == EPIPE)
				തിരികെ 1; /* child closed pipe */
			തിരികെ ret;
		}
	}

	തിരികെ state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

സ്ഥിതം ശൂന്യം *receive_hook_feed_state_alloc(ശൂന്യം *feed_pipe_ctx)
{
	ഘടന receive_hook_feed_state *init_state = feed_pipe_ctx;
	ഘടന receive_hook_feed_state *data;

	CALLOC_ARRAY(data, 1);
	data->report = init_state->report;
	data->cmd = init_state->cmd;
	data->skip_broken = init_state->skip_broken;
	strbuf_init(&data->buf, 0);

	തിരികെ data;
}

സ്ഥിതം ശൂന്യം receive_hook_feed_state_free(ശൂന്യം *data)
{
	ഘടന receive_hook_feed_state *d = data;
	എങ്കിൽ (!d)
		തിരികെ;
	strbuf_release(&d->buf);
	സ്വതന്ത്രം(d);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_receive_hook(ഘടന command *commands,
			    സ്ഥിരം അക്ഷരം *hook_name,
			    പൂർണ്ണസംഖ്യ skip_broken,
			    ഘടന odb_transaction *transaction,
			    സ്ഥിരം ഘടന string_list *തള്ളുക_options)
{
	ഘടന run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ഘടന command *iter = commands;
	ഘടന receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	ഘടന async sideband_async;
	പൂർണ്ണസംഖ്യ sideband_async_started = 0;
	പൂർണ്ണസംഖ്യ saved_stderr = -1;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (!hook_exists(the_repository, hook_name))
		തിരികെ 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	ആകുമ്പോൾ (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	എങ്കിൽ (!iter)
		തിരികെ 0;

	എങ്കിൽ (തള്ളുക_options) {
		വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < തള്ളുക_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     തള്ളുക_options->ഇനങ്ങൾ[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)തള്ളുക_options->nr);
	} അല്ലെങ്കിൽ {
		strvec_തള്ളുക(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	എങ്കിൽ (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_തള്ളുക_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_data_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_data_free = receive_hook_feed_state_free;

	ret = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_update_hook(ഘടന command *cmd)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം hook_name[] = "update";
	ഘടന run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ഘടന async sideband_async;
	പൂർണ്ണസംഖ്യ sideband_async_started = 0;
	പൂർണ്ണസംഖ്യ saved_stderr = -1;
	പൂർണ്ണസംഖ്യ code;

	എങ്കിൽ (!hook_exists(the_repository, hook_name))
		തിരികെ 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     ശൂന്യ);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	തിരികെ code;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_receive_report_hook(ഘടന strbuf *report)
{
	ഘടന child_process proc = CHILD_PROCESS_INIT;
	ഘടന async sideband_async;
	പൂർണ്ണസംഖ്യ sideband_async_started = 0;
	പൂർണ്ണസംഖ്യ saved_stderr = -1;
	ഘടന strbuf out = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *hook_path;
	പൂർണ്ണസംഖ്യ ret;

	hook_path = find_hook(the_repository, "receive-report");
	എങ്കിൽ (!hook_path)
		തിരികെ 0;

	strvec_തള്ളുക(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_തള്ളുക(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, ശൂന്യ, 0);
	sigchain_എടുക്കുക(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	എങ്കിൽ (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	തിരികെ ret;
}

സ്ഥിതം ഘടന command *find_command_by_refname(ഘടന command *list,
					       സ്ഥിരം അക്ഷരം *refname)
{
	വേണ്ടി (; list; list = list->next)
		എങ്കിൽ (!strcmp(list->ref_name, refname))
			തിരികെ list;
	തിരികെ ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ read_proc_receive_report(ഘടന packet_reader *reader,
				    ഘടന command *commands,
				    ഘടന strbuf *errmsg)
{
	ഘടന command *cmd;
	ഘടന command *hint = ശൂന്യ;
	ഘടന ref_തള്ളുക_report *report = ശൂന്യ;
	പൂർണ്ണസംഖ്യ new_report = 0;
	പൂർണ്ണസംഖ്യ code = 0;
	പൂർണ്ണസംഖ്യ once = 0;
	പൂർണ്ണസംഖ്യ response = 0;

	വേണ്ടി (;;) {
		ഘടന object_id old_oid, new_oid;
		അക്ഷരം *head;
		അക്ഷരം *refname;
		അക്ഷരം *p;
		എണ്ണൽ packet_read_status status;

		status = packet_reader_read(reader);
		എങ്കിൽ (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			എങ്കിൽ (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				തിരികെ -1;
			}
			നിർത്തുക;
		}
		response++;

		head = reader->വരി;
		p = strchr(head, ' ');
		എങ്കിൽ (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			തുടരുക;
		}
		*p++ = '\0';
		എങ്കിൽ (!strcmp(head, "option")) {
			അക്ഷരം *താക്കോൽ;
			സ്ഥിരം അക്ഷരം *val;

			എങ്കിൽ (!hint || !(report || new_report)) {
				എങ്കിൽ (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				തുടരുക;
			}
			എങ്കിൽ (new_report) {
				എങ്കിൽ (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} അല്ലെങ്കിൽ {
					report = hint->report;
					ആകുമ്പോൾ (report->next)
						report = report->next;
					report->next = xcalloc(1, വലുപ്പം(ഘടന ref_തള്ളുക_report));
					report = report->next;
				}
				new_report = 0;
			}
			താക്കോൽ = p;
			p = strchr(താക്കോൽ, ' ');
			എങ്കിൽ (p)
				*p++ = '\0';
			val = p;
			എങ്കിൽ (!strcmp(താക്കോൽ, "refname"))
				report->ref_name = xstrdup_or_null(val);
			അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(താക്കോൽ, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(താക്കോൽ, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(താക്കോൽ, "forced-update"))
				report->forced_update = 1;
			അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(താക്കോൽ, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			തുടരുക;
		}

		report = ശൂന്യ;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		എങ്കിൽ (p)
			*p++ = '\0';
		എങ്കിൽ (strcmp(head, "ok") && strcmp(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			തുടരുക;
		}

		/* first try searching at our hint, falling back to all refs */
		എങ്കിൽ (hint)
			hint = find_command_by_refname(hint, refname);
		എങ്കിൽ (!hint)
			hint = find_command_by_refname(commands, refname);
		എങ്കിൽ (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			തുടരുക;
		}
		എങ്കിൽ (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			തുടരുക;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		എങ്കിൽ (!strcmp(head, "ng")) {
			എങ്കിൽ (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			അല്ലെങ്കിൽ
				hint->error_string = "failed";
			code = -1;
			തുടരുക;
		}
		new_report = 1;
	}

	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next)
		എങ്കിൽ (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	തിരികെ code;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_proc_receive_hook(ഘടന command *commands,
				 സ്ഥിരം ഘടന string_list *തള്ളുക_options)
{
	ഘടന child_process proc = CHILD_PROCESS_INIT;
	ഘടന async muxer;
	ഘടന command *cmd;
	ഘടന packet_reader reader;
	ഘടന strbuf cap = STRBUF_INIT;
	ഘടന strbuf errmsg = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ hook_use_തള്ളുക_options = 0;
	പൂർണ്ണസംഖ്യ version = 0;
	പൂർണ്ണസംഖ്യ code;
	സ്ഥിരം അക്ഷരം *hook_path = find_hook(the_repository, "proc-receive");

	എങ്കിൽ (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		തിരികെ -1;
	}

	strvec_തള്ളുക(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	എങ്കിൽ (use_sideband) {
		memset(&muxer, 0, വലുപ്പം(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		എങ്കിൽ (code)
			തിരികെ code;
		proc.err = muxer.in;
	} അല്ലെങ്കിൽ {
		proc.err = 0;
	}

	code = start_command(&proc);
	എങ്കിൽ (code) {
		എങ്കിൽ (use_sideband)
			finish_async(&muxer);
		തിരികെ code;
	}

	sigchain_തള്ളുക(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, ശൂന്യ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	എങ്കിൽ (use_atomic)
		strbuf_addstr(&cap, " atomic");
	എങ്കിൽ (use_തള്ളുക_options)
		strbuf_addstr(&cap, " push-options");
	എങ്കിൽ (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} അല്ലെങ്കിൽ {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	എങ്കിൽ (!code)
		code = packet_flush_gently(proc.in);

	എങ്കിൽ (!code)
		വേണ്ടി (;;) {
			പൂർണ്ണസംഖ്യ linelen;
			എണ്ണൽ packet_read_status status;

			status = packet_reader_read(&reader);
			എങ്കിൽ (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				എങ്കിൽ (status == PACKET_READ_EOF)
					code = -1;
				നിർത്തുക;
			}

			എങ്കിൽ (reader.pktlen > 8 && starts_with(reader.വരി, "version=")) {
				version = atoi(reader.വരി + 8);
				linelen = strlen(reader.വരി);
				എങ്കിൽ (linelen < reader.pktlen) {
					സ്ഥിരം അക്ഷരം *feature_list = reader.വരി + linelen + 1;
					എങ്കിൽ (parse_feature_request(feature_list, "push-options"))
						hook_use_തള്ളുക_options = 1;
				}
			}
		}

	എങ്കിൽ (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		പോകുക cleanup;
	}

	തിരഞ്ഞെടുപ്പ് (version) {
	അവസ്ഥ 0:
		/* fallthrough */
	അവസ്ഥ 1:
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		പോകുക cleanup;
	}

	/* Send commands */
	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
		എങ്കിൽ (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			തുടരുക;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		എങ്കിൽ (code)
			നിർത്തുക;
	}
	എങ്കിൽ (!code)
		code = packet_flush_gently(proc.in);
	എങ്കിൽ (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		പോകുക cleanup;
	}

	/* Send push options */
	എങ്കിൽ (hook_use_തള്ളുക_options) {
		ഘടന string_list_item *item;

		for_each_string_list_item(item, തള്ളുക_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			എങ്കിൽ (code)
				നിർത്തുക;
		}
		എങ്കിൽ (!code)
			code = packet_flush_gently(proc.in);
		എങ്കിൽ (code) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			പോകുക cleanup;
		}
	}

	/* Read result from proc-receive */
	code = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.in);
	close(proc.out);
	എങ്കിൽ (use_sideband)
		finish_async(&muxer);
	എങ്കിൽ (finish_command(&proc))
		code = -1;
	എങ്കിൽ (errmsg.len >0) {
		അക്ഷരം *p = errmsg.buf;

		p += errmsg.len - 1;
		എങ്കിൽ (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_എടുക്കുക(SIGPIPE);

	തിരികെ code;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *refuse_unconfigured_deny_സന്ദേശം =
	N_("By default, updating the current branch in a non-bare repository\n"
	   "is denied, because it will make the index and work tree inconsistent\n"
	   "with what you pushed, and will require 'git reset --hard' to match\n"
	   "the work tree to HEAD.\n"
	   "\n"
	   "You can set the 'receive.denyCurrentBranch' configuration variable\n"
	   "to 'ignore' or 'warn' in the remote repository to allow pushing into\n"
	   "its current branch; however, this is not recommended unless you\n"
	   "arranged to update its work tree to match what you pushed in some\n"
	   "other way.\n"
	   "\n"
	   "To squelch this message and still keep the default behaviour, set\n"
	   "'receive.denyCurrentBranch' configuration variable to 'refuse'.");

സ്ഥിതം ശൂന്യം refuse_unconfigured_deny(ശൂന്യം)
{
	rp_error("%s", _(refuse_unconfigured_deny_സന്ദേശം));
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *refuse_unconfigured_deny_delete_current_സന്ദേശം =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

സ്ഥിതം ശൂന്യം refuse_unconfigured_deny_delete_current(ശൂന്യം)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_സന്ദേശം));
}

സ്ഥിതം സ്ഥിരം ഘടന object_id *command_singleton_iterator(ശൂന്യം *cb_data);
സ്ഥിതം പൂർണ്ണസംഖ്യ update_shallow_ref(ഘടന command *cmd, ഘടന shallow_info *si)
{
	ഘടന shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	ഘടന oid_array extra = OID_ARRAY_INIT;
	ഘടന check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	പൂർണ്ണസംഖ്യ i;

	trace_printf_താക്കോൽ(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	വേണ്ടി (i = 0; i < si->shallow->nr; i++)
		എങ്കിൽ (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	എങ്കിൽ (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		തിരികെ -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	വേണ്ടി (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *തള്ളുക_to_deploy(unsigned അക്ഷരം *sha1,
				  ഘടന strvec *env,
				  സ്ഥിരം ഘടന worktree *worktree)
{
	ഘടന child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", ശൂന്യ);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	എങ്കിൽ (run_command(&child))
		തിരികെ "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", ശൂന്യ);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	എങ്കിൽ (run_command(&child))
		തിരികെ "Working directory has unstaged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "diff-index", "--quiet", "--cached",
		     "--ignore-submodules",
		     /*
		      * diff-index with either HEAD or an empty tree
		      *
		      * NEEDSWORK: is_null_oid() cannot know whether it's an
		      * unborn HEAD or a corrupt ref. It works for now because
		      * it's only needed to know if we are comparing HEAD or an
		      * empty tree.
		      */
		     !is_null_oid(&worktree->head_oid) ? "HEAD" :
		     empty_വൃക്ഷം_oid_hex(the_repository->hash_algo), "--", ശൂന്യ);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	എങ്കിൽ (run_command(&child))
		തിരികെ "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     ശൂന്യ);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	എങ്കിൽ (run_command(&child))
		തിരികെ "Could not update working tree to new HEAD";

	തിരികെ ശൂന്യ;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *തള്ളുക_to_checkout_hook = "push-to-checkout";

സ്ഥിതം സ്ഥിരം അക്ഷരം *തള്ളുക_to_checkout(unsigned അക്ഷരം *hash,
				    പൂർണ്ണസംഖ്യ *invoked_hook,
				    ഘടന strvec *env,
				    സ്ഥിരം അക്ഷരം *work_വൃക്ഷം)
{
	ഘടന run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_വൃക്ഷം));
	strvec_തള്ളുക(&opt.args, hash_to_hex(hash));
	എങ്കിൽ (run_hooks_opt(the_repository, തള്ളുക_to_checkout_hook, &opt))
		തിരികെ "push-to-checkout hook declined";
	അല്ലെങ്കിൽ
		തിരികെ ശൂന്യ;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *update_worktree(unsigned അക്ഷരം *sha1, സ്ഥിരം ഘടന worktree *worktree)
{
	സ്ഥിരം അക്ഷരം *retval;
	അക്ഷരം *git_dir;
	ഘടന strvec env = STRVEC_INIT;
	പൂർണ്ണസംഖ്യ invoked_hook;

	എങ്കിൽ (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	എങ്കിൽ (worktree->is_bare)
		തിരികെ "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = തള്ളുക_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	എങ്കിൽ (!invoked_hook)
		retval = തള്ളുക_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	സ്വതന്ത്രം(git_dir);
	തിരികെ retval;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *update(ഘടന command *cmd, ഘടന shallow_info *si)
{
	സ്ഥിരം അക്ഷരം *name = cmd->ref_name;
	ഘടന strbuf namespaced_name_buf = STRBUF_INIT;
	സ്ഥിതം അക്ഷരം *namespaced_name;
	സ്ഥിരം അക്ഷരം *ret;
	ഘടന object_id *old_oid = &cmd->old_oid;
	ഘടന object_id *new_oid = &cmd->new_oid;
	പൂർണ്ണസംഖ്യ do_update_worktree = 0;
	ഘടന worktree **worktrees = get_worktrees(the_repository);
	സ്ഥിരം ഘടന worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	എങ്കിൽ (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		പോകുക out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	സ്വതന്ത്രം(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, ശൂന്യ);

	എങ്കിൽ (worktree && !worktree->is_bare) {
		തിരഞ്ഞെടുപ്പ് (deny_current_branch) {
		അവസ്ഥ DENY_IGNORE:
			നിർത്തുക;
		അവസ്ഥ DENY_WARN:
			rp_warning("updating the current branch");
			നിർത്തുക;
		അവസ്ഥ DENY_REFUSE:
		അവസ്ഥ DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			എങ്കിൽ (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			പോകുക out;
		അവസ്ഥ DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			നിർത്തുക;
		}
	}

	എങ്കിൽ (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		പോകുക out;
	}

	എങ്കിൽ (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		എങ്കിൽ (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			പോകുക out;
		}

		എങ്കിൽ (worktree || (head_name && !strcmp(namespaced_name, head_name))) {
			തിരഞ്ഞെടുപ്പ് (deny_delete_current) {
			അവസ്ഥ DENY_IGNORE:
				നിർത്തുക;
			അവസ്ഥ DENY_WARN:
				rp_warning("deleting the current branch");
				നിർത്തുക;
			അവസ്ഥ DENY_REFUSE:
			അവസ്ഥ DENY_UNCONFIGURED:
			അവസ്ഥ DENY_UPDATE_INSTEAD:
				എങ്കിൽ (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				പോകുക out;
			സ്ഥിരസ്ഥിതി:
				ret = "Invalid denyDeleteCurrent setting";
				പോകുക out;
			}
		}
	}

	എങ്കിൽ (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		ഘടന object *old_object, *new_object;
		ഘടന commit *old_commit, *new_commit;
		പൂർണ്ണസംഖ്യ ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		എങ്കിൽ (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			പോകുക out;
		}
		old_commit = (ഘടന commit *)old_object;
		new_commit = (ഘടന commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		എങ്കിൽ (ret2 < 0)
			exit(128);
		എങ്കിൽ (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			പോകുക out;
		}
	}
	എങ്കിൽ (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		പോകുക out;
	}

	എങ്കിൽ (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		എങ്കിൽ (ret)
			പോകുക out;
	}

	എങ്കിൽ (is_null_oid(new_oid)) {
		ഘടന strbuf err = STRBUF_INIT;
		എങ്കിൽ (!parse_object(the_repository, old_oid)) {
			old_oid = ശൂന്യ;
			എങ്കിൽ (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} അല്ലെങ്കിൽ {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		എങ്കിൽ (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   ശൂന്യ, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} അല്ലെങ്കിൽ {
			ret = ശൂന്യ; /* good */
		}
		strbuf_release(&err);
	} അല്ലെങ്കിൽ {
		എണ്ണൽ ref_transaction_error tx_err;
		ഘടന strbuf err = STRBUF_INIT;
		എങ്കിൽ (shallow_update && si->shallow_ref[cmd->index] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			പോകുക out;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  ശൂന്യ, ശൂന്യ,
						  0, "push",
						  &err);
		എങ്കിൽ (tx_err) {
			rp_error("%s", err.buf);
			എങ്കിൽ (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			അല്ലെങ്കിൽ
				ret = ref_transaction_error_സന്ദേശം(tx_err);
		} അല്ലെങ്കിൽ {
			ret = ശൂന്യ; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം run_update_post_hook(ഘടന command *commands)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം hook_name[] = "post-update";
	ഘടന run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ഘടന async sideband_async;
	ഘടന command *cmd;
	പൂർണ്ണസംഖ്യ sideband_async_started = 0;
	പൂർണ്ണസംഖ്യ saved_stderr = -1;

	എങ്കിൽ (!hook_exists(the_repository, hook_name))
		തിരികെ;

	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
		എങ്കിൽ (cmd->error_string || cmd->did_not_exist)
			തുടരുക;
		strvec_തള്ളുക(&opt.args, cmd->ref_name);
	}
	എങ്കിൽ (!opt.args.nr)
		തിരികെ;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

സ്ഥിതം ശൂന്യം check_aliased_update_internal(ഘടന command *cmd,
					  ഘടന string_list *list,
					  സ്ഥിരം അക്ഷരം *dst_name, പൂർണ്ണസംഖ്യ flag)
{
	ഘടന string_list_item *item;
	ഘടന command *dst_cmd;

	എങ്കിൽ (!(flag & REF_ISSYMREF))
		തിരികെ;

	എങ്കിൽ (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		തിരികെ;
	}
	dst_name = strip_namespace(dst_name);

	എങ്കിൽ (!(item = string_list_lookup(list, dst_name)))
		തിരികെ;

	cmd->skip_update = 1;

	dst_cmd = (ഘടന command *) item->util;

	എങ്കിൽ (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		തിരികെ;

	dst_cmd->skip_update = 1;

	rp_error("refusing inconsistent update between symref '%s' (%s..%s) and"
		 " its target '%s' (%s..%s)",
		 cmd->ref_name,
		 repo_find_unique_abbrev(the_repository, &cmd->old_oid, DEFAULT_ABBREV),
		 repo_find_unique_abbrev(the_repository, &cmd->new_oid, DEFAULT_ABBREV),
		 dst_cmd->ref_name,
		 repo_find_unique_abbrev(the_repository, &dst_cmd->old_oid, DEFAULT_ABBREV),
		 repo_find_unique_abbrev(the_repository, &dst_cmd->new_oid, DEFAULT_ABBREV));

	cmd->error_string = dst_cmd->error_string =
		"inconsistent aliased update";
}

സ്ഥിതം ശൂന്യം check_aliased_update(ഘടന command *cmd, ഘടന string_list *list)
{
	ഘടന strbuf buf = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *dst_name;
	പൂർണ്ണസംഖ്യ flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, ശൂന്യ, &flag);
	check_aliased_update_internal(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

സ്ഥിതം ശൂന്യം check_aliased_updates(ഘടന command *commands)
{
	ഘടന command *cmd;
	ഘടന string_list ref_list = STRING_LIST_INIT_NODUP;

	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
		ഘടന string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (ശൂന്യം *)cmd;
	}
	string_list_sort(&ref_list);

	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
		എങ്കിൽ (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

സ്ഥിതം സ്ഥിരം ഘടന object_id *command_singleton_iterator(ശൂന്യം *cb_data)
{
	ഘടന command **cmd_list = cb_data;
	ഘടന command *cmd = *cmd_list;

	എങ്കിൽ (!cmd || is_null_oid(&cmd->new_oid))
		തിരികെ ശൂന്യ;
	*cmd_list = ശൂന്യ; /* this returns only one */
	തിരികെ &cmd->new_oid;
}

സ്ഥിതം ശൂന്യം set_connectivity_errors(ഘടന command *commands,
				    ഘടന shallow_info *si,
				    ഘടന odb_transaction *transaction)
{
	ഘടന command *cmd;

	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
		ഘടന command *singleton = cmd;
		ഘടന check_connected_options opt = CHECK_CONNECTED_INIT;
		ഘടന strvec env = STRVEC_INIT;

		എങ്കിൽ (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			തുടരുക;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		എങ്കിൽ (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			തുടരുക;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

ഘടന iterate_data {
	ഘടന command *cmds;
	ഘടന shallow_info *si;
};

സ്ഥിതം സ്ഥിരം ഘടന object_id *iterate_receive_command_list(ശൂന്യം *cb_data)
{
	ഘടന iterate_data *data = cb_data;
	ഘടന command **cmd_list = &data->cmds;
	ഘടന command *cmd = *cmd_list;

	വേണ്ടി (; cmd; cmd = cmd->next) {
		എങ്കിൽ (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			തുടരുക;
		എങ്കിൽ (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			തിരികെ &cmd->new_oid;
		}
	}
	തിരികെ ശൂന്യ;
}

സ്ഥിതം ശൂന്യം reject_updates_to_hidden(ഘടന command *commands)
{
	ഘടന strbuf refname_full = STRBUF_INIT;
	size_t prefix_len;
	ഘടന command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
		എങ്കിൽ (cmd->error_string)
			തുടരുക;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		എങ്കിൽ (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			തുടരുക;
		എങ്കിൽ (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		അല്ലെങ്കിൽ
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ should_process_cmd(ഘടന command *cmd)
{
	തിരികെ !cmd->error_string && !cmd->skip_update;
}

സ്ഥിതം ശൂന്യം BUG_if_skipped_connectivity_check(ഘടന command *commands,
					       ഘടന shallow_info *si)
{
	ഘടന command *cmd;

	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
		എങ്കിൽ (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

സ്ഥിതം ശൂന്യം ref_transaction_rejection_handler(സ്ഥിരം അക്ഷരം *refname,
					      സ്ഥിരം ഘടന object_id *old_oid UNUSED,
					      സ്ഥിരം ഘടന object_id *new_oid UNUSED,
					      സ്ഥിരം അക്ഷരം *old_target UNUSED,
					      സ്ഥിരം അക്ഷരം *new_target UNUSED,
					      എണ്ണൽ ref_transaction_error err,
					      സ്ഥിരം അക്ഷരം *details,
					      ശൂന്യം *cb_data)
{
	ഘടന strmap *failed_refs = cb_data;

	എങ്കിൽ (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (അക്ഷരം *)ref_transaction_error_സന്ദേശം(err));
}

സ്ഥിതം ശൂന്യം execute_commands_non_atomic(ഘടന command *commands,
					ഘടന shallow_info *si)
{
	ഘടന command *cmd;
	ഘടന strbuf err = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *reported_error = ശൂന്യ;
	ഘടന strmap failed_refs = STRMAP_INIT;

	/*
	 * Reference updates, where D/F conflicts shouldn't arise due to
	 * one reference being deleted, while the other being created
	 * are treated as conflicts in batched updates. This is because
	 * we don't do conflict resolution inside a transaction. To
	 * mitigate this, delete references in a separate batch.
	 *
	 * NEEDSWORK: Add conflict resolution between deletion and creation
	 * of reference updates within a transaction. With that, we can
	 * combine the two phases.
	 */
	എണ്ണൽ processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	വേണ്ടി (എണ്ണൽ processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
			എങ്കിൽ (!should_process_cmd(cmd) || cmd->run_proc_receive)
				തുടരുക;

			എങ്കിൽ (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				തുടരുക;
			അല്ലെങ്കിൽ എങ്കിൽ (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				തുടരുക;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			എങ്കിൽ (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				എങ്കിൽ (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					പോകുക failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		എങ്കിൽ (!transaction)
			പോകുക cleanup;

		എങ്കിൽ (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			പോകുക failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		എങ്കിൽ (strmap_empty(&failed_refs))
			പോകുക cleanup;

	failure:
		വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
			എങ്കിൽ (reported_error)
				cmd->error_string = reported_error;
			അല്ലെങ്കിൽ എങ്കിൽ (strmap_contains(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = ശൂന്യ;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

സ്ഥിതം ശൂന്യം execute_commands_atomic(ഘടന command *commands,
					ഘടന shallow_info *si)
{
	ഘടന command *cmd;
	ഘടന strbuf err = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	എങ്കിൽ (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		പോകുക failure;
	}

	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
		എങ്കിൽ (!should_process_cmd(cmd) || cmd->run_proc_receive)
			തുടരുക;

		cmd->error_string = update(cmd, si);

		എങ്കിൽ (cmd->error_string)
			പോകുക failure;
	}

	എങ്കിൽ (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		പോകുക failure;
	}
	പോകുക cleanup;

failure:
	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next)
		എങ്കിൽ (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

സ്ഥിതം ശൂന്യം execute_commands(ഘടന command *commands,
			     പൂർണ്ണസംഖ്യ unpacker_error,
			     ഘടന shallow_info *si,
			     ഘടന odb_transaction *transaction,
			     സ്ഥിരം ഘടന string_list *തള്ളുക_options)
{
	ഘടന check_connected_options opt = CHECK_CONNECTED_INIT;
	ഘടന command *cmd;
	ഘടന iterate_data data;
	ഘടന async muxer;
	പൂർണ്ണസംഖ്യ err_fd = 0;
	പൂർണ്ണസംഖ്യ run_proc_receive = 0;

	എങ്കിൽ (unpacker_error) {
		വേണ്ടി (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		തിരികെ;
	}

	എങ്കിൽ (!skip_connectivity_check) {
		ഘടന strvec env = STRVEC_INIT;

		എങ്കിൽ (use_sideband) {
			memset(&muxer, 0, വലുപ്പം(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			എങ്കിൽ (!start_async(&muxer))
				err_fd = muxer.in;
			/* ...else, continue without relaying sideband */
		}

		data.cmds = commands;
		data.si = si;
		opt.err_fd = err_fd;
		opt.progress = err_fd && !quiet;
		odb_transaction_env(transaction, &env);
		opt.env = env.v;
		opt.exclude_hidden_refs_section = "receive";

		എങ്കിൽ (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		എങ്കിൽ (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	എങ്കിൽ (proc_receive_ref) {
		വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
			എങ്കിൽ (!should_process_cmd(cmd))
				തുടരുക;

			എങ്കിൽ (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	എങ്കിൽ (run_receive_hook(commands, "pre-receive", 0, transaction, തള്ളുക_options)) {
		വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
			എങ്കിൽ (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		തിരികെ;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	വേണ്ടി (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	എങ്കിൽ (!cmd)
		തിരികെ;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	എങ്കിൽ (odb_transaction_commit(transaction)) {
		വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
			എങ്കിൽ (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		തിരികെ;
	}

	check_aliased_updates(commands);

	സ്വതന്ത്രം(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, ശൂന്യ,
							    ശൂന്യ);

	എങ്കിൽ (run_proc_receive &&
	    run_proc_receive_hook(commands, തള്ളുക_options))
		വേണ്ടി (cmd = commands; cmd; cmd = cmd->next)
			എങ്കിൽ (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	എങ്കിൽ (use_atomic)
		execute_commands_atomic(commands, si);
	അല്ലെങ്കിൽ
		execute_commands_non_atomic(commands, si);

	എങ്കിൽ (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

സ്ഥിതം ഘടന command **queue_command(ഘടന command **tail,
				      സ്ഥിരം അക്ഷരം *വരി,
				      പൂർണ്ണസംഖ്യ linelen)
{
	ഘടന object_id old_oid, new_oid;
	ഘടന command *cmd;
	സ്ഥിരം അക്ഷരം *refname;
	പൂർണ്ണസംഖ്യ reflen;
	സ്ഥിരം അക്ഷരം *p;

	എങ്കിൽ (parse_oid_hex(വരി, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", വരി);

	refname = p;
	reflen = linelen - (p - വരി);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*tail = cmd;
	തിരികെ &cmd->next;
}

സ്ഥിതം ശൂന്യം free_commands(ഘടന command *commands)
{
	ആകുമ്പോൾ (commands) {
		ഘടന command *next = commands->next;

		ref_തള്ളുക_report_free(commands->report);
		സ്വതന്ത്രം(commands->error_string_owned);
		സ്വതന്ത്രം(commands);
		commands = next;
	}
}

സ്ഥിതം ശൂന്യം queue_commands_from_cert(ഘടന command **tail,
				     ഘടന strbuf *തള്ളുക_cert)
{
	സ്ഥിരം അക്ഷരം *boc, *eoc;

	എങ്കിൽ (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(തള്ളുക_cert->buf, "\n\n");
	എങ്കിൽ (!boc)
		die("malformed push certificate %.*s", 100, തള്ളുക_cert->buf);
	അല്ലെങ്കിൽ
		boc += 2;
	eoc = തള്ളുക_cert->buf + parse_signed_buffer(തള്ളുക_cert->buf, തള്ളുക_cert->len);

	ആകുമ്പോൾ (boc < eoc) {
		സ്ഥിരം അക്ഷരം *eol = memchr(boc, '\n', eoc - boc);
		tail = queue_command(tail, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

സ്ഥിതം ഘടന command *read_head_info(ഘടന packet_reader *reader,
				      ഘടന oid_array *shallow,
				      എണ്ണൽ report_status_version *version)
{
	ഘടന command *commands = ശൂന്യ;
	ഘടന command **p = &commands;
	വേണ്ടി (;;) {
		പൂർണ്ണസംഖ്യ linelen;

		എങ്കിൽ (packet_reader_read(reader) != PACKET_READ_NORMAL)
			നിർത്തുക;

		എങ്കിൽ (reader->pktlen > 8 && starts_with(reader->വരി, "shallow ")) {
			ഘടന object_id oid;
			എങ്കിൽ (get_oid_hex(reader->വരി + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->വരി + 8);
			oid_array_append(shallow, &oid);
			തുടരുക;
		}

		linelen = strlen(reader->വരി);
		എങ്കിൽ (linelen < reader->pktlen) {
			സ്ഥിരം അക്ഷരം *feature_list = reader->വരി + linelen + 1;
			സ്ഥിരം അക്ഷരം *hash = ശൂന്യ;
			സ്ഥിരം അക്ഷരം *client_sid;
			size_t len = 0;
			എങ്കിൽ (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			എങ്കിൽ (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			എങ്കിൽ (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			എങ്കിൽ (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			എങ്കിൽ (advertise_atomic_തള്ളുക
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			എങ്കിൽ (advertise_തള്ളുക_options
			    && parse_feature_request(feature_list, "push-options"))
				use_തള്ളുക_options = 1;
			hash = parse_feature_മൂല്യം(feature_list, "object-format", &len, ശൂന്യ);
			എങ്കിൽ (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			എങ്കിൽ (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_മൂല്യം(feature_list, "session-id", &len, ശൂന്യ);
			എങ്കിൽ (client_sid) {
				അക്ഷരം *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", ശൂന്യ, "client-sid", client_sid);
				സ്വതന്ത്രം(sid);
			}
		}

		എങ്കിൽ (!strcmp(reader->വരി, "push-cert")) {
			പൂർണ്ണസംഖ്യ true_flush = 0;
			പൂർണ്ണസംഖ്യ saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			വേണ്ടി (;;) {
				packet_reader_read(reader);
				എങ്കിൽ (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					നിർത്തുക;
				}
				എങ്കിൽ (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				എങ്കിൽ (!strcmp(reader->വരി, "push-cert-end\n"))
					നിർത്തുക; /* end of cert */
				strbuf_addstr(&തള്ളുക_cert, reader->വരി);
			}
			reader->options = saved_options;

			എങ്കിൽ (true_flush)
				നിർത്തുക;
			തുടരുക;
		}

		p = queue_command(p, reader->വരി, linelen);
	}

	എങ്കിൽ (തള്ളുക_cert.len)
		queue_commands_from_cert(p, &തള്ളുക_cert);

	തിരികെ commands;
}

സ്ഥിതം ശൂന്യം read_തള്ളുക_options(ഘടന packet_reader *reader,
			      ഘടന string_list *options)
{
	ആകുമ്പോൾ (1) {
		എങ്കിൽ (packet_reader_read(reader) != PACKET_READ_NORMAL)
			നിർത്തുക;

		string_list_append(options, reader->വരി);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ unpack_with_sideband(ഘടന odb_transaction *transaction,
				സ്ഥിരം അക്ഷരം *shallow_file,
				ഘടന strbuf *err_സന്ദേശം)
{
	ഘടന odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_സന്ദേശം_types = fsck_സന്ദേശം_types.buf,
		.max_നിവേശം_size = max_നിവേശം_size,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	ഘടന async muxer;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (!use_sideband)
		തിരികെ odb_transaction_write_pack(transaction, 0, err_സന്ദേശം, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, വലുപ്പം(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	എങ്കിൽ (start_async(&muxer))
		തിരികെ 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_സന്ദേശം, &opts);

	finish_async(&muxer);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം prepare_shallow_update(ഘടന shallow_info *si,
				   സ്ഥിരം അക്ഷരം *shallow_file)
{
	പൂർണ്ണസംഖ്യ i, j, k, bitmap_size = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, ശൂന്യ);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	വേണ്ടി (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	വേണ്ടി (i = 0; i < si->shallow->nr; i++) {
		എങ്കിൽ (!si->used_shallow[i])
			തുടരുക;
		വേണ്ടി (j = 0; j < bitmap_size; j++) {
			എങ്കിൽ (!si->used_shallow[i][j])
				തുടരുക;
			si->need_reachability_test[i]++;
			വേണ്ടി (k = 0; k < 32; k++)
				എങ്കിൽ (si->used_shallow[i][j] & (1U << k))
					si->shallow_ref[j * 32 + k]++;
		}

		/*
		 * true for those associated with some refs and belong
		 * in "ours" list aka "step 7 not done yet"
		 */
		si->need_reachability_test[i] =
			si->need_reachability_test[i] > 1;
	}

	/*
	 * keep hooks happy by forcing a temporary shallow file via
	 * env variable because we can't add --shallow-file to every
	 * command. check_connected() will be done with
	 * true .git/shallow though.
	 */
	setenv(GIT_SHALLOW_FILE_ENVIRONMENT, shallow_file, 1);
}

സ്ഥിതം ശൂന്യം update_shallow_info(ഘടന command *commands,
				ഘടന shallow_info *si,
				ഘടന oid_array *ref,
				സ്ഥിരം അക്ഷരം *shallow_file)
{
	ഘടന command *cmd;
	പൂർണ്ണസംഖ്യ *ref_status;
	remove_nonexistent_theirs_shallow(si);
	എങ്കിൽ (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		തിരികെ;
	}

	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
		എങ്കിൽ (is_null_oid(&cmd->new_oid))
			തുടരുക;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	എങ്കിൽ (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		തിരികെ;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, ശൂന്യ, ref_status);
	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
		എങ്കിൽ (is_null_oid(&cmd->new_oid))
			തുടരുക;
		എങ്കിൽ (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	സ്വതന്ത്രം(ref_status);
}

സ്ഥിതം ശൂന്യം override_cmds_error(ഘടന command *commands, സ്ഥിരം അക്ഷരം *err)
{
	വേണ്ടി (ഘടന command *cmd = commands; cmd; cmd = cmd->next) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
സ്ഥിതം ശൂന്യം generate_report(ഘടന strbuf *buf, ഘടന command *commands,
			    സ്ഥിരം ഘടന strbuf *unpack_status,
			    എണ്ണൽ report_status_version version)
{
	ഘടന command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
		ഘടന ref_തള്ളുക_report *report;
		പൂർണ്ണസംഖ്യ എണ്ണം = 0;

		എങ്കിൽ (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		അല്ലെങ്കിൽ
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		എങ്കിൽ (version != REPORT_STATUS_V2 || cmd->error_string)
			തുടരുക;

		വേണ്ടി (report = cmd->report; report; report = report->next) {
			എങ്കിൽ (എണ്ണം++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			എങ്കിൽ (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			എങ്കിൽ (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			എങ്കിൽ (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			എങ്കിൽ (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

സ്ഥിതം ശൂന്യം report(ഘടന command *commands, സ്ഥിരം ഘടന strbuf *unpack_status,
		   എണ്ണൽ report_status_version version)
{
	ഘടന strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	എങ്കിൽ (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	എങ്കിൽ (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	അല്ലെങ്കിൽ
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ delete_only(ഘടന command *commands)
{
	ഘടന command *cmd;
	വേണ്ടി (cmd = commands; cmd; cmd = cmd->next) {
		എങ്കിൽ (!is_null_oid(&cmd->new_oid))
			തിരികെ 0;
	}
	തിരികെ 1;
}

പൂർണ്ണസംഖ്യ cmd_receive_pack(പൂർണ്ണസംഖ്യ argc,
		     സ്ഥിരം അക്ഷരം **argv,
		     സ്ഥിരം അക്ഷരം *prefix,
		     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ advertise_refs = 0;
	ഘടന command *commands;
	ഘടന oid_array shallow = OID_ARRAY_INIT;
	ഘടന oid_array ref = OID_ARRAY_INIT;
	ഘടന shallow_info si;
	ഘടന packet_reader reader;
	ഘടന odb_transaction *transaction = ശൂന്യ;
	എണ്ണൽ report_status_version version = REPORT_STATUS_UNKNOWN;

	ഘടന option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, ശൂന്യ),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, ശൂന്യ),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, ശൂന്യ),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, ശൂന്യ),
		OPT_END()
	};

	packet_trace_identity("receive-pack");

	argc = parse_options(argc, argv, prefix, options, receive_pack_usage, 0);

	എങ്കിൽ (argc > 1)
		usage_സന്ദേശം_opt(_("too many arguments"), receive_pack_usage, options);
	എങ്കിൽ (argc == 0)
		usage_സന്ദേശം_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	എങ്കിൽ (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, ശൂന്യ);
	എങ്കിൽ (cert_nonce_seed)
		തള്ളുക_cert_nonce = prepare_തള്ളുക_cert_nonce(service_dir, time(ശൂന്യ));

	തിരഞ്ഞെടുപ്പ് (determine_protocol_version_server()) {
	അവസ്ഥ protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		നിർത്തുക;
	അവസ്ഥ protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		എങ്കിൽ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	അവസ്ഥ protocol_v0:
		നിർത്തുക;
	അവസ്ഥ protocol_unknown_version:
		BUG("unknown protocol version");
	}

	എങ്കിൽ (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	എങ്കിൽ (advertise_refs)
		തിരികെ 0;

	packet_reader_init(&reader, 0, ശൂന്യ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	എങ്കിൽ ((commands = read_head_info(&reader, &shallow, &version))) {
		ഘടന string_list തള്ളുക_options = STRING_LIST_INIT_DUP;
		ഘടന strbuf unpack_status = STRBUF_INIT;

		എങ്കിൽ (use_തള്ളുക_options)
			read_തള്ളുക_options(&reader, &തള്ളുക_options);
		എങ്കിൽ (!check_cert_തള്ളുക_options(&തള്ളുക_options)) {
			ഘടന command *cmd;
			വേണ്ടി (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		എങ്കിൽ (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		എങ്കിൽ (!delete_only(commands)) {
			സ്ഥിരം അക്ഷരം *alt_shallow_file = ശൂന്യ;

			എങ്കിൽ (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			എങ്കിൽ (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			അല്ലെങ്കിൽ
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &തള്ളുക_options);
		odb_transaction_finalize(transaction);
		sigchain_തള്ളുക(SIGPIPE, SIG_IGN);

		തിരഞ്ഞെടുപ്പ് (version) {
		അവസ്ഥ REPORT_STATUS_V2:
		അവസ്ഥ REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			നിർത്തുക;
		അവസ്ഥ REPORT_STATUS_UNKNOWN:
			നിർത്തുക;
		}

		sigchain_എടുക്കുക(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, ശൂന്യ,
				 &തള്ളുക_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&തള്ളുക_options, 0);
		എങ്കിൽ (auto_gc) {
			ഘടന child_process proc = CHILD_PROCESS_INIT;

			എങ്കിൽ (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				എങ്കിൽ (!start_command(&proc)) {
					എങ്കിൽ (use_sideband)
						copy_to_sideband(proc.err, -1, ശൂന്യ);
					finish_command(&proc);
				}
			}
		}
		എങ്കിൽ (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	എങ്കിൽ (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	സ്വതന്ത്രം((ശൂന്യം *)തള്ളുക_cert_nonce);
	തിരികെ 0;
}
