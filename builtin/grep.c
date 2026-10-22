/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "grep.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "submodule-config.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/source.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "oidset.h"
#ಅನ್ತರ್ಭಾವ "pager.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "promisor-remote.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *grep_ಪೂರ್ವಪ್ರತ್ಯಯ;

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ recurse_submodules;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಎಳೆಗಳು;

ಸ್ಥಿರ pthread_t *ಎಳೆಗಳು;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
ರಚನೆ work_ಅಂಶ {
	ರಚನೆ grep_ಮೂಲ ಮೂಲ;
	ಅಕ್ಷರ ಮುಗಿದ;
	ರಚನೆ strbuf ಹೊರಹೋಗು;
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
#ವ್ಯಾಖ್ಯಾನಿಸು TODO_ಗಾತ್ರ 128
ಸ್ಥಿರ ರಚನೆ work_ಅಂಶ todo[TODO_ಗಾತ್ರ];
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ todo_ಆರಂಭ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ todo_ಕೊನೆ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ todo_ಮುಗಿದ;

/* Has all work items been added? */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ all_work_added;

ಸ್ಥಿರ ರಚನೆ repository **repos_ಗಮ್ಯ_free;
ಸ್ಥಿರ ಗಾತ್ರ_t repos_ಗಮ್ಯ_free_nr, repos_ಗಮ್ಯ_free_alloc;

/* This lock protects all the variables above. */
ಸ್ಥಿರ pthread_mutex_t grep_mutex;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ grep_ಬೀಗ(ಶೂನ್ಯ)
{
	pthread_mutex_ಬೀಗ(&grep_mutex);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ grep_unlock(ಶೂನ್ಯ)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
ಸ್ಥಿರ pthread_cond_t cond_ಕೂಡಿಸು;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
ಸ್ಥಿರ pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
ಸ್ಥಿರ pthread_cond_t cond_ಫಲಿತಾಂಶ;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ skip_ಮೊದಲ_ಸಾಲು;

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_work(ರಚನೆ grep_opt *opt, ರಚನೆ grep_ಮೂಲ *gs)
{
	ಆದರೆ (opt->ದ್ವಿಮಾನ != GREP_ದ್ವಿಮಾನ_ಪಠ್ಯ)
		grep_ಮೂಲ_ತುಂಬು_driver(gs, opt->repo->ಸೂಚ್ಯಂಕ);

	grep_ಬೀಗ();

	ಆಗ ((todo_ಕೊನೆ+1) % ARRAY_ಗಾತ್ರ(todo) == todo_ಮುಗಿದ) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_ಕೊನೆ].ಮೂಲ = *gs;
	todo[todo_ಕೊನೆ].ಮುಗಿದ = 0;
	strbuf_reset(&todo[todo_ಕೊನೆ].ಹೊರಹೋಗು);
	todo_ಕೊನೆ = (todo_ಕೊನೆ + 1) % ARRAY_ಗಾತ್ರ(todo);

	pthread_cond_signal(&cond_ಕೂಡಿಸು);
	grep_unlock();
}

ಸ್ಥಿರ ರಚನೆ work_ಅಂಶ *get_work(ಶೂನ್ಯ)
{
	ರಚನೆ work_ಅಂಶ *ret;

	grep_ಬೀಗ();
	ಆಗ (todo_ಆರಂಭ == todo_ಕೊನೆ && !all_work_added) {
		pthread_cond_wait(&cond_ಕೂಡಿಸು, &grep_mutex);
	}

	ಆದರೆ (todo_ಆರಂಭ == todo_ಕೊನೆ && all_work_added) {
		ret = ಖಾಲಿ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ret = &todo[todo_ಆರಂಭ];
		todo_ಆರಂಭ = (todo_ಆರಂಭ + 1) % ARRAY_ಗಾತ್ರ(todo);
	}
	grep_unlock();
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ work_ಮುಗಿದ(ರಚನೆ work_ಅಂಶ *w)
{
	ಪೂರ್ಣಾಂಕ old_ಮುಗಿದ;

	grep_ಬೀಗ();
	w->ಮುಗಿದ = 1;
	old_ಮುಗಿದ = todo_ಮುಗಿದ;
	ಗಾಗಿ(; todo[todo_ಮುಗಿದ].ಮುಗಿದ && todo_ಮುಗಿದ != todo_ಆರಂಭ;
	    todo_ಮುಗಿದ = (todo_ಮುಗಿದ+1) % ARRAY_ಗಾತ್ರ(todo)) {
		w = &todo[todo_ಮುಗಿದ];
		ಆದರೆ (w->ಹೊರಹೋಗು.len) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p = w->ಹೊರಹೋಗು.buf;
			ಗಾತ್ರ_t len = w->ಹೊರಹೋಗು.len;

			/* Skip the leading hunk mark of the first file. */
			ಆದರೆ (skip_ಮೊದಲ_ಸಾಲು) {
				ಆಗ (len) {
					len--;
					ಆದರೆ (*p++ == '\n')
						ನಿಲ್ಲಿಸು;
				}
				skip_ಮೊದಲ_ಸಾಲು = 0;
			}

			write_or_die(1, p, len);
		}
		grep_ಮೂಲ_clear(&w->ಮೂಲ);
	}

	ಆದರೆ (old_ಮುಗಿದ != todo_ಮುಗಿದ)
		pthread_cond_signal(&cond_write);

	ಆದರೆ (all_work_added && todo_ಮುಗಿದ == todo_ಕೊನೆ)
		pthread_cond_signal(&cond_ಫಲಿತಾಂಶ);

	grep_unlock();
}

ಸ್ಥಿರ ಶೂನ್ಯ free_repos(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < repos_ಗಮ್ಯ_free_nr; i++) {
		repo_clear(repos_ಗಮ್ಯ_free[i]);
		ಬಿಡುಗಡೆ(repos_ಗಮ್ಯ_free[i]);
	}
	FREE_AND_NULL(repos_ಗಮ್ಯ_free);
	repos_ಗಮ್ಯ_free_nr = 0;
	repos_ಗಮ್ಯ_free_alloc = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ *ಸತತ(ಶೂನ್ಯ *arg)
{
	ಪೂರ್ಣಾಂಕ ದೊರಕಿತು = 0;
	ರಚನೆ grep_opt *opt = arg;

	ಆಗ (1) {
		ರಚನೆ work_ಅಂಶ *w = get_work();
		ಆದರೆ (!w)
			ನಿಲ್ಲಿಸು;

		opt->output_priv = w;
		ದೊರಕಿತು |= grep_ಮೂಲ(opt, &w->ಮೂಲ);
		grep_ಮೂಲ_clear_ದತ್ತಾಂಶ(&w->ಮೂಲ);
		work_ಮುಗಿದ(w);
	}
	free_grep_patterns(opt);
	ಬಿಡುಗಡೆ(opt);

	ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ*) (intptr_t) ದೊರಕಿತು;
}

ಸ್ಥಿರ ಶೂನ್ಯ strbuf_ಹೊರಹೋಗು(ರಚನೆ grep_opt *opt, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
	ರಚನೆ work_ಅಂಶ *w = opt->output_priv;
	strbuf_ಕೂಡಿಸು(&w->ಹೊರಹೋಗು, buf, ಗಾತ್ರ_2);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಆರಂಭ_ಎಳೆಗಳು(ರಚನೆ grep_opt *opt)
{
	ಪೂರ್ಣಾಂಕ i;

	pthread_mutex_init(&grep_mutex, ಖಾಲಿ);
	pthread_mutex_init(&grep_attr_mutex, ಖಾಲಿ);
	pthread_cond_init(&cond_ಕೂಡಿಸು, ಖಾಲಿ);
	pthread_cond_init(&cond_write, ಖಾಲಿ);
	pthread_cond_init(&cond_ಫಲಿತಾಂಶ, ಖಾಲಿ);
	grep_use_locks = 1;
	enable_obj_read_ಬೀಗ();

	ಗಾಗಿ (i = 0; i < ARRAY_ಗಾತ್ರ(todo); i++) {
		strbuf_init(&todo[i].ಹೊರಹೋಗು, 0);
	}

	CALLOC_ARRAY(ಎಳೆಗಳು, ಅಂಕೆ_ಎಳೆಗಳು);
	ಗಾಗಿ (i = 0; i < ಅಂಕೆ_ಎಳೆಗಳು; i++) {
		ಪೂರ್ಣಾಂಕ err;
		ರಚನೆ grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_ಹೊರಹೋಗು;
		compile_grep_patterns(o);
		err = pthread_create(&ಎಳೆಗಳು[i], ಖಾಲಿ, ಸತತ, o);

		ಆದರೆ (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ wait_all(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ ದೊರಕಿತು = 0;
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (!HAVE_ಎಳೆಗಳು)
		BUG("Never call this function unless you have started threads");

	grep_ಬೀಗ();
	all_work_added = 1;

	/* Wait until all work is done. */
	ಆಗ (todo_ಮುಗಿದ != todo_ಕೊನೆ)
		pthread_cond_wait(&cond_ಫಲಿತಾಂಶ, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_ಪ್ರಸಾರ(&cond_ಕೂಡಿಸು);
	grep_unlock();

	ಗಾಗಿ (i = 0; i < ಅಂಕೆ_ಎಳೆಗಳು; i++) {
		ಶೂನ್ಯ *h;
		pthread_join(ಎಳೆಗಳು[i], &h);
		ದೊರಕಿತು |= (ಪೂರ್ಣಾಂಕ) (intptr_t) h;
	}

	ಬಿಡುಗಡೆ(ಎಳೆಗಳು);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_ಕೂಡಿಸು);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_ಫಲಿತಾಂಶ);
	grep_use_locks = 0;
	disable_obj_read_ಬೀಗ();

	ಹಿಂತಿರುಗಿ ದೊರಕಿತು;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ grep_cmd_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಪೂರ್ಣಾಂಕ st = grep_config(var, ಮೌಲ್ಯ, ctx, cb);

	ಆದರೆ (git_color_config(var, ಮೌಲ್ಯ, cb) < 0)
		st = -1;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (git_default_config(var, ಮೌಲ್ಯ, ctx, cb) < 0)
		st = -1;

	ಆದರೆ (!strcmp(var, "grep.threads")) {
		ಅಂಕೆ_ಎಳೆಗಳು = git_config_int(var, ಮೌಲ್ಯ, ctx->kvi);
		ಆದರೆ (ಅಂಕೆ_ಎಳೆಗಳು < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    ಅಂಕೆ_ಎಳೆಗಳು, var);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!HAVE_ಎಳೆಗಳು && ಅಂಕೆ_ಎಳೆಗಳು > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			ಅಂಕೆ_ಎಳೆಗಳು = 1;
		}
	}

	ಆದರೆ (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, ಮೌಲ್ಯ);

	ಹಿಂತಿರುಗಿ st;
}

ಸ್ಥಿರ ಶೂನ್ಯ grep_ಮೂಲ_name(ರಚನೆ grep_opt *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename,
			     ಪೂರ್ಣಾಂಕ ವೃಕ್ಷ_name_len, ರಚನೆ strbuf *ಹೊರಹೋಗು)
{
	strbuf_reset(ಹೊರಹೋಗು);

	ಆದರೆ (opt->null_following_name) {
		ಆದರೆ (opt->relative && grep_ಪೂರ್ವಪ್ರತ್ಯಯ) {
			ರಚನೆ strbuf rel_buf = STRBUF_INIT;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel_name =
				relative_ಮಾರ್ಗ(filename + ವೃಕ್ಷ_name_len,
					      grep_ಪೂರ್ವಪ್ರತ್ಯಯ, &rel_buf);

			ಆದರೆ (ವೃಕ್ಷ_name_len)
				strbuf_ಕೂಡಿಸು(ಹೊರಹೋಗು, filename, ವೃಕ್ಷ_name_len);

			strbuf_addstr(ಹೊರಹೋಗು, rel_name);
			strbuf_release(&rel_buf);
		} ಇಲ್ಲದಿದ್ದರೆ {
			strbuf_addstr(ಹೊರಹೋಗು, filename);
		}
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (opt->relative && grep_ಪೂರ್ವಪ್ರತ್ಯಯ)
		quote_ಮಾರ್ಗ(filename + ವೃಕ್ಷ_name_len, grep_ಪೂರ್ವಪ್ರತ್ಯಯ, ಹೊರಹೋಗು, 0);
	ಇಲ್ಲದಿದ್ದರೆ
		quote_c_style(filename + ವೃಕ್ಷ_name_len, ಹೊರಹೋಗು, ಖಾಲಿ, 0);

	ಆದರೆ (ವೃಕ್ಷ_name_len)
		strbuf_ಸೇರಿಸು(ಹೊರಹೋಗು, 0, filename, ವೃಕ್ಷ_name_len);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ grep_oid(ರಚನೆ grep_opt *opt, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename, ಪೂರ್ಣಾಂಕ ವೃಕ್ಷ_name_len,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ರಚನೆ strbuf pathbuf = STRBUF_INIT;
	ರಚನೆ grep_ಮೂಲ gs;

	grep_ಮೂಲ_name(opt, filename, ವೃಕ್ಷ_name_len, &pathbuf);
	grep_ಮೂಲ_init_oid(&gs, pathbuf.buf, ಮಾರ್ಗ, oid, opt->repo);
	strbuf_release(&pathbuf);

	ಆದರೆ (ಅಂಕೆ_ಎಳೆಗಳು > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		ಕೂಡಿಸು_work(opt, &gs);
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಪೂರ್ಣಾಂಕ ದೊರಕಿತು;

		ದೊರಕಿತು = grep_ಮೂಲ(opt, &gs);

		grep_ಮೂಲ_clear(&gs);
		ಹಿಂತಿರುಗಿ ದೊರಕಿತು;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ grep_file(ರಚನೆ grep_opt *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ grep_ಮೂಲ gs;

	grep_ಮೂಲ_name(opt, filename, 0, &buf);
	grep_ಮೂಲ_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	ಆದರೆ (ಅಂಕೆ_ಎಳೆಗಳು > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		ಕೂಡಿಸು_work(opt, &gs);
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಪೂರ್ಣಾಂಕ ದೊರಕಿತು;

		ದೊರಕಿತು = grep_ಮೂಲ(opt, &gs);

		grep_ಮೂಲ_clear(&gs);
		ಹಿಂತಿರುಗಿ ದೊರಕಿತು;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ append_ಮಾರ್ಗ(ರಚನೆ grep_opt *opt, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಗಾತ್ರ_t len)
{
	ರಚನೆ string_list *ಮಾರ್ಗ_list = opt->output_priv;

	ಆದರೆ (len == 1 && *(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)ದತ್ತಾಂಶ == '\0')
		ಹಿಂತಿರುಗಿ;
	string_list_append_nodup(ಮಾರ್ಗ_list, xstrndup(ದತ್ತಾಂಶ, len));
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_pager(ರಚನೆ grep_opt *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ರಚನೆ string_list *ಮಾರ್ಗ_list = opt->output_priv;
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;
	ಪೂರ್ಣಾಂಕ i, ಸ್ಥಿತಿ_2;

	ಗಾಗಿ (i = 0; i < ಮಾರ್ಗ_list->nr; i++)
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, ಮಾರ್ಗ_list->ವಸ್ತುಗಳು[i].string);
	ಮಗು.dir = ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಮಗು.use_shell = 1;

	ಸ್ಥಿತಿ_2 = ಸತತ_command(&ಮಗು);
	ಆದರೆ (ಸ್ಥಿತಿ_2)
		ನಿರ್ಗಮಿಸು(ಸ್ಥಿತಿ_2);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ grep_cache(ರಚನೆ grep_opt *opt,
		      ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec, ಪೂರ್ಣಾಂಕ cached);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ grep_ವೃಕ್ಷ(ರಚನೆ grep_opt *opt, ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
		     ರಚನೆ ವೃಕ್ಷ_desc *ವೃಕ್ಷ, ರಚನೆ strbuf *ತಳ, ಪೂರ್ಣಾಂಕ tn_len,
		     ಪೂರ್ಣಾಂಕ check_attr);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ grep_submodule(ರಚನೆ grep_opt *opt,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ cached)
{
	ರಚನೆ repository *subrepo;
	ರಚನೆ repository *superproject = opt->repo;
	ರಚನೆ grep_opt subopt;
	ಪೂರ್ಣಾಂಕ ದೊರಕಿತು = 0;

	ಆದರೆ (!is_submodule_ಸಕ್ರಿಯ(superproject, ಮಾರ್ಗ))
		ಹಿಂತಿರುಗಿ 0;

	subrepo = xmalloc(ಗಾತ್ರ(*subrepo));
	ಆದರೆ (repo_submodule_init(subrepo, superproject, ಮಾರ್ಗ, null_oid(opt->repo->hash_algo))) {
		ಬಿಡುಗಡೆ(subrepo);
		ಹಿಂತಿರುಗಿ 0;
	}
	ALLOC_GROW(repos_ಗಮ್ಯ_free, repos_ಗಮ್ಯ_free_nr + 1, repos_ಗಮ್ಯ_free_alloc);
	repos_ಗಮ್ಯ_free[repos_ಗಮ್ಯ_free_nr++] = subrepo;

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
	obj_read_ಬೀಗ();
	repo_read_gitmodules(subrepo, 0);
	obj_read_unlock();

	ಸ್ಮರಣೆ_ನಕಲು(&subopt, opt, ಗಾತ್ರ(subopt));
	subopt.repo = subrepo;

	ಆದರೆ (oid) {
		ಎಣಿಕೆ object_type object_type;
		ರಚನೆ ವೃಕ್ಷ_desc ವೃಕ್ಷ;
		ಶೂನ್ಯ *ದತ್ತಾಂಶ;
		ಗಾತ್ರ_t ಗಾತ್ರ_2;
		ರಚನೆ strbuf ತಳ = STRBUF_INIT;

		obj_read_ಬೀಗ();
		object_type = odb_read_object_info(subrepo->objects, oid, ಖಾಲಿ);
		obj_read_unlock();
		ದತ್ತಾಂಶ = odb_read_object_peeled(subrepo->objects, oid, OBJ_ವೃಕ್ಷ, &ಗಾತ್ರ_2, ಖಾಲಿ);
		ಆದರೆ (!ದತ್ತಾಂಶ)
			die(_("unable to read tree (%s)"), oid_ಗಮ್ಯ_hex(oid));

		strbuf_addstr(&ತಳ, filename);
		strbuf_addch(&ತಳ, '/');

		init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ, oid, ದತ್ತಾಂಶ, ಗಾತ್ರ_2);
		ದೊರಕಿತು = grep_ವೃಕ್ಷ(&subopt, pathspec, &ವೃಕ್ಷ, &ತಳ, ತಳ.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&ತಳ);
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ದೊರಕಿತು = grep_cache(&subopt, pathspec, cached);
	}

	ಹಿಂತಿರುಗಿ ದೊರಕಿತು;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ grep_cache(ರಚನೆ grep_opt *opt,
		      ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec, ಪೂರ್ಣಾಂಕ cached)
{
	ರಚನೆ repository *repo = opt->repo;
	ಪೂರ್ಣಾಂಕ ದೊರಕಿತು = 0;
	ಪೂರ್ಣಾಂಕ nr;
	ರಚನೆ strbuf name = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ name_ತಳ_len = 0;
	ಆದರೆ (repo->submodule_ಪೂರ್ವಪ್ರತ್ಯಯ) {
		name_ತಳ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(repo->submodule_ಪೂರ್ವಪ್ರತ್ಯಯ);
		strbuf_addstr(&name, repo->submodule_ಪೂರ್ವಪ್ರತ್ಯಯ);
	}

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(repo) < 0)
		die(_("index file corrupt"));

	ಗಾಗಿ (nr = 0; nr < repo->ಸೂಚ್ಯಂಕ->cache_nr; nr++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = repo->ಸೂಚ್ಯಂಕ->cache[nr];

		ಆದರೆ (!cached && ce_skip_worktree(ce))
			ಮುಂದುವರಿ;

		strbuf_setlen(&name, name_ತಳ_len);
		strbuf_addstr(&name, ce->name);
		ಆದರೆ (S_ISSPARSEDIR(ce->ce_ಬಹುಲಕ)) {
			ಎಣಿಕೆ object_type type;
			ರಚನೆ ವೃಕ್ಷ_desc ವೃಕ್ಷ;
			ಶೂನ್ಯ *ದತ್ತಾಂಶ;
			ಗಾತ್ರ_t ಗಾತ್ರ_2;

			ದತ್ತಾಂಶ = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &ಗಾತ್ರ_2);
			ಆದರೆ (!ದತ್ತಾಂಶ)
				die(_("unable to read tree %s"), oid_ಗಮ್ಯ_hex(&ce->oid));
			init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ, &ce->oid, ದತ್ತಾಂಶ, ಗಾತ್ರ_2);

			ದೊರಕಿತು |= grep_ವೃಕ್ಷ(opt, pathspec, &ವೃಕ್ಷ, &name, 0, 0);
			strbuf_setlen(&name, name_ತಳ_len);
			strbuf_addstr(&name, ce->name);
			ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISREG(ce->ce_ಬಹುಲಕ) &&
		    ಹೊಂದಿಕೆ_pathspec(repo->ಸೂಚ್ಯಂಕ, pathspec, name.buf, name.len, 0, ಖಾಲಿ,
				   S_ISDIR(ce->ce_ಬಹುಲಕ) ||
				   S_ISGITLINK(ce->ce_ಬಹುಲಕ))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			ಆದರೆ (cached || (ce->ce_flags & CE_VALID)) {
				ಆದರೆ (ce_stage(ce) || ce_intent_ಗಮ್ಯ_ಕೂಡಿಸು(ce))
					ಮುಂದುವರಿ;
				ದೊರಕಿತು |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} ಇಲ್ಲದಿದ್ದರೆ {
				ದೊರಕಿತು |= grep_file(opt, name.buf);
			}
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (recurse_submodules && S_ISGITLINK(ce->ce_ಬಹುಲಕ) &&
			   submodule_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(repo->ಸೂಚ್ಯಂಕ, pathspec, name.buf, ಖಾಲಿ)) {
			ದೊರಕಿತು |= grep_submodule(opt, pathspec, ಖಾಲಿ, ce->name,
					      ce->name, cached);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (ce_stage(ce)) {
			ಮಾಡು {
				nr++;
			} ಆಗ (nr < repo->ಸೂಚ್ಯಂಕ->cache_nr &&
				 !strcmp(ce->name, repo->ಸೂಚ್ಯಂಕ->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		ಆದರೆ (ದೊರಕಿತು && opt->ಸ್ಥಿತಿ_only)
			ನಿಲ್ಲಿಸು;
	}

	strbuf_release(&name);
	ಹಿಂತಿರುಗಿ ದೊರಕಿತು;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ grep_ವೃಕ್ಷ(ರಚನೆ grep_opt *opt, ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
		     ರಚನೆ ವೃಕ್ಷ_desc *ವೃಕ್ಷ, ರಚನೆ strbuf *ತಳ, ಪೂರ್ಣಾಂಕ tn_len,
		     ಪೂರ್ಣಾಂಕ check_attr)
{
	ರಚನೆ repository *repo = opt->repo;
	ಪೂರ್ಣಾಂಕ ದೊರಕಿತು = 0;
	ಎಣಿಕೆ interesting ಹೊಂದಿಕೆ = ನಮೂದು_not_interesting;
	ರಚನೆ name_ನಮೂದು ನಮೂದು;
	ಪೂರ್ಣಾಂಕ old_baselen = ತಳ->len;
	ರಚನೆ strbuf name = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ name_ತಳ_len = 0;
	ಆದರೆ (repo->submodule_ಪೂರ್ವಪ್ರತ್ಯಯ) {
		strbuf_addstr(&name, repo->submodule_ಪೂರ್ವಪ್ರತ್ಯಯ);
		name_ತಳ_len = name.len;
	}

	ಆಗ (ವೃಕ್ಷ_ನಮೂದು(ವೃಕ್ಷ, &ನಮೂದು)) {
		ಪೂರ್ಣಾಂಕ te_len = ವೃಕ್ಷ_ನಮೂದು_len(&ನಮೂದು);

		ಆದರೆ (ಹೊಂದಿಕೆ != all_entries_interesting) {
			strbuf_addstr(&name, ತಳ->buf + tn_len);
			ಹೊಂದಿಕೆ = ವೃಕ್ಷ_ನಮೂದು_interesting(repo->ಸೂಚ್ಯಂಕ,
						       &ನಮೂದು, &name,
						       pathspec);
			strbuf_setlen(&name, name_ತಳ_len);

			ಆದರೆ (ಹೊಂದಿಕೆ == all_entries_not_interesting)
				ನಿಲ್ಲಿಸು;
			ಆದರೆ (ಹೊಂದಿಕೆ == ನಮೂದು_not_interesting)
				ಮುಂದುವರಿ;
		}

		strbuf_ಕೂಡಿಸು(ತಳ, ನಮೂದು.ಮಾರ್ಗ, te_len);

		ಆದರೆ (S_ISREG(ನಮೂದು.ಬಹುಲಕ)) {
			ದೊರಕಿತು |= grep_oid(opt, &ನಮೂದು.oid, ತಳ->buf, tn_len,
					 check_attr ? ತಳ->buf + tn_len : ಖಾಲಿ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISDIR(ನಮೂದು.ಬಹುಲಕ)) {
			ಎಣಿಕೆ object_type type;
			ರಚನೆ ವೃಕ್ಷ_desc sub;
			ಶೂನ್ಯ *ದತ್ತಾಂಶ;
			ಗಾತ್ರ_t ಗಾತ್ರ_2;

			ದತ್ತಾಂಶ = odb_read_object(the_repository->objects,
					       &ನಮೂದು.oid, &type, &ಗಾತ್ರ_2);
			ಆದರೆ (!ದತ್ತಾಂಶ)
				die(_("unable to read tree (%s)"),
				    oid_ಗಮ್ಯ_hex(&ನಮೂದು.oid));

			strbuf_addch(ತಳ, '/');
			init_ವೃಕ್ಷ_desc(&sub, &ನಮೂದು.oid, ದತ್ತಾಂಶ, ಗಾತ್ರ_2);
			ದೊರಕಿತು |= grep_ವೃಕ್ಷ(opt, pathspec, &sub, ತಳ, tn_len,
					 check_attr);
			ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (recurse_submodules && S_ISGITLINK(ನಮೂದು.ಬಹುಲಕ)) {
			ದೊರಕಿತು |= grep_submodule(opt, pathspec, &ನಮೂದು.oid,
					      ತಳ->buf, ತಳ->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(ತಳ, old_baselen);

		ಆದರೆ (ದೊರಕಿತು && opt->ಸ್ಥಿತಿ_only)
			ನಿಲ್ಲಿಸು;
	}

	strbuf_release(&name);
	ಹಿಂತಿರುಗಿ ದೊರಕಿತು;
}

ಸ್ಥಿರ ಶೂನ್ಯ collect_blob_oids_for_ವೃಕ್ಷ(ರಚನೆ repository *repo,
				       ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
				       ರಚನೆ ವೃಕ್ಷ_desc *ವೃಕ್ಷ,
				       ರಚನೆ strbuf *ತಳ,
				       ಪೂರ್ಣಾಂಕ tn_len,
				       ರಚನೆ oidset *blob_oids)
{
	ರಚನೆ name_ನಮೂದು ನಮೂದು;
	ಪೂರ್ಣಾಂಕ old_baselen = ತಳ->len;
	ರಚನೆ strbuf name = STRBUF_INIT;
	ಎಣಿಕೆ interesting ಹೊಂದಿಕೆ = ನಮೂದು_not_interesting;

	ಆಗ (ವೃಕ್ಷ_ನಮೂದು(ವೃಕ್ಷ, &ನಮೂದು)) {
		ಆದರೆ (ಹೊಂದಿಕೆ != all_entries_interesting) {
			strbuf_addstr(&name, ತಳ->buf + tn_len);
			ಹೊಂದಿಕೆ = ವೃಕ್ಷ_ನಮೂದು_interesting(repo->ಸೂಚ್ಯಂಕ,
						       &ನಮೂದು, &name,
						       pathspec);
			strbuf_reset(&name);

			ಆದರೆ (ಹೊಂದಿಕೆ == all_entries_not_interesting)
				ನಿಲ್ಲಿಸು;
			ಆದರೆ (ಹೊಂದಿಕೆ == ನಮೂದು_not_interesting)
				ಮುಂದುವರಿ;
		}

		strbuf_ಕೂಡಿಸು(ತಳ, ನಮೂದು.ಮಾರ್ಗ, ವೃಕ್ಷ_ನಮೂದು_len(&ನಮೂದು));

		ಆದರೆ (S_ISREG(ನಮೂದು.ಬಹುಲಕ)) {
			ಆದರೆ (!odb_has_object(repo->objects, &ನಮೂದು.oid, 0))
				oidset_ಸೇರಿಸು(blob_oids, &ನಮೂದು.oid);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISDIR(ನಮೂದು.ಬಹುಲಕ)) {
			ಎಣಿಕೆ object_type type;
			ರಚನೆ ವೃಕ್ಷ_desc sub_ವೃಕ್ಷ;
			ಶೂನ್ಯ *ದತ್ತಾಂಶ;
			ಗಾತ್ರ_t ಗಾತ್ರ_2;

			ದತ್ತಾಂಶ = odb_read_object(repo->objects, &ನಮೂದು.oid,
					       &type, &ಗಾತ್ರ_2);
			ಆದರೆ (!ದತ್ತಾಂಶ)
				die(_("unable to read tree (%s)"),
				    oid_ಗಮ್ಯ_hex(&ನಮೂದು.oid));

			strbuf_addch(ತಳ, '/');
			init_ವೃಕ್ಷ_desc(&sub_ವೃಕ್ಷ, &ನಮೂದು.oid, ದತ್ತಾಂಶ, ಗಾತ್ರ_2);
			collect_blob_oids_for_ವೃಕ್ಷ(repo, pathspec, &sub_ವೃಕ್ಷ,
						   ತಳ, tn_len, blob_oids);
			ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
		}
		/*
		 * ...no else clause for S_ISGITLINK: submodules have their
		 * own promisor configuration and would need separate fetches
		 * anyway.
		 */

		strbuf_setlen(ತಳ, old_baselen);
	}

	strbuf_release(&name);
}

ಸ್ಥಿರ ಶೂನ್ಯ collect_blob_oids_for_treeish(ರಚನೆ grep_opt *opt,
					  ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
					  ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ವೃಕ್ಷ_ish_oid,
					  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
					  ರಚನೆ oidset *blob_oids)
{
	ರಚನೆ ವೃಕ್ಷ_desc ವೃಕ್ಷ;
	ಶೂನ್ಯ *ದತ್ತಾಂಶ;
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
	ರಚನೆ strbuf ತಳ = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ len;

	ದತ್ತಾಂಶ = odb_read_object_peeled(opt->repo->objects, ವೃಕ್ಷ_ish_oid,
				      OBJ_ವೃಕ್ಷ, &ಗಾತ್ರ_2, ಖಾಲಿ);

	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ;

	len = name ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name) : 0;
	ಆದರೆ (len) {
		strbuf_ಕೂಡಿಸು(&ತಳ, name, len);
		strbuf_addch(&ತಳ, ':');
	}
	init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ, ವೃಕ್ಷ_ish_oid, ದತ್ತಾಂಶ, ಗಾತ್ರ_2);

	collect_blob_oids_for_ವೃಕ್ಷ(opt->repo, pathspec, &ವೃಕ್ಷ,
				   &ತಳ, ತಳ.len, blob_oids);

	strbuf_release(&ತಳ);
	ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಶೂನ್ಯ prefetch_grep_blobs(ರಚನೆ grep_opt *opt,
				ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
				ಸ್ಥಿರಾಂಕ ರಚನೆ object_array *list)
{
	ರಚನೆ oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	ಆದರೆ (!repo_has_promisor_remote(opt->repo))
		ಹಿಂತಿರುಗಿ;

	/* For each tree, gather the blobs in it */
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < list->nr; i++) {
		ರಚನೆ object *real_obj;

		obj_read_ಬೀಗ();
		real_obj = deref_ಟ್ಯಾಗ್(opt->repo, list->objects[i].ಅಂಶ,
				     ಖಾಲಿ, 0);
		obj_read_unlock();

		ಆದರೆ (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_ವೃಕ್ಷ))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	ಆದರೆ (oidset_ಗಾತ್ರ(&blob_oids)) {
		ರಚನೆ oid_array ಗಮ್ಯ_fetch = OID_ARRAY_INIT;
		ರಚನೆ oidset_iter iter;
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		ಆಗ ((oid = oidset_iter_ಮುಂದಿನ(&iter)))
			oid_array_append(&ಗಮ್ಯ_fetch, oid);

		promisor_remote_get_ನೇರ(opt->repo, ಗಮ್ಯ_fetch.oid, ಗಮ್ಯ_fetch.nr);

		oid_array_clear(&ಗಮ್ಯ_fetch);
	}
	oidset_clear(&blob_oids);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ grep_object(ರಚನೆ grep_opt *opt, ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
		       ರಚನೆ object *obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಆದರೆ (obj->type == OBJ_BLOB)
		ಹಿಂತಿರುಗಿ grep_oid(opt, &obj->oid, name, 0, ಮಾರ್ಗ);
	ಆದರೆ (obj->type == OBJ_COMMIT || obj->type == OBJ_ವೃಕ್ಷ) {
		ರಚನೆ ವೃಕ್ಷ_desc ವೃಕ್ಷ;
		ಶೂನ್ಯ *ದತ್ತಾಂಶ;
		ಗಾತ್ರ_t ಗಾತ್ರ_2;
		ರಚನೆ strbuf ತಳ;
		ಪೂರ್ಣಾಂಕ ದೊರಕಿತು, len;

		ದತ್ತಾಂಶ = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_ವೃಕ್ಷ, &ಗಾತ್ರ_2, ಖಾಲಿ);
		ಆದರೆ (!ದತ್ತಾಂಶ)
			die(_("unable to read tree (%s)"), oid_ಗಮ್ಯ_hex(&obj->oid));

		len = name ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name) : 0;
		strbuf_init(&ತಳ, ಮಾರ್ಗ_MAX + len + 1);
		ಆದರೆ (len) {
			strbuf_ಕೂಡಿಸು(&ತಳ, name, len);
			strbuf_addch(&ತಳ, ':');
		}
		init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ, &obj->oid, ದತ್ತಾಂಶ, ಗಾತ್ರ_2);
		ದೊರಕಿತು = grep_ವೃಕ್ಷ(opt, pathspec, &ವೃಕ್ಷ, &ತಳ, ತಳ.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&ತಳ);
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
		ಹಿಂತಿರುಗಿ ದೊರಕಿತು;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ grep_objects(ರಚನೆ grep_opt *opt, ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
			ಸ್ಥಿರಾಂಕ ರಚನೆ object_array *list)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ ದೊರಕಿತು = 0;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	ಗಾಗಿ (i = 0; i < nr; i++) {
		ರಚನೆ object *real_obj;

		obj_read_ಬೀಗ();
		real_obj = deref_ಟ್ಯಾಗ್(opt->repo, list->objects[i].ಅಂಶ,
				     ಖಾಲಿ, 0);
		obj_read_unlock();

		ಆದರೆ (!real_obj) {
			ಅಕ್ಷರ hex[GIT_MAX_HEXSZ + 1];
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = list->objects[i].name;

			ಆದರೆ (!name) {
				oid_ಗಮ್ಯ_hex_r(hex, &list->objects[i].ಅಂಶ->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		ಆದರೆ (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_ಬೀಗ();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		ಆದರೆ (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].ಮಾರ್ಗ)) {
			ದೊರಕಿತು = 1;
			ಆದರೆ (opt->ಸ್ಥಿತಿ_only)
				ನಿಲ್ಲಿಸು;
		}
	}
	ಹಿಂತಿರುಗಿ ದೊರಕಿತು;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ grep_directory(ರಚನೆ grep_opt *opt, ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
			  ಪೂರ್ಣಾಂಕ exc_std, ಪೂರ್ಣಾಂಕ use_ಸೂಚ್ಯಂಕ)
{
	ರಚನೆ dir_struct dir = DIR_INIT;
	ಪೂರ್ಣಾಂಕ i, ದೊರಕಿತು = 0;

	ಆದರೆ (!use_ಸೂಚ್ಯಂಕ)
		dir.flags |= DIR_NO_GITLINKS;
	ಆದರೆ (exc_std)
		ಸಿದ್ಧತೆ_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->ಸೂಚ್ಯಂಕ, pathspec);
	ಗಾಗಿ (i = 0; i < dir.nr; i++) {
		ದೊರಕಿತು |= grep_file(opt, dir.entries[i]->name);
		ಆದರೆ (ದೊರಕಿತು && opt->ಸ್ಥಿತಿ_only)
			ನಿಲ್ಲಿಸು;
	}
	dir_clear(&dir);
	ಹಿಂತಿರುಗಿ ದೊರಕಿತು;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ context_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			    ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *endp;

	ಆದರೆ (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಮೌಲ್ಯ = strtol(arg, (ಅಕ್ಷರ **)&endp, 10);
	ಆದರೆ (*endp) {
		ಹಿಂತಿರುಗಿ error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = ಮೌಲ್ಯ;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ file_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	ಪೂರ್ಣಾಂಕ from_stdin;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename = arg;
	FILE *patterns;
	ಪೂರ್ಣಾಂಕ lno = 0;
	ರಚನೆ strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (!*filename)
		; /* leave it as-is */
	ಇಲ್ಲದಿದ್ದರೆ
		filename = ಪೂರ್ವಪ್ರತ್ಯಯ_filename_except_for_dash(grep_ಪೂರ್ವಪ್ರತ್ಯಯ, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	ಆದರೆ (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	ಆಗ (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		ಆದರೆ (sb.len == 0)
			ಮುಂದುವರಿ;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_ನಮೂನೆ);
	}
	ಆದರೆ (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	ಆದರೆ (filename != arg)
		ಬಿಡುಗಡೆ((ಶೂನ್ಯ *)filename);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ not_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_ನಮೂನೆ(grep_opt, "--not", "command line", 0, GREP_NOT);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ and_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_ನಮೂನೆ(grep_opt, "--and", "command line", 0, GREP_AND);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ open_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_ನಮೂನೆ(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ close_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_ನಮೂನೆ(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ನಮೂನೆ_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			    ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	append_grep_ನಮೂನೆ(grep_opt, arg, "-e option", 0, GREP_ನಮೂನೆ);
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_grep(ಪೂರ್ಣಾಂಕ argc,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ದೊರಕಿತು = 0;
	ಪೂರ್ಣಾಂಕ cached = 0, untracked = 0, opt_exclude = -1;
	ಪೂರ್ಣಾಂಕ ನೋಡಿದ_dashdash = 0;
	ಪೂರ್ಣಾಂಕ external_grep_allowed__ignored;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *show_ಒಳ_pager = ಖಾಲಿ, *default_pager = "dummy";
	ರಚನೆ grep_opt opt;
	ರಚನೆ object_array list = OBJECT_ARRAY_INIT;
	ರಚನೆ pathspec pathspec;
	ರಚನೆ string_list ಮಾರ್ಗ_list = STRING_LIST_INIT_DUP;
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ dummy;
	ಪೂರ್ಣಾಂಕ use_ಸೂಚ್ಯಂಕ = 1;
	ಪೂರ್ಣಾಂಕ allow_revs;
	ಪೂರ್ಣಾಂಕ ret;

	ರಚನೆ option options[] = {
		OPT_BOOL(0, "cached", &cached,
			N_("search in index instead of in the work tree")),
		OPT_NEGBIT(0, "no-index", &use_ಸೂಚ್ಯಂಕ,
			 N_("find in contents not managed by git"), 1),
		OPT_BOOL(0, "untracked", &untracked,
			N_("search in both tracked and untracked files")),
		OPT_ಗಣ_INT(0, "exclude-standard", &opt_exclude,
			    N_("ignore files specified via '.gitignore'"), 1),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			 N_("recursively search in each submodule")),
		OPT_ಗುಂಪು(""),
		OPT_BOOL('v', "invert-match", &opt.invert,
			N_("show non-matching lines")),
		OPT_BOOL('i', "ignore-case", &opt.ignore_case,
			N_("case insensitive matching")),
		OPT_BOOL('w', "word-regexp", &opt.ಶಬ್ದ_regexp,
			N_("match patterns only at word boundaries")),
		OPT_ಗಣ_INT('a', "text", &opt.ದ್ವಿಮಾನ,
			N_("process binary files as text"), GREP_ದ್ವಿಮಾನ_ಪಠ್ಯ),
		OPT_ಗಣ_INT('I', ಖಾಲಿ, &opt.ದ್ವಿಮಾನ,
			N_("don't match patterns in binary files"),
			GREP_ದ್ವಿಮಾನ_NOMATCH),
		OPT_BOOL(0, "textconv", &opt.allow_textconv,
			 N_("process binary files with textconv filters")),
		OPT_ಗಣ_INT('r', "recursive", &opt.max_ಆಳ,
			    N_("search in subdirectories (default)"), -1),
		OPT_INTEGER_F(0, "max-depth", &opt.max_ಆಳ,
			N_("descend at most <n> levels"), PARSE_OPT_NONEG),
		OPT_ಗುಂಪು(""),
		OPT_ಗಣ_INT('E', "extended-regexp", &opt.ನಮೂನೆ_type_option,
			    N_("use extended POSIX regular expressions"),
			    GREP_ನಮೂನೆ_TYPE_ERE),
		OPT_ಗಣ_INT('G', "basic-regexp", &opt.ನಮೂನೆ_type_option,
			    N_("use basic POSIX regular expressions (default)"),
			    GREP_ನಮೂನೆ_TYPE_BRE),
		OPT_ಗಣ_INT('F', "fixed-strings", &opt.ನಮೂನೆ_type_option,
			    N_("interpret patterns as fixed strings"),
			    GREP_ನಮೂನೆ_TYPE_FIXED),
		OPT_ಗಣ_INT('P', "perl-regexp", &opt.ನಮೂನೆ_type_option,
			    N_("use Perl-compatible regular expressions"),
			    GREP_ನಮೂನೆ_TYPE_PCRE),
		OPT_ಗುಂಪು(""),
		OPT_BOOL('n', "line-number", &opt.linenum, N_("show line numbers")),
		OPT_BOOL(0, "column", &opt.columnnum, N_("show column number of first match")),
		OPT_NEGBIT('h', ಖಾಲಿ, &opt.pathname, N_("don't show filenames"), 1),
		OPT_ಬಿಟ್('H', ಖಾಲಿ, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.ಎಣಿಕೆ_2,
			N_("show the number of matches instead of matching lines")),
		OPT__COLOR(&opt.color, N_("highlight matches")),
		OPT_BOOL(0, "break", &opt.file_break,
			N_("print empty line between matches from different files")),
		OPT_BOOL(0, "heading", &opt.heading,
			N_("show filename only once above matches from same file")),
		OPT_ಗುಂಪು(""),
		OPT_CALLBACK('C', "context", &opt, N_("n"),
			N_("show <n> context lines before and after matches"),
			context_callback),
		OPT_UNSIGNED('B', "before-context", &opt.pre_context,
			N_("show <n> context lines before matches")),
		OPT_UNSIGNED('A', "after-context", &opt.post_context,
			N_("show <n> context lines after matches")),
		OPT_INTEGER(0, "threads", &ಅಂಕೆ_ಎಳೆಗಳು,
			N_("use <n> worker threads")),
		OPT_ಸಂಖ್ಯೆ_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_ಗುಂಪು(""),
		OPT_CALLBACK('f', ಖಾಲಿ, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', ಖಾಲಿ, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, ನಮೂನೆ_callback),
		OPT_CALLBACK_F(0, "and", &opt, ಖಾಲಿ,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, ಖಾಲಿ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', ಖಾಲಿ, &opt, ಖಾಲಿ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', ಖಾಲಿ, &opt, ಖಾಲಿ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			close_callback),
		OPT__QUIET(&opt.ಸ್ಥಿತಿ_only,
			   N_("indicate hit with exit status without output")),
		OPT_BOOL(0, "all-match", &opt.all_ಹೊಂದಿಕೆ,
			N_("show only matches from files that match all patterns")),
		OPT_ಗುಂಪು(""),
		{
			.type = OPTION_STRING,
			.short_name = 'O',
			.long_name = "open-files-in-pager",
			.ಮೌಲ್ಯ = &show_ಒಳ_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_ಎಣಿಕೆ,
			N_("maximum number of results per file")),
		OPT_ಕೊನೆ()
	};
	grep_ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;

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
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, grep_usage,
			     PARSE_OPT_KEEP_DASHDASH |
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ಆದರೆ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;
	}

	ಆದರೆ (use_ಸೂಚ್ಯಂಕ && !startup_info->have_repository) {
		ಪೂರ್ಣಾಂಕ fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		ಆದರೆ (fallback)
			use_ಸೂಚ್ಯಂಕ = 0;
		ಇಲ್ಲದಿದ್ದರೆ
			/* die the same way as if we did it at the beginning */
			ಸಿದ್ಧತೆ_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	ಆದರೆ (!use_ಸೂಚ್ಯಂಕ)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	ಆದರೆ (argc > 0 && !opt.ನಮೂನೆ_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	ಆದರೆ (argc > 0 && !opt.ನಮೂನೆ_list) {
		append_grep_ನಮೂನೆ(&opt, argv[0], "command line", 0,
				    GREP_ನಮೂನೆ);
		argv++;
		argc--;
	}

	ಆದರೆ (show_ಒಳ_pager == default_pager)
		show_ಒಳ_pager = git_pager(the_repository, 1);
	ಆದರೆ (show_ಒಳ_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &ಮಾರ್ಗ_list;
		opt.output = append_ಮಾರ್ಗ;
		string_list_append(&ಮಾರ್ಗ_list, show_ಒಳ_pager);
	}

	ಆದರೆ (!opt.ನಮೂನೆ_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	ಆದರೆ (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಆದರೆ (!strcmp(argv[i], "--")) {
			ನೋಡಿದ_dashdash = 1;
			ನಿಲ್ಲಿಸು;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_ಸೂಚ್ಯಂಕ && !untracked;
	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];
		ರಚನೆ object_id oid;
		ರಚನೆ object_context oc = {0};
		ರಚನೆ object *object;

		ಆದರೆ (!strcmp(arg, "--")) {
			i++;
			ನಿಲ್ಲಿಸು;
		}

		ಆದರೆ (!allow_revs) {
			ಆದರೆ (ನೋಡಿದ_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			ನಿಲ್ಲಿಸು;
		}

		ಆದರೆ (get_oid_with_context(the_repository, arg,
					 GET_OID_ದಾಖಲೆ_ಮಾರ್ಗ,
					 &oid, &oc)) {
			ಆದರೆ (ನೋಡಿದ_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			ನಿಲ್ಲಿಸು;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		ಆದರೆ (!ನೋಡಿದ_dashdash)
			verify_non_filename(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, arg);
		ಕೂಡಿಸು_object_array_with_ಮಾರ್ಗ(object, arg, &list, oc.ಬಹುಲಕ, oc.ಮಾರ್ಗ);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	ಆದರೆ (!ನೋಡಿದ_dashdash) {
		ಪೂರ್ಣಾಂಕ j;
		ಗಾಗಿ (j = i; j < argc; j++)
			verify_filename(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_ಆಳ != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv + i);
	pathspec.max_ಆಳ = opt.max_ಆಳ;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	ಆದರೆ (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	ಆದರೆ (opt.max_ಎಣಿಕೆ == 0) {
		ret = 1;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (show_ಒಳ_pager) {
		ಆದರೆ (ಅಂಕೆ_ಎಳೆಗಳು > 1)
			warning(_("invalid option combination, ignoring --threads"));
		ಅಂಕೆ_ಎಳೆಗಳು = 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!HAVE_ಎಳೆಗಳು && ಅಂಕೆ_ಎಳೆಗಳು > 1) {
		warning(_("no threads support, ignoring --threads"));
		ಅಂಕೆ_ಎಳೆಗಳು = 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಂಕೆ_ಎಳೆಗಳು < 0)
		die(_("invalid number of threads specified (%d)"), ಅಂಕೆ_ಎಳೆಗಳು);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಂಕೆ_ಎಳೆಗಳು == 0)
		ಅಂಕೆ_ಎಳೆಗಳು = HAVE_ಎಳೆಗಳು ? online_cpus() : 1;

	ಆದರೆ (ಅಂಕೆ_ಎಳೆಗಳು > 1) {
		ಆದರೆ (!HAVE_ಎಳೆಗಳು)
			BUG("Somebody got num_threads calculation wrong!");
		ಆದರೆ (!(opt.name_only || opt.unmatch_name_only || opt.ಎಣಿಕೆ_2)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_ಮೊದಲ_ಸಾಲು = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		ಆದರೆ (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		ಆದರೆ (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		ಆರಂಭ_ಎಳೆಗಳು(&opt);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	ಆದರೆ (show_ಒಳ_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	ಆದರೆ (show_ಒಳ_pager && opt.ನಮೂನೆ_list && !opt.ನಮೂನೆ_list->ಮುಂದಿನ) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pager = ಮಾರ್ಗ_list.ವಸ್ತುಗಳು[0].string;
		ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(pager);

		ಆದರೆ (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		ಆದರೆ (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&ಮಾರ್ಗ_list, "-I");

		ಆದರೆ (!strcmp("less", pager) || !strcmp("vi", pager)) {
			ರಚನೆ strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.ನಮೂನೆ_list->ನಮೂನೆ);
			string_list_append_nodup(&ಮಾರ್ಗ_list,
						 strbuf_detach(&buf, ಖಾಲಿ));
		}
	}

	ಆದರೆ (!show_ಒಳ_pager && !opt.ಸ್ಥಿತಿ_only)
		ಸಿದ್ಧತೆ_pager(the_repository);

	die_for_incompatible_opt3(!use_ಸೂಚ್ಯಂಕ, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	ಆದರೆ (!use_ಸೂಚ್ಯಂಕ || untracked) {
		ಪೂರ್ಣಾಂಕ use_exclude = (opt_exclude < 0) ? use_ಸೂಚ್ಯಂಕ : !!opt_exclude;
		ದೊರಕಿತು = grep_directory(&opt, &pathspec, use_exclude, use_ಸೂಚ್ಯಂಕ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!list.nr) {
		ಆದರೆ (!cached)
			ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);

		ದೊರಕಿತು = grep_cache(&opt, &pathspec, cached);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (cached)
			die(_("both --cached and trees are given"));

		ದೊರಕಿತು = grep_objects(&opt, &pathspec, &list);
	}

	ಆದರೆ (ಅಂಕೆ_ಎಳೆಗಳು > 1)
		ದೊರಕಿತು |= wait_all();
	ಆದರೆ (ದೊರಕಿತು && show_ಒಳ_pager)
		ಸತತ_pager(&opt, ಪೂರ್ವಪ್ರತ್ಯಯ);

	ret = !ದೊರಕಿತು;

ಹೊರಹೋಗು:
	clear_pathspec(&pathspec);
	string_list_clear(&ಮಾರ್ಗ_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	ಹಿಂತಿರುಗಿ ret;
}
