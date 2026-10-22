#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "connect.h"
#અન્તર્ભાવ "connected.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "exec-cmd.h"
#અન્તર્ભાવ "fsck.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "gpg-interface.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "hook.h"
#અન્તર્ભાવ "object.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "oidset.h"
#અન્તર્ભાવ "pack.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "pkt-line.h"
#અન્તર્ભાવ "protocol.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "server-info.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "shallow.h"
#અન્તર્ભાવ "sideband.h"
#અન્તર્ભાવ "sigchain.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "trace.h"
#અન્તર્ભાવ "trace2.h"
#અન્તર્ભાવ "version.h"
#અન્તર્ભાવ "worktree.h"

સ્થિર અચલ અક્ષર * અચલ receive_pack_usage[] = {
	N_("git receive-pack <git-dir>"),
	ખાલી
};

ગણના deny_action {
	DENY_UNCONFIGURED,
	DENY_IGNORE,
	DENY_WARN,
	DENY_REFUSE,
	DENY_UPDATE_INSTEAD
};

ગણના report_status_version {
	REPORT_STATUS_UNKNOWN = 0,
	REPORT_STATUS_V0,
	REPORT_STATUS_V2,
};

સ્થિર પૂર્ણાંક deny_deletes;
સ્થિર પૂર્ણાંક deny_non_fast_forwards;
સ્થિર ગણના deny_action deny_current_branch = DENY_UNCONFIGURED;
સ્થિર ગણના deny_action deny_delete_current = DENY_UNCONFIGURED;
સ્થિર પૂર્ણાંક receive_fsck_objects = -1;
સ્થિર પૂર્ણાંક transfer_fsck_objects = -1;
સ્થિર સંરચના strbuf fsck_સંદેશ_types = STRBUF_INIT;
સ્થિર પૂર્ણાંક advertise_atomic_ધકેલો = 1;
સ્થિર પૂર્ણાંક advertise_ધકેલો_options;
સ્થિર પૂર્ણાંક advertise_sid;
સ્થિર off_t max_આગત_size;
સ્થિર પૂર્ણાંક use_sideband;
સ્થિર પૂર્ણાંક use_atomic;
સ્થિર પૂર્ણાંક use_ધકેલો_options;
સ્થિર પૂર્ણાંક quiet;
સ્થિર પૂર્ણાંક prefer_ofs_delta = 1;
સ્થિર પૂર્ણાંક auto_update_server_info;
સ્થિર પૂર્ણાંક auto_gc = 1;
સ્થિર પૂર્ણાંક reject_thin;
સ્થિર પૂર્ણાંક skip_connectivity_check;
સ્થિર પૂર્ણાંક stateless_rpc;
સ્થિર અચલ અક્ષર *service_dir;
સ્થિર અચલ અક્ષર *head_name;
સ્થિર શૂન્ય *head_name_to_free;
સ્થિર પૂર્ણાંક sent_capabilities;
સ્થિર પૂર્ણાંક shallow_update;
સ્થિર સંરચના strbuf ધકેલો_cert = STRBUF_INIT;
સ્થિર સંરચના object_id ધકેલો_cert_oid;
સ્થિર સંરચના signature_check sigcheck;
સ્થિર અચલ અક્ષર *ધકેલો_cert_nonce;
સ્થિર અક્ષર *cert_nonce_seed;
સ્થિર સંરચના strvec hidden_refs = STRVEC_INIT;

સ્થિર અચલ અક્ષર *NONCE_UNSOLICITED = "UNSOLICITED";
સ્થિર અચલ અક્ષર *NONCE_BAD = "BAD";
સ્થિર અચલ અક્ષર *NONCE_MISSING = "MISSING";
સ્થિર અચલ અક્ષર *NONCE_OK = "OK";
સ્થિર અચલ અક્ષર *NONCE_SLOP = "SLOP";
સ્થિર અચલ અક્ષર *nonce_status;
સ્થિર long nonce_stamp_slop;
સ્થિર timestamp_t nonce_stamp_slop_limit;
સ્થિર સંરચના ref_transaction *transaction;

સ્થિર ગણના {
	KEEPALIVE_NEVER = 0,
	KEEPALIVE_AFTER_NUL,
	KEEPALIVE_ALWAYS
} use_keepalive;
સ્થિર પૂર્ણાંક keepalive_in_sec = 5;

સ્થિર સંરચના proc_receive_ref {
	unsigned પૂર્ણાંક want_add:1,
		     want_delete:1,
		     want_modify:1,
		     negative_ref:1;
	અક્ષર *ref_prefix;
	સંરચના proc_receive_ref *next;
} *proc_receive_ref;

સ્થિર શૂન્ય proc_receive_ref_append(અચલ અક્ષર *prefix);

સ્થિર ગણના deny_action parse_deny_action(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત)
{
	જો (કિંમત) {
		જો (!strcasecmp(કિંમત, "ignore"))
			પાછા DENY_IGNORE;
		જો (!strcasecmp(કિંમત, "warn"))
			પાછા DENY_WARN;
		જો (!strcasecmp(કિંમત, "refuse"))
			પાછા DENY_REFUSE;
		જો (!strcasecmp(કિંમત, "updateinstead"))
			પાછા DENY_UPDATE_INSTEAD;
	}
	જો (git_config_bool(var, કિંમત))
		પાછા DENY_REFUSE;
	પાછા DENY_IGNORE;
}

સ્થિર પૂર્ણાંક receive_pack_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			       અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	અચલ અક્ષર *સંદેશ_id;
	પૂર્ણાંક status = parse_hide_refs_config(var, કિંમત, "receive", &hidden_refs);

	જો (status)
		પાછા status;

	જો (strcmp(var, "receive.denydeletes") == 0) {
		deny_deletes = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (strcmp(var, "receive.denynonfastforwards") == 0) {
		deny_non_fast_forwards = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (strcmp(var, "receive.fsck.skiplist") == 0) {
		અક્ષર *path;

		જો (git_config_pathname(&path, var, કિંમત))
			પાછા -1;
		જો (path)
			strbuf_addf(&fsck_સંદેશ_types, "%cskiplist=%s",
				    fsck_સંદેશ_types.len ? ',' : '=', path);
		મુક્ત(path);
		પાછા 0;
	}

	જો (skip_prefix(var, "receive.fsck.", &સંદેશ_id)) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		જો (is_valid_સંદેશ_type(સંદેશ_id, કિંમત))
			strbuf_addf(&fsck_સંદેશ_types, "%c%s=%s",
				fsck_સંદેશ_types.len ? ',' : '=', સંદેશ_id, કિંમત);
		નહીંતર
			warning("skipping unknown msg id '%s'", સંદેશ_id);
		પાછા 0;
	}

	જો (strcmp(var, "receive.fsckobjects") == 0) {
		receive_fsck_objects = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (strcmp(var, "transfer.fsckobjects") == 0) {
		transfer_fsck_objects = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (!strcmp(var, "receive.denycurrentbranch")) {
		deny_current_branch = parse_deny_action(var, કિંમત);
		પાછા 0;
	}

	જો (strcmp(var, "receive.denydeletecurrent") == 0) {
		deny_delete_current = parse_deny_action(var, કિંમત);
		પાછા 0;
	}

	જો (strcmp(var, "repack.usedeltabaseoffset") == 0) {
		prefer_ofs_delta = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (strcmp(var, "receive.updateserverinfo") == 0) {
		auto_update_server_info = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (strcmp(var, "receive.autogc") == 0) {
		auto_gc = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (strcmp(var, "receive.shallowupdate") == 0) {
		shallow_update = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (strcmp(var, "receive.certnonceseed") == 0)
		પાછા git_config_string(&cert_nonce_seed, var, કિંમત);

	જો (strcmp(var, "receive.certnonceslop") == 0) {
		nonce_stamp_slop_limit = git_config_ulong(var, કિંમત, ctx->kvi);
		પાછા 0;
	}

	જો (strcmp(var, "receive.advertiseatomic") == 0) {
		advertise_atomic_ધકેલો = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (strcmp(var, "receive.advertisepushoptions") == 0) {
		advertise_ધકેલો_options = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (strcmp(var, "receive.keepalive") == 0) {
		keepalive_in_sec = git_config_int(var, કિંમત, ctx->kvi);
		પાછા 0;
	}

	જો (strcmp(var, "receive.maxinputsize") == 0) {
		max_આગત_size = git_config_int64(var, કિંમત, ctx->kvi);
		પાછા 0;
	}

	જો (strcmp(var, "receive.procreceiverefs") == 0) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		proc_receive_ref_append(કિંમત);
		પાછા 0;
	}

	જો (strcmp(var, "transfer.advertisesid") == 0) {
		advertise_sid = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

સ્થિર શૂન્ય show_ref(અચલ અક્ષર *path, અચલ સંરચના object_id *oid)
{
	જો (sent_capabilities) {
		packet_write_fmt(1, "%s %s\n", oid_to_hex(oid), path);
	} નહીંતર {
		સંરચના strbuf cap = STRBUF_INIT;

		strbuf_addstr(&cap,
			      "report-status report-status-v2 delete-refs side-band-64k quiet");
		જો (advertise_atomic_ધકેલો)
			strbuf_addstr(&cap, " atomic");
		જો (prefer_ofs_delta)
			strbuf_addstr(&cap, " ofs-delta");
		જો (ધકેલો_cert_nonce)
			strbuf_addf(&cap, " push-cert=%s", ધકેલો_cert_nonce);
		જો (advertise_ધકેલો_options)
			strbuf_addstr(&cap, " push-options");
		જો (advertise_sid)
			strbuf_addf(&cap, " session-id=%s", trace2_session_id());
		strbuf_addf(&cap, " object-format=%s", the_hash_algo->name);
		strbuf_addf(&cap, " agent=%s", git_user_agent_sanitized());
		packet_write_fmt(1, "%s %s%c%s\n",
			     oid_to_hex(oid), path, 0, cap.buf);
		strbuf_release(&cap);
		sent_capabilities = 1;
	}
}

સ્થિર પૂર્ણાંક show_ref_cb(અચલ સંરચના reference *ref, શૂન્ય *data)
{
	સંરચના oidset *seen = data;
	અચલ અક્ષર *path = strip_namespace(ref->name);

	જો (ref_is_hidden(path, ref->name, &hidden_refs))
		પાછા 0;

	/*
	 * Advertise refs outside our current namespace as ".have"
	 * refs, so that the client can use them to minimize data
	 * transfer but will otherwise ignore them.
	 */
	જો (!path) {
		જો (oidset_ઉમેરો(seen, ref->oid))
			પાછા 0;
		path = ".have";
	} નહીંતર {
		oidset_ઉમેરો(seen, ref->oid);
	}
	show_ref(path, ref->oid);
	પાછા 0;
}

સ્થિર શૂન્ય show_one_alternate_ref(અચલ સંરચના object_id *oid,
				   શૂન્ય *data)
{
	સંરચના oidset *seen = data;

	જો (oidset_ઉમેરો(seen, oid))
		પાછા;

	show_ref(".have", oid);
}

સ્થિર શૂન્ય write_head_info(શૂન્ય)
{
	સંરચના refs_for_each_ref_options opts = { 0 };
	સ્થિર સંરચના oidset seen = OIDSET_INIT;
	સંરચના strvec excludes_vector = STRVEC_INIT;

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

	જો (!sent_capabilities)
		show_ref("capabilities^{}", null_oid(the_hash_algo));

	advertise_shallow_grafts(1);

	/* EOF */
	packet_flush(1);
}

#define RUN_PROC_RECEIVE_SCHEDULED	1
#define RUN_PROC_RECEIVE_RETURNED	2
સંરચના command {
	સંરચના command *next;
	અચલ અક્ષર *error_string;
	અક્ષર *error_string_owned;
	સંરચના ref_ધકેલો_report *report;
	unsigned પૂર્ણાંક skip_update:1,
		     did_not_exist:1,
		     run_proc_receive:2;
	પૂર્ણાંક index;
	સંરચના object_id old_oid;
	સંરચના object_id new_oid;
	અક્ષર ref_name[FLEX_ARRAY]; /* more */
};

સ્થિર શૂન્ય proc_receive_ref_append(અચલ અક્ષર *prefix)
{
	સંરચના proc_receive_ref *ref_pattern;
	અચલ અક્ષર *p;
	પૂર્ણાંક len;

	CALLOC_ARRAY(ref_pattern, 1);
	p = strchr(prefix, ':');
	જો (p) {
		જ્યારે (prefix < p) {
			જો (*prefix == 'a')
				ref_pattern->want_add = 1;
			નહીંતર જો (*prefix == 'd')
				ref_pattern->want_delete = 1;
			નહીંતર જો (*prefix == 'm')
				ref_pattern->want_modify = 1;
			નહીંતર જો (*prefix == '!')
				ref_pattern->negative_ref = 1;
			prefix++;
		}
		prefix++;
	} નહીંતર {
		ref_pattern->want_add = 1;
		ref_pattern->want_delete = 1;
		ref_pattern->want_modify = 1;
	}
	len = strlen(prefix);
	જ્યારે (len && prefix[len - 1] == '/')
		len--;
	ref_pattern->ref_prefix = xmemdupz(prefix, len);
	જો (!proc_receive_ref) {
		proc_receive_ref = ref_pattern;
	} નહીંતર {
		સંરચના proc_receive_ref *end;

		end = proc_receive_ref;
		જ્યારે (end->next)
			end = end->next;
		end->next = ref_pattern;
	}
}

સ્થિર પૂર્ણાંક proc_receive_ref_matches(સંરચના command *cmd)
{
	સંરચના proc_receive_ref *p;

	જો (!proc_receive_ref)
		પાછા 0;

	માટે (p = proc_receive_ref; p; p = p->next) {
		અચલ અક્ષર *match = p->ref_prefix;
		અચલ અક્ષર *remains;

		જો (!p->want_add && is_null_oid(&cmd->old_oid))
			ચાલુ;
		નહીંતર જો (!p->want_delete && is_null_oid(&cmd->new_oid))
			ચાલુ;
		નહીંતર જો (!p->want_modify &&
			 !is_null_oid(&cmd->old_oid) &&
			 !is_null_oid(&cmd->new_oid))
			ચાલુ;

		જો (skip_prefix(cmd->ref_name, match, &remains) &&
		    (!*remains || *remains == '/')) {
			જો (!p->negative_ref)
				પાછા 1;
		} નહીંતર જો (p->negative_ref) {
			પાછા 1;
		}
	}
	પાછા 0;
}

સ્થિર શૂન્ય report_message(અચલ અક્ષર *prefix, અચલ અક્ષર *err, va_list params)
{
	પૂર્ણાંક sz;
	અક્ષર સંદેશ[4096];

	sz = xsnprintf(સંદેશ, કદ(સંદેશ), "%s", prefix);
	sz += vsnprintf(સંદેશ + sz, કદ(સંદેશ) - sz, err, params);
	જો (sz > (કદ(સંદેશ) - 1))
		sz = કદ(સંદેશ) - 1;
	સંદેશ[sz++] = '\n';

	જો (use_sideband)
		send_sideband(1, 2, સંદેશ, sz, use_sideband);
	નહીંતર
		xwrite(2, સંદેશ, sz);
}

__attribute__((format (printf, 1, 2)))
સ્થિર શૂન્ય rp_warning(અચલ અક્ષર *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("warning: ", err, params);
	va_end(params);
}

__attribute__((format (printf, 1, 2)))
સ્થિર શૂન્ય rp_error(અચલ અક્ષર *err, ...)
{
	va_list params;
	va_start(params, err);
	report_message("error: ", err, params);
	va_end(params);
}

સ્થિર પૂર્ણાંક copy_to_sideband(પૂર્ણાંક in, પૂર્ણાંક out UNUSED, શૂન્ય *arg UNUSED)
{
	અક્ષર data[128];
	પૂર્ણાંક keepalive_active = 0;

	જો (keepalive_in_sec <= 0)
		use_keepalive = KEEPALIVE_NEVER;
	જો (use_keepalive == KEEPALIVE_ALWAYS)
		keepalive_active = 1;

	જ્યારે (1) {
		ssize_t sz;

		જો (keepalive_active) {
			સંરચના pollfd pfd;
			પૂર્ણાંક ret;

			pfd.fd = in;
			pfd.events = POLLIN;
			ret = poll(&pfd, 1, 1000 * keepalive_in_sec);

			જો (ret < 0) {
				જો (errno == EINTR)
					ચાલુ;
				નહીંતર
					તોડો;
			} નહીંતર જો (ret == 0) {
				/* no data; send a keepalive packet */
				સ્થિર અચલ અક્ષર buf[] = "0005\1";
				write_or_die(1, buf, કદ(buf) - 1);
				ચાલુ;
			} /* else there is actual data to read */
		}

		sz = xread(in, data, કદ(data));
		જો (sz <= 0)
			તોડો;

		જો (use_keepalive == KEEPALIVE_AFTER_NUL && !keepalive_active) {
			અચલ અક્ષર *p = memchr(data, '\0', sz);
			જો (p) {
				/*
				 * The NUL tells us to start sending keepalives. Make
				 * sure we send any other data we read along
				 * with it.
				 */
				keepalive_active = 1;
				send_sideband(1, 2, data, p - data, use_sideband);
				send_sideband(1, 2, p + 1, sz - (p - data + 1), use_sideband);
				ચાલુ;
			}
		}

		/*
		 * Either we're not looking for a NUL signal, or we didn't see
		 * it yet; just pass along the data.
		 */
		send_sideband(1, 2, data, sz, use_sideband);
	}
	close(in);
	પાછા 0;
}

/*
 * Start an async thread which redirects hook stderr over the sideband.
 * The original stderr fd is saved to `saved_stderr` and STDERR_FILENO is
 * redirected to the async's input pipe.
 */
સ્થિર શૂન્ય prepare_sideband_async(સંરચના async *sideband_async, પૂર્ણાંક *saved_stderr, પૂર્ણાંક *started)
{
	*started = 0;

	જો (!use_sideband)
		પાછા;

	memset(sideband_async, 0, કદ(*sideband_async));
	sideband_async->proc = copy_to_sideband;
	sideband_async->in = -1;

	જો (!start_async(sideband_async)) {
		*started = 1;
		*saved_stderr = dup(STDERR_FILENO);
		જો (*saved_stderr >= 0)
			dup2(sideband_async->in, STDERR_FILENO);
		close(sideband_async->in);
	}
}

/*
 * Restore the original stderr and wait for the async sideband thread to finish.
 */
સ્થિર શૂન્ય finish_sideband_async(સંરચના async *sideband_async, પૂર્ણાંક saved_stderr, પૂર્ણાંક started)
{
	જો (!use_sideband)
		પાછા;

	જો (saved_stderr >= 0) {
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}

	જો (started)
		finish_async(sideband_async);
}

સ્થિર શૂન્ય hmac_hash(unsigned અક્ષર *out,
		      અચલ અક્ષર *ચાવી_in, size_t ચાવી_len,
		      અચલ અક્ષર *text, size_t text_len)
{
	unsigned અક્ષર ચાવી[GIT_MAX_BLKSZ];
	unsigned અક્ષર k_ipad[GIT_MAX_BLKSZ];
	unsigned અક્ષર k_opad[GIT_MAX_BLKSZ];
	પૂર્ણાંક i;
	સંરચના git_hash_ctx ctx;

	/* RFC 2104 2. (1) */
	memset(ચાવી, '\0', GIT_MAX_BLKSZ);
	જો (the_hash_algo->blksz < ચાવી_len) {
		git_hash_init(&ctx, the_hash_algo);
		git_hash_update(&ctx, ચાવી_in, ચાવી_len);
		git_hash_final(ચાવી, &ctx);
	} નહીંતર {
		memcpy(ચાવી, ચાવી_in, ચાવી_len);
	}

	/* RFC 2104 2. (2) & (5) */
	માટે (i = 0; i < કદ(ચાવી); i++) {
		k_ipad[i] = ચાવી[i] ^ 0x36;
		k_opad[i] = ચાવી[i] ^ 0x5c;
	}

	/* RFC 2104 2. (3) & (4) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_ipad, કદ(k_ipad));
	git_hash_update(&ctx, text, text_len);
	git_hash_final(out, &ctx);

	/* RFC 2104 2. (6) & (7) */
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, k_opad, કદ(k_opad));
	git_hash_update(&ctx, out, the_hash_algo->rawsz);
	git_hash_final(out, &ctx);
}

સ્થિર અક્ષર *prepare_ધકેલો_cert_nonce(અચલ અક્ષર *path, timestamp_t stamp)
{
	સંરચના strbuf buf = STRBUF_INIT;
	unsigned અક્ષર hash[GIT_MAX_RAWSZ];

	strbuf_addf(&buf, "%s:%"PRItime, path, stamp);
	hmac_hash(hash, buf.buf, buf.len, cert_nonce_seed, strlen(cert_nonce_seed));
	strbuf_release(&buf);

	/* RFC 2104 5. HMAC-SHA1 or HMAC-SHA256 */
	strbuf_addf(&buf, "%"PRItime"-%.*s", stamp, (પૂર્ણાંક)the_hash_algo->hexsz, hash_to_hex(hash));
	પાછા strbuf_detach(&buf, ખાલી);
}

/*
 * Return zero if a and b are equal up to n bytes and nonzero if they are not.
 * This operation is guaranteed to run in constant time to avoid leaking data.
 */
સ્થિર પૂર્ણાંક constant_memequal(અચલ અક્ષર *a, અચલ અક્ષર *b, size_t n)
{
	પૂર્ણાંક res = 0;
	size_t i;

	માટે (i = 0; i < n; i++)
		res |= a[i] ^ b[i];
	પાછા res;
}

સ્થિર અચલ અક્ષર *check_nonce(અચલ અક્ષર *buf)
{
	size_t noncelen;
	અચલ અક્ષર *found = find_commit_header(buf, "nonce", &noncelen);
	અક્ષર *nonce = found ? xmemdupz(found, noncelen) : ખાલી;
	timestamp_t stamp, ostamp;
	અક્ષર *bohmac, *expect = ખાલી;
	અચલ અક્ષર *retval = NONCE_BAD;

	જો (!nonce) {
		retval = NONCE_MISSING;
		જાઓ leave;
	} નહીંતર જો (!ધકેલો_cert_nonce) {
		retval = NONCE_UNSOLICITED;
		જાઓ leave;
	} નહીંતર જો (!strcmp(ધકેલો_cert_nonce, nonce)) {
		retval = NONCE_OK;
		જાઓ leave;
	}

	જો (!stateless_rpc) {
		/* returned nonce MUST match what we gave out earlier */
		retval = NONCE_BAD;
		જાઓ leave;
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
	જો (*nonce <= '0' || '9' < *nonce) {
		retval = NONCE_BAD;
		જાઓ leave;
	}
	stamp = parse_timestamp(nonce, &bohmac, 10);
	જો (bohmac == nonce || bohmac[0] != '-') {
		retval = NONCE_BAD;
		જાઓ leave;
	}

	expect = prepare_ધકેલો_cert_nonce(service_dir, stamp);
	જો (noncelen != strlen(expect)) {
		/* This is not even the right size. */
		retval = NONCE_BAD;
		જાઓ leave;
	}
	જો (constant_memequal(expect, nonce, noncelen)) {
		/* Not what we would have signed earlier */
		retval = NONCE_BAD;
		જાઓ leave;
	}

	/*
	 * By how many seconds is this nonce stale?  Negative value
	 * would mean it was issued by another server with its clock
	 * skewed in the future.
	 */
	ostamp = parse_timestamp(ધકેલો_cert_nonce, ખાલી, 10);
	nonce_stamp_slop = (long)ostamp - (long)stamp;

	જો (nonce_stamp_slop_limit &&
	    labs(nonce_stamp_slop) <= nonce_stamp_slop_limit) {
		/*
		 * Pretend as if the received nonce (which passes the
		 * HMAC check, so it is not a forged by third-party)
		 * is what we issued.
		 */
		મુક્ત((શૂન્ય *)ધકેલો_cert_nonce);
		ધકેલો_cert_nonce = xstrdup(nonce);
		retval = NONCE_OK;
	} નહીંતર {
		retval = NONCE_SLOP;
	}

leave:
	મુક્ત(nonce);
	મુક્ત(expect);
	પાછા retval;
}

/*
 * Return 1 if there is no push_cert or if the push options in push_cert are
 * the same as those in the argument; 0 otherwise.
 */
સ્થિર પૂર્ણાંક check_cert_ધકેલો_options(અચલ સંરચના string_list *ધકેલો_options)
{
	અચલ અક્ષર *buf = ધકેલો_cert.buf;

	અચલ અક્ષર *option;
	size_t optionlen;
	પૂર્ણાંક options_seen = 0;

	પૂર્ણાંક retval = 1;

	જો (!*buf)
		પાછા 1;

	જ્યારે ((option = find_commit_header(buf, "push-option", &optionlen))) {
		buf = option + optionlen + 1;
		options_seen++;
		જો (options_seen > ધકેલો_options->nr
		    || xstrncmpz(ધકેલો_options->વસ્તુઓ[options_seen - 1].string,
				 option, optionlen))
			પાછા 0;
	}

	જો (options_seen != ધકેલો_options->nr)
		retval = 0;

	પાછા retval;
}

સ્થિર શૂન્ય prepare_ધકેલો_cert_sha1(સંરચના run_hooks_opt *opt)
{
	સ્થિર પૂર્ણાંક already_done;

	જો (!ધકેલો_cert.len)
		પાછા;

	જો (!already_done) {
		પૂર્ણાંક bogs /* beginning_of_gpg_sig */;

		already_done = 1;
		જો (odb_write_object(the_repository->objects, ધકેલો_cert.buf,
				     ધકેલો_cert.len, OBJ_BLOB, &ધકેલો_cert_oid))
			oidclr(&ધકેલો_cert_oid, the_repository->hash_algo);

		memset(&sigcheck, '\0', કદ(sigcheck));

		bogs = parse_signed_buffer(ધકેલો_cert.buf, ધકેલો_cert.len);
		sigcheck.payload = xmemdupz(ધકેલો_cert.buf, bogs);
		sigcheck.payload_len = bogs;
		check_signature(&sigcheck, ધકેલો_cert.buf + bogs,
				ધકેલો_cert.len - bogs);

		nonce_status = check_nonce(sigcheck.payload);
	}
	જો (!is_null_oid(&ધકેલો_cert_oid)) {
		strvec_pushf(&opt->env, "GIT_PUSH_CERT=%s",
			     oid_to_hex(&ધકેલો_cert_oid));
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_SIGNER=%s",
			     sigcheck.signer ? sigcheck.signer : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_KEY=%s",
			     sigcheck.ચાવી ? sigcheck.ચાવી : "");
		strvec_pushf(&opt->env, "GIT_PUSH_CERT_STATUS=%c",
			     sigcheck.result);
		જો (ધકેલો_cert_nonce) {
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE=%s",
				     ધકેલો_cert_nonce);
			strvec_pushf(&opt->env,
				     "GIT_PUSH_CERT_NONCE_STATUS=%s",
				     nonce_status);
			જો (nonce_status == NONCE_SLOP)
				strvec_pushf(&opt->env,
					     "GIT_PUSH_CERT_NONCE_SLOP=%ld",
					     nonce_stamp_slop);
		}
	}
}

સંરચના receive_hook_feed_state {
	સંરચના command *cmd;
	સંરચના ref_ધકેલો_report *report;
	પૂર્ણાંક skip_broken;
	સંરચના strbuf buf;
};

સ્થિર પૂર્ણાંક feed_receive_hook_cb(પૂર્ણાંક hook_stdin_fd, શૂન્ય *pp_cb UNUSED, શૂન્ય *pp_task_cb)
{
	સંરચના receive_hook_feed_state *state = pp_task_cb;
	સંરચના command *cmd = state->cmd;

	strbuf_reset(&state->buf);

	જ્યારે (cmd &&
	       state->skip_broken && (cmd->error_string || cmd->did_not_exist))
		cmd = cmd->next;

	જો (!cmd)
		પાછા 1;  /* no more commands left */

	જો (!state->report)
		state->report = cmd->report;

	જો (state->report) {
		સંરચના object_id *old_oid;
		સંરચના object_id *new_oid;
		અચલ અક્ષર *ref_name;

		old_oid = state->report->old_oid ? state->report->old_oid : &cmd->old_oid;
		new_oid = state->report->new_oid ? state->report->new_oid : &cmd->new_oid;
		ref_name = state->report->ref_name ? state->report->ref_name : cmd->ref_name;

		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(old_oid), oid_to_hex(new_oid),
			    ref_name);

		state->report = state->report->next;
		જો (!state->report)
			cmd = cmd->next;
	} નહીંતર {
		strbuf_addf(&state->buf, "%s %s %s\n",
			    oid_to_hex(&cmd->old_oid), oid_to_hex(&cmd->new_oid),
			    cmd->ref_name);
		cmd = cmd->next;
	}

	state->cmd = cmd;

	જો (state->buf.len > 0) {
		પૂર્ણાંક ret = write_in_full(hook_stdin_fd, state->buf.buf, state->buf.len);
		જો (ret < 0) {
			જો (errno == EPIPE)
				પાછા 1; /* child closed pipe */
			પાછા ret;
		}
	}

	પાછા state->cmd ? 0 : 1;  /* 0 = more to come, 1 = EOF */
}

સ્થિર શૂન્ય *receive_hook_feed_state_alloc(શૂન્ય *feed_pipe_ctx)
{
	સંરચના receive_hook_feed_state *init_state = feed_pipe_ctx;
	સંરચના receive_hook_feed_state *data;

	CALLOC_ARRAY(data, 1);
	data->report = init_state->report;
	data->cmd = init_state->cmd;
	data->skip_broken = init_state->skip_broken;
	strbuf_init(&data->buf, 0);

	પાછા data;
}

સ્થિર શૂન્ય receive_hook_feed_state_free(શૂન્ય *data)
{
	સંરચના receive_hook_feed_state *d = data;
	જો (!d)
		પાછા;
	strbuf_release(&d->buf);
	મુક્ત(d);
}

સ્થિર પૂર્ણાંક run_receive_hook(સંરચના command *commands,
			    અચલ અક્ષર *hook_name,
			    પૂર્ણાંક skip_broken,
			    સંરચના odb_transaction *transaction,
			    અચલ સંરચના string_list *ધકેલો_options)
{
	સંરચના run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	સંરચના command *iter = commands;
	સંરચના receive_hook_feed_state feed_init_state = {
		.cmd = commands,
		.skip_broken = skip_broken,
		.buf = STRBUF_INIT,
	};
	સંરચના async sideband_async;
	પૂર્ણાંક sideband_async_started = 0;
	પૂર્ણાંક saved_stderr = -1;
	પૂર્ણાંક ret;

	જો (!hook_exists(the_repository, hook_name))
		પાછા 0;

	/* if there are no valid commands, don't invoke the hook at all. */
	જ્યારે (iter && skip_broken && (iter->error_string || iter->did_not_exist))
		iter = iter->next;
	જો (!iter)
		પાછા 0;

	જો (ધકેલો_options) {
		માટે (પૂર્ણાંક i = 0; i < ધકેલો_options->nr; i++)
			strvec_pushf(&opt.env, "GIT_PUSH_OPTION_%d=%s", i,
				     ધકેલો_options->વસ્તુઓ[i].string);
		strvec_pushf(&opt.env, "GIT_PUSH_OPTION_COUNT=%"PRIuMAX"",
					     (uintmax_t)ધકેલો_options->nr);
	} નહીંતર {
		strvec_ધકેલો(&opt.env, "GIT_PUSH_OPTION_COUNT");
	}

	જો (transaction)
		odb_transaction_env(transaction, &opt.env);

	prepare_ધકેલો_cert_sha1(&opt);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	/* set up stdin callback */
	opt.feed_pipe_ctx = &feed_init_state;
	opt.feed_pipe = feed_receive_hook_cb;
	opt.feed_pipe_cb_data_alloc = receive_hook_feed_state_alloc;
	opt.feed_pipe_cb_data_free = receive_hook_feed_state_free;

	ret = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	પાછા ret;
}

સ્થિર પૂર્ણાંક run_update_hook(સંરચના command *cmd)
{
	સ્થિર અચલ અક્ષર hook_name[] = "update";
	સંરચના run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	સંરચના async sideband_async;
	પૂર્ણાંક sideband_async_started = 0;
	પૂર્ણાંક saved_stderr = -1;
	પૂર્ણાંક code;

	જો (!hook_exists(the_repository, hook_name))
		પાછા 0;

	strvec_pushl(&opt.args,
		     cmd->ref_name,
		     oid_to_hex(&cmd->old_oid),
		     oid_to_hex(&cmd->new_oid),
		     ખાલી);

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	code = run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);

	પાછા code;
}

સ્થિર પૂર્ણાંક run_receive_report_hook(સંરચના strbuf *report)
{
	સંરચના child_process proc = CHILD_PROCESS_INIT;
	સંરચના async sideband_async;
	પૂર્ણાંક sideband_async_started = 0;
	પૂર્ણાંક saved_stderr = -1;
	સંરચના strbuf out = STRBUF_INIT;
	અચલ અક્ષર *hook_path;
	પૂર્ણાંક ret;

	hook_path = find_hook(the_repository, "receive-report");
	જો (!hook_path)
		પાછા 0;

	strvec_ધકેલો(&proc.args, hook_path);
	proc.trace2_hook_name = "receive-report";

	prepare_sideband_async(&sideband_async, &saved_stderr,
			       &sideband_async_started);

	sigchain_ધકેલો(SIGPIPE, SIG_IGN);
	ret = pipe_command(&proc, report->buf, report->len, &out,
			   report->len, ખાલી, 0);
	sigchain_કાઢો(SIGPIPE);

	finish_sideband_async(&sideband_async, saved_stderr,
			      sideband_async_started);

	જો (!ret)
		strbuf_swap(&out, report);

	strbuf_release(&out);
	પાછા ret;
}

સ્થિર સંરચના command *find_command_by_refname(સંરચના command *list,
					       અચલ અક્ષર *refname)
{
	માટે (; list; list = list->next)
		જો (!strcmp(list->ref_name, refname))
			પાછા list;
	પાછા ખાલી;
}

સ્થિર પૂર્ણાંક read_proc_receive_report(સંરચના packet_reader *reader,
				    સંરચના command *commands,
				    સંરચના strbuf *errmsg)
{
	સંરચના command *cmd;
	સંરચના command *hint = ખાલી;
	સંરચના ref_ધકેલો_report *report = ખાલી;
	પૂર્ણાંક new_report = 0;
	પૂર્ણાંક code = 0;
	પૂર્ણાંક once = 0;
	પૂર્ણાંક response = 0;

	માટે (;;) {
		સંરચના object_id old_oid, new_oid;
		અક્ષર *head;
		અક્ષર *refname;
		અક્ષર *p;
		ગણના packet_read_status status;

		status = packet_reader_read(reader);
		જો (status != PACKET_READ_NORMAL) {
			/* Check whether proc-receive exited abnormally */
			જો (status == PACKET_READ_EOF && !response) {
				strbuf_addstr(errmsg, "proc-receive exited abnormally");
				પાછા -1;
			}
			તોડો;
		}
		response++;

		head = reader->લીટી;
		p = strchr(head, ' ');
		જો (!p) {
			strbuf_addf(errmsg, "proc-receive reported incomplete status line: '%s'\n", head);
			code = -1;
			ચાલુ;
		}
		*p++ = '\0';
		જો (!strcmp(head, "option")) {
			અક્ષર *ચાવી;
			અચલ અક્ષર *val;

			જો (!hint || !(report || new_report)) {
				જો (!once++)
					strbuf_addstr(errmsg, "proc-receive reported 'option' without a matching 'ok/ng' directive\n");
				code = -1;
				ચાલુ;
			}
			જો (new_report) {
				જો (!hint->report) {
					CALLOC_ARRAY(hint->report, 1);
					report = hint->report;
				} નહીંતર {
					report = hint->report;
					જ્યારે (report->next)
						report = report->next;
					report->next = xcalloc(1, કદ(સંરચના ref_ધકેલો_report));
					report = report->next;
				}
				new_report = 0;
			}
			ચાવી = p;
			p = strchr(ચાવી, ' ');
			જો (p)
				*p++ = '\0';
			val = p;
			જો (!strcmp(ચાવી, "refname"))
				report->ref_name = xstrdup_or_null(val);
			નહીંતર જો (!strcmp(ચાવી, "old-oid") && val &&
				 !parse_oid_hex(val, &old_oid, &val))
				report->old_oid = oiddup(&old_oid);
			નહીંતર જો (!strcmp(ચાવી, "new-oid") && val &&
				 !parse_oid_hex(val, &new_oid, &val))
				report->new_oid = oiddup(&new_oid);
			નહીંતર જો (!strcmp(ચાવી, "forced-update"))
				report->forced_update = 1;
			નહીંતર જો (!strcmp(ચાવી, "fall-through"))
				/* Fall through, let 'receive-pack' to execute it. */
				hint->run_proc_receive = 0;
			ચાલુ;
		}

		report = ખાલી;
		new_report = 0;
		refname = p;
		p = strchr(refname, ' ');
		જો (p)
			*p++ = '\0';
		જો (strcmp(head, "ok") && strcmp(head, "ng")) {
			strbuf_addf(errmsg, "proc-receive reported bad status '%s' on ref '%s'\n",
				    head, refname);
			code = -1;
			ચાલુ;
		}

		/* first try searching at our hint, falling back to all refs */
		જો (hint)
			hint = find_command_by_refname(hint, refname);
		જો (!hint)
			hint = find_command_by_refname(commands, refname);
		જો (!hint) {
			strbuf_addf(errmsg, "proc-receive reported status on unknown ref: %s\n",
				    refname);
			code = -1;
			ચાલુ;
		}
		જો (!hint->run_proc_receive) {
			strbuf_addf(errmsg, "proc-receive reported status on unexpected ref: %s\n",
				    refname);
			code = -1;
			ચાલુ;
		}
		hint->run_proc_receive |= RUN_PROC_RECEIVE_RETURNED;
		જો (!strcmp(head, "ng")) {
			જો (p)
				hint->error_string = hint->error_string_owned = xstrdup(p);
			નહીંતર
				hint->error_string = "failed";
			code = -1;
			ચાલુ;
		}
		new_report = 1;
	}

	માટે (cmd = commands; cmd; cmd = cmd->next)
		જો (cmd->run_proc_receive && !cmd->error_string &&
		    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED)) {
		    cmd->error_string = "proc-receive failed to report status";
		    code = -1;
		}
	પાછા code;
}

સ્થિર પૂર્ણાંક run_proc_receive_hook(સંરચના command *commands,
				 અચલ સંરચના string_list *ધકેલો_options)
{
	સંરચના child_process proc = CHILD_PROCESS_INIT;
	સંરચના async muxer;
	સંરચના command *cmd;
	સંરચના packet_reader reader;
	સંરચના strbuf cap = STRBUF_INIT;
	સંરચના strbuf errmsg = STRBUF_INIT;
	પૂર્ણાંક hook_use_ધકેલો_options = 0;
	પૂર્ણાંક version = 0;
	પૂર્ણાંક code;
	અચલ અક્ષર *hook_path = find_hook(the_repository, "proc-receive");

	જો (!hook_path) {
		rp_error("cannot find hook 'proc-receive'");
		પાછા -1;
	}

	strvec_ધકેલો(&proc.args, hook_path);
	proc.in = -1;
	proc.out = -1;
	proc.trace2_hook_name = "proc-receive";

	જો (use_sideband) {
		memset(&muxer, 0, કદ(muxer));
		muxer.proc = copy_to_sideband;
		muxer.in = -1;
		code = start_async(&muxer);
		જો (code)
			પાછા code;
		proc.err = muxer.in;
	} નહીંતર {
		proc.err = 0;
	}

	code = start_command(&proc);
	જો (code) {
		જો (use_sideband)
			finish_async(&muxer);
		પાછા code;
	}

	sigchain_ધકેલો(SIGPIPE, SIG_IGN);

	/* Version negotiaton */
	packet_reader_init(&reader, proc.out, ખાલી, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF);
	જો (use_atomic)
		strbuf_addstr(&cap, " atomic");
	જો (use_ધકેલો_options)
		strbuf_addstr(&cap, " push-options");
	જો (cap.len) {
		code = packet_write_fmt_gently(proc.in, "version=1%c%s\n", '\0', cap.buf + 1);
		strbuf_release(&cap);
	} નહીંતર {
		code = packet_write_fmt_gently(proc.in, "version=1\n");
	}
	જો (!code)
		code = packet_flush_gently(proc.in);

	જો (!code)
		માટે (;;) {
			પૂર્ણાંક linelen;
			ગણના packet_read_status status;

			status = packet_reader_read(&reader);
			જો (status != PACKET_READ_NORMAL) {
				/* Check whether proc-receive exited abnormally */
				જો (status == PACKET_READ_EOF)
					code = -1;
				તોડો;
			}

			જો (reader.pktlen > 8 && starts_with(reader.લીટી, "version=")) {
				version = atoi(reader.લીટી + 8);
				linelen = strlen(reader.લીટી);
				જો (linelen < reader.pktlen) {
					અચલ અક્ષર *feature_list = reader.લીટી + linelen + 1;
					જો (parse_feature_request(feature_list, "push-options"))
						hook_use_ધકેલો_options = 1;
				}
			}
		}

	જો (code) {
		strbuf_addstr(&errmsg, "fail to negotiate version with proc-receive hook");
		જાઓ cleanup;
	}

	પસંદગી (version) {
	સ્થિતિ 0:
		/* fallthrough */
	સ્થિતિ 1:
		તોડો;
	મૂળભૂત:
		strbuf_addf(&errmsg, "proc-receive version '%d' is not supported",
			    version);
		code = -1;
		જાઓ cleanup;
	}

	/* Send commands */
	માટે (cmd = commands; cmd; cmd = cmd->next) {
		જો (!cmd->run_proc_receive || cmd->skip_update || cmd->error_string)
			ચાલુ;
		code = packet_write_fmt_gently(proc.in, "%s %s %s",
					       oid_to_hex(&cmd->old_oid),
					       oid_to_hex(&cmd->new_oid),
					       cmd->ref_name);
		જો (code)
			તોડો;
	}
	જો (!code)
		code = packet_flush_gently(proc.in);
	જો (code) {
		strbuf_addstr(&errmsg, "fail to write commands to proc-receive hook");
		જાઓ cleanup;
	}

	/* Send push options */
	જો (hook_use_ધકેલો_options) {
		સંરચના string_list_item *item;

		for_each_string_list_item(item, ધકેલો_options) {
			code = packet_write_fmt_gently(proc.in, "%s", item->string);
			જો (code)
				તોડો;
		}
		જો (!code)
			code = packet_flush_gently(proc.in);
		જો (code) {
			strbuf_addstr(&errmsg,
				      "fail to write push-options to proc-receive hook");
			જાઓ cleanup;
		}
	}

	/* Read result from proc-receive */
	code = read_proc_receive_report(&reader, commands, &errmsg);

cleanup:
	close(proc.in);
	close(proc.out);
	જો (use_sideband)
		finish_async(&muxer);
	જો (finish_command(&proc))
		code = -1;
	જો (errmsg.len >0) {
		અક્ષર *p = errmsg.buf;

		p += errmsg.len - 1;
		જો (*p == '\n')
			*p = '\0';
		rp_error("%s", errmsg.buf);
		strbuf_release(&errmsg);
	}
	sigchain_કાઢો(SIGPIPE);

	પાછા code;
}

સ્થિર અચલ અક્ષર *refuse_unconfigured_deny_સંદેશ =
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

સ્થિર શૂન્ય refuse_unconfigured_deny(શૂન્ય)
{
	rp_error("%s", _(refuse_unconfigured_deny_સંદેશ));
}

સ્થિર અચલ અક્ષર *refuse_unconfigured_deny_delete_current_સંદેશ =
	N_("By default, deleting the current branch is denied, because the next\n"
	   "'git clone' won't result in any file checked out, causing confusion.\n"
	   "\n"
	   "You can set 'receive.denyDeleteCurrent' configuration variable to\n"
	   "'warn' or 'ignore' in the remote repository to allow deleting the\n"
	   "current branch, with or without a warning message.\n"
	   "\n"
	   "To squelch this message, you can set it to 'refuse'.");

સ્થિર શૂન્ય refuse_unconfigured_deny_delete_current(શૂન્ય)
{
	rp_error("%s", _(refuse_unconfigured_deny_delete_current_સંદેશ));
}

સ્થિર અચલ સંરચના object_id *command_singleton_iterator(શૂન્ય *cb_data);
સ્થિર પૂર્ણાંક update_shallow_ref(સંરચના command *cmd, સંરચના shallow_info *si)
{
	સંરચના shallow_lock shallow_lock = SHALLOW_LOCK_INIT;
	સંરચના oid_array extra = OID_ARRAY_INIT;
	સંરચના check_connected_options opt = CHECK_CONNECTED_INIT;
	uint32_t mask = 1 << (cmd->index % 32);
	પૂર્ણાંક i;

	trace_printf_ચાવી(&trace_shallow,
			 "shallow: update_shallow_ref %s\n", cmd->ref_name);
	માટે (i = 0; i < si->shallow->nr; i++)
		જો (si->used_shallow[i] &&
		    (si->used_shallow[i][cmd->index / 32] & mask) &&
		    !delayed_reachability_test(si, i))
			oid_array_append(&extra, &si->shallow->oid[i]);

	setup_alternate_shallow(&shallow_lock, &opt.shallow_file, &extra);
	જો (check_connected(command_singleton_iterator, cmd, &opt)) {
		rollback_shallow_file(the_repository, &shallow_lock);
		oid_array_clear(&extra);
		પાછા -1;
	}

	commit_shallow_file(the_repository, &shallow_lock);

	/*
	 * Make sure setup_alternate_shallow() for the next ref does
	 * not lose these new roots..
	 */
	માટે (i = 0; i < extra.nr; i++)
		register_shallow(the_repository, &extra.oid[i]);

	si->shallow_ref[cmd->index] = 0;
	oid_array_clear(&extra);
	પાછા 0;
}

સ્થિર અચલ અક્ષર *ધકેલો_to_deploy(unsigned અક્ષર *sha1,
				  સંરચના strvec *env,
				  અચલ સંરચના worktree *worktree)
{
	સંરચના child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "update-index", "-q", "--ignore-submodules",
		     "--refresh", ખાલી);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	જો (run_command(&child))
		પાછા "Up-to-date check failed";

	/* run_command() does not clean up completely; reinitialize */
	child_process_init(&child);
	strvec_pushl(&child.args, "diff-files", "--quiet",
		     "--ignore-submodules", "--", ખાલી);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.stdout_to_stderr = 1;
	child.git_cmd = 1;
	જો (run_command(&child))
		પાછા "Working directory has unstaged changes";

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
		     empty_વૃક્ષ_oid_hex(the_repository->hash_algo), "--", ખાલી);
	strvec_pushv(&child.env, env->v);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	જો (run_command(&child))
		પાછા "Working directory has staged changes";

	child_process_init(&child);
	strvec_pushl(&child.args, "read-tree", "-u", "-m", hash_to_hex(sha1),
		     ખાલી);
	strvec_pushv(&child.env, env->v);
	child.dir = worktree->path;
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.stdout_to_stderr = 0;
	child.git_cmd = 1;
	જો (run_command(&child))
		પાછા "Could not update working tree to new HEAD";

	પાછા ખાલી;
}

સ્થિર અચલ અક્ષર *ધકેલો_to_checkout_hook = "push-to-checkout";

સ્થિર અચલ અક્ષર *ધકેલો_to_checkout(unsigned અક્ષર *hash,
				    પૂર્ણાંક *invoked_hook,
				    સંરચના strvec *env,
				    અચલ અક્ષર *work_વૃક્ષ)
{
	સંરચના run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	opt.invoked_hook = invoked_hook;

	strvec_pushv(&opt.env, env->v);
	strvec_pushf(&opt.env, "GIT_WORK_TREE=%s", absolute_path(work_વૃક્ષ));
	strvec_ધકેલો(&opt.args, hash_to_hex(hash));
	જો (run_hooks_opt(the_repository, ધકેલો_to_checkout_hook, &opt))
		પાછા "push-to-checkout hook declined";
	નહીંતર
		પાછા ખાલી;
}

સ્થિર અચલ અક્ષર *update_worktree(unsigned અક્ષર *sha1, અચલ સંરચના worktree *worktree)
{
	અચલ અક્ષર *retval;
	અક્ષર *git_dir;
	સંરચના strvec env = STRVEC_INIT;
	પૂર્ણાંક invoked_hook;

	જો (!worktree || !worktree->path)
		BUG("worktree->path must be non-NULL");

	જો (worktree->is_bare)
		પાછા "denyCurrentBranch = updateInstead needs a worktree";
	git_dir = get_worktree_git_dir(worktree);

	strvec_pushf(&env, "GIT_DIR=%s", absolute_path(git_dir));

	retval = ધકેલો_to_checkout(sha1, &invoked_hook, &env, worktree->path);
	જો (!invoked_hook)
		retval = ધકેલો_to_deploy(sha1, &env, worktree);

	strvec_clear(&env);
	મુક્ત(git_dir);
	પાછા retval;
}

સ્થિર અચલ અક્ષર *update(સંરચના command *cmd, સંરચના shallow_info *si)
{
	અચલ અક્ષર *name = cmd->ref_name;
	સંરચના strbuf namespaced_name_buf = STRBUF_INIT;
	સ્થિર અક્ષર *namespaced_name;
	અચલ અક્ષર *ret;
	સંરચના object_id *old_oid = &cmd->old_oid;
	સંરચના object_id *new_oid = &cmd->new_oid;
	પૂર્ણાંક do_update_worktree = 0;
	સંરચના worktree **worktrees = get_worktrees(the_repository);
	અચલ સંરચના worktree *worktree =
		find_shared_symref(worktrees, "HEAD", name);

	/* only refs/... are allowed */
	જો (!starts_with(name, "refs/") ||
	    check_refname_format(name + 5, is_null_oid(new_oid) ?
				 REFNAME_ALLOW_ONELEVEL : 0)) {
		rp_error("refusing to update funny ref '%s' remotely", name);
		ret = "funny refname";
		જાઓ out;
	}

	strbuf_addf(&namespaced_name_buf, "%s%s", get_git_namespace(), name);
	મુક્ત(namespaced_name);
	namespaced_name = strbuf_detach(&namespaced_name_buf, ખાલી);

	જો (worktree && !worktree->is_bare) {
		પસંદગી (deny_current_branch) {
		સ્થિતિ DENY_IGNORE:
			તોડો;
		સ્થિતિ DENY_WARN:
			rp_warning("updating the current branch");
			તોડો;
		સ્થિતિ DENY_REFUSE:
		સ્થિતિ DENY_UNCONFIGURED:
			rp_error("refusing to update checked out branch: %s", name);
			જો (deny_current_branch == DENY_UNCONFIGURED)
				refuse_unconfigured_deny();
			ret = "branch is currently checked out";
			જાઓ out;
		સ્થિતિ DENY_UPDATE_INSTEAD:
			/* pass -- let other checks intervene first */
			do_update_worktree = 1;
			તોડો;
		}
	}

	જો (!is_null_oid(new_oid) &&
	    !odb_has_object(the_repository->objects, new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR)) {
		error("unpack should have generated %s, "
		      "but I can't find it!", oid_to_hex(new_oid));
		ret = "bad pack";
		જાઓ out;
	}

	જો (!is_null_oid(old_oid) && is_null_oid(new_oid)) {
		જો (deny_deletes && starts_with(name, "refs/heads/")) {
			rp_error("denying ref deletion for %s", name);
			ret = "deletion prohibited";
			જાઓ out;
		}

		જો (worktree || (head_name && !strcmp(namespaced_name, head_name))) {
			પસંદગી (deny_delete_current) {
			સ્થિતિ DENY_IGNORE:
				તોડો;
			સ્થિતિ DENY_WARN:
				rp_warning("deleting the current branch");
				તોડો;
			સ્થિતિ DENY_REFUSE:
			સ્થિતિ DENY_UNCONFIGURED:
			સ્થિતિ DENY_UPDATE_INSTEAD:
				જો (deny_delete_current == DENY_UNCONFIGURED)
					refuse_unconfigured_deny_delete_current();
				rp_error("refusing to delete the current branch: %s", name);
				ret = "deletion of the current branch prohibited";
				જાઓ out;
			મૂળભૂત:
				ret = "Invalid denyDeleteCurrent setting";
				જાઓ out;
			}
		}
	}

	જો (deny_non_fast_forwards && !is_null_oid(new_oid) &&
	    !is_null_oid(old_oid) &&
	    starts_with(name, "refs/heads/")) {
		સંરચના object *old_object, *new_object;
		સંરચના commit *old_commit, *new_commit;
		પૂર્ણાંક ret2;

		old_object = parse_object(the_repository, old_oid);
		new_object = parse_object(the_repository, new_oid);

		જો (!old_object || !new_object ||
		    old_object->type != OBJ_COMMIT ||
		    new_object->type != OBJ_COMMIT) {
			error("bad sha1 objects for %s", name);
			ret = "bad ref";
			જાઓ out;
		}
		old_commit = (સંરચના commit *)old_object;
		new_commit = (સંરચના commit *)new_object;
		ret2 = repo_in_merge_bases(the_repository, old_commit, new_commit);
		જો (ret2 < 0)
			exit(128);
		જો (!ret2) {
			rp_error("denying non-fast-forward %s"
				 " (you should pull first)", name);
			ret = "non-fast-forward";
			જાઓ out;
		}
	}
	જો (run_update_hook(cmd)) {
		rp_error("hook declined to update %s", name);
		ret = "hook declined";
		જાઓ out;
	}

	જો (do_update_worktree) {
		ret = update_worktree(new_oid->hash, worktree);
		જો (ret)
			જાઓ out;
	}

	જો (is_null_oid(new_oid)) {
		સંરચના strbuf err = STRBUF_INIT;
		જો (!parse_object(the_repository, old_oid)) {
			old_oid = ખાલી;
			જો (refs_ref_exists(get_main_ref_store(the_repository), name)) {
				rp_warning("allowing deletion of corrupt ref");
			} નહીંતર {
				rp_warning("deleting a non-existent ref");
				cmd->did_not_exist = 1;
			}
		}
		જો (ref_transaction_delete(transaction,
					   namespaced_name,
					   old_oid,
					   ખાલી, 0,
					   "push", &err)) {
			rp_error("%s", err.buf);
			ret = "failed to delete";
		} નહીંતર {
			ret = ખાલી; /* good */
		}
		strbuf_release(&err);
	} નહીંતર {
		ગણના ref_transaction_error tx_err;
		સંરચના strbuf err = STRBUF_INIT;
		જો (shallow_update && si->shallow_ref[cmd->index] &&
		    update_shallow_ref(cmd, si)) {
			ret = "shallow error";
			જાઓ out;
		}

		tx_err = ref_transaction_update(transaction,
						  namespaced_name,
						  new_oid, old_oid,
						  ખાલી, ખાલી,
						  0, "push",
						  &err);
		જો (tx_err) {
			rp_error("%s", err.buf);
			જો (tx_err == REF_TRANSACTION_ERROR_GENERIC)
				ret = "failed to update ref";
			નહીંતર
				ret = ref_transaction_error_સંદેશ(tx_err);
		} નહીંતર {
			ret = ખાલી; /* good */
		}
		strbuf_release(&err);
	}

out:
	free_worktrees(worktrees);
	પાછા ret;
}

સ્થિર શૂન્ય run_update_post_hook(સંરચના command *commands)
{
	સ્થિર અચલ અક્ષર hook_name[] = "post-update";
	સંરચના run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	સંરચના async sideband_async;
	સંરચના command *cmd;
	પૂર્ણાંક sideband_async_started = 0;
	પૂર્ણાંક saved_stderr = -1;

	જો (!hook_exists(the_repository, hook_name))
		પાછા;

	માટે (cmd = commands; cmd; cmd = cmd->next) {
		જો (cmd->error_string || cmd->did_not_exist)
			ચાલુ;
		strvec_ધકેલો(&opt.args, cmd->ref_name);
	}
	જો (!opt.args.nr)
		પાછા;

	prepare_sideband_async(&sideband_async, &saved_stderr, &sideband_async_started);

	run_hooks_opt(the_repository, hook_name, &opt);

	finish_sideband_async(&sideband_async, saved_stderr, sideband_async_started);
}

સ્થિર શૂન્ય check_aliased_update_internal(સંરચના command *cmd,
					  સંરચના string_list *list,
					  અચલ અક્ષર *dst_name, પૂર્ણાંક flag)
{
	સંરચના string_list_item *item;
	સંરચના command *dst_cmd;

	જો (!(flag & REF_ISSYMREF))
		પાછા;

	જો (!dst_name) {
		rp_error("refusing update to broken symref '%s'", cmd->ref_name);
		cmd->skip_update = 1;
		cmd->error_string = "broken symref";
		પાછા;
	}
	dst_name = strip_namespace(dst_name);

	જો (!(item = string_list_lookup(list, dst_name)))
		પાછા;

	cmd->skip_update = 1;

	dst_cmd = (સંરચના command *) item->util;

	જો (oideq(&cmd->old_oid, &dst_cmd->old_oid) &&
	    oideq(&cmd->new_oid, &dst_cmd->new_oid))
		પાછા;

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

સ્થિર શૂન્ય check_aliased_update(સંરચના command *cmd, સંરચના string_list *list)
{
	સંરચના strbuf buf = STRBUF_INIT;
	અચલ અક્ષર *dst_name;
	પૂર્ણાંક flag;

	strbuf_addf(&buf, "%s%s", get_git_namespace(), cmd->ref_name);
	dst_name = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					   buf.buf, 0, ખાલી, &flag);
	check_aliased_update_internal(cmd, list, dst_name, flag);
	strbuf_release(&buf);
}

સ્થિર શૂન્ય check_aliased_updates(સંરચના command *commands)
{
	સંરચના command *cmd;
	સંરચના string_list ref_list = STRING_LIST_INIT_NODUP;

	માટે (cmd = commands; cmd; cmd = cmd->next) {
		સંરચના string_list_item *item =
			string_list_append(&ref_list, cmd->ref_name);
		item->util = (શૂન્ય *)cmd;
	}
	string_list_sort(&ref_list);

	માટે (cmd = commands; cmd; cmd = cmd->next) {
		જો (!cmd->error_string)
			check_aliased_update(cmd, &ref_list);
	}

	string_list_clear(&ref_list, 0);
}

સ્થિર અચલ સંરચના object_id *command_singleton_iterator(શૂન્ય *cb_data)
{
	સંરચના command **cmd_list = cb_data;
	સંરચના command *cmd = *cmd_list;

	જો (!cmd || is_null_oid(&cmd->new_oid))
		પાછા ખાલી;
	*cmd_list = ખાલી; /* this returns only one */
	પાછા &cmd->new_oid;
}

સ્થિર શૂન્ય set_connectivity_errors(સંરચના command *commands,
				    સંરચના shallow_info *si,
				    સંરચના odb_transaction *transaction)
{
	સંરચના command *cmd;

	માટે (cmd = commands; cmd; cmd = cmd->next) {
		સંરચના command *singleton = cmd;
		સંરચના check_connected_options opt = CHECK_CONNECTED_INIT;
		સંરચના strvec env = STRVEC_INIT;

		જો (shallow_update && si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			ચાલુ;

		odb_transaction_env(transaction, &env);
		opt.env = env.v;

		જો (!check_connected(command_singleton_iterator, &singleton,
				     &opt))
			ચાલુ;

		cmd->error_string = "missing necessary objects";

		strvec_clear(&env);
	}
}

સંરચના iterate_data {
	સંરચના command *cmds;
	સંરચના shallow_info *si;
};

સ્થિર અચલ સંરચના object_id *iterate_receive_command_list(શૂન્ય *cb_data)
{
	સંરચના iterate_data *data = cb_data;
	સંરચના command **cmd_list = &data->cmds;
	સંરચના command *cmd = *cmd_list;

	માટે (; cmd; cmd = cmd->next) {
		જો (shallow_update && data->si->shallow_ref[cmd->index])
			/* to be checked in update_shallow_ref() */
			ચાલુ;
		જો (!is_null_oid(&cmd->new_oid) && !cmd->skip_update) {
			*cmd_list = cmd->next;
			પાછા &cmd->new_oid;
		}
	}
	પાછા ખાલી;
}

સ્થિર શૂન્ય reject_updates_to_hidden(સંરચના command *commands)
{
	સંરચના strbuf refname_full = STRBUF_INIT;
	size_t prefix_len;
	સંરચના command *cmd;

	strbuf_addstr(&refname_full, get_git_namespace());
	prefix_len = refname_full.len;

	માટે (cmd = commands; cmd; cmd = cmd->next) {
		જો (cmd->error_string)
			ચાલુ;

		strbuf_setlen(&refname_full, prefix_len);
		strbuf_addstr(&refname_full, cmd->ref_name);

		જો (!ref_is_hidden(cmd->ref_name, refname_full.buf, &hidden_refs))
			ચાલુ;
		જો (is_null_oid(&cmd->new_oid))
			cmd->error_string = "deny deleting a hidden ref";
		નહીંતર
			cmd->error_string = "deny updating a hidden ref";
	}

	strbuf_release(&refname_full);
}

સ્થિર પૂર્ણાંક should_process_cmd(સંરચના command *cmd)
{
	પાછા !cmd->error_string && !cmd->skip_update;
}

સ્થિર શૂન્ય BUG_if_skipped_connectivity_check(સંરચના command *commands,
					       સંરચના shallow_info *si)
{
	સંરચના command *cmd;

	માટે (cmd = commands; cmd; cmd = cmd->next) {
		જો (should_process_cmd(cmd) && si->shallow_ref[cmd->index])
			bug("connectivity check has not been run on ref %s",
			    cmd->ref_name);
	}
	BUG_if_bug("connectivity check skipped???");
}

સ્થિર શૂન્ય ref_transaction_rejection_handler(અચલ અક્ષર *refname,
					      અચલ સંરચના object_id *old_oid UNUSED,
					      અચલ સંરચના object_id *new_oid UNUSED,
					      અચલ અક્ષર *old_target UNUSED,
					      અચલ અક્ષર *new_target UNUSED,
					      ગણના ref_transaction_error err,
					      અચલ અક્ષર *details,
					      શૂન્ય *cb_data)
{
	સંરચના strmap *failed_refs = cb_data;

	જો (details)
		rp_error("%s", details);

	strmap_put(failed_refs, refname, (અક્ષર *)ref_transaction_error_સંદેશ(err));
}

સ્થિર શૂન્ય execute_commands_non_atomic(સંરચના command *commands,
					સંરચના shallow_info *si)
{
	સંરચના command *cmd;
	સંરચના strbuf err = STRBUF_INIT;
	અચલ અક્ષર *reported_error = ખાલી;
	સંરચના strmap failed_refs = STRMAP_INIT;

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
	ગણના processing_phase {
		PHASE_DELETIONS,
		PHASE_OTHERS
	};

	માટે (ગણના processing_phase phase = PHASE_DELETIONS; phase <= PHASE_OTHERS; phase++) {
		માટે (cmd = commands; cmd; cmd = cmd->next) {
			જો (!should_process_cmd(cmd) || cmd->run_proc_receive)
				ચાલુ;

			જો (phase == PHASE_DELETIONS && !is_null_oid(&cmd->new_oid))
				ચાલુ;
			નહીંતર જો (phase == PHASE_OTHERS && is_null_oid(&cmd->new_oid))
				ચાલુ;

			/*
			 * Lazily create a transaction only when we know there are
			 * updates to be added.
			 */
			જો (!transaction) {
				transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
									  REF_TRANSACTION_ALLOW_FAILURE, &err);
				જો (!transaction) {
					rp_error("%s", err.buf);
					strbuf_reset(&err);
					reported_error = "transaction failed to start";
					જાઓ failure;
				}
			}

			cmd->error_string = update(cmd, si);
		}

		/* No transaction, so nothing to commit */
		જો (!transaction)
			જાઓ cleanup;

		જો (ref_transaction_commit(transaction, &err)) {
			rp_error("%s", err.buf);
			reported_error = "failed to update refs";
			જાઓ failure;
		}

		ref_transaction_for_each_rejected_update(transaction,

							 ref_transaction_rejection_handler,
							 &failed_refs);

		જો (strmap_empty(&failed_refs))
			જાઓ cleanup;

	failure:
		માટે (cmd = commands; cmd; cmd = cmd->next) {
			જો (reported_error)
				cmd->error_string = reported_error;
			નહીંતર જો (strmap_contains(&failed_refs, cmd->ref_name))
				cmd->error_string = cmd->error_string_owned = xstrdup(strmap_get(&failed_refs, cmd->ref_name));
		}

	cleanup:
		ref_transaction_free(transaction);
		transaction = ખાલી;
		strmap_clear(&failed_refs, 0);
		strbuf_release(&err);
	}
}

સ્થિર શૂન્ય execute_commands_atomic(સંરચના command *commands,
					સંરચના shallow_info *si)
{
	સંરચના command *cmd;
	સંરચના strbuf err = STRBUF_INIT;
	અચલ અક્ષર *reported_error = "atomic push failure";

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	જો (!transaction) {
		rp_error("%s", err.buf);
		strbuf_reset(&err);
		reported_error = "transaction failed to start";
		જાઓ failure;
	}

	માટે (cmd = commands; cmd; cmd = cmd->next) {
		જો (!should_process_cmd(cmd) || cmd->run_proc_receive)
			ચાલુ;

		cmd->error_string = update(cmd, si);

		જો (cmd->error_string)
			જાઓ failure;
	}

	જો (ref_transaction_commit(transaction, &err)) {
		rp_error("%s", err.buf);
		reported_error = "atomic transaction failed";
		જાઓ failure;
	}
	જાઓ cleanup;

failure:
	માટે (cmd = commands; cmd; cmd = cmd->next)
		જો (!cmd->error_string)
			cmd->error_string = reported_error;

cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&err);
}

સ્થિર શૂન્ય execute_commands(સંરચના command *commands,
			     પૂર્ણાંક unpacker_error,
			     સંરચના shallow_info *si,
			     સંરચના odb_transaction *transaction,
			     અચલ સંરચના string_list *ધકેલો_options)
{
	સંરચના check_connected_options opt = CHECK_CONNECTED_INIT;
	સંરચના command *cmd;
	સંરચના iterate_data data;
	સંરચના async muxer;
	પૂર્ણાંક err_fd = 0;
	પૂર્ણાંક run_proc_receive = 0;

	જો (unpacker_error) {
		માટે (cmd = commands; cmd; cmd = cmd->next)
			cmd->error_string = "unpacker error";
		પાછા;
	}

	જો (!skip_connectivity_check) {
		સંરચના strvec env = STRVEC_INIT;

		જો (use_sideband) {
			memset(&muxer, 0, કદ(muxer));
			muxer.proc = copy_to_sideband;
			muxer.in = -1;
			જો (!start_async(&muxer))
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

		જો (check_connected(iterate_receive_command_list, &data, &opt))
			set_connectivity_errors(commands, si, transaction);

		જો (use_sideband)
			finish_async(&muxer);

		strvec_clear(&env);
	}

	reject_updates_to_hidden(commands);

	/*
	 * Try to find commands that have special prefix in their reference names,
	 * and mark them to run an external "proc-receive" hook later.
	 */
	જો (proc_receive_ref) {
		માટે (cmd = commands; cmd; cmd = cmd->next) {
			જો (!should_process_cmd(cmd))
				ચાલુ;

			જો (proc_receive_ref_matches(cmd)) {
				cmd->run_proc_receive = RUN_PROC_RECEIVE_SCHEDULED;
				run_proc_receive = 1;
			}
		}
	}

	જો (run_receive_hook(commands, "pre-receive", 0, transaction, ધકેલો_options)) {
		માટે (cmd = commands; cmd; cmd = cmd->next) {
			જો (!cmd->error_string)
				cmd->error_string = "pre-receive hook declined";
		}
		પાછા;
	}

	/*
	 * If there is no command ready to run, should return directly to destroy
	 * temporary data in the quarantine area.
	 */
	માટે (cmd = commands; cmd && cmd->error_string; cmd = cmd->next)
		; /* nothing */
	જો (!cmd)
		પાછા;

	/*
	 * Now we'll start writing out refs, which means the objects need
	 * to be in their final positions so that other processes can see them.
	 */
	જો (odb_transaction_commit(transaction)) {
		માટે (cmd = commands; cmd; cmd = cmd->next) {
			જો (!cmd->error_string)
				cmd->error_string = "unable to migrate objects to permanent storage";
		}
		પાછા;
	}

	check_aliased_updates(commands);

	મુક્ત(head_name_to_free);
	head_name = head_name_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
							    "HEAD", 0, ખાલી,
							    ખાલી);

	જો (run_proc_receive &&
	    run_proc_receive_hook(commands, ધકેલો_options))
		માટે (cmd = commands; cmd; cmd = cmd->next)
			જો (!cmd->error_string &&
			    !(cmd->run_proc_receive & RUN_PROC_RECEIVE_RETURNED) &&
			    (cmd->run_proc_receive || use_atomic))
				cmd->error_string = "fail to run proc-receive hook";

	જો (use_atomic)
		execute_commands_atomic(commands, si);
	નહીંતર
		execute_commands_non_atomic(commands, si);

	જો (shallow_update)
		BUG_if_skipped_connectivity_check(commands, si);
}

સ્થિર સંરચના command **queue_command(સંરચના command **tail,
				      અચલ અક્ષર *લીટી,
				      પૂર્ણાંક linelen)
{
	સંરચના object_id old_oid, new_oid;
	સંરચના command *cmd;
	અચલ અક્ષર *refname;
	પૂર્ણાંક reflen;
	અચલ અક્ષર *p;

	જો (parse_oid_hex(લીટી, &old_oid, &p) ||
	    *p++ != ' ' ||
	    parse_oid_hex(p, &new_oid, &p) ||
	    *p++ != ' ')
		die("protocol error: expected old/new/ref, got '%s'", લીટી);

	refname = p;
	reflen = linelen - (p - લીટી);
	FLEX_ALLOC_MEM(cmd, ref_name, refname, reflen);
	oidcpy(&cmd->old_oid, &old_oid);
	oidcpy(&cmd->new_oid, &new_oid);
	*tail = cmd;
	પાછા &cmd->next;
}

સ્થિર શૂન્ય free_commands(સંરચના command *commands)
{
	જ્યારે (commands) {
		સંરચના command *next = commands->next;

		ref_ધકેલો_report_free(commands->report);
		મુક્ત(commands->error_string_owned);
		મુક્ત(commands);
		commands = next;
	}
}

સ્થિર શૂન્ય queue_commands_from_cert(સંરચના command **tail,
				     સંરચના strbuf *ધકેલો_cert)
{
	અચલ અક્ષર *boc, *eoc;

	જો (*tail)
		die("protocol error: got both push certificate and unsigned commands");

	boc = strstr(ધકેલો_cert->buf, "\n\n");
	જો (!boc)
		die("malformed push certificate %.*s", 100, ધકેલો_cert->buf);
	નહીંતર
		boc += 2;
	eoc = ધકેલો_cert->buf + parse_signed_buffer(ધકેલો_cert->buf, ધકેલો_cert->len);

	જ્યારે (boc < eoc) {
		અચલ અક્ષર *eol = memchr(boc, '\n', eoc - boc);
		tail = queue_command(tail, boc, eol ? eol - boc : eoc - boc);
		boc = eol ? eol + 1 : eoc;
	}
}

સ્થિર સંરચના command *read_head_info(સંરચના packet_reader *reader,
				      સંરચના oid_array *shallow,
				      ગણના report_status_version *version)
{
	સંરચના command *commands = ખાલી;
	સંરચના command **p = &commands;
	માટે (;;) {
		પૂર્ણાંક linelen;

		જો (packet_reader_read(reader) != PACKET_READ_NORMAL)
			તોડો;

		જો (reader->pktlen > 8 && starts_with(reader->લીટી, "shallow ")) {
			સંરચના object_id oid;
			જો (get_oid_hex(reader->લીટી + 8, &oid))
				die("protocol error: expected shallow sha, got '%s'",
				    reader->લીટી + 8);
			oid_array_append(shallow, &oid);
			ચાલુ;
		}

		linelen = strlen(reader->લીટી);
		જો (linelen < reader->pktlen) {
			અચલ અક્ષર *feature_list = reader->લીટી + linelen + 1;
			અચલ અક્ષર *hash = ખાલી;
			અચલ અક્ષર *client_sid;
			size_t len = 0;
			જો (parse_feature_request(feature_list, "report-status"))
				*version = REPORT_STATUS_V0;
			જો (parse_feature_request(feature_list, "report-status-v2"))
				*version = REPORT_STATUS_V2;
			જો (parse_feature_request(feature_list, "side-band-64k"))
				use_sideband = LARGE_PACKET_MAX;
			જો (parse_feature_request(feature_list, "quiet"))
				quiet = 1;
			જો (advertise_atomic_ધકેલો
			    && parse_feature_request(feature_list, "atomic"))
				use_atomic = 1;
			જો (advertise_ધકેલો_options
			    && parse_feature_request(feature_list, "push-options"))
				use_ધકેલો_options = 1;
			hash = parse_feature_કિંમત(feature_list, "object-format", &len, ખાલી);
			જો (!hash) {
				hash = hash_algos[GIT_HASH_SHA1_LEGACY].name;
				len = strlen(hash);
			}
			જો (xstrncmpz(the_hash_algo->name, hash, len))
				die("error: unsupported object format '%s'", hash);
			client_sid = parse_feature_કિંમત(feature_list, "session-id", &len, ખાલી);
			જો (client_sid) {
				અક્ષર *sid = xstrndup(client_sid, len);
				trace2_data_string("transfer", ખાલી, "client-sid", client_sid);
				મુક્ત(sid);
			}
		}

		જો (!strcmp(reader->લીટી, "push-cert")) {
			પૂર્ણાંક true_flush = 0;
			પૂર્ણાંક saved_options = reader->options;
			reader->options &= ~PACKET_READ_CHOMP_NEWLINE;

			માટે (;;) {
				packet_reader_read(reader);
				જો (reader->status == PACKET_READ_FLUSH) {
					true_flush = 1;
					તોડો;
				}
				જો (reader->status != PACKET_READ_NORMAL) {
					die("protocol error: got an unexpected packet");
				}
				જો (!strcmp(reader->લીટી, "push-cert-end\n"))
					તોડો; /* end of cert */
				strbuf_addstr(&ધકેલો_cert, reader->લીટી);
			}
			reader->options = saved_options;

			જો (true_flush)
				તોડો;
			ચાલુ;
		}

		p = queue_command(p, reader->લીટી, linelen);
	}

	જો (ધકેલો_cert.len)
		queue_commands_from_cert(p, &ધકેલો_cert);

	પાછા commands;
}

સ્થિર શૂન્ય read_ધકેલો_options(સંરચના packet_reader *reader,
			      સંરચના string_list *options)
{
	જ્યારે (1) {
		જો (packet_reader_read(reader) != PACKET_READ_NORMAL)
			તોડો;

		string_list_append(options, reader->લીટી);
	}
}

સ્થિર પૂર્ણાંક unpack_with_sideband(સંરચના odb_transaction *transaction,
				અચલ અક્ષર *shallow_file,
				સંરચના strbuf *err_સંદેશ)
{
	સંરચના odb_transaction_write_pack_opts opts = {
		.fsck_objects = (receive_fsck_objects >= 0
				 ? receive_fsck_objects
				 : transfer_fsck_objects >= 0
				 ? transfer_fsck_objects
				 : 0),
		.fsck_સંદેશ_types = fsck_સંદેશ_types.buf,
		.max_આગત_size = max_આગત_size,
		.shallow_file = shallow_file,
		.reject_thin = reject_thin,
		.quiet = quiet,
	};
	સંરચના async muxer;
	પૂર્ણાંક ret;

	જો (!use_sideband)
		પાછા odb_transaction_write_pack(transaction, 0, err_સંદેશ, &opts);

	use_keepalive = KEEPALIVE_AFTER_NUL;
	memset(&muxer, 0, કદ(muxer));
	muxer.proc = copy_to_sideband;
	muxer.in = -1;
	જો (start_async(&muxer))
		પાછા 0;

	opts.err_fd = muxer.in;
	ret = odb_transaction_write_pack(transaction, 0, err_સંદેશ, &opts);

	finish_async(&muxer);
	પાછા ret;
}

સ્થિર શૂન્ય prepare_shallow_update(સંરચના shallow_info *si,
				   અચલ અક્ષર *shallow_file)
{
	પૂર્ણાંક i, j, k, bitmap_size = DIV_ROUND_UP(si->ref->nr, 32);

	ALLOC_ARRAY(si->used_shallow, si->shallow->nr);
	assign_shallow_commits_to_refs(si, si->used_shallow, ખાલી);

	CALLOC_ARRAY(si->need_reachability_test, si->shallow->nr);
	CALLOC_ARRAY(si->reachable, si->shallow->nr);
	CALLOC_ARRAY(si->shallow_ref, si->ref->nr);

	માટે (i = 0; i < si->nr_ours; i++)
		si->need_reachability_test[si->ours[i]] = 1;

	માટે (i = 0; i < si->shallow->nr; i++) {
		જો (!si->used_shallow[i])
			ચાલુ;
		માટે (j = 0; j < bitmap_size; j++) {
			જો (!si->used_shallow[i][j])
				ચાલુ;
			si->need_reachability_test[i]++;
			માટે (k = 0; k < 32; k++)
				જો (si->used_shallow[i][j] & (1U << k))
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

સ્થિર શૂન્ય update_shallow_info(સંરચના command *commands,
				સંરચના shallow_info *si,
				સંરચના oid_array *ref,
				અચલ અક્ષર *shallow_file)
{
	સંરચના command *cmd;
	પૂર્ણાંક *ref_status;
	remove_nonexistent_theirs_shallow(si);
	જો (!si->nr_ours && !si->nr_theirs) {
		shallow_update = 0;
		પાછા;
	}

	માટે (cmd = commands; cmd; cmd = cmd->next) {
		જો (is_null_oid(&cmd->new_oid))
			ચાલુ;
		oid_array_append(ref, &cmd->new_oid);
		cmd->index = ref->nr - 1;
	}
	si->ref = ref;

	જો (shallow_update) {
		prepare_shallow_update(si, shallow_file);
		પાછા;
	}

	ALLOC_ARRAY(ref_status, ref->nr);
	assign_shallow_commits_to_refs(si, ખાલી, ref_status);
	માટે (cmd = commands; cmd; cmd = cmd->next) {
		જો (is_null_oid(&cmd->new_oid))
			ચાલુ;
		જો (ref_status[cmd->index]) {
			cmd->error_string = "shallow update not allowed";
			cmd->skip_update = 1;
		}
	}
	મુક્ત(ref_status);
}

સ્થિર શૂન્ય override_cmds_error(સંરચના command *commands, અચલ અક્ષર *err)
{
	માટે (સંરચના command *cmd = commands; cmd; cmd = cmd->next) {
		FREE_AND_NULL(cmd->error_string_owned);
		cmd->error_string = err;
	}
}

/*
 * Generate the response to be sent to the client invoking 'git-receive-pack(1)'.
 */
સ્થિર શૂન્ય generate_report(સંરચના strbuf *buf, સંરચના command *commands,
			    અચલ સંરચના strbuf *unpack_status,
			    ગણના report_status_version version)
{
	સંરચના command *cmd;

	packet_buf_write(buf, "unpack %s\n",
			 unpack_status->len ? unpack_status->buf : "ok");

	માટે (cmd = commands; cmd; cmd = cmd->next) {
		સંરચના ref_ધકેલો_report *report;
		પૂર્ણાંક ગણતરી = 0;

		જો (cmd->error_string)
			packet_buf_write(buf, "ng %s %s\n",
					 cmd->ref_name, cmd->error_string);
		નહીંતર
			packet_buf_write(buf, "ok %s\n", cmd->ref_name);

		જો (version != REPORT_STATUS_V2 || cmd->error_string)
			ચાલુ;

		માટે (report = cmd->report; report; report = report->next) {
			જો (ગણતરી++ > 0)
				packet_buf_write(buf, "ok %s\n",
						 cmd->ref_name);
			જો (report->ref_name)
				packet_buf_write(buf, "option refname %s\n",
						 report->ref_name);
			જો (report->old_oid)
				packet_buf_write(buf, "option old-oid %s\n",
						 oid_to_hex(report->old_oid));
			જો (report->new_oid)
				packet_buf_write(buf, "option new-oid %s\n",
						 oid_to_hex(report->new_oid));
			જો (report->forced_update)
				packet_buf_write(buf, "option forced-update\n");
		}
	}

	packet_buf_flush(buf);
}

સ્થિર શૂન્ય report(સંરચના command *commands, અચલ સંરચના strbuf *unpack_status,
		   ગણના report_status_version version)
{
	સંરચના strbuf buf = STRBUF_INIT;

	generate_report(&buf, commands, unpack_status, version);

	જો (run_receive_report_hook(&buf)) {
		strbuf_reset(&buf);
		override_cmds_error(commands, "receive-report hook failed");
		generate_report(&buf, commands, unpack_status, version);
	}

	જો (use_sideband)
		send_sideband(1, 1, buf.buf, buf.len, use_sideband);
	નહીંતર
		write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
}

સ્થિર પૂર્ણાંક delete_only(સંરચના command *commands)
{
	સંરચના command *cmd;
	માટે (cmd = commands; cmd; cmd = cmd->next) {
		જો (!is_null_oid(&cmd->new_oid))
			પાછા 0;
	}
	પાછા 1;
}

પૂર્ણાંક cmd_receive_pack(પૂર્ણાંક argc,
		     અચલ અક્ષર **argv,
		     અચલ અક્ષર *prefix,
		     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક advertise_refs = 0;
	સંરચના command *commands;
	સંરચના oid_array shallow = OID_ARRAY_INIT;
	સંરચના oid_array ref = OID_ARRAY_INIT;
	સંરચના shallow_info si;
	સંરચના packet_reader reader;
	સંરચના odb_transaction *transaction = ખાલી;
	ગણના report_status_version version = REPORT_STATUS_UNKNOWN;

	સંરચના option options[] = {
		OPT__QUIET(&quiet, N_("quiet")),
		OPT_HIDDEN_BOOL(0, "skip-connectivity-check", &skip_connectivity_check, ખાલી),
		OPT_HIDDEN_BOOL(0, "stateless-rpc", &stateless_rpc, ખાલી),
		OPT_HIDDEN_BOOL(0, "http-backend-info-refs", &advertise_refs, ખાલી),
		OPT_ALIAS(0, "advertise-refs", "http-backend-info-refs"),
		OPT_HIDDEN_BOOL(0, "reject-thin-pack-for-testing", &reject_thin, ખાલી),
		OPT_END()
	};

	packet_trace_identity("receive-pack");

	argc = parse_options(argc, argv, prefix, options, receive_pack_usage, 0);

	જો (argc > 1)
		usage_સંદેશ_opt(_("too many arguments"), receive_pack_usage, options);
	જો (argc == 0)
		usage_સંદેશ_opt(_("you must specify a directory"), receive_pack_usage, options);

	service_dir = argv[0];

	setup_path();

	જો (!enter_repo(the_repository, service_dir, 0))
		die("'%s' does not appear to be a git repository", service_dir);

	repo_config(the_repository, receive_pack_config, ખાલી);
	જો (cert_nonce_seed)
		ધકેલો_cert_nonce = prepare_ધકેલો_cert_nonce(service_dir, time(ખાલી));

	પસંદગી (determine_protocol_version_server()) {
	સ્થિતિ protocol_v2:
		/*
		 * push support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		તોડો;
	સ્થિતિ protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		જો (advertise_refs || !stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	સ્થિતિ protocol_v0:
		તોડો;
	સ્થિતિ protocol_unknown_version:
		BUG("unknown protocol version");
	}

	જો (advertise_refs || !stateless_rpc) {
		write_head_info();
	}
	જો (advertise_refs)
		પાછા 0;

	packet_reader_init(&reader, 0, ખાલી, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	જો ((commands = read_head_info(&reader, &shallow, &version))) {
		સંરચના string_list ધકેલો_options = STRING_LIST_INIT_DUP;
		સંરચના strbuf unpack_status = STRBUF_INIT;

		જો (use_ધકેલો_options)
			read_ધકેલો_options(&reader, &ધકેલો_options);
		જો (!check_cert_ધકેલો_options(&ધકેલો_options)) {
			સંરચના command *cmd;
			માટે (cmd = commands; cmd; cmd = cmd->next)
				cmd->error_string = "inconsistent push options";
		}

		prepare_shallow_info(&si, &shallow);
		જો (!si.nr_ours && !si.nr_theirs)
			shallow_update = 0;
		જો (!delete_only(commands)) {
			અચલ અક્ષર *alt_shallow_file = ખાલી;

			જો (si.nr_ours || si.nr_theirs)
				alt_shallow_file = setup_temporary_shallow(si.shallow);

			જો (odb_transaction_begin(the_repository->objects, &transaction, ODB_TRANSACTION_RECEIVE))
				strbuf_addstr(&unpack_status, "unable to start object transaction");
			નહીંતર
				unpack_with_sideband(transaction, alt_shallow_file, &unpack_status);

			update_shallow_info(commands, &si, &ref, alt_shallow_file);
		}
		use_keepalive = KEEPALIVE_ALWAYS;
		execute_commands(commands, !!unpack_status.len, &si, transaction,
				 &ધકેલો_options);
		odb_transaction_finalize(transaction);
		sigchain_ધકેલો(SIGPIPE, SIG_IGN);

		પસંદગી (version) {
		સ્થિતિ REPORT_STATUS_V2:
		સ્થિતિ REPORT_STATUS_V0:
			report(commands, &unpack_status, version);
			તોડો;
		સ્થિતિ REPORT_STATUS_UNKNOWN:
			તોડો;
		}

		sigchain_કાઢો(SIGPIPE);
		run_receive_hook(commands, "post-receive", 1, ખાલી,
				 &ધકેલો_options);
		run_update_post_hook(commands);
		free_commands(commands);
		string_list_clear(&ધકેલો_options, 0);
		જો (auto_gc) {
			સંરચના child_process proc = CHILD_PROCESS_INIT;

			જો (prepare_auto_maintenance(the_repository, 1, &proc)) {
				proc.no_stdin = 1;
				proc.stdout_to_stderr = 1;
				proc.err = use_sideband ? -1 : 0;

				જો (!start_command(&proc)) {
					જો (use_sideband)
						copy_to_sideband(proc.err, -1, ખાલી);
					finish_command(&proc);
				}
			}
		}
		જો (auto_update_server_info)
			update_server_info(the_repository, 0);
		clear_shallow_info(&si);
		strbuf_release(&unpack_status);
	}
	જો (use_sideband)
		packet_flush(1);
	oid_array_clear(&shallow);
	oid_array_clear(&ref);
	strvec_clear(&hidden_refs);
	મુક્ત((શૂન્ય *)ધકેલો_cert_nonce);
	પાછા 0;
}
