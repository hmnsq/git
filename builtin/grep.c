/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *grep_prefix;

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ recurse_submodules;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_threads;

ಸ್ಥಿರ pthread_t *threads;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
ಸಂರಚನಾ work_item {
	ಸಂರಚನಾ grep_source source;
	ವರ್ಣ done;
	ಸಂರಚನಾ strbuf out;
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
#define TODO_SIZE 128
ಸ್ಥಿರ ಸಂರಚನಾ work_item todo[TODO_SIZE];
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ todo_start;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ todo_end;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ todo_done;

/* Has all work items been added? */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ all_work_added;

ಸ್ಥಿರ ಸಂರಚನಾ repository **repos_to_free;
ಸ್ಥಿರ size_t repos_to_free_nr, repos_to_free_alloc;

/* This lock protects all the variables above. */
ಸ್ಥಿರ pthread_mutex_t grep_mutex;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ grep_lock(ಶೂನ್ಯ)
{
	pthread_mutex_lock(&grep_mutex);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ grep_unlock(ಶೂನ್ಯ)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
ಸ್ಥಿರ pthread_cond_t cond_add;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
ಸ್ಥಿರ pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
ಸ್ಥಿರ pthread_cond_t cond_result;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ skip_first_ಸಾಲು;

ಸ್ಥಿರ ಶೂನ್ಯ add_work(ಸಂರಚನಾ grep_opt *opt, ಸಂರಚನಾ grep_source *gs)
{
	ಯದಿ (opt->ದ್ವಿಮಾನ != GREP_ದ್ವಿಮಾನ_TEXT)
		grep_source_load_driver(gs, opt->repo->index);

	grep_lock();

	ಯಾವತ್ ((todo_end+1) % ARRAY_SIZE(todo) == todo_done) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_end].source = *gs;
	todo[todo_end].done = 0;
	strbuf_reset(&todo[todo_end].out);
	todo_end = (todo_end + 1) % ARRAY_SIZE(todo);

	pthread_cond_signal(&cond_add);
	grep_unlock();
}

ಸ್ಥಿರ ಸಂರಚನಾ work_item *get_work(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ work_item *ret;

	grep_lock();
	ಯಾವತ್ (todo_start == todo_end && !all_work_added) {
		pthread_cond_wait(&cond_add, &grep_mutex);
	}

	ಯದಿ (todo_start == todo_end && all_work_added) {
		ret = ರಿಕ್ತ;
	} ಅನ್ಯಥಾ {
		ret = &todo[todo_start];
		todo_start = (todo_start + 1) % ARRAY_SIZE(todo);
	}
	grep_unlock();
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ work_done(ಸಂರಚನಾ work_item *w)
{
	ಪೂರ್ಣಾಙ್ಕ old_done;

	grep_lock();
	w->done = 1;
	old_done = todo_done;
	ಪ್ರತಿ(; todo[todo_done].done && todo_done != todo_start;
	    todo_done = (todo_done+1) % ARRAY_SIZE(todo)) {
		w = &todo[todo_done];
		ಯದಿ (w->out.len) {
			ಧ್ರುವ ವರ್ಣ *p = w->out.buf;
			size_t len = w->out.len;

			/* Skip the leading hunk mark of the first file. */
			ಯದಿ (skip_first_ಸಾಲು) {
				ಯಾವತ್ (len) {
					len--;
					ಯದಿ (*p++ == '\n')
						ಭಙ್ಗ;
				}
				skip_first_ಸಾಲು = 0;
			}

			write_or_die(1, p, len);
		}
		grep_source_clear(&w->source);
	}

	ಯದಿ (old_done != todo_done)
		pthread_cond_signal(&cond_write);

	ಯದಿ (all_work_added && todo_done == todo_end)
		pthread_cond_signal(&cond_result);

	grep_unlock();
}

ಸ್ಥಿರ ಶೂನ್ಯ free_repos(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < repos_to_free_nr; i++) {
		repo_clear(repos_to_free[i]);
		ಮುಕ್ತ(repos_to_free[i]);
	}
	FREE_AND_NULL(repos_to_free);
	repos_to_free_nr = 0;
	repos_to_free_alloc = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ *run(ಶೂನ್ಯ *arg)
{
	ಪೂರ್ಣಾಙ್ಕ hit = 0;
	ಸಂರಚನಾ grep_opt *opt = arg;

	ಯಾವತ್ (1) {
		ಸಂರಚನಾ work_item *w = get_work();
		ಯದಿ (!w)
			ಭಙ್ಗ;

		opt->output_priv = w;
		hit |= grep_source(opt, &w->source);
		grep_source_clear_data(&w->source);
		work_done(w);
	}
	free_grep_patterns(opt);
	ಮುಕ್ತ(opt);

	ಪ್ರತ್ಯಾಗಮ (ಶೂನ್ಯ*) (intptr_t) hit;
}

ಸ್ಥಿರ ಶೂನ್ಯ strbuf_out(ಸಂರಚನಾ grep_opt *opt, ಧ್ರುವ ಶೂನ್ಯ *buf, size_t size)
{
	ಸಂರಚನಾ work_item *w = opt->output_priv;
	strbuf_add(&w->out, buf, size);
}

ಸ್ಥಿರ ಶೂನ್ಯ start_threads(ಸಂರಚನಾ grep_opt *opt)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	pthread_mutex_init(&grep_mutex, ರಿಕ್ತ);
	pthread_mutex_init(&grep_attr_mutex, ರಿಕ್ತ);
	pthread_cond_init(&cond_add, ರಿಕ್ತ);
	pthread_cond_init(&cond_write, ರಿಕ್ತ);
	pthread_cond_init(&cond_result, ರಿಕ್ತ);
	grep_use_locks = 1;
	enable_obj_read_lock();

	ಪ್ರತಿ (i = 0; i < ARRAY_SIZE(todo); i++) {
		strbuf_init(&todo[i].out, 0);
	}

	CALLOC_ARRAY(threads, ಅಂಕೆ_threads);
	ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_threads; i++) {
		ಪೂರ್ಣಾಙ್ಕ err;
		ಸಂರಚನಾ grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_out;
		compile_grep_patterns(o);
		err = pthread_create(&threads[i], ರಿಕ್ತ, run, o);

		ಯದಿ (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ wait_all(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ hit = 0;
	ಪೂರ್ಣಾಙ್ಕ i;

	ಯದಿ (!HAVE_THREADS)
		BUG("Never call this function unless you have started threads");

	grep_lock();
	all_work_added = 1;

	/* Wait until all work is done. */
	ಯಾವತ್ (todo_done != todo_end)
		pthread_cond_wait(&cond_result, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_broadcast(&cond_add);
	grep_unlock();

	ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_threads; i++) {
		ಶೂನ್ಯ *h;
		pthread_join(threads[i], &h);
		hit |= (ಪೂರ್ಣಾಙ್ಕ) (intptr_t) h;
	}

	ಮುಕ್ತ(threads);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_add);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_result);
	grep_use_locks = 0;
	disable_obj_read_lock();

	ಪ್ರತ್ಯಾಗಮ hit;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ grep_cmd_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			   ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಪೂರ್ಣಾಙ್ಕ st = grep_config(var, ಮೌಲ್ಯ, ctx, cb);

	ಯದಿ (git_color_config(var, ಮೌಲ್ಯ, cb) < 0)
		st = -1;
	ಅನ್ಯಥಾ ಯದಿ (git_default_config(var, ಮೌಲ್ಯ, ctx, cb) < 0)
		st = -1;

	ಯದಿ (!strcmp(var, "grep.threads")) {
		ಅಂಕೆ_threads = git_config_int(var, ಮೌಲ್ಯ, ctx->kvi);
		ಯದಿ (ಅಂಕೆ_threads < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    ಅಂಕೆ_threads, var);
		ಅನ್ಯಥಾ ಯದಿ (!HAVE_THREADS && ಅಂಕೆ_threads > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			ಅಂಕೆ_threads = 1;
		}
	}

	ಯದಿ (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, ಮೌಲ್ಯ);

	ಪ್ರತ್ಯಾಗಮ st;
}

ಸ್ಥಿರ ಶೂನ್ಯ grep_source_name(ಸಂರಚನಾ grep_opt *opt, ಧ್ರುವ ವರ್ಣ *filename,
			     ಪೂರ್ಣಾಙ್ಕ ವೃಕ್ಷ_name_len, ಸಂರಚನಾ strbuf *out)
{
	strbuf_reset(out);

	ಯದಿ (opt->null_following_name) {
		ಯದಿ (opt->relative && grep_prefix) {
			ಸಂರಚನಾ strbuf rel_buf = STRBUF_INIT;
			ಧ್ರುವ ವರ್ಣ *rel_name =
				relative_path(filename + ವೃಕ್ಷ_name_len,
					      grep_prefix, &rel_buf);

			ಯದಿ (ವೃಕ್ಷ_name_len)
				strbuf_add(out, filename, ವೃಕ್ಷ_name_len);

			strbuf_addstr(out, rel_name);
			strbuf_release(&rel_buf);
		} ಅನ್ಯಥಾ {
			strbuf_addstr(out, filename);
		}
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (opt->relative && grep_prefix)
		quote_path(filename + ವೃಕ್ಷ_name_len, grep_prefix, out, 0);
	ಅನ್ಯಥಾ
		quote_c_style(filename + ವೃಕ್ಷ_name_len, out, ರಿಕ್ತ, 0);

	ಯದಿ (ವೃಕ್ಷ_name_len)
		strbuf_ಸೇರಿಸು(out, 0, filename, ವೃಕ್ಷ_name_len);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ grep_oid(ಸಂರಚನಾ grep_opt *opt, ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
		     ಧ್ರುವ ವರ್ಣ *filename, ಪೂರ್ಣಾಙ್ಕ ವೃಕ್ಷ_name_len,
		     ಧ್ರುವ ವರ್ಣ *path)
{
	ಸಂರಚನಾ strbuf pathbuf = STRBUF_INIT;
	ಸಂರಚನಾ grep_source gs;

	grep_source_name(opt, filename, ವೃಕ್ಷ_name_len, &pathbuf);
	grep_source_init_oid(&gs, pathbuf.buf, path, oid, opt->repo);
	strbuf_release(&pathbuf);

	ಯದಿ (ಅಂಕೆ_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ {
		ಪೂರ್ಣಾಙ್ಕ hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		ಪ್ರತ್ಯಾಗಮ hit;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ grep_file(ಸಂರಚನಾ grep_opt *opt, ಧ್ರುವ ವರ್ಣ *filename)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ grep_source gs;

	grep_source_name(opt, filename, 0, &buf);
	grep_source_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	ಯದಿ (ಅಂಕೆ_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ {
		ಪೂರ್ಣಾಙ್ಕ hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		ಪ್ರತ್ಯಾಗಮ hit;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ append_path(ಸಂರಚನಾ grep_opt *opt, ಧ್ರುವ ಶೂನ್ಯ *data, size_t len)
{
	ಸಂರಚನಾ string_list *path_list = opt->output_priv;

	ಯದಿ (len == 1 && *(ಧ್ರುವ ವರ್ಣ *)data == '\0')
		ಪ್ರತ್ಯಾಗಮ;
	string_list_append_nodup(path_list, xstrndup(data, len));
}

ಸ್ಥಿರ ಶೂನ್ಯ run_pager(ಸಂರಚನಾ grep_opt *opt, ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಸಂರಚನಾ string_list *path_list = opt->output_priv;
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;
	ಪೂರ್ಣಾಙ್ಕ i, status;

	ಪ್ರತಿ (i = 0; i < path_list->nr; i++)
		strvec_ತಳ್ಳು(&child.args, path_list->ವಸ್ತುಗಳು[i].string);
	child.dir = prefix;
	child.use_shell = 1;

	status = run_command(&child);
	ಯದಿ (status)
		exit(status);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ grep_cache(ಸಂರಚನಾ grep_opt *opt,
		      ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec, ಪೂರ್ಣಾಙ್ಕ cached);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ grep_ವೃಕ್ಷ(ಸಂರಚನಾ grep_opt *opt, ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec,
		     ಸಂರಚನಾ ವೃಕ್ಷ_desc *ವೃಕ್ಷ, ಸಂರಚನಾ strbuf *base, ಪೂರ್ಣಾಙ್ಕ tn_len,
		     ಪೂರ್ಣಾಙ್ಕ check_attr);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ grep_submodule(ಸಂರಚನಾ grep_opt *opt,
			  ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec,
			  ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
			  ಧ್ರುವ ವರ್ಣ *filename, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ cached)
{
	ಸಂರಚನಾ repository *subrepo;
	ಸಂರಚನಾ repository *superproject = opt->repo;
	ಸಂರಚನಾ grep_opt subopt;
	ಪೂರ್ಣಾಙ್ಕ hit = 0;

	ಯದಿ (!is_submodule_active(superproject, path))
		ಪ್ರತ್ಯಾಗಮ 0;

	subrepo = xmalloc(ಪರಿಮಾಣ(*subrepo));
	ಯದಿ (repo_submodule_init(subrepo, superproject, path, null_oid(opt->repo->hash_algo))) {
		ಮುಕ್ತ(subrepo);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ALLOC_GROW(repos_to_free, repos_to_free_nr + 1, repos_to_free_alloc);
	repos_to_free[repos_to_free_nr++] = subrepo;

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
	obj_read_lock();
	repo_read_gitmodules(subrepo, 0);
	obj_read_unlock();

	memcpy(&subopt, opt, ಪರಿಮಾಣ(subopt));
	subopt.repo = subrepo;

	ಯದಿ (oid) {
		ಗಣನಾ object_type object_type;
		ಸಂರಚನಾ ವೃಕ್ಷ_desc ವೃಕ್ಷ;
		ಶೂನ್ಯ *data;
		size_t size;
		ಸಂರಚನಾ strbuf base = STRBUF_INIT;

		obj_read_lock();
		object_type = odb_read_object_info(subrepo->objects, oid, ರಿಕ್ತ);
		obj_read_unlock();
		data = odb_read_object_peeled(subrepo->objects, oid, OBJ_ವೃಕ್ಷ, &size, ರಿಕ್ತ);
		ಯದಿ (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(oid));

		strbuf_addstr(&base, filename);
		strbuf_addch(&base, '/');

		init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ, oid, data, size);
		hit = grep_ವೃಕ್ಷ(&subopt, pathspec, &ವೃಕ್ಷ, &base, base.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&base);
		ಮುಕ್ತ(data);
	} ಅನ್ಯಥಾ {
		hit = grep_cache(&subopt, pathspec, cached);
	}

	ಪ್ರತ್ಯಾಗಮ hit;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ grep_cache(ಸಂರಚನಾ grep_opt *opt,
		      ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec, ಪೂರ್ಣಾಙ್ಕ cached)
{
	ಸಂರಚನಾ repository *repo = opt->repo;
	ಪೂರ್ಣಾಙ್ಕ hit = 0;
	ಪೂರ್ಣಾಙ್ಕ nr;
	ಸಂರಚನಾ strbuf name = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ name_base_len = 0;
	ಯದಿ (repo->submodule_prefix) {
		name_base_len = strlen(repo->submodule_prefix);
		strbuf_addstr(&name, repo->submodule_prefix);
	}

	ಯದಿ (repo_read_index(repo) < 0)
		die(_("index file corrupt"));

	ಪ್ರತಿ (nr = 0; nr < repo->index->cache_nr; nr++) {
		ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce = repo->index->cache[nr];

		ಯದಿ (!cached && ce_skip_worktree(ce))
			ಅನುವರ್ತ;

		strbuf_setlen(&name, name_base_len);
		strbuf_addstr(&name, ce->name);
		ಯದಿ (S_ISSPARSEDIR(ce->ce_mode)) {
			ಗಣನಾ object_type type;
			ಸಂರಚನಾ ವೃಕ್ಷ_desc ವೃಕ್ಷ;
			ಶೂನ್ಯ *data;
			size_t size;

			data = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &size);
			ಯದಿ (!data)
				die(_("unable to read tree %s"), oid_to_hex(&ce->oid));
			init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ, &ce->oid, data, size);

			hit |= grep_ವೃಕ್ಷ(opt, pathspec, &ವೃಕ್ಷ, &name, 0, 0);
			strbuf_setlen(&name, name_base_len);
			strbuf_addstr(&name, ce->name);
			ಮುಕ್ತ(data);
		} ಅನ್ಯಥಾ ಯದಿ (S_ISREG(ce->ce_mode) &&
		    match_pathspec(repo->index, pathspec, name.buf, name.len, 0, ರಿಕ್ತ,
				   S_ISDIR(ce->ce_mode) ||
				   S_ISGITLINK(ce->ce_mode))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			ಯದಿ (cached || (ce->ce_flags & CE_VALID)) {
				ಯದಿ (ce_stage(ce) || ce_intent_to_add(ce))
					ಅನುವರ್ತ;
				hit |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} ಅನ್ಯಥಾ {
				hit |= grep_file(opt, name.buf);
			}
		} ಅನ್ಯಥಾ ಯದಿ (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
			   submodule_path_match(repo->index, pathspec, name.buf, ರಿಕ್ತ)) {
			hit |= grep_submodule(opt, pathspec, ರಿಕ್ತ, ce->name,
					      ce->name, cached);
		} ಅನ್ಯಥಾ {
			ಅನುವರ್ತ;
		}

		ಯದಿ (ce_stage(ce)) {
			ಕುರು {
				nr++;
			} ಯಾವತ್ (nr < repo->index->cache_nr &&
				 !strcmp(ce->name, repo->index->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		ಯದಿ (hit && opt->status_only)
			ಭಙ್ಗ;
	}

	strbuf_release(&name);
	ಪ್ರತ್ಯಾಗಮ hit;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ grep_ವೃಕ್ಷ(ಸಂರಚನಾ grep_opt *opt, ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec,
		     ಸಂರಚನಾ ವೃಕ್ಷ_desc *ವೃಕ್ಷ, ಸಂರಚನಾ strbuf *base, ಪೂರ್ಣಾಙ್ಕ tn_len,
		     ಪೂರ್ಣಾಙ್ಕ check_attr)
{
	ಸಂರಚನಾ repository *repo = opt->repo;
	ಪೂರ್ಣಾಙ್ಕ hit = 0;
	ಗಣನಾ interesting match = entry_not_interesting;
	ಸಂರಚನಾ name_entry entry;
	ಪೂರ್ಣಾಙ್ಕ old_baselen = base->len;
	ಸಂರಚನಾ strbuf name = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ name_base_len = 0;
	ಯದಿ (repo->submodule_prefix) {
		strbuf_addstr(&name, repo->submodule_prefix);
		name_base_len = name.len;
	}

	ಯಾವತ್ (ವೃಕ್ಷ_entry(ವೃಕ್ಷ, &entry)) {
		ಪೂರ್ಣಾಙ್ಕ te_len = ವೃಕ್ಷ_entry_len(&entry);

		ಯದಿ (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = ವೃಕ್ಷ_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_setlen(&name, name_base_len);

			ಯದಿ (match == all_entries_not_interesting)
				ಭಙ್ಗ;
			ಯದಿ (match == entry_not_interesting)
				ಅನುವರ್ತ;
		}

		strbuf_add(base, entry.path, te_len);

		ಯದಿ (S_ISREG(entry.mode)) {
			hit |= grep_oid(opt, &entry.oid, base->buf, tn_len,
					 check_attr ? base->buf + tn_len : ರಿಕ್ತ);
		} ಅನ್ಯಥಾ ಯದಿ (S_ISDIR(entry.mode)) {
			ಗಣನಾ object_type type;
			ಸಂರಚನಾ ವೃಕ್ಷ_desc sub;
			ಶೂನ್ಯ *data;
			size_t size;

			data = odb_read_object(the_repository->objects,
					       &entry.oid, &type, &size);
			ಯದಿ (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_ವೃಕ್ಷ_desc(&sub, &entry.oid, data, size);
			hit |= grep_ವೃಕ್ಷ(opt, pathspec, &sub, base, tn_len,
					 check_attr);
			ಮುಕ್ತ(data);
		} ಅನ್ಯಥಾ ಯದಿ (recurse_submodules && S_ISGITLINK(entry.mode)) {
			hit |= grep_submodule(opt, pathspec, &entry.oid,
					      base->buf, base->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(base, old_baselen);

		ಯದಿ (hit && opt->status_only)
			ಭಙ್ಗ;
	}

	strbuf_release(&name);
	ಪ್ರತ್ಯಾಗಮ hit;
}

ಸ್ಥಿರ ಶೂನ್ಯ collect_blob_oids_for_ವೃಕ್ಷ(ಸಂರಚನಾ repository *repo,
				       ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec,
				       ಸಂರಚನಾ ವೃಕ್ಷ_desc *ವೃಕ್ಷ,
				       ಸಂರಚನಾ strbuf *base,
				       ಪೂರ್ಣಾಙ್ಕ tn_len,
				       ಸಂರಚನಾ oidset *blob_oids)
{
	ಸಂರಚನಾ name_entry entry;
	ಪೂರ್ಣಾಙ್ಕ old_baselen = base->len;
	ಸಂರಚನಾ strbuf name = STRBUF_INIT;
	ಗಣನಾ interesting match = entry_not_interesting;

	ಯಾವತ್ (ವೃಕ್ಷ_entry(ವೃಕ್ಷ, &entry)) {
		ಯದಿ (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = ವೃಕ್ಷ_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_reset(&name);

			ಯದಿ (match == all_entries_not_interesting)
				ಭಙ್ಗ;
			ಯದಿ (match == entry_not_interesting)
				ಅನುವರ್ತ;
		}

		strbuf_add(base, entry.path, ವೃಕ್ಷ_entry_len(&entry));

		ಯದಿ (S_ISREG(entry.mode)) {
			ಯದಿ (!odb_has_object(repo->objects, &entry.oid, 0))
				oidset_ಸೇರಿಸು(blob_oids, &entry.oid);
		} ಅನ್ಯಥಾ ಯದಿ (S_ISDIR(entry.mode)) {
			ಗಣನಾ object_type type;
			ಸಂರಚನಾ ವೃಕ್ಷ_desc sub_ವೃಕ್ಷ;
			ಶೂನ್ಯ *data;
			size_t size;

			data = odb_read_object(repo->objects, &entry.oid,
					       &type, &size);
			ಯದಿ (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_ವೃಕ್ಷ_desc(&sub_ವೃಕ್ಷ, &entry.oid, data, size);
			collect_blob_oids_for_ವೃಕ್ಷ(repo, pathspec, &sub_ವೃಕ್ಷ,
						   base, tn_len, blob_oids);
			ಮುಕ್ತ(data);
		}
		/*
		 * ...no else clause for S_ISGITLINK: submodules have their
		 * own promisor configuration and would need separate fetches
		 * anyway.
		 */

		strbuf_setlen(base, old_baselen);
	}

	strbuf_release(&name);
}

ಸ್ಥಿರ ಶೂನ್ಯ collect_blob_oids_for_treeish(ಸಂರಚನಾ grep_opt *opt,
					  ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec,
					  ಧ್ರುವ ಸಂರಚನಾ object_id *ವೃಕ್ಷ_ish_oid,
					  ಧ್ರುವ ವರ್ಣ *name,
					  ಸಂರಚನಾ oidset *blob_oids)
{
	ಸಂರಚನಾ ವೃಕ್ಷ_desc ವೃಕ್ಷ;
	ಶೂನ್ಯ *data;
	size_t size;
	ಸಂರಚನಾ strbuf base = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ len;

	data = odb_read_object_peeled(opt->repo->objects, ವೃಕ್ಷ_ish_oid,
				      OBJ_ವೃಕ್ಷ, &size, ರಿಕ್ತ);

	ಯದಿ (!data)
		ಪ್ರತ್ಯಾಗಮ;

	len = name ? strlen(name) : 0;
	ಯದಿ (len) {
		strbuf_add(&base, name, len);
		strbuf_addch(&base, ':');
	}
	init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ, ವೃಕ್ಷ_ish_oid, data, size);

	collect_blob_oids_for_ವೃಕ್ಷ(opt->repo, pathspec, &ವೃಕ್ಷ,
				   &base, base.len, blob_oids);

	strbuf_release(&base);
	ಮುಕ್ತ(data);
}

ಸ್ಥಿರ ಶೂನ್ಯ prefetch_grep_blobs(ಸಂರಚನಾ grep_opt *opt,
				ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec,
				ಧ್ರುವ ಸಂರಚನಾ object_array *list)
{
	ಸಂರಚನಾ oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	ಯದಿ (!repo_has_promisor_remote(opt->repo))
		ಪ್ರತ್ಯಾಗಮ;

	/* For each tree, gather the blobs in it */
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < list->nr; i++) {
		ಸಂರಚನಾ object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     ರಿಕ್ತ, 0);
		obj_read_unlock();

		ಯದಿ (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_ವೃಕ್ಷ))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	ಯದಿ (oidset_size(&blob_oids)) {
		ಸಂರಚನಾ oid_array to_fetch = OID_ARRAY_INIT;
		ಸಂರಚನಾ oidset_iter iter;
		ಧ್ರುವ ಸಂರಚನಾ object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		ಯಾವತ್ ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(opt->repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}
	oidset_clear(&blob_oids);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ grep_object(ಸಂರಚನಾ grep_opt *opt, ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec,
		       ಸಂರಚನಾ object *obj, ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *path)
{
	ಯದಿ (obj->type == OBJ_BLOB)
		ಪ್ರತ್ಯಾಗಮ grep_oid(opt, &obj->oid, name, 0, path);
	ಯದಿ (obj->type == OBJ_COMMIT || obj->type == OBJ_ವೃಕ್ಷ) {
		ಸಂರಚನಾ ವೃಕ್ಷ_desc ವೃಕ್ಷ;
		ಶೂನ್ಯ *data;
		size_t size;
		ಸಂರಚನಾ strbuf base;
		ಪೂರ್ಣಾಙ್ಕ hit, len;

		data = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_ವೃಕ್ಷ, &size, ರಿಕ್ತ);
		ಯದಿ (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(&obj->oid));

		len = name ? strlen(name) : 0;
		strbuf_init(&base, PATH_MAX + len + 1);
		ಯದಿ (len) {
			strbuf_add(&base, name, len);
			strbuf_addch(&base, ':');
		}
		init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ, &obj->oid, data, size);
		hit = grep_ವೃಕ್ಷ(opt, pathspec, &ವೃಕ್ಷ, &base, base.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&base);
		ಮುಕ್ತ(data);
		ಪ್ರತ್ಯಾಗಮ hit;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ grep_objects(ಸಂರಚನಾ grep_opt *opt, ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec,
			ಧ್ರುವ ಸಂರಚನಾ object_array *list)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ hit = 0;
	ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	ಪ್ರತಿ (i = 0; i < nr; i++) {
		ಸಂರಚನಾ object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     ರಿಕ್ತ, 0);
		obj_read_unlock();

		ಯದಿ (!real_obj) {
			ವರ್ಣ hex[GIT_MAX_HEXSZ + 1];
			ಧ್ರುವ ವರ್ಣ *name = list->objects[i].name;

			ಯದಿ (!name) {
				oid_to_hex_r(hex, &list->objects[i].item->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		ಯದಿ (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_lock();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		ಯದಿ (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].path)) {
			hit = 1;
			ಯದಿ (opt->status_only)
				ಭಙ್ಗ;
		}
	}
	ಪ್ರತ್ಯಾಗಮ hit;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ grep_directory(ಸಂರಚನಾ grep_opt *opt, ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec,
			  ಪೂರ್ಣಾಙ್ಕ exc_std, ಪೂರ್ಣಾಙ್ಕ use_index)
{
	ಸಂರಚನಾ dir_struct dir = DIR_INIT;
	ಪೂರ್ಣಾಙ್ಕ i, hit = 0;

	ಯದಿ (!use_index)
		dir.flags |= DIR_NO_GITLINKS;
	ಯದಿ (exc_std)
		setup_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->index, pathspec);
	ಪ್ರತಿ (i = 0; i < dir.nr; i++) {
		hit |= grep_file(opt, dir.entries[i]->name);
		ಯದಿ (hit && opt->status_only)
			ಭಙ್ಗ;
	}
	dir_clear(&dir);
	ಪ್ರತ್ಯಾಗಮ hit;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ context_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
			    ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;
	ಧ್ರುವ ವರ್ಣ *endp;

	ಯದಿ (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಮೌಲ್ಯ = strtol(arg, (ವರ್ಣ **)&endp, 10);
	ಯದಿ (*endp) {
		ಪ್ರತ್ಯಾಗಮ error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = ಮೌಲ್ಯ;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ file_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	ಪೂರ್ಣಾಙ್ಕ from_stdin;
	ಧ್ರುವ ವರ್ಣ *filename = arg;
	FILE *patterns;
	ಪೂರ್ಣಾಙ್ಕ lno = 0;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	ಯದಿ (!*filename)
		; /* leave it as-is */
	ಅನ್ಯಥಾ
		filename = prefix_filename_except_for_dash(grep_prefix, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	ಯದಿ (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	ಯಾವತ್ (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		ಯದಿ (sb.len == 0)
			ಅನುವರ್ತ;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_PATTERN);
	}
	ಯದಿ (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	ಯದಿ (filename != arg)
		ಮುಕ್ತ((ಶೂನ್ಯ *)filename);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ not_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--not", "command line", 0, GREP_NOT);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ and_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--and", "command line", 0, GREP_AND);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ open_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ close_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pattern_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
			    ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ grep_opt *grep_opt = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	append_grep_pattern(grep_opt, arg, "-e option", 0, GREP_PATTERN);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_grep(ಪೂರ್ಣಾಙ್ಕ argc,
	     ಧ್ರುವ ವರ್ಣ **argv,
	     ಧ್ರುವ ವರ್ಣ *prefix,
	     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ hit = 0;
	ಪೂರ್ಣಾಙ್ಕ cached = 0, untracked = 0, opt_exclude = -1;
	ಪೂರ್ಣಾಙ್ಕ seen_dashdash = 0;
	ಪೂರ್ಣಾಙ್ಕ external_grep_allowed__ignored;
	ಧ್ರುವ ವರ್ಣ *show_in_pager = ರಿಕ್ತ, *default_pager = "dummy";
	ಸಂರಚನಾ grep_opt opt;
	ಸಂರಚನಾ object_array list = OBJECT_ARRAY_INIT;
	ಸಂರಚನಾ pathspec pathspec;
	ಸಂರಚನಾ string_list path_list = STRING_LIST_INIT_DUP;
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ dummy;
	ಪೂರ್ಣಾಙ್ಕ use_index = 1;
	ಪೂರ್ಣಾಙ್ಕ allow_revs;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಸಂರಚನಾ option options[] = {
		OPT_BOOL(0, "cached", &cached,
			N_("search in index instead of in the work tree")),
		OPT_NEGBIT(0, "no-index", &use_index,
			 N_("find in contents not managed by git"), 1),
		OPT_BOOL(0, "untracked", &untracked,
			N_("search in both tracked and untracked files")),
		OPT_SET_INT(0, "exclude-standard", &opt_exclude,
			    N_("ignore files specified via '.gitignore'"), 1),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			 N_("recursively search in each submodule")),
		OPT_GROUP(""),
		OPT_BOOL('v', "invert-match", &opt.invert,
			N_("show non-matching lines")),
		OPT_BOOL('i', "ignore-case", &opt.ignore_case,
			N_("case insensitive matching")),
		OPT_BOOL('w', "word-regexp", &opt.word_regexp,
			N_("match patterns only at word boundaries")),
		OPT_SET_INT('a', "text", &opt.ದ್ವಿಮಾನ,
			N_("process binary files as text"), GREP_ದ್ವಿಮಾನ_TEXT),
		OPT_SET_INT('I', ರಿಕ್ತ, &opt.ದ್ವಿಮಾನ,
			N_("don't match patterns in binary files"),
			GREP_ದ್ವಿಮಾನ_NOMATCH),
		OPT_BOOL(0, "textconv", &opt.allow_textconv,
			 N_("process binary files with textconv filters")),
		OPT_SET_INT('r', "recursive", &opt.max_depth,
			    N_("search in subdirectories (default)"), -1),
		OPT_INTEGER_F(0, "max-depth", &opt.max_depth,
			N_("descend at most <n> levels"), PARSE_OPT_NONEG),
		OPT_GROUP(""),
		OPT_SET_INT('E', "extended-regexp", &opt.pattern_type_option,
			    N_("use extended POSIX regular expressions"),
			    GREP_PATTERN_TYPE_ERE),
		OPT_SET_INT('G', "basic-regexp", &opt.pattern_type_option,
			    N_("use basic POSIX regular expressions (default)"),
			    GREP_PATTERN_TYPE_BRE),
		OPT_SET_INT('F', "fixed-strings", &opt.pattern_type_option,
			    N_("interpret patterns as fixed strings"),
			    GREP_PATTERN_TYPE_FIXED),
		OPT_SET_INT('P', "perl-regexp", &opt.pattern_type_option,
			    N_("use Perl-compatible regular expressions"),
			    GREP_PATTERN_TYPE_PCRE),
		OPT_GROUP(""),
		OPT_BOOL('n', "line-number", &opt.linenum, N_("show line numbers")),
		OPT_BOOL(0, "column", &opt.columnnum, N_("show column number of first match")),
		OPT_NEGBIT('h', ರಿಕ್ತ, &opt.pathname, N_("don't show filenames"), 1),
		OPT_BIT('H', ರಿಕ್ತ, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.ಎಣಿಕೆ,
			N_("show the number of matches instead of matching lines")),
		OPT__COLOR(&opt.color, N_("highlight matches")),
		OPT_BOOL(0, "break", &opt.file_break,
			N_("print empty line between matches from different files")),
		OPT_BOOL(0, "heading", &opt.heading,
			N_("show filename only once above matches from same file")),
		OPT_GROUP(""),
		OPT_CALLBACK('C', "context", &opt, N_("n"),
			N_("show <n> context lines before and after matches"),
			context_callback),
		OPT_UNSIGNED('B', "before-context", &opt.pre_context,
			N_("show <n> context lines before matches")),
		OPT_UNSIGNED('A', "after-context", &opt.post_context,
			N_("show <n> context lines after matches")),
		OPT_INTEGER(0, "threads", &ಅಂಕೆ_threads,
			N_("use <n> worker threads")),
		OPT_ಸಂಖ್ಯೆ_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_GROUP(""),
		OPT_CALLBACK('f', ರಿಕ್ತ, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', ರಿಕ್ತ, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, pattern_callback),
		OPT_CALLBACK_F(0, "and", &opt, ರಿಕ್ತ,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, ರಿಕ್ತ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', ರಿಕ್ತ, &opt, ರಿಕ್ತ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', ರಿಕ್ತ, &opt, ರಿಕ್ತ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			close_callback),
		OPT__QUIET(&opt.status_only,
			   N_("indicate hit with exit status without output")),
		OPT_BOOL(0, "all-match", &opt.all_match,
			N_("show only matches from files that match all patterns")),
		OPT_GROUP(""),
		{
			.type = OPTION_STRING,
			.short_name = 'O',
			.long_name = "open-files-in-pager",
			.ಮೌಲ್ಯ = &show_in_pager,
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
		OPT_END()
	};
	grep_prefix = prefix;

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
	argc = parse_options(argc, argv, prefix, options, grep_usage,
			     PARSE_OPT_KEEP_DASHDASH |
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ಯದಿ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	ಯದಿ (use_index && !startup_info->have_repository) {
		ಪೂರ್ಣಾಙ್ಕ fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		ಯದಿ (fallback)
			use_index = 0;
		ಅನ್ಯಥಾ
			/* die the same way as if we did it at the beginning */
			setup_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	ಯದಿ (!use_index)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	ಯದಿ (argc > 0 && !opt.pattern_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	ಯದಿ (argc > 0 && !opt.pattern_list) {
		append_grep_pattern(&opt, argv[0], "command line", 0,
				    GREP_PATTERN);
		argv++;
		argc--;
	}

	ಯದಿ (show_in_pager == default_pager)
		show_in_pager = git_pager(the_repository, 1);
	ಯದಿ (show_in_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &path_list;
		opt.output = append_path;
		string_list_append(&path_list, show_in_pager);
	}

	ಯದಿ (!opt.pattern_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	ಯದಿ (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಯದಿ (!strcmp(argv[i], "--")) {
			seen_dashdash = 1;
			ಭಙ್ಗ;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_index && !untracked;
	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[i];
		ಸಂರಚನಾ object_id oid;
		ಸಂರಚನಾ object_context oc = {0};
		ಸಂರಚನಾ object *object;

		ಯದಿ (!strcmp(arg, "--")) {
			i++;
			ಭಙ್ಗ;
		}

		ಯದಿ (!allow_revs) {
			ಯದಿ (seen_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			ಭಙ್ಗ;
		}

		ಯದಿ (get_oid_with_context(the_repository, arg,
					 GET_OID_RECORD_PATH,
					 &oid, &oc)) {
			ಯದಿ (seen_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			ಭಙ್ಗ;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		ಯದಿ (!seen_dashdash)
			verify_non_filename(the_repository, prefix, arg);
		add_object_array_with_path(object, arg, &list, oc.mode, oc.path);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	ಯದಿ (!seen_dashdash) {
		ಪೂರ್ಣಾಙ್ಕ j;
		ಪ್ರತಿ (j = i; j < argc; j++)
			verify_filename(the_repository, prefix, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_depth != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       prefix, argv + i);
	pathspec.max_depth = opt.max_depth;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	ಯದಿ (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	ಯದಿ (opt.max_ಎಣಿಕೆ == 0) {
		ret = 1;
		ಗಚ್ಛ out;
	}

	ಯದಿ (show_in_pager) {
		ಯದಿ (ಅಂಕೆ_threads > 1)
			warning(_("invalid option combination, ignoring --threads"));
		ಅಂಕೆ_threads = 1;
	} ಅನ್ಯಥಾ ಯದಿ (!HAVE_THREADS && ಅಂಕೆ_threads > 1) {
		warning(_("no threads support, ignoring --threads"));
		ಅಂಕೆ_threads = 1;
	} ಅನ್ಯಥಾ ಯದಿ (ಅಂಕೆ_threads < 0)
		die(_("invalid number of threads specified (%d)"), ಅಂಕೆ_threads);
	ಅನ್ಯಥಾ ಯದಿ (ಅಂಕೆ_threads == 0)
		ಅಂಕೆ_threads = HAVE_THREADS ? online_cpus() : 1;

	ಯದಿ (ಅಂಕೆ_threads > 1) {
		ಯದಿ (!HAVE_THREADS)
			BUG("Somebody got num_threads calculation wrong!");
		ಯದಿ (!(opt.name_only || opt.unmatch_name_only || opt.ಎಣಿಕೆ)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_first_ಸಾಲು = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		ಯದಿ (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		ಯದಿ (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		start_threads(&opt);
	} ಅನ್ಯಥಾ {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	ಯದಿ (show_in_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	ಯದಿ (show_in_pager && opt.pattern_list && !opt.pattern_list->next) {
		ಧ್ರುವ ವರ್ಣ *pager = path_list.ವಸ್ತುಗಳು[0].string;
		ಪೂರ್ಣಾಙ್ಕ len = strlen(pager);

		ಯದಿ (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		ಯದಿ (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&path_list, "-I");

		ಯದಿ (!strcmp("less", pager) || !strcmp("vi", pager)) {
			ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.pattern_list->pattern);
			string_list_append_nodup(&path_list,
						 strbuf_detach(&buf, ರಿಕ್ತ));
		}
	}

	ಯದಿ (!show_in_pager && !opt.status_only)
		setup_pager(the_repository);

	die_for_incompatible_opt3(!use_index, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	ಯದಿ (!use_index || untracked) {
		ಪೂರ್ಣಾಙ್ಕ use_exclude = (opt_exclude < 0) ? use_index : !!opt_exclude;
		hit = grep_directory(&opt, &pathspec, use_exclude, use_index);
	} ಅನ್ಯಥಾ ಯದಿ (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} ಅನ್ಯಥಾ ಯದಿ (!list.nr) {
		ಯದಿ (!cached)
			setup_work_ವೃಕ್ಷ(the_repository);

		hit = grep_cache(&opt, &pathspec, cached);
	} ಅನ್ಯಥಾ {
		ಯದಿ (cached)
			die(_("both --cached and trees are given"));

		hit = grep_objects(&opt, &pathspec, &list);
	}

	ಯದಿ (ಅಂಕೆ_threads > 1)
		hit |= wait_all();
	ಯದಿ (hit && show_in_pager)
		run_pager(&opt, prefix);

	ret = !hit;

out:
	clear_pathspec(&pathspec);
	string_list_clear(&path_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	ಪ್ರತ್ಯಾಗಮ ret;
}
