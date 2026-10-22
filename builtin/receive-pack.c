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

स्थिर ध्रुव वर्ण * ध्रुव receive_pack_usage[] = {
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

स्थिर पूर्णाङ्क deny_deletes;
स्थिर पूर्णाङ्क deny_non_fast_forwards;
स्थिर गणना deny_action deny_current_branch = DENY_UNCONFIGURED;
स्थिर गणना deny_action deny_delete_current = DENY_UNCONFIGURED;
स्थिर पूर्णाङ्क receive_fsck_objects = -1;
स्थिर पूर्णाङ्क transfer_fsck_objects = -1;
स्थिर संरचना strbuf fsck_संदेश_types = STRBUF_INIT;
स्थिर पूर्णाङ्क advertise_atomic_ढकला = 1;
स्थिर पूर्णाङ्क advertise_ढकला_options;
स्थिर पूर्णाङ्क advertise_sid;
स्थिर off_t max_आदान_size;
स्थिर पूर्णाङ्क use_sideband;
स्थिर पूर्णाङ्क use_atomic;
स्थिर पूर्णाङ्क use_ढकला_options;
स्थिर पूर्णाङ्क quiet;
स्थिर पूर्णाङ्क prefer_ofs_delta = 1;
स्थिर पूर्णाङ्क auto_update_server_info;
स्थिर पूर्णाङ्क auto_gc = 1;
स्थिर पूर्णाङ्क reject_thin;
स्थिर पूर्णाङ्क skip_connectivity_check;
स्थिर पूर्णाङ्क stateless_rpc;
स्थिर ध्रुव वर्ण *service_dir;
स्थिर ध्रुव वर्ण *head_name;
स्थिर शून्य *head_name_to_free;
स्थिर पूर्णाङ्क sent_capabilities;
स्थिर पूर्णाङ्क shallow_update;
स्थिर संरचना strbuf ढकला_cert = STRBUF_INIT;
स्थिर संरचना object_id ढकला_cert_oid;
स्थिर संरचना signature_check sigcheck;
स्थिर ध्रुव वर्ण *ढकला_cert_nonce;
स्थिर वर्ण *cert_nonce_seed;
स्थिर संरचना strvec hidden_refs = STRVEC_INIT;

स्थिर ध्रुव वर्ण *NONCE_UNSOLICITED = "UNSOLICITED";
स्थिर ध्रुव वर्ण *NONCE_BAD = "BAD";
स्थिर ध्रुव वर्ण *NONCE_MISSING = "MISSING";
स्थिर ध्रुव वर्ण *NONCE_OK = "OK";
स्थिर ध्रुव वर्ण *NONCE_SLOP = "SLOP";
स्थिर ध्रुव वर्ण *nonce_status;
स्थिर long nonce_stamp_slop;
स्थिर timestamp_t nonce_stamp_slop_limit;
स्थिर संरचना ref_transaction *transaction;

स्थिर गणना {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
स्थिर पूर्णाङ्क keepalive_in_sec = 5;

स्थिर संरचना proc_receive_ref {
	unsigned पूर्णाङ्क want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	वर्ण *ref_prefix;
	संरचना proc_receive_ref *next;
} *proc_receive_ref;

स्थिर शून्य proc_receive_ref_append(ध्रुव वर्ण *prefix);

स्थिर गणना deny_action parse_deny_action(ध्रुव वर्ण *var, ध्रुव वर्ण *मूल्य)
{
	यदि (मूल्य) {
		यदि (!strcasecmp(मूल्य, "ignore"))
			प्रत्यागम DENY_IGNORE;
		यदि (!strcasecmp(मूल्य, "warn"))
			प्रत्यागम DENY_WARN;
		यदि (!strcasecmp(मूल्य, "refuse"))
			प्रत्यागम DENY_REFUSE;
		यदि (!strcasecmp(मूल्य, "updateinstead"))
			प्रत्यागम DENY_UPDATE_INSTEAD;
	}
	यदि (git_config_bool(var, मूल्य))
		प्रत्यागम DENY_REFUSE;
	प्रत्यागम DENY_IGNORE;
}

स्थिर पूर्णाङ्क receive_pack_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मूल्य,
			       ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	ध्रुव वर्ण *संदेश_id;
	पूर्णाङ्क status = parse_hide_refs_config(var, मूल्य, "receive", &hidden_refs);

	यदि (status)
		प्रत्यागम status;

	यदि (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.fsck.skiplist") == 0) {
		वर्ण *path;

		यदि (git_config_pathname(&path, var, मूल्य))
			प्रत्यागम -1;
		यदि (path)
			strbuf_addf(&fsck_संदेश_types, "%cskiplist=%s",
				    fsck_संदेश_types.len ? ',' : '=', path);
		मुक्त(path);
		प्रत्यागम 0;
	}

	यदि (skip_prefix(var, "receive.fsck.", &संदेश_id)) {
		यदि (!मूल्य)
			प्रत्यागम config_error_nonbool(var);
		यदि (is_valid_संदेश_type(संदेश_id, मूल्य))
			strbuf_addf(&fsck_संदेश_types, "%c%s=%s",
				fsck_संदेश_types.len ? ',' : '=', संदेश_id, मूल्य);
		अन्यथा
			warning("skipping unknown msg id '%s'", संदेश_id);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.certnonceseed") == 0)
		प्रत्यागम git_config_string(&cert_nonce_seed, var, मूल्य);

	यदि (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, मूल्य, ctx->kvi);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_ढकला = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_ढकला_options = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, मूल्य, ctx->kvi);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.maxinputsize") == 0) {
		max_आदान_size = git_config_int64(var, मूल्य, ctx->kvi);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "receive.procreceiverefs") == 0) {
		यदि (!मूल्य)
			प्रत्यागम config_error_nonbool(var);
		proc_receive_ref_append(मूल्य);
		प्रत्यागम 0;
	}

	यदि (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	प्रत्यागम git_default_config(var, मूल्य, ctx, cb);
}

स्थिर शून्य show_ref(ध्रुव वर्ण *path, ध्रुव संरचना object_id *oid)
{
	यदि (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} अन्यथा {
		संरचना strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		यदि (advertise_atomic_ढकला)
			strbuf_addstr(&cap, " atomic");
		यदि (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		यदि (ढकला_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", ढकला_cert_nonce);
		यदि (advertise_ढकला_options)
			strbuf_addstr(&cap, " push-options");
		यदि (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

स्थिर पूर्णाङ्क show_ref_cb(ध्रुव संरचना reference *ref, शून्य *data)
{
	संरचना oidset *seen = data;
	ध्रुव वर्ण *path = strip_namespace(ref->name);

	यदि (ref_is_hidden(path, ref->name, &hidden_refs))
		प्रत्यागम 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	यदि (!path) {
		यदि (oidset_घाला(seen, ref->oid))
			प्रत्यागम 0;
		path = ".have";
	} अन्यथा {
		oidset_घाला(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	प्रत्यागम 0;
}

स्थिर शून्य show_one_alternate_ref(ध्रुव संरचना object_id *oid,
				   शून्य *data)
{
	संरचना oidset *seen = data;

	यदि (oidset_घाला(seen, oid))
		प्रत्यागम;

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

	यदि (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#define RUN_PROC_RECEIVE_SCHEDULED	1
#define RUN_PROC_RECEIVE_RETURNED	2
संरचना command {
	संरचना command *next;
	ध्रुव वर्ण *error_string;
	वर्ण *error_string_owned;
	संरचना ref_ढकला_report *report;
	unsigned पूर्णाङ्क skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	पूर्णाङ्क index;
	संरचना object_id old_oid;
	संरचना object_id new_oid;
	वर्ण ref_name[FLEX_ARRAY]; /* more */
};

स्थिर शून्य proc_receive_ref_append(ध्रुव वर्ण *prefix)
{
	संरचना proc_receive_ref *ref_pattern;
	ध्रुव वर्ण *p;
	पूर्णाङ्क len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = strchr(prefix, ':');
	यदि (p) {
		यावत् (prefix < p) {
			यदि (*prefix == 'a')
				ref_pattern->want_add = 1;
			अन्यथा यदि (*prefix == 'd')
				ref_pattern->want_delete = 1;
			अन्यथा यदि (*prefix == 'm')
				ref_pattern->want_modify = 1;
			अन्यथा यदि (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} अन्यथा {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	यावत् (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	यदि (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} अन्यथा {
		संरचना proc_receive_ref *end;

		end = proc_receive_ref;
		यावत् (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

स्थिर पूर्णाङ्क proc_receive_ref_matches(संरचना command *cmd)
{
	संरचना proc_receive_ref *p;

	यदि (!proc_receive_ref)
		प्रत्यागम 0;

	प्रति (p = proc_receive_ref; p; p = p->next) {
		ध्रुव वर्ण *match = p->ref_prefix;
		ध्रुव वर्ण *remains;

		यदि (!p->want_add && is_null_oid(&cmd->old_oid))
			अनुवर्त;
		अन्यथा यदि (!p->want_delete && is_null_oid(&cmd->new_oid))
			अनुवर्त;
		अन्यथा यदि (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			अनुवर्त;

		यदि (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			यदि (!p->negative_ref)
				प्रत्यागम 1;
		} अन्यथा यदि (p->negative_ref) {
			प्रत्यागम 1;
		}
	}
	प्रत्यागम 0;
}

स्थिर शून्य report_message(ध्रुव वर्ण *prefix, ध्रुव वर्ण *err, va_list params)
{
	पूर्णाङ्क sz;
	वर्ण संदेश[4096];

	sz = xsnprintf(संदेश, परिमाण(संदेश), "%s", prefix);
	sz += vsnprintf(संदेश + sz, परिमाण(संदेश) - sz, err, params);
	यदि (sz > (परिमाण(संदेश) - 1))
		sz = परिमाण(संदेश) - 1;
	संदेश[sz++] = '\n';

	यदि (use_sideband)
		send_sideband(1, 2, संदेश, sz, use_sideband);
	अन्यथा
		xwrite(2, संदेश, sz);
}

__attribute__((format (printf, 1, 2)))
स्थिर शून्य rp_warning(ध्रुव वर्ण *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("warning: ", err, params);
	va_end(params);
}

__attribute__((format (printf, 1, 2)))
स्थिर शून्य rp_error(ध्रुव वर्ण *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("error: ", err, params);
	va_end(params);
}

स्थिर पूर्णाङ्क copy_to_sideband(पूर्णाङ्क in, पूर्णाङ्क out UNUSED, शून्य *arg UNUSED)
{
	वर्ण data[128];
	पूर्णाङ्क keepalive_active = 0;

	यदि (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	यदि (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	यावत् (1) {
		ssize_t sz;

		यदि (keepalive_active) {
			संरचना pollfd pfd;
			पूर्णाङ्क ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			यदि (ret < 0) {
				यदि (errno == EINTR)
					अनुवर्त;
				अन्यथा
					भङ्ग;
			} अन्यथा यदि (ret == 0) {
				/* no data; send a keepalive packet */
				स्थिर ध्रुव वर्ण buf[] = "0005\1";
				write_or_die(1, buf, परिमाण(buf) - 1);
				अनुवर्त;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, परिमाण(data));
		यदि (sz <= 0)
			भङ्ग;

		यदि (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			ध्रुव वर्ण *p = memchr(data, '\0', sz);
			यदि (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_active = 1;
				send_sideband(1, 2, data, p - data, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - data + 1), use_sideband);
				अनुवर्त;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, data, sz, use_sideband);
	}
	close(in);
	प्रत्यागम 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
स्थिर शून्य prepare_sideband_async(संरचना async *sideband_async, पूर्णाङ्क *saved_stderr, पूर्णाङ्क *started)
{
	*started = 0;

	यदि (!use_sideband)
		प्रत्यागम;

	memset(sideband_async, 0, परिमाण(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	यदि (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		यदि (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
स्थिर शून्य finish_sideband_async(संरचना async *sideband_async, पूर्णाङ्क saved_stderr, पूर्णाङ्क started)
{
	यदि (!use_sideband)
		प्रत्यागम;

	यदि (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	यदि (started)
		finish_async(sideband_async);
}

स्थिर शून्य hmac_hash(unsigned वर्ण *out,
		      ध्रुव वर्ण *किल्ली_in, size_t किल्ली_len,
		      ध्रुव वर्ण *text, size_t text_len)
{
	unsigned वर्ण किल्ली[GIT_MAX_BLKSZ];
	unsigned वर्ण k_ipad[GIT_MAX_BLKSZ];
	unsigned वर्ण k_opad[GIT_MAX_BLKSZ];
	पूर्णाङ्क i;
	संरचना git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(किल्ली, '\0', GIT_MAX_BLKSZ);
	यदि (the_hash_algo->blksz < किल्ली_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, किल्ली_in, किल्ली_len);
		git_hash_final(किल्ली, &ctx);
	} अन्यथा {
		memcpy(किल्ली, किल्ली_in, किल्ली_len);
	}

	/* RFC 2104 2. (2) & (5) */
	प्रति (i = 0; i < परिमाण(किल्ली); i++) {
		k_ipad[i] = किल्ली[i] ^ 0x36;
		k_opad[i] = किल्ली[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, परिमाण(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, परिमाण(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

स्थिर वर्ण *prepare_ढकला_cert_nonce(ध्रुव वर्ण *path, timestamp_t stamp)
{
	संरचना strbuf buf = STRBUF_INIT;
	unsigned वर्ण hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, strlen(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (पूर्णाङ्क)the_hash_algo->hexsz, hash_to_hex(hash));
	प्रत्यागम strbuf_detach(&buf, रिक्त);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
स्थिर पूर्णाङ्क constant_memequal(ध्रुव वर्ण *a, ध्रुव वर्ण *b, size_t n)
{
	पूर्णाङ्क res = 0;
	size_t i;

	प्रति (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	प्रत्यागम res;
}

स्थिर ध्रुव वर्ण *check_nonce(ध्रुव वर्ण *buf)
{
	size_t noncelen;
	ध्रुव वर्ण *found = find_commit_header(buf, "nonce", &noncelen);
	वर्ण *nonce = found ? xmemdupz(found, noncelen) : रिक्त;
	timestamp_t stamp, ostamp;
	वर्ण *bohmac, *expect = रिक्त;
	ध्रुव वर्ण *retval = NONCE_BAD;

	यदि (!nonce) {
		retval = NONCE_MISSING;
		गच्छ leave;
	} अन्यथा यदि (!ढकला_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		गच्छ leave;
	} अन्यथा यदि (!strcmp(ढकला_cert_nonce, nonce)) {
		retval = NONCE_OK;
		गच्छ leave;
	}

	यदि (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		गच्छ leave;
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
	यदि (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		गच्छ leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	यदि (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		गच्छ leave;
	}

	expect = prepare_ढकला_cert_nonce(service_dir, stamp);
	यदि (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		गच्छ leave;
	}
	यदि (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		गच्छ leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(ढकला_cert_nonce, रिक्त, 10);
	nonce_stamp_slop = (long)ostamp - (long)stamp;

	यदि (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
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
	प्रत्यागम retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
स्थिर पूर्णाङ्क check_cert_ढकला_options(ध्रुव संरचना string_list *ढकला_options)
{
	ध्रुव वर्ण *buf = ढकला_cert.buf;

	ध्रुव वर्ण *option;
	size_t optionlen;
	पूर्णाङ्क options_seen = 0;

	पूर्णाङ्क retval = 1;

	यदि (!*buf)
		प्रत्यागम 1;

	यावत् ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		यदि (options_seen > ढकला_options->nr
		    || xstrncmpz(ढकला_options->वस्तुसंच[options_seen - 1].string,
				 option, optionlen))
			प्रत्यागम 0;
	}

	यदि (options_seen != ढकला_options->nr)
		retval = 0;

	प्रत्यागम retval;
}

स्थिर शून्य prepare_ढकला_cert_sha1(संरचना run_hooks_opt *opt)
{
	स्थिर पूर्णाङ्क already_done;

	यदि (!ढकला_cert.len)
		प्रत्यागम;

	यदि (!already_done) {
		पूर्णाङ्क bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		यदि (odb_write_object(the_repository->objects, ढकला_cert.buf,
				     ढकला_cert.len, OBJ_BLOB, &ढकला_cert_oid))
			oidclr(&ढकला_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', परिमाण(sigcheck));

		bogs = parse_signed_buffer(ढकला_cert.buf, ढकला_cert.len);
		sigcheck.payload = xmemdupz(ढकला_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, ढकला_cert.buf + bogs,
				ढकला_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	यदि (!is_null_oid(&ढकला_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&ढकला_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.किल्ली ? sigcheck.किल्ली : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		यदि (ढकला_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     ढकला_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			यदि (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

संरचना receive_hook_feed_state {
	संरचना command *cmd;
	संरचना ref_ढकला_report *report;
	पूर्णाङ्क skip_broken;
	संरचना strbuf buf;
};

स्थिर पूर्णाङ्क feed_receive_hook_cb(पूर्णाङ्क hook_stdin_fd, शून्य *pp_cb UNUSED, शून्य *pp_task_cb)
{
	संरचना receive_hook_feed_state *state = pp_task_cb;
	संरचना command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	यावत् (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	यदि (!cmd)
		प्रत्यागम 1;  /* no more commands left */

	यदि (!state->report)
		state->report = cmd->report;

	यदि (state->report) {
		संरचना object_id *old_oid;
		संरचना object_id *new_oid;
		ध्रुव वर्ण *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->next;
		यदि (!state->report)
			cmd = cmd->next;
	} अन्यथा {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	यदि (state->buf.len > 0) {
		पूर्णाङ्क ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		यदि (ret < 0) {
			यदि (errno == EPIPE)
				प्रत्यागम 1; /* child closed pipe */
			प्रत्यागम ret;
		}
	}

	प्रत्यागम state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
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

	प्रत्यागम data;
}

स्थिर शून्य receive_hook_feed_state_free(शून्य *data)
{
	संरचना receive_hook_feed_state *d = data;
	यदि (!d)
		प्रत्यागम;
	strbuf_release(&d->buf);
	मुक्त(d);
}

स्थिर पूर्णाङ्क run_receive_hook(संरचना command *commands,
			    ध्रुव वर्ण *hook_name,
			    पूर्णाङ्क skip_broken,
			    संरचना odb_transaction *transaction,
			    ध्रुव संरचना string_list *ढकला_options)
{
	संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	संरचना command *iter = commands;
	संरचना receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	संरचना async sideband_async;
	पूर्णाङ्क sideband_async_started = 0;
	पूर्णाङ्क saved_stderr = -1;
	पूर्णाङ्क ret;

	यदि (!hook_exists(the_repository, hook_name))
		प्रत्यागम 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	यावत् (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	यदि (!iter)
		प्रत्यागम 0;

	यदि (ढकला_options) {
		प्रति (पूर्णाङ्क i = 0; i < ढकला_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     ढकला_options->वस्तुसंच[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)ढकला_options->nr);
	} अन्यथा {
		strvec_ढकला(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	यदि (transaction)
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

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क run_update_hook(संरचना command *cmd)
{
	स्थिर ध्रुव वर्ण hook_name[] = "update";
	संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	संरचना async sideband_async;
	पूर्णाङ्क sideband_async_started = 0;
	पूर्णाङ्क saved_stderr = -1;
	पूर्णाङ्क code;

	यदि (!hook_exists(the_repository, hook_name))
		प्रत्यागम 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     रिक्त);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	प्रत्यागम code;
}

स्थिर पूर्णाङ्क run_receive_report_hook(संरचना strbuf *report)
{
	संरचना child_process proc = CHILD_PROCESS_INIT;
	संरचना async sideband_async;
	पूर्णाङ्क sideband_async_started = 0;
	पूर्णाङ्क saved_stderr = -1;
	संरचना strbuf out = STRBUF_INIT;
	ध्रुव वर्ण *hook_path;
	पूर्णाङ्क ret;

	hook_path = find_hook(the_repository, "receive-report");
	यदि (!hook_path)
		प्रत्यागम 0;

	strvec_ढकला(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_ढकला(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, रिक्त, 0);
	sigchain_काढा(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	यदि (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	प्रत्यागम ret;
}

स्थिर संरचना command *find_command_by_refname(संरचना command *list,
					       ध्रुव वर्ण *refname)
{
	प्रति (; list; list = list->next)
		यदि (!strcmp(list->ref_name, refname))
			प्रत्यागम list;
	प्रत्यागम रिक्त;
}

स्थिर पूर्णाङ्क read_proc_receive_report(संरचना packet_reader *reader,
				    संरचना command *commands,
				    संरचना strbuf *errmsg)
{
	संरचना command *cmd;
	संरचना command *hint = रिक्त;
	संरचना ref_ढकला_report *report = रिक्त;
	पूर्णाङ्क new_report = 0;
	पूर्णाङ्क code = 0;
	पूर्णाङ्क once = 0;
	पूर्णाङ्क response = 0;

	प्रति (;;) {
		संरचना object_id old_oid, new_oid;
		वर्ण *head;
		वर्ण *refname;
		वर्ण *p;
		गणना packet_read_status status;

		status = packet_reader_read(reader);
		यदि (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			यदि (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				प्रत्यागम -1;
			}
			भङ्ग;
		}
		response++;

		head = reader->ओळ;
		p = strchr(head, ' ');
		यदि (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			अनुवर्त;
		}
		*p++ = '\0';
		यदि (!strcmp(head, "option")) {
			वर्ण *किल्ली;
			ध्रुव वर्ण *val;

			यदि (!hint || !(report || new_report)) {
				यदि (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				अनुवर्त;
			}
			यदि (new_report) {
				यदि (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} अन्यथा {
					report = hint->report;
					यावत् (report->next)
						report = report->next;
					report->next = xcalloc(1, परिमाण(संरचना ref_ढकला_report));
					report = report->next;
				}
				new_report = 0;
			}
			किल्ली = p;
			p = strchr(किल्ली, ' ');
			यदि (p)
				*p++ = '\0';
			val = p;
			यदि (!strcmp(किल्ली, "refname"))
				report->ref_name = xstrdup_or_null(val);
			अन्यथा यदि (!strcmp(किल्ली, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			अन्यथा यदि (!strcmp(किल्ली, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			अन्यथा यदि (!strcmp(किल्ली, "forced-update"))
				report->forced_update = 1;
			अन्यथा यदि (!strcmp(किल्ली, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			अनुवर्त;
		}

		report = रिक्त;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		यदि (p)
			*p++ = '\0';
		यदि (strcmp(head, "ok") && strcmp(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			अनुवर्त;
		}

		/* first try searching at our hint, falling back to all refs */
		यदि (hint)
			hint = find_command_by_refname(hint, refname);
		यदि (!hint)
			hint = find_command_by_refname(commands, refname);
		यदि (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			अनुवर्त;
		}
		यदि (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			अनुवर्त;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		यदि (!strcmp(head, "ng")) {
			यदि (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			अन्यथा
				hint->error_string = "failed";
			code = -1;
			अनुवर्त;
		}
		new_report = 1;
	}

	प्रति (cmd = commands; cmd; cmd = cmd->next)
		यदि (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	प्रत्यागम code;
}

स्थिर पूर्णाङ्क run_proc_receive_hook(संरचना command *commands,
				 ध्रुव संरचना string_list *ढकला_options)
{
	संरचना child_process proc = CHILD_PROCESS_INIT;
	संरचना async muxer;
	संरचना command *cmd;
	संरचना packet_reader reader;
	संरचना strbuf cap = STRBUF_INIT;
	संरचना strbuf errmsg = STRBUF_INIT;
	पूर्णाङ्क hook_use_ढकला_options = 0;
	पूर्णाङ्क version = 0;
	पूर्णाङ्क code;
	ध्रुव वर्ण *hook_path = find_hook(the_repository, "proc-receive");

	यदि (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		प्रत्यागम -1;
	}

	strvec_ढकला(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	यदि (use_sideband) {
		memset(&muxer, 0, परिमाण(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		यदि (code)
			प्रत्यागम code;
		proc.err = muxer.in;
	} अन्यथा {
		proc.err = 0;
	}

	code = start_command(&proc);
	यदि (code) {
		यदि (use_sideband)
			finish_async(&muxer);
		प्रत्यागम code;
	}

	sigchain_ढकला(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, रिक्त, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	यदि (use_atomic)
		strbuf_addstr(&cap, " atomic");
	यदि (use_ढकला_options)
		strbuf_addstr(&cap, " push-options");
	यदि (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} अन्यथा {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	यदि (!code)
		code = packet_flush_gently(proc.in);

	यदि (!code)
		प्रति (;;) {
			पूर्णाङ्क linelen;
			गणना packet_read_status status;

			status = packet_reader_read(&reader);
			यदि (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				यदि (status == PACKET_READ_EOF)
					code = -1;
				भङ्ग;
			}

			यदि (reader.pktlen > 8 && starts_with(reader.ओळ, "version=")) {
				version = atoi(reader.ओळ + 8);
				linelen = strlen(reader.ओळ);
				यदि (linelen < reader.pktlen) {
					ध्रुव वर्ण *feature_list = reader.ओळ + linelen + 1;
					यदि (parse_feature_request(feature_list, "push-options"))
						hook_use_ढकला_options = 1;
				}
			}
		}

	यदि (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		गच्छ cleanup;
	}

	विकल्प (version) {
	पक्ष 0:
		/* fallthrough */
	पक्ष 1:
		भङ्ग;
	स्वभाव:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		गच्छ cleanup;
	}

	/* Send commands */
	प्रति (cmd = commands; cmd; cmd = cmd->next) {
		यदि (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			अनुवर्त;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		यदि (code)
			भङ्ग;
	}
	यदि (!code)
		code = packet_flush_gently(proc.in);
	यदि (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		गच्छ cleanup;
	}

	/* Send push options */
	यदि (hook_use_ढकला_options) {
		संरचना string_list_item *item;

		for_each_string_list_item(item, ढकला_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			यदि (code)
				भङ्ग;
		}
		यदि (!code)
			code = packet_flush_gently(proc.in);
		यदि (code) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			गच्छ cleanup;
		}
	}

	/* Read result from proc-receive */
	code = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.in);
	close(proc.out);
	यदि (use_sideband)
		finish_async(&muxer);
	यदि (finish_command(&proc))
		code = -1;
	यदि (errmsg.len >0) {
		वर्ण *p = errmsg.buf;

		p += errmsg.len - 1;
		यदि (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_काढा(SIGPIPE);

	प्रत्यागम code;
}

स्थिर ध्रुव वर्ण *refuse_unconfigured_deny_संदेश =
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

स्थिर ध्रुव वर्ण *refuse_unconfigured_deny_delete_current_संदेश =
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

स्थिर ध्रुव संरचना object_id *command_singleton_iterator(शून्य *cb_data);
स्थिर पूर्णाङ्क update_shallow_ref(संरचना command *cmd, संरचना shallow_info *si)
{
	संरचना shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	संरचना oid_array extra = OID_ARRAY_INIT;
	संरचना check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	पूर्णाङ्क i;

	trace_printf_किल्ली(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	प्रति (i = 0; i < si->shallow->nr; i++)
		यदि (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	यदि (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		प्रत्यागम -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	प्रति (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *ढकला_to_deploy(unsigned वर्ण *sha1,
				  संरचना strvec *env,
				  ध्रुव संरचना worktree *worktree)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", रिक्त);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	यदि (run_command(&child))
		प्रत्यागम "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", रिक्त);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	यदि (run_command(&child))
		प्रत्यागम "Working directory has unstaged changes";

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
	यदि (run_command(&child))
		प्रत्यागम "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     रिक्त);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	यदि (run_command(&child))
		प्रत्यागम "Could not update working tree to new HEAD";

	प्रत्यागम रिक्त;
}

स्थिर ध्रुव वर्ण *ढकला_to_checkout_hook = "push-to-checkout";

स्थिर ध्रुव वर्ण *ढकला_to_checkout(unsigned वर्ण *hash,
				    पूर्णाङ्क *invoked_hook,
				    संरचना strvec *env,
				    ध्रुव वर्ण *work_वृक्ष)
{
	संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_वृक्ष));
	strvec_ढकला(&opt.args, hash_to_hex(hash));
	यदि (run_hooks_opt(the_repository, ढकला_to_checkout_hook, &opt))
		प्रत्यागम "push-to-checkout hook declined";
	अन्यथा
		प्रत्यागम रिक्त;
}

स्थिर ध्रुव वर्ण *update_worktree(unsigned वर्ण *sha1, ध्रुव संरचना worktree *worktree)
{
	ध्रुव वर्ण *retval;
	वर्ण *git_dir;
	संरचना strvec env = STRVEC_INIT;
	पूर्णाङ्क invoked_hook;

	यदि (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	यदि (worktree->is_bare)
		प्रत्यागम "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = ढकला_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	यदि (!invoked_hook)
		retval = ढकला_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	मुक्त(git_dir);
	प्रत्यागम retval;
}

स्थिर ध्रुव वर्ण *update(संरचना command *cmd, संरचना shallow_info *si)
{
	ध्रुव वर्ण *name = cmd->ref_name;
	संरचना strbuf namespaced_name_buf = STRBUF_INIT;
	स्थिर वर्ण *namespaced_name;
	ध्रुव वर्ण *ret;
	संरचना object_id *old_oid = &cmd->old_oid;
	संरचना object_id *new_oid = &cmd->new_oid;
	पूर्णाङ्क do_update_worktree = 0;
	संरचना worktree **worktrees = get_worktrees(the_repository);
	ध्रुव संरचना worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	यदि (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		गच्छ out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	मुक्त(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, रिक्त);

	यदि (worktree && !worktree->is_bare) {
		विकल्प (deny_current_branch) {
		पक्ष DENY_IGNORE:
			भङ्ग;
		पक्ष DENY_WARN:
			rp_warning("updating the current branch");
			भङ्ग;
		पक्ष DENY_REFUSE:
		पक्ष DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			यदि (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			गच्छ out;
		पक्ष DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			भङ्ग;
		}
	}

	यदि (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		गच्छ out;
	}

	यदि (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		यदि (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			गच्छ out;
		}

		यदि (worktree || (head_name && !strcmp(namespaced_name, head_name))) {
			विकल्प (deny_delete_current) {
			पक्ष DENY_IGNORE:
				भङ्ग;
			पक्ष DENY_WARN:
				rp_warning("deleting the current branch");
				भङ्ग;
			पक्ष DENY_REFUSE:
			पक्ष DENY_UNCONFIGURED:
			पक्ष DENY_UPDATE_INSTEAD:
				यदि (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				गच्छ out;
			स्वभाव:
				ret = "Invalid denyDeleteCurrent setting";
				गच्छ out;
			}
		}
	}

	यदि (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		संरचना object *old_object, *new_object;
		संरचना commit *old_commit, *new_commit;
		पूर्णाङ्क ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		यदि (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			गच्छ out;
		}
		old_commit = (संरचना commit *)old_object;
		new_commit = (संरचना commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		यदि (ret2 < 0)
			exit(128);
		यदि (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			गच्छ out;
		}
	}
	यदि (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		गच्छ out;
	}

	यदि (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		यदि (ret)
			गच्छ out;
	}

	यदि (is_null_oid(new_oid)) {
		संरचना strbuf err = STRBUF_INIT;
		यदि (!parse_object(the_repository, old_oid)) {
			old_oid = रिक्त;
			यदि (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} अन्यथा {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		यदि (ref_transaction_delete(transaction,
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
		यदि (shallow_update && si->shallow_ref[cmd->index] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			गच्छ out;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  रिक्त, रिक्त,
						  0, "push",
						  &err);
		यदि (tx_err) {
			rp_error("%s", err.buf);
			यदि (tx_err == REF_TRANSACTION_ERROR_GENERIC)
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
	प्रत्यागम ret;
}

स्थिर शून्य run_update_post_hook(संरचना command *commands)
{
	स्थिर ध्रुव वर्ण hook_name[] = "post-update";
	संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	संरचना async sideband_async;
	संरचना command *cmd;
	पूर्णाङ्क sideband_async_started = 0;
	पूर्णाङ्क saved_stderr = -1;

	यदि (!hook_exists(the_repository, hook_name))
		प्रत्यागम;

	प्रति (cmd = commands; cmd; cmd = cmd->next) {
		यदि (cmd->error_string || cmd->did_not_exist)
			अनुवर्त;
		strvec_ढकला(&opt.args, cmd->ref_name);
	}
	यदि (!opt.args.nr)
		प्रत्यागम;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

स्थिर शून्य check_aliased_update_internal(संरचना command *cmd,
					  संरचना string_list *list,
					  ध्रुव वर्ण *dst_name, पूर्णाङ्क flag)
{
	संरचना string_list_item *item;
	संरचना command *dst_cmd;

	यदि (!(flag & REF_ISSYMREF))
		प्रत्यागम;

	यदि (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		प्रत्यागम;
	}
	dst_name = strip_namespace(dst_name);

	यदि (!(item = string_list_lookup(list, dst_name)))
		प्रत्यागम;

	cmd->skip_update = 1;

	dst_cmd = (संरचना command *) item->util;

	यदि (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		प्रत्यागम;

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
	ध्रुव वर्ण *dst_name;
	पूर्णाङ्क flag;

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

	प्रति (cmd = commands; cmd; cmd = cmd->next) {
		संरचना string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (शून्य *)cmd;
	}
	string_list_sort(&ref_list);

	प्रति (cmd = commands; cmd; cmd = cmd->next) {
		यदि (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

स्थिर ध्रुव संरचना object_id *command_singleton_iterator(शून्य *cb_data)
{
	संरचना command **cmd_list = cb_data;
	संरचना command *cmd = *cmd_list;

	यदि (!cmd || is_null_oid(&cmd->new_oid))
		प्रत्यागम रिक्त;
	*cmd_list = रिक्त; /* this returns only one */
	प्रत्यागम &cmd->new_oid;
}

स्थिर शून्य set_connectivity_errors(संरचना command *commands,
				    संरचना shallow_info *si,
				    संरचना odb_transaction *transaction)
{
	संरचना command *cmd;

	प्रति (cmd = commands; cmd; cmd = cmd->next) {
		संरचना command *singleton = cmd;
		संरचना check_connected_options opt = CHECK_CONNECTED_INIT;
		संरचना strvec env = STRVEC_INIT;

		यदि (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			अनुवर्त;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		यदि (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			अनुवर्त;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

संरचना iterate_data {
	संरचना command *cmds;
	संरचना shallow_info *si;
};

स्थिर ध्रुव संरचना object_id *iterate_receive_command_list(शून्य *cb_data)
{
	संरचना iterate_data *data = cb_data;
	संरचना command **cmd_list = &data->cmds;
	संरचना command *cmd = *cmd_list;

	प्रति (; cmd; cmd = cmd->next) {
		यदि (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			अनुवर्त;
		यदि (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			प्रत्यागम &cmd->new_oid;
		}
	}
	प्रत्यागम रिक्त;
}

स्थिर शून्य reject_updates_to_hidden(संरचना command *commands)
{
	संरचना strbuf refname_full = STRBUF_INIT;
	size_t prefix_len;
	संरचना command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	प्रति (cmd = commands; cmd; cmd = cmd->next) {
		यदि (cmd->error_string)
			अनुवर्त;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		यदि (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			अनुवर्त;
		यदि (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		अन्यथा
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

स्थिर पूर्णाङ्क should_process_cmd(संरचना command *cmd)
{
	प्रत्यागम !cmd->error_string && !cmd->skip_update;
}

स्थिर शून्य BUG_if_skipped_connectivity_check(संरचना command *commands,
					       संरचना shallow_info *si)
{
	संरचना command *cmd;

	प्रति (cmd = commands; cmd; cmd = cmd->next) {
		यदि (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

स्थिर शून्य ref_transaction_rejection_handler(ध्रुव वर्ण *refname,
					      ध्रुव संरचना object_id *old_oid UNUSED,
					      ध्रुव संरचना object_id *new_oid UNUSED,
					      ध्रुव वर्ण *old_target UNUSED,
					      ध्रुव वर्ण *new_target UNUSED,
					      गणना ref_transaction_error err,
					      ध्रुव वर्ण *details,
					      शून्य *cb_data)
{
	संरचना strmap *failed_refs = cb_data;

	यदि (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (वर्ण *)ref_transaction_error_संदेश(err));
}

स्थिर शून्य execute_commands_non_atomic(संरचना command *commands,
					संरचना shallow_info *si)
{
	संरचना command *cmd;
	संरचना strbuf err = STRBUF_INIT;
	ध्रुव वर्ण *reported_error = रिक्त;
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

	प्रति (गणना processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		प्रति (cmd = commands; cmd; cmd = cmd->next) {
			यदि (!should_process_cmd(cmd) || cmd->run_proc_receive)
				अनुवर्त;

			यदि (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				अनुवर्त;
			अन्यथा यदि (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				अनुवर्त;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			यदि (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				यदि (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					गच्छ failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		यदि (!transaction)
			गच्छ cleanup;

		यदि (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			गच्छ failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		यदि (strmap_empty(&failed_refs))
			गच्छ cleanup;

	failure:
		प्रति (cmd = commands; cmd; cmd = cmd->next) {
			यदि (reported_error)
				cmd->error_string = reported_error;
			अन्यथा यदि (strmap_contains(&failed_refs, cmd->ref_name))
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
	ध्रुव वर्ण *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	यदि (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		गच्छ failure;
	}

	प्रति (cmd = commands; cmd; cmd = cmd->next) {
		यदि (!should_process_cmd(cmd) || cmd->run_proc_receive)
			अनुवर्त;

		cmd->error_string = update(cmd, si);

		यदि (cmd->error_string)
			गच्छ failure;
	}

	यदि (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		गच्छ failure;
	}
	गच्छ cleanup;

failure:
	प्रति (cmd = commands; cmd; cmd = cmd->next)
		यदि (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

स्थिर शून्य execute_commands(संरचना command *commands,
			     पूर्णाङ्क unpacker_error,
			     संरचना shallow_info *si,
			     संरचना odb_transaction *transaction,
			     ध्रुव संरचना string_list *ढकला_options)
{
	संरचना check_connected_options opt = CHECK_CONNECTED_INIT;
	संरचना command *cmd;
	संरचना iterate_data data;
	संरचना async muxer;
	पूर्णाङ्क err_fd = 0;
	पूर्णाङ्क run_proc_receive = 0;

	यदि (unpacker_error) {
		प्रति (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		प्रत्यागम;
	}

	यदि (!skip_connectivity_check) {
		संरचना strvec env = STRVEC_INIT;

		यदि (use_sideband) {
			memset(&muxer, 0, परिमाण(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			यदि (!start_async(&muxer))
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

		यदि (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		यदि (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	यदि (proc_receive_ref) {
		प्रति (cmd = commands; cmd; cmd = cmd->next) {
			यदि (!should_process_cmd(cmd))
				अनुवर्त;

			यदि (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	यदि (run_receive_hook(commands, "pre-receive", 0, transaction, ढकला_options)) {
		प्रति (cmd = commands; cmd; cmd = cmd->next) {
			यदि (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		प्रत्यागम;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	प्रति (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	यदि (!cmd)
		प्रत्यागम;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	यदि (odb_transaction_commit(transaction)) {
		प्रति (cmd = commands; cmd; cmd = cmd->next) {
			यदि (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		प्रत्यागम;
	}

	check_aliased_updates(commands);

	मुक्त(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, रिक्त,
							    रिक्त);

	यदि (run_proc_receive &&
	    run_proc_receive_hook(commands, ढकला_options))
		प्रति (cmd = commands; cmd; cmd = cmd->next)
			यदि (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	यदि (use_atomic)
		execute_commands_atomic(commands, si);
	अन्यथा
		execute_commands_non_atomic(commands, si);

	यदि (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

स्थिर संरचना command **queue_command(संरचना command **tail,
				      ध्रुव वर्ण *ओळ,
				      पूर्णाङ्क linelen)
{
	संरचना object_id old_oid, new_oid;
	संरचना command *cmd;
	ध्रुव वर्ण *refname;
	पूर्णाङ्क reflen;
	ध्रुव वर्ण *p;

	यदि (parse_oid_hex(ओळ, &old_oid, &p) ||
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
	प्रत्यागम &cmd->next;
}

स्थिर शून्य free_commands(संरचना command *commands)
{
	यावत् (commands) {
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
	ध्रुव वर्ण *boc, *eoc;

	यदि (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(ढकला_cert->buf, "\n\n");
	यदि (!boc)
		die("malformed push certificate %.*s", 100, ढकला_cert->buf);
	अन्यथा
		boc += 2;
	eoc = ढकला_cert->buf + parse_signed_buffer(ढकला_cert->buf, ढकला_cert->len);

	यावत् (boc < eoc) {
		ध्रुव वर्ण *eol = memchr(boc, '\n', eoc - boc);
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
	प्रति (;;) {
		पूर्णाङ्क linelen;

		यदि (packet_reader_read(reader) != PACKET_READ_NORMAL)
			भङ्ग;

		यदि (reader->pktlen > 8 && starts_with(reader->ओळ, "shallow ")) {
			संरचना object_id oid;
			यदि (get_oid_hex(reader->ओळ + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->ओळ + 8);
			oid_array_append(shallow, &oid);
			अनुवर्त;
		}

		linelen = strlen(reader->ओळ);
		यदि (linelen < reader->pktlen) {
			ध्रुव वर्ण *feature_list = reader->ओळ + linelen + 1;
			ध्रुव वर्ण *hash = रिक्त;
			ध्रुव वर्ण *client_sid;
			size_t len = 0;
			यदि (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			यदि (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			यदि (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			यदि (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			यदि (advertise_atomic_ढकला
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			यदि (advertise_ढकला_options
			    && parse_feature_request(feature_list, "push-options"))
				use_ढकला_options = 1;
			hash = parse_feature_मूल्य(feature_list, "object-format", &len, रिक्त);
			यदि (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			यदि (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_मूल्य(feature_list, "session-id", &len, रिक्त);
			यदि (client_sid) {
				वर्ण *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", रिक्त, "client-sid", client_sid);
				मुक्त(sid);
			}
		}

		यदि (!strcmp(reader->ओळ, "push-cert")) {
			पूर्णाङ्क true_flush = 0;
			पूर्णाङ्क saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			प्रति (;;) {
				packet_reader_read(reader);
				यदि (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					भङ्ग;
				}
				यदि (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				यदि (!strcmp(reader->ओळ, "push-cert-end\n"))
					भङ्ग; /* end of cert */
				strbuf_addstr(&ढकला_cert, reader->ओळ);
			}
			reader->options = saved_options;

			यदि (true_flush)
				भङ्ग;
			अनुवर्त;
		}

		p = queue_command(p, reader->ओळ, linelen);
	}

	यदि (ढकला_cert.len)
		queue_commands_from_cert(p, &ढकला_cert);

	प्रत्यागम commands;
}

स्थिर शून्य read_ढकला_options(संरचना packet_reader *reader,
			      संरचना string_list *options)
{
	यावत् (1) {
		यदि (packet_reader_read(reader) != PACKET_READ_NORMAL)
			भङ्ग;

		string_list_append(options, reader->ओळ);
	}
}

स्थिर पूर्णाङ्क unpack_with_sideband(संरचना odb_transaction *transaction,
				ध्रुव वर्ण *shallow_file,
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
	पूर्णाङ्क ret;

	यदि (!use_sideband)
		प्रत्यागम odb_transaction_write_pack(transaction, 0, err_संदेश, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, परिमाण(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	यदि (start_async(&muxer))
		प्रत्यागम 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_संदेश, &opts);

	finish_async(&muxer);
	प्रत्यागम ret;
}

स्थिर शून्य prepare_shallow_update(संरचना shallow_info *si,
				   ध्रुव वर्ण *shallow_file)
{
	पूर्णाङ्क i, j, k, bitmap_size = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, रिक्त);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	प्रति (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	प्रति (i = 0; i < si->shallow->nr; i++) {
		यदि (!si->used_shallow[i])
			अनुवर्त;
		प्रति (j = 0; j < bitmap_size; j++) {
			यदि (!si->used_shallow[i][j])
				अनुवर्त;
			si->need_reachability_test[i]++;
			प्रति (k = 0; k < 32; k++)
				यदि (si->used_shallow[i][j] & (1U << k))
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
				ध्रुव वर्ण *shallow_file)
{
	संरचना command *cmd;
	पूर्णाङ्क *ref_status;
	remove_nonexistent_theirs_shallow(si);
	यदि (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		प्रत्यागम;
	}

	प्रति (cmd = commands; cmd; cmd = cmd->next) {
		यदि (is_null_oid(&cmd->new_oid))
			अनुवर्त;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	यदि (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		प्रत्यागम;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, रिक्त, ref_status);
	प्रति (cmd = commands; cmd; cmd = cmd->next) {
		यदि (is_null_oid(&cmd->new_oid))
			अनुवर्त;
		यदि (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	मुक्त(ref_status);
}

स्थिर शून्य override_cmds_error(संरचना command *commands, ध्रुव वर्ण *err)
{
	प्रति (संरचना command *cmd = commands; cmd; cmd = cmd->next) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
स्थिर शून्य generate_report(संरचना strbuf *buf, संरचना command *commands,
			    ध्रुव संरचना strbuf *unpack_status,
			    गणना report_status_version version)
{
	संरचना command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	प्रति (cmd = commands; cmd; cmd = cmd->next) {
		संरचना ref_ढकला_report *report;
		पूर्णाङ्क मोजणी = 0;

		यदि (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		अन्यथा
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		यदि (version != REPORT_STATUS_V2 || cmd->error_string)
			अनुवर्त;

		प्रति (report = cmd->report; report; report = report->next) {
			यदि (मोजणी++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			यदि (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			यदि (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			यदि (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			यदि (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

स्थिर शून्य report(संरचना command *commands, ध्रुव संरचना strbuf *unpack_status,
		   गणना report_status_version version)
{
	संरचना strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	यदि (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	यदि (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	अन्यथा
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

स्थिर पूर्णाङ्क delete_only(संरचना command *commands)
{
	संरचना command *cmd;
	प्रति (cmd = commands; cmd; cmd = cmd->next) {
		यदि (!is_null_oid(&cmd->new_oid))
			प्रत्यागम 0;
	}
	प्रत्यागम 1;
}

पूर्णाङ्क cmd_receive_pack(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *prefix,
		     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क advertise_refs = 0;
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

	यदि (argc > 1)
		usage_संदेश_opt(_("too many arguments"), receive_pack_usage, options);
	यदि (argc == 0)
		usage_संदेश_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	यदि (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, रिक्त);
	यदि (cert_nonce_seed)
		ढकला_cert_nonce = prepare_ढकला_cert_nonce(service_dir, time(रिक्त));

	विकल्प (determine_protocol_version_server()) {
	पक्ष protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		भङ्ग;
	पक्ष protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		यदि (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	पक्ष protocol_v0:
		भङ्ग;
	पक्ष protocol_unknown_version:
		BUG("unknown protocol version");
	}

	यदि (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	यदि (advertise_refs)
		प्रत्यागम 0;

	packet_reader_init(&reader, 0, रिक्त, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	यदि ((commands = read_head_info(&reader, &shallow, &version))) {
		संरचना string_list ढकला_options = STRING_LIST_INIT_DUP;
		संरचना strbuf unpack_status = STRBUF_INIT;

		यदि (use_ढकला_options)
			read_ढकला_options(&reader, &ढकला_options);
		यदि (!check_cert_ढकला_options(&ढकला_options)) {
			संरचना command *cmd;
			प्रति (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		यदि (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		यदि (!delete_only(commands)) {
			ध्रुव वर्ण *alt_shallow_file = रिक्त;

			यदि (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			यदि (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			अन्यथा
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &ढकला_options);
		odb_transaction_finalize(transaction);
		sigchain_ढकला(SIGPIPE, SIG_IGN);

		विकल्प (version) {
		पक्ष REPORT_STATUS_V2:
		पक्ष REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			भङ्ग;
		पक्ष REPORT_STATUS_UNKNOWN:
			भङ्ग;
		}

		sigchain_काढा(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, रिक्त,
				 &ढकला_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&ढकला_options, 0);
		यदि (auto_gc) {
			संरचना child_process proc = CHILD_PROCESS_INIT;

			यदि (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				यदि (!start_command(&proc)) {
					यदि (use_sideband)
						copy_to_sideband(proc.err, -1, रिक्त);
					finish_command(&proc);
				}
			}
		}
		यदि (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	यदि (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	मुक्त((शून्य *)ढकला_cert_nonce);
	प्रत्यागम 0;
}
