/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"

#அந்தர்பாவ "packfile.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "strbuf.h"

#define BLKSIZE 512

நிலையான மாறிலி எழுத்து pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

நிலையான முழு load_all_packs, verbose, alt_odb;

கட்டமைப்பு llist_item {
	கட்டமைப்பு llist_item *next;
	கட்டமைப்பு object_id oid;
};
நிலையான கட்டமைப்பு llist {
	கட்டமைப்பு llist_item *front;
	கட்டமைப்பு llist_item *back;
	size_t size;
} *all_objects; /* all objects which must be present in local packfiles */

நிலையான கட்டமைப்பு pack_list {
	கட்டமைப்பு pack_list *next;
	கட்டமைப்பு packed_git *pack;
	கட்டமைப்பு llist *unique_objects;
	கட்டமைப்பு llist *remaining_objects;
	size_t all_objects_size;
} *local_packs = வெற்று, *altodb_packs = வெற்று;

நிலையான கட்டமைப்பு llist_item *free_nodes;

நிலையான உள்வரி வெறுமை llist_item_put(கட்டமைப்பு llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

நிலையான உள்வரி கட்டமைப்பு llist_item *llist_item_get(வெறுமை)
{
	கட்டமைப்பு llist_item *new_item;
	எனில் ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} இல்லையெனில் {
		முழு i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		க்கு (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	திரும்பு new_item;
}

நிலையான உள்வரி வெறுமை llist_init(கட்டமைப்பு llist **list)
{
	*list = xmalloc(அளவு(கட்டமைப்பு llist));
	(*list)->front = (*list)->back = வெற்று;
	(*list)->size = 0;
}

நிலையான வெறுமை llist_free(கட்டமைப்பு llist *list)
{
	க்கு (கட்டமைப்பு llist_item *i = list->front, *next; i; i = next) {
		next = i->next;
		llist_item_put(i);
	}
	விடுவி(list);
}

நிலையான கட்டமைப்பு llist * llist_copy(கட்டமைப்பு llist *list)
{
	கட்டமைப்பு llist *ret;
	கட்டமைப்பு llist_item *new_item, *old_item, *prev;

	llist_init(&ret);

	எனில் ((ret->size = list->size) == 0)
		திரும்பு ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	போது (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = வெற்று;
	ret->back = new_item;

	திரும்பு ret;
}

நிலையான உள்வரி கட்டமைப்பு llist_item *llist_செருகு(கட்டமைப்பு llist *list,
					      கட்டமைப்பு llist_item *after,
					      மாறிலி unsigned எழுத்து *oid)
{
	கட்டமைப்பு llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = வெற்று;

	எனில் (after) {
		new_item->next = after->next;
		after->next = new_item;
		எனில் (after == list->back)
			list->back = new_item;
	} இல்லையெனில் {/* insert in front */
		எனில் (list->size == 0)
			list->back = new_item;
		இல்லையெனில்
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	திரும்பு new_item;
}

நிலையான உள்வரி கட்டமைப்பு llist_item *llist_செருகு_back(கட்டமைப்பு llist *list,
						   மாறிலி unsigned எழுத்து *oid)
{
	திரும்பு llist_செருகு(list, list->back, oid);
}

நிலையான உள்வரி கட்டமைப்பு llist_item *llist_செருகு_sorted_unique(கட்டமைப்பு llist *list,
			மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு llist_item *hint)
{
	கட்டமைப்பு llist_item *prev = வெற்று, *l;

	l = (hint == வெற்று) ? list->front : hint;
	போது (l) {
		முழு cmp = oidcmp(&l->oid, oid);
		எனில் (cmp > 0) { /* we insert before this entry */
			திரும்பு llist_செருகு(list, prev, oid->hash);
		}
		எனில் (!cmp) { /* already exists */
			திரும்பு l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	திரும்பு llist_செருகு_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
நிலையான உள்வரி கட்டமைப்பு llist_item * llist_sorted_remove(கட்டமைப்பு llist *list, மாறிலி unsigned எழுத்து *oid, கட்டமைப்பு llist_item *hint)
{
	கட்டமைப்பு llist_item *prev, *l;

redo_from_start:
	l = (hint == வெற்று) ? list->front : hint;
	prev = வெற்று;
	போது (l) {
		மாறிலி முழு cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		எனில் (cmp > 0) /* not in list, since sorted */
			திரும்பு prev;
		எனில் (!cmp) { /* found */
			எனில் (!prev) {
				எனில் (hint != வெற்று && hint != list->front) {
					/* we don't know the previous element */
					hint = வெற்று;
					செல் redo_from_start;
				}
				list->front = l->next;
			} இல்லையெனில்
				prev->next = l->next;
			எனில் (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			திரும்பு prev;
		}
		prev = l;
		l = l->next;
	}
	திரும்பு prev;
}

/* computes A\B */
நிலையான வெறுமை llist_sorted_difference_inplace(கட்டமைப்பு llist *A,
				     கட்டமைப்பு llist *B)
{
	கட்டமைப்பு llist_item *hint, *b;

	hint = வெற்று;
	b = B->front;

	போது (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

நிலையான உள்வரி கட்டமைப்பு pack_list * pack_list_செருகு(கட்டமைப்பு pack_list **pl,
					   கட்டமைப்பு pack_list *entry)
{
	கட்டமைப்பு pack_list *p = xmalloc(அளவு(கட்டமைப்பு pack_list));
	memcpy(p, entry, அளவு(கட்டமைப்பு pack_list));
	p->next = *pl;
	*pl = p;
	திரும்பு p;
}

நிலையான வெறுமை pack_list_free(கட்டமைப்பு pack_list *pl)
{
	க்கு (கட்டமைப்பு pack_list *next; pl; pl = next) {
		next = pl->next;
		விடுவி(pl);
	}
}

நிலையான உள்வரி size_t pack_list_size(கட்டமைப்பு pack_list *pl)
{
	size_t ret = 0;
	போது (pl) {
		ret++;
		pl = pl->next;
	}
	திரும்பு ret;
}

நிலையான கட்டமைப்பு pack_list * pack_list_difference(மாறிலி கட்டமைப்பு pack_list *A,
					       மாறிலி கட்டமைப்பு pack_list *B)
{
	கட்டமைப்பு pack_list *ret;
	மாறிலி கட்டமைப்பு pack_list *pl;

	எனில் (!A)
		திரும்பு வெற்று;

	pl = B;
	போது (pl != வெற்று) {
		எனில் (A->pack == pl->pack)
			திரும்பு pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(அளவு(கட்டமைப்பு pack_list));
	memcpy(ret, A, அளவு(கட்டமைப்பு pack_list));
	ret->next = pack_list_difference(A->next, B);
	திரும்பு ret;
}

நிலையான வெறுமை cmp_two_packs(கட்டமைப்பு pack_list *p1, கட்டமைப்பு pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	மாறிலி unsigned எழுத்து *p1_base, *p2_base;
	கட்டமைப்பு llist_item *p1_hint = வெற்று, *p2_hint = வெற்று;
	மாறிலி unsigned முழு hashsz = the_hash_algo->rawsz;

	எனில் (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	எனில் (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	போது (p1_off < p1->pack->இலக்கம்_objects * p1_step &&
	       p2_off < p2->pack->இலக்கம்_objects * p2_step)
	{
		மாறிலி முழு cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		எனில் (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			தொடர்;
		}
		எனில் (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} இல்லையெனில் { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

நிலையான size_t sizeof_union(கட்டமைப்பு packed_git *p1, கட்டமைப்பு packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	மாறிலி unsigned எழுத்து *p1_base, *p2_base;
	மாறிலி unsigned முழு hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	போது (p1_off < p1->இலக்கம்_objects * p1_step &&
	       p2_off < p2->இலக்கம்_objects * p2_step)
	{
		முழு cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		எனில் (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			தொடர்;
		}
		எனில் (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} இல்லையெனில் { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	திரும்பு ret;
}

/* another O(n^2) function ... */
நிலையான size_t get_pack_redundancy(கட்டமைப்பு pack_list *pl)
{
	கட்டமைப்பு pack_list *subset;
	size_t ret = 0;

	எனில் (!pl)
		திரும்பு 0;

	போது ((subset = pl->next)) {
		போது (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	திரும்பு ret;
}

நிலையான உள்வரி off_t pack_set_bytecount(கட்டமைப்பு pack_list *pl)
{
	off_t ret = 0;
	போது (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	திரும்பு ret;
}

நிலையான முழு cmp_remaining_objects(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
	கட்டமைப்பு pack_list *pl_a = *((கட்டமைப்பு pack_list **)a);
	கட்டமைப்பு pack_list *pl_b = *((கட்டமைப்பு pack_list **)b);

	எனில் (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		எனில் (pl_a->all_objects_size == pl_b->all_objects_size)
			திரும்பு 0;
		இல்லையெனில் எனில் (pl_a->all_objects_size < pl_b->all_objects_size)
			திரும்பு 1;
		இல்லையெனில்
			திரும்பு -1;
	} இல்லையெனில் எனில் (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		திரும்பு 1;
	} இல்லையெனில் {
		திரும்பு -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
நிலையான வெறுமை sort_pack_list(கட்டமைப்பு pack_list **pl)
{
	கட்டமைப்பு pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	எனில் (n < 2)
		திரும்பு;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	க்கு (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	க்கு (size_t i = 0; i < n - 1; i++)
		ary[i]->next = ary[i + 1];
	ary[n - 1]->next = வெற்று;
	*pl = ary[0];

	விடுவி(ary);
}


நிலையான வெறுமை minimize(கட்டமைப்பு pack_list **min)
{
	கட்டமைப்பு pack_list *pl, *unique = வெற்று, *non_unique = வெற்று;
	கட்டமைப்பு llist *missing, *unique_pack_objects;

	pl = local_packs;
	போது (pl) {
		எனில் (pl->unique_objects->size)
			pack_list_செருகு(&unique, pl);
		இல்லையெனில்
			pack_list_செருகு(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	போது (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	எனில் (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		திரும்பு;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	போது (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	போது (non_unique) {
		கட்டமைப்பு pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		எனில் (non_unique->remaining_objects->size == 0)
			நிறுத்து;

		pack_list_செருகு(min, non_unique);

		க்கு (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		next = non_unique->next;
		விடுவி(non_unique);
		non_unique = next;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

நிலையான வெறுமை load_all_objects(வெறுமை)
{
	கட்டமைப்பு pack_list *pl = local_packs;
	கட்டமைப்பு llist_item *hint, *l;

	llist_init(&all_objects);

	போது (pl) {
		hint = வெற்று;
		l = pl->remaining_objects->front;
		போது (l) {
			hint = llist_செருகு_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	போது (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
நிலையான வெறுமை cmp_local_packs(வெறுமை)
{
	கட்டமைப்பு pack_list *subset, *pl = local_packs;

	/* only one packfile */
	எனில் (!pl->next) {
		llist_init(&pl->unique_objects);
		திரும்பு;
	}

	போது ((subset = pl)) {
		போது ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

நிலையான வெறுமை scan_alt_odb_packs(வெறுமை)
{
	கட்டமைப்பு pack_list *local, *alt;

	alt = altodb_packs;
	போது (alt) {
		local = local_packs;
		போது (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->next;
		}
		alt = alt->next;
	}
}

நிலையான கட்டமைப்பு pack_list * add_pack(கட்டமைப்பு packed_git *p)
{
	கட்டமைப்பு pack_list l;
	size_t off = 0, step;
	மாறிலி unsigned எழுத்து *base;

	எனில் (!p->pack_local && !(alt_odb || verbose))
		திரும்பு வெற்று;

	l.pack = p;
	llist_init(&l.remaining_objects);

	எனில் (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		திரும்பு வெற்று;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	போது (off < p->இலக்கம்_objects * step) {
		llist_செருகு_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = வெற்று;
	எனில் (p->pack_local)
		திரும்பு pack_list_செருகு(&local_packs, &l);
	இல்லையெனில்
		திரும்பு pack_list_செருகு(&altodb_packs, &l);
}

நிலையான கட்டமைப்பு pack_list * add_pack_file(மாறிலி எழுத்து *filename)
{
	கட்டமைப்பு packed_git *p;

	எனில் (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		எனில் (strstr(p->pack_name, filename))
			திரும்பு add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

நிலையான வெறுமை load_all(வெறுமை)
{
	கட்டமைப்பு packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

முழு cmd_pack_redundant(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix UNUSED, கட்டமைப்பு repository *repo UNUSED) {
	முழு i; முழு i_still_use_this = 0; கட்டமைப்பு pack_list *min = வெற்று, *red, *pl;
	கட்டமைப்பு llist *ignore;
	கட்டமைப்பு strbuf idx_name = STRBUF_INIT;
	எழுத்து buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	க்கு (i = 1; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];
		எனில் (!strcmp(arg, "--")) {
			i++;
			நிறுத்து;
		}
		எனில் (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			தொடர்;
		}
		எனில் (!strcmp(arg, "--verbose")) {
			verbose = 1;
			தொடர்;
		}
		எனில் (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			தொடர்;
		}
		எனில் (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			தொடர்;
		}
		எனில் (*arg == '-')
			usage(pack_redundant_usage);
		இல்லையெனில்
			நிறுத்து;
	}

	எனில் (!i_still_use_this)
		you_still_use_that("git pack-redundant", வெற்று);

	எனில் (load_all_packs)
		load_all();
	இல்லையெனில்
		போது (*(argv + i) != வெற்று)
			add_pack_file(*(argv + i++));

	எனில் (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	எனில் (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	எனில் (!isatty(0)) {
		கட்டமைப்பு object_id oid;
		போது (fgets(buf, அளவு(buf), stdin)) {
			எனில் (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_செருகு_sorted_unique(ignore, &oid, வெற்று);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	போது (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	எனில் (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		போது (pl) {
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
	போது (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	எனில் (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	திரும்பு 0;
}
