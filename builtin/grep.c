/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن ثابت حرف *grep_اڳياڙي;

ساكن حرف ثابت * ثابت grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	عدم
};

ساكن صحيح recurse_submodules;

ساكن صحيح انگ_ڌاڳا;

ساكن pthread_t *ڌاڳا;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
بنية work_شي {
	بنية grep_ذريعو ذريعو;
	حرف پورو;
	بنية strbuf ٻاهريون;
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
#وصف_ڪريو TODO_سائيز 128
ساكن بنية work_شي todo[TODO_سائيز];
ساكن صحيح todo_شروعات;
ساكن صحيح todo_پڇاڙي;
ساكن صحيح todo_پورو;

/* Has all work items been added? */
ساكن صحيح all_work_added;

ساكن بنية repository **repos_منزل_free;
ساكن سائيز_t repos_منزل_free_nr, repos_منزل_free_alloc;

/* This lock protects all the variables above. */
ساكن pthread_mutex_t grep_mutex;

ساكن مضمن فراغ grep_تالو(فراغ)
{
	pthread_mutex_تالو(&grep_mutex);
}

ساكن مضمن فراغ grep_unlock(فراغ)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
ساكن pthread_cond_t cond_جوڙيو;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
ساكن pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
ساكن pthread_cond_t cond_نتيجو;

ساكن صحيح skip_پهريون_سٽ;

ساكن فراغ جوڙيو_work(بنية grep_opt *opt, بنية grep_ذريعو *gs)
{
	إذا (opt->ٻٽو != GREP_ٻٽو_متن)
		grep_ذريعو_ڀريو_driver(gs, opt->repo->اشاريو);

	grep_تالو();

	طالما ((todo_پڇاڙي+1) % ARRAY_سائيز(todo) == todo_پورو) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_پڇاڙي].ذريعو = *gs;
	todo[todo_پڇاڙي].پورو = 0;
	strbuf_reset(&todo[todo_پڇاڙي].ٻاهريون);
	todo_پڇاڙي = (todo_پڇاڙي + 1) % ARRAY_سائيز(todo);

	pthread_cond_signal(&cond_جوڙيو);
	grep_unlock();
}

ساكن بنية work_شي *get_work(فراغ)
{
	بنية work_شي *ret;

	grep_تالو();
	طالما (todo_شروعات == todo_پڇاڙي && !all_work_added) {
		pthread_cond_wait(&cond_جوڙيو, &grep_mutex);
	}

	إذا (todo_شروعات == todo_پڇاڙي && all_work_added) {
		ret = عدم;
	} وإلا {
		ret = &todo[todo_شروعات];
		todo_شروعات = (todo_شروعات + 1) % ARRAY_سائيز(todo);
	}
	grep_unlock();
	أرجع ret;
}

ساكن فراغ work_پورو(بنية work_شي *w)
{
	صحيح old_پورو;

	grep_تالو();
	w->پورو = 1;
	old_پورو = todo_پورو;
	لكل(; todo[todo_پورو].پورو && todo_پورو != todo_شروعات;
	    todo_پورو = (todo_پورو+1) % ARRAY_سائيز(todo)) {
		w = &todo[todo_پورو];
		إذا (w->ٻاهريون.len) {
			ثابت حرف *p = w->ٻاهريون.buf;
			سائيز_t len = w->ٻاهريون.len;

			/* Skip the leading hunk mark of the first file. */
			إذا (skip_پهريون_سٽ) {
				طالما (len) {
					len--;
					إذا (*p++ == '\n')
						اكسر;
				}
				skip_پهريون_سٽ = 0;
			}

			write_or_die(1, p, len);
		}
		grep_ذريعو_clear(&w->ذريعو);
	}

	إذا (old_پورو != todo_پورو)
		pthread_cond_signal(&cond_write);

	إذا (all_work_added && todo_پورو == todo_پڇاڙي)
		pthread_cond_signal(&cond_نتيجو);

	grep_unlock();
}

ساكن فراغ free_repos(فراغ)
{
	صحيح i;

	لكل (i = 0; i < repos_منزل_free_nr; i++) {
		repo_clear(repos_منزل_free[i]);
		حرر(repos_منزل_free[i]);
	}
	FREE_AND_NULL(repos_منزل_free);
	repos_منزل_free_nr = 0;
	repos_منزل_free_alloc = 0;
}

ساكن فراغ *لاڳيتو(فراغ *arg)
{
	صحيح ڪاميابي = 0;
	بنية grep_opt *opt = arg;

	طالما (1) {
		بنية work_شي *w = get_work();
		إذا (!w)
			اكسر;

		opt->output_priv = w;
		ڪاميابي |= grep_ذريعو(opt, &w->ذريعو);
		grep_ذريعو_clear_معلومات(&w->ذريعو);
		work_پورو(w);
	}
	free_grep_patterns(opt);
	حرر(opt);

	أرجع (فراغ*) (intptr_t) ڪاميابي;
}

ساكن فراغ strbuf_ٻاهريون(بنية grep_opt *opt, ثابت فراغ *buf, سائيز_t سائيز)
{
	بنية work_شي *w = opt->output_priv;
	strbuf_جوڙيو(&w->ٻاهريون, buf, سائيز);
}

ساكن فراغ شروعات_ڌاڳا(بنية grep_opt *opt)
{
	صحيح i;

	pthread_mutex_init(&grep_mutex, عدم);
	pthread_mutex_init(&grep_attr_mutex, عدم);
	pthread_cond_init(&cond_جوڙيو, عدم);
	pthread_cond_init(&cond_write, عدم);
	pthread_cond_init(&cond_نتيجو, عدم);
	grep_use_locks = 1;
	enable_obj_read_تالو();

	لكل (i = 0; i < ARRAY_سائيز(todo); i++) {
		strbuf_init(&todo[i].ٻاهريون, 0);
	}

	CALLOC_ARRAY(ڌاڳا, انگ_ڌاڳا);
	لكل (i = 0; i < انگ_ڌاڳا; i++) {
		صحيح err;
		بنية grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_ٻاهريون;
		compile_grep_patterns(o);
		err = pthread_create(&ڌاڳا[i], عدم, لاڳيتو, o);

		إذا (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

ساكن صحيح wait_all(فراغ)
{
	صحيح ڪاميابي = 0;
	صحيح i;

	إذا (!HAVE_ڌاڳا)
		BUG("Never call this function unless you have started threads");

	grep_تالو();
	all_work_added = 1;

	/* Wait until all work is done. */
	طالما (todo_پورو != todo_پڇاڙي)
		pthread_cond_wait(&cond_نتيجو, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_نشر(&cond_جوڙيو);
	grep_unlock();

	لكل (i = 0; i < انگ_ڌاڳا; i++) {
		فراغ *h;
		pthread_join(ڌاڳا[i], &h);
		ڪاميابي |= (صحيح) (intptr_t) h;
	}

	حرر(ڌاڳا);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_جوڙيو);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_نتيجو);
	grep_use_locks = 0;
	disable_obj_read_تالو();

	أرجع ڪاميابي;
}

ساكن صحيح grep_cmd_config(ثابت حرف *var, ثابت حرف *قدر,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	صحيح st = grep_config(var, قدر, ctx, cb);

	إذا (git_color_config(var, قدر, cb) < 0)
		st = -1;
	وإلا إذا (git_default_config(var, قدر, ctx, cb) < 0)
		st = -1;

	إذا (!strcmp(var, "grep.threads")) {
		انگ_ڌاڳا = git_config_int(var, قدر, ctx->kvi);
		إذا (انگ_ڌاڳا < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    انگ_ڌاڳا, var);
		وإلا إذا (!HAVE_ڌاڳا && انگ_ڌاڳا > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			انگ_ڌاڳا = 1;
		}
	}

	إذا (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, قدر);

	أرجع st;
}

ساكن فراغ grep_ذريعو_name(بنية grep_opt *opt, ثابت حرف *filename,
			     صحيح وڻ_name_len, بنية strbuf *ٻاهريون)
{
	strbuf_reset(ٻاهريون);

	إذا (opt->null_following_name) {
		إذا (opt->relative && grep_اڳياڙي) {
			بنية strbuf rel_buf = STRBUF_INIT;
			ثابت حرف *rel_name =
				relative_رستو(filename + وڻ_name_len,
					      grep_اڳياڙي, &rel_buf);

			إذا (وڻ_name_len)
				strbuf_جوڙيو(ٻاهريون, filename, وڻ_name_len);

			strbuf_addstr(ٻاهريون, rel_name);
			strbuf_release(&rel_buf);
		} وإلا {
			strbuf_addstr(ٻاهريون, filename);
		}
		أرجع;
	}

	إذا (opt->relative && grep_اڳياڙي)
		quote_رستو(filename + وڻ_name_len, grep_اڳياڙي, ٻاهريون, 0);
	وإلا
		quote_c_style(filename + وڻ_name_len, ٻاهريون, عدم, 0);

	إذا (وڻ_name_len)
		strbuf_وجهو(ٻاهريون, 0, filename, وڻ_name_len);
}

ساكن صحيح grep_oid(بنية grep_opt *opt, ثابت بنية object_id *oid,
		     ثابت حرف *filename, صحيح وڻ_name_len,
		     ثابت حرف *رستو)
{
	بنية strbuf pathbuf = STRBUF_INIT;
	بنية grep_ذريعو gs;

	grep_ذريعو_name(opt, filename, وڻ_name_len, &pathbuf);
	grep_ذريعو_init_oid(&gs, pathbuf.buf, رستو, oid, opt->repo);
	strbuf_release(&pathbuf);

	إذا (انگ_ڌاڳا > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		جوڙيو_work(opt, &gs);
		أرجع 0;
	} وإلا {
		صحيح ڪاميابي;

		ڪاميابي = grep_ذريعو(opt, &gs);

		grep_ذريعو_clear(&gs);
		أرجع ڪاميابي;
	}
}

ساكن صحيح grep_file(بنية grep_opt *opt, ثابت حرف *filename)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية grep_ذريعو gs;

	grep_ذريعو_name(opt, filename, 0, &buf);
	grep_ذريعو_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	إذا (انگ_ڌاڳا > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		جوڙيو_work(opt, &gs);
		أرجع 0;
	} وإلا {
		صحيح ڪاميابي;

		ڪاميابي = grep_ذريعو(opt, &gs);

		grep_ذريعو_clear(&gs);
		أرجع ڪاميابي;
	}
}

ساكن فراغ append_رستو(بنية grep_opt *opt, ثابت فراغ *معلومات, سائيز_t len)
{
	بنية string_list *رستو_list = opt->output_priv;

	إذا (len == 1 && *(ثابت حرف *)معلومات == '\0')
		أرجع;
	string_list_append_nodup(رستو_list, xstrndup(معلومات, len));
}

ساكن فراغ لاڳيتو_pager(بنية grep_opt *opt, ثابت حرف *اڳياڙي)
{
	بنية string_list *رستو_list = opt->output_priv;
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;
	صحيح i, حالت;

	لكل (i = 0; i < رستو_list->nr; i++)
		strvec_ڌڪو(&ٻار.دليلون, رستو_list->شيون[i].string);
	ٻار.dir = اڳياڙي;
	ٻار.use_shell = 1;

	حالت = لاڳيتو_command(&ٻار);
	إذا (حالت)
		ٻاهر_نڪرو(حالت);
}

ساكن صحيح grep_cache(بنية grep_opt *opt,
		      ثابت بنية pathspec *pathspec, صحيح cached);
ساكن صحيح grep_وڻ(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
		     بنية وڻ_desc *وڻ, بنية strbuf *بنياد, صحيح tn_len,
		     صحيح check_attr);

ساكن صحيح grep_submodule(بنية grep_opt *opt,
			  ثابت بنية pathspec *pathspec,
			  ثابت بنية object_id *oid,
			  ثابت حرف *filename, ثابت حرف *رستو, صحيح cached)
{
	بنية repository *subrepo;
	بنية repository *superproject = opt->repo;
	بنية grep_opt subopt;
	صحيح ڪاميابي = 0;

	إذا (!is_submodule_سرگرم(superproject, رستو))
		أرجع 0;

	subrepo = xmalloc(حجم(*subrepo));
	إذا (repo_submodule_init(subrepo, superproject, رستو, null_oid(opt->repo->hash_algo))) {
		حرر(subrepo);
		أرجع 0;
	}
	ALLOC_GROW(repos_منزل_free, repos_منزل_free_nr + 1, repos_منزل_free_alloc);
	repos_منزل_free[repos_منزل_free_nr++] = subrepo;

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
	obj_read_تالو();
	repo_read_gitmodules(subrepo, 0);
	obj_read_unlock();

	يادگيري_نقل(&subopt, opt, حجم(subopt));
	subopt.repo = subrepo;

	إذا (oid) {
		تعداد object_type object_type;
		بنية وڻ_desc وڻ;
		فراغ *معلومات;
		سائيز_t سائيز;
		بنية strbuf بنياد = STRBUF_INIT;

		obj_read_تالو();
		object_type = odb_read_object_info(subrepo->objects, oid, عدم);
		obj_read_unlock();
		معلومات = odb_read_object_peeled(subrepo->objects, oid, OBJ_وڻ, &سائيز, عدم);
		إذا (!معلومات)
			die(_("unable to read tree (%s)"), oid_منزل_hex(oid));

		strbuf_addstr(&بنياد, filename);
		strbuf_addch(&بنياد, '/');

		init_وڻ_desc(&وڻ, oid, معلومات, سائيز);
		ڪاميابي = grep_وڻ(&subopt, pathspec, &وڻ, &بنياد, بنياد.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&بنياد);
		حرر(معلومات);
	} وإلا {
		ڪاميابي = grep_cache(&subopt, pathspec, cached);
	}

	أرجع ڪاميابي;
}

ساكن صحيح grep_cache(بنية grep_opt *opt,
		      ثابت بنية pathspec *pathspec, صحيح cached)
{
	بنية repository *repo = opt->repo;
	صحيح ڪاميابي = 0;
	صحيح nr;
	بنية strbuf name = STRBUF_INIT;
	صحيح name_بنياد_len = 0;
	إذا (repo->submodule_اڳياڙي) {
		name_بنياد_len = لڙي_ڊيگهه(repo->submodule_اڳياڙي);
		strbuf_addstr(&name, repo->submodule_اڳياڙي);
	}

	إذا (repo_read_اشاريو(repo) < 0)
		die(_("index file corrupt"));

	لكل (nr = 0; nr < repo->اشاريو->cache_nr; nr++) {
		ثابت بنية cache_داخلا *ce = repo->اشاريو->cache[nr];

		إذا (!cached && ce_skip_worktree(ce))
			استمر;

		strbuf_setlen(&name, name_بنياد_len);
		strbuf_addstr(&name, ce->name);
		إذا (S_ISSPARSEDIR(ce->ce_گهڻائي)) {
			تعداد object_type type;
			بنية وڻ_desc وڻ;
			فراغ *معلومات;
			سائيز_t سائيز;

			معلومات = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &سائيز);
			إذا (!معلومات)
				die(_("unable to read tree %s"), oid_منزل_hex(&ce->oid));
			init_وڻ_desc(&وڻ, &ce->oid, معلومات, سائيز);

			ڪاميابي |= grep_وڻ(opt, pathspec, &وڻ, &name, 0, 0);
			strbuf_setlen(&name, name_بنياد_len);
			strbuf_addstr(&name, ce->name);
			حرر(معلومات);
		} وإلا إذا (S_ISREG(ce->ce_گهڻائي) &&
		    ميلاپ_pathspec(repo->اشاريو, pathspec, name.buf, name.len, 0, عدم,
				   S_ISDIR(ce->ce_گهڻائي) ||
				   S_ISGITLINK(ce->ce_گهڻائي))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			إذا (cached || (ce->ce_flags & CE_VALID)) {
				إذا (ce_stage(ce) || ce_intent_منزل_جوڙيو(ce))
					استمر;
				ڪاميابي |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} وإلا {
				ڪاميابي |= grep_file(opt, name.buf);
			}
		} وإلا إذا (recurse_submodules && S_ISGITLINK(ce->ce_گهڻائي) &&
			   submodule_رستو_ميلاپ(repo->اشاريو, pathspec, name.buf, عدم)) {
			ڪاميابي |= grep_submodule(opt, pathspec, عدم, ce->name,
					      ce->name, cached);
		} وإلا {
			استمر;
		}

		إذا (ce_stage(ce)) {
			افعل {
				nr++;
			} طالما (nr < repo->اشاريو->cache_nr &&
				 !strcmp(ce->name, repo->اشاريو->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		إذا (ڪاميابي && opt->حالت_only)
			اكسر;
	}

	strbuf_release(&name);
	أرجع ڪاميابي;
}

ساكن صحيح grep_وڻ(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
		     بنية وڻ_desc *وڻ, بنية strbuf *بنياد, صحيح tn_len,
		     صحيح check_attr)
{
	بنية repository *repo = opt->repo;
	صحيح ڪاميابي = 0;
	تعداد interesting ميلاپ = داخلا_not_interesting;
	بنية name_داخلا داخلا;
	صحيح old_baselen = بنياد->len;
	بنية strbuf name = STRBUF_INIT;
	صحيح name_بنياد_len = 0;
	إذا (repo->submodule_اڳياڙي) {
		strbuf_addstr(&name, repo->submodule_اڳياڙي);
		name_بنياد_len = name.len;
	}

	طالما (وڻ_داخلا(وڻ, &داخلا)) {
		صحيح te_len = وڻ_داخلا_len(&داخلا);

		إذا (ميلاپ != all_entries_interesting) {
			strbuf_addstr(&name, بنياد->buf + tn_len);
			ميلاپ = وڻ_داخلا_interesting(repo->اشاريو,
						       &داخلا, &name,
						       pathspec);
			strbuf_setlen(&name, name_بنياد_len);

			إذا (ميلاپ == all_entries_not_interesting)
				اكسر;
			إذا (ميلاپ == داخلا_not_interesting)
				استمر;
		}

		strbuf_جوڙيو(بنياد, داخلا.رستو, te_len);

		إذا (S_ISREG(داخلا.گهڻائي)) {
			ڪاميابي |= grep_oid(opt, &داخلا.oid, بنياد->buf, tn_len,
					 check_attr ? بنياد->buf + tn_len : عدم);
		} وإلا إذا (S_ISDIR(داخلا.گهڻائي)) {
			تعداد object_type type;
			بنية وڻ_desc sub;
			فراغ *معلومات;
			سائيز_t سائيز;

			معلومات = odb_read_object(the_repository->objects,
					       &داخلا.oid, &type, &سائيز);
			إذا (!معلومات)
				die(_("unable to read tree (%s)"),
				    oid_منزل_hex(&داخلا.oid));

			strbuf_addch(بنياد, '/');
			init_وڻ_desc(&sub, &داخلا.oid, معلومات, سائيز);
			ڪاميابي |= grep_وڻ(opt, pathspec, &sub, بنياد, tn_len,
					 check_attr);
			حرر(معلومات);
		} وإلا إذا (recurse_submodules && S_ISGITLINK(داخلا.گهڻائي)) {
			ڪاميابي |= grep_submodule(opt, pathspec, &داخلا.oid,
					      بنياد->buf, بنياد->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(بنياد, old_baselen);

		إذا (ڪاميابي && opt->حالت_only)
			اكسر;
	}

	strbuf_release(&name);
	أرجع ڪاميابي;
}

ساكن فراغ collect_blob_oids_for_وڻ(بنية repository *repo,
				       ثابت بنية pathspec *pathspec,
				       بنية وڻ_desc *وڻ,
				       بنية strbuf *بنياد,
				       صحيح tn_len,
				       بنية oidset *blob_oids)
{
	بنية name_داخلا داخلا;
	صحيح old_baselen = بنياد->len;
	بنية strbuf name = STRBUF_INIT;
	تعداد interesting ميلاپ = داخلا_not_interesting;

	طالما (وڻ_داخلا(وڻ, &داخلا)) {
		إذا (ميلاپ != all_entries_interesting) {
			strbuf_addstr(&name, بنياد->buf + tn_len);
			ميلاپ = وڻ_داخلا_interesting(repo->اشاريو,
						       &داخلا, &name,
						       pathspec);
			strbuf_reset(&name);

			إذا (ميلاپ == all_entries_not_interesting)
				اكسر;
			إذا (ميلاپ == داخلا_not_interesting)
				استمر;
		}

		strbuf_جوڙيو(بنياد, داخلا.رستو, وڻ_داخلا_len(&داخلا));

		إذا (S_ISREG(داخلا.گهڻائي)) {
			إذا (!odb_has_object(repo->objects, &داخلا.oid, 0))
				oidset_وجهو(blob_oids, &داخلا.oid);
		} وإلا إذا (S_ISDIR(داخلا.گهڻائي)) {
			تعداد object_type type;
			بنية وڻ_desc sub_وڻ;
			فراغ *معلومات;
			سائيز_t سائيز;

			معلومات = odb_read_object(repo->objects, &داخلا.oid,
					       &type, &سائيز);
			إذا (!معلومات)
				die(_("unable to read tree (%s)"),
				    oid_منزل_hex(&داخلا.oid));

			strbuf_addch(بنياد, '/');
			init_وڻ_desc(&sub_وڻ, &داخلا.oid, معلومات, سائيز);
			collect_blob_oids_for_وڻ(repo, pathspec, &sub_وڻ,
						   بنياد, tn_len, blob_oids);
			حرر(معلومات);
		}
		/*
		 * ...no else clause for S_ISGITLINK: submodules have their
		 * own promisor configuration and would need separate fetches
		 * anyway.
		 */

		strbuf_setlen(بنياد, old_baselen);
	}

	strbuf_release(&name);
}

ساكن فراغ collect_blob_oids_for_treeish(بنية grep_opt *opt,
					  ثابت بنية pathspec *pathspec,
					  ثابت بنية object_id *وڻ_ish_oid,
					  ثابت حرف *name,
					  بنية oidset *blob_oids)
{
	بنية وڻ_desc وڻ;
	فراغ *معلومات;
	سائيز_t سائيز;
	بنية strbuf بنياد = STRBUF_INIT;
	صحيح len;

	معلومات = odb_read_object_peeled(opt->repo->objects, وڻ_ish_oid,
				      OBJ_وڻ, &سائيز, عدم);

	إذا (!معلومات)
		أرجع;

	len = name ? لڙي_ڊيگهه(name) : 0;
	إذا (len) {
		strbuf_جوڙيو(&بنياد, name, len);
		strbuf_addch(&بنياد, ':');
	}
	init_وڻ_desc(&وڻ, وڻ_ish_oid, معلومات, سائيز);

	collect_blob_oids_for_وڻ(opt->repo, pathspec, &وڻ,
				   &بنياد, بنياد.len, blob_oids);

	strbuf_release(&بنياد);
	حرر(معلومات);
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

		obj_read_تالو();
		real_obj = deref_ٽيگ(opt->repo, list->objects[i].شي,
				     عدم, 0);
		obj_read_unlock();

		إذا (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_وڻ))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	إذا (oidset_سائيز(&blob_oids)) {
		بنية oid_array منزل_fetch = OID_ARRAY_INIT;
		بنية oidset_iter iter;
		ثابت بنية object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		طالما ((oid = oidset_iter_ايندڙ(&iter)))
			oid_array_append(&منزل_fetch, oid);

		promisor_remote_get_سڌو(opt->repo, منزل_fetch.oid, منزل_fetch.nr);

		oid_array_clear(&منزل_fetch);
	}
	oidset_clear(&blob_oids);
}

ساكن صحيح grep_object(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
		       بنية object *obj, ثابت حرف *name, ثابت حرف *رستو)
{
	إذا (obj->type == OBJ_BLOB)
		أرجع grep_oid(opt, &obj->oid, name, 0, رستو);
	إذا (obj->type == OBJ_COMMIT || obj->type == OBJ_وڻ) {
		بنية وڻ_desc وڻ;
		فراغ *معلومات;
		سائيز_t سائيز;
		بنية strbuf بنياد;
		صحيح ڪاميابي, len;

		معلومات = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_وڻ, &سائيز, عدم);
		إذا (!معلومات)
			die(_("unable to read tree (%s)"), oid_منزل_hex(&obj->oid));

		len = name ? لڙي_ڊيگهه(name) : 0;
		strbuf_init(&بنياد, رستو_MAX + len + 1);
		إذا (len) {
			strbuf_جوڙيو(&بنياد, name, len);
			strbuf_addch(&بنياد, ':');
		}
		init_وڻ_desc(&وڻ, &obj->oid, معلومات, سائيز);
		ڪاميابي = grep_وڻ(opt, pathspec, &وڻ, &بنياد, بنياد.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&بنياد);
		حرر(معلومات);
		أرجع ڪاميابي;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

ساكن صحيح grep_objects(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
			ثابت بنية object_array *list)
{
	بنانشان صحيح i;
	صحيح ڪاميابي = 0;
	ثابت بنانشان صحيح nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	لكل (i = 0; i < nr; i++) {
		بنية object *real_obj;

		obj_read_تالو();
		real_obj = deref_ٽيگ(opt->repo, list->objects[i].شي,
				     عدم, 0);
		obj_read_unlock();

		إذا (!real_obj) {
			حرف hex[GIT_MAX_HEXSZ + 1];
			ثابت حرف *name = list->objects[i].name;

			إذا (!name) {
				oid_منزل_hex_r(hex, &list->objects[i].شي->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		إذا (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_تالو();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		إذا (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].رستو)) {
			ڪاميابي = 1;
			إذا (opt->حالت_only)
				اكسر;
		}
	}
	أرجع ڪاميابي;
}

ساكن صحيح grep_directory(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
			  صحيح exc_std, صحيح use_اشاريو)
{
	بنية dir_struct dir = DIR_INIT;
	صحيح i, ڪاميابي = 0;

	إذا (!use_اشاريو)
		dir.flags |= DIR_NO_GITLINKS;
	إذا (exc_std)
		تياري_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->اشاريو, pathspec);
	لكل (i = 0; i < dir.nr; i++) {
		ڪاميابي |= grep_file(opt, dir.entries[i]->name);
		إذا (ڪاميابي && opt->حالت_only)
			اكسر;
	}
	dir_clear(&dir);
	أرجع ڪاميابي;
}

ساكن صحيح context_callback(ثابت بنية option *opt, ثابت حرف *arg,
			    صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قدر;
	صحيح قدر;
	ثابت حرف *endp;

	إذا (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		أرجع 0;
	}
	قدر = strtol(arg, (حرف **)&endp, 10);
	إذا (*endp) {
		أرجع error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = قدر;
	أرجع 0;
}

ساكن صحيح file_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قدر;
	صحيح from_stdin;
	ثابت حرف *filename = arg;
	FILE *patterns;
	صحيح lno = 0;
	بنية strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	إذا (!*filename)
		; /* leave it as-is */
	وإلا
		filename = اڳياڙي_filename_except_for_dash(grep_اڳياڙي, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	إذا (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	طالما (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		إذا (sb.len == 0)
			استمر;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_نمونو);
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
	بنية grep_opt *grep_opt = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_نمونو(grep_opt, "--not", "command line", 0, GREP_NOT);
	أرجع 0;
}

ساكن صحيح and_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_نمونو(grep_opt, "--and", "command line", 0, GREP_AND);
	أرجع 0;
}

ساكن صحيح open_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_نمونو(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	أرجع 0;
}

ساكن صحيح close_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_نمونو(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	أرجع 0;
}

ساكن صحيح نمونو_callback(ثابت بنية option *opt, ثابت حرف *arg,
			    صحيح unset)
{
	بنية grep_opt *grep_opt = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	append_grep_نمونو(grep_opt, arg, "-e option", 0, GREP_نمونو);
	أرجع 0;
}

صحيح cmd_grep(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *اڳياڙي,
	     بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح ڪاميابي = 0;
	صحيح cached = 0, untracked = 0, opt_exclude = -1;
	صحيح ڏٺل_dashdash = 0;
	صحيح external_grep_allowed__ignored;
	ثابت حرف *show_اندريون_pager = عدم, *default_pager = "dummy";
	بنية grep_opt opt;
	بنية object_array list = OBJECT_ARRAY_INIT;
	بنية pathspec pathspec;
	بنية string_list رستو_list = STRING_LIST_INIT_DUP;
	صحيح i;
	صحيح dummy;
	صحيح use_اشاريو = 1;
	صحيح allow_revs;
	صحيح ret;

	بنية option options[] = {
		OPT_BOOL(0, "cached", &cached,
			N_("search in index instead of in the work tree")),
		OPT_NEGBIT(0, "no-index", &use_اشاريو,
			 N_("find in contents not managed by git"), 1),
		OPT_BOOL(0, "untracked", &untracked,
			N_("search in both tracked and untracked files")),
		OPT_سيٽ_INT(0, "exclude-standard", &opt_exclude,
			    N_("ignore files specified via '.gitignore'"), 1),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			 N_("recursively search in each submodule")),
		OPT_گروهه(""),
		OPT_BOOL('v', "invert-match", &opt.invert,
			N_("show non-matching lines")),
		OPT_BOOL('i', "ignore-case", &opt.ignore_case,
			N_("case insensitive matching")),
		OPT_BOOL('w', "word-regexp", &opt.لفظ_regexp,
			N_("match patterns only at word boundaries")),
		OPT_سيٽ_INT('a', "text", &opt.ٻٽو,
			N_("process binary files as text"), GREP_ٻٽو_متن),
		OPT_سيٽ_INT('I', عدم, &opt.ٻٽو,
			N_("don't match patterns in binary files"),
			GREP_ٻٽو_NOMATCH),
		OPT_BOOL(0, "textconv", &opt.allow_textconv,
			 N_("process binary files with textconv filters")),
		OPT_سيٽ_INT('r', "recursive", &opt.max_اونهائي,
			    N_("search in subdirectories (default)"), -1),
		OPT_INTEGER_F(0, "max-depth", &opt.max_اونهائي,
			N_("descend at most <n> levels"), PARSE_OPT_NONEG),
		OPT_گروهه(""),
		OPT_سيٽ_INT('E', "extended-regexp", &opt.نمونو_type_option,
			    N_("use extended POSIX regular expressions"),
			    GREP_نمونو_TYPE_ERE),
		OPT_سيٽ_INT('G', "basic-regexp", &opt.نمونو_type_option,
			    N_("use basic POSIX regular expressions (default)"),
			    GREP_نمونو_TYPE_BRE),
		OPT_سيٽ_INT('F', "fixed-strings", &opt.نمونو_type_option,
			    N_("interpret patterns as fixed strings"),
			    GREP_نمونو_TYPE_FIXED),
		OPT_سيٽ_INT('P', "perl-regexp", &opt.نمونو_type_option,
			    N_("use Perl-compatible regular expressions"),
			    GREP_نمونو_TYPE_PCRE),
		OPT_گروهه(""),
		OPT_BOOL('n', "line-number", &opt.linenum, N_("show line numbers")),
		OPT_BOOL(0, "column", &opt.columnnum, N_("show column number of first match")),
		OPT_NEGBIT('h', عدم, &opt.pathname, N_("don't show filenames"), 1),
		OPT_ٻٽو_انگ('H', عدم, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.ڳڻپ,
			N_("show the number of matches instead of matching lines")),
		OPT__COLOR(&opt.color, N_("highlight matches")),
		OPT_BOOL(0, "break", &opt.file_break,
			N_("print empty line between matches from different files")),
		OPT_BOOL(0, "heading", &opt.heading,
			N_("show filename only once above matches from same file")),
		OPT_گروهه(""),
		OPT_CALLBACK('C', "context", &opt, N_("n"),
			N_("show <n> context lines before and after matches"),
			context_callback),
		OPT_UNSIGNED('B', "before-context", &opt.pre_context,
			N_("show <n> context lines before matches")),
		OPT_UNSIGNED('A', "after-context", &opt.post_context,
			N_("show <n> context lines after matches")),
		OPT_INTEGER(0, "threads", &انگ_ڌاڳا,
			N_("use <n> worker threads")),
		OPT_عدد_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_گروهه(""),
		OPT_CALLBACK('f', عدم, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', عدم, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, نمونو_callback),
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
		OPT__QUIET(&opt.حالت_only,
			   N_("indicate hit with exit status without output")),
		OPT_BOOL(0, "all-match", &opt.all_ميلاپ,
			N_("show only matches from files that match all patterns")),
		OPT_گروهه(""),
		{
			.type = OPTION_STRING,
			.short_name = 'O',
			.long_name = "open-files-in-pager",
			.قدر = &show_اندريون_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_ڳڻپ,
			N_("maximum number of results per file")),
		OPT_پڇاڙي()
	};
	grep_اڳياڙي = اڳياڙي;

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
	argc = parse_options(argc, argv, اڳياڙي, options, grep_usage,
			     PARSE_OPT_KEEP_DASHDASH |
			     PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ڀريل_اشاريو = 0;
	}

	إذا (use_اشاريو && !startup_info->have_repository) {
		صحيح fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		إذا (fallback)
			use_اشاريو = 0;
		وإلا
			/* die the same way as if we did it at the beginning */
			تياري_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	إذا (!use_اشاريو)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	إذا (argc > 0 && !opt.نمونو_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	إذا (argc > 0 && !opt.نمونو_list) {
		append_grep_نمونو(&opt, argv[0], "command line", 0,
				    GREP_نمونو);
		argv++;
		argc--;
	}

	إذا (show_اندريون_pager == default_pager)
		show_اندريون_pager = git_pager(the_repository, 1);
	إذا (show_اندريون_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &رستو_list;
		opt.output = append_رستو;
		string_list_append(&رستو_list, show_اندريون_pager);
	}

	إذا (!opt.نمونو_list)
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
			ڏٺل_dashdash = 1;
			اكسر;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_اشاريو && !untracked;
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
			إذا (ڏٺل_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			اكسر;
		}

		إذا (get_oid_with_context(the_repository, arg,
					 GET_OID_رڪارڊ_رستو,
					 &oid, &oc)) {
			إذا (ڏٺل_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			اكسر;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		إذا (!ڏٺل_dashdash)
			verify_non_filename(the_repository, اڳياڙي, arg);
		جوڙيو_object_array_with_رستو(object, arg, &list, oc.گهڻائي, oc.رستو);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	إذا (!ڏٺل_dashdash) {
		صحيح j;
		لكل (j = i; j < argc; j++)
			verify_filename(the_repository, اڳياڙي, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_اونهائي != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       اڳياڙي, argv + i);
	pathspec.max_اونهائي = opt.max_اونهائي;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	إذا (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	إذا (opt.max_ڳڻپ == 0) {
		ret = 1;
		اذهب ٻاهريون;
	}

	إذا (show_اندريون_pager) {
		إذا (انگ_ڌاڳا > 1)
			warning(_("invalid option combination, ignoring --threads"));
		انگ_ڌاڳا = 1;
	} وإلا إذا (!HAVE_ڌاڳا && انگ_ڌاڳا > 1) {
		warning(_("no threads support, ignoring --threads"));
		انگ_ڌاڳا = 1;
	} وإلا إذا (انگ_ڌاڳا < 0)
		die(_("invalid number of threads specified (%d)"), انگ_ڌاڳا);
	وإلا إذا (انگ_ڌاڳا == 0)
		انگ_ڌاڳا = HAVE_ڌاڳا ? online_cpus() : 1;

	إذا (انگ_ڌاڳا > 1) {
		إذا (!HAVE_ڌاڳا)
			BUG("Somebody got num_threads calculation wrong!");
		إذا (!(opt.name_only || opt.unmatch_name_only || opt.ڳڻپ)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_پهريون_سٽ = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		إذا (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		إذا (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		شروعات_ڌاڳا(&opt);
	} وإلا {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	إذا (show_اندريون_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	إذا (show_اندريون_pager && opt.نمونو_list && !opt.نمونو_list->ايندڙ) {
		ثابت حرف *pager = رستو_list.شيون[0].string;
		صحيح len = لڙي_ڊيگهه(pager);

		إذا (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		إذا (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&رستو_list, "-I");

		إذا (!strcmp("less", pager) || !strcmp("vi", pager)) {
			بنية strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.نمونو_list->نمونو);
			string_list_append_nodup(&رستو_list,
						 strbuf_detach(&buf, عدم));
		}
	}

	إذا (!show_اندريون_pager && !opt.حالت_only)
		تياري_pager(the_repository);

	die_for_incompatible_opt3(!use_اشاريو, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	إذا (!use_اشاريو || untracked) {
		صحيح use_exclude = (opt_exclude < 0) ? use_اشاريو : !!opt_exclude;
		ڪاميابي = grep_directory(&opt, &pathspec, use_exclude, use_اشاريو);
	} وإلا إذا (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} وإلا إذا (!list.nr) {
		إذا (!cached)
			تياري_work_وڻ(the_repository);

		ڪاميابي = grep_cache(&opt, &pathspec, cached);
	} وإلا {
		إذا (cached)
			die(_("both --cached and trees are given"));

		ڪاميابي = grep_objects(&opt, &pathspec, &list);
	}

	إذا (انگ_ڌاڳا > 1)
		ڪاميابي |= wait_all();
	إذا (ڪاميابي && show_اندريون_pager)
		لاڳيتو_pager(&opt, اڳياڙي);

	ret = !ڪاميابي;

ٻاهريون:
	clear_pathspec(&pathspec);
	string_list_clear(&رستو_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	أرجع ret;
}
