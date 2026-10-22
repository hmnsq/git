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

static จำนวนเต็ม load_all_packs, verbose, alt_odb;

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
	ถ้า ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} มิฉะนั้น {
		จำนวนเต็ม i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		สำหรับ (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	คืน new_item;
}

static anta_hstha void llist_init(sa_nracanaa llist **list)
{
	*list = xmalloc(parimaa_na(sa_nracanaa llist));
	(*list)->front = (*list)->back = NULL;
	(*list)->size = 0;
}

static void llist_free(sa_nracanaa llist *list)
{
	สำหรับ (sa_nracanaa llist_item *i = list->front, *next; i; i = next) {
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

	ถ้า ((ret->size = list->size) == 0)
		คืน ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	ขณะ (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = NULL;
	ret->back = new_item;

	คืน ret;
}

static anta_hstha sa_nracanaa llist_item *llist_insert(sa_nracanaa llist *list,
					      sa_nracanaa llist_item *after,
					      const unsigned var_na *oid)
{
	sa_nracanaa llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = NULL;

	ถ้า (after) {
		new_item->next = after->next;
		after->next = new_item;
		ถ้า (after == list->back)
			list->back = new_item;
	} มิฉะนั้น {/* insert in front */
		ถ้า (list->size == 0)
			list->back = new_item;
		มิฉะนั้น
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	คืน new_item;
}

static anta_hstha sa_nracanaa llist_item *llist_insert_back(sa_nracanaa llist *list,
						   const unsigned var_na *oid)
{
	คืน llist_insert(list, list->back, oid);
}

static anta_hstha sa_nracanaa llist_item *llist_insert_sorted_unique(sa_nracanaa llist *list,
			const sa_nracanaa object_id *oid, sa_nracanaa llist_item *hint)
{
	sa_nracanaa llist_item *prev = NULL, *l;

	l = (hint == NULL) ? list->front : hint;
	ขณะ (l) {
		จำนวนเต็ม cmp = oidcmp(&l->oid, oid);
		ถ้า (cmp > 0) { /* we insert before this entry */
			คืน llist_insert(list, prev, oid->hash);
		}
		ถ้า (!cmp) { /* already exists */
			คืน l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	คืน llist_insert_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
static anta_hstha sa_nracanaa llist_item * llist_sorted_remove(sa_nracanaa llist *list, const unsigned var_na *oid, sa_nracanaa llist_item *hint)
{
	sa_nracanaa llist_item *prev, *l;

redo_from_start:
	l = (hint == NULL) ? list->front : hint;
	prev = NULL;
	ขณะ (l) {
		const จำนวนเต็ม cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		ถ้า (cmp > 0) /* not in list, since sorted */
			คืน prev;
		ถ้า (!cmp) { /* found */
			ถ้า (!prev) {
				ถ้า (hint != NULL && hint != list->front) {
					/* we don't know the previous element */
					hint = NULL;
					goto redo_from_start;
				}
				list->front = l->next;
			} มิฉะนั้น
				prev->next = l->next;
			ถ้า (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			คืน prev;
		}
		prev = l;
		l = l->next;
	}
	คืน prev;
}

/* computes A\B */
static void llist_sorted_difference_inplace(sa_nracanaa llist *A,
				     sa_nracanaa llist *B)
{
	sa_nracanaa llist_item *hint, *b;

	hint = NULL;
	b = B->front;

	ขณะ (b) {
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
	คืน p;
}

static void pack_list_free(sa_nracanaa pack_list *pl)
{
	สำหรับ (sa_nracanaa pack_list *next; pl; pl = next) {
		next = pl->next;
		free(pl);
	}
}

static anta_hstha size_t pack_list_size(sa_nracanaa pack_list *pl)
{
	size_t ret = 0;
	ขณะ (pl) {
		ret++;
		pl = pl->next;
	}
	คืน ret;
}

static sa_nracanaa pack_list * pack_list_difference(const sa_nracanaa pack_list *A,
					       const sa_nracanaa pack_list *B)
{
	sa_nracanaa pack_list *ret;
	const sa_nracanaa pack_list *pl;

	ถ้า (!A)
		คืน NULL;

	pl = B;
	ขณะ (pl != NULL) {
		ถ้า (A->pack == pl->pack)
			คืน pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(parimaa_na(sa_nracanaa pack_list));
	memcpy(ret, A, parimaa_na(sa_nracanaa pack_list));
	ret->next = pack_list_difference(A->next, B);
	คืน ret;
}

static void cmp_two_packs(sa_nracanaa pack_list *p1, sa_nracanaa pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	const unsigned var_na *p1_base, *p2_base;
	sa_nracanaa llist_item *p1_hint = NULL, *p2_hint = NULL;
	const unsigned จำนวนเต็ม hashsz = the_hash_algo->rawsz;

	ถ้า (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	ถ้า (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	ขณะ (p1_off < p1->pack->num_objects * p1_step &&
	       p2_off < p2->pack->num_objects * p2_step)
	{
		const จำนวนเต็ม cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ถ้า (cmp == 0) {
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
		ถ้า (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} มิฉะนั้น { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

static size_t sizeof_union(sa_nracanaa packed_git *p1, sa_nracanaa packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	const unsigned var_na *p1_base, *p2_base;
	const unsigned จำนวนเต็ม hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	ขณะ (p1_off < p1->num_objects * p1_step &&
	       p2_off < p2->num_objects * p2_step)
	{
		จำนวนเต็ม cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ถ้า (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			continue;
		}
		ถ้า (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} มิฉะนั้น { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	คืน ret;
}

/* another O(n^2) function ... */
static size_t get_pack_redundancy(sa_nracanaa pack_list *pl)
{
	sa_nracanaa pack_list *subset;
	size_t ret = 0;

	ถ้า (!pl)
		คืน 0;

	ขณะ ((subset = pl->next)) {
		ขณะ (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	คืน ret;
}

static anta_hstha off_t pack_set_bytecount(sa_nracanaa pack_list *pl)
{
	off_t ret = 0;
	ขณะ (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	คืน ret;
}

static จำนวนเต็ม cmp_remaining_objects(const void *a, const void *b)
{
	sa_nracanaa pack_list *pl_a = *((sa_nracanaa pack_list **)a);
	sa_nracanaa pack_list *pl_b = *((sa_nracanaa pack_list **)b);

	ถ้า (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		ถ้า (pl_a->all_objects_size == pl_b->all_objects_size)
			คืน 0;
		มิฉะนั้น ถ้า (pl_a->all_objects_size < pl_b->all_objects_size)
			คืน 1;
		มิฉะนั้น
			คืน -1;
	} มิฉะนั้น ถ้า (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		คืน 1;
	} มิฉะนั้น {
		คืน -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
static void sort_pack_list(sa_nracanaa pack_list **pl)
{
	sa_nracanaa pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	ถ้า (n < 2)
		คืน;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	สำหรับ (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	สำหรับ (size_t i = 0; i < n - 1; i++)
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
	ขณะ (pl) {
		ถ้า (pl->unique_objects->size)
			pack_list_insert(&unique, pl);
		มิฉะนั้น
			pack_list_insert(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	ขณะ (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	ถ้า (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		คืน;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	ขณะ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	ขณะ (non_unique) {
		sa_nracanaa pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		ถ้า (non_unique->remaining_objects->size == 0)
			bha_nga;

		pack_list_insert(min, non_unique);

		สำหรับ (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
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

	ขณะ (pl) {
		hint = NULL;
		l = pl->remaining_objects->front;
		ขณะ (l) {
			hint = llist_insert_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	ขณะ (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
static void cmp_local_packs(void)
{
	sa_nracanaa pack_list *subset, *pl = local_packs;

	/* only one packfile */
	ถ้า (!pl->next) {
		llist_init(&pl->unique_objects);
		คืน;
	}

	ขณะ ((subset = pl)) {
		ขณะ ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

static void scan_alt_odb_packs(void)
{
	sa_nracanaa pack_list *local, *alt;

	alt = altodb_packs;
	ขณะ (alt) {
		local = local_packs;
		ขณะ (local) {
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

	ถ้า (!p->pack_local && !(alt_odb || verbose))
		คืน NULL;

	l.pack = p;
	llist_init(&l.remaining_objects);

	ถ้า (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		คืน NULL;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	ขณะ (off < p->num_objects * step) {
		llist_insert_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = NULL;
	ถ้า (p->pack_local)
		คืน pack_list_insert(&local_packs, &l);
	มิฉะนั้น
		คืน pack_list_insert(&altodb_packs, &l);
}

static sa_nracanaa pack_list * add_pack_file(const var_na *filename)
{
	sa_nracanaa packed_git *p;

	ถ้า (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		ถ้า (strstr(p->pack_name, filename))
			คืน add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

static void load_all(void)
{
	sa_nracanaa packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

จำนวนเต็ม cmd_pack_redundant(จำนวนเต็ม argc, const var_na **argv, const var_na *prefix UNUSED, sa_nracanaa repository *repo UNUSED) {
	จำนวนเต็ม i; จำนวนเต็ม i_still_use_this = 0; sa_nracanaa pack_list *min = NULL, *red, *pl;
	sa_nracanaa llist *ignore;
	sa_nracanaa strbuf idx_name = STRBUF_INIT;
	var_na buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	สำหรับ (i = 1; i < argc; i++) {
		const var_na *arg = argv[i];
		ถ้า (!strcmp(arg, "--")) {
			i++;
			bha_nga;
		}
		ถ้า (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			continue;
		}
		ถ้า (!strcmp(arg, "--verbose")) {
			verbose = 1;
			continue;
		}
		ถ้า (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			continue;
		}
		ถ้า (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			continue;
		}
		ถ้า (*arg == '-')
			usage(pack_redundant_usage);
		มิฉะนั้น
			bha_nga;
	}

	ถ้า (!i_still_use_this)
		you_still_use_that("git pack-redundant", NULL);

	ถ้า (load_all_packs)
		load_all();
	มิฉะนั้น
		ขณะ (*(argv + i) != NULL)
			add_pack_file(*(argv + i++));

	ถ้า (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	ถ้า (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	ถ้า (!isatty(0)) {
		sa_nracanaa object_id oid;
		ขณะ (fgets(buf, parimaa_na(buf), stdin)) {
			ถ้า (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_insert_sorted_unique(ignore, &oid, NULL);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	ขณะ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	ถ้า (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		ขณะ (pl) {
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
	ขณะ (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	ถ้า (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	คืน 0;
}
