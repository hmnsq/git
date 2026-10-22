#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "fsmonitor-ll.h"
#ಅನ್ತರ್ಭಾವ "fsmonitor-ipc.h"
#ಅನ್ತರ್ಭಾವ "fsmonitor-settings.h"
#ಅನ್ತರ್ಭಾವ "compat/fsmonitor/fsm-health.h"
#ಅನ್ತರ್ಭಾವ "compat/fsmonitor/fsm-listen.h"
#ಅನ್ತರ್ಭಾವ "fsmonitor--daemon.h"

#ಅನ್ತರ್ಭಾವ "simple-ipc.h"
#ಅನ್ತರ್ಭಾವ "strmap.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "trace.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_fsmonitor__daemon_usage[] = {
	N_("git fsmonitor--daemon start [<options>]"),
	N_("git fsmonitor--daemon run [<options>]"),
	"git fsmonitor--daemon stop",
	"git fsmonitor--daemon status",
	ಖಾಲಿ
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_FSMONITOR_DAEMON_BACKEND
/*
 * Global state loaded from config.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು FSMONITOR_IPC_ಎಳೆಗಳು "fsmonitor.ipcthreads"
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsmonitor_ipc_ಎಳೆಗಳು = 8;

#ವ್ಯಾಖ್ಯಾನಿಸು FSMONITOR_ಆರಂಭ_TIMEOUT "fsmonitor.starttimeout"
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsmonitor_ಆರಂಭ_timeout_sec = 60;

#ವ್ಯಾಖ್ಯಾನಿಸು FSMONITOR__ANNOUNCE_STARTUP "fsmonitor.announcestartup"
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsmonitor__announce_startup = 0;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsmonitor_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(var, FSMONITOR_IPC_ಎಳೆಗಳು)) {
		ಪೂರ್ಣಾಂಕ i = git_config_int(var, ಮೌಲ್ಯ, ctx->kvi);
		ಆದರೆ (i < 1)
			ಹಿಂತಿರುಗಿ error(_("value of '%s' out of range: %d"),
				     FSMONITOR_IPC_ಎಳೆಗಳು, i);
		fsmonitor_ipc_ಎಳೆಗಳು = i;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, FSMONITOR_ಆರಂಭ_TIMEOUT)) {
		ಪೂರ್ಣಾಂಕ i = git_config_int(var, ಮೌಲ್ಯ, ctx->kvi);
		ಆದರೆ (i < 0)
			ಹಿಂತಿರುಗಿ error(_("value of '%s' out of range: %d"),
				     FSMONITOR_ಆರಂಭ_TIMEOUT, i);
		fsmonitor_ಆರಂಭ_timeout_sec = i;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, FSMONITOR__ANNOUNCE_STARTUP)) {
		ಪೂರ್ಣಾಂಕ is_bool;
		ಪೂರ್ಣಾಂಕ i = git_config_bool_or_int(var, ಮೌಲ್ಯ, ctx->kvi, &is_bool);
		ಆದರೆ (i < 0)
			ಹಿಂತಿರುಗಿ error(_("value of '%s' not bool or int: %d"),
				     var, i);
		fsmonitor__announce_startup = i;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

/*
 * Acting as a CLIENT.
 *
 * Send a "quit" command to the `git-fsmonitor--daemon` (if running)
 * and wait for it to shutdown.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_as_client__send_stop(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf answer = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret;
	ಪೂರ್ಣಾಂಕ max_wait_ms = 30000;
	ಪೂರ್ಣಾಂಕ elapsed_ms = 0;

	ret = fsmonitor_ipc__send_command("quit", &answer);

	/* The quit command does not return any response data. */
	strbuf_release(&answer);

	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	trace2_region_enter("fsm_client", "polling-for-daemon-exit", ಖಾಲಿ);
	ಆಗ (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING) {
		ಆದರೆ (elapsed_ms >= max_wait_ms) {
			trace2_region_leave("fsm_client",
					    "polling-for-daemon-exit", ಖಾಲಿ);
			ಹಿಂತಿರುಗಿ error(_("daemon did not stop within %d seconds"),
				     max_wait_ms / 1000);
		}
		sleep_millisec(50);
		elapsed_ms += 50;
	}
	trace2_region_leave("fsm_client", "polling-for-daemon-exit", ಖಾಲಿ);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_as_client_ಸ್ಥಿತಿ(ಶೂನ್ಯ)
{
	ಎಣಿಕೆ ipc_ಸಕ್ರಿಯ_state state = fsmonitor_ipc__get_state();

	ಆಯ್ಕೆ (state) {
	ಸ್ಥಿತಿ IPC_STATE__LISTENING:
		ಬರೆ(_("fsmonitor-daemon is watching '%s'\n"),
		       the_repository->worktree);
		ಹಿಂತಿರುಗಿ 0;

	ಪೂರ್ವನಿಯೋಜಿತ:
		ಬರೆ(_("fsmonitor-daemon is not watching '%s'\n"),
		       the_repository->worktree);
		ಹಿಂತಿರುಗಿ 1;
	}
}

ಎಣಿಕೆ fsmonitor_cookie_ಅಂಶ_ಫಲಿತಾಂಶ {
	FCIR_ERROR = -1, /* could not create cookie file ? */
	FCIR_INIT,
	FCIR_ನೋಡಿದ,
	FCIR_ABORT,
};

ರಚನೆ fsmonitor_cookie_ಅಂಶ {
	ರಚನೆ hashmap_ನಮೂದು ನಮೂದು;
	ಅಕ್ಷರ *name;
	ಎಣಿಕೆ fsmonitor_cookie_ಅಂಶ_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cookies_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ,
		       ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *he1,
		       ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *he2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *keydata)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ fsmonitor_cookie_ಅಂಶ *a =
		container_ದ(he1, ಸ್ಥಿರಾಂಕ ರಚನೆ fsmonitor_cookie_ಅಂಶ, ನಮೂದು);
	ಸ್ಥಿರಾಂಕ ರಚನೆ fsmonitor_cookie_ಅಂಶ *b =
		container_ದ(he2, ಸ್ಥಿರಾಂಕ ರಚನೆ fsmonitor_cookie_ಅಂಶ, ನಮೂದು);

	ಹಿಂತಿರುಗಿ strcmp(a->name, keydata ? keydata : b->name);
}

ಸ್ಥಿರ ಎಣಿಕೆ fsmonitor_cookie_ಅಂಶ_ಫಲಿತಾಂಶ with_ಬೀಗ_wait_for_cookie(
	ರಚನೆ fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	ಪೂರ್ಣಾಂಕ fd;
	ರಚನೆ fsmonitor_cookie_ಅಂಶ *cookie;
	ರಚನೆ strbuf cookie_pathname = STRBUF_INIT;
	ರಚನೆ strbuf cookie_filename = STRBUF_INIT;
	ಎಣಿಕೆ fsmonitor_cookie_ಅಂಶ_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ;
	ಪೂರ್ಣಾಂಕ my_cookie_seq;

	CALLOC_ARRAY(cookie, 1);

	my_cookie_seq = state->cookie_seq++;

	strbuf_addf(&cookie_filename, "%i-%i", getpid(), my_cookie_seq);

	strbuf_addbuf(&cookie_pathname, &state->ಮಾರ್ಗ_cookie_ಪೂರ್ವಪ್ರತ್ಯಯ);
	strbuf_addbuf(&cookie_pathname, &cookie_filename);

	cookie->name = strbuf_detach(&cookie_filename, ಖಾಲಿ);
	cookie->ಫಲಿತಾಂಶ = FCIR_INIT;
	hashmap_ನಮೂದು_init(&cookie->ನಮೂದು, strhash(cookie->name));

	hashmap_ಕೂಡಿಸು(&state->cookies, &cookie->ನಮೂದು);

	trace_printf_ಕೀಲಿ(&trace_fsmonitor, "cookie-wait: '%s' '%s'",
			 cookie->name, cookie_pathname.buf);

	/*
	 * Create the cookie file on disk and then wait for a notification
	 * that the listener thread has seen it.
	 */
	fd = open(cookie_pathname.buf, O_WRONLY | O_CREAT | O_EXCL, 0600);
	ಆದರೆ (fd < 0) {
		error_errno(_("could not create fsmonitor cookie '%s'"),
			    cookie->name);

		cookie->ಫಲಿತಾಂಶ = FCIR_ERROR;
		ಹೋಗು ಮುಗಿದ;
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
		ರಚನೆ timeval now;
		ರಚನೆ timespec ts;
		ಪೂರ್ಣಾಂಕ err = 0;

		gettimeofday(&now, ಖಾಲಿ);
		ts.tv_sec = now.tv_sec + 1;
		ts.tv_nsec = now.tv_usec * 1000;

		ಆಗ (cookie->ಫಲಿತಾಂಶ == FCIR_INIT && !err)
			err = pthread_cond_timedwait(&state->cookies_cond,
						     &state->main_ಬೀಗ,
						     &ts);
		ಆದರೆ (err == ETIMEDOUT && cookie->ಫಲಿತಾಂಶ == FCIR_INIT) {
			trace_printf_ಕೀಲಿ(&trace_fsmonitor,
					 "cookie_wait timed out");
			cookie->ಫಲಿತಾಂಶ = FCIR_ERROR;
		}
	}

ಮುಗಿದ:
	hashmap_remove(&state->cookies, &cookie->ನಮೂದು, ಖಾಲಿ);

	ಫಲಿತಾಂಶ = cookie->ಫಲಿತಾಂಶ;

	ಬಿಡುಗಡೆ(cookie->name);
	ಬಿಡುಗಡೆ(cookie);
	strbuf_release(&cookie_pathname);

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/*
 * Mark these cookies as _SEEN and wake up the corresponding client threads.
 */
ಸ್ಥಿರ ಶೂನ್ಯ with_ಬೀಗ_mark_cookies_ನೋಡಿದ(ರಚನೆ fsmonitor_daemon_state *state,
					 ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *cookie_ಹೆಸರುಗಳು)
{
	/* assert current thread holding state->main_lock */

	ಪೂರ್ಣಾಂಕ k;
	ಪೂರ್ಣಾಂಕ nr_ನೋಡಿದ = 0;

	ಗಾಗಿ (k = 0; k < cookie_ಹೆಸರುಗಳು->nr; k++) {
		ರಚನೆ fsmonitor_cookie_ಅಂಶ ಕೀಲಿ;
		ರಚನೆ fsmonitor_cookie_ಅಂಶ *cookie;

		ಕೀಲಿ.name = cookie_ಹೆಸರುಗಳು->ವಸ್ತುಗಳು[k].string;
		hashmap_ನಮೂದು_init(&ಕೀಲಿ.ನಮೂದು, strhash(ಕೀಲಿ.name));

		cookie = hashmap_get_ನಮೂದು(&state->cookies, &ಕೀಲಿ, ನಮೂದು, ಖಾಲಿ);
		ಆದರೆ (cookie) {
			trace_printf_ಕೀಲಿ(&trace_fsmonitor, "cookie-seen: '%s'",
					 cookie->name);
			cookie->ಫಲಿತಾಂಶ = FCIR_ನೋಡಿದ;
			nr_ನೋಡಿದ++;
		}
	}

	ಆದರೆ (nr_ನೋಡಿದ)
		pthread_cond_ಪ್ರಸಾರ(&state->cookies_cond);
}

/*
 * Set _ABORT on all pending cookies and wake up all client threads.
 */
ಸ್ಥಿರ ಶೂನ್ಯ with_ಬೀಗ_abort_all_cookies(ರಚನೆ fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	ರಚನೆ hashmap_iter iter;
	ರಚನೆ fsmonitor_cookie_ಅಂಶ *cookie;
	ಪೂರ್ಣಾಂಕ nr_aborted = 0;

	hashmap_for_each_ನಮೂದು(&state->cookies, &iter, cookie, ನಮೂದು) {
		trace_printf_ಕೀಲಿ(&trace_fsmonitor, "cookie-abort: '%s'",
				 cookie->name);
		cookie->ಫಲಿತಾಂಶ = FCIR_ABORT;
		nr_aborted++;
	}

	ಆದರೆ (nr_aborted)
		pthread_cond_ಪ್ರಸಾರ(&state->cookies_cond);
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
ರಚನೆ fsmonitor_token_ದತ್ತಾಂಶ {
	ರಚನೆ strbuf token_id;
	ರಚನೆ fsmonitor_batch *batch_ತಲೆ;
	ರಚನೆ fsmonitor_batch *batch_ಬಾಲ;
	uint64_t client_ref_ಎಣಿಕೆ;
};

ರಚನೆ fsmonitor_batch {
	ರಚನೆ fsmonitor_batch *ಮುಂದಿನ;
	uint64_t batch_seq_nr;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **interned_paths;
	ಗಾತ್ರ_t nr, alloc;
	ಸಮಯ_t pinned_ಸಮಯ;
};

ಸ್ಥಿರ ರಚನೆ fsmonitor_token_ದತ್ತಾಂಶ *fsmonitor_new_token_ದತ್ತಾಂಶ(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ test_env_ಮೌಲ್ಯ = -1;
	ಸ್ಥಿರ uint64_t flush_ಎಣಿಕೆ = 0;
	ರಚನೆ fsmonitor_token_ದತ್ತಾಂಶ *token;
	ರಚನೆ fsmonitor_batch *batch;

	CALLOC_ARRAY(token, 1);
	batch = fsmonitor_batch__new();

	strbuf_init(&token->token_id, 0);
	token->batch_ತಲೆ = batch;
	token->batch_ಬಾಲ = batch;
	token->client_ref_ಎಣಿಕೆ = 0;

	ಆದರೆ (test_env_ಮೌಲ್ಯ < 0)
		test_env_ಮೌಲ್ಯ = git_env_bool("GIT_TEST_FSMONITOR_TOKEN", 0);

	ಆದರೆ (!test_env_ಮೌಲ್ಯ) {
		ರಚನೆ timeval tv;
		ರಚನೆ tm tm;
		ಸಮಯ_t secs;

		gettimeofday(&tv, ಖಾಲಿ);
		secs = tv.tv_sec;
		gmtime_r(&secs, &tm);

		strbuf_addf(&token->token_id,
			    "%"PRIu64".%d.%4d%02d%02dT%02d%02d%02d.%06ldZ",
			    flush_ಎಣಿಕೆ++,
			    getpid(),
			    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			    tm.tm_hour, tm.tm_min, tm.tm_sec,
			    (ದೀರ್ಘ)tv.tv_usec);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addf(&token->token_id, "test_%08x", test_env_ಮೌಲ್ಯ++);
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
	ಆದರೆ (test_env_ಮೌಲ್ಯ)
		batch->pinned_ಸಮಯ = ಸಮಯ(ಖಾಲಿ);

	ಹಿಂತಿರುಗಿ token;
}

ರಚನೆ fsmonitor_batch *fsmonitor_batch__new(ಶೂನ್ಯ)
{
	ರಚನೆ fsmonitor_batch *batch;

	CALLOC_ARRAY(batch, 1);

	ಹಿಂತಿರುಗಿ batch;
}

ಶೂನ್ಯ fsmonitor_batch__free_list(ರಚನೆ fsmonitor_batch *batch)
{
	ಆಗ (batch) {
		ರಚನೆ fsmonitor_batch *ಮುಂದಿನ = batch->ಮುಂದಿನ;

		/*
		 * The actual strings within the array of this batch
		 * are interned, so we don't own them.  We only own
		 * the array.
		 */
		ಬಿಡುಗಡೆ(batch->interned_paths);
		ಬಿಡುಗಡೆ(batch);

		batch = ಮುಂದಿನ;
	}
}

ಶೂನ್ಯ fsmonitor_batch_ಕೂಡಿಸು_ಮಾರ್ಗ(ರಚನೆ fsmonitor_batch *batch,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *interned_ಮಾರ್ಗ = strintern(ಮಾರ್ಗ);

	trace_printf_ಕೀಲಿ(&trace_fsmonitor, "event: %s", interned_ಮಾರ್ಗ);

	ALLOC_GROW(batch->interned_paths, batch->nr + 1, batch->alloc);
	batch->interned_paths[batch->nr++] = interned_ಮಾರ್ಗ;
}

ಸ್ಥಿರ ಶೂನ್ಯ fsmonitor_batch__combine(ರಚನೆ fsmonitor_batch *batch_dest,
				     ಸ್ಥಿರಾಂಕ ರಚನೆ fsmonitor_batch *batch_src)
{
	ಗಾತ್ರ_t k;

	ALLOC_GROW(batch_dest->interned_paths,
		   batch_dest->nr + batch_src->nr + 1,
		   batch_dest->alloc);

	ಗಾಗಿ (k = 0; k < batch_src->nr; k++)
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
#ವ್ಯಾಖ್ಯಾನಿಸು MY_ಸಮಯ_DELAY_SECONDS (5 * 60) /* seconds */

ಸ್ಥಿರ ರಚನೆ fsmonitor_batch *with_ಬೀಗ_truncate_old_batches(
	ರಚನೆ fsmonitor_daemon_state *state,
	ಸ್ಥಿರಾಂಕ ರಚನೆ fsmonitor_batch *batch_marker)
{
	/* assert current thread holding state->main_lock */

	ಸ್ಥಿರಾಂಕ ರಚನೆ fsmonitor_batch *batch;
	ರಚನೆ fsmonitor_batch *remainder;

	ಆದರೆ (!batch_marker)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	trace_printf_ಕೀಲಿ(&trace_fsmonitor, "Truncate: mark (%"PRIu64",%"PRIu64")",
			 batch_marker->batch_seq_nr,
			 (uint64_t)batch_marker->pinned_ಸಮಯ);

	ಗಾಗಿ (batch = batch_marker; batch; batch = batch->ಮುಂದಿನ) {
		ಸಮಯ_t t;

		ಆದರೆ (!batch->pinned_ಸಮಯ) /* an overflow batch */
			ಮುಂದುವರಿ;

		t = batch->pinned_ಸಮಯ + MY_ಸಮಯ_DELAY_SECONDS;
		ಆದರೆ (t > batch_marker->pinned_ಸಮಯ) /* too close to marker */
			ಮುಂದುವರಿ;

		ಹೋಗು truncate_past_ಇಲ್ಲಿ;
	}

	ಹಿಂತಿರುಗಿ ಖಾಲಿ;

truncate_past_ಇಲ್ಲಿ:
	state->current_token_ದತ್ತಾಂಶ->batch_ಬಾಲ = (ರಚನೆ fsmonitor_batch *)batch;

	remainder = ((ರಚನೆ fsmonitor_batch *)batch)->ಮುಂದಿನ;
	((ರಚನೆ fsmonitor_batch *)batch)->ಮುಂದಿನ = ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ remainder;
}

ಸ್ಥಿರ ಶೂನ್ಯ fsmonitor_free_token_ದತ್ತಾಂಶ(ರಚನೆ fsmonitor_token_ದತ್ತಾಂಶ *token)
{
	ಆದರೆ (!token)
		ಹಿಂತಿರುಗಿ;

	ಪ್ರತಿಜ್ಞಾ(token->client_ref_ಎಣಿಕೆ == 0);

	strbuf_release(&token->token_id);

	fsmonitor_batch__free_list(token->batch_ತಲೆ);

	ಬಿಡುಗಡೆ(token);
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
ಸ್ಥಿರ ಶೂನ್ಯ with_ಬೀಗ_do_force_resync(ರಚನೆ fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	ರಚನೆ fsmonitor_token_ದತ್ತಾಂಶ *free_me = ಖಾಲಿ;
	ರಚನೆ fsmonitor_token_ದತ್ತಾಂಶ *new_one = ಖಾಲಿ;

	new_one = fsmonitor_new_token_ದತ್ತಾಂಶ();

	ಆದರೆ (state->current_token_ದತ್ತಾಂಶ->client_ref_ಎಣಿಕೆ == 0)
		free_me = state->current_token_ದತ್ತಾಂಶ;
	state->current_token_ದತ್ತಾಂಶ = new_one;

	fsmonitor_free_token_ದತ್ತಾಂಶ(free_me);

	with_ಬೀಗ_abort_all_cookies(state);
}

ಶೂನ್ಯ fsmonitor_force_resync(ರಚನೆ fsmonitor_daemon_state *state)
{
	pthread_mutex_ಬೀಗ(&state->main_ಬೀಗ);
	with_ಬೀಗ_do_force_resync(state);
	pthread_mutex_unlock(&state->main_ಬೀಗ);
}

/*
 * Format an opaque token string to send to the client.
 */
ಸ್ಥಿರ ಶೂನ್ಯ with_ಬೀಗ_format_response_token(
	ರಚನೆ strbuf *response_token,
	ಸ್ಥಿರಾಂಕ ರಚನೆ strbuf *response_token_id,
	ಸ್ಥಿರಾಂಕ ರಚನೆ fsmonitor_batch *batch)
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsmonitor_parse_client_token(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf_token,
					ರಚನೆ strbuf *requested_token_id,
					uint64_t *seq_nr)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
	ಅಕ್ಷರ *p_ಕೊನೆ;

	strbuf_reset(requested_token_id);
	*seq_nr = 0;

	ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(buf_token, "builtin:", &p))
		ಹಿಂತಿರುಗಿ -1;

	ಆಗ (*p && *p != ':')
		strbuf_addch(requested_token_id, *p++);
	ಆದರೆ (!*p++)
		ಹಿಂತಿರುಗಿ -1;

	*seq_nr = (uint64_t)strtoumax(p, &p_ಕೊನೆ, 10);
	ಆದರೆ (*p_ಕೊನೆ)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_handle_client(ರಚನೆ fsmonitor_daemon_state *state,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *command,
			    ipc_server_ಉತ್ತರ_cb *ಉತ್ತರ,
			    ರಚನೆ ipc_server_ಉತ್ತರ_ದತ್ತಾಂಶ *ಉತ್ತರ_ದತ್ತಾಂಶ)
{
	ರಚನೆ fsmonitor_token_ದತ್ತಾಂಶ *token_ದತ್ತಾಂಶ = ಖಾಲಿ;
	ರಚನೆ strbuf response_token = STRBUF_INIT;
	ರಚನೆ strbuf requested_token_id = STRBUF_INIT;
	ರಚನೆ strbuf payload = STRBUF_INIT;
	uint64_t requested_oldest_seq_nr = 0;
	uint64_t ಒಟ್ಟು_response_len = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
	ಸ್ಥಿರಾಂಕ ರಚನೆ fsmonitor_batch *batch_ತಲೆ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ fsmonitor_batch *batch;
	ರಚನೆ fsmonitor_batch *remainder = ಖಾಲಿ;
	intmax_t ಎಣಿಕೆ_2 = 0, duplicates = 0;
	ರಚನೆ strset shown = STRSET_INIT;
	ಪೂರ್ಣಾಂಕ do_trivial = 0;
	ಪೂರ್ಣಾಂಕ do_flush = 0;
	ಪೂರ್ಣಾಂಕ do_cookie = 0;
	ಎಣಿಕೆ fsmonitor_cookie_ಅಂಶ_ಫಲಿತಾಂಶ cookie_ಫಲಿತಾಂಶ;

	/*
	 * We expect `command` to be of the form:
	 *
	 * <command> := quit NUL
	 *            | flush NUL
	 *            | <V1-time-since-epoch-ns> NUL
	 *            | <V2-opaque-fsmonitor-token> NUL
	 */

	ಆದರೆ (!strcmp(command, "quit")) {
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
		ಹಿಂತಿರುಗಿ SIMPLE_IPC_QUIT;

	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(command, "flush")) {
		/*
		 * Flush all of our cached data and generate a new token
		 * just like if we lost sync with the filesystem.
		 *
		 * Then send a trivial response using the new token.
		 */
		do_flush = 1;
		do_trivial = 1;

	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command, "builtin:", &p)) {
		/* assume V1 timestamp or garbage */

		ಅಕ್ಷರ *p_ಕೊನೆ;

		strtoumax(command, &p_ಕೊನೆ, 10);
		trace_printf_ಕೀಲಿ(&trace_fsmonitor,
				 ((*p_ಕೊನೆ) ?
				  "fsmonitor: invalid command line '%s'" :
				  "fsmonitor: unsupported V1 protocol '%s'"),
				 command);
		do_trivial = 1;
		do_cookie = 1;

	} ಇಲ್ಲದಿದ್ದರೆ {
		/* We have "builtin:*" */
		ಆದರೆ (fsmonitor_parse_client_token(command, &requested_token_id,
						 &requested_oldest_seq_nr)) {
			trace_printf_ಕೀಲಿ(&trace_fsmonitor,
					 "fsmonitor: invalid V2 protocol token '%s'",
					 command);
			do_trivial = 1;
			do_cookie = 1;

		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * We have a V2 valid token:
			 *     "builtin:<token_id>:<seq_nr>"
			 */
			do_cookie = 1;
		}
	}

	pthread_mutex_ಬೀಗ(&state->main_ಬೀಗ);

	ಆದರೆ (!state->current_token_ದತ್ತಾಂಶ)
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
	ಆದರೆ (do_cookie) {
		cookie_ಫಲಿತಾಂಶ = with_ಬೀಗ_wait_for_cookie(state);
		ಆದರೆ (cookie_ಫಲಿತಾಂಶ != FCIR_ನೋಡಿದ) {
			error(_("fsmonitor: cookie_result '%d' != SEEN"),
			      cookie_ಫಲಿತಾಂಶ);
			do_trivial = 1;
		}
	}

	ಆದರೆ (do_flush)
		with_ಬೀಗ_do_force_resync(state);

	/*
	 * We mark the current head of the batch list as "pinned" so
	 * that the listener thread will treat this item as read-only
	 * (and prevent any more paths from being added to it) from
	 * now on.
	 */
	token_ದತ್ತಾಂಶ = state->current_token_ದತ್ತಾಂಶ;
	batch_ತಲೆ = token_ದತ್ತಾಂಶ->batch_ತಲೆ;
	((ರಚನೆ fsmonitor_batch *)batch_ತಲೆ)->pinned_ಸಮಯ = ಸಮಯ(ಖಾಲಿ);

	/*
	 * FSMonitor Protocol V2 requires that we send a response header
	 * with a "new current token" and then all of the paths that changed
	 * since the "requested token".  We send the seq_nr of the just-pinned
	 * head batch so that future requests from a client will be relative
	 * to it.
	 */
	with_ಬೀಗ_format_response_token(&response_token,
					 &token_ದತ್ತಾಂಶ->token_id, batch_ತಲೆ);

	ಉತ್ತರ(ಉತ್ತರ_ದತ್ತಾಂಶ, response_token.buf, response_token.len + 1);
	ಒಟ್ಟು_response_len += response_token.len + 1;

	trace2_ದತ್ತಾಂಶ_string("fsmonitor", the_repository, "response/token",
			   response_token.buf);
	trace_printf_ಕೀಲಿ(&trace_fsmonitor, "response token: %s",
			 response_token.buf);

	ಆದರೆ (!do_trivial) {
		ಆದರೆ (strcmp(requested_token_id.buf, token_ದತ್ತಾಂಶ->token_id.buf)) {
			/*
			 * The client last spoke to a different daemon
			 * instance -OR- the daemon had to resync with
			 * the filesystem (and lost events), so reject.
			 */
			trace2_ದತ್ತಾಂಶ_string("fsmonitor", the_repository,
					   "response/token", "different");
			do_trivial = 1;

		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (requested_oldest_seq_nr <
			   token_ದತ್ತಾಂಶ->batch_ಬಾಲ->batch_seq_nr) {
			/*
			 * The client wants older events than we have for
			 * this token_id.  This means that the end of our
			 * batch list was truncated and we cannot give the
			 * client a complete snapshot relative to their
			 * request.
			 */
			trace_printf_ಕೀಲಿ(&trace_fsmonitor,
					 "client requested truncated data");
			do_trivial = 1;
		}
	}

	ಆದರೆ (do_trivial) {
		pthread_mutex_unlock(&state->main_ಬೀಗ);

		ಉತ್ತರ(ಉತ್ತರ_ದತ್ತಾಂಶ, "/", 2);

		trace2_ದತ್ತಾಂಶ_intmax("fsmonitor", the_repository,
				   "response/trivial", 1);

		ಹೋಗು cleanup;
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
	token_ದತ್ತಾಂಶ->client_ref_ಎಣಿಕೆ++;

	pthread_mutex_unlock(&state->main_ಬೀಗ);

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
	strset_init_with_options(&shown, ಖಾಲಿ, 0);
	ಗಾಗಿ (batch = batch_ತಲೆ;
	     batch && batch->batch_seq_nr > requested_oldest_seq_nr;
	     batch = batch->ಮುಂದಿನ) {
		ಗಾತ್ರ_t k;

		ಗಾಗಿ (k = 0; k < batch->nr; k++) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s = batch->interned_paths[k];
			ಗಾತ್ರ_t s_len;

			ಆದರೆ (!strset_ಕೂಡಿಸು(&shown, s))
				duplicates++;
			ಇಲ್ಲದಿದ್ದರೆ {
				trace_printf_ಕೀಲಿ(&trace_fsmonitor,
						 "send[%"PRIuMAX"]: %s",
						 ಎಣಿಕೆ_2, s);

				/* Each path gets written with a trailing NUL */
				s_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(s) + 1;

				ಆದರೆ (payload.len + s_len >=
				    LARGE_PACKET_ದತ್ತಾಂಶ_MAX) {
					ಉತ್ತರ(ಉತ್ತರ_ದತ್ತಾಂಶ, payload.buf,
					      payload.len);
					ಒಟ್ಟು_response_len += payload.len;
					strbuf_reset(&payload);
				}

				strbuf_ಕೂಡಿಸು(&payload, s, s_len);
				ಎಣಿಕೆ_2++;
			}
		}
	}

	ಆದರೆ (payload.len) {
		ಉತ್ತರ(ಉತ್ತರ_ದತ್ತಾಂಶ, payload.buf, payload.len);
		ಒಟ್ಟು_response_len += payload.len;
	}

	pthread_mutex_ಬೀಗ(&state->main_ಬೀಗ);

	ಆದರೆ (token_ದತ್ತಾಂಶ->client_ref_ಎಣಿಕೆ > 0)
		token_ದತ್ತಾಂಶ->client_ref_ಎಣಿಕೆ--;

	ಆದರೆ (token_ದತ್ತಾಂಶ->client_ref_ಎಣಿಕೆ == 0) {
		ಆದರೆ (token_ದತ್ತಾಂಶ != state->current_token_ದತ್ತಾಂಶ) {
			/*
			 * The listener thread did a token-reset while we were
			 * walking the batch list.  Therefore, this token is
			 * stale and can be discarded completely.  If we are
			 * the last reader thread using this token, we own
			 * that work.
			 */
			fsmonitor_free_token_ದತ್ತಾಂಶ(token_ದತ್ತಾಂಶ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (batch) {
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
			remainder = with_ಬೀಗ_truncate_old_batches(state,
								    batch);
		}
	}

	pthread_mutex_unlock(&state->main_ಬೀಗ);

	ಆದರೆ (remainder)
		fsmonitor_batch__free_list(remainder);

	trace2_ದತ್ತಾಂಶ_intmax("fsmonitor", the_repository, "response/length", ಒಟ್ಟು_response_len);
	trace2_ದತ್ತಾಂಶ_intmax("fsmonitor", the_repository, "response/count/files", ಎಣಿಕೆ_2);
	trace2_ದತ್ತಾಂಶ_intmax("fsmonitor", the_repository, "response/count/duplicates", duplicates);

cleanup:
	strset_clear(&shown);
	strbuf_release(&response_token);
	strbuf_release(&requested_token_id);
	strbuf_release(&payload);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ipc_server_application_cb handle_client;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ handle_client(ಶೂನ್ಯ *ದತ್ತಾಂಶ,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *command, ಗಾತ್ರ_t command_len,
			 ipc_server_ಉತ್ತರ_cb *ಉತ್ತರ,
			 ರಚನೆ ipc_server_ಉತ್ತರ_ದತ್ತಾಂಶ *ಉತ್ತರ_ದತ್ತಾಂಶ)
{
	ರಚನೆ fsmonitor_daemon_state *state = ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;

	/*
	 * The Simple IPC API now supports {char*, len} arguments, but
	 * FSMonitor always uses proper null-terminated strings, so
	 * we can ignore the command_len argument.  (Trust, but verify.)
	 */
	ಆದರೆ (command_len != ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(command))
		BUG("FSMonitor assumes text messages");

	trace_printf_ಕೀಲಿ(&trace_fsmonitor, "requested token: %s", command);

	trace2_region_enter("fsmonitor", "handle_client", the_repository);
	trace2_ದತ್ತಾಂಶ_string("fsmonitor", the_repository, "request", command);

	ಫಲಿತಾಂಶ = do_handle_client(state, command, ಉತ್ತರ, ಉತ್ತರ_ದತ್ತಾಂಶ);

	trace2_region_leave("fsmonitor", "handle_client", the_repository);

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

#ವ್ಯಾಖ್ಯಾನಿಸು FSMONITOR_DIR           "fsmonitor--daemon"
#ವ್ಯಾಖ್ಯಾನಿಸು FSMONITOR_COOKIE_DIR    "cookies"
#ವ್ಯಾಖ್ಯಾನಿಸು FSMONITOR_COOKIE_ಪೂರ್ವಪ್ರತ್ಯಯ (FSMONITOR_DIR "/" FSMONITOR_COOKIE_DIR "/")

ಎಣಿಕೆ fsmonitor_ಮಾರ್ಗ_type fsmonitor_classify_ಮಾರ್ಗ_workdir_relative(
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel)
{
	ಆದರೆ (fspathncmp(rel, ".git", 4))
		ಹಿಂತಿರುಗಿ IS_WORKDIR_ಮಾರ್ಗ;
	rel += 4;

	ಆದರೆ (!*rel)
		ಹಿಂತಿರುಗಿ IS_DOT_GIT;
	ಆದರೆ (*rel != '/')
		ಹಿಂತಿರುಗಿ IS_WORKDIR_ಮಾರ್ಗ; /* e.g. .gitignore */
	rel++;

	ಆದರೆ (!fspathncmp(rel, FSMONITOR_COOKIE_ಪೂರ್ವಪ್ರತ್ಯಯ,
			ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(FSMONITOR_COOKIE_ಪೂರ್ವಪ್ರತ್ಯಯ)))
		ಹಿಂತಿರುಗಿ IS_INSIDE_DOT_GIT_WITH_COOKIE_ಪೂರ್ವಪ್ರತ್ಯಯ;

	ಹಿಂತಿರುಗಿ IS_INSIDE_DOT_GIT;
}

ಎಣಿಕೆ fsmonitor_ಮಾರ್ಗ_type fsmonitor_classify_ಮಾರ್ಗ_gitdir_relative(
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel)
{
	ಆದರೆ (!fspathncmp(rel, FSMONITOR_COOKIE_ಪೂರ್ವಪ್ರತ್ಯಯ,
			ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(FSMONITOR_COOKIE_ಪೂರ್ವಪ್ರತ್ಯಯ)))
		ಹಿಂತಿರುಗಿ IS_INSIDE_GITDIR_WITH_COOKIE_ಪೂರ್ವಪ್ರತ್ಯಯ;

	ಹಿಂತಿರುಗಿ IS_INSIDE_GITDIR;
}

ಸ್ಥಿರ ಎಣಿಕೆ fsmonitor_ಮಾರ್ಗ_type try_classify_workdir_abs_ಮಾರ್ಗ(
	ರಚನೆ fsmonitor_daemon_state *state,
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel;

	ಆದರೆ (fspathncmp(ಮಾರ್ಗ, state->ಮಾರ್ಗ_worktree_watch.buf,
		       state->ಮಾರ್ಗ_worktree_watch.len))
		ಹಿಂತಿರುಗಿ IS_OUTSIDE_CONE;

	rel = ಮಾರ್ಗ + state->ಮಾರ್ಗ_worktree_watch.len;

	ಆದರೆ (!*rel)
		ಹಿಂತಿರುಗಿ IS_WORKDIR_ಮಾರ್ಗ; /* it is the root dir exactly */
	ಆದರೆ (*rel != '/')
		ಹಿಂತಿರುಗಿ IS_OUTSIDE_CONE;
	rel++;

	ಹಿಂತಿರುಗಿ fsmonitor_classify_ಮಾರ್ಗ_workdir_relative(rel);
}

ಎಣಿಕೆ fsmonitor_ಮಾರ್ಗ_type fsmonitor_classify_ಮಾರ್ಗ_absolute(
	ರಚನೆ fsmonitor_daemon_state *state,
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel;
	ಎಣಿಕೆ fsmonitor_ಮಾರ್ಗ_type t;

	t = try_classify_workdir_abs_ಮಾರ್ಗ(state, ಮಾರ್ಗ);
	ಆದರೆ (state->nr_paths_watching == 1)
		ಹಿಂತಿರುಗಿ t;
	ಆದರೆ (t != IS_OUTSIDE_CONE)
		ಹಿಂತಿರುಗಿ t;

	ಆದರೆ (fspathncmp(ಮಾರ್ಗ, state->ಮಾರ್ಗ_gitdir_watch.buf,
		       state->ಮಾರ್ಗ_gitdir_watch.len))
		ಹಿಂತಿರುಗಿ IS_OUTSIDE_CONE;

	rel = ಮಾರ್ಗ + state->ಮಾರ್ಗ_gitdir_watch.len;

	ಆದರೆ (!*rel)
		ಹಿಂತಿರುಗಿ IS_GITDIR; /* it is the <gitdir> exactly */
	ಆದರೆ (*rel != '/')
		ಹಿಂತಿರುಗಿ IS_OUTSIDE_CONE;
	rel++;

	ಹಿಂತಿರುಗಿ fsmonitor_classify_ಮಾರ್ಗ_gitdir_relative(rel);
}

/*
 * We try to combine small batches at the front of the batch-list to avoid
 * having a long list.  This hopefully makes it a little easier when we want
 * to truncate and maintain the list.  However, we don't want the paths array
 * to just keep growing and growing with realloc, so we insert an arbitrary
 * limit.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು MY_COMBINE_ಎಲ್ಲೆ (1024)

ಶೂನ್ಯ fsmonitor_publish(ರಚನೆ fsmonitor_daemon_state *state,
		       ರಚನೆ fsmonitor_batch *batch,
		       ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *cookie_ಹೆಸರುಗಳು)
{
	ಆದರೆ (!batch && !cookie_ಹೆಸರುಗಳು->nr)
		ಹಿಂತಿರುಗಿ;

	pthread_mutex_ಬೀಗ(&state->main_ಬೀಗ);

	ಆದರೆ (batch) {
		ರಚನೆ fsmonitor_batch *ತಲೆ;

		ತಲೆ = state->current_token_ದತ್ತಾಂಶ->batch_ತಲೆ;
		ಆದರೆ (!ತಲೆ) {
			BUG("token does not have batch");
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ತಲೆ->pinned_ಸಮಯ) {
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
			batch->batch_seq_nr = ತಲೆ->batch_seq_nr + 1;
			batch->ಮುಂದಿನ = ತಲೆ;
			state->current_token_ದತ್ತಾಂಶ->batch_ತಲೆ = batch;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ತಲೆ->batch_seq_nr) {
			/*
			 * Batch 0 is unpinned.  See the note in
			 * `fsmonitor_new_token_data()` about why we
			 * don't need to accumulate these paths.
			 */
			fsmonitor_batch__free_list(batch);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ತಲೆ->nr + batch->nr > MY_COMBINE_ಎಲ್ಲೆ) {
			/*
			 * The head batch in the list has never been
			 * transmitted to a client, but folding the
			 * contents of the new batch onto it would
			 * exceed our arbitrary limit, so just prepend
			 * the new batch onto the list.
			 */
			batch->batch_seq_nr = ತಲೆ->batch_seq_nr + 1;
			batch->ಮುಂದಿನ = ತಲೆ;
			state->current_token_ದತ್ತಾಂಶ->batch_ತಲೆ = batch;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * We are free to add the paths in the given
			 * batch onto the end of the current head batch.
			 */
			fsmonitor_batch__combine(ತಲೆ, batch);
			fsmonitor_batch__free_list(batch);
		}
	}

	ಆದರೆ (cookie_ಹೆಸರುಗಳು->nr)
		with_ಬೀಗ_mark_cookies_ನೋಡಿದ(state, cookie_ಹೆಸರುಗಳು);

	pthread_mutex_unlock(&state->main_ಬೀಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ *fsm_health__thread_proc(ಶೂನ್ಯ *_state)
{
	ರಚನೆ fsmonitor_daemon_state *state = _state;

	trace2_thread_ಆರಂಭ("fsm-health");

	fsm_health__loop(state);

	trace2_thread_exit();
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ *fsm_listen__thread_proc(ಶೂನ್ಯ *_state)
{
	ರಚನೆ fsmonitor_daemon_state *state = _state;

	trace2_thread_ಆರಂಭ("fsm-listen");

	trace_printf_ಕೀಲಿ(&trace_fsmonitor, "Watching: worktree '%s'",
			 state->ಮಾರ್ಗ_worktree_watch.buf);
	ಆದರೆ (state->nr_paths_watching > 1)
		trace_printf_ಕೀಲಿ(&trace_fsmonitor, "Watching: gitdir '%s'",
				 state->ಮಾರ್ಗ_gitdir_watch.buf);

	fsm_listen__loop(state);

	pthread_mutex_ಬೀಗ(&state->main_ಬೀಗ);
	ಆದರೆ (state->current_token_ದತ್ತಾಂಶ &&
	    state->current_token_ದತ್ತಾಂಶ->client_ref_ಎಣಿಕೆ == 0)
		fsmonitor_free_token_ದತ್ತಾಂಶ(state->current_token_ದತ್ತಾಂಶ);
	state->current_token_ದತ್ತಾಂಶ = ಖಾಲಿ;
	pthread_mutex_unlock(&state->main_ಬೀಗ);

	trace2_thread_exit();
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsmonitor_ಸತತ_daemon_1(ರಚನೆ fsmonitor_daemon_state *state)
{
	ರಚನೆ ipc_server_opts ipc_opts = {
		.nr_ಎಳೆಗಳು = fsmonitor_ipc_ಎಳೆಗಳು,

		/*
		 * We know that there are no other active threads yet,
		 * so we can let the IPC layer temporarily chdir() if
		 * it needs to when creating the server side of the
		 * Unix domain socket.
		 */
		.uds_disallow_chdir = 0
	};
	ಪೂರ್ಣಾಂಕ health_started = 0;
	ಪೂರ್ಣಾಂಕ listener_started = 0;
	ಪೂರ್ಣಾಂಕ err = 0;

	/*
	 * Start the IPC thread pool before the we've started the file
	 * system event listener thread so that we have the IPC handle
	 * before we need it.
	 */
	ಆದರೆ (ipc_server_init_async(&state->ipc_server_ದತ್ತಾಂಶ,
				  state->ಮಾರ್ಗ_ipc.buf, &ipc_opts,
				  handle_client, state))
		ಹಿಂತಿರುಗಿ error_errno(
			_("could not start IPC thread pool on '%s'"),
			state->ಮಾರ್ಗ_ipc.buf);

	/*
	 * Start the fsmonitor listener thread to collect filesystem
	 * events.
	 */
	ಆದರೆ (pthread_create(&state->listener_thread, ಖಾಲಿ,
			   fsm_listen__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_ದತ್ತಾಂಶ);
		err = error(_("could not start fsmonitor listener thread"));
		ಹೋಗು cleanup;
	}
	listener_started = 1;

	/*
	 * Start the health thread to watch over our process.
	 */
	ಆದರೆ (pthread_create(&state->health_thread, ಖಾಲಿ,
			   fsm_health__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_ದತ್ತಾಂಶ);
		err = error(_("could not start fsmonitor health thread"));
		ಹೋಗು cleanup;
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
	ipc_server_await(state->ipc_server_ದತ್ತಾಂಶ);

	/*
	 * The fsmonitor listener thread may have received a shutdown
	 * event from the IPC thread pool, but it doesn't hurt to tell
	 * it again.  And wait for it to shutdown.
	 */
	ಆದರೆ (listener_started) {
		fsm_listen__stop_async(state);
		pthread_join(state->listener_thread, ಖಾಲಿ);
	}

	ಆದರೆ (health_started) {
		fsm_health__stop_async(state);
		pthread_join(state->health_thread, ಖಾಲಿ);
	}

	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;
	ಆದರೆ (state->listen_error_ಸಂಕೇತ)
		ಹಿಂತಿರುಗಿ state->listen_error_ಸಂಕೇತ;
	ಆದರೆ (state->health_error_ಸಂಕೇತ)
		ಹಿಂತಿರುಗಿ state->health_error_ಸಂಕೇತ;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsmonitor_ಸತತ_daemon(ಶೂನ್ಯ)
{
	ರಚನೆ fsmonitor_daemon_state state;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *home;
	ಪೂರ್ಣಾಂಕ err;

	memset(&state, 0, ಗಾತ್ರ(state));

	hashmap_init(&state.cookies, cookies_cmp, ಖಾಲಿ, 0);
	pthread_mutex_init(&state.main_ಬೀಗ, ಖಾಲಿ);
	pthread_cond_init(&state.cookies_cond, ಖಾಲಿ);
	state.listen_error_ಸಂಕೇತ = 0;
	state.health_error_ಸಂಕೇತ = 0;
	state.current_token_ದತ್ತಾಂಶ = fsmonitor_new_token_ದತ್ತಾಂಶ();

	/* Prepare to (recursively) watch the <worktree-root> directory. */
	strbuf_init(&state.ಮಾರ್ಗ_worktree_watch, 0);
	strbuf_addstr(&state.ಮಾರ್ಗ_worktree_watch,
		      absolute_ಮಾರ್ಗ(repo_get_work_ವೃಕ್ಷ(the_repository)));
	state.nr_paths_watching = 1;

	strbuf_init(&state.alias.alias, 0);
	strbuf_init(&state.alias.points_ಗಮ್ಯ, 0);
	ಆದರೆ ((err = fsmonitor__get_alias(state.ಮಾರ್ಗ_worktree_watch.buf, &state.alias)))
		ಹೋಗು ಮುಗಿದ;

	/*
	 * We create and delete cookie files somewhere inside the .git
	 * directory to help us keep sync with the file system.  If
	 * ".git" is not a directory, then <gitdir> is not inside the
	 * cone of <worktree-root>, so set up a second watch to watch
	 * the <gitdir> so that we get events for the cookie files.
	 */
	strbuf_init(&state.ಮಾರ್ಗ_gitdir_watch, 0);
	strbuf_addbuf(&state.ಮಾರ್ಗ_gitdir_watch, &state.ಮಾರ್ಗ_worktree_watch);
	strbuf_addstr(&state.ಮಾರ್ಗ_gitdir_watch, "/.git");
	ಆದರೆ (!is_directory(state.ಮಾರ್ಗ_gitdir_watch.buf)) {
		strbuf_reset(&state.ಮಾರ್ಗ_gitdir_watch);
		strbuf_addstr(&state.ಮಾರ್ಗ_gitdir_watch,
			      absolute_ಮಾರ್ಗ(repo_get_git_dir(the_repository)));
		strbuf_strip_suffix(&state.ಮಾರ್ಗ_gitdir_watch, "/.");
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
	strbuf_init(&state.ಮಾರ್ಗ_cookie_ಪೂರ್ವಪ್ರತ್ಯಯ, 0);
	strbuf_addbuf(&state.ಮಾರ್ಗ_cookie_ಪೂರ್ವಪ್ರತ್ಯಯ, &state.ಮಾರ್ಗ_gitdir_watch);

	strbuf_addch(&state.ಮಾರ್ಗ_cookie_ಪೂರ್ವಪ್ರತ್ಯಯ, '/');
	strbuf_addstr(&state.ಮಾರ್ಗ_cookie_ಪೂರ್ವಪ್ರತ್ಯಯ, FSMONITOR_DIR);
	mkdir(state.ಮಾರ್ಗ_cookie_ಪೂರ್ವಪ್ರತ್ಯಯ.buf, 0777);

	strbuf_addch(&state.ಮಾರ್ಗ_cookie_ಪೂರ್ವಪ್ರತ್ಯಯ, '/');
	strbuf_addstr(&state.ಮಾರ್ಗ_cookie_ಪೂರ್ವಪ್ರತ್ಯಯ, FSMONITOR_COOKIE_DIR);
	mkdir(state.ಮಾರ್ಗ_cookie_ಪೂರ್ವಪ್ರತ್ಯಯ.buf, 0777);

	strbuf_addch(&state.ಮಾರ್ಗ_cookie_ಪೂರ್ವಪ್ರತ್ಯಯ, '/');

	/*
	 * We create a named-pipe or unix domain socket inside of the
	 * ".git" directory.  (Well, on Windows, we base our named
	 * pipe in the NPFS on the absolute path of the git
	 * directory.)
	 */
	strbuf_init(&state.ಮಾರ್ಗ_ipc, 0);
	strbuf_addstr(&state.ಮಾರ್ಗ_ipc,
		absolute_ಮಾರ್ಗ(fsmonitor_ipc_get_ಮಾರ್ಗ(the_repository)));

	/*
	 * Confirm that we can create platform-specific resources for the
	 * filesystem listener before we bother starting all the threads.
	 */
	ಆದರೆ (fsm_listen__ctor(&state)) {
		err = error(_("could not initialize listener thread"));
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (fsm_health__ctor(&state)) {
		err = error(_("could not initialize health thread"));
		ಹೋಗು ಮುಗಿದ;
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
	ಆದರೆ (home && *home && chdir(home))
		die_errno(_("could not cd home '%s'"), home);

	err = fsmonitor_ಸತತ_daemon_1(&state);

ಮುಗಿದ:
	fsmonitor_free_token_ದತ್ತಾಂಶ(state.current_token_ದತ್ತಾಂಶ);
	state.current_token_ದತ್ತಾಂಶ = ಖಾಲಿ;
	pthread_cond_destroy(&state.cookies_cond);
	pthread_mutex_destroy(&state.main_ಬೀಗ);
	{
		ರಚನೆ hashmap_iter iter;
		ರಚನೆ fsmonitor_cookie_ಅಂಶ *cookie;

		hashmap_for_each_ನಮೂದು(&state.cookies, &iter, cookie, ನಮೂದು)
			ಬಿಡುಗಡೆ(cookie->name);
		hashmap_clear_and_free(&state.cookies,
				       ರಚನೆ fsmonitor_cookie_ಅಂಶ, ನಮೂದು);
	}
	fsm_listen__dtor(&state);
	fsm_health__dtor(&state);

	ipc_server_free(state.ipc_server_ದತ್ತಾಂಶ);

	strbuf_release(&state.ಮಾರ್ಗ_worktree_watch);
	strbuf_release(&state.ಮಾರ್ಗ_gitdir_watch);
	strbuf_release(&state.ಮಾರ್ಗ_cookie_ಪೂರ್ವಪ್ರತ್ಯಯ);
	strbuf_release(&state.ಮಾರ್ಗ_ipc);
	strbuf_release(&state.alias.alias);
	strbuf_release(&state.alias.points_ಗಮ್ಯ);

	ಹಿಂತಿರುಗಿ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ try_ಗಮ್ಯ_ಸತತ_foreground_daemon(ಪೂರ್ಣಾಂಕ detach_console)
{
	/*
	 * Technically, we don't need to probe for an existing daemon
	 * process, since we could just call `fsmonitor_run_daemon()`
	 * and let it fail if the pipe/socket is busy.
	 *
	 * However, this method gives us a nicer error message for a
	 * common error case.
	 */
	ಆದರೆ (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	ಆದರೆ (fsmonitor__announce_startup) {
		fprintf(stderr, _("running fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	ಆದರೆ (detach_console) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GIT_WINDOWS_NATIVE
		FreeConsole();
#ಇಲ್ಲದಿದ್ದರೆ
		/*
		 * Create a new session so that the daemon is detached
		 * from the parent's process group.  This prevents
		 * shells with job control (e.g. bash with "set -m")
		 * from waiting on the daemon when they wait for a
		 * foreground command that implicitly spawned it.
		 */
		ಆದರೆ (setsid() == -1)
			warning_errno(_("setsid failed"));
#ಆದರೆ_ಅಂತ್ಯ
	}

	ಹಿಂತಿರುಗಿ !!fsmonitor_ಸತತ_daemon();
}

ಸ್ಥಿರ ಆರಂಭ_bg_wait_cb bg_wait_cb;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bg_wait_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ ಮಗು_process *cp ಬಳಸದ,
		      ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಎಣಿಕೆ ipc_ಸಕ್ರಿಯ_state s = fsmonitor_ipc__get_state();

	ಆಯ್ಕೆ (s) {
	ಸ್ಥಿತಿ IPC_STATE__LISTENING:
		/* child is "ready" */
		ಹಿಂತಿರುಗಿ 0;

	ಸ್ಥಿತಿ IPC_STATE__NOT_LISTENING:
	ಸ್ಥಿತಿ IPC_STATE_ಮಾರ್ಗ_NOT_ಸಿಕ್ಕಿತು:
		/* give child more time */
		ಹಿಂತಿರುಗಿ 1;

	ಪೂರ್ವನಿಯೋಜಿತ:
	ಸ್ಥಿತಿ IPC_STATE_INVALID_ಮಾರ್ಗ:
	ಸ್ಥಿತಿ IPC_STATE__OTHER_ERROR:
		/* all the time in world won't help */
		ಹಿಂತಿರುಗಿ -1;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ try_ಗಮ್ಯ_ಆರಂಭ_background_daemon(ಶೂನ್ಯ)
{
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
	ಎಣಿಕೆ ಆರಂಭ_bg_ಫಲಿತಾಂಶ sbgr;

	/*
	 * Before we try to create a background daemon process, see
	 * if a daemon process is already listening.  This makes it
	 * easier for us to report an already-listening error to the
	 * console, since our spawn/daemon can only report the success
	 * of creating the background process (and not whether it
	 * immediately exited).
	 */
	ಆದರೆ (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	ಆದರೆ (fsmonitor__announce_startup) {
		fprintf(stderr, _("starting fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	cp.git_cmd = 1;

	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "fsmonitor--daemon");
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "run");
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--detach");
	strvec_pushf(&cp.ವಾದಗಳು, "--ipc-threads=%d", fsmonitor_ipc_ಎಳೆಗಳು);

	cp.no_stdin = 1;
	cp.no_stdout = 1;
	cp.no_stderr = 1;
	cp.close_fd_above_stderr = 1;

	sbgr = ಆರಂಭ_bg_command(&cp, bg_wait_cb, ಖಾಲಿ,
				fsmonitor_ಆರಂಭ_timeout_sec);

	ಆಯ್ಕೆ (sbgr) {
	ಸ್ಥಿತಿ SBGR_READY:
		ಹಿಂತಿರುಗಿ 0;

	ಪೂರ್ವನಿಯೋಜಿತ:
	ಸ್ಥಿತಿ SBGR_ERROR:
	ಸ್ಥಿತಿ SBGR_CB_ERROR:
		ಹಿಂತಿರುಗಿ error(_("daemon failed to start"));

	ಸ್ಥಿತಿ SBGR_TIMEOUT:
		ಹಿಂತಿರುಗಿ error(_("daemon not online yet"));

	ಸ್ಥಿತಿ SBGR_DIED:
		ಹಿಂತಿರುಗಿ error(_("daemon terminated"));
	}
}

ಪೂರ್ಣಾಂಕ cmd_fsmonitor__daemon(ಪೂರ್ಣಾಂಕ argc,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			  ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subcmd;
	ಎಣಿಕೆ fsmonitor_reason reason;
	ಪೂರ್ಣಾಂಕ detach_console = 0;

	ರಚನೆ option options[] = {
		OPT_BOOL(0, "detach", &detach_console, N_("detach from console")),
		OPT_INTEGER(0, "ipc-threads",
			    &fsmonitor_ipc_ಎಳೆಗಳು,
			    N_("use <n> ipc worker threads")),
		OPT_INTEGER(0, "start-timeout",
			    &fsmonitor_ಆರಂಭ_timeout_sec,
			    N_("max seconds to wait for background daemon startup")),

		OPT_ಕೊನೆ()
	};

	repo_config(the_repository, fsmonitor_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_fsmonitor__daemon_usage, 0);
	ಆದರೆ (argc != 1)
		usage_with_options(builtin_fsmonitor__daemon_usage, options);
	subcmd = argv[0];

	ಆದರೆ (fsmonitor_ipc_ಎಳೆಗಳು < 1)
		die(_("invalid 'ipc-threads' value (%d)"),
		    fsmonitor_ipc_ಎಳೆಗಳು);

	prepare_repo_settings(the_repository);
	/*
	 * If the repo is fsmonitor-compatible, explicitly set IPC-mode
	 * (without bothering to load the `core.fsmonitor` config settings).
	 *
	 * If the repo is not compatible, the repo-settings will be set to
	 * incompatible rather than IPC, so we can use one of the __get
	 * routines to detect the discrepancy.
	 */
	fsm_settings_ಗಣ_ipc(the_repository);

	reason = fsm_settings__get_reason(the_repository);
	ಆದರೆ (reason > FSMONITOR_REASON_OK)
		die("%s",
		    fsm_settings_get_incompatible_ಸಂದೇಶ(the_repository,
						       reason));

	ಆದರೆ (!strcmp(subcmd, "start"))
		ಹಿಂತಿರುಗಿ !!try_ಗಮ್ಯ_ಆರಂಭ_background_daemon();

	ಆದರೆ (!strcmp(subcmd, "run"))
		ಹಿಂತಿರುಗಿ !!try_ಗಮ್ಯ_ಸತತ_foreground_daemon(detach_console);

	ಆದರೆ (!strcmp(subcmd, "stop"))
		ಹಿಂತಿರುಗಿ !!do_as_client__send_stop();

	ಆದರೆ (!strcmp(subcmd, "status"))
		ಹಿಂತಿರುಗಿ !!do_as_client_ಸ್ಥಿತಿ();

	die(_("Unhandled subcommand '%s'"), subcmd);
}

#ಇಲ್ಲದಿದ್ದರೆ
ಪೂರ್ಣಾಂಕ cmd_fsmonitor__daemon(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ, ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_fsmonitor__daemon_usage, options);

	die(_("fsmonitor--daemon not supported on this platform"));
}
#ಆದರೆ_ಅಂತ್ಯ
