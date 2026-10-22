/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "abspath.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "tag.h"
#অন্তর্ভাৱ "tree-walk.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "string-list.h"
#অন্তর্ভাৱ "run-command.h"
#অন্তর্ভাৱ "grep.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "dir.h"
#অন্তর্ভাৱ "pathspec.h"
#অন্তর্ভাৱ "setup.h"
#অন্তর্ভাৱ "submodule.h"
#অন্তর্ভাৱ "submodule-config.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "odb/source.h"
#অন্তর্ভাৱ "oid-array.h"
#অন্তর্ভাৱ "oidset.h"
#অন্তর্ভাৱ "pager.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "promisor-remote.h"
#অন্তর্ভাৱ "read-cache-ll.h"
#অন্তর্ভাৱ "write-or-die.h"

স্থির ধ্রুৱ ৱর্ণ *grep_prefix;

স্থির ৱর্ণ ধ্রুৱ * ধ্রুৱ grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	রিক্ত
};

স্থির পূর্ণাঙ্ক recurse_submodules;

স্থির পূর্ণাঙ্ক অঙ্ক_threads;

স্থির pthread_t *threads;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
সংরচনা work_item {
	সংরচনা grep_source source;
	ৱর্ণ done;
	সংরচনা strbuf out;
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
স্থির সংরচনা work_item todo[TODO_SIZE];
স্থির পূর্ণাঙ্ক todo_start;
স্থির পূর্ণাঙ্ক todo_end;
স্থির পূর্ণাঙ্ক todo_done;

/* Has all work items been added? */
স্থির পূর্ণাঙ্ক all_work_added;

স্থির সংরচনা repository **repos_to_free;
স্থির size_t repos_to_free_nr, repos_to_free_alloc;

/* This lock protects all the variables above. */
স্থির pthread_mutex_t grep_mutex;

স্থির অন্তঃস্থ শূন্য grep_lock(শূন্য)
{
	pthread_mutex_lock(&grep_mutex);
}

স্থির অন্তঃস্থ শূন্য grep_unlock(শূন্য)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
স্থির pthread_cond_t cond_add;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
স্থির pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
স্থির pthread_cond_t cond_result;

স্থির পূর্ণাঙ্ক skip_first_সাৰি;

স্থির শূন্য add_work(সংরচনা grep_opt *opt, সংরচনা grep_source *gs)
{
	যদি (opt->দ্বিমিক != GREP_দ্বিমিক_TEXT)
		grep_source_load_driver(gs, opt->repo->index);

	grep_lock();

	যেতিয়ালৈ ((todo_end+1) % ARRAY_SIZE(todo) == todo_done) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_end].source = *gs;
	todo[todo_end].done = 0;
	strbuf_reset(&todo[todo_end].out);
	todo_end = (todo_end + 1) % ARRAY_SIZE(todo);

	pthread_cond_signal(&cond_add);
	grep_unlock();
}

স্থির সংরচনা work_item *get_work(শূন্য)
{
	সংরচনা work_item *ret;

	grep_lock();
	যেতিয়ালৈ (todo_start == todo_end && !all_work_added) {
		pthread_cond_wait(&cond_add, &grep_mutex);
	}

	যদি (todo_start == todo_end && all_work_added) {
		ret = রিক্ত;
	} অন্যথা {
		ret = &todo[todo_start];
		todo_start = (todo_start + 1) % ARRAY_SIZE(todo);
	}
	grep_unlock();
	ঘূৰাই ret;
}

স্থির শূন্য work_done(সংরচনা work_item *w)
{
	পূর্ণাঙ্ক old_done;

	grep_lock();
	w->done = 1;
	old_done = todo_done;
	বাবে(; todo[todo_done].done && todo_done != todo_start;
	    todo_done = (todo_done+1) % ARRAY_SIZE(todo)) {
		w = &todo[todo_done];
		যদি (w->out.len) {
			ধ্রুৱ ৱর্ণ *p = w->out.buf;
			size_t len = w->out.len;

			/* Skip the leading hunk mark of the first file. */
			যদি (skip_first_সাৰি) {
				যেতিয়ালৈ (len) {
					len--;
					যদি (*p++ == '\n')
						ভঙ্গ;
				}
				skip_first_সাৰি = 0;
			}

			write_or_die(1, p, len);
		}
		grep_source_clear(&w->source);
	}

	যদি (old_done != todo_done)
		pthread_cond_signal(&cond_write);

	যদি (all_work_added && todo_done == todo_end)
		pthread_cond_signal(&cond_result);

	grep_unlock();
}

স্থির শূন্য free_repos(শূন্য)
{
	পূর্ণাঙ্ক i;

	বাবে (i = 0; i < repos_to_free_nr; i++) {
		repo_clear(repos_to_free[i]);
		মুক্ত(repos_to_free[i]);
	}
	FREE_AND_NULL(repos_to_free);
	repos_to_free_nr = 0;
	repos_to_free_alloc = 0;
}

স্থির শূন্য *run(শূন্য *arg)
{
	পূর্ণাঙ্ক hit = 0;
	সংরচনা grep_opt *opt = arg;

	যেতিয়ালৈ (1) {
		সংরচনা work_item *w = get_work();
		যদি (!w)
			ভঙ্গ;

		opt->output_priv = w;
		hit |= grep_source(opt, &w->source);
		grep_source_clear_data(&w->source);
		work_done(w);
	}
	free_grep_patterns(opt);
	মুক্ত(opt);

	ঘূৰাই (শূন্য*) (intptr_t) hit;
}

স্থির শূন্য strbuf_out(সংরচনা grep_opt *opt, ধ্রুৱ শূন্য *buf, size_t size)
{
	সংরচনা work_item *w = opt->output_priv;
	strbuf_add(&w->out, buf, size);
}

স্থির শূন্য start_threads(সংরচনা grep_opt *opt)
{
	পূর্ণাঙ্ক i;

	pthread_mutex_init(&grep_mutex, রিক্ত);
	pthread_mutex_init(&grep_attr_mutex, রিক্ত);
	pthread_cond_init(&cond_add, রিক্ত);
	pthread_cond_init(&cond_write, রিক্ত);
	pthread_cond_init(&cond_result, রিক্ত);
	grep_use_locks = 1;
	enable_obj_read_lock();

	বাবে (i = 0; i < ARRAY_SIZE(todo); i++) {
		strbuf_init(&todo[i].out, 0);
	}

	CALLOC_ARRAY(threads, অঙ্ক_threads);
	বাবে (i = 0; i < অঙ্ক_threads; i++) {
		পূর্ণাঙ্ক err;
		সংরচনা grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_out;
		compile_grep_patterns(o);
		err = pthread_create(&threads[i], রিক্ত, run, o);

		যদি (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

স্থির পূর্ণাঙ্ক wait_all(শূন্য)
{
	পূর্ণাঙ্ক hit = 0;
	পূর্ণাঙ্ক i;

	যদি (!HAVE_THREADS)
		BUG("Never call this function unless you have started threads");

	grep_lock();
	all_work_added = 1;

	/* Wait until all work is done. */
	যেতিয়ালৈ (todo_done != todo_end)
		pthread_cond_wait(&cond_result, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_broadcast(&cond_add);
	grep_unlock();

	বাবে (i = 0; i < অঙ্ক_threads; i++) {
		শূন্য *h;
		pthread_join(threads[i], &h);
		hit |= (পূর্ণাঙ্ক) (intptr_t) h;
	}

	মুক্ত(threads);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_add);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_result);
	grep_use_locks = 0;
	disable_obj_read_lock();

	ঘূৰাই hit;
}

স্থির পূর্ণাঙ্ক grep_cmd_config(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান,
			   ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	পূর্ণাঙ্ক st = grep_config(var, মান, ctx, cb);

	যদি (git_color_config(var, মান, cb) < 0)
		st = -1;
	অন্যথা যদি (git_default_config(var, মান, ctx, cb) < 0)
		st = -1;

	যদি (!strcmp(var, "grep.threads")) {
		অঙ্ক_threads = git_config_int(var, মান, ctx->kvi);
		যদি (অঙ্ক_threads < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    অঙ্ক_threads, var);
		অন্যথা যদি (!HAVE_THREADS && অঙ্ক_threads > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			অঙ্ক_threads = 1;
		}
	}

	যদি (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, মান);

	ঘূৰাই st;
}

স্থির শূন্য grep_source_name(সংরচনা grep_opt *opt, ধ্রুৱ ৱর্ণ *filename,
			     পূর্ণাঙ্ক বৃক্ষ_name_len, সংরচনা strbuf *out)
{
	strbuf_reset(out);

	যদি (opt->null_following_name) {
		যদি (opt->relative && grep_prefix) {
			সংরচনা strbuf rel_buf = STRBUF_INIT;
			ধ্রুৱ ৱর্ণ *rel_name =
				relative_path(filename + বৃক্ষ_name_len,
					      grep_prefix, &rel_buf);

			যদি (বৃক্ষ_name_len)
				strbuf_add(out, filename, বৃক্ষ_name_len);

			strbuf_addstr(out, rel_name);
			strbuf_release(&rel_buf);
		} অন্যথা {
			strbuf_addstr(out, filename);
		}
		ঘূৰাই;
	}

	যদি (opt->relative && grep_prefix)
		quote_path(filename + বৃক্ষ_name_len, grep_prefix, out, 0);
	অন্যথা
		quote_c_style(filename + বৃক্ষ_name_len, out, রিক্ত, 0);

	যদি (বৃক্ষ_name_len)
		strbuf_ঢোকাও(out, 0, filename, বৃক্ষ_name_len);
}

স্থির পূর্ণাঙ্ক grep_oid(সংরচনা grep_opt *opt, ধ্রুৱ সংরচনা object_id *oid,
		     ধ্রুৱ ৱর্ণ *filename, পূর্ণাঙ্ক বৃক্ষ_name_len,
		     ধ্রুৱ ৱর্ণ *path)
{
	সংরচনা strbuf pathbuf = STRBUF_INIT;
	সংরচনা grep_source gs;

	grep_source_name(opt, filename, বৃক্ষ_name_len, &pathbuf);
	grep_source_init_oid(&gs, pathbuf.buf, path, oid, opt->repo);
	strbuf_release(&pathbuf);

	যদি (অঙ্ক_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		ঘূৰাই 0;
	} অন্যথা {
		পূর্ণাঙ্ক hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		ঘূৰাই hit;
	}
}

স্থির পূর্ণাঙ্ক grep_file(সংরচনা grep_opt *opt, ধ্রুৱ ৱর্ণ *filename)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	সংরচনা grep_source gs;

	grep_source_name(opt, filename, 0, &buf);
	grep_source_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	যদি (অঙ্ক_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		ঘূৰাই 0;
	} অন্যথা {
		পূর্ণাঙ্ক hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		ঘূৰাই hit;
	}
}

স্থির শূন্য append_path(সংরচনা grep_opt *opt, ধ্রুৱ শূন্য *data, size_t len)
{
	সংরচনা string_list *path_list = opt->output_priv;

	যদি (len == 1 && *(ধ্রুৱ ৱর্ণ *)data == '\0')
		ঘূৰাই;
	string_list_append_nodup(path_list, xstrndup(data, len));
}

স্থির শূন্য run_pager(সংরচনা grep_opt *opt, ধ্রুৱ ৱর্ণ *prefix)
{
	সংরচনা string_list *path_list = opt->output_priv;
	সংরচনা child_process child = CHILD_PROCESS_INIT;
	পূর্ণাঙ্ক i, status;

	বাবে (i = 0; i < path_list->nr; i++)
		strvec_ঠেলো(&child.args, path_list->বস্তুসমূহ[i].string);
	child.dir = prefix;
	child.use_shell = 1;

	status = run_command(&child);
	যদি (status)
		exit(status);
}

স্থির পূর্ণাঙ্ক grep_cache(সংরচনা grep_opt *opt,
		      ধ্রুৱ সংরচনা pathspec *pathspec, পূর্ণাঙ্ক cached);
স্থির পূর্ণাঙ্ক grep_বৃক্ষ(সংরচনা grep_opt *opt, ধ্রুৱ সংরচনা pathspec *pathspec,
		     সংরচনা বৃক্ষ_desc *বৃক্ষ, সংরচনা strbuf *base, পূর্ণাঙ্ক tn_len,
		     পূর্ণাঙ্ক check_attr);

স্থির পূর্ণাঙ্ক grep_submodule(সংরচনা grep_opt *opt,
			  ধ্রুৱ সংরচনা pathspec *pathspec,
			  ধ্রুৱ সংরচনা object_id *oid,
			  ধ্রুৱ ৱর্ণ *filename, ধ্রুৱ ৱর্ণ *path, পূর্ণাঙ্ক cached)
{
	সংরচনা repository *subrepo;
	সংরচনা repository *superproject = opt->repo;
	সংরচনা grep_opt subopt;
	পূর্ণাঙ্ক hit = 0;

	যদি (!is_submodule_active(superproject, path))
		ঘূৰাই 0;

	subrepo = xmalloc(পরিমাণ(*subrepo));
	যদি (repo_submodule_init(subrepo, superproject, path, null_oid(opt->repo->hash_algo))) {
		মুক্ত(subrepo);
		ঘূৰাই 0;
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

	memcpy(&subopt, opt, পরিমাণ(subopt));
	subopt.repo = subrepo;

	যদি (oid) {
		গণনা object_type object_type;
		সংরচনা বৃক্ষ_desc বৃক্ষ;
		শূন্য *data;
		size_t size;
		সংরচনা strbuf base = STRBUF_INIT;

		obj_read_lock();
		object_type = odb_read_object_info(subrepo->objects, oid, রিক্ত);
		obj_read_unlock();
		data = odb_read_object_peeled(subrepo->objects, oid, OBJ_বৃক্ষ, &size, রিক্ত);
		যদি (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(oid));

		strbuf_addstr(&base, filename);
		strbuf_addch(&base, '/');

		init_বৃক্ষ_desc(&বৃক্ষ, oid, data, size);
		hit = grep_বৃক্ষ(&subopt, pathspec, &বৃক্ষ, &base, base.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&base);
		মুক্ত(data);
	} অন্যথা {
		hit = grep_cache(&subopt, pathspec, cached);
	}

	ঘূৰাই hit;
}

স্থির পূর্ণাঙ্ক grep_cache(সংরচনা grep_opt *opt,
		      ধ্রুৱ সংরচনা pathspec *pathspec, পূর্ণাঙ্ক cached)
{
	সংরচনা repository *repo = opt->repo;
	পূর্ণাঙ্ক hit = 0;
	পূর্ণাঙ্ক nr;
	সংরচনা strbuf name = STRBUF_INIT;
	পূর্ণাঙ্ক name_base_len = 0;
	যদি (repo->submodule_prefix) {
		name_base_len = strlen(repo->submodule_prefix);
		strbuf_addstr(&name, repo->submodule_prefix);
	}

	যদি (repo_read_index(repo) < 0)
		die(_("index file corrupt"));

	বাবে (nr = 0; nr < repo->index->cache_nr; nr++) {
		ধ্রুৱ সংরচনা cache_entry *ce = repo->index->cache[nr];

		যদি (!cached && ce_skip_worktree(ce))
			অনুৱর্ত;

		strbuf_setlen(&name, name_base_len);
		strbuf_addstr(&name, ce->name);
		যদি (S_ISSPARSEDIR(ce->ce_mode)) {
			গণনা object_type type;
			সংরচনা বৃক্ষ_desc বৃক্ষ;
			শূন্য *data;
			size_t size;

			data = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &size);
			যদি (!data)
				die(_("unable to read tree %s"), oid_to_hex(&ce->oid));
			init_বৃক্ষ_desc(&বৃক্ষ, &ce->oid, data, size);

			hit |= grep_বৃক্ষ(opt, pathspec, &বৃক্ষ, &name, 0, 0);
			strbuf_setlen(&name, name_base_len);
			strbuf_addstr(&name, ce->name);
			মুক্ত(data);
		} অন্যথা যদি (S_ISREG(ce->ce_mode) &&
		    match_pathspec(repo->index, pathspec, name.buf, name.len, 0, রিক্ত,
				   S_ISDIR(ce->ce_mode) ||
				   S_ISGITLINK(ce->ce_mode))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			যদি (cached || (ce->ce_flags & CE_VALID)) {
				যদি (ce_stage(ce) || ce_intent_to_add(ce))
					অনুৱর্ত;
				hit |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} অন্যথা {
				hit |= grep_file(opt, name.buf);
			}
		} অন্যথা যদি (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
			   submodule_path_match(repo->index, pathspec, name.buf, রিক্ত)) {
			hit |= grep_submodule(opt, pathspec, রিক্ত, ce->name,
					      ce->name, cached);
		} অন্যথা {
			অনুৱর্ত;
		}

		যদি (ce_stage(ce)) {
			কৰা {
				nr++;
			} যেতিয়ালৈ (nr < repo->index->cache_nr &&
				 !strcmp(ce->name, repo->index->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		যদি (hit && opt->status_only)
			ভঙ্গ;
	}

	strbuf_release(&name);
	ঘূৰাই hit;
}

স্থির পূর্ণাঙ্ক grep_বৃক্ষ(সংরচনা grep_opt *opt, ধ্রুৱ সংরচনা pathspec *pathspec,
		     সংরচনা বৃক্ষ_desc *বৃক্ষ, সংরচনা strbuf *base, পূর্ণাঙ্ক tn_len,
		     পূর্ণাঙ্ক check_attr)
{
	সংরচনা repository *repo = opt->repo;
	পূর্ণাঙ্ক hit = 0;
	গণনা interesting match = entry_not_interesting;
	সংরচনা name_entry entry;
	পূর্ণাঙ্ক old_baselen = base->len;
	সংরচনা strbuf name = STRBUF_INIT;
	পূর্ণাঙ্ক name_base_len = 0;
	যদি (repo->submodule_prefix) {
		strbuf_addstr(&name, repo->submodule_prefix);
		name_base_len = name.len;
	}

	যেতিয়ালৈ (বৃক্ষ_entry(বৃক্ষ, &entry)) {
		পূর্ণাঙ্ক te_len = বৃক্ষ_entry_len(&entry);

		যদি (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = বৃক্ষ_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_setlen(&name, name_base_len);

			যদি (match == all_entries_not_interesting)
				ভঙ্গ;
			যদি (match == entry_not_interesting)
				অনুৱর্ত;
		}

		strbuf_add(base, entry.path, te_len);

		যদি (S_ISREG(entry.mode)) {
			hit |= grep_oid(opt, &entry.oid, base->buf, tn_len,
					 check_attr ? base->buf + tn_len : রিক্ত);
		} অন্যথা যদি (S_ISDIR(entry.mode)) {
			গণনা object_type type;
			সংরচনা বৃক্ষ_desc sub;
			শূন্য *data;
			size_t size;

			data = odb_read_object(the_repository->objects,
					       &entry.oid, &type, &size);
			যদি (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_বৃক্ষ_desc(&sub, &entry.oid, data, size);
			hit |= grep_বৃক্ষ(opt, pathspec, &sub, base, tn_len,
					 check_attr);
			মুক্ত(data);
		} অন্যথা যদি (recurse_submodules && S_ISGITLINK(entry.mode)) {
			hit |= grep_submodule(opt, pathspec, &entry.oid,
					      base->buf, base->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(base, old_baselen);

		যদি (hit && opt->status_only)
			ভঙ্গ;
	}

	strbuf_release(&name);
	ঘূৰাই hit;
}

স্থির শূন্য collect_blob_oids_for_বৃক্ষ(সংরচনা repository *repo,
				       ধ্রুৱ সংরচনা pathspec *pathspec,
				       সংরচনা বৃক্ষ_desc *বৃক্ষ,
				       সংরচনা strbuf *base,
				       পূর্ণাঙ্ক tn_len,
				       সংরচনা oidset *blob_oids)
{
	সংরচনা name_entry entry;
	পূর্ণাঙ্ক old_baselen = base->len;
	সংরচনা strbuf name = STRBUF_INIT;
	গণনা interesting match = entry_not_interesting;

	যেতিয়ালৈ (বৃক্ষ_entry(বৃক্ষ, &entry)) {
		যদি (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = বৃক্ষ_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_reset(&name);

			যদি (match == all_entries_not_interesting)
				ভঙ্গ;
			যদি (match == entry_not_interesting)
				অনুৱর্ত;
		}

		strbuf_add(base, entry.path, বৃক্ষ_entry_len(&entry));

		যদি (S_ISREG(entry.mode)) {
			যদি (!odb_has_object(repo->objects, &entry.oid, 0))
				oidset_ঢোকাও(blob_oids, &entry.oid);
		} অন্যথা যদি (S_ISDIR(entry.mode)) {
			গণনা object_type type;
			সংরচনা বৃক্ষ_desc sub_বৃক্ষ;
			শূন্য *data;
			size_t size;

			data = odb_read_object(repo->objects, &entry.oid,
					       &type, &size);
			যদি (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_বৃক্ষ_desc(&sub_বৃক্ষ, &entry.oid, data, size);
			collect_blob_oids_for_বৃক্ষ(repo, pathspec, &sub_বৃক্ষ,
						   base, tn_len, blob_oids);
			মুক্ত(data);
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

স্থির শূন্য collect_blob_oids_for_treeish(সংরচনা grep_opt *opt,
					  ধ্রুৱ সংরচনা pathspec *pathspec,
					  ধ্রুৱ সংরচনা object_id *বৃক্ষ_ish_oid,
					  ধ্রুৱ ৱর্ণ *name,
					  সংরচনা oidset *blob_oids)
{
	সংরচনা বৃক্ষ_desc বৃক্ষ;
	শূন্য *data;
	size_t size;
	সংরচনা strbuf base = STRBUF_INIT;
	পূর্ণাঙ্ক len;

	data = odb_read_object_peeled(opt->repo->objects, বৃক্ষ_ish_oid,
				      OBJ_বৃক্ষ, &size, রিক্ত);

	যদি (!data)
		ঘূৰাই;

	len = name ? strlen(name) : 0;
	যদি (len) {
		strbuf_add(&base, name, len);
		strbuf_addch(&base, ':');
	}
	init_বৃক্ষ_desc(&বৃক্ষ, বৃক্ষ_ish_oid, data, size);

	collect_blob_oids_for_বৃক্ষ(opt->repo, pathspec, &বৃক্ষ,
				   &base, base.len, blob_oids);

	strbuf_release(&base);
	মুক্ত(data);
}

স্থির শূন্য prefetch_grep_blobs(সংরচনা grep_opt *opt,
				ধ্রুৱ সংরচনা pathspec *pathspec,
				ধ্রুৱ সংরচনা object_array *list)
{
	সংরচনা oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	যদি (!repo_has_promisor_remote(opt->repo))
		ঘূৰাই;

	/* For each tree, gather the blobs in it */
	বাবে (পূর্ণাঙ্ক i = 0; i < list->nr; i++) {
		সংরচনা object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     রিক্ত, 0);
		obj_read_unlock();

		যদি (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_বৃক্ষ))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	যদি (oidset_size(&blob_oids)) {
		সংরচনা oid_array to_fetch = OID_ARRAY_INIT;
		সংরচনা oidset_iter iter;
		ধ্রুৱ সংরচনা object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		যেতিয়ালৈ ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(opt->repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}
	oidset_clear(&blob_oids);
}

স্থির পূর্ণাঙ্ক grep_object(সংরচনা grep_opt *opt, ধ্রুৱ সংরচনা pathspec *pathspec,
		       সংরচনা object *obj, ধ্রুৱ ৱর্ণ *name, ধ্রুৱ ৱর্ণ *path)
{
	যদি (obj->type == OBJ_BLOB)
		ঘূৰাই grep_oid(opt, &obj->oid, name, 0, path);
	যদি (obj->type == OBJ_COMMIT || obj->type == OBJ_বৃক্ষ) {
		সংরচনা বৃক্ষ_desc বৃক্ষ;
		শূন্য *data;
		size_t size;
		সংরচনা strbuf base;
		পূর্ণাঙ্ক hit, len;

		data = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_বৃক্ষ, &size, রিক্ত);
		যদি (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(&obj->oid));

		len = name ? strlen(name) : 0;
		strbuf_init(&base, PATH_MAX + len + 1);
		যদি (len) {
			strbuf_add(&base, name, len);
			strbuf_addch(&base, ':');
		}
		init_বৃক্ষ_desc(&বৃক্ষ, &obj->oid, data, size);
		hit = grep_বৃক্ষ(opt, pathspec, &বৃক্ষ, &base, base.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&base);
		মুক্ত(data);
		ঘূৰাই hit;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

স্থির পূর্ণাঙ্ক grep_objects(সংরচনা grep_opt *opt, ধ্রুৱ সংরচনা pathspec *pathspec,
			ধ্রুৱ সংরচনা object_array *list)
{
	unsigned পূর্ণাঙ্ক i;
	পূর্ণাঙ্ক hit = 0;
	ধ্রুৱ unsigned পূর্ণাঙ্ক nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	বাবে (i = 0; i < nr; i++) {
		সংরচনা object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     রিক্ত, 0);
		obj_read_unlock();

		যদি (!real_obj) {
			ৱর্ণ hex[GIT_MAX_HEXSZ + 1];
			ধ্রুৱ ৱর্ণ *name = list->objects[i].name;

			যদি (!name) {
				oid_to_hex_r(hex, &list->objects[i].item->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		যদি (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_lock();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		যদি (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].path)) {
			hit = 1;
			যদি (opt->status_only)
				ভঙ্গ;
		}
	}
	ঘূৰাই hit;
}

স্থির পূর্ণাঙ্ক grep_directory(সংরচনা grep_opt *opt, ধ্রুৱ সংরচনা pathspec *pathspec,
			  পূর্ণাঙ্ক exc_std, পূর্ণাঙ্ক use_index)
{
	সংরচনা dir_struct dir = DIR_INIT;
	পূর্ণাঙ্ক i, hit = 0;

	যদি (!use_index)
		dir.flags |= DIR_NO_GITLINKS;
	যদি (exc_std)
		setup_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->index, pathspec);
	বাবে (i = 0; i < dir.nr; i++) {
		hit |= grep_file(opt, dir.entries[i]->name);
		যদি (hit && opt->status_only)
			ভঙ্গ;
	}
	dir_clear(&dir);
	ঘূৰাই hit;
}

স্থির পূর্ণাঙ্ক context_callback(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg,
			    পূর্ণাঙ্ক unset)
{
	সংরচনা grep_opt *grep_opt = opt->মান;
	পূর্ণাঙ্ক মান;
	ধ্রুৱ ৱর্ণ *endp;

	যদি (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		ঘূৰাই 0;
	}
	মান = strtol(arg, (ৱর্ণ **)&endp, 10);
	যদি (*endp) {
		ঘূৰাই error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = মান;
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক file_callback(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা grep_opt *grep_opt = opt->মান;
	পূর্ণাঙ্ক from_stdin;
	ধ্রুৱ ৱর্ণ *filename = arg;
	FILE *patterns;
	পূর্ণাঙ্ক lno = 0;
	সংরচনা strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	যদি (!*filename)
		; /* leave it as-is */
	অন্যথা
		filename = prefix_filename_except_for_dash(grep_prefix, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	যদি (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	যেতিয়ালৈ (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		যদি (sb.len == 0)
			অনুৱর্ত;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_PATTERN);
	}
	যদি (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	যদি (filename != arg)
		মুক্ত((শূন্য *)filename);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক not_callback(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা grep_opt *grep_opt = opt->মান;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--not", "command line", 0, GREP_NOT);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক and_callback(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা grep_opt *grep_opt = opt->মান;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--and", "command line", 0, GREP_AND);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক open_callback(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা grep_opt *grep_opt = opt->মান;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক close_callback(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা grep_opt *grep_opt = opt->মান;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক pattern_callback(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg,
			    পূর্ণাঙ্ক unset)
{
	সংরচনা grep_opt *grep_opt = opt->মান;
	BUG_ON_OPT_NEG(unset);
	append_grep_pattern(grep_opt, arg, "-e option", 0, GREP_PATTERN);
	ঘূৰাই 0;
}

পূর্ণাঙ্ক cmd_grep(পূর্ণাঙ্ক argc,
	     ধ্রুৱ ৱর্ণ **argv,
	     ধ্রুৱ ৱর্ণ *prefix,
	     সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক hit = 0;
	পূর্ণাঙ্ক cached = 0, untracked = 0, opt_exclude = -1;
	পূর্ণাঙ্ক seen_dashdash = 0;
	পূর্ণাঙ্ক external_grep_allowed__ignored;
	ধ্রুৱ ৱর্ণ *show_in_pager = রিক্ত, *default_pager = "dummy";
	সংরচনা grep_opt opt;
	সংরচনা object_array list = OBJECT_ARRAY_INIT;
	সংরচনা pathspec pathspec;
	সংরচনা string_list path_list = STRING_LIST_INIT_DUP;
	পূর্ণাঙ্ক i;
	পূর্ণাঙ্ক dummy;
	পূর্ণাঙ্ক use_index = 1;
	পূর্ণাঙ্ক allow_revs;
	পূর্ণাঙ্ক ret;

	সংরচনা option options[] = {
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
		OPT_SET_INT('a', "text", &opt.দ্বিমিক,
			N_("process binary files as text"), GREP_দ্বিমিক_TEXT),
		OPT_SET_INT('I', রিক্ত, &opt.দ্বিমিক,
			N_("don't match patterns in binary files"),
			GREP_দ্বিমিক_NOMATCH),
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
		OPT_NEGBIT('h', রিক্ত, &opt.pathname, N_("don't show filenames"), 1),
		OPT_BIT('H', রিক্ত, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.গুনতি,
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
		OPT_INTEGER(0, "threads", &অঙ্ক_threads,
			N_("use <n> worker threads")),
		OPT_সংখ্যা_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_GROUP(""),
		OPT_CALLBACK('f', রিক্ত, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', রিক্ত, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, pattern_callback),
		OPT_CALLBACK_F(0, "and", &opt, রিক্ত,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, রিক্ত, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', রিক্ত, &opt, রিক্ত, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', রিক্ত, &opt, রিক্ত, "",
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
			.মান = &show_in_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_গুনতি,
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

	যদি (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	যদি (use_index && !startup_info->have_repository) {
		পূর্ণাঙ্ক fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		যদি (fallback)
			use_index = 0;
		অন্যথা
			/* die the same way as if we did it at the beginning */
			setup_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	যদি (!use_index)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	যদি (argc > 0 && !opt.pattern_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	যদি (argc > 0 && !opt.pattern_list) {
		append_grep_pattern(&opt, argv[0], "command line", 0,
				    GREP_PATTERN);
		argv++;
		argc--;
	}

	যদি (show_in_pager == default_pager)
		show_in_pager = git_pager(the_repository, 1);
	যদি (show_in_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &path_list;
		opt.output = append_path;
		string_list_append(&path_list, show_in_pager);
	}

	যদি (!opt.pattern_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	যদি (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	বাবে (i = 0; i < argc; i++) {
		যদি (!strcmp(argv[i], "--")) {
			seen_dashdash = 1;
			ভঙ্গ;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_index && !untracked;
	বাবে (i = 0; i < argc; i++) {
		ধ্রুৱ ৱর্ণ *arg = argv[i];
		সংরচনা object_id oid;
		সংরচনা object_context oc = {0};
		সংরচনা object *object;

		যদি (!strcmp(arg, "--")) {
			i++;
			ভঙ্গ;
		}

		যদি (!allow_revs) {
			যদি (seen_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			ভঙ্গ;
		}

		যদি (get_oid_with_context(the_repository, arg,
					 GET_OID_RECORD_PATH,
					 &oid, &oc)) {
			যদি (seen_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			ভঙ্গ;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		যদি (!seen_dashdash)
			verify_non_filename(the_repository, prefix, arg);
		add_object_array_with_path(object, arg, &list, oc.mode, oc.path);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	যদি (!seen_dashdash) {
		পূর্ণাঙ্ক j;
		বাবে (j = i; j < argc; j++)
			verify_filename(the_repository, prefix, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_depth != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       prefix, argv + i);
	pathspec.max_depth = opt.max_depth;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	যদি (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	যদি (opt.max_গুনতি == 0) {
		ret = 1;
		গচ্ছ out;
	}

	যদি (show_in_pager) {
		যদি (অঙ্ক_threads > 1)
			warning(_("invalid option combination, ignoring --threads"));
		অঙ্ক_threads = 1;
	} অন্যথা যদি (!HAVE_THREADS && অঙ্ক_threads > 1) {
		warning(_("no threads support, ignoring --threads"));
		অঙ্ক_threads = 1;
	} অন্যথা যদি (অঙ্ক_threads < 0)
		die(_("invalid number of threads specified (%d)"), অঙ্ক_threads);
	অন্যথা যদি (অঙ্ক_threads == 0)
		অঙ্ক_threads = HAVE_THREADS ? online_cpus() : 1;

	যদি (অঙ্ক_threads > 1) {
		যদি (!HAVE_THREADS)
			BUG("Somebody got num_threads calculation wrong!");
		যদি (!(opt.name_only || opt.unmatch_name_only || opt.গুনতি)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_first_সাৰি = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		যদি (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		যদি (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		start_threads(&opt);
	} অন্যথা {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	যদি (show_in_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	যদি (show_in_pager && opt.pattern_list && !opt.pattern_list->next) {
		ধ্রুৱ ৱর্ণ *pager = path_list.বস্তুসমূহ[0].string;
		পূর্ণাঙ্ক len = strlen(pager);

		যদি (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		যদি (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&path_list, "-I");

		যদি (!strcmp("less", pager) || !strcmp("vi", pager)) {
			সংরচনা strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.pattern_list->pattern);
			string_list_append_nodup(&path_list,
						 strbuf_detach(&buf, রিক্ত));
		}
	}

	যদি (!show_in_pager && !opt.status_only)
		setup_pager(the_repository);

	die_for_incompatible_opt3(!use_index, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	যদি (!use_index || untracked) {
		পূর্ণাঙ্ক use_exclude = (opt_exclude < 0) ? use_index : !!opt_exclude;
		hit = grep_directory(&opt, &pathspec, use_exclude, use_index);
	} অন্যথা যদি (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} অন্যথা যদি (!list.nr) {
		যদি (!cached)
			setup_work_বৃক্ষ(the_repository);

		hit = grep_cache(&opt, &pathspec, cached);
	} অন্যথা {
		যদি (cached)
			die(_("both --cached and trees are given"));

		hit = grep_objects(&opt, &pathspec, &list);
	}

	যদি (অঙ্ক_threads > 1)
		hit |= wait_all();
	যদি (hit && show_in_pager)
		run_pager(&opt, prefix);

	ret = !hit;

out:
	clear_pathspec(&pathspec);
	string_list_clear(&path_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	ঘূৰাই ret;
}
