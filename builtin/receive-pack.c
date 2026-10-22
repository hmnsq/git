#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "connect.h"
#अन्तर्भाव "connected.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "exec-cmd.h"
#अन्तर्भाव "fsck.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "gpg-interface.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "oidset.h"
#अन्तर्भाव "pack.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "pkt-line.h"
#अन्तर्भाव "protocol.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "server-info.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "shallow.h"
#अन्तर्भाव "sideband.h"
#अन्तर्भाव "sigchain.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "trace.h"
#अन्तर्भाव "trace2.h"
#अन्तर्भाव "version.h"
#अन्तर्भाव "worktree.h"

स्थिर स्थिरांक अक्षर * स्थिरांक receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	रिक्त
};

गणना deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

गणना report_status_version {
	REPORT_STATUS_UNKNOWN = 0,
	REPORT_STATUS_V0,
	REPORT_STATUS_V2,
};

स्थिर पूर्णांक deny_deletes;
स्थिर पूर्णांक deny_non_fast_forwards;
स्थिर गणना deny_action deny_current_branch = DENY_UNCONFIGURED;
स्थिर गणना deny_action deny_delete_current = DENY_UNCONFIGURED;
स्थिर पूर्णांक receive_fsck_objects = -1;
स्थिर पूर्णांक transfer_fsck_objects = -1;
स्थिर संरचना strbuf fsck_संदेश_types = STRBUF_INIT;
स्थिर पूर्णांक advertise_atomic_ढकला = 1;
स्थिर पूर्णांक advertise_ढकला_options;
स्थिर पूर्णांक advertise_sid;
स्थिर off_t max_आदान_size;
स्थिर पूर्णांक use_sideband;
स्थिर पूर्णांक use_atomic;
स्थिर पूर्णांक use_ढकला_options;
स्थिर पूर्णांक quiet;
स्थिर पूर्णांक prefer_ofs_delta = 1;
स्थिर पूर्णांक auto_update_server_info;
स्थिर पूर्णांक auto_gc = 1;
स्थिर पूर्णांक reject_thin;
स्थिर पूर्णांक skip_connectivity_check;
स्थिर पूर्णांक stateless_rpc;
स्थिर स्थिरांक अक्षर *service_dir;
स्थिर स्थिरांक अक्षर *head_name;
स्थिर शून्य *head_name_to_free;
स्थिर पूर्णांक sent_capabilities;
स्थिर पूर्णांक shallow_update;
स्थिर संरचना strbuf ढकला_cert = STRBUF_INIT;
स्थिर संरचना object_id ढकला_cert_oid;
स्थिर संरचना signature_check sigcheck;
स्थिर स्थिरांक अक्षर *ढकला_cert_nonce;
स्थिर अक्षर *cert_nonce_seed;
स्थिर संरचना strvec hidden_refs = STRVEC_INIT;

स्थिर स्थिरांक अक्षर *NONCE_UNSOLICITED = "UNSOLICITED";
स्थिर स्थिरांक अक्षर *NONCE_BAD = "BAD";
स्थिर स्थिरांक अक्षर *NONCE_MISSING = "MISSING";
स्थिर स्थिरांक अक्षर *NONCE_OK = "OK";
स्थिर स्थिरांक अक्षर *NONCE_SLOP = "SLOP";
स्थिर स्थिरांक अक्षर *nonce_status;
स्थिर दीर्घ nonce_stamp_slop;
स्थिर timestamp_t nonce_stamp_slop_limit;
स्थिर संरचना ref_transaction *transaction;

स्थिर गणना {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
स्थिर पूर्णांक keepalive_in_sec = 5;

स्थिर संरचना proc_receive_ref {
	अचिन्हित पूर्णांक want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	अक्षर *ref_prefix;
	संरचना proc_receive_ref *next;
} *proc_receive_ref;

स्थिर शून्य proc_receive_ref_append(स्थिरांक अक्षर *prefix);

स्थिर गणना deny_action parse_deny_action(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य)
{
	जर (मूल्य) {
		जर (!strcasecmp(मूल्य, "ignore"))
			परत DENY_IGNORE;
		जर (!strcasecmp(मूल्य, "warn"))
			परत DENY_WARN;
		जर (!strcasecmp(मूल्य, "refuse"))
			परत DENY_REFUSE;
		जर (!strcasecmp(मूल्य, "updateinstead"))
			परत DENY_UPDATE_INSTEAD;
	}
	जर (git_config_bool(var, मूल्य))
		परत DENY_REFUSE;
	परत DENY_IGNORE;
}

स्थिर पूर्णांक receive_pack_config(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य,
			       स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	स्थिरांक अक्षर *संदेश_id;
	पूर्णांक status = parse_hide_refs_config(var, मूल्य, "receive", &hidden_refs);

	जर (status)
		परत status;

	जर (म_भेद(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (म_भेद(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (म_भेद(var, "receive.fsck.skiplist") == 0) {
		अक्षर *path;

		जर (git_config_pathname(&path, var, मूल्य))
			परत -1;
		जर (path)
			strbuf_addf(&fsck_संदेश_types, "%cskiplist=%s",
				    fsck_संदेश_types.len ? ',' : '=', path);
		मुक्त(path);
		परत 0;
	}

	जर (skip_prefix(var, "receive.fsck.", &संदेश_id)) {
		जर (!मूल्य)
			परत config_error_nonbool(var);
		जर (is_valid_संदेश_type(संदेश_id, मूल्य))
			strbuf_addf(&fsck_संदेश_types, "%c%s=%s",
				fsck_संदेश_types.len ? ',' : '=', संदेश_id, मूल्य);
		अन्यथा
			warning("skipping unknown msg id '%s'", संदेश_id);
		परत 0;
	}

	जर (म_भेद(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (म_भेद(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (!म_भेद(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, मूल्य);
		परत 0;
	}

	जर (म_भेद(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, मूल्य);
		परत 0;
	}

	जर (म_भेद(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (म_भेद(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (म_भेद(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (म_भेद(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (म_भेद(var, "receive.certnonceseed") == 0)
		परत git_config_string(&cert_nonce_seed, var, मूल्य);

	जर (म_भेद(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, मूल्य, ctx->kvi);
		परत 0;
	}

	जर (म_भेद(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_ढकला = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (म_भेद(var, "receive.advertisepushoptions") == 0) {
		advertise_ढकला_options = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (म_भेद(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, मूल्य, ctx->kvi);
		परत 0;
	}

	जर (म_भेद(var, "receive.maxinputsize") == 0) {
		max_आदान_size = git_config_int64(var, मूल्य, ctx->kvi);
		परत 0;
	}

	जर (म_भेद(var, "receive.procreceiverefs") == 0) {
		जर (!मूल्य)
			परत config_error_nonbool(var);
		proc_receive_ref_append(मूल्य);
		परत 0;
	}

	जर (म_भेद(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, मूल्य);
		परत 0;
	}

	परत git_default_config(var, मूल्य, ctx, cb);
}

स्थिर शून्य show_ref(स्थिरांक अक्षर *path, स्थिरांक संरचना object_id *oid)
{
	जर (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} अन्यथा {
		संरचना strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		जर (advertise_atomic_ढकला)
			strbuf_addstr(&cap, " atomic");
		जर (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		जर (ढकला_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", ढकला_cert_nonce);
		जर (advertise_ढकला_options)
			strbuf_addstr(&cap, " push-options");
		जर (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

स्थिर पूर्णांक show_ref_cb(स्थिरांक संरचना reference *ref, शून्य *data)
{
	संरचना oidset *seen = data;
	स्थिरांक अक्षर *path = strip_namespace(ref->name);

	जर (ref_is_hidden(path, ref->name, &hidden_refs))
		परत 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	जर (!path) {
		जर (oidset_घाला(seen, ref->oid))
			परत 0;
		path = ".have";
	} अन्यथा {
		oidset_घाला(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	परत 0;
}

स्थिर शून्य show_one_alternate_ref(स्थिरांक संरचना object_id *oid,
				   शून्य *data)
{
	संरचना oidset *seen = data;

	जर (oidset_घाला(seen, oid))
		परत;

	show_ref(".have", oid);
}

स्थिर शून्य write_head_info(शून्य)
{
	संरचना refs_for_each_ref_options opts = { 0 };
	स्थिर संरचना oidset seen = OIDSET_INIT;
	संरचना strvec excludes_vector = STRVEC_INIT;

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

	जर (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#define RUN_PROC_RECEIVE_SCHEDULED	1
#define RUN_PROC_RECEIVE_RETURNED	2
संरचना command {
	संरचना command *next;
	स्थिरांक अक्षर *error_string;
	अक्षर *error_string_owned;
	संरचना ref_ढकला_report *report;
	अचिन्हित पूर्णांक skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	पूर्णांक index;
	संरचना object_id old_oid;
	संरचना object_id new_oid;
	अक्षर ref_name[FLEX_ARRAY]; /* more */
};

स्थिर शून्य proc_receive_ref_append(स्थिरांक अक्षर *prefix)
{
	संरचना proc_receive_ref *ref_pattern;
	स्थिरांक अक्षर *p;
	पूर्णांक len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = म_अक्षर(prefix, ':');
	जर (p) {
		पर्यंत (prefix < p) {
			जर (*prefix == 'a')
				ref_pattern->want_add = 1;
			अन्यथा जर (*prefix == 'd')
				ref_pattern->want_delete = 1;
			अन्यथा जर (*prefix == 'm')
				ref_pattern->want_modify = 1;
			अन्यथा जर (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} अन्यथा {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = म_माप(prefix);
	पर्यंत (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	जर (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} अन्यथा {
		संरचना proc_receive_ref *end;

		end = proc_receive_ref;
		पर्यंत (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

स्थिर पूर्णांक proc_receive_ref_matches(संरचना command *cmd)
{
	संरचना proc_receive_ref *p;

	जर (!proc_receive_ref)
		परत 0;

	साठी (p = proc_receive_ref; p; p = p->next) {
		स्थिरांक अक्षर *match = p->ref_prefix;
		स्थिरांक अक्षर *remains;

		जर (!p->want_add && is_null_oid(&cmd->old_oid))
			सुरूठेवा;
		अन्यथा जर (!p->want_delete && is_null_oid(&cmd->new_oid))
			सुरूठेवा;
		अन्यथा जर (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			सुरूठेवा;

		जर (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			जर (!p->negative_ref)
				परत 1;
		} अन्यथा जर (p->negative_ref) {
			परत 1;
		}
	}
	परत 0;
}

स्थिर शून्य report_message(स्थिरांक अक्षर *prefix, स्थिरांक अक्षर *err, बहु_सूची params)
{
	पूर्णांक sz;
	अक्षर संदेश[4096];

	sz = xsnprintf(संदेश, आकार(संदेश), "%s", prefix);
	sz += vsnprintf(संदेश + sz, आकार(संदेश) - sz, err, params);
	जर (sz > (आकार(संदेश) - 1))
		sz = आकार(संदेश) - 1;
	संदेश[sz++] = '\n';

	जर (use_sideband)
		send_sideband(1, 2, संदेश, sz, use_sideband);
	अन्यथा
		xwrite(2, संदेश, sz);
}

__attribute__((format (म_लिखो, 1, 2)))
स्थिर शून्य rp_warning(स्थिरांक अक्षर *err, ...)
{
	बहु_सूची params;
	बहु_शुरू(params, err);
	report_message("warning: ", err, params);
	बहु_पूर्ण(params);
}

__attribute__((format (म_लिखो, 1, 2)))
स्थिर शून्य rp_error(स्थिरांक अक्षर *err, ...)
{
	बहु_सूची params;
	बहु_शुरू(params, err);
	report_message("error: ", err, params);
	बहु_पूर्ण(params);
}

स्थिर पूर्णांक copy_to_sideband(पूर्णांक in, पूर्णांक out UNUSED, शून्य *arg UNUSED)
{
	अक्षर data[128];
	पूर्णांक keepalive_active = 0;

	जर (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	जर (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	पर्यंत (1) {
		ssize_t sz;

		जर (keepalive_active) {
			संरचना pollfd pfd;
			पूर्णांक ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			जर (ret < 0) {
				जर (त्रुटि_सं == EINTR)
					सुरूठेवा;
				अन्यथा
					थांबा;
			} अन्यथा जर (ret == 0) {
				/* no data; send a keepalive packet */
				स्थिर स्थिरांक अक्षर buf[] = "0005\1";
				write_or_die(1, buf, आकार(buf) - 1);
				सुरूठेवा;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, आकार(data));
		जर (sz <= 0)
			थांबा;

		जर (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			स्थिरांक अक्षर *p = स_प्रथम(data, '\0', sz);
			जर (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_active = 1;
				send_sideband(1, 2, data, p - data, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - data + 1), use_sideband);
				सुरूठेवा;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, data, sz, use_sideband);
	}
	बंद(in);
	परत 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
स्थिर शून्य prepare_sideband_async(संरचना async *sideband_async, पूर्णांक *saved_stderr, पूर्णांक *started)
{
	*started = 0;

	जर (!use_sideband)
		परत;

	स_रखो(sideband_async, 0, आकार(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	जर (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		जर (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		बंद(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
स्थिर शून्य finish_sideband_async(संरचना async *sideband_async, पूर्णांक saved_stderr, पूर्णांक started)
{
	जर (!use_sideband)
		परत;

	जर (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		बंद(saved_stderr);
	}

	जर (started)
		finish_async(sideband_async);
}

स्थिर शून्य hmac_hash(अचिन्हित अक्षर *out,
		      स्थिरांक अक्षर *किल्ली_in, माप_प्रकार किल्ली_len,
		      स्थिरांक अक्षर *text, माप_प्रकार text_len)
{
	अचिन्हित अक्षर किल्ली[GIT_MAX_BLKSZ];
	अचिन्हित अक्षर k_ipad[GIT_MAX_BLKSZ];
	अचिन्हित अक्षर k_opad[GIT_MAX_BLKSZ];
	पूर्णांक i;
	संरचना git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	स_रखो(किल्ली, '\0', GIT_MAX_BLKSZ);
	जर (the_hash_algo->blksz < किल्ली_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, किल्ली_in, किल्ली_len);
		git_hash_final(किल्ली, &ctx);
	} अन्यथा {
		स_नकल(किल्ली, किल्ली_in, किल्ली_len);
	}

	/* RFC 2104 2. (2) & (5) */
	साठी (i = 0; i < आकार(किल्ली); i++) {
		k_ipad[i] = किल्ली[i] ^ 0x36;
		k_opad[i] = किल्ली[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, आकार(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, आकार(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

स्थिर अक्षर *prepare_ढकला_cert_nonce(स्थिरांक अक्षर *path, timestamp_t stamp)
{
	संरचना strbuf buf = STRBUF_INIT;
	अचिन्हित अक्षर hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, म_माप(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (पूर्णांक)the_hash_algo->hexsz, hash_to_hex(hash));
	परत strbuf_detach(&buf, रिक्त);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
स्थिर पूर्णांक constant_memequal(स्थिरांक अक्षर *a, स्थिरांक अक्षर *b, माप_प्रकार n)
{
	पूर्णांक res = 0;
	माप_प्रकार i;

	साठी (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	परत res;
}

स्थिर स्थिरांक अक्षर *check_nonce(स्थिरांक अक्षर *buf)
{
	माप_प्रकार noncelen;
	स्थिरांक अक्षर *found = find_commit_header(buf, "nonce", &noncelen);
	अक्षर *nonce = found ? xmemdupz(found, noncelen) : रिक्त;
	timestamp_t stamp, ostamp;
	अक्षर *bohmac, *expect = रिक्त;
	स्थिरांक अक्षर *retval = NONCE_BAD;

	जर (!nonce) {
		retval = NONCE_MISSING;
		जा leave;
	} अन्यथा जर (!ढकला_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		जा leave;
	} अन्यथा जर (!म_भेद(ढकला_cert_nonce, nonce)) {
		retval = NONCE_OK;
		जा leave;
	}

	जर (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		जा leave;
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
	जर (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		जा leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	जर (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		जा leave;
	}

	expect = prepare_ढकला_cert_nonce(service_dir, stamp);
	जर (noncelen != म_माप(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		जा leave;
	}
	जर (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		जा leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(ढकला_cert_nonce, रिक्त, 10);
	nonce_stamp_slop = (दीर्घ)ostamp - (दीर्घ)stamp;

	जर (nonce_stamp_slop_limit &&
	    द_असल(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		मुक्त((शून्य *)ढकला_cert_nonce);
		ढकला_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} अन्यथा {
		retval = NONCE_SLOP;
	}

leave:
	मुक्त(nonce);
	मुक्त(expect);
	परत retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
स्थिर पूर्णांक check_cert_ढकला_options(स्थिरांक संरचना string_list *ढकला_options)
{
	स्थिरांक अक्षर *buf = ढकला_cert.buf;

	स्थिरांक अक्षर *option;
	माप_प्रकार optionlen;
	पूर्णांक options_seen = 0;

	पूर्णांक retval = 1;

	जर (!*buf)
		परत 1;

	पर्यंत ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		जर (options_seen > ढकला_options->nr
		    || xstrncmpz(ढकला_options->वस्तुसंच[options_seen - 1].string,
				 option, optionlen))
			परत 0;
	}

	जर (options_seen != ढकला_options->nr)
		retval = 0;

	परत retval;
}

स्थिर शून्य prepare_ढकला_cert_sha1(संरचना run_hooks_opt *opt)
{
	स्थिर पूर्णांक already_done;

	जर (!ढकला_cert.len)
		परत;

	जर (!already_done) {
		पूर्णांक bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		जर (odb_write_object(the_repository->objects, ढकला_cert.buf,
				     ढकला_cert.len, OBJ_BLOB, &ढकला_cert_oid))
			oidclr(&ढकला_cert_oid, the_repository->hash_algo);

		स_रखो(&sigcheck, '\0', आकार(sigcheck));

		bogs = parse_signed_buffer(ढकला_cert.buf, ढकला_cert.len);
		sigcheck.payload = xmemdupz(ढकला_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, ढकला_cert.buf + bogs,
				ढकला_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	जर (!is_null_oid(&ढकला_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&ढकला_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.किल्ली ? sigcheck.किल्ली : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		जर (ढकला_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     ढकला_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			जर (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

संरचना receive_hook_feed_state {
	संरचना command *cmd;
	संरचना ref_ढकला_report *report;
	पूर्णांक skip_broken;
	संरचना strbuf buf;
};

स्थिर पूर्णांक feed_receive_hook_cb(पूर्णांक hook_stdin_fd, शून्य *pp_cb UNUSED, शून्य *pp_task_cb)
{
	संरचना receive_hook_feed_state *state = pp_task_cb;
	संरचना command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	पर्यंत (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	जर (!cmd)
		परत 1;  /* no more commands left */

	जर (!state->report)
		state->report = cmd->report;

	जर (state->report) {
		संरचना object_id *old_oid;
		संरचना object_id *new_oid;
		स्थिरांक अक्षर *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->next;
		जर (!state->report)
			cmd = cmd->next;
	} अन्यथा {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	जर (state->buf.len > 0) {
		पूर्णांक ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		जर (ret < 0) {
			जर (त्रुटि_सं == EPIPE)
				परत 1; /* child closed pipe */
			परत ret;
		}
	}

	परत state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

स्थिर शून्य *receive_hook_feed_state_alloc(शून्य *feed_pipe_ctx)
{
	संरचना receive_hook_feed_state *init_state = feed_pipe_ctx;
	संरचना receive_hook_feed_state *data;

	CALLOC_ARRAY(data, 1);
	data->report = init_state->report;
	data->cmd = init_state->cmd;
	data->skip_broken = init_state->skip_broken;
	strbuf_init(&data->buf, 0);

	परत data;
}

स्थिर शून्य receive_hook_feed_state_free(शून्य *data)
{
	संरचना receive_hook_feed_state *d = data;
	जर (!d)
		परत;
	strbuf_release(&d->buf);
	मुक्त(d);
}

स्थिर पूर्णांक run_receive_hook(संरचना command *commands,
			    स्थिरांक अक्षर *hook_name,
			    पूर्णांक skip_broken,
			    संरचना odb_transaction *transaction,
			    स्थिरांक संरचना string_list *ढकला_options)
{
	संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	संरचना command *iter = commands;
	संरचना receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	संरचना async sideband_async;
	पूर्णांक sideband_async_started = 0;
	पूर्णांक saved_stderr = -1;
	पूर्णांक ret;

	जर (!hook_exists(the_repository, hook_name))
		परत 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	पर्यंत (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	जर (!iter)
		परत 0;

	जर (ढकला_options) {
		साठी (पूर्णांक i = 0; i < ढकला_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     ढकला_options->वस्तुसंच[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)ढकला_options->nr);
	} अन्यथा {
		strvec_ढकला(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	जर (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_ढकला_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_data_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_data_free = receive_hook_feed_state_free;

	ret = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	परत ret;
}

स्थिर पूर्णांक run_update_hook(संरचना command *cmd)
{
	स्थिर स्थिरांक अक्षर hook_name[] = "update";
	संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	संरचना async sideband_async;
	पूर्णांक sideband_async_started = 0;
	पूर्णांक saved_stderr = -1;
	पूर्णांक code;

	जर (!hook_exists(the_repository, hook_name))
		परत 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     रिक्त);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	परत code;
}

स्थिर पूर्णांक run_receive_report_hook(संरचना strbuf *report)
{
	संरचना child_process proc = CHILD_PROCESS_INIT;
	संरचना async sideband_async;
	पूर्णांक sideband_async_started = 0;
	पूर्णांक saved_stderr = -1;
	संरचना strbuf out = STRBUF_INIT;
	स्थिरांक अक्षर *hook_path;
	पूर्णांक ret;

	hook_path = find_hook(the_repository, "receive-report");
	जर (!hook_path)
		परत 0;

	strvec_ढकला(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_ढकला(SIGPIPE, संक_छोड़ो);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, रिक्त, 0);
	sigchain_काढा(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	जर (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	परत ret;
}

स्थिर संरचना command *find_command_by_refname(संरचना command *list,
					       स्थिरांक अक्षर *refname)
{
	साठी (; list; list = list->next)
		जर (!म_भेद(list->ref_name, refname))
			परत list;
	परत रिक्त;
}

स्थिर पूर्णांक read_proc_receive_report(संरचना packet_reader *reader,
				    संरचना command *commands,
				    संरचना strbuf *errmsg)
{
	संरचना command *cmd;
	संरचना command *hint = रिक्त;
	संरचना ref_ढकला_report *report = रिक्त;
	पूर्णांक new_report = 0;
	पूर्णांक code = 0;
	पूर्णांक once = 0;
	पूर्णांक response = 0;

	साठी (;;) {
		संरचना object_id old_oid, new_oid;
		अक्षर *head;
		अक्षर *refname;
		अक्षर *p;
		गणना packet_read_status status;

		status = packet_reader_read(reader);
		जर (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			जर (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				परत -1;
			}
			थांबा;
		}
		response++;

		head = reader->ओळ;
		p = म_अक्षर(head, ' ');
		जर (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			सुरूठेवा;
		}
		*p++ = '\0';
		जर (!म_भेद(head, "option")) {
			अक्षर *किल्ली;
			स्थिरांक अक्षर *val;

			जर (!hint || !(report || new_report)) {
				जर (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				सुरूठेवा;
			}
			जर (new_report) {
				जर (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} अन्यथा {
					report = hint->report;
					पर्यंत (report->next)
						report = report->next;
					report->next = xcalloc(1, आकार(संरचना ref_ढकला_report));
					report = report->next;
				}
				new_report = 0;
			}
			किल्ली = p;
			p = म_अक्षर(किल्ली, ' ');
			जर (p)
				*p++ = '\0';
			val = p;
			जर (!म_भेद(किल्ली, "refname"))
				report->ref_name = xstrdup_or_null(val);
			अन्यथा जर (!म_भेद(किल्ली, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			अन्यथा जर (!म_भेद(किल्ली, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			अन्यथा जर (!म_भेद(किल्ली, "forced-update"))
				report->forced_update = 1;
			अन्यथा जर (!म_भेद(किल्ली, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			सुरूठेवा;
		}

		report = रिक्त;
		new_report = 0;
		refname = p;
		p = म_अक्षर(refname, ' ');
		जर (p)
			*p++ = '\0';
		जर (म_भेद(head, "ok") && म_भेद(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			सुरूठेवा;
		}

		/* first try searching at our hint, falling back to all refs */
		जर (hint)
			hint = find_command_by_refname(hint, refname);
		जर (!hint)
			hint = find_command_by_refname(commands, refname);
		जर (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			सुरूठेवा;
		}
		जर (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			सुरूठेवा;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		जर (!म_भेद(head, "ng")) {
			जर (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			अन्यथा
				hint->error_string = "failed";
			code = -1;
			सुरूठेवा;
		}
		new_report = 1;
	}

	साठी (cmd = commands; cmd; cmd = cmd->next)
		जर (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	परत code;
}

स्थिर पूर्णांक run_proc_receive_hook(संरचना command *commands,
				 स्थिरांक संरचना string_list *ढकला_options)
{
	संरचना child_process proc = CHILD_PROCESS_INIT;
	संरचना async muxer;
	संरचना command *cmd;
	संरचना packet_reader reader;
	संरचना strbuf cap = STRBUF_INIT;
	संरचना strbuf errmsg = STRBUF_INIT;
	पूर्णांक hook_use_ढकला_options = 0;
	पूर्णांक version = 0;
	पूर्णांक code;
	स्थिरांक अक्षर *hook_path = find_hook(the_repository, "proc-receive");

	जर (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		परत -1;
	}

	strvec_ढकला(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	जर (use_sideband) {
		स_रखो(&muxer, 0, आकार(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		जर (code)
			परत code;
		proc.err = muxer.in;
	} अन्यथा {
		proc.err = 0;
	}

	code = start_command(&proc);
	जर (code) {
		जर (use_sideband)
			finish_async(&muxer);
		परत code;
	}

	sigchain_ढकला(SIGPIPE, संक_छोड़ो);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, रिक्त, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	जर (use_atomic)
		strbuf_addstr(&cap, " atomic");
	जर (use_ढकला_options)
		strbuf_addstr(&cap, " push-options");
	जर (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} अन्यथा {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	जर (!code)
		code = packet_flush_gently(proc.in);

	जर (!code)
		साठी (;;) {
			पूर्णांक linelen;
			गणना packet_read_status status;

			status = packet_reader_read(&reader);
			जर (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				जर (status == PACKET_READ_EOF)
					code = -1;
				थांबा;
			}

			जर (reader.pktlen > 8 && starts_with(reader.ओळ, "version=")) {
				version = म_से_प(reader.ओळ + 8);
				linelen = म_माप(reader.ओळ);
				जर (linelen < reader.pktlen) {
					स्थिरांक अक्षर *feature_list = reader.ओळ + linelen + 1;
					जर (parse_feature_request(feature_list, "push-options"))
						hook_use_ढकला_options = 1;
				}
			}
		}

	जर (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		जा cleanup;
	}

	निवड (version) {
	स्थिती 0:
		/* fallthrough */
	स्थिती 1:
		थांबा;
	पूर्वनिर्धारित:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		जा cleanup;
	}

	/* Send commands */
	साठी (cmd = commands; cmd; cmd = cmd->next) {
		जर (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			सुरूठेवा;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		जर (code)
			थांबा;
	}
	जर (!code)
		code = packet_flush_gently(proc.in);
	जर (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		जा cleanup;
	}

	/* Send push options */
	जर (hook_use_ढकला_options) {
		संरचना string_list_item *item;

		for_each_string_list_item(item, ढकला_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			जर (code)
				थांबा;
		}
		जर (!code)
			code = packet_flush_gently(proc.in);
		जर (code) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			जा cleanup;
		}
	}

	/* Read result from proc-receive */
	code = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	बंद(proc.in);
	बंद(proc.out);
	जर (use_sideband)
		finish_async(&muxer);
	जर (finish_command(&proc))
		code = -1;
	जर (errmsg.len >0) {
		अक्षर *p = errmsg.buf;

		p += errmsg.len - 1;
		जर (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_काढा(SIGPIPE);

	परत code;
}

स्थिर स्थिरांक अक्षर *refuse_unconfigured_deny_संदेश =
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

स्थिर शून्य refuse_unconfigured_deny(शून्य)
{
	rp_error("%s", _(refuse_unconfigured_deny_संदेश));
}

स्थिर स्थिरांक अक्षर *refuse_unconfigured_deny_delete_current_संदेश =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

स्थिर शून्य refuse_unconfigured_deny_delete_current(शून्य)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_संदेश));
}

स्थिर स्थिरांक संरचना object_id *command_singleton_iterator(शून्य *cb_data);
स्थिर पूर्णांक update_shallow_ref(संरचना command *cmd, संरचना shallow_info *si)
{
	संरचना shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	संरचना oid_array extra = OID_ARRAY_INIT;
	संरचना check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	पूर्णांक i;

	trace_printf_किल्ली(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	साठी (i = 0; i < si->shallow->nr; i++)
		जर (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	जर (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		परत -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	साठी (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	परत 0;
}

स्थिर स्थिरांक अक्षर *ढकला_to_deploy(अचिन्हित अक्षर *sha1,
				  संरचना strvec *env,
				  स्थिरांक संरचना worktree *worktree)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", रिक्त);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	जर (run_command(&child))
		परत "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", रिक्त);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	जर (run_command(&child))
		परत "Working directory has unstaged changes";

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
		     empty_वृक्ष_oid_hex(the_repository->hash_algo), "--", रिक्त);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	जर (run_command(&child))
		परत "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     रिक्त);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	जर (run_command(&child))
		परत "Could not update working tree to new HEAD";

	परत रिक्त;
}

स्थिर स्थिरांक अक्षर *ढकला_to_checkout_hook = "push-to-checkout";

स्थिर स्थिरांक अक्षर *ढकला_to_checkout(अचिन्हित अक्षर *hash,
				    पूर्णांक *invoked_hook,
				    संरचना strvec *env,
				    स्थिरांक अक्षर *work_वृक्ष)
{
	संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_वृक्ष));
	strvec_ढकला(&opt.args, hash_to_hex(hash));
	जर (run_hooks_opt(the_repository, ढकला_to_checkout_hook, &opt))
		परत "push-to-checkout hook declined";
	अन्यथा
		परत रिक्त;
}

स्थिर स्थिरांक अक्षर *update_worktree(अचिन्हित अक्षर *sha1, स्थिरांक संरचना worktree *worktree)
{
	स्थिरांक अक्षर *retval;
	अक्षर *git_dir;
	संरचना strvec env = STRVEC_INIT;
	पूर्णांक invoked_hook;

	जर (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	जर (worktree->is_bare)
		परत "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = ढकला_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	जर (!invoked_hook)
		retval = ढकला_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	मुक्त(git_dir);
	परत retval;
}

स्थिर स्थिरांक अक्षर *update(संरचना command *cmd, संरचना shallow_info *si)
{
	स्थिरांक अक्षर *name = cmd->ref_name;
	संरचना strbuf namespaced_name_buf = STRBUF_INIT;
	स्थिर अक्षर *namespaced_name;
	स्थिरांक अक्षर *ret;
	संरचना object_id *old_oid = &cmd->old_oid;
	संरचना object_id *new_oid = &cmd->new_oid;
	पूर्णांक do_update_worktree = 0;
	संरचना worktree **worktrees = get_worktrees(the_repository);
	स्थिरांक संरचना worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	जर (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		जा out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	मुक्त(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, रिक्त);

	जर (worktree && !worktree->is_bare) {
		निवड (deny_current_branch) {
		स्थिती DENY_IGNORE:
			थांबा;
		स्थिती DENY_WARN:
			rp_warning("updating the current branch");
			थांबा;
		स्थिती DENY_REFUSE:
		स्थिती DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			जर (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			जा out;
		स्थिती DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			थांबा;
		}
	}

	जर (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		जा out;
	}

	जर (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		जर (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			जा out;
		}

		जर (worktree || (head_name && !म_भेद(namespaced_name, head_name))) {
			निवड (deny_delete_current) {
			स्थिती DENY_IGNORE:
				थांबा;
			स्थिती DENY_WARN:
				rp_warning("deleting the current branch");
				थांबा;
			स्थिती DENY_REFUSE:
			स्थिती DENY_UNCONFIGURED:
			स्थिती DENY_UPDATE_INSTEAD:
				जर (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				जा out;
			पूर्वनिर्धारित:
				ret = "Invalid denyDeleteCurrent setting";
				जा out;
			}
		}
	}

	जर (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		संरचना object *old_object, *new_object;
		संरचना commit *old_commit, *new_commit;
		पूर्णांक ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		जर (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			जा out;
		}
		old_commit = (संरचना commit *)old_object;
		new_commit = (संरचना commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		जर (ret2 < 0)
			निकास(128);
		जर (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			जा out;
		}
	}
	जर (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		जा out;
	}

	जर (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		जर (ret)
			जा out;
	}

	जर (is_null_oid(new_oid)) {
		संरचना strbuf err = STRBUF_INIT;
		जर (!parse_object(the_repository, old_oid)) {
			old_oid = रिक्त;
			जर (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} अन्यथा {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		जर (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   रिक्त, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} अन्यथा {
			ret = रिक्त; /* good */
		}
		strbuf_release(&err);
	} अन्यथा {
		गणना ref_transaction_error tx_err;
		संरचना strbuf err = STRBUF_INIT;
		जर (shallow_update && si->shallow_ref[cmd->index] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			जा out;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  रिक्त, रिक्त,
						  0, "push",
						  &err);
		जर (tx_err) {
			rp_error("%s", err.buf);
			जर (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			अन्यथा
				ret = ref_transaction_error_संदेश(tx_err);
		} अन्यथा {
			ret = रिक्त; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	परत ret;
}

स्थिर शून्य run_update_post_hook(संरचना command *commands)
{
	स्थिर स्थिरांक अक्षर hook_name[] = "post-update";
	संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	संरचना async sideband_async;
	संरचना command *cmd;
	पूर्णांक sideband_async_started = 0;
	पूर्णांक saved_stderr = -1;

	जर (!hook_exists(the_repository, hook_name))
		परत;

	साठी (cmd = commands; cmd; cmd = cmd->next) {
		जर (cmd->error_string || cmd->did_not_exist)
			सुरूठेवा;
		strvec_ढकला(&opt.args, cmd->ref_name);
	}
	जर (!opt.args.nr)
		परत;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

स्थिर शून्य check_aliased_update_internal(संरचना command *cmd,
					  संरचना string_list *list,
					  स्थिरांक अक्षर *dst_name, पूर्णांक flag)
{
	संरचना string_list_item *item;
	संरचना command *dst_cmd;

	जर (!(flag & REF_ISSYMREF))
		परत;

	जर (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		परत;
	}
	dst_name = strip_namespace(dst_name);

	जर (!(item = string_list_lookup(list, dst_name)))
		परत;

	cmd->skip_update = 1;

	dst_cmd = (संरचना command *) item->util;

	जर (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		परत;

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

स्थिर शून्य check_aliased_update(संरचना command *cmd, संरचना string_list *list)
{
	संरचना strbuf buf = STRBUF_INIT;
	स्थिरांक अक्षर *dst_name;
	पूर्णांक flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, रिक्त, &flag);
	check_aliased_update_internal(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

स्थिर शून्य check_aliased_updates(संरचना command *commands)
{
	संरचना command *cmd;
	संरचना string_list ref_list = STRING_LIST_INIT_NODUP;

	साठी (cmd = commands; cmd; cmd = cmd->next) {
		संरचना string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (शून्य *)cmd;
	}
	string_list_sort(&ref_list);

	साठी (cmd = commands; cmd; cmd = cmd->next) {
		जर (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

स्थिर स्थिरांक संरचना object_id *command_singleton_iterator(शून्य *cb_data)
{
	संरचना command **cmd_list = cb_data;
	संरचना command *cmd = *cmd_list;

	जर (!cmd || is_null_oid(&cmd->new_oid))
		परत रिक्त;
	*cmd_list = रिक्त; /* this returns only one */
	परत &cmd->new_oid;
}

स्थिर शून्य set_connectivity_errors(संरचना command *commands,
				    संरचना shallow_info *si,
				    संरचना odb_transaction *transaction)
{
	संरचना command *cmd;

	साठी (cmd = commands; cmd; cmd = cmd->next) {
		संरचना command *singleton = cmd;
		संरचना check_connected_options opt = CHECK_CONNECTED_INIT;
		संरचना strvec env = STRVEC_INIT;

		जर (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			सुरूठेवा;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		जर (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			सुरूठेवा;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

संरचना iterate_data {
	संरचना command *cmds;
	संरचना shallow_info *si;
};

स्थिर स्थिरांक संरचना object_id *iterate_receive_command_list(शून्य *cb_data)
{
	संरचना iterate_data *data = cb_data;
	संरचना command **cmd_list = &data->cmds;
	संरचना command *cmd = *cmd_list;

	साठी (; cmd; cmd = cmd->next) {
		जर (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			सुरूठेवा;
		जर (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			परत &cmd->new_oid;
		}
	}
	परत रिक्त;
}

स्थिर शून्य reject_updates_to_hidden(संरचना command *commands)
{
	संरचना strbuf refname_full = STRBUF_INIT;
	माप_प्रकार prefix_len;
	संरचना command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	साठी (cmd = commands; cmd; cmd = cmd->next) {
		जर (cmd->error_string)
			सुरूठेवा;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		जर (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			सुरूठेवा;
		जर (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		अन्यथा
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

स्थिर पूर्णांक should_process_cmd(संरचना command *cmd)
{
	परत !cmd->error_string && !cmd->skip_update;
}

स्थिर शून्य BUG_if_skipped_connectivity_check(संरचना command *commands,
					       संरचना shallow_info *si)
{
	संरचना command *cmd;

	साठी (cmd = commands; cmd; cmd = cmd->next) {
		जर (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

स्थिर शून्य ref_transaction_rejection_handler(स्थिरांक अक्षर *refname,
					      स्थिरांक संरचना object_id *old_oid UNUSED,
					      स्थिरांक संरचना object_id *new_oid UNUSED,
					      स्थिरांक अक्षर *old_target UNUSED,
					      स्थिरांक अक्षर *new_target UNUSED,
					      गणना ref_transaction_error err,
					      स्थिरांक अक्षर *details,
					      शून्य *cb_data)
{
	संरचना strmap *failed_refs = cb_data;

	जर (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (अक्षर *)ref_transaction_error_संदेश(err));
}

स्थिर शून्य execute_commands_non_atomic(संरचना command *commands,
					संरचना shallow_info *si)
{
	संरचना command *cmd;
	संरचना strbuf err = STRBUF_INIT;
	स्थिरांक अक्षर *reported_error = रिक्त;
	संरचना strmap failed_refs = STRMAP_INIT;

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
	गणना processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	साठी (गणना processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		साठी (cmd = commands; cmd; cmd = cmd->next) {
			जर (!should_process_cmd(cmd) || cmd->run_proc_receive)
				सुरूठेवा;

			जर (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				सुरूठेवा;
			अन्यथा जर (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				सुरूठेवा;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			जर (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				जर (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					जा failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		जर (!transaction)
			जा cleanup;

		जर (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			जा failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		जर (strmap_empty(&failed_refs))
			जा cleanup;

	failure:
		साठी (cmd = commands; cmd; cmd = cmd->next) {
			जर (reported_error)
				cmd->error_string = reported_error;
			अन्यथा जर (strmap_contains(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = रिक्त;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

स्थिर शून्य execute_commands_atomic(संरचना command *commands,
					संरचना shallow_info *si)
{
	संरचना command *cmd;
	संरचना strbuf err = STRBUF_INIT;
	स्थिरांक अक्षर *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	जर (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		जा failure;
	}

	साठी (cmd = commands; cmd; cmd = cmd->next) {
		जर (!should_process_cmd(cmd) || cmd->run_proc_receive)
			सुरूठेवा;

		cmd->error_string = update(cmd, si);

		जर (cmd->error_string)
			जा failure;
	}

	जर (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		जा failure;
	}
	जा cleanup;

failure:
	साठी (cmd = commands; cmd; cmd = cmd->next)
		जर (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

स्थिर शून्य execute_commands(संरचना command *commands,
			     पूर्णांक unpacker_error,
			     संरचना shallow_info *si,
			     संरचना odb_transaction *transaction,
			     स्थिरांक संरचना string_list *ढकला_options)
{
	संरचना check_connected_options opt = CHECK_CONNECTED_INIT;
	संरचना command *cmd;
	संरचना iterate_data data;
	संरचना async muxer;
	पूर्णांक err_fd = 0;
	पूर्णांक run_proc_receive = 0;

	जर (unpacker_error) {
		साठी (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		परत;
	}

	जर (!skip_connectivity_check) {
		संरचना strvec env = STRVEC_INIT;

		जर (use_sideband) {
			स_रखो(&muxer, 0, आकार(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			जर (!start_async(&muxer))
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

		जर (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		जर (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	जर (proc_receive_ref) {
		साठी (cmd = commands; cmd; cmd = cmd->next) {
			जर (!should_process_cmd(cmd))
				सुरूठेवा;

			जर (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	जर (run_receive_hook(commands, "pre-receive", 0, transaction, ढकला_options)) {
		साठी (cmd = commands; cmd; cmd = cmd->next) {
			जर (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		परत;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	साठी (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	जर (!cmd)
		परत;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	जर (odb_transaction_commit(transaction)) {
		साठी (cmd = commands; cmd; cmd = cmd->next) {
			जर (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		परत;
	}

	check_aliased_updates(commands);

	मुक्त(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, रिक्त,
							    रिक्त);

	जर (run_proc_receive &&
	    run_proc_receive_hook(commands, ढकला_options))
		साठी (cmd = commands; cmd; cmd = cmd->next)
			जर (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	जर (use_atomic)
		execute_commands_atomic(commands, si);
	अन्यथा
		execute_commands_non_atomic(commands, si);

	जर (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

स्थिर संरचना command **queue_command(संरचना command **tail,
				      स्थिरांक अक्षर *ओळ,
				      पूर्णांक linelen)
{
	संरचना object_id old_oid, new_oid;
	संरचना command *cmd;
	स्थिरांक अक्षर *refname;
	पूर्णांक reflen;
	स्थिरांक अक्षर *p;

	जर (parse_oid_hex(ओळ, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", ओळ);

	refname = p;
	reflen = linelen - (p - ओळ);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*tail = cmd;
	परत &cmd->next;
}

स्थिर शून्य free_commands(संरचना command *commands)
{
	पर्यंत (commands) {
		संरचना command *next = commands->next;

		ref_ढकला_report_free(commands->report);
		मुक्त(commands->error_string_owned);
		मुक्त(commands);
		commands = next;
	}
}

स्थिर शून्य queue_commands_from_cert(संरचना command **tail,
				     संरचना strbuf *ढकला_cert)
{
	स्थिरांक अक्षर *boc, *eoc;

	जर (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = म_माला(ढकला_cert->buf, "\n\n");
	जर (!boc)
		die("malformed push certificate %.*s", 100, ढकला_cert->buf);
	अन्यथा
		boc += 2;
	eoc = ढकला_cert->buf + parse_signed_buffer(ढकला_cert->buf, ढकला_cert->len);

	पर्यंत (boc < eoc) {
		स्थिरांक अक्षर *eol = स_प्रथम(boc, '\n', eoc - boc);
		tail = queue_command(tail, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

स्थिर संरचना command *read_head_info(संरचना packet_reader *reader,
				      संरचना oid_array *shallow,
				      गणना report_status_version *version)
{
	संरचना command *commands = रिक्त;
	संरचना command **p = &commands;
	साठी (;;) {
		पूर्णांक linelen;

		जर (packet_reader_read(reader) != PACKET_READ_NORMAL)
			थांबा;

		जर (reader->pktlen > 8 && starts_with(reader->ओळ, "shallow ")) {
			संरचना object_id oid;
			जर (get_oid_hex(reader->ओळ + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->ओळ + 8);
			oid_array_append(shallow, &oid);
			सुरूठेवा;
		}

		linelen = म_माप(reader->ओळ);
		जर (linelen < reader->pktlen) {
			स्थिरांक अक्षर *feature_list = reader->ओळ + linelen + 1;
			स्थिरांक अक्षर *hash = रिक्त;
			स्थिरांक अक्षर *client_sid;
			माप_प्रकार len = 0;
			जर (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			जर (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			जर (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			जर (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			जर (advertise_atomic_ढकला
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			जर (advertise_ढकला_options
			    && parse_feature_request(feature_list, "push-options"))
				use_ढकला_options = 1;
			hash = parse_feature_मूल्य(feature_list, "object-format", &len, रिक्त);
			जर (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = म_माप(hash);
			}
			जर (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_मूल्य(feature_list, "session-id", &len, रिक्त);
			जर (client_sid) {
				अक्षर *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", रिक्त, "client-sid", client_sid);
				मुक्त(sid);
			}
		}

		जर (!म_भेद(reader->ओळ, "push-cert")) {
			पूर्णांक true_flush = 0;
			पूर्णांक saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			साठी (;;) {
				packet_reader_read(reader);
				जर (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					थांबा;
				}
				जर (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				जर (!म_भेद(reader->ओळ, "push-cert-end\n"))
					थांबा; /* end of cert */
				strbuf_addstr(&ढकला_cert, reader->ओळ);
			}
			reader->options = saved_options;

			जर (true_flush)
				थांबा;
			सुरूठेवा;
		}

		p = queue_command(p, reader->ओळ, linelen);
	}

	जर (ढकला_cert.len)
		queue_commands_from_cert(p, &ढकला_cert);

	परत commands;
}

स्थिर शून्य read_ढकला_options(संरचना packet_reader *reader,
			      संरचना string_list *options)
{
	पर्यंत (1) {
		जर (packet_reader_read(reader) != PACKET_READ_NORMAL)
			थांबा;

		string_list_append(options, reader->ओळ);
	}
}

स्थिर पूर्णांक unpack_with_sideband(संरचना odb_transaction *transaction,
				स्थिरांक अक्षर *shallow_file,
				संरचना strbuf *err_संदेश)
{
	संरचना odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_संदेश_types = fsck_संदेश_types.buf,
		.max_आदान_size = max_आदान_size,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	संरचना async muxer;
	पूर्णांक ret;

	जर (!use_sideband)
		परत odb_transaction_write_pack(transaction, 0, err_संदेश, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	स_रखो(&muxer, 0, आकार(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	जर (start_async(&muxer))
		परत 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_संदेश, &opts);

	finish_async(&muxer);
	परत ret;
}

स्थिर शून्य prepare_shallow_update(संरचना shallow_info *si,
				   स्थिरांक अक्षर *shallow_file)
{
	पूर्णांक i, j, k, bitmap_size = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, रिक्त);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	साठी (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	साठी (i = 0; i < si->shallow->nr; i++) {
		जर (!si->used_shallow[i])
			सुरूठेवा;
		साठी (j = 0; j < bitmap_size; j++) {
			जर (!si->used_shallow[i][j])
				सुरूठेवा;
			si->need_reachability_test[i]++;
			साठी (k = 0; k < 32; k++)
				जर (si->used_shallow[i][j] & (1U << k))
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

स्थिर शून्य update_shallow_info(संरचना command *commands,
				संरचना shallow_info *si,
				संरचना oid_array *ref,
				स्थिरांक अक्षर *shallow_file)
{
	संरचना command *cmd;
	पूर्णांक *ref_status;
	remove_nonexistent_theirs_shallow(si);
	जर (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		परत;
	}

	साठी (cmd = commands; cmd; cmd = cmd->next) {
		जर (is_null_oid(&cmd->new_oid))
			सुरूठेवा;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	जर (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		परत;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, रिक्त, ref_status);
	साठी (cmd = commands; cmd; cmd = cmd->next) {
		जर (is_null_oid(&cmd->new_oid))
			सुरूठेवा;
		जर (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	मुक्त(ref_status);
}

स्थिर शून्य override_cmds_error(संरचना command *commands, स्थिरांक अक्षर *err)
{
	साठी (संरचना command *cmd = commands; cmd; cmd = cmd->next) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
स्थिर शून्य generate_report(संरचना strbuf *buf, संरचना command *commands,
			    स्थिरांक संरचना strbuf *unpack_status,
			    गणना report_status_version version)
{
	संरचना command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	साठी (cmd = commands; cmd; cmd = cmd->next) {
		संरचना ref_ढकला_report *report;
		पूर्णांक मोजणी = 0;

		जर (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		अन्यथा
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		जर (version != REPORT_STATUS_V2 || cmd->error_string)
			सुरूठेवा;

		साठी (report = cmd->report; report; report = report->next) {
			जर (मोजणी++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			जर (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			जर (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			जर (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			जर (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

स्थिर शून्य report(संरचना command *commands, स्थिरांक संरचना strbuf *unpack_status,
		   गणना report_status_version version)
{
	संरचना strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	जर (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	जर (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	अन्यथा
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

स्थिर पूर्णांक delete_only(संरचना command *commands)
{
	संरचना command *cmd;
	साठी (cmd = commands; cmd; cmd = cmd->next) {
		जर (!is_null_oid(&cmd->new_oid))
			परत 0;
	}
	परत 1;
}

पूर्णांक cmd_receive_pack(पूर्णांक argc,
		     स्थिरांक अक्षर **argv,
		     स्थिरांक अक्षर *prefix,
		     संरचना repository *repo UNUSED)
{
	पूर्णांक advertise_refs = 0;
	संरचना command *commands;
	संरचना oid_array shallow = OID_ARRAY_INIT;
	संरचना oid_array ref = OID_ARRAY_INIT;
	संरचना shallow_info si;
	संरचना packet_reader reader;
	संरचना odb_transaction *transaction = रिक्त;
	गणना report_status_version version = REPORT_STATUS_UNKNOWN;

	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, रिक्त),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, रिक्त),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, रिक्त),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, रिक्त),
		OPT_END()
	};

	packet_trace_identity("receive-pack");

	argc = parse_options(argc, argv, prefix, options, receive_pack_usage, 0);

	जर (argc > 1)
		usage_संदेश_opt(_("too many arguments"), receive_pack_usage, options);
	जर (argc == 0)
		usage_संदेश_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	जर (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, रिक्त);
	जर (cert_nonce_seed)
		ढकला_cert_nonce = prepare_ढकला_cert_nonce(service_dir, समय(रिक्त));

	निवड (determine_protocol_version_server()) {
	स्थिती protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		थांबा;
	स्थिती protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		जर (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	स्थिती protocol_v0:
		थांबा;
	स्थिती protocol_unknown_version:
		BUG("unknown protocol version");
	}

	जर (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	जर (advertise_refs)
		परत 0;

	packet_reader_init(&reader, 0, रिक्त, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	जर ((commands = read_head_info(&reader, &shallow, &version))) {
		संरचना string_list ढकला_options = STRING_LIST_INIT_DUP;
		संरचना strbuf unpack_status = STRBUF_INIT;

		जर (use_ढकला_options)
			read_ढकला_options(&reader, &ढकला_options);
		जर (!check_cert_ढकला_options(&ढकला_options)) {
			संरचना command *cmd;
			साठी (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		जर (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		जर (!delete_only(commands)) {
			स्थिरांक अक्षर *alt_shallow_file = रिक्त;

			जर (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			जर (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			अन्यथा
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &ढकला_options);
		odb_transaction_finalize(transaction);
		sigchain_ढकला(SIGPIPE, संक_छोड़ो);

		निवड (version) {
		स्थिती REPORT_STATUS_V2:
		स्थिती REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			थांबा;
		स्थिती REPORT_STATUS_UNKNOWN:
			थांबा;
		}

		sigchain_काढा(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, रिक्त,
				 &ढकला_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&ढकला_options, 0);
		जर (auto_gc) {
			संरचना child_process proc = CHILD_PROCESS_INIT;

			जर (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				जर (!start_command(&proc)) {
					जर (use_sideband)
						copy_to_sideband(proc.err, -1, रिक्त);
					finish_command(&proc);
				}
			}
		}
		जर (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	जर (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	मुक्त((शून्य *)ढकला_cert_nonce);
	परत 0;
}
