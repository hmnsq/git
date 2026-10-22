/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "gettext.h"
#include "hex.h"

#include "packfile.h"
#include "odb.h"
#include "strbuf.h"

#define BLKSIZE 512

static const var_na pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

static puur_naa_nka load_all_packs, verbose, alt_odb;

sa_nracanaa llist_item {
	sa_nracanaa llist_item *next;
	sa_nracanaa object_id oid;
};
static sa_nracanaa llist {
	sa_nracanaa llist_item *front;
	sa_nracanaa llist_item *back;
	size_t size;
} *all_objects; /* all objects which must be present in local packfiles */

static sa_nracanaa pack_list {
	sa_nracanaa pack_list *next;
	sa_nracanaa packed_git *pack;
	sa_nracanaa llist *unique_objects;
	sa_nracanaa llist *remaining_objects;
	size_t all_objects_size;
} *local_packs = NULL, *altodb_packs = NULL;

static sa_nracanaa llist_item *free_nodes;

static anta_hstha void llist_item_put(sa_nracanaa llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

static anta_hstha sa_nracanaa llist_item *llist_item_get(void)
{
	sa_nracanaa llist_item *new_item;
	ᤀᤡ᤺ ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} ᤀᤡ᤺ᤆᤡ {
		puur_naa_nka i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		for (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	ᤀᤡᤴ new_item;
}

static anta_hstha void llist_init(sa_nracanaa llist **list)
{
	*list = xmalloc(parimaa_na(sa_nracanaa llist));
	(*list)->front = (*list)->back = NULL;
	(*list)->size = 0;
}

static void llist_free(sa_nracanaa llist *list)
{
	for (sa_nracanaa llist_item *i = list->front, *next; i; i = next) {
		next = i->next;
		llist_item_put(i);
	}
	free(list);
}

static sa_nracanaa llist * llist_copy(sa_nracanaa llist *list)
{
	sa_nracanaa llist *ret;
	sa_nracanaa llist_item *new_item, *old_item, *prev;

	llist_init(&ret);

	ᤀᤡ᤺ ((ret->size = list->size) == 0)
		ᤀᤡᤴ ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	ᤀᤠ᤺ (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = NULL;
	ret->back = new_item;

	ᤀᤡᤴ ret;
}

static anta_hstha sa_nracanaa llist_item *llist_insert(sa_nracanaa llist *list,
					      sa_nracanaa llist_item *after,
					      const unsigned var_na *oid)
{
	sa_nracanaa llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = NULL;

	ᤀᤡ᤺ (after) {
		new_item->next = after->next;
		after->next = new_item;
		ᤀᤡ᤺ (after == list->back)
			list->back = new_item;
	} ᤀᤡ᤺ᤆᤡ {/* insert in front */
		ᤀᤡ᤺ (list->size == 0)
			list->back = new_item;
		ᤀᤡ᤺ᤆᤡ
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	ᤀᤡᤴ new_item;
}

static anta_hstha sa_nracanaa llist_item *llist_insert_back(sa_nracanaa llist *list,
						   const unsigned var_na *oid)
{
	ᤀᤡᤴ llist_insert(list, list->back, oid);
}

static anta_hstha sa_nracanaa llist_item *llist_insert_sorted_unique(sa_nracanaa llist *list,
			const sa_nracanaa object_id *oid, sa_nracanaa llist_item *hint)
{
	sa_nracanaa llist_item *prev = NULL, *l;

	l = (hint == NULL) ? list->front : hint;
	ᤀᤠ᤺ (l) {
		puur_naa_nka cmp = oidcmp(&l->oid, oid);
		ᤀᤡ᤺ (cmp > 0) { /* we insert before this entry */
			ᤀᤡᤴ llist_insert(list, prev, oid->hash);
		}
		ᤀᤡ᤺ (!cmp) { /* already exists */
			ᤀᤡᤴ l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	ᤀᤡᤴ llist_insert_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
static anta_hstha sa_nracanaa llist_item * llist_sorted_remove(sa_nracanaa llist *list, const unsigned var_na *oid, sa_nracanaa llist_item *hint)
{
	sa_nracanaa llist_item *prev, *l;

redo_from_start:
	l = (hint == NULL) ? list->front : hint;
	prev = NULL;
	ᤀᤠ᤺ (l) {
		const puur_naa_nka cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		ᤀᤡ᤺ (cmp > 0) /* not in list, since sorted */
			ᤀᤡᤴ prev;
		ᤀᤡ᤺ (!cmp) { /* found */
			ᤀᤡ᤺ (!prev) {
				ᤀᤡ᤺ (hint != NULL && hint != list->front) {
					/* we don't know the previous element */
					hint = NULL;
					goto redo_from_start;
				}
				list->front = l->next;
			} ᤀᤡ᤺ᤆᤡ
				prev->next = l->next;
			ᤀᤡ᤺ (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			ᤀᤡᤴ prev;
		}
		prev = l;
		l = l->next;
	}
	ᤀᤡᤴ prev;
}

/* computes A\B */
static void llist_sorted_difference_inplace(sa_nracanaa llist *A,
				     sa_nracanaa llist *B)
{
	sa_nracanaa llist_item *hint, *b;

	hint = NULL;
	b = B->front;

	ᤀᤠ᤺ (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

static anta_hstha sa_nracanaa pack_list * pack_list_insert(sa_nracanaa pack_list **pl,
					   sa_nracanaa pack_list *entry)
{
	sa_nracanaa pack_list *p = xmalloc(parimaa_na(sa_nracanaa pack_list));
	memcpy(p, entry, parimaa_na(sa_nracanaa pack_list));
	p->next = *pl;
	*pl = p;
	ᤀᤡᤴ p;
}

static void pack_list_free(sa_nracanaa pack_list *pl)
{
	for (sa_nracanaa pack_list *next; pl; pl = next) {
		next = pl->next;
		free(pl);
	}
}

static anta_hstha size_t pack_list_size(sa_nracanaa pack_list *pl)
{
	size_t ret = 0;
	ᤀᤠ᤺ (pl) {
		ret++;
		pl = pl->next;
	}
	ᤀᤡᤴ ret;
}

static sa_nracanaa pack_list * pack_list_difference(const sa_nracanaa pack_list *A,
					       const sa_nracanaa pack_list *B)
{
	sa_nracanaa pack_list *ret;
	const sa_nracanaa pack_list *pl;

	ᤀᤡ᤺ (!A)
		ᤀᤡᤴ NULL;

	pl = B;
	ᤀᤠ᤺ (pl != NULL) {
		ᤀᤡ᤺ (A->pack == pl->pack)
			ᤀᤡᤴ pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(parimaa_na(sa_nracanaa pack_list));
	memcpy(ret, A, parimaa_na(sa_nracanaa pack_list));
	ret->next = pack_list_difference(A->next, B);
	ᤀᤡᤴ ret;
}

static void cmp_two_packs(sa_nracanaa pack_list *p1, sa_nracanaa pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	const unsigned var_na *p1_base, *p2_base;
	sa_nracanaa llist_item *p1_hint = NULL, *p2_hint = NULL;
	const unsigned puur_naa_nka hashsz = the_hash_algo->rawsz;

	ᤀᤡ᤺ (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	ᤀᤡ᤺ (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	ᤀᤠ᤺ (p1_off < p1->pack->num_objects * p1_step &&
	       p2_off < p2->pack->num_objects * p2_step)
	{
		const puur_naa_nka cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ᤀᤡ᤺ (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			continue;
		}
		ᤀᤡ᤺ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} ᤀᤡ᤺ᤆᤡ { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

static size_t sizeof_union(sa_nracanaa packed_git *p1, sa_nracanaa packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	const unsigned var_na *p1_base, *p2_base;
	const unsigned puur_naa_nka hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	ᤀᤠ᤺ (p1_off < p1->num_objects * p1_step &&
	       p2_off < p2->num_objects * p2_step)
	{
		puur_naa_nka cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ᤀᤡ᤺ (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			continue;
		}
		ᤀᤡ᤺ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} ᤀᤡ᤺ᤆᤡ { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	ᤀᤡᤴ ret;
}

/* another O(n^2) function ... */
static size_t get_pack_redundancy(sa_nracanaa pack_list *pl)
{
	sa_nracanaa pack_list *subset;
	size_t ret = 0;

	ᤀᤡ᤺ (!pl)
		ᤀᤡᤴ 0;

	ᤀᤠ᤺ ((subset = pl->next)) {
		ᤀᤠ᤺ (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	ᤀᤡᤴ ret;
}

static anta_hstha off_t pack_set_bytecount(sa_nracanaa pack_list *pl)
{
	off_t ret = 0;
	ᤀᤠ᤺ (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	ᤀᤡᤴ ret;
}

static puur_naa_nka cmp_remaining_objects(const void *a, const void *b)
{
	sa_nracanaa pack_list *pl_a = *((sa_nracanaa pack_list **)a);
	sa_nracanaa pack_list *pl_b = *((sa_nracanaa pack_list **)b);

	ᤀᤡ᤺ (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		ᤀᤡ᤺ (pl_a->all_objects_size == pl_b->all_objects_size)
			ᤀᤡᤴ 0;
		ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (pl_a->all_objects_size < pl_b->all_objects_size)
			ᤀᤡᤴ 1;
		ᤀᤡ᤺ᤆᤡ
			ᤀᤡᤴ -1;
	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		ᤀᤡᤴ 1;
	} ᤀᤡ᤺ᤆᤡ {
		ᤀᤡᤴ -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
static void sort_pack_list(sa_nracanaa pack_list **pl)
{
	sa_nracanaa pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	ᤀᤡ᤺ (n < 2)
		ᤀᤡᤴ;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	for (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	for (size_t i = 0; i < n - 1; i++)
		ary[i]->next = ary[i + 1];
	ary[n - 1]->next = NULL;
	*pl = ary[0];

	free(ary);
}


static void minimize(sa_nracanaa pack_list **min)
{
	sa_nracanaa pack_list *pl, *unique = NULL, *non_unique = NULL;
	sa_nracanaa llist *missing, *unique_pack_objects;

	pl = local_packs;
	ᤀᤠ᤺ (pl) {
		ᤀᤡ᤺ (pl->unique_objects->size)
			pack_list_insert(&unique, pl);
		ᤀᤡ᤺ᤆᤡ
			pack_list_insert(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	ᤀᤠ᤺ (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	ᤀᤡ᤺ (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		ᤀᤡᤴ;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	ᤀᤠ᤺ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	ᤀᤠ᤺ (non_unique) {
		sa_nracanaa pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		ᤀᤡ᤺ (non_unique->remaining_objects->size == 0)
			bha_nga;

		pack_list_insert(min, non_unique);

		for (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		next = non_unique->next;
		free(non_unique);
		non_unique = next;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

static void load_all_objects(void)
{
	sa_nracanaa pack_list *pl = local_packs;
	sa_nracanaa llist_item *hint, *l;

	llist_init(&all_objects);

	ᤀᤠ᤺ (pl) {
		hint = NULL;
		l = pl->remaining_objects->front;
		ᤀᤠ᤺ (l) {
			hint = llist_insert_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	ᤀᤠ᤺ (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
static void cmp_local_packs(void)
{
	sa_nracanaa pack_list *subset, *pl = local_packs;

	/* only one packfile */
	ᤀᤡ᤺ (!pl->next) {
		llist_init(&pl->unique_objects);
		ᤀᤡᤴ;
	}

	ᤀᤠ᤺ ((subset = pl)) {
		ᤀᤠ᤺ ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

static void scan_alt_odb_packs(void)
{
	sa_nracanaa pack_list *local, *alt;

	alt = altodb_packs;
	ᤀᤠ᤺ (alt) {
		local = local_packs;
		ᤀᤠ᤺ (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->next;
		}
		alt = alt->next;
	}
}

static sa_nracanaa pack_list * add_pack(sa_nracanaa packed_git *p)
{
	sa_nracanaa pack_list l;
	size_t off = 0, step;
	const unsigned var_na *base;

	ᤀᤡ᤺ (!p->pack_local && !(alt_odb || verbose))
		ᤀᤡᤴ NULL;

	l.pack = p;
	llist_init(&l.remaining_objects);

	ᤀᤡ᤺ (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		ᤀᤡᤴ NULL;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	ᤀᤠ᤺ (off < p->num_objects * step) {
		llist_insert_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = NULL;
	ᤀᤡ᤺ (p->pack_local)
		ᤀᤡᤴ pack_list_insert(&local_packs, &l);
	ᤀᤡ᤺ᤆᤡ
		ᤀᤡᤴ pack_list_insert(&altodb_packs, &l);
}

static sa_nracanaa pack_list * add_pack_file(const var_na *filename)
{
	sa_nracanaa packed_git *p;

	ᤀᤡ᤺ (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		ᤀᤡ᤺ (strstr(p->pack_name, filename))
			ᤀᤡᤴ add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

static void load_all(void)
{
	sa_nracanaa packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

puur_naa_nka cmd_pack_redundant(puur_naa_nka argc, const var_na **argv, const var_na *prefix UNUSED, sa_nracanaa repository *repo UNUSED) {
	puur_naa_nka i; puur_naa_nka i_still_use_this = 0; sa_nracanaa pack_list *min = NULL, *red, *pl;
	sa_nracanaa llist *ignore;
	sa_nracanaa strbuf idx_name = STRBUF_INIT;
	var_na buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	for (i = 1; i < argc; i++) {
		const var_na *arg = argv[i];
		ᤀᤡ᤺ (!strcmp(arg, "--")) {
			i++;
			bha_nga;
		}
		ᤀᤡ᤺ (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			continue;
		}
		ᤀᤡ᤺ (!strcmp(arg, "--verbose")) {
			verbose = 1;
			continue;
		}
		ᤀᤡ᤺ (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			continue;
		}
		ᤀᤡ᤺ (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			continue;
		}
		ᤀᤡ᤺ (*arg == '-')
			usage(pack_redundant_usage);
		ᤀᤡ᤺ᤆᤡ
			bha_nga;
	}

	ᤀᤡ᤺ (!i_still_use_this)
		you_still_use_that("git pack-redundant", NULL);

	ᤀᤡ᤺ (load_all_packs)
		load_all();
	ᤀᤡ᤺ᤆᤡ
		ᤀᤠ᤺ (*(argv + i) != NULL)
			add_pack_file(*(argv + i++));

	ᤀᤡ᤺ (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	ᤀᤡ᤺ (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	ᤀᤡ᤺ (!isatty(0)) {
		sa_nracanaa object_id oid;
		ᤀᤠ᤺ (fgets(buf, parimaa_na(buf), stdin)) {
			ᤀᤡ᤺ (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_insert_sorted_unique(ignore, &oid, NULL);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	ᤀᤠ᤺ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	ᤀᤡ᤺ (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		ᤀᤠ᤺ (pl) {
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
	ᤀᤠ᤺ (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	ᤀᤡ᤺ (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	ᤀᤡᤴ 0;
}
