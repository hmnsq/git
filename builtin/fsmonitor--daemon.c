#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "config.h"
#समावेश "dir.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "parse-options.h"
#समावेश "fsmonitor-ll.h"
#समावेश "fsmonitor-ipc.h"
#समावेश "fsmonitor-settings.h"
#समावेश "compat/fsmonitor/fsm-health.h"
#समावेश "compat/fsmonitor/fsm-listen.h"
#समावेश "fsmonitor--daemon.h"

#समावेश "simple-ipc.h"
#समावेश "strmap.h"
#समावेश "run-command.h"
#समावेश "trace.h"
#समावेश "trace2.h"

अटल स्थिर अक्षर * स्थिर builtin_fsmonitor__daemon_usage[] = {
	N_("git fsmonitor--daemon start [<options>]"),
	N_("git fsmonitor--daemon run [<options>]"),
	"git fsmonitor--daemon stop",
	"git fsmonitor--daemon status",
	शून्य
};

#अगर_घोषित HAVE_FSMONITOR_DAEMON_BACKEND
/*
 * Global state loaded from config.
 */
#घोषणा FSMONITOR__IPC_THREADS "fsmonitor.ipcthreads"
अटल पूर्णांक fsmonitor__ipc_threads = 8;

#घोषणा FSMONITOR__START_TIMEOUT "fsmonitor.starttimeout"
अटल पूर्णांक fsmonitor__start_timeout_sec = 60;

#घोषणा FSMONITOR__ANNOUNCE_STARTUP "fsmonitor.announcestartup"
अटल पूर्णांक fsmonitor__announce_startup = 0;

अटल पूर्णांक fsmonitor_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			    स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(var, FSMONITOR__IPC_THREADS)) {
		पूर्णांक i = git_config_int(var, मूल्य, ctx->kvi);
		अगर (i < 1)
			वापस error(_("value of '%s' out of range: %d"),
				     FSMONITOR__IPC_THREADS, i);
		fsmonitor__ipc_threads = i;
		वापस 0;
	}

	अगर (!म_भेद(var, FSMONITOR__START_TIMEOUT)) {
		पूर्णांक i = git_config_int(var, मूल्य, ctx->kvi);
		अगर (i < 0)
			वापस error(_("value of '%s' out of range: %d"),
				     FSMONITOR__START_TIMEOUT, i);
		fsmonitor__start_timeout_sec = i;
		वापस 0;
	}

	अगर (!म_भेद(var, FSMONITOR__ANNOUNCE_STARTUP)) {
		पूर्णांक is_bool;
		पूर्णांक i = git_config_bool_or_int(var, मूल्य, ctx->kvi, &is_bool);
		अगर (i < 0)
			वापस error(_("value of '%s' not bool or int: %d"),
				     var, i);
		fsmonitor__announce_startup = i;
		वापस 0;
	}

	वापस git_default_config(var, मूल्य, ctx, cb);
}

/*
 * Acting as a CLIENT.
 *
 * Send a "quit" command to the `git-fsmonitor--daemon` (if running)
 * and wait for it to shutdown.
 */
अटल पूर्णांक do_as_client__send_stop(व्योम)
{
	काष्ठा strbuf answer = STRBUF_INIT;
	पूर्णांक ret;
	पूर्णांक max_wait_ms = 30000;
	पूर्णांक elapsed_ms = 0;

	ret = fsmonitor_ipc__send_command("quit", &answer);

	/* The quit command does not return any response data. */
	strbuf_release(&answer);

	अगर (ret)
		वापस ret;

	trace2_region_enter("fsm_client", "polling-for-daemon-exit", शून्य);
	जबतक (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING) {
		अगर (elapsed_ms >= max_wait_ms) {
			trace2_region_leave("fsm_client",
					    "polling-for-daemon-exit", शून्य);
			वापस error(_("daemon did not stop within %d seconds"),
				     max_wait_ms / 1000);
		}
		sleep_millisec(50);
		elapsed_ms += 50;
	}
	trace2_region_leave("fsm_client", "polling-for-daemon-exit", शून्य);

	वापस 0;
}

अटल पूर्णांक do_as_client__status(व्योम)
{
	क्रमागत ipc_active_state state = fsmonitor_ipc__get_state();

	चयन (state) {
	हाल IPC_STATE__LISTENING:
		म_लिखो(_("fsmonitor-daemon is watching '%s'\n"),
		       the_repository->worktree);
		वापस 0;

	शेष:
		म_लिखो(_("fsmonitor-daemon is not watching '%s'\n"),
		       the_repository->worktree);
		वापस 1;
	}
}

क्रमागत fsmonitor_cookie_item_परिणाम {
	FCIR_ERROR = -1, /* could not create cookie file ? */
	FCIR_INIT,
	FCIR_SEEN,
	FCIR_ABORT,
};

काष्ठा fsmonitor_cookie_item {
	काष्ठा hashmap_entry entry;
	अक्षर *name;
	क्रमागत fsmonitor_cookie_item_परिणाम परिणाम;
};

अटल पूर्णांक cookies_cmp(स्थिर व्योम *तथ्य UNUSED,
		       स्थिर काष्ठा hashmap_entry *he1,
		       स्थिर काष्ठा hashmap_entry *he2, स्थिर व्योम *keydata)
{
	स्थिर काष्ठा fsmonitor_cookie_item *a =
		container_of(he1, स्थिर काष्ठा fsmonitor_cookie_item, entry);
	स्थिर काष्ठा fsmonitor_cookie_item *b =
		container_of(he2, स्थिर काष्ठा fsmonitor_cookie_item, entry);

	वापस म_भेद(a->name, keydata ? keydata : b->name);
}

अटल क्रमागत fsmonitor_cookie_item_परिणाम with_lock__wait_for_cookie(
	काष्ठा fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	पूर्णांक fd;
	काष्ठा fsmonitor_cookie_item *cookie;
	काष्ठा strbuf cookie_pathname = STRBUF_INIT;
	काष्ठा strbuf cookie_filename = STRBUF_INIT;
	क्रमागत fsmonitor_cookie_item_परिणाम परिणाम;
	पूर्णांक my_cookie_seq;

	CALLOC_ARRAY(cookie, 1);

	my_cookie_seq = state->cookie_seq++;

	strbuf_addf(&cookie_filename, "%i-%i", getpid(), my_cookie_seq);

	strbuf_addbuf(&cookie_pathname, &state->path_cookie_prefix);
	strbuf_addbuf(&cookie_pathname, &cookie_filename);

	cookie->name = strbuf_detach(&cookie_filename, शून्य);
	cookie->परिणाम = FCIR_INIT;
	hashmap_entry_init(&cookie->entry, strhash(cookie->name));

	hashmap_add(&state->cookies, &cookie->entry);

	trace_printf_कुंजी(&trace_fsmonitor, "cookie-wait: '%s' '%s'",
			 cookie->name, cookie_pathname.buf);

	/*
	 * Create the cookie file on disk and then wait for a notification
	 * that the listener thread has seen it.
	 */
	fd = खोलो(cookie_pathname.buf, O_WRONLY | O_CREAT | O_EXCL, 0600);
	अगर (fd < 0) {
		error_errno(_("could not create fsmonitor cookie '%s'"),
			    cookie->name);

		cookie->परिणाम = FCIR_ERROR;
		जाओ done;
	}

	/*
	 * Technically, close() and unlink() can fail, but we don't
	 * care here.  We only created the file to trigger a watch
	 * event from the FS to know that when we're up to date.
	 */
	बंद(fd);
	unlink(cookie_pathname.buf);

	/*
	 * Wait for the listener thread to observe the cookie file.
	 * Time out after a short interval so that the client
	 * does not hang forever if the filesystem does not deliver
	 * events (e.g., on certain container/overlay filesystems
	 * where inotify watches succeed but events never arrive).
	 */
	{
		काष्ठा timeval now;
		काष्ठा timespec ts;
		पूर्णांक err = 0;

		gettimeofday(&now, शून्य);
		ts.tv_sec = now.tv_sec + 1;
		ts.tv_nsec = now.tv_usec * 1000;

		जबतक (cookie->परिणाम == FCIR_INIT && !err)
			err = pthread_cond_timedwait(&state->cookies_cond,
						     &state->main_lock,
						     &ts);
		अगर (err == ETIMEDOUT && cookie->परिणाम == FCIR_INIT) {
			trace_printf_कुंजी(&trace_fsmonitor,
					 "cookie_wait timed out");
			cookie->परिणाम = FCIR_ERROR;
		}
	}

done:
	hashmap_remove(&state->cookies, &cookie->entry, शून्य);

	परिणाम = cookie->परिणाम;

	मुक्त(cookie->name);
	मुक्त(cookie);
	strbuf_release(&cookie_pathname);

	वापस परिणाम;
}

/*
 * Mark these cookies as _SEEN and wake up the corresponding client threads.
 */
अटल व्योम with_lock__mark_cookies_seen(काष्ठा fsmonitor_daemon_state *state,
					 स्थिर काष्ठा string_list *cookie_names)
{
	/* assert current thread holding state->main_lock */

	पूर्णांक k;
	पूर्णांक nr_seen = 0;

	क्रम (k = 0; k < cookie_names->nr; k++) {
		काष्ठा fsmonitor_cookie_item कुंजी;
		काष्ठा fsmonitor_cookie_item *cookie;

		कुंजी.name = cookie_names->वस्तुएँ[k].string;
		hashmap_entry_init(&कुंजी.entry, strhash(कुंजी.name));

		cookie = hashmap_get_entry(&state->cookies, &कुंजी, entry, शून्य);
		अगर (cookie) {
			trace_printf_कुंजी(&trace_fsmonitor, "cookie-seen: '%s'",
					 cookie->name);
			cookie->परिणाम = FCIR_SEEN;
			nr_seen++;
		}
	}

	अगर (nr_seen)
		pthread_cond_broadcast(&state->cookies_cond);
}

/*
 * Set _ABORT on all pending cookies and wake up all client threads.
 */
अटल व्योम with_lock__abort_all_cookies(काष्ठा fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	काष्ठा hashmap_iter iter;
	काष्ठा fsmonitor_cookie_item *cookie;
	पूर्णांक nr_aborted = 0;

	hashmap_for_each_entry(&state->cookies, &iter, cookie, entry) {
		trace_printf_कुंजी(&trace_fsmonitor, "cookie-abort: '%s'",
				 cookie->name);
		cookie->परिणाम = FCIR_ABORT;
		nr_aborted++;
	}

	अगर (nr_aborted)
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
काष्ठा fsmonitor_token_तथ्य {
	काष्ठा strbuf token_id;
	काष्ठा fsmonitor_batch *batch_शीर्ष;
	काष्ठा fsmonitor_batch *batch_पुच्छ;
	uint64_t client_ref_गिनती;
};

काष्ठा fsmonitor_batch {
	काष्ठा fsmonitor_batch *अग्र;
	uint64_t batch_seq_nr;
	स्थिर अक्षर **interned_paths;
	माप_प्रकार nr, alloc;
	समय_प्रकार pinned_time;
};

अटल काष्ठा fsmonitor_token_तथ्य *fsmonitor_new_token_तथ्य(व्योम)
{
	अटल पूर्णांक test_env_मूल्य = -1;
	अटल uint64_t flush_गिनती = 0;
	काष्ठा fsmonitor_token_तथ्य *token;
	काष्ठा fsmonitor_batch *batch;

	CALLOC_ARRAY(token, 1);
	batch = fsmonitor_batch__new();

	strbuf_init(&token->token_id, 0);
	token->batch_शीर्ष = batch;
	token->batch_पुच्छ = batch;
	token->client_ref_गिनती = 0;

	अगर (test_env_मूल्य < 0)
		test_env_मूल्य = git_env_bool("GIT_TEST_FSMONITOR_TOKEN", 0);

	अगर (!test_env_मूल्य) {
		काष्ठा timeval tv;
		काष्ठा पंचांग पंचांग;
		समय_प्रकार secs;

		gettimeofday(&tv, शून्य);
		secs = tv.tv_sec;
		gmtime_r(&secs, &पंचांग);

		strbuf_addf(&token->token_id,
			    "%"PRIu64".%d.%4d%02d%02dT%02d%02d%02d.%06ldZ",
			    flush_गिनती++,
			    getpid(),
			    पंचांग.tm_year + 1900, पंचांग.tm_mon + 1, पंचांग.tm_mday,
			    पंचांग.tm_hour, पंचांग.tm_min, पंचांग.tm_sec,
			    (दीर्घ)tv.tv_usec);
	} अन्यथा {
		strbuf_addf(&token->token_id, "test_%08x", test_env_मूल्य++);
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
	अगर (test_env_मूल्य)
		batch->pinned_time = समय(शून्य);

	वापस token;
}

काष्ठा fsmonitor_batch *fsmonitor_batch__new(व्योम)
{
	काष्ठा fsmonitor_batch *batch;

	CALLOC_ARRAY(batch, 1);

	वापस batch;
}

व्योम fsmonitor_batch__free_list(काष्ठा fsmonitor_batch *batch)
{
	जबतक (batch) {
		काष्ठा fsmonitor_batch *अग्र = batch->अग्र;

		/*
		 * The actual strings within the array of this batch
		 * are interned, so we don't own them.  We only own
		 * the array.
		 */
		मुक्त(batch->interned_paths);
		मुक्त(batch);

		batch = अग्र;
	}
}

व्योम fsmonitor_batch__add_path(काष्ठा fsmonitor_batch *batch,
			       स्थिर अक्षर *path)
{
	स्थिर अक्षर *interned_path = strintern(path);

	trace_printf_कुंजी(&trace_fsmonitor, "event: %s", interned_path);

	ALLOC_GROW(batch->interned_paths, batch->nr + 1, batch->alloc);
	batch->interned_paths[batch->nr++] = interned_path;
}

अटल व्योम fsmonitor_batch__combine(काष्ठा fsmonitor_batch *batch_dest,
				     स्थिर काष्ठा fsmonitor_batch *batch_src)
{
	माप_प्रकार k;

	ALLOC_GROW(batch_dest->interned_paths,
		   batch_dest->nr + batch_src->nr + 1,
		   batch_dest->alloc);

	क्रम (k = 0; k < batch_src->nr; k++)
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
#घोषणा MY_TIME_DELAY_SECONDS (5 * 60) /* seconds */

अटल काष्ठा fsmonitor_batch *with_lock__truncate_old_batches(
	काष्ठा fsmonitor_daemon_state *state,
	स्थिर काष्ठा fsmonitor_batch *batch_marker)
{
	/* assert current thread holding state->main_lock */

	स्थिर काष्ठा fsmonitor_batch *batch;
	काष्ठा fsmonitor_batch *remainder;

	अगर (!batch_marker)
		वापस शून्य;

	trace_printf_कुंजी(&trace_fsmonitor, "Truncate: mark (%"PRIu64",%"PRIu64")",
			 batch_marker->batch_seq_nr,
			 (uint64_t)batch_marker->pinned_time);

	क्रम (batch = batch_marker; batch; batch = batch->अग्र) {
		समय_प्रकार t;

		अगर (!batch->pinned_time) /* an overflow batch */
			जारी;

		t = batch->pinned_time + MY_TIME_DELAY_SECONDS;
		अगर (t > batch_marker->pinned_time) /* too close to marker */
			जारी;

		जाओ truncate_past_here;
	}

	वापस शून्य;

truncate_past_here:
	state->current_token_तथ्य->batch_पुच्छ = (काष्ठा fsmonitor_batch *)batch;

	remainder = ((काष्ठा fsmonitor_batch *)batch)->अग्र;
	((काष्ठा fsmonitor_batch *)batch)->अग्र = शून्य;

	वापस remainder;
}

अटल व्योम fsmonitor_free_token_तथ्य(काष्ठा fsmonitor_token_तथ्य *token)
{
	अगर (!token)
		वापस;

	निश्चित(token->client_ref_गिनती == 0);

	strbuf_release(&token->token_id);

	fsmonitor_batch__free_list(token->batch_शीर्ष);

	मुक्त(token);
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
अटल व्योम with_lock__do_force_resync(काष्ठा fsmonitor_daemon_state *state)
{
	/* assert current thread holding state->main_lock */

	काष्ठा fsmonitor_token_तथ्य *free_me = शून्य;
	काष्ठा fsmonitor_token_तथ्य *new_one = शून्य;

	new_one = fsmonitor_new_token_तथ्य();

	अगर (state->current_token_तथ्य->client_ref_गिनती == 0)
		free_me = state->current_token_तथ्य;
	state->current_token_तथ्य = new_one;

	fsmonitor_free_token_तथ्य(free_me);

	with_lock__abort_all_cookies(state);
}

व्योम fsmonitor_force_resync(काष्ठा fsmonitor_daemon_state *state)
{
	pthread_mutex_lock(&state->main_lock);
	with_lock__do_force_resync(state);
	pthread_mutex_unlock(&state->main_lock);
}

/*
 * Format an opaque token string to send to the client.
 */
अटल व्योम with_lock__format_response_token(
	काष्ठा strbuf *response_token,
	स्थिर काष्ठा strbuf *response_token_id,
	स्थिर काष्ठा fsmonitor_batch *batch)
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
अटल पूर्णांक fsmonitor_parse_client_token(स्थिर अक्षर *buf_token,
					काष्ठा strbuf *requested_token_id,
					uint64_t *seq_nr)
{
	स्थिर अक्षर *p;
	अक्षर *p_end;

	strbuf_reset(requested_token_id);
	*seq_nr = 0;

	अगर (!skip_prefix(buf_token, "builtin:", &p))
		वापस -1;

	जबतक (*p && *p != ':')
		strbuf_addch(requested_token_id, *p++);
	अगर (!*p++)
		वापस -1;

	*seq_nr = (uint64_t)strtoumax(p, &p_end, 10);
	अगर (*p_end)
		वापस -1;

	वापस 0;
}

अटल पूर्णांक do_handle_client(काष्ठा fsmonitor_daemon_state *state,
			    स्थिर अक्षर *command,
			    ipc_server_reply_cb *reply,
			    काष्ठा ipc_server_reply_तथ्य *reply_तथ्य)
{
	काष्ठा fsmonitor_token_तथ्य *token_तथ्य = शून्य;
	काष्ठा strbuf response_token = STRBUF_INIT;
	काष्ठा strbuf requested_token_id = STRBUF_INIT;
	काष्ठा strbuf payload = STRBUF_INIT;
	uint64_t requested_oldest_seq_nr = 0;
	uint64_t योग_response_len = 0;
	स्थिर अक्षर *p;
	स्थिर काष्ठा fsmonitor_batch *batch_शीर्ष;
	स्थिर काष्ठा fsmonitor_batch *batch;
	काष्ठा fsmonitor_batch *remainder = शून्य;
	intmax_t गिनती = 0, duplicates = 0;
	काष्ठा strset shown = STRSET_INIT;
	पूर्णांक do_trivial = 0;
	पूर्णांक do_flush = 0;
	पूर्णांक do_cookie = 0;
	क्रमागत fsmonitor_cookie_item_परिणाम cookie_परिणाम;

	/*
	 * We expect `command` to be of the form:
	 *
	 * <command> := quit NUL
	 *            | flush NUL
	 *            | <V1-time-since-epoch-ns> NUL
	 *            | <V2-opaque-fsmonitor-token> NUL
	 */

	अगर (!म_भेद(command, "quit")) {
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
		वापस SIMPLE_IPC_QUIT;

	} अन्यथा अगर (!म_भेद(command, "flush")) {
		/*
		 * Flush all of our cached data and generate a new token
		 * just like if we lost sync with the filesystem.
		 *
		 * Then send a trivial response using the new token.
		 */
		do_flush = 1;
		do_trivial = 1;

	} अन्यथा अगर (!skip_prefix(command, "builtin:", &p)) {
		/* assume V1 timestamp or garbage */

		अक्षर *p_end;

		strtoumax(command, &p_end, 10);
		trace_printf_कुंजी(&trace_fsmonitor,
				 ((*p_end) ?
				  "fsmonitor: invalid command line '%s'" :
				  "fsmonitor: unsupported V1 protocol '%s'"),
				 command);
		do_trivial = 1;
		do_cookie = 1;

	} अन्यथा {
		/* We have "builtin:*" */
		अगर (fsmonitor_parse_client_token(command, &requested_token_id,
						 &requested_oldest_seq_nr)) {
			trace_printf_कुंजी(&trace_fsmonitor,
					 "fsmonitor: invalid V2 protocol token '%s'",
					 command);
			do_trivial = 1;
			do_cookie = 1;

		} अन्यथा {
			/*
			 * We have a V2 valid token:
			 *     "builtin:<token_id>:<seq_nr>"
			 */
			do_cookie = 1;
		}
	}

	pthread_mutex_lock(&state->main_lock);

	अगर (!state->current_token_तथ्य)
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
	अगर (do_cookie) {
		cookie_परिणाम = with_lock__wait_for_cookie(state);
		अगर (cookie_परिणाम != FCIR_SEEN) {
			error(_("fsmonitor: cookie_result '%d' != SEEN"),
			      cookie_परिणाम);
			do_trivial = 1;
		}
	}

	अगर (do_flush)
		with_lock__do_force_resync(state);

	/*
	 * We mark the current head of the batch list as "pinned" so
	 * that the listener thread will treat this item as read-only
	 * (and prevent any more paths from being added to it) from
	 * now on.
	 */
	token_तथ्य = state->current_token_तथ्य;
	batch_शीर्ष = token_तथ्य->batch_शीर्ष;
	((काष्ठा fsmonitor_batch *)batch_शीर्ष)->pinned_time = समय(शून्य);

	/*
	 * FSMonitor Protocol V2 requires that we send a response header
	 * with a "new current token" and then all of the paths that changed
	 * since the "requested token".  We send the seq_nr of the just-pinned
	 * head batch so that future requests from a client will be relative
	 * to it.
	 */
	with_lock__format_response_token(&response_token,
					 &token_तथ्य->token_id, batch_शीर्ष);

	reply(reply_तथ्य, response_token.buf, response_token.len + 1);
	योग_response_len += response_token.len + 1;

	trace2_तथ्य_string("fsmonitor", the_repository, "response/token",
			   response_token.buf);
	trace_printf_कुंजी(&trace_fsmonitor, "response token: %s",
			 response_token.buf);

	अगर (!do_trivial) {
		अगर (म_भेद(requested_token_id.buf, token_तथ्य->token_id.buf)) {
			/*
			 * The client last spoke to a different daemon
			 * instance -OR- the daemon had to resync with
			 * the filesystem (and lost events), so reject.
			 */
			trace2_तथ्य_string("fsmonitor", the_repository,
					   "response/token", "different");
			do_trivial = 1;

		} अन्यथा अगर (requested_oldest_seq_nr <
			   token_तथ्य->batch_पुच्छ->batch_seq_nr) {
			/*
			 * The client wants older events than we have for
			 * this token_id.  This means that the end of our
			 * batch list was truncated and we cannot give the
			 * client a complete snapshot relative to their
			 * request.
			 */
			trace_printf_कुंजी(&trace_fsmonitor,
					 "client requested truncated data");
			do_trivial = 1;
		}
	}

	अगर (do_trivial) {
		pthread_mutex_unlock(&state->main_lock);

		reply(reply_तथ्य, "/", 2);

		trace2_तथ्य_intmax("fsmonitor", the_repository,
				   "response/trivial", 1);

		जाओ cleanup;
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
	token_तथ्य->client_ref_गिनती++;

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
	strset_init_with_options(&shown, शून्य, 0);
	क्रम (batch = batch_शीर्ष;
	     batch && batch->batch_seq_nr > requested_oldest_seq_nr;
	     batch = batch->अग्र) {
		माप_प्रकार k;

		क्रम (k = 0; k < batch->nr; k++) {
			स्थिर अक्षर *s = batch->interned_paths[k];
			माप_प्रकार s_len;

			अगर (!strset_add(&shown, s))
				duplicates++;
			अन्यथा {
				trace_printf_कुंजी(&trace_fsmonitor,
						 "send[%"PRIuMAX"]: %s",
						 गिनती, s);

				/* Each path gets written with a trailing NUL */
				s_len = म_माप(s) + 1;

				अगर (payload.len + s_len >=
				    LARGE_PACKET_तथ्य_MAX) {
					reply(reply_तथ्य, payload.buf,
					      payload.len);
					योग_response_len += payload.len;
					strbuf_reset(&payload);
				}

				strbuf_add(&payload, s, s_len);
				गिनती++;
			}
		}
	}

	अगर (payload.len) {
		reply(reply_तथ्य, payload.buf, payload.len);
		योग_response_len += payload.len;
	}

	pthread_mutex_lock(&state->main_lock);

	अगर (token_तथ्य->client_ref_गिनती > 0)
		token_तथ्य->client_ref_गिनती--;

	अगर (token_तथ्य->client_ref_गिनती == 0) {
		अगर (token_तथ्य != state->current_token_तथ्य) {
			/*
			 * The listener thread did a token-reset while we were
			 * walking the batch list.  Therefore, this token is
			 * stale and can be discarded completely.  If we are
			 * the last reader thread using this token, we own
			 * that work.
			 */
			fsmonitor_free_token_तथ्य(token_तथ्य);
		} अन्यथा अगर (batch) {
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

	अगर (remainder)
		fsmonitor_batch__free_list(remainder);

	trace2_तथ्य_intmax("fsmonitor", the_repository, "response/length", योग_response_len);
	trace2_तथ्य_intmax("fsmonitor", the_repository, "response/count/files", गिनती);
	trace2_तथ्य_intmax("fsmonitor", the_repository, "response/count/duplicates", duplicates);

cleanup:
	strset_clear(&shown);
	strbuf_release(&response_token);
	strbuf_release(&requested_token_id);
	strbuf_release(&payload);

	वापस 0;
}

अटल ipc_server_application_cb handle_client;

अटल पूर्णांक handle_client(व्योम *तथ्य,
			 स्थिर अक्षर *command, माप_प्रकार command_len,
			 ipc_server_reply_cb *reply,
			 काष्ठा ipc_server_reply_तथ्य *reply_तथ्य)
{
	काष्ठा fsmonitor_daemon_state *state = तथ्य;
	पूर्णांक परिणाम;

	/*
	 * The Simple IPC API now supports {char*, len} arguments, but
	 * FSMonitor always uses proper null-terminated strings, so
	 * we can ignore the command_len argument.  (Trust, but verify.)
	 */
	अगर (command_len != म_माप(command))
		BUG("FSMonitor assumes text messages");

	trace_printf_कुंजी(&trace_fsmonitor, "requested token: %s", command);

	trace2_region_enter("fsmonitor", "handle_client", the_repository);
	trace2_तथ्य_string("fsmonitor", the_repository, "request", command);

	परिणाम = do_handle_client(state, command, reply, reply_तथ्य);

	trace2_region_leave("fsmonitor", "handle_client", the_repository);

	वापस परिणाम;
}

#घोषणा FSMONITOR_DIR           "fsmonitor--daemon"
#घोषणा FSMONITOR_COOKIE_DIR    "cookies"
#घोषणा FSMONITOR_COOKIE_PREFIX (FSMONITOR_DIR "/" FSMONITOR_COOKIE_DIR "/")

क्रमागत fsmonitor_path_type fsmonitor_classify_path_workdir_relative(
	स्थिर अक्षर *rel)
{
	अगर (fspathncmp(rel, ".git", 4))
		वापस IS_WORKDIR_PATH;
	rel += 4;

	अगर (!*rel)
		वापस IS_DOT_GIT;
	अगर (*rel != '/')
		वापस IS_WORKDIR_PATH; /* e.g. .gitignore */
	rel++;

	अगर (!fspathncmp(rel, FSMONITOR_COOKIE_PREFIX,
			म_माप(FSMONITOR_COOKIE_PREFIX)))
		वापस IS_INSIDE_DOT_GIT_WITH_COOKIE_PREFIX;

	वापस IS_INSIDE_DOT_GIT;
}

क्रमागत fsmonitor_path_type fsmonitor_classify_path_gitdir_relative(
	स्थिर अक्षर *rel)
{
	अगर (!fspathncmp(rel, FSMONITOR_COOKIE_PREFIX,
			म_माप(FSMONITOR_COOKIE_PREFIX)))
		वापस IS_INSIDE_GITDIR_WITH_COOKIE_PREFIX;

	वापस IS_INSIDE_GITDIR;
}

अटल क्रमागत fsmonitor_path_type try_classify_workdir_abs_path(
	काष्ठा fsmonitor_daemon_state *state,
	स्थिर अक्षर *path)
{
	स्थिर अक्षर *rel;

	अगर (fspathncmp(path, state->path_worktree_watch.buf,
		       state->path_worktree_watch.len))
		वापस IS_OUTSIDE_CONE;

	rel = path + state->path_worktree_watch.len;

	अगर (!*rel)
		वापस IS_WORKDIR_PATH; /* it is the root dir exactly */
	अगर (*rel != '/')
		वापस IS_OUTSIDE_CONE;
	rel++;

	वापस fsmonitor_classify_path_workdir_relative(rel);
}

क्रमागत fsmonitor_path_type fsmonitor_classify_path_absolute(
	काष्ठा fsmonitor_daemon_state *state,
	स्थिर अक्षर *path)
{
	स्थिर अक्षर *rel;
	क्रमागत fsmonitor_path_type t;

	t = try_classify_workdir_abs_path(state, path);
	अगर (state->nr_paths_watching == 1)
		वापस t;
	अगर (t != IS_OUTSIDE_CONE)
		वापस t;

	अगर (fspathncmp(path, state->path_gitdir_watch.buf,
		       state->path_gitdir_watch.len))
		वापस IS_OUTSIDE_CONE;

	rel = path + state->path_gitdir_watch.len;

	अगर (!*rel)
		वापस IS_GITDIR; /* it is the <gitdir> exactly */
	अगर (*rel != '/')
		वापस IS_OUTSIDE_CONE;
	rel++;

	वापस fsmonitor_classify_path_gitdir_relative(rel);
}

/*
 * We try to combine small batches at the front of the batch-list to avoid
 * having a long list.  This hopefully makes it a little easier when we want
 * to truncate and maintain the list.  However, we don't want the paths array
 * to just keep growing and growing with realloc, so we insert an arbitrary
 * limit.
 */
#घोषणा MY_COMBINE_LIMIT (1024)

व्योम fsmonitor_publish(काष्ठा fsmonitor_daemon_state *state,
		       काष्ठा fsmonitor_batch *batch,
		       स्थिर काष्ठा string_list *cookie_names)
{
	अगर (!batch && !cookie_names->nr)
		वापस;

	pthread_mutex_lock(&state->main_lock);

	अगर (batch) {
		काष्ठा fsmonitor_batch *शीर्ष;

		शीर्ष = state->current_token_तथ्य->batch_शीर्ष;
		अगर (!शीर्ष) {
			BUG("token does not have batch");
		} अन्यथा अगर (शीर्ष->pinned_time) {
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
			batch->batch_seq_nr = शीर्ष->batch_seq_nr + 1;
			batch->अग्र = शीर्ष;
			state->current_token_तथ्य->batch_शीर्ष = batch;
		} अन्यथा अगर (!शीर्ष->batch_seq_nr) {
			/*
			 * Batch 0 is unpinned.  See the note in
			 * `fsmonitor_new_token_data()` about why we
			 * don't need to accumulate these paths.
			 */
			fsmonitor_batch__free_list(batch);
		} अन्यथा अगर (शीर्ष->nr + batch->nr > MY_COMBINE_LIMIT) {
			/*
			 * The head batch in the list has never been
			 * transmitted to a client, but folding the
			 * contents of the new batch onto it would
			 * exceed our arbitrary limit, so just prepend
			 * the new batch onto the list.
			 */
			batch->batch_seq_nr = शीर्ष->batch_seq_nr + 1;
			batch->अग्र = शीर्ष;
			state->current_token_तथ्य->batch_शीर्ष = batch;
		} अन्यथा {
			/*
			 * We are free to add the paths in the given
			 * batch onto the end of the current head batch.
			 */
			fsmonitor_batch__combine(शीर्ष, batch);
			fsmonitor_batch__free_list(batch);
		}
	}

	अगर (cookie_names->nr)
		with_lock__mark_cookies_seen(state, cookie_names);

	pthread_mutex_unlock(&state->main_lock);
}

अटल व्योम *fsm_health__thread_proc(व्योम *_state)
{
	काष्ठा fsmonitor_daemon_state *state = _state;

	trace2_thread_start("fsm-health");

	fsm_health__loop(state);

	trace2_thread_exit();
	वापस शून्य;
}

अटल व्योम *fsm_listen__thread_proc(व्योम *_state)
{
	काष्ठा fsmonitor_daemon_state *state = _state;

	trace2_thread_start("fsm-listen");

	trace_printf_कुंजी(&trace_fsmonitor, "Watching: worktree '%s'",
			 state->path_worktree_watch.buf);
	अगर (state->nr_paths_watching > 1)
		trace_printf_कुंजी(&trace_fsmonitor, "Watching: gitdir '%s'",
				 state->path_gitdir_watch.buf);

	fsm_listen__loop(state);

	pthread_mutex_lock(&state->main_lock);
	अगर (state->current_token_तथ्य &&
	    state->current_token_तथ्य->client_ref_गिनती == 0)
		fsmonitor_free_token_तथ्य(state->current_token_तथ्य);
	state->current_token_तथ्य = शून्य;
	pthread_mutex_unlock(&state->main_lock);

	trace2_thread_exit();
	वापस शून्य;
}

अटल पूर्णांक fsmonitor_run_daemon_1(काष्ठा fsmonitor_daemon_state *state)
{
	काष्ठा ipc_server_opts ipc_opts = {
		.nr_threads = fsmonitor__ipc_threads,

		/*
		 * We know that there are no other active threads yet,
		 * so we can let the IPC layer temporarily chdir() if
		 * it needs to when creating the server side of the
		 * Unix domain socket.
		 */
		.uds_disallow_chdir = 0
	};
	पूर्णांक health_started = 0;
	पूर्णांक listener_started = 0;
	पूर्णांक err = 0;

	/*
	 * Start the IPC thread pool before the we've started the file
	 * system event listener thread so that we have the IPC handle
	 * before we need it.
	 */
	अगर (ipc_server_init_async(&state->ipc_server_तथ्य,
				  state->path_ipc.buf, &ipc_opts,
				  handle_client, state))
		वापस error_errno(
			_("could not start IPC thread pool on '%s'"),
			state->path_ipc.buf);

	/*
	 * Start the fsmonitor listener thread to collect filesystem
	 * events.
	 */
	अगर (pthread_create(&state->listener_thread, शून्य,
			   fsm_listen__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_तथ्य);
		err = error(_("could not start fsmonitor listener thread"));
		जाओ cleanup;
	}
	listener_started = 1;

	/*
	 * Start the health thread to watch over our process.
	 */
	अगर (pthread_create(&state->health_thread, शून्य,
			   fsm_health__thread_proc, state)) {
		ipc_server_stop_async(state->ipc_server_तथ्य);
		err = error(_("could not start fsmonitor health thread"));
		जाओ cleanup;
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
	ipc_server_await(state->ipc_server_तथ्य);

	/*
	 * The fsmonitor listener thread may have received a shutdown
	 * event from the IPC thread pool, but it doesn't hurt to tell
	 * it again.  And wait for it to shutdown.
	 */
	अगर (listener_started) {
		fsm_listen__stop_async(state);
		pthread_join(state->listener_thread, शून्य);
	}

	अगर (health_started) {
		fsm_health__stop_async(state);
		pthread_join(state->health_thread, शून्य);
	}

	अगर (err)
		वापस err;
	अगर (state->listen_error_code)
		वापस state->listen_error_code;
	अगर (state->health_error_code)
		वापस state->health_error_code;
	वापस 0;
}

अटल पूर्णांक fsmonitor_run_daemon(व्योम)
{
	काष्ठा fsmonitor_daemon_state state;
	स्थिर अक्षर *home;
	पूर्णांक err;

	स_रखो(&state, 0, माप(state));

	hashmap_init(&state.cookies, cookies_cmp, शून्य, 0);
	pthread_mutex_init(&state.main_lock, शून्य);
	pthread_cond_init(&state.cookies_cond, शून्य);
	state.listen_error_code = 0;
	state.health_error_code = 0;
	state.current_token_तथ्य = fsmonitor_new_token_तथ्य();

	/* Prepare to (recursively) watch the <worktree-root> directory. */
	strbuf_init(&state.path_worktree_watch, 0);
	strbuf_addstr(&state.path_worktree_watch,
		      absolute_path(repo_get_work_वृक्ष(the_repository)));
	state.nr_paths_watching = 1;

	strbuf_init(&state.alias.alias, 0);
	strbuf_init(&state.alias.points_to, 0);
	अगर ((err = fsmonitor__get_alias(state.path_worktree_watch.buf, &state.alias)))
		जाओ done;

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
	अगर (!is_directory(state.path_gitdir_watch.buf)) {
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
	सूची_गढ़ो(state.path_cookie_prefix.buf, 0777);

	strbuf_addch(&state.path_cookie_prefix, '/');
	strbuf_addstr(&state.path_cookie_prefix, FSMONITOR_COOKIE_DIR);
	सूची_गढ़ो(state.path_cookie_prefix.buf, 0777);

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
	अगर (fsm_listen__ctor(&state)) {
		err = error(_("could not initialize listener thread"));
		जाओ done;
	}

	अगर (fsm_health__ctor(&state)) {
		err = error(_("could not initialize health thread"));
		जाओ done;
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
	home = दो_पर्या("HOME");
	अगर (home && *home && स_बदलो(home))
		die_errno(_("could not cd home '%s'"), home);

	err = fsmonitor_run_daemon_1(&state);

done:
	fsmonitor_free_token_तथ्य(state.current_token_तथ्य);
	state.current_token_तथ्य = शून्य;
	pthread_cond_destroy(&state.cookies_cond);
	pthread_mutex_destroy(&state.main_lock);
	{
		काष्ठा hashmap_iter iter;
		काष्ठा fsmonitor_cookie_item *cookie;

		hashmap_for_each_entry(&state.cookies, &iter, cookie, entry)
			मुक्त(cookie->name);
		hashmap_clear_and_free(&state.cookies,
				       काष्ठा fsmonitor_cookie_item, entry);
	}
	fsm_listen__dtor(&state);
	fsm_health__dtor(&state);

	ipc_server_free(state.ipc_server_तथ्य);

	strbuf_release(&state.path_worktree_watch);
	strbuf_release(&state.path_gitdir_watch);
	strbuf_release(&state.path_cookie_prefix);
	strbuf_release(&state.path_ipc);
	strbuf_release(&state.alias.alias);
	strbuf_release(&state.alias.points_to);

	वापस err;
}

अटल पूर्णांक try_to_run_foreground_daemon(पूर्णांक detach_console)
{
	/*
	 * Technically, we don't need to probe for an existing daemon
	 * process, since we could just call `fsmonitor_run_daemon()`
	 * and let it fail if the pipe/socket is busy.
	 *
	 * However, this method gives us a nicer error message for a
	 * common error case.
	 */
	अगर (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	अगर (fsmonitor__announce_startup) {
		ख_लिखो(मानक_त्रुटि, _("running fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		ख_साफ(मानक_त्रुटि);
	}

	अगर (detach_console) {
#अगर_घोषित GIT_WINDOWS_NATIVE
		FreeConsole();
#अन्यथा
		/*
		 * Create a new session so that the daemon is detached
		 * from the parent's process group.  This prevents
		 * shells with job control (e.g. bash with "set -m")
		 * from waiting on the daemon when they wait for a
		 * foreground command that implicitly spawned it.
		 */
		अगर (setsid() == -1)
			warning_errno(_("setsid failed"));
#पूर्ण_अगर
	}

	वापस !!fsmonitor_run_daemon();
}

अटल start_bg_wait_cb bg_wait_cb;

अटल पूर्णांक bg_wait_cb(स्थिर काष्ठा child_process *cp UNUSED,
		      व्योम *cb_तथ्य UNUSED)
{
	क्रमागत ipc_active_state s = fsmonitor_ipc__get_state();

	चयन (s) {
	हाल IPC_STATE__LISTENING:
		/* child is "ready" */
		वापस 0;

	हाल IPC_STATE__NOT_LISTENING:
	हाल IPC_STATE__PATH_NOT_FOUND:
		/* give child more time */
		वापस 1;

	शेष:
	हाल IPC_STATE__INVALID_PATH:
	हाल IPC_STATE__OTHER_ERROR:
		/* all the time in world won't help */
		वापस -1;
	}
}

अटल पूर्णांक try_to_start_background_daemon(व्योम)
{
	काष्ठा child_process cp = CHILD_PROCESS_INIT;
	क्रमागत start_bg_परिणाम sbgr;

	/*
	 * Before we try to create a background daemon process, see
	 * if a daemon process is already listening.  This makes it
	 * easier for us to report an already-listening error to the
	 * console, since our spawn/daemon can only report the success
	 * of creating the background process (and not whether it
	 * immediately exited).
	 */
	अगर (fsmonitor_ipc__get_state() == IPC_STATE__LISTENING)
		die(_("fsmonitor--daemon is already running '%s'"),
		    the_repository->worktree);

	अगर (fsmonitor__announce_startup) {
		ख_लिखो(मानक_त्रुटि, _("starting fsmonitor-daemon in '%s'\n"),
			the_repository->worktree);
		ख_साफ(मानक_त्रुटि);
	}

	cp.git_cmd = 1;

	strvec_धकेलो(&cp.args, "fsmonitor--daemon");
	strvec_धकेलो(&cp.args, "run");
	strvec_धकेलो(&cp.args, "--detach");
	strvec_pushf(&cp.args, "--ipc-threads=%d", fsmonitor__ipc_threads);

	cp.no_stdin = 1;
	cp.no_stdout = 1;
	cp.no_stderr = 1;
	cp.close_fd_above_stderr = 1;

	sbgr = start_bg_command(&cp, bg_wait_cb, शून्य,
				fsmonitor__start_timeout_sec);

	चयन (sbgr) {
	हाल SBGR_READY:
		वापस 0;

	शेष:
	हाल SBGR_ERROR:
	हाल SBGR_CB_ERROR:
		वापस error(_("daemon failed to start"));

	हाल SBGR_TIMEOUT:
		वापस error(_("daemon not online yet"));

	हाल SBGR_DIED:
		वापस error(_("daemon terminated"));
	}
}

पूर्णांक cmd_fsmonitor__daemon(पूर्णांक argc,
			  स्थिर अक्षर **argv,
			  स्थिर अक्षर *prefix,
			  काष्ठा repository *repo UNUSED)
{
	स्थिर अक्षर *subcmd;
	क्रमागत fsmonitor_reason reason;
	पूर्णांक detach_console = 0;

	काष्ठा option options[] = {
		OPT_BOOL(0, "detach", &detach_console, N_("detach from console")),
		OPT_INTEGER(0, "ipc-threads",
			    &fsmonitor__ipc_threads,
			    N_("use <n> ipc worker threads")),
		OPT_INTEGER(0, "start-timeout",
			    &fsmonitor__start_timeout_sec,
			    N_("max seconds to wait for background daemon startup")),

		OPT_END()
	};

	repo_config(the_repository, fsmonitor_config, शून्य);

	argc = parse_options(argc, argv, prefix, options,
			     builtin_fsmonitor__daemon_usage, 0);
	अगर (argc != 1)
		usage_with_options(builtin_fsmonitor__daemon_usage, options);
	subcmd = argv[0];

	अगर (fsmonitor__ipc_threads < 1)
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
	अगर (reason > FSMONITOR_REASON_OK)
		die("%s",
		    fsm_settings_get_incompatible_संदेश(the_repository,
						       reason));

	अगर (!म_भेद(subcmd, "start"))
		वापस !!try_to_start_background_daemon();

	अगर (!म_भेद(subcmd, "run"))
		वापस !!try_to_run_foreground_daemon(detach_console);

	अगर (!म_भेद(subcmd, "stop"))
		वापस !!do_as_client__send_stop();

	अगर (!म_भेद(subcmd, "status"))
		वापस !!do_as_client__status();

	die(_("Unhandled subcommand '%s'"), subcmd);
}

#अन्यथा
पूर्णांक cmd_fsmonitor__daemon(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix UNUSED, काष्ठा repository *repo UNUSED)
{
	काष्ठा option options[] = {
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_fsmonitor__daemon_usage, options);

	die(_("fsmonitor--daemon not supported on this platform"));
}
#पूर्ण_अगर
