/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "tree-walk.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "grep.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "pathspec.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "submodule-config.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/source.h"
#அந்தர்பாவ "oid-array.h"
#அந்தர்பாவ "oidset.h"
#அந்தர்பாவ "pager.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "promisor-remote.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "write-or-die.h"

நிலையான மாறிலி எழுத்து *grep_prefix;

நிலையான எழுத்து மாறிலி * மாறிலி grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	வெற்று
};

நிலையான முழு recurse_submodules;

நிலையான முழு இலக்கம்_threads;

நிலையான pthread_t *threads;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
கட்டமைப்பு work_item {
	கட்டமைப்பு grep_source source;
	எழுத்து done;
	கட்டமைப்பு strbuf out;
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
நிலையான கட்டமைப்பு work_item todo[TODO_SIZE];
நிலையான முழு todo_start;
நிலையான முழு todo_end;
நிலையான முழு todo_done;

/* Has all work items been added? */
நிலையான முழு all_work_added;

நிலையான கட்டமைப்பு repository **repos_to_free;
நிலையான size_t repos_to_free_nr, repos_to_free_alloc;

/* This lock protects all the variables above. */
நிலையான pthread_mutex_t grep_mutex;

நிலையான உள்வரி வெறுமை grep_lock(வெறுமை)
{
	pthread_mutex_lock(&grep_mutex);
}

நிலையான உள்வரி வெறுமை grep_unlock(வெறுமை)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
நிலையான pthread_cond_t cond_add;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
நிலையான pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
நிலையான pthread_cond_t cond_result;

நிலையான முழு skip_first_வரி;

நிலையான வெறுமை add_work(கட்டமைப்பு grep_opt *opt, கட்டமைப்பு grep_source *gs)
{
	எனில் (opt->இருமம் != GREP_இருமம்_TEXT)
		grep_source_load_driver(gs, opt->repo->index);

	grep_lock();

	போது ((todo_end+1) % ARRAY_SIZE(todo) == todo_done) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_end].source = *gs;
	todo[todo_end].done = 0;
	strbuf_reset(&todo[todo_end].out);
	todo_end = (todo_end + 1) % ARRAY_SIZE(todo);

	pthread_cond_signal(&cond_add);
	grep_unlock();
}

நிலையான கட்டமைப்பு work_item *get_work(வெறுமை)
{
	கட்டமைப்பு work_item *ret;

	grep_lock();
	போது (todo_start == todo_end && !all_work_added) {
		pthread_cond_wait(&cond_add, &grep_mutex);
	}

	எனில் (todo_start == todo_end && all_work_added) {
		ret = வெற்று;
	} இல்லையெனில் {
		ret = &todo[todo_start];
		todo_start = (todo_start + 1) % ARRAY_SIZE(todo);
	}
	grep_unlock();
	திரும்பு ret;
}

நிலையான வெறுமை work_done(கட்டமைப்பு work_item *w)
{
	முழு old_done;

	grep_lock();
	w->done = 1;
	old_done = todo_done;
	க்கு(; todo[todo_done].done && todo_done != todo_start;
	    todo_done = (todo_done+1) % ARRAY_SIZE(todo)) {
		w = &todo[todo_done];
		எனில் (w->out.len) {
			மாறிலி எழுத்து *p = w->out.buf;
			size_t len = w->out.len;

			/* Skip the leading hunk mark of the first file. */
			எனில் (skip_first_வரி) {
				போது (len) {
					len--;
					எனில் (*p++ == '\n')
						நிறுத்து;
				}
				skip_first_வரி = 0;
			}

			write_or_die(1, p, len);
		}
		grep_source_clear(&w->source);
	}

	எனில் (old_done != todo_done)
		pthread_cond_signal(&cond_write);

	எனில் (all_work_added && todo_done == todo_end)
		pthread_cond_signal(&cond_result);

	grep_unlock();
}

நிலையான வெறுமை free_repos(வெறுமை)
{
	முழு i;

	க்கு (i = 0; i < repos_to_free_nr; i++) {
		repo_clear(repos_to_free[i]);
		விடுவி(repos_to_free[i]);
	}
	FREE_AND_NULL(repos_to_free);
	repos_to_free_nr = 0;
	repos_to_free_alloc = 0;
}

நிலையான வெறுமை *run(வெறுமை *arg)
{
	முழு hit = 0;
	கட்டமைப்பு grep_opt *opt = arg;

	போது (1) {
		கட்டமைப்பு work_item *w = get_work();
		எனில் (!w)
			நிறுத்து;

		opt->output_priv = w;
		hit |= grep_source(opt, &w->source);
		grep_source_clear_data(&w->source);
		work_done(w);
	}
	free_grep_patterns(opt);
	விடுவி(opt);

	திரும்பு (வெறுமை*) (intptr_t) hit;
}

நிலையான வெறுமை strbuf_out(கட்டமைப்பு grep_opt *opt, மாறிலி வெறுமை *buf, size_t size)
{
	கட்டமைப்பு work_item *w = opt->output_priv;
	strbuf_add(&w->out, buf, size);
}

நிலையான வெறுமை start_threads(கட்டமைப்பு grep_opt *opt)
{
	முழு i;

	pthread_mutex_init(&grep_mutex, வெற்று);
	pthread_mutex_init(&grep_attr_mutex, வெற்று);
	pthread_cond_init(&cond_add, வெற்று);
	pthread_cond_init(&cond_write, வெற்று);
	pthread_cond_init(&cond_result, வெற்று);
	grep_use_locks = 1;
	enable_obj_read_lock();

	க்கு (i = 0; i < ARRAY_SIZE(todo); i++) {
		strbuf_init(&todo[i].out, 0);
	}

	CALLOC_ARRAY(threads, இலக்கம்_threads);
	க்கு (i = 0; i < இலக்கம்_threads; i++) {
		முழு err;
		கட்டமைப்பு grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_out;
		compile_grep_patterns(o);
		err = pthread_create(&threads[i], வெற்று, run, o);

		எனில் (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

நிலையான முழு wait_all(வெறுமை)
{
	முழு hit = 0;
	முழு i;

	எனில் (!HAVE_THREADS)
		BUG("Never call this function unless you have started threads");

	grep_lock();
	all_work_added = 1;

	/* Wait until all work is done. */
	போது (todo_done != todo_end)
		pthread_cond_wait(&cond_result, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_broadcast(&cond_add);
	grep_unlock();

	க்கு (i = 0; i < இலக்கம்_threads; i++) {
		வெறுமை *h;
		pthread_join(threads[i], &h);
		hit |= (முழு) (intptr_t) h;
	}

	விடுவி(threads);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_add);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_result);
	grep_use_locks = 0;
	disable_obj_read_lock();

	திரும்பு hit;
}

நிலையான முழு grep_cmd_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			   மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	முழு st = grep_config(var, மதிப்பு, ctx, cb);

	எனில் (git_color_config(var, மதிப்பு, cb) < 0)
		st = -1;
	இல்லையெனில் எனில் (git_default_config(var, மதிப்பு, ctx, cb) < 0)
		st = -1;

	எனில் (!strcmp(var, "grep.threads")) {
		இலக்கம்_threads = git_config_int(var, மதிப்பு, ctx->kvi);
		எனில் (இலக்கம்_threads < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    இலக்கம்_threads, var);
		இல்லையெனில் எனில் (!HAVE_THREADS && இலக்கம்_threads > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			இலக்கம்_threads = 1;
		}
	}

	எனில் (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, மதிப்பு);

	திரும்பு st;
}

நிலையான வெறுமை grep_source_name(கட்டமைப்பு grep_opt *opt, மாறிலி எழுத்து *filename,
			     முழு மரம்_name_len, கட்டமைப்பு strbuf *out)
{
	strbuf_reset(out);

	எனில் (opt->null_following_name) {
		எனில் (opt->relative && grep_prefix) {
			கட்டமைப்பு strbuf rel_buf = STRBUF_INIT;
			மாறிலி எழுத்து *rel_name =
				relative_path(filename + மரம்_name_len,
					      grep_prefix, &rel_buf);

			எனில் (மரம்_name_len)
				strbuf_add(out, filename, மரம்_name_len);

			strbuf_addstr(out, rel_name);
			strbuf_release(&rel_buf);
		} இல்லையெனில் {
			strbuf_addstr(out, filename);
		}
		திரும்பு;
	}

	எனில் (opt->relative && grep_prefix)
		quote_path(filename + மரம்_name_len, grep_prefix, out, 0);
	இல்லையெனில்
		quote_c_style(filename + மரம்_name_len, out, வெற்று, 0);

	எனில் (மரம்_name_len)
		strbuf_செருகு(out, 0, filename, மரம்_name_len);
}

நிலையான முழு grep_oid(கட்டமைப்பு grep_opt *opt, மாறிலி கட்டமைப்பு object_id *oid,
		     மாறிலி எழுத்து *filename, முழு மரம்_name_len,
		     மாறிலி எழுத்து *path)
{
	கட்டமைப்பு strbuf pathbuf = STRBUF_INIT;
	கட்டமைப்பு grep_source gs;

	grep_source_name(opt, filename, மரம்_name_len, &pathbuf);
	grep_source_init_oid(&gs, pathbuf.buf, path, oid, opt->repo);
	strbuf_release(&pathbuf);

	எனில் (இலக்கம்_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		திரும்பு 0;
	} இல்லையெனில் {
		முழு hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		திரும்பு hit;
	}
}

நிலையான முழு grep_file(கட்டமைப்பு grep_opt *opt, மாறிலி எழுத்து *filename)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு grep_source gs;

	grep_source_name(opt, filename, 0, &buf);
	grep_source_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	எனில் (இலக்கம்_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		திரும்பு 0;
	} இல்லையெனில் {
		முழு hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		திரும்பு hit;
	}
}

நிலையான வெறுமை append_path(கட்டமைப்பு grep_opt *opt, மாறிலி வெறுமை *data, size_t len)
{
	கட்டமைப்பு string_list *path_list = opt->output_priv;

	எனில் (len == 1 && *(மாறிலி எழுத்து *)data == '\0')
		திரும்பு;
	string_list_append_nodup(path_list, xstrndup(data, len));
}

நிலையான வெறுமை run_pager(கட்டமைப்பு grep_opt *opt, மாறிலி எழுத்து *prefix)
{
	கட்டமைப்பு string_list *path_list = opt->output_priv;
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;
	முழு i, status;

	க்கு (i = 0; i < path_list->nr; i++)
		strvec_தள்ளு(&child.args, path_list->பொருட்கள்[i].string);
	child.dir = prefix;
	child.use_shell = 1;

	status = run_command(&child);
	எனில் (status)
		exit(status);
}

நிலையான முழு grep_cache(கட்டமைப்பு grep_opt *opt,
		      மாறிலி கட்டமைப்பு pathspec *pathspec, முழு cached);
நிலையான முழு grep_மரம்(கட்டமைப்பு grep_opt *opt, மாறிலி கட்டமைப்பு pathspec *pathspec,
		     கட்டமைப்பு மரம்_desc *மரம், கட்டமைப்பு strbuf *base, முழு tn_len,
		     முழு check_attr);

நிலையான முழு grep_submodule(கட்டமைப்பு grep_opt *opt,
			  மாறிலி கட்டமைப்பு pathspec *pathspec,
			  மாறிலி கட்டமைப்பு object_id *oid,
			  மாறிலி எழுத்து *filename, மாறிலி எழுத்து *path, முழு cached)
{
	கட்டமைப்பு repository *subrepo;
	கட்டமைப்பு repository *superproject = opt->repo;
	கட்டமைப்பு grep_opt subopt;
	முழு hit = 0;

	எனில் (!is_submodule_active(superproject, path))
		திரும்பு 0;

	subrepo = xmalloc(அளவு(*subrepo));
	எனில் (repo_submodule_init(subrepo, superproject, path, null_oid(opt->repo->hash_algo))) {
		விடுவி(subrepo);
		திரும்பு 0;
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

	memcpy(&subopt, opt, அளவு(subopt));
	subopt.repo = subrepo;

	எனில் (oid) {
		எண்ணிக்கை object_type object_type;
		கட்டமைப்பு மரம்_desc மரம்;
		வெறுமை *data;
		size_t size;
		கட்டமைப்பு strbuf base = STRBUF_INIT;

		obj_read_lock();
		object_type = odb_read_object_info(subrepo->objects, oid, வெற்று);
		obj_read_unlock();
		data = odb_read_object_peeled(subrepo->objects, oid, OBJ_மரம், &size, வெற்று);
		எனில் (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(oid));

		strbuf_addstr(&base, filename);
		strbuf_addch(&base, '/');

		init_மரம்_desc(&மரம், oid, data, size);
		hit = grep_மரம்(&subopt, pathspec, &மரம், &base, base.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&base);
		விடுவி(data);
	} இல்லையெனில் {
		hit = grep_cache(&subopt, pathspec, cached);
	}

	திரும்பு hit;
}

நிலையான முழு grep_cache(கட்டமைப்பு grep_opt *opt,
		      மாறிலி கட்டமைப்பு pathspec *pathspec, முழு cached)
{
	கட்டமைப்பு repository *repo = opt->repo;
	முழு hit = 0;
	முழு nr;
	கட்டமைப்பு strbuf name = STRBUF_INIT;
	முழு name_base_len = 0;
	எனில் (repo->submodule_prefix) {
		name_base_len = strlen(repo->submodule_prefix);
		strbuf_addstr(&name, repo->submodule_prefix);
	}

	எனில் (repo_read_index(repo) < 0)
		die(_("index file corrupt"));

	க்கு (nr = 0; nr < repo->index->cache_nr; nr++) {
		மாறிலி கட்டமைப்பு cache_entry *ce = repo->index->cache[nr];

		எனில் (!cached && ce_skip_worktree(ce))
			தொடர்;

		strbuf_setlen(&name, name_base_len);
		strbuf_addstr(&name, ce->name);
		எனில் (S_ISSPARSEDIR(ce->ce_mode)) {
			எண்ணிக்கை object_type type;
			கட்டமைப்பு மரம்_desc மரம்;
			வெறுமை *data;
			size_t size;

			data = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &size);
			எனில் (!data)
				die(_("unable to read tree %s"), oid_to_hex(&ce->oid));
			init_மரம்_desc(&மரம், &ce->oid, data, size);

			hit |= grep_மரம்(opt, pathspec, &மரம், &name, 0, 0);
			strbuf_setlen(&name, name_base_len);
			strbuf_addstr(&name, ce->name);
			விடுவி(data);
		} இல்லையெனில் எனில் (S_ISREG(ce->ce_mode) &&
		    match_pathspec(repo->index, pathspec, name.buf, name.len, 0, வெற்று,
				   S_ISDIR(ce->ce_mode) ||
				   S_ISGITLINK(ce->ce_mode))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			எனில் (cached || (ce->ce_flags & CE_VALID)) {
				எனில் (ce_stage(ce) || ce_intent_to_add(ce))
					தொடர்;
				hit |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} இல்லையெனில் {
				hit |= grep_file(opt, name.buf);
			}
		} இல்லையெனில் எனில் (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
			   submodule_path_match(repo->index, pathspec, name.buf, வெற்று)) {
			hit |= grep_submodule(opt, pathspec, வெற்று, ce->name,
					      ce->name, cached);
		} இல்லையெனில் {
			தொடர்;
		}

		எனில் (ce_stage(ce)) {
			செய் {
				nr++;
			} போது (nr < repo->index->cache_nr &&
				 !strcmp(ce->name, repo->index->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		எனில் (hit && opt->status_only)
			நிறுத்து;
	}

	strbuf_release(&name);
	திரும்பு hit;
}

நிலையான முழு grep_மரம்(கட்டமைப்பு grep_opt *opt, மாறிலி கட்டமைப்பு pathspec *pathspec,
		     கட்டமைப்பு மரம்_desc *மரம், கட்டமைப்பு strbuf *base, முழு tn_len,
		     முழு check_attr)
{
	கட்டமைப்பு repository *repo = opt->repo;
	முழு hit = 0;
	எண்ணிக்கை interesting match = entry_not_interesting;
	கட்டமைப்பு name_entry entry;
	முழு old_baselen = base->len;
	கட்டமைப்பு strbuf name = STRBUF_INIT;
	முழு name_base_len = 0;
	எனில் (repo->submodule_prefix) {
		strbuf_addstr(&name, repo->submodule_prefix);
		name_base_len = name.len;
	}

	போது (மரம்_entry(மரம், &entry)) {
		முழு te_len = மரம்_entry_len(&entry);

		எனில் (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = மரம்_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_setlen(&name, name_base_len);

			எனில் (match == all_entries_not_interesting)
				நிறுத்து;
			எனில் (match == entry_not_interesting)
				தொடர்;
		}

		strbuf_add(base, entry.path, te_len);

		எனில் (S_ISREG(entry.mode)) {
			hit |= grep_oid(opt, &entry.oid, base->buf, tn_len,
					 check_attr ? base->buf + tn_len : வெற்று);
		} இல்லையெனில் எனில் (S_ISDIR(entry.mode)) {
			எண்ணிக்கை object_type type;
			கட்டமைப்பு மரம்_desc sub;
			வெறுமை *data;
			size_t size;

			data = odb_read_object(the_repository->objects,
					       &entry.oid, &type, &size);
			எனில் (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_மரம்_desc(&sub, &entry.oid, data, size);
			hit |= grep_மரம்(opt, pathspec, &sub, base, tn_len,
					 check_attr);
			விடுவி(data);
		} இல்லையெனில் எனில் (recurse_submodules && S_ISGITLINK(entry.mode)) {
			hit |= grep_submodule(opt, pathspec, &entry.oid,
					      base->buf, base->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(base, old_baselen);

		எனில் (hit && opt->status_only)
			நிறுத்து;
	}

	strbuf_release(&name);
	திரும்பு hit;
}

நிலையான வெறுமை collect_blob_oids_for_மரம்(கட்டமைப்பு repository *repo,
				       மாறிலி கட்டமைப்பு pathspec *pathspec,
				       கட்டமைப்பு மரம்_desc *மரம்,
				       கட்டமைப்பு strbuf *base,
				       முழு tn_len,
				       கட்டமைப்பு oidset *blob_oids)
{
	கட்டமைப்பு name_entry entry;
	முழு old_baselen = base->len;
	கட்டமைப்பு strbuf name = STRBUF_INIT;
	எண்ணிக்கை interesting match = entry_not_interesting;

	போது (மரம்_entry(மரம், &entry)) {
		எனில் (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = மரம்_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_reset(&name);

			எனில் (match == all_entries_not_interesting)
				நிறுத்து;
			எனில் (match == entry_not_interesting)
				தொடர்;
		}

		strbuf_add(base, entry.path, மரம்_entry_len(&entry));

		எனில் (S_ISREG(entry.mode)) {
			எனில் (!odb_has_object(repo->objects, &entry.oid, 0))
				oidset_செருகு(blob_oids, &entry.oid);
		} இல்லையெனில் எனில் (S_ISDIR(entry.mode)) {
			எண்ணிக்கை object_type type;
			கட்டமைப்பு மரம்_desc sub_மரம்;
			வெறுமை *data;
			size_t size;

			data = odb_read_object(repo->objects, &entry.oid,
					       &type, &size);
			எனில் (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_மரம்_desc(&sub_மரம், &entry.oid, data, size);
			collect_blob_oids_for_மரம்(repo, pathspec, &sub_மரம்,
						   base, tn_len, blob_oids);
			விடுவி(data);
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

நிலையான வெறுமை collect_blob_oids_for_treeish(கட்டமைப்பு grep_opt *opt,
					  மாறிலி கட்டமைப்பு pathspec *pathspec,
					  மாறிலி கட்டமைப்பு object_id *மரம்_ish_oid,
					  மாறிலி எழுத்து *name,
					  கட்டமைப்பு oidset *blob_oids)
{
	கட்டமைப்பு மரம்_desc மரம்;
	வெறுமை *data;
	size_t size;
	கட்டமைப்பு strbuf base = STRBUF_INIT;
	முழு len;

	data = odb_read_object_peeled(opt->repo->objects, மரம்_ish_oid,
				      OBJ_மரம், &size, வெற்று);

	எனில் (!data)
		திரும்பு;

	len = name ? strlen(name) : 0;
	எனில் (len) {
		strbuf_add(&base, name, len);
		strbuf_addch(&base, ':');
	}
	init_மரம்_desc(&மரம், மரம்_ish_oid, data, size);

	collect_blob_oids_for_மரம்(opt->repo, pathspec, &மரம்,
				   &base, base.len, blob_oids);

	strbuf_release(&base);
	விடுவி(data);
}

நிலையான வெறுமை prefetch_grep_blobs(கட்டமைப்பு grep_opt *opt,
				மாறிலி கட்டமைப்பு pathspec *pathspec,
				மாறிலி கட்டமைப்பு object_array *list)
{
	கட்டமைப்பு oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	எனில் (!repo_has_promisor_remote(opt->repo))
		திரும்பு;

	/* For each tree, gather the blobs in it */
	க்கு (முழு i = 0; i < list->nr; i++) {
		கட்டமைப்பு object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     வெற்று, 0);
		obj_read_unlock();

		எனில் (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_மரம்))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	எனில் (oidset_size(&blob_oids)) {
		கட்டமைப்பு oid_array to_fetch = OID_ARRAY_INIT;
		கட்டமைப்பு oidset_iter iter;
		மாறிலி கட்டமைப்பு object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		போது ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(opt->repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}
	oidset_clear(&blob_oids);
}

நிலையான முழு grep_object(கட்டமைப்பு grep_opt *opt, மாறிலி கட்டமைப்பு pathspec *pathspec,
		       கட்டமைப்பு object *obj, மாறிலி எழுத்து *name, மாறிலி எழுத்து *path)
{
	எனில் (obj->type == OBJ_BLOB)
		திரும்பு grep_oid(opt, &obj->oid, name, 0, path);
	எனில் (obj->type == OBJ_COMMIT || obj->type == OBJ_மரம்) {
		கட்டமைப்பு மரம்_desc மரம்;
		வெறுமை *data;
		size_t size;
		கட்டமைப்பு strbuf base;
		முழு hit, len;

		data = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_மரம், &size, வெற்று);
		எனில் (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(&obj->oid));

		len = name ? strlen(name) : 0;
		strbuf_init(&base, PATH_MAX + len + 1);
		எனில் (len) {
			strbuf_add(&base, name, len);
			strbuf_addch(&base, ':');
		}
		init_மரம்_desc(&மரம், &obj->oid, data, size);
		hit = grep_மரம்(opt, pathspec, &மரம், &base, base.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&base);
		விடுவி(data);
		திரும்பு hit;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

நிலையான முழு grep_objects(கட்டமைப்பு grep_opt *opt, மாறிலி கட்டமைப்பு pathspec *pathspec,
			மாறிலி கட்டமைப்பு object_array *list)
{
	unsigned முழு i;
	முழு hit = 0;
	மாறிலி unsigned முழு nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	க்கு (i = 0; i < nr; i++) {
		கட்டமைப்பு object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     வெற்று, 0);
		obj_read_unlock();

		எனில் (!real_obj) {
			எழுத்து hex[GIT_MAX_HEXSZ + 1];
			மாறிலி எழுத்து *name = list->objects[i].name;

			எனில் (!name) {
				oid_to_hex_r(hex, &list->objects[i].item->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		எனில் (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_lock();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		எனில் (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].path)) {
			hit = 1;
			எனில் (opt->status_only)
				நிறுத்து;
		}
	}
	திரும்பு hit;
}

நிலையான முழு grep_directory(கட்டமைப்பு grep_opt *opt, மாறிலி கட்டமைப்பு pathspec *pathspec,
			  முழு exc_std, முழு use_index)
{
	கட்டமைப்பு dir_struct dir = DIR_INIT;
	முழு i, hit = 0;

	எனில் (!use_index)
		dir.flags |= DIR_NO_GITLINKS;
	எனில் (exc_std)
		setup_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->index, pathspec);
	க்கு (i = 0; i < dir.nr; i++) {
		hit |= grep_file(opt, dir.entries[i]->name);
		எனில் (hit && opt->status_only)
			நிறுத்து;
	}
	dir_clear(&dir);
	திரும்பு hit;
}

நிலையான முழு context_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			    முழு unset)
{
	கட்டமைப்பு grep_opt *grep_opt = opt->மதிப்பு;
	முழு மதிப்பு;
	மாறிலி எழுத்து *endp;

	எனில் (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		திரும்பு 0;
	}
	மதிப்பு = strtol(arg, (எழுத்து **)&endp, 10);
	எனில் (*endp) {
		திரும்பு error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = மதிப்பு;
	திரும்பு 0;
}

நிலையான முழு file_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு grep_opt *grep_opt = opt->மதிப்பு;
	முழு from_stdin;
	மாறிலி எழுத்து *filename = arg;
	FILE *patterns;
	முழு lno = 0;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	எனில் (!*filename)
		; /* leave it as-is */
	இல்லையெனில்
		filename = prefix_filename_except_for_dash(grep_prefix, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	எனில் (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	போது (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		எனில் (sb.len == 0)
			தொடர்;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_PATTERN);
	}
	எனில் (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	எனில் (filename != arg)
		விடுவி((வெறுமை *)filename);
	திரும்பு 0;
}

நிலையான முழு not_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு grep_opt *grep_opt = opt->மதிப்பு;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--not", "command line", 0, GREP_NOT);
	திரும்பு 0;
}

நிலையான முழு and_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு grep_opt *grep_opt = opt->மதிப்பு;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--and", "command line", 0, GREP_AND);
	திரும்பு 0;
}

நிலையான முழு open_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு grep_opt *grep_opt = opt->மதிப்பு;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	திரும்பு 0;
}

நிலையான முழு close_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு grep_opt *grep_opt = opt->மதிப்பு;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	திரும்பு 0;
}

நிலையான முழு pattern_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			    முழு unset)
{
	கட்டமைப்பு grep_opt *grep_opt = opt->மதிப்பு;
	BUG_ON_OPT_NEG(unset);
	append_grep_pattern(grep_opt, arg, "-e option", 0, GREP_PATTERN);
	திரும்பு 0;
}

முழு cmd_grep(முழு argc,
	     மாறிலி எழுத்து **argv,
	     மாறிலி எழுத்து *prefix,
	     கட்டமைப்பு repository *repo UNUSED)
{
	முழு hit = 0;
	முழு cached = 0, untracked = 0, opt_exclude = -1;
	முழு seen_dashdash = 0;
	முழு external_grep_allowed__ignored;
	மாறிலி எழுத்து *show_in_pager = வெற்று, *default_pager = "dummy";
	கட்டமைப்பு grep_opt opt;
	கட்டமைப்பு object_array list = OBJECT_ARRAY_INIT;
	கட்டமைப்பு pathspec pathspec;
	கட்டமைப்பு string_list path_list = STRING_LIST_INIT_DUP;
	முழு i;
	முழு dummy;
	முழு use_index = 1;
	முழு allow_revs;
	முழு ret;

	கட்டமைப்பு option options[] = {
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
		OPT_SET_INT('a', "text", &opt.இருமம்,
			N_("process binary files as text"), GREP_இருமம்_TEXT),
		OPT_SET_INT('I', வெற்று, &opt.இருமம்,
			N_("don't match patterns in binary files"),
			GREP_இருமம்_NOMATCH),
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
		OPT_NEGBIT('h', வெற்று, &opt.pathname, N_("don't show filenames"), 1),
		OPT_BIT('H', வெற்று, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.மொத்தம்,
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
		OPT_INTEGER(0, "threads", &இலக்கம்_threads,
			N_("use <n> worker threads")),
		OPT_எண்_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_GROUP(""),
		OPT_CALLBACK('f', வெற்று, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', வெற்று, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, pattern_callback),
		OPT_CALLBACK_F(0, "and", &opt, வெற்று,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, வெற்று, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', வெற்று, &opt, வெற்று, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', வெற்று, &opt, வெற்று, "",
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
			.மதிப்பு = &show_in_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_மொத்தம்,
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

	எனில் (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	எனில் (use_index && !startup_info->have_repository) {
		முழு fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		எனில் (fallback)
			use_index = 0;
		இல்லையெனில்
			/* die the same way as if we did it at the beginning */
			setup_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	எனில் (!use_index)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	எனில் (argc > 0 && !opt.pattern_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	எனில் (argc > 0 && !opt.pattern_list) {
		append_grep_pattern(&opt, argv[0], "command line", 0,
				    GREP_PATTERN);
		argv++;
		argc--;
	}

	எனில் (show_in_pager == default_pager)
		show_in_pager = git_pager(the_repository, 1);
	எனில் (show_in_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &path_list;
		opt.output = append_path;
		string_list_append(&path_list, show_in_pager);
	}

	எனில் (!opt.pattern_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	எனில் (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	க்கு (i = 0; i < argc; i++) {
		எனில் (!strcmp(argv[i], "--")) {
			seen_dashdash = 1;
			நிறுத்து;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_index && !untracked;
	க்கு (i = 0; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];
		கட்டமைப்பு object_id oid;
		கட்டமைப்பு object_context oc = {0};
		கட்டமைப்பு object *object;

		எனில் (!strcmp(arg, "--")) {
			i++;
			நிறுத்து;
		}

		எனில் (!allow_revs) {
			எனில் (seen_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			நிறுத்து;
		}

		எனில் (get_oid_with_context(the_repository, arg,
					 GET_OID_RECORD_PATH,
					 &oid, &oc)) {
			எனில் (seen_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			நிறுத்து;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		எனில் (!seen_dashdash)
			verify_non_filename(the_repository, prefix, arg);
		add_object_array_with_path(object, arg, &list, oc.mode, oc.path);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	எனில் (!seen_dashdash) {
		முழு j;
		க்கு (j = i; j < argc; j++)
			verify_filename(the_repository, prefix, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_depth != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       prefix, argv + i);
	pathspec.max_depth = opt.max_depth;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	எனில் (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	எனில் (opt.max_மொத்தம் == 0) {
		ret = 1;
		செல் out;
	}

	எனில் (show_in_pager) {
		எனில் (இலக்கம்_threads > 1)
			warning(_("invalid option combination, ignoring --threads"));
		இலக்கம்_threads = 1;
	} இல்லையெனில் எனில் (!HAVE_THREADS && இலக்கம்_threads > 1) {
		warning(_("no threads support, ignoring --threads"));
		இலக்கம்_threads = 1;
	} இல்லையெனில் எனில் (இலக்கம்_threads < 0)
		die(_("invalid number of threads specified (%d)"), இலக்கம்_threads);
	இல்லையெனில் எனில் (இலக்கம்_threads == 0)
		இலக்கம்_threads = HAVE_THREADS ? online_cpus() : 1;

	எனில் (இலக்கம்_threads > 1) {
		எனில் (!HAVE_THREADS)
			BUG("Somebody got num_threads calculation wrong!");
		எனில் (!(opt.name_only || opt.unmatch_name_only || opt.மொத்தம்)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_first_வரி = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		எனில் (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		எனில் (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		start_threads(&opt);
	} இல்லையெனில் {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	எனில் (show_in_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	எனில் (show_in_pager && opt.pattern_list && !opt.pattern_list->next) {
		மாறிலி எழுத்து *pager = path_list.பொருட்கள்[0].string;
		முழு len = strlen(pager);

		எனில் (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		எனில் (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&path_list, "-I");

		எனில் (!strcmp("less", pager) || !strcmp("vi", pager)) {
			கட்டமைப்பு strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.pattern_list->pattern);
			string_list_append_nodup(&path_list,
						 strbuf_detach(&buf, வெற்று));
		}
	}

	எனில் (!show_in_pager && !opt.status_only)
		setup_pager(the_repository);

	die_for_incompatible_opt3(!use_index, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	எனில் (!use_index || untracked) {
		முழு use_exclude = (opt_exclude < 0) ? use_index : !!opt_exclude;
		hit = grep_directory(&opt, &pathspec, use_exclude, use_index);
	} இல்லையெனில் எனில் (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} இல்லையெனில் எனில் (!list.nr) {
		எனில் (!cached)
			setup_work_மரம்(the_repository);

		hit = grep_cache(&opt, &pathspec, cached);
	} இல்லையெனில் {
		எனில் (cached)
			die(_("both --cached and trees are given"));

		hit = grep_objects(&opt, &pathspec, &list);
	}

	எனில் (இலக்கம்_threads > 1)
		hit |= wait_all();
	எனில் (hit && show_in_pager)
		run_pager(&opt, prefix);

	ret = !hit;

out:
	clear_pathspec(&pathspec);
	string_list_clear(&path_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	திரும்பு ret;
}
