#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "fsmonitor-ll.h"
#അന്തര്ഭാവ "fsmonitor-ipc.h"
#അന്തര്ഭാവ "fsmonitor-settings.h"
#അന്തര്ഭാവ "compat/fsmonitor/fsm-health.h"
#അന്തര്ഭാവ "compat/fsmonitor/fsm-listen.h"
#അന്തര്ഭാവ "fsmonitor--daemon.h"

#അന്തര്ഭാവ "simple-ipc.h"
#അന്തര്ഭാവ "strmap.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "trace.h"
#അന്തര്ഭാവ "trace2.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_fsmonitor__daemon_usage[] = {
	N_("git fsmonitor--daemon start [<options>]"),
	N_("git fsmonitor--daemon run [<options>]"),
	"git fsmonitor--daemon stop",
	"git fsmonitor--daemon status",
	ശൂന്യ
};

#ifdef HAVE_FSMONITOR_DAEMON_BACKEND
/*
 * Global state loaded from config.
 */
#define FSMONITOR__IPC_THREADS "fsmonitor.ipcthreads"
സ്ഥിതം പൂർണ്ണസംഖ്യ fsmonitor__ipc_threads = 8;

#define FSMONITOR__START_TIMEOUT "fsmonitor.starttimeout"
സ്ഥിതം പൂർണ്ണസംഖ്യ fsmonitor__start_timeout_sec = 60;

#define FSMONITOR__ANNOUNCE_STARTUP "fsmonitor.announcestartup"
സ്ഥിതം പൂർണ്ണസംഖ്യ fsmonitor__announce_startup = 0;

സ്ഥിതം പൂർണ്ണസംഖ്യ fsmonitor_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			    സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(var, FSMONITOR__IPC_THREADS)) {
		പൂർണ്ണസംഖ്യ i = git_config_int(var, മൂല്യം, ctx->kvi);
		എങ്കിൽ (i < 1)
			തിരികെ error(_("value of '%s' out of range: %d"),
				     FSMONITOR__IPC_THREADS, i);
		fsmonitor__ipc_threads = i;
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, FSMONITOR__START_TIMEOUT)) {
		പൂർണ്ണസംഖ്യ i = git_config_int(var, മൂല്യം, ctx->kvi);
		എങ്കിൽ (i < 0)
			തിരികെ error(_("value of '%s' out of range: %d"),
				     FSMONITOR__START_TIMEOUT, i);
		fsmonitor__start_timeout_sec = i;
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, FSMONITOR__ANNOUNCE_STARTUP)) {
		പൂർണ്ണസംഖ്യ is_bool;
		പൂർണ്ണസംഖ്യ i = git_config_bool_or_int(var, മൂല്യം, ctx->kvi, &is_bool);
		എങ്കിൽ (i < 0)
			തിരികെ error(_("value of '%s' not bool or int: %d"),
				     var, i);
		fsmonitor__announce_startup = i;
		തിരികെ 0;
	}

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

/*
 * Acting as a CLIENT.
 *
 * Send a "quit" command to the `git-fsmonitor--daemon` (if running)
 * and wait for it to shutdown.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ do_as_client__send_stop(ശൂന്യം)
{
	ഘടന strbuf answer = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret;
	പൂർണ്ണസംഖ്യ max_wait_ms = 30000;
	പൂർണ്ണസംഖ്യ elapsed_ms = 0;

	ret = fsmonitor_ipc__send_command("quit", &answer);

	/* The quit command does not return any response data. */
	strbuf_release(&answer);

	എങ്കിൽ (ret)
		തിരികെ ret;

	trace2_region_enter("fsm_client", "polling-for-daemon-exit", ശൂന്യ);
	ആകുമ്പോൾ (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING) {
		എങ്കിൽ (elapsed_ms >= max_wait_ms) {
			trace2_region_leave("fsm_client",
					    "polling-for-daemon-exit", ശൂന്യ);
			തിരികെ error(_("daemon did not stop within %d seconds"),
				     max_wait_ms / 1000);
		}
		sleep_millisec(50);
		elapsed_ms += 50;
	}
	trace2_region_leave("fsm_client", "polling-for-daemon-exit", ശൂന്യ);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_as_client__status(ശൂന്യം)
{
	എണ്ണൽ ipc_active_state state = fsmonitor_ipc__get_state();

	തിരഞ്ഞെടുപ്പ് (state) {
	അവസ്ഥ IPC_STATE__LISTENING:
		printf(_("fsmonitor-daemon is watching '%s'\n"),
		       the_repository->worktree);
		തിരികെ 0;

	സ്ഥിരസ്ഥിതി:
		printf(_("fsmonitor-daemon is not watching '%s'\n"),
		       the_repository->worktree);
		തിരികെ 1;
	}
}

എണ്ണൽ fsmonitor_cookie_item_result {
	FCIR_ERROR = -1, /* could not create cookie file ? */
	FCIR_INIT,
	FCIR_SEEN,
	FCIR_ABORT,
};

ഘടന fsmonitor_cookie_item {
	ഘടന hashmap_entry entry;
	അക്ഷരം *name;
	എണ്ണൽ fsmonitor_cookie_item_result result;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ cookies_cmp(സ്ഥിരം ശൂന്യം *data UNUSED,
		       സ്ഥിരം ഘടന hashmap_entry *he1,
		       സ്ഥിരം ഘടന hashmap_entry *he2, സ്ഥിരം ശൂന്യം *keydata)
{
	സ്ഥിരം ഘടന fsmonitor_cookie_item *a =
		container_of(he1, സ്ഥിരം ഘടന fsmonitor_cookie_item, entry);
	സ്ഥിരം ഘടന fsmonitor_cookie_item *b =
		container_of(he2, സ്ഥിരം ഘടന fsmonitor_cookie_item, entry);

	തിരികെ strcmp(a->name, keydata ? keydata : b->name);
}

സ്ഥിതം എണ്ണൽ fsmonitor_cookie_item_result with_lock__wait_for_cookie(
	ഘടന fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	പൂർണ്ണസംഖ്യ fd;
	ഘടന fsmonitor_cookie_item *cookie;
	ഘടന strbuf cookie_pathname = STRBUF_INIT;
	ഘടന strbuf cookie_filename = STRBUF_INIT;
	എണ്ണൽ fsmonitor_cookie_item_result result;
	പൂർണ്ണസംഖ്യ my_cookie_seq;

	CALLOC_ARRAY(cookie, 1);

	my_cookie_seq = state->cookie_seq++;

	strbuf_addf(&cookie_filename, "%i-%i", getpid(), my_cookie_seq);

	strbuf_addbuf(&cookie_pathname, &state->path_cookie_prefix);
	strbuf_addbuf(&cookie_pathname, &cookie_filename);

	cookie->name = strbuf_detach(&cookie_filename, ശൂന്യ);
	cookie->result = FCIR_INIT;
	hashmap_entry_init(&cookie->entry, strhash(cookie->name));

	hashmap_add(&state->cookies, &cookie->entry);

	trace_printf_താക്കോൽ(&trace_fsmonitor, "cookie-wait: '%s' '%s'",
			 cookie->name, cookie_pathname.buf);

	/*
	 * Create the cookie file on disk and then wait for a notification
	 * that the listener thread has seen it.
	 */
	fd = open(cookie_pathname.buf, O_WRONLY | O_CREAT | O_EXCL, 0600);
	എങ്കിൽ (fd < 0) {
		error_errno(_("could not create fsmonitor cookie '%s'"),
			    cookie->name);

		cookie->result = FCIR_ERROR;
		പോകുക done;
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
		ഘടന timeval now;
		ഘടന timespec ts;
		പൂർണ്ണസംഖ്യ err = 0;

		gettimeofday(&now, ശൂന്യ);
		ts.tv_sec = now.tv_sec + 1;
		ts.tv_nsec = now.tv_usec * 1000;

		ആകുമ്പോൾ (cookie->result == FCIR_INIT && !err)
			err = pthread_cond_timedwait(&state->cookies_cond,
						     &state->main_lock,
						     &ts);
		എങ്കിൽ (err == ETIMEDOUT && cookie->result == FCIR_INIT) {
			trace_printf_താക്കോൽ(&trace_fsmonitor,
					 "cookie_wait timed out");
			cookie->result = FCIR_ERROR;
		}
	}

done:
	hashmap_remove(&state->cookies, &cookie->entry, ശൂന്യ);

	result = cookie->result;

	സ്വതന്ത്രം(cookie->name);
	സ്വതന്ത്രം(cookie);
	strbuf_release(&cookie_pathname);

	തിരികെ result;
}

/*
 * Mark these cookies as _SEEN and wake up the corresponding client threads.
 */
സ്ഥിതം ശൂന്യം with_lock__mark_cookies_seen(ഘടന fsmonitor_daemon_state *state,
					 സ്ഥിരം ഘടന string_list *cookie_names)
{
	/* assert current thread holding state->main_lock */

	പൂർണ്ണസംഖ്യ k;
	പൂർണ്ണസംഖ്യ nr_seen = 0;

	വേണ്ടി (k = 0; k < cookie_names->nr; k++) {
		ഘടന fsmonitor_cookie_item താക്കോൽ;
		ഘടന fsmonitor_cookie_item *cookie;

		താക്കോൽ.name = cookie_names->ഇനങ്ങൾ[k].string;
		hashmap_entry_init(&താക്കോൽ.entry, strhash(താക്കോൽ.name));

		cookie = hashmap_get_entry(&state->cookies, &താക്കോൽ, entry, ശൂന്യ);
		എങ്കിൽ (cookie) {
			trace_printf_താക്കോൽ(&trace_fsmonitor, "cookie-seen: '%s'",
					 cookie->name);
			cookie->result = FCIR_SEEN;
			nr_seen++;
		}
	}

	എങ്കിൽ (nr_seen)
		pthread_cond_broadcast(&state->cookies_cond);
}

/*
 * Set _ABORT on all pending cookies and wake up all client threads.
 */
സ്ഥിതം ശൂന്യം with_lock__abort_all_cookies(ഘടന fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	ഘടന hashmap_iter iter;
	ഘടന fsmonitor_cookie_item *cookie;
	പൂർണ്ണസംഖ്യ nr_aborted = 0;

	hashmap_for_each_entry(&state->cookies, &iter, cookie, entry) {
		trace_printf_താക്കോൽ(&trace_fsmonitor, "cookie-abort: '%s'",
				 cookie->name);
		cookie->result = FCIR_ABORT;
		nr_aborted++;
	}

	എങ്കിൽ (nr_aborted)
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
ഘടന fsmonitor_token_data {
	ഘടന strbuf token_id;
	ഘടന fsmonitor_batch *batch_head;
	ഘടന fsmonitor_batch *batch_tail;
	uint64_t client_ref_എണ്ണം;
};

ഘടന fsmonitor_batch {
	ഘടന fsmonitor_batch *next;
	uint64_t batch_seq_nr;
	സ്ഥിരം അക്ഷരം **interned_paths;
	size_t nr, alloc;
	time_t pinned_time;
};

സ്ഥിതം ഘടന fsmonitor_token_data *fsmonitor_new_token_data(ശൂന്യം)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ test_env_മൂല്യം = -1;
	സ്ഥിതം uint64_t flush_എണ്ണം = 0;
	ഘടന fsmonitor_token_data *token;
	ഘടന fsmonitor_batch *batch;

	CALLOC_ARRAY(token, 1);
	batch = fsmonitor_batch__new();

	strbuf_init(&token->token_id, 0);
	token->batch_head = batch;
	token->batch_tail = batch;
	token->client_ref_എണ്ണം = 0;

	എങ്കിൽ (test_env_മൂല്യം < 0)
		test_env_മൂല്യം = git_env_bool("GIT_TEST_FSMONITOR_TOKEN", 0);

	എങ്കിൽ (!test_env_മൂല്യം) {
		ഘടന timeval tv;
		ഘടന tm tm;
		time_t secs;

		gettimeofday(&tv, ശൂന്യ);
		secs = tv.tv_sec;
		gmtime_r(&secs, &tm);

		strbuf_addf(&token->token_id,
			    "%"PRIu64".%d.%4d%02d%02dT%02d%02d%02d.%06ldZ",
			    flush_എണ്ണം++,
			    getpid(),
			    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			    tm.tm_hour, tm.tm_min, tm.tm_sec,
			    (long)tv.tv_usec);
	} അല്ലെങ്കിൽ {
		strbuf_addf(&token->token_id, "test_%08x", test_env_മൂല്യം++);
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
	എങ്കിൽ (test_env_മൂല്യം)
		batch->pinned_time = time(ശൂന്യ);

	തിരികെ token;
}

ഘടന fsmonitor_batch *fsmonitor_batch__new(ശൂന്യം)
{
	ഘടന fsmonitor_batch *batch;

	CALLOC_ARRAY(batch, 1);

	തിരികെ batch;
}

ശൂന്യം fsmonitor_batch__free_list(ഘടന fsmonitor_batch *batch)
{
	ആകുമ്പോൾ (batch) {
		ഘടന fsmonitor_batch *next = batch->next;

		/*
		 * The actual strings within the array of this batch
		 * are interned, so we don't own them.  We only own
		 * the array.
		 */
		സ്വതന്ത്രം(batch->interned_paths);
		സ്വതന്ത്രം(batch);

		batch = next;
	}
}

ശൂന്യം fsmonitor_batch__add_path(ഘടന fsmonitor_batch *batch,
			       സ്ഥിരം അക്ഷരം *path)
{
	സ്ഥിരം അക്ഷരം *interned_path = strintern(path);

	trace_printf_താക്കോൽ(&trace_fsmonitor, "event: %s", interned_path);

	ALLOC_GROW(batch->interned_paths, batch->nr + 1, batch->alloc);
	batch->interned_paths[batch->nr++] = interned_path;
}

സ്ഥിതം ശൂന്യം fsmonitor_batch__combine(ഘടന fsmonitor_batch *batch_dest,
				     സ്ഥിരം ഘടന fsmonitor_batch *batch_src)
{
	size_t k;

	ALLOC_GROW(batch_dest->interned_paths,
		   batch_dest->nr + batch_src->nr + 1,
		   batch_dest->alloc);

	വേണ്ടി (k = 0; k < batch_src->nr; k++)
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

സ്ഥിതം ഘടന fsmonitor_batch *with_lock__truncate_old_batches(
	ഘടന fsmonitor_daemon_state *state,
	സ്ഥിരം ഘടന fsmonitor_batch *batch_marker)
{
	/* assert current thread holding state->main_lock */

	സ്ഥിരം ഘടന fsmonitor_batch *batch;
	ഘടന fsmonitor_batch *remainder;

	എങ്കിൽ (!batch_marker)
		തിരികെ ശൂന്യ;

	trace_printf_താക്കോൽ(&trace_fsmonitor, "Truncate: mark (%"PRIu64",%"PRIu64")",
			 batch_marker->batch_seq_nr,
			 (uint64_t)batch_marker->pinned_time);

	വേണ്ടി (batch = batch_marker; batch; batch = batch->next) {
		time_t t;

		എങ്കിൽ (!batch->pinned_time) /* an overflow batch */
			തുടരുക;

		t = batch->pinned_time + MY_TIME_DELAY_SECONDS;
		എങ്കിൽ (t > batch_marker->pinned_time) /* too close to marker */
			തുടരുക;

		പോകുക truncate_past_here;
	}

	തിരികെ ശൂന്യ;

truncate_past_here:
	state->current_token_data->batch_tail = (ഘടന fsmonitor_batch *)batch;

	remainder = ((ഘടന fsmonitor_batch *)batch)->next;
	((ഘടന fsmonitor_batch *)batch)->next = ശൂന്യ;

	തിരികെ remainder;
}

സ്ഥിതം ശൂന്യം fsmonitor_free_token_data(ഘടന fsmonitor_token_data *token)
{
	എങ്കിൽ (!token)
		തിരികെ;

	പ്രതിജ്ഞാ(token->client_ref_എണ്ണം == 0);

	strbuf_release(&token->token_id);

	fsmonitor_batch__free_list(token->batch_head);

	സ്വതന്ത്രം(token);
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
സ്ഥിതം ശൂന്യം with_lock__do_force_resync(ഘടന fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	ഘടന fsmonitor_token_data *free_me = ശൂന്യ;
	ഘടന fsmonitor_token_data *new_one = ശൂന്യ;

	new_one = fsmonitor_new_token_data();

	എങ്കിൽ (state->current_token_data->client_ref_എണ്ണം == 0)
		free_me = state->current_token_data;
	state->current_token_data = new_one;

	fsmonitor_free_token_data(free_me);

	with_lock__abort_all_cookies(state);
}

ശൂന്യം fsmonitor_force_resync(ഘടന fsmonitor_daemon_state *state)
{
	pthread_mutex_lock(&state->main_lock);
	with_lock__do_force_resync(state);
	pthread_mutex_unlock(&state->main_lock);
}

/*
 * Format an opaque token string to send to the client.
 */
സ്ഥിതം ശൂന്യം with_lock__format_response_token(
	ഘടന strbuf *response_token,
	സ്ഥിരം ഘടന strbuf *response_token_id,
	സ്ഥിരം ഘടന fsmonitor_batch *batch)
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
സ്ഥിതം പൂർണ്ണസംഖ്യ fsmonitor_parse_client_token(സ്ഥിരം അക്ഷരം *buf_token,
					ഘടന strbuf *requested_token_id,
					uint64_t *seq_nr)
{
	സ്ഥിരം അക്ഷരം *p;
	അക്ഷരം *p_end;

	strbuf_reset(requested_token_id);
	*seq_nr = 0;

	എങ്കിൽ (!skip_prefix(buf_token, "builtin:", &p))
		തിരികെ -1;

	ആകുമ്പോൾ (*p && *p != ':')
		strbuf_addch(requested_token_id, *p++);
	എങ്കിൽ (!*p++)
		തിരികെ -1;

	*seq_nr = (uint64_t)strtoumax(p, &p_end, 10);
	എങ്കിൽ (*p_end)
		തിരികെ -1;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_handle_client(ഘടന fsmonitor_daemon_state *state,
			    സ്ഥിരം അക്ഷരം *command,
			    ipc_server_reply_cb *reply,
			    ഘടന ipc_server_reply_data *reply_data)
{
	ഘടന fsmonitor_token_data *token_data = ശൂന്യ;
	ഘടന strbuf response_token = STRBUF_INIT;
	ഘടന strbuf requested_token_id = STRBUF_INIT;
	ഘടന strbuf payload = STRBUF_INIT;
	uint64_t requested_oldest_seq_nr = 0;
	uint64_t total_response_len = 0;
	സ്ഥിരം അക്ഷരം *p;
	സ്ഥിരം ഘടന fsmonitor_batch *batch_head;
	സ്ഥിരം ഘടന fsmonitor_batch *batch;
	ഘടന fsmonitor_batch *remainder = ശൂന്യ;
	intmax_t എണ്ണം = 0, duplicates = 0;
	ഘടന strset shown = STRSET_INIT;
	പൂർണ്ണസംഖ്യ do_trivial = 0;
	പൂർണ്ണസംഖ്യ do_flush = 0;
	പൂർണ്ണസംഖ്യ do_cookie = 0;
	എണ്ണൽ fsmonitor_cookie_item_result cookie_result;

	/*
	 * We expect `command` to be of the form:
	 *
	 * <command> := quit NUL
	 *            | flush NUL
	 *            | <V1-time-since-epoch-ns> NUL
	 *            | <V2-opaque-fsmonitor-token> NUL
	 */

	എങ്കിൽ (!strcmp(command, "quit")) {
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
		തിരികെ SIMPLE_IPC_QUIT;

	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(command, "flush")) {
		/*
		 * Flush all of our cached data and generate a new token
		 * just like if we lost sync with the filesystem.
		 *
		 * Then send a trivial response using the new token.
		 */
		do_flush = 1;
		do_trivial = 1;

	} അല്ലെങ്കിൽ എങ്കിൽ (!skip_prefix(command, "builtin:", &p)) {
		/* assume V1 timestamp or garbage */

		അക്ഷരം *p_end;

		strtoumax(command, &p_end, 10);
		trace_printf_താക്കോൽ(&trace_fsmonitor,
				 ((*p_end) ?
				  "fsmonitor: invalid command line '%s'" :
				  "fsmonitor: unsupported V1 protocol '%s'"),
				 command);
		do_trivial = 1;
		do_cookie = 1;

	} അല്ലെങ്കിൽ {
		/* We have "builtin:*" */
		എങ്കിൽ (fsmonitor_parse_client_token(command, &requested_token_id,
						 &requested_oldest_seq_nr)) {
			trace_printf_താക്കോൽ(&trace_fsmonitor,
					 "fsmonitor: invalid V2 protocol token '%s'",
					 command);
			do_trivial = 1;
			do_cookie = 1;

		} അല്ലെങ്കിൽ {
			/*
			 * We have a V2 valid token:
			 *     "builtin:<token_id>:<seq_nr>"
			 */
			do_cookie = 1;
		}
	}

	pthread_mutex_lock(&state->main_lock);

	എങ്കിൽ (!state->current_token_data)
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
	എങ്കിൽ (do_cookie) {
		cookie_result = with_lock__wait_for_cookie(state);
		എങ്കിൽ (cookie_result != FCIR_SEEN) {
			error(_("fsmonitor: cookie_result '%d' != SEEN"),
			      cookie_result);
			do_trivial = 1;
		}
	}

	എങ്കിൽ (do_flush)
		with_lock__do_force_resync(state);

	/*
	 * We mark the current head of the batch list as "pinned" so
	 * that the listener thread will treat this item as read-only
	 * (and prevent any more paths from being added to it) from
	 * now on.
	 */
	token_data = state->current_token_data;
	batch_head = token_data->batch_head;
	((ഘടന fsmonitor_batch *)batch_head)->pinned_time = time(ശൂന്യ);

	/*
	 * FSMonitor Protocol V2 requires that we send a response header
	 * with a "new current token" and then all of the paths that changed
	 * since the "requested token".  We send the seq_nr of the just-pinned
	 * head batch so that future requests from a client will be relative
	 * to it.
	 */
	with_lock__format_response_token(&response_token,
					 &token_data->token_id, batch_head);

	reply(reply_data, response_token.buf, response_token.len + 1);
	total_response_len += response_token.len + 1;

	trace2_data_string("fsmonitor", the_repository, "response/token",
			   response_token.buf);
	trace_printf_താക്കോൽ(&trace_fsmonitor, "response token: %s",
			 response_token.buf);

	എങ്കിൽ (!do_trivial) {
		എങ്കിൽ (strcmp(requested_token_id.buf, token_data->token_id.buf)) {
			/*
			 * The client last spoke to a different daemon
			 * instance -OR- the daemon had to resync with
			 * the filesystem (and lost events), so reject.
			 */
			trace2_data_string("fsmonitor", the_repository,
					   "response/token", "different");
			do_trivial = 1;

		} അല്ലെങ്കിൽ എങ്കിൽ (requested_oldest_seq_nr <
			   token_data->batch_tail->batch_seq_nr) {
			/*
			 * The client wants older events than we have for
			 * this token_id.  This means that the end of our
			 * batch list was truncated and we cannot give the
			 * client a complete snapshot relative to their
			 * request.
			 */
			trace_printf_താക്കോൽ(&trace_fsmonitor,
					 "client requested truncated data");
			do_trivial = 1;
		}
	}

	എങ്കിൽ (do_trivial) {
		pthread_mutex_unlock(&state->main_lock);

		reply(reply_data, "/", 2);

		trace2_data_intmax("fsmonitor", the_repository,
				   "response/trivial", 1);

		പോകുക cleanup;
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
	token_data->client_ref_എണ്ണം++;

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
	strset_init_with_options(&shown, ശൂന്യ, 0);
	വേണ്ടി (batch = batch_head;
	     batch && batch->batch_seq_nr > requested_oldest_seq_nr;
	     batch = batch->next) {
		size_t k;

		വേണ്ടി (k = 0; k < batch->nr; k++) {
			സ്ഥിരം അക്ഷരം *s = batch->interned_paths[k];
			size_t s_len;

			എങ്കിൽ (!strset_add(&shown, s))
				duplicates++;
			അല്ലെങ്കിൽ {
				trace_printf_താക്കോൽ(&trace_fsmonitor,
						 "send[%"PRIuMAX"]: %s",
						 എണ്ണം, s);

				/* Each path gets written with a trailing NUL */
				s_len = strlen(s) + 1;

				എങ്കിൽ (payload.len + s_len >=
				    LARGE_PACKET_DATA_MAX) {
					reply(reply_data, payload.buf,
					      payload.len);
					total_response_len += payload.len;
					strbuf_reset(&payload);
				}

				strbuf_add(&payload, s, s_len);
				എണ്ണം++;
			}
		}
	}

	എങ്കിൽ (payload.len) {
		reply(reply_data, payload.buf, payload.len);
		total_response_len += payload.len;
	}

	pthread_mutex_lock(&state->main_lock);

	എങ്കിൽ (token_data->client_ref_എണ്ണം > 0)
		token_data->client_ref_എണ്ണം--;

	എങ്കിൽ (token_data->client_ref_എണ്ണം == 0) {
		എങ്കിൽ (token_data != state->current_token_data) {
			/*
			 * The listener thread did a token-reset while we were
			 * walking the batch list.  Therefore, this token is
			 * stale and can be discarded completely.  If we are
			 * the last reader thread using this token, we own
			 * that work.
			 */
			fsmonitor_free_token_data(token_data);
		} അല്ലെങ്കിൽ എങ്കിൽ (batch) {
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

	എങ്കിൽ (remainder)
		fsmonitor_batch__free_list(remainder);

	trace2_data_intmax("fsmonitor", the_repository, "response/length", total_response_len);
	trace2_data_intmax("fsmonitor", the_repository, "response/count/files", എണ്ണം);
	trace2_data_intmax("fsmonitor", the_repository, "response/count/duplicates", duplicates);

cleanup:
	strset_clear(&shown);
	strbuf_release(&response_token);
	strbuf_release(&requested_token_id);
	strbuf_release(&payload);

	തിരികെ 0;
}

സ്ഥിതം ipc_server_application_cb handle_client;

സ്ഥിതം പൂർണ്ണസംഖ്യ handle_client(ശൂന്യം *data,
			 സ്ഥിരം അക്ഷരം *command, size_t command_len,
			 ipc_server_reply_cb *reply,
			 ഘടന ipc_server_reply_data *reply_data)
{
	ഘടന fsmonitor_daemon_state *state = data;
	പൂർണ്ണസംഖ്യ result;

	/*
	 * The Simple IPC API now supports {char*, len} arguments, but
	 * FSMonitor always uses proper null-terminated strings, so
	 * we can ignore the command_len argument.  (Trust, but verify.)
	 */
	എങ്കിൽ (command_len != strlen(command))
		BUG("FSMonitor assumes text messages");

	trace_printf_താക്കോൽ(&trace_fsmonitor, "requested token: %s", command);

	trace2_region_enter("fsmonitor", "handle_client", the_repository);
	trace2_data_string("fsmonitor", the_repository, "request", command);

	result = do_handle_client(state, command, reply, reply_data);

	trace2_region_leave("fsmonitor", "handle_client", the_repository);

	തിരികെ result;
}

#define FSMONITOR_DIR           "fsmonitor--daemon"
#define FSMONITOR_COOKIE_DIR    "cookies"
#define FSMONITOR_COOKIE_PREFIX (FSMONITOR_DIR "/" FSMONITOR_COOKIE_DIR "/")

എണ്ണൽ fsmonitor_path_type fsmonitor_classify_path_workdir_relative(
	സ്ഥിരം അക്ഷരം *rel)
{
	എങ്കിൽ (fspathncmp(rel, ".git", 4))
		തിരികെ IS_WORKDIR_PATH;
	rel += 4;

	എങ്കിൽ (!*rel)
		തിരികെ IS_DOT_GIT;
	എങ്കിൽ (*rel != '/')
		തിരികെ IS_WORKDIR_PATH; /* e.g. .gitignore */
	rel++;

	എങ്കിൽ (!fspathncmp(rel, FSMONITOR_COOKIE_PREFIX,
			strlen(FSMONITOR_COOKIE_PREFIX)))
		തിരികെ IS_INSIDE_DOT_GIT_WITH_COOKIE_PREFIX;

	തിരികെ IS_INSIDE_DOT_GIT;
}

എണ്ണൽ fsmonitor_path_type fsmonitor_classify_path_gitdir_relative(
	സ്ഥിരം അക്ഷരം *rel)
{
	എങ്കിൽ (!fspathncmp(rel, FSMONITOR_COOKIE_PREFIX,
			strlen(FSMONITOR_COOKIE_PREFIX)))
		തിരികെ IS_INSIDE_GITDIR_WITH_COOKIE_PREFIX;

	തിരികെ IS_INSIDE_GITDIR;
}

സ്ഥിതം എണ്ണൽ fsmonitor_path_type try_classify_workdir_abs_path(
	ഘടന fsmonitor_daemon_state *state,
	സ്ഥിരം അക്ഷരം *path)
{
	സ്ഥിരം അക്ഷരം *rel;

	എങ്കിൽ (fspathncmp(path, state->path_worktree_watch.buf,
		       state->path_worktree_watch.len))
		തിരികെ IS_OUTSIDE_CONE;

	rel = path + state->path_worktree_watch.len;

	എങ്കിൽ (!*rel)
		തിരികെ IS_WORKDIR_PATH; /* it is the root dir exactly */
	എങ്കിൽ (*rel != '/')
		തിരികെ IS_OUTSIDE_CONE;
	rel++;

	തിരികെ fsmonitor_classify_path_workdir_relative(rel);
}

എണ്ണൽ fsmonitor_path_type fsmonitor_classify_path_absolute(
	ഘടന fsmonitor_daemon_state *state,
	സ്ഥിരം അക്ഷരം *path)
{
	സ്ഥിരം അക്ഷരം *rel;
	എണ്ണൽ fsmonitor_path_type t;

	t = try_classify_workdir_abs_path(state, path);
	എങ്കിൽ (state->nr_paths_watching == 1)
		തിരികെ t;
	എങ്കിൽ (t != IS_OUTSIDE_CONE)
		തിരികെ t;

	എങ്കിൽ (fspathncmp(path, state->path_gitdir_watch.buf,
		       state->path_gitdir_watch.len))
		തിരികെ IS_OUTSIDE_CONE;

	rel = path + state->path_gitdir_watch.len;

	എങ്കിൽ (!*rel)
		തിരികെ IS_GITDIR; /* it is the <gitdir> exactly */
	എങ്കിൽ (*rel != '/')
		തിരികെ IS_OUTSIDE_CONE;
	rel++;

	തിരികെ fsmonitor_classify_path_gitdir_relative(rel);
}

/*
 * We try to combine small batches at the front of the batch-list to avoid
 * having a long list.  This hopefully makes it a little easier when we want
 * to truncate and maintain the list.  However, we don't want the paths array
 * to just keep growing and growing with realloc, so we insert an arbitrary
 * limit.
 */
#define MY_COMBINE_LIMIT (1024)

ശൂന്യം fsmonitor_publish(ഘടന fsmonitor_daemon_state *state,
		       ഘടന fsmonitor_batch *batch,
		       സ്ഥിരം ഘടന string_list *cookie_names)
{
	എങ്കിൽ (!batch && !cookie_names->nr)
		തിരികെ;

	pthread_mutex_lock(&state->main_lock);

	എങ്കിൽ (batch) {
		ഘടന fsmonitor_batch *head;

		head = state->current_token_data->batch_head;
		എങ്കിൽ (!head) {
			BUG("token does not have batch");
		} അല്ലെങ്കിൽ എങ്കിൽ (head->pinned_time) {
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
			batch->batch_seq_nr = head->batch_seq_nr + 1;
			batch->next = head;
			state->current_token_data->batch_head = batch;
		} അല്ലെങ്കിൽ എങ്കിൽ (!head->batch_seq_nr) {
			/*
			 * Batch 0 is unpinned.  See the note in
			 * `fsmonitor_new_token_data()` about why we
			 * don't need to accumulate these paths.
			 */
			fsmonitor_batch__free_list(batch);
		} അല്ലെങ്കിൽ എങ്കിൽ (head->nr + batch->nr > MY_COMBINE_LIMIT) {
			/*
			 * The head batch in the list has never been
			 * transmitted to a client, but folding the
			 * contents of the new batch onto it would
			 * exceed our arbitrary limit, so just prepend
			 * the new batch onto the list.
			 */
			batch->batch_seq_nr = head->batch_seq_nr + 1;
			batch->next = head;
			state->current_token_data->batch_head = batch;
		} അല്ലെങ്കിൽ {
			/*
			 * We are free to add the paths in the given
			 * batch onto the end of the current head batch.
			 */
			fsmonitor_batch__combine(head, batch);
			fsmonitor_batch__free_list(batch);
		}
	}

	എങ്കിൽ (cookie_names->nr)
		with_lock__mark_cookies_seen(state, cookie_names);

	pthread_mutex_unlock(&state->main_lock);
}

സ്ഥിതം ശൂന്യം *fsm_health__thread_proc(ശൂന്യം *_state)
{
	ഘടന fsmonitor_daemon_state *state = _state;

	trace2_thread_start("fsm-health");

	fsm_health__loop(state);

	trace2_thread_exit();
	തിരികെ ശൂന്യ;
}

സ്ഥിതം ശൂന്യം *fsm_listen__thread_proc(ശൂന്യം *_state)
{
	ഘടന fsmonitor_daemon_state *state = _state;

	trace2_thread_start("fsm-listen");

	trace_printf_താക്കോൽ(&trace_fsmonitor, "Watching: worktree '%s'",
			 state->path_worktree_watch.buf);
	എങ്കിൽ (state->nr_paths_watching > 1)
		trace_printf_താക്കോൽ(&trace_fsmonitor, "Watching: gitdir '%s'",
				 state->path_gitdir_watch.buf);

	fsm_listen__loop(state);

	pthread_mutex_lock(&state->main_lock);
	എങ്കിൽ (state->current_token_data &&
	    state->current_token_data->client_ref_എണ്ണം == 0)
		fsmonitor_free_token_data(state->current_token_data);
	state->current_token_data = ശൂന്യ;
	pthread_mutex_unlock(&state->main_lock);

	trace2_thread_exit();
	തിരികെ ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fsmonitor_run_daemon_1(ഘടന fsmonitor_daemon_state *state)
{
	ഘടന ipc_server_opts ipc_opts = {
		.nr_threads = fsmonitor__ipc_threads,

		/*
		 * We know that there are no other active threads yet,
		 * so we can let the IPC layer temporarily chdir() if
		 * it needs to when creating the server side of the
		 * Unix domain socket.
		 */
		.uds_disallow_chdir = 0
	};
	പൂർണ്ണസംഖ്യ health_started = 0;
	പൂർണ്ണസംഖ്യ listener_started = 0;
	പൂർണ്ണസംഖ്യ err = 0;

	/*
	 * Start the IPC thread pool before the we've started the file
	 * system event listener thread so that we have the IPC handle
	 * before we need it.
	 */
	എങ്കിൽ (ipc_server_init_async(&state->ipc_server_data,
				  state->path_ipc.buf, &ipc_opts,
				  handle_client, state))
		തിരികെ error_errno(
			_("could not start IPC thread pool on '%s'"),
			state->path_ipc.buf);

	/*
	 * Start the fsmonitor listener thread to collect filesystem
	 * events.
	 */
	എങ്കിൽ (pthread_create(&state->listener_thread, ശൂന്യ,
			   fsm_listen__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_data);
		err = error(_("could not start fsmonitor listener thread"));
		പോകുക cleanup;
	}
	listener_started = 1;

	/*
	 * Start the health thread to watch over our process.
	 */
	എങ്കിൽ (pthread_create(&state->health_thread, ശൂന്യ,
			   fsm_health__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_data);
		err = error(_("could not start fsmonitor health thread"));
		പോകുക cleanup;
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
	ipc_server_await(state->ipc_server_data);

	/*
	 * The fsmonitor listener thread may have received a shutdown
	 * event from the IPC thread pool, but it doesn't hurt to tell
	 * it again.  And wait for it to shutdown.
	 */
	എങ്കിൽ (listener_started) {
		fsm_listen__stop_async(state);
		pthread_join(state->listener_thread, ശൂന്യ);
	}

	എങ്കിൽ (health_started) {
		fsm_health__stop_async(state);
		pthread_join(state->health_thread, ശൂന്യ);
	}

	എങ്കിൽ (err)
		തിരികെ err;
	എങ്കിൽ (state->listen_error_code)
		തിരികെ state->listen_error_code;
	എങ്കിൽ (state->health_error_code)
		തിരികെ state->health_error_code;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fsmonitor_run_daemon(ശൂന്യം)
{
	ഘടന fsmonitor_daemon_state state;
	സ്ഥിരം അക്ഷരം *home;
	പൂർണ്ണസംഖ്യ err;

	memset(&state, 0, വലുപ്പം(state));

	hashmap_init(&state.cookies, cookies_cmp, ശൂന്യ, 0);
	pthread_mutex_init(&state.main_lock, ശൂന്യ);
	pthread_cond_init(&state.cookies_cond, ശൂന്യ);
	state.listen_error_code = 0;
	state.health_error_code = 0;
	state.current_token_data = fsmonitor_new_token_data();

	/* Prepare to (recursively) watch the <worktree-root> directory. */
	strbuf_init(&state.path_worktree_watch, 0);
	strbuf_addstr(&state.path_worktree_watch,
		      absolute_path(repo_get_work_വൃക്ഷം(the_repository)));
	state.nr_paths_watching = 1;

	strbuf_init(&state.alias.alias, 0);
	strbuf_init(&state.alias.points_to, 0);
	എങ്കിൽ ((err = fsmonitor__get_alias(state.path_worktree_watch.buf, &state.alias)))
		പോകുക done;

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
	എങ്കിൽ (!is_directory(state.path_gitdir_watch.buf)) {
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
	എങ്കിൽ (fsm_listen__ctor(&state)) {
		err = error(_("could not initialize listener thread"));
		പോകുക done;
	}

	എങ്കിൽ (fsm_health__ctor(&state)) {
		err = error(_("could not initialize health thread"));
		പോകുക done;
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
	എങ്കിൽ (home && *home && chdir(home))
		die_errno(_("could not cd home '%s'"), home);

	err = fsmonitor_run_daemon_1(&state);

done:
	fsmonitor_free_token_data(state.current_token_data);
	state.current_token_data = ശൂന്യ;
	pthread_cond_destroy(&state.cookies_cond);
	pthread_mutex_destroy(&state.main_lock);
	{
		ഘടന hashmap_iter iter;
		ഘടന fsmonitor_cookie_item *cookie;

		hashmap_for_each_entry(&state.cookies, &iter, cookie, entry)
			സ്വതന്ത്രം(cookie->name);
		hashmap_clear_and_free(&state.cookies,
				       ഘടന fsmonitor_cookie_item, entry);
	}
	fsm_listen__dtor(&state);
	fsm_health__dtor(&state);

	ipc_server_free(state.ipc_server_data);

	strbuf_release(&state.path_worktree_watch);
	strbuf_release(&state.path_gitdir_watch);
	strbuf_release(&state.path_cookie_prefix);
	strbuf_release(&state.path_ipc);
	strbuf_release(&state.alias.alias);
	strbuf_release(&state.alias.points_to);

	തിരികെ err;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ try_to_run_foreground_daemon(പൂർണ്ണസംഖ്യ detach_console)
{
	/*
	 * Technically, we don't need to probe for an existing daemon
	 * process, since we could just call `fsmonitor_run_daemon()`
	 * and let it fail if the pipe/socket is busy.
	 *
	 * However, this method gives us a nicer error message for a
	 * common error case.
	 */
	എങ്കിൽ (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	എങ്കിൽ (fsmonitor__announce_startup) {
		fprintf(stderr, _("running fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	എങ്കിൽ (detach_console) {
#ifdef GIT_WINDOWS_NATIVE
		FreeConsole();
#അല്ലെങ്കിൽ
		/*
		 * Create a new session so that the daemon is detached
		 * from the parent's process group.  This prevents
		 * shells with job control (e.g. bash with "set -m")
		 * from waiting on the daemon when they wait for a
		 * foreground command that implicitly spawned it.
		 */
		എങ്കിൽ (setsid() == -1)
			warning_errno(_("setsid failed"));
#endif
	}

	തിരികെ !!fsmonitor_run_daemon();
}

സ്ഥിതം start_bg_wait_cb bg_wait_cb;

സ്ഥിതം പൂർണ്ണസംഖ്യ bg_wait_cb(സ്ഥിരം ഘടന child_process *cp UNUSED,
		      ശൂന്യം *cb_data UNUSED)
{
	എണ്ണൽ ipc_active_state s = fsmonitor_ipc__get_state();

	തിരഞ്ഞെടുപ്പ് (s) {
	അവസ്ഥ IPC_STATE__LISTENING:
		/* child is "ready" */
		തിരികെ 0;

	അവസ്ഥ IPC_STATE__NOT_LISTENING:
	അവസ്ഥ IPC_STATE__PATH_NOT_FOUND:
		/* give child more time */
		തിരികെ 1;

	സ്ഥിരസ്ഥിതി:
	അവസ്ഥ IPC_STATE__INVALID_PATH:
	അവസ്ഥ IPC_STATE__OTHER_ERROR:
		/* all the time in world won't help */
		തിരികെ -1;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ try_to_start_background_daemon(ശൂന്യം)
{
	ഘടന child_process cp = CHILD_PROCESS_INIT;
	എണ്ണൽ start_bg_result sbgr;

	/*
	 * Before we try to create a background daemon process, see
	 * if a daemon process is already listening.  This makes it
	 * easier for us to report an already-listening error to the
	 * console, since our spawn/daemon can only report the success
	 * of creating the background process (and not whether it
	 * immediately exited).
	 */
	എങ്കിൽ (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	എങ്കിൽ (fsmonitor__announce_startup) {
		fprintf(stderr, _("starting fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	cp.git_cmd = 1;

	strvec_തള്ളുക(&cp.args, "fsmonitor--daemon");
	strvec_തള്ളുക(&cp.args, "run");
	strvec_തള്ളുക(&cp.args, "--detach");
	strvec_pushf(&cp.args, "--ipc-threads=%d", fsmonitor__ipc_threads);

	cp.no_stdin = 1;
	cp.no_stdout = 1;
	cp.no_stderr = 1;
	cp.close_fd_above_stderr = 1;

	sbgr = start_bg_command(&cp, bg_wait_cb, ശൂന്യ,
				fsmonitor__start_timeout_sec);

	തിരഞ്ഞെടുപ്പ് (sbgr) {
	അവസ്ഥ SBGR_READY:
		തിരികെ 0;

	സ്ഥിരസ്ഥിതി:
	അവസ്ഥ SBGR_ERROR:
	അവസ്ഥ SBGR_CB_ERROR:
		തിരികെ error(_("daemon failed to start"));

	അവസ്ഥ SBGR_TIMEOUT:
		തിരികെ error(_("daemon not online yet"));

	അവസ്ഥ SBGR_DIED:
		തിരികെ error(_("daemon terminated"));
	}
}

പൂർണ്ണസംഖ്യ cmd_fsmonitor__daemon(പൂർണ്ണസംഖ്യ argc,
			  സ്ഥിരം അക്ഷരം **argv,
			  സ്ഥിരം അക്ഷരം *prefix,
			  ഘടന repository *repo UNUSED)
{
	സ്ഥിരം അക്ഷരം *subcmd;
	എണ്ണൽ fsmonitor_reason reason;
	പൂർണ്ണസംഖ്യ detach_console = 0;

	ഘടന option options[] = {
		OPT_BOOL(0, "detach", &detach_console, N_("detach from console")),
		OPT_INTEGER(0, "ipc-threads",
			    &fsmonitor__ipc_threads,
			    N_("use <n> ipc worker threads")),
		OPT_INTEGER(0, "start-timeout",
			    &fsmonitor__start_timeout_sec,
			    N_("max seconds to wait for background daemon startup")),

		OPT_END()
	};

	repo_config(the_repository, fsmonitor_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, options,
			     builtin_fsmonitor__daemon_usage, 0);
	എങ്കിൽ (argc != 1)
		usage_with_options(builtin_fsmonitor__daemon_usage, options);
	subcmd = argv[0];

	എങ്കിൽ (fsmonitor__ipc_threads < 1)
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
	എങ്കിൽ (reason > FSMONITOR_REASON_OK)
		die("%s",
		    fsm_settings_get_incompatible_സന്ദേശം(the_repository,
						       reason));

	എങ്കിൽ (!strcmp(subcmd, "start"))
		തിരികെ !!try_to_start_background_daemon();

	എങ്കിൽ (!strcmp(subcmd, "run"))
		തിരികെ !!try_to_run_foreground_daemon(detach_console);

	എങ്കിൽ (!strcmp(subcmd, "stop"))
		തിരികെ !!do_as_client__send_stop();

	എങ്കിൽ (!strcmp(subcmd, "status"))
		തിരികെ !!do_as_client__status();

	die(_("Unhandled subcommand '%s'"), subcmd);
}

#അല്ലെങ്കിൽ
പൂർണ്ണസംഖ്യ cmd_fsmonitor__daemon(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix UNUSED, ഘടന repository *repo UNUSED)
{
	ഘടന option options[] = {
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_fsmonitor__daemon_usage, options);

	die(_("fsmonitor--daemon not supported on this platform"));
}
#endif
