#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fsmonitor-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fsmonitor-ipc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fsmonitor-settings.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "compat/fsmonitor/fsm-health.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "compat/fsmonitor/fsm-listen.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fsmonitor--daemon.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "simple-ipc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace2.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_fsmonitor__daemon_usage[] = {
	N_("git fsmonitor--daemon start [<options>]"),
	N_("git fsmonitor--daemon run [<options>]"),
	"git fsmonitor--daemon stop",
	"git fsmonitor--daemon status",
	ਨਿਰਾ
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FSMONITOR_DAEMON_BACKEND
/*
 * Global state loaded from config.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FSMONITOR_IPC_ਧਾਗੇ "fsmonitor.ipcthreads"
ਸਥਿਰ ਪੂਰਨ fsmonitor_ipc_ਧਾਗੇ = 8;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FSMONITOR_ਸ਼ੁਰੂ_TIMEOUT "fsmonitor.starttimeout"
ਸਥਿਰ ਪੂਰਨ fsmonitor_ਸ਼ੁਰੂ_timeout_sec = 60;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FSMONITOR__ANNOUNCE_STARTUP "fsmonitor.announcestartup"
ਸਥਿਰ ਪੂਰਨ fsmonitor__announce_startup = 0;

ਸਥਿਰ ਪੂਰਨ fsmonitor_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			    ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(var, FSMONITOR_IPC_ਧਾਗੇ)) {
		ਪੂਰਨ i = git_config_int(var, ਮੁੱਲ, ctx->kvi);
		ਜੇ (i < 1)
			ਵਾਪਸ error(_("value of '%s' out of range: %d"),
				     FSMONITOR_IPC_ਧਾਗੇ, i);
		fsmonitor_ipc_ਧਾਗੇ = i;
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, FSMONITOR_ਸ਼ੁਰੂ_TIMEOUT)) {
		ਪੂਰਨ i = git_config_int(var, ਮੁੱਲ, ctx->kvi);
		ਜੇ (i < 0)
			ਵਾਪਸ error(_("value of '%s' out of range: %d"),
				     FSMONITOR_ਸ਼ੁਰੂ_TIMEOUT, i);
		fsmonitor_ਸ਼ੁਰੂ_timeout_sec = i;
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, FSMONITOR__ANNOUNCE_STARTUP)) {
		ਪੂਰਨ is_bool;
		ਪੂਰਨ i = git_config_bool_or_int(var, ਮੁੱਲ, ctx->kvi, &is_bool);
		ਜੇ (i < 0)
			ਵਾਪਸ error(_("value of '%s' not bool or int: %d"),
				     var, i);
		fsmonitor__announce_startup = i;
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

/*
 * Acting as a CLIENT.
 *
 * Send a "quit" command to the `git-fsmonitor--daemon` (if running)
 * and wait for it to shutdown.
 */
ਸਥਿਰ ਪੂਰਨ do_as_client__send_stop(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf answer = STRBUF_INIT;
	ਪੂਰਨ ret;
	ਪੂਰਨ max_wait_ms = 30000;
	ਪੂਰਨ elapsed_ms = 0;

	ret = fsmonitor_ipc__send_command("quit", &answer);

	/* The quit command does not return any response data. */
	strbuf_release(&answer);

	ਜੇ (ret)
		ਵਾਪਸ ret;

	trace2_region_enter("fsm_client", "polling-for-daemon-exit", ਨਿਰਾ);
	ਜਦੋਂ (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING) {
		ਜੇ (elapsed_ms >= max_wait_ms) {
			trace2_region_leave("fsm_client",
					    "polling-for-daemon-exit", ਨਿਰਾ);
			ਵਾਪਸ error(_("daemon did not stop within %d seconds"),
				     max_wait_ms / 1000);
		}
		sleep_millisec(50);
		elapsed_ms += 50;
	}
	trace2_region_leave("fsm_client", "polling-for-daemon-exit", ਨਿਰਾ);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ do_as_client_ਸਥਿਤੀ(ਖਾਲੀ)
{
	ਗਿਣਤੀ ipc_ਸਰਗਰਮ_state state = fsmonitor_ipc__get_state();

	ਚੋਣ (state) {
	ਹਾਲਤ IPC_STATE__LISTENING:
		ਲਿਖੋ(_("fsmonitor-daemon is watching '%s'\n"),
		       the_repository->worktree);
		ਵਾਪਸ 0;

	ਮੂਲ:
		ਲਿਖੋ(_("fsmonitor-daemon is not watching '%s'\n"),
		       the_repository->worktree);
		ਵਾਪਸ 1;
	}
}

ਗਿਣਤੀ fsmonitor_cookie_ਵਸਤੂ_ਨਤੀਜਾ {
	FCIR_ERROR = -1, /* could not create cookie file ? */
	FCIR_INIT,
	FCIR_ਵੇਖਿਆ,
	FCIR_ABORT,
};

ਬਣਤਰ fsmonitor_cookie_ਵਸਤੂ {
	ਬਣਤਰ hashmap_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
	ਅੱਖਰ *name;
	ਗਿਣਤੀ fsmonitor_cookie_ਵਸਤੂ_ਨਤੀਜਾ ਨਤੀਜਾ;
};

ਸਥਿਰ ਪੂਰਨ cookies_cmp(ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ,
		       ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *he1,
		       ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *he2, ਅਚਲ ਖਾਲੀ *keydata)
{
	ਅਚਲ ਬਣਤਰ fsmonitor_cookie_ਵਸਤੂ *a =
		container_ਦਾ(he1, ਅਚਲ ਬਣਤਰ fsmonitor_cookie_ਵਸਤੂ, ਇੰਦਰਾਜ);
	ਅਚਲ ਬਣਤਰ fsmonitor_cookie_ਵਸਤੂ *b =
		container_ਦਾ(he2, ਅਚਲ ਬਣਤਰ fsmonitor_cookie_ਵਸਤੂ, ਇੰਦਰਾਜ);

	ਵਾਪਸ strcmp(a->name, keydata ? keydata : b->name);
}

ਸਥਿਰ ਗਿਣਤੀ fsmonitor_cookie_ਵਸਤੂ_ਨਤੀਜਾ with_ਤਾਲਾ_wait_for_cookie(
	ਬਣਤਰ fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	ਪੂਰਨ fd;
	ਬਣਤਰ fsmonitor_cookie_ਵਸਤੂ *cookie;
	ਬਣਤਰ strbuf cookie_pathname = STRBUF_INIT;
	ਬਣਤਰ strbuf cookie_filename = STRBUF_INIT;
	ਗਿਣਤੀ fsmonitor_cookie_ਵਸਤੂ_ਨਤੀਜਾ ਨਤੀਜਾ;
	ਪੂਰਨ my_cookie_seq;

	CALLOC_ARRAY(cookie, 1);

	my_cookie_seq = state->cookie_seq++;

	strbuf_addf(&cookie_filename, "%i-%i", getpid(), my_cookie_seq);

	strbuf_addbuf(&cookie_pathname, &state->ਰਸਤਾ_cookie_ਅਗੇਤਰ);
	strbuf_addbuf(&cookie_pathname, &cookie_filename);

	cookie->name = strbuf_detach(&cookie_filename, ਨਿਰਾ);
	cookie->ਨਤੀਜਾ = FCIR_INIT;
	hashmap_ਇੰਦਰਾਜ_init(&cookie->ਇੰਦਰਾਜ, strhash(cookie->name));

	hashmap_ਜੋੜੋ(&state->cookies, &cookie->ਇੰਦਰਾਜ);

	trace_printf_ਕੁੰਜੀ(&trace_fsmonitor, "cookie-wait: '%s' '%s'",
			 cookie->name, cookie_pathname.buf);

	/*
	 * Create the cookie file on disk and then wait for a notification
	 * that the listener thread has seen it.
	 */
	fd = open(cookie_pathname.buf, O_WRONLY | O_CREAT | O_EXCL, 0600);
	ਜੇ (fd < 0) {
		error_errno(_("could not create fsmonitor cookie '%s'"),
			    cookie->name);

		cookie->ਨਤੀਜਾ = FCIR_ERROR;
		ਜਾਓ ਪੂਰਾ;
	}

	/*
	 * Technically, close() and unlink() can fail, but we don't
	 * care here.  We only created the file to trigger a watch
	 * event from the FS to know that when we're up to date.
	 */
	close(fd);
	unlink(cookie_pathname.buf);

	/*
	 * Wait for the listener thread to observe the cookie file.
	 * Time out after a short interval so that the client
	 * does not hang forever if the filesystem does not deliver
	 * events (e.g., on certain container/overlay filesystems
	 * where inotify watches succeed but events never arrive).
	 */
	{
		ਬਣਤਰ timeval now;
		ਬਣਤਰ timespec ts;
		ਪੂਰਨ err = 0;

		gettimeofday(&now, ਨਿਰਾ);
		ts.tv_sec = now.tv_sec + 1;
		ts.tv_nsec = now.tv_usec * 1000;

		ਜਦੋਂ (cookie->ਨਤੀਜਾ == FCIR_INIT && !err)
			err = pthread_cond_timedwait(&state->cookies_cond,
						     &state->main_ਤਾਲਾ,
						     &ts);
		ਜੇ (err == ETIMEDOUT && cookie->ਨਤੀਜਾ == FCIR_INIT) {
			trace_printf_ਕੁੰਜੀ(&trace_fsmonitor,
					 "cookie_wait timed out");
			cookie->ਨਤੀਜਾ = FCIR_ERROR;
		}
	}

ਪੂਰਾ:
	hashmap_remove(&state->cookies, &cookie->ਇੰਦਰਾਜ, ਨਿਰਾ);

	ਨਤੀਜਾ = cookie->ਨਤੀਜਾ;

	ਮੁਕਤ(cookie->name);
	ਮੁਕਤ(cookie);
	strbuf_release(&cookie_pathname);

	ਵਾਪਸ ਨਤੀਜਾ;
}

/*
 * Mark these cookies as _SEEN and wake up the corresponding client threads.
 */
ਸਥਿਰ ਖਾਲੀ with_ਤਾਲਾ_mark_cookies_ਵੇਖਿਆ(ਬਣਤਰ fsmonitor_daemon_state *state,
					 ਅਚਲ ਬਣਤਰ string_list *cookie_ਨਾਂ)
{
	/* assert current thread holding state->main_lock */

	ਪੂਰਨ k;
	ਪੂਰਨ nr_ਵੇਖਿਆ = 0;

	ਲਈ (k = 0; k < cookie_ਨਾਂ->nr; k++) {
		ਬਣਤਰ fsmonitor_cookie_ਵਸਤੂ ਕੁੰਜੀ;
		ਬਣਤਰ fsmonitor_cookie_ਵਸਤੂ *cookie;

		ਕੁੰਜੀ.name = cookie_ਨਾਂ->ਚੀਜ਼ਾਂ[k].string;
		hashmap_ਇੰਦਰਾਜ_init(&ਕੁੰਜੀ.ਇੰਦਰਾਜ, strhash(ਕੁੰਜੀ.name));

		cookie = hashmap_get_ਇੰਦਰਾਜ(&state->cookies, &ਕੁੰਜੀ, ਇੰਦਰਾਜ, ਨਿਰਾ);
		ਜੇ (cookie) {
			trace_printf_ਕੁੰਜੀ(&trace_fsmonitor, "cookie-seen: '%s'",
					 cookie->name);
			cookie->ਨਤੀਜਾ = FCIR_ਵੇਖਿਆ;
			nr_ਵੇਖਿਆ++;
		}
	}

	ਜੇ (nr_ਵੇਖਿਆ)
		pthread_cond_ਪ੍ਰਸਾਰਣ(&state->cookies_cond);
}

/*
 * Set _ABORT on all pending cookies and wake up all client threads.
 */
ਸਥਿਰ ਖਾਲੀ with_ਤਾਲਾ_abort_all_cookies(ਬਣਤਰ fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	ਬਣਤਰ hashmap_iter iter;
	ਬਣਤਰ fsmonitor_cookie_ਵਸਤੂ *cookie;
	ਪੂਰਨ nr_aborted = 0;

	hashmap_for_each_ਇੰਦਰਾਜ(&state->cookies, &iter, cookie, ਇੰਦਰਾਜ) {
		trace_printf_ਕੁੰਜੀ(&trace_fsmonitor, "cookie-abort: '%s'",
				 cookie->name);
		cookie->ਨਤੀਜਾ = FCIR_ABORT;
		nr_aborted++;
	}

	ਜੇ (nr_aborted)
		pthread_cond_ਪ੍ਰਸਾਰਣ(&state->cookies_cond);
}

/*
 * Requests to and from a FSMonitor Protocol V2 provider use an opaque
 * "token" as a virtual timestamp.  Clients can request a summary of all
 * created/deleted/modified files relative to a token.  In the response,
 * clients receive a new token for the next (relative) request.
 *
 *
 * Token Format
 * ============
 *
 * The contents of the token are private and provider-specific.
 *
 * For the built-in fsmonitor--daemon, we define a token as follows:
 *
 *     "builtin" ":" <token_id> ":" <sequence_nr>
 *
 * The "builtin" prefix is used as a namespace to avoid conflicts
 * with other providers (such as Watchman).
 *
 * The <token_id> is an arbitrary OPAQUE string, such as a GUID,
 * UUID, or {timestamp,pid}.  It is used to group all filesystem
 * events that happened while the daemon was monitoring (and in-sync
 * with the filesystem).
 *
 *     Unlike FSMonitor Protocol V1, it is not defined as a timestamp
 *     and does not define less-than/greater-than relationships.
 *     (There are too many race conditions to rely on file system
 *     event timestamps.)
 *
 * The <sequence_nr> is a simple integer incremented whenever the
 * daemon needs to make its state public.  For example, if 1000 file
 * system events come in, but no clients have requested the data,
 * the daemon can continue to accumulate file changes in the same
 * bin and does not need to advance the sequence number.  However,
 * as soon as a client does arrive, the daemon needs to start a new
 * bin and increment the sequence number.
 *
 *     The sequence number serves as the boundary between 2 sets
 *     of bins -- the older ones that the client has already seen
 *     and the newer ones that it hasn't.
 *
 * When a new <token_id> is created, the <sequence_nr> is reset to
 * zero.
 *
 *
 * About Token Ids
 * ===============
 *
 * A new token_id is created:
 *
 * [1] each time the daemon is started.
 *
 * [2] any time that the daemon must re-sync with the filesystem
 *     (such as when the kernel drops or we miss events on a very
 *     active volume).
 *
 * [3] in response to a client "flush" command (for dropped event
 *     testing).
 *
 * When a new token_id is created, the daemon is free to discard all
 * cached filesystem events associated with any previous token_ids.
 * Events associated with a non-current token_id will never be sent
 * to a client.  A token_id change implicitly means that the daemon
 * has gap in its event history.
 *
 * Therefore, clients that present a token with a stale (non-current)
 * token_id will always be given a trivial response.
 */
ਬਣਤਰ fsmonitor_token_ਅੰਕੜੇ {
	ਬਣਤਰ strbuf token_id;
	ਬਣਤਰ fsmonitor_batch *batch_ਸਿਰ;
	ਬਣਤਰ fsmonitor_batch *batch_ਪੂਛ;
	uint64_t client_ref_ਗਿਣਤੀ;
};

ਬਣਤਰ fsmonitor_batch {
	ਬਣਤਰ fsmonitor_batch *ਅਗਲਾ;
	uint64_t batch_seq_nr;
	ਅਚਲ ਅੱਖਰ **interned_paths;
	ਮਾਪ_t nr, alloc;
	ਸਮਾਂ_t pinned_ਸਮਾਂ;
};

ਸਥਿਰ ਬਣਤਰ fsmonitor_token_ਅੰਕੜੇ *fsmonitor_new_token_ਅੰਕੜੇ(ਖਾਲੀ)
{
	ਸਥਿਰ ਪੂਰਨ test_env_ਮੁੱਲ = -1;
	ਸਥਿਰ uint64_t flush_ਗਿਣਤੀ = 0;
	ਬਣਤਰ fsmonitor_token_ਅੰਕੜੇ *token;
	ਬਣਤਰ fsmonitor_batch *batch;

	CALLOC_ARRAY(token, 1);
	batch = fsmonitor_batch__new();

	strbuf_init(&token->token_id, 0);
	token->batch_ਸਿਰ = batch;
	token->batch_ਪੂਛ = batch;
	token->client_ref_ਗਿਣਤੀ = 0;

	ਜੇ (test_env_ਮੁੱਲ < 0)
		test_env_ਮੁੱਲ = git_env_bool("GIT_TEST_FSMONITOR_TOKEN", 0);

	ਜੇ (!test_env_ਮੁੱਲ) {
		ਬਣਤਰ timeval tv;
		ਬਣਤਰ tm tm;
		ਸਮਾਂ_t secs;

		gettimeofday(&tv, ਨਿਰਾ);
		secs = tv.tv_sec;
		gmtime_r(&secs, &tm);

		strbuf_addf(&token->token_id,
			    "%"PRIu64".%d.%4d%02d%02dT%02d%02d%02d.%06ldZ",
			    flush_ਗਿਣਤੀ++,
			    getpid(),
			    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			    tm.tm_hour, tm.tm_min, tm.tm_sec,
			    (ਲੰਮਾ)tv.tv_usec);
	} ਨਹੀਂਤਾਂ {
		strbuf_addf(&token->token_id, "test_%08x", test_env_ਮੁੱਲ++);
	}

	/*
	 * We created a new <token_id> and are starting a new series
	 * of tokens with a zero <seq_nr>.
	 *
	 * Since clients cannot guess our new (non test) <token_id>
	 * they will always receive a trivial response (because of the
	 * mismatch on the <token_id>).  The trivial response will
	 * tell them our new <token_id> so that subsequent requests
	 * will be relative to our new series.  (And when sending that
	 * response, we pin the current head of the batch list.)
	 *
	 * Even if the client correctly guesses the <token_id>, their
	 * request of "builtin:<token_id>:0" asks for all changes MORE
	 * RECENT than batch/bin 0.
	 *
	 * This implies that it is a waste to accumulate paths in the
	 * initial batch/bin (because they will never be transmitted).
	 *
	 * So the daemon could be running for days and watching the
	 * file system, but doesn't need to actually accumulate any
	 * paths UNTIL we need to set a reference point for a later
	 * relative request.
	 *
	 * However, it is very useful for testing to always have a
	 * reference point set.  Pin batch 0 to force early file system
	 * events to accumulate.
	 */
	ਜੇ (test_env_ਮੁੱਲ)
		batch->pinned_ਸਮਾਂ = ਸਮਾਂ(ਨਿਰਾ);

	ਵਾਪਸ token;
}

ਬਣਤਰ fsmonitor_batch *fsmonitor_batch__new(ਖਾਲੀ)
{
	ਬਣਤਰ fsmonitor_batch *batch;

	CALLOC_ARRAY(batch, 1);

	ਵਾਪਸ batch;
}

ਖਾਲੀ fsmonitor_batch__free_list(ਬਣਤਰ fsmonitor_batch *batch)
{
	ਜਦੋਂ (batch) {
		ਬਣਤਰ fsmonitor_batch *ਅਗਲਾ = batch->ਅਗਲਾ;

		/*
		 * The actual strings within the array of this batch
		 * are interned, so we don't own them.  We only own
		 * the array.
		 */
		ਮੁਕਤ(batch->interned_paths);
		ਮੁਕਤ(batch);

		batch = ਅਗਲਾ;
	}
}

ਖਾਲੀ fsmonitor_batch_ਜੋੜੋ_ਰਸਤਾ(ਬਣਤਰ fsmonitor_batch *batch,
			       ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਅਚਲ ਅੱਖਰ *interned_ਰਸਤਾ = strintern(ਰਸਤਾ);

	trace_printf_ਕੁੰਜੀ(&trace_fsmonitor, "event: %s", interned_ਰਸਤਾ);

	ALLOC_GROW(batch->interned_paths, batch->nr + 1, batch->alloc);
	batch->interned_paths[batch->nr++] = interned_ਰਸਤਾ;
}

ਸਥਿਰ ਖਾਲੀ fsmonitor_batch__combine(ਬਣਤਰ fsmonitor_batch *batch_dest,
				     ਅਚਲ ਬਣਤਰ fsmonitor_batch *batch_src)
{
	ਮਾਪ_t k;

	ALLOC_GROW(batch_dest->interned_paths,
		   batch_dest->nr + batch_src->nr + 1,
		   batch_dest->alloc);

	ਲਈ (k = 0; k < batch_src->nr; k++)
		batch_dest->interned_paths[batch_dest->nr++] =
			batch_src->interned_paths[k];
}

/*
 * To keep the batch list from growing unbounded in response to filesystem
 * activity, we try to truncate old batches from the end of the list as
 * they become irrelevant.
 *
 * We assume that the .git/index will be updated with the most recent token
 * any time the index is updated.  And future commands will only ask for
 * recent changes *since* that new token.  So as tokens advance into the
 * future, older batch items will never be requested/needed.  So we can
 * truncate them without loss of functionality.
 *
 * However, multiple commands may be talking to the daemon concurrently
 * or perform a slow command, so a little "token skew" is possible.
 * Therefore, we want this to be a little bit lazy and have a generous
 * delay.
 *
 * The current reader thread walked backwards in time from `token->batch_head`
 * back to `batch_marker` somewhere in the middle of the batch list.
 *
 * Let's walk backwards in time from that marker an arbitrary delay
 * and truncate the list there.  Note that these timestamps are completely
 * artificial (based on when we pinned the batch item) and not on any
 * filesystem activity.
 *
 * Return the obsolete portion of the list after we have removed it from
 * the official list so that the caller can free it after leaving the lock.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MY_ਸਮਾਂ_DELAY_SECONDS (5 * 60) /* seconds */

ਸਥਿਰ ਬਣਤਰ fsmonitor_batch *with_ਤਾਲਾ_truncate_old_batches(
	ਬਣਤਰ fsmonitor_daemon_state *state,
	ਅਚਲ ਬਣਤਰ fsmonitor_batch *batch_marker)
{
	/* assert current thread holding state->main_lock */

	ਅਚਲ ਬਣਤਰ fsmonitor_batch *batch;
	ਬਣਤਰ fsmonitor_batch *remainder;

	ਜੇ (!batch_marker)
		ਵਾਪਸ ਨਿਰਾ;

	trace_printf_ਕੁੰਜੀ(&trace_fsmonitor, "Truncate: mark (%"PRIu64",%"PRIu64")",
			 batch_marker->batch_seq_nr,
			 (uint64_t)batch_marker->pinned_ਸਮਾਂ);

	ਲਈ (batch = batch_marker; batch; batch = batch->ਅਗਲਾ) {
		ਸਮਾਂ_t t;

		ਜੇ (!batch->pinned_ਸਮਾਂ) /* an overflow batch */
			ਜਾਰੀ;

		t = batch->pinned_ਸਮਾਂ + MY_ਸਮਾਂ_DELAY_SECONDS;
		ਜੇ (t > batch_marker->pinned_ਸਮਾਂ) /* too close to marker */
			ਜਾਰੀ;

		ਜਾਓ truncate_past_ਇੱਥੇ;
	}

	ਵਾਪਸ ਨਿਰਾ;

truncate_past_ਇੱਥੇ:
	state->current_token_ਅੰਕੜੇ->batch_ਪੂਛ = (ਬਣਤਰ fsmonitor_batch *)batch;

	remainder = ((ਬਣਤਰ fsmonitor_batch *)batch)->ਅਗਲਾ;
	((ਬਣਤਰ fsmonitor_batch *)batch)->ਅਗਲਾ = ਨਿਰਾ;

	ਵਾਪਸ remainder;
}

ਸਥਿਰ ਖਾਲੀ fsmonitor_free_token_ਅੰਕੜੇ(ਬਣਤਰ fsmonitor_token_ਅੰਕੜੇ *token)
{
	ਜੇ (!token)
		ਵਾਪਸ;

	ਪ੍ਰਤਿਜ੍ਞਾ(token->client_ref_ਗਿਣਤੀ == 0);

	strbuf_release(&token->token_id);

	fsmonitor_batch__free_list(token->batch_ਸਿਰ);

	ਮੁਕਤ(token);
}

/*
 * Flush all of our cached data about the filesystem.  Call this if we
 * lose sync with the filesystem and miss some notification events.
 *
 * [1] If we are missing events, then we no longer have a complete
 *     history of the directory (relative to our current start token).
 *     We should create a new token and start fresh (as if we just
 *     booted up).
 *
 * [2] Some of those lost events may have been for cookie files.  We
 *     should assume the worst and abort them rather letting them starve.
 *
 * If there are no concurrent threads reading the current token data
 * series, we can free it now.  Otherwise, let the last reader free
 * it.
 *
 * Either way, the old token data series is no longer associated with
 * our state data.
 */
ਸਥਿਰ ਖਾਲੀ with_ਤਾਲਾ_do_force_resync(ਬਣਤਰ fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	ਬਣਤਰ fsmonitor_token_ਅੰਕੜੇ *free_me = ਨਿਰਾ;
	ਬਣਤਰ fsmonitor_token_ਅੰਕੜੇ *new_one = ਨਿਰਾ;

	new_one = fsmonitor_new_token_ਅੰਕੜੇ();

	ਜੇ (state->current_token_ਅੰਕੜੇ->client_ref_ਗਿਣਤੀ == 0)
		free_me = state->current_token_ਅੰਕੜੇ;
	state->current_token_ਅੰਕੜੇ = new_one;

	fsmonitor_free_token_ਅੰਕੜੇ(free_me);

	with_ਤਾਲਾ_abort_all_cookies(state);
}

ਖਾਲੀ fsmonitor_force_resync(ਬਣਤਰ fsmonitor_daemon_state *state)
{
	pthread_mutex_ਤਾਲਾ(&state->main_ਤਾਲਾ);
	with_ਤਾਲਾ_do_force_resync(state);
	pthread_mutex_unlock(&state->main_ਤਾਲਾ);
}

/*
 * Format an opaque token string to send to the client.
 */
ਸਥਿਰ ਖਾਲੀ with_ਤਾਲਾ_format_response_token(
	ਬਣਤਰ strbuf *response_token,
	ਅਚਲ ਬਣਤਰ strbuf *response_token_id,
	ਅਚਲ ਬਣਤਰ fsmonitor_batch *batch)
{
	/* assert current thread holding state->main_lock */

	strbuf_reset(response_token);
	strbuf_addf(response_token, "builtin:%s:%"PRIu64,
		    response_token_id->buf, batch->batch_seq_nr);
}

/*
 * Parse an opaque token from the client.
 * Returns -1 on error.
 */
ਸਥਿਰ ਪੂਰਨ fsmonitor_parse_client_token(ਅਚਲ ਅੱਖਰ *buf_token,
					ਬਣਤਰ strbuf *requested_token_id,
					uint64_t *seq_nr)
{
	ਅਚਲ ਅੱਖਰ *p;
	ਅੱਖਰ *p_ਸਿਰਾ;

	strbuf_reset(requested_token_id);
	*seq_nr = 0;

	ਜੇ (!skip_ਅਗੇਤਰ(buf_token, "builtin:", &p))
		ਵਾਪਸ -1;

	ਜਦੋਂ (*p && *p != ':')
		strbuf_addch(requested_token_id, *p++);
	ਜੇ (!*p++)
		ਵਾਪਸ -1;

	*seq_nr = (uint64_t)strtoumax(p, &p_ਸਿਰਾ, 10);
	ਜੇ (*p_ਸਿਰਾ)
		ਵਾਪਸ -1;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ do_handle_client(ਬਣਤਰ fsmonitor_daemon_state *state,
			    ਅਚਲ ਅੱਖਰ *command,
			    ipc_server_ਜਵਾਬ_cb *ਜਵਾਬ,
			    ਬਣਤਰ ipc_server_ਜਵਾਬ_ਅੰਕੜੇ *ਜਵਾਬ_ਅੰਕੜੇ)
{
	ਬਣਤਰ fsmonitor_token_ਅੰਕੜੇ *token_ਅੰਕੜੇ = ਨਿਰਾ;
	ਬਣਤਰ strbuf response_token = STRBUF_INIT;
	ਬਣਤਰ strbuf requested_token_id = STRBUF_INIT;
	ਬਣਤਰ strbuf payload = STRBUF_INIT;
	uint64_t requested_oldest_seq_nr = 0;
	uint64_t ਕੁੱਲ_response_len = 0;
	ਅਚਲ ਅੱਖਰ *p;
	ਅਚਲ ਬਣਤਰ fsmonitor_batch *batch_ਸਿਰ;
	ਅਚਲ ਬਣਤਰ fsmonitor_batch *batch;
	ਬਣਤਰ fsmonitor_batch *remainder = ਨਿਰਾ;
	intmax_t ਗਿਣਤੀ_2 = 0, duplicates = 0;
	ਬਣਤਰ strset shown = STRSET_INIT;
	ਪੂਰਨ do_trivial = 0;
	ਪੂਰਨ do_flush = 0;
	ਪੂਰਨ do_cookie = 0;
	ਗਿਣਤੀ fsmonitor_cookie_ਵਸਤੂ_ਨਤੀਜਾ cookie_ਨਤੀਜਾ;

	/*
	 * We expect `command` to be of the form:
	 *
	 * <command> := quit NUL
	 *            | flush NUL
	 *            | <V1-time-since-epoch-ns> NUL
	 *            | <V2-opaque-fsmonitor-token> NUL
	 */

	ਜੇ (!strcmp(command, "quit")) {
		/*
		 * A client has requested over the socket/pipe that the
		 * daemon shutdown.
		 *
		 * Tell the IPC thread pool to shutdown (which completes
		 * the await in the main thread (which can stop the
		 * fsmonitor listener thread)).
		 *
		 * There is no reply to the client.
		 */
		ਵਾਪਸ SIMPLE_IPC_QUIT;

	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(command, "flush")) {
		/*
		 * Flush all of our cached data and generate a new token
		 * just like if we lost sync with the filesystem.
		 *
		 * Then send a trivial response using the new token.
		 */
		do_flush = 1;
		do_trivial = 1;

	} ਨਹੀਂਤਾਂ ਜੇ (!skip_ਅਗੇਤਰ(command, "builtin:", &p)) {
		/* assume V1 timestamp or garbage */

		ਅੱਖਰ *p_ਸਿਰਾ;

		strtoumax(command, &p_ਸਿਰਾ, 10);
		trace_printf_ਕੁੰਜੀ(&trace_fsmonitor,
				 ((*p_ਸਿਰਾ) ?
				  "fsmonitor: invalid command line '%s'" :
				  "fsmonitor: unsupported V1 protocol '%s'"),
				 command);
		do_trivial = 1;
		do_cookie = 1;

	} ਨਹੀਂਤਾਂ {
		/* We have "builtin:*" */
		ਜੇ (fsmonitor_parse_client_token(command, &requested_token_id,
						 &requested_oldest_seq_nr)) {
			trace_printf_ਕੁੰਜੀ(&trace_fsmonitor,
					 "fsmonitor: invalid V2 protocol token '%s'",
					 command);
			do_trivial = 1;
			do_cookie = 1;

		} ਨਹੀਂਤਾਂ {
			/*
			 * We have a V2 valid token:
			 *     "builtin:<token_id>:<seq_nr>"
			 */
			do_cookie = 1;
		}
	}

	pthread_mutex_ਤਾਲਾ(&state->main_ਤਾਲਾ);

	ਜੇ (!state->current_token_ਅੰਕੜੇ)
		BUG("fsmonitor state does not have a current token");

	/*
	 * Write a cookie file inside the directory being watched in
	 * an effort to flush out existing filesystem events that we
	 * actually care about.  Suspend this client thread until we
	 * see the filesystem events for this cookie file.
	 *
	 * Creating the cookie lets us guarantee that our FS listener
	 * thread has drained the kernel queue and we are caught up
	 * with the kernel.
	 *
	 * If we cannot create the cookie (or otherwise guarantee that
	 * we are caught up), we send a trivial response.  We have to
	 * assume that there might be some very, very recent activity
	 * on the FS still in flight.
	 */
	ਜੇ (do_cookie) {
		cookie_ਨਤੀਜਾ = with_ਤਾਲਾ_wait_for_cookie(state);
		ਜੇ (cookie_ਨਤੀਜਾ != FCIR_ਵੇਖਿਆ) {
			error(_("fsmonitor: cookie_result '%d' != SEEN"),
			      cookie_ਨਤੀਜਾ);
			do_trivial = 1;
		}
	}

	ਜੇ (do_flush)
		with_ਤਾਲਾ_do_force_resync(state);

	/*
	 * We mark the current head of the batch list as "pinned" so
	 * that the listener thread will treat this item as read-only
	 * (and prevent any more paths from being added to it) from
	 * now on.
	 */
	token_ਅੰਕੜੇ = state->current_token_ਅੰਕੜੇ;
	batch_ਸਿਰ = token_ਅੰਕੜੇ->batch_ਸਿਰ;
	((ਬਣਤਰ fsmonitor_batch *)batch_ਸਿਰ)->pinned_ਸਮਾਂ = ਸਮਾਂ(ਨਿਰਾ);

	/*
	 * FSMonitor Protocol V2 requires that we send a response header
	 * with a "new current token" and then all of the paths that changed
	 * since the "requested token".  We send the seq_nr of the just-pinned
	 * head batch so that future requests from a client will be relative
	 * to it.
	 */
	with_ਤਾਲਾ_format_response_token(&response_token,
					 &token_ਅੰਕੜੇ->token_id, batch_ਸਿਰ);

	ਜਵਾਬ(ਜਵਾਬ_ਅੰਕੜੇ, response_token.buf, response_token.len + 1);
	ਕੁੱਲ_response_len += response_token.len + 1;

	trace2_ਅੰਕੜੇ_string("fsmonitor", the_repository, "response/token",
			   response_token.buf);
	trace_printf_ਕੁੰਜੀ(&trace_fsmonitor, "response token: %s",
			 response_token.buf);

	ਜੇ (!do_trivial) {
		ਜੇ (strcmp(requested_token_id.buf, token_ਅੰਕੜੇ->token_id.buf)) {
			/*
			 * The client last spoke to a different daemon
			 * instance -OR- the daemon had to resync with
			 * the filesystem (and lost events), so reject.
			 */
			trace2_ਅੰਕੜੇ_string("fsmonitor", the_repository,
					   "response/token", "different");
			do_trivial = 1;

		} ਨਹੀਂਤਾਂ ਜੇ (requested_oldest_seq_nr <
			   token_ਅੰਕੜੇ->batch_ਪੂਛ->batch_seq_nr) {
			/*
			 * The client wants older events than we have for
			 * this token_id.  This means that the end of our
			 * batch list was truncated and we cannot give the
			 * client a complete snapshot relative to their
			 * request.
			 */
			trace_printf_ਕੁੰਜੀ(&trace_fsmonitor,
					 "client requested truncated data");
			do_trivial = 1;
		}
	}

	ਜੇ (do_trivial) {
		pthread_mutex_unlock(&state->main_ਤਾਲਾ);

		ਜਵਾਬ(ਜਵਾਬ_ਅੰਕੜੇ, "/", 2);

		trace2_ਅੰਕੜੇ_intmax("fsmonitor", the_repository,
				   "response/trivial", 1);

		ਜਾਓ cleanup;
	}

	/*
	 * We're going to hold onto a pointer to the current
	 * token-data while we walk the list of batches of files.
	 * During this time, we will NOT be under the lock.
	 * So we ref-count it.
	 *
	 * This allows the listener thread to continue prepending
	 * new batches of items to the token-data (which we'll ignore).
	 *
	 * AND it allows the listener thread to do a token-reset
	 * (and install a new `current_token_data`).
	 */
	token_ਅੰਕੜੇ->client_ref_ਗਿਣਤੀ++;

	pthread_mutex_unlock(&state->main_ਤਾਲਾ);

	/*
	 * The client request is relative to the token that they sent,
	 * so walk the batch list backwards from the current head back
	 * to the batch (sequence number) they named.
	 *
	 * We use a strset to de-dup the list of pathnames.
	 *
	 * NEEDSWORK: each batch contains a list of interned strings,
	 * so we only need to do pointer comparisons here to build the
	 * hash table.  Currently, we're still comparing the string
	 * values.
	 */
	strset_init_with_options(&shown, ਨਿਰਾ, 0);
	ਲਈ (batch = batch_ਸਿਰ;
	     batch && batch->batch_seq_nr > requested_oldest_seq_nr;
	     batch = batch->ਅਗਲਾ) {
		ਮਾਪ_t k;

		ਲਈ (k = 0; k < batch->nr; k++) {
			ਅਚਲ ਅੱਖਰ *s = batch->interned_paths[k];
			ਮਾਪ_t s_len;

			ਜੇ (!strset_ਜੋੜੋ(&shown, s))
				duplicates++;
			ਨਹੀਂਤਾਂ {
				trace_printf_ਕੁੰਜੀ(&trace_fsmonitor,
						 "send[%"PRIuMAX"]: %s",
						 ਗਿਣਤੀ_2, s);

				/* Each path gets written with a trailing NUL */
				s_len = ਲੜੀ_ਲੰਬਾਈ(s) + 1;

				ਜੇ (payload.len + s_len >=
				    LARGE_PACKET_ਅੰਕੜੇ_MAX) {
					ਜਵਾਬ(ਜਵਾਬ_ਅੰਕੜੇ, payload.buf,
					      payload.len);
					ਕੁੱਲ_response_len += payload.len;
					strbuf_reset(&payload);
				}

				strbuf_ਜੋੜੋ(&payload, s, s_len);
				ਗਿਣਤੀ_2++;
			}
		}
	}

	ਜੇ (payload.len) {
		ਜਵਾਬ(ਜਵਾਬ_ਅੰਕੜੇ, payload.buf, payload.len);
		ਕੁੱਲ_response_len += payload.len;
	}

	pthread_mutex_ਤਾਲਾ(&state->main_ਤਾਲਾ);

	ਜੇ (token_ਅੰਕੜੇ->client_ref_ਗਿਣਤੀ > 0)
		token_ਅੰਕੜੇ->client_ref_ਗਿਣਤੀ--;

	ਜੇ (token_ਅੰਕੜੇ->client_ref_ਗਿਣਤੀ == 0) {
		ਜੇ (token_ਅੰਕੜੇ != state->current_token_ਅੰਕੜੇ) {
			/*
			 * The listener thread did a token-reset while we were
			 * walking the batch list.  Therefore, this token is
			 * stale and can be discarded completely.  If we are
			 * the last reader thread using this token, we own
			 * that work.
			 */
			fsmonitor_free_token_ਅੰਕੜੇ(token_ਅੰਕੜੇ);
		} ਨਹੀਂਤਾਂ ਜੇ (batch) {
			/*
			 * We are holding the lock and are the only
			 * reader of the ref-counted portion of the
			 * list, so we get the honor of seeing if the
			 * list can be truncated to save memory.
			 *
			 * The main loop did not walk to the end of the
			 * list, so this batch is the first item in the
			 * batch-list that is older than the requested
			 * end-point sequence number.  See if the tail
			 * end of the list is obsolete.
			 */
			remainder = with_ਤਾਲਾ_truncate_old_batches(state,
								    batch);
		}
	}

	pthread_mutex_unlock(&state->main_ਤਾਲਾ);

	ਜੇ (remainder)
		fsmonitor_batch__free_list(remainder);

	trace2_ਅੰਕੜੇ_intmax("fsmonitor", the_repository, "response/length", ਕੁੱਲ_response_len);
	trace2_ਅੰਕੜੇ_intmax("fsmonitor", the_repository, "response/count/files", ਗਿਣਤੀ_2);
	trace2_ਅੰਕੜੇ_intmax("fsmonitor", the_repository, "response/count/duplicates", duplicates);

cleanup:
	strset_clear(&shown);
	strbuf_release(&response_token);
	strbuf_release(&requested_token_id);
	strbuf_release(&payload);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ipc_server_application_cb handle_client;

ਸਥਿਰ ਪੂਰਨ handle_client(ਖਾਲੀ *ਅੰਕੜੇ,
			 ਅਚਲ ਅੱਖਰ *command, ਮਾਪ_t command_len,
			 ipc_server_ਜਵਾਬ_cb *ਜਵਾਬ,
			 ਬਣਤਰ ipc_server_ਜਵਾਬ_ਅੰਕੜੇ *ਜਵਾਬ_ਅੰਕੜੇ)
{
	ਬਣਤਰ fsmonitor_daemon_state *state = ਅੰਕੜੇ;
	ਪੂਰਨ ਨਤੀਜਾ;

	/*
	 * The Simple IPC API now supports {char*, len} arguments, but
	 * FSMonitor always uses proper null-terminated strings, so
	 * we can ignore the command_len argument.  (Trust, but verify.)
	 */
	ਜੇ (command_len != ਲੜੀ_ਲੰਬਾਈ(command))
		BUG("FSMonitor assumes text messages");

	trace_printf_ਕੁੰਜੀ(&trace_fsmonitor, "requested token: %s", command);

	trace2_region_enter("fsmonitor", "handle_client", the_repository);
	trace2_ਅੰਕੜੇ_string("fsmonitor", the_repository, "request", command);

	ਨਤੀਜਾ = do_handle_client(state, command, ਜਵਾਬ, ਜਵਾਬ_ਅੰਕੜੇ);

	trace2_region_leave("fsmonitor", "handle_client", the_repository);

	ਵਾਪਸ ਨਤੀਜਾ;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FSMONITOR_DIR           "fsmonitor--daemon"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FSMONITOR_COOKIE_DIR    "cookies"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FSMONITOR_COOKIE_ਅਗੇਤਰ (FSMONITOR_DIR "/" FSMONITOR_COOKIE_DIR "/")

ਗਿਣਤੀ fsmonitor_ਰਸਤਾ_type fsmonitor_classify_ਰਸਤਾ_workdir_relative(
	ਅਚਲ ਅੱਖਰ *rel)
{
	ਜੇ (fspathncmp(rel, ".git", 4))
		ਵਾਪਸ IS_WORKDIR_ਰਸਤਾ;
	rel += 4;

	ਜੇ (!*rel)
		ਵਾਪਸ IS_DOT_GIT;
	ਜੇ (*rel != '/')
		ਵਾਪਸ IS_WORKDIR_ਰਸਤਾ; /* e.g. .gitignore */
	rel++;

	ਜੇ (!fspathncmp(rel, FSMONITOR_COOKIE_ਅਗੇਤਰ,
			ਲੜੀ_ਲੰਬਾਈ(FSMONITOR_COOKIE_ਅਗੇਤਰ)))
		ਵਾਪਸ IS_INSIDE_DOT_GIT_WITH_COOKIE_ਅਗੇਤਰ;

	ਵਾਪਸ IS_INSIDE_DOT_GIT;
}

ਗਿਣਤੀ fsmonitor_ਰਸਤਾ_type fsmonitor_classify_ਰਸਤਾ_gitdir_relative(
	ਅਚਲ ਅੱਖਰ *rel)
{
	ਜੇ (!fspathncmp(rel, FSMONITOR_COOKIE_ਅਗੇਤਰ,
			ਲੜੀ_ਲੰਬਾਈ(FSMONITOR_COOKIE_ਅਗੇਤਰ)))
		ਵਾਪਸ IS_INSIDE_GITDIR_WITH_COOKIE_ਅਗੇਤਰ;

	ਵਾਪਸ IS_INSIDE_GITDIR;
}

ਸਥਿਰ ਗਿਣਤੀ fsmonitor_ਰਸਤਾ_type try_classify_workdir_abs_ਰਸਤਾ(
	ਬਣਤਰ fsmonitor_daemon_state *state,
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਅਚਲ ਅੱਖਰ *rel;

	ਜੇ (fspathncmp(ਰਸਤਾ, state->ਰਸਤਾ_worktree_watch.buf,
		       state->ਰਸਤਾ_worktree_watch.len))
		ਵਾਪਸ IS_OUTSIDE_CONE;

	rel = ਰਸਤਾ + state->ਰਸਤਾ_worktree_watch.len;

	ਜੇ (!*rel)
		ਵਾਪਸ IS_WORKDIR_ਰਸਤਾ; /* it is the root dir exactly */
	ਜੇ (*rel != '/')
		ਵਾਪਸ IS_OUTSIDE_CONE;
	rel++;

	ਵਾਪਸ fsmonitor_classify_ਰਸਤਾ_workdir_relative(rel);
}

ਗਿਣਤੀ fsmonitor_ਰਸਤਾ_type fsmonitor_classify_ਰਸਤਾ_absolute(
	ਬਣਤਰ fsmonitor_daemon_state *state,
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਅਚਲ ਅੱਖਰ *rel;
	ਗਿਣਤੀ fsmonitor_ਰਸਤਾ_type t;

	t = try_classify_workdir_abs_ਰਸਤਾ(state, ਰਸਤਾ);
	ਜੇ (state->nr_paths_watching == 1)
		ਵਾਪਸ t;
	ਜੇ (t != IS_OUTSIDE_CONE)
		ਵਾਪਸ t;

	ਜੇ (fspathncmp(ਰਸਤਾ, state->ਰਸਤਾ_gitdir_watch.buf,
		       state->ਰਸਤਾ_gitdir_watch.len))
		ਵਾਪਸ IS_OUTSIDE_CONE;

	rel = ਰਸਤਾ + state->ਰਸਤਾ_gitdir_watch.len;

	ਜੇ (!*rel)
		ਵਾਪਸ IS_GITDIR; /* it is the <gitdir> exactly */
	ਜੇ (*rel != '/')
		ਵਾਪਸ IS_OUTSIDE_CONE;
	rel++;

	ਵਾਪਸ fsmonitor_classify_ਰਸਤਾ_gitdir_relative(rel);
}

/*
 * We try to combine small batches at the front of the batch-list to avoid
 * having a long list.  This hopefully makes it a little easier when we want
 * to truncate and maintain the list.  However, we don't want the paths array
 * to just keep growing and growing with realloc, so we insert an arbitrary
 * limit.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MY_COMBINE_ਹੱਦਬੰਦੀ (1024)

ਖਾਲੀ fsmonitor_publish(ਬਣਤਰ fsmonitor_daemon_state *state,
		       ਬਣਤਰ fsmonitor_batch *batch,
		       ਅਚਲ ਬਣਤਰ string_list *cookie_ਨਾਂ)
{
	ਜੇ (!batch && !cookie_ਨਾਂ->nr)
		ਵਾਪਸ;

	pthread_mutex_ਤਾਲਾ(&state->main_ਤਾਲਾ);

	ਜੇ (batch) {
		ਬਣਤਰ fsmonitor_batch *ਸਿਰ;

		ਸਿਰ = state->current_token_ਅੰਕੜੇ->batch_ਸਿਰ;
		ਜੇ (!ਸਿਰ) {
			BUG("token does not have batch");
		} ਨਹੀਂਤਾਂ ਜੇ (ਸਿਰ->pinned_ਸਮਾਂ) {
			/*
			 * We cannot alter the current batch list
			 * because:
			 *
			 * [a] it is being transmitted to at least one
			 * client and the handle_client() thread has a
			 * ref-count, but not a lock on the batch list
			 * starting with this item.
			 *
			 * [b] it has been transmitted in the past to
			 * at least one client such that future
			 * requests are relative to this head batch.
			 *
			 * So, we can only prepend a new batch onto
			 * the front of the list.
			 */
			batch->batch_seq_nr = ਸਿਰ->batch_seq_nr + 1;
			batch->ਅਗਲਾ = ਸਿਰ;
			state->current_token_ਅੰਕੜੇ->batch_ਸਿਰ = batch;
		} ਨਹੀਂਤਾਂ ਜੇ (!ਸਿਰ->batch_seq_nr) {
			/*
			 * Batch 0 is unpinned.  See the note in
			 * `fsmonitor_new_token_data()` about why we
			 * don't need to accumulate these paths.
			 */
			fsmonitor_batch__free_list(batch);
		} ਨਹੀਂਤਾਂ ਜੇ (ਸਿਰ->nr + batch->nr > MY_COMBINE_ਹੱਦਬੰਦੀ) {
			/*
			 * The head batch in the list has never been
			 * transmitted to a client, but folding the
			 * contents of the new batch onto it would
			 * exceed our arbitrary limit, so just prepend
			 * the new batch onto the list.
			 */
			batch->batch_seq_nr = ਸਿਰ->batch_seq_nr + 1;
			batch->ਅਗਲਾ = ਸਿਰ;
			state->current_token_ਅੰਕੜੇ->batch_ਸਿਰ = batch;
		} ਨਹੀਂਤਾਂ {
			/*
			 * We are free to add the paths in the given
			 * batch onto the end of the current head batch.
			 */
			fsmonitor_batch__combine(ਸਿਰ, batch);
			fsmonitor_batch__free_list(batch);
		}
	}

	ਜੇ (cookie_ਨਾਂ->nr)
		with_ਤਾਲਾ_mark_cookies_ਵੇਖਿਆ(state, cookie_ਨਾਂ);

	pthread_mutex_unlock(&state->main_ਤਾਲਾ);
}

ਸਥਿਰ ਖਾਲੀ *fsm_health__thread_proc(ਖਾਲੀ *_state)
{
	ਬਣਤਰ fsmonitor_daemon_state *state = _state;

	trace2_thread_ਸ਼ੁਰੂ("fsm-health");

	fsm_health__loop(state);

	trace2_thread_exit();
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ *fsm_listen__thread_proc(ਖਾਲੀ *_state)
{
	ਬਣਤਰ fsmonitor_daemon_state *state = _state;

	trace2_thread_ਸ਼ੁਰੂ("fsm-listen");

	trace_printf_ਕੁੰਜੀ(&trace_fsmonitor, "Watching: worktree '%s'",
			 state->ਰਸਤਾ_worktree_watch.buf);
	ਜੇ (state->nr_paths_watching > 1)
		trace_printf_ਕੁੰਜੀ(&trace_fsmonitor, "Watching: gitdir '%s'",
				 state->ਰਸਤਾ_gitdir_watch.buf);

	fsm_listen__loop(state);

	pthread_mutex_ਤਾਲਾ(&state->main_ਤਾਲਾ);
	ਜੇ (state->current_token_ਅੰਕੜੇ &&
	    state->current_token_ਅੰਕੜੇ->client_ref_ਗਿਣਤੀ == 0)
		fsmonitor_free_token_ਅੰਕੜੇ(state->current_token_ਅੰਕੜੇ);
	state->current_token_ਅੰਕੜੇ = ਨਿਰਾ;
	pthread_mutex_unlock(&state->main_ਤਾਲਾ);

	trace2_thread_exit();
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ fsmonitor_ਲੜੀਵਾਰ_daemon_1(ਬਣਤਰ fsmonitor_daemon_state *state)
{
	ਬਣਤਰ ipc_server_opts ipc_opts = {
		.nr_ਧਾਗੇ = fsmonitor_ipc_ਧਾਗੇ,

		/*
		 * We know that there are no other active threads yet,
		 * so we can let the IPC layer temporarily chdir() if
		 * it needs to when creating the server side of the
		 * Unix domain socket.
		 */
		.uds_disallow_chdir = 0
	};
	ਪੂਰਨ health_started = 0;
	ਪੂਰਨ listener_started = 0;
	ਪੂਰਨ err = 0;

	/*
	 * Start the IPC thread pool before the we've started the file
	 * system event listener thread so that we have the IPC handle
	 * before we need it.
	 */
	ਜੇ (ipc_server_init_async(&state->ipc_server_ਅੰਕੜੇ,
				  state->ਰਸਤਾ_ipc.buf, &ipc_opts,
				  handle_client, state))
		ਵਾਪਸ error_errno(
			_("could not start IPC thread pool on '%s'"),
			state->ਰਸਤਾ_ipc.buf);

	/*
	 * Start the fsmonitor listener thread to collect filesystem
	 * events.
	 */
	ਜੇ (pthread_create(&state->listener_thread, ਨਿਰਾ,
			   fsm_listen__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_ਅੰਕੜੇ);
		err = error(_("could not start fsmonitor listener thread"));
		ਜਾਓ cleanup;
	}
	listener_started = 1;

	/*
	 * Start the health thread to watch over our process.
	 */
	ਜੇ (pthread_create(&state->health_thread, ਨਿਰਾ,
			   fsm_health__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_ਅੰਕੜੇ);
		err = error(_("could not start fsmonitor health thread"));
		ਜਾਓ cleanup;
	}
	health_started = 1;

	/*
	 * The daemon is now fully functional in background threads.
	 * Our primary thread should now just wait while the threads
	 * do all the work.
	 */
cleanup:
	/*
	 * Wait for the IPC thread pool to shutdown (whether by client
	 * request, from filesystem activity, or an error).
	 */
	ipc_server_await(state->ipc_server_ਅੰਕੜੇ);

	/*
	 * The fsmonitor listener thread may have received a shutdown
	 * event from the IPC thread pool, but it doesn't hurt to tell
	 * it again.  And wait for it to shutdown.
	 */
	ਜੇ (listener_started) {
		fsm_listen__stop_async(state);
		pthread_join(state->listener_thread, ਨਿਰਾ);
	}

	ਜੇ (health_started) {
		fsm_health__stop_async(state);
		pthread_join(state->health_thread, ਨਿਰਾ);
	}

	ਜੇ (err)
		ਵਾਪਸ err;
	ਜੇ (state->listen_error_ਕੋਡ)
		ਵਾਪਸ state->listen_error_ਕੋਡ;
	ਜੇ (state->health_error_ਕੋਡ)
		ਵਾਪਸ state->health_error_ਕੋਡ;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ fsmonitor_ਲੜੀਵਾਰ_daemon(ਖਾਲੀ)
{
	ਬਣਤਰ fsmonitor_daemon_state state;
	ਅਚਲ ਅੱਖਰ *home;
	ਪੂਰਨ err;

	memset(&state, 0, ਆਕਾਰ(state));

	hashmap_init(&state.cookies, cookies_cmp, ਨਿਰਾ, 0);
	pthread_mutex_init(&state.main_ਤਾਲਾ, ਨਿਰਾ);
	pthread_cond_init(&state.cookies_cond, ਨਿਰਾ);
	state.listen_error_ਕੋਡ = 0;
	state.health_error_ਕੋਡ = 0;
	state.current_token_ਅੰਕੜੇ = fsmonitor_new_token_ਅੰਕੜੇ();

	/* Prepare to (recursively) watch the <worktree-root> directory. */
	strbuf_init(&state.ਰਸਤਾ_worktree_watch, 0);
	strbuf_addstr(&state.ਰਸਤਾ_worktree_watch,
		      absolute_ਰਸਤਾ(repo_get_work_ਰੁੱਖ(the_repository)));
	state.nr_paths_watching = 1;

	strbuf_init(&state.alias.alias, 0);
	strbuf_init(&state.alias.points_ਮੰਜ਼ਿਲ, 0);
	ਜੇ ((err = fsmonitor__get_alias(state.ਰਸਤਾ_worktree_watch.buf, &state.alias)))
		ਜਾਓ ਪੂਰਾ;

	/*
	 * We create and delete cookie files somewhere inside the .git
	 * directory to help us keep sync with the file system.  If
	 * ".git" is not a directory, then <gitdir> is not inside the
	 * cone of <worktree-root>, so set up a second watch to watch
	 * the <gitdir> so that we get events for the cookie files.
	 */
	strbuf_init(&state.ਰਸਤਾ_gitdir_watch, 0);
	strbuf_addbuf(&state.ਰਸਤਾ_gitdir_watch, &state.ਰਸਤਾ_worktree_watch);
	strbuf_addstr(&state.ਰਸਤਾ_gitdir_watch, "/.git");
	ਜੇ (!is_directory(state.ਰਸਤਾ_gitdir_watch.buf)) {
		strbuf_reset(&state.ਰਸਤਾ_gitdir_watch);
		strbuf_addstr(&state.ਰਸਤਾ_gitdir_watch,
			      absolute_ਰਸਤਾ(repo_get_git_dir(the_repository)));
		strbuf_strip_suffix(&state.ਰਸਤਾ_gitdir_watch, "/.");
		state.nr_paths_watching = 2;
	}

	/*
	 * We will write filesystem syncing cookie files into
	 * <gitdir>/<fsmonitor-dir>/<cookie-dir>/<pid>-<seq>.
	 *
	 * The extra layers of subdirectories here keep us from
	 * changing the mtime on ".git/" or ".git/foo/" when we create
	 * or delete cookie files.
	 *
	 * There have been problems with some IDEs that do a
	 * non-recursive watch of the ".git/" directory and run a
	 * series of commands any time something happens.
	 *
	 * For example, if we place our cookie files directly in
	 * ".git/" or ".git/foo/" then a `git status` (or similar
	 * command) from the IDE will cause a cookie file to be
	 * created in one of those dirs.  This causes the mtime of
	 * those dirs to change.  This triggers the IDE's watch
	 * notification.  This triggers the IDE to run those commands
	 * again.  And the process repeats and the machine never goes
	 * idle.
	 *
	 * Adding the extra layers of subdirectories prevents the
	 * mtime of ".git/" and ".git/foo" from changing when a
	 * cookie file is created.
	 */
	strbuf_init(&state.ਰਸਤਾ_cookie_ਅਗੇਤਰ, 0);
	strbuf_addbuf(&state.ਰਸਤਾ_cookie_ਅਗੇਤਰ, &state.ਰਸਤਾ_gitdir_watch);

	strbuf_addch(&state.ਰਸਤਾ_cookie_ਅਗੇਤਰ, '/');
	strbuf_addstr(&state.ਰਸਤਾ_cookie_ਅਗੇਤਰ, FSMONITOR_DIR);
	mkdir(state.ਰਸਤਾ_cookie_ਅਗੇਤਰ.buf, 0777);

	strbuf_addch(&state.ਰਸਤਾ_cookie_ਅਗੇਤਰ, '/');
	strbuf_addstr(&state.ਰਸਤਾ_cookie_ਅਗੇਤਰ, FSMONITOR_COOKIE_DIR);
	mkdir(state.ਰਸਤਾ_cookie_ਅਗੇਤਰ.buf, 0777);

	strbuf_addch(&state.ਰਸਤਾ_cookie_ਅਗੇਤਰ, '/');

	/*
	 * We create a named-pipe or unix domain socket inside of the
	 * ".git" directory.  (Well, on Windows, we base our named
	 * pipe in the NPFS on the absolute path of the git
	 * directory.)
	 */
	strbuf_init(&state.ਰਸਤਾ_ipc, 0);
	strbuf_addstr(&state.ਰਸਤਾ_ipc,
		absolute_ਰਸਤਾ(fsmonitor_ipc_get_ਰਸਤਾ(the_repository)));

	/*
	 * Confirm that we can create platform-specific resources for the
	 * filesystem listener before we bother starting all the threads.
	 */
	ਜੇ (fsm_listen__ctor(&state)) {
		err = error(_("could not initialize listener thread"));
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (fsm_health__ctor(&state)) {
		err = error(_("could not initialize health thread"));
		ਜਾਓ ਪੂਰਾ;
	}

	/*
	 * CD out of the worktree root directory.
	 *
	 * The common Git startup mechanism causes our CWD to be the
	 * root of the worktree.  On Windows, this causes our process
	 * to hold a locked handle on the CWD.  This prevents the
	 * worktree from being moved or deleted while the daemon is
	 * running.
	 *
	 * We assume that our FS and IPC listener threads have either
	 * opened all of the handles that they need or will do
	 * everything using absolute paths.
	 */
	home = getenv("HOME");
	ਜੇ (home && *home && chdir(home))
		die_errno(_("could not cd home '%s'"), home);

	err = fsmonitor_ਲੜੀਵਾਰ_daemon_1(&state);

ਪੂਰਾ:
	fsmonitor_free_token_ਅੰਕੜੇ(state.current_token_ਅੰਕੜੇ);
	state.current_token_ਅੰਕੜੇ = ਨਿਰਾ;
	pthread_cond_destroy(&state.cookies_cond);
	pthread_mutex_destroy(&state.main_ਤਾਲਾ);
	{
		ਬਣਤਰ hashmap_iter iter;
		ਬਣਤਰ fsmonitor_cookie_ਵਸਤੂ *cookie;

		hashmap_for_each_ਇੰਦਰਾਜ(&state.cookies, &iter, cookie, ਇੰਦਰਾਜ)
			ਮੁਕਤ(cookie->name);
		hashmap_clear_and_free(&state.cookies,
				       ਬਣਤਰ fsmonitor_cookie_ਵਸਤੂ, ਇੰਦਰਾਜ);
	}
	fsm_listen__dtor(&state);
	fsm_health__dtor(&state);

	ipc_server_free(state.ipc_server_ਅੰਕੜੇ);

	strbuf_release(&state.ਰਸਤਾ_worktree_watch);
	strbuf_release(&state.ਰਸਤਾ_gitdir_watch);
	strbuf_release(&state.ਰਸਤਾ_cookie_ਅਗੇਤਰ);
	strbuf_release(&state.ਰਸਤਾ_ipc);
	strbuf_release(&state.alias.alias);
	strbuf_release(&state.alias.points_ਮੰਜ਼ਿਲ);

	ਵਾਪਸ err;
}

ਸਥਿਰ ਪੂਰਨ try_ਮੰਜ਼ਿਲ_ਲੜੀਵਾਰ_foreground_daemon(ਪੂਰਨ detach_console)
{
	/*
	 * Technically, we don't need to probe for an existing daemon
	 * process, since we could just call `fsmonitor_run_daemon()`
	 * and let it fail if the pipe/socket is busy.
	 *
	 * However, this method gives us a nicer error message for a
	 * common error case.
	 */
	ਜੇ (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	ਜੇ (fsmonitor__announce_startup) {
		fprintf(stderr, _("running fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	ਜੇ (detach_console) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GIT_WINDOWS_NATIVE
		FreeConsole();
#ਨਹੀਂਤਾਂ
		/*
		 * Create a new session so that the daemon is detached
		 * from the parent's process group.  This prevents
		 * shells with job control (e.g. bash with "set -m")
		 * from waiting on the daemon when they wait for a
		 * foreground command that implicitly spawned it.
		 */
		ਜੇ (setsid() == -1)
			warning_errno(_("setsid failed"));
#ਜੇ_ਅੰਤ
	}

	ਵਾਪਸ !!fsmonitor_ਲੜੀਵਾਰ_daemon();
}

ਸਥਿਰ ਸ਼ੁਰੂ_bg_wait_cb bg_wait_cb;

ਸਥਿਰ ਪੂਰਨ bg_wait_cb(ਅਚਲ ਬਣਤਰ ਬੱਚਾ_process *cp ਅਣਵਰਤਿਆ,
		      ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਗਿਣਤੀ ipc_ਸਰਗਰਮ_state s = fsmonitor_ipc__get_state();

	ਚੋਣ (s) {
	ਹਾਲਤ IPC_STATE__LISTENING:
		/* child is "ready" */
		ਵਾਪਸ 0;

	ਹਾਲਤ IPC_STATE__NOT_LISTENING:
	ਹਾਲਤ IPC_STATE_ਰਸਤਾ_NOT_ਮਿਲਿਆ:
		/* give child more time */
		ਵਾਪਸ 1;

	ਮੂਲ:
	ਹਾਲਤ IPC_STATE_INVALID_ਰਸਤਾ:
	ਹਾਲਤ IPC_STATE__OTHER_ERROR:
		/* all the time in world won't help */
		ਵਾਪਸ -1;
	}
}

ਸਥਿਰ ਪੂਰਨ try_ਮੰਜ਼ਿਲ_ਸ਼ੁਰੂ_background_daemon(ਖਾਲੀ)
{
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
	ਗਿਣਤੀ ਸ਼ੁਰੂ_bg_ਨਤੀਜਾ sbgr;

	/*
	 * Before we try to create a background daemon process, see
	 * if a daemon process is already listening.  This makes it
	 * easier for us to report an already-listening error to the
	 * console, since our spawn/daemon can only report the success
	 * of creating the background process (and not whether it
	 * immediately exited).
	 */
	ਜੇ (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	ਜੇ (fsmonitor__announce_startup) {
		fprintf(stderr, _("starting fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	cp.git_cmd = 1;

	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "fsmonitor--daemon");
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "run");
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--detach");
	strvec_pushf(&cp.ਦਲੀਲਾਂ, "--ipc-threads=%d", fsmonitor_ipc_ਧਾਗੇ);

	cp.no_stdin = 1;
	cp.no_stdout = 1;
	cp.no_stderr = 1;
	cp.close_fd_above_stderr = 1;

	sbgr = ਸ਼ੁਰੂ_bg_command(&cp, bg_wait_cb, ਨਿਰਾ,
				fsmonitor_ਸ਼ੁਰੂ_timeout_sec);

	ਚੋਣ (sbgr) {
	ਹਾਲਤ SBGR_READY:
		ਵਾਪਸ 0;

	ਮੂਲ:
	ਹਾਲਤ SBGR_ERROR:
	ਹਾਲਤ SBGR_CB_ERROR:
		ਵਾਪਸ error(_("daemon failed to start"));

	ਹਾਲਤ SBGR_TIMEOUT:
		ਵਾਪਸ error(_("daemon not online yet"));

	ਹਾਲਤ SBGR_DIED:
		ਵਾਪਸ error(_("daemon terminated"));
	}
}

ਪੂਰਨ cmd_fsmonitor__daemon(ਪੂਰਨ argc,
			  ਅਚਲ ਅੱਖਰ **argv,
			  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *subcmd;
	ਗਿਣਤੀ fsmonitor_reason reason;
	ਪੂਰਨ detach_console = 0;

	ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "detach", &detach_console, N_("detach from console")),
		OPT_INTEGER(0, "ipc-threads",
			    &fsmonitor_ipc_ਧਾਗੇ,
			    N_("use <n> ipc worker threads")),
		OPT_INTEGER(0, "start-timeout",
			    &fsmonitor_ਸ਼ੁਰੂ_timeout_sec,
			    N_("max seconds to wait for background daemon startup")),

		OPT_ਸਿਰਾ()
	};

	repo_config(the_repository, fsmonitor_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_fsmonitor__daemon_usage, 0);
	ਜੇ (argc != 1)
		usage_with_options(builtin_fsmonitor__daemon_usage, options);
	subcmd = argv[0];

	ਜੇ (fsmonitor_ipc_ਧਾਗੇ < 1)
		die(_("invalid 'ipc-threads' value (%d)"),
		    fsmonitor_ipc_ਧਾਗੇ);

	prepare_repo_settings(the_repository);
	/*
	 * If the repo is fsmonitor-compatible, explicitly set IPC-mode
	 * (without bothering to load the `core.fsmonitor` config settings).
	 *
	 * If the repo is not compatible, the repo-settings will be set to
	 * incompatible rather than IPC, so we can use one of the __get
	 * routines to detect the discrepancy.
	 */
	fsm_settings_ਸੈੱਟ_ipc(the_repository);

	reason = fsm_settings__get_reason(the_repository);
	ਜੇ (reason > FSMONITOR_REASON_OK)
		die("%s",
		    fsm_settings_get_incompatible_ਸੁਨੇਹਾ(the_repository,
						       reason));

	ਜੇ (!strcmp(subcmd, "start"))
		ਵਾਪਸ !!try_ਮੰਜ਼ਿਲ_ਸ਼ੁਰੂ_background_daemon();

	ਜੇ (!strcmp(subcmd, "run"))
		ਵਾਪਸ !!try_ਮੰਜ਼ਿਲ_ਲੜੀਵਾਰ_foreground_daemon(detach_console);

	ਜੇ (!strcmp(subcmd, "stop"))
		ਵਾਪਸ !!do_as_client__send_stop();

	ਜੇ (!strcmp(subcmd, "status"))
		ਵਾਪਸ !!do_as_client_ਸਥਿਤੀ();

	die(_("Unhandled subcommand '%s'"), subcmd);
}

#ਨਹੀਂਤਾਂ
ਪੂਰਨ cmd_fsmonitor__daemon(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ, ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_fsmonitor__daemon_usage, options);

	die(_("fsmonitor--daemon not supported on this platform"));
}
#ਜੇ_ਅੰਤ
