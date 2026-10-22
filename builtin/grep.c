/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "grep.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "pathspec.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "submodule-config.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/source.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "oidset.h"
#अन्तर्भाव "pager.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "promisor-remote.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "write-or-die.h"

स्थिर ध्रुव वर्ण *grep_prefix;

स्थिर वर्ण ध्रुव * ध्रुव grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	रिक्त
};

स्थिर पूर्णाङ्क recurse_submodules;

स्थिर पूर्णाङ्क अंक_threads;

स्थिर pthread_t *threads;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
संरचना work_item {
	संरचना grep_source source;
	वर्ण done;
	संरचना strbuf out;
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
स्थिर संरचना work_item todo[TODO_SIZE];
स्थिर पूर्णाङ्क todo_start;
स्थिर पूर्णाङ्क todo_end;
स्थिर पूर्णाङ्क todo_done;

/* Has all work items been added? */
स्थिर पूर्णाङ्क all_work_added;

स्थिर संरचना repository **repos_to_free;
स्थिर size_t repos_to_free_nr, repos_to_free_alloc;

/* This lock protects all the variables above. */
स्थिर pthread_mutex_t grep_mutex;

स्थिर अन्तःस्थ शून्य grep_lock(शून्य)
{
	pthread_mutex_lock(&grep_mutex);
}

स्थिर अन्तःस्थ शून्य grep_unlock(शून्य)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
स्थिर pthread_cond_t cond_add;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
स्थिर pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
स्थिर pthread_cond_t cond_result;

स्थिर पूर्णाङ्क skip_first_पंक्ति;

स्थिर शून्य add_work(संरचना grep_opt *opt, संरचना grep_source *gs)
{
	यदि (opt->द्विआधारी != GREP_द्विआधारी_TEXT)
		grep_source_load_driver(gs, opt->repo->index);

	grep_lock();

	यावत् ((todo_end+1) % ARRAY_SIZE(todo) == todo_done) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_end].source = *gs;
	todo[todo_end].done = 0;
	strbuf_reset(&todo[todo_end].out);
	todo_end = (todo_end + 1) % ARRAY_SIZE(todo);

	pthread_cond_signal(&cond_add);
	grep_unlock();
}

स्थिर संरचना work_item *get_work(शून्य)
{
	संरचना work_item *ret;

	grep_lock();
	यावत् (todo_start == todo_end && !all_work_added) {
		pthread_cond_wait(&cond_add, &grep_mutex);
	}

	यदि (todo_start == todo_end && all_work_added) {
		ret = रिक्त;
	} अन्यथा {
		ret = &todo[todo_start];
		todo_start = (todo_start + 1) % ARRAY_SIZE(todo);
	}
	grep_unlock();
	प्रत्यागम ret;
}

स्थिर शून्य work_done(संरचना work_item *w)
{
	पूर्णाङ्क old_done;

	grep_lock();
	w->done = 1;
	old_done = todo_done;
	प्रति(; todo[todo_done].done && todo_done != todo_start;
	    todo_done = (todo_done+1) % ARRAY_SIZE(todo)) {
		w = &todo[todo_done];
		यदि (w->out.len) {
			ध्रुव वर्ण *p = w->out.buf;
			size_t len = w->out.len;

			/* Skip the leading hunk mark of the first file. */
			यदि (skip_first_पंक्ति) {
				यावत् (len) {
					len--;
					यदि (*p++ == '\n')
						भङ्ग;
				}
				skip_first_पंक्ति = 0;
			}

			write_or_die(1, p, len);
		}
		grep_source_clear(&w->source);
	}

	यदि (old_done != todo_done)
		pthread_cond_signal(&cond_write);

	यदि (all_work_added && todo_done == todo_end)
		pthread_cond_signal(&cond_result);

	grep_unlock();
}

स्थिर शून्य free_repos(शून्य)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < repos_to_free_nr; i++) {
		repo_clear(repos_to_free[i]);
		मुक्त(repos_to_free[i]);
	}
	FREE_AND_NULL(repos_to_free);
	repos_to_free_nr = 0;
	repos_to_free_alloc = 0;
}

स्थिर शून्य *run(शून्य *arg)
{
	पूर्णाङ्क hit = 0;
	संरचना grep_opt *opt = arg;

	यावत् (1) {
		संरचना work_item *w = get_work();
		यदि (!w)
			भङ्ग;

		opt->output_priv = w;
		hit |= grep_source(opt, &w->source);
		grep_source_clear_data(&w->source);
		work_done(w);
	}
	free_grep_patterns(opt);
	मुक्त(opt);

	प्रत्यागम (शून्य*) (intptr_t) hit;
}

स्थिर शून्य strbuf_out(संरचना grep_opt *opt, ध्रुव शून्य *buf, size_t size)
{
	संरचना work_item *w = opt->output_priv;
	strbuf_add(&w->out, buf, size);
}

स्थिर शून्य start_threads(संरचना grep_opt *opt)
{
	पूर्णाङ्क i;

	pthread_mutex_init(&grep_mutex, रिक्त);
	pthread_mutex_init(&grep_attr_mutex, रिक्त);
	pthread_cond_init(&cond_add, रिक्त);
	pthread_cond_init(&cond_write, रिक्त);
	pthread_cond_init(&cond_result, रिक्त);
	grep_use_locks = 1;
	enable_obj_read_lock();

	प्रति (i = 0; i < ARRAY_SIZE(todo); i++) {
		strbuf_init(&todo[i].out, 0);
	}

	CALLOC_ARRAY(threads, अंक_threads);
	प्रति (i = 0; i < अंक_threads; i++) {
		पूर्णाङ्क err;
		संरचना grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_out;
		compile_grep_patterns(o);
		err = pthread_create(&threads[i], रिक्त, run, o);

		यदि (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

स्थिर पूर्णाङ्क wait_all(शून्य)
{
	पूर्णाङ्क hit = 0;
	पूर्णाङ्क i;

	यदि (!HAVE_THREADS)
		BUG("Never call this function unless you have started threads");

	grep_lock();
	all_work_added = 1;

	/* Wait until all work is done. */
	यावत् (todo_done != todo_end)
		pthread_cond_wait(&cond_result, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_broadcast(&cond_add);
	grep_unlock();

	प्रति (i = 0; i < अंक_threads; i++) {
		शून्य *h;
		pthread_join(threads[i], &h);
		hit |= (पूर्णाङ्क) (intptr_t) h;
	}

	मुक्त(threads);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_add);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_result);
	grep_use_locks = 0;
	disable_obj_read_lock();

	प्रत्यागम hit;
}

स्थिर पूर्णाङ्क grep_cmd_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मूल्य,
			   ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	पूर्णाङ्क st = grep_config(var, मूल्य, ctx, cb);

	यदि (git_color_config(var, मूल्य, cb) < 0)
		st = -1;
	अन्यथा यदि (git_default_config(var, मूल्य, ctx, cb) < 0)
		st = -1;

	यदि (!strcmp(var, "grep.threads")) {
		अंक_threads = git_config_int(var, मूल्य, ctx->kvi);
		यदि (अंक_threads < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    अंक_threads, var);
		अन्यथा यदि (!HAVE_THREADS && अंक_threads > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			अंक_threads = 1;
		}
	}

	यदि (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, मूल्य);

	प्रत्यागम st;
}

स्थिर शून्य grep_source_name(संरचना grep_opt *opt, ध्रुव वर्ण *filename,
			     पूर्णाङ्क वृक्ष_name_len, संरचना strbuf *out)
{
	strbuf_reset(out);

	यदि (opt->null_following_name) {
		यदि (opt->relative && grep_prefix) {
			संरचना strbuf rel_buf = STRBUF_INIT;
			ध्रुव वर्ण *rel_name =
				relative_path(filename + वृक्ष_name_len,
					      grep_prefix, &rel_buf);

			यदि (वृक्ष_name_len)
				strbuf_add(out, filename, वृक्ष_name_len);

			strbuf_addstr(out, rel_name);
			strbuf_release(&rel_buf);
		} अन्यथा {
			strbuf_addstr(out, filename);
		}
		प्रत्यागम;
	}

	यदि (opt->relative && grep_prefix)
		quote_path(filename + वृक्ष_name_len, grep_prefix, out, 0);
	अन्यथा
		quote_c_style(filename + वृक्ष_name_len, out, रिक्त, 0);

	यदि (वृक्ष_name_len)
		strbuf_जोड़ो(out, 0, filename, वृक्ष_name_len);
}

स्थिर पूर्णाङ्क grep_oid(संरचना grep_opt *opt, ध्रुव संरचना object_id *oid,
		     ध्रुव वर्ण *filename, पूर्णाङ्क वृक्ष_name_len,
		     ध्रुव वर्ण *path)
{
	संरचना strbuf pathbuf = STRBUF_INIT;
	संरचना grep_source gs;

	grep_source_name(opt, filename, वृक्ष_name_len, &pathbuf);
	grep_source_init_oid(&gs, pathbuf.buf, path, oid, opt->repo);
	strbuf_release(&pathbuf);

	यदि (अंक_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		प्रत्यागम 0;
	} अन्यथा {
		पूर्णाङ्क hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		प्रत्यागम hit;
	}
}

स्थिर पूर्णाङ्क grep_file(संरचना grep_opt *opt, ध्रुव वर्ण *filename)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना grep_source gs;

	grep_source_name(opt, filename, 0, &buf);
	grep_source_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	यदि (अंक_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		प्रत्यागम 0;
	} अन्यथा {
		पूर्णाङ्क hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		प्रत्यागम hit;
	}
}

स्थिर शून्य append_path(संरचना grep_opt *opt, ध्रुव शून्य *data, size_t len)
{
	संरचना string_list *path_list = opt->output_priv;

	यदि (len == 1 && *(ध्रुव वर्ण *)data == '\0')
		प्रत्यागम;
	string_list_append_nodup(path_list, xstrndup(data, len));
}

स्थिर शून्य run_pager(संरचना grep_opt *opt, ध्रुव वर्ण *prefix)
{
	संरचना string_list *path_list = opt->output_priv;
	संरचना child_process child = CHILD_PROCESS_INIT;
	पूर्णाङ्क i, status;

	प्रति (i = 0; i < path_list->nr; i++)
		strvec_धकेलो(&child.args, path_list->वस्तुएँ[i].string);
	child.dir = prefix;
	child.use_shell = 1;

	status = run_command(&child);
	यदि (status)
		exit(status);
}

स्थिर पूर्णाङ्क grep_cache(संरचना grep_opt *opt,
		      ध्रुव संरचना pathspec *pathspec, पूर्णाङ्क cached);
स्थिर पूर्णाङ्क grep_वृक्ष(संरचना grep_opt *opt, ध्रुव संरचना pathspec *pathspec,
		     संरचना वृक्ष_desc *वृक्ष, संरचना strbuf *base, पूर्णाङ्क tn_len,
		     पूर्णाङ्क check_attr);

स्थिर पूर्णाङ्क grep_submodule(संरचना grep_opt *opt,
			  ध्रुव संरचना pathspec *pathspec,
			  ध्रुव संरचना object_id *oid,
			  ध्रुव वर्ण *filename, ध्रुव वर्ण *path, पूर्णाङ्क cached)
{
	संरचना repository *subrepo;
	संरचना repository *superproject = opt->repo;
	संरचना grep_opt subopt;
	पूर्णाङ्क hit = 0;

	यदि (!is_submodule_active(superproject, path))
		प्रत्यागम 0;

	subrepo = xmalloc(परिमाण(*subrepo));
	यदि (repo_submodule_init(subrepo, superproject, path, null_oid(opt->repo->hash_algo))) {
		मुक्त(subrepo);
		प्रत्यागम 0;
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

	memcpy(&subopt, opt, परिमाण(subopt));
	subopt.repo = subrepo;

	यदि (oid) {
		गणना object_type object_type;
		संरचना वृक्ष_desc वृक्ष;
		शून्य *data;
		size_t size;
		संरचना strbuf base = STRBUF_INIT;

		obj_read_lock();
		object_type = odb_read_object_info(subrepo->objects, oid, रिक्त);
		obj_read_unlock();
		data = odb_read_object_peeled(subrepo->objects, oid, OBJ_वृक्ष, &size, रिक्त);
		यदि (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(oid));

		strbuf_addstr(&base, filename);
		strbuf_addch(&base, '/');

		init_वृक्ष_desc(&वृक्ष, oid, data, size);
		hit = grep_वृक्ष(&subopt, pathspec, &वृक्ष, &base, base.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&base);
		मुक्त(data);
	} अन्यथा {
		hit = grep_cache(&subopt, pathspec, cached);
	}

	प्रत्यागम hit;
}

स्थिर पूर्णाङ्क grep_cache(संरचना grep_opt *opt,
		      ध्रुव संरचना pathspec *pathspec, पूर्णाङ्क cached)
{
	संरचना repository *repo = opt->repo;
	पूर्णाङ्क hit = 0;
	पूर्णाङ्क nr;
	संरचना strbuf name = STRBUF_INIT;
	पूर्णाङ्क name_base_len = 0;
	यदि (repo->submodule_prefix) {
		name_base_len = strlen(repo->submodule_prefix);
		strbuf_addstr(&name, repo->submodule_prefix);
	}

	यदि (repo_read_index(repo) < 0)
		die(_("index file corrupt"));

	प्रति (nr = 0; nr < repo->index->cache_nr; nr++) {
		ध्रुव संरचना cache_entry *ce = repo->index->cache[nr];

		यदि (!cached && ce_skip_worktree(ce))
			अनुवर्त;

		strbuf_setlen(&name, name_base_len);
		strbuf_addstr(&name, ce->name);
		यदि (S_ISSPARSEDIR(ce->ce_mode)) {
			गणना object_type type;
			संरचना वृक्ष_desc वृक्ष;
			शून्य *data;
			size_t size;

			data = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &size);
			यदि (!data)
				die(_("unable to read tree %s"), oid_to_hex(&ce->oid));
			init_वृक्ष_desc(&वृक्ष, &ce->oid, data, size);

			hit |= grep_वृक्ष(opt, pathspec, &वृक्ष, &name, 0, 0);
			strbuf_setlen(&name, name_base_len);
			strbuf_addstr(&name, ce->name);
			मुक्त(data);
		} अन्यथा यदि (S_ISREG(ce->ce_mode) &&
		    match_pathspec(repo->index, pathspec, name.buf, name.len, 0, रिक्त,
				   S_ISDIR(ce->ce_mode) ||
				   S_ISGITLINK(ce->ce_mode))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			यदि (cached || (ce->ce_flags & CE_VALID)) {
				यदि (ce_stage(ce) || ce_intent_to_add(ce))
					अनुवर्त;
				hit |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} अन्यथा {
				hit |= grep_file(opt, name.buf);
			}
		} अन्यथा यदि (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
			   submodule_path_match(repo->index, pathspec, name.buf, रिक्त)) {
			hit |= grep_submodule(opt, pathspec, रिक्त, ce->name,
					      ce->name, cached);
		} अन्यथा {
			अनुवर्त;
		}

		यदि (ce_stage(ce)) {
			कुरु {
				nr++;
			} यावत् (nr < repo->index->cache_nr &&
				 !strcmp(ce->name, repo->index->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		यदि (hit && opt->status_only)
			भङ्ग;
	}

	strbuf_release(&name);
	प्रत्यागम hit;
}

स्थिर पूर्णाङ्क grep_वृक्ष(संरचना grep_opt *opt, ध्रुव संरचना pathspec *pathspec,
		     संरचना वृक्ष_desc *वृक्ष, संरचना strbuf *base, पूर्णाङ्क tn_len,
		     पूर्णाङ्क check_attr)
{
	संरचना repository *repo = opt->repo;
	पूर्णाङ्क hit = 0;
	गणना interesting match = entry_not_interesting;
	संरचना name_entry entry;
	पूर्णाङ्क old_baselen = base->len;
	संरचना strbuf name = STRBUF_INIT;
	पूर्णाङ्क name_base_len = 0;
	यदि (repo->submodule_prefix) {
		strbuf_addstr(&name, repo->submodule_prefix);
		name_base_len = name.len;
	}

	यावत् (वृक्ष_entry(वृक्ष, &entry)) {
		पूर्णाङ्क te_len = वृक्ष_entry_len(&entry);

		यदि (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = वृक्ष_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_setlen(&name, name_base_len);

			यदि (match == all_entries_not_interesting)
				भङ्ग;
			यदि (match == entry_not_interesting)
				अनुवर्त;
		}

		strbuf_add(base, entry.path, te_len);

		यदि (S_ISREG(entry.mode)) {
			hit |= grep_oid(opt, &entry.oid, base->buf, tn_len,
					 check_attr ? base->buf + tn_len : रिक्त);
		} अन्यथा यदि (S_ISDIR(entry.mode)) {
			गणना object_type type;
			संरचना वृक्ष_desc sub;
			शून्य *data;
			size_t size;

			data = odb_read_object(the_repository->objects,
					       &entry.oid, &type, &size);
			यदि (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_वृक्ष_desc(&sub, &entry.oid, data, size);
			hit |= grep_वृक्ष(opt, pathspec, &sub, base, tn_len,
					 check_attr);
			मुक्त(data);
		} अन्यथा यदि (recurse_submodules && S_ISGITLINK(entry.mode)) {
			hit |= grep_submodule(opt, pathspec, &entry.oid,
					      base->buf, base->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(base, old_baselen);

		यदि (hit && opt->status_only)
			भङ्ग;
	}

	strbuf_release(&name);
	प्रत्यागम hit;
}

स्थिर शून्य collect_blob_oids_for_वृक्ष(संरचना repository *repo,
				       ध्रुव संरचना pathspec *pathspec,
				       संरचना वृक्ष_desc *वृक्ष,
				       संरचना strbuf *base,
				       पूर्णाङ्क tn_len,
				       संरचना oidset *blob_oids)
{
	संरचना name_entry entry;
	पूर्णाङ्क old_baselen = base->len;
	संरचना strbuf name = STRBUF_INIT;
	गणना interesting match = entry_not_interesting;

	यावत् (वृक्ष_entry(वृक्ष, &entry)) {
		यदि (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = वृक्ष_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_reset(&name);

			यदि (match == all_entries_not_interesting)
				भङ्ग;
			यदि (match == entry_not_interesting)
				अनुवर्त;
		}

		strbuf_add(base, entry.path, वृक्ष_entry_len(&entry));

		यदि (S_ISREG(entry.mode)) {
			यदि (!odb_has_object(repo->objects, &entry.oid, 0))
				oidset_जोड़ो(blob_oids, &entry.oid);
		} अन्यथा यदि (S_ISDIR(entry.mode)) {
			गणना object_type type;
			संरचना वृक्ष_desc sub_वृक्ष;
			शून्य *data;
			size_t size;

			data = odb_read_object(repo->objects, &entry.oid,
					       &type, &size);
			यदि (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_वृक्ष_desc(&sub_वृक्ष, &entry.oid, data, size);
			collect_blob_oids_for_वृक्ष(repo, pathspec, &sub_वृक्ष,
						   base, tn_len, blob_oids);
			मुक्त(data);
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

स्थिर शून्य collect_blob_oids_for_treeish(संरचना grep_opt *opt,
					  ध्रुव संरचना pathspec *pathspec,
					  ध्रुव संरचना object_id *वृक्ष_ish_oid,
					  ध्रुव वर्ण *name,
					  संरचना oidset *blob_oids)
{
	संरचना वृक्ष_desc वृक्ष;
	शून्य *data;
	size_t size;
	संरचना strbuf base = STRBUF_INIT;
	पूर्णाङ्क len;

	data = odb_read_object_peeled(opt->repo->objects, वृक्ष_ish_oid,
				      OBJ_वृक्ष, &size, रिक्त);

	यदि (!data)
		प्रत्यागम;

	len = name ? strlen(name) : 0;
	यदि (len) {
		strbuf_add(&base, name, len);
		strbuf_addch(&base, ':');
	}
	init_वृक्ष_desc(&वृक्ष, वृक्ष_ish_oid, data, size);

	collect_blob_oids_for_वृक्ष(opt->repo, pathspec, &वृक्ष,
				   &base, base.len, blob_oids);

	strbuf_release(&base);
	मुक्त(data);
}

स्थिर शून्य prefetch_grep_blobs(संरचना grep_opt *opt,
				ध्रुव संरचना pathspec *pathspec,
				ध्रुव संरचना object_array *list)
{
	संरचना oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	यदि (!repo_has_promisor_remote(opt->repo))
		प्रत्यागम;

	/* For each tree, gather the blobs in it */
	प्रति (पूर्णाङ्क i = 0; i < list->nr; i++) {
		संरचना object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     रिक्त, 0);
		obj_read_unlock();

		यदि (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_वृक्ष))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	यदि (oidset_size(&blob_oids)) {
		संरचना oid_array to_fetch = OID_ARRAY_INIT;
		संरचना oidset_iter iter;
		ध्रुव संरचना object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		यावत् ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(opt->repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}
	oidset_clear(&blob_oids);
}

स्थिर पूर्णाङ्क grep_object(संरचना grep_opt *opt, ध्रुव संरचना pathspec *pathspec,
		       संरचना object *obj, ध्रुव वर्ण *name, ध्रुव वर्ण *path)
{
	यदि (obj->type == OBJ_BLOB)
		प्रत्यागम grep_oid(opt, &obj->oid, name, 0, path);
	यदि (obj->type == OBJ_COMMIT || obj->type == OBJ_वृक्ष) {
		संरचना वृक्ष_desc वृक्ष;
		शून्य *data;
		size_t size;
		संरचना strbuf base;
		पूर्णाङ्क hit, len;

		data = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_वृक्ष, &size, रिक्त);
		यदि (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(&obj->oid));

		len = name ? strlen(name) : 0;
		strbuf_init(&base, PATH_MAX + len + 1);
		यदि (len) {
			strbuf_add(&base, name, len);
			strbuf_addch(&base, ':');
		}
		init_वृक्ष_desc(&वृक्ष, &obj->oid, data, size);
		hit = grep_वृक्ष(opt, pathspec, &वृक्ष, &base, base.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&base);
		मुक्त(data);
		प्रत्यागम hit;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

स्थिर पूर्णाङ्क grep_objects(संरचना grep_opt *opt, ध्रुव संरचना pathspec *pathspec,
			ध्रुव संरचना object_array *list)
{
	unsigned पूर्णाङ्क i;
	पूर्णाङ्क hit = 0;
	ध्रुव unsigned पूर्णाङ्क nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	प्रति (i = 0; i < nr; i++) {
		संरचना object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     रिक्त, 0);
		obj_read_unlock();

		यदि (!real_obj) {
			वर्ण hex[GIT_MAX_HEXSZ + 1];
			ध्रुव वर्ण *name = list->objects[i].name;

			यदि (!name) {
				oid_to_hex_r(hex, &list->objects[i].item->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		यदि (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_lock();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		यदि (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].path)) {
			hit = 1;
			यदि (opt->status_only)
				भङ्ग;
		}
	}
	प्रत्यागम hit;
}

स्थिर पूर्णाङ्क grep_directory(संरचना grep_opt *opt, ध्रुव संरचना pathspec *pathspec,
			  पूर्णाङ्क exc_std, पूर्णाङ्क use_index)
{
	संरचना dir_struct dir = DIR_INIT;
	पूर्णाङ्क i, hit = 0;

	यदि (!use_index)
		dir.flags |= DIR_NO_GITLINKS;
	यदि (exc_std)
		setup_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->index, pathspec);
	प्रति (i = 0; i < dir.nr; i++) {
		hit |= grep_file(opt, dir.entries[i]->name);
		यदि (hit && opt->status_only)
			भङ्ग;
	}
	dir_clear(&dir);
	प्रत्यागम hit;
}

स्थिर पूर्णाङ्क context_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			    पूर्णाङ्क unset)
{
	संरचना grep_opt *grep_opt = opt->मूल्य;
	पूर्णाङ्क मूल्य;
	ध्रुव वर्ण *endp;

	यदि (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		प्रत्यागम 0;
	}
	मूल्य = strtol(arg, (वर्ण **)&endp, 10);
	यदि (*endp) {
		प्रत्यागम error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = मूल्य;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क file_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना grep_opt *grep_opt = opt->मूल्य;
	पूर्णाङ्क from_stdin;
	ध्रुव वर्ण *filename = arg;
	FILE *patterns;
	पूर्णाङ्क lno = 0;
	संरचना strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	यदि (!*filename)
		; /* leave it as-is */
	अन्यथा
		filename = prefix_filename_except_for_dash(grep_prefix, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	यदि (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	यावत् (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		यदि (sb.len == 0)
			अनुवर्त;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_PATTERN);
	}
	यदि (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	यदि (filename != arg)
		मुक्त((शून्य *)filename);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क not_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना grep_opt *grep_opt = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--not", "command line", 0, GREP_NOT);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क and_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना grep_opt *grep_opt = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--and", "command line", 0, GREP_AND);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क open_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना grep_opt *grep_opt = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क close_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना grep_opt *grep_opt = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क pattern_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			    पूर्णाङ्क unset)
{
	संरचना grep_opt *grep_opt = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	append_grep_pattern(grep_opt, arg, "-e option", 0, GREP_PATTERN);
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_grep(पूर्णाङ्क argc,
	     ध्रुव वर्ण **argv,
	     ध्रुव वर्ण *prefix,
	     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क hit = 0;
	पूर्णाङ्क cached = 0, untracked = 0, opt_exclude = -1;
	पूर्णाङ्क seen_dashdash = 0;
	पूर्णाङ्क external_grep_allowed__ignored;
	ध्रुव वर्ण *show_in_pager = रिक्त, *default_pager = "dummy";
	संरचना grep_opt opt;
	संरचना object_array list = OBJECT_ARRAY_INIT;
	संरचना pathspec pathspec;
	संरचना string_list path_list = STRING_LIST_INIT_DUP;
	पूर्णाङ्क i;
	पूर्णाङ्क dummy;
	पूर्णाङ्क use_index = 1;
	पूर्णाङ्क allow_revs;
	पूर्णाङ्क ret;

	संरचना option options[] = {
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
		OPT_SET_INT('a', "text", &opt.द्विआधारी,
			N_("process binary files as text"), GREP_द्विआधारी_TEXT),
		OPT_SET_INT('I', रिक्त, &opt.द्विआधारी,
			N_("don't match patterns in binary files"),
			GREP_द्विआधारी_NOMATCH),
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
		OPT_NEGBIT('h', रिक्त, &opt.pathname, N_("don't show filenames"), 1),
		OPT_BIT('H', रिक्त, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.गिनती,
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
		OPT_INTEGER(0, "threads", &अंक_threads,
			N_("use <n> worker threads")),
		OPT_संख्या_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_GROUP(""),
		OPT_CALLBACK('f', रिक्त, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', रिक्त, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, pattern_callback),
		OPT_CALLBACK_F(0, "and", &opt, रिक्त,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, रिक्त, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', रिक्त, &opt, रिक्त, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', रिक्त, &opt, रिक्त, "",
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
			.मूल्य = &show_in_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_गिनती,
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

	यदि (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	यदि (use_index && !startup_info->have_repository) {
		पूर्णाङ्क fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		यदि (fallback)
			use_index = 0;
		अन्यथा
			/* die the same way as if we did it at the beginning */
			setup_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	यदि (!use_index)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	यदि (argc > 0 && !opt.pattern_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	यदि (argc > 0 && !opt.pattern_list) {
		append_grep_pattern(&opt, argv[0], "command line", 0,
				    GREP_PATTERN);
		argv++;
		argc--;
	}

	यदि (show_in_pager == default_pager)
		show_in_pager = git_pager(the_repository, 1);
	यदि (show_in_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &path_list;
		opt.output = append_path;
		string_list_append(&path_list, show_in_pager);
	}

	यदि (!opt.pattern_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	यदि (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	प्रति (i = 0; i < argc; i++) {
		यदि (!strcmp(argv[i], "--")) {
			seen_dashdash = 1;
			भङ्ग;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_index && !untracked;
	प्रति (i = 0; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		संरचना object_id oid;
		संरचना object_context oc = {0};
		संरचना object *object;

		यदि (!strcmp(arg, "--")) {
			i++;
			भङ्ग;
		}

		यदि (!allow_revs) {
			यदि (seen_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			भङ्ग;
		}

		यदि (get_oid_with_context(the_repository, arg,
					 GET_OID_RECORD_PATH,
					 &oid, &oc)) {
			यदि (seen_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			भङ्ग;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		यदि (!seen_dashdash)
			verify_non_filename(the_repository, prefix, arg);
		add_object_array_with_path(object, arg, &list, oc.mode, oc.path);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	यदि (!seen_dashdash) {
		पूर्णाङ्क j;
		प्रति (j = i; j < argc; j++)
			verify_filename(the_repository, prefix, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_depth != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       prefix, argv + i);
	pathspec.max_depth = opt.max_depth;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	यदि (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	यदि (opt.max_गिनती == 0) {
		ret = 1;
		गच्छ out;
	}

	यदि (show_in_pager) {
		यदि (अंक_threads > 1)
			warning(_("invalid option combination, ignoring --threads"));
		अंक_threads = 1;
	} अन्यथा यदि (!HAVE_THREADS && अंक_threads > 1) {
		warning(_("no threads support, ignoring --threads"));
		अंक_threads = 1;
	} अन्यथा यदि (अंक_threads < 0)
		die(_("invalid number of threads specified (%d)"), अंक_threads);
	अन्यथा यदि (अंक_threads == 0)
		अंक_threads = HAVE_THREADS ? online_cpus() : 1;

	यदि (अंक_threads > 1) {
		यदि (!HAVE_THREADS)
			BUG("Somebody got num_threads calculation wrong!");
		यदि (!(opt.name_only || opt.unmatch_name_only || opt.गिनती)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_first_पंक्ति = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		यदि (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		यदि (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		start_threads(&opt);
	} अन्यथा {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	यदि (show_in_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	यदि (show_in_pager && opt.pattern_list && !opt.pattern_list->next) {
		ध्रुव वर्ण *pager = path_list.वस्तुएँ[0].string;
		पूर्णाङ्क len = strlen(pager);

		यदि (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		यदि (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&path_list, "-I");

		यदि (!strcmp("less", pager) || !strcmp("vi", pager)) {
			संरचना strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.pattern_list->pattern);
			string_list_append_nodup(&path_list,
						 strbuf_detach(&buf, रिक्त));
		}
	}

	यदि (!show_in_pager && !opt.status_only)
		setup_pager(the_repository);

	die_for_incompatible_opt3(!use_index, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	यदि (!use_index || untracked) {
		पूर्णाङ्क use_exclude = (opt_exclude < 0) ? use_index : !!opt_exclude;
		hit = grep_directory(&opt, &pathspec, use_exclude, use_index);
	} अन्यथा यदि (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} अन्यथा यदि (!list.nr) {
		यदि (!cached)
			setup_work_वृक्ष(the_repository);

		hit = grep_cache(&opt, &pathspec, cached);
	} अन्यथा {
		यदि (cached)
			die(_("both --cached and trees are given"));

		hit = grep_objects(&opt, &pathspec, &list);
	}

	यदि (अंक_threads > 1)
		hit |= wait_all();
	यदि (hit && show_in_pager)
		run_pager(&opt, prefix);

	ret = !hit;

out:
	clear_pathspec(&pathspec);
	string_list_clear(&path_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	प्रत्यागम ret;
}
