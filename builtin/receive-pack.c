#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

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

गणना report_स्थितिः_संस्करणम् {
	REPORT_स्थितिः_UNKNOWN = 0,
	REPORT_स्थितिः_V0,
	REPORT_स्थितिः_V2,
};

स्थिर पूर्णाङ्क deny_deletes;
स्थिर पूर्णाङ्क deny_non_fast_forwards;
स्थिर गणना deny_action deny_current_branch = DENY_UNCONFIGURED;
स्थिर गणना deny_action deny_delete_current = DENY_UNCONFIGURED;
स्थिर पूर्णाङ्क receive_fsck_objects = -1;
स्थिर पूर्णाङ्क transfer_fsck_objects = -1;
स्थिर संरचना strbuf fsck_सन्देशः_types = STRBUF_INIT;
स्थिर पूर्णाङ्क advertise_atomic_प्रक्षिप = 1;
स्थिर पूर्णाङ्क advertise_प्रक्षिप_options;
स्थिर पूर्णाङ्क advertise_sid;
स्थिर off_t max_निवेशः_आकार;
स्थिर पूर्णाङ्क use_sideband;
स्थिर पूर्णाङ्क use_atomic;
स्थिर पूर्णाङ्क use_प्रक्षिप_options;
स्थिर पूर्णाङ्क quiet;
स्थिर पूर्णाङ्क prefer_ofs_delta = 1;
स्थिर पूर्णाङ्क auto_update_server_info;
स्थिर पूर्णाङ्क auto_gc = 1;
स्थिर पूर्णाङ्क reject_thin;
स्थिर पूर्णाङ्क skip_connectivity_check;
स्थिर पूर्णाङ्क stateless_rpc;
स्थिर ध्रुव वर्ण *service_dir;
स्थिर ध्रुव वर्ण *शीर्ष_name;
स्थिर शून्य *शीर्ष_name_गन्तव्यम्_free;
स्थिर पूर्णाङ्क प्रेषितम्_capabilities;
स्थिर पूर्णाङ्क shallow_update;
स्थिर संरचना strbuf प्रक्षिप_cert = STRBUF_INIT;
स्थिर संरचना object_id प्रक्षिप_cert_oid;
स्थिर संरचना signature_check sigcheck;
स्थिर ध्रुव वर्ण *प्रक्षिप_cert_nonce;
स्थिर वर्ण *cert_nonce_बीजम्;
स्थिर संरचना strvec hidden_refs = STRVEC_INIT;

स्थिर ध्रुव वर्ण *NONCE_UNSOLICITED = "UNSOLICITED";
स्थिर ध्रुव वर्ण *NONCE_BAD = "BAD";
स्थिर ध्रुव वर्ण *NONCE_MISSING = "MISSING";
स्थिर ध्रुव वर्ण *NONCE_OK = "OK";
स्थिर ध्रुव वर्ण *NONCE_SLOP = "SLOP";
स्थिर ध्रुव वर्ण *nonce_स्थितिः;
स्थिर दीर्घ nonce_stamp_slop;
स्थिर timestamp_t nonce_stamp_slop_सीमा;
स्थिर संरचना ref_transaction *transaction;

स्थिर गणना {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
स्थिर पूर्णाङ्क keepalive_अन्तर्_sec = 5;

स्थिर संरचना proc_receive_ref {
	अचिह्नित पूर्णाङ्क want_सङ्कलय:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	वर्ण *ref_उपसर्गः;
	संरचना proc_receive_ref *अग्र;
} *proc_receive_ref;

स्थिर शून्य proc_receive_ref_append(ध्रुव वर्ण *उपसर्गः);

स्थिर गणना deny_action parse_deny_action(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्)
{
	यदि (मानम्) {
		यदि (!strcasecmp(मानम्, "ignore"))
			प्रत्यागम DENY_IGNORE;
		यदि (!strcasecmp(मानम्, "warn"))
			प्रत्यागम DENY_WARN;
		यदि (!strcasecmp(मानम्, "refuse"))
			प्रत्यागम DENY_REFUSE;
		यदि (!strcasecmp(मानम्, "updateinstead"))
			प्रत्यागम DENY_UPDATE_INSTEAD;
	}
	यदि (git_config_bool(var, मानम्))
		प्रत्यागम DENY_REFUSE;
	प्रत्यागम DENY_IGNORE;
}

स्थिर पूर्णाङ्क receive_pack_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			       ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	ध्रुव वर्ण *सन्देशः_id_2;
	पूर्णाङ्क स्थितिः = parse_hide_refs_config(var, मानम्, "receive", &hidden_refs);

	यदि (स्थितिः)
		प्रत्यागम स्थितिः;

	यदि (वाक्यं_तुलय(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.fsck.skiplist") == 0) {
		वर्ण *मार्गः;

		यदि (git_config_pathname(&मार्गः, var, मानम्))
			प्रत्यागम -1;
		यदि (मार्गः)
			strbuf_addf(&fsck_सन्देशः_types, "%cskiplist=%s",
				    fsck_सन्देशः_types.len ? ',' : '=', मार्गः);
		मुक्त(मार्गः);
		प्रत्यागम 0;
	}

	यदि (skip_उपसर्गः(var, "receive.fsck.", &सन्देशः_id_2)) {
		यदि (!मानम्)
			प्रत्यागम config_error_nonbool(var);
		यदि (is_valid_सन्देशः_type(सन्देशः_id_2, मानम्))
			strbuf_addf(&fsck_सन्देशः_types, "%c%s=%s",
				fsck_सन्देशः_types.len ? ',' : '=', सन्देशः_id_2, मानम्);
		अन्यथा
			warning("skipping unknown msg id '%s'", सन्देशः_id_2);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.certnonceseed") == 0)
		प्रत्यागम git_config_string(&cert_nonce_बीजम्, var, मानम्);

	यदि (वाक्यं_तुलय(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_सीमा = git_config_ulong(var, मानम्, ctx->kvi);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_प्रक्षिप = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.advertisepushoptions") == 0) {
		advertise_प्रक्षिप_options = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.keepalive") == 0) {
		keepalive_अन्तर्_sec = git_config_int(var, मानम्, ctx->kvi);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.maxinputsize") == 0) {
		max_निवेशः_आकार = git_config_int64(var, मानम्, ctx->kvi);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "receive.procreceiverefs") == 0) {
		यदि (!मानम्)
			प्रत्यागम config_error_nonbool(var);
		proc_receive_ref_append(मानम्);
		प्रत्यागम 0;
	}

	यदि (वाक्यं_तुलय(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	प्रत्यागम git_default_config(var, मानम्, ctx, cb);
}

स्थिर शून्य show_ref(ध्रुव वर्ण *मार्गः, ध्रुव संरचना object_id *oid)
{
	यदि (प्रेषितम्_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_गन्तव्यम्_hex(oid), मार्गः);
	} अन्यथा {
		संरचना strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		यदि (advertise_atomic_प्रक्षिप)
			strbuf_addstr(&cap, " atomic");
		यदि (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		यदि (प्रक्षिप_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", प्रक्षिप_cert_nonce);
		यदि (advertise_प्रक्षिप_options)
			strbuf_addstr(&cap, " push-options");
		यदि (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_गन्तव्यम्_hex(oid), मार्गः, 0, cap.buf);
		strbuf_release(&cap);
		प्रेषितम्_capabilities = 1;
	}
}

स्थिर पूर्णाङ्क show_ref_cb(ध्रुव संरचना reference *ref, शून्य *दत्त)
{
	संरचना oidset *आलोकितम् = दत्त;
	ध्रुव वर्ण *मार्गः = strip_namespace(ref->name);

	यदि (ref_is_hidden(मार्गः, ref->name, &hidden_refs))
		प्रत्यागम 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	यदि (!मार्गः) {
		यदि (oidset_निवेशय(आलोकितम्, ref->oid))
			प्रत्यागम 0;
		मार्गः = ".have";
	} अन्यथा {
		oidset_निवेशय(आलोकितम्, ref->oid);
	}
	show_ref(मार्गः, ref->oid);
	प्रत्यागम 0;
}

स्थिर शून्य show_one_alternate_ref(ध्रुव संरचना object_id *oid,
				   शून्य *दत्त)
{
	संरचना oidset *आलोकितम् = दत्त;

	यदि (oidset_निवेशय(आलोकितम्, oid))
		प्रत्यागम;

	show_ref(".have", oid);
}

स्थिर शून्य write_शीर्ष_info(शून्य)
{
	संरचना refs_for_each_ref_options opts = { 0 };
	स्थिर संरचना oidset आलोकितम् = OIDSET_INIT;
	संरचना strvec excludes_सदिशम् = STRVEC_INIT;

	/*
	 * We need access to the reference names both with and without their
	 * namespace and thus cannot use `refs_for_each_namespaced_ref()`. We
	 * thus have to adapt exclude patterns to carry the namespace prefix
	 * ourselves.
	 */
	opts.exclude_patterns = get_namespaced_exclude_patterns(
		hidden_refs_गन्तव्यम्_excludes(&hidden_refs),
		get_git_namespace(), &excludes_सदिशम्);

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      show_ref_cb, &आलोकितम्, &opts);
	odb_for_each_alternate_ref(the_repository->objects,
				   show_one_alternate_ref, &आलोकितम्);

	oidset_clear(&आलोकितम्);
	strvec_clear(&excludes_सदिशम्);

	यदि (!प्रेषितम्_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#परिभाषय सततम्_PROC_RECEIVE_SCHEDULED	1
#परिभाषय सततम्_PROC_RECEIVE_RETURNED	2
संरचना command {
	संरचना command *अग्र;
	ध्रुव वर्ण *error_string;
	वर्ण *error_string_owned;
	संरचना ref_प्रक्षिप_report *report;
	अचिह्नित पूर्णाङ्क skip_update:1,
		     did_not_exist:1,
		     सततम्_proc_receive:2;
	पूर्णाङ्क सूचकांक;
	संरचना object_id old_oid;
	संरचना object_id new_oid;
	वर्ण ref_name[FLEX_ARRAY]; /*अधिकम्*/
};

स्थिर शून्य proc_receive_ref_append(ध्रुव वर्ण *उपसर्गः)
{
	संरचना proc_receive_ref *ref_आदर्शः;
	ध्रुव वर्ण *p;
	पूर्णाङ्क len;

	CALLOC_ARRAY(ref_आदर्शः, 1);
	p = strchr(उपसर्गः, ':');
	यदि (p) {
		यावत् (उपसर्गः < p) {
			यदि (*उपसर्गः == 'a')
				ref_आदर्शः->want_सङ्कलय = 1;
			अन्यथा यदि (*उपसर्गः == 'd')
				ref_आदर्शः->want_delete = 1;
			अन्यथा यदि (*उपसर्गः == 'm')
				ref_आदर्शः->want_modify = 1;
			अन्यथा यदि (*उपसर्गः == '!')
				ref_आदर्शः->negative_ref = 1;
			उपसर्गः++;
		}
		उपसर्गः++;
	} अन्यथा {
		ref_आदर्शः->want_सङ्कलय = 1;
		ref_आदर्शः->want_delete = 1;
		ref_आदर्शः->want_modify = 1;
	}
	len = वाक्यदैर्घ्यम्(उपसर्गः);
	यावत् (len && उपसर्गः[len - 1] == '/')
		len--;
	ref_आदर्शः->ref_उपसर्गः = xmemdupz(उपसर्गः, len);
	यदि (!proc_receive_ref) {
		proc_receive_ref = ref_आदर्शः;
	} अन्यथा {
		संरचना proc_receive_ref *अन्तः;

		अन्तः = proc_receive_ref;
		यावत् (अन्तः->अग्र)
			अन्तः = अन्तः->अग्र;
		अन्तः->अग्र = ref_आदर्शः;
	}
}

स्थिर पूर्णाङ्क proc_receive_ref_matches(संरचना command *cmd)
{
	संरचना proc_receive_ref *p;

	यदि (!proc_receive_ref)
		प्रत्यागम 0;

	प्रति (p = proc_receive_ref; p; p = p->अग्र) {
		ध्रुव वर्ण *मेलनम् = p->ref_उपसर्गः;
		ध्रुव वर्ण *remains;

		यदि (!p->want_सङ्कलय && is_null_oid(&cmd->old_oid))
			अनुवर्त;
		अन्यथा यदि (!p->want_delete && is_null_oid(&cmd->new_oid))
			अनुवर्त;
		अन्यथा यदि (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			अनुवर्त;

		यदि (skip_उपसर्गः(cmd->ref_name, मेलनम्, &remains) &&
		    (!*remains || *remains == '/')) {
			यदि (!p->negative_ref)
				प्रत्यागम 1;
		} अन्यथा यदि (p->negative_ref) {
			प्रत्यागम 1;
		}
	}
	प्रत्यागम 0;
}

स्थिर शून्य report_सन्देशः(ध्रुव वर्ण *उपसर्गः, ध्रुव वर्ण *err, va_list params)
{
	पूर्णाङ्क sz;
	वर्ण सन्देशः[4096];

	sz = xsnprintf(सन्देशः, परिमाण(सन्देशः), "%s", उपसर्गः);
	sz += vsnprintf(सन्देशः + sz, परिमाण(सन्देशः) - sz, err, params);
	यदि (sz > (परिमाण(सन्देशः) - 1))
		sz = परिमाण(सन्देशः) - 1;
	सन्देशः[sz++] = '\n';

	यदि (use_sideband)
		send_sideband(1, 2, सन्देशः, sz, use_sideband);
	अन्यथा
		xwrite(2, सन्देशः, sz);
}

__attribute__((format (प्रारूप_लिख, 1, 2)))
स्थिर शून्य rp_warning(ध्रुव वर्ण *err, ...)
{
	va_list params;
	va_start(params, err);
	report_सन्देशः("warning: ", err, params);
	va_अन्तः(params);
}

__attribute__((format (प्रारूप_लिख, 1, 2)))
स्थिर शून्य rp_error(ध्रुव वर्ण *err, ...)
{
	va_list params;
	va_start(params, err);
	report_सन्देशः("error: ", err, params);
	va_अन्तः(params);
}

स्थिर पूर्णाङ्क copy_गन्तव्यम्_sideband(पूर्णाङ्क अन्तर्, पूर्णाङ्क बहिर्गमनम् अप्रयुक्तम्, शून्य *arg अप्रयुक्तम्)
{
	वर्ण दत्त[128];
	पूर्णाङ्क keepalive_सक्रिय = 0;

	यदि (keepalive_अन्तर्_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	यदि (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_सक्रिय = 1;

	यावत् (1) {
		ssize_t sz;

		यदि (keepalive_सक्रिय) {
			संरचना pollfd pfd;
			पूर्णाङ्क ret;

			pfd.fd = अन्तर्;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_अन्तर्_sec);

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

		sz = xread(अन्तर्, दत्त, परिमाण(दत्त));
		यदि (sz <= 0)
			भङ्ग;

		यदि (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_सक्रिय) {
			ध्रुव वर्ण *p = memchr(दत्त, '\0', sz);
			यदि (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_सक्रिय = 1;
				send_sideband(1, 2, दत्त, p - दत्त, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - दत्त + 1), use_sideband);
				अनुवर्त;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, दत्त, sz, use_sideband);
	}
	close(अन्तर्);
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

	स्मृतिं_पूरय(sideband_async, 0, परिमाण(*sideband_async));
	sideband_async->proc = copy_गन्तव्यम्_sideband;
	sideband_async->अन्तर् = -1;

	यदि (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		यदि (*saved_stderr >= 0)
			dup2(sideband_async->अन्तर्, STDERR_FILENO);
		close(sideband_async->अन्तर्);
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

स्थिर शून्य hmac_hash(अचिह्नित वर्ण *बहिर्गमनम्,
		      ध्रुव वर्ण *कुञ्चिका_अन्तर्, आकार_t कुञ्चिका_len,
		      ध्रुव वर्ण *text, आकार_t text_len)
{
	अचिह्नित वर्ण कुञ्चिका[GIT_MAX_BLKSZ];
	अचिह्नित वर्ण k_ipad[GIT_MAX_BLKSZ];
	अचिह्नित वर्ण k_opad[GIT_MAX_BLKSZ];
	पूर्णाङ्क i;
	संरचना git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	स्मृतिं_पूरय(कुञ्चिका, '\0', GIT_MAX_BLKSZ);
	यदि (the_hash_algo->blksz < कुञ्चिका_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, कुञ्चिका_अन्तर्, कुञ्चिका_len);
		git_hash_final(कुञ्चिका, &ctx);
	} अन्यथा {
		स्मृतिं_प्रतिलिख(कुञ्चिका, कुञ्चिका_अन्तर्, कुञ्चिका_len);
	}

	/* RFC 2104 2. (2) & (5) */
	प्रति (i = 0; i < परिमाण(कुञ्चिका); i++) {
		k_ipad[i] = कुञ्चिका[i] ^ 0x36;
		k_opad[i] = कुञ्चिका[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, परिमाण(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(बहिर्गमनम्, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, परिमाण(k_opad));
	git_hash_update(&ctx, बहिर्गमनम्, the_hash_algo->rawsz);
	git_hash_final(बहिर्गमनम्, &ctx);
}

स्थिर वर्ण *prepare_प्रक्षिप_cert_nonce(ध्रुव वर्ण *मार्गः, timestamp_t stamp)
{
	संरचना strbuf buf = STRBUF_INIT;
	अचिह्नित वर्ण hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, मार्गः, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_बीजम्, वाक्यदैर्घ्यम्(cert_nonce_बीजम्));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (पूर्णाङ्क)the_hash_algo->hexsz, hash_गन्तव्यम्_hex(hash));
	प्रत्यागम strbuf_detach(&buf, रिक्त);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
स्थिर पूर्णाङ्क constant_memequal(ध्रुव वर्ण *a, ध्रुव वर्ण *b, आकार_t n)
{
	पूर्णाङ्क res = 0;
	आकार_t i;

	प्रति (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	प्रत्यागम res;
}

स्थिर ध्रुव वर्ण *check_nonce(ध्रुव वर्ण *buf)
{
	आकार_t noncelen;
	ध्रुव वर्ण *प्राप्तम्_2 = find_commit_शीर्षकम्(buf, "nonce", &noncelen);
	वर्ण *nonce = प्राप्तम्_2 ? xmemdupz(प्राप्तम्_2, noncelen) : रिक्त;
	timestamp_t stamp, ostamp;
	वर्ण *bohmac, *expect = रिक्त;
	ध्रुव वर्ण *retval = NONCE_BAD;

	यदि (!nonce) {
		retval = NONCE_MISSING;
		गच्छ leave;
	} अन्यथा यदि (!प्रक्षिप_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		गच्छ leave;
	} अन्यथा यदि (!वाक्यं_तुलय(प्रक्षिप_cert_nonce, nonce)) {
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

	expect = prepare_प्रक्षिप_cert_nonce(service_dir, stamp);
	यदि (noncelen != वाक्यदैर्घ्यम्(expect)) {
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
	ostamp = parse_timestamp(प्रक्षिप_cert_nonce, रिक्त, 10);
	nonce_stamp_slop = (दीर्घ)ostamp - (दीर्घ)stamp;

	यदि (nonce_stamp_slop_सीमा &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_सीमा) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		मुक्त((शून्य *)प्रक्षिप_cert_nonce);
		प्रक्षिप_cert_nonce = xstrdup(nonce);
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
स्थिर पूर्णाङ्क check_cert_प्रक्षिप_options(ध्रुव संरचना string_list *प्रक्षिप_options)
{
	ध्रुव वर्ण *buf = प्रक्षिप_cert.buf;

	ध्रुव वर्ण *option;
	आकार_t optionlen;
	पूर्णाङ्क options_आलोकितम् = 0;

	पूर्णाङ्क retval = 1;

	यदि (!*buf)
		प्रत्यागम 1;

	यावत् ((option = find_commit_शीर्षकम्(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_आलोकितम्++;
		यदि (options_आलोकितम् > प्रक्षिप_options->nr
		    || xstrncmpz(प्रक्षिप_options->वस्तूनि[options_आलोकितम् - 1].string,
				 option, optionlen))
			प्रत्यागम 0;
	}

	यदि (options_आलोकितम् != प्रक्षिप_options->nr)
		retval = 0;

	प्रत्यागम retval;
}

स्थिर शून्य prepare_प्रक्षिप_cert_sha1(संरचना सततम्_hooks_opt *opt)
{
	स्थिर पूर्णाङ्क already_समाप्तम्;

	यदि (!प्रक्षिप_cert.len)
		प्रत्यागम;

	यदि (!already_समाप्तम्) {
		पूर्णाङ्क bogs /* beginning_of_gpg_sig */;

		already_समाप्तम् = 1;
		यदि (odb_write_object(the_repository->objects, प्रक्षिप_cert.buf,
				     प्रक्षिप_cert.len, OBJ_BLOB, &प्रक्षिप_cert_oid))
			oidclr(&प्रक्षिप_cert_oid, the_repository->hash_algo);

		स्मृतिं_पूरय(&sigcheck, '\0', परिमाण(sigcheck));

		bogs = parse_signed_बफर(प्रक्षिप_cert.buf, प्रक्षिप_cert.len);
		sigcheck.payload = xmemdupz(प्रक्षिप_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, प्रक्षिप_cert.buf + bogs,
				प्रक्षिप_cert.len - bogs);

		nonce_स्थितिः = check_nonce(sigcheck.payload);
	}
	यदि (!is_null_oid(&प्रक्षिप_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_गन्तव्यम्_hex(&प्रक्षिप_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.कुञ्चिका ? sigcheck.कुञ्चिका : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.परिणाम);
		यदि (प्रक्षिप_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     प्रक्षिप_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_स्थितिः);
			यदि (nonce_स्थितिः == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

संरचना receive_hook_feed_state {
	संरचना command *cmd;
	संरचना ref_प्रक्षिप_report *report;
	पूर्णाङ्क skip_broken;
	संरचना strbuf buf;
};

स्थिर पूर्णाङ्क feed_receive_hook_cb(पूर्णाङ्क hook_stdin_fd, शून्य *pp_cb अप्रयुक्तम्, शून्य *pp_task_cb)
{
	संरचना receive_hook_feed_state *state = pp_task_cb;
	संरचना command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	यावत् (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->अग्र;

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
			    oid_गन्तव्यम्_hex(old_oid), oid_गन्तव्यम्_hex(new_oid),
			    ref_name);

		state->report = state->report->अग्र;
		यदि (!state->report)
			cmd = cmd->अग्र;
	} अन्यथा {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_गन्तव्यम्_hex(&cmd->old_oid), oid_गन्तव्यम्_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->अग्र;
	}

	state->cmd = cmd;

	यदि (state->buf.len > 0) {
		पूर्णाङ्क ret = write_अन्तर्_पूर्णम्(hook_stdin_fd, state->buf.buf, state->buf.len);
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
	संरचना receive_hook_feed_state *दत्त;

	CALLOC_ARRAY(दत्त, 1);
	दत्त->report = init_state->report;
	दत्त->cmd = init_state->cmd;
	दत्त->skip_broken = init_state->skip_broken;
	strbuf_init(&दत्त->buf, 0);

	प्रत्यागम दत्त;
}

स्थिर शून्य receive_hook_feed_state_free(शून्य *दत्त)
{
	संरचना receive_hook_feed_state *d = दत्त;
	यदि (!d)
		प्रत्यागम;
	strbuf_release(&d->buf);
	मुक्त(d);
}

स्थिर पूर्णाङ्क सततम्_receive_hook(संरचना command *commands,
			    ध्रुव वर्ण *hook_name,
			    पूर्णाङ्क skip_broken,
			    संरचना odb_transaction *transaction,
			    ध्रुव संरचना string_list *प्रक्षिप_options)
{
	संरचना सततम्_hooks_opt opt = सततम्_HOOKS_OPT_INIT;
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
		iter = iter->अग्र;
	यदि (!iter)
		प्रत्यागम 0;

	यदि (प्रक्षिप_options) {
		प्रति (पूर्णाङ्क i = 0; i < प्रक्षिप_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     प्रक्षिप_options->वस्तूनि[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)प्रक्षिप_options->nr);
	} अन्यथा {
		strvec_प्रक्षिप(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	यदि (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_प्रक्षिप_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_दत्त_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_दत्त_free = receive_hook_feed_state_free;

	ret = सततम्_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क सततम्_update_hook(संरचना command *cmd)
{
	स्थिर ध्रुव वर्ण hook_name[] = "update";
	संरचना सततम्_hooks_opt opt = सततम्_HOOKS_OPT_INIT;
	संरचना async sideband_async;
	पूर्णाङ्क sideband_async_started = 0;
	पूर्णाङ्क saved_stderr = -1;
	पूर्णाङ्क संहिता;

	यदि (!hook_exists(the_repository, hook_name))
		प्रत्यागम 0;

	strvec_pushl(&opt.तर्काः,
		     cmd->ref_name,
		     oid_गन्तव्यम्_hex(&cmd->old_oid),
		     oid_गन्तव्यम्_hex(&cmd->new_oid),
		     रिक्त);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	संहिता = सततम्_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	प्रत्यागम संहिता;
}

स्थिर पूर्णाङ्क सततम्_receive_report_hook(संरचना strbuf *report)
{
	संरचना child_process proc = CHILD_PROCESS_INIT;
	संरचना async sideband_async;
	पूर्णाङ्क sideband_async_started = 0;
	पूर्णाङ्क saved_stderr = -1;
	संरचना strbuf बहिर्गमनम् = STRBUF_INIT;
	ध्रुव वर्ण *hook_मार्गः;
	पूर्णाङ्क ret;

	hook_मार्गः = find_hook(the_repository, "receive-report");
	यदि (!hook_मार्गः)
		प्रत्यागम 0;

	strvec_प्रक्षिप(&proc.तर्काः, hook_मार्गः);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_प्रक्षिप(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &बहिर्गमनम्,
			   report->len, रिक्त, 0);
	sigchain_निष्कासय(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	यदि (!ret)
		strbuf_swap(&बहिर्गमनम्, report);

	strbuf_release(&बहिर्गमनम्);
	प्रत्यागम ret;
}

स्थिर संरचना command *find_command_by_refname(संरचना command *list,
					       ध्रुव वर्ण *refname)
{
	प्रति (; list; list = list->अग्र)
		यदि (!वाक्यं_तुलय(list->ref_name, refname))
			प्रत्यागम list;
	प्रत्यागम रिक्त;
}

स्थिर पूर्णाङ्क read_proc_receive_report(संरचना packet_reader *reader,
				    संरचना command *commands,
				    संरचना strbuf *errmsg)
{
	संरचना command *cmd;
	संरचना command *hint = रिक्त;
	संरचना ref_प्रक्षिप_report *report = रिक्त;
	पूर्णाङ्क new_report = 0;
	पूर्णाङ्क संहिता = 0;
	पूर्णाङ्क once = 0;
	पूर्णाङ्क response = 0;

	प्रति (;;) {
		संरचना object_id old_oid, new_oid;
		वर्ण *शीर्ष;
		वर्ण *refname;
		वर्ण *p;
		गणना packet_read_स्थितिः स्थितिः;

		स्थितिः = packet_reader_read(reader);
		यदि (स्थितिः != PACKET_READ_प्रसामान्यम्) {
			/* Check whether proc-receive exited abnormally */
			यदि (स्थितिः == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				प्रत्यागम -1;
			}
			भङ्ग;
		}
		response++;

		शीर्ष = reader->पङ्क्तिः;
		p = strchr(शीर्ष, ' ');
		यदि (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", शीर्ष);
			संहिता = -1;
			अनुवर्त;
		}
		*p++ = '\0';
		यदि (!वाक्यं_तुलय(शीर्ष, "option")) {
			वर्ण *कुञ्चिका;
			ध्रुव वर्ण *val;

			यदि (!hint || !(report || new_report)) {
				यदि (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				संहिता = -1;
				अनुवर्त;
			}
			यदि (new_report) {
				यदि (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} अन्यथा {
					report = hint->report;
					यावत् (report->अग्र)
						report = report->अग्र;
					report->अग्र = xcalloc(1, परिमाण(संरचना ref_प्रक्षिप_report));
					report = report->अग्र;
				}
				new_report = 0;
			}
			कुञ्चिका = p;
			p = strchr(कुञ्चिका, ' ');
			यदि (p)
				*p++ = '\0';
			val = p;
			यदि (!वाक्यं_तुलय(कुञ्चिका, "refname"))
				report->ref_name = xstrdup_or_null(val);
			अन्यथा यदि (!वाक्यं_तुलय(कुञ्चिका, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			अन्यथा यदि (!वाक्यं_तुलय(कुञ्चिका, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			अन्यथा यदि (!वाक्यं_तुलय(कुञ्चिका, "forced-update"))
				report->forced_update = 1;
			अन्यथा यदि (!वाक्यं_तुलय(कुञ्चिका, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->सततम्_proc_receive = 0;
			अनुवर्त;
		}

		report = रिक्त;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		यदि (p)
			*p++ = '\0';
		यदि (वाक्यं_तुलय(शीर्ष, "ok") && वाक्यं_तुलय(शीर्ष, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    शीर्ष, refname);
			संहिता = -1;
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
			संहिता = -1;
			अनुवर्त;
		}
		यदि (!hint->सततम्_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			संहिता = -1;
			अनुवर्त;
		}
		hint->सततम्_proc_receive |= सततम्_PROC_RECEIVE_RETURNED;
		यदि (!वाक्यं_तुलय(शीर्ष, "ng")) {
			यदि (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			अन्यथा
				hint->error_string = "failed";
			संहिता = -1;
			अनुवर्त;
		}
		new_report = 1;
	}

	प्रति (cmd = commands; cmd; cmd = cmd->अग्र)
		यदि (cmd->सततम्_proc_receive && !cmd->error_string &&
		    !(cmd->सततम्_proc_receive & सततम्_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    संहिता = -1;
		}
	प्रत्यागम संहिता;
}

स्थिर पूर्णाङ्क सततम्_proc_receive_hook(संरचना command *commands,
				 ध्रुव संरचना string_list *प्रक्षिप_options)
{
	संरचना child_process proc = CHILD_PROCESS_INIT;
	संरचना async muxer;
	संरचना command *cmd;
	संरचना packet_reader reader;
	संरचना strbuf cap = STRBUF_INIT;
	संरचना strbuf errmsg = STRBUF_INIT;
	पूर्णाङ्क hook_use_प्रक्षिप_options = 0;
	पूर्णाङ्क संस्करणम् = 0;
	पूर्णाङ्क संहिता;
	ध्रुव वर्ण *hook_मार्गः = find_hook(the_repository, "proc-receive");

	यदि (!hook_मार्गः) {
		rp_error("cannot find hook 'proc-receive'");
		प्रत्यागम -1;
	}

	strvec_प्रक्षिप(&proc.तर्काः, hook_मार्गः);
	proc.अन्तर् = -1;
	proc.बहिर्गमनम् = -1;
	proc.trace2_hook_name = "proc-receive";

	यदि (use_sideband) {
		स्मृतिं_पूरय(&muxer, 0, परिमाण(muxer));
		muxer.proc = copy_गन्तव्यम्_sideband;
		muxer.अन्तर् = -1;
		संहिता = start_async(&muxer);
		यदि (संहिता)
			प्रत्यागम संहिता;
		proc.err = muxer.अन्तर्;
	} अन्यथा {
		proc.err = 0;
	}

	संहिता = start_command(&proc);
	यदि (संहिता) {
		यदि (use_sideband)
			finish_async(&muxer);
		प्रत्यागम संहिता;
	}

	sigchain_प्रक्षिप(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.बहिर्गमनम्, रिक्त, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	यदि (use_atomic)
		strbuf_addstr(&cap, " atomic");
	यदि (use_प्रक्षिप_options)
		strbuf_addstr(&cap, " push-options");
	यदि (cap.len) {
		संहिता = packet_write_fmt_gently(proc.अन्तर्, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} अन्यथा {
		संहिता = packet_write_fmt_gently(proc.अन्तर्, "version=1\n");
	}
	यदि (!संहिता)
		संहिता = packet_flush_gently(proc.अन्तर्);

	यदि (!संहिता)
		प्रति (;;) {
			पूर्णाङ्क linelen;
			गणना packet_read_स्थितिः स्थितिः;

			स्थितिः = packet_reader_read(&reader);
			यदि (स्थितिः != PACKET_READ_प्रसामान्यम्) {
				/* Check whether proc-receive exited abnormally */
				यदि (स्थितिः == PACKET_READ_EOF)
					संहिता = -1;
				भङ्ग;
			}

			यदि (reader.pktlen > 8 && starts_with(reader.पङ्क्तिः, "version=")) {
				संस्करणम् = atoi(reader.पङ्क्तिः + 8);
				linelen = वाक्यदैर्घ्यम्(reader.पङ्क्तिः);
				यदि (linelen < reader.pktlen) {
					ध्रुव वर्ण *feature_list = reader.पङ्क्तिः + linelen + 1;
					यदि (parse_feature_request(feature_list, "push-options"))
						hook_use_प्रक्षिप_options = 1;
				}
			}
		}

	यदि (संहिता) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		गच्छ cleanup;
	}

	विकल्प (संस्करणम्) {
	पक्ष 0:
		/* fallthrough */
	पक्ष 1:
		भङ्ग;
	स्वभाव:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    संस्करणम्);
		संहिता = -1;
		गच्छ cleanup;
	}

	/* Send commands */
	प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
		यदि (!cmd->सततम्_proc_receive || cmd->skip_update || cmd->error_string)
			अनुवर्त;
		संहिता = packet_write_fmt_gently(proc.अन्तर्, "%s %s %s",
					       oid_गन्तव्यम्_hex(&cmd->old_oid),
					       oid_गन्तव्यम्_hex(&cmd->new_oid),
					       cmd->ref_name);
		यदि (संहिता)
			भङ्ग;
	}
	यदि (!संहिता)
		संहिता = packet_flush_gently(proc.अन्तर्);
	यदि (संहिता) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		गच्छ cleanup;
	}

	/* Send push options */
	यदि (hook_use_प्रक्षिप_options) {
		संरचना string_list_item *item;

		for_each_string_list_item(item, प्रक्षिप_options) {
			संहिता = packet_write_fmt_gently(proc.अन्तर्, "%s", item->string);
			यदि (संहिता)
				भङ्ग;
		}
		यदि (!संहिता)
			संहिता = packet_flush_gently(proc.अन्तर्);
		यदि (संहिता) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			गच्छ cleanup;
		}
	}

	/* Read result from proc-receive */
	संहिता = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.अन्तर्);
	close(proc.बहिर्गमनम्);
	यदि (use_sideband)
		finish_async(&muxer);
	यदि (finish_command(&proc))
		संहिता = -1;
	यदि (errmsg.len >0) {
		वर्ण *p = errmsg.buf;

		p += errmsg.len - 1;
		यदि (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_निष्कासय(SIGPIPE);

	प्रत्यागम संहिता;
}

स्थिर ध्रुव वर्ण *refuse_unconfigured_deny_सन्देशः =
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
	rp_error("%s", _(refuse_unconfigured_deny_सन्देशः));
}

स्थिर ध्रुव वर्ण *refuse_unconfigured_deny_delete_current_सन्देशः =
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
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_सन्देशः));
}

स्थिर ध्रुव संरचना object_id *command_singleton_iterator(शून्य *cb_दत्त);
स्थिर पूर्णाङ्क update_shallow_ref(संरचना command *cmd, संरचना shallow_info *si)
{
	संरचना shallow_तालकम् shallow_तालकम् = SHALLOW_तालकम्_INIT;
	संरचना oid_array extra = OID_ARRAY_INIT;
	संरचना check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t आवरणम् = 1 << (cmd->सूचकांक % 32);
	पूर्णाङ्क i;

	trace_printf_कुञ्चिका(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	प्रति (i = 0; i < si->shallow->nr; i++)
		यदि (si->प्रयुक्तम्_shallow[i] &&
		    (si->प्रयुक्तम्_shallow[i][cmd->सूचकांक / 32] & आवरणम्) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	सज्जता_alternate_shallow(&shallow_तालकम्, &opt.shallow_file, &extra);
	यदि (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_तालकम्);
		oid_array_clear(&extra);
		प्रत्यागम -1;
	}

	commit_shallow_file(the_repository, &shallow_तालकम्);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	प्रति (i = 0; i < extra.nr; i++)
		पञ्जिकाकोशः_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->सूचकांक] = 0;
	oid_array_clear(&extra);
	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *प्रक्षिप_गन्तव्यम्_deploy(अचिह्नित वर्ण *sha1,
				  संरचना strvec *env,
				  ध्रुव संरचना worktree *worktree)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.तर्काः, "update-index", "-q", "--ignore-submodules",
		     "--refresh", रिक्त);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->मार्गः;
	child.no_stdin = 1;
	child.stdout_गन्तव्यम्_stderr = 1;
	child.git_cmd = 1;
	यदि (सततम्_command(&child))
		प्रत्यागम "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.तर्काः, "diff-files", "--quiet",
		     "--ignore-submodules", "--", रिक्त);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->मार्गः;
	child.no_stdin = 1;
	child.stdout_गन्तव्यम्_stderr = 1;
	child.git_cmd = 1;
	यदि (सततम्_command(&child))
		प्रत्यागम "Working directory has unstaged changes";

	child_process_init(&child);
	strvec_pushl(&child.तर्काः, "diff-index", "--quiet", "--cached",
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
		     रिक्तानि_वृक्षः_oid_hex(the_repository->hash_algo), "--", रिक्त);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_गन्तव्यम्_stderr = 0;
	child.git_cmd = 1;
	यदि (सततम्_command(&child))
		प्रत्यागम "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.तर्काः, "read-tree", "-u", "-m", hash_गन्तव्यम्_hex(sha1),
		     रिक्त);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->मार्गः;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_गन्तव्यम्_stderr = 0;
	child.git_cmd = 1;
	यदि (सततम्_command(&child))
		प्रत्यागम "Could not update working tree to new HEAD";

	प्रत्यागम रिक्त;
}

स्थिर ध्रुव वर्ण *प्रक्षिप_गन्तव्यम्_checkout_hook = "push-to-checkout";

स्थिर ध्रुव वर्ण *प्रक्षिप_गन्तव्यम्_checkout(अचिह्नित वर्ण *hash,
				    पूर्णाङ्क *invoked_hook,
				    संरचना strvec *env,
				    ध्रुव वर्ण *work_वृक्षः)
{
	संरचना सततम्_hooks_opt opt = सततम्_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_मार्गः(work_वृक्षः));
	strvec_प्रक्षिप(&opt.तर्काः, hash_गन्तव्यम्_hex(hash));
	यदि (सततम्_hooks_opt(the_repository, प्रक्षिप_गन्तव्यम्_checkout_hook, &opt))
		प्रत्यागम "push-to-checkout hook declined";
	अन्यथा
		प्रत्यागम रिक्त;
}

स्थिर ध्रुव वर्ण *update_worktree(अचिह्नित वर्ण *sha1, ध्रुव संरचना worktree *worktree)
{
	ध्रुव वर्ण *retval;
	वर्ण *git_dir;
	संरचना strvec env = STRVEC_INIT;
	पूर्णाङ्क invoked_hook;

	यदि (!worktree || !worktree->मार्गः)
		BUG("worktree->path must be non-NULL");

	यदि (worktree->is_bare)
		प्रत्यागम "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_मार्गः(git_dir));

	retval = प्रक्षिप_गन्तव्यम्_checkout(sha1, &invoked_hook, &env, worktree->मार्गः);
	यदि (!invoked_hook)
		retval = प्रक्षिप_गन्तव्यम्_deploy(sha1, &env, worktree);

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
		गच्छ बहिर्गमनम्;
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
			गच्छ बहिर्गमनम्;
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
		      "but I can't find it!", oid_गन्तव्यम्_hex(new_oid));
		ret = "bad pack";
		गच्छ बहिर्गमनम्;
	}

	यदि (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		यदि (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			गच्छ बहिर्गमनम्;
		}

		यदि (worktree || (शीर्ष_name && !वाक्यं_तुलय(namespaced_name, शीर्ष_name))) {
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
				गच्छ बहिर्गमनम्;
			स्वभाव:
				ret = "Invalid denyDeleteCurrent setting";
				गच्छ बहिर्गमनम्;
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
			गच्छ बहिर्गमनम्;
		}
		old_commit = (संरचना commit *)old_object;
		new_commit = (संरचना commit *)new_object;
		ret2 = repo_अन्तर्_merge_bases(the_repository, old_commit, new_commit);
		यदि (ret2 < 0)
			निर्गच्छ(128);
		यदि (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			गच्छ बहिर्गमनम्;
		}
	}
	यदि (सततम्_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		गच्छ बहिर्गमनम्;
	}

	यदि (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		यदि (ret)
			गच्छ बहिर्गमनम्;
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
		यदि (shallow_update && si->shallow_ref[cmd->सूचकांक] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			गच्छ बहिर्गमनम्;
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
				ret = ref_transaction_error_सन्देशः(tx_err);
		} अन्यथा {
			ret = रिक्त; /* good */
		}
		strbuf_release(&err);
	}

बहिर्गमनम्:
	free_worktrees(worktrees);
	प्रत्यागम ret;
}

स्थिर शून्य सततम्_update_post_hook(संरचना command *commands)
{
	स्थिर ध्रुव वर्ण hook_name[] = "post-update";
	संरचना सततम्_hooks_opt opt = सततम्_HOOKS_OPT_INIT;
	संरचना async sideband_async;
	संरचना command *cmd;
	पूर्णाङ्क sideband_async_started = 0;
	पूर्णाङ्क saved_stderr = -1;

	यदि (!hook_exists(the_repository, hook_name))
		प्रत्यागम;

	प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
		यदि (cmd->error_string || cmd->did_not_exist)
			अनुवर्त;
		strvec_प्रक्षिप(&opt.तर्काः, cmd->ref_name);
	}
	यदि (!opt.तर्काः.nr)
		प्रत्यागम;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	सततम्_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

स्थिर शून्य check_aliased_update_आन्तरिकम्(संरचना command *cmd,
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
	check_aliased_update_आन्तरिकम्(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

स्थिर शून्य check_aliased_updates(संरचना command *commands)
{
	संरचना command *cmd;
	संरचना string_list ref_list = STRING_LIST_INIT_NODUP;

	प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
		संरचना string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (शून्य *)cmd;
	}
	string_list_क्रमय(&ref_list);

	प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
		यदि (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

स्थिर ध्रुव संरचना object_id *command_singleton_iterator(शून्य *cb_दत्त)
{
	संरचना command **cmd_list = cb_दत्त;
	संरचना command *cmd = *cmd_list;

	यदि (!cmd || is_null_oid(&cmd->new_oid))
		प्रत्यागम रिक्त;
	*cmd_list = रिक्त; /* this returns only one */
	प्रत्यागम &cmd->new_oid;
}

स्थिर शून्य समुच्चयः_connectivity_errors(संरचना command *commands,
				    संरचना shallow_info *si,
				    संरचना odb_transaction *transaction)
{
	संरचना command *cmd;

	प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
		संरचना command *singleton = cmd;
		संरचना check_connected_options opt = CHECK_CONNECTED_INIT;
		संरचना strvec env = STRVEC_INIT;

		यदि (shallow_update && si->shallow_ref[cmd->सूचकांक])
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

संरचना iterate_दत्त {
	संरचना command *cmds;
	संरचना shallow_info *si;
};

स्थिर ध्रुव संरचना object_id *iterate_receive_command_list(शून्य *cb_दत्त)
{
	संरचना iterate_दत्त *दत्त = cb_दत्त;
	संरचना command **cmd_list = &दत्त->cmds;
	संरचना command *cmd = *cmd_list;

	प्रति (; cmd; cmd = cmd->अग्र) {
		यदि (shallow_update && दत्त->si->shallow_ref[cmd->सूचकांक])
			/* to be checked in update_shallow_ref() */
			अनुवर्त;
		यदि (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->अग्र;
			प्रत्यागम &cmd->new_oid;
		}
	}
	प्रत्यागम रिक्त;
}

स्थिर शून्य reject_updates_गन्तव्यम्_hidden(संरचना command *commands)
{
	संरचना strbuf refname_पूर्णम् = STRBUF_INIT;
	आकार_t उपसर्गः_len;
	संरचना command *cmd;

	strbuf_addstr(&refname_पूर्णम्, get_git_namespace());
	उपसर्गः_len = refname_पूर्णम्.len;

	प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
		यदि (cmd->error_string)
			अनुवर्त;

		strbuf_setlen(&refname_पूर्णम्, उपसर्गः_len);
		strbuf_addstr(&refname_पूर्णम्, cmd->ref_name);

		यदि (!ref_is_hidden(cmd->ref_name, refname_पूर्णम्.buf, &hidden_refs))
			अनुवर्त;
		यदि (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		अन्यथा
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_पूर्णम्);
}

स्थिर पूर्णाङ्क should_process_cmd(संरचना command *cmd)
{
	प्रत्यागम !cmd->error_string && !cmd->skip_update;
}

स्थिर शून्य BUG_if_skipped_connectivity_check(संरचना command *commands,
					       संरचना shallow_info *si)
{
	संरचना command *cmd;

	प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
		यदि (should_process_cmd(cmd) && si->shallow_ref[cmd->सूचकांक])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

स्थिर शून्य ref_transaction_rejection_handler(ध्रुव वर्ण *refname,
					      ध्रुव संरचना object_id *old_oid अप्रयुक्तम्,
					      ध्रुव संरचना object_id *new_oid अप्रयुक्तम्,
					      ध्रुव वर्ण *old_target अप्रयुक्तम्,
					      ध्रुव वर्ण *new_target अप्रयुक्तम्,
					      गणना ref_transaction_error err,
					      ध्रुव वर्ण *details,
					      शून्य *cb_दत्त)
{
	संरचना strmap *failed_refs = cb_दत्त;

	यदि (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (वर्ण *)ref_transaction_error_सन्देशः(err));
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
		प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
			यदि (!should_process_cmd(cmd) || cmd->सततम्_proc_receive)
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

		यदि (strmap_रिक्तानि(&failed_refs))
			गच्छ cleanup;

	failure:
		प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
			यदि (reported_error)
				cmd->error_string = reported_error;
			अन्यथा यदि (strmap_अन्तर्भवति(&failed_refs, cmd->ref_name))
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

	प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
		यदि (!should_process_cmd(cmd) || cmd->सततम्_proc_receive)
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
	प्रति (cmd = commands; cmd; cmd = cmd->अग्र)
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
			     ध्रुव संरचना string_list *प्रक्षिप_options)
{
	संरचना check_connected_options opt = CHECK_CONNECTED_INIT;
	संरचना command *cmd;
	संरचना iterate_दत्त दत्त;
	संरचना async muxer;
	पूर्णाङ्क err_fd = 0;
	पूर्णाङ्क सततम्_proc_receive = 0;

	यदि (unpacker_error) {
		प्रति (cmd = commands; cmd; cmd = cmd->अग्र)
			cmd->error_string = "unpacker error";
		प्रत्यागम;
	}

	यदि (!skip_connectivity_check) {
		संरचना strvec env = STRVEC_INIT;

		यदि (use_sideband) {
			स्मृतिं_पूरय(&muxer, 0, परिमाण(muxer));
			muxer.proc = copy_गन्तव्यम्_sideband;
			muxer.अन्तर् = -1;
			यदि (!start_async(&muxer))
				err_fd = muxer.अन्तर्;
			/* ...else, continue without relaying sideband */
		}

		दत्त.cmds = commands;
		दत्त.si = si;
		opt.err_fd = err_fd;
		opt.प्रगतिः = err_fd && !quiet;
		odb_transaction_env(transaction, &env);
		opt.env = env.v;
		opt.exclude_hidden_refs_section = "receive";

		यदि (check_connected(iterate_receive_command_list, &दत्त, &opt))
			समुच्चयः_connectivity_errors(commands, si, transaction);

		यदि (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_गन्तव्यम्_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	यदि (proc_receive_ref) {
		प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
			यदि (!should_process_cmd(cmd))
				अनुवर्त;

			यदि (proc_receive_ref_matches(cmd)) {
				cmd->सततम्_proc_receive = सततम्_PROC_RECEIVE_SCHEDULED;
				सततम्_proc_receive = 1;
			}
		}
	}

	यदि (सततम्_receive_hook(commands, "pre-receive", 0, transaction, प्रक्षिप_options)) {
		प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
			यदि (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		प्रत्यागम;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	प्रति (cmd = commands; cmd && cmd->error_string; cmd = cmd->अग्र)
		; /* nothing */
	यदि (!cmd)
		प्रत्यागम;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	यदि (odb_transaction_commit(transaction)) {
		प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
			यदि (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		प्रत्यागम;
	}

	check_aliased_updates(commands);

	मुक्त(शीर्ष_name_गन्तव्यम्_free);
	शीर्ष_name = शीर्ष_name_गन्तव्यम्_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, रिक्त,
							    रिक्त);

	यदि (सततम्_proc_receive &&
	    सततम्_proc_receive_hook(commands, प्रक्षिप_options))
		प्रति (cmd = commands; cmd; cmd = cmd->अग्र)
			यदि (!cmd->error_string &&
			    !(cmd->सततम्_proc_receive & सततम्_PROC_RECEIVE_RETURNED) &&
			    (cmd->सततम्_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	यदि (use_atomic)
		execute_commands_atomic(commands, si);
	अन्यथा
		execute_commands_non_atomic(commands, si);

	यदि (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

स्थिर संरचना command **queue_command(संरचना command **पुच्छ,
				      ध्रुव वर्ण *पङ्क्तिः,
				      पूर्णाङ्क linelen)
{
	संरचना object_id old_oid, new_oid;
	संरचना command *cmd;
	ध्रुव वर्ण *refname;
	पूर्णाङ्क reflen;
	ध्रुव वर्ण *p;

	यदि (parse_oid_hex(पङ्क्तिः, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", पङ्क्तिः);

	refname = p;
	reflen = linelen - (p - पङ्क्तिः);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*पुच्छ = cmd;
	प्रत्यागम &cmd->अग्र;
}

स्थिर शून्य free_commands(संरचना command *commands)
{
	यावत् (commands) {
		संरचना command *अग्र = commands->अग्र;

		ref_प्रक्षिप_report_free(commands->report);
		मुक्त(commands->error_string_owned);
		मुक्त(commands);
		commands = अग्र;
	}
}

स्थिर शून्य queue_commands_from_cert(संरचना command **पुच्छ,
				     संरचना strbuf *प्रक्षिप_cert)
{
	ध्रुव वर्ण *boc, *eoc;

	यदि (*पुच्छ)
		die("protocol error: got both push certificate and unsigned commands");

	boc = वाक्ये_अन्विष(प्रक्षिप_cert->buf, "\n\n");
	यदि (!boc)
		die("malformed push certificate %.*s", 100, प्रक्षिप_cert->buf);
	अन्यथा
		boc += 2;
	eoc = प्रक्षिप_cert->buf + parse_signed_बफर(प्रक्षिप_cert->buf, प्रक्षिप_cert->len);

	यावत् (boc < eoc) {
		ध्रुव वर्ण *eol = memchr(boc, '\n', eoc - boc);
		पुच्छ = queue_command(पुच्छ, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

स्थिर संरचना command *read_शीर्ष_info(संरचना packet_reader *reader,
				      संरचना oid_array *shallow,
				      गणना report_स्थितिः_संस्करणम् *संस्करणम्)
{
	संरचना command *commands = रिक्त;
	संरचना command **p = &commands;
	प्रति (;;) {
		पूर्णाङ्क linelen;

		यदि (packet_reader_read(reader) != PACKET_READ_प्रसामान्यम्)
			भङ्ग;

		यदि (reader->pktlen > 8 && starts_with(reader->पङ्क्तिः, "shallow ")) {
			संरचना object_id oid;
			यदि (get_oid_hex(reader->पङ्क्तिः + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->पङ्क्तिः + 8);
			oid_array_append(shallow, &oid);
			अनुवर्त;
		}

		linelen = वाक्यदैर्घ्यम्(reader->पङ्क्तिः);
		यदि (linelen < reader->pktlen) {
			ध्रुव वर्ण *feature_list = reader->पङ्क्तिः + linelen + 1;
			ध्रुव वर्ण *hash = रिक्त;
			ध्रुव वर्ण *client_sid;
			आकार_t len = 0;
			यदि (parse_feature_request(feature_list, "report-status"))
				*संस्करणम् = REPORT_स्थितिः_V0;
			यदि (parse_feature_request(feature_list, "report-status-v2"))
				*संस्करणम् = REPORT_स्थितिः_V2;
			यदि (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			यदि (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			यदि (advertise_atomic_प्रक्षिप
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			यदि (advertise_प्रक्षिप_options
			    && parse_feature_request(feature_list, "push-options"))
				use_प्रक्षिप_options = 1;
			hash = parse_feature_मानम्(feature_list, "object-format", &len, रिक्त);
			यदि (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = वाक्यदैर्घ्यम्(hash);
			}
			यदि (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_मानम्(feature_list, "session-id", &len, रिक्त);
			यदि (client_sid) {
				वर्ण *sid = xstrndup(client_sid, len);
				trace2_दत्त_string("transfer", रिक्त, "client-sid", client_sid);
				मुक्त(sid);
			}
		}

		यदि (!वाक्यं_तुलय(reader->पङ्क्तिः, "push-cert")) {
			पूर्णाङ्क true_flush = 0;
			पूर्णाङ्क saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			प्रति (;;) {
				packet_reader_read(reader);
				यदि (reader->स्थितिः == PACKET_READ_FLUSH) {
					true_flush = 1;
					भङ्ग;
				}
				यदि (reader->स्थितिः != PACKET_READ_प्रसामान्यम्) {
					die("protocol error: got an unexpected packet");
				}
				यदि (!वाक्यं_तुलय(reader->पङ्क्तिः, "push-cert-end\n"))
					भङ्ग; /* end of cert */
				strbuf_addstr(&प्रक्षिप_cert, reader->पङ्क्तिः);
			}
			reader->options = saved_options;

			यदि (true_flush)
				भङ्ग;
			अनुवर्त;
		}

		p = queue_command(p, reader->पङ्क्तिः, linelen);
	}

	यदि (प्रक्षिप_cert.len)
		queue_commands_from_cert(p, &प्रक्षिप_cert);

	प्रत्यागम commands;
}

स्थिर शून्य read_प्रक्षिप_options(संरचना packet_reader *reader,
			      संरचना string_list *options)
{
	यावत् (1) {
		यदि (packet_reader_read(reader) != PACKET_READ_प्रसामान्यम्)
			भङ्ग;

		string_list_append(options, reader->पङ्क्तिः);
	}
}

स्थिर पूर्णाङ्क unpack_with_sideband(संरचना odb_transaction *transaction,
				ध्रुव वर्ण *shallow_file,
				संरचना strbuf *err_सन्देशः)
{
	संरचना odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_सन्देशः_types = fsck_सन्देशः_types.buf,
		.max_निवेशः_आकार = max_निवेशः_आकार,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	संरचना async muxer;
	पूर्णाङ्क ret;

	यदि (!use_sideband)
		प्रत्यागम odb_transaction_write_pack(transaction, 0, err_सन्देशः, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	स्मृतिं_पूरय(&muxer, 0, परिमाण(muxer));
	muxer.proc = copy_गन्तव्यम्_sideband;
	muxer.अन्तर् = -1;
	यदि (start_async(&muxer))
		प्रत्यागम 0;

	opts.err_fd = muxer.अन्तर्;
	ret = odb_transaction_write_pack(transaction, 0, err_सन्देशः, &opts);

	finish_async(&muxer);
	प्रत्यागम ret;
}

स्थिर शून्य prepare_shallow_update(संरचना shallow_info *si,
				   ध्रुव वर्ण *shallow_file)
{
	पूर्णाङ्क i, j, k, bitmap_आकार = DIV_आवर्तनम्_ऊर्ध्वम्(si->ref->nr, 32);

	ALLOC_ARRAY(si->प्रयुक्तम्_shallow, si->shallow->nr);
	नियोजय_shallow_commits_गन्तव्यम्_refs(si, si->प्रयुक्तम्_shallow, रिक्त);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	प्रति (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	प्रति (i = 0; i < si->shallow->nr; i++) {
		यदि (!si->प्रयुक्तम्_shallow[i])
			अनुवर्त;
		प्रति (j = 0; j < bitmap_आकार; j++) {
			यदि (!si->प्रयुक्तम्_shallow[i][j])
				अनुवर्त;
			si->need_reachability_test[i]++;
			प्रति (k = 0; k < 32; k++)
				यदि (si->प्रयुक्तम्_shallow[i][j] & (1U << k))
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
	पूर्णाङ्क *ref_स्थितिः;
	remove_nonexistent_theirs_shallow(si);
	यदि (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		प्रत्यागम;
	}

	प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
		यदि (is_null_oid(&cmd->new_oid))
			अनुवर्त;
		oid_array_append(ref, &cmd->new_oid);
		cmd->सूचकांक = ref->nr - 1;
	}
	si->ref = ref;

	यदि (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		प्रत्यागम;
	}

	ALLOC_ARRAY(ref_स्थितिः, ref->nr);
	नियोजय_shallow_commits_गन्तव्यम्_refs(si, रिक्त, ref_स्थितिः);
	प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
		यदि (is_null_oid(&cmd->new_oid))
			अनुवर्त;
		यदि (ref_स्थितिः[cmd->सूचकांक]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	मुक्त(ref_स्थितिः);
}

स्थिर शून्य override_cmds_error(संरचना command *commands, ध्रुव वर्ण *err)
{
	प्रति (संरचना command *cmd = commands; cmd; cmd = cmd->अग्र) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
स्थिर शून्य generate_report(संरचना strbuf *buf, संरचना command *commands,
			    ध्रुव संरचना strbuf *unpack_स्थितिः,
			    गणना report_स्थितिः_संस्करणम् संस्करणम्)
{
	संरचना command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_स्थितिः->len ? unpack_स्थितिः->buf : "ok");

	प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
		संरचना ref_प्रक्षिप_report *report;
		पूर्णाङ्क गणनम् = 0;

		यदि (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		अन्यथा
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		यदि (संस्करणम् != REPORT_स्थितिः_V2 || cmd->error_string)
			अनुवर्त;

		प्रति (report = cmd->report; report; report = report->अग्र) {
			यदि (गणनम्++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			यदि (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			यदि (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_गन्तव्यम्_hex(report->old_oid));
			यदि (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_गन्तव्यम्_hex(report->new_oid));
			यदि (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

स्थिर शून्य report(संरचना command *commands, ध्रुव संरचना strbuf *unpack_स्थितिः,
		   गणना report_स्थितिः_संस्करणम् संस्करणम्)
{
	संरचना strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_स्थितिः, संस्करणम्);

	यदि (सततम्_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_स्थितिः, संस्करणम्);
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
	प्रति (cmd = commands; cmd; cmd = cmd->अग्र) {
		यदि (!is_null_oid(&cmd->new_oid))
			प्रत्यागम 0;
	}
	प्रत्यागम 1;
}

पूर्णाङ्क cmd_receive_pack(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *उपसर्गः,
		     संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क advertise_refs = 0;
	संरचना command *commands;
	संरचना oid_array shallow = OID_ARRAY_INIT;
	संरचना oid_array ref = OID_ARRAY_INIT;
	संरचना shallow_info si;
	संरचना packet_reader reader;
	संरचना odb_transaction *transaction = रिक्त;
	गणना report_स्थितिः_संस्करणम् संस्करणम् = REPORT_स्थितिः_UNKNOWN;

	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, रिक्त),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, रिक्त),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, रिक्त),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, रिक्त),
		OPT_अन्तः()
	};

	packet_trace_तटस्थः("receive-pack");

	argc = parse_options(argc, argv, उपसर्गः, options, receive_pack_usage, 0);

	यदि (argc > 1)
		usage_सन्देशः_opt(_("too many arguments"), receive_pack_usage, options);
	यदि (argc == 0)
		usage_सन्देशः_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	सज्जता_मार्गः();

	यदि (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, रिक्त);
	यदि (cert_nonce_बीजम्)
		प्रक्षिप_cert_nonce = prepare_प्रक्षिप_cert_nonce(service_dir, time(रिक्त));

	विकल्प (determine_protocol_संस्करणम्_server()) {
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
	पक्ष protocol_unknown_संस्करणम्:
		BUG("unknown protocol version");
	}

	यदि (advertise_refs || !stateless_rpc) {
		write_शीर्ष_info();
	}
	यदि (advertise_refs)
		प्रत्यागम 0;

	packet_reader_init(&reader, 0, रिक्त, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	यदि ((commands = read_शीर्ष_info(&reader, &shallow, &संस्करणम्))) {
		संरचना string_list प्रक्षिप_options = STRING_LIST_INIT_DUP;
		संरचना strbuf unpack_स्थितिः = STRBUF_INIT;

		यदि (use_प्रक्षिप_options)
			read_प्रक्षिप_options(&reader, &प्रक्षिप_options);
		यदि (!check_cert_प्रक्षिप_options(&प्रक्षिप_options)) {
			संरचना command *cmd;
			प्रति (cmd = commands; cmd; cmd = cmd->अग्र)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		यदि (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		यदि (!delete_only(commands)) {
			ध्रुव वर्ण *alt_shallow_file = रिक्त;

			यदि (si.nr_ours || si.nr_theirs)
				alt_shallow_file = सज्जता_temporary_shallow(si.shallow);

			यदि (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_स्थितिः, "unable to start object transaction");
			अन्यथा
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_स्थितिः);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_स्थितिः.len, &si, transaction,
				 &प्रक्षिप_options);
		odb_transaction_finalize(transaction);
		sigchain_प्रक्षिप(SIGPIPE, SIG_IGN);

		विकल्प (संस्करणम्) {
		पक्ष REPORT_स्थितिः_V2:
		पक्ष REPORT_स्थितिः_V0:
			report(commands, &unpack_स्थितिः, संस्करणम्);
			भङ्ग;
		पक्ष REPORT_स्थितिः_UNKNOWN:
			भङ्ग;
		}

		sigchain_निष्कासय(SIGPIPE);
		सततम्_receive_hook(commands, "post-receive", 1, रिक्त,
				 &प्रक्षिप_options);
		सततम्_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&प्रक्षिप_options, 0);
		यदि (auto_gc) {
			संरचना child_process proc = CHILD_PROCESS_INIT;

			यदि (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_गन्तव्यम्_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				यदि (!start_command(&proc)) {
					यदि (use_sideband)
						copy_गन्तव्यम्_sideband(proc.err, -1, रिक्त);
					finish_command(&proc);
				}
			}
		}
		यदि (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_स्थितिः);
	}
	यदि (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	मुक्त((शून्य *)प्रक्षिप_cert_nonce);
	प्रत्यागम 0;
}
