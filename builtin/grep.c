/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "config.h"
#समावेश "tag.h"
#समावेश "tree-walk.h"
#समावेश "parse-options.h"
#समावेश "string-list.h"
#समावेश "run-command.h"
#समावेश "grep.h"
#समावेश "quote.h"
#समावेश "dir.h"
#समावेश "pathspec.h"
#समावेश "setup.h"
#समावेश "submodule.h"
#समावेश "submodule-config.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "odb/source.h"
#समावेश "oid-array.h"
#समावेश "oidset.h"
#समावेश "pager.h"
#समावेश "path.h"
#समावेश "promisor-remote.h"
#समावेश "read-cache-ll.h"
#समावेश "write-or-die.h"

अटल स्थिर अक्षर *grep_prefix;

अटल अक्षर स्थिर * स्थिर grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	शून्य
};

अटल पूर्णांक recurse_submodules;

अटल पूर्णांक अंक_threads;

अटल pthread_t *threads;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
काष्ठा work_item {
	काष्ठा grep_source source;
	अक्षर done;
	काष्ठा strbuf out;
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
#घोषणा TODO_आकार 128
अटल काष्ठा work_item todo[TODO_आकार];
अटल पूर्णांक todo_start;
अटल पूर्णांक todo_end;
अटल पूर्णांक todo_done;

/* Has all work items been added? */
अटल पूर्णांक all_work_added;

अटल काष्ठा repository **repos_to_free;
अटल माप_प्रकार repos_to_free_nr, repos_to_free_alloc;

/* This lock protects all the variables above. */
अटल pthread_mutex_t grep_mutex;

अटल अंतरभूत व्योम grep_lock(व्योम)
{
	pthread_mutex_lock(&grep_mutex);
}

अटल अंतरभूत व्योम grep_unlock(व्योम)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
अटल pthread_cond_t cond_add;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
अटल pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
अटल pthread_cond_t cond_परिणाम;

अटल पूर्णांक skip_first_पंक्ति;

अटल व्योम add_work(काष्ठा grep_opt *opt, काष्ठा grep_source *gs)
{
	अगर (opt->द्विआधारी != GREP_द्विआधारी_TEXT)
		grep_source_load_driver(gs, opt->repo->सूचकांक);

	grep_lock();

	जबतक ((todo_end+1) % ARRAY_आकार(todo) == todo_done) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_end].source = *gs;
	todo[todo_end].done = 0;
	strbuf_reset(&todo[todo_end].out);
	todo_end = (todo_end + 1) % ARRAY_आकार(todo);

	pthread_cond_signal(&cond_add);
	grep_unlock();
}

अटल काष्ठा work_item *get_work(व्योम)
{
	काष्ठा work_item *ret;

	grep_lock();
	जबतक (todo_start == todo_end && !all_work_added) {
		pthread_cond_wait(&cond_add, &grep_mutex);
	}

	अगर (todo_start == todo_end && all_work_added) {
		ret = शून्य;
	} अन्यथा {
		ret = &todo[todo_start];
		todo_start = (todo_start + 1) % ARRAY_आकार(todo);
	}
	grep_unlock();
	वापस ret;
}

अटल व्योम work_done(काष्ठा work_item *w)
{
	पूर्णांक old_done;

	grep_lock();
	w->done = 1;
	old_done = todo_done;
	क्रम(; todo[todo_done].done && todo_done != todo_start;
	    todo_done = (todo_done+1) % ARRAY_आकार(todo)) {
		w = &todo[todo_done];
		अगर (w->out.len) {
			स्थिर अक्षर *p = w->out.buf;
			माप_प्रकार len = w->out.len;

			/* Skip the leading hunk mark of the first file. */
			अगर (skip_first_पंक्ति) {
				जबतक (len) {
					len--;
					अगर (*p++ == '\n')
						अवरोध;
				}
				skip_first_पंक्ति = 0;
			}

			write_or_die(1, p, len);
		}
		grep_source_clear(&w->source);
	}

	अगर (old_done != todo_done)
		pthread_cond_signal(&cond_write);

	अगर (all_work_added && todo_done == todo_end)
		pthread_cond_signal(&cond_परिणाम);

	grep_unlock();
}

अटल व्योम free_repos(व्योम)
{
	पूर्णांक i;

	क्रम (i = 0; i < repos_to_free_nr; i++) {
		repo_clear(repos_to_free[i]);
		मुक्त(repos_to_free[i]);
	}
	FREE_AND_NULL(repos_to_free);
	repos_to_free_nr = 0;
	repos_to_free_alloc = 0;
}

अटल व्योम *run(व्योम *arg)
{
	पूर्णांक hit = 0;
	काष्ठा grep_opt *opt = arg;

	जबतक (1) {
		काष्ठा work_item *w = get_work();
		अगर (!w)
			अवरोध;

		opt->output_priv = w;
		hit |= grep_source(opt, &w->source);
		grep_source_clear_तथ्य(&w->source);
		work_done(w);
	}
	free_grep_patterns(opt);
	मुक्त(opt);

	वापस (व्योम*) (intptr_t) hit;
}

अटल व्योम strbuf_out(काष्ठा grep_opt *opt, स्थिर व्योम *buf, माप_प्रकार आकार)
{
	काष्ठा work_item *w = opt->output_priv;
	strbuf_add(&w->out, buf, आकार);
}

अटल व्योम start_threads(काष्ठा grep_opt *opt)
{
	पूर्णांक i;

	pthread_mutex_init(&grep_mutex, शून्य);
	pthread_mutex_init(&grep_attr_mutex, शून्य);
	pthread_cond_init(&cond_add, शून्य);
	pthread_cond_init(&cond_write, शून्य);
	pthread_cond_init(&cond_परिणाम, शून्य);
	grep_use_locks = 1;
	enable_obj_read_lock();

	क्रम (i = 0; i < ARRAY_आकार(todo); i++) {
		strbuf_init(&todo[i].out, 0);
	}

	CALLOC_ARRAY(threads, अंक_threads);
	क्रम (i = 0; i < अंक_threads; i++) {
		पूर्णांक err;
		काष्ठा grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_out;
		compile_grep_patterns(o);
		err = pthread_create(&threads[i], शून्य, run, o);

		अगर (err)
			die(_("grep: failed to create thread: %s"),
			    म_त्रुटि(err));
	}
}

अटल पूर्णांक wait_all(व्योम)
{
	पूर्णांक hit = 0;
	पूर्णांक i;

	अगर (!HAVE_THREADS)
		BUG("Never call this function unless you have started threads");

	grep_lock();
	all_work_added = 1;

	/* Wait until all work is done. */
	जबतक (todo_done != todo_end)
		pthread_cond_wait(&cond_परिणाम, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_broadcast(&cond_add);
	grep_unlock();

	क्रम (i = 0; i < अंक_threads; i++) {
		व्योम *h;
		pthread_join(threads[i], &h);
		hit |= (पूर्णांक) (intptr_t) h;
	}

	मुक्त(threads);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_add);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_परिणाम);
	grep_use_locks = 0;
	disable_obj_read_lock();

	वापस hit;
}

अटल पूर्णांक grep_cmd_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			   स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	पूर्णांक st = grep_config(var, मूल्य, ctx, cb);

	अगर (git_color_config(var, मूल्य, cb) < 0)
		st = -1;
	अन्यथा अगर (git_default_config(var, मूल्य, ctx, cb) < 0)
		st = -1;

	अगर (!म_भेद(var, "grep.threads")) {
		अंक_threads = git_config_int(var, मूल्य, ctx->kvi);
		अगर (अंक_threads < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    अंक_threads, var);
		अन्यथा अगर (!HAVE_THREADS && अंक_threads > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			अंक_threads = 1;
		}
	}

	अगर (!म_भेद(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, मूल्य);

	वापस st;
}

अटल व्योम grep_source_name(काष्ठा grep_opt *opt, स्थिर अक्षर *filename,
			     पूर्णांक वृक्ष_name_len, काष्ठा strbuf *out)
{
	strbuf_reset(out);

	अगर (opt->null_following_name) {
		अगर (opt->relative && grep_prefix) {
			काष्ठा strbuf rel_buf = STRBUF_INIT;
			स्थिर अक्षर *rel_name =
				relative_path(filename + वृक्ष_name_len,
					      grep_prefix, &rel_buf);

			अगर (वृक्ष_name_len)
				strbuf_add(out, filename, वृक्ष_name_len);

			strbuf_addstr(out, rel_name);
			strbuf_release(&rel_buf);
		} अन्यथा {
			strbuf_addstr(out, filename);
		}
		वापस;
	}

	अगर (opt->relative && grep_prefix)
		quote_path(filename + वृक्ष_name_len, grep_prefix, out, 0);
	अन्यथा
		quote_c_style(filename + वृक्ष_name_len, out, शून्य, 0);

	अगर (वृक्ष_name_len)
		strbuf_जोड़ो(out, 0, filename, वृक्ष_name_len);
}

अटल पूर्णांक grep_oid(काष्ठा grep_opt *opt, स्थिर काष्ठा object_id *oid,
		     स्थिर अक्षर *filename, पूर्णांक वृक्ष_name_len,
		     स्थिर अक्षर *path)
{
	काष्ठा strbuf pathbuf = STRBUF_INIT;
	काष्ठा grep_source gs;

	grep_source_name(opt, filename, वृक्ष_name_len, &pathbuf);
	grep_source_init_oid(&gs, pathbuf.buf, path, oid, opt->repo);
	strbuf_release(&pathbuf);

	अगर (अंक_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		वापस 0;
	} अन्यथा {
		पूर्णांक hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		वापस hit;
	}
}

अटल पूर्णांक grep_file(काष्ठा grep_opt *opt, स्थिर अक्षर *filename)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा grep_source gs;

	grep_source_name(opt, filename, 0, &buf);
	grep_source_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	अगर (अंक_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		वापस 0;
	} अन्यथा {
		पूर्णांक hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		वापस hit;
	}
}

अटल व्योम append_path(काष्ठा grep_opt *opt, स्थिर व्योम *तथ्य, माप_प्रकार len)
{
	काष्ठा string_list *path_list = opt->output_priv;

	अगर (len == 1 && *(स्थिर अक्षर *)तथ्य == '\0')
		वापस;
	string_list_append_nodup(path_list, xstrndup(तथ्य, len));
}

अटल व्योम run_pager(काष्ठा grep_opt *opt, स्थिर अक्षर *prefix)
{
	काष्ठा string_list *path_list = opt->output_priv;
	काष्ठा child_process child = CHILD_PROCESS_INIT;
	पूर्णांक i, status;

	क्रम (i = 0; i < path_list->nr; i++)
		strvec_धकेलो(&child.args, path_list->वस्तुएँ[i].string);
	child.dir = prefix;
	child.use_shell = 1;

	status = run_command(&child);
	अगर (status)
		निकास(status);
}

अटल पूर्णांक grep_cache(काष्ठा grep_opt *opt,
		      स्थिर काष्ठा pathspec *pathspec, पूर्णांक cached);
अटल पूर्णांक grep_वृक्ष(काष्ठा grep_opt *opt, स्थिर काष्ठा pathspec *pathspec,
		     काष्ठा वृक्ष_desc *वृक्ष, काष्ठा strbuf *base, पूर्णांक tn_len,
		     पूर्णांक check_attr);

अटल पूर्णांक grep_submodule(काष्ठा grep_opt *opt,
			  स्थिर काष्ठा pathspec *pathspec,
			  स्थिर काष्ठा object_id *oid,
			  स्थिर अक्षर *filename, स्थिर अक्षर *path, पूर्णांक cached)
{
	काष्ठा repository *subrepo;
	काष्ठा repository *superproject = opt->repo;
	काष्ठा grep_opt subopt;
	पूर्णांक hit = 0;

	अगर (!is_submodule_active(superproject, path))
		वापस 0;

	subrepo = xmalloc(माप(*subrepo));
	अगर (repo_submodule_init(subrepo, superproject, path, null_oid(opt->repo->hash_algo))) {
		मुक्त(subrepo);
		वापस 0;
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

	स_नकल(&subopt, opt, माप(subopt));
	subopt.repo = subrepo;

	अगर (oid) {
		क्रमागत object_type object_type;
		काष्ठा वृक्ष_desc वृक्ष;
		व्योम *तथ्य;
		माप_प्रकार आकार;
		काष्ठा strbuf base = STRBUF_INIT;

		obj_read_lock();
		object_type = odb_read_object_info(subrepo->objects, oid, शून्य);
		obj_read_unlock();
		तथ्य = odb_read_object_peeled(subrepo->objects, oid, OBJ_वृक्ष, &आकार, शून्य);
		अगर (!तथ्य)
			die(_("unable to read tree (%s)"), oid_to_hex(oid));

		strbuf_addstr(&base, filename);
		strbuf_addch(&base, '/');

		init_वृक्ष_desc(&वृक्ष, oid, तथ्य, आकार);
		hit = grep_वृक्ष(&subopt, pathspec, &वृक्ष, &base, base.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&base);
		मुक्त(तथ्य);
	} अन्यथा {
		hit = grep_cache(&subopt, pathspec, cached);
	}

	वापस hit;
}

अटल पूर्णांक grep_cache(काष्ठा grep_opt *opt,
		      स्थिर काष्ठा pathspec *pathspec, पूर्णांक cached)
{
	काष्ठा repository *repo = opt->repo;
	पूर्णांक hit = 0;
	पूर्णांक nr;
	काष्ठा strbuf name = STRBUF_INIT;
	पूर्णांक name_base_len = 0;
	अगर (repo->submodule_prefix) {
		name_base_len = म_माप(repo->submodule_prefix);
		strbuf_addstr(&name, repo->submodule_prefix);
	}

	अगर (repo_read_सूचकांक(repo) < 0)
		die(_("index file corrupt"));

	क्रम (nr = 0; nr < repo->सूचकांक->cache_nr; nr++) {
		स्थिर काष्ठा cache_entry *ce = repo->सूचकांक->cache[nr];

		अगर (!cached && ce_skip_worktree(ce))
			जारी;

		strbuf_setlen(&name, name_base_len);
		strbuf_addstr(&name, ce->name);
		अगर (S_ISSPARSEDIR(ce->ce_mode)) {
			क्रमागत object_type type;
			काष्ठा वृक्ष_desc वृक्ष;
			व्योम *तथ्य;
			माप_प्रकार आकार;

			तथ्य = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &आकार);
			अगर (!तथ्य)
				die(_("unable to read tree %s"), oid_to_hex(&ce->oid));
			init_वृक्ष_desc(&वृक्ष, &ce->oid, तथ्य, आकार);

			hit |= grep_वृक्ष(opt, pathspec, &वृक्ष, &name, 0, 0);
			strbuf_setlen(&name, name_base_len);
			strbuf_addstr(&name, ce->name);
			मुक्त(तथ्य);
		} अन्यथा अगर (S_ISREG(ce->ce_mode) &&
		    match_pathspec(repo->सूचकांक, pathspec, name.buf, name.len, 0, शून्य,
				   S_ISDIR(ce->ce_mode) ||
				   S_ISGITLINK(ce->ce_mode))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			अगर (cached || (ce->ce_flags & CE_VALID)) {
				अगर (ce_stage(ce) || ce_intent_to_add(ce))
					जारी;
				hit |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} अन्यथा {
				hit |= grep_file(opt, name.buf);
			}
		} अन्यथा अगर (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
			   submodule_path_match(repo->सूचकांक, pathspec, name.buf, शून्य)) {
			hit |= grep_submodule(opt, pathspec, शून्य, ce->name,
					      ce->name, cached);
		} अन्यथा {
			जारी;
		}

		अगर (ce_stage(ce)) {
			करो {
				nr++;
			} जबतक (nr < repo->सूचकांक->cache_nr &&
				 !म_भेद(ce->name, repo->सूचकांक->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		अगर (hit && opt->status_only)
			अवरोध;
	}

	strbuf_release(&name);
	वापस hit;
}

अटल पूर्णांक grep_वृक्ष(काष्ठा grep_opt *opt, स्थिर काष्ठा pathspec *pathspec,
		     काष्ठा वृक्ष_desc *वृक्ष, काष्ठा strbuf *base, पूर्णांक tn_len,
		     पूर्णांक check_attr)
{
	काष्ठा repository *repo = opt->repo;
	पूर्णांक hit = 0;
	क्रमागत interesting match = entry_not_interesting;
	काष्ठा name_entry entry;
	पूर्णांक old_baselen = base->len;
	काष्ठा strbuf name = STRBUF_INIT;
	पूर्णांक name_base_len = 0;
	अगर (repo->submodule_prefix) {
		strbuf_addstr(&name, repo->submodule_prefix);
		name_base_len = name.len;
	}

	जबतक (वृक्ष_entry(वृक्ष, &entry)) {
		पूर्णांक te_len = वृक्ष_entry_len(&entry);

		अगर (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = वृक्ष_entry_interesting(repo->सूचकांक,
						       &entry, &name,
						       pathspec);
			strbuf_setlen(&name, name_base_len);

			अगर (match == all_entries_not_interesting)
				अवरोध;
			अगर (match == entry_not_interesting)
				जारी;
		}

		strbuf_add(base, entry.path, te_len);

		अगर (S_ISREG(entry.mode)) {
			hit |= grep_oid(opt, &entry.oid, base->buf, tn_len,
					 check_attr ? base->buf + tn_len : शून्य);
		} अन्यथा अगर (S_ISDIR(entry.mode)) {
			क्रमागत object_type type;
			काष्ठा वृक्ष_desc sub;
			व्योम *तथ्य;
			माप_प्रकार आकार;

			तथ्य = odb_read_object(the_repository->objects,
					       &entry.oid, &type, &आकार);
			अगर (!तथ्य)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_वृक्ष_desc(&sub, &entry.oid, तथ्य, आकार);
			hit |= grep_वृक्ष(opt, pathspec, &sub, base, tn_len,
					 check_attr);
			मुक्त(तथ्य);
		} अन्यथा अगर (recurse_submodules && S_ISGITLINK(entry.mode)) {
			hit |= grep_submodule(opt, pathspec, &entry.oid,
					      base->buf, base->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(base, old_baselen);

		अगर (hit && opt->status_only)
			अवरोध;
	}

	strbuf_release(&name);
	वापस hit;
}

अटल व्योम collect_blob_oids_for_वृक्ष(काष्ठा repository *repo,
				       स्थिर काष्ठा pathspec *pathspec,
				       काष्ठा वृक्ष_desc *वृक्ष,
				       काष्ठा strbuf *base,
				       पूर्णांक tn_len,
				       काष्ठा oidset *blob_oids)
{
	काष्ठा name_entry entry;
	पूर्णांक old_baselen = base->len;
	काष्ठा strbuf name = STRBUF_INIT;
	क्रमागत interesting match = entry_not_interesting;

	जबतक (वृक्ष_entry(वृक्ष, &entry)) {
		अगर (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = वृक्ष_entry_interesting(repo->सूचकांक,
						       &entry, &name,
						       pathspec);
			strbuf_reset(&name);

			अगर (match == all_entries_not_interesting)
				अवरोध;
			अगर (match == entry_not_interesting)
				जारी;
		}

		strbuf_add(base, entry.path, वृक्ष_entry_len(&entry));

		अगर (S_ISREG(entry.mode)) {
			अगर (!odb_has_object(repo->objects, &entry.oid, 0))
				oidset_जोड़ो(blob_oids, &entry.oid);
		} अन्यथा अगर (S_ISDIR(entry.mode)) {
			क्रमागत object_type type;
			काष्ठा वृक्ष_desc sub_वृक्ष;
			व्योम *तथ्य;
			माप_प्रकार आकार;

			तथ्य = odb_read_object(repo->objects, &entry.oid,
					       &type, &आकार);
			अगर (!तथ्य)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_वृक्ष_desc(&sub_वृक्ष, &entry.oid, तथ्य, आकार);
			collect_blob_oids_for_वृक्ष(repo, pathspec, &sub_वृक्ष,
						   base, tn_len, blob_oids);
			मुक्त(तथ्य);
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

अटल व्योम collect_blob_oids_for_treeish(काष्ठा grep_opt *opt,
					  स्थिर काष्ठा pathspec *pathspec,
					  स्थिर काष्ठा object_id *वृक्ष_ish_oid,
					  स्थिर अक्षर *name,
					  काष्ठा oidset *blob_oids)
{
	काष्ठा वृक्ष_desc वृक्ष;
	व्योम *तथ्य;
	माप_प्रकार आकार;
	काष्ठा strbuf base = STRBUF_INIT;
	पूर्णांक len;

	तथ्य = odb_read_object_peeled(opt->repo->objects, वृक्ष_ish_oid,
				      OBJ_वृक्ष, &आकार, शून्य);

	अगर (!तथ्य)
		वापस;

	len = name ? म_माप(name) : 0;
	अगर (len) {
		strbuf_add(&base, name, len);
		strbuf_addch(&base, ':');
	}
	init_वृक्ष_desc(&वृक्ष, वृक्ष_ish_oid, तथ्य, आकार);

	collect_blob_oids_for_वृक्ष(opt->repo, pathspec, &वृक्ष,
				   &base, base.len, blob_oids);

	strbuf_release(&base);
	मुक्त(तथ्य);
}

अटल व्योम prefetch_grep_blobs(काष्ठा grep_opt *opt,
				स्थिर काष्ठा pathspec *pathspec,
				स्थिर काष्ठा object_array *list)
{
	काष्ठा oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	अगर (!repo_has_promisor_remote(opt->repo))
		वापस;

	/* For each tree, gather the blobs in it */
	क्रम (पूर्णांक i = 0; i < list->nr; i++) {
		काष्ठा object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     शून्य, 0);
		obj_read_unlock();

		अगर (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_वृक्ष))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	अगर (oidset_आकार(&blob_oids)) {
		काष्ठा oid_array to_fetch = OID_ARRAY_INIT;
		काष्ठा oidset_iter iter;
		स्थिर काष्ठा object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		जबतक ((oid = oidset_iter_अग्र(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(opt->repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}
	oidset_clear(&blob_oids);
}

अटल पूर्णांक grep_object(काष्ठा grep_opt *opt, स्थिर काष्ठा pathspec *pathspec,
		       काष्ठा object *obj, स्थिर अक्षर *name, स्थिर अक्षर *path)
{
	अगर (obj->type == OBJ_BLOB)
		वापस grep_oid(opt, &obj->oid, name, 0, path);
	अगर (obj->type == OBJ_COMMIT || obj->type == OBJ_वृक्ष) {
		काष्ठा वृक्ष_desc वृक्ष;
		व्योम *तथ्य;
		माप_प्रकार आकार;
		काष्ठा strbuf base;
		पूर्णांक hit, len;

		तथ्य = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_वृक्ष, &आकार, शून्य);
		अगर (!तथ्य)
			die(_("unable to read tree (%s)"), oid_to_hex(&obj->oid));

		len = name ? म_माप(name) : 0;
		strbuf_init(&base, PATH_MAX + len + 1);
		अगर (len) {
			strbuf_add(&base, name, len);
			strbuf_addch(&base, ':');
		}
		init_वृक्ष_desc(&वृक्ष, &obj->oid, तथ्य, आकार);
		hit = grep_वृक्ष(opt, pathspec, &वृक्ष, &base, base.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&base);
		मुक्त(तथ्य);
		वापस hit;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

अटल पूर्णांक grep_objects(काष्ठा grep_opt *opt, स्थिर काष्ठा pathspec *pathspec,
			स्थिर काष्ठा object_array *list)
{
	अचिन्हित पूर्णांक i;
	पूर्णांक hit = 0;
	स्थिर अचिन्हित पूर्णांक nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	क्रम (i = 0; i < nr; i++) {
		काष्ठा object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     शून्य, 0);
		obj_read_unlock();

		अगर (!real_obj) {
			अक्षर hex[GIT_MAX_HEXSZ + 1];
			स्थिर अक्षर *name = list->objects[i].name;

			अगर (!name) {
				oid_to_hex_r(hex, &list->objects[i].item->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		अगर (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_lock();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		अगर (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].path)) {
			hit = 1;
			अगर (opt->status_only)
				अवरोध;
		}
	}
	वापस hit;
}

अटल पूर्णांक grep_directory(काष्ठा grep_opt *opt, स्थिर काष्ठा pathspec *pathspec,
			  पूर्णांक exc_std, पूर्णांक use_सूचकांक)
{
	काष्ठा dir_struct dir = DIR_INIT;
	पूर्णांक i, hit = 0;

	अगर (!use_सूचकांक)
		dir.flags |= DIR_NO_GITLINKS;
	अगर (exc_std)
		setup_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->सूचकांक, pathspec);
	क्रम (i = 0; i < dir.nr; i++) {
		hit |= grep_file(opt, dir.entries[i]->name);
		अगर (hit && opt->status_only)
			अवरोध;
	}
	dir_clear(&dir);
	वापस hit;
}

अटल पूर्णांक context_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			    पूर्णांक unset)
{
	काष्ठा grep_opt *grep_opt = opt->मूल्य;
	पूर्णांक मूल्य;
	स्थिर अक्षर *endp;

	अगर (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		वापस 0;
	}
	मूल्य = म_से_दीर्घ(arg, (अक्षर **)&endp, 10);
	अगर (*endp) {
		वापस error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = मूल्य;
	वापस 0;
}

अटल पूर्णांक file_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा grep_opt *grep_opt = opt->मूल्य;
	पूर्णांक from_stdin;
	स्थिर अक्षर *filename = arg;
	खाता *patterns;
	पूर्णांक lno = 0;
	काष्ठा strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	अगर (!*filename)
		; /* leave it as-is */
	अन्यथा
		filename = prefix_filename_except_for_dash(grep_prefix, filename);

	from_stdin = !म_भेद(filename, "-");
	patterns = from_stdin ? मानक_निवेश : ख_खोलो(filename, "r");
	अगर (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	जबतक (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		अगर (sb.len == 0)
			जारी;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_PATTERN);
	}
	अगर (!from_stdin)
		ख_बंद(patterns);
	strbuf_release(&sb);
	अगर (filename != arg)
		मुक्त((व्योम *)filename);
	वापस 0;
}

अटल पूर्णांक not_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा grep_opt *grep_opt = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--not", "command line", 0, GREP_NOT);
	वापस 0;
}

अटल पूर्णांक and_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा grep_opt *grep_opt = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--and", "command line", 0, GREP_AND);
	वापस 0;
}

अटल पूर्णांक open_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा grep_opt *grep_opt = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	वापस 0;
}

अटल पूर्णांक close_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा grep_opt *grep_opt = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	वापस 0;
}

अटल पूर्णांक pattern_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			    पूर्णांक unset)
{
	काष्ठा grep_opt *grep_opt = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	append_grep_pattern(grep_opt, arg, "-e option", 0, GREP_PATTERN);
	वापस 0;
}

पूर्णांक cmd_grep(पूर्णांक argc,
	     स्थिर अक्षर **argv,
	     स्थिर अक्षर *prefix,
	     काष्ठा repository *repo UNUSED)
{
	पूर्णांक hit = 0;
	पूर्णांक cached = 0, untracked = 0, opt_exclude = -1;
	पूर्णांक seen_dashdash = 0;
	पूर्णांक external_grep_allowed__ignored;
	स्थिर अक्षर *show_in_pager = शून्य, *default_pager = "dummy";
	काष्ठा grep_opt opt;
	काष्ठा object_array list = OBJECT_ARRAY_INIT;
	काष्ठा pathspec pathspec;
	काष्ठा string_list path_list = STRING_LIST_INIT_DUP;
	पूर्णांक i;
	पूर्णांक dummy;
	पूर्णांक use_सूचकांक = 1;
	पूर्णांक allow_revs;
	पूर्णांक ret;

	काष्ठा option options[] = {
		OPT_BOOL(0, "cached", &cached,
			N_("search in index instead of in the work tree")),
		OPT_NEGBIT(0, "no-index", &use_सूचकांक,
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
		OPT_SET_INT('I', शून्य, &opt.द्विआधारी,
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
		OPT_NEGBIT('h', शून्य, &opt.pathname, N_("don't show filenames"), 1),
		OPT_BIT('H', शून्य, &opt.pathname, N_("show filenames"), 1),
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
		OPT_CALLBACK('f', शून्य, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', शून्य, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, pattern_callback),
		OPT_CALLBACK_F(0, "and", &opt, शून्य,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, शून्य, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', शून्य, &opt, शून्य, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', शून्य, &opt, शून्य, "",
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

	अगर (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_सूचकांक = 0;
	}

	अगर (use_सूचकांक && !startup_info->have_repository) {
		पूर्णांक fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		अगर (fallback)
			use_सूचकांक = 0;
		अन्यथा
			/* die the same way as if we did it at the beginning */
			setup_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	अगर (!use_सूचकांक)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	अगर (argc > 0 && !opt.pattern_list && !म_भेद(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	अगर (argc > 0 && !opt.pattern_list) {
		append_grep_pattern(&opt, argv[0], "command line", 0,
				    GREP_PATTERN);
		argv++;
		argc--;
	}

	अगर (show_in_pager == default_pager)
		show_in_pager = git_pager(the_repository, 1);
	अगर (show_in_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &path_list;
		opt.output = append_path;
		string_list_append(&path_list, show_in_pager);
	}

	अगर (!opt.pattern_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	अगर (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	क्रम (i = 0; i < argc; i++) {
		अगर (!म_भेद(argv[i], "--")) {
			seen_dashdash = 1;
			अवरोध;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_सूचकांक && !untracked;
	क्रम (i = 0; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];
		काष्ठा object_id oid;
		काष्ठा object_context oc = {0};
		काष्ठा object *object;

		अगर (!म_भेद(arg, "--")) {
			i++;
			अवरोध;
		}

		अगर (!allow_revs) {
			अगर (seen_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			अवरोध;
		}

		अगर (get_oid_with_context(the_repository, arg,
					 GET_OID_RECORD_PATH,
					 &oid, &oc)) {
			अगर (seen_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			अवरोध;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		अगर (!seen_dashdash)
			verify_non_filename(the_repository, prefix, arg);
		add_object_array_with_path(object, arg, &list, oc.mode, oc.path);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	अगर (!seen_dashdash) {
		पूर्णांक j;
		क्रम (j = i; j < argc; j++)
			verify_filename(the_repository, prefix, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_depth != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       prefix, argv + i);
	pathspec.max_depth = opt.max_depth;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	अगर (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	अगर (opt.max_गिनती == 0) {
		ret = 1;
		जाओ out;
	}

	अगर (show_in_pager) {
		अगर (अंक_threads > 1)
			warning(_("invalid option combination, ignoring --threads"));
		अंक_threads = 1;
	} अन्यथा अगर (!HAVE_THREADS && अंक_threads > 1) {
		warning(_("no threads support, ignoring --threads"));
		अंक_threads = 1;
	} अन्यथा अगर (अंक_threads < 0)
		die(_("invalid number of threads specified (%d)"), अंक_threads);
	अन्यथा अगर (अंक_threads == 0)
		अंक_threads = HAVE_THREADS ? online_cpus() : 1;

	अगर (अंक_threads > 1) {
		अगर (!HAVE_THREADS)
			BUG("Somebody got num_threads calculation wrong!");
		अगर (!(opt.name_only || opt.unmatch_name_only || opt.गिनती)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_first_पंक्ति = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		अगर (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		अगर (startup_info->have_repository)
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

	अगर (show_in_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	अगर (show_in_pager && opt.pattern_list && !opt.pattern_list->अग्र) {
		स्थिर अक्षर *pager = path_list.वस्तुएँ[0].string;
		पूर्णांक len = म_माप(pager);

		अगर (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		अगर (opt.ignore_case && !म_भेद("less", pager))
			string_list_append(&path_list, "-I");

		अगर (!म_भेद("less", pager) || !म_भेद("vi", pager)) {
			काष्ठा strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					म_भेद("less", pager) ? "" : "*",
					opt.pattern_list->pattern);
			string_list_append_nodup(&path_list,
						 strbuf_detach(&buf, शून्य));
		}
	}

	अगर (!show_in_pager && !opt.status_only)
		setup_pager(the_repository);

	die_for_incompatible_opt3(!use_सूचकांक, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	अगर (!use_सूचकांक || untracked) {
		पूर्णांक use_exclude = (opt_exclude < 0) ? use_सूचकांक : !!opt_exclude;
		hit = grep_directory(&opt, &pathspec, use_exclude, use_सूचकांक);
	} अन्यथा अगर (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} अन्यथा अगर (!list.nr) {
		अगर (!cached)
			setup_work_वृक्ष(the_repository);

		hit = grep_cache(&opt, &pathspec, cached);
	} अन्यथा {
		अगर (cached)
			die(_("both --cached and trees are given"));

		hit = grep_objects(&opt, &pathspec, &list);
	}

	अगर (अंक_threads > 1)
		hit |= wait_all();
	अगर (hit && show_in_pager)
		run_pager(&opt, prefix);

	ret = !hit;

out:
	clear_pathspec(&pathspec);
	string_list_clear(&path_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	वापस ret;
}
