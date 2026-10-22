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

static ចំនួនគត់ deny_deletes;
static ចំនួនគត់ deny_non_fast_forwards;
static ga_nanaa deny_action deny_current_branch = DENY_UNCONFIGURED;
static ga_nanaa deny_action deny_delete_current = DENY_UNCONFIGURED;
static ចំនួនគត់ receive_fsck_objects = -1;
static ចំនួនគត់ transfer_fsck_objects = -1;
static sa_nracanaa strbuf fsck_msg_types = STRBUF_INIT;
static ចំនួនគត់ advertise_atomic_push = 1;
static ចំនួនគត់ advertise_push_options;
static ចំនួនគត់ advertise_sid;
static off_t max_input_size;
static ចំនួនគត់ use_sideband;
static ចំនួនគត់ use_atomic;
static ចំនួនគត់ use_push_options;
static ចំនួនគត់ quiet;
static ចំនួនគត់ prefer_ofs_delta = 1;
static ចំនួនគត់ auto_update_server_info;
static ចំនួនគត់ auto_gc = 1;
static ចំនួនគត់ reject_thin;
static ចំនួនគត់ skip_connectivity_check;
static ចំនួនគត់ stateless_rpc;
static const var_na *service_dir;
static const var_na *head_name;
static void *head_name_to_free;
static ចំនួនគត់ sent_capabilities;
static ចំនួនគត់ shallow_update;
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
static ចំនួនគត់ keepalive_in_sec = 5;

static sa_nracanaa proc_receive_ref {
	unsigned ចំនួនគត់ want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	var_na *ref_prefix;
	sa_nracanaa proc_receive_ref *next;
} *proc_receive_ref;

static void proc_receive_ref_append(const var_na *prefix);

static ga_nanaa deny_action parse_deny_action(const var_na *var, const var_na *value)
{
	ប្រសិនបើ (value) {
		ប្រសិនបើ (!strcasecmp(value, "ignore"))
			ត្រឡប់ DENY_IGNORE;
		ប្រសិនបើ (!strcasecmp(value, "warn"))
			ត្រឡប់ DENY_WARN;
		ប្រសិនបើ (!strcasecmp(value, "refuse"))
			ត្រឡប់ DENY_REFUSE;
		ប្រសិនបើ (!strcasecmp(value, "updateinstead"))
			ត្រឡប់ DENY_UPDATE_INSTEAD;
	}
	ប្រសិនបើ (git_config_bool(var, value))
		ត្រឡប់ DENY_REFUSE;
	ត្រឡប់ DENY_IGNORE;
}

static ចំនួនគត់ receive_pack_config(const var_na *var, const var_na *value,
			       const sa_nracanaa config_context *ctx, void *cb)
{
	const var_na *msg_id;
	ចំនួនគត់ status = parse_hide_refs_config(var, value, "receive", &hidden_refs);

	ប្រសិនបើ (status)
		ត្រឡប់ status;

	ប្រសិនបើ (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.fsck.skiplist") == 0) {
		var_na *path;

		ប្រសិនបើ (git_config_pathname(&path, var, value))
			ត្រឡប់ -1;
		ប្រសិនបើ (path)
			strbuf_addf(&fsck_msg_types, "%cskiplist=%s",
				    fsck_msg_types.len ? ',' : '=', path);
		free(path);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (skip_prefix(var, "receive.fsck.", &msg_id)) {
		ប្រសិនបើ (!value)
			ត្រឡប់ config_error_nonbool(var);
		ប្រសិនបើ (is_valid_msg_type(msg_id, value))
			strbuf_addf(&fsck_msg_types, "%c%s=%s",
				fsck_msg_types.len ? ',' : '=', msg_id, value);
		បើមិនដូច្នេះ
			warning("skipping unknown msg id '%s'", msg_id);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.certnonceseed") == 0)
		ត្រឡប់ git_config_string(&cert_nonce_seed, var, value);

	ប្រសិនបើ (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, value, ctx->kvi);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_push = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_push_options = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, value, ctx->kvi);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.maxinputsize") == 0) {
		max_input_size = git_config_int64(var, value, ctx->kvi);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "receive.procreceiverefs") == 0) {
		ប្រសិនបើ (!value)
			ត្រឡប់ config_error_nonbool(var);
		proc_receive_ref_append(value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ត្រឡប់ git_default_config(var, value, ctx, cb);
}

static void show_ref(const var_na *path, const sa_nracanaa object_id *oid)
{
	ប្រសិនបើ (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} បើមិនដូច្នេះ {
		sa_nracanaa strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		ប្រសិនបើ (advertise_atomic_push)
			strbuf_addstr(&cap, " atomic");
		ប្រសិនបើ (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		ប្រសិនបើ (push_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", push_cert_nonce);
		ប្រសិនបើ (advertise_push_options)
			strbuf_addstr(&cap, " push-options");
		ប្រសិនបើ (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

static ចំនួនគត់ show_ref_cb(const sa_nracanaa reference *ref, void *data)
{
	sa_nracanaa oidset *seen = data;
	const var_na *path = strip_namespace(ref->name);

	ប្រសិនបើ (ref_is_hidden(path, ref->name, &hidden_refs))
		ត្រឡប់ 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	ប្រសិនបើ (!path) {
		ប្រសិនបើ (oidset_insert(seen, ref->oid))
			ត្រឡប់ 0;
		path = ".have";
	} បើមិនដូច្នេះ {
		oidset_insert(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	ត្រឡប់ 0;
}

static void show_one_alternate_ref(const sa_nracanaa object_id *oid,
				   void *data)
{
	sa_nracanaa oidset *seen = data;

	ប្រសិនបើ (oidset_insert(seen, oid))
		ត្រឡប់;

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

	ប្រសិនបើ (!sent_capabilities)
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
	unsigned ចំនួនគត់ skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	ចំនួនគត់ index;
	sa_nracanaa object_id old_oid;
	sa_nracanaa object_id new_oid;
	var_na ref_name[FLEX_ARRAY]; /* more */
};

static void proc_receive_ref_append(const var_na *prefix)
{
	sa_nracanaa proc_receive_ref *ref_pattern;
	const var_na *p;
	ចំនួនគត់ len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = strchr(prefix, ':');
	ប្រសិនបើ (p) {
		ខណៈពេល (prefix < p) {
			ប្រសិនបើ (*prefix == 'a')
				ref_pattern->want_add = 1;
			បើមិនដូច្នេះ ប្រសិនបើ (*prefix == 'd')
				ref_pattern->want_delete = 1;
			បើមិនដូច្នេះ ប្រសិនបើ (*prefix == 'm')
				ref_pattern->want_modify = 1;
			បើមិនដូច្នេះ ប្រសិនបើ (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} បើមិនដូច្នេះ {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	ខណៈពេល (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	ប្រសិនបើ (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} បើមិនដូច្នេះ {
		sa_nracanaa proc_receive_ref *end;

		end = proc_receive_ref;
		ខណៈពេល (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

static ចំនួនគត់ proc_receive_ref_matches(sa_nracanaa command *cmd)
{
	sa_nracanaa proc_receive_ref *p;

	ប្រសិនបើ (!proc_receive_ref)
		ត្រឡប់ 0;

	សម្រាប់ (p = proc_receive_ref; p; p = p->next) {
		const var_na *match = p->ref_prefix;
		const var_na *remains;

		ប្រសិនបើ (!p->want_add && is_null_oid(&cmd->old_oid))
			continue;
		បើមិនដូច្នេះ ប្រសិនបើ (!p->want_delete && is_null_oid(&cmd->new_oid))
			continue;
		បើមិនដូច្នេះ ប្រសិនបើ (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			continue;

		ប្រសិនបើ (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			ប្រសិនបើ (!p->negative_ref)
				ត្រឡប់ 1;
		} បើមិនដូច្នេះ ប្រសិនបើ (p->negative_ref) {
			ត្រឡប់ 1;
		}
	}
	ត្រឡប់ 0;
}

static void report_message(const var_na *prefix, const var_na *err, va_list params)
{
	ចំនួនគត់ sz;
	var_na msg[4096];

	sz = xsnprintf(msg, parimaa_na(msg), "%s", prefix);
	sz += vsnprintf(msg + sz, parimaa_na(msg) - sz, err, params);
	ប្រសិនបើ (sz > (parimaa_na(msg) - 1))
		sz = parimaa_na(msg) - 1;
	msg[sz++] = '\n';

	ប្រសិនបើ (use_sideband)
		send_sideband(1, 2, msg, sz, use_sideband);
	បើមិនដូច្នេះ
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

static ចំនួនគត់ copy_to_sideband(ចំនួនគត់ in, ចំនួនគត់ out UNUSED, void *arg UNUSED)
{
	var_na data[128];
	ចំនួនគត់ keepalive_active = 0;

	ប្រសិនបើ (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	ប្រសិនបើ (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	ខណៈពេល (1) {
		ssize_t sz;

		ប្រសិនបើ (keepalive_active) {
			sa_nracanaa pollfd pfd;
			ចំនួនគត់ ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			ប្រសិនបើ (ret < 0) {
				ប្រសិនបើ (errno == EINTR)
					continue;
				បើមិនដូច្នេះ
					bha_nga;
			} បើមិនដូច្នេះ ប្រសិនបើ (ret == 0) {
				/* no data; send a keepalive packet */
				static const var_na buf[] = "0005\1";
				write_or_die(1, buf, parimaa_na(buf) - 1);
				continue;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, parimaa_na(data));
		ប្រសិនបើ (sz <= 0)
			bha_nga;

		ប្រសិនបើ (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			const var_na *p = memchr(data, '\0', sz);
			ប្រសិនបើ (p) {
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
	ត្រឡប់ 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
static void prepare_sideband_async(sa_nracanaa async *sideband_async, ចំនួនគត់ *saved_stderr, ចំនួនគត់ *started)
{
	*started = 0;

	ប្រសិនបើ (!use_sideband)
		ត្រឡប់;

	memset(sideband_async, 0, parimaa_na(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	ប្រសិនបើ (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		ប្រសិនបើ (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
static void finish_sideband_async(sa_nracanaa async *sideband_async, ចំនួនគត់ saved_stderr, ចំនួនគត់ started)
{
	ប្រសិនបើ (!use_sideband)
		ត្រឡប់;

	ប្រសិនបើ (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	ប្រសិនបើ (started)
		finish_async(sideband_async);
}

static void hmac_hash(unsigned var_na *out,
		      const var_na *key_in, size_t key_len,
		      const var_na *text, size_t text_len)
{
	unsigned var_na key[GIT_MAX_BLKSZ];
	unsigned var_na k_ipad[GIT_MAX_BLKSZ];
	unsigned var_na k_opad[GIT_MAX_BLKSZ];
	ចំនួនគត់ i;
	sa_nracanaa git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(key, '\0', GIT_MAX_BLKSZ);
	ប្រសិនបើ (the_hash_algo->blksz < key_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, key_in, key_len);
		git_hash_final(key, &ctx);
	} បើមិនដូច្នេះ {
		memcpy(key, key_in, key_len);
	}

	/* RFC 2104 2. (2) & (5) */
	សម្រាប់ (i = 0; i < parimaa_na(key); i++) {
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
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (ចំនួនគត់)the_hash_algo->hexsz, hash_to_hex(hash));
	ត្រឡប់ strbuf_detach(&buf, NULL);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
static ចំនួនគត់ constant_memequal(const var_na *a, const var_na *b, size_t n)
{
	ចំនួនគត់ res = 0;
	size_t i;

	សម្រាប់ (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	ត្រឡប់ res;
}

static const var_na *check_nonce(const var_na *buf)
{
	size_t noncelen;
	const var_na *found = find_commit_header(buf, "nonce", &noncelen);
	var_na *nonce = found ? xmemdupz(found, noncelen) : NULL;
	timestamp_t stamp, ostamp;
	var_na *bohmac, *expect = NULL;
	const var_na *retval = NONCE_BAD;

	ប្រសិនបើ (!nonce) {
		retval = NONCE_MISSING;
		goto leave;
	} បើមិនដូច្នេះ ប្រសិនបើ (!push_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		goto leave;
	} បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(push_cert_nonce, nonce)) {
		retval = NONCE_OK;
		goto leave;
	}

	ប្រសិនបើ (!stateless_rpc) {
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
	ប្រសិនបើ (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		goto leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	ប្រសិនបើ (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		goto leave;
	}

	expect = prepare_push_cert_nonce(service_dir, stamp);
	ប្រសិនបើ (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		goto leave;
	}
	ប្រសិនបើ (constant_memequal(expect, nonce, noncelen)) {
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

	ប្រសិនបើ (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		free((void *)push_cert_nonce);
		push_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} បើមិនដូច្នេះ {
		retval = NONCE_SLOP;
	}

leave:
	free(nonce);
	free(expect);
	ត្រឡប់ retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
static ចំនួនគត់ check_cert_push_options(const sa_nracanaa string_list *push_options)
{
	const var_na *buf = push_cert.buf;

	const var_na *option;
	size_t optionlen;
	ចំនួនគត់ options_seen = 0;

	ចំនួនគត់ retval = 1;

	ប្រសិនបើ (!*buf)
		ត្រឡប់ 1;

	ខណៈពេល ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		ប្រសិនបើ (options_seen > push_options->nr
		    || xstrncmpz(push_options->items[options_seen - 1].string,
				 option, optionlen))
			ត្រឡប់ 0;
	}

	ប្រសិនបើ (options_seen != push_options->nr)
		retval = 0;

	ត្រឡប់ retval;
}

static void prepare_push_cert_sha1(sa_nracanaa run_hooks_opt *opt)
{
	static ចំនួនគត់ already_done;

	ប្រសិនបើ (!push_cert.len)
		ត្រឡប់;

	ប្រសិនបើ (!already_done) {
		ចំនួនគត់ bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		ប្រសិនបើ (odb_write_object(the_repository->objects, push_cert.buf,
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
	ប្រសិនបើ (!is_null_oid(&push_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&push_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.key ? sigcheck.key : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		ប្រសិនបើ (push_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     push_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			ប្រសិនបើ (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

sa_nracanaa receive_hook_feed_state {
	sa_nracanaa command *cmd;
	sa_nracanaa ref_push_report *report;
	ចំនួនគត់ skip_broken;
	sa_nracanaa strbuf buf;
};

static ចំនួនគត់ feed_receive_hook_cb(ចំនួនគត់ hook_stdin_fd, void *pp_cb UNUSED, void *pp_task_cb)
{
	sa_nracanaa receive_hook_feed_state *state = pp_task_cb;
	sa_nracanaa command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	ខណៈពេល (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	ប្រសិនបើ (!cmd)
		ត្រឡប់ 1;  /* no more commands left */

	ប្រសិនបើ (!state->report)
		state->report = cmd->report;

	ប្រសិនបើ (state->report) {
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
		ប្រសិនបើ (!state->report)
			cmd = cmd->next;
	} បើមិនដូច្នេះ {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	ប្រសិនបើ (state->buf.len > 0) {
		ចំនួនគត់ ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		ប្រសិនបើ (ret < 0) {
			ប្រសិនបើ (errno == EPIPE)
				ត្រឡប់ 1; /* child closed pipe */
			ត្រឡប់ ret;
		}
	}

	ត្រឡប់ state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
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

	ត្រឡប់ data;
}

static void receive_hook_feed_state_free(void *data)
{
	sa_nracanaa receive_hook_feed_state *d = data;
	ប្រសិនបើ (!d)
		ត្រឡប់;
	strbuf_release(&d->buf);
	free(d);
}

static ចំនួនគត់ run_receive_hook(sa_nracanaa command *commands,
			    const var_na *hook_name,
			    ចំនួនគត់ skip_broken,
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
	ចំនួនគត់ sideband_async_started = 0;
	ចំនួនគត់ saved_stderr = -1;
	ចំនួនគត់ ret;

	ប្រសិនបើ (!hook_exists(the_repository, hook_name))
		ត្រឡប់ 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	ខណៈពេល (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	ប្រសិនបើ (!iter)
		ត្រឡប់ 0;

	ប្រសិនបើ (push_options) {
		សម្រាប់ (ចំនួនគត់ i = 0; i < push_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     push_options->items[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)push_options->nr);
	} បើមិនដូច្នេះ {
		strvec_push(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	ប្រសិនបើ (transaction)
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

	ត្រឡប់ ret;
}

static ចំនួនគត់ run_update_hook(sa_nracanaa command *cmd)
{
	static const var_na hook_name[] = "update";
	sa_nracanaa run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	sa_nracanaa async sideband_async;
	ចំនួនគត់ sideband_async_started = 0;
	ចំនួនគត់ saved_stderr = -1;
	ចំនួនគត់ code;

	ប្រសិនបើ (!hook_exists(the_repository, hook_name))
		ត្រឡប់ 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     NULL);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ត្រឡប់ code;
}

static ចំនួនគត់ run_receive_report_hook(sa_nracanaa strbuf *report)
{
	sa_nracanaa child_process proc = CHILD_PROCESS_INIT;
	sa_nracanaa async sideband_async;
	ចំនួនគត់ sideband_async_started = 0;
	ចំនួនគត់ saved_stderr = -1;
	sa_nracanaa strbuf out = STRBUF_INIT;
	const var_na *hook_path;
	ចំនួនគត់ ret;

	hook_path = find_hook(the_repository, "receive-report");
	ប្រសិនបើ (!hook_path)
		ត្រឡប់ 0;

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

	ប្រសិនបើ (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	ត្រឡប់ ret;
}

static sa_nracanaa command *find_command_by_refname(sa_nracanaa command *list,
					       const var_na *refname)
{
	សម្រាប់ (; list; list = list->next)
		ប្រសិនបើ (!strcmp(list->ref_name, refname))
			ត្រឡប់ list;
	ត្រឡប់ NULL;
}

static ចំនួនគត់ read_proc_receive_report(sa_nracanaa packet_reader *reader,
				    sa_nracanaa command *commands,
				    sa_nracanaa strbuf *errmsg)
{
	sa_nracanaa command *cmd;
	sa_nracanaa command *hint = NULL;
	sa_nracanaa ref_push_report *report = NULL;
	ចំនួនគត់ new_report = 0;
	ចំនួនគត់ code = 0;
	ចំនួនគត់ once = 0;
	ចំនួនគត់ response = 0;

	សម្រាប់ (;;) {
		sa_nracanaa object_id old_oid, new_oid;
		var_na *head;
		var_na *refname;
		var_na *p;
		ga_nanaa packet_read_status status;

		status = packet_reader_read(reader);
		ប្រសិនបើ (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			ប្រសិនបើ (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				ត្រឡប់ -1;
			}
			bha_nga;
		}
		response++;

		head = reader->line;
		p = strchr(head, ' ');
		ប្រសិនបើ (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			continue;
		}
		*p++ = '\0';
		ប្រសិនបើ (!strcmp(head, "option")) {
			var_na *key;
			const var_na *val;

			ប្រសិនបើ (!hint || !(report || new_report)) {
				ប្រសិនបើ (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				continue;
			}
			ប្រសិនបើ (new_report) {
				ប្រសិនបើ (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} បើមិនដូច្នេះ {
					report = hint->report;
					ខណៈពេល (report->next)
						report = report->next;
					report->next = xcalloc(1, parimaa_na(sa_nracanaa ref_push_report));
					report = report->next;
				}
				new_report = 0;
			}
			key = p;
			p = strchr(key, ' ');
			ប្រសិនបើ (p)
				*p++ = '\0';
			val = p;
			ប្រសិនបើ (!strcmp(key, "refname"))
				report->ref_name = xstrdup_or_null(val);
			បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(key, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(key, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(key, "forced-update"))
				report->forced_update = 1;
			បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(key, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			continue;
		}

		report = NULL;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		ប្រសិនបើ (p)
			*p++ = '\0';
		ប្រសិនបើ (strcmp(head, "ok") && strcmp(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			continue;
		}

		/* first try searching at our hint, falling back to all refs */
		ប្រសិនបើ (hint)
			hint = find_command_by_refname(hint, refname);
		ប្រសិនបើ (!hint)
			hint = find_command_by_refname(commands, refname);
		ប្រសិនបើ (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			continue;
		}
		ប្រសិនបើ (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			continue;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		ប្រសិនបើ (!strcmp(head, "ng")) {
			ប្រសិនបើ (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			បើមិនដូច្នេះ
				hint->error_string = "failed";
			code = -1;
			continue;
		}
		new_report = 1;
	}

	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next)
		ប្រសិនបើ (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	ត្រឡប់ code;
}

static ចំនួនគត់ run_proc_receive_hook(sa_nracanaa command *commands,
				 const sa_nracanaa string_list *push_options)
{
	sa_nracanaa child_process proc = CHILD_PROCESS_INIT;
	sa_nracanaa async muxer;
	sa_nracanaa command *cmd;
	sa_nracanaa packet_reader reader;
	sa_nracanaa strbuf cap = STRBUF_INIT;
	sa_nracanaa strbuf errmsg = STRBUF_INIT;
	ចំនួនគត់ hook_use_push_options = 0;
	ចំនួនគត់ version = 0;
	ចំនួនគត់ code;
	const var_na *hook_path = find_hook(the_repository, "proc-receive");

	ប្រសិនបើ (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		ត្រឡប់ -1;
	}

	strvec_push(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	ប្រសិនបើ (use_sideband) {
		memset(&muxer, 0, parimaa_na(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		ប្រសិនបើ (code)
			ត្រឡប់ code;
		proc.err = muxer.in;
	} បើមិនដូច្នេះ {
		proc.err = 0;
	}

	code = start_command(&proc);
	ប្រសិនបើ (code) {
		ប្រសិនបើ (use_sideband)
			finish_async(&muxer);
		ត្រឡប់ code;
	}

	sigchain_push(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, NULL, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	ប្រសិនបើ (use_atomic)
		strbuf_addstr(&cap, " atomic");
	ប្រសិនបើ (use_push_options)
		strbuf_addstr(&cap, " push-options");
	ប្រសិនបើ (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} បើមិនដូច្នេះ {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	ប្រសិនបើ (!code)
		code = packet_flush_gently(proc.in);

	ប្រសិនបើ (!code)
		សម្រាប់ (;;) {
			ចំនួនគត់ linelen;
			ga_nanaa packet_read_status status;

			status = packet_reader_read(&reader);
			ប្រសិនបើ (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				ប្រសិនបើ (status == PACKET_READ_EOF)
					code = -1;
				bha_nga;
			}

			ប្រសិនបើ (reader.pktlen > 8 && starts_with(reader.line, "version=")) {
				version = atoi(reader.line + 8);
				linelen = strlen(reader.line);
				ប្រសិនបើ (linelen < reader.pktlen) {
					const var_na *feature_list = reader.line + linelen + 1;
					ប្រសិនបើ (parse_feature_request(feature_list, "push-options"))
						hook_use_push_options = 1;
				}
			}
		}

	ប្រសិនបើ (code) {
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
	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
		ប្រសិនបើ (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			continue;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		ប្រសិនបើ (code)
			bha_nga;
	}
	ប្រសិនបើ (!code)
		code = packet_flush_gently(proc.in);
	ប្រសិនបើ (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		goto cleanup;
	}

	/* Send push options */
	ប្រសិនបើ (hook_use_push_options) {
		sa_nracanaa string_list_item *item;

		for_each_string_list_item(item, push_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			ប្រសិនបើ (code)
				bha_nga;
		}
		ប្រសិនបើ (!code)
			code = packet_flush_gently(proc.in);
		ប្រសិនបើ (code) {
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
	ប្រសិនបើ (use_sideband)
		finish_async(&muxer);
	ប្រសិនបើ (finish_command(&proc))
		code = -1;
	ប្រសិនបើ (errmsg.len >0) {
		var_na *p = errmsg.buf;

		p += errmsg.len - 1;
		ប្រសិនបើ (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_pop(SIGPIPE);

	ត្រឡប់ code;
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
static ចំនួនគត់ update_shallow_ref(sa_nracanaa command *cmd, sa_nracanaa shallow_info *si)
{
	sa_nracanaa shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	sa_nracanaa oid_array extra = OID_ARRAY_INIT;
	sa_nracanaa check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	ចំនួនគត់ i;

	trace_printf_key(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	សម្រាប់ (i = 0; i < si->shallow->nr; i++)
		ប្រសិនបើ (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	ប្រសិនបើ (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		ត្រឡប់ -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	សម្រាប់ (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	ត្រឡប់ 0;
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
	ប្រសិនបើ (run_command(&child))
		ត្រឡប់ "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", NULL);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	ប្រសិនបើ (run_command(&child))
		ត្រឡប់ "Working directory has unstaged changes";

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
	ប្រសិនបើ (run_command(&child))
		ត្រឡប់ "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     NULL);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	ប្រសិនបើ (run_command(&child))
		ត្រឡប់ "Could not update working tree to new HEAD";

	ត្រឡប់ NULL;
}

static const var_na *push_to_checkout_hook = "push-to-checkout";

static const var_na *push_to_checkout(unsigned var_na *hash,
				    ចំនួនគត់ *invoked_hook,
				    sa_nracanaa strvec *env,
				    const var_na *work_tree)
{
	sa_nracanaa run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_tree));
	strvec_push(&opt.args, hash_to_hex(hash));
	ប្រសិនបើ (run_hooks_opt(the_repository, push_to_checkout_hook, &opt))
		ត្រឡប់ "push-to-checkout hook declined";
	បើមិនដូច្នេះ
		ត្រឡប់ NULL;
}

static const var_na *update_worktree(unsigned var_na *sha1, const sa_nracanaa worktree *worktree)
{
	const var_na *retval;
	var_na *git_dir;
	sa_nracanaa strvec env = STRVEC_INIT;
	ចំនួនគត់ invoked_hook;

	ប្រសិនបើ (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	ប្រសិនបើ (worktree->is_bare)
		ត្រឡប់ "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = push_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	ប្រសិនបើ (!invoked_hook)
		retval = push_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	free(git_dir);
	ត្រឡប់ retval;
}

static const var_na *update(sa_nracanaa command *cmd, sa_nracanaa shallow_info *si)
{
	const var_na *name = cmd->ref_name;
	sa_nracanaa strbuf namespaced_name_buf = STRBUF_INIT;
	static var_na *namespaced_name;
	const var_na *ret;
	sa_nracanaa object_id *old_oid = &cmd->old_oid;
	sa_nracanaa object_id *new_oid = &cmd->new_oid;
	ចំនួនគត់ do_update_worktree = 0;
	sa_nracanaa worktree **worktrees = get_worktrees(the_repository);
	const sa_nracanaa worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	ប្រសិនបើ (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		goto out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	free(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, NULL);

	ប្រសិនបើ (worktree && !worktree->is_bare) {
		switch (deny_current_branch) {
		pak_sa DENY_IGNORE:
			bha_nga;
		pak_sa DENY_WARN:
			rp_warning("updating the current branch");
			bha_nga;
		pak_sa DENY_REFUSE:
		pak_sa DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			ប្រសិនបើ (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			goto out;
		pak_sa DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			bha_nga;
		}
	}

	ប្រសិនបើ (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		goto out;
	}

	ប្រសិនបើ (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		ប្រសិនបើ (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			goto out;
		}

		ប្រសិនបើ (worktree || (head_name && !strcmp(namespaced_name, head_name))) {
			switch (deny_delete_current) {
			pak_sa DENY_IGNORE:
				bha_nga;
			pak_sa DENY_WARN:
				rp_warning("deleting the current branch");
				bha_nga;
			pak_sa DENY_REFUSE:
			pak_sa DENY_UNCONFIGURED:
			pak_sa DENY_UPDATE_INSTEAD:
				ប្រសិនបើ (deny_delete_current == DENY_UNCONFIGURED)
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

	ប្រសិនបើ (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		sa_nracanaa object *old_object, *new_object;
		sa_nracanaa commit *old_commit, *new_commit;
		ចំនួនគត់ ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		ប្រសិនបើ (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			goto out;
		}
		old_commit = (sa_nracanaa commit *)old_object;
		new_commit = (sa_nracanaa commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		ប្រសិនបើ (ret2 < 0)
			exit(128);
		ប្រសិនបើ (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			goto out;
		}
	}
	ប្រសិនបើ (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		goto out;
	}

	ប្រសិនបើ (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		ប្រសិនបើ (ret)
			goto out;
	}

	ប្រសិនបើ (is_null_oid(new_oid)) {
		sa_nracanaa strbuf err = STRBUF_INIT;
		ប្រសិនបើ (!parse_object(the_repository, old_oid)) {
			old_oid = NULL;
			ប្រសិនបើ (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} បើមិនដូច្នេះ {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		ប្រសិនបើ (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   NULL, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} បើមិនដូច្នេះ {
			ret = NULL; /* good */
		}
		strbuf_release(&err);
	} បើមិនដូច្នេះ {
		ga_nanaa ref_transaction_error tx_err;
		sa_nracanaa strbuf err = STRBUF_INIT;
		ប្រសិនបើ (shallow_update && si->shallow_ref[cmd->index] &&
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
		ប្រសិនបើ (tx_err) {
			rp_error("%s", err.buf);
			ប្រសិនបើ (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			បើមិនដូច្នេះ
				ret = ref_transaction_error_msg(tx_err);
		} បើមិនដូច្នេះ {
			ret = NULL; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	ត្រឡប់ ret;
}

static void run_update_post_hook(sa_nracanaa command *commands)
{
	static const var_na hook_name[] = "post-update";
	sa_nracanaa run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	sa_nracanaa async sideband_async;
	sa_nracanaa command *cmd;
	ចំនួនគត់ sideband_async_started = 0;
	ចំនួនគត់ saved_stderr = -1;

	ប្រសិនបើ (!hook_exists(the_repository, hook_name))
		ត្រឡប់;

	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
		ប្រសិនបើ (cmd->error_string || cmd->did_not_exist)
			continue;
		strvec_push(&opt.args, cmd->ref_name);
	}
	ប្រសិនបើ (!opt.args.nr)
		ត្រឡប់;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

static void check_aliased_update_internal(sa_nracanaa command *cmd,
					  sa_nracanaa string_list *list,
					  const var_na *dst_name, ចំនួនគត់ flag)
{
	sa_nracanaa string_list_item *item;
	sa_nracanaa command *dst_cmd;

	ប្រសិនបើ (!(flag & REF_ISSYMREF))
		ត្រឡប់;

	ប្រសិនបើ (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		ត្រឡប់;
	}
	dst_name = strip_namespace(dst_name);

	ប្រសិនបើ (!(item = string_list_lookup(list, dst_name)))
		ត្រឡប់;

	cmd->skip_update = 1;

	dst_cmd = (sa_nracanaa command *) item->util;

	ប្រសិនបើ (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		ត្រឡប់;

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
	ចំនួនគត់ flag;

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

	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
		sa_nracanaa string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (void *)cmd;
	}
	string_list_sort(&ref_list);

	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
		ប្រសិនបើ (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

static const sa_nracanaa object_id *command_singleton_iterator(void *cb_data)
{
	sa_nracanaa command **cmd_list = cb_data;
	sa_nracanaa command *cmd = *cmd_list;

	ប្រសិនបើ (!cmd || is_null_oid(&cmd->new_oid))
		ត្រឡប់ NULL;
	*cmd_list = NULL; /* this returns only one */
	ត្រឡប់ &cmd->new_oid;
}

static void set_connectivity_errors(sa_nracanaa command *commands,
				    sa_nracanaa shallow_info *si,
				    sa_nracanaa odb_transaction *transaction)
{
	sa_nracanaa command *cmd;

	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
		sa_nracanaa command *singleton = cmd;
		sa_nracanaa check_connected_options opt = CHECK_CONNECTED_INIT;
		sa_nracanaa strvec env = STRVEC_INIT;

		ប្រសិនបើ (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			continue;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		ប្រសិនបើ (!check_connected(command_singleton_iterator, &singleton,
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

	សម្រាប់ (; cmd; cmd = cmd->next) {
		ប្រសិនបើ (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			continue;
		ប្រសិនបើ (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			ត្រឡប់ &cmd->new_oid;
		}
	}
	ត្រឡប់ NULL;
}

static void reject_updates_to_hidden(sa_nracanaa command *commands)
{
	sa_nracanaa strbuf refname_full = STRBUF_INIT;
	size_t prefix_len;
	sa_nracanaa command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
		ប្រសិនបើ (cmd->error_string)
			continue;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		ប្រសិនបើ (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			continue;
		ប្រសិនបើ (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		បើមិនដូច្នេះ
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

static ចំនួនគត់ should_process_cmd(sa_nracanaa command *cmd)
{
	ត្រឡប់ !cmd->error_string && !cmd->skip_update;
}

static void BUG_if_skipped_connectivity_check(sa_nracanaa command *commands,
					       sa_nracanaa shallow_info *si)
{
	sa_nracanaa command *cmd;

	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
		ប្រសិនបើ (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
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

	ប្រសិនបើ (details)
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

	សម្រាប់ (ga_nanaa processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
			ប្រសិនបើ (!should_process_cmd(cmd) || cmd->run_proc_receive)
				continue;

			ប្រសិនបើ (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				continue;
			បើមិនដូច្នេះ ប្រសិនបើ (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				continue;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			ប្រសិនបើ (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				ប្រសិនបើ (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					goto failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		ប្រសិនបើ (!transaction)
			goto cleanup;

		ប្រសិនបើ (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			goto failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		ប្រសិនបើ (strmap_empty(&failed_refs))
			goto cleanup;

	failure:
		សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
			ប្រសិនបើ (reported_error)
				cmd->error_string = reported_error;
			បើមិនដូច្នេះ ប្រសិនបើ (strmap_contains(&failed_refs, cmd->ref_name))
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
	ប្រសិនបើ (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		goto failure;
	}

	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
		ប្រសិនបើ (!should_process_cmd(cmd) || cmd->run_proc_receive)
			continue;

		cmd->error_string = update(cmd, si);

		ប្រសិនបើ (cmd->error_string)
			goto failure;
	}

	ប្រសិនបើ (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		goto failure;
	}
	goto cleanup;

failure:
	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next)
		ប្រសិនបើ (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

static void execute_commands(sa_nracanaa command *commands,
			     ចំនួនគត់ unpacker_error,
			     sa_nracanaa shallow_info *si,
			     sa_nracanaa odb_transaction *transaction,
			     const sa_nracanaa string_list *push_options)
{
	sa_nracanaa check_connected_options opt = CHECK_CONNECTED_INIT;
	sa_nracanaa command *cmd;
	sa_nracanaa iterate_data data;
	sa_nracanaa async muxer;
	ចំនួនគត់ err_fd = 0;
	ចំនួនគត់ run_proc_receive = 0;

	ប្រសិនបើ (unpacker_error) {
		សម្រាប់ (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		ត្រឡប់;
	}

	ប្រសិនបើ (!skip_connectivity_check) {
		sa_nracanaa strvec env = STRVEC_INIT;

		ប្រសិនបើ (use_sideband) {
			memset(&muxer, 0, parimaa_na(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			ប្រសិនបើ (!start_async(&muxer))
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

		ប្រសិនបើ (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		ប្រសិនបើ (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	ប្រសិនបើ (proc_receive_ref) {
		សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
			ប្រសិនបើ (!should_process_cmd(cmd))
				continue;

			ប្រសិនបើ (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	ប្រសិនបើ (run_receive_hook(commands, "pre-receive", 0, transaction, push_options)) {
		សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
			ប្រសិនបើ (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		ត្រឡប់;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	សម្រាប់ (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	ប្រសិនបើ (!cmd)
		ត្រឡប់;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	ប្រសិនបើ (odb_transaction_commit(transaction)) {
		សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
			ប្រសិនបើ (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		ត្រឡប់;
	}

	check_aliased_updates(commands);

	free(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, NULL,
							    NULL);

	ប្រសិនបើ (run_proc_receive &&
	    run_proc_receive_hook(commands, push_options))
		សម្រាប់ (cmd = commands; cmd; cmd = cmd->next)
			ប្រសិនបើ (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	ប្រសិនបើ (use_atomic)
		execute_commands_atomic(commands, si);
	បើមិនដូច្នេះ
		execute_commands_non_atomic(commands, si);

	ប្រសិនបើ (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

static sa_nracanaa command **queue_command(sa_nracanaa command **tail,
				      const var_na *line,
				      ចំនួនគត់ linelen)
{
	sa_nracanaa object_id old_oid, new_oid;
	sa_nracanaa command *cmd;
	const var_na *refname;
	ចំនួនគត់ reflen;
	const var_na *p;

	ប្រសិនបើ (parse_oid_hex(line, &old_oid, &p) ||
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
	ត្រឡប់ &cmd->next;
}

static void free_commands(sa_nracanaa command *commands)
{
	ខណៈពេល (commands) {
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

	ប្រសិនបើ (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(push_cert->buf, "\n\n");
	ប្រសិនបើ (!boc)
		die("malformed push certificate %.*s", 100, push_cert->buf);
	បើមិនដូច្នេះ
		boc += 2;
	eoc = push_cert->buf + parse_signed_buffer(push_cert->buf, push_cert->len);

	ខណៈពេល (boc < eoc) {
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
	សម្រាប់ (;;) {
		ចំនួនគត់ linelen;

		ប្រសិនបើ (packet_reader_read(reader) != PACKET_READ_NORMAL)
			bha_nga;

		ប្រសិនបើ (reader->pktlen > 8 && starts_with(reader->line, "shallow ")) {
			sa_nracanaa object_id oid;
			ប្រសិនបើ (get_oid_hex(reader->line + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->line + 8);
			oid_array_append(shallow, &oid);
			continue;
		}

		linelen = strlen(reader->line);
		ប្រសិនបើ (linelen < reader->pktlen) {
			const var_na *feature_list = reader->line + linelen + 1;
			const var_na *hash = NULL;
			const var_na *client_sid;
			size_t len = 0;
			ប្រសិនបើ (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			ប្រសិនបើ (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			ប្រសិនបើ (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			ប្រសិនបើ (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			ប្រសិនបើ (advertise_atomic_push
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			ប្រសិនបើ (advertise_push_options
			    && parse_feature_request(feature_list, "push-options"))
				use_push_options = 1;
			hash = parse_feature_value(feature_list, "object-format", &len, NULL);
			ប្រសិនបើ (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			ប្រសិនបើ (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_value(feature_list, "session-id", &len, NULL);
			ប្រសិនបើ (client_sid) {
				var_na *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", NULL, "client-sid", client_sid);
				free(sid);
			}
		}

		ប្រសិនបើ (!strcmp(reader->line, "push-cert")) {
			ចំនួនគត់ true_flush = 0;
			ចំនួនគត់ saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			សម្រាប់ (;;) {
				packet_reader_read(reader);
				ប្រសិនបើ (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					bha_nga;
				}
				ប្រសិនបើ (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				ប្រសិនបើ (!strcmp(reader->line, "push-cert-end\n"))
					bha_nga; /* end of cert */
				strbuf_addstr(&push_cert, reader->line);
			}
			reader->options = saved_options;

			ប្រសិនបើ (true_flush)
				bha_nga;
			continue;
		}

		p = queue_command(p, reader->line, linelen);
	}

	ប្រសិនបើ (push_cert.len)
		queue_commands_from_cert(p, &push_cert);

	ត្រឡប់ commands;
}

static void read_push_options(sa_nracanaa packet_reader *reader,
			      sa_nracanaa string_list *options)
{
	ខណៈពេល (1) {
		ប្រសិនបើ (packet_reader_read(reader) != PACKET_READ_NORMAL)
			bha_nga;

		string_list_append(options, reader->line);
	}
}

static ចំនួនគត់ unpack_with_sideband(sa_nracanaa odb_transaction *transaction,
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
	ចំនួនគត់ ret;

	ប្រសិនបើ (!use_sideband)
		ត្រឡប់ odb_transaction_write_pack(transaction, 0, err_msg, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, parimaa_na(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	ប្រសិនបើ (start_async(&muxer))
		ត្រឡប់ 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_msg, &opts);

	finish_async(&muxer);
	ត្រឡប់ ret;
}

static void prepare_shallow_update(sa_nracanaa shallow_info *si,
				   const var_na *shallow_file)
{
	ចំនួនគត់ i, j, k, bitmap_size = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, NULL);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	សម្រាប់ (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	សម្រាប់ (i = 0; i < si->shallow->nr; i++) {
		ប្រសិនបើ (!si->used_shallow[i])
			continue;
		សម្រាប់ (j = 0; j < bitmap_size; j++) {
			ប្រសិនបើ (!si->used_shallow[i][j])
				continue;
			si->need_reachability_test[i]++;
			សម្រាប់ (k = 0; k < 32; k++)
				ប្រសិនបើ (si->used_shallow[i][j] & (1U << k))
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
	ចំនួនគត់ *ref_status;
	remove_nonexistent_theirs_shallow(si);
	ប្រសិនបើ (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		ត្រឡប់;
	}

	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
		ប្រសិនបើ (is_null_oid(&cmd->new_oid))
			continue;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	ប្រសិនបើ (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		ត្រឡប់;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, NULL, ref_status);
	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
		ប្រសិនបើ (is_null_oid(&cmd->new_oid))
			continue;
		ប្រសិនបើ (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	free(ref_status);
}

static void override_cmds_error(sa_nracanaa command *commands, const var_na *err)
{
	សម្រាប់ (sa_nracanaa command *cmd = commands; cmd; cmd = cmd->next) {
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

	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
		sa_nracanaa ref_push_report *report;
		ចំនួនគត់ count = 0;

		ប្រសិនបើ (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		បើមិនដូច្នេះ
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		ប្រសិនបើ (version != REPORT_STATUS_V2 || cmd->error_string)
			continue;

		សម្រាប់ (report = cmd->report; report; report = report->next) {
			ប្រសិនបើ (count++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			ប្រសិនបើ (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			ប្រសិនបើ (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			ប្រសិនបើ (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			ប្រសិនបើ (report->forced_update)
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

	ប្រសិនបើ (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	ប្រសិនបើ (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	បើមិនដូច្នេះ
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

static ចំនួនគត់ delete_only(sa_nracanaa command *commands)
{
	sa_nracanaa command *cmd;
	សម្រាប់ (cmd = commands; cmd; cmd = cmd->next) {
		ប្រសិនបើ (!is_null_oid(&cmd->new_oid))
			ត្រឡប់ 0;
	}
	ត្រឡប់ 1;
}

ចំនួនគត់ cmd_receive_pack(ចំនួនគត់ argc,
		     const var_na **argv,
		     const var_na *prefix,
		     sa_nracanaa repository *repo UNUSED)
{
	ចំនួនគត់ advertise_refs = 0;
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

	ប្រសិនបើ (argc > 1)
		usage_msg_opt(_("too many arguments"), receive_pack_usage, options);
	ប្រសិនបើ (argc == 0)
		usage_msg_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	ប្រសិនបើ (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, NULL);
	ប្រសិនបើ (cert_nonce_seed)
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
		ប្រសិនបើ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	pak_sa protocol_v0:
		bha_nga;
	pak_sa protocol_unknown_version:
		BUG("unknown protocol version");
	}

	ប្រសិនបើ (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	ប្រសិនបើ (advertise_refs)
		ត្រឡប់ 0;

	packet_reader_init(&reader, 0, NULL, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ប្រសិនបើ ((commands = read_head_info(&reader, &shallow, &version))) {
		sa_nracanaa string_list push_options = STRING_LIST_INIT_DUP;
		sa_nracanaa strbuf unpack_status = STRBUF_INIT;

		ប្រសិនបើ (use_push_options)
			read_push_options(&reader, &push_options);
		ប្រសិនបើ (!check_cert_push_options(&push_options)) {
			sa_nracanaa command *cmd;
			សម្រាប់ (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		ប្រសិនបើ (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		ប្រសិនបើ (!delete_only(commands)) {
			const var_na *alt_shallow_file = NULL;

			ប្រសិនបើ (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			ប្រសិនបើ (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			បើមិនដូច្នេះ
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
		ប្រសិនបើ (auto_gc) {
			sa_nracanaa child_process proc = CHILD_PROCESS_INIT;

			ប្រសិនបើ (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				ប្រសិនបើ (!start_command(&proc)) {
					ប្រសិនបើ (use_sideband)
						copy_to_sideband(proc.err, -1, NULL);
					finish_command(&proc);
				}
			}
		}
		ប្រសិនបើ (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	ប្រសិនបើ (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	free((void *)push_cert_nonce);
	ត្រឡប់ 0;
}
