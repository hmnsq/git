#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "connect.h"
#ଅନ୍ତର୍ଭାଵ "connected.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "exec-cmd.h"
#ଅନ୍ତର୍ଭାଵ "fsck.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "gpg-interface.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "hook.h"
#ଅନ୍ତର୍ଭାଵ "object.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "oid-array.h"
#ଅନ୍ତର୍ଭାଵ "oidset.h"
#ଅନ୍ତର୍ଭାଵ "pack.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "pkt-line.h"
#ଅନ୍ତର୍ଭାଵ "protocol.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "server-info.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "shallow.h"
#ଅନ୍ତର୍ଭାଵ "sideband.h"
#ଅନ୍ତର୍ଭାଵ "sigchain.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "trace.h"
#ଅନ୍ତର୍ଭାଵ "trace2.h"
#ଅନ୍ତର୍ଭାଵ "version.h"
#ଅନ୍ତର୍ଭାଵ "worktree.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	ଖାଲି
};

ଗଣନା deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

ଗଣନା report_status_version {
	REPORT_STATUS_UNKNOWN = 0,
	REPORT_STATUS_V0,
	REPORT_STATUS_V2,
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା deny_deletes;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା deny_non_fast_forwards;
ସ୍ଥିର ଗଣନା deny_action deny_current_branch = DENY_UNCONFIGURED;
ସ୍ଥିର ଗଣନା deny_action deny_delete_current = DENY_UNCONFIGURED;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା receive_fsck_objects = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା transfer_fsck_objects = -1;
ସ୍ଥିର ସଂରଚନା strbuf fsck_ସନ୍ଦେଶ_types = STRBUF_INIT;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା advertise_atomic_ଠେଲ = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା advertise_ଠେଲ_options;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା advertise_sid;
ସ୍ଥିର off_t max_ନିବେଶ_size;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_sideband;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_atomic;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_ଠେଲ_options;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prefer_ofs_delta = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା auto_update_server_info;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା auto_gc = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reject_thin;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip_connectivity_check;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stateless_rpc;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *service_dir;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *head_name;
ସ୍ଥିର ଶୂନ୍ୟ *head_name_to_free;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sent_capabilities;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା shallow_update;
ସ୍ଥିର ସଂରଚନା strbuf ଠେଲ_cert = STRBUF_INIT;
ସ୍ଥିର ସଂରଚନା object_id ଠେଲ_cert_oid;
ସ୍ଥିର ସଂରଚନା signature_check sigcheck;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଠେଲ_cert_nonce;
ସ୍ଥିର ଅକ୍ଷର *cert_nonce_seed;
ସ୍ଥିର ସଂରଚନା strvec hidden_refs = STRVEC_INIT;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *NONCE_UNSOLICITED = "UNSOLICITED";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *NONCE_BAD = "BAD";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *NONCE_MISSING = "MISSING";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *NONCE_OK = "OK";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *NONCE_SLOP = "SLOP";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *nonce_status;
ସ୍ଥିର long nonce_stamp_slop;
ସ୍ଥିର timestamp_t nonce_stamp_slop_limit;
ସ୍ଥିର ସଂରଚନା ref_transaction *transaction;

ସ୍ଥିର ଗଣନା {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା keepalive_in_sec = 5;

ସ୍ଥିର ସଂରଚନା proc_receive_ref {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	ଅକ୍ଷର *ref_prefix;
	ସଂରଚନା proc_receive_ref *next;
} *proc_receive_ref;

ସ୍ଥିର ଶୂନ୍ୟ proc_receive_ref_append(ଅଚଳ ଅକ୍ଷର *prefix);

ସ୍ଥିର ଗଣନା deny_action parse_deny_action(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ଯଦି (ମୂଲ୍ୟ) {
		ଯଦି (!strcasecmp(ମୂଲ୍ୟ, "ignore"))
			ଫେରନ୍ତୁ DENY_IGNORE;
		ଯଦି (!strcasecmp(ମୂଲ୍ୟ, "warn"))
			ଫେରନ୍ତୁ DENY_WARN;
		ଯଦି (!strcasecmp(ମୂଲ୍ୟ, "refuse"))
			ଫେରନ୍ତୁ DENY_REFUSE;
		ଯଦି (!strcasecmp(ମୂଲ୍ୟ, "updateinstead"))
			ଫେରନ୍ତୁ DENY_UPDATE_INSTEAD;
	}
	ଯଦି (git_config_bool(var, ମୂଲ୍ୟ))
		ଫେରନ୍ତୁ DENY_REFUSE;
	ଫେରନ୍ତୁ DENY_IGNORE;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା receive_pack_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			       ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ_id;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = parse_hide_refs_config(var, ମୂଲ୍ୟ, "receive", &hidden_refs);

	ଯଦି (status)
		ଫେରନ୍ତୁ status;

	ଯଦି (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.fsck.skiplist") == 0) {
		ଅକ୍ଷର *path;

		ଯଦି (git_config_pathname(&path, var, ମୂଲ୍ୟ))
			ଫେରନ୍ତୁ -1;
		ଯଦି (path)
			strbuf_addf(&fsck_ସନ୍ଦେଶ_types, "%cskiplist=%s",
				    fsck_ସନ୍ଦେଶ_types.len ? ',' : '=', path);
		ମୁକ୍ତ(path);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (skip_prefix(var, "receive.fsck.", &ସନ୍ଦେଶ_id)) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		ଯଦି (is_valid_ସନ୍ଦେଶ_type(ସନ୍ଦେଶ_id, ମୂଲ୍ୟ))
			strbuf_addf(&fsck_ସନ୍ଦେଶ_types, "%c%s=%s",
				fsck_ସନ୍ଦେଶ_types.len ? ',' : '=', ସନ୍ଦେଶ_id, ମୂଲ୍ୟ);
		ଅନ୍ୟଥା
			warning("skipping unknown msg id '%s'", ସନ୍ଦେଶ_id);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.certnonceseed") == 0)
		ଫେରନ୍ତୁ git_config_string(&cert_nonce_seed, var, ମୂଲ୍ୟ);

	ଯଦି (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, ମୂଲ୍ୟ, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_ଠେଲ = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_ଠେଲ_options = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, ମୂଲ୍ୟ, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.maxinputsize") == 0) {
		max_ନିବେଶ_size = git_config_int64(var, ମୂଲ୍ୟ, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "receive.procreceiverefs") == 0) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		proc_receive_ref_append(ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ସ୍ଥିର ଶୂନ୍ୟ show_ref(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ସଂରଚନା object_id *oid)
{
	ଯଦି (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} ଅନ୍ୟଥା {
		ସଂରଚନା strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		ଯଦି (advertise_atomic_ଠେଲ)
			strbuf_addstr(&cap, " atomic");
		ଯଦି (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		ଯଦି (ଠେଲ_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", ଠେଲ_cert_nonce);
		ଯଦି (advertise_ଠେଲ_options)
			strbuf_addstr(&cap, " push-options");
		ଯଦି (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ref_cb(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *data)
{
	ସଂରଚନା oidset *seen = data;
	ଅଚଳ ଅକ୍ଷର *path = strip_namespace(ref->name);

	ଯଦି (ref_is_hidden(path, ref->name, &hidden_refs))
		ଫେରନ୍ତୁ 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	ଯଦି (!path) {
		ଯଦି (oidset_ଭର୍ତ୍ତି(seen, ref->oid))
			ଫେରନ୍ତୁ 0;
		path = ".have";
	} ଅନ୍ୟଥା {
		oidset_ଭର୍ତ୍ତି(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ show_one_alternate_ref(ଅଚଳ ସଂରଚନା object_id *oid,
				   ଶୂନ୍ୟ *data)
{
	ସଂରଚନା oidset *seen = data;

	ଯଦି (oidset_ଭର୍ତ୍ତି(seen, oid))
		ଫେରନ୍ତୁ;

	show_ref(".have", oid);
}

ସ୍ଥିର ଶୂନ୍ୟ write_head_info(ଶୂନ୍ୟ)
{
	ସଂରଚନା refs_for_each_ref_options opts = { 0 };
	ସ୍ଥିର ସଂରଚନା oidset seen = OIDSET_INIT;
	ସଂରଚନା strvec excludes_vector = STRVEC_INIT;

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

	ଯଦି (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#define RUN_PROC_RECEIVE_SCHEDULED	1
#define RUN_PROC_RECEIVE_RETURNED	2
ସଂରଚନା command {
	ସଂରଚନା command *next;
	ଅଚଳ ଅକ୍ଷର *error_string;
	ଅକ୍ଷର *error_string_owned;
	ସଂରଚନା ref_ଠେଲ_report *report;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା index;
	ସଂରଚନା object_id old_oid;
	ସଂରଚନା object_id new_oid;
	ଅକ୍ଷର ref_name[FLEX_ARRAY]; /* more */
};

ସ୍ଥିର ଶୂନ୍ୟ proc_receive_ref_append(ଅଚଳ ଅକ୍ଷର *prefix)
{
	ସଂରଚନା proc_receive_ref *ref_pattern;
	ଅଚଳ ଅକ୍ଷର *p;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = strchr(prefix, ':');
	ଯଦି (p) {
		ଯେତେବେଳେ (prefix < p) {
			ଯଦି (*prefix == 'a')
				ref_pattern->want_add = 1;
			ଅନ୍ୟଥା ଯଦି (*prefix == 'd')
				ref_pattern->want_delete = 1;
			ଅନ୍ୟଥା ଯଦି (*prefix == 'm')
				ref_pattern->want_modify = 1;
			ଅନ୍ୟଥା ଯଦି (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} ଅନ୍ୟଥା {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	ଯେତେବେଳେ (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	ଯଦି (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} ଅନ୍ୟଥା {
		ସଂରଚନା proc_receive_ref *end;

		end = proc_receive_ref;
		ଯେତେବେଳେ (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା proc_receive_ref_matches(ସଂରଚନା command *cmd)
{
	ସଂରଚନା proc_receive_ref *p;

	ଯଦି (!proc_receive_ref)
		ଫେରନ୍ତୁ 0;

	ପାଇଁ (p = proc_receive_ref; p; p = p->next) {
		ଅଚଳ ଅକ୍ଷର *match = p->ref_prefix;
		ଅଚଳ ଅକ୍ଷର *remains;

		ଯଦି (!p->want_add && is_null_oid(&cmd->old_oid))
			ଜାରି;
		ଅନ୍ୟଥା ଯଦି (!p->want_delete && is_null_oid(&cmd->new_oid))
			ଜାରି;
		ଅନ୍ୟଥା ଯଦି (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			ଜାରି;

		ଯଦି (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			ଯଦି (!p->negative_ref)
				ଫେରନ୍ତୁ 1;
		} ଅନ୍ୟଥା ଯଦି (p->negative_ref) {
			ଫେରନ୍ତୁ 1;
		}
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ report_message(ଅଚଳ ଅକ୍ଷର *prefix, ଅଚଳ ଅକ୍ଷର *err, va_list params)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz;
	ଅକ୍ଷର ସନ୍ଦେଶ[4096];

	sz = xsnprintf(ସନ୍ଦେଶ, ଆକାର(ସନ୍ଦେଶ), "%s", prefix);
	sz += vsnprintf(ସନ୍ଦେଶ + sz, ଆକାର(ସନ୍ଦେଶ) - sz, err, params);
	ଯଦି (sz > (ଆକାର(ସନ୍ଦେଶ) - 1))
		sz = ଆକାର(ସନ୍ଦେଶ) - 1;
	ସନ୍ଦେଶ[sz++] = '\n';

	ଯଦି (use_sideband)
		send_sideband(1, 2, ସନ୍ଦେଶ, sz, use_sideband);
	ଅନ୍ୟଥା
		xwrite(2, ସନ୍ଦେଶ, sz);
}

__attribute__((format (printf, 1, 2)))
ସ୍ଥିର ଶୂନ୍ୟ rp_warning(ଅଚଳ ଅକ୍ଷର *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("warning: ", err, params);
	va_end(params);
}

__attribute__((format (printf, 1, 2)))
ସ୍ଥିର ଶୂନ୍ୟ rp_error(ଅଚଳ ଅକ୍ଷର *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("error: ", err, params);
	va_end(params);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା copy_to_sideband(ପୂର୍ଣ୍ଣସଂଖ୍ୟା in, ପୂର୍ଣ୍ଣସଂଖ୍ୟା out UNUSED, ଶୂନ୍ୟ *arg UNUSED)
{
	ଅକ୍ଷର data[128];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା keepalive_active = 0;

	ଯଦି (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	ଯଦି (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	ଯେତେବେଳେ (1) {
		ssize_t sz;

		ଯଦି (keepalive_active) {
			ସଂରଚନା pollfd pfd;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			ଯଦି (ret < 0) {
				ଯଦି (errno == EINTR)
					ଜାରି;
				ଅନ୍ୟଥା
					ଭାଙ୍ଗ;
			} ଅନ୍ୟଥା ଯଦି (ret == 0) {
				/* no data; send a keepalive packet */
				ସ୍ଥିର ଅଚଳ ଅକ୍ଷର buf[] = "0005\1";
				write_or_die(1, buf, ଆକାର(buf) - 1);
				ଜାରି;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, ଆକାର(data));
		ଯଦି (sz <= 0)
			ଭାଙ୍ଗ;

		ଯଦି (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			ଅଚଳ ଅକ୍ଷର *p = memchr(data, '\0', sz);
			ଯଦି (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_active = 1;
				send_sideband(1, 2, data, p - data, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - data + 1), use_sideband);
				ଜାରି;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, data, sz, use_sideband);
	}
	close(in);
	ଫେରନ୍ତୁ 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
ସ୍ଥିର ଶୂନ୍ୟ prepare_sideband_async(ସଂରଚନା async *sideband_async, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *saved_stderr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *started)
{
	*started = 0;

	ଯଦି (!use_sideband)
		ଫେରନ୍ତୁ;

	memset(sideband_async, 0, ଆକାର(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	ଯଦି (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		ଯଦି (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
ସ୍ଥିର ଶୂନ୍ୟ finish_sideband_async(ସଂରଚନା async *sideband_async, ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_stderr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା started)
{
	ଯଦି (!use_sideband)
		ଫେରନ୍ତୁ;

	ଯଦି (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	ଯଦି (started)
		finish_async(sideband_async);
}

ସ୍ଥିର ଶୂନ୍ୟ hmac_hash(unsigned ଅକ୍ଷର *out,
		      ଅଚଳ ଅକ୍ଷର *ଚାବି_in, size_t ଚାବି_len,
		      ଅଚଳ ଅକ୍ଷର *text, size_t text_len)
{
	unsigned ଅକ୍ଷର ଚାବି[GIT_MAX_BLKSZ];
	unsigned ଅକ୍ଷର k_ipad[GIT_MAX_BLKSZ];
	unsigned ଅକ୍ଷର k_opad[GIT_MAX_BLKSZ];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(ଚାବି, '\0', GIT_MAX_BLKSZ);
	ଯଦି (the_hash_algo->blksz < ଚାବି_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, ଚାବି_in, ଚାବି_len);
		git_hash_final(ଚାବି, &ctx);
	} ଅନ୍ୟଥା {
		memcpy(ଚାବି, ଚାବି_in, ଚାବି_len);
	}

	/* RFC 2104 2. (2) & (5) */
	ପାଇଁ (i = 0; i < ଆକାର(ଚାବି); i++) {
		k_ipad[i] = ଚାବି[i] ^ 0x36;
		k_opad[i] = ଚାବି[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, ଆକାର(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, ଆକାର(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

ସ୍ଥିର ଅକ୍ଷର *prepare_ଠେଲ_cert_nonce(ଅଚଳ ଅକ୍ଷର *path, timestamp_t stamp)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	unsigned ଅକ୍ଷର hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, strlen(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)the_hash_algo->hexsz, hash_to_hex(hash));
	ଫେରନ୍ତୁ strbuf_detach(&buf, ଖାଲି);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା constant_memequal(ଅଚଳ ଅକ୍ଷର *a, ଅଚଳ ଅକ୍ଷର *b, size_t n)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
	size_t i;

	ପାଇଁ (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *check_nonce(ଅଚଳ ଅକ୍ଷର *buf)
{
	size_t noncelen;
	ଅଚଳ ଅକ୍ଷର *found = find_commit_header(buf, "nonce", &noncelen);
	ଅକ୍ଷର *nonce = found ? xmemdupz(found, noncelen) : ଖାଲି;
	timestamp_t stamp, ostamp;
	ଅକ୍ଷର *bohmac, *expect = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *retval = NONCE_BAD;

	ଯଦି (!nonce) {
		retval = NONCE_MISSING;
		ଯାଅ leave;
	} ଅନ୍ୟଥା ଯଦି (!ଠେଲ_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		ଯାଅ leave;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(ଠେଲ_cert_nonce, nonce)) {
		retval = NONCE_OK;
		ଯାଅ leave;
	}

	ଯଦି (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		ଯାଅ leave;
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
	ଯଦି (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		ଯାଅ leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	ଯଦି (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		ଯାଅ leave;
	}

	expect = prepare_ଠେଲ_cert_nonce(service_dir, stamp);
	ଯଦି (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		ଯାଅ leave;
	}
	ଯଦି (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		ଯାଅ leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(ଠେଲ_cert_nonce, ଖାଲି, 10);
	nonce_stamp_slop = (long)ostamp - (long)stamp;

	ଯଦି (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		ମୁକ୍ତ((ଶୂନ୍ୟ *)ଠେଲ_cert_nonce);
		ଠେଲ_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} ଅନ୍ୟଥା {
		retval = NONCE_SLOP;
	}

leave:
	ମୁକ୍ତ(nonce);
	ମୁକ୍ତ(expect);
	ଫେରନ୍ତୁ retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_cert_ଠେଲ_options(ଅଚଳ ସଂରଚନା string_list *ଠେଲ_options)
{
	ଅଚଳ ଅକ୍ଷର *buf = ଠେଲ_cert.buf;

	ଅଚଳ ଅକ୍ଷର *option;
	size_t optionlen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା options_seen = 0;

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval = 1;

	ଯଦି (!*buf)
		ଫେରନ୍ତୁ 1;

	ଯେତେବେଳେ ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		ଯଦି (options_seen > ଠେଲ_options->nr
		    || xstrncmpz(ଠେଲ_options->ବସ୍ତୁଗୁଡ଼ିକ[options_seen - 1].string,
				 option, optionlen))
			ଫେରନ୍ତୁ 0;
	}

	ଯଦି (options_seen != ଠେଲ_options->nr)
		retval = 0;

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ଶୂନ୍ୟ prepare_ଠେଲ_cert_sha1(ସଂରଚନା run_hooks_opt *opt)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା already_done;

	ଯଦି (!ଠେଲ_cert.len)
		ଫେରନ୍ତୁ;

	ଯଦି (!already_done) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		ଯଦି (odb_write_object(the_repository->objects, ଠେଲ_cert.buf,
				     ଠେଲ_cert.len, OBJ_BLOB, &ଠେଲ_cert_oid))
			oidclr(&ଠେଲ_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', ଆକାର(sigcheck));

		bogs = parse_signed_buffer(ଠେଲ_cert.buf, ଠେଲ_cert.len);
		sigcheck.payload = xmemdupz(ଠେଲ_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, ଠେଲ_cert.buf + bogs,
				ଠେଲ_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	ଯଦି (!is_null_oid(&ଠେଲ_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&ଠେଲ_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.ଚାବି ? sigcheck.ଚାବି : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		ଯଦି (ଠେଲ_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     ଠେଲ_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			ଯଦି (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

ସଂରଚନା receive_hook_feed_state {
	ସଂରଚନା command *cmd;
	ସଂରଚନା ref_ଠେଲ_report *report;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip_broken;
	ସଂରଚନା strbuf buf;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା feed_receive_hook_cb(ପୂର୍ଣ୍ଣସଂଖ୍ୟା hook_stdin_fd, ଶୂନ୍ୟ *pp_cb UNUSED, ଶୂନ୍ୟ *pp_task_cb)
{
	ସଂରଚନା receive_hook_feed_state *state = pp_task_cb;
	ସଂରଚନା command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	ଯେତେବେଳେ (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	ଯଦି (!cmd)
		ଫେରନ୍ତୁ 1;  /* no more commands left */

	ଯଦି (!state->report)
		state->report = cmd->report;

	ଯଦି (state->report) {
		ସଂରଚନା object_id *old_oid;
		ସଂରଚନା object_id *new_oid;
		ଅଚଳ ଅକ୍ଷର *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->next;
		ଯଦି (!state->report)
			cmd = cmd->next;
	} ଅନ୍ୟଥା {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	ଯଦି (state->buf.len > 0) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		ଯଦି (ret < 0) {
			ଯଦି (errno == EPIPE)
				ଫେରନ୍ତୁ 1; /* child closed pipe */
			ଫେରନ୍ତୁ ret;
		}
	}

	ଫେରନ୍ତୁ state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

ସ୍ଥିର ଶୂନ୍ୟ *receive_hook_feed_state_alloc(ଶୂନ୍ୟ *feed_pipe_ctx)
{
	ସଂରଚନା receive_hook_feed_state *init_state = feed_pipe_ctx;
	ସଂରଚନା receive_hook_feed_state *data;

	CALLOC_ARRAY(data, 1);
	data->report = init_state->report;
	data->cmd = init_state->cmd;
	data->skip_broken = init_state->skip_broken;
	strbuf_init(&data->buf, 0);

	ଫେରନ୍ତୁ data;
}

ସ୍ଥିର ଶୂନ୍ୟ receive_hook_feed_state_free(ଶୂନ୍ୟ *data)
{
	ସଂରଚନା receive_hook_feed_state *d = data;
	ଯଦି (!d)
		ଫେରନ୍ତୁ;
	strbuf_release(&d->buf);
	ମୁକ୍ତ(d);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_receive_hook(ସଂରଚନା command *commands,
			    ଅଚଳ ଅକ୍ଷର *hook_name,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip_broken,
			    ସଂରଚନା odb_transaction *transaction,
			    ଅଚଳ ସଂରଚନା string_list *ଠେଲ_options)
{
	ସଂରଚନା run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ସଂରଚନା command *iter = commands;
	ସଂରଚନା receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	ସଂରଚନା async sideband_async;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା sideband_async_started = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_stderr = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (!hook_exists(the_repository, hook_name))
		ଫେରନ୍ତୁ 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	ଯେତେବେଳେ (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	ଯଦି (!iter)
		ଫେରନ୍ତୁ 0;

	ଯଦି (ଠେଲ_options) {
		ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଠେଲ_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     ଠେଲ_options->ବସ୍ତୁଗୁଡ଼ିକ[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)ଠେଲ_options->nr);
	} ଅନ୍ୟଥା {
		strvec_ଠେଲ(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	ଯଦି (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_ଠେଲ_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_data_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_data_free = receive_hook_feed_state_free;

	ret = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_update_hook(ସଂରଚନା command *cmd)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର hook_name[] = "update";
	ସଂରଚନା run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ସଂରଚନା async sideband_async;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା sideband_async_started = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_stderr = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;

	ଯଦି (!hook_exists(the_repository, hook_name))
		ଫେରନ୍ତୁ 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     ଖାଲି);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ଫେରନ୍ତୁ code;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_receive_report_hook(ସଂରଚନା strbuf *report)
{
	ସଂରଚନା child_process proc = CHILD_PROCESS_INIT;
	ସଂରଚନା async sideband_async;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା sideband_async_started = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_stderr = -1;
	ସଂରଚନା strbuf out = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *hook_path;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	hook_path = find_hook(the_repository, "receive-report");
	ଯଦି (!hook_path)
		ଫେରନ୍ତୁ 0;

	strvec_ଠେଲ(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_ଠେଲ(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, ଖାଲି, 0);
	sigchain_କାଢ଼(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	ଯଦି (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ସଂରଚନା command *find_command_by_refname(ସଂରଚନା command *list,
					       ଅଚଳ ଅକ୍ଷର *refname)
{
	ପାଇଁ (; list; list = list->next)
		ଯଦି (!strcmp(list->ref_name, refname))
			ଫେରନ୍ତୁ list;
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_proc_receive_report(ସଂରଚନା packet_reader *reader,
				    ସଂରଚନା command *commands,
				    ସଂରଚନା strbuf *errmsg)
{
	ସଂରଚନା command *cmd;
	ସଂରଚନା command *hint = ଖାଲି;
	ସଂରଚନା ref_ଠେଲ_report *report = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_report = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା code = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା once = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା response = 0;

	ପାଇଁ (;;) {
		ସଂରଚନା object_id old_oid, new_oid;
		ଅକ୍ଷର *head;
		ଅକ୍ଷର *refname;
		ଅକ୍ଷର *p;
		ଗଣନା packet_read_status status;

		status = packet_reader_read(reader);
		ଯଦି (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			ଯଦି (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				ଫେରନ୍ତୁ -1;
			}
			ଭାଙ୍ଗ;
		}
		response++;

		head = reader->ଧାଡ଼ି;
		p = strchr(head, ' ');
		ଯଦି (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			ଜାରି;
		}
		*p++ = '\0';
		ଯଦି (!strcmp(head, "option")) {
			ଅକ୍ଷର *ଚାବି;
			ଅଚଳ ଅକ୍ଷର *val;

			ଯଦି (!hint || !(report || new_report)) {
				ଯଦି (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				ଜାରି;
			}
			ଯଦି (new_report) {
				ଯଦି (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} ଅନ୍ୟଥା {
					report = hint->report;
					ଯେତେବେଳେ (report->next)
						report = report->next;
					report->next = xcalloc(1, ଆକାର(ସଂରଚନା ref_ଠେଲ_report));
					report = report->next;
				}
				new_report = 0;
			}
			ଚାବି = p;
			p = strchr(ଚାବି, ' ');
			ଯଦି (p)
				*p++ = '\0';
			val = p;
			ଯଦି (!strcmp(ଚାବି, "refname"))
				report->ref_name = xstrdup_or_null(val);
			ଅନ୍ୟଥା ଯଦି (!strcmp(ଚାବି, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			ଅନ୍ୟଥା ଯଦି (!strcmp(ଚାବି, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			ଅନ୍ୟଥା ଯଦି (!strcmp(ଚାବି, "forced-update"))
				report->forced_update = 1;
			ଅନ୍ୟଥା ଯଦି (!strcmp(ଚାବି, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			ଜାରି;
		}

		report = ଖାଲି;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		ଯଦି (p)
			*p++ = '\0';
		ଯଦି (strcmp(head, "ok") && strcmp(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			ଜାରି;
		}

		/* first try searching at our hint, falling back to all refs */
		ଯଦି (hint)
			hint = find_command_by_refname(hint, refname);
		ଯଦି (!hint)
			hint = find_command_by_refname(commands, refname);
		ଯଦି (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			ଜାରି;
		}
		ଯଦି (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			ଜାରି;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		ଯଦି (!strcmp(head, "ng")) {
			ଯଦି (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			ଅନ୍ୟଥା
				hint->error_string = "failed";
			code = -1;
			ଜାରି;
		}
		new_report = 1;
	}

	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next)
		ଯଦି (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	ଫେରନ୍ତୁ code;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_proc_receive_hook(ସଂରଚନା command *commands,
				 ଅଚଳ ସଂରଚନା string_list *ଠେଲ_options)
{
	ସଂରଚନା child_process proc = CHILD_PROCESS_INIT;
	ସଂରଚନା async muxer;
	ସଂରଚନା command *cmd;
	ସଂରଚନା packet_reader reader;
	ସଂରଚନା strbuf cap = STRBUF_INIT;
	ସଂରଚନା strbuf errmsg = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hook_use_ଠେଲ_options = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା version = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;
	ଅଚଳ ଅକ୍ଷର *hook_path = find_hook(the_repository, "proc-receive");

	ଯଦି (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		ଫେରନ୍ତୁ -1;
	}

	strvec_ଠେଲ(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	ଯଦି (use_sideband) {
		memset(&muxer, 0, ଆକାର(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		ଯଦି (code)
			ଫେରନ୍ତୁ code;
		proc.err = muxer.in;
	} ଅନ୍ୟଥା {
		proc.err = 0;
	}

	code = start_command(&proc);
	ଯଦି (code) {
		ଯଦି (use_sideband)
			finish_async(&muxer);
		ଫେରନ୍ତୁ code;
	}

	sigchain_ଠେଲ(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, ଖାଲି, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	ଯଦି (use_atomic)
		strbuf_addstr(&cap, " atomic");
	ଯଦି (use_ଠେଲ_options)
		strbuf_addstr(&cap, " push-options");
	ଯଦି (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} ଅନ୍ୟଥା {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	ଯଦି (!code)
		code = packet_flush_gently(proc.in);

	ଯଦି (!code)
		ପାଇଁ (;;) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା linelen;
			ଗଣନା packet_read_status status;

			status = packet_reader_read(&reader);
			ଯଦି (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				ଯଦି (status == PACKET_READ_EOF)
					code = -1;
				ଭାଙ୍ଗ;
			}

			ଯଦି (reader.pktlen > 8 && starts_with(reader.ଧାଡ଼ି, "version=")) {
				version = atoi(reader.ଧାଡ଼ି + 8);
				linelen = strlen(reader.ଧାଡ଼ି);
				ଯଦି (linelen < reader.pktlen) {
					ଅଚଳ ଅକ୍ଷର *feature_list = reader.ଧାଡ଼ି + linelen + 1;
					ଯଦି (parse_feature_request(feature_list, "push-options"))
						hook_use_ଠେଲ_options = 1;
				}
			}
		}

	ଯଦି (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		ଯାଅ cleanup;
	}

	ବାଛ (version) {
	ସ୍ଥିତି 0:
		/* fallthrough */
	ସ୍ଥିତି 1:
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		ଯାଅ cleanup;
	}

	/* Send commands */
	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
		ଯଦି (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			ଜାରି;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		ଯଦି (code)
			ଭାଙ୍ଗ;
	}
	ଯଦି (!code)
		code = packet_flush_gently(proc.in);
	ଯଦି (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		ଯାଅ cleanup;
	}

	/* Send push options */
	ଯଦି (hook_use_ଠେଲ_options) {
		ସଂରଚନା string_list_item *item;

		for_each_string_list_item(item, ଠେଲ_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			ଯଦି (code)
				ଭାଙ୍ଗ;
		}
		ଯଦି (!code)
			code = packet_flush_gently(proc.in);
		ଯଦି (code) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			ଯାଅ cleanup;
		}
	}

	/* Read result from proc-receive */
	code = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.in);
	close(proc.out);
	ଯଦି (use_sideband)
		finish_async(&muxer);
	ଯଦି (finish_command(&proc))
		code = -1;
	ଯଦି (errmsg.len >0) {
		ଅକ୍ଷର *p = errmsg.buf;

		p += errmsg.len - 1;
		ଯଦି (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_କାଢ଼(SIGPIPE);

	ଫେରନ୍ତୁ code;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *refuse_unconfigured_deny_ସନ୍ଦେଶ =
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

ସ୍ଥିର ଶୂନ୍ୟ refuse_unconfigured_deny(ଶୂନ୍ୟ)
{
	rp_error("%s", _(refuse_unconfigured_deny_ସନ୍ଦେଶ));
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *refuse_unconfigured_deny_delete_current_ସନ୍ଦେଶ =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

ସ୍ଥିର ଶୂନ୍ୟ refuse_unconfigured_deny_delete_current(ଶୂନ୍ୟ)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_ସନ୍ଦେଶ));
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା object_id *command_singleton_iterator(ଶୂନ୍ୟ *cb_data);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_shallow_ref(ସଂରଚନା command *cmd, ସଂରଚନା shallow_info *si)
{
	ସଂରଚନା shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	ସଂରଚନା oid_array extra = OID_ARRAY_INIT;
	ସଂରଚନା check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	trace_printf_ଚାବି(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	ପାଇଁ (i = 0; i < si->shallow->nr; i++)
		ଯଦି (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	ଯଦି (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		ଫେରନ୍ତୁ -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	ପାଇଁ (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଠେଲ_to_deploy(unsigned ଅକ୍ଷର *sha1,
				  ସଂରଚନା strvec *env,
				  ଅଚଳ ସଂରଚନା worktree *worktree)
{
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", ଖାଲି);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	ଯଦି (run_command(&child))
		ଫେରନ୍ତୁ "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", ଖାଲି);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	ଯଦି (run_command(&child))
		ଫେରନ୍ତୁ "Working directory has unstaged changes";

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
		     empty_ବୃକ୍ଷ_oid_hex(the_repository->hash_algo), "--", ଖାଲି);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	ଯଦି (run_command(&child))
		ଫେରନ୍ତୁ "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     ଖାଲି);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	ଯଦି (run_command(&child))
		ଫେରନ୍ତୁ "Could not update working tree to new HEAD";

	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଠେଲ_to_checkout_hook = "push-to-checkout";

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଠେଲ_to_checkout(unsigned ଅକ୍ଷର *hash,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *invoked_hook,
				    ସଂରଚନା strvec *env,
				    ଅଚଳ ଅକ୍ଷର *work_ବୃକ୍ଷ)
{
	ସଂରଚନା run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_ବୃକ୍ଷ));
	strvec_ଠେଲ(&opt.args, hash_to_hex(hash));
	ଯଦି (run_hooks_opt(the_repository, ଠେଲ_to_checkout_hook, &opt))
		ଫେରନ୍ତୁ "push-to-checkout hook declined";
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *update_worktree(unsigned ଅକ୍ଷର *sha1, ଅଚଳ ସଂରଚନା worktree *worktree)
{
	ଅଚଳ ଅକ୍ଷର *retval;
	ଅକ୍ଷର *git_dir;
	ସଂରଚନା strvec env = STRVEC_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା invoked_hook;

	ଯଦି (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	ଯଦି (worktree->is_bare)
		ଫେରନ୍ତୁ "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = ଠେଲ_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	ଯଦି (!invoked_hook)
		retval = ଠେଲ_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	ମୁକ୍ତ(git_dir);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *update(ସଂରଚନା command *cmd, ସଂରଚନା shallow_info *si)
{
	ଅଚଳ ଅକ୍ଷର *name = cmd->ref_name;
	ସଂରଚନା strbuf namespaced_name_buf = STRBUF_INIT;
	ସ୍ଥିର ଅକ୍ଷର *namespaced_name;
	ଅଚଳ ଅକ୍ଷର *ret;
	ସଂରଚନା object_id *old_oid = &cmd->old_oid;
	ସଂରଚନା object_id *new_oid = &cmd->new_oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_update_worktree = 0;
	ସଂରଚନା worktree **worktrees = get_worktrees(the_repository);
	ଅଚଳ ସଂରଚନା worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	ଯଦି (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		ଯାଅ out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	ମୁକ୍ତ(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, ଖାଲି);

	ଯଦି (worktree && !worktree->is_bare) {
		ବାଛ (deny_current_branch) {
		ସ୍ଥିତି DENY_IGNORE:
			ଭାଙ୍ଗ;
		ସ୍ଥିତି DENY_WARN:
			rp_warning("updating the current branch");
			ଭାଙ୍ଗ;
		ସ୍ଥିତି DENY_REFUSE:
		ସ୍ଥିତି DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			ଯଦି (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			ଯାଅ out;
		ସ୍ଥିତି DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			ଭାଙ୍ଗ;
		}
	}

	ଯଦି (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		ଯାଅ out;
	}

	ଯଦି (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		ଯଦି (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			ଯାଅ out;
		}

		ଯଦି (worktree || (head_name && !strcmp(namespaced_name, head_name))) {
			ବାଛ (deny_delete_current) {
			ସ୍ଥିତି DENY_IGNORE:
				ଭାଙ୍ଗ;
			ସ୍ଥିତି DENY_WARN:
				rp_warning("deleting the current branch");
				ଭାଙ୍ଗ;
			ସ୍ଥିତି DENY_REFUSE:
			ସ୍ଥିତି DENY_UNCONFIGURED:
			ସ୍ଥିତି DENY_UPDATE_INSTEAD:
				ଯଦି (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				ଯାଅ out;
			ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
				ret = "Invalid denyDeleteCurrent setting";
				ଯାଅ out;
			}
		}
	}

	ଯଦି (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		ସଂରଚନା object *old_object, *new_object;
		ସଂରଚନା commit *old_commit, *new_commit;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		ଯଦି (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			ଯାଅ out;
		}
		old_commit = (ସଂରଚନା commit *)old_object;
		new_commit = (ସଂରଚନା commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		ଯଦି (ret2 < 0)
			exit(128);
		ଯଦି (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			ଯାଅ out;
		}
	}
	ଯଦି (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		ଯାଅ out;
	}

	ଯଦି (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		ଯଦି (ret)
			ଯାଅ out;
	}

	ଯଦି (is_null_oid(new_oid)) {
		ସଂରଚନା strbuf err = STRBUF_INIT;
		ଯଦି (!parse_object(the_repository, old_oid)) {
			old_oid = ଖାଲି;
			ଯଦି (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} ଅନ୍ୟଥା {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		ଯଦି (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   ଖାଲି, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} ଅନ୍ୟଥା {
			ret = ଖାଲି; /* good */
		}
		strbuf_release(&err);
	} ଅନ୍ୟଥା {
		ଗଣନା ref_transaction_error tx_err;
		ସଂରଚନା strbuf err = STRBUF_INIT;
		ଯଦି (shallow_update && si->shallow_ref[cmd->index] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			ଯାଅ out;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  ଖାଲି, ଖାଲି,
						  0, "push",
						  &err);
		ଯଦି (tx_err) {
			rp_error("%s", err.buf);
			ଯଦି (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			ଅନ୍ୟଥା
				ret = ref_transaction_error_ସନ୍ଦେଶ(tx_err);
		} ଅନ୍ୟଥା {
			ret = ଖାଲି; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ run_update_post_hook(ସଂରଚନା command *commands)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର hook_name[] = "post-update";
	ସଂରଚନା run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ସଂରଚନା async sideband_async;
	ସଂରଚନା command *cmd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା sideband_async_started = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_stderr = -1;

	ଯଦି (!hook_exists(the_repository, hook_name))
		ଫେରନ୍ତୁ;

	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
		ଯଦି (cmd->error_string || cmd->did_not_exist)
			ଜାରି;
		strvec_ଠେଲ(&opt.args, cmd->ref_name);
	}
	ଯଦି (!opt.args.nr)
		ଫେରନ୍ତୁ;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

ସ୍ଥିର ଶୂନ୍ୟ check_aliased_update_internal(ସଂରଚନା command *cmd,
					  ସଂରଚନା string_list *list,
					  ଅଚଳ ଅକ୍ଷର *dst_name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag)
{
	ସଂରଚନା string_list_item *item;
	ସଂରଚନା command *dst_cmd;

	ଯଦି (!(flag & REF_ISSYMREF))
		ଫେରନ୍ତୁ;

	ଯଦି (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		ଫେରନ୍ତୁ;
	}
	dst_name = strip_namespace(dst_name);

	ଯଦି (!(item = string_list_lookup(list, dst_name)))
		ଫେରନ୍ତୁ;

	cmd->skip_update = 1;

	dst_cmd = (ସଂରଚନା command *) item->util;

	ଯଦି (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		ଫେରନ୍ତୁ;

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

ସ୍ଥିର ଶୂନ୍ୟ check_aliased_update(ସଂରଚନା command *cmd, ସଂରଚନା string_list *list)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *dst_name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, ଖାଲି, &flag);
	check_aliased_update_internal(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

ସ୍ଥିର ଶୂନ୍ୟ check_aliased_updates(ସଂରଚନା command *commands)
{
	ସଂରଚନା command *cmd;
	ସଂରଚନା string_list ref_list = STRING_LIST_INIT_NODUP;

	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
		ସଂରଚନା string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (ଶୂନ୍ୟ *)cmd;
	}
	string_list_sort(&ref_list);

	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
		ଯଦି (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା object_id *command_singleton_iterator(ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା command **cmd_list = cb_data;
	ସଂରଚନା command *cmd = *cmd_list;

	ଯଦି (!cmd || is_null_oid(&cmd->new_oid))
		ଫେରନ୍ତୁ ଖାଲି;
	*cmd_list = ଖାଲି; /* this returns only one */
	ଫେରନ୍ତୁ &cmd->new_oid;
}

ସ୍ଥିର ଶୂନ୍ୟ set_connectivity_errors(ସଂରଚନା command *commands,
				    ସଂରଚନା shallow_info *si,
				    ସଂରଚନା odb_transaction *transaction)
{
	ସଂରଚନା command *cmd;

	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
		ସଂରଚନା command *singleton = cmd;
		ସଂରଚନା check_connected_options opt = CHECK_CONNECTED_INIT;
		ସଂରଚନା strvec env = STRVEC_INIT;

		ଯଦି (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			ଜାରି;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		ଯଦି (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			ଜାରି;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

ସଂରଚନା iterate_data {
	ସଂରଚନା command *cmds;
	ସଂରଚନା shallow_info *si;
};

ସ୍ଥିର ଅଚଳ ସଂରଚନା object_id *iterate_receive_command_list(ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା iterate_data *data = cb_data;
	ସଂରଚନା command **cmd_list = &data->cmds;
	ସଂରଚନା command *cmd = *cmd_list;

	ପାଇଁ (; cmd; cmd = cmd->next) {
		ଯଦି (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			ଜାରି;
		ଯଦି (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			ଫେରନ୍ତୁ &cmd->new_oid;
		}
	}
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଶୂନ୍ୟ reject_updates_to_hidden(ସଂରଚନା command *commands)
{
	ସଂରଚନା strbuf refname_full = STRBUF_INIT;
	size_t prefix_len;
	ସଂରଚନା command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
		ଯଦି (cmd->error_string)
			ଜାରି;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		ଯଦି (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			ଜାରି;
		ଯଦି (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		ଅନ୍ୟଥା
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା should_process_cmd(ସଂରଚନା command *cmd)
{
	ଫେରନ୍ତୁ !cmd->error_string && !cmd->skip_update;
}

ସ୍ଥିର ଶୂନ୍ୟ BUG_if_skipped_connectivity_check(ସଂରଚନା command *commands,
					       ସଂରଚନା shallow_info *si)
{
	ସଂରଚନା command *cmd;

	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
		ଯଦି (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

ସ୍ଥିର ଶୂନ୍ୟ ref_transaction_rejection_handler(ଅଚଳ ଅକ୍ଷର *refname,
					      ଅଚଳ ସଂରଚନା object_id *old_oid UNUSED,
					      ଅଚଳ ସଂରଚନା object_id *new_oid UNUSED,
					      ଅଚଳ ଅକ୍ଷର *old_target UNUSED,
					      ଅଚଳ ଅକ୍ଷର *new_target UNUSED,
					      ଗଣନା ref_transaction_error err,
					      ଅଚଳ ଅକ୍ଷର *details,
					      ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା strmap *failed_refs = cb_data;

	ଯଦି (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (ଅକ୍ଷର *)ref_transaction_error_ସନ୍ଦେଶ(err));
}

ସ୍ଥିର ଶୂନ୍ୟ execute_commands_non_atomic(ସଂରଚନା command *commands,
					ସଂରଚନା shallow_info *si)
{
	ସଂରଚନା command *cmd;
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *reported_error = ଖାଲି;
	ସଂରଚନା strmap failed_refs = STRMAP_INIT;

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
	ଗଣନା processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	ପାଇଁ (ଗଣନା processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
			ଯଦି (!should_process_cmd(cmd) || cmd->run_proc_receive)
				ଜାରି;

			ଯଦି (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				ଜାରି;
			ଅନ୍ୟଥା ଯଦି (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				ଜାରି;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			ଯଦି (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				ଯଦି (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					ଯାଅ failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		ଯଦି (!transaction)
			ଯାଅ cleanup;

		ଯଦି (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			ଯାଅ failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		ଯଦି (strmap_empty(&failed_refs))
			ଯାଅ cleanup;

	failure:
		ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
			ଯଦି (reported_error)
				cmd->error_string = reported_error;
			ଅନ୍ୟଥା ଯଦି (strmap_contains(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = ଖାଲି;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ execute_commands_atomic(ସଂରଚନା command *commands,
					ସଂରଚନା shallow_info *si)
{
	ସଂରଚନା command *cmd;
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ଯଦି (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		ଯାଅ failure;
	}

	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
		ଯଦି (!should_process_cmd(cmd) || cmd->run_proc_receive)
			ଜାରି;

		cmd->error_string = update(cmd, si);

		ଯଦି (cmd->error_string)
			ଯାଅ failure;
	}

	ଯଦି (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		ଯାଅ failure;
	}
	ଯାଅ cleanup;

failure:
	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next)
		ଯଦି (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

ସ୍ଥିର ଶୂନ୍ୟ execute_commands(ସଂରଚନା command *commands,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା unpacker_error,
			     ସଂରଚନା shallow_info *si,
			     ସଂରଚନା odb_transaction *transaction,
			     ଅଚଳ ସଂରଚନା string_list *ଠେଲ_options)
{
	ସଂରଚନା check_connected_options opt = CHECK_CONNECTED_INIT;
	ସଂରଚନା command *cmd;
	ସଂରଚନା iterate_data data;
	ସଂରଚନା async muxer;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err_fd = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_proc_receive = 0;

	ଯଦି (unpacker_error) {
		ପାଇଁ (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		ଫେରନ୍ତୁ;
	}

	ଯଦି (!skip_connectivity_check) {
		ସଂରଚନା strvec env = STRVEC_INIT;

		ଯଦି (use_sideband) {
			memset(&muxer, 0, ଆକାର(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			ଯଦି (!start_async(&muxer))
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

		ଯଦି (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		ଯଦି (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	ଯଦି (proc_receive_ref) {
		ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
			ଯଦି (!should_process_cmd(cmd))
				ଜାରି;

			ଯଦି (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	ଯଦି (run_receive_hook(commands, "pre-receive", 0, transaction, ଠେଲ_options)) {
		ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
			ଯଦି (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		ଫେରନ୍ତୁ;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	ପାଇଁ (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	ଯଦି (!cmd)
		ଫେରନ୍ତୁ;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	ଯଦି (odb_transaction_commit(transaction)) {
		ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
			ଯଦି (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		ଫେରନ୍ତୁ;
	}

	check_aliased_updates(commands);

	ମୁକ୍ତ(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, ଖାଲି,
							    ଖାଲି);

	ଯଦି (run_proc_receive &&
	    run_proc_receive_hook(commands, ଠେଲ_options))
		ପାଇଁ (cmd = commands; cmd; cmd = cmd->next)
			ଯଦି (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	ଯଦି (use_atomic)
		execute_commands_atomic(commands, si);
	ଅନ୍ୟଥା
		execute_commands_non_atomic(commands, si);

	ଯଦି (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

ସ୍ଥିର ସଂରଚନା command **queue_command(ସଂରଚନା command **tail,
				      ଅଚଳ ଅକ୍ଷର *ଧାଡ଼ି,
				      ପୂର୍ଣ୍ଣସଂଖ୍ୟା linelen)
{
	ସଂରଚନା object_id old_oid, new_oid;
	ସଂରଚନା command *cmd;
	ଅଚଳ ଅକ୍ଷର *refname;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା reflen;
	ଅଚଳ ଅକ୍ଷର *p;

	ଯଦି (parse_oid_hex(ଧାଡ଼ି, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", ଧାଡ଼ି);

	refname = p;
	reflen = linelen - (p - ଧାଡ଼ି);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*tail = cmd;
	ଫେରନ୍ତୁ &cmd->next;
}

ସ୍ଥିର ଶୂନ୍ୟ free_commands(ସଂରଚନା command *commands)
{
	ଯେତେବେଳେ (commands) {
		ସଂରଚନା command *next = commands->next;

		ref_ଠେଲ_report_free(commands->report);
		ମୁକ୍ତ(commands->error_string_owned);
		ମୁକ୍ତ(commands);
		commands = next;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ queue_commands_from_cert(ସଂରଚନା command **tail,
				     ସଂରଚନା strbuf *ଠେଲ_cert)
{
	ଅଚଳ ଅକ୍ଷର *boc, *eoc;

	ଯଦି (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(ଠେଲ_cert->buf, "\n\n");
	ଯଦି (!boc)
		die("malformed push certificate %.*s", 100, ଠେଲ_cert->buf);
	ଅନ୍ୟଥା
		boc += 2;
	eoc = ଠେଲ_cert->buf + parse_signed_buffer(ଠେଲ_cert->buf, ଠେଲ_cert->len);

	ଯେତେବେଳେ (boc < eoc) {
		ଅଚଳ ଅକ୍ଷର *eol = memchr(boc, '\n', eoc - boc);
		tail = queue_command(tail, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

ସ୍ଥିର ସଂରଚନା command *read_head_info(ସଂରଚନା packet_reader *reader,
				      ସଂରଚନା oid_array *shallow,
				      ଗଣନା report_status_version *version)
{
	ସଂରଚନା command *commands = ଖାଲି;
	ସଂରଚନା command **p = &commands;
	ପାଇଁ (;;) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା linelen;

		ଯଦି (packet_reader_read(reader) != PACKET_READ_NORMAL)
			ଭାଙ୍ଗ;

		ଯଦି (reader->pktlen > 8 && starts_with(reader->ଧାଡ଼ି, "shallow ")) {
			ସଂରଚନା object_id oid;
			ଯଦି (get_oid_hex(reader->ଧାଡ଼ି + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->ଧାଡ଼ି + 8);
			oid_array_append(shallow, &oid);
			ଜାରି;
		}

		linelen = strlen(reader->ଧାଡ଼ି);
		ଯଦି (linelen < reader->pktlen) {
			ଅଚଳ ଅକ୍ଷର *feature_list = reader->ଧାଡ଼ି + linelen + 1;
			ଅଚଳ ଅକ୍ଷର *hash = ଖାଲି;
			ଅଚଳ ଅକ୍ଷର *client_sid;
			size_t len = 0;
			ଯଦି (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			ଯଦି (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			ଯଦି (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			ଯଦି (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			ଯଦି (advertise_atomic_ଠେଲ
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			ଯଦି (advertise_ଠେଲ_options
			    && parse_feature_request(feature_list, "push-options"))
				use_ଠେଲ_options = 1;
			hash = parse_feature_ମୂଲ୍ୟ(feature_list, "object-format", &len, ଖାଲି);
			ଯଦି (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			ଯଦି (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_ମୂଲ୍ୟ(feature_list, "session-id", &len, ଖାଲି);
			ଯଦି (client_sid) {
				ଅକ୍ଷର *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", ଖାଲି, "client-sid", client_sid);
				ମୁକ୍ତ(sid);
			}
		}

		ଯଦି (!strcmp(reader->ଧାଡ଼ି, "push-cert")) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା true_flush = 0;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			ପାଇଁ (;;) {
				packet_reader_read(reader);
				ଯଦି (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					ଭାଙ୍ଗ;
				}
				ଯଦି (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				ଯଦି (!strcmp(reader->ଧାଡ଼ି, "push-cert-end\n"))
					ଭାଙ୍ଗ; /* end of cert */
				strbuf_addstr(&ଠେଲ_cert, reader->ଧାଡ଼ି);
			}
			reader->options = saved_options;

			ଯଦି (true_flush)
				ଭାଙ୍ଗ;
			ଜାରି;
		}

		p = queue_command(p, reader->ଧାଡ଼ି, linelen);
	}

	ଯଦି (ଠେଲ_cert.len)
		queue_commands_from_cert(p, &ଠେଲ_cert);

	ଫେରନ୍ତୁ commands;
}

ସ୍ଥିର ଶୂନ୍ୟ read_ଠେଲ_options(ସଂରଚନା packet_reader *reader,
			      ସଂରଚନା string_list *options)
{
	ଯେତେବେଳେ (1) {
		ଯଦି (packet_reader_read(reader) != PACKET_READ_NORMAL)
			ଭାଙ୍ଗ;

		string_list_append(options, reader->ଧାଡ଼ି);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା unpack_with_sideband(ସଂରଚନା odb_transaction *transaction,
				ଅଚଳ ଅକ୍ଷର *shallow_file,
				ସଂରଚନା strbuf *err_ସନ୍ଦେଶ)
{
	ସଂରଚନା odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_ସନ୍ଦେଶ_types = fsck_ସନ୍ଦେଶ_types.buf,
		.max_ନିବେଶ_size = max_ନିବେଶ_size,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	ସଂରଚନା async muxer;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (!use_sideband)
		ଫେରନ୍ତୁ odb_transaction_write_pack(transaction, 0, err_ସନ୍ଦେଶ, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, ଆକାର(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	ଯଦି (start_async(&muxer))
		ଫେରନ୍ତୁ 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_ସନ୍ଦେଶ, &opts);

	finish_async(&muxer);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ prepare_shallow_update(ସଂରଚନା shallow_info *si,
				   ଅଚଳ ଅକ୍ଷର *shallow_file)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k, bitmap_size = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, ଖାଲି);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	ପାଇଁ (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	ପାଇଁ (i = 0; i < si->shallow->nr; i++) {
		ଯଦି (!si->used_shallow[i])
			ଜାରି;
		ପାଇଁ (j = 0; j < bitmap_size; j++) {
			ଯଦି (!si->used_shallow[i][j])
				ଜାରି;
			si->need_reachability_test[i]++;
			ପାଇଁ (k = 0; k < 32; k++)
				ଯଦି (si->used_shallow[i][j] & (1U << k))
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

ସ୍ଥିର ଶୂନ୍ୟ update_shallow_info(ସଂରଚନା command *commands,
				ସଂରଚନା shallow_info *si,
				ସଂରଚନା oid_array *ref,
				ଅଚଳ ଅକ୍ଷର *shallow_file)
{
	ସଂରଚନା command *cmd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ref_status;
	remove_nonexistent_theirs_shallow(si);
	ଯଦି (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		ଫେରନ୍ତୁ;
	}

	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
		ଯଦି (is_null_oid(&cmd->new_oid))
			ଜାରି;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	ଯଦି (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		ଫେରନ୍ତୁ;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, ଖାଲି, ref_status);
	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
		ଯଦି (is_null_oid(&cmd->new_oid))
			ଜାରି;
		ଯଦି (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	ମୁକ୍ତ(ref_status);
}

ସ୍ଥିର ଶୂନ୍ୟ override_cmds_error(ସଂରଚନା command *commands, ଅଚଳ ଅକ୍ଷର *err)
{
	ପାଇଁ (ସଂରଚନା command *cmd = commands; cmd; cmd = cmd->next) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
ସ୍ଥିର ଶୂନ୍ୟ generate_report(ସଂରଚନା strbuf *buf, ସଂରଚନା command *commands,
			    ଅଚଳ ସଂରଚନା strbuf *unpack_status,
			    ଗଣନା report_status_version version)
{
	ସଂରଚନା command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
		ସଂରଚନା ref_ଠେଲ_report *report;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;

		ଯଦି (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		ଅନ୍ୟଥା
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		ଯଦି (version != REPORT_STATUS_V2 || cmd->error_string)
			ଜାରି;

		ପାଇଁ (report = cmd->report; report; report = report->next) {
			ଯଦି (ଗଣନା_2++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			ଯଦି (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			ଯଦି (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			ଯଦି (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			ଯଦି (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

ସ୍ଥିର ଶୂନ୍ୟ report(ସଂରଚନା command *commands, ଅଚଳ ସଂରଚନା strbuf *unpack_status,
		   ଗଣନା report_status_version version)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	ଯଦି (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	ଯଦି (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	ଅନ୍ୟଥା
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା delete_only(ସଂରଚନା command *commands)
{
	ସଂରଚନା command *cmd;
	ପାଇଁ (cmd = commands; cmd; cmd = cmd->next) {
		ଯଦି (!is_null_oid(&cmd->new_oid))
			ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_receive_pack(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		     ଅଚଳ ଅକ୍ଷର **argv,
		     ଅଚଳ ଅକ୍ଷର *prefix,
		     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା advertise_refs = 0;
	ସଂରଚନା command *commands;
	ସଂରଚନା oid_array shallow = OID_ARRAY_INIT;
	ସଂରଚନା oid_array ref = OID_ARRAY_INIT;
	ସଂରଚନା shallow_info si;
	ସଂରଚନା packet_reader reader;
	ସଂରଚନା odb_transaction *transaction = ଖାଲି;
	ଗଣନା report_status_version version = REPORT_STATUS_UNKNOWN;

	ସଂରଚନା option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, ଖାଲି),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, ଖାଲି),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, ଖାଲି),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, ଖାଲି),
		OPT_END()
	};

	packet_trace_identity("receive-pack");

	argc = parse_options(argc, argv, prefix, options, receive_pack_usage, 0);

	ଯଦି (argc > 1)
		usage_ସନ୍ଦେଶ_opt(_("too many arguments"), receive_pack_usage, options);
	ଯଦି (argc == 0)
		usage_ସନ୍ଦେଶ_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	ଯଦି (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, ଖାଲି);
	ଯଦି (cert_nonce_seed)
		ଠେଲ_cert_nonce = prepare_ଠେଲ_cert_nonce(service_dir, time(ଖାଲି));

	ବାଛ (determine_protocol_version_server()) {
	ସ୍ଥିତି protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		ଭାଙ୍ଗ;
	ସ୍ଥିତି protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ଯଦି (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	ସ୍ଥିତି protocol_v0:
		ଭାଙ୍ଗ;
	ସ୍ଥିତି protocol_unknown_version:
		BUG("unknown protocol version");
	}

	ଯଦି (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	ଯଦି (advertise_refs)
		ଫେରନ୍ତୁ 0;

	packet_reader_init(&reader, 0, ଖାଲି, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ଯଦି ((commands = read_head_info(&reader, &shallow, &version))) {
		ସଂରଚନା string_list ଠେଲ_options = STRING_LIST_INIT_DUP;
		ସଂରଚନା strbuf unpack_status = STRBUF_INIT;

		ଯଦି (use_ଠେଲ_options)
			read_ଠେଲ_options(&reader, &ଠେଲ_options);
		ଯଦି (!check_cert_ଠେଲ_options(&ଠେଲ_options)) {
			ସଂରଚନା command *cmd;
			ପାଇଁ (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		ଯଦି (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		ଯଦି (!delete_only(commands)) {
			ଅଚଳ ଅକ୍ଷର *alt_shallow_file = ଖାଲି;

			ଯଦି (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			ଯଦି (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			ଅନ୍ୟଥା
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &ଠେଲ_options);
		odb_transaction_finalize(transaction);
		sigchain_ଠେଲ(SIGPIPE, SIG_IGN);

		ବାଛ (version) {
		ସ୍ଥିତି REPORT_STATUS_V2:
		ସ୍ଥିତି REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି REPORT_STATUS_UNKNOWN:
			ଭାଙ୍ଗ;
		}

		sigchain_କାଢ଼(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, ଖାଲି,
				 &ଠେଲ_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&ଠେଲ_options, 0);
		ଯଦି (auto_gc) {
			ସଂରଚନା child_process proc = CHILD_PROCESS_INIT;

			ଯଦି (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				ଯଦି (!start_command(&proc)) {
					ଯଦି (use_sideband)
						copy_to_sideband(proc.err, -1, ଖାଲି);
					finish_command(&proc);
				}
			}
		}
		ଯଦି (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	ଯଦି (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	ମୁକ୍ତ((ଶୂନ୍ୟ *)ଠେଲ_cert_nonce);
	ଫେରନ୍ତୁ 0;
}
