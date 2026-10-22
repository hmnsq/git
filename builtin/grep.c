/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "tag.h"
#অন্তর্ভাব "tree-walk.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "grep.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "pathspec.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "submodule.h"
#অন্তর্ভাব "submodule-config.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "odb/source.h"
#অন্তর্ভাব "oid-array.h"
#অন্তর্ভাব "oidset.h"
#অন্তর্ভাব "pager.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "promisor-remote.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "write-or-die.h"

স্থির ধ্রুবক অক্ষর *grep_prefix;

স্থির অক্ষর ধ্রুবক * ধ্রুবক grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	ফাঁকা
};

স্থির পূর্ণসংখ্যা recurse_submodules;

স্থির পূর্ণসংখ্যা অঙ্ক_threads;

স্থির pthread_t *threads;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
কাঠামো work_item {
	কাঠামো grep_source source;
	অক্ষর done;
	কাঠামো strbuf out;
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
#define TODO_আকার 128
স্থির কাঠামো work_item todo[TODO_আকার];
স্থির পূর্ণসংখ্যা todo_start;
স্থির পূর্ণসংখ্যা todo_end;
স্থির পূর্ণসংখ্যা todo_done;

/* Has all work items been added? */
স্থির পূর্ণসংখ্যা all_work_added;

স্থির কাঠামো repository **repos_to_free;
স্থির আকার_t repos_to_free_nr, repos_to_free_alloc;

/* This lock protects all the variables above. */
স্থির pthread_mutex_t grep_mutex;

স্থির অন্তঃরেখ শূন্য grep_lock(শূন্য)
{
	pthread_mutex_lock(&grep_mutex);
}

স্থির অন্তঃরেখ শূন্য grep_unlock(শূন্য)
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
স্থির pthread_cond_t cond_ফলাফল;

স্থির পূর্ণসংখ্যা skip_first_সারি;

স্থির শূন্য add_work(কাঠামো grep_opt *opt, কাঠামো grep_source *gs)
{
	যদি (opt->দ্বিমিক != GREP_দ্বিমিক_TEXT)
		grep_source_load_driver(gs, opt->repo->সূচক);

	grep_lock();

	যখন ((todo_end+1) % ARRAY_আকার(todo) == todo_done) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_end].source = *gs;
	todo[todo_end].done = 0;
	strbuf_reset(&todo[todo_end].out);
	todo_end = (todo_end + 1) % ARRAY_আকার(todo);

	pthread_cond_signal(&cond_add);
	grep_unlock();
}

স্থির কাঠামো work_item *get_work(শূন্য)
{
	কাঠামো work_item *ret;

	grep_lock();
	যখন (todo_start == todo_end && !all_work_added) {
		pthread_cond_wait(&cond_add, &grep_mutex);
	}

	যদি (todo_start == todo_end && all_work_added) {
		ret = ফাঁকা;
	} অন্যথা {
		ret = &todo[todo_start];
		todo_start = (todo_start + 1) % ARRAY_আকার(todo);
	}
	grep_unlock();
	ফেরত ret;
}

স্থির শূন্য work_done(কাঠামো work_item *w)
{
	পূর্ণসংখ্যা old_done;

	grep_lock();
	w->done = 1;
	old_done = todo_done;
	জন্য(; todo[todo_done].done && todo_done != todo_start;
	    todo_done = (todo_done+1) % ARRAY_আকার(todo)) {
		w = &todo[todo_done];
		যদি (w->out.len) {
			ধ্রুবক অক্ষর *p = w->out.buf;
			আকার_t len = w->out.len;

			/* Skip the leading hunk mark of the first file. */
			যদি (skip_first_সারি) {
				যখন (len) {
					len--;
					যদি (*p++ == '\n')
						বিরতি;
				}
				skip_first_সারি = 0;
			}

			write_or_die(1, p, len);
		}
		grep_source_clear(&w->source);
	}

	যদি (old_done != todo_done)
		pthread_cond_signal(&cond_write);

	যদি (all_work_added && todo_done == todo_end)
		pthread_cond_signal(&cond_ফলাফল);

	grep_unlock();
}

স্থির শূন্য free_repos(শূন্য)
{
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < repos_to_free_nr; i++) {
		repo_clear(repos_to_free[i]);
		মুক্ত(repos_to_free[i]);
	}
	FREE_AND_NULL(repos_to_free);
	repos_to_free_nr = 0;
	repos_to_free_alloc = 0;
}

স্থির শূন্য *run(শূন্য *arg)
{
	পূর্ণসংখ্যা hit = 0;
	কাঠামো grep_opt *opt = arg;

	যখন (1) {
		কাঠামো work_item *w = get_work();
		যদি (!w)
			বিরতি;

		opt->output_priv = w;
		hit |= grep_source(opt, &w->source);
		grep_source_clear_তথ্য(&w->source);
		work_done(w);
	}
	free_grep_patterns(opt);
	মুক্ত(opt);

	ফেরত (শূন্য*) (intptr_t) hit;
}

স্থির শূন্য strbuf_out(কাঠামো grep_opt *opt, ধ্রুবক শূন্য *buf, আকার_t আকার_2)
{
	কাঠামো work_item *w = opt->output_priv;
	strbuf_add(&w->out, buf, আকার_2);
}

স্থির শূন্য start_threads(কাঠামো grep_opt *opt)
{
	পূর্ণসংখ্যা i;

	pthread_mutex_init(&grep_mutex, ফাঁকা);
	pthread_mutex_init(&grep_attr_mutex, ফাঁকা);
	pthread_cond_init(&cond_add, ফাঁকা);
	pthread_cond_init(&cond_write, ফাঁকা);
	pthread_cond_init(&cond_ফলাফল, ফাঁকা);
	grep_use_locks = 1;
	enable_obj_read_lock();

	জন্য (i = 0; i < ARRAY_আকার(todo); i++) {
		strbuf_init(&todo[i].out, 0);
	}

	CALLOC_ARRAY(threads, অঙ্ক_threads);
	জন্য (i = 0; i < অঙ্ক_threads; i++) {
		পূর্ণসংখ্যা err;
		কাঠামো grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_out;
		compile_grep_patterns(o);
		err = pthread_create(&threads[i], ফাঁকা, run, o);

		যদি (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

স্থির পূর্ণসংখ্যা wait_all(শূন্য)
{
	পূর্ণসংখ্যা hit = 0;
	পূর্ণসংখ্যা i;

	যদি (!HAVE_THREADS)
		BUG("Never call this function unless you have started threads");

	grep_lock();
	all_work_added = 1;

	/* Wait until all work is done. */
	যখন (todo_done != todo_end)
		pthread_cond_wait(&cond_ফলাফল, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_broadcast(&cond_add);
	grep_unlock();

	জন্য (i = 0; i < অঙ্ক_threads; i++) {
		শূন্য *h;
		pthread_join(threads[i], &h);
		hit |= (পূর্ণসংখ্যা) (intptr_t) h;
	}

	মুক্ত(threads);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_add);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_ফলাফল);
	grep_use_locks = 0;
	disable_obj_read_lock();

	ফেরত hit;
}

স্থির পূর্ণসংখ্যা grep_cmd_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			   ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	পূর্ণসংখ্যা st = grep_config(var, মান, ctx, cb);

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

	ফেরত st;
}

স্থির শূন্য grep_source_name(কাঠামো grep_opt *opt, ধ্রুবক অক্ষর *filename,
			     পূর্ণসংখ্যা বৃক্ষ_name_len, কাঠামো strbuf *out)
{
	strbuf_reset(out);

	যদি (opt->null_following_name) {
		যদি (opt->relative && grep_prefix) {
			কাঠামো strbuf rel_buf = STRBUF_INIT;
			ধ্রুবক অক্ষর *rel_name =
				relative_path(filename + বৃক্ষ_name_len,
					      grep_prefix, &rel_buf);

			যদি (বৃক্ষ_name_len)
				strbuf_add(out, filename, বৃক্ষ_name_len);

			strbuf_addstr(out, rel_name);
			strbuf_release(&rel_buf);
		} অন্যথা {
			strbuf_addstr(out, filename);
		}
		ফেরত;
	}

	যদি (opt->relative && grep_prefix)
		quote_path(filename + বৃক্ষ_name_len, grep_prefix, out, 0);
	অন্যথা
		quote_c_style(filename + বৃক্ষ_name_len, out, ফাঁকা, 0);

	যদি (বৃক্ষ_name_len)
		strbuf_ঢোকাও(out, 0, filename, বৃক্ষ_name_len);
}

স্থির পূর্ণসংখ্যা grep_oid(কাঠামো grep_opt *opt, ধ্রুবক কাঠামো object_id *oid,
		     ধ্রুবক অক্ষর *filename, পূর্ণসংখ্যা বৃক্ষ_name_len,
		     ধ্রুবক অক্ষর *path)
{
	কাঠামো strbuf pathbuf = STRBUF_INIT;
	কাঠামো grep_source gs;

	grep_source_name(opt, filename, বৃক্ষ_name_len, &pathbuf);
	grep_source_init_oid(&gs, pathbuf.buf, path, oid, opt->repo);
	strbuf_release(&pathbuf);

	যদি (অঙ্ক_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		ফেরত 0;
	} অন্যথা {
		পূর্ণসংখ্যা hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		ফেরত hit;
	}
}

স্থির পূর্ণসংখ্যা grep_file(কাঠামো grep_opt *opt, ধ্রুবক অক্ষর *filename)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো grep_source gs;

	grep_source_name(opt, filename, 0, &buf);
	grep_source_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	যদি (অঙ্ক_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		ফেরত 0;
	} অন্যথা {
		পূর্ণসংখ্যা hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		ফেরত hit;
	}
}

স্থির শূন্য append_path(কাঠামো grep_opt *opt, ধ্রুবক শূন্য *তথ্য, আকার_t len)
{
	কাঠামো string_list *path_list = opt->output_priv;

	যদি (len == 1 && *(ধ্রুবক অক্ষর *)তথ্য == '\0')
		ফেরত;
	string_list_append_nodup(path_list, xstrndup(তথ্য, len));
}

স্থির শূন্য run_pager(কাঠামো grep_opt *opt, ধ্রুবক অক্ষর *prefix)
{
	কাঠামো string_list *path_list = opt->output_priv;
	কাঠামো child_process child = CHILD_PROCESS_INIT;
	পূর্ণসংখ্যা i, status;

	জন্য (i = 0; i < path_list->nr; i++)
		strvec_ঠেলো(&child.args, path_list->বস্তুসমূহ[i].string);
	child.dir = prefix;
	child.use_shell = 1;

	status = run_command(&child);
	যদি (status)
		exit(status);
}

স্থির পূর্ণসংখ্যা grep_cache(কাঠামো grep_opt *opt,
		      ধ্রুবক কাঠামো pathspec *pathspec, পূর্ণসংখ্যা cached);
স্থির পূর্ণসংখ্যা grep_বৃক্ষ(কাঠামো grep_opt *opt, ধ্রুবক কাঠামো pathspec *pathspec,
		     কাঠামো বৃক্ষ_desc *বৃক্ষ, কাঠামো strbuf *base, পূর্ণসংখ্যা tn_len,
		     পূর্ণসংখ্যা check_attr);

স্থির পূর্ণসংখ্যা grep_submodule(কাঠামো grep_opt *opt,
			  ধ্রুবক কাঠামো pathspec *pathspec,
			  ধ্রুবক কাঠামো object_id *oid,
			  ধ্রুবক অক্ষর *filename, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা cached)
{
	কাঠামো repository *subrepo;
	কাঠামো repository *superproject = opt->repo;
	কাঠামো grep_opt subopt;
	পূর্ণসংখ্যা hit = 0;

	যদি (!is_submodule_active(superproject, path))
		ফেরত 0;

	subrepo = xmalloc(আকার(*subrepo));
	যদি (repo_submodule_init(subrepo, superproject, path, null_oid(opt->repo->hash_algo))) {
		মুক্ত(subrepo);
		ফেরত 0;
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

	memcpy(&subopt, opt, আকার(subopt));
	subopt.repo = subrepo;

	যদি (oid) {
		গণনা object_type object_type;
		কাঠামো বৃক্ষ_desc বৃক্ষ;
		শূন্য *তথ্য;
		আকার_t আকার_2;
		কাঠামো strbuf base = STRBUF_INIT;

		obj_read_lock();
		object_type = odb_read_object_info(subrepo->objects, oid, ফাঁকা);
		obj_read_unlock();
		তথ্য = odb_read_object_peeled(subrepo->objects, oid, OBJ_বৃক্ষ, &আকার_2, ফাঁকা);
		যদি (!তথ্য)
			die(_("unable to read tree (%s)"), oid_to_hex(oid));

		strbuf_addstr(&base, filename);
		strbuf_addch(&base, '/');

		init_বৃক্ষ_desc(&বৃক্ষ, oid, তথ্য, আকার_2);
		hit = grep_বৃক্ষ(&subopt, pathspec, &বৃক্ষ, &base, base.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&base);
		মুক্ত(তথ্য);
	} অন্যথা {
		hit = grep_cache(&subopt, pathspec, cached);
	}

	ফেরত hit;
}

স্থির পূর্ণসংখ্যা grep_cache(কাঠামো grep_opt *opt,
		      ধ্রুবক কাঠামো pathspec *pathspec, পূর্ণসংখ্যা cached)
{
	কাঠামো repository *repo = opt->repo;
	পূর্ণসংখ্যা hit = 0;
	পূর্ণসংখ্যা nr;
	কাঠামো strbuf name = STRBUF_INIT;
	পূর্ণসংখ্যা name_base_len = 0;
	যদি (repo->submodule_prefix) {
		name_base_len = strlen(repo->submodule_prefix);
		strbuf_addstr(&name, repo->submodule_prefix);
	}

	যদি (repo_read_সূচক(repo) < 0)
		die(_("index file corrupt"));

	জন্য (nr = 0; nr < repo->সূচক->cache_nr; nr++) {
		ধ্রুবক কাঠামো cache_entry *ce = repo->সূচক->cache[nr];

		যদি (!cached && ce_skip_worktree(ce))
			চালু;

		strbuf_setlen(&name, name_base_len);
		strbuf_addstr(&name, ce->name);
		যদি (S_ISSPARSEDIR(ce->ce_mode)) {
			গণনা object_type type;
			কাঠামো বৃক্ষ_desc বৃক্ষ;
			শূন্য *তথ্য;
			আকার_t আকার_2;

			তথ্য = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &আকার_2);
			যদি (!তথ্য)
				die(_("unable to read tree %s"), oid_to_hex(&ce->oid));
			init_বৃক্ষ_desc(&বৃক্ষ, &ce->oid, তথ্য, আকার_2);

			hit |= grep_বৃক্ষ(opt, pathspec, &বৃক্ষ, &name, 0, 0);
			strbuf_setlen(&name, name_base_len);
			strbuf_addstr(&name, ce->name);
			মুক্ত(তথ্য);
		} অন্যথা যদি (S_ISREG(ce->ce_mode) &&
		    match_pathspec(repo->সূচক, pathspec, name.buf, name.len, 0, ফাঁকা,
				   S_ISDIR(ce->ce_mode) ||
				   S_ISGITLINK(ce->ce_mode))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			যদি (cached || (ce->ce_flags & CE_VALID)) {
				যদি (ce_stage(ce) || ce_intent_to_add(ce))
					চালু;
				hit |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} অন্যথা {
				hit |= grep_file(opt, name.buf);
			}
		} অন্যথা যদি (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
			   submodule_path_match(repo->সূচক, pathspec, name.buf, ফাঁকা)) {
			hit |= grep_submodule(opt, pathspec, ফাঁকা, ce->name,
					      ce->name, cached);
		} অন্যথা {
			চালু;
		}

		যদি (ce_stage(ce)) {
			করো {
				nr++;
			} যখন (nr < repo->সূচক->cache_nr &&
				 !strcmp(ce->name, repo->সূচক->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		যদি (hit && opt->status_only)
			বিরতি;
	}

	strbuf_release(&name);
	ফেরত hit;
}

স্থির পূর্ণসংখ্যা grep_বৃক্ষ(কাঠামো grep_opt *opt, ধ্রুবক কাঠামো pathspec *pathspec,
		     কাঠামো বৃক্ষ_desc *বৃক্ষ, কাঠামো strbuf *base, পূর্ণসংখ্যা tn_len,
		     পূর্ণসংখ্যা check_attr)
{
	কাঠামো repository *repo = opt->repo;
	পূর্ণসংখ্যা hit = 0;
	গণনা interesting match = entry_not_interesting;
	কাঠামো name_entry entry;
	পূর্ণসংখ্যা old_baselen = base->len;
	কাঠামো strbuf name = STRBUF_INIT;
	পূর্ণসংখ্যা name_base_len = 0;
	যদি (repo->submodule_prefix) {
		strbuf_addstr(&name, repo->submodule_prefix);
		name_base_len = name.len;
	}

	যখন (বৃক্ষ_entry(বৃক্ষ, &entry)) {
		পূর্ণসংখ্যা te_len = বৃক্ষ_entry_len(&entry);

		যদি (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = বৃক্ষ_entry_interesting(repo->সূচক,
						       &entry, &name,
						       pathspec);
			strbuf_setlen(&name, name_base_len);

			যদি (match == all_entries_not_interesting)
				বিরতি;
			যদি (match == entry_not_interesting)
				চালু;
		}

		strbuf_add(base, entry.path, te_len);

		যদি (S_ISREG(entry.mode)) {
			hit |= grep_oid(opt, &entry.oid, base->buf, tn_len,
					 check_attr ? base->buf + tn_len : ফাঁকা);
		} অন্যথা যদি (S_ISDIR(entry.mode)) {
			গণনা object_type type;
			কাঠামো বৃক্ষ_desc sub;
			শূন্য *তথ্য;
			আকার_t আকার_2;

			তথ্য = odb_read_object(the_repository->objects,
					       &entry.oid, &type, &আকার_2);
			যদি (!তথ্য)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_বৃক্ষ_desc(&sub, &entry.oid, তথ্য, আকার_2);
			hit |= grep_বৃক্ষ(opt, pathspec, &sub, base, tn_len,
					 check_attr);
			মুক্ত(তথ্য);
		} অন্যথা যদি (recurse_submodules && S_ISGITLINK(entry.mode)) {
			hit |= grep_submodule(opt, pathspec, &entry.oid,
					      base->buf, base->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(base, old_baselen);

		যদি (hit && opt->status_only)
			বিরতি;
	}

	strbuf_release(&name);
	ফেরত hit;
}

স্থির শূন্য collect_blob_oids_for_বৃক্ষ(কাঠামো repository *repo,
				       ধ্রুবক কাঠামো pathspec *pathspec,
				       কাঠামো বৃক্ষ_desc *বৃক্ষ,
				       কাঠামো strbuf *base,
				       পূর্ণসংখ্যা tn_len,
				       কাঠামো oidset *blob_oids)
{
	কাঠামো name_entry entry;
	পূর্ণসংখ্যা old_baselen = base->len;
	কাঠামো strbuf name = STRBUF_INIT;
	গণনা interesting match = entry_not_interesting;

	যখন (বৃক্ষ_entry(বৃক্ষ, &entry)) {
		যদি (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = বৃক্ষ_entry_interesting(repo->সূচক,
						       &entry, &name,
						       pathspec);
			strbuf_reset(&name);

			যদি (match == all_entries_not_interesting)
				বিরতি;
			যদি (match == entry_not_interesting)
				চালু;
		}

		strbuf_add(base, entry.path, বৃক্ষ_entry_len(&entry));

		যদি (S_ISREG(entry.mode)) {
			যদি (!odb_has_object(repo->objects, &entry.oid, 0))
				oidset_ঢোকাও(blob_oids, &entry.oid);
		} অন্যথা যদি (S_ISDIR(entry.mode)) {
			গণনা object_type type;
			কাঠামো বৃক্ষ_desc sub_বৃক্ষ;
			শূন্য *তথ্য;
			আকার_t আকার_2;

			তথ্য = odb_read_object(repo->objects, &entry.oid,
					       &type, &আকার_2);
			যদি (!তথ্য)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_বৃক্ষ_desc(&sub_বৃক্ষ, &entry.oid, তথ্য, আকার_2);
			collect_blob_oids_for_বৃক্ষ(repo, pathspec, &sub_বৃক্ষ,
						   base, tn_len, blob_oids);
			মুক্ত(তথ্য);
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

স্থির শূন্য collect_blob_oids_for_treeish(কাঠামো grep_opt *opt,
					  ধ্রুবক কাঠামো pathspec *pathspec,
					  ধ্রুবক কাঠামো object_id *বৃক্ষ_ish_oid,
					  ধ্রুবক অক্ষর *name,
					  কাঠামো oidset *blob_oids)
{
	কাঠামো বৃক্ষ_desc বৃক্ষ;
	শূন্য *তথ্য;
	আকার_t আকার_2;
	কাঠামো strbuf base = STRBUF_INIT;
	পূর্ণসংখ্যা len;

	তথ্য = odb_read_object_peeled(opt->repo->objects, বৃক্ষ_ish_oid,
				      OBJ_বৃক্ষ, &আকার_2, ফাঁকা);

	যদি (!তথ্য)
		ফেরত;

	len = name ? strlen(name) : 0;
	যদি (len) {
		strbuf_add(&base, name, len);
		strbuf_addch(&base, ':');
	}
	init_বৃক্ষ_desc(&বৃক্ষ, বৃক্ষ_ish_oid, তথ্য, আকার_2);

	collect_blob_oids_for_বৃক্ষ(opt->repo, pathspec, &বৃক্ষ,
				   &base, base.len, blob_oids);

	strbuf_release(&base);
	মুক্ত(তথ্য);
}

স্থির শূন্য prefetch_grep_blobs(কাঠামো grep_opt *opt,
				ধ্রুবক কাঠামো pathspec *pathspec,
				ধ্রুবক কাঠামো object_array *list)
{
	কাঠামো oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	যদি (!repo_has_promisor_remote(opt->repo))
		ফেরত;

	/* For each tree, gather the blobs in it */
	জন্য (পূর্ণসংখ্যা i = 0; i < list->nr; i++) {
		কাঠামো object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     ফাঁকা, 0);
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
	যদি (oidset_আকার(&blob_oids)) {
		কাঠামো oid_array to_fetch = OID_ARRAY_INIT;
		কাঠামো oidset_iter iter;
		ধ্রুবক কাঠামো object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		যখন ((oid = oidset_iter_পরবর্তী(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(opt->repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}
	oidset_clear(&blob_oids);
}

স্থির পূর্ণসংখ্যা grep_object(কাঠামো grep_opt *opt, ধ্রুবক কাঠামো pathspec *pathspec,
		       কাঠামো object *obj, ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *path)
{
	যদি (obj->type == OBJ_BLOB)
		ফেরত grep_oid(opt, &obj->oid, name, 0, path);
	যদি (obj->type == OBJ_COMMIT || obj->type == OBJ_বৃক্ষ) {
		কাঠামো বৃক্ষ_desc বৃক্ষ;
		শূন্য *তথ্য;
		আকার_t আকার_2;
		কাঠামো strbuf base;
		পূর্ণসংখ্যা hit, len;

		তথ্য = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_বৃক্ষ, &আকার_2, ফাঁকা);
		যদি (!তথ্য)
			die(_("unable to read tree (%s)"), oid_to_hex(&obj->oid));

		len = name ? strlen(name) : 0;
		strbuf_init(&base, PATH_MAX + len + 1);
		যদি (len) {
			strbuf_add(&base, name, len);
			strbuf_addch(&base, ':');
		}
		init_বৃক্ষ_desc(&বৃক্ষ, &obj->oid, তথ্য, আকার_2);
		hit = grep_বৃক্ষ(opt, pathspec, &বৃক্ষ, &base, base.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&base);
		মুক্ত(তথ্য);
		ফেরত hit;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

স্থির পূর্ণসংখ্যা grep_objects(কাঠামো grep_opt *opt, ধ্রুবক কাঠামো pathspec *pathspec,
			ধ্রুবক কাঠামো object_array *list)
{
	unsigned পূর্ণসংখ্যা i;
	পূর্ণসংখ্যা hit = 0;
	ধ্রুবক unsigned পূর্ণসংখ্যা nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	জন্য (i = 0; i < nr; i++) {
		কাঠামো object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     ফাঁকা, 0);
		obj_read_unlock();

		যদি (!real_obj) {
			অক্ষর hex[GIT_MAX_HEXSZ + 1];
			ধ্রুবক অক্ষর *name = list->objects[i].name;

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
				বিরতি;
		}
	}
	ফেরত hit;
}

স্থির পূর্ণসংখ্যা grep_directory(কাঠামো grep_opt *opt, ধ্রুবক কাঠামো pathspec *pathspec,
			  পূর্ণসংখ্যা exc_std, পূর্ণসংখ্যা use_সূচক)
{
	কাঠামো dir_struct dir = DIR_INIT;
	পূর্ণসংখ্যা i, hit = 0;

	যদি (!use_সূচক)
		dir.flags |= DIR_NO_GITLINKS;
	যদি (exc_std)
		setup_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->সূচক, pathspec);
	জন্য (i = 0; i < dir.nr; i++) {
		hit |= grep_file(opt, dir.entries[i]->name);
		যদি (hit && opt->status_only)
			বিরতি;
	}
	dir_clear(&dir);
	ফেরত hit;
}

স্থির পূর্ণসংখ্যা context_callback(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
			    পূর্ণসংখ্যা unset)
{
	কাঠামো grep_opt *grep_opt = opt->মান;
	পূর্ণসংখ্যা মান;
	ধ্রুবক অক্ষর *endp;

	যদি (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		ফেরত 0;
	}
	মান = strtol(arg, (অক্ষর **)&endp, 10);
	যদি (*endp) {
		ফেরত error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = মান;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা file_callback(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো grep_opt *grep_opt = opt->মান;
	পূর্ণসংখ্যা from_stdin;
	ধ্রুবক অক্ষর *filename = arg;
	FILE *patterns;
	পূর্ণসংখ্যা lno = 0;
	কাঠামো strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	যদি (!*filename)
		; /* leave it as-is */
	অন্যথা
		filename = prefix_filename_except_for_dash(grep_prefix, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	যদি (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	যখন (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		যদি (sb.len == 0)
			চালু;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_PATTERN);
	}
	যদি (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	যদি (filename != arg)
		মুক্ত((শূন্য *)filename);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা not_callback(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো grep_opt *grep_opt = opt->মান;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--not", "command line", 0, GREP_NOT);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা and_callback(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো grep_opt *grep_opt = opt->মান;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--and", "command line", 0, GREP_AND);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা open_callback(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো grep_opt *grep_opt = opt->মান;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা close_callback(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো grep_opt *grep_opt = opt->মান;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা pattern_callback(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
			    পূর্ণসংখ্যা unset)
{
	কাঠামো grep_opt *grep_opt = opt->মান;
	BUG_ON_OPT_NEG(unset);
	append_grep_pattern(grep_opt, arg, "-e option", 0, GREP_PATTERN);
	ফেরত 0;
}

পূর্ণসংখ্যা cmd_grep(পূর্ণসংখ্যা argc,
	     ধ্রুবক অক্ষর **argv,
	     ধ্রুবক অক্ষর *prefix,
	     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা hit = 0;
	পূর্ণসংখ্যা cached = 0, untracked = 0, opt_exclude = -1;
	পূর্ণসংখ্যা seen_dashdash = 0;
	পূর্ণসংখ্যা external_grep_allowed__ignored;
	ধ্রুবক অক্ষর *show_in_pager = ফাঁকা, *default_pager = "dummy";
	কাঠামো grep_opt opt;
	কাঠামো object_array list = OBJECT_ARRAY_INIT;
	কাঠামো pathspec pathspec;
	কাঠামো string_list path_list = STRING_LIST_INIT_DUP;
	পূর্ণসংখ্যা i;
	পূর্ণসংখ্যা dummy;
	পূর্ণসংখ্যা use_সূচক = 1;
	পূর্ণসংখ্যা allow_revs;
	পূর্ণসংখ্যা ret;

	কাঠামো option options[] = {
		OPT_BOOL(0, "cached", &cached,
			N_("search in index instead of in the work tree")),
		OPT_NEGBIT(0, "no-index", &use_সূচক,
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
		OPT_SET_INT('I', ফাঁকা, &opt.দ্বিমিক,
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
		OPT_NEGBIT('h', ফাঁকা, &opt.pathname, N_("don't show filenames"), 1),
		OPT_BIT('H', ফাঁকা, &opt.pathname, N_("show filenames"), 1),
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
		OPT_CALLBACK('f', ফাঁকা, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', ফাঁকা, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, pattern_callback),
		OPT_CALLBACK_F(0, "and", &opt, ফাঁকা,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, ফাঁকা, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', ফাঁকা, &opt, ফাঁকা, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', ফাঁকা, &opt, ফাঁকা, "",
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
		the_repository->settings.command_requires_full_সূচক = 0;
	}

	যদি (use_সূচক && !startup_info->have_repository) {
		পূর্ণসংখ্যা fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		যদি (fallback)
			use_সূচক = 0;
		অন্যথা
			/* die the same way as if we did it at the beginning */
			setup_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	যদি (!use_সূচক)
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
	জন্য (i = 0; i < argc; i++) {
		যদি (!strcmp(argv[i], "--")) {
			seen_dashdash = 1;
			বিরতি;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_সূচক && !untracked;
	জন্য (i = 0; i < argc; i++) {
		ধ্রুবক অক্ষর *arg = argv[i];
		কাঠামো object_id oid;
		কাঠামো object_context oc = {0};
		কাঠামো object *object;

		যদি (!strcmp(arg, "--")) {
			i++;
			বিরতি;
		}

		যদি (!allow_revs) {
			যদি (seen_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			বিরতি;
		}

		যদি (get_oid_with_context(the_repository, arg,
					 GET_OID_RECORD_PATH,
					 &oid, &oc)) {
			যদি (seen_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			বিরতি;
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
		পূর্ণসংখ্যা j;
		জন্য (j = i; j < argc; j++)
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
		যাও out;
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
			skip_first_সারি = 1;

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

	যদি (show_in_pager && opt.pattern_list && !opt.pattern_list->পরবর্তী) {
		ধ্রুবক অক্ষর *pager = path_list.বস্তুসমূহ[0].string;
		পূর্ণসংখ্যা len = strlen(pager);

		যদি (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		যদি (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&path_list, "-I");

		যদি (!strcmp("less", pager) || !strcmp("vi", pager)) {
			কাঠামো strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.pattern_list->pattern);
			string_list_append_nodup(&path_list,
						 strbuf_detach(&buf, ফাঁকা));
		}
	}

	যদি (!show_in_pager && !opt.status_only)
		setup_pager(the_repository);

	die_for_incompatible_opt3(!use_সূচক, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	যদি (!use_সূচক || untracked) {
		পূর্ণসংখ্যা use_exclude = (opt_exclude < 0) ? use_সূচক : !!opt_exclude;
		hit = grep_directory(&opt, &pathspec, use_exclude, use_সূচক);
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
	ফেরত ret;
}
