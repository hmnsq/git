/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"

#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"

#ವ್ಯಾಖ್ಯಾನಿಸು BLKSIZE 512

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ತುಂಬು_all_packs, verbose, alt_odb;

ರಚನೆ llist_ಅಂಶ {
	ರಚನೆ llist_ಅಂಶ *ಮುಂದಿನ;
	ರಚನೆ object_id oid;
};
ಸ್ಥಿರ ರಚನೆ llist {
	ರಚನೆ llist_ಅಂಶ *front;
	ರಚನೆ llist_ಅಂಶ *ಹಿಂಭಾಗ;
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
} *all_objects; /* all objects which must be present in local packfiles */

ಸ್ಥಿರ ರಚನೆ pack_list {
	ರಚನೆ pack_list *ಮುಂದಿನ;
	ರಚನೆ packed_git *pack;
	ರಚನೆ llist *unique_objects;
	ರಚನೆ llist *ಉಳಿದ_objects;
	ಗಾತ್ರ_t all_objects_ಗಾತ್ರ;
} *local_packs = ಖಾಲಿ, *altodb_packs = ಖಾಲಿ;

ಸ್ಥಿರ ರಚನೆ llist_ಅಂಶ *free_nodes;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ llist_ಅಂಶ_put(ರಚನೆ llist_ಅಂಶ *ಅಂಶ)
{
	ಅಂಶ->ಮುಂದಿನ = free_nodes;
	free_nodes = ಅಂಶ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ llist_ಅಂಶ *llist_ಅಂಶ_get(ಶೂನ್ಯ)
{
	ರಚನೆ llist_ಅಂಶ *new_ಅಂಶ;
	ಆದರೆ ( free_nodes ) {
		new_ಅಂಶ = free_nodes;
		free_nodes = free_nodes->ಮುಂದಿನ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಪೂರ್ಣಾಂಕ i = 1;
		ALLOC_ARRAY(new_ಅಂಶ, BLKSIZE);
		ಗಾಗಿ (; i < BLKSIZE; i++)
			llist_ಅಂಶ_put(&new_ಅಂಶ[i]);
	}
	ಹಿಂತಿರುಗಿ new_ಅಂಶ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ llist_init(ರಚನೆ llist **list)
{
	*list = xmalloc(ಗಾತ್ರ(ರಚನೆ llist));
	(*list)->front = (*list)->ಹಿಂಭಾಗ = ಖಾಲಿ;
	(*list)->ಗಾತ್ರ_2 = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ llist_free(ರಚನೆ llist *list)
{
	ಗಾಗಿ (ರಚನೆ llist_ಅಂಶ *i = list->front, *ಮುಂದಿನ; i; i = ಮುಂದಿನ) {
		ಮುಂದಿನ = i->ಮುಂದಿನ;
		llist_ಅಂಶ_put(i);
	}
	ಬಿಡುಗಡೆ(list);
}

ಸ್ಥಿರ ರಚನೆ llist * llist_copy(ರಚನೆ llist *list)
{
	ರಚನೆ llist *ret;
	ರಚನೆ llist_ಅಂಶ *new_ಅಂಶ, *old_ಅಂಶ, *prev;

	llist_init(&ret);

	ಆದರೆ ((ret->ಗಾತ್ರ_2 = list->ಗಾತ್ರ_2) == 0)
		ಹಿಂತಿರುಗಿ ret;

	new_ಅಂಶ = ret->front = llist_ಅಂಶ_get();
	new_ಅಂಶ->oid = list->front->oid;

	old_ಅಂಶ = list->front->ಮುಂದಿನ;
	ಆಗ (old_ಅಂಶ) {
		prev = new_ಅಂಶ;
		new_ಅಂಶ = llist_ಅಂಶ_get();
		prev->ಮುಂದಿನ = new_ಅಂಶ;
		new_ಅಂಶ->oid = old_ಅಂಶ->oid;
		old_ಅಂಶ = old_ಅಂಶ->ಮುಂದಿನ;
	}
	new_ಅಂಶ->ಮುಂದಿನ = ಖಾಲಿ;
	ret->ಹಿಂಭಾಗ = new_ಅಂಶ;

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ llist_ಅಂಶ *llist_ಸೇರಿಸು(ರಚನೆ llist *list,
					      ರಚನೆ llist_ಅಂಶ *after,
					      ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *oid)
{
	ರಚನೆ llist_ಅಂಶ *new_ಅಂಶ = llist_ಅಂಶ_get();
	oidread(&new_ಅಂಶ->oid, oid, the_repository->hash_algo);
	new_ಅಂಶ->ಮುಂದಿನ = ಖಾಲಿ;

	ಆದರೆ (after) {
		new_ಅಂಶ->ಮುಂದಿನ = after->ಮುಂದಿನ;
		after->ಮುಂದಿನ = new_ಅಂಶ;
		ಆದರೆ (after == list->ಹಿಂಭಾಗ)
			list->ಹಿಂಭಾಗ = new_ಅಂಶ;
	} ಇಲ್ಲದಿದ್ದರೆ {/* insert in front */
		ಆದರೆ (list->ಗಾತ್ರ_2 == 0)
			list->ಹಿಂಭಾಗ = new_ಅಂಶ;
		ಇಲ್ಲದಿದ್ದರೆ
			new_ಅಂಶ->ಮುಂದಿನ = list->front;
		list->front = new_ಅಂಶ;
	}
	list->ಗಾತ್ರ_2++;
	ಹಿಂತಿರುಗಿ new_ಅಂಶ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ llist_ಅಂಶ *llist_ಸೇರಿಸು_ಹಿಂಭಾಗ(ರಚನೆ llist *list,
						   ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *oid)
{
	ಹಿಂತಿರುಗಿ llist_ಸೇರಿಸು(list, list->ಹಿಂಭಾಗ, oid);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ llist_ಅಂಶ *llist_ಸೇರಿಸು_sorted_unique(ರಚನೆ llist *list,
			ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ llist_ಅಂಶ *hint)
{
	ರಚನೆ llist_ಅಂಶ *prev = ಖಾಲಿ, *l;

	l = (hint == ಖಾಲಿ) ? list->front : hint;
	ಆಗ (l) {
		ಪೂರ್ಣಾಂಕ cmp = oidcmp(&l->oid, oid);
		ಆದರೆ (cmp > 0) { /* we insert before this entry */
			ಹಿಂತಿರುಗಿ llist_ಸೇರಿಸು(list, prev, oid->hash);
		}
		ಆದರೆ (!cmp) { /* already exists */
			ಹಿಂತಿರುಗಿ l;
		}
		prev = l;
		l = l->ಮುಂದಿನ;
	}
	/* insert at the end */
	ಹಿಂತಿರುಗಿ llist_ಸೇರಿಸು_ಹಿಂಭಾಗ(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ llist_ಅಂಶ * llist_sorted_remove(ರಚನೆ llist *list, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *oid, ರಚನೆ llist_ಅಂಶ *hint)
{
	ರಚನೆ llist_ಅಂಶ *prev, *l;

redo_from_ಆರಂಭ:
	l = (hint == ಖಾಲಿ) ? list->front : hint;
	prev = ಖಾಲಿ;
	ಆಗ (l) {
		ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		ಆದರೆ (cmp > 0) /* not in list, since sorted */
			ಹಿಂತಿರುಗಿ prev;
		ಆದರೆ (!cmp) { /*ಸಿಕ್ಕಿತು*/
			ಆದರೆ (!prev) {
				ಆದರೆ (hint != ಖಾಲಿ && hint != list->front) {
					/* we don't know the previous element */
					hint = ಖಾಲಿ;
					ಹೋಗು redo_from_ಆರಂಭ;
				}
				list->front = l->ಮುಂದಿನ;
			} ಇಲ್ಲದಿದ್ದರೆ
				prev->ಮುಂದಿನ = l->ಮುಂದಿನ;
			ಆದರೆ (l == list->ಹಿಂಭಾಗ)
				list->ಹಿಂಭಾಗ = prev;
			llist_ಅಂಶ_put(l);
			list->ಗಾತ್ರ_2--;
			ಹಿಂತಿರುಗಿ prev;
		}
		prev = l;
		l = l->ಮುಂದಿನ;
	}
	ಹಿಂತಿರುಗಿ prev;
}

/* computes A\B */
ಸ್ಥಿರ ಶೂನ್ಯ llist_sorted_difference_inplace(ರಚನೆ llist *A,
				     ರಚನೆ llist *B)
{
	ರಚನೆ llist_ಅಂಶ *hint, *b;

	hint = ಖಾಲಿ;
	b = B->front;

	ಆಗ (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->ಮುಂದಿನ;
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ pack_list * pack_list_ಸೇರಿಸು(ರಚನೆ pack_list **pl,
					   ರಚನೆ pack_list *ನಮೂದು)
{
	ರಚನೆ pack_list *p = xmalloc(ಗಾತ್ರ(ರಚನೆ pack_list));
	ಸ್ಮರಣೆ_ನಕಲು(p, ನಮೂದು, ಗಾತ್ರ(ರಚನೆ pack_list));
	p->ಮುಂದಿನ = *pl;
	*pl = p;
	ಹಿಂತಿರುಗಿ p;
}

ಸ್ಥಿರ ಶೂನ್ಯ pack_list_free(ರಚನೆ pack_list *pl)
{
	ಗಾಗಿ (ರಚನೆ pack_list *ಮುಂದಿನ; pl; pl = ಮುಂದಿನ) {
		ಮುಂದಿನ = pl->ಮುಂದಿನ;
		ಬಿಡುಗಡೆ(pl);
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಗಾತ್ರ_t pack_list_ಗಾತ್ರ(ರಚನೆ pack_list *pl)
{
	ಗಾತ್ರ_t ret = 0;
	ಆಗ (pl) {
		ret++;
		pl = pl->ಮುಂದಿನ;
	}
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ರಚನೆ pack_list * pack_list_difference(ಸ್ಥಿರಾಂಕ ರಚನೆ pack_list *A,
					       ಸ್ಥಿರಾಂಕ ರಚನೆ pack_list *B)
{
	ರಚನೆ pack_list *ret;
	ಸ್ಥಿರಾಂಕ ರಚನೆ pack_list *pl;

	ಆದರೆ (!A)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	pl = B;
	ಆಗ (pl != ಖಾಲಿ) {
		ಆದರೆ (A->pack == pl->pack)
			ಹಿಂತಿರುಗಿ pack_list_difference(A->ಮುಂದಿನ, B);
		pl = pl->ಮುಂದಿನ;
	}
	ret = xmalloc(ಗಾತ್ರ(ರಚನೆ pack_list));
	ಸ್ಮರಣೆ_ನಕಲು(ret, A, ಗಾತ್ರ(ರಚನೆ pack_list));
	ret->ಮುಂದಿನ = pack_list_difference(A->ಮುಂದಿನ, B);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ cmp_two_packs(ರಚನೆ pack_list *p1, ರಚನೆ pack_list *p2)
{
	ಗಾತ್ರ_t p1_off = 0, p2_off = 0, p1_ಹೆಜ್ಜೆ, p2_ಹೆಜ್ಜೆ;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *p1_ತಳ, *p2_ತಳ;
	ರಚನೆ llist_ಅಂಶ *p1_hint = ಖಾಲಿ, *p2_hint = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hashsz = the_hash_algo->rawsz;

	ಆದರೆ (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->ಉಳಿದ_objects);
	ಆದರೆ (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->ಉಳಿದ_objects);

	p1_ತಳ = p1->pack->ಸೂಚ್ಯಂಕ_ದತ್ತಾಂಶ;
	p2_ತಳ = p2->pack->ಸೂಚ್ಯಂಕ_ದತ್ತಾಂಶ;
	p1_ತಳ += 256 * 4 + ((p1->pack->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ < 2) ? 4 : 8);
	p2_ತಳ += 256 * 4 + ((p2->pack->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ < 2) ? 4 : 8);
	p1_ಹೆಜ್ಜೆ = hashsz + ((p1->pack->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ < 2) ? 4 : 0);
	p2_ಹೆಜ್ಜೆ = hashsz + ((p2->pack->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ < 2) ? 4 : 0);

	ಆಗ (p1_off < p1->pack->ಅಂಕೆ_objects * p1_ಹೆಜ್ಜೆ &&
	       p2_off < p2->pack->ಅಂಕೆ_objects * p2_ಹೆಜ್ಜೆ)
	{
		ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cmp = hashcmp(p1_ತಳ + p1_off, p2_ತಳ + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ಆದರೆ (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_ತಳ + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_ತಳ + p1_off,
					p2_hint);
			p1_off += p1_ಹೆಜ್ಜೆ;
			p2_off += p2_ಹೆಜ್ಜೆ;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_ಹೆಜ್ಜೆ;
		} ಇಲ್ಲದಿದ್ದರೆ { /* p2 has the object, p1 doesn't */
			p2_off += p2_ಹೆಜ್ಜೆ;
		}
	}
}

ಸ್ಥಿರ ಗಾತ್ರ_t sizeof_union(ರಚನೆ packed_git *p1, ರಚನೆ packed_git *p2)
{
	ಗಾತ್ರ_t ret = 0;
	ಗಾತ್ರ_t p1_off = 0, p2_off = 0, p1_ಹೆಜ್ಜೆ, p2_ಹೆಜ್ಜೆ;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *p1_ತಳ, *p2_ತಳ;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hashsz = the_hash_algo->rawsz;

	p1_ತಳ = p1->ಸೂಚ್ಯಂಕ_ದತ್ತಾಂಶ;
	p2_ತಳ = p2->ಸೂಚ್ಯಂಕ_ದತ್ತಾಂಶ;
	p1_ತಳ += 256 * 4 + ((p1->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ < 2) ? 4 : 8);
	p2_ತಳ += 256 * 4 + ((p2->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ < 2) ? 4 : 8);
	p1_ಹೆಜ್ಜೆ = hashsz + ((p1->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ < 2) ? 4 : 0);
	p2_ಹೆಜ್ಜೆ = hashsz + ((p2->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ < 2) ? 4 : 0);

	ಆಗ (p1_off < p1->ಅಂಕೆ_objects * p1_ಹೆಜ್ಜೆ &&
	       p2_off < p2->ಅಂಕೆ_objects * p2_ಹೆಜ್ಜೆ)
	{
		ಪೂರ್ಣಾಂಕ cmp = hashcmp(p1_ತಳ + p1_off, p2_ತಳ + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ಆದರೆ (cmp == 0) {
			ret++;
			p1_off += p1_ಹೆಜ್ಜೆ;
			p2_off += p2_ಹೆಜ್ಜೆ;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_ಹೆಜ್ಜೆ;
		} ಇಲ್ಲದಿದ್ದರೆ { /* p2 has the object, p1 doesn't */
			p2_off += p2_ಹೆಜ್ಜೆ;
		}
	}
	ಹಿಂತಿರುಗಿ ret;
}

/* another O(n^2) function ... */
ಸ್ಥಿರ ಗಾತ್ರ_t get_pack_redundancy(ರಚನೆ pack_list *pl)
{
	ರಚನೆ pack_list *subset;
	ಗಾತ್ರ_t ret = 0;

	ಆದರೆ (!pl)
		ಹಿಂತಿರುಗಿ 0;

	ಆಗ ((subset = pl->ಮುಂದಿನ)) {
		ಆಗ (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->ಮುಂದಿನ;
		}
		pl = pl->ಮುಂದಿನ;
	}
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ off_t pack_ಗಣ_bytecount(ರಚನೆ pack_list *pl)
{
	off_t ret = 0;
	ಆಗ (pl) {
		ret += pl->pack->pack_ಗಾತ್ರ;
		ret += pl->pack->ಸೂಚ್ಯಂಕ_ಗಾತ್ರ;
		pl = pl->ಮುಂದಿನ;
	}
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmp_ಉಳಿದ_objects(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
	ರಚನೆ pack_list *pl_a = *((ರಚನೆ pack_list **)a);
	ರಚನೆ pack_list *pl_b = *((ರಚನೆ pack_list **)b);

	ಆದರೆ (pl_a->ಉಳಿದ_objects->ಗಾತ್ರ_2 == pl_b->ಉಳಿದ_objects->ಗಾತ್ರ_2) {
		/* have the same remaining_objects, big pack first */
		ಆದರೆ (pl_a->all_objects_ಗಾತ್ರ == pl_b->all_objects_ಗಾತ್ರ)
			ಹಿಂತಿರುಗಿ 0;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pl_a->all_objects_ಗಾತ್ರ < pl_b->all_objects_ಗಾತ್ರ)
			ಹಿಂತಿರುಗಿ 1;
		ಇಲ್ಲದಿದ್ದರೆ
			ಹಿಂತಿರುಗಿ -1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pl_a->ಉಳಿದ_objects->ಗಾತ್ರ_2 < pl_b->ಉಳಿದ_objects->ಗಾತ್ರ_2) {
		/* sort by remaining objects, more objects first */
		ಹಿಂತಿರುಗಿ 1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
ಸ್ಥಿರ ಶೂನ್ಯ ವಿಂಗಡಿಸು_pack_list(ರಚನೆ pack_list **pl)
{
	ರಚನೆ pack_list **ary, *p;
	ಗಾತ್ರ_t n = pack_list_ಗಾತ್ರ(*pl);

	ಆದರೆ (n < 2)
		ಹಿಂತಿರುಗಿ;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	ಗಾಗಿ (n = 0, p = *pl; p; p = p->ಮುಂದಿನ)
		ary[n++] = p;

	QSORT(ary, n, cmp_ಉಳಿದ_objects);

	/* link them back again */
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < n - 1; i++)
		ary[i]->ಮುಂದಿನ = ary[i + 1];
	ary[n - 1]->ಮುಂದಿನ = ಖಾಲಿ;
	*pl = ary[0];

	ಬಿಡುಗಡೆ(ary);
}


ಸ್ಥಿರ ಶೂನ್ಯ minimize(ರಚನೆ pack_list **min)
{
	ರಚನೆ pack_list *pl, *unique = ಖಾಲಿ, *non_unique = ಖಾಲಿ;
	ರಚನೆ llist *missing, *unique_pack_objects;

	pl = local_packs;
	ಆಗ (pl) {
		ಆದರೆ (pl->unique_objects->ಗಾತ್ರ_2)
			pack_list_ಸೇರಿಸು(&unique, pl);
		ಇಲ್ಲದಿದ್ದರೆ
			pack_list_ಸೇರಿಸು(&non_unique, pl);
		pl = pl->ಮುಂದಿನ;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	ಆಗ (pl) {
		llist_sorted_difference_inplace(missing, pl->ಉಳಿದ_objects);
		pl = pl->ಮುಂದಿನ;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	ಆದರೆ (missing->ಗಾತ್ರ_2 == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		ಹಿಂತಿರುಗಿ;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	ಆಗ (pl) {
		llist_sorted_difference_inplace(pl->ಉಳಿದ_objects, unique_pack_objects);
		pl = pl->ಮುಂದಿನ;
	}

	ಆಗ (non_unique) {
		ರಚನೆ pack_list *ಮುಂದಿನ;

		/* sort the non_unique packs, greater size of remaining_objects first */
		ವಿಂಗಡಿಸು_pack_list(&non_unique);
		ಆದರೆ (non_unique->ಉಳಿದ_objects->ಗಾತ್ರ_2 == 0)
			ನಿಲ್ಲಿಸು;

		pack_list_ಸೇರಿಸು(min, non_unique);

		ಗಾಗಿ (pl = non_unique->ಮುಂದಿನ; pl && pl->ಉಳಿದ_objects->ಗಾತ್ರ_2 > 0;  pl = pl->ಮುಂದಿನ)
			llist_sorted_difference_inplace(pl->ಉಳಿದ_objects, non_unique->ಉಳಿದ_objects);

		ಮುಂದಿನ = non_unique->ಮುಂದಿನ;
		ಬಿಡುಗಡೆ(non_unique);
		non_unique = ಮುಂದಿನ;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

ಸ್ಥಿರ ಶೂನ್ಯ ತುಂಬು_all_objects(ಶೂನ್ಯ)
{
	ರಚನೆ pack_list *pl = local_packs;
	ರಚನೆ llist_ಅಂಶ *hint, *l;

	llist_init(&all_objects);

	ಆಗ (pl) {
		hint = ಖಾಲಿ;
		l = pl->ಉಳಿದ_objects->front;
		ಆಗ (l) {
			hint = llist_ಸೇರಿಸು_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->ಮುಂದಿನ;
		}
		pl = pl->ಮುಂದಿನ;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	ಆಗ (pl) {
		llist_sorted_difference_inplace(all_objects, pl->ಉಳಿದ_objects);
		pl = pl->ಮುಂದಿನ;
	}
}

/* this scales like O(n^2) */
ಸ್ಥಿರ ಶೂನ್ಯ cmp_local_packs(ಶೂನ್ಯ)
{
	ರಚನೆ pack_list *subset, *pl = local_packs;

	/* only one packfile */
	ಆದರೆ (!pl->ಮುಂದಿನ) {
		llist_init(&pl->unique_objects);
		ಹಿಂತಿರುಗಿ;
	}

	ಆಗ ((subset = pl)) {
		ಆಗ ((subset = subset->ಮುಂದಿನ))
			cmp_two_packs(pl, subset);
		pl = pl->ಮುಂದಿನ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಪರಿಶೀಲನೆ_alt_odb_packs(ಶೂನ್ಯ)
{
	ರಚನೆ pack_list *local, *alt;

	alt = altodb_packs;
	ಆಗ (alt) {
		local = local_packs;
		ಆಗ (local) {
			llist_sorted_difference_inplace(local->ಉಳಿದ_objects,
							alt->ಉಳಿದ_objects);
			local = local->ಮುಂದಿನ;
		}
		alt = alt->ಮುಂದಿನ;
	}
}

ಸ್ಥಿರ ರಚನೆ pack_list * ಕೂಡಿಸು_pack(ರಚನೆ packed_git *p)
{
	ರಚನೆ pack_list l;
	ಗಾತ್ರ_t off = 0, ಹೆಜ್ಜೆ;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ತಳ;

	ಆದರೆ (!p->pack_local && !(alt_odb || verbose))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	l.pack = p;
	llist_init(&l.ಉಳಿದ_objects);

	ಆದರೆ (open_pack_ಸೂಚ್ಯಂಕ(p)) {
		llist_free(l.ಉಳಿದ_objects);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ತಳ = p->ಸೂಚ್ಯಂಕ_ದತ್ತಾಂಶ;
	ತಳ += 256 * 4 + ((p->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ < 2) ? 4 : 8);
	ಹೆಜ್ಜೆ = the_hash_algo->rawsz + ((p->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ < 2) ? 4 : 0);
	ಆಗ (off < p->ಅಂಕೆ_objects * ಹೆಜ್ಜೆ) {
		llist_ಸೇರಿಸು_ಹಿಂಭಾಗ(l.ಉಳಿದ_objects, ತಳ + off);
		off += ಹೆಜ್ಜೆ;
	}
	l.all_objects_ಗಾತ್ರ = l.ಉಳಿದ_objects->ಗಾತ್ರ_2;
	l.unique_objects = ಖಾಲಿ;
	ಆದರೆ (p->pack_local)
		ಹಿಂತಿರುಗಿ pack_list_ಸೇರಿಸು(&local_packs, &l);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ pack_list_ಸೇರಿಸು(&altodb_packs, &l);
}

ಸ್ಥಿರ ರಚನೆ pack_list * ಕೂಡಿಸು_pack_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename)
{
	ರಚನೆ packed_git *p;

	ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(p->pack_name, filename))
			ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_pack(p);
	die("Filename %s not found in packed_git", filename);
}

ಸ್ಥಿರ ಶೂನ್ಯ ತುಂಬು_all(ಶೂನ್ಯ)
{
	ರಚನೆ packed_git *p;

	repo_for_each_pack(the_repository, p)
		ಕೂಡಿಸು_pack(p);
}

ಪೂರ್ಣಾಂಕ cmd_pack_redundant(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ, ರಚನೆ repository *repo ಬಳಸದ) {
	ಪೂರ್ಣಾಂಕ i; ಪೂರ್ಣಾಂಕ i_still_use_this = 0; ರಚನೆ pack_list *min = ಖಾಲಿ, *red, *pl;
	ರಚನೆ llist *ignore;
	ರಚನೆ strbuf idx_name = STRBUF_INIT;
	ಅಕ್ಷರ buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	ಗಾಗಿ (i = 1; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];
		ಆದರೆ (!strcmp(arg, "--")) {
			i++;
			ನಿಲ್ಲಿಸು;
		}
		ಆದರೆ (!strcmp(arg, "--all")) {
			ತುಂಬು_all_packs = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!strcmp(arg, "--verbose")) {
			verbose = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (*arg == '-')
			usage(pack_redundant_usage);
		ಇಲ್ಲದಿದ್ದರೆ
			ನಿಲ್ಲಿಸು;
	}

	ಆದರೆ (!i_still_use_this)
		you_still_use_that("git pack-redundant", ಖಾಲಿ);

	ಆದರೆ (ತುಂಬು_all_packs)
		ತುಂಬು_all();
	ಇಲ್ಲದಿದ್ದರೆ
		ಆಗ (*(argv + i) != ಖಾಲಿ)
			ಕೂಡಿಸು_pack_file(*(argv + i++));

	ಆದರೆ (!local_packs)
		die("Zero packs found!");

	ತುಂಬು_all_objects();

	ಆದರೆ (alt_odb)
		ಪರಿಶೀಲನೆ_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	ಆದರೆ (!isatty(0)) {
		ರಚನೆ object_id oid;
		ಆಗ (fgets(buf, ಗಾತ್ರ(buf), stdin)) {
			ಆದರೆ (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_ಸೇರಿಸು_sorted_unique(ignore, &oid, ಖಾಲಿ);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	ಆಗ (pl) {
		llist_sorted_difference_inplace(pl->ಉಳಿದ_objects, ignore);
		pl = pl->ಮುಂದಿನ;
	}

	cmp_local_packs();

	minimize(&min);

	ಆದರೆ (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)pack_list_ಗಾತ್ರ(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		ಆಗ (pl) {
			fprintf(stderr, "\t%s\n", pl->pack->pack_name);
			pl = pl->ಮುಂದಿನ;
		}
		fprintf(stderr, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)get_pack_redundancy(min),
			(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)pack_ಗಣ_bytecount(min)/1024);
		fprintf(stderr, "A total of %lu unique objects were considered.\n",
			(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)all_objects->ಗಾತ್ರ_2);
		fprintf(stderr, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	ಆಗ (pl) {
		ಬರೆ("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->ಮುಂದಿನ;
	}
	ಆದರೆ (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)pack_ಗಣ_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	ಹಿಂತಿರುಗಿ 0;
}
