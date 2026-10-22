/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "tree-walk.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "grep.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "pathspec.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "submodule-config.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/source.h"
#ଅନ୍ତର୍ଭାଵ "oid-array.h"
#ଅନ୍ତର୍ଭାଵ "oidset.h"
#ଅନ୍ତର୍ଭାଵ "pager.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "promisor-remote.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *grep_prefix;

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse_submodules;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_threads;

ସ୍ଥିର pthread_t *threads;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
ସଂରଚନା work_item {
	ସଂରଚନା grep_source source;
	ଅକ୍ଷର done;
	ସଂରଚନା strbuf out;
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
ସ୍ଥିର ସଂରଚନା work_item todo[TODO_SIZE];
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା todo_start;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା todo_end;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା todo_done;

/* Has all work items been added? */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_work_added;

ସ୍ଥିର ସଂରଚନା repository **repos_to_free;
ସ୍ଥିର size_t repos_to_free_nr, repos_to_free_alloc;

/* This lock protects all the variables above. */
ସ୍ଥିର pthread_mutex_t grep_mutex;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ grep_lock(ଶୂନ୍ୟ)
{
	pthread_mutex_lock(&grep_mutex);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ grep_unlock(ଶୂନ୍ୟ)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
ସ୍ଥିର pthread_cond_t cond_add;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
ସ୍ଥିର pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
ସ୍ଥିର pthread_cond_t cond_result;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip_first_ଧାଡ଼ି;

ସ୍ଥିର ଶୂନ୍ୟ add_work(ସଂରଚନା grep_opt *opt, ସଂରଚନା grep_source *gs)
{
	ଯଦି (opt->ଦ୍ୱିମିକ != GREP_ଦ୍ୱିମିକ_TEXT)
		grep_source_load_driver(gs, opt->repo->index);

	grep_lock();

	ଯେତେବେଳେ ((todo_end+1) % ARRAY_SIZE(todo) == todo_done) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_end].source = *gs;
	todo[todo_end].done = 0;
	strbuf_reset(&todo[todo_end].out);
	todo_end = (todo_end + 1) % ARRAY_SIZE(todo);

	pthread_cond_signal(&cond_add);
	grep_unlock();
}

ସ୍ଥିର ସଂରଚନା work_item *get_work(ଶୂନ୍ୟ)
{
	ସଂରଚନା work_item *ret;

	grep_lock();
	ଯେତେବେଳେ (todo_start == todo_end && !all_work_added) {
		pthread_cond_wait(&cond_add, &grep_mutex);
	}

	ଯଦି (todo_start == todo_end && all_work_added) {
		ret = ଖାଲି;
	} ଅନ୍ୟଥା {
		ret = &todo[todo_start];
		todo_start = (todo_start + 1) % ARRAY_SIZE(todo);
	}
	grep_unlock();
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ work_done(ସଂରଚନା work_item *w)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_done;

	grep_lock();
	w->done = 1;
	old_done = todo_done;
	ପାଇଁ(; todo[todo_done].done && todo_done != todo_start;
	    todo_done = (todo_done+1) % ARRAY_SIZE(todo)) {
		w = &todo[todo_done];
		ଯଦି (w->out.len) {
			ଅଚଳ ଅକ୍ଷର *p = w->out.buf;
			size_t len = w->out.len;

			/* Skip the leading hunk mark of the first file. */
			ଯଦି (skip_first_ଧାଡ଼ି) {
				ଯେତେବେଳେ (len) {
					len--;
					ଯଦି (*p++ == '\n')
						ଭାଙ୍ଗ;
				}
				skip_first_ଧାଡ଼ି = 0;
			}

			write_or_die(1, p, len);
		}
		grep_source_clear(&w->source);
	}

	ଯଦି (old_done != todo_done)
		pthread_cond_signal(&cond_write);

	ଯଦି (all_work_added && todo_done == todo_end)
		pthread_cond_signal(&cond_result);

	grep_unlock();
}

ସ୍ଥିର ଶୂନ୍ୟ free_repos(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < repos_to_free_nr; i++) {
		repo_clear(repos_to_free[i]);
		ମୁକ୍ତ(repos_to_free[i]);
	}
	FREE_AND_NULL(repos_to_free);
	repos_to_free_nr = 0;
	repos_to_free_alloc = 0;
}

ସ୍ଥିର ଶୂନ୍ୟ *run(ଶୂନ୍ୟ *arg)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hit = 0;
	ସଂରଚନା grep_opt *opt = arg;

	ଯେତେବେଳେ (1) {
		ସଂରଚନା work_item *w = get_work();
		ଯଦି (!w)
			ଭାଙ୍ଗ;

		opt->output_priv = w;
		hit |= grep_source(opt, &w->source);
		grep_source_clear_data(&w->source);
		work_done(w);
	}
	free_grep_patterns(opt);
	ମୁକ୍ତ(opt);

	ଫେରନ୍ତୁ (ଶୂନ୍ୟ*) (intptr_t) hit;
}

ସ୍ଥିର ଶୂନ୍ୟ strbuf_out(ସଂରଚନା grep_opt *opt, ଅଚଳ ଶୂନ୍ୟ *buf, size_t size)
{
	ସଂରଚନା work_item *w = opt->output_priv;
	strbuf_add(&w->out, buf, size);
}

ସ୍ଥିର ଶୂନ୍ୟ start_threads(ସଂରଚନା grep_opt *opt)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	pthread_mutex_init(&grep_mutex, ଖାଲି);
	pthread_mutex_init(&grep_attr_mutex, ଖାଲି);
	pthread_cond_init(&cond_add, ଖାଲି);
	pthread_cond_init(&cond_write, ଖାଲି);
	pthread_cond_init(&cond_result, ଖାଲି);
	grep_use_locks = 1;
	enable_obj_read_lock();

	ପାଇଁ (i = 0; i < ARRAY_SIZE(todo); i++) {
		strbuf_init(&todo[i].out, 0);
	}

	CALLOC_ARRAY(threads, ଅଙ୍କ_threads);
	ପାଇଁ (i = 0; i < ଅଙ୍କ_threads; i++) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
		ସଂରଚନା grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_out;
		compile_grep_patterns(o);
		err = pthread_create(&threads[i], ଖାଲି, run, o);

		ଯଦି (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା wait_all(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hit = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (!HAVE_THREADS)
		BUG("Never call this function unless you have started threads");

	grep_lock();
	all_work_added = 1;

	/* Wait until all work is done. */
	ଯେତେବେଳେ (todo_done != todo_end)
		pthread_cond_wait(&cond_result, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_broadcast(&cond_add);
	grep_unlock();

	ପାଇଁ (i = 0; i < ଅଙ୍କ_threads; i++) {
		ଶୂନ୍ୟ *h;
		pthread_join(threads[i], &h);
		hit |= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (intptr_t) h;
	}

	ମୁକ୍ତ(threads);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_add);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_result);
	grep_use_locks = 0;
	disable_obj_read_lock();

	ଫେରନ୍ତୁ hit;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା grep_cmd_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			   ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା st = grep_config(var, ମୂଲ୍ୟ, ctx, cb);

	ଯଦି (git_color_config(var, ମୂଲ୍ୟ, cb) < 0)
		st = -1;
	ଅନ୍ୟଥା ଯଦି (git_default_config(var, ମୂଲ୍ୟ, ctx, cb) < 0)
		st = -1;

	ଯଦି (!strcmp(var, "grep.threads")) {
		ଅଙ୍କ_threads = git_config_int(var, ମୂଲ୍ୟ, ctx->kvi);
		ଯଦି (ଅଙ୍କ_threads < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    ଅଙ୍କ_threads, var);
		ଅନ୍ୟଥା ଯଦି (!HAVE_THREADS && ଅଙ୍କ_threads > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			ଅଙ୍କ_threads = 1;
		}
	}

	ଯଦି (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, ମୂଲ୍ୟ);

	ଫେରନ୍ତୁ st;
}

ସ୍ଥିର ଶୂନ୍ୟ grep_source_name(ସଂରଚନା grep_opt *opt, ଅଚଳ ଅକ୍ଷର *filename,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବୃକ୍ଷ_name_len, ସଂରଚନା strbuf *out)
{
	strbuf_reset(out);

	ଯଦି (opt->null_following_name) {
		ଯଦି (opt->relative && grep_prefix) {
			ସଂରଚନା strbuf rel_buf = STRBUF_INIT;
			ଅଚଳ ଅକ୍ଷର *rel_name =
				relative_path(filename + ବୃକ୍ଷ_name_len,
					      grep_prefix, &rel_buf);

			ଯଦି (ବୃକ୍ଷ_name_len)
				strbuf_add(out, filename, ବୃକ୍ଷ_name_len);

			strbuf_addstr(out, rel_name);
			strbuf_release(&rel_buf);
		} ଅନ୍ୟଥା {
			strbuf_addstr(out, filename);
		}
		ଫେରନ୍ତୁ;
	}

	ଯଦି (opt->relative && grep_prefix)
		quote_path(filename + ବୃକ୍ଷ_name_len, grep_prefix, out, 0);
	ଅନ୍ୟଥା
		quote_c_style(filename + ବୃକ୍ଷ_name_len, out, ଖାଲି, 0);

	ଯଦି (ବୃକ୍ଷ_name_len)
		strbuf_ଭର୍ତ୍ତି(out, 0, filename, ବୃକ୍ଷ_name_len);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା grep_oid(ସଂରଚନା grep_opt *opt, ଅଚଳ ସଂରଚନା object_id *oid,
		     ଅଚଳ ଅକ୍ଷର *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବୃକ୍ଷ_name_len,
		     ଅଚଳ ଅକ୍ଷର *path)
{
	ସଂରଚନା strbuf pathbuf = STRBUF_INIT;
	ସଂରଚନା grep_source gs;

	grep_source_name(opt, filename, ବୃକ୍ଷ_name_len, &pathbuf);
	grep_source_init_oid(&gs, pathbuf.buf, path, oid, opt->repo);
	strbuf_release(&pathbuf);

	ଯଦି (ଅଙ୍କ_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		ଫେରନ୍ତୁ hit;
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା grep_file(ସଂରଚନା grep_opt *opt, ଅଚଳ ଅକ୍ଷର *filename)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା grep_source gs;

	grep_source_name(opt, filename, 0, &buf);
	grep_source_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	ଯଦି (ଅଙ୍କ_threads > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		add_work(opt, &gs);
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା hit;

		hit = grep_source(opt, &gs);

		grep_source_clear(&gs);
		ଫେରନ୍ତୁ hit;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ append_path(ସଂରଚନା grep_opt *opt, ଅଚଳ ଶୂନ୍ୟ *data, size_t len)
{
	ସଂରଚନା string_list *path_list = opt->output_priv;

	ଯଦି (len == 1 && *(ଅଚଳ ଅକ୍ଷର *)data == '\0')
		ଫେରନ୍ତୁ;
	string_list_append_nodup(path_list, xstrndup(data, len));
}

ସ୍ଥିର ଶୂନ୍ୟ run_pager(ସଂରଚନା grep_opt *opt, ଅଚଳ ଅକ୍ଷର *prefix)
{
	ସଂରଚନା string_list *path_list = opt->output_priv;
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, status;

	ପାଇଁ (i = 0; i < path_list->nr; i++)
		strvec_ଠେଲ(&child.args, path_list->ବସ୍ତୁଗୁଡ଼ିକ[i].string);
	child.dir = prefix;
	child.use_shell = 1;

	status = run_command(&child);
	ଯଦି (status)
		exit(status);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା grep_cache(ସଂରଚନା grep_opt *opt,
		      ଅଚଳ ସଂରଚନା pathspec *pathspec, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cached);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା grep_ବୃକ୍ଷ(ସଂରଚନା grep_opt *opt, ଅଚଳ ସଂରଚନା pathspec *pathspec,
		     ସଂରଚନା ବୃକ୍ଷ_desc *ବୃକ୍ଷ, ସଂରଚନା strbuf *base, ପୂର୍ଣ୍ଣସଂଖ୍ୟା tn_len,
		     ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_attr);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା grep_submodule(ସଂରଚନା grep_opt *opt,
			  ଅଚଳ ସଂରଚନା pathspec *pathspec,
			  ଅଚଳ ସଂରଚନା object_id *oid,
			  ଅଚଳ ଅକ୍ଷର *filename, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cached)
{
	ସଂରଚନା repository *subrepo;
	ସଂରଚନା repository *superproject = opt->repo;
	ସଂରଚନା grep_opt subopt;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hit = 0;

	ଯଦି (!is_submodule_active(superproject, path))
		ଫେରନ୍ତୁ 0;

	subrepo = xmalloc(ଆକାର(*subrepo));
	ଯଦି (repo_submodule_init(subrepo, superproject, path, null_oid(opt->repo->hash_algo))) {
		ମୁକ୍ତ(subrepo);
		ଫେରନ୍ତୁ 0;
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

	memcpy(&subopt, opt, ଆକାର(subopt));
	subopt.repo = subrepo;

	ଯଦି (oid) {
		ଗଣନା object_type object_type;
		ସଂରଚନା ବୃକ୍ଷ_desc ବୃକ୍ଷ;
		ଶୂନ୍ୟ *data;
		size_t size;
		ସଂରଚନା strbuf base = STRBUF_INIT;

		obj_read_lock();
		object_type = odb_read_object_info(subrepo->objects, oid, ଖାଲି);
		obj_read_unlock();
		data = odb_read_object_peeled(subrepo->objects, oid, OBJ_ବୃକ୍ଷ, &size, ଖାଲି);
		ଯଦି (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(oid));

		strbuf_addstr(&base, filename);
		strbuf_addch(&base, '/');

		init_ବୃକ୍ଷ_desc(&ବୃକ୍ଷ, oid, data, size);
		hit = grep_ବୃକ୍ଷ(&subopt, pathspec, &ବୃକ୍ଷ, &base, base.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&base);
		ମୁକ୍ତ(data);
	} ଅନ୍ୟଥା {
		hit = grep_cache(&subopt, pathspec, cached);
	}

	ଫେରନ୍ତୁ hit;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା grep_cache(ସଂରଚନା grep_opt *opt,
		      ଅଚଳ ସଂରଚନା pathspec *pathspec, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cached)
{
	ସଂରଚନା repository *repo = opt->repo;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hit = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr;
	ସଂରଚନା strbuf name = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_base_len = 0;
	ଯଦି (repo->submodule_prefix) {
		name_base_len = strlen(repo->submodule_prefix);
		strbuf_addstr(&name, repo->submodule_prefix);
	}

	ଯଦି (repo_read_index(repo) < 0)
		die(_("index file corrupt"));

	ପାଇଁ (nr = 0; nr < repo->index->cache_nr; nr++) {
		ଅଚଳ ସଂରଚନା cache_entry *ce = repo->index->cache[nr];

		ଯଦି (!cached && ce_skip_worktree(ce))
			ଜାରି;

		strbuf_setlen(&name, name_base_len);
		strbuf_addstr(&name, ce->name);
		ଯଦି (S_ISSPARSEDIR(ce->ce_mode)) {
			ଗଣନା object_type type;
			ସଂରଚନା ବୃକ୍ଷ_desc ବୃକ୍ଷ;
			ଶୂନ୍ୟ *data;
			size_t size;

			data = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &size);
			ଯଦି (!data)
				die(_("unable to read tree %s"), oid_to_hex(&ce->oid));
			init_ବୃକ୍ଷ_desc(&ବୃକ୍ଷ, &ce->oid, data, size);

			hit |= grep_ବୃକ୍ଷ(opt, pathspec, &ବୃକ୍ଷ, &name, 0, 0);
			strbuf_setlen(&name, name_base_len);
			strbuf_addstr(&name, ce->name);
			ମୁକ୍ତ(data);
		} ଅନ୍ୟଥା ଯଦି (S_ISREG(ce->ce_mode) &&
		    match_pathspec(repo->index, pathspec, name.buf, name.len, 0, ଖାଲି,
				   S_ISDIR(ce->ce_mode) ||
				   S_ISGITLINK(ce->ce_mode))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			ଯଦି (cached || (ce->ce_flags & CE_VALID)) {
				ଯଦି (ce_stage(ce) || ce_intent_to_add(ce))
					ଜାରି;
				hit |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} ଅନ୍ୟଥା {
				hit |= grep_file(opt, name.buf);
			}
		} ଅନ୍ୟଥା ଯଦି (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
			   submodule_path_match(repo->index, pathspec, name.buf, ଖାଲି)) {
			hit |= grep_submodule(opt, pathspec, ଖାଲି, ce->name,
					      ce->name, cached);
		} ଅନ୍ୟଥା {
			ଜାରି;
		}

		ଯଦି (ce_stage(ce)) {
			କର {
				nr++;
			} ଯେତେବେଳେ (nr < repo->index->cache_nr &&
				 !strcmp(ce->name, repo->index->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		ଯଦି (hit && opt->status_only)
			ଭାଙ୍ଗ;
	}

	strbuf_release(&name);
	ଫେରନ୍ତୁ hit;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା grep_ବୃକ୍ଷ(ସଂରଚନା grep_opt *opt, ଅଚଳ ସଂରଚନା pathspec *pathspec,
		     ସଂରଚନା ବୃକ୍ଷ_desc *ବୃକ୍ଷ, ସଂରଚନା strbuf *base, ପୂର୍ଣ୍ଣସଂଖ୍ୟା tn_len,
		     ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_attr)
{
	ସଂରଚନା repository *repo = opt->repo;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hit = 0;
	ଗଣନା interesting match = entry_not_interesting;
	ସଂରଚନା name_entry entry;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_baselen = base->len;
	ସଂରଚନା strbuf name = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_base_len = 0;
	ଯଦି (repo->submodule_prefix) {
		strbuf_addstr(&name, repo->submodule_prefix);
		name_base_len = name.len;
	}

	ଯେତେବେଳେ (ବୃକ୍ଷ_entry(ବୃକ୍ଷ, &entry)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା te_len = ବୃକ୍ଷ_entry_len(&entry);

		ଯଦି (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = ବୃକ୍ଷ_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_setlen(&name, name_base_len);

			ଯଦି (match == all_entries_not_interesting)
				ଭାଙ୍ଗ;
			ଯଦି (match == entry_not_interesting)
				ଜାରି;
		}

		strbuf_add(base, entry.path, te_len);

		ଯଦି (S_ISREG(entry.mode)) {
			hit |= grep_oid(opt, &entry.oid, base->buf, tn_len,
					 check_attr ? base->buf + tn_len : ଖାଲି);
		} ଅନ୍ୟଥା ଯଦି (S_ISDIR(entry.mode)) {
			ଗଣନା object_type type;
			ସଂରଚନା ବୃକ୍ଷ_desc sub;
			ଶୂନ୍ୟ *data;
			size_t size;

			data = odb_read_object(the_repository->objects,
					       &entry.oid, &type, &size);
			ଯଦି (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_ବୃକ୍ଷ_desc(&sub, &entry.oid, data, size);
			hit |= grep_ବୃକ୍ଷ(opt, pathspec, &sub, base, tn_len,
					 check_attr);
			ମୁକ୍ତ(data);
		} ଅନ୍ୟଥା ଯଦି (recurse_submodules && S_ISGITLINK(entry.mode)) {
			hit |= grep_submodule(opt, pathspec, &entry.oid,
					      base->buf, base->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(base, old_baselen);

		ଯଦି (hit && opt->status_only)
			ଭାଙ୍ଗ;
	}

	strbuf_release(&name);
	ଫେରନ୍ତୁ hit;
}

ସ୍ଥିର ଶୂନ୍ୟ collect_blob_oids_for_ବୃକ୍ଷ(ସଂରଚନା repository *repo,
				       ଅଚଳ ସଂରଚନା pathspec *pathspec,
				       ସଂରଚନା ବୃକ୍ଷ_desc *ବୃକ୍ଷ,
				       ସଂରଚନା strbuf *base,
				       ପୂର୍ଣ୍ଣସଂଖ୍ୟା tn_len,
				       ସଂରଚନା oidset *blob_oids)
{
	ସଂରଚନା name_entry entry;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_baselen = base->len;
	ସଂରଚନା strbuf name = STRBUF_INIT;
	ଗଣନା interesting match = entry_not_interesting;

	ଯେତେବେଳେ (ବୃକ୍ଷ_entry(ବୃକ୍ଷ, &entry)) {
		ଯଦି (match != all_entries_interesting) {
			strbuf_addstr(&name, base->buf + tn_len);
			match = ବୃକ୍ଷ_entry_interesting(repo->index,
						       &entry, &name,
						       pathspec);
			strbuf_reset(&name);

			ଯଦି (match == all_entries_not_interesting)
				ଭାଙ୍ଗ;
			ଯଦି (match == entry_not_interesting)
				ଜାରି;
		}

		strbuf_add(base, entry.path, ବୃକ୍ଷ_entry_len(&entry));

		ଯଦି (S_ISREG(entry.mode)) {
			ଯଦି (!odb_has_object(repo->objects, &entry.oid, 0))
				oidset_ଭର୍ତ୍ତି(blob_oids, &entry.oid);
		} ଅନ୍ୟଥା ଯଦି (S_ISDIR(entry.mode)) {
			ଗଣନା object_type type;
			ସଂରଚନା ବୃକ୍ଷ_desc sub_ବୃକ୍ଷ;
			ଶୂନ୍ୟ *data;
			size_t size;

			data = odb_read_object(repo->objects, &entry.oid,
					       &type, &size);
			ଯଦି (!data)
				die(_("unable to read tree (%s)"),
				    oid_to_hex(&entry.oid));

			strbuf_addch(base, '/');
			init_ବୃକ୍ଷ_desc(&sub_ବୃକ୍ଷ, &entry.oid, data, size);
			collect_blob_oids_for_ବୃକ୍ଷ(repo, pathspec, &sub_ବୃକ୍ଷ,
						   base, tn_len, blob_oids);
			ମୁକ୍ତ(data);
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

ସ୍ଥିର ଶୂନ୍ୟ collect_blob_oids_for_treeish(ସଂରଚନା grep_opt *opt,
					  ଅଚଳ ସଂରଚନା pathspec *pathspec,
					  ଅଚଳ ସଂରଚନା object_id *ବୃକ୍ଷ_ish_oid,
					  ଅଚଳ ଅକ୍ଷର *name,
					  ସଂରଚନା oidset *blob_oids)
{
	ସଂରଚନା ବୃକ୍ଷ_desc ବୃକ୍ଷ;
	ଶୂନ୍ୟ *data;
	size_t size;
	ସଂରଚନା strbuf base = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

	data = odb_read_object_peeled(opt->repo->objects, ବୃକ୍ଷ_ish_oid,
				      OBJ_ବୃକ୍ଷ, &size, ଖାଲି);

	ଯଦି (!data)
		ଫେରନ୍ତୁ;

	len = name ? strlen(name) : 0;
	ଯଦି (len) {
		strbuf_add(&base, name, len);
		strbuf_addch(&base, ':');
	}
	init_ବୃକ୍ଷ_desc(&ବୃକ୍ଷ, ବୃକ୍ଷ_ish_oid, data, size);

	collect_blob_oids_for_ବୃକ୍ଷ(opt->repo, pathspec, &ବୃକ୍ଷ,
				   &base, base.len, blob_oids);

	strbuf_release(&base);
	ମୁକ୍ତ(data);
}

ସ୍ଥିର ଶୂନ୍ୟ prefetch_grep_blobs(ସଂରଚନା grep_opt *opt,
				ଅଚଳ ସଂରଚନା pathspec *pathspec,
				ଅଚଳ ସଂରଚନା object_array *list)
{
	ସଂରଚନା oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	ଯଦି (!repo_has_promisor_remote(opt->repo))
		ଫେରନ୍ତୁ;

	/* For each tree, gather the blobs in it */
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < list->nr; i++) {
		ସଂରଚନା object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     ଖାଲି, 0);
		obj_read_unlock();

		ଯଦି (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_ବୃକ୍ଷ))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	ଯଦି (oidset_size(&blob_oids)) {
		ସଂରଚନା oid_array to_fetch = OID_ARRAY_INIT;
		ସଂରଚନା oidset_iter iter;
		ଅଚଳ ସଂରଚନା object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		ଯେତେବେଳେ ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(opt->repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}
	oidset_clear(&blob_oids);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା grep_object(ସଂରଚନା grep_opt *opt, ଅଚଳ ସଂରଚନା pathspec *pathspec,
		       ସଂରଚନା object *obj, ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *path)
{
	ଯଦି (obj->type == OBJ_BLOB)
		ଫେରନ୍ତୁ grep_oid(opt, &obj->oid, name, 0, path);
	ଯଦି (obj->type == OBJ_COMMIT || obj->type == OBJ_ବୃକ୍ଷ) {
		ସଂରଚନା ବୃକ୍ଷ_desc ବୃକ୍ଷ;
		ଶୂନ୍ୟ *data;
		size_t size;
		ସଂରଚନା strbuf base;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା hit, len;

		data = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_ବୃକ୍ଷ, &size, ଖାଲି);
		ଯଦି (!data)
			die(_("unable to read tree (%s)"), oid_to_hex(&obj->oid));

		len = name ? strlen(name) : 0;
		strbuf_init(&base, PATH_MAX + len + 1);
		ଯଦି (len) {
			strbuf_add(&base, name, len);
			strbuf_addch(&base, ':');
		}
		init_ବୃକ୍ଷ_desc(&ବୃକ୍ଷ, &obj->oid, data, size);
		hit = grep_ବୃକ୍ଷ(opt, pathspec, &ବୃକ୍ଷ, &base, base.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&base);
		ମୁକ୍ତ(data);
		ଫେରନ୍ତୁ hit;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା grep_objects(ସଂରଚନା grep_opt *opt, ଅଚଳ ସଂରଚନା pathspec *pathspec,
			ଅଚଳ ସଂରଚନା object_array *list)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hit = 0;
	ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	ପାଇଁ (i = 0; i < nr; i++) {
		ସଂରଚନା object *real_obj;

		obj_read_lock();
		real_obj = deref_tag(opt->repo, list->objects[i].item,
				     ଖାଲି, 0);
		obj_read_unlock();

		ଯଦି (!real_obj) {
			ଅକ୍ଷର hex[GIT_MAX_HEXSZ + 1];
			ଅଚଳ ଅକ୍ଷର *name = list->objects[i].name;

			ଯଦି (!name) {
				oid_to_hex_r(hex, &list->objects[i].item->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		ଯଦି (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_lock();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		ଯଦି (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].path)) {
			hit = 1;
			ଯଦି (opt->status_only)
				ଭାଙ୍ଗ;
		}
	}
	ଫେରନ୍ତୁ hit;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା grep_directory(ସଂରଚନା grep_opt *opt, ଅଚଳ ସଂରଚନା pathspec *pathspec,
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exc_std, ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_index)
{
	ସଂରଚନା dir_struct dir = DIR_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, hit = 0;

	ଯଦି (!use_index)
		dir.flags |= DIR_NO_GITLINKS;
	ଯଦି (exc_std)
		setup_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->index, pathspec);
	ପାଇଁ (i = 0; i < dir.nr; i++) {
		hit |= grep_file(opt, dir.entries[i]->name);
		ଯଦି (hit && opt->status_only)
			ଭାଙ୍ଗ;
	}
	dir_clear(&dir);
	ଫେରନ୍ତୁ hit;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା context_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା grep_opt *grep_opt = opt->ମୂଲ୍ୟ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;
	ଅଚଳ ଅକ୍ଷର *endp;

	ଯଦି (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		ଫେରନ୍ତୁ 0;
	}
	ମୂଲ୍ୟ = strtol(arg, (ଅକ୍ଷର **)&endp, 10);
	ଯଦି (*endp) {
		ଫେରନ୍ତୁ error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = ମୂଲ୍ୟ;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା file_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା grep_opt *grep_opt = opt->ମୂଲ୍ୟ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା from_stdin;
	ଅଚଳ ଅକ୍ଷର *filename = arg;
	FILE *patterns;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା lno = 0;
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	ଯଦି (!*filename)
		; /* leave it as-is */
	ଅନ୍ୟଥା
		filename = prefix_filename_except_for_dash(grep_prefix, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	ଯଦି (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	ଯେତେବେଳେ (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		ଯଦି (sb.len == 0)
			ଜାରି;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_PATTERN);
	}
	ଯଦି (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	ଯଦି (filename != arg)
		ମୁକ୍ତ((ଶୂନ୍ୟ *)filename);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା not_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା grep_opt *grep_opt = opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--not", "command line", 0, GREP_NOT);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା and_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା grep_opt *grep_opt = opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "--and", "command line", 0, GREP_AND);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା grep_opt *grep_opt = opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା close_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା grep_opt *grep_opt = opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_pattern(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pattern_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା grep_opt *grep_opt = opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_NEG(unset);
	append_grep_pattern(grep_opt, arg, "-e option", 0, GREP_PATTERN);
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_grep(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	     ଅଚଳ ଅକ୍ଷର **argv,
	     ଅଚଳ ଅକ୍ଷର *prefix,
	     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hit = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cached = 0, untracked = 0, opt_exclude = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା seen_dashdash = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା external_grep_allowed__ignored;
	ଅଚଳ ଅକ୍ଷର *show_in_pager = ଖାଲି, *default_pager = "dummy";
	ସଂରଚନା grep_opt opt;
	ସଂରଚନା object_array list = OBJECT_ARRAY_INIT;
	ସଂରଚନା pathspec pathspec;
	ସଂରଚନା string_list path_list = STRING_LIST_INIT_DUP;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dummy;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_index = 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_revs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ସଂରଚନା option options[] = {
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
		OPT_SET_INT('a', "text", &opt.ଦ୍ୱିମିକ,
			N_("process binary files as text"), GREP_ଦ୍ୱିମିକ_TEXT),
		OPT_SET_INT('I', ଖାଲି, &opt.ଦ୍ୱିମିକ,
			N_("don't match patterns in binary files"),
			GREP_ଦ୍ୱିମିକ_NOMATCH),
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
		OPT_NEGBIT('h', ଖାଲି, &opt.pathname, N_("don't show filenames"), 1),
		OPT_BIT('H', ଖାଲି, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.ଗଣନା_2,
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
		OPT_INTEGER(0, "threads", &ଅଙ୍କ_threads,
			N_("use <n> worker threads")),
		OPT_ସଂଖ୍ୟା_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_GROUP(""),
		OPT_CALLBACK('f', ଖାଲି, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', ଖାଲି, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, pattern_callback),
		OPT_CALLBACK_F(0, "and", &opt, ଖାଲି,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, ଖାଲି, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', ଖାଲି, &opt, ଖାଲି, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', ଖାଲି, &opt, ଖାଲି, "",
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
			.ମୂଲ୍ୟ = &show_in_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_ଗଣନା,
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

	ଯଦି (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	ଯଦି (use_index && !startup_info->have_repository) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		ଯଦି (fallback)
			use_index = 0;
		ଅନ୍ୟଥା
			/* die the same way as if we did it at the beginning */
			setup_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	ଯଦି (!use_index)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	ଯଦି (argc > 0 && !opt.pattern_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	ଯଦି (argc > 0 && !opt.pattern_list) {
		append_grep_pattern(&opt, argv[0], "command line", 0,
				    GREP_PATTERN);
		argv++;
		argc--;
	}

	ଯଦି (show_in_pager == default_pager)
		show_in_pager = git_pager(the_repository, 1);
	ଯଦି (show_in_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &path_list;
		opt.output = append_path;
		string_list_append(&path_list, show_in_pager);
	}

	ଯଦି (!opt.pattern_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	ଯଦି (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	ପାଇଁ (i = 0; i < argc; i++) {
		ଯଦି (!strcmp(argv[i], "--")) {
			seen_dashdash = 1;
			ଭାଙ୍ଗ;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_index && !untracked;
	ପାଇଁ (i = 0; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];
		ସଂରଚନା object_id oid;
		ସଂରଚନା object_context oc = {0};
		ସଂରଚନା object *object;

		ଯଦି (!strcmp(arg, "--")) {
			i++;
			ଭାଙ୍ଗ;
		}

		ଯଦି (!allow_revs) {
			ଯଦି (seen_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			ଭାଙ୍ଗ;
		}

		ଯଦି (get_oid_with_context(the_repository, arg,
					 GET_OID_RECORD_PATH,
					 &oid, &oc)) {
			ଯଦି (seen_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			ଭାଙ୍ଗ;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		ଯଦି (!seen_dashdash)
			verify_non_filename(the_repository, prefix, arg);
		add_object_array_with_path(object, arg, &list, oc.mode, oc.path);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	ଯଦି (!seen_dashdash) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
		ପାଇଁ (j = i; j < argc; j++)
			verify_filename(the_repository, prefix, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_depth != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       prefix, argv + i);
	pathspec.max_depth = opt.max_depth;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	ଯଦି (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	ଯଦି (opt.max_ଗଣନା == 0) {
		ret = 1;
		ଯାଅ out;
	}

	ଯଦି (show_in_pager) {
		ଯଦି (ଅଙ୍କ_threads > 1)
			warning(_("invalid option combination, ignoring --threads"));
		ଅଙ୍କ_threads = 1;
	} ଅନ୍ୟଥା ଯଦି (!HAVE_THREADS && ଅଙ୍କ_threads > 1) {
		warning(_("no threads support, ignoring --threads"));
		ଅଙ୍କ_threads = 1;
	} ଅନ୍ୟଥା ଯଦି (ଅଙ୍କ_threads < 0)
		die(_("invalid number of threads specified (%d)"), ଅଙ୍କ_threads);
	ଅନ୍ୟଥା ଯଦି (ଅଙ୍କ_threads == 0)
		ଅଙ୍କ_threads = HAVE_THREADS ? online_cpus() : 1;

	ଯଦି (ଅଙ୍କ_threads > 1) {
		ଯଦି (!HAVE_THREADS)
			BUG("Somebody got num_threads calculation wrong!");
		ଯଦି (!(opt.name_only || opt.unmatch_name_only || opt.ଗଣନା_2)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_first_ଧାଡ଼ି = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		ଯଦି (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		ଯଦି (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		start_threads(&opt);
	} ଅନ୍ୟଥା {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	ଯଦି (show_in_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	ଯଦି (show_in_pager && opt.pattern_list && !opt.pattern_list->next) {
		ଅଚଳ ଅକ୍ଷର *pager = path_list.ବସ୍ତୁଗୁଡ଼ିକ[0].string;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = strlen(pager);

		ଯଦି (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		ଯଦି (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&path_list, "-I");

		ଯଦି (!strcmp("less", pager) || !strcmp("vi", pager)) {
			ସଂରଚନା strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.pattern_list->pattern);
			string_list_append_nodup(&path_list,
						 strbuf_detach(&buf, ଖାଲି));
		}
	}

	ଯଦି (!show_in_pager && !opt.status_only)
		setup_pager(the_repository);

	die_for_incompatible_opt3(!use_index, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	ଯଦି (!use_index || untracked) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_exclude = (opt_exclude < 0) ? use_index : !!opt_exclude;
		hit = grep_directory(&opt, &pathspec, use_exclude, use_index);
	} ଅନ୍ୟଥା ଯଦି (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} ଅନ୍ୟଥା ଯଦି (!list.nr) {
		ଯଦି (!cached)
			setup_work_ବୃକ୍ଷ(the_repository);

		hit = grep_cache(&opt, &pathspec, cached);
	} ଅନ୍ୟଥା {
		ଯଦି (cached)
			die(_("both --cached and trees are given"));

		hit = grep_objects(&opt, &pathspec, &list);
	}

	ଯଦି (ଅଙ୍କ_threads > 1)
		hit |= wait_all();
	ଯଦି (hit && show_in_pager)
		run_pager(&opt, prefix);

	ret = !hit;

out:
	clear_pathspec(&pathspec);
	string_list_clear(&path_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	ଫେରନ୍ତୁ ret;
}
