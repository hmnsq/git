/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "abspath.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "config.h"
#include "tag.h"
#include "tree-walk.h"
#include "parse-options.h"
#include "string-list.h"
#include "run-command.h"
#include "grep.h"
#include "quote.h"
#include "dir.h"
#include "pathspec.h"
#include "setup.h"
#include "submodule.h"
#include "submodule-config.h"
#include "object-name.h"
#include "odb.h"
#include "odb/source.h"
#include "oid-array.h"
#include "oidset.h"
#include "pager.h"
#include "path.h"
#include "promisor-remote.h"
#include "read-cache-ll.h"
#include "write-or-die.h"

static const var_na *grep_prefix;

static var_na const * const grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	NULL
};

static පූර්ණ_සංඛ්‍යා recurse_submodules;

static පූර්ණ_සංඛ්‍යා num_threads;

static pthread_t *threads;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
sa_nracanaa work_item {
	sa_nracanaa grep_source source;
	var_na done;
	sa_nracanaa strbuf out;
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
static sa_nracanaa work_item todo[TODO_SIZE];
static පූර්ණ_සංඛ්‍යා todo_start;
static පූර්ණ_සංඛ්‍යා todo_end;
static පූර්ණ_සංඛ්‍යා todo_done;

/* Has all work items been added? */
static පූර්ණ_සංඛ්‍යා all_work_added;

static sa_nracanaa repository **repos_to_free;
static size_t repos_to_free_nr, repos_to_free_alloc;

/* This lock protects all the variables above. */
static pthread_mutex_t grep_mutex;

static anta_hstha void grep_lock(void)
{
	pthread_mutex_lock(&grep_mutex);
}

static anta_hstha void grep_unlock(void)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
static pthread_cond_t cond_add;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
static pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
static pthread_cond_t cond_result;

static පූර්ණ_සංඛ්‍යා skip_first_line;

static void add_work(sa_nracanaa grep_opt *opt, sa_nracanaa grep_source *gs)
{
	නම් (opt->binary != GREP_BINARY_TEXT)
		grep_source_load_driver(gs, opt->repo->index);

	grep_lock();

	අතරතුර ((todo_end+1) % ARRAY_SIZE(todo) == todo_done) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_end].source = *gs;
	todo[todo_end].done = 0;
	strbuf_reset(&todo[todo_end].out);
	todo_end = (todo_end + 1) % ARRAY_SIZE(todo);

	pthread_cond_signal(&cond_add);
	grep_unlock();
}

static sa_nracanaa work_item *get_work(void)
{
	sa_nracanaa work_item *ret;

	grep_lock();
	අතරතුර (todo_start == todo_end && !all_work_added) {
		pthread_cond_wait(&cond_add, &grep_mutex);
	}

	නම් (todo_start == todo_end && all_work_added) {
		ret = NULL;
	} නැතහොත් {
		ret = &todo[todo_start];
		todo_start = (todo_start + 1) % ARRAY_SIZE(todo);
	}
	grep_unlock();
	ආපසු ret;
}

static void work_done(sa_nracanaa work_item *w)
{
	පූර්ණ_සංඛ්‍යා old_done;

	grep_lock();
	w->done = 1;
	old_done = todo_done;
	සඳහා(; todo[todo_done].done && todo_done != todo_start;
	    todo_done = (todo_done+1) % ARRAY_SIZE(todo)) {
		w = &todo[todo_done];
		නම් (w->out.len) {
			const var_na *p = w->out.buf;
			size_t len = w->out.len;

			/* Skip the leading hunk mark of the first file. */
			නම් (skip_first_line) {
				අතරතුර (len) {
					len--;
					නම් (*p++ == '\n')
						bha_nga;
				}
				skip_first_line = 0;
			}

			write_or_die(1, p, len);
		}
		grep_source_clear(&w->source);
	}

	නම් (old_done != todo_done)
		pthread_cond_signal(&cond_write);

	නම් (all_work_added && todo_done == todo_end)
		pthread_cond_signal(&cond_result);

	grep_unlock();
}

static void free_repos(void)
{
	පූර්ණ_සංඛ්‍යා i;

	සඳහා (i = 0; i < repos_to_free_nr; i++) {
		repo_clear(repos_to_free[i]);
		free(repos_to_free[i]);
	}
	FREE_AND_NULL(repos_to_free);
	repos_to_free_nr = 0;
	repos_to_free_alloc = 0;
}

static void *run(void *arg)
{
	පූර්ණ_සංඛ්‍යා hit = 0;
	sa_nracanaa grep_opt *opt = arg;

	අතරතුර (1) {
		sa_nracanaa work_item *w = get_work();
		නම් (!w)
			bha_nga;

		opt->output_priv = w;
		hit |= grep_source(opt, &w->source);
		grep_source_clear_data(&w->source);
		work_done(w);
	}
	free_grep_patterns(opt);
	free(opt);

	ආපසු (void*) (intptr_t) hit;
}

static void strbuf_out(sa_nracanaa grep_opt *opt, const void *buf, size_t size)
{
	sa_nracanaa work_item *w = opt->output_priv;
	strbuf_add(&w->out, buf, size);
}

static void start_threads(sa_nracanaa grep_opt *opt)
{
	පූර්ණ_සංඛ්‍යා i;

	pthread_mutex_init(&grep_mutex, NULL);
	pthread_mutex_init(&grep_attr_mutex, NULL);
	pthread_cond_init(&cond_add, NULL);
	pthread_cond_init(&cond_write, NULL);
	pthread_cond_init(&cond_result, NULL);
	grep_use_locks = 1;
	enable_obj_read_lock();

	සඳහා (i = 0; i < ARRAY_SIZE(todo); i++) {
		strbuf_init(&todo[i].out, 0);
	}

	CALLOC_ARRAY(threads, num_threads);
	සඳහා (i = 0; i < num_threads; i++) {
		පූර්ණ_සංඛ්‍යා err;
		sa_nracanaa grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_out;
		compile_grep_patterns(o);
		err = pthread_create(&threads[i], NULL, run, o);

		නම් (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

static පූර්ණ_සංඛ්‍යා wait_all(void)
{
	පූර්ණ_සංඛ්‍යා hit = 0;
	පූර්ණ_සංඛ්‍යා i;

	නම් (!HAVE_THREADS)
		BUG("Never call this function unless you have started threads");

	grep_lock();
	all_work_added = 1;

	/* Wait until all work is done. */
	අතරතුර (todo_done != todo_end)
		pthread_cond_wait(&cond_result, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_broadcast(&cond_add);
	grep_unlock();

	සඳහා (i = 0; i < num_threads; i++) {
		void *h;
		pthread_join(threads[i], &h);
		hit |= (පූර්ණ_සංඛ්‍යා) (intptr_t) h;
	}

	free(threads);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_add);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_result);
	grep_use_locks = 0;
	disable_obj_read_lock();

	ආපසු hit;
}

static පූර්ණ_සංඛ්‍යා grep_cmd_config(const var_na *var, const var_na *value,
			   const sa_nracanaa config_context *ctx, void *cb)
{
	පූර්ණ_සංඛ්‍යා st = grep_config(var, value, ctx, cb);

	නම් (git_color_config(var, value, cb) < 0)
		st = -1;
	නැතහොත් නම් (git_default_config(var, value, ctx, cb) < 0)
		st = -1;

	නම් (!strcmp(var, "grep.threads")) {
		num_threads = git_config_int(var, value, ctx->kvi);
		නම් (num_threads < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    num_threads, var);
		නැතහොත් නම් (!HAVE_THREADS && num_threads > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			num_threads = 1;
		}
	}

	නම් (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, value);

	ආපසු st;
}

static void grep_source_name(sa_nracanaa grep_opt *opt, const var_na *filename,
			     පූර්ණ_සංඛ්‍යා tree_name_len, sa_nracanaa strbuf *out)
{
	strbuf_reset(out);

	නම් (opt->null_following_name) {
		නම් (opt->relative && grep_prefix) {
			sa_nracanaa strbuf rel_buf = STRBUF_INIT;
			const var_na *rel_name =
				relative_path(filename + tree_name_len,
					      grep_prefix, &rel_buf);

			නම් (tree_name_len)
				strbuf_add(out, filename, tree_name_len);

			strbuf_addstr(out, rel_name);
			strbuf_release(&rel_buf);
		} නැතහොත් {
			strbuf_addstr(out, filename);
		}
		ආපසු;
	}

	නම් (opt->relative && grep_prefix)
		quote_path(filename + tree_name_len, grep_prefix, out, 0);
	නැතහොත්
		quote_c_style(filename + tree_name_len, out, NULL, 0);

	නම් (tree_name_len)
		strbuf_insert(out, 0, filename, tree_name_len);
}

static පූර්ණ_සංඛ්‍යා grep_oid(sa_nracanaa grep_opt *opt, const sa_nracanaa object_id *oid,
		     const var_na *filename, පූර්ණ_සංඛ්‍යා tree_name_len,
		     const var_na *path)
{
	sa_nracanaa strbuf pathbuf = STRBUF_INIT;
	sa_nracanaa grep_source gs;

	grep_source_name(opt, filename, tree_name_len, &pathbuf);
	grep_source_init_oid(&gs, pathbuf.buf, path, oid, opt->repo);
	strbuf_release(&pathbuf);

	නම් (num_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		ආපසු 0;
	} නැතහොත් {
		පූර්ණ_සංඛ්‍යා hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		ආපසු hit;
	}
}

static පූර්ණ_සංඛ්‍යා grep_file(sa_nracanaa grep_opt *opt, const var_na *filename)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa grep_source gs;

	grep_source_name(opt, filename, 0, &buf);
	grep_source_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	නම් (num_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		ආපසු 0;
	} නැතහොත් {
		පූර්ණ_සංඛ්‍යා hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		ආපසු hit;
	}
}

static void append_path(sa_nracanaa grep_opt *opt, const void *data, size_t len)
{
	sa_nracanaa string_list *path_list = opt->output_priv;

	නම් (len == 1 && *(const var_na *)data == '\0')
		ආපසු;
	string_list_append_nodup(path_list, xstrndup(data, len));
}

static void run_pager(sa_nracanaa grep_opt *opt, const var_na *prefix)
{
	sa_nracanaa string_list *path_list = opt->output_priv;
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;
	පූර්ණ_සංඛ්‍යා i, status;

	සඳහා (i = 0; i < path_list->nr; i++)
		strvec_push(&child.args, path_list->items[i].string);
	child.dir = prefix;
	child.use_shell = 1;

	status = run_command(&child);
	නම් (status)
		exit(status);
}

static පූර්ණ_සංඛ්‍යා grep_cache(sa_nracanaa grep_opt *opt,
		      const sa_nracanaa pathspec *pathspec, පූර්ණ_සංඛ්‍යා cached);
static පූර්ණ_සංඛ්‍යා grep_tree(sa_nracanaa grep_opt *opt, const sa_nracanaa pathspec *pathspec,
		     sa_nracanaa tree_desc *tree, sa_nracanaa strbuf *base, පූර්ණ_සංඛ්‍යා tn_len,
		     පූර්ණ_සංඛ්‍යා check_attr);

static පූර්ණ_සංඛ්‍යා grep_submodule(sa_nracanaa grep_opt *opt,
			  const sa_nracanaa pathspec *pathspec,
			  const sa_nracanaa object_id *oid,
			  const var_na *filename, const var_na *path, පූර්ණ_සංඛ්‍යා cached)
{
	sa_nracanaa repository *subrepo;
	sa_nracanaa repository *superproject = opt->repo;
	sa_nracanaa grep_opt subopt;
	පූර්ණ_සංඛ්‍යා hit = 0;

	නම් (!is_submodule_active(superproject, path))
		ආපසු 0;

	subrepo = xmalloc(parimaa_na(*subrepo));
	නම් (repo_submodule_init(subrepo, superproject, path, null_oid(opt->repo->hash_algo))) {
		free(subrepo);
		ආපසු 0;
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

	memcpy(&subopt, opt, parimaa_na(subopt));
	subopt.repo = subrepo;

	නම් (oid) {
		ga_nanaa object_type object_type;
		sa_nracanaa tree_desc tree;
		void *data;
		size_t size;
		sa_nracanaa strbuf base = STRBUF_INIT;

		obj_read_lock();
		object_type = odb_read_object_info(subrepo->objects, oid, NULL);
		obj_read_unlock();
		data = odb_read_object_peeled(subrepo->objects, oid, OBJ_TREE, &size, NULL);
		නම් (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(oid));

		strbuf_addstr(&base, filename);
		strbuf_addch(&base, '/');

		init_tree_desc(&tree, oid, data, size);
		hit = grep_tree(&subopt, pathspec, &tree, &base, base.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&base);
		free(data);
	} නැතහොත් {
		hit = grep_cache(&subopt, pathspec, cached);
	}

	ආපසු hit;
}

static පූර්ණ_සංඛ්‍යා grep_cache(sa_nracanaa grep_opt *opt,
		      const sa_nracanaa pathspec *pathspec, පූර්ණ_සංඛ්‍යා cached)
{
	sa_nracanaa repository *repo = opt->repo;
	පූර්ණ_සංඛ්‍යා hit = 0;
	පූර්ණ_සංඛ්‍යා nr;
	sa_nracanaa strbuf name = STRBUF_INIT;
	පූර්ණ_සංඛ්‍යා name_base_len = 0;
	නම් (repo->submodule_prefix) {
		name_base_len = strlen(repo->submodule_prefix);
		strbuf_addstr(&name, repo->submodule_prefix);
	}

	නම් (repo_read_index(repo) < 0)
		die(_("index file corrupt"));

	සඳහා (nr = 0; nr < repo->index->cache_nr; nr++) {
		const sa_nracanaa cache_entry *ce = repo->index->cache[nr];

		නම් (!cached && ce_skip_worktree(ce))
			continue;

		strbuf_setlen(&name, name_base_len);
		strbuf_addstr(&name, ce->name);
		නම් (S_ISSPARSEDIR(ce->ce_mode)) {
			ga_nanaa object_type type;
			sa_nracanaa tree_desc tree;
			void *data;
			size_t size;

			data = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &size);
			නම් (!data)
				die(_("unable to read tree %s"), oid_to_hex(&ce->oid));
			init_tree_desc(&tree, &ce->oid, data, size);

			hit |= grep_tree(opt, pathspec, &tree, &name, 0, 0);
			strbuf_setlen(&name, name_base_len);
			strbuf_addstr(&name, ce->name);
			free(data);
		} නැතහොත් නම් (S_ISREG(ce->ce_mode) &&
		    match_pathspec(repo->index, pathspec, name.buf, name.len, 0, NULL,
				   S_ISDIR(ce->ce_mode) ||
				   S_ISGITLINK(ce->ce_mode))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			නම් (cached || (ce->ce_flags & CE_VALID)) {
				නම් (ce_stage(ce) || ce_intent_to_add(ce))
					continue;
				hit |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} නැතහොත් {
				hit |= grep_file(opt, name.buf);
			}
		} නැතහොත් නම් (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
			   submodule_path_match(repo->index, pathspec, name.buf, NULL)) {
			hit |= grep_submodule(opt, pathspec, NULL, ce->name,
					      ce->name, cached);
		} නැතහොත් {
			continue;
		}

		නම් (ce_stage(ce)) {
			do {
				nr++;
			} අතරතුර (nr < repo->index->cache_nr &&
				 !strcmp(ce->name, repo->index->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		නම් (hit && opt->status_only)
			bha_nga;
	}

	strbuf_release(&name);
	ආපසු hit;
}

static පූර්ණ_සංඛ්‍යා grep_tree(sa_nracanaa grep_opt *opt, const sa_nracanaa pathspec *pathspec,
		     sa_nracanaa tree_desc *tree, sa_nracanaa strbuf *base, පූර්ණ_සංඛ්‍යා tn_len,
		     පූර්ණ_සංඛ්‍යා check_attr)
{
	sa_nracanaa repository *repo = opt->repo;
	පූර්ණ_සංඛ්‍යා hit = 0;
	ga_nanaa interesting match = entry_not_interesting;
	sa_nracanaa name_entry entry;
	පූර්ණ_සංඛ්‍යා old_baselen = base->len;
	sa_nracanaa strbuf name = STRBUF_INIT;
	පූර්ණ_සංඛ්‍යා name_base_len = 0;
	නම් (repo->submodule_prefix) {
		strbuf_addstr(&name, repo->submodule_prefix);
		name_base_len = name.len;
	}

	අතරතුර (tree_entry(tree, &entry)) {
		පූර්ණ_සංඛ්‍යා te_len = tree_entry_len(&entry);

		නම් (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = tree_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_setlen(&name, name_base_len);

			නම් (match == all_entries_not_interesting)
				bha_nga;
			නම් (match == entry_not_interesting)
				continue;
		}

		strbuf_add(base, entry.path, te_len);

		නම් (S_ISREG(entry.mode)) {
			hit |= grep_oid(opt, &entry.oid, base->buf, tn_len,
					 check_attr ? base->buf + tn_len : NULL);
		} නැතහොත් නම් (S_ISDIR(entry.mode)) {
			ga_nanaa object_type type;
			sa_nracanaa tree_desc sub;
			void *data;
			size_t size;

			data = odb_read_object(the_repository->objects,
					       &entry.oid, &type, &size);
			නම් (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_tree_desc(&sub, &entry.oid, data, size);
			hit |= grep_tree(opt, pathspec, &sub, base, tn_len,
					 check_attr);
			free(data);
		} නැතහොත් නම් (recurse_submodules && S_ISGITLINK(entry.mode)) {
			hit |= grep_submodule(opt, pathspec, &entry.oid,
					      base->buf, base->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(base, old_baselen);

		නම් (hit && opt->status_only)
			bha_nga;
	}

	strbuf_release(&name);
	ආපසු hit;
}

static void collect_blob_oids_for_tree(sa_nracanaa repository *repo,
				       const sa_nracanaa pathspec *pathspec,
				       sa_nracanaa tree_desc *tree,
				       sa_nracanaa strbuf *base,
				       පූර්ණ_සංඛ්‍යා tn_len,
				       sa_nracanaa oidset *blob_oids)
{
	sa_nracanaa name_entry entry;
	පූර්ණ_සංඛ්‍යා old_baselen = base->len;
	sa_nracanaa strbuf name = STRBUF_INIT;
	ga_nanaa interesting match = entry_not_interesting;

	අතරතුර (tree_entry(tree, &entry)) {
		නම් (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = tree_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_reset(&name);

			නම් (match == all_entries_not_interesting)
				bha_nga;
			නම් (match == entry_not_interesting)
				continue;
		}

		strbuf_add(base, entry.path, tree_entry_len(&entry));

		නම් (S_ISREG(entry.mode)) {
			නම් (!odb_has_object(repo->objects, &entry.oid, 0))
				oidset_insert(blob_oids, &entry.oid);
		} නැතහොත් නම් (S_ISDIR(entry.mode)) {
			ga_nanaa object_type type;
			sa_nracanaa tree_desc sub_tree;
			void *data;
			size_t size;

			data = odb_read_object(repo->objects, &entry.oid,
					       &type, &size);
			නම් (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_tree_desc(&sub_tree, &entry.oid, data, size);
			collect_blob_oids_for_tree(repo, pathspec, &sub_tree,
						   base, tn_len, blob_oids);
			free(data);
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

static void collect_blob_oids_for_treeish(sa_nracanaa grep_opt *opt,
					  const sa_nracanaa pathspec *pathspec,
					  const sa_nracanaa object_id *tree_ish_oid,
					  const var_na *name,
					  sa_nracanaa oidset *blob_oids)
{
	sa_nracanaa tree_desc tree;
	void *data;
	size_t size;
	sa_nracanaa strbuf base = STRBUF_INIT;
	පූර්ණ_සංඛ්‍යා len;

	data = odb_read_object_peeled(opt->repo->objects, tree_ish_oid,
				      OBJ_TREE, &size, NULL);

	නම් (!data)
		ආපසු;

	len = name ? strlen(name) : 0;
	නම් (len) {
		strbuf_add(&base, name, len);
		strbuf_addch(&base, ':');
	}
	init_tree_desc(&tree, tree_ish_oid, data, size);

	collect_blob_oids_for_tree(opt->repo, pathspec, &tree,
				   &base, base.len, blob_oids);

	strbuf_release(&base);
	free(data);
}

static void prefetch_grep_blobs(sa_nracanaa grep_opt *opt,
				const sa_nracanaa pathspec *pathspec,
				const sa_nracanaa object_array *list)
{
	sa_nracanaa oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	නම් (!repo_has_promisor_remote(opt->repo))
		ආපසු;

	/* For each tree, gather the blobs in it */
	සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < list->nr; i++) {
		sa_nracanaa object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     NULL, 0);
		obj_read_unlock();

		නම් (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_TREE))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	නම් (oidset_size(&blob_oids)) {
		sa_nracanaa oid_array to_fetch = OID_ARRAY_INIT;
		sa_nracanaa oidset_iter iter;
		const sa_nracanaa object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		අතරතුර ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(opt->repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}
	oidset_clear(&blob_oids);
}

static පූර්ණ_සංඛ්‍යා grep_object(sa_nracanaa grep_opt *opt, const sa_nracanaa pathspec *pathspec,
		       sa_nracanaa object *obj, const var_na *name, const var_na *path)
{
	නම් (obj->type == OBJ_BLOB)
		ආපසු grep_oid(opt, &obj->oid, name, 0, path);
	නම් (obj->type == OBJ_COMMIT || obj->type == OBJ_TREE) {
		sa_nracanaa tree_desc tree;
		void *data;
		size_t size;
		sa_nracanaa strbuf base;
		පූර්ණ_සංඛ්‍යා hit, len;

		data = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_TREE, &size, NULL);
		නම් (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(&obj->oid));

		len = name ? strlen(name) : 0;
		strbuf_init(&base, PATH_MAX + len + 1);
		නම් (len) {
			strbuf_add(&base, name, len);
			strbuf_addch(&base, ':');
		}
		init_tree_desc(&tree, &obj->oid, data, size);
		hit = grep_tree(opt, pathspec, &tree, &base, base.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&base);
		free(data);
		ආපසු hit;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

static පූර්ණ_සංඛ්‍යා grep_objects(sa_nracanaa grep_opt *opt, const sa_nracanaa pathspec *pathspec,
			const sa_nracanaa object_array *list)
{
	unsigned පූර්ණ_සංඛ්‍යා i;
	පූර්ණ_සංඛ්‍යා hit = 0;
	const unsigned පූර්ණ_සංඛ්‍යා nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	සඳහා (i = 0; i < nr; i++) {
		sa_nracanaa object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     NULL, 0);
		obj_read_unlock();

		නම් (!real_obj) {
			var_na hex[GIT_MAX_HEXSZ + 1];
			const var_na *name = list->objects[i].name;

			නම් (!name) {
				oid_to_hex_r(hex, &list->objects[i].item->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		නම් (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_lock();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		නම් (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].path)) {
			hit = 1;
			නම් (opt->status_only)
				bha_nga;
		}
	}
	ආපසු hit;
}

static පූර්ණ_සංඛ්‍යා grep_directory(sa_nracanaa grep_opt *opt, const sa_nracanaa pathspec *pathspec,
			  පූර්ණ_සංඛ්‍යා exc_std, පූර්ණ_සංඛ්‍යා use_index)
{
	sa_nracanaa dir_struct dir = DIR_INIT;
	පූර්ණ_සංඛ්‍යා i, hit = 0;

	නම් (!use_index)
		dir.flags |= DIR_NO_GITLINKS;
	නම් (exc_std)
		setup_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->index, pathspec);
	සඳහා (i = 0; i < dir.nr; i++) {
		hit |= grep_file(opt, dir.entries[i]->name);
		නම් (hit && opt->status_only)
			bha_nga;
	}
	dir_clear(&dir);
	ආපසු hit;
}

static පූර්ණ_සංඛ්‍යා context_callback(const sa_nracanaa option *opt, const var_na *arg,
			    පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa grep_opt *grep_opt = opt->value;
	පූර්ණ_සංඛ්‍යා value;
	const var_na *endp;

	නම් (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		ආපසු 0;
	}
	value = strtol(arg, (var_na **)&endp, 10);
	නම් (*endp) {
		ආපසු error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = value;
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා file_callback(const sa_nracanaa option *opt, const var_na *arg, පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa grep_opt *grep_opt = opt->value;
	පූර්ණ_සංඛ්‍යා from_stdin;
	const var_na *filename = arg;
	FILE *patterns;
	පූර්ණ_සංඛ්‍යා lno = 0;
	sa_nracanaa strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	නම් (!*filename)
		; /* leave it as-is */
	නැතහොත්
		filename = prefix_filename_except_for_dash(grep_prefix, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	නම් (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	අතරතුර (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		නම් (sb.len == 0)
			continue;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_PATTERN);
	}
	නම් (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	නම් (filename != arg)
		free((void *)filename);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා not_callback(const sa_nracanaa option *opt, const var_na *arg, පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa grep_opt *grep_opt = opt->value;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--not", "command line", 0, GREP_NOT);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා and_callback(const sa_nracanaa option *opt, const var_na *arg, පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa grep_opt *grep_opt = opt->value;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--and", "command line", 0, GREP_AND);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා open_callback(const sa_nracanaa option *opt, const var_na *arg, පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa grep_opt *grep_opt = opt->value;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා close_callback(const sa_nracanaa option *opt, const var_na *arg, පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa grep_opt *grep_opt = opt->value;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා pattern_callback(const sa_nracanaa option *opt, const var_na *arg,
			    පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa grep_opt *grep_opt = opt->value;
	BUG_ON_OPT_NEG(unset);
	append_grep_pattern(grep_opt, arg, "-e option", 0, GREP_PATTERN);
	ආපසු 0;
}

පූර්ණ_සංඛ්‍යා cmd_grep(පූර්ණ_සංඛ්‍යා argc,
	     const var_na **argv,
	     const var_na *prefix,
	     sa_nracanaa repository *repo UNUSED)
{
	පූර්ණ_සංඛ්‍යා hit = 0;
	පූර්ණ_සංඛ්‍යා cached = 0, untracked = 0, opt_exclude = -1;
	පූර්ණ_සංඛ්‍යා seen_dashdash = 0;
	පූර්ණ_සංඛ්‍යා external_grep_allowed__ignored;
	const var_na *show_in_pager = NULL, *default_pager = "dummy";
	sa_nracanaa grep_opt opt;
	sa_nracanaa object_array list = OBJECT_ARRAY_INIT;
	sa_nracanaa pathspec pathspec;
	sa_nracanaa string_list path_list = STRING_LIST_INIT_DUP;
	පූර්ණ_සංඛ්‍යා i;
	පූර්ණ_සංඛ්‍යා dummy;
	පූර්ණ_සංඛ්‍යා use_index = 1;
	පූර්ණ_සංඛ්‍යා allow_revs;
	පූර්ණ_සංඛ්‍යා ret;

	sa_nracanaa option options[] = {
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
		OPT_SET_INT('a', "text", &opt.binary,
			N_("process binary files as text"), GREP_BINARY_TEXT),
		OPT_SET_INT('I', NULL, &opt.binary,
			N_("don't match patterns in binary files"),
			GREP_BINARY_NOMATCH),
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
		OPT_NEGBIT('h', NULL, &opt.pathname, N_("don't show filenames"), 1),
		OPT_BIT('H', NULL, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.count,
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
		OPT_INTEGER(0, "threads", &num_threads,
			N_("use <n> worker threads")),
		OPT_NUMBER_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_GROUP(""),
		OPT_CALLBACK('f', NULL, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', NULL, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, pattern_callback),
		OPT_CALLBACK_F(0, "and", &opt, NULL,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, NULL, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', NULL, &opt, NULL, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', NULL, &opt, NULL, "",
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
			.value = &show_in_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_count,
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

	නම් (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	නම් (use_index && !startup_info->have_repository) {
		පූර්ණ_සංඛ්‍යා fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		නම් (fallback)
			use_index = 0;
		නැතහොත්
			/* die the same way as if we did it at the beginning */
			setup_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	නම් (!use_index)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	නම් (argc > 0 && !opt.pattern_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	නම් (argc > 0 && !opt.pattern_list) {
		append_grep_pattern(&opt, argv[0], "command line", 0,
				    GREP_PATTERN);
		argv++;
		argc--;
	}

	නම් (show_in_pager == default_pager)
		show_in_pager = git_pager(the_repository, 1);
	නම් (show_in_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &path_list;
		opt.output = append_path;
		string_list_append(&path_list, show_in_pager);
	}

	නම් (!opt.pattern_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	නම් (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	සඳහා (i = 0; i < argc; i++) {
		නම් (!strcmp(argv[i], "--")) {
			seen_dashdash = 1;
			bha_nga;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_index && !untracked;
	සඳහා (i = 0; i < argc; i++) {
		const var_na *arg = argv[i];
		sa_nracanaa object_id oid;
		sa_nracanaa object_context oc = {0};
		sa_nracanaa object *object;

		නම් (!strcmp(arg, "--")) {
			i++;
			bha_nga;
		}

		නම් (!allow_revs) {
			නම් (seen_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			bha_nga;
		}

		නම් (get_oid_with_context(the_repository, arg,
					 GET_OID_RECORD_PATH,
					 &oid, &oc)) {
			නම් (seen_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			bha_nga;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		නම් (!seen_dashdash)
			verify_non_filename(the_repository, prefix, arg);
		add_object_array_with_path(object, arg, &list, oc.mode, oc.path);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	නම් (!seen_dashdash) {
		පූර්ණ_සංඛ්‍යා j;
		සඳහා (j = i; j < argc; j++)
			verify_filename(the_repository, prefix, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_depth != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       prefix, argv + i);
	pathspec.max_depth = opt.max_depth;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	නම් (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	නම් (opt.max_count == 0) {
		ret = 1;
		goto out;
	}

	නම් (show_in_pager) {
		නම් (num_threads > 1)
			warning(_("invalid option combination, ignoring --threads"));
		num_threads = 1;
	} නැතහොත් නම් (!HAVE_THREADS && num_threads > 1) {
		warning(_("no threads support, ignoring --threads"));
		num_threads = 1;
	} නැතහොත් නම් (num_threads < 0)
		die(_("invalid number of threads specified (%d)"), num_threads);
	නැතහොත් නම් (num_threads == 0)
		num_threads = HAVE_THREADS ? online_cpus() : 1;

	නම් (num_threads > 1) {
		නම් (!HAVE_THREADS)
			BUG("Somebody got num_threads calculation wrong!");
		නම් (!(opt.name_only || opt.unmatch_name_only || opt.count)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_first_line = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		නම් (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		නම් (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		start_threads(&opt);
	} නැතහොත් {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	නම් (show_in_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	නම් (show_in_pager && opt.pattern_list && !opt.pattern_list->next) {
		const var_na *pager = path_list.items[0].string;
		පූර්ණ_සංඛ්‍යා len = strlen(pager);

		නම් (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		නම් (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&path_list, "-I");

		නම් (!strcmp("less", pager) || !strcmp("vi", pager)) {
			sa_nracanaa strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.pattern_list->pattern);
			string_list_append_nodup(&path_list,
						 strbuf_detach(&buf, NULL));
		}
	}

	නම් (!show_in_pager && !opt.status_only)
		setup_pager(the_repository);

	die_for_incompatible_opt3(!use_index, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	නම් (!use_index || untracked) {
		පූර්ණ_සංඛ්‍යා use_exclude = (opt_exclude < 0) ? use_index : !!opt_exclude;
		hit = grep_directory(&opt, &pathspec, use_exclude, use_index);
	} නැතහොත් නම් (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} නැතහොත් නම් (!list.nr) {
		නම් (!cached)
			setup_work_tree(the_repository);

		hit = grep_cache(&opt, &pathspec, cached);
	} නැතහොත් {
		නම් (cached)
			die(_("both --cached and trees are given"));

		hit = grep_objects(&opt, &pathspec, &list);
	}

	නම් (num_threads > 1)
		hit |= wait_all();
	නම් (hit && show_in_pager)
		run_pager(&opt, prefix);

	ret = !hit;

out:
	clear_pathspec(&pathspec);
	string_list_clear(&path_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	ආපසු ret;
}
