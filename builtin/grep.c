/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "tree-walk.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "grep.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "pathspec.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "submodule-config.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "odb/source.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "oidset.h"
#അന്തര്ഭാവ "pager.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "promisor-remote.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "write-or-die.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം *grep_prefix;

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ recurse_submodules;

സ്ഥിതം പൂർണ്ണസംഖ്യ അക്കം_threads;

സ്ഥിതം pthread_t *threads;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
ഘടന work_item {
	ഘടന grep_source source;
	അക്ഷരം done;
	ഘടന strbuf out;
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
സ്ഥിതം ഘടന work_item todo[TODO_SIZE];
സ്ഥിതം പൂർണ്ണസംഖ്യ todo_start;
സ്ഥിതം പൂർണ്ണസംഖ്യ todo_end;
സ്ഥിതം പൂർണ്ണസംഖ്യ todo_done;

/* Has all work items been added? */
സ്ഥിതം പൂർണ്ണസംഖ്യ all_work_added;

സ്ഥിതം ഘടന repository **repos_to_free;
സ്ഥിതം size_t repos_to_free_nr, repos_to_free_alloc;

/* This lock protects all the variables above. */
സ്ഥിതം pthread_mutex_t grep_mutex;

സ്ഥിതം വരിയിൽ ശൂന്യം grep_lock(ശൂന്യം)
{
	pthread_mutex_lock(&grep_mutex);
}

സ്ഥിതം വരിയിൽ ശൂന്യം grep_unlock(ശൂന്യം)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
സ്ഥിതം pthread_cond_t cond_add;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
സ്ഥിതം pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
സ്ഥിതം pthread_cond_t cond_result;

സ്ഥിതം പൂർണ്ണസംഖ്യ skip_first_വരി;

സ്ഥിതം ശൂന്യം add_work(ഘടന grep_opt *opt, ഘടന grep_source *gs)
{
	എങ്കിൽ (opt->ദ്വിമാന != GREP_ദ്വിമാന_TEXT)
		grep_source_load_driver(gs, opt->repo->index);

	grep_lock();

	ആകുമ്പോൾ ((todo_end+1) % ARRAY_SIZE(todo) == todo_done) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_end].source = *gs;
	todo[todo_end].done = 0;
	strbuf_reset(&todo[todo_end].out);
	todo_end = (todo_end + 1) % ARRAY_SIZE(todo);

	pthread_cond_signal(&cond_add);
	grep_unlock();
}

സ്ഥിതം ഘടന work_item *get_work(ശൂന്യം)
{
	ഘടന work_item *ret;

	grep_lock();
	ആകുമ്പോൾ (todo_start == todo_end && !all_work_added) {
		pthread_cond_wait(&cond_add, &grep_mutex);
	}

	എങ്കിൽ (todo_start == todo_end && all_work_added) {
		ret = ശൂന്യ;
	} അല്ലെങ്കിൽ {
		ret = &todo[todo_start];
		todo_start = (todo_start + 1) % ARRAY_SIZE(todo);
	}
	grep_unlock();
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം work_done(ഘടന work_item *w)
{
	പൂർണ്ണസംഖ്യ old_done;

	grep_lock();
	w->done = 1;
	old_done = todo_done;
	വേണ്ടി(; todo[todo_done].done && todo_done != todo_start;
	    todo_done = (todo_done+1) % ARRAY_SIZE(todo)) {
		w = &todo[todo_done];
		എങ്കിൽ (w->out.len) {
			സ്ഥിരം അക്ഷരം *p = w->out.buf;
			size_t len = w->out.len;

			/* Skip the leading hunk mark of the first file. */
			എങ്കിൽ (skip_first_വരി) {
				ആകുമ്പോൾ (len) {
					len--;
					എങ്കിൽ (*p++ == '\n')
						നിർത്തുക;
				}
				skip_first_വരി = 0;
			}

			write_or_die(1, p, len);
		}
		grep_source_clear(&w->source);
	}

	എങ്കിൽ (old_done != todo_done)
		pthread_cond_signal(&cond_write);

	എങ്കിൽ (all_work_added && todo_done == todo_end)
		pthread_cond_signal(&cond_result);

	grep_unlock();
}

സ്ഥിതം ശൂന്യം free_repos(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < repos_to_free_nr; i++) {
		repo_clear(repos_to_free[i]);
		സ്വതന്ത്രം(repos_to_free[i]);
	}
	FREE_AND_NULL(repos_to_free);
	repos_to_free_nr = 0;
	repos_to_free_alloc = 0;
}

സ്ഥിതം ശൂന്യം *run(ശൂന്യം *arg)
{
	പൂർണ്ണസംഖ്യ hit = 0;
	ഘടന grep_opt *opt = arg;

	ആകുമ്പോൾ (1) {
		ഘടന work_item *w = get_work();
		എങ്കിൽ (!w)
			നിർത്തുക;

		opt->output_priv = w;
		hit |= grep_source(opt, &w->source);
		grep_source_clear_data(&w->source);
		work_done(w);
	}
	free_grep_patterns(opt);
	സ്വതന്ത്രം(opt);

	തിരികെ (ശൂന്യം*) (intptr_t) hit;
}

സ്ഥിതം ശൂന്യം strbuf_out(ഘടന grep_opt *opt, സ്ഥിരം ശൂന്യം *buf, size_t size)
{
	ഘടന work_item *w = opt->output_priv;
	strbuf_add(&w->out, buf, size);
}

സ്ഥിതം ശൂന്യം start_threads(ഘടന grep_opt *opt)
{
	പൂർണ്ണസംഖ്യ i;

	pthread_mutex_init(&grep_mutex, ശൂന്യ);
	pthread_mutex_init(&grep_attr_mutex, ശൂന്യ);
	pthread_cond_init(&cond_add, ശൂന്യ);
	pthread_cond_init(&cond_write, ശൂന്യ);
	pthread_cond_init(&cond_result, ശൂന്യ);
	grep_use_locks = 1;
	enable_obj_read_lock();

	വേണ്ടി (i = 0; i < ARRAY_SIZE(todo); i++) {
		strbuf_init(&todo[i].out, 0);
	}

	CALLOC_ARRAY(threads, അക്കം_threads);
	വേണ്ടി (i = 0; i < അക്കം_threads; i++) {
		പൂർണ്ണസംഖ്യ err;
		ഘടന grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_out;
		compile_grep_patterns(o);
		err = pthread_create(&threads[i], ശൂന്യ, run, o);

		എങ്കിൽ (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ wait_all(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ hit = 0;
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (!HAVE_THREADS)
		BUG("Never call this function unless you have started threads");

	grep_lock();
	all_work_added = 1;

	/* Wait until all work is done. */
	ആകുമ്പോൾ (todo_done != todo_end)
		pthread_cond_wait(&cond_result, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_broadcast(&cond_add);
	grep_unlock();

	വേണ്ടി (i = 0; i < അക്കം_threads; i++) {
		ശൂന്യം *h;
		pthread_join(threads[i], &h);
		hit |= (പൂർണ്ണസംഖ്യ) (intptr_t) h;
	}

	സ്വതന്ത്രം(threads);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_add);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_result);
	grep_use_locks = 0;
	disable_obj_read_lock();

	തിരികെ hit;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ grep_cmd_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			   സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	പൂർണ്ണസംഖ്യ st = grep_config(var, മൂല്യം, ctx, cb);

	എങ്കിൽ (git_color_config(var, മൂല്യം, cb) < 0)
		st = -1;
	അല്ലെങ്കിൽ എങ്കിൽ (git_default_config(var, മൂല്യം, ctx, cb) < 0)
		st = -1;

	എങ്കിൽ (!strcmp(var, "grep.threads")) {
		അക്കം_threads = git_config_int(var, മൂല്യം, ctx->kvi);
		എങ്കിൽ (അക്കം_threads < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    അക്കം_threads, var);
		അല്ലെങ്കിൽ എങ്കിൽ (!HAVE_THREADS && അക്കം_threads > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			അക്കം_threads = 1;
		}
	}

	എങ്കിൽ (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, മൂല്യം);

	തിരികെ st;
}

സ്ഥിതം ശൂന്യം grep_source_name(ഘടന grep_opt *opt, സ്ഥിരം അക്ഷരം *filename,
			     പൂർണ്ണസംഖ്യ വൃക്ഷം_name_len, ഘടന strbuf *out)
{
	strbuf_reset(out);

	എങ്കിൽ (opt->null_following_name) {
		എങ്കിൽ (opt->relative && grep_prefix) {
			ഘടന strbuf rel_buf = STRBUF_INIT;
			സ്ഥിരം അക്ഷരം *rel_name =
				relative_path(filename + വൃക്ഷം_name_len,
					      grep_prefix, &rel_buf);

			എങ്കിൽ (വൃക്ഷം_name_len)
				strbuf_add(out, filename, വൃക്ഷം_name_len);

			strbuf_addstr(out, rel_name);
			strbuf_release(&rel_buf);
		} അല്ലെങ്കിൽ {
			strbuf_addstr(out, filename);
		}
		തിരികെ;
	}

	എങ്കിൽ (opt->relative && grep_prefix)
		quote_path(filename + വൃക്ഷം_name_len, grep_prefix, out, 0);
	അല്ലെങ്കിൽ
		quote_c_style(filename + വൃക്ഷം_name_len, out, ശൂന്യ, 0);

	എങ്കിൽ (വൃക്ഷം_name_len)
		strbuf_തിരുകുക(out, 0, filename, വൃക്ഷം_name_len);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ grep_oid(ഘടന grep_opt *opt, സ്ഥിരം ഘടന object_id *oid,
		     സ്ഥിരം അക്ഷരം *filename, പൂർണ്ണസംഖ്യ വൃക്ഷം_name_len,
		     സ്ഥിരം അക്ഷരം *path)
{
	ഘടന strbuf pathbuf = STRBUF_INIT;
	ഘടന grep_source gs;

	grep_source_name(opt, filename, വൃക്ഷം_name_len, &pathbuf);
	grep_source_init_oid(&gs, pathbuf.buf, path, oid, opt->repo);
	strbuf_release(&pathbuf);

	എങ്കിൽ (അക്കം_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		തിരികെ 0;
	} അല്ലെങ്കിൽ {
		പൂർണ്ണസംഖ്യ hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		തിരികെ hit;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ grep_file(ഘടന grep_opt *opt, സ്ഥിരം അക്ഷരം *filename)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന grep_source gs;

	grep_source_name(opt, filename, 0, &buf);
	grep_source_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	എങ്കിൽ (അക്കം_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		തിരികെ 0;
	} അല്ലെങ്കിൽ {
		പൂർണ്ണസംഖ്യ hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		തിരികെ hit;
	}
}

സ്ഥിതം ശൂന്യം append_path(ഘടന grep_opt *opt, സ്ഥിരം ശൂന്യം *data, size_t len)
{
	ഘടന string_list *path_list = opt->output_priv;

	എങ്കിൽ (len == 1 && *(സ്ഥിരം അക്ഷരം *)data == '\0')
		തിരികെ;
	string_list_append_nodup(path_list, xstrndup(data, len));
}

സ്ഥിതം ശൂന്യം run_pager(ഘടന grep_opt *opt, സ്ഥിരം അക്ഷരം *prefix)
{
	ഘടന string_list *path_list = opt->output_priv;
	ഘടന child_process child = CHILD_PROCESS_INIT;
	പൂർണ്ണസംഖ്യ i, status;

	വേണ്ടി (i = 0; i < path_list->nr; i++)
		strvec_തള്ളുക(&child.args, path_list->ഇനങ്ങൾ[i].string);
	child.dir = prefix;
	child.use_shell = 1;

	status = run_command(&child);
	എങ്കിൽ (status)
		exit(status);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ grep_cache(ഘടന grep_opt *opt,
		      സ്ഥിരം ഘടന pathspec *pathspec, പൂർണ്ണസംഖ്യ cached);
സ്ഥിതം പൂർണ്ണസംഖ്യ grep_വൃക്ഷം(ഘടന grep_opt *opt, സ്ഥിരം ഘടന pathspec *pathspec,
		     ഘടന വൃക്ഷം_desc *വൃക്ഷം, ഘടന strbuf *base, പൂർണ്ണസംഖ്യ tn_len,
		     പൂർണ്ണസംഖ്യ check_attr);

സ്ഥിതം പൂർണ്ണസംഖ്യ grep_submodule(ഘടന grep_opt *opt,
			  സ്ഥിരം ഘടന pathspec *pathspec,
			  സ്ഥിരം ഘടന object_id *oid,
			  സ്ഥിരം അക്ഷരം *filename, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ cached)
{
	ഘടന repository *subrepo;
	ഘടന repository *superproject = opt->repo;
	ഘടന grep_opt subopt;
	പൂർണ്ണസംഖ്യ hit = 0;

	എങ്കിൽ (!is_submodule_active(superproject, path))
		തിരികെ 0;

	subrepo = xmalloc(വലുപ്പം(*subrepo));
	എങ്കിൽ (repo_submodule_init(subrepo, superproject, path, null_oid(opt->repo->hash_algo))) {
		സ്വതന്ത്രം(subrepo);
		തിരികെ 0;
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

	memcpy(&subopt, opt, വലുപ്പം(subopt));
	subopt.repo = subrepo;

	എങ്കിൽ (oid) {
		എണ്ണൽ object_type object_type;
		ഘടന വൃക്ഷം_desc വൃക്ഷം;
		ശൂന്യം *data;
		size_t size;
		ഘടന strbuf base = STRBUF_INIT;

		obj_read_lock();
		object_type = odb_read_object_info(subrepo->objects, oid, ശൂന്യ);
		obj_read_unlock();
		data = odb_read_object_peeled(subrepo->objects, oid, OBJ_വൃക്ഷം, &size, ശൂന്യ);
		എങ്കിൽ (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(oid));

		strbuf_addstr(&base, filename);
		strbuf_addch(&base, '/');

		init_വൃക്ഷം_desc(&വൃക്ഷം, oid, data, size);
		hit = grep_വൃക്ഷം(&subopt, pathspec, &വൃക്ഷം, &base, base.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&base);
		സ്വതന്ത്രം(data);
	} അല്ലെങ്കിൽ {
		hit = grep_cache(&subopt, pathspec, cached);
	}

	തിരികെ hit;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ grep_cache(ഘടന grep_opt *opt,
		      സ്ഥിരം ഘടന pathspec *pathspec, പൂർണ്ണസംഖ്യ cached)
{
	ഘടന repository *repo = opt->repo;
	പൂർണ്ണസംഖ്യ hit = 0;
	പൂർണ്ണസംഖ്യ nr;
	ഘടന strbuf name = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ name_base_len = 0;
	എങ്കിൽ (repo->submodule_prefix) {
		name_base_len = strlen(repo->submodule_prefix);
		strbuf_addstr(&name, repo->submodule_prefix);
	}

	എങ്കിൽ (repo_read_index(repo) < 0)
		die(_("index file corrupt"));

	വേണ്ടി (nr = 0; nr < repo->index->cache_nr; nr++) {
		സ്ഥിരം ഘടന cache_entry *ce = repo->index->cache[nr];

		എങ്കിൽ (!cached && ce_skip_worktree(ce))
			തുടരുക;

		strbuf_setlen(&name, name_base_len);
		strbuf_addstr(&name, ce->name);
		എങ്കിൽ (S_ISSPARSEDIR(ce->ce_mode)) {
			എണ്ണൽ object_type type;
			ഘടന വൃക്ഷം_desc വൃക്ഷം;
			ശൂന്യം *data;
			size_t size;

			data = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &size);
			എങ്കിൽ (!data)
				die(_("unable to read tree %s"), oid_to_hex(&ce->oid));
			init_വൃക്ഷം_desc(&വൃക്ഷം, &ce->oid, data, size);

			hit |= grep_വൃക്ഷം(opt, pathspec, &വൃക്ഷം, &name, 0, 0);
			strbuf_setlen(&name, name_base_len);
			strbuf_addstr(&name, ce->name);
			സ്വതന്ത്രം(data);
		} അല്ലെങ്കിൽ എങ്കിൽ (S_ISREG(ce->ce_mode) &&
		    match_pathspec(repo->index, pathspec, name.buf, name.len, 0, ശൂന്യ,
				   S_ISDIR(ce->ce_mode) ||
				   S_ISGITLINK(ce->ce_mode))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			എങ്കിൽ (cached || (ce->ce_flags & CE_VALID)) {
				എങ്കിൽ (ce_stage(ce) || ce_intent_to_add(ce))
					തുടരുക;
				hit |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} അല്ലെങ്കിൽ {
				hit |= grep_file(opt, name.buf);
			}
		} അല്ലെങ്കിൽ എങ്കിൽ (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
			   submodule_path_match(repo->index, pathspec, name.buf, ശൂന്യ)) {
			hit |= grep_submodule(opt, pathspec, ശൂന്യ, ce->name,
					      ce->name, cached);
		} അല്ലെങ്കിൽ {
			തുടരുക;
		}

		എങ്കിൽ (ce_stage(ce)) {
			ചെയ്യുക {
				nr++;
			} ആകുമ്പോൾ (nr < repo->index->cache_nr &&
				 !strcmp(ce->name, repo->index->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		എങ്കിൽ (hit && opt->status_only)
			നിർത്തുക;
	}

	strbuf_release(&name);
	തിരികെ hit;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ grep_വൃക്ഷം(ഘടന grep_opt *opt, സ്ഥിരം ഘടന pathspec *pathspec,
		     ഘടന വൃക്ഷം_desc *വൃക്ഷം, ഘടന strbuf *base, പൂർണ്ണസംഖ്യ tn_len,
		     പൂർണ്ണസംഖ്യ check_attr)
{
	ഘടന repository *repo = opt->repo;
	പൂർണ്ണസംഖ്യ hit = 0;
	എണ്ണൽ interesting match = entry_not_interesting;
	ഘടന name_entry entry;
	പൂർണ്ണസംഖ്യ old_baselen = base->len;
	ഘടന strbuf name = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ name_base_len = 0;
	എങ്കിൽ (repo->submodule_prefix) {
		strbuf_addstr(&name, repo->submodule_prefix);
		name_base_len = name.len;
	}

	ആകുമ്പോൾ (വൃക്ഷം_entry(വൃക്ഷം, &entry)) {
		പൂർണ്ണസംഖ്യ te_len = വൃക്ഷം_entry_len(&entry);

		എങ്കിൽ (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = വൃക്ഷം_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_setlen(&name, name_base_len);

			എങ്കിൽ (match == all_entries_not_interesting)
				നിർത്തുക;
			എങ്കിൽ (match == entry_not_interesting)
				തുടരുക;
		}

		strbuf_add(base, entry.path, te_len);

		എങ്കിൽ (S_ISREG(entry.mode)) {
			hit |= grep_oid(opt, &entry.oid, base->buf, tn_len,
					 check_attr ? base->buf + tn_len : ശൂന്യ);
		} അല്ലെങ്കിൽ എങ്കിൽ (S_ISDIR(entry.mode)) {
			എണ്ണൽ object_type type;
			ഘടന വൃക്ഷം_desc sub;
			ശൂന്യം *data;
			size_t size;

			data = odb_read_object(the_repository->objects,
					       &entry.oid, &type, &size);
			എങ്കിൽ (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_വൃക്ഷം_desc(&sub, &entry.oid, data, size);
			hit |= grep_വൃക്ഷം(opt, pathspec, &sub, base, tn_len,
					 check_attr);
			സ്വതന്ത്രം(data);
		} അല്ലെങ്കിൽ എങ്കിൽ (recurse_submodules && S_ISGITLINK(entry.mode)) {
			hit |= grep_submodule(opt, pathspec, &entry.oid,
					      base->buf, base->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(base, old_baselen);

		എങ്കിൽ (hit && opt->status_only)
			നിർത്തുക;
	}

	strbuf_release(&name);
	തിരികെ hit;
}

സ്ഥിതം ശൂന്യം collect_blob_oids_for_വൃക്ഷം(ഘടന repository *repo,
				       സ്ഥിരം ഘടന pathspec *pathspec,
				       ഘടന വൃക്ഷം_desc *വൃക്ഷം,
				       ഘടന strbuf *base,
				       പൂർണ്ണസംഖ്യ tn_len,
				       ഘടന oidset *blob_oids)
{
	ഘടന name_entry entry;
	പൂർണ്ണസംഖ്യ old_baselen = base->len;
	ഘടന strbuf name = STRBUF_INIT;
	എണ്ണൽ interesting match = entry_not_interesting;

	ആകുമ്പോൾ (വൃക്ഷം_entry(വൃക്ഷം, &entry)) {
		എങ്കിൽ (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = വൃക്ഷം_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_reset(&name);

			എങ്കിൽ (match == all_entries_not_interesting)
				നിർത്തുക;
			എങ്കിൽ (match == entry_not_interesting)
				തുടരുക;
		}

		strbuf_add(base, entry.path, വൃക്ഷം_entry_len(&entry));

		എങ്കിൽ (S_ISREG(entry.mode)) {
			എങ്കിൽ (!odb_has_object(repo->objects, &entry.oid, 0))
				oidset_തിരുകുക(blob_oids, &entry.oid);
		} അല്ലെങ്കിൽ എങ്കിൽ (S_ISDIR(entry.mode)) {
			എണ്ണൽ object_type type;
			ഘടന വൃക്ഷം_desc sub_വൃക്ഷം;
			ശൂന്യം *data;
			size_t size;

			data = odb_read_object(repo->objects, &entry.oid,
					       &type, &size);
			എങ്കിൽ (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_വൃക്ഷം_desc(&sub_വൃക്ഷം, &entry.oid, data, size);
			collect_blob_oids_for_വൃക്ഷം(repo, pathspec, &sub_വൃക്ഷം,
						   base, tn_len, blob_oids);
			സ്വതന്ത്രം(data);
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

സ്ഥിതം ശൂന്യം collect_blob_oids_for_treeish(ഘടന grep_opt *opt,
					  സ്ഥിരം ഘടന pathspec *pathspec,
					  സ്ഥിരം ഘടന object_id *വൃക്ഷം_ish_oid,
					  സ്ഥിരം അക്ഷരം *name,
					  ഘടന oidset *blob_oids)
{
	ഘടന വൃക്ഷം_desc വൃക്ഷം;
	ശൂന്യം *data;
	size_t size;
	ഘടന strbuf base = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ len;

	data = odb_read_object_peeled(opt->repo->objects, വൃക്ഷം_ish_oid,
				      OBJ_വൃക്ഷം, &size, ശൂന്യ);

	എങ്കിൽ (!data)
		തിരികെ;

	len = name ? strlen(name) : 0;
	എങ്കിൽ (len) {
		strbuf_add(&base, name, len);
		strbuf_addch(&base, ':');
	}
	init_വൃക്ഷം_desc(&വൃക്ഷം, വൃക്ഷം_ish_oid, data, size);

	collect_blob_oids_for_വൃക്ഷം(opt->repo, pathspec, &വൃക്ഷം,
				   &base, base.len, blob_oids);

	strbuf_release(&base);
	സ്വതന്ത്രം(data);
}

സ്ഥിതം ശൂന്യം prefetch_grep_blobs(ഘടന grep_opt *opt,
				സ്ഥിരം ഘടന pathspec *pathspec,
				സ്ഥിരം ഘടന object_array *list)
{
	ഘടന oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	എങ്കിൽ (!repo_has_promisor_remote(opt->repo))
		തിരികെ;

	/* For each tree, gather the blobs in it */
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < list->nr; i++) {
		ഘടന object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     ശൂന്യ, 0);
		obj_read_unlock();

		എങ്കിൽ (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_വൃക്ഷം))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	എങ്കിൽ (oidset_size(&blob_oids)) {
		ഘടന oid_array to_fetch = OID_ARRAY_INIT;
		ഘടന oidset_iter iter;
		സ്ഥിരം ഘടന object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		ആകുമ്പോൾ ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(opt->repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}
	oidset_clear(&blob_oids);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ grep_object(ഘടന grep_opt *opt, സ്ഥിരം ഘടന pathspec *pathspec,
		       ഘടന object *obj, സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *path)
{
	എങ്കിൽ (obj->type == OBJ_BLOB)
		തിരികെ grep_oid(opt, &obj->oid, name, 0, path);
	എങ്കിൽ (obj->type == OBJ_COMMIT || obj->type == OBJ_വൃക്ഷം) {
		ഘടന വൃക്ഷം_desc വൃക്ഷം;
		ശൂന്യം *data;
		size_t size;
		ഘടന strbuf base;
		പൂർണ്ണസംഖ്യ hit, len;

		data = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_വൃക്ഷം, &size, ശൂന്യ);
		എങ്കിൽ (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(&obj->oid));

		len = name ? strlen(name) : 0;
		strbuf_init(&base, PATH_MAX + len + 1);
		എങ്കിൽ (len) {
			strbuf_add(&base, name, len);
			strbuf_addch(&base, ':');
		}
		init_വൃക്ഷം_desc(&വൃക്ഷം, &obj->oid, data, size);
		hit = grep_വൃക്ഷം(opt, pathspec, &വൃക്ഷം, &base, base.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&base);
		സ്വതന്ത്രം(data);
		തിരികെ hit;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

സ്ഥിതം പൂർണ്ണസംഖ്യ grep_objects(ഘടന grep_opt *opt, സ്ഥിരം ഘടന pathspec *pathspec,
			സ്ഥിരം ഘടന object_array *list)
{
	unsigned പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ hit = 0;
	സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	വേണ്ടി (i = 0; i < nr; i++) {
		ഘടന object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     ശൂന്യ, 0);
		obj_read_unlock();

		എങ്കിൽ (!real_obj) {
			അക്ഷരം hex[GIT_MAX_HEXSZ + 1];
			സ്ഥിരം അക്ഷരം *name = list->objects[i].name;

			എങ്കിൽ (!name) {
				oid_to_hex_r(hex, &list->objects[i].item->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		എങ്കിൽ (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_lock();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		എങ്കിൽ (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].path)) {
			hit = 1;
			എങ്കിൽ (opt->status_only)
				നിർത്തുക;
		}
	}
	തിരികെ hit;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ grep_directory(ഘടന grep_opt *opt, സ്ഥിരം ഘടന pathspec *pathspec,
			  പൂർണ്ണസംഖ്യ exc_std, പൂർണ്ണസംഖ്യ use_index)
{
	ഘടന dir_struct dir = DIR_INIT;
	പൂർണ്ണസംഖ്യ i, hit = 0;

	എങ്കിൽ (!use_index)
		dir.flags |= DIR_NO_GITLINKS;
	എങ്കിൽ (exc_std)
		setup_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->index, pathspec);
	വേണ്ടി (i = 0; i < dir.nr; i++) {
		hit |= grep_file(opt, dir.entries[i]->name);
		എങ്കിൽ (hit && opt->status_only)
			നിർത്തുക;
	}
	dir_clear(&dir);
	തിരികെ hit;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ context_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			    പൂർണ്ണസംഖ്യ unset)
{
	ഘടന grep_opt *grep_opt = opt->മൂല്യം;
	പൂർണ്ണസംഖ്യ മൂല്യം;
	സ്ഥിരം അക്ഷരം *endp;

	എങ്കിൽ (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		തിരികെ 0;
	}
	മൂല്യം = strtol(arg, (അക്ഷരം **)&endp, 10);
	എങ്കിൽ (*endp) {
		തിരികെ error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = മൂല്യം;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ file_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന grep_opt *grep_opt = opt->മൂല്യം;
	പൂർണ്ണസംഖ്യ from_stdin;
	സ്ഥിരം അക്ഷരം *filename = arg;
	FILE *patterns;
	പൂർണ്ണസംഖ്യ lno = 0;
	ഘടന strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (!*filename)
		; /* leave it as-is */
	അല്ലെങ്കിൽ
		filename = prefix_filename_except_for_dash(grep_prefix, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	എങ്കിൽ (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	ആകുമ്പോൾ (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		എങ്കിൽ (sb.len == 0)
			തുടരുക;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_PATTERN);
	}
	എങ്കിൽ (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	എങ്കിൽ (filename != arg)
		സ്വതന്ത്രം((ശൂന്യം *)filename);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ not_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന grep_opt *grep_opt = opt->മൂല്യം;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--not", "command line", 0, GREP_NOT);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ and_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന grep_opt *grep_opt = opt->മൂല്യം;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--and", "command line", 0, GREP_AND);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ open_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന grep_opt *grep_opt = opt->മൂല്യം;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ close_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന grep_opt *grep_opt = opt->മൂല്യം;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ pattern_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			    പൂർണ്ണസംഖ്യ unset)
{
	ഘടന grep_opt *grep_opt = opt->മൂല്യം;
	BUG_ON_OPT_NEG(unset);
	append_grep_pattern(grep_opt, arg, "-e option", 0, GREP_PATTERN);
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_grep(പൂർണ്ണസംഖ്യ argc,
	     സ്ഥിരം അക്ഷരം **argv,
	     സ്ഥിരം അക്ഷരം *prefix,
	     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ hit = 0;
	പൂർണ്ണസംഖ്യ cached = 0, untracked = 0, opt_exclude = -1;
	പൂർണ്ണസംഖ്യ seen_dashdash = 0;
	പൂർണ്ണസംഖ്യ external_grep_allowed__ignored;
	സ്ഥിരം അക്ഷരം *show_in_pager = ശൂന്യ, *default_pager = "dummy";
	ഘടന grep_opt opt;
	ഘടന object_array list = OBJECT_ARRAY_INIT;
	ഘടന pathspec pathspec;
	ഘടന string_list path_list = STRING_LIST_INIT_DUP;
	പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ dummy;
	പൂർണ്ണസംഖ്യ use_index = 1;
	പൂർണ്ണസംഖ്യ allow_revs;
	പൂർണ്ണസംഖ്യ ret;

	ഘടന option options[] = {
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
		OPT_SET_INT('a', "text", &opt.ദ്വിമാന,
			N_("process binary files as text"), GREP_ദ്വിമാന_TEXT),
		OPT_SET_INT('I', ശൂന്യ, &opt.ദ്വിമാന,
			N_("don't match patterns in binary files"),
			GREP_ദ്വിമാന_NOMATCH),
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
		OPT_NEGBIT('h', ശൂന്യ, &opt.pathname, N_("don't show filenames"), 1),
		OPT_BIT('H', ശൂന്യ, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.എണ്ണം,
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
		OPT_INTEGER(0, "threads", &അക്കം_threads,
			N_("use <n> worker threads")),
		OPT_സംഖ്യ_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_GROUP(""),
		OPT_CALLBACK('f', ശൂന്യ, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', ശൂന്യ, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, pattern_callback),
		OPT_CALLBACK_F(0, "and", &opt, ശൂന്യ,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, ശൂന്യ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', ശൂന്യ, &opt, ശൂന്യ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', ശൂന്യ, &opt, ശൂന്യ, "",
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
			.മൂല്യം = &show_in_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_എണ്ണം,
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

	എങ്കിൽ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	എങ്കിൽ (use_index && !startup_info->have_repository) {
		പൂർണ്ണസംഖ്യ fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		എങ്കിൽ (fallback)
			use_index = 0;
		അല്ലെങ്കിൽ
			/* die the same way as if we did it at the beginning */
			setup_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	എങ്കിൽ (!use_index)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	എങ്കിൽ (argc > 0 && !opt.pattern_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	എങ്കിൽ (argc > 0 && !opt.pattern_list) {
		append_grep_pattern(&opt, argv[0], "command line", 0,
				    GREP_PATTERN);
		argv++;
		argc--;
	}

	എങ്കിൽ (show_in_pager == default_pager)
		show_in_pager = git_pager(the_repository, 1);
	എങ്കിൽ (show_in_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &path_list;
		opt.output = append_path;
		string_list_append(&path_list, show_in_pager);
	}

	എങ്കിൽ (!opt.pattern_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	എങ്കിൽ (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	വേണ്ടി (i = 0; i < argc; i++) {
		എങ്കിൽ (!strcmp(argv[i], "--")) {
			seen_dashdash = 1;
			നിർത്തുക;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_index && !untracked;
	വേണ്ടി (i = 0; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];
		ഘടന object_id oid;
		ഘടന object_context oc = {0};
		ഘടന object *object;

		എങ്കിൽ (!strcmp(arg, "--")) {
			i++;
			നിർത്തുക;
		}

		എങ്കിൽ (!allow_revs) {
			എങ്കിൽ (seen_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			നിർത്തുക;
		}

		എങ്കിൽ (get_oid_with_context(the_repository, arg,
					 GET_OID_RECORD_PATH,
					 &oid, &oc)) {
			എങ്കിൽ (seen_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			നിർത്തുക;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		എങ്കിൽ (!seen_dashdash)
			verify_non_filename(the_repository, prefix, arg);
		add_object_array_with_path(object, arg, &list, oc.mode, oc.path);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	എങ്കിൽ (!seen_dashdash) {
		പൂർണ്ണസംഖ്യ j;
		വേണ്ടി (j = i; j < argc; j++)
			verify_filename(the_repository, prefix, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_depth != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       prefix, argv + i);
	pathspec.max_depth = opt.max_depth;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	എങ്കിൽ (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	എങ്കിൽ (opt.max_എണ്ണം == 0) {
		ret = 1;
		പോകുക out;
	}

	എങ്കിൽ (show_in_pager) {
		എങ്കിൽ (അക്കം_threads > 1)
			warning(_("invalid option combination, ignoring --threads"));
		അക്കം_threads = 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (!HAVE_THREADS && അക്കം_threads > 1) {
		warning(_("no threads support, ignoring --threads"));
		അക്കം_threads = 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (അക്കം_threads < 0)
		die(_("invalid number of threads specified (%d)"), അക്കം_threads);
	അല്ലെങ്കിൽ എങ്കിൽ (അക്കം_threads == 0)
		അക്കം_threads = HAVE_THREADS ? online_cpus() : 1;

	എങ്കിൽ (അക്കം_threads > 1) {
		എങ്കിൽ (!HAVE_THREADS)
			BUG("Somebody got num_threads calculation wrong!");
		എങ്കിൽ (!(opt.name_only || opt.unmatch_name_only || opt.എണ്ണം)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_first_വരി = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		എങ്കിൽ (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		എങ്കിൽ (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		start_threads(&opt);
	} അല്ലെങ്കിൽ {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	എങ്കിൽ (show_in_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	എങ്കിൽ (show_in_pager && opt.pattern_list && !opt.pattern_list->next) {
		സ്ഥിരം അക്ഷരം *pager = path_list.ഇനങ്ങൾ[0].string;
		പൂർണ്ണസംഖ്യ len = strlen(pager);

		എങ്കിൽ (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		എങ്കിൽ (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&path_list, "-I");

		എങ്കിൽ (!strcmp("less", pager) || !strcmp("vi", pager)) {
			ഘടന strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.pattern_list->pattern);
			string_list_append_nodup(&path_list,
						 strbuf_detach(&buf, ശൂന്യ));
		}
	}

	എങ്കിൽ (!show_in_pager && !opt.status_only)
		setup_pager(the_repository);

	die_for_incompatible_opt3(!use_index, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	എങ്കിൽ (!use_index || untracked) {
		പൂർണ്ണസംഖ്യ use_exclude = (opt_exclude < 0) ? use_index : !!opt_exclude;
		hit = grep_directory(&opt, &pathspec, use_exclude, use_index);
	} അല്ലെങ്കിൽ എങ്കിൽ (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} അല്ലെങ്കിൽ എങ്കിൽ (!list.nr) {
		എങ്കിൽ (!cached)
			setup_work_വൃക്ഷം(the_repository);

		hit = grep_cache(&opt, &pathspec, cached);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (cached)
			die(_("both --cached and trees are given"));

		hit = grep_objects(&opt, &pathspec, &list);
	}

	എങ്കിൽ (അക്കം_threads > 1)
		hit |= wait_all();
	എങ്കിൽ (hit && show_in_pager)
		run_pager(&opt, prefix);

	ret = !hit;

out:
	clear_pathspec(&pathspec);
	string_list_clear(&path_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	തിരികെ ret;
}
