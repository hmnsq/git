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

static const char * const receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	NULL
};

enum deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

enum report_status_version {
	REPORT_STATUS_UNKNOWN = 0,
	REPORT_STATUS_V0,
	REPORT_STATUS_V2,
};

static số_nguyên deny_deletes;
static số_nguyên deny_non_fast_forwards;
static enum deny_action deny_current_branch = DENY_UNCONFIGURED;
static enum deny_action deny_delete_current = DENY_UNCONFIGURED;
static số_nguyên receive_fsck_objects = -1;
static số_nguyên transfer_fsck_objects = -1;
static struct strbuf fsck_msg_types = STRBUF_INIT;
static số_nguyên advertise_atomic_push = 1;
static số_nguyên advertise_push_options;
static số_nguyên advertise_sid;
static off_t max_input_size;
static số_nguyên use_sideband;
static số_nguyên use_atomic;
static số_nguyên use_push_options;
static số_nguyên quiet;
static số_nguyên prefer_ofs_delta = 1;
static số_nguyên auto_update_server_info;
static số_nguyên auto_gc = 1;
static số_nguyên reject_thin;
static số_nguyên skip_connectivity_check;
static số_nguyên stateless_rpc;
static const char *service_dir;
static const char *head_name;
static void *head_name_to_free;
static số_nguyên sent_capabilities;
static số_nguyên shallow_update;
static struct strbuf push_cert = STRBUF_INIT;
static struct object_id push_cert_oid;
static struct signature_check sigcheck;
static const char *push_cert_nonce;
static char *cert_nonce_seed;
static struct strvec hidden_refs = STRVEC_INIT;

static const char *NONCE_UNSOLICITED = "UNSOLICITED";
static const char *NONCE_BAD = "BAD";
static const char *NONCE_MISSING = "MISSING";
static const char *NONCE_OK = "OK";
static const char *NONCE_SLOP = "SLOP";
static const char *nonce_status;
static long nonce_stamp_slop;
static timestamp_t nonce_stamp_slop_limit;
static struct ref_transaction *transaction;

static enum {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
static số_nguyên keepalive_in_sec = 5;

static struct proc_receive_ref {
	unsigned số_nguyên want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	char *ref_prefix;
	struct proc_receive_ref *next;
} *proc_receive_ref;

static void proc_receive_ref_append(const char *prefix);

static enum deny_action parse_deny_action(const char *var, const char *value)
{
	nếu (value) {
		nếu (!strcasecmp(value, "ignore"))
			trả_về DENY_IGNORE;
		nếu (!strcasecmp(value, "warn"))
			trả_về DENY_WARN;
		nếu (!strcasecmp(value, "refuse"))
			trả_về DENY_REFUSE;
		nếu (!strcasecmp(value, "updateinstead"))
			trả_về DENY_UPDATE_INSTEAD;
	}
	nếu (git_config_bool(var, value))
		trả_về DENY_REFUSE;
	trả_về DENY_IGNORE;
}

static số_nguyên receive_pack_config(const char *var, const char *value,
			       const struct config_context *ctx, void *cb)
{
	const char *msg_id;
	số_nguyên status = parse_hide_refs_config(var, value, "receive", &hidden_refs);

	nếu (status)
		trả_về status;

	nếu (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, value);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, value);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.fsck.skiplist") == 0) {
		char *path;

		nếu (git_config_pathname(&path, var, value))
			trả_về -1;
		nếu (path)
			strbuf_addf(&fsck_msg_types, "%cskiplist=%s",
				    fsck_msg_types.len ? ',' : '=', path);
		free(path);
		trả_về 0;
	}

	nếu (skip_prefix(var, "receive.fsck.", &msg_id)) {
		nếu (!value)
			trả_về config_error_nonbool(var);
		nếu (is_valid_msg_type(msg_id, value))
			strbuf_addf(&fsck_msg_types, "%c%s=%s",
				fsck_msg_types.len ? ',' : '=', msg_id, value);
		không_thì
			warning("skipping unknown msg id '%s'", msg_id);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, value);
		trả_về 0;
	}

	nếu (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, value);
		trả_về 0;
	}

	nếu (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, value);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, value);
		trả_về 0;
	}

	nếu (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, value);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, value);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, value);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, value);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.certnonceseed") == 0)
		trả_về git_config_string(&cert_nonce_seed, var, value);

	nếu (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, value, ctx->kvi);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_push = git_config_bool(var, value);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_push_options = git_config_bool(var, value);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, value, ctx->kvi);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.maxinputsize") == 0) {
		max_input_size = git_config_int64(var, value, ctx->kvi);
		trả_về 0;
	}

	nếu (strcmp(var, "receive.procreceiverefs") == 0) {
		nếu (!value)
			trả_về config_error_nonbool(var);
		proc_receive_ref_append(value);
		trả_về 0;
	}

	nếu (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, value);
		trả_về 0;
	}

	trả_về git_default_config(var, value, ctx, cb);
}

static void show_ref(const char *path, const struct object_id *oid)
{
	nếu (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} không_thì {
		struct strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		nếu (advertise_atomic_push)
			strbuf_addstr(&cap, " atomic");
		nếu (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		nếu (push_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", push_cert_nonce);
		nếu (advertise_push_options)
			strbuf_addstr(&cap, " push-options");
		nếu (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

static số_nguyên show_ref_cb(const struct reference *ref, void *data)
{
	struct oidset *seen = data;
	const char *path = strip_namespace(ref->name);

	nếu (ref_is_hidden(path, ref->name, &hidden_refs))
		trả_về 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	nếu (!path) {
		nếu (oidset_insert(seen, ref->oid))
			trả_về 0;
		path = ".have";
	} không_thì {
		oidset_insert(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	trả_về 0;
}

static void show_one_alternate_ref(const struct object_id *oid,
				   void *data)
{
	struct oidset *seen = data;

	nếu (oidset_insert(seen, oid))
		trả_về;

	show_ref(".have", oid);
}

static void write_head_info(void)
{
	struct refs_for_each_ref_options opts = { 0 };
	static struct oidset seen = OIDSET_INIT;
	struct strvec excludes_vector = STRVEC_INIT;

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

	nếu (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#define RUN_PROC_RECEIVE_SCHEDULED	1
#define RUN_PROC_RECEIVE_RETURNED	2
struct command {
	struct command *next;
	const char *error_string;
	char *error_string_owned;
	struct ref_push_report *report;
	unsigned số_nguyên skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	số_nguyên index;
	struct object_id old_oid;
	struct object_id new_oid;
	char ref_name[FLEX_ARRAY]; /* more */
};

static void proc_receive_ref_append(const char *prefix)
{
	struct proc_receive_ref *ref_pattern;
	const char *p;
	số_nguyên len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = strchr(prefix, ':');
	nếu (p) {
		trong_khi (prefix < p) {
			nếu (*prefix == 'a')
				ref_pattern->want_add = 1;
			không_thì nếu (*prefix == 'd')
				ref_pattern->want_delete = 1;
			không_thì nếu (*prefix == 'm')
				ref_pattern->want_modify = 1;
			không_thì nếu (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} không_thì {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	trong_khi (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	nếu (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} không_thì {
		struct proc_receive_ref *end;

		end = proc_receive_ref;
		trong_khi (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

static số_nguyên proc_receive_ref_matches(struct command *cmd)
{
	struct proc_receive_ref *p;

	nếu (!proc_receive_ref)
		trả_về 0;

	cho (p = proc_receive_ref; p; p = p->next) {
		const char *match = p->ref_prefix;
		const char *remains;

		nếu (!p->want_add && is_null_oid(&cmd->old_oid))
			continue;
		không_thì nếu (!p->want_delete && is_null_oid(&cmd->new_oid))
			continue;
		không_thì nếu (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			continue;

		nếu (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			nếu (!p->negative_ref)
				trả_về 1;
		} không_thì nếu (p->negative_ref) {
			trả_về 1;
		}
	}
	trả_về 0;
}

static void report_message(const char *prefix, const char *err, va_list params)
{
	số_nguyên sz;
	char msg[4096];

	sz = xsnprintf(msg, sizeof(msg), "%s", prefix);
	sz += vsnprintf(msg + sz, sizeof(msg) - sz, err, params);
	nếu (sz > (sizeof(msg) - 1))
		sz = sizeof(msg) - 1;
	msg[sz++] = '\n';

	nếu (use_sideband)
		send_sideband(1, 2, msg, sz, use_sideband);
	không_thì
		xwrite(2, msg, sz);
}

__attribute__((format (printf, 1, 2)))
static void rp_warning(const char *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("warning: ", err, params);
	va_end(params);
}

__attribute__((format (printf, 1, 2)))
static void rp_error(const char *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("error: ", err, params);
	va_end(params);
}

static số_nguyên copy_to_sideband(số_nguyên in, số_nguyên out UNUSED, void *arg UNUSED)
{
	char data[128];
	số_nguyên keepalive_active = 0;

	nếu (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	nếu (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	trong_khi (1) {
		ssize_t sz;

		nếu (keepalive_active) {
			struct pollfd pfd;
			số_nguyên ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			nếu (ret < 0) {
				nếu (errno == EINTR)
					continue;
				không_thì
					break;
			} không_thì nếu (ret == 0) {
				/* no data; send a keepalive packet */
				static const char buf[] = "0005\1";
				write_or_die(1, buf, sizeof(buf) - 1);
				continue;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, sizeof(data));
		nếu (sz <= 0)
			break;

		nếu (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			const char *p = memchr(data, '\0', sz);
			nếu (p) {
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
	trả_về 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
static void prepare_sideband_async(struct async *sideband_async, số_nguyên *saved_stderr, số_nguyên *started)
{
	*started = 0;

	nếu (!use_sideband)
		trả_về;

	memset(sideband_async, 0, sizeof(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	nếu (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		nếu (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
static void finish_sideband_async(struct async *sideband_async, số_nguyên saved_stderr, số_nguyên started)
{
	nếu (!use_sideband)
		trả_về;

	nếu (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	nếu (started)
		finish_async(sideband_async);
}

static void hmac_hash(unsigned char *out,
		      const char *key_in, size_t key_len,
		      const char *text, size_t text_len)
{
	unsigned char key[GIT_MAX_BLKSZ];
	unsigned char k_ipad[GIT_MAX_BLKSZ];
	unsigned char k_opad[GIT_MAX_BLKSZ];
	số_nguyên i;
	struct git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(key, '\0', GIT_MAX_BLKSZ);
	nếu (the_hash_algo->blksz < key_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, key_in, key_len);
		git_hash_final(key, &ctx);
	} không_thì {
		memcpy(key, key_in, key_len);
	}

	/* RFC 2104 2. (2) & (5) */
	cho (i = 0; i < sizeof(key); i++) {
		k_ipad[i] = key[i] ^ 0x36;
		k_opad[i] = key[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, sizeof(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, sizeof(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

static char *prepare_push_cert_nonce(const char *path, timestamp_t stamp)
{
	struct strbuf buf = STRBUF_INIT;
	unsigned char hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, strlen(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (số_nguyên)the_hash_algo->hexsz, hash_to_hex(hash));
	trả_về strbuf_detach(&buf, NULL);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
static số_nguyên constant_memequal(const char *a, const char *b, size_t n)
{
	số_nguyên res = 0;
	size_t i;

	cho (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	trả_về res;
}

static const char *check_nonce(const char *buf)
{
	size_t noncelen;
	const char *found = find_commit_header(buf, "nonce", &noncelen);
	char *nonce = found ? xmemdupz(found, noncelen) : NULL;
	timestamp_t stamp, ostamp;
	char *bohmac, *expect = NULL;
	const char *retval = NONCE_BAD;

	nếu (!nonce) {
		retval = NONCE_MISSING;
		goto leave;
	} không_thì nếu (!push_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		goto leave;
	} không_thì nếu (!strcmp(push_cert_nonce, nonce)) {
		retval = NONCE_OK;
		goto leave;
	}

	nếu (!stateless_rpc) {
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
	nếu (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		goto leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	nếu (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		goto leave;
	}

	expect = prepare_push_cert_nonce(service_dir, stamp);
	nếu (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		goto leave;
	}
	nếu (constant_memequal(expect, nonce, noncelen)) {
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

	nếu (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		free((void *)push_cert_nonce);
		push_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} không_thì {
		retval = NONCE_SLOP;
	}

leave:
	free(nonce);
	free(expect);
	trả_về retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
static số_nguyên check_cert_push_options(const struct string_list *push_options)
{
	const char *buf = push_cert.buf;

	const char *option;
	size_t optionlen;
	số_nguyên options_seen = 0;

	số_nguyên retval = 1;

	nếu (!*buf)
		trả_về 1;

	trong_khi ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		nếu (options_seen > push_options->nr
		    || xstrncmpz(push_options->items[options_seen - 1].string,
				 option, optionlen))
			trả_về 0;
	}

	nếu (options_seen != push_options->nr)
		retval = 0;

	trả_về retval;
}

static void prepare_push_cert_sha1(struct run_hooks_opt *opt)
{
	static số_nguyên already_done;

	nếu (!push_cert.len)
		trả_về;

	nếu (!already_done) {
		số_nguyên bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		nếu (odb_write_object(the_repository->objects, push_cert.buf,
				     push_cert.len, OBJ_BLOB, &push_cert_oid))
			oidclr(&push_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', sizeof(sigcheck));

		bogs = parse_signed_buffer(push_cert.buf, push_cert.len);
		sigcheck.payload = xmemdupz(push_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, push_cert.buf + bogs,
				push_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	nếu (!is_null_oid(&push_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&push_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.key ? sigcheck.key : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		nếu (push_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     push_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			nếu (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

struct receive_hook_feed_state {
	struct command *cmd;
	struct ref_push_report *report;
	số_nguyên skip_broken;
	struct strbuf buf;
};

static số_nguyên feed_receive_hook_cb(số_nguyên hook_stdin_fd, void *pp_cb UNUSED, void *pp_task_cb)
{
	struct receive_hook_feed_state *state = pp_task_cb;
	struct command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	trong_khi (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	nếu (!cmd)
		trả_về 1;  /* no more commands left */

	nếu (!state->report)
		state->report = cmd->report;

	nếu (state->report) {
		struct object_id *old_oid;
		struct object_id *new_oid;
		const char *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->next;
		nếu (!state->report)
			cmd = cmd->next;
	} không_thì {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	nếu (state->buf.len > 0) {
		số_nguyên ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		nếu (ret < 0) {
			nếu (errno == EPIPE)
				trả_về 1; /* child closed pipe */
			trả_về ret;
		}
	}

	trả_về state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

static void *receive_hook_feed_state_alloc(void *feed_pipe_ctx)
{
	struct receive_hook_feed_state *init_state = feed_pipe_ctx;
	struct receive_hook_feed_state *data;

	CALLOC_ARRAY(data, 1);
	data->report = init_state->report;
	data->cmd = init_state->cmd;
	data->skip_broken = init_state->skip_broken;
	strbuf_init(&data->buf, 0);

	trả_về data;
}

static void receive_hook_feed_state_free(void *data)
{
	struct receive_hook_feed_state *d = data;
	nếu (!d)
		trả_về;
	strbuf_release(&d->buf);
	free(d);
}

static số_nguyên run_receive_hook(struct command *commands,
			    const char *hook_name,
			    số_nguyên skip_broken,
			    struct odb_transaction *transaction,
			    const struct string_list *push_options)
{
	struct run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	struct command *iter = commands;
	struct receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	struct async sideband_async;
	số_nguyên sideband_async_started = 0;
	số_nguyên saved_stderr = -1;
	số_nguyên ret;

	nếu (!hook_exists(the_repository, hook_name))
		trả_về 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	trong_khi (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	nếu (!iter)
		trả_về 0;

	nếu (push_options) {
		cho (số_nguyên i = 0; i < push_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     push_options->items[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)push_options->nr);
	} không_thì {
		strvec_push(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	nếu (transaction)
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

	trả_về ret;
}

static số_nguyên run_update_hook(struct command *cmd)
{
	static const char hook_name[] = "update";
	struct run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	struct async sideband_async;
	số_nguyên sideband_async_started = 0;
	số_nguyên saved_stderr = -1;
	số_nguyên code;

	nếu (!hook_exists(the_repository, hook_name))
		trả_về 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     NULL);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	trả_về code;
}

static số_nguyên run_receive_report_hook(struct strbuf *report)
{
	struct child_process proc = CHILD_PROCESS_INIT;
	struct async sideband_async;
	số_nguyên sideband_async_started = 0;
	số_nguyên saved_stderr = -1;
	struct strbuf out = STRBUF_INIT;
	const char *hook_path;
	số_nguyên ret;

	hook_path = find_hook(the_repository, "receive-report");
	nếu (!hook_path)
		trả_về 0;

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

	nếu (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	trả_về ret;
}

static struct command *find_command_by_refname(struct command *list,
					       const char *refname)
{
	cho (; list; list = list->next)
		nếu (!strcmp(list->ref_name, refname))
			trả_về list;
	trả_về NULL;
}

static số_nguyên read_proc_receive_report(struct packet_reader *reader,
				    struct command *commands,
				    struct strbuf *errmsg)
{
	struct command *cmd;
	struct command *hint = NULL;
	struct ref_push_report *report = NULL;
	số_nguyên new_report = 0;
	số_nguyên code = 0;
	số_nguyên once = 0;
	số_nguyên response = 0;

	cho (;;) {
		struct object_id old_oid, new_oid;
		char *head;
		char *refname;
		char *p;
		enum packet_read_status status;

		status = packet_reader_read(reader);
		nếu (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			nếu (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				trả_về -1;
			}
			break;
		}
		response++;

		head = reader->line;
		p = strchr(head, ' ');
		nếu (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			continue;
		}
		*p++ = '\0';
		nếu (!strcmp(head, "option")) {
			char *key;
			const char *val;

			nếu (!hint || !(report || new_report)) {
				nếu (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				continue;
			}
			nếu (new_report) {
				nếu (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} không_thì {
					report = hint->report;
					trong_khi (report->next)
						report = report->next;
					report->next = xcalloc(1, sizeof(struct ref_push_report));
					report = report->next;
				}
				new_report = 0;
			}
			key = p;
			p = strchr(key, ' ');
			nếu (p)
				*p++ = '\0';
			val = p;
			nếu (!strcmp(key, "refname"))
				report->ref_name = xstrdup_or_null(val);
			không_thì nếu (!strcmp(key, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			không_thì nếu (!strcmp(key, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			không_thì nếu (!strcmp(key, "forced-update"))
				report->forced_update = 1;
			không_thì nếu (!strcmp(key, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			continue;
		}

		report = NULL;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		nếu (p)
			*p++ = '\0';
		nếu (strcmp(head, "ok") && strcmp(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			continue;
		}

		/* first try searching at our hint, falling back to all refs */
		nếu (hint)
			hint = find_command_by_refname(hint, refname);
		nếu (!hint)
			hint = find_command_by_refname(commands, refname);
		nếu (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			continue;
		}
		nếu (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			continue;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		nếu (!strcmp(head, "ng")) {
			nếu (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			không_thì
				hint->error_string = "failed";
			code = -1;
			continue;
		}
		new_report = 1;
	}

	cho (cmd = commands; cmd; cmd = cmd->next)
		nếu (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	trả_về code;
}

static số_nguyên run_proc_receive_hook(struct command *commands,
				 const struct string_list *push_options)
{
	struct child_process proc = CHILD_PROCESS_INIT;
	struct async muxer;
	struct command *cmd;
	struct packet_reader reader;
	struct strbuf cap = STRBUF_INIT;
	struct strbuf errmsg = STRBUF_INIT;
	số_nguyên hook_use_push_options = 0;
	số_nguyên version = 0;
	số_nguyên code;
	const char *hook_path = find_hook(the_repository, "proc-receive");

	nếu (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		trả_về -1;
	}

	strvec_push(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	nếu (use_sideband) {
		memset(&muxer, 0, sizeof(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		nếu (code)
			trả_về code;
		proc.err = muxer.in;
	} không_thì {
		proc.err = 0;
	}

	code = start_command(&proc);
	nếu (code) {
		nếu (use_sideband)
			finish_async(&muxer);
		trả_về code;
	}

	sigchain_push(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, NULL, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	nếu (use_atomic)
		strbuf_addstr(&cap, " atomic");
	nếu (use_push_options)
		strbuf_addstr(&cap, " push-options");
	nếu (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} không_thì {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	nếu (!code)
		code = packet_flush_gently(proc.in);

	nếu (!code)
		cho (;;) {
			số_nguyên linelen;
			enum packet_read_status status;

			status = packet_reader_read(&reader);
			nếu (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				nếu (status == PACKET_READ_EOF)
					code = -1;
				break;
			}

			nếu (reader.pktlen > 8 && starts_with(reader.line, "version=")) {
				version = atoi(reader.line + 8);
				linelen = strlen(reader.line);
				nếu (linelen < reader.pktlen) {
					const char *feature_list = reader.line + linelen + 1;
					nếu (parse_feature_request(feature_list, "push-options"))
						hook_use_push_options = 1;
				}
			}
		}

	nếu (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		goto cleanup;
	}

	switch (version) {
	case 0:
		/* fallthrough */
	case 1:
		break;
	default:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		goto cleanup;
	}

	/* Send commands */
	cho (cmd = commands; cmd; cmd = cmd->next) {
		nếu (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			continue;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		nếu (code)
			break;
	}
	nếu (!code)
		code = packet_flush_gently(proc.in);
	nếu (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		goto cleanup;
	}

	/* Send push options */
	nếu (hook_use_push_options) {
		struct string_list_item *item;

		for_each_string_list_item(item, push_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			nếu (code)
				break;
		}
		nếu (!code)
			code = packet_flush_gently(proc.in);
		nếu (code) {
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
	nếu (use_sideband)
		finish_async(&muxer);
	nếu (finish_command(&proc))
		code = -1;
	nếu (errmsg.len >0) {
		char *p = errmsg.buf;

		p += errmsg.len - 1;
		nếu (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_pop(SIGPIPE);

	trả_về code;
}

static const char *refuse_unconfigured_deny_msg =
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

static const char *refuse_unconfigured_deny_delete_current_msg =
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

static const struct object_id *command_singleton_iterator(void *cb_data);
static số_nguyên update_shallow_ref(struct command *cmd, struct shallow_info *si)
{
	struct shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	struct oid_array extra = OID_ARRAY_INIT;
	struct check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	số_nguyên i;

	trace_printf_key(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	cho (i = 0; i < si->shallow->nr; i++)
		nếu (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	nếu (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		trả_về -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	cho (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	trả_về 0;
}

static const char *push_to_deploy(unsigned char *sha1,
				  struct strvec *env,
				  const struct worktree *worktree)
{
	struct child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", NULL);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	nếu (run_command(&child))
		trả_về "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", NULL);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	nếu (run_command(&child))
		trả_về "Working directory has unstaged changes";

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
	nếu (run_command(&child))
		trả_về "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     NULL);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	nếu (run_command(&child))
		trả_về "Could not update working tree to new HEAD";

	trả_về NULL;
}

static const char *push_to_checkout_hook = "push-to-checkout";

static const char *push_to_checkout(unsigned char *hash,
				    số_nguyên *invoked_hook,
				    struct strvec *env,
				    const char *work_tree)
{
	struct run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_tree));
	strvec_push(&opt.args, hash_to_hex(hash));
	nếu (run_hooks_opt(the_repository, push_to_checkout_hook, &opt))
		trả_về "push-to-checkout hook declined";
	không_thì
		trả_về NULL;
}

static const char *update_worktree(unsigned char *sha1, const struct worktree *worktree)
{
	const char *retval;
	char *git_dir;
	struct strvec env = STRVEC_INIT;
	số_nguyên invoked_hook;

	nếu (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	nếu (worktree->is_bare)
		trả_về "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = push_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	nếu (!invoked_hook)
		retval = push_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	free(git_dir);
	trả_về retval;
}

static const char *update(struct command *cmd, struct shallow_info *si)
{
	const char *name = cmd->ref_name;
	struct strbuf namespaced_name_buf = STRBUF_INIT;
	static char *namespaced_name;
	const char *ret;
	struct object_id *old_oid = &cmd->old_oid;
	struct object_id *new_oid = &cmd->new_oid;
	số_nguyên do_update_worktree = 0;
	struct worktree **worktrees = get_worktrees(the_repository);
	const struct worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	nếu (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		goto out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	free(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, NULL);

	nếu (worktree && !worktree->is_bare) {
		switch (deny_current_branch) {
		case DENY_IGNORE:
			break;
		case DENY_WARN:
			rp_warning("updating the current branch");
			break;
		case DENY_REFUSE:
		case DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			nếu (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			goto out;
		case DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			break;
		}
	}

	nếu (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		goto out;
	}

	nếu (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		nếu (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			goto out;
		}

		nếu (worktree || (head_name && !strcmp(namespaced_name, head_name))) {
			switch (deny_delete_current) {
			case DENY_IGNORE:
				break;
			case DENY_WARN:
				rp_warning("deleting the current branch");
				break;
			case DENY_REFUSE:
			case DENY_UNCONFIGURED:
			case DENY_UPDATE_INSTEAD:
				nếu (deny_delete_current == DENY_UNCONFIGURED)
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

	nếu (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		struct object *old_object, *new_object;
		struct commit *old_commit, *new_commit;
		số_nguyên ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		nếu (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			goto out;
		}
		old_commit = (struct commit *)old_object;
		new_commit = (struct commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		nếu (ret2 < 0)
			exit(128);
		nếu (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			goto out;
		}
	}
	nếu (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		goto out;
	}

	nếu (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		nếu (ret)
			goto out;
	}

	nếu (is_null_oid(new_oid)) {
		struct strbuf err = STRBUF_INIT;
		nếu (!parse_object(the_repository, old_oid)) {
			old_oid = NULL;
			nếu (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} không_thì {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		nếu (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   NULL, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} không_thì {
			ret = NULL; /* good */
		}
		strbuf_release(&err);
	} không_thì {
		enum ref_transaction_error tx_err;
		struct strbuf err = STRBUF_INIT;
		nếu (shallow_update && si->shallow_ref[cmd->index] &&
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
		nếu (tx_err) {
			rp_error("%s", err.buf);
			nếu (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			không_thì
				ret = ref_transaction_error_msg(tx_err);
		} không_thì {
			ret = NULL; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	trả_về ret;
}

static void run_update_post_hook(struct command *commands)
{
	static const char hook_name[] = "post-update";
	struct run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	struct async sideband_async;
	struct command *cmd;
	số_nguyên sideband_async_started = 0;
	số_nguyên saved_stderr = -1;

	nếu (!hook_exists(the_repository, hook_name))
		trả_về;

	cho (cmd = commands; cmd; cmd = cmd->next) {
		nếu (cmd->error_string || cmd->did_not_exist)
			continue;
		strvec_push(&opt.args, cmd->ref_name);
	}
	nếu (!opt.args.nr)
		trả_về;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

static void check_aliased_update_internal(struct command *cmd,
					  struct string_list *list,
					  const char *dst_name, số_nguyên flag)
{
	struct string_list_item *item;
	struct command *dst_cmd;

	nếu (!(flag & REF_ISSYMREF))
		trả_về;

	nếu (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		trả_về;
	}
	dst_name = strip_namespace(dst_name);

	nếu (!(item = string_list_lookup(list, dst_name)))
		trả_về;

	cmd->skip_update = 1;

	dst_cmd = (struct command *) item->util;

	nếu (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		trả_về;

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

static void check_aliased_update(struct command *cmd, struct string_list *list)
{
	struct strbuf buf = STRBUF_INIT;
	const char *dst_name;
	số_nguyên flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, NULL, &flag);
	check_aliased_update_internal(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

static void check_aliased_updates(struct command *commands)
{
	struct command *cmd;
	struct string_list ref_list = STRING_LIST_INIT_NODUP;

	cho (cmd = commands; cmd; cmd = cmd->next) {
		struct string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (void *)cmd;
	}
	string_list_sort(&ref_list);

	cho (cmd = commands; cmd; cmd = cmd->next) {
		nếu (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

static const struct object_id *command_singleton_iterator(void *cb_data)
{
	struct command **cmd_list = cb_data;
	struct command *cmd = *cmd_list;

	nếu (!cmd || is_null_oid(&cmd->new_oid))
		trả_về NULL;
	*cmd_list = NULL; /* this returns only one */
	trả_về &cmd->new_oid;
}

static void set_connectivity_errors(struct command *commands,
				    struct shallow_info *si,
				    struct odb_transaction *transaction)
{
	struct command *cmd;

	cho (cmd = commands; cmd; cmd = cmd->next) {
		struct command *singleton = cmd;
		struct check_connected_options opt = CHECK_CONNECTED_INIT;
		struct strvec env = STRVEC_INIT;

		nếu (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			continue;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		nếu (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			continue;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

struct iterate_data {
	struct command *cmds;
	struct shallow_info *si;
};

static const struct object_id *iterate_receive_command_list(void *cb_data)
{
	struct iterate_data *data = cb_data;
	struct command **cmd_list = &data->cmds;
	struct command *cmd = *cmd_list;

	cho (; cmd; cmd = cmd->next) {
		nếu (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			continue;
		nếu (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			trả_về &cmd->new_oid;
		}
	}
	trả_về NULL;
}

static void reject_updates_to_hidden(struct command *commands)
{
	struct strbuf refname_full = STRBUF_INIT;
	size_t prefix_len;
	struct command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	cho (cmd = commands; cmd; cmd = cmd->next) {
		nếu (cmd->error_string)
			continue;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		nếu (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			continue;
		nếu (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		không_thì
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

static số_nguyên should_process_cmd(struct command *cmd)
{
	trả_về !cmd->error_string && !cmd->skip_update;
}

static void BUG_if_skipped_connectivity_check(struct command *commands,
					       struct shallow_info *si)
{
	struct command *cmd;

	cho (cmd = commands; cmd; cmd = cmd->next) {
		nếu (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

static void ref_transaction_rejection_handler(const char *refname,
					      const struct object_id *old_oid UNUSED,
					      const struct object_id *new_oid UNUSED,
					      const char *old_target UNUSED,
					      const char *new_target UNUSED,
					      enum ref_transaction_error err,
					      const char *details,
					      void *cb_data)
{
	struct strmap *failed_refs = cb_data;

	nếu (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (char *)ref_transaction_error_msg(err));
}

static void execute_commands_non_atomic(struct command *commands,
					struct shallow_info *si)
{
	struct command *cmd;
	struct strbuf err = STRBUF_INIT;
	const char *reported_error = NULL;
	struct strmap failed_refs = STRMAP_INIT;

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
	enum processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	cho (enum processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		cho (cmd = commands; cmd; cmd = cmd->next) {
			nếu (!should_process_cmd(cmd) || cmd->run_proc_receive)
				continue;

			nếu (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				continue;
			không_thì nếu (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				continue;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			nếu (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				nếu (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					goto failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		nếu (!transaction)
			goto cleanup;

		nếu (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			goto failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		nếu (strmap_empty(&failed_refs))
			goto cleanup;

	failure:
		cho (cmd = commands; cmd; cmd = cmd->next) {
			nếu (reported_error)
				cmd->error_string = reported_error;
			không_thì nếu (strmap_contains(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = NULL;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

static void execute_commands_atomic(struct command *commands,
					struct shallow_info *si)
{
	struct command *cmd;
	struct strbuf err = STRBUF_INIT;
	const char *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	nếu (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		goto failure;
	}

	cho (cmd = commands; cmd; cmd = cmd->next) {
		nếu (!should_process_cmd(cmd) || cmd->run_proc_receive)
			continue;

		cmd->error_string = update(cmd, si);

		nếu (cmd->error_string)
			goto failure;
	}

	nếu (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		goto failure;
	}
	goto cleanup;

failure:
	cho (cmd = commands; cmd; cmd = cmd->next)
		nếu (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

static void execute_commands(struct command *commands,
			     số_nguyên unpacker_error,
			     struct shallow_info *si,
			     struct odb_transaction *transaction,
			     const struct string_list *push_options)
{
	struct check_connected_options opt = CHECK_CONNECTED_INIT;
	struct command *cmd;
	struct iterate_data data;
	struct async muxer;
	số_nguyên err_fd = 0;
	số_nguyên run_proc_receive = 0;

	nếu (unpacker_error) {
		cho (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		trả_về;
	}

	nếu (!skip_connectivity_check) {
		struct strvec env = STRVEC_INIT;

		nếu (use_sideband) {
			memset(&muxer, 0, sizeof(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			nếu (!start_async(&muxer))
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

		nếu (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		nếu (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	nếu (proc_receive_ref) {
		cho (cmd = commands; cmd; cmd = cmd->next) {
			nếu (!should_process_cmd(cmd))
				continue;

			nếu (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	nếu (run_receive_hook(commands, "pre-receive", 0, transaction, push_options)) {
		cho (cmd = commands; cmd; cmd = cmd->next) {
			nếu (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		trả_về;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	cho (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	nếu (!cmd)
		trả_về;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	nếu (odb_transaction_commit(transaction)) {
		cho (cmd = commands; cmd; cmd = cmd->next) {
			nếu (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		trả_về;
	}

	check_aliased_updates(commands);

	free(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, NULL,
							    NULL);

	nếu (run_proc_receive &&
	    run_proc_receive_hook(commands, push_options))
		cho (cmd = commands; cmd; cmd = cmd->next)
			nếu (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	nếu (use_atomic)
		execute_commands_atomic(commands, si);
	không_thì
		execute_commands_non_atomic(commands, si);

	nếu (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

static struct command **queue_command(struct command **tail,
				      const char *line,
				      số_nguyên linelen)
{
	struct object_id old_oid, new_oid;
	struct command *cmd;
	const char *refname;
	số_nguyên reflen;
	const char *p;

	nếu (parse_oid_hex(line, &old_oid, &p) ||
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
	trả_về &cmd->next;
}

static void free_commands(struct command *commands)
{
	trong_khi (commands) {
		struct command *next = commands->next;

		ref_push_report_free(commands->report);
		free(commands->error_string_owned);
		free(commands);
		commands = next;
	}
}

static void queue_commands_from_cert(struct command **tail,
				     struct strbuf *push_cert)
{
	const char *boc, *eoc;

	nếu (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(push_cert->buf, "\n\n");
	nếu (!boc)
		die("malformed push certificate %.*s", 100, push_cert->buf);
	không_thì
		boc += 2;
	eoc = push_cert->buf + parse_signed_buffer(push_cert->buf, push_cert->len);

	trong_khi (boc < eoc) {
		const char *eol = memchr(boc, '\n', eoc - boc);
		tail = queue_command(tail, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

static struct command *read_head_info(struct packet_reader *reader,
				      struct oid_array *shallow,
				      enum report_status_version *version)
{
	struct command *commands = NULL;
	struct command **p = &commands;
	cho (;;) {
		số_nguyên linelen;

		nếu (packet_reader_read(reader) != PACKET_READ_NORMAL)
			break;

		nếu (reader->pktlen > 8 && starts_with(reader->line, "shallow ")) {
			struct object_id oid;
			nếu (get_oid_hex(reader->line + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->line + 8);
			oid_array_append(shallow, &oid);
			continue;
		}

		linelen = strlen(reader->line);
		nếu (linelen < reader->pktlen) {
			const char *feature_list = reader->line + linelen + 1;
			const char *hash = NULL;
			const char *client_sid;
			size_t len = 0;
			nếu (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			nếu (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			nếu (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			nếu (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			nếu (advertise_atomic_push
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			nếu (advertise_push_options
			    && parse_feature_request(feature_list, "push-options"))
				use_push_options = 1;
			hash = parse_feature_value(feature_list, "object-format", &len, NULL);
			nếu (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			nếu (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_value(feature_list, "session-id", &len, NULL);
			nếu (client_sid) {
				char *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", NULL, "client-sid", client_sid);
				free(sid);
			}
		}

		nếu (!strcmp(reader->line, "push-cert")) {
			số_nguyên true_flush = 0;
			số_nguyên saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			cho (;;) {
				packet_reader_read(reader);
				nếu (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					break;
				}
				nếu (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				nếu (!strcmp(reader->line, "push-cert-end\n"))
					break; /* end of cert */
				strbuf_addstr(&push_cert, reader->line);
			}
			reader->options = saved_options;

			nếu (true_flush)
				break;
			continue;
		}

		p = queue_command(p, reader->line, linelen);
	}

	nếu (push_cert.len)
		queue_commands_from_cert(p, &push_cert);

	trả_về commands;
}

static void read_push_options(struct packet_reader *reader,
			      struct string_list *options)
{
	trong_khi (1) {
		nếu (packet_reader_read(reader) != PACKET_READ_NORMAL)
			break;

		string_list_append(options, reader->line);
	}
}

static số_nguyên unpack_with_sideband(struct odb_transaction *transaction,
				const char *shallow_file,
				struct strbuf *err_msg)
{
	struct odb_transaction_write_pack_opts opts = {
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
	struct async muxer;
	số_nguyên ret;

	nếu (!use_sideband)
		trả_về odb_transaction_write_pack(transaction, 0, err_msg, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, sizeof(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	nếu (start_async(&muxer))
		trả_về 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_msg, &opts);

	finish_async(&muxer);
	trả_về ret;
}

static void prepare_shallow_update(struct shallow_info *si,
				   const char *shallow_file)
{
	số_nguyên i, j, k, bitmap_size = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, NULL);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	cho (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	cho (i = 0; i < si->shallow->nr; i++) {
		nếu (!si->used_shallow[i])
			continue;
		cho (j = 0; j < bitmap_size; j++) {
			nếu (!si->used_shallow[i][j])
				continue;
			si->need_reachability_test[i]++;
			cho (k = 0; k < 32; k++)
				nếu (si->used_shallow[i][j] & (1U << k))
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

static void update_shallow_info(struct command *commands,
				struct shallow_info *si,
				struct oid_array *ref,
				const char *shallow_file)
{
	struct command *cmd;
	số_nguyên *ref_status;
	remove_nonexistent_theirs_shallow(si);
	nếu (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		trả_về;
	}

	cho (cmd = commands; cmd; cmd = cmd->next) {
		nếu (is_null_oid(&cmd->new_oid))
			continue;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	nếu (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		trả_về;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, NULL, ref_status);
	cho (cmd = commands; cmd; cmd = cmd->next) {
		nếu (is_null_oid(&cmd->new_oid))
			continue;
		nếu (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	free(ref_status);
}

static void override_cmds_error(struct command *commands, const char *err)
{
	cho (struct command *cmd = commands; cmd; cmd = cmd->next) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
static void generate_report(struct strbuf *buf, struct command *commands,
			    const struct strbuf *unpack_status,
			    enum report_status_version version)
{
	struct command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	cho (cmd = commands; cmd; cmd = cmd->next) {
		struct ref_push_report *report;
		số_nguyên count = 0;

		nếu (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		không_thì
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		nếu (version != REPORT_STATUS_V2 || cmd->error_string)
			continue;

		cho (report = cmd->report; report; report = report->next) {
			nếu (count++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			nếu (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			nếu (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			nếu (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			nếu (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

static void report(struct command *commands, const struct strbuf *unpack_status,
		   enum report_status_version version)
{
	struct strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	nếu (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	nếu (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	không_thì
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

static số_nguyên delete_only(struct command *commands)
{
	struct command *cmd;
	cho (cmd = commands; cmd; cmd = cmd->next) {
		nếu (!is_null_oid(&cmd->new_oid))
			trả_về 0;
	}
	trả_về 1;
}

số_nguyên cmd_receive_pack(số_nguyên argc,
		     const char **argv,
		     const char *prefix,
		     struct repository *repo UNUSED)
{
	số_nguyên advertise_refs = 0;
	struct command *commands;
	struct oid_array shallow = OID_ARRAY_INIT;
	struct oid_array ref = OID_ARRAY_INIT;
	struct shallow_info si;
	struct packet_reader reader;
	struct odb_transaction *transaction = NULL;
	enum report_status_version version = REPORT_STATUS_UNKNOWN;

	struct option options[] = {
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

	nếu (argc > 1)
		usage_msg_opt(_("too many arguments"), receive_pack_usage, options);
	nếu (argc == 0)
		usage_msg_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	nếu (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, NULL);
	nếu (cert_nonce_seed)
		push_cert_nonce = prepare_push_cert_nonce(service_dir, time(NULL));

	switch (determine_protocol_version_server()) {
	case protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		break;
	case protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		nếu (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	case protocol_v0:
		break;
	case protocol_unknown_version:
		BUG("unknown protocol version");
	}

	nếu (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	nếu (advertise_refs)
		trả_về 0;

	packet_reader_init(&reader, 0, NULL, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	nếu ((commands = read_head_info(&reader, &shallow, &version))) {
		struct string_list push_options = STRING_LIST_INIT_DUP;
		struct strbuf unpack_status = STRBUF_INIT;

		nếu (use_push_options)
			read_push_options(&reader, &push_options);
		nếu (!check_cert_push_options(&push_options)) {
			struct command *cmd;
			cho (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		nếu (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		nếu (!delete_only(commands)) {
			const char *alt_shallow_file = NULL;

			nếu (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			nếu (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			không_thì
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &push_options);
		odb_transaction_finalize(transaction);
		sigchain_push(SIGPIPE, SIG_IGN);

		switch (version) {
		case REPORT_STATUS_V2:
		case REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			break;
		case REPORT_STATUS_UNKNOWN:
			break;
		}

		sigchain_pop(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, NULL,
				 &push_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&push_options, 0);
		nếu (auto_gc) {
			struct child_process proc = CHILD_PROCESS_INIT;

			nếu (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				nếu (!start_command(&proc)) {
					nếu (use_sideband)
						copy_to_sideband(proc.err, -1, NULL);
					finish_command(&proc);
				}
			}
		}
		nếu (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	nếu (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	free((void *)push_cert_nonce);
	trả_về 0;
}
