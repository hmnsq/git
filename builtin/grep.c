/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن ثابت حرف *grep_مختاړی;

ساكن حرف ثابت * ثابت grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	عدم
};

ساكن صحيح recurse_submodules;

ساكن صحيح عدد_تارونه;

ساكن pthread_t *تارونه;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
بنية work_توکی {
	بنية grep_سرچینه سرچینه;
	حرف بشپړ;
	بنية strbuf بهر;
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
#تعریف_کړه TODO_اندازه 128
ساكن بنية work_توکی todo[TODO_اندازه];
ساكن صحيح todo_پیل;
ساكن صحيح todo_پای_ټکی;
ساكن صحيح todo_بشپړ;

/* Has all work items been added? */
ساكن صحيح all_work_added;

ساكن بنية repository **repos_منزل_free;
ساكن اندازه_t repos_منزل_free_nr, repos_منزل_free_alloc;

/* This lock protects all the variables above. */
ساكن pthread_mutex_t grep_mutex;

ساكن مضمن فراغ grep_قلف(فراغ)
{
	pthread_mutex_قلف(&grep_mutex);
}

ساكن مضمن فراغ grep_unlock(فراغ)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
ساكن pthread_cond_t cond_جمع_کړه;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
ساكن pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
ساكن pthread_cond_t cond_پایله;

ساكن صحيح skip_لومړی_کرښه;

ساكن فراغ جمع_کړه_work(بنية grep_opt *opt, بنية grep_سرچینه *gs)
{
	إذا (opt->دوګونی != GREP_دوګونی_متن)
		grep_سرچینه_بارول_driver(gs, opt->repo->شاخص);

	grep_قلف();

	طالما ((todo_پای_ټکی+1) % ARRAY_اندازه(todo) == todo_بشپړ) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_پای_ټکی].سرچینه = *gs;
	todo[todo_پای_ټکی].بشپړ = 0;
	strbuf_reset(&todo[todo_پای_ټکی].بهر);
	todo_پای_ټکی = (todo_پای_ټکی + 1) % ARRAY_اندازه(todo);

	pthread_cond_signal(&cond_جمع_کړه);
	grep_unlock();
}

ساكن بنية work_توکی *get_work(فراغ)
{
	بنية work_توکی *ret;

	grep_قلف();
	طالما (todo_پیل == todo_پای_ټکی && !all_work_added) {
		pthread_cond_wait(&cond_جمع_کړه, &grep_mutex);
	}

	إذا (todo_پیل == todo_پای_ټکی && all_work_added) {
		ret = عدم;
	} وإلا {
		ret = &todo[todo_پیل];
		todo_پیل = (todo_پیل + 1) % ARRAY_اندازه(todo);
	}
	grep_unlock();
	أرجع ret;
}

ساكن فراغ work_بشپړ(بنية work_توکی *w)
{
	صحيح old_بشپړ;

	grep_قلف();
	w->بشپړ = 1;
	old_بشپړ = todo_بشپړ;
	لكل(; todo[todo_بشپړ].بشپړ && todo_بشپړ != todo_پیل;
	    todo_بشپړ = (todo_بشپړ+1) % ARRAY_اندازه(todo)) {
		w = &todo[todo_بشپړ];
		إذا (w->بهر.len) {
			ثابت حرف *p = w->بهر.buf;
			اندازه_t len = w->بهر.len;

			/* Skip the leading hunk mark of the first file. */
			إذا (skip_لومړی_کرښه) {
				طالما (len) {
					len--;
					إذا (*p++ == '\n')
						اكسر;
				}
				skip_لومړی_کرښه = 0;
			}

			write_or_die(1, p, len);
		}
		grep_سرچینه_clear(&w->سرچینه);
	}

	إذا (old_بشپړ != todo_بشپړ)
		pthread_cond_signal(&cond_write);

	إذا (all_work_added && todo_بشپړ == todo_پای_ټکی)
		pthread_cond_signal(&cond_پایله);

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

ساكن فراغ *پرلپسې(فراغ *arg)
{
	صحيح موندنه = 0;
	بنية grep_opt *opt = arg;

	طالما (1) {
		بنية work_توکی *w = get_work();
		إذا (!w)
			اكسر;

		opt->output_priv = w;
		موندنه |= grep_سرچینه(opt, &w->سرچینه);
		grep_سرچینه_clear_معلومات(&w->سرچینه);
		work_بشپړ(w);
	}
	free_grep_patterns(opt);
	حرر(opt);

	أرجع (فراغ*) (intptr_t) موندنه;
}

ساكن فراغ strbuf_بهر(بنية grep_opt *opt, ثابت فراغ *buf, اندازه_t اندازه)
{
	بنية work_توکی *w = opt->output_priv;
	strbuf_جمع_کړه(&w->بهر, buf, اندازه);
}

ساكن فراغ پیل_تارونه(بنية grep_opt *opt)
{
	صحيح i;

	pthread_mutex_init(&grep_mutex, عدم);
	pthread_mutex_init(&grep_attr_mutex, عدم);
	pthread_cond_init(&cond_جمع_کړه, عدم);
	pthread_cond_init(&cond_write, عدم);
	pthread_cond_init(&cond_پایله, عدم);
	grep_use_locks = 1;
	enable_obj_read_قلف();

	لكل (i = 0; i < ARRAY_اندازه(todo); i++) {
		strbuf_init(&todo[i].بهر, 0);
	}

	CALLOC_ARRAY(تارونه, عدد_تارونه);
	لكل (i = 0; i < عدد_تارونه; i++) {
		صحيح err;
		بنية grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_بهر;
		compile_grep_patterns(o);
		err = pthread_create(&تارونه[i], عدم, پرلپسې, o);

		إذا (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

ساكن صحيح wait_all(فراغ)
{
	صحيح موندنه = 0;
	صحيح i;

	إذا (!HAVE_تارونه)
		BUG("Never call this function unless you have started threads");

	grep_قلف();
	all_work_added = 1;

	/* Wait until all work is done. */
	طالما (todo_بشپړ != todo_پای_ټکی)
		pthread_cond_wait(&cond_پایله, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_خپرونه(&cond_جمع_کړه);
	grep_unlock();

	لكل (i = 0; i < عدد_تارونه; i++) {
		فراغ *h;
		pthread_join(تارونه[i], &h);
		موندنه |= (صحيح) (intptr_t) h;
	}

	حرر(تارونه);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_جمع_کړه);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_پایله);
	grep_use_locks = 0;
	disable_obj_read_قلف();

	أرجع موندنه;
}

ساكن صحيح grep_cmd_config(ثابت حرف *var, ثابت حرف *ارزښت,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	صحيح st = grep_config(var, ارزښت, ctx, cb);

	إذا (git_color_config(var, ارزښت, cb) < 0)
		st = -1;
	وإلا إذا (git_default_config(var, ارزښت, ctx, cb) < 0)
		st = -1;

	إذا (!strcmp(var, "grep.threads")) {
		عدد_تارونه = git_config_int(var, ارزښت, ctx->kvi);
		إذا (عدد_تارونه < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    عدد_تارونه, var);
		وإلا إذا (!HAVE_تارونه && عدد_تارونه > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			عدد_تارونه = 1;
		}
	}

	إذا (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, ارزښت);

	أرجع st;
}

ساكن فراغ grep_سرچینه_name(بنية grep_opt *opt, ثابت حرف *filename,
			     صحيح ونه_name_len, بنية strbuf *بهر)
{
	strbuf_reset(بهر);

	إذا (opt->null_following_name) {
		إذا (opt->relative && grep_مختاړی) {
			بنية strbuf rel_buf = STRBUF_INIT;
			ثابت حرف *rel_name =
				relative_لار(filename + ونه_name_len,
					      grep_مختاړی, &rel_buf);

			إذا (ونه_name_len)
				strbuf_جمع_کړه(بهر, filename, ونه_name_len);

			strbuf_addstr(بهر, rel_name);
			strbuf_release(&rel_buf);
		} وإلا {
			strbuf_addstr(بهر, filename);
		}
		أرجع;
	}

	إذا (opt->relative && grep_مختاړی)
		quote_لار(filename + ونه_name_len, grep_مختاړی, بهر, 0);
	وإلا
		quote_c_style(filename + ونه_name_len, بهر, عدم, 0);

	إذا (ونه_name_len)
		strbuf_ورننباسه(بهر, 0, filename, ونه_name_len);
}

ساكن صحيح grep_oid(بنية grep_opt *opt, ثابت بنية object_id *oid,
		     ثابت حرف *filename, صحيح ونه_name_len,
		     ثابت حرف *لار)
{
	بنية strbuf pathbuf = STRBUF_INIT;
	بنية grep_سرچینه gs;

	grep_سرچینه_name(opt, filename, ونه_name_len, &pathbuf);
	grep_سرچینه_init_oid(&gs, pathbuf.buf, لار, oid, opt->repo);
	strbuf_release(&pathbuf);

	إذا (عدد_تارونه > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		جمع_کړه_work(opt, &gs);
		أرجع 0;
	} وإلا {
		صحيح موندنه;

		موندنه = grep_سرچینه(opt, &gs);

		grep_سرچینه_clear(&gs);
		أرجع موندنه;
	}
}

ساكن صحيح grep_file(بنية grep_opt *opt, ثابت حرف *filename)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية grep_سرچینه gs;

	grep_سرچینه_name(opt, filename, 0, &buf);
	grep_سرچینه_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	إذا (عدد_تارونه > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		جمع_کړه_work(opt, &gs);
		أرجع 0;
	} وإلا {
		صحيح موندنه;

		موندنه = grep_سرچینه(opt, &gs);

		grep_سرچینه_clear(&gs);
		أرجع موندنه;
	}
}

ساكن فراغ append_لار(بنية grep_opt *opt, ثابت فراغ *معلومات, اندازه_t len)
{
	بنية string_list *لار_list = opt->output_priv;

	إذا (len == 1 && *(ثابت حرف *)معلومات == '\0')
		أرجع;
	string_list_append_nodup(لار_list, xstrndup(معلومات, len));
}

ساكن فراغ پرلپسې_pager(بنية grep_opt *opt, ثابت حرف *مختاړی)
{
	بنية string_list *لار_list = opt->output_priv;
	بنية ماشوم_process ماشوم = ماشوم_PROCESS_INIT;
	صحيح i, حالت;

	لكل (i = 0; i < لار_list->nr; i++)
		strvec_ټېل(&ماشوم.دلیلونه, لار_list->توکي[i].string);
	ماشوم.dir = مختاړی;
	ماشوم.use_shell = 1;

	حالت = پرلپسې_command(&ماشوم);
	إذا (حالت)
		وتل(حالت);
}

ساكن صحيح grep_cache(بنية grep_opt *opt,
		      ثابت بنية pathspec *pathspec, صحيح cached);
ساكن صحيح grep_ونه(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
		     بنية ونه_desc *ونه, بنية strbuf *بنسټ, صحيح tn_len,
		     صحيح check_attr);

ساكن صحيح grep_submodule(بنية grep_opt *opt,
			  ثابت بنية pathspec *pathspec,
			  ثابت بنية object_id *oid,
			  ثابت حرف *filename, ثابت حرف *لار, صحيح cached)
{
	بنية repository *subrepo;
	بنية repository *superproject = opt->repo;
	بنية grep_opt subopt;
	صحيح موندنه = 0;

	إذا (!is_submodule_فعال(superproject, لار))
		أرجع 0;

	subrepo = xmalloc(حجم(*subrepo));
	إذا (repo_submodule_init(subrepo, superproject, لار, null_oid(opt->repo->hash_algo))) {
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
	obj_read_قلف();
	repo_read_gitmodules(subrepo, 0);
	obj_read_unlock();

	حافظه_نقل(&subopt, opt, حجم(subopt));
	subopt.repo = subrepo;

	إذا (oid) {
		تعداد object_type object_type;
		بنية ونه_desc ونه;
		فراغ *معلومات;
		اندازه_t اندازه;
		بنية strbuf بنسټ = STRBUF_INIT;

		obj_read_قلف();
		object_type = odb_read_object_info(subrepo->objects, oid, عدم);
		obj_read_unlock();
		معلومات = odb_read_object_peeled(subrepo->objects, oid, OBJ_ونه, &اندازه, عدم);
		إذا (!معلومات)
			die(_("unable to read tree (%s)"), oid_منزل_hex(oid));

		strbuf_addstr(&بنسټ, filename);
		strbuf_addch(&بنسټ, '/');

		init_ونه_desc(&ونه, oid, معلومات, اندازه);
		موندنه = grep_ونه(&subopt, pathspec, &ونه, &بنسټ, بنسټ.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&بنسټ);
		حرر(معلومات);
	} وإلا {
		موندنه = grep_cache(&subopt, pathspec, cached);
	}

	أرجع موندنه;
}

ساكن صحيح grep_cache(بنية grep_opt *opt,
		      ثابت بنية pathspec *pathspec, صحيح cached)
{
	بنية repository *repo = opt->repo;
	صحيح موندنه = 0;
	صحيح nr;
	بنية strbuf name = STRBUF_INIT;
	صحيح name_بنسټ_len = 0;
	إذا (repo->submodule_مختاړی) {
		name_بنسټ_len = مزي_اوږدوالی(repo->submodule_مختاړی);
		strbuf_addstr(&name, repo->submodule_مختاړی);
	}

	إذا (repo_read_شاخص(repo) < 0)
		die(_("index file corrupt"));

	لكل (nr = 0; nr < repo->شاخص->cache_nr; nr++) {
		ثابت بنية cache_ثبت *ce = repo->شاخص->cache[nr];

		إذا (!cached && ce_skip_worktree(ce))
			استمر;

		strbuf_setlen(&name, name_بنسټ_len);
		strbuf_addstr(&name, ce->name);
		إذا (S_ISSPARSEDIR(ce->ce_ډېرپېښ)) {
			تعداد object_type type;
			بنية ونه_desc ونه;
			فراغ *معلومات;
			اندازه_t اندازه;

			معلومات = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &اندازه);
			إذا (!معلومات)
				die(_("unable to read tree %s"), oid_منزل_hex(&ce->oid));
			init_ونه_desc(&ونه, &ce->oid, معلومات, اندازه);

			موندنه |= grep_ونه(opt, pathspec, &ونه, &name, 0, 0);
			strbuf_setlen(&name, name_بنسټ_len);
			strbuf_addstr(&name, ce->name);
			حرر(معلومات);
		} وإلا إذا (S_ISREG(ce->ce_ډېرپېښ) &&
		    سمون_pathspec(repo->شاخص, pathspec, name.buf, name.len, 0, عدم,
				   S_ISDIR(ce->ce_ډېرپېښ) ||
				   S_ISGITLINK(ce->ce_ډېرپېښ))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			إذا (cached || (ce->ce_flags & CE_VALID)) {
				إذا (ce_stage(ce) || ce_intent_منزل_جمع_کړه(ce))
					استمر;
				موندنه |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} وإلا {
				موندنه |= grep_file(opt, name.buf);
			}
		} وإلا إذا (recurse_submodules && S_ISGITLINK(ce->ce_ډېرپېښ) &&
			   submodule_لار_سمون(repo->شاخص, pathspec, name.buf, عدم)) {
			موندنه |= grep_submodule(opt, pathspec, عدم, ce->name,
					      ce->name, cached);
		} وإلا {
			استمر;
		}

		إذا (ce_stage(ce)) {
			افعل {
				nr++;
			} طالما (nr < repo->شاخص->cache_nr &&
				 !strcmp(ce->name, repo->شاخص->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		إذا (موندنه && opt->حالت_only)
			اكسر;
	}

	strbuf_release(&name);
	أرجع موندنه;
}

ساكن صحيح grep_ونه(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
		     بنية ونه_desc *ونه, بنية strbuf *بنسټ, صحيح tn_len,
		     صحيح check_attr)
{
	بنية repository *repo = opt->repo;
	صحيح موندنه = 0;
	تعداد interesting سمون = ثبت_not_interesting;
	بنية name_ثبت ثبت;
	صحيح old_baselen = بنسټ->len;
	بنية strbuf name = STRBUF_INIT;
	صحيح name_بنسټ_len = 0;
	إذا (repo->submodule_مختاړی) {
		strbuf_addstr(&name, repo->submodule_مختاړی);
		name_بنسټ_len = name.len;
	}

	طالما (ونه_ثبت(ونه, &ثبت)) {
		صحيح te_len = ونه_ثبت_len(&ثبت);

		إذا (سمون != all_entries_interesting) {
			strbuf_addstr(&name, بنسټ->buf + tn_len);
			سمون = ونه_ثبت_interesting(repo->شاخص,
						       &ثبت, &name,
						       pathspec);
			strbuf_setlen(&name, name_بنسټ_len);

			إذا (سمون == all_entries_not_interesting)
				اكسر;
			إذا (سمون == ثبت_not_interesting)
				استمر;
		}

		strbuf_جمع_کړه(بنسټ, ثبت.لار, te_len);

		إذا (S_ISREG(ثبت.ډېرپېښ)) {
			موندنه |= grep_oid(opt, &ثبت.oid, بنسټ->buf, tn_len,
					 check_attr ? بنسټ->buf + tn_len : عدم);
		} وإلا إذا (S_ISDIR(ثبت.ډېرپېښ)) {
			تعداد object_type type;
			بنية ونه_desc sub;
			فراغ *معلومات;
			اندازه_t اندازه;

			معلومات = odb_read_object(the_repository->objects,
					       &ثبت.oid, &type, &اندازه);
			إذا (!معلومات)
				die(_("unable to read tree (%s)"),
				    oid_منزل_hex(&ثبت.oid));

			strbuf_addch(بنسټ, '/');
			init_ونه_desc(&sub, &ثبت.oid, معلومات, اندازه);
			موندنه |= grep_ونه(opt, pathspec, &sub, بنسټ, tn_len,
					 check_attr);
			حرر(معلومات);
		} وإلا إذا (recurse_submodules && S_ISGITLINK(ثبت.ډېرپېښ)) {
			موندنه |= grep_submodule(opt, pathspec, &ثبت.oid,
					      بنسټ->buf, بنسټ->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(بنسټ, old_baselen);

		إذا (موندنه && opt->حالت_only)
			اكسر;
	}

	strbuf_release(&name);
	أرجع موندنه;
}

ساكن فراغ collect_blob_oids_for_ونه(بنية repository *repo,
				       ثابت بنية pathspec *pathspec,
				       بنية ونه_desc *ونه,
				       بنية strbuf *بنسټ,
				       صحيح tn_len,
				       بنية oidset *blob_oids)
{
	بنية name_ثبت ثبت;
	صحيح old_baselen = بنسټ->len;
	بنية strbuf name = STRBUF_INIT;
	تعداد interesting سمون = ثبت_not_interesting;

	طالما (ونه_ثبت(ونه, &ثبت)) {
		إذا (سمون != all_entries_interesting) {
			strbuf_addstr(&name, بنسټ->buf + tn_len);
			سمون = ونه_ثبت_interesting(repo->شاخص,
						       &ثبت, &name,
						       pathspec);
			strbuf_reset(&name);

			إذا (سمون == all_entries_not_interesting)
				اكسر;
			إذا (سمون == ثبت_not_interesting)
				استمر;
		}

		strbuf_جمع_کړه(بنسټ, ثبت.لار, ونه_ثبت_len(&ثبت));

		إذا (S_ISREG(ثبت.ډېرپېښ)) {
			إذا (!odb_has_object(repo->objects, &ثبت.oid, 0))
				oidset_ورننباسه(blob_oids, &ثبت.oid);
		} وإلا إذا (S_ISDIR(ثبت.ډېرپېښ)) {
			تعداد object_type type;
			بنية ونه_desc sub_ونه;
			فراغ *معلومات;
			اندازه_t اندازه;

			معلومات = odb_read_object(repo->objects, &ثبت.oid,
					       &type, &اندازه);
			إذا (!معلومات)
				die(_("unable to read tree (%s)"),
				    oid_منزل_hex(&ثبت.oid));

			strbuf_addch(بنسټ, '/');
			init_ونه_desc(&sub_ونه, &ثبت.oid, معلومات, اندازه);
			collect_blob_oids_for_ونه(repo, pathspec, &sub_ونه,
						   بنسټ, tn_len, blob_oids);
			حرر(معلومات);
		}
		/*
		 * ...no else clause for S_ISGITLINK: submodules have their
		 * own promisor configuration and would need separate fetches
		 * anyway.
		 */

		strbuf_setlen(بنسټ, old_baselen);
	}

	strbuf_release(&name);
}

ساكن فراغ collect_blob_oids_for_treeish(بنية grep_opt *opt,
					  ثابت بنية pathspec *pathspec,
					  ثابت بنية object_id *ونه_ish_oid,
					  ثابت حرف *name,
					  بنية oidset *blob_oids)
{
	بنية ونه_desc ونه;
	فراغ *معلومات;
	اندازه_t اندازه;
	بنية strbuf بنسټ = STRBUF_INIT;
	صحيح len;

	معلومات = odb_read_object_peeled(opt->repo->objects, ونه_ish_oid,
				      OBJ_ونه, &اندازه, عدم);

	إذا (!معلومات)
		أرجع;

	len = name ? مزي_اوږدوالی(name) : 0;
	إذا (len) {
		strbuf_جمع_کړه(&بنسټ, name, len);
		strbuf_addch(&بنسټ, ':');
	}
	init_ونه_desc(&ونه, ونه_ish_oid, معلومات, اندازه);

	collect_blob_oids_for_ونه(opt->repo, pathspec, &ونه,
				   &بنسټ, بنسټ.len, blob_oids);

	strbuf_release(&بنسټ);
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

		obj_read_قلف();
		real_obj = deref_نښان(opt->repo, list->objects[i].توکی,
				     عدم, 0);
		obj_read_unlock();

		إذا (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_ونه))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	إذا (oidset_اندازه(&blob_oids)) {
		بنية oid_array منزل_fetch = OID_ARRAY_INIT;
		بنية oidset_iter iter;
		ثابت بنية object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		طالما ((oid = oidset_iter_بل(&iter)))
			oid_array_append(&منزل_fetch, oid);

		promisor_remote_get_مستقیم(opt->repo, منزل_fetch.oid, منزل_fetch.nr);

		oid_array_clear(&منزل_fetch);
	}
	oidset_clear(&blob_oids);
}

ساكن صحيح grep_object(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
		       بنية object *obj, ثابت حرف *name, ثابت حرف *لار)
{
	إذا (obj->type == OBJ_BLOB)
		أرجع grep_oid(opt, &obj->oid, name, 0, لار);
	إذا (obj->type == OBJ_COMMIT || obj->type == OBJ_ونه) {
		بنية ونه_desc ونه;
		فراغ *معلومات;
		اندازه_t اندازه;
		بنية strbuf بنسټ;
		صحيح موندنه, len;

		معلومات = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_ونه, &اندازه, عدم);
		إذا (!معلومات)
			die(_("unable to read tree (%s)"), oid_منزل_hex(&obj->oid));

		len = name ? مزي_اوږدوالی(name) : 0;
		strbuf_init(&بنسټ, لار_MAX + len + 1);
		إذا (len) {
			strbuf_جمع_کړه(&بنسټ, name, len);
			strbuf_addch(&بنسټ, ':');
		}
		init_ونه_desc(&ونه, &obj->oid, معلومات, اندازه);
		موندنه = grep_ونه(opt, pathspec, &ونه, &بنسټ, بنسټ.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&بنسټ);
		حرر(معلومات);
		أرجع موندنه;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

ساكن صحيح grep_objects(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
			ثابت بنية object_array *list)
{
	بینښه صحيح i;
	صحيح موندنه = 0;
	ثابت بینښه صحيح nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	لكل (i = 0; i < nr; i++) {
		بنية object *real_obj;

		obj_read_قلف();
		real_obj = deref_نښان(opt->repo, list->objects[i].توکی,
				     عدم, 0);
		obj_read_unlock();

		إذا (!real_obj) {
			حرف hex[GIT_MAX_HEXSZ + 1];
			ثابت حرف *name = list->objects[i].name;

			إذا (!name) {
				oid_منزل_hex_r(hex, &list->objects[i].توکی->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		إذا (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_قلف();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		إذا (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].لار)) {
			موندنه = 1;
			إذا (opt->حالت_only)
				اكسر;
		}
	}
	أرجع موندنه;
}

ساكن صحيح grep_directory(بنية grep_opt *opt, ثابت بنية pathspec *pathspec,
			  صحيح exc_std, صحيح use_شاخص)
{
	بنية dir_struct dir = DIR_INIT;
	صحيح i, موندنه = 0;

	إذا (!use_شاخص)
		dir.flags |= DIR_NO_GITLINKS;
	إذا (exc_std)
		تیاری_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->شاخص, pathspec);
	لكل (i = 0; i < dir.nr; i++) {
		موندنه |= grep_file(opt, dir.entries[i]->name);
		إذا (موندنه && opt->حالت_only)
			اكسر;
	}
	dir_clear(&dir);
	أرجع موندنه;
}

ساكن صحيح context_callback(ثابت بنية option *opt, ثابت حرف *arg,
			    صحيح unset)
{
	بنية grep_opt *grep_opt = opt->ارزښت;
	صحيح ارزښت;
	ثابت حرف *endp;

	إذا (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		أرجع 0;
	}
	ارزښت = strtol(arg, (حرف **)&endp, 10);
	إذا (*endp) {
		أرجع error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = ارزښت;
	أرجع 0;
}

ساكن صحيح file_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->ارزښت;
	صحيح from_stdin;
	ثابت حرف *filename = arg;
	FILE *patterns;
	صحيح lno = 0;
	بنية strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	إذا (!*filename)
		; /* leave it as-is */
	وإلا
		filename = مختاړی_filename_except_for_dash(grep_مختاړی, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	إذا (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	طالما (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		إذا (sb.len == 0)
			استمر;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_نمونه);
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
	بنية grep_opt *grep_opt = opt->ارزښت;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_نمونه(grep_opt, "--not", "command line", 0, GREP_NOT);
	أرجع 0;
}

ساكن صحيح and_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->ارزښت;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_نمونه(grep_opt, "--and", "command line", 0, GREP_AND);
	أرجع 0;
}

ساكن صحيح open_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->ارزښت;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_نمونه(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	أرجع 0;
}

ساكن صحيح close_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية grep_opt *grep_opt = opt->ارزښت;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_نمونه(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	أرجع 0;
}

ساكن صحيح نمونه_callback(ثابت بنية option *opt, ثابت حرف *arg,
			    صحيح unset)
{
	بنية grep_opt *grep_opt = opt->ارزښت;
	BUG_ON_OPT_NEG(unset);
	append_grep_نمونه(grep_opt, arg, "-e option", 0, GREP_نمونه);
	أرجع 0;
}

صحيح cmd_grep(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *مختاړی,
	     بنية repository *repo ناکارول_شوی)
{
	صحيح موندنه = 0;
	صحيح cached = 0, untracked = 0, opt_exclude = -1;
	صحيح کتل_شوی_dashdash = 0;
	صحيح external_grep_allowed__ignored;
	ثابت حرف *show_دننه_pager = عدم, *default_pager = "dummy";
	بنية grep_opt opt;
	بنية object_array list = OBJECT_ARRAY_INIT;
	بنية pathspec pathspec;
	بنية string_list لار_list = STRING_LIST_INIT_DUP;
	صحيح i;
	صحيح dummy;
	صحيح use_شاخص = 1;
	صحيح allow_revs;
	صحيح ret;

	بنية option options[] = {
		OPT_BOOL(0, "cached", &cached,
			N_("search in index instead of in the work tree")),
		OPT_NEGBIT(0, "no-index", &use_شاخص,
			 N_("find in contents not managed by git"), 1),
		OPT_BOOL(0, "untracked", &untracked,
			N_("search in both tracked and untracked files")),
		OPT_سېټ_INT(0, "exclude-standard", &opt_exclude,
			    N_("ignore files specified via '.gitignore'"), 1),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			 N_("recursively search in each submodule")),
		OPT_ډله(""),
		OPT_BOOL('v', "invert-match", &opt.invert,
			N_("show non-matching lines")),
		OPT_BOOL('i', "ignore-case", &opt.ignore_case,
			N_("case insensitive matching")),
		OPT_BOOL('w', "word-regexp", &opt.کلمه_regexp,
			N_("match patterns only at word boundaries")),
		OPT_سېټ_INT('a', "text", &opt.دوګونی,
			N_("process binary files as text"), GREP_دوګونی_متن),
		OPT_سېټ_INT('I', عدم, &opt.دوګونی,
			N_("don't match patterns in binary files"),
			GREP_دوګونی_NOMATCH),
		OPT_BOOL(0, "textconv", &opt.allow_textconv,
			 N_("process binary files with textconv filters")),
		OPT_سېټ_INT('r', "recursive", &opt.max_ژوروالی,
			    N_("search in subdirectories (default)"), -1),
		OPT_INTEGER_F(0, "max-depth", &opt.max_ژوروالی,
			N_("descend at most <n> levels"), PARSE_OPT_NONEG),
		OPT_ډله(""),
		OPT_سېټ_INT('E', "extended-regexp", &opt.نمونه_type_option,
			    N_("use extended POSIX regular expressions"),
			    GREP_نمونه_TYPE_ERE),
		OPT_سېټ_INT('G', "basic-regexp", &opt.نمونه_type_option,
			    N_("use basic POSIX regular expressions (default)"),
			    GREP_نمونه_TYPE_BRE),
		OPT_سېټ_INT('F', "fixed-strings", &opt.نمونه_type_option,
			    N_("interpret patterns as fixed strings"),
			    GREP_نمونه_TYPE_FIXED),
		OPT_سېټ_INT('P', "perl-regexp", &opt.نمونه_type_option,
			    N_("use Perl-compatible regular expressions"),
			    GREP_نمونه_TYPE_PCRE),
		OPT_ډله(""),
		OPT_BOOL('n', "line-number", &opt.linenum, N_("show line numbers")),
		OPT_BOOL(0, "column", &opt.columnnum, N_("show column number of first match")),
		OPT_NEGBIT('h', عدم, &opt.pathname, N_("don't show filenames"), 1),
		OPT_بټ('H', عدم, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.شمېر,
			N_("show the number of matches instead of matching lines")),
		OPT__COLOR(&opt.color, N_("highlight matches")),
		OPT_BOOL(0, "break", &opt.file_break,
			N_("print empty line between matches from different files")),
		OPT_BOOL(0, "heading", &opt.heading,
			N_("show filename only once above matches from same file")),
		OPT_ډله(""),
		OPT_CALLBACK('C', "context", &opt, N_("n"),
			N_("show <n> context lines before and after matches"),
			context_callback),
		OPT_UNSIGNED('B', "before-context", &opt.pre_context,
			N_("show <n> context lines before matches")),
		OPT_UNSIGNED('A', "after-context", &opt.post_context,
			N_("show <n> context lines after matches")),
		OPT_INTEGER(0, "threads", &عدد_تارونه,
			N_("use <n> worker threads")),
		OPT_شمېره_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_ډله(""),
		OPT_CALLBACK('f', عدم, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', عدم, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, نمونه_callback),
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
		OPT_BOOL(0, "all-match", &opt.all_سمون,
			N_("show only matches from files that match all patterns")),
		OPT_ډله(""),
		{
			.type = OPTION_STRING,
			.short_name = 'O',
			.long_name = "open-files-in-pager",
			.ارزښت = &show_دننه_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_شمېر,
			N_("maximum number of results per file")),
		OPT_پای_ټکی()
	};
	grep_مختاړی = مختاړی;

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
	argc = parse_options(argc, argv, مختاړی, options, grep_usage,
			     PARSE_OPT_KEEP_DASHDASH |
			     PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ډک_شاخص = 0;
	}

	إذا (use_شاخص && !startup_info->have_repository) {
		صحيح fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		إذا (fallback)
			use_شاخص = 0;
		وإلا
			/* die the same way as if we did it at the beginning */
			تیاری_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	إذا (!use_شاخص)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	إذا (argc > 0 && !opt.نمونه_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	إذا (argc > 0 && !opt.نمونه_list) {
		append_grep_نمونه(&opt, argv[0], "command line", 0,
				    GREP_نمونه);
		argv++;
		argc--;
	}

	إذا (show_دننه_pager == default_pager)
		show_دننه_pager = git_pager(the_repository, 1);
	إذا (show_دننه_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &لار_list;
		opt.output = append_لار;
		string_list_append(&لار_list, show_دننه_pager);
	}

	إذا (!opt.نمونه_list)
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
			کتل_شوی_dashdash = 1;
			اكسر;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_شاخص && !untracked;
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
			إذا (کتل_شوی_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			اكسر;
		}

		إذا (get_oid_with_context(the_repository, arg,
					 GET_OID_ریکارډ_لار,
					 &oid, &oc)) {
			إذا (کتل_شوی_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			اكسر;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		إذا (!کتل_شوی_dashdash)
			verify_non_filename(the_repository, مختاړی, arg);
		جمع_کړه_object_array_with_لار(object, arg, &list, oc.ډېرپېښ, oc.لار);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	إذا (!کتل_شوی_dashdash) {
		صحيح j;
		لكل (j = i; j < argc; j++)
			verify_filename(the_repository, مختاړی, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_ژوروالی != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       مختاړی, argv + i);
	pathspec.max_ژوروالی = opt.max_ژوروالی;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	إذا (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	إذا (opt.max_شمېر == 0) {
		ret = 1;
		اذهب بهر;
	}

	إذا (show_دننه_pager) {
		إذا (عدد_تارونه > 1)
			warning(_("invalid option combination, ignoring --threads"));
		عدد_تارونه = 1;
	} وإلا إذا (!HAVE_تارونه && عدد_تارونه > 1) {
		warning(_("no threads support, ignoring --threads"));
		عدد_تارونه = 1;
	} وإلا إذا (عدد_تارونه < 0)
		die(_("invalid number of threads specified (%d)"), عدد_تارونه);
	وإلا إذا (عدد_تارونه == 0)
		عدد_تارونه = HAVE_تارونه ? online_cpus() : 1;

	إذا (عدد_تارونه > 1) {
		إذا (!HAVE_تارونه)
			BUG("Somebody got num_threads calculation wrong!");
		إذا (!(opt.name_only || opt.unmatch_name_only || opt.شمېر)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_لومړی_کرښه = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		إذا (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		إذا (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		پیل_تارونه(&opt);
	} وإلا {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	إذا (show_دننه_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	إذا (show_دننه_pager && opt.نمونه_list && !opt.نمونه_list->بل) {
		ثابت حرف *pager = لار_list.توکي[0].string;
		صحيح len = مزي_اوږدوالی(pager);

		إذا (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		إذا (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&لار_list, "-I");

		إذا (!strcmp("less", pager) || !strcmp("vi", pager)) {
			بنية strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.نمونه_list->نمونه);
			string_list_append_nodup(&لار_list,
						 strbuf_detach(&buf, عدم));
		}
	}

	إذا (!show_دننه_pager && !opt.حالت_only)
		تیاری_pager(the_repository);

	die_for_incompatible_opt3(!use_شاخص, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	إذا (!use_شاخص || untracked) {
		صحيح use_exclude = (opt_exclude < 0) ? use_شاخص : !!opt_exclude;
		موندنه = grep_directory(&opt, &pathspec, use_exclude, use_شاخص);
	} وإلا إذا (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} وإلا إذا (!list.nr) {
		إذا (!cached)
			تیاری_work_ونه(the_repository);

		موندنه = grep_cache(&opt, &pathspec, cached);
	} وإلا {
		إذا (cached)
			die(_("both --cached and trees are given"));

		موندنه = grep_objects(&opt, &pathspec, &list);
	}

	إذا (عدد_تارونه > 1)
		موندنه |= wait_all();
	إذا (موندنه && show_دننه_pager)
		پرلپسې_pager(&opt, مختاړی);

	ret = !موندنه;

بهر:
	clear_pathspec(&pathspec);
	string_list_clear(&لار_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	أرجع ret;
}
