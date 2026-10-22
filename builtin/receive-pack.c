#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "abspath.h"
#include "commit.h"
#include "commit-reach.h"
#include "config.h"
#include "connect.h"
#include "connected.h"
#include "environment.h"
#include "exec-cmd.h"
#include "fsck.h"
#include "gettext.h"
#include "gpg-interface.h"
#include "hex.h"
#include "hook.h"
#include "object.h"
#include "object-file.h"
#include "object-name.h"
#include "odb.h"
#include "oid-array.h"
#include "oidset.h"
#include "pack.h"
#include "parse-options.h"
#include "pkt-line.h"
#include "protocol.h"
#include "refs.h"
#include "remote.h"
#include "run-command.h"
#include "server-info.h"
#include "setup.h"
#include "shallow.h"
#include "sideband.h"
#include "sigchain.h"
#include "string-list.h"
#include "strvec.h"
#include "trace.h"
#include "trace2.h"
#include "version.h"
#include "worktree.h"

static const var_na * const receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	NULL
};

ga_nanaa deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

ga_nanaa report_status_version {
	REPORT_STATUS_UNKNOWN = 0,
	REPORT_STATUS_V0,
	REPORT_STATUS_V2,
};

static puur_naa_nka deny_deletes;
static puur_naa_nka deny_non_fast_forwards;
static ga_nanaa deny_action deny_current_branch = DENY_UNCONFIGURED;
static ga_nanaa deny_action deny_delete_current = DENY_UNCONFIGURED;
static puur_naa_nka receive_fsck_objects = -1;
static puur_naa_nka transfer_fsck_objects = -1;
static sa_nracanaa strbuf fsck_msg_types = STRBUF_INIT;
static puur_naa_nka advertise_atomic_push = 1;
static puur_naa_nka advertise_push_options;
static puur_naa_nka advertise_sid;
static off_t max_input_size;
static puur_naa_nka use_sideband;
static puur_naa_nka use_atomic;
static puur_naa_nka use_push_options;
static puur_naa_nka quiet;
static puur_naa_nka prefer_ofs_delta = 1;
static puur_naa_nka auto_update_server_info;
static puur_naa_nka auto_gc = 1;
static puur_naa_nka reject_thin;
static puur_naa_nka skip_connectivity_check;
static puur_naa_nka stateless_rpc;
static const var_na *service_dir;
static const var_na *head_name;
static void *head_name_to_free;
static puur_naa_nka sent_capabilities;
static puur_naa_nka shallow_update;
static sa_nracanaa strbuf push_cert = STRBUF_INIT;
static sa_nracanaa object_id push_cert_oid;
static sa_nracanaa signature_check sigcheck;
static const var_na *push_cert_nonce;
static var_na *cert_nonce_seed;
static sa_nracanaa strvec hidden_refs = STRVEC_INIT;

static const var_na *NONCE_UNSOLICITED = "UNSOLICITED";
static const var_na *NONCE_BAD = "BAD";
static const var_na *NONCE_MISSING = "MISSING";
static const var_na *NONCE_OK = "OK";
static const var_na *NONCE_SLOP = "SLOP";
static const var_na *nonce_status;
static long nonce_stamp_slop;
static timestamp_t nonce_stamp_slop_limit;
static sa_nracanaa ref_transaction *transaction;

static ga_nanaa {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
static puur_naa_nka keepalive_in_sec = 5;

static sa_nracanaa proc_receive_ref {
	unsigned puur_naa_nka want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	var_na *ref_prefix;
	sa_nracanaa proc_receive_ref *next;
} *proc_receive_ref;

static void proc_receive_ref_append(const var_na *prefix);

static ga_nanaa deny_action parse_deny_action(const var_na *var, const var_na *value)
{
	ꦪꦼꦤ꧀ (value) {
		ꦪꦼꦤ꧀ (!strcasecmp(value, "ignore"))
			ꦮꦭꦶ DENY_IGNORE;
		ꦪꦼꦤ꧀ (!strcasecmp(value, "warn"))
			ꦮꦭꦶ DENY_WARN;
		ꦪꦼꦤ꧀ (!strcasecmp(value, "refuse"))
			ꦮꦭꦶ DENY_REFUSE;
		ꦪꦼꦤ꧀ (!strcasecmp(value, "updateinstead"))
			ꦮꦭꦶ DENY_UPDATE_INSTEAD;
	}
	ꦪꦼꦤ꧀ (git_config_bool(var, value))
		ꦮꦭꦶ DENY_REFUSE;
	ꦮꦭꦶ DENY_IGNORE;
}

static puur_naa_nka receive_pack_config(const var_na *var, const var_na *value,
			       const sa_nracanaa config_context *ctx, void *cb)
{
	const var_na *msg_id;
	puur_naa_nka status = parse_hide_refs_config(var, value, "receive", &hidden_refs);

	ꦪꦼꦤ꧀ (status)
		ꦮꦭꦶ status;

	ꦪꦼꦤ꧀ (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.fsck.skiplist") == 0) {
		var_na *path;

		ꦪꦼꦤ꧀ (git_config_pathname(&path, var, value))
			ꦮꦭꦶ -1;
		ꦪꦼꦤ꧀ (path)
			strbuf_addf(&fsck_msg_types, "%cskiplist=%s",
				    fsck_msg_types.len ? ',' : '=', path);
		free(path);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (skip_prefix(var, "receive.fsck.", &msg_id)) {
		ꦪꦼꦤ꧀ (!value)
			ꦮꦭꦶ config_error_nonbool(var);
		ꦪꦼꦤ꧀ (is_valid_msg_type(msg_id, value))
			strbuf_addf(&fsck_msg_types, "%c%s=%s",
				fsck_msg_types.len ? ',' : '=', msg_id, value);
		ꦭꦶꦪ
			warning("skipping unknown msg id '%s'", msg_id);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.certnonceseed") == 0)
		ꦮꦭꦶ git_config_string(&cert_nonce_seed, var, value);

	ꦪꦼꦤ꧀ (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, value, ctx->kvi);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_push = git_config_bool(var, value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_push_options = git_config_bool(var, value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, value, ctx->kvi);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.maxinputsize") == 0) {
		max_input_size = git_config_int64(var, value, ctx->kvi);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "receive.procreceiverefs") == 0) {
		ꦪꦼꦤ꧀ (!value)
			ꦮꦭꦶ config_error_nonbool(var);
		proc_receive_ref_append(value);
		ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, value);
		ꦮꦭꦶ 0;
	}

	ꦮꦭꦶ git_default_config(var, value, ctx, cb);
}

static void show_ref(const var_na *path, const sa_nracanaa object_id *oid)
{
	ꦪꦼꦤ꧀ (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} ꦭꦶꦪ {
		sa_nracanaa strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		ꦪꦼꦤ꧀ (advertise_atomic_push)
			strbuf_addstr(&cap, " atomic");
		ꦪꦼꦤ꧀ (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		ꦪꦼꦤ꧀ (push_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", push_cert_nonce);
		ꦪꦼꦤ꧀ (advertise_push_options)
			strbuf_addstr(&cap, " push-options");
		ꦪꦼꦤ꧀ (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

static puur_naa_nka show_ref_cb(const sa_nracanaa reference *ref, void *data)
{
	sa_nracanaa oidset *seen = data;
	const var_na *path = strip_namespace(ref->name);

	ꦪꦼꦤ꧀ (ref_is_hidden(path, ref->name, &hidden_refs))
		ꦮꦭꦶ 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	ꦪꦼꦤ꧀ (!path) {
		ꦪꦼꦤ꧀ (oidset_insert(seen, ref->oid))
			ꦮꦭꦶ 0;
		path = ".have";
	} ꦭꦶꦪ {
		oidset_insert(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	ꦮꦭꦶ 0;
}

static void show_one_alternate_ref(const sa_nracanaa object_id *oid,
				   void *data)
{
	sa_nracanaa oidset *seen = data;

	ꦪꦼꦤ꧀ (oidset_insert(seen, oid))
		ꦮꦭꦶ;

	show_ref(".have", oid);
}

static void write_head_info(void)
{
	sa_nracanaa refs_for_each_ref_options opts = { 0 };
	static sa_nracanaa oidset seen = OIDSET_INIT;
	sa_nracanaa strvec excludes_vector = STRVEC_INIT;

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

	ꦪꦼꦤ꧀ (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#define RUN_PROC_RECEIVE_SCHEDULED	1
#define RUN_PROC_RECEIVE_RETURNED	2
sa_nracanaa command {
	sa_nracanaa command *next;
	const var_na *error_string;
	var_na *error_string_owned;
	sa_nracanaa ref_push_report *report;
	unsigned puur_naa_nka skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	puur_naa_nka index;
	sa_nracanaa object_id old_oid;
	sa_nracanaa object_id new_oid;
	var_na ref_name[FLEX_ARRAY]; /* more */
};

static void proc_receive_ref_append(const var_na *prefix)
{
	sa_nracanaa proc_receive_ref *ref_pattern;
	const var_na *p;
	puur_naa_nka len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = strchr(prefix, ':');
	ꦪꦼꦤ꧀ (p) {
		ꦱꦼꦢꦼꦭ (prefix < p) {
			ꦪꦼꦤ꧀ (*prefix == 'a')
				ref_pattern->want_add = 1;
			ꦭꦶꦪ ꦪꦼꦤ꧀ (*prefix == 'd')
				ref_pattern->want_delete = 1;
			ꦭꦶꦪ ꦪꦼꦤ꧀ (*prefix == 'm')
				ref_pattern->want_modify = 1;
			ꦭꦶꦪ ꦪꦼꦤ꧀ (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} ꦭꦶꦪ {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	ꦱꦼꦢꦼꦭ (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	ꦪꦼꦤ꧀ (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} ꦭꦶꦪ {
		sa_nracanaa proc_receive_ref *end;

		end = proc_receive_ref;
		ꦱꦼꦢꦼꦭ (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

static puur_naa_nka proc_receive_ref_matches(sa_nracanaa command *cmd)
{
	sa_nracanaa proc_receive_ref *p;

	ꦪꦼꦤ꧀ (!proc_receive_ref)
		ꦮꦭꦶ 0;

	ꦏꦁꦒꦺꦴ (p = proc_receive_ref; p; p = p->next) {
		const var_na *match = p->ref_prefix;
		const var_na *remains;

		ꦪꦼꦤ꧀ (!p->want_add && is_null_oid(&cmd->old_oid))
			continue;
		ꦭꦶꦪ ꦪꦼꦤ꧀ (!p->want_delete && is_null_oid(&cmd->new_oid))
			continue;
		ꦭꦶꦪ ꦪꦼꦤ꧀ (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			continue;

		ꦪꦼꦤ꧀ (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			ꦪꦼꦤ꧀ (!p->negative_ref)
				ꦮꦭꦶ 1;
		} ꦭꦶꦪ ꦪꦼꦤ꧀ (p->negative_ref) {
			ꦮꦭꦶ 1;
		}
	}
	ꦮꦭꦶ 0;
}

static void report_message(const var_na *prefix, const var_na *err, va_list params)
{
	puur_naa_nka sz;
	var_na msg[4096];

	sz = xsnprintf(msg, parimaa_na(msg), "%s", prefix);
	sz += vsnprintf(msg + sz, parimaa_na(msg) - sz, err, params);
	ꦪꦼꦤ꧀ (sz > (parimaa_na(msg) - 1))
		sz = parimaa_na(msg) - 1;
	msg[sz++] = '\n';

	ꦪꦼꦤ꧀ (use_sideband)
		send_sideband(1, 2, msg, sz, use_sideband);
	ꦭꦶꦪ
		xwrite(2, msg, sz);
}

__attribute__((format (printf, 1, 2)))
static void rp_warning(const var_na *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("warning: ", err, params);
	va_end(params);
}

__attribute__((format (printf, 1, 2)))
static void rp_error(const var_na *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("error: ", err, params);
	va_end(params);
}

static puur_naa_nka copy_to_sideband(puur_naa_nka in, puur_naa_nka out UNUSED, void *arg UNUSED)
{
	var_na data[128];
	puur_naa_nka keepalive_active = 0;

	ꦪꦼꦤ꧀ (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	ꦪꦼꦤ꧀ (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	ꦱꦼꦢꦼꦭ (1) {
		ssize_t sz;

		ꦪꦼꦤ꧀ (keepalive_active) {
			sa_nracanaa pollfd pfd;
			puur_naa_nka ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			ꦪꦼꦤ꧀ (ret < 0) {
				ꦪꦼꦤ꧀ (errno == EINTR)
					continue;
				ꦭꦶꦪ
					bha_nga;
			} ꦭꦶꦪ ꦪꦼꦤ꧀ (ret == 0) {
				/* no data; send a keepalive packet */
				static const var_na buf[] = "0005\1";
				write_or_die(1, buf, parimaa_na(buf) - 1);
				continue;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, parimaa_na(data));
		ꦪꦼꦤ꧀ (sz <= 0)
			bha_nga;

		ꦪꦼꦤ꧀ (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			const var_na *p = memchr(data, '\0', sz);
			ꦪꦼꦤ꧀ (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_active = 1;
				send_sideband(1, 2, data, p - data, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - data + 1), use_sideband);
				continue;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, data, sz, use_sideband);
	}
	close(in);
	ꦮꦭꦶ 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
static void prepare_sideband_async(sa_nracanaa async *sideband_async, puur_naa_nka *saved_stderr, puur_naa_nka *started)
{
	*started = 0;

	ꦪꦼꦤ꧀ (!use_sideband)
		ꦮꦭꦶ;

	memset(sideband_async, 0, parimaa_na(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	ꦪꦼꦤ꧀ (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		ꦪꦼꦤ꧀ (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
static void finish_sideband_async(sa_nracanaa async *sideband_async, puur_naa_nka saved_stderr, puur_naa_nka started)
{
	ꦪꦼꦤ꧀ (!use_sideband)
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	ꦪꦼꦤ꧀ (started)
		finish_async(sideband_async);
}

static void hmac_hash(unsigned var_na *out,
		      const var_na *key_in, size_t key_len,
		      const var_na *text, size_t text_len)
{
	unsigned var_na key[GIT_MAX_BLKSZ];
	unsigned var_na k_ipad[GIT_MAX_BLKSZ];
	unsigned var_na k_opad[GIT_MAX_BLKSZ];
	puur_naa_nka i;
	sa_nracanaa git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(key, '\0', GIT_MAX_BLKSZ);
	ꦪꦼꦤ꧀ (the_hash_algo->blksz < key_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, key_in, key_len);
		git_hash_final(key, &ctx);
	} ꦭꦶꦪ {
		memcpy(key, key_in, key_len);
	}

	/* RFC 2104 2. (2) & (5) */
	ꦏꦁꦒꦺꦴ (i = 0; i < parimaa_na(key); i++) {
		k_ipad[i] = key[i] ^ 0x36;
		k_opad[i] = key[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, parimaa_na(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, parimaa_na(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

static var_na *prepare_push_cert_nonce(const var_na *path, timestamp_t stamp)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	unsigned var_na hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, strlen(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (puur_naa_nka)the_hash_algo->hexsz, hash_to_hex(hash));
	ꦮꦭꦶ strbuf_detach(&buf, NULL);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
static puur_naa_nka constant_memequal(const var_na *a, const var_na *b, size_t n)
{
	puur_naa_nka res = 0;
	size_t i;

	ꦏꦁꦒꦺꦴ (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	ꦮꦭꦶ res;
}

static const var_na *check_nonce(const var_na *buf)
{
	size_t noncelen;
	const var_na *found = find_commit_header(buf, "nonce", &noncelen);
	var_na *nonce = found ? xmemdupz(found, noncelen) : NULL;
	timestamp_t stamp, ostamp;
	var_na *bohmac, *expect = NULL;
	const var_na *retval = NONCE_BAD;

	ꦪꦼꦤ꧀ (!nonce) {
		retval = NONCE_MISSING;
		goto leave;
	} ꦭꦶꦪ ꦪꦼꦤ꧀ (!push_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		goto leave;
	} ꦭꦶꦪ ꦪꦼꦤ꧀ (!strcmp(push_cert_nonce, nonce)) {
		retval = NONCE_OK;
		goto leave;
	}

	ꦪꦼꦤ꧀ (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		goto leave;
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
	ꦪꦼꦤ꧀ (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		goto leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	ꦪꦼꦤ꧀ (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		goto leave;
	}

	expect = prepare_push_cert_nonce(service_dir, stamp);
	ꦪꦼꦤ꧀ (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		goto leave;
	}
	ꦪꦼꦤ꧀ (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		goto leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(push_cert_nonce, NULL, 10);
	nonce_stamp_slop = (long)ostamp - (long)stamp;

	ꦪꦼꦤ꧀ (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		free((void *)push_cert_nonce);
		push_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} ꦭꦶꦪ {
		retval = NONCE_SLOP;
	}

leave:
	free(nonce);
	free(expect);
	ꦮꦭꦶ retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
static puur_naa_nka check_cert_push_options(const sa_nracanaa string_list *push_options)
{
	const var_na *buf = push_cert.buf;

	const var_na *option;
	size_t optionlen;
	puur_naa_nka options_seen = 0;

	puur_naa_nka retval = 1;

	ꦪꦼꦤ꧀ (!*buf)
		ꦮꦭꦶ 1;

	ꦱꦼꦢꦼꦭ ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		ꦪꦼꦤ꧀ (options_seen > push_options->nr
		    || xstrncmpz(push_options->items[options_seen - 1].string,
				 option, optionlen))
			ꦮꦭꦶ 0;
	}

	ꦪꦼꦤ꧀ (options_seen != push_options->nr)
		retval = 0;

	ꦮꦭꦶ retval;
}

static void prepare_push_cert_sha1(sa_nracanaa run_hooks_opt *opt)
{
	static puur_naa_nka already_done;

	ꦪꦼꦤ꧀ (!push_cert.len)
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (!already_done) {
		puur_naa_nka bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		ꦪꦼꦤ꧀ (odb_write_object(the_repository->objects, push_cert.buf,
				     push_cert.len, OBJ_BLOB, &push_cert_oid))
			oidclr(&push_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', parimaa_na(sigcheck));

		bogs = parse_signed_buffer(push_cert.buf, push_cert.len);
		sigcheck.payload = xmemdupz(push_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, push_cert.buf + bogs,
				push_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	ꦪꦼꦤ꧀ (!is_null_oid(&push_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&push_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.key ? sigcheck.key : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		ꦪꦼꦤ꧀ (push_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     push_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			ꦪꦼꦤ꧀ (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

sa_nracanaa receive_hook_feed_state {
	sa_nracanaa command *cmd;
	sa_nracanaa ref_push_report *report;
	puur_naa_nka skip_broken;
	sa_nracanaa strbuf buf;
};

static puur_naa_nka feed_receive_hook_cb(puur_naa_nka hook_stdin_fd, void *pp_cb UNUSED, void *pp_task_cb)
{
	sa_nracanaa receive_hook_feed_state *state = pp_task_cb;
	sa_nracanaa command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	ꦱꦼꦢꦼꦭ (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	ꦪꦼꦤ꧀ (!cmd)
		ꦮꦭꦶ 1;  /* no more commands left */

	ꦪꦼꦤ꧀ (!state->report)
		state->report = cmd->report;

	ꦪꦼꦤ꧀ (state->report) {
		sa_nracanaa object_id *old_oid;
		sa_nracanaa object_id *new_oid;
		const var_na *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->next;
		ꦪꦼꦤ꧀ (!state->report)
			cmd = cmd->next;
	} ꦭꦶꦪ {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	ꦪꦼꦤ꧀ (state->buf.len > 0) {
		puur_naa_nka ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		ꦪꦼꦤ꧀ (ret < 0) {
			ꦪꦼꦤ꧀ (errno == EPIPE)
				ꦮꦭꦶ 1; /* child closed pipe */
			ꦮꦭꦶ ret;
		}
	}

	ꦮꦭꦶ state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

static void *receive_hook_feed_state_alloc(void *feed_pipe_ctx)
{
	sa_nracanaa receive_hook_feed_state *init_state = feed_pipe_ctx;
	sa_nracanaa receive_hook_feed_state *data;

	CALLOC_ARRAY(data, 1);
	data->report = init_state->report;
	data->cmd = init_state->cmd;
	data->skip_broken = init_state->skip_broken;
	strbuf_init(&data->buf, 0);

	ꦮꦭꦶ data;
}

static void receive_hook_feed_state_free(void *data)
{
	sa_nracanaa receive_hook_feed_state *d = data;
	ꦪꦼꦤ꧀ (!d)
		ꦮꦭꦶ;
	strbuf_release(&d->buf);
	free(d);
}

static puur_naa_nka run_receive_hook(sa_nracanaa command *commands,
			    const var_na *hook_name,
			    puur_naa_nka skip_broken,
			    sa_nracanaa odb_transaction *transaction,
			    const sa_nracanaa string_list *push_options)
{
	sa_nracanaa run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	sa_nracanaa command *iter = commands;
	sa_nracanaa receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	sa_nracanaa async sideband_async;
	puur_naa_nka sideband_async_started = 0;
	puur_naa_nka saved_stderr = -1;
	puur_naa_nka ret;

	ꦪꦼꦤ꧀ (!hook_exists(the_repository, hook_name))
		ꦮꦭꦶ 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	ꦱꦼꦢꦼꦭ (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	ꦪꦼꦤ꧀ (!iter)
		ꦮꦭꦶ 0;

	ꦪꦼꦤ꧀ (push_options) {
		ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < push_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     push_options->items[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)push_options->nr);
	} ꦭꦶꦪ {
		strvec_push(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	ꦪꦼꦤ꧀ (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_push_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_data_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_data_free = receive_hook_feed_state_free;

	ret = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ꦮꦭꦶ ret;
}

static puur_naa_nka run_update_hook(sa_nracanaa command *cmd)
{
	static const var_na hook_name[] = "update";
	sa_nracanaa run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	sa_nracanaa async sideband_async;
	puur_naa_nka sideband_async_started = 0;
	puur_naa_nka saved_stderr = -1;
	puur_naa_nka code;

	ꦪꦼꦤ꧀ (!hook_exists(the_repository, hook_name))
		ꦮꦭꦶ 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     NULL);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ꦮꦭꦶ code;
}

static puur_naa_nka run_receive_report_hook(sa_nracanaa strbuf *report)
{
	sa_nracanaa child_process proc = CHILD_PROCESS_INIT;
	sa_nracanaa async sideband_async;
	puur_naa_nka sideband_async_started = 0;
	puur_naa_nka saved_stderr = -1;
	sa_nracanaa strbuf out = STRBUF_INIT;
	const var_na *hook_path;
	puur_naa_nka ret;

	hook_path = find_hook(the_repository, "receive-report");
	ꦪꦼꦤ꧀ (!hook_path)
		ꦮꦭꦶ 0;

	strvec_push(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_push(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, NULL, 0);
	sigchain_pop(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	ꦪꦼꦤ꧀ (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	ꦮꦭꦶ ret;
}

static sa_nracanaa command *find_command_by_refname(sa_nracanaa command *list,
					       const var_na *refname)
{
	ꦏꦁꦒꦺꦴ (; list; list = list->next)
		ꦪꦼꦤ꧀ (!strcmp(list->ref_name, refname))
			ꦮꦭꦶ list;
	ꦮꦭꦶ NULL;
}

static puur_naa_nka read_proc_receive_report(sa_nracanaa packet_reader *reader,
				    sa_nracanaa command *commands,
				    sa_nracanaa strbuf *errmsg)
{
	sa_nracanaa command *cmd;
	sa_nracanaa command *hint = NULL;
	sa_nracanaa ref_push_report *report = NULL;
	puur_naa_nka new_report = 0;
	puur_naa_nka code = 0;
	puur_naa_nka once = 0;
	puur_naa_nka response = 0;

	ꦏꦁꦒꦺꦴ (;;) {
		sa_nracanaa object_id old_oid, new_oid;
		var_na *head;
		var_na *refname;
		var_na *p;
		ga_nanaa packet_read_status status;

		status = packet_reader_read(reader);
		ꦪꦼꦤ꧀ (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			ꦪꦼꦤ꧀ (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				ꦮꦭꦶ -1;
			}
			bha_nga;
		}
		response++;

		head = reader->line;
		p = strchr(head, ' ');
		ꦪꦼꦤ꧀ (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			continue;
		}
		*p++ = '\0';
		ꦪꦼꦤ꧀ (!strcmp(head, "option")) {
			var_na *key;
			const var_na *val;

			ꦪꦼꦤ꧀ (!hint || !(report || new_report)) {
				ꦪꦼꦤ꧀ (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				continue;
			}
			ꦪꦼꦤ꧀ (new_report) {
				ꦪꦼꦤ꧀ (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} ꦭꦶꦪ {
					report = hint->report;
					ꦱꦼꦢꦼꦭ (report->next)
						report = report->next;
					report->next = xcalloc(1, parimaa_na(sa_nracanaa ref_push_report));
					report = report->next;
				}
				new_report = 0;
			}
			key = p;
			p = strchr(key, ' ');
			ꦪꦼꦤ꧀ (p)
				*p++ = '\0';
			val = p;
			ꦪꦼꦤ꧀ (!strcmp(key, "refname"))
				report->ref_name = xstrdup_or_null(val);
			ꦭꦶꦪ ꦪꦼꦤ꧀ (!strcmp(key, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			ꦭꦶꦪ ꦪꦼꦤ꧀ (!strcmp(key, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			ꦭꦶꦪ ꦪꦼꦤ꧀ (!strcmp(key, "forced-update"))
				report->forced_update = 1;
			ꦭꦶꦪ ꦪꦼꦤ꧀ (!strcmp(key, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			continue;
		}

		report = NULL;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		ꦪꦼꦤ꧀ (p)
			*p++ = '\0';
		ꦪꦼꦤ꧀ (strcmp(head, "ok") && strcmp(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			continue;
		}

		/* first try searching at our hint, falling back to all refs */
		ꦪꦼꦤ꧀ (hint)
			hint = find_command_by_refname(hint, refname);
		ꦪꦼꦤ꧀ (!hint)
			hint = find_command_by_refname(commands, refname);
		ꦪꦼꦤ꧀ (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			continue;
		}
		ꦪꦼꦤ꧀ (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			continue;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		ꦪꦼꦤ꧀ (!strcmp(head, "ng")) {
			ꦪꦼꦤ꧀ (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			ꦭꦶꦪ
				hint->error_string = "failed";
			code = -1;
			continue;
		}
		new_report = 1;
	}

	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next)
		ꦪꦼꦤ꧀ (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	ꦮꦭꦶ code;
}

static puur_naa_nka run_proc_receive_hook(sa_nracanaa command *commands,
				 const sa_nracanaa string_list *push_options)
{
	sa_nracanaa child_process proc = CHILD_PROCESS_INIT;
	sa_nracanaa async muxer;
	sa_nracanaa command *cmd;
	sa_nracanaa packet_reader reader;
	sa_nracanaa strbuf cap = STRBUF_INIT;
	sa_nracanaa strbuf errmsg = STRBUF_INIT;
	puur_naa_nka hook_use_push_options = 0;
	puur_naa_nka version = 0;
	puur_naa_nka code;
	const var_na *hook_path = find_hook(the_repository, "proc-receive");

	ꦪꦼꦤ꧀ (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		ꦮꦭꦶ -1;
	}

	strvec_push(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	ꦪꦼꦤ꧀ (use_sideband) {
		memset(&muxer, 0, parimaa_na(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		ꦪꦼꦤ꧀ (code)
			ꦮꦭꦶ code;
		proc.err = muxer.in;
	} ꦭꦶꦪ {
		proc.err = 0;
	}

	code = start_command(&proc);
	ꦪꦼꦤ꧀ (code) {
		ꦪꦼꦤ꧀ (use_sideband)
			finish_async(&muxer);
		ꦮꦭꦶ code;
	}

	sigchain_push(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, NULL, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	ꦪꦼꦤ꧀ (use_atomic)
		strbuf_addstr(&cap, " atomic");
	ꦪꦼꦤ꧀ (use_push_options)
		strbuf_addstr(&cap, " push-options");
	ꦪꦼꦤ꧀ (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} ꦭꦶꦪ {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	ꦪꦼꦤ꧀ (!code)
		code = packet_flush_gently(proc.in);

	ꦪꦼꦤ꧀ (!code)
		ꦏꦁꦒꦺꦴ (;;) {
			puur_naa_nka linelen;
			ga_nanaa packet_read_status status;

			status = packet_reader_read(&reader);
			ꦪꦼꦤ꧀ (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				ꦪꦼꦤ꧀ (status == PACKET_READ_EOF)
					code = -1;
				bha_nga;
			}

			ꦪꦼꦤ꧀ (reader.pktlen > 8 && starts_with(reader.line, "version=")) {
				version = atoi(reader.line + 8);
				linelen = strlen(reader.line);
				ꦪꦼꦤ꧀ (linelen < reader.pktlen) {
					const var_na *feature_list = reader.line + linelen + 1;
					ꦪꦼꦤ꧀ (parse_feature_request(feature_list, "push-options"))
						hook_use_push_options = 1;
				}
			}
		}

	ꦪꦼꦤ꧀ (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		goto cleanup;
	}

	switch (version) {
	pak_sa 0:
		/* fallthrough */
	pak_sa 1:
		bha_nga;
	default:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		goto cleanup;
	}

	/* Send commands */
	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
		ꦪꦼꦤ꧀ (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			continue;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		ꦪꦼꦤ꧀ (code)
			bha_nga;
	}
	ꦪꦼꦤ꧀ (!code)
		code = packet_flush_gently(proc.in);
	ꦪꦼꦤ꧀ (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		goto cleanup;
	}

	/* Send push options */
	ꦪꦼꦤ꧀ (hook_use_push_options) {
		sa_nracanaa string_list_item *item;

		for_each_string_list_item(item, push_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			ꦪꦼꦤ꧀ (code)
				bha_nga;
		}
		ꦪꦼꦤ꧀ (!code)
			code = packet_flush_gently(proc.in);
		ꦪꦼꦤ꧀ (code) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			goto cleanup;
		}
	}

	/* Read result from proc-receive */
	code = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.in);
	close(proc.out);
	ꦪꦼꦤ꧀ (use_sideband)
		finish_async(&muxer);
	ꦪꦼꦤ꧀ (finish_command(&proc))
		code = -1;
	ꦪꦼꦤ꧀ (errmsg.len >0) {
		var_na *p = errmsg.buf;

		p += errmsg.len - 1;
		ꦪꦼꦤ꧀ (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_pop(SIGPIPE);

	ꦮꦭꦶ code;
}

static const var_na *refuse_unconfigured_deny_msg =
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

static void refuse_unconfigured_deny(void)
{
	rp_error("%s", _(refuse_unconfigured_deny_msg));
}

static const var_na *refuse_unconfigured_deny_delete_current_msg =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

static void refuse_unconfigured_deny_delete_current(void)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_msg));
}

static const sa_nracanaa object_id *command_singleton_iterator(void *cb_data);
static puur_naa_nka update_shallow_ref(sa_nracanaa command *cmd, sa_nracanaa shallow_info *si)
{
	sa_nracanaa shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	sa_nracanaa oid_array extra = OID_ARRAY_INIT;
	sa_nracanaa check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	puur_naa_nka i;

	trace_printf_key(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	ꦏꦁꦒꦺꦴ (i = 0; i < si->shallow->nr; i++)
		ꦪꦼꦤ꧀ (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	ꦪꦼꦤ꧀ (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		ꦮꦭꦶ -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	ꦏꦁꦒꦺꦴ (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	ꦮꦭꦶ 0;
}

static const var_na *push_to_deploy(unsigned var_na *sha1,
				  sa_nracanaa strvec *env,
				  const sa_nracanaa worktree *worktree)
{
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", NULL);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	ꦪꦼꦤ꧀ (run_command(&child))
		ꦮꦭꦶ "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", NULL);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	ꦪꦼꦤ꧀ (run_command(&child))
		ꦮꦭꦶ "Working directory has unstaged changes";

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
		     empty_tree_oid_hex(the_repository->hash_algo), "--", NULL);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	ꦪꦼꦤ꧀ (run_command(&child))
		ꦮꦭꦶ "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     NULL);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	ꦪꦼꦤ꧀ (run_command(&child))
		ꦮꦭꦶ "Could not update working tree to new HEAD";

	ꦮꦭꦶ NULL;
}

static const var_na *push_to_checkout_hook = "push-to-checkout";

static const var_na *push_to_checkout(unsigned var_na *hash,
				    puur_naa_nka *invoked_hook,
				    sa_nracanaa strvec *env,
				    const var_na *work_tree)
{
	sa_nracanaa run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_tree));
	strvec_push(&opt.args, hash_to_hex(hash));
	ꦪꦼꦤ꧀ (run_hooks_opt(the_repository, push_to_checkout_hook, &opt))
		ꦮꦭꦶ "push-to-checkout hook declined";
	ꦭꦶꦪ
		ꦮꦭꦶ NULL;
}

static const var_na *update_worktree(unsigned var_na *sha1, const sa_nracanaa worktree *worktree)
{
	const var_na *retval;
	var_na *git_dir;
	sa_nracanaa strvec env = STRVEC_INIT;
	puur_naa_nka invoked_hook;

	ꦪꦼꦤ꧀ (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	ꦪꦼꦤ꧀ (worktree->is_bare)
		ꦮꦭꦶ "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = push_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	ꦪꦼꦤ꧀ (!invoked_hook)
		retval = push_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	free(git_dir);
	ꦮꦭꦶ retval;
}

static const var_na *update(sa_nracanaa command *cmd, sa_nracanaa shallow_info *si)
{
	const var_na *name = cmd->ref_name;
	sa_nracanaa strbuf namespaced_name_buf = STRBUF_INIT;
	static var_na *namespaced_name;
	const var_na *ret;
	sa_nracanaa object_id *old_oid = &cmd->old_oid;
	sa_nracanaa object_id *new_oid = &cmd->new_oid;
	puur_naa_nka do_update_worktree = 0;
	sa_nracanaa worktree **worktrees = get_worktrees(the_repository);
	const sa_nracanaa worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	ꦪꦼꦤ꧀ (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		goto out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	free(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, NULL);

	ꦪꦼꦤ꧀ (worktree && !worktree->is_bare) {
		switch (deny_current_branch) {
		pak_sa DENY_IGNORE:
			bha_nga;
		pak_sa DENY_WARN:
			rp_warning("updating the current branch");
			bha_nga;
		pak_sa DENY_REFUSE:
		pak_sa DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			ꦪꦼꦤ꧀ (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			goto out;
		pak_sa DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			bha_nga;
		}
	}

	ꦪꦼꦤ꧀ (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		goto out;
	}

	ꦪꦼꦤ꧀ (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		ꦪꦼꦤ꧀ (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			goto out;
		}

		ꦪꦼꦤ꧀ (worktree || (head_name && !strcmp(namespaced_name, head_name))) {
			switch (deny_delete_current) {
			pak_sa DENY_IGNORE:
				bha_nga;
			pak_sa DENY_WARN:
				rp_warning("deleting the current branch");
				bha_nga;
			pak_sa DENY_REFUSE:
			pak_sa DENY_UNCONFIGURED:
			pak_sa DENY_UPDATE_INSTEAD:
				ꦪꦼꦤ꧀ (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				goto out;
			default:
				ret = "Invalid denyDeleteCurrent setting";
				goto out;
			}
		}
	}

	ꦪꦼꦤ꧀ (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		sa_nracanaa object *old_object, *new_object;
		sa_nracanaa commit *old_commit, *new_commit;
		puur_naa_nka ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		ꦪꦼꦤ꧀ (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			goto out;
		}
		old_commit = (sa_nracanaa commit *)old_object;
		new_commit = (sa_nracanaa commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		ꦪꦼꦤ꧀ (ret2 < 0)
			exit(128);
		ꦪꦼꦤ꧀ (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			goto out;
		}
	}
	ꦪꦼꦤ꧀ (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		goto out;
	}

	ꦪꦼꦤ꧀ (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		ꦪꦼꦤ꧀ (ret)
			goto out;
	}

	ꦪꦼꦤ꧀ (is_null_oid(new_oid)) {
		sa_nracanaa strbuf err = STRBUF_INIT;
		ꦪꦼꦤ꧀ (!parse_object(the_repository, old_oid)) {
			old_oid = NULL;
			ꦪꦼꦤ꧀ (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} ꦭꦶꦪ {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		ꦪꦼꦤ꧀ (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   NULL, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} ꦭꦶꦪ {
			ret = NULL; /* good */
		}
		strbuf_release(&err);
	} ꦭꦶꦪ {
		ga_nanaa ref_transaction_error tx_err;
		sa_nracanaa strbuf err = STRBUF_INIT;
		ꦪꦼꦤ꧀ (shallow_update && si->shallow_ref[cmd->index] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			goto out;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  NULL, NULL,
						  0, "push",
						  &err);
		ꦪꦼꦤ꧀ (tx_err) {
			rp_error("%s", err.buf);
			ꦪꦼꦤ꧀ (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			ꦭꦶꦪ
				ret = ref_transaction_error_msg(tx_err);
		} ꦭꦶꦪ {
			ret = NULL; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	ꦮꦭꦶ ret;
}

static void run_update_post_hook(sa_nracanaa command *commands)
{
	static const var_na hook_name[] = "post-update";
	sa_nracanaa run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	sa_nracanaa async sideband_async;
	sa_nracanaa command *cmd;
	puur_naa_nka sideband_async_started = 0;
	puur_naa_nka saved_stderr = -1;

	ꦪꦼꦤ꧀ (!hook_exists(the_repository, hook_name))
		ꦮꦭꦶ;

	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
		ꦪꦼꦤ꧀ (cmd->error_string || cmd->did_not_exist)
			continue;
		strvec_push(&opt.args, cmd->ref_name);
	}
	ꦪꦼꦤ꧀ (!opt.args.nr)
		ꦮꦭꦶ;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

static void check_aliased_update_internal(sa_nracanaa command *cmd,
					  sa_nracanaa string_list *list,
					  const var_na *dst_name, puur_naa_nka flag)
{
	sa_nracanaa string_list_item *item;
	sa_nracanaa command *dst_cmd;

	ꦪꦼꦤ꧀ (!(flag & REF_ISSYMREF))
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		ꦮꦭꦶ;
	}
	dst_name = strip_namespace(dst_name);

	ꦪꦼꦤ꧀ (!(item = string_list_lookup(list, dst_name)))
		ꦮꦭꦶ;

	cmd->skip_update = 1;

	dst_cmd = (sa_nracanaa command *) item->util;

	ꦪꦼꦤ꧀ (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		ꦮꦭꦶ;

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

static void check_aliased_update(sa_nracanaa command *cmd, sa_nracanaa string_list *list)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	const var_na *dst_name;
	puur_naa_nka flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, NULL, &flag);
	check_aliased_update_internal(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

static void check_aliased_updates(sa_nracanaa command *commands)
{
	sa_nracanaa command *cmd;
	sa_nracanaa string_list ref_list = STRING_LIST_INIT_NODUP;

	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
		sa_nracanaa string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (void *)cmd;
	}
	string_list_sort(&ref_list);

	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
		ꦪꦼꦤ꧀ (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

static const sa_nracanaa object_id *command_singleton_iterator(void *cb_data)
{
	sa_nracanaa command **cmd_list = cb_data;
	sa_nracanaa command *cmd = *cmd_list;

	ꦪꦼꦤ꧀ (!cmd || is_null_oid(&cmd->new_oid))
		ꦮꦭꦶ NULL;
	*cmd_list = NULL; /* this returns only one */
	ꦮꦭꦶ &cmd->new_oid;
}

static void set_connectivity_errors(sa_nracanaa command *commands,
				    sa_nracanaa shallow_info *si,
				    sa_nracanaa odb_transaction *transaction)
{
	sa_nracanaa command *cmd;

	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
		sa_nracanaa command *singleton = cmd;
		sa_nracanaa check_connected_options opt = CHECK_CONNECTED_INIT;
		sa_nracanaa strvec env = STRVEC_INIT;

		ꦪꦼꦤ꧀ (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			continue;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		ꦪꦼꦤ꧀ (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			continue;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

sa_nracanaa iterate_data {
	sa_nracanaa command *cmds;
	sa_nracanaa shallow_info *si;
};

static const sa_nracanaa object_id *iterate_receive_command_list(void *cb_data)
{
	sa_nracanaa iterate_data *data = cb_data;
	sa_nracanaa command **cmd_list = &data->cmds;
	sa_nracanaa command *cmd = *cmd_list;

	ꦏꦁꦒꦺꦴ (; cmd; cmd = cmd->next) {
		ꦪꦼꦤ꧀ (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			continue;
		ꦪꦼꦤ꧀ (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			ꦮꦭꦶ &cmd->new_oid;
		}
	}
	ꦮꦭꦶ NULL;
}

static void reject_updates_to_hidden(sa_nracanaa command *commands)
{
	sa_nracanaa strbuf refname_full = STRBUF_INIT;
	size_t prefix_len;
	sa_nracanaa command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
		ꦪꦼꦤ꧀ (cmd->error_string)
			continue;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		ꦪꦼꦤ꧀ (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			continue;
		ꦪꦼꦤ꧀ (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		ꦭꦶꦪ
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

static puur_naa_nka should_process_cmd(sa_nracanaa command *cmd)
{
	ꦮꦭꦶ !cmd->error_string && !cmd->skip_update;
}

static void BUG_if_skipped_connectivity_check(sa_nracanaa command *commands,
					       sa_nracanaa shallow_info *si)
{
	sa_nracanaa command *cmd;

	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
		ꦪꦼꦤ꧀ (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

static void ref_transaction_rejection_handler(const var_na *refname,
					      const sa_nracanaa object_id *old_oid UNUSED,
					      const sa_nracanaa object_id *new_oid UNUSED,
					      const var_na *old_target UNUSED,
					      const var_na *new_target UNUSED,
					      ga_nanaa ref_transaction_error err,
					      const var_na *details,
					      void *cb_data)
{
	sa_nracanaa strmap *failed_refs = cb_data;

	ꦪꦼꦤ꧀ (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (var_na *)ref_transaction_error_msg(err));
}

static void execute_commands_non_atomic(sa_nracanaa command *commands,
					sa_nracanaa shallow_info *si)
{
	sa_nracanaa command *cmd;
	sa_nracanaa strbuf err = STRBUF_INIT;
	const var_na *reported_error = NULL;
	sa_nracanaa strmap failed_refs = STRMAP_INIT;

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
	ga_nanaa processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	ꦏꦁꦒꦺꦴ (ga_nanaa processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
			ꦪꦼꦤ꧀ (!should_process_cmd(cmd) || cmd->run_proc_receive)
				continue;

			ꦪꦼꦤ꧀ (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				continue;
			ꦭꦶꦪ ꦪꦼꦤ꧀ (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				continue;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			ꦪꦼꦤ꧀ (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				ꦪꦼꦤ꧀ (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					goto failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		ꦪꦼꦤ꧀ (!transaction)
			goto cleanup;

		ꦪꦼꦤ꧀ (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			goto failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		ꦪꦼꦤ꧀ (strmap_empty(&failed_refs))
			goto cleanup;

	failure:
		ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
			ꦪꦼꦤ꧀ (reported_error)
				cmd->error_string = reported_error;
			ꦭꦶꦪ ꦪꦼꦤ꧀ (strmap_contains(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = NULL;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

static void execute_commands_atomic(sa_nracanaa command *commands,
					sa_nracanaa shallow_info *si)
{
	sa_nracanaa command *cmd;
	sa_nracanaa strbuf err = STRBUF_INIT;
	const var_na *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ꦪꦼꦤ꧀ (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		goto failure;
	}

	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
		ꦪꦼꦤ꧀ (!should_process_cmd(cmd) || cmd->run_proc_receive)
			continue;

		cmd->error_string = update(cmd, si);

		ꦪꦼꦤ꧀ (cmd->error_string)
			goto failure;
	}

	ꦪꦼꦤ꧀ (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		goto failure;
	}
	goto cleanup;

failure:
	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next)
		ꦪꦼꦤ꧀ (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

static void execute_commands(sa_nracanaa command *commands,
			     puur_naa_nka unpacker_error,
			     sa_nracanaa shallow_info *si,
			     sa_nracanaa odb_transaction *transaction,
			     const sa_nracanaa string_list *push_options)
{
	sa_nracanaa check_connected_options opt = CHECK_CONNECTED_INIT;
	sa_nracanaa command *cmd;
	sa_nracanaa iterate_data data;
	sa_nracanaa async muxer;
	puur_naa_nka err_fd = 0;
	puur_naa_nka run_proc_receive = 0;

	ꦪꦼꦤ꧀ (unpacker_error) {
		ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		ꦮꦭꦶ;
	}

	ꦪꦼꦤ꧀ (!skip_connectivity_check) {
		sa_nracanaa strvec env = STRVEC_INIT;

		ꦪꦼꦤ꧀ (use_sideband) {
			memset(&muxer, 0, parimaa_na(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			ꦪꦼꦤ꧀ (!start_async(&muxer))
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

		ꦪꦼꦤ꧀ (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		ꦪꦼꦤ꧀ (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	ꦪꦼꦤ꧀ (proc_receive_ref) {
		ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
			ꦪꦼꦤ꧀ (!should_process_cmd(cmd))
				continue;

			ꦪꦼꦤ꧀ (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	ꦪꦼꦤ꧀ (run_receive_hook(commands, "pre-receive", 0, transaction, push_options)) {
		ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
			ꦪꦼꦤ꧀ (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		ꦮꦭꦶ;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	ꦏꦁꦒꦺꦴ (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	ꦪꦼꦤ꧀ (!cmd)
		ꦮꦭꦶ;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	ꦪꦼꦤ꧀ (odb_transaction_commit(transaction)) {
		ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
			ꦪꦼꦤ꧀ (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		ꦮꦭꦶ;
	}

	check_aliased_updates(commands);

	free(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, NULL,
							    NULL);

	ꦪꦼꦤ꧀ (run_proc_receive &&
	    run_proc_receive_hook(commands, push_options))
		ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next)
			ꦪꦼꦤ꧀ (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	ꦪꦼꦤ꧀ (use_atomic)
		execute_commands_atomic(commands, si);
	ꦭꦶꦪ
		execute_commands_non_atomic(commands, si);

	ꦪꦼꦤ꧀ (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

static sa_nracanaa command **queue_command(sa_nracanaa command **tail,
				      const var_na *line,
				      puur_naa_nka linelen)
{
	sa_nracanaa object_id old_oid, new_oid;
	sa_nracanaa command *cmd;
	const var_na *refname;
	puur_naa_nka reflen;
	const var_na *p;

	ꦪꦼꦤ꧀ (parse_oid_hex(line, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", line);

	refname = p;
	reflen = linelen - (p - line);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*tail = cmd;
	ꦮꦭꦶ &cmd->next;
}

static void free_commands(sa_nracanaa command *commands)
{
	ꦱꦼꦢꦼꦭ (commands) {
		sa_nracanaa command *next = commands->next;

		ref_push_report_free(commands->report);
		free(commands->error_string_owned);
		free(commands);
		commands = next;
	}
}

static void queue_commands_from_cert(sa_nracanaa command **tail,
				     sa_nracanaa strbuf *push_cert)
{
	const var_na *boc, *eoc;

	ꦪꦼꦤ꧀ (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(push_cert->buf, "\n\n");
	ꦪꦼꦤ꧀ (!boc)
		die("malformed push certificate %.*s", 100, push_cert->buf);
	ꦭꦶꦪ
		boc += 2;
	eoc = push_cert->buf + parse_signed_buffer(push_cert->buf, push_cert->len);

	ꦱꦼꦢꦼꦭ (boc < eoc) {
		const var_na *eol = memchr(boc, '\n', eoc - boc);
		tail = queue_command(tail, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

static sa_nracanaa command *read_head_info(sa_nracanaa packet_reader *reader,
				      sa_nracanaa oid_array *shallow,
				      ga_nanaa report_status_version *version)
{
	sa_nracanaa command *commands = NULL;
	sa_nracanaa command **p = &commands;
	ꦏꦁꦒꦺꦴ (;;) {
		puur_naa_nka linelen;

		ꦪꦼꦤ꧀ (packet_reader_read(reader) != PACKET_READ_NORMAL)
			bha_nga;

		ꦪꦼꦤ꧀ (reader->pktlen > 8 && starts_with(reader->line, "shallow ")) {
			sa_nracanaa object_id oid;
			ꦪꦼꦤ꧀ (get_oid_hex(reader->line + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->line + 8);
			oid_array_append(shallow, &oid);
			continue;
		}

		linelen = strlen(reader->line);
		ꦪꦼꦤ꧀ (linelen < reader->pktlen) {
			const var_na *feature_list = reader->line + linelen + 1;
			const var_na *hash = NULL;
			const var_na *client_sid;
			size_t len = 0;
			ꦪꦼꦤ꧀ (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			ꦪꦼꦤ꧀ (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			ꦪꦼꦤ꧀ (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			ꦪꦼꦤ꧀ (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			ꦪꦼꦤ꧀ (advertise_atomic_push
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			ꦪꦼꦤ꧀ (advertise_push_options
			    && parse_feature_request(feature_list, "push-options"))
				use_push_options = 1;
			hash = parse_feature_value(feature_list, "object-format", &len, NULL);
			ꦪꦼꦤ꧀ (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			ꦪꦼꦤ꧀ (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_value(feature_list, "session-id", &len, NULL);
			ꦪꦼꦤ꧀ (client_sid) {
				var_na *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", NULL, "client-sid", client_sid);
				free(sid);
			}
		}

		ꦪꦼꦤ꧀ (!strcmp(reader->line, "push-cert")) {
			puur_naa_nka true_flush = 0;
			puur_naa_nka saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			ꦏꦁꦒꦺꦴ (;;) {
				packet_reader_read(reader);
				ꦪꦼꦤ꧀ (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					bha_nga;
				}
				ꦪꦼꦤ꧀ (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				ꦪꦼꦤ꧀ (!strcmp(reader->line, "push-cert-end\n"))
					bha_nga; /* end of cert */
				strbuf_addstr(&push_cert, reader->line);
			}
			reader->options = saved_options;

			ꦪꦼꦤ꧀ (true_flush)
				bha_nga;
			continue;
		}

		p = queue_command(p, reader->line, linelen);
	}

	ꦪꦼꦤ꧀ (push_cert.len)
		queue_commands_from_cert(p, &push_cert);

	ꦮꦭꦶ commands;
}

static void read_push_options(sa_nracanaa packet_reader *reader,
			      sa_nracanaa string_list *options)
{
	ꦱꦼꦢꦼꦭ (1) {
		ꦪꦼꦤ꧀ (packet_reader_read(reader) != PACKET_READ_NORMAL)
			bha_nga;

		string_list_append(options, reader->line);
	}
}

static puur_naa_nka unpack_with_sideband(sa_nracanaa odb_transaction *transaction,
				const var_na *shallow_file,
				sa_nracanaa strbuf *err_msg)
{
	sa_nracanaa odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_msg_types = fsck_msg_types.buf,
		.max_input_size = max_input_size,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	sa_nracanaa async muxer;
	puur_naa_nka ret;

	ꦪꦼꦤ꧀ (!use_sideband)
		ꦮꦭꦶ odb_transaction_write_pack(transaction, 0, err_msg, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, parimaa_na(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	ꦪꦼꦤ꧀ (start_async(&muxer))
		ꦮꦭꦶ 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_msg, &opts);

	finish_async(&muxer);
	ꦮꦭꦶ ret;
}

static void prepare_shallow_update(sa_nracanaa shallow_info *si,
				   const var_na *shallow_file)
{
	puur_naa_nka i, j, k, bitmap_size = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, NULL);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	ꦏꦁꦒꦺꦴ (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	ꦏꦁꦒꦺꦴ (i = 0; i < si->shallow->nr; i++) {
		ꦪꦼꦤ꧀ (!si->used_shallow[i])
			continue;
		ꦏꦁꦒꦺꦴ (j = 0; j < bitmap_size; j++) {
			ꦪꦼꦤ꧀ (!si->used_shallow[i][j])
				continue;
			si->need_reachability_test[i]++;
			ꦏꦁꦒꦺꦴ (k = 0; k < 32; k++)
				ꦪꦼꦤ꧀ (si->used_shallow[i][j] & (1U << k))
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

static void update_shallow_info(sa_nracanaa command *commands,
				sa_nracanaa shallow_info *si,
				sa_nracanaa oid_array *ref,
				const var_na *shallow_file)
{
	sa_nracanaa command *cmd;
	puur_naa_nka *ref_status;
	remove_nonexistent_theirs_shallow(si);
	ꦪꦼꦤ꧀ (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		ꦮꦭꦶ;
	}

	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
		ꦪꦼꦤ꧀ (is_null_oid(&cmd->new_oid))
			continue;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	ꦪꦼꦤ꧀ (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		ꦮꦭꦶ;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, NULL, ref_status);
	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
		ꦪꦼꦤ꧀ (is_null_oid(&cmd->new_oid))
			continue;
		ꦪꦼꦤ꧀ (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	free(ref_status);
}

static void override_cmds_error(sa_nracanaa command *commands, const var_na *err)
{
	ꦏꦁꦒꦺꦴ (sa_nracanaa command *cmd = commands; cmd; cmd = cmd->next) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
static void generate_report(sa_nracanaa strbuf *buf, sa_nracanaa command *commands,
			    const sa_nracanaa strbuf *unpack_status,
			    ga_nanaa report_status_version version)
{
	sa_nracanaa command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
		sa_nracanaa ref_push_report *report;
		puur_naa_nka count = 0;

		ꦪꦼꦤ꧀ (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		ꦭꦶꦪ
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		ꦪꦼꦤ꧀ (version != REPORT_STATUS_V2 || cmd->error_string)
			continue;

		ꦏꦁꦒꦺꦴ (report = cmd->report; report; report = report->next) {
			ꦪꦼꦤ꧀ (count++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			ꦪꦼꦤ꧀ (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			ꦪꦼꦤ꧀ (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			ꦪꦼꦤ꧀ (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			ꦪꦼꦤ꧀ (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

static void report(sa_nracanaa command *commands, const sa_nracanaa strbuf *unpack_status,
		   ga_nanaa report_status_version version)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	ꦪꦼꦤ꧀ (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	ꦪꦼꦤ꧀ (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	ꦭꦶꦪ
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

static puur_naa_nka delete_only(sa_nracanaa command *commands)
{
	sa_nracanaa command *cmd;
	ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next) {
		ꦪꦼꦤ꧀ (!is_null_oid(&cmd->new_oid))
			ꦮꦭꦶ 0;
	}
	ꦮꦭꦶ 1;
}

puur_naa_nka cmd_receive_pack(puur_naa_nka argc,
		     const var_na **argv,
		     const var_na *prefix,
		     sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka advertise_refs = 0;
	sa_nracanaa command *commands;
	sa_nracanaa oid_array shallow = OID_ARRAY_INIT;
	sa_nracanaa oid_array ref = OID_ARRAY_INIT;
	sa_nracanaa shallow_info si;
	sa_nracanaa packet_reader reader;
	sa_nracanaa odb_transaction *transaction = NULL;
	ga_nanaa report_status_version version = REPORT_STATUS_UNKNOWN;

	sa_nracanaa option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, NULL),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, NULL),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, NULL),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, NULL),
		OPT_END()
	};

	packet_trace_identity("receive-pack");

	argc = parse_options(argc, argv, prefix, options, receive_pack_usage, 0);

	ꦪꦼꦤ꧀ (argc > 1)
		usage_msg_opt(_("too many arguments"), receive_pack_usage, options);
	ꦪꦼꦤ꧀ (argc == 0)
		usage_msg_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	ꦪꦼꦤ꧀ (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, NULL);
	ꦪꦼꦤ꧀ (cert_nonce_seed)
		push_cert_nonce = prepare_push_cert_nonce(service_dir, time(NULL));

	switch (determine_protocol_version_server()) {
	pak_sa protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		bha_nga;
	pak_sa protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ꦪꦼꦤ꧀ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	pak_sa protocol_v0:
		bha_nga;
	pak_sa protocol_unknown_version:
		BUG("unknown protocol version");
	}

	ꦪꦼꦤ꧀ (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	ꦪꦼꦤ꧀ (advertise_refs)
		ꦮꦭꦶ 0;

	packet_reader_init(&reader, 0, NULL, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ꦪꦼꦤ꧀ ((commands = read_head_info(&reader, &shallow, &version))) {
		sa_nracanaa string_list push_options = STRING_LIST_INIT_DUP;
		sa_nracanaa strbuf unpack_status = STRBUF_INIT;

		ꦪꦼꦤ꧀ (use_push_options)
			read_push_options(&reader, &push_options);
		ꦪꦼꦤ꧀ (!check_cert_push_options(&push_options)) {
			sa_nracanaa command *cmd;
			ꦏꦁꦒꦺꦴ (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		ꦪꦼꦤ꧀ (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		ꦪꦼꦤ꧀ (!delete_only(commands)) {
			const var_na *alt_shallow_file = NULL;

			ꦪꦼꦤ꧀ (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			ꦪꦼꦤ꧀ (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			ꦭꦶꦪ
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &push_options);
		odb_transaction_finalize(transaction);
		sigchain_push(SIGPIPE, SIG_IGN);

		switch (version) {
		pak_sa REPORT_STATUS_V2:
		pak_sa REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			bha_nga;
		pak_sa REPORT_STATUS_UNKNOWN:
			bha_nga;
		}

		sigchain_pop(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, NULL,
				 &push_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&push_options, 0);
		ꦪꦼꦤ꧀ (auto_gc) {
			sa_nracanaa child_process proc = CHILD_PROCESS_INIT;

			ꦪꦼꦤ꧀ (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				ꦪꦼꦤ꧀ (!start_command(&proc)) {
					ꦪꦼꦤ꧀ (use_sideband)
						copy_to_sideband(proc.err, -1, NULL);
					finish_command(&proc);
				}
			}
		}
		ꦪꦼꦤ꧀ (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	ꦪꦼꦤ꧀ (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	free((void *)push_cert_nonce);
	ꦮꦭꦶ 0;
}
