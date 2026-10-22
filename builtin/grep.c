/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "tree-walk.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "grep.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "pathspec.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "submodule-config.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "odb/source.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "oidset.h"
#અન્તર્ભાવ "pager.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "promisor-remote.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "write-or-die.h"

સ્થિર અચલ અક્ષર *grep_prefix;

સ્થિર અક્ષર અચલ * અચલ grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	ખાલી
};

સ્થિર પૂર્ણાંક recurse_submodules;

સ્થિર પૂર્ણાંક અંક_threads;

સ્થિર pthread_t *threads;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
સંરચના work_item {
	સંરચના grep_source source;
	અક્ષર done;
	સંરચના strbuf out;
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
સ્થિર સંરચના work_item todo[TODO_SIZE];
સ્થિર પૂર્ણાંક todo_start;
સ્થિર પૂર્ણાંક todo_end;
સ્થિર પૂર્ણાંક todo_done;

/* Has all work items been added? */
સ્થિર પૂર્ણાંક all_work_added;

સ્થિર સંરચના repository **repos_to_free;
સ્થિર size_t repos_to_free_nr, repos_to_free_alloc;

/* This lock protects all the variables above. */
સ્થિર pthread_mutex_t grep_mutex;

સ્થિર અંતઃરેખ શૂન્ય grep_lock(શૂન્ય)
{
	pthread_mutex_lock(&grep_mutex);
}

સ્થિર અંતઃરેખ શૂન્ય grep_unlock(શૂન્ય)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
સ્થિર pthread_cond_t cond_add;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
સ્થિર pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
સ્થિર pthread_cond_t cond_result;

સ્થિર પૂર્ણાંક skip_first_લીટી;

સ્થિર શૂન્ય add_work(સંરચના grep_opt *opt, સંરચના grep_source *gs)
{
	જો (opt->દ્વિઅંકી != GREP_દ્વિઅંકી_TEXT)
		grep_source_load_driver(gs, opt->repo->index);

	grep_lock();

	જ્યારે ((todo_end+1) % ARRAY_SIZE(todo) == todo_done) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_end].source = *gs;
	todo[todo_end].done = 0;
	strbuf_reset(&todo[todo_end].out);
	todo_end = (todo_end + 1) % ARRAY_SIZE(todo);

	pthread_cond_signal(&cond_add);
	grep_unlock();
}

સ્થિર સંરચના work_item *get_work(શૂન્ય)
{
	સંરચના work_item *ret;

	grep_lock();
	જ્યારે (todo_start == todo_end && !all_work_added) {
		pthread_cond_wait(&cond_add, &grep_mutex);
	}

	જો (todo_start == todo_end && all_work_added) {
		ret = ખાલી;
	} નહીંતર {
		ret = &todo[todo_start];
		todo_start = (todo_start + 1) % ARRAY_SIZE(todo);
	}
	grep_unlock();
	પાછા ret;
}

સ્થિર શૂન્ય work_done(સંરચના work_item *w)
{
	પૂર્ણાંક old_done;

	grep_lock();
	w->done = 1;
	old_done = todo_done;
	માટે(; todo[todo_done].done && todo_done != todo_start;
	    todo_done = (todo_done+1) % ARRAY_SIZE(todo)) {
		w = &todo[todo_done];
		જો (w->out.len) {
			અચલ અક્ષર *p = w->out.buf;
			size_t len = w->out.len;

			/* Skip the leading hunk mark of the first file. */
			જો (skip_first_લીટી) {
				જ્યારે (len) {
					len--;
					જો (*p++ == '\n')
						તોડો;
				}
				skip_first_લીટી = 0;
			}

			write_or_die(1, p, len);
		}
		grep_source_clear(&w->source);
	}

	જો (old_done != todo_done)
		pthread_cond_signal(&cond_write);

	જો (all_work_added && todo_done == todo_end)
		pthread_cond_signal(&cond_result);

	grep_unlock();
}

સ્થિર શૂન્ય free_repos(શૂન્ય)
{
	પૂર્ણાંક i;

	માટે (i = 0; i < repos_to_free_nr; i++) {
		repo_clear(repos_to_free[i]);
		મુક્ત(repos_to_free[i]);
	}
	FREE_AND_NULL(repos_to_free);
	repos_to_free_nr = 0;
	repos_to_free_alloc = 0;
}

સ્થિર શૂન્ય *run(શૂન્ય *arg)
{
	પૂર્ણાંક hit = 0;
	સંરચના grep_opt *opt = arg;

	જ્યારે (1) {
		સંરચના work_item *w = get_work();
		જો (!w)
			તોડો;

		opt->output_priv = w;
		hit |= grep_source(opt, &w->source);
		grep_source_clear_data(&w->source);
		work_done(w);
	}
	free_grep_patterns(opt);
	મુક્ત(opt);

	પાછા (શૂન્ય*) (intptr_t) hit;
}

સ્થિર શૂન્ય strbuf_out(સંરચના grep_opt *opt, અચલ શૂન્ય *buf, size_t size)
{
	સંરચના work_item *w = opt->output_priv;
	strbuf_add(&w->out, buf, size);
}

સ્થિર શૂન્ય start_threads(સંરચના grep_opt *opt)
{
	પૂર્ણાંક i;

	pthread_mutex_init(&grep_mutex, ખાલી);
	pthread_mutex_init(&grep_attr_mutex, ખાલી);
	pthread_cond_init(&cond_add, ખાલી);
	pthread_cond_init(&cond_write, ખાલી);
	pthread_cond_init(&cond_result, ખાલી);
	grep_use_locks = 1;
	enable_obj_read_lock();

	માટે (i = 0; i < ARRAY_SIZE(todo); i++) {
		strbuf_init(&todo[i].out, 0);
	}

	CALLOC_ARRAY(threads, અંક_threads);
	માટે (i = 0; i < અંક_threads; i++) {
		પૂર્ણાંક err;
		સંરચના grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_out;
		compile_grep_patterns(o);
		err = pthread_create(&threads[i], ખાલી, run, o);

		જો (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

સ્થિર પૂર્ણાંક wait_all(શૂન્ય)
{
	પૂર્ણાંક hit = 0;
	પૂર્ણાંક i;

	જો (!HAVE_THREADS)
		BUG("Never call this function unless you have started threads");

	grep_lock();
	all_work_added = 1;

	/* Wait until all work is done. */
	જ્યારે (todo_done != todo_end)
		pthread_cond_wait(&cond_result, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_broadcast(&cond_add);
	grep_unlock();

	માટે (i = 0; i < અંક_threads; i++) {
		શૂન્ય *h;
		pthread_join(threads[i], &h);
		hit |= (પૂર્ણાંક) (intptr_t) h;
	}

	મુક્ત(threads);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_add);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_result);
	grep_use_locks = 0;
	disable_obj_read_lock();

	પાછા hit;
}

સ્થિર પૂર્ણાંક grep_cmd_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			   અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	પૂર્ણાંક st = grep_config(var, કિંમત, ctx, cb);

	જો (git_color_config(var, કિંમત, cb) < 0)
		st = -1;
	નહીંતર જો (git_default_config(var, કિંમત, ctx, cb) < 0)
		st = -1;

	જો (!strcmp(var, "grep.threads")) {
		અંક_threads = git_config_int(var, કિંમત, ctx->kvi);
		જો (અંક_threads < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    અંક_threads, var);
		નહીંતર જો (!HAVE_THREADS && અંક_threads > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			અંક_threads = 1;
		}
	}

	જો (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, કિંમત);

	પાછા st;
}

સ્થિર શૂન્ય grep_source_name(સંરચના grep_opt *opt, અચલ અક્ષર *filename,
			     પૂર્ણાંક વૃક્ષ_name_len, સંરચના strbuf *out)
{
	strbuf_reset(out);

	જો (opt->null_following_name) {
		જો (opt->relative && grep_prefix) {
			સંરચના strbuf rel_buf = STRBUF_INIT;
			અચલ અક્ષર *rel_name =
				relative_path(filename + વૃક્ષ_name_len,
					      grep_prefix, &rel_buf);

			જો (વૃક્ષ_name_len)
				strbuf_add(out, filename, વૃક્ષ_name_len);

			strbuf_addstr(out, rel_name);
			strbuf_release(&rel_buf);
		} નહીંતર {
			strbuf_addstr(out, filename);
		}
		પાછા;
	}

	જો (opt->relative && grep_prefix)
		quote_path(filename + વૃક્ષ_name_len, grep_prefix, out, 0);
	નહીંતર
		quote_c_style(filename + વૃક્ષ_name_len, out, ખાલી, 0);

	જો (વૃક્ષ_name_len)
		strbuf_ઉમેરો(out, 0, filename, વૃક્ષ_name_len);
}

સ્થિર પૂર્ણાંક grep_oid(સંરચના grep_opt *opt, અચલ સંરચના object_id *oid,
		     અચલ અક્ષર *filename, પૂર્ણાંક વૃક્ષ_name_len,
		     અચલ અક્ષર *path)
{
	સંરચના strbuf pathbuf = STRBUF_INIT;
	સંરચના grep_source gs;

	grep_source_name(opt, filename, વૃક્ષ_name_len, &pathbuf);
	grep_source_init_oid(&gs, pathbuf.buf, path, oid, opt->repo);
	strbuf_release(&pathbuf);

	જો (અંક_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		પાછા 0;
	} નહીંતર {
		પૂર્ણાંક hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		પાછા hit;
	}
}

સ્થિર પૂર્ણાંક grep_file(સંરચના grep_opt *opt, અચલ અક્ષર *filename)
{
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના grep_source gs;

	grep_source_name(opt, filename, 0, &buf);
	grep_source_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	જો (અંક_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		પાછા 0;
	} નહીંતર {
		પૂર્ણાંક hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		પાછા hit;
	}
}

સ્થિર શૂન્ય append_path(સંરચના grep_opt *opt, અચલ શૂન્ય *data, size_t len)
{
	સંરચના string_list *path_list = opt->output_priv;

	જો (len == 1 && *(અચલ અક્ષર *)data == '\0')
		પાછા;
	string_list_append_nodup(path_list, xstrndup(data, len));
}

સ્થિર શૂન્ય run_pager(સંરચના grep_opt *opt, અચલ અક્ષર *prefix)
{
	સંરચના string_list *path_list = opt->output_priv;
	સંરચના child_process child = CHILD_PROCESS_INIT;
	પૂર્ણાંક i, status;

	માટે (i = 0; i < path_list->nr; i++)
		strvec_ધકેલો(&child.args, path_list->વસ્તુઓ[i].string);
	child.dir = prefix;
	child.use_shell = 1;

	status = run_command(&child);
	જો (status)
		exit(status);
}

સ્થિર પૂર્ણાંક grep_cache(સંરચના grep_opt *opt,
		      અચલ સંરચના pathspec *pathspec, પૂર્ણાંક cached);
સ્થિર પૂર્ણાંક grep_વૃક્ષ(સંરચના grep_opt *opt, અચલ સંરચના pathspec *pathspec,
		     સંરચના વૃક્ષ_desc *વૃક્ષ, સંરચના strbuf *base, પૂર્ણાંક tn_len,
		     પૂર્ણાંક check_attr);

સ્થિર પૂર્ણાંક grep_submodule(સંરચના grep_opt *opt,
			  અચલ સંરચના pathspec *pathspec,
			  અચલ સંરચના object_id *oid,
			  અચલ અક્ષર *filename, અચલ અક્ષર *path, પૂર્ણાંક cached)
{
	સંરચના repository *subrepo;
	સંરચના repository *superproject = opt->repo;
	સંરચના grep_opt subopt;
	પૂર્ણાંક hit = 0;

	જો (!is_submodule_active(superproject, path))
		પાછા 0;

	subrepo = xmalloc(કદ(*subrepo));
	જો (repo_submodule_init(subrepo, superproject, path, null_oid(opt->repo->hash_algo))) {
		મુક્ત(subrepo);
		પાછા 0;
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

	memcpy(&subopt, opt, કદ(subopt));
	subopt.repo = subrepo;

	જો (oid) {
		ગણના object_type object_type;
		સંરચના વૃક્ષ_desc વૃક્ષ;
		શૂન્ય *data;
		size_t size;
		સંરચના strbuf base = STRBUF_INIT;

		obj_read_lock();
		object_type = odb_read_object_info(subrepo->objects, oid, ખાલી);
		obj_read_unlock();
		data = odb_read_object_peeled(subrepo->objects, oid, OBJ_વૃક્ષ, &size, ખાલી);
		જો (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(oid));

		strbuf_addstr(&base, filename);
		strbuf_addch(&base, '/');

		init_વૃક્ષ_desc(&વૃક્ષ, oid, data, size);
		hit = grep_વૃક્ષ(&subopt, pathspec, &વૃક્ષ, &base, base.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&base);
		મુક્ત(data);
	} નહીંતર {
		hit = grep_cache(&subopt, pathspec, cached);
	}

	પાછા hit;
}

સ્થિર પૂર્ણાંક grep_cache(સંરચના grep_opt *opt,
		      અચલ સંરચના pathspec *pathspec, પૂર્ણાંક cached)
{
	સંરચના repository *repo = opt->repo;
	પૂર્ણાંક hit = 0;
	પૂર્ણાંક nr;
	સંરચના strbuf name = STRBUF_INIT;
	પૂર્ણાંક name_base_len = 0;
	જો (repo->submodule_prefix) {
		name_base_len = strlen(repo->submodule_prefix);
		strbuf_addstr(&name, repo->submodule_prefix);
	}

	જો (repo_read_index(repo) < 0)
		die(_("index file corrupt"));

	માટે (nr = 0; nr < repo->index->cache_nr; nr++) {
		અચલ સંરચના cache_entry *ce = repo->index->cache[nr];

		જો (!cached && ce_skip_worktree(ce))
			ચાલુ;

		strbuf_setlen(&name, name_base_len);
		strbuf_addstr(&name, ce->name);
		જો (S_ISSPARSEDIR(ce->ce_mode)) {
			ગણના object_type type;
			સંરચના વૃક્ષ_desc વૃક્ષ;
			શૂન્ય *data;
			size_t size;

			data = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &size);
			જો (!data)
				die(_("unable to read tree %s"), oid_to_hex(&ce->oid));
			init_વૃક્ષ_desc(&વૃક્ષ, &ce->oid, data, size);

			hit |= grep_વૃક્ષ(opt, pathspec, &વૃક્ષ, &name, 0, 0);
			strbuf_setlen(&name, name_base_len);
			strbuf_addstr(&name, ce->name);
			મુક્ત(data);
		} નહીંતર જો (S_ISREG(ce->ce_mode) &&
		    match_pathspec(repo->index, pathspec, name.buf, name.len, 0, ખાલી,
				   S_ISDIR(ce->ce_mode) ||
				   S_ISGITLINK(ce->ce_mode))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			જો (cached || (ce->ce_flags & CE_VALID)) {
				જો (ce_stage(ce) || ce_intent_to_add(ce))
					ચાલુ;
				hit |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} નહીંતર {
				hit |= grep_file(opt, name.buf);
			}
		} નહીંતર જો (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
			   submodule_path_match(repo->index, pathspec, name.buf, ખાલી)) {
			hit |= grep_submodule(opt, pathspec, ખાલી, ce->name,
					      ce->name, cached);
		} નહીંતર {
			ચાલુ;
		}

		જો (ce_stage(ce)) {
			કરો {
				nr++;
			} જ્યારે (nr < repo->index->cache_nr &&
				 !strcmp(ce->name, repo->index->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		જો (hit && opt->status_only)
			તોડો;
	}

	strbuf_release(&name);
	પાછા hit;
}

સ્થિર પૂર્ણાંક grep_વૃક્ષ(સંરચના grep_opt *opt, અચલ સંરચના pathspec *pathspec,
		     સંરચના વૃક્ષ_desc *વૃક્ષ, સંરચના strbuf *base, પૂર્ણાંક tn_len,
		     પૂર્ણાંક check_attr)
{
	સંરચના repository *repo = opt->repo;
	પૂર્ણાંક hit = 0;
	ગણના interesting match = entry_not_interesting;
	સંરચના name_entry entry;
	પૂર્ણાંક old_baselen = base->len;
	સંરચના strbuf name = STRBUF_INIT;
	પૂર્ણાંક name_base_len = 0;
	જો (repo->submodule_prefix) {
		strbuf_addstr(&name, repo->submodule_prefix);
		name_base_len = name.len;
	}

	જ્યારે (વૃક્ષ_entry(વૃક્ષ, &entry)) {
		પૂર્ણાંક te_len = વૃક્ષ_entry_len(&entry);

		જો (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = વૃક્ષ_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_setlen(&name, name_base_len);

			જો (match == all_entries_not_interesting)
				તોડો;
			જો (match == entry_not_interesting)
				ચાલુ;
		}

		strbuf_add(base, entry.path, te_len);

		જો (S_ISREG(entry.mode)) {
			hit |= grep_oid(opt, &entry.oid, base->buf, tn_len,
					 check_attr ? base->buf + tn_len : ખાલી);
		} નહીંતર જો (S_ISDIR(entry.mode)) {
			ગણના object_type type;
			સંરચના વૃક્ષ_desc sub;
			શૂન્ય *data;
			size_t size;

			data = odb_read_object(the_repository->objects,
					       &entry.oid, &type, &size);
			જો (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_વૃક્ષ_desc(&sub, &entry.oid, data, size);
			hit |= grep_વૃક્ષ(opt, pathspec, &sub, base, tn_len,
					 check_attr);
			મુક્ત(data);
		} નહીંતર જો (recurse_submodules && S_ISGITLINK(entry.mode)) {
			hit |= grep_submodule(opt, pathspec, &entry.oid,
					      base->buf, base->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(base, old_baselen);

		જો (hit && opt->status_only)
			તોડો;
	}

	strbuf_release(&name);
	પાછા hit;
}

સ્થિર શૂન્ય collect_blob_oids_for_વૃક્ષ(સંરચના repository *repo,
				       અચલ સંરચના pathspec *pathspec,
				       સંરચના વૃક્ષ_desc *વૃક્ષ,
				       સંરચના strbuf *base,
				       પૂર્ણાંક tn_len,
				       સંરચના oidset *blob_oids)
{
	સંરચના name_entry entry;
	પૂર્ણાંક old_baselen = base->len;
	સંરચના strbuf name = STRBUF_INIT;
	ગણના interesting match = entry_not_interesting;

	જ્યારે (વૃક્ષ_entry(વૃક્ષ, &entry)) {
		જો (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = વૃક્ષ_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_reset(&name);

			જો (match == all_entries_not_interesting)
				તોડો;
			જો (match == entry_not_interesting)
				ચાલુ;
		}

		strbuf_add(base, entry.path, વૃક્ષ_entry_len(&entry));

		જો (S_ISREG(entry.mode)) {
			જો (!odb_has_object(repo->objects, &entry.oid, 0))
				oidset_ઉમેરો(blob_oids, &entry.oid);
		} નહીંતર જો (S_ISDIR(entry.mode)) {
			ગણના object_type type;
			સંરચના વૃક્ષ_desc sub_વૃક્ષ;
			શૂન્ય *data;
			size_t size;

			data = odb_read_object(repo->objects, &entry.oid,
					       &type, &size);
			જો (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_વૃક્ષ_desc(&sub_વૃક્ષ, &entry.oid, data, size);
			collect_blob_oids_for_વૃક્ષ(repo, pathspec, &sub_વૃક્ષ,
						   base, tn_len, blob_oids);
			મુક્ત(data);
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

સ્થિર શૂન્ય collect_blob_oids_for_treeish(સંરચના grep_opt *opt,
					  અચલ સંરચના pathspec *pathspec,
					  અચલ સંરચના object_id *વૃક્ષ_ish_oid,
					  અચલ અક્ષર *name,
					  સંરચના oidset *blob_oids)
{
	સંરચના વૃક્ષ_desc વૃક્ષ;
	શૂન્ય *data;
	size_t size;
	સંરચના strbuf base = STRBUF_INIT;
	પૂર્ણાંક len;

	data = odb_read_object_peeled(opt->repo->objects, વૃક્ષ_ish_oid,
				      OBJ_વૃક્ષ, &size, ખાલી);

	જો (!data)
		પાછા;

	len = name ? strlen(name) : 0;
	જો (len) {
		strbuf_add(&base, name, len);
		strbuf_addch(&base, ':');
	}
	init_વૃક્ષ_desc(&વૃક્ષ, વૃક્ષ_ish_oid, data, size);

	collect_blob_oids_for_વૃક્ષ(opt->repo, pathspec, &વૃક્ષ,
				   &base, base.len, blob_oids);

	strbuf_release(&base);
	મુક્ત(data);
}

સ્થિર શૂન્ય prefetch_grep_blobs(સંરચના grep_opt *opt,
				અચલ સંરચના pathspec *pathspec,
				અચલ સંરચના object_array *list)
{
	સંરચના oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	જો (!repo_has_promisor_remote(opt->repo))
		પાછા;

	/* For each tree, gather the blobs in it */
	માટે (પૂર્ણાંક i = 0; i < list->nr; i++) {
		સંરચના object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     ખાલી, 0);
		obj_read_unlock();

		જો (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_વૃક્ષ))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	જો (oidset_size(&blob_oids)) {
		સંરચના oid_array to_fetch = OID_ARRAY_INIT;
		સંરચના oidset_iter iter;
		અચલ સંરચના object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		જ્યારે ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(opt->repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}
	oidset_clear(&blob_oids);
}

સ્થિર પૂર્ણાંક grep_object(સંરચના grep_opt *opt, અચલ સંરચના pathspec *pathspec,
		       સંરચના object *obj, અચલ અક્ષર *name, અચલ અક્ષર *path)
{
	જો (obj->type == OBJ_BLOB)
		પાછા grep_oid(opt, &obj->oid, name, 0, path);
	જો (obj->type == OBJ_COMMIT || obj->type == OBJ_વૃક્ષ) {
		સંરચના વૃક્ષ_desc વૃક્ષ;
		શૂન્ય *data;
		size_t size;
		સંરચના strbuf base;
		પૂર્ણાંક hit, len;

		data = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_વૃક્ષ, &size, ખાલી);
		જો (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(&obj->oid));

		len = name ? strlen(name) : 0;
		strbuf_init(&base, PATH_MAX + len + 1);
		જો (len) {
			strbuf_add(&base, name, len);
			strbuf_addch(&base, ':');
		}
		init_વૃક્ષ_desc(&વૃક્ષ, &obj->oid, data, size);
		hit = grep_વૃક્ષ(opt, pathspec, &વૃક્ષ, &base, base.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&base);
		મુક્ત(data);
		પાછા hit;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

સ્થિર પૂર્ણાંક grep_objects(સંરચના grep_opt *opt, અચલ સંરચના pathspec *pathspec,
			અચલ સંરચના object_array *list)
{
	unsigned પૂર્ણાંક i;
	પૂર્ણાંક hit = 0;
	અચલ unsigned પૂર્ણાંક nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	માટે (i = 0; i < nr; i++) {
		સંરચના object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     ખાલી, 0);
		obj_read_unlock();

		જો (!real_obj) {
			અક્ષર hex[GIT_MAX_HEXSZ + 1];
			અચલ અક્ષર *name = list->objects[i].name;

			જો (!name) {
				oid_to_hex_r(hex, &list->objects[i].item->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		જો (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_lock();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		જો (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].path)) {
			hit = 1;
			જો (opt->status_only)
				તોડો;
		}
	}
	પાછા hit;
}

સ્થિર પૂર્ણાંક grep_directory(સંરચના grep_opt *opt, અચલ સંરચના pathspec *pathspec,
			  પૂર્ણાંક exc_std, પૂર્ણાંક use_index)
{
	સંરચના dir_struct dir = DIR_INIT;
	પૂર્ણાંક i, hit = 0;

	જો (!use_index)
		dir.flags |= DIR_NO_GITLINKS;
	જો (exc_std)
		setup_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->index, pathspec);
	માટે (i = 0; i < dir.nr; i++) {
		hit |= grep_file(opt, dir.entries[i]->name);
		જો (hit && opt->status_only)
			તોડો;
	}
	dir_clear(&dir);
	પાછા hit;
}

સ્થિર પૂર્ણાંક context_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			    પૂર્ણાંક unset)
{
	સંરચના grep_opt *grep_opt = opt->કિંમત;
	પૂર્ણાંક કિંમત;
	અચલ અક્ષર *endp;

	જો (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		પાછા 0;
	}
	કિંમત = strtol(arg, (અક્ષર **)&endp, 10);
	જો (*endp) {
		પાછા error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = કિંમત;
	પાછા 0;
}

સ્થિર પૂર્ણાંક file_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના grep_opt *grep_opt = opt->કિંમત;
	પૂર્ણાંક from_stdin;
	અચલ અક્ષર *filename = arg;
	FILE *patterns;
	પૂર્ણાંક lno = 0;
	સંરચના strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	જો (!*filename)
		; /* leave it as-is */
	નહીંતર
		filename = prefix_filename_except_for_dash(grep_prefix, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	જો (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	જ્યારે (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		જો (sb.len == 0)
			ચાલુ;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_PATTERN);
	}
	જો (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	જો (filename != arg)
		મુક્ત((શૂન્ય *)filename);
	પાછા 0;
}

સ્થિર પૂર્ણાંક not_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના grep_opt *grep_opt = opt->કિંમત;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--not", "command line", 0, GREP_NOT);
	પાછા 0;
}

સ્થિર પૂર્ણાંક and_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના grep_opt *grep_opt = opt->કિંમત;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--and", "command line", 0, GREP_AND);
	પાછા 0;
}

સ્થિર પૂર્ણાંક open_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના grep_opt *grep_opt = opt->કિંમત;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	પાછા 0;
}

સ્થિર પૂર્ણાંક close_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના grep_opt *grep_opt = opt->કિંમત;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	પાછા 0;
}

સ્થિર પૂર્ણાંક pattern_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			    પૂર્ણાંક unset)
{
	સંરચના grep_opt *grep_opt = opt->કિંમત;
	BUG_ON_OPT_NEG(unset);
	append_grep_pattern(grep_opt, arg, "-e option", 0, GREP_PATTERN);
	પાછા 0;
}

પૂર્ણાંક cmd_grep(પૂર્ણાંક argc,
	     અચલ અક્ષર **argv,
	     અચલ અક્ષર *prefix,
	     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક hit = 0;
	પૂર્ણાંક cached = 0, untracked = 0, opt_exclude = -1;
	પૂર્ણાંક seen_dashdash = 0;
	પૂર્ણાંક external_grep_allowed__ignored;
	અચલ અક્ષર *show_in_pager = ખાલી, *default_pager = "dummy";
	સંરચના grep_opt opt;
	સંરચના object_array list = OBJECT_ARRAY_INIT;
	સંરચના pathspec pathspec;
	સંરચના string_list path_list = STRING_LIST_INIT_DUP;
	પૂર્ણાંક i;
	પૂર્ણાંક dummy;
	પૂર્ણાંક use_index = 1;
	પૂર્ણાંક allow_revs;
	પૂર્ણાંક ret;

	સંરચના option options[] = {
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
		OPT_SET_INT('a', "text", &opt.દ્વિઅંકી,
			N_("process binary files as text"), GREP_દ્વિઅંકી_TEXT),
		OPT_SET_INT('I', ખાલી, &opt.દ્વિઅંકી,
			N_("don't match patterns in binary files"),
			GREP_દ્વિઅંકી_NOMATCH),
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
		OPT_NEGBIT('h', ખાલી, &opt.pathname, N_("don't show filenames"), 1),
		OPT_BIT('H', ખાલી, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.ગણતરી,
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
		OPT_INTEGER(0, "threads", &અંક_threads,
			N_("use <n> worker threads")),
		OPT_સંખ્યા_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_GROUP(""),
		OPT_CALLBACK('f', ખાલી, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', ખાલી, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, pattern_callback),
		OPT_CALLBACK_F(0, "and", &opt, ખાલી,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, ખાલી, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', ખાલી, &opt, ખાલી, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', ખાલી, &opt, ખાલી, "",
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
			.કિંમત = &show_in_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_ગણતરી,
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

	જો (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	જો (use_index && !startup_info->have_repository) {
		પૂર્ણાંક fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		જો (fallback)
			use_index = 0;
		નહીંતર
			/* die the same way as if we did it at the beginning */
			setup_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	જો (!use_index)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	જો (argc > 0 && !opt.pattern_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	જો (argc > 0 && !opt.pattern_list) {
		append_grep_pattern(&opt, argv[0], "command line", 0,
				    GREP_PATTERN);
		argv++;
		argc--;
	}

	જો (show_in_pager == default_pager)
		show_in_pager = git_pager(the_repository, 1);
	જો (show_in_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &path_list;
		opt.output = append_path;
		string_list_append(&path_list, show_in_pager);
	}

	જો (!opt.pattern_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	જો (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	માટે (i = 0; i < argc; i++) {
		જો (!strcmp(argv[i], "--")) {
			seen_dashdash = 1;
			તોડો;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_index && !untracked;
	માટે (i = 0; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];
		સંરચના object_id oid;
		સંરચના object_context oc = {0};
		સંરચના object *object;

		જો (!strcmp(arg, "--")) {
			i++;
			તોડો;
		}

		જો (!allow_revs) {
			જો (seen_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			તોડો;
		}

		જો (get_oid_with_context(the_repository, arg,
					 GET_OID_RECORD_PATH,
					 &oid, &oc)) {
			જો (seen_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			તોડો;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		જો (!seen_dashdash)
			verify_non_filename(the_repository, prefix, arg);
		add_object_array_with_path(object, arg, &list, oc.mode, oc.path);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	જો (!seen_dashdash) {
		પૂર્ણાંક j;
		માટે (j = i; j < argc; j++)
			verify_filename(the_repository, prefix, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_depth != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       prefix, argv + i);
	pathspec.max_depth = opt.max_depth;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	જો (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	જો (opt.max_ગણતરી == 0) {
		ret = 1;
		જાઓ out;
	}

	જો (show_in_pager) {
		જો (અંક_threads > 1)
			warning(_("invalid option combination, ignoring --threads"));
		અંક_threads = 1;
	} નહીંતર જો (!HAVE_THREADS && અંક_threads > 1) {
		warning(_("no threads support, ignoring --threads"));
		અંક_threads = 1;
	} નહીંતર જો (અંક_threads < 0)
		die(_("invalid number of threads specified (%d)"), અંક_threads);
	નહીંતર જો (અંક_threads == 0)
		અંક_threads = HAVE_THREADS ? online_cpus() : 1;

	જો (અંક_threads > 1) {
		જો (!HAVE_THREADS)
			BUG("Somebody got num_threads calculation wrong!");
		જો (!(opt.name_only || opt.unmatch_name_only || opt.ગણતરી)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_first_લીટી = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		જો (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		જો (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		start_threads(&opt);
	} નહીંતર {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	જો (show_in_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	જો (show_in_pager && opt.pattern_list && !opt.pattern_list->next) {
		અચલ અક્ષર *pager = path_list.વસ્તુઓ[0].string;
		પૂર્ણાંક len = strlen(pager);

		જો (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		જો (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&path_list, "-I");

		જો (!strcmp("less", pager) || !strcmp("vi", pager)) {
			સંરચના strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.pattern_list->pattern);
			string_list_append_nodup(&path_list,
						 strbuf_detach(&buf, ખાલી));
		}
	}

	જો (!show_in_pager && !opt.status_only)
		setup_pager(the_repository);

	die_for_incompatible_opt3(!use_index, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	જો (!use_index || untracked) {
		પૂર્ણાંક use_exclude = (opt_exclude < 0) ? use_index : !!opt_exclude;
		hit = grep_directory(&opt, &pathspec, use_exclude, use_index);
	} નહીંતર જો (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} નહીંતર જો (!list.nr) {
		જો (!cached)
			setup_work_વૃક્ષ(the_repository);

		hit = grep_cache(&opt, &pathspec, cached);
	} નહીંતર {
		જો (cached)
			die(_("both --cached and trees are given"));

		hit = grep_objects(&opt, &pathspec, &list);
	}

	જો (અંક_threads > 1)
		hit |= wait_all();
	જો (hit && show_in_pager)
		run_pager(&opt, prefix);

	ret = !hit;

out:
	clear_pathspec(&pathspec);
	string_list_clear(&path_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	પાછા ret;
}
