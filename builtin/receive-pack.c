#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "connect.h"
#அந்தர்பாவ "connected.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "exec-cmd.h"
#அந்தர்பாவ "fsck.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "gpg-interface.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "hook.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "oid-array.h"
#அந்தர்பாவ "oidset.h"
#அந்தர்பாவ "pack.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "pkt-line.h"
#அந்தர்பாவ "protocol.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "server-info.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "shallow.h"
#அந்தர்பாவ "sideband.h"
#அந்தர்பாவ "sigchain.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "trace.h"
#அந்தர்பாவ "trace2.h"
#அந்தர்பாவ "version.h"
#அந்தர்பாவ "worktree.h"

நிலையான மாறிலி எழுத்து * மாறிலி receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	வெற்று
};

எண்ணிக்கை deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

எண்ணிக்கை report_status_version {
	REPORT_STATUS_UNKNOWN = 0,
	REPORT_STATUS_V0,
	REPORT_STATUS_V2,
};

நிலையான முழு deny_deletes;
நிலையான முழு deny_non_fast_forwards;
நிலையான எண்ணிக்கை deny_action deny_current_branch = DENY_UNCONFIGURED;
நிலையான எண்ணிக்கை deny_action deny_delete_current = DENY_UNCONFIGURED;
நிலையான முழு receive_fsck_objects = -1;
நிலையான முழு transfer_fsck_objects = -1;
நிலையான கட்டமைப்பு strbuf fsck_செய்தி_types = STRBUF_INIT;
நிலையான முழு advertise_atomic_தள்ளு = 1;
நிலையான முழு advertise_தள்ளு_options;
நிலையான முழு advertise_sid;
நிலையான off_t max_உள்ளீடு_size;
நிலையான முழு use_sideband;
நிலையான முழு use_atomic;
நிலையான முழு use_தள்ளு_options;
நிலையான முழு quiet;
நிலையான முழு prefer_ofs_delta = 1;
நிலையான முழு auto_update_server_info;
நிலையான முழு auto_gc = 1;
நிலையான முழு reject_thin;
நிலையான முழு skip_connectivity_check;
நிலையான முழு stateless_rpc;
நிலையான மாறிலி எழுத்து *service_dir;
நிலையான மாறிலி எழுத்து *head_name;
நிலையான வெறுமை *head_name_to_free;
நிலையான முழு sent_capabilities;
நிலையான முழு shallow_update;
நிலையான கட்டமைப்பு strbuf தள்ளு_cert = STRBUF_INIT;
நிலையான கட்டமைப்பு object_id தள்ளு_cert_oid;
நிலையான கட்டமைப்பு signature_check sigcheck;
நிலையான மாறிலி எழுத்து *தள்ளு_cert_nonce;
நிலையான எழுத்து *cert_nonce_seed;
நிலையான கட்டமைப்பு strvec hidden_refs = STRVEC_INIT;

நிலையான மாறிலி எழுத்து *NONCE_UNSOLICITED = "UNSOLICITED";
நிலையான மாறிலி எழுத்து *NONCE_BAD = "BAD";
நிலையான மாறிலி எழுத்து *NONCE_MISSING = "MISSING";
நிலையான மாறிலி எழுத்து *NONCE_OK = "OK";
நிலையான மாறிலி எழுத்து *NONCE_SLOP = "SLOP";
நிலையான மாறிலி எழுத்து *nonce_status;
நிலையான long nonce_stamp_slop;
நிலையான timestamp_t nonce_stamp_slop_limit;
நிலையான கட்டமைப்பு ref_transaction *transaction;

நிலையான எண்ணிக்கை {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
நிலையான முழு keepalive_in_sec = 5;

நிலையான கட்டமைப்பு proc_receive_ref {
	unsigned முழு want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	எழுத்து *ref_prefix;
	கட்டமைப்பு proc_receive_ref *next;
} *proc_receive_ref;

நிலையான வெறுமை proc_receive_ref_append(மாறிலி எழுத்து *prefix);

நிலையான எண்ணிக்கை deny_action parse_deny_action(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு)
{
	எனில் (மதிப்பு) {
		எனில் (!strcasecmp(மதிப்பு, "ignore"))
			திரும்பு DENY_IGNORE;
		எனில் (!strcasecmp(மதிப்பு, "warn"))
			திரும்பு DENY_WARN;
		எனில் (!strcasecmp(மதிப்பு, "refuse"))
			திரும்பு DENY_REFUSE;
		எனில் (!strcasecmp(மதிப்பு, "updateinstead"))
			திரும்பு DENY_UPDATE_INSTEAD;
	}
	எனில் (git_config_bool(var, மதிப்பு))
		திரும்பு DENY_REFUSE;
	திரும்பு DENY_IGNORE;
}

நிலையான முழு receive_pack_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			       மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	மாறிலி எழுத்து *செய்தி_id;
	முழு status = parse_hide_refs_config(var, மதிப்பு, "receive", &hidden_refs);

	எனில் (status)
		திரும்பு status;

	எனில் (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.fsck.skiplist") == 0) {
		எழுத்து *path;

		எனில் (git_config_pathname(&path, var, மதிப்பு))
			திரும்பு -1;
		எனில் (path)
			strbuf_addf(&fsck_செய்தி_types, "%cskiplist=%s",
				    fsck_செய்தி_types.len ? ',' : '=', path);
		விடுவி(path);
		திரும்பு 0;
	}

	எனில் (skip_prefix(var, "receive.fsck.", &செய்தி_id)) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		எனில் (is_valid_செய்தி_type(செய்தி_id, மதிப்பு))
			strbuf_addf(&fsck_செய்தி_types, "%c%s=%s",
				fsck_செய்தி_types.len ? ',' : '=', செய்தி_id, மதிப்பு);
		இல்லையெனில்
			warning("skipping unknown msg id '%s'", செய்தி_id);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.certnonceseed") == 0)
		திரும்பு git_config_string(&cert_nonce_seed, var, மதிப்பு);

	எனில் (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, மதிப்பு, ctx->kvi);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_தள்ளு = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_தள்ளு_options = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, மதிப்பு, ctx->kvi);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.maxinputsize") == 0) {
		max_உள்ளீடு_size = git_config_int64(var, மதிப்பு, ctx->kvi);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "receive.procreceiverefs") == 0) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		proc_receive_ref_append(மதிப்பு);
		திரும்பு 0;
	}

	எனில் (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

நிலையான வெறுமை show_ref(மாறிலி எழுத்து *path, மாறிலி கட்டமைப்பு object_id *oid)
{
	எனில் (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} இல்லையெனில் {
		கட்டமைப்பு strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		எனில் (advertise_atomic_தள்ளு)
			strbuf_addstr(&cap, " atomic");
		எனில் (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		எனில் (தள்ளு_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", தள்ளு_cert_nonce);
		எனில் (advertise_தள்ளு_options)
			strbuf_addstr(&cap, " push-options");
		எனில் (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

நிலையான முழு show_ref_cb(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *data)
{
	கட்டமைப்பு oidset *seen = data;
	மாறிலி எழுத்து *path = strip_namespace(ref->name);

	எனில் (ref_is_hidden(path, ref->name, &hidden_refs))
		திரும்பு 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	எனில் (!path) {
		எனில் (oidset_செருகு(seen, ref->oid))
			திரும்பு 0;
		path = ".have";
	} இல்லையெனில் {
		oidset_செருகு(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	திரும்பு 0;
}

நிலையான வெறுமை show_one_alternate_ref(மாறிலி கட்டமைப்பு object_id *oid,
				   வெறுமை *data)
{
	கட்டமைப்பு oidset *seen = data;

	எனில் (oidset_செருகு(seen, oid))
		திரும்பு;

	show_ref(".have", oid);
}

நிலையான வெறுமை write_head_info(வெறுமை)
{
	கட்டமைப்பு refs_for_each_ref_options opts = { 0 };
	நிலையான கட்டமைப்பு oidset seen = OIDSET_INIT;
	கட்டமைப்பு strvec excludes_vector = STRVEC_INIT;

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

	எனில் (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#define RUN_PROC_RECEIVE_SCHEDULED	1
#define RUN_PROC_RECEIVE_RETURNED	2
கட்டமைப்பு command {
	கட்டமைப்பு command *next;
	மாறிலி எழுத்து *error_string;
	எழுத்து *error_string_owned;
	கட்டமைப்பு ref_தள்ளு_report *report;
	unsigned முழு skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	முழு index;
	கட்டமைப்பு object_id old_oid;
	கட்டமைப்பு object_id new_oid;
	எழுத்து ref_name[FLEX_ARRAY]; /* more */
};

நிலையான வெறுமை proc_receive_ref_append(மாறிலி எழுத்து *prefix)
{
	கட்டமைப்பு proc_receive_ref *ref_pattern;
	மாறிலி எழுத்து *p;
	முழு len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = strchr(prefix, ':');
	எனில் (p) {
		போது (prefix < p) {
			எனில் (*prefix == 'a')
				ref_pattern->want_add = 1;
			இல்லையெனில் எனில் (*prefix == 'd')
				ref_pattern->want_delete = 1;
			இல்லையெனில் எனில் (*prefix == 'm')
				ref_pattern->want_modify = 1;
			இல்லையெனில் எனில் (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} இல்லையெனில் {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	போது (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	எனில் (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} இல்லையெனில் {
		கட்டமைப்பு proc_receive_ref *end;

		end = proc_receive_ref;
		போது (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

நிலையான முழு proc_receive_ref_matches(கட்டமைப்பு command *cmd)
{
	கட்டமைப்பு proc_receive_ref *p;

	எனில் (!proc_receive_ref)
		திரும்பு 0;

	க்கு (p = proc_receive_ref; p; p = p->next) {
		மாறிலி எழுத்து *match = p->ref_prefix;
		மாறிலி எழுத்து *remains;

		எனில் (!p->want_add && is_null_oid(&cmd->old_oid))
			தொடர்;
		இல்லையெனில் எனில் (!p->want_delete && is_null_oid(&cmd->new_oid))
			தொடர்;
		இல்லையெனில் எனில் (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			தொடர்;

		எனில் (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			எனில் (!p->negative_ref)
				திரும்பு 1;
		} இல்லையெனில் எனில் (p->negative_ref) {
			திரும்பு 1;
		}
	}
	திரும்பு 0;
}

நிலையான வெறுமை report_message(மாறிலி எழுத்து *prefix, மாறிலி எழுத்து *err, va_list params)
{
	முழு sz;
	எழுத்து செய்தி[4096];

	sz = xsnprintf(செய்தி, அளவு(செய்தி), "%s", prefix);
	sz += vsnprintf(செய்தி + sz, அளவு(செய்தி) - sz, err, params);
	எனில் (sz > (அளவு(செய்தி) - 1))
		sz = அளவு(செய்தி) - 1;
	செய்தி[sz++] = '\n';

	எனில் (use_sideband)
		send_sideband(1, 2, செய்தி, sz, use_sideband);
	இல்லையெனில்
		xwrite(2, செய்தி, sz);
}

__attribute__((format (printf, 1, 2)))
நிலையான வெறுமை rp_warning(மாறிலி எழுத்து *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("warning: ", err, params);
	va_end(params);
}

__attribute__((format (printf, 1, 2)))
நிலையான வெறுமை rp_error(மாறிலி எழுத்து *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("error: ", err, params);
	va_end(params);
}

நிலையான முழு copy_to_sideband(முழு in, முழு out UNUSED, வெறுமை *arg UNUSED)
{
	எழுத்து data[128];
	முழு keepalive_active = 0;

	எனில் (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	எனில் (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	போது (1) {
		ssize_t sz;

		எனில் (keepalive_active) {
			கட்டமைப்பு pollfd pfd;
			முழு ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			எனில் (ret < 0) {
				எனில் (errno == EINTR)
					தொடர்;
				இல்லையெனில்
					நிறுத்து;
			} இல்லையெனில் எனில் (ret == 0) {
				/* no data; send a keepalive packet */
				நிலையான மாறிலி எழுத்து buf[] = "0005\1";
				write_or_die(1, buf, அளவு(buf) - 1);
				தொடர்;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, அளவு(data));
		எனில் (sz <= 0)
			நிறுத்து;

		எனில் (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			மாறிலி எழுத்து *p = memchr(data, '\0', sz);
			எனில் (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_active = 1;
				send_sideband(1, 2, data, p - data, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - data + 1), use_sideband);
				தொடர்;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, data, sz, use_sideband);
	}
	close(in);
	திரும்பு 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
நிலையான வெறுமை prepare_sideband_async(கட்டமைப்பு async *sideband_async, முழு *saved_stderr, முழு *started)
{
	*started = 0;

	எனில் (!use_sideband)
		திரும்பு;

	memset(sideband_async, 0, அளவு(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	எனில் (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		எனில் (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
நிலையான வெறுமை finish_sideband_async(கட்டமைப்பு async *sideband_async, முழு saved_stderr, முழு started)
{
	எனில் (!use_sideband)
		திரும்பு;

	எனில் (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	எனில் (started)
		finish_async(sideband_async);
}

நிலையான வெறுமை hmac_hash(unsigned எழுத்து *out,
		      மாறிலி எழுத்து *சாவி_in, size_t சாவி_len,
		      மாறிலி எழுத்து *text, size_t text_len)
{
	unsigned எழுத்து சாவி[GIT_MAX_BLKSZ];
	unsigned எழுத்து k_ipad[GIT_MAX_BLKSZ];
	unsigned எழுத்து k_opad[GIT_MAX_BLKSZ];
	முழு i;
	கட்டமைப்பு git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(சாவி, '\0', GIT_MAX_BLKSZ);
	எனில் (the_hash_algo->blksz < சாவி_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, சாவி_in, சாவி_len);
		git_hash_final(சாவி, &ctx);
	} இல்லையெனில் {
		memcpy(சாவி, சாவி_in, சாவி_len);
	}

	/* RFC 2104 2. (2) & (5) */
	க்கு (i = 0; i < அளவு(சாவி); i++) {
		k_ipad[i] = சாவி[i] ^ 0x36;
		k_opad[i] = சாவி[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, அளவு(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, அளவு(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

நிலையான எழுத்து *prepare_தள்ளு_cert_nonce(மாறிலி எழுத்து *path, timestamp_t stamp)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	unsigned எழுத்து hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, strlen(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (முழு)the_hash_algo->hexsz, hash_to_hex(hash));
	திரும்பு strbuf_detach(&buf, வெற்று);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
நிலையான முழு constant_memequal(மாறிலி எழுத்து *a, மாறிலி எழுத்து *b, size_t n)
{
	முழு res = 0;
	size_t i;

	க்கு (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	திரும்பு res;
}

நிலையான மாறிலி எழுத்து *check_nonce(மாறிலி எழுத்து *buf)
{
	size_t noncelen;
	மாறிலி எழுத்து *found = find_commit_header(buf, "nonce", &noncelen);
	எழுத்து *nonce = found ? xmemdupz(found, noncelen) : வெற்று;
	timestamp_t stamp, ostamp;
	எழுத்து *bohmac, *expect = வெற்று;
	மாறிலி எழுத்து *retval = NONCE_BAD;

	எனில் (!nonce) {
		retval = NONCE_MISSING;
		செல் leave;
	} இல்லையெனில் எனில் (!தள்ளு_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		செல் leave;
	} இல்லையெனில் எனில் (!strcmp(தள்ளு_cert_nonce, nonce)) {
		retval = NONCE_OK;
		செல் leave;
	}

	எனில் (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		செல் leave;
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
	எனில் (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		செல் leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	எனில் (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		செல் leave;
	}

	expect = prepare_தள்ளு_cert_nonce(service_dir, stamp);
	எனில் (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		செல் leave;
	}
	எனில் (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		செல் leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(தள்ளு_cert_nonce, வெற்று, 10);
	nonce_stamp_slop = (long)ostamp - (long)stamp;

	எனில் (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		விடுவி((வெறுமை *)தள்ளு_cert_nonce);
		தள்ளு_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} இல்லையெனில் {
		retval = NONCE_SLOP;
	}

leave:
	விடுவி(nonce);
	விடுவி(expect);
	திரும்பு retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
நிலையான முழு check_cert_தள்ளு_options(மாறிலி கட்டமைப்பு string_list *தள்ளு_options)
{
	மாறிலி எழுத்து *buf = தள்ளு_cert.buf;

	மாறிலி எழுத்து *option;
	size_t optionlen;
	முழு options_seen = 0;

	முழு retval = 1;

	எனில் (!*buf)
		திரும்பு 1;

	போது ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		எனில் (options_seen > தள்ளு_options->nr
		    || xstrncmpz(தள்ளு_options->பொருட்கள்[options_seen - 1].string,
				 option, optionlen))
			திரும்பு 0;
	}

	எனில் (options_seen != தள்ளு_options->nr)
		retval = 0;

	திரும்பு retval;
}

நிலையான வெறுமை prepare_தள்ளு_cert_sha1(கட்டமைப்பு run_hooks_opt *opt)
{
	நிலையான முழு already_done;

	எனில் (!தள்ளு_cert.len)
		திரும்பு;

	எனில் (!already_done) {
		முழு bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		எனில் (odb_write_object(the_repository->objects, தள்ளு_cert.buf,
				     தள்ளு_cert.len, OBJ_BLOB, &தள்ளு_cert_oid))
			oidclr(&தள்ளு_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', அளவு(sigcheck));

		bogs = parse_signed_buffer(தள்ளு_cert.buf, தள்ளு_cert.len);
		sigcheck.payload = xmemdupz(தள்ளு_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, தள்ளு_cert.buf + bogs,
				தள்ளு_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	எனில் (!is_null_oid(&தள்ளு_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&தள்ளு_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.சாவி ? sigcheck.சாவி : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		எனில் (தள்ளு_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     தள்ளு_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			எனில் (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

கட்டமைப்பு receive_hook_feed_state {
	கட்டமைப்பு command *cmd;
	கட்டமைப்பு ref_தள்ளு_report *report;
	முழு skip_broken;
	கட்டமைப்பு strbuf buf;
};

நிலையான முழு feed_receive_hook_cb(முழு hook_stdin_fd, வெறுமை *pp_cb UNUSED, வெறுமை *pp_task_cb)
{
	கட்டமைப்பு receive_hook_feed_state *state = pp_task_cb;
	கட்டமைப்பு command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	போது (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	எனில் (!cmd)
		திரும்பு 1;  /* no more commands left */

	எனில் (!state->report)
		state->report = cmd->report;

	எனில் (state->report) {
		கட்டமைப்பு object_id *old_oid;
		கட்டமைப்பு object_id *new_oid;
		மாறிலி எழுத்து *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->next;
		எனில் (!state->report)
			cmd = cmd->next;
	} இல்லையெனில் {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	எனில் (state->buf.len > 0) {
		முழு ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		எனில் (ret < 0) {
			எனில் (errno == EPIPE)
				திரும்பு 1; /* child closed pipe */
			திரும்பு ret;
		}
	}

	திரும்பு state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

நிலையான வெறுமை *receive_hook_feed_state_alloc(வெறுமை *feed_pipe_ctx)
{
	கட்டமைப்பு receive_hook_feed_state *init_state = feed_pipe_ctx;
	கட்டமைப்பு receive_hook_feed_state *data;

	CALLOC_ARRAY(data, 1);
	data->report = init_state->report;
	data->cmd = init_state->cmd;
	data->skip_broken = init_state->skip_broken;
	strbuf_init(&data->buf, 0);

	திரும்பு data;
}

நிலையான வெறுமை receive_hook_feed_state_free(வெறுமை *data)
{
	கட்டமைப்பு receive_hook_feed_state *d = data;
	எனில் (!d)
		திரும்பு;
	strbuf_release(&d->buf);
	விடுவி(d);
}

நிலையான முழு run_receive_hook(கட்டமைப்பு command *commands,
			    மாறிலி எழுத்து *hook_name,
			    முழு skip_broken,
			    கட்டமைப்பு odb_transaction *transaction,
			    மாறிலி கட்டமைப்பு string_list *தள்ளு_options)
{
	கட்டமைப்பு run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	கட்டமைப்பு command *iter = commands;
	கட்டமைப்பு receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	கட்டமைப்பு async sideband_async;
	முழு sideband_async_started = 0;
	முழு saved_stderr = -1;
	முழு ret;

	எனில் (!hook_exists(the_repository, hook_name))
		திரும்பு 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	போது (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	எனில் (!iter)
		திரும்பு 0;

	எனில் (தள்ளு_options) {
		க்கு (முழு i = 0; i < தள்ளு_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     தள்ளு_options->பொருட்கள்[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)தள்ளு_options->nr);
	} இல்லையெனில் {
		strvec_தள்ளு(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	எனில் (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_தள்ளு_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_data_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_data_free = receive_hook_feed_state_free;

	ret = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	திரும்பு ret;
}

நிலையான முழு run_update_hook(கட்டமைப்பு command *cmd)
{
	நிலையான மாறிலி எழுத்து hook_name[] = "update";
	கட்டமைப்பு run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	கட்டமைப்பு async sideband_async;
	முழு sideband_async_started = 0;
	முழு saved_stderr = -1;
	முழு code;

	எனில் (!hook_exists(the_repository, hook_name))
		திரும்பு 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     வெற்று);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	திரும்பு code;
}

நிலையான முழு run_receive_report_hook(கட்டமைப்பு strbuf *report)
{
	கட்டமைப்பு child_process proc = CHILD_PROCESS_INIT;
	கட்டமைப்பு async sideband_async;
	முழு sideband_async_started = 0;
	முழு saved_stderr = -1;
	கட்டமைப்பு strbuf out = STRBUF_INIT;
	மாறிலி எழுத்து *hook_path;
	முழு ret;

	hook_path = find_hook(the_repository, "receive-report");
	எனில் (!hook_path)
		திரும்பு 0;

	strvec_தள்ளு(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_தள்ளு(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, வெற்று, 0);
	sigchain_எடு(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	எனில் (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	திரும்பு ret;
}

நிலையான கட்டமைப்பு command *find_command_by_refname(கட்டமைப்பு command *list,
					       மாறிலி எழுத்து *refname)
{
	க்கு (; list; list = list->next)
		எனில் (!strcmp(list->ref_name, refname))
			திரும்பு list;
	திரும்பு வெற்று;
}

நிலையான முழு read_proc_receive_report(கட்டமைப்பு packet_reader *reader,
				    கட்டமைப்பு command *commands,
				    கட்டமைப்பு strbuf *errmsg)
{
	கட்டமைப்பு command *cmd;
	கட்டமைப்பு command *hint = வெற்று;
	கட்டமைப்பு ref_தள்ளு_report *report = வெற்று;
	முழு new_report = 0;
	முழு code = 0;
	முழு once = 0;
	முழு response = 0;

	க்கு (;;) {
		கட்டமைப்பு object_id old_oid, new_oid;
		எழுத்து *head;
		எழுத்து *refname;
		எழுத்து *p;
		எண்ணிக்கை packet_read_status status;

		status = packet_reader_read(reader);
		எனில் (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			எனில் (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				திரும்பு -1;
			}
			நிறுத்து;
		}
		response++;

		head = reader->வரி;
		p = strchr(head, ' ');
		எனில் (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			தொடர்;
		}
		*p++ = '\0';
		எனில் (!strcmp(head, "option")) {
			எழுத்து *சாவி;
			மாறிலி எழுத்து *val;

			எனில் (!hint || !(report || new_report)) {
				எனில் (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				தொடர்;
			}
			எனில் (new_report) {
				எனில் (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} இல்லையெனில் {
					report = hint->report;
					போது (report->next)
						report = report->next;
					report->next = xcalloc(1, அளவு(கட்டமைப்பு ref_தள்ளு_report));
					report = report->next;
				}
				new_report = 0;
			}
			சாவி = p;
			p = strchr(சாவி, ' ');
			எனில் (p)
				*p++ = '\0';
			val = p;
			எனில் (!strcmp(சாவி, "refname"))
				report->ref_name = xstrdup_or_null(val);
			இல்லையெனில் எனில் (!strcmp(சாவி, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			இல்லையெனில் எனில் (!strcmp(சாவி, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			இல்லையெனில் எனில் (!strcmp(சாவி, "forced-update"))
				report->forced_update = 1;
			இல்லையெனில் எனில் (!strcmp(சாவி, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			தொடர்;
		}

		report = வெற்று;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		எனில் (p)
			*p++ = '\0';
		எனில் (strcmp(head, "ok") && strcmp(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			தொடர்;
		}

		/* first try searching at our hint, falling back to all refs */
		எனில் (hint)
			hint = find_command_by_refname(hint, refname);
		எனில் (!hint)
			hint = find_command_by_refname(commands, refname);
		எனில் (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			தொடர்;
		}
		எனில் (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			தொடர்;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		எனில் (!strcmp(head, "ng")) {
			எனில் (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			இல்லையெனில்
				hint->error_string = "failed";
			code = -1;
			தொடர்;
		}
		new_report = 1;
	}

	க்கு (cmd = commands; cmd; cmd = cmd->next)
		எனில் (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	திரும்பு code;
}

நிலையான முழு run_proc_receive_hook(கட்டமைப்பு command *commands,
				 மாறிலி கட்டமைப்பு string_list *தள்ளு_options)
{
	கட்டமைப்பு child_process proc = CHILD_PROCESS_INIT;
	கட்டமைப்பு async muxer;
	கட்டமைப்பு command *cmd;
	கட்டமைப்பு packet_reader reader;
	கட்டமைப்பு strbuf cap = STRBUF_INIT;
	கட்டமைப்பு strbuf errmsg = STRBUF_INIT;
	முழு hook_use_தள்ளு_options = 0;
	முழு version = 0;
	முழு code;
	மாறிலி எழுத்து *hook_path = find_hook(the_repository, "proc-receive");

	எனில் (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		திரும்பு -1;
	}

	strvec_தள்ளு(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	எனில் (use_sideband) {
		memset(&muxer, 0, அளவு(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		எனில் (code)
			திரும்பு code;
		proc.err = muxer.in;
	} இல்லையெனில் {
		proc.err = 0;
	}

	code = start_command(&proc);
	எனில் (code) {
		எனில் (use_sideband)
			finish_async(&muxer);
		திரும்பு code;
	}

	sigchain_தள்ளு(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, வெற்று, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	எனில் (use_atomic)
		strbuf_addstr(&cap, " atomic");
	எனில் (use_தள்ளு_options)
		strbuf_addstr(&cap, " push-options");
	எனில் (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} இல்லையெனில் {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	எனில் (!code)
		code = packet_flush_gently(proc.in);

	எனில் (!code)
		க்கு (;;) {
			முழு linelen;
			எண்ணிக்கை packet_read_status status;

			status = packet_reader_read(&reader);
			எனில் (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				எனில் (status == PACKET_READ_EOF)
					code = -1;
				நிறுத்து;
			}

			எனில் (reader.pktlen > 8 && starts_with(reader.வரி, "version=")) {
				version = atoi(reader.வரி + 8);
				linelen = strlen(reader.வரி);
				எனில் (linelen < reader.pktlen) {
					மாறிலி எழுத்து *feature_list = reader.வரி + linelen + 1;
					எனில் (parse_feature_request(feature_list, "push-options"))
						hook_use_தள்ளு_options = 1;
				}
			}
		}

	எனில் (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		செல் cleanup;
	}

	தேர்வு (version) {
	நிலை 0:
		/* fallthrough */
	நிலை 1:
		நிறுத்து;
	இயல்பு:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		செல் cleanup;
	}

	/* Send commands */
	க்கு (cmd = commands; cmd; cmd = cmd->next) {
		எனில் (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			தொடர்;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		எனில் (code)
			நிறுத்து;
	}
	எனில் (!code)
		code = packet_flush_gently(proc.in);
	எனில் (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		செல் cleanup;
	}

	/* Send push options */
	எனில் (hook_use_தள்ளு_options) {
		கட்டமைப்பு string_list_item *item;

		for_each_string_list_item(item, தள்ளு_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			எனில் (code)
				நிறுத்து;
		}
		எனில் (!code)
			code = packet_flush_gently(proc.in);
		எனில் (code) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			செல் cleanup;
		}
	}

	/* Read result from proc-receive */
	code = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.in);
	close(proc.out);
	எனில் (use_sideband)
		finish_async(&muxer);
	எனில் (finish_command(&proc))
		code = -1;
	எனில் (errmsg.len >0) {
		எழுத்து *p = errmsg.buf;

		p += errmsg.len - 1;
		எனில் (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_எடு(SIGPIPE);

	திரும்பு code;
}

நிலையான மாறிலி எழுத்து *refuse_unconfigured_deny_செய்தி =
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

நிலையான வெறுமை refuse_unconfigured_deny(வெறுமை)
{
	rp_error("%s", _(refuse_unconfigured_deny_செய்தி));
}

நிலையான மாறிலி எழுத்து *refuse_unconfigured_deny_delete_current_செய்தி =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

நிலையான வெறுமை refuse_unconfigured_deny_delete_current(வெறுமை)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_செய்தி));
}

நிலையான மாறிலி கட்டமைப்பு object_id *command_singleton_iterator(வெறுமை *cb_data);
நிலையான முழு update_shallow_ref(கட்டமைப்பு command *cmd, கட்டமைப்பு shallow_info *si)
{
	கட்டமைப்பு shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	கட்டமைப்பு oid_array extra = OID_ARRAY_INIT;
	கட்டமைப்பு check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	முழு i;

	trace_printf_சாவி(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	க்கு (i = 0; i < si->shallow->nr; i++)
		எனில் (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	எனில் (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		திரும்பு -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	க்கு (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *தள்ளு_to_deploy(unsigned எழுத்து *sha1,
				  கட்டமைப்பு strvec *env,
				  மாறிலி கட்டமைப்பு worktree *worktree)
{
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", வெற்று);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	எனில் (run_command(&child))
		திரும்பு "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", வெற்று);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	எனில் (run_command(&child))
		திரும்பு "Working directory has unstaged changes";

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
		     empty_மரம்_oid_hex(the_repository->hash_algo), "--", வெற்று);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	எனில் (run_command(&child))
		திரும்பு "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     வெற்று);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	எனில் (run_command(&child))
		திரும்பு "Could not update working tree to new HEAD";

	திரும்பு வெற்று;
}

நிலையான மாறிலி எழுத்து *தள்ளு_to_checkout_hook = "push-to-checkout";

நிலையான மாறிலி எழுத்து *தள்ளு_to_checkout(unsigned எழுத்து *hash,
				    முழு *invoked_hook,
				    கட்டமைப்பு strvec *env,
				    மாறிலி எழுத்து *work_மரம்)
{
	கட்டமைப்பு run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_மரம்));
	strvec_தள்ளு(&opt.args, hash_to_hex(hash));
	எனில் (run_hooks_opt(the_repository, தள்ளு_to_checkout_hook, &opt))
		திரும்பு "push-to-checkout hook declined";
	இல்லையெனில்
		திரும்பு வெற்று;
}

நிலையான மாறிலி எழுத்து *update_worktree(unsigned எழுத்து *sha1, மாறிலி கட்டமைப்பு worktree *worktree)
{
	மாறிலி எழுத்து *retval;
	எழுத்து *git_dir;
	கட்டமைப்பு strvec env = STRVEC_INIT;
	முழு invoked_hook;

	எனில் (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	எனில் (worktree->is_bare)
		திரும்பு "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = தள்ளு_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	எனில் (!invoked_hook)
		retval = தள்ளு_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	விடுவி(git_dir);
	திரும்பு retval;
}

நிலையான மாறிலி எழுத்து *update(கட்டமைப்பு command *cmd, கட்டமைப்பு shallow_info *si)
{
	மாறிலி எழுத்து *name = cmd->ref_name;
	கட்டமைப்பு strbuf namespaced_name_buf = STRBUF_INIT;
	நிலையான எழுத்து *namespaced_name;
	மாறிலி எழுத்து *ret;
	கட்டமைப்பு object_id *old_oid = &cmd->old_oid;
	கட்டமைப்பு object_id *new_oid = &cmd->new_oid;
	முழு do_update_worktree = 0;
	கட்டமைப்பு worktree **worktrees = get_worktrees(the_repository);
	மாறிலி கட்டமைப்பு worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	எனில் (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		செல் out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	விடுவி(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, வெற்று);

	எனில் (worktree && !worktree->is_bare) {
		தேர்வு (deny_current_branch) {
		நிலை DENY_IGNORE:
			நிறுத்து;
		நிலை DENY_WARN:
			rp_warning("updating the current branch");
			நிறுத்து;
		நிலை DENY_REFUSE:
		நிலை DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			எனில் (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			செல் out;
		நிலை DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			நிறுத்து;
		}
	}

	எனில் (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		செல் out;
	}

	எனில் (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		எனில் (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			செல் out;
		}

		எனில் (worktree || (head_name && !strcmp(namespaced_name, head_name))) {
			தேர்வு (deny_delete_current) {
			நிலை DENY_IGNORE:
				நிறுத்து;
			நிலை DENY_WARN:
				rp_warning("deleting the current branch");
				நிறுத்து;
			நிலை DENY_REFUSE:
			நிலை DENY_UNCONFIGURED:
			நிலை DENY_UPDATE_INSTEAD:
				எனில் (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				செல் out;
			இயல்பு:
				ret = "Invalid denyDeleteCurrent setting";
				செல் out;
			}
		}
	}

	எனில் (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		கட்டமைப்பு object *old_object, *new_object;
		கட்டமைப்பு commit *old_commit, *new_commit;
		முழு ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		எனில் (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			செல் out;
		}
		old_commit = (கட்டமைப்பு commit *)old_object;
		new_commit = (கட்டமைப்பு commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		எனில் (ret2 < 0)
			exit(128);
		எனில் (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			செல் out;
		}
	}
	எனில் (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		செல் out;
	}

	எனில் (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		எனில் (ret)
			செல் out;
	}

	எனில் (is_null_oid(new_oid)) {
		கட்டமைப்பு strbuf err = STRBUF_INIT;
		எனில் (!parse_object(the_repository, old_oid)) {
			old_oid = வெற்று;
			எனில் (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} இல்லையெனில் {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		எனில் (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   வெற்று, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} இல்லையெனில் {
			ret = வெற்று; /* good */
		}
		strbuf_release(&err);
	} இல்லையெனில் {
		எண்ணிக்கை ref_transaction_error tx_err;
		கட்டமைப்பு strbuf err = STRBUF_INIT;
		எனில் (shallow_update && si->shallow_ref[cmd->index] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			செல் out;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  வெற்று, வெற்று,
						  0, "push",
						  &err);
		எனில் (tx_err) {
			rp_error("%s", err.buf);
			எனில் (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			இல்லையெனில்
				ret = ref_transaction_error_செய்தி(tx_err);
		} இல்லையெனில் {
			ret = வெற்று; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	திரும்பு ret;
}

நிலையான வெறுமை run_update_post_hook(கட்டமைப்பு command *commands)
{
	நிலையான மாறிலி எழுத்து hook_name[] = "post-update";
	கட்டமைப்பு run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	கட்டமைப்பு async sideband_async;
	கட்டமைப்பு command *cmd;
	முழு sideband_async_started = 0;
	முழு saved_stderr = -1;

	எனில் (!hook_exists(the_repository, hook_name))
		திரும்பு;

	க்கு (cmd = commands; cmd; cmd = cmd->next) {
		எனில் (cmd->error_string || cmd->did_not_exist)
			தொடர்;
		strvec_தள்ளு(&opt.args, cmd->ref_name);
	}
	எனில் (!opt.args.nr)
		திரும்பு;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

நிலையான வெறுமை check_aliased_update_internal(கட்டமைப்பு command *cmd,
					  கட்டமைப்பு string_list *list,
					  மாறிலி எழுத்து *dst_name, முழு flag)
{
	கட்டமைப்பு string_list_item *item;
	கட்டமைப்பு command *dst_cmd;

	எனில் (!(flag & REF_ISSYMREF))
		திரும்பு;

	எனில் (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		திரும்பு;
	}
	dst_name = strip_namespace(dst_name);

	எனில் (!(item = string_list_lookup(list, dst_name)))
		திரும்பு;

	cmd->skip_update = 1;

	dst_cmd = (கட்டமைப்பு command *) item->util;

	எனில் (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		திரும்பு;

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

நிலையான வெறுமை check_aliased_update(கட்டமைப்பு command *cmd, கட்டமைப்பு string_list *list)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	மாறிலி எழுத்து *dst_name;
	முழு flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, வெற்று, &flag);
	check_aliased_update_internal(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

நிலையான வெறுமை check_aliased_updates(கட்டமைப்பு command *commands)
{
	கட்டமைப்பு command *cmd;
	கட்டமைப்பு string_list ref_list = STRING_LIST_INIT_NODUP;

	க்கு (cmd = commands; cmd; cmd = cmd->next) {
		கட்டமைப்பு string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (வெறுமை *)cmd;
	}
	string_list_sort(&ref_list);

	க்கு (cmd = commands; cmd; cmd = cmd->next) {
		எனில் (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

நிலையான மாறிலி கட்டமைப்பு object_id *command_singleton_iterator(வெறுமை *cb_data)
{
	கட்டமைப்பு command **cmd_list = cb_data;
	கட்டமைப்பு command *cmd = *cmd_list;

	எனில் (!cmd || is_null_oid(&cmd->new_oid))
		திரும்பு வெற்று;
	*cmd_list = வெற்று; /* this returns only one */
	திரும்பு &cmd->new_oid;
}

நிலையான வெறுமை set_connectivity_errors(கட்டமைப்பு command *commands,
				    கட்டமைப்பு shallow_info *si,
				    கட்டமைப்பு odb_transaction *transaction)
{
	கட்டமைப்பு command *cmd;

	க்கு (cmd = commands; cmd; cmd = cmd->next) {
		கட்டமைப்பு command *singleton = cmd;
		கட்டமைப்பு check_connected_options opt = CHECK_CONNECTED_INIT;
		கட்டமைப்பு strvec env = STRVEC_INIT;

		எனில் (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			தொடர்;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		எனில் (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			தொடர்;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

கட்டமைப்பு iterate_data {
	கட்டமைப்பு command *cmds;
	கட்டமைப்பு shallow_info *si;
};

நிலையான மாறிலி கட்டமைப்பு object_id *iterate_receive_command_list(வெறுமை *cb_data)
{
	கட்டமைப்பு iterate_data *data = cb_data;
	கட்டமைப்பு command **cmd_list = &data->cmds;
	கட்டமைப்பு command *cmd = *cmd_list;

	க்கு (; cmd; cmd = cmd->next) {
		எனில் (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			தொடர்;
		எனில் (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			திரும்பு &cmd->new_oid;
		}
	}
	திரும்பு வெற்று;
}

நிலையான வெறுமை reject_updates_to_hidden(கட்டமைப்பு command *commands)
{
	கட்டமைப்பு strbuf refname_full = STRBUF_INIT;
	size_t prefix_len;
	கட்டமைப்பு command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	க்கு (cmd = commands; cmd; cmd = cmd->next) {
		எனில் (cmd->error_string)
			தொடர்;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		எனில் (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			தொடர்;
		எனில் (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		இல்லையெனில்
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

நிலையான முழு should_process_cmd(கட்டமைப்பு command *cmd)
{
	திரும்பு !cmd->error_string && !cmd->skip_update;
}

நிலையான வெறுமை BUG_if_skipped_connectivity_check(கட்டமைப்பு command *commands,
					       கட்டமைப்பு shallow_info *si)
{
	கட்டமைப்பு command *cmd;

	க்கு (cmd = commands; cmd; cmd = cmd->next) {
		எனில் (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

நிலையான வெறுமை ref_transaction_rejection_handler(மாறிலி எழுத்து *refname,
					      மாறிலி கட்டமைப்பு object_id *old_oid UNUSED,
					      மாறிலி கட்டமைப்பு object_id *new_oid UNUSED,
					      மாறிலி எழுத்து *old_target UNUSED,
					      மாறிலி எழுத்து *new_target UNUSED,
					      எண்ணிக்கை ref_transaction_error err,
					      மாறிலி எழுத்து *details,
					      வெறுமை *cb_data)
{
	கட்டமைப்பு strmap *failed_refs = cb_data;

	எனில் (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (எழுத்து *)ref_transaction_error_செய்தி(err));
}

நிலையான வெறுமை execute_commands_non_atomic(கட்டமைப்பு command *commands,
					கட்டமைப்பு shallow_info *si)
{
	கட்டமைப்பு command *cmd;
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	மாறிலி எழுத்து *reported_error = வெற்று;
	கட்டமைப்பு strmap failed_refs = STRMAP_INIT;

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
	எண்ணிக்கை processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	க்கு (எண்ணிக்கை processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		க்கு (cmd = commands; cmd; cmd = cmd->next) {
			எனில் (!should_process_cmd(cmd) || cmd->run_proc_receive)
				தொடர்;

			எனில் (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				தொடர்;
			இல்லையெனில் எனில் (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				தொடர்;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			எனில் (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				எனில் (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					செல் failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		எனில் (!transaction)
			செல் cleanup;

		எனில் (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			செல் failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		எனில் (strmap_empty(&failed_refs))
			செல் cleanup;

	failure:
		க்கு (cmd = commands; cmd; cmd = cmd->next) {
			எனில் (reported_error)
				cmd->error_string = reported_error;
			இல்லையெனில் எனில் (strmap_contains(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = வெற்று;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

நிலையான வெறுமை execute_commands_atomic(கட்டமைப்பு command *commands,
					கட்டமைப்பு shallow_info *si)
{
	கட்டமைப்பு command *cmd;
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	மாறிலி எழுத்து *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	எனில் (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		செல் failure;
	}

	க்கு (cmd = commands; cmd; cmd = cmd->next) {
		எனில் (!should_process_cmd(cmd) || cmd->run_proc_receive)
			தொடர்;

		cmd->error_string = update(cmd, si);

		எனில் (cmd->error_string)
			செல் failure;
	}

	எனில் (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		செல் failure;
	}
	செல் cleanup;

failure:
	க்கு (cmd = commands; cmd; cmd = cmd->next)
		எனில் (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

நிலையான வெறுமை execute_commands(கட்டமைப்பு command *commands,
			     முழு unpacker_error,
			     கட்டமைப்பு shallow_info *si,
			     கட்டமைப்பு odb_transaction *transaction,
			     மாறிலி கட்டமைப்பு string_list *தள்ளு_options)
{
	கட்டமைப்பு check_connected_options opt = CHECK_CONNECTED_INIT;
	கட்டமைப்பு command *cmd;
	கட்டமைப்பு iterate_data data;
	கட்டமைப்பு async muxer;
	முழு err_fd = 0;
	முழு run_proc_receive = 0;

	எனில் (unpacker_error) {
		க்கு (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		திரும்பு;
	}

	எனில் (!skip_connectivity_check) {
		கட்டமைப்பு strvec env = STRVEC_INIT;

		எனில் (use_sideband) {
			memset(&muxer, 0, அளவு(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			எனில் (!start_async(&muxer))
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

		எனில் (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		எனில் (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	எனில் (proc_receive_ref) {
		க்கு (cmd = commands; cmd; cmd = cmd->next) {
			எனில் (!should_process_cmd(cmd))
				தொடர்;

			எனில் (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	எனில் (run_receive_hook(commands, "pre-receive", 0, transaction, தள்ளு_options)) {
		க்கு (cmd = commands; cmd; cmd = cmd->next) {
			எனில் (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		திரும்பு;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	க்கு (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	எனில் (!cmd)
		திரும்பு;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	எனில் (odb_transaction_commit(transaction)) {
		க்கு (cmd = commands; cmd; cmd = cmd->next) {
			எனில் (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		திரும்பு;
	}

	check_aliased_updates(commands);

	விடுவி(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, வெற்று,
							    வெற்று);

	எனில் (run_proc_receive &&
	    run_proc_receive_hook(commands, தள்ளு_options))
		க்கு (cmd = commands; cmd; cmd = cmd->next)
			எனில் (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	எனில் (use_atomic)
		execute_commands_atomic(commands, si);
	இல்லையெனில்
		execute_commands_non_atomic(commands, si);

	எனில் (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

நிலையான கட்டமைப்பு command **queue_command(கட்டமைப்பு command **tail,
				      மாறிலி எழுத்து *வரி,
				      முழு linelen)
{
	கட்டமைப்பு object_id old_oid, new_oid;
	கட்டமைப்பு command *cmd;
	மாறிலி எழுத்து *refname;
	முழு reflen;
	மாறிலி எழுத்து *p;

	எனில் (parse_oid_hex(வரி, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", வரி);

	refname = p;
	reflen = linelen - (p - வரி);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*tail = cmd;
	திரும்பு &cmd->next;
}

நிலையான வெறுமை free_commands(கட்டமைப்பு command *commands)
{
	போது (commands) {
		கட்டமைப்பு command *next = commands->next;

		ref_தள்ளு_report_free(commands->report);
		விடுவி(commands->error_string_owned);
		விடுவி(commands);
		commands = next;
	}
}

நிலையான வெறுமை queue_commands_from_cert(கட்டமைப்பு command **tail,
				     கட்டமைப்பு strbuf *தள்ளு_cert)
{
	மாறிலி எழுத்து *boc, *eoc;

	எனில் (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(தள்ளு_cert->buf, "\n\n");
	எனில் (!boc)
		die("malformed push certificate %.*s", 100, தள்ளு_cert->buf);
	இல்லையெனில்
		boc += 2;
	eoc = தள்ளு_cert->buf + parse_signed_buffer(தள்ளு_cert->buf, தள்ளு_cert->len);

	போது (boc < eoc) {
		மாறிலி எழுத்து *eol = memchr(boc, '\n', eoc - boc);
		tail = queue_command(tail, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

நிலையான கட்டமைப்பு command *read_head_info(கட்டமைப்பு packet_reader *reader,
				      கட்டமைப்பு oid_array *shallow,
				      எண்ணிக்கை report_status_version *version)
{
	கட்டமைப்பு command *commands = வெற்று;
	கட்டமைப்பு command **p = &commands;
	க்கு (;;) {
		முழு linelen;

		எனில் (packet_reader_read(reader) != PACKET_READ_NORMAL)
			நிறுத்து;

		எனில் (reader->pktlen > 8 && starts_with(reader->வரி, "shallow ")) {
			கட்டமைப்பு object_id oid;
			எனில் (get_oid_hex(reader->வரி + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->வரி + 8);
			oid_array_append(shallow, &oid);
			தொடர்;
		}

		linelen = strlen(reader->வரி);
		எனில் (linelen < reader->pktlen) {
			மாறிலி எழுத்து *feature_list = reader->வரி + linelen + 1;
			மாறிலி எழுத்து *hash = வெற்று;
			மாறிலி எழுத்து *client_sid;
			size_t len = 0;
			எனில் (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			எனில் (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			எனில் (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			எனில் (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			எனில் (advertise_atomic_தள்ளு
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			எனில் (advertise_தள்ளு_options
			    && parse_feature_request(feature_list, "push-options"))
				use_தள்ளு_options = 1;
			hash = parse_feature_மதிப்பு(feature_list, "object-format", &len, வெற்று);
			எனில் (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			எனில் (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_மதிப்பு(feature_list, "session-id", &len, வெற்று);
			எனில் (client_sid) {
				எழுத்து *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", வெற்று, "client-sid", client_sid);
				விடுவி(sid);
			}
		}

		எனில் (!strcmp(reader->வரி, "push-cert")) {
			முழு true_flush = 0;
			முழு saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			க்கு (;;) {
				packet_reader_read(reader);
				எனில் (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					நிறுத்து;
				}
				எனில் (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				எனில் (!strcmp(reader->வரி, "push-cert-end\n"))
					நிறுத்து; /* end of cert */
				strbuf_addstr(&தள்ளு_cert, reader->வரி);
			}
			reader->options = saved_options;

			எனில் (true_flush)
				நிறுத்து;
			தொடர்;
		}

		p = queue_command(p, reader->வரி, linelen);
	}

	எனில் (தள்ளு_cert.len)
		queue_commands_from_cert(p, &தள்ளு_cert);

	திரும்பு commands;
}

நிலையான வெறுமை read_தள்ளு_options(கட்டமைப்பு packet_reader *reader,
			      கட்டமைப்பு string_list *options)
{
	போது (1) {
		எனில் (packet_reader_read(reader) != PACKET_READ_NORMAL)
			நிறுத்து;

		string_list_append(options, reader->வரி);
	}
}

நிலையான முழு unpack_with_sideband(கட்டமைப்பு odb_transaction *transaction,
				மாறிலி எழுத்து *shallow_file,
				கட்டமைப்பு strbuf *err_செய்தி)
{
	கட்டமைப்பு odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_செய்தி_types = fsck_செய்தி_types.buf,
		.max_உள்ளீடு_size = max_உள்ளீடு_size,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	கட்டமைப்பு async muxer;
	முழு ret;

	எனில் (!use_sideband)
		திரும்பு odb_transaction_write_pack(transaction, 0, err_செய்தி, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, அளவு(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	எனில் (start_async(&muxer))
		திரும்பு 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_செய்தி, &opts);

	finish_async(&muxer);
	திரும்பு ret;
}

நிலையான வெறுமை prepare_shallow_update(கட்டமைப்பு shallow_info *si,
				   மாறிலி எழுத்து *shallow_file)
{
	முழு i, j, k, bitmap_size = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, வெற்று);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	க்கு (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	க்கு (i = 0; i < si->shallow->nr; i++) {
		எனில் (!si->used_shallow[i])
			தொடர்;
		க்கு (j = 0; j < bitmap_size; j++) {
			எனில் (!si->used_shallow[i][j])
				தொடர்;
			si->need_reachability_test[i]++;
			க்கு (k = 0; k < 32; k++)
				எனில் (si->used_shallow[i][j] & (1U << k))
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

நிலையான வெறுமை update_shallow_info(கட்டமைப்பு command *commands,
				கட்டமைப்பு shallow_info *si,
				கட்டமைப்பு oid_array *ref,
				மாறிலி எழுத்து *shallow_file)
{
	கட்டமைப்பு command *cmd;
	முழு *ref_status;
	remove_nonexistent_theirs_shallow(si);
	எனில் (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		திரும்பு;
	}

	க்கு (cmd = commands; cmd; cmd = cmd->next) {
		எனில் (is_null_oid(&cmd->new_oid))
			தொடர்;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	எனில் (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		திரும்பு;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, வெற்று, ref_status);
	க்கு (cmd = commands; cmd; cmd = cmd->next) {
		எனில் (is_null_oid(&cmd->new_oid))
			தொடர்;
		எனில் (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	விடுவி(ref_status);
}

நிலையான வெறுமை override_cmds_error(கட்டமைப்பு command *commands, மாறிலி எழுத்து *err)
{
	க்கு (கட்டமைப்பு command *cmd = commands; cmd; cmd = cmd->next) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
நிலையான வெறுமை generate_report(கட்டமைப்பு strbuf *buf, கட்டமைப்பு command *commands,
			    மாறிலி கட்டமைப்பு strbuf *unpack_status,
			    எண்ணிக்கை report_status_version version)
{
	கட்டமைப்பு command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	க்கு (cmd = commands; cmd; cmd = cmd->next) {
		கட்டமைப்பு ref_தள்ளு_report *report;
		முழு மொத்தம் = 0;

		எனில் (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		இல்லையெனில்
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		எனில் (version != REPORT_STATUS_V2 || cmd->error_string)
			தொடர்;

		க்கு (report = cmd->report; report; report = report->next) {
			எனில் (மொத்தம்++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			எனில் (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			எனில் (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			எனில் (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			எனில் (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

நிலையான வெறுமை report(கட்டமைப்பு command *commands, மாறிலி கட்டமைப்பு strbuf *unpack_status,
		   எண்ணிக்கை report_status_version version)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	எனில் (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	எனில் (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	இல்லையெனில்
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

நிலையான முழு delete_only(கட்டமைப்பு command *commands)
{
	கட்டமைப்பு command *cmd;
	க்கு (cmd = commands; cmd; cmd = cmd->next) {
		எனில் (!is_null_oid(&cmd->new_oid))
			திரும்பு 0;
	}
	திரும்பு 1;
}

முழு cmd_receive_pack(முழு argc,
		     மாறிலி எழுத்து **argv,
		     மாறிலி எழுத்து *prefix,
		     கட்டமைப்பு repository *repo UNUSED)
{
	முழு advertise_refs = 0;
	கட்டமைப்பு command *commands;
	கட்டமைப்பு oid_array shallow = OID_ARRAY_INIT;
	கட்டமைப்பு oid_array ref = OID_ARRAY_INIT;
	கட்டமைப்பு shallow_info si;
	கட்டமைப்பு packet_reader reader;
	கட்டமைப்பு odb_transaction *transaction = வெற்று;
	எண்ணிக்கை report_status_version version = REPORT_STATUS_UNKNOWN;

	கட்டமைப்பு option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, வெற்று),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, வெற்று),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, வெற்று),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, வெற்று),
		OPT_END()
	};

	packet_trace_identity("receive-pack");

	argc = parse_options(argc, argv, prefix, options, receive_pack_usage, 0);

	எனில் (argc > 1)
		usage_செய்தி_opt(_("too many arguments"), receive_pack_usage, options);
	எனில் (argc == 0)
		usage_செய்தி_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	எனில் (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, வெற்று);
	எனில் (cert_nonce_seed)
		தள்ளு_cert_nonce = prepare_தள்ளு_cert_nonce(service_dir, time(வெற்று));

	தேர்வு (determine_protocol_version_server()) {
	நிலை protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		நிறுத்து;
	நிலை protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		எனில் (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	நிலை protocol_v0:
		நிறுத்து;
	நிலை protocol_unknown_version:
		BUG("unknown protocol version");
	}

	எனில் (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	எனில் (advertise_refs)
		திரும்பு 0;

	packet_reader_init(&reader, 0, வெற்று, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	எனில் ((commands = read_head_info(&reader, &shallow, &version))) {
		கட்டமைப்பு string_list தள்ளு_options = STRING_LIST_INIT_DUP;
		கட்டமைப்பு strbuf unpack_status = STRBUF_INIT;

		எனில் (use_தள்ளு_options)
			read_தள்ளு_options(&reader, &தள்ளு_options);
		எனில் (!check_cert_தள்ளு_options(&தள்ளு_options)) {
			கட்டமைப்பு command *cmd;
			க்கு (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		எனில் (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		எனில் (!delete_only(commands)) {
			மாறிலி எழுத்து *alt_shallow_file = வெற்று;

			எனில் (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			எனில் (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			இல்லையெனில்
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &தள்ளு_options);
		odb_transaction_finalize(transaction);
		sigchain_தள்ளு(SIGPIPE, SIG_IGN);

		தேர்வு (version) {
		நிலை REPORT_STATUS_V2:
		நிலை REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			நிறுத்து;
		நிலை REPORT_STATUS_UNKNOWN:
			நிறுத்து;
		}

		sigchain_எடு(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, வெற்று,
				 &தள்ளு_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&தள்ளு_options, 0);
		எனில் (auto_gc) {
			கட்டமைப்பு child_process proc = CHILD_PROCESS_INIT;

			எனில் (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				எனில் (!start_command(&proc)) {
					எனில் (use_sideband)
						copy_to_sideband(proc.err, -1, வெற்று);
					finish_command(&proc);
				}
			}
		}
		எனில் (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	எனில் (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	விடுவி((வெறுமை *)தள்ளு_cert_nonce);
	திரும்பு 0;
}
