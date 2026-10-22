#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "connect.h"
#అన్తర్భావ "connected.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "exec-cmd.h"
#అన్తర్భావ "fsck.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "gpg-interface.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "hook.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "oidset.h"
#అన్తర్భావ "pack.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "pkt-line.h"
#అన్తర్భావ "protocol.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "server-info.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "shallow.h"
#అన్తర్భావ "sideband.h"
#అన్తర్భావ "sigchain.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "trace.h"
#అన్తర్భావ "trace2.h"
#అన్తర్భావ "version.h"
#అన్తర్భావ "worktree.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	ఖాళీ
};

గణన deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

గణన report_స్థితి_సంస్కరణ {
	REPORT_స్థితి_UNKNOWN = 0,
	REPORT_స్థితి_V0,
	REPORT_స్థితి_V2,
};

స్థిర పూర్ణాంకం deny_deletes;
స్థిర పూర్ణాంకం deny_non_fast_forwards;
స్థిర గణన deny_action deny_current_branch = DENY_UNCONFIGURED;
స్థిర గణన deny_action deny_delete_current = DENY_UNCONFIGURED;
స్థిర పూర్ణాంకం receive_fsck_objects = -1;
స్థిర పూర్ణాంకం transfer_fsck_objects = -1;
స్థిర నిర్మాణం strbuf fsck_సందేశం_types = STRBUF_INIT;
స్థిర పూర్ణాంకం advertise_atomic_నెట్టు = 1;
స్థిర పూర్ణాంకం advertise_నెట్టు_options;
స్థిర పూర్ణాంకం advertise_sid;
స్థిర off_t max_ఆదానం_కొలత;
స్థిర పూర్ణాంకం use_sideband;
స్థిర పూర్ణాంకం use_atomic;
స్థిర పూర్ణాంకం use_నెట్టు_options;
స్థిర పూర్ణాంకం quiet;
స్థిర పూర్ణాంకం prefer_ofs_delta = 1;
స్థిర పూర్ణాంకం auto_update_server_info;
స్థిర పూర్ణాంకం auto_gc = 1;
స్థిర పూర్ణాంకం reject_thin;
స్థిర పూర్ణాంకం skip_connectivity_check;
స్థిర పూర్ణాంకం stateless_rpc;
స్థిర స్థిరరాశి అక్షరం *service_dir;
స్థిర స్థిరరాశి అక్షరం *తల_name;
స్థిర శూన్య *తల_name_గమ్యం_free;
స్థిర పూర్ణాంకం పంపిన_capabilities;
స్థిర పూర్ణాంకం shallow_update;
స్థిర నిర్మాణం strbuf నెట్టు_cert = STRBUF_INIT;
స్థిర నిర్మాణం object_id నెట్టు_cert_oid;
స్థిర నిర్మాణం signature_check sigcheck;
స్థిర స్థిరరాశి అక్షరం *నెట్టు_cert_nonce;
స్థిర అక్షరం *cert_nonce_విత్తనం;
స్థిర నిర్మాణం strvec hidden_refs = STRVEC_INIT;

స్థిర స్థిరరాశి అక్షరం *NONCE_UNSOLICITED = "UNSOLICITED";
స్థిర స్థిరరాశి అక్షరం *NONCE_BAD = "BAD";
స్థిర స్థిరరాశి అక్షరం *NONCE_MISSING = "MISSING";
స్థిర స్థిరరాశి అక్షరం *NONCE_OK = "OK";
స్థిర స్థిరరాశి అక్షరం *NONCE_SLOP = "SLOP";
స్థిర స్థిరరాశి అక్షరం *nonce_స్థితి;
స్థిర దీర్ఘ nonce_stamp_slop;
స్థిర timestamp_t nonce_stamp_slop_హద్దు;
స్థిర నిర్మాణం ref_transaction *transaction;

స్థిర గణన {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
స్థిర పూర్ణాంకం keepalive_లోపల_sec = 5;

స్థిర నిర్మాణం proc_receive_ref {
	గుర్తులేని పూర్ణాంకం want_కూడు:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	అక్షరం *ref_ఉపసర్గ;
	నిర్మాణం proc_receive_ref *తదుపరి;
} *proc_receive_ref;

స్థిర శూన్య proc_receive_ref_append(స్థిరరాశి అక్షరం *ఉపసర్గ);

స్థిర గణన deny_action parse_deny_action(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ)
{
	ఒకవేళ (విలువ) {
		ఒకవేళ (!strcasecmp(విలువ, "ignore"))
			తిరిగి DENY_IGNORE;
		ఒకవేళ (!strcasecmp(విలువ, "warn"))
			తిరిగి DENY_WARN;
		ఒకవేళ (!strcasecmp(విలువ, "refuse"))
			తిరిగి DENY_REFUSE;
		ఒకవేళ (!strcasecmp(విలువ, "updateinstead"))
			తిరిగి DENY_UPDATE_INSTEAD;
	}
	ఒకవేళ (git_config_bool(var, విలువ))
		తిరిగి DENY_REFUSE;
	తిరిగి DENY_IGNORE;
}

స్థిర పూర్ణాంకం receive_pack_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			       స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	స్థిరరాశి అక్షరం *సందేశం_id_2;
	పూర్ణాంకం స్థితి_2 = parse_hide_refs_config(var, విలువ, "receive", &hidden_refs);

	ఒకవేళ (స్థితి_2)
		తిరిగి స్థితి_2;

	ఒకవేళ (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.fsck.skiplist") == 0) {
		అక్షరం *మార్గం;

		ఒకవేళ (git_config_pathname(&మార్గం, var, విలువ))
			తిరిగి -1;
		ఒకవేళ (మార్గం)
			strbuf_addf(&fsck_సందేశం_types, "%cskiplist=%s",
				    fsck_సందేశం_types.len ? ',' : '=', మార్గం);
		విడుదల(మార్గం);
		తిరిగి 0;
	}

	ఒకవేళ (skip_ఉపసర్గ(var, "receive.fsck.", &సందేశం_id_2)) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		ఒకవేళ (is_valid_సందేశం_type(సందేశం_id_2, విలువ))
			strbuf_addf(&fsck_సందేశం_types, "%c%s=%s",
				fsck_సందేశం_types.len ? ',' : '=', సందేశం_id_2, విలువ);
		లేకపోతే
			warning("skipping unknown msg id '%s'", సందేశం_id_2);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.certnonceseed") == 0)
		తిరిగి git_config_string(&cert_nonce_విత్తనం, var, విలువ);

	ఒకవేళ (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_హద్దు = git_config_ulong(var, విలువ, ctx->kvi);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_నెట్టు = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_నెట్టు_options = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.keepalive") == 0) {
		keepalive_లోపల_sec = git_config_int(var, విలువ, ctx->kvi);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.maxinputsize") == 0) {
		max_ఆదానం_కొలత = git_config_int64(var, విలువ, ctx->kvi);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "receive.procreceiverefs") == 0) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		proc_receive_ref_append(విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

స్థిర శూన్య show_ref(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి నిర్మాణం object_id *oid)
{
	ఒకవేళ (పంపిన_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_గమ్యం_hex(oid), మార్గం);
	} లేకపోతే {
		నిర్మాణం strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		ఒకవేళ (advertise_atomic_నెట్టు)
			strbuf_addstr(&cap, " atomic");
		ఒకవేళ (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		ఒకవేళ (నెట్టు_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", నెట్టు_cert_nonce);
		ఒకవేళ (advertise_నెట్టు_options)
			strbuf_addstr(&cap, " push-options");
		ఒకవేళ (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_గమ్యం_hex(oid), మార్గం, 0, cap.buf);
		strbuf_release(&cap);
		పంపిన_capabilities = 1;
	}
}

స్థిర పూర్ణాంకం show_ref_cb(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *దత్తాంశం)
{
	నిర్మాణం oidset *చూసిన = దత్తాంశం;
	స్థిరరాశి అక్షరం *మార్గం = strip_namespace(ref->name);

	ఒకవేళ (ref_is_hidden(మార్గం, ref->name, &hidden_refs))
		తిరిగి 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	ఒకవేళ (!మార్గం) {
		ఒకవేళ (oidset_చొప్పించు(చూసిన, ref->oid))
			తిరిగి 0;
		మార్గం = ".have";
	} లేకపోతే {
		oidset_చొప్పించు(చూసిన, ref->oid);
	}
	show_ref(మార్గం, ref->oid);
	తిరిగి 0;
}

స్థిర శూన్య show_one_alternate_ref(స్థిరరాశి నిర్మాణం object_id *oid,
				   శూన్య *దత్తాంశం)
{
	నిర్మాణం oidset *చూసిన = దత్తాంశం;

	ఒకవేళ (oidset_చొప్పించు(చూసిన, oid))
		తిరిగి;

	show_ref(".have", oid);
}

స్థిర శూన్య write_తల_info(శూన్య)
{
	నిర్మాణం refs_for_each_ref_options opts = { 0 };
	స్థిర నిర్మాణం oidset చూసిన = OIDSET_INIT;
	నిర్మాణం strvec excludes_సదిశ = STRVEC_INIT;

	/*
	 * We need access to the reference names both with and without their
	 * namespace and thus cannot use `refs_for_each_namespaced_ref()`. We
	 * thus have to adapt exclude patterns to carry the namespace prefix
	 * ourselves.
	 */
	opts.exclude_patterns = get_namespaced_exclude_patterns(
		hidden_refs_గమ్యం_excludes(&hidden_refs),
		get_git_namespace(), &excludes_సదిశ);

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      show_ref_cb, &చూసిన, &opts);
	odb_for_each_alternate_ref(the_repository->objects,
				   show_one_alternate_ref, &చూసిన);

	oidset_clear(&చూసిన);
	strvec_clear(&excludes_సదిశ);

	ఒకవేళ (!పంపిన_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#నిర్వచించు నిరంతరం_PROC_RECEIVE_SCHEDULED	1
#నిర్వచించు నిరంతరం_PROC_RECEIVE_RETURNED	2
నిర్మాణం command {
	నిర్మాణం command *తదుపరి;
	స్థిరరాశి అక్షరం *error_string;
	అక్షరం *error_string_owned;
	నిర్మాణం ref_నెట్టు_report *report;
	గుర్తులేని పూర్ణాంకం skip_update:1,
		     did_not_exist:1,
		     నిరంతరం_proc_receive:2;
	పూర్ణాంకం సూచీ;
	నిర్మాణం object_id old_oid;
	నిర్మాణం object_id new_oid;
	అక్షరం ref_name[FLEX_ARRAY]; /*మరిన్ని*/
};

స్థిర శూన్య proc_receive_ref_append(స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	నిర్మాణం proc_receive_ref *ref_నమూనా;
	స్థిరరాశి అక్షరం *p;
	పూర్ణాంకం len;

	CALLOC_ARRAY(ref_నమూనా, 1);
	p = strchr(ఉపసర్గ, ':');
	ఒకవేళ (p) {
		అయితే (ఉపసర్గ < p) {
			ఒకవేళ (*ఉపసర్గ == 'a')
				ref_నమూనా->want_కూడు = 1;
			లేకపోతే ఒకవేళ (*ఉపసర్గ == 'd')
				ref_నమూనా->want_delete = 1;
			లేకపోతే ఒకవేళ (*ఉపసర్గ == 'm')
				ref_నమూనా->want_modify = 1;
			లేకపోతే ఒకవేళ (*ఉపసర్గ == '!')
				ref_నమూనా->negative_ref = 1;
			ఉపసర్గ++;
		}
		ఉపసర్గ++;
	} లేకపోతే {
		ref_నమూనా->want_కూడు = 1;
		ref_నమూనా->want_delete = 1;
		ref_నమూనా->want_modify = 1;
	}
	len = అక్షరశ్రేణి_పొడవు(ఉపసర్గ);
	అయితే (len && ఉపసర్గ[len - 1] == '/')
		len--;
	ref_నమూనా->ref_ఉపసర్గ = xmemdupz(ఉపసర్గ, len);
	ఒకవేళ (!proc_receive_ref) {
		proc_receive_ref = ref_నమూనా;
	} లేకపోతే {
		నిర్మాణం proc_receive_ref *చివర;

		చివర = proc_receive_ref;
		అయితే (చివర->తదుపరి)
			చివర = చివర->తదుపరి;
		చివర->తదుపరి = ref_నమూనా;
	}
}

స్థిర పూర్ణాంకం proc_receive_ref_matches(నిర్మాణం command *cmd)
{
	నిర్మాణం proc_receive_ref *p;

	ఒకవేళ (!proc_receive_ref)
		తిరిగి 0;

	కొరకు (p = proc_receive_ref; p; p = p->తదుపరి) {
		స్థిరరాశి అక్షరం *సరిపోలిక = p->ref_ఉపసర్గ;
		స్థిరరాశి అక్షరం *remains;

		ఒకవేళ (!p->want_కూడు && is_null_oid(&cmd->old_oid))
			కొనసాగు;
		లేకపోతే ఒకవేళ (!p->want_delete && is_null_oid(&cmd->new_oid))
			కొనసాగు;
		లేకపోతే ఒకవేళ (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			కొనసాగు;

		ఒకవేళ (skip_ఉపసర్గ(cmd->ref_name, సరిపోలిక, &remains) &&
		    (!*remains || *remains == '/')) {
			ఒకవేళ (!p->negative_ref)
				తిరిగి 1;
		} లేకపోతే ఒకవేళ (p->negative_ref) {
			తిరిగి 1;
		}
	}
	తిరిగి 0;
}

స్థిర శూన్య report_సందేశం(స్థిరరాశి అక్షరం *ఉపసర్గ, స్థిరరాశి అక్షరం *err, va_list params)
{
	పూర్ణాంకం sz;
	అక్షరం సందేశం[4096];

	sz = xsnprintf(సందేశం, పరిమాణం(సందేశం), "%s", ఉపసర్గ);
	sz += vsnprintf(సందేశం + sz, పరిమాణం(సందేశం) - sz, err, params);
	ఒకవేళ (sz > (పరిమాణం(సందేశం) - 1))
		sz = పరిమాణం(సందేశం) - 1;
	సందేశం[sz++] = '\n';

	ఒకవేళ (use_sideband)
		send_sideband(1, 2, సందేశం, sz, use_sideband);
	లేకపోతే
		xwrite(2, సందేశం, sz);
}

__attribute__((format (రాయి, 1, 2)))
స్థిర శూన్య rp_warning(స్థిరరాశి అక్షరం *err, ...)
{
	va_list params;
	va_ప్రారంభం(params, err);
	report_సందేశం("warning: ", err, params);
	va_చివర(params);
}

__attribute__((format (రాయి, 1, 2)))
స్థిర శూన్య rp_error(స్థిరరాశి అక్షరం *err, ...)
{
	va_list params;
	va_ప్రారంభం(params, err);
	report_సందేశం("error: ", err, params);
	va_చివర(params);
}

స్థిర పూర్ణాంకం copy_గమ్యం_sideband(పూర్ణాంకం లోపల, పూర్ణాంకం బహిర్గతం వాడని, శూన్య *arg వాడని)
{
	అక్షరం దత్తాంశం[128];
	పూర్ణాంకం keepalive_చురుకైన = 0;

	ఒకవేళ (keepalive_లోపల_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	ఒకవేళ (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_చురుకైన = 1;

	అయితే (1) {
		ssize_t sz;

		ఒకవేళ (keepalive_చురుకైన) {
			నిర్మాణం pollfd pfd;
			పూర్ణాంకం ret;

			pfd.fd = లోపల;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_లోపల_sec);

			ఒకవేళ (ret < 0) {
				ఒకవేళ (errno == EINTR)
					కొనసాగు;
				లేకపోతే
					ఆపు;
			} లేకపోతే ఒకవేళ (ret == 0) {
				/* no data; send a keepalive packet */
				స్థిర స్థిరరాశి అక్షరం buf[] = "0005\1";
				write_or_die(1, buf, పరిమాణం(buf) - 1);
				కొనసాగు;
			} /* else there is actual data to read */
		}

		sz = xread(లోపల, దత్తాంశం, పరిమాణం(దత్తాంశం));
		ఒకవేళ (sz <= 0)
			ఆపు;

		ఒకవేళ (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_చురుకైన) {
			స్థిరరాశి అక్షరం *p = memchr(దత్తాంశం, '\0', sz);
			ఒకవేళ (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_చురుకైన = 1;
				send_sideband(1, 2, దత్తాంశం, p - దత్తాంశం, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - దత్తాంశం + 1), use_sideband);
				కొనసాగు;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, దత్తాంశం, sz, use_sideband);
	}
	close(లోపల);
	తిరిగి 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
స్థిర శూన్య prepare_sideband_async(నిర్మాణం async *sideband_async, పూర్ణాంకం *saved_stderr, పూర్ణాంకం *started)
{
	*started = 0;

	ఒకవేళ (!use_sideband)
		తిరిగి;

	memset(sideband_async, 0, పరిమాణం(*sideband_async));
	sideband_async->proc = copy_గమ్యం_sideband;
	sideband_async->లోపల = -1;

	ఒకవేళ (!ప్రారంభం_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		ఒకవేళ (*saved_stderr >= 0)
			dup2(sideband_async->లోపల, STDERR_FILENO);
		close(sideband_async->లోపల);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
స్థిర శూన్య ముగింపు_sideband_async(నిర్మాణం async *sideband_async, పూర్ణాంకం saved_stderr, పూర్ణాంకం started)
{
	ఒకవేళ (!use_sideband)
		తిరిగి;

	ఒకవేళ (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	ఒకవేళ (started)
		ముగింపు_async(sideband_async);
}

స్థిర శూన్య hmac_hash(గుర్తులేని అక్షరం *బహిర్గతం,
		      స్థిరరాశి అక్షరం *కీలకం_లోపల, కొలత_t కీలకం_len,
		      స్థిరరాశి అక్షరం *పాఠ్యం, కొలత_t పాఠ్యం_len)
{
	గుర్తులేని అక్షరం కీలకం[GIT_MAX_BLKSZ];
	గుర్తులేని అక్షరం k_ipad[GIT_MAX_BLKSZ];
	గుర్తులేని అక్షరం k_opad[GIT_MAX_BLKSZ];
	పూర్ణాంకం i;
	నిర్మాణం git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(కీలకం, '\0', GIT_MAX_BLKSZ);
	ఒకవేళ (the_hash_algo->blksz < కీలకం_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, కీలకం_లోపల, కీలకం_len);
		git_hash_final(కీలకం, &ctx);
	} లేకపోతే {
		జ్ఞాపకం_నకలు(కీలకం, కీలకం_లోపల, కీలకం_len);
	}

	/* RFC 2104 2. (2) & (5) */
	కొరకు (i = 0; i < పరిమాణం(కీలకం); i++) {
		k_ipad[i] = కీలకం[i] ^ 0x36;
		k_opad[i] = కీలకం[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, పరిమాణం(k_ipad));
	git_hash_update(&ctx, పాఠ్యం, పాఠ్యం_len);
	git_hash_final(బహిర్గతం, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, పరిమాణం(k_opad));
	git_hash_update(&ctx, బహిర్గతం, the_hash_algo->rawsz);
	git_hash_final(బహిర్గతం, &ctx);
}

స్థిర అక్షరం *prepare_నెట్టు_cert_nonce(స్థిరరాశి అక్షరం *మార్గం, timestamp_t stamp)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	గుర్తులేని అక్షరం hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, మార్గం, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_విత్తనం, అక్షరశ్రేణి_పొడవు(cert_nonce_విత్తనం));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (పూర్ణాంకం)the_hash_algo->hexsz, hash_గమ్యం_hex(hash));
	తిరిగి strbuf_detach(&buf, ఖాళీ);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
స్థిర పూర్ణాంకం constant_memequal(స్థిరరాశి అక్షరం *a, స్థిరరాశి అక్షరం *b, కొలత_t n)
{
	పూర్ణాంకం res = 0;
	కొలత_t i;

	కొరకు (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	తిరిగి res;
}

స్థిర స్థిరరాశి అక్షరం *check_nonce(స్థిరరాశి అక్షరం *buf)
{
	కొలత_t noncelen;
	స్థిరరాశి అక్షరం *దొరికింది_2 = కనుగొను_commit_శీర్షిక(buf, "nonce", &noncelen);
	అక్షరం *nonce = దొరికింది_2 ? xmemdupz(దొరికింది_2, noncelen) : ఖాళీ;
	timestamp_t stamp, ostamp;
	అక్షరం *bohmac, *expect = ఖాళీ;
	స్థిరరాశి అక్షరం *retval = NONCE_BAD;

	ఒకవేళ (!nonce) {
		retval = NONCE_MISSING;
		వెళ్ళు leave;
	} లేకపోతే ఒకవేళ (!నెట్టు_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		వెళ్ళు leave;
	} లేకపోతే ఒకవేళ (!strcmp(నెట్టు_cert_nonce, nonce)) {
		retval = NONCE_OK;
		వెళ్ళు leave;
	}

	ఒకవేళ (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		వెళ్ళు leave;
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
	ఒకవేళ (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		వెళ్ళు leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	ఒకవేళ (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		వెళ్ళు leave;
	}

	expect = prepare_నెట్టు_cert_nonce(service_dir, stamp);
	ఒకవేళ (noncelen != అక్షరశ్రేణి_పొడవు(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		వెళ్ళు leave;
	}
	ఒకవేళ (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		వెళ్ళు leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(నెట్టు_cert_nonce, ఖాళీ, 10);
	nonce_stamp_slop = (దీర్ఘ)ostamp - (దీర్ఘ)stamp;

	ఒకవేళ (nonce_stamp_slop_హద్దు &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_హద్దు) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		విడుదల((శూన్య *)నెట్టు_cert_nonce);
		నెట్టు_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} లేకపోతే {
		retval = NONCE_SLOP;
	}

leave:
	విడుదల(nonce);
	విడుదల(expect);
	తిరిగి retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
స్థిర పూర్ణాంకం check_cert_నెట్టు_options(స్థిరరాశి నిర్మాణం string_list *నెట్టు_options)
{
	స్థిరరాశి అక్షరం *buf = నెట్టు_cert.buf;

	స్థిరరాశి అక్షరం *option;
	కొలత_t optionlen;
	పూర్ణాంకం options_చూసిన = 0;

	పూర్ణాంకం retval = 1;

	ఒకవేళ (!*buf)
		తిరిగి 1;

	అయితే ((option = కనుగొను_commit_శీర్షిక(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_చూసిన++;
		ఒకవేళ (options_చూసిన > నెట్టు_options->nr
		    || xstrncmpz(నెట్టు_options->వస్తువులు[options_చూసిన - 1].string,
				 option, optionlen))
			తిరిగి 0;
	}

	ఒకవేళ (options_చూసిన != నెట్టు_options->nr)
		retval = 0;

	తిరిగి retval;
}

స్థిర శూన్య prepare_నెట్టు_cert_sha1(నిర్మాణం నిరంతరం_hooks_opt *opt)
{
	స్థిర పూర్ణాంకం already_పూర్తి;

	ఒకవేళ (!నెట్టు_cert.len)
		తిరిగి;

	ఒకవేళ (!already_పూర్తి) {
		పూర్ణాంకం bogs /* beginning_of_gpg_sig */;

		already_పూర్తి = 1;
		ఒకవేళ (odb_write_object(the_repository->objects, నెట్టు_cert.buf,
				     నెట్టు_cert.len, OBJ_BLOB, &నెట్టు_cert_oid))
			oidclr(&నెట్టు_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', పరిమాణం(sigcheck));

		bogs = parse_signed_బఫరు(నెట్టు_cert.buf, నెట్టు_cert.len);
		sigcheck.payload = xmemdupz(నెట్టు_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, నెట్టు_cert.buf + bogs,
				నెట్టు_cert.len - bogs);

		nonce_స్థితి = check_nonce(sigcheck.payload);
	}
	ఒకవేళ (!is_null_oid(&నెట్టు_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_గమ్యం_hex(&నెట్టు_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.కీలకం ? sigcheck.కీలకం : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.ఫలితం);
		ఒకవేళ (నెట్టు_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     నెట్టు_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_స్థితి);
			ఒకవేళ (nonce_స్థితి == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

నిర్మాణం receive_hook_feed_state {
	నిర్మాణం command *cmd;
	నిర్మాణం ref_నెట్టు_report *report;
	పూర్ణాంకం skip_broken;
	నిర్మాణం strbuf buf;
};

స్థిర పూర్ణాంకం feed_receive_hook_cb(పూర్ణాంకం hook_stdin_fd, శూన్య *pp_cb వాడని, శూన్య *pp_task_cb)
{
	నిర్మాణం receive_hook_feed_state *state = pp_task_cb;
	నిర్మాణం command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	అయితే (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->తదుపరి;

	ఒకవేళ (!cmd)
		తిరిగి 1;  /* no more commands left */

	ఒకవేళ (!state->report)
		state->report = cmd->report;

	ఒకవేళ (state->report) {
		నిర్మాణం object_id *old_oid;
		నిర్మాణం object_id *new_oid;
		స్థిరరాశి అక్షరం *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_గమ్యం_hex(old_oid), oid_గమ్యం_hex(new_oid),
			    ref_name);

		state->report = state->report->తదుపరి;
		ఒకవేళ (!state->report)
			cmd = cmd->తదుపరి;
	} లేకపోతే {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_గమ్యం_hex(&cmd->old_oid), oid_గమ్యం_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->తదుపరి;
	}

	state->cmd = cmd;

	ఒకవేళ (state->buf.len > 0) {
		పూర్ణాంకం ret = write_లోపల_నిండిన(hook_stdin_fd, state->buf.buf, state->buf.len);
		ఒకవేళ (ret < 0) {
			ఒకవేళ (errno == EPIPE)
				తిరిగి 1; /* child closed pipe */
			తిరిగి ret;
		}
	}

	తిరిగి state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

స్థిర శూన్య *receive_hook_feed_state_alloc(శూన్య *feed_pipe_ctx)
{
	నిర్మాణం receive_hook_feed_state *init_state = feed_pipe_ctx;
	నిర్మాణం receive_hook_feed_state *దత్తాంశం;

	CALLOC_ARRAY(దత్తాంశం, 1);
	దత్తాంశం->report = init_state->report;
	దత్తాంశం->cmd = init_state->cmd;
	దత్తాంశం->skip_broken = init_state->skip_broken;
	strbuf_init(&దత్తాంశం->buf, 0);

	తిరిగి దత్తాంశం;
}

స్థిర శూన్య receive_hook_feed_state_free(శూన్య *దత్తాంశం)
{
	నిర్మాణం receive_hook_feed_state *d = దత్తాంశం;
	ఒకవేళ (!d)
		తిరిగి;
	strbuf_release(&d->buf);
	విడుదల(d);
}

స్థిర పూర్ణాంకం నిరంతరం_receive_hook(నిర్మాణం command *commands,
			    స్థిరరాశి అక్షరం *hook_name,
			    పూర్ణాంకం skip_broken,
			    నిర్మాణం odb_transaction *transaction,
			    స్థిరరాశి నిర్మాణం string_list *నెట్టు_options)
{
	నిర్మాణం నిరంతరం_hooks_opt opt = నిరంతరం_HOOKS_OPT_INIT;
	నిర్మాణం command *iter = commands;
	నిర్మాణం receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	నిర్మాణం async sideband_async;
	పూర్ణాంకం sideband_async_started = 0;
	పూర్ణాంకం saved_stderr = -1;
	పూర్ణాంకం ret;

	ఒకవేళ (!hook_exists(the_repository, hook_name))
		తిరిగి 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	అయితే (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->తదుపరి;
	ఒకవేళ (!iter)
		తిరిగి 0;

	ఒకవేళ (నెట్టు_options) {
		కొరకు (పూర్ణాంకం i = 0; i < నెట్టు_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     నెట్టు_options->వస్తువులు[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)నెట్టు_options->nr);
	} లేకపోతే {
		strvec_నెట్టు(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	ఒకవేళ (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_నెట్టు_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_దత్తాంశం_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_దత్తాంశం_free = receive_hook_feed_state_free;

	ret = నిరంతరం_hooks_opt(the_repository, hook_name, &opt);

	ముగింపు_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	తిరిగి ret;
}

స్థిర పూర్ణాంకం నిరంతరం_update_hook(నిర్మాణం command *cmd)
{
	స్థిర స్థిరరాశి అక్షరం hook_name[] = "update";
	నిర్మాణం నిరంతరం_hooks_opt opt = నిరంతరం_HOOKS_OPT_INIT;
	నిర్మాణం async sideband_async;
	పూర్ణాంకం sideband_async_started = 0;
	పూర్ణాంకం saved_stderr = -1;
	పూర్ణాంకం సంకేతం_2;

	ఒకవేళ (!hook_exists(the_repository, hook_name))
		తిరిగి 0;

	strvec_pushl(&opt.వాదనలు,
		     cmd->ref_name,
		     oid_గమ్యం_hex(&cmd->old_oid),
		     oid_గమ్యం_hex(&cmd->new_oid),
		     ఖాళీ);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	సంకేతం_2 = నిరంతరం_hooks_opt(the_repository, hook_name, &opt);

	ముగింపు_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	తిరిగి సంకేతం_2;
}

స్థిర పూర్ణాంకం నిరంతరం_receive_report_hook(నిర్మాణం strbuf *report)
{
	నిర్మాణం బిడ్డ_process proc = బిడ్డ_PROCESS_INIT;
	నిర్మాణం async sideband_async;
	పూర్ణాంకం sideband_async_started = 0;
	పూర్ణాంకం saved_stderr = -1;
	నిర్మాణం strbuf బహిర్గతం = STRBUF_INIT;
	స్థిరరాశి అక్షరం *hook_మార్గం;
	పూర్ణాంకం ret;

	hook_మార్గం = కనుగొను_hook(the_repository, "receive-report");
	ఒకవేళ (!hook_మార్గం)
		తిరిగి 0;

	strvec_నెట్టు(&proc.వాదనలు, hook_మార్గం);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_నెట్టు(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &బహిర్గతం,
			   report->len, ఖాళీ, 0);
	sigchain_తీయి(SIGPIPE);

	ముగింపు_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	ఒకవేళ (!ret)
		strbuf_మార్పిడి(&బహిర్గతం, report);

	strbuf_release(&బహిర్గతం);
	తిరిగి ret;
}

స్థిర నిర్మాణం command *కనుగొను_command_by_refname(నిర్మాణం command *list,
					       స్థిరరాశి అక్షరం *refname)
{
	కొరకు (; list; list = list->తదుపరి)
		ఒకవేళ (!strcmp(list->ref_name, refname))
			తిరిగి list;
	తిరిగి ఖాళీ;
}

స్థిర పూర్ణాంకం read_proc_receive_report(నిర్మాణం packet_reader *reader,
				    నిర్మాణం command *commands,
				    నిర్మాణం strbuf *errmsg)
{
	నిర్మాణం command *cmd;
	నిర్మాణం command *hint = ఖాళీ;
	నిర్మాణం ref_నెట్టు_report *report = ఖాళీ;
	పూర్ణాంకం new_report = 0;
	పూర్ణాంకం సంకేతం_2 = 0;
	పూర్ణాంకం once = 0;
	పూర్ణాంకం response = 0;

	కొరకు (;;) {
		నిర్మాణం object_id old_oid, new_oid;
		అక్షరం *తల;
		అక్షరం *refname;
		అక్షరం *p;
		గణన packet_read_స్థితి స్థితి_2;

		స్థితి_2 = packet_reader_read(reader);
		ఒకవేళ (స్థితి_2 != PACKET_READ_సాధారణ) {
			/* Check whether proc-receive exited abnormally */
			ఒకవేళ (స్థితి_2 == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				తిరిగి -1;
			}
			ఆపు;
		}
		response++;

		తల = reader->పంక్తి;
		p = strchr(తల, ' ');
		ఒకవేళ (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", తల);
			సంకేతం_2 = -1;
			కొనసాగు;
		}
		*p++ = '\0';
		ఒకవేళ (!strcmp(తల, "option")) {
			అక్షరం *కీలకం;
			స్థిరరాశి అక్షరం *val;

			ఒకవేళ (!hint || !(report || new_report)) {
				ఒకవేళ (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				సంకేతం_2 = -1;
				కొనసాగు;
			}
			ఒకవేళ (new_report) {
				ఒకవేళ (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} లేకపోతే {
					report = hint->report;
					అయితే (report->తదుపరి)
						report = report->తదుపరి;
					report->తదుపరి = xcalloc(1, పరిమాణం(నిర్మాణం ref_నెట్టు_report));
					report = report->తదుపరి;
				}
				new_report = 0;
			}
			కీలకం = p;
			p = strchr(కీలకం, ' ');
			ఒకవేళ (p)
				*p++ = '\0';
			val = p;
			ఒకవేళ (!strcmp(కీలకం, "refname"))
				report->ref_name = xstrdup_or_null(val);
			లేకపోతే ఒకవేళ (!strcmp(కీలకం, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			లేకపోతే ఒకవేళ (!strcmp(కీలకం, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			లేకపోతే ఒకవేళ (!strcmp(కీలకం, "forced-update"))
				report->forced_update = 1;
			లేకపోతే ఒకవేళ (!strcmp(కీలకం, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->నిరంతరం_proc_receive = 0;
			కొనసాగు;
		}

		report = ఖాళీ;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		ఒకవేళ (p)
			*p++ = '\0';
		ఒకవేళ (strcmp(తల, "ok") && strcmp(తల, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    తల, refname);
			సంకేతం_2 = -1;
			కొనసాగు;
		}

		/* first try searching at our hint, falling back to all refs */
		ఒకవేళ (hint)
			hint = కనుగొను_command_by_refname(hint, refname);
		ఒకవేళ (!hint)
			hint = కనుగొను_command_by_refname(commands, refname);
		ఒకవేళ (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			సంకేతం_2 = -1;
			కొనసాగు;
		}
		ఒకవేళ (!hint->నిరంతరం_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			సంకేతం_2 = -1;
			కొనసాగు;
		}
		hint->నిరంతరం_proc_receive |= నిరంతరం_PROC_RECEIVE_RETURNED;
		ఒకవేళ (!strcmp(తల, "ng")) {
			ఒకవేళ (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			లేకపోతే
				hint->error_string = "failed";
			సంకేతం_2 = -1;
			కొనసాగు;
		}
		new_report = 1;
	}

	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి)
		ఒకవేళ (cmd->నిరంతరం_proc_receive && !cmd->error_string &&
		    !(cmd->నిరంతరం_proc_receive & నిరంతరం_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    సంకేతం_2 = -1;
		}
	తిరిగి సంకేతం_2;
}

స్థిర పూర్ణాంకం నిరంతరం_proc_receive_hook(నిర్మాణం command *commands,
				 స్థిరరాశి నిర్మాణం string_list *నెట్టు_options)
{
	నిర్మాణం బిడ్డ_process proc = బిడ్డ_PROCESS_INIT;
	నిర్మాణం async muxer;
	నిర్మాణం command *cmd;
	నిర్మాణం packet_reader reader;
	నిర్మాణం strbuf cap = STRBUF_INIT;
	నిర్మాణం strbuf errmsg = STRBUF_INIT;
	పూర్ణాంకం hook_use_నెట్టు_options = 0;
	పూర్ణాంకం సంస్కరణ = 0;
	పూర్ణాంకం సంకేతం_2;
	స్థిరరాశి అక్షరం *hook_మార్గం = కనుగొను_hook(the_repository, "proc-receive");

	ఒకవేళ (!hook_మార్గం) {
		rp_error("cannot find hook 'proc-receive'");
		తిరిగి -1;
	}

	strvec_నెట్టు(&proc.వాదనలు, hook_మార్గం);
	proc.లోపల = -1;
	proc.బహిర్గతం = -1;
	proc.trace2_hook_name = "proc-receive";

	ఒకవేళ (use_sideband) {
		memset(&muxer, 0, పరిమాణం(muxer));
		muxer.proc = copy_గమ్యం_sideband;
		muxer.లోపల = -1;
		సంకేతం_2 = ప్రారంభం_async(&muxer);
		ఒకవేళ (సంకేతం_2)
			తిరిగి సంకేతం_2;
		proc.err = muxer.లోపల;
	} లేకపోతే {
		proc.err = 0;
	}

	సంకేతం_2 = ప్రారంభం_command(&proc);
	ఒకవేళ (సంకేతం_2) {
		ఒకవేళ (use_sideband)
			ముగింపు_async(&muxer);
		తిరిగి సంకేతం_2;
	}

	sigchain_నెట్టు(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.బహిర్గతం, ఖాళీ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	ఒకవేళ (use_atomic)
		strbuf_addstr(&cap, " atomic");
	ఒకవేళ (use_నెట్టు_options)
		strbuf_addstr(&cap, " push-options");
	ఒకవేళ (cap.len) {
		సంకేతం_2 = packet_write_fmt_gently(proc.లోపల, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} లేకపోతే {
		సంకేతం_2 = packet_write_fmt_gently(proc.లోపల, "version=1\n");
	}
	ఒకవేళ (!సంకేతం_2)
		సంకేతం_2 = packet_flush_gently(proc.లోపల);

	ఒకవేళ (!సంకేతం_2)
		కొరకు (;;) {
			పూర్ణాంకం linelen;
			గణన packet_read_స్థితి స్థితి_2;

			స్థితి_2 = packet_reader_read(&reader);
			ఒకవేళ (స్థితి_2 != PACKET_READ_సాధారణ) {
				/* Check whether proc-receive exited abnormally */
				ఒకవేళ (స్థితి_2 == PACKET_READ_EOF)
					సంకేతం_2 = -1;
				ఆపు;
			}

			ఒకవేళ (reader.pktlen > 8 && starts_with(reader.పంక్తి, "version=")) {
				సంస్కరణ = atoi(reader.పంక్తి + 8);
				linelen = అక్షరశ్రేణి_పొడవు(reader.పంక్తి);
				ఒకవేళ (linelen < reader.pktlen) {
					స్థిరరాశి అక్షరం *feature_list = reader.పంక్తి + linelen + 1;
					ఒకవేళ (parse_feature_request(feature_list, "push-options"))
						hook_use_నెట్టు_options = 1;
				}
			}
		}

	ఒకవేళ (సంకేతం_2) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		వెళ్ళు cleanup;
	}

	ఎంపిక (సంస్కరణ) {
	స్థితి 0:
		/* fallthrough */
	స్థితి 1:
		ఆపు;
	అప్రమేయ:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    సంస్కరణ);
		సంకేతం_2 = -1;
		వెళ్ళు cleanup;
	}

	/* Send commands */
	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
		ఒకవేళ (!cmd->నిరంతరం_proc_receive || cmd->skip_update || cmd->error_string)
			కొనసాగు;
		సంకేతం_2 = packet_write_fmt_gently(proc.లోపల, "%s %s %s",
					       oid_గమ్యం_hex(&cmd->old_oid),
					       oid_గమ్యం_hex(&cmd->new_oid),
					       cmd->ref_name);
		ఒకవేళ (సంకేతం_2)
			ఆపు;
	}
	ఒకవేళ (!సంకేతం_2)
		సంకేతం_2 = packet_flush_gently(proc.లోపల);
	ఒకవేళ (సంకేతం_2) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		వెళ్ళు cleanup;
	}

	/* Send push options */
	ఒకవేళ (hook_use_నెట్టు_options) {
		నిర్మాణం string_list_అంశం *అంశం;

		for_each_string_list_అంశం(అంశం, నెట్టు_options) {
			సంకేతం_2 = packet_write_fmt_gently(proc.లోపల, "%s", అంశం->string);
			ఒకవేళ (సంకేతం_2)
				ఆపు;
		}
		ఒకవేళ (!సంకేతం_2)
			సంకేతం_2 = packet_flush_gently(proc.లోపల);
		ఒకవేళ (సంకేతం_2) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			వెళ్ళు cleanup;
		}
	}

	/* Read result from proc-receive */
	సంకేతం_2 = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.లోపల);
	close(proc.బహిర్గతం);
	ఒకవేళ (use_sideband)
		ముగింపు_async(&muxer);
	ఒకవేళ (ముగింపు_command(&proc))
		సంకేతం_2 = -1;
	ఒకవేళ (errmsg.len >0) {
		అక్షరం *p = errmsg.buf;

		p += errmsg.len - 1;
		ఒకవేళ (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_తీయి(SIGPIPE);

	తిరిగి సంకేతం_2;
}

స్థిర స్థిరరాశి అక్షరం *refuse_unconfigured_deny_సందేశం =
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

స్థిర శూన్య refuse_unconfigured_deny(శూన్య)
{
	rp_error("%s", _(refuse_unconfigured_deny_సందేశం));
}

స్థిర స్థిరరాశి అక్షరం *refuse_unconfigured_deny_delete_current_సందేశం =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

స్థిర శూన్య refuse_unconfigured_deny_delete_current(శూన్య)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_సందేశం));
}

స్థిర స్థిరరాశి నిర్మాణం object_id *command_singleton_iterator(శూన్య *cb_దత్తాంశం);
స్థిర పూర్ణాంకం update_shallow_ref(నిర్మాణం command *cmd, నిర్మాణం shallow_info *si)
{
	నిర్మాణం shallow_తాళం shallow_తాళం = SHALLOW_తాళం_INIT;
	నిర్మాణం oid_array extra = OID_ARRAY_INIT;
	నిర్మాణం check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t ముసుగు = 1 << (cmd->సూచీ % 32);
	పూర్ణాంకం i;

	trace_printf_కీలకం(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	కొరకు (i = 0; i < si->shallow->nr; i++)
		ఒకవేళ (si->వాడిన_shallow[i] &&
		    (si->వాడిన_shallow[i][cmd->సూచీ / 32] & ముసుగు) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	అమరిక_alternate_shallow(&shallow_తాళం, &opt.shallow_file, &extra);
	ఒకవేళ (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_తాళం);
		oid_array_clear(&extra);
		తిరిగి -1;
	}

	commit_shallow_file(the_repository, &shallow_తాళం);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	కొరకు (i = 0; i < extra.nr; i++)
		రిజిస్టరు_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->సూచీ] = 0;
	oid_array_clear(&extra);
	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *నెట్టు_గమ్యం_deploy(గుర్తులేని అక్షరం *sha1,
				  నిర్మాణం strvec *env,
				  స్థిరరాశి నిర్మాణం worktree *worktree)
{
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;

	strvec_pushl(&బిడ్డ.వాదనలు, "update-index", "-q", "--ignore-submodules",
		     "--refresh", ఖాళీ);
	strvec_pushv(&బిడ్డ.env, env->v);
	బిడ్డ.dir = worktree->మార్గం;
	బిడ్డ.no_stdin = 1;
	బిడ్డ.stdout_గమ్యం_stderr = 1;
	బిడ్డ.git_cmd = 1;
	ఒకవేళ (నిరంతరం_command(&బిడ్డ))
		తిరిగి "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	బిడ్డ_process_init(&బిడ్డ);
	strvec_pushl(&బిడ్డ.వాదనలు, "diff-files", "--quiet",
		     "--ignore-submodules", "--", ఖాళీ);
	strvec_pushv(&బిడ్డ.env, env->v);
	బిడ్డ.dir = worktree->మార్గం;
	బిడ్డ.no_stdin = 1;
	బిడ్డ.stdout_గమ్యం_stderr = 1;
	బిడ్డ.git_cmd = 1;
	ఒకవేళ (నిరంతరం_command(&బిడ్డ))
		తిరిగి "Working directory has unstaged changes";

	బిడ్డ_process_init(&బిడ్డ);
	strvec_pushl(&బిడ్డ.వాదనలు, "diff-index", "--quiet", "--cached",
		     "--ignore-submodules",
		     /*
		      * diff-index with either HEAD or an empty tree
		      *
		      * NEEDSWORK: is_null_oid() cannot know whether it's an
		      * unborn HEAD or a corrupt ref. It works for now because
		      * it's only needed to know if we are comparing HEAD or an
		      * empty tree.
		      */
		     !is_null_oid(&worktree->తల_oid) ? "HEAD" :
		     ఖాళీలు_వృక్షం_oid_hex(the_repository->hash_algo), "--", ఖాళీ);
	strvec_pushv(&బిడ్డ.env, env->v);
	బిడ్డ.no_stdin = 1;
	బిడ్డ.no_stdout = 1;
	బిడ్డ.stdout_గమ్యం_stderr = 0;
	బిడ్డ.git_cmd = 1;
	ఒకవేళ (నిరంతరం_command(&బిడ్డ))
		తిరిగి "Working directory has staged changes";

	బిడ్డ_process_init(&బిడ్డ);
	strvec_pushl(&బిడ్డ.వాదనలు, "read-tree", "-u", "-m", hash_గమ్యం_hex(sha1),
		     ఖాళీ);
	strvec_pushv(&బిడ్డ.env, env->v);
	బిడ్డ.dir = worktree->మార్గం;
	బిడ్డ.no_stdin = 1;
	బిడ్డ.no_stdout = 1;
	బిడ్డ.stdout_గమ్యం_stderr = 0;
	బిడ్డ.git_cmd = 1;
	ఒకవేళ (నిరంతరం_command(&బిడ్డ))
		తిరిగి "Could not update working tree to new HEAD";

	తిరిగి ఖాళీ;
}

స్థిర స్థిరరాశి అక్షరం *నెట్టు_గమ్యం_checkout_hook = "push-to-checkout";

స్థిర స్థిరరాశి అక్షరం *నెట్టు_గమ్యం_checkout(గుర్తులేని అక్షరం *hash,
				    పూర్ణాంకం *invoked_hook,
				    నిర్మాణం strvec *env,
				    స్థిరరాశి అక్షరం *work_వృక్షం)
{
	నిర్మాణం నిరంతరం_hooks_opt opt = నిరంతరం_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_మార్గం(work_వృక్షం));
	strvec_నెట్టు(&opt.వాదనలు, hash_గమ్యం_hex(hash));
	ఒకవేళ (నిరంతరం_hooks_opt(the_repository, నెట్టు_గమ్యం_checkout_hook, &opt))
		తిరిగి "push-to-checkout hook declined";
	లేకపోతే
		తిరిగి ఖాళీ;
}

స్థిర స్థిరరాశి అక్షరం *update_worktree(గుర్తులేని అక్షరం *sha1, స్థిరరాశి నిర్మాణం worktree *worktree)
{
	స్థిరరాశి అక్షరం *retval;
	అక్షరం *git_dir;
	నిర్మాణం strvec env = STRVEC_INIT;
	పూర్ణాంకం invoked_hook;

	ఒకవేళ (!worktree || !worktree->మార్గం)
		BUG("worktree->path must be non-NULL");

	ఒకవేళ (worktree->is_bare)
		తిరిగి "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_మార్గం(git_dir));

	retval = నెట్టు_గమ్యం_checkout(sha1, &invoked_hook, &env, worktree->మార్గం);
	ఒకవేళ (!invoked_hook)
		retval = నెట్టు_గమ్యం_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	విడుదల(git_dir);
	తిరిగి retval;
}

స్థిర స్థిరరాశి అక్షరం *update(నిర్మాణం command *cmd, నిర్మాణం shallow_info *si)
{
	స్థిరరాశి అక్షరం *name = cmd->ref_name;
	నిర్మాణం strbuf namespaced_name_buf = STRBUF_INIT;
	స్థిర అక్షరం *namespaced_name;
	స్థిరరాశి అక్షరం *ret;
	నిర్మాణం object_id *old_oid = &cmd->old_oid;
	నిర్మాణం object_id *new_oid = &cmd->new_oid;
	పూర్ణాంకం do_update_worktree = 0;
	నిర్మాణం worktree **worktrees = get_worktrees(the_repository);
	స్థిరరాశి నిర్మాణం worktree *worktree =
		కనుగొను_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	ఒకవేళ (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		వెళ్ళు బహిర్గతం;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	విడుదల(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, ఖాళీ);

	ఒకవేళ (worktree && !worktree->is_bare) {
		ఎంపిక (deny_current_branch) {
		స్థితి DENY_IGNORE:
			ఆపు;
		స్థితి DENY_WARN:
			rp_warning("updating the current branch");
			ఆపు;
		స్థితి DENY_REFUSE:
		స్థితి DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			ఒకవేళ (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			వెళ్ళు బహిర్గతం;
		స్థితి DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			ఆపు;
		}
	}

	ఒకవేళ (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_గమ్యం_hex(new_oid));
		ret = "bad pack";
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		ఒకవేళ (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			వెళ్ళు బహిర్గతం;
		}

		ఒకవేళ (worktree || (తల_name && !strcmp(namespaced_name, తల_name))) {
			ఎంపిక (deny_delete_current) {
			స్థితి DENY_IGNORE:
				ఆపు;
			స్థితి DENY_WARN:
				rp_warning("deleting the current branch");
				ఆపు;
			స్థితి DENY_REFUSE:
			స్థితి DENY_UNCONFIGURED:
			స్థితి DENY_UPDATE_INSTEAD:
				ఒకవేళ (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				వెళ్ళు బహిర్గతం;
			అప్రమేయ:
				ret = "Invalid denyDeleteCurrent setting";
				వెళ్ళు బహిర్గతం;
			}
		}
	}

	ఒకవేళ (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		నిర్మాణం object *old_object, *new_object;
		నిర్మాణం commit *old_commit, *new_commit;
		పూర్ణాంకం ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		ఒకవేళ (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			వెళ్ళు బహిర్గతం;
		}
		old_commit = (నిర్మాణం commit *)old_object;
		new_commit = (నిర్మాణం commit *)new_object;
		ret2 = repo_లోపల_విలీనం_bases(the_repository, old_commit, new_commit);
		ఒకవేళ (ret2 < 0)
			నిష్క్రమించు(128);
		ఒకవేళ (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			వెళ్ళు బహిర్గతం;
		}
	}
	ఒకవేళ (నిరంతరం_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		ఒకవేళ (ret)
			వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (is_null_oid(new_oid)) {
		నిర్మాణం strbuf err = STRBUF_INIT;
		ఒకవేళ (!parse_object(the_repository, old_oid)) {
			old_oid = ఖాళీ;
			ఒకవేళ (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} లేకపోతే {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		ఒకవేళ (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   ఖాళీ, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} లేకపోతే {
			ret = ఖాళీ; /* good */
		}
		strbuf_release(&err);
	} లేకపోతే {
		గణన ref_transaction_error tx_err;
		నిర్మాణం strbuf err = STRBUF_INIT;
		ఒకవేళ (shallow_update && si->shallow_ref[cmd->సూచీ] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			వెళ్ళు బహిర్గతం;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  ఖాళీ, ఖాళీ,
						  0, "push",
						  &err);
		ఒకవేళ (tx_err) {
			rp_error("%s", err.buf);
			ఒకవేళ (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			లేకపోతే
				ret = ref_transaction_error_సందేశం(tx_err);
		} లేకపోతే {
			ret = ఖాళీ; /* good */
		}
		strbuf_release(&err);
	}

బహిర్గతం:
	free_worktrees(worktrees);
	తిరిగి ret;
}

స్థిర శూన్య నిరంతరం_update_post_hook(నిర్మాణం command *commands)
{
	స్థిర స్థిరరాశి అక్షరం hook_name[] = "post-update";
	నిర్మాణం నిరంతరం_hooks_opt opt = నిరంతరం_HOOKS_OPT_INIT;
	నిర్మాణం async sideband_async;
	నిర్మాణం command *cmd;
	పూర్ణాంకం sideband_async_started = 0;
	పూర్ణాంకం saved_stderr = -1;

	ఒకవేళ (!hook_exists(the_repository, hook_name))
		తిరిగి;

	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
		ఒకవేళ (cmd->error_string || cmd->did_not_exist)
			కొనసాగు;
		strvec_నెట్టు(&opt.వాదనలు, cmd->ref_name);
	}
	ఒకవేళ (!opt.వాదనలు.nr)
		తిరిగి;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	నిరంతరం_hooks_opt(the_repository, hook_name, &opt);

	ముగింపు_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

స్థిర శూన్య check_aliased_update_అంతర్గత(నిర్మాణం command *cmd,
					  నిర్మాణం string_list *list,
					  స్థిరరాశి అక్షరం *dst_name, పూర్ణాంకం flag)
{
	నిర్మాణం string_list_అంశం *అంశం;
	నిర్మాణం command *dst_cmd;

	ఒకవేళ (!(flag & REF_ISSYMREF))
		తిరిగి;

	ఒకవేళ (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		తిరిగి;
	}
	dst_name = strip_namespace(dst_name);

	ఒకవేళ (!(అంశం = string_list_lookup(list, dst_name)))
		తిరిగి;

	cmd->skip_update = 1;

	dst_cmd = (నిర్మాణం command *) అంశం->util;

	ఒకవేళ (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		తిరిగి;

	dst_cmd->skip_update = 1;

	rp_error("refusing inconsistent update between symref '%s' (%s..%s) and"
		 " its target '%s' (%s..%s)",
		 cmd->ref_name,
		 repo_కనుగొను_unique_abbrev(the_repository, &cmd->old_oid, DEFAULT_ABBREV),
		 repo_కనుగొను_unique_abbrev(the_repository, &cmd->new_oid, DEFAULT_ABBREV),
		 dst_cmd->ref_name,
		 repo_కనుగొను_unique_abbrev(the_repository, &dst_cmd->old_oid, DEFAULT_ABBREV),
		 repo_కనుగొను_unique_abbrev(the_repository, &dst_cmd->new_oid, DEFAULT_ABBREV));

	cmd->error_string = dst_cmd->error_string =
		"inconsistent aliased update";
}

స్థిర శూన్య check_aliased_update(నిర్మాణం command *cmd, నిర్మాణం string_list *list)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	స్థిరరాశి అక్షరం *dst_name;
	పూర్ణాంకం flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, ఖాళీ, &flag);
	check_aliased_update_అంతర్గత(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

స్థిర శూన్య check_aliased_updates(నిర్మాణం command *commands)
{
	నిర్మాణం command *cmd;
	నిర్మాణం string_list ref_list = STRING_LIST_INIT_NODUP;

	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
		నిర్మాణం string_list_అంశం *అంశం =
			string_list_append(&ref_list, cmd->ref_name);
		అంశం->util = (శూన్య *)cmd;
	}
	string_list_క్రమీకరించు(&ref_list);

	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
		ఒకవేళ (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

స్థిర స్థిరరాశి నిర్మాణం object_id *command_singleton_iterator(శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం command **cmd_list = cb_దత్తాంశం;
	నిర్మాణం command *cmd = *cmd_list;

	ఒకవేళ (!cmd || is_null_oid(&cmd->new_oid))
		తిరిగి ఖాళీ;
	*cmd_list = ఖాళీ; /* this returns only one */
	తిరిగి &cmd->new_oid;
}

స్థిర శూన్య సమితి_connectivity_errors(నిర్మాణం command *commands,
				    నిర్మాణం shallow_info *si,
				    నిర్మాణం odb_transaction *transaction)
{
	నిర్మాణం command *cmd;

	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
		నిర్మాణం command *singleton = cmd;
		నిర్మాణం check_connected_options opt = CHECK_CONNECTED_INIT;
		నిర్మాణం strvec env = STRVEC_INIT;

		ఒకవేళ (shallow_update && si->shallow_ref[cmd->సూచీ])
			/* to be checked in update_shallow_ref() */
			కొనసాగు;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		ఒకవేళ (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			కొనసాగు;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

నిర్మాణం iterate_దత్తాంశం {
	నిర్మాణం command *cmds;
	నిర్మాణం shallow_info *si;
};

స్థిర స్థిరరాశి నిర్మాణం object_id *iterate_receive_command_list(శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం iterate_దత్తాంశం *దత్తాంశం = cb_దత్తాంశం;
	నిర్మాణం command **cmd_list = &దత్తాంశం->cmds;
	నిర్మాణం command *cmd = *cmd_list;

	కొరకు (; cmd; cmd = cmd->తదుపరి) {
		ఒకవేళ (shallow_update && దత్తాంశం->si->shallow_ref[cmd->సూచీ])
			/* to be checked in update_shallow_ref() */
			కొనసాగు;
		ఒకవేళ (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->తదుపరి;
			తిరిగి &cmd->new_oid;
		}
	}
	తిరిగి ఖాళీ;
}

స్థిర శూన్య reject_updates_గమ్యం_hidden(నిర్మాణం command *commands)
{
	నిర్మాణం strbuf refname_నిండిన = STRBUF_INIT;
	కొలత_t ఉపసర్గ_len;
	నిర్మాణం command *cmd;

	strbuf_addstr(&refname_నిండిన, get_git_namespace());
	ఉపసర్గ_len = refname_నిండిన.len;

	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
		ఒకవేళ (cmd->error_string)
			కొనసాగు;

		strbuf_setlen(&refname_నిండిన, ఉపసర్గ_len);
		strbuf_addstr(&refname_నిండిన, cmd->ref_name);

		ఒకవేళ (!ref_is_hidden(cmd->ref_name, refname_నిండిన.buf, &hidden_refs))
			కొనసాగు;
		ఒకవేళ (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		లేకపోతే
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_నిండిన);
}

స్థిర పూర్ణాంకం should_process_cmd(నిర్మాణం command *cmd)
{
	తిరిగి !cmd->error_string && !cmd->skip_update;
}

స్థిర శూన్య BUG_if_skipped_connectivity_check(నిర్మాణం command *commands,
					       నిర్మాణం shallow_info *si)
{
	నిర్మాణం command *cmd;

	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
		ఒకవేళ (should_process_cmd(cmd) && si->shallow_ref[cmd->సూచీ])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

స్థిర శూన్య ref_transaction_rejection_handler(స్థిరరాశి అక్షరం *refname,
					      స్థిరరాశి నిర్మాణం object_id *old_oid వాడని,
					      స్థిరరాశి నిర్మాణం object_id *new_oid వాడని,
					      స్థిరరాశి అక్షరం *old_లక్ష్యం వాడని,
					      స్థిరరాశి అక్షరం *new_లక్ష్యం వాడని,
					      గణన ref_transaction_error err,
					      స్థిరరాశి అక్షరం *details,
					      శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం strmap *failed_refs = cb_దత్తాంశం;

	ఒకవేళ (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (అక్షరం *)ref_transaction_error_సందేశం(err));
}

స్థిర శూన్య execute_commands_non_atomic(నిర్మాణం command *commands,
					నిర్మాణం shallow_info *si)
{
	నిర్మాణం command *cmd;
	నిర్మాణం strbuf err = STRBUF_INIT;
	స్థిరరాశి అక్షరం *reported_error = ఖాళీ;
	నిర్మాణం strmap failed_refs = STRMAP_INIT;

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
	గణన processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	కొరకు (గణన processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
			ఒకవేళ (!should_process_cmd(cmd) || cmd->నిరంతరం_proc_receive)
				కొనసాగు;

			ఒకవేళ (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				కొనసాగు;
			లేకపోతే ఒకవేళ (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				కొనసాగు;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			ఒకవేళ (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				ఒకవేళ (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					వెళ్ళు failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		ఒకవేళ (!transaction)
			వెళ్ళు cleanup;

		ఒకవేళ (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			వెళ్ళు failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		ఒకవేళ (strmap_ఖాళీలు(&failed_refs))
			వెళ్ళు cleanup;

	failure:
		కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
			ఒకవేళ (reported_error)
				cmd->error_string = reported_error;
			లేకపోతే ఒకవేళ (strmap_కలిగియుంది(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = ఖాళీ;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

స్థిర శూన్య execute_commands_atomic(నిర్మాణం command *commands,
					నిర్మాణం shallow_info *si)
{
	నిర్మాణం command *cmd;
	నిర్మాణం strbuf err = STRBUF_INIT;
	స్థిరరాశి అక్షరం *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ఒకవేళ (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		వెళ్ళు failure;
	}

	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
		ఒకవేళ (!should_process_cmd(cmd) || cmd->నిరంతరం_proc_receive)
			కొనసాగు;

		cmd->error_string = update(cmd, si);

		ఒకవేళ (cmd->error_string)
			వెళ్ళు failure;
	}

	ఒకవేళ (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		వెళ్ళు failure;
	}
	వెళ్ళు cleanup;

failure:
	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి)
		ఒకవేళ (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

స్థిర శూన్య execute_commands(నిర్మాణం command *commands,
			     పూర్ణాంకం unpacker_error,
			     నిర్మాణం shallow_info *si,
			     నిర్మాణం odb_transaction *transaction,
			     స్థిరరాశి నిర్మాణం string_list *నెట్టు_options)
{
	నిర్మాణం check_connected_options opt = CHECK_CONNECTED_INIT;
	నిర్మాణం command *cmd;
	నిర్మాణం iterate_దత్తాంశం దత్తాంశం;
	నిర్మాణం async muxer;
	పూర్ణాంకం err_fd = 0;
	పూర్ణాంకం నిరంతరం_proc_receive = 0;

	ఒకవేళ (unpacker_error) {
		కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి)
			cmd->error_string = "unpacker error";
		తిరిగి;
	}

	ఒకవేళ (!skip_connectivity_check) {
		నిర్మాణం strvec env = STRVEC_INIT;

		ఒకవేళ (use_sideband) {
			memset(&muxer, 0, పరిమాణం(muxer));
			muxer.proc = copy_గమ్యం_sideband;
			muxer.లోపల = -1;
			ఒకవేళ (!ప్రారంభం_async(&muxer))
				err_fd = muxer.లోపల;
			/* ...else, continue without relaying sideband */
		}

		దత్తాంశం.cmds = commands;
		దత్తాంశం.si = si;
		opt.err_fd = err_fd;
		opt.పురోగతి = err_fd && !quiet;
		odb_transaction_env(transaction, &env);
		opt.env = env.v;
		opt.exclude_hidden_refs_section = "receive";

		ఒకవేళ (check_connected(iterate_receive_command_list, &దత్తాంశం, &opt))
			సమితి_connectivity_errors(commands, si, transaction);

		ఒకవేళ (use_sideband)
			ముగింపు_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_గమ్యం_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	ఒకవేళ (proc_receive_ref) {
		కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
			ఒకవేళ (!should_process_cmd(cmd))
				కొనసాగు;

			ఒకవేళ (proc_receive_ref_matches(cmd)) {
				cmd->నిరంతరం_proc_receive = నిరంతరం_PROC_RECEIVE_SCHEDULED;
				నిరంతరం_proc_receive = 1;
			}
		}
	}

	ఒకవేళ (నిరంతరం_receive_hook(commands, "pre-receive", 0, transaction, నెట్టు_options)) {
		కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
			ఒకవేళ (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		తిరిగి;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	కొరకు (cmd = commands; cmd && cmd->error_string; cmd = cmd->తదుపరి)
		; /* nothing */
	ఒకవేళ (!cmd)
		తిరిగి;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	ఒకవేళ (odb_transaction_commit(transaction)) {
		కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
			ఒకవేళ (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		తిరిగి;
	}

	check_aliased_updates(commands);

	విడుదల(తల_name_గమ్యం_free);
	తల_name = తల_name_గమ్యం_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, ఖాళీ,
							    ఖాళీ);

	ఒకవేళ (నిరంతరం_proc_receive &&
	    నిరంతరం_proc_receive_hook(commands, నెట్టు_options))
		కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి)
			ఒకవేళ (!cmd->error_string &&
			    !(cmd->నిరంతరం_proc_receive & నిరంతరం_PROC_RECEIVE_RETURNED) &&
			    (cmd->నిరంతరం_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	ఒకవేళ (use_atomic)
		execute_commands_atomic(commands, si);
	లేకపోతే
		execute_commands_non_atomic(commands, si);

	ఒకవేళ (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

స్థిర నిర్మాణం command **వరుస_command(నిర్మాణం command **తోక,
				      స్థిరరాశి అక్షరం *పంక్తి,
				      పూర్ణాంకం linelen)
{
	నిర్మాణం object_id old_oid, new_oid;
	నిర్మాణం command *cmd;
	స్థిరరాశి అక్షరం *refname;
	పూర్ణాంకం reflen;
	స్థిరరాశి అక్షరం *p;

	ఒకవేళ (parse_oid_hex(పంక్తి, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", పంక్తి);

	refname = p;
	reflen = linelen - (p - పంక్తి);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*తోక = cmd;
	తిరిగి &cmd->తదుపరి;
}

స్థిర శూన్య free_commands(నిర్మాణం command *commands)
{
	అయితే (commands) {
		నిర్మాణం command *తదుపరి = commands->తదుపరి;

		ref_నెట్టు_report_free(commands->report);
		విడుదల(commands->error_string_owned);
		విడుదల(commands);
		commands = తదుపరి;
	}
}

స్థిర శూన్య వరుస_commands_from_cert(నిర్మాణం command **తోక,
				     నిర్మాణం strbuf *నెట్టు_cert)
{
	స్థిరరాశి అక్షరం *boc, *eoc;

	ఒకవేళ (*తోక)
		die("protocol error: got both push certificate and unsigned commands");

	boc = అక్షరశ్రేణి_శోధన(నెట్టు_cert->buf, "\n\n");
	ఒకవేళ (!boc)
		die("malformed push certificate %.*s", 100, నెట్టు_cert->buf);
	లేకపోతే
		boc += 2;
	eoc = నెట్టు_cert->buf + parse_signed_బఫరు(నెట్టు_cert->buf, నెట్టు_cert->len);

	అయితే (boc < eoc) {
		స్థిరరాశి అక్షరం *eol = memchr(boc, '\n', eoc - boc);
		తోక = వరుస_command(తోక, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

స్థిర నిర్మాణం command *read_తల_info(నిర్మాణం packet_reader *reader,
				      నిర్మాణం oid_array *shallow,
				      గణన report_స్థితి_సంస్కరణ *సంస్కరణ)
{
	నిర్మాణం command *commands = ఖాళీ;
	నిర్మాణం command **p = &commands;
	కొరకు (;;) {
		పూర్ణాంకం linelen;

		ఒకవేళ (packet_reader_read(reader) != PACKET_READ_సాధారణ)
			ఆపు;

		ఒకవేళ (reader->pktlen > 8 && starts_with(reader->పంక్తి, "shallow ")) {
			నిర్మాణం object_id oid;
			ఒకవేళ (get_oid_hex(reader->పంక్తి + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->పంక్తి + 8);
			oid_array_append(shallow, &oid);
			కొనసాగు;
		}

		linelen = అక్షరశ్రేణి_పొడవు(reader->పంక్తి);
		ఒకవేళ (linelen < reader->pktlen) {
			స్థిరరాశి అక్షరం *feature_list = reader->పంక్తి + linelen + 1;
			స్థిరరాశి అక్షరం *hash = ఖాళీ;
			స్థిరరాశి అక్షరం *client_sid;
			కొలత_t len = 0;
			ఒకవేళ (parse_feature_request(feature_list, "report-status"))
				*సంస్కరణ = REPORT_స్థితి_V0;
			ఒకవేళ (parse_feature_request(feature_list, "report-status-v2"))
				*సంస్కరణ = REPORT_స్థితి_V2;
			ఒకవేళ (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			ఒకవేళ (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			ఒకవేళ (advertise_atomic_నెట్టు
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			ఒకవేళ (advertise_నెట్టు_options
			    && parse_feature_request(feature_list, "push-options"))
				use_నెట్టు_options = 1;
			hash = parse_feature_విలువ(feature_list, "object-format", &len, ఖాళీ);
			ఒకవేళ (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = అక్షరశ్రేణి_పొడవు(hash);
			}
			ఒకవేళ (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_విలువ(feature_list, "session-id", &len, ఖాళీ);
			ఒకవేళ (client_sid) {
				అక్షరం *sid = xstrndup(client_sid, len);
				trace2_దత్తాంశం_string("transfer", ఖాళీ, "client-sid", client_sid);
				విడుదల(sid);
			}
		}

		ఒకవేళ (!strcmp(reader->పంక్తి, "push-cert")) {
			పూర్ణాంకం true_flush = 0;
			పూర్ణాంకం saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			కొరకు (;;) {
				packet_reader_read(reader);
				ఒకవేళ (reader->స్థితి_2 == PACKET_READ_FLUSH) {
					true_flush = 1;
					ఆపు;
				}
				ఒకవేళ (reader->స్థితి_2 != PACKET_READ_సాధారణ) {
					die("protocol error: got an unexpected packet");
				}
				ఒకవేళ (!strcmp(reader->పంక్తి, "push-cert-end\n"))
					ఆపు; /* end of cert */
				strbuf_addstr(&నెట్టు_cert, reader->పంక్తి);
			}
			reader->options = saved_options;

			ఒకవేళ (true_flush)
				ఆపు;
			కొనసాగు;
		}

		p = వరుస_command(p, reader->పంక్తి, linelen);
	}

	ఒకవేళ (నెట్టు_cert.len)
		వరుస_commands_from_cert(p, &నెట్టు_cert);

	తిరిగి commands;
}

స్థిర శూన్య read_నెట్టు_options(నిర్మాణం packet_reader *reader,
			      నిర్మాణం string_list *options)
{
	అయితే (1) {
		ఒకవేళ (packet_reader_read(reader) != PACKET_READ_సాధారణ)
			ఆపు;

		string_list_append(options, reader->పంక్తి);
	}
}

స్థిర పూర్ణాంకం unpack_with_sideband(నిర్మాణం odb_transaction *transaction,
				స్థిరరాశి అక్షరం *shallow_file,
				నిర్మాణం strbuf *err_సందేశం)
{
	నిర్మాణం odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_సందేశం_types = fsck_సందేశం_types.buf,
		.max_ఆదానం_కొలత = max_ఆదానం_కొలత,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	నిర్మాణం async muxer;
	పూర్ణాంకం ret;

	ఒకవేళ (!use_sideband)
		తిరిగి odb_transaction_write_pack(transaction, 0, err_సందేశం, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, పరిమాణం(muxer));
	muxer.proc = copy_గమ్యం_sideband;
	muxer.లోపల = -1;
	ఒకవేళ (ప్రారంభం_async(&muxer))
		తిరిగి 0;

	opts.err_fd = muxer.లోపల;
	ret = odb_transaction_write_pack(transaction, 0, err_సందేశం, &opts);

	ముగింపు_async(&muxer);
	తిరిగి ret;
}

స్థిర శూన్య prepare_shallow_update(నిర్మాణం shallow_info *si,
				   స్థిరరాశి అక్షరం *shallow_file)
{
	పూర్ణాంకం i, j, k, bitmap_కొలత = DIV_విడత_పైకి(si->ref->nr, 32);

	ALLOC_ARRAY(si->వాడిన_shallow, si->shallow->nr);
	కేటాయించు_shallow_commits_గమ్యం_refs(si, si->వాడిన_shallow, ఖాళీ);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	కొరకు (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	కొరకు (i = 0; i < si->shallow->nr; i++) {
		ఒకవేళ (!si->వాడిన_shallow[i])
			కొనసాగు;
		కొరకు (j = 0; j < bitmap_కొలత; j++) {
			ఒకవేళ (!si->వాడిన_shallow[i][j])
				కొనసాగు;
			si->need_reachability_test[i]++;
			కొరకు (k = 0; k < 32; k++)
				ఒకవేళ (si->వాడిన_shallow[i][j] & (1U << k))
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

స్థిర శూన్య update_shallow_info(నిర్మాణం command *commands,
				నిర్మాణం shallow_info *si,
				నిర్మాణం oid_array *ref,
				స్థిరరాశి అక్షరం *shallow_file)
{
	నిర్మాణం command *cmd;
	పూర్ణాంకం *ref_స్థితి;
	remove_nonexistent_theirs_shallow(si);
	ఒకవేళ (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		తిరిగి;
	}

	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
		ఒకవేళ (is_null_oid(&cmd->new_oid))
			కొనసాగు;
		oid_array_append(ref, &cmd->new_oid);
		cmd->సూచీ = ref->nr - 1;
	}
	si->ref = ref;

	ఒకవేళ (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		తిరిగి;
	}

	ALLOC_ARRAY(ref_స్థితి, ref->nr);
	కేటాయించు_shallow_commits_గమ్యం_refs(si, ఖాళీ, ref_స్థితి);
	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
		ఒకవేళ (is_null_oid(&cmd->new_oid))
			కొనసాగు;
		ఒకవేళ (ref_స్థితి[cmd->సూచీ]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	విడుదల(ref_స్థితి);
}

స్థిర శూన్య override_cmds_error(నిర్మాణం command *commands, స్థిరరాశి అక్షరం *err)
{
	కొరకు (నిర్మాణం command *cmd = commands; cmd; cmd = cmd->తదుపరి) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
స్థిర శూన్య generate_report(నిర్మాణం strbuf *buf, నిర్మాణం command *commands,
			    స్థిరరాశి నిర్మాణం strbuf *unpack_స్థితి,
			    గణన report_స్థితి_సంస్కరణ సంస్కరణ)
{
	నిర్మాణం command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_స్థితి->len ? unpack_స్థితి->buf : "ok");

	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
		నిర్మాణం ref_నెట్టు_report *report;
		పూర్ణాంకం లెక్క = 0;

		ఒకవేళ (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		లేకపోతే
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		ఒకవేళ (సంస్కరణ != REPORT_స్థితి_V2 || cmd->error_string)
			కొనసాగు;

		కొరకు (report = cmd->report; report; report = report->తదుపరి) {
			ఒకవేళ (లెక్క++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			ఒకవేళ (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			ఒకవేళ (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_గమ్యం_hex(report->old_oid));
			ఒకవేళ (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_గమ్యం_hex(report->new_oid));
			ఒకవేళ (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

స్థిర శూన్య report(నిర్మాణం command *commands, స్థిరరాశి నిర్మాణం strbuf *unpack_స్థితి,
		   గణన report_స్థితి_సంస్కరణ సంస్కరణ)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_స్థితి, సంస్కరణ);

	ఒకవేళ (నిరంతరం_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_స్థితి, సంస్కరణ);
	}

	ఒకవేళ (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	లేకపోతే
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

స్థిర పూర్ణాంకం delete_only(నిర్మాణం command *commands)
{
	నిర్మాణం command *cmd;
	కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి) {
		ఒకవేళ (!is_null_oid(&cmd->new_oid))
			తిరిగి 0;
	}
	తిరిగి 1;
}

పూర్ణాంకం cmd_receive_pack(పూర్ణాంకం argc,
		     స్థిరరాశి అక్షరం **argv,
		     స్థిరరాశి అక్షరం *ఉపసర్గ,
		     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం advertise_refs = 0;
	నిర్మాణం command *commands;
	నిర్మాణం oid_array shallow = OID_ARRAY_INIT;
	నిర్మాణం oid_array ref = OID_ARRAY_INIT;
	నిర్మాణం shallow_info si;
	నిర్మాణం packet_reader reader;
	నిర్మాణం odb_transaction *transaction = ఖాళీ;
	గణన report_స్థితి_సంస్కరణ సంస్కరణ = REPORT_స్థితి_UNKNOWN;

	నిర్మాణం option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, ఖాళీ),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, ఖాళీ),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, ఖాళీ),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, ఖాళీ),
		OPT_చివర()
	};

	packet_trace_తటస్థం("receive-pack");

	argc = parse_options(argc, argv, ఉపసర్గ, options, receive_pack_usage, 0);

	ఒకవేళ (argc > 1)
		usage_సందేశం_opt(_("too many arguments"), receive_pack_usage, options);
	ఒకవేళ (argc == 0)
		usage_సందేశం_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	అమరిక_మార్గం();

	ఒకవేళ (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, ఖాళీ);
	ఒకవేళ (cert_nonce_విత్తనం)
		నెట్టు_cert_nonce = prepare_నెట్టు_cert_nonce(service_dir, సమయం(ఖాళీ));

	ఎంపిక (determine_protocol_సంస్కరణ_server()) {
	స్థితి protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		ఆపు;
	స్థితి protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		ఒకవేళ (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	స్థితి protocol_v0:
		ఆపు;
	స్థితి protocol_unknown_సంస్కరణ:
		BUG("unknown protocol version");
	}

	ఒకవేళ (advertise_refs || !stateless_rpc) {
		write_తల_info();
	}
	ఒకవేళ (advertise_refs)
		తిరిగి 0;

	packet_reader_init(&reader, 0, ఖాళీ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ఒకవేళ ((commands = read_తల_info(&reader, &shallow, &సంస్కరణ))) {
		నిర్మాణం string_list నెట్టు_options = STRING_LIST_INIT_DUP;
		నిర్మాణం strbuf unpack_స్థితి = STRBUF_INIT;

		ఒకవేళ (use_నెట్టు_options)
			read_నెట్టు_options(&reader, &నెట్టు_options);
		ఒకవేళ (!check_cert_నెట్టు_options(&నెట్టు_options)) {
			నిర్మాణం command *cmd;
			కొరకు (cmd = commands; cmd; cmd = cmd->తదుపరి)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		ఒకవేళ (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		ఒకవేళ (!delete_only(commands)) {
			స్థిరరాశి అక్షరం *alt_shallow_file = ఖాళీ;

			ఒకవేళ (si.nr_ours || si.nr_theirs)
				alt_shallow_file = అమరిక_temporary_shallow(si.shallow);

			ఒకవేళ (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_స్థితి, "unable to start object transaction");
			లేకపోతే
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_స్థితి);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_స్థితి.len, &si, transaction,
				 &నెట్టు_options);
		odb_transaction_finalize(transaction);
		sigchain_నెట్టు(SIGPIPE, SIG_IGN);

		ఎంపిక (సంస్కరణ) {
		స్థితి REPORT_స్థితి_V2:
		స్థితి REPORT_స్థితి_V0:
			report(commands, &unpack_స్థితి, సంస్కరణ);
			ఆపు;
		స్థితి REPORT_స్థితి_UNKNOWN:
			ఆపు;
		}

		sigchain_తీయి(SIGPIPE);
		నిరంతరం_receive_hook(commands, "post-receive", 1, ఖాళీ,
				 &నెట్టు_options);
		నిరంతరం_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&నెట్టు_options, 0);
		ఒకవేళ (auto_gc) {
			నిర్మాణం బిడ్డ_process proc = బిడ్డ_PROCESS_INIT;

			ఒకవేళ (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_గమ్యం_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				ఒకవేళ (!ప్రారంభం_command(&proc)) {
					ఒకవేళ (use_sideband)
						copy_గమ్యం_sideband(proc.err, -1, ఖాళీ);
					ముగింపు_command(&proc);
				}
			}
		}
		ఒకవేళ (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_స్థితి);
	}
	ఒకవేళ (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	విడుదల((శూన్య *)నెట్టు_cert_nonce);
	తిరిగి 0;
}
