/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "grep.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pathspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule-config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/source.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oidset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pager.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "promisor-remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *grep_ਅਗੇਤਰ;

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ recurse_submodules;

ਸਥਿਰ ਪੂਰਨ ਅੰਕ_ਧਾਗੇ;

ਸਥਿਰ pthread_t *ਧਾਗੇ;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
ਬਣਤਰ work_ਵਸਤੂ {
	ਬਣਤਰ grep_ਸਰੋਤ ਸਰੋਤ;
	ਅੱਖਰ ਪੂਰਾ;
	ਬਣਤਰ strbuf ਬਾਹਰਲਾ;
};

/* In the range [todo_done, todo_start) in 'todo' we have work_items
 * that have been or are processed by a consumer thread. We haven't
 * written the result for these to stdout yet.
 *
 * The work_items in [todo_start, todo_end) are waiting to be picked
 * up by a consumer thread.
 *
 * The ranges are modulo TODO_SIZE.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TODO_ਮਾਪ 128
ਸਥਿਰ ਬਣਤਰ work_ਵਸਤੂ todo[TODO_ਮਾਪ];
ਸਥਿਰ ਪੂਰਨ todo_ਸ਼ੁਰੂ;
ਸਥਿਰ ਪੂਰਨ todo_ਸਿਰਾ;
ਸਥਿਰ ਪੂਰਨ todo_ਪੂਰਾ;

/* Has all work items been added? */
ਸਥਿਰ ਪੂਰਨ all_work_added;

ਸਥਿਰ ਬਣਤਰ repository **repos_ਮੰਜ਼ਿਲ_free;
ਸਥਿਰ ਮਾਪ_t repos_ਮੰਜ਼ਿਲ_free_nr, repos_ਮੰਜ਼ਿਲ_free_alloc;

/* This lock protects all the variables above. */
ਸਥਿਰ pthread_mutex_t grep_mutex;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ grep_ਤਾਲਾ(ਖਾਲੀ)
{
	pthread_mutex_ਤਾਲਾ(&grep_mutex);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ grep_unlock(ਖਾਲੀ)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
ਸਥਿਰ pthread_cond_t cond_ਜੋੜੋ;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
ਸਥਿਰ pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
ਸਥਿਰ pthread_cond_t cond_ਨਤੀਜਾ;

ਸਥਿਰ ਪੂਰਨ skip_ਪਹਿਲਾ_ਸਤਰ;

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_work(ਬਣਤਰ grep_opt *opt, ਬਣਤਰ grep_ਸਰੋਤ *gs)
{
	ਜੇ (opt->ਦੁਆਧਾਰੀ != GREP_ਦੁਆਧਾਰੀ_ਪਾਠ)
		grep_ਸਰੋਤ_ਲੱਦੋ_driver(gs, opt->repo->ਸੂਚਕਾਂਕ);

	grep_ਤਾਲਾ();

	ਜਦੋਂ ((todo_ਸਿਰਾ+1) % ARRAY_ਮਾਪ(todo) == todo_ਪੂਰਾ) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_ਸਿਰਾ].ਸਰੋਤ = *gs;
	todo[todo_ਸਿਰਾ].ਪੂਰਾ = 0;
	strbuf_reset(&todo[todo_ਸਿਰਾ].ਬਾਹਰਲਾ);
	todo_ਸਿਰਾ = (todo_ਸਿਰਾ + 1) % ARRAY_ਮਾਪ(todo);

	pthread_cond_signal(&cond_ਜੋੜੋ);
	grep_unlock();
}

ਸਥਿਰ ਬਣਤਰ work_ਵਸਤੂ *get_work(ਖਾਲੀ)
{
	ਬਣਤਰ work_ਵਸਤੂ *ret;

	grep_ਤਾਲਾ();
	ਜਦੋਂ (todo_ਸ਼ੁਰੂ == todo_ਸਿਰਾ && !all_work_added) {
		pthread_cond_wait(&cond_ਜੋੜੋ, &grep_mutex);
	}

	ਜੇ (todo_ਸ਼ੁਰੂ == todo_ਸਿਰਾ && all_work_added) {
		ret = ਨਿਰਾ;
	} ਨਹੀਂਤਾਂ {
		ret = &todo[todo_ਸ਼ੁਰੂ];
		todo_ਸ਼ੁਰੂ = (todo_ਸ਼ੁਰੂ + 1) % ARRAY_ਮਾਪ(todo);
	}
	grep_unlock();
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ work_ਪੂਰਾ(ਬਣਤਰ work_ਵਸਤੂ *w)
{
	ਪੂਰਨ old_ਪੂਰਾ;

	grep_ਤਾਲਾ();
	w->ਪੂਰਾ = 1;
	old_ਪੂਰਾ = todo_ਪੂਰਾ;
	ਲਈ(; todo[todo_ਪੂਰਾ].ਪੂਰਾ && todo_ਪੂਰਾ != todo_ਸ਼ੁਰੂ;
	    todo_ਪੂਰਾ = (todo_ਪੂਰਾ+1) % ARRAY_ਮਾਪ(todo)) {
		w = &todo[todo_ਪੂਰਾ];
		ਜੇ (w->ਬਾਹਰਲਾ.len) {
			ਅਚਲ ਅੱਖਰ *p = w->ਬਾਹਰਲਾ.buf;
			ਮਾਪ_t len = w->ਬਾਹਰਲਾ.len;

			/* Skip the leading hunk mark of the first file. */
			ਜੇ (skip_ਪਹਿਲਾ_ਸਤਰ) {
				ਜਦੋਂ (len) {
					len--;
					ਜੇ (*p++ == '\n')
						ਰੋਕੋ;
				}
				skip_ਪਹਿਲਾ_ਸਤਰ = 0;
			}

			write_or_die(1, p, len);
		}
		grep_ਸਰੋਤ_clear(&w->ਸਰੋਤ);
	}

	ਜੇ (old_ਪੂਰਾ != todo_ਪੂਰਾ)
		pthread_cond_signal(&cond_write);

	ਜੇ (all_work_added && todo_ਪੂਰਾ == todo_ਸਿਰਾ)
		pthread_cond_signal(&cond_ਨਤੀਜਾ);

	grep_unlock();
}

ਸਥਿਰ ਖਾਲੀ free_repos(ਖਾਲੀ)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < repos_ਮੰਜ਼ਿਲ_free_nr; i++) {
		repo_clear(repos_ਮੰਜ਼ਿਲ_free[i]);
		ਮੁਕਤ(repos_ਮੰਜ਼ਿਲ_free[i]);
	}
	FREE_AND_NULL(repos_ਮੰਜ਼ਿਲ_free);
	repos_ਮੰਜ਼ਿਲ_free_nr = 0;
	repos_ਮੰਜ਼ਿਲ_free_alloc = 0;
}

ਸਥਿਰ ਖਾਲੀ *ਲੜੀਵਾਰ(ਖਾਲੀ *arg)
{
	ਪੂਰਨ ਪ੍ਰਾਪਤੀ = 0;
	ਬਣਤਰ grep_opt *opt = arg;

	ਜਦੋਂ (1) {
		ਬਣਤਰ work_ਵਸਤੂ *w = get_work();
		ਜੇ (!w)
			ਰੋਕੋ;

		opt->output_priv = w;
		ਪ੍ਰਾਪਤੀ |= grep_ਸਰੋਤ(opt, &w->ਸਰੋਤ);
		grep_ਸਰੋਤ_clear_ਅੰਕੜੇ(&w->ਸਰੋਤ);
		work_ਪੂਰਾ(w);
	}
	free_grep_patterns(opt);
	ਮੁਕਤ(opt);

	ਵਾਪਸ (ਖਾਲੀ*) (intptr_t) ਪ੍ਰਾਪਤੀ;
}

ਸਥਿਰ ਖਾਲੀ strbuf_ਬਾਹਰਲਾ(ਬਣਤਰ grep_opt *opt, ਅਚਲ ਖਾਲੀ *buf, ਮਾਪ_t ਮਾਪ)
{
	ਬਣਤਰ work_ਵਸਤੂ *w = opt->output_priv;
	strbuf_ਜੋੜੋ(&w->ਬਾਹਰਲਾ, buf, ਮਾਪ);
}

ਸਥਿਰ ਖਾਲੀ ਸ਼ੁਰੂ_ਧਾਗੇ(ਬਣਤਰ grep_opt *opt)
{
	ਪੂਰਨ i;

	pthread_mutex_init(&grep_mutex, ਨਿਰਾ);
	pthread_mutex_init(&grep_attr_mutex, ਨਿਰਾ);
	pthread_cond_init(&cond_ਜੋੜੋ, ਨਿਰਾ);
	pthread_cond_init(&cond_write, ਨਿਰਾ);
	pthread_cond_init(&cond_ਨਤੀਜਾ, ਨਿਰਾ);
	grep_use_locks = 1;
	enable_obj_read_ਤਾਲਾ();

	ਲਈ (i = 0; i < ARRAY_ਮਾਪ(todo); i++) {
		strbuf_init(&todo[i].ਬਾਹਰਲਾ, 0);
	}

	CALLOC_ARRAY(ਧਾਗੇ, ਅੰਕ_ਧਾਗੇ);
	ਲਈ (i = 0; i < ਅੰਕ_ਧਾਗੇ; i++) {
		ਪੂਰਨ err;
		ਬਣਤਰ grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_ਬਾਹਰਲਾ;
		compile_grep_patterns(o);
		err = pthread_create(&ਧਾਗੇ[i], ਨਿਰਾ, ਲੜੀਵਾਰ, o);

		ਜੇ (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

ਸਥਿਰ ਪੂਰਨ wait_all(ਖਾਲੀ)
{
	ਪੂਰਨ ਪ੍ਰਾਪਤੀ = 0;
	ਪੂਰਨ i;

	ਜੇ (!HAVE_ਧਾਗੇ)
		BUG("Never call this function unless you have started threads");

	grep_ਤਾਲਾ();
	all_work_added = 1;

	/* Wait until all work is done. */
	ਜਦੋਂ (todo_ਪੂਰਾ != todo_ਸਿਰਾ)
		pthread_cond_wait(&cond_ਨਤੀਜਾ, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_ਪ੍ਰਸਾਰਣ(&cond_ਜੋੜੋ);
	grep_unlock();

	ਲਈ (i = 0; i < ਅੰਕ_ਧਾਗੇ; i++) {
		ਖਾਲੀ *h;
		pthread_join(ਧਾਗੇ[i], &h);
		ਪ੍ਰਾਪਤੀ |= (ਪੂਰਨ) (intptr_t) h;
	}

	ਮੁਕਤ(ਧਾਗੇ);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_ਜੋੜੋ);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_ਨਤੀਜਾ);
	grep_use_locks = 0;
	disable_obj_read_ਤਾਲਾ();

	ਵਾਪਸ ਪ੍ਰਾਪਤੀ;
}

ਸਥਿਰ ਪੂਰਨ grep_cmd_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			   ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਪੂਰਨ st = grep_config(var, ਮੁੱਲ, ctx, cb);

	ਜੇ (git_color_config(var, ਮੁੱਲ, cb) < 0)
		st = -1;
	ਨਹੀਂਤਾਂ ਜੇ (git_default_config(var, ਮੁੱਲ, ctx, cb) < 0)
		st = -1;

	ਜੇ (!strcmp(var, "grep.threads")) {
		ਅੰਕ_ਧਾਗੇ = git_config_int(var, ਮੁੱਲ, ctx->kvi);
		ਜੇ (ਅੰਕ_ਧਾਗੇ < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    ਅੰਕ_ਧਾਗੇ, var);
		ਨਹੀਂਤਾਂ ਜੇ (!HAVE_ਧਾਗੇ && ਅੰਕ_ਧਾਗੇ > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			ਅੰਕ_ਧਾਗੇ = 1;
		}
	}

	ਜੇ (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, ਮੁੱਲ);

	ਵਾਪਸ st;
}

ਸਥਿਰ ਖਾਲੀ grep_ਸਰੋਤ_name(ਬਣਤਰ grep_opt *opt, ਅਚਲ ਅੱਖਰ *filename,
			     ਪੂਰਨ ਰੁੱਖ_name_len, ਬਣਤਰ strbuf *ਬਾਹਰਲਾ)
{
	strbuf_reset(ਬਾਹਰਲਾ);

	ਜੇ (opt->null_following_name) {
		ਜੇ (opt->relative && grep_ਅਗੇਤਰ) {
			ਬਣਤਰ strbuf rel_buf = STRBUF_INIT;
			ਅਚਲ ਅੱਖਰ *rel_name =
				relative_ਰਸਤਾ(filename + ਰੁੱਖ_name_len,
					      grep_ਅਗੇਤਰ, &rel_buf);

			ਜੇ (ਰੁੱਖ_name_len)
				strbuf_ਜੋੜੋ(ਬਾਹਰਲਾ, filename, ਰੁੱਖ_name_len);

			strbuf_addstr(ਬਾਹਰਲਾ, rel_name);
			strbuf_release(&rel_buf);
		} ਨਹੀਂਤਾਂ {
			strbuf_addstr(ਬਾਹਰਲਾ, filename);
		}
		ਵਾਪਸ;
	}

	ਜੇ (opt->relative && grep_ਅਗੇਤਰ)
		quote_ਰਸਤਾ(filename + ਰੁੱਖ_name_len, grep_ਅਗੇਤਰ, ਬਾਹਰਲਾ, 0);
	ਨਹੀਂਤਾਂ
		quote_c_style(filename + ਰੁੱਖ_name_len, ਬਾਹਰਲਾ, ਨਿਰਾ, 0);

	ਜੇ (ਰੁੱਖ_name_len)
		strbuf_ਪਾਓ(ਬਾਹਰਲਾ, 0, filename, ਰੁੱਖ_name_len);
}

ਸਥਿਰ ਪੂਰਨ grep_oid(ਬਣਤਰ grep_opt *opt, ਅਚਲ ਬਣਤਰ object_id *oid,
		     ਅਚਲ ਅੱਖਰ *filename, ਪੂਰਨ ਰੁੱਖ_name_len,
		     ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਣਤਰ strbuf pathbuf = STRBUF_INIT;
	ਬਣਤਰ grep_ਸਰੋਤ gs;

	grep_ਸਰੋਤ_name(opt, filename, ਰੁੱਖ_name_len, &pathbuf);
	grep_ਸਰੋਤ_init_oid(&gs, pathbuf.buf, ਰਸਤਾ, oid, opt->repo);
	strbuf_release(&pathbuf);

	ਜੇ (ਅੰਕ_ਧਾਗੇ > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		ਜੋੜੋ_work(opt, &gs);
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ {
		ਪੂਰਨ ਪ੍ਰਾਪਤੀ;

		ਪ੍ਰਾਪਤੀ = grep_ਸਰੋਤ(opt, &gs);

		grep_ਸਰੋਤ_clear(&gs);
		ਵਾਪਸ ਪ੍ਰਾਪਤੀ;
	}
}

ਸਥਿਰ ਪੂਰਨ grep_file(ਬਣਤਰ grep_opt *opt, ਅਚਲ ਅੱਖਰ *filename)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ grep_ਸਰੋਤ gs;

	grep_ਸਰੋਤ_name(opt, filename, 0, &buf);
	grep_ਸਰੋਤ_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	ਜੇ (ਅੰਕ_ਧਾਗੇ > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		ਜੋੜੋ_work(opt, &gs);
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ {
		ਪੂਰਨ ਪ੍ਰਾਪਤੀ;

		ਪ੍ਰਾਪਤੀ = grep_ਸਰੋਤ(opt, &gs);

		grep_ਸਰੋਤ_clear(&gs);
		ਵਾਪਸ ਪ੍ਰਾਪਤੀ;
	}
}

ਸਥਿਰ ਖਾਲੀ append_ਰਸਤਾ(ਬਣਤਰ grep_opt *opt, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਮਾਪ_t len)
{
	ਬਣਤਰ string_list *ਰਸਤਾ_list = opt->output_priv;

	ਜੇ (len == 1 && *(ਅਚਲ ਅੱਖਰ *)ਅੰਕੜੇ == '\0')
		ਵਾਪਸ;
	string_list_append_nodup(ਰਸਤਾ_list, xstrndup(ਅੰਕੜੇ, len));
}

ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_pager(ਬਣਤਰ grep_opt *opt, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਬਣਤਰ string_list *ਰਸਤਾ_list = opt->output_priv;
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;
	ਪੂਰਨ i, ਸਥਿਤੀ;

	ਲਈ (i = 0; i < ਰਸਤਾ_list->nr; i++)
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, ਰਸਤਾ_list->ਚੀਜ਼ਾਂ[i].string);
	ਬੱਚਾ.dir = ਅਗੇਤਰ;
	ਬੱਚਾ.use_shell = 1;

	ਸਥਿਤੀ = ਲੜੀਵਾਰ_command(&ਬੱਚਾ);
	ਜੇ (ਸਥਿਤੀ)
		ਬਾਹਰ_ਨਿਕਲੋ(ਸਥਿਤੀ);
}

ਸਥਿਰ ਪੂਰਨ grep_cache(ਬਣਤਰ grep_opt *opt,
		      ਅਚਲ ਬਣਤਰ pathspec *pathspec, ਪੂਰਨ cached);
ਸਥਿਰ ਪੂਰਨ grep_ਰੁੱਖ(ਬਣਤਰ grep_opt *opt, ਅਚਲ ਬਣਤਰ pathspec *pathspec,
		     ਬਣਤਰ ਰੁੱਖ_desc *ਰੁੱਖ, ਬਣਤਰ strbuf *ਅਧਾਰ, ਪੂਰਨ tn_len,
		     ਪੂਰਨ check_attr);

ਸਥਿਰ ਪੂਰਨ grep_submodule(ਬਣਤਰ grep_opt *opt,
			  ਅਚਲ ਬਣਤਰ pathspec *pathspec,
			  ਅਚਲ ਬਣਤਰ object_id *oid,
			  ਅਚਲ ਅੱਖਰ *filename, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ cached)
{
	ਬਣਤਰ repository *subrepo;
	ਬਣਤਰ repository *superproject = opt->repo;
	ਬਣਤਰ grep_opt subopt;
	ਪੂਰਨ ਪ੍ਰਾਪਤੀ = 0;

	ਜੇ (!is_submodule_ਸਰਗਰਮ(superproject, ਰਸਤਾ))
		ਵਾਪਸ 0;

	subrepo = xmalloc(ਆਕਾਰ(*subrepo));
	ਜੇ (repo_submodule_init(subrepo, superproject, ਰਸਤਾ, null_oid(opt->repo->hash_algo))) {
		ਮੁਕਤ(subrepo);
		ਵਾਪਸ 0;
	}
	ALLOC_GROW(repos_ਮੰਜ਼ਿਲ_free, repos_ਮੰਜ਼ਿਲ_free_nr + 1, repos_ਮੰਜ਼ਿਲ_free_alloc);
	repos_ਮੰਜ਼ਿਲ_free[repos_ਮੰਜ਼ਿਲ_free_nr++] = subrepo;

	/*
	 * NEEDSWORK: when reading a submodule, the sparsity settings in the
	 * superproject are incorrectly forgotten or misused. For example:
	 *
	 * 1. "command_requires_full_index"
	 * 	When this setting is turned on for `grep`, only the superproject
	 *	knows it. All the submodules are read with their own configs
	 *	and get prepare_repo_settings()'d. Therefore, these submodules
	 *	"forget" the sparse-index feature switch. As a result, the index
	 *	of these submodules are expanded unexpectedly.
	 *
	 * 2. "config_values_private_.apply_sparse_checkout"
	 *	When running `grep` in the superproject, this setting is
	 *	populated using the superproject's configs. However, once
	 *	initialized, this config is globally accessible and is read by
	 *	prepare_repo_settings() for the submodules. For instance, if a
	 *	submodule is using a sparse-checkout, however, the superproject
	 *	is not, the result is that the config from the superproject will
	 *	dictate the behavior for the submodule, making it "forget" its
	 *	sparse-checkout state.
	 *
	 * 3. "core_sparse_checkout_cone"
	 *	ditto.
	 *
	 * Note that this list is not exhaustive.
	 *
	 * NEEDSWORK: initializing the subrepository is not thread-safe,
	 * either, as it may cause us to race around `get_main_ref_store()`. We
	 * thus need to hold the object-read lock to serialize all readers with
	 * one another.
	 */
	obj_read_ਤਾਲਾ();
	repo_read_gitmodules(subrepo, 0);
	obj_read_unlock();

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(&subopt, opt, ਆਕਾਰ(subopt));
	subopt.repo = subrepo;

	ਜੇ (oid) {
		ਗਿਣਤੀ object_type object_type;
		ਬਣਤਰ ਰੁੱਖ_desc ਰੁੱਖ;
		ਖਾਲੀ *ਅੰਕੜੇ;
		ਮਾਪ_t ਮਾਪ;
		ਬਣਤਰ strbuf ਅਧਾਰ = STRBUF_INIT;

		obj_read_ਤਾਲਾ();
		object_type = odb_read_object_info(subrepo->objects, oid, ਨਿਰਾ);
		obj_read_unlock();
		ਅੰਕੜੇ = odb_read_object_peeled(subrepo->objects, oid, OBJ_ਰੁੱਖ, &ਮਾਪ, ਨਿਰਾ);
		ਜੇ (!ਅੰਕੜੇ)
			die(_("unable to read tree (%s)"), oid_ਮੰਜ਼ਿਲ_hex(oid));

		strbuf_addstr(&ਅਧਾਰ, filename);
		strbuf_addch(&ਅਧਾਰ, '/');

		init_ਰੁੱਖ_desc(&ਰੁੱਖ, oid, ਅੰਕੜੇ, ਮਾਪ);
		ਪ੍ਰਾਪਤੀ = grep_ਰੁੱਖ(&subopt, pathspec, &ਰੁੱਖ, &ਅਧਾਰ, ਅਧਾਰ.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&ਅਧਾਰ);
		ਮੁਕਤ(ਅੰਕੜੇ);
	} ਨਹੀਂਤਾਂ {
		ਪ੍ਰਾਪਤੀ = grep_cache(&subopt, pathspec, cached);
	}

	ਵਾਪਸ ਪ੍ਰਾਪਤੀ;
}

ਸਥਿਰ ਪੂਰਨ grep_cache(ਬਣਤਰ grep_opt *opt,
		      ਅਚਲ ਬਣਤਰ pathspec *pathspec, ਪੂਰਨ cached)
{
	ਬਣਤਰ repository *repo = opt->repo;
	ਪੂਰਨ ਪ੍ਰਾਪਤੀ = 0;
	ਪੂਰਨ nr;
	ਬਣਤਰ strbuf name = STRBUF_INIT;
	ਪੂਰਨ name_ਅਧਾਰ_len = 0;
	ਜੇ (repo->submodule_ਅਗੇਤਰ) {
		name_ਅਧਾਰ_len = ਲੜੀ_ਲੰਬਾਈ(repo->submodule_ਅਗੇਤਰ);
		strbuf_addstr(&name, repo->submodule_ਅਗੇਤਰ);
	}

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(repo) < 0)
		die(_("index file corrupt"));

	ਲਈ (nr = 0; nr < repo->ਸੂਚਕਾਂਕ->cache_nr; nr++) {
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = repo->ਸੂਚਕਾਂਕ->cache[nr];

		ਜੇ (!cached && ce_skip_worktree(ce))
			ਜਾਰੀ;

		strbuf_setlen(&name, name_ਅਧਾਰ_len);
		strbuf_addstr(&name, ce->name);
		ਜੇ (S_ISSPARSEDIR(ce->ce_ਬਹੁਲਕ)) {
			ਗਿਣਤੀ object_type type;
			ਬਣਤਰ ਰੁੱਖ_desc ਰੁੱਖ;
			ਖਾਲੀ *ਅੰਕੜੇ;
			ਮਾਪ_t ਮਾਪ;

			ਅੰਕੜੇ = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &ਮਾਪ);
			ਜੇ (!ਅੰਕੜੇ)
				die(_("unable to read tree %s"), oid_ਮੰਜ਼ਿਲ_hex(&ce->oid));
			init_ਰੁੱਖ_desc(&ਰੁੱਖ, &ce->oid, ਅੰਕੜੇ, ਮਾਪ);

			ਪ੍ਰਾਪਤੀ |= grep_ਰੁੱਖ(opt, pathspec, &ਰੁੱਖ, &name, 0, 0);
			strbuf_setlen(&name, name_ਅਧਾਰ_len);
			strbuf_addstr(&name, ce->name);
			ਮੁਕਤ(ਅੰਕੜੇ);
		} ਨਹੀਂਤਾਂ ਜੇ (S_ISREG(ce->ce_ਬਹੁਲਕ) &&
		    ਮੇਲ_pathspec(repo->ਸੂਚਕਾਂਕ, pathspec, name.buf, name.len, 0, ਨਿਰਾ,
				   S_ISDIR(ce->ce_ਬਹੁਲਕ) ||
				   S_ISGITLINK(ce->ce_ਬਹੁਲਕ))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			ਜੇ (cached || (ce->ce_flags & CE_VALID)) {
				ਜੇ (ce_stage(ce) || ce_intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ(ce))
					ਜਾਰੀ;
				ਪ੍ਰਾਪਤੀ |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} ਨਹੀਂਤਾਂ {
				ਪ੍ਰਾਪਤੀ |= grep_file(opt, name.buf);
			}
		} ਨਹੀਂਤਾਂ ਜੇ (recurse_submodules && S_ISGITLINK(ce->ce_ਬਹੁਲਕ) &&
			   submodule_ਰਸਤਾ_ਮੇਲ(repo->ਸੂਚਕਾਂਕ, pathspec, name.buf, ਨਿਰਾ)) {
			ਪ੍ਰਾਪਤੀ |= grep_submodule(opt, pathspec, ਨਿਰਾ, ce->name,
					      ce->name, cached);
		} ਨਹੀਂਤਾਂ {
			ਜਾਰੀ;
		}

		ਜੇ (ce_stage(ce)) {
			ਕਰੋ {
				nr++;
			} ਜਦੋਂ (nr < repo->ਸੂਚਕਾਂਕ->cache_nr &&
				 !strcmp(ce->name, repo->ਸੂਚਕਾਂਕ->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		ਜੇ (ਪ੍ਰਾਪਤੀ && opt->ਸਥਿਤੀ_only)
			ਰੋਕੋ;
	}

	strbuf_release(&name);
	ਵਾਪਸ ਪ੍ਰਾਪਤੀ;
}

ਸਥਿਰ ਪੂਰਨ grep_ਰੁੱਖ(ਬਣਤਰ grep_opt *opt, ਅਚਲ ਬਣਤਰ pathspec *pathspec,
		     ਬਣਤਰ ਰੁੱਖ_desc *ਰੁੱਖ, ਬਣਤਰ strbuf *ਅਧਾਰ, ਪੂਰਨ tn_len,
		     ਪੂਰਨ check_attr)
{
	ਬਣਤਰ repository *repo = opt->repo;
	ਪੂਰਨ ਪ੍ਰਾਪਤੀ = 0;
	ਗਿਣਤੀ interesting ਮੇਲ = ਇੰਦਰਾਜ_not_interesting;
	ਬਣਤਰ name_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
	ਪੂਰਨ old_baselen = ਅਧਾਰ->len;
	ਬਣਤਰ strbuf name = STRBUF_INIT;
	ਪੂਰਨ name_ਅਧਾਰ_len = 0;
	ਜੇ (repo->submodule_ਅਗੇਤਰ) {
		strbuf_addstr(&name, repo->submodule_ਅਗੇਤਰ);
		name_ਅਧਾਰ_len = name.len;
	}

	ਜਦੋਂ (ਰੁੱਖ_ਇੰਦਰਾਜ(ਰੁੱਖ, &ਇੰਦਰਾਜ)) {
		ਪੂਰਨ te_len = ਰੁੱਖ_ਇੰਦਰਾਜ_len(&ਇੰਦਰਾਜ);

		ਜੇ (ਮੇਲ != all_entries_interesting) {
			strbuf_addstr(&name, ਅਧਾਰ->buf + tn_len);
			ਮੇਲ = ਰੁੱਖ_ਇੰਦਰਾਜ_interesting(repo->ਸੂਚਕਾਂਕ,
						       &ਇੰਦਰਾਜ, &name,
						       pathspec);
			strbuf_setlen(&name, name_ਅਧਾਰ_len);

			ਜੇ (ਮੇਲ == all_entries_not_interesting)
				ਰੋਕੋ;
			ਜੇ (ਮੇਲ == ਇੰਦਰਾਜ_not_interesting)
				ਜਾਰੀ;
		}

		strbuf_ਜੋੜੋ(ਅਧਾਰ, ਇੰਦਰਾਜ.ਰਸਤਾ, te_len);

		ਜੇ (S_ISREG(ਇੰਦਰਾਜ.ਬਹੁਲਕ)) {
			ਪ੍ਰਾਪਤੀ |= grep_oid(opt, &ਇੰਦਰਾਜ.oid, ਅਧਾਰ->buf, tn_len,
					 check_attr ? ਅਧਾਰ->buf + tn_len : ਨਿਰਾ);
		} ਨਹੀਂਤਾਂ ਜੇ (S_ISDIR(ਇੰਦਰਾਜ.ਬਹੁਲਕ)) {
			ਗਿਣਤੀ object_type type;
			ਬਣਤਰ ਰੁੱਖ_desc sub;
			ਖਾਲੀ *ਅੰਕੜੇ;
			ਮਾਪ_t ਮਾਪ;

			ਅੰਕੜੇ = odb_read_object(the_repository->objects,
					       &ਇੰਦਰਾਜ.oid, &type, &ਮਾਪ);
			ਜੇ (!ਅੰਕੜੇ)
				die(_("unable to read tree (%s)"),
				    oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ.oid));

			strbuf_addch(ਅਧਾਰ, '/');
			init_ਰੁੱਖ_desc(&sub, &ਇੰਦਰਾਜ.oid, ਅੰਕੜੇ, ਮਾਪ);
			ਪ੍ਰਾਪਤੀ |= grep_ਰੁੱਖ(opt, pathspec, &sub, ਅਧਾਰ, tn_len,
					 check_attr);
			ਮੁਕਤ(ਅੰਕੜੇ);
		} ਨਹੀਂਤਾਂ ਜੇ (recurse_submodules && S_ISGITLINK(ਇੰਦਰਾਜ.ਬਹੁਲਕ)) {
			ਪ੍ਰਾਪਤੀ |= grep_submodule(opt, pathspec, &ਇੰਦਰਾਜ.oid,
					      ਅਧਾਰ->buf, ਅਧਾਰ->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(ਅਧਾਰ, old_baselen);

		ਜੇ (ਪ੍ਰਾਪਤੀ && opt->ਸਥਿਤੀ_only)
			ਰੋਕੋ;
	}

	strbuf_release(&name);
	ਵਾਪਸ ਪ੍ਰਾਪਤੀ;
}

ਸਥਿਰ ਖਾਲੀ collect_blob_oids_for_ਰੁੱਖ(ਬਣਤਰ repository *repo,
				       ਅਚਲ ਬਣਤਰ pathspec *pathspec,
				       ਬਣਤਰ ਰੁੱਖ_desc *ਰੁੱਖ,
				       ਬਣਤਰ strbuf *ਅਧਾਰ,
				       ਪੂਰਨ tn_len,
				       ਬਣਤਰ oidset *blob_oids)
{
	ਬਣਤਰ name_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
	ਪੂਰਨ old_baselen = ਅਧਾਰ->len;
	ਬਣਤਰ strbuf name = STRBUF_INIT;
	ਗਿਣਤੀ interesting ਮੇਲ = ਇੰਦਰਾਜ_not_interesting;

	ਜਦੋਂ (ਰੁੱਖ_ਇੰਦਰਾਜ(ਰੁੱਖ, &ਇੰਦਰਾਜ)) {
		ਜੇ (ਮੇਲ != all_entries_interesting) {
			strbuf_addstr(&name, ਅਧਾਰ->buf + tn_len);
			ਮੇਲ = ਰੁੱਖ_ਇੰਦਰਾਜ_interesting(repo->ਸੂਚਕਾਂਕ,
						       &ਇੰਦਰਾਜ, &name,
						       pathspec);
			strbuf_reset(&name);

			ਜੇ (ਮੇਲ == all_entries_not_interesting)
				ਰੋਕੋ;
			ਜੇ (ਮੇਲ == ਇੰਦਰਾਜ_not_interesting)
				ਜਾਰੀ;
		}

		strbuf_ਜੋੜੋ(ਅਧਾਰ, ਇੰਦਰਾਜ.ਰਸਤਾ, ਰੁੱਖ_ਇੰਦਰਾਜ_len(&ਇੰਦਰਾਜ));

		ਜੇ (S_ISREG(ਇੰਦਰਾਜ.ਬਹੁਲਕ)) {
			ਜੇ (!odb_has_object(repo->objects, &ਇੰਦਰਾਜ.oid, 0))
				oidset_ਪਾਓ(blob_oids, &ਇੰਦਰਾਜ.oid);
		} ਨਹੀਂਤਾਂ ਜੇ (S_ISDIR(ਇੰਦਰਾਜ.ਬਹੁਲਕ)) {
			ਗਿਣਤੀ object_type type;
			ਬਣਤਰ ਰੁੱਖ_desc sub_ਰੁੱਖ;
			ਖਾਲੀ *ਅੰਕੜੇ;
			ਮਾਪ_t ਮਾਪ;

			ਅੰਕੜੇ = odb_read_object(repo->objects, &ਇੰਦਰਾਜ.oid,
					       &type, &ਮਾਪ);
			ਜੇ (!ਅੰਕੜੇ)
				die(_("unable to read tree (%s)"),
				    oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ.oid));

			strbuf_addch(ਅਧਾਰ, '/');
			init_ਰੁੱਖ_desc(&sub_ਰੁੱਖ, &ਇੰਦਰਾਜ.oid, ਅੰਕੜੇ, ਮਾਪ);
			collect_blob_oids_for_ਰੁੱਖ(repo, pathspec, &sub_ਰੁੱਖ,
						   ਅਧਾਰ, tn_len, blob_oids);
			ਮੁਕਤ(ਅੰਕੜੇ);
		}
		/*
		 * ...no else clause for S_ISGITLINK: submodules have their
		 * own promisor configuration and would need separate fetches
		 * anyway.
		 */

		strbuf_setlen(ਅਧਾਰ, old_baselen);
	}

	strbuf_release(&name);
}

ਸਥਿਰ ਖਾਲੀ collect_blob_oids_for_treeish(ਬਣਤਰ grep_opt *opt,
					  ਅਚਲ ਬਣਤਰ pathspec *pathspec,
					  ਅਚਲ ਬਣਤਰ object_id *ਰੁੱਖ_ish_oid,
					  ਅਚਲ ਅੱਖਰ *name,
					  ਬਣਤਰ oidset *blob_oids)
{
	ਬਣਤਰ ਰੁੱਖ_desc ਰੁੱਖ;
	ਖਾਲੀ *ਅੰਕੜੇ;
	ਮਾਪ_t ਮਾਪ;
	ਬਣਤਰ strbuf ਅਧਾਰ = STRBUF_INIT;
	ਪੂਰਨ len;

	ਅੰਕੜੇ = odb_read_object_peeled(opt->repo->objects, ਰੁੱਖ_ish_oid,
				      OBJ_ਰੁੱਖ, &ਮਾਪ, ਨਿਰਾ);

	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ;

	len = name ? ਲੜੀ_ਲੰਬਾਈ(name) : 0;
	ਜੇ (len) {
		strbuf_ਜੋੜੋ(&ਅਧਾਰ, name, len);
		strbuf_addch(&ਅਧਾਰ, ':');
	}
	init_ਰੁੱਖ_desc(&ਰੁੱਖ, ਰੁੱਖ_ish_oid, ਅੰਕੜੇ, ਮਾਪ);

	collect_blob_oids_for_ਰੁੱਖ(opt->repo, pathspec, &ਰੁੱਖ,
				   &ਅਧਾਰ, ਅਧਾਰ.len, blob_oids);

	strbuf_release(&ਅਧਾਰ);
	ਮੁਕਤ(ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ prefetch_grep_blobs(ਬਣਤਰ grep_opt *opt,
				ਅਚਲ ਬਣਤਰ pathspec *pathspec,
				ਅਚਲ ਬਣਤਰ object_array *list)
{
	ਬਣਤਰ oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	ਜੇ (!repo_has_promisor_remote(opt->repo))
		ਵਾਪਸ;

	/* For each tree, gather the blobs in it */
	ਲਈ (ਪੂਰਨ i = 0; i < list->nr; i++) {
		ਬਣਤਰ object *real_obj;

		obj_read_ਤਾਲਾ();
		real_obj = deref_ਟੈਗ(opt->repo, list->objects[i].ਵਸਤੂ,
				     ਨਿਰਾ, 0);
		obj_read_unlock();

		ਜੇ (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_ਰੁੱਖ))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	ਜੇ (oidset_ਮਾਪ(&blob_oids)) {
		ਬਣਤਰ oid_array ਮੰਜ਼ਿਲ_fetch = OID_ARRAY_INIT;
		ਬਣਤਰ oidset_iter iter;
		ਅਚਲ ਬਣਤਰ object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		ਜਦੋਂ ((oid = oidset_iter_ਅਗਲਾ(&iter)))
			oid_array_append(&ਮੰਜ਼ਿਲ_fetch, oid);

		promisor_remote_get_ਸਿੱਧਾ(opt->repo, ਮੰਜ਼ਿਲ_fetch.oid, ਮੰਜ਼ਿਲ_fetch.nr);

		oid_array_clear(&ਮੰਜ਼ਿਲ_fetch);
	}
	oidset_clear(&blob_oids);
}

ਸਥਿਰ ਪੂਰਨ grep_object(ਬਣਤਰ grep_opt *opt, ਅਚਲ ਬਣਤਰ pathspec *pathspec,
		       ਬਣਤਰ object *obj, ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਜੇ (obj->type == OBJ_BLOB)
		ਵਾਪਸ grep_oid(opt, &obj->oid, name, 0, ਰਸਤਾ);
	ਜੇ (obj->type == OBJ_COMMIT || obj->type == OBJ_ਰੁੱਖ) {
		ਬਣਤਰ ਰੁੱਖ_desc ਰੁੱਖ;
		ਖਾਲੀ *ਅੰਕੜੇ;
		ਮਾਪ_t ਮਾਪ;
		ਬਣਤਰ strbuf ਅਧਾਰ;
		ਪੂਰਨ ਪ੍ਰਾਪਤੀ, len;

		ਅੰਕੜੇ = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_ਰੁੱਖ, &ਮਾਪ, ਨਿਰਾ);
		ਜੇ (!ਅੰਕੜੇ)
			die(_("unable to read tree (%s)"), oid_ਮੰਜ਼ਿਲ_hex(&obj->oid));

		len = name ? ਲੜੀ_ਲੰਬਾਈ(name) : 0;
		strbuf_init(&ਅਧਾਰ, ਰਸਤਾ_MAX + len + 1);
		ਜੇ (len) {
			strbuf_ਜੋੜੋ(&ਅਧਾਰ, name, len);
			strbuf_addch(&ਅਧਾਰ, ':');
		}
		init_ਰੁੱਖ_desc(&ਰੁੱਖ, &obj->oid, ਅੰਕੜੇ, ਮਾਪ);
		ਪ੍ਰਾਪਤੀ = grep_ਰੁੱਖ(opt, pathspec, &ਰੁੱਖ, &ਅਧਾਰ, ਅਧਾਰ.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&ਅਧਾਰ);
		ਮੁਕਤ(ਅੰਕੜੇ);
		ਵਾਪਸ ਪ੍ਰਾਪਤੀ;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

ਸਥਿਰ ਪੂਰਨ grep_objects(ਬਣਤਰ grep_opt *opt, ਅਚਲ ਬਣਤਰ pathspec *pathspec,
			ਅਚਲ ਬਣਤਰ object_array *list)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
	ਪੂਰਨ ਪ੍ਰਾਪਤੀ = 0;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	ਲਈ (i = 0; i < nr; i++) {
		ਬਣਤਰ object *real_obj;

		obj_read_ਤਾਲਾ();
		real_obj = deref_ਟੈਗ(opt->repo, list->objects[i].ਵਸਤੂ,
				     ਨਿਰਾ, 0);
		obj_read_unlock();

		ਜੇ (!real_obj) {
			ਅੱਖਰ hex[GIT_MAX_HEXSZ + 1];
			ਅਚਲ ਅੱਖਰ *name = list->objects[i].name;

			ਜੇ (!name) {
				oid_ਮੰਜ਼ਿਲ_hex_r(hex, &list->objects[i].ਵਸਤੂ->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		ਜੇ (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_ਤਾਲਾ();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		ਜੇ (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].ਰਸਤਾ)) {
			ਪ੍ਰਾਪਤੀ = 1;
			ਜੇ (opt->ਸਥਿਤੀ_only)
				ਰੋਕੋ;
		}
	}
	ਵਾਪਸ ਪ੍ਰਾਪਤੀ;
}

ਸਥਿਰ ਪੂਰਨ grep_directory(ਬਣਤਰ grep_opt *opt, ਅਚਲ ਬਣਤਰ pathspec *pathspec,
			  ਪੂਰਨ exc_std, ਪੂਰਨ use_ਸੂਚਕਾਂਕ)
{
	ਬਣਤਰ dir_struct dir = DIR_INIT;
	ਪੂਰਨ i, ਪ੍ਰਾਪਤੀ = 0;

	ਜੇ (!use_ਸੂਚਕਾਂਕ)
		dir.flags |= DIR_NO_GITLINKS;
	ਜੇ (exc_std)
		ਤਿਆਰੀ_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->ਸੂਚਕਾਂਕ, pathspec);
	ਲਈ (i = 0; i < dir.nr; i++) {
		ਪ੍ਰਾਪਤੀ |= grep_file(opt, dir.entries[i]->name);
		ਜੇ (ਪ੍ਰਾਪਤੀ && opt->ਸਥਿਤੀ_only)
			ਰੋਕੋ;
	}
	dir_clear(&dir);
	ਵਾਪਸ ਪ੍ਰਾਪਤੀ;
}

ਸਥਿਰ ਪੂਰਨ context_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			    ਪੂਰਨ unset)
{
	ਬਣਤਰ grep_opt *grep_opt = opt->ਮੁੱਲ;
	ਪੂਰਨ ਮੁੱਲ;
	ਅਚਲ ਅੱਖਰ *endp;

	ਜੇ (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		ਵਾਪਸ 0;
	}
	ਮੁੱਲ = strtol(arg, (ਅੱਖਰ **)&endp, 10);
	ਜੇ (*endp) {
		ਵਾਪਸ error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = ਮੁੱਲ;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ file_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ grep_opt *grep_opt = opt->ਮੁੱਲ;
	ਪੂਰਨ from_stdin;
	ਅਚਲ ਅੱਖਰ *filename = arg;
	FILE *patterns;
	ਪੂਰਨ lno = 0;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (!*filename)
		; /* leave it as-is */
	ਨਹੀਂਤਾਂ
		filename = ਅਗੇਤਰ_filename_except_for_dash(grep_ਅਗੇਤਰ, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	ਜੇ (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	ਜਦੋਂ (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		ਜੇ (sb.len == 0)
			ਜਾਰੀ;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_ਨਮੂਨਾ);
	}
	ਜੇ (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	ਜੇ (filename != arg)
		ਮੁਕਤ((ਖਾਲੀ *)filename);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ not_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ grep_opt *grep_opt = opt->ਮੁੱਲ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_ਨਮੂਨਾ(grep_opt, "--not", "command line", 0, GREP_NOT);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ and_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ grep_opt *grep_opt = opt->ਮੁੱਲ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_ਨਮੂਨਾ(grep_opt, "--and", "command line", 0, GREP_AND);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ open_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ grep_opt *grep_opt = opt->ਮੁੱਲ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_ਨਮੂਨਾ(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ close_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ grep_opt *grep_opt = opt->ਮੁੱਲ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_ਨਮੂਨਾ(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਨਮੂਨਾ_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			    ਪੂਰਨ unset)
{
	ਬਣਤਰ grep_opt *grep_opt = opt->ਮੁੱਲ;
	BUG_ON_OPT_NEG(unset);
	append_grep_ਨਮੂਨਾ(grep_opt, arg, "-e option", 0, GREP_ਨਮੂਨਾ);
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_grep(ਪੂਰਨ argc,
	     ਅਚਲ ਅੱਖਰ **argv,
	     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ਪ੍ਰਾਪਤੀ = 0;
	ਪੂਰਨ cached = 0, untracked = 0, opt_exclude = -1;
	ਪੂਰਨ ਵੇਖਿਆ_dashdash = 0;
	ਪੂਰਨ external_grep_allowed__ignored;
	ਅਚਲ ਅੱਖਰ *show_ਵਿੱਚ_pager = ਨਿਰਾ, *default_pager = "dummy";
	ਬਣਤਰ grep_opt opt;
	ਬਣਤਰ object_array list = OBJECT_ARRAY_INIT;
	ਬਣਤਰ pathspec pathspec;
	ਬਣਤਰ string_list ਰਸਤਾ_list = STRING_LIST_INIT_DUP;
	ਪੂਰਨ i;
	ਪੂਰਨ dummy;
	ਪੂਰਨ use_ਸੂਚਕਾਂਕ = 1;
	ਪੂਰਨ allow_revs;
	ਪੂਰਨ ret;

	ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "cached", &cached,
			N_("search in index instead of in the work tree")),
		OPT_NEGBIT(0, "no-index", &use_ਸੂਚਕਾਂਕ,
			 N_("find in contents not managed by git"), 1),
		OPT_BOOL(0, "untracked", &untracked,
			N_("search in both tracked and untracked files")),
		OPT_ਸੈੱਟ_INT(0, "exclude-standard", &opt_exclude,
			    N_("ignore files specified via '.gitignore'"), 1),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			 N_("recursively search in each submodule")),
		OPT_ਸਮੂਹ(""),
		OPT_BOOL('v', "invert-match", &opt.invert,
			N_("show non-matching lines")),
		OPT_BOOL('i', "ignore-case", &opt.ignore_case,
			N_("case insensitive matching")),
		OPT_BOOL('w', "word-regexp", &opt.ਸ਼ਬਦ_regexp,
			N_("match patterns only at word boundaries")),
		OPT_ਸੈੱਟ_INT('a', "text", &opt.ਦੁਆਧਾਰੀ,
			N_("process binary files as text"), GREP_ਦੁਆਧਾਰੀ_ਪਾਠ),
		OPT_ਸੈੱਟ_INT('I', ਨਿਰਾ, &opt.ਦੁਆਧਾਰੀ,
			N_("don't match patterns in binary files"),
			GREP_ਦੁਆਧਾਰੀ_NOMATCH),
		OPT_BOOL(0, "textconv", &opt.allow_textconv,
			 N_("process binary files with textconv filters")),
		OPT_ਸੈੱਟ_INT('r', "recursive", &opt.max_ਡੂੰਘਾਈ,
			    N_("search in subdirectories (default)"), -1),
		OPT_INTEGER_F(0, "max-depth", &opt.max_ਡੂੰਘਾਈ,
			N_("descend at most <n> levels"), PARSE_OPT_NONEG),
		OPT_ਸਮੂਹ(""),
		OPT_ਸੈੱਟ_INT('E', "extended-regexp", &opt.ਨਮੂਨਾ_type_option,
			    N_("use extended POSIX regular expressions"),
			    GREP_ਨਮੂਨਾ_TYPE_ERE),
		OPT_ਸੈੱਟ_INT('G', "basic-regexp", &opt.ਨਮੂਨਾ_type_option,
			    N_("use basic POSIX regular expressions (default)"),
			    GREP_ਨਮੂਨਾ_TYPE_BRE),
		OPT_ਸੈੱਟ_INT('F', "fixed-strings", &opt.ਨਮੂਨਾ_type_option,
			    N_("interpret patterns as fixed strings"),
			    GREP_ਨਮੂਨਾ_TYPE_FIXED),
		OPT_ਸੈੱਟ_INT('P', "perl-regexp", &opt.ਨਮੂਨਾ_type_option,
			    N_("use Perl-compatible regular expressions"),
			    GREP_ਨਮੂਨਾ_TYPE_PCRE),
		OPT_ਸਮੂਹ(""),
		OPT_BOOL('n', "line-number", &opt.linenum, N_("show line numbers")),
		OPT_BOOL(0, "column", &opt.columnnum, N_("show column number of first match")),
		OPT_NEGBIT('h', ਨਿਰਾ, &opt.pathname, N_("don't show filenames"), 1),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('H', ਨਿਰਾ, &opt.pathname, N_("show filenames"), 1),
		OPT_NEGBIT(0, "full-name", &opt.relative,
			N_("show filenames relative to top directory"), 1),
		OPT_BOOL('l', "files-with-matches", &opt.name_only,
			N_("show only filenames instead of matching lines")),
		OPT_BOOL(0, "name-only", &opt.name_only,
			N_("synonym for --files-with-matches")),
		OPT_BOOL('L', "files-without-match",
			&opt.unmatch_name_only,
			N_("show only the names of files without match")),
		OPT_BOOL_F('z', "null", &opt.null_following_name,
			   N_("print NUL after filenames"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('o', "only-matching", &opt.only_matching,
			N_("show only matching parts of a line")),
		OPT_BOOL('c', "count", &opt.ਗਿਣਤੀ_2,
			N_("show the number of matches instead of matching lines")),
		OPT__COLOR(&opt.color, N_("highlight matches")),
		OPT_BOOL(0, "break", &opt.file_break,
			N_("print empty line between matches from different files")),
		OPT_BOOL(0, "heading", &opt.heading,
			N_("show filename only once above matches from same file")),
		OPT_ਸਮੂਹ(""),
		OPT_CALLBACK('C', "context", &opt, N_("n"),
			N_("show <n> context lines before and after matches"),
			context_callback),
		OPT_UNSIGNED('B', "before-context", &opt.pre_context,
			N_("show <n> context lines before matches")),
		OPT_UNSIGNED('A', "after-context", &opt.post_context,
			N_("show <n> context lines after matches")),
		OPT_INTEGER(0, "threads", &ਅੰਕ_ਧਾਗੇ,
			N_("use <n> worker threads")),
		OPT_ਸੰਖਿਆ_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_ਸਮੂਹ(""),
		OPT_CALLBACK('f', ਨਿਰਾ, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', ਨਿਰਾ, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, ਨਮੂਨਾ_callback),
		OPT_CALLBACK_F(0, "and", &opt, ਨਿਰਾ,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, ਨਿਰਾ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', ਨਿਰਾ, &opt, ਨਿਰਾ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', ਨਿਰਾ, &opt, ਨਿਰਾ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			close_callback),
		OPT__QUIET(&opt.ਸਥਿਤੀ_only,
			   N_("indicate hit with exit status without output")),
		OPT_BOOL(0, "all-match", &opt.all_ਮੇਲ,
			N_("show only matches from files that match all patterns")),
		OPT_ਸਮੂਹ(""),
		{
			.type = OPTION_STRING,
			.short_name = 'O',
			.long_name = "open-files-in-pager",
			.ਮੁੱਲ = &show_ਵਿੱਚ_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_ਗਿਣਤੀ,
			N_("maximum number of results per file")),
		OPT_ਸਿਰਾ()
	};
	grep_ਅਗੇਤਰ = ਅਗੇਤਰ;

	grep_init(&opt, the_repository);
	repo_config(the_repository, grep_cmd_config, &opt);

	/*
	 * If there is no -- then the paths must exist in the working
	 * tree.  If there is no explicit pattern specified with -e or
	 * -f, we take the first unrecognized non option to be the
	 * pattern, but then what follows it must be zero or more
	 * valid refs up to the -- (if exists), and then existing
	 * paths.  If there is an explicit pattern, then the first
	 * unrecognized non option is the beginning of the refs list
	 * that continues up to the -- (if exists), and then paths.
	 */
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, grep_usage,
			     PARSE_OPT_KEEP_DASHDASH |
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ਜੇ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;
	}

	ਜੇ (use_ਸੂਚਕਾਂਕ && !startup_info->have_repository) {
		ਪੂਰਨ fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		ਜੇ (fallback)
			use_ਸੂਚਕਾਂਕ = 0;
		ਨਹੀਂਤਾਂ
			/* die the same way as if we did it at the beginning */
			ਤਿਆਰੀ_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	ਜੇ (!use_ਸੂਚਕਾਂਕ)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	ਜੇ (argc > 0 && !opt.ਨਮੂਨਾ_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	ਜੇ (argc > 0 && !opt.ਨਮੂਨਾ_list) {
		append_grep_ਨਮੂਨਾ(&opt, argv[0], "command line", 0,
				    GREP_ਨਮੂਨਾ);
		argv++;
		argc--;
	}

	ਜੇ (show_ਵਿੱਚ_pager == default_pager)
		show_ਵਿੱਚ_pager = git_pager(the_repository, 1);
	ਜੇ (show_ਵਿੱਚ_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &ਰਸਤਾ_list;
		opt.output = append_ਰਸਤਾ;
		string_list_append(&ਰਸਤਾ_list, show_ਵਿੱਚ_pager);
	}

	ਜੇ (!opt.ਨਮੂਨਾ_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	ਜੇ (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	ਲਈ (i = 0; i < argc; i++) {
		ਜੇ (!strcmp(argv[i], "--")) {
			ਵੇਖਿਆ_dashdash = 1;
			ਰੋਕੋ;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_ਸੂਚਕਾਂਕ && !untracked;
	ਲਈ (i = 0; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];
		ਬਣਤਰ object_id oid;
		ਬਣਤਰ object_context oc = {0};
		ਬਣਤਰ object *object;

		ਜੇ (!strcmp(arg, "--")) {
			i++;
			ਰੋਕੋ;
		}

		ਜੇ (!allow_revs) {
			ਜੇ (ਵੇਖਿਆ_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			ਰੋਕੋ;
		}

		ਜੇ (get_oid_with_context(the_repository, arg,
					 GET_OID_ਰਿਕਾਰਡ_ਰਸਤਾ,
					 &oid, &oc)) {
			ਜੇ (ਵੇਖਿਆ_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			ਰੋਕੋ;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		ਜੇ (!ਵੇਖਿਆ_dashdash)
			verify_non_filename(the_repository, ਅਗੇਤਰ, arg);
		ਜੋੜੋ_object_array_with_ਰਸਤਾ(object, arg, &list, oc.ਬਹੁਲਕ, oc.ਰਸਤਾ);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	ਜੇ (!ਵੇਖਿਆ_dashdash) {
		ਪੂਰਨ j;
		ਲਈ (j = i; j < argc; j++)
			verify_filename(the_repository, ਅਗੇਤਰ, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_ਡੂੰਘਾਈ != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       ਅਗੇਤਰ, argv + i);
	pathspec.max_ਡੂੰਘਾਈ = opt.max_ਡੂੰਘਾਈ;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	ਜੇ (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	ਜੇ (opt.max_ਗਿਣਤੀ == 0) {
		ret = 1;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (show_ਵਿੱਚ_pager) {
		ਜੇ (ਅੰਕ_ਧਾਗੇ > 1)
			warning(_("invalid option combination, ignoring --threads"));
		ਅੰਕ_ਧਾਗੇ = 1;
	} ਨਹੀਂਤਾਂ ਜੇ (!HAVE_ਧਾਗੇ && ਅੰਕ_ਧਾਗੇ > 1) {
		warning(_("no threads support, ignoring --threads"));
		ਅੰਕ_ਧਾਗੇ = 1;
	} ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕ_ਧਾਗੇ < 0)
		die(_("invalid number of threads specified (%d)"), ਅੰਕ_ਧਾਗੇ);
	ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕ_ਧਾਗੇ == 0)
		ਅੰਕ_ਧਾਗੇ = HAVE_ਧਾਗੇ ? online_cpus() : 1;

	ਜੇ (ਅੰਕ_ਧਾਗੇ > 1) {
		ਜੇ (!HAVE_ਧਾਗੇ)
			BUG("Somebody got num_threads calculation wrong!");
		ਜੇ (!(opt.name_only || opt.unmatch_name_only || opt.ਗਿਣਤੀ_2)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_ਪਹਿਲਾ_ਸਤਰ = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		ਜੇ (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		ਜੇ (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		ਸ਼ੁਰੂ_ਧਾਗੇ(&opt);
	} ਨਹੀਂਤਾਂ {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	ਜੇ (show_ਵਿੱਚ_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	ਜੇ (show_ਵਿੱਚ_pager && opt.ਨਮੂਨਾ_list && !opt.ਨਮੂਨਾ_list->ਅਗਲਾ) {
		ਅਚਲ ਅੱਖਰ *pager = ਰਸਤਾ_list.ਚੀਜ਼ਾਂ[0].string;
		ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(pager);

		ਜੇ (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		ਜੇ (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&ਰਸਤਾ_list, "-I");

		ਜੇ (!strcmp("less", pager) || !strcmp("vi", pager)) {
			ਬਣਤਰ strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.ਨਮੂਨਾ_list->ਨਮੂਨਾ);
			string_list_append_nodup(&ਰਸਤਾ_list,
						 strbuf_detach(&buf, ਨਿਰਾ));
		}
	}

	ਜੇ (!show_ਵਿੱਚ_pager && !opt.ਸਥਿਤੀ_only)
		ਤਿਆਰੀ_pager(the_repository);

	die_for_incompatible_opt3(!use_ਸੂਚਕਾਂਕ, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	ਜੇ (!use_ਸੂਚਕਾਂਕ || untracked) {
		ਪੂਰਨ use_exclude = (opt_exclude < 0) ? use_ਸੂਚਕਾਂਕ : !!opt_exclude;
		ਪ੍ਰਾਪਤੀ = grep_directory(&opt, &pathspec, use_exclude, use_ਸੂਚਕਾਂਕ);
	} ਨਹੀਂਤਾਂ ਜੇ (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} ਨਹੀਂਤਾਂ ਜੇ (!list.nr) {
		ਜੇ (!cached)
			ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);

		ਪ੍ਰਾਪਤੀ = grep_cache(&opt, &pathspec, cached);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (cached)
			die(_("both --cached and trees are given"));

		ਪ੍ਰਾਪਤੀ = grep_objects(&opt, &pathspec, &list);
	}

	ਜੇ (ਅੰਕ_ਧਾਗੇ > 1)
		ਪ੍ਰਾਪਤੀ |= wait_all();
	ਜੇ (ਪ੍ਰਾਪਤੀ && show_ਵਿੱਚ_pager)
		ਲੜੀਵਾਰ_pager(&opt, ਅਗੇਤਰ);

	ret = !ਪ੍ਰਾਪਤੀ;

ਬਾਹਰਲਾ:
	clear_pathspec(&pathspec);
	string_list_clear(&ਰਸਤਾ_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	ਵਾਪਸ ret;
}
