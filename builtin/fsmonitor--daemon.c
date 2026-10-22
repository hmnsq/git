#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "fsmonitor-ll.h"
#అన్తర్భావ "fsmonitor-ipc.h"
#అన్తర్భావ "fsmonitor-settings.h"
#అన్తర్భావ "compat/fsmonitor/fsm-health.h"
#అన్తర్భావ "compat/fsmonitor/fsm-listen.h"
#అన్తర్భావ "fsmonitor--daemon.h"

#అన్తర్భావ "simple-ipc.h"
#అన్తర్భావ "strmap.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "trace.h"
#అన్తర్భావ "trace2.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_fsmonitor__daemon_usage[] = {
	N_("git fsmonitor--daemon start [<options>]"),
	N_("git fsmonitor--daemon run [<options>]"),
	"git fsmonitor--daemon stop",
	"git fsmonitor--daemon status",
	ఖాళీ
};

#నిర్వచితమైతే HAVE_FSMONITOR_DAEMON_BACKEND
/*
 * Global state loaded from config.
 */
#నిర్వచించు FSMONITOR_IPC_దారాలు "fsmonitor.ipcthreads"
స్థిర పూర్ణాంకం fsmonitor_ipc_దారాలు = 8;

#నిర్వచించు FSMONITOR_ప్రారంభం_TIMEOUT "fsmonitor.starttimeout"
స్థిర పూర్ణాంకం fsmonitor_ప్రారంభం_timeout_sec = 60;

#నిర్వచించు FSMONITOR__ANNOUNCE_STARTUP "fsmonitor.announcestartup"
స్థిర పూర్ణాంకం fsmonitor__announce_startup = 0;

స్థిర పూర్ణాంకం fsmonitor_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			    స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(var, FSMONITOR_IPC_దారాలు)) {
		పూర్ణాంకం i = git_config_int(var, విలువ, ctx->kvi);
		ఒకవేళ (i < 1)
			తిరిగి error(_("value of '%s' out of range: %d"),
				     FSMONITOR_IPC_దారాలు, i);
		fsmonitor_ipc_దారాలు = i;
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, FSMONITOR_ప్రారంభం_TIMEOUT)) {
		పూర్ణాంకం i = git_config_int(var, విలువ, ctx->kvi);
		ఒకవేళ (i < 0)
			తిరిగి error(_("value of '%s' out of range: %d"),
				     FSMONITOR_ప్రారంభం_TIMEOUT, i);
		fsmonitor_ప్రారంభం_timeout_sec = i;
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, FSMONITOR__ANNOUNCE_STARTUP)) {
		పూర్ణాంకం is_bool;
		పూర్ణాంకం i = git_config_bool_or_int(var, విలువ, ctx->kvi, &is_bool);
		ఒకవేళ (i < 0)
			తిరిగి error(_("value of '%s' not bool or int: %d"),
				     var, i);
		fsmonitor__announce_startup = i;
		తిరిగి 0;
	}

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

/*
 * Acting as a CLIENT.
 *
 * Send a "quit" command to the `git-fsmonitor--daemon` (if running)
 * and wait for it to shutdown.
 */
స్థిర పూర్ణాంకం do_as_client__send_stop(శూన్య)
{
	నిర్మాణం strbuf answer = STRBUF_INIT;
	పూర్ణాంకం ret;
	పూర్ణాంకం max_wait_ms = 30000;
	పూర్ణాంకం elapsed_ms = 0;

	ret = fsmonitor_ipc__send_command("quit", &answer);

	/* The quit command does not return any response data. */
	strbuf_release(&answer);

	ఒకవేళ (ret)
		తిరిగి ret;

	trace2_region_enter("fsm_client", "polling-for-daemon-exit", ఖాళీ);
	అయితే (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING) {
		ఒకవేళ (elapsed_ms >= max_wait_ms) {
			trace2_region_leave("fsm_client",
					    "polling-for-daemon-exit", ఖాళీ);
			తిరిగి error(_("daemon did not stop within %d seconds"),
				     max_wait_ms / 1000);
		}
		sleep_millisec(50);
		elapsed_ms += 50;
	}
	trace2_region_leave("fsm_client", "polling-for-daemon-exit", ఖాళీ);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం do_as_client_స్థితి(శూన్య)
{
	గణన ipc_చురుకైన_state state = fsmonitor_ipc__get_state();

	ఎంపిక (state) {
	స్థితి IPC_STATE__LISTENING:
		రాయి(_("fsmonitor-daemon is watching '%s'\n"),
		       the_repository->worktree);
		తిరిగి 0;

	అప్రమేయ:
		రాయి(_("fsmonitor-daemon is not watching '%s'\n"),
		       the_repository->worktree);
		తిరిగి 1;
	}
}

గణన fsmonitor_cookie_అంశం_ఫలితం {
	FCIR_ERROR = -1, /* could not create cookie file ? */
	FCIR_INIT,
	FCIR_చూసిన,
	FCIR_ABORT,
};

నిర్మాణం fsmonitor_cookie_అంశం {
	నిర్మాణం hashmap_నమోదు నమోదు_2;
	అక్షరం *name;
	గణన fsmonitor_cookie_అంశం_ఫలితం ఫలితం;
};

స్థిర పూర్ణాంకం cookies_cmp(స్థిరరాశి శూన్య *దత్తాంశం వాడని,
		       స్థిరరాశి నిర్మాణం hashmap_నమోదు *he1,
		       స్థిరరాశి నిర్మాణం hashmap_నమోదు *he2, స్థిరరాశి శూన్య *keydata)
{
	స్థిరరాశి నిర్మాణం fsmonitor_cookie_అంశం *a =
		container_యొక్క(he1, స్థిరరాశి నిర్మాణం fsmonitor_cookie_అంశం, నమోదు_2);
	స్థిరరాశి నిర్మాణం fsmonitor_cookie_అంశం *b =
		container_యొక్క(he2, స్థిరరాశి నిర్మాణం fsmonitor_cookie_అంశం, నమోదు_2);

	తిరిగి strcmp(a->name, keydata ? keydata : b->name);
}

స్థిర గణన fsmonitor_cookie_అంశం_ఫలితం with_తాళం_wait_for_cookie(
	నిర్మాణం fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	పూర్ణాంకం fd;
	నిర్మాణం fsmonitor_cookie_అంశం *cookie;
	నిర్మాణం strbuf cookie_pathname = STRBUF_INIT;
	నిర్మాణం strbuf cookie_filename = STRBUF_INIT;
	గణన fsmonitor_cookie_అంశం_ఫలితం ఫలితం;
	పూర్ణాంకం my_cookie_seq;

	CALLOC_ARRAY(cookie, 1);

	my_cookie_seq = state->cookie_seq++;

	strbuf_addf(&cookie_filename, "%i-%i", getpid(), my_cookie_seq);

	strbuf_addbuf(&cookie_pathname, &state->మార్గం_cookie_ఉపసర్గ);
	strbuf_addbuf(&cookie_pathname, &cookie_filename);

	cookie->name = strbuf_detach(&cookie_filename, ఖాళీ);
	cookie->ఫలితం = FCIR_INIT;
	hashmap_నమోదు_init(&cookie->నమోదు_2, strhash(cookie->name));

	hashmap_కూడు(&state->cookies, &cookie->నమోదు_2);

	trace_printf_కీలకం(&trace_fsmonitor, "cookie-wait: '%s' '%s'",
			 cookie->name, cookie_pathname.buf);

	/*
	 * Create the cookie file on disk and then wait for a notification
	 * that the listener thread has seen it.
	 */
	fd = open(cookie_pathname.buf, O_WRONLY | O_CREAT | O_EXCL, 0600);
	ఒకవేళ (fd < 0) {
		error_errno(_("could not create fsmonitor cookie '%s'"),
			    cookie->name);

		cookie->ఫలితం = FCIR_ERROR;
		వెళ్ళు పూర్తి;
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
		నిర్మాణం timeval now;
		నిర్మాణం timespec ts;
		పూర్ణాంకం err = 0;

		gettimeofday(&now, ఖాళీ);
		ts.tv_sec = now.tv_sec + 1;
		ts.tv_nsec = now.tv_usec * 1000;

		అయితే (cookie->ఫలితం == FCIR_INIT && !err)
			err = pthread_cond_timedwait(&state->cookies_cond,
						     &state->main_తాళం,
						     &ts);
		ఒకవేళ (err == ETIMEDOUT && cookie->ఫలితం == FCIR_INIT) {
			trace_printf_కీలకం(&trace_fsmonitor,
					 "cookie_wait timed out");
			cookie->ఫలితం = FCIR_ERROR;
		}
	}

పూర్తి:
	hashmap_remove(&state->cookies, &cookie->నమోదు_2, ఖాళీ);

	ఫలితం = cookie->ఫలితం;

	విడుదల(cookie->name);
	విడుదల(cookie);
	strbuf_release(&cookie_pathname);

	తిరిగి ఫలితం;
}

/*
 * Mark these cookies as _SEEN and wake up the corresponding client threads.
 */
స్థిర శూన్య with_తాళం_mark_cookies_చూసిన(నిర్మాణం fsmonitor_daemon_state *state,
					 స్థిరరాశి నిర్మాణం string_list *cookie_పేర్లు)
{
	/* assert current thread holding state->main_lock */

	పూర్ణాంకం k;
	పూర్ణాంకం nr_చూసిన = 0;

	కొరకు (k = 0; k < cookie_పేర్లు->nr; k++) {
		నిర్మాణం fsmonitor_cookie_అంశం కీలకం;
		నిర్మాణం fsmonitor_cookie_అంశం *cookie;

		కీలకం.name = cookie_పేర్లు->వస్తువులు[k].string;
		hashmap_నమోదు_init(&కీలకం.నమోదు_2, strhash(కీలకం.name));

		cookie = hashmap_get_నమోదు(&state->cookies, &కీలకం, నమోదు_2, ఖాళీ);
		ఒకవేళ (cookie) {
			trace_printf_కీలకం(&trace_fsmonitor, "cookie-seen: '%s'",
					 cookie->name);
			cookie->ఫలితం = FCIR_చూసిన;
			nr_చూసిన++;
		}
	}

	ఒకవేళ (nr_చూసిన)
		pthread_cond_ప్రసారం(&state->cookies_cond);
}

/*
 * Set _ABORT on all pending cookies and wake up all client threads.
 */
స్థిర శూన్య with_తాళం_abort_all_cookies(నిర్మాణం fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	నిర్మాణం hashmap_iter iter;
	నిర్మాణం fsmonitor_cookie_అంశం *cookie;
	పూర్ణాంకం nr_aborted = 0;

	hashmap_for_each_నమోదు(&state->cookies, &iter, cookie, నమోదు_2) {
		trace_printf_కీలకం(&trace_fsmonitor, "cookie-abort: '%s'",
				 cookie->name);
		cookie->ఫలితం = FCIR_ABORT;
		nr_aborted++;
	}

	ఒకవేళ (nr_aborted)
		pthread_cond_ప్రసారం(&state->cookies_cond);
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
నిర్మాణం fsmonitor_token_దత్తాంశం {
	నిర్మాణం strbuf token_id;
	నిర్మాణం fsmonitor_batch *batch_తల;
	నిర్మాణం fsmonitor_batch *batch_తోక;
	uint64_t client_ref_లెక్క;
};

నిర్మాణం fsmonitor_batch {
	నిర్మాణం fsmonitor_batch *తదుపరి;
	uint64_t batch_seq_nr;
	స్థిరరాశి అక్షరం **interned_paths;
	కొలత_t nr, alloc;
	సమయం_t pinned_సమయం;
};

స్థిర నిర్మాణం fsmonitor_token_దత్తాంశం *fsmonitor_new_token_దత్తాంశం(శూన్య)
{
	స్థిర పూర్ణాంకం test_env_విలువ = -1;
	స్థిర uint64_t flush_లెక్క = 0;
	నిర్మాణం fsmonitor_token_దత్తాంశం *token;
	నిర్మాణం fsmonitor_batch *batch;

	CALLOC_ARRAY(token, 1);
	batch = fsmonitor_batch__new();

	strbuf_init(&token->token_id, 0);
	token->batch_తల = batch;
	token->batch_తోక = batch;
	token->client_ref_లెక్క = 0;

	ఒకవేళ (test_env_విలువ < 0)
		test_env_విలువ = git_env_bool("GIT_TEST_FSMONITOR_TOKEN", 0);

	ఒకవేళ (!test_env_విలువ) {
		నిర్మాణం timeval tv;
		నిర్మాణం tm tm;
		సమయం_t secs;

		gettimeofday(&tv, ఖాళీ);
		secs = tv.tv_sec;
		gmtime_r(&secs, &tm);

		strbuf_addf(&token->token_id,
			    "%"PRIu64".%d.%4d%02d%02dT%02d%02d%02d.%06ldZ",
			    flush_లెక్క++,
			    getpid(),
			    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			    tm.tm_hour, tm.tm_min, tm.tm_sec,
			    (దీర్ఘ)tv.tv_usec);
	} లేకపోతే {
		strbuf_addf(&token->token_id, "test_%08x", test_env_విలువ++);
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
	ఒకవేళ (test_env_విలువ)
		batch->pinned_సమయం = సమయం(ఖాళీ);

	తిరిగి token;
}

నిర్మాణం fsmonitor_batch *fsmonitor_batch__new(శూన్య)
{
	నిర్మాణం fsmonitor_batch *batch;

	CALLOC_ARRAY(batch, 1);

	తిరిగి batch;
}

శూన్య fsmonitor_batch__free_list(నిర్మాణం fsmonitor_batch *batch)
{
	అయితే (batch) {
		నిర్మాణం fsmonitor_batch *తదుపరి = batch->తదుపరి;

		/*
		 * The actual strings within the array of this batch
		 * are interned, so we don't own them.  We only own
		 * the array.
		 */
		విడుదల(batch->interned_paths);
		విడుదల(batch);

		batch = తదుపరి;
	}
}

శూన్య fsmonitor_batch_కూడు_మార్గం(నిర్మాణం fsmonitor_batch *batch,
			       స్థిరరాశి అక్షరం *మార్గం)
{
	స్థిరరాశి అక్షరం *interned_మార్గం = strintern(మార్గం);

	trace_printf_కీలకం(&trace_fsmonitor, "event: %s", interned_మార్గం);

	ALLOC_GROW(batch->interned_paths, batch->nr + 1, batch->alloc);
	batch->interned_paths[batch->nr++] = interned_మార్గం;
}

స్థిర శూన్య fsmonitor_batch__combine(నిర్మాణం fsmonitor_batch *batch_dest,
				     స్థిరరాశి నిర్మాణం fsmonitor_batch *batch_src)
{
	కొలత_t k;

	ALLOC_GROW(batch_dest->interned_paths,
		   batch_dest->nr + batch_src->nr + 1,
		   batch_dest->alloc);

	కొరకు (k = 0; k < batch_src->nr; k++)
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
#నిర్వచించు MY_సమయం_DELAY_SECONDS (5 * 60) /* seconds */

స్థిర నిర్మాణం fsmonitor_batch *with_తాళం_truncate_old_batches(
	నిర్మాణం fsmonitor_daemon_state *state,
	స్థిరరాశి నిర్మాణం fsmonitor_batch *batch_marker)
{
	/* assert current thread holding state->main_lock */

	స్థిరరాశి నిర్మాణం fsmonitor_batch *batch;
	నిర్మాణం fsmonitor_batch *remainder;

	ఒకవేళ (!batch_marker)
		తిరిగి ఖాళీ;

	trace_printf_కీలకం(&trace_fsmonitor, "Truncate: mark (%"PRIu64",%"PRIu64")",
			 batch_marker->batch_seq_nr,
			 (uint64_t)batch_marker->pinned_సమయం);

	కొరకు (batch = batch_marker; batch; batch = batch->తదుపరి) {
		సమయం_t t;

		ఒకవేళ (!batch->pinned_సమయం) /* an overflow batch */
			కొనసాగు;

		t = batch->pinned_సమయం + MY_సమయం_DELAY_SECONDS;
		ఒకవేళ (t > batch_marker->pinned_సమయం) /* too close to marker */
			కొనసాగు;

		వెళ్ళు truncate_past_ఇక్కడ;
	}

	తిరిగి ఖాళీ;

truncate_past_ఇక్కడ:
	state->current_token_దత్తాంశం->batch_తోక = (నిర్మాణం fsmonitor_batch *)batch;

	remainder = ((నిర్మాణం fsmonitor_batch *)batch)->తదుపరి;
	((నిర్మాణం fsmonitor_batch *)batch)->తదుపరి = ఖాళీ;

	తిరిగి remainder;
}

స్థిర శూన్య fsmonitor_free_token_దత్తాంశం(నిర్మాణం fsmonitor_token_దత్తాంశం *token)
{
	ఒకవేళ (!token)
		తిరిగి;

	ప్రతిజ్ఞా(token->client_ref_లెక్క == 0);

	strbuf_release(&token->token_id);

	fsmonitor_batch__free_list(token->batch_తల);

	విడుదల(token);
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
స్థిర శూన్య with_తాళం_do_force_resync(నిర్మాణం fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	నిర్మాణం fsmonitor_token_దత్తాంశం *free_me = ఖాళీ;
	నిర్మాణం fsmonitor_token_దత్తాంశం *new_one = ఖాళీ;

	new_one = fsmonitor_new_token_దత్తాంశం();

	ఒకవేళ (state->current_token_దత్తాంశం->client_ref_లెక్క == 0)
		free_me = state->current_token_దత్తాంశం;
	state->current_token_దత్తాంశం = new_one;

	fsmonitor_free_token_దత్తాంశం(free_me);

	with_తాళం_abort_all_cookies(state);
}

శూన్య fsmonitor_force_resync(నిర్మాణం fsmonitor_daemon_state *state)
{
	pthread_mutex_తాళం(&state->main_తాళం);
	with_తాళం_do_force_resync(state);
	pthread_mutex_unlock(&state->main_తాళం);
}

/*
 * Format an opaque token string to send to the client.
 */
స్థిర శూన్య with_తాళం_format_response_token(
	నిర్మాణం strbuf *response_token,
	స్థిరరాశి నిర్మాణం strbuf *response_token_id,
	స్థిరరాశి నిర్మాణం fsmonitor_batch *batch)
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
స్థిర పూర్ణాంకం fsmonitor_parse_client_token(స్థిరరాశి అక్షరం *buf_token,
					నిర్మాణం strbuf *requested_token_id,
					uint64_t *seq_nr)
{
	స్థిరరాశి అక్షరం *p;
	అక్షరం *p_చివర;

	strbuf_reset(requested_token_id);
	*seq_nr = 0;

	ఒకవేళ (!skip_ఉపసర్గ(buf_token, "builtin:", &p))
		తిరిగి -1;

	అయితే (*p && *p != ':')
		strbuf_addch(requested_token_id, *p++);
	ఒకవేళ (!*p++)
		తిరిగి -1;

	*seq_nr = (uint64_t)strtoumax(p, &p_చివర, 10);
	ఒకవేళ (*p_చివర)
		తిరిగి -1;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం do_handle_client(నిర్మాణం fsmonitor_daemon_state *state,
			    స్థిరరాశి అక్షరం *command,
			    ipc_server_జవాబు_cb *జవాబు,
			    నిర్మాణం ipc_server_జవాబు_దత్తాంశం *జవాబు_దత్తాంశం)
{
	నిర్మాణం fsmonitor_token_దత్తాంశం *token_దత్తాంశం = ఖాళీ;
	నిర్మాణం strbuf response_token = STRBUF_INIT;
	నిర్మాణం strbuf requested_token_id = STRBUF_INIT;
	నిర్మాణం strbuf payload = STRBUF_INIT;
	uint64_t requested_oldest_seq_nr = 0;
	uint64_t మొత్తం_response_len = 0;
	స్థిరరాశి అక్షరం *p;
	స్థిరరాశి నిర్మాణం fsmonitor_batch *batch_తల;
	స్థిరరాశి నిర్మాణం fsmonitor_batch *batch;
	నిర్మాణం fsmonitor_batch *remainder = ఖాళీ;
	intmax_t లెక్క = 0, duplicates = 0;
	నిర్మాణం strset shown = STRSET_INIT;
	పూర్ణాంకం do_trivial = 0;
	పూర్ణాంకం do_flush = 0;
	పూర్ణాంకం do_cookie = 0;
	గణన fsmonitor_cookie_అంశం_ఫలితం cookie_ఫలితం;

	/*
	 * We expect `command` to be of the form:
	 *
	 * <command> := quit NUL
	 *            | flush NUL
	 *            | <V1-time-since-epoch-ns> NUL
	 *            | <V2-opaque-fsmonitor-token> NUL
	 */

	ఒకవేళ (!strcmp(command, "quit")) {
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
		తిరిగి SIMPLE_IPC_QUIT;

	} లేకపోతే ఒకవేళ (!strcmp(command, "flush")) {
		/*
		 * Flush all of our cached data and generate a new token
		 * just like if we lost sync with the filesystem.
		 *
		 * Then send a trivial response using the new token.
		 */
		do_flush = 1;
		do_trivial = 1;

	} లేకపోతే ఒకవేళ (!skip_ఉపసర్గ(command, "builtin:", &p)) {
		/* assume V1 timestamp or garbage */

		అక్షరం *p_చివర;

		strtoumax(command, &p_చివర, 10);
		trace_printf_కీలకం(&trace_fsmonitor,
				 ((*p_చివర) ?
				  "fsmonitor: invalid command line '%s'" :
				  "fsmonitor: unsupported V1 protocol '%s'"),
				 command);
		do_trivial = 1;
		do_cookie = 1;

	} లేకపోతే {
		/* We have "builtin:*" */
		ఒకవేళ (fsmonitor_parse_client_token(command, &requested_token_id,
						 &requested_oldest_seq_nr)) {
			trace_printf_కీలకం(&trace_fsmonitor,
					 "fsmonitor: invalid V2 protocol token '%s'",
					 command);
			do_trivial = 1;
			do_cookie = 1;

		} లేకపోతే {
			/*
			 * We have a V2 valid token:
			 *     "builtin:<token_id>:<seq_nr>"
			 */
			do_cookie = 1;
		}
	}

	pthread_mutex_తాళం(&state->main_తాళం);

	ఒకవేళ (!state->current_token_దత్తాంశం)
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
	ఒకవేళ (do_cookie) {
		cookie_ఫలితం = with_తాళం_wait_for_cookie(state);
		ఒకవేళ (cookie_ఫలితం != FCIR_చూసిన) {
			error(_("fsmonitor: cookie_result '%d' != SEEN"),
			      cookie_ఫలితం);
			do_trivial = 1;
		}
	}

	ఒకవేళ (do_flush)
		with_తాళం_do_force_resync(state);

	/*
	 * We mark the current head of the batch list as "pinned" so
	 * that the listener thread will treat this item as read-only
	 * (and prevent any more paths from being added to it) from
	 * now on.
	 */
	token_దత్తాంశం = state->current_token_దత్తాంశం;
	batch_తల = token_దత్తాంశం->batch_తల;
	((నిర్మాణం fsmonitor_batch *)batch_తల)->pinned_సమయం = సమయం(ఖాళీ);

	/*
	 * FSMonitor Protocol V2 requires that we send a response header
	 * with a "new current token" and then all of the paths that changed
	 * since the "requested token".  We send the seq_nr of the just-pinned
	 * head batch so that future requests from a client will be relative
	 * to it.
	 */
	with_తాళం_format_response_token(&response_token,
					 &token_దత్తాంశం->token_id, batch_తల);

	జవాబు(జవాబు_దత్తాంశం, response_token.buf, response_token.len + 1);
	మొత్తం_response_len += response_token.len + 1;

	trace2_దత్తాంశం_string("fsmonitor", the_repository, "response/token",
			   response_token.buf);
	trace_printf_కీలకం(&trace_fsmonitor, "response token: %s",
			 response_token.buf);

	ఒకవేళ (!do_trivial) {
		ఒకవేళ (strcmp(requested_token_id.buf, token_దత్తాంశం->token_id.buf)) {
			/*
			 * The client last spoke to a different daemon
			 * instance -OR- the daemon had to resync with
			 * the filesystem (and lost events), so reject.
			 */
			trace2_దత్తాంశం_string("fsmonitor", the_repository,
					   "response/token", "different");
			do_trivial = 1;

		} లేకపోతే ఒకవేళ (requested_oldest_seq_nr <
			   token_దత్తాంశం->batch_తోక->batch_seq_nr) {
			/*
			 * The client wants older events than we have for
			 * this token_id.  This means that the end of our
			 * batch list was truncated and we cannot give the
			 * client a complete snapshot relative to their
			 * request.
			 */
			trace_printf_కీలకం(&trace_fsmonitor,
					 "client requested truncated data");
			do_trivial = 1;
		}
	}

	ఒకవేళ (do_trivial) {
		pthread_mutex_unlock(&state->main_తాళం);

		జవాబు(జవాబు_దత్తాంశం, "/", 2);

		trace2_దత్తాంశం_intmax("fsmonitor", the_repository,
				   "response/trivial", 1);

		వెళ్ళు cleanup;
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
	token_దత్తాంశం->client_ref_లెక్క++;

	pthread_mutex_unlock(&state->main_తాళం);

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
	strset_init_with_options(&shown, ఖాళీ, 0);
	కొరకు (batch = batch_తల;
	     batch && batch->batch_seq_nr > requested_oldest_seq_nr;
	     batch = batch->తదుపరి) {
		కొలత_t k;

		కొరకు (k = 0; k < batch->nr; k++) {
			స్థిరరాశి అక్షరం *s = batch->interned_paths[k];
			కొలత_t s_len;

			ఒకవేళ (!strset_కూడు(&shown, s))
				duplicates++;
			లేకపోతే {
				trace_printf_కీలకం(&trace_fsmonitor,
						 "send[%"PRIuMAX"]: %s",
						 లెక్క, s);

				/* Each path gets written with a trailing NUL */
				s_len = అక్షరశ్రేణి_పొడవు(s) + 1;

				ఒకవేళ (payload.len + s_len >=
				    LARGE_PACKET_దత్తాంశం_MAX) {
					జవాబు(జవాబు_దత్తాంశం, payload.buf,
					      payload.len);
					మొత్తం_response_len += payload.len;
					strbuf_reset(&payload);
				}

				strbuf_కూడు(&payload, s, s_len);
				లెక్క++;
			}
		}
	}

	ఒకవేళ (payload.len) {
		జవాబు(జవాబు_దత్తాంశం, payload.buf, payload.len);
		మొత్తం_response_len += payload.len;
	}

	pthread_mutex_తాళం(&state->main_తాళం);

	ఒకవేళ (token_దత్తాంశం->client_ref_లెక్క > 0)
		token_దత్తాంశం->client_ref_లెక్క--;

	ఒకవేళ (token_దత్తాంశం->client_ref_లెక్క == 0) {
		ఒకవేళ (token_దత్తాంశం != state->current_token_దత్తాంశం) {
			/*
			 * The listener thread did a token-reset while we were
			 * walking the batch list.  Therefore, this token is
			 * stale and can be discarded completely.  If we are
			 * the last reader thread using this token, we own
			 * that work.
			 */
			fsmonitor_free_token_దత్తాంశం(token_దత్తాంశం);
		} లేకపోతే ఒకవేళ (batch) {
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
			remainder = with_తాళం_truncate_old_batches(state,
								    batch);
		}
	}

	pthread_mutex_unlock(&state->main_తాళం);

	ఒకవేళ (remainder)
		fsmonitor_batch__free_list(remainder);

	trace2_దత్తాంశం_intmax("fsmonitor", the_repository, "response/length", మొత్తం_response_len);
	trace2_దత్తాంశం_intmax("fsmonitor", the_repository, "response/count/files", లెక్క);
	trace2_దత్తాంశం_intmax("fsmonitor", the_repository, "response/count/duplicates", duplicates);

cleanup:
	strset_clear(&shown);
	strbuf_release(&response_token);
	strbuf_release(&requested_token_id);
	strbuf_release(&payload);

	తిరిగి 0;
}

స్థిర ipc_server_application_cb handle_client;

స్థిర పూర్ణాంకం handle_client(శూన్య *దత్తాంశం,
			 స్థిరరాశి అక్షరం *command, కొలత_t command_len,
			 ipc_server_జవాబు_cb *జవాబు,
			 నిర్మాణం ipc_server_జవాబు_దత్తాంశం *జవాబు_దత్తాంశం)
{
	నిర్మాణం fsmonitor_daemon_state *state = దత్తాంశం;
	పూర్ణాంకం ఫలితం;

	/*
	 * The Simple IPC API now supports {char*, len} arguments, but
	 * FSMonitor always uses proper null-terminated strings, so
	 * we can ignore the command_len argument.  (Trust, but verify.)
	 */
	ఒకవేళ (command_len != అక్షరశ్రేణి_పొడవు(command))
		BUG("FSMonitor assumes text messages");

	trace_printf_కీలకం(&trace_fsmonitor, "requested token: %s", command);

	trace2_region_enter("fsmonitor", "handle_client", the_repository);
	trace2_దత్తాంశం_string("fsmonitor", the_repository, "request", command);

	ఫలితం = do_handle_client(state, command, జవాబు, జవాబు_దత్తాంశం);

	trace2_region_leave("fsmonitor", "handle_client", the_repository);

	తిరిగి ఫలితం;
}

#నిర్వచించు FSMONITOR_DIR           "fsmonitor--daemon"
#నిర్వచించు FSMONITOR_COOKIE_DIR    "cookies"
#నిర్వచించు FSMONITOR_COOKIE_ఉపసర్గ (FSMONITOR_DIR "/" FSMONITOR_COOKIE_DIR "/")

గణన fsmonitor_మార్గం_type fsmonitor_classify_మార్గం_workdir_relative(
	స్థిరరాశి అక్షరం *rel)
{
	ఒకవేళ (fspathncmp(rel, ".git", 4))
		తిరిగి IS_WORKDIR_మార్గం;
	rel += 4;

	ఒకవేళ (!*rel)
		తిరిగి IS_DOT_GIT;
	ఒకవేళ (*rel != '/')
		తిరిగి IS_WORKDIR_మార్గం; /* e.g. .gitignore */
	rel++;

	ఒకవేళ (!fspathncmp(rel, FSMONITOR_COOKIE_ఉపసర్గ,
			అక్షరశ్రేణి_పొడవు(FSMONITOR_COOKIE_ఉపసర్గ)))
		తిరిగి IS_INSIDE_DOT_GIT_WITH_COOKIE_ఉపసర్గ;

	తిరిగి IS_INSIDE_DOT_GIT;
}

గణన fsmonitor_మార్గం_type fsmonitor_classify_మార్గం_gitdir_relative(
	స్థిరరాశి అక్షరం *rel)
{
	ఒకవేళ (!fspathncmp(rel, FSMONITOR_COOKIE_ఉపసర్గ,
			అక్షరశ్రేణి_పొడవు(FSMONITOR_COOKIE_ఉపసర్గ)))
		తిరిగి IS_INSIDE_GITDIR_WITH_COOKIE_ఉపసర్గ;

	తిరిగి IS_INSIDE_GITDIR;
}

స్థిర గణన fsmonitor_మార్గం_type try_classify_workdir_abs_మార్గం(
	నిర్మాణం fsmonitor_daemon_state *state,
	స్థిరరాశి అక్షరం *మార్గం)
{
	స్థిరరాశి అక్షరం *rel;

	ఒకవేళ (fspathncmp(మార్గం, state->మార్గం_worktree_watch.buf,
		       state->మార్గం_worktree_watch.len))
		తిరిగి IS_OUTSIDE_CONE;

	rel = మార్గం + state->మార్గం_worktree_watch.len;

	ఒకవేళ (!*rel)
		తిరిగి IS_WORKDIR_మార్గం; /* it is the root dir exactly */
	ఒకవేళ (*rel != '/')
		తిరిగి IS_OUTSIDE_CONE;
	rel++;

	తిరిగి fsmonitor_classify_మార్గం_workdir_relative(rel);
}

గణన fsmonitor_మార్గం_type fsmonitor_classify_మార్గం_absolute(
	నిర్మాణం fsmonitor_daemon_state *state,
	స్థిరరాశి అక్షరం *మార్గం)
{
	స్థిరరాశి అక్షరం *rel;
	గణన fsmonitor_మార్గం_type t;

	t = try_classify_workdir_abs_మార్గం(state, మార్గం);
	ఒకవేళ (state->nr_paths_watching == 1)
		తిరిగి t;
	ఒకవేళ (t != IS_OUTSIDE_CONE)
		తిరిగి t;

	ఒకవేళ (fspathncmp(మార్గం, state->మార్గం_gitdir_watch.buf,
		       state->మార్గం_gitdir_watch.len))
		తిరిగి IS_OUTSIDE_CONE;

	rel = మార్గం + state->మార్గం_gitdir_watch.len;

	ఒకవేళ (!*rel)
		తిరిగి IS_GITDIR; /* it is the <gitdir> exactly */
	ఒకవేళ (*rel != '/')
		తిరిగి IS_OUTSIDE_CONE;
	rel++;

	తిరిగి fsmonitor_classify_మార్గం_gitdir_relative(rel);
}

/*
 * We try to combine small batches at the front of the batch-list to avoid
 * having a long list.  This hopefully makes it a little easier when we want
 * to truncate and maintain the list.  However, we don't want the paths array
 * to just keep growing and growing with realloc, so we insert an arbitrary
 * limit.
 */
#నిర్వచించు MY_COMBINE_హద్దు (1024)

శూన్య fsmonitor_publish(నిర్మాణం fsmonitor_daemon_state *state,
		       నిర్మాణం fsmonitor_batch *batch,
		       స్థిరరాశి నిర్మాణం string_list *cookie_పేర్లు)
{
	ఒకవేళ (!batch && !cookie_పేర్లు->nr)
		తిరిగి;

	pthread_mutex_తాళం(&state->main_తాళం);

	ఒకవేళ (batch) {
		నిర్మాణం fsmonitor_batch *తల;

		తల = state->current_token_దత్తాంశం->batch_తల;
		ఒకవేళ (!తల) {
			BUG("token does not have batch");
		} లేకపోతే ఒకవేళ (తల->pinned_సమయం) {
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
			batch->batch_seq_nr = తల->batch_seq_nr + 1;
			batch->తదుపరి = తల;
			state->current_token_దత్తాంశం->batch_తల = batch;
		} లేకపోతే ఒకవేళ (!తల->batch_seq_nr) {
			/*
			 * Batch 0 is unpinned.  See the note in
			 * `fsmonitor_new_token_data()` about why we
			 * don't need to accumulate these paths.
			 */
			fsmonitor_batch__free_list(batch);
		} లేకపోతే ఒకవేళ (తల->nr + batch->nr > MY_COMBINE_హద్దు) {
			/*
			 * The head batch in the list has never been
			 * transmitted to a client, but folding the
			 * contents of the new batch onto it would
			 * exceed our arbitrary limit, so just prepend
			 * the new batch onto the list.
			 */
			batch->batch_seq_nr = తల->batch_seq_nr + 1;
			batch->తదుపరి = తల;
			state->current_token_దత్తాంశం->batch_తల = batch;
		} లేకపోతే {
			/*
			 * We are free to add the paths in the given
			 * batch onto the end of the current head batch.
			 */
			fsmonitor_batch__combine(తల, batch);
			fsmonitor_batch__free_list(batch);
		}
	}

	ఒకవేళ (cookie_పేర్లు->nr)
		with_తాళం_mark_cookies_చూసిన(state, cookie_పేర్లు);

	pthread_mutex_unlock(&state->main_తాళం);
}

స్థిర శూన్య *fsm_health__thread_proc(శూన్య *_state)
{
	నిర్మాణం fsmonitor_daemon_state *state = _state;

	trace2_thread_ప్రారంభం("fsm-health");

	fsm_health__loop(state);

	trace2_thread_exit();
	తిరిగి ఖాళీ;
}

స్థిర శూన్య *fsm_listen__thread_proc(శూన్య *_state)
{
	నిర్మాణం fsmonitor_daemon_state *state = _state;

	trace2_thread_ప్రారంభం("fsm-listen");

	trace_printf_కీలకం(&trace_fsmonitor, "Watching: worktree '%s'",
			 state->మార్గం_worktree_watch.buf);
	ఒకవేళ (state->nr_paths_watching > 1)
		trace_printf_కీలకం(&trace_fsmonitor, "Watching: gitdir '%s'",
				 state->మార్గం_gitdir_watch.buf);

	fsm_listen__loop(state);

	pthread_mutex_తాళం(&state->main_తాళం);
	ఒకవేళ (state->current_token_దత్తాంశం &&
	    state->current_token_దత్తాంశం->client_ref_లెక్క == 0)
		fsmonitor_free_token_దత్తాంశం(state->current_token_దత్తాంశం);
	state->current_token_దత్తాంశం = ఖాళీ;
	pthread_mutex_unlock(&state->main_తాళం);

	trace2_thread_exit();
	తిరిగి ఖాళీ;
}

స్థిర పూర్ణాంకం fsmonitor_నిరంతరం_daemon_1(నిర్మాణం fsmonitor_daemon_state *state)
{
	నిర్మాణం ipc_server_opts ipc_opts = {
		.nr_దారాలు = fsmonitor_ipc_దారాలు,

		/*
		 * We know that there are no other active threads yet,
		 * so we can let the IPC layer temporarily chdir() if
		 * it needs to when creating the server side of the
		 * Unix domain socket.
		 */
		.uds_disallow_chdir = 0
	};
	పూర్ణాంకం health_started = 0;
	పూర్ణాంకం listener_started = 0;
	పూర్ణాంకం err = 0;

	/*
	 * Start the IPC thread pool before the we've started the file
	 * system event listener thread so that we have the IPC handle
	 * before we need it.
	 */
	ఒకవేళ (ipc_server_init_async(&state->ipc_server_దత్తాంశం,
				  state->మార్గం_ipc.buf, &ipc_opts,
				  handle_client, state))
		తిరిగి error_errno(
			_("could not start IPC thread pool on '%s'"),
			state->మార్గం_ipc.buf);

	/*
	 * Start the fsmonitor listener thread to collect filesystem
	 * events.
	 */
	ఒకవేళ (pthread_create(&state->listener_thread, ఖాళీ,
			   fsm_listen__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_దత్తాంశం);
		err = error(_("could not start fsmonitor listener thread"));
		వెళ్ళు cleanup;
	}
	listener_started = 1;

	/*
	 * Start the health thread to watch over our process.
	 */
	ఒకవేళ (pthread_create(&state->health_thread, ఖాళీ,
			   fsm_health__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_దత్తాంశం);
		err = error(_("could not start fsmonitor health thread"));
		వెళ్ళు cleanup;
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
	ipc_server_await(state->ipc_server_దత్తాంశం);

	/*
	 * The fsmonitor listener thread may have received a shutdown
	 * event from the IPC thread pool, but it doesn't hurt to tell
	 * it again.  And wait for it to shutdown.
	 */
	ఒకవేళ (listener_started) {
		fsm_listen__stop_async(state);
		pthread_join(state->listener_thread, ఖాళీ);
	}

	ఒకవేళ (health_started) {
		fsm_health__stop_async(state);
		pthread_join(state->health_thread, ఖాళీ);
	}

	ఒకవేళ (err)
		తిరిగి err;
	ఒకవేళ (state->listen_error_సంకేతం)
		తిరిగి state->listen_error_సంకేతం;
	ఒకవేళ (state->health_error_సంకేతం)
		తిరిగి state->health_error_సంకేతం;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం fsmonitor_నిరంతరం_daemon(శూన్య)
{
	నిర్మాణం fsmonitor_daemon_state state;
	స్థిరరాశి అక్షరం *home;
	పూర్ణాంకం err;

	memset(&state, 0, పరిమాణం(state));

	hashmap_init(&state.cookies, cookies_cmp, ఖాళీ, 0);
	pthread_mutex_init(&state.main_తాళం, ఖాళీ);
	pthread_cond_init(&state.cookies_cond, ఖాళీ);
	state.listen_error_సంకేతం = 0;
	state.health_error_సంకేతం = 0;
	state.current_token_దత్తాంశం = fsmonitor_new_token_దత్తాంశం();

	/* Prepare to (recursively) watch the <worktree-root> directory. */
	strbuf_init(&state.మార్గం_worktree_watch, 0);
	strbuf_addstr(&state.మార్గం_worktree_watch,
		      absolute_మార్గం(repo_get_work_వృక్షం(the_repository)));
	state.nr_paths_watching = 1;

	strbuf_init(&state.alias.alias, 0);
	strbuf_init(&state.alias.points_గమ్యం, 0);
	ఒకవేళ ((err = fsmonitor__get_alias(state.మార్గం_worktree_watch.buf, &state.alias)))
		వెళ్ళు పూర్తి;

	/*
	 * We create and delete cookie files somewhere inside the .git
	 * directory to help us keep sync with the file system.  If
	 * ".git" is not a directory, then <gitdir> is not inside the
	 * cone of <worktree-root>, so set up a second watch to watch
	 * the <gitdir> so that we get events for the cookie files.
	 */
	strbuf_init(&state.మార్గం_gitdir_watch, 0);
	strbuf_addbuf(&state.మార్గం_gitdir_watch, &state.మార్గం_worktree_watch);
	strbuf_addstr(&state.మార్గం_gitdir_watch, "/.git");
	ఒకవేళ (!is_directory(state.మార్గం_gitdir_watch.buf)) {
		strbuf_reset(&state.మార్గం_gitdir_watch);
		strbuf_addstr(&state.మార్గం_gitdir_watch,
			      absolute_మార్గం(repo_get_git_dir(the_repository)));
		strbuf_strip_suffix(&state.మార్గం_gitdir_watch, "/.");
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
	strbuf_init(&state.మార్గం_cookie_ఉపసర్గ, 0);
	strbuf_addbuf(&state.మార్గం_cookie_ఉపసర్గ, &state.మార్గం_gitdir_watch);

	strbuf_addch(&state.మార్గం_cookie_ఉపసర్గ, '/');
	strbuf_addstr(&state.మార్గం_cookie_ఉపసర్గ, FSMONITOR_DIR);
	mkdir(state.మార్గం_cookie_ఉపసర్గ.buf, 0777);

	strbuf_addch(&state.మార్గం_cookie_ఉపసర్గ, '/');
	strbuf_addstr(&state.మార్గం_cookie_ఉపసర్గ, FSMONITOR_COOKIE_DIR);
	mkdir(state.మార్గం_cookie_ఉపసర్గ.buf, 0777);

	strbuf_addch(&state.మార్గం_cookie_ఉపసర్గ, '/');

	/*
	 * We create a named-pipe or unix domain socket inside of the
	 * ".git" directory.  (Well, on Windows, we base our named
	 * pipe in the NPFS on the absolute path of the git
	 * directory.)
	 */
	strbuf_init(&state.మార్గం_ipc, 0);
	strbuf_addstr(&state.మార్గం_ipc,
		absolute_మార్గం(fsmonitor_ipc_get_మార్గం(the_repository)));

	/*
	 * Confirm that we can create platform-specific resources for the
	 * filesystem listener before we bother starting all the threads.
	 */
	ఒకవేళ (fsm_listen__ctor(&state)) {
		err = error(_("could not initialize listener thread"));
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (fsm_health__ctor(&state)) {
		err = error(_("could not initialize health thread"));
		వెళ్ళు పూర్తి;
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
	ఒకవేళ (home && *home && chdir(home))
		die_errno(_("could not cd home '%s'"), home);

	err = fsmonitor_నిరంతరం_daemon_1(&state);

పూర్తి:
	fsmonitor_free_token_దత్తాంశం(state.current_token_దత్తాంశం);
	state.current_token_దత్తాంశం = ఖాళీ;
	pthread_cond_destroy(&state.cookies_cond);
	pthread_mutex_destroy(&state.main_తాళం);
	{
		నిర్మాణం hashmap_iter iter;
		నిర్మాణం fsmonitor_cookie_అంశం *cookie;

		hashmap_for_each_నమోదు(&state.cookies, &iter, cookie, నమోదు_2)
			విడుదల(cookie->name);
		hashmap_clear_and_free(&state.cookies,
				       నిర్మాణం fsmonitor_cookie_అంశం, నమోదు_2);
	}
	fsm_listen__dtor(&state);
	fsm_health__dtor(&state);

	ipc_server_free(state.ipc_server_దత్తాంశం);

	strbuf_release(&state.మార్గం_worktree_watch);
	strbuf_release(&state.మార్గం_gitdir_watch);
	strbuf_release(&state.మార్గం_cookie_ఉపసర్గ);
	strbuf_release(&state.మార్గం_ipc);
	strbuf_release(&state.alias.alias);
	strbuf_release(&state.alias.points_గమ్యం);

	తిరిగి err;
}

స్థిర పూర్ణాంకం try_గమ్యం_నిరంతరం_foreground_daemon(పూర్ణాంకం detach_console)
{
	/*
	 * Technically, we don't need to probe for an existing daemon
	 * process, since we could just call `fsmonitor_run_daemon()`
	 * and let it fail if the pipe/socket is busy.
	 *
	 * However, this method gives us a nicer error message for a
	 * common error case.
	 */
	ఒకవేళ (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	ఒకవేళ (fsmonitor__announce_startup) {
		fprintf(stderr, _("running fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	ఒకవేళ (detach_console) {
#నిర్వచితమైతే GIT_WINDOWS_NATIVE
		FreeConsole();
#లేకపోతే
		/*
		 * Create a new session so that the daemon is detached
		 * from the parent's process group.  This prevents
		 * shells with job control (e.g. bash with "set -m")
		 * from waiting on the daemon when they wait for a
		 * foreground command that implicitly spawned it.
		 */
		ఒకవేళ (setsid() == -1)
			warning_errno(_("setsid failed"));
#అయితే_ముగింపు
	}

	తిరిగి !!fsmonitor_నిరంతరం_daemon();
}

స్థిర ప్రారంభం_bg_wait_cb bg_wait_cb;

స్థిర పూర్ణాంకం bg_wait_cb(స్థిరరాశి నిర్మాణం బిడ్డ_process *cp వాడని,
		      శూన్య *cb_దత్తాంశం వాడని)
{
	గణన ipc_చురుకైన_state s = fsmonitor_ipc__get_state();

	ఎంపిక (s) {
	స్థితి IPC_STATE__LISTENING:
		/* child is "ready" */
		తిరిగి 0;

	స్థితి IPC_STATE__NOT_LISTENING:
	స్థితి IPC_STATE_మార్గం_NOT_దొరికింది:
		/* give child more time */
		తిరిగి 1;

	అప్రమేయ:
	స్థితి IPC_STATE_INVALID_మార్గం:
	స్థితి IPC_STATE__OTHER_ERROR:
		/* all the time in world won't help */
		తిరిగి -1;
	}
}

స్థిర పూర్ణాంకం try_గమ్యం_ప్రారంభం_background_daemon(శూన్య)
{
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
	గణన ప్రారంభం_bg_ఫలితం sbgr;

	/*
	 * Before we try to create a background daemon process, see
	 * if a daemon process is already listening.  This makes it
	 * easier for us to report an already-listening error to the
	 * console, since our spawn/daemon can only report the success
	 * of creating the background process (and not whether it
	 * immediately exited).
	 */
	ఒకవేళ (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	ఒకవేళ (fsmonitor__announce_startup) {
		fprintf(stderr, _("starting fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		fflush(stderr);
	}

	cp.git_cmd = 1;

	strvec_నెట్టు(&cp.వాదనలు, "fsmonitor--daemon");
	strvec_నెట్టు(&cp.వాదనలు, "run");
	strvec_నెట్టు(&cp.వాదనలు, "--detach");
	strvec_pushf(&cp.వాదనలు, "--ipc-threads=%d", fsmonitor_ipc_దారాలు);

	cp.no_stdin = 1;
	cp.no_stdout = 1;
	cp.no_stderr = 1;
	cp.close_fd_above_stderr = 1;

	sbgr = ప్రారంభం_bg_command(&cp, bg_wait_cb, ఖాళీ,
				fsmonitor_ప్రారంభం_timeout_sec);

	ఎంపిక (sbgr) {
	స్థితి SBGR_READY:
		తిరిగి 0;

	అప్రమేయ:
	స్థితి SBGR_ERROR:
	స్థితి SBGR_CB_ERROR:
		తిరిగి error(_("daemon failed to start"));

	స్థితి SBGR_TIMEOUT:
		తిరిగి error(_("daemon not online yet"));

	స్థితి SBGR_DIED:
		తిరిగి error(_("daemon terminated"));
	}
}

పూర్ణాంకం cmd_fsmonitor__daemon(పూర్ణాంకం argc,
			  స్థిరరాశి అక్షరం **argv,
			  స్థిరరాశి అక్షరం *ఉపసర్గ,
			  నిర్మాణం repository *repo వాడని)
{
	స్థిరరాశి అక్షరం *subcmd;
	గణన fsmonitor_reason reason;
	పూర్ణాంకం detach_console = 0;

	నిర్మాణం option options[] = {
		OPT_BOOL(0, "detach", &detach_console, N_("detach from console")),
		OPT_INTEGER(0, "ipc-threads",
			    &fsmonitor_ipc_దారాలు,
			    N_("use <n> ipc worker threads")),
		OPT_INTEGER(0, "start-timeout",
			    &fsmonitor_ప్రారంభం_timeout_sec,
			    N_("max seconds to wait for background daemon startup")),

		OPT_చివర()
	};

	repo_config(the_repository, fsmonitor_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_fsmonitor__daemon_usage, 0);
	ఒకవేళ (argc != 1)
		usage_with_options(builtin_fsmonitor__daemon_usage, options);
	subcmd = argv[0];

	ఒకవేళ (fsmonitor_ipc_దారాలు < 1)
		die(_("invalid 'ipc-threads' value (%d)"),
		    fsmonitor_ipc_దారాలు);

	prepare_repo_settings(the_repository);
	/*
	 * If the repo is fsmonitor-compatible, explicitly set IPC-mode
	 * (without bothering to load the `core.fsmonitor` config settings).
	 *
	 * If the repo is not compatible, the repo-settings will be set to
	 * incompatible rather than IPC, so we can use one of the __get
	 * routines to detect the discrepancy.
	 */
	fsm_settings_సమితి_ipc(the_repository);

	reason = fsm_settings__get_reason(the_repository);
	ఒకవేళ (reason > FSMONITOR_REASON_OK)
		die("%s",
		    fsm_settings_get_incompatible_సందేశం(the_repository,
						       reason));

	ఒకవేళ (!strcmp(subcmd, "start"))
		తిరిగి !!try_గమ్యం_ప్రారంభం_background_daemon();

	ఒకవేళ (!strcmp(subcmd, "run"))
		తిరిగి !!try_గమ్యం_నిరంతరం_foreground_daemon(detach_console);

	ఒకవేళ (!strcmp(subcmd, "stop"))
		తిరిగి !!do_as_client__send_stop();

	ఒకవేళ (!strcmp(subcmd, "status"))
		తిరిగి !!do_as_client_స్థితి();

	die(_("Unhandled subcommand '%s'"), subcmd);
}

#లేకపోతే
పూర్ణాంకం cmd_fsmonitor__daemon(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ వాడని, నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option options[] = {
		OPT_చివర()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_fsmonitor__daemon_usage, options);

	die(_("fsmonitor--daemon not supported on this platform"));
}
#అయితే_ముగింపు
