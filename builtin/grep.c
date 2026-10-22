/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "config.h"
#ضمن "tag.h"
#ضمن "tree-walk.h"
#ضمن "parse-options.h"
#ضمن "string-list.h"
#ضمن "run-command.h"
#ضمن "grep.h"
#ضمن "quote.h"
#ضمن "dir.h"
#ضمن "pathspec.h"
#ضمن "setup.h"
#ضمن "submodule.h"
#ضمن "submodule-config.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "odb/source.h"
#ضمن "oid-array.h"
#ضمن "oidset.h"
#ضمن "pager.h"
#ضمن "path.h"
#ضمن "promisor-remote.h"
#ضمن "read-cache-ll.h"
#ضمن "write-or-die.h"

ساكن ثابت حرف *grep_prefix;

ساكن حرف ثابت * ثابت grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	عدم
};

ساكن صحيح recurse_submodules;

ساكن صحيح رەقەم_threads;

ساكن pthread_t *threads;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
بنية work_item {
	بنية grep_source source;
	حرف done;
	بنية strbuf out;
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
ساكن بنية work_item todo[TODO_SIZE];
ساكن صحيح todo_start;
ساكن صحيح todo_end;
ساكن صحيح todo_done;

/* Has all work items been added? */
ساكن صحيح all_work_added;

ساكن بنية repository **repos_to_free;
ساكن size_t repos_to_free_nr, repos_to_free_alloc;

/* This lock protects all the variables above. */
ساكن pthread_mutex_t grep_mutex;

ساكن مضمن فراغ grep_lock(فراغ)
{
	pthread_mutex_lock(&grep_mutex);
}

ساكن مضمن فراغ grep_unlock(فراغ)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
ساكن pthread_cond_t cond_add;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
ساكن pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
ساكن pthread_cond_t cond_result;

ساكن صحيح skip_first_قۇر;

ساكن فراغ add_work(بنية grep_opt *opt, بنية grep_source *gs)
{
	إذا (opt->ئىككىلىك != GREP_ئىككىلىك_TEXT)
		grep_source_load_driver(gs, opt->repo->index);

	grep_lock();

	طالما ((todo_end+1) % ARRAY_SIZE(todo) == todo_done) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_end].source = *gs;
	todo[todo_end].done = 0;
	strbuf_reset(&todo[todo_end].out);
	todo_end = (todo_end + 1) % ARRAY_SIZE(todo);

	pthread_cond_signal(&cond_add);
	grep_unlock();
}

ساكن بنية work_item *get_work(فراغ)
{
	بنية work_item *ret;

	grep_lock();
	طالما (todo_start == todo_end && !all_work_added) {
		pthread_cond_wait(&cond_add, &grep_mutex);
	}

	إذا (todo_start == todo_end && all_work_added) {
		ret = عدم;
	} وإلا {
		ret = &todo[todo_start];
		todo_start = (todo_start + 1) % ARRAY_SIZE(todo);
	}
	grep_unlock();
	أرجع ret;
}

ساكن فراغ work_done(بنية work_item *w)
{
	صحيح old_done;

	grep_lock();
	w->done = 1;
	old_done = todo_done;
	لكل(; todo[todo_done].done && todo_done != todo_start;
	    todo_done = (todo_done+1) % ARRAY_SIZE(todo)) {
		w = &todo[todo_done];
		إذا (w->out.len) {
			ثابت حرف *p = w->out.buf;
			size_t len = w->out.len;

			/* Skip the leading hunk mark of the first file. */
			إذا (skip_first_قۇر) {
				طالما (len) {
					len--;
					إذا (*p++ == '\n')
						اكسر;
				}
				skip_first_قۇر = 0;
			}

			write_or_die(1, p, len);
		}
		grep_source_clear(&w->source);
	}

	إذا (old_done != todo_done)
		pthread_cond_signal(&cond_write);

	إذا (all_work_added && todo_done == todo_end)
		pthread_cond_signal(&cond_result);

	grep_unlock();
}

ساكن فراغ free_repos(فراغ)
{
	صحيح i;

	لكل (i = 0; i < repos_to_free_nr; i++) {
		repo_clear(repos_to_free[i]);
		حرر(repos_to_free[i]);
	}
	FREE_AND_NULL(repos_to_free);
	repos_to_free_nr = 0;
	repos_to_free_alloc = 0;
}

ساكن فراغ *run(فراغ *arg)
{
	صحيح hit = 0;
	بنية grep_opt *opt = arg;

	طالما (1) {
		بنية work_item *w = get_work();
		إذا (!w)
			اكسر;

		opt->output_priv = w;
		hit |= grep_source(opt, &w->source);
		grep_source_clear_data(&w->source);
		work_done(w);
	}
	free_grep_patterns(opt);
	حرر(opt);

	أرجع (فراغ*) (intptr_t) hit;
}

ساكن فراغ strbuf_out(بنية grep_opt *opt, ثابت فراغ *buf, size_t size)
{
	بنية work_item *w = opt->output_priv;
	strbuf_add(&w->out, buf, size);
}

ساكن فراغ start_threads(بنية grep_opt *opt)
{
	صحيح i;

	pthread_mutex_init(&grep_mutex, عدم);
	pthread_mutex_init(&grep_attr_mutex, عدم);
	pthread_cond_init(&cond_add, عدم);
	pthread_cond_init(&cond_write, عدم);
	pthread_cond_init(&cond_result, عدم);
	grep_use_locks = 1;
	enable_obj_read_lock();

	لكل (i = 0; i < ARRAY_SIZE(todo); i++) {
		strbuf_init(&todo[i].out, 0);
	}

	CALLOC_ARRAY(threads, رەقەم_threads);
	لكل (i = 0; i < رەقەم_threads; i++) {
		صحيح err;
		بنية grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_out;
		compile_grep_patterns(o);
		err = pthread_create(&threads[i], عدم, run, o);

		إذا (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

ساكن صحيح wait_all(فراغ)
{
	صحيح hit = 0;
	صحيح i;

	إذا (!HAVE_THREADS)
		BUG("Never call this function unless you have started threads");

	grep_lock();
	all_work_added = 1;

	/* Wait until all work is done. */
	طالما (todo_done != todo_end)
		pthread_cond_wait(&cond_result, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_broadcast(&cond_add);
	grep_unlock();

	لكل (i = 0; i < رەقەم_threads; i++) {
		فراغ *h;
		pthread_join(threads[i], &h);
		hit |= (صحيح) (intptr_t) h;
	}

	حرر(threads);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_add);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_result);
	grep_use_locks = 0;
	disable_obj_read_lock();

	أرجع hit;
}

ساكن صحيح grep_cmd_config(ثابت حرف *var, ثابت حرف *قىممەت,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	صحيح st = grep_config(var, قىممەت, ctx, cb);

	إذا (git_color_config(var, قىممەت, cb) < 0)
		st = -1;
	وإلا إذا (git_default_config(var, قىممەت, ctx, cb) < 0)
		st = -1;

	إذا (!strcmp(var, "grep.threads")) {
		رەقەم_threads = git_config_int(var, قىممەت, ctx->kvi);
		إذا (رەقەم_threads < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    رەقەم_threads, var);
		وإلا إذا (!HAVE_THREADS && رەقەم_threads > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			رەقەم_threads = 1;
		}
	}

	إذا (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, قىممەت);

	أرجع st;
}

ساكن فراغ grep_source_name(بنية grep_opt *opt, ثابت حرف *filename,
			     صحيح دەرەخ_name_len, بنية strbuf *out)
{
	strbuf_reset(out);

	إذا (opt->null_following_name) {
		إذا (opt->relative && grep_prefix) {
			بنية strbuf rel_buf = STRBUF_INIT;
			ثابت حرف *rel_name =
				relative_path(filename + دەرەخ_name_len,
					      grep_prefix, &rel_buf);

			إذا (دەرەخ_name_len)
				strbuf_add(out, filename, دەرەخ_name_len);

			strbuf_addstr(out, rel_name);
			strbuf_release(&rel_buf);
		} وإلا {
			strbuf_addstr(out, filename);
		}
		أرجع;
	}

	إذا (opt->relative && grep_prefix)
		quote_path(filename + دەرەخ_name_len, grep_prefix, out, 0);
	وإلا
		quote_c_style(filename + دەرەخ_name_len, out, عدم, 0);

	إذا (دەرەخ_name_len)
		strbuf_قىستۇرۇش(out, 0, filename, دەرەخ_name_len);
}

ساكن صحيح grep_oid(بنية grep_opt *opt, ثابت بنية object_id *oid,
		     ثابت حرف *filename, صحيح دەرەخ_name_len,
		     ثابت حرف *path)
{
	بنية strbuf pathbuf = STRBUF_INIT;
	بنية grep_source gs;

	grep_source_name(opt, filename, دەرەخ_name_len, &pathbuf);
	grep_source_init_oid(&gs, pathbuf.buf, path, oid, opt->repo);
	strbuf_release(&pathbuf);

	إذا (رەقەم_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		أرجع 0;
	} وإلا {
		صحيح hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		أرجع hit;
	}
}

ساكن صحيح grep_file(بنية grep_opt *opt, ثابت حرف *filename)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية grep_source gs;

	grep_source_name(opt, filename, 0, &buf);
	grep_source_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	إذا (رەقەم_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		أرجع 0;
	} وإلا {
		صحيح hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		أرجع hit;
	}
}

ساكن فراغ append_path(بنية grep_opt *opt, ثابت فراغ *data, size_t len)
{
	بنية string_list *path_list = opt->output_priv;

	إذا (len == 1 && *(ثابت حرف *)data == '\0')
		أرجع;
	string_list_append_nodup(path_list, xstrndup(data, len));
}

ساكن فراغ run_pager(بنية grep_opt *opt, ثابت حرف *prefix)
{
	بنية string_list *path_list = opt->output_priv;
	بنية child_process child = CHILD_PROCESS_INIT;
	صحيح i, status;

	لكل (i = 0; i < path_list->nr; i++)
		strvec_ئىتتىرىش(&child.args, path_list->بۇيۇملار[i].string);
	child.dir = prefix;
	child.use_shell = 1;

	status = run_command(&child);
	إذا (status)
		exit(status);
}

ساكن صحيح grep_cache(بنية grep_opt *opt,
		      ثابت بنية pathspec *pathspec, صحيح cached);
ساكن صحيح grep_دەرەخ(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
		     بنية دەرەخ_desc *دەرەخ, بنية strbuf *base, صحيح tn_len,
		     صحيح check_attr);

ساكن صحيح grep_submodule(بنية grep_opt *opt,
			  ثابت بنية pathspec *pathspec,
			  ثابت بنية object_id *oid,
			  ثابت حرف *filename, ثابت حرف *path, صحيح cached)
{
	بنية repository *subrepo;
	بنية repository *superproject = opt->repo;
	بنية grep_opt subopt;
	صحيح hit = 0;

	إذا (!is_submodule_active(superproject, path))
		أرجع 0;

	subrepo = xmalloc(حجم(*subrepo));
	إذا (repo_submodule_init(subrepo, superproject, path, null_oid(opt->repo->hash_algo))) {
		حرر(subrepo);
		أرجع 0;
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

	memcpy(&subopt, opt, حجم(subopt));
	subopt.repo = subrepo;

	إذا (oid) {
		تعداد object_type object_type;
		بنية دەرەخ_desc دەرەخ;
		فراغ *data;
		size_t size;
		بنية strbuf base = STRBUF_INIT;

		obj_read_lock();
		object_type = odb_read_object_info(subrepo->objects, oid, عدم);
		obj_read_unlock();
		data = odb_read_object_peeled(subrepo->objects, oid, OBJ_دەرەخ, &size, عدم);
		إذا (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(oid));

		strbuf_addstr(&base, filename);
		strbuf_addch(&base, '/');

		init_دەرەخ_desc(&دەرەخ, oid, data, size);
		hit = grep_دەرەخ(&subopt, pathspec, &دەرەخ, &base, base.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&base);
		حرر(data);
	} وإلا {
		hit = grep_cache(&subopt, pathspec, cached);
	}

	أرجع hit;
}

ساكن صحيح grep_cache(بنية grep_opt *opt,
		      ثابت بنية pathspec *pathspec, صحيح cached)
{
	بنية repository *repo = opt->repo;
	صحيح hit = 0;
	صحيح nr;
	بنية strbuf name = STRBUF_INIT;
	صحيح name_base_len = 0;
	إذا (repo->submodule_prefix) {
		name_base_len = strlen(repo->submodule_prefix);
		strbuf_addstr(&name, repo->submodule_prefix);
	}

	إذا (repo_read_index(repo) < 0)
		die(_("index file corrupt"));

	لكل (nr = 0; nr < repo->index->cache_nr; nr++) {
		ثابت بنية cache_entry *ce = repo->index->cache[nr];

		إذا (!cached && ce_skip_worktree(ce))
			استمر;

		strbuf_setlen(&name, name_base_len);
		strbuf_addstr(&name, ce->name);
		إذا (S_ISSPARSEDIR(ce->ce_mode)) {
			تعداد object_type type;
			بنية دەرەخ_desc دەرەخ;
			فراغ *data;
			size_t size;

			data = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &size);
			إذا (!data)
				die(_("unable to read tree %s"), oid_to_hex(&ce->oid));
			init_دەرەخ_desc(&دەرەخ, &ce->oid, data, size);

			hit |= grep_دەرەخ(opt, pathspec, &دەرەخ, &name, 0, 0);
			strbuf_setlen(&name, name_base_len);
			strbuf_addstr(&name, ce->name);
			حرر(data);
		} وإلا إذا (S_ISREG(ce->ce_mode) &&
		    match_pathspec(repo->index, pathspec, name.buf, name.len, 0, عدم,
				   S_ISDIR(ce->ce_mode) ||
				   S_ISGITLINK(ce->ce_mode))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			إذا (cached || (ce->ce_flags & CE_VALID)) {
				إذا (ce_stage(ce) || ce_intent_to_add(ce))
					استمر;
				hit |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} وإلا {
				hit |= grep_file(opt, name.buf);
			}
		} وإلا إذا (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
			   submodule_path_match(repo->index, pathspec, name.buf, عدم)) {
			hit |= grep_submodule(opt, pathspec, عدم, ce->name,
					      ce->name, cached);
		} وإلا {
			استمر;
		}

		إذا (ce_stage(ce)) {
			افعل {
				nr++;
			} طالما (nr < repo->index->cache_nr &&
				 !strcmp(ce->name, repo->index->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		إذا (hit && opt->status_only)
			اكسر;
	}

	strbuf_release(&name);
	أرجع hit;
}

ساكن صحيح grep_دەرەخ(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
		     بنية دەرەخ_desc *دەرەخ, بنية strbuf *base, صحيح tn_len,
		     صحيح check_attr)
{
	بنية repository *repo = opt->repo;
	صحيح hit = 0;
	تعداد interesting match = entry_not_interesting;
	بنية name_entry entry;
	صحيح old_baselen = base->len;
	بنية strbuf name = STRBUF_INIT;
	صحيح name_base_len = 0;
	إذا (repo->submodule_prefix) {
		strbuf_addstr(&name, repo->submodule_prefix);
		name_base_len = name.len;
	}

	طالما (دەرەخ_entry(دەرەخ, &entry)) {
		صحيح te_len = دەرەخ_entry_len(&entry);

		إذا (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = دەرەخ_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_setlen(&name, name_base_len);

			إذا (match == all_entries_not_interesting)
				اكسر;
			إذا (match == entry_not_interesting)
				استمر;
		}

		strbuf_add(base, entry.path, te_len);

		إذا (S_ISREG(entry.mode)) {
			hit |= grep_oid(opt, &entry.oid, base->buf, tn_len,
					 check_attr ? base->buf + tn_len : عدم);
		} وإلا إذا (S_ISDIR(entry.mode)) {
			تعداد object_type type;
			بنية دەرەخ_desc sub;
			فراغ *data;
			size_t size;

			data = odb_read_object(the_repository->objects,
					       &entry.oid, &type, &size);
			إذا (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_دەرەخ_desc(&sub, &entry.oid, data, size);
			hit |= grep_دەرەخ(opt, pathspec, &sub, base, tn_len,
					 check_attr);
			حرر(data);
		} وإلا إذا (recurse_submodules && S_ISGITLINK(entry.mode)) {
			hit |= grep_submodule(opt, pathspec, &entry.oid,
					      base->buf, base->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(base, old_baselen);

		إذا (hit && opt->status_only)
			اكسر;
	}

	strbuf_release(&name);
	أرجع hit;
}

ساكن فراغ collect_blob_oids_for_دەرەخ(بنية repository *repo,
				       ثابت بنية pathspec *pathspec,
				       بنية دەرەخ_desc *دەرەخ,
				       بنية strbuf *base,
				       صحيح tn_len,
				       بنية oidset *blob_oids)
{
	بنية name_entry entry;
	صحيح old_baselen = base->len;
	بنية strbuf name = STRBUF_INIT;
	تعداد interesting match = entry_not_interesting;

	طالما (دەرەخ_entry(دەرەخ, &entry)) {
		إذا (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = دەرەخ_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_reset(&name);

			إذا (match == all_entries_not_interesting)
				اكسر;
			إذا (match == entry_not_interesting)
				استمر;
		}

		strbuf_add(base, entry.path, دەرەخ_entry_len(&entry));

		إذا (S_ISREG(entry.mode)) {
			إذا (!odb_has_object(repo->objects, &entry.oid, 0))
				oidset_قىستۇرۇش(blob_oids, &entry.oid);
		} وإلا إذا (S_ISDIR(entry.mode)) {
			تعداد object_type type;
			بنية دەرەخ_desc sub_دەرەخ;
			فراغ *data;
			size_t size;

			data = odb_read_object(repo->objects, &entry.oid,
					       &type, &size);
			إذا (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_دەرەخ_desc(&sub_دەرەخ, &entry.oid, data, size);
			collect_blob_oids_for_دەرەخ(repo, pathspec, &sub_دەرەخ,
						   base, tn_len, blob_oids);
			حرر(data);
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

ساكن فراغ collect_blob_oids_for_treeish(بنية grep_opt *opt,
					  ثابت بنية pathspec *pathspec,
					  ثابت بنية object_id *دەرەخ_ish_oid,
					  ثابت حرف *name,
					  بنية oidset *blob_oids)
{
	بنية دەرەخ_desc دەرەخ;
	فراغ *data;
	size_t size;
	بنية strbuf base = STRBUF_INIT;
	صحيح len;

	data = odb_read_object_peeled(opt->repo->objects, دەرەخ_ish_oid,
				      OBJ_دەرەخ, &size, عدم);

	إذا (!data)
		أرجع;

	len = name ? strlen(name) : 0;
	إذا (len) {
		strbuf_add(&base, name, len);
		strbuf_addch(&base, ':');
	}
	init_دەرەخ_desc(&دەرەخ, دەرەخ_ish_oid, data, size);

	collect_blob_oids_for_دەرەخ(opt->repo, pathspec, &دەرەخ,
				   &base, base.len, blob_oids);

	strbuf_release(&base);
	حرر(data);
}

ساكن فراغ prefetch_grep_blobs(بنية grep_opt *opt,
				ثابت بنية pathspec *pathspec,
				ثابت بنية object_array *list)
{
	بنية oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	إذا (!repo_has_promisor_remote(opt->repo))
		أرجع;

	/* For each tree, gather the blobs in it */
	لكل (صحيح i = 0; i < list->nr; i++) {
		بنية object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     عدم, 0);
		obj_read_unlock();

		إذا (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_دەرەخ))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	إذا (oidset_size(&blob_oids)) {
		بنية oid_array to_fetch = OID_ARRAY_INIT;
		بنية oidset_iter iter;
		ثابت بنية object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		طالما ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(opt->repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}
	oidset_clear(&blob_oids);
}

ساكن صحيح grep_object(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
		       بنية object *obj, ثابت حرف *name, ثابت حرف *path)
{
	إذا (obj->type == OBJ_BLOB)
		أرجع grep_oid(opt, &obj->oid, name, 0, path);
	إذا (obj->type == OBJ_COMMIT || obj->type == OBJ_دەرەخ) {
		بنية دەرەخ_desc دەرەخ;
		فراغ *data;
		size_t size;
		بنية strbuf base;
		صحيح hit, len;

		data = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_دەرەخ, &size, عدم);
		إذا (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(&obj->oid));

		len = name ? strlen(name) : 0;
		strbuf_init(&base, PATH_MAX + len + 1);
		إذا (len) {
			strbuf_add(&base, name, len);
			strbuf_addch(&base, ':');
		}
		init_دەرەخ_desc(&دەرەخ, &obj->oid, data, size);
		hit = grep_دەرەخ(opt, pathspec, &دەرەخ, &base, base.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&base);
		حرر(data);
		أرجع hit;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

ساكن صحيح grep_objects(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
			ثابت بنية object_array *list)
{
	unsigned صحيح i;
	صحيح hit = 0;
	ثابت unsigned صحيح nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	لكل (i = 0; i < nr; i++) {
		بنية object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     عدم, 0);
		obj_read_unlock();

		إذا (!real_obj) {
			حرف hex[GIT_MAX_HEXSZ + 1];
			ثابت حرف *name = list->objects[i].name;

			إذا (!name) {
				oid_to_hex_r(hex, &list->objects[i].item->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		إذا (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_lock();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		إذا (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].path)) {
			hit = 1;
			إذا (opt->status_only)
				اكسر;
		}
	}
	أرجع hit;
}

ساكن صحيح grep_directory(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
			  صحيح exc_std, صحيح use_index)
{
	بنية dir_struct dir = DIR_INIT;
	صحيح i, hit = 0;

	إذا (!use_index)
		dir.flags |= DIR_NO_GITLINKS;
	إذا (exc_std)
		setup_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->index, pathspec);
	لكل (i = 0; i < dir.nr; i++) {
		hit |= grep_file(opt, dir.entries[i]->name);
		إذا (hit && opt->status_only)
			اكسر;
	}
	dir_clear(&dir);
	أرجع hit;
}

ساكن صحيح context_callback(ثابت بنية option *opt, ثابت حرف *arg,
			    صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قىممەت;
	صحيح قىممەت;
	ثابت حرف *endp;

	إذا (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		أرجع 0;
	}
	قىممەت = strtol(arg, (حرف **)&endp, 10);
	إذا (*endp) {
		أرجع error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = قىممەت;
	أرجع 0;
}

ساكن صحيح file_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قىممەت;
	صحيح from_stdin;
	ثابت حرف *filename = arg;
	FILE *patterns;
	صحيح lno = 0;
	بنية strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	إذا (!*filename)
		; /* leave it as-is */
	وإلا
		filename = prefix_filename_except_for_dash(grep_prefix, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	إذا (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	طالما (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		إذا (sb.len == 0)
			استمر;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_PATTERN);
	}
	إذا (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	إذا (filename != arg)
		حرر((فراغ *)filename);
	أرجع 0;
}

ساكن صحيح not_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قىممەت;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--not", "command line", 0, GREP_NOT);
	أرجع 0;
}

ساكن صحيح and_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قىممەت;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--and", "command line", 0, GREP_AND);
	أرجع 0;
}

ساكن صحيح open_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قىممەت;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	أرجع 0;
}

ساكن صحيح close_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قىممەت;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	أرجع 0;
}

ساكن صحيح pattern_callback(ثابت بنية option *opt, ثابت حرف *arg,
			    صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قىممەت;
	BUG_ON_OPT_NEG(unset);
	append_grep_pattern(grep_opt, arg, "-e option", 0, GREP_PATTERN);
	أرجع 0;
}

صحيح cmd_grep(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *prefix,
	     بنية repository *repo UNUSED)
{
	صحيح hit = 0;
	صحيح cached = 0, untracked = 0, opt_exclude = -1;
	صحيح seen_dashdash = 0;
	صحيح external_grep_allowed__ignored;
	ثابت حرف *show_in_pager = عدم, *default_pager = "dummy";
	بنية grep_opt opt;
	بنية object_array list = OBJECT_ARRAY_INIT;
	بنية pathspec pathspec;
	بنية string_list path_list = STRING_LIST_INIT_DUP;
	صحيح i;
	صحيح dummy;
	صحيح use_index = 1;
	صحيح allow_revs;
	صحيح ret;

	بنية option options[] = {
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
		OPT_SET_INT('a', "text", &opt.ئىككىلىك,
			N_("process binary files as text"), GREP_ئىككىلىك_TEXT),
		OPT_SET_INT('I', عدم, &opt.ئىككىلىك,
			N_("don't match patterns in binary files"),
			GREP_ئىككىلىك_NOMATCH),
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
		OPT_NEGBIT('h', عدم, &opt.pathname, N_("don't show filenames"), 1),
		OPT_BIT('H', عدم, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.سانى,
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
		OPT_INTEGER(0, "threads", &رەقەم_threads,
			N_("use <n> worker threads")),
		OPT_سان_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_GROUP(""),
		OPT_CALLBACK('f', عدم, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', عدم, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, pattern_callback),
		OPT_CALLBACK_F(0, "and", &opt, عدم,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, عدم, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', عدم, &opt, عدم, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', عدم, &opt, عدم, "",
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
			.قىممەت = &show_in_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_سانى,
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

	إذا (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	إذا (use_index && !startup_info->have_repository) {
		صحيح fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		إذا (fallback)
			use_index = 0;
		وإلا
			/* die the same way as if we did it at the beginning */
			setup_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	إذا (!use_index)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	إذا (argc > 0 && !opt.pattern_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	إذا (argc > 0 && !opt.pattern_list) {
		append_grep_pattern(&opt, argv[0], "command line", 0,
				    GREP_PATTERN);
		argv++;
		argc--;
	}

	إذا (show_in_pager == default_pager)
		show_in_pager = git_pager(the_repository, 1);
	إذا (show_in_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &path_list;
		opt.output = append_path;
		string_list_append(&path_list, show_in_pager);
	}

	إذا (!opt.pattern_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	إذا (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	لكل (i = 0; i < argc; i++) {
		إذا (!strcmp(argv[i], "--")) {
			seen_dashdash = 1;
			اكسر;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_index && !untracked;
	لكل (i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		بنية object_id oid;
		بنية object_context oc = {0};
		بنية object *object;

		إذا (!strcmp(arg, "--")) {
			i++;
			اكسر;
		}

		إذا (!allow_revs) {
			إذا (seen_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			اكسر;
		}

		إذا (get_oid_with_context(the_repository, arg,
					 GET_OID_RECORD_PATH,
					 &oid, &oc)) {
			إذا (seen_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			اكسر;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		إذا (!seen_dashdash)
			verify_non_filename(the_repository, prefix, arg);
		add_object_array_with_path(object, arg, &list, oc.mode, oc.path);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	إذا (!seen_dashdash) {
		صحيح j;
		لكل (j = i; j < argc; j++)
			verify_filename(the_repository, prefix, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_depth != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       prefix, argv + i);
	pathspec.max_depth = opt.max_depth;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	إذا (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	إذا (opt.max_سانى == 0) {
		ret = 1;
		اذهب out;
	}

	إذا (show_in_pager) {
		إذا (رەقەم_threads > 1)
			warning(_("invalid option combination, ignoring --threads"));
		رەقەم_threads = 1;
	} وإلا إذا (!HAVE_THREADS && رەقەم_threads > 1) {
		warning(_("no threads support, ignoring --threads"));
		رەقەم_threads = 1;
	} وإلا إذا (رەقەم_threads < 0)
		die(_("invalid number of threads specified (%d)"), رەقەم_threads);
	وإلا إذا (رەقەم_threads == 0)
		رەقەم_threads = HAVE_THREADS ? online_cpus() : 1;

	إذا (رەقەم_threads > 1) {
		إذا (!HAVE_THREADS)
			BUG("Somebody got num_threads calculation wrong!");
		إذا (!(opt.name_only || opt.unmatch_name_only || opt.سانى)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_first_قۇر = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		إذا (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		إذا (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		start_threads(&opt);
	} وإلا {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	إذا (show_in_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	إذا (show_in_pager && opt.pattern_list && !opt.pattern_list->next) {
		ثابت حرف *pager = path_list.بۇيۇملار[0].string;
		صحيح len = strlen(pager);

		إذا (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		إذا (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&path_list, "-I");

		إذا (!strcmp("less", pager) || !strcmp("vi", pager)) {
			بنية strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.pattern_list->pattern);
			string_list_append_nodup(&path_list,
						 strbuf_detach(&buf, عدم));
		}
	}

	إذا (!show_in_pager && !opt.status_only)
		setup_pager(the_repository);

	die_for_incompatible_opt3(!use_index, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	إذا (!use_index || untracked) {
		صحيح use_exclude = (opt_exclude < 0) ? use_index : !!opt_exclude;
		hit = grep_directory(&opt, &pathspec, use_exclude, use_index);
	} وإلا إذا (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} وإلا إذا (!list.nr) {
		إذا (!cached)
			setup_work_دەرەخ(the_repository);

		hit = grep_cache(&opt, &pathspec, cached);
	} وإلا {
		إذا (cached)
			die(_("both --cached and trees are given"));

		hit = grep_objects(&opt, &pathspec, &list);
	}

	إذا (رەقەم_threads > 1)
		hit |= wait_all();
	إذا (hit && show_in_pager)
		run_pager(&opt, prefix);

	ret = !hit;

out:
	clear_pathspec(&pathspec);
	string_list_clear(&path_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	أرجع ret;
}
