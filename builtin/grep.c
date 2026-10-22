/*
 * Builtin "git grep"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "tree-walk.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "grep.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "pathspec.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "submodule-config.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/source.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "oidset.h"
#అన్తర్భావ "pager.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "promisor-remote.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "write-or-die.h"

స్థిర స్థిరరాశి అక్షరం *grep_ఉపసర్గ;

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి grep_usage[] = {
	N_("git grep [<options>] [-e] <pattern> [<rev>...] [[--] <path>...]"),
	ఖాళీ
};

స్థిర పూర్ణాంకం recurse_submodules;

స్థిర పూర్ణాంకం అంకె_దారాలు;

స్థిర pthread_t *దారాలు;

/* We use one producer thread and THREADS consumer
 * threads. The producer adds struct work_items to 'todo' and the
 * consumers pick work items from the same array.
 */
నిర్మాణం work_అంశం {
	నిర్మాణం grep_ఆధారం ఆధారం;
	అక్షరం పూర్తి;
	నిర్మాణం strbuf బహిర్గతం;
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
#నిర్వచించు TODO_కొలత 128
స్థిర నిర్మాణం work_అంశం todo[TODO_కొలత];
స్థిర పూర్ణాంకం todo_ప్రారంభం;
స్థిర పూర్ణాంకం todo_చివర;
స్థిర పూర్ణాంకం todo_పూర్తి;

/* Has all work items been added? */
స్థిర పూర్ణాంకం all_work_added;

స్థిర నిర్మాణం repository **repos_గమ్యం_free;
స్థిర కొలత_t repos_గమ్యం_free_nr, repos_గమ్యం_free_alloc;

/* This lock protects all the variables above. */
స్థిర pthread_mutex_t grep_mutex;

స్థిర అంతఃవరుస శూన్య grep_తాళం(శూన్య)
{
	pthread_mutex_తాళం(&grep_mutex);
}

స్థిర అంతఃవరుస శూన్య grep_unlock(శూన్య)
{
	pthread_mutex_unlock(&grep_mutex);
}

/* Signalled when a new work_item is added to todo. */
స్థిర pthread_cond_t cond_కూడు;

/* Signalled when the result from one work_item is written to
 * stdout.
 */
స్థిర pthread_cond_t cond_write;

/* Signalled when we are finished with everything. */
స్థిర pthread_cond_t cond_ఫలితం;

స్థిర పూర్ణాంకం skip_మొదటి_పంక్తి;

స్థిర శూన్య కూడు_work(నిర్మాణం grep_opt *opt, నిర్మాణం grep_ఆధారం *gs)
{
	ఒకవేళ (opt->ద్విమాన != GREP_ద్విమాన_పాఠ్యం)
		grep_ఆధారం_ఎక్కించు_driver(gs, opt->repo->సూచీ);

	grep_తాళం();

	అయితే ((todo_చివర+1) % ARRAY_కొలత(todo) == todo_పూర్తి) {
		pthread_cond_wait(&cond_write, &grep_mutex);
	}

	todo[todo_చివర].ఆధారం = *gs;
	todo[todo_చివర].పూర్తి = 0;
	strbuf_reset(&todo[todo_చివర].బహిర్గతం);
	todo_చివర = (todo_చివర + 1) % ARRAY_కొలత(todo);

	pthread_cond_signal(&cond_కూడు);
	grep_unlock();
}

స్థిర నిర్మాణం work_అంశం *get_work(శూన్య)
{
	నిర్మాణం work_అంశం *ret;

	grep_తాళం();
	అయితే (todo_ప్రారంభం == todo_చివర && !all_work_added) {
		pthread_cond_wait(&cond_కూడు, &grep_mutex);
	}

	ఒకవేళ (todo_ప్రారంభం == todo_చివర && all_work_added) {
		ret = ఖాళీ;
	} లేకపోతే {
		ret = &todo[todo_ప్రారంభం];
		todo_ప్రారంభం = (todo_ప్రారంభం + 1) % ARRAY_కొలత(todo);
	}
	grep_unlock();
	తిరిగి ret;
}

స్థిర శూన్య work_పూర్తి(నిర్మాణం work_అంశం *w)
{
	పూర్ణాంకం old_పూర్తి;

	grep_తాళం();
	w->పూర్తి = 1;
	old_పూర్తి = todo_పూర్తి;
	కొరకు(; todo[todo_పూర్తి].పూర్తి && todo_పూర్తి != todo_ప్రారంభం;
	    todo_పూర్తి = (todo_పూర్తి+1) % ARRAY_కొలత(todo)) {
		w = &todo[todo_పూర్తి];
		ఒకవేళ (w->బహిర్గతం.len) {
			స్థిరరాశి అక్షరం *p = w->బహిర్గతం.buf;
			కొలత_t len = w->బహిర్గతం.len;

			/* Skip the leading hunk mark of the first file. */
			ఒకవేళ (skip_మొదటి_పంక్తి) {
				అయితే (len) {
					len--;
					ఒకవేళ (*p++ == '\n')
						ఆపు;
				}
				skip_మొదటి_పంక్తి = 0;
			}

			write_or_die(1, p, len);
		}
		grep_ఆధారం_clear(&w->ఆధారం);
	}

	ఒకవేళ (old_పూర్తి != todo_పూర్తి)
		pthread_cond_signal(&cond_write);

	ఒకవేళ (all_work_added && todo_పూర్తి == todo_చివర)
		pthread_cond_signal(&cond_ఫలితం);

	grep_unlock();
}

స్థిర శూన్య free_repos(శూన్య)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < repos_గమ్యం_free_nr; i++) {
		repo_clear(repos_గమ్యం_free[i]);
		విడుదల(repos_గమ్యం_free[i]);
	}
	FREE_AND_NULL(repos_గమ్యం_free);
	repos_గమ్యం_free_nr = 0;
	repos_గమ్యం_free_alloc = 0;
}

స్థిర శూన్య *నిరంతరం(శూన్య *arg)
{
	పూర్ణాంకం తగిలింది = 0;
	నిర్మాణం grep_opt *opt = arg;

	అయితే (1) {
		నిర్మాణం work_అంశం *w = get_work();
		ఒకవేళ (!w)
			ఆపు;

		opt->output_priv = w;
		తగిలింది |= grep_ఆధారం(opt, &w->ఆధారం);
		grep_ఆధారం_clear_దత్తాంశం(&w->ఆధారం);
		work_పూర్తి(w);
	}
	free_grep_patterns(opt);
	విడుదల(opt);

	తిరిగి (శూన్య*) (intptr_t) తగిలింది;
}

స్థిర శూన్య strbuf_బహిర్గతం(నిర్మాణం grep_opt *opt, స్థిరరాశి శూన్య *buf, కొలత_t కొలత)
{
	నిర్మాణం work_అంశం *w = opt->output_priv;
	strbuf_కూడు(&w->బహిర్గతం, buf, కొలత);
}

స్థిర శూన్య ప్రారంభం_దారాలు(నిర్మాణం grep_opt *opt)
{
	పూర్ణాంకం i;

	pthread_mutex_init(&grep_mutex, ఖాళీ);
	pthread_mutex_init(&grep_attr_mutex, ఖాళీ);
	pthread_cond_init(&cond_కూడు, ఖాళీ);
	pthread_cond_init(&cond_write, ఖాళీ);
	pthread_cond_init(&cond_ఫలితం, ఖాళీ);
	grep_use_locks = 1;
	enable_obj_read_తాళం();

	కొరకు (i = 0; i < ARRAY_కొలత(todo); i++) {
		strbuf_init(&todo[i].బహిర్గతం, 0);
	}

	CALLOC_ARRAY(దారాలు, అంకె_దారాలు);
	కొరకు (i = 0; i < అంకె_దారాలు; i++) {
		పూర్ణాంకం err;
		నిర్మాణం grep_opt *o = grep_opt_dup(opt);
		o->output = strbuf_బహిర్గతం;
		compile_grep_patterns(o);
		err = pthread_create(&దారాలు[i], ఖాళీ, నిరంతరం, o);

		ఒకవేళ (err)
			die(_("grep: failed to create thread: %s"),
			    strerror(err));
	}
}

స్థిర పూర్ణాంకం wait_all(శూన్య)
{
	పూర్ణాంకం తగిలింది = 0;
	పూర్ణాంకం i;

	ఒకవేళ (!HAVE_దారాలు)
		BUG("Never call this function unless you have started threads");

	grep_తాళం();
	all_work_added = 1;

	/* Wait until all work is done. */
	అయితే (todo_పూర్తి != todo_చివర)
		pthread_cond_wait(&cond_ఫలితం, &grep_mutex);

	/* Wake up all the consumer threads so they can see that there
	 * is no more work to do.
	 */
	pthread_cond_ప్రసారం(&cond_కూడు);
	grep_unlock();

	కొరకు (i = 0; i < అంకె_దారాలు; i++) {
		శూన్య *h;
		pthread_join(దారాలు[i], &h);
		తగిలింది |= (పూర్ణాంకం) (intptr_t) h;
	}

	విడుదల(దారాలు);

	pthread_mutex_destroy(&grep_mutex);
	pthread_mutex_destroy(&grep_attr_mutex);
	pthread_cond_destroy(&cond_కూడు);
	pthread_cond_destroy(&cond_write);
	pthread_cond_destroy(&cond_ఫలితం);
	grep_use_locks = 0;
	disable_obj_read_తాళం();

	తిరిగి తగిలింది;
}

స్థిర పూర్ణాంకం grep_cmd_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			   స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	పూర్ణాంకం st = grep_config(var, విలువ, ctx, cb);

	ఒకవేళ (git_color_config(var, విలువ, cb) < 0)
		st = -1;
	లేకపోతే ఒకవేళ (git_default_config(var, విలువ, ctx, cb) < 0)
		st = -1;

	ఒకవేళ (!strcmp(var, "grep.threads")) {
		అంకె_దారాలు = git_config_int(var, విలువ, ctx->kvi);
		ఒకవేళ (అంకె_దారాలు < 0)
			die(_("invalid number of threads specified (%d) for %s"),
			    అంకె_దారాలు, var);
		లేకపోతే ఒకవేళ (!HAVE_దారాలు && అంకె_దారాలు > 1) {
			/*
			 * TRANSLATORS: %s is the configuration
			 * variable for tweaking threads, currently
			 * grep.threads
			 */
			warning(_("no threads support, ignoring %s"), var);
			అంకె_దారాలు = 1;
		}
	}

	ఒకవేళ (!strcmp(var, "submodule.recurse"))
		recurse_submodules = git_config_bool(var, విలువ);

	తిరిగి st;
}

స్థిర శూన్య grep_ఆధారం_name(నిర్మాణం grep_opt *opt, స్థిరరాశి అక్షరం *filename,
			     పూర్ణాంకం వృక్షం_name_len, నిర్మాణం strbuf *బహిర్గతం)
{
	strbuf_reset(బహిర్గతం);

	ఒకవేళ (opt->null_following_name) {
		ఒకవేళ (opt->relative && grep_ఉపసర్గ) {
			నిర్మాణం strbuf rel_buf = STRBUF_INIT;
			స్థిరరాశి అక్షరం *rel_name =
				relative_మార్గం(filename + వృక్షం_name_len,
					      grep_ఉపసర్గ, &rel_buf);

			ఒకవేళ (వృక్షం_name_len)
				strbuf_కూడు(బహిర్గతం, filename, వృక్షం_name_len);

			strbuf_addstr(బహిర్గతం, rel_name);
			strbuf_release(&rel_buf);
		} లేకపోతే {
			strbuf_addstr(బహిర్గతం, filename);
		}
		తిరిగి;
	}

	ఒకవేళ (opt->relative && grep_ఉపసర్గ)
		quote_మార్గం(filename + వృక్షం_name_len, grep_ఉపసర్గ, బహిర్గతం, 0);
	లేకపోతే
		quote_c_style(filename + వృక్షం_name_len, బహిర్గతం, ఖాళీ, 0);

	ఒకవేళ (వృక్షం_name_len)
		strbuf_చొప్పించు(బహిర్గతం, 0, filename, వృక్షం_name_len);
}

స్థిర పూర్ణాంకం grep_oid(నిర్మాణం grep_opt *opt, స్థిరరాశి నిర్మాణం object_id *oid,
		     స్థిరరాశి అక్షరం *filename, పూర్ణాంకం వృక్షం_name_len,
		     స్థిరరాశి అక్షరం *మార్గం)
{
	నిర్మాణం strbuf pathbuf = STRBUF_INIT;
	నిర్మాణం grep_ఆధారం gs;

	grep_ఆధారం_name(opt, filename, వృక్షం_name_len, &pathbuf);
	grep_ఆధారం_init_oid(&gs, pathbuf.buf, మార్గం, oid, opt->repo);
	strbuf_release(&pathbuf);

	ఒకవేళ (అంకె_దారాలు > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		కూడు_work(opt, &gs);
		తిరిగి 0;
	} లేకపోతే {
		పూర్ణాంకం తగిలింది;

		తగిలింది = grep_ఆధారం(opt, &gs);

		grep_ఆధారం_clear(&gs);
		తిరిగి తగిలింది;
	}
}

స్థిర పూర్ణాంకం grep_file(నిర్మాణం grep_opt *opt, స్థిరరాశి అక్షరం *filename)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం grep_ఆధారం gs;

	grep_ఆధారం_name(opt, filename, 0, &buf);
	grep_ఆధారం_init_file(&gs, buf.buf, filename);
	strbuf_release(&buf);

	ఒకవేళ (అంకె_దారాలు > 1) {
		/*
		 * add_work() copies gs and thus assumes ownership of
		 * its fields, so do not call grep_source_clear()
		 */
		కూడు_work(opt, &gs);
		తిరిగి 0;
	} లేకపోతే {
		పూర్ణాంకం తగిలింది;

		తగిలింది = grep_ఆధారం(opt, &gs);

		grep_ఆధారం_clear(&gs);
		తిరిగి తగిలింది;
	}
}

స్థిర శూన్య append_మార్గం(నిర్మాణం grep_opt *opt, స్థిరరాశి శూన్య *దత్తాంశం, కొలత_t len)
{
	నిర్మాణం string_list *మార్గం_list = opt->output_priv;

	ఒకవేళ (len == 1 && *(స్థిరరాశి అక్షరం *)దత్తాంశం == '\0')
		తిరిగి;
	string_list_append_nodup(మార్గం_list, xstrndup(దత్తాంశం, len));
}

స్థిర శూన్య నిరంతరం_pager(నిర్మాణం grep_opt *opt, స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	నిర్మాణం string_list *మార్గం_list = opt->output_priv;
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;
	పూర్ణాంకం i, స్థితి_2;

	కొరకు (i = 0; i < మార్గం_list->nr; i++)
		strvec_నెట్టు(&బిడ్డ.వాదనలు, మార్గం_list->వస్తువులు[i].string);
	బిడ్డ.dir = ఉపసర్గ;
	బిడ్డ.use_shell = 1;

	స్థితి_2 = నిరంతరం_command(&బిడ్డ);
	ఒకవేళ (స్థితి_2)
		నిష్క్రమించు(స్థితి_2);
}

స్థిర పూర్ణాంకం grep_cache(నిర్మాణం grep_opt *opt,
		      స్థిరరాశి నిర్మాణం pathspec *pathspec, పూర్ణాంకం cached);
స్థిర పూర్ణాంకం grep_వృక్షం(నిర్మాణం grep_opt *opt, స్థిరరాశి నిర్మాణం pathspec *pathspec,
		     నిర్మాణం వృక్షం_desc *వృక్షం, నిర్మాణం strbuf *పునాది, పూర్ణాంకం tn_len,
		     పూర్ణాంకం check_attr);

స్థిర పూర్ణాంకం grep_submodule(నిర్మాణం grep_opt *opt,
			  స్థిరరాశి నిర్మాణం pathspec *pathspec,
			  స్థిరరాశి నిర్మాణం object_id *oid,
			  స్థిరరాశి అక్షరం *filename, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం cached)
{
	నిర్మాణం repository *subrepo;
	నిర్మాణం repository *superproject = opt->repo;
	నిర్మాణం grep_opt subopt;
	పూర్ణాంకం తగిలింది = 0;

	ఒకవేళ (!is_submodule_చురుకైన(superproject, మార్గం))
		తిరిగి 0;

	subrepo = xmalloc(పరిమాణం(*subrepo));
	ఒకవేళ (repo_submodule_init(subrepo, superproject, మార్గం, null_oid(opt->repo->hash_algo))) {
		విడుదల(subrepo);
		తిరిగి 0;
	}
	ALLOC_GROW(repos_గమ్యం_free, repos_గమ్యం_free_nr + 1, repos_గమ్యం_free_alloc);
	repos_గమ్యం_free[repos_గమ్యం_free_nr++] = subrepo;

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
	obj_read_తాళం();
	repo_read_gitmodules(subrepo, 0);
	obj_read_unlock();

	జ్ఞాపకం_నకలు(&subopt, opt, పరిమాణం(subopt));
	subopt.repo = subrepo;

	ఒకవేళ (oid) {
		గణన object_type object_type;
		నిర్మాణం వృక్షం_desc వృక్షం;
		శూన్య *దత్తాంశం;
		కొలత_t కొలత;
		నిర్మాణం strbuf పునాది = STRBUF_INIT;

		obj_read_తాళం();
		object_type = odb_read_object_info(subrepo->objects, oid, ఖాళీ);
		obj_read_unlock();
		దత్తాంశం = odb_read_object_peeled(subrepo->objects, oid, OBJ_వృక్షం, &కొలత, ఖాళీ);
		ఒకవేళ (!దత్తాంశం)
			die(_("unable to read tree (%s)"), oid_గమ్యం_hex(oid));

		strbuf_addstr(&పునాది, filename);
		strbuf_addch(&పునాది, '/');

		init_వృక్షం_desc(&వృక్షం, oid, దత్తాంశం, కొలత);
		తగిలింది = grep_వృక్షం(&subopt, pathspec, &వృక్షం, &పునాది, పునాది.len,
				object_type == OBJ_COMMIT);
		strbuf_release(&పునాది);
		విడుదల(దత్తాంశం);
	} లేకపోతే {
		తగిలింది = grep_cache(&subopt, pathspec, cached);
	}

	తిరిగి తగిలింది;
}

స్థిర పూర్ణాంకం grep_cache(నిర్మాణం grep_opt *opt,
		      స్థిరరాశి నిర్మాణం pathspec *pathspec, పూర్ణాంకం cached)
{
	నిర్మాణం repository *repo = opt->repo;
	పూర్ణాంకం తగిలింది = 0;
	పూర్ణాంకం nr;
	నిర్మాణం strbuf name = STRBUF_INIT;
	పూర్ణాంకం name_పునాది_len = 0;
	ఒకవేళ (repo->submodule_ఉపసర్గ) {
		name_పునాది_len = అక్షరశ్రేణి_పొడవు(repo->submodule_ఉపసర్గ);
		strbuf_addstr(&name, repo->submodule_ఉపసర్గ);
	}

	ఒకవేళ (repo_read_సూచీ(repo) < 0)
		die(_("index file corrupt"));

	కొరకు (nr = 0; nr < repo->సూచీ->cache_nr; nr++) {
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = repo->సూచీ->cache[nr];

		ఒకవేళ (!cached && ce_skip_worktree(ce))
			కొనసాగు;

		strbuf_setlen(&name, name_పునాది_len);
		strbuf_addstr(&name, ce->name);
		ఒకవేళ (S_ISSPARSEDIR(ce->ce_బహుళకం)) {
			గణన object_type type;
			నిర్మాణం వృక్షం_desc వృక్షం;
			శూన్య *దత్తాంశం;
			కొలత_t కొలత;

			దత్తాంశం = odb_read_object(the_repository->objects, &ce->oid,
					       &type, &కొలత);
			ఒకవేళ (!దత్తాంశం)
				die(_("unable to read tree %s"), oid_గమ్యం_hex(&ce->oid));
			init_వృక్షం_desc(&వృక్షం, &ce->oid, దత్తాంశం, కొలత);

			తగిలింది |= grep_వృక్షం(opt, pathspec, &వృక్షం, &name, 0, 0);
			strbuf_setlen(&name, name_పునాది_len);
			strbuf_addstr(&name, ce->name);
			విడుదల(దత్తాంశం);
		} లేకపోతే ఒకవేళ (S_ISREG(ce->ce_బహుళకం) &&
		    సరిపోలిక_pathspec(repo->సూచీ, pathspec, name.buf, name.len, 0, ఖాళీ,
				   S_ISDIR(ce->ce_బహుళకం) ||
				   S_ISGITLINK(ce->ce_బహుళకం))) {
			/*
			 * If CE_VALID is on, we assume worktree file and its
			 * cache entry are identical, even if worktree file has
			 * been modified, so use cache version instead
			 */
			ఒకవేళ (cached || (ce->ce_flags & CE_VALID)) {
				ఒకవేళ (ce_stage(ce) || ce_intent_గమ్యం_కూడు(ce))
					కొనసాగు;
				తగిలింది |= grep_oid(opt, &ce->oid, name.buf,
						 0, name.buf);
			} లేకపోతే {
				తగిలింది |= grep_file(opt, name.buf);
			}
		} లేకపోతే ఒకవేళ (recurse_submodules && S_ISGITLINK(ce->ce_బహుళకం) &&
			   submodule_మార్గం_సరిపోలిక(repo->సూచీ, pathspec, name.buf, ఖాళీ)) {
			తగిలింది |= grep_submodule(opt, pathspec, ఖాళీ, ce->name,
					      ce->name, cached);
		} లేకపోతే {
			కొనసాగు;
		}

		ఒకవేళ (ce_stage(ce)) {
			చేయి {
				nr++;
			} అయితే (nr < repo->సూచీ->cache_nr &&
				 !strcmp(ce->name, repo->సూచీ->cache[nr]->name));
			nr--; /* compensate for loop control */
		}
		ఒకవేళ (తగిలింది && opt->స్థితి_only)
			ఆపు;
	}

	strbuf_release(&name);
	తిరిగి తగిలింది;
}

స్థిర పూర్ణాంకం grep_వృక్షం(నిర్మాణం grep_opt *opt, స్థిరరాశి నిర్మాణం pathspec *pathspec,
		     నిర్మాణం వృక్షం_desc *వృక్షం, నిర్మాణం strbuf *పునాది, పూర్ణాంకం tn_len,
		     పూర్ణాంకం check_attr)
{
	నిర్మాణం repository *repo = opt->repo;
	పూర్ణాంకం తగిలింది = 0;
	గణన interesting సరిపోలిక = నమోదు_not_interesting;
	నిర్మాణం name_నమోదు నమోదు_2;
	పూర్ణాంకం old_baselen = పునాది->len;
	నిర్మాణం strbuf name = STRBUF_INIT;
	పూర్ణాంకం name_పునాది_len = 0;
	ఒకవేళ (repo->submodule_ఉపసర్గ) {
		strbuf_addstr(&name, repo->submodule_ఉపసర్గ);
		name_పునాది_len = name.len;
	}

	అయితే (వృక్షం_నమోదు(వృక్షం, &నమోదు_2)) {
		పూర్ణాంకం te_len = వృక్షం_నమోదు_len(&నమోదు_2);

		ఒకవేళ (సరిపోలిక != all_entries_interesting) {
			strbuf_addstr(&name, పునాది->buf + tn_len);
			సరిపోలిక = వృక్షం_నమోదు_interesting(repo->సూచీ,
						       &నమోదు_2, &name,
						       pathspec);
			strbuf_setlen(&name, name_పునాది_len);

			ఒకవేళ (సరిపోలిక == all_entries_not_interesting)
				ఆపు;
			ఒకవేళ (సరిపోలిక == నమోదు_not_interesting)
				కొనసాగు;
		}

		strbuf_కూడు(పునాది, నమోదు_2.మార్గం, te_len);

		ఒకవేళ (S_ISREG(నమోదు_2.బహుళకం)) {
			తగిలింది |= grep_oid(opt, &నమోదు_2.oid, పునాది->buf, tn_len,
					 check_attr ? పునాది->buf + tn_len : ఖాళీ);
		} లేకపోతే ఒకవేళ (S_ISDIR(నమోదు_2.బహుళకం)) {
			గణన object_type type;
			నిర్మాణం వృక్షం_desc sub;
			శూన్య *దత్తాంశం;
			కొలత_t కొలత;

			దత్తాంశం = odb_read_object(the_repository->objects,
					       &నమోదు_2.oid, &type, &కొలత);
			ఒకవేళ (!దత్తాంశం)
				die(_("unable to read tree (%s)"),
				    oid_గమ్యం_hex(&నమోదు_2.oid));

			strbuf_addch(పునాది, '/');
			init_వృక్షం_desc(&sub, &నమోదు_2.oid, దత్తాంశం, కొలత);
			తగిలింది |= grep_వృక్షం(opt, pathspec, &sub, పునాది, tn_len,
					 check_attr);
			విడుదల(దత్తాంశం);
		} లేకపోతే ఒకవేళ (recurse_submodules && S_ISGITLINK(నమోదు_2.బహుళకం)) {
			తగిలింది |= grep_submodule(opt, pathspec, &నమోదు_2.oid,
					      పునాది->buf, పునాది->buf + tn_len,
					      1); /* ignored */
		}

		strbuf_setlen(పునాది, old_baselen);

		ఒకవేళ (తగిలింది && opt->స్థితి_only)
			ఆపు;
	}

	strbuf_release(&name);
	తిరిగి తగిలింది;
}

స్థిర శూన్య collect_blob_oids_for_వృక్షం(నిర్మాణం repository *repo,
				       స్థిరరాశి నిర్మాణం pathspec *pathspec,
				       నిర్మాణం వృక్షం_desc *వృక్షం,
				       నిర్మాణం strbuf *పునాది,
				       పూర్ణాంకం tn_len,
				       నిర్మాణం oidset *blob_oids)
{
	నిర్మాణం name_నమోదు నమోదు_2;
	పూర్ణాంకం old_baselen = పునాది->len;
	నిర్మాణం strbuf name = STRBUF_INIT;
	గణన interesting సరిపోలిక = నమోదు_not_interesting;

	అయితే (వృక్షం_నమోదు(వృక్షం, &నమోదు_2)) {
		ఒకవేళ (సరిపోలిక != all_entries_interesting) {
			strbuf_addstr(&name, పునాది->buf + tn_len);
			సరిపోలిక = వృక్షం_నమోదు_interesting(repo->సూచీ,
						       &నమోదు_2, &name,
						       pathspec);
			strbuf_reset(&name);

			ఒకవేళ (సరిపోలిక == all_entries_not_interesting)
				ఆపు;
			ఒకవేళ (సరిపోలిక == నమోదు_not_interesting)
				కొనసాగు;
		}

		strbuf_కూడు(పునాది, నమోదు_2.మార్గం, వృక్షం_నమోదు_len(&నమోదు_2));

		ఒకవేళ (S_ISREG(నమోదు_2.బహుళకం)) {
			ఒకవేళ (!odb_has_object(repo->objects, &నమోదు_2.oid, 0))
				oidset_చొప్పించు(blob_oids, &నమోదు_2.oid);
		} లేకపోతే ఒకవేళ (S_ISDIR(నమోదు_2.బహుళకం)) {
			గణన object_type type;
			నిర్మాణం వృక్షం_desc sub_వృక్షం;
			శూన్య *దత్తాంశం;
			కొలత_t కొలత;

			దత్తాంశం = odb_read_object(repo->objects, &నమోదు_2.oid,
					       &type, &కొలత);
			ఒకవేళ (!దత్తాంశం)
				die(_("unable to read tree (%s)"),
				    oid_గమ్యం_hex(&నమోదు_2.oid));

			strbuf_addch(పునాది, '/');
			init_వృక్షం_desc(&sub_వృక్షం, &నమోదు_2.oid, దత్తాంశం, కొలత);
			collect_blob_oids_for_వృక్షం(repo, pathspec, &sub_వృక్షం,
						   పునాది, tn_len, blob_oids);
			విడుదల(దత్తాంశం);
		}
		/*
		 * ...no else clause for S_ISGITLINK: submodules have their
		 * own promisor configuration and would need separate fetches
		 * anyway.
		 */

		strbuf_setlen(పునాది, old_baselen);
	}

	strbuf_release(&name);
}

స్థిర శూన్య collect_blob_oids_for_treeish(నిర్మాణం grep_opt *opt,
					  స్థిరరాశి నిర్మాణం pathspec *pathspec,
					  స్థిరరాశి నిర్మాణం object_id *వృక్షం_ish_oid,
					  స్థిరరాశి అక్షరం *name,
					  నిర్మాణం oidset *blob_oids)
{
	నిర్మాణం వృక్షం_desc వృక్షం;
	శూన్య *దత్తాంశం;
	కొలత_t కొలత;
	నిర్మాణం strbuf పునాది = STRBUF_INIT;
	పూర్ణాంకం len;

	దత్తాంశం = odb_read_object_peeled(opt->repo->objects, వృక్షం_ish_oid,
				      OBJ_వృక్షం, &కొలత, ఖాళీ);

	ఒకవేళ (!దత్తాంశం)
		తిరిగి;

	len = name ? అక్షరశ్రేణి_పొడవు(name) : 0;
	ఒకవేళ (len) {
		strbuf_కూడు(&పునాది, name, len);
		strbuf_addch(&పునాది, ':');
	}
	init_వృక్షం_desc(&వృక్షం, వృక్షం_ish_oid, దత్తాంశం, కొలత);

	collect_blob_oids_for_వృక్షం(opt->repo, pathspec, &వృక్షం,
				   &పునాది, పునాది.len, blob_oids);

	strbuf_release(&పునాది);
	విడుదల(దత్తాంశం);
}

స్థిర శూన్య prefetch_grep_blobs(నిర్మాణం grep_opt *opt,
				స్థిరరాశి నిర్మాణం pathspec *pathspec,
				స్థిరరాశి నిర్మాణం object_array *list)
{
	నిర్మాణం oidset blob_oids = OIDSET_INIT;

	/* Exit if we're not in a partial clone */
	ఒకవేళ (!repo_has_promisor_remote(opt->repo))
		తిరిగి;

	/* For each tree, gather the blobs in it */
	కొరకు (పూర్ణాంకం i = 0; i < list->nr; i++) {
		నిర్మాణం object *real_obj;

		obj_read_తాళం();
		real_obj = deref_ట్యాగ్(opt->repo, list->objects[i].అంశం,
				     ఖాళీ, 0);
		obj_read_unlock();

		ఒకవేళ (real_obj &&
		    (real_obj->type == OBJ_COMMIT ||
		     real_obj->type == OBJ_వృక్షం))
			collect_blob_oids_for_treeish(opt, pathspec,
						      &real_obj->oid,
						      list->objects[i].name,
						      &blob_oids);
	}

	/* Prefetch the blobs we found */
	ఒకవేళ (oidset_కొలత(&blob_oids)) {
		నిర్మాణం oid_array గమ్యం_fetch = OID_ARRAY_INIT;
		నిర్మాణం oidset_iter iter;
		స్థిరరాశి నిర్మాణం object_id *oid;

		oidset_iter_init(&blob_oids, &iter);
		అయితే ((oid = oidset_iter_తదుపరి(&iter)))
			oid_array_append(&గమ్యం_fetch, oid);

		promisor_remote_get_ప్రత్యక్ష(opt->repo, గమ్యం_fetch.oid, గమ్యం_fetch.nr);

		oid_array_clear(&గమ్యం_fetch);
	}
	oidset_clear(&blob_oids);
}

స్థిర పూర్ణాంకం grep_object(నిర్మాణం grep_opt *opt, స్థిరరాశి నిర్మాణం pathspec *pathspec,
		       నిర్మాణం object *obj, స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *మార్గం)
{
	ఒకవేళ (obj->type == OBJ_BLOB)
		తిరిగి grep_oid(opt, &obj->oid, name, 0, మార్గం);
	ఒకవేళ (obj->type == OBJ_COMMIT || obj->type == OBJ_వృక్షం) {
		నిర్మాణం వృక్షం_desc వృక్షం;
		శూన్య *దత్తాంశం;
		కొలత_t కొలత;
		నిర్మాణం strbuf పునాది;
		పూర్ణాంకం తగిలింది, len;

		దత్తాంశం = odb_read_object_peeled(opt->repo->objects, &obj->oid,
					      OBJ_వృక్షం, &కొలత, ఖాళీ);
		ఒకవేళ (!దత్తాంశం)
			die(_("unable to read tree (%s)"), oid_గమ్యం_hex(&obj->oid));

		len = name ? అక్షరశ్రేణి_పొడవు(name) : 0;
		strbuf_init(&పునాది, మార్గం_MAX + len + 1);
		ఒకవేళ (len) {
			strbuf_కూడు(&పునాది, name, len);
			strbuf_addch(&పునాది, ':');
		}
		init_వృక్షం_desc(&వృక్షం, &obj->oid, దత్తాంశం, కొలత);
		తగిలింది = grep_వృక్షం(opt, pathspec, &వృక్షం, &పునాది, పునాది.len,
				obj->type == OBJ_COMMIT);
		strbuf_release(&పునాది);
		విడుదల(దత్తాంశం);
		తిరిగి తగిలింది;
	}
	die(_("unable to grep from object of type %s"), type_name(obj->type));
}

స్థిర పూర్ణాంకం grep_objects(నిర్మాణం grep_opt *opt, స్థిరరాశి నిర్మాణం pathspec *pathspec,
			స్థిరరాశి నిర్మాణం object_array *list)
{
	గుర్తులేని పూర్ణాంకం i;
	పూర్ణాంకం తగిలింది = 0;
	స్థిరరాశి గుర్తులేని పూర్ణాంకం nr = list->nr;

	prefetch_grep_blobs(opt, pathspec, list);

	కొరకు (i = 0; i < nr; i++) {
		నిర్మాణం object *real_obj;

		obj_read_తాళం();
		real_obj = deref_ట్యాగ్(opt->repo, list->objects[i].అంశం,
				     ఖాళీ, 0);
		obj_read_unlock();

		ఒకవేళ (!real_obj) {
			అక్షరం hex[GIT_MAX_HEXSZ + 1];
			స్థిరరాశి అక్షరం *name = list->objects[i].name;

			ఒకవేళ (!name) {
				oid_గమ్యం_hex_r(hex, &list->objects[i].అంశం->oid);
				name = hex;
			}
			die(_("invalid object '%s' given."), name);
		}

		/* load the gitmodules file for this rev */
		ఒకవేళ (recurse_submodules) {
			submodule_free(opt->repo);
			obj_read_తాళం();
			gitmodules_config_oid(the_repository, &real_obj->oid);
			obj_read_unlock();
		}
		ఒకవేళ (grep_object(opt, pathspec, real_obj, list->objects[i].name,
				list->objects[i].మార్గం)) {
			తగిలింది = 1;
			ఒకవేళ (opt->స్థితి_only)
				ఆపు;
		}
	}
	తిరిగి తగిలింది;
}

స్థిర పూర్ణాంకం grep_directory(నిర్మాణం grep_opt *opt, స్థిరరాశి నిర్మాణం pathspec *pathspec,
			  పూర్ణాంకం exc_std, పూర్ణాంకం use_సూచీ)
{
	నిర్మాణం dir_struct dir = DIR_INIT;
	పూర్ణాంకం i, తగిలింది = 0;

	ఒకవేళ (!use_సూచీ)
		dir.flags |= DIR_NO_GITLINKS;
	ఒకవేళ (exc_std)
		అమరిక_standard_excludes(&dir);

	fill_directory(&dir, opt->repo->సూచీ, pathspec);
	కొరకు (i = 0; i < dir.nr; i++) {
		తగిలింది |= grep_file(opt, dir.entries[i]->name);
		ఒకవేళ (తగిలింది && opt->స్థితి_only)
			ఆపు;
	}
	dir_clear(&dir);
	తిరిగి తగిలింది;
}

స్థిర పూర్ణాంకం context_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			    పూర్ణాంకం unset)
{
	నిర్మాణం grep_opt *grep_opt = opt->విలువ;
	పూర్ణాంకం విలువ;
	స్థిరరాశి అక్షరం *endp;

	ఒకవేళ (unset) {
		grep_opt->pre_context = grep_opt->post_context = 0;
		తిరిగి 0;
	}
	విలువ = strtol(arg, (అక్షరం **)&endp, 10);
	ఒకవేళ (*endp) {
		తిరిగి error(_("switch `%c' expects a numerical value"),
			     opt->short_name);
	}
	grep_opt->pre_context = grep_opt->post_context = విలువ;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం file_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం grep_opt *grep_opt = opt->విలువ;
	పూర్ణాంకం from_stdin;
	స్థిరరాశి అక్షరం *filename = arg;
	FILE *patterns;
	పూర్ణాంకం lno = 0;
	నిర్మాణం strbuf sb = STRBUF_INIT;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (!*filename)
		; /* leave it as-is */
	లేకపోతే
		filename = ఉపసర్గ_filename_except_for_dash(grep_ఉపసర్గ, filename);

	from_stdin = !strcmp(filename, "-");
	patterns = from_stdin ? stdin : fopen(filename, "r");
	ఒకవేళ (!patterns)
		die_errno(_("cannot open '%s'"), arg);
	అయితే (strbuf_getline(&sb, patterns) == 0) {
		/* ignore empty line like grep does */
		ఒకవేళ (sb.len == 0)
			కొనసాగు;

		append_grep_pat(grep_opt, sb.buf, sb.len, arg, ++lno,
				GREP_నమూనా);
	}
	ఒకవేళ (!from_stdin)
		fclose(patterns);
	strbuf_release(&sb);
	ఒకవేళ (filename != arg)
		విడుదల((శూన్య *)filename);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం not_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం grep_opt *grep_opt = opt->విలువ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_నమూనా(grep_opt, "--not", "command line", 0, GREP_NOT);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం and_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం grep_opt *grep_opt = opt->విలువ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_నమూనా(grep_opt, "--and", "command line", 0, GREP_AND);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం open_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం grep_opt *grep_opt = opt->విలువ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_నమూనా(grep_opt, "(", "command line", 0, GREP_OPEN_PAREN);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం close_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం grep_opt *grep_opt = opt->విలువ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	append_grep_నమూనా(grep_opt, ")", "command line", 0, GREP_CLOSE_PAREN);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం నమూనా_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			    పూర్ణాంకం unset)
{
	నిర్మాణం grep_opt *grep_opt = opt->విలువ;
	BUG_ON_OPT_NEG(unset);
	append_grep_నమూనా(grep_opt, arg, "-e option", 0, GREP_నమూనా);
	తిరిగి 0;
}

పూర్ణాంకం cmd_grep(పూర్ణాంకం argc,
	     స్థిరరాశి అక్షరం **argv,
	     స్థిరరాశి అక్షరం *ఉపసర్గ,
	     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం తగిలింది = 0;
	పూర్ణాంకం cached = 0, untracked = 0, opt_exclude = -1;
	పూర్ణాంకం చూసిన_dashdash = 0;
	పూర్ణాంకం external_grep_allowed__ignored;
	స్థిరరాశి అక్షరం *show_లోపల_pager = ఖాళీ, *default_pager = "dummy";
	నిర్మాణం grep_opt opt;
	నిర్మాణం object_array list = OBJECT_ARRAY_INIT;
	నిర్మాణం pathspec pathspec;
	నిర్మాణం string_list మార్గం_list = STRING_LIST_INIT_DUP;
	పూర్ణాంకం i;
	పూర్ణాంకం dummy;
	పూర్ణాంకం use_సూచీ = 1;
	పూర్ణాంకం allow_revs;
	పూర్ణాంకం ret;

	నిర్మాణం option options[] = {
		OPT_BOOL(0, "cached", &cached,
			N_("search in index instead of in the work tree")),
		OPT_NEGBIT(0, "no-index", &use_సూచీ,
			 N_("find in contents not managed by git"), 1),
		OPT_BOOL(0, "untracked", &untracked,
			N_("search in both tracked and untracked files")),
		OPT_సమితి_INT(0, "exclude-standard", &opt_exclude,
			    N_("ignore files specified via '.gitignore'"), 1),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			 N_("recursively search in each submodule")),
		OPT_సమూహం(""),
		OPT_BOOL('v', "invert-match", &opt.invert,
			N_("show non-matching lines")),
		OPT_BOOL('i', "ignore-case", &opt.ignore_case,
			N_("case insensitive matching")),
		OPT_BOOL('w', "word-regexp", &opt.మాట_regexp,
			N_("match patterns only at word boundaries")),
		OPT_సమితి_INT('a', "text", &opt.ద్విమాన,
			N_("process binary files as text"), GREP_ద్విమాన_పాఠ్యం),
		OPT_సమితి_INT('I', ఖాళీ, &opt.ద్విమాన,
			N_("don't match patterns in binary files"),
			GREP_ద్విమాన_NOMATCH),
		OPT_BOOL(0, "textconv", &opt.allow_textconv,
			 N_("process binary files with textconv filters")),
		OPT_సమితి_INT('r', "recursive", &opt.max_లోతు,
			    N_("search in subdirectories (default)"), -1),
		OPT_INTEGER_F(0, "max-depth", &opt.max_లోతు,
			N_("descend at most <n> levels"), PARSE_OPT_NONEG),
		OPT_సమూహం(""),
		OPT_సమితి_INT('E', "extended-regexp", &opt.నమూనా_type_option,
			    N_("use extended POSIX regular expressions"),
			    GREP_నమూనా_TYPE_ERE),
		OPT_సమితి_INT('G', "basic-regexp", &opt.నమూనా_type_option,
			    N_("use basic POSIX regular expressions (default)"),
			    GREP_నమూనా_TYPE_BRE),
		OPT_సమితి_INT('F', "fixed-strings", &opt.నమూనా_type_option,
			    N_("interpret patterns as fixed strings"),
			    GREP_నమూనా_TYPE_FIXED),
		OPT_సమితి_INT('P', "perl-regexp", &opt.నమూనా_type_option,
			    N_("use Perl-compatible regular expressions"),
			    GREP_నమూనా_TYPE_PCRE),
		OPT_సమూహం(""),
		OPT_BOOL('n', "line-number", &opt.linenum, N_("show line numbers")),
		OPT_BOOL(0, "column", &opt.columnnum, N_("show column number of first match")),
		OPT_NEGBIT('h', ఖాళీ, &opt.pathname, N_("don't show filenames"), 1),
		OPT_బిట్టు('H', ఖాళీ, &opt.pathname, N_("show filenames"), 1),
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
		OPT_BOOL('c', "count", &opt.లెక్క,
			N_("show the number of matches instead of matching lines")),
		OPT__COLOR(&opt.color, N_("highlight matches")),
		OPT_BOOL(0, "break", &opt.file_break,
			N_("print empty line between matches from different files")),
		OPT_BOOL(0, "heading", &opt.heading,
			N_("show filename only once above matches from same file")),
		OPT_సమూహం(""),
		OPT_CALLBACK('C', "context", &opt, N_("n"),
			N_("show <n> context lines before and after matches"),
			context_callback),
		OPT_UNSIGNED('B', "before-context", &opt.pre_context,
			N_("show <n> context lines before matches")),
		OPT_UNSIGNED('A', "after-context", &opt.post_context,
			N_("show <n> context lines after matches")),
		OPT_INTEGER(0, "threads", &అంకె_దారాలు,
			N_("use <n> worker threads")),
		OPT_సంఖ్య_CALLBACK(&opt, N_("shortcut for -C NUM"),
			context_callback),
		OPT_BOOL('p', "show-function", &opt.funcname,
			N_("show a line with the function name before matches")),
		OPT_BOOL('W', "function-context", &opt.funcbody,
			N_("show the surrounding function")),
		OPT_సమూహం(""),
		OPT_CALLBACK('f', ఖాళీ, &opt, N_("file"),
			N_("read patterns from file"), file_callback),
		OPT_CALLBACK_F('e', ఖాళీ, &opt, N_("pattern"),
			N_("match <pattern>"), PARSE_OPT_NONEG, నమూనా_callback),
		OPT_CALLBACK_F(0, "and", &opt, ఖాళీ,
			N_("combine patterns specified with -e"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, and_callback),
		OPT_BOOL_F(0, "or", &dummy, "", PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "not", &opt, ఖాళీ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, not_callback),
		OPT_CALLBACK_F('(', ఖాళీ, &opt, ఖాళీ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			open_callback),
		OPT_CALLBACK_F(')', ఖాళీ, &opt, ఖాళీ, "",
			PARSE_OPT_NOARG | PARSE_OPT_NONEG | PARSE_OPT_NODASH,
			close_callback),
		OPT__QUIET(&opt.స్థితి_only,
			   N_("indicate hit with exit status without output")),
		OPT_BOOL(0, "all-match", &opt.all_సరిపోలిక,
			N_("show only matches from files that match all patterns")),
		OPT_సమూహం(""),
		{
			.type = OPTION_STRING,
			.short_name = 'O',
			.long_name = "open-files-in-pager",
			.విలువ = &show_లోపల_pager,
			.argh = N_("pager"),
			.help = N_("show matching files in the pager"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NOCOMPLETE,
			.defval = (intptr_t)default_pager,
		},
		OPT_BOOL_F(0, "ext-grep", &external_grep_allowed__ignored,
			   N_("allow calling of grep(1) (ignored by this build)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_INTEGER('m', "max-count", &opt.max_లెక్క,
			N_("maximum number of results per file")),
		OPT_చివర()
	};
	grep_ఉపసర్గ = ఉపసర్గ;

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
	argc = parse_options(argc, argv, ఉపసర్గ, options, grep_usage,
			     PARSE_OPT_KEEP_DASHDASH |
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ఒకవేళ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_నిండిన_సూచీ = 0;
	}

	ఒకవేళ (use_సూచీ && !startup_info->have_repository) {
		పూర్ణాంకం fallback = 0;
		repo_config_get_bool(the_repository, "grep.fallbacktonoindex", &fallback);
		ఒకవేళ (fallback)
			use_సూచీ = 0;
		లేకపోతే
			/* die the same way as if we did it at the beginning */
			అమరిక_git_directory(the_repository);
	}
	/* Ignore --recurse-submodules if --no-index is given or implied */
	ఒకవేళ (!use_సూచీ)
		recurse_submodules = 0;

	/*
	 * skip a -- separator; we know it cannot be
	 * separating revisions from pathnames if
	 * we haven't even had any patterns yet
	 */
	ఒకవేళ (argc > 0 && !opt.నమూనా_list && !strcmp(argv[0], "--")) {
		argv++;
		argc--;
	}

	/* First unrecognized non-option token */
	ఒకవేళ (argc > 0 && !opt.నమూనా_list) {
		append_grep_నమూనా(&opt, argv[0], "command line", 0,
				    GREP_నమూనా);
		argv++;
		argc--;
	}

	ఒకవేళ (show_లోపల_pager == default_pager)
		show_లోపల_pager = git_pager(the_repository, 1);
	ఒకవేళ (show_లోపల_pager) {
		opt.color = GIT_COLOR_NEVER;
		opt.name_only = 1;
		opt.null_following_name = 1;
		opt.output_priv = &మార్గం_list;
		opt.output = append_మార్గం;
		string_list_append(&మార్గం_list, show_లోపల_pager);
	}

	ఒకవేళ (!opt.నమూనా_list)
		die(_("no pattern given"));

	/* --only-matching has no effect with --invert. */
	ఒకవేళ (opt.invert)
		opt.only_matching = 0;

	/*
	 * We have to find "--" in a separate pass, because its presence
	 * influences how we will parse arguments that come before it.
	 */
	కొరకు (i = 0; i < argc; i++) {
		ఒకవేళ (!strcmp(argv[i], "--")) {
			చూసిన_dashdash = 1;
			ఆపు;
		}
	}

	/*
	 * Resolve any rev arguments. If we have a dashdash, then everything up
	 * to it must resolve as a rev. If not, then we stop at the first
	 * non-rev and assume everything else is a path.
	 */
	allow_revs = use_సూచీ && !untracked;
	కొరకు (i = 0; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];
		నిర్మాణం object_id oid;
		నిర్మాణం object_context oc = {0};
		నిర్మాణం object *object;

		ఒకవేళ (!strcmp(arg, "--")) {
			i++;
			ఆపు;
		}

		ఒకవేళ (!allow_revs) {
			ఒకవేళ (చూసిన_dashdash)
				die(_("--no-index or --untracked cannot be used with revs"));
			ఆపు;
		}

		ఒకవేళ (get_oid_with_context(the_repository, arg,
					 GET_OID_రికార్డు_మార్గం,
					 &oid, &oc)) {
			ఒకవేళ (చూసిన_dashdash)
				die(_("unable to resolve revision: %s"), arg);
			object_context_release(&oc);
			ఆపు;
		}

		object = parse_object_or_die(the_repository, &oid, arg);
		ఒకవేళ (!చూసిన_dashdash)
			verify_non_filename(the_repository, ఉపసర్గ, arg);
		కూడు_object_array_with_మార్గం(object, arg, &list, oc.బహుళకం, oc.మార్గం);
		object_context_release(&oc);
	}

	/*
	 * Anything left over is presumed to be a path. But in the non-dashdash
	 * "do what I mean" case, we verify and complain when that isn't true.
	 */
	ఒకవేళ (!చూసిన_dashdash) {
		పూర్ణాంకం j;
		కొరకు (j = i; j < argc; j++)
			verify_filename(the_repository, ఉపసర్గ, argv[j], j == i && allow_revs);
	}

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD |
		       (opt.max_లోతు != -1 ? PATHSPEC_MAXDEPTH_VALID : 0),
		       ఉపసర్గ, argv + i);
	pathspec.max_లోతు = opt.max_లోతు;
	pathspec.recursive = 1;
	pathspec.recurse_submodules = !!recurse_submodules;

	ఒకవేళ (recurse_submodules && untracked)
		die(_("--untracked not supported with --recurse-submodules"));

	/*
	 * Optimize out the case where the amount of matches is limited to zero.
	 * We do this to keep results consistent with GNU grep(1).
	 */
	ఒకవేళ (opt.max_లెక్క == 0) {
		ret = 1;
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (show_లోపల_pager) {
		ఒకవేళ (అంకె_దారాలు > 1)
			warning(_("invalid option combination, ignoring --threads"));
		అంకె_దారాలు = 1;
	} లేకపోతే ఒకవేళ (!HAVE_దారాలు && అంకె_దారాలు > 1) {
		warning(_("no threads support, ignoring --threads"));
		అంకె_దారాలు = 1;
	} లేకపోతే ఒకవేళ (అంకె_దారాలు < 0)
		die(_("invalid number of threads specified (%d)"), అంకె_దారాలు);
	లేకపోతే ఒకవేళ (అంకె_దారాలు == 0)
		అంకె_దారాలు = HAVE_దారాలు ? online_cpus() : 1;

	ఒకవేళ (అంకె_దారాలు > 1) {
		ఒకవేళ (!HAVE_దారాలు)
			BUG("Somebody got num_threads calculation wrong!");
		ఒకవేళ (!(opt.name_only || opt.unmatch_name_only || opt.లెక్క)
		    && (opt.pre_context || opt.post_context ||
			opt.file_break || opt.funcbody))
			skip_మొదటి_పంక్తి = 1;

		/*
		 * Pre-read gitmodules (if not read already) and force eager
		 * initialization of packed_git to prevent racy lazy
		 * reading/initialization once worker threads are started.
		 */
		ఒకవేళ (recurse_submodules)
			repo_read_gitmodules(the_repository, 1);

		ఒకవేళ (startup_info->have_repository)
			odb_prepare(the_repository->objects, 0);

		ప్రారంభం_దారాలు(&opt);
	} లేకపోతే {
		/*
		 * The compiled patterns on the main path are only
		 * used when not using threading. Otherwise
		 * start_threads() above calls compile_grep_patterns()
		 * for each thread.
		 */
		compile_grep_patterns(&opt);
	}

	ఒకవేళ (show_లోపల_pager && (cached || list.nr))
		die(_("--open-files-in-pager only works on the worktree"));

	ఒకవేళ (show_లోపల_pager && opt.నమూనా_list && !opt.నమూనా_list->తదుపరి) {
		స్థిరరాశి అక్షరం *pager = మార్గం_list.వస్తువులు[0].string;
		పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(pager);

		ఒకవేళ (len > 4 && is_dir_sep(pager[len - 5]))
			pager += len - 4;

		ఒకవేళ (opt.ignore_case && !strcmp("less", pager))
			string_list_append(&మార్గం_list, "-I");

		ఒకవేళ (!strcmp("less", pager) || !strcmp("vi", pager)) {
			నిర్మాణం strbuf buf = STRBUF_INIT;
			strbuf_addf(&buf, "+/%s%s",
					strcmp("less", pager) ? "" : "*",
					opt.నమూనా_list->నమూనా);
			string_list_append_nodup(&మార్గం_list,
						 strbuf_detach(&buf, ఖాళీ));
		}
	}

	ఒకవేళ (!show_లోపల_pager && !opt.స్థితి_only)
		అమరిక_pager(the_repository);

	die_for_incompatible_opt3(!use_సూచీ, "--no-index",
				  untracked, "--untracked",
				  cached, "--cached");

	ఒకవేళ (!use_సూచీ || untracked) {
		పూర్ణాంకం use_exclude = (opt_exclude < 0) ? use_సూచీ : !!opt_exclude;
		తగిలింది = grep_directory(&opt, &pathspec, use_exclude, use_సూచీ);
	} లేకపోతే ఒకవేళ (0 <= opt_exclude) {
		die(_("--[no-]exclude-standard cannot be used for tracked contents"));
	} లేకపోతే ఒకవేళ (!list.nr) {
		ఒకవేళ (!cached)
			అమరిక_work_వృక్షం(the_repository);

		తగిలింది = grep_cache(&opt, &pathspec, cached);
	} లేకపోతే {
		ఒకవేళ (cached)
			die(_("both --cached and trees are given"));

		తగిలింది = grep_objects(&opt, &pathspec, &list);
	}

	ఒకవేళ (అంకె_దారాలు > 1)
		తగిలింది |= wait_all();
	ఒకవేళ (తగిలింది && show_లోపల_pager)
		నిరంతరం_pager(&opt, ఉపసర్గ);

	ret = !తగిలింది;

బహిర్గతం:
	clear_pathspec(&pathspec);
	string_list_clear(&మార్గం_list, 0);
	free_grep_patterns(&opt);
	object_array_clear(&list);
	free_repos();
	తిరిగి ret;
}
