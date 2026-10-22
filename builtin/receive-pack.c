#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "commit.h"
#ضمن "commit-reach.h"
#ضمن "config.h"
#ضمن "connect.h"
#ضمن "connected.h"
#ضمن "environment.h"
#ضمن "exec-cmd.h"
#ضمن "fsck.h"
#ضمن "gettext.h"
#ضمن "gpg-interface.h"
#ضمن "hex.h"
#ضمن "hook.h"
#ضمن "object.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "oid-array.h"
#ضمن "oidset.h"
#ضمن "pack.h"
#ضمن "parse-options.h"
#ضمن "pkt-line.h"
#ضمن "protocol.h"
#ضمن "refs.h"
#ضمن "remote.h"
#ضمن "run-command.h"
#ضمن "server-info.h"
#ضمن "setup.h"
#ضمن "shallow.h"
#ضمن "sideband.h"
#ضمن "sigchain.h"
#ضمن "string-list.h"
#ضمن "strvec.h"
#ضمن "trace.h"
#ضمن "trace2.h"
#ضمن "version.h"
#ضمن "worktree.h"

ساكن ثابت حرف * ثابت receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	عدم
};

تعداد deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

تعداد report_status_version {
	REPORT_STATUS_UNKNOWN = 0,
	REPORT_STATUS_V0,
	REPORT_STATUS_V2,
};

ساكن صحيح deny_deletes;
ساكن صحيح deny_non_fast_forwards;
ساكن تعداد deny_action deny_current_branch = DENY_UNCONFIGURED;
ساكن تعداد deny_action deny_delete_current = DENY_UNCONFIGURED;
ساكن صحيح receive_fsck_objects = -1;
ساكن صحيح transfer_fsck_objects = -1;
ساكن بنية strbuf fsck_msg_types = STRBUF_INIT;
ساكن صحيح advertise_atomic_push = 1;
ساكن صحيح advertise_push_options;
ساكن صحيح advertise_sid;
ساكن off_t max_input_size;
ساكن صحيح use_sideband;
ساكن صحيح use_atomic;
ساكن صحيح use_push_options;
ساكن صحيح quiet;
ساكن صحيح prefer_ofs_delta = 1;
ساكن صحيح auto_update_server_info;
ساكن صحيح auto_gc = 1;
ساكن صحيح reject_thin;
ساكن صحيح skip_connectivity_check;
ساكن صحيح stateless_rpc;
ساكن ثابت حرف *service_dir;
ساكن ثابت حرف *head_name;
ساكن فراغ *head_name_to_free;
ساكن صحيح sent_capabilities;
ساكن صحيح shallow_update;
ساكن بنية strbuf push_cert = STRBUF_INIT;
ساكن بنية object_id push_cert_oid;
ساكن بنية signature_check sigcheck;
ساكن ثابت حرف *push_cert_nonce;
ساكن حرف *cert_nonce_seed;
ساكن بنية strvec hidden_refs = STRVEC_INIT;

ساكن ثابت حرف *NONCE_UNSOLICITED = "UNSOLICITED";
ساكن ثابت حرف *NONCE_BAD = "BAD";
ساكن ثابت حرف *NONCE_MISSING = "MISSING";
ساكن ثابت حرف *NONCE_OK = "OK";
ساكن ثابت حرف *NONCE_SLOP = "SLOP";
ساكن ثابت حرف *nonce_status;
ساكن long nonce_stamp_slop;
ساكن timestamp_t nonce_stamp_slop_limit;
ساكن بنية ref_transaction *transaction;

ساكن تعداد {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
ساكن صحيح keepalive_in_sec = 5;

ساكن بنية proc_receive_ref {
	unsigned صحيح want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	حرف *ref_prefix;
	بنية proc_receive_ref *next;
} *proc_receive_ref;

ساكن فراغ proc_receive_ref_append(ثابت حرف *prefix);

ساكن تعداد deny_action parse_deny_action(ثابت حرف *var, ثابت حرف *value)
{
	אם (value) {
		אם (!strcasecmp(value, "ignore"))
			החזר DENY_IGNORE;
		אם (!strcasecmp(value, "warn"))
			החזר DENY_WARN;
		אם (!strcasecmp(value, "refuse"))
			החזר DENY_REFUSE;
		אם (!strcasecmp(value, "updateinstead"))
			החזר DENY_UPDATE_INSTEAD;
	}
	אם (git_config_bool(var, value))
		החזר DENY_REFUSE;
	החזר DENY_IGNORE;
}

ساكن صحيح receive_pack_config(ثابت حرف *var, ثابت حرف *value,
			       ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت حرف *msg_id;
	صحيح status = parse_hide_refs_config(var, value, "receive", &hidden_refs);

	אם (status)
		החזר status;

	אם (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, value);
		החזר 0;
	}

	אם (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, value);
		החזר 0;
	}

	אם (strcmp(var, "receive.fsck.skiplist") == 0) {
		حرف *path;

		אם (git_config_pathname(&path, var, value))
			החזר -1;
		אם (path)
			strbuf_addf(&fsck_msg_types, "%cskiplist=%s",
				    fsck_msg_types.len ? ',' : '=', path);
		حرر(path);
		החזר 0;
	}

	אם (skip_prefix(var, "receive.fsck.", &msg_id)) {
		אם (!value)
			החזר config_error_nonbool(var);
		אם (is_valid_msg_type(msg_id, value))
			strbuf_addf(&fsck_msg_types, "%c%s=%s",
				fsck_msg_types.len ? ',' : '=', msg_id, value);
		אחרת
			warning("skipping unknown msg id '%s'", msg_id);
		החזר 0;
	}

	אם (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, value);
		החזר 0;
	}

	אם (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, value);
		החזר 0;
	}

	אם (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, value);
		החזר 0;
	}

	אם (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, value);
		החזר 0;
	}

	אם (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, value);
		החזר 0;
	}

	אם (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, value);
		החזר 0;
	}

	אם (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, value);
		החזר 0;
	}

	אם (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, value);
		החזר 0;
	}

	אם (strcmp(var, "receive.certnonceseed") == 0)
		החזר git_config_string(&cert_nonce_seed, var, value);

	אם (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, value, ctx->kvi);
		החזר 0;
	}

	אם (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_push = git_config_bool(var, value);
		החזר 0;
	}

	אם (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_push_options = git_config_bool(var, value);
		החזר 0;
	}

	אם (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, value, ctx->kvi);
		החזר 0;
	}

	אם (strcmp(var, "receive.maxinputsize") == 0) {
		max_input_size = git_config_int64(var, value, ctx->kvi);
		החזר 0;
	}

	אם (strcmp(var, "receive.procreceiverefs") == 0) {
		אם (!value)
			החזר config_error_nonbool(var);
		proc_receive_ref_append(value);
		החזר 0;
	}

	אם (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, value);
		החזר 0;
	}

	החזר git_default_config(var, value, ctx, cb);
}

ساكن فراغ show_ref(ثابت حرف *path, ثابت بنية object_id *oid)
{
	אם (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} אחרת {
		بنية strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		אם (advertise_atomic_push)
			strbuf_addstr(&cap, " atomic");
		אם (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		אם (push_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", push_cert_nonce);
		אם (advertise_push_options)
			strbuf_addstr(&cap, " push-options");
		אם (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

ساكن صحيح show_ref_cb(ثابت بنية reference *ref, فراغ *data)
{
	بنية oidset *seen = data;
	ثابت حرف *path = strip_namespace(ref->name);

	אם (ref_is_hidden(path, ref->name, &hidden_refs))
		החזר 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	אם (!path) {
		אם (oidset_insert(seen, ref->oid))
			החזר 0;
		path = ".have";
	} אחרת {
		oidset_insert(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	החזר 0;
}

ساكن فراغ show_one_alternate_ref(ثابت بنية object_id *oid,
				   فراغ *data)
{
	بنية oidset *seen = data;

	אם (oidset_insert(seen, oid))
		החזר;

	show_ref(".have", oid);
}

ساكن فراغ write_head_info(فراغ)
{
	بنية refs_for_each_ref_options opts = { 0 };
	ساكن بنية oidset seen = OIDSET_INIT;
	بنية strvec excludes_vector = STRVEC_INIT;

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

	אם (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#define RUN_PROC_RECEIVE_SCHEDULED	1
#define RUN_PROC_RECEIVE_RETURNED	2
بنية command {
	بنية command *next;
	ثابت حرف *error_string;
	حرف *error_string_owned;
	بنية ref_push_report *report;
	unsigned صحيح skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	صحيح index;
	بنية object_id old_oid;
	بنية object_id new_oid;
	حرف ref_name[FLEX_ARRAY]; /* more */
};

ساكن فراغ proc_receive_ref_append(ثابت حرف *prefix)
{
	بنية proc_receive_ref *ref_pattern;
	ثابت حرف *p;
	صحيح len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = strchr(prefix, ':');
	אם (p) {
		כלעוד (prefix < p) {
			אם (*prefix == 'a')
				ref_pattern->want_add = 1;
			אחרת אם (*prefix == 'd')
				ref_pattern->want_delete = 1;
			אחרת אם (*prefix == 'm')
				ref_pattern->want_modify = 1;
			אחרת אם (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} אחרת {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	כלעוד (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	אם (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} אחרת {
		بنية proc_receive_ref *end;

		end = proc_receive_ref;
		כלעוד (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

ساكن صحيح proc_receive_ref_matches(بنية command *cmd)
{
	بنية proc_receive_ref *p;

	אם (!proc_receive_ref)
		החזר 0;

	עבור (p = proc_receive_ref; p; p = p->next) {
		ثابت حرف *match = p->ref_prefix;
		ثابت حرف *remains;

		אם (!p->want_add && is_null_oid(&cmd->old_oid))
			استمر;
		אחרת אם (!p->want_delete && is_null_oid(&cmd->new_oid))
			استمر;
		אחרת אם (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			استمر;

		אם (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			אם (!p->negative_ref)
				החזר 1;
		} אחרת אם (p->negative_ref) {
			החזר 1;
		}
	}
	החזר 0;
}

ساكن فراغ report_message(ثابت حرف *prefix, ثابت حرف *err, va_list params)
{
	صحيح sz;
	حرف msg[4096];

	sz = xsnprintf(msg, حجم(msg), "%s", prefix);
	sz += vsnprintf(msg + sz, حجم(msg) - sz, err, params);
	אם (sz > (حجم(msg) - 1))
		sz = حجم(msg) - 1;
	msg[sz++] = '\n';

	אם (use_sideband)
		send_sideband(1, 2, msg, sz, use_sideband);
	אחרת
		xwrite(2, msg, sz);
}

__attribute__((format (printf, 1, 2)))
ساكن فراغ rp_warning(ثابت حرف *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("warning: ", err, params);
	va_end(params);
}

__attribute__((format (printf, 1, 2)))
ساكن فراغ rp_error(ثابت حرف *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("error: ", err, params);
	va_end(params);
}

ساكن صحيح copy_to_sideband(صحيح in, صحيح out UNUSED, فراغ *arg UNUSED)
{
	حرف data[128];
	صحيح keepalive_active = 0;

	אם (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	אם (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	כלעוד (1) {
		ssize_t sz;

		אם (keepalive_active) {
			بنية pollfd pfd;
			صحيح ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			אם (ret < 0) {
				אם (errno == EINTR)
					استمر;
				אחרת
					اكسر;
			} אחרת אם (ret == 0) {
				/* no data; send a keepalive packet */
				ساكن ثابت حرف buf[] = "0005\1";
				write_or_die(1, buf, حجم(buf) - 1);
				استمر;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, حجم(data));
		אם (sz <= 0)
			اكسر;

		אם (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			ثابت حرف *p = memchr(data, '\0', sz);
			אם (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_active = 1;
				send_sideband(1, 2, data, p - data, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - data + 1), use_sideband);
				استمر;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, data, sz, use_sideband);
	}
	close(in);
	החזר 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
ساكن فراغ prepare_sideband_async(بنية async *sideband_async, صحيح *saved_stderr, صحيح *started)
{
	*started = 0;

	אם (!use_sideband)
		החזר;

	memset(sideband_async, 0, حجم(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	אם (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		אם (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
ساكن فراغ finish_sideband_async(بنية async *sideband_async, صحيح saved_stderr, صحيح started)
{
	אם (!use_sideband)
		החזר;

	אם (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	אם (started)
		finish_async(sideband_async);
}

ساكن فراغ hmac_hash(unsigned حرف *out,
		      ثابت حرف *key_in, size_t key_len,
		      ثابت حرف *text, size_t text_len)
{
	unsigned حرف key[GIT_MAX_BLKSZ];
	unsigned حرف k_ipad[GIT_MAX_BLKSZ];
	unsigned حرف k_opad[GIT_MAX_BLKSZ];
	صحيح i;
	بنية git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(key, '\0', GIT_MAX_BLKSZ);
	אם (the_hash_algo->blksz < key_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, key_in, key_len);
		git_hash_final(key, &ctx);
	} אחרת {
		memcpy(key, key_in, key_len);
	}

	/* RFC 2104 2. (2) & (5) */
	עבור (i = 0; i < حجم(key); i++) {
		k_ipad[i] = key[i] ^ 0x36;
		k_opad[i] = key[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, حجم(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, حجم(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

ساكن حرف *prepare_push_cert_nonce(ثابت حرف *path, timestamp_t stamp)
{
	بنية strbuf buf = STRBUF_INIT;
	unsigned حرف hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, strlen(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (صحيح)the_hash_algo->hexsz, hash_to_hex(hash));
	החזר strbuf_detach(&buf, عدم);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
ساكن صحيح constant_memequal(ثابت حرف *a, ثابت حرف *b, size_t n)
{
	صحيح res = 0;
	size_t i;

	עבור (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	החזר res;
}

ساكن ثابت حرف *check_nonce(ثابت حرف *buf)
{
	size_t noncelen;
	ثابت حرف *found = find_commit_header(buf, "nonce", &noncelen);
	حرف *nonce = found ? xmemdupz(found, noncelen) : عدم;
	timestamp_t stamp, ostamp;
	حرف *bohmac, *expect = عدم;
	ثابت حرف *retval = NONCE_BAD;

	אם (!nonce) {
		retval = NONCE_MISSING;
		اذهب leave;
	} אחרת אם (!push_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		اذهب leave;
	} אחרת אם (!strcmp(push_cert_nonce, nonce)) {
		retval = NONCE_OK;
		اذهب leave;
	}

	אם (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		اذهب leave;
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
	אם (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		اذهب leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	אם (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		اذهب leave;
	}

	expect = prepare_push_cert_nonce(service_dir, stamp);
	אם (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		اذهب leave;
	}
	אם (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		اذهب leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(push_cert_nonce, عدم, 10);
	nonce_stamp_slop = (long)ostamp - (long)stamp;

	אם (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		حرر((فراغ *)push_cert_nonce);
		push_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} אחרת {
		retval = NONCE_SLOP;
	}

leave:
	حرر(nonce);
	حرر(expect);
	החזר retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
ساكن صحيح check_cert_push_options(ثابت بنية string_list *push_options)
{
	ثابت حرف *buf = push_cert.buf;

	ثابت حرف *option;
	size_t optionlen;
	صحيح options_seen = 0;

	صحيح retval = 1;

	אם (!*buf)
		החזר 1;

	כלעוד ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		אם (options_seen > push_options->nr
		    || xstrncmpz(push_options->items[options_seen - 1].string,
				 option, optionlen))
			החזר 0;
	}

	אם (options_seen != push_options->nr)
		retval = 0;

	החזר retval;
}

ساكن فراغ prepare_push_cert_sha1(بنية run_hooks_opt *opt)
{
	ساكن صحيح already_done;

	אם (!push_cert.len)
		החזר;

	אם (!already_done) {
		صحيح bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		אם (odb_write_object(the_repository->objects, push_cert.buf,
				     push_cert.len, OBJ_BLOB, &push_cert_oid))
			oidclr(&push_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', حجم(sigcheck));

		bogs = parse_signed_buffer(push_cert.buf, push_cert.len);
		sigcheck.payload = xmemdupz(push_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, push_cert.buf + bogs,
				push_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	אם (!is_null_oid(&push_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&push_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.key ? sigcheck.key : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		אם (push_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     push_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			אם (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

بنية receive_hook_feed_state {
	بنية command *cmd;
	بنية ref_push_report *report;
	صحيح skip_broken;
	بنية strbuf buf;
};

ساكن صحيح feed_receive_hook_cb(صحيح hook_stdin_fd, فراغ *pp_cb UNUSED, فراغ *pp_task_cb)
{
	بنية receive_hook_feed_state *state = pp_task_cb;
	بنية command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	כלעוד (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	אם (!cmd)
		החזר 1;  /* no more commands left */

	אם (!state->report)
		state->report = cmd->report;

	אם (state->report) {
		بنية object_id *old_oid;
		بنية object_id *new_oid;
		ثابت حرف *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->next;
		אם (!state->report)
			cmd = cmd->next;
	} אחרת {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	אם (state->buf.len > 0) {
		صحيح ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		אם (ret < 0) {
			אם (errno == EPIPE)
				החזר 1; /* child closed pipe */
			החזר ret;
		}
	}

	החזר state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

ساكن فراغ *receive_hook_feed_state_alloc(فراغ *feed_pipe_ctx)
{
	بنية receive_hook_feed_state *init_state = feed_pipe_ctx;
	بنية receive_hook_feed_state *data;

	CALLOC_ARRAY(data, 1);
	data->report = init_state->report;
	data->cmd = init_state->cmd;
	data->skip_broken = init_state->skip_broken;
	strbuf_init(&data->buf, 0);

	החזר data;
}

ساكن فراغ receive_hook_feed_state_free(فراغ *data)
{
	بنية receive_hook_feed_state *d = data;
	אם (!d)
		החזר;
	strbuf_release(&d->buf);
	حرر(d);
}

ساكن صحيح run_receive_hook(بنية command *commands,
			    ثابت حرف *hook_name,
			    صحيح skip_broken,
			    بنية odb_transaction *transaction,
			    ثابت بنية string_list *push_options)
{
	بنية run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	بنية command *iter = commands;
	بنية receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	بنية async sideband_async;
	صحيح sideband_async_started = 0;
	صحيح saved_stderr = -1;
	صحيح ret;

	אם (!hook_exists(the_repository, hook_name))
		החזר 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	כלעוד (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	אם (!iter)
		החזר 0;

	אם (push_options) {
		עבור (صحيح i = 0; i < push_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     push_options->items[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)push_options->nr);
	} אחרת {
		strvec_push(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	אם (transaction)
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

	החזר ret;
}

ساكن صحيح run_update_hook(بنية command *cmd)
{
	ساكن ثابت حرف hook_name[] = "update";
	بنية run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	بنية async sideband_async;
	صحيح sideband_async_started = 0;
	صحيح saved_stderr = -1;
	صحيح code;

	אם (!hook_exists(the_repository, hook_name))
		החזר 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     عدم);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	החזר code;
}

ساكن صحيح run_receive_report_hook(بنية strbuf *report)
{
	بنية child_process proc = CHILD_PROCESS_INIT;
	بنية async sideband_async;
	صحيح sideband_async_started = 0;
	صحيح saved_stderr = -1;
	بنية strbuf out = STRBUF_INIT;
	ثابت حرف *hook_path;
	صحيح ret;

	hook_path = find_hook(the_repository, "receive-report");
	אם (!hook_path)
		החזר 0;

	strvec_push(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_push(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, عدم, 0);
	sigchain_pop(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	אם (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	החזר ret;
}

ساكن بنية command *find_command_by_refname(بنية command *list,
					       ثابت حرف *refname)
{
	עבור (; list; list = list->next)
		אם (!strcmp(list->ref_name, refname))
			החזר list;
	החזר عدم;
}

ساكن صحيح read_proc_receive_report(بنية packet_reader *reader,
				    بنية command *commands,
				    بنية strbuf *errmsg)
{
	بنية command *cmd;
	بنية command *hint = عدم;
	بنية ref_push_report *report = عدم;
	صحيح new_report = 0;
	صحيح code = 0;
	صحيح once = 0;
	صحيح response = 0;

	עבור (;;) {
		بنية object_id old_oid, new_oid;
		حرف *head;
		حرف *refname;
		حرف *p;
		تعداد packet_read_status status;

		status = packet_reader_read(reader);
		אם (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			אם (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				החזר -1;
			}
			اكسر;
		}
		response++;

		head = reader->line;
		p = strchr(head, ' ');
		אם (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			استمر;
		}
		*p++ = '\0';
		אם (!strcmp(head, "option")) {
			حرف *key;
			ثابت حرف *val;

			אם (!hint || !(report || new_report)) {
				אם (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				استمر;
			}
			אם (new_report) {
				אם (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} אחרת {
					report = hint->report;
					כלעוד (report->next)
						report = report->next;
					report->next = xcalloc(1, حجم(بنية ref_push_report));
					report = report->next;
				}
				new_report = 0;
			}
			key = p;
			p = strchr(key, ' ');
			אם (p)
				*p++ = '\0';
			val = p;
			אם (!strcmp(key, "refname"))
				report->ref_name = xstrdup_or_null(val);
			אחרת אם (!strcmp(key, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			אחרת אם (!strcmp(key, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			אחרת אם (!strcmp(key, "forced-update"))
				report->forced_update = 1;
			אחרת אם (!strcmp(key, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			استمر;
		}

		report = عدم;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		אם (p)
			*p++ = '\0';
		אם (strcmp(head, "ok") && strcmp(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			استمر;
		}

		/* first try searching at our hint, falling back to all refs */
		אם (hint)
			hint = find_command_by_refname(hint, refname);
		אם (!hint)
			hint = find_command_by_refname(commands, refname);
		אם (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			استمر;
		}
		אם (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			استمر;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		אם (!strcmp(head, "ng")) {
			אם (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			אחרת
				hint->error_string = "failed";
			code = -1;
			استمر;
		}
		new_report = 1;
	}

	עבור (cmd = commands; cmd; cmd = cmd->next)
		אם (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	החזר code;
}

ساكن صحيح run_proc_receive_hook(بنية command *commands,
				 ثابت بنية string_list *push_options)
{
	بنية child_process proc = CHILD_PROCESS_INIT;
	بنية async muxer;
	بنية command *cmd;
	بنية packet_reader reader;
	بنية strbuf cap = STRBUF_INIT;
	بنية strbuf errmsg = STRBUF_INIT;
	صحيح hook_use_push_options = 0;
	صحيح version = 0;
	صحيح code;
	ثابت حرف *hook_path = find_hook(the_repository, "proc-receive");

	אם (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		החזר -1;
	}

	strvec_push(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	אם (use_sideband) {
		memset(&muxer, 0, حجم(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		אם (code)
			החזר code;
		proc.err = muxer.in;
	} אחרת {
		proc.err = 0;
	}

	code = start_command(&proc);
	אם (code) {
		אם (use_sideband)
			finish_async(&muxer);
		החזר code;
	}

	sigchain_push(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	אם (use_atomic)
		strbuf_addstr(&cap, " atomic");
	אם (use_push_options)
		strbuf_addstr(&cap, " push-options");
	אם (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} אחרת {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	אם (!code)
		code = packet_flush_gently(proc.in);

	אם (!code)
		עבור (;;) {
			صحيح linelen;
			تعداد packet_read_status status;

			status = packet_reader_read(&reader);
			אם (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				אם (status == PACKET_READ_EOF)
					code = -1;
				اكسر;
			}

			אם (reader.pktlen > 8 && starts_with(reader.line, "version=")) {
				version = atoi(reader.line + 8);
				linelen = strlen(reader.line);
				אם (linelen < reader.pktlen) {
					ثابت حرف *feature_list = reader.line + linelen + 1;
					אם (parse_feature_request(feature_list, "push-options"))
						hook_use_push_options = 1;
				}
			}
		}

	אם (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		اذهب cleanup;
	}

	بدل (version) {
	حالة 0:
		/* fallthrough */
	حالة 1:
		اكسر;
	افتراضي:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		اذهب cleanup;
	}

	/* Send commands */
	עבור (cmd = commands; cmd; cmd = cmd->next) {
		אם (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			استمر;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		אם (code)
			اكسر;
	}
	אם (!code)
		code = packet_flush_gently(proc.in);
	אם (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		اذهب cleanup;
	}

	/* Send push options */
	אם (hook_use_push_options) {
		بنية string_list_item *item;

		for_each_string_list_item(item, push_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			אם (code)
				اكسر;
		}
		אם (!code)
			code = packet_flush_gently(proc.in);
		אם (code) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			اذهب cleanup;
		}
	}

	/* Read result from proc-receive */
	code = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.in);
	close(proc.out);
	אם (use_sideband)
		finish_async(&muxer);
	אם (finish_command(&proc))
		code = -1;
	אם (errmsg.len >0) {
		حرف *p = errmsg.buf;

		p += errmsg.len - 1;
		אם (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_pop(SIGPIPE);

	החזר code;
}

ساكن ثابت حرف *refuse_unconfigured_deny_msg =
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

ساكن فراغ refuse_unconfigured_deny(فراغ)
{
	rp_error("%s", _(refuse_unconfigured_deny_msg));
}

ساكن ثابت حرف *refuse_unconfigured_deny_delete_current_msg =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

ساكن فراغ refuse_unconfigured_deny_delete_current(فراغ)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_msg));
}

ساكن ثابت بنية object_id *command_singleton_iterator(فراغ *cb_data);
ساكن صحيح update_shallow_ref(بنية command *cmd, بنية shallow_info *si)
{
	بنية shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	بنية oid_array extra = OID_ARRAY_INIT;
	بنية check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	صحيح i;

	trace_printf_key(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	עבור (i = 0; i < si->shallow->nr; i++)
		אם (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	אם (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		החזר -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	עבור (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	החזר 0;
}

ساكن ثابت حرف *push_to_deploy(unsigned حرف *sha1,
				  بنية strvec *env,
				  ثابت بنية worktree *worktree)
{
	بنية child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", عدم);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	אם (run_command(&child))
		החזר "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", عدم);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	אם (run_command(&child))
		החזר "Working directory has unstaged changes";

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
		     empty_tree_oid_hex(the_repository->hash_algo), "--", عدم);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	אם (run_command(&child))
		החזר "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     عدم);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	אם (run_command(&child))
		החזר "Could not update working tree to new HEAD";

	החזר عدم;
}

ساكن ثابت حرف *push_to_checkout_hook = "push-to-checkout";

ساكن ثابت حرف *push_to_checkout(unsigned حرف *hash,
				    صحيح *invoked_hook,
				    بنية strvec *env,
				    ثابت حرف *work_tree)
{
	بنية run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_tree));
	strvec_push(&opt.args, hash_to_hex(hash));
	אם (run_hooks_opt(the_repository, push_to_checkout_hook, &opt))
		החזר "push-to-checkout hook declined";
	אחרת
		החזר عدم;
}

ساكن ثابت حرف *update_worktree(unsigned حرف *sha1, ثابت بنية worktree *worktree)
{
	ثابت حرف *retval;
	حرف *git_dir;
	بنية strvec env = STRVEC_INIT;
	صحيح invoked_hook;

	אם (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	אם (worktree->is_bare)
		החזר "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = push_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	אם (!invoked_hook)
		retval = push_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	حرر(git_dir);
	החזר retval;
}

ساكن ثابت حرف *update(بنية command *cmd, بنية shallow_info *si)
{
	ثابت حرف *name = cmd->ref_name;
	بنية strbuf namespaced_name_buf = STRBUF_INIT;
	ساكن حرف *namespaced_name;
	ثابت حرف *ret;
	بنية object_id *old_oid = &cmd->old_oid;
	بنية object_id *new_oid = &cmd->new_oid;
	صحيح do_update_worktree = 0;
	بنية worktree **worktrees = get_worktrees(the_repository);
	ثابت بنية worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	אם (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		اذهب out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	حرر(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, عدم);

	אם (worktree && !worktree->is_bare) {
		بدل (deny_current_branch) {
		حالة DENY_IGNORE:
			اكسر;
		حالة DENY_WARN:
			rp_warning("updating the current branch");
			اكسر;
		حالة DENY_REFUSE:
		حالة DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			אם (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			اذهب out;
		حالة DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			اكسر;
		}
	}

	אם (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		اذهب out;
	}

	אם (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		אם (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			اذهب out;
		}

		אם (worktree || (head_name && !strcmp(namespaced_name, head_name))) {
			بدل (deny_delete_current) {
			حالة DENY_IGNORE:
				اكسر;
			حالة DENY_WARN:
				rp_warning("deleting the current branch");
				اكسر;
			حالة DENY_REFUSE:
			حالة DENY_UNCONFIGURED:
			حالة DENY_UPDATE_INSTEAD:
				אם (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				اذهب out;
			افتراضي:
				ret = "Invalid denyDeleteCurrent setting";
				اذهب out;
			}
		}
	}

	אם (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		بنية object *old_object, *new_object;
		بنية commit *old_commit, *new_commit;
		صحيح ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		אם (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			اذهب out;
		}
		old_commit = (بنية commit *)old_object;
		new_commit = (بنية commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		אם (ret2 < 0)
			exit(128);
		אם (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			اذهب out;
		}
	}
	אם (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		اذهب out;
	}

	אם (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		אם (ret)
			اذهب out;
	}

	אם (is_null_oid(new_oid)) {
		بنية strbuf err = STRBUF_INIT;
		אם (!parse_object(the_repository, old_oid)) {
			old_oid = عدم;
			אם (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} אחרת {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		אם (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   عدم, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} אחרת {
			ret = عدم; /* good */
		}
		strbuf_release(&err);
	} אחרת {
		تعداد ref_transaction_error tx_err;
		بنية strbuf err = STRBUF_INIT;
		אם (shallow_update && si->shallow_ref[cmd->index] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			اذهب out;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  عدم, عدم,
						  0, "push",
						  &err);
		אם (tx_err) {
			rp_error("%s", err.buf);
			אם (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			אחרת
				ret = ref_transaction_error_msg(tx_err);
		} אחרת {
			ret = عدم; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	החזר ret;
}

ساكن فراغ run_update_post_hook(بنية command *commands)
{
	ساكن ثابت حرف hook_name[] = "post-update";
	بنية run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	بنية async sideband_async;
	بنية command *cmd;
	صحيح sideband_async_started = 0;
	صحيح saved_stderr = -1;

	אם (!hook_exists(the_repository, hook_name))
		החזר;

	עבור (cmd = commands; cmd; cmd = cmd->next) {
		אם (cmd->error_string || cmd->did_not_exist)
			استمر;
		strvec_push(&opt.args, cmd->ref_name);
	}
	אם (!opt.args.nr)
		החזר;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

ساكن فراغ check_aliased_update_internal(بنية command *cmd,
					  بنية string_list *list,
					  ثابت حرف *dst_name, صحيح flag)
{
	بنية string_list_item *item;
	بنية command *dst_cmd;

	אם (!(flag & REF_ISSYMREF))
		החזר;

	אם (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		החזר;
	}
	dst_name = strip_namespace(dst_name);

	אם (!(item = string_list_lookup(list, dst_name)))
		החזר;

	cmd->skip_update = 1;

	dst_cmd = (بنية command *) item->util;

	אם (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		החזר;

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

ساكن فراغ check_aliased_update(بنية command *cmd, بنية string_list *list)
{
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *dst_name;
	صحيح flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, عدم, &flag);
	check_aliased_update_internal(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

ساكن فراغ check_aliased_updates(بنية command *commands)
{
	بنية command *cmd;
	بنية string_list ref_list = STRING_LIST_INIT_NODUP;

	עבור (cmd = commands; cmd; cmd = cmd->next) {
		بنية string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (فراغ *)cmd;
	}
	string_list_sort(&ref_list);

	עבור (cmd = commands; cmd; cmd = cmd->next) {
		אם (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

ساكن ثابت بنية object_id *command_singleton_iterator(فراغ *cb_data)
{
	بنية command **cmd_list = cb_data;
	بنية command *cmd = *cmd_list;

	אם (!cmd || is_null_oid(&cmd->new_oid))
		החזר عدم;
	*cmd_list = عدم; /* this returns only one */
	החזר &cmd->new_oid;
}

ساكن فراغ set_connectivity_errors(بنية command *commands,
				    بنية shallow_info *si,
				    بنية odb_transaction *transaction)
{
	بنية command *cmd;

	עבור (cmd = commands; cmd; cmd = cmd->next) {
		بنية command *singleton = cmd;
		بنية check_connected_options opt = CHECK_CONNECTED_INIT;
		بنية strvec env = STRVEC_INIT;

		אם (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			استمر;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		אם (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			استمر;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

بنية iterate_data {
	بنية command *cmds;
	بنية shallow_info *si;
};

ساكن ثابت بنية object_id *iterate_receive_command_list(فراغ *cb_data)
{
	بنية iterate_data *data = cb_data;
	بنية command **cmd_list = &data->cmds;
	بنية command *cmd = *cmd_list;

	עבור (; cmd; cmd = cmd->next) {
		אם (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			استمر;
		אם (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			החזר &cmd->new_oid;
		}
	}
	החזר عدم;
}

ساكن فراغ reject_updates_to_hidden(بنية command *commands)
{
	بنية strbuf refname_full = STRBUF_INIT;
	size_t prefix_len;
	بنية command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	עבור (cmd = commands; cmd; cmd = cmd->next) {
		אם (cmd->error_string)
			استمر;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		אם (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			استمر;
		אם (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		אחרת
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

ساكن صحيح should_process_cmd(بنية command *cmd)
{
	החזר !cmd->error_string && !cmd->skip_update;
}

ساكن فراغ BUG_if_skipped_connectivity_check(بنية command *commands,
					       بنية shallow_info *si)
{
	بنية command *cmd;

	עבור (cmd = commands; cmd; cmd = cmd->next) {
		אם (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

ساكن فراغ ref_transaction_rejection_handler(ثابت حرف *refname,
					      ثابت بنية object_id *old_oid UNUSED,
					      ثابت بنية object_id *new_oid UNUSED,
					      ثابت حرف *old_target UNUSED,
					      ثابت حرف *new_target UNUSED,
					      تعداد ref_transaction_error err,
					      ثابت حرف *details,
					      فراغ *cb_data)
{
	بنية strmap *failed_refs = cb_data;

	אם (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (حرف *)ref_transaction_error_msg(err));
}

ساكن فراغ execute_commands_non_atomic(بنية command *commands,
					بنية shallow_info *si)
{
	بنية command *cmd;
	بنية strbuf err = STRBUF_INIT;
	ثابت حرف *reported_error = عدم;
	بنية strmap failed_refs = STRMAP_INIT;

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
	تعداد processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	עבור (تعداد processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		עבור (cmd = commands; cmd; cmd = cmd->next) {
			אם (!should_process_cmd(cmd) || cmd->run_proc_receive)
				استمر;

			אם (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				استمر;
			אחרת אם (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				استمر;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			אם (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				אם (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					اذهب failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		אם (!transaction)
			اذهب cleanup;

		אם (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			اذهب failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		אם (strmap_empty(&failed_refs))
			اذهب cleanup;

	failure:
		עבור (cmd = commands; cmd; cmd = cmd->next) {
			אם (reported_error)
				cmd->error_string = reported_error;
			אחרת אם (strmap_contains(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = عدم;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

ساكن فراغ execute_commands_atomic(بنية command *commands,
					بنية shallow_info *si)
{
	بنية command *cmd;
	بنية strbuf err = STRBUF_INIT;
	ثابت حرف *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	אם (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		اذهب failure;
	}

	עבור (cmd = commands; cmd; cmd = cmd->next) {
		אם (!should_process_cmd(cmd) || cmd->run_proc_receive)
			استمر;

		cmd->error_string = update(cmd, si);

		אם (cmd->error_string)
			اذهب failure;
	}

	אם (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		اذهب failure;
	}
	اذهب cleanup;

failure:
	עבור (cmd = commands; cmd; cmd = cmd->next)
		אם (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

ساكن فراغ execute_commands(بنية command *commands,
			     صحيح unpacker_error,
			     بنية shallow_info *si,
			     بنية odb_transaction *transaction,
			     ثابت بنية string_list *push_options)
{
	بنية check_connected_options opt = CHECK_CONNECTED_INIT;
	بنية command *cmd;
	بنية iterate_data data;
	بنية async muxer;
	صحيح err_fd = 0;
	صحيح run_proc_receive = 0;

	אם (unpacker_error) {
		עבור (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		החזר;
	}

	אם (!skip_connectivity_check) {
		بنية strvec env = STRVEC_INIT;

		אם (use_sideband) {
			memset(&muxer, 0, حجم(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			אם (!start_async(&muxer))
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

		אם (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		אם (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	אם (proc_receive_ref) {
		עבור (cmd = commands; cmd; cmd = cmd->next) {
			אם (!should_process_cmd(cmd))
				استمر;

			אם (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	אם (run_receive_hook(commands, "pre-receive", 0, transaction, push_options)) {
		עבור (cmd = commands; cmd; cmd = cmd->next) {
			אם (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		החזר;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	עבור (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	אם (!cmd)
		החזר;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	אם (odb_transaction_commit(transaction)) {
		עבור (cmd = commands; cmd; cmd = cmd->next) {
			אם (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		החזר;
	}

	check_aliased_updates(commands);

	حرر(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, عدم,
							    عدم);

	אם (run_proc_receive &&
	    run_proc_receive_hook(commands, push_options))
		עבור (cmd = commands; cmd; cmd = cmd->next)
			אם (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	אם (use_atomic)
		execute_commands_atomic(commands, si);
	אחרת
		execute_commands_non_atomic(commands, si);

	אם (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

ساكن بنية command **queue_command(بنية command **tail,
				      ثابت حرف *line,
				      صحيح linelen)
{
	بنية object_id old_oid, new_oid;
	بنية command *cmd;
	ثابت حرف *refname;
	صحيح reflen;
	ثابت حرف *p;

	אם (parse_oid_hex(line, &old_oid, &p) ||
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
	החזר &cmd->next;
}

ساكن فراغ free_commands(بنية command *commands)
{
	כלעוד (commands) {
		بنية command *next = commands->next;

		ref_push_report_free(commands->report);
		حرر(commands->error_string_owned);
		حرر(commands);
		commands = next;
	}
}

ساكن فراغ queue_commands_from_cert(بنية command **tail,
				     بنية strbuf *push_cert)
{
	ثابت حرف *boc, *eoc;

	אם (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(push_cert->buf, "\n\n");
	אם (!boc)
		die("malformed push certificate %.*s", 100, push_cert->buf);
	אחרת
		boc += 2;
	eoc = push_cert->buf + parse_signed_buffer(push_cert->buf, push_cert->len);

	כלעוד (boc < eoc) {
		ثابت حرف *eol = memchr(boc, '\n', eoc - boc);
		tail = queue_command(tail, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

ساكن بنية command *read_head_info(بنية packet_reader *reader,
				      بنية oid_array *shallow,
				      تعداد report_status_version *version)
{
	بنية command *commands = عدم;
	بنية command **p = &commands;
	עבור (;;) {
		صحيح linelen;

		אם (packet_reader_read(reader) != PACKET_READ_NORMAL)
			اكسر;

		אם (reader->pktlen > 8 && starts_with(reader->line, "shallow ")) {
			بنية object_id oid;
			אם (get_oid_hex(reader->line + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->line + 8);
			oid_array_append(shallow, &oid);
			استمر;
		}

		linelen = strlen(reader->line);
		אם (linelen < reader->pktlen) {
			ثابت حرف *feature_list = reader->line + linelen + 1;
			ثابت حرف *hash = عدم;
			ثابت حرف *client_sid;
			size_t len = 0;
			אם (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			אם (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			אם (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			אם (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			אם (advertise_atomic_push
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			אם (advertise_push_options
			    && parse_feature_request(feature_list, "push-options"))
				use_push_options = 1;
			hash = parse_feature_value(feature_list, "object-format", &len, عدم);
			אם (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			אם (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_value(feature_list, "session-id", &len, عدم);
			אם (client_sid) {
				حرف *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", عدم, "client-sid", client_sid);
				حرر(sid);
			}
		}

		אם (!strcmp(reader->line, "push-cert")) {
			صحيح true_flush = 0;
			صحيح saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			עבור (;;) {
				packet_reader_read(reader);
				אם (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					اكسر;
				}
				אם (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				אם (!strcmp(reader->line, "push-cert-end\n"))
					اكسر; /* end of cert */
				strbuf_addstr(&push_cert, reader->line);
			}
			reader->options = saved_options;

			אם (true_flush)
				اكسر;
			استمر;
		}

		p = queue_command(p, reader->line, linelen);
	}

	אם (push_cert.len)
		queue_commands_from_cert(p, &push_cert);

	החזר commands;
}

ساكن فراغ read_push_options(بنية packet_reader *reader,
			      بنية string_list *options)
{
	כלעוד (1) {
		אם (packet_reader_read(reader) != PACKET_READ_NORMAL)
			اكسر;

		string_list_append(options, reader->line);
	}
}

ساكن صحيح unpack_with_sideband(بنية odb_transaction *transaction,
				ثابت حرف *shallow_file,
				بنية strbuf *err_msg)
{
	بنية odb_transaction_write_pack_opts opts = {
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
	بنية async muxer;
	صحيح ret;

	אם (!use_sideband)
		החזר odb_transaction_write_pack(transaction, 0, err_msg, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, حجم(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	אם (start_async(&muxer))
		החזר 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_msg, &opts);

	finish_async(&muxer);
	החזר ret;
}

ساكن فراغ prepare_shallow_update(بنية shallow_info *si,
				   ثابت حرف *shallow_file)
{
	صحيح i, j, k, bitmap_size = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, عدم);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	עבור (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	עבור (i = 0; i < si->shallow->nr; i++) {
		אם (!si->used_shallow[i])
			استمر;
		עבור (j = 0; j < bitmap_size; j++) {
			אם (!si->used_shallow[i][j])
				استمر;
			si->need_reachability_test[i]++;
			עבור (k = 0; k < 32; k++)
				אם (si->used_shallow[i][j] & (1U << k))
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

ساكن فراغ update_shallow_info(بنية command *commands,
				بنية shallow_info *si,
				بنية oid_array *ref,
				ثابت حرف *shallow_file)
{
	بنية command *cmd;
	صحيح *ref_status;
	remove_nonexistent_theirs_shallow(si);
	אם (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		החזר;
	}

	עבור (cmd = commands; cmd; cmd = cmd->next) {
		אם (is_null_oid(&cmd->new_oid))
			استمر;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	אם (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		החזר;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, عدم, ref_status);
	עבור (cmd = commands; cmd; cmd = cmd->next) {
		אם (is_null_oid(&cmd->new_oid))
			استمر;
		אם (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	حرر(ref_status);
}

ساكن فراغ override_cmds_error(بنية command *commands, ثابت حرف *err)
{
	עבור (بنية command *cmd = commands; cmd; cmd = cmd->next) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
ساكن فراغ generate_report(بنية strbuf *buf, بنية command *commands,
			    ثابت بنية strbuf *unpack_status,
			    تعداد report_status_version version)
{
	بنية command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	עבור (cmd = commands; cmd; cmd = cmd->next) {
		بنية ref_push_report *report;
		صحيح count = 0;

		אם (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		אחרת
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		אם (version != REPORT_STATUS_V2 || cmd->error_string)
			استمر;

		עבור (report = cmd->report; report; report = report->next) {
			אם (count++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			אם (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			אם (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			אם (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			אם (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

ساكن فراغ report(بنية command *commands, ثابت بنية strbuf *unpack_status,
		   تعداد report_status_version version)
{
	بنية strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	אם (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	אם (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	אחרת
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

ساكن صحيح delete_only(بنية command *commands)
{
	بنية command *cmd;
	עבור (cmd = commands; cmd; cmd = cmd->next) {
		אם (!is_null_oid(&cmd->new_oid))
			החזר 0;
	}
	החזר 1;
}

صحيح cmd_receive_pack(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *prefix,
		     بنية repository *repo UNUSED)
{
	صحيح advertise_refs = 0;
	بنية command *commands;
	بنية oid_array shallow = OID_ARRAY_INIT;
	بنية oid_array ref = OID_ARRAY_INIT;
	بنية shallow_info si;
	بنية packet_reader reader;
	بنية odb_transaction *transaction = عدم;
	تعداد report_status_version version = REPORT_STATUS_UNKNOWN;

	بنية option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, عدم),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, عدم),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, عدم),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, عدم),
		OPT_END()
	};

	packet_trace_identity("receive-pack");

	argc = parse_options(argc, argv, prefix, options, receive_pack_usage, 0);

	אם (argc > 1)
		usage_msg_opt(_("too many arguments"), receive_pack_usage, options);
	אם (argc == 0)
		usage_msg_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	אם (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, عدم);
	אם (cert_nonce_seed)
		push_cert_nonce = prepare_push_cert_nonce(service_dir, time(عدم));

	بدل (determine_protocol_version_server()) {
	حالة protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		اكسر;
	حالة protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		אם (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	حالة protocol_v0:
		اكسر;
	حالة protocol_unknown_version:
		BUG("unknown protocol version");
	}

	אם (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	אם (advertise_refs)
		החזר 0;

	packet_reader_init(&reader, 0, عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	אם ((commands = read_head_info(&reader, &shallow, &version))) {
		بنية string_list push_options = STRING_LIST_INIT_DUP;
		بنية strbuf unpack_status = STRBUF_INIT;

		אם (use_push_options)
			read_push_options(&reader, &push_options);
		אם (!check_cert_push_options(&push_options)) {
			بنية command *cmd;
			עבור (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		אם (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		אם (!delete_only(commands)) {
			ثابت حرف *alt_shallow_file = عدم;

			אם (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			אם (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			אחרת
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &push_options);
		odb_transaction_finalize(transaction);
		sigchain_push(SIGPIPE, SIG_IGN);

		بدل (version) {
		حالة REPORT_STATUS_V2:
		حالة REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			اكسر;
		حالة REPORT_STATUS_UNKNOWN:
			اكسر;
		}

		sigchain_pop(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, عدم,
				 &push_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&push_options, 0);
		אם (auto_gc) {
			بنية child_process proc = CHILD_PROCESS_INIT;

			אם (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				אם (!start_command(&proc)) {
					אם (use_sideband)
						copy_to_sideband(proc.err, -1, عدم);
					finish_command(&proc);
				}
			}
		}
		אם (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	אם (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	حرر((فراغ *)push_cert_nonce);
	החזר 0;
}
