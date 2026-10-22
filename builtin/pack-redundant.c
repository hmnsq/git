/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLKSIZE 512

ਸਥਿਰ ਅਚਲ ਅੱਖਰ pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

ਸਥਿਰ ਪੂਰਨ ਲੱਦੋ_all_packs, verbose, alt_odb;

ਬਣਤਰ llist_ਵਸਤੂ {
	ਬਣਤਰ llist_ਵਸਤੂ *ਅਗਲਾ;
	ਬਣਤਰ object_id oid;
};
ਸਥਿਰ ਬਣਤਰ llist {
	ਬਣਤਰ llist_ਵਸਤੂ *front;
	ਬਣਤਰ llist_ਵਸਤੂ *ਪਿੱਠ;
	ਮਾਪ_t ਮਾਪ;
} *all_objects; /* all objects which must be present in local packfiles */

ਸਥਿਰ ਬਣਤਰ pack_list {
	ਬਣਤਰ pack_list *ਅਗਲਾ;
	ਬਣਤਰ packed_git *pack;
	ਬਣਤਰ llist *unique_objects;
	ਬਣਤਰ llist *ਬਚਿਆ_objects;
	ਮਾਪ_t all_objects_ਮਾਪ;
} *local_packs = ਨਿਰਾ, *altodb_packs = ਨਿਰਾ;

ਸਥਿਰ ਬਣਤਰ llist_ਵਸਤੂ *free_nodes;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ llist_ਵਸਤੂ_put(ਬਣਤਰ llist_ਵਸਤੂ *ਵਸਤੂ)
{
	ਵਸਤੂ->ਅਗਲਾ = free_nodes;
	free_nodes = ਵਸਤੂ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ llist_ਵਸਤੂ *llist_ਵਸਤੂ_get(ਖਾਲੀ)
{
	ਬਣਤਰ llist_ਵਸਤੂ *new_ਵਸਤੂ;
	ਜੇ ( free_nodes ) {
		new_ਵਸਤੂ = free_nodes;
		free_nodes = free_nodes->ਅਗਲਾ;
	} ਨਹੀਂਤਾਂ {
		ਪੂਰਨ i = 1;
		ALLOC_ARRAY(new_ਵਸਤੂ, BLKSIZE);
		ਲਈ (; i < BLKSIZE; i++)
			llist_ਵਸਤੂ_put(&new_ਵਸਤੂ[i]);
	}
	ਵਾਪਸ new_ਵਸਤੂ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ llist_init(ਬਣਤਰ llist **list)
{
	*list = xmalloc(ਆਕਾਰ(ਬਣਤਰ llist));
	(*list)->front = (*list)->ਪਿੱਠ = ਨਿਰਾ;
	(*list)->ਮਾਪ = 0;
}

ਸਥਿਰ ਖਾਲੀ llist_free(ਬਣਤਰ llist *list)
{
	ਲਈ (ਬਣਤਰ llist_ਵਸਤੂ *i = list->front, *ਅਗਲਾ; i; i = ਅਗਲਾ) {
		ਅਗਲਾ = i->ਅਗਲਾ;
		llist_ਵਸਤੂ_put(i);
	}
	ਮੁਕਤ(list);
}

ਸਥਿਰ ਬਣਤਰ llist * llist_copy(ਬਣਤਰ llist *list)
{
	ਬਣਤਰ llist *ret;
	ਬਣਤਰ llist_ਵਸਤੂ *new_ਵਸਤੂ, *old_ਵਸਤੂ, *prev;

	llist_init(&ret);

	ਜੇ ((ret->ਮਾਪ = list->ਮਾਪ) == 0)
		ਵਾਪਸ ret;

	new_ਵਸਤੂ = ret->front = llist_ਵਸਤੂ_get();
	new_ਵਸਤੂ->oid = list->front->oid;

	old_ਵਸਤੂ = list->front->ਅਗਲਾ;
	ਜਦੋਂ (old_ਵਸਤੂ) {
		prev = new_ਵਸਤੂ;
		new_ਵਸਤੂ = llist_ਵਸਤੂ_get();
		prev->ਅਗਲਾ = new_ਵਸਤੂ;
		new_ਵਸਤੂ->oid = old_ਵਸਤੂ->oid;
		old_ਵਸਤੂ = old_ਵਸਤੂ->ਅਗਲਾ;
	}
	new_ਵਸਤੂ->ਅਗਲਾ = ਨਿਰਾ;
	ret->ਪਿੱਠ = new_ਵਸਤੂ;

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ llist_ਵਸਤੂ *llist_ਪਾਓ(ਬਣਤਰ llist *list,
					      ਬਣਤਰ llist_ਵਸਤੂ *after,
					      ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *oid)
{
	ਬਣਤਰ llist_ਵਸਤੂ *new_ਵਸਤੂ = llist_ਵਸਤੂ_get();
	oidread(&new_ਵਸਤੂ->oid, oid, the_repository->hash_algo);
	new_ਵਸਤੂ->ਅਗਲਾ = ਨਿਰਾ;

	ਜੇ (after) {
		new_ਵਸਤੂ->ਅਗਲਾ = after->ਅਗਲਾ;
		after->ਅਗਲਾ = new_ਵਸਤੂ;
		ਜੇ (after == list->ਪਿੱਠ)
			list->ਪਿੱਠ = new_ਵਸਤੂ;
	} ਨਹੀਂਤਾਂ {/* insert in front */
		ਜੇ (list->ਮਾਪ == 0)
			list->ਪਿੱਠ = new_ਵਸਤੂ;
		ਨਹੀਂਤਾਂ
			new_ਵਸਤੂ->ਅਗਲਾ = list->front;
		list->front = new_ਵਸਤੂ;
	}
	list->ਮਾਪ++;
	ਵਾਪਸ new_ਵਸਤੂ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ llist_ਵਸਤੂ *llist_ਪਾਓ_ਪਿੱਠ(ਬਣਤਰ llist *list,
						   ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *oid)
{
	ਵਾਪਸ llist_ਪਾਓ(list, list->ਪਿੱਠ, oid);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ llist_ਵਸਤੂ *llist_ਪਾਓ_sorted_unique(ਬਣਤਰ llist *list,
			ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ llist_ਵਸਤੂ *hint)
{
	ਬਣਤਰ llist_ਵਸਤੂ *prev = ਨਿਰਾ, *l;

	l = (hint == ਨਿਰਾ) ? list->front : hint;
	ਜਦੋਂ (l) {
		ਪੂਰਨ cmp = oidcmp(&l->oid, oid);
		ਜੇ (cmp > 0) { /* we insert before this entry */
			ਵਾਪਸ llist_ਪਾਓ(list, prev, oid->hash);
		}
		ਜੇ (!cmp) { /* already exists */
			ਵਾਪਸ l;
		}
		prev = l;
		l = l->ਅਗਲਾ;
	}
	/* insert at the end */
	ਵਾਪਸ llist_ਪਾਓ_ਪਿੱਠ(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ llist_ਵਸਤੂ * llist_sorted_remove(ਬਣਤਰ llist *list, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *oid, ਬਣਤਰ llist_ਵਸਤੂ *hint)
{
	ਬਣਤਰ llist_ਵਸਤੂ *prev, *l;

redo_from_ਸ਼ੁਰੂ:
	l = (hint == ਨਿਰਾ) ? list->front : hint;
	prev = ਨਿਰਾ;
	ਜਦੋਂ (l) {
		ਅਚਲ ਪੂਰਨ cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		ਜੇ (cmp > 0) /* not in list, since sorted */
			ਵਾਪਸ prev;
		ਜੇ (!cmp) { /*ਮਿਲਿਆ*/
			ਜੇ (!prev) {
				ਜੇ (hint != ਨਿਰਾ && hint != list->front) {
					/* we don't know the previous element */
					hint = ਨਿਰਾ;
					ਜਾਓ redo_from_ਸ਼ੁਰੂ;
				}
				list->front = l->ਅਗਲਾ;
			} ਨਹੀਂਤਾਂ
				prev->ਅਗਲਾ = l->ਅਗਲਾ;
			ਜੇ (l == list->ਪਿੱਠ)
				list->ਪਿੱਠ = prev;
			llist_ਵਸਤੂ_put(l);
			list->ਮਾਪ--;
			ਵਾਪਸ prev;
		}
		prev = l;
		l = l->ਅਗਲਾ;
	}
	ਵਾਪਸ prev;
}

/* computes A\B */
ਸਥਿਰ ਖਾਲੀ llist_sorted_difference_inplace(ਬਣਤਰ llist *A,
				     ਬਣਤਰ llist *B)
{
	ਬਣਤਰ llist_ਵਸਤੂ *hint, *b;

	hint = ਨਿਰਾ;
	b = B->front;

	ਜਦੋਂ (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->ਅਗਲਾ;
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ pack_list * pack_list_ਪਾਓ(ਬਣਤਰ pack_list **pl,
					   ਬਣਤਰ pack_list *ਇੰਦਰਾਜ)
{
	ਬਣਤਰ pack_list *p = xmalloc(ਆਕਾਰ(ਬਣਤਰ pack_list));
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(p, ਇੰਦਰਾਜ, ਆਕਾਰ(ਬਣਤਰ pack_list));
	p->ਅਗਲਾ = *pl;
	*pl = p;
	ਵਾਪਸ p;
}

ਸਥਿਰ ਖਾਲੀ pack_list_free(ਬਣਤਰ pack_list *pl)
{
	ਲਈ (ਬਣਤਰ pack_list *ਅਗਲਾ; pl; pl = ਅਗਲਾ) {
		ਅਗਲਾ = pl->ਅਗਲਾ;
		ਮੁਕਤ(pl);
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਮਾਪ_t pack_list_ਮਾਪ(ਬਣਤਰ pack_list *pl)
{
	ਮਾਪ_t ret = 0;
	ਜਦੋਂ (pl) {
		ret++;
		pl = pl->ਅਗਲਾ;
	}
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਬਣਤਰ pack_list * pack_list_difference(ਅਚਲ ਬਣਤਰ pack_list *A,
					       ਅਚਲ ਬਣਤਰ pack_list *B)
{
	ਬਣਤਰ pack_list *ret;
	ਅਚਲ ਬਣਤਰ pack_list *pl;

	ਜੇ (!A)
		ਵਾਪਸ ਨਿਰਾ;

	pl = B;
	ਜਦੋਂ (pl != ਨਿਰਾ) {
		ਜੇ (A->pack == pl->pack)
			ਵਾਪਸ pack_list_difference(A->ਅਗਲਾ, B);
		pl = pl->ਅਗਲਾ;
	}
	ret = xmalloc(ਆਕਾਰ(ਬਣਤਰ pack_list));
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ret, A, ਆਕਾਰ(ਬਣਤਰ pack_list));
	ret->ਅਗਲਾ = pack_list_difference(A->ਅਗਲਾ, B);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ cmp_two_packs(ਬਣਤਰ pack_list *p1, ਬਣਤਰ pack_list *p2)
{
	ਮਾਪ_t p1_off = 0, p2_off = 0, p1_ਕਦਮ, p2_ਕਦਮ;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p1_ਅਧਾਰ, *p2_ਅਧਾਰ;
	ਬਣਤਰ llist_ਵਸਤੂ *p1_hint = ਨਿਰਾ, *p2_hint = ਨਿਰਾ;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hashsz = the_hash_algo->rawsz;

	ਜੇ (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->ਬਚਿਆ_objects);
	ਜੇ (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->ਬਚਿਆ_objects);

	p1_ਅਧਾਰ = p1->pack->ਸੂਚਕਾਂਕ_ਅੰਕੜੇ;
	p2_ਅਧਾਰ = p2->pack->ਸੂਚਕਾਂਕ_ਅੰਕੜੇ;
	p1_ਅਧਾਰ += 256 * 4 + ((p1->pack->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ < 2) ? 4 : 8);
	p2_ਅਧਾਰ += 256 * 4 + ((p2->pack->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ < 2) ? 4 : 8);
	p1_ਕਦਮ = hashsz + ((p1->pack->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ < 2) ? 4 : 0);
	p2_ਕਦਮ = hashsz + ((p2->pack->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ < 2) ? 4 : 0);

	ਜਦੋਂ (p1_off < p1->pack->ਅੰਕ_objects * p1_ਕਦਮ &&
	       p2_off < p2->pack->ਅੰਕ_objects * p2_ਕਦਮ)
	{
		ਅਚਲ ਪੂਰਨ cmp = hashcmp(p1_ਅਧਾਰ + p1_off, p2_ਅਧਾਰ + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ਜੇ (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_ਅਧਾਰ + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_ਅਧਾਰ + p1_off,
					p2_hint);
			p1_off += p1_ਕਦਮ;
			p2_off += p2_ਕਦਮ;
			ਜਾਰੀ;
		}
		ਜੇ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_ਕਦਮ;
		} ਨਹੀਂਤਾਂ { /* p2 has the object, p1 doesn't */
			p2_off += p2_ਕਦਮ;
		}
	}
}

ਸਥਿਰ ਮਾਪ_t sizeof_union(ਬਣਤਰ packed_git *p1, ਬਣਤਰ packed_git *p2)
{
	ਮਾਪ_t ret = 0;
	ਮਾਪ_t p1_off = 0, p2_off = 0, p1_ਕਦਮ, p2_ਕਦਮ;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p1_ਅਧਾਰ, *p2_ਅਧਾਰ;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hashsz = the_hash_algo->rawsz;

	p1_ਅਧਾਰ = p1->ਸੂਚਕਾਂਕ_ਅੰਕੜੇ;
	p2_ਅਧਾਰ = p2->ਸੂਚਕਾਂਕ_ਅੰਕੜੇ;
	p1_ਅਧਾਰ += 256 * 4 + ((p1->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ < 2) ? 4 : 8);
	p2_ਅਧਾਰ += 256 * 4 + ((p2->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ < 2) ? 4 : 8);
	p1_ਕਦਮ = hashsz + ((p1->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ < 2) ? 4 : 0);
	p2_ਕਦਮ = hashsz + ((p2->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ < 2) ? 4 : 0);

	ਜਦੋਂ (p1_off < p1->ਅੰਕ_objects * p1_ਕਦਮ &&
	       p2_off < p2->ਅੰਕ_objects * p2_ਕਦਮ)
	{
		ਪੂਰਨ cmp = hashcmp(p1_ਅਧਾਰ + p1_off, p2_ਅਧਾਰ + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ਜੇ (cmp == 0) {
			ret++;
			p1_off += p1_ਕਦਮ;
			p2_off += p2_ਕਦਮ;
			ਜਾਰੀ;
		}
		ਜੇ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_ਕਦਮ;
		} ਨਹੀਂਤਾਂ { /* p2 has the object, p1 doesn't */
			p2_off += p2_ਕਦਮ;
		}
	}
	ਵਾਪਸ ret;
}

/* another O(n^2) function ... */
ਸਥਿਰ ਮਾਪ_t get_pack_redundancy(ਬਣਤਰ pack_list *pl)
{
	ਬਣਤਰ pack_list *subset;
	ਮਾਪ_t ret = 0;

	ਜੇ (!pl)
		ਵਾਪਸ 0;

	ਜਦੋਂ ((subset = pl->ਅਗਲਾ)) {
		ਜਦੋਂ (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->ਅਗਲਾ;
		}
		pl = pl->ਅਗਲਾ;
	}
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ off_t pack_ਸੈੱਟ_bytecount(ਬਣਤਰ pack_list *pl)
{
	off_t ret = 0;
	ਜਦੋਂ (pl) {
		ret += pl->pack->pack_ਮਾਪ;
		ret += pl->pack->ਸੂਚਕਾਂਕ_ਮਾਪ;
		pl = pl->ਅਗਲਾ;
	}
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmp_ਬਚਿਆ_objects(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
	ਬਣਤਰ pack_list *pl_a = *((ਬਣਤਰ pack_list **)a);
	ਬਣਤਰ pack_list *pl_b = *((ਬਣਤਰ pack_list **)b);

	ਜੇ (pl_a->ਬਚਿਆ_objects->ਮਾਪ == pl_b->ਬਚਿਆ_objects->ਮਾਪ) {
		/* have the same remaining_objects, big pack first */
		ਜੇ (pl_a->all_objects_ਮਾਪ == pl_b->all_objects_ਮਾਪ)
			ਵਾਪਸ 0;
		ਨਹੀਂਤਾਂ ਜੇ (pl_a->all_objects_ਮਾਪ < pl_b->all_objects_ਮਾਪ)
			ਵਾਪਸ 1;
		ਨਹੀਂਤਾਂ
			ਵਾਪਸ -1;
	} ਨਹੀਂਤਾਂ ਜੇ (pl_a->ਬਚਿਆ_objects->ਮਾਪ < pl_b->ਬਚਿਆ_objects->ਮਾਪ) {
		/* sort by remaining objects, more objects first */
		ਵਾਪਸ 1;
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
ਸਥਿਰ ਖਾਲੀ ਛਾਂਟੋ_pack_list(ਬਣਤਰ pack_list **pl)
{
	ਬਣਤਰ pack_list **ary, *p;
	ਮਾਪ_t n = pack_list_ਮਾਪ(*pl);

	ਜੇ (n < 2)
		ਵਾਪਸ;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	ਲਈ (n = 0, p = *pl; p; p = p->ਅਗਲਾ)
		ary[n++] = p;

	QSORT(ary, n, cmp_ਬਚਿਆ_objects);

	/* link them back again */
	ਲਈ (ਮਾਪ_t i = 0; i < n - 1; i++)
		ary[i]->ਅਗਲਾ = ary[i + 1];
	ary[n - 1]->ਅਗਲਾ = ਨਿਰਾ;
	*pl = ary[0];

	ਮੁਕਤ(ary);
}


ਸਥਿਰ ਖਾਲੀ minimize(ਬਣਤਰ pack_list **min)
{
	ਬਣਤਰ pack_list *pl, *unique = ਨਿਰਾ, *non_unique = ਨਿਰਾ;
	ਬਣਤਰ llist *missing, *unique_pack_objects;

	pl = local_packs;
	ਜਦੋਂ (pl) {
		ਜੇ (pl->unique_objects->ਮਾਪ)
			pack_list_ਪਾਓ(&unique, pl);
		ਨਹੀਂਤਾਂ
			pack_list_ਪਾਓ(&non_unique, pl);
		pl = pl->ਅਗਲਾ;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	ਜਦੋਂ (pl) {
		llist_sorted_difference_inplace(missing, pl->ਬਚਿਆ_objects);
		pl = pl->ਅਗਲਾ;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	ਜੇ (missing->ਮਾਪ == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		ਵਾਪਸ;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	ਜਦੋਂ (pl) {
		llist_sorted_difference_inplace(pl->ਬਚਿਆ_objects, unique_pack_objects);
		pl = pl->ਅਗਲਾ;
	}

	ਜਦੋਂ (non_unique) {
		ਬਣਤਰ pack_list *ਅਗਲਾ;

		/* sort the non_unique packs, greater size of remaining_objects first */
		ਛਾਂਟੋ_pack_list(&non_unique);
		ਜੇ (non_unique->ਬਚਿਆ_objects->ਮਾਪ == 0)
			ਰੋਕੋ;

		pack_list_ਪਾਓ(min, non_unique);

		ਲਈ (pl = non_unique->ਅਗਲਾ; pl && pl->ਬਚਿਆ_objects->ਮਾਪ > 0;  pl = pl->ਅਗਲਾ)
			llist_sorted_difference_inplace(pl->ਬਚਿਆ_objects, non_unique->ਬਚਿਆ_objects);

		ਅਗਲਾ = non_unique->ਅਗਲਾ;
		ਮੁਕਤ(non_unique);
		non_unique = ਅਗਲਾ;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

ਸਥਿਰ ਖਾਲੀ ਲੱਦੋ_all_objects(ਖਾਲੀ)
{
	ਬਣਤਰ pack_list *pl = local_packs;
	ਬਣਤਰ llist_ਵਸਤੂ *hint, *l;

	llist_init(&all_objects);

	ਜਦੋਂ (pl) {
		hint = ਨਿਰਾ;
		l = pl->ਬਚਿਆ_objects->front;
		ਜਦੋਂ (l) {
			hint = llist_ਪਾਓ_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->ਅਗਲਾ;
		}
		pl = pl->ਅਗਲਾ;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	ਜਦੋਂ (pl) {
		llist_sorted_difference_inplace(all_objects, pl->ਬਚਿਆ_objects);
		pl = pl->ਅਗਲਾ;
	}
}

/* this scales like O(n^2) */
ਸਥਿਰ ਖਾਲੀ cmp_local_packs(ਖਾਲੀ)
{
	ਬਣਤਰ pack_list *subset, *pl = local_packs;

	/* only one packfile */
	ਜੇ (!pl->ਅਗਲਾ) {
		llist_init(&pl->unique_objects);
		ਵਾਪਸ;
	}

	ਜਦੋਂ ((subset = pl)) {
		ਜਦੋਂ ((subset = subset->ਅਗਲਾ))
			cmp_two_packs(pl, subset);
		pl = pl->ਅਗਲਾ;
	}
}

ਸਥਿਰ ਖਾਲੀ ਜਾਂਚ_alt_odb_packs(ਖਾਲੀ)
{
	ਬਣਤਰ pack_list *local, *alt;

	alt = altodb_packs;
	ਜਦੋਂ (alt) {
		local = local_packs;
		ਜਦੋਂ (local) {
			llist_sorted_difference_inplace(local->ਬਚਿਆ_objects,
							alt->ਬਚਿਆ_objects);
			local = local->ਅਗਲਾ;
		}
		alt = alt->ਅਗਲਾ;
	}
}

ਸਥਿਰ ਬਣਤਰ pack_list * ਜੋੜੋ_pack(ਬਣਤਰ packed_git *p)
{
	ਬਣਤਰ pack_list l;
	ਮਾਪ_t off = 0, ਕਦਮ;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅਧਾਰ;

	ਜੇ (!p->pack_local && !(alt_odb || verbose))
		ਵਾਪਸ ਨਿਰਾ;

	l.pack = p;
	llist_init(&l.ਬਚਿਆ_objects);

	ਜੇ (open_pack_ਸੂਚਕਾਂਕ(p)) {
		llist_free(l.ਬਚਿਆ_objects);
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਅਧਾਰ = p->ਸੂਚਕਾਂਕ_ਅੰਕੜੇ;
	ਅਧਾਰ += 256 * 4 + ((p->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ < 2) ? 4 : 8);
	ਕਦਮ = the_hash_algo->rawsz + ((p->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ < 2) ? 4 : 0);
	ਜਦੋਂ (off < p->ਅੰਕ_objects * ਕਦਮ) {
		llist_ਪਾਓ_ਪਿੱਠ(l.ਬਚਿਆ_objects, ਅਧਾਰ + off);
		off += ਕਦਮ;
	}
	l.all_objects_ਮਾਪ = l.ਬਚਿਆ_objects->ਮਾਪ;
	l.unique_objects = ਨਿਰਾ;
	ਜੇ (p->pack_local)
		ਵਾਪਸ pack_list_ਪਾਓ(&local_packs, &l);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ pack_list_ਪਾਓ(&altodb_packs, &l);
}

ਸਥਿਰ ਬਣਤਰ pack_list * ਜੋੜੋ_pack_file(ਅਚਲ ਅੱਖਰ *filename)
{
	ਬਣਤਰ packed_git *p;

	ਜੇ (ਲੜੀ_ਲੰਬਾਈ(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		ਜੇ (ਲੜੀ_ਖੋਜ(p->pack_name, filename))
			ਵਾਪਸ ਜੋੜੋ_pack(p);
	die("Filename %s not found in packed_git", filename);
}

ਸਥਿਰ ਖਾਲੀ ਲੱਦੋ_all(ਖਾਲੀ)
{
	ਬਣਤਰ packed_git *p;

	repo_for_each_pack(the_repository, p)
		ਜੋੜੋ_pack(p);
}

ਪੂਰਨ cmd_pack_redundant(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ, ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ) {
	ਪੂਰਨ i; ਪੂਰਨ i_still_use_this = 0; ਬਣਤਰ pack_list *min = ਨਿਰਾ, *red, *pl;
	ਬਣਤਰ llist *ignore;
	ਬਣਤਰ strbuf idx_name = STRBUF_INIT;
	ਅੱਖਰ buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	ਲਈ (i = 1; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];
		ਜੇ (!strcmp(arg, "--")) {
			i++;
			ਰੋਕੋ;
		}
		ਜੇ (!strcmp(arg, "--all")) {
			ਲੱਦੋ_all_packs = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--verbose")) {
			verbose = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			ਜਾਰੀ;
		}
		ਜੇ (*arg == '-')
			usage(pack_redundant_usage);
		ਨਹੀਂਤਾਂ
			ਰੋਕੋ;
	}

	ਜੇ (!i_still_use_this)
		you_still_use_that("git pack-redundant", ਨਿਰਾ);

	ਜੇ (ਲੱਦੋ_all_packs)
		ਲੱਦੋ_all();
	ਨਹੀਂਤਾਂ
		ਜਦੋਂ (*(argv + i) != ਨਿਰਾ)
			ਜੋੜੋ_pack_file(*(argv + i++));

	ਜੇ (!local_packs)
		die("Zero packs found!");

	ਲੱਦੋ_all_objects();

	ਜੇ (alt_odb)
		ਜਾਂਚ_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	ਜੇ (!isatty(0)) {
		ਬਣਤਰ object_id oid;
		ਜਦੋਂ (fgets(buf, ਆਕਾਰ(buf), stdin)) {
			ਜੇ (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_ਪਾਓ_sorted_unique(ignore, &oid, ਨਿਰਾ);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	ਜਦੋਂ (pl) {
		llist_sorted_difference_inplace(pl->ਬਚਿਆ_objects, ignore);
		pl = pl->ਅਗਲਾ;
	}

	cmp_local_packs();

	minimize(&min);

	ਜੇ (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)pack_list_ਮਾਪ(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		ਜਦੋਂ (pl) {
			fprintf(stderr, "\t%s\n", pl->pack->pack_name);
			pl = pl->ਅਗਲਾ;
		}
		fprintf(stderr, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)get_pack_redundancy(min),
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)pack_ਸੈੱਟ_bytecount(min)/1024);
		fprintf(stderr, "A total of %lu unique objects were considered.\n",
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)all_objects->ਮਾਪ);
		fprintf(stderr, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	ਜਦੋਂ (pl) {
		ਲਿਖੋ("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->ਅਗਲਾ;
	}
	ਜੇ (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)pack_ਸੈੱਟ_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	ਵਾਪਸ 0;
}
