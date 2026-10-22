/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"

#ଅନ୍ତର୍ଭାଵ "packfile.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"

#define BLKSIZE 512

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା load_all_packs, verbose, alt_odb;

ସଂରଚନା llist_item {
	ସଂରଚନା llist_item *next;
	ସଂରଚନା object_id oid;
};
ସ୍ଥିର ସଂରଚନା llist {
	ସଂରଚନା llist_item *front;
	ସଂରଚନା llist_item *back;
	size_t size;
} *all_objects; /* all objects which must be present in local packfiles */

ସ୍ଥିର ସଂରଚନା pack_list {
	ସଂରଚନା pack_list *next;
	ସଂରଚନା packed_git *pack;
	ସଂରଚନା llist *unique_objects;
	ସଂରଚନା llist *remaining_objects;
	size_t all_objects_size;
} *local_packs = ଖାଲି, *altodb_packs = ଖାଲି;

ସ୍ଥିର ସଂରଚନା llist_item *free_nodes;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ llist_item_put(ସଂରଚନା llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା llist_item *llist_item_get(ଶୂନ୍ୟ)
{
	ସଂରଚନା llist_item *new_item;
	ଯଦି ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} ଅନ୍ୟଥା {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		ପାଇଁ (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	ଫେରନ୍ତୁ new_item;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ llist_init(ସଂରଚନା llist **list)
{
	*list = xmalloc(ଆକାର(ସଂରଚନା llist));
	(*list)->front = (*list)->back = ଖାଲି;
	(*list)->size = 0;
}

ସ୍ଥିର ଶୂନ୍ୟ llist_free(ସଂରଚନା llist *list)
{
	ପାଇଁ (ସଂରଚନା llist_item *i = list->front, *next; i; i = next) {
		next = i->next;
		llist_item_put(i);
	}
	ମୁକ୍ତ(list);
}

ସ୍ଥିର ସଂରଚନା llist * llist_copy(ସଂରଚନା llist *list)
{
	ସଂରଚନା llist *ret;
	ସଂରଚନା llist_item *new_item, *old_item, *prev;

	llist_init(&ret);

	ଯଦି ((ret->size = list->size) == 0)
		ଫେରନ୍ତୁ ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	ଯେତେବେଳେ (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = ଖାଲି;
	ret->back = new_item;

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା llist_item *llist_ଭର୍ତ୍ତି(ସଂରଚନା llist *list,
					      ସଂରଚନା llist_item *after,
					      ଅଚଳ unsigned ଅକ୍ଷର *oid)
{
	ସଂରଚନା llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = ଖାଲି;

	ଯଦି (after) {
		new_item->next = after->next;
		after->next = new_item;
		ଯଦି (after == list->back)
			list->back = new_item;
	} ଅନ୍ୟଥା {/* insert in front */
		ଯଦି (list->size == 0)
			list->back = new_item;
		ଅନ୍ୟଥା
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	ଫେରନ୍ତୁ new_item;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା llist_item *llist_ଭର୍ତ୍ତି_back(ସଂରଚନା llist *list,
						   ଅଚଳ unsigned ଅକ୍ଷର *oid)
{
	ଫେରନ୍ତୁ llist_ଭର୍ତ୍ତି(list, list->back, oid);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା llist_item *llist_ଭର୍ତ୍ତି_sorted_unique(ସଂରଚନା llist *list,
			ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା llist_item *hint)
{
	ସଂରଚନା llist_item *prev = ଖାଲି, *l;

	l = (hint == ଖାଲି) ? list->front : hint;
	ଯେତେବେଳେ (l) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = oidcmp(&l->oid, oid);
		ଯଦି (cmp > 0) { /* we insert before this entry */
			ଫେରନ୍ତୁ llist_ଭର୍ତ୍ତି(list, prev, oid->hash);
		}
		ଯଦି (!cmp) { /* already exists */
			ଫେରନ୍ତୁ l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	ଫେରନ୍ତୁ llist_ଭର୍ତ୍ତି_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା llist_item * llist_sorted_remove(ସଂରଚନା llist *list, ଅଚଳ unsigned ଅକ୍ଷର *oid, ସଂରଚନା llist_item *hint)
{
	ସଂରଚନା llist_item *prev, *l;

redo_from_start:
	l = (hint == ଖାଲି) ? list->front : hint;
	prev = ଖାଲି;
	ଯେତେବେଳେ (l) {
		ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		ଯଦି (cmp > 0) /* not in list, since sorted */
			ଫେରନ୍ତୁ prev;
		ଯଦି (!cmp) { /* found */
			ଯଦି (!prev) {
				ଯଦି (hint != ଖାଲି && hint != list->front) {
					/* we don't know the previous element */
					hint = ଖାଲି;
					ଯାଅ redo_from_start;
				}
				list->front = l->next;
			} ଅନ୍ୟଥା
				prev->next = l->next;
			ଯଦି (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			ଫେରନ୍ତୁ prev;
		}
		prev = l;
		l = l->next;
	}
	ଫେରନ୍ତୁ prev;
}

/* computes A\B */
ସ୍ଥିର ଶୂନ୍ୟ llist_sorted_difference_inplace(ସଂରଚନା llist *A,
				     ସଂରଚନା llist *B)
{
	ସଂରଚନା llist_item *hint, *b;

	hint = ଖାଲି;
	b = B->front;

	ଯେତେବେଳେ (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା pack_list * pack_list_ଭର୍ତ୍ତି(ସଂରଚନା pack_list **pl,
					   ସଂରଚନା pack_list *entry)
{
	ସଂରଚନା pack_list *p = xmalloc(ଆକାର(ସଂରଚନା pack_list));
	memcpy(p, entry, ଆକାର(ସଂରଚନା pack_list));
	p->next = *pl;
	*pl = p;
	ଫେରନ୍ତୁ p;
}

ସ୍ଥିର ଶୂନ୍ୟ pack_list_free(ସଂରଚନା pack_list *pl)
{
	ପାଇଁ (ସଂରଚନା pack_list *next; pl; pl = next) {
		next = pl->next;
		ମୁକ୍ତ(pl);
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t pack_list_size(ସଂରଚନା pack_list *pl)
{
	size_t ret = 0;
	ଯେତେବେଳେ (pl) {
		ret++;
		pl = pl->next;
	}
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ସଂରଚନା pack_list * pack_list_difference(ଅଚଳ ସଂରଚନା pack_list *A,
					       ଅଚଳ ସଂରଚନା pack_list *B)
{
	ସଂରଚନା pack_list *ret;
	ଅଚଳ ସଂରଚନା pack_list *pl;

	ଯଦି (!A)
		ଫେରନ୍ତୁ ଖାଲି;

	pl = B;
	ଯେତେବେଳେ (pl != ଖାଲି) {
		ଯଦି (A->pack == pl->pack)
			ଫେରନ୍ତୁ pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(ଆକାର(ସଂରଚନା pack_list));
	memcpy(ret, A, ଆକାର(ସଂରଚନା pack_list));
	ret->next = pack_list_difference(A->next, B);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ cmp_two_packs(ସଂରଚନା pack_list *p1, ସଂରଚନା pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ଅଚଳ unsigned ଅକ୍ଷର *p1_base, *p2_base;
	ସଂରଚନା llist_item *p1_hint = ଖାଲି, *p2_hint = ଖାଲି;
	ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hashsz = the_hash_algo->rawsz;

	ଯଦି (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	ଯଦି (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	ଯେତେବେଳେ (p1_off < p1->pack->ଅଙ୍କ_objects * p1_step &&
	       p2_off < p2->pack->ଅଙ୍କ_objects * p2_step)
	{
		ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ଯଦି (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			ଜାରି;
		}
		ଯଦି (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} ଅନ୍ୟଥା { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

ସ୍ଥିର size_t sizeof_union(ସଂରଚନା packed_git *p1, ସଂରଚନା packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ଅଚଳ unsigned ଅକ୍ଷର *p1_base, *p2_base;
	ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	ଯେତେବେଳେ (p1_off < p1->ଅଙ୍କ_objects * p1_step &&
	       p2_off < p2->ଅଙ୍କ_objects * p2_step)
	{
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ଯଦି (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			ଜାରି;
		}
		ଯଦି (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} ଅନ୍ୟଥା { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	ଫେରନ୍ତୁ ret;
}

/* another O(n^2) function ... */
ସ୍ଥିର size_t get_pack_redundancy(ସଂରଚନା pack_list *pl)
{
	ସଂରଚନା pack_list *subset;
	size_t ret = 0;

	ଯଦି (!pl)
		ଫେରନ୍ତୁ 0;

	ଯେତେବେଳେ ((subset = pl->next)) {
		ଯେତେବେଳେ (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ off_t pack_set_bytecount(ସଂରଚନା pack_list *pl)
{
	off_t ret = 0;
	ଯେତେବେଳେ (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp_remaining_objects(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
	ସଂରଚନା pack_list *pl_a = *((ସଂରଚନା pack_list **)a);
	ସଂରଚନା pack_list *pl_b = *((ସଂରଚନା pack_list **)b);

	ଯଦି (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		ଯଦି (pl_a->all_objects_size == pl_b->all_objects_size)
			ଫେରନ୍ତୁ 0;
		ଅନ୍ୟଥା ଯଦି (pl_a->all_objects_size < pl_b->all_objects_size)
			ଫେରନ୍ତୁ 1;
		ଅନ୍ୟଥା
			ଫେରନ୍ତୁ -1;
	} ଅନ୍ୟଥା ଯଦି (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		ଫେରନ୍ତୁ 1;
	} ଅନ୍ୟଥା {
		ଫେରନ୍ତୁ -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
ସ୍ଥିର ଶୂନ୍ୟ sort_pack_list(ସଂରଚନା pack_list **pl)
{
	ସଂରଚନା pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	ଯଦି (n < 2)
		ଫେରନ୍ତୁ;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	ପାଇଁ (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	ପାଇଁ (size_t i = 0; i < n - 1; i++)
		ary[i]->next = ary[i + 1];
	ary[n - 1]->next = ଖାଲି;
	*pl = ary[0];

	ମୁକ୍ତ(ary);
}


ସ୍ଥିର ଶୂନ୍ୟ minimize(ସଂରଚନା pack_list **min)
{
	ସଂରଚନା pack_list *pl, *unique = ଖାଲି, *non_unique = ଖାଲି;
	ସଂରଚନା llist *missing, *unique_pack_objects;

	pl = local_packs;
	ଯେତେବେଳେ (pl) {
		ଯଦି (pl->unique_objects->size)
			pack_list_ଭର୍ତ୍ତି(&unique, pl);
		ଅନ୍ୟଥା
			pack_list_ଭର୍ତ୍ତି(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	ଯେତେବେଳେ (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	ଯଦି (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		ଫେରନ୍ତୁ;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	ଯେତେବେଳେ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	ଯେତେବେଳେ (non_unique) {
		ସଂରଚନା pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		ଯଦି (non_unique->remaining_objects->size == 0)
			ଭାଙ୍ଗ;

		pack_list_ଭର୍ତ୍ତି(min, non_unique);

		ପାଇଁ (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		next = non_unique->next;
		ମୁକ୍ତ(non_unique);
		non_unique = next;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

ସ୍ଥିର ଶୂନ୍ୟ load_all_objects(ଶୂନ୍ୟ)
{
	ସଂରଚନା pack_list *pl = local_packs;
	ସଂରଚନା llist_item *hint, *l;

	llist_init(&all_objects);

	ଯେତେବେଳେ (pl) {
		hint = ଖାଲି;
		l = pl->remaining_objects->front;
		ଯେତେବେଳେ (l) {
			hint = llist_ଭର୍ତ୍ତି_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	ଯେତେବେଳେ (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
ସ୍ଥିର ଶୂନ୍ୟ cmp_local_packs(ଶୂନ୍ୟ)
{
	ସଂରଚନା pack_list *subset, *pl = local_packs;

	/* only one packfile */
	ଯଦି (!pl->next) {
		llist_init(&pl->unique_objects);
		ଫେରନ୍ତୁ;
	}

	ଯେତେବେଳେ ((subset = pl)) {
		ଯେତେବେଳେ ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ scan_alt_odb_packs(ଶୂନ୍ୟ)
{
	ସଂରଚନା pack_list *local, *alt;

	alt = altodb_packs;
	ଯେତେବେଳେ (alt) {
		local = local_packs;
		ଯେତେବେଳେ (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->next;
		}
		alt = alt->next;
	}
}

ସ୍ଥିର ସଂରଚନା pack_list * add_pack(ସଂରଚନା packed_git *p)
{
	ସଂରଚନା pack_list l;
	size_t off = 0, step;
	ଅଚଳ unsigned ଅକ୍ଷର *base;

	ଯଦି (!p->pack_local && !(alt_odb || verbose))
		ଫେରନ୍ତୁ ଖାଲି;

	l.pack = p;
	llist_init(&l.remaining_objects);

	ଯଦି (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		ଫେରନ୍ତୁ ଖାଲି;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	ଯେତେବେଳେ (off < p->ଅଙ୍କ_objects * step) {
		llist_ଭର୍ତ୍ତି_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = ଖାଲି;
	ଯଦି (p->pack_local)
		ଫେରନ୍ତୁ pack_list_ଭର୍ତ୍ତି(&local_packs, &l);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ pack_list_ଭର୍ତ୍ତି(&altodb_packs, &l);
}

ସ୍ଥିର ସଂରଚନା pack_list * add_pack_file(ଅଚଳ ଅକ୍ଷର *filename)
{
	ସଂରଚନା packed_git *p;

	ଯଦି (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		ଯଦି (strstr(p->pack_name, filename))
			ଫେରନ୍ତୁ add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

ସ୍ଥିର ଶୂନ୍ୟ load_all(ଶୂନ୍ୟ)
{
	ସଂରଚନା packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_pack_redundant(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix UNUSED, ସଂରଚନା repository *repo UNUSED) {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i; ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_still_use_this = 0; ସଂରଚନା pack_list *min = ଖାଲି, *red, *pl;
	ସଂରଚନା llist *ignore;
	ସଂରଚନା strbuf idx_name = STRBUF_INIT;
	ଅକ୍ଷର buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	ପାଇଁ (i = 1; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];
		ଯଦି (!strcmp(arg, "--")) {
			i++;
			ଭାଙ୍ଗ;
		}
		ଯଦି (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			ଜାରି;
		}
		ଯଦି (!strcmp(arg, "--verbose")) {
			verbose = 1;
			ଜାରି;
		}
		ଯଦି (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			ଜାରି;
		}
		ଯଦି (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			ଜାରି;
		}
		ଯଦି (*arg == '-')
			usage(pack_redundant_usage);
		ଅନ୍ୟଥା
			ଭାଙ୍ଗ;
	}

	ଯଦି (!i_still_use_this)
		you_still_use_that("git pack-redundant", ଖାଲି);

	ଯଦି (load_all_packs)
		load_all();
	ଅନ୍ୟଥା
		ଯେତେବେଳେ (*(argv + i) != ଖାଲି)
			add_pack_file(*(argv + i++));

	ଯଦି (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	ଯଦି (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	ଯଦି (!isatty(0)) {
		ସଂରଚନା object_id oid;
		ଯେତେବେଳେ (fgets(buf, ଆକାର(buf), stdin)) {
			ଯଦି (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_ଭର୍ତ୍ତି_sorted_unique(ignore, &oid, ଖାଲି);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	ଯେତେବେଳେ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	ଯଦି (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		ଯେତେବେଳେ (pl) {
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
	ଯେତେବେଳେ (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	ଯଦି (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	ଫେରନ୍ତୁ 0;
}
