#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	ಖಾಲಿ
};

ಎಣಿಕೆ deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

ಎಣಿಕೆ report_ಸ್ಥಿತಿ_ಆವೃತ್ತಿ {
	REPORT_ಸ್ಥಿತಿ_UNKNOWN = 0,
	REPORT_ಸ್ಥಿತಿ_V0,
	REPORT_ಸ್ಥಿತಿ_V2,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ deny_deletes;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ deny_non_fast_forwards;
ಸ್ಥಿರ ಎಣಿಕೆ deny_action deny_current_branch = DENY_UNCONFIGURED;
ಸ್ಥಿರ ಎಣಿಕೆ deny_action deny_delete_current = DENY_UNCONFIGURED;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ receive_fsck_objects = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ transfer_fsck_objects = -1;
ಸ್ಥಿರ ರಚನೆ strbuf fsck_ಸಂದೇಶ_types = STRBUF_INIT;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ advertise_atomic_ತಳ್ಳು = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ advertise_ತಳ್ಳು_options;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ advertise_sid;
ಸ್ಥಿರ off_t max_ಒಳಹರಿವು_ಗಾತ್ರ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_sideband;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_atomic;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_ತಳ್ಳು_options;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ quiet;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prefer_ofs_delta = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ auto_update_server_info;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ auto_gc = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reject_thin;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ skip_connectivity_check;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stateless_rpc;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *service_dir;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_name;
ಸ್ಥಿರ ಶೂನ್ಯ *ತಲೆ_name_ಗಮ್ಯ_free;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕಳುಹಿಸಿದ_capabilities;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ shallow_update;
ಸ್ಥಿರ ರಚನೆ strbuf ತಳ್ಳು_cert = STRBUF_INIT;
ಸ್ಥಿರ ರಚನೆ object_id ತಳ್ಳು_cert_oid;
ಸ್ಥಿರ ರಚನೆ signature_check sigcheck;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ್ಳು_cert_nonce;
ಸ್ಥಿರ ಅಕ್ಷರ *cert_nonce_ಬೀಜ;
ಸ್ಥಿರ ರಚನೆ strvec hidden_refs = STRVEC_INIT;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *NONCE_UNSOLICITED = "UNSOLICITED";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *NONCE_BAD = "BAD";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *NONCE_MISSING = "MISSING";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *NONCE_OK = "OK";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *NONCE_SLOP = "SLOP";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *nonce_ಸ್ಥಿತಿ;
ಸ್ಥಿರ ದೀರ್ಘ nonce_stamp_slop;
ಸ್ಥಿರ timestamp_t nonce_stamp_slop_ಎಲ್ಲೆ;
ಸ್ಥಿರ ರಚನೆ ref_transaction *transaction;

ಸ್ಥಿರ ಎಣಿಕೆ {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ keepalive_ಒಳ_sec = 5;

ಸ್ಥಿರ ರಚನೆ proc_receive_ref {
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ want_ಕೂಡಿಸು:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	ಅಕ್ಷರ *ref_ಪೂರ್ವಪ್ರತ್ಯಯ;
	ರಚನೆ proc_receive_ref *ಮುಂದಿನ;
} *proc_receive_ref;

ಸ್ಥಿರ ಶೂನ್ಯ proc_receive_ref_append(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ);

ಸ್ಥಿರ ಎಣಿಕೆ deny_action parse_deny_action(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಆದರೆ (ಮೌಲ್ಯ) {
		ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "ignore"))
			ಹಿಂತಿರುಗಿ DENY_IGNORE;
		ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "warn"))
			ಹಿಂತಿರುಗಿ DENY_WARN;
		ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "refuse"))
			ಹಿಂತಿರುಗಿ DENY_REFUSE;
		ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "updateinstead"))
			ಹಿಂತಿರುಗಿ DENY_UPDATE_INSTEAD;
	}
	ಆದರೆ (git_config_bool(var, ಮೌಲ್ಯ))
		ಹಿಂತಿರುಗಿ DENY_REFUSE;
	ಹಿಂತಿರುಗಿ DENY_IGNORE;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ receive_pack_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_id_2;
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = parse_hide_refs_config(var, ಮೌಲ್ಯ, "receive", &hidden_refs);

	ಆದರೆ (ಸ್ಥಿತಿ_2)
		ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;

	ಆದರೆ (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.fsck.skiplist") == 0) {
		ಅಕ್ಷರ *ಮಾರ್ಗ;

		ಆದರೆ (git_config_pathname(&ಮಾರ್ಗ, var, ಮೌಲ್ಯ))
			ಹಿಂತಿರುಗಿ -1;
		ಆದರೆ (ಮಾರ್ಗ)
			strbuf_addf(&fsck_ಸಂದೇಶ_types, "%cskiplist=%s",
				    fsck_ಸಂದೇಶ_types.len ? ',' : '=', ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(var, "receive.fsck.", &ಸಂದೇಶ_id_2)) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		ಆದರೆ (is_valid_ಸಂದೇಶ_type(ಸಂದೇಶ_id_2, ಮೌಲ್ಯ))
			strbuf_addf(&fsck_ಸಂದೇಶ_types, "%c%s=%s",
				fsck_ಸಂದೇಶ_types.len ? ',' : '=', ಸಂದೇಶ_id_2, ಮೌಲ್ಯ);
		ಇಲ್ಲದಿದ್ದರೆ
			warning("skipping unknown msg id '%s'", ಸಂದೇಶ_id_2);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.certnonceseed") == 0)
		ಹಿಂತಿರುಗಿ git_config_string(&cert_nonce_ಬೀಜ, var, ಮೌಲ್ಯ);

	ಆದರೆ (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_ಎಲ್ಲೆ = git_config_ulong(var, ಮೌಲ್ಯ, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_ತಳ್ಳು = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_ತಳ್ಳು_options = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.keepalive") == 0) {
		keepalive_ಒಳ_sec = git_config_int(var, ಮೌಲ್ಯ, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.maxinputsize") == 0) {
		max_ಒಳಹರಿವು_ಗಾತ್ರ = git_config_int64(var, ಮೌಲ್ಯ, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "receive.procreceiverefs") == 0) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		proc_receive_ref_append(ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ref(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಆದರೆ (ಕಳುಹಿಸಿದ_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_ಗಮ್ಯ_hex(oid), ಮಾರ್ಗ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		ಆದರೆ (advertise_atomic_ತಳ್ಳು)
			strbuf_addstr(&cap, " atomic");
		ಆದರೆ (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		ಆದರೆ (ತಳ್ಳು_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", ತಳ್ಳು_cert_nonce);
		ಆದರೆ (advertise_ತಳ್ಳು_options)
			strbuf_addstr(&cap, " push-options");
		ಆದರೆ (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_ಗಮ್ಯ_hex(oid), ಮಾರ್ಗ, 0, cap.buf);
		strbuf_release(&cap);
		ಕಳುಹಿಸಿದ_capabilities = 1;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ref_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ oidset *ನೋಡಿದ = ದತ್ತಾಂಶ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = strip_namespace(ref->name);

	ಆದರೆ (ref_is_hidden(ಮಾರ್ಗ, ref->name, &hidden_refs))
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	ಆದರೆ (!ಮಾರ್ಗ) {
		ಆದರೆ (oidset_ಸೇರಿಸು(ನೋಡಿದ, ref->oid))
			ಹಿಂತಿರುಗಿ 0;
		ಮಾರ್ಗ = ".have";
	} ಇಲ್ಲದಿದ್ದರೆ {
		oidset_ಸೇರಿಸು(ನೋಡಿದ, ref->oid);
	}
	show_ref(ಮಾರ್ಗ, ref->oid);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_one_alternate_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				   ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ oidset *ನೋಡಿದ = ದತ್ತಾಂಶ;

	ಆದರೆ (oidset_ಸೇರಿಸು(ನೋಡಿದ, oid))
		ಹಿಂತಿರುಗಿ;

	show_ref(".have", oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_ತಲೆ_info(ಶೂನ್ಯ)
{
	ರಚನೆ refs_for_each_ref_options opts = { 0 };
	ಸ್ಥಿರ ರಚನೆ oidset ನೋಡಿದ = OIDSET_INIT;
	ರಚನೆ strvec excludes_ಸದಿಶ = STRVEC_INIT;

	/*
	 * We need access to the reference names both with and without their
	 * namespace and thus cannot use `refs_for_each_namespaced_ref()`. We
	 * thus have to adapt exclude patterns to carry the namespace prefix
	 * ourselves.
	 */
	opts.exclude_patterns = get_namespaced_exclude_patterns(
		hidden_refs_ಗಮ್ಯ_excludes(&hidden_refs),
		get_git_namespace(), &excludes_ಸದಿಶ);

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      show_ref_cb, &ನೋಡಿದ, &opts);
	odb_for_each_alternate_ref(the_repository->objects,
				   show_one_alternate_ref, &ನೋಡಿದ);

	oidset_clear(&ನೋಡಿದ);
	strvec_clear(&excludes_ಸದಿಶ);

	ಆದರೆ (!ಕಳುಹಿಸಿದ_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#ವ್ಯಾಖ್ಯಾನಿಸು ಸತತ_PROC_RECEIVE_SCHEDULED	1
#ವ್ಯಾಖ್ಯಾನಿಸು ಸತತ_PROC_RECEIVE_RETURNED	2
ರಚನೆ command {
	ರಚನೆ command *ಮುಂದಿನ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *error_string;
	ಅಕ್ಷರ *error_string_owned;
	ರಚನೆ ref_ತಳ್ಳು_report *report;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ skip_update:1,
		     did_not_exist:1,
		     ಸತತ_proc_receive:2;
	ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ;
	ರಚನೆ object_id old_oid;
	ರಚನೆ object_id new_oid;
	ಅಕ್ಷರ ref_name[FLEX_ARRAY]; /*ಇನ್ನಷ್ಟು*/
};

ಸ್ಥಿರ ಶೂನ್ಯ proc_receive_ref_append(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ರಚನೆ proc_receive_ref *ref_ನಮೂನೆ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
	ಪೂರ್ಣಾಂಕ len;

	CALLOC_ARRAY(ref_ನಮೂನೆ, 1);
	p = strchr(ಪೂರ್ವಪ್ರತ್ಯಯ, ':');
	ಆದರೆ (p) {
		ಆಗ (ಪೂರ್ವಪ್ರತ್ಯಯ < p) {
			ಆದರೆ (*ಪೂರ್ವಪ್ರತ್ಯಯ == 'a')
				ref_ನಮೂನೆ->want_ಕೂಡಿಸು = 1;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಪೂರ್ವಪ್ರತ್ಯಯ == 'd')
				ref_ನಮೂನೆ->want_delete = 1;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಪೂರ್ವಪ್ರತ್ಯಯ == 'm')
				ref_ನಮೂನೆ->want_modify = 1;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಪೂರ್ವಪ್ರತ್ಯಯ == '!')
				ref_ನಮೂನೆ->negative_ref = 1;
			ಪೂರ್ವಪ್ರತ್ಯಯ++;
		}
		ಪೂರ್ವಪ್ರತ್ಯಯ++;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ref_ನಮೂನೆ->want_ಕೂಡಿಸು = 1;
		ref_ನಮೂನೆ->want_delete = 1;
		ref_ನಮೂನೆ->want_modify = 1;
	}
	len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ);
	ಆಗ (len && ಪೂರ್ವಪ್ರತ್ಯಯ[len - 1] == '/')
		len--;
	ref_ನಮೂನೆ->ref_ಪೂರ್ವಪ್ರತ್ಯಯ = xmemdupz(ಪೂರ್ವಪ್ರತ್ಯಯ, len);
	ಆದರೆ (!proc_receive_ref) {
		proc_receive_ref = ref_ನಮೂನೆ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ proc_receive_ref *ಕೊನೆ;

		ಕೊನೆ = proc_receive_ref;
		ಆಗ (ಕೊನೆ->ಮುಂದಿನ)
			ಕೊನೆ = ಕೊನೆ->ಮುಂದಿನ;
		ಕೊನೆ->ಮುಂದಿನ = ref_ನಮೂನೆ;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ proc_receive_ref_matches(ರಚನೆ command *cmd)
{
	ರಚನೆ proc_receive_ref *p;

	ಆದರೆ (!proc_receive_ref)
		ಹಿಂತಿರುಗಿ 0;

	ಗಾಗಿ (p = proc_receive_ref; p; p = p->ಮುಂದಿನ) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಹೊಂದಿಕೆ = p->ref_ಪೂರ್ವಪ್ರತ್ಯಯ;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remains;

		ಆದರೆ (!p->want_ಕೂಡಿಸು && is_null_oid(&cmd->old_oid))
			ಮುಂದುವರಿ;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!p->want_delete && is_null_oid(&cmd->new_oid))
			ಮುಂದುವರಿ;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			ಮುಂದುವರಿ;

		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(cmd->ref_name, ಹೊಂದಿಕೆ, &remains) &&
		    (!*remains || *remains == '/')) {
			ಆದರೆ (!p->negative_ref)
				ಹಿಂತಿರುಗಿ 1;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p->negative_ref) {
			ಹಿಂತಿರುಗಿ 1;
		}
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ report_ಸಂದೇಶ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *err, va_list params)
{
	ಪೂರ್ಣಾಂಕ sz;
	ಅಕ್ಷರ ಸಂದೇಶ[4096];

	sz = xsnprintf(ಸಂದೇಶ, ಗಾತ್ರ(ಸಂದೇಶ), "%s", ಪೂರ್ವಪ್ರತ್ಯಯ);
	sz += vsnprintf(ಸಂದೇಶ + sz, ಗಾತ್ರ(ಸಂದೇಶ) - sz, err, params);
	ಆದರೆ (sz > (ಗಾತ್ರ(ಸಂದೇಶ) - 1))
		sz = ಗಾತ್ರ(ಸಂದೇಶ) - 1;
	ಸಂದೇಶ[sz++] = '\n';

	ಆದರೆ (use_sideband)
		send_sideband(1, 2, ಸಂದೇಶ, sz, use_sideband);
	ಇಲ್ಲದಿದ್ದರೆ
		xwrite(2, ಸಂದೇಶ, sz);
}

__attribute__((format (ಬರೆ, 1, 2)))
ಸ್ಥಿರ ಶೂನ್ಯ rp_warning(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *err, ...)
{
	va_list params;
	va_ಆರಂಭ(params, err);
	report_ಸಂದೇಶ("warning: ", err, params);
	va_ಕೊನೆ(params);
}

__attribute__((format (ಬರೆ, 1, 2)))
ಸ್ಥಿರ ಶೂನ್ಯ rp_error(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *err, ...)
{
	va_list params;
	va_ಆರಂಭ(params, err);
	report_ಸಂದೇಶ("error: ", err, params);
	va_ಕೊನೆ(params);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ copy_ಗಮ್ಯ_sideband(ಪೂರ್ಣಾಂಕ ಒಳ, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು ಬಳಸದ, ಶೂನ್ಯ *arg ಬಳಸದ)
{
	ಅಕ್ಷರ ದತ್ತಾಂಶ[128];
	ಪೂರ್ಣಾಂಕ keepalive_ಸಕ್ರಿಯ = 0;

	ಆದರೆ (keepalive_ಒಳ_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	ಆದರೆ (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_ಸಕ್ರಿಯ = 1;

	ಆಗ (1) {
		ssize_t sz;

		ಆದರೆ (keepalive_ಸಕ್ರಿಯ) {
			ರಚನೆ pollfd pfd;
			ಪೂರ್ಣಾಂಕ ret;

			pfd.fd = ಒಳ;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_ಒಳ_sec);

			ಆದರೆ (ret < 0) {
				ಆದರೆ (errno == EINTR)
					ಮುಂದುವರಿ;
				ಇಲ್ಲದಿದ್ದರೆ
					ನಿಲ್ಲಿಸು;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ret == 0) {
				/* no data; send a keepalive packet */
				ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ buf[] = "0005\1";
				write_or_die(1, buf, ಗಾತ್ರ(buf) - 1);
				ಮುಂದುವರಿ;
			} /* else there is actual data to read */
		}

		sz = xread(ಒಳ, ದತ್ತಾಂಶ, ಗಾತ್ರ(ದತ್ತಾಂಶ));
		ಆದರೆ (sz <= 0)
			ನಿಲ್ಲಿಸು;

		ಆದರೆ (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_ಸಕ್ರಿಯ) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p = memchr(ದತ್ತಾಂಶ, '\0', sz);
			ಆದರೆ (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_ಸಕ್ರಿಯ = 1;
				send_sideband(1, 2, ದತ್ತಾಂಶ, p - ದತ್ತಾಂಶ, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - ದತ್ತಾಂಶ + 1), use_sideband);
				ಮುಂದುವರಿ;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, ದತ್ತಾಂಶ, sz, use_sideband);
	}
	close(ಒಳ);
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
ಸ್ಥಿರ ಶೂನ್ಯ prepare_sideband_async(ರಚನೆ async *sideband_async, ಪೂರ್ಣಾಂಕ *saved_stderr, ಪೂರ್ಣಾಂಕ *started)
{
	*started = 0;

	ಆದರೆ (!use_sideband)
		ಹಿಂತಿರುಗಿ;

	memset(sideband_async, 0, ಗಾತ್ರ(*sideband_async));
	sideband_async->proc = copy_ಗಮ್ಯ_sideband;
	sideband_async->ಒಳ = -1;

	ಆದರೆ (!ಆರಂಭ_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		ಆದರೆ (*saved_stderr >= 0)
			dup2(sideband_async->ಒಳ, STDERR_FILENO);
		close(sideband_async->ಒಳ);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ಮುಕ್ತಾಯ_sideband_async(ರಚನೆ async *sideband_async, ಪೂರ್ಣಾಂಕ saved_stderr, ಪೂರ್ಣಾಂಕ started)
{
	ಆದರೆ (!use_sideband)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	ಆದರೆ (started)
		ಮುಕ್ತಾಯ_async(sideband_async);
}

ಸ್ಥಿರ ಶೂನ್ಯ hmac_hash(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಹೊರಹೋಗು,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ_ಒಳ, ಗಾತ್ರ_t ಕೀಲಿ_len,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪಠ್ಯ, ಗಾತ್ರ_t ಪಠ್ಯ_len)
{
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಕೀಲಿ[GIT_MAX_BLKSZ];
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ k_ipad[GIT_MAX_BLKSZ];
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ k_opad[GIT_MAX_BLKSZ];
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(ಕೀಲಿ, '\0', GIT_MAX_BLKSZ);
	ಆದರೆ (the_hash_algo->blksz < ಕೀಲಿ_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, ಕೀಲಿ_ಒಳ, ಕೀಲಿ_len);
		git_hash_final(ಕೀಲಿ, &ctx);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಸ್ಮರಣೆ_ನಕಲು(ಕೀಲಿ, ಕೀಲಿ_ಒಳ, ಕೀಲಿ_len);
	}

	/* RFC 2104 2. (2) & (5) */
	ಗಾಗಿ (i = 0; i < ಗಾತ್ರ(ಕೀಲಿ); i++) {
		k_ipad[i] = ಕೀಲಿ[i] ^ 0x36;
		k_opad[i] = ಕೀಲಿ[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, ಗಾತ್ರ(k_ipad));
	git_hash_update(&ctx, ಪಠ್ಯ, ಪಠ್ಯ_len);
	git_hash_final(ಹೊರಹೋಗು, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, ಗಾತ್ರ(k_opad));
	git_hash_update(&ctx, ಹೊರಹೋಗು, the_hash_algo->rawsz);
	git_hash_final(ಹೊರಹೋಗು, &ctx);
}

ಸ್ಥಿರ ಅಕ್ಷರ *prepare_ತಳ್ಳು_cert_nonce(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, timestamp_t stamp)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, ಮಾರ್ಗ, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_ಬೀಜ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(cert_nonce_ಬೀಜ));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (ಪೂರ್ಣಾಂಕ)the_hash_algo->hexsz, hash_ಗಮ್ಯ_hex(hash));
	ಹಿಂತಿರುಗಿ strbuf_detach(&buf, ಖಾಲಿ);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ constant_memequal(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *b, ಗಾತ್ರ_t n)
{
	ಪೂರ್ಣಾಂಕ res = 0;
	ಗಾತ್ರ_t i;

	ಗಾಗಿ (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *check_nonce(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf)
{
	ಗಾತ್ರ_t noncelen;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಿಕ್ಕಿತು_2 = ಕಂಡುಹಿಡಿ_commit_ಶೀರ್ಷಿಕೆ(buf, "nonce", &noncelen);
	ಅಕ್ಷರ *nonce = ಸಿಕ್ಕಿತು_2 ? xmemdupz(ಸಿಕ್ಕಿತು_2, noncelen) : ಖಾಲಿ;
	timestamp_t stamp, ostamp;
	ಅಕ್ಷರ *bohmac, *expect = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *retval = NONCE_BAD;

	ಆದರೆ (!nonce) {
		retval = NONCE_MISSING;
		ಹೋಗು leave;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ತಳ್ಳು_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		ಹೋಗು leave;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ತಳ್ಳು_cert_nonce, nonce)) {
		retval = NONCE_OK;
		ಹೋಗು leave;
	}

	ಆದರೆ (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		ಹೋಗು leave;
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
	ಆದರೆ (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		ಹೋಗು leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	ಆದರೆ (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		ಹೋಗು leave;
	}

	expect = prepare_ತಳ್ಳು_cert_nonce(service_dir, stamp);
	ಆದರೆ (noncelen != ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		ಹೋಗು leave;
	}
	ಆದರೆ (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		ಹೋಗು leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(ತಳ್ಳು_cert_nonce, ಖಾಲಿ, 10);
	nonce_stamp_slop = (ದೀರ್ಘ)ostamp - (ದೀರ್ಘ)stamp;

	ಆದರೆ (nonce_stamp_slop_ಎಲ್ಲೆ &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_ಎಲ್ಲೆ) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		ಬಿಡುಗಡೆ((ಶೂನ್ಯ *)ತಳ್ಳು_cert_nonce);
		ತಳ್ಳು_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} ಇಲ್ಲದಿದ್ದರೆ {
		retval = NONCE_SLOP;
	}

leave:
	ಬಿಡುಗಡೆ(nonce);
	ಬಿಡುಗಡೆ(expect);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_cert_ತಳ್ಳು_options(ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *ತಳ್ಳು_options)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf = ತಳ್ಳು_cert.buf;

	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option;
	ಗಾತ್ರ_t optionlen;
	ಪೂರ್ಣಾಂಕ options_ನೋಡಿದ = 0;

	ಪೂರ್ಣಾಂಕ retval = 1;

	ಆದರೆ (!*buf)
		ಹಿಂತಿರುಗಿ 1;

	ಆಗ ((option = ಕಂಡುಹಿಡಿ_commit_ಶೀರ್ಷಿಕೆ(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_ನೋಡಿದ++;
		ಆದರೆ (options_ನೋಡಿದ > ತಳ್ಳು_options->nr
		    || xstrncmpz(ತಳ್ಳು_options->ವಸ್ತುಗಳು[options_ನೋಡಿದ - 1].string,
				 option, optionlen))
			ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (options_ನೋಡಿದ != ತಳ್ಳು_options->nr)
		retval = 0;

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_ತಳ್ಳು_cert_sha1(ರಚನೆ ಸತತ_hooks_opt *opt)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ already_ಮುಗಿದ;

	ಆದರೆ (!ತಳ್ಳು_cert.len)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!already_ಮುಗಿದ) {
		ಪೂರ್ಣಾಂಕ bogs /* beginning_of_gpg_sig */;

		already_ಮುಗಿದ = 1;
		ಆದರೆ (odb_write_object(the_repository->objects, ತಳ್ಳು_cert.buf,
				     ತಳ್ಳು_cert.len, OBJ_BLOB, &ತಳ್ಳು_cert_oid))
			oidclr(&ತಳ್ಳು_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', ಗಾತ್ರ(sigcheck));

		bogs = parse_signed_ಬಫರ್(ತಳ್ಳು_cert.buf, ತಳ್ಳು_cert.len);
		sigcheck.payload = xmemdupz(ತಳ್ಳು_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, ತಳ್ಳು_cert.buf + bogs,
				ತಳ್ಳು_cert.len - bogs);

		nonce_ಸ್ಥಿತಿ = check_nonce(sigcheck.payload);
	}
	ಆದರೆ (!is_null_oid(&ತಳ್ಳು_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_ಗಮ್ಯ_hex(&ತಳ್ಳು_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.ಕೀಲಿ ? sigcheck.ಕೀಲಿ : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.ಫಲಿತಾಂಶ);
		ಆದರೆ (ತಳ್ಳು_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     ತಳ್ಳು_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_ಸ್ಥಿತಿ);
			ಆದರೆ (nonce_ಸ್ಥಿತಿ == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

ರಚನೆ receive_hook_feed_state {
	ರಚನೆ command *cmd;
	ರಚನೆ ref_ತಳ್ಳು_report *report;
	ಪೂರ್ಣಾಂಕ skip_broken;
	ರಚನೆ strbuf buf;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ feed_receive_hook_cb(ಪೂರ್ಣಾಂಕ hook_stdin_fd, ಶೂನ್ಯ *pp_cb ಬಳಸದ, ಶೂನ್ಯ *pp_task_cb)
{
	ರಚನೆ receive_hook_feed_state *state = pp_task_cb;
	ರಚನೆ command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	ಆಗ (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->ಮುಂದಿನ;

	ಆದರೆ (!cmd)
		ಹಿಂತಿರುಗಿ 1;  /* no more commands left */

	ಆದರೆ (!state->report)
		state->report = cmd->report;

	ಆದರೆ (state->report) {
		ರಚನೆ object_id *old_oid;
		ರಚನೆ object_id *new_oid;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_ಗಮ್ಯ_hex(old_oid), oid_ಗಮ್ಯ_hex(new_oid),
			    ref_name);

		state->report = state->report->ಮುಂದಿನ;
		ಆದರೆ (!state->report)
			cmd = cmd->ಮುಂದಿನ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_ಗಮ್ಯ_hex(&cmd->old_oid), oid_ಗಮ್ಯ_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->ಮುಂದಿನ;
	}

	state->cmd = cmd;

	ಆದರೆ (state->buf.len > 0) {
		ಪೂರ್ಣಾಂಕ ret = write_ಒಳ_ತುಂಬಿದ(hook_stdin_fd, state->buf.buf, state->buf.len);
		ಆದರೆ (ret < 0) {
			ಆದರೆ (errno == EPIPE)
				ಹಿಂತಿರುಗಿ 1; /* child closed pipe */
			ಹಿಂತಿರುಗಿ ret;
		}
	}

	ಹಿಂತಿರುಗಿ state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

ಸ್ಥಿರ ಶೂನ್ಯ *receive_hook_feed_state_alloc(ಶೂನ್ಯ *feed_pipe_ctx)
{
	ರಚನೆ receive_hook_feed_state *init_state = feed_pipe_ctx;
	ರಚನೆ receive_hook_feed_state *ದತ್ತಾಂಶ;

	CALLOC_ARRAY(ದತ್ತಾಂಶ, 1);
	ದತ್ತಾಂಶ->report = init_state->report;
	ದತ್ತಾಂಶ->cmd = init_state->cmd;
	ದತ್ತಾಂಶ->skip_broken = init_state->skip_broken;
	strbuf_init(&ದತ್ತಾಂಶ->buf, 0);

	ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ receive_hook_feed_state_free(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ receive_hook_feed_state *d = ದತ್ತಾಂಶ;
	ಆದರೆ (!d)
		ಹಿಂತಿರುಗಿ;
	strbuf_release(&d->buf);
	ಬಿಡುಗಡೆ(d);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_receive_hook(ರಚನೆ command *commands,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hook_name,
			    ಪೂರ್ಣಾಂಕ skip_broken,
			    ರಚನೆ odb_transaction *transaction,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *ತಳ್ಳು_options)
{
	ರಚನೆ ಸತತ_hooks_opt opt = ಸತತ_HOOKS_OPT_INIT;
	ರಚನೆ command *iter = commands;
	ರಚನೆ receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	ರಚನೆ async sideband_async;
	ಪೂರ್ಣಾಂಕ sideband_async_started = 0;
	ಪೂರ್ಣಾಂಕ saved_stderr = -1;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (!hook_exists(the_repository, hook_name))
		ಹಿಂತಿರುಗಿ 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	ಆಗ (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->ಮುಂದಿನ;
	ಆದರೆ (!iter)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (ತಳ್ಳು_options) {
		ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ತಳ್ಳು_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     ತಳ್ಳು_options->ವಸ್ತುಗಳು[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)ತಳ್ಳು_options->nr);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strvec_ತಳ್ಳು(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	ಆದರೆ (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_ತಳ್ಳು_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_ದತ್ತಾಂಶ_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_ದತ್ತಾಂಶ_free = receive_hook_feed_state_free;

	ret = ಸತತ_hooks_opt(the_repository, hook_name, &opt);

	ಮುಕ್ತಾಯ_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_update_hook(ರಚನೆ command *cmd)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ hook_name[] = "update";
	ರಚನೆ ಸತತ_hooks_opt opt = ಸತತ_HOOKS_OPT_INIT;
	ರಚನೆ async sideband_async;
	ಪೂರ್ಣಾಂಕ sideband_async_started = 0;
	ಪೂರ್ಣಾಂಕ saved_stderr = -1;
	ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;

	ಆದರೆ (!hook_exists(the_repository, hook_name))
		ಹಿಂತಿರುಗಿ 0;

	strvec_pushl(&opt.ವಾದಗಳು,
		     cmd->ref_name,
		     oid_ಗಮ್ಯ_hex(&cmd->old_oid),
		     oid_ಗಮ್ಯ_hex(&cmd->new_oid),
		     ಖಾಲಿ);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	ಸಂಕೇತ_2 = ಸತತ_hooks_opt(the_repository, hook_name, &opt);

	ಮುಕ್ತಾಯ_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_receive_report_hook(ರಚನೆ strbuf *report)
{
	ರಚನೆ ಮಗು_process proc = ಮಗು_PROCESS_INIT;
	ರಚನೆ async sideband_async;
	ಪೂರ್ಣಾಂಕ sideband_async_started = 0;
	ಪೂರ್ಣಾಂಕ saved_stderr = -1;
	ರಚನೆ strbuf ಹೊರಹೋಗು = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hook_ಮಾರ್ಗ;
	ಪೂರ್ಣಾಂಕ ret;

	hook_ಮಾರ್ಗ = ಕಂಡುಹಿಡಿ_hook(the_repository, "receive-report");
	ಆದರೆ (!hook_ಮಾರ್ಗ)
		ಹಿಂತಿರುಗಿ 0;

	strvec_ತಳ್ಳು(&proc.ವಾದಗಳು, hook_ಮಾರ್ಗ);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_ತಳ್ಳು(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &ಹೊರಹೋಗು,
			   report->len, ಖಾಲಿ, 0);
	sigchain_ತೆಗೆ(SIGPIPE);

	ಮುಕ್ತಾಯ_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	ಆದರೆ (!ret)
		strbuf_ಅದಲುಬದಲು(&ಹೊರಹೋಗು, report);

	strbuf_release(&ಹೊರಹೋಗು);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ರಚನೆ command *ಕಂಡುಹಿಡಿ_command_by_refname(ರಚನೆ command *list,
					       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname)
{
	ಗಾಗಿ (; list; list = list->ಮುಂದಿನ)
		ಆದರೆ (!strcmp(list->ref_name, refname))
			ಹಿಂತಿರುಗಿ list;
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_proc_receive_report(ರಚನೆ packet_reader *reader,
				    ರಚನೆ command *commands,
				    ರಚನೆ strbuf *errmsg)
{
	ರಚನೆ command *cmd;
	ರಚನೆ command *hint = ಖಾಲಿ;
	ರಚನೆ ref_ತಳ್ಳು_report *report = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ new_report = 0;
	ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2 = 0;
	ಪೂರ್ಣಾಂಕ once = 0;
	ಪೂರ್ಣಾಂಕ response = 0;

	ಗಾಗಿ (;;) {
		ರಚನೆ object_id old_oid, new_oid;
		ಅಕ್ಷರ *ತಲೆ;
		ಅಕ್ಷರ *refname;
		ಅಕ್ಷರ *p;
		ಎಣಿಕೆ packet_read_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2;

		ಸ್ಥಿತಿ_2 = packet_reader_read(reader);
		ಆದರೆ (ಸ್ಥಿತಿ_2 != PACKET_READ_ಸಾಮಾನ್ಯ) {
			/* Check whether proc-receive exited abnormally */
			ಆದರೆ (ಸ್ಥಿತಿ_2 == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				ಹಿಂತಿರುಗಿ -1;
			}
			ನಿಲ್ಲಿಸು;
		}
		response++;

		ತಲೆ = reader->ಸಾಲು;
		p = strchr(ತಲೆ, ' ');
		ಆದರೆ (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", ತಲೆ);
			ಸಂಕೇತ_2 = -1;
			ಮುಂದುವರಿ;
		}
		*p++ = '\0';
		ಆದರೆ (!strcmp(ತಲೆ, "option")) {
			ಅಕ್ಷರ *ಕೀಲಿ;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *val;

			ಆದರೆ (!hint || !(report || new_report)) {
				ಆದರೆ (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				ಸಂಕೇತ_2 = -1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (new_report) {
				ಆದರೆ (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} ಇಲ್ಲದಿದ್ದರೆ {
					report = hint->report;
					ಆಗ (report->ಮುಂದಿನ)
						report = report->ಮುಂದಿನ;
					report->ಮುಂದಿನ = xcalloc(1, ಗಾತ್ರ(ರಚನೆ ref_ತಳ್ಳು_report));
					report = report->ಮುಂದಿನ;
				}
				new_report = 0;
			}
			ಕೀಲಿ = p;
			p = strchr(ಕೀಲಿ, ' ');
			ಆದರೆ (p)
				*p++ = '\0';
			val = p;
			ಆದರೆ (!strcmp(ಕೀಲಿ, "refname"))
				report->ref_name = xstrdup_or_null(val);
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ಕೀಲಿ, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ಕೀಲಿ, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ಕೀಲಿ, "forced-update"))
				report->forced_update = 1;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ಕೀಲಿ, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->ಸತತ_proc_receive = 0;
			ಮುಂದುವರಿ;
		}

		report = ಖಾಲಿ;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		ಆದರೆ (p)
			*p++ = '\0';
		ಆದರೆ (strcmp(ತಲೆ, "ok") && strcmp(ತಲೆ, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    ತಲೆ, refname);
			ಸಂಕೇತ_2 = -1;
			ಮುಂದುವರಿ;
		}

		/* first try searching at our hint, falling back to all refs */
		ಆದರೆ (hint)
			hint = ಕಂಡುಹಿಡಿ_command_by_refname(hint, refname);
		ಆದರೆ (!hint)
			hint = ಕಂಡುಹಿಡಿ_command_by_refname(commands, refname);
		ಆದರೆ (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			ಸಂಕೇತ_2 = -1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!hint->ಸತತ_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			ಸಂಕೇತ_2 = -1;
			ಮುಂದುವರಿ;
		}
		hint->ಸತತ_proc_receive |= ಸತತ_PROC_RECEIVE_RETURNED;
		ಆದರೆ (!strcmp(ತಲೆ, "ng")) {
			ಆದರೆ (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			ಇಲ್ಲದಿದ್ದರೆ
				hint->error_string = "failed";
			ಸಂಕೇತ_2 = -1;
			ಮುಂದುವರಿ;
		}
		new_report = 1;
	}

	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ)
		ಆದರೆ (cmd->ಸತತ_proc_receive && !cmd->error_string &&
		    !(cmd->ಸತತ_proc_receive & ಸತತ_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    ಸಂಕೇತ_2 = -1;
		}
	ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_proc_receive_hook(ರಚನೆ command *commands,
				 ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *ತಳ್ಳು_options)
{
	ರಚನೆ ಮಗು_process proc = ಮಗು_PROCESS_INIT;
	ರಚನೆ async muxer;
	ರಚನೆ command *cmd;
	ರಚನೆ packet_reader reader;
	ರಚನೆ strbuf cap = STRBUF_INIT;
	ರಚನೆ strbuf errmsg = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ hook_use_ತಳ್ಳು_options = 0;
	ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ = 0;
	ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hook_ಮಾರ್ಗ = ಕಂಡುಹಿಡಿ_hook(the_repository, "proc-receive");

	ಆದರೆ (!hook_ಮಾರ್ಗ) {
		rp_error("cannot find hook 'proc-receive'");
		ಹಿಂತಿರುಗಿ -1;
	}

	strvec_ತಳ್ಳು(&proc.ವಾದಗಳು, hook_ಮಾರ್ಗ);
	proc.ಒಳ = -1;
	proc.ಹೊರಹೋಗು = -1;
	proc.trace2_hook_name = "proc-receive";

	ಆದರೆ (use_sideband) {
		memset(&muxer, 0, ಗಾತ್ರ(muxer));
		muxer.proc = copy_ಗಮ್ಯ_sideband;
		muxer.ಒಳ = -1;
		ಸಂಕೇತ_2 = ಆರಂಭ_async(&muxer);
		ಆದರೆ (ಸಂಕೇತ_2)
			ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
		proc.err = muxer.ಒಳ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		proc.err = 0;
	}

	ಸಂಕೇತ_2 = ಆರಂಭ_command(&proc);
	ಆದರೆ (ಸಂಕೇತ_2) {
		ಆದರೆ (use_sideband)
			ಮುಕ್ತಾಯ_async(&muxer);
		ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
	}

	sigchain_ತಳ್ಳು(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.ಹೊರಹೋಗು, ಖಾಲಿ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	ಆದರೆ (use_atomic)
		strbuf_addstr(&cap, " atomic");
	ಆದರೆ (use_ತಳ್ಳು_options)
		strbuf_addstr(&cap, " push-options");
	ಆದರೆ (cap.len) {
		ಸಂಕೇತ_2 = packet_write_fmt_gently(proc.ಒಳ, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಸಂಕೇತ_2 = packet_write_fmt_gently(proc.ಒಳ, "version=1\n");
	}
	ಆದರೆ (!ಸಂಕೇತ_2)
		ಸಂಕೇತ_2 = packet_flush_gently(proc.ಒಳ);

	ಆದರೆ (!ಸಂಕೇತ_2)
		ಗಾಗಿ (;;) {
			ಪೂರ್ಣಾಂಕ linelen;
			ಎಣಿಕೆ packet_read_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2;

			ಸ್ಥಿತಿ_2 = packet_reader_read(&reader);
			ಆದರೆ (ಸ್ಥಿತಿ_2 != PACKET_READ_ಸಾಮಾನ್ಯ) {
				/* Check whether proc-receive exited abnormally */
				ಆದರೆ (ಸ್ಥಿತಿ_2 == PACKET_READ_EOF)
					ಸಂಕೇತ_2 = -1;
				ನಿಲ್ಲಿಸು;
			}

			ಆದರೆ (reader.pktlen > 8 && starts_with(reader.ಸಾಲು, "version=")) {
				ಆವೃತ್ತಿ = atoi(reader.ಸಾಲು + 8);
				linelen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(reader.ಸಾಲು);
				ಆದರೆ (linelen < reader.pktlen) {
					ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *feature_list = reader.ಸಾಲು + linelen + 1;
					ಆದರೆ (parse_feature_request(feature_list, "push-options"))
						hook_use_ತಳ್ಳು_options = 1;
				}
			}
		}

	ಆದರೆ (ಸಂಕೇತ_2) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		ಹೋಗು cleanup;
	}

	ಆಯ್ಕೆ (ಆವೃತ್ತಿ) {
	ಸ್ಥಿತಿ 0:
		/* fallthrough */
	ಸ್ಥಿತಿ 1:
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    ಆವೃತ್ತಿ);
		ಸಂಕೇತ_2 = -1;
		ಹೋಗು cleanup;
	}

	/* Send commands */
	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		ಆದರೆ (!cmd->ಸತತ_proc_receive || cmd->skip_update || cmd->error_string)
			ಮುಂದುವರಿ;
		ಸಂಕೇತ_2 = packet_write_fmt_gently(proc.ಒಳ, "%s %s %s",
					       oid_ಗಮ್ಯ_hex(&cmd->old_oid),
					       oid_ಗಮ್ಯ_hex(&cmd->new_oid),
					       cmd->ref_name);
		ಆದರೆ (ಸಂಕೇತ_2)
			ನಿಲ್ಲಿಸು;
	}
	ಆದರೆ (!ಸಂಕೇತ_2)
		ಸಂಕೇತ_2 = packet_flush_gently(proc.ಒಳ);
	ಆದರೆ (ಸಂಕೇತ_2) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		ಹೋಗು cleanup;
	}

	/* Send push options */
	ಆದರೆ (hook_use_ತಳ್ಳು_options) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

		for_each_string_list_ಅಂಶ(ಅಂಶ, ತಳ್ಳು_options) {
			ಸಂಕೇತ_2 = packet_write_fmt_gently(proc.ಒಳ, "%s", ಅಂಶ->string);
			ಆದರೆ (ಸಂಕೇತ_2)
				ನಿಲ್ಲಿಸು;
		}
		ಆದರೆ (!ಸಂಕೇತ_2)
			ಸಂಕೇತ_2 = packet_flush_gently(proc.ಒಳ);
		ಆದರೆ (ಸಂಕೇತ_2) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			ಹೋಗು cleanup;
		}
	}

	/* Read result from proc-receive */
	ಸಂಕೇತ_2 = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.ಒಳ);
	close(proc.ಹೊರಹೋಗು);
	ಆದರೆ (use_sideband)
		ಮುಕ್ತಾಯ_async(&muxer);
	ಆದರೆ (ಮುಕ್ತಾಯ_command(&proc))
		ಸಂಕೇತ_2 = -1;
	ಆದರೆ (errmsg.len >0) {
		ಅಕ್ಷರ *p = errmsg.buf;

		p += errmsg.len - 1;
		ಆದರೆ (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_ತೆಗೆ(SIGPIPE);

	ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refuse_unconfigured_deny_ಸಂದೇಶ =
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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refuse_unconfigured_deny_delete_current_ಸಂದೇಶ =
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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *command_singleton_iterator(ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_shallow_ref(ರಚನೆ command *cmd, ರಚನೆ shallow_info *si)
{
	ರಚನೆ shallow_ಬೀಗ shallow_ಬೀಗ = SHALLOW_ಬೀಗ_INIT;
	ರಚನೆ oid_array extra = OID_ARRAY_INIT;
	ರಚನೆ check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t ಮುಖವಾಡ = 1 << (cmd->ಸೂಚ್ಯಂಕ % 32);
	ಪೂರ್ಣಾಂಕ i;

	trace_printf_ಕೀಲಿ(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	ಗಾಗಿ (i = 0; i < si->shallow->nr; i++)
		ಆದರೆ (si->ಬಳಸಿದ_shallow[i] &&
		    (si->ಬಳಸಿದ_shallow[i][cmd->ಸೂಚ್ಯಂಕ / 32] & ಮುಖವಾಡ) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	ಸಿದ್ಧತೆ_alternate_shallow(&shallow_ಬೀಗ, &opt.shallow_file, &extra);
	ಆದರೆ (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_ಬೀಗ);
		oid_array_clear(&extra);
		ಹಿಂತಿರುಗಿ -1;
	}

	commit_shallow_file(the_repository, &shallow_ಬೀಗ);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	ಗಾಗಿ (i = 0; i < extra.nr; i++)
		ರಿಜಿಸ್ಟರ್_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->ಸೂಚ್ಯಂಕ] = 0;
	oid_array_clear(&extra);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ್ಳು_ಗಮ್ಯ_deploy(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *sha1,
				  ರಚನೆ strvec *env,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ worktree *worktree)
{
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;

	strvec_pushl(&ಮಗು.ವಾದಗಳು, "update-index", "-q", "--ignore-submodules",
		     "--refresh", ಖಾಲಿ);
	strvec_pushv(&ಮಗು.env, env->v);
	ಮಗು.dir = worktree->ಮಾರ್ಗ;
	ಮಗು.no_stdin = 1;
	ಮಗು.stdout_ಗಮ್ಯ_stderr = 1;
	ಮಗು.git_cmd = 1;
	ಆದರೆ (ಸತತ_command(&ಮಗು))
		ಹಿಂತಿರುಗಿ "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	ಮಗು_process_init(&ಮಗು);
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "diff-files", "--quiet",
		     "--ignore-submodules", "--", ಖಾಲಿ);
	strvec_pushv(&ಮಗು.env, env->v);
	ಮಗು.dir = worktree->ಮಾರ್ಗ;
	ಮಗು.no_stdin = 1;
	ಮಗು.stdout_ಗಮ್ಯ_stderr = 1;
	ಮಗು.git_cmd = 1;
	ಆದರೆ (ಸತತ_command(&ಮಗು))
		ಹಿಂತಿರುಗಿ "Working directory has unstaged changes";

	ಮಗು_process_init(&ಮಗು);
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "diff-index", "--quiet", "--cached",
		     "--ignore-submodules",
		     /*
		      * diff-index with either HEAD or an empty tree
		      *
		      * NEEDSWORK: is_null_oid() cannot know whether it's an
		      * unborn HEAD or a corrupt ref. It works for now because
		      * it's only needed to know if we are comparing HEAD or an
		      * empty tree.
		      */
		     !is_null_oid(&worktree->ತಲೆ_oid) ? "HEAD" :
		     ಖಾಲಿಗಳು_ವೃಕ್ಷ_oid_hex(the_repository->hash_algo), "--", ಖಾಲಿ);
	strvec_pushv(&ಮಗು.env, env->v);
	ಮಗು.no_stdin = 1;
	ಮಗು.no_stdout = 1;
	ಮಗು.stdout_ಗಮ್ಯ_stderr = 0;
	ಮಗು.git_cmd = 1;
	ಆದರೆ (ಸತತ_command(&ಮಗು))
		ಹಿಂತಿರುಗಿ "Working directory has staged changes";

	ಮಗು_process_init(&ಮಗು);
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "read-tree", "-u", "-m", hash_ಗಮ್ಯ_hex(sha1),
		     ಖಾಲಿ);
	strvec_pushv(&ಮಗು.env, env->v);
	ಮಗು.dir = worktree->ಮಾರ್ಗ;
	ಮಗು.no_stdin = 1;
	ಮಗು.no_stdout = 1;
	ಮಗು.stdout_ಗಮ್ಯ_stderr = 0;
	ಮಗು.git_cmd = 1;
	ಆದರೆ (ಸತತ_command(&ಮಗು))
		ಹಿಂತಿರುಗಿ "Could not update working tree to new HEAD";

	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ್ಳು_ಗಮ್ಯ_checkout_hook = "push-to-checkout";

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ್ಳು_ಗಮ್ಯ_checkout(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *hash,
				    ಪೂರ್ಣಾಂಕ *invoked_hook,
				    ರಚನೆ strvec *env,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *work_ವೃಕ್ಷ)
{
	ರಚನೆ ಸತತ_hooks_opt opt = ಸತತ_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_ಮಾರ್ಗ(work_ವೃಕ್ಷ));
	strvec_ತಳ್ಳು(&opt.ವಾದಗಳು, hash_ಗಮ್ಯ_hex(hash));
	ಆದರೆ (ಸತತ_hooks_opt(the_repository, ತಳ್ಳು_ಗಮ್ಯ_checkout_hook, &opt))
		ಹಿಂತಿರುಗಿ "push-to-checkout hook declined";
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *update_worktree(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *sha1, ಸ್ಥಿರಾಂಕ ರಚನೆ worktree *worktree)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *retval;
	ಅಕ್ಷರ *git_dir;
	ರಚನೆ strvec env = STRVEC_INIT;
	ಪೂರ್ಣಾಂಕ invoked_hook;

	ಆದರೆ (!worktree || !worktree->ಮಾರ್ಗ)
		BUG("worktree->path must be non-NULL");

	ಆದರೆ (worktree->is_bare)
		ಹಿಂತಿರುಗಿ "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_ಮಾರ್ಗ(git_dir));

	retval = ತಳ್ಳು_ಗಮ್ಯ_checkout(sha1, &invoked_hook, &env, worktree->ಮಾರ್ಗ);
	ಆದರೆ (!invoked_hook)
		retval = ತಳ್ಳು_ಗಮ್ಯ_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	ಬಿಡುಗಡೆ(git_dir);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *update(ರಚನೆ command *cmd, ರಚನೆ shallow_info *si)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = cmd->ref_name;
	ರಚನೆ strbuf namespaced_name_buf = STRBUF_INIT;
	ಸ್ಥಿರ ಅಕ್ಷರ *namespaced_name;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ret;
	ರಚನೆ object_id *old_oid = &cmd->old_oid;
	ರಚನೆ object_id *new_oid = &cmd->new_oid;
	ಪೂರ್ಣಾಂಕ do_update_worktree = 0;
	ರಚನೆ worktree **worktrees = get_worktrees(the_repository);
	ಸ್ಥಿರಾಂಕ ರಚನೆ worktree *worktree =
		ಕಂಡುಹಿಡಿ_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	ಆದರೆ (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		ಹೋಗು ಹೊರಹೋಗು;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	ಬಿಡುಗಡೆ(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, ಖಾಲಿ);

	ಆದರೆ (worktree && !worktree->is_bare) {
		ಆಯ್ಕೆ (deny_current_branch) {
		ಸ್ಥಿತಿ DENY_IGNORE:
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ DENY_WARN:
			rp_warning("updating the current branch");
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ DENY_REFUSE:
		ಸ್ಥಿತಿ DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			ಆದರೆ (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			ಹೋಗು ಹೊರಹೋಗು;
		ಸ್ಥಿತಿ DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			ನಿಲ್ಲಿಸು;
		}
	}

	ಆದರೆ (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_ಗಮ್ಯ_hex(new_oid));
		ret = "bad pack";
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		ಆದರೆ (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			ಹೋಗು ಹೊರಹೋಗು;
		}

		ಆದರೆ (worktree || (ತಲೆ_name && !strcmp(namespaced_name, ತಲೆ_name))) {
			ಆಯ್ಕೆ (deny_delete_current) {
			ಸ್ಥಿತಿ DENY_IGNORE:
				ನಿಲ್ಲಿಸು;
			ಸ್ಥಿತಿ DENY_WARN:
				rp_warning("deleting the current branch");
				ನಿಲ್ಲಿಸು;
			ಸ್ಥಿತಿ DENY_REFUSE:
			ಸ್ಥಿತಿ DENY_UNCONFIGURED:
			ಸ್ಥಿತಿ DENY_UPDATE_INSTEAD:
				ಆದರೆ (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				ಹೋಗು ಹೊರಹೋಗು;
			ಪೂರ್ವನಿಯೋಜಿತ:
				ret = "Invalid denyDeleteCurrent setting";
				ಹೋಗು ಹೊರಹೋಗು;
			}
		}
	}

	ಆದರೆ (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		ರಚನೆ object *old_object, *new_object;
		ರಚನೆ commit *old_commit, *new_commit;
		ಪೂರ್ಣಾಂಕ ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		ಆದರೆ (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			ಹೋಗು ಹೊರಹೋಗು;
		}
		old_commit = (ರಚನೆ commit *)old_object;
		new_commit = (ರಚನೆ commit *)new_object;
		ret2 = repo_ಒಳ_ವಿಲೀನ_bases(the_repository, old_commit, new_commit);
		ಆದರೆ (ret2 < 0)
			ನಿರ್ಗಮಿಸು(128);
		ಆದರೆ (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}
	ಆದರೆ (ಸತತ_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		ಆದರೆ (ret)
			ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (is_null_oid(new_oid)) {
		ರಚನೆ strbuf err = STRBUF_INIT;
		ಆದರೆ (!parse_object(the_repository, old_oid)) {
			old_oid = ಖಾಲಿ;
			ಆದರೆ (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} ಇಲ್ಲದಿದ್ದರೆ {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		ಆದರೆ (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   ಖಾಲಿ, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} ಇಲ್ಲದಿದ್ದರೆ {
			ret = ಖಾಲಿ; /* good */
		}
		strbuf_release(&err);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಎಣಿಕೆ ref_transaction_error tx_err;
		ರಚನೆ strbuf err = STRBUF_INIT;
		ಆದರೆ (shallow_update && si->shallow_ref[cmd->ಸೂಚ್ಯಂಕ] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			ಹೋಗು ಹೊರಹೋಗು;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  ಖಾಲಿ, ಖಾಲಿ,
						  0, "push",
						  &err);
		ಆದರೆ (tx_err) {
			rp_error("%s", err.buf);
			ಆದರೆ (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			ಇಲ್ಲದಿದ್ದರೆ
				ret = ref_transaction_error_ಸಂದೇಶ(tx_err);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ret = ಖಾಲಿ; /* good */
		}
		strbuf_release(&err);
	}

ಹೊರಹೋಗು:
	free_worktrees(worktrees);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_update_post_hook(ರಚನೆ command *commands)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ hook_name[] = "post-update";
	ರಚನೆ ಸತತ_hooks_opt opt = ಸತತ_HOOKS_OPT_INIT;
	ರಚನೆ async sideband_async;
	ರಚನೆ command *cmd;
	ಪೂರ್ಣಾಂಕ sideband_async_started = 0;
	ಪೂರ್ಣಾಂಕ saved_stderr = -1;

	ಆದರೆ (!hook_exists(the_repository, hook_name))
		ಹಿಂತಿರುಗಿ;

	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		ಆದರೆ (cmd->error_string || cmd->did_not_exist)
			ಮುಂದುವರಿ;
		strvec_ತಳ್ಳು(&opt.ವಾದಗಳು, cmd->ref_name);
	}
	ಆದರೆ (!opt.ವಾದಗಳು.nr)
		ಹಿಂತಿರುಗಿ;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	ಸತತ_hooks_opt(the_repository, hook_name, &opt);

	ಮುಕ್ತಾಯ_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

ಸ್ಥಿರ ಶೂನ್ಯ check_aliased_update_ಆಂತರಿಕ(ರಚನೆ command *cmd,
					  ರಚನೆ string_list *list,
					  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dst_name, ಪೂರ್ಣಾಂಕ flag)
{
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ರಚನೆ command *dst_cmd;

	ಆದರೆ (!(flag & REF_ISSYMREF))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		ಹಿಂತಿರುಗಿ;
	}
	dst_name = strip_namespace(dst_name);

	ಆದರೆ (!(ಅಂಶ = string_list_lookup(list, dst_name)))
		ಹಿಂತಿರುಗಿ;

	cmd->skip_update = 1;

	dst_cmd = (ರಚನೆ command *) ಅಂಶ->util;

	ಆದರೆ (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		ಹಿಂತಿರುಗಿ;

	dst_cmd->skip_update = 1;

	rp_error("refusing inconsistent update between symref '%s' (%s..%s) and"
		 " its target '%s' (%s..%s)",
		 cmd->ref_name,
		 repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &cmd->old_oid, DEFAULT_ABBREV),
		 repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &cmd->new_oid, DEFAULT_ABBREV),
		 dst_cmd->ref_name,
		 repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &dst_cmd->old_oid, DEFAULT_ABBREV),
		 repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &dst_cmd->new_oid, DEFAULT_ABBREV));

	cmd->error_string = dst_cmd->error_string =
		"inconsistent aliased update";
}

ಸ್ಥಿರ ಶೂನ್ಯ check_aliased_update(ರಚನೆ command *cmd, ರಚನೆ string_list *list)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dst_name;
	ಪೂರ್ಣಾಂಕ flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, ಖಾಲಿ, &flag);
	check_aliased_update_ಆಂತರಿಕ(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ check_aliased_updates(ರಚನೆ command *commands)
{
	ರಚನೆ command *cmd;
	ರಚನೆ string_list ref_list = STRING_LIST_INIT_NODUP;

	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ =
			string_list_append(&ref_list, cmd->ref_name);
		ಅಂಶ->util = (ಶೂನ್ಯ *)cmd;
	}
	string_list_ವಿಂಗಡಿಸು(&ref_list);

	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		ಆದರೆ (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *command_singleton_iterator(ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ command **cmd_list = cb_ದತ್ತಾಂಶ;
	ರಚನೆ command *cmd = *cmd_list;

	ಆದರೆ (!cmd || is_null_oid(&cmd->new_oid))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	*cmd_list = ಖಾಲಿ; /* this returns only one */
	ಹಿಂತಿರುಗಿ &cmd->new_oid;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_connectivity_errors(ರಚನೆ command *commands,
				    ರಚನೆ shallow_info *si,
				    ರಚನೆ odb_transaction *transaction)
{
	ರಚನೆ command *cmd;

	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		ರಚನೆ command *singleton = cmd;
		ರಚನೆ check_connected_options opt = CHECK_CONNECTED_INIT;
		ರಚನೆ strvec env = STRVEC_INIT;

		ಆದರೆ (shallow_update && si->shallow_ref[cmd->ಸೂಚ್ಯಂಕ])
			/* to be checked in update_shallow_ref() */
			ಮುಂದುವರಿ;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		ಆದರೆ (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			ಮುಂದುವರಿ;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

ರಚನೆ iterate_ದತ್ತಾಂಶ {
	ರಚನೆ command *cmds;
	ರಚನೆ shallow_info *si;
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *iterate_receive_command_list(ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ iterate_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb_ದತ್ತಾಂಶ;
	ರಚನೆ command **cmd_list = &ದತ್ತಾಂಶ->cmds;
	ರಚನೆ command *cmd = *cmd_list;

	ಗಾಗಿ (; cmd; cmd = cmd->ಮುಂದಿನ) {
		ಆದರೆ (shallow_update && ದತ್ತಾಂಶ->si->shallow_ref[cmd->ಸೂಚ್ಯಂಕ])
			/* to be checked in update_shallow_ref() */
			ಮುಂದುವರಿ;
		ಆದರೆ (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->ಮುಂದಿನ;
			ಹಿಂತಿರುಗಿ &cmd->new_oid;
		}
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ reject_updates_ಗಮ್ಯ_hidden(ರಚನೆ command *commands)
{
	ರಚನೆ strbuf refname_ತುಂಬಿದ = STRBUF_INIT;
	ಗಾತ್ರ_t ಪೂರ್ವಪ್ರತ್ಯಯ_len;
	ರಚನೆ command *cmd;

	strbuf_addstr(&refname_ತುಂಬಿದ, get_git_namespace());
	ಪೂರ್ವಪ್ರತ್ಯಯ_len = refname_ತುಂಬಿದ.len;

	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		ಆದರೆ (cmd->error_string)
			ಮುಂದುವರಿ;

		strbuf_setlen(&refname_ತುಂಬಿದ, ಪೂರ್ವಪ್ರತ್ಯಯ_len);
		strbuf_addstr(&refname_ತುಂಬಿದ, cmd->ref_name);

		ಆದರೆ (!ref_is_hidden(cmd->ref_name, refname_ತುಂಬಿದ.buf, &hidden_refs))
			ಮುಂದುವರಿ;
		ಆದರೆ (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		ಇಲ್ಲದಿದ್ದರೆ
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_ತುಂಬಿದ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ should_process_cmd(ರಚನೆ command *cmd)
{
	ಹಿಂತಿರುಗಿ !cmd->error_string && !cmd->skip_update;
}

ಸ್ಥಿರ ಶೂನ್ಯ BUG_if_skipped_connectivity_check(ರಚನೆ command *commands,
					       ರಚನೆ shallow_info *si)
{
	ರಚನೆ command *cmd;

	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		ಆದರೆ (should_process_cmd(cmd) && si->shallow_ref[cmd->ಸೂಚ್ಯಂಕ])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

ಸ್ಥಿರ ಶೂನ್ಯ ref_transaction_rejection_handler(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
					      ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_oid ಬಳಸದ,
					      ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *new_oid ಬಳಸದ,
					      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old_ಗುರಿ ಬಳಸದ,
					      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_ಗುರಿ ಬಳಸದ,
					      ಎಣಿಕೆ ref_transaction_error err,
					      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *details,
					      ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ strmap *failed_refs = cb_ದತ್ತಾಂಶ;

	ಆದರೆ (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (ಅಕ್ಷರ *)ref_transaction_error_ಸಂದೇಶ(err));
}

ಸ್ಥಿರ ಶೂನ್ಯ execute_commands_non_atomic(ರಚನೆ command *commands,
					ರಚನೆ shallow_info *si)
{
	ರಚನೆ command *cmd;
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reported_error = ಖಾಲಿ;
	ರಚನೆ strmap failed_refs = STRMAP_INIT;

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
	ಎಣಿಕೆ processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	ಗಾಗಿ (ಎಣಿಕೆ processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
			ಆದರೆ (!should_process_cmd(cmd) || cmd->ಸತತ_proc_receive)
				ಮುಂದುವರಿ;

			ಆದರೆ (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				ಮುಂದುವರಿ;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				ಮುಂದುವರಿ;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			ಆದರೆ (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				ಆದರೆ (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					ಹೋಗು failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		ಆದರೆ (!transaction)
			ಹೋಗು cleanup;

		ಆದರೆ (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			ಹೋಗು failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		ಆದರೆ (strmap_ಖಾಲಿಗಳು(&failed_refs))
			ಹೋಗು cleanup;

	failure:
		ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
			ಆದರೆ (reported_error)
				cmd->error_string = reported_error;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strmap_ಒಳಗೊಂಡಿದೆ(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = ಖಾಲಿ;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ execute_commands_atomic(ರಚನೆ command *commands,
					ರಚನೆ shallow_info *si)
{
	ರಚನೆ command *cmd;
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ಆದರೆ (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		ಹೋಗು failure;
	}

	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		ಆದರೆ (!should_process_cmd(cmd) || cmd->ಸತತ_proc_receive)
			ಮುಂದುವರಿ;

		cmd->error_string = update(cmd, si);

		ಆದರೆ (cmd->error_string)
			ಹೋಗು failure;
	}

	ಆದರೆ (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		ಹೋಗು failure;
	}
	ಹೋಗು cleanup;

failure:
	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ)
		ಆದರೆ (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ execute_commands(ರಚನೆ command *commands,
			     ಪೂರ್ಣಾಂಕ unpacker_error,
			     ರಚನೆ shallow_info *si,
			     ರಚನೆ odb_transaction *transaction,
			     ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *ತಳ್ಳು_options)
{
	ರಚನೆ check_connected_options opt = CHECK_CONNECTED_INIT;
	ರಚನೆ command *cmd;
	ರಚನೆ iterate_ದತ್ತಾಂಶ ದತ್ತಾಂಶ;
	ರಚನೆ async muxer;
	ಪೂರ್ಣಾಂಕ err_fd = 0;
	ಪೂರ್ಣಾಂಕ ಸತತ_proc_receive = 0;

	ಆದರೆ (unpacker_error) {
		ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ)
			cmd->error_string = "unpacker error";
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (!skip_connectivity_check) {
		ರಚನೆ strvec env = STRVEC_INIT;

		ಆದರೆ (use_sideband) {
			memset(&muxer, 0, ಗಾತ್ರ(muxer));
			muxer.proc = copy_ಗಮ್ಯ_sideband;
			muxer.ಒಳ = -1;
			ಆದರೆ (!ಆರಂಭ_async(&muxer))
				err_fd = muxer.ಒಳ;
			/* ...else, continue without relaying sideband */
		}

		ದತ್ತಾಂಶ.cmds = commands;
		ದತ್ತಾಂಶ.si = si;
		opt.err_fd = err_fd;
		opt.ಪ್ರಗತಿ = err_fd && !quiet;
		odb_transaction_env(transaction, &env);
		opt.env = env.v;
		opt.exclude_hidden_refs_section = "receive";

		ಆದರೆ (check_connected(iterate_receive_command_list, &ದತ್ತಾಂಶ, &opt))
			ಗಣ_connectivity_errors(commands, si, transaction);

		ಆದರೆ (use_sideband)
			ಮುಕ್ತಾಯ_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_ಗಮ್ಯ_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	ಆದರೆ (proc_receive_ref) {
		ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
			ಆದರೆ (!should_process_cmd(cmd))
				ಮುಂದುವರಿ;

			ಆದರೆ (proc_receive_ref_matches(cmd)) {
				cmd->ಸತತ_proc_receive = ಸತತ_PROC_RECEIVE_SCHEDULED;
				ಸತತ_proc_receive = 1;
			}
		}
	}

	ಆದರೆ (ಸತತ_receive_hook(commands, "pre-receive", 0, transaction, ತಳ್ಳು_options)) {
		ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
			ಆದರೆ (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	ಗಾಗಿ (cmd = commands; cmd && cmd->error_string; cmd = cmd->ಮುಂದಿನ)
		; /* nothing */
	ಆದರೆ (!cmd)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	ಆದರೆ (odb_transaction_commit(transaction)) {
		ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
			ಆದರೆ (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		ಹಿಂತಿರುಗಿ;
	}

	check_aliased_updates(commands);

	ಬಿಡುಗಡೆ(ತಲೆ_name_ಗಮ್ಯ_free);
	ತಲೆ_name = ತಲೆ_name_ಗಮ್ಯ_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, ಖಾಲಿ,
							    ಖಾಲಿ);

	ಆದರೆ (ಸತತ_proc_receive &&
	    ಸತತ_proc_receive_hook(commands, ತಳ್ಳು_options))
		ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ)
			ಆದರೆ (!cmd->error_string &&
			    !(cmd->ಸತತ_proc_receive & ಸತತ_PROC_RECEIVE_RETURNED) &&
			    (cmd->ಸತತ_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	ಆದರೆ (use_atomic)
		execute_commands_atomic(commands, si);
	ಇಲ್ಲದಿದ್ದರೆ
		execute_commands_non_atomic(commands, si);

	ಆದರೆ (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

ಸ್ಥಿರ ರಚನೆ command **ಸರತಿ_command(ರಚನೆ command **ಬಾಲ,
				      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಾಲು,
				      ಪೂರ್ಣಾಂಕ linelen)
{
	ರಚನೆ object_id old_oid, new_oid;
	ರಚನೆ command *cmd;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname;
	ಪೂರ್ಣಾಂಕ reflen;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;

	ಆದರೆ (parse_oid_hex(ಸಾಲು, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", ಸಾಲು);

	refname = p;
	reflen = linelen - (p - ಸಾಲು);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*ಬಾಲ = cmd;
	ಹಿಂತಿರುಗಿ &cmd->ಮುಂದಿನ;
}

ಸ್ಥಿರ ಶೂನ್ಯ free_commands(ರಚನೆ command *commands)
{
	ಆಗ (commands) {
		ರಚನೆ command *ಮುಂದಿನ = commands->ಮುಂದಿನ;

		ref_ತಳ್ಳು_report_free(commands->report);
		ಬಿಡುಗಡೆ(commands->error_string_owned);
		ಬಿಡುಗಡೆ(commands);
		commands = ಮುಂದಿನ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿ_commands_from_cert(ರಚನೆ command **ಬಾಲ,
				     ರಚನೆ strbuf *ತಳ್ಳು_cert)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *boc, *eoc;

	ಆದರೆ (*ಬಾಲ)
		die("protocol error: got both push certificate and unsigned commands");

	boc = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(ತಳ್ಳು_cert->buf, "\n\n");
	ಆದರೆ (!boc)
		die("malformed push certificate %.*s", 100, ತಳ್ಳು_cert->buf);
	ಇಲ್ಲದಿದ್ದರೆ
		boc += 2;
	eoc = ತಳ್ಳು_cert->buf + parse_signed_ಬಫರ್(ತಳ್ಳು_cert->buf, ತಳ್ಳು_cert->len);

	ಆಗ (boc < eoc) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *eol = memchr(boc, '\n', eoc - boc);
		ಬಾಲ = ಸರತಿ_command(ಬಾಲ, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

ಸ್ಥಿರ ರಚನೆ command *read_ತಲೆ_info(ರಚನೆ packet_reader *reader,
				      ರಚನೆ oid_array *shallow,
				      ಎಣಿಕೆ report_ಸ್ಥಿತಿ_ಆವೃತ್ತಿ *ಆವೃತ್ತಿ)
{
	ರಚನೆ command *commands = ಖಾಲಿ;
	ರಚನೆ command **p = &commands;
	ಗಾಗಿ (;;) {
		ಪೂರ್ಣಾಂಕ linelen;

		ಆದರೆ (packet_reader_read(reader) != PACKET_READ_ಸಾಮಾನ್ಯ)
			ನಿಲ್ಲಿಸು;

		ಆದರೆ (reader->pktlen > 8 && starts_with(reader->ಸಾಲು, "shallow ")) {
			ರಚನೆ object_id oid;
			ಆದರೆ (get_oid_hex(reader->ಸಾಲು + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->ಸಾಲು + 8);
			oid_array_append(shallow, &oid);
			ಮುಂದುವರಿ;
		}

		linelen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(reader->ಸಾಲು);
		ಆದರೆ (linelen < reader->pktlen) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *feature_list = reader->ಸಾಲು + linelen + 1;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hash = ಖಾಲಿ;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *client_sid;
			ಗಾತ್ರ_t len = 0;
			ಆದರೆ (parse_feature_request(feature_list, "report-status"))
				*ಆವೃತ್ತಿ = REPORT_ಸ್ಥಿತಿ_V0;
			ಆದರೆ (parse_feature_request(feature_list, "report-status-v2"))
				*ಆವೃತ್ತಿ = REPORT_ಸ್ಥಿತಿ_V2;
			ಆದರೆ (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			ಆದರೆ (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			ಆದರೆ (advertise_atomic_ತಳ್ಳು
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			ಆದರೆ (advertise_ತಳ್ಳು_options
			    && parse_feature_request(feature_list, "push-options"))
				use_ತಳ್ಳು_options = 1;
			hash = parse_feature_ಮೌಲ್ಯ(feature_list, "object-format", &len, ಖಾಲಿ);
			ಆದರೆ (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(hash);
			}
			ಆದರೆ (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_ಮೌಲ್ಯ(feature_list, "session-id", &len, ಖಾಲಿ);
			ಆದರೆ (client_sid) {
				ಅಕ್ಷರ *sid = xstrndup(client_sid, len);
				trace2_ದತ್ತಾಂಶ_string("transfer", ಖಾಲಿ, "client-sid", client_sid);
				ಬಿಡುಗಡೆ(sid);
			}
		}

		ಆದರೆ (!strcmp(reader->ಸಾಲು, "push-cert")) {
			ಪೂರ್ಣಾಂಕ true_flush = 0;
			ಪೂರ್ಣಾಂಕ saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			ಗಾಗಿ (;;) {
				packet_reader_read(reader);
				ಆದರೆ (reader->ಸ್ಥಿತಿ_2 == PACKET_READ_FLUSH) {
					true_flush = 1;
					ನಿಲ್ಲಿಸು;
				}
				ಆದರೆ (reader->ಸ್ಥಿತಿ_2 != PACKET_READ_ಸಾಮಾನ್ಯ) {
					die("protocol error: got an unexpected packet");
				}
				ಆದರೆ (!strcmp(reader->ಸಾಲು, "push-cert-end\n"))
					ನಿಲ್ಲಿಸು; /* end of cert */
				strbuf_addstr(&ತಳ್ಳು_cert, reader->ಸಾಲು);
			}
			reader->options = saved_options;

			ಆದರೆ (true_flush)
				ನಿಲ್ಲಿಸು;
			ಮುಂದುವರಿ;
		}

		p = ಸರತಿ_command(p, reader->ಸಾಲು, linelen);
	}

	ಆದರೆ (ತಳ್ಳು_cert.len)
		ಸರತಿ_commands_from_cert(p, &ತಳ್ಳು_cert);

	ಹಿಂತಿರುಗಿ commands;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_ತಳ್ಳು_options(ರಚನೆ packet_reader *reader,
			      ರಚನೆ string_list *options)
{
	ಆಗ (1) {
		ಆದರೆ (packet_reader_read(reader) != PACKET_READ_ಸಾಮಾನ್ಯ)
			ನಿಲ್ಲಿಸು;

		string_list_append(options, reader->ಸಾಲು);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ unpack_with_sideband(ರಚನೆ odb_transaction *transaction,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *shallow_file,
				ರಚನೆ strbuf *err_ಸಂದೇಶ)
{
	ರಚನೆ odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_ಸಂದೇಶ_types = fsck_ಸಂದೇಶ_types.buf,
		.max_ಒಳಹರಿವು_ಗಾತ್ರ = max_ಒಳಹರಿವು_ಗಾತ್ರ,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	ರಚನೆ async muxer;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (!use_sideband)
		ಹಿಂತಿರುಗಿ odb_transaction_write_pack(transaction, 0, err_ಸಂದೇಶ, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, ಗಾತ್ರ(muxer));
	muxer.proc = copy_ಗಮ್ಯ_sideband;
	muxer.ಒಳ = -1;
	ಆದರೆ (ಆರಂಭ_async(&muxer))
		ಹಿಂತಿರುಗಿ 0;

	opts.err_fd = muxer.ಒಳ;
	ret = odb_transaction_write_pack(transaction, 0, err_ಸಂದೇಶ, &opts);

	ಮುಕ್ತಾಯ_async(&muxer);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_shallow_update(ರಚನೆ shallow_info *si,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *shallow_file)
{
	ಪೂರ್ಣಾಂಕ i, j, k, bitmap_ಗಾತ್ರ = DIV_ಸುತ್ತು_ಮೇಲೆ(si->ref->nr, 32);

	ALLOC_ARRAY(si->ಬಳಸಿದ_shallow, si->shallow->nr);
	ನಿಯೋಜಿಸು_shallow_commits_ಗಮ್ಯ_refs(si, si->ಬಳಸಿದ_shallow, ಖಾಲಿ);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	ಗಾಗಿ (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	ಗಾಗಿ (i = 0; i < si->shallow->nr; i++) {
		ಆದರೆ (!si->ಬಳಸಿದ_shallow[i])
			ಮುಂದುವರಿ;
		ಗಾಗಿ (j = 0; j < bitmap_ಗಾತ್ರ; j++) {
			ಆದರೆ (!si->ಬಳಸಿದ_shallow[i][j])
				ಮುಂದುವರಿ;
			si->need_reachability_test[i]++;
			ಗಾಗಿ (k = 0; k < 32; k++)
				ಆದರೆ (si->ಬಳಸಿದ_shallow[i][j] & (1U << k))
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

ಸ್ಥಿರ ಶೂನ್ಯ update_shallow_info(ರಚನೆ command *commands,
				ರಚನೆ shallow_info *si,
				ರಚನೆ oid_array *ref,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *shallow_file)
{
	ರಚನೆ command *cmd;
	ಪೂರ್ಣಾಂಕ *ref_ಸ್ಥಿತಿ;
	remove_nonexistent_theirs_shallow(si);
	ಆದರೆ (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		ಹಿಂತಿರುಗಿ;
	}

	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		ಆದರೆ (is_null_oid(&cmd->new_oid))
			ಮುಂದುವರಿ;
		oid_array_append(ref, &cmd->new_oid);
		cmd->ಸೂಚ್ಯಂಕ = ref->nr - 1;
	}
	si->ref = ref;

	ಆದರೆ (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		ಹಿಂತಿರುಗಿ;
	}

	ALLOC_ARRAY(ref_ಸ್ಥಿತಿ, ref->nr);
	ನಿಯೋಜಿಸು_shallow_commits_ಗಮ್ಯ_refs(si, ಖಾಲಿ, ref_ಸ್ಥಿತಿ);
	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		ಆದರೆ (is_null_oid(&cmd->new_oid))
			ಮುಂದುವರಿ;
		ಆದರೆ (ref_ಸ್ಥಿತಿ[cmd->ಸೂಚ್ಯಂಕ]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	ಬಿಡುಗಡೆ(ref_ಸ್ಥಿತಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ override_cmds_error(ರಚನೆ command *commands, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *err)
{
	ಗಾಗಿ (ರಚನೆ command *cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
ಸ್ಥಿರ ಶೂನ್ಯ generate_report(ರಚನೆ strbuf *buf, ರಚನೆ command *commands,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ strbuf *unpack_ಸ್ಥಿತಿ,
			    ಎಣಿಕೆ report_ಸ್ಥಿತಿ_ಆವೃತ್ತಿ ಆವೃತ್ತಿ)
{
	ರಚನೆ command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_ಸ್ಥಿತಿ->len ? unpack_ಸ್ಥಿತಿ->buf : "ok");

	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		ರಚನೆ ref_ತಳ್ಳು_report *report;
		ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;

		ಆದರೆ (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		ಇಲ್ಲದಿದ್ದರೆ
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		ಆದರೆ (ಆವೃತ್ತಿ != REPORT_ಸ್ಥಿತಿ_V2 || cmd->error_string)
			ಮುಂದುವರಿ;

		ಗಾಗಿ (report = cmd->report; report; report = report->ಮುಂದಿನ) {
			ಆದರೆ (ಎಣಿಕೆ_2++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			ಆದರೆ (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			ಆದರೆ (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_ಗಮ್ಯ_hex(report->old_oid));
			ಆದರೆ (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_ಗಮ್ಯ_hex(report->new_oid));
			ಆದರೆ (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ report(ರಚನೆ command *commands, ಸ್ಥಿರಾಂಕ ರಚನೆ strbuf *unpack_ಸ್ಥಿತಿ,
		   ಎಣಿಕೆ report_ಸ್ಥಿತಿ_ಆವೃತ್ತಿ ಆವೃತ್ತಿ)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_ಸ್ಥಿತಿ, ಆವೃತ್ತಿ);

	ಆದರೆ (ಸತತ_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_ಸ್ಥಿತಿ, ಆವೃತ್ತಿ);
	}

	ಆದರೆ (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	ಇಲ್ಲದಿದ್ದರೆ
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ delete_only(ರಚನೆ command *commands)
{
	ರಚನೆ command *cmd;
	ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ) {
		ಆದರೆ (!is_null_oid(&cmd->new_oid))
			ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ 1;
}

ಪೂರ್ಣಾಂಕ cmd_receive_pack(ಪೂರ್ಣಾಂಕ argc,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ advertise_refs = 0;
	ರಚನೆ command *commands;
	ರಚನೆ oid_array shallow = OID_ARRAY_INIT;
	ರಚನೆ oid_array ref = OID_ARRAY_INIT;
	ರಚನೆ shallow_info si;
	ರಚನೆ packet_reader reader;
	ರಚನೆ odb_transaction *transaction = ಖಾಲಿ;
	ಎಣಿಕೆ report_ಸ್ಥಿತಿ_ಆವೃತ್ತಿ ಆವೃತ್ತಿ = REPORT_ಸ್ಥಿತಿ_UNKNOWN;

	ರಚನೆ option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, ಖಾಲಿ),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, ಖಾಲಿ),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, ಖಾಲಿ),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, ಖಾಲಿ),
		OPT_ಕೊನೆ()
	};

	packet_trace_ತಟಸ್ಥ("receive-pack");

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, receive_pack_usage, 0);

	ಆದರೆ (argc > 1)
		usage_ಸಂದೇಶ_opt(_("too many arguments"), receive_pack_usage, options);
	ಆದರೆ (argc == 0)
		usage_ಸಂದೇಶ_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	ಸಿದ್ಧತೆ_ಮಾರ್ಗ();

	ಆದರೆ (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, ಖಾಲಿ);
	ಆದರೆ (cert_nonce_ಬೀಜ)
		ತಳ್ಳು_cert_nonce = prepare_ತಳ್ಳು_cert_nonce(service_dir, ಸಮಯ(ಖಾಲಿ));

	ಆಯ್ಕೆ (determine_protocol_ಆವೃತ್ತಿ_server()) {
	ಸ್ಥಿತಿ protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ಆದರೆ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	ಸ್ಥಿತಿ protocol_v0:
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ protocol_unknown_ಆವೃತ್ತಿ:
		BUG("unknown protocol version");
	}

	ಆದರೆ (advertise_refs || !stateless_rpc) {
		write_ತಲೆ_info();
	}
	ಆದರೆ (advertise_refs)
		ಹಿಂತಿರುಗಿ 0;

	packet_reader_init(&reader, 0, ಖಾಲಿ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ಆದರೆ ((commands = read_ತಲೆ_info(&reader, &shallow, &ಆವೃತ್ತಿ))) {
		ರಚನೆ string_list ತಳ್ಳು_options = STRING_LIST_INIT_DUP;
		ರಚನೆ strbuf unpack_ಸ್ಥಿತಿ = STRBUF_INIT;

		ಆದರೆ (use_ತಳ್ಳು_options)
			read_ತಳ್ಳು_options(&reader, &ತಳ್ಳು_options);
		ಆದರೆ (!check_cert_ತಳ್ಳು_options(&ತಳ್ಳು_options)) {
			ರಚನೆ command *cmd;
			ಗಾಗಿ (cmd = commands; cmd; cmd = cmd->ಮುಂದಿನ)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		ಆದರೆ (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		ಆದರೆ (!delete_only(commands)) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *alt_shallow_file = ಖಾಲಿ;

			ಆದರೆ (si.nr_ours || si.nr_theirs)
				alt_shallow_file = ಸಿದ್ಧತೆ_temporary_shallow(si.shallow);

			ಆದರೆ (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_ಸ್ಥಿತಿ, "unable to start object transaction");
			ಇಲ್ಲದಿದ್ದರೆ
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_ಸ್ಥಿತಿ);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_ಸ್ಥಿತಿ.len, &si, transaction,
				 &ತಳ್ಳು_options);
		odb_transaction_finalize(transaction);
		sigchain_ತಳ್ಳು(SIGPIPE, SIG_IGN);

		ಆಯ್ಕೆ (ಆವೃತ್ತಿ) {
		ಸ್ಥಿತಿ REPORT_ಸ್ಥಿತಿ_V2:
		ಸ್ಥಿತಿ REPORT_ಸ್ಥಿತಿ_V0:
			report(commands, &unpack_ಸ್ಥಿತಿ, ಆವೃತ್ತಿ);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ REPORT_ಸ್ಥಿತಿ_UNKNOWN:
			ನಿಲ್ಲಿಸು;
		}

		sigchain_ತೆಗೆ(SIGPIPE);
		ಸತತ_receive_hook(commands, "post-receive", 1, ಖಾಲಿ,
				 &ತಳ್ಳು_options);
		ಸತತ_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&ತಳ್ಳು_options, 0);
		ಆದರೆ (auto_gc) {
			ರಚನೆ ಮಗು_process proc = ಮಗು_PROCESS_INIT;

			ಆದರೆ (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_ಗಮ್ಯ_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				ಆದರೆ (!ಆರಂಭ_command(&proc)) {
					ಆದರೆ (use_sideband)
						copy_ಗಮ್ಯ_sideband(proc.err, -1, ಖಾಲಿ);
					ಮುಕ್ತಾಯ_command(&proc);
				}
			}
		}
		ಆದರೆ (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_ಸ್ಥಿತಿ);
	}
	ಆದರೆ (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	ಬಿಡುಗಡೆ((ಶೂನ್ಯ *)ತಳ್ಳು_cert_nonce);
	ಹಿಂತಿರುಗಿ 0;
}
