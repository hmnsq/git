#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "commit.h"
#شامل "commit-reach.h"
#شامل "config.h"
#شامل "connect.h"
#شامل "connected.h"
#شامل "environment.h"
#شامل "exec-cmd.h"
#شامل "fsck.h"
#شامل "gettext.h"
#شامل "gpg-interface.h"
#شامل "hex.h"
#شامل "hook.h"
#شامل "object.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "oid-array.h"
#شامل "oidset.h"
#شامل "pack.h"
#شامل "parse-options.h"
#شامل "pkt-line.h"
#شامل "protocol.h"
#شامل "refs.h"
#شامل "remote.h"
#شامل "run-command.h"
#شامل "server-info.h"
#شامل "setup.h"
#شامل "shallow.h"
#شامل "sideband.h"
#شامل "sigchain.h"
#شامل "string-list.h"
#شامل "strvec.h"
#شامل "trace.h"
#شامل "trace2.h"
#شامل "version.h"
#شامل "worktree.h"

ایستا ثابت نویسه * ثابت receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	پوچ
};

شمارش deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

شمارش report_status_version {
	REPORT_STATUS_UNKNOWN = 0,
	REPORT_STATUS_V0,
	REPORT_STATUS_V2,
};

ایستا صحیح deny_deletes;
ایستا صحیح deny_non_fast_forwards;
ایستا شمارش deny_action deny_current_branch = DENY_UNCONFIGURED;
ایستا شمارش deny_action deny_delete_current = DENY_UNCONFIGURED;
ایستا صحیح receive_fsck_objects = -1;
ایستا صحیح transfer_fsck_objects = -1;
ایستا ساختار strbuf fsck_پیام_types = STRBUF_INIT;
ایستا صحیح advertise_atomic_افزودن = 1;
ایستا صحیح advertise_افزودن_options;
ایستا صحیح advertise_sid;
ایستا off_t max_درون_داد_size;
ایستا صحیح use_sideband;
ایستا صحیح use_atomic;
ایستا صحیح use_افزودن_options;
ایستا صحیح quiet;
ایستا صحیح prefer_ofs_delta = 1;
ایستا صحیح auto_update_server_info;
ایستا صحیح auto_gc = 1;
ایستا صحیح reject_thin;
ایستا صحیح skip_connectivity_check;
ایستا صحیح stateless_rpc;
ایستا ثابت نویسه *service_dir;
ایستا ثابت نویسه *head_name;
ایستا تهی *head_name_to_free;
ایستا صحیح sent_capabilities;
ایستا صحیح shallow_update;
ایستا ساختار strbuf افزودن_cert = STRBUF_INIT;
ایستا ساختار object_id افزودن_cert_oid;
ایستا ساختار signature_check sigcheck;
ایستا ثابت نویسه *افزودن_cert_nonce;
ایستا نویسه *cert_nonce_seed;
ایستا ساختار strvec hidden_refs = STRVEC_INIT;

ایستا ثابت نویسه *NONCE_UNSOLICITED = "UNSOLICITED";
ایستا ثابت نویسه *NONCE_BAD = "BAD";
ایستا ثابت نویسه *NONCE_MISSING = "MISSING";
ایستا ثابت نویسه *NONCE_OK = "OK";
ایستا ثابت نویسه *NONCE_SLOP = "SLOP";
ایستا ثابت نویسه *nonce_status;
ایستا long nonce_stamp_slop;
ایستا timestamp_t nonce_stamp_slop_limit;
ایستا ساختار ref_transaction *transaction;

ایستا شمارش {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
ایستا صحیح keepalive_in_sec = 5;

ایستا ساختار proc_receive_ref {
	unsigned صحیح want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	نویسه *ref_prefix;
	ساختار proc_receive_ref *next;
} *proc_receive_ref;

ایستا تهی proc_receive_ref_append(ثابت نویسه *prefix);

ایستا شمارش deny_action parse_deny_action(ثابت نویسه *var, ثابت نویسه *مقدار)
{
	اگر (مقدار) {
		اگر (!strcasecmp(مقدار, "ignore"))
			بازگشت DENY_IGNORE;
		اگر (!strcasecmp(مقدار, "warn"))
			بازگشت DENY_WARN;
		اگر (!strcasecmp(مقدار, "refuse"))
			بازگشت DENY_REFUSE;
		اگر (!strcasecmp(مقدار, "updateinstead"))
			بازگشت DENY_UPDATE_INSTEAD;
	}
	اگر (git_config_bool(var, مقدار))
		بازگشت DENY_REFUSE;
	بازگشت DENY_IGNORE;
}

ایستا صحیح receive_pack_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			       ثابت ساختار config_context *ctx, تهی *cb)
{
	ثابت نویسه *پیام_id;
	صحیح status = parse_hide_refs_config(var, مقدار, "receive", &hidden_refs);

	اگر (status)
		بازگشت status;

	اگر (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.fsck.skiplist") == 0) {
		نویسه *path;

		اگر (git_config_pathname(&path, var, مقدار))
			بازگشت -1;
		اگر (path)
			strbuf_addf(&fsck_پیام_types, "%cskiplist=%s",
				    fsck_پیام_types.len ? ',' : '=', path);
		آزاد(path);
		بازگشت 0;
	}

	اگر (skip_prefix(var, "receive.fsck.", &پیام_id)) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		اگر (is_valid_پیام_type(پیام_id, مقدار))
			strbuf_addf(&fsck_پیام_types, "%c%s=%s",
				fsck_پیام_types.len ? ',' : '=', پیام_id, مقدار);
		وگرنه
			warning("skipping unknown msg id '%s'", پیام_id);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, مقدار);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, مقدار);
		بازگشت 0;
	}

	اگر (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.certnonceseed") == 0)
		بازگشت git_config_string(&cert_nonce_seed, var, مقدار);

	اگر (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, مقدار, ctx->kvi);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_افزودن = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_افزودن_options = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, مقدار, ctx->kvi);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.maxinputsize") == 0) {
		max_درون_داد_size = git_config_int64(var, مقدار, ctx->kvi);
		بازگشت 0;
	}

	اگر (strcmp(var, "receive.procreceiverefs") == 0) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		proc_receive_ref_append(مقدار);
		بازگشت 0;
	}

	اگر (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

ایستا تهی show_ref(ثابت نویسه *path, ثابت ساختار object_id *oid)
{
	اگر (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} وگرنه {
		ساختار strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		اگر (advertise_atomic_افزودن)
			strbuf_addstr(&cap, " atomic");
		اگر (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		اگر (افزودن_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", افزودن_cert_nonce);
		اگر (advertise_افزودن_options)
			strbuf_addstr(&cap, " push-options");
		اگر (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

ایستا صحیح show_ref_cb(ثابت ساختار reference *ref, تهی *data)
{
	ساختار oidset *seen = data;
	ثابت نویسه *path = strip_namespace(ref->name);

	اگر (ref_is_hidden(path, ref->name, &hidden_refs))
		بازگشت 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	اگر (!path) {
		اگر (oidset_درج(seen, ref->oid))
			بازگشت 0;
		path = ".have";
	} وگرنه {
		oidset_درج(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	بازگشت 0;
}

ایستا تهی show_one_alternate_ref(ثابت ساختار object_id *oid,
				   تهی *data)
{
	ساختار oidset *seen = data;

	اگر (oidset_درج(seen, oid))
		بازگشت;

	show_ref(".have", oid);
}

ایستا تهی write_head_info(تهی)
{
	ساختار refs_for_each_ref_options opts = { 0 };
	ایستا ساختار oidset seen = OIDSET_INIT;
	ساختار strvec excludes_vector = STRVEC_INIT;

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

	اگر (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#define RUN_PROC_RECEIVE_SCHEDULED	1
#define RUN_PROC_RECEIVE_RETURNED	2
ساختار command {
	ساختار command *next;
	ثابت نویسه *error_string;
	نویسه *error_string_owned;
	ساختار ref_افزودن_report *report;
	unsigned صحیح skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	صحیح index;
	ساختار object_id old_oid;
	ساختار object_id new_oid;
	نویسه ref_name[FLEX_ARRAY]; /* more */
};

ایستا تهی proc_receive_ref_append(ثابت نویسه *prefix)
{
	ساختار proc_receive_ref *ref_pattern;
	ثابت نویسه *p;
	صحیح len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = strchr(prefix, ':');
	اگر (p) {
		وقتی (prefix < p) {
			اگر (*prefix == 'a')
				ref_pattern->want_add = 1;
			وگرنه اگر (*prefix == 'd')
				ref_pattern->want_delete = 1;
			وگرنه اگر (*prefix == 'm')
				ref_pattern->want_modify = 1;
			وگرنه اگر (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} وگرنه {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	وقتی (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	اگر (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} وگرنه {
		ساختار proc_receive_ref *end;

		end = proc_receive_ref;
		وقتی (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

ایستا صحیح proc_receive_ref_matches(ساختار command *cmd)
{
	ساختار proc_receive_ref *p;

	اگر (!proc_receive_ref)
		بازگشت 0;

	برای (p = proc_receive_ref; p; p = p->next) {
		ثابت نویسه *match = p->ref_prefix;
		ثابت نویسه *remains;

		اگر (!p->want_add && is_null_oid(&cmd->old_oid))
			ادامه;
		وگرنه اگر (!p->want_delete && is_null_oid(&cmd->new_oid))
			ادامه;
		وگرنه اگر (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			ادامه;

		اگر (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			اگر (!p->negative_ref)
				بازگشت 1;
		} وگرنه اگر (p->negative_ref) {
			بازگشت 1;
		}
	}
	بازگشت 0;
}

ایستا تهی report_message(ثابت نویسه *prefix, ثابت نویسه *err, va_list params)
{
	صحیح sz;
	نویسه پیام[4096];

	sz = xsnprintf(پیام, اندازه(پیام), "%s", prefix);
	sz += vsnprintf(پیام + sz, اندازه(پیام) - sz, err, params);
	اگر (sz > (اندازه(پیام) - 1))
		sz = اندازه(پیام) - 1;
	پیام[sz++] = '\n';

	اگر (use_sideband)
		send_sideband(1, 2, پیام, sz, use_sideband);
	وگرنه
		xwrite(2, پیام, sz);
}

__attribute__((format (printf, 1, 2)))
ایستا تهی rp_warning(ثابت نویسه *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("warning: ", err, params);
	va_end(params);
}

__attribute__((format (printf, 1, 2)))
ایستا تهی rp_error(ثابت نویسه *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("error: ", err, params);
	va_end(params);
}

ایستا صحیح copy_to_sideband(صحیح in, صحیح out UNUSED, تهی *arg UNUSED)
{
	نویسه data[128];
	صحیح keepalive_active = 0;

	اگر (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	اگر (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	وقتی (1) {
		ssize_t sz;

		اگر (keepalive_active) {
			ساختار pollfd pfd;
			صحیح ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			اگر (ret < 0) {
				اگر (errno == EINTR)
					ادامه;
				وگرنه
					بشکن;
			} وگرنه اگر (ret == 0) {
				/* no data; send a keepalive packet */
				ایستا ثابت نویسه buf[] = "0005\1";
				write_or_die(1, buf, اندازه(buf) - 1);
				ادامه;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, اندازه(data));
		اگر (sz <= 0)
			بشکن;

		اگر (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			ثابت نویسه *p = memchr(data, '\0', sz);
			اگر (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_active = 1;
				send_sideband(1, 2, data, p - data, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - data + 1), use_sideband);
				ادامه;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, data, sz, use_sideband);
	}
	close(in);
	بازگشت 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
ایستا تهی prepare_sideband_async(ساختار async *sideband_async, صحیح *saved_stderr, صحیح *started)
{
	*started = 0;

	اگر (!use_sideband)
		بازگشت;

	memset(sideband_async, 0, اندازه(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	اگر (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		اگر (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
ایستا تهی finish_sideband_async(ساختار async *sideband_async, صحیح saved_stderr, صحیح started)
{
	اگر (!use_sideband)
		بازگشت;

	اگر (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	اگر (started)
		finish_async(sideband_async);
}

ایستا تهی hmac_hash(unsigned نویسه *out,
		      ثابت نویسه *کلید_in, size_t کلید_len,
		      ثابت نویسه *text, size_t text_len)
{
	unsigned نویسه کلید[GIT_MAX_BLKSZ];
	unsigned نویسه k_ipad[GIT_MAX_BLKSZ];
	unsigned نویسه k_opad[GIT_MAX_BLKSZ];
	صحیح i;
	ساختار git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(کلید, '\0', GIT_MAX_BLKSZ);
	اگر (the_hash_algo->blksz < کلید_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, کلید_in, کلید_len);
		git_hash_final(کلید, &ctx);
	} وگرنه {
		memcpy(کلید, کلید_in, کلید_len);
	}

	/* RFC 2104 2. (2) & (5) */
	برای (i = 0; i < اندازه(کلید); i++) {
		k_ipad[i] = کلید[i] ^ 0x36;
		k_opad[i] = کلید[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, اندازه(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, اندازه(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

ایستا نویسه *prepare_افزودن_cert_nonce(ثابت نویسه *path, timestamp_t stamp)
{
	ساختار strbuf buf = STRBUF_INIT;
	unsigned نویسه hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, strlen(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (صحیح)the_hash_algo->hexsz, hash_to_hex(hash));
	بازگشت strbuf_detach(&buf, پوچ);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
ایستا صحیح constant_memequal(ثابت نویسه *a, ثابت نویسه *b, size_t n)
{
	صحیح res = 0;
	size_t i;

	برای (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	بازگشت res;
}

ایستا ثابت نویسه *check_nonce(ثابت نویسه *buf)
{
	size_t noncelen;
	ثابت نویسه *found = find_commit_header(buf, "nonce", &noncelen);
	نویسه *nonce = found ? xmemdupz(found, noncelen) : پوچ;
	timestamp_t stamp, ostamp;
	نویسه *bohmac, *expect = پوچ;
	ثابت نویسه *retval = NONCE_BAD;

	اگر (!nonce) {
		retval = NONCE_MISSING;
		برو leave;
	} وگرنه اگر (!افزودن_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		برو leave;
	} وگرنه اگر (!strcmp(افزودن_cert_nonce, nonce)) {
		retval = NONCE_OK;
		برو leave;
	}

	اگر (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		برو leave;
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
	اگر (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		برو leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	اگر (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		برو leave;
	}

	expect = prepare_افزودن_cert_nonce(service_dir, stamp);
	اگر (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		برو leave;
	}
	اگر (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		برو leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(افزودن_cert_nonce, پوچ, 10);
	nonce_stamp_slop = (long)ostamp - (long)stamp;

	اگر (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		آزاد((تهی *)افزودن_cert_nonce);
		افزودن_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} وگرنه {
		retval = NONCE_SLOP;
	}

leave:
	آزاد(nonce);
	آزاد(expect);
	بازگشت retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
ایستا صحیح check_cert_افزودن_options(ثابت ساختار string_list *افزودن_options)
{
	ثابت نویسه *buf = افزودن_cert.buf;

	ثابت نویسه *option;
	size_t optionlen;
	صحیح options_seen = 0;

	صحیح retval = 1;

	اگر (!*buf)
		بازگشت 1;

	وقتی ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		اگر (options_seen > افزودن_options->nr
		    || xstrncmpz(افزودن_options->اقلام[options_seen - 1].string,
				 option, optionlen))
			بازگشت 0;
	}

	اگر (options_seen != افزودن_options->nr)
		retval = 0;

	بازگشت retval;
}

ایستا تهی prepare_افزودن_cert_sha1(ساختار run_hooks_opt *opt)
{
	ایستا صحیح already_done;

	اگر (!افزودن_cert.len)
		بازگشت;

	اگر (!already_done) {
		صحیح bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		اگر (odb_write_object(the_repository->objects, افزودن_cert.buf,
				     افزودن_cert.len, OBJ_BLOB, &افزودن_cert_oid))
			oidclr(&افزودن_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', اندازه(sigcheck));

		bogs = parse_signed_buffer(افزودن_cert.buf, افزودن_cert.len);
		sigcheck.payload = xmemdupz(افزودن_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, افزودن_cert.buf + bogs,
				افزودن_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	اگر (!is_null_oid(&افزودن_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&افزودن_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.کلید ? sigcheck.کلید : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		اگر (افزودن_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     افزودن_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			اگر (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

ساختار receive_hook_feed_state {
	ساختار command *cmd;
	ساختار ref_افزودن_report *report;
	صحیح skip_broken;
	ساختار strbuf buf;
};

ایستا صحیح feed_receive_hook_cb(صحیح hook_stdin_fd, تهی *pp_cb UNUSED, تهی *pp_task_cb)
{
	ساختار receive_hook_feed_state *state = pp_task_cb;
	ساختار command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	وقتی (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	اگر (!cmd)
		بازگشت 1;  /* no more commands left */

	اگر (!state->report)
		state->report = cmd->report;

	اگر (state->report) {
		ساختار object_id *old_oid;
		ساختار object_id *new_oid;
		ثابت نویسه *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->next;
		اگر (!state->report)
			cmd = cmd->next;
	} وگرنه {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	اگر (state->buf.len > 0) {
		صحیح ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		اگر (ret < 0) {
			اگر (errno == EPIPE)
				بازگشت 1; /* child closed pipe */
			بازگشت ret;
		}
	}

	بازگشت state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

ایستا تهی *receive_hook_feed_state_alloc(تهی *feed_pipe_ctx)
{
	ساختار receive_hook_feed_state *init_state = feed_pipe_ctx;
	ساختار receive_hook_feed_state *data;

	CALLOC_ARRAY(data, 1);
	data->report = init_state->report;
	data->cmd = init_state->cmd;
	data->skip_broken = init_state->skip_broken;
	strbuf_init(&data->buf, 0);

	بازگشت data;
}

ایستا تهی receive_hook_feed_state_free(تهی *data)
{
	ساختار receive_hook_feed_state *d = data;
	اگر (!d)
		بازگشت;
	strbuf_release(&d->buf);
	آزاد(d);
}

ایستا صحیح run_receive_hook(ساختار command *commands,
			    ثابت نویسه *hook_name,
			    صحیح skip_broken,
			    ساختار odb_transaction *transaction,
			    ثابت ساختار string_list *افزودن_options)
{
	ساختار run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ساختار command *iter = commands;
	ساختار receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	ساختار async sideband_async;
	صحیح sideband_async_started = 0;
	صحیح saved_stderr = -1;
	صحیح ret;

	اگر (!hook_exists(the_repository, hook_name))
		بازگشت 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	وقتی (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	اگر (!iter)
		بازگشت 0;

	اگر (افزودن_options) {
		برای (صحیح i = 0; i < افزودن_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     افزودن_options->اقلام[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)افزودن_options->nr);
	} وگرنه {
		strvec_افزودن(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	اگر (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_افزودن_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_data_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_data_free = receive_hook_feed_state_free;

	ret = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	بازگشت ret;
}

ایستا صحیح run_update_hook(ساختار command *cmd)
{
	ایستا ثابت نویسه hook_name[] = "update";
	ساختار run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ساختار async sideband_async;
	صحیح sideband_async_started = 0;
	صحیح saved_stderr = -1;
	صحیح code;

	اگر (!hook_exists(the_repository, hook_name))
		بازگشت 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     پوچ);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	بازگشت code;
}

ایستا صحیح run_receive_report_hook(ساختار strbuf *report)
{
	ساختار child_process proc = CHILD_PROCESS_INIT;
	ساختار async sideband_async;
	صحیح sideband_async_started = 0;
	صحیح saved_stderr = -1;
	ساختار strbuf out = STRBUF_INIT;
	ثابت نویسه *hook_path;
	صحیح ret;

	hook_path = find_hook(the_repository, "receive-report");
	اگر (!hook_path)
		بازگشت 0;

	strvec_افزودن(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_افزودن(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, پوچ, 0);
	sigchain_برداشت(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	اگر (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	بازگشت ret;
}

ایستا ساختار command *find_command_by_refname(ساختار command *list,
					       ثابت نویسه *refname)
{
	برای (; list; list = list->next)
		اگر (!strcmp(list->ref_name, refname))
			بازگشت list;
	بازگشت پوچ;
}

ایستا صحیح read_proc_receive_report(ساختار packet_reader *reader,
				    ساختار command *commands,
				    ساختار strbuf *errmsg)
{
	ساختار command *cmd;
	ساختار command *hint = پوچ;
	ساختار ref_افزودن_report *report = پوچ;
	صحیح new_report = 0;
	صحیح code = 0;
	صحیح once = 0;
	صحیح response = 0;

	برای (;;) {
		ساختار object_id old_oid, new_oid;
		نویسه *head;
		نویسه *refname;
		نویسه *p;
		شمارش packet_read_status status;

		status = packet_reader_read(reader);
		اگر (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			اگر (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				بازگشت -1;
			}
			بشکن;
		}
		response++;

		head = reader->سطر;
		p = strchr(head, ' ');
		اگر (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			ادامه;
		}
		*p++ = '\0';
		اگر (!strcmp(head, "option")) {
			نویسه *کلید;
			ثابت نویسه *val;

			اگر (!hint || !(report || new_report)) {
				اگر (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				ادامه;
			}
			اگر (new_report) {
				اگر (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} وگرنه {
					report = hint->report;
					وقتی (report->next)
						report = report->next;
					report->next = xcalloc(1, اندازه(ساختار ref_افزودن_report));
					report = report->next;
				}
				new_report = 0;
			}
			کلید = p;
			p = strchr(کلید, ' ');
			اگر (p)
				*p++ = '\0';
			val = p;
			اگر (!strcmp(کلید, "refname"))
				report->ref_name = xstrdup_or_null(val);
			وگرنه اگر (!strcmp(کلید, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			وگرنه اگر (!strcmp(کلید, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			وگرنه اگر (!strcmp(کلید, "forced-update"))
				report->forced_update = 1;
			وگرنه اگر (!strcmp(کلید, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			ادامه;
		}

		report = پوچ;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		اگر (p)
			*p++ = '\0';
		اگر (strcmp(head, "ok") && strcmp(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			ادامه;
		}

		/* first try searching at our hint, falling back to all refs */
		اگر (hint)
			hint = find_command_by_refname(hint, refname);
		اگر (!hint)
			hint = find_command_by_refname(commands, refname);
		اگر (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			ادامه;
		}
		اگر (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			ادامه;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		اگر (!strcmp(head, "ng")) {
			اگر (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			وگرنه
				hint->error_string = "failed";
			code = -1;
			ادامه;
		}
		new_report = 1;
	}

	برای (cmd = commands; cmd; cmd = cmd->next)
		اگر (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	بازگشت code;
}

ایستا صحیح run_proc_receive_hook(ساختار command *commands,
				 ثابت ساختار string_list *افزودن_options)
{
	ساختار child_process proc = CHILD_PROCESS_INIT;
	ساختار async muxer;
	ساختار command *cmd;
	ساختار packet_reader reader;
	ساختار strbuf cap = STRBUF_INIT;
	ساختار strbuf errmsg = STRBUF_INIT;
	صحیح hook_use_افزودن_options = 0;
	صحیح version = 0;
	صحیح code;
	ثابت نویسه *hook_path = find_hook(the_repository, "proc-receive");

	اگر (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		بازگشت -1;
	}

	strvec_افزودن(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	اگر (use_sideband) {
		memset(&muxer, 0, اندازه(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		اگر (code)
			بازگشت code;
		proc.err = muxer.in;
	} وگرنه {
		proc.err = 0;
	}

	code = start_command(&proc);
	اگر (code) {
		اگر (use_sideband)
			finish_async(&muxer);
		بازگشت code;
	}

	sigchain_افزودن(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, پوچ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	اگر (use_atomic)
		strbuf_addstr(&cap, " atomic");
	اگر (use_افزودن_options)
		strbuf_addstr(&cap, " push-options");
	اگر (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} وگرنه {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	اگر (!code)
		code = packet_flush_gently(proc.in);

	اگر (!code)
		برای (;;) {
			صحیح linelen;
			شمارش packet_read_status status;

			status = packet_reader_read(&reader);
			اگر (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				اگر (status == PACKET_READ_EOF)
					code = -1;
				بشکن;
			}

			اگر (reader.pktlen > 8 && starts_with(reader.سطر, "version=")) {
				version = atoi(reader.سطر + 8);
				linelen = strlen(reader.سطر);
				اگر (linelen < reader.pktlen) {
					ثابت نویسه *feature_list = reader.سطر + linelen + 1;
					اگر (parse_feature_request(feature_list, "push-options"))
						hook_use_افزودن_options = 1;
				}
			}
		}

	اگر (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		برو cleanup;
	}

	انتخاب (version) {
	حالت 0:
		/* fallthrough */
	حالت 1:
		بشکن;
	پیشفرض:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		برو cleanup;
	}

	/* Send commands */
	برای (cmd = commands; cmd; cmd = cmd->next) {
		اگر (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			ادامه;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		اگر (code)
			بشکن;
	}
	اگر (!code)
		code = packet_flush_gently(proc.in);
	اگر (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		برو cleanup;
	}

	/* Send push options */
	اگر (hook_use_افزودن_options) {
		ساختار string_list_item *item;

		for_each_string_list_item(item, افزودن_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			اگر (code)
				بشکن;
		}
		اگر (!code)
			code = packet_flush_gently(proc.in);
		اگر (code) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			برو cleanup;
		}
	}

	/* Read result from proc-receive */
	code = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.in);
	close(proc.out);
	اگر (use_sideband)
		finish_async(&muxer);
	اگر (finish_command(&proc))
		code = -1;
	اگر (errmsg.len >0) {
		نویسه *p = errmsg.buf;

		p += errmsg.len - 1;
		اگر (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_برداشت(SIGPIPE);

	بازگشت code;
}

ایستا ثابت نویسه *refuse_unconfigured_deny_پیام =
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

ایستا تهی refuse_unconfigured_deny(تهی)
{
	rp_error("%s", _(refuse_unconfigured_deny_پیام));
}

ایستا ثابت نویسه *refuse_unconfigured_deny_delete_current_پیام =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

ایستا تهی refuse_unconfigured_deny_delete_current(تهی)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_پیام));
}

ایستا ثابت ساختار object_id *command_singleton_iterator(تهی *cb_data);
ایستا صحیح update_shallow_ref(ساختار command *cmd, ساختار shallow_info *si)
{
	ساختار shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	ساختار oid_array extra = OID_ARRAY_INIT;
	ساختار check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	صحیح i;

	trace_printf_کلید(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	برای (i = 0; i < si->shallow->nr; i++)
		اگر (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	اگر (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		بازگشت -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	برای (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	بازگشت 0;
}

ایستا ثابت نویسه *افزودن_to_deploy(unsigned نویسه *sha1,
				  ساختار strvec *env,
				  ثابت ساختار worktree *worktree)
{
	ساختار child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", پوچ);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	اگر (run_command(&child))
		بازگشت "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", پوچ);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	اگر (run_command(&child))
		بازگشت "Working directory has unstaged changes";

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
		     empty_درخت_oid_hex(the_repository->hash_algo), "--", پوچ);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	اگر (run_command(&child))
		بازگشت "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     پوچ);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	اگر (run_command(&child))
		بازگشت "Could not update working tree to new HEAD";

	بازگشت پوچ;
}

ایستا ثابت نویسه *افزودن_to_checkout_hook = "push-to-checkout";

ایستا ثابت نویسه *افزودن_to_checkout(unsigned نویسه *hash,
				    صحیح *invoked_hook,
				    ساختار strvec *env,
				    ثابت نویسه *work_درخت)
{
	ساختار run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_درخت));
	strvec_افزودن(&opt.args, hash_to_hex(hash));
	اگر (run_hooks_opt(the_repository, افزودن_to_checkout_hook, &opt))
		بازگشت "push-to-checkout hook declined";
	وگرنه
		بازگشت پوچ;
}

ایستا ثابت نویسه *update_worktree(unsigned نویسه *sha1, ثابت ساختار worktree *worktree)
{
	ثابت نویسه *retval;
	نویسه *git_dir;
	ساختار strvec env = STRVEC_INIT;
	صحیح invoked_hook;

	اگر (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	اگر (worktree->is_bare)
		بازگشت "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = افزودن_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	اگر (!invoked_hook)
		retval = افزودن_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	آزاد(git_dir);
	بازگشت retval;
}

ایستا ثابت نویسه *update(ساختار command *cmd, ساختار shallow_info *si)
{
	ثابت نویسه *name = cmd->ref_name;
	ساختار strbuf namespaced_name_buf = STRBUF_INIT;
	ایستا نویسه *namespaced_name;
	ثابت نویسه *ret;
	ساختار object_id *old_oid = &cmd->old_oid;
	ساختار object_id *new_oid = &cmd->new_oid;
	صحیح do_update_worktree = 0;
	ساختار worktree **worktrees = get_worktrees(the_repository);
	ثابت ساختار worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	اگر (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		برو out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	آزاد(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, پوچ);

	اگر (worktree && !worktree->is_bare) {
		انتخاب (deny_current_branch) {
		حالت DENY_IGNORE:
			بشکن;
		حالت DENY_WARN:
			rp_warning("updating the current branch");
			بشکن;
		حالت DENY_REFUSE:
		حالت DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			اگر (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			برو out;
		حالت DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			بشکن;
		}
	}

	اگر (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		برو out;
	}

	اگر (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		اگر (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			برو out;
		}

		اگر (worktree || (head_name && !strcmp(namespaced_name, head_name))) {
			انتخاب (deny_delete_current) {
			حالت DENY_IGNORE:
				بشکن;
			حالت DENY_WARN:
				rp_warning("deleting the current branch");
				بشکن;
			حالت DENY_REFUSE:
			حالت DENY_UNCONFIGURED:
			حالت DENY_UPDATE_INSTEAD:
				اگر (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				برو out;
			پیشفرض:
				ret = "Invalid denyDeleteCurrent setting";
				برو out;
			}
		}
	}

	اگر (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		ساختار object *old_object, *new_object;
		ساختار commit *old_commit, *new_commit;
		صحیح ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		اگر (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			برو out;
		}
		old_commit = (ساختار commit *)old_object;
		new_commit = (ساختار commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		اگر (ret2 < 0)
			exit(128);
		اگر (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			برو out;
		}
	}
	اگر (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		برو out;
	}

	اگر (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		اگر (ret)
			برو out;
	}

	اگر (is_null_oid(new_oid)) {
		ساختار strbuf err = STRBUF_INIT;
		اگر (!parse_object(the_repository, old_oid)) {
			old_oid = پوچ;
			اگر (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} وگرنه {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		اگر (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   پوچ, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} وگرنه {
			ret = پوچ; /* good */
		}
		strbuf_release(&err);
	} وگرنه {
		شمارش ref_transaction_error tx_err;
		ساختار strbuf err = STRBUF_INIT;
		اگر (shallow_update && si->shallow_ref[cmd->index] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			برو out;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  پوچ, پوچ,
						  0, "push",
						  &err);
		اگر (tx_err) {
			rp_error("%s", err.buf);
			اگر (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			وگرنه
				ret = ref_transaction_error_پیام(tx_err);
		} وگرنه {
			ret = پوچ; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	بازگشت ret;
}

ایستا تهی run_update_post_hook(ساختار command *commands)
{
	ایستا ثابت نویسه hook_name[] = "post-update";
	ساختار run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ساختار async sideband_async;
	ساختار command *cmd;
	صحیح sideband_async_started = 0;
	صحیح saved_stderr = -1;

	اگر (!hook_exists(the_repository, hook_name))
		بازگشت;

	برای (cmd = commands; cmd; cmd = cmd->next) {
		اگر (cmd->error_string || cmd->did_not_exist)
			ادامه;
		strvec_افزودن(&opt.args, cmd->ref_name);
	}
	اگر (!opt.args.nr)
		بازگشت;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

ایستا تهی check_aliased_update_internal(ساختار command *cmd,
					  ساختار string_list *list,
					  ثابت نویسه *dst_name, صحیح flag)
{
	ساختار string_list_item *item;
	ساختار command *dst_cmd;

	اگر (!(flag & REF_ISSYMREF))
		بازگشت;

	اگر (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		بازگشت;
	}
	dst_name = strip_namespace(dst_name);

	اگر (!(item = string_list_lookup(list, dst_name)))
		بازگشت;

	cmd->skip_update = 1;

	dst_cmd = (ساختار command *) item->util;

	اگر (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		بازگشت;

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

ایستا تهی check_aliased_update(ساختار command *cmd, ساختار string_list *list)
{
	ساختار strbuf buf = STRBUF_INIT;
	ثابت نویسه *dst_name;
	صحیح flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, پوچ, &flag);
	check_aliased_update_internal(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

ایستا تهی check_aliased_updates(ساختار command *commands)
{
	ساختار command *cmd;
	ساختار string_list ref_list = STRING_LIST_INIT_NODUP;

	برای (cmd = commands; cmd; cmd = cmd->next) {
		ساختار string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (تهی *)cmd;
	}
	string_list_sort(&ref_list);

	برای (cmd = commands; cmd; cmd = cmd->next) {
		اگر (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

ایستا ثابت ساختار object_id *command_singleton_iterator(تهی *cb_data)
{
	ساختار command **cmd_list = cb_data;
	ساختار command *cmd = *cmd_list;

	اگر (!cmd || is_null_oid(&cmd->new_oid))
		بازگشت پوچ;
	*cmd_list = پوچ; /* this returns only one */
	بازگشت &cmd->new_oid;
}

ایستا تهی set_connectivity_errors(ساختار command *commands,
				    ساختار shallow_info *si,
				    ساختار odb_transaction *transaction)
{
	ساختار command *cmd;

	برای (cmd = commands; cmd; cmd = cmd->next) {
		ساختار command *singleton = cmd;
		ساختار check_connected_options opt = CHECK_CONNECTED_INIT;
		ساختار strvec env = STRVEC_INIT;

		اگر (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			ادامه;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		اگر (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			ادامه;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

ساختار iterate_data {
	ساختار command *cmds;
	ساختار shallow_info *si;
};

ایستا ثابت ساختار object_id *iterate_receive_command_list(تهی *cb_data)
{
	ساختار iterate_data *data = cb_data;
	ساختار command **cmd_list = &data->cmds;
	ساختار command *cmd = *cmd_list;

	برای (; cmd; cmd = cmd->next) {
		اگر (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			ادامه;
		اگر (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			بازگشت &cmd->new_oid;
		}
	}
	بازگشت پوچ;
}

ایستا تهی reject_updates_to_hidden(ساختار command *commands)
{
	ساختار strbuf refname_full = STRBUF_INIT;
	size_t prefix_len;
	ساختار command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	برای (cmd = commands; cmd; cmd = cmd->next) {
		اگر (cmd->error_string)
			ادامه;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		اگر (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			ادامه;
		اگر (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		وگرنه
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

ایستا صحیح should_process_cmd(ساختار command *cmd)
{
	بازگشت !cmd->error_string && !cmd->skip_update;
}

ایستا تهی BUG_if_skipped_connectivity_check(ساختار command *commands,
					       ساختار shallow_info *si)
{
	ساختار command *cmd;

	برای (cmd = commands; cmd; cmd = cmd->next) {
		اگر (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

ایستا تهی ref_transaction_rejection_handler(ثابت نویسه *refname,
					      ثابت ساختار object_id *old_oid UNUSED,
					      ثابت ساختار object_id *new_oid UNUSED,
					      ثابت نویسه *old_target UNUSED,
					      ثابت نویسه *new_target UNUSED,
					      شمارش ref_transaction_error err,
					      ثابت نویسه *details,
					      تهی *cb_data)
{
	ساختار strmap *failed_refs = cb_data;

	اگر (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (نویسه *)ref_transaction_error_پیام(err));
}

ایستا تهی execute_commands_non_atomic(ساختار command *commands,
					ساختار shallow_info *si)
{
	ساختار command *cmd;
	ساختار strbuf err = STRBUF_INIT;
	ثابت نویسه *reported_error = پوچ;
	ساختار strmap failed_refs = STRMAP_INIT;

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
	شمارش processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	برای (شمارش processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		برای (cmd = commands; cmd; cmd = cmd->next) {
			اگر (!should_process_cmd(cmd) || cmd->run_proc_receive)
				ادامه;

			اگر (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				ادامه;
			وگرنه اگر (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				ادامه;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			اگر (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				اگر (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					برو failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		اگر (!transaction)
			برو cleanup;

		اگر (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			برو failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		اگر (strmap_empty(&failed_refs))
			برو cleanup;

	failure:
		برای (cmd = commands; cmd; cmd = cmd->next) {
			اگر (reported_error)
				cmd->error_string = reported_error;
			وگرنه اگر (strmap_contains(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = پوچ;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

ایستا تهی execute_commands_atomic(ساختار command *commands,
					ساختار shallow_info *si)
{
	ساختار command *cmd;
	ساختار strbuf err = STRBUF_INIT;
	ثابت نویسه *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	اگر (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		برو failure;
	}

	برای (cmd = commands; cmd; cmd = cmd->next) {
		اگر (!should_process_cmd(cmd) || cmd->run_proc_receive)
			ادامه;

		cmd->error_string = update(cmd, si);

		اگر (cmd->error_string)
			برو failure;
	}

	اگر (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		برو failure;
	}
	برو cleanup;

failure:
	برای (cmd = commands; cmd; cmd = cmd->next)
		اگر (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

ایستا تهی execute_commands(ساختار command *commands,
			     صحیح unpacker_error,
			     ساختار shallow_info *si,
			     ساختار odb_transaction *transaction,
			     ثابت ساختار string_list *افزودن_options)
{
	ساختار check_connected_options opt = CHECK_CONNECTED_INIT;
	ساختار command *cmd;
	ساختار iterate_data data;
	ساختار async muxer;
	صحیح err_fd = 0;
	صحیح run_proc_receive = 0;

	اگر (unpacker_error) {
		برای (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		بازگشت;
	}

	اگر (!skip_connectivity_check) {
		ساختار strvec env = STRVEC_INIT;

		اگر (use_sideband) {
			memset(&muxer, 0, اندازه(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			اگر (!start_async(&muxer))
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

		اگر (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		اگر (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	اگر (proc_receive_ref) {
		برای (cmd = commands; cmd; cmd = cmd->next) {
			اگر (!should_process_cmd(cmd))
				ادامه;

			اگر (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	اگر (run_receive_hook(commands, "pre-receive", 0, transaction, افزودن_options)) {
		برای (cmd = commands; cmd; cmd = cmd->next) {
			اگر (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		بازگشت;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	برای (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	اگر (!cmd)
		بازگشت;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	اگر (odb_transaction_commit(transaction)) {
		برای (cmd = commands; cmd; cmd = cmd->next) {
			اگر (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		بازگشت;
	}

	check_aliased_updates(commands);

	آزاد(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, پوچ,
							    پوچ);

	اگر (run_proc_receive &&
	    run_proc_receive_hook(commands, افزودن_options))
		برای (cmd = commands; cmd; cmd = cmd->next)
			اگر (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	اگر (use_atomic)
		execute_commands_atomic(commands, si);
	وگرنه
		execute_commands_non_atomic(commands, si);

	اگر (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

ایستا ساختار command **queue_command(ساختار command **tail,
				      ثابت نویسه *سطر,
				      صحیح linelen)
{
	ساختار object_id old_oid, new_oid;
	ساختار command *cmd;
	ثابت نویسه *refname;
	صحیح reflen;
	ثابت نویسه *p;

	اگر (parse_oid_hex(سطر, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", سطر);

	refname = p;
	reflen = linelen - (p - سطر);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*tail = cmd;
	بازگشت &cmd->next;
}

ایستا تهی free_commands(ساختار command *commands)
{
	وقتی (commands) {
		ساختار command *next = commands->next;

		ref_افزودن_report_free(commands->report);
		آزاد(commands->error_string_owned);
		آزاد(commands);
		commands = next;
	}
}

ایستا تهی queue_commands_from_cert(ساختار command **tail,
				     ساختار strbuf *افزودن_cert)
{
	ثابت نویسه *boc, *eoc;

	اگر (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(افزودن_cert->buf, "\n\n");
	اگر (!boc)
		die("malformed push certificate %.*s", 100, افزودن_cert->buf);
	وگرنه
		boc += 2;
	eoc = افزودن_cert->buf + parse_signed_buffer(افزودن_cert->buf, افزودن_cert->len);

	وقتی (boc < eoc) {
		ثابت نویسه *eol = memchr(boc, '\n', eoc - boc);
		tail = queue_command(tail, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

ایستا ساختار command *read_head_info(ساختار packet_reader *reader,
				      ساختار oid_array *shallow,
				      شمارش report_status_version *version)
{
	ساختار command *commands = پوچ;
	ساختار command **p = &commands;
	برای (;;) {
		صحیح linelen;

		اگر (packet_reader_read(reader) != PACKET_READ_NORMAL)
			بشکن;

		اگر (reader->pktlen > 8 && starts_with(reader->سطر, "shallow ")) {
			ساختار object_id oid;
			اگر (get_oid_hex(reader->سطر + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->سطر + 8);
			oid_array_append(shallow, &oid);
			ادامه;
		}

		linelen = strlen(reader->سطر);
		اگر (linelen < reader->pktlen) {
			ثابت نویسه *feature_list = reader->سطر + linelen + 1;
			ثابت نویسه *hash = پوچ;
			ثابت نویسه *client_sid;
			size_t len = 0;
			اگر (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			اگر (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			اگر (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			اگر (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			اگر (advertise_atomic_افزودن
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			اگر (advertise_افزودن_options
			    && parse_feature_request(feature_list, "push-options"))
				use_افزودن_options = 1;
			hash = parse_feature_مقدار(feature_list, "object-format", &len, پوچ);
			اگر (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			اگر (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_مقدار(feature_list, "session-id", &len, پوچ);
			اگر (client_sid) {
				نویسه *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", پوچ, "client-sid", client_sid);
				آزاد(sid);
			}
		}

		اگر (!strcmp(reader->سطر, "push-cert")) {
			صحیح true_flush = 0;
			صحیح saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			برای (;;) {
				packet_reader_read(reader);
				اگر (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					بشکن;
				}
				اگر (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				اگر (!strcmp(reader->سطر, "push-cert-end\n"))
					بشکن; /* end of cert */
				strbuf_addstr(&افزودن_cert, reader->سطر);
			}
			reader->options = saved_options;

			اگر (true_flush)
				بشکن;
			ادامه;
		}

		p = queue_command(p, reader->سطر, linelen);
	}

	اگر (افزودن_cert.len)
		queue_commands_from_cert(p, &افزودن_cert);

	بازگشت commands;
}

ایستا تهی read_افزودن_options(ساختار packet_reader *reader,
			      ساختار string_list *options)
{
	وقتی (1) {
		اگر (packet_reader_read(reader) != PACKET_READ_NORMAL)
			بشکن;

		string_list_append(options, reader->سطر);
	}
}

ایستا صحیح unpack_with_sideband(ساختار odb_transaction *transaction,
				ثابت نویسه *shallow_file,
				ساختار strbuf *err_پیام)
{
	ساختار odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_پیام_types = fsck_پیام_types.buf,
		.max_درون_داد_size = max_درون_داد_size,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	ساختار async muxer;
	صحیح ret;

	اگر (!use_sideband)
		بازگشت odb_transaction_write_pack(transaction, 0, err_پیام, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, اندازه(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	اگر (start_async(&muxer))
		بازگشت 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_پیام, &opts);

	finish_async(&muxer);
	بازگشت ret;
}

ایستا تهی prepare_shallow_update(ساختار shallow_info *si,
				   ثابت نویسه *shallow_file)
{
	صحیح i, j, k, bitmap_size = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, پوچ);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	برای (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	برای (i = 0; i < si->shallow->nr; i++) {
		اگر (!si->used_shallow[i])
			ادامه;
		برای (j = 0; j < bitmap_size; j++) {
			اگر (!si->used_shallow[i][j])
				ادامه;
			si->need_reachability_test[i]++;
			برای (k = 0; k < 32; k++)
				اگر (si->used_shallow[i][j] & (1U << k))
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

ایستا تهی update_shallow_info(ساختار command *commands,
				ساختار shallow_info *si,
				ساختار oid_array *ref,
				ثابت نویسه *shallow_file)
{
	ساختار command *cmd;
	صحیح *ref_status;
	remove_nonexistent_theirs_shallow(si);
	اگر (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		بازگشت;
	}

	برای (cmd = commands; cmd; cmd = cmd->next) {
		اگر (is_null_oid(&cmd->new_oid))
			ادامه;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	اگر (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		بازگشت;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, پوچ, ref_status);
	برای (cmd = commands; cmd; cmd = cmd->next) {
		اگر (is_null_oid(&cmd->new_oid))
			ادامه;
		اگر (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	آزاد(ref_status);
}

ایستا تهی override_cmds_error(ساختار command *commands, ثابت نویسه *err)
{
	برای (ساختار command *cmd = commands; cmd; cmd = cmd->next) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
ایستا تهی generate_report(ساختار strbuf *buf, ساختار command *commands,
			    ثابت ساختار strbuf *unpack_status,
			    شمارش report_status_version version)
{
	ساختار command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	برای (cmd = commands; cmd; cmd = cmd->next) {
		ساختار ref_افزودن_report *report;
		صحیح شمار = 0;

		اگر (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		وگرنه
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		اگر (version != REPORT_STATUS_V2 || cmd->error_string)
			ادامه;

		برای (report = cmd->report; report; report = report->next) {
			اگر (شمار++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			اگر (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			اگر (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			اگر (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			اگر (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

ایستا تهی report(ساختار command *commands, ثابت ساختار strbuf *unpack_status,
		   شمارش report_status_version version)
{
	ساختار strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	اگر (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	اگر (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	وگرنه
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

ایستا صحیح delete_only(ساختار command *commands)
{
	ساختار command *cmd;
	برای (cmd = commands; cmd; cmd = cmd->next) {
		اگر (!is_null_oid(&cmd->new_oid))
			بازگشت 0;
	}
	بازگشت 1;
}

صحیح cmd_receive_pack(صحیح argc,
		     ثابت نویسه **argv,
		     ثابت نویسه *prefix,
		     ساختار repository *repo UNUSED)
{
	صحیح advertise_refs = 0;
	ساختار command *commands;
	ساختار oid_array shallow = OID_ARRAY_INIT;
	ساختار oid_array ref = OID_ARRAY_INIT;
	ساختار shallow_info si;
	ساختار packet_reader reader;
	ساختار odb_transaction *transaction = پوچ;
	شمارش report_status_version version = REPORT_STATUS_UNKNOWN;

	ساختار option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, پوچ),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, پوچ),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, پوچ),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, پوچ),
		OPT_END()
	};

	packet_trace_identity("receive-pack");

	argc = parse_options(argc, argv, prefix, options, receive_pack_usage, 0);

	اگر (argc > 1)
		usage_پیام_opt(_("too many arguments"), receive_pack_usage, options);
	اگر (argc == 0)
		usage_پیام_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	اگر (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, پوچ);
	اگر (cert_nonce_seed)
		افزودن_cert_nonce = prepare_افزودن_cert_nonce(service_dir, time(پوچ));

	انتخاب (determine_protocol_version_server()) {
	حالت protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		بشکن;
	حالت protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		اگر (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	حالت protocol_v0:
		بشکن;
	حالت protocol_unknown_version:
		BUG("unknown protocol version");
	}

	اگر (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	اگر (advertise_refs)
		بازگشت 0;

	packet_reader_init(&reader, 0, پوچ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	اگر ((commands = read_head_info(&reader, &shallow, &version))) {
		ساختار string_list افزودن_options = STRING_LIST_INIT_DUP;
		ساختار strbuf unpack_status = STRBUF_INIT;

		اگر (use_افزودن_options)
			read_افزودن_options(&reader, &افزودن_options);
		اگر (!check_cert_افزودن_options(&افزودن_options)) {
			ساختار command *cmd;
			برای (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		اگر (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		اگر (!delete_only(commands)) {
			ثابت نویسه *alt_shallow_file = پوچ;

			اگر (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			اگر (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			وگرنه
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &افزودن_options);
		odb_transaction_finalize(transaction);
		sigchain_افزودن(SIGPIPE, SIG_IGN);

		انتخاب (version) {
		حالت REPORT_STATUS_V2:
		حالت REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			بشکن;
		حالت REPORT_STATUS_UNKNOWN:
			بشکن;
		}

		sigchain_برداشت(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, پوچ,
				 &افزودن_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&افزودن_options, 0);
		اگر (auto_gc) {
			ساختار child_process proc = CHILD_PROCESS_INIT;

			اگر (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				اگر (!start_command(&proc)) {
					اگر (use_sideband)
						copy_to_sideband(proc.err, -1, پوچ);
					finish_command(&proc);
				}
			}
		}
		اگر (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	اگر (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	آزاد((تهی *)افزودن_cert_nonce);
	بازگشت 0;
}
