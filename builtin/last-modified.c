#અન્તર્ભાવ "git-compat-util.h"
#અન્તર્ભાવ "bloom.h"
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "commit-graph.h"
#અન્તર્ભાવ "commit-slab.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "diffcore.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "ewah/ewok.h"
#અન્તર્ભાવ "hashmap.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "object.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "prio-queue.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "repository.h"
#અન્તર્ભાવ "revision.h"

/* Remember to update object flag allocation in object.h */
#define PARENT1 (1u<<16) /* used instead of SEEN */
#define PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

સંરચના last_modified_entry {
	સંરચના hashmap_entry hashent;
	સંરચના object_id oid;
	સંરચના bloom_ચાવી ચાવી;
	size_t diff_idx;
	અચલ અક્ષર path[FLEX_ARRAY];
};

સ્થિર પૂર્ણાંક last_modified_entry_hashcmp(અચલ શૂન્ય *unused UNUSED,
				       અચલ સંરચના hashmap_entry *hent1,
				       અચલ સંરચના hashmap_entry *hent2,
				       અચલ શૂન્ય *path)
{
	અચલ સંરચના last_modified_entry *ent1 =
		container_of(hent1, અચલ સંરચના last_modified_entry, hashent);
	અચલ સંરચના last_modified_entry *ent2 =
		container_of(hent2, અચલ સંરચના last_modified_entry, hashent);
	પાછા strcmp(ent1->path, path ? path : ent2->path);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(active_paths_for_commit, સંરચના bitmap *);

સંરચના last_modified {
	સંરચના hashmap paths;
	સંરચના rev_info rev;
	bool show_trees;
	bool nul_termination;
	પૂર્ણાંક max_depth;

	અચલ અક્ષર **all_paths;
	size_t all_paths_nr;
	સંરચના active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	સંરચના bitmap *scratch;
};

સ્થિર સંરચના bitmap *active_paths_for(સંરચના last_modified *lm, સંરચના commit *c)
{
	સંરચના bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	જો (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	પાછા *bitmap;
}

સ્થિર શૂન્ય active_paths_free(સંરચના last_modified *lm, સંરચના commit *c)
{
	સંરચના bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	જો (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = ખાલી;
	}
}

સ્થિર શૂન્ય last_modified_release(સંરચના last_modified *lm)
{
	સંરચના hashmap_iter iter;
	સંરચના last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_ચાવી_clear(&ent->ચાવી);

	hashmap_clear_and_free(&lm->paths, સંરચના last_modified_entry, hashent);
	release_revisions(&lm->rev);

	મુક્ત(lm->all_paths);
}

સંરચના last_modified_callback_data {
	સંરચના last_modified *lm;
	સંરચના commit *commit;
};

સ્થિર શૂન્ય add_path_from_diff(સંરચના diff_queue_struct *q,
			       સંરચના diff_options *opt UNUSED, શૂન્ય *data)
{
	સંરચના last_modified *lm = data;

	માટે (પૂર્ણાંક i = 0; i < q->nr; i++) {
		સંરચના diff_filepair *p = q->queue[i];
		સંરચના last_modified_entry *ent;
		અચલ અક્ષર *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		જો (lm->rev.bloom_filter_settings)
			bloom_ચાવી_fill(&ent->ચાવી, path, strlen(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

સ્થિર પૂર્ણાંક populate_paths_from_revs(સંરચના last_modified *lm)
{
	પૂર્ણાંક અંક_interesting = 0, ret = 0;
	સંરચના diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	memcpy(&diffopt, &lm->rev.diffopt, કદ(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_data = lm;

	માટે (size_t i = 0; i < lm->rev.pending.nr; i++) {
		સંરચના object_array_entry *obj = lm->rev.pending.objects + i;

		જો (obj->item->flags & UNINTERESTING)
			ચાલુ;

		જો (અંક_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			જાઓ out;
		}

		જો (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			જાઓ out;
		}

		diff_વૃક્ષ_oid(lm->rev.repo->hash_algo->empty_વૃક્ષ,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	પાછા ret;
}

સ્થિર શૂન્ય last_modified_emit(સંરચના last_modified *lm,
			       અચલ અક્ષર *path, અચલ સંરચના commit *commit)

{
	જો (commit->object.flags & BOUNDARY)
		putchar('^');
	printf("%s\t", oid_to_hex(&commit->object.oid));

	જો (lm->nul_termination)
		printf("%s%c", path, '\0');
	નહીંતર
		write_name_quoted(path, stdout, '\n');
}

સ્થિર શૂન્ય mark_path(અચલ અક્ષર *path, અચલ સંરચના object_id *oid,
		      સંરચના last_modified_callback_data *data)
{
	સંરચના last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&data->lm->paths, strhash(path), path,
					  સંરચના last_modified_entry, hashent);
	જો (!ent)
		પાછા;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	જો (oid && !oideq(oid, &ent->oid))
		પાછા;

	last_modified_emit(data->lm, path, data->commit);

	hashmap_remove(&data->lm->paths, &ent->hashent, path);
	bloom_ચાવી_clear(&ent->ચાવી);
	મુક્ત(ent);
}

સ્થિર શૂન્ય last_modified_diff(સંરચના diff_queue_struct *q,
			       સંરચના diff_options *opt UNUSED, શૂન્ય *cbdata)
{
	સંરચના last_modified_callback_data *data = cbdata;

	માટે (પૂર્ણાંક i = 0; i < q->nr; i++) {
		સંરચના diff_filepair *p = q->queue[i];
		પસંદગી (p->status) {
		સ્થિતિ DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			તોડો;

		મૂળભૂત:
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
			તોડો;
		}
	}
}

સ્થિર શૂન્ય pass_to_parent(સંરચના bitmap *c,
			   સંરચના bitmap *p,
			   size_t pos)
{
	bitmap_unset(c, pos);
	bitmap_set(p, pos);
}

સ્થિર bool maybe_changed_path(સંરચના last_modified *lm,
			       સંરચના commit *origin,
			       સંરચના bitmap *active)
{
	સંરચના bloom_filter *filter;
	સંરચના last_modified_entry *ent;
	સંરચના hashmap_iter iter;

	જો (!lm->rev.bloom_filter_settings)
		પાછા true;

	જો (commit_graph_generation(origin) == GENERATION_સંખ્યા_INFINITY)
		પાછા true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	જો (!filter)
		પાછા true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		જો (active && !bitmap_get(active, ent->diff_idx))
			ચાલુ;

		જો (bloom_filter_contains(filter, &ent->ચાવી,
					  lm->rev.bloom_filter_settings))
			પાછા true;
	}
	પાછા false;
}

સ્થિર શૂન્ય process_parent(સંરચના last_modified *lm,
			   સંરચના prio_queue *queue,
			   સંરચના commit *c, સંરચના bitmap *active_c,
			   સંરચના commit *parent, પૂર્ણાંક parent_i)
{
	સંરચના bitmap *active_p;

	જો (repo_parse_commit(lm->rev.repo, parent))
		પાછા;
	active_p = active_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	જો (parent_i || maybe_changed_path(lm, c, active_c)) {
		diff_વૃક્ષ_oid(&parent->object.oid,
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
	માટે (પૂર્ણાંક i = 0; i < diff_queued_diff.nr; i++) {
		સંરચના diff_filepair *fp = diff_queued_diff.queue[i];
		અચલ અક્ષર *path = fp->two->path;
		સંરચના last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    સંરચના last_modified_entry, hashent);
		જો (ent) {
			size_t k = ent->diff_idx;
			જો (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	માટે (size_t i = 0; i < lm->all_paths_nr; i++) {
		જો (bitmap_get(active_c, i) && !bitmap_get(lm->scratch, i))
			pass_to_parent(active_c, active_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	જો (!bitmap_is_empty(active_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	જો (!(parent->object.flags & PARENT1))
		active_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->શબ્દો, lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

સ્થિર પૂર્ણાંક last_modified_run(સંરચના last_modified *lm)
{
	પૂર્ણાંક max_ગણતરી, queue_popped = 0;
	સંરચના commit *c, *n;
	સંરચના prio_queue queue = { compare_commits_by_gen_then_commit_date };
	સંરચના prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	સંરચના commit_list *list;
	સંરચના last_modified_callback_data data = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = last_modified_diff;
	lm->rev.diffopt.format_callback_data = &data;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_ગણતરી = lm->rev.max_ગણતરી;

	init_active_paths_for_commit(&lm->active_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	માટે (list = lm->rev.commits; list; list = list->next) {
		સંરચના commit *c = list->item;

		જો (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} નહીંતર જો (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			સંરચના bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			માટે (size_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	જ્યારે ((c = prio_queue_get(&queue))) {
		પૂર્ણાંક parent_i;
		સંરચના commit_list *p;
		સંરચના bitmap *active_c = active_paths_for(lm, c);

		જો ((0 <= max_ગણતરી && max_ગણતરી < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			data.commit = c;
			diff_વૃક્ષ_oid(lm->rev.repo->hash_algo->empty_વૃક્ષ,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			જાઓ cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		જો (repo_parse_commit(lm->rev.repo, c))
			જાઓ cleanup;

		જ્યારે ((n = prio_queue_get(&not_queue))) {
			સંરચના commit_list *np;

			જો (repo_parse_commit(lm->rev.repo, n))
				ચાલુ;

			માટે (np = n->parents; np; np = np->next) {
				જો (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			જો (commit_graph_generation(n) < commit_graph_generation(c))
				તોડો;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		માટે (p = c->parents, parent_i = 0; p; p = p->next, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			જો (bitmap_is_empty(active_c))
				તોડો;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		જો (!bitmap_is_empty(active_c))  {
			data.commit = c;
			માટે (size_t i = 0; i < lm->all_paths_nr; i++) {
				જો (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], ખાલી, &data);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	જો (hashmap_get_size(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	પાછા 0;
}

સ્થિર પૂર્ણાંક last_modified_init(સંરચના last_modified *lm, સંરચના repository *r,
			      અચલ અક્ષર *prefix, પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
	સંરચના hashmap_iter iter;
	સંરચના last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, ખાલી, 0);

	repo_init_revisions(r, &lm->rev, prefix);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_મૂળ_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_વૃક્ષ_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.વૃક્ષ_in_recursive = lm->show_trees;
	lm->rev.diffopt.max_depth = lm->max_depth;
	lm->rev.diffopt.max_depth_valid = lm->max_depth >= 0;

	argc = setup_revisions(argc, argv, &lm->rev, ખાલી);
	જો (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		પાછા argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	જો (populate_paths_from_revs(lm) < 0)
		પાછા -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_size(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	પાછા 0;
}

પૂર્ણાંક cmd_last_modified(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		      સંરચના repository *repo)
{
	પૂર્ણાંક ret;
	સંરચના last_modified lm = { 0 };

	અચલ અક્ષર * અચલ last_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		ખાલી
	};

	સંરચના option last_modified_options[] = {
		OPT_SET_INT('r', "recursive", &lm.max_depth,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_depth,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', ખાલી, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, last_modified_options,
			     last_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, ખાલી);

	ret = last_modified_init(&lm, repo, prefix, argc, argv);
	જો (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	જો (ret)
		જાઓ out;

	ret = last_modified_run(&lm);
	જો (ret)
		જાઓ out;

out:
	last_modified_release(&lm);

	પાછા ret;
}
