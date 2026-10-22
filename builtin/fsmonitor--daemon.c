#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "fsmonitor-ll.h"
#অন্তর্ভাব "fsmonitor-ipc.h"
#অন্তর্ভাব "fsmonitor-settings.h"
#অন্তর্ভাব "compat/fsmonitor/fsm-health.h"
#অন্তর্ভাব "compat/fsmonitor/fsm-listen.h"
#অন্তর্ভাব "fsmonitor--daemon.h"

#অন্তর্ভাব "simple-ipc.h"
#অন্তর্ভাব "strmap.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "trace.h"
#অন্তর্ভাব "trace2.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_fsmonitor__daemon_usage[] = {
	N_("git fsmonitor--daemon start [<options>]"),
	N_("git fsmonitor--daemon run [<options>]"),
	"git fsmonitor--daemon stop",
	"git fsmonitor--daemon status",
	ফাঁকা
};

#ifdef HAVE_FSMONITOR_DAEMON_BACKEND
/*
 * Global state loaded from config.
 */
#define FSMONITOR__IPC_THREADS "fsmonitor.ipcthreads"
স্থির পূর্ণসংখ্যা fsmonitor__ipc_threads = 8;

#define FSMONITOR__START_TIMEOUT "fsmonitor.starttimeout"
স্থির পূর্ণসংখ্যা fsmonitor__start_timeout_sec = 60;

#define FSMONITOR__ANNOUNCE_STARTUP "fsmonitor.announcestartup"
স্থির পূর্ণসংখ্যা fsmonitor__announce_startup = 0;

স্থির পূর্ণসংখ্যা fsmonitor_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			    ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, FSMONITOR__IPC_THREADS)) {
		পূর্ণসংখ্যা i = git_config_int(var, মান, ctx->kvi);
		যদি (i < 1)
			ফেরত error(_("value of '%s' out of range: %d"),
				     FSMONITOR__IPC_THREADS, i);
		fsmonitor__ipc_threads = i;
		ফেরত 0;
	}

	যদি (!strcmp(var, FSMONITOR__START_TIMEOUT)) {
		পূর্ণসংখ্যা i = git_config_int(var, মান, ctx->kvi);
		যদি (i < 0)
			ফেরত error(_("value of '%s' out of range: %d"),
				     FSMONITOR__START_TIMEOUT, i);
		fsmonitor__start_timeout_sec = i;
		ফেরত 0;
	}

	যদি (!strcmp(var, FSMONITOR__ANNOUNCE_STARTUP)) {
		পূর্ণসংখ্যা is_bool;
		পূর্ণসংখ্যা i = git_config_bool_or_int(var, মান, ctx->kvi, &is_bool);
		যদি (i < 0)
			ফেরত error(_("value of '%s' not bool or int: %d"),
				     var, i);
		fsmonitor__announce_startup = i;
		ফেরত 0;
	}

	ফেরত git_default_config(var, মান, ctx, cb);
}

/*
 * Acting as a CLIENT.
 *
 * Send a "quit" command to the `git-fsmonitor--daemon` (if running)
 * and wait for it to shutdown.
 */
স্থির পূর্ণসংখ্যা do_as_client__send_stop(শূন্য)
{
	কাঠামো strbuf answer = STRBUF_INIT;
	পূর্ণসংখ্যা ret;
	পূর্ণসংখ্যা max_wait_ms = 30000;
	পূর্ণসংখ্যা elapsed_ms = 0;

	ret = fsmonitor_ipc__send_command("quit", &answer);

	/* The quit command does not return any response data. */
	strbuf_release(&answer);

	যদি (ret)
		ফেরত ret;

	trace2_region_enter("fsm_client", "polling-for-daemon-exit", ফাঁকা);
	যখন (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING) {
		যদি (elapsed_ms >= max_wait_ms) {
			trace2_region_leave("fsm_client",
					    "polling-for-daemon-exit", ফাঁকা);
			ফেরত error(_("daemon did not stop within %d seconds"),
				     max_wait_ms / 1000);
		}
		sleep_millisec(50);
		elapsed_ms += 50;
	}
	trace2_region_leave("fsm_client", "polling-for-daemon-exit", ফাঁকা);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা do_as_client__status(শূন্য)
{
	গণনা ipc_active_state state = fsmonitor_ipc__get_state();

	নির্বাচন (state) {
	ক্ষেত্র IPC_STATE__LISTENING:
		printf(_("fsmonitor-daemon is watching '%s'\n"),
		       the_repository->worktree);
		ফেরত 0;

	পূর্বনির্ধারিত:
		printf(_("fsmonitor-daemon is not watching '%s'\n"),
		       the_repository->worktree);
		ফেরত 1;
	}
}

গণনা fsmonitor_cookie_item_ফলাফল {
	FCIR_ERROR = -1, /* could not create cookie file ? */
	FCIR_INIT,
	FCIR_SEEN,
	FCIR_ABORT,
};

কাঠামো fsmonitor_cookie_item {
	কাঠামো hashmap_entry entry;
	অক্ষর *name;
	গণনা fsmonitor_cookie_item_ফলাফল ফলাফল;
};

স্থির পূর্ণসংখ্যা cookies_cmp(ধ্রুবক শূন্য *তথ্য UNUSED,
		       ধ্রুবক কাঠামো hashmap_entry *he1,
		       ধ্রুবক কাঠামো hashmap_entry *he2, ধ্রুবক শূন্য *keydata)
{
	ধ্রুবক কাঠামো fsmonitor_cookie_item *a =
		container_of(he1, ধ্রুবক কাঠামো fsmonitor_cookie_item, entry);
	ধ্রুবক কাঠামো fsmonitor_cookie_item *b =
		container_of(he2, ধ্রুবক কাঠামো fsmonitor_cookie_item, entry);

	ফেরত strcmp(a->name, keydata ? keydata : b->name);
}

স্থির গণনা fsmonitor_cookie_item_ফলাফল with_lock__wait_for_cookie(
	কাঠামো fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	পূর্ণসংখ্যা fd;
	কাঠামো fsmonitor_cookie_item *cookie;
	কাঠামো strbuf cookie_pathname = STRBUF_INIT;
	কাঠামো strbuf cookie_filename = STRBUF_INIT;
	গণনা fsmonitor_cookie_item_ফলাফল ফলাফল;
	পূর্ণসংখ্যা my_cookie_seq;

	CALLOC_ARRAY(cookie, 1);

	my_cookie_seq = state->cookie_seq++;

	strbuf_addf(&cookie_filename, "%i-%i", getpid(), my_cookie_seq);

	strbuf_addbuf(&cookie_pathname, &state->path_cookie_prefix);
	strbuf_addbuf(&cookie_pathname, &cookie_filename);

	cookie->name = strbuf_detach(&cookie_filename, ফাঁকা);
	cookie->ফলাফল = FCIR_INIT;
	hashmap_entry_init(&cookie->entry, strhash(cookie->name));

	hashmap_add(&state->cookies, &cookie->entry);

	trace_printf_চাবি(&trace_fsmonitor, "cookie-wait: '%s' '%s'",
			 cookie->name, cookie_pathname.buf);

	/*
	 * Create the cookie file on disk and then wait for a notification
	 * that the listener thread has seen it.
	 */
	fd = open(cookie_pathname.buf, O_WRONLY | O_CREAT | O_EXCL, 0600);
	যদি (fd < 0) {
		error_errno(_("could not create fsmonitor cookie '%s'"),
			    cookie->name);

		cookie->ফলাফল = FCIR_ERROR;
		যাও done;
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
		কাঠামো timeval now;
		কাঠামো timespec ts;
		পূর্ণসংখ্যা err = 0;

		gettimeofday(&now, ফাঁকা);
		ts.tv_sec = now.tv_sec + 1;
		ts.tv_nsec = now.tv_usec * 1000;

		যখন (cookie->ফলাফল == FCIR_INIT && !err)
			err = pthread_cond_timedwait(&state->cookies_cond,
						     &state->main_lock,
						     &ts);
		যদি (err == ETIMEDOUT && cookie->ফলাফল == FCIR_INIT) {
			trace_printf_চাবি(&trace_fsmonitor,
					 "cookie_wait timed out");
			cookie->ফলাফল = FCIR_ERROR;
		}
	}

done:
	hashmap_remove(&state->cookies, &cookie->entry, ফাঁকা);

	ফলাফল = cookie->ফলাফল;

	মুক্ত(cookie->name);
	মুক্ত(cookie);
	strbuf_release(&cookie_pathname);

	ফেরত ফলাফল;
}

/*
 * Mark these cookies as _SEEN and wake up the corresponding client threads.
 */
স্থির শূন্য with_lock__mark_cookies_seen(কাঠামো fsmonitor_daemon_state *state,
					 ধ্রুবক কাঠামো string_list *cookie_names)
{
	/* assert current thread holding state->main_lock */

	পূর্ণসংখ্যা k;
	পূর্ণসংখ্যা nr_seen = 0;

	জন্য (k = 0; k < cookie_names->nr; k++) {
		কাঠামো fsmonitor_cookie_item চাবি;
		কাঠামো fsmonitor_cookie_item *cookie;

		চাবি.name = cookie_names->বস্তুসমূহ[k].string;
		hashmap_entry_init(&চাবি.entry, strhash(চাবি.name));

		cookie = hashmap_get_entry(&state->cookies, &চাবি, entry, ফাঁকা);
		যদি (cookie) {
			trace_printf_চাবি(&trace_fsmonitor, "cookie-seen: '%s'",
					 cookie->name);
			cookie->ফলাফল = FCIR_SEEN;
			nr_seen++;
		}
	}

	যদি (nr_seen)
		pthread_cond_broadcast(&state->cookies_cond);
}

/*
 * Set _ABORT on all pending cookies and wake up all client threads.
 */
স্থির শূন্য with_lock__abort_all_cookies(কাঠামো fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	কাঠামো hashmap_iter iter;
	কাঠামো fsmonitor_cookie_item *cookie;
	পূর্ণসংখ্যা nr_aborted = 0;

	hashmap_for_each_entry(&state->cookies, &iter, cookie, entry) {
		trace_printf_চাবি(&trace_fsmonitor, "cookie-abort: '%s'",
				 cookie->name);
		cookie->ফলাফল = FCIR_ABORT;
		nr_aborted++;
	}

	যদি (nr_aborted)
		pthread_cond_broadcast(&state->cookies_cond);
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
কাঠামো fsmonitor_token_তথ্য {
	কাঠামো strbuf token_id;
	কাঠামো fsmonitor_batch *batch_শীর্ষ;
	কাঠামো fsmonitor_batch *batch_লেজ;
	uint64_t client_ref_গুনতি;
};

কাঠামো fsmonitor_batch {
	কাঠামো fsmonitor_batch *পরবর্তী;
	uint64_t batch_seq_nr;
	ধ্রুবক অক্ষর **interned_paths;
	আকার_t nr, alloc;
	time_t pinned_time;
};

স্থির কাঠামো fsmonitor_token_তথ্য *fsmonitor_new_token_তথ্য(শূন্য)
{
	স্থির পূর্ণসংখ্যা test_env_মান = -1;
	স্থির uint64_t flush_গুনতি = 0;
	কাঠামো fsmonitor_token_তথ্য *token;
	কাঠামো fsmonitor_batch *batch;

	CALLOC_ARRAY(token, 1);
	batch = fsmonitor_batch__new();

	strbuf_init(&token->token_id, 0);
	token->batch_শীর্ষ = batch;
	token->batch_লেজ = batch;
	token->client_ref_গুনতি = 0;

	যদি (test_env_মান < 0)
		test_env_মান = git_env_bool("GIT_TEST_FSMONITOR_TOKEN", 0);

	যদি (!test_env_মান) {
		কাঠামো timeval tv;
		কাঠামো tm tm;
		time_t secs;

		gettimeofday(&tv, ফাঁকা);
		secs = tv.tv_sec;
		gmtime_r(&secs, &tm);

		strbuf_addf(&token->token_id,
			    "%"PRIu64".%d.%4d%02d%02dT%02d%02d%02d.%06ldZ",
			    flush_গুনতি++,
			    getpid(),
			    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			    tm.tm_hour, tm.tm_min, tm.tm_sec,
			    (long)tv.tv_usec);
	} অন্যথা {
		strbuf_addf(&token->token_id, "test_%08x", test_env_মান++);
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
	যদি (test_env_মান)
		batch->pinned_time = time(ফাঁকা);

	ফেরত token;
}

কাঠামো fsmonitor_batch *fsmonitor_batch__new(শূন্য)
{
	কাঠামো fsmonitor_batch *batch;

	CALLOC_ARRAY(batch, 1);

	ফেরত batch;
}

শূন্য fsmonitor_batch__free_list(কাঠামো fsmonitor_batch *batch)
{
	যখন (batch) {
		কাঠামো fsmonitor_batch *পরবর্তী = batch->পরবর্তী;

		/*
		 * The actual strings within the array of this batch
		 * are interned, so we don't own them.  We only own
		 * the array.
		 */
		মুক্ত(batch->interned_paths);
		মুক্ত(batch);

		batch = পরবর্তী;
	}
}

শূন্য fsmonitor_batch__add_path(কাঠামো fsmonitor_batch *batch,
			       ধ্রুবক অক্ষর *path)
{
	ধ্রুবক অক্ষর *interned_path = strintern(path);

	trace_printf_চাবি(&trace_fsmonitor, "event: %s", interned_path);

	ALLOC_GROW(batch->interned_paths, batch->nr + 1, batch->alloc);
	batch->interned_paths[batch->nr++] = interned_path;
}

স্থির শূন্য fsmonitor_batch__combine(কাঠামো fsmonitor_batch *batch_dest,
				     ধ্রুবক কাঠামো fsmonitor_batch *batch_src)
{
	আকার_t k;

	ALLOC_GROW(batch_dest->interned_paths,
		   batch_dest->nr + batch_src->nr + 1,
		   batch_dest->alloc);

	জন্য (k = 0; k < batch_src->nr; k++)
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
#define MY_TIME_DELAY_SECONDS (5 * 60) /* seconds */

স্থির কাঠামো fsmonitor_batch *with_lock__truncate_old_batches(
	কাঠামো fsmonitor_daemon_state *state,
	ধ্রুবক কাঠামো fsmonitor_batch *batch_marker)
{
	/* assert current thread holding state->main_lock */

	ধ্রুবক কাঠামো fsmonitor_batch *batch;
	কাঠামো fsmonitor_batch *remainder;

	যদি (!batch_marker)
		ফেরত ফাঁকা;

	trace_printf_চাবি(&trace_fsmonitor, "Truncate: mark (%"PRIu64",%"PRIu64")",
			 batch_marker->batch_seq_nr,
			 (uint64_t)batch_marker->pinned_time);

	জন্য (batch = batch_marker; batch; batch = batch->পরবর্তী) {
		time_t t;

		যদি (!batch->pinned_time) /* an overflow batch */
			চালু;

		t = batch->pinned_time + MY_TIME_DELAY_SECONDS;
		যদি (t > batch_marker->pinned_time) /* too close to marker */
			চালু;

		যাও truncate_past_here;
	}

	ফেরত ফাঁকা;

truncate_past_here:
	state->current_token_তথ্য->batch_লেজ = (কাঠামো fsmonitor_batch *)batch;

	remainder = ((কাঠামো fsmonitor_batch *)batch)->পরবর্তী;
	((কাঠামো fsmonitor_batch *)batch)->পরবর্তী = ফাঁকা;

	ফেরত remainder;
}

স্থির শূন্য fsmonitor_free_token_তথ্য(কাঠামো fsmonitor_token_তথ্য *token)
{
	যদি (!token)
		ফেরত;

	প্রতিজ্ঞা(token->client_ref_গুনতি == 0);

	strbuf_release(&token->token_id);

	fsmonitor_batch__free_list(token->batch_শীর্ষ);

	মুক্ত(token);
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
স্থির শূন্য with_lock__do_force_resync(কাঠামো fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	কাঠামো fsmonitor_token_তথ্য *free_me = ফাঁকা;
	কাঠামো fsmonitor_token_তথ্য *new_one = ফাঁকা;

	new_one = fsmonitor_new_token_তথ্য();

	যদি (state->current_token_তথ্য->client_ref_গুনতি == 0)
		free_me = state->current_token_তথ্য;
	state->current_token_তথ্য = new_one;

	fsmonitor_free_token_তথ্য(free_me);

	with_lock__abort_all_cookies(state);
}

শূন্য fsmonitor_force_resync(কাঠামো fsmonitor_daemon_state *state)
{
	pthread_mutex_lock(&state->main_lock);
	with_lock__do_force_resync(state);
	pthread_mutex_unlock(&state->main_lock);
}

/*
 * Format an opaque token string to send to the client.
 */
স্থির শূন্য with_lock__format_response_token(
	কাঠামো strbuf *response_token,
	ধ্রুবক কাঠামো strbuf *response_token_id,
	ধ্রুবক কাঠামো fsmonitor_batch *batch)
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
স্থির পূর্ণসংখ্যা fsmonitor_parse_client_token(ধ্রুবক অক্ষর *buf_token,
					কাঠামো strbuf *requested_token_id,
					uint64_t *seq_nr)
{
	ধ্রুবক অক্ষর *p;
	অক্ষর *p_end;

	strbuf_reset(requested_token_id);
	*seq_nr = 0;

	যদি (!skip_prefix(buf_token, "builtin:", &p))
		ফেরত -1;

	যখন (*p && *p != ':')
		strbuf_addch(requested_token_id, *p++);
	যদি (!*p++)
		ফেরত -1;

	*seq_nr = (uint64_t)strtoumax(p, &p_end, 10);
	যদি (*p_end)
		ফেরত -1;

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা do_handle_client(কাঠামো fsmonitor_daemon_state *state,
			    ধ্রুবক অক্ষর *command,
			    ipc_server_reply_cb *reply,
			    কাঠামো ipc_server_reply_তথ্য *reply_তথ্য)
{
	কাঠামো fsmonitor_token_তথ্য *token_তথ্য = ফাঁকা;
	কাঠামো strbuf response_token = STRBUF_INIT;
	কাঠামো strbuf requested_token_id = STRBUF_INIT;
	কাঠামো strbuf payload = STRBUF_INIT;
	uint64_t requested_oldest_seq_nr = 0;
	uint64_t যোগ_response_len = 0;
	ধ্রুবক অক্ষর *p;
	ধ্রুবক কাঠামো fsmonitor_batch *batch_শীর্ষ;
	ধ্রুবক কাঠামো fsmonitor_batch *batch;
	কাঠামো fsmonitor_batch *remainder = ফাঁকা;
	intmax_t গুনতি = 0, duplicates = 0;
	কাঠামো strset shown = STRSET_INIT;
	পূর্ণসংখ্যা do_trivial = 0;
	পূর্ণসংখ্যা do_flush = 0;
	পূর্ণসংখ্যা do_cookie = 0;
	গণনা fsmonitor_cookie_item_ফলাফল cookie_ফলাফল;

	/*
	 * We expect `command` to be of the form:
	 *
	 * <command> := quit NUL
	 *            | flush NUL
	 *            | <V1-time-since-epoch-ns> NUL
	 *            | <V2-opaque-fsmonitor-token> NUL
	 */

	যদি (!strcmp(command, "quit")) {
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
		ফেরত SIMPLE_IPC_QUIT;

	} অন্যথা যদি (!strcmp(command, "flush")) {
		/*
		 * Flush all of our cached data and generate a new token
		 * just like if we lost sync with the filesystem.
		 *
		 * Then send a trivial response using the new token.
		 */
		do_flush = 1;
		do_trivial = 1;

	} অন্যথা যদি (!skip_prefix(command, "builtin:", &p)) {
		/* assume V1 timestamp or garbage */

		অক্ষর *p_end;

		strtoumax(command, &p_end, 10);
		trace_printf_চাবি(&trace_fsmonitor,
				 ((*p_end) ?
				  "fsmonitor: invalid command line '%s'" :
				  "fsmonitor: unsupported V1 protocol '%s'"),
				 command);
		do_trivial = 1;
		do_cookie = 1;

	} অন্যথা {
		/* We have "builtin:*" */
		যদি (fsmonitor_parse_client_token(command, &requested_token_id,
						 &requested_oldest_seq_nr)) {
			trace_printf_চাবি(&trace_fsmonitor,
					 "fsmonitor: invalid V2 protocol token '%s'",
					 command);
			do_trivial = 1;
			do_cookie = 1;

		} অন্যথা {
			/*
			 * We have a V2 valid token:
			 *     "builtin:<token_id>:<seq_nr>"
			 */
			do_cookie = 1;
		}
	}

	pthread_mutex_lock(&state->main_lock);

	যদি (!state->current_token_তথ্য)
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
	যদি (do_cookie) {
		cookie_ফলাফল = with_lock__wait_for_cookie(state);
		যদি (cookie_ফলাফল != FCIR_SEEN) {
			error(_("fsmonitor: cookie_result '%d' != SEEN"),
			      cookie_ফলাফল);
			do_trivial = 1;
		}
	}

	যদি (do_flush)
		with_lock__do_force_resync(state);

	/*
	 * We mark the current head of the batch list as "pinned" so
	 * that the listener thread will treat this item as read-only
	 * (and prevent any more paths from being added to it) from
	 * now on.
	 */
	token_তথ্য = state->current_token_তথ্য;
	batch_শীর্ষ = token_তথ্য->batch_শীর্ষ;
	((কাঠামো fsmonitor_batch *)batch_শীর্ষ)->pinned_time = time(ফাঁকা);

	/*
	 * FSMonitor Protocol V2 requires that we send a response header
	 * with a "new current token" and then all of the paths that changed
	 * since the "requested token".  We send the seq_nr of the just-pinned
	 * head batch so that future requests from a client will be relative
	 * to it.
	 */
	with_lock__format_response_token(&response_token,
					 &token_তথ্য->token_id, batch_শীর্ষ);

	reply(reply_তথ্য, response_token.buf, response_token.len + 1);
	যোগ_response_len += response_token.len + 1;

	trace2_তথ্য_string("fsmonitor", the_repository, "response/token",
			   response_token.buf);
	trace_printf_চাবি(&trace_fsmonitor, "response token: %s",
			 response_token.buf);

	যদি (!do_trivial) {
		যদি (strcmp(requested_token_id.buf, token_তথ্য->token_id.buf)) {
			/*
			 * The client last spoke to a different daemon
			 * instance -OR- the daemon had to resync with
			 * the filesystem (and lost events), so reject.
			 */
			trace2_তথ্য_string("fsmonitor", the_repository,
					   "response/token", "different");
			do_trivial = 1;

		} অন্যথা যদি (requested_oldest_seq_nr <
			   token_তথ্য->batch_লেজ->batch_seq_nr) {
			/*
			 * The client wants older events than we have for
			 * this token_id.  This means that the end of our
			 * batch list was truncated and we cannot give the
			 * client a complete snapshot relative to their
			 * request.
			 */
			trace_printf_চাবি(&trace_fsmonitor,
					 "client requested truncated data");
			do_trivial = 1;
		}
	}

	যদি (do_trivial) {
		pthread_mutex_unlock(&state->main_lock);

		reply(reply_তথ্য, "/", 2);

		trace2_তথ্য_intmax("fsmonitor", the_repository,
				   "response/trivial", 1);

		যাও cleanup;
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
	token_তথ্য->client_ref_গুনতি++;

	pthread_mutex_unlock(&state->main_lock);

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
	strset_init_with_options(&shown, ফাঁকা, 0);
	জন্য (batch = batch_শীর্ষ;
	     batch && batch->batch_seq_nr > requested_oldest_seq_nr;
	     batch = batch->পরবর্তী) {
		আকার_t k;

		জন্য (k = 0; k < batch->nr; k++) {
			ধ্রুবক অক্ষর *s = batch->interned_paths[k];
			আকার_t s_len;

			যদি (!strset_add(&shown, s))
				duplicates++;
			অন্যথা {
				trace_printf_চাবি(&trace_fsmonitor,
						 "send[%"PRIuMAX"]: %s",
						 গুনতি, s);

				/* Each path gets written with a trailing NUL */
				s_len = strlen(s) + 1;

				যদি (payload.len + s_len >=
				    LARGE_PACKET_তথ্য_MAX) {
					reply(reply_তথ্য, payload.buf,
					      payload.len);
					যোগ_response_len += payload.len;
					strbuf_reset(&payload);
				}

				strbuf_add(&payload, s, s_len);
				গুনতি++;
			}
		}
	}

	যদি (payload.len) {
		reply(reply_তথ্য, payload.buf, payload.len);
		যোগ_response_len += payload.len;
	}

	pthread_mutex_lock(&state->main_lock);

	যদি (token_তথ্য->client_ref_গুনতি > 0)
		token_তথ্য->client_ref_গুনতি--;

	যদি (token_তথ্য->client_ref_গুনতি == 0) {
		যদি (token_তথ্য != state->current_token_তথ্য) {
			/*
			 * The listener thread did a token-reset while we were
			 * walking the batch list.  Therefore, this token is
			 * stale and can be discarded completely.  If we are
			 * the last reader thread using this token, we own
			 * that work.
			 */
			fsmonitor_free_token_তথ্য(token_তথ্য);
		} অন্যথা যদি (batch) {
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
			remainder = with_lock__truncate_old_batches(state,
								    batch);
		}
	}

	pthread_mutex_unlock(&state->main_lock);

	যদি (remainder)
		fsmonitor_batch__free_list(remainder);

	trace2_তথ্য_intmax("fsmonitor", the_repository, "response/length", যোগ_response_len);
	trace2_তথ্য_intmax("fsmonitor", the_repository, "response/count/files", গুনতি);
	trace2_তথ্য_intmax("fsmonitor", the_repository, "response/count/duplicates", duplicates);

cleanup:
	strset_clear(&shown);
	strbuf_release(&response_token);
	strbuf_release(&requested_token_id);
	strbuf_release(&payload);

	ফেরত 0;
}

স্থির ipc_server_application_cb handle_client;

স্থির পূর্ণসংখ্যা handle_client(শূন্য *তথ্য,
			 ধ্রুবক অক্ষর *command, আকার_t command_len,
			 ipc_server_reply_cb *reply,
			 কাঠামো ipc_server_reply_তথ্য *reply_তথ্য)
{
	কাঠামো fsmonitor_daemon_state *state = তথ্য;
	পূর্ণসংখ্যা ফলাফল;

	/*
	 * The Simple IPC API now supports {char*, len} arguments, but
	 * FSMonitor always uses proper null-terminated strings, so
	 * we can ignore the command_len argument.  (Trust, but verify.)
	 */
	যদি (command_len != strlen(command))
		BUG("FSMonitor assumes text messages");

	trace_printf_চাবি(&trace_fsmonitor, "requested token: %s", command);

	trace2_region_enter("fsmonitor", "handle_client", the_repository);
	trace2_তথ্য_string("fsmonitor", the_repository, "request", command);

	ফলাফল = do_handle_client(state, command, reply, reply_তথ্য);

	trace2_region_leave("fsmonitor", "handle_client", the_repository);

	ফেরত ফলাফল;
}

#define FSMONITOR_DIR           "fsmonitor--daemon"
#define FSMONITOR_COOKIE_DIR    "cookies"
#define FSMONITOR_COOKIE_PREFIX (FSMONITOR_DIR "/" FSMONITOR_COOKIE_DIR "/")

গণনা fsmonitor_path_type fsmonitor_classify_path_workdir_relative(
	ধ্রুবক অক্ষর *rel)
{
	যদি (fspathncmp(rel, ".git", 4))
		ফেরত IS_WORKDIR_PATH;
	rel += 4;

	যদি (!*rel)
		ফেরত IS_DOT_GIT;
	যদি (*rel != '/')
		ফেরত IS_WORKDIR_PATH; /* e.g. .gitignore */
	rel++;

	যদি (!fspathncmp(rel, FSMONITOR_COOKIE_PREFIX,
			strlen(FSMONITOR_COOKIE_PREFIX)))
		ফেরত IS_INSIDE_DOT_GIT_WITH_COOKIE_PREFIX;

	ফেরত IS_INSIDE_DOT_GIT;
}

গণনা fsmonitor_path_type fsmonitor_classify_path_gitdir_relative(
	ধ্রুবক অক্ষর *rel)
{
	যদি (!fspathncmp(rel, FSMONITOR_COOKIE_PREFIX,
			strlen(FSMONITOR_COOKIE_PREFIX)))
		ফেরত IS_INSIDE_GITDIR_WITH_COOKIE_PREFIX;

	ফেরত IS_INSIDE_GITDIR;
}

স্থির গণনা fsmonitor_path_type try_classify_workdir_abs_path(
	কাঠামো fsmonitor_daemon_state *state,
	ধ্রুবক অক্ষর *path)
{
	ধ্রুবক অক্ষর *rel;

	যদি (fspathncmp(path, state->path_worktree_watch.buf,
		       state->path_worktree_watch.len))
		ফেরত IS_OUTSIDE_CONE;

	rel = path + state->path_worktree_watch.len;

	যদি (!*rel)
		ফেরত IS_WORKDIR_PATH; /* it is the root dir exactly */
	যদি (*rel != '/')
		ফেরত IS_OUTSIDE_CONE;
	rel++;

	ফেরত fsmonitor_classify_path_workdir_relative(rel);
}

গণনা fsmonitor_path_type fsmonitor_classify_path_absolute(
	কাঠামো fsmonitor_daemon_state *state,
	ধ্রুবক অক্ষর *path)
{
	ধ্রুবক অক্ষর *rel;
	গণনা fsmonitor_path_type t;

	t = try_classify_workdir_abs_path(state, path);
	যদি (state->nr_paths_watching == 1)
		ফেরত t;
	যদি (t != IS_OUTSIDE_CONE)
		ফেরত t;

	যদি (fspathncmp(path, state->path_gitdir_watch.buf,
		       state->path_gitdir_watch.len))
		ফেরত IS_OUTSIDE_CONE;

	rel = path + state->path_gitdir_watch.len;

	যদি (!*rel)
		ফেরত IS_GITDIR; /* it is the <gitdir> exactly */
	যদি (*rel != '/')
		ফেরত IS_OUTSIDE_CONE;
	rel++;

	ফেরত fsmonitor_classify_path_gitdir_relative(rel);
}

/*
 * We try to combine small batches at the front of the batch-list to avoid
 * having a long list.  This hopefully makes it a little easier when we want
 * to truncate and maintain the list.  However, we don't want the paths array
 * to just keep growing and growing with realloc, so we insert an arbitrary
 * limit.
 */
#define MY_COMBINE_LIMIT (1024)

শূন্য fsmonitor_publish(কাঠামো fsmonitor_daemon_state *state,
		       কাঠামো fsmonitor_batch *batch,
		       ধ্রুবক কাঠামো string_list *cookie_names)
{
	যদি (!batch && !cookie_names->nr)
		ফেরত;

	pthread_mutex_lock(&state->main_lock);

	যদি (batch) {
		কাঠামো fsmonitor_batch *শীর্ষ;

		শীর্ষ = state->current_token_তথ্য->batch_শীর্ষ;
		যদি (!শীর্ষ) {
			BUG("token does not have batch");
		} অন্যথা যদি (শীর্ষ->pinned_time) {
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
			batch->batch_seq_nr = শীর্ষ->batch_seq_nr + 1;
			batch->পরবর্তী = শীর্ষ;
			state->current_token_তথ্য->batch_শীর্ষ = batch;
		} অন্যথা যদি (!শীর্ষ->batch_seq_nr) {
			/*
			 * Batch 0 is unpinned.  See the note in
			 * `fsmonitor_new_token_data()` about why we
			 * don't need to accumulate these paths.
			 */
			fsmonitor_batch__free_list(batch);
		} অন্যথা যদি (শীর্ষ->nr + batch->nr > MY_COMBINE_LIMIT) {
			/*
			 * The head batch in the list has never been
			 * transmitted to a client, but folding the
			 * contents of the new batch onto it would
			 * exceed our arbitrary limit, so just prepend
			 * the new batch onto the list.
			 */
			batch->batch_seq_nr = শীর্ষ->batch_seq_nr + 1;
			batch->পরবর্তী = শীর্ষ;
			state->current_token_তথ্য->batch_শীর্ষ = batch;
		} অন্যথা {
			/*
			 * We are free to add the paths in the given
			 * batch onto the end of the current head batch.
			 */
			fsmonitor_batch__combine(শীর্ষ, batch);
			fsmonitor_batch__free_list(batch);
		}
	}

	যদি (cookie_names->nr)
		with_lock__mark_cookies_seen(state, cookie_names);

	pthread_mutex_unlock(&state->main_lock);
}

স্থির শূন্য *fsm_health__thread_proc(শূন্য *_state)
{
	কাঠামো fsmonitor_daemon_state *state = _state;

	trace2_thread_start("fsm-health");

	fsm_health__loop(state);

	trace2_thread_exit();
	ফেরত ফাঁকা;
}

স্থির শূন্য *fsm_listen__thread_proc(শূন্য *_state)
{
	কাঠামো fsmonitor_daemon_state *state = _state;

	trace2_thread_start("fsm-listen");

	trace_printf_চাবি(&trace_fsmonitor, "Watching: worktree '%s'",
			 state->path_worktree_watch.buf);
	যদি (state->nr_paths_watching > 1)
		trace_printf_চাবি(&trace_fsmonitor, "Watching: gitdir '%s'",
				 state->path_gitdir_watch.buf);

	fsm_listen__loop(state);

	pthread_mutex_lock(&state->main_lock);
	যদি (state->current_token_তথ্য &&
	    state->current_token_তথ্য->client_ref_গুনতি == 0)
		fsmonitor_free_token_তথ্য(state->current_token_তথ্য);
	state->current_token_তথ্য = ফাঁকা;
	pthread_mutex_unlock(&state->main_lock);

	trace2_thread_exit();
	ফেরত ফাঁকা;
}

স্থির পূর্ণসংখ্যা fsmonitor_run_daemon_1(কাঠামো fsmonitor_daemon_state *state)
{
	কাঠামো ipc_server_opts ipc_opts = {
		.nr_threads = fsmonitor__ipc_threads,

		/*
		 * We know that there are no other active threads yet,
		 * so we can let the IPC layer temporarily chdir() if
		 * it needs to when creating the server side of the
		 * Unix domain socket.
		 */
		.uds_disallow_chdir = 0
	};
	পূর্ণসংখ্যা health_started = 0;
	পূর্ণসংখ্যা listener_started = 0;
	পূর্ণসংখ্যা err = 0;

	/*
	 * Start the IPC thread pool before the we've started the file
	 * system event listener thread so that we have the IPC handle
	 * before we need it.
	 */
	যদি (ipc_server_init_async(&state->ipc_server_তথ্য,
				  state->path_ipc.buf, &ipc_opts,
				  handle_client, state))
		ফেরত error_errno(
			_("could not start IPC thread pool on '%s'"),
			state->path_ipc.buf);

	/*
	 * Start the fsmonitor listener thread to collect filesystem
	 * events.
	 */
	যদি (pthread_create(&state->listener_thread, ফাঁকা,
			   fsm_listen__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_তথ্য);
		err = error(_("could not start fsmonitor listener thread"));
		যাও cleanup;
	}
	listener_started = 1;

	/*
	 * Start the health thread to watch over our process.
	 */
	যদি (pthread_create(&state->health_thread, ফাঁকা,
			   fsm_health__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_তথ্য);
		err = error(_("could not start fsmonitor health thread"));
		যাও cleanup;
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
	ipc_server_await(state->ipc_server_তথ্য);

	/*
	 * The fsmonitor listener thread may have received a shutdown
	 * event from the IPC thread pool, but it doesn't hurt to tell
	 * it again.  And wait for it to shutdown.
	 */
	যদি (listener_started) {
		fsm_listen__stop_async(state);
		pthread_join(state->listener_thread, ফাঁকা);
	}

	যদি (health_started) {
		fsm_health__stop_async(state);
		pthread_join(state->health_thread, ফাঁকা);
	}

	যদি (err)
		ফেরত err;
	যদি (state->listen_error_code)
		ফেরত state->listen_error_code;
	যদি (state->health_error_code)
		ফেরত state->health_error_code;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা fsmonitor_run_daemon(শূন্য)
{
	কাঠামো fsmonitor_daemon_state state;
	ধ্রুবক অক্ষর *home;
	পূর্ণসংখ্যা err;

	memset(&state, 0, আকার(state));

	hashmap_init(&state.cookies, cookies_cmp, ফাঁকা, 0);
	pthread_mutex_init(&state.main_lock, ফাঁকা);
	pthread_cond_init(&state.cookies_cond, ফাঁকা);
	state.listen_error_code = 0;
	state.health_error_code = 0;
	state.current_token_তথ্য = fsmonitor_new_token_তথ্য();

	/* Prepare to (recursively) watch the <worktree-root> directory. */
	strbuf_init(&state.path_worktree_watch, 0);
	strbuf_addstr(&state.path_worktree_watch,
		      absolute_path(repo_get_work_বৃক্ষ(the_repository)));
	state.nr_paths_watching = 1;

	strbuf_init(&state.alias.alias, 0);
	strbuf_init(&state.alias.points_to, 0);
	যদি ((err = fsmonitor__get_alias(state.path_worktree_watch.buf, &state.alias)))
		যাও done;

	/*
	 * We create and delete cookie files somewhere inside the .git
	 * directory to help us keep sync with the file system.  If
	 * ".git" is not a directory, then <gitdir> is not inside the
	 * cone of <worktree-root>, so set up a second watch to watch
	 * the <gitdir> so that we get events for the cookie files.
	 */
	strbuf_init(&state.path_gitdir_watch, 0);
	strbuf_addbuf(&state.path_gitdir_watch, &state.path_worktree_watch);
	strbuf_addstr(&state.path_gitdir_watch, "/.git");
	যদি (!is_directory(state.path_gitdir_watch.buf)) {
		strbuf_reset(&state.path_gitdir_watch);
		strbuf_addstr(&state.path_gitdir_watch,
			      absolute_path(repo_get_git_dir(the_repository)));
		strbuf_strip_suffix(&state.path_gitdir_watch, "/.");
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
	strbuf_init(&state.path_cookie_prefix, 0);
	strbuf_addbuf(&state.path_cookie_prefix, &state.path_gitdir_watch);

	strbuf_addch(&state.path_cookie_prefix, '/');
	strbuf_addstr(&state.path_cookie_prefix, FSMONITOR_DIR);
	mkdir(state.path_cookie_prefix.buf, 0777);

	strbuf_addch(&state.path_cookie_prefix, '/');
	strbuf_addstr(&state.path_cookie_prefix, FSMONITOR_COOKIE_DIR);
	mkdir(state.path_cookie_prefix.buf, 0777);

	strbuf_addch(&state.path_cookie_prefix, '/');

	/*
	 * We create a named-pipe or unix domain socket inside of the
	 * ".git" directory.  (Well, on Windows, we base our named
	 * pipe in the NPFS on the absolute path of the git
	 * directory.)
	 */
	strbuf_init(&state.path_ipc, 0);
	strbuf_addstr(&state.path_ipc,
		absolute_path(fsmonitor_ipc__get_path(the_repository)));

	/*
	 * Confirm that we can create platform-specific resources for the
	 * filesystem listener before we bother starting all the threads.
	 */
	যদি (fsm_listen__ctor(&state)) {
		err = error(_("could not initialize listener thread"));
		যাও done;
	}

	যদি (fsm_health__ctor(&state)) {
		err = error(_("could not initialize health thread"));
		যাও done;
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
	যদি (home && *home && chdir(home))
		die_errno(_("could not cd home '%s'"), home);

	err = fsmonitor_run_daemon_1(&state);

done:
	fsmonitor_free_token_তথ্য(state.current_token_তথ্য);
	state.current_token_তথ্য = ফাঁকা;
	pthread_cond_destroy(&state.cookies_cond);
	pthread_mutex_destroy(&state.main_lock);
	{
		কাঠামো hashmap_iter iter;
		কাঠামো fsmonitor_cookie_item *cookie;

		hashmap_for_each_entry(&state.cookies, &iter, cookie, entry)
			মুক্ত(cookie->name);
		hashmap_clear_and_free(&state.cookies,
				       কাঠামো fsmonitor_cookie_item, entry);
	}
	fsm_listen__dtor(&state);
	fsm_health__dtor(&state);

	ipc_server_free(state.ipc_server_তথ্য);

	strbuf_release(&state.path_worktree_watch);
	strbuf_release(&state.path_gitdir_watch);
	strbuf_release(&state.path_cookie_prefix);
	strbuf_release(&state.path_ipc);
	strbuf_release(&state.alias.alias);
	strbuf_release(&state.alias.points_to);

	ফেরত err;
}

স্থির পূর্ণসংখ্যা try_to_run_foreground_daemon(পূর্ণসংখ্যা detach_console)
{
	/*
	 * Technically, we don't need to probe for an existing daemon
	 * process, since we could just call `fsmonitor_run_daemon()`
	 * and let it fail if the pipe/socket is busy.
	 *
	 * However, this method gives us a nicer error message for a
	 * common error case.
	 */
	যদি (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	যদি (fsmonitor__announce_startup) {
		fprintf(stderr, _("running fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	যদি (detach_console) {
#ifdef GIT_WINDOWS_NATIVE
		FreeConsole();
#অন্যথা
		/*
		 * Create a new session so that the daemon is detached
		 * from the parent's process group.  This prevents
		 * shells with job control (e.g. bash with "set -m")
		 * from waiting on the daemon when they wait for a
		 * foreground command that implicitly spawned it.
		 */
		যদি (setsid() == -1)
			warning_errno(_("setsid failed"));
#endif
	}

	ফেরত !!fsmonitor_run_daemon();
}

স্থির start_bg_wait_cb bg_wait_cb;

স্থির পূর্ণসংখ্যা bg_wait_cb(ধ্রুবক কাঠামো child_process *cp UNUSED,
		      শূন্য *cb_তথ্য UNUSED)
{
	গণনা ipc_active_state s = fsmonitor_ipc__get_state();

	নির্বাচন (s) {
	ক্ষেত্র IPC_STATE__LISTENING:
		/* child is "ready" */
		ফেরত 0;

	ক্ষেত্র IPC_STATE__NOT_LISTENING:
	ক্ষেত্র IPC_STATE__PATH_NOT_FOUND:
		/* give child more time */
		ফেরত 1;

	পূর্বনির্ধারিত:
	ক্ষেত্র IPC_STATE__INVALID_PATH:
	ক্ষেত্র IPC_STATE__OTHER_ERROR:
		/* all the time in world won't help */
		ফেরত -1;
	}
}

স্থির পূর্ণসংখ্যা try_to_start_background_daemon(শূন্য)
{
	কাঠামো child_process cp = CHILD_PROCESS_INIT;
	গণনা start_bg_ফলাফল sbgr;

	/*
	 * Before we try to create a background daemon process, see
	 * if a daemon process is already listening.  This makes it
	 * easier for us to report an already-listening error to the
	 * console, since our spawn/daemon can only report the success
	 * of creating the background process (and not whether it
	 * immediately exited).
	 */
	যদি (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	যদি (fsmonitor__announce_startup) {
		fprintf(stderr, _("starting fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	cp.git_cmd = 1;

	strvec_ঠেলো(&cp.args, "fsmonitor--daemon");
	strvec_ঠেলো(&cp.args, "run");
	strvec_ঠেলো(&cp.args, "--detach");
	strvec_pushf(&cp.args, "--ipc-threads=%d", fsmonitor__ipc_threads);

	cp.no_stdin = 1;
	cp.no_stdout = 1;
	cp.no_stderr = 1;
	cp.close_fd_above_stderr = 1;

	sbgr = start_bg_command(&cp, bg_wait_cb, ফাঁকা,
				fsmonitor__start_timeout_sec);

	নির্বাচন (sbgr) {
	ক্ষেত্র SBGR_READY:
		ফেরত 0;

	পূর্বনির্ধারিত:
	ক্ষেত্র SBGR_ERROR:
	ক্ষেত্র SBGR_CB_ERROR:
		ফেরত error(_("daemon failed to start"));

	ক্ষেত্র SBGR_TIMEOUT:
		ফেরত error(_("daemon not online yet"));

	ক্ষেত্র SBGR_DIED:
		ফেরত error(_("daemon terminated"));
	}
}

পূর্ণসংখ্যা cmd_fsmonitor__daemon(পূর্ণসংখ্যা argc,
			  ধ্রুবক অক্ষর **argv,
			  ধ্রুবক অক্ষর *prefix,
			  কাঠামো repository *repo UNUSED)
{
	ধ্রুবক অক্ষর *subcmd;
	গণনা fsmonitor_reason reason;
	পূর্ণসংখ্যা detach_console = 0;

	কাঠামো option options[] = {
		OPT_BOOL(0, "detach", &detach_console, N_("detach from console")),
		OPT_INTEGER(0, "ipc-threads",
			    &fsmonitor__ipc_threads,
			    N_("use <n> ipc worker threads")),
		OPT_INTEGER(0, "start-timeout",
			    &fsmonitor__start_timeout_sec,
			    N_("max seconds to wait for background daemon startup")),

		OPT_END()
	};

	repo_config(the_repository, fsmonitor_config, ফাঁকা);

	argc = parse_options(argc, argv, prefix, options,
			     builtin_fsmonitor__daemon_usage, 0);
	যদি (argc != 1)
		usage_with_options(builtin_fsmonitor__daemon_usage, options);
	subcmd = argv[0];

	যদি (fsmonitor__ipc_threads < 1)
		die(_("invalid 'ipc-threads' value (%d)"),
		    fsmonitor__ipc_threads);

	prepare_repo_settings(the_repository);
	/*
	 * If the repo is fsmonitor-compatible, explicitly set IPC-mode
	 * (without bothering to load the `core.fsmonitor` config settings).
	 *
	 * If the repo is not compatible, the repo-settings will be set to
	 * incompatible rather than IPC, so we can use one of the __get
	 * routines to detect the discrepancy.
	 */
	fsm_settings__set_ipc(the_repository);

	reason = fsm_settings__get_reason(the_repository);
	যদি (reason > FSMONITOR_REASON_OK)
		die("%s",
		    fsm_settings_get_incompatible_বার্তা(the_repository,
						       reason));

	যদি (!strcmp(subcmd, "start"))
		ফেরত !!try_to_start_background_daemon();

	যদি (!strcmp(subcmd, "run"))
		ফেরত !!try_to_run_foreground_daemon(detach_console);

	যদি (!strcmp(subcmd, "stop"))
		ফেরত !!do_as_client__send_stop();

	যদি (!strcmp(subcmd, "status"))
		ফেরত !!do_as_client__status();

	die(_("Unhandled subcommand '%s'"), subcmd);
}

#অন্যথা
পূর্ণসংখ্যা cmd_fsmonitor__daemon(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix UNUSED, কাঠামো repository *repo UNUSED)
{
	কাঠামো option options[] = {
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_fsmonitor__daemon_usage, options);

	die(_("fsmonitor--daemon not supported on this platform"));
}
#endif
