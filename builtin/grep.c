/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساکن ثابت حرف *grep_سابقہ;

ساکن حرف ثابت * ثابت grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	تہی
};

ساکن صحیح recurse_submodules;

ساکن صحیح عدد_دھاگے;

ساکن pthread_t *دھاگے;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
ساخت work_شے {
	ساخت grep_ماخذ ماخذ;
	حرف مکمل;
	ساخت strbuf برآمد;
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
#تعریف_کرو TODO_سائز 128
ساکن ساخت work_شے todo[TODO_سائز];
ساکن صحیح todo_آغاز;
ساکن صحیح todo_انتہا;
ساکن صحیح todo_مکمل;

/* Has all work items been added? */
ساکن صحیح all_work_added;

ساکن ساخت repository **repos_مقصد_free;
ساکن سائز_t repos_مقصد_free_nr, repos_مقصد_free_alloc;

/* This lock protects all the variables above. */
ساکن pthread_mutex_t grep_mutex;

ساکن اندرونی خالی grep_تالا(خالی)
{
	pthread_mutex_تالا(&grep_mutex);
}

ساکن اندرونی خالی grep_unlock(خالی)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
ساکن pthread_cond_t cond_جوڑو;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
ساکن pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
ساکن pthread_cond_t cond_نتیجہ;

ساکن صحیح skip_پہلا_سطر;

ساکن خالی جوڑو_work(ساخت grep_opt *opt, ساخت grep_ماخذ *gs)
{
	اگر (opt->ثنائی != GREP_ثنائی_متن)
		grep_ماخذ_بھرو_driver(gs, opt->repo->اشاریہ);

	grep_تالا();

	جب ((todo_انتہا+1) % ARRAY_سائز(todo) == todo_مکمل) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_انتہا].ماخذ = *gs;
	todo[todo_انتہا].مکمل = 0;
	strbuf_reset(&todo[todo_انتہا].برآمد);
	todo_انتہا = (todo_انتہا + 1) % ARRAY_سائز(todo);

	pthread_cond_signal(&cond_جوڑو);
	grep_unlock();
}

ساکن ساخت work_شے *get_work(خالی)
{
	ساخت work_شے *ret;

	grep_تالا();
	جب (todo_آغاز == todo_انتہا && !all_work_added) {
		pthread_cond_wait(&cond_جوڑو, &grep_mutex);
	}

	اگر (todo_آغاز == todo_انتہا && all_work_added) {
		ret = تہی;
	} ورنہ {
		ret = &todo[todo_آغاز];
		todo_آغاز = (todo_آغاز + 1) % ARRAY_سائز(todo);
	}
	grep_unlock();
	واپس ret;
}

ساکن خالی work_مکمل(ساخت work_شے *w)
{
	صحیح old_مکمل;

	grep_تالا();
	w->مکمل = 1;
	old_مکمل = todo_مکمل;
	برائے(; todo[todo_مکمل].مکمل && todo_مکمل != todo_آغاز;
	    todo_مکمل = (todo_مکمل+1) % ARRAY_سائز(todo)) {
		w = &todo[todo_مکمل];
		اگر (w->برآمد.len) {
			ثابت حرف *p = w->برآمد.buf;
			سائز_t len = w->برآمد.len;

			/* Skip the leading hunk mark of the first file. */
			اگر (skip_پہلا_سطر) {
				جب (len) {
					len--;
					اگر (*p++ == '\n')
						توڑو;
				}
				skip_پہلا_سطر = 0;
			}

			write_or_die(1, p, len);
		}
		grep_ماخذ_clear(&w->ماخذ);
	}

	اگر (old_مکمل != todo_مکمل)
		pthread_cond_signal(&cond_write);

	اگر (all_work_added && todo_مکمل == todo_انتہا)
		pthread_cond_signal(&cond_نتیجہ);

	grep_unlock();
}

ساکن خالی free_repos(خالی)
{
	صحیح i;

	برائے (i = 0; i < repos_مقصد_free_nr; i++) {
		repo_clear(repos_مقصد_free[i]);
		آزاد(repos_مقصد_free[i]);
	}
	FREE_AND_NULL(repos_مقصد_free);
	repos_مقصد_free_nr = 0;
	repos_مقصد_free_alloc = 0;
}

ساکن خالی *تسلسل(خالی *arg)
{
	صحیح کامیابی = 0;
	ساخت grep_opt *opt = arg;

	جب (1) {
		ساخت work_شے *w = get_work();
		اگر (!w)
			توڑو;

		opt->output_priv = w;
		کامیابی |= grep_ماخذ(opt, &w->ماخذ);
		grep_ماخذ_clear_ڈیٹا(&w->ماخذ);
		work_مکمل(w);
	}
	free_grep_patterns(opt);
	آزاد(opt);

	واپس (خالی*) (intptr_t) کامیابی;
}

ساکن خالی strbuf_برآمد(ساخت grep_opt *opt, ثابت خالی *buf, سائز_t سائز)
{
	ساخت work_شے *w = opt->output_priv;
	strbuf_جوڑو(&w->برآمد, buf, سائز);
}

ساکن خالی آغاز_دھاگے(ساخت grep_opt *opt)
{
	صحیح i;

	pthread_mutex_init(&grep_mutex, تہی);
	pthread_mutex_init(&grep_attr_mutex, تہی);
	pthread_cond_init(&cond_جوڑو, تہی);
	pthread_cond_init(&cond_write, تہی);
	pthread_cond_init(&cond_نتیجہ, تہی);
	grep_use_locks = 1;
	enable_obj_read_تالا();

	برائے (i = 0; i < ARRAY_سائز(todo); i++) {
		strbuf_init(&todo[i].برآمد, 0);
	}

	CALLOC_ARRAY(دھاگے, عدد_دھاگے);
	برائے (i = 0; i < عدد_دھاگے; i++) {
		صحیح err;
		ساخت grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_برآمد;
		compile_grep_patterns(o);
		err = pthread_create(&دھاگے[i], تہی, تسلسل, o);

		اگر (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

ساکن صحیح wait_all(خالی)
{
	صحیح کامیابی = 0;
	صحیح i;

	اگر (!HAVE_دھاگے)
		BUG("Never call this function unless you have started threads");

	grep_تالا();
	all_work_added = 1;

	/* Wait until all work is done. */
	جب (todo_مکمل != todo_انتہا)
		pthread_cond_wait(&cond_نتیجہ, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_نشر(&cond_جوڑو);
	grep_unlock();

	برائے (i = 0; i < عدد_دھاگے; i++) {
		خالی *h;
		pthread_join(دھاگے[i], &h);
		کامیابی |= (صحیح) (intptr_t) h;
	}

	آزاد(دھاگے);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_جوڑو);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_نتیجہ);
	grep_use_locks = 0;
	disable_obj_read_تالا();

	واپس کامیابی;
}

ساکن صحیح grep_cmd_config(ثابت حرف *var, ثابت حرف *قدر,
			   ثابت ساخت config_context *ctx, خالی *cb)
{
	صحیح st = grep_config(var, قدر, ctx, cb);

	اگر (git_color_config(var, قدر, cb) < 0)
		st = -1;
	ورنہ اگر (git_default_config(var, قدر, ctx, cb) < 0)
		st = -1;

	اگر (!strcmp(var, "grep.threads")) {
		عدد_دھاگے = git_config_int(var, قدر, ctx->kvi);
		اگر (عدد_دھاگے < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    عدد_دھاگے, var);
		ورنہ اگر (!HAVE_دھاگے && عدد_دھاگے > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			عدد_دھاگے = 1;
		}
	}

	اگر (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, قدر);

	واپس st;
}

ساکن خالی grep_ماخذ_name(ساخت grep_opt *opt, ثابت حرف *filename,
			     صحیح درخت_name_len, ساخت strbuf *برآمد)
{
	strbuf_reset(برآمد);

	اگر (opt->null_following_name) {
		اگر (opt->relative && grep_سابقہ) {
			ساخت strbuf rel_buf = STRBUF_INIT;
			ثابت حرف *rel_name =
				relative_راستہ(filename + درخت_name_len,
					      grep_سابقہ, &rel_buf);

			اگر (درخت_name_len)
				strbuf_جوڑو(برآمد, filename, درخت_name_len);

			strbuf_addstr(برآمد, rel_name);
			strbuf_release(&rel_buf);
		} ورنہ {
			strbuf_addstr(برآمد, filename);
		}
		واپس;
	}

	اگر (opt->relative && grep_سابقہ)
		quote_راستہ(filename + درخت_name_len, grep_سابقہ, برآمد, 0);
	ورنہ
		quote_c_style(filename + درخت_name_len, برآمد, تہی, 0);

	اگر (درخت_name_len)
		strbuf_داخل_کرو(برآمد, 0, filename, درخت_name_len);
}

ساکن صحیح grep_oid(ساخت grep_opt *opt, ثابت ساخت object_id *oid,
		     ثابت حرف *filename, صحیح درخت_name_len,
		     ثابت حرف *راستہ)
{
	ساخت strbuf pathbuf = STRBUF_INIT;
	ساخت grep_ماخذ gs;

	grep_ماخذ_name(opt, filename, درخت_name_len, &pathbuf);
	grep_ماخذ_init_oid(&gs, pathbuf.buf, راستہ, oid, opt->repo);
	strbuf_release(&pathbuf);

	اگر (عدد_دھاگے > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		جوڑو_work(opt, &gs);
		واپس 0;
	} ورنہ {
		صحیح کامیابی;

		کامیابی = grep_ماخذ(opt, &gs);

		grep_ماخذ_clear(&gs);
		واپس کامیابی;
	}
}

ساکن صحیح grep_file(ساخت grep_opt *opt, ثابت حرف *filename)
{
	ساخت strbuf buf = STRBUF_INIT;
	ساخت grep_ماخذ gs;

	grep_ماخذ_name(opt, filename, 0, &buf);
	grep_ماخذ_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	اگر (عدد_دھاگے > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		جوڑو_work(opt, &gs);
		واپس 0;
	} ورنہ {
		صحیح کامیابی;

		کامیابی = grep_ماخذ(opt, &gs);

		grep_ماخذ_clear(&gs);
		واپس کامیابی;
	}
}

ساکن خالی append_راستہ(ساخت grep_opt *opt, ثابت خالی *ڈیٹا, سائز_t len)
{
	ساخت string_list *راستہ_list = opt->output_priv;

	اگر (len == 1 && *(ثابت حرف *)ڈیٹا == '\0')
		واپس;
	string_list_append_nodup(راستہ_list, xstrndup(ڈیٹا, len));
}

ساکن خالی تسلسل_pager(ساخت grep_opt *opt, ثابت حرف *سابقہ)
{
	ساخت string_list *راستہ_list = opt->output_priv;
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;
	صحیح i, حالت_2;

	برائے (i = 0; i < راستہ_list->nr; i++)
		strvec_ڈالو(&بچہ.دلائل, راستہ_list->اشیاء[i].string);
	بچہ.dir = سابقہ;
	بچہ.use_shell = 1;

	حالت_2 = تسلسل_command(&بچہ);
	اگر (حالت_2)
		باہر_نکلو(حالت_2);
}

ساکن صحیح grep_cache(ساخت grep_opt *opt,
		      ثابت ساخت pathspec *pathspec, صحیح cached);
ساکن صحیح grep_درخت(ساخت grep_opt *opt, ثابت ساخت pathspec *pathspec,
		     ساخت درخت_desc *درخت, ساخت strbuf *بنیاد, صحیح tn_len,
		     صحیح check_attr);

ساکن صحیح grep_submodule(ساخت grep_opt *opt,
			  ثابت ساخت pathspec *pathspec,
			  ثابت ساخت object_id *oid,
			  ثابت حرف *filename, ثابت حرف *راستہ, صحیح cached)
{
	ساخت repository *subrepo;
	ساخت repository *superproject = opt->repo;
	ساخت grep_opt subopt;
	صحیح کامیابی = 0;

	اگر (!is_submodule_فعال(superproject, راستہ))
		واپس 0;

	subrepo = xmalloc(حجم(*subrepo));
	اگر (repo_submodule_init(subrepo, superproject, راستہ, null_oid(opt->repo->hash_algo))) {
		آزاد(subrepo);
		واپس 0;
	}
	ALLOC_GROW(repos_مقصد_free, repos_مقصد_free_nr + 1, repos_مقصد_free_alloc);
	repos_مقصد_free[repos_مقصد_free_nr++] = subrepo;

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
	obj_read_تالا();
	repo_read_gitmodules(subrepo, 0);
	obj_read_unlock();

	حافظہ_نقل(&subopt, opt, حجم(subopt));
	subopt.repo = subrepo;

	اگر (oid) {
		شمار object_type object_type;
		ساخت درخت_desc درخت;
		خالی *ڈیٹا;
		سائز_t سائز;
		ساخت strbuf بنیاد = STRBUF_INIT;

		obj_read_تالا();
		object_type = odb_read_object_info(subrepo->objects, oid, تہی);
		obj_read_unlock();
		ڈیٹا = odb_read_object_peeled(subrepo->objects, oid, OBJ_درخت, &سائز, تہی);
		اگر (!ڈیٹا)
			die(_("unable to read tree (%s)"), oid_مقصد_hex(oid));

		strbuf_addstr(&بنیاد, filename);
		strbuf_addch(&بنیاد, '/');

		init_درخت_desc(&درخت, oid, ڈیٹا, سائز);
		کامیابی = grep_درخت(&subopt, pathspec, &درخت, &بنیاد, بنیاد.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&بنیاد);
		آزاد(ڈیٹا);
	} ورنہ {
		کامیابی = grep_cache(&subopt, pathspec, cached);
	}

	واپس کامیابی;
}

ساکن صحیح grep_cache(ساخت grep_opt *opt,
		      ثابت ساخت pathspec *pathspec, صحیح cached)
{
	ساخت repository *repo = opt->repo;
	صحیح کامیابی = 0;
	صحیح nr;
	ساخت strbuf name = STRBUF_INIT;
	صحیح name_بنیاد_len = 0;
	اگر (repo->submodule_سابقہ) {
		name_بنیاد_len = لڑی_لمبائی(repo->submodule_سابقہ);
		strbuf_addstr(&name, repo->submodule_سابقہ);
	}

	اگر (repo_read_اشاریہ(repo) < 0)
		die(_("index file corrupt"));

	برائے (nr = 0; nr < repo->اشاریہ->cache_nr; nr++) {
		ثابت ساخت cache_اندراج *ce = repo->اشاریہ->cache[nr];

		اگر (!cached && ce_skip_worktree(ce))
			جاری;

		strbuf_setlen(&name, name_بنیاد_len);
		strbuf_addstr(&name, ce->name);
		اگر (S_ISSPARSEDIR(ce->ce_منوال)) {
			شمار object_type type;
			ساخت درخت_desc درخت;
			خالی *ڈیٹا;
			سائز_t سائز;

			ڈیٹا = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &سائز);
			اگر (!ڈیٹا)
				die(_("unable to read tree %s"), oid_مقصد_hex(&ce->oid));
			init_درخت_desc(&درخت, &ce->oid, ڈیٹا, سائز);

			کامیابی |= grep_درخت(opt, pathspec, &درخت, &name, 0, 0);
			strbuf_setlen(&name, name_بنیاد_len);
			strbuf_addstr(&name, ce->name);
			آزاد(ڈیٹا);
		} ورنہ اگر (S_ISREG(ce->ce_منوال) &&
		    مطابقت_pathspec(repo->اشاریہ, pathspec, name.buf, name.len, 0, تہی,
				   S_ISDIR(ce->ce_منوال) ||
				   S_ISGITLINK(ce->ce_منوال))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			اگر (cached || (ce->ce_flags & CE_VALID)) {
				اگر (ce_stage(ce) || ce_intent_مقصد_جوڑو(ce))
					جاری;
				کامیابی |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} ورنہ {
				کامیابی |= grep_file(opt, name.buf);
			}
		} ورنہ اگر (recurse_submodules && S_ISGITLINK(ce->ce_منوال) &&
			   submodule_راستہ_مطابقت(repo->اشاریہ, pathspec, name.buf, تہی)) {
			کامیابی |= grep_submodule(opt, pathspec, تہی, ce->name,
					      ce->name, cached);
		} ورنہ {
			جاری;
		}

		اگر (ce_stage(ce)) {
			کرو {
				nr++;
			} جب (nr < repo->اشاریہ->cache_nr &&
				 !strcmp(ce->name, repo->اشاریہ->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		اگر (کامیابی && opt->حالت_only)
			توڑو;
	}

	strbuf_release(&name);
	واپس کامیابی;
}

ساکن صحیح grep_درخت(ساخت grep_opt *opt, ثابت ساخت pathspec *pathspec,
		     ساخت درخت_desc *درخت, ساخت strbuf *بنیاد, صحیح tn_len,
		     صحیح check_attr)
{
	ساخت repository *repo = opt->repo;
	صحیح کامیابی = 0;
	شمار interesting مطابقت = اندراج_not_interesting;
	ساخت name_اندراج اندراج_2;
	صحیح old_baselen = بنیاد->len;
	ساخت strbuf name = STRBUF_INIT;
	صحیح name_بنیاد_len = 0;
	اگر (repo->submodule_سابقہ) {
		strbuf_addstr(&name, repo->submodule_سابقہ);
		name_بنیاد_len = name.len;
	}

	جب (درخت_اندراج(درخت, &اندراج_2)) {
		صحیح te_len = درخت_اندراج_len(&اندراج_2);

		اگر (مطابقت != all_entries_interesting) {
			strbuf_addstr(&name, بنیاد->buf + tn_len);
			مطابقت = درخت_اندراج_interesting(repo->اشاریہ,
						       &اندراج_2, &name,
						       pathspec);
			strbuf_setlen(&name, name_بنیاد_len);

			اگر (مطابقت == all_entries_not_interesting)
				توڑو;
			اگر (مطابقت == اندراج_not_interesting)
				جاری;
		}

		strbuf_جوڑو(بنیاد, اندراج_2.راستہ, te_len);

		اگر (S_ISREG(اندراج_2.منوال)) {
			کامیابی |= grep_oid(opt, &اندراج_2.oid, بنیاد->buf, tn_len,
					 check_attr ? بنیاد->buf + tn_len : تہی);
		} ورنہ اگر (S_ISDIR(اندراج_2.منوال)) {
			شمار object_type type;
			ساخت درخت_desc sub;
			خالی *ڈیٹا;
			سائز_t سائز;

			ڈیٹا = odb_read_object(the_repository->objects,
					       &اندراج_2.oid, &type, &سائز);
			اگر (!ڈیٹا)
				die(_("unable to read tree (%s)"),
				    oid_مقصد_hex(&اندراج_2.oid));

			strbuf_addch(بنیاد, '/');
			init_درخت_desc(&sub, &اندراج_2.oid, ڈیٹا, سائز);
			کامیابی |= grep_درخت(opt, pathspec, &sub, بنیاد, tn_len,
					 check_attr);
			آزاد(ڈیٹا);
		} ورنہ اگر (recurse_submodules && S_ISGITLINK(اندراج_2.منوال)) {
			کامیابی |= grep_submodule(opt, pathspec, &اندراج_2.oid,
					      بنیاد->buf, بنیاد->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(بنیاد, old_baselen);

		اگر (کامیابی && opt->حالت_only)
			توڑو;
	}

	strbuf_release(&name);
	واپس کامیابی;
}

ساکن خالی collect_blob_oids_for_درخت(ساخت repository *repo,
				       ثابت ساخت pathspec *pathspec,
				       ساخت درخت_desc *درخت,
				       ساخت strbuf *بنیاد,
				       صحیح tn_len,
				       ساخت oidset *blob_oids)
{
	ساخت name_اندراج اندراج_2;
	صحیح old_baselen = بنیاد->len;
	ساخت strbuf name = STRBUF_INIT;
	شمار interesting مطابقت = اندراج_not_interesting;

	جب (درخت_اندراج(درخت, &اندراج_2)) {
		اگر (مطابقت != all_entries_interesting) {
			strbuf_addstr(&name, بنیاد->buf + tn_len);
			مطابقت = درخت_اندراج_interesting(repo->اشاریہ,
						       &اندراج_2, &name,
						       pathspec);
			strbuf_reset(&name);

			اگر (مطابقت == all_entries_not_interesting)
				توڑو;
			اگر (مطابقت == اندراج_not_interesting)
				جاری;
		}

		strbuf_جوڑو(بنیاد, اندراج_2.راستہ, درخت_اندراج_len(&اندراج_2));

		اگر (S_ISREG(اندراج_2.منوال)) {
			اگر (!odb_has_object(repo->objects, &اندراج_2.oid, 0))
				oidset_داخل_کرو(blob_oids, &اندراج_2.oid);
		} ورنہ اگر (S_ISDIR(اندراج_2.منوال)) {
			شمار object_type type;
			ساخت درخت_desc sub_درخت;
			خالی *ڈیٹا;
			سائز_t سائز;

			ڈیٹا = odb_read_object(repo->objects, &اندراج_2.oid,
					       &type, &سائز);
			اگر (!ڈیٹا)
				die(_("unable to read tree (%s)"),
				    oid_مقصد_hex(&اندراج_2.oid));

			strbuf_addch(بنیاد, '/');
			init_درخت_desc(&sub_درخت, &اندراج_2.oid, ڈیٹا, سائز);
			collect_blob_oids_for_درخت(repo, pathspec, &sub_درخت,
						   بنیاد, tn_len, blob_oids);
			آزاد(ڈیٹا);
		}
		/*
		 * ...no else clause for S_ISGITLINK: submodules have their
		 * own promisor configuration and would need separate fetches
		 * anyway.
		 */

		strbuf_setlen(بنیاد, old_baselen);
	}

	strbuf_release(&name);
}

ساکن خالی collect_blob_oids_for_treeish(ساخت grep_opt *opt,
					  ثابت ساخت pathspec *pathspec,
					  ثابت ساخت object_id *درخت_ish_oid,
					  ثابت حرف *name,
					  ساخت oidset *blob_oids)
{
	ساخت درخت_desc درخت;
	خالی *ڈیٹا;
	سائز_t سائز;
	ساخت strbuf بنیاد = STRBUF_INIT;
	صحیح len;

	ڈیٹا = odb_read_object_peeled(opt->repo->objects, درخت_ish_oid,
				      OBJ_درخت, &سائز, تہی);

	اگر (!ڈیٹا)
		واپس;

	len = name ? لڑی_لمبائی(name) : 0;
	اگر (len) {
		strbuf_جوڑو(&بنیاد, name, len);
		strbuf_addch(&بنیاد, ':');
	}
	init_درخت_desc(&درخت, درخت_ish_oid, ڈیٹا, سائز);

	collect_blob_oids_for_درخت(opt->repo, pathspec, &درخت,
				   &بنیاد, بنیاد.len, blob_oids);

	strbuf_release(&بنیاد);
	آزاد(ڈیٹا);
}

ساکن خالی prefetch_grep_blobs(ساخت grep_opt *opt,
				ثابت ساخت pathspec *pathspec,
				ثابت ساخت object_array *list)
{
	ساخت oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	اگر (!repo_has_promisor_remote(opt->repo))
		واپس;

	/* For each tree, gather the blobs in it */
	برائے (صحیح i = 0; i < list->nr; i++) {
		ساخت object *real_obj;

		obj_read_تالا();
		real_obj = deref_لیبل(opt->repo, list->objects[i].شے,
				     تہی, 0);
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
	اگر (oidset_سائز(&blob_oids)) {
		ساخت oid_array مقصد_fetch = OID_ARRAY_INIT;
		ساخت oidset_iter iter;
		ثابت ساخت object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		جب ((oid = oidset_iter_اگلا(&iter)))
			oid_array_append(&مقصد_fetch, oid);

		promisor_remote_get_براہ_راست(opt->repo, مقصد_fetch.oid, مقصد_fetch.nr);

		oid_array_clear(&مقصد_fetch);
	}
	oidset_clear(&blob_oids);
}

ساکن صحیح grep_object(ساخت grep_opt *opt, ثابت ساخت pathspec *pathspec,
		       ساخت object *obj, ثابت حرف *name, ثابت حرف *راستہ)
{
	اگر (obj->type == OBJ_BLOB)
		واپس grep_oid(opt, &obj->oid, name, 0, راستہ);
	اگر (obj->type == OBJ_COMMIT || obj->type == OBJ_درخت) {
		ساخت درخت_desc درخت;
		خالی *ڈیٹا;
		سائز_t سائز;
		ساخت strbuf بنیاد;
		صحیح کامیابی, len;

		ڈیٹا = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_درخت, &سائز, تہی);
		اگر (!ڈیٹا)
			die(_("unable to read tree (%s)"), oid_مقصد_hex(&obj->oid));

		len = name ? لڑی_لمبائی(name) : 0;
		strbuf_init(&بنیاد, راستہ_MAX + len + 1);
		اگر (len) {
			strbuf_جوڑو(&بنیاد, name, len);
			strbuf_addch(&بنیاد, ':');
		}
		init_درخت_desc(&درخت, &obj->oid, ڈیٹا, سائز);
		کامیابی = grep_درخت(opt, pathspec, &درخت, &بنیاد, بنیاد.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&بنیاد);
		آزاد(ڈیٹا);
		واپس کامیابی;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

ساکن صحیح grep_objects(ساخت grep_opt *opt, ثابت ساخت pathspec *pathspec,
			ثابت ساخت object_array *list)
{
	غیرعلامتی صحیح i;
	صحیح کامیابی = 0;
	ثابت غیرعلامتی صحیح nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	برائے (i = 0; i < nr; i++) {
		ساخت object *real_obj;

		obj_read_تالا();
		real_obj = deref_لیبل(opt->repo, list->objects[i].شے,
				     تہی, 0);
		obj_read_unlock();

		اگر (!real_obj) {
			حرف hex[GIT_MAX_HEXSZ + 1];
			ثابت حرف *name = list->objects[i].name;

			اگر (!name) {
				oid_مقصد_hex_r(hex, &list->objects[i].شے->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		اگر (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_تالا();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		اگر (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].راستہ)) {
			کامیابی = 1;
			اگر (opt->حالت_only)
				توڑو;
		}
	}
	واپس کامیابی;
}

ساکن صحیح grep_directory(ساخت grep_opt *opt, ثابت ساخت pathspec *pathspec,
			  صحیح exc_std, صحیح use_اشاریہ)
{
	ساخت dir_struct dir = DIR_INIT;
	صحیح i, کامیابی = 0;

	اگر (!use_اشاریہ)
		dir.flags |= DIR_NO_GITLINKS;
	اگر (exc_std)
		تیاری_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->اشاریہ, pathspec);
	برائے (i = 0; i < dir.nr; i++) {
		کامیابی |= grep_file(opt, dir.entries[i]->name);
		اگر (کامیابی && opt->حالت_only)
			توڑو;
	}
	dir_clear(&dir);
	واپس کامیابی;
}

ساکن صحیح context_callback(ثابت ساخت option *opt, ثابت حرف *arg,
			    صحیح unset)
{
	ساخت grep_opt *grep_opt = opt->قدر;
	صحیح قدر;
	ثابت حرف *endp;

	اگر (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		واپس 0;
	}
	قدر = strtol(arg, (حرف **)&endp, 10);
	اگر (*endp) {
		واپس error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = قدر;
	واپس 0;
}

ساکن صحیح file_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت grep_opt *grep_opt = opt->قدر;
	صحیح from_stdin;
	ثابت حرف *filename = arg;
	FILE *patterns;
	صحیح lno = 0;
	ساخت strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	اگر (!*filename)
		; /* leave it as-is */
	ورنہ
		filename = سابقہ_filename_except_for_dash(grep_سابقہ, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	اگر (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	جب (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		اگر (sb.len == 0)
			جاری;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_نمونہ);
	}
	اگر (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	اگر (filename != arg)
		آزاد((خالی *)filename);
	واپس 0;
}

ساکن صحیح not_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت grep_opt *grep_opt = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_نمونہ(grep_opt, "--not", "command line", 0, GREP_NOT);
	واپس 0;
}

ساکن صحیح and_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت grep_opt *grep_opt = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_نمونہ(grep_opt, "--and", "command line", 0, GREP_AND);
	واپس 0;
}

ساکن صحیح open_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت grep_opt *grep_opt = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_نمونہ(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	واپس 0;
}

ساکن صحیح close_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت grep_opt *grep_opt = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_نمونہ(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	واپس 0;
}

ساکن صحیح نمونہ_callback(ثابت ساخت option *opt, ثابت حرف *arg,
			    صحیح unset)
{
	ساخت grep_opt *grep_opt = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	append_grep_نمونہ(grep_opt, arg, "-e option", 0, GREP_نمونہ);
	واپس 0;
}

صحیح cmd_grep(صحیح argc,
	     ثابت حرف **argv,
	     ثابت حرف *سابقہ,
	     ساخت repository *repo غیر_مستعمل)
{
	صحیح کامیابی = 0;
	صحیح cached = 0, untracked = 0, opt_exclude = -1;
	صحیح نظر_آیا_dashdash = 0;
	صحیح external_grep_allowed__ignored;
	ثابت حرف *show_اندر_pager = تہی, *default_pager = "dummy";
	ساخت grep_opt opt;
	ساخت object_array list = OBJECT_ARRAY_INIT;
	ساخت pathspec pathspec;
	ساخت string_list راستہ_list = STRING_LIST_INIT_DUP;
	صحیح i;
	صحیح dummy;
	صحیح use_اشاریہ = 1;
	صحیح allow_revs;
	صحیح ret;

	ساخت option options[] = {
		OPT_BOOL(0, "cached", &cached,
			N_("search in index instead of in the work tree")),
		OPT_NEGBIT(0, "no-index", &use_اشاریہ,
			 N_("find in contents not managed by git"), 1),
		OPT_BOOL(0, "untracked", &untracked,
			N_("search in both tracked and untracked files")),
		OPT_سیٹ_INT(0, "exclude-standard", &opt_exclude,
			    N_("ignore files specified via '.gitignore'"), 1),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			 N_("recursively search in each submodule")),
		OPT_گروہ(""),
		OPT_BOOL('v', "invert-match", &opt.invert,
			N_("show non-matching lines")),
		OPT_BOOL('i', "ignore-case", &opt.ignore_case,
			N_("case insensitive matching")),
		OPT_BOOL('w', "word-regexp", &opt.لفظ_regexp,
			N_("match patterns only at word boundaries")),
		OPT_سیٹ_INT('a', "text", &opt.ثنائی,
			N_("process binary files as text"), GREP_ثنائی_متن),
		OPT_سیٹ_INT('I', تہی, &opt.ثنائی,
			N_("don't match patterns in binary files"),
			GREP_ثنائی_NOMATCH),
		OPT_BOOL(0, "textconv", &opt.allow_textconv,
			 N_("process binary files with textconv filters")),
		OPT_سیٹ_INT('r', "recursive", &opt.max_گہرائی,
			    N_("search in subdirectories (default)"), -1),
		OPT_INTEGER_F(0, "max-depth", &opt.max_گہرائی,
			N_("descend at most <n> levels"), PARSE_OPT_NONEG),
		OPT_گروہ(""),
		OPT_سیٹ_INT('E', "extended-regexp", &opt.نمونہ_type_option,
			    N_("use extended POSIX regular expressions"),
			    GREP_نمونہ_TYPE_ERE),
		OPT_سیٹ_INT('G', "basic-regexp", &opt.نمونہ_type_option,
			    N_("use basic POSIX regular expressions (default)"),
			    GREP_نمونہ_TYPE_BRE),
		OPT_سیٹ_INT('F', "fixed-strings", &opt.نمونہ_type_option,
			    N_("interpret patterns as fixed strings"),
			    GREP_نمونہ_TYPE_FIXED),
		OPT_سیٹ_INT('P', "perl-regexp", &opt.نمونہ_type_option,
			    N_("use Perl-compatible regular expressions"),
			    GREP_نمونہ_TYPE_PCRE),
		OPT_گروہ(""),
		OPT_BOOL('n', "line-number", &opt.linenum, N_("show line numbers")),
		OPT_BOOL(0, "column", &opt.columnnum, N_("show column number of first match")),
		OPT_NEGBIT('h', تہی, &opt.pathname, N_("don't show filenames"), 1),
		OPT_ثنائی_ہندسہ('H', تہی, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.گنتی,
			N_("show the number of matches instead of matching lines")),
		OPT__COLOR(&opt.color, N_("highlight matches")),
		OPT_BOOL(0, "break", &opt.file_break,
			N_("print empty line between matches from different files")),
		OPT_BOOL(0, "heading", &opt.heading,
			N_("show filename only once above matches from same file")),
		OPT_گروہ(""),
		OPT_CALLBACK('C', "context", &opt, N_("n"),
			N_("show <n> context lines before and after matches"),
			context_callback),
		OPT_UNSIGNED('B', "before-context", &opt.pre_context,
			N_("show <n> context lines before matches")),
		OPT_UNSIGNED('A', "after-context", &opt.post_context,
			N_("show <n> context lines after matches")),
		OPT_INTEGER(0, "threads", &عدد_دھاگے,
			N_("use <n> worker threads")),
		OPT_نمبر_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_گروہ(""),
		OPT_CALLBACK('f', تہی, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', تہی, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, نمونہ_callback),
		OPT_CALLBACK_F(0, "and", &opt, تہی,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, تہی, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', تہی, &opt, تہی, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', تہی, &opt, تہی, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			close_callback),
		OPT__QUIET(&opt.حالت_only,
			   N_("indicate hit with exit status without output")),
		OPT_BOOL(0, "all-match", &opt.all_مطابقت,
			N_("show only matches from files that match all patterns")),
		OPT_گروہ(""),
		{
			.type = OPTION_STRING,
			.short_name = 'O',
			.long_name = "open-files-in-pager",
			.قدر = &show_اندر_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_گنتی,
			N_("maximum number of results per file")),
		OPT_انتہا()
	};
	grep_سابقہ = سابقہ;

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
	argc = parse_options(argc, argv, سابقہ, options, grep_usage,
			     PARSE_OPT_KEEP_DASHDASH |
			     PARSE_OPT_STOP_AT_NON_OPTION);

	اگر (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_بھرا_اشاریہ = 0;
	}

	اگر (use_اشاریہ && !startup_info->have_repository) {
		صحیح fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		اگر (fallback)
			use_اشاریہ = 0;
		ورنہ
			/* die the same way as if we did it at the beginning */
			تیاری_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	اگر (!use_اشاریہ)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	اگر (argc > 0 && !opt.نمونہ_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	اگر (argc > 0 && !opt.نمونہ_list) {
		append_grep_نمونہ(&opt, argv[0], "command line", 0,
				    GREP_نمونہ);
		argv++;
		argc--;
	}

	اگر (show_اندر_pager == default_pager)
		show_اندر_pager = git_pager(the_repository, 1);
	اگر (show_اندر_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &راستہ_list;
		opt.output = append_راستہ;
		string_list_append(&راستہ_list, show_اندر_pager);
	}

	اگر (!opt.نمونہ_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	اگر (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	برائے (i = 0; i < argc; i++) {
		اگر (!strcmp(argv[i], "--")) {
			نظر_آیا_dashdash = 1;
			توڑو;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_اشاریہ && !untracked;
	برائے (i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		ساخت object_id oid;
		ساخت object_context oc = {0};
		ساخت object *object;

		اگر (!strcmp(arg, "--")) {
			i++;
			توڑو;
		}

		اگر (!allow_revs) {
			اگر (نظر_آیا_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			توڑو;
		}

		اگر (get_oid_with_context(the_repository, arg,
					 GET_OID_ریکارڈ_راستہ,
					 &oid, &oc)) {
			اگر (نظر_آیا_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			توڑو;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		اگر (!نظر_آیا_dashdash)
			verify_non_filename(the_repository, سابقہ, arg);
		جوڑو_object_array_with_راستہ(object, arg, &list, oc.منوال, oc.راستہ);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	اگر (!نظر_آیا_dashdash) {
		صحیح j;
		برائے (j = i; j < argc; j++)
			verify_filename(the_repository, سابقہ, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_گہرائی != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       سابقہ, argv + i);
	pathspec.max_گہرائی = opt.max_گہرائی;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	اگر (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	اگر (opt.max_گنتی == 0) {
		ret = 1;
		جاؤ برآمد;
	}

	اگر (show_اندر_pager) {
		اگر (عدد_دھاگے > 1)
			warning(_("invalid option combination, ignoring --threads"));
		عدد_دھاگے = 1;
	} ورنہ اگر (!HAVE_دھاگے && عدد_دھاگے > 1) {
		warning(_("no threads support, ignoring --threads"));
		عدد_دھاگے = 1;
	} ورنہ اگر (عدد_دھاگے < 0)
		die(_("invalid number of threads specified (%d)"), عدد_دھاگے);
	ورنہ اگر (عدد_دھاگے == 0)
		عدد_دھاگے = HAVE_دھاگے ? online_cpus() : 1;

	اگر (عدد_دھاگے > 1) {
		اگر (!HAVE_دھاگے)
			BUG("Somebody got num_threads calculation wrong!");
		اگر (!(opt.name_only || opt.unmatch_name_only || opt.گنتی)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_پہلا_سطر = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		اگر (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		اگر (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		آغاز_دھاگے(&opt);
	} ورنہ {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	اگر (show_اندر_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	اگر (show_اندر_pager && opt.نمونہ_list && !opt.نمونہ_list->اگلا) {
		ثابت حرف *pager = راستہ_list.اشیاء[0].string;
		صحیح len = لڑی_لمبائی(pager);

		اگر (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		اگر (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&راستہ_list, "-I");

		اگر (!strcmp("less", pager) || !strcmp("vi", pager)) {
			ساخت strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.نمونہ_list->نمونہ);
			string_list_append_nodup(&راستہ_list,
						 strbuf_detach(&buf, تہی));
		}
	}

	اگر (!show_اندر_pager && !opt.حالت_only)
		تیاری_pager(the_repository);

	die_for_incompatible_opt3(!use_اشاریہ, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	اگر (!use_اشاریہ || untracked) {
		صحیح use_exclude = (opt_exclude < 0) ? use_اشاریہ : !!opt_exclude;
		کامیابی = grep_directory(&opt, &pathspec, use_exclude, use_اشاریہ);
	} ورنہ اگر (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} ورنہ اگر (!list.nr) {
		اگر (!cached)
			تیاری_work_درخت(the_repository);

		کامیابی = grep_cache(&opt, &pathspec, cached);
	} ورنہ {
		اگر (cached)
			die(_("both --cached and trees are given"));

		کامیابی = grep_objects(&opt, &pathspec, &list);
	}

	اگر (عدد_دھاگے > 1)
		کامیابی |= wait_all();
	اگر (کامیابی && show_اندر_pager)
		تسلسل_pager(&opt, سابقہ);

	ret = !کامیابی;

برآمد:
	clear_pathspec(&pathspec);
	string_list_clear(&راستہ_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	واپس ret;
}
