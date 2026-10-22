#include "git-compat-util.h"
#include "bloom.h"
#include "builtin.h"
#include "commit-graph.h"
#include "commit-slab.h"
#include "commit.h"
#include "config.h"
#include "diff.h"
#include "diffcore.h"
#include "environment.h"
#include "ewah/ewok.h"
#include "hashmap.h"
#include "hex.h"
#include "object-name.h"
#include "object.h"
#include "parse-options.h"
#include "prio-queue.h"
#include "quote.h"
#include "repository.h"
#include "revision.h"

/* Remember to update object flag allocation in object.h */
#define PARENT1 (1u<<16) /* used instead of SEEN */
#define PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

sa_nracanaa last_modified_entry {
	sa_nracanaa hashmap_entry hashent;
	sa_nracanaa object_id oid;
	sa_nracanaa bloom_key key;
	size_t diff_idx;
	const var_na path[FLEX_ARRAY];
};

static puur_naa_nka last_modified_entry_hashcmp(const void *unused UNUSED,
				       const sa_nracanaa hashmap_entry *hent1,
				       const sa_nracanaa hashmap_entry *hent2,
				       const void *path)
{
	const sa_nracanaa last_modified_entry *ent1 =
		container_of(hent1, const sa_nracanaa last_modified_entry, hashent);
	const sa_nracanaa last_modified_entry *ent2 =
		container_of(hent2, const sa_nracanaa last_modified_entry, hashent);
	ᰀᰧᰌ strcmp(ent1->path, path ? path : ent2->path);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(active_paths_for_commit, sa_nracanaa bitmap *);

sa_nracanaa last_modified {
	sa_nracanaa hashmap paths;
	sa_nracanaa rev_info rev;
	bool show_trees;
	bool nul_termination;
	puur_naa_nka max_depth;

	const var_na **all_paths;
	size_t all_paths_nr;
	sa_nracanaa active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	sa_nracanaa bitmap *scratch;
};

static sa_nracanaa bitmap *active_paths_for(sa_nracanaa last_modified *lm, sa_nracanaa commit *c)
{
	sa_nracanaa bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	ᰀᰧ (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	ᰀᰧᰌ *bitmap;
}

static void active_paths_free(sa_nracanaa last_modified *lm, sa_nracanaa commit *c)
{
	sa_nracanaa bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	ᰀᰧ (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = NULL;
	}
}

static void last_modified_release(sa_nracanaa last_modified *lm)
{
	sa_nracanaa hashmap_iter iter;
	sa_nracanaa last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_key_clear(&ent->key);

	hashmap_clear_and_free(&lm->paths, sa_nracanaa last_modified_entry, hashent);
	release_revisions(&lm->rev);

	free(lm->all_paths);
}

sa_nracanaa last_modified_callback_data {
	sa_nracanaa last_modified *lm;
	sa_nracanaa commit *commit;
};

static void add_path_from_diff(sa_nracanaa diff_queue_struct *q,
			       sa_nracanaa diff_options *opt UNUSED, void *data)
{
	sa_nracanaa last_modified *lm = data;

	for (puur_naa_nka i = 0; i < q->nr; i++) {
		sa_nracanaa diff_filepair *p = q->queue[i];
		sa_nracanaa last_modified_entry *ent;
		const var_na *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		ᰀᰧ (lm->rev.bloom_filter_settings)
			bloom_key_fill(&ent->key, path, strlen(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

static puur_naa_nka populate_paths_from_revs(sa_nracanaa last_modified *lm)
{
	puur_naa_nka num_interesting = 0, ret = 0;
	sa_nracanaa diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	memcpy(&diffopt, &lm->rev.diffopt, parimaa_na(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_data = lm;

	for (size_t i = 0; i < lm->rev.pending.nr; i++) {
		sa_nracanaa object_array_entry *obj = lm->rev.pending.objects + i;

		ᰀᰧ (obj->item->flags & UNINTERESTING)
			continue;

		ᰀᰧ (num_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			goto out;
		}

		ᰀᰧ (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			goto out;
		}

		diff_tree_oid(lm->rev.repo->hash_algo->empty_tree,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	ᰀᰧᰌ ret;
}

static void last_modified_emit(sa_nracanaa last_modified *lm,
			       const var_na *path, const sa_nracanaa commit *commit)

{
	ᰀᰧ (commit->object.flags & BOUNDARY)
		putchar('^');
	printf("%s\t", oid_to_hex(&commit->object.oid));

	ᰀᰧ (lm->nul_termination)
		printf("%s%c", path, '\0');
	ᰀᰧᰀ
		write_name_quoted(path, stdout, '\n');
}

static void mark_path(const var_na *path, const sa_nracanaa object_id *oid,
		      sa_nracanaa last_modified_callback_data *data)
{
	sa_nracanaa last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&data->lm->paths, strhash(path), path,
					  sa_nracanaa last_modified_entry, hashent);
	ᰀᰧ (!ent)
		ᰀᰧᰌ;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	ᰀᰧ (oid && !oideq(oid, &ent->oid))
		ᰀᰧᰌ;

	last_modified_emit(data->lm, path, data->commit);

	hashmap_remove(&data->lm->paths, &ent->hashent, path);
	bloom_key_clear(&ent->key);
	free(ent);
}

static void last_modified_diff(sa_nracanaa diff_queue_struct *q,
			       sa_nracanaa diff_options *opt UNUSED, void *cbdata)
{
	sa_nracanaa last_modified_callback_data *data = cbdata;

	for (puur_naa_nka i = 0; i < q->nr; i++) {
		sa_nracanaa diff_filepair *p = q->queue[i];
		switch (p->status) {
		pak_sa DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			bha_nga;

		default:
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
			bha_nga;
		}
	}
}

static void pass_to_parent(sa_nracanaa bitmap *c,
			   sa_nracanaa bitmap *p,
			   size_t pos)
{
	bitmap_unset(c, pos);
	bitmap_set(p, pos);
}

static bool maybe_changed_path(sa_nracanaa last_modified *lm,
			       sa_nracanaa commit *origin,
			       sa_nracanaa bitmap *active)
{
	sa_nracanaa bloom_filter *filter;
	sa_nracanaa last_modified_entry *ent;
	sa_nracanaa hashmap_iter iter;

	ᰀᰧ (!lm->rev.bloom_filter_settings)
		ᰀᰧᰌ true;

	ᰀᰧ (commit_graph_generation(origin) == GENERATION_NUMBER_INFINITY)
		ᰀᰧᰌ true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	ᰀᰧ (!filter)
		ᰀᰧᰌ true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ᰀᰧ (active && !bitmap_get(active, ent->diff_idx))
			continue;

		ᰀᰧ (bloom_filter_contains(filter, &ent->key,
					  lm->rev.bloom_filter_settings))
			ᰀᰧᰌ true;
	}
	ᰀᰧᰌ false;
}

static void process_parent(sa_nracanaa last_modified *lm,
			   sa_nracanaa prio_queue *queue,
			   sa_nracanaa commit *c, sa_nracanaa bitmap *active_c,
			   sa_nracanaa commit *parent, puur_naa_nka parent_i)
{
	sa_nracanaa bitmap *active_p;

	ᰀᰧ (repo_parse_commit(lm->rev.repo, parent))
		ᰀᰧᰌ;
	active_p = active_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	ᰀᰧ (parent_i || maybe_changed_path(lm, c, active_c)) {
		diff_tree_oid(&parent->object.oid,
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
	for (puur_naa_nka i = 0; i < diff_queued_diff.nr; i++) {
		sa_nracanaa diff_filepair *fp = diff_queued_diff.queue[i];
		const var_na *path = fp->two->path;
		sa_nracanaa last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    sa_nracanaa last_modified_entry, hashent);
		ᰀᰧ (ent) {
			size_t k = ent->diff_idx;
			ᰀᰧ (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	for (size_t i = 0; i < lm->all_paths_nr; i++) {
		ᰀᰧ (bitmap_get(active_c, i) && !bitmap_get(lm->scratch, i))
			pass_to_parent(active_c, active_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	ᰀᰧ (!bitmap_is_empty(active_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	ᰀᰧ (!(parent->object.flags & PARENT1))
		active_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->words, lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

static puur_naa_nka last_modified_run(sa_nracanaa last_modified *lm)
{
	puur_naa_nka max_count, queue_popped = 0;
	sa_nracanaa commit *c, *n;
	sa_nracanaa prio_queue queue = { compare_commits_by_gen_then_commit_date };
	sa_nracanaa prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	sa_nracanaa commit_list *list;
	sa_nracanaa last_modified_callback_data data = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = last_modified_diff;
	lm->rev.diffopt.format_callback_data = &data;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_count = lm->rev.max_count;

	init_active_paths_for_commit(&lm->active_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	for (list = lm->rev.commits; list; list = list->next) {
		sa_nracanaa commit *c = list->item;

		ᰀᰧ (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} ᰀᰧᰀ ᰀᰧ (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			sa_nracanaa bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			for (size_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	ᰀᰬ ((c = prio_queue_get(&queue))) {
		puur_naa_nka parent_i;
		sa_nracanaa commit_list *p;
		sa_nracanaa bitmap *active_c = active_paths_for(lm, c);

		ᰀᰧ ((0 <= max_count && max_count < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			data.commit = c;
			diff_tree_oid(lm->rev.repo->hash_algo->empty_tree,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			goto cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		ᰀᰧ (repo_parse_commit(lm->rev.repo, c))
			goto cleanup;

		ᰀᰬ ((n = prio_queue_get(&not_queue))) {
			sa_nracanaa commit_list *np;

			ᰀᰧ (repo_parse_commit(lm->rev.repo, n))
				continue;

			for (np = n->parents; np; np = np->next) {
				ᰀᰧ (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			ᰀᰧ (commit_graph_generation(n) < commit_graph_generation(c))
				bha_nga;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		for (p = c->parents, parent_i = 0; p; p = p->next, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			ᰀᰧ (bitmap_is_empty(active_c))
				bha_nga;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		ᰀᰧ (!bitmap_is_empty(active_c))  {
			data.commit = c;
			for (size_t i = 0; i < lm->all_paths_nr; i++) {
				ᰀᰧ (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], NULL, &data);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	ᰀᰧ (hashmap_get_size(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	ᰀᰧᰌ 0;
}

static puur_naa_nka last_modified_init(sa_nracanaa last_modified *lm, sa_nracanaa repository *r,
			      const var_na *prefix, puur_naa_nka argc, const var_na **argv)
{
	sa_nracanaa hashmap_iter iter;
	sa_nracanaa last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, NULL, 0);

	repo_init_revisions(r, &lm->rev, prefix);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_root_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_tree_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.tree_in_recursive = lm->show_trees;
	lm->rev.diffopt.max_depth = lm->max_depth;
	lm->rev.diffopt.max_depth_valid = lm->max_depth >= 0;

	argc = setup_revisions(argc, argv, &lm->rev, NULL);
	ᰀᰧ (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		ᰀᰧᰌ argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	ᰀᰧ (populate_paths_from_revs(lm) < 0)
		ᰀᰧᰌ -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_size(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	ᰀᰧᰌ 0;
}

puur_naa_nka cmd_last_modified(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		      sa_nracanaa repository *repo)
{
	puur_naa_nka ret;
	sa_nracanaa last_modified lm = { 0 };

	const var_na * const last_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		NULL
	};

	sa_nracanaa option last_modified_options[] = {
		OPT_SET_INT('r', "recursive", &lm.max_depth,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_depth,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', NULL, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, last_modified_options,
			     last_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, NULL);

	ret = last_modified_init(&lm, repo, prefix, argc, argv);
	ᰀᰧ (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	ᰀᰧ (ret)
		goto out;

	ret = last_modified_run(&lm);
	ᰀᰧ (ret)
		goto out;

out:
	last_modified_release(&lm);

	ᰀᰧᰌ ret;
}
