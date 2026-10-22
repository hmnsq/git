#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "commit.h"
#समावेश "commit-reach.h"
#समावेश "config.h"
#समावेश "connect.h"
#समावेश "connected.h"
#समावेश "environment.h"
#समावेश "exec-cmd.h"
#समावेश "fsck.h"
#समावेश "gettext.h"
#समावेश "gpg-interface.h"
#समावेश "hex.h"
#समावेश "hook.h"
#समावेश "object.h"
#समावेश "object-file.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "oid-array.h"
#समावेश "oidset.h"
#समावेश "pack.h"
#समावेश "parse-options.h"
#समावेश "pkt-line.h"
#समावेश "protocol.h"
#समावेश "refs.h"
#समावेश "remote.h"
#समावेश "run-command.h"
#समावेश "server-info.h"
#समावेश "setup.h"
#समावेश "shallow.h"
#समावेश "sideband.h"
#समावेश "sigchain.h"
#समावेश "string-list.h"
#समावेश "strvec.h"
#समावेश "trace.h"
#समावेश "trace2.h"
#समावेश "version.h"
#समावेश "worktree.h"

अटल स्थिर अक्षर * स्थिर receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	शून्य
};

क्रमागत deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

क्रमागत report_status_version {
	REPORT_STATUS_UNKNOWN = 0,
	REPORT_STATUS_V0,
	REPORT_STATUS_V2,
};

अटल पूर्णांक deny_deletes;
अटल पूर्णांक deny_non_fast_forwards;
अटल क्रमागत deny_action deny_current_branch = DENY_UNCONFIGURED;
अटल क्रमागत deny_action deny_delete_current = DENY_UNCONFIGURED;
अटल पूर्णांक receive_fsck_objects = -1;
अटल पूर्णांक transfer_fsck_objects = -1;
अटल काष्ठा strbuf fsck_संदेश_types = STRBUF_INIT;
अटल पूर्णांक advertise_atomic_धकेलो = 1;
अटल पूर्णांक advertise_धकेलो_options;
अटल पूर्णांक advertise_sid;
अटल off_t max_निवेश_आकार;
अटल पूर्णांक use_sideband;
अटल पूर्णांक use_atomic;
अटल पूर्णांक use_धकेलो_options;
अटल पूर्णांक quiet;
अटल पूर्णांक prefer_ofs_delta = 1;
अटल पूर्णांक auto_update_server_info;
अटल पूर्णांक auto_gc = 1;
अटल पूर्णांक reject_thin;
अटल पूर्णांक skip_connectivity_check;
अटल पूर्णांक stateless_rpc;
अटल स्थिर अक्षर *service_dir;
अटल स्थिर अक्षर *शीर्ष_name;
अटल व्योम *शीर्ष_name_to_free;
अटल पूर्णांक sent_capabilities;
अटल पूर्णांक shallow_update;
अटल काष्ठा strbuf धकेलो_cert = STRBUF_INIT;
अटल काष्ठा object_id धकेलो_cert_oid;
अटल काष्ठा signature_check sigcheck;
अटल स्थिर अक्षर *धकेलो_cert_nonce;
अटल अक्षर *cert_nonce_seed;
अटल काष्ठा strvec hidden_refs = STRVEC_INIT;

अटल स्थिर अक्षर *NONCE_UNSOLICITED = "UNSOLICITED";
अटल स्थिर अक्षर *NONCE_BAD = "BAD";
अटल स्थिर अक्षर *NONCE_MISSING = "MISSING";
अटल स्थिर अक्षर *NONCE_OK = "OK";
अटल स्थिर अक्षर *NONCE_SLOP = "SLOP";
अटल स्थिर अक्षर *nonce_status;
अटल दीर्घ nonce_stamp_slop;
अटल timestamp_t nonce_stamp_slop_limit;
अटल काष्ठा ref_transaction *transaction;

अटल क्रमागत {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
अटल पूर्णांक keepalive_in_sec = 5;

अटल काष्ठा proc_receive_ref {
	अचिन्हित पूर्णांक want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	अक्षर *ref_prefix;
	काष्ठा proc_receive_ref *अग्र;
} *proc_receive_ref;

अटल व्योम proc_receive_ref_append(स्थिर अक्षर *prefix);

अटल क्रमागत deny_action parse_deny_action(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य)
{
	अगर (मूल्य) {
		अगर (!strcasecmp(मूल्य, "ignore"))
			वापस DENY_IGNORE;
		अगर (!strcasecmp(मूल्य, "warn"))
			वापस DENY_WARN;
		अगर (!strcasecmp(मूल्य, "refuse"))
			वापस DENY_REFUSE;
		अगर (!strcasecmp(मूल्य, "updateinstead"))
			वापस DENY_UPDATE_INSTEAD;
	}
	अगर (git_config_bool(var, मूल्य))
		वापस DENY_REFUSE;
	वापस DENY_IGNORE;
}

अटल पूर्णांक receive_pack_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			       स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	स्थिर अक्षर *संदेश_id;
	पूर्णांक status = parse_hide_refs_config(var, मूल्य, "receive", &hidden_refs);

	अगर (status)
		वापस status;

	अगर (म_भेद(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.fsck.skiplist") == 0) {
		अक्षर *path;

		अगर (git_config_pathname(&path, var, मूल्य))
			वापस -1;
		अगर (path)
			strbuf_addf(&fsck_संदेश_types, "%cskiplist=%s",
				    fsck_संदेश_types.len ? ',' : '=', path);
		मुक्त(path);
		वापस 0;
	}

	अगर (skip_prefix(var, "receive.fsck.", &संदेश_id)) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		अगर (is_valid_संदेश_type(संदेश_id, मूल्य))
			strbuf_addf(&fsck_संदेश_types, "%c%s=%s",
				fsck_संदेश_types.len ? ',' : '=', संदेश_id, मूल्य);
		अन्यथा
			warning("skipping unknown msg id '%s'", संदेश_id);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (म_भेद(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (!म_भेद(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, मूल्य);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, मूल्य);
		वापस 0;
	}

	अगर (म_भेद(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.certnonceseed") == 0)
		वापस git_config_string(&cert_nonce_seed, var, मूल्य);

	अगर (म_भेद(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, मूल्य, ctx->kvi);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_धकेलो = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.advertisepushoptions") == 0) {
		advertise_धकेलो_options = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, मूल्य, ctx->kvi);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.maxinputsize") == 0) {
		max_निवेश_आकार = git_config_int64(var, मूल्य, ctx->kvi);
		वापस 0;
	}

	अगर (म_भेद(var, "receive.procreceiverefs") == 0) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		proc_receive_ref_append(मूल्य);
		वापस 0;
	}

	अगर (म_भेद(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, मूल्य);
		वापस 0;
	}

	वापस git_default_config(var, मूल्य, ctx, cb);
}

अटल व्योम show_ref(स्थिर अक्षर *path, स्थिर काष्ठा object_id *oid)
{
	अगर (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} अन्यथा {
		काष्ठा strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		अगर (advertise_atomic_धकेलो)
			strbuf_addstr(&cap, " atomic");
		अगर (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		अगर (धकेलो_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", धकेलो_cert_nonce);
		अगर (advertise_धकेलो_options)
			strbuf_addstr(&cap, " push-options");
		अगर (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

अटल पूर्णांक show_ref_cb(स्थिर काष्ठा reference *ref, व्योम *तथ्य)
{
	काष्ठा oidset *seen = तथ्य;
	स्थिर अक्षर *path = strip_namespace(ref->name);

	अगर (ref_is_hidden(path, ref->name, &hidden_refs))
		वापस 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	अगर (!path) {
		अगर (oidset_जोड़ो(seen, ref->oid))
			वापस 0;
		path = ".have";
	} अन्यथा {
		oidset_जोड़ो(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	वापस 0;
}

अटल व्योम show_one_alternate_ref(स्थिर काष्ठा object_id *oid,
				   व्योम *तथ्य)
{
	काष्ठा oidset *seen = तथ्य;

	अगर (oidset_जोड़ो(seen, oid))
		वापस;

	show_ref(".have", oid);
}

अटल व्योम write_शीर्ष_info(व्योम)
{
	काष्ठा refs_for_each_ref_options opts = { 0 };
	अटल काष्ठा oidset seen = OIDSET_INIT;
	काष्ठा strvec excludes_vector = STRVEC_INIT;

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

	अगर (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#घोषणा RUN_PROC_RECEIVE_SCHEDULED	1
#घोषणा RUN_PROC_RECEIVE_RETURNED	2
काष्ठा command {
	काष्ठा command *अग्र;
	स्थिर अक्षर *error_string;
	अक्षर *error_string_owned;
	काष्ठा ref_धकेलो_report *report;
	अचिन्हित पूर्णांक skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	पूर्णांक सूचकांक;
	काष्ठा object_id old_oid;
	काष्ठा object_id new_oid;
	अक्षर ref_name[FLEX_ARRAY]; /* more */
};

अटल व्योम proc_receive_ref_append(स्थिर अक्षर *prefix)
{
	काष्ठा proc_receive_ref *ref_pattern;
	स्थिर अक्षर *p;
	पूर्णांक len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = म_अक्षर(prefix, ':');
	अगर (p) {
		जबतक (prefix < p) {
			अगर (*prefix == 'a')
				ref_pattern->want_add = 1;
			अन्यथा अगर (*prefix == 'd')
				ref_pattern->want_delete = 1;
			अन्यथा अगर (*prefix == 'm')
				ref_pattern->want_modify = 1;
			अन्यथा अगर (*prefix == '!')
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
	जबतक (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	अगर (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} अन्यथा {
		काष्ठा proc_receive_ref *end;

		end = proc_receive_ref;
		जबतक (end->अग्र)
			end = end->अग्र;
		end->अग्र = ref_pattern;
	}
}

अटल पूर्णांक proc_receive_ref_matches(काष्ठा command *cmd)
{
	काष्ठा proc_receive_ref *p;

	अगर (!proc_receive_ref)
		वापस 0;

	क्रम (p = proc_receive_ref; p; p = p->अग्र) {
		स्थिर अक्षर *match = p->ref_prefix;
		स्थिर अक्षर *remains;

		अगर (!p->want_add && is_null_oid(&cmd->old_oid))
			जारी;
		अन्यथा अगर (!p->want_delete && is_null_oid(&cmd->new_oid))
			जारी;
		अन्यथा अगर (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			जारी;

		अगर (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			अगर (!p->negative_ref)
				वापस 1;
		} अन्यथा अगर (p->negative_ref) {
			वापस 1;
		}
	}
	वापस 0;
}

अटल व्योम report_message(स्थिर अक्षर *prefix, स्थिर अक्षर *err, बहु_सूची params)
{
	पूर्णांक sz;
	अक्षर संदेश[4096];

	sz = xsnprintf(संदेश, माप(संदेश), "%s", prefix);
	sz += vsnprintf(संदेश + sz, माप(संदेश) - sz, err, params);
	अगर (sz > (माप(संदेश) - 1))
		sz = माप(संदेश) - 1;
	संदेश[sz++] = '\n';

	अगर (use_sideband)
		send_sideband(1, 2, संदेश, sz, use_sideband);
	अन्यथा
		xwrite(2, संदेश, sz);
}

__attribute__((format (म_लिखो, 1, 2)))
अटल व्योम rp_warning(स्थिर अक्षर *err, ...)
{
	बहु_सूची params;
	बहु_शुरू(params, err);
	report_message("warning: ", err, params);
	बहु_पूर्ण(params);
}

__attribute__((format (म_लिखो, 1, 2)))
अटल व्योम rp_error(स्थिर अक्षर *err, ...)
{
	बहु_सूची params;
	बहु_शुरू(params, err);
	report_message("error: ", err, params);
	बहु_पूर्ण(params);
}

अटल पूर्णांक copy_to_sideband(पूर्णांक in, पूर्णांक out UNUSED, व्योम *arg UNUSED)
{
	अक्षर तथ्य[128];
	पूर्णांक keepalive_active = 0;

	अगर (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	अगर (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	जबतक (1) {
		ssize_t sz;

		अगर (keepalive_active) {
			काष्ठा pollfd pfd;
			पूर्णांक ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			अगर (ret < 0) {
				अगर (त्रुटि_सं == EINTR)
					जारी;
				अन्यथा
					अवरोध;
			} अन्यथा अगर (ret == 0) {
				/* no data; send a keepalive packet */
				अटल स्थिर अक्षर buf[] = "0005\1";
				write_or_die(1, buf, माप(buf) - 1);
				जारी;
			} /* else there is actual data to read */
		}

		sz = xread(in, तथ्य, माप(तथ्य));
		अगर (sz <= 0)
			अवरोध;

		अगर (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			स्थिर अक्षर *p = स_प्रथम(तथ्य, '\0', sz);
			अगर (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_active = 1;
				send_sideband(1, 2, तथ्य, p - तथ्य, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - तथ्य + 1), use_sideband);
				जारी;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, तथ्य, sz, use_sideband);
	}
	बंद(in);
	वापस 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
अटल व्योम prepare_sideband_async(काष्ठा async *sideband_async, पूर्णांक *saved_stderr, पूर्णांक *started)
{
	*started = 0;

	अगर (!use_sideband)
		वापस;

	स_रखो(sideband_async, 0, माप(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	अगर (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		अगर (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		बंद(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
अटल व्योम finish_sideband_async(काष्ठा async *sideband_async, पूर्णांक saved_stderr, पूर्णांक started)
{
	अगर (!use_sideband)
		वापस;

	अगर (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		बंद(saved_stderr);
	}

	अगर (started)
		finish_async(sideband_async);
}

अटल व्योम hmac_hash(अचिन्हित अक्षर *out,
		      स्थिर अक्षर *कुंजी_in, माप_प्रकार कुंजी_len,
		      स्थिर अक्षर *text, माप_प्रकार text_len)
{
	अचिन्हित अक्षर कुंजी[GIT_MAX_BLKSZ];
	अचिन्हित अक्षर k_ipad[GIT_MAX_BLKSZ];
	अचिन्हित अक्षर k_opad[GIT_MAX_BLKSZ];
	पूर्णांक i;
	काष्ठा git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	स_रखो(कुंजी, '\0', GIT_MAX_BLKSZ);
	अगर (the_hash_algo->blksz < कुंजी_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, कुंजी_in, कुंजी_len);
		git_hash_final(कुंजी, &ctx);
	} अन्यथा {
		स_नकल(कुंजी, कुंजी_in, कुंजी_len);
	}

	/* RFC 2104 2. (2) & (5) */
	क्रम (i = 0; i < माप(कुंजी); i++) {
		k_ipad[i] = कुंजी[i] ^ 0x36;
		k_opad[i] = कुंजी[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, माप(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, माप(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

अटल अक्षर *prepare_धकेलो_cert_nonce(स्थिर अक्षर *path, timestamp_t stamp)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	अचिन्हित अक्षर hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, म_माप(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (पूर्णांक)the_hash_algo->hexsz, hash_to_hex(hash));
	वापस strbuf_detach(&buf, शून्य);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
अटल पूर्णांक constant_memequal(स्थिर अक्षर *a, स्थिर अक्षर *b, माप_प्रकार n)
{
	पूर्णांक res = 0;
	माप_प्रकार i;

	क्रम (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	वापस res;
}

अटल स्थिर अक्षर *check_nonce(स्थिर अक्षर *buf)
{
	माप_प्रकार noncelen;
	स्थिर अक्षर *found = find_commit_header(buf, "nonce", &noncelen);
	अक्षर *nonce = found ? xmemdupz(found, noncelen) : शून्य;
	timestamp_t stamp, ostamp;
	अक्षर *bohmac, *expect = शून्य;
	स्थिर अक्षर *retval = NONCE_BAD;

	अगर (!nonce) {
		retval = NONCE_MISSING;
		जाओ leave;
	} अन्यथा अगर (!धकेलो_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		जाओ leave;
	} अन्यथा अगर (!म_भेद(धकेलो_cert_nonce, nonce)) {
		retval = NONCE_OK;
		जाओ leave;
	}

	अगर (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		जाओ leave;
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
	अगर (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		जाओ leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	अगर (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		जाओ leave;
	}

	expect = prepare_धकेलो_cert_nonce(service_dir, stamp);
	अगर (noncelen != म_माप(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		जाओ leave;
	}
	अगर (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		जाओ leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(धकेलो_cert_nonce, शून्य, 10);
	nonce_stamp_slop = (दीर्घ)ostamp - (दीर्घ)stamp;

	अगर (nonce_stamp_slop_limit &&
	    द_असल(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		मुक्त((व्योम *)धकेलो_cert_nonce);
		धकेलो_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} अन्यथा {
		retval = NONCE_SLOP;
	}

leave:
	मुक्त(nonce);
	मुक्त(expect);
	वापस retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
अटल पूर्णांक check_cert_धकेलो_options(स्थिर काष्ठा string_list *धकेलो_options)
{
	स्थिर अक्षर *buf = धकेलो_cert.buf;

	स्थिर अक्षर *option;
	माप_प्रकार optionlen;
	पूर्णांक options_seen = 0;

	पूर्णांक retval = 1;

	अगर (!*buf)
		वापस 1;

	जबतक ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		अगर (options_seen > धकेलो_options->nr
		    || xstrncmpz(धकेलो_options->वस्तुएँ[options_seen - 1].string,
				 option, optionlen))
			वापस 0;
	}

	अगर (options_seen != धकेलो_options->nr)
		retval = 0;

	वापस retval;
}

अटल व्योम prepare_धकेलो_cert_sha1(काष्ठा run_hooks_opt *opt)
{
	अटल पूर्णांक already_done;

	अगर (!धकेलो_cert.len)
		वापस;

	अगर (!already_done) {
		पूर्णांक bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		अगर (odb_write_object(the_repository->objects, धकेलो_cert.buf,
				     धकेलो_cert.len, OBJ_BLOB, &धकेलो_cert_oid))
			oidclr(&धकेलो_cert_oid, the_repository->hash_algo);

		स_रखो(&sigcheck, '\0', माप(sigcheck));

		bogs = parse_signed_बफर(धकेलो_cert.buf, धकेलो_cert.len);
		sigcheck.payload = xmemdupz(धकेलो_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, धकेलो_cert.buf + bogs,
				धकेलो_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	अगर (!is_null_oid(&धकेलो_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&धकेलो_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.कुंजी ? sigcheck.कुंजी : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.परिणाम);
		अगर (धकेलो_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     धकेलो_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			अगर (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

काष्ठा receive_hook_feed_state {
	काष्ठा command *cmd;
	काष्ठा ref_धकेलो_report *report;
	पूर्णांक skip_broken;
	काष्ठा strbuf buf;
};

अटल पूर्णांक feed_receive_hook_cb(पूर्णांक hook_stdin_fd, व्योम *pp_cb UNUSED, व्योम *pp_task_cb)
{
	काष्ठा receive_hook_feed_state *state = pp_task_cb;
	काष्ठा command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	जबतक (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->अग्र;

	अगर (!cmd)
		वापस 1;  /* no more commands left */

	अगर (!state->report)
		state->report = cmd->report;

	अगर (state->report) {
		काष्ठा object_id *old_oid;
		काष्ठा object_id *new_oid;
		स्थिर अक्षर *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->अग्र;
		अगर (!state->report)
			cmd = cmd->अग्र;
	} अन्यथा {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->अग्र;
	}

	state->cmd = cmd;

	अगर (state->buf.len > 0) {
		पूर्णांक ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		अगर (ret < 0) {
			अगर (त्रुटि_सं == EPIPE)
				वापस 1; /* child closed pipe */
			वापस ret;
		}
	}

	वापस state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

अटल व्योम *receive_hook_feed_state_alloc(व्योम *feed_pipe_ctx)
{
	काष्ठा receive_hook_feed_state *init_state = feed_pipe_ctx;
	काष्ठा receive_hook_feed_state *तथ्य;

	CALLOC_ARRAY(तथ्य, 1);
	तथ्य->report = init_state->report;
	तथ्य->cmd = init_state->cmd;
	तथ्य->skip_broken = init_state->skip_broken;
	strbuf_init(&तथ्य->buf, 0);

	वापस तथ्य;
}

अटल व्योम receive_hook_feed_state_free(व्योम *तथ्य)
{
	काष्ठा receive_hook_feed_state *d = तथ्य;
	अगर (!d)
		वापस;
	strbuf_release(&d->buf);
	मुक्त(d);
}

अटल पूर्णांक run_receive_hook(काष्ठा command *commands,
			    स्थिर अक्षर *hook_name,
			    पूर्णांक skip_broken,
			    काष्ठा odb_transaction *transaction,
			    स्थिर काष्ठा string_list *धकेलो_options)
{
	काष्ठा run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	काष्ठा command *iter = commands;
	काष्ठा receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	काष्ठा async sideband_async;
	पूर्णांक sideband_async_started = 0;
	पूर्णांक saved_stderr = -1;
	पूर्णांक ret;

	अगर (!hook_exists(the_repository, hook_name))
		वापस 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	जबतक (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->अग्र;
	अगर (!iter)
		वापस 0;

	अगर (धकेलो_options) {
		क्रम (पूर्णांक i = 0; i < धकेलो_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     धकेलो_options->वस्तुएँ[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)धकेलो_options->nr);
	} अन्यथा {
		strvec_धकेलो(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	अगर (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_धकेलो_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_तथ्य_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_तथ्य_free = receive_hook_feed_state_free;

	ret = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	वापस ret;
}

अटल पूर्णांक run_update_hook(काष्ठा command *cmd)
{
	अटल स्थिर अक्षर hook_name[] = "update";
	काष्ठा run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	काष्ठा async sideband_async;
	पूर्णांक sideband_async_started = 0;
	पूर्णांक saved_stderr = -1;
	पूर्णांक code;

	अगर (!hook_exists(the_repository, hook_name))
		वापस 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     शून्य);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	वापस code;
}

अटल पूर्णांक run_receive_report_hook(काष्ठा strbuf *report)
{
	काष्ठा child_process proc = CHILD_PROCESS_INIT;
	काष्ठा async sideband_async;
	पूर्णांक sideband_async_started = 0;
	पूर्णांक saved_stderr = -1;
	काष्ठा strbuf out = STRBUF_INIT;
	स्थिर अक्षर *hook_path;
	पूर्णांक ret;

	hook_path = find_hook(the_repository, "receive-report");
	अगर (!hook_path)
		वापस 0;

	strvec_धकेलो(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_धकेलो(SIGPIPE, संक_छोड़ो);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, शून्य, 0);
	sigchain_निकालो(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	अगर (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	वापस ret;
}

अटल काष्ठा command *find_command_by_refname(काष्ठा command *list,
					       स्थिर अक्षर *refname)
{
	क्रम (; list; list = list->अग्र)
		अगर (!म_भेद(list->ref_name, refname))
			वापस list;
	वापस शून्य;
}

अटल पूर्णांक read_proc_receive_report(काष्ठा packet_reader *reader,
				    काष्ठा command *commands,
				    काष्ठा strbuf *errmsg)
{
	काष्ठा command *cmd;
	काष्ठा command *hint = शून्य;
	काष्ठा ref_धकेलो_report *report = शून्य;
	पूर्णांक new_report = 0;
	पूर्णांक code = 0;
	पूर्णांक once = 0;
	पूर्णांक response = 0;

	क्रम (;;) {
		काष्ठा object_id old_oid, new_oid;
		अक्षर *शीर्ष;
		अक्षर *refname;
		अक्षर *p;
		क्रमागत packet_read_status status;

		status = packet_reader_read(reader);
		अगर (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			अगर (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				वापस -1;
			}
			अवरोध;
		}
		response++;

		शीर्ष = reader->पंक्ति;
		p = म_अक्षर(शीर्ष, ' ');
		अगर (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", शीर्ष);
			code = -1;
			जारी;
		}
		*p++ = '\0';
		अगर (!म_भेद(शीर्ष, "option")) {
			अक्षर *कुंजी;
			स्थिर अक्षर *val;

			अगर (!hint || !(report || new_report)) {
				अगर (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				जारी;
			}
			अगर (new_report) {
				अगर (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} अन्यथा {
					report = hint->report;
					जबतक (report->अग्र)
						report = report->अग्र;
					report->अग्र = xcalloc(1, माप(काष्ठा ref_धकेलो_report));
					report = report->अग्र;
				}
				new_report = 0;
			}
			कुंजी = p;
			p = म_अक्षर(कुंजी, ' ');
			अगर (p)
				*p++ = '\0';
			val = p;
			अगर (!म_भेद(कुंजी, "refname"))
				report->ref_name = xstrdup_or_null(val);
			अन्यथा अगर (!म_भेद(कुंजी, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			अन्यथा अगर (!म_भेद(कुंजी, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			अन्यथा अगर (!म_भेद(कुंजी, "forced-update"))
				report->forced_update = 1;
			अन्यथा अगर (!म_भेद(कुंजी, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			जारी;
		}

		report = शून्य;
		new_report = 0;
		refname = p;
		p = म_अक्षर(refname, ' ');
		अगर (p)
			*p++ = '\0';
		अगर (म_भेद(शीर्ष, "ok") && म_भेद(शीर्ष, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    शीर्ष, refname);
			code = -1;
			जारी;
		}

		/* first try searching at our hint, falling back to all refs */
		अगर (hint)
			hint = find_command_by_refname(hint, refname);
		अगर (!hint)
			hint = find_command_by_refname(commands, refname);
		अगर (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			जारी;
		}
		अगर (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			जारी;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		अगर (!म_भेद(शीर्ष, "ng")) {
			अगर (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			अन्यथा
				hint->error_string = "failed";
			code = -1;
			जारी;
		}
		new_report = 1;
	}

	क्रम (cmd = commands; cmd; cmd = cmd->अग्र)
		अगर (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	वापस code;
}

अटल पूर्णांक run_proc_receive_hook(काष्ठा command *commands,
				 स्थिर काष्ठा string_list *धकेलो_options)
{
	काष्ठा child_process proc = CHILD_PROCESS_INIT;
	काष्ठा async muxer;
	काष्ठा command *cmd;
	काष्ठा packet_reader reader;
	काष्ठा strbuf cap = STRBUF_INIT;
	काष्ठा strbuf errmsg = STRBUF_INIT;
	पूर्णांक hook_use_धकेलो_options = 0;
	पूर्णांक version = 0;
	पूर्णांक code;
	स्थिर अक्षर *hook_path = find_hook(the_repository, "proc-receive");

	अगर (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		वापस -1;
	}

	strvec_धकेलो(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	अगर (use_sideband) {
		स_रखो(&muxer, 0, माप(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		अगर (code)
			वापस code;
		proc.err = muxer.in;
	} अन्यथा {
		proc.err = 0;
	}

	code = start_command(&proc);
	अगर (code) {
		अगर (use_sideband)
			finish_async(&muxer);
		वापस code;
	}

	sigchain_धकेलो(SIGPIPE, संक_छोड़ो);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, शून्य, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	अगर (use_atomic)
		strbuf_addstr(&cap, " atomic");
	अगर (use_धकेलो_options)
		strbuf_addstr(&cap, " push-options");
	अगर (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} अन्यथा {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	अगर (!code)
		code = packet_flush_gently(proc.in);

	अगर (!code)
		क्रम (;;) {
			पूर्णांक linelen;
			क्रमागत packet_read_status status;

			status = packet_reader_read(&reader);
			अगर (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				अगर (status == PACKET_READ_EOF)
					code = -1;
				अवरोध;
			}

			अगर (reader.pktlen > 8 && starts_with(reader.पंक्ति, "version=")) {
				version = म_से_प(reader.पंक्ति + 8);
				linelen = म_माप(reader.पंक्ति);
				अगर (linelen < reader.pktlen) {
					स्थिर अक्षर *feature_list = reader.पंक्ति + linelen + 1;
					अगर (parse_feature_request(feature_list, "push-options"))
						hook_use_धकेलो_options = 1;
				}
			}
		}

	अगर (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		जाओ cleanup;
	}

	चयन (version) {
	हाल 0:
		/* fallthrough */
	हाल 1:
		अवरोध;
	शेष:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		जाओ cleanup;
	}

	/* Send commands */
	क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
		अगर (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			जारी;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		अगर (code)
			अवरोध;
	}
	अगर (!code)
		code = packet_flush_gently(proc.in);
	अगर (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		जाओ cleanup;
	}

	/* Send push options */
	अगर (hook_use_धकेलो_options) {
		काष्ठा string_list_item *item;

		for_each_string_list_item(item, धकेलो_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			अगर (code)
				अवरोध;
		}
		अगर (!code)
			code = packet_flush_gently(proc.in);
		अगर (code) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			जाओ cleanup;
		}
	}

	/* Read result from proc-receive */
	code = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	बंद(proc.in);
	बंद(proc.out);
	अगर (use_sideband)
		finish_async(&muxer);
	अगर (finish_command(&proc))
		code = -1;
	अगर (errmsg.len >0) {
		अक्षर *p = errmsg.buf;

		p += errmsg.len - 1;
		अगर (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_निकालो(SIGPIPE);

	वापस code;
}

अटल स्थिर अक्षर *refuse_unconfigured_deny_संदेश =
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

अटल व्योम refuse_unconfigured_deny(व्योम)
{
	rp_error("%s", _(refuse_unconfigured_deny_संदेश));
}

अटल स्थिर अक्षर *refuse_unconfigured_deny_delete_current_संदेश =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

अटल व्योम refuse_unconfigured_deny_delete_current(व्योम)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_संदेश));
}

अटल स्थिर काष्ठा object_id *command_singleton_iterator(व्योम *cb_तथ्य);
अटल पूर्णांक update_shallow_ref(काष्ठा command *cmd, काष्ठा shallow_info *si)
{
	काष्ठा shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	काष्ठा oid_array extra = OID_ARRAY_INIT;
	काष्ठा check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->सूचकांक % 32);
	पूर्णांक i;

	trace_printf_कुंजी(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	क्रम (i = 0; i < si->shallow->nr; i++)
		अगर (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->सूचकांक / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	अगर (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		वापस -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	क्रम (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->सूचकांक] = 0;
	oid_array_clear(&extra);
	वापस 0;
}

अटल स्थिर अक्षर *धकेलो_to_deploy(अचिन्हित अक्षर *sha1,
				  काष्ठा strvec *env,
				  स्थिर काष्ठा worktree *worktree)
{
	काष्ठा child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", शून्य);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	अगर (run_command(&child))
		वापस "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", शून्य);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	अगर (run_command(&child))
		वापस "Working directory has unstaged changes";

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
		     !is_null_oid(&worktree->शीर्ष_oid) ? "HEAD" :
		     empty_वृक्ष_oid_hex(the_repository->hash_algo), "--", शून्य);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	अगर (run_command(&child))
		वापस "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     शून्य);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	अगर (run_command(&child))
		वापस "Could not update working tree to new HEAD";

	वापस शून्य;
}

अटल स्थिर अक्षर *धकेलो_to_checkout_hook = "push-to-checkout";

अटल स्थिर अक्षर *धकेलो_to_checkout(अचिन्हित अक्षर *hash,
				    पूर्णांक *invoked_hook,
				    काष्ठा strvec *env,
				    स्थिर अक्षर *work_वृक्ष)
{
	काष्ठा run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_वृक्ष));
	strvec_धकेलो(&opt.args, hash_to_hex(hash));
	अगर (run_hooks_opt(the_repository, धकेलो_to_checkout_hook, &opt))
		वापस "push-to-checkout hook declined";
	अन्यथा
		वापस शून्य;
}

अटल स्थिर अक्षर *update_worktree(अचिन्हित अक्षर *sha1, स्थिर काष्ठा worktree *worktree)
{
	स्थिर अक्षर *retval;
	अक्षर *git_dir;
	काष्ठा strvec env = STRVEC_INIT;
	पूर्णांक invoked_hook;

	अगर (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	अगर (worktree->is_bare)
		वापस "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = धकेलो_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	अगर (!invoked_hook)
		retval = धकेलो_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	मुक्त(git_dir);
	वापस retval;
}

अटल स्थिर अक्षर *update(काष्ठा command *cmd, काष्ठा shallow_info *si)
{
	स्थिर अक्षर *name = cmd->ref_name;
	काष्ठा strbuf namespaced_name_buf = STRBUF_INIT;
	अटल अक्षर *namespaced_name;
	स्थिर अक्षर *ret;
	काष्ठा object_id *old_oid = &cmd->old_oid;
	काष्ठा object_id *new_oid = &cmd->new_oid;
	पूर्णांक do_update_worktree = 0;
	काष्ठा worktree **worktrees = get_worktrees(the_repository);
	स्थिर काष्ठा worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	अगर (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		जाओ out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	मुक्त(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, शून्य);

	अगर (worktree && !worktree->is_bare) {
		चयन (deny_current_branch) {
		हाल DENY_IGNORE:
			अवरोध;
		हाल DENY_WARN:
			rp_warning("updating the current branch");
			अवरोध;
		हाल DENY_REFUSE:
		हाल DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			अगर (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			जाओ out;
		हाल DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			अवरोध;
		}
	}

	अगर (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		जाओ out;
	}

	अगर (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		अगर (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			जाओ out;
		}

		अगर (worktree || (शीर्ष_name && !म_भेद(namespaced_name, शीर्ष_name))) {
			चयन (deny_delete_current) {
			हाल DENY_IGNORE:
				अवरोध;
			हाल DENY_WARN:
				rp_warning("deleting the current branch");
				अवरोध;
			हाल DENY_REFUSE:
			हाल DENY_UNCONFIGURED:
			हाल DENY_UPDATE_INSTEAD:
				अगर (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				जाओ out;
			शेष:
				ret = "Invalid denyDeleteCurrent setting";
				जाओ out;
			}
		}
	}

	अगर (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		काष्ठा object *old_object, *new_object;
		काष्ठा commit *old_commit, *new_commit;
		पूर्णांक ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		अगर (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			जाओ out;
		}
		old_commit = (काष्ठा commit *)old_object;
		new_commit = (काष्ठा commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		अगर (ret2 < 0)
			निकास(128);
		अगर (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			जाओ out;
		}
	}
	अगर (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		जाओ out;
	}

	अगर (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		अगर (ret)
			जाओ out;
	}

	अगर (is_null_oid(new_oid)) {
		काष्ठा strbuf err = STRBUF_INIT;
		अगर (!parse_object(the_repository, old_oid)) {
			old_oid = शून्य;
			अगर (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} अन्यथा {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		अगर (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   शून्य, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} अन्यथा {
			ret = शून्य; /* good */
		}
		strbuf_release(&err);
	} अन्यथा {
		क्रमागत ref_transaction_error tx_err;
		काष्ठा strbuf err = STRBUF_INIT;
		अगर (shallow_update && si->shallow_ref[cmd->सूचकांक] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			जाओ out;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  शून्य, शून्य,
						  0, "push",
						  &err);
		अगर (tx_err) {
			rp_error("%s", err.buf);
			अगर (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			अन्यथा
				ret = ref_transaction_error_संदेश(tx_err);
		} अन्यथा {
			ret = शून्य; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	वापस ret;
}

अटल व्योम run_update_post_hook(काष्ठा command *commands)
{
	अटल स्थिर अक्षर hook_name[] = "post-update";
	काष्ठा run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	काष्ठा async sideband_async;
	काष्ठा command *cmd;
	पूर्णांक sideband_async_started = 0;
	पूर्णांक saved_stderr = -1;

	अगर (!hook_exists(the_repository, hook_name))
		वापस;

	क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
		अगर (cmd->error_string || cmd->did_not_exist)
			जारी;
		strvec_धकेलो(&opt.args, cmd->ref_name);
	}
	अगर (!opt.args.nr)
		वापस;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

अटल व्योम check_aliased_update_internal(काष्ठा command *cmd,
					  काष्ठा string_list *list,
					  स्थिर अक्षर *dst_name, पूर्णांक flag)
{
	काष्ठा string_list_item *item;
	काष्ठा command *dst_cmd;

	अगर (!(flag & REF_ISSYMREF))
		वापस;

	अगर (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		वापस;
	}
	dst_name = strip_namespace(dst_name);

	अगर (!(item = string_list_lookup(list, dst_name)))
		वापस;

	cmd->skip_update = 1;

	dst_cmd = (काष्ठा command *) item->util;

	अगर (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		वापस;

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

अटल व्योम check_aliased_update(काष्ठा command *cmd, काष्ठा string_list *list)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	स्थिर अक्षर *dst_name;
	पूर्णांक flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, शून्य, &flag);
	check_aliased_update_internal(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

अटल व्योम check_aliased_updates(काष्ठा command *commands)
{
	काष्ठा command *cmd;
	काष्ठा string_list ref_list = STRING_LIST_INIT_NODUP;

	क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
		काष्ठा string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (व्योम *)cmd;
	}
	string_list_sort(&ref_list);

	क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
		अगर (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

अटल स्थिर काष्ठा object_id *command_singleton_iterator(व्योम *cb_तथ्य)
{
	काष्ठा command **cmd_list = cb_तथ्य;
	काष्ठा command *cmd = *cmd_list;

	अगर (!cmd || is_null_oid(&cmd->new_oid))
		वापस शून्य;
	*cmd_list = शून्य; /* this returns only one */
	वापस &cmd->new_oid;
}

अटल व्योम set_connectivity_errors(काष्ठा command *commands,
				    काष्ठा shallow_info *si,
				    काष्ठा odb_transaction *transaction)
{
	काष्ठा command *cmd;

	क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
		काष्ठा command *singleton = cmd;
		काष्ठा check_connected_options opt = CHECK_CONNECTED_INIT;
		काष्ठा strvec env = STRVEC_INIT;

		अगर (shallow_update && si->shallow_ref[cmd->सूचकांक])
			/* to be checked in update_shallow_ref() */
			जारी;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		अगर (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			जारी;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

काष्ठा iterate_तथ्य {
	काष्ठा command *cmds;
	काष्ठा shallow_info *si;
};

अटल स्थिर काष्ठा object_id *iterate_receive_command_list(व्योम *cb_तथ्य)
{
	काष्ठा iterate_तथ्य *तथ्य = cb_तथ्य;
	काष्ठा command **cmd_list = &तथ्य->cmds;
	काष्ठा command *cmd = *cmd_list;

	क्रम (; cmd; cmd = cmd->अग्र) {
		अगर (shallow_update && तथ्य->si->shallow_ref[cmd->सूचकांक])
			/* to be checked in update_shallow_ref() */
			जारी;
		अगर (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->अग्र;
			वापस &cmd->new_oid;
		}
	}
	वापस शून्य;
}

अटल व्योम reject_updates_to_hidden(काष्ठा command *commands)
{
	काष्ठा strbuf refname_full = STRBUF_INIT;
	माप_प्रकार prefix_len;
	काष्ठा command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
		अगर (cmd->error_string)
			जारी;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		अगर (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			जारी;
		अगर (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		अन्यथा
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

अटल पूर्णांक should_process_cmd(काष्ठा command *cmd)
{
	वापस !cmd->error_string && !cmd->skip_update;
}

अटल व्योम BUG_if_skipped_connectivity_check(काष्ठा command *commands,
					       काष्ठा shallow_info *si)
{
	काष्ठा command *cmd;

	क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
		अगर (should_process_cmd(cmd) && si->shallow_ref[cmd->सूचकांक])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

अटल व्योम ref_transaction_rejection_handler(स्थिर अक्षर *refname,
					      स्थिर काष्ठा object_id *old_oid UNUSED,
					      स्थिर काष्ठा object_id *new_oid UNUSED,
					      स्थिर अक्षर *old_target UNUSED,
					      स्थिर अक्षर *new_target UNUSED,
					      क्रमागत ref_transaction_error err,
					      स्थिर अक्षर *details,
					      व्योम *cb_तथ्य)
{
	काष्ठा strmap *failed_refs = cb_तथ्य;

	अगर (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (अक्षर *)ref_transaction_error_संदेश(err));
}

अटल व्योम execute_commands_non_atomic(काष्ठा command *commands,
					काष्ठा shallow_info *si)
{
	काष्ठा command *cmd;
	काष्ठा strbuf err = STRBUF_INIT;
	स्थिर अक्षर *reported_error = शून्य;
	काष्ठा strmap failed_refs = STRMAP_INIT;

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
	क्रमागत processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	क्रम (क्रमागत processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
			अगर (!should_process_cmd(cmd) || cmd->run_proc_receive)
				जारी;

			अगर (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				जारी;
			अन्यथा अगर (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				जारी;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			अगर (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				अगर (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					जाओ failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		अगर (!transaction)
			जाओ cleanup;

		अगर (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			जाओ failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		अगर (strmap_empty(&failed_refs))
			जाओ cleanup;

	failure:
		क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
			अगर (reported_error)
				cmd->error_string = reported_error;
			अन्यथा अगर (strmap_contains(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = शून्य;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

अटल व्योम execute_commands_atomic(काष्ठा command *commands,
					काष्ठा shallow_info *si)
{
	काष्ठा command *cmd;
	काष्ठा strbuf err = STRBUF_INIT;
	स्थिर अक्षर *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	अगर (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		जाओ failure;
	}

	क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
		अगर (!should_process_cmd(cmd) || cmd->run_proc_receive)
			जारी;

		cmd->error_string = update(cmd, si);

		अगर (cmd->error_string)
			जाओ failure;
	}

	अगर (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		जाओ failure;
	}
	जाओ cleanup;

failure:
	क्रम (cmd = commands; cmd; cmd = cmd->अग्र)
		अगर (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

अटल व्योम execute_commands(काष्ठा command *commands,
			     पूर्णांक unpacker_error,
			     काष्ठा shallow_info *si,
			     काष्ठा odb_transaction *transaction,
			     स्थिर काष्ठा string_list *धकेलो_options)
{
	काष्ठा check_connected_options opt = CHECK_CONNECTED_INIT;
	काष्ठा command *cmd;
	काष्ठा iterate_तथ्य तथ्य;
	काष्ठा async muxer;
	पूर्णांक err_fd = 0;
	पूर्णांक run_proc_receive = 0;

	अगर (unpacker_error) {
		क्रम (cmd = commands; cmd; cmd = cmd->अग्र)
			cmd->error_string = "unpacker error";
		वापस;
	}

	अगर (!skip_connectivity_check) {
		काष्ठा strvec env = STRVEC_INIT;

		अगर (use_sideband) {
			स_रखो(&muxer, 0, माप(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			अगर (!start_async(&muxer))
				err_fd = muxer.in;
			/* ...else, continue without relaying sideband */
		}

		तथ्य.cmds = commands;
		तथ्य.si = si;
		opt.err_fd = err_fd;
		opt.progress = err_fd && !quiet;
		odb_transaction_env(transaction, &env);
		opt.env = env.v;
		opt.exclude_hidden_refs_section = "receive";

		अगर (check_connected(iterate_receive_command_list, &तथ्य, &opt))
			set_connectivity_errors(commands, si, transaction);

		अगर (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	अगर (proc_receive_ref) {
		क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
			अगर (!should_process_cmd(cmd))
				जारी;

			अगर (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	अगर (run_receive_hook(commands, "pre-receive", 0, transaction, धकेलो_options)) {
		क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
			अगर (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		वापस;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	क्रम (cmd = commands; cmd && cmd->error_string; cmd = cmd->अग्र)
		; /* nothing */
	अगर (!cmd)
		वापस;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	अगर (odb_transaction_commit(transaction)) {
		क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
			अगर (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		वापस;
	}

	check_aliased_updates(commands);

	मुक्त(शीर्ष_name_to_free);
	शीर्ष_name = शीर्ष_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, शून्य,
							    शून्य);

	अगर (run_proc_receive &&
	    run_proc_receive_hook(commands, धकेलो_options))
		क्रम (cmd = commands; cmd; cmd = cmd->अग्र)
			अगर (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	अगर (use_atomic)
		execute_commands_atomic(commands, si);
	अन्यथा
		execute_commands_non_atomic(commands, si);

	अगर (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

अटल काष्ठा command **queue_command(काष्ठा command **पुच्छ,
				      स्थिर अक्षर *पंक्ति,
				      पूर्णांक linelen)
{
	काष्ठा object_id old_oid, new_oid;
	काष्ठा command *cmd;
	स्थिर अक्षर *refname;
	पूर्णांक reflen;
	स्थिर अक्षर *p;

	अगर (parse_oid_hex(पंक्ति, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", पंक्ति);

	refname = p;
	reflen = linelen - (p - पंक्ति);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*पुच्छ = cmd;
	वापस &cmd->अग्र;
}

अटल व्योम free_commands(काष्ठा command *commands)
{
	जबतक (commands) {
		काष्ठा command *अग्र = commands->अग्र;

		ref_धकेलो_report_free(commands->report);
		मुक्त(commands->error_string_owned);
		मुक्त(commands);
		commands = अग्र;
	}
}

अटल व्योम queue_commands_from_cert(काष्ठा command **पुच्छ,
				     काष्ठा strbuf *धकेलो_cert)
{
	स्थिर अक्षर *boc, *eoc;

	अगर (*पुच्छ)
		die("protocol error: got both push certificate and unsigned commands");

	boc = म_माला(धकेलो_cert->buf, "\n\n");
	अगर (!boc)
		die("malformed push certificate %.*s", 100, धकेलो_cert->buf);
	अन्यथा
		boc += 2;
	eoc = धकेलो_cert->buf + parse_signed_बफर(धकेलो_cert->buf, धकेलो_cert->len);

	जबतक (boc < eoc) {
		स्थिर अक्षर *eol = स_प्रथम(boc, '\n', eoc - boc);
		पुच्छ = queue_command(पुच्छ, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

अटल काष्ठा command *read_शीर्ष_info(काष्ठा packet_reader *reader,
				      काष्ठा oid_array *shallow,
				      क्रमागत report_status_version *version)
{
	काष्ठा command *commands = शून्य;
	काष्ठा command **p = &commands;
	क्रम (;;) {
		पूर्णांक linelen;

		अगर (packet_reader_read(reader) != PACKET_READ_NORMAL)
			अवरोध;

		अगर (reader->pktlen > 8 && starts_with(reader->पंक्ति, "shallow ")) {
			काष्ठा object_id oid;
			अगर (get_oid_hex(reader->पंक्ति + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->पंक्ति + 8);
			oid_array_append(shallow, &oid);
			जारी;
		}

		linelen = म_माप(reader->पंक्ति);
		अगर (linelen < reader->pktlen) {
			स्थिर अक्षर *feature_list = reader->पंक्ति + linelen + 1;
			स्थिर अक्षर *hash = शून्य;
			स्थिर अक्षर *client_sid;
			माप_प्रकार len = 0;
			अगर (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			अगर (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			अगर (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			अगर (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			अगर (advertise_atomic_धकेलो
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			अगर (advertise_धकेलो_options
			    && parse_feature_request(feature_list, "push-options"))
				use_धकेलो_options = 1;
			hash = parse_feature_मूल्य(feature_list, "object-format", &len, शून्य);
			अगर (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = म_माप(hash);
			}
			अगर (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_मूल्य(feature_list, "session-id", &len, शून्य);
			अगर (client_sid) {
				अक्षर *sid = xstrndup(client_sid, len);
				trace2_तथ्य_string("transfer", शून्य, "client-sid", client_sid);
				मुक्त(sid);
			}
		}

		अगर (!म_भेद(reader->पंक्ति, "push-cert")) {
			पूर्णांक true_flush = 0;
			पूर्णांक saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			क्रम (;;) {
				packet_reader_read(reader);
				अगर (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					अवरोध;
				}
				अगर (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				अगर (!म_भेद(reader->पंक्ति, "push-cert-end\n"))
					अवरोध; /* end of cert */
				strbuf_addstr(&धकेलो_cert, reader->पंक्ति);
			}
			reader->options = saved_options;

			अगर (true_flush)
				अवरोध;
			जारी;
		}

		p = queue_command(p, reader->पंक्ति, linelen);
	}

	अगर (धकेलो_cert.len)
		queue_commands_from_cert(p, &धकेलो_cert);

	वापस commands;
}

अटल व्योम read_धकेलो_options(काष्ठा packet_reader *reader,
			      काष्ठा string_list *options)
{
	जबतक (1) {
		अगर (packet_reader_read(reader) != PACKET_READ_NORMAL)
			अवरोध;

		string_list_append(options, reader->पंक्ति);
	}
}

अटल पूर्णांक unpack_with_sideband(काष्ठा odb_transaction *transaction,
				स्थिर अक्षर *shallow_file,
				काष्ठा strbuf *err_संदेश)
{
	काष्ठा odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_संदेश_types = fsck_संदेश_types.buf,
		.max_निवेश_आकार = max_निवेश_आकार,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	काष्ठा async muxer;
	पूर्णांक ret;

	अगर (!use_sideband)
		वापस odb_transaction_write_pack(transaction, 0, err_संदेश, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	स_रखो(&muxer, 0, माप(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	अगर (start_async(&muxer))
		वापस 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_संदेश, &opts);

	finish_async(&muxer);
	वापस ret;
}

अटल व्योम prepare_shallow_update(काष्ठा shallow_info *si,
				   स्थिर अक्षर *shallow_file)
{
	पूर्णांक i, j, k, bitmap_आकार = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, शून्य);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	क्रम (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	क्रम (i = 0; i < si->shallow->nr; i++) {
		अगर (!si->used_shallow[i])
			जारी;
		क्रम (j = 0; j < bitmap_आकार; j++) {
			अगर (!si->used_shallow[i][j])
				जारी;
			si->need_reachability_test[i]++;
			क्रम (k = 0; k < 32; k++)
				अगर (si->used_shallow[i][j] & (1U << k))
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

अटल व्योम update_shallow_info(काष्ठा command *commands,
				काष्ठा shallow_info *si,
				काष्ठा oid_array *ref,
				स्थिर अक्षर *shallow_file)
{
	काष्ठा command *cmd;
	पूर्णांक *ref_status;
	remove_nonexistent_theirs_shallow(si);
	अगर (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		वापस;
	}

	क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
		अगर (is_null_oid(&cmd->new_oid))
			जारी;
		oid_array_append(ref, &cmd->new_oid);
		cmd->सूचकांक = ref->nr - 1;
	}
	si->ref = ref;

	अगर (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		वापस;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, शून्य, ref_status);
	क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
		अगर (is_null_oid(&cmd->new_oid))
			जारी;
		अगर (ref_status[cmd->सूचकांक]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	मुक्त(ref_status);
}

अटल व्योम override_cmds_error(काष्ठा command *commands, स्थिर अक्षर *err)
{
	क्रम (काष्ठा command *cmd = commands; cmd; cmd = cmd->अग्र) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
अटल व्योम generate_report(काष्ठा strbuf *buf, काष्ठा command *commands,
			    स्थिर काष्ठा strbuf *unpack_status,
			    क्रमागत report_status_version version)
{
	काष्ठा command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
		काष्ठा ref_धकेलो_report *report;
		पूर्णांक गिनती = 0;

		अगर (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		अन्यथा
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		अगर (version != REPORT_STATUS_V2 || cmd->error_string)
			जारी;

		क्रम (report = cmd->report; report; report = report->अग्र) {
			अगर (गिनती++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			अगर (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			अगर (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			अगर (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			अगर (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

अटल व्योम report(काष्ठा command *commands, स्थिर काष्ठा strbuf *unpack_status,
		   क्रमागत report_status_version version)
{
	काष्ठा strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	अगर (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	अगर (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	अन्यथा
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

अटल पूर्णांक delete_only(काष्ठा command *commands)
{
	काष्ठा command *cmd;
	क्रम (cmd = commands; cmd; cmd = cmd->अग्र) {
		अगर (!is_null_oid(&cmd->new_oid))
			वापस 0;
	}
	वापस 1;
}

पूर्णांक cmd_receive_pack(पूर्णांक argc,
		     स्थिर अक्षर **argv,
		     स्थिर अक्षर *prefix,
		     काष्ठा repository *repo UNUSED)
{
	पूर्णांक advertise_refs = 0;
	काष्ठा command *commands;
	काष्ठा oid_array shallow = OID_ARRAY_INIT;
	काष्ठा oid_array ref = OID_ARRAY_INIT;
	काष्ठा shallow_info si;
	काष्ठा packet_reader reader;
	काष्ठा odb_transaction *transaction = शून्य;
	क्रमागत report_status_version version = REPORT_STATUS_UNKNOWN;

	काष्ठा option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, शून्य),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, शून्य),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, शून्य),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, शून्य),
		OPT_END()
	};

	packet_trace_identity("receive-pack");

	argc = parse_options(argc, argv, prefix, options, receive_pack_usage, 0);

	अगर (argc > 1)
		usage_संदेश_opt(_("too many arguments"), receive_pack_usage, options);
	अगर (argc == 0)
		usage_संदेश_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	अगर (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, शून्य);
	अगर (cert_nonce_seed)
		धकेलो_cert_nonce = prepare_धकेलो_cert_nonce(service_dir, समय(शून्य));

	चयन (determine_protocol_version_server()) {
	हाल protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		अवरोध;
	हाल protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		अगर (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	हाल protocol_v0:
		अवरोध;
	हाल protocol_unknown_version:
		BUG("unknown protocol version");
	}

	अगर (advertise_refs || !stateless_rpc) {
		write_शीर्ष_info();
	}
	अगर (advertise_refs)
		वापस 0;

	packet_reader_init(&reader, 0, शून्य, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	अगर ((commands = read_शीर्ष_info(&reader, &shallow, &version))) {
		काष्ठा string_list धकेलो_options = STRING_LIST_INIT_DUP;
		काष्ठा strbuf unpack_status = STRBUF_INIT;

		अगर (use_धकेलो_options)
			read_धकेलो_options(&reader, &धकेलो_options);
		अगर (!check_cert_धकेलो_options(&धकेलो_options)) {
			काष्ठा command *cmd;
			क्रम (cmd = commands; cmd; cmd = cmd->अग्र)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		अगर (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		अगर (!delete_only(commands)) {
			स्थिर अक्षर *alt_shallow_file = शून्य;

			अगर (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			अगर (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			अन्यथा
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &धकेलो_options);
		odb_transaction_finalize(transaction);
		sigchain_धकेलो(SIGPIPE, संक_छोड़ो);

		चयन (version) {
		हाल REPORT_STATUS_V2:
		हाल REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			अवरोध;
		हाल REPORT_STATUS_UNKNOWN:
			अवरोध;
		}

		sigchain_निकालो(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, शून्य,
				 &धकेलो_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&धकेलो_options, 0);
		अगर (auto_gc) {
			काष्ठा child_process proc = CHILD_PROCESS_INIT;

			अगर (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				अगर (!start_command(&proc)) {
					अगर (use_sideband)
						copy_to_sideband(proc.err, -1, शून्य);
					finish_command(&proc);
				}
			}
		}
		अगर (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	अगर (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	मुक्त((व्योम *)धकेलो_cert_nonce);
	वापस 0;
}
