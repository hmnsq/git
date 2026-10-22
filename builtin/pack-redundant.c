/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#define USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"

#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"

#define BLKSIZE 512

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ load_all_packs, verbose, alt_odb;

ಸಂರಚನಾ llist_item {
	ಸಂರಚನಾ llist_item *next;
	ಸಂರಚನಾ object_id oid;
};
ಸ್ಥಿರ ಸಂರಚನಾ llist {
	ಸಂರಚನಾ llist_item *front;
	ಸಂರಚನಾ llist_item *back;
	size_t size;
} *all_objects; /* all objects which must be present in local packfiles */

ಸ್ಥಿರ ಸಂರಚನಾ pack_list {
	ಸಂರಚನಾ pack_list *next;
	ಸಂರಚನಾ packed_git *pack;
	ಸಂರಚನಾ llist *unique_objects;
	ಸಂರಚನಾ llist *remaining_objects;
	size_t all_objects_size;
} *local_packs = ರಿಕ್ತ, *altodb_packs = ರಿಕ್ತ;

ಸ್ಥಿರ ಸಂರಚನಾ llist_item *free_nodes;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ llist_item_put(ಸಂರಚನಾ llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ llist_item *llist_item_get(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ llist_item *new_item;
	ಯದಿ ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} ಅನ್ಯಥಾ {
		ಪೂರ್ಣಾಙ್ಕ i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		ಪ್ರತಿ (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	ಪ್ರತ್ಯಾಗಮ new_item;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ llist_init(ಸಂರಚನಾ llist **list)
{
	*list = xmalloc(ಪರಿಮಾಣ(ಸಂರಚನಾ llist));
	(*list)->front = (*list)->back = ರಿಕ್ತ;
	(*list)->size = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ llist_free(ಸಂರಚನಾ llist *list)
{
	ಪ್ರತಿ (ಸಂರಚನಾ llist_item *i = list->front, *next; i; i = next) {
		next = i->next;
		llist_item_put(i);
	}
	ಮುಕ್ತ(list);
}

ಸ್ಥಿರ ಸಂರಚನಾ llist * llist_copy(ಸಂರಚನಾ llist *list)
{
	ಸಂರಚನಾ llist *ret;
	ಸಂರಚನಾ llist_item *new_item, *old_item, *prev;

	llist_init(&ret);

	ಯದಿ ((ret->size = list->size) == 0)
		ಪ್ರತ್ಯಾಗಮ ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	ಯಾವತ್ (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = ರಿಕ್ತ;
	ret->back = new_item;

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ llist_item *llist_ಸೇರಿಸು(ಸಂರಚನಾ llist *list,
					      ಸಂರಚನಾ llist_item *after,
					      ಧ್ರುವ unsigned ವರ್ಣ *oid)
{
	ಸಂರಚನಾ llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = ರಿಕ್ತ;

	ಯದಿ (after) {
		new_item->next = after->next;
		after->next = new_item;
		ಯದಿ (after == list->back)
			list->back = new_item;
	} ಅನ್ಯಥಾ {/* insert in front */
		ಯದಿ (list->size == 0)
			list->back = new_item;
		ಅನ್ಯಥಾ
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	ಪ್ರತ್ಯಾಗಮ new_item;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ llist_item *llist_ಸೇರಿಸು_back(ಸಂರಚನಾ llist *list,
						   ಧ್ರುವ unsigned ವರ್ಣ *oid)
{
	ಪ್ರತ್ಯಾಗಮ llist_ಸೇರಿಸು(list, list->back, oid);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ llist_item *llist_ಸೇರಿಸು_sorted_unique(ಸಂರಚನಾ llist *list,
			ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಸಂರಚನಾ llist_item *hint)
{
	ಸಂರಚನಾ llist_item *prev = ರಿಕ್ತ, *l;

	l = (hint == ರಿಕ್ತ) ? list->front : hint;
	ಯಾವತ್ (l) {
		ಪೂರ್ಣಾಙ್ಕ cmp = oidcmp(&l->oid, oid);
		ಯದಿ (cmp > 0) { /* we insert before this entry */
			ಪ್ರತ್ಯಾಗಮ llist_ಸೇರಿಸು(list, prev, oid->hash);
		}
		ಯದಿ (!cmp) { /* already exists */
			ಪ್ರತ್ಯಾಗಮ l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	ಪ್ರತ್ಯಾಗಮ llist_ಸೇರಿಸು_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ llist_item * llist_sorted_remove(ಸಂರಚನಾ llist *list, ಧ್ರುವ unsigned ವರ್ಣ *oid, ಸಂರಚನಾ llist_item *hint)
{
	ಸಂರಚನಾ llist_item *prev, *l;

redo_from_start:
	l = (hint == ರಿಕ್ತ) ? list->front : hint;
	prev = ರಿಕ್ತ;
	ಯಾವತ್ (l) {
		ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		ಯದಿ (cmp > 0) /* not in list, since sorted */
			ಪ್ರತ್ಯಾಗಮ prev;
		ಯದಿ (!cmp) { /* found */
			ಯದಿ (!prev) {
				ಯದಿ (hint != ರಿಕ್ತ && hint != list->front) {
					/* we don't know the previous element */
					hint = ರಿಕ್ತ;
					ಗಚ್ಛ redo_from_start;
				}
				list->front = l->next;
			} ಅನ್ಯಥಾ
				prev->next = l->next;
			ಯದಿ (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			ಪ್ರತ್ಯಾಗಮ prev;
		}
		prev = l;
		l = l->next;
	}
	ಪ್ರತ್ಯಾಗಮ prev;
}

/* computes A\B */
ಸ್ಥಿರ ಶೂನ್ಯ llist_sorted_difference_inplace(ಸಂರಚನಾ llist *A,
				     ಸಂರಚನಾ llist *B)
{
	ಸಂರಚನಾ llist_item *hint, *b;

	hint = ರಿಕ್ತ;
	b = B->front;

	ಯಾವತ್ (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ pack_list * pack_list_ಸೇರಿಸು(ಸಂರಚನಾ pack_list **pl,
					   ಸಂರಚನಾ pack_list *entry)
{
	ಸಂರಚನಾ pack_list *p = xmalloc(ಪರಿಮಾಣ(ಸಂರಚನಾ pack_list));
	memcpy(p, entry, ಪರಿಮಾಣ(ಸಂರಚನಾ pack_list));
	p->next = *pl;
	*pl = p;
	ಪ್ರತ್ಯಾಗಮ p;
}

ಸ್ಥಿರ ಶೂನ್ಯ pack_list_free(ಸಂರಚನಾ pack_list *pl)
{
	ಪ್ರತಿ (ಸಂರಚನಾ pack_list *next; pl; pl = next) {
		next = pl->next;
		ಮುಕ್ತ(pl);
	}
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t pack_list_size(ಸಂರಚನಾ pack_list *pl)
{
	size_t ret = 0;
	ಯಾವತ್ (pl) {
		ret++;
		pl = pl->next;
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಸಂರಚನಾ pack_list * pack_list_difference(ಧ್ರುವ ಸಂರಚನಾ pack_list *A,
					       ಧ್ರುವ ಸಂರಚನಾ pack_list *B)
{
	ಸಂರಚನಾ pack_list *ret;
	ಧ್ರುವ ಸಂರಚನಾ pack_list *pl;

	ಯದಿ (!A)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	pl = B;
	ಯಾವತ್ (pl != ರಿಕ್ತ) {
		ಯದಿ (A->pack == pl->pack)
			ಪ್ರತ್ಯಾಗಮ pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(ಪರಿಮಾಣ(ಸಂರಚನಾ pack_list));
	memcpy(ret, A, ಪರಿಮಾಣ(ಸಂರಚನಾ pack_list));
	ret->next = pack_list_difference(A->next, B);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ cmp_two_packs(ಸಂರಚನಾ pack_list *p1, ಸಂರಚನಾ pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ಧ್ರುವ unsigned ವರ್ಣ *p1_base, *p2_base;
	ಸಂರಚನಾ llist_item *p1_hint = ರಿಕ್ತ, *p2_hint = ರಿಕ್ತ;
	ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ hashsz = the_hash_algo->rawsz;

	ಯದಿ (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	ಯದಿ (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	ಯಾವತ್ (p1_off < p1->pack->ಅಂಕೆ_objects * p1_step &&
	       p2_off < p2->pack->ಅಂಕೆ_objects * p2_step)
	{
		ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ಯದಿ (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			ಅನುವರ್ತ;
		}
		ಯದಿ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} ಅನ್ಯಥಾ { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

ಸ್ಥಿರ size_t sizeof_union(ಸಂರಚನಾ packed_git *p1, ಸಂರಚನಾ packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ಧ್ರುವ unsigned ವರ್ಣ *p1_base, *p2_base;
	ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	ಯಾವತ್ (p1_off < p1->ಅಂಕೆ_objects * p1_step &&
	       p2_off < p2->ಅಂಕೆ_objects * p2_step)
	{
		ಪೂರ್ಣಾಙ್ಕ cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ಯದಿ (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			ಅನುವರ್ತ;
		}
		ಯದಿ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} ಅನ್ಯಥಾ { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}

/* another O(n^2) function ... */
ಸ್ಥಿರ size_t get_pack_redundancy(ಸಂರಚನಾ pack_list *pl)
{
	ಸಂರಚನಾ pack_list *subset;
	size_t ret = 0;

	ಯದಿ (!pl)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯಾವತ್ ((subset = pl->next)) {
		ಯಾವತ್ (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ off_t pack_set_bytecount(ಸಂರಚನಾ pack_list *pl)
{
	off_t ret = 0;
	ಯಾವತ್ (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp_remaining_objects(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
	ಸಂರಚನಾ pack_list *pl_a = *((ಸಂರಚನಾ pack_list **)a);
	ಸಂರಚನಾ pack_list *pl_b = *((ಸಂರಚನಾ pack_list **)b);

	ಯದಿ (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		ಯದಿ (pl_a->all_objects_size == pl_b->all_objects_size)
			ಪ್ರತ್ಯಾಗಮ 0;
		ಅನ್ಯಥಾ ಯದಿ (pl_a->all_objects_size < pl_b->all_objects_size)
			ಪ್ರತ್ಯಾಗಮ 1;
		ಅನ್ಯಥಾ
			ಪ್ರತ್ಯಾಗಮ -1;
	} ಅನ್ಯಥಾ ಯದಿ (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		ಪ್ರತ್ಯಾಗಮ 1;
	} ಅನ್ಯಥಾ {
		ಪ್ರತ್ಯಾಗಮ -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
ಸ್ಥಿರ ಶೂನ್ಯ sort_pack_list(ಸಂರಚನಾ pack_list **pl)
{
	ಸಂರಚನಾ pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	ಯದಿ (n < 2)
		ಪ್ರತ್ಯಾಗಮ;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	ಪ್ರತಿ (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	ಪ್ರತಿ (size_t i = 0; i < n - 1; i++)
		ary[i]->next = ary[i + 1];
	ary[n - 1]->next = ರಿಕ್ತ;
	*pl = ary[0];

	ಮುಕ್ತ(ary);
}


ಸ್ಥಿರ ಶೂನ್ಯ minimize(ಸಂರಚನಾ pack_list **min)
{
	ಸಂರಚನಾ pack_list *pl, *unique = ರಿಕ್ತ, *non_unique = ರಿಕ್ತ;
	ಸಂರಚನಾ llist *missing, *unique_pack_objects;

	pl = local_packs;
	ಯಾವತ್ (pl) {
		ಯದಿ (pl->unique_objects->size)
			pack_list_ಸೇರಿಸು(&unique, pl);
		ಅನ್ಯಥಾ
			pack_list_ಸೇರಿಸು(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	ಯಾವತ್ (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	ಯದಿ (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		ಪ್ರತ್ಯಾಗಮ;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	ಯಾವತ್ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	ಯಾವತ್ (non_unique) {
		ಸಂರಚನಾ pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		ಯದಿ (non_unique->remaining_objects->size == 0)
			ಭಙ್ಗ;

		pack_list_ಸೇರಿಸು(min, non_unique);

		ಪ್ರತಿ (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		next = non_unique->next;
		ಮುಕ್ತ(non_unique);
		non_unique = next;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

ಸ್ಥಿರ ಶೂನ್ಯ load_all_objects(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ pack_list *pl = local_packs;
	ಸಂರಚನಾ llist_item *hint, *l;

	llist_init(&all_objects);

	ಯಾವತ್ (pl) {
		hint = ರಿಕ್ತ;
		l = pl->remaining_objects->front;
		ಯಾವತ್ (l) {
			hint = llist_ಸೇರಿಸು_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	ಯಾವತ್ (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
ಸ್ಥಿರ ಶೂನ್ಯ cmp_local_packs(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ pack_list *subset, *pl = local_packs;

	/* only one packfile */
	ಯದಿ (!pl->next) {
		llist_init(&pl->unique_objects);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯಾವತ್ ((subset = pl)) {
		ಯಾವತ್ ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ scan_alt_odb_packs(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ pack_list *local, *alt;

	alt = altodb_packs;
	ಯಾವತ್ (alt) {
		local = local_packs;
		ಯಾವತ್ (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->next;
		}
		alt = alt->next;
	}
}

ಸ್ಥಿರ ಸಂರಚನಾ pack_list * add_pack(ಸಂರಚನಾ packed_git *p)
{
	ಸಂರಚನಾ pack_list l;
	size_t off = 0, step;
	ಧ್ರುವ unsigned ವರ್ಣ *base;

	ಯದಿ (!p->pack_local && !(alt_odb || verbose))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	l.pack = p;
	llist_init(&l.remaining_objects);

	ಯದಿ (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	ಯಾವತ್ (off < p->ಅಂಕೆ_objects * step) {
		llist_ಸೇರಿಸು_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = ರಿಕ್ತ;
	ಯದಿ (p->pack_local)
		ಪ್ರತ್ಯಾಗಮ pack_list_ಸೇರಿಸು(&local_packs, &l);
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ pack_list_ಸೇರಿಸು(&altodb_packs, &l);
}

ಸ್ಥಿರ ಸಂರಚನಾ pack_list * add_pack_file(ಧ್ರುವ ವರ್ಣ *filename)
{
	ಸಂರಚನಾ packed_git *p;

	ಯದಿ (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		ಯದಿ (strstr(p->pack_name, filename))
			ಪ್ರತ್ಯಾಗಮ add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

ಸ್ಥಿರ ಶೂನ್ಯ load_all(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

ಪೂರ್ಣಾಙ್ಕ cmd_pack_redundant(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix UNUSED, ಸಂರಚನಾ repository *repo UNUSED) {
	ಪೂರ್ಣಾಙ್ಕ i; ಪೂರ್ಣಾಙ್ಕ i_still_use_this = 0; ಸಂರಚನಾ pack_list *min = ರಿಕ್ತ, *red, *pl;
	ಸಂರಚನಾ llist *ignore;
	ಸಂರಚನಾ strbuf idx_name = STRBUF_INIT;
	ವರ್ಣ buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	ಪ್ರತಿ (i = 1; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[i];
		ಯದಿ (!strcmp(arg, "--")) {
			i++;
			ಭಙ್ಗ;
		}
		ಯದಿ (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!strcmp(arg, "--verbose")) {
			verbose = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (*arg == '-')
			usage(pack_redundant_usage);
		ಅನ್ಯಥಾ
			ಭಙ್ಗ;
	}

	ಯದಿ (!i_still_use_this)
		you_still_use_that("git pack-redundant", ರಿಕ್ತ);

	ಯದಿ (load_all_packs)
		load_all();
	ಅನ್ಯಥಾ
		ಯಾವತ್ (*(argv + i) != ರಿಕ್ತ)
			add_pack_file(*(argv + i++));

	ಯದಿ (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	ಯದಿ (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	ಯದಿ (!isatty(0)) {
		ಸಂರಚನಾ object_id oid;
		ಯಾವತ್ (fgets(buf, ಪರಿಮಾಣ(buf), stdin)) {
			ಯದಿ (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_ಸೇರಿಸು_sorted_unique(ignore, &oid, ರಿಕ್ತ);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	ಯಾವತ್ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	ಯದಿ (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		ಯಾವತ್ (pl) {
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
	ಯಾವತ್ (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	ಯದಿ (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	ಪ್ರತ್ಯಾಗಮ 0;
}
