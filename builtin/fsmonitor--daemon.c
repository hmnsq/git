#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "fsmonitor-ll.h"
#ଅନ୍ତର୍ଭାଵ "fsmonitor-ipc.h"
#ଅନ୍ତର୍ଭାଵ "fsmonitor-settings.h"
#ଅନ୍ତର୍ଭାଵ "compat/fsmonitor/fsm-health.h"
#ଅନ୍ତର୍ଭାଵ "compat/fsmonitor/fsm-listen.h"
#ଅନ୍ତର୍ଭାଵ "fsmonitor--daemon.h"

#ଅନ୍ତର୍ଭାଵ "simple-ipc.h"
#ଅନ୍ତର୍ଭାଵ "strmap.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "trace.h"
#ଅନ୍ତର୍ଭାଵ "trace2.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_fsmonitor__daemon_usage[] = {
	N_("git fsmonitor--daemon start [<options>]"),
	N_("git fsmonitor--daemon run [<options>]"),
	"git fsmonitor--daemon stop",
	"git fsmonitor--daemon status",
	ଖାଲି
};

#ifdef HAVE_FSMONITOR_DAEMON_BACKEND
/*
 * Global state loaded from config.
 */
#define FSMONITOR__IPC_THREADS "fsmonitor.ipcthreads"
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsmonitor__ipc_threads = 8;

#define FSMONITOR__START_TIMEOUT "fsmonitor.starttimeout"
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsmonitor__start_timeout_sec = 60;

#define FSMONITOR__ANNOUNCE_STARTUP "fsmonitor.announcestartup"
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsmonitor__announce_startup = 0;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsmonitor_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			    ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(var, FSMONITOR__IPC_THREADS)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = git_config_int(var, ମୂଲ୍ୟ, ctx->kvi);
		ଯଦି (i < 1)
			ଫେରନ୍ତୁ error(_("value of '%s' out of range: %d"),
				     FSMONITOR__IPC_THREADS, i);
		fsmonitor__ipc_threads = i;
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(var, FSMONITOR__START_TIMEOUT)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = git_config_int(var, ମୂଲ୍ୟ, ctx->kvi);
		ଯଦି (i < 0)
			ଫେରନ୍ତୁ error(_("value of '%s' out of range: %d"),
				     FSMONITOR__START_TIMEOUT, i);
		fsmonitor__start_timeout_sec = i;
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(var, FSMONITOR__ANNOUNCE_STARTUP)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_bool;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = git_config_bool_or_int(var, ମୂଲ୍ୟ, ctx->kvi, &is_bool);
		ଯଦି (i < 0)
			ଫେରନ୍ତୁ error(_("value of '%s' not bool or int: %d"),
				     var, i);
		fsmonitor__announce_startup = i;
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

/*
 * Acting as a CLIENT.
 *
 * Send a "quit" command to the `git-fsmonitor--daemon` (if running)
 * and wait for it to shutdown.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_as_client__send_stop(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf answer = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_wait_ms = 30000;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା elapsed_ms = 0;

	ret = fsmonitor_ipc__send_command("quit", &answer);

	/* The quit command does not return any response data. */
	strbuf_release(&answer);

	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	trace2_region_enter("fsm_client", "polling-for-daemon-exit", ଖାଲି);
	ଯେତେବେଳେ (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING) {
		ଯଦି (elapsed_ms >= max_wait_ms) {
			trace2_region_leave("fsm_client",
					    "polling-for-daemon-exit", ଖାଲି);
			ଫେରନ୍ତୁ error(_("daemon did not stop within %d seconds"),
				     max_wait_ms / 1000);
		}
		sleep_millisec(50);
		elapsed_ms += 50;
	}
	trace2_region_leave("fsm_client", "polling-for-daemon-exit", ଖାଲି);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_as_client__status(ଶୂନ୍ୟ)
{
	ଗଣନା ipc_active_state state = fsmonitor_ipc__get_state();

	ବାଛ (state) {
	ସ୍ଥିତି IPC_STATE__LISTENING:
		printf(_("fsmonitor-daemon is watching '%s'\n"),
		       the_repository->worktree);
		ଫେରନ୍ତୁ 0;

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		printf(_("fsmonitor-daemon is not watching '%s'\n"),
		       the_repository->worktree);
		ଫେରନ୍ତୁ 1;
	}
}

ଗଣନା fsmonitor_cookie_item_result {
	FCIR_ERROR = -1, /* could not create cookie file ? */
	FCIR_INIT,
	FCIR_SEEN,
	FCIR_ABORT,
};

ସଂରଚନା fsmonitor_cookie_item {
	ସଂରଚନା hashmap_entry entry;
	ଅକ୍ଷର *name;
	ଗଣନା fsmonitor_cookie_item_result result;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cookies_cmp(ଅଚଳ ଶୂନ୍ୟ *data UNUSED,
		       ଅଚଳ ସଂରଚନା hashmap_entry *he1,
		       ଅଚଳ ସଂରଚନା hashmap_entry *he2, ଅଚଳ ଶୂନ୍ୟ *keydata)
{
	ଅଚଳ ସଂରଚନା fsmonitor_cookie_item *a =
		container_of(he1, ଅଚଳ ସଂରଚନା fsmonitor_cookie_item, entry);
	ଅଚଳ ସଂରଚନା fsmonitor_cookie_item *b =
		container_of(he2, ଅଚଳ ସଂରଚନା fsmonitor_cookie_item, entry);

	ଫେରନ୍ତୁ strcmp(a->name, keydata ? keydata : b->name);
}

ସ୍ଥିର ଗଣନା fsmonitor_cookie_item_result with_lock__wait_for_cookie(
	ସଂରଚନା fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	ସଂରଚନା fsmonitor_cookie_item *cookie;
	ସଂରଚନା strbuf cookie_pathname = STRBUF_INIT;
	ସଂରଚନା strbuf cookie_filename = STRBUF_INIT;
	ଗଣନା fsmonitor_cookie_item_result result;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା my_cookie_seq;

	CALLOC_ARRAY(cookie, 1);

	my_cookie_seq = state->cookie_seq++;

	strbuf_addf(&cookie_filename, "%i-%i", getpid(), my_cookie_seq);

	strbuf_addbuf(&cookie_pathname, &state->path_cookie_prefix);
	strbuf_addbuf(&cookie_pathname, &cookie_filename);

	cookie->name = strbuf_detach(&cookie_filename, ଖାଲି);
	cookie->result = FCIR_INIT;
	hashmap_entry_init(&cookie->entry, strhash(cookie->name));

	hashmap_add(&state->cookies, &cookie->entry);

	trace_printf_ଚାବି(&trace_fsmonitor, "cookie-wait: '%s' '%s'",
			 cookie->name, cookie_pathname.buf);

	/*
	 * Create the cookie file on disk and then wait for a notification
	 * that the listener thread has seen it.
	 */
	fd = open(cookie_pathname.buf, O_WRONLY | O_CREAT | O_EXCL, 0600);
	ଯଦି (fd < 0) {
		error_errno(_("could not create fsmonitor cookie '%s'"),
			    cookie->name);

		cookie->result = FCIR_ERROR;
		ଯାଅ done;
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
		ସଂରଚନା timeval now;
		ସଂରଚନା timespec ts;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;

		gettimeofday(&now, ଖାଲି);
		ts.tv_sec = now.tv_sec + 1;
		ts.tv_nsec = now.tv_usec * 1000;

		ଯେତେବେଳେ (cookie->result == FCIR_INIT && !err)
			err = pthread_cond_timedwait(&state->cookies_cond,
						     &state->main_lock,
						     &ts);
		ଯଦି (err == ETIMEDOUT && cookie->result == FCIR_INIT) {
			trace_printf_ଚାବି(&trace_fsmonitor,
					 "cookie_wait timed out");
			cookie->result = FCIR_ERROR;
		}
	}

done:
	hashmap_remove(&state->cookies, &cookie->entry, ଖାଲି);

	result = cookie->result;

	ମୁକ୍ତ(cookie->name);
	ମୁକ୍ତ(cookie);
	strbuf_release(&cookie_pathname);

	ଫେରନ୍ତୁ result;
}

/*
 * Mark these cookies as _SEEN and wake up the corresponding client threads.
 */
ସ୍ଥିର ଶୂନ୍ୟ with_lock__mark_cookies_seen(ସଂରଚନା fsmonitor_daemon_state *state,
					 ଅଚଳ ସଂରଚନା string_list *cookie_names)
{
	/* assert current thread holding state->main_lock */

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା k;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_seen = 0;

	ପାଇଁ (k = 0; k < cookie_names->nr; k++) {
		ସଂରଚନା fsmonitor_cookie_item ଚାବି;
		ସଂରଚନା fsmonitor_cookie_item *cookie;

		ଚାବି.name = cookie_names->ବସ୍ତୁଗୁଡ଼ିକ[k].string;
		hashmap_entry_init(&ଚାବି.entry, strhash(ଚାବି.name));

		cookie = hashmap_get_entry(&state->cookies, &ଚାବି, entry, ଖାଲି);
		ଯଦି (cookie) {
			trace_printf_ଚାବି(&trace_fsmonitor, "cookie-seen: '%s'",
					 cookie->name);
			cookie->result = FCIR_SEEN;
			nr_seen++;
		}
	}

	ଯଦି (nr_seen)
		pthread_cond_broadcast(&state->cookies_cond);
}

/*
 * Set _ABORT on all pending cookies and wake up all client threads.
 */
ସ୍ଥିର ଶୂନ୍ୟ with_lock__abort_all_cookies(ସଂରଚନା fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	ସଂରଚନା hashmap_iter iter;
	ସଂରଚନା fsmonitor_cookie_item *cookie;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_aborted = 0;

	hashmap_for_each_entry(&state->cookies, &iter, cookie, entry) {
		trace_printf_ଚାବି(&trace_fsmonitor, "cookie-abort: '%s'",
				 cookie->name);
		cookie->result = FCIR_ABORT;
		nr_aborted++;
	}

	ଯଦି (nr_aborted)
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
ସଂରଚନା fsmonitor_token_data {
	ସଂରଚନା strbuf token_id;
	ସଂରଚନା fsmonitor_batch *batch_head;
	ସଂରଚନା fsmonitor_batch *batch_tail;
	uint64_t client_ref_ଗଣନା;
};

ସଂରଚନା fsmonitor_batch {
	ସଂରଚନା fsmonitor_batch *next;
	uint64_t batch_seq_nr;
	ଅଚଳ ଅକ୍ଷର **interned_paths;
	size_t nr, alloc;
	time_t pinned_time;
};

ସ୍ଥିର ସଂରଚନା fsmonitor_token_data *fsmonitor_new_token_data(ଶୂନ୍ୟ)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା test_env_ମୂଲ୍ୟ = -1;
	ସ୍ଥିର uint64_t flush_ଗଣନା = 0;
	ସଂରଚନା fsmonitor_token_data *token;
	ସଂରଚନା fsmonitor_batch *batch;

	CALLOC_ARRAY(token, 1);
	batch = fsmonitor_batch__new();

	strbuf_init(&token->token_id, 0);
	token->batch_head = batch;
	token->batch_tail = batch;
	token->client_ref_ଗଣନା = 0;

	ଯଦି (test_env_ମୂଲ୍ୟ < 0)
		test_env_ମୂଲ୍ୟ = git_env_bool("GIT_TEST_FSMONITOR_TOKEN", 0);

	ଯଦି (!test_env_ମୂଲ୍ୟ) {
		ସଂରଚନା timeval tv;
		ସଂରଚନା tm tm;
		time_t secs;

		gettimeofday(&tv, ଖାଲି);
		secs = tv.tv_sec;
		gmtime_r(&secs, &tm);

		strbuf_addf(&token->token_id,
			    "%"PRIu64".%d.%4d%02d%02dT%02d%02d%02d.%06ldZ",
			    flush_ଗଣନା++,
			    getpid(),
			    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			    tm.tm_hour, tm.tm_min, tm.tm_sec,
			    (long)tv.tv_usec);
	} ଅନ୍ୟଥା {
		strbuf_addf(&token->token_id, "test_%08x", test_env_ମୂଲ୍ୟ++);
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
	ଯଦି (test_env_ମୂଲ୍ୟ)
		batch->pinned_time = time(ଖାଲି);

	ଫେରନ୍ତୁ token;
}

ସଂରଚନା fsmonitor_batch *fsmonitor_batch__new(ଶୂନ୍ୟ)
{
	ସଂରଚନା fsmonitor_batch *batch;

	CALLOC_ARRAY(batch, 1);

	ଫେରନ୍ତୁ batch;
}

ଶୂନ୍ୟ fsmonitor_batch__free_list(ସଂରଚନା fsmonitor_batch *batch)
{
	ଯେତେବେଳେ (batch) {
		ସଂରଚନା fsmonitor_batch *next = batch->next;

		/*
		 * The actual strings within the array of this batch
		 * are interned, so we don't own them.  We only own
		 * the array.
		 */
		ମୁକ୍ତ(batch->interned_paths);
		ମୁକ୍ତ(batch);

		batch = next;
	}
}

ଶୂନ୍ୟ fsmonitor_batch__add_path(ସଂରଚନା fsmonitor_batch *batch,
			       ଅଚଳ ଅକ୍ଷର *path)
{
	ଅଚଳ ଅକ୍ଷର *interned_path = strintern(path);

	trace_printf_ଚାବି(&trace_fsmonitor, "event: %s", interned_path);

	ALLOC_GROW(batch->interned_paths, batch->nr + 1, batch->alloc);
	batch->interned_paths[batch->nr++] = interned_path;
}

ସ୍ଥିର ଶୂନ୍ୟ fsmonitor_batch__combine(ସଂରଚନା fsmonitor_batch *batch_dest,
				     ଅଚଳ ସଂରଚନା fsmonitor_batch *batch_src)
{
	size_t k;

	ALLOC_GROW(batch_dest->interned_paths,
		   batch_dest->nr + batch_src->nr + 1,
		   batch_dest->alloc);

	ପାଇଁ (k = 0; k < batch_src->nr; k++)
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

ସ୍ଥିର ସଂରଚନା fsmonitor_batch *with_lock__truncate_old_batches(
	ସଂରଚନା fsmonitor_daemon_state *state,
	ଅଚଳ ସଂରଚନା fsmonitor_batch *batch_marker)
{
	/* assert current thread holding state->main_lock */

	ଅଚଳ ସଂରଚନା fsmonitor_batch *batch;
	ସଂରଚନା fsmonitor_batch *remainder;

	ଯଦି (!batch_marker)
		ଫେରନ୍ତୁ ଖାଲି;

	trace_printf_ଚାବି(&trace_fsmonitor, "Truncate: mark (%"PRIu64",%"PRIu64")",
			 batch_marker->batch_seq_nr,
			 (uint64_t)batch_marker->pinned_time);

	ପାଇଁ (batch = batch_marker; batch; batch = batch->next) {
		time_t t;

		ଯଦି (!batch->pinned_time) /* an overflow batch */
			ଜାରି;

		t = batch->pinned_time + MY_TIME_DELAY_SECONDS;
		ଯଦି (t > batch_marker->pinned_time) /* too close to marker */
			ଜାରି;

		ଯାଅ truncate_past_here;
	}

	ଫେରନ୍ତୁ ଖାଲି;

truncate_past_here:
	state->current_token_data->batch_tail = (ସଂରଚନା fsmonitor_batch *)batch;

	remainder = ((ସଂରଚନା fsmonitor_batch *)batch)->next;
	((ସଂରଚନା fsmonitor_batch *)batch)->next = ଖାଲି;

	ଫେରନ୍ତୁ remainder;
}

ସ୍ଥିର ଶୂନ୍ୟ fsmonitor_free_token_data(ସଂରଚନା fsmonitor_token_data *token)
{
	ଯଦି (!token)
		ଫେରନ୍ତୁ;

	ପ୍ରତିଜ୍ଞା(token->client_ref_ଗଣନା == 0);

	strbuf_release(&token->token_id);

	fsmonitor_batch__free_list(token->batch_head);

	ମୁକ୍ତ(token);
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
ସ୍ଥିର ଶୂନ୍ୟ with_lock__do_force_resync(ସଂରଚନା fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	ସଂରଚନା fsmonitor_token_data *free_me = ଖାଲି;
	ସଂରଚନା fsmonitor_token_data *new_one = ଖାଲି;

	new_one = fsmonitor_new_token_data();

	ଯଦି (state->current_token_data->client_ref_ଗଣନା == 0)
		free_me = state->current_token_data;
	state->current_token_data = new_one;

	fsmonitor_free_token_data(free_me);

	with_lock__abort_all_cookies(state);
}

ଶୂନ୍ୟ fsmonitor_force_resync(ସଂରଚନା fsmonitor_daemon_state *state)
{
	pthread_mutex_lock(&state->main_lock);
	with_lock__do_force_resync(state);
	pthread_mutex_unlock(&state->main_lock);
}

/*
 * Format an opaque token string to send to the client.
 */
ସ୍ଥିର ଶୂନ୍ୟ with_lock__format_response_token(
	ସଂରଚନା strbuf *response_token,
	ଅଚଳ ସଂରଚନା strbuf *response_token_id,
	ଅଚଳ ସଂରଚନା fsmonitor_batch *batch)
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsmonitor_parse_client_token(ଅଚଳ ଅକ୍ଷର *buf_token,
					ସଂରଚନା strbuf *requested_token_id,
					uint64_t *seq_nr)
{
	ଅଚଳ ଅକ୍ଷର *p;
	ଅକ୍ଷର *p_end;

	strbuf_reset(requested_token_id);
	*seq_nr = 0;

	ଯଦି (!skip_prefix(buf_token, "builtin:", &p))
		ଫେରନ୍ତୁ -1;

	ଯେତେବେଳେ (*p && *p != ':')
		strbuf_addch(requested_token_id, *p++);
	ଯଦି (!*p++)
		ଫେରନ୍ତୁ -1;

	*seq_nr = (uint64_t)strtoumax(p, &p_end, 10);
	ଯଦି (*p_end)
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_handle_client(ସଂରଚନା fsmonitor_daemon_state *state,
			    ଅଚଳ ଅକ୍ଷର *command,
			    ipc_server_reply_cb *reply,
			    ସଂରଚନା ipc_server_reply_data *reply_data)
{
	ସଂରଚନା fsmonitor_token_data *token_data = ଖାଲି;
	ସଂରଚନା strbuf response_token = STRBUF_INIT;
	ସଂରଚନା strbuf requested_token_id = STRBUF_INIT;
	ସଂରଚନା strbuf payload = STRBUF_INIT;
	uint64_t requested_oldest_seq_nr = 0;
	uint64_t total_response_len = 0;
	ଅଚଳ ଅକ୍ଷର *p;
	ଅଚଳ ସଂରଚନା fsmonitor_batch *batch_head;
	ଅଚଳ ସଂରଚନା fsmonitor_batch *batch;
	ସଂରଚନା fsmonitor_batch *remainder = ଖାଲି;
	intmax_t ଗଣନା_2 = 0, duplicates = 0;
	ସଂରଚନା strset shown = STRSET_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_trivial = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_flush = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_cookie = 0;
	ଗଣନା fsmonitor_cookie_item_result cookie_result;

	/*
	 * We expect `command` to be of the form:
	 *
	 * <command> := quit NUL
	 *            | flush NUL
	 *            | <V1-time-since-epoch-ns> NUL
	 *            | <V2-opaque-fsmonitor-token> NUL
	 */

	ଯଦି (!strcmp(command, "quit")) {
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
		ଫେରନ୍ତୁ SIMPLE_IPC_QUIT;

	} ଅନ୍ୟଥା ଯଦି (!strcmp(command, "flush")) {
		/*
		 * Flush all of our cached data and generate a new token
		 * just like if we lost sync with the filesystem.
		 *
		 * Then send a trivial response using the new token.
		 */
		do_flush = 1;
		do_trivial = 1;

	} ଅନ୍ୟଥା ଯଦି (!skip_prefix(command, "builtin:", &p)) {
		/* assume V1 timestamp or garbage */

		ଅକ୍ଷର *p_end;

		strtoumax(command, &p_end, 10);
		trace_printf_ଚାବି(&trace_fsmonitor,
				 ((*p_end) ?
				  "fsmonitor: invalid command line '%s'" :
				  "fsmonitor: unsupported V1 protocol '%s'"),
				 command);
		do_trivial = 1;
		do_cookie = 1;

	} ଅନ୍ୟଥା {
		/* We have "builtin:*" */
		ଯଦି (fsmonitor_parse_client_token(command, &requested_token_id,
						 &requested_oldest_seq_nr)) {
			trace_printf_ଚାବି(&trace_fsmonitor,
					 "fsmonitor: invalid V2 protocol token '%s'",
					 command);
			do_trivial = 1;
			do_cookie = 1;

		} ଅନ୍ୟଥା {
			/*
			 * We have a V2 valid token:
			 *     "builtin:<token_id>:<seq_nr>"
			 */
			do_cookie = 1;
		}
	}

	pthread_mutex_lock(&state->main_lock);

	ଯଦି (!state->current_token_data)
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
	ଯଦି (do_cookie) {
		cookie_result = with_lock__wait_for_cookie(state);
		ଯଦି (cookie_result != FCIR_SEEN) {
			error(_("fsmonitor: cookie_result '%d' != SEEN"),
			      cookie_result);
			do_trivial = 1;
		}
	}

	ଯଦି (do_flush)
		with_lock__do_force_resync(state);

	/*
	 * We mark the current head of the batch list as "pinned" so
	 * that the listener thread will treat this item as read-only
	 * (and prevent any more paths from being added to it) from
	 * now on.
	 */
	token_data = state->current_token_data;
	batch_head = token_data->batch_head;
	((ସଂରଚନା fsmonitor_batch *)batch_head)->pinned_time = time(ଖାଲି);

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
	trace_printf_ଚାବି(&trace_fsmonitor, "response token: %s",
			 response_token.buf);

	ଯଦି (!do_trivial) {
		ଯଦି (strcmp(requested_token_id.buf, token_data->token_id.buf)) {
			/*
			 * The client last spoke to a different daemon
			 * instance -OR- the daemon had to resync with
			 * the filesystem (and lost events), so reject.
			 */
			trace2_data_string("fsmonitor", the_repository,
					   "response/token", "different");
			do_trivial = 1;

		} ଅନ୍ୟଥା ଯଦି (requested_oldest_seq_nr <
			   token_data->batch_tail->batch_seq_nr) {
			/*
			 * The client wants older events than we have for
			 * this token_id.  This means that the end of our
			 * batch list was truncated and we cannot give the
			 * client a complete snapshot relative to their
			 * request.
			 */
			trace_printf_ଚାବି(&trace_fsmonitor,
					 "client requested truncated data");
			do_trivial = 1;
		}
	}

	ଯଦି (do_trivial) {
		pthread_mutex_unlock(&state->main_lock);

		reply(reply_data, "/", 2);

		trace2_data_intmax("fsmonitor", the_repository,
				   "response/trivial", 1);

		ଯାଅ cleanup;
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
	token_data->client_ref_ଗଣନା++;

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
	strset_init_with_options(&shown, ଖାଲି, 0);
	ପାଇଁ (batch = batch_head;
	     batch && batch->batch_seq_nr > requested_oldest_seq_nr;
	     batch = batch->next) {
		size_t k;

		ପାଇଁ (k = 0; k < batch->nr; k++) {
			ଅଚଳ ଅକ୍ଷର *s = batch->interned_paths[k];
			size_t s_len;

			ଯଦି (!strset_add(&shown, s))
				duplicates++;
			ଅନ୍ୟଥା {
				trace_printf_ଚାବି(&trace_fsmonitor,
						 "send[%"PRIuMAX"]: %s",
						 ଗଣନା_2, s);

				/* Each path gets written with a trailing NUL */
				s_len = strlen(s) + 1;

				ଯଦି (payload.len + s_len >=
				    LARGE_PACKET_DATA_MAX) {
					reply(reply_data, payload.buf,
					      payload.len);
					total_response_len += payload.len;
					strbuf_reset(&payload);
				}

				strbuf_add(&payload, s, s_len);
				ଗଣନା_2++;
			}
		}
	}

	ଯଦି (payload.len) {
		reply(reply_data, payload.buf, payload.len);
		total_response_len += payload.len;
	}

	pthread_mutex_lock(&state->main_lock);

	ଯଦି (token_data->client_ref_ଗଣନା > 0)
		token_data->client_ref_ଗଣନା--;

	ଯଦି (token_data->client_ref_ଗଣନା == 0) {
		ଯଦି (token_data != state->current_token_data) {
			/*
			 * The listener thread did a token-reset while we were
			 * walking the batch list.  Therefore, this token is
			 * stale and can be discarded completely.  If we are
			 * the last reader thread using this token, we own
			 * that work.
			 */
			fsmonitor_free_token_data(token_data);
		} ଅନ୍ୟଥା ଯଦି (batch) {
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

	ଯଦି (remainder)
		fsmonitor_batch__free_list(remainder);

	trace2_data_intmax("fsmonitor", the_repository, "response/length", total_response_len);
	trace2_data_intmax("fsmonitor", the_repository, "response/count/files", ଗଣନା_2);
	trace2_data_intmax("fsmonitor", the_repository, "response/count/duplicates", duplicates);

cleanup:
	strset_clear(&shown);
	strbuf_release(&response_token);
	strbuf_release(&requested_token_id);
	strbuf_release(&payload);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ipc_server_application_cb handle_client;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା handle_client(ଶୂନ୍ୟ *data,
			 ଅଚଳ ଅକ୍ଷର *command, size_t command_len,
			 ipc_server_reply_cb *reply,
			 ସଂରଚନା ipc_server_reply_data *reply_data)
{
	ସଂରଚନା fsmonitor_daemon_state *state = data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;

	/*
	 * The Simple IPC API now supports {char*, len} arguments, but
	 * FSMonitor always uses proper null-terminated strings, so
	 * we can ignore the command_len argument.  (Trust, but verify.)
	 */
	ଯଦି (command_len != strlen(command))
		BUG("FSMonitor assumes text messages");

	trace_printf_ଚାବି(&trace_fsmonitor, "requested token: %s", command);

	trace2_region_enter("fsmonitor", "handle_client", the_repository);
	trace2_data_string("fsmonitor", the_repository, "request", command);

	result = do_handle_client(state, command, reply, reply_data);

	trace2_region_leave("fsmonitor", "handle_client", the_repository);

	ଫେରନ୍ତୁ result;
}

#define FSMONITOR_DIR           "fsmonitor--daemon"
#define FSMONITOR_COOKIE_DIR    "cookies"
#define FSMONITOR_COOKIE_PREFIX (FSMONITOR_DIR "/" FSMONITOR_COOKIE_DIR "/")

ଗଣନା fsmonitor_path_type fsmonitor_classify_path_workdir_relative(
	ଅଚଳ ଅକ୍ଷର *rel)
{
	ଯଦି (fspathncmp(rel, ".git", 4))
		ଫେରନ୍ତୁ IS_WORKDIR_PATH;
	rel += 4;

	ଯଦି (!*rel)
		ଫେରନ୍ତୁ IS_DOT_GIT;
	ଯଦି (*rel != '/')
		ଫେରନ୍ତୁ IS_WORKDIR_PATH; /* e.g. .gitignore */
	rel++;

	ଯଦି (!fspathncmp(rel, FSMONITOR_COOKIE_PREFIX,
			strlen(FSMONITOR_COOKIE_PREFIX)))
		ଫେରନ୍ତୁ IS_INSIDE_DOT_GIT_WITH_COOKIE_PREFIX;

	ଫେରନ୍ତୁ IS_INSIDE_DOT_GIT;
}

ଗଣନା fsmonitor_path_type fsmonitor_classify_path_gitdir_relative(
	ଅଚଳ ଅକ୍ଷର *rel)
{
	ଯଦି (!fspathncmp(rel, FSMONITOR_COOKIE_PREFIX,
			strlen(FSMONITOR_COOKIE_PREFIX)))
		ଫେରନ୍ତୁ IS_INSIDE_GITDIR_WITH_COOKIE_PREFIX;

	ଫେରନ୍ତୁ IS_INSIDE_GITDIR;
}

ସ୍ଥିର ଗଣନା fsmonitor_path_type try_classify_workdir_abs_path(
	ସଂରଚନା fsmonitor_daemon_state *state,
	ଅଚଳ ଅକ୍ଷର *path)
{
	ଅଚଳ ଅକ୍ଷର *rel;

	ଯଦି (fspathncmp(path, state->path_worktree_watch.buf,
		       state->path_worktree_watch.len))
		ଫେରନ୍ତୁ IS_OUTSIDE_CONE;

	rel = path + state->path_worktree_watch.len;

	ଯଦି (!*rel)
		ଫେରନ୍ତୁ IS_WORKDIR_PATH; /* it is the root dir exactly */
	ଯଦି (*rel != '/')
		ଫେରନ୍ତୁ IS_OUTSIDE_CONE;
	rel++;

	ଫେରନ୍ତୁ fsmonitor_classify_path_workdir_relative(rel);
}

ଗଣନା fsmonitor_path_type fsmonitor_classify_path_absolute(
	ସଂରଚନା fsmonitor_daemon_state *state,
	ଅଚଳ ଅକ୍ଷର *path)
{
	ଅଚଳ ଅକ୍ଷର *rel;
	ଗଣନା fsmonitor_path_type t;

	t = try_classify_workdir_abs_path(state, path);
	ଯଦି (state->nr_paths_watching == 1)
		ଫେରନ୍ତୁ t;
	ଯଦି (t != IS_OUTSIDE_CONE)
		ଫେରନ୍ତୁ t;

	ଯଦି (fspathncmp(path, state->path_gitdir_watch.buf,
		       state->path_gitdir_watch.len))
		ଫେରନ୍ତୁ IS_OUTSIDE_CONE;

	rel = path + state->path_gitdir_watch.len;

	ଯଦି (!*rel)
		ଫେରନ୍ତୁ IS_GITDIR; /* it is the <gitdir> exactly */
	ଯଦି (*rel != '/')
		ଫେରନ୍ତୁ IS_OUTSIDE_CONE;
	rel++;

	ଫେରନ୍ତୁ fsmonitor_classify_path_gitdir_relative(rel);
}

/*
 * We try to combine small batches at the front of the batch-list to avoid
 * having a long list.  This hopefully makes it a little easier when we want
 * to truncate and maintain the list.  However, we don't want the paths array
 * to just keep growing and growing with realloc, so we insert an arbitrary
 * limit.
 */
#define MY_COMBINE_LIMIT (1024)

ଶୂନ୍ୟ fsmonitor_publish(ସଂରଚନା fsmonitor_daemon_state *state,
		       ସଂରଚନା fsmonitor_batch *batch,
		       ଅଚଳ ସଂରଚନା string_list *cookie_names)
{
	ଯଦି (!batch && !cookie_names->nr)
		ଫେରନ୍ତୁ;

	pthread_mutex_lock(&state->main_lock);

	ଯଦି (batch) {
		ସଂରଚନା fsmonitor_batch *head;

		head = state->current_token_data->batch_head;
		ଯଦି (!head) {
			BUG("token does not have batch");
		} ଅନ୍ୟଥା ଯଦି (head->pinned_time) {
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
		} ଅନ୍ୟଥା ଯଦି (!head->batch_seq_nr) {
			/*
			 * Batch 0 is unpinned.  See the note in
			 * `fsmonitor_new_token_data()` about why we
			 * don't need to accumulate these paths.
			 */
			fsmonitor_batch__free_list(batch);
		} ଅନ୍ୟଥା ଯଦି (head->nr + batch->nr > MY_COMBINE_LIMIT) {
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
		} ଅନ୍ୟଥା {
			/*
			 * We are free to add the paths in the given
			 * batch onto the end of the current head batch.
			 */
			fsmonitor_batch__combine(head, batch);
			fsmonitor_batch__free_list(batch);
		}
	}

	ଯଦି (cookie_names->nr)
		with_lock__mark_cookies_seen(state, cookie_names);

	pthread_mutex_unlock(&state->main_lock);
}

ସ୍ଥିର ଶୂନ୍ୟ *fsm_health__thread_proc(ଶୂନ୍ୟ *_state)
{
	ସଂରଚନା fsmonitor_daemon_state *state = _state;

	trace2_thread_start("fsm-health");

	fsm_health__loop(state);

	trace2_thread_exit();
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଶୂନ୍ୟ *fsm_listen__thread_proc(ଶୂନ୍ୟ *_state)
{
	ସଂରଚନା fsmonitor_daemon_state *state = _state;

	trace2_thread_start("fsm-listen");

	trace_printf_ଚାବି(&trace_fsmonitor, "Watching: worktree '%s'",
			 state->path_worktree_watch.buf);
	ଯଦି (state->nr_paths_watching > 1)
		trace_printf_ଚାବି(&trace_fsmonitor, "Watching: gitdir '%s'",
				 state->path_gitdir_watch.buf);

	fsm_listen__loop(state);

	pthread_mutex_lock(&state->main_lock);
	ଯଦି (state->current_token_data &&
	    state->current_token_data->client_ref_ଗଣନା == 0)
		fsmonitor_free_token_data(state->current_token_data);
	state->current_token_data = ଖାଲି;
	pthread_mutex_unlock(&state->main_lock);

	trace2_thread_exit();
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsmonitor_run_daemon_1(ସଂରଚନା fsmonitor_daemon_state *state)
{
	ସଂରଚନା ipc_server_opts ipc_opts = {
		.nr_threads = fsmonitor__ipc_threads,

		/*
		 * We know that there are no other active threads yet,
		 * so we can let the IPC layer temporarily chdir() if
		 * it needs to when creating the server side of the
		 * Unix domain socket.
		 */
		.uds_disallow_chdir = 0
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା health_started = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା listener_started = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;

	/*
	 * Start the IPC thread pool before the we've started the file
	 * system event listener thread so that we have the IPC handle
	 * before we need it.
	 */
	ଯଦି (ipc_server_init_async(&state->ipc_server_data,
				  state->path_ipc.buf, &ipc_opts,
				  handle_client, state))
		ଫେରନ୍ତୁ error_errno(
			_("could not start IPC thread pool on '%s'"),
			state->path_ipc.buf);

	/*
	 * Start the fsmonitor listener thread to collect filesystem
	 * events.
	 */
	ଯଦି (pthread_create(&state->listener_thread, ଖାଲି,
			   fsm_listen__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_data);
		err = error(_("could not start fsmonitor listener thread"));
		ଯାଅ cleanup;
	}
	listener_started = 1;

	/*
	 * Start the health thread to watch over our process.
	 */
	ଯଦି (pthread_create(&state->health_thread, ଖାଲି,
			   fsm_health__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_data);
		err = error(_("could not start fsmonitor health thread"));
		ଯାଅ cleanup;
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
	ଯଦି (listener_started) {
		fsm_listen__stop_async(state);
		pthread_join(state->listener_thread, ଖାଲି);
	}

	ଯଦି (health_started) {
		fsm_health__stop_async(state);
		pthread_join(state->health_thread, ଖାଲି);
	}

	ଯଦି (err)
		ଫେରନ୍ତୁ err;
	ଯଦି (state->listen_error_code)
		ଫେରନ୍ତୁ state->listen_error_code;
	ଯଦି (state->health_error_code)
		ଫେରନ୍ତୁ state->health_error_code;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsmonitor_run_daemon(ଶୂନ୍ୟ)
{
	ସଂରଚନା fsmonitor_daemon_state state;
	ଅଚଳ ଅକ୍ଷର *home;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

	memset(&state, 0, ଆକାର(state));

	hashmap_init(&state.cookies, cookies_cmp, ଖାଲି, 0);
	pthread_mutex_init(&state.main_lock, ଖାଲି);
	pthread_cond_init(&state.cookies_cond, ଖାଲି);
	state.listen_error_code = 0;
	state.health_error_code = 0;
	state.current_token_data = fsmonitor_new_token_data();

	/* Prepare to (recursively) watch the <worktree-root> directory. */
	strbuf_init(&state.path_worktree_watch, 0);
	strbuf_addstr(&state.path_worktree_watch,
		      absolute_path(repo_get_work_ବୃକ୍ଷ(the_repository)));
	state.nr_paths_watching = 1;

	strbuf_init(&state.alias.alias, 0);
	strbuf_init(&state.alias.points_to, 0);
	ଯଦି ((err = fsmonitor__get_alias(state.path_worktree_watch.buf, &state.alias)))
		ଯାଅ done;

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
	ଯଦି (!is_directory(state.path_gitdir_watch.buf)) {
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
	ଯଦି (fsm_listen__ctor(&state)) {
		err = error(_("could not initialize listener thread"));
		ଯାଅ done;
	}

	ଯଦି (fsm_health__ctor(&state)) {
		err = error(_("could not initialize health thread"));
		ଯାଅ done;
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
	ଯଦି (home && *home && chdir(home))
		die_errno(_("could not cd home '%s'"), home);

	err = fsmonitor_run_daemon_1(&state);

done:
	fsmonitor_free_token_data(state.current_token_data);
	state.current_token_data = ଖାଲି;
	pthread_cond_destroy(&state.cookies_cond);
	pthread_mutex_destroy(&state.main_lock);
	{
		ସଂରଚନା hashmap_iter iter;
		ସଂରଚନା fsmonitor_cookie_item *cookie;

		hashmap_for_each_entry(&state.cookies, &iter, cookie, entry)
			ମୁକ୍ତ(cookie->name);
		hashmap_clear_and_free(&state.cookies,
				       ସଂରଚନା fsmonitor_cookie_item, entry);
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

	ଫେରନ୍ତୁ err;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_to_run_foreground_daemon(ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach_console)
{
	/*
	 * Technically, we don't need to probe for an existing daemon
	 * process, since we could just call `fsmonitor_run_daemon()`
	 * and let it fail if the pipe/socket is busy.
	 *
	 * However, this method gives us a nicer error message for a
	 * common error case.
	 */
	ଯଦି (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	ଯଦି (fsmonitor__announce_startup) {
		fprintf(stderr, _("running fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	ଯଦି (detach_console) {
#ifdef GIT_WINDOWS_NATIVE
		FreeConsole();
#ଅନ୍ୟଥା
		/*
		 * Create a new session so that the daemon is detached
		 * from the parent's process group.  This prevents
		 * shells with job control (e.g. bash with "set -m")
		 * from waiting on the daemon when they wait for a
		 * foreground command that implicitly spawned it.
		 */
		ଯଦି (setsid() == -1)
			warning_errno(_("setsid failed"));
#endif
	}

	ଫେରନ୍ତୁ !!fsmonitor_run_daemon();
}

ସ୍ଥିର start_bg_wait_cb bg_wait_cb;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bg_wait_cb(ଅଚଳ ସଂରଚନା child_process *cp UNUSED,
		      ଶୂନ୍ୟ *cb_data UNUSED)
{
	ଗଣନା ipc_active_state s = fsmonitor_ipc__get_state();

	ବାଛ (s) {
	ସ୍ଥିତି IPC_STATE__LISTENING:
		/* child is "ready" */
		ଫେରନ୍ତୁ 0;

	ସ୍ଥିତି IPC_STATE__NOT_LISTENING:
	ସ୍ଥିତି IPC_STATE__PATH_NOT_FOUND:
		/* give child more time */
		ଫେରନ୍ତୁ 1;

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	ସ୍ଥିତି IPC_STATE__INVALID_PATH:
	ସ୍ଥିତି IPC_STATE__OTHER_ERROR:
		/* all the time in world won't help */
		ଫେରନ୍ତୁ -1;
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_to_start_background_daemon(ଶୂନ୍ୟ)
{
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
	ଗଣନା start_bg_result sbgr;

	/*
	 * Before we try to create a background daemon process, see
	 * if a daemon process is already listening.  This makes it
	 * easier for us to report an already-listening error to the
	 * console, since our spawn/daemon can only report the success
	 * of creating the background process (and not whether it
	 * immediately exited).
	 */
	ଯଦି (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	ଯଦି (fsmonitor__announce_startup) {
		fprintf(stderr, _("starting fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	cp.git_cmd = 1;

	strvec_ଠେଲ(&cp.args, "fsmonitor--daemon");
	strvec_ଠେଲ(&cp.args, "run");
	strvec_ଠେଲ(&cp.args, "--detach");
	strvec_pushf(&cp.args, "--ipc-threads=%d", fsmonitor__ipc_threads);

	cp.no_stdin = 1;
	cp.no_stdout = 1;
	cp.no_stderr = 1;
	cp.close_fd_above_stderr = 1;

	sbgr = start_bg_command(&cp, bg_wait_cb, ଖାଲି,
				fsmonitor__start_timeout_sec);

	ବାଛ (sbgr) {
	ସ୍ଥିତି SBGR_READY:
		ଫେରନ୍ତୁ 0;

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	ସ୍ଥିତି SBGR_ERROR:
	ସ୍ଥିତି SBGR_CB_ERROR:
		ଫେରନ୍ତୁ error(_("daemon failed to start"));

	ସ୍ଥିତି SBGR_TIMEOUT:
		ଫେରନ୍ତୁ error(_("daemon not online yet"));

	ସ୍ଥିତି SBGR_DIED:
		ଫେରନ୍ତୁ error(_("daemon terminated"));
	}
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_fsmonitor__daemon(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			  ଅଚଳ ଅକ୍ଷର **argv,
			  ଅଚଳ ଅକ୍ଷର *prefix,
			  ସଂରଚନା repository *repo UNUSED)
{
	ଅଚଳ ଅକ୍ଷର *subcmd;
	ଗଣନା fsmonitor_reason reason;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach_console = 0;

	ସଂରଚନା option options[] = {
		OPT_BOOL(0, "detach", &detach_console, N_("detach from console")),
		OPT_INTEGER(0, "ipc-threads",
			    &fsmonitor__ipc_threads,
			    N_("use <n> ipc worker threads")),
		OPT_INTEGER(0, "start-timeout",
			    &fsmonitor__start_timeout_sec,
			    N_("max seconds to wait for background daemon startup")),

		OPT_END()
	};

	repo_config(the_repository, fsmonitor_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, options,
			     builtin_fsmonitor__daemon_usage, 0);
	ଯଦି (argc != 1)
		usage_with_options(builtin_fsmonitor__daemon_usage, options);
	subcmd = argv[0];

	ଯଦି (fsmonitor__ipc_threads < 1)
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
	ଯଦି (reason > FSMONITOR_REASON_OK)
		die("%s",
		    fsm_settings_get_incompatible_ସନ୍ଦେଶ(the_repository,
						       reason));

	ଯଦି (!strcmp(subcmd, "start"))
		ଫେରନ୍ତୁ !!try_to_start_background_daemon();

	ଯଦି (!strcmp(subcmd, "run"))
		ଫେରନ୍ତୁ !!try_to_run_foreground_daemon(detach_console);

	ଯଦି (!strcmp(subcmd, "stop"))
		ଫେରନ୍ତୁ !!do_as_client__send_stop();

	ଯଦି (!strcmp(subcmd, "status"))
		ଫେରନ୍ତୁ !!do_as_client__status();

	die(_("Unhandled subcommand '%s'"), subcmd);
}

#ଅନ୍ୟଥା
ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_fsmonitor__daemon(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix UNUSED, ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option options[] = {
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_fsmonitor__daemon_usage, options);

	die(_("fsmonitor--daemon not supported on this platform"));
}
#endif
