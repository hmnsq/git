#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "connect.h"
#ಅನ್ತರ್ಭಾವ "connected.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "exec-cmd.h"
#ಅನ್ತರ್ಭಾವ "fsck.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "gpg-interface.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "hook.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "oidset.h"
#ಅನ್ತರ್ಭಾವ "pack.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "pkt-line.h"
#ಅನ್ತರ್ಭಾವ "protocol.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "server-info.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "shallow.h"
#ಅನ್ತರ್ಭಾವ "sideband.h"
#ಅನ್ತರ್ಭಾವ "sigchain.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "trace.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"
#ಅನ್ತರ್ಭಾವ "version.h"
#ಅನ್ತರ್ಭಾವ "worktree.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	ರಿಕ್ತ
};

ಗಣನಾ deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

ಗಣನಾ report_status_version {
	REPORT_STATUS_UNKNOWN = 0,
	REPORT_STATUS_V0,
	REPORT_STATUS_V2,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ deny_deletes;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ deny_non_fast_forwards;
ಸ್ಥಿರ ಗಣನಾ deny_action deny_current_branch = DENY_UNCONFIGURED;
ಸ್ಥಿರ ಗಣನಾ deny_action deny_delete_current = DENY_UNCONFIGURED;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ receive_fsck_objects = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ transfer_fsck_objects = -1;
ಸ್ಥಿರ ಸಂರಚನಾ strbuf fsck_ಸಂದೇಶ_types = STRBUF_INIT;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ advertise_atomic_ತಳ್ಳು = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ advertise_ತಳ್ಳು_options;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ advertise_sid;
ಸ್ಥಿರ off_t max_ಒಳಹರಿವು_size;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_sideband;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_atomic;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_ತಳ್ಳು_options;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ quiet;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prefer_ofs_delta = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ auto_update_server_info;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ auto_gc = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reject_thin;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ skip_connectivity_check;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stateless_rpc;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *service_dir;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *head_name;
ಸ್ಥಿರ ಶೂನ್ಯ *head_name_to_free;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sent_capabilities;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ shallow_update;
ಸ್ಥಿರ ಸಂರಚನಾ strbuf ತಳ್ಳು_cert = STRBUF_INIT;
ಸ್ಥಿರ ಸಂರಚನಾ object_id ತಳ್ಳು_cert_oid;
ಸ್ಥಿರ ಸಂರಚನಾ signature_check sigcheck;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ತಳ್ಳು_cert_nonce;
ಸ್ಥಿರ ವರ್ಣ *cert_nonce_seed;
ಸ್ಥಿರ ಸಂರಚನಾ strvec hidden_refs = STRVEC_INIT;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *NONCE_UNSOLICITED = "UNSOLICITED";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *NONCE_BAD = "BAD";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *NONCE_MISSING = "MISSING";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *NONCE_OK = "OK";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *NONCE_SLOP = "SLOP";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *nonce_status;
ಸ್ಥಿರ long nonce_stamp_slop;
ಸ್ಥಿರ timestamp_t nonce_stamp_slop_limit;
ಸ್ಥಿರ ಸಂರಚನಾ ref_transaction *transaction;

ಸ್ಥಿರ ಗಣನಾ {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ keepalive_in_sec = 5;

ಸ್ಥಿರ ಸಂರಚನಾ proc_receive_ref {
	unsigned ಪೂರ್ಣಾಙ್ಕ want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	ವರ್ಣ *ref_prefix;
	ಸಂರಚನಾ proc_receive_ref *next;
} *proc_receive_ref;

ಸ್ಥಿರ ಶೂನ್ಯ proc_receive_ref_append(ಧ್ರುವ ವರ್ಣ *prefix);

ಸ್ಥಿರ ಗಣನಾ deny_action parse_deny_action(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ಯದಿ (ಮೌಲ್ಯ) {
		ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "ignore"))
			ಪ್ರತ್ಯಾಗಮ DENY_IGNORE;
		ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "warn"))
			ಪ್ರತ್ಯಾಗಮ DENY_WARN;
		ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "refuse"))
			ಪ್ರತ್ಯಾಗಮ DENY_REFUSE;
		ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "updateinstead"))
			ಪ್ರತ್ಯಾಗಮ DENY_UPDATE_INSTEAD;
	}
	ಯದಿ (git_config_bool(var, ಮೌಲ್ಯ))
		ಪ್ರತ್ಯಾಗಮ DENY_REFUSE;
	ಪ್ರತ್ಯಾಗಮ DENY_IGNORE;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ receive_pack_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			       ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ_id;
	ಪೂರ್ಣಾಙ್ಕ status = parse_hide_refs_config(var, ಮೌಲ್ಯ, "receive", &hidden_refs);

	ಯದಿ (status)
		ಪ್ರತ್ಯಾಗಮ status;

	ಯದಿ (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.fsck.skiplist") == 0) {
		ವರ್ಣ *path;

		ಯದಿ (git_config_pathname(&path, var, ಮೌಲ್ಯ))
			ಪ್ರತ್ಯಾಗಮ -1;
		ಯದಿ (path)
			strbuf_addf(&fsck_ಸಂದೇಶ_types, "%cskiplist=%s",
				    fsck_ಸಂದೇಶ_types.len ? ',' : '=', path);
		ಮುಕ್ತ(path);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (skip_prefix(var, "receive.fsck.", &ಸಂದೇಶ_id)) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		ಯದಿ (is_valid_ಸಂದೇಶ_type(ಸಂದೇಶ_id, ಮೌಲ್ಯ))
			strbuf_addf(&fsck_ಸಂದೇಶ_types, "%c%s=%s",
				fsck_ಸಂದೇಶ_types.len ? ',' : '=', ಸಂದೇಶ_id, ಮೌಲ್ಯ);
		ಅನ್ಯಥಾ
			warning("skipping unknown msg id '%s'", ಸಂದೇಶ_id);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.certnonceseed") == 0)
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cert_nonce_seed, var, ಮೌಲ್ಯ);

	ಯದಿ (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, ಮೌಲ್ಯ, ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_ತಳ್ಳು = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_ತಳ್ಳು_options = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, ಮೌಲ್ಯ, ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.maxinputsize") == 0) {
		max_ಒಳಹರಿವು_size = git_config_int64(var, ಮೌಲ್ಯ, ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "receive.procreceiverefs") == 0) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		proc_receive_ref_append(ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ref(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಯದಿ (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		ಯದಿ (advertise_atomic_ತಳ್ಳು)
			strbuf_addstr(&cap, " atomic");
		ಯದಿ (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		ಯದಿ (ತಳ್ಳು_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", ತಳ್ಳು_cert_nonce);
		ಯದಿ (advertise_ತಳ್ಳು_options)
			strbuf_addstr(&cap, " push-options");
		ಯದಿ (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_ref_cb(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ oidset *seen = data;
	ಧ್ರುವ ವರ್ಣ *path = strip_namespace(ref->name);

	ಯದಿ (ref_is_hidden(path, ref->name, &hidden_refs))
		ಪ್ರತ್ಯಾಗಮ 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	ಯದಿ (!path) {
		ಯದಿ (oidset_ಸೇರಿಸು(seen, ref->oid))
			ಪ್ರತ್ಯಾಗಮ 0;
		path = ".have";
	} ಅನ್ಯಥಾ {
		oidset_ಸೇರಿಸು(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_one_alternate_ref(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				   ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ oidset *seen = data;

	ಯದಿ (oidset_ಸೇರಿಸು(seen, oid))
		ಪ್ರತ್ಯಾಗಮ;

	show_ref(".have", oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_head_info(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ refs_for_each_ref_options opts = { 0 };
	ಸ್ಥಿರ ಸಂರಚನಾ oidset seen = OIDSET_INIT;
	ಸಂರಚನಾ strvec excludes_vector = STRVEC_INIT;

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

	ಯದಿ (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#define RUN_PROC_RECEIVE_SCHEDULED	1
#define RUN_PROC_RECEIVE_RETURNED	2
ಸಂರಚನಾ command {
	ಸಂರಚನಾ command *next;
	ಧ್ರುವ ವರ್ಣ *error_string;
	ವರ್ಣ *error_string_owned;
	ಸಂರಚನಾ ref_ತಳ್ಳು_report *report;
	unsigned ಪೂರ್ಣಾಙ್ಕ skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	ಪೂರ್ಣಾಙ್ಕ index;
	ಸಂರಚನಾ object_id old_oid;
	ಸಂರಚನಾ object_id new_oid;
	ವರ್ಣ ref_name[FLEX_ARRAY]; /* more */
};

ಸ್ಥಿರ ಶೂನ್ಯ proc_receive_ref_append(ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಸಂರಚನಾ proc_receive_ref *ref_pattern;
	ಧ್ರುವ ವರ್ಣ *p;
	ಪೂರ್ಣಾಙ್ಕ len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = strchr(prefix, ':');
	ಯದಿ (p) {
		ಯಾವತ್ (prefix < p) {
			ಯದಿ (*prefix == 'a')
				ref_pattern->want_add = 1;
			ಅನ್ಯಥಾ ಯದಿ (*prefix == 'd')
				ref_pattern->want_delete = 1;
			ಅನ್ಯಥಾ ಯದಿ (*prefix == 'm')
				ref_pattern->want_modify = 1;
			ಅನ್ಯಥಾ ಯದಿ (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} ಅನ್ಯಥಾ {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	ಯಾವತ್ (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	ಯದಿ (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ proc_receive_ref *end;

		end = proc_receive_ref;
		ಯಾವತ್ (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ proc_receive_ref_matches(ಸಂರಚನಾ command *cmd)
{
	ಸಂರಚನಾ proc_receive_ref *p;

	ಯದಿ (!proc_receive_ref)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತಿ (p = proc_receive_ref; p; p = p->next) {
		ಧ್ರುವ ವರ್ಣ *match = p->ref_prefix;
		ಧ್ರುವ ವರ್ಣ *remains;

		ಯದಿ (!p->want_add && is_null_oid(&cmd->old_oid))
			ಅನುವರ್ತ;
		ಅನ್ಯಥಾ ಯದಿ (!p->want_delete && is_null_oid(&cmd->new_oid))
			ಅನುವರ್ತ;
		ಅನ್ಯಥಾ ಯದಿ (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			ಅನುವರ್ತ;

		ಯದಿ (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			ಯದಿ (!p->negative_ref)
				ಪ್ರತ್ಯಾಗಮ 1;
		} ಅನ್ಯಥಾ ಯದಿ (p->negative_ref) {
			ಪ್ರತ್ಯಾಗಮ 1;
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ report_message(ಧ್ರುವ ವರ್ಣ *prefix, ಧ್ರುವ ವರ್ಣ *err, va_list params)
{
	ಪೂರ್ಣಾಙ್ಕ sz;
	ವರ್ಣ ಸಂದೇಶ[4096];

	sz = xsnprintf(ಸಂದೇಶ, ಪರಿಮಾಣ(ಸಂದೇಶ), "%s", prefix);
	sz += vsnprintf(ಸಂದೇಶ + sz, ಪರಿಮಾಣ(ಸಂದೇಶ) - sz, err, params);
	ಯದಿ (sz > (ಪರಿಮಾಣ(ಸಂದೇಶ) - 1))
		sz = ಪರಿಮಾಣ(ಸಂದೇಶ) - 1;
	ಸಂದೇಶ[sz++] = '\n';

	ಯದಿ (use_sideband)
		send_sideband(1, 2, ಸಂದೇಶ, sz, use_sideband);
	ಅನ್ಯಥಾ
		xwrite(2, ಸಂದೇಶ, sz);
}

__attribute__((format (printf, 1, 2)))
ಸ್ಥಿರ ಶೂನ್ಯ rp_warning(ಧ್ರುವ ವರ್ಣ *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("warning: ", err, params);
	va_end(params);
}

__attribute__((format (printf, 1, 2)))
ಸ್ಥಿರ ಶೂನ್ಯ rp_error(ಧ್ರುವ ವರ್ಣ *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("error: ", err, params);
	va_end(params);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ copy_to_sideband(ಪೂರ್ಣಾಙ್ಕ in, ಪೂರ್ಣಾಙ್ಕ out UNUSED, ಶೂನ್ಯ *arg UNUSED)
{
	ವರ್ಣ data[128];
	ಪೂರ್ಣಾಙ್ಕ keepalive_active = 0;

	ಯದಿ (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	ಯದಿ (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	ಯಾವತ್ (1) {
		ssize_t sz;

		ಯದಿ (keepalive_active) {
			ಸಂರಚನಾ pollfd pfd;
			ಪೂರ್ಣಾಙ್ಕ ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			ಯದಿ (ret < 0) {
				ಯದಿ (errno == EINTR)
					ಅನುವರ್ತ;
				ಅನ್ಯಥಾ
					ಭಙ್ಗ;
			} ಅನ್ಯಥಾ ಯದಿ (ret == 0) {
				/* no data; send a keepalive packet */
				ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ buf[] = "0005\1";
				write_or_die(1, buf, ಪರಿಮಾಣ(buf) - 1);
				ಅನುವರ್ತ;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, ಪರಿಮಾಣ(data));
		ಯದಿ (sz <= 0)
			ಭಙ್ಗ;

		ಯದಿ (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			ಧ್ರುವ ವರ್ಣ *p = memchr(data, '\0', sz);
			ಯದಿ (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_active = 1;
				send_sideband(1, 2, data, p - data, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - data + 1), use_sideband);
				ಅನುವರ್ತ;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, data, sz, use_sideband);
	}
	close(in);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
ಸ್ಥಿರ ಶೂನ್ಯ prepare_sideband_async(ಸಂರಚನಾ async *sideband_async, ಪೂರ್ಣಾಙ್ಕ *saved_stderr, ಪೂರ್ಣಾಙ್ಕ *started)
{
	*started = 0;

	ಯದಿ (!use_sideband)
		ಪ್ರತ್ಯಾಗಮ;

	memset(sideband_async, 0, ಪರಿಮಾಣ(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	ಯದಿ (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		ಯದಿ (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
ಸ್ಥಿರ ಶೂನ್ಯ finish_sideband_async(ಸಂರಚನಾ async *sideband_async, ಪೂರ್ಣಾಙ್ಕ saved_stderr, ಪೂರ್ಣಾಙ್ಕ started)
{
	ಯದಿ (!use_sideband)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	ಯದಿ (started)
		finish_async(sideband_async);
}

ಸ್ಥಿರ ಶೂನ್ಯ hmac_hash(unsigned ವರ್ಣ *out,
		      ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ_in, size_t ಕೀಲಿ_len,
		      ಧ್ರುವ ವರ್ಣ *text, size_t text_len)
{
	unsigned ವರ್ಣ ಕೀಲಿ[GIT_MAX_BLKSZ];
	unsigned ವರ್ಣ k_ipad[GIT_MAX_BLKSZ];
	unsigned ವರ್ಣ k_opad[GIT_MAX_BLKSZ];
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(ಕೀಲಿ, '\0', GIT_MAX_BLKSZ);
	ಯದಿ (the_hash_algo->blksz < ಕೀಲಿ_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, ಕೀಲಿ_in, ಕೀಲಿ_len);
		git_hash_final(ಕೀಲಿ, &ctx);
	} ಅನ್ಯಥಾ {
		memcpy(ಕೀಲಿ, ಕೀಲಿ_in, ಕೀಲಿ_len);
	}

	/* RFC 2104 2. (2) & (5) */
	ಪ್ರತಿ (i = 0; i < ಪರಿಮಾಣ(ಕೀಲಿ); i++) {
		k_ipad[i] = ಕೀಲಿ[i] ^ 0x36;
		k_opad[i] = ಕೀಲಿ[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, ಪರಿಮಾಣ(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, ಪರಿಮಾಣ(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

ಸ್ಥಿರ ವರ್ಣ *prepare_ತಳ್ಳು_cert_nonce(ಧ್ರುವ ವರ್ಣ *path, timestamp_t stamp)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	unsigned ವರ್ಣ hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, strlen(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (ಪೂರ್ಣಾಙ್ಕ)the_hash_algo->hexsz, hash_to_hex(hash));
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&buf, ರಿಕ್ತ);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ constant_memequal(ಧ್ರುವ ವರ್ಣ *a, ಧ್ರುವ ವರ್ಣ *b, size_t n)
{
	ಪೂರ್ಣಾಙ್ಕ res = 0;
	size_t i;

	ಪ್ರತಿ (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *check_nonce(ಧ್ರುವ ವರ್ಣ *buf)
{
	size_t noncelen;
	ಧ್ರುವ ವರ್ಣ *found = find_commit_header(buf, "nonce", &noncelen);
	ವರ್ಣ *nonce = found ? xmemdupz(found, noncelen) : ರಿಕ್ತ;
	timestamp_t stamp, ostamp;
	ವರ್ಣ *bohmac, *expect = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *retval = NONCE_BAD;

	ಯದಿ (!nonce) {
		retval = NONCE_MISSING;
		ಗಚ್ಛ leave;
	} ಅನ್ಯಥಾ ಯದಿ (!ತಳ್ಳು_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		ಗಚ್ಛ leave;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(ತಳ್ಳು_cert_nonce, nonce)) {
		retval = NONCE_OK;
		ಗಚ್ಛ leave;
	}

	ಯದಿ (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		ಗಚ್ಛ leave;
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
	ಯದಿ (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		ಗಚ್ಛ leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	ಯದಿ (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		ಗಚ್ಛ leave;
	}

	expect = prepare_ತಳ್ಳು_cert_nonce(service_dir, stamp);
	ಯದಿ (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		ಗಚ್ಛ leave;
	}
	ಯದಿ (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		ಗಚ್ಛ leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(ತಳ್ಳು_cert_nonce, ರಿಕ್ತ, 10);
	nonce_stamp_slop = (long)ostamp - (long)stamp;

	ಯದಿ (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		ಮುಕ್ತ((ಶೂನ್ಯ *)ತಳ್ಳು_cert_nonce);
		ತಳ್ಳು_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} ಅನ್ಯಥಾ {
		retval = NONCE_SLOP;
	}

leave:
	ಮುಕ್ತ(nonce);
	ಮುಕ್ತ(expect);
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_cert_ತಳ್ಳು_options(ಧ್ರುವ ಸಂರಚನಾ string_list *ತಳ್ಳು_options)
{
	ಧ್ರುವ ವರ್ಣ *buf = ತಳ್ಳು_cert.buf;

	ಧ್ರುವ ವರ್ಣ *option;
	size_t optionlen;
	ಪೂರ್ಣಾಙ್ಕ options_seen = 0;

	ಪೂರ್ಣಾಙ್ಕ retval = 1;

	ಯದಿ (!*buf)
		ಪ್ರತ್ಯಾಗಮ 1;

	ಯಾವತ್ ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		ಯದಿ (options_seen > ತಳ್ಳು_options->nr
		    || xstrncmpz(ತಳ್ಳು_options->ವಸ್ತುಗಳು[options_seen - 1].string,
				 option, optionlen))
			ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (options_seen != ತಳ್ಳು_options->nr)
		retval = 0;

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_ತಳ್ಳು_cert_sha1(ಸಂರಚನಾ run_hooks_opt *opt)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ already_done;

	ಯದಿ (!ತಳ್ಳು_cert.len)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (!already_done) {
		ಪೂರ್ಣಾಙ್ಕ bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		ಯದಿ (odb_write_object(the_repository->objects, ತಳ್ಳು_cert.buf,
				     ತಳ್ಳು_cert.len, OBJ_BLOB, &ತಳ್ಳು_cert_oid))
			oidclr(&ತಳ್ಳು_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', ಪರಿಮಾಣ(sigcheck));

		bogs = parse_signed_buffer(ತಳ್ಳು_cert.buf, ತಳ್ಳು_cert.len);
		sigcheck.payload = xmemdupz(ತಳ್ಳು_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, ತಳ್ಳು_cert.buf + bogs,
				ತಳ್ಳು_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	ಯದಿ (!is_null_oid(&ತಳ್ಳು_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&ತಳ್ಳು_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.ಕೀಲಿ ? sigcheck.ಕೀಲಿ : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		ಯದಿ (ತಳ್ಳು_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     ತಳ್ಳು_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			ಯದಿ (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

ಸಂರಚನಾ receive_hook_feed_state {
	ಸಂರಚನಾ command *cmd;
	ಸಂರಚನಾ ref_ತಳ್ಳು_report *report;
	ಪೂರ್ಣಾಙ್ಕ skip_broken;
	ಸಂರಚನಾ strbuf buf;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ feed_receive_hook_cb(ಪೂರ್ಣಾಙ್ಕ hook_stdin_fd, ಶೂನ್ಯ *pp_cb UNUSED, ಶೂನ್ಯ *pp_task_cb)
{
	ಸಂರಚನಾ receive_hook_feed_state *state = pp_task_cb;
	ಸಂರಚನಾ command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	ಯಾವತ್ (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	ಯದಿ (!cmd)
		ಪ್ರತ್ಯಾಗಮ 1;  /* no more commands left */

	ಯದಿ (!state->report)
		state->report = cmd->report;

	ಯದಿ (state->report) {
		ಸಂರಚನಾ object_id *old_oid;
		ಸಂರಚನಾ object_id *new_oid;
		ಧ್ರುವ ವರ್ಣ *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->next;
		ಯದಿ (!state->report)
			cmd = cmd->next;
	} ಅನ್ಯಥಾ {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	ಯದಿ (state->buf.len > 0) {
		ಪೂರ್ಣಾಙ್ಕ ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		ಯದಿ (ret < 0) {
			ಯದಿ (errno == EPIPE)
				ಪ್ರತ್ಯಾಗಮ 1; /* child closed pipe */
			ಪ್ರತ್ಯಾಗಮ ret;
		}
	}

	ಪ್ರತ್ಯಾಗಮ state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

ಸ್ಥಿರ ಶೂನ್ಯ *receive_hook_feed_state_alloc(ಶೂನ್ಯ *feed_pipe_ctx)
{
	ಸಂರಚನಾ receive_hook_feed_state *init_state = feed_pipe_ctx;
	ಸಂರಚನಾ receive_hook_feed_state *data;

	CALLOC_ARRAY(data, 1);
	data->report = init_state->report;
	data->cmd = init_state->cmd;
	data->skip_broken = init_state->skip_broken;
	strbuf_init(&data->buf, 0);

	ಪ್ರತ್ಯಾಗಮ data;
}

ಸ್ಥಿರ ಶೂನ್ಯ receive_hook_feed_state_free(ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ receive_hook_feed_state *d = data;
	ಯದಿ (!d)
		ಪ್ರತ್ಯಾಗಮ;
	strbuf_release(&d->buf);
	ಮುಕ್ತ(d);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_receive_hook(ಸಂರಚನಾ command *commands,
			    ಧ್ರುವ ವರ್ಣ *hook_name,
			    ಪೂರ್ಣಾಙ್ಕ skip_broken,
			    ಸಂರಚನಾ odb_transaction *transaction,
			    ಧ್ರುವ ಸಂರಚನಾ string_list *ತಳ್ಳು_options)
{
	ಸಂರಚನಾ run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ಸಂರಚನಾ command *iter = commands;
	ಸಂರಚನಾ receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	ಸಂರಚನಾ async sideband_async;
	ಪೂರ್ಣಾಙ್ಕ sideband_async_started = 0;
	ಪೂರ್ಣಾಙ್ಕ saved_stderr = -1;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (!hook_exists(the_repository, hook_name))
		ಪ್ರತ್ಯಾಗಮ 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	ಯಾವತ್ (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	ಯದಿ (!iter)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (ತಳ್ಳು_options) {
		ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ತಳ್ಳು_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     ತಳ್ಳು_options->ವಸ್ತುಗಳು[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)ತಳ್ಳು_options->nr);
	} ಅನ್ಯಥಾ {
		strvec_ತಳ್ಳು(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	ಯದಿ (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_ತಳ್ಳು_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_data_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_data_free = receive_hook_feed_state_free;

	ret = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_update_hook(ಸಂರಚನಾ command *cmd)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ hook_name[] = "update";
	ಸಂರಚನಾ run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ಸಂರಚನಾ async sideband_async;
	ಪೂರ್ಣಾಙ್ಕ sideband_async_started = 0;
	ಪೂರ್ಣಾಙ್ಕ saved_stderr = -1;
	ಪೂರ್ಣಾಙ್ಕ code;

	ಯದಿ (!hook_exists(the_repository, hook_name))
		ಪ್ರತ್ಯಾಗಮ 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     ರಿಕ್ತ);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ಪ್ರತ್ಯಾಗಮ code;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_receive_report_hook(ಸಂರಚನಾ strbuf *report)
{
	ಸಂರಚನಾ child_process proc = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ async sideband_async;
	ಪೂರ್ಣಾಙ್ಕ sideband_async_started = 0;
	ಪೂರ್ಣಾಙ್ಕ saved_stderr = -1;
	ಸಂರಚನಾ strbuf out = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *hook_path;
	ಪೂರ್ಣಾಙ್ಕ ret;

	hook_path = find_hook(the_repository, "receive-report");
	ಯದಿ (!hook_path)
		ಪ್ರತ್ಯಾಗಮ 0;

	strvec_ತಳ್ಳು(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_ತಳ್ಳು(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, ರಿಕ್ತ, 0);
	sigchain_ತೆಗೆ(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	ಯದಿ (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಸಂರಚನಾ command *find_command_by_refname(ಸಂರಚನಾ command *list,
					       ಧ್ರುವ ವರ್ಣ *refname)
{
	ಪ್ರತಿ (; list; list = list->next)
		ಯದಿ (!strcmp(list->ref_name, refname))
			ಪ್ರತ್ಯಾಗಮ list;
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ read_proc_receive_report(ಸಂರಚನಾ packet_reader *reader,
				    ಸಂರಚನಾ command *commands,
				    ಸಂರಚನಾ strbuf *errmsg)
{
	ಸಂರಚನಾ command *cmd;
	ಸಂರಚನಾ command *hint = ರಿಕ್ತ;
	ಸಂರಚನಾ ref_ತಳ್ಳು_report *report = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ new_report = 0;
	ಪೂರ್ಣಾಙ್ಕ code = 0;
	ಪೂರ್ಣಾಙ್ಕ once = 0;
	ಪೂರ್ಣಾಙ್ಕ response = 0;

	ಪ್ರತಿ (;;) {
		ಸಂರಚನಾ object_id old_oid, new_oid;
		ವರ್ಣ *head;
		ವರ್ಣ *refname;
		ವರ್ಣ *p;
		ಗಣನಾ packet_read_status status;

		status = packet_reader_read(reader);
		ಯದಿ (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			ಯದಿ (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				ಪ್ರತ್ಯಾಗಮ -1;
			}
			ಭಙ್ಗ;
		}
		response++;

		head = reader->ಸಾಲು;
		p = strchr(head, ' ');
		ಯದಿ (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			ಅನುವರ್ತ;
		}
		*p++ = '\0';
		ಯದಿ (!strcmp(head, "option")) {
			ವರ್ಣ *ಕೀಲಿ;
			ಧ್ರುವ ವರ್ಣ *val;

			ಯದಿ (!hint || !(report || new_report)) {
				ಯದಿ (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				ಅನುವರ್ತ;
			}
			ಯದಿ (new_report) {
				ಯದಿ (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} ಅನ್ಯಥಾ {
					report = hint->report;
					ಯಾವತ್ (report->next)
						report = report->next;
					report->next = xcalloc(1, ಪರಿಮಾಣ(ಸಂರಚನಾ ref_ತಳ್ಳು_report));
					report = report->next;
				}
				new_report = 0;
			}
			ಕೀಲಿ = p;
			p = strchr(ಕೀಲಿ, ' ');
			ಯದಿ (p)
				*p++ = '\0';
			val = p;
			ಯದಿ (!strcmp(ಕೀಲಿ, "refname"))
				report->ref_name = xstrdup_or_null(val);
			ಅನ್ಯಥಾ ಯದಿ (!strcmp(ಕೀಲಿ, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			ಅನ್ಯಥಾ ಯದಿ (!strcmp(ಕೀಲಿ, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			ಅನ್ಯಥಾ ಯದಿ (!strcmp(ಕೀಲಿ, "forced-update"))
				report->forced_update = 1;
			ಅನ್ಯಥಾ ಯದಿ (!strcmp(ಕೀಲಿ, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			ಅನುವರ್ತ;
		}

		report = ರಿಕ್ತ;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		ಯದಿ (p)
			*p++ = '\0';
		ಯದಿ (strcmp(head, "ok") && strcmp(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			ಅನುವರ್ತ;
		}

		/* first try searching at our hint, falling back to all refs */
		ಯದಿ (hint)
			hint = find_command_by_refname(hint, refname);
		ಯದಿ (!hint)
			hint = find_command_by_refname(commands, refname);
		ಯದಿ (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			ಅನುವರ್ತ;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		ಯದಿ (!strcmp(head, "ng")) {
			ಯದಿ (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			ಅನ್ಯಥಾ
				hint->error_string = "failed";
			code = -1;
			ಅನುವರ್ತ;
		}
		new_report = 1;
	}

	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next)
		ಯದಿ (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	ಪ್ರತ್ಯಾಗಮ code;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_proc_receive_hook(ಸಂರಚನಾ command *commands,
				 ಧ್ರುವ ಸಂರಚನಾ string_list *ತಳ್ಳು_options)
{
	ಸಂರಚನಾ child_process proc = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ async muxer;
	ಸಂರಚನಾ command *cmd;
	ಸಂರಚನಾ packet_reader reader;
	ಸಂರಚನಾ strbuf cap = STRBUF_INIT;
	ಸಂರಚನಾ strbuf errmsg = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ hook_use_ತಳ್ಳು_options = 0;
	ಪೂರ್ಣಾಙ್ಕ version = 0;
	ಪೂರ್ಣಾಙ್ಕ code;
	ಧ್ರುವ ವರ್ಣ *hook_path = find_hook(the_repository, "proc-receive");

	ಯದಿ (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	strvec_ತಳ್ಳು(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	ಯದಿ (use_sideband) {
		memset(&muxer, 0, ಪರಿಮಾಣ(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		ಯದಿ (code)
			ಪ್ರತ್ಯಾಗಮ code;
		proc.err = muxer.in;
	} ಅನ್ಯಥಾ {
		proc.err = 0;
	}

	code = start_command(&proc);
	ಯದಿ (code) {
		ಯದಿ (use_sideband)
			finish_async(&muxer);
		ಪ್ರತ್ಯಾಗಮ code;
	}

	sigchain_ತಳ್ಳು(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, ರಿಕ್ತ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	ಯದಿ (use_atomic)
		strbuf_addstr(&cap, " atomic");
	ಯದಿ (use_ತಳ್ಳು_options)
		strbuf_addstr(&cap, " push-options");
	ಯದಿ (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} ಅನ್ಯಥಾ {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	ಯದಿ (!code)
		code = packet_flush_gently(proc.in);

	ಯದಿ (!code)
		ಪ್ರತಿ (;;) {
			ಪೂರ್ಣಾಙ್ಕ linelen;
			ಗಣನಾ packet_read_status status;

			status = packet_reader_read(&reader);
			ಯದಿ (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				ಯದಿ (status == PACKET_READ_EOF)
					code = -1;
				ಭಙ್ಗ;
			}

			ಯದಿ (reader.pktlen > 8 && starts_with(reader.ಸಾಲು, "version=")) {
				version = atoi(reader.ಸಾಲು + 8);
				linelen = strlen(reader.ಸಾಲು);
				ಯದಿ (linelen < reader.pktlen) {
					ಧ್ರುವ ವರ್ಣ *feature_list = reader.ಸಾಲು + linelen + 1;
					ಯದಿ (parse_feature_request(feature_list, "push-options"))
						hook_use_ತಳ್ಳು_options = 1;
				}
			}
		}

	ಯದಿ (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		ಗಚ್ಛ cleanup;
	}

	ವಿಕಲ್ಪ (version) {
	ಪಕ್ಷ 0:
		/* fallthrough */
	ಪಕ್ಷ 1:
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		ಗಚ್ಛ cleanup;
	}

	/* Send commands */
	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
		ಯದಿ (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			ಅನುವರ್ತ;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		ಯದಿ (code)
			ಭಙ್ಗ;
	}
	ಯದಿ (!code)
		code = packet_flush_gently(proc.in);
	ಯದಿ (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		ಗಚ್ಛ cleanup;
	}

	/* Send push options */
	ಯದಿ (hook_use_ತಳ್ಳು_options) {
		ಸಂರಚನಾ string_list_item *item;

		for_each_string_list_item(item, ತಳ್ಳು_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			ಯದಿ (code)
				ಭಙ್ಗ;
		}
		ಯದಿ (!code)
			code = packet_flush_gently(proc.in);
		ಯದಿ (code) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			ಗಚ್ಛ cleanup;
		}
	}

	/* Read result from proc-receive */
	code = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.in);
	close(proc.out);
	ಯದಿ (use_sideband)
		finish_async(&muxer);
	ಯದಿ (finish_command(&proc))
		code = -1;
	ಯದಿ (errmsg.len >0) {
		ವರ್ಣ *p = errmsg.buf;

		p += errmsg.len - 1;
		ಯದಿ (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_ತೆಗೆ(SIGPIPE);

	ಪ್ರತ್ಯಾಗಮ code;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *refuse_unconfigured_deny_ಸಂದೇಶ =
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

ಸ್ಥಿರ ಶೂನ್ಯ refuse_unconfigured_deny(ಶೂನ್ಯ)
{
	rp_error("%s", _(refuse_unconfigured_deny_ಸಂದೇಶ));
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *refuse_unconfigured_deny_delete_current_ಸಂದೇಶ =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

ಸ್ಥಿರ ಶೂನ್ಯ refuse_unconfigured_deny_delete_current(ಶೂನ್ಯ)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_ಸಂದೇಶ));
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ object_id *command_singleton_iterator(ಶೂನ್ಯ *cb_data);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_shallow_ref(ಸಂರಚನಾ command *cmd, ಸಂರಚನಾ shallow_info *si)
{
	ಸಂರಚನಾ shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	ಸಂರಚನಾ oid_array extra = OID_ARRAY_INIT;
	ಸಂರಚನಾ check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	ಪೂರ್ಣಾಙ್ಕ i;

	trace_printf_ಕೀಲಿ(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	ಪ್ರತಿ (i = 0; i < si->shallow->nr; i++)
		ಯದಿ (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	ಯದಿ (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	ಪ್ರತಿ (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ತಳ್ಳು_to_deploy(unsigned ವರ್ಣ *sha1,
				  ಸಂರಚನಾ strvec *env,
				  ಧ್ರುವ ಸಂರಚನಾ worktree *worktree)
{
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", ರಿಕ್ತ);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	ಯದಿ (run_command(&child))
		ಪ್ರತ್ಯಾಗಮ "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", ರಿಕ್ತ);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	ಯದಿ (run_command(&child))
		ಪ್ರತ್ಯಾಗಮ "Working directory has unstaged changes";

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
		     empty_ವೃಕ್ಷ_oid_hex(the_repository->hash_algo), "--", ರಿಕ್ತ);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	ಯದಿ (run_command(&child))
		ಪ್ರತ್ಯಾಗಮ "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     ರಿಕ್ತ);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	ಯದಿ (run_command(&child))
		ಪ್ರತ್ಯಾಗಮ "Could not update working tree to new HEAD";

	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ತಳ್ಳು_to_checkout_hook = "push-to-checkout";

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ತಳ್ಳು_to_checkout(unsigned ವರ್ಣ *hash,
				    ಪೂರ್ಣಾಙ್ಕ *invoked_hook,
				    ಸಂರಚನಾ strvec *env,
				    ಧ್ರುವ ವರ್ಣ *work_ವೃಕ್ಷ)
{
	ಸಂರಚನಾ run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_ವೃಕ್ಷ));
	strvec_ತಳ್ಳು(&opt.args, hash_to_hex(hash));
	ಯದಿ (run_hooks_opt(the_repository, ತಳ್ಳು_to_checkout_hook, &opt))
		ಪ್ರತ್ಯಾಗಮ "push-to-checkout hook declined";
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *update_worktree(unsigned ವರ್ಣ *sha1, ಧ್ರುವ ಸಂರಚನಾ worktree *worktree)
{
	ಧ್ರುವ ವರ್ಣ *retval;
	ವರ್ಣ *git_dir;
	ಸಂರಚನಾ strvec env = STRVEC_INIT;
	ಪೂರ್ಣಾಙ್ಕ invoked_hook;

	ಯದಿ (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	ಯದಿ (worktree->is_bare)
		ಪ್ರತ್ಯಾಗಮ "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = ತಳ್ಳು_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	ಯದಿ (!invoked_hook)
		retval = ತಳ್ಳು_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	ಮುಕ್ತ(git_dir);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *update(ಸಂರಚನಾ command *cmd, ಸಂರಚನಾ shallow_info *si)
{
	ಧ್ರುವ ವರ್ಣ *name = cmd->ref_name;
	ಸಂರಚನಾ strbuf namespaced_name_buf = STRBUF_INIT;
	ಸ್ಥಿರ ವರ್ಣ *namespaced_name;
	ಧ್ರುವ ವರ್ಣ *ret;
	ಸಂರಚನಾ object_id *old_oid = &cmd->old_oid;
	ಸಂರಚನಾ object_id *new_oid = &cmd->new_oid;
	ಪೂರ್ಣಾಙ್ಕ do_update_worktree = 0;
	ಸಂರಚನಾ worktree **worktrees = get_worktrees(the_repository);
	ಧ್ರುವ ಸಂರಚನಾ worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	ಯದಿ (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		ಗಚ್ಛ out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	ಮುಕ್ತ(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, ರಿಕ್ತ);

	ಯದಿ (worktree && !worktree->is_bare) {
		ವಿಕಲ್ಪ (deny_current_branch) {
		ಪಕ್ಷ DENY_IGNORE:
			ಭಙ್ಗ;
		ಪಕ್ಷ DENY_WARN:
			rp_warning("updating the current branch");
			ಭಙ್ಗ;
		ಪಕ್ಷ DENY_REFUSE:
		ಪಕ್ಷ DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			ಯದಿ (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			ಗಚ್ಛ out;
		ಪಕ್ಷ DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			ಭಙ್ಗ;
		}
	}

	ಯದಿ (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		ಗಚ್ಛ out;
	}

	ಯದಿ (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		ಯದಿ (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			ಗಚ್ಛ out;
		}

		ಯದಿ (worktree || (head_name && !strcmp(namespaced_name, head_name))) {
			ವಿಕಲ್ಪ (deny_delete_current) {
			ಪಕ್ಷ DENY_IGNORE:
				ಭಙ್ಗ;
			ಪಕ್ಷ DENY_WARN:
				rp_warning("deleting the current branch");
				ಭಙ್ಗ;
			ಪಕ್ಷ DENY_REFUSE:
			ಪಕ್ಷ DENY_UNCONFIGURED:
			ಪಕ್ಷ DENY_UPDATE_INSTEAD:
				ಯದಿ (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				ಗಚ್ಛ out;
			ಸ್ವಭಾವ:
				ret = "Invalid denyDeleteCurrent setting";
				ಗಚ್ಛ out;
			}
		}
	}

	ಯದಿ (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		ಸಂರಚನಾ object *old_object, *new_object;
		ಸಂರಚನಾ commit *old_commit, *new_commit;
		ಪೂರ್ಣಾಙ್ಕ ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		ಯದಿ (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			ಗಚ್ಛ out;
		}
		old_commit = (ಸಂರಚನಾ commit *)old_object;
		new_commit = (ಸಂರಚನಾ commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		ಯದಿ (ret2 < 0)
			exit(128);
		ಯದಿ (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			ಗಚ್ಛ out;
		}
	}
	ಯದಿ (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		ಗಚ್ಛ out;
	}

	ಯದಿ (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		ಯದಿ (ret)
			ಗಚ್ಛ out;
	}

	ಯದಿ (is_null_oid(new_oid)) {
		ಸಂರಚನಾ strbuf err = STRBUF_INIT;
		ಯದಿ (!parse_object(the_repository, old_oid)) {
			old_oid = ರಿಕ್ತ;
			ಯದಿ (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} ಅನ್ಯಥಾ {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		ಯದಿ (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   ರಿಕ್ತ, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} ಅನ್ಯಥಾ {
			ret = ರಿಕ್ತ; /* good */
		}
		strbuf_release(&err);
	} ಅನ್ಯಥಾ {
		ಗಣನಾ ref_transaction_error tx_err;
		ಸಂರಚನಾ strbuf err = STRBUF_INIT;
		ಯದಿ (shallow_update && si->shallow_ref[cmd->index] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			ಗಚ್ಛ out;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  ರಿಕ್ತ, ರಿಕ್ತ,
						  0, "push",
						  &err);
		ಯದಿ (tx_err) {
			rp_error("%s", err.buf);
			ಯದಿ (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			ಅನ್ಯಥಾ
				ret = ref_transaction_error_ಸಂದೇಶ(tx_err);
		} ಅನ್ಯಥಾ {
			ret = ರಿಕ್ತ; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ run_update_post_hook(ಸಂರಚನಾ command *commands)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ hook_name[] = "post-update";
	ಸಂರಚನಾ run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ಸಂರಚನಾ async sideband_async;
	ಸಂರಚನಾ command *cmd;
	ಪೂರ್ಣಾಙ್ಕ sideband_async_started = 0;
	ಪೂರ್ಣಾಙ್ಕ saved_stderr = -1;

	ಯದಿ (!hook_exists(the_repository, hook_name))
		ಪ್ರತ್ಯಾಗಮ;

	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
		ಯದಿ (cmd->error_string || cmd->did_not_exist)
			ಅನುವರ್ತ;
		strvec_ತಳ್ಳು(&opt.args, cmd->ref_name);
	}
	ಯದಿ (!opt.args.nr)
		ಪ್ರತ್ಯಾಗಮ;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

ಸ್ಥಿರ ಶೂನ್ಯ check_aliased_update_internal(ಸಂರಚನಾ command *cmd,
					  ಸಂರಚನಾ string_list *list,
					  ಧ್ರುವ ವರ್ಣ *dst_name, ಪೂರ್ಣಾಙ್ಕ flag)
{
	ಸಂರಚನಾ string_list_item *item;
	ಸಂರಚನಾ command *dst_cmd;

	ಯದಿ (!(flag & REF_ISSYMREF))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		ಪ್ರತ್ಯಾಗಮ;
	}
	dst_name = strip_namespace(dst_name);

	ಯದಿ (!(item = string_list_lookup(list, dst_name)))
		ಪ್ರತ್ಯಾಗಮ;

	cmd->skip_update = 1;

	dst_cmd = (ಸಂರಚನಾ command *) item->util;

	ಯದಿ (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		ಪ್ರತ್ಯಾಗಮ;

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

ಸ್ಥಿರ ಶೂನ್ಯ check_aliased_update(ಸಂರಚನಾ command *cmd, ಸಂರಚನಾ string_list *list)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *dst_name;
	ಪೂರ್ಣಾಙ್ಕ flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, ರಿಕ್ತ, &flag);
	check_aliased_update_internal(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ check_aliased_updates(ಸಂರಚನಾ command *commands)
{
	ಸಂರಚನಾ command *cmd;
	ಸಂರಚನಾ string_list ref_list = STRING_LIST_INIT_NODUP;

	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
		ಸಂರಚನಾ string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (ಶೂನ್ಯ *)cmd;
	}
	string_list_sort(&ref_list);

	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
		ಯದಿ (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ object_id *command_singleton_iterator(ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ command **cmd_list = cb_data;
	ಸಂರಚನಾ command *cmd = *cmd_list;

	ಯದಿ (!cmd || is_null_oid(&cmd->new_oid))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	*cmd_list = ರಿಕ್ತ; /* this returns only one */
	ಪ್ರತ್ಯಾಗಮ &cmd->new_oid;
}

ಸ್ಥಿರ ಶೂನ್ಯ set_connectivity_errors(ಸಂರಚನಾ command *commands,
				    ಸಂರಚನಾ shallow_info *si,
				    ಸಂರಚನಾ odb_transaction *transaction)
{
	ಸಂರಚನಾ command *cmd;

	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
		ಸಂರಚನಾ command *singleton = cmd;
		ಸಂರಚನಾ check_connected_options opt = CHECK_CONNECTED_INIT;
		ಸಂರಚನಾ strvec env = STRVEC_INIT;

		ಯದಿ (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			ಅನುವರ್ತ;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		ಯದಿ (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			ಅನುವರ್ತ;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

ಸಂರಚನಾ iterate_data {
	ಸಂರಚನಾ command *cmds;
	ಸಂರಚನಾ shallow_info *si;
};

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ object_id *iterate_receive_command_list(ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ iterate_data *data = cb_data;
	ಸಂರಚನಾ command **cmd_list = &data->cmds;
	ಸಂರಚನಾ command *cmd = *cmd_list;

	ಪ್ರತಿ (; cmd; cmd = cmd->next) {
		ಯದಿ (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			ಅನುವರ್ತ;
		ಯದಿ (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			ಪ್ರತ್ಯಾಗಮ &cmd->new_oid;
		}
	}
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ reject_updates_to_hidden(ಸಂರಚನಾ command *commands)
{
	ಸಂರಚನಾ strbuf refname_full = STRBUF_INIT;
	size_t prefix_len;
	ಸಂರಚನಾ command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
		ಯದಿ (cmd->error_string)
			ಅನುವರ್ತ;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		ಯದಿ (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			ಅನುವರ್ತ;
		ಯದಿ (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		ಅನ್ಯಥಾ
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ should_process_cmd(ಸಂರಚನಾ command *cmd)
{
	ಪ್ರತ್ಯಾಗಮ !cmd->error_string && !cmd->skip_update;
}

ಸ್ಥಿರ ಶೂನ್ಯ BUG_if_skipped_connectivity_check(ಸಂರಚನಾ command *commands,
					       ಸಂರಚನಾ shallow_info *si)
{
	ಸಂರಚನಾ command *cmd;

	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
		ಯದಿ (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

ಸ್ಥಿರ ಶೂನ್ಯ ref_transaction_rejection_handler(ಧ್ರುವ ವರ್ಣ *refname,
					      ಧ್ರುವ ಸಂರಚನಾ object_id *old_oid UNUSED,
					      ಧ್ರುವ ಸಂರಚನಾ object_id *new_oid UNUSED,
					      ಧ್ರುವ ವರ್ಣ *old_target UNUSED,
					      ಧ್ರುವ ವರ್ಣ *new_target UNUSED,
					      ಗಣನಾ ref_transaction_error err,
					      ಧ್ರುವ ವರ್ಣ *details,
					      ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ strmap *failed_refs = cb_data;

	ಯದಿ (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (ವರ್ಣ *)ref_transaction_error_ಸಂದೇಶ(err));
}

ಸ್ಥಿರ ಶೂನ್ಯ execute_commands_non_atomic(ಸಂರಚನಾ command *commands,
					ಸಂರಚನಾ shallow_info *si)
{
	ಸಂರಚನಾ command *cmd;
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *reported_error = ರಿಕ್ತ;
	ಸಂರಚನಾ strmap failed_refs = STRMAP_INIT;

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
	ಗಣನಾ processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	ಪ್ರತಿ (ಗಣನಾ processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
			ಯದಿ (!should_process_cmd(cmd) || cmd->run_proc_receive)
				ಅನುವರ್ತ;

			ಯದಿ (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				ಅನುವರ್ತ;
			ಅನ್ಯಥಾ ಯದಿ (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				ಅನುವರ್ತ;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			ಯದಿ (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				ಯದಿ (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					ಗಚ್ಛ failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		ಯದಿ (!transaction)
			ಗಚ್ಛ cleanup;

		ಯದಿ (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			ಗಚ್ಛ failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		ಯದಿ (strmap_empty(&failed_refs))
			ಗಚ್ಛ cleanup;

	failure:
		ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
			ಯದಿ (reported_error)
				cmd->error_string = reported_error;
			ಅನ್ಯಥಾ ಯದಿ (strmap_contains(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = ರಿಕ್ತ;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ execute_commands_atomic(ಸಂರಚನಾ command *commands,
					ಸಂರಚನಾ shallow_info *si)
{
	ಸಂರಚನಾ command *cmd;
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ಯದಿ (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		ಗಚ್ಛ failure;
	}

	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
		ಯದಿ (!should_process_cmd(cmd) || cmd->run_proc_receive)
			ಅನುವರ್ತ;

		cmd->error_string = update(cmd, si);

		ಯದಿ (cmd->error_string)
			ಗಚ್ಛ failure;
	}

	ಯದಿ (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		ಗಚ್ಛ failure;
	}
	ಗಚ್ಛ cleanup;

failure:
	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next)
		ಯದಿ (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ execute_commands(ಸಂರಚನಾ command *commands,
			     ಪೂರ್ಣಾಙ್ಕ unpacker_error,
			     ಸಂರಚನಾ shallow_info *si,
			     ಸಂರಚನಾ odb_transaction *transaction,
			     ಧ್ರುವ ಸಂರಚನಾ string_list *ತಳ್ಳು_options)
{
	ಸಂರಚನಾ check_connected_options opt = CHECK_CONNECTED_INIT;
	ಸಂರಚನಾ command *cmd;
	ಸಂರಚನಾ iterate_data data;
	ಸಂರಚನಾ async muxer;
	ಪೂರ್ಣಾಙ್ಕ err_fd = 0;
	ಪೂರ್ಣಾಙ್ಕ run_proc_receive = 0;

	ಯದಿ (unpacker_error) {
		ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (!skip_connectivity_check) {
		ಸಂರಚನಾ strvec env = STRVEC_INIT;

		ಯದಿ (use_sideband) {
			memset(&muxer, 0, ಪರಿಮಾಣ(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			ಯದಿ (!start_async(&muxer))
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

		ಯದಿ (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		ಯದಿ (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	ಯದಿ (proc_receive_ref) {
		ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
			ಯದಿ (!should_process_cmd(cmd))
				ಅನುವರ್ತ;

			ಯದಿ (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	ಯದಿ (run_receive_hook(commands, "pre-receive", 0, transaction, ತಳ್ಳು_options)) {
		ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
			ಯದಿ (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		ಪ್ರತ್ಯಾಗಮ;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	ಪ್ರತಿ (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	ಯದಿ (!cmd)
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	ಯದಿ (odb_transaction_commit(transaction)) {
		ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
			ಯದಿ (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		ಪ್ರತ್ಯಾಗಮ;
	}

	check_aliased_updates(commands);

	ಮುಕ್ತ(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, ರಿಕ್ತ,
							    ರಿಕ್ತ);

	ಯದಿ (run_proc_receive &&
	    run_proc_receive_hook(commands, ತಳ್ಳು_options))
		ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next)
			ಯದಿ (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	ಯದಿ (use_atomic)
		execute_commands_atomic(commands, si);
	ಅನ್ಯಥಾ
		execute_commands_non_atomic(commands, si);

	ಯದಿ (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

ಸ್ಥಿರ ಸಂರಚನಾ command **queue_command(ಸಂರಚನಾ command **tail,
				      ಧ್ರುವ ವರ್ಣ *ಸಾಲು,
				      ಪೂರ್ಣಾಙ್ಕ linelen)
{
	ಸಂರಚನಾ object_id old_oid, new_oid;
	ಸಂರಚನಾ command *cmd;
	ಧ್ರುವ ವರ್ಣ *refname;
	ಪೂರ್ಣಾಙ್ಕ reflen;
	ಧ್ರುವ ವರ್ಣ *p;

	ಯದಿ (parse_oid_hex(ಸಾಲು, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", ಸಾಲು);

	refname = p;
	reflen = linelen - (p - ಸಾಲು);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*tail = cmd;
	ಪ್ರತ್ಯಾಗಮ &cmd->next;
}

ಸ್ಥಿರ ಶೂನ್ಯ free_commands(ಸಂರಚನಾ command *commands)
{
	ಯಾವತ್ (commands) {
		ಸಂರಚನಾ command *next = commands->next;

		ref_ತಳ್ಳು_report_free(commands->report);
		ಮುಕ್ತ(commands->error_string_owned);
		ಮುಕ್ತ(commands);
		commands = next;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ queue_commands_from_cert(ಸಂರಚನಾ command **tail,
				     ಸಂರಚನಾ strbuf *ತಳ್ಳು_cert)
{
	ಧ್ರುವ ವರ್ಣ *boc, *eoc;

	ಯದಿ (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(ತಳ್ಳು_cert->buf, "\n\n");
	ಯದಿ (!boc)
		die("malformed push certificate %.*s", 100, ತಳ್ಳು_cert->buf);
	ಅನ್ಯಥಾ
		boc += 2;
	eoc = ತಳ್ಳು_cert->buf + parse_signed_buffer(ತಳ್ಳು_cert->buf, ತಳ್ಳು_cert->len);

	ಯಾವತ್ (boc < eoc) {
		ಧ್ರುವ ವರ್ಣ *eol = memchr(boc, '\n', eoc - boc);
		tail = queue_command(tail, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

ಸ್ಥಿರ ಸಂರಚನಾ command *read_head_info(ಸಂರಚನಾ packet_reader *reader,
				      ಸಂರಚನಾ oid_array *shallow,
				      ಗಣನಾ report_status_version *version)
{
	ಸಂರಚನಾ command *commands = ರಿಕ್ತ;
	ಸಂರಚನಾ command **p = &commands;
	ಪ್ರತಿ (;;) {
		ಪೂರ್ಣಾಙ್ಕ linelen;

		ಯದಿ (packet_reader_read(reader) != PACKET_READ_NORMAL)
			ಭಙ್ಗ;

		ಯದಿ (reader->pktlen > 8 && starts_with(reader->ಸಾಲು, "shallow ")) {
			ಸಂರಚನಾ object_id oid;
			ಯದಿ (get_oid_hex(reader->ಸಾಲು + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->ಸಾಲು + 8);
			oid_array_append(shallow, &oid);
			ಅನುವರ್ತ;
		}

		linelen = strlen(reader->ಸಾಲು);
		ಯದಿ (linelen < reader->pktlen) {
			ಧ್ರುವ ವರ್ಣ *feature_list = reader->ಸಾಲು + linelen + 1;
			ಧ್ರುವ ವರ್ಣ *hash = ರಿಕ್ತ;
			ಧ್ರುವ ವರ್ಣ *client_sid;
			size_t len = 0;
			ಯದಿ (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			ಯದಿ (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			ಯದಿ (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			ಯದಿ (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			ಯದಿ (advertise_atomic_ತಳ್ಳು
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			ಯದಿ (advertise_ತಳ್ಳು_options
			    && parse_feature_request(feature_list, "push-options"))
				use_ತಳ್ಳು_options = 1;
			hash = parse_feature_ಮೌಲ್ಯ(feature_list, "object-format", &len, ರಿಕ್ತ);
			ಯದಿ (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			ಯದಿ (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_ಮೌಲ್ಯ(feature_list, "session-id", &len, ರಿಕ್ತ);
			ಯದಿ (client_sid) {
				ವರ್ಣ *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", ರಿಕ್ತ, "client-sid", client_sid);
				ಮುಕ್ತ(sid);
			}
		}

		ಯದಿ (!strcmp(reader->ಸಾಲು, "push-cert")) {
			ಪೂರ್ಣಾಙ್ಕ true_flush = 0;
			ಪೂರ್ಣಾಙ್ಕ saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			ಪ್ರತಿ (;;) {
				packet_reader_read(reader);
				ಯದಿ (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					ಭಙ್ಗ;
				}
				ಯದಿ (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				ಯದಿ (!strcmp(reader->ಸಾಲು, "push-cert-end\n"))
					ಭಙ್ಗ; /* end of cert */
				strbuf_addstr(&ತಳ್ಳು_cert, reader->ಸಾಲು);
			}
			reader->options = saved_options;

			ಯದಿ (true_flush)
				ಭಙ್ಗ;
			ಅನುವರ್ತ;
		}

		p = queue_command(p, reader->ಸಾಲು, linelen);
	}

	ಯದಿ (ತಳ್ಳು_cert.len)
		queue_commands_from_cert(p, &ತಳ್ಳು_cert);

	ಪ್ರತ್ಯಾಗಮ commands;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_ತಳ್ಳು_options(ಸಂರಚನಾ packet_reader *reader,
			      ಸಂರಚನಾ string_list *options)
{
	ಯಾವತ್ (1) {
		ಯದಿ (packet_reader_read(reader) != PACKET_READ_NORMAL)
			ಭಙ್ಗ;

		string_list_append(options, reader->ಸಾಲು);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ unpack_with_sideband(ಸಂರಚನಾ odb_transaction *transaction,
				ಧ್ರುವ ವರ್ಣ *shallow_file,
				ಸಂರಚನಾ strbuf *err_ಸಂದೇಶ)
{
	ಸಂರಚನಾ odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_ಸಂದೇಶ_types = fsck_ಸಂದೇಶ_types.buf,
		.max_ಒಳಹರಿವು_size = max_ಒಳಹರಿವು_size,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	ಸಂರಚನಾ async muxer;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (!use_sideband)
		ಪ್ರತ್ಯಾಗಮ odb_transaction_write_pack(transaction, 0, err_ಸಂದೇಶ, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, ಪರಿಮಾಣ(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	ಯದಿ (start_async(&muxer))
		ಪ್ರತ್ಯಾಗಮ 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_ಸಂದೇಶ, &opts);

	finish_async(&muxer);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_shallow_update(ಸಂರಚನಾ shallow_info *si,
				   ಧ್ರುವ ವರ್ಣ *shallow_file)
{
	ಪೂರ್ಣಾಙ್ಕ i, j, k, bitmap_size = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, ರಿಕ್ತ);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	ಪ್ರತಿ (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	ಪ್ರತಿ (i = 0; i < si->shallow->nr; i++) {
		ಯದಿ (!si->used_shallow[i])
			ಅನುವರ್ತ;
		ಪ್ರತಿ (j = 0; j < bitmap_size; j++) {
			ಯದಿ (!si->used_shallow[i][j])
				ಅನುವರ್ತ;
			si->need_reachability_test[i]++;
			ಪ್ರತಿ (k = 0; k < 32; k++)
				ಯದಿ (si->used_shallow[i][j] & (1U << k))
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

ಸ್ಥಿರ ಶೂನ್ಯ update_shallow_info(ಸಂರಚನಾ command *commands,
				ಸಂರಚನಾ shallow_info *si,
				ಸಂರಚನಾ oid_array *ref,
				ಧ್ರುವ ವರ್ಣ *shallow_file)
{
	ಸಂರಚನಾ command *cmd;
	ಪೂರ್ಣಾಙ್ಕ *ref_status;
	remove_nonexistent_theirs_shallow(si);
	ಯದಿ (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
		ಯದಿ (is_null_oid(&cmd->new_oid))
			ಅನುವರ್ತ;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	ಯದಿ (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, ರಿಕ್ತ, ref_status);
	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
		ಯದಿ (is_null_oid(&cmd->new_oid))
			ಅನುವರ್ತ;
		ಯದಿ (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	ಮುಕ್ತ(ref_status);
}

ಸ್ಥಿರ ಶೂನ್ಯ override_cmds_error(ಸಂರಚನಾ command *commands, ಧ್ರುವ ವರ್ಣ *err)
{
	ಪ್ರತಿ (ಸಂರಚನಾ command *cmd = commands; cmd; cmd = cmd->next) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
ಸ್ಥಿರ ಶೂನ್ಯ generate_report(ಸಂರಚನಾ strbuf *buf, ಸಂರಚನಾ command *commands,
			    ಧ್ರುವ ಸಂರಚನಾ strbuf *unpack_status,
			    ಗಣನಾ report_status_version version)
{
	ಸಂರಚನಾ command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
		ಸಂರಚನಾ ref_ತಳ್ಳು_report *report;
		ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;

		ಯದಿ (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		ಅನ್ಯಥಾ
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		ಯದಿ (version != REPORT_STATUS_V2 || cmd->error_string)
			ಅನುವರ್ತ;

		ಪ್ರತಿ (report = cmd->report; report; report = report->next) {
			ಯದಿ (ಎಣಿಕೆ++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			ಯದಿ (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			ಯದಿ (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			ಯದಿ (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			ಯದಿ (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ report(ಸಂರಚನಾ command *commands, ಧ್ರುವ ಸಂರಚನಾ strbuf *unpack_status,
		   ಗಣನಾ report_status_version version)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	ಯದಿ (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	ಯದಿ (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	ಅನ್ಯಥಾ
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ delete_only(ಸಂರಚನಾ command *commands)
{
	ಸಂರಚನಾ command *cmd;
	ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next) {
		ಯದಿ (!is_null_oid(&cmd->new_oid))
			ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಪೂರ್ಣಾಙ್ಕ cmd_receive_pack(ಪೂರ್ಣಾಙ್ಕ argc,
		     ಧ್ರುವ ವರ್ಣ **argv,
		     ಧ್ರುವ ವರ್ಣ *prefix,
		     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ advertise_refs = 0;
	ಸಂರಚನಾ command *commands;
	ಸಂರಚನಾ oid_array shallow = OID_ARRAY_INIT;
	ಸಂರಚನಾ oid_array ref = OID_ARRAY_INIT;
	ಸಂರಚನಾ shallow_info si;
	ಸಂರಚನಾ packet_reader reader;
	ಸಂರಚನಾ odb_transaction *transaction = ರಿಕ್ತ;
	ಗಣನಾ report_status_version version = REPORT_STATUS_UNKNOWN;

	ಸಂರಚನಾ option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, ರಿಕ್ತ),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, ರಿಕ್ತ),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, ರಿಕ್ತ),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, ರಿಕ್ತ),
		OPT_END()
	};

	packet_trace_identity("receive-pack");

	argc = parse_options(argc, argv, prefix, options, receive_pack_usage, 0);

	ಯದಿ (argc > 1)
		usage_ಸಂದೇಶ_opt(_("too many arguments"), receive_pack_usage, options);
	ಯದಿ (argc == 0)
		usage_ಸಂದೇಶ_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	ಯದಿ (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, ರಿಕ್ತ);
	ಯದಿ (cert_nonce_seed)
		ತಳ್ಳು_cert_nonce = prepare_ತಳ್ಳು_cert_nonce(service_dir, time(ರಿಕ್ತ));

	ವಿಕಲ್ಪ (determine_protocol_version_server()) {
	ಪಕ್ಷ protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		ಭಙ್ಗ;
	ಪಕ್ಷ protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ಯದಿ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	ಪಕ್ಷ protocol_v0:
		ಭಙ್ಗ;
	ಪಕ್ಷ protocol_unknown_version:
		BUG("unknown protocol version");
	}

	ಯದಿ (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	ಯದಿ (advertise_refs)
		ಪ್ರತ್ಯಾಗಮ 0;

	packet_reader_init(&reader, 0, ರಿಕ್ತ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ಯದಿ ((commands = read_head_info(&reader, &shallow, &version))) {
		ಸಂರಚನಾ string_list ತಳ್ಳು_options = STRING_LIST_INIT_DUP;
		ಸಂರಚನಾ strbuf unpack_status = STRBUF_INIT;

		ಯದಿ (use_ತಳ್ಳು_options)
			read_ತಳ್ಳು_options(&reader, &ತಳ್ಳು_options);
		ಯದಿ (!check_cert_ತಳ್ಳು_options(&ತಳ್ಳು_options)) {
			ಸಂರಚನಾ command *cmd;
			ಪ್ರತಿ (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		ಯದಿ (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		ಯದಿ (!delete_only(commands)) {
			ಧ್ರುವ ವರ್ಣ *alt_shallow_file = ರಿಕ್ತ;

			ಯದಿ (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			ಯದಿ (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			ಅನ್ಯಥಾ
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &ತಳ್ಳು_options);
		odb_transaction_finalize(transaction);
		sigchain_ತಳ್ಳು(SIGPIPE, SIG_IGN);

		ವಿಕಲ್ಪ (version) {
		ಪಕ್ಷ REPORT_STATUS_V2:
		ಪಕ್ಷ REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			ಭಙ್ಗ;
		ಪಕ್ಷ REPORT_STATUS_UNKNOWN:
			ಭಙ್ಗ;
		}

		sigchain_ತೆಗೆ(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, ರಿಕ್ತ,
				 &ತಳ್ಳು_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&ತಳ್ಳು_options, 0);
		ಯದಿ (auto_gc) {
			ಸಂರಚನಾ child_process proc = CHILD_PROCESS_INIT;

			ಯದಿ (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				ಯದಿ (!start_command(&proc)) {
					ಯದಿ (use_sideband)
						copy_to_sideband(proc.err, -1, ರಿಕ್ತ);
					finish_command(&proc);
				}
			}
		}
		ಯದಿ (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	ಯದಿ (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	ಮುಕ್ತ((ಶೂನ್ಯ *)ತಳ್ಳು_cert_nonce);
	ಪ್ರತ್ಯಾಗಮ 0;
}
