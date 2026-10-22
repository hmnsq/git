#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساکن ثابت حرف * ثابت receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	تہی
};

شمار deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

شمار report_حالت_نسخہ {
	REPORT_حالت_UNKNOWN = 0,
	REPORT_حالت_V0,
	REPORT_حالت_V2,
};

ساکن صحیح deny_deletes;
ساکن صحیح deny_non_fast_forwards;
ساکن شمار deny_action deny_current_branch = DENY_UNCONFIGURED;
ساکن شمار deny_action deny_delete_current = DENY_UNCONFIGURED;
ساکن صحیح receive_fsck_objects = -1;
ساکن صحیح transfer_fsck_objects = -1;
ساکن ساخت strbuf fsck_پیغام_types = STRBUF_INIT;
ساکن صحیح advertise_atomic_ڈالو = 1;
ساکن صحیح advertise_ڈالو_options;
ساکن صحیح advertise_sid;
ساکن off_t max_داخلہ_سائز;
ساکن صحیح use_sideband;
ساکن صحیح use_atomic;
ساکن صحیح use_ڈالو_options;
ساکن صحیح quiet;
ساکن صحیح prefer_ofs_delta = 1;
ساکن صحیح auto_update_server_info;
ساکن صحیح auto_gc = 1;
ساکن صحیح reject_thin;
ساکن صحیح skip_connectivity_check;
ساکن صحیح stateless_rpc;
ساکن ثابت حرف *service_dir;
ساکن ثابت حرف *سر_name;
ساکن خالی *سر_name_مقصد_free;
ساکن صحیح بھیجا_capabilities;
ساکن صحیح shallow_update;
ساکن ساخت strbuf ڈالو_cert = STRBUF_INIT;
ساکن ساخت object_id ڈالو_cert_oid;
ساکن ساخت signature_check sigcheck;
ساکن ثابت حرف *ڈالو_cert_nonce;
ساکن حرف *cert_nonce_بیج;
ساکن ساخت strvec hidden_refs = STRVEC_INIT;

ساکن ثابت حرف *NONCE_UNSOLICITED = "UNSOLICITED";
ساکن ثابت حرف *NONCE_BAD = "BAD";
ساکن ثابت حرف *NONCE_MISSING = "MISSING";
ساکن ثابت حرف *NONCE_OK = "OK";
ساکن ثابت حرف *NONCE_SLOP = "SLOP";
ساکن ثابت حرف *nonce_حالت;
ساکن طویل nonce_stamp_slop;
ساکن timestamp_t nonce_stamp_slop_حد_بندی;
ساکن ساخت ref_transaction *transaction;

ساکن شمار {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
ساکن صحیح keepalive_اندر_sec = 5;

ساکن ساخت proc_receive_ref {
	غیرعلامتی صحیح want_جوڑو:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	حرف *ref_سابقہ;
	ساخت proc_receive_ref *اگلا;
} *proc_receive_ref;

ساکن خالی proc_receive_ref_append(ثابت حرف *سابقہ);

ساکن شمار deny_action parse_deny_action(ثابت حرف *var, ثابت حرف *قدر)
{
	اگر (قدر) {
		اگر (!strcasecmp(قدر, "ignore"))
			واپس DENY_IGNORE;
		اگر (!strcasecmp(قدر, "warn"))
			واپس DENY_WARN;
		اگر (!strcasecmp(قدر, "refuse"))
			واپس DENY_REFUSE;
		اگر (!strcasecmp(قدر, "updateinstead"))
			واپس DENY_UPDATE_INSTEAD;
	}
	اگر (git_config_bool(var, قدر))
		واپس DENY_REFUSE;
	واپس DENY_IGNORE;
}

ساکن صحیح receive_pack_config(ثابت حرف *var, ثابت حرف *قدر,
			       ثابت ساخت config_context *ctx, خالی *cb)
{
	ثابت حرف *پیغام_id_2;
	صحیح حالت_2 = parse_hide_refs_config(var, قدر, "receive", &hidden_refs);

	اگر (حالت_2)
		واپس حالت_2;

	اگر (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (strcmp(var, "receive.fsck.skiplist") == 0) {
		حرف *راستہ;

		اگر (git_config_pathname(&راستہ, var, قدر))
			واپس -1;
		اگر (راستہ)
			strbuf_addf(&fsck_پیغام_types, "%cskiplist=%s",
				    fsck_پیغام_types.len ? ',' : '=', راستہ);
		آزاد(راستہ);
		واپس 0;
	}

	اگر (skip_سابقہ(var, "receive.fsck.", &پیغام_id_2)) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		اگر (is_valid_پیغام_type(پیغام_id_2, قدر))
			strbuf_addf(&fsck_پیغام_types, "%c%s=%s",
				fsck_پیغام_types.len ? ',' : '=', پیغام_id_2, قدر);
		ورنہ
			warning("skipping unknown msg id '%s'", پیغام_id_2);
		واپس 0;
	}

	اگر (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, قدر);
		واپس 0;
	}

	اگر (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, قدر);
		واپس 0;
	}

	اگر (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (strcmp(var, "receive.certnonceseed") == 0)
		واپس git_config_string(&cert_nonce_بیج, var, قدر);

	اگر (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_حد_بندی = git_config_ulong(var, قدر, ctx->kvi);
		واپس 0;
	}

	اگر (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_ڈالو = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_ڈالو_options = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (strcmp(var, "receive.keepalive") == 0) {
		keepalive_اندر_sec = git_config_int(var, قدر, ctx->kvi);
		واپس 0;
	}

	اگر (strcmp(var, "receive.maxinputsize") == 0) {
		max_داخلہ_سائز = git_config_int64(var, قدر, ctx->kvi);
		واپس 0;
	}

	اگر (strcmp(var, "receive.procreceiverefs") == 0) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		proc_receive_ref_append(قدر);
		واپس 0;
	}

	اگر (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, قدر);
		واپس 0;
	}

	واپس git_default_config(var, قدر, ctx, cb);
}

ساکن خالی show_ref(ثابت حرف *راستہ, ثابت ساخت object_id *oid)
{
	اگر (بھیجا_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_مقصد_hex(oid), راستہ);
	} ورنہ {
		ساخت strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		اگر (advertise_atomic_ڈالو)
			strbuf_addstr(&cap, " atomic");
		اگر (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		اگر (ڈالو_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", ڈالو_cert_nonce);
		اگر (advertise_ڈالو_options)
			strbuf_addstr(&cap, " push-options");
		اگر (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_مقصد_hex(oid), راستہ, 0, cap.buf);
		strbuf_release(&cap);
		بھیجا_capabilities = 1;
	}
}

ساکن صحیح show_ref_cb(ثابت ساخت reference *ref, خالی *ڈیٹا)
{
	ساخت oidset *نظر_آیا = ڈیٹا;
	ثابت حرف *راستہ = strip_namespace(ref->name);

	اگر (ref_is_hidden(راستہ, ref->name, &hidden_refs))
		واپس 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	اگر (!راستہ) {
		اگر (oidset_داخل_کرو(نظر_آیا, ref->oid))
			واپس 0;
		راستہ = ".have";
	} ورنہ {
		oidset_داخل_کرو(نظر_آیا, ref->oid);
	}
	show_ref(راستہ, ref->oid);
	واپس 0;
}

ساکن خالی show_one_alternate_ref(ثابت ساخت object_id *oid,
				   خالی *ڈیٹا)
{
	ساخت oidset *نظر_آیا = ڈیٹا;

	اگر (oidset_داخل_کرو(نظر_آیا, oid))
		واپس;

	show_ref(".have", oid);
}

ساکن خالی write_سر_info(خالی)
{
	ساخت refs_for_each_ref_options opts = { 0 };
	ساکن ساخت oidset نظر_آیا = OIDSET_INIT;
	ساخت strvec excludes_سمتیہ = STRVEC_INIT;

	/*
	 * We need access to the reference names both with and without their
	 * namespace and thus cannot use `refs_for_each_namespaced_ref()`. We
	 * thus have to adapt exclude patterns to carry the namespace prefix
	 * ourselves.
	 */
	opts.exclude_patterns = get_namespaced_exclude_patterns(
		hidden_refs_مقصد_excludes(&hidden_refs),
		get_git_namespace(), &excludes_سمتیہ);

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      show_ref_cb, &نظر_آیا, &opts);
	odb_for_each_alternate_ref(the_repository->objects,
				   show_one_alternate_ref, &نظر_آیا);

	oidset_clear(&نظر_آیا);
	strvec_clear(&excludes_سمتیہ);

	اگر (!بھیجا_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#تعریف_کرو تسلسل_PROC_RECEIVE_SCHEDULED	1
#تعریف_کرو تسلسل_PROC_RECEIVE_RETURNED	2
ساخت command {
	ساخت command *اگلا;
	ثابت حرف *error_string;
	حرف *error_string_owned;
	ساخت ref_ڈالو_report *report;
	غیرعلامتی صحیح skip_update:1,
		     did_not_exist:1,
		     تسلسل_proc_receive:2;
	صحیح اشاریہ;
	ساخت object_id old_oid;
	ساخت object_id new_oid;
	حرف ref_name[FLEX_ARRAY]; /*مزید*/
};

ساکن خالی proc_receive_ref_append(ثابت حرف *سابقہ)
{
	ساخت proc_receive_ref *ref_نمونہ;
	ثابت حرف *p;
	صحیح len;

	CALLOC_ARRAY(ref_نمونہ, 1);
	p = strchr(سابقہ, ':');
	اگر (p) {
		جب (سابقہ < p) {
			اگر (*سابقہ == 'a')
				ref_نمونہ->want_جوڑو = 1;
			ورنہ اگر (*سابقہ == 'd')
				ref_نمونہ->want_delete = 1;
			ورنہ اگر (*سابقہ == 'm')
				ref_نمونہ->want_modify = 1;
			ورنہ اگر (*سابقہ == '!')
				ref_نمونہ->negative_ref = 1;
			سابقہ++;
		}
		سابقہ++;
	} ورنہ {
		ref_نمونہ->want_جوڑو = 1;
		ref_نمونہ->want_delete = 1;
		ref_نمونہ->want_modify = 1;
	}
	len = لڑی_لمبائی(سابقہ);
	جب (len && سابقہ[len - 1] == '/')
		len--;
	ref_نمونہ->ref_سابقہ = xmemdupz(سابقہ, len);
	اگر (!proc_receive_ref) {
		proc_receive_ref = ref_نمونہ;
	} ورنہ {
		ساخت proc_receive_ref *انتہا;

		انتہا = proc_receive_ref;
		جب (انتہا->اگلا)
			انتہا = انتہا->اگلا;
		انتہا->اگلا = ref_نمونہ;
	}
}

ساکن صحیح proc_receive_ref_matches(ساخت command *cmd)
{
	ساخت proc_receive_ref *p;

	اگر (!proc_receive_ref)
		واپس 0;

	برائے (p = proc_receive_ref; p; p = p->اگلا) {
		ثابت حرف *مطابقت = p->ref_سابقہ;
		ثابت حرف *remains;

		اگر (!p->want_جوڑو && is_null_oid(&cmd->old_oid))
			جاری;
		ورنہ اگر (!p->want_delete && is_null_oid(&cmd->new_oid))
			جاری;
		ورنہ اگر (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			جاری;

		اگر (skip_سابقہ(cmd->ref_name, مطابقت, &remains) &&
		    (!*remains || *remains == '/')) {
			اگر (!p->negative_ref)
				واپس 1;
		} ورنہ اگر (p->negative_ref) {
			واپس 1;
		}
	}
	واپس 0;
}

ساکن خالی report_پیغام(ثابت حرف *سابقہ, ثابت حرف *err, va_list params)
{
	صحیح sz;
	حرف پیغام[4096];

	sz = xsnprintf(پیغام, حجم(پیغام), "%s", سابقہ);
	sz += vsnprintf(پیغام + sz, حجم(پیغام) - sz, err, params);
	اگر (sz > (حجم(پیغام) - 1))
		sz = حجم(پیغام) - 1;
	پیغام[sz++] = '\n';

	اگر (use_sideband)
		send_sideband(1, 2, پیغام, sz, use_sideband);
	ورنہ
		xwrite(2, پیغام, sz);
}

__attribute__((format (لکھو, 1, 2)))
ساکن خالی rp_warning(ثابت حرف *err, ...)
{
	va_list params;
	va_آغاز(params, err);
	report_پیغام("warning: ", err, params);
	va_انتہا(params);
}

__attribute__((format (لکھو, 1, 2)))
ساکن خالی rp_error(ثابت حرف *err, ...)
{
	va_list params;
	va_آغاز(params, err);
	report_پیغام("error: ", err, params);
	va_انتہا(params);
}

ساکن صحیح copy_مقصد_sideband(صحیح اندر, صحیح برآمد غیر_مستعمل, خالی *arg غیر_مستعمل)
{
	حرف ڈیٹا[128];
	صحیح keepalive_فعال = 0;

	اگر (keepalive_اندر_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	اگر (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_فعال = 1;

	جب (1) {
		ssize_t sz;

		اگر (keepalive_فعال) {
			ساخت pollfd pfd;
			صحیح ret;

			pfd.fd = اندر;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_اندر_sec);

			اگر (ret < 0) {
				اگر (errno == EINTR)
					جاری;
				ورنہ
					توڑو;
			} ورنہ اگر (ret == 0) {
				/* no data; send a keepalive packet */
				ساکن ثابت حرف buf[] = "0005\1";
				write_or_die(1, buf, حجم(buf) - 1);
				جاری;
			} /* else there is actual data to read */
		}

		sz = xread(اندر, ڈیٹا, حجم(ڈیٹا));
		اگر (sz <= 0)
			توڑو;

		اگر (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_فعال) {
			ثابت حرف *p = memchr(ڈیٹا, '\0', sz);
			اگر (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_فعال = 1;
				send_sideband(1, 2, ڈیٹا, p - ڈیٹا, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - ڈیٹا + 1), use_sideband);
				جاری;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, ڈیٹا, sz, use_sideband);
	}
	close(اندر);
	واپس 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
ساکن خالی prepare_sideband_async(ساخت async *sideband_async, صحیح *saved_stderr, صحیح *started)
{
	*started = 0;

	اگر (!use_sideband)
		واپس;

	memset(sideband_async, 0, حجم(*sideband_async));
	sideband_async->proc = copy_مقصد_sideband;
	sideband_async->اندر = -1;

	اگر (!آغاز_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		اگر (*saved_stderr >= 0)
			dup2(sideband_async->اندر, STDERR_FILENO);
		close(sideband_async->اندر);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
ساکن خالی اختتام_sideband_async(ساخت async *sideband_async, صحیح saved_stderr, صحیح started)
{
	اگر (!use_sideband)
		واپس;

	اگر (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	اگر (started)
		اختتام_async(sideband_async);
}

ساکن خالی hmac_hash(غیرعلامتی حرف *برآمد,
		      ثابت حرف *کلید_اندر, سائز_t کلید_len,
		      ثابت حرف *متن, سائز_t متن_len)
{
	غیرعلامتی حرف کلید[GIT_MAX_BLKSZ];
	غیرعلامتی حرف k_ipad[GIT_MAX_BLKSZ];
	غیرعلامتی حرف k_opad[GIT_MAX_BLKSZ];
	صحیح i;
	ساخت git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(کلید, '\0', GIT_MAX_BLKSZ);
	اگر (the_hash_algo->blksz < کلید_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, کلید_اندر, کلید_len);
		git_hash_final(کلید, &ctx);
	} ورنہ {
		حافظہ_نقل(کلید, کلید_اندر, کلید_len);
	}

	/* RFC 2104 2. (2) & (5) */
	برائے (i = 0; i < حجم(کلید); i++) {
		k_ipad[i] = کلید[i] ^ 0x36;
		k_opad[i] = کلید[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, حجم(k_ipad));
	git_hash_update(&ctx, متن, متن_len);
	git_hash_final(برآمد, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, حجم(k_opad));
	git_hash_update(&ctx, برآمد, the_hash_algo->rawsz);
	git_hash_final(برآمد, &ctx);
}

ساکن حرف *prepare_ڈالو_cert_nonce(ثابت حرف *راستہ, timestamp_t stamp)
{
	ساخت strbuf buf = STRBUF_INIT;
	غیرعلامتی حرف hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, راستہ, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_بیج, لڑی_لمبائی(cert_nonce_بیج));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (صحیح)the_hash_algo->hexsz, hash_مقصد_hex(hash));
	واپس strbuf_detach(&buf, تہی);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
ساکن صحیح constant_memequal(ثابت حرف *a, ثابت حرف *b, سائز_t n)
{
	صحیح res = 0;
	سائز_t i;

	برائے (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	واپس res;
}

ساکن ثابت حرف *check_nonce(ثابت حرف *buf)
{
	سائز_t noncelen;
	ثابت حرف *ملا_2 = ڈھونڈو_commit_سرخی(buf, "nonce", &noncelen);
	حرف *nonce = ملا_2 ? xmemdupz(ملا_2, noncelen) : تہی;
	timestamp_t stamp, ostamp;
	حرف *bohmac, *expect = تہی;
	ثابت حرف *retval = NONCE_BAD;

	اگر (!nonce) {
		retval = NONCE_MISSING;
		جاؤ leave;
	} ورنہ اگر (!ڈالو_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		جاؤ leave;
	} ورنہ اگر (!strcmp(ڈالو_cert_nonce, nonce)) {
		retval = NONCE_OK;
		جاؤ leave;
	}

	اگر (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		جاؤ leave;
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
		جاؤ leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	اگر (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		جاؤ leave;
	}

	expect = prepare_ڈالو_cert_nonce(service_dir, stamp);
	اگر (noncelen != لڑی_لمبائی(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		جاؤ leave;
	}
	اگر (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		جاؤ leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(ڈالو_cert_nonce, تہی, 10);
	nonce_stamp_slop = (طویل)ostamp - (طویل)stamp;

	اگر (nonce_stamp_slop_حد_بندی &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_حد_بندی) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		آزاد((خالی *)ڈالو_cert_nonce);
		ڈالو_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} ورنہ {
		retval = NONCE_SLOP;
	}

leave:
	آزاد(nonce);
	آزاد(expect);
	واپس retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
ساکن صحیح check_cert_ڈالو_options(ثابت ساخت string_list *ڈالو_options)
{
	ثابت حرف *buf = ڈالو_cert.buf;

	ثابت حرف *option;
	سائز_t optionlen;
	صحیح options_نظر_آیا = 0;

	صحیح retval = 1;

	اگر (!*buf)
		واپس 1;

	جب ((option = ڈھونڈو_commit_سرخی(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_نظر_آیا++;
		اگر (options_نظر_آیا > ڈالو_options->nr
		    || xstrncmpz(ڈالو_options->اشیاء[options_نظر_آیا - 1].string,
				 option, optionlen))
			واپس 0;
	}

	اگر (options_نظر_آیا != ڈالو_options->nr)
		retval = 0;

	واپس retval;
}

ساکن خالی prepare_ڈالو_cert_sha1(ساخت تسلسل_hooks_opt *opt)
{
	ساکن صحیح already_مکمل;

	اگر (!ڈالو_cert.len)
		واپس;

	اگر (!already_مکمل) {
		صحیح bogs /* beginning_of_gpg_sig */;

		already_مکمل = 1;
		اگر (odb_write_object(the_repository->objects, ڈالو_cert.buf,
				     ڈالو_cert.len, OBJ_BLOB, &ڈالو_cert_oid))
			oidclr(&ڈالو_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', حجم(sigcheck));

		bogs = parse_signed_بفر(ڈالو_cert.buf, ڈالو_cert.len);
		sigcheck.payload = xmemdupz(ڈالو_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, ڈالو_cert.buf + bogs,
				ڈالو_cert.len - bogs);

		nonce_حالت = check_nonce(sigcheck.payload);
	}
	اگر (!is_null_oid(&ڈالو_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_مقصد_hex(&ڈالو_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.کلید ? sigcheck.کلید : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.نتیجہ);
		اگر (ڈالو_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     ڈالو_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_حالت);
			اگر (nonce_حالت == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

ساخت receive_hook_feed_state {
	ساخت command *cmd;
	ساخت ref_ڈالو_report *report;
	صحیح skip_broken;
	ساخت strbuf buf;
};

ساکن صحیح feed_receive_hook_cb(صحیح hook_stdin_fd, خالی *pp_cb غیر_مستعمل, خالی *pp_task_cb)
{
	ساخت receive_hook_feed_state *state = pp_task_cb;
	ساخت command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	جب (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->اگلا;

	اگر (!cmd)
		واپس 1;  /* no more commands left */

	اگر (!state->report)
		state->report = cmd->report;

	اگر (state->report) {
		ساخت object_id *old_oid;
		ساخت object_id *new_oid;
		ثابت حرف *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_مقصد_hex(old_oid), oid_مقصد_hex(new_oid),
			    ref_name);

		state->report = state->report->اگلا;
		اگر (!state->report)
			cmd = cmd->اگلا;
	} ورنہ {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_مقصد_hex(&cmd->old_oid), oid_مقصد_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->اگلا;
	}

	state->cmd = cmd;

	اگر (state->buf.len > 0) {
		صحیح ret = write_اندر_بھرا(hook_stdin_fd, state->buf.buf, state->buf.len);
		اگر (ret < 0) {
			اگر (errno == EPIPE)
				واپس 1; /* child closed pipe */
			واپس ret;
		}
	}

	واپس state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

ساکن خالی *receive_hook_feed_state_alloc(خالی *feed_pipe_ctx)
{
	ساخت receive_hook_feed_state *init_state = feed_pipe_ctx;
	ساخت receive_hook_feed_state *ڈیٹا;

	CALLOC_ARRAY(ڈیٹا, 1);
	ڈیٹا->report = init_state->report;
	ڈیٹا->cmd = init_state->cmd;
	ڈیٹا->skip_broken = init_state->skip_broken;
	strbuf_init(&ڈیٹا->buf, 0);

	واپس ڈیٹا;
}

ساکن خالی receive_hook_feed_state_free(خالی *ڈیٹا)
{
	ساخت receive_hook_feed_state *d = ڈیٹا;
	اگر (!d)
		واپس;
	strbuf_release(&d->buf);
	آزاد(d);
}

ساکن صحیح تسلسل_receive_hook(ساخت command *commands,
			    ثابت حرف *hook_name,
			    صحیح skip_broken,
			    ساخت odb_transaction *transaction,
			    ثابت ساخت string_list *ڈالو_options)
{
	ساخت تسلسل_hooks_opt opt = تسلسل_HOOKS_OPT_INIT;
	ساخت command *iter = commands;
	ساخت receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	ساخت async sideband_async;
	صحیح sideband_async_started = 0;
	صحیح saved_stderr = -1;
	صحیح ret;

	اگر (!hook_exists(the_repository, hook_name))
		واپس 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	جب (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->اگلا;
	اگر (!iter)
		واپس 0;

	اگر (ڈالو_options) {
		برائے (صحیح i = 0; i < ڈالو_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     ڈالو_options->اشیاء[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)ڈالو_options->nr);
	} ورنہ {
		strvec_ڈالو(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	اگر (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_ڈالو_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_ڈیٹا_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_ڈیٹا_free = receive_hook_feed_state_free;

	ret = تسلسل_hooks_opt(the_repository, hook_name, &opt);

	اختتام_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	واپس ret;
}

ساکن صحیح تسلسل_update_hook(ساخت command *cmd)
{
	ساکن ثابت حرف hook_name[] = "update";
	ساخت تسلسل_hooks_opt opt = تسلسل_HOOKS_OPT_INIT;
	ساخت async sideband_async;
	صحیح sideband_async_started = 0;
	صحیح saved_stderr = -1;
	صحیح رمز;

	اگر (!hook_exists(the_repository, hook_name))
		واپس 0;

	strvec_pushl(&opt.دلائل,
		     cmd->ref_name,
		     oid_مقصد_hex(&cmd->old_oid),
		     oid_مقصد_hex(&cmd->new_oid),
		     تہی);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	رمز = تسلسل_hooks_opt(the_repository, hook_name, &opt);

	اختتام_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	واپس رمز;
}

ساکن صحیح تسلسل_receive_report_hook(ساخت strbuf *report)
{
	ساخت بچہ_process proc = بچہ_PROCESS_INIT;
	ساخت async sideband_async;
	صحیح sideband_async_started = 0;
	صحیح saved_stderr = -1;
	ساخت strbuf برآمد = STRBUF_INIT;
	ثابت حرف *hook_راستہ;
	صحیح ret;

	hook_راستہ = ڈھونڈو_hook(the_repository, "receive-report");
	اگر (!hook_راستہ)
		واپس 0;

	strvec_ڈالو(&proc.دلائل, hook_راستہ);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_ڈالو(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &برآمد,
			   report->len, تہی, 0);
	sigchain_نکالو(SIGPIPE);

	اختتام_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	اگر (!ret)
		strbuf_تبادلہ(&برآمد, report);

	strbuf_release(&برآمد);
	واپس ret;
}

ساکن ساخت command *ڈھونڈو_command_by_refname(ساخت command *list,
					       ثابت حرف *refname)
{
	برائے (; list; list = list->اگلا)
		اگر (!strcmp(list->ref_name, refname))
			واپس list;
	واپس تہی;
}

ساکن صحیح read_proc_receive_report(ساخت packet_reader *reader,
				    ساخت command *commands,
				    ساخت strbuf *errmsg)
{
	ساخت command *cmd;
	ساخت command *hint = تہی;
	ساخت ref_ڈالو_report *report = تہی;
	صحیح new_report = 0;
	صحیح رمز = 0;
	صحیح once = 0;
	صحیح response = 0;

	برائے (;;) {
		ساخت object_id old_oid, new_oid;
		حرف *سر;
		حرف *refname;
		حرف *p;
		شمار packet_read_حالت حالت_2;

		حالت_2 = packet_reader_read(reader);
		اگر (حالت_2 != PACKET_READ_معمول) {
			/* Check whether proc-receive exited abnormally */
			اگر (حالت_2 == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				واپس -1;
			}
			توڑو;
		}
		response++;

		سر = reader->سطر;
		p = strchr(سر, ' ');
		اگر (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", سر);
			رمز = -1;
			جاری;
		}
		*p++ = '\0';
		اگر (!strcmp(سر, "option")) {
			حرف *کلید;
			ثابت حرف *val;

			اگر (!hint || !(report || new_report)) {
				اگر (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				رمز = -1;
				جاری;
			}
			اگر (new_report) {
				اگر (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} ورنہ {
					report = hint->report;
					جب (report->اگلا)
						report = report->اگلا;
					report->اگلا = xcalloc(1, حجم(ساخت ref_ڈالو_report));
					report = report->اگلا;
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
			ورنہ اگر (!strcmp(کلید, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			ورنہ اگر (!strcmp(کلید, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			ورنہ اگر (!strcmp(کلید, "forced-update"))
				report->forced_update = 1;
			ورنہ اگر (!strcmp(کلید, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->تسلسل_proc_receive = 0;
			جاری;
		}

		report = تہی;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		اگر (p)
			*p++ = '\0';
		اگر (strcmp(سر, "ok") && strcmp(سر, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    سر, refname);
			رمز = -1;
			جاری;
		}

		/* first try searching at our hint, falling back to all refs */
		اگر (hint)
			hint = ڈھونڈو_command_by_refname(hint, refname);
		اگر (!hint)
			hint = ڈھونڈو_command_by_refname(commands, refname);
		اگر (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			رمز = -1;
			جاری;
		}
		اگر (!hint->تسلسل_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			رمز = -1;
			جاری;
		}
		hint->تسلسل_proc_receive |= تسلسل_PROC_RECEIVE_RETURNED;
		اگر (!strcmp(سر, "ng")) {
			اگر (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			ورنہ
				hint->error_string = "failed";
			رمز = -1;
			جاری;
		}
		new_report = 1;
	}

	برائے (cmd = commands; cmd; cmd = cmd->اگلا)
		اگر (cmd->تسلسل_proc_receive && !cmd->error_string &&
		    !(cmd->تسلسل_proc_receive & تسلسل_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    رمز = -1;
		}
	واپس رمز;
}

ساکن صحیح تسلسل_proc_receive_hook(ساخت command *commands,
				 ثابت ساخت string_list *ڈالو_options)
{
	ساخت بچہ_process proc = بچہ_PROCESS_INIT;
	ساخت async muxer;
	ساخت command *cmd;
	ساخت packet_reader reader;
	ساخت strbuf cap = STRBUF_INIT;
	ساخت strbuf errmsg = STRBUF_INIT;
	صحیح hook_use_ڈالو_options = 0;
	صحیح نسخہ = 0;
	صحیح رمز;
	ثابت حرف *hook_راستہ = ڈھونڈو_hook(the_repository, "proc-receive");

	اگر (!hook_راستہ) {
		rp_error("cannot find hook 'proc-receive'");
		واپس -1;
	}

	strvec_ڈالو(&proc.دلائل, hook_راستہ);
	proc.اندر = -1;
	proc.برآمد = -1;
	proc.trace2_hook_name = "proc-receive";

	اگر (use_sideband) {
		memset(&muxer, 0, حجم(muxer));
		muxer.proc = copy_مقصد_sideband;
		muxer.اندر = -1;
		رمز = آغاز_async(&muxer);
		اگر (رمز)
			واپس رمز;
		proc.err = muxer.اندر;
	} ورنہ {
		proc.err = 0;
	}

	رمز = آغاز_command(&proc);
	اگر (رمز) {
		اگر (use_sideband)
			اختتام_async(&muxer);
		واپس رمز;
	}

	sigchain_ڈالو(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.برآمد, تہی, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	اگر (use_atomic)
		strbuf_addstr(&cap, " atomic");
	اگر (use_ڈالو_options)
		strbuf_addstr(&cap, " push-options");
	اگر (cap.len) {
		رمز = packet_write_fmt_gently(proc.اندر, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} ورنہ {
		رمز = packet_write_fmt_gently(proc.اندر, "version=1\n");
	}
	اگر (!رمز)
		رمز = packet_flush_gently(proc.اندر);

	اگر (!رمز)
		برائے (;;) {
			صحیح linelen;
			شمار packet_read_حالت حالت_2;

			حالت_2 = packet_reader_read(&reader);
			اگر (حالت_2 != PACKET_READ_معمول) {
				/* Check whether proc-receive exited abnormally */
				اگر (حالت_2 == PACKET_READ_EOF)
					رمز = -1;
				توڑو;
			}

			اگر (reader.pktlen > 8 && starts_with(reader.سطر, "version=")) {
				نسخہ = atoi(reader.سطر + 8);
				linelen = لڑی_لمبائی(reader.سطر);
				اگر (linelen < reader.pktlen) {
					ثابت حرف *feature_list = reader.سطر + linelen + 1;
					اگر (parse_feature_request(feature_list, "push-options"))
						hook_use_ڈالو_options = 1;
				}
			}
		}

	اگر (رمز) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		جاؤ cleanup;
	}

	انتخاب (نسخہ) {
	حالت 0:
		/* fallthrough */
	حالت 1:
		توڑو;
	طےشدہ:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    نسخہ);
		رمز = -1;
		جاؤ cleanup;
	}

	/* Send commands */
	برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
		اگر (!cmd->تسلسل_proc_receive || cmd->skip_update || cmd->error_string)
			جاری;
		رمز = packet_write_fmt_gently(proc.اندر, "%s %s %s",
					       oid_مقصد_hex(&cmd->old_oid),
					       oid_مقصد_hex(&cmd->new_oid),
					       cmd->ref_name);
		اگر (رمز)
			توڑو;
	}
	اگر (!رمز)
		رمز = packet_flush_gently(proc.اندر);
	اگر (رمز) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		جاؤ cleanup;
	}

	/* Send push options */
	اگر (hook_use_ڈالو_options) {
		ساخت string_list_شے *شے;

		for_each_string_list_شے(شے, ڈالو_options) {
			رمز = packet_write_fmt_gently(proc.اندر, "%s", شے->string);
			اگر (رمز)
				توڑو;
		}
		اگر (!رمز)
			رمز = packet_flush_gently(proc.اندر);
		اگر (رمز) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			جاؤ cleanup;
		}
	}

	/* Read result from proc-receive */
	رمز = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.اندر);
	close(proc.برآمد);
	اگر (use_sideband)
		اختتام_async(&muxer);
	اگر (اختتام_command(&proc))
		رمز = -1;
	اگر (errmsg.len >0) {
		حرف *p = errmsg.buf;

		p += errmsg.len - 1;
		اگر (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_نکالو(SIGPIPE);

	واپس رمز;
}

ساکن ثابت حرف *refuse_unconfigured_deny_پیغام =
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

ساکن خالی refuse_unconfigured_deny(خالی)
{
	rp_error("%s", _(refuse_unconfigured_deny_پیغام));
}

ساکن ثابت حرف *refuse_unconfigured_deny_delete_current_پیغام =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

ساکن خالی refuse_unconfigured_deny_delete_current(خالی)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_پیغام));
}

ساکن ثابت ساخت object_id *command_singleton_iterator(خالی *cb_ڈیٹا);
ساکن صحیح update_shallow_ref(ساخت command *cmd, ساخت shallow_info *si)
{
	ساخت shallow_تالا shallow_تالا = SHALLOW_تالا_INIT;
	ساخت oid_array extra = OID_ARRAY_INIT;
	ساخت check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t نقاب = 1 << (cmd->اشاریہ % 32);
	صحیح i;

	trace_printf_کلید(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	برائے (i = 0; i < si->shallow->nr; i++)
		اگر (si->مستعمل_shallow[i] &&
		    (si->مستعمل_shallow[i][cmd->اشاریہ / 32] & نقاب) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	تیاری_alternate_shallow(&shallow_تالا, &opt.shallow_file, &extra);
	اگر (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_تالا);
		oid_array_clear(&extra);
		واپس -1;
	}

	commit_shallow_file(the_repository, &shallow_تالا);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	برائے (i = 0; i < extra.nr; i++)
		رجسٹر_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->اشاریہ] = 0;
	oid_array_clear(&extra);
	واپس 0;
}

ساکن ثابت حرف *ڈالو_مقصد_deploy(غیرعلامتی حرف *sha1,
				  ساخت strvec *env,
				  ثابت ساخت worktree *worktree)
{
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;

	strvec_pushl(&بچہ.دلائل, "update-index", "-q", "--ignore-submodules",
		     "--refresh", تہی);
	strvec_pushv(&بچہ.env, env->v);
	بچہ.dir = worktree->راستہ;
	بچہ.no_stdin = 1;
	بچہ.stdout_مقصد_stderr = 1;
	بچہ.git_cmd = 1;
	اگر (تسلسل_command(&بچہ))
		واپس "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	بچہ_process_init(&بچہ);
	strvec_pushl(&بچہ.دلائل, "diff-files", "--quiet",
		     "--ignore-submodules", "--", تہی);
	strvec_pushv(&بچہ.env, env->v);
	بچہ.dir = worktree->راستہ;
	بچہ.no_stdin = 1;
	بچہ.stdout_مقصد_stderr = 1;
	بچہ.git_cmd = 1;
	اگر (تسلسل_command(&بچہ))
		واپس "Working directory has unstaged changes";

	بچہ_process_init(&بچہ);
	strvec_pushl(&بچہ.دلائل, "diff-index", "--quiet", "--cached",
		     "--ignore-submodules",
		     /*
		      * diff-index with either HEAD or an empty tree
		      *
		      * NEEDSWORK: is_null_oid() cannot know whether it's an
		      * unborn HEAD or a corrupt ref. It works for now because
		      * it's only needed to know if we are comparing HEAD or an
		      * empty tree.
		      */
		     !is_null_oid(&worktree->سر_oid) ? "HEAD" :
		     خالی_خانے_درخت_oid_hex(the_repository->hash_algo), "--", تہی);
	strvec_pushv(&بچہ.env, env->v);
	بچہ.no_stdin = 1;
	بچہ.no_stdout = 1;
	بچہ.stdout_مقصد_stderr = 0;
	بچہ.git_cmd = 1;
	اگر (تسلسل_command(&بچہ))
		واپس "Working directory has staged changes";

	بچہ_process_init(&بچہ);
	strvec_pushl(&بچہ.دلائل, "read-tree", "-u", "-m", hash_مقصد_hex(sha1),
		     تہی);
	strvec_pushv(&بچہ.env, env->v);
	بچہ.dir = worktree->راستہ;
	بچہ.no_stdin = 1;
	بچہ.no_stdout = 1;
	بچہ.stdout_مقصد_stderr = 0;
	بچہ.git_cmd = 1;
	اگر (تسلسل_command(&بچہ))
		واپس "Could not update working tree to new HEAD";

	واپس تہی;
}

ساکن ثابت حرف *ڈالو_مقصد_checkout_hook = "push-to-checkout";

ساکن ثابت حرف *ڈالو_مقصد_checkout(غیرعلامتی حرف *hash,
				    صحیح *invoked_hook,
				    ساخت strvec *env,
				    ثابت حرف *work_درخت)
{
	ساخت تسلسل_hooks_opt opt = تسلسل_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_راستہ(work_درخت));
	strvec_ڈالو(&opt.دلائل, hash_مقصد_hex(hash));
	اگر (تسلسل_hooks_opt(the_repository, ڈالو_مقصد_checkout_hook, &opt))
		واپس "push-to-checkout hook declined";
	ورنہ
		واپس تہی;
}

ساکن ثابت حرف *update_worktree(غیرعلامتی حرف *sha1, ثابت ساخت worktree *worktree)
{
	ثابت حرف *retval;
	حرف *git_dir;
	ساخت strvec env = STRVEC_INIT;
	صحیح invoked_hook;

	اگر (!worktree || !worktree->راستہ)
		BUG("worktree->path must be non-NULL");

	اگر (worktree->is_bare)
		واپس "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_راستہ(git_dir));

	retval = ڈالو_مقصد_checkout(sha1, &invoked_hook, &env, worktree->راستہ);
	اگر (!invoked_hook)
		retval = ڈالو_مقصد_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	آزاد(git_dir);
	واپس retval;
}

ساکن ثابت حرف *update(ساخت command *cmd, ساخت shallow_info *si)
{
	ثابت حرف *name = cmd->ref_name;
	ساخت strbuf namespaced_name_buf = STRBUF_INIT;
	ساکن حرف *namespaced_name;
	ثابت حرف *ret;
	ساخت object_id *old_oid = &cmd->old_oid;
	ساخت object_id *new_oid = &cmd->new_oid;
	صحیح do_update_worktree = 0;
	ساخت worktree **worktrees = get_worktrees(the_repository);
	ثابت ساخت worktree *worktree =
		ڈھونڈو_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	اگر (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		جاؤ برآمد;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	آزاد(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, تہی);

	اگر (worktree && !worktree->is_bare) {
		انتخاب (deny_current_branch) {
		حالت DENY_IGNORE:
			توڑو;
		حالت DENY_WARN:
			rp_warning("updating the current branch");
			توڑو;
		حالت DENY_REFUSE:
		حالت DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			اگر (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			جاؤ برآمد;
		حالت DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			توڑو;
		}
	}

	اگر (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_مقصد_hex(new_oid));
		ret = "bad pack";
		جاؤ برآمد;
	}

	اگر (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		اگر (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			جاؤ برآمد;
		}

		اگر (worktree || (سر_name && !strcmp(namespaced_name, سر_name))) {
			انتخاب (deny_delete_current) {
			حالت DENY_IGNORE:
				توڑو;
			حالت DENY_WARN:
				rp_warning("deleting the current branch");
				توڑو;
			حالت DENY_REFUSE:
			حالت DENY_UNCONFIGURED:
			حالت DENY_UPDATE_INSTEAD:
				اگر (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				جاؤ برآمد;
			طےشدہ:
				ret = "Invalid denyDeleteCurrent setting";
				جاؤ برآمد;
			}
		}
	}

	اگر (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		ساخت object *old_object, *new_object;
		ساخت commit *old_commit, *new_commit;
		صحیح ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		اگر (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			جاؤ برآمد;
		}
		old_commit = (ساخت commit *)old_object;
		new_commit = (ساخت commit *)new_object;
		ret2 = repo_اندر_ملاؤ_bases(the_repository, old_commit, new_commit);
		اگر (ret2 < 0)
			باہر_نکلو(128);
		اگر (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			جاؤ برآمد;
		}
	}
	اگر (تسلسل_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		جاؤ برآمد;
	}

	اگر (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		اگر (ret)
			جاؤ برآمد;
	}

	اگر (is_null_oid(new_oid)) {
		ساخت strbuf err = STRBUF_INIT;
		اگر (!parse_object(the_repository, old_oid)) {
			old_oid = تہی;
			اگر (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} ورنہ {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		اگر (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   تہی, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} ورنہ {
			ret = تہی; /* good */
		}
		strbuf_release(&err);
	} ورنہ {
		شمار ref_transaction_error tx_err;
		ساخت strbuf err = STRBUF_INIT;
		اگر (shallow_update && si->shallow_ref[cmd->اشاریہ] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			جاؤ برآمد;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  تہی, تہی,
						  0, "push",
						  &err);
		اگر (tx_err) {
			rp_error("%s", err.buf);
			اگر (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			ورنہ
				ret = ref_transaction_error_پیغام(tx_err);
		} ورنہ {
			ret = تہی; /* good */
		}
		strbuf_release(&err);
	}

برآمد:
	free_worktrees(worktrees);
	واپس ret;
}

ساکن خالی تسلسل_update_post_hook(ساخت command *commands)
{
	ساکن ثابت حرف hook_name[] = "post-update";
	ساخت تسلسل_hooks_opt opt = تسلسل_HOOKS_OPT_INIT;
	ساخت async sideband_async;
	ساخت command *cmd;
	صحیح sideband_async_started = 0;
	صحیح saved_stderr = -1;

	اگر (!hook_exists(the_repository, hook_name))
		واپس;

	برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
		اگر (cmd->error_string || cmd->did_not_exist)
			جاری;
		strvec_ڈالو(&opt.دلائل, cmd->ref_name);
	}
	اگر (!opt.دلائل.nr)
		واپس;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	تسلسل_hooks_opt(the_repository, hook_name, &opt);

	اختتام_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

ساکن خالی check_aliased_update_اندرونی(ساخت command *cmd,
					  ساخت string_list *list,
					  ثابت حرف *dst_name, صحیح flag)
{
	ساخت string_list_شے *شے;
	ساخت command *dst_cmd;

	اگر (!(flag & REF_ISSYMREF))
		واپس;

	اگر (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		واپس;
	}
	dst_name = strip_namespace(dst_name);

	اگر (!(شے = string_list_lookup(list, dst_name)))
		واپس;

	cmd->skip_update = 1;

	dst_cmd = (ساخت command *) شے->util;

	اگر (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		واپس;

	dst_cmd->skip_update = 1;

	rp_error("refusing inconsistent update between symref '%s' (%s..%s) and"
		 " its target '%s' (%s..%s)",
		 cmd->ref_name,
		 repo_ڈھونڈو_unique_abbrev(the_repository, &cmd->old_oid, DEFAULT_ABBREV),
		 repo_ڈھونڈو_unique_abbrev(the_repository, &cmd->new_oid, DEFAULT_ABBREV),
		 dst_cmd->ref_name,
		 repo_ڈھونڈو_unique_abbrev(the_repository, &dst_cmd->old_oid, DEFAULT_ABBREV),
		 repo_ڈھونڈو_unique_abbrev(the_repository, &dst_cmd->new_oid, DEFAULT_ABBREV));

	cmd->error_string = dst_cmd->error_string =
		"inconsistent aliased update";
}

ساکن خالی check_aliased_update(ساخت command *cmd, ساخت string_list *list)
{
	ساخت strbuf buf = STRBUF_INIT;
	ثابت حرف *dst_name;
	صحیح flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, تہی, &flag);
	check_aliased_update_اندرونی(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

ساکن خالی check_aliased_updates(ساخت command *commands)
{
	ساخت command *cmd;
	ساخت string_list ref_list = STRING_LIST_INIT_NODUP;

	برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
		ساخت string_list_شے *شے =
			string_list_append(&ref_list, cmd->ref_name);
		شے->util = (خالی *)cmd;
	}
	string_list_ترتیب_دو(&ref_list);

	برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
		اگر (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

ساکن ثابت ساخت object_id *command_singleton_iterator(خالی *cb_ڈیٹا)
{
	ساخت command **cmd_list = cb_ڈیٹا;
	ساخت command *cmd = *cmd_list;

	اگر (!cmd || is_null_oid(&cmd->new_oid))
		واپس تہی;
	*cmd_list = تہی; /* this returns only one */
	واپس &cmd->new_oid;
}

ساکن خالی سیٹ_connectivity_errors(ساخت command *commands,
				    ساخت shallow_info *si,
				    ساخت odb_transaction *transaction)
{
	ساخت command *cmd;

	برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
		ساخت command *singleton = cmd;
		ساخت check_connected_options opt = CHECK_CONNECTED_INIT;
		ساخت strvec env = STRVEC_INIT;

		اگر (shallow_update && si->shallow_ref[cmd->اشاریہ])
			/* to be checked in update_shallow_ref() */
			جاری;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		اگر (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			جاری;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

ساخت iterate_ڈیٹا {
	ساخت command *cmds;
	ساخت shallow_info *si;
};

ساکن ثابت ساخت object_id *iterate_receive_command_list(خالی *cb_ڈیٹا)
{
	ساخت iterate_ڈیٹا *ڈیٹا = cb_ڈیٹا;
	ساخت command **cmd_list = &ڈیٹا->cmds;
	ساخت command *cmd = *cmd_list;

	برائے (; cmd; cmd = cmd->اگلا) {
		اگر (shallow_update && ڈیٹا->si->shallow_ref[cmd->اشاریہ])
			/* to be checked in update_shallow_ref() */
			جاری;
		اگر (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->اگلا;
			واپس &cmd->new_oid;
		}
	}
	واپس تہی;
}

ساکن خالی reject_updates_مقصد_hidden(ساخت command *commands)
{
	ساخت strbuf refname_بھرا = STRBUF_INIT;
	سائز_t سابقہ_len;
	ساخت command *cmd;

	strbuf_addstr(&refname_بھرا, get_git_namespace());
	سابقہ_len = refname_بھرا.len;

	برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
		اگر (cmd->error_string)
			جاری;

		strbuf_setlen(&refname_بھرا, سابقہ_len);
		strbuf_addstr(&refname_بھرا, cmd->ref_name);

		اگر (!ref_is_hidden(cmd->ref_name, refname_بھرا.buf, &hidden_refs))
			جاری;
		اگر (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		ورنہ
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_بھرا);
}

ساکن صحیح should_process_cmd(ساخت command *cmd)
{
	واپس !cmd->error_string && !cmd->skip_update;
}

ساکن خالی BUG_if_skipped_connectivity_check(ساخت command *commands,
					       ساخت shallow_info *si)
{
	ساخت command *cmd;

	برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
		اگر (should_process_cmd(cmd) && si->shallow_ref[cmd->اشاریہ])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

ساکن خالی ref_transaction_rejection_handler(ثابت حرف *refname,
					      ثابت ساخت object_id *old_oid غیر_مستعمل,
					      ثابت ساخت object_id *new_oid غیر_مستعمل,
					      ثابت حرف *old_ہدف غیر_مستعمل,
					      ثابت حرف *new_ہدف غیر_مستعمل,
					      شمار ref_transaction_error err,
					      ثابت حرف *details,
					      خالی *cb_ڈیٹا)
{
	ساخت strmap *failed_refs = cb_ڈیٹا;

	اگر (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (حرف *)ref_transaction_error_پیغام(err));
}

ساکن خالی execute_commands_non_atomic(ساخت command *commands,
					ساخت shallow_info *si)
{
	ساخت command *cmd;
	ساخت strbuf err = STRBUF_INIT;
	ثابت حرف *reported_error = تہی;
	ساخت strmap failed_refs = STRMAP_INIT;

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
	شمار processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	برائے (شمار processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
			اگر (!should_process_cmd(cmd) || cmd->تسلسل_proc_receive)
				جاری;

			اگر (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				جاری;
			ورنہ اگر (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				جاری;

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
					جاؤ failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		اگر (!transaction)
			جاؤ cleanup;

		اگر (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			جاؤ failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		اگر (strmap_خالی_خانے(&failed_refs))
			جاؤ cleanup;

	failure:
		برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
			اگر (reported_error)
				cmd->error_string = reported_error;
			ورنہ اگر (strmap_شامل_ہے(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = تہی;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

ساکن خالی execute_commands_atomic(ساخت command *commands,
					ساخت shallow_info *si)
{
	ساخت command *cmd;
	ساخت strbuf err = STRBUF_INIT;
	ثابت حرف *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	اگر (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		جاؤ failure;
	}

	برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
		اگر (!should_process_cmd(cmd) || cmd->تسلسل_proc_receive)
			جاری;

		cmd->error_string = update(cmd, si);

		اگر (cmd->error_string)
			جاؤ failure;
	}

	اگر (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		جاؤ failure;
	}
	جاؤ cleanup;

failure:
	برائے (cmd = commands; cmd; cmd = cmd->اگلا)
		اگر (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

ساکن خالی execute_commands(ساخت command *commands,
			     صحیح unpacker_error,
			     ساخت shallow_info *si,
			     ساخت odb_transaction *transaction,
			     ثابت ساخت string_list *ڈالو_options)
{
	ساخت check_connected_options opt = CHECK_CONNECTED_INIT;
	ساخت command *cmd;
	ساخت iterate_ڈیٹا ڈیٹا;
	ساخت async muxer;
	صحیح err_fd = 0;
	صحیح تسلسل_proc_receive = 0;

	اگر (unpacker_error) {
		برائے (cmd = commands; cmd; cmd = cmd->اگلا)
			cmd->error_string = "unpacker error";
		واپس;
	}

	اگر (!skip_connectivity_check) {
		ساخت strvec env = STRVEC_INIT;

		اگر (use_sideband) {
			memset(&muxer, 0, حجم(muxer));
			muxer.proc = copy_مقصد_sideband;
			muxer.اندر = -1;
			اگر (!آغاز_async(&muxer))
				err_fd = muxer.اندر;
			/* ...else, continue without relaying sideband */
		}

		ڈیٹا.cmds = commands;
		ڈیٹا.si = si;
		opt.err_fd = err_fd;
		opt.پیش_رفت = err_fd && !quiet;
		odb_transaction_env(transaction, &env);
		opt.env = env.v;
		opt.exclude_hidden_refs_section = "receive";

		اگر (check_connected(iterate_receive_command_list, &ڈیٹا, &opt))
			سیٹ_connectivity_errors(commands, si, transaction);

		اگر (use_sideband)
			اختتام_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_مقصد_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	اگر (proc_receive_ref) {
		برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
			اگر (!should_process_cmd(cmd))
				جاری;

			اگر (proc_receive_ref_matches(cmd)) {
				cmd->تسلسل_proc_receive = تسلسل_PROC_RECEIVE_SCHEDULED;
				تسلسل_proc_receive = 1;
			}
		}
	}

	اگر (تسلسل_receive_hook(commands, "pre-receive", 0, transaction, ڈالو_options)) {
		برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
			اگر (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		واپس;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	برائے (cmd = commands; cmd && cmd->error_string; cmd = cmd->اگلا)
		; /* nothing */
	اگر (!cmd)
		واپس;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	اگر (odb_transaction_commit(transaction)) {
		برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
			اگر (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		واپس;
	}

	check_aliased_updates(commands);

	آزاد(سر_name_مقصد_free);
	سر_name = سر_name_مقصد_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, تہی,
							    تہی);

	اگر (تسلسل_proc_receive &&
	    تسلسل_proc_receive_hook(commands, ڈالو_options))
		برائے (cmd = commands; cmd; cmd = cmd->اگلا)
			اگر (!cmd->error_string &&
			    !(cmd->تسلسل_proc_receive & تسلسل_PROC_RECEIVE_RETURNED) &&
			    (cmd->تسلسل_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	اگر (use_atomic)
		execute_commands_atomic(commands, si);
	ورنہ
		execute_commands_non_atomic(commands, si);

	اگر (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

ساکن ساخت command **قطار_command(ساخت command **دم,
				      ثابت حرف *سطر,
				      صحیح linelen)
{
	ساخت object_id old_oid, new_oid;
	ساخت command *cmd;
	ثابت حرف *refname;
	صحیح reflen;
	ثابت حرف *p;

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
	*دم = cmd;
	واپس &cmd->اگلا;
}

ساکن خالی free_commands(ساخت command *commands)
{
	جب (commands) {
		ساخت command *اگلا = commands->اگلا;

		ref_ڈالو_report_free(commands->report);
		آزاد(commands->error_string_owned);
		آزاد(commands);
		commands = اگلا;
	}
}

ساکن خالی قطار_commands_from_cert(ساخت command **دم,
				     ساخت strbuf *ڈالو_cert)
{
	ثابت حرف *boc, *eoc;

	اگر (*دم)
		die("protocol error: got both push certificate and unsigned commands");

	boc = لڑی_تلاش(ڈالو_cert->buf, "\n\n");
	اگر (!boc)
		die("malformed push certificate %.*s", 100, ڈالو_cert->buf);
	ورنہ
		boc += 2;
	eoc = ڈالو_cert->buf + parse_signed_بفر(ڈالو_cert->buf, ڈالو_cert->len);

	جب (boc < eoc) {
		ثابت حرف *eol = memchr(boc, '\n', eoc - boc);
		دم = قطار_command(دم, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

ساکن ساخت command *read_سر_info(ساخت packet_reader *reader,
				      ساخت oid_array *shallow,
				      شمار report_حالت_نسخہ *نسخہ)
{
	ساخت command *commands = تہی;
	ساخت command **p = &commands;
	برائے (;;) {
		صحیح linelen;

		اگر (packet_reader_read(reader) != PACKET_READ_معمول)
			توڑو;

		اگر (reader->pktlen > 8 && starts_with(reader->سطر, "shallow ")) {
			ساخت object_id oid;
			اگر (get_oid_hex(reader->سطر + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->سطر + 8);
			oid_array_append(shallow, &oid);
			جاری;
		}

		linelen = لڑی_لمبائی(reader->سطر);
		اگر (linelen < reader->pktlen) {
			ثابت حرف *feature_list = reader->سطر + linelen + 1;
			ثابت حرف *hash = تہی;
			ثابت حرف *client_sid;
			سائز_t len = 0;
			اگر (parse_feature_request(feature_list, "report-status"))
				*نسخہ = REPORT_حالت_V0;
			اگر (parse_feature_request(feature_list, "report-status-v2"))
				*نسخہ = REPORT_حالت_V2;
			اگر (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			اگر (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			اگر (advertise_atomic_ڈالو
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			اگر (advertise_ڈالو_options
			    && parse_feature_request(feature_list, "push-options"))
				use_ڈالو_options = 1;
			hash = parse_feature_قدر(feature_list, "object-format", &len, تہی);
			اگر (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = لڑی_لمبائی(hash);
			}
			اگر (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_قدر(feature_list, "session-id", &len, تہی);
			اگر (client_sid) {
				حرف *sid = xstrndup(client_sid, len);
				trace2_ڈیٹا_string("transfer", تہی, "client-sid", client_sid);
				آزاد(sid);
			}
		}

		اگر (!strcmp(reader->سطر, "push-cert")) {
			صحیح true_flush = 0;
			صحیح saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			برائے (;;) {
				packet_reader_read(reader);
				اگر (reader->حالت_2 == PACKET_READ_FLUSH) {
					true_flush = 1;
					توڑو;
				}
				اگر (reader->حالت_2 != PACKET_READ_معمول) {
					die("protocol error: got an unexpected packet");
				}
				اگر (!strcmp(reader->سطر, "push-cert-end\n"))
					توڑو; /* end of cert */
				strbuf_addstr(&ڈالو_cert, reader->سطر);
			}
			reader->options = saved_options;

			اگر (true_flush)
				توڑو;
			جاری;
		}

		p = قطار_command(p, reader->سطر, linelen);
	}

	اگر (ڈالو_cert.len)
		قطار_commands_from_cert(p, &ڈالو_cert);

	واپس commands;
}

ساکن خالی read_ڈالو_options(ساخت packet_reader *reader,
			      ساخت string_list *options)
{
	جب (1) {
		اگر (packet_reader_read(reader) != PACKET_READ_معمول)
			توڑو;

		string_list_append(options, reader->سطر);
	}
}

ساکن صحیح unpack_with_sideband(ساخت odb_transaction *transaction,
				ثابت حرف *shallow_file,
				ساخت strbuf *err_پیغام)
{
	ساخت odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_پیغام_types = fsck_پیغام_types.buf,
		.max_داخلہ_سائز = max_داخلہ_سائز,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	ساخت async muxer;
	صحیح ret;

	اگر (!use_sideband)
		واپس odb_transaction_write_pack(transaction, 0, err_پیغام, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, حجم(muxer));
	muxer.proc = copy_مقصد_sideband;
	muxer.اندر = -1;
	اگر (آغاز_async(&muxer))
		واپس 0;

	opts.err_fd = muxer.اندر;
	ret = odb_transaction_write_pack(transaction, 0, err_پیغام, &opts);

	اختتام_async(&muxer);
	واپس ret;
}

ساکن خالی prepare_shallow_update(ساخت shallow_info *si,
				   ثابت حرف *shallow_file)
{
	صحیح i, j, k, bitmap_سائز = DIV_باری_اوپر(si->ref->nr, 32);

	ALLOC_ARRAY(si->مستعمل_shallow, si->shallow->nr);
	تفویض_shallow_commits_مقصد_refs(si, si->مستعمل_shallow, تہی);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	برائے (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	برائے (i = 0; i < si->shallow->nr; i++) {
		اگر (!si->مستعمل_shallow[i])
			جاری;
		برائے (j = 0; j < bitmap_سائز; j++) {
			اگر (!si->مستعمل_shallow[i][j])
				جاری;
			si->need_reachability_test[i]++;
			برائے (k = 0; k < 32; k++)
				اگر (si->مستعمل_shallow[i][j] & (1U << k))
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

ساکن خالی update_shallow_info(ساخت command *commands,
				ساخت shallow_info *si,
				ساخت oid_array *ref,
				ثابت حرف *shallow_file)
{
	ساخت command *cmd;
	صحیح *ref_حالت;
	remove_nonexistent_theirs_shallow(si);
	اگر (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		واپس;
	}

	برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
		اگر (is_null_oid(&cmd->new_oid))
			جاری;
		oid_array_append(ref, &cmd->new_oid);
		cmd->اشاریہ = ref->nr - 1;
	}
	si->ref = ref;

	اگر (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		واپس;
	}

	ALLOC_ARRAY(ref_حالت, ref->nr);
	تفویض_shallow_commits_مقصد_refs(si, تہی, ref_حالت);
	برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
		اگر (is_null_oid(&cmd->new_oid))
			جاری;
		اگر (ref_حالت[cmd->اشاریہ]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	آزاد(ref_حالت);
}

ساکن خالی override_cmds_error(ساخت command *commands, ثابت حرف *err)
{
	برائے (ساخت command *cmd = commands; cmd; cmd = cmd->اگلا) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
ساکن خالی generate_report(ساخت strbuf *buf, ساخت command *commands,
			    ثابت ساخت strbuf *unpack_حالت,
			    شمار report_حالت_نسخہ نسخہ)
{
	ساخت command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_حالت->len ? unpack_حالت->buf : "ok");

	برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
		ساخت ref_ڈالو_report *report;
		صحیح گنتی = 0;

		اگر (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		ورنہ
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		اگر (نسخہ != REPORT_حالت_V2 || cmd->error_string)
			جاری;

		برائے (report = cmd->report; report; report = report->اگلا) {
			اگر (گنتی++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			اگر (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			اگر (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_مقصد_hex(report->old_oid));
			اگر (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_مقصد_hex(report->new_oid));
			اگر (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

ساکن خالی report(ساخت command *commands, ثابت ساخت strbuf *unpack_حالت,
		   شمار report_حالت_نسخہ نسخہ)
{
	ساخت strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_حالت, نسخہ);

	اگر (تسلسل_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_حالت, نسخہ);
	}

	اگر (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	ورنہ
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

ساکن صحیح delete_only(ساخت command *commands)
{
	ساخت command *cmd;
	برائے (cmd = commands; cmd; cmd = cmd->اگلا) {
		اگر (!is_null_oid(&cmd->new_oid))
			واپس 0;
	}
	واپس 1;
}

صحیح cmd_receive_pack(صحیح argc,
		     ثابت حرف **argv,
		     ثابت حرف *سابقہ,
		     ساخت repository *repo غیر_مستعمل)
{
	صحیح advertise_refs = 0;
	ساخت command *commands;
	ساخت oid_array shallow = OID_ARRAY_INIT;
	ساخت oid_array ref = OID_ARRAY_INIT;
	ساخت shallow_info si;
	ساخت packet_reader reader;
	ساخت odb_transaction *transaction = تہی;
	شمار report_حالت_نسخہ نسخہ = REPORT_حالت_UNKNOWN;

	ساخت option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, تہی),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, تہی),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, تہی),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, تہی),
		OPT_انتہا()
	};

	packet_trace_شناختی_عنصر("receive-pack");

	argc = parse_options(argc, argv, سابقہ, options, receive_pack_usage, 0);

	اگر (argc > 1)
		usage_پیغام_opt(_("too many arguments"), receive_pack_usage, options);
	اگر (argc == 0)
		usage_پیغام_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	تیاری_راستہ();

	اگر (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, تہی);
	اگر (cert_nonce_بیج)
		ڈالو_cert_nonce = prepare_ڈالو_cert_nonce(service_dir, وقت(تہی));

	انتخاب (determine_protocol_نسخہ_server()) {
	حالت protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		توڑو;
	حالت protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		اگر (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	حالت protocol_v0:
		توڑو;
	حالت protocol_unknown_نسخہ:
		BUG("unknown protocol version");
	}

	اگر (advertise_refs || !stateless_rpc) {
		write_سر_info();
	}
	اگر (advertise_refs)
		واپس 0;

	packet_reader_init(&reader, 0, تہی, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	اگر ((commands = read_سر_info(&reader, &shallow, &نسخہ))) {
		ساخت string_list ڈالو_options = STRING_LIST_INIT_DUP;
		ساخت strbuf unpack_حالت = STRBUF_INIT;

		اگر (use_ڈالو_options)
			read_ڈالو_options(&reader, &ڈالو_options);
		اگر (!check_cert_ڈالو_options(&ڈالو_options)) {
			ساخت command *cmd;
			برائے (cmd = commands; cmd; cmd = cmd->اگلا)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		اگر (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		اگر (!delete_only(commands)) {
			ثابت حرف *alt_shallow_file = تہی;

			اگر (si.nr_ours || si.nr_theirs)
				alt_shallow_file = تیاری_temporary_shallow(si.shallow);

			اگر (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_حالت, "unable to start object transaction");
			ورنہ
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_حالت);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_حالت.len, &si, transaction,
				 &ڈالو_options);
		odb_transaction_finalize(transaction);
		sigchain_ڈالو(SIGPIPE, SIG_IGN);

		انتخاب (نسخہ) {
		حالت REPORT_حالت_V2:
		حالت REPORT_حالت_V0:
			report(commands, &unpack_حالت, نسخہ);
			توڑو;
		حالت REPORT_حالت_UNKNOWN:
			توڑو;
		}

		sigchain_نکالو(SIGPIPE);
		تسلسل_receive_hook(commands, "post-receive", 1, تہی,
				 &ڈالو_options);
		تسلسل_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&ڈالو_options, 0);
		اگر (auto_gc) {
			ساخت بچہ_process proc = بچہ_PROCESS_INIT;

			اگر (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_مقصد_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				اگر (!آغاز_command(&proc)) {
					اگر (use_sideband)
						copy_مقصد_sideband(proc.err, -1, تہی);
					اختتام_command(&proc);
				}
			}
		}
		اگر (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_حالت);
	}
	اگر (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	آزاد((خالی *)ڈالو_cert_nonce);
	واپس 0;
}
