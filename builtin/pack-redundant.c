/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"

#अन्तर्भाव "packfile.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "strbuf.h"

#define BLKSIZE 512

स्थिर ध्रुव वर्ण pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

स्थिर पूर्णाङ्क load_all_packs, verbose, alt_odb;

संरचना llist_item {
	संरचना llist_item *next;
	संरचना object_id oid;
};
स्थिर संरचना llist {
	संरचना llist_item *front;
	संरचना llist_item *back;
	size_t size;
} *all_objects; /* all objects which must be present in local packfiles */

स्थिर संरचना pack_list {
	संरचना pack_list *next;
	संरचना packed_git *pack;
	संरचना llist *unique_objects;
	संरचना llist *remaining_objects;
	size_t all_objects_size;
} *local_packs = रिक्त, *altodb_packs = रिक्त;

स्थिर संरचना llist_item *free_nodes;

स्थिर अन्तःस्थ शून्य llist_item_put(संरचना llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

स्थिर अन्तःस्थ संरचना llist_item *llist_item_get(शून्य)
{
	संरचना llist_item *new_item;
	सचे ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} अञ्ञथा {
		पूर्णाङ्क i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		अत्थाय (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	पटिनिवत्त new_item;
}

स्थिर अन्तःस्थ शून्य llist_init(संरचना llist **list)
{
	*list = xmalloc(परिमाण(संरचना llist));
	(*list)->front = (*list)->back = रिक्त;
	(*list)->size = 0;
}

स्थिर शून्य llist_free(संरचना llist *list)
{
	अत्थाय (संरचना llist_item *i = list->front, *next; i; i = next) {
		next = i->next;
		llist_item_put(i);
	}
	मुक्त(list);
}

स्थिर संरचना llist * llist_copy(संरचना llist *list)
{
	संरचना llist *ret;
	संरचना llist_item *new_item, *old_item, *prev;

	llist_init(&ret);

	सचे ((ret->size = list->size) == 0)
		पटिनिवत्त ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	याव (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = रिक्त;
	ret->back = new_item;

	पटिनिवत्त ret;
}

स्थिर अन्तःस्थ संरचना llist_item *llist_निवेशय(संरचना llist *list,
					      संरचना llist_item *after,
					      ध्रुव unsigned वर्ण *oid)
{
	संरचना llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = रिक्त;

	सचे (after) {
		new_item->next = after->next;
		after->next = new_item;
		सचे (after == list->back)
			list->back = new_item;
	} अञ्ञथा {/* insert in front */
		सचे (list->size == 0)
			list->back = new_item;
		अञ्ञथा
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	पटिनिवत्त new_item;
}

स्थिर अन्तःस्थ संरचना llist_item *llist_निवेशय_back(संरचना llist *list,
						   ध्रुव unsigned वर्ण *oid)
{
	पटिनिवत्त llist_निवेशय(list, list->back, oid);
}

स्थिर अन्तःस्थ संरचना llist_item *llist_निवेशय_sorted_unique(संरचना llist *list,
			ध्रुव संरचना object_id *oid, संरचना llist_item *hint)
{
	संरचना llist_item *prev = रिक्त, *l;

	l = (hint == रिक्त) ? list->front : hint;
	याव (l) {
		पूर्णाङ्क cmp = oidcmp(&l->oid, oid);
		सचे (cmp > 0) { /* we insert before this entry */
			पटिनिवत्त llist_निवेशय(list, prev, oid->hash);
		}
		सचे (!cmp) { /* already exists */
			पटिनिवत्त l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	पटिनिवत्त llist_निवेशय_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
स्थिर अन्तःस्थ संरचना llist_item * llist_sorted_remove(संरचना llist *list, ध्रुव unsigned वर्ण *oid, संरचना llist_item *hint)
{
	संरचना llist_item *prev, *l;

redo_from_start:
	l = (hint == रिक्त) ? list->front : hint;
	prev = रिक्त;
	याव (l) {
		ध्रुव पूर्णाङ्क cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		सचे (cmp > 0) /* not in list, since sorted */
			पटिनिवत्त prev;
		सचे (!cmp) { /* found */
			सचे (!prev) {
				सचे (hint != रिक्त && hint != list->front) {
					/* we don't know the previous element */
					hint = रिक्त;
					गच्छ redo_from_start;
				}
				list->front = l->next;
			} अञ्ञथा
				prev->next = l->next;
			सचे (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			पटिनिवत्त prev;
		}
		prev = l;
		l = l->next;
	}
	पटिनिवत्त prev;
}

/* computes A\B */
स्थिर शून्य llist_sorted_difference_inplace(संरचना llist *A,
				     संरचना llist *B)
{
	संरचना llist_item *hint, *b;

	hint = रिक्त;
	b = B->front;

	याव (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

स्थिर अन्तःस्थ संरचना pack_list * pack_list_निवेशय(संरचना pack_list **pl,
					   संरचना pack_list *entry)
{
	संरचना pack_list *p = xmalloc(परिमाण(संरचना pack_list));
	memcpy(p, entry, परिमाण(संरचना pack_list));
	p->next = *pl;
	*pl = p;
	पटिनिवत्त p;
}

स्थिर शून्य pack_list_free(संरचना pack_list *pl)
{
	अत्थाय (संरचना pack_list *next; pl; pl = next) {
		next = pl->next;
		मुक्त(pl);
	}
}

स्थिर अन्तःस्थ size_t pack_list_size(संरचना pack_list *pl)
{
	size_t ret = 0;
	याव (pl) {
		ret++;
		pl = pl->next;
	}
	पटिनिवत्त ret;
}

स्थिर संरचना pack_list * pack_list_difference(ध्रुव संरचना pack_list *A,
					       ध्रुव संरचना pack_list *B)
{
	संरचना pack_list *ret;
	ध्रुव संरचना pack_list *pl;

	सचे (!A)
		पटिनिवत्त रिक्त;

	pl = B;
	याव (pl != रिक्त) {
		सचे (A->pack == pl->pack)
			पटिनिवत्त pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(परिमाण(संरचना pack_list));
	memcpy(ret, A, परिमाण(संरचना pack_list));
	ret->next = pack_list_difference(A->next, B);
	पटिनिवत्त ret;
}

स्थिर शून्य cmp_two_packs(संरचना pack_list *p1, संरचना pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ध्रुव unsigned वर्ण *p1_base, *p2_base;
	संरचना llist_item *p1_hint = रिक्त, *p2_hint = रिक्त;
	ध्रुव unsigned पूर्णाङ्क hashsz = the_hash_algo->rawsz;

	सचे (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	सचे (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	याव (p1_off < p1->pack->अङ्कः_objects * p1_step &&
	       p2_off < p2->pack->अङ्कः_objects * p2_step)
	{
		ध्रुव पूर्णाङ्क cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		सचे (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			अनुवर्त;
		}
		सचे (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} अञ्ञथा { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

स्थिर size_t sizeof_union(संरचना packed_git *p1, संरचना packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ध्रुव unsigned वर्ण *p1_base, *p2_base;
	ध्रुव unsigned पूर्णाङ्क hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	याव (p1_off < p1->अङ्कः_objects * p1_step &&
	       p2_off < p2->अङ्कः_objects * p2_step)
	{
		पूर्णाङ्क cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		सचे (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			अनुवर्त;
		}
		सचे (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} अञ्ञथा { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	पटिनिवत्त ret;
}

/* another O(n^2) function ... */
स्थिर size_t get_pack_redundancy(संरचना pack_list *pl)
{
	संरचना pack_list *subset;
	size_t ret = 0;

	सचे (!pl)
		पटिनिवत्त 0;

	याव ((subset = pl->next)) {
		याव (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	पटिनिवत्त ret;
}

स्थिर अन्तःस्थ off_t pack_set_bytecount(संरचना pack_list *pl)
{
	off_t ret = 0;
	याव (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क cmp_remaining_objects(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	संरचना pack_list *pl_a = *((संरचना pack_list **)a);
	संरचना pack_list *pl_b = *((संरचना pack_list **)b);

	सचे (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		सचे (pl_a->all_objects_size == pl_b->all_objects_size)
			पटिनिवत्त 0;
		अञ्ञथा सचे (pl_a->all_objects_size < pl_b->all_objects_size)
			पटिनिवत्त 1;
		अञ्ञथा
			पटिनिवत्त -1;
	} अञ्ञथा सचे (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		पटिनिवत्त 1;
	} अञ्ञथा {
		पटिनिवत्त -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
स्थिर शून्य sort_pack_list(संरचना pack_list **pl)
{
	संरचना pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	सचे (n < 2)
		पटिनिवत्त;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	अत्थाय (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	अत्थाय (size_t i = 0; i < n - 1; i++)
		ary[i]->next = ary[i + 1];
	ary[n - 1]->next = रिक्त;
	*pl = ary[0];

	मुक्त(ary);
}


स्थिर शून्य minimize(संरचना pack_list **min)
{
	संरचना pack_list *pl, *unique = रिक्त, *non_unique = रिक्त;
	संरचना llist *missing, *unique_pack_objects;

	pl = local_packs;
	याव (pl) {
		सचे (pl->unique_objects->size)
			pack_list_निवेशय(&unique, pl);
		अञ्ञथा
			pack_list_निवेशय(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	याव (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	सचे (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		पटिनिवत्त;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	याव (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	याव (non_unique) {
		संरचना pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		सचे (non_unique->remaining_objects->size == 0)
			भङ्ग;

		pack_list_निवेशय(min, non_unique);

		अत्थाय (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		next = non_unique->next;
		मुक्त(non_unique);
		non_unique = next;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

स्थिर शून्य load_all_objects(शून्य)
{
	संरचना pack_list *pl = local_packs;
	संरचना llist_item *hint, *l;

	llist_init(&all_objects);

	याव (pl) {
		hint = रिक्त;
		l = pl->remaining_objects->front;
		याव (l) {
			hint = llist_निवेशय_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	याव (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
स्थिर शून्य cmp_local_packs(शून्य)
{
	संरचना pack_list *subset, *pl = local_packs;

	/* only one packfile */
	सचे (!pl->next) {
		llist_init(&pl->unique_objects);
		पटिनिवत्त;
	}

	याव ((subset = pl)) {
		याव ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

स्थिर शून्य scan_alt_odb_packs(शून्य)
{
	संरचना pack_list *local, *alt;

	alt = altodb_packs;
	याव (alt) {
		local = local_packs;
		याव (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->next;
		}
		alt = alt->next;
	}
}

स्थिर संरचना pack_list * add_pack(संरचना packed_git *p)
{
	संरचना pack_list l;
	size_t off = 0, step;
	ध्रुव unsigned वर्ण *base;

	सचे (!p->pack_local && !(alt_odb || verbose))
		पटिनिवत्त रिक्त;

	l.pack = p;
	llist_init(&l.remaining_objects);

	सचे (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		पटिनिवत्त रिक्त;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	याव (off < p->अङ्कः_objects * step) {
		llist_निवेशय_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = रिक्त;
	सचे (p->pack_local)
		पटिनिवत्त pack_list_निवेशय(&local_packs, &l);
	अञ्ञथा
		पटिनिवत्त pack_list_निवेशय(&altodb_packs, &l);
}

स्थिर संरचना pack_list * add_pack_file(ध्रुव वर्ण *filename)
{
	संरचना packed_git *p;

	सचे (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		सचे (strstr(p->pack_name, filename))
			पटिनिवत्त add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

स्थिर शून्य load_all(शून्य)
{
	संरचना packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

पूर्णाङ्क cmd_pack_redundant(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix UNUSED, संरचना repository *repo UNUSED) {
	पूर्णाङ्क i; पूर्णाङ्क i_still_use_this = 0; संरचना pack_list *min = रिक्त, *red, *pl;
	संरचना llist *ignore;
	संरचना strbuf idx_name = STRBUF_INIT;
	वर्ण buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	अत्थाय (i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		सचे (!strcmp(arg, "--")) {
			i++;
			भङ्ग;
		}
		सचे (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			अनुवर्त;
		}
		सचे (!strcmp(arg, "--verbose")) {
			verbose = 1;
			अनुवर्त;
		}
		सचे (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			अनुवर्त;
		}
		सचे (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			अनुवर्त;
		}
		सचे (*arg == '-')
			usage(pack_redundant_usage);
		अञ्ञथा
			भङ्ग;
	}

	सचे (!i_still_use_this)
		you_still_use_that("git pack-redundant", रिक्त);

	सचे (load_all_packs)
		load_all();
	अञ्ञथा
		याव (*(argv + i) != रिक्त)
			add_pack_file(*(argv + i++));

	सचे (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	सचे (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	सचे (!isatty(0)) {
		संरचना object_id oid;
		याव (fgets(buf, परिमाण(buf), stdin)) {
			सचे (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_निवेशय_sorted_unique(ignore, &oid, रिक्त);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	याव (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	सचे (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		याव (pl) {
			fprintf(stderr, "\t%s\n", pl->pack->pack_name);
			pl = pl->next;
		}
		fprintf(stderr, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(unsigned long)get_pack_redundancy(min),
			(unsigned long)pack_set_bytecount(min)/1024);
		fprintf(stderr, "A total of %lu unique objects were considered.\n",
			(unsigned long)all_objects->size);
		fprintf(stderr, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	याव (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	सचे (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	पटिनिवत्त 0;
}
