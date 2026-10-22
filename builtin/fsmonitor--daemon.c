#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "config.h"
#ضمن "dir.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "fsmonitor-ll.h"
#ضمن "fsmonitor-ipc.h"
#ضمن "fsmonitor-settings.h"
#ضمن "compat/fsmonitor/fsm-health.h"
#ضمن "compat/fsmonitor/fsm-listen.h"
#ضمن "fsmonitor--daemon.h"

#ضمن "simple-ipc.h"
#ضمن "strmap.h"
#ضمن "run-command.h"
#ضمن "trace.h"
#ضمن "trace2.h"

ساكن ثابت حرف * ثابت builtin_fsmonitor__daemon_usage[] = {
	N_("git fsmonitor--daemon start [<options>]"),
	N_("git fsmonitor--daemon run [<options>]"),
	"git fsmonitor--daemon stop",
	"git fsmonitor--daemon status",
	عدم
};

#که_تعریف_شوی HAVE_FSMONITOR_DAEMON_BACKEND
/*
 * Global state loaded from config.
 */
#تعریف_کړه FSMONITOR_IPC_تارونه "fsmonitor.ipcthreads"
ساكن صحيح fsmonitor_ipc_تارونه = 8;

#تعریف_کړه FSMONITOR_پیل_TIMEOUT "fsmonitor.starttimeout"
ساكن صحيح fsmonitor_پیل_timeout_sec = 60;

#تعریف_کړه FSMONITOR__ANNOUNCE_STARTUP "fsmonitor.announcestartup"
ساكن صحيح fsmonitor__announce_startup = 0;

ساكن صحيح fsmonitor_config(ثابت حرف *var, ثابت حرف *ارزښت,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, FSMONITOR_IPC_تارونه)) {
		صحيح i = git_config_int(var, ارزښت, ctx->kvi);
		إذا (i < 1)
			أرجع error(_("value of '%s' out of range: %d"),
				     FSMONITOR_IPC_تارونه, i);
		fsmonitor_ipc_تارونه = i;
		أرجع 0;
	}

	إذا (!strcmp(var, FSMONITOR_پیل_TIMEOUT)) {
		صحيح i = git_config_int(var, ارزښت, ctx->kvi);
		إذا (i < 0)
			أرجع error(_("value of '%s' out of range: %d"),
				     FSMONITOR_پیل_TIMEOUT, i);
		fsmonitor_پیل_timeout_sec = i;
		أرجع 0;
	}

	إذا (!strcmp(var, FSMONITOR__ANNOUNCE_STARTUP)) {
		صحيح is_bool;
		صحيح i = git_config_bool_or_int(var, ارزښت, ctx->kvi, &is_bool);
		إذا (i < 0)
			أرجع error(_("value of '%s' not bool or int: %d"),
				     var, i);
		fsmonitor__announce_startup = i;
		أرجع 0;
	}

	أرجع git_default_config(var, ارزښت, ctx, cb);
}

/*
 * Acting as a CLIENT.
 *
 * Send a "quit" command to the `git-fsmonitor--daemon` (if running)
 * and wait for it to shutdown.
 */
ساكن صحيح do_as_client__send_stop(فراغ)
{
	بنية strbuf answer = STRBUF_INIT;
	صحيح ret;
	صحيح max_wait_ms = 30000;
	صحيح elapsed_ms = 0;

	ret = fsmonitor_ipc__send_command("quit", &answer);

	/* The quit command does not return any response data. */
	strbuf_release(&answer);

	إذا (ret)
		أرجع ret;

	trace2_region_enter("fsm_client", "polling-for-daemon-exit", عدم);
	طالما (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING) {
		إذا (elapsed_ms >= max_wait_ms) {
			trace2_region_leave("fsm_client",
					    "polling-for-daemon-exit", عدم);
			أرجع error(_("daemon did not stop within %d seconds"),
				     max_wait_ms / 1000);
		}
		sleep_millisec(50);
		elapsed_ms += 50;
	}
	trace2_region_leave("fsm_client", "polling-for-daemon-exit", عدم);

	أرجع 0;
}

ساكن صحيح do_as_client_حالت(فراغ)
{
	تعداد ipc_فعال_state state = fsmonitor_ipc__get_state();

	بدل (state) {
	حالة IPC_STATE__LISTENING:
		چاپ_کړه(_("fsmonitor-daemon is watching '%s'\n"),
		       the_repository->worktree);
		أرجع 0;

	افتراضي:
		چاپ_کړه(_("fsmonitor-daemon is not watching '%s'\n"),
		       the_repository->worktree);
		أرجع 1;
	}
}

تعداد fsmonitor_cookie_توکی_پایله {
	FCIR_ERROR = -1, /* could not create cookie file ? */
	FCIR_INIT,
	FCIR_کتل_شوی,
	FCIR_ABORT,
};

بنية fsmonitor_cookie_توکی {
	بنية hashmap_ثبت ثبت;
	حرف *name;
	تعداد fsmonitor_cookie_توکی_پایله پایله;
};

ساكن صحيح cookies_cmp(ثابت فراغ *معلومات ناکارول_شوی,
		       ثابت بنية hashmap_ثبت *he1,
		       ثابت بنية hashmap_ثبت *he2, ثابت فراغ *keydata)
{
	ثابت بنية fsmonitor_cookie_توکی *a =
		container_د(he1, ثابت بنية fsmonitor_cookie_توکی, ثبت);
	ثابت بنية fsmonitor_cookie_توکی *b =
		container_د(he2, ثابت بنية fsmonitor_cookie_توکی, ثبت);

	أرجع strcmp(a->name, keydata ? keydata : b->name);
}

ساكن تعداد fsmonitor_cookie_توکی_پایله with_قلف_wait_for_cookie(
	بنية fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	صحيح fd;
	بنية fsmonitor_cookie_توکی *cookie;
	بنية strbuf cookie_pathname = STRBUF_INIT;
	بنية strbuf cookie_filename = STRBUF_INIT;
	تعداد fsmonitor_cookie_توکی_پایله پایله;
	صحيح my_cookie_seq;

	CALLOC_ARRAY(cookie, 1);

	my_cookie_seq = state->cookie_seq++;

	strbuf_addf(&cookie_filename, "%i-%i", getpid(), my_cookie_seq);

	strbuf_addbuf(&cookie_pathname, &state->لار_cookie_مختاړی);
	strbuf_addbuf(&cookie_pathname, &cookie_filename);

	cookie->name = strbuf_detach(&cookie_filename, عدم);
	cookie->پایله = FCIR_INIT;
	hashmap_ثبت_init(&cookie->ثبت, strhash(cookie->name));

	hashmap_جمع_کړه(&state->cookies, &cookie->ثبت);

	trace_printf_کلی(&trace_fsmonitor, "cookie-wait: '%s' '%s'",
			 cookie->name, cookie_pathname.buf);

	/*
	 * Create the cookie file on disk and then wait for a notification
	 * that the listener thread has seen it.
	 */
	fd = open(cookie_pathname.buf, O_WRONLY | O_CREAT | O_EXCL, 0600);
	إذا (fd < 0) {
		error_errno(_("could not create fsmonitor cookie '%s'"),
			    cookie->name);

		cookie->پایله = FCIR_ERROR;
		اذهب بشپړ;
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
		بنية timeval now;
		بنية timespec ts;
		صحيح err = 0;

		gettimeofday(&now, عدم);
		ts.tv_sec = now.tv_sec + 1;
		ts.tv_nsec = now.tv_usec * 1000;

		طالما (cookie->پایله == FCIR_INIT && !err)
			err = pthread_cond_timedwait(&state->cookies_cond,
						     &state->main_قلف,
						     &ts);
		إذا (err == ETIMEDOUT && cookie->پایله == FCIR_INIT) {
			trace_printf_کلی(&trace_fsmonitor,
					 "cookie_wait timed out");
			cookie->پایله = FCIR_ERROR;
		}
	}

بشپړ:
	hashmap_remove(&state->cookies, &cookie->ثبت, عدم);

	پایله = cookie->پایله;

	حرر(cookie->name);
	حرر(cookie);
	strbuf_release(&cookie_pathname);

	أرجع پایله;
}

/*
 * Mark these cookies as _SEEN and wake up the corresponding client threads.
 */
ساكن فراغ with_قلف_mark_cookies_کتل_شوی(بنية fsmonitor_daemon_state *state,
					 ثابت بنية string_list *cookie_نومونه)
{
	/* assert current thread holding state->main_lock */

	صحيح k;
	صحيح nr_کتل_شوی = 0;

	لكل (k = 0; k < cookie_نومونه->nr; k++) {
		بنية fsmonitor_cookie_توکی کلی;
		بنية fsmonitor_cookie_توکی *cookie;

		کلی.name = cookie_نومونه->توکي[k].string;
		hashmap_ثبت_init(&کلی.ثبت, strhash(کلی.name));

		cookie = hashmap_get_ثبت(&state->cookies, &کلی, ثبت, عدم);
		إذا (cookie) {
			trace_printf_کلی(&trace_fsmonitor, "cookie-seen: '%s'",
					 cookie->name);
			cookie->پایله = FCIR_کتل_شوی;
			nr_کتل_شوی++;
		}
	}

	إذا (nr_کتل_شوی)
		pthread_cond_خپرونه(&state->cookies_cond);
}

/*
 * Set _ABORT on all pending cookies and wake up all client threads.
 */
ساكن فراغ with_قلف_abort_all_cookies(بنية fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	بنية hashmap_iter iter;
	بنية fsmonitor_cookie_توکی *cookie;
	صحيح nr_aborted = 0;

	hashmap_for_each_ثبت(&state->cookies, &iter, cookie, ثبت) {
		trace_printf_کلی(&trace_fsmonitor, "cookie-abort: '%s'",
				 cookie->name);
		cookie->پایله = FCIR_ABORT;
		nr_aborted++;
	}

	إذا (nr_aborted)
		pthread_cond_خپرونه(&state->cookies_cond);
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
بنية fsmonitor_token_معلومات {
	بنية strbuf token_id;
	بنية fsmonitor_batch *batch_سر;
	بنية fsmonitor_batch *batch_لکۍ;
	uint64_t client_ref_شمېر;
};

بنية fsmonitor_batch {
	بنية fsmonitor_batch *بل;
	uint64_t batch_seq_nr;
	ثابت حرف **interned_paths;
	اندازه_t nr, alloc;
	وخت_t pinned_وخت;
};

ساكن بنية fsmonitor_token_معلومات *fsmonitor_new_token_معلومات(فراغ)
{
	ساكن صحيح test_env_ارزښت = -1;
	ساكن uint64_t flush_شمېر = 0;
	بنية fsmonitor_token_معلومات *token;
	بنية fsmonitor_batch *batch;

	CALLOC_ARRAY(token, 1);
	batch = fsmonitor_batch__new();

	strbuf_init(&token->token_id, 0);
	token->batch_سر = batch;
	token->batch_لکۍ = batch;
	token->client_ref_شمېر = 0;

	إذا (test_env_ارزښت < 0)
		test_env_ارزښت = git_env_bool("GIT_TEST_FSMONITOR_TOKEN", 0);

	إذا (!test_env_ارزښت) {
		بنية timeval tv;
		بنية tm tm;
		وخت_t secs;

		gettimeofday(&tv, عدم);
		secs = tv.tv_sec;
		gmtime_r(&secs, &tm);

		strbuf_addf(&token->token_id,
			    "%"PRIu64".%d.%4d%02d%02dT%02d%02d%02d.%06ldZ",
			    flush_شمېر++,
			    getpid(),
			    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			    tm.tm_hour, tm.tm_min, tm.tm_sec,
			    (اوږد)tv.tv_usec);
	} وإلا {
		strbuf_addf(&token->token_id, "test_%08x", test_env_ارزښت++);
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
	إذا (test_env_ارزښت)
		batch->pinned_وخت = وخت(عدم);

	أرجع token;
}

بنية fsmonitor_batch *fsmonitor_batch__new(فراغ)
{
	بنية fsmonitor_batch *batch;

	CALLOC_ARRAY(batch, 1);

	أرجع batch;
}

فراغ fsmonitor_batch__free_list(بنية fsmonitor_batch *batch)
{
	طالما (batch) {
		بنية fsmonitor_batch *بل = batch->بل;

		/*
		 * The actual strings within the array of this batch
		 * are interned, so we don't own them.  We only own
		 * the array.
		 */
		حرر(batch->interned_paths);
		حرر(batch);

		batch = بل;
	}
}

فراغ fsmonitor_batch_جمع_کړه_لار(بنية fsmonitor_batch *batch,
			       ثابت حرف *لار)
{
	ثابت حرف *interned_لار = strintern(لار);

	trace_printf_کلی(&trace_fsmonitor, "event: %s", interned_لار);

	ALLOC_GROW(batch->interned_paths, batch->nr + 1, batch->alloc);
	batch->interned_paths[batch->nr++] = interned_لار;
}

ساكن فراغ fsmonitor_batch__combine(بنية fsmonitor_batch *batch_dest,
				     ثابت بنية fsmonitor_batch *batch_src)
{
	اندازه_t k;

	ALLOC_GROW(batch_dest->interned_paths,
		   batch_dest->nr + batch_src->nr + 1,
		   batch_dest->alloc);

	لكل (k = 0; k < batch_src->nr; k++)
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
#تعریف_کړه MY_وخت_DELAY_SECONDS (5 * 60) /* seconds */

ساكن بنية fsmonitor_batch *with_قلف_truncate_old_batches(
	بنية fsmonitor_daemon_state *state,
	ثابت بنية fsmonitor_batch *batch_marker)
{
	/* assert current thread holding state->main_lock */

	ثابت بنية fsmonitor_batch *batch;
	بنية fsmonitor_batch *remainder;

	إذا (!batch_marker)
		أرجع عدم;

	trace_printf_کلی(&trace_fsmonitor, "Truncate: mark (%"PRIu64",%"PRIu64")",
			 batch_marker->batch_seq_nr,
			 (uint64_t)batch_marker->pinned_وخت);

	لكل (batch = batch_marker; batch; batch = batch->بل) {
		وخت_t t;

		إذا (!batch->pinned_وخت) /* an overflow batch */
			استمر;

		t = batch->pinned_وخت + MY_وخت_DELAY_SECONDS;
		إذا (t > batch_marker->pinned_وخت) /* too close to marker */
			استمر;

		اذهب truncate_past_دلته;
	}

	أرجع عدم;

truncate_past_دلته:
	state->current_token_معلومات->batch_لکۍ = (بنية fsmonitor_batch *)batch;

	remainder = ((بنية fsmonitor_batch *)batch)->بل;
	((بنية fsmonitor_batch *)batch)->بل = عدم;

	أرجع remainder;
}

ساكن فراغ fsmonitor_free_token_معلومات(بنية fsmonitor_token_معلومات *token)
{
	إذا (!token)
		أرجع;

	أكد(token->client_ref_شمېر == 0);

	strbuf_release(&token->token_id);

	fsmonitor_batch__free_list(token->batch_سر);

	حرر(token);
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
ساكن فراغ with_قلف_do_force_resync(بنية fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	بنية fsmonitor_token_معلومات *free_me = عدم;
	بنية fsmonitor_token_معلومات *new_one = عدم;

	new_one = fsmonitor_new_token_معلومات();

	إذا (state->current_token_معلومات->client_ref_شمېر == 0)
		free_me = state->current_token_معلومات;
	state->current_token_معلومات = new_one;

	fsmonitor_free_token_معلومات(free_me);

	with_قلف_abort_all_cookies(state);
}

فراغ fsmonitor_force_resync(بنية fsmonitor_daemon_state *state)
{
	pthread_mutex_قلف(&state->main_قلف);
	with_قلف_do_force_resync(state);
	pthread_mutex_unlock(&state->main_قلف);
}

/*
 * Format an opaque token string to send to the client.
 */
ساكن فراغ with_قلف_format_response_token(
	بنية strbuf *response_token,
	ثابت بنية strbuf *response_token_id,
	ثابت بنية fsmonitor_batch *batch)
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
ساكن صحيح fsmonitor_parse_client_token(ثابت حرف *buf_token,
					بنية strbuf *requested_token_id,
					uint64_t *seq_nr)
{
	ثابت حرف *p;
	حرف *p_پای_ټکی;

	strbuf_reset(requested_token_id);
	*seq_nr = 0;

	إذا (!skip_مختاړی(buf_token, "builtin:", &p))
		أرجع -1;

	طالما (*p && *p != ':')
		strbuf_addch(requested_token_id, *p++);
	إذا (!*p++)
		أرجع -1;

	*seq_nr = (uint64_t)strtoumax(p, &p_پای_ټکی, 10);
	إذا (*p_پای_ټکی)
		أرجع -1;

	أرجع 0;
}

ساكن صحيح do_handle_client(بنية fsmonitor_daemon_state *state,
			    ثابت حرف *command,
			    ipc_server_ځواب_cb *ځواب,
			    بنية ipc_server_ځواب_معلومات *ځواب_معلومات)
{
	بنية fsmonitor_token_معلومات *token_معلومات = عدم;
	بنية strbuf response_token = STRBUF_INIT;
	بنية strbuf requested_token_id = STRBUF_INIT;
	بنية strbuf payload = STRBUF_INIT;
	uint64_t requested_oldest_seq_nr = 0;
	uint64_t ټول_response_len = 0;
	ثابت حرف *p;
	ثابت بنية fsmonitor_batch *batch_سر;
	ثابت بنية fsmonitor_batch *batch;
	بنية fsmonitor_batch *remainder = عدم;
	intmax_t شمېر = 0, duplicates = 0;
	بنية strset shown = STRSET_INIT;
	صحيح do_trivial = 0;
	صحيح do_flush = 0;
	صحيح do_cookie = 0;
	تعداد fsmonitor_cookie_توکی_پایله cookie_پایله;

	/*
	 * We expect `command` to be of the form:
	 *
	 * <command> := quit NUL
	 *            | flush NUL
	 *            | <V1-time-since-epoch-ns> NUL
	 *            | <V2-opaque-fsmonitor-token> NUL
	 */

	إذا (!strcmp(command, "quit")) {
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
		أرجع SIMPLE_IPC_QUIT;

	} وإلا إذا (!strcmp(command, "flush")) {
		/*
		 * Flush all of our cached data and generate a new token
		 * just like if we lost sync with the filesystem.
		 *
		 * Then send a trivial response using the new token.
		 */
		do_flush = 1;
		do_trivial = 1;

	} وإلا إذا (!skip_مختاړی(command, "builtin:", &p)) {
		/* assume V1 timestamp or garbage */

		حرف *p_پای_ټکی;

		strtoumax(command, &p_پای_ټکی, 10);
		trace_printf_کلی(&trace_fsmonitor,
				 ((*p_پای_ټکی) ?
				  "fsmonitor: invalid command line '%s'" :
				  "fsmonitor: unsupported V1 protocol '%s'"),
				 command);
		do_trivial = 1;
		do_cookie = 1;

	} وإلا {
		/* We have "builtin:*" */
		إذا (fsmonitor_parse_client_token(command, &requested_token_id,
						 &requested_oldest_seq_nr)) {
			trace_printf_کلی(&trace_fsmonitor,
					 "fsmonitor: invalid V2 protocol token '%s'",
					 command);
			do_trivial = 1;
			do_cookie = 1;

		} وإلا {
			/*
			 * We have a V2 valid token:
			 *     "builtin:<token_id>:<seq_nr>"
			 */
			do_cookie = 1;
		}
	}

	pthread_mutex_قلف(&state->main_قلف);

	إذا (!state->current_token_معلومات)
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
	إذا (do_cookie) {
		cookie_پایله = with_قلف_wait_for_cookie(state);
		إذا (cookie_پایله != FCIR_کتل_شوی) {
			error(_("fsmonitor: cookie_result '%d' != SEEN"),
			      cookie_پایله);
			do_trivial = 1;
		}
	}

	إذا (do_flush)
		with_قلف_do_force_resync(state);

	/*
	 * We mark the current head of the batch list as "pinned" so
	 * that the listener thread will treat this item as read-only
	 * (and prevent any more paths from being added to it) from
	 * now on.
	 */
	token_معلومات = state->current_token_معلومات;
	batch_سر = token_معلومات->batch_سر;
	((بنية fsmonitor_batch *)batch_سر)->pinned_وخت = وخت(عدم);

	/*
	 * FSMonitor Protocol V2 requires that we send a response header
	 * with a "new current token" and then all of the paths that changed
	 * since the "requested token".  We send the seq_nr of the just-pinned
	 * head batch so that future requests from a client will be relative
	 * to it.
	 */
	with_قلف_format_response_token(&response_token,
					 &token_معلومات->token_id, batch_سر);

	ځواب(ځواب_معلومات, response_token.buf, response_token.len + 1);
	ټول_response_len += response_token.len + 1;

	trace2_معلومات_string("fsmonitor", the_repository, "response/token",
			   response_token.buf);
	trace_printf_کلی(&trace_fsmonitor, "response token: %s",
			 response_token.buf);

	إذا (!do_trivial) {
		إذا (strcmp(requested_token_id.buf, token_معلومات->token_id.buf)) {
			/*
			 * The client last spoke to a different daemon
			 * instance -OR- the daemon had to resync with
			 * the filesystem (and lost events), so reject.
			 */
			trace2_معلومات_string("fsmonitor", the_repository,
					   "response/token", "different");
			do_trivial = 1;

		} وإلا إذا (requested_oldest_seq_nr <
			   token_معلومات->batch_لکۍ->batch_seq_nr) {
			/*
			 * The client wants older events than we have for
			 * this token_id.  This means that the end of our
			 * batch list was truncated and we cannot give the
			 * client a complete snapshot relative to their
			 * request.
			 */
			trace_printf_کلی(&trace_fsmonitor,
					 "client requested truncated data");
			do_trivial = 1;
		}
	}

	إذا (do_trivial) {
		pthread_mutex_unlock(&state->main_قلف);

		ځواب(ځواب_معلومات, "/", 2);

		trace2_معلومات_intmax("fsmonitor", the_repository,
				   "response/trivial", 1);

		اذهب cleanup;
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
	token_معلومات->client_ref_شمېر++;

	pthread_mutex_unlock(&state->main_قلف);

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
	strset_init_with_options(&shown, عدم, 0);
	لكل (batch = batch_سر;
	     batch && batch->batch_seq_nr > requested_oldest_seq_nr;
	     batch = batch->بل) {
		اندازه_t k;

		لكل (k = 0; k < batch->nr; k++) {
			ثابت حرف *s = batch->interned_paths[k];
			اندازه_t s_len;

			إذا (!strset_جمع_کړه(&shown, s))
				duplicates++;
			وإلا {
				trace_printf_کلی(&trace_fsmonitor,
						 "send[%"PRIuMAX"]: %s",
						 شمېر, s);

				/* Each path gets written with a trailing NUL */
				s_len = مزي_اوږدوالی(s) + 1;

				إذا (payload.len + s_len >=
				    LARGE_PACKET_معلومات_MAX) {
					ځواب(ځواب_معلومات, payload.buf,
					      payload.len);
					ټول_response_len += payload.len;
					strbuf_reset(&payload);
				}

				strbuf_جمع_کړه(&payload, s, s_len);
				شمېر++;
			}
		}
	}

	إذا (payload.len) {
		ځواب(ځواب_معلومات, payload.buf, payload.len);
		ټول_response_len += payload.len;
	}

	pthread_mutex_قلف(&state->main_قلف);

	إذا (token_معلومات->client_ref_شمېر > 0)
		token_معلومات->client_ref_شمېر--;

	إذا (token_معلومات->client_ref_شمېر == 0) {
		إذا (token_معلومات != state->current_token_معلومات) {
			/*
			 * The listener thread did a token-reset while we were
			 * walking the batch list.  Therefore, this token is
			 * stale and can be discarded completely.  If we are
			 * the last reader thread using this token, we own
			 * that work.
			 */
			fsmonitor_free_token_معلومات(token_معلومات);
		} وإلا إذا (batch) {
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
			remainder = with_قلف_truncate_old_batches(state,
								    batch);
		}
	}

	pthread_mutex_unlock(&state->main_قلف);

	إذا (remainder)
		fsmonitor_batch__free_list(remainder);

	trace2_معلومات_intmax("fsmonitor", the_repository, "response/length", ټول_response_len);
	trace2_معلومات_intmax("fsmonitor", the_repository, "response/count/files", شمېر);
	trace2_معلومات_intmax("fsmonitor", the_repository, "response/count/duplicates", duplicates);

cleanup:
	strset_clear(&shown);
	strbuf_release(&response_token);
	strbuf_release(&requested_token_id);
	strbuf_release(&payload);

	أرجع 0;
}

ساكن ipc_server_application_cb handle_client;

ساكن صحيح handle_client(فراغ *معلومات,
			 ثابت حرف *command, اندازه_t command_len,
			 ipc_server_ځواب_cb *ځواب,
			 بنية ipc_server_ځواب_معلومات *ځواب_معلومات)
{
	بنية fsmonitor_daemon_state *state = معلومات;
	صحيح پایله;

	/*
	 * The Simple IPC API now supports {char*, len} arguments, but
	 * FSMonitor always uses proper null-terminated strings, so
	 * we can ignore the command_len argument.  (Trust, but verify.)
	 */
	إذا (command_len != مزي_اوږدوالی(command))
		BUG("FSMonitor assumes text messages");

	trace_printf_کلی(&trace_fsmonitor, "requested token: %s", command);

	trace2_region_enter("fsmonitor", "handle_client", the_repository);
	trace2_معلومات_string("fsmonitor", the_repository, "request", command);

	پایله = do_handle_client(state, command, ځواب, ځواب_معلومات);

	trace2_region_leave("fsmonitor", "handle_client", the_repository);

	أرجع پایله;
}

#تعریف_کړه FSMONITOR_DIR           "fsmonitor--daemon"
#تعریف_کړه FSMONITOR_COOKIE_DIR    "cookies"
#تعریف_کړه FSMONITOR_COOKIE_مختاړی (FSMONITOR_DIR "/" FSMONITOR_COOKIE_DIR "/")

تعداد fsmonitor_لار_type fsmonitor_classify_لار_workdir_relative(
	ثابت حرف *rel)
{
	إذا (fspathncmp(rel, ".git", 4))
		أرجع IS_WORKDIR_لار;
	rel += 4;

	إذا (!*rel)
		أرجع IS_DOT_GIT;
	إذا (*rel != '/')
		أرجع IS_WORKDIR_لار; /* e.g. .gitignore */
	rel++;

	إذا (!fspathncmp(rel, FSMONITOR_COOKIE_مختاړی,
			مزي_اوږدوالی(FSMONITOR_COOKIE_مختاړی)))
		أرجع IS_INSIDE_DOT_GIT_WITH_COOKIE_مختاړی;

	أرجع IS_INSIDE_DOT_GIT;
}

تعداد fsmonitor_لار_type fsmonitor_classify_لار_gitdir_relative(
	ثابت حرف *rel)
{
	إذا (!fspathncmp(rel, FSMONITOR_COOKIE_مختاړی,
			مزي_اوږدوالی(FSMONITOR_COOKIE_مختاړی)))
		أرجع IS_INSIDE_GITDIR_WITH_COOKIE_مختاړی;

	أرجع IS_INSIDE_GITDIR;
}

ساكن تعداد fsmonitor_لار_type try_classify_workdir_abs_لار(
	بنية fsmonitor_daemon_state *state,
	ثابت حرف *لار)
{
	ثابت حرف *rel;

	إذا (fspathncmp(لار, state->لار_worktree_watch.buf,
		       state->لار_worktree_watch.len))
		أرجع IS_OUTSIDE_CONE;

	rel = لار + state->لار_worktree_watch.len;

	إذا (!*rel)
		أرجع IS_WORKDIR_لار; /* it is the root dir exactly */
	إذا (*rel != '/')
		أرجع IS_OUTSIDE_CONE;
	rel++;

	أرجع fsmonitor_classify_لار_workdir_relative(rel);
}

تعداد fsmonitor_لار_type fsmonitor_classify_لار_absolute(
	بنية fsmonitor_daemon_state *state,
	ثابت حرف *لار)
{
	ثابت حرف *rel;
	تعداد fsmonitor_لار_type t;

	t = try_classify_workdir_abs_لار(state, لار);
	إذا (state->nr_paths_watching == 1)
		أرجع t;
	إذا (t != IS_OUTSIDE_CONE)
		أرجع t;

	إذا (fspathncmp(لار, state->لار_gitdir_watch.buf,
		       state->لار_gitdir_watch.len))
		أرجع IS_OUTSIDE_CONE;

	rel = لار + state->لار_gitdir_watch.len;

	إذا (!*rel)
		أرجع IS_GITDIR; /* it is the <gitdir> exactly */
	إذا (*rel != '/')
		أرجع IS_OUTSIDE_CONE;
	rel++;

	أرجع fsmonitor_classify_لار_gitdir_relative(rel);
}

/*
 * We try to combine small batches at the front of the batch-list to avoid
 * having a long list.  This hopefully makes it a little easier when we want
 * to truncate and maintain the list.  However, we don't want the paths array
 * to just keep growing and growing with realloc, so we insert an arbitrary
 * limit.
 */
#تعریف_کړه MY_COMBINE_حد (1024)

فراغ fsmonitor_publish(بنية fsmonitor_daemon_state *state,
		       بنية fsmonitor_batch *batch,
		       ثابت بنية string_list *cookie_نومونه)
{
	إذا (!batch && !cookie_نومونه->nr)
		أرجع;

	pthread_mutex_قلف(&state->main_قلف);

	إذا (batch) {
		بنية fsmonitor_batch *سر;

		سر = state->current_token_معلومات->batch_سر;
		إذا (!سر) {
			BUG("token does not have batch");
		} وإلا إذا (سر->pinned_وخت) {
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
			batch->batch_seq_nr = سر->batch_seq_nr + 1;
			batch->بل = سر;
			state->current_token_معلومات->batch_سر = batch;
		} وإلا إذا (!سر->batch_seq_nr) {
			/*
			 * Batch 0 is unpinned.  See the note in
			 * `fsmonitor_new_token_data()` about why we
			 * don't need to accumulate these paths.
			 */
			fsmonitor_batch__free_list(batch);
		} وإلا إذا (سر->nr + batch->nr > MY_COMBINE_حد) {
			/*
			 * The head batch in the list has never been
			 * transmitted to a client, but folding the
			 * contents of the new batch onto it would
			 * exceed our arbitrary limit, so just prepend
			 * the new batch onto the list.
			 */
			batch->batch_seq_nr = سر->batch_seq_nr + 1;
			batch->بل = سر;
			state->current_token_معلومات->batch_سر = batch;
		} وإلا {
			/*
			 * We are free to add the paths in the given
			 * batch onto the end of the current head batch.
			 */
			fsmonitor_batch__combine(سر, batch);
			fsmonitor_batch__free_list(batch);
		}
	}

	إذا (cookie_نومونه->nr)
		with_قلف_mark_cookies_کتل_شوی(state, cookie_نومونه);

	pthread_mutex_unlock(&state->main_قلف);
}

ساكن فراغ *fsm_health__thread_proc(فراغ *_state)
{
	بنية fsmonitor_daemon_state *state = _state;

	trace2_thread_پیل("fsm-health");

	fsm_health__loop(state);

	trace2_thread_exit();
	أرجع عدم;
}

ساكن فراغ *fsm_listen__thread_proc(فراغ *_state)
{
	بنية fsmonitor_daemon_state *state = _state;

	trace2_thread_پیل("fsm-listen");

	trace_printf_کلی(&trace_fsmonitor, "Watching: worktree '%s'",
			 state->لار_worktree_watch.buf);
	إذا (state->nr_paths_watching > 1)
		trace_printf_کلی(&trace_fsmonitor, "Watching: gitdir '%s'",
				 state->لار_gitdir_watch.buf);

	fsm_listen__loop(state);

	pthread_mutex_قلف(&state->main_قلف);
	إذا (state->current_token_معلومات &&
	    state->current_token_معلومات->client_ref_شمېر == 0)
		fsmonitor_free_token_معلومات(state->current_token_معلومات);
	state->current_token_معلومات = عدم;
	pthread_mutex_unlock(&state->main_قلف);

	trace2_thread_exit();
	أرجع عدم;
}

ساكن صحيح fsmonitor_پرلپسې_daemon_1(بنية fsmonitor_daemon_state *state)
{
	بنية ipc_server_opts ipc_opts = {
		.nr_تارونه = fsmonitor_ipc_تارونه,

		/*
		 * We know that there are no other active threads yet,
		 * so we can let the IPC layer temporarily chdir() if
		 * it needs to when creating the server side of the
		 * Unix domain socket.
		 */
		.uds_disallow_chdir = 0
	};
	صحيح health_started = 0;
	صحيح listener_started = 0;
	صحيح err = 0;

	/*
	 * Start the IPC thread pool before the we've started the file
	 * system event listener thread so that we have the IPC handle
	 * before we need it.
	 */
	إذا (ipc_server_init_async(&state->ipc_server_معلومات,
				  state->لار_ipc.buf, &ipc_opts,
				  handle_client, state))
		أرجع error_errno(
			_("could not start IPC thread pool on '%s'"),
			state->لار_ipc.buf);

	/*
	 * Start the fsmonitor listener thread to collect filesystem
	 * events.
	 */
	إذا (pthread_create(&state->listener_thread, عدم,
			   fsm_listen__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_معلومات);
		err = error(_("could not start fsmonitor listener thread"));
		اذهب cleanup;
	}
	listener_started = 1;

	/*
	 * Start the health thread to watch over our process.
	 */
	إذا (pthread_create(&state->health_thread, عدم,
			   fsm_health__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_معلومات);
		err = error(_("could not start fsmonitor health thread"));
		اذهب cleanup;
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
	ipc_server_await(state->ipc_server_معلومات);

	/*
	 * The fsmonitor listener thread may have received a shutdown
	 * event from the IPC thread pool, but it doesn't hurt to tell
	 * it again.  And wait for it to shutdown.
	 */
	إذا (listener_started) {
		fsm_listen__stop_async(state);
		pthread_join(state->listener_thread, عدم);
	}

	إذا (health_started) {
		fsm_health__stop_async(state);
		pthread_join(state->health_thread, عدم);
	}

	إذا (err)
		أرجع err;
	إذا (state->listen_error_کوډ)
		أرجع state->listen_error_کوډ;
	إذا (state->health_error_کوډ)
		أرجع state->health_error_کوډ;
	أرجع 0;
}

ساكن صحيح fsmonitor_پرلپسې_daemon(فراغ)
{
	بنية fsmonitor_daemon_state state;
	ثابت حرف *home;
	صحيح err;

	memset(&state, 0, حجم(state));

	hashmap_init(&state.cookies, cookies_cmp, عدم, 0);
	pthread_mutex_init(&state.main_قلف, عدم);
	pthread_cond_init(&state.cookies_cond, عدم);
	state.listen_error_کوډ = 0;
	state.health_error_کوډ = 0;
	state.current_token_معلومات = fsmonitor_new_token_معلومات();

	/* Prepare to (recursively) watch the <worktree-root> directory. */
	strbuf_init(&state.لار_worktree_watch, 0);
	strbuf_addstr(&state.لار_worktree_watch,
		      absolute_لار(repo_get_work_ونه(the_repository)));
	state.nr_paths_watching = 1;

	strbuf_init(&state.alias.alias, 0);
	strbuf_init(&state.alias.points_منزل, 0);
	إذا ((err = fsmonitor__get_alias(state.لار_worktree_watch.buf, &state.alias)))
		اذهب بشپړ;

	/*
	 * We create and delete cookie files somewhere inside the .git
	 * directory to help us keep sync with the file system.  If
	 * ".git" is not a directory, then <gitdir> is not inside the
	 * cone of <worktree-root>, so set up a second watch to watch
	 * the <gitdir> so that we get events for the cookie files.
	 */
	strbuf_init(&state.لار_gitdir_watch, 0);
	strbuf_addbuf(&state.لار_gitdir_watch, &state.لار_worktree_watch);
	strbuf_addstr(&state.لار_gitdir_watch, "/.git");
	إذا (!is_directory(state.لار_gitdir_watch.buf)) {
		strbuf_reset(&state.لار_gitdir_watch);
		strbuf_addstr(&state.لار_gitdir_watch,
			      absolute_لار(repo_get_git_dir(the_repository)));
		strbuf_strip_suffix(&state.لار_gitdir_watch, "/.");
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
	strbuf_init(&state.لار_cookie_مختاړی, 0);
	strbuf_addbuf(&state.لار_cookie_مختاړی, &state.لار_gitdir_watch);

	strbuf_addch(&state.لار_cookie_مختاړی, '/');
	strbuf_addstr(&state.لار_cookie_مختاړی, FSMONITOR_DIR);
	mkdir(state.لار_cookie_مختاړی.buf, 0777);

	strbuf_addch(&state.لار_cookie_مختاړی, '/');
	strbuf_addstr(&state.لار_cookie_مختاړی, FSMONITOR_COOKIE_DIR);
	mkdir(state.لار_cookie_مختاړی.buf, 0777);

	strbuf_addch(&state.لار_cookie_مختاړی, '/');

	/*
	 * We create a named-pipe or unix domain socket inside of the
	 * ".git" directory.  (Well, on Windows, we base our named
	 * pipe in the NPFS on the absolute path of the git
	 * directory.)
	 */
	strbuf_init(&state.لار_ipc, 0);
	strbuf_addstr(&state.لار_ipc,
		absolute_لار(fsmonitor_ipc_get_لار(the_repository)));

	/*
	 * Confirm that we can create platform-specific resources for the
	 * filesystem listener before we bother starting all the threads.
	 */
	إذا (fsm_listen__ctor(&state)) {
		err = error(_("could not initialize listener thread"));
		اذهب بشپړ;
	}

	إذا (fsm_health__ctor(&state)) {
		err = error(_("could not initialize health thread"));
		اذهب بشپړ;
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
	إذا (home && *home && chdir(home))
		die_errno(_("could not cd home '%s'"), home);

	err = fsmonitor_پرلپسې_daemon_1(&state);

بشپړ:
	fsmonitor_free_token_معلومات(state.current_token_معلومات);
	state.current_token_معلومات = عدم;
	pthread_cond_destroy(&state.cookies_cond);
	pthread_mutex_destroy(&state.main_قلف);
	{
		بنية hashmap_iter iter;
		بنية fsmonitor_cookie_توکی *cookie;

		hashmap_for_each_ثبت(&state.cookies, &iter, cookie, ثبت)
			حرر(cookie->name);
		hashmap_clear_and_free(&state.cookies,
				       بنية fsmonitor_cookie_توکی, ثبت);
	}
	fsm_listen__dtor(&state);
	fsm_health__dtor(&state);

	ipc_server_free(state.ipc_server_معلومات);

	strbuf_release(&state.لار_worktree_watch);
	strbuf_release(&state.لار_gitdir_watch);
	strbuf_release(&state.لار_cookie_مختاړی);
	strbuf_release(&state.لار_ipc);
	strbuf_release(&state.alias.alias);
	strbuf_release(&state.alias.points_منزل);

	أرجع err;
}

ساكن صحيح try_منزل_پرلپسې_foreground_daemon(صحيح detach_console)
{
	/*
	 * Technically, we don't need to probe for an existing daemon
	 * process, since we could just call `fsmonitor_run_daemon()`
	 * and let it fail if the pipe/socket is busy.
	 *
	 * However, this method gives us a nicer error message for a
	 * common error case.
	 */
	إذا (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	إذا (fsmonitor__announce_startup) {
		fprintf(stderr, _("running fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	إذا (detach_console) {
#که_تعریف_شوی GIT_WINDOWS_NATIVE
		FreeConsole();
#وإلا
		/*
		 * Create a new session so that the daemon is detached
		 * from the parent's process group.  This prevents
		 * shells with job control (e.g. bash with "set -m")
		 * from waiting on the daemon when they wait for a
		 * foreground command that implicitly spawned it.
		 */
		إذا (setsid() == -1)
			warning_errno(_("setsid failed"));
#د_که_پای
	}

	أرجع !!fsmonitor_پرلپسې_daemon();
}

ساكن پیل_bg_wait_cb bg_wait_cb;

ساكن صحيح bg_wait_cb(ثابت بنية ماشوم_process *cp ناکارول_شوی,
		      فراغ *cb_معلومات ناکارول_شوی)
{
	تعداد ipc_فعال_state s = fsmonitor_ipc__get_state();

	بدل (s) {
	حالة IPC_STATE__LISTENING:
		/* child is "ready" */
		أرجع 0;

	حالة IPC_STATE__NOT_LISTENING:
	حالة IPC_STATE_لار_NOT_وموندل:
		/* give child more time */
		أرجع 1;

	افتراضي:
	حالة IPC_STATE_INVALID_لار:
	حالة IPC_STATE__OTHER_ERROR:
		/* all the time in world won't help */
		أرجع -1;
	}
}

ساكن صحيح try_منزل_پیل_background_daemon(فراغ)
{
	بنية ماشوم_process cp = ماشوم_PROCESS_INIT;
	تعداد پیل_bg_پایله sbgr;

	/*
	 * Before we try to create a background daemon process, see
	 * if a daemon process is already listening.  This makes it
	 * easier for us to report an already-listening error to the
	 * console, since our spawn/daemon can only report the success
	 * of creating the background process (and not whether it
	 * immediately exited).
	 */
	إذا (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	إذا (fsmonitor__announce_startup) {
		fprintf(stderr, _("starting fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	cp.git_cmd = 1;

	strvec_ټېل(&cp.دلیلونه, "fsmonitor--daemon");
	strvec_ټېل(&cp.دلیلونه, "run");
	strvec_ټېل(&cp.دلیلونه, "--detach");
	strvec_pushf(&cp.دلیلونه, "--ipc-threads=%d", fsmonitor_ipc_تارونه);

	cp.no_stdin = 1;
	cp.no_stdout = 1;
	cp.no_stderr = 1;
	cp.close_fd_above_stderr = 1;

	sbgr = پیل_bg_command(&cp, bg_wait_cb, عدم,
				fsmonitor_پیل_timeout_sec);

	بدل (sbgr) {
	حالة SBGR_READY:
		أرجع 0;

	افتراضي:
	حالة SBGR_ERROR:
	حالة SBGR_CB_ERROR:
		أرجع error(_("daemon failed to start"));

	حالة SBGR_TIMEOUT:
		أرجع error(_("daemon not online yet"));

	حالة SBGR_DIED:
		أرجع error(_("daemon terminated"));
	}
}

صحيح cmd_fsmonitor__daemon(صحيح argc,
			  ثابت حرف **argv,
			  ثابت حرف *مختاړی,
			  بنية repository *repo ناکارول_شوی)
{
	ثابت حرف *subcmd;
	تعداد fsmonitor_reason reason;
	صحيح detach_console = 0;

	بنية option options[] = {
		OPT_BOOL(0, "detach", &detach_console, N_("detach from console")),
		OPT_INTEGER(0, "ipc-threads",
			    &fsmonitor_ipc_تارونه,
			    N_("use <n> ipc worker threads")),
		OPT_INTEGER(0, "start-timeout",
			    &fsmonitor_پیل_timeout_sec,
			    N_("max seconds to wait for background daemon startup")),

		OPT_پای_ټکی()
	};

	repo_config(the_repository, fsmonitor_config, عدم);

	argc = parse_options(argc, argv, مختاړی, options,
			     builtin_fsmonitor__daemon_usage, 0);
	إذا (argc != 1)
		usage_with_options(builtin_fsmonitor__daemon_usage, options);
	subcmd = argv[0];

	إذا (fsmonitor_ipc_تارونه < 1)
		die(_("invalid 'ipc-threads' value (%d)"),
		    fsmonitor_ipc_تارونه);

	prepare_repo_settings(the_repository);
	/*
	 * If the repo is fsmonitor-compatible, explicitly set IPC-mode
	 * (without bothering to load the `core.fsmonitor` config settings).
	 *
	 * If the repo is not compatible, the repo-settings will be set to
	 * incompatible rather than IPC, so we can use one of the __get
	 * routines to detect the discrepancy.
	 */
	fsm_settings_سېټ_ipc(the_repository);

	reason = fsm_settings__get_reason(the_repository);
	إذا (reason > FSMONITOR_REASON_OK)
		die("%s",
		    fsm_settings_get_incompatible_پیغام(the_repository,
						       reason));

	إذا (!strcmp(subcmd, "start"))
		أرجع !!try_منزل_پیل_background_daemon();

	إذا (!strcmp(subcmd, "run"))
		أرجع !!try_منزل_پرلپسې_foreground_daemon(detach_console);

	إذا (!strcmp(subcmd, "stop"))
		أرجع !!do_as_client__send_stop();

	إذا (!strcmp(subcmd, "status"))
		أرجع !!do_as_client_حالت();

	die(_("Unhandled subcommand '%s'"), subcmd);
}

#وإلا
صحيح cmd_fsmonitor__daemon(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی ناکارول_شوی, بنية repository *repo ناکارول_شوی)
{
	بنية option options[] = {
		OPT_پای_ټکی()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_fsmonitor__daemon_usage, options);

	die(_("fsmonitor--daemon not supported on this platform"));
}
#د_که_پای
