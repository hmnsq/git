#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

تعداد report_حالت_نسخو {
	REPORT_حالت_UNKNOWN = 0,
	REPORT_حالت_V0,
	REPORT_حالت_V2,
};

ساكن صحيح deny_deletes;
ساكن صحيح deny_non_fast_forwards;
ساكن تعداد deny_action deny_current_branch = DENY_UNCONFIGURED;
ساكن تعداد deny_action deny_delete_current = DENY_UNCONFIGURED;
ساكن صحيح receive_fsck_objects = -1;
ساكن صحيح transfer_fsck_objects = -1;
ساكن بنية strbuf fsck_نياپو_types = STRBUF_INIT;
ساكن صحيح advertise_atomic_ڌڪو = 1;
ساكن صحيح advertise_ڌڪو_options;
ساكن صحيح advertise_sid;
ساكن off_t max_داخلا_سائيز;
ساكن صحيح use_sideband;
ساكن صحيح use_atomic;
ساكن صحيح use_ڌڪو_options;
ساكن صحيح quiet;
ساكن صحيح prefer_ofs_delta = 1;
ساكن صحيح auto_update_server_info;
ساكن صحيح auto_gc = 1;
ساكن صحيح reject_thin;
ساكن صحيح skip_connectivity_check;
ساكن صحيح stateless_rpc;
ساكن ثابت حرف *service_dir;
ساكن ثابت حرف *سر_name;
ساكن فراغ *سر_name_منزل_free;
ساكن صحيح موڪليل_capabilities;
ساكن صحيح shallow_update;
ساكن بنية strbuf ڌڪو_cert = STRBUF_INIT;
ساكن بنية object_id ڌڪو_cert_oid;
ساكن بنية signature_check sigcheck;
ساكن ثابت حرف *ڌڪو_cert_nonce;
ساكن حرف *cert_nonce_ٻج;
ساكن بنية strvec hidden_refs = STRVEC_INIT;

ساكن ثابت حرف *NONCE_UNSOLICITED = "UNSOLICITED";
ساكن ثابت حرف *NONCE_BAD = "BAD";
ساكن ثابت حرف *NONCE_MISSING = "MISSING";
ساكن ثابت حرف *NONCE_OK = "OK";
ساكن ثابت حرف *NONCE_SLOP = "SLOP";
ساكن ثابت حرف *nonce_حالت;
ساكن ڊگهو nonce_stamp_slop;
ساكن timestamp_t nonce_stamp_slop_حد_بندي;
ساكن بنية ref_transaction *transaction;

ساكن تعداد {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
ساكن صحيح keepalive_اندريون_sec = 5;

ساكن بنية proc_receive_ref {
	بنانشان صحيح want_جوڙيو:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	حرف *ref_اڳياڙي;
	بنية proc_receive_ref *ايندڙ;
} *proc_receive_ref;

ساكن فراغ proc_receive_ref_append(ثابت حرف *اڳياڙي);

ساكن تعداد deny_action parse_deny_action(ثابت حرف *var, ثابت حرف *قدر)
{
	إذا (قدر) {
		إذا (!strcasecmp(قدر, "ignore"))
			أرجع DENY_IGNORE;
		إذا (!strcasecmp(قدر, "warn"))
			أرجع DENY_WARN;
		إذا (!strcasecmp(قدر, "refuse"))
			أرجع DENY_REFUSE;
		إذا (!strcasecmp(قدر, "updateinstead"))
			أرجع DENY_UPDATE_INSTEAD;
	}
	إذا (git_config_bool(var, قدر))
		أرجع DENY_REFUSE;
	أرجع DENY_IGNORE;
}

ساكن صحيح receive_pack_config(ثابت حرف *var, ثابت حرف *قدر,
			       ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت حرف *نياپو_id;
	صحيح حالت = parse_hide_refs_config(var, قدر, "receive", &hidden_refs);

	إذا (حالت)
		أرجع حالت;

	إذا (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.fsck.skiplist") == 0) {
		حرف *رستو;

		إذا (git_config_pathname(&رستو, var, قدر))
			أرجع -1;
		إذا (رستو)
			strbuf_addf(&fsck_نياپو_types, "%cskiplist=%s",
				    fsck_نياپو_types.len ? ',' : '=', رستو);
		حرر(رستو);
		أرجع 0;
	}

	إذا (skip_اڳياڙي(var, "receive.fsck.", &نياپو_id)) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		إذا (is_valid_نياپو_type(نياپو_id, قدر))
			strbuf_addf(&fsck_نياپو_types, "%c%s=%s",
				fsck_نياپو_types.len ? ',' : '=', نياپو_id, قدر);
		وإلا
			warning("skipping unknown msg id '%s'", نياپو_id);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, قدر);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, قدر);
		أرجع 0;
	}

	إذا (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.certnonceseed") == 0)
		أرجع git_config_string(&cert_nonce_ٻج, var, قدر);

	إذا (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_حد_بندي = git_config_ulong(var, قدر, ctx->kvi);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_ڌڪو = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_ڌڪو_options = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.keepalive") == 0) {
		keepalive_اندريون_sec = git_config_int(var, قدر, ctx->kvi);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.maxinputsize") == 0) {
		max_داخلا_سائيز = git_config_int64(var, قدر, ctx->kvi);
		أرجع 0;
	}

	إذا (strcmp(var, "receive.procreceiverefs") == 0) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		proc_receive_ref_append(قدر);
		أرجع 0;
	}

	إذا (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, قدر);
		أرجع 0;
	}

	أرجع git_default_config(var, قدر, ctx, cb);
}

ساكن فراغ show_ref(ثابت حرف *رستو, ثابت بنية object_id *oid)
{
	إذا (موڪليل_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_منزل_hex(oid), رستو);
	} وإلا {
		بنية strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		إذا (advertise_atomic_ڌڪو)
			strbuf_addstr(&cap, " atomic");
		إذا (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		إذا (ڌڪو_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", ڌڪو_cert_nonce);
		إذا (advertise_ڌڪو_options)
			strbuf_addstr(&cap, " push-options");
		إذا (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_منزل_hex(oid), رستو, 0, cap.buf);
		strbuf_release(&cap);
		موڪليل_capabilities = 1;
	}
}

ساكن صحيح show_ref_cb(ثابت بنية reference *ref, فراغ *معلومات)
{
	بنية oidset *ڏٺل = معلومات;
	ثابت حرف *رستو = strip_namespace(ref->name);

	إذا (ref_is_hidden(رستو, ref->name, &hidden_refs))
		أرجع 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	إذا (!رستو) {
		إذا (oidset_وجهو(ڏٺل, ref->oid))
			أرجع 0;
		رستو = ".have";
	} وإلا {
		oidset_وجهو(ڏٺل, ref->oid);
	}
	show_ref(رستو, ref->oid);
	أرجع 0;
}

ساكن فراغ show_one_alternate_ref(ثابت بنية object_id *oid,
				   فراغ *معلومات)
{
	بنية oidset *ڏٺل = معلومات;

	إذا (oidset_وجهو(ڏٺل, oid))
		أرجع;

	show_ref(".have", oid);
}

ساكن فراغ write_سر_info(فراغ)
{
	بنية refs_for_each_ref_options opts = { 0 };
	ساكن بنية oidset ڏٺل = OIDSET_INIT;
	بنية strvec excludes_ويڪٽر = STRVEC_INIT;

	/*
	 * We need access to the reference names both with and without their
	 * namespace and thus cannot use `refs_for_each_namespaced_ref()`. We
	 * thus have to adapt exclude patterns to carry the namespace prefix
	 * ourselves.
	 */
	opts.exclude_patterns = get_namespaced_exclude_patterns(
		hidden_refs_منزل_excludes(&hidden_refs),
		get_git_namespace(), &excludes_ويڪٽر);

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      show_ref_cb, &ڏٺل, &opts);
	odb_for_each_alternate_ref(the_repository->objects,
				   show_one_alternate_ref, &ڏٺل);

	oidset_clear(&ڏٺل);
	strvec_clear(&excludes_ويڪٽر);

	إذا (!موڪليل_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#وصف_ڪريو لاڳيتو_PROC_RECEIVE_SCHEDULED	1
#وصف_ڪريو لاڳيتو_PROC_RECEIVE_RETURNED	2
بنية command {
	بنية command *ايندڙ;
	ثابت حرف *error_string;
	حرف *error_string_owned;
	بنية ref_ڌڪو_report *report;
	بنانشان صحيح skip_update:1,
		     did_not_exist:1,
		     لاڳيتو_proc_receive:2;
	صحيح اشاريو;
	بنية object_id old_oid;
	بنية object_id new_oid;
	حرف ref_name[FLEX_ARRAY]; /*وڌيڪ*/
};

ساكن فراغ proc_receive_ref_append(ثابت حرف *اڳياڙي)
{
	بنية proc_receive_ref *ref_نمونو;
	ثابت حرف *p;
	صحيح len;

	CALLOC_ARRAY(ref_نمونو, 1);
	p = strchr(اڳياڙي, ':');
	إذا (p) {
		طالما (اڳياڙي < p) {
			إذا (*اڳياڙي == 'a')
				ref_نمونو->want_جوڙيو = 1;
			وإلا إذا (*اڳياڙي == 'd')
				ref_نمونو->want_delete = 1;
			وإلا إذا (*اڳياڙي == 'm')
				ref_نمونو->want_modify = 1;
			وإلا إذا (*اڳياڙي == '!')
				ref_نمونو->negative_ref = 1;
			اڳياڙي++;
		}
		اڳياڙي++;
	} وإلا {
		ref_نمونو->want_جوڙيو = 1;
		ref_نمونو->want_delete = 1;
		ref_نمونو->want_modify = 1;
	}
	len = لڙي_ڊيگهه(اڳياڙي);
	طالما (len && اڳياڙي[len - 1] == '/')
		len--;
	ref_نمونو->ref_اڳياڙي = xmemdupz(اڳياڙي, len);
	إذا (!proc_receive_ref) {
		proc_receive_ref = ref_نمونو;
	} وإلا {
		بنية proc_receive_ref *پڇاڙي;

		پڇاڙي = proc_receive_ref;
		طالما (پڇاڙي->ايندڙ)
			پڇاڙي = پڇاڙي->ايندڙ;
		پڇاڙي->ايندڙ = ref_نمونو;
	}
}

ساكن صحيح proc_receive_ref_matches(بنية command *cmd)
{
	بنية proc_receive_ref *p;

	إذا (!proc_receive_ref)
		أرجع 0;

	لكل (p = proc_receive_ref; p; p = p->ايندڙ) {
		ثابت حرف *ميلاپ = p->ref_اڳياڙي;
		ثابت حرف *remains;

		إذا (!p->want_جوڙيو && is_null_oid(&cmd->old_oid))
			استمر;
		وإلا إذا (!p->want_delete && is_null_oid(&cmd->new_oid))
			استمر;
		وإلا إذا (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			استمر;

		إذا (skip_اڳياڙي(cmd->ref_name, ميلاپ, &remains) &&
		    (!*remains || *remains == '/')) {
			إذا (!p->negative_ref)
				أرجع 1;
		} وإلا إذا (p->negative_ref) {
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن فراغ report_پيغام(ثابت حرف *اڳياڙي, ثابت حرف *err, va_list params)
{
	صحيح sz;
	حرف نياپو[4096];

	sz = xsnprintf(نياپو, حجم(نياپو), "%s", اڳياڙي);
	sz += vsnprintf(نياپو + sz, حجم(نياپو) - sz, err, params);
	إذا (sz > (حجم(نياپو) - 1))
		sz = حجم(نياپو) - 1;
	نياپو[sz++] = '\n';

	إذا (use_sideband)
		send_sideband(1, 2, نياپو, sz, use_sideband);
	وإلا
		xwrite(2, نياپو, sz);
}

__attribute__((format (لکو, 1, 2)))
ساكن فراغ rp_warning(ثابت حرف *err, ...)
{
	va_list params;
	va_شروعات(params, err);
	report_پيغام("warning: ", err, params);
	va_پڇاڙي(params);
}

__attribute__((format (لکو, 1, 2)))
ساكن فراغ rp_error(ثابت حرف *err, ...)
{
	va_list params;
	va_شروعات(params, err);
	report_پيغام("error: ", err, params);
	va_پڇاڙي(params);
}

ساكن صحيح copy_منزل_sideband(صحيح اندريون, صحيح ٻاهريون اڻ_استعمال_ٿيل, فراغ *arg اڻ_استعمال_ٿيل)
{
	حرف معلومات[128];
	صحيح keepalive_سرگرم = 0;

	إذا (keepalive_اندريون_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	إذا (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_سرگرم = 1;

	طالما (1) {
		ssize_t sz;

		إذا (keepalive_سرگرم) {
			بنية pollfd pfd;
			صحيح ret;

			pfd.fd = اندريون;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_اندريون_sec);

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

		sz = xread(اندريون, معلومات, حجم(معلومات));
		إذا (sz <= 0)
			اكسر;

		إذا (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_سرگرم) {
			ثابت حرف *p = memchr(معلومات, '\0', sz);
			إذا (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_سرگرم = 1;
				send_sideband(1, 2, معلومات, p - معلومات, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - معلومات + 1), use_sideband);
				استمر;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, معلومات, sz, use_sideband);
	}
	close(اندريون);
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
	sideband_async->proc = copy_منزل_sideband;
	sideband_async->اندريون = -1;

	إذا (!شروعات_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		إذا (*saved_stderr >= 0)
			dup2(sideband_async->اندريون, STDERR_FILENO);
		close(sideband_async->اندريون);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
ساكن فراغ پڄاڻي_sideband_async(بنية async *sideband_async, صحيح saved_stderr, صحيح started)
{
	إذا (!use_sideband)
		أرجع;

	إذا (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	إذا (started)
		پڄاڻي_async(sideband_async);
}

ساكن فراغ hmac_hash(بنانشان حرف *ٻاهريون,
		      ثابت حرف *ڪنجي_اندريون, سائيز_t ڪنجي_len,
		      ثابت حرف *متن, سائيز_t متن_len)
{
	بنانشان حرف ڪنجي[GIT_MAX_BLKSZ];
	بنانشان حرف k_ipad[GIT_MAX_BLKSZ];
	بنانشان حرف k_opad[GIT_MAX_BLKSZ];
	صحيح i;
	بنية git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(ڪنجي, '\0', GIT_MAX_BLKSZ);
	إذا (the_hash_algo->blksz < ڪنجي_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, ڪنجي_اندريون, ڪنجي_len);
		git_hash_final(ڪنجي, &ctx);
	} وإلا {
		يادگيري_نقل(ڪنجي, ڪنجي_اندريون, ڪنجي_len);
	}

	/* RFC 2104 2. (2) & (5) */
	لكل (i = 0; i < حجم(ڪنجي); i++) {
		k_ipad[i] = ڪنجي[i] ^ 0x36;
		k_opad[i] = ڪنجي[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, حجم(k_ipad));
	git_hash_update(&ctx, متن, متن_len);
	git_hash_final(ٻاهريون, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, حجم(k_opad));
	git_hash_update(&ctx, ٻاهريون, the_hash_algo->rawsz);
	git_hash_final(ٻاهريون, &ctx);
}

ساكن حرف *prepare_ڌڪو_cert_nonce(ثابت حرف *رستو, timestamp_t stamp)
{
	بنية strbuf buf = STRBUF_INIT;
	بنانشان حرف hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, رستو, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_ٻج, لڙي_ڊيگهه(cert_nonce_ٻج));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (صحيح)the_hash_algo->hexsz, hash_منزل_hex(hash));
	أرجع strbuf_detach(&buf, عدم);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
ساكن صحيح constant_memequal(ثابت حرف *a, ثابت حرف *b, سائيز_t n)
{
	صحيح res = 0;
	سائيز_t i;

	لكل (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	أرجع res;
}

ساكن ثابت حرف *check_nonce(ثابت حرف *buf)
{
	سائيز_t noncelen;
	ثابت حرف *مليو_2 = ڳوليو_commit_سرو(buf, "nonce", &noncelen);
	حرف *nonce = مليو_2 ? xmemdupz(مليو_2, noncelen) : عدم;
	timestamp_t stamp, ostamp;
	حرف *bohmac, *expect = عدم;
	ثابت حرف *retval = NONCE_BAD;

	إذا (!nonce) {
		retval = NONCE_MISSING;
		اذهب leave;
	} وإلا إذا (!ڌڪو_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		اذهب leave;
	} وإلا إذا (!strcmp(ڌڪو_cert_nonce, nonce)) {
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

	expect = prepare_ڌڪو_cert_nonce(service_dir, stamp);
	إذا (noncelen != لڙي_ڊيگهه(expect)) {
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
	ostamp = parse_timestamp(ڌڪو_cert_nonce, عدم, 10);
	nonce_stamp_slop = (ڊگهو)ostamp - (ڊگهو)stamp;

	إذا (nonce_stamp_slop_حد_بندي &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_حد_بندي) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		حرر((فراغ *)ڌڪو_cert_nonce);
		ڌڪو_cert_nonce = xstrdup(nonce);
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
ساكن صحيح check_cert_ڌڪو_options(ثابت بنية string_list *ڌڪو_options)
{
	ثابت حرف *buf = ڌڪو_cert.buf;

	ثابت حرف *option;
	سائيز_t optionlen;
	صحيح options_ڏٺل = 0;

	صحيح retval = 1;

	إذا (!*buf)
		أرجع 1;

	طالما ((option = ڳوليو_commit_سرو(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_ڏٺل++;
		إذا (options_ڏٺل > ڌڪو_options->nr
		    || xstrncmpz(ڌڪو_options->شيون[options_ڏٺل - 1].string,
				 option, optionlen))
			أرجع 0;
	}

	إذا (options_ڏٺل != ڌڪو_options->nr)
		retval = 0;

	أرجع retval;
}

ساكن فراغ prepare_ڌڪو_cert_sha1(بنية لاڳيتو_hooks_opt *opt)
{
	ساكن صحيح already_پورو;

	إذا (!ڌڪو_cert.len)
		أرجع;

	إذا (!already_پورو) {
		صحيح bogs /* beginning_of_gpg_sig */;

		already_پورو = 1;
		إذا (odb_write_object(the_repository->objects, ڌڪو_cert.buf,
				     ڌڪو_cert.len, OBJ_BLOB, &ڌڪو_cert_oid))
			oidclr(&ڌڪو_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', حجم(sigcheck));

		bogs = parse_signed_بفر(ڌڪو_cert.buf, ڌڪو_cert.len);
		sigcheck.payload = xmemdupz(ڌڪو_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, ڌڪو_cert.buf + bogs,
				ڌڪو_cert.len - bogs);

		nonce_حالت = check_nonce(sigcheck.payload);
	}
	إذا (!is_null_oid(&ڌڪو_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_منزل_hex(&ڌڪو_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.ڪنجي ? sigcheck.ڪنجي : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.نتيجو);
		إذا (ڌڪو_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     ڌڪو_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_حالت);
			إذا (nonce_حالت == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

بنية receive_hook_feed_state {
	بنية command *cmd;
	بنية ref_ڌڪو_report *report;
	صحيح skip_broken;
	بنية strbuf buf;
};

ساكن صحيح feed_receive_hook_cb(صحيح hook_stdin_fd, فراغ *pp_cb اڻ_استعمال_ٿيل, فراغ *pp_task_cb)
{
	بنية receive_hook_feed_state *state = pp_task_cb;
	بنية command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	طالما (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->ايندڙ;

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
			    oid_منزل_hex(old_oid), oid_منزل_hex(new_oid),
			    ref_name);

		state->report = state->report->ايندڙ;
		إذا (!state->report)
			cmd = cmd->ايندڙ;
	} وإلا {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_منزل_hex(&cmd->old_oid), oid_منزل_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->ايندڙ;
	}

	state->cmd = cmd;

	إذا (state->buf.len > 0) {
		صحيح ret = write_اندريون_ڀريل(hook_stdin_fd, state->buf.buf, state->buf.len);
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
	بنية receive_hook_feed_state *معلومات;

	CALLOC_ARRAY(معلومات, 1);
	معلومات->report = init_state->report;
	معلومات->cmd = init_state->cmd;
	معلومات->skip_broken = init_state->skip_broken;
	strbuf_init(&معلومات->buf, 0);

	أرجع معلومات;
}

ساكن فراغ receive_hook_feed_state_free(فراغ *معلومات)
{
	بنية receive_hook_feed_state *d = معلومات;
	إذا (!d)
		أرجع;
	strbuf_release(&d->buf);
	حرر(d);
}

ساكن صحيح لاڳيتو_receive_hook(بنية command *commands,
			    ثابت حرف *hook_name,
			    صحيح skip_broken,
			    بنية odb_transaction *transaction,
			    ثابت بنية string_list *ڌڪو_options)
{
	بنية لاڳيتو_hooks_opt opt = لاڳيتو_HOOKS_OPT_INIT;
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
		iter = iter->ايندڙ;
	إذا (!iter)
		أرجع 0;

	إذا (ڌڪو_options) {
		لكل (صحيح i = 0; i < ڌڪو_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     ڌڪو_options->شيون[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)ڌڪو_options->nr);
	} وإلا {
		strvec_ڌڪو(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	إذا (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_ڌڪو_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_معلومات_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_معلومات_free = receive_hook_feed_state_free;

	ret = لاڳيتو_hooks_opt(the_repository, hook_name, &opt);

	پڄاڻي_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	أرجع ret;
}

ساكن صحيح لاڳيتو_update_hook(بنية command *cmd)
{
	ساكن ثابت حرف hook_name[] = "update";
	بنية لاڳيتو_hooks_opt opt = لاڳيتو_HOOKS_OPT_INIT;
	بنية async sideband_async;
	صحيح sideband_async_started = 0;
	صحيح saved_stderr = -1;
	صحيح ڪوڊ;

	إذا (!hook_exists(the_repository, hook_name))
		أرجع 0;

	strvec_pushl(&opt.دليلون,
		     cmd->ref_name,
		     oid_منزل_hex(&cmd->old_oid),
		     oid_منزل_hex(&cmd->new_oid),
		     عدم);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	ڪوڊ = لاڳيتو_hooks_opt(the_repository, hook_name, &opt);

	پڄاڻي_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	أرجع ڪوڊ;
}

ساكن صحيح لاڳيتو_receive_report_hook(بنية strbuf *report)
{
	بنية ٻار_process proc = ٻار_PROCESS_INIT;
	بنية async sideband_async;
	صحيح sideband_async_started = 0;
	صحيح saved_stderr = -1;
	بنية strbuf ٻاهريون = STRBUF_INIT;
	ثابت حرف *hook_رستو;
	صحيح ret;

	hook_رستو = ڳوليو_hook(the_repository, "receive-report");
	إذا (!hook_رستو)
		أرجع 0;

	strvec_ڌڪو(&proc.دليلون, hook_رستو);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_ڌڪو(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &ٻاهريون,
			   report->len, عدم, 0);
	sigchain_ڪڍو(SIGPIPE);

	پڄاڻي_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	إذا (!ret)
		strbuf_مٽا_سٽا(&ٻاهريون, report);

	strbuf_release(&ٻاهريون);
	أرجع ret;
}

ساكن بنية command *ڳوليو_command_by_refname(بنية command *list,
					       ثابت حرف *refname)
{
	لكل (; list; list = list->ايندڙ)
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
	بنية ref_ڌڪو_report *report = عدم;
	صحيح new_report = 0;
	صحيح ڪوڊ = 0;
	صحيح once = 0;
	صحيح response = 0;

	لكل (;;) {
		بنية object_id old_oid, new_oid;
		حرف *سر;
		حرف *refname;
		حرف *p;
		تعداد packet_read_حالت حالت;

		حالت = packet_reader_read(reader);
		إذا (حالت != PACKET_READ_عام) {
			/* Check whether proc-receive exited abnormally */
			إذا (حالت == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				أرجع -1;
			}
			اكسر;
		}
		response++;

		سر = reader->سٽ;
		p = strchr(سر, ' ');
		إذا (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", سر);
			ڪوڊ = -1;
			استمر;
		}
		*p++ = '\0';
		إذا (!strcmp(سر, "option")) {
			حرف *ڪنجي;
			ثابت حرف *val;

			إذا (!hint || !(report || new_report)) {
				إذا (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				ڪوڊ = -1;
				استمر;
			}
			إذا (new_report) {
				إذا (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} وإلا {
					report = hint->report;
					طالما (report->ايندڙ)
						report = report->ايندڙ;
					report->ايندڙ = xcalloc(1, حجم(بنية ref_ڌڪو_report));
					report = report->ايندڙ;
				}
				new_report = 0;
			}
			ڪنجي = p;
			p = strchr(ڪنجي, ' ');
			إذا (p)
				*p++ = '\0';
			val = p;
			إذا (!strcmp(ڪنجي, "refname"))
				report->ref_name = xstrdup_or_null(val);
			وإلا إذا (!strcmp(ڪنجي, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			وإلا إذا (!strcmp(ڪنجي, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			وإلا إذا (!strcmp(ڪنجي, "forced-update"))
				report->forced_update = 1;
			وإلا إذا (!strcmp(ڪنجي, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->لاڳيتو_proc_receive = 0;
			استمر;
		}

		report = عدم;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		إذا (p)
			*p++ = '\0';
		إذا (strcmp(سر, "ok") && strcmp(سر, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    سر, refname);
			ڪوڊ = -1;
			استمر;
		}

		/* first try searching at our hint, falling back to all refs */
		إذا (hint)
			hint = ڳوليو_command_by_refname(hint, refname);
		إذا (!hint)
			hint = ڳوليو_command_by_refname(commands, refname);
		إذا (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			ڪوڊ = -1;
			استمر;
		}
		إذا (!hint->لاڳيتو_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			ڪوڊ = -1;
			استمر;
		}
		hint->لاڳيتو_proc_receive |= لاڳيتو_PROC_RECEIVE_RETURNED;
		إذا (!strcmp(سر, "ng")) {
			إذا (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			وإلا
				hint->error_string = "failed";
			ڪوڊ = -1;
			استمر;
		}
		new_report = 1;
	}

	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ)
		إذا (cmd->لاڳيتو_proc_receive && !cmd->error_string &&
		    !(cmd->لاڳيتو_proc_receive & لاڳيتو_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    ڪوڊ = -1;
		}
	أرجع ڪوڊ;
}

ساكن صحيح لاڳيتو_proc_receive_hook(بنية command *commands,
				 ثابت بنية string_list *ڌڪو_options)
{
	بنية ٻار_process proc = ٻار_PROCESS_INIT;
	بنية async muxer;
	بنية command *cmd;
	بنية packet_reader reader;
	بنية strbuf cap = STRBUF_INIT;
	بنية strbuf errmsg = STRBUF_INIT;
	صحيح hook_use_ڌڪو_options = 0;
	صحيح نسخو = 0;
	صحيح ڪوڊ;
	ثابت حرف *hook_رستو = ڳوليو_hook(the_repository, "proc-receive");

	إذا (!hook_رستو) {
		rp_error("cannot find hook 'proc-receive'");
		أرجع -1;
	}

	strvec_ڌڪو(&proc.دليلون, hook_رستو);
	proc.اندريون = -1;
	proc.ٻاهريون = -1;
	proc.trace2_hook_name = "proc-receive";

	إذا (use_sideband) {
		memset(&muxer, 0, حجم(muxer));
		muxer.proc = copy_منزل_sideband;
		muxer.اندريون = -1;
		ڪوڊ = شروعات_async(&muxer);
		إذا (ڪوڊ)
			أرجع ڪوڊ;
		proc.err = muxer.اندريون;
	} وإلا {
		proc.err = 0;
	}

	ڪوڊ = شروعات_command(&proc);
	إذا (ڪوڊ) {
		إذا (use_sideband)
			پڄاڻي_async(&muxer);
		أرجع ڪوڊ;
	}

	sigchain_ڌڪو(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.ٻاهريون, عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	إذا (use_atomic)
		strbuf_addstr(&cap, " atomic");
	إذا (use_ڌڪو_options)
		strbuf_addstr(&cap, " push-options");
	إذا (cap.len) {
		ڪوڊ = packet_write_fmt_gently(proc.اندريون, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} وإلا {
		ڪوڊ = packet_write_fmt_gently(proc.اندريون, "version=1\n");
	}
	إذا (!ڪوڊ)
		ڪوڊ = packet_flush_gently(proc.اندريون);

	إذا (!ڪوڊ)
		لكل (;;) {
			صحيح linelen;
			تعداد packet_read_حالت حالت;

			حالت = packet_reader_read(&reader);
			إذا (حالت != PACKET_READ_عام) {
				/* Check whether proc-receive exited abnormally */
				إذا (حالت == PACKET_READ_EOF)
					ڪوڊ = -1;
				اكسر;
			}

			إذا (reader.pktlen > 8 && starts_with(reader.سٽ, "version=")) {
				نسخو = atoi(reader.سٽ + 8);
				linelen = لڙي_ڊيگهه(reader.سٽ);
				إذا (linelen < reader.pktlen) {
					ثابت حرف *feature_list = reader.سٽ + linelen + 1;
					إذا (parse_feature_request(feature_list, "push-options"))
						hook_use_ڌڪو_options = 1;
				}
			}
		}

	إذا (ڪوڊ) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		اذهب cleanup;
	}

	بدل (نسخو) {
	حالة 0:
		/* fallthrough */
	حالة 1:
		اكسر;
	افتراضي:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    نسخو);
		ڪوڊ = -1;
		اذهب cleanup;
	}

	/* Send commands */
	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
		إذا (!cmd->لاڳيتو_proc_receive || cmd->skip_update || cmd->error_string)
			استمر;
		ڪوڊ = packet_write_fmt_gently(proc.اندريون, "%s %s %s",
					       oid_منزل_hex(&cmd->old_oid),
					       oid_منزل_hex(&cmd->new_oid),
					       cmd->ref_name);
		إذا (ڪوڊ)
			اكسر;
	}
	إذا (!ڪوڊ)
		ڪوڊ = packet_flush_gently(proc.اندريون);
	إذا (ڪوڊ) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		اذهب cleanup;
	}

	/* Send push options */
	إذا (hook_use_ڌڪو_options) {
		بنية string_list_شي *شي;

		for_each_string_list_شي(شي, ڌڪو_options) {
			ڪوڊ = packet_write_fmt_gently(proc.اندريون, "%s", شي->string);
			إذا (ڪوڊ)
				اكسر;
		}
		إذا (!ڪوڊ)
			ڪوڊ = packet_flush_gently(proc.اندريون);
		إذا (ڪوڊ) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			اذهب cleanup;
		}
	}

	/* Read result from proc-receive */
	ڪوڊ = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.اندريون);
	close(proc.ٻاهريون);
	إذا (use_sideband)
		پڄاڻي_async(&muxer);
	إذا (پڄاڻي_command(&proc))
		ڪوڊ = -1;
	إذا (errmsg.len >0) {
		حرف *p = errmsg.buf;

		p += errmsg.len - 1;
		إذا (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_ڪڍو(SIGPIPE);

	أرجع ڪوڊ;
}

ساكن ثابت حرف *refuse_unconfigured_deny_نياپو =
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
	rp_error("%s", _(refuse_unconfigured_deny_نياپو));
}

ساكن ثابت حرف *refuse_unconfigured_deny_delete_current_نياپو =
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
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_نياپو));
}

ساكن ثابت بنية object_id *command_singleton_iterator(فراغ *cb_معلومات);
ساكن صحيح update_shallow_ref(بنية command *cmd, بنية shallow_info *si)
{
	بنية shallow_تالو shallow_تالو = SHALLOW_تالو_INIT;
	بنية oid_array extra = OID_ARRAY_INIT;
	بنية check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t نقاب = 1 << (cmd->اشاريو % 32);
	صحيح i;

	trace_printf_ڪنجي(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	لكل (i = 0; i < si->shallow->nr; i++)
		إذا (si->استعمال_ٿيل_shallow[i] &&
		    (si->استعمال_ٿيل_shallow[i][cmd->اشاريو / 32] & نقاب) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	تياري_alternate_shallow(&shallow_تالو, &opt.shallow_file, &extra);
	إذا (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_تالو);
		oid_array_clear(&extra);
		أرجع -1;
	}

	commit_shallow_file(the_repository, &shallow_تالو);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	لكل (i = 0; i < extra.nr; i++)
		رجسٽر_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->اشاريو] = 0;
	oid_array_clear(&extra);
	أرجع 0;
}

ساكن ثابت حرف *ڌڪو_منزل_deploy(بنانشان حرف *sha1,
				  بنية strvec *env,
				  ثابت بنية worktree *worktree)
{
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;

	strvec_pushl(&ٻار.دليلون, "update-index", "-q", "--ignore-submodules",
		     "--refresh", عدم);
	strvec_pushv(&ٻار.env, env->v);
	ٻار.dir = worktree->رستو;
	ٻار.no_stdin = 1;
	ٻار.stdout_منزل_stderr = 1;
	ٻار.git_cmd = 1;
	إذا (لاڳيتو_command(&ٻار))
		أرجع "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	ٻار_process_init(&ٻار);
	strvec_pushl(&ٻار.دليلون, "diff-files", "--quiet",
		     "--ignore-submodules", "--", عدم);
	strvec_pushv(&ٻار.env, env->v);
	ٻار.dir = worktree->رستو;
	ٻار.no_stdin = 1;
	ٻار.stdout_منزل_stderr = 1;
	ٻار.git_cmd = 1;
	إذا (لاڳيتو_command(&ٻار))
		أرجع "Working directory has unstaged changes";

	ٻار_process_init(&ٻار);
	strvec_pushl(&ٻار.دليلون, "diff-index", "--quiet", "--cached",
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
		     خالي_جايون_وڻ_oid_hex(the_repository->hash_algo), "--", عدم);
	strvec_pushv(&ٻار.env, env->v);
	ٻار.no_stdin = 1;
	ٻار.no_stdout = 1;
	ٻار.stdout_منزل_stderr = 0;
	ٻار.git_cmd = 1;
	إذا (لاڳيتو_command(&ٻار))
		أرجع "Working directory has staged changes";

	ٻار_process_init(&ٻار);
	strvec_pushl(&ٻار.دليلون, "read-tree", "-u", "-m", hash_منزل_hex(sha1),
		     عدم);
	strvec_pushv(&ٻار.env, env->v);
	ٻار.dir = worktree->رستو;
	ٻار.no_stdin = 1;
	ٻار.no_stdout = 1;
	ٻار.stdout_منزل_stderr = 0;
	ٻار.git_cmd = 1;
	إذا (لاڳيتو_command(&ٻار))
		أرجع "Could not update working tree to new HEAD";

	أرجع عدم;
}

ساكن ثابت حرف *ڌڪو_منزل_checkout_hook = "push-to-checkout";

ساكن ثابت حرف *ڌڪو_منزل_checkout(بنانشان حرف *hash,
				    صحيح *invoked_hook,
				    بنية strvec *env,
				    ثابت حرف *work_وڻ)
{
	بنية لاڳيتو_hooks_opt opt = لاڳيتو_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_رستو(work_وڻ));
	strvec_ڌڪو(&opt.دليلون, hash_منزل_hex(hash));
	إذا (لاڳيتو_hooks_opt(the_repository, ڌڪو_منزل_checkout_hook, &opt))
		أرجع "push-to-checkout hook declined";
	وإلا
		أرجع عدم;
}

ساكن ثابت حرف *update_worktree(بنانشان حرف *sha1, ثابت بنية worktree *worktree)
{
	ثابت حرف *retval;
	حرف *git_dir;
	بنية strvec env = STRVEC_INIT;
	صحيح invoked_hook;

	إذا (!worktree || !worktree->رستو)
		BUG("worktree->path must be non-NULL");

	إذا (worktree->is_bare)
		أرجع "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_رستو(git_dir));

	retval = ڌڪو_منزل_checkout(sha1, &invoked_hook, &env, worktree->رستو);
	إذا (!invoked_hook)
		retval = ڌڪو_منزل_deploy(sha1, &env, worktree);

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
		ڳوليو_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	إذا (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		اذهب ٻاهريون;
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
			اذهب ٻاهريون;
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
		      "but I can't find it!", oid_منزل_hex(new_oid));
		ret = "bad pack";
		اذهب ٻاهريون;
	}

	إذا (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		إذا (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			اذهب ٻاهريون;
		}

		إذا (worktree || (سر_name && !strcmp(namespaced_name, سر_name))) {
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
				اذهب ٻاهريون;
			افتراضي:
				ret = "Invalid denyDeleteCurrent setting";
				اذهب ٻاهريون;
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
			اذهب ٻاهريون;
		}
		old_commit = (بنية commit *)old_object;
		new_commit = (بنية commit *)new_object;
		ret2 = repo_اندريون_ملايو_bases(the_repository, old_commit, new_commit);
		إذا (ret2 < 0)
			ٻاهر_نڪرو(128);
		إذا (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			اذهب ٻاهريون;
		}
	}
	إذا (لاڳيتو_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		اذهب ٻاهريون;
	}

	إذا (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		إذا (ret)
			اذهب ٻاهريون;
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
		إذا (shallow_update && si->shallow_ref[cmd->اشاريو] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			اذهب ٻاهريون;
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
				ret = ref_transaction_error_نياپو(tx_err);
		} وإلا {
			ret = عدم; /* good */
		}
		strbuf_release(&err);
	}

ٻاهريون:
	free_worktrees(worktrees);
	أرجع ret;
}

ساكن فراغ لاڳيتو_update_post_hook(بنية command *commands)
{
	ساكن ثابت حرف hook_name[] = "post-update";
	بنية لاڳيتو_hooks_opt opt = لاڳيتو_HOOKS_OPT_INIT;
	بنية async sideband_async;
	بنية command *cmd;
	صحيح sideband_async_started = 0;
	صحيح saved_stderr = -1;

	إذا (!hook_exists(the_repository, hook_name))
		أرجع;

	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
		إذا (cmd->error_string || cmd->did_not_exist)
			استمر;
		strvec_ڌڪو(&opt.دليلون, cmd->ref_name);
	}
	إذا (!opt.دليلون.nr)
		أرجع;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	لاڳيتو_hooks_opt(the_repository, hook_name, &opt);

	پڄاڻي_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

ساكن فراغ check_aliased_update_اندروني(بنية command *cmd,
					  بنية string_list *list,
					  ثابت حرف *dst_name, صحيح flag)
{
	بنية string_list_شي *شي;
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

	إذا (!(شي = string_list_lookup(list, dst_name)))
		أرجع;

	cmd->skip_update = 1;

	dst_cmd = (بنية command *) شي->util;

	إذا (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		أرجع;

	dst_cmd->skip_update = 1;

	rp_error("refusing inconsistent update between symref '%s' (%s..%s) and"
		 " its target '%s' (%s..%s)",
		 cmd->ref_name,
		 repo_ڳوليو_unique_abbrev(the_repository, &cmd->old_oid, DEFAULT_ABBREV),
		 repo_ڳوليو_unique_abbrev(the_repository, &cmd->new_oid, DEFAULT_ABBREV),
		 dst_cmd->ref_name,
		 repo_ڳوليو_unique_abbrev(the_repository, &dst_cmd->old_oid, DEFAULT_ABBREV),
		 repo_ڳوليو_unique_abbrev(the_repository, &dst_cmd->new_oid, DEFAULT_ABBREV));

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
	check_aliased_update_اندروني(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

ساكن فراغ check_aliased_updates(بنية command *commands)
{
	بنية command *cmd;
	بنية string_list ref_list = STRING_LIST_INIT_NODUP;

	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
		بنية string_list_شي *شي =
			string_list_append(&ref_list, cmd->ref_name);
		شي->util = (فراغ *)cmd;
	}
	string_list_ترتيب_ڏيو(&ref_list);

	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
		إذا (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

ساكن ثابت بنية object_id *command_singleton_iterator(فراغ *cb_معلومات)
{
	بنية command **cmd_list = cb_معلومات;
	بنية command *cmd = *cmd_list;

	إذا (!cmd || is_null_oid(&cmd->new_oid))
		أرجع عدم;
	*cmd_list = عدم; /* this returns only one */
	أرجع &cmd->new_oid;
}

ساكن فراغ سيٽ_connectivity_errors(بنية command *commands,
				    بنية shallow_info *si,
				    بنية odb_transaction *transaction)
{
	بنية command *cmd;

	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
		بنية command *singleton = cmd;
		بنية check_connected_options opt = CHECK_CONNECTED_INIT;
		بنية strvec env = STRVEC_INIT;

		إذا (shallow_update && si->shallow_ref[cmd->اشاريو])
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

بنية iterate_معلومات {
	بنية command *cmds;
	بنية shallow_info *si;
};

ساكن ثابت بنية object_id *iterate_receive_command_list(فراغ *cb_معلومات)
{
	بنية iterate_معلومات *معلومات = cb_معلومات;
	بنية command **cmd_list = &معلومات->cmds;
	بنية command *cmd = *cmd_list;

	لكل (; cmd; cmd = cmd->ايندڙ) {
		إذا (shallow_update && معلومات->si->shallow_ref[cmd->اشاريو])
			/* to be checked in update_shallow_ref() */
			استمر;
		إذا (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->ايندڙ;
			أرجع &cmd->new_oid;
		}
	}
	أرجع عدم;
}

ساكن فراغ reject_updates_منزل_hidden(بنية command *commands)
{
	بنية strbuf refname_ڀريل = STRBUF_INIT;
	سائيز_t اڳياڙي_len;
	بنية command *cmd;

	strbuf_addstr(&refname_ڀريل, get_git_namespace());
	اڳياڙي_len = refname_ڀريل.len;

	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
		إذا (cmd->error_string)
			استمر;

		strbuf_setlen(&refname_ڀريل, اڳياڙي_len);
		strbuf_addstr(&refname_ڀريل, cmd->ref_name);

		إذا (!ref_is_hidden(cmd->ref_name, refname_ڀريل.buf, &hidden_refs))
			استمر;
		إذا (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		وإلا
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_ڀريل);
}

ساكن صحيح should_process_cmd(بنية command *cmd)
{
	أرجع !cmd->error_string && !cmd->skip_update;
}

ساكن فراغ BUG_if_skipped_connectivity_check(بنية command *commands,
					       بنية shallow_info *si)
{
	بنية command *cmd;

	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
		إذا (should_process_cmd(cmd) && si->shallow_ref[cmd->اشاريو])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

ساكن فراغ ref_transaction_rejection_handler(ثابت حرف *refname,
					      ثابت بنية object_id *old_oid اڻ_استعمال_ٿيل,
					      ثابت بنية object_id *new_oid اڻ_استعمال_ٿيل,
					      ثابت حرف *old_هدف اڻ_استعمال_ٿيل,
					      ثابت حرف *new_هدف اڻ_استعمال_ٿيل,
					      تعداد ref_transaction_error err,
					      ثابت حرف *details,
					      فراغ *cb_معلومات)
{
	بنية strmap *failed_refs = cb_معلومات;

	إذا (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (حرف *)ref_transaction_error_نياپو(err));
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
		لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
			إذا (!should_process_cmd(cmd) || cmd->لاڳيتو_proc_receive)
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

		إذا (strmap_خالي_جايون(&failed_refs))
			اذهب cleanup;

	failure:
		لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
			إذا (reported_error)
				cmd->error_string = reported_error;
			وإلا إذا (strmap_شامل_آهي(&failed_refs, cmd->ref_name))
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

	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
		إذا (!should_process_cmd(cmd) || cmd->لاڳيتو_proc_receive)
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
	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ)
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
			     ثابت بنية string_list *ڌڪو_options)
{
	بنية check_connected_options opt = CHECK_CONNECTED_INIT;
	بنية command *cmd;
	بنية iterate_معلومات معلومات;
	بنية async muxer;
	صحيح err_fd = 0;
	صحيح لاڳيتو_proc_receive = 0;

	إذا (unpacker_error) {
		لكل (cmd = commands; cmd; cmd = cmd->ايندڙ)
			cmd->error_string = "unpacker error";
		أرجع;
	}

	إذا (!skip_connectivity_check) {
		بنية strvec env = STRVEC_INIT;

		إذا (use_sideband) {
			memset(&muxer, 0, حجم(muxer));
			muxer.proc = copy_منزل_sideband;
			muxer.اندريون = -1;
			إذا (!شروعات_async(&muxer))
				err_fd = muxer.اندريون;
			/* ...else, continue without relaying sideband */
		}

		معلومات.cmds = commands;
		معلومات.si = si;
		opt.err_fd = err_fd;
		opt.ترقي = err_fd && !quiet;
		odb_transaction_env(transaction, &env);
		opt.env = env.v;
		opt.exclude_hidden_refs_section = "receive";

		إذا (check_connected(iterate_receive_command_list, &معلومات, &opt))
			سيٽ_connectivity_errors(commands, si, transaction);

		إذا (use_sideband)
			پڄاڻي_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_منزل_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	إذا (proc_receive_ref) {
		لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
			إذا (!should_process_cmd(cmd))
				استمر;

			إذا (proc_receive_ref_matches(cmd)) {
				cmd->لاڳيتو_proc_receive = لاڳيتو_PROC_RECEIVE_SCHEDULED;
				لاڳيتو_proc_receive = 1;
			}
		}
	}

	إذا (لاڳيتو_receive_hook(commands, "pre-receive", 0, transaction, ڌڪو_options)) {
		لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
			إذا (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		أرجع;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	لكل (cmd = commands; cmd && cmd->error_string; cmd = cmd->ايندڙ)
		; /* nothing */
	إذا (!cmd)
		أرجع;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	إذا (odb_transaction_commit(transaction)) {
		لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
			إذا (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		أرجع;
	}

	check_aliased_updates(commands);

	حرر(سر_name_منزل_free);
	سر_name = سر_name_منزل_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, عدم,
							    عدم);

	إذا (لاڳيتو_proc_receive &&
	    لاڳيتو_proc_receive_hook(commands, ڌڪو_options))
		لكل (cmd = commands; cmd; cmd = cmd->ايندڙ)
			إذا (!cmd->error_string &&
			    !(cmd->لاڳيتو_proc_receive & لاڳيتو_PROC_RECEIVE_RETURNED) &&
			    (cmd->لاڳيتو_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	إذا (use_atomic)
		execute_commands_atomic(commands, si);
	وإلا
		execute_commands_non_atomic(commands, si);

	إذا (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

ساكن بنية command **قطار_command(بنية command **پڇ,
				      ثابت حرف *سٽ,
				      صحيح linelen)
{
	بنية object_id old_oid, new_oid;
	بنية command *cmd;
	ثابت حرف *refname;
	صحيح reflen;
	ثابت حرف *p;

	إذا (parse_oid_hex(سٽ, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", سٽ);

	refname = p;
	reflen = linelen - (p - سٽ);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*پڇ = cmd;
	أرجع &cmd->ايندڙ;
}

ساكن فراغ free_commands(بنية command *commands)
{
	طالما (commands) {
		بنية command *ايندڙ = commands->ايندڙ;

		ref_ڌڪو_report_free(commands->report);
		حرر(commands->error_string_owned);
		حرر(commands);
		commands = ايندڙ;
	}
}

ساكن فراغ قطار_commands_from_cert(بنية command **پڇ,
				     بنية strbuf *ڌڪو_cert)
{
	ثابت حرف *boc, *eoc;

	إذا (*پڇ)
		die("protocol error: got both push certificate and unsigned commands");

	boc = لڙي_ڳولا(ڌڪو_cert->buf, "\n\n");
	إذا (!boc)
		die("malformed push certificate %.*s", 100, ڌڪو_cert->buf);
	وإلا
		boc += 2;
	eoc = ڌڪو_cert->buf + parse_signed_بفر(ڌڪو_cert->buf, ڌڪو_cert->len);

	طالما (boc < eoc) {
		ثابت حرف *eol = memchr(boc, '\n', eoc - boc);
		پڇ = قطار_command(پڇ, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

ساكن بنية command *read_سر_info(بنية packet_reader *reader,
				      بنية oid_array *shallow,
				      تعداد report_حالت_نسخو *نسخو)
{
	بنية command *commands = عدم;
	بنية command **p = &commands;
	لكل (;;) {
		صحيح linelen;

		إذا (packet_reader_read(reader) != PACKET_READ_عام)
			اكسر;

		إذا (reader->pktlen > 8 && starts_with(reader->سٽ, "shallow ")) {
			بنية object_id oid;
			إذا (get_oid_hex(reader->سٽ + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->سٽ + 8);
			oid_array_append(shallow, &oid);
			استمر;
		}

		linelen = لڙي_ڊيگهه(reader->سٽ);
		إذا (linelen < reader->pktlen) {
			ثابت حرف *feature_list = reader->سٽ + linelen + 1;
			ثابت حرف *hash = عدم;
			ثابت حرف *client_sid;
			سائيز_t len = 0;
			إذا (parse_feature_request(feature_list, "report-status"))
				*نسخو = REPORT_حالت_V0;
			إذا (parse_feature_request(feature_list, "report-status-v2"))
				*نسخو = REPORT_حالت_V2;
			إذا (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			إذا (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			إذا (advertise_atomic_ڌڪو
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			إذا (advertise_ڌڪو_options
			    && parse_feature_request(feature_list, "push-options"))
				use_ڌڪو_options = 1;
			hash = parse_feature_قدر(feature_list, "object-format", &len, عدم);
			إذا (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = لڙي_ڊيگهه(hash);
			}
			إذا (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_قدر(feature_list, "session-id", &len, عدم);
			إذا (client_sid) {
				حرف *sid = xstrndup(client_sid, len);
				trace2_معلومات_string("transfer", عدم, "client-sid", client_sid);
				حرر(sid);
			}
		}

		إذا (!strcmp(reader->سٽ, "push-cert")) {
			صحيح true_flush = 0;
			صحيح saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			لكل (;;) {
				packet_reader_read(reader);
				إذا (reader->حالت == PACKET_READ_FLUSH) {
					true_flush = 1;
					اكسر;
				}
				إذا (reader->حالت != PACKET_READ_عام) {
					die("protocol error: got an unexpected packet");
				}
				إذا (!strcmp(reader->سٽ, "push-cert-end\n"))
					اكسر; /* end of cert */
				strbuf_addstr(&ڌڪو_cert, reader->سٽ);
			}
			reader->options = saved_options;

			إذا (true_flush)
				اكسر;
			استمر;
		}

		p = قطار_command(p, reader->سٽ, linelen);
	}

	إذا (ڌڪو_cert.len)
		قطار_commands_from_cert(p, &ڌڪو_cert);

	أرجع commands;
}

ساكن فراغ read_ڌڪو_options(بنية packet_reader *reader,
			      بنية string_list *options)
{
	طالما (1) {
		إذا (packet_reader_read(reader) != PACKET_READ_عام)
			اكسر;

		string_list_append(options, reader->سٽ);
	}
}

ساكن صحيح unpack_with_sideband(بنية odb_transaction *transaction,
				ثابت حرف *shallow_file,
				بنية strbuf *err_نياپو)
{
	بنية odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_نياپو_types = fsck_نياپو_types.buf,
		.max_داخلا_سائيز = max_داخلا_سائيز,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	بنية async muxer;
	صحيح ret;

	إذا (!use_sideband)
		أرجع odb_transaction_write_pack(transaction, 0, err_نياپو, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, حجم(muxer));
	muxer.proc = copy_منزل_sideband;
	muxer.اندريون = -1;
	إذا (شروعات_async(&muxer))
		أرجع 0;

	opts.err_fd = muxer.اندريون;
	ret = odb_transaction_write_pack(transaction, 0, err_نياپو, &opts);

	پڄاڻي_async(&muxer);
	أرجع ret;
}

ساكن فراغ prepare_shallow_update(بنية shallow_info *si,
				   ثابت حرف *shallow_file)
{
	صحيح i, j, k, bitmap_سائيز = DIV_ڦيرو_مٿي(si->ref->nr, 32);

	ALLOC_ARRAY(si->استعمال_ٿيل_shallow, si->shallow->nr);
	سونپيو_shallow_commits_منزل_refs(si, si->استعمال_ٿيل_shallow, عدم);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	لكل (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	لكل (i = 0; i < si->shallow->nr; i++) {
		إذا (!si->استعمال_ٿيل_shallow[i])
			استمر;
		لكل (j = 0; j < bitmap_سائيز; j++) {
			إذا (!si->استعمال_ٿيل_shallow[i][j])
				استمر;
			si->need_reachability_test[i]++;
			لكل (k = 0; k < 32; k++)
				إذا (si->استعمال_ٿيل_shallow[i][j] & (1U << k))
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
	صحيح *ref_حالت;
	remove_nonexistent_theirs_shallow(si);
	إذا (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		أرجع;
	}

	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
		إذا (is_null_oid(&cmd->new_oid))
			استمر;
		oid_array_append(ref, &cmd->new_oid);
		cmd->اشاريو = ref->nr - 1;
	}
	si->ref = ref;

	إذا (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		أرجع;
	}

	ALLOC_ARRAY(ref_حالت, ref->nr);
	سونپيو_shallow_commits_منزل_refs(si, عدم, ref_حالت);
	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
		إذا (is_null_oid(&cmd->new_oid))
			استمر;
		إذا (ref_حالت[cmd->اشاريو]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	حرر(ref_حالت);
}

ساكن فراغ override_cmds_error(بنية command *commands, ثابت حرف *err)
{
	لكل (بنية command *cmd = commands; cmd; cmd = cmd->ايندڙ) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
ساكن فراغ generate_report(بنية strbuf *buf, بنية command *commands,
			    ثابت بنية strbuf *unpack_حالت,
			    تعداد report_حالت_نسخو نسخو)
{
	بنية command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_حالت->len ? unpack_حالت->buf : "ok");

	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
		بنية ref_ڌڪو_report *report;
		صحيح ڳڻپ = 0;

		إذا (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		وإلا
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		إذا (نسخو != REPORT_حالت_V2 || cmd->error_string)
			استمر;

		لكل (report = cmd->report; report; report = report->ايندڙ) {
			إذا (ڳڻپ++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			إذا (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			إذا (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_منزل_hex(report->old_oid));
			إذا (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_منزل_hex(report->new_oid));
			إذا (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

ساكن فراغ report(بنية command *commands, ثابت بنية strbuf *unpack_حالت,
		   تعداد report_حالت_نسخو نسخو)
{
	بنية strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_حالت, نسخو);

	إذا (لاڳيتو_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_حالت, نسخو);
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
	لكل (cmd = commands; cmd; cmd = cmd->ايندڙ) {
		إذا (!is_null_oid(&cmd->new_oid))
			أرجع 0;
	}
	أرجع 1;
}

صحيح cmd_receive_pack(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *اڳياڙي,
		     بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح advertise_refs = 0;
	بنية command *commands;
	بنية oid_array shallow = OID_ARRAY_INIT;
	بنية oid_array ref = OID_ARRAY_INIT;
	بنية shallow_info si;
	بنية packet_reader reader;
	بنية odb_transaction *transaction = عدم;
	تعداد report_حالت_نسخو نسخو = REPORT_حالت_UNKNOWN;

	بنية option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, عدم),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, عدم),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, عدم),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, عدم),
		OPT_پڇاڙي()
	};

	packet_trace_سڃاڻپ_عنصر("receive-pack");

	argc = parse_options(argc, argv, اڳياڙي, options, receive_pack_usage, 0);

	إذا (argc > 1)
		usage_نياپو_opt(_("too many arguments"), receive_pack_usage, options);
	إذا (argc == 0)
		usage_نياپو_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	تياري_رستو();

	إذا (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, عدم);
	إذا (cert_nonce_ٻج)
		ڌڪو_cert_nonce = prepare_ڌڪو_cert_nonce(service_dir, وقت(عدم));

	بدل (determine_protocol_نسخو_server()) {
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
	حالة protocol_unknown_نسخو:
		BUG("unknown protocol version");
	}

	إذا (advertise_refs || !stateless_rpc) {
		write_سر_info();
	}
	إذا (advertise_refs)
		أرجع 0;

	packet_reader_init(&reader, 0, عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	إذا ((commands = read_سر_info(&reader, &shallow, &نسخو))) {
		بنية string_list ڌڪو_options = STRING_LIST_INIT_DUP;
		بنية strbuf unpack_حالت = STRBUF_INIT;

		إذا (use_ڌڪو_options)
			read_ڌڪو_options(&reader, &ڌڪو_options);
		إذا (!check_cert_ڌڪو_options(&ڌڪو_options)) {
			بنية command *cmd;
			لكل (cmd = commands; cmd; cmd = cmd->ايندڙ)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		إذا (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		إذا (!delete_only(commands)) {
			ثابت حرف *alt_shallow_file = عدم;

			إذا (si.nr_ours || si.nr_theirs)
				alt_shallow_file = تياري_temporary_shallow(si.shallow);

			إذا (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_حالت, "unable to start object transaction");
			وإلا
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_حالت);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_حالت.len, &si, transaction,
				 &ڌڪو_options);
		odb_transaction_finalize(transaction);
		sigchain_ڌڪو(SIGPIPE, SIG_IGN);

		بدل (نسخو) {
		حالة REPORT_حالت_V2:
		حالة REPORT_حالت_V0:
			report(commands, &unpack_حالت, نسخو);
			اكسر;
		حالة REPORT_حالت_UNKNOWN:
			اكسر;
		}

		sigchain_ڪڍو(SIGPIPE);
		لاڳيتو_receive_hook(commands, "post-receive", 1, عدم,
				 &ڌڪو_options);
		لاڳيتو_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&ڌڪو_options, 0);
		إذا (auto_gc) {
			بنية ٻار_process proc = ٻار_PROCESS_INIT;

			إذا (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_منزل_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				إذا (!شروعات_command(&proc)) {
					إذا (use_sideband)
						copy_منزل_sideband(proc.err, -1, عدم);
					پڄاڻي_command(&proc);
				}
			}
		}
		إذا (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_حالت);
	}
	إذا (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	حرر((فراغ *)ڌڪو_cert_nonce);
	أرجع 0;
}
