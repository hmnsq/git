#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "connect.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "connected.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "exec-cmd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fsck.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gpg-interface.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hook.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oidset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pkt-line.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "protocol.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "server-info.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "shallow.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sideband.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sigchain.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace2.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "version.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "worktree.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	ਨਿਰਾ
};

ਗਿਣਤੀ deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

ਗਿਣਤੀ report_ਸਥਿਤੀ_ਸੰਸਕਰਣ {
	REPORT_ਸਥਿਤੀ_UNKNOWN = 0,
	REPORT_ਸਥਿਤੀ_V0,
	REPORT_ਸਥਿਤੀ_V2,
};

ਸਥਿਰ ਪੂਰਨ deny_deletes;
ਸਥਿਰ ਪੂਰਨ deny_non_fast_forwards;
ਸਥਿਰ ਗਿਣਤੀ deny_action deny_current_branch = DENY_UNCONFIGURED;
ਸਥਿਰ ਗਿਣਤੀ deny_action deny_delete_current = DENY_UNCONFIGURED;
ਸਥਿਰ ਪੂਰਨ receive_fsck_objects = -1;
ਸਥਿਰ ਪੂਰਨ transfer_fsck_objects = -1;
ਸਥਿਰ ਬਣਤਰ strbuf fsck_ਸੁਨੇਹਾ_types = STRBUF_INIT;
ਸਥਿਰ ਪੂਰਨ advertise_atomic_ਧੱਕੋ = 1;
ਸਥਿਰ ਪੂਰਨ advertise_ਧੱਕੋ_options;
ਸਥਿਰ ਪੂਰਨ advertise_sid;
ਸਥਿਰ off_t max_ਨਿਵੇਸ਼_ਮਾਪ;
ਸਥਿਰ ਪੂਰਨ use_sideband;
ਸਥਿਰ ਪੂਰਨ use_atomic;
ਸਥਿਰ ਪੂਰਨ use_ਧੱਕੋ_options;
ਸਥਿਰ ਪੂਰਨ quiet;
ਸਥਿਰ ਪੂਰਨ prefer_ofs_delta = 1;
ਸਥਿਰ ਪੂਰਨ auto_update_server_info;
ਸਥਿਰ ਪੂਰਨ auto_gc = 1;
ਸਥਿਰ ਪੂਰਨ reject_thin;
ਸਥਿਰ ਪੂਰਨ skip_connectivity_check;
ਸਥਿਰ ਪੂਰਨ stateless_rpc;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *service_dir;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸਿਰ_name;
ਸਥਿਰ ਖਾਲੀ *ਸਿਰ_name_ਮੰਜ਼ਿਲ_free;
ਸਥਿਰ ਪੂਰਨ ਭੇਜਿਆ_capabilities;
ਸਥਿਰ ਪੂਰਨ shallow_update;
ਸਥਿਰ ਬਣਤਰ strbuf ਧੱਕੋ_cert = STRBUF_INIT;
ਸਥਿਰ ਬਣਤਰ object_id ਧੱਕੋ_cert_oid;
ਸਥਿਰ ਬਣਤਰ signature_check sigcheck;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਧੱਕੋ_cert_nonce;
ਸਥਿਰ ਅੱਖਰ *cert_nonce_ਬੀਜ;
ਸਥਿਰ ਬਣਤਰ strvec hidden_refs = STRVEC_INIT;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *NONCE_UNSOLICITED = "UNSOLICITED";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *NONCE_BAD = "BAD";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *NONCE_MISSING = "MISSING";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *NONCE_OK = "OK";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *NONCE_SLOP = "SLOP";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *nonce_ਸਥਿਤੀ;
ਸਥਿਰ ਲੰਮਾ nonce_stamp_slop;
ਸਥਿਰ timestamp_t nonce_stamp_slop_ਹੱਦਬੰਦੀ;
ਸਥਿਰ ਬਣਤਰ ref_transaction *transaction;

ਸਥਿਰ ਗਿਣਤੀ {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
ਸਥਿਰ ਪੂਰਨ keepalive_ਵਿੱਚ_sec = 5;

ਸਥਿਰ ਬਣਤਰ proc_receive_ref {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ want_ਜੋੜੋ:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	ਅੱਖਰ *ref_ਅਗੇਤਰ;
	ਬਣਤਰ proc_receive_ref *ਅਗਲਾ;
} *proc_receive_ref;

ਸਥਿਰ ਖਾਲੀ proc_receive_ref_append(ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ);

ਸਥਿਰ ਗਿਣਤੀ deny_action parse_deny_action(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਜੇ (ਮੁੱਲ) {
		ਜੇ (!strcasecmp(ਮੁੱਲ, "ignore"))
			ਵਾਪਸ DENY_IGNORE;
		ਜੇ (!strcasecmp(ਮੁੱਲ, "warn"))
			ਵਾਪਸ DENY_WARN;
		ਜੇ (!strcasecmp(ਮੁੱਲ, "refuse"))
			ਵਾਪਸ DENY_REFUSE;
		ਜੇ (!strcasecmp(ਮੁੱਲ, "updateinstead"))
			ਵਾਪਸ DENY_UPDATE_INSTEAD;
	}
	ਜੇ (git_config_bool(var, ਮੁੱਲ))
		ਵਾਪਸ DENY_REFUSE;
	ਵਾਪਸ DENY_IGNORE;
}

ਸਥਿਰ ਪੂਰਨ receive_pack_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			       ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_id_2;
	ਪੂਰਨ ਸਥਿਤੀ = parse_hide_refs_config(var, ਮੁੱਲ, "receive", &hidden_refs);

	ਜੇ (ਸਥਿਤੀ)
		ਵਾਪਸ ਸਥਿਤੀ;

	ਜੇ (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.fsck.skiplist") == 0) {
		ਅੱਖਰ *ਰਸਤਾ;

		ਜੇ (git_config_pathname(&ਰਸਤਾ, var, ਮੁੱਲ))
			ਵਾਪਸ -1;
		ਜੇ (ਰਸਤਾ)
			strbuf_addf(&fsck_ਸੁਨੇਹਾ_types, "%cskiplist=%s",
				    fsck_ਸੁਨੇਹਾ_types.len ? ',' : '=', ਰਸਤਾ);
		ਮੁਕਤ(ਰਸਤਾ);
		ਵਾਪਸ 0;
	}

	ਜੇ (skip_ਅਗੇਤਰ(var, "receive.fsck.", &ਸੁਨੇਹਾ_id_2)) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		ਜੇ (is_valid_ਸੁਨੇਹਾ_type(ਸੁਨੇਹਾ_id_2, ਮੁੱਲ))
			strbuf_addf(&fsck_ਸੁਨੇਹਾ_types, "%c%s=%s",
				fsck_ਸੁਨੇਹਾ_types.len ? ',' : '=', ਸੁਨੇਹਾ_id_2, ਮੁੱਲ);
		ਨਹੀਂਤਾਂ
			warning("skipping unknown msg id '%s'", ਸੁਨੇਹਾ_id_2);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.certnonceseed") == 0)
		ਵਾਪਸ git_config_string(&cert_nonce_ਬੀਜ, var, ਮੁੱਲ);

	ਜੇ (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_ਹੱਦਬੰਦੀ = git_config_ulong(var, ਮੁੱਲ, ctx->kvi);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_ਧੱਕੋ = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_ਧੱਕੋ_options = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.keepalive") == 0) {
		keepalive_ਵਿੱਚ_sec = git_config_int(var, ਮੁੱਲ, ctx->kvi);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.maxinputsize") == 0) {
		max_ਨਿਵੇਸ਼_ਮਾਪ = git_config_int64(var, ਮੁੱਲ, ctx->kvi);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "receive.procreceiverefs") == 0) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		proc_receive_ref_append(ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਖਾਲੀ show_ref(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਜੇ (ਭੇਜਿਆ_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_ਮੰਜ਼ਿਲ_hex(oid), ਰਸਤਾ);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		ਜੇ (advertise_atomic_ਧੱਕੋ)
			strbuf_addstr(&cap, " atomic");
		ਜੇ (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		ਜੇ (ਧੱਕੋ_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", ਧੱਕੋ_cert_nonce);
		ਜੇ (advertise_ਧੱਕੋ_options)
			strbuf_addstr(&cap, " push-options");
		ਜੇ (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_ਮੰਜ਼ਿਲ_hex(oid), ਰਸਤਾ, 0, cap.buf);
		strbuf_release(&cap);
		ਭੇਜਿਆ_capabilities = 1;
	}
}

ਸਥਿਰ ਪੂਰਨ show_ref_cb(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ oidset *ਵੇਖਿਆ = ਅੰਕੜੇ;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = strip_namespace(ref->name);

	ਜੇ (ref_is_hidden(ਰਸਤਾ, ref->name, &hidden_refs))
		ਵਾਪਸ 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	ਜੇ (!ਰਸਤਾ) {
		ਜੇ (oidset_ਪਾਓ(ਵੇਖਿਆ, ref->oid))
			ਵਾਪਸ 0;
		ਰਸਤਾ = ".have";
	} ਨਹੀਂਤਾਂ {
		oidset_ਪਾਓ(ਵੇਖਿਆ, ref->oid);
	}
	show_ref(ਰਸਤਾ, ref->oid);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ show_one_alternate_ref(ਅਚਲ ਬਣਤਰ object_id *oid,
				   ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ oidset *ਵੇਖਿਆ = ਅੰਕੜੇ;

	ਜੇ (oidset_ਪਾਓ(ਵੇਖਿਆ, oid))
		ਵਾਪਸ;

	show_ref(".have", oid);
}

ਸਥਿਰ ਖਾਲੀ write_ਸਿਰ_info(ਖਾਲੀ)
{
	ਬਣਤਰ refs_for_each_ref_options opts = { 0 };
	ਸਥਿਰ ਬਣਤਰ oidset ਵੇਖਿਆ = OIDSET_INIT;
	ਬਣਤਰ strvec excludes_ਸਦਿਸ਼ = STRVEC_INIT;

	/*
	 * We need access to the reference names both with and without their
	 * namespace and thus cannot use `refs_for_each_namespaced_ref()`. We
	 * thus have to adapt exclude patterns to carry the namespace prefix
	 * ourselves.
	 */
	opts.exclude_patterns = get_namespaced_exclude_patterns(
		hidden_refs_ਮੰਜ਼ਿਲ_excludes(&hidden_refs),
		get_git_namespace(), &excludes_ਸਦਿਸ਼);

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      show_ref_cb, &ਵੇਖਿਆ, &opts);
	odb_for_each_alternate_ref(the_repository->objects,
				   show_one_alternate_ref, &ਵੇਖਿਆ);

	oidset_clear(&ਵੇਖਿਆ);
	strvec_clear(&excludes_ਸਦਿਸ਼);

	ਜੇ (!ਭੇਜਿਆ_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਲੜੀਵਾਰ_PROC_RECEIVE_SCHEDULED	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਲੜੀਵਾਰ_PROC_RECEIVE_RETURNED	2
ਬਣਤਰ command {
	ਬਣਤਰ command *ਅਗਲਾ;
	ਅਚਲ ਅੱਖਰ *error_string;
	ਅੱਖਰ *error_string_owned;
	ਬਣਤਰ ref_ਧੱਕੋ_report *report;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ skip_update:1,
		     did_not_exist:1,
		     ਲੜੀਵਾਰ_proc_receive:2;
	ਪੂਰਨ ਸੂਚਕਾਂਕ;
	ਬਣਤਰ object_id old_oid;
	ਬਣਤਰ object_id new_oid;
	ਅੱਖਰ ref_name[FLEX_ARRAY]; /*ਹੋਰ*/
};

ਸਥਿਰ ਖਾਲੀ proc_receive_ref_append(ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਬਣਤਰ proc_receive_ref *ref_ਨਮੂਨਾ;
	ਅਚਲ ਅੱਖਰ *p;
	ਪੂਰਨ len;

	CALLOC_ARRAY(ref_ਨਮੂਨਾ, 1);
	p = strchr(ਅਗੇਤਰ, ':');
	ਜੇ (p) {
		ਜਦੋਂ (ਅਗੇਤਰ < p) {
			ਜੇ (*ਅਗੇਤਰ == 'a')
				ref_ਨਮੂਨਾ->want_ਜੋੜੋ = 1;
			ਨਹੀਂਤਾਂ ਜੇ (*ਅਗੇਤਰ == 'd')
				ref_ਨਮੂਨਾ->want_delete = 1;
			ਨਹੀਂਤਾਂ ਜੇ (*ਅਗੇਤਰ == 'm')
				ref_ਨਮੂਨਾ->want_modify = 1;
			ਨਹੀਂਤਾਂ ਜੇ (*ਅਗੇਤਰ == '!')
				ref_ਨਮੂਨਾ->negative_ref = 1;
			ਅਗੇਤਰ++;
		}
		ਅਗੇਤਰ++;
	} ਨਹੀਂਤਾਂ {
		ref_ਨਮੂਨਾ->want_ਜੋੜੋ = 1;
		ref_ਨਮੂਨਾ->want_delete = 1;
		ref_ਨਮੂਨਾ->want_modify = 1;
	}
	len = ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ);
	ਜਦੋਂ (len && ਅਗੇਤਰ[len - 1] == '/')
		len--;
	ref_ਨਮੂਨਾ->ref_ਅਗੇਤਰ = xmemdupz(ਅਗੇਤਰ, len);
	ਜੇ (!proc_receive_ref) {
		proc_receive_ref = ref_ਨਮੂਨਾ;
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ proc_receive_ref *ਸਿਰਾ;

		ਸਿਰਾ = proc_receive_ref;
		ਜਦੋਂ (ਸਿਰਾ->ਅਗਲਾ)
			ਸਿਰਾ = ਸਿਰਾ->ਅਗਲਾ;
		ਸਿਰਾ->ਅਗਲਾ = ref_ਨਮੂਨਾ;
	}
}

ਸਥਿਰ ਪੂਰਨ proc_receive_ref_matches(ਬਣਤਰ command *cmd)
{
	ਬਣਤਰ proc_receive_ref *p;

	ਜੇ (!proc_receive_ref)
		ਵਾਪਸ 0;

	ਲਈ (p = proc_receive_ref; p; p = p->ਅਗਲਾ) {
		ਅਚਲ ਅੱਖਰ *ਮੇਲ = p->ref_ਅਗੇਤਰ;
		ਅਚਲ ਅੱਖਰ *remains;

		ਜੇ (!p->want_ਜੋੜੋ && is_null_oid(&cmd->old_oid))
			ਜਾਰੀ;
		ਨਹੀਂਤਾਂ ਜੇ (!p->want_delete && is_null_oid(&cmd->new_oid))
			ਜਾਰੀ;
		ਨਹੀਂਤਾਂ ਜੇ (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			ਜਾਰੀ;

		ਜੇ (skip_ਅਗੇਤਰ(cmd->ref_name, ਮੇਲ, &remains) &&
		    (!*remains || *remains == '/')) {
			ਜੇ (!p->negative_ref)
				ਵਾਪਸ 1;
		} ਨਹੀਂਤਾਂ ਜੇ (p->negative_ref) {
			ਵਾਪਸ 1;
		}
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ report_ਸੁਨੇਹਾ(ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਅਚਲ ਅੱਖਰ *err, va_list params)
{
	ਪੂਰਨ sz;
	ਅੱਖਰ ਸੁਨੇਹਾ[4096];

	sz = xsnprintf(ਸੁਨੇਹਾ, ਆਕਾਰ(ਸੁਨੇਹਾ), "%s", ਅਗੇਤਰ);
	sz += vsnprintf(ਸੁਨੇਹਾ + sz, ਆਕਾਰ(ਸੁਨੇਹਾ) - sz, err, params);
	ਜੇ (sz > (ਆਕਾਰ(ਸੁਨੇਹਾ) - 1))
		sz = ਆਕਾਰ(ਸੁਨੇਹਾ) - 1;
	ਸੁਨੇਹਾ[sz++] = '\n';

	ਜੇ (use_sideband)
		send_sideband(1, 2, ਸੁਨੇਹਾ, sz, use_sideband);
	ਨਹੀਂਤਾਂ
		xwrite(2, ਸੁਨੇਹਾ, sz);
}

__attribute__((format (ਲਿਖੋ, 1, 2)))
ਸਥਿਰ ਖਾਲੀ rp_warning(ਅਚਲ ਅੱਖਰ *err, ...)
{
	va_list params;
	va_ਸ਼ੁਰੂ(params, err);
	report_ਸੁਨੇਹਾ("warning: ", err, params);
	va_ਸਿਰਾ(params);
}

__attribute__((format (ਲਿਖੋ, 1, 2)))
ਸਥਿਰ ਖਾਲੀ rp_error(ਅਚਲ ਅੱਖਰ *err, ...)
{
	va_list params;
	va_ਸ਼ੁਰੂ(params, err);
	report_ਸੁਨੇਹਾ("error: ", err, params);
	va_ਸਿਰਾ(params);
}

ਸਥਿਰ ਪੂਰਨ copy_ਮੰਜ਼ਿਲ_sideband(ਪੂਰਨ ਵਿੱਚ, ਪੂਰਨ ਬਾਹਰਲਾ ਅਣਵਰਤਿਆ, ਖਾਲੀ *arg ਅਣਵਰਤਿਆ)
{
	ਅੱਖਰ ਅੰਕੜੇ[128];
	ਪੂਰਨ keepalive_ਸਰਗਰਮ = 0;

	ਜੇ (keepalive_ਵਿੱਚ_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	ਜੇ (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_ਸਰਗਰਮ = 1;

	ਜਦੋਂ (1) {
		ssize_t sz;

		ਜੇ (keepalive_ਸਰਗਰਮ) {
			ਬਣਤਰ pollfd pfd;
			ਪੂਰਨ ret;

			pfd.fd = ਵਿੱਚ;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_ਵਿੱਚ_sec);

			ਜੇ (ret < 0) {
				ਜੇ (errno == EINTR)
					ਜਾਰੀ;
				ਨਹੀਂਤਾਂ
					ਰੋਕੋ;
			} ਨਹੀਂਤਾਂ ਜੇ (ret == 0) {
				/* no data; send a keepalive packet */
				ਸਥਿਰ ਅਚਲ ਅੱਖਰ buf[] = "0005\1";
				write_or_die(1, buf, ਆਕਾਰ(buf) - 1);
				ਜਾਰੀ;
			} /* else there is actual data to read */
		}

		sz = xread(ਵਿੱਚ, ਅੰਕੜੇ, ਆਕਾਰ(ਅੰਕੜੇ));
		ਜੇ (sz <= 0)
			ਰੋਕੋ;

		ਜੇ (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_ਸਰਗਰਮ) {
			ਅਚਲ ਅੱਖਰ *p = memchr(ਅੰਕੜੇ, '\0', sz);
			ਜੇ (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_ਸਰਗਰਮ = 1;
				send_sideband(1, 2, ਅੰਕੜੇ, p - ਅੰਕੜੇ, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - ਅੰਕੜੇ + 1), use_sideband);
				ਜਾਰੀ;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, ਅੰਕੜੇ, sz, use_sideband);
	}
	close(ਵਿੱਚ);
	ਵਾਪਸ 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
ਸਥਿਰ ਖਾਲੀ prepare_sideband_async(ਬਣਤਰ async *sideband_async, ਪੂਰਨ *saved_stderr, ਪੂਰਨ *started)
{
	*started = 0;

	ਜੇ (!use_sideband)
		ਵਾਪਸ;

	memset(sideband_async, 0, ਆਕਾਰ(*sideband_async));
	sideband_async->proc = copy_ਮੰਜ਼ਿਲ_sideband;
	sideband_async->ਵਿੱਚ = -1;

	ਜੇ (!ਸ਼ੁਰੂ_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		ਜੇ (*saved_stderr >= 0)
			dup2(sideband_async->ਵਿੱਚ, STDERR_FILENO);
		close(sideband_async->ਵਿੱਚ);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
ਸਥਿਰ ਖਾਲੀ ਅੰਤ_sideband_async(ਬਣਤਰ async *sideband_async, ਪੂਰਨ saved_stderr, ਪੂਰਨ started)
{
	ਜੇ (!use_sideband)
		ਵਾਪਸ;

	ਜੇ (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	ਜੇ (started)
		ਅੰਤ_async(sideband_async);
}

ਸਥਿਰ ਖਾਲੀ hmac_hash(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਬਾਹਰਲਾ,
		      ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ_ਵਿੱਚ, ਮਾਪ_t ਕੁੰਜੀ_len,
		      ਅਚਲ ਅੱਖਰ *ਪਾਠ, ਮਾਪ_t ਪਾਠ_len)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਕੁੰਜੀ[GIT_MAX_BLKSZ];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ k_ipad[GIT_MAX_BLKSZ];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ k_opad[GIT_MAX_BLKSZ];
	ਪੂਰਨ i;
	ਬਣਤਰ git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(ਕੁੰਜੀ, '\0', GIT_MAX_BLKSZ);
	ਜੇ (the_hash_algo->blksz < ਕੁੰਜੀ_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, ਕੁੰਜੀ_ਵਿੱਚ, ਕੁੰਜੀ_len);
		git_hash_final(ਕੁੰਜੀ, &ctx);
	} ਨਹੀਂਤਾਂ {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਕੁੰਜੀ, ਕੁੰਜੀ_ਵਿੱਚ, ਕੁੰਜੀ_len);
	}

	/* RFC 2104 2. (2) & (5) */
	ਲਈ (i = 0; i < ਆਕਾਰ(ਕੁੰਜੀ); i++) {
		k_ipad[i] = ਕੁੰਜੀ[i] ^ 0x36;
		k_opad[i] = ਕੁੰਜੀ[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, ਆਕਾਰ(k_ipad));
	git_hash_update(&ctx, ਪਾਠ, ਪਾਠ_len);
	git_hash_final(ਬਾਹਰਲਾ, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, ਆਕਾਰ(k_opad));
	git_hash_update(&ctx, ਬਾਹਰਲਾ, the_hash_algo->rawsz);
	git_hash_final(ਬਾਹਰਲਾ, &ctx);
}

ਸਥਿਰ ਅੱਖਰ *prepare_ਧੱਕੋ_cert_nonce(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, timestamp_t stamp)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, ਰਸਤਾ, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_ਬੀਜ, ਲੜੀ_ਲੰਬਾਈ(cert_nonce_ਬੀਜ));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (ਪੂਰਨ)the_hash_algo->hexsz, hash_ਮੰਜ਼ਿਲ_hex(hash));
	ਵਾਪਸ strbuf_detach(&buf, ਨਿਰਾ);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
ਸਥਿਰ ਪੂਰਨ constant_memequal(ਅਚਲ ਅੱਖਰ *a, ਅਚਲ ਅੱਖਰ *b, ਮਾਪ_t n)
{
	ਪੂਰਨ res = 0;
	ਮਾਪ_t i;

	ਲਈ (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	ਵਾਪਸ res;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *check_nonce(ਅਚਲ ਅੱਖਰ *buf)
{
	ਮਾਪ_t noncelen;
	ਅਚਲ ਅੱਖਰ *ਮਿਲਿਆ_2 = ਲੱਭੋ_commit_ਸਿਰਲੇਖ(buf, "nonce", &noncelen);
	ਅੱਖਰ *nonce = ਮਿਲਿਆ_2 ? xmemdupz(ਮਿਲਿਆ_2, noncelen) : ਨਿਰਾ;
	timestamp_t stamp, ostamp;
	ਅੱਖਰ *bohmac, *expect = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *retval = NONCE_BAD;

	ਜੇ (!nonce) {
		retval = NONCE_MISSING;
		ਜਾਓ leave;
	} ਨਹੀਂਤਾਂ ਜੇ (!ਧੱਕੋ_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		ਜਾਓ leave;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ਧੱਕੋ_cert_nonce, nonce)) {
		retval = NONCE_OK;
		ਜਾਓ leave;
	}

	ਜੇ (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		ਜਾਓ leave;
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
	ਜੇ (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		ਜਾਓ leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	ਜੇ (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		ਜਾਓ leave;
	}

	expect = prepare_ਧੱਕੋ_cert_nonce(service_dir, stamp);
	ਜੇ (noncelen != ਲੜੀ_ਲੰਬਾਈ(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		ਜਾਓ leave;
	}
	ਜੇ (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		ਜਾਓ leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(ਧੱਕੋ_cert_nonce, ਨਿਰਾ, 10);
	nonce_stamp_slop = (ਲੰਮਾ)ostamp - (ਲੰਮਾ)stamp;

	ਜੇ (nonce_stamp_slop_ਹੱਦਬੰਦੀ &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_ਹੱਦਬੰਦੀ) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		ਮੁਕਤ((ਖਾਲੀ *)ਧੱਕੋ_cert_nonce);
		ਧੱਕੋ_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} ਨਹੀਂਤਾਂ {
		retval = NONCE_SLOP;
	}

leave:
	ਮੁਕਤ(nonce);
	ਮੁਕਤ(expect);
	ਵਾਪਸ retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
ਸਥਿਰ ਪੂਰਨ check_cert_ਧੱਕੋ_options(ਅਚਲ ਬਣਤਰ string_list *ਧੱਕੋ_options)
{
	ਅਚਲ ਅੱਖਰ *buf = ਧੱਕੋ_cert.buf;

	ਅਚਲ ਅੱਖਰ *option;
	ਮਾਪ_t optionlen;
	ਪੂਰਨ options_ਵੇਖਿਆ = 0;

	ਪੂਰਨ retval = 1;

	ਜੇ (!*buf)
		ਵਾਪਸ 1;

	ਜਦੋਂ ((option = ਲੱਭੋ_commit_ਸਿਰਲੇਖ(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_ਵੇਖਿਆ++;
		ਜੇ (options_ਵੇਖਿਆ > ਧੱਕੋ_options->nr
		    || xstrncmpz(ਧੱਕੋ_options->ਚੀਜ਼ਾਂ[options_ਵੇਖਿਆ - 1].string,
				 option, optionlen))
			ਵਾਪਸ 0;
	}

	ਜੇ (options_ਵੇਖਿਆ != ਧੱਕੋ_options->nr)
		retval = 0;

	ਵਾਪਸ retval;
}

ਸਥਿਰ ਖਾਲੀ prepare_ਧੱਕੋ_cert_sha1(ਬਣਤਰ ਲੜੀਵਾਰ_hooks_opt *opt)
{
	ਸਥਿਰ ਪੂਰਨ already_ਪੂਰਾ;

	ਜੇ (!ਧੱਕੋ_cert.len)
		ਵਾਪਸ;

	ਜੇ (!already_ਪੂਰਾ) {
		ਪੂਰਨ bogs /* beginning_of_gpg_sig */;

		already_ਪੂਰਾ = 1;
		ਜੇ (odb_write_object(the_repository->objects, ਧੱਕੋ_cert.buf,
				     ਧੱਕੋ_cert.len, OBJ_BLOB, &ਧੱਕੋ_cert_oid))
			oidclr(&ਧੱਕੋ_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', ਆਕਾਰ(sigcheck));

		bogs = parse_signed_ਬਫਰ(ਧੱਕੋ_cert.buf, ਧੱਕੋ_cert.len);
		sigcheck.payload = xmemdupz(ਧੱਕੋ_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, ਧੱਕੋ_cert.buf + bogs,
				ਧੱਕੋ_cert.len - bogs);

		nonce_ਸਥਿਤੀ = check_nonce(sigcheck.payload);
	}
	ਜੇ (!is_null_oid(&ਧੱਕੋ_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_ਮੰਜ਼ਿਲ_hex(&ਧੱਕੋ_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.ਕੁੰਜੀ ? sigcheck.ਕੁੰਜੀ : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.ਨਤੀਜਾ);
		ਜੇ (ਧੱਕੋ_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     ਧੱਕੋ_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_ਸਥਿਤੀ);
			ਜੇ (nonce_ਸਥਿਤੀ == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

ਬਣਤਰ receive_hook_feed_state {
	ਬਣਤਰ command *cmd;
	ਬਣਤਰ ref_ਧੱਕੋ_report *report;
	ਪੂਰਨ skip_broken;
	ਬਣਤਰ strbuf buf;
};

ਸਥਿਰ ਪੂਰਨ feed_receive_hook_cb(ਪੂਰਨ hook_stdin_fd, ਖਾਲੀ *pp_cb ਅਣਵਰਤਿਆ, ਖਾਲੀ *pp_task_cb)
{
	ਬਣਤਰ receive_hook_feed_state *state = pp_task_cb;
	ਬਣਤਰ command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	ਜਦੋਂ (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->ਅਗਲਾ;

	ਜੇ (!cmd)
		ਵਾਪਸ 1;  /* no more commands left */

	ਜੇ (!state->report)
		state->report = cmd->report;

	ਜੇ (state->report) {
		ਬਣਤਰ object_id *old_oid;
		ਬਣਤਰ object_id *new_oid;
		ਅਚਲ ਅੱਖਰ *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_ਮੰਜ਼ਿਲ_hex(old_oid), oid_ਮੰਜ਼ਿਲ_hex(new_oid),
			    ref_name);

		state->report = state->report->ਅਗਲਾ;
		ਜੇ (!state->report)
			cmd = cmd->ਅਗਲਾ;
	} ਨਹੀਂਤਾਂ {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_ਮੰਜ਼ਿਲ_hex(&cmd->old_oid), oid_ਮੰਜ਼ਿਲ_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->ਅਗਲਾ;
	}

	state->cmd = cmd;

	ਜੇ (state->buf.len > 0) {
		ਪੂਰਨ ret = write_ਵਿੱਚ_ਭਰਿਆ(hook_stdin_fd, state->buf.buf, state->buf.len);
		ਜੇ (ret < 0) {
			ਜੇ (errno == EPIPE)
				ਵਾਪਸ 1; /* child closed pipe */
			ਵਾਪਸ ret;
		}
	}

	ਵਾਪਸ state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

ਸਥਿਰ ਖਾਲੀ *receive_hook_feed_state_alloc(ਖਾਲੀ *feed_pipe_ctx)
{
	ਬਣਤਰ receive_hook_feed_state *init_state = feed_pipe_ctx;
	ਬਣਤਰ receive_hook_feed_state *ਅੰਕੜੇ;

	CALLOC_ARRAY(ਅੰਕੜੇ, 1);
	ਅੰਕੜੇ->report = init_state->report;
	ਅੰਕੜੇ->cmd = init_state->cmd;
	ਅੰਕੜੇ->skip_broken = init_state->skip_broken;
	strbuf_init(&ਅੰਕੜੇ->buf, 0);

	ਵਾਪਸ ਅੰਕੜੇ;
}

ਸਥਿਰ ਖਾਲੀ receive_hook_feed_state_free(ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ receive_hook_feed_state *d = ਅੰਕੜੇ;
	ਜੇ (!d)
		ਵਾਪਸ;
	strbuf_release(&d->buf);
	ਮੁਕਤ(d);
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_receive_hook(ਬਣਤਰ command *commands,
			    ਅਚਲ ਅੱਖਰ *hook_name,
			    ਪੂਰਨ skip_broken,
			    ਬਣਤਰ odb_transaction *transaction,
			    ਅਚਲ ਬਣਤਰ string_list *ਧੱਕੋ_options)
{
	ਬਣਤਰ ਲੜੀਵਾਰ_hooks_opt opt = ਲੜੀਵਾਰ_HOOKS_OPT_INIT;
	ਬਣਤਰ command *iter = commands;
	ਬਣਤਰ receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	ਬਣਤਰ async sideband_async;
	ਪੂਰਨ sideband_async_started = 0;
	ਪੂਰਨ saved_stderr = -1;
	ਪੂਰਨ ret;

	ਜੇ (!hook_exists(the_repository, hook_name))
		ਵਾਪਸ 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	ਜਦੋਂ (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->ਅਗਲਾ;
	ਜੇ (!iter)
		ਵਾਪਸ 0;

	ਜੇ (ਧੱਕੋ_options) {
		ਲਈ (ਪੂਰਨ i = 0; i < ਧੱਕੋ_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     ਧੱਕੋ_options->ਚੀਜ਼ਾਂ[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)ਧੱਕੋ_options->nr);
	} ਨਹੀਂਤਾਂ {
		strvec_ਧੱਕੋ(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	ਜੇ (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_ਧੱਕੋ_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_ਅੰਕੜੇ_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_ਅੰਕੜੇ_free = receive_hook_feed_state_free;

	ret = ਲੜੀਵਾਰ_hooks_opt(the_repository, hook_name, &opt);

	ਅੰਤ_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_update_hook(ਬਣਤਰ command *cmd)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ hook_name[] = "update";
	ਬਣਤਰ ਲੜੀਵਾਰ_hooks_opt opt = ਲੜੀਵਾਰ_HOOKS_OPT_INIT;
	ਬਣਤਰ async sideband_async;
	ਪੂਰਨ sideband_async_started = 0;
	ਪੂਰਨ saved_stderr = -1;
	ਪੂਰਨ ਕੋਡ;

	ਜੇ (!hook_exists(the_repository, hook_name))
		ਵਾਪਸ 0;

	strvec_pushl(&opt.ਦਲੀਲਾਂ,
		     cmd->ref_name,
		     oid_ਮੰਜ਼ਿਲ_hex(&cmd->old_oid),
		     oid_ਮੰਜ਼ਿਲ_hex(&cmd->new_oid),
		     ਨਿਰਾ);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	ਕੋਡ = ਲੜੀਵਾਰ_hooks_opt(the_repository, hook_name, &opt);

	ਅੰਤ_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ਵਾਪਸ ਕੋਡ;
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_receive_report_hook(ਬਣਤਰ strbuf *report)
{
	ਬਣਤਰ ਬੱਚਾ_process proc = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ async sideband_async;
	ਪੂਰਨ sideband_async_started = 0;
	ਪੂਰਨ saved_stderr = -1;
	ਬਣਤਰ strbuf ਬਾਹਰਲਾ = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *hook_ਰਸਤਾ;
	ਪੂਰਨ ret;

	hook_ਰਸਤਾ = ਲੱਭੋ_hook(the_repository, "receive-report");
	ਜੇ (!hook_ਰਸਤਾ)
		ਵਾਪਸ 0;

	strvec_ਧੱਕੋ(&proc.ਦਲੀਲਾਂ, hook_ਰਸਤਾ);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_ਧੱਕੋ(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &ਬਾਹਰਲਾ,
			   report->len, ਨਿਰਾ, 0);
	sigchain_ਕੱਢੋ(SIGPIPE);

	ਅੰਤ_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	ਜੇ (!ret)
		strbuf_ਅਦਲਾ_ਬਦਲੀ(&ਬਾਹਰਲਾ, report);

	strbuf_release(&ਬਾਹਰਲਾ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਬਣਤਰ command *ਲੱਭੋ_command_by_refname(ਬਣਤਰ command *list,
					       ਅਚਲ ਅੱਖਰ *refname)
{
	ਲਈ (; list; list = list->ਅਗਲਾ)
		ਜੇ (!strcmp(list->ref_name, refname))
			ਵਾਪਸ list;
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ read_proc_receive_report(ਬਣਤਰ packet_reader *reader,
				    ਬਣਤਰ command *commands,
				    ਬਣਤਰ strbuf *errmsg)
{
	ਬਣਤਰ command *cmd;
	ਬਣਤਰ command *hint = ਨਿਰਾ;
	ਬਣਤਰ ref_ਧੱਕੋ_report *report = ਨਿਰਾ;
	ਪੂਰਨ new_report = 0;
	ਪੂਰਨ ਕੋਡ = 0;
	ਪੂਰਨ once = 0;
	ਪੂਰਨ response = 0;

	ਲਈ (;;) {
		ਬਣਤਰ object_id old_oid, new_oid;
		ਅੱਖਰ *ਸਿਰ;
		ਅੱਖਰ *refname;
		ਅੱਖਰ *p;
		ਗਿਣਤੀ packet_read_ਸਥਿਤੀ ਸਥਿਤੀ;

		ਸਥਿਤੀ = packet_reader_read(reader);
		ਜੇ (ਸਥਿਤੀ != PACKET_READ_ਸਾਧਾਰਨ) {
			/* Check whether proc-receive exited abnormally */
			ਜੇ (ਸਥਿਤੀ == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				ਵਾਪਸ -1;
			}
			ਰੋਕੋ;
		}
		response++;

		ਸਿਰ = reader->ਸਤਰ;
		p = strchr(ਸਿਰ, ' ');
		ਜੇ (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", ਸਿਰ);
			ਕੋਡ = -1;
			ਜਾਰੀ;
		}
		*p++ = '\0';
		ਜੇ (!strcmp(ਸਿਰ, "option")) {
			ਅੱਖਰ *ਕੁੰਜੀ;
			ਅਚਲ ਅੱਖਰ *val;

			ਜੇ (!hint || !(report || new_report)) {
				ਜੇ (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				ਕੋਡ = -1;
				ਜਾਰੀ;
			}
			ਜੇ (new_report) {
				ਜੇ (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} ਨਹੀਂਤਾਂ {
					report = hint->report;
					ਜਦੋਂ (report->ਅਗਲਾ)
						report = report->ਅਗਲਾ;
					report->ਅਗਲਾ = xcalloc(1, ਆਕਾਰ(ਬਣਤਰ ref_ਧੱਕੋ_report));
					report = report->ਅਗਲਾ;
				}
				new_report = 0;
			}
			ਕੁੰਜੀ = p;
			p = strchr(ਕੁੰਜੀ, ' ');
			ਜੇ (p)
				*p++ = '\0';
			val = p;
			ਜੇ (!strcmp(ਕੁੰਜੀ, "refname"))
				report->ref_name = xstrdup_or_null(val);
			ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ਕੁੰਜੀ, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ਕੁੰਜੀ, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ਕੁੰਜੀ, "forced-update"))
				report->forced_update = 1;
			ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ਕੁੰਜੀ, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->ਲੜੀਵਾਰ_proc_receive = 0;
			ਜਾਰੀ;
		}

		report = ਨਿਰਾ;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		ਜੇ (p)
			*p++ = '\0';
		ਜੇ (strcmp(ਸਿਰ, "ok") && strcmp(ਸਿਰ, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    ਸਿਰ, refname);
			ਕੋਡ = -1;
			ਜਾਰੀ;
		}

		/* first try searching at our hint, falling back to all refs */
		ਜੇ (hint)
			hint = ਲੱਭੋ_command_by_refname(hint, refname);
		ਜੇ (!hint)
			hint = ਲੱਭੋ_command_by_refname(commands, refname);
		ਜੇ (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			ਕੋਡ = -1;
			ਜਾਰੀ;
		}
		ਜੇ (!hint->ਲੜੀਵਾਰ_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			ਕੋਡ = -1;
			ਜਾਰੀ;
		}
		hint->ਲੜੀਵਾਰ_proc_receive |= ਲੜੀਵਾਰ_PROC_RECEIVE_RETURNED;
		ਜੇ (!strcmp(ਸਿਰ, "ng")) {
			ਜੇ (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			ਨਹੀਂਤਾਂ
				hint->error_string = "failed";
			ਕੋਡ = -1;
			ਜਾਰੀ;
		}
		new_report = 1;
	}

	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ)
		ਜੇ (cmd->ਲੜੀਵਾਰ_proc_receive && !cmd->error_string &&
		    !(cmd->ਲੜੀਵਾਰ_proc_receive & ਲੜੀਵਾਰ_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    ਕੋਡ = -1;
		}
	ਵਾਪਸ ਕੋਡ;
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_proc_receive_hook(ਬਣਤਰ command *commands,
				 ਅਚਲ ਬਣਤਰ string_list *ਧੱਕੋ_options)
{
	ਬਣਤਰ ਬੱਚਾ_process proc = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ async muxer;
	ਬਣਤਰ command *cmd;
	ਬਣਤਰ packet_reader reader;
	ਬਣਤਰ strbuf cap = STRBUF_INIT;
	ਬਣਤਰ strbuf errmsg = STRBUF_INIT;
	ਪੂਰਨ hook_use_ਧੱਕੋ_options = 0;
	ਪੂਰਨ ਸੰਸਕਰਣ = 0;
	ਪੂਰਨ ਕੋਡ;
	ਅਚਲ ਅੱਖਰ *hook_ਰਸਤਾ = ਲੱਭੋ_hook(the_repository, "proc-receive");

	ਜੇ (!hook_ਰਸਤਾ) {
		rp_error("cannot find hook 'proc-receive'");
		ਵਾਪਸ -1;
	}

	strvec_ਧੱਕੋ(&proc.ਦਲੀਲਾਂ, hook_ਰਸਤਾ);
	proc.ਵਿੱਚ = -1;
	proc.ਬਾਹਰਲਾ = -1;
	proc.trace2_hook_name = "proc-receive";

	ਜੇ (use_sideband) {
		memset(&muxer, 0, ਆਕਾਰ(muxer));
		muxer.proc = copy_ਮੰਜ਼ਿਲ_sideband;
		muxer.ਵਿੱਚ = -1;
		ਕੋਡ = ਸ਼ੁਰੂ_async(&muxer);
		ਜੇ (ਕੋਡ)
			ਵਾਪਸ ਕੋਡ;
		proc.err = muxer.ਵਿੱਚ;
	} ਨਹੀਂਤਾਂ {
		proc.err = 0;
	}

	ਕੋਡ = ਸ਼ੁਰੂ_command(&proc);
	ਜੇ (ਕੋਡ) {
		ਜੇ (use_sideband)
			ਅੰਤ_async(&muxer);
		ਵਾਪਸ ਕੋਡ;
	}

	sigchain_ਧੱਕੋ(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.ਬਾਹਰਲਾ, ਨਿਰਾ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	ਜੇ (use_atomic)
		strbuf_addstr(&cap, " atomic");
	ਜੇ (use_ਧੱਕੋ_options)
		strbuf_addstr(&cap, " push-options");
	ਜੇ (cap.len) {
		ਕੋਡ = packet_write_fmt_gently(proc.ਵਿੱਚ, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} ਨਹੀਂਤਾਂ {
		ਕੋਡ = packet_write_fmt_gently(proc.ਵਿੱਚ, "version=1\n");
	}
	ਜੇ (!ਕੋਡ)
		ਕੋਡ = packet_flush_gently(proc.ਵਿੱਚ);

	ਜੇ (!ਕੋਡ)
		ਲਈ (;;) {
			ਪੂਰਨ linelen;
			ਗਿਣਤੀ packet_read_ਸਥਿਤੀ ਸਥਿਤੀ;

			ਸਥਿਤੀ = packet_reader_read(&reader);
			ਜੇ (ਸਥਿਤੀ != PACKET_READ_ਸਾਧਾਰਨ) {
				/* Check whether proc-receive exited abnormally */
				ਜੇ (ਸਥਿਤੀ == PACKET_READ_EOF)
					ਕੋਡ = -1;
				ਰੋਕੋ;
			}

			ਜੇ (reader.pktlen > 8 && starts_with(reader.ਸਤਰ, "version=")) {
				ਸੰਸਕਰਣ = atoi(reader.ਸਤਰ + 8);
				linelen = ਲੜੀ_ਲੰਬਾਈ(reader.ਸਤਰ);
				ਜੇ (linelen < reader.pktlen) {
					ਅਚਲ ਅੱਖਰ *feature_list = reader.ਸਤਰ + linelen + 1;
					ਜੇ (parse_feature_request(feature_list, "push-options"))
						hook_use_ਧੱਕੋ_options = 1;
				}
			}
		}

	ਜੇ (ਕੋਡ) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		ਜਾਓ cleanup;
	}

	ਚੋਣ (ਸੰਸਕਰਣ) {
	ਹਾਲਤ 0:
		/* fallthrough */
	ਹਾਲਤ 1:
		ਰੋਕੋ;
	ਮੂਲ:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    ਸੰਸਕਰਣ);
		ਕੋਡ = -1;
		ਜਾਓ cleanup;
	}

	/* Send commands */
	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		ਜੇ (!cmd->ਲੜੀਵਾਰ_proc_receive || cmd->skip_update || cmd->error_string)
			ਜਾਰੀ;
		ਕੋਡ = packet_write_fmt_gently(proc.ਵਿੱਚ, "%s %s %s",
					       oid_ਮੰਜ਼ਿਲ_hex(&cmd->old_oid),
					       oid_ਮੰਜ਼ਿਲ_hex(&cmd->new_oid),
					       cmd->ref_name);
		ਜੇ (ਕੋਡ)
			ਰੋਕੋ;
	}
	ਜੇ (!ਕੋਡ)
		ਕੋਡ = packet_flush_gently(proc.ਵਿੱਚ);
	ਜੇ (ਕੋਡ) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		ਜਾਓ cleanup;
	}

	/* Send push options */
	ਜੇ (hook_use_ਧੱਕੋ_options) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, ਧੱਕੋ_options) {
			ਕੋਡ = packet_write_fmt_gently(proc.ਵਿੱਚ, "%s", ਵਸਤੂ->string);
			ਜੇ (ਕੋਡ)
				ਰੋਕੋ;
		}
		ਜੇ (!ਕੋਡ)
			ਕੋਡ = packet_flush_gently(proc.ਵਿੱਚ);
		ਜੇ (ਕੋਡ) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			ਜਾਓ cleanup;
		}
	}

	/* Read result from proc-receive */
	ਕੋਡ = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.ਵਿੱਚ);
	close(proc.ਬਾਹਰਲਾ);
	ਜੇ (use_sideband)
		ਅੰਤ_async(&muxer);
	ਜੇ (ਅੰਤ_command(&proc))
		ਕੋਡ = -1;
	ਜੇ (errmsg.len >0) {
		ਅੱਖਰ *p = errmsg.buf;

		p += errmsg.len - 1;
		ਜੇ (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_ਕੱਢੋ(SIGPIPE);

	ਵਾਪਸ ਕੋਡ;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *refuse_unconfigured_deny_ਸੁਨੇਹਾ =
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

ਸਥਿਰ ਖਾਲੀ refuse_unconfigured_deny(ਖਾਲੀ)
{
	rp_error("%s", _(refuse_unconfigured_deny_ਸੁਨੇਹਾ));
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *refuse_unconfigured_deny_delete_current_ਸੁਨੇਹਾ =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

ਸਥਿਰ ਖਾਲੀ refuse_unconfigured_deny_delete_current(ਖਾਲੀ)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_ਸੁਨੇਹਾ));
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ object_id *command_singleton_iterator(ਖਾਲੀ *cb_ਅੰਕੜੇ);
ਸਥਿਰ ਪੂਰਨ update_shallow_ref(ਬਣਤਰ command *cmd, ਬਣਤਰ shallow_info *si)
{
	ਬਣਤਰ shallow_ਤਾਲਾ shallow_ਤਾਲਾ = SHALLOW_ਤਾਲਾ_INIT;
	ਬਣਤਰ oid_array extra = OID_ARRAY_INIT;
	ਬਣਤਰ check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t ਨਕਾਬ = 1 << (cmd->ਸੂਚਕਾਂਕ % 32);
	ਪੂਰਨ i;

	trace_printf_ਕੁੰਜੀ(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	ਲਈ (i = 0; i < si->shallow->nr; i++)
		ਜੇ (si->ਵਰਤਿਆ_shallow[i] &&
		    (si->ਵਰਤਿਆ_shallow[i][cmd->ਸੂਚਕਾਂਕ / 32] & ਨਕਾਬ) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	ਤਿਆਰੀ_alternate_shallow(&shallow_ਤਾਲਾ, &opt.shallow_file, &extra);
	ਜੇ (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_ਤਾਲਾ);
		oid_array_clear(&extra);
		ਵਾਪਸ -1;
	}

	commit_shallow_file(the_repository, &shallow_ਤਾਲਾ);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	ਲਈ (i = 0; i < extra.nr; i++)
		ਰਜਿਸਟਰ_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->ਸੂਚਕਾਂਕ] = 0;
	oid_array_clear(&extra);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਧੱਕੋ_ਮੰਜ਼ਿਲ_deploy(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *sha1,
				  ਬਣਤਰ strvec *env,
				  ਅਚਲ ਬਣਤਰ worktree *worktree)
{
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;

	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "update-index", "-q", "--ignore-submodules",
		     "--refresh", ਨਿਰਾ);
	strvec_pushv(&ਬੱਚਾ.env, env->v);
	ਬੱਚਾ.dir = worktree->ਰਸਤਾ;
	ਬੱਚਾ.no_stdin = 1;
	ਬੱਚਾ.stdout_ਮੰਜ਼ਿਲ_stderr = 1;
	ਬੱਚਾ.git_cmd = 1;
	ਜੇ (ਲੜੀਵਾਰ_command(&ਬੱਚਾ))
		ਵਾਪਸ "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	ਬੱਚਾ_process_init(&ਬੱਚਾ);
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "diff-files", "--quiet",
		     "--ignore-submodules", "--", ਨਿਰਾ);
	strvec_pushv(&ਬੱਚਾ.env, env->v);
	ਬੱਚਾ.dir = worktree->ਰਸਤਾ;
	ਬੱਚਾ.no_stdin = 1;
	ਬੱਚਾ.stdout_ਮੰਜ਼ਿਲ_stderr = 1;
	ਬੱਚਾ.git_cmd = 1;
	ਜੇ (ਲੜੀਵਾਰ_command(&ਬੱਚਾ))
		ਵਾਪਸ "Working directory has unstaged changes";

	ਬੱਚਾ_process_init(&ਬੱਚਾ);
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "diff-index", "--quiet", "--cached",
		     "--ignore-submodules",
		     /*
		      * diff-index with either HEAD or an empty tree
		      *
		      * NEEDSWORK: is_null_oid() cannot know whether it's an
		      * unborn HEAD or a corrupt ref. It works for now because
		      * it's only needed to know if we are comparing HEAD or an
		      * empty tree.
		      */
		     !is_null_oid(&worktree->ਸਿਰ_oid) ? "HEAD" :
		     ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ_oid_hex(the_repository->hash_algo), "--", ਨਿਰਾ);
	strvec_pushv(&ਬੱਚਾ.env, env->v);
	ਬੱਚਾ.no_stdin = 1;
	ਬੱਚਾ.no_stdout = 1;
	ਬੱਚਾ.stdout_ਮੰਜ਼ਿਲ_stderr = 0;
	ਬੱਚਾ.git_cmd = 1;
	ਜੇ (ਲੜੀਵਾਰ_command(&ਬੱਚਾ))
		ਵਾਪਸ "Working directory has staged changes";

	ਬੱਚਾ_process_init(&ਬੱਚਾ);
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "read-tree", "-u", "-m", hash_ਮੰਜ਼ਿਲ_hex(sha1),
		     ਨਿਰਾ);
	strvec_pushv(&ਬੱਚਾ.env, env->v);
	ਬੱਚਾ.dir = worktree->ਰਸਤਾ;
	ਬੱਚਾ.no_stdin = 1;
	ਬੱਚਾ.no_stdout = 1;
	ਬੱਚਾ.stdout_ਮੰਜ਼ਿਲ_stderr = 0;
	ਬੱਚਾ.git_cmd = 1;
	ਜੇ (ਲੜੀਵਾਰ_command(&ਬੱਚਾ))
		ਵਾਪਸ "Could not update working tree to new HEAD";

	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਧੱਕੋ_ਮੰਜ਼ਿਲ_checkout_hook = "push-to-checkout";

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਧੱਕੋ_ਮੰਜ਼ਿਲ_checkout(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *hash,
				    ਪੂਰਨ *invoked_hook,
				    ਬਣਤਰ strvec *env,
				    ਅਚਲ ਅੱਖਰ *work_ਰੁੱਖ)
{
	ਬਣਤਰ ਲੜੀਵਾਰ_hooks_opt opt = ਲੜੀਵਾਰ_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_ਰਸਤਾ(work_ਰੁੱਖ));
	strvec_ਧੱਕੋ(&opt.ਦਲੀਲਾਂ, hash_ਮੰਜ਼ਿਲ_hex(hash));
	ਜੇ (ਲੜੀਵਾਰ_hooks_opt(the_repository, ਧੱਕੋ_ਮੰਜ਼ਿਲ_checkout_hook, &opt))
		ਵਾਪਸ "push-to-checkout hook declined";
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *update_worktree(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *sha1, ਅਚਲ ਬਣਤਰ worktree *worktree)
{
	ਅਚਲ ਅੱਖਰ *retval;
	ਅੱਖਰ *git_dir;
	ਬਣਤਰ strvec env = STRVEC_INIT;
	ਪੂਰਨ invoked_hook;

	ਜੇ (!worktree || !worktree->ਰਸਤਾ)
		BUG("worktree->path must be non-NULL");

	ਜੇ (worktree->is_bare)
		ਵਾਪਸ "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_ਰਸਤਾ(git_dir));

	retval = ਧੱਕੋ_ਮੰਜ਼ਿਲ_checkout(sha1, &invoked_hook, &env, worktree->ਰਸਤਾ);
	ਜੇ (!invoked_hook)
		retval = ਧੱਕੋ_ਮੰਜ਼ਿਲ_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	ਮੁਕਤ(git_dir);
	ਵਾਪਸ retval;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *update(ਬਣਤਰ command *cmd, ਬਣਤਰ shallow_info *si)
{
	ਅਚਲ ਅੱਖਰ *name = cmd->ref_name;
	ਬਣਤਰ strbuf namespaced_name_buf = STRBUF_INIT;
	ਸਥਿਰ ਅੱਖਰ *namespaced_name;
	ਅਚਲ ਅੱਖਰ *ret;
	ਬਣਤਰ object_id *old_oid = &cmd->old_oid;
	ਬਣਤਰ object_id *new_oid = &cmd->new_oid;
	ਪੂਰਨ do_update_worktree = 0;
	ਬਣਤਰ worktree **worktrees = get_worktrees(the_repository);
	ਅਚਲ ਬਣਤਰ worktree *worktree =
		ਲੱਭੋ_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	ਜੇ (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	ਮੁਕਤ(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, ਨਿਰਾ);

	ਜੇ (worktree && !worktree->is_bare) {
		ਚੋਣ (deny_current_branch) {
		ਹਾਲਤ DENY_IGNORE:
			ਰੋਕੋ;
		ਹਾਲਤ DENY_WARN:
			rp_warning("updating the current branch");
			ਰੋਕੋ;
		ਹਾਲਤ DENY_REFUSE:
		ਹਾਲਤ DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			ਜੇ (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			ਜਾਓ ਬਾਹਰਲਾ;
		ਹਾਲਤ DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			ਰੋਕੋ;
		}
	}

	ਜੇ (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_ਮੰਜ਼ਿਲ_hex(new_oid));
		ret = "bad pack";
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		ਜੇ (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		ਜੇ (worktree || (ਸਿਰ_name && !strcmp(namespaced_name, ਸਿਰ_name))) {
			ਚੋਣ (deny_delete_current) {
			ਹਾਲਤ DENY_IGNORE:
				ਰੋਕੋ;
			ਹਾਲਤ DENY_WARN:
				rp_warning("deleting the current branch");
				ਰੋਕੋ;
			ਹਾਲਤ DENY_REFUSE:
			ਹਾਲਤ DENY_UNCONFIGURED:
			ਹਾਲਤ DENY_UPDATE_INSTEAD:
				ਜੇ (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				ਜਾਓ ਬਾਹਰਲਾ;
			ਮੂਲ:
				ret = "Invalid denyDeleteCurrent setting";
				ਜਾਓ ਬਾਹਰਲਾ;
			}
		}
	}

	ਜੇ (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		ਬਣਤਰ object *old_object, *new_object;
		ਬਣਤਰ commit *old_commit, *new_commit;
		ਪੂਰਨ ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		ਜੇ (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		old_commit = (ਬਣਤਰ commit *)old_object;
		new_commit = (ਬਣਤਰ commit *)new_object;
		ret2 = repo_ਵਿੱਚ_ਮਿਲਾਓ_bases(the_repository, old_commit, new_commit);
		ਜੇ (ret2 < 0)
			ਬਾਹਰ_ਨਿਕਲੋ(128);
		ਜੇ (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}
	ਜੇ (ਲੜੀਵਾਰ_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		ਜੇ (ret)
			ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (is_null_oid(new_oid)) {
		ਬਣਤਰ strbuf err = STRBUF_INIT;
		ਜੇ (!parse_object(the_repository, old_oid)) {
			old_oid = ਨਿਰਾ;
			ਜੇ (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} ਨਹੀਂਤਾਂ {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		ਜੇ (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   ਨਿਰਾ, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} ਨਹੀਂਤਾਂ {
			ret = ਨਿਰਾ; /* good */
		}
		strbuf_release(&err);
	} ਨਹੀਂਤਾਂ {
		ਗਿਣਤੀ ref_transaction_error tx_err;
		ਬਣਤਰ strbuf err = STRBUF_INIT;
		ਜੇ (shallow_update && si->shallow_ref[cmd->ਸੂਚਕਾਂਕ] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  ਨਿਰਾ, ਨਿਰਾ,
						  0, "push",
						  &err);
		ਜੇ (tx_err) {
			rp_error("%s", err.buf);
			ਜੇ (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			ਨਹੀਂਤਾਂ
				ret = ref_transaction_error_ਸੁਨੇਹਾ(tx_err);
		} ਨਹੀਂਤਾਂ {
			ret = ਨਿਰਾ; /* good */
		}
		strbuf_release(&err);
	}

ਬਾਹਰਲਾ:
	free_worktrees(worktrees);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_update_post_hook(ਬਣਤਰ command *commands)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ hook_name[] = "post-update";
	ਬਣਤਰ ਲੜੀਵਾਰ_hooks_opt opt = ਲੜੀਵਾਰ_HOOKS_OPT_INIT;
	ਬਣਤਰ async sideband_async;
	ਬਣਤਰ command *cmd;
	ਪੂਰਨ sideband_async_started = 0;
	ਪੂਰਨ saved_stderr = -1;

	ਜੇ (!hook_exists(the_repository, hook_name))
		ਵਾਪਸ;

	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		ਜੇ (cmd->error_string || cmd->did_not_exist)
			ਜਾਰੀ;
		strvec_ਧੱਕੋ(&opt.ਦਲੀਲਾਂ, cmd->ref_name);
	}
	ਜੇ (!opt.ਦਲੀਲਾਂ.nr)
		ਵਾਪਸ;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	ਲੜੀਵਾਰ_hooks_opt(the_repository, hook_name, &opt);

	ਅੰਤ_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

ਸਥਿਰ ਖਾਲੀ check_aliased_update_ਅੰਦਰੂਨੀ(ਬਣਤਰ command *cmd,
					  ਬਣਤਰ string_list *list,
					  ਅਚਲ ਅੱਖਰ *dst_name, ਪੂਰਨ flag)
{
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਬਣਤਰ command *dst_cmd;

	ਜੇ (!(flag & REF_ISSYMREF))
		ਵਾਪਸ;

	ਜੇ (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		ਵਾਪਸ;
	}
	dst_name = strip_namespace(dst_name);

	ਜੇ (!(ਵਸਤੂ = string_list_lookup(list, dst_name)))
		ਵਾਪਸ;

	cmd->skip_update = 1;

	dst_cmd = (ਬਣਤਰ command *) ਵਸਤੂ->util;

	ਜੇ (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		ਵਾਪਸ;

	dst_cmd->skip_update = 1;

	rp_error("refusing inconsistent update between symref '%s' (%s..%s) and"
		 " its target '%s' (%s..%s)",
		 cmd->ref_name,
		 repo_ਲੱਭੋ_unique_abbrev(the_repository, &cmd->old_oid, DEFAULT_ABBREV),
		 repo_ਲੱਭੋ_unique_abbrev(the_repository, &cmd->new_oid, DEFAULT_ABBREV),
		 dst_cmd->ref_name,
		 repo_ਲੱਭੋ_unique_abbrev(the_repository, &dst_cmd->old_oid, DEFAULT_ABBREV),
		 repo_ਲੱਭੋ_unique_abbrev(the_repository, &dst_cmd->new_oid, DEFAULT_ABBREV));

	cmd->error_string = dst_cmd->error_string =
		"inconsistent aliased update";
}

ਸਥਿਰ ਖਾਲੀ check_aliased_update(ਬਣਤਰ command *cmd, ਬਣਤਰ string_list *list)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *dst_name;
	ਪੂਰਨ flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, ਨਿਰਾ, &flag);
	check_aliased_update_ਅੰਦਰੂਨੀ(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

ਸਥਿਰ ਖਾਲੀ check_aliased_updates(ਬਣਤਰ command *commands)
{
	ਬਣਤਰ command *cmd;
	ਬਣਤਰ string_list ref_list = STRING_LIST_INIT_NODUP;

	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ =
			string_list_append(&ref_list, cmd->ref_name);
		ਵਸਤੂ->util = (ਖਾਲੀ *)cmd;
	}
	string_list_ਛਾਂਟੋ(&ref_list);

	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		ਜੇ (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ object_id *command_singleton_iterator(ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ command **cmd_list = cb_ਅੰਕੜੇ;
	ਬਣਤਰ command *cmd = *cmd_list;

	ਜੇ (!cmd || is_null_oid(&cmd->new_oid))
		ਵਾਪਸ ਨਿਰਾ;
	*cmd_list = ਨਿਰਾ; /* this returns only one */
	ਵਾਪਸ &cmd->new_oid;
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_connectivity_errors(ਬਣਤਰ command *commands,
				    ਬਣਤਰ shallow_info *si,
				    ਬਣਤਰ odb_transaction *transaction)
{
	ਬਣਤਰ command *cmd;

	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		ਬਣਤਰ command *singleton = cmd;
		ਬਣਤਰ check_connected_options opt = CHECK_CONNECTED_INIT;
		ਬਣਤਰ strvec env = STRVEC_INIT;

		ਜੇ (shallow_update && si->shallow_ref[cmd->ਸੂਚਕਾਂਕ])
			/* to be checked in update_shallow_ref() */
			ਜਾਰੀ;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		ਜੇ (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			ਜਾਰੀ;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

ਬਣਤਰ iterate_ਅੰਕੜੇ {
	ਬਣਤਰ command *cmds;
	ਬਣਤਰ shallow_info *si;
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ object_id *iterate_receive_command_list(ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ iterate_ਅੰਕੜੇ *ਅੰਕੜੇ = cb_ਅੰਕੜੇ;
	ਬਣਤਰ command **cmd_list = &ਅੰਕੜੇ->cmds;
	ਬਣਤਰ command *cmd = *cmd_list;

	ਲਈ (; cmd; cmd = cmd->ਅਗਲਾ) {
		ਜੇ (shallow_update && ਅੰਕੜੇ->si->shallow_ref[cmd->ਸੂਚਕਾਂਕ])
			/* to be checked in update_shallow_ref() */
			ਜਾਰੀ;
		ਜੇ (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->ਅਗਲਾ;
			ਵਾਪਸ &cmd->new_oid;
		}
	}
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ reject_updates_ਮੰਜ਼ਿਲ_hidden(ਬਣਤਰ command *commands)
{
	ਬਣਤਰ strbuf refname_ਭਰਿਆ = STRBUF_INIT;
	ਮਾਪ_t ਅਗੇਤਰ_len;
	ਬਣਤਰ command *cmd;

	strbuf_addstr(&refname_ਭਰਿਆ, get_git_namespace());
	ਅਗੇਤਰ_len = refname_ਭਰਿਆ.len;

	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		ਜੇ (cmd->error_string)
			ਜਾਰੀ;

		strbuf_setlen(&refname_ਭਰਿਆ, ਅਗੇਤਰ_len);
		strbuf_addstr(&refname_ਭਰਿਆ, cmd->ref_name);

		ਜੇ (!ref_is_hidden(cmd->ref_name, refname_ਭਰਿਆ.buf, &hidden_refs))
			ਜਾਰੀ;
		ਜੇ (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		ਨਹੀਂਤਾਂ
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_ਭਰਿਆ);
}

ਸਥਿਰ ਪੂਰਨ should_process_cmd(ਬਣਤਰ command *cmd)
{
	ਵਾਪਸ !cmd->error_string && !cmd->skip_update;
}

ਸਥਿਰ ਖਾਲੀ BUG_if_skipped_connectivity_check(ਬਣਤਰ command *commands,
					       ਬਣਤਰ shallow_info *si)
{
	ਬਣਤਰ command *cmd;

	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		ਜੇ (should_process_cmd(cmd) && si->shallow_ref[cmd->ਸੂਚਕਾਂਕ])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

ਸਥਿਰ ਖਾਲੀ ref_transaction_rejection_handler(ਅਚਲ ਅੱਖਰ *refname,
					      ਅਚਲ ਬਣਤਰ object_id *old_oid ਅਣਵਰਤਿਆ,
					      ਅਚਲ ਬਣਤਰ object_id *new_oid ਅਣਵਰਤਿਆ,
					      ਅਚਲ ਅੱਖਰ *old_ਨਿਸ਼ਾਨਾ ਅਣਵਰਤਿਆ,
					      ਅਚਲ ਅੱਖਰ *new_ਨਿਸ਼ਾਨਾ ਅਣਵਰਤਿਆ,
					      ਗਿਣਤੀ ref_transaction_error err,
					      ਅਚਲ ਅੱਖਰ *details,
					      ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ strmap *failed_refs = cb_ਅੰਕੜੇ;

	ਜੇ (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (ਅੱਖਰ *)ref_transaction_error_ਸੁਨੇਹਾ(err));
}

ਸਥਿਰ ਖਾਲੀ execute_commands_non_atomic(ਬਣਤਰ command *commands,
					ਬਣਤਰ shallow_info *si)
{
	ਬਣਤਰ command *cmd;
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *reported_error = ਨਿਰਾ;
	ਬਣਤਰ strmap failed_refs = STRMAP_INIT;

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
	ਗਿਣਤੀ processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	ਲਈ (ਗਿਣਤੀ processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
			ਜੇ (!should_process_cmd(cmd) || cmd->ਲੜੀਵਾਰ_proc_receive)
				ਜਾਰੀ;

			ਜੇ (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				ਜਾਰੀ;
			ਨਹੀਂਤਾਂ ਜੇ (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				ਜਾਰੀ;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			ਜੇ (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				ਜੇ (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					ਜਾਓ failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		ਜੇ (!transaction)
			ਜਾਓ cleanup;

		ਜੇ (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			ਜਾਓ failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		ਜੇ (strmap_ਖਾਲੀ_ਥਾਵਾਂ(&failed_refs))
			ਜਾਓ cleanup;

	failure:
		ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
			ਜੇ (reported_error)
				cmd->error_string = reported_error;
			ਨਹੀਂਤਾਂ ਜੇ (strmap_ਸ਼ਾਮਲ_ਹੈ(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = ਨਿਰਾ;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

ਸਥਿਰ ਖਾਲੀ execute_commands_atomic(ਬਣਤਰ command *commands,
					ਬਣਤਰ shallow_info *si)
{
	ਬਣਤਰ command *cmd;
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ਜੇ (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		ਜਾਓ failure;
	}

	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		ਜੇ (!should_process_cmd(cmd) || cmd->ਲੜੀਵਾਰ_proc_receive)
			ਜਾਰੀ;

		cmd->error_string = update(cmd, si);

		ਜੇ (cmd->error_string)
			ਜਾਓ failure;
	}

	ਜੇ (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		ਜਾਓ failure;
	}
	ਜਾਓ cleanup;

failure:
	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ)
		ਜੇ (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

ਸਥਿਰ ਖਾਲੀ execute_commands(ਬਣਤਰ command *commands,
			     ਪੂਰਨ unpacker_error,
			     ਬਣਤਰ shallow_info *si,
			     ਬਣਤਰ odb_transaction *transaction,
			     ਅਚਲ ਬਣਤਰ string_list *ਧੱਕੋ_options)
{
	ਬਣਤਰ check_connected_options opt = CHECK_CONNECTED_INIT;
	ਬਣਤਰ command *cmd;
	ਬਣਤਰ iterate_ਅੰਕੜੇ ਅੰਕੜੇ;
	ਬਣਤਰ async muxer;
	ਪੂਰਨ err_fd = 0;
	ਪੂਰਨ ਲੜੀਵਾਰ_proc_receive = 0;

	ਜੇ (unpacker_error) {
		ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ)
			cmd->error_string = "unpacker error";
		ਵਾਪਸ;
	}

	ਜੇ (!skip_connectivity_check) {
		ਬਣਤਰ strvec env = STRVEC_INIT;

		ਜੇ (use_sideband) {
			memset(&muxer, 0, ਆਕਾਰ(muxer));
			muxer.proc = copy_ਮੰਜ਼ਿਲ_sideband;
			muxer.ਵਿੱਚ = -1;
			ਜੇ (!ਸ਼ੁਰੂ_async(&muxer))
				err_fd = muxer.ਵਿੱਚ;
			/* ...else, continue without relaying sideband */
		}

		ਅੰਕੜੇ.cmds = commands;
		ਅੰਕੜੇ.si = si;
		opt.err_fd = err_fd;
		opt.ਤਰੱਕੀ = err_fd && !quiet;
		odb_transaction_env(transaction, &env);
		opt.env = env.v;
		opt.exclude_hidden_refs_section = "receive";

		ਜੇ (check_connected(iterate_receive_command_list, &ਅੰਕੜੇ, &opt))
			ਸੈੱਟ_connectivity_errors(commands, si, transaction);

		ਜੇ (use_sideband)
			ਅੰਤ_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_ਮੰਜ਼ਿਲ_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	ਜੇ (proc_receive_ref) {
		ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
			ਜੇ (!should_process_cmd(cmd))
				ਜਾਰੀ;

			ਜੇ (proc_receive_ref_matches(cmd)) {
				cmd->ਲੜੀਵਾਰ_proc_receive = ਲੜੀਵਾਰ_PROC_RECEIVE_SCHEDULED;
				ਲੜੀਵਾਰ_proc_receive = 1;
			}
		}
	}

	ਜੇ (ਲੜੀਵਾਰ_receive_hook(commands, "pre-receive", 0, transaction, ਧੱਕੋ_options)) {
		ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
			ਜੇ (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		ਵਾਪਸ;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	ਲਈ (cmd = commands; cmd && cmd->error_string; cmd = cmd->ਅਗਲਾ)
		; /* nothing */
	ਜੇ (!cmd)
		ਵਾਪਸ;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	ਜੇ (odb_transaction_commit(transaction)) {
		ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
			ਜੇ (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		ਵਾਪਸ;
	}

	check_aliased_updates(commands);

	ਮੁਕਤ(ਸਿਰ_name_ਮੰਜ਼ਿਲ_free);
	ਸਿਰ_name = ਸਿਰ_name_ਮੰਜ਼ਿਲ_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, ਨਿਰਾ,
							    ਨਿਰਾ);

	ਜੇ (ਲੜੀਵਾਰ_proc_receive &&
	    ਲੜੀਵਾਰ_proc_receive_hook(commands, ਧੱਕੋ_options))
		ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ)
			ਜੇ (!cmd->error_string &&
			    !(cmd->ਲੜੀਵਾਰ_proc_receive & ਲੜੀਵਾਰ_PROC_RECEIVE_RETURNED) &&
			    (cmd->ਲੜੀਵਾਰ_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	ਜੇ (use_atomic)
		execute_commands_atomic(commands, si);
	ਨਹੀਂਤਾਂ
		execute_commands_non_atomic(commands, si);

	ਜੇ (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

ਸਥਿਰ ਬਣਤਰ command **ਕਤਾਰ_command(ਬਣਤਰ command **ਪੂਛ,
				      ਅਚਲ ਅੱਖਰ *ਸਤਰ,
				      ਪੂਰਨ linelen)
{
	ਬਣਤਰ object_id old_oid, new_oid;
	ਬਣਤਰ command *cmd;
	ਅਚਲ ਅੱਖਰ *refname;
	ਪੂਰਨ reflen;
	ਅਚਲ ਅੱਖਰ *p;

	ਜੇ (parse_oid_hex(ਸਤਰ, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", ਸਤਰ);

	refname = p;
	reflen = linelen - (p - ਸਤਰ);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*ਪੂਛ = cmd;
	ਵਾਪਸ &cmd->ਅਗਲਾ;
}

ਸਥਿਰ ਖਾਲੀ free_commands(ਬਣਤਰ command *commands)
{
	ਜਦੋਂ (commands) {
		ਬਣਤਰ command *ਅਗਲਾ = commands->ਅਗਲਾ;

		ref_ਧੱਕੋ_report_free(commands->report);
		ਮੁਕਤ(commands->error_string_owned);
		ਮੁਕਤ(commands);
		commands = ਅਗਲਾ;
	}
}

ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_commands_from_cert(ਬਣਤਰ command **ਪੂਛ,
				     ਬਣਤਰ strbuf *ਧੱਕੋ_cert)
{
	ਅਚਲ ਅੱਖਰ *boc, *eoc;

	ਜੇ (*ਪੂਛ)
		die("protocol error: got both push certificate and unsigned commands");

	boc = ਲੜੀ_ਖੋਜ(ਧੱਕੋ_cert->buf, "\n\n");
	ਜੇ (!boc)
		die("malformed push certificate %.*s", 100, ਧੱਕੋ_cert->buf);
	ਨਹੀਂਤਾਂ
		boc += 2;
	eoc = ਧੱਕੋ_cert->buf + parse_signed_ਬਫਰ(ਧੱਕੋ_cert->buf, ਧੱਕੋ_cert->len);

	ਜਦੋਂ (boc < eoc) {
		ਅਚਲ ਅੱਖਰ *eol = memchr(boc, '\n', eoc - boc);
		ਪੂਛ = ਕਤਾਰ_command(ਪੂਛ, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

ਸਥਿਰ ਬਣਤਰ command *read_ਸਿਰ_info(ਬਣਤਰ packet_reader *reader,
				      ਬਣਤਰ oid_array *shallow,
				      ਗਿਣਤੀ report_ਸਥਿਤੀ_ਸੰਸਕਰਣ *ਸੰਸਕਰਣ)
{
	ਬਣਤਰ command *commands = ਨਿਰਾ;
	ਬਣਤਰ command **p = &commands;
	ਲਈ (;;) {
		ਪੂਰਨ linelen;

		ਜੇ (packet_reader_read(reader) != PACKET_READ_ਸਾਧਾਰਨ)
			ਰੋਕੋ;

		ਜੇ (reader->pktlen > 8 && starts_with(reader->ਸਤਰ, "shallow ")) {
			ਬਣਤਰ object_id oid;
			ਜੇ (get_oid_hex(reader->ਸਤਰ + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->ਸਤਰ + 8);
			oid_array_append(shallow, &oid);
			ਜਾਰੀ;
		}

		linelen = ਲੜੀ_ਲੰਬਾਈ(reader->ਸਤਰ);
		ਜੇ (linelen < reader->pktlen) {
			ਅਚਲ ਅੱਖਰ *feature_list = reader->ਸਤਰ + linelen + 1;
			ਅਚਲ ਅੱਖਰ *hash = ਨਿਰਾ;
			ਅਚਲ ਅੱਖਰ *client_sid;
			ਮਾਪ_t len = 0;
			ਜੇ (parse_feature_request(feature_list, "report-status"))
				*ਸੰਸਕਰਣ = REPORT_ਸਥਿਤੀ_V0;
			ਜੇ (parse_feature_request(feature_list, "report-status-v2"))
				*ਸੰਸਕਰਣ = REPORT_ਸਥਿਤੀ_V2;
			ਜੇ (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			ਜੇ (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			ਜੇ (advertise_atomic_ਧੱਕੋ
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			ਜੇ (advertise_ਧੱਕੋ_options
			    && parse_feature_request(feature_list, "push-options"))
				use_ਧੱਕੋ_options = 1;
			hash = parse_feature_ਮੁੱਲ(feature_list, "object-format", &len, ਨਿਰਾ);
			ਜੇ (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = ਲੜੀ_ਲੰਬਾਈ(hash);
			}
			ਜੇ (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_ਮੁੱਲ(feature_list, "session-id", &len, ਨਿਰਾ);
			ਜੇ (client_sid) {
				ਅੱਖਰ *sid = xstrndup(client_sid, len);
				trace2_ਅੰਕੜੇ_string("transfer", ਨਿਰਾ, "client-sid", client_sid);
				ਮੁਕਤ(sid);
			}
		}

		ਜੇ (!strcmp(reader->ਸਤਰ, "push-cert")) {
			ਪੂਰਨ true_flush = 0;
			ਪੂਰਨ saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			ਲਈ (;;) {
				packet_reader_read(reader);
				ਜੇ (reader->ਸਥਿਤੀ == PACKET_READ_FLUSH) {
					true_flush = 1;
					ਰੋਕੋ;
				}
				ਜੇ (reader->ਸਥਿਤੀ != PACKET_READ_ਸਾਧਾਰਨ) {
					die("protocol error: got an unexpected packet");
				}
				ਜੇ (!strcmp(reader->ਸਤਰ, "push-cert-end\n"))
					ਰੋਕੋ; /* end of cert */
				strbuf_addstr(&ਧੱਕੋ_cert, reader->ਸਤਰ);
			}
			reader->options = saved_options;

			ਜੇ (true_flush)
				ਰੋਕੋ;
			ਜਾਰੀ;
		}

		p = ਕਤਾਰ_command(p, reader->ਸਤਰ, linelen);
	}

	ਜੇ (ਧੱਕੋ_cert.len)
		ਕਤਾਰ_commands_from_cert(p, &ਧੱਕੋ_cert);

	ਵਾਪਸ commands;
}

ਸਥਿਰ ਖਾਲੀ read_ਧੱਕੋ_options(ਬਣਤਰ packet_reader *reader,
			      ਬਣਤਰ string_list *options)
{
	ਜਦੋਂ (1) {
		ਜੇ (packet_reader_read(reader) != PACKET_READ_ਸਾਧਾਰਨ)
			ਰੋਕੋ;

		string_list_append(options, reader->ਸਤਰ);
	}
}

ਸਥਿਰ ਪੂਰਨ unpack_with_sideband(ਬਣਤਰ odb_transaction *transaction,
				ਅਚਲ ਅੱਖਰ *shallow_file,
				ਬਣਤਰ strbuf *err_ਸੁਨੇਹਾ)
{
	ਬਣਤਰ odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_ਸੁਨੇਹਾ_types = fsck_ਸੁਨੇਹਾ_types.buf,
		.max_ਨਿਵੇਸ਼_ਮਾਪ = max_ਨਿਵੇਸ਼_ਮਾਪ,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	ਬਣਤਰ async muxer;
	ਪੂਰਨ ret;

	ਜੇ (!use_sideband)
		ਵਾਪਸ odb_transaction_write_pack(transaction, 0, err_ਸੁਨੇਹਾ, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, ਆਕਾਰ(muxer));
	muxer.proc = copy_ਮੰਜ਼ਿਲ_sideband;
	muxer.ਵਿੱਚ = -1;
	ਜੇ (ਸ਼ੁਰੂ_async(&muxer))
		ਵਾਪਸ 0;

	opts.err_fd = muxer.ਵਿੱਚ;
	ret = odb_transaction_write_pack(transaction, 0, err_ਸੁਨੇਹਾ, &opts);

	ਅੰਤ_async(&muxer);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ prepare_shallow_update(ਬਣਤਰ shallow_info *si,
				   ਅਚਲ ਅੱਖਰ *shallow_file)
{
	ਪੂਰਨ i, j, k, bitmap_ਮਾਪ = DIV_ਗੇੜਾ_ਉੱਪਰ(si->ref->nr, 32);

	ALLOC_ARRAY(si->ਵਰਤਿਆ_shallow, si->shallow->nr);
	ਸੌਂਪੋ_shallow_commits_ਮੰਜ਼ਿਲ_refs(si, si->ਵਰਤਿਆ_shallow, ਨਿਰਾ);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	ਲਈ (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	ਲਈ (i = 0; i < si->shallow->nr; i++) {
		ਜੇ (!si->ਵਰਤਿਆ_shallow[i])
			ਜਾਰੀ;
		ਲਈ (j = 0; j < bitmap_ਮਾਪ; j++) {
			ਜੇ (!si->ਵਰਤਿਆ_shallow[i][j])
				ਜਾਰੀ;
			si->need_reachability_test[i]++;
			ਲਈ (k = 0; k < 32; k++)
				ਜੇ (si->ਵਰਤਿਆ_shallow[i][j] & (1U << k))
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

ਸਥਿਰ ਖਾਲੀ update_shallow_info(ਬਣਤਰ command *commands,
				ਬਣਤਰ shallow_info *si,
				ਬਣਤਰ oid_array *ref,
				ਅਚਲ ਅੱਖਰ *shallow_file)
{
	ਬਣਤਰ command *cmd;
	ਪੂਰਨ *ref_ਸਥਿਤੀ;
	remove_nonexistent_theirs_shallow(si);
	ਜੇ (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		ਵਾਪਸ;
	}

	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		ਜੇ (is_null_oid(&cmd->new_oid))
			ਜਾਰੀ;
		oid_array_append(ref, &cmd->new_oid);
		cmd->ਸੂਚਕਾਂਕ = ref->nr - 1;
	}
	si->ref = ref;

	ਜੇ (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		ਵਾਪਸ;
	}

	ALLOC_ARRAY(ref_ਸਥਿਤੀ, ref->nr);
	ਸੌਂਪੋ_shallow_commits_ਮੰਜ਼ਿਲ_refs(si, ਨਿਰਾ, ref_ਸਥਿਤੀ);
	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		ਜੇ (is_null_oid(&cmd->new_oid))
			ਜਾਰੀ;
		ਜੇ (ref_ਸਥਿਤੀ[cmd->ਸੂਚਕਾਂਕ]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	ਮੁਕਤ(ref_ਸਥਿਤੀ);
}

ਸਥਿਰ ਖਾਲੀ override_cmds_error(ਬਣਤਰ command *commands, ਅਚਲ ਅੱਖਰ *err)
{
	ਲਈ (ਬਣਤਰ command *cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
ਸਥਿਰ ਖਾਲੀ generate_report(ਬਣਤਰ strbuf *buf, ਬਣਤਰ command *commands,
			    ਅਚਲ ਬਣਤਰ strbuf *unpack_ਸਥਿਤੀ,
			    ਗਿਣਤੀ report_ਸਥਿਤੀ_ਸੰਸਕਰਣ ਸੰਸਕਰਣ)
{
	ਬਣਤਰ command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_ਸਥਿਤੀ->len ? unpack_ਸਥਿਤੀ->buf : "ok");

	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		ਬਣਤਰ ref_ਧੱਕੋ_report *report;
		ਪੂਰਨ ਗਿਣਤੀ_2 = 0;

		ਜੇ (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		ਨਹੀਂਤਾਂ
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		ਜੇ (ਸੰਸਕਰਣ != REPORT_ਸਥਿਤੀ_V2 || cmd->error_string)
			ਜਾਰੀ;

		ਲਈ (report = cmd->report; report; report = report->ਅਗਲਾ) {
			ਜੇ (ਗਿਣਤੀ_2++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			ਜੇ (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			ਜੇ (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_ਮੰਜ਼ਿਲ_hex(report->old_oid));
			ਜੇ (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_ਮੰਜ਼ਿਲ_hex(report->new_oid));
			ਜੇ (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

ਸਥਿਰ ਖਾਲੀ report(ਬਣਤਰ command *commands, ਅਚਲ ਬਣਤਰ strbuf *unpack_ਸਥਿਤੀ,
		   ਗਿਣਤੀ report_ਸਥਿਤੀ_ਸੰਸਕਰਣ ਸੰਸਕਰਣ)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_ਸਥਿਤੀ, ਸੰਸਕਰਣ);

	ਜੇ (ਲੜੀਵਾਰ_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_ਸਥਿਤੀ, ਸੰਸਕਰਣ);
	}

	ਜੇ (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	ਨਹੀਂਤਾਂ
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

ਸਥਿਰ ਪੂਰਨ delete_only(ਬਣਤਰ command *commands)
{
	ਬਣਤਰ command *cmd;
	ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ) {
		ਜੇ (!is_null_oid(&cmd->new_oid))
			ਵਾਪਸ 0;
	}
	ਵਾਪਸ 1;
}

ਪੂਰਨ cmd_receive_pack(ਪੂਰਨ argc,
		     ਅਚਲ ਅੱਖਰ **argv,
		     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ advertise_refs = 0;
	ਬਣਤਰ command *commands;
	ਬਣਤਰ oid_array shallow = OID_ARRAY_INIT;
	ਬਣਤਰ oid_array ref = OID_ARRAY_INIT;
	ਬਣਤਰ shallow_info si;
	ਬਣਤਰ packet_reader reader;
	ਬਣਤਰ odb_transaction *transaction = ਨਿਰਾ;
	ਗਿਣਤੀ report_ਸਥਿਤੀ_ਸੰਸਕਰਣ ਸੰਸਕਰਣ = REPORT_ਸਥਿਤੀ_UNKNOWN;

	ਬਣਤਰ option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, ਨਿਰਾ),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, ਨਿਰਾ),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, ਨਿਰਾ),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, ਨਿਰਾ),
		OPT_ਸਿਰਾ()
	};

	packet_trace_ਪਛਾਣ_ਤੱਤ("receive-pack");

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, receive_pack_usage, 0);

	ਜੇ (argc > 1)
		usage_ਸੁਨੇਹਾ_opt(_("too many arguments"), receive_pack_usage, options);
	ਜੇ (argc == 0)
		usage_ਸੁਨੇਹਾ_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	ਤਿਆਰੀ_ਰਸਤਾ();

	ਜੇ (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, ਨਿਰਾ);
	ਜੇ (cert_nonce_ਬੀਜ)
		ਧੱਕੋ_cert_nonce = prepare_ਧੱਕੋ_cert_nonce(service_dir, ਸਮਾਂ(ਨਿਰਾ));

	ਚੋਣ (determine_protocol_ਸੰਸਕਰਣ_server()) {
	ਹਾਲਤ protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		ਰੋਕੋ;
	ਹਾਲਤ protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ਜੇ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	ਹਾਲਤ protocol_v0:
		ਰੋਕੋ;
	ਹਾਲਤ protocol_unknown_ਸੰਸਕਰਣ:
		BUG("unknown protocol version");
	}

	ਜੇ (advertise_refs || !stateless_rpc) {
		write_ਸਿਰ_info();
	}
	ਜੇ (advertise_refs)
		ਵਾਪਸ 0;

	packet_reader_init(&reader, 0, ਨਿਰਾ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ਜੇ ((commands = read_ਸਿਰ_info(&reader, &shallow, &ਸੰਸਕਰਣ))) {
		ਬਣਤਰ string_list ਧੱਕੋ_options = STRING_LIST_INIT_DUP;
		ਬਣਤਰ strbuf unpack_ਸਥਿਤੀ = STRBUF_INIT;

		ਜੇ (use_ਧੱਕੋ_options)
			read_ਧੱਕੋ_options(&reader, &ਧੱਕੋ_options);
		ਜੇ (!check_cert_ਧੱਕੋ_options(&ਧੱਕੋ_options)) {
			ਬਣਤਰ command *cmd;
			ਲਈ (cmd = commands; cmd; cmd = cmd->ਅਗਲਾ)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		ਜੇ (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		ਜੇ (!delete_only(commands)) {
			ਅਚਲ ਅੱਖਰ *alt_shallow_file = ਨਿਰਾ;

			ਜੇ (si.nr_ours || si.nr_theirs)
				alt_shallow_file = ਤਿਆਰੀ_temporary_shallow(si.shallow);

			ਜੇ (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_ਸਥਿਤੀ, "unable to start object transaction");
			ਨਹੀਂਤਾਂ
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_ਸਥਿਤੀ);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_ਸਥਿਤੀ.len, &si, transaction,
				 &ਧੱਕੋ_options);
		odb_transaction_finalize(transaction);
		sigchain_ਧੱਕੋ(SIGPIPE, SIG_IGN);

		ਚੋਣ (ਸੰਸਕਰਣ) {
		ਹਾਲਤ REPORT_ਸਥਿਤੀ_V2:
		ਹਾਲਤ REPORT_ਸਥਿਤੀ_V0:
			report(commands, &unpack_ਸਥਿਤੀ, ਸੰਸਕਰਣ);
			ਰੋਕੋ;
		ਹਾਲਤ REPORT_ਸਥਿਤੀ_UNKNOWN:
			ਰੋਕੋ;
		}

		sigchain_ਕੱਢੋ(SIGPIPE);
		ਲੜੀਵਾਰ_receive_hook(commands, "post-receive", 1, ਨਿਰਾ,
				 &ਧੱਕੋ_options);
		ਲੜੀਵਾਰ_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&ਧੱਕੋ_options, 0);
		ਜੇ (auto_gc) {
			ਬਣਤਰ ਬੱਚਾ_process proc = ਬੱਚਾ_PROCESS_INIT;

			ਜੇ (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_ਮੰਜ਼ਿਲ_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				ਜੇ (!ਸ਼ੁਰੂ_command(&proc)) {
					ਜੇ (use_sideband)
						copy_ਮੰਜ਼ਿਲ_sideband(proc.err, -1, ਨਿਰਾ);
					ਅੰਤ_command(&proc);
				}
			}
		}
		ਜੇ (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_ਸਥਿਤੀ);
	}
	ਜੇ (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	ਮੁਕਤ((ਖਾਲੀ *)ਧੱਕੋ_cert_nonce);
	ਵਾਪਸ 0;
}
