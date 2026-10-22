/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"

#അന്തര്ഭാവ "packfile.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "strbuf.h"

#define BLKSIZE 512

സ്ഥിതം സ്ഥിരം അക്ഷരം pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

സ്ഥിതം പൂർണ്ണസംഖ്യ load_all_packs, verbose, alt_odb;

ഘടന llist_item {
	ഘടന llist_item *next;
	ഘടന object_id oid;
};
സ്ഥിതം ഘടന llist {
	ഘടന llist_item *front;
	ഘടന llist_item *back;
	size_t size;
} *all_objects; /* all objects which must be present in local packfiles */

സ്ഥിതം ഘടന pack_list {
	ഘടന pack_list *next;
	ഘടന packed_git *pack;
	ഘടന llist *unique_objects;
	ഘടന llist *remaining_objects;
	size_t all_objects_size;
} *local_packs = ശൂന്യ, *altodb_packs = ശൂന്യ;

സ്ഥിതം ഘടന llist_item *free_nodes;

സ്ഥിതം വരിയിൽ ശൂന്യം llist_item_put(ഘടന llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

സ്ഥിതം വരിയിൽ ഘടന llist_item *llist_item_get(ശൂന്യം)
{
	ഘടന llist_item *new_item;
	എങ്കിൽ ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} അല്ലെങ്കിൽ {
		പൂർണ്ണസംഖ്യ i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		വേണ്ടി (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	തിരികെ new_item;
}

സ്ഥിതം വരിയിൽ ശൂന്യം llist_init(ഘടന llist **list)
{
	*list = xmalloc(വലുപ്പം(ഘടന llist));
	(*list)->front = (*list)->back = ശൂന്യ;
	(*list)->size = 0;
}

സ്ഥിതം ശൂന്യം llist_free(ഘടന llist *list)
{
	വേണ്ടി (ഘടന llist_item *i = list->front, *next; i; i = next) {
		next = i->next;
		llist_item_put(i);
	}
	സ്വതന്ത്രം(list);
}

സ്ഥിതം ഘടന llist * llist_copy(ഘടന llist *list)
{
	ഘടന llist *ret;
	ഘടന llist_item *new_item, *old_item, *prev;

	llist_init(&ret);

	എങ്കിൽ ((ret->size = list->size) == 0)
		തിരികെ ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	ആകുമ്പോൾ (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = ശൂന്യ;
	ret->back = new_item;

	തിരികെ ret;
}

സ്ഥിതം വരിയിൽ ഘടന llist_item *llist_തിരുകുക(ഘടന llist *list,
					      ഘടന llist_item *after,
					      സ്ഥിരം unsigned അക്ഷരം *oid)
{
	ഘടന llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = ശൂന്യ;

	എങ്കിൽ (after) {
		new_item->next = after->next;
		after->next = new_item;
		എങ്കിൽ (after == list->back)
			list->back = new_item;
	} അല്ലെങ്കിൽ {/* insert in front */
		എങ്കിൽ (list->size == 0)
			list->back = new_item;
		അല്ലെങ്കിൽ
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	തിരികെ new_item;
}

സ്ഥിതം വരിയിൽ ഘടന llist_item *llist_തിരുകുക_back(ഘടന llist *list,
						   സ്ഥിരം unsigned അക്ഷരം *oid)
{
	തിരികെ llist_തിരുകുക(list, list->back, oid);
}

സ്ഥിതം വരിയിൽ ഘടന llist_item *llist_തിരുകുക_sorted_unique(ഘടന llist *list,
			സ്ഥിരം ഘടന object_id *oid, ഘടന llist_item *hint)
{
	ഘടന llist_item *prev = ശൂന്യ, *l;

	l = (hint == ശൂന്യ) ? list->front : hint;
	ആകുമ്പോൾ (l) {
		പൂർണ്ണസംഖ്യ cmp = oidcmp(&l->oid, oid);
		എങ്കിൽ (cmp > 0) { /* we insert before this entry */
			തിരികെ llist_തിരുകുക(list, prev, oid->hash);
		}
		എങ്കിൽ (!cmp) { /* already exists */
			തിരികെ l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	തിരികെ llist_തിരുകുക_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
സ്ഥിതം വരിയിൽ ഘടന llist_item * llist_sorted_remove(ഘടന llist *list, സ്ഥിരം unsigned അക്ഷരം *oid, ഘടന llist_item *hint)
{
	ഘടന llist_item *prev, *l;

redo_from_start:
	l = (hint == ശൂന്യ) ? list->front : hint;
	prev = ശൂന്യ;
	ആകുമ്പോൾ (l) {
		സ്ഥിരം പൂർണ്ണസംഖ്യ cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		എങ്കിൽ (cmp > 0) /* not in list, since sorted */
			തിരികെ prev;
		എങ്കിൽ (!cmp) { /* found */
			എങ്കിൽ (!prev) {
				എങ്കിൽ (hint != ശൂന്യ && hint != list->front) {
					/* we don't know the previous element */
					hint = ശൂന്യ;
					പോകുക redo_from_start;
				}
				list->front = l->next;
			} അല്ലെങ്കിൽ
				prev->next = l->next;
			എങ്കിൽ (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			തിരികെ prev;
		}
		prev = l;
		l = l->next;
	}
	തിരികെ prev;
}

/* computes A\B */
സ്ഥിതം ശൂന്യം llist_sorted_difference_inplace(ഘടന llist *A,
				     ഘടന llist *B)
{
	ഘടന llist_item *hint, *b;

	hint = ശൂന്യ;
	b = B->front;

	ആകുമ്പോൾ (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

സ്ഥിതം വരിയിൽ ഘടന pack_list * pack_list_തിരുകുക(ഘടന pack_list **pl,
					   ഘടന pack_list *entry)
{
	ഘടന pack_list *p = xmalloc(വലുപ്പം(ഘടന pack_list));
	memcpy(p, entry, വലുപ്പം(ഘടന pack_list));
	p->next = *pl;
	*pl = p;
	തിരികെ p;
}

സ്ഥിതം ശൂന്യം pack_list_free(ഘടന pack_list *pl)
{
	വേണ്ടി (ഘടന pack_list *next; pl; pl = next) {
		next = pl->next;
		സ്വതന്ത്രം(pl);
	}
}

സ്ഥിതം വരിയിൽ size_t pack_list_size(ഘടന pack_list *pl)
{
	size_t ret = 0;
	ആകുമ്പോൾ (pl) {
		ret++;
		pl = pl->next;
	}
	തിരികെ ret;
}

സ്ഥിതം ഘടന pack_list * pack_list_difference(സ്ഥിരം ഘടന pack_list *A,
					       സ്ഥിരം ഘടന pack_list *B)
{
	ഘടന pack_list *ret;
	സ്ഥിരം ഘടന pack_list *pl;

	എങ്കിൽ (!A)
		തിരികെ ശൂന്യ;

	pl = B;
	ആകുമ്പോൾ (pl != ശൂന്യ) {
		എങ്കിൽ (A->pack == pl->pack)
			തിരികെ pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(വലുപ്പം(ഘടന pack_list));
	memcpy(ret, A, വലുപ്പം(ഘടന pack_list));
	ret->next = pack_list_difference(A->next, B);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം cmp_two_packs(ഘടന pack_list *p1, ഘടന pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	സ്ഥിരം unsigned അക്ഷരം *p1_base, *p2_base;
	ഘടന llist_item *p1_hint = ശൂന്യ, *p2_hint = ശൂന്യ;
	സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ hashsz = the_hash_algo->rawsz;

	എങ്കിൽ (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	എങ്കിൽ (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	ആകുമ്പോൾ (p1_off < p1->pack->അക്കം_objects * p1_step &&
	       p2_off < p2->pack->അക്കം_objects * p2_step)
	{
		സ്ഥിരം പൂർണ്ണസംഖ്യ cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		എങ്കിൽ (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			തുടരുക;
		}
		എങ്കിൽ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} അല്ലെങ്കിൽ { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

സ്ഥിതം size_t sizeof_union(ഘടന packed_git *p1, ഘടന packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	സ്ഥിരം unsigned അക്ഷരം *p1_base, *p2_base;
	സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	ആകുമ്പോൾ (p1_off < p1->അക്കം_objects * p1_step &&
	       p2_off < p2->അക്കം_objects * p2_step)
	{
		പൂർണ്ണസംഖ്യ cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		എങ്കിൽ (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			തുടരുക;
		}
		എങ്കിൽ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} അല്ലെങ്കിൽ { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	തിരികെ ret;
}

/* another O(n^2) function ... */
സ്ഥിതം size_t get_pack_redundancy(ഘടന pack_list *pl)
{
	ഘടന pack_list *subset;
	size_t ret = 0;

	എങ്കിൽ (!pl)
		തിരികെ 0;

	ആകുമ്പോൾ ((subset = pl->next)) {
		ആകുമ്പോൾ (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	തിരികെ ret;
}

സ്ഥിതം വരിയിൽ off_t pack_set_bytecount(ഘടന pack_list *pl)
{
	off_t ret = 0;
	ആകുമ്പോൾ (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmp_remaining_objects(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
	ഘടന pack_list *pl_a = *((ഘടന pack_list **)a);
	ഘടന pack_list *pl_b = *((ഘടന pack_list **)b);

	എങ്കിൽ (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		എങ്കിൽ (pl_a->all_objects_size == pl_b->all_objects_size)
			തിരികെ 0;
		അല്ലെങ്കിൽ എങ്കിൽ (pl_a->all_objects_size < pl_b->all_objects_size)
			തിരികെ 1;
		അല്ലെങ്കിൽ
			തിരികെ -1;
	} അല്ലെങ്കിൽ എങ്കിൽ (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		തിരികെ 1;
	} അല്ലെങ്കിൽ {
		തിരികെ -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
സ്ഥിതം ശൂന്യം sort_pack_list(ഘടന pack_list **pl)
{
	ഘടന pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	എങ്കിൽ (n < 2)
		തിരികെ;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	വേണ്ടി (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	വേണ്ടി (size_t i = 0; i < n - 1; i++)
		ary[i]->next = ary[i + 1];
	ary[n - 1]->next = ശൂന്യ;
	*pl = ary[0];

	സ്വതന്ത്രം(ary);
}


സ്ഥിതം ശൂന്യം minimize(ഘടന pack_list **min)
{
	ഘടന pack_list *pl, *unique = ശൂന്യ, *non_unique = ശൂന്യ;
	ഘടന llist *missing, *unique_pack_objects;

	pl = local_packs;
	ആകുമ്പോൾ (pl) {
		എങ്കിൽ (pl->unique_objects->size)
			pack_list_തിരുകുക(&unique, pl);
		അല്ലെങ്കിൽ
			pack_list_തിരുകുക(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	ആകുമ്പോൾ (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	എങ്കിൽ (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		തിരികെ;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	ആകുമ്പോൾ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	ആകുമ്പോൾ (non_unique) {
		ഘടന pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		എങ്കിൽ (non_unique->remaining_objects->size == 0)
			നിർത്തുക;

		pack_list_തിരുകുക(min, non_unique);

		വേണ്ടി (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		next = non_unique->next;
		സ്വതന്ത്രം(non_unique);
		non_unique = next;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

സ്ഥിതം ശൂന്യം load_all_objects(ശൂന്യം)
{
	ഘടന pack_list *pl = local_packs;
	ഘടന llist_item *hint, *l;

	llist_init(&all_objects);

	ആകുമ്പോൾ (pl) {
		hint = ശൂന്യ;
		l = pl->remaining_objects->front;
		ആകുമ്പോൾ (l) {
			hint = llist_തിരുകുക_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	ആകുമ്പോൾ (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
സ്ഥിതം ശൂന്യം cmp_local_packs(ശൂന്യം)
{
	ഘടന pack_list *subset, *pl = local_packs;

	/* only one packfile */
	എങ്കിൽ (!pl->next) {
		llist_init(&pl->unique_objects);
		തിരികെ;
	}

	ആകുമ്പോൾ ((subset = pl)) {
		ആകുമ്പോൾ ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

സ്ഥിതം ശൂന്യം scan_alt_odb_packs(ശൂന്യം)
{
	ഘടന pack_list *local, *alt;

	alt = altodb_packs;
	ആകുമ്പോൾ (alt) {
		local = local_packs;
		ആകുമ്പോൾ (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->next;
		}
		alt = alt->next;
	}
}

സ്ഥിതം ഘടന pack_list * add_pack(ഘടന packed_git *p)
{
	ഘടന pack_list l;
	size_t off = 0, step;
	സ്ഥിരം unsigned അക്ഷരം *base;

	എങ്കിൽ (!p->pack_local && !(alt_odb || verbose))
		തിരികെ ശൂന്യ;

	l.pack = p;
	llist_init(&l.remaining_objects);

	എങ്കിൽ (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		തിരികെ ശൂന്യ;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	ആകുമ്പോൾ (off < p->അക്കം_objects * step) {
		llist_തിരുകുക_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = ശൂന്യ;
	എങ്കിൽ (p->pack_local)
		തിരികെ pack_list_തിരുകുക(&local_packs, &l);
	അല്ലെങ്കിൽ
		തിരികെ pack_list_തിരുകുക(&altodb_packs, &l);
}

സ്ഥിതം ഘടന pack_list * add_pack_file(സ്ഥിരം അക്ഷരം *filename)
{
	ഘടന packed_git *p;

	എങ്കിൽ (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		എങ്കിൽ (strstr(p->pack_name, filename))
			തിരികെ add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

സ്ഥിതം ശൂന്യം load_all(ശൂന്യം)
{
	ഘടന packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

പൂർണ്ണസംഖ്യ cmd_pack_redundant(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix UNUSED, ഘടന repository *repo UNUSED) {
	പൂർണ്ണസംഖ്യ i; പൂർണ്ണസംഖ്യ i_still_use_this = 0; ഘടന pack_list *min = ശൂന്യ, *red, *pl;
	ഘടന llist *ignore;
	ഘടന strbuf idx_name = STRBUF_INIT;
	അക്ഷരം buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	വേണ്ടി (i = 1; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];
		എങ്കിൽ (!strcmp(arg, "--")) {
			i++;
			നിർത്തുക;
		}
		എങ്കിൽ (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			തുടരുക;
		}
		എങ്കിൽ (!strcmp(arg, "--verbose")) {
			verbose = 1;
			തുടരുക;
		}
		എങ്കിൽ (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			തുടരുക;
		}
		എങ്കിൽ (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			തുടരുക;
		}
		എങ്കിൽ (*arg == '-')
			usage(pack_redundant_usage);
		അല്ലെങ്കിൽ
			നിർത്തുക;
	}

	എങ്കിൽ (!i_still_use_this)
		you_still_use_that("git pack-redundant", ശൂന്യ);

	എങ്കിൽ (load_all_packs)
		load_all();
	അല്ലെങ്കിൽ
		ആകുമ്പോൾ (*(argv + i) != ശൂന്യ)
			add_pack_file(*(argv + i++));

	എങ്കിൽ (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	എങ്കിൽ (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	എങ്കിൽ (!isatty(0)) {
		ഘടന object_id oid;
		ആകുമ്പോൾ (fgets(buf, വലുപ്പം(buf), stdin)) {
			എങ്കിൽ (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_തിരുകുക_sorted_unique(ignore, &oid, ശൂന്യ);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	ആകുമ്പോൾ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	എങ്കിൽ (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		ആകുമ്പോൾ (pl) {
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
	ആകുമ്പോൾ (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	എങ്കിൽ (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	തിരികെ 0;
}
