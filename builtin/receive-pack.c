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
ساكن بنية strbuf fsck_ڤسنن_types = STRBUF_INIT;
ساكن صحيح advertise_atomic_تولق = 1;
ساكن صحيح advertise_تولق_options;
ساكن صحيح advertise_sid;
ساكن off_t max_ماسوقن_size;
ساكن صحيح use_sideband;
ساكن صحيح use_atomic;
ساكن صحيح use_تولق_options;
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
ساكن بنية strbuf تولق_cert = STRBUF_INIT;
ساكن بنية object_id تولق_cert_oid;
ساكن بنية signature_check sigcheck;
ساكن ثابت حرف *تولق_cert_nonce;
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

ساكن تعداد deny_action parse_deny_action(ثابت حرف *var, ثابت حرف *نيلاي)
{
	إذا (نيلاي) {
		إذا (!strcasecmp(نيلاي, "ignore"))
			أرجع DENY_IGNORE;
		إذا (!strcasecmp(نيلاي, "warn"))
			أرجع DENY_WARN;
		إذا (!strcasecmp(نيلاي, "refuse"))
			أرجع DENY_REFUSE;
		إذا (!strcasecmp(نيلاي, "updateinstead"))
			أرجع DENY_UPDATE_INSTEAD;
	}
	إذا (git_config_bool(var, نيلاي))
		أرجع DENY_REFUSE;
	أرجع DENY_IGNORE;
}

ساكن صحيح receive_pack_config(ثابت حرف *var, ثابت حرف *نيلاي,
			       ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت حرف *ڤسنن_id;
	صحيح status = parse_hide_refs_config(var, نيلاي, "receive", &hidden_refs);

	إذا (status)
		أرجع status;

	إذا (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, نيلاي);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, نيلاي);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.fsck.skiplist") == 0) {
		حرف *path;

		إذا (git_config_pathname(&path, var, نيلاي))
			أرجع -1;
		إذا (path)
			strbuf_addf(&fsck_ڤسنن_types, "%cskiplist=%s",
				    fsck_ڤسنن_types.len ? ',' : '=', path);
		حرر(path);
		أرجع 0;
	}

	إذا (skip_prefix(var, "receive.fsck.", &ڤسنن_id)) {
		إذا (!نيلاي)
			أرجع config_error_nonbool(var);
		إذا (is_valid_ڤسنن_type(ڤسنن_id, نيلاي))
			strbuf_addf(&fsck_ڤسنن_types, "%c%s=%s",
				fsck_ڤسنن_types.len ? ',' : '=', ڤسنن_id, نيلاي);
		وإلا
			warning("skipping unknown msg id '%s'", ڤسنن_id);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, نيلاي);
		أرجع 0;
	}

	إذا (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, نيلاي);
		أرجع 0;
	}

	إذا (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, نيلاي);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, نيلاي);
		أرجع 0;
	}

	إذا (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, نيلاي);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, نيلاي);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, نيلاي);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, نيلاي);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.certnonceseed") == 0)
		أرجع git_config_string(&cert_nonce_seed, var, نيلاي);

	إذا (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, نيلاي, ctx->kvi);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_تولق = git_config_bool(var, نيلاي);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_تولق_options = git_config_bool(var, نيلاي);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, نيلاي, ctx->kvi);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.maxinputsize") == 0) {
		max_ماسوقن_size = git_config_int64(var, نيلاي, ctx->kvi);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.procreceiverefs") == 0) {
		إذا (!نيلاي)
			أرجع config_error_nonbool(var);
		proc_receive_ref_append(نيلاي);
		أرجع 0;
	}

	إذا (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, نيلاي);
		أرجع 0;
	}

	أرجع git_default_config(var, نيلاي, ctx, cb);
}

ساكن فراغ show_ref(ثابت حرف *path, ثابت بنية object_id *oid)
{
	إذا (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} وإلا {
		بنية strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		إذا (advertise_atomic_تولق)
			strbuf_addstr(&cap, " atomic");
		إذا (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		إذا (تولق_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", تولق_cert_nonce);
		إذا (advertise_تولق_options)
			strbuf_addstr(&cap, " push-options");
		إذا (advertise_sid)
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

	إذا (ref_is_hidden(path, ref->name, &hidden_refs))
		أرجع 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	إذا (!path) {
		إذا (oidset_سيسيڤ(seen, ref->oid))
			أرجع 0;
		path = ".have";
	} وإلا {
		oidset_سيسيڤ(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	أرجع 0;
}

ساكن فراغ show_one_alternate_ref(ثابت بنية object_id *oid,
				   فراغ *data)
{
	بنية oidset *seen = data;

	إذا (oidset_سيسيڤ(seen, oid))
		أرجع;

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

	إذا (!sent_capabilities)
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
	بنية ref_تولق_report *report;
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
	إذا (p) {
		طالما (prefix < p) {
			إذا (*prefix == 'a')
				ref_pattern->want_add = 1;
			وإلا إذا (*prefix == 'd')
				ref_pattern->want_delete = 1;
			وإلا إذا (*prefix == 'm')
				ref_pattern->want_modify = 1;
			وإلا إذا (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} وإلا {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	طالما (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	إذا (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} وإلا {
		بنية proc_receive_ref *end;

		end = proc_receive_ref;
		طالما (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

ساكن صحيح proc_receive_ref_matches(بنية command *cmd)
{
	بنية proc_receive_ref *p;

	إذا (!proc_receive_ref)
		أرجع 0;

	لكل (p = proc_receive_ref; p; p = p->next) {
		ثابت حرف *match = p->ref_prefix;
		ثابت حرف *remains;

		إذا (!p->want_add && is_null_oid(&cmd->old_oid))
			استمر;
		وإلا إذا (!p->want_delete && is_null_oid(&cmd->new_oid))
			استمر;
		وإلا إذا (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			استمر;

		إذا (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			إذا (!p->negative_ref)
				أرجع 1;
		} وإلا إذا (p->negative_ref) {
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن فراغ report_message(ثابت حرف *prefix, ثابت حرف *err, va_list params)
{
	صحيح sz;
	حرف ڤسنن[4096];

	sz = xsnprintf(ڤسنن, حجم(ڤسنن), "%s", prefix);
	sz += vsnprintf(ڤسنن + sz, حجم(ڤسنن) - sz, err, params);
	إذا (sz > (حجم(ڤسنن) - 1))
		sz = حجم(ڤسنن) - 1;
	ڤسنن[sz++] = '\n';

	إذا (use_sideband)
		send_sideband(1, 2, ڤسنن, sz, use_sideband);
	وإلا
		xwrite(2, ڤسنن, sz);
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

	إذا (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	إذا (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	طالما (1) {
		ssize_t sz;

		إذا (keepalive_active) {
			بنية pollfd pfd;
			صحيح ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			إذا (ret < 0) {
				إذا (errno == EINTR)
					استمر;
				وإلا
					اكسر;
			} وإلا إذا (ret == 0) {
				/* no data; send a keepalive packet */
				ساكن ثابت حرف buf[] = "0005\1";
				write_or_die(1, buf, حجم(buf) - 1);
				استمر;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, حجم(data));
		إذا (sz <= 0)
			اكسر;

		إذا (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			ثابت حرف *p = memchr(data, '\0', sz);
			إذا (p) {
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
	أرجع 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
ساكن فراغ prepare_sideband_async(بنية async *sideband_async, صحيح *saved_stderr, صحيح *started)
{
	*started = 0;

	إذا (!use_sideband)
		أرجع;

	memset(sideband_async, 0, حجم(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	إذا (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		إذا (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
ساكن فراغ finish_sideband_async(بنية async *sideband_async, صحيح saved_stderr, صحيح started)
{
	إذا (!use_sideband)
		أرجع;

	إذا (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	إذا (started)
		finish_async(sideband_async);
}

ساكن فراغ hmac_hash(unsigned حرف *out,
		      ثابت حرف *کونچي_in, size_t کونچي_len,
		      ثابت حرف *text, size_t text_len)
{
	unsigned حرف کونچي[GIT_MAX_BLKSZ];
	unsigned حرف k_ipad[GIT_MAX_BLKSZ];
	unsigned حرف k_opad[GIT_MAX_BLKSZ];
	صحيح i;
	بنية git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(کونچي, '\0', GIT_MAX_BLKSZ);
	إذا (the_hash_algo->blksz < کونچي_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, کونچي_in, کونچي_len);
		git_hash_final(کونچي, &ctx);
	} وإلا {
		memcpy(کونچي, کونچي_in, کونچي_len);
	}

	/* RFC 2104 2. (2) & (5) */
	لكل (i = 0; i < حجم(کونچي); i++) {
		k_ipad[i] = کونچي[i] ^ 0x36;
		k_opad[i] = کونچي[i] ^ 0x5c;
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

ساكن حرف *prepare_تولق_cert_nonce(ثابت حرف *path, timestamp_t stamp)
{
	بنية strbuf buf = STRBUF_INIT;
	unsigned حرف hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, strlen(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (صحيح)the_hash_algo->hexsz, hash_to_hex(hash));
	أرجع strbuf_detach(&buf, عدم);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
ساكن صحيح constant_memequal(ثابت حرف *a, ثابت حرف *b, size_t n)
{
	صحيح res = 0;
	size_t i;

	لكل (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	أرجع res;
}

ساكن ثابت حرف *check_nonce(ثابت حرف *buf)
{
	size_t noncelen;
	ثابت حرف *found = find_commit_header(buf, "nonce", &noncelen);
	حرف *nonce = found ? xmemdupz(found, noncelen) : عدم;
	timestamp_t stamp, ostamp;
	حرف *bohmac, *expect = عدم;
	ثابت حرف *retval = NONCE_BAD;

	إذا (!nonce) {
		retval = NONCE_MISSING;
		اذهب leave;
	} وإلا إذا (!تولق_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		اذهب leave;
	} وإلا إذا (!strcmp(تولق_cert_nonce, nonce)) {
		retval = NONCE_OK;
		اذهب leave;
	}

	إذا (!stateless_rpc) {
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
	إذا (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		اذهب leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	إذا (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		اذهب leave;
	}

	expect = prepare_تولق_cert_nonce(service_dir, stamp);
	إذا (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		اذهب leave;
	}
	إذا (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		اذهب leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(تولق_cert_nonce, عدم, 10);
	nonce_stamp_slop = (long)ostamp - (long)stamp;

	إذا (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		حرر((فراغ *)تولق_cert_nonce);
		تولق_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} وإلا {
		retval = NONCE_SLOP;
	}

leave:
	حرر(nonce);
	حرر(expect);
	أرجع retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
ساكن صحيح check_cert_تولق_options(ثابت بنية string_list *تولق_options)
{
	ثابت حرف *buf = تولق_cert.buf;

	ثابت حرف *option;
	size_t optionlen;
	صحيح options_seen = 0;

	صحيح retval = 1;

	إذا (!*buf)
		أرجع 1;

	طالما ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		إذا (options_seen > تولق_options->nr
		    || xstrncmpz(تولق_options->بارڠن[options_seen - 1].string,
				 option, optionlen))
			أرجع 0;
	}

	إذا (options_seen != تولق_options->nr)
		retval = 0;

	أرجع retval;
}

ساكن فراغ prepare_تولق_cert_sha1(بنية run_hooks_opt *opt)
{
	ساكن صحيح already_done;

	إذا (!تولق_cert.len)
		أرجع;

	إذا (!already_done) {
		صحيح bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		إذا (odb_write_object(the_repository->objects, تولق_cert.buf,
				     تولق_cert.len, OBJ_BLOB, &تولق_cert_oid))
			oidclr(&تولق_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', حجم(sigcheck));

		bogs = parse_signed_buffer(تولق_cert.buf, تولق_cert.len);
		sigcheck.payload = xmemdupz(تولق_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, تولق_cert.buf + bogs,
				تولق_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	إذا (!is_null_oid(&تولق_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&تولق_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.کونچي ? sigcheck.کونچي : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		إذا (تولق_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     تولق_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			إذا (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

بنية receive_hook_feed_state {
	بنية command *cmd;
	بنية ref_تولق_report *report;
	صحيح skip_broken;
	بنية strbuf buf;
};

ساكن صحيح feed_receive_hook_cb(صحيح hook_stdin_fd, فراغ *pp_cb UNUSED, فراغ *pp_task_cb)
{
	بنية receive_hook_feed_state *state = pp_task_cb;
	بنية command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	طالما (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	إذا (!cmd)
		أرجع 1;  /* no more commands left */

	إذا (!state->report)
		state->report = cmd->report;

	إذا (state->report) {
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
		إذا (!state->report)
			cmd = cmd->next;
	} وإلا {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	إذا (state->buf.len > 0) {
		صحيح ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		إذا (ret < 0) {
			إذا (errno == EPIPE)
				أرجع 1; /* child closed pipe */
			أرجع ret;
		}
	}

	أرجع state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
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

	أرجع data;
}

ساكن فراغ receive_hook_feed_state_free(فراغ *data)
{
	بنية receive_hook_feed_state *d = data;
	إذا (!d)
		أرجع;
	strbuf_release(&d->buf);
	حرر(d);
}

ساكن صحيح run_receive_hook(بنية command *commands,
			    ثابت حرف *hook_name,
			    صحيح skip_broken,
			    بنية odb_transaction *transaction,
			    ثابت بنية string_list *تولق_options)
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

	إذا (!hook_exists(the_repository, hook_name))
		أرجع 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	طالما (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	إذا (!iter)
		أرجع 0;

	إذا (تولق_options) {
		لكل (صحيح i = 0; i < تولق_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     تولق_options->بارڠن[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)تولق_options->nr);
	} وإلا {
		strvec_تولق(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	إذا (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_تولق_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_data_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_data_free = receive_hook_feed_state_free;

	ret = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	أرجع ret;
}

ساكن صحيح run_update_hook(بنية command *cmd)
{
	ساكن ثابت حرف hook_name[] = "update";
	بنية run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	بنية async sideband_async;
	صحيح sideband_async_started = 0;
	صحيح saved_stderr = -1;
	صحيح code;

	إذا (!hook_exists(the_repository, hook_name))
		أرجع 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     عدم);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	أرجع code;
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
	إذا (!hook_path)
		أرجع 0;

	strvec_تولق(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_تولق(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, عدم, 0);
	sigchain_کلوار(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	إذا (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	أرجع ret;
}

ساكن بنية command *find_command_by_refname(بنية command *list,
					       ثابت حرف *refname)
{
	لكل (; list; list = list->next)
		إذا (!strcmp(list->ref_name, refname))
			أرجع list;
	أرجع عدم;
}

ساكن صحيح read_proc_receive_report(بنية packet_reader *reader,
				    بنية command *commands,
				    بنية strbuf *errmsg)
{
	بنية command *cmd;
	بنية command *hint = عدم;
	بنية ref_تولق_report *report = عدم;
	صحيح new_report = 0;
	صحيح code = 0;
	صحيح once = 0;
	صحيح response = 0;

	لكل (;;) {
		بنية object_id old_oid, new_oid;
		حرف *head;
		حرف *refname;
		حرف *p;
		تعداد packet_read_status status;

		status = packet_reader_read(reader);
		إذا (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			إذا (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				أرجع -1;
			}
			اكسر;
		}
		response++;

		head = reader->باريس;
		p = strchr(head, ' ');
		إذا (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			استمر;
		}
		*p++ = '\0';
		إذا (!strcmp(head, "option")) {
			حرف *کونچي;
			ثابت حرف *val;

			إذا (!hint || !(report || new_report)) {
				إذا (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				استمر;
			}
			إذا (new_report) {
				إذا (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} وإلا {
					report = hint->report;
					طالما (report->next)
						report = report->next;
					report->next = xcalloc(1, حجم(بنية ref_تولق_report));
					report = report->next;
				}
				new_report = 0;
			}
			کونچي = p;
			p = strchr(کونچي, ' ');
			إذا (p)
				*p++ = '\0';
			val = p;
			إذا (!strcmp(کونچي, "refname"))
				report->ref_name = xstrdup_or_null(val);
			وإلا إذا (!strcmp(کونچي, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			وإلا إذا (!strcmp(کونچي, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			وإلا إذا (!strcmp(کونچي, "forced-update"))
				report->forced_update = 1;
			وإلا إذا (!strcmp(کونچي, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			استمر;
		}

		report = عدم;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		إذا (p)
			*p++ = '\0';
		إذا (strcmp(head, "ok") && strcmp(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			استمر;
		}

		/* first try searching at our hint, falling back to all refs */
		إذا (hint)
			hint = find_command_by_refname(hint, refname);
		إذا (!hint)
			hint = find_command_by_refname(commands, refname);
		إذا (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			استمر;
		}
		إذا (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			استمر;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		إذا (!strcmp(head, "ng")) {
			إذا (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			وإلا
				hint->error_string = "failed";
			code = -1;
			استمر;
		}
		new_report = 1;
	}

	لكل (cmd = commands; cmd; cmd = cmd->next)
		إذا (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	أرجع code;
}

ساكن صحيح run_proc_receive_hook(بنية command *commands,
				 ثابت بنية string_list *تولق_options)
{
	بنية child_process proc = CHILD_PROCESS_INIT;
	بنية async muxer;
	بنية command *cmd;
	بنية packet_reader reader;
	بنية strbuf cap = STRBUF_INIT;
	بنية strbuf errmsg = STRBUF_INIT;
	صحيح hook_use_تولق_options = 0;
	صحيح version = 0;
	صحيح code;
	ثابت حرف *hook_path = find_hook(the_repository, "proc-receive");

	إذا (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		أرجع -1;
	}

	strvec_تولق(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	إذا (use_sideband) {
		memset(&muxer, 0, حجم(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		إذا (code)
			أرجع code;
		proc.err = muxer.in;
	} وإلا {
		proc.err = 0;
	}

	code = start_command(&proc);
	إذا (code) {
		إذا (use_sideband)
			finish_async(&muxer);
		أرجع code;
	}

	sigchain_تولق(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	إذا (use_atomic)
		strbuf_addstr(&cap, " atomic");
	إذا (use_تولق_options)
		strbuf_addstr(&cap, " push-options");
	إذا (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} وإلا {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	إذا (!code)
		code = packet_flush_gently(proc.in);

	إذا (!code)
		لكل (;;) {
			صحيح linelen;
			تعداد packet_read_status status;

			status = packet_reader_read(&reader);
			إذا (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				إذا (status == PACKET_READ_EOF)
					code = -1;
				اكسر;
			}

			إذا (reader.pktlen > 8 && starts_with(reader.باريس, "version=")) {
				version = atoi(reader.باريس + 8);
				linelen = strlen(reader.باريس);
				إذا (linelen < reader.pktlen) {
					ثابت حرف *feature_list = reader.باريس + linelen + 1;
					إذا (parse_feature_request(feature_list, "push-options"))
						hook_use_تولق_options = 1;
				}
			}
		}

	إذا (code) {
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
	لكل (cmd = commands; cmd; cmd = cmd->next) {
		إذا (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			استمر;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		إذا (code)
			اكسر;
	}
	إذا (!code)
		code = packet_flush_gently(proc.in);
	إذا (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		اذهب cleanup;
	}

	/* Send push options */
	إذا (hook_use_تولق_options) {
		بنية string_list_item *item;

		for_each_string_list_item(item, تولق_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			إذا (code)
				اكسر;
		}
		إذا (!code)
			code = packet_flush_gently(proc.in);
		إذا (code) {
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
	إذا (use_sideband)
		finish_async(&muxer);
	إذا (finish_command(&proc))
		code = -1;
	إذا (errmsg.len >0) {
		حرف *p = errmsg.buf;

		p += errmsg.len - 1;
		إذا (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_کلوار(SIGPIPE);

	أرجع code;
}

ساكن ثابت حرف *refuse_unconfigured_deny_ڤسنن =
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
	rp_error("%s", _(refuse_unconfigured_deny_ڤسنن));
}

ساكن ثابت حرف *refuse_unconfigured_deny_delete_current_ڤسنن =
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
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_ڤسنن));
}

ساكن ثابت بنية object_id *command_singleton_iterator(فراغ *cb_data);
ساكن صحيح update_shallow_ref(بنية command *cmd, بنية shallow_info *si)
{
	بنية shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	بنية oid_array extra = OID_ARRAY_INIT;
	بنية check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	صحيح i;

	trace_printf_کونچي(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	لكل (i = 0; i < si->shallow->nr; i++)
		إذا (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	إذا (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		أرجع -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	لكل (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	أرجع 0;
}

ساكن ثابت حرف *تولق_to_deploy(unsigned حرف *sha1,
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
	إذا (run_command(&child))
		أرجع "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", عدم);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	إذا (run_command(&child))
		أرجع "Working directory has unstaged changes";

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
		     empty_ڤوکوق_oid_hex(the_repository->hash_algo), "--", عدم);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	إذا (run_command(&child))
		أرجع "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     عدم);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	إذا (run_command(&child))
		أرجع "Could not update working tree to new HEAD";

	أرجع عدم;
}

ساكن ثابت حرف *تولق_to_checkout_hook = "push-to-checkout";

ساكن ثابت حرف *تولق_to_checkout(unsigned حرف *hash,
				    صحيح *invoked_hook,
				    بنية strvec *env,
				    ثابت حرف *work_ڤوکوق)
{
	بنية run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_ڤوکوق));
	strvec_تولق(&opt.args, hash_to_hex(hash));
	إذا (run_hooks_opt(the_repository, تولق_to_checkout_hook, &opt))
		أرجع "push-to-checkout hook declined";
	وإلا
		أرجع عدم;
}

ساكن ثابت حرف *update_worktree(unsigned حرف *sha1, ثابت بنية worktree *worktree)
{
	ثابت حرف *retval;
	حرف *git_dir;
	بنية strvec env = STRVEC_INIT;
	صحيح invoked_hook;

	إذا (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	إذا (worktree->is_bare)
		أرجع "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = تولق_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	إذا (!invoked_hook)
		retval = تولق_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	حرر(git_dir);
	أرجع retval;
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
	إذا (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		اذهب out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	حرر(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, عدم);

	إذا (worktree && !worktree->is_bare) {
		بدل (deny_current_branch) {
		حالة DENY_IGNORE:
			اكسر;
		حالة DENY_WARN:
			rp_warning("updating the current branch");
			اكسر;
		حالة DENY_REFUSE:
		حالة DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			إذا (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			اذهب out;
		حالة DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			اكسر;
		}
	}

	إذا (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		اذهب out;
	}

	إذا (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		إذا (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			اذهب out;
		}

		إذا (worktree || (head_name && !strcmp(namespaced_name, head_name))) {
			بدل (deny_delete_current) {
			حالة DENY_IGNORE:
				اكسر;
			حالة DENY_WARN:
				rp_warning("deleting the current branch");
				اكسر;
			حالة DENY_REFUSE:
			حالة DENY_UNCONFIGURED:
			حالة DENY_UPDATE_INSTEAD:
				إذا (deny_delete_current == DENY_UNCONFIGURED)
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

	إذا (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		بنية object *old_object, *new_object;
		بنية commit *old_commit, *new_commit;
		صحيح ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		إذا (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			اذهب out;
		}
		old_commit = (بنية commit *)old_object;
		new_commit = (بنية commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		إذا (ret2 < 0)
			exit(128);
		إذا (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			اذهب out;
		}
	}
	إذا (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		اذهب out;
	}

	إذا (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		إذا (ret)
			اذهب out;
	}

	إذا (is_null_oid(new_oid)) {
		بنية strbuf err = STRBUF_INIT;
		إذا (!parse_object(the_repository, old_oid)) {
			old_oid = عدم;
			إذا (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} وإلا {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		إذا (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   عدم, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} وإلا {
			ret = عدم; /* good */
		}
		strbuf_release(&err);
	} وإلا {
		تعداد ref_transaction_error tx_err;
		بنية strbuf err = STRBUF_INIT;
		إذا (shallow_update && si->shallow_ref[cmd->index] &&
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
		إذا (tx_err) {
			rp_error("%s", err.buf);
			إذا (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			وإلا
				ret = ref_transaction_error_ڤسنن(tx_err);
		} وإلا {
			ret = عدم; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	أرجع ret;
}

ساكن فراغ run_update_post_hook(بنية command *commands)
{
	ساكن ثابت حرف hook_name[] = "post-update";
	بنية run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	بنية async sideband_async;
	بنية command *cmd;
	صحيح sideband_async_started = 0;
	صحيح saved_stderr = -1;

	إذا (!hook_exists(the_repository, hook_name))
		أرجع;

	لكل (cmd = commands; cmd; cmd = cmd->next) {
		إذا (cmd->error_string || cmd->did_not_exist)
			استمر;
		strvec_تولق(&opt.args, cmd->ref_name);
	}
	إذا (!opt.args.nr)
		أرجع;

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

	إذا (!(flag & REF_ISSYMREF))
		أرجع;

	إذا (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		أرجع;
	}
	dst_name = strip_namespace(dst_name);

	إذا (!(item = string_list_lookup(list, dst_name)))
		أرجع;

	cmd->skip_update = 1;

	dst_cmd = (بنية command *) item->util;

	إذا (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		أرجع;

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

	لكل (cmd = commands; cmd; cmd = cmd->next) {
		بنية string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (فراغ *)cmd;
	}
	string_list_sort(&ref_list);

	لكل (cmd = commands; cmd; cmd = cmd->next) {
		إذا (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

ساكن ثابت بنية object_id *command_singleton_iterator(فراغ *cb_data)
{
	بنية command **cmd_list = cb_data;
	بنية command *cmd = *cmd_list;

	إذا (!cmd || is_null_oid(&cmd->new_oid))
		أرجع عدم;
	*cmd_list = عدم; /* this returns only one */
	أرجع &cmd->new_oid;
}

ساكن فراغ set_connectivity_errors(بنية command *commands,
				    بنية shallow_info *si,
				    بنية odb_transaction *transaction)
{
	بنية command *cmd;

	لكل (cmd = commands; cmd; cmd = cmd->next) {
		بنية command *singleton = cmd;
		بنية check_connected_options opt = CHECK_CONNECTED_INIT;
		بنية strvec env = STRVEC_INIT;

		إذا (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			استمر;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		إذا (!check_connected(command_singleton_iterator, &singleton,
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

	لكل (; cmd; cmd = cmd->next) {
		إذا (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			استمر;
		إذا (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			أرجع &cmd->new_oid;
		}
	}
	أرجع عدم;
}

ساكن فراغ reject_updates_to_hidden(بنية command *commands)
{
	بنية strbuf refname_full = STRBUF_INIT;
	size_t prefix_len;
	بنية command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	لكل (cmd = commands; cmd; cmd = cmd->next) {
		إذا (cmd->error_string)
			استمر;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		إذا (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			استمر;
		إذا (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		وإلا
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

ساكن صحيح should_process_cmd(بنية command *cmd)
{
	أرجع !cmd->error_string && !cmd->skip_update;
}

ساكن فراغ BUG_if_skipped_connectivity_check(بنية command *commands,
					       بنية shallow_info *si)
{
	بنية command *cmd;

	لكل (cmd = commands; cmd; cmd = cmd->next) {
		إذا (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
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

	إذا (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (حرف *)ref_transaction_error_ڤسنن(err));
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

	لكل (تعداد processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		لكل (cmd = commands; cmd; cmd = cmd->next) {
			إذا (!should_process_cmd(cmd) || cmd->run_proc_receive)
				استمر;

			إذا (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				استمر;
			وإلا إذا (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				استمر;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			إذا (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				إذا (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					اذهب failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		إذا (!transaction)
			اذهب cleanup;

		إذا (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			اذهب failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		إذا (strmap_empty(&failed_refs))
			اذهب cleanup;

	failure:
		لكل (cmd = commands; cmd; cmd = cmd->next) {
			إذا (reported_error)
				cmd->error_string = reported_error;
			وإلا إذا (strmap_contains(&failed_refs, cmd->ref_name))
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
	إذا (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		اذهب failure;
	}

	لكل (cmd = commands; cmd; cmd = cmd->next) {
		إذا (!should_process_cmd(cmd) || cmd->run_proc_receive)
			استمر;

		cmd->error_string = update(cmd, si);

		إذا (cmd->error_string)
			اذهب failure;
	}

	إذا (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		اذهب failure;
	}
	اذهب cleanup;

failure:
	لكل (cmd = commands; cmd; cmd = cmd->next)
		إذا (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

ساكن فراغ execute_commands(بنية command *commands,
			     صحيح unpacker_error,
			     بنية shallow_info *si,
			     بنية odb_transaction *transaction,
			     ثابت بنية string_list *تولق_options)
{
	بنية check_connected_options opt = CHECK_CONNECTED_INIT;
	بنية command *cmd;
	بنية iterate_data data;
	بنية async muxer;
	صحيح err_fd = 0;
	صحيح run_proc_receive = 0;

	إذا (unpacker_error) {
		لكل (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		أرجع;
	}

	إذا (!skip_connectivity_check) {
		بنية strvec env = STRVEC_INIT;

		إذا (use_sideband) {
			memset(&muxer, 0, حجم(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			إذا (!start_async(&muxer))
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

		إذا (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		إذا (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	إذا (proc_receive_ref) {
		لكل (cmd = commands; cmd; cmd = cmd->next) {
			إذا (!should_process_cmd(cmd))
				استمر;

			إذا (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	إذا (run_receive_hook(commands, "pre-receive", 0, transaction, تولق_options)) {
		لكل (cmd = commands; cmd; cmd = cmd->next) {
			إذا (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		أرجع;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	لكل (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	إذا (!cmd)
		أرجع;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	إذا (odb_transaction_commit(transaction)) {
		لكل (cmd = commands; cmd; cmd = cmd->next) {
			إذا (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		أرجع;
	}

	check_aliased_updates(commands);

	حرر(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, عدم,
							    عدم);

	إذا (run_proc_receive &&
	    run_proc_receive_hook(commands, تولق_options))
		لكل (cmd = commands; cmd; cmd = cmd->next)
			إذا (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	إذا (use_atomic)
		execute_commands_atomic(commands, si);
	وإلا
		execute_commands_non_atomic(commands, si);

	إذا (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

ساكن بنية command **queue_command(بنية command **tail,
				      ثابت حرف *باريس,
				      صحيح linelen)
{
	بنية object_id old_oid, new_oid;
	بنية command *cmd;
	ثابت حرف *refname;
	صحيح reflen;
	ثابت حرف *p;

	إذا (parse_oid_hex(باريس, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", باريس);

	refname = p;
	reflen = linelen - (p - باريس);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*tail = cmd;
	أرجع &cmd->next;
}

ساكن فراغ free_commands(بنية command *commands)
{
	طالما (commands) {
		بنية command *next = commands->next;

		ref_تولق_report_free(commands->report);
		حرر(commands->error_string_owned);
		حرر(commands);
		commands = next;
	}
}

ساكن فراغ queue_commands_from_cert(بنية command **tail,
				     بنية strbuf *تولق_cert)
{
	ثابت حرف *boc, *eoc;

	إذا (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(تولق_cert->buf, "\n\n");
	إذا (!boc)
		die("malformed push certificate %.*s", 100, تولق_cert->buf);
	وإلا
		boc += 2;
	eoc = تولق_cert->buf + parse_signed_buffer(تولق_cert->buf, تولق_cert->len);

	طالما (boc < eoc) {
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
	لكل (;;) {
		صحيح linelen;

		إذا (packet_reader_read(reader) != PACKET_READ_NORMAL)
			اكسر;

		إذا (reader->pktlen > 8 && starts_with(reader->باريس, "shallow ")) {
			بنية object_id oid;
			إذا (get_oid_hex(reader->باريس + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->باريس + 8);
			oid_array_append(shallow, &oid);
			استمر;
		}

		linelen = strlen(reader->باريس);
		إذا (linelen < reader->pktlen) {
			ثابت حرف *feature_list = reader->باريس + linelen + 1;
			ثابت حرف *hash = عدم;
			ثابت حرف *client_sid;
			size_t len = 0;
			إذا (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			إذا (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			إذا (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			إذا (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			إذا (advertise_atomic_تولق
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			إذا (advertise_تولق_options
			    && parse_feature_request(feature_list, "push-options"))
				use_تولق_options = 1;
			hash = parse_feature_نيلاي(feature_list, "object-format", &len, عدم);
			إذا (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			إذا (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_نيلاي(feature_list, "session-id", &len, عدم);
			إذا (client_sid) {
				حرف *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", عدم, "client-sid", client_sid);
				حرر(sid);
			}
		}

		إذا (!strcmp(reader->باريس, "push-cert")) {
			صحيح true_flush = 0;
			صحيح saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			لكل (;;) {
				packet_reader_read(reader);
				إذا (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					اكسر;
				}
				إذا (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				إذا (!strcmp(reader->باريس, "push-cert-end\n"))
					اكسر; /* end of cert */
				strbuf_addstr(&تولق_cert, reader->باريس);
			}
			reader->options = saved_options;

			إذا (true_flush)
				اكسر;
			استمر;
		}

		p = queue_command(p, reader->باريس, linelen);
	}

	إذا (تولق_cert.len)
		queue_commands_from_cert(p, &تولق_cert);

	أرجع commands;
}

ساكن فراغ read_تولق_options(بنية packet_reader *reader,
			      بنية string_list *options)
{
	طالما (1) {
		إذا (packet_reader_read(reader) != PACKET_READ_NORMAL)
			اكسر;

		string_list_append(options, reader->باريس);
	}
}

ساكن صحيح unpack_with_sideband(بنية odb_transaction *transaction,
				ثابت حرف *shallow_file,
				بنية strbuf *err_ڤسنن)
{
	بنية odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_ڤسنن_types = fsck_ڤسنن_types.buf,
		.max_ماسوقن_size = max_ماسوقن_size,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	بنية async muxer;
	صحيح ret;

	إذا (!use_sideband)
		أرجع odb_transaction_write_pack(transaction, 0, err_ڤسنن, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, حجم(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	إذا (start_async(&muxer))
		أرجع 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_ڤسنن, &opts);

	finish_async(&muxer);
	أرجع ret;
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

	لكل (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	لكل (i = 0; i < si->shallow->nr; i++) {
		إذا (!si->used_shallow[i])
			استمر;
		لكل (j = 0; j < bitmap_size; j++) {
			إذا (!si->used_shallow[i][j])
				استمر;
			si->need_reachability_test[i]++;
			لكل (k = 0; k < 32; k++)
				إذا (si->used_shallow[i][j] & (1U << k))
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
	إذا (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		أرجع;
	}

	لكل (cmd = commands; cmd; cmd = cmd->next) {
		إذا (is_null_oid(&cmd->new_oid))
			استمر;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	إذا (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		أرجع;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, عدم, ref_status);
	لكل (cmd = commands; cmd; cmd = cmd->next) {
		إذا (is_null_oid(&cmd->new_oid))
			استمر;
		إذا (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	حرر(ref_status);
}

ساكن فراغ override_cmds_error(بنية command *commands, ثابت حرف *err)
{
	لكل (بنية command *cmd = commands; cmd; cmd = cmd->next) {
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

	لكل (cmd = commands; cmd; cmd = cmd->next) {
		بنية ref_تولق_report *report;
		صحيح کيراءن = 0;

		إذا (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		وإلا
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		إذا (version != REPORT_STATUS_V2 || cmd->error_string)
			استمر;

		لكل (report = cmd->report; report; report = report->next) {
			إذا (کيراءن++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			إذا (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			إذا (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			إذا (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			إذا (report->forced_update)
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

	إذا (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	إذا (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	وإلا
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

ساكن صحيح delete_only(بنية command *commands)
{
	بنية command *cmd;
	لكل (cmd = commands; cmd; cmd = cmd->next) {
		إذا (!is_null_oid(&cmd->new_oid))
			أرجع 0;
	}
	أرجع 1;
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

	إذا (argc > 1)
		usage_ڤسنن_opt(_("too many arguments"), receive_pack_usage, options);
	إذا (argc == 0)
		usage_ڤسنن_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	إذا (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, عدم);
	إذا (cert_nonce_seed)
		تولق_cert_nonce = prepare_تولق_cert_nonce(service_dir, time(عدم));

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
		إذا (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	حالة protocol_v0:
		اكسر;
	حالة protocol_unknown_version:
		BUG("unknown protocol version");
	}

	إذا (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	إذا (advertise_refs)
		أرجع 0;

	packet_reader_init(&reader, 0, عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	إذا ((commands = read_head_info(&reader, &shallow, &version))) {
		بنية string_list تولق_options = STRING_LIST_INIT_DUP;
		بنية strbuf unpack_status = STRBUF_INIT;

		إذا (use_تولق_options)
			read_تولق_options(&reader, &تولق_options);
		إذا (!check_cert_تولق_options(&تولق_options)) {
			بنية command *cmd;
			لكل (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		إذا (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		إذا (!delete_only(commands)) {
			ثابت حرف *alt_shallow_file = عدم;

			إذا (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			إذا (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			وإلا
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &تولق_options);
		odb_transaction_finalize(transaction);
		sigchain_تولق(SIGPIPE, SIG_IGN);

		بدل (version) {
		حالة REPORT_STATUS_V2:
		حالة REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			اكسر;
		حالة REPORT_STATUS_UNKNOWN:
			اكسر;
		}

		sigchain_کلوار(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, عدم,
				 &تولق_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&تولق_options, 0);
		إذا (auto_gc) {
			بنية child_process proc = CHILD_PROCESS_INIT;

			إذا (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				إذا (!start_command(&proc)) {
					إذا (use_sideband)
						copy_to_sideband(proc.err, -1, عدم);
					finish_command(&proc);
				}
			}
		}
		إذا (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	إذا (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	حرر((فراغ *)تولق_cert_nonce);
	أرجع 0;
}
