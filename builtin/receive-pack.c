#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "connect.h"
#অন্তর্ভাব "connected.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "exec-cmd.h"
#অন্তর্ভাব "fsck.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "gpg-interface.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "hook.h"
#অন্তর্ভাব "object.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "oid-array.h"
#অন্তর্ভাব "oidset.h"
#অন্তর্ভাব "pack.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "pkt-line.h"
#অন্তর্ভাব "protocol.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "server-info.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "shallow.h"
#অন্তর্ভাব "sideband.h"
#অন্তর্ভাব "sigchain.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "trace.h"
#অন্তর্ভাব "trace2.h"
#অন্তর্ভাব "version.h"
#অন্তর্ভাব "worktree.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	ফাঁকা
};

গণনা deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

গণনা report_status_version {
	REPORT_STATUS_UNKNOWN = 0,
	REPORT_STATUS_V0,
	REPORT_STATUS_V2,
};

স্থির পূর্ণসংখ্যা deny_deletes;
স্থির পূর্ণসংখ্যা deny_non_fast_forwards;
স্থির গণনা deny_action deny_current_branch = DENY_UNCONFIGURED;
স্থির গণনা deny_action deny_delete_current = DENY_UNCONFIGURED;
স্থির পূর্ণসংখ্যা receive_fsck_objects = -1;
স্থির পূর্ণসংখ্যা transfer_fsck_objects = -1;
স্থির কাঠামো strbuf fsck_বার্তা_types = STRBUF_INIT;
স্থির পূর্ণসংখ্যা advertise_atomic_ঠেলো = 1;
স্থির পূর্ণসংখ্যা advertise_ঠেলো_options;
স্থির পূর্ণসংখ্যা advertise_sid;
স্থির off_t max_প্রবেশ_আকার;
স্থির পূর্ণসংখ্যা use_sideband;
স্থির পূর্ণসংখ্যা use_atomic;
স্থির পূর্ণসংখ্যা use_ঠেলো_options;
স্থির পূর্ণসংখ্যা quiet;
স্থির পূর্ণসংখ্যা prefer_ofs_delta = 1;
স্থির পূর্ণসংখ্যা auto_update_server_info;
স্থির পূর্ণসংখ্যা auto_gc = 1;
স্থির পূর্ণসংখ্যা reject_thin;
স্থির পূর্ণসংখ্যা skip_connectivity_check;
স্থির পূর্ণসংখ্যা stateless_rpc;
স্থির ধ্রুবক অক্ষর *service_dir;
স্থির ধ্রুবক অক্ষর *শীর্ষ_name;
স্থির শূন্য *শীর্ষ_name_to_free;
স্থির পূর্ণসংখ্যা sent_capabilities;
স্থির পূর্ণসংখ্যা shallow_update;
স্থির কাঠামো strbuf ঠেলো_cert = STRBUF_INIT;
স্থির কাঠামো object_id ঠেলো_cert_oid;
স্থির কাঠামো signature_check sigcheck;
স্থির ধ্রুবক অক্ষর *ঠেলো_cert_nonce;
স্থির অক্ষর *cert_nonce_seed;
স্থির কাঠামো strvec hidden_refs = STRVEC_INIT;

স্থির ধ্রুবক অক্ষর *NONCE_UNSOLICITED = "UNSOLICITED";
স্থির ধ্রুবক অক্ষর *NONCE_BAD = "BAD";
স্থির ধ্রুবক অক্ষর *NONCE_MISSING = "MISSING";
স্থির ধ্রুবক অক্ষর *NONCE_OK = "OK";
স্থির ধ্রুবক অক্ষর *NONCE_SLOP = "SLOP";
স্থির ধ্রুবক অক্ষর *nonce_status;
স্থির long nonce_stamp_slop;
স্থির timestamp_t nonce_stamp_slop_limit;
স্থির কাঠামো ref_transaction *transaction;

স্থির গণনা {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
স্থির পূর্ণসংখ্যা keepalive_in_sec = 5;

স্থির কাঠামো proc_receive_ref {
	unsigned পূর্ণসংখ্যা want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	অক্ষর *ref_prefix;
	কাঠামো proc_receive_ref *পরবর্তী;
} *proc_receive_ref;

স্থির শূন্য proc_receive_ref_append(ধ্রুবক অক্ষর *prefix);

স্থির গণনা deny_action parse_deny_action(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান)
{
	যদি (মান) {
		যদি (!strcasecmp(মান, "ignore"))
			ফেরত DENY_IGNORE;
		যদি (!strcasecmp(মান, "warn"))
			ফেরত DENY_WARN;
		যদি (!strcasecmp(মান, "refuse"))
			ফেরত DENY_REFUSE;
		যদি (!strcasecmp(মান, "updateinstead"))
			ফেরত DENY_UPDATE_INSTEAD;
	}
	যদি (git_config_bool(var, মান))
		ফেরত DENY_REFUSE;
	ফেরত DENY_IGNORE;
}

স্থির পূর্ণসংখ্যা receive_pack_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			       ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	ধ্রুবক অক্ষর *বার্তা_id;
	পূর্ণসংখ্যা status = parse_hide_refs_config(var, মান, "receive", &hidden_refs);

	যদি (status)
		ফেরত status;

	যদি (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.fsck.skiplist") == 0) {
		অক্ষর *path;

		যদি (git_config_pathname(&path, var, মান))
			ফেরত -1;
		যদি (path)
			strbuf_addf(&fsck_বার্তা_types, "%cskiplist=%s",
				    fsck_বার্তা_types.len ? ',' : '=', path);
		মুক্ত(path);
		ফেরত 0;
	}

	যদি (skip_prefix(var, "receive.fsck.", &বার্তা_id)) {
		যদি (!মান)
			ফেরত config_error_nonbool(var);
		যদি (is_valid_বার্তা_type(বার্তা_id, মান))
			strbuf_addf(&fsck_বার্তা_types, "%c%s=%s",
				fsck_বার্তা_types.len ? ',' : '=', বার্তা_id, মান);
		অন্যথা
			warning("skipping unknown msg id '%s'", বার্তা_id);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, মান);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, মান);
		ফেরত 0;
	}

	যদি (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.certnonceseed") == 0)
		ফেরত git_config_string(&cert_nonce_seed, var, মান);

	যদি (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, মান, ctx->kvi);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_ঠেলো = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_ঠেলো_options = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, মান, ctx->kvi);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.maxinputsize") == 0) {
		max_প্রবেশ_আকার = git_config_int64(var, মান, ctx->kvi);
		ফেরত 0;
	}

	যদি (strcmp(var, "receive.procreceiverefs") == 0) {
		যদি (!মান)
			ফেরত config_error_nonbool(var);
		proc_receive_ref_append(মান);
		ফেরত 0;
	}

	যদি (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, মান);
		ফেরত 0;
	}

	ফেরত git_default_config(var, মান, ctx, cb);
}

স্থির শূন্য show_ref(ধ্রুবক অক্ষর *path, ধ্রুবক কাঠামো object_id *oid)
{
	যদি (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} অন্যথা {
		কাঠামো strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		যদি (advertise_atomic_ঠেলো)
			strbuf_addstr(&cap, " atomic");
		যদি (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		যদি (ঠেলো_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", ঠেলো_cert_nonce);
		যদি (advertise_ঠেলো_options)
			strbuf_addstr(&cap, " push-options");
		যদি (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

স্থির পূর্ণসংখ্যা show_ref_cb(ধ্রুবক কাঠামো reference *ref, শূন্য *তথ্য)
{
	কাঠামো oidset *seen = তথ্য;
	ধ্রুবক অক্ষর *path = strip_namespace(ref->name);

	যদি (ref_is_hidden(path, ref->name, &hidden_refs))
		ফেরত 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	যদি (!path) {
		যদি (oidset_ঢোকাও(seen, ref->oid))
			ফেরত 0;
		path = ".have";
	} অন্যথা {
		oidset_ঢোকাও(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	ফেরত 0;
}

স্থির শূন্য show_one_alternate_ref(ধ্রুবক কাঠামো object_id *oid,
				   শূন্য *তথ্য)
{
	কাঠামো oidset *seen = তথ্য;

	যদি (oidset_ঢোকাও(seen, oid))
		ফেরত;

	show_ref(".have", oid);
}

স্থির শূন্য write_শীর্ষ_info(শূন্য)
{
	কাঠামো refs_for_each_ref_options opts = { 0 };
	স্থির কাঠামো oidset seen = OIDSET_INIT;
	কাঠামো strvec excludes_vector = STRVEC_INIT;

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

	যদি (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#define RUN_PROC_RECEIVE_SCHEDULED	1
#define RUN_PROC_RECEIVE_RETURNED	2
কাঠামো command {
	কাঠামো command *পরবর্তী;
	ধ্রুবক অক্ষর *error_string;
	অক্ষর *error_string_owned;
	কাঠামো ref_ঠেলো_report *report;
	unsigned পূর্ণসংখ্যা skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	পূর্ণসংখ্যা সূচক;
	কাঠামো object_id old_oid;
	কাঠামো object_id new_oid;
	অক্ষর ref_name[FLEX_ARRAY]; /* more */
};

স্থির শূন্য proc_receive_ref_append(ধ্রুবক অক্ষর *prefix)
{
	কাঠামো proc_receive_ref *ref_pattern;
	ধ্রুবক অক্ষর *p;
	পূর্ণসংখ্যা len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = strchr(prefix, ':');
	যদি (p) {
		যখন (prefix < p) {
			যদি (*prefix == 'a')
				ref_pattern->want_add = 1;
			অন্যথা যদি (*prefix == 'd')
				ref_pattern->want_delete = 1;
			অন্যথা যদি (*prefix == 'm')
				ref_pattern->want_modify = 1;
			অন্যথা যদি (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} অন্যথা {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	যখন (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	যদি (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} অন্যথা {
		কাঠামো proc_receive_ref *end;

		end = proc_receive_ref;
		যখন (end->পরবর্তী)
			end = end->পরবর্তী;
		end->পরবর্তী = ref_pattern;
	}
}

স্থির পূর্ণসংখ্যা proc_receive_ref_matches(কাঠামো command *cmd)
{
	কাঠামো proc_receive_ref *p;

	যদি (!proc_receive_ref)
		ফেরত 0;

	জন্য (p = proc_receive_ref; p; p = p->পরবর্তী) {
		ধ্রুবক অক্ষর *match = p->ref_prefix;
		ধ্রুবক অক্ষর *remains;

		যদি (!p->want_add && is_null_oid(&cmd->old_oid))
			চালু;
		অন্যথা যদি (!p->want_delete && is_null_oid(&cmd->new_oid))
			চালু;
		অন্যথা যদি (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			চালু;

		যদি (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			যদি (!p->negative_ref)
				ফেরত 1;
		} অন্যথা যদি (p->negative_ref) {
			ফেরত 1;
		}
	}
	ফেরত 0;
}

স্থির শূন্য report_message(ধ্রুবক অক্ষর *prefix, ধ্রুবক অক্ষর *err, va_list params)
{
	পূর্ণসংখ্যা sz;
	অক্ষর বার্তা[4096];

	sz = xsnprintf(বার্তা, আকার(বার্তা), "%s", prefix);
	sz += vsnprintf(বার্তা + sz, আকার(বার্তা) - sz, err, params);
	যদি (sz > (আকার(বার্তা) - 1))
		sz = আকার(বার্তা) - 1;
	বার্তা[sz++] = '\n';

	যদি (use_sideband)
		send_sideband(1, 2, বার্তা, sz, use_sideband);
	অন্যথা
		xwrite(2, বার্তা, sz);
}

__attribute__((format (printf, 1, 2)))
স্থির শূন্য rp_warning(ধ্রুবক অক্ষর *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("warning: ", err, params);
	va_end(params);
}

__attribute__((format (printf, 1, 2)))
স্থির শূন্য rp_error(ধ্রুবক অক্ষর *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("error: ", err, params);
	va_end(params);
}

স্থির পূর্ণসংখ্যা copy_to_sideband(পূর্ণসংখ্যা in, পূর্ণসংখ্যা out UNUSED, শূন্য *arg UNUSED)
{
	অক্ষর তথ্য[128];
	পূর্ণসংখ্যা keepalive_active = 0;

	যদি (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	যদি (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	যখন (1) {
		ssize_t sz;

		যদি (keepalive_active) {
			কাঠামো pollfd pfd;
			পূর্ণসংখ্যা ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			যদি (ret < 0) {
				যদি (errno == EINTR)
					চালু;
				অন্যথা
					বিরতি;
			} অন্যথা যদি (ret == 0) {
				/* no data; send a keepalive packet */
				স্থির ধ্রুবক অক্ষর buf[] = "0005\1";
				write_or_die(1, buf, আকার(buf) - 1);
				চালু;
			} /* else there is actual data to read */
		}

		sz = xread(in, তথ্য, আকার(তথ্য));
		যদি (sz <= 0)
			বিরতি;

		যদি (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			ধ্রুবক অক্ষর *p = memchr(তথ্য, '\0', sz);
			যদি (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_active = 1;
				send_sideband(1, 2, তথ্য, p - তথ্য, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - তথ্য + 1), use_sideband);
				চালু;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, তথ্য, sz, use_sideband);
	}
	close(in);
	ফেরত 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
স্থির শূন্য prepare_sideband_async(কাঠামো async *sideband_async, পূর্ণসংখ্যা *saved_stderr, পূর্ণসংখ্যা *started)
{
	*started = 0;

	যদি (!use_sideband)
		ফেরত;

	memset(sideband_async, 0, আকার(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	যদি (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		যদি (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
স্থির শূন্য finish_sideband_async(কাঠামো async *sideband_async, পূর্ণসংখ্যা saved_stderr, পূর্ণসংখ্যা started)
{
	যদি (!use_sideband)
		ফেরত;

	যদি (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	যদি (started)
		finish_async(sideband_async);
}

স্থির শূন্য hmac_hash(unsigned অক্ষর *out,
		      ধ্রুবক অক্ষর *চাবি_in, আকার_t চাবি_len,
		      ধ্রুবক অক্ষর *text, আকার_t text_len)
{
	unsigned অক্ষর চাবি[GIT_MAX_BLKSZ];
	unsigned অক্ষর k_ipad[GIT_MAX_BLKSZ];
	unsigned অক্ষর k_opad[GIT_MAX_BLKSZ];
	পূর্ণসংখ্যা i;
	কাঠামো git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(চাবি, '\0', GIT_MAX_BLKSZ);
	যদি (the_hash_algo->blksz < চাবি_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, চাবি_in, চাবি_len);
		git_hash_final(চাবি, &ctx);
	} অন্যথা {
		memcpy(চাবি, চাবি_in, চাবি_len);
	}

	/* RFC 2104 2. (2) & (5) */
	জন্য (i = 0; i < আকার(চাবি); i++) {
		k_ipad[i] = চাবি[i] ^ 0x36;
		k_opad[i] = চাবি[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, আকার(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, আকার(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

স্থির অক্ষর *prepare_ঠেলো_cert_nonce(ধ্রুবক অক্ষর *path, timestamp_t stamp)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	unsigned অক্ষর hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, strlen(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (পূর্ণসংখ্যা)the_hash_algo->hexsz, hash_to_hex(hash));
	ফেরত strbuf_detach(&buf, ফাঁকা);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
স্থির পূর্ণসংখ্যা constant_memequal(ধ্রুবক অক্ষর *a, ধ্রুবক অক্ষর *b, আকার_t n)
{
	পূর্ণসংখ্যা res = 0;
	আকার_t i;

	জন্য (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	ফেরত res;
}

স্থির ধ্রুবক অক্ষর *check_nonce(ধ্রুবক অক্ষর *buf)
{
	আকার_t noncelen;
	ধ্রুবক অক্ষর *found = find_commit_header(buf, "nonce", &noncelen);
	অক্ষর *nonce = found ? xmemdupz(found, noncelen) : ফাঁকা;
	timestamp_t stamp, ostamp;
	অক্ষর *bohmac, *expect = ফাঁকা;
	ধ্রুবক অক্ষর *retval = NONCE_BAD;

	যদি (!nonce) {
		retval = NONCE_MISSING;
		যাও leave;
	} অন্যথা যদি (!ঠেলো_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		যাও leave;
	} অন্যথা যদি (!strcmp(ঠেলো_cert_nonce, nonce)) {
		retval = NONCE_OK;
		যাও leave;
	}

	যদি (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		যাও leave;
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
	যদি (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		যাও leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	যদি (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		যাও leave;
	}

	expect = prepare_ঠেলো_cert_nonce(service_dir, stamp);
	যদি (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		যাও leave;
	}
	যদি (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		যাও leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(ঠেলো_cert_nonce, ফাঁকা, 10);
	nonce_stamp_slop = (long)ostamp - (long)stamp;

	যদি (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		মুক্ত((শূন্য *)ঠেলো_cert_nonce);
		ঠেলো_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} অন্যথা {
		retval = NONCE_SLOP;
	}

leave:
	মুক্ত(nonce);
	মুক্ত(expect);
	ফেরত retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
স্থির পূর্ণসংখ্যা check_cert_ঠেলো_options(ধ্রুবক কাঠামো string_list *ঠেলো_options)
{
	ধ্রুবক অক্ষর *buf = ঠেলো_cert.buf;

	ধ্রুবক অক্ষর *option;
	আকার_t optionlen;
	পূর্ণসংখ্যা options_seen = 0;

	পূর্ণসংখ্যা retval = 1;

	যদি (!*buf)
		ফেরত 1;

	যখন ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		যদি (options_seen > ঠেলো_options->nr
		    || xstrncmpz(ঠেলো_options->বস্তুসমূহ[options_seen - 1].string,
				 option, optionlen))
			ফেরত 0;
	}

	যদি (options_seen != ঠেলো_options->nr)
		retval = 0;

	ফেরত retval;
}

স্থির শূন্য prepare_ঠেলো_cert_sha1(কাঠামো run_hooks_opt *opt)
{
	স্থির পূর্ণসংখ্যা already_done;

	যদি (!ঠেলো_cert.len)
		ফেরত;

	যদি (!already_done) {
		পূর্ণসংখ্যা bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		যদি (odb_write_object(the_repository->objects, ঠেলো_cert.buf,
				     ঠেলো_cert.len, OBJ_BLOB, &ঠেলো_cert_oid))
			oidclr(&ঠেলো_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', আকার(sigcheck));

		bogs = parse_signed_বাফার(ঠেলো_cert.buf, ঠেলো_cert.len);
		sigcheck.payload = xmemdupz(ঠেলো_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, ঠেলো_cert.buf + bogs,
				ঠেলো_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	যদি (!is_null_oid(&ঠেলো_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&ঠেলো_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.চাবি ? sigcheck.চাবি : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.ফলাফল);
		যদি (ঠেলো_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     ঠেলো_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			যদি (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

কাঠামো receive_hook_feed_state {
	কাঠামো command *cmd;
	কাঠামো ref_ঠেলো_report *report;
	পূর্ণসংখ্যা skip_broken;
	কাঠামো strbuf buf;
};

স্থির পূর্ণসংখ্যা feed_receive_hook_cb(পূর্ণসংখ্যা hook_stdin_fd, শূন্য *pp_cb UNUSED, শূন্য *pp_task_cb)
{
	কাঠামো receive_hook_feed_state *state = pp_task_cb;
	কাঠামো command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	যখন (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->পরবর্তী;

	যদি (!cmd)
		ফেরত 1;  /* no more commands left */

	যদি (!state->report)
		state->report = cmd->report;

	যদি (state->report) {
		কাঠামো object_id *old_oid;
		কাঠামো object_id *new_oid;
		ধ্রুবক অক্ষর *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->পরবর্তী;
		যদি (!state->report)
			cmd = cmd->পরবর্তী;
	} অন্যথা {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->পরবর্তী;
	}

	state->cmd = cmd;

	যদি (state->buf.len > 0) {
		পূর্ণসংখ্যা ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		যদি (ret < 0) {
			যদি (errno == EPIPE)
				ফেরত 1; /* child closed pipe */
			ফেরত ret;
		}
	}

	ফেরত state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

স্থির শূন্য *receive_hook_feed_state_alloc(শূন্য *feed_pipe_ctx)
{
	কাঠামো receive_hook_feed_state *init_state = feed_pipe_ctx;
	কাঠামো receive_hook_feed_state *তথ্য;

	CALLOC_ARRAY(তথ্য, 1);
	তথ্য->report = init_state->report;
	তথ্য->cmd = init_state->cmd;
	তথ্য->skip_broken = init_state->skip_broken;
	strbuf_init(&তথ্য->buf, 0);

	ফেরত তথ্য;
}

স্থির শূন্য receive_hook_feed_state_free(শূন্য *তথ্য)
{
	কাঠামো receive_hook_feed_state *d = তথ্য;
	যদি (!d)
		ফেরত;
	strbuf_release(&d->buf);
	মুক্ত(d);
}

স্থির পূর্ণসংখ্যা run_receive_hook(কাঠামো command *commands,
			    ধ্রুবক অক্ষর *hook_name,
			    পূর্ণসংখ্যা skip_broken,
			    কাঠামো odb_transaction *transaction,
			    ধ্রুবক কাঠামো string_list *ঠেলো_options)
{
	কাঠামো run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	কাঠামো command *iter = commands;
	কাঠামো receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	কাঠামো async sideband_async;
	পূর্ণসংখ্যা sideband_async_started = 0;
	পূর্ণসংখ্যা saved_stderr = -1;
	পূর্ণসংখ্যা ret;

	যদি (!hook_exists(the_repository, hook_name))
		ফেরত 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	যখন (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->পরবর্তী;
	যদি (!iter)
		ফেরত 0;

	যদি (ঠেলো_options) {
		জন্য (পূর্ণসংখ্যা i = 0; i < ঠেলো_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     ঠেলো_options->বস্তুসমূহ[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)ঠেলো_options->nr);
	} অন্যথা {
		strvec_ঠেলো(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	যদি (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_ঠেলো_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_তথ্য_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_তথ্য_free = receive_hook_feed_state_free;

	ret = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ফেরত ret;
}

স্থির পূর্ণসংখ্যা run_update_hook(কাঠামো command *cmd)
{
	স্থির ধ্রুবক অক্ষর hook_name[] = "update";
	কাঠামো run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	কাঠামো async sideband_async;
	পূর্ণসংখ্যা sideband_async_started = 0;
	পূর্ণসংখ্যা saved_stderr = -1;
	পূর্ণসংখ্যা code;

	যদি (!hook_exists(the_repository, hook_name))
		ফেরত 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     ফাঁকা);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	ফেরত code;
}

স্থির পূর্ণসংখ্যা run_receive_report_hook(কাঠামো strbuf *report)
{
	কাঠামো child_process proc = CHILD_PROCESS_INIT;
	কাঠামো async sideband_async;
	পূর্ণসংখ্যা sideband_async_started = 0;
	পূর্ণসংখ্যা saved_stderr = -1;
	কাঠামো strbuf out = STRBUF_INIT;
	ধ্রুবক অক্ষর *hook_path;
	পূর্ণসংখ্যা ret;

	hook_path = find_hook(the_repository, "receive-report");
	যদি (!hook_path)
		ফেরত 0;

	strvec_ঠেলো(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_ঠেলো(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, ফাঁকা, 0);
	sigchain_বের_করো(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	যদি (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	ফেরত ret;
}

স্থির কাঠামো command *find_command_by_refname(কাঠামো command *list,
					       ধ্রুবক অক্ষর *refname)
{
	জন্য (; list; list = list->পরবর্তী)
		যদি (!strcmp(list->ref_name, refname))
			ফেরত list;
	ফেরত ফাঁকা;
}

স্থির পূর্ণসংখ্যা read_proc_receive_report(কাঠামো packet_reader *reader,
				    কাঠামো command *commands,
				    কাঠামো strbuf *errmsg)
{
	কাঠামো command *cmd;
	কাঠামো command *hint = ফাঁকা;
	কাঠামো ref_ঠেলো_report *report = ফাঁকা;
	পূর্ণসংখ্যা new_report = 0;
	পূর্ণসংখ্যা code = 0;
	পূর্ণসংখ্যা once = 0;
	পূর্ণসংখ্যা response = 0;

	জন্য (;;) {
		কাঠামো object_id old_oid, new_oid;
		অক্ষর *শীর্ষ;
		অক্ষর *refname;
		অক্ষর *p;
		গণনা packet_read_status status;

		status = packet_reader_read(reader);
		যদি (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			যদি (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				ফেরত -1;
			}
			বিরতি;
		}
		response++;

		শীর্ষ = reader->সারি;
		p = strchr(শীর্ষ, ' ');
		যদি (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", শীর্ষ);
			code = -1;
			চালু;
		}
		*p++ = '\0';
		যদি (!strcmp(শীর্ষ, "option")) {
			অক্ষর *চাবি;
			ধ্রুবক অক্ষর *val;

			যদি (!hint || !(report || new_report)) {
				যদি (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				চালু;
			}
			যদি (new_report) {
				যদি (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} অন্যথা {
					report = hint->report;
					যখন (report->পরবর্তী)
						report = report->পরবর্তী;
					report->পরবর্তী = xcalloc(1, আকার(কাঠামো ref_ঠেলো_report));
					report = report->পরবর্তী;
				}
				new_report = 0;
			}
			চাবি = p;
			p = strchr(চাবি, ' ');
			যদি (p)
				*p++ = '\0';
			val = p;
			যদি (!strcmp(চাবি, "refname"))
				report->ref_name = xstrdup_or_null(val);
			অন্যথা যদি (!strcmp(চাবি, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			অন্যথা যদি (!strcmp(চাবি, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			অন্যথা যদি (!strcmp(চাবি, "forced-update"))
				report->forced_update = 1;
			অন্যথা যদি (!strcmp(চাবি, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			চালু;
		}

		report = ফাঁকা;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		যদি (p)
			*p++ = '\0';
		যদি (strcmp(শীর্ষ, "ok") && strcmp(শীর্ষ, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    শীর্ষ, refname);
			code = -1;
			চালু;
		}

		/* first try searching at our hint, falling back to all refs */
		যদি (hint)
			hint = find_command_by_refname(hint, refname);
		যদি (!hint)
			hint = find_command_by_refname(commands, refname);
		যদি (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			চালু;
		}
		যদি (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			চালু;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		যদি (!strcmp(শীর্ষ, "ng")) {
			যদি (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			অন্যথা
				hint->error_string = "failed";
			code = -1;
			চালু;
		}
		new_report = 1;
	}

	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী)
		যদি (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	ফেরত code;
}

স্থির পূর্ণসংখ্যা run_proc_receive_hook(কাঠামো command *commands,
				 ধ্রুবক কাঠামো string_list *ঠেলো_options)
{
	কাঠামো child_process proc = CHILD_PROCESS_INIT;
	কাঠামো async muxer;
	কাঠামো command *cmd;
	কাঠামো packet_reader reader;
	কাঠামো strbuf cap = STRBUF_INIT;
	কাঠামো strbuf errmsg = STRBUF_INIT;
	পূর্ণসংখ্যা hook_use_ঠেলো_options = 0;
	পূর্ণসংখ্যা version = 0;
	পূর্ণসংখ্যা code;
	ধ্রুবক অক্ষর *hook_path = find_hook(the_repository, "proc-receive");

	যদি (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		ফেরত -1;
	}

	strvec_ঠেলো(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	যদি (use_sideband) {
		memset(&muxer, 0, আকার(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		যদি (code)
			ফেরত code;
		proc.err = muxer.in;
	} অন্যথা {
		proc.err = 0;
	}

	code = start_command(&proc);
	যদি (code) {
		যদি (use_sideband)
			finish_async(&muxer);
		ফেরত code;
	}

	sigchain_ঠেলো(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, ফাঁকা, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	যদি (use_atomic)
		strbuf_addstr(&cap, " atomic");
	যদি (use_ঠেলো_options)
		strbuf_addstr(&cap, " push-options");
	যদি (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} অন্যথা {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	যদি (!code)
		code = packet_flush_gently(proc.in);

	যদি (!code)
		জন্য (;;) {
			পূর্ণসংখ্যা linelen;
			গণনা packet_read_status status;

			status = packet_reader_read(&reader);
			যদি (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				যদি (status == PACKET_READ_EOF)
					code = -1;
				বিরতি;
			}

			যদি (reader.pktlen > 8 && starts_with(reader.সারি, "version=")) {
				version = atoi(reader.সারি + 8);
				linelen = strlen(reader.সারি);
				যদি (linelen < reader.pktlen) {
					ধ্রুবক অক্ষর *feature_list = reader.সারি + linelen + 1;
					যদি (parse_feature_request(feature_list, "push-options"))
						hook_use_ঠেলো_options = 1;
				}
			}
		}

	যদি (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		যাও cleanup;
	}

	নির্বাচন (version) {
	ক্ষেত্র 0:
		/* fallthrough */
	ক্ষেত্র 1:
		বিরতি;
	পূর্বনির্ধারিত:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		যাও cleanup;
	}

	/* Send commands */
	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		যদি (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			চালু;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		যদি (code)
			বিরতি;
	}
	যদি (!code)
		code = packet_flush_gently(proc.in);
	যদি (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		যাও cleanup;
	}

	/* Send push options */
	যদি (hook_use_ঠেলো_options) {
		কাঠামো string_list_item *item;

		for_each_string_list_item(item, ঠেলো_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			যদি (code)
				বিরতি;
		}
		যদি (!code)
			code = packet_flush_gently(proc.in);
		যদি (code) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			যাও cleanup;
		}
	}

	/* Read result from proc-receive */
	code = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.in);
	close(proc.out);
	যদি (use_sideband)
		finish_async(&muxer);
	যদি (finish_command(&proc))
		code = -1;
	যদি (errmsg.len >0) {
		অক্ষর *p = errmsg.buf;

		p += errmsg.len - 1;
		যদি (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_বের_করো(SIGPIPE);

	ফেরত code;
}

স্থির ধ্রুবক অক্ষর *refuse_unconfigured_deny_বার্তা =
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

স্থির শূন্য refuse_unconfigured_deny(শূন্য)
{
	rp_error("%s", _(refuse_unconfigured_deny_বার্তা));
}

স্থির ধ্রুবক অক্ষর *refuse_unconfigured_deny_delete_current_বার্তা =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

স্থির শূন্য refuse_unconfigured_deny_delete_current(শূন্য)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_বার্তা));
}

স্থির ধ্রুবক কাঠামো object_id *command_singleton_iterator(শূন্য *cb_তথ্য);
স্থির পূর্ণসংখ্যা update_shallow_ref(কাঠামো command *cmd, কাঠামো shallow_info *si)
{
	কাঠামো shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	কাঠামো oid_array extra = OID_ARRAY_INIT;
	কাঠামো check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->সূচক % 32);
	পূর্ণসংখ্যা i;

	trace_printf_চাবি(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	জন্য (i = 0; i < si->shallow->nr; i++)
		যদি (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->সূচক / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	যদি (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		ফেরত -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	জন্য (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->সূচক] = 0;
	oid_array_clear(&extra);
	ফেরত 0;
}

স্থির ধ্রুবক অক্ষর *ঠেলো_to_deploy(unsigned অক্ষর *sha1,
				  কাঠামো strvec *env,
				  ধ্রুবক কাঠামো worktree *worktree)
{
	কাঠামো child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", ফাঁকা);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	যদি (run_command(&child))
		ফেরত "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", ফাঁকা);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	যদি (run_command(&child))
		ফেরত "Working directory has unstaged changes";

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
		     !is_null_oid(&worktree->শীর্ষ_oid) ? "HEAD" :
		     empty_বৃক্ষ_oid_hex(the_repository->hash_algo), "--", ফাঁকা);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	যদি (run_command(&child))
		ফেরত "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     ফাঁকা);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	যদি (run_command(&child))
		ফেরত "Could not update working tree to new HEAD";

	ফেরত ফাঁকা;
}

স্থির ধ্রুবক অক্ষর *ঠেলো_to_checkout_hook = "push-to-checkout";

স্থির ধ্রুবক অক্ষর *ঠেলো_to_checkout(unsigned অক্ষর *hash,
				    পূর্ণসংখ্যা *invoked_hook,
				    কাঠামো strvec *env,
				    ধ্রুবক অক্ষর *work_বৃক্ষ)
{
	কাঠামো run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_বৃক্ষ));
	strvec_ঠেলো(&opt.args, hash_to_hex(hash));
	যদি (run_hooks_opt(the_repository, ঠেলো_to_checkout_hook, &opt))
		ফেরত "push-to-checkout hook declined";
	অন্যথা
		ফেরত ফাঁকা;
}

স্থির ধ্রুবক অক্ষর *update_worktree(unsigned অক্ষর *sha1, ধ্রুবক কাঠামো worktree *worktree)
{
	ধ্রুবক অক্ষর *retval;
	অক্ষর *git_dir;
	কাঠামো strvec env = STRVEC_INIT;
	পূর্ণসংখ্যা invoked_hook;

	যদি (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	যদি (worktree->is_bare)
		ফেরত "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = ঠেলো_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	যদি (!invoked_hook)
		retval = ঠেলো_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	মুক্ত(git_dir);
	ফেরত retval;
}

স্থির ধ্রুবক অক্ষর *update(কাঠামো command *cmd, কাঠামো shallow_info *si)
{
	ধ্রুবক অক্ষর *name = cmd->ref_name;
	কাঠামো strbuf namespaced_name_buf = STRBUF_INIT;
	স্থির অক্ষর *namespaced_name;
	ধ্রুবক অক্ষর *ret;
	কাঠামো object_id *old_oid = &cmd->old_oid;
	কাঠামো object_id *new_oid = &cmd->new_oid;
	পূর্ণসংখ্যা do_update_worktree = 0;
	কাঠামো worktree **worktrees = get_worktrees(the_repository);
	ধ্রুবক কাঠামো worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	যদি (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		যাও out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	মুক্ত(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, ফাঁকা);

	যদি (worktree && !worktree->is_bare) {
		নির্বাচন (deny_current_branch) {
		ক্ষেত্র DENY_IGNORE:
			বিরতি;
		ক্ষেত্র DENY_WARN:
			rp_warning("updating the current branch");
			বিরতি;
		ক্ষেত্র DENY_REFUSE:
		ক্ষেত্র DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			যদি (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			যাও out;
		ক্ষেত্র DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			বিরতি;
		}
	}

	যদি (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		যাও out;
	}

	যদি (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		যদি (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			যাও out;
		}

		যদি (worktree || (শীর্ষ_name && !strcmp(namespaced_name, শীর্ষ_name))) {
			নির্বাচন (deny_delete_current) {
			ক্ষেত্র DENY_IGNORE:
				বিরতি;
			ক্ষেত্র DENY_WARN:
				rp_warning("deleting the current branch");
				বিরতি;
			ক্ষেত্র DENY_REFUSE:
			ক্ষেত্র DENY_UNCONFIGURED:
			ক্ষেত্র DENY_UPDATE_INSTEAD:
				যদি (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				যাও out;
			পূর্বনির্ধারিত:
				ret = "Invalid denyDeleteCurrent setting";
				যাও out;
			}
		}
	}

	যদি (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		কাঠামো object *old_object, *new_object;
		কাঠামো commit *old_commit, *new_commit;
		পূর্ণসংখ্যা ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		যদি (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			যাও out;
		}
		old_commit = (কাঠামো commit *)old_object;
		new_commit = (কাঠামো commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		যদি (ret2 < 0)
			exit(128);
		যদি (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			যাও out;
		}
	}
	যদি (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		যাও out;
	}

	যদি (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		যদি (ret)
			যাও out;
	}

	যদি (is_null_oid(new_oid)) {
		কাঠামো strbuf err = STRBUF_INIT;
		যদি (!parse_object(the_repository, old_oid)) {
			old_oid = ফাঁকা;
			যদি (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} অন্যথা {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		যদি (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   ফাঁকা, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} অন্যথা {
			ret = ফাঁকা; /* good */
		}
		strbuf_release(&err);
	} অন্যথা {
		গণনা ref_transaction_error tx_err;
		কাঠামো strbuf err = STRBUF_INIT;
		যদি (shallow_update && si->shallow_ref[cmd->সূচক] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			যাও out;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  ফাঁকা, ফাঁকা,
						  0, "push",
						  &err);
		যদি (tx_err) {
			rp_error("%s", err.buf);
			যদি (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			অন্যথা
				ret = ref_transaction_error_বার্তা(tx_err);
		} অন্যথা {
			ret = ফাঁকা; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	ফেরত ret;
}

স্থির শূন্য run_update_post_hook(কাঠামো command *commands)
{
	স্থির ধ্রুবক অক্ষর hook_name[] = "post-update";
	কাঠামো run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	কাঠামো async sideband_async;
	কাঠামো command *cmd;
	পূর্ণসংখ্যা sideband_async_started = 0;
	পূর্ণসংখ্যা saved_stderr = -1;

	যদি (!hook_exists(the_repository, hook_name))
		ফেরত;

	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		যদি (cmd->error_string || cmd->did_not_exist)
			চালু;
		strvec_ঠেলো(&opt.args, cmd->ref_name);
	}
	যদি (!opt.args.nr)
		ফেরত;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

স্থির শূন্য check_aliased_update_internal(কাঠামো command *cmd,
					  কাঠামো string_list *list,
					  ধ্রুবক অক্ষর *dst_name, পূর্ণসংখ্যা flag)
{
	কাঠামো string_list_item *item;
	কাঠামো command *dst_cmd;

	যদি (!(flag & REF_ISSYMREF))
		ফেরত;

	যদি (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		ফেরত;
	}
	dst_name = strip_namespace(dst_name);

	যদি (!(item = string_list_lookup(list, dst_name)))
		ফেরত;

	cmd->skip_update = 1;

	dst_cmd = (কাঠামো command *) item->util;

	যদি (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		ফেরত;

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

স্থির শূন্য check_aliased_update(কাঠামো command *cmd, কাঠামো string_list *list)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	ধ্রুবক অক্ষর *dst_name;
	পূর্ণসংখ্যা flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, ফাঁকা, &flag);
	check_aliased_update_internal(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

স্থির শূন্য check_aliased_updates(কাঠামো command *commands)
{
	কাঠামো command *cmd;
	কাঠামো string_list ref_list = STRING_LIST_INIT_NODUP;

	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		কাঠামো string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (শূন্য *)cmd;
	}
	string_list_sort(&ref_list);

	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		যদি (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

স্থির ধ্রুবক কাঠামো object_id *command_singleton_iterator(শূন্য *cb_তথ্য)
{
	কাঠামো command **cmd_list = cb_তথ্য;
	কাঠামো command *cmd = *cmd_list;

	যদি (!cmd || is_null_oid(&cmd->new_oid))
		ফেরত ফাঁকা;
	*cmd_list = ফাঁকা; /* this returns only one */
	ফেরত &cmd->new_oid;
}

স্থির শূন্য set_connectivity_errors(কাঠামো command *commands,
				    কাঠামো shallow_info *si,
				    কাঠামো odb_transaction *transaction)
{
	কাঠামো command *cmd;

	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		কাঠামো command *singleton = cmd;
		কাঠামো check_connected_options opt = CHECK_CONNECTED_INIT;
		কাঠামো strvec env = STRVEC_INIT;

		যদি (shallow_update && si->shallow_ref[cmd->সূচক])
			/* to be checked in update_shallow_ref() */
			চালু;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		যদি (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			চালু;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

কাঠামো iterate_তথ্য {
	কাঠামো command *cmds;
	কাঠামো shallow_info *si;
};

স্থির ধ্রুবক কাঠামো object_id *iterate_receive_command_list(শূন্য *cb_তথ্য)
{
	কাঠামো iterate_তথ্য *তথ্য = cb_তথ্য;
	কাঠামো command **cmd_list = &তথ্য->cmds;
	কাঠামো command *cmd = *cmd_list;

	জন্য (; cmd; cmd = cmd->পরবর্তী) {
		যদি (shallow_update && তথ্য->si->shallow_ref[cmd->সূচক])
			/* to be checked in update_shallow_ref() */
			চালু;
		যদি (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->পরবর্তী;
			ফেরত &cmd->new_oid;
		}
	}
	ফেরত ফাঁকা;
}

স্থির শূন্য reject_updates_to_hidden(কাঠামো command *commands)
{
	কাঠামো strbuf refname_full = STRBUF_INIT;
	আকার_t prefix_len;
	কাঠামো command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		যদি (cmd->error_string)
			চালু;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		যদি (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			চালু;
		যদি (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		অন্যথা
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

স্থির পূর্ণসংখ্যা should_process_cmd(কাঠামো command *cmd)
{
	ফেরত !cmd->error_string && !cmd->skip_update;
}

স্থির শূন্য BUG_if_skipped_connectivity_check(কাঠামো command *commands,
					       কাঠামো shallow_info *si)
{
	কাঠামো command *cmd;

	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		যদি (should_process_cmd(cmd) && si->shallow_ref[cmd->সূচক])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

স্থির শূন্য ref_transaction_rejection_handler(ধ্রুবক অক্ষর *refname,
					      ধ্রুবক কাঠামো object_id *old_oid UNUSED,
					      ধ্রুবক কাঠামো object_id *new_oid UNUSED,
					      ধ্রুবক অক্ষর *old_target UNUSED,
					      ধ্রুবক অক্ষর *new_target UNUSED,
					      গণনা ref_transaction_error err,
					      ধ্রুবক অক্ষর *details,
					      শূন্য *cb_তথ্য)
{
	কাঠামো strmap *failed_refs = cb_তথ্য;

	যদি (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (অক্ষর *)ref_transaction_error_বার্তা(err));
}

স্থির শূন্য execute_commands_non_atomic(কাঠামো command *commands,
					কাঠামো shallow_info *si)
{
	কাঠামো command *cmd;
	কাঠামো strbuf err = STRBUF_INIT;
	ধ্রুবক অক্ষর *reported_error = ফাঁকা;
	কাঠামো strmap failed_refs = STRMAP_INIT;

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
	গণনা processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	জন্য (গণনা processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
			যদি (!should_process_cmd(cmd) || cmd->run_proc_receive)
				চালু;

			যদি (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				চালু;
			অন্যথা যদি (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				চালু;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			যদি (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				যদি (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					যাও failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		যদি (!transaction)
			যাও cleanup;

		যদি (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			যাও failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		যদি (strmap_empty(&failed_refs))
			যাও cleanup;

	failure:
		জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
			যদি (reported_error)
				cmd->error_string = reported_error;
			অন্যথা যদি (strmap_contains(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = ফাঁকা;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

স্থির শূন্য execute_commands_atomic(কাঠামো command *commands,
					কাঠামো shallow_info *si)
{
	কাঠামো command *cmd;
	কাঠামো strbuf err = STRBUF_INIT;
	ধ্রুবক অক্ষর *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	যদি (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		যাও failure;
	}

	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		যদি (!should_process_cmd(cmd) || cmd->run_proc_receive)
			চালু;

		cmd->error_string = update(cmd, si);

		যদি (cmd->error_string)
			যাও failure;
	}

	যদি (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		যাও failure;
	}
	যাও cleanup;

failure:
	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী)
		যদি (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

স্থির শূন্য execute_commands(কাঠামো command *commands,
			     পূর্ণসংখ্যা unpacker_error,
			     কাঠামো shallow_info *si,
			     কাঠামো odb_transaction *transaction,
			     ধ্রুবক কাঠামো string_list *ঠেলো_options)
{
	কাঠামো check_connected_options opt = CHECK_CONNECTED_INIT;
	কাঠামো command *cmd;
	কাঠামো iterate_তথ্য তথ্য;
	কাঠামো async muxer;
	পূর্ণসংখ্যা err_fd = 0;
	পূর্ণসংখ্যা run_proc_receive = 0;

	যদি (unpacker_error) {
		জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী)
			cmd->error_string = "unpacker error";
		ফেরত;
	}

	যদি (!skip_connectivity_check) {
		কাঠামো strvec env = STRVEC_INIT;

		যদি (use_sideband) {
			memset(&muxer, 0, আকার(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			যদি (!start_async(&muxer))
				err_fd = muxer.in;
			/* ...else, continue without relaying sideband */
		}

		তথ্য.cmds = commands;
		তথ্য.si = si;
		opt.err_fd = err_fd;
		opt.progress = err_fd && !quiet;
		odb_transaction_env(transaction, &env);
		opt.env = env.v;
		opt.exclude_hidden_refs_section = "receive";

		যদি (check_connected(iterate_receive_command_list, &তথ্য, &opt))
			set_connectivity_errors(commands, si, transaction);

		যদি (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	যদি (proc_receive_ref) {
		জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
			যদি (!should_process_cmd(cmd))
				চালু;

			যদি (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	যদি (run_receive_hook(commands, "pre-receive", 0, transaction, ঠেলো_options)) {
		জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
			যদি (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		ফেরত;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	জন্য (cmd = commands; cmd && cmd->error_string; cmd = cmd->পরবর্তী)
		; /* nothing */
	যদি (!cmd)
		ফেরত;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	যদি (odb_transaction_commit(transaction)) {
		জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
			যদি (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		ফেরত;
	}

	check_aliased_updates(commands);

	মুক্ত(শীর্ষ_name_to_free);
	শীর্ষ_name = শীর্ষ_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, ফাঁকা,
							    ফাঁকা);

	যদি (run_proc_receive &&
	    run_proc_receive_hook(commands, ঠেলো_options))
		জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী)
			যদি (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	যদি (use_atomic)
		execute_commands_atomic(commands, si);
	অন্যথা
		execute_commands_non_atomic(commands, si);

	যদি (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

স্থির কাঠামো command **queue_command(কাঠামো command **লেজ,
				      ধ্রুবক অক্ষর *সারি,
				      পূর্ণসংখ্যা linelen)
{
	কাঠামো object_id old_oid, new_oid;
	কাঠামো command *cmd;
	ধ্রুবক অক্ষর *refname;
	পূর্ণসংখ্যা reflen;
	ধ্রুবক অক্ষর *p;

	যদি (parse_oid_hex(সারি, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", সারি);

	refname = p;
	reflen = linelen - (p - সারি);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*লেজ = cmd;
	ফেরত &cmd->পরবর্তী;
}

স্থির শূন্য free_commands(কাঠামো command *commands)
{
	যখন (commands) {
		কাঠামো command *পরবর্তী = commands->পরবর্তী;

		ref_ঠেলো_report_free(commands->report);
		মুক্ত(commands->error_string_owned);
		মুক্ত(commands);
		commands = পরবর্তী;
	}
}

স্থির শূন্য queue_commands_from_cert(কাঠামো command **লেজ,
				     কাঠামো strbuf *ঠেলো_cert)
{
	ধ্রুবক অক্ষর *boc, *eoc;

	যদি (*লেজ)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(ঠেলো_cert->buf, "\n\n");
	যদি (!boc)
		die("malformed push certificate %.*s", 100, ঠেলো_cert->buf);
	অন্যথা
		boc += 2;
	eoc = ঠেলো_cert->buf + parse_signed_বাফার(ঠেলো_cert->buf, ঠেলো_cert->len);

	যখন (boc < eoc) {
		ধ্রুবক অক্ষর *eol = memchr(boc, '\n', eoc - boc);
		লেজ = queue_command(লেজ, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

স্থির কাঠামো command *read_শীর্ষ_info(কাঠামো packet_reader *reader,
				      কাঠামো oid_array *shallow,
				      গণনা report_status_version *version)
{
	কাঠামো command *commands = ফাঁকা;
	কাঠামো command **p = &commands;
	জন্য (;;) {
		পূর্ণসংখ্যা linelen;

		যদি (packet_reader_read(reader) != PACKET_READ_NORMAL)
			বিরতি;

		যদি (reader->pktlen > 8 && starts_with(reader->সারি, "shallow ")) {
			কাঠামো object_id oid;
			যদি (get_oid_hex(reader->সারি + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->সারি + 8);
			oid_array_append(shallow, &oid);
			চালু;
		}

		linelen = strlen(reader->সারি);
		যদি (linelen < reader->pktlen) {
			ধ্রুবক অক্ষর *feature_list = reader->সারি + linelen + 1;
			ধ্রুবক অক্ষর *hash = ফাঁকা;
			ধ্রুবক অক্ষর *client_sid;
			আকার_t len = 0;
			যদি (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			যদি (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			যদি (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			যদি (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			যদি (advertise_atomic_ঠেলো
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			যদি (advertise_ঠেলো_options
			    && parse_feature_request(feature_list, "push-options"))
				use_ঠেলো_options = 1;
			hash = parse_feature_মান(feature_list, "object-format", &len, ফাঁকা);
			যদি (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			যদি (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_মান(feature_list, "session-id", &len, ফাঁকা);
			যদি (client_sid) {
				অক্ষর *sid = xstrndup(client_sid, len);
				trace2_তথ্য_string("transfer", ফাঁকা, "client-sid", client_sid);
				মুক্ত(sid);
			}
		}

		যদি (!strcmp(reader->সারি, "push-cert")) {
			পূর্ণসংখ্যা true_flush = 0;
			পূর্ণসংখ্যা saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			জন্য (;;) {
				packet_reader_read(reader);
				যদি (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					বিরতি;
				}
				যদি (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				যদি (!strcmp(reader->সারি, "push-cert-end\n"))
					বিরতি; /* end of cert */
				strbuf_addstr(&ঠেলো_cert, reader->সারি);
			}
			reader->options = saved_options;

			যদি (true_flush)
				বিরতি;
			চালু;
		}

		p = queue_command(p, reader->সারি, linelen);
	}

	যদি (ঠেলো_cert.len)
		queue_commands_from_cert(p, &ঠেলো_cert);

	ফেরত commands;
}

স্থির শূন্য read_ঠেলো_options(কাঠামো packet_reader *reader,
			      কাঠামো string_list *options)
{
	যখন (1) {
		যদি (packet_reader_read(reader) != PACKET_READ_NORMAL)
			বিরতি;

		string_list_append(options, reader->সারি);
	}
}

স্থির পূর্ণসংখ্যা unpack_with_sideband(কাঠামো odb_transaction *transaction,
				ধ্রুবক অক্ষর *shallow_file,
				কাঠামো strbuf *err_বার্তা)
{
	কাঠামো odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_বার্তা_types = fsck_বার্তা_types.buf,
		.max_প্রবেশ_আকার = max_প্রবেশ_আকার,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	কাঠামো async muxer;
	পূর্ণসংখ্যা ret;

	যদি (!use_sideband)
		ফেরত odb_transaction_write_pack(transaction, 0, err_বার্তা, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, আকার(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	যদি (start_async(&muxer))
		ফেরত 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_বার্তা, &opts);

	finish_async(&muxer);
	ফেরত ret;
}

স্থির শূন্য prepare_shallow_update(কাঠামো shallow_info *si,
				   ধ্রুবক অক্ষর *shallow_file)
{
	পূর্ণসংখ্যা i, j, k, bitmap_আকার = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, ফাঁকা);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	জন্য (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	জন্য (i = 0; i < si->shallow->nr; i++) {
		যদি (!si->used_shallow[i])
			চালু;
		জন্য (j = 0; j < bitmap_আকার; j++) {
			যদি (!si->used_shallow[i][j])
				চালু;
			si->need_reachability_test[i]++;
			জন্য (k = 0; k < 32; k++)
				যদি (si->used_shallow[i][j] & (1U << k))
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

স্থির শূন্য update_shallow_info(কাঠামো command *commands,
				কাঠামো shallow_info *si,
				কাঠামো oid_array *ref,
				ধ্রুবক অক্ষর *shallow_file)
{
	কাঠামো command *cmd;
	পূর্ণসংখ্যা *ref_status;
	remove_nonexistent_theirs_shallow(si);
	যদি (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		ফেরত;
	}

	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		যদি (is_null_oid(&cmd->new_oid))
			চালু;
		oid_array_append(ref, &cmd->new_oid);
		cmd->সূচক = ref->nr - 1;
	}
	si->ref = ref;

	যদি (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		ফেরত;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, ফাঁকা, ref_status);
	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		যদি (is_null_oid(&cmd->new_oid))
			চালু;
		যদি (ref_status[cmd->সূচক]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	মুক্ত(ref_status);
}

স্থির শূন্য override_cmds_error(কাঠামো command *commands, ধ্রুবক অক্ষর *err)
{
	জন্য (কাঠামো command *cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
স্থির শূন্য generate_report(কাঠামো strbuf *buf, কাঠামো command *commands,
			    ধ্রুবক কাঠামো strbuf *unpack_status,
			    গণনা report_status_version version)
{
	কাঠামো command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		কাঠামো ref_ঠেলো_report *report;
		পূর্ণসংখ্যা গুনতি = 0;

		যদি (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		অন্যথা
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		যদি (version != REPORT_STATUS_V2 || cmd->error_string)
			চালু;

		জন্য (report = cmd->report; report; report = report->পরবর্তী) {
			যদি (গুনতি++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			যদি (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			যদি (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			যদি (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			যদি (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

স্থির শূন্য report(কাঠামো command *commands, ধ্রুবক কাঠামো strbuf *unpack_status,
		   গণনা report_status_version version)
{
	কাঠামো strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	যদি (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	যদি (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	অন্যথা
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

স্থির পূর্ণসংখ্যা delete_only(কাঠামো command *commands)
{
	কাঠামো command *cmd;
	জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী) {
		যদি (!is_null_oid(&cmd->new_oid))
			ফেরত 0;
	}
	ফেরত 1;
}

পূর্ণসংখ্যা cmd_receive_pack(পূর্ণসংখ্যা argc,
		     ধ্রুবক অক্ষর **argv,
		     ধ্রুবক অক্ষর *prefix,
		     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা advertise_refs = 0;
	কাঠামো command *commands;
	কাঠামো oid_array shallow = OID_ARRAY_INIT;
	কাঠামো oid_array ref = OID_ARRAY_INIT;
	কাঠামো shallow_info si;
	কাঠামো packet_reader reader;
	কাঠামো odb_transaction *transaction = ফাঁকা;
	গণনা report_status_version version = REPORT_STATUS_UNKNOWN;

	কাঠামো option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, ফাঁকা),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, ফাঁকা),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, ফাঁকা),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, ফাঁকা),
		OPT_END()
	};

	packet_trace_identity("receive-pack");

	argc = parse_options(argc, argv, prefix, options, receive_pack_usage, 0);

	যদি (argc > 1)
		usage_বার্তা_opt(_("too many arguments"), receive_pack_usage, options);
	যদি (argc == 0)
		usage_বার্তা_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	যদি (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, ফাঁকা);
	যদি (cert_nonce_seed)
		ঠেলো_cert_nonce = prepare_ঠেলো_cert_nonce(service_dir, time(ফাঁকা));

	নির্বাচন (determine_protocol_version_server()) {
	ক্ষেত্র protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		বিরতি;
	ক্ষেত্র protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		যদি (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	ক্ষেত্র protocol_v0:
		বিরতি;
	ক্ষেত্র protocol_unknown_version:
		BUG("unknown protocol version");
	}

	যদি (advertise_refs || !stateless_rpc) {
		write_শীর্ষ_info();
	}
	যদি (advertise_refs)
		ফেরত 0;

	packet_reader_init(&reader, 0, ফাঁকা, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	যদি ((commands = read_শীর্ষ_info(&reader, &shallow, &version))) {
		কাঠামো string_list ঠেলো_options = STRING_LIST_INIT_DUP;
		কাঠামো strbuf unpack_status = STRBUF_INIT;

		যদি (use_ঠেলো_options)
			read_ঠেলো_options(&reader, &ঠেলো_options);
		যদি (!check_cert_ঠেলো_options(&ঠেলো_options)) {
			কাঠামো command *cmd;
			জন্য (cmd = commands; cmd; cmd = cmd->পরবর্তী)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		যদি (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		যদি (!delete_only(commands)) {
			ধ্রুবক অক্ষর *alt_shallow_file = ফাঁকা;

			যদি (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			যদি (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			অন্যথা
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &ঠেলো_options);
		odb_transaction_finalize(transaction);
		sigchain_ঠেলো(SIGPIPE, SIG_IGN);

		নির্বাচন (version) {
		ক্ষেত্র REPORT_STATUS_V2:
		ক্ষেত্র REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			বিরতি;
		ক্ষেত্র REPORT_STATUS_UNKNOWN:
			বিরতি;
		}

		sigchain_বের_করো(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, ফাঁকা,
				 &ঠেলো_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&ঠেলো_options, 0);
		যদি (auto_gc) {
			কাঠামো child_process proc = CHILD_PROCESS_INIT;

			যদি (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				যদি (!start_command(&proc)) {
					যদি (use_sideband)
						copy_to_sideband(proc.err, -1, ফাঁকা);
					finish_command(&proc);
				}
			}
		}
		যদি (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	যদি (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	মুক্ত((শূন্য *)ঠেলো_cert_nonce);
	ফেরত 0;
}
