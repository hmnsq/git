#ضمن "git-compat-util.h"
#ضمن "bloom.h"
#ضمن "builtin.h"
#ضمن "commit-graph.h"
#ضمن "commit-slab.h"
#ضمن "commit.h"
#ضمن "config.h"
#ضمن "diff.h"
#ضمن "diffcore.h"
#ضمن "environment.h"
#ضمن "ewah/ewok.h"
#ضمن "hashmap.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "object.h"
#ضمن "parse-options.h"
#ضمن "prio-queue.h"
#ضمن "quote.h"
#ضمن "repository.h"
#ضمن "revision.h"

/* Remember to update object flag allocation in object.h */
#define PARENT1 (1u<<16) /* used instead of SEEN */
#define PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

بنية last_modified_entry {
	بنية hashmap_entry hashent;
	بنية object_id oid;
	بنية bloom_مفتاح مفتاح;
	size_t diff_idx;
	ثابت حرف path[FLEX_ARRAY];
};

ساكن صحيح last_modified_entry_hashcmp(ثابت فراغ *unused UNUSED,
				       ثابت بنية hashmap_entry *hent1,
				       ثابت بنية hashmap_entry *hent2,
				       ثابت فراغ *path)
{
	ثابت بنية last_modified_entry *ent1 =
		container_of(hent1, ثابت بنية last_modified_entry, hashent);
	ثابت بنية last_modified_entry *ent2 =
		container_of(hent2, ثابت بنية last_modified_entry, hashent);
	إرجاع strcmp(ent1->path, path ? path : ent2->path);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(active_paths_for_commit, بنية bitmap *);

بنية last_modified {
	بنية hashmap paths;
	بنية rev_info rev;
	bool show_trees;
	bool nul_termination;
	صحيح max_depth;

	ثابت حرف **all_paths;
	size_t all_paths_nr;
	بنية active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	بنية bitmap *scratch;
};

ساكن بنية bitmap *active_paths_for(بنية last_modified *lm, بنية commit *c)
{
	بنية bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	إذا (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	إرجاع *bitmap;
}

ساكن فراغ active_paths_free(بنية last_modified *lm, بنية commit *c)
{
	بنية bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	إذا (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = عدم;
	}
}

ساكن فراغ last_modified_release(بنية last_modified *lm)
{
	بنية hashmap_iter iter;
	بنية last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_مفتاح_clear(&ent->مفتاح);

	hashmap_clear_and_free(&lm->paths, بنية last_modified_entry, hashent);
	release_revisions(&lm->rev);

	حرر(lm->all_paths);
}

بنية last_modified_callback_data {
	بنية last_modified *lm;
	بنية commit *commit;
};

ساكن فراغ add_path_from_diff(بنية diff_queue_struct *q,
			       بنية diff_options *opt UNUSED, فراغ *data)
{
	بنية last_modified *lm = data;

	لكل (صحيح i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->queue[i];
		بنية last_modified_entry *ent;
		ثابت حرف *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		إذا (lm->rev.bloom_filter_settings)
			bloom_مفتاح_fill(&ent->مفتاح, path, strlen(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

ساكن صحيح populate_paths_from_revs(بنية last_modified *lm)
{
	صحيح رقم_interesting = 0, ret = 0;
	بنية diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	memcpy(&diffopt, &lm->rev.diffopt, حجم(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_data = lm;

	لكل (size_t i = 0; i < lm->rev.pending.nr; i++) {
		بنية object_array_entry *obj = lm->rev.pending.objects + i;

		إذا (obj->item->flags & UNINTERESTING)
			استمر;

		إذا (رقم_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			اذهب out;
		}

		إذا (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			اذهب out;
		}

		diff_شجرة_oid(lm->rev.repo->hash_algo->empty_شجرة,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	إرجاع ret;
}

ساكن فراغ last_modified_emit(بنية last_modified *lm,
			       ثابت حرف *path, ثابت بنية commit *commit)

{
	إذا (commit->object.flags & BOUNDARY)
		putchar('^');
	printf("%s\t", oid_to_hex(&commit->object.oid));

	إذا (lm->nul_termination)
		printf("%s%c", path, '\0');
	وإلا
		write_name_quoted(path, stdout, '\n');
}

ساكن فراغ mark_path(ثابت حرف *path, ثابت بنية object_id *oid,
		      بنية last_modified_callback_data *data)
{
	بنية last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&data->lm->paths, strhash(path), path,
					  بنية last_modified_entry, hashent);
	إذا (!ent)
		إرجاع;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	إذا (oid && !oideq(oid, &ent->oid))
		إرجاع;

	last_modified_emit(data->lm, path, data->commit);

	hashmap_remove(&data->lm->paths, &ent->hashent, path);
	bloom_مفتاح_clear(&ent->مفتاح);
	حرر(ent);
}

ساكن فراغ last_modified_diff(بنية diff_queue_struct *q,
			       بنية diff_options *opt UNUSED, فراغ *cbdata)
{
	بنية last_modified_callback_data *data = cbdata;

	لكل (صحيح i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->queue[i];
		بدل (p->status) {
		حالة DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			اكسر;

		افتراضي:
			/*
			 * Otherwise, we care only that we somehow arrived at
			 * a final oid state. Note that this covers some
			 * potentially controversial areas, including:
			 *
			 *  1. A rename or copy will be found, as it is the
			 *     first time the content has arrived at the given
			 *     path.
			 *
			 *  2. Even a non-content modification like a mode or
			 *     type change will trigger it.
			 *
			 * We take the inclusive approach for now, and find
			 * anything which impacts the path. Options to tweak
			 * the behavior (e.g., to "--follow" the content across
			 * renames) can come later.
			 */
			mark_path(p->two->path, &p->two->oid, data);
			اكسر;
		}
	}
}

ساكن فراغ pass_to_parent(بنية bitmap *c,
			   بنية bitmap *p,
			   size_t pos)
{
	bitmap_unset(c, pos);
	bitmap_set(p, pos);
}

ساكن bool maybe_changed_path(بنية last_modified *lm,
			       بنية commit *origin,
			       بنية bitmap *active)
{
	بنية bloom_filter *filter;
	بنية last_modified_entry *ent;
	بنية hashmap_iter iter;

	إذا (!lm->rev.bloom_filter_settings)
		إرجاع true;

	إذا (commit_graph_generation(origin) == GENERATION_العدد_INFINITY)
		إرجاع true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	إذا (!filter)
		إرجاع true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		إذا (active && !bitmap_get(active, ent->diff_idx))
			استمر;

		إذا (bloom_filter_contains(filter, &ent->مفتاح,
					  lm->rev.bloom_filter_settings))
			إرجاع true;
	}
	إرجاع false;
}

ساكن فراغ process_parent(بنية last_modified *lm,
			   بنية prio_queue *queue,
			   بنية commit *c, بنية bitmap *active_c,
			   بنية commit *parent, صحيح parent_i)
{
	بنية bitmap *active_p;

	إذا (repo_parse_commit(lm->rev.repo, parent))
		إرجاع;
	active_p = active_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	إذا (parent_i || maybe_changed_path(lm, c, active_c)) {
		diff_شجرة_oid(&parent->object.oid,
			      &c->object.oid, "", &lm->rev.diffopt);
		diffcore_std(&lm->rev.diffopt);
	}

	/*
	 * Test each path for TREESAME-ness against the parent. If a path is
	 * TREESAME, pass it on to this parent.
	 *
	 * First, collect all paths that are *not* TREESAME in 'scratch'.
	 * Then, pass paths that *are* TREESAME and active to the parent.
	 */
	لكل (صحيح i = 0; i < diff_queued_diff.nr; i++) {
		بنية diff_filepair *fp = diff_queued_diff.queue[i];
		ثابت حرف *path = fp->two->path;
		بنية last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    بنية last_modified_entry, hashent);
		إذا (ent) {
			size_t k = ent->diff_idx;
			إذا (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	لكل (size_t i = 0; i < lm->all_paths_nr; i++) {
		إذا (bitmap_get(active_c, i) && !bitmap_get(lm->scratch, i))
			pass_to_parent(active_c, active_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	إذا (!bitmap_is_empty(active_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	إذا (!(parent->object.flags & PARENT1))
		active_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->كلمات, lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

ساكن صحيح last_modified_run(بنية last_modified *lm)
{
	صحيح max_تعداد, queue_popped = 0;
	بنية commit *c, *n;
	بنية prio_queue queue = { compare_commits_by_gen_then_commit_date };
	بنية prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	بنية commit_list *list;
	بنية last_modified_callback_data data = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = last_modified_diff;
	lm->rev.diffopt.format_callback_data = &data;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_تعداد = lm->rev.max_تعداد;

	init_active_paths_for_commit(&lm->active_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	لكل (list = lm->rev.commits; list; list = list->next) {
		بنية commit *c = list->item;

		إذا (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} وإلا إذا (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			بنية bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			لكل (size_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	بينما ((c = prio_queue_get(&queue))) {
		صحيح parent_i;
		بنية commit_list *p;
		بنية bitmap *active_c = active_paths_for(lm, c);

		إذا ((0 <= max_تعداد && max_تعداد < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			data.commit = c;
			diff_شجرة_oid(lm->rev.repo->hash_algo->empty_شجرة,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			اذهب cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		إذا (repo_parse_commit(lm->rev.repo, c))
			اذهب cleanup;

		بينما ((n = prio_queue_get(&not_queue))) {
			بنية commit_list *np;

			إذا (repo_parse_commit(lm->rev.repo, n))
				استمر;

			لكل (np = n->parents; np; np = np->next) {
				إذا (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			إذا (commit_graph_generation(n) < commit_graph_generation(c))
				اكسر;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		لكل (p = c->parents, parent_i = 0; p; p = p->next, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			إذا (bitmap_is_empty(active_c))
				اكسر;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		إذا (!bitmap_is_empty(active_c))  {
			data.commit = c;
			لكل (size_t i = 0; i < lm->all_paths_nr; i++) {
				إذا (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], عدم, &data);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	إذا (hashmap_get_size(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	إرجاع 0;
}

ساكن صحيح last_modified_init(بنية last_modified *lm, بنية repository *r,
			      ثابت حرف *prefix, صحيح argc, ثابت حرف **argv)
{
	بنية hashmap_iter iter;
	بنية last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, عدم, 0);

	repo_init_revisions(r, &lm->rev, prefix);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_جذر_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_شجرة_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.شجرة_in_recursive = lm->show_trees;
	lm->rev.diffopt.max_depth = lm->max_depth;
	lm->rev.diffopt.max_depth_valid = lm->max_depth >= 0;

	argc = setup_revisions(argc, argv, &lm->rev, عدم);
	إذا (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		إرجاع argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	إذا (populate_paths_from_revs(lm) < 0)
		إرجاع -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_size(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	إرجاع 0;
}

صحيح cmd_last_modified(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		      بنية repository *repo)
{
	صحيح ret;
	بنية last_modified lm = { 0 };

	ثابت حرف * ثابت last_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		عدم
	};

	بنية option last_modified_options[] = {
		OPT_SET_INT('r', "recursive", &lm.max_depth,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_depth,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', عدم, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, last_modified_options,
			     last_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, عدم);

	ret = last_modified_init(&lm, repo, prefix, argc, argv);
	إذا (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	إذا (ret)
		اذهب out;

	ret = last_modified_run(&lm);
	إذا (ret)
		اذهب out;

out:
	last_modified_release(&lm);

	إرجاع ret;
}
