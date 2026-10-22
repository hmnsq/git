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

static 정수 load_all_packs, verbose, alt_odb;

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
	만약 ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} 아니면 {
		정수 i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		위해 (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	반환 new_item;
}

static inline void llist_init(struct llist **list)
{
	*list = xmalloc(sizeof(struct llist));
	(*list)->front = (*list)->back = NULL;
	(*list)->size = 0;
}

static void llist_free(struct llist *list)
{
	위해 (struct llist_item *i = list->front, *next; i; i = next) {
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

	만약 ((ret->size = list->size) == 0)
		반환 ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	동안 (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = NULL;
	ret->back = new_item;

	반환 ret;
}

static inline struct llist_item *llist_insert(struct llist *list,
					      struct llist_item *after,
					      const unsigned char *oid)
{
	struct llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = NULL;

	만약 (after) {
		new_item->next = after->next;
		after->next = new_item;
		만약 (after == list->back)
			list->back = new_item;
	} 아니면 {/* insert in front */
		만약 (list->size == 0)
			list->back = new_item;
		아니면
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	반환 new_item;
}

static inline struct llist_item *llist_insert_back(struct llist *list,
						   const unsigned char *oid)
{
	반환 llist_insert(list, list->back, oid);
}

static inline struct llist_item *llist_insert_sorted_unique(struct llist *list,
			const struct object_id *oid, struct llist_item *hint)
{
	struct llist_item *prev = NULL, *l;

	l = (hint == NULL) ? list->front : hint;
	동안 (l) {
		정수 cmp = oidcmp(&l->oid, oid);
		만약 (cmp > 0) { /* we insert before this entry */
			반환 llist_insert(list, prev, oid->hash);
		}
		만약 (!cmp) { /* already exists */
			반환 l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	반환 llist_insert_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
static inline struct llist_item * llist_sorted_remove(struct llist *list, const unsigned char *oid, struct llist_item *hint)
{
	struct llist_item *prev, *l;

redo_from_start:
	l = (hint == NULL) ? list->front : hint;
	prev = NULL;
	동안 (l) {
		const 정수 cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		만약 (cmp > 0) /* not in list, since sorted */
			반환 prev;
		만약 (!cmp) { /* found */
			만약 (!prev) {
				만약 (hint != NULL && hint != list->front) {
					/* we don't know the previous element */
					hint = NULL;
					goto redo_from_start;
				}
				list->front = l->next;
			} 아니면
				prev->next = l->next;
			만약 (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			반환 prev;
		}
		prev = l;
		l = l->next;
	}
	반환 prev;
}

/* computes A\B */
static void llist_sorted_difference_inplace(struct llist *A,
				     struct llist *B)
{
	struct llist_item *hint, *b;

	hint = NULL;
	b = B->front;

	동안 (b) {
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
	반환 p;
}

static void pack_list_free(struct pack_list *pl)
{
	위해 (struct pack_list *next; pl; pl = next) {
		next = pl->next;
		free(pl);
	}
}

static inline size_t pack_list_size(struct pack_list *pl)
{
	size_t ret = 0;
	동안 (pl) {
		ret++;
		pl = pl->next;
	}
	반환 ret;
}

static struct pack_list * pack_list_difference(const struct pack_list *A,
					       const struct pack_list *B)
{
	struct pack_list *ret;
	const struct pack_list *pl;

	만약 (!A)
		반환 NULL;

	pl = B;
	동안 (pl != NULL) {
		만약 (A->pack == pl->pack)
			반환 pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(sizeof(struct pack_list));
	memcpy(ret, A, sizeof(struct pack_list));
	ret->next = pack_list_difference(A->next, B);
	반환 ret;
}

static void cmp_two_packs(struct pack_list *p1, struct pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	const unsigned char *p1_base, *p2_base;
	struct llist_item *p1_hint = NULL, *p2_hint = NULL;
	const unsigned 정수 hashsz = the_hash_algo->rawsz;

	만약 (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	만약 (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	동안 (p1_off < p1->pack->num_objects * p1_step &&
	       p2_off < p2->pack->num_objects * p2_step)
	{
		const 정수 cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		만약 (cmp == 0) {
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
		만약 (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} 아니면 { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

static size_t sizeof_union(struct packed_git *p1, struct packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	const unsigned char *p1_base, *p2_base;
	const unsigned 정수 hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	동안 (p1_off < p1->num_objects * p1_step &&
	       p2_off < p2->num_objects * p2_step)
	{
		정수 cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		만약 (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			continue;
		}
		만약 (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} 아니면 { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	반환 ret;
}

/* another O(n^2) function ... */
static size_t get_pack_redundancy(struct pack_list *pl)
{
	struct pack_list *subset;
	size_t ret = 0;

	만약 (!pl)
		반환 0;

	동안 ((subset = pl->next)) {
		동안 (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	반환 ret;
}

static inline off_t pack_set_bytecount(struct pack_list *pl)
{
	off_t ret = 0;
	동안 (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	반환 ret;
}

static 정수 cmp_remaining_objects(const void *a, const void *b)
{
	struct pack_list *pl_a = *((struct pack_list **)a);
	struct pack_list *pl_b = *((struct pack_list **)b);

	만약 (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		만약 (pl_a->all_objects_size == pl_b->all_objects_size)
			반환 0;
		아니면 만약 (pl_a->all_objects_size < pl_b->all_objects_size)
			반환 1;
		아니면
			반환 -1;
	} 아니면 만약 (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		반환 1;
	} 아니면 {
		반환 -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
static void sort_pack_list(struct pack_list **pl)
{
	struct pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	만약 (n < 2)
		반환;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	위해 (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	위해 (size_t i = 0; i < n - 1; i++)
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
	동안 (pl) {
		만약 (pl->unique_objects->size)
			pack_list_insert(&unique, pl);
		아니면
			pack_list_insert(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	동안 (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	만약 (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		반환;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	동안 (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	동안 (non_unique) {
		struct pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		만약 (non_unique->remaining_objects->size == 0)
			break;

		pack_list_insert(min, non_unique);

		위해 (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
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

	동안 (pl) {
		hint = NULL;
		l = pl->remaining_objects->front;
		동안 (l) {
			hint = llist_insert_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	동안 (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
static void cmp_local_packs(void)
{
	struct pack_list *subset, *pl = local_packs;

	/* only one packfile */
	만약 (!pl->next) {
		llist_init(&pl->unique_objects);
		반환;
	}

	동안 ((subset = pl)) {
		동안 ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

static void scan_alt_odb_packs(void)
{
	struct pack_list *local, *alt;

	alt = altodb_packs;
	동안 (alt) {
		local = local_packs;
		동안 (local) {
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

	만약 (!p->pack_local && !(alt_odb || verbose))
		반환 NULL;

	l.pack = p;
	llist_init(&l.remaining_objects);

	만약 (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		반환 NULL;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	동안 (off < p->num_objects * step) {
		llist_insert_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = NULL;
	만약 (p->pack_local)
		반환 pack_list_insert(&local_packs, &l);
	아니면
		반환 pack_list_insert(&altodb_packs, &l);
}

static struct pack_list * add_pack_file(const char *filename)
{
	struct packed_git *p;

	만약 (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		만약 (strstr(p->pack_name, filename))
			반환 add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

static void load_all(void)
{
	struct packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

정수 cmd_pack_redundant(정수 argc, const char **argv, const char *prefix UNUSED, struct repository *repo UNUSED) {
	정수 i; 정수 i_still_use_this = 0; struct pack_list *min = NULL, *red, *pl;
	struct llist *ignore;
	struct strbuf idx_name = STRBUF_INIT;
	char buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	위해 (i = 1; i < argc; i++) {
		const char *arg = argv[i];
		만약 (!strcmp(arg, "--")) {
			i++;
			break;
		}
		만약 (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			continue;
		}
		만약 (!strcmp(arg, "--verbose")) {
			verbose = 1;
			continue;
		}
		만약 (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			continue;
		}
		만약 (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			continue;
		}
		만약 (*arg == '-')
			usage(pack_redundant_usage);
		아니면
			break;
	}

	만약 (!i_still_use_this)
		you_still_use_that("git pack-redundant", NULL);

	만약 (load_all_packs)
		load_all();
	아니면
		동안 (*(argv + i) != NULL)
			add_pack_file(*(argv + i++));

	만약 (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	만약 (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	만약 (!isatty(0)) {
		struct object_id oid;
		동안 (fgets(buf, sizeof(buf), stdin)) {
			만약 (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_insert_sorted_unique(ignore, &oid, NULL);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	동안 (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	만약 (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		동안 (pl) {
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
	동안 (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	만약 (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	반환 0;
}
