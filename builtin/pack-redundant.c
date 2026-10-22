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

static const char pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

static цяло load_all_packs, verbose, alt_odb;

struct llist_item {
	struct llist_item *next;
	struct object_id oid;
};
static struct llist {
	struct llist_item *front;
	struct llist_item *back;
	size_t size;
} *all_objects; /* all objects which must be present in local packfiles */

static struct pack_list {
	struct pack_list *next;
	struct packed_git *pack;
	struct llist *unique_objects;
	struct llist *remaining_objects;
	size_t all_objects_size;
} *local_packs = NULL, *altodb_packs = NULL;

static struct llist_item *free_nodes;

static inline void llist_item_put(struct llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

static inline struct llist_item *llist_item_get(void)
{
	struct llist_item *new_item;
	ако ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} иначе {
		цяло i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		за (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	върни new_item;
}

static inline void llist_init(struct llist **list)
{
	*list = xmalloc(sizeof(struct llist));
	(*list)->front = (*list)->back = NULL;
	(*list)->size = 0;
}

static void llist_free(struct llist *list)
{
	за (struct llist_item *i = list->front, *next; i; i = next) {
		next = i->next;
		llist_item_put(i);
	}
	free(list);
}

static struct llist * llist_copy(struct llist *list)
{
	struct llist *ret;
	struct llist_item *new_item, *old_item, *prev;

	llist_init(&ret);

	ако ((ret->size = list->size) == 0)
		върни ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	докато (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = NULL;
	ret->back = new_item;

	върни ret;
}

static inline struct llist_item *llist_insert(struct llist *list,
					      struct llist_item *after,
					      const unsigned char *oid)
{
	struct llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = NULL;

	ако (after) {
		new_item->next = after->next;
		after->next = new_item;
		ако (after == list->back)
			list->back = new_item;
	} иначе {/* insert in front */
		ако (list->size == 0)
			list->back = new_item;
		иначе
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	върни new_item;
}

static inline struct llist_item *llist_insert_back(struct llist *list,
						   const unsigned char *oid)
{
	върни llist_insert(list, list->back, oid);
}

static inline struct llist_item *llist_insert_sorted_unique(struct llist *list,
			const struct object_id *oid, struct llist_item *hint)
{
	struct llist_item *prev = NULL, *l;

	l = (hint == NULL) ? list->front : hint;
	докато (l) {
		цяло cmp = oidcmp(&l->oid, oid);
		ако (cmp > 0) { /* we insert before this entry */
			върни llist_insert(list, prev, oid->hash);
		}
		ако (!cmp) { /* already exists */
			върни l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	върни llist_insert_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
static inline struct llist_item * llist_sorted_remove(struct llist *list, const unsigned char *oid, struct llist_item *hint)
{
	struct llist_item *prev, *l;

redo_from_start:
	l = (hint == NULL) ? list->front : hint;
	prev = NULL;
	докато (l) {
		const цяло cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		ако (cmp > 0) /* not in list, since sorted */
			върни prev;
		ако (!cmp) { /* found */
			ако (!prev) {
				ако (hint != NULL && hint != list->front) {
					/* we don't know the previous element */
					hint = NULL;
					goto redo_from_start;
				}
				list->front = l->next;
			} иначе
				prev->next = l->next;
			ако (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			върни prev;
		}
		prev = l;
		l = l->next;
	}
	върни prev;
}

/* computes A\B */
static void llist_sorted_difference_inplace(struct llist *A,
				     struct llist *B)
{
	struct llist_item *hint, *b;

	hint = NULL;
	b = B->front;

	докато (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

static inline struct pack_list * pack_list_insert(struct pack_list **pl,
					   struct pack_list *entry)
{
	struct pack_list *p = xmalloc(sizeof(struct pack_list));
	memcpy(p, entry, sizeof(struct pack_list));
	p->next = *pl;
	*pl = p;
	върни p;
}

static void pack_list_free(struct pack_list *pl)
{
	за (struct pack_list *next; pl; pl = next) {
		next = pl->next;
		free(pl);
	}
}

static inline size_t pack_list_size(struct pack_list *pl)
{
	size_t ret = 0;
	докато (pl) {
		ret++;
		pl = pl->next;
	}
	върни ret;
}

static struct pack_list * pack_list_difference(const struct pack_list *A,
					       const struct pack_list *B)
{
	struct pack_list *ret;
	const struct pack_list *pl;

	ако (!A)
		върни NULL;

	pl = B;
	докато (pl != NULL) {
		ако (A->pack == pl->pack)
			върни pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(sizeof(struct pack_list));
	memcpy(ret, A, sizeof(struct pack_list));
	ret->next = pack_list_difference(A->next, B);
	върни ret;
}

static void cmp_two_packs(struct pack_list *p1, struct pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	const unsigned char *p1_base, *p2_base;
	struct llist_item *p1_hint = NULL, *p2_hint = NULL;
	const unsigned цяло hashsz = the_hash_algo->rawsz;

	ако (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	ако (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	докато (p1_off < p1->pack->num_objects * p1_step &&
	       p2_off < p2->pack->num_objects * p2_step)
	{
		const цяло cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ако (cmp == 0) {
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
		ако (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} иначе { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

static size_t sizeof_union(struct packed_git *p1, struct packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	const unsigned char *p1_base, *p2_base;
	const unsigned цяло hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	докато (p1_off < p1->num_objects * p1_step &&
	       p2_off < p2->num_objects * p2_step)
	{
		цяло cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ако (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			continue;
		}
		ако (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} иначе { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	върни ret;
}

/* another O(n^2) function ... */
static size_t get_pack_redundancy(struct pack_list *pl)
{
	struct pack_list *subset;
	size_t ret = 0;

	ако (!pl)
		върни 0;

	докато ((subset = pl->next)) {
		докато (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	върни ret;
}

static inline off_t pack_set_bytecount(struct pack_list *pl)
{
	off_t ret = 0;
	докато (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	върни ret;
}

static цяло cmp_remaining_objects(const void *a, const void *b)
{
	struct pack_list *pl_a = *((struct pack_list **)a);
	struct pack_list *pl_b = *((struct pack_list **)b);

	ако (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		ако (pl_a->all_objects_size == pl_b->all_objects_size)
			върни 0;
		иначе ако (pl_a->all_objects_size < pl_b->all_objects_size)
			върни 1;
		иначе
			върни -1;
	} иначе ако (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		върни 1;
	} иначе {
		върни -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
static void sort_pack_list(struct pack_list **pl)
{
	struct pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	ако (n < 2)
		върни;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	за (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	за (size_t i = 0; i < n - 1; i++)
		ary[i]->next = ary[i + 1];
	ary[n - 1]->next = NULL;
	*pl = ary[0];

	free(ary);
}


static void minimize(struct pack_list **min)
{
	struct pack_list *pl, *unique = NULL, *non_unique = NULL;
	struct llist *missing, *unique_pack_objects;

	pl = local_packs;
	докато (pl) {
		ако (pl->unique_objects->size)
			pack_list_insert(&unique, pl);
		иначе
			pack_list_insert(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	докато (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	ако (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		върни;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	докато (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	докато (non_unique) {
		struct pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		ако (non_unique->remaining_objects->size == 0)
			break;

		pack_list_insert(min, non_unique);

		за (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
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
	struct pack_list *pl = local_packs;
	struct llist_item *hint, *l;

	llist_init(&all_objects);

	докато (pl) {
		hint = NULL;
		l = pl->remaining_objects->front;
		докато (l) {
			hint = llist_insert_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	докато (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
static void cmp_local_packs(void)
{
	struct pack_list *subset, *pl = local_packs;

	/* only one packfile */
	ако (!pl->next) {
		llist_init(&pl->unique_objects);
		върни;
	}

	докато ((subset = pl)) {
		докато ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

static void scan_alt_odb_packs(void)
{
	struct pack_list *local, *alt;

	alt = altodb_packs;
	докато (alt) {
		local = local_packs;
		докато (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->next;
		}
		alt = alt->next;
	}
}

static struct pack_list * add_pack(struct packed_git *p)
{
	struct pack_list l;
	size_t off = 0, step;
	const unsigned char *base;

	ако (!p->pack_local && !(alt_odb || verbose))
		върни NULL;

	l.pack = p;
	llist_init(&l.remaining_objects);

	ако (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		върни NULL;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	докато (off < p->num_objects * step) {
		llist_insert_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = NULL;
	ако (p->pack_local)
		върни pack_list_insert(&local_packs, &l);
	иначе
		върни pack_list_insert(&altodb_packs, &l);
}

static struct pack_list * add_pack_file(const char *filename)
{
	struct packed_git *p;

	ако (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		ако (strstr(p->pack_name, filename))
			върни add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

static void load_all(void)
{
	struct packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

цяло cmd_pack_redundant(цяло argc, const char **argv, const char *prefix UNUSED, struct repository *repo UNUSED) {
	цяло i; цяло i_still_use_this = 0; struct pack_list *min = NULL, *red, *pl;
	struct llist *ignore;
	struct strbuf idx_name = STRBUF_INIT;
	char buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	за (i = 1; i < argc; i++) {
		const char *arg = argv[i];
		ако (!strcmp(arg, "--")) {
			i++;
			break;
		}
		ако (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			continue;
		}
		ако (!strcmp(arg, "--verbose")) {
			verbose = 1;
			continue;
		}
		ако (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			continue;
		}
		ако (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			continue;
		}
		ако (*arg == '-')
			usage(pack_redundant_usage);
		иначе
			break;
	}

	ако (!i_still_use_this)
		you_still_use_that("git pack-redundant", NULL);

	ако (load_all_packs)
		load_all();
	иначе
		докато (*(argv + i) != NULL)
			add_pack_file(*(argv + i++));

	ако (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	ако (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	ако (!isatty(0)) {
		struct object_id oid;
		докато (fgets(buf, sizeof(buf), stdin)) {
			ако (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_insert_sorted_unique(ignore, &oid, NULL);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	докато (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	ако (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		докато (pl) {
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
	докато (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	ако (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	върни 0;
}
