/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "config.h"
#شامل "tag.h"
#شامل "tree-walk.h"
#شامل "parse-options.h"
#شامل "string-list.h"
#شامل "run-command.h"
#شامل "grep.h"
#شامل "quote.h"
#شامل "dir.h"
#شامل "pathspec.h"
#شامل "setup.h"
#شامل "submodule.h"
#شامل "submodule-config.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "odb/source.h"
#شامل "oid-array.h"
#شامل "oidset.h"
#شامل "pager.h"
#شامل "path.h"
#شامل "promisor-remote.h"
#شامل "read-cache-ll.h"
#شامل "write-or-die.h"

ایستا ثابت نویسه *grep_prefix;

ایستا نویسه ثابت * ثابت grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	پوچ
};

ایستا صحیح recurse_submodules;

ایستا صحیح رقم_threads;

ایستا pthread_t *threads;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
ساختار work_item {
	ساختار grep_source source;
	نویسه done;
	ساختار strbuf out;
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
ایستا ساختار work_item todo[TODO_SIZE];
ایستا صحیح todo_start;
ایستا صحیح todo_end;
ایستا صحیح todo_done;

/* Has all work items been added? */
ایستا صحیح all_work_added;

ایستا ساختار repository **repos_to_free;
ایستا size_t repos_to_free_nr, repos_to_free_alloc;

/* This lock protects all the variables above. */
ایستا pthread_mutex_t grep_mutex;

ایستا درونخطی تهی grep_lock(تهی)
{
	pthread_mutex_lock(&grep_mutex);
}

ایستا درونخطی تهی grep_unlock(تهی)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
ایستا pthread_cond_t cond_add;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
ایستا pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
ایستا pthread_cond_t cond_result;

ایستا صحیح skip_first_سطر;

ایستا تهی add_work(ساختار grep_opt *opt, ساختار grep_source *gs)
{
	اگر (opt->دودویی != GREP_دودویی_TEXT)
		grep_source_load_driver(gs, opt->repo->index);

	grep_lock();

	وقتی ((todo_end+1) % ARRAY_SIZE(todo) == todo_done) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_end].source = *gs;
	todo[todo_end].done = 0;
	strbuf_reset(&todo[todo_end].out);
	todo_end = (todo_end + 1) % ARRAY_SIZE(todo);

	pthread_cond_signal(&cond_add);
	grep_unlock();
}

ایستا ساختار work_item *get_work(تهی)
{
	ساختار work_item *ret;

	grep_lock();
	وقتی (todo_start == todo_end && !all_work_added) {
		pthread_cond_wait(&cond_add, &grep_mutex);
	}

	اگر (todo_start == todo_end && all_work_added) {
		ret = پوچ;
	} وگرنه {
		ret = &todo[todo_start];
		todo_start = (todo_start + 1) % ARRAY_SIZE(todo);
	}
	grep_unlock();
	بازگشت ret;
}

ایستا تهی work_done(ساختار work_item *w)
{
	صحیح old_done;

	grep_lock();
	w->done = 1;
	old_done = todo_done;
	برای(; todo[todo_done].done && todo_done != todo_start;
	    todo_done = (todo_done+1) % ARRAY_SIZE(todo)) {
		w = &todo[todo_done];
		اگر (w->out.len) {
			ثابت نویسه *p = w->out.buf;
			size_t len = w->out.len;

			/* Skip the leading hunk mark of the first file. */
			اگر (skip_first_سطر) {
				وقتی (len) {
					len--;
					اگر (*p++ == '\n')
						بشکن;
				}
				skip_first_سطر = 0;
			}

			write_or_die(1, p, len);
		}
		grep_source_clear(&w->source);
	}

	اگر (old_done != todo_done)
		pthread_cond_signal(&cond_write);

	اگر (all_work_added && todo_done == todo_end)
		pthread_cond_signal(&cond_result);

	grep_unlock();
}

ایستا تهی free_repos(تهی)
{
	صحیح i;

	برای (i = 0; i < repos_to_free_nr; i++) {
		repo_clear(repos_to_free[i]);
		آزاد(repos_to_free[i]);
	}
	FREE_AND_NULL(repos_to_free);
	repos_to_free_nr = 0;
	repos_to_free_alloc = 0;
}

ایستا تهی *run(تهی *arg)
{
	صحیح hit = 0;
	ساختار grep_opt *opt = arg;

	وقتی (1) {
		ساختار work_item *w = get_work();
		اگر (!w)
			بشکن;

		opt->output_priv = w;
		hit |= grep_source(opt, &w->source);
		grep_source_clear_data(&w->source);
		work_done(w);
	}
	free_grep_patterns(opt);
	آزاد(opt);

	بازگشت (تهی*) (intptr_t) hit;
}

ایستا تهی strbuf_out(ساختار grep_opt *opt, ثابت تهی *buf, size_t size)
{
	ساختار work_item *w = opt->output_priv;
	strbuf_add(&w->out, buf, size);
}

ایستا تهی start_threads(ساختار grep_opt *opt)
{
	صحیح i;

	pthread_mutex_init(&grep_mutex, پوچ);
	pthread_mutex_init(&grep_attr_mutex, پوچ);
	pthread_cond_init(&cond_add, پوچ);
	pthread_cond_init(&cond_write, پوچ);
	pthread_cond_init(&cond_result, پوچ);
	grep_use_locks = 1;
	enable_obj_read_lock();

	برای (i = 0; i < ARRAY_SIZE(todo); i++) {
		strbuf_init(&todo[i].out, 0);
	}

	CALLOC_ARRAY(threads, رقم_threads);
	برای (i = 0; i < رقم_threads; i++) {
		صحیح err;
		ساختار grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_out;
		compile_grep_patterns(o);
		err = pthread_create(&threads[i], پوچ, run, o);

		اگر (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

ایستا صحیح wait_all(تهی)
{
	صحیح hit = 0;
	صحیح i;

	اگر (!HAVE_THREADS)
		BUG("Never call this function unless you have started threads");

	grep_lock();
	all_work_added = 1;

	/* Wait until all work is done. */
	وقتی (todo_done != todo_end)
		pthread_cond_wait(&cond_result, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_broadcast(&cond_add);
	grep_unlock();

	برای (i = 0; i < رقم_threads; i++) {
		تهی *h;
		pthread_join(threads[i], &h);
		hit |= (صحیح) (intptr_t) h;
	}

	آزاد(threads);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_add);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_result);
	grep_use_locks = 0;
	disable_obj_read_lock();

	بازگشت hit;
}

ایستا صحیح grep_cmd_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			   ثابت ساختار config_context *ctx, تهی *cb)
{
	صحیح st = grep_config(var, مقدار, ctx, cb);

	اگر (git_color_config(var, مقدار, cb) < 0)
		st = -1;
	وگرنه اگر (git_default_config(var, مقدار, ctx, cb) < 0)
		st = -1;

	اگر (!strcmp(var, "grep.threads")) {
		رقم_threads = git_config_int(var, مقدار, ctx->kvi);
		اگر (رقم_threads < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    رقم_threads, var);
		وگرنه اگر (!HAVE_THREADS && رقم_threads > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			رقم_threads = 1;
		}
	}

	اگر (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, مقدار);

	بازگشت st;
}

ایستا تهی grep_source_name(ساختار grep_opt *opt, ثابت نویسه *filename,
			     صحیح درخت_name_len, ساختار strbuf *out)
{
	strbuf_reset(out);

	اگر (opt->null_following_name) {
		اگر (opt->relative && grep_prefix) {
			ساختار strbuf rel_buf = STRBUF_INIT;
			ثابت نویسه *rel_name =
				relative_path(filename + درخت_name_len,
					      grep_prefix, &rel_buf);

			اگر (درخت_name_len)
				strbuf_add(out, filename, درخت_name_len);

			strbuf_addstr(out, rel_name);
			strbuf_release(&rel_buf);
		} وگرنه {
			strbuf_addstr(out, filename);
		}
		بازگشت;
	}

	اگر (opt->relative && grep_prefix)
		quote_path(filename + درخت_name_len, grep_prefix, out, 0);
	وگرنه
		quote_c_style(filename + درخت_name_len, out, پوچ, 0);

	اگر (درخت_name_len)
		strbuf_درج(out, 0, filename, درخت_name_len);
}

ایستا صحیح grep_oid(ساختار grep_opt *opt, ثابت ساختار object_id *oid,
		     ثابت نویسه *filename, صحیح درخت_name_len,
		     ثابت نویسه *path)
{
	ساختار strbuf pathbuf = STRBUF_INIT;
	ساختار grep_source gs;

	grep_source_name(opt, filename, درخت_name_len, &pathbuf);
	grep_source_init_oid(&gs, pathbuf.buf, path, oid, opt->repo);
	strbuf_release(&pathbuf);

	اگر (رقم_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		بازگشت 0;
	} وگرنه {
		صحیح hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		بازگشت hit;
	}
}

ایستا صحیح grep_file(ساختار grep_opt *opt, ثابت نویسه *filename)
{
	ساختار strbuf buf = STRBUF_INIT;
	ساختار grep_source gs;

	grep_source_name(opt, filename, 0, &buf);
	grep_source_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	اگر (رقم_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		بازگشت 0;
	} وگرنه {
		صحیح hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		بازگشت hit;
	}
}

ایستا تهی append_path(ساختار grep_opt *opt, ثابت تهی *data, size_t len)
{
	ساختار string_list *path_list = opt->output_priv;

	اگر (len == 1 && *(ثابت نویسه *)data == '\0')
		بازگشت;
	string_list_append_nodup(path_list, xstrndup(data, len));
}

ایستا تهی run_pager(ساختار grep_opt *opt, ثابت نویسه *prefix)
{
	ساختار string_list *path_list = opt->output_priv;
	ساختار child_process child = CHILD_PROCESS_INIT;
	صحیح i, status;

	برای (i = 0; i < path_list->nr; i++)
		strvec_افزودن(&child.args, path_list->اقلام[i].string);
	child.dir = prefix;
	child.use_shell = 1;

	status = run_command(&child);
	اگر (status)
		exit(status);
}

ایستا صحیح grep_cache(ساختار grep_opt *opt,
		      ثابت ساختار pathspec *pathspec, صحیح cached);
ایستا صحیح grep_درخت(ساختار grep_opt *opt, ثابت ساختار pathspec *pathspec,
		     ساختار درخت_desc *درخت, ساختار strbuf *base, صحیح tn_len,
		     صحیح check_attr);

ایستا صحیح grep_submodule(ساختار grep_opt *opt,
			  ثابت ساختار pathspec *pathspec,
			  ثابت ساختار object_id *oid,
			  ثابت نویسه *filename, ثابت نویسه *path, صحیح cached)
{
	ساختار repository *subrepo;
	ساختار repository *superproject = opt->repo;
	ساختار grep_opt subopt;
	صحیح hit = 0;

	اگر (!is_submodule_active(superproject, path))
		بازگشت 0;

	subrepo = xmalloc(اندازه(*subrepo));
	اگر (repo_submodule_init(subrepo, superproject, path, null_oid(opt->repo->hash_algo))) {
		آزاد(subrepo);
		بازگشت 0;
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

	memcpy(&subopt, opt, اندازه(subopt));
	subopt.repo = subrepo;

	اگر (oid) {
		شمارش object_type object_type;
		ساختار درخت_desc درخت;
		تهی *data;
		size_t size;
		ساختار strbuf base = STRBUF_INIT;

		obj_read_lock();
		object_type = odb_read_object_info(subrepo->objects, oid, پوچ);
		obj_read_unlock();
		data = odb_read_object_peeled(subrepo->objects, oid, OBJ_درخت, &size, پوچ);
		اگر (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(oid));

		strbuf_addstr(&base, filename);
		strbuf_addch(&base, '/');

		init_درخت_desc(&درخت, oid, data, size);
		hit = grep_درخت(&subopt, pathspec, &درخت, &base, base.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&base);
		آزاد(data);
	} وگرنه {
		hit = grep_cache(&subopt, pathspec, cached);
	}

	بازگشت hit;
}

ایستا صحیح grep_cache(ساختار grep_opt *opt,
		      ثابت ساختار pathspec *pathspec, صحیح cached)
{
	ساختار repository *repo = opt->repo;
	صحیح hit = 0;
	صحیح nr;
	ساختار strbuf name = STRBUF_INIT;
	صحیح name_base_len = 0;
	اگر (repo->submodule_prefix) {
		name_base_len = strlen(repo->submodule_prefix);
		strbuf_addstr(&name, repo->submodule_prefix);
	}

	اگر (repo_read_index(repo) < 0)
		die(_("index file corrupt"));

	برای (nr = 0; nr < repo->index->cache_nr; nr++) {
		ثابت ساختار cache_entry *ce = repo->index->cache[nr];

		اگر (!cached && ce_skip_worktree(ce))
			ادامه;

		strbuf_setlen(&name, name_base_len);
		strbuf_addstr(&name, ce->name);
		اگر (S_ISSPARSEDIR(ce->ce_mode)) {
			شمارش object_type type;
			ساختار درخت_desc درخت;
			تهی *data;
			size_t size;

			data = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &size);
			اگر (!data)
				die(_("unable to read tree %s"), oid_to_hex(&ce->oid));
			init_درخت_desc(&درخت, &ce->oid, data, size);

			hit |= grep_درخت(opt, pathspec, &درخت, &name, 0, 0);
			strbuf_setlen(&name, name_base_len);
			strbuf_addstr(&name, ce->name);
			آزاد(data);
		} وگرنه اگر (S_ISREG(ce->ce_mode) &&
		    match_pathspec(repo->index, pathspec, name.buf, name.len, 0, پوچ,
				   S_ISDIR(ce->ce_mode) ||
				   S_ISGITLINK(ce->ce_mode))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			اگر (cached || (ce->ce_flags & CE_VALID)) {
				اگر (ce_stage(ce) || ce_intent_to_add(ce))
					ادامه;
				hit |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} وگرنه {
				hit |= grep_file(opt, name.buf);
			}
		} وگرنه اگر (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
			   submodule_path_match(repo->index, pathspec, name.buf, پوچ)) {
			hit |= grep_submodule(opt, pathspec, پوچ, ce->name,
					      ce->name, cached);
		} وگرنه {
			ادامه;
		}

		اگر (ce_stage(ce)) {
			انجامبده {
				nr++;
			} وقتی (nr < repo->index->cache_nr &&
				 !strcmp(ce->name, repo->index->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		اگر (hit && opt->status_only)
			بشکن;
	}

	strbuf_release(&name);
	بازگشت hit;
}

ایستا صحیح grep_درخت(ساختار grep_opt *opt, ثابت ساختار pathspec *pathspec,
		     ساختار درخت_desc *درخت, ساختار strbuf *base, صحیح tn_len,
		     صحیح check_attr)
{
	ساختار repository *repo = opt->repo;
	صحیح hit = 0;
	شمارش interesting match = entry_not_interesting;
	ساختار name_entry entry;
	صحیح old_baselen = base->len;
	ساختار strbuf name = STRBUF_INIT;
	صحیح name_base_len = 0;
	اگر (repo->submodule_prefix) {
		strbuf_addstr(&name, repo->submodule_prefix);
		name_base_len = name.len;
	}

	وقتی (درخت_entry(درخت, &entry)) {
		صحیح te_len = درخت_entry_len(&entry);

		اگر (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = درخت_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_setlen(&name, name_base_len);

			اگر (match == all_entries_not_interesting)
				بشکن;
			اگر (match == entry_not_interesting)
				ادامه;
		}

		strbuf_add(base, entry.path, te_len);

		اگر (S_ISREG(entry.mode)) {
			hit |= grep_oid(opt, &entry.oid, base->buf, tn_len,
					 check_attr ? base->buf + tn_len : پوچ);
		} وگرنه اگر (S_ISDIR(entry.mode)) {
			شمارش object_type type;
			ساختار درخت_desc sub;
			تهی *data;
			size_t size;

			data = odb_read_object(the_repository->objects,
					       &entry.oid, &type, &size);
			اگر (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_درخت_desc(&sub, &entry.oid, data, size);
			hit |= grep_درخت(opt, pathspec, &sub, base, tn_len,
					 check_attr);
			آزاد(data);
		} وگرنه اگر (recurse_submodules && S_ISGITLINK(entry.mode)) {
			hit |= grep_submodule(opt, pathspec, &entry.oid,
					      base->buf, base->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(base, old_baselen);

		اگر (hit && opt->status_only)
			بشکن;
	}

	strbuf_release(&name);
	بازگشت hit;
}

ایستا تهی collect_blob_oids_for_درخت(ساختار repository *repo,
				       ثابت ساختار pathspec *pathspec,
				       ساختار درخت_desc *درخت,
				       ساختار strbuf *base,
				       صحیح tn_len,
				       ساختار oidset *blob_oids)
{
	ساختار name_entry entry;
	صحیح old_baselen = base->len;
	ساختار strbuf name = STRBUF_INIT;
	شمارش interesting match = entry_not_interesting;

	وقتی (درخت_entry(درخت, &entry)) {
		اگر (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = درخت_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_reset(&name);

			اگر (match == all_entries_not_interesting)
				بشکن;
			اگر (match == entry_not_interesting)
				ادامه;
		}

		strbuf_add(base, entry.path, درخت_entry_len(&entry));

		اگر (S_ISREG(entry.mode)) {
			اگر (!odb_has_object(repo->objects, &entry.oid, 0))
				oidset_درج(blob_oids, &entry.oid);
		} وگرنه اگر (S_ISDIR(entry.mode)) {
			شمارش object_type type;
			ساختار درخت_desc sub_درخت;
			تهی *data;
			size_t size;

			data = odb_read_object(repo->objects, &entry.oid,
					       &type, &size);
			اگر (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_درخت_desc(&sub_درخت, &entry.oid, data, size);
			collect_blob_oids_for_درخت(repo, pathspec, &sub_درخت,
						   base, tn_len, blob_oids);
			آزاد(data);
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

ایستا تهی collect_blob_oids_for_treeish(ساختار grep_opt *opt,
					  ثابت ساختار pathspec *pathspec,
					  ثابت ساختار object_id *درخت_ish_oid,
					  ثابت نویسه *name,
					  ساختار oidset *blob_oids)
{
	ساختار درخت_desc درخت;
	تهی *data;
	size_t size;
	ساختار strbuf base = STRBUF_INIT;
	صحیح len;

	data = odb_read_object_peeled(opt->repo->objects, درخت_ish_oid,
				      OBJ_درخت, &size, پوچ);

	اگر (!data)
		بازگشت;

	len = name ? strlen(name) : 0;
	اگر (len) {
		strbuf_add(&base, name, len);
		strbuf_addch(&base, ':');
	}
	init_درخت_desc(&درخت, درخت_ish_oid, data, size);

	collect_blob_oids_for_درخت(opt->repo, pathspec, &درخت,
				   &base, base.len, blob_oids);

	strbuf_release(&base);
	آزاد(data);
}

ایستا تهی prefetch_grep_blobs(ساختار grep_opt *opt,
				ثابت ساختار pathspec *pathspec,
				ثابت ساختار object_array *list)
{
	ساختار oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	اگر (!repo_has_promisor_remote(opt->repo))
		بازگشت;

	/* For each tree, gather the blobs in it */
	برای (صحیح i = 0; i < list->nr; i++) {
		ساختار object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     پوچ, 0);
		obj_read_unlock();

		اگر (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_درخت))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	اگر (oidset_size(&blob_oids)) {
		ساختار oid_array to_fetch = OID_ARRAY_INIT;
		ساختار oidset_iter iter;
		ثابت ساختار object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		وقتی ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(opt->repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}
	oidset_clear(&blob_oids);
}

ایستا صحیح grep_object(ساختار grep_opt *opt, ثابت ساختار pathspec *pathspec,
		       ساختار object *obj, ثابت نویسه *name, ثابت نویسه *path)
{
	اگر (obj->type == OBJ_BLOB)
		بازگشت grep_oid(opt, &obj->oid, name, 0, path);
	اگر (obj->type == OBJ_COMMIT || obj->type == OBJ_درخت) {
		ساختار درخت_desc درخت;
		تهی *data;
		size_t size;
		ساختار strbuf base;
		صحیح hit, len;

		data = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_درخت, &size, پوچ);
		اگر (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(&obj->oid));

		len = name ? strlen(name) : 0;
		strbuf_init(&base, PATH_MAX + len + 1);
		اگر (len) {
			strbuf_add(&base, name, len);
			strbuf_addch(&base, ':');
		}
		init_درخت_desc(&درخت, &obj->oid, data, size);
		hit = grep_درخت(opt, pathspec, &درخت, &base, base.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&base);
		آزاد(data);
		بازگشت hit;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

ایستا صحیح grep_objects(ساختار grep_opt *opt, ثابت ساختار pathspec *pathspec,
			ثابت ساختار object_array *list)
{
	unsigned صحیح i;
	صحیح hit = 0;
	ثابت unsigned صحیح nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	برای (i = 0; i < nr; i++) {
		ساختار object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     پوچ, 0);
		obj_read_unlock();

		اگر (!real_obj) {
			نویسه hex[GIT_MAX_HEXSZ + 1];
			ثابت نویسه *name = list->objects[i].name;

			اگر (!name) {
				oid_to_hex_r(hex, &list->objects[i].item->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		اگر (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_lock();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		اگر (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].path)) {
			hit = 1;
			اگر (opt->status_only)
				بشکن;
		}
	}
	بازگشت hit;
}

ایستا صحیح grep_directory(ساختار grep_opt *opt, ثابت ساختار pathspec *pathspec,
			  صحیح exc_std, صحیح use_index)
{
	ساختار dir_struct dir = DIR_INIT;
	صحیح i, hit = 0;

	اگر (!use_index)
		dir.flags |= DIR_NO_GITLINKS;
	اگر (exc_std)
		setup_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->index, pathspec);
	برای (i = 0; i < dir.nr; i++) {
		hit |= grep_file(opt, dir.entries[i]->name);
		اگر (hit && opt->status_only)
			بشکن;
	}
	dir_clear(&dir);
	بازگشت hit;
}

ایستا صحیح context_callback(ثابت ساختار option *opt, ثابت نویسه *arg,
			    صحیح unset)
{
	ساختار grep_opt *grep_opt = opt->مقدار;
	صحیح مقدار;
	ثابت نویسه *endp;

	اگر (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		بازگشت 0;
	}
	مقدار = strtol(arg, (نویسه **)&endp, 10);
	اگر (*endp) {
		بازگشت error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = مقدار;
	بازگشت 0;
}

ایستا صحیح file_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار grep_opt *grep_opt = opt->مقدار;
	صحیح from_stdin;
	ثابت نویسه *filename = arg;
	FILE *patterns;
	صحیح lno = 0;
	ساختار strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	اگر (!*filename)
		; /* leave it as-is */
	وگرنه
		filename = prefix_filename_except_for_dash(grep_prefix, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	اگر (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	وقتی (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		اگر (sb.len == 0)
			ادامه;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_PATTERN);
	}
	اگر (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	اگر (filename != arg)
		آزاد((تهی *)filename);
	بازگشت 0;
}

ایستا صحیح not_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار grep_opt *grep_opt = opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--not", "command line", 0, GREP_NOT);
	بازگشت 0;
}

ایستا صحیح and_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار grep_opt *grep_opt = opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--and", "command line", 0, GREP_AND);
	بازگشت 0;
}

ایستا صحیح open_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار grep_opt *grep_opt = opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	بازگشت 0;
}

ایستا صحیح close_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار grep_opt *grep_opt = opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	بازگشت 0;
}

ایستا صحیح pattern_callback(ثابت ساختار option *opt, ثابت نویسه *arg,
			    صحیح unset)
{
	ساختار grep_opt *grep_opt = opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	append_grep_pattern(grep_opt, arg, "-e option", 0, GREP_PATTERN);
	بازگشت 0;
}

صحیح cmd_grep(صحیح argc,
	     ثابت نویسه **argv,
	     ثابت نویسه *prefix,
	     ساختار repository *repo UNUSED)
{
	صحیح hit = 0;
	صحیح cached = 0, untracked = 0, opt_exclude = -1;
	صحیح seen_dashdash = 0;
	صحیح external_grep_allowed__ignored;
	ثابت نویسه *show_in_pager = پوچ, *default_pager = "dummy";
	ساختار grep_opt opt;
	ساختار object_array list = OBJECT_ARRAY_INIT;
	ساختار pathspec pathspec;
	ساختار string_list path_list = STRING_LIST_INIT_DUP;
	صحیح i;
	صحیح dummy;
	صحیح use_index = 1;
	صحیح allow_revs;
	صحیح ret;

	ساختار option options[] = {
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
		OPT_SET_INT('a', "text", &opt.دودویی,
			N_("process binary files as text"), GREP_دودویی_TEXT),
		OPT_SET_INT('I', پوچ, &opt.دودویی,
			N_("don't match patterns in binary files"),
			GREP_دودویی_NOMATCH),
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
		OPT_NEGBIT('h', پوچ, &opt.pathname, N_("don't show filenames"), 1),
		OPT_BIT('H', پوچ, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.شمار,
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
		OPT_INTEGER(0, "threads", &رقم_threads,
			N_("use <n> worker threads")),
		OPT_عدد_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_GROUP(""),
		OPT_CALLBACK('f', پوچ, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', پوچ, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, pattern_callback),
		OPT_CALLBACK_F(0, "and", &opt, پوچ,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, پوچ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', پوچ, &opt, پوچ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', پوچ, &opt, پوچ, "",
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
			.مقدار = &show_in_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_شمار,
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

	اگر (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	اگر (use_index && !startup_info->have_repository) {
		صحیح fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		اگر (fallback)
			use_index = 0;
		وگرنه
			/* die the same way as if we did it at the beginning */
			setup_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	اگر (!use_index)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	اگر (argc > 0 && !opt.pattern_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	اگر (argc > 0 && !opt.pattern_list) {
		append_grep_pattern(&opt, argv[0], "command line", 0,
				    GREP_PATTERN);
		argv++;
		argc--;
	}

	اگر (show_in_pager == default_pager)
		show_in_pager = git_pager(the_repository, 1);
	اگر (show_in_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &path_list;
		opt.output = append_path;
		string_list_append(&path_list, show_in_pager);
	}

	اگر (!opt.pattern_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	اگر (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	برای (i = 0; i < argc; i++) {
		اگر (!strcmp(argv[i], "--")) {
			seen_dashdash = 1;
			بشکن;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_index && !untracked;
	برای (i = 0; i < argc; i++) {
		ثابت نویسه *arg = argv[i];
		ساختار object_id oid;
		ساختار object_context oc = {0};
		ساختار object *object;

		اگر (!strcmp(arg, "--")) {
			i++;
			بشکن;
		}

		اگر (!allow_revs) {
			اگر (seen_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			بشکن;
		}

		اگر (get_oid_with_context(the_repository, arg,
					 GET_OID_RECORD_PATH,
					 &oid, &oc)) {
			اگر (seen_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			بشکن;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		اگر (!seen_dashdash)
			verify_non_filename(the_repository, prefix, arg);
		add_object_array_with_path(object, arg, &list, oc.mode, oc.path);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	اگر (!seen_dashdash) {
		صحیح j;
		برای (j = i; j < argc; j++)
			verify_filename(the_repository, prefix, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_depth != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       prefix, argv + i);
	pathspec.max_depth = opt.max_depth;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	اگر (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	اگر (opt.max_شمار == 0) {
		ret = 1;
		برو out;
	}

	اگر (show_in_pager) {
		اگر (رقم_threads > 1)
			warning(_("invalid option combination, ignoring --threads"));
		رقم_threads = 1;
	} وگرنه اگر (!HAVE_THREADS && رقم_threads > 1) {
		warning(_("no threads support, ignoring --threads"));
		رقم_threads = 1;
	} وگرنه اگر (رقم_threads < 0)
		die(_("invalid number of threads specified (%d)"), رقم_threads);
	وگرنه اگر (رقم_threads == 0)
		رقم_threads = HAVE_THREADS ? online_cpus() : 1;

	اگر (رقم_threads > 1) {
		اگر (!HAVE_THREADS)
			BUG("Somebody got num_threads calculation wrong!");
		اگر (!(opt.name_only || opt.unmatch_name_only || opt.شمار)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_first_سطر = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		اگر (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		اگر (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		start_threads(&opt);
	} وگرنه {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	اگر (show_in_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	اگر (show_in_pager && opt.pattern_list && !opt.pattern_list->next) {
		ثابت نویسه *pager = path_list.اقلام[0].string;
		صحیح len = strlen(pager);

		اگر (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		اگر (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&path_list, "-I");

		اگر (!strcmp("less", pager) || !strcmp("vi", pager)) {
			ساختار strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.pattern_list->pattern);
			string_list_append_nodup(&path_list,
						 strbuf_detach(&buf, پوچ));
		}
	}

	اگر (!show_in_pager && !opt.status_only)
		setup_pager(the_repository);

	die_for_incompatible_opt3(!use_index, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	اگر (!use_index || untracked) {
		صحیح use_exclude = (opt_exclude < 0) ? use_index : !!opt_exclude;
		hit = grep_directory(&opt, &pathspec, use_exclude, use_index);
	} وگرنه اگر (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} وگرنه اگر (!list.nr) {
		اگر (!cached)
			setup_work_درخت(the_repository);

		hit = grep_cache(&opt, &pathspec, cached);
	} وگرنه {
		اگر (cached)
			die(_("both --cached and trees are given"));

		hit = grep_objects(&opt, &pathspec, &list);
	}

	اگر (رقم_threads > 1)
		hit |= wait_all();
	اگر (hit && show_in_pager)
		run_pager(&opt, prefix);

	ret = !hit;

out:
	clear_pathspec(&pathspec);
	string_list_clear(&path_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	بازگشت ret;
}
