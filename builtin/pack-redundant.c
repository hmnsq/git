/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"

#অন্তর্ভাব "packfile.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "strbuf.h"

#define BLKSIZE 512

স্থির ধ্রুব বর্ণ pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

স্থির পূর্ণাঙ্ক load_all_packs, verbose, alt_odb;

সংরচনা llist_item {
	সংরচনা llist_item *next;
	সংরচনা object_id oid;
};
স্থির সংরচনা llist {
	সংরচনা llist_item *front;
	সংরচনা llist_item *back;
	size_t size;
} *all_objects; /* all objects which must be present in local packfiles */

স্থির সংরচনা pack_list {
	সংরচনা pack_list *next;
	সংরচনা packed_git *pack;
	সংরচনা llist *unique_objects;
	সংরচনা llist *remaining_objects;
	size_t all_objects_size;
} *local_packs = রিক্ত, *altodb_packs = রিক্ত;

স্থির সংরচনা llist_item *free_nodes;

স্থির অন্তঃস্থ শূন্য llist_item_put(সংরচনা llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

স্থির অন্তঃস্থ সংরচনা llist_item *llist_item_get(শূন্য)
{
	সংরচনা llist_item *new_item;
	যদি ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} অন্যথা {
		পূর্ণাঙ্ক i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		প্রতি (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	প্রত্যাগম new_item;
}

স্থির অন্তঃস্থ শূন্য llist_init(সংরচনা llist **list)
{
	*list = xmalloc(পরিমাণ(সংরচনা llist));
	(*list)->front = (*list)->back = রিক্ত;
	(*list)->size = 0;
}

স্থির শূন্য llist_free(সংরচনা llist *list)
{
	প্রতি (সংরচনা llist_item *i = list->front, *next; i; i = next) {
		next = i->next;
		llist_item_put(i);
	}
	মুক্ত(list);
}

স্থির সংরচনা llist * llist_copy(সংরচনা llist *list)
{
	সংরচনা llist *ret;
	সংরচনা llist_item *new_item, *old_item, *prev;

	llist_init(&ret);

	যদি ((ret->size = list->size) == 0)
		প্রত্যাগম ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	যাবত্ (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = রিক্ত;
	ret->back = new_item;

	প্রত্যাগম ret;
}

স্থির অন্তঃস্থ সংরচনা llist_item *llist_ঢোকাও(সংরচনা llist *list,
					      সংরচনা llist_item *after,
					      ধ্রুব unsigned বর্ণ *oid)
{
	সংরচনা llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = রিক্ত;

	যদি (after) {
		new_item->next = after->next;
		after->next = new_item;
		যদি (after == list->back)
			list->back = new_item;
	} অন্যথা {/* insert in front */
		যদি (list->size == 0)
			list->back = new_item;
		অন্যথা
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	প্রত্যাগম new_item;
}

স্থির অন্তঃস্থ সংরচনা llist_item *llist_ঢোকাও_back(সংরচনা llist *list,
						   ধ্রুব unsigned বর্ণ *oid)
{
	প্রত্যাগম llist_ঢোকাও(list, list->back, oid);
}

স্থির অন্তঃস্থ সংরচনা llist_item *llist_ঢোকাও_sorted_unique(সংরচনা llist *list,
			ধ্রুব সংরচনা object_id *oid, সংরচনা llist_item *hint)
{
	সংরচনা llist_item *prev = রিক্ত, *l;

	l = (hint == রিক্ত) ? list->front : hint;
	যাবত্ (l) {
		পূর্ণাঙ্ক cmp = oidcmp(&l->oid, oid);
		যদি (cmp > 0) { /* we insert before this entry */
			প্রত্যাগম llist_ঢোকাও(list, prev, oid->hash);
		}
		যদি (!cmp) { /* already exists */
			প্রত্যাগম l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	প্রত্যাগম llist_ঢোকাও_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
স্থির অন্তঃস্থ সংরচনা llist_item * llist_sorted_remove(সংরচনা llist *list, ধ্রুব unsigned বর্ণ *oid, সংরচনা llist_item *hint)
{
	সংরচনা llist_item *prev, *l;

redo_from_start:
	l = (hint == রিক্ত) ? list->front : hint;
	prev = রিক্ত;
	যাবত্ (l) {
		ধ্রুব পূর্ণাঙ্ক cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		যদি (cmp > 0) /* not in list, since sorted */
			প্রত্যাগম prev;
		যদি (!cmp) { /* found */
			যদি (!prev) {
				যদি (hint != রিক্ত && hint != list->front) {
					/* we don't know the previous element */
					hint = রিক্ত;
					গচ্ছ redo_from_start;
				}
				list->front = l->next;
			} অন্যথা
				prev->next = l->next;
			যদি (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			প্রত্যাগম prev;
		}
		prev = l;
		l = l->next;
	}
	প্রত্যাগম prev;
}

/* computes A\B */
স্থির শূন্য llist_sorted_difference_inplace(সংরচনা llist *A,
				     সংরচনা llist *B)
{
	সংরচনা llist_item *hint, *b;

	hint = রিক্ত;
	b = B->front;

	যাবত্ (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

স্থির অন্তঃস্থ সংরচনা pack_list * pack_list_ঢোকাও(সংরচনা pack_list **pl,
					   সংরচনা pack_list *entry)
{
	সংরচনা pack_list *p = xmalloc(পরিমাণ(সংরচনা pack_list));
	memcpy(p, entry, পরিমাণ(সংরচনা pack_list));
	p->next = *pl;
	*pl = p;
	প্রত্যাগম p;
}

স্থির শূন্য pack_list_free(সংরচনা pack_list *pl)
{
	প্রতি (সংরচনা pack_list *next; pl; pl = next) {
		next = pl->next;
		মুক্ত(pl);
	}
}

স্থির অন্তঃস্থ size_t pack_list_size(সংরচনা pack_list *pl)
{
	size_t ret = 0;
	যাবত্ (pl) {
		ret++;
		pl = pl->next;
	}
	প্রত্যাগম ret;
}

স্থির সংরচনা pack_list * pack_list_difference(ধ্রুব সংরচনা pack_list *A,
					       ধ্রুব সংরচনা pack_list *B)
{
	সংরচনা pack_list *ret;
	ধ্রুব সংরচনা pack_list *pl;

	যদি (!A)
		প্রত্যাগম রিক্ত;

	pl = B;
	যাবত্ (pl != রিক্ত) {
		যদি (A->pack == pl->pack)
			প্রত্যাগম pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(পরিমাণ(সংরচনা pack_list));
	memcpy(ret, A, পরিমাণ(সংরচনা pack_list));
	ret->next = pack_list_difference(A->next, B);
	প্রত্যাগম ret;
}

স্থির শূন্য cmp_two_packs(সংরচনা pack_list *p1, সংরচনা pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ধ্রুব unsigned বর্ণ *p1_base, *p2_base;
	সংরচনা llist_item *p1_hint = রিক্ত, *p2_hint = রিক্ত;
	ধ্রুব unsigned পূর্ণাঙ্ক hashsz = the_hash_algo->rawsz;

	যদি (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	যদি (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	যাবত্ (p1_off < p1->pack->অঙ্ক_objects * p1_step &&
	       p2_off < p2->pack->অঙ্ক_objects * p2_step)
	{
		ধ্রুব পূর্ণাঙ্ক cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		যদি (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			অনুবর্ত;
		}
		যদি (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} অন্যথা { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

স্থির size_t sizeof_union(সংরচনা packed_git *p1, সংরচনা packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ধ্রুব unsigned বর্ণ *p1_base, *p2_base;
	ধ্রুব unsigned পূর্ণাঙ্ক hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	যাবত্ (p1_off < p1->অঙ্ক_objects * p1_step &&
	       p2_off < p2->অঙ্ক_objects * p2_step)
	{
		পূর্ণাঙ্ক cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		যদি (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			অনুবর্ত;
		}
		যদি (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} অন্যথা { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	প্রত্যাগম ret;
}

/* another O(n^2) function ... */
স্থির size_t get_pack_redundancy(সংরচনা pack_list *pl)
{
	সংরচনা pack_list *subset;
	size_t ret = 0;

	যদি (!pl)
		প্রত্যাগম 0;

	যাবত্ ((subset = pl->next)) {
		যাবত্ (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	প্রত্যাগম ret;
}

স্থির অন্তঃস্থ off_t pack_set_bytecount(সংরচনা pack_list *pl)
{
	off_t ret = 0;
	যাবত্ (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক cmp_remaining_objects(ধ্রুব শূন্য *a, ধ্রুব শূন্য *b)
{
	সংরচনা pack_list *pl_a = *((সংরচনা pack_list **)a);
	সংরচনা pack_list *pl_b = *((সংরচনা pack_list **)b);

	যদি (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		যদি (pl_a->all_objects_size == pl_b->all_objects_size)
			প্রত্যাগম 0;
		অন্যথা যদি (pl_a->all_objects_size < pl_b->all_objects_size)
			প্রত্যাগম 1;
		অন্যথা
			প্রত্যাগম -1;
	} অন্যথা যদি (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		প্রত্যাগম 1;
	} অন্যথা {
		প্রত্যাগম -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
স্থির শূন্য sort_pack_list(সংরচনা pack_list **pl)
{
	সংরচনা pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	যদি (n < 2)
		প্রত্যাগম;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	প্রতি (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	প্রতি (size_t i = 0; i < n - 1; i++)
		ary[i]->next = ary[i + 1];
	ary[n - 1]->next = রিক্ত;
	*pl = ary[0];

	মুক্ত(ary);
}


স্থির শূন্য minimize(সংরচনা pack_list **min)
{
	সংরচনা pack_list *pl, *unique = রিক্ত, *non_unique = রিক্ত;
	সংরচনা llist *missing, *unique_pack_objects;

	pl = local_packs;
	যাবত্ (pl) {
		যদি (pl->unique_objects->size)
			pack_list_ঢোকাও(&unique, pl);
		অন্যথা
			pack_list_ঢোকাও(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	যাবত্ (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	যদি (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		প্রত্যাগম;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	যাবত্ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	যাবত্ (non_unique) {
		সংরচনা pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		যদি (non_unique->remaining_objects->size == 0)
			ভঙ্গ;

		pack_list_ঢোকাও(min, non_unique);

		প্রতি (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		next = non_unique->next;
		মুক্ত(non_unique);
		non_unique = next;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

স্থির শূন্য load_all_objects(শূন্য)
{
	সংরচনা pack_list *pl = local_packs;
	সংরচনা llist_item *hint, *l;

	llist_init(&all_objects);

	যাবত্ (pl) {
		hint = রিক্ত;
		l = pl->remaining_objects->front;
		যাবত্ (l) {
			hint = llist_ঢোকাও_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	যাবত্ (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
স্থির শূন্য cmp_local_packs(শূন্য)
{
	সংরচনা pack_list *subset, *pl = local_packs;

	/* only one packfile */
	যদি (!pl->next) {
		llist_init(&pl->unique_objects);
		প্রত্যাগম;
	}

	যাবত্ ((subset = pl)) {
		যাবত্ ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

স্থির শূন্য scan_alt_odb_packs(শূন্য)
{
	সংরচনা pack_list *local, *alt;

	alt = altodb_packs;
	যাবত্ (alt) {
		local = local_packs;
		যাবত্ (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->next;
		}
		alt = alt->next;
	}
}

স্থির সংরচনা pack_list * add_pack(সংরচনা packed_git *p)
{
	সংরচনা pack_list l;
	size_t off = 0, step;
	ধ্রুব unsigned বর্ণ *base;

	যদি (!p->pack_local && !(alt_odb || verbose))
		প্রত্যাগম রিক্ত;

	l.pack = p;
	llist_init(&l.remaining_objects);

	যদি (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		প্রত্যাগম রিক্ত;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	যাবত্ (off < p->অঙ্ক_objects * step) {
		llist_ঢোকাও_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = রিক্ত;
	যদি (p->pack_local)
		প্রত্যাগম pack_list_ঢোকাও(&local_packs, &l);
	অন্যথা
		প্রত্যাগম pack_list_ঢোকাও(&altodb_packs, &l);
}

স্থির সংরচনা pack_list * add_pack_file(ধ্রুব বর্ণ *filename)
{
	সংরচনা packed_git *p;

	যদি (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		যদি (strstr(p->pack_name, filename))
			প্রত্যাগম add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

স্থির শূন্য load_all(শূন্য)
{
	সংরচনা packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

পূর্ণাঙ্ক cmd_pack_redundant(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix UNUSED, সংরচনা repository *repo UNUSED) {
	পূর্ণাঙ্ক i; পূর্ণাঙ্ক i_still_use_this = 0; সংরচনা pack_list *min = রিক্ত, *red, *pl;
	সংরচনা llist *ignore;
	সংরচনা strbuf idx_name = STRBUF_INIT;
	বর্ণ buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	প্রতি (i = 1; i < argc; i++) {
		ধ্রুব বর্ণ *arg = argv[i];
		যদি (!strcmp(arg, "--")) {
			i++;
			ভঙ্গ;
		}
		যদি (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			অনুবর্ত;
		}
		যদি (!strcmp(arg, "--verbose")) {
			verbose = 1;
			অনুবর্ত;
		}
		যদি (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			অনুবর্ত;
		}
		যদি (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			অনুবর্ত;
		}
		যদি (*arg == '-')
			usage(pack_redundant_usage);
		অন্যথা
			ভঙ্গ;
	}

	যদি (!i_still_use_this)
		you_still_use_that("git pack-redundant", রিক্ত);

	যদি (load_all_packs)
		load_all();
	অন্যথা
		যাবত্ (*(argv + i) != রিক্ত)
			add_pack_file(*(argv + i++));

	যদি (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	যদি (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	যদি (!isatty(0)) {
		সংরচনা object_id oid;
		যাবত্ (fgets(buf, পরিমাণ(buf), stdin)) {
			যদি (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_ঢোকাও_sorted_unique(ignore, &oid, রিক্ত);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	যাবত্ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	যদি (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		যাবত্ (pl) {
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
	যাবত্ (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	যদি (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	প্রত্যাগম 0;
}
