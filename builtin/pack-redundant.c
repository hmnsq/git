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

স্থির ধ্রুবক অক্ষর pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

স্থির পূর্ণসংখ্যা load_all_packs, verbose, alt_odb;

কাঠামো llist_item {
	কাঠামো llist_item *পরবর্তী;
	কাঠামো object_id oid;
};
স্থির কাঠামো llist {
	কাঠামো llist_item *front;
	কাঠামো llist_item *back;
	আকার_t আকার_2;
} *all_objects; /* all objects which must be present in local packfiles */

স্থির কাঠামো pack_list {
	কাঠামো pack_list *পরবর্তী;
	কাঠামো packed_git *pack;
	কাঠামো llist *unique_objects;
	কাঠামো llist *remaining_objects;
	আকার_t all_objects_আকার;
} *local_packs = ফাঁকা, *altodb_packs = ফাঁকা;

স্থির কাঠামো llist_item *free_nodes;

স্থির অন্তঃরেখ শূন্য llist_item_put(কাঠামো llist_item *item)
{
	item->পরবর্তী = free_nodes;
	free_nodes = item;
}

স্থির অন্তঃরেখ কাঠামো llist_item *llist_item_get(শূন্য)
{
	কাঠামো llist_item *new_item;
	যদি ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->পরবর্তী;
	} অন্যথা {
		পূর্ণসংখ্যা i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		জন্য (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	ফেরত new_item;
}

স্থির অন্তঃরেখ শূন্য llist_init(কাঠামো llist **list)
{
	*list = xmalloc(আকার(কাঠামো llist));
	(*list)->front = (*list)->back = ফাঁকা;
	(*list)->আকার_2 = 0;
}

স্থির শূন্য llist_free(কাঠামো llist *list)
{
	জন্য (কাঠামো llist_item *i = list->front, *পরবর্তী; i; i = পরবর্তী) {
		পরবর্তী = i->পরবর্তী;
		llist_item_put(i);
	}
	মুক্ত(list);
}

স্থির কাঠামো llist * llist_copy(কাঠামো llist *list)
{
	কাঠামো llist *ret;
	কাঠামো llist_item *new_item, *old_item, *পূর্ব;

	llist_init(&ret);

	যদি ((ret->আকার_2 = list->আকার_2) == 0)
		ফেরত ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->পরবর্তী;
	যখন (old_item) {
		পূর্ব = new_item;
		new_item = llist_item_get();
		পূর্ব->পরবর্তী = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->পরবর্তী;
	}
	new_item->পরবর্তী = ফাঁকা;
	ret->back = new_item;

	ফেরত ret;
}

স্থির অন্তঃরেখ কাঠামো llist_item *llist_ঢোকাও(কাঠামো llist *list,
					      কাঠামো llist_item *after,
					      ধ্রুবক unsigned অক্ষর *oid)
{
	কাঠামো llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->পরবর্তী = ফাঁকা;

	যদি (after) {
		new_item->পরবর্তী = after->পরবর্তী;
		after->পরবর্তী = new_item;
		যদি (after == list->back)
			list->back = new_item;
	} অন্যথা {/* insert in front */
		যদি (list->আকার_2 == 0)
			list->back = new_item;
		অন্যথা
			new_item->পরবর্তী = list->front;
		list->front = new_item;
	}
	list->আকার_2++;
	ফেরত new_item;
}

স্থির অন্তঃরেখ কাঠামো llist_item *llist_ঢোকাও_back(কাঠামো llist *list,
						   ধ্রুবক unsigned অক্ষর *oid)
{
	ফেরত llist_ঢোকাও(list, list->back, oid);
}

স্থির অন্তঃরেখ কাঠামো llist_item *llist_ঢোকাও_sorted_unique(কাঠামো llist *list,
			ধ্রুবক কাঠামো object_id *oid, কাঠামো llist_item *hint)
{
	কাঠামো llist_item *পূর্ব = ফাঁকা, *l;

	l = (hint == ফাঁকা) ? list->front : hint;
	যখন (l) {
		পূর্ণসংখ্যা cmp = oidcmp(&l->oid, oid);
		যদি (cmp > 0) { /* we insert before this entry */
			ফেরত llist_ঢোকাও(list, পূর্ব, oid->hash);
		}
		যদি (!cmp) { /* already exists */
			ফেরত l;
		}
		পূর্ব = l;
		l = l->পরবর্তী;
	}
	/* insert at the end */
	ফেরত llist_ঢোকাও_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
স্থির অন্তঃরেখ কাঠামো llist_item * llist_sorted_remove(কাঠামো llist *list, ধ্রুবক unsigned অক্ষর *oid, কাঠামো llist_item *hint)
{
	কাঠামো llist_item *পূর্ব, *l;

redo_from_start:
	l = (hint == ফাঁকা) ? list->front : hint;
	পূর্ব = ফাঁকা;
	যখন (l) {
		ধ্রুবক পূর্ণসংখ্যা cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		যদি (cmp > 0) /* not in list, since sorted */
			ফেরত পূর্ব;
		যদি (!cmp) { /* found */
			যদি (!পূর্ব) {
				যদি (hint != ফাঁকা && hint != list->front) {
					/* we don't know the previous element */
					hint = ফাঁকা;
					যাও redo_from_start;
				}
				list->front = l->পরবর্তী;
			} অন্যথা
				পূর্ব->পরবর্তী = l->পরবর্তী;
			যদি (l == list->back)
				list->back = পূর্ব;
			llist_item_put(l);
			list->আকার_2--;
			ফেরত পূর্ব;
		}
		পূর্ব = l;
		l = l->পরবর্তী;
	}
	ফেরত পূর্ব;
}

/* computes A\B */
স্থির শূন্য llist_sorted_difference_inplace(কাঠামো llist *A,
				     কাঠামো llist *B)
{
	কাঠামো llist_item *hint, *b;

	hint = ফাঁকা;
	b = B->front;

	যখন (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->পরবর্তী;
	}
}

স্থির অন্তঃরেখ কাঠামো pack_list * pack_list_ঢোকাও(কাঠামো pack_list **pl,
					   কাঠামো pack_list *entry)
{
	কাঠামো pack_list *p = xmalloc(আকার(কাঠামো pack_list));
	memcpy(p, entry, আকার(কাঠামো pack_list));
	p->পরবর্তী = *pl;
	*pl = p;
	ফেরত p;
}

স্থির শূন্য pack_list_free(কাঠামো pack_list *pl)
{
	জন্য (কাঠামো pack_list *পরবর্তী; pl; pl = পরবর্তী) {
		পরবর্তী = pl->পরবর্তী;
		মুক্ত(pl);
	}
}

স্থির অন্তঃরেখ আকার_t pack_list_আকার(কাঠামো pack_list *pl)
{
	আকার_t ret = 0;
	যখন (pl) {
		ret++;
		pl = pl->পরবর্তী;
	}
	ফেরত ret;
}

স্থির কাঠামো pack_list * pack_list_difference(ধ্রুবক কাঠামো pack_list *A,
					       ধ্রুবক কাঠামো pack_list *B)
{
	কাঠামো pack_list *ret;
	ধ্রুবক কাঠামো pack_list *pl;

	যদি (!A)
		ফেরত ফাঁকা;

	pl = B;
	যখন (pl != ফাঁকা) {
		যদি (A->pack == pl->pack)
			ফেরত pack_list_difference(A->পরবর্তী, B);
		pl = pl->পরবর্তী;
	}
	ret = xmalloc(আকার(কাঠামো pack_list));
	memcpy(ret, A, আকার(কাঠামো pack_list));
	ret->পরবর্তী = pack_list_difference(A->পরবর্তী, B);
	ফেরত ret;
}

স্থির শূন্য cmp_two_packs(কাঠামো pack_list *p1, কাঠামো pack_list *p2)
{
	আকার_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ধ্রুবক unsigned অক্ষর *p1_base, *p2_base;
	কাঠামো llist_item *p1_hint = ফাঁকা, *p2_hint = ফাঁকা;
	ধ্রুবক unsigned পূর্ণসংখ্যা hashsz = the_hash_algo->rawsz;

	যদি (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	যদি (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->সূচক_তথ্য;
	p2_base = p2->pack->সূচক_তথ্য;
	p1_base += 256 * 4 + ((p1->pack->সূচক_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->সূচক_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->সূচক_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->সূচক_version < 2) ? 4 : 0);

	যখন (p1_off < p1->pack->অঙ্ক_objects * p1_step &&
	       p2_off < p2->pack->অঙ্ক_objects * p2_step)
	{
		ধ্রুবক পূর্ণসংখ্যা cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
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
			চালু;
		}
		যদি (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} অন্যথা { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

স্থির আকার_t sizeof_union(কাঠামো packed_git *p1, কাঠামো packed_git *p2)
{
	আকার_t ret = 0;
	আকার_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ধ্রুবক unsigned অক্ষর *p1_base, *p2_base;
	ধ্রুবক unsigned পূর্ণসংখ্যা hashsz = the_hash_algo->rawsz;

	p1_base = p1->সূচক_তথ্য;
	p2_base = p2->সূচক_তথ্য;
	p1_base += 256 * 4 + ((p1->সূচক_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->সূচক_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->সূচক_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->সূচক_version < 2) ? 4 : 0);

	যখন (p1_off < p1->অঙ্ক_objects * p1_step &&
	       p2_off < p2->অঙ্ক_objects * p2_step)
	{
		পূর্ণসংখ্যা cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		যদি (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			চালু;
		}
		যদি (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} অন্যথা { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	ফেরত ret;
}

/* another O(n^2) function ... */
স্থির আকার_t get_pack_redundancy(কাঠামো pack_list *pl)
{
	কাঠামো pack_list *subset;
	আকার_t ret = 0;

	যদি (!pl)
		ফেরত 0;

	যখন ((subset = pl->পরবর্তী)) {
		যখন (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->পরবর্তী;
		}
		pl = pl->পরবর্তী;
	}
	ফেরত ret;
}

স্থির অন্তঃরেখ off_t pack_set_bytecount(কাঠামো pack_list *pl)
{
	off_t ret = 0;
	যখন (pl) {
		ret += pl->pack->pack_আকার;
		ret += pl->pack->সূচক_আকার;
		pl = pl->পরবর্তী;
	}
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা cmp_remaining_objects(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
{
	কাঠামো pack_list *pl_a = *((কাঠামো pack_list **)a);
	কাঠামো pack_list *pl_b = *((কাঠামো pack_list **)b);

	যদি (pl_a->remaining_objects->আকার_2 == pl_b->remaining_objects->আকার_2) {
		/* have the same remaining_objects, big pack first */
		যদি (pl_a->all_objects_আকার == pl_b->all_objects_আকার)
			ফেরত 0;
		অন্যথা যদি (pl_a->all_objects_আকার < pl_b->all_objects_আকার)
			ফেরত 1;
		অন্যথা
			ফেরত -1;
	} অন্যথা যদি (pl_a->remaining_objects->আকার_2 < pl_b->remaining_objects->আকার_2) {
		/* sort by remaining objects, more objects first */
		ফেরত 1;
	} অন্যথা {
		ফেরত -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
স্থির শূন্য sort_pack_list(কাঠামো pack_list **pl)
{
	কাঠামো pack_list **ary, *p;
	আকার_t n = pack_list_আকার(*pl);

	যদি (n < 2)
		ফেরত;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	জন্য (n = 0, p = *pl; p; p = p->পরবর্তী)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	জন্য (আকার_t i = 0; i < n - 1; i++)
		ary[i]->পরবর্তী = ary[i + 1];
	ary[n - 1]->পরবর্তী = ফাঁকা;
	*pl = ary[0];

	মুক্ত(ary);
}


স্থির শূন্য minimize(কাঠামো pack_list **min)
{
	কাঠামো pack_list *pl, *unique = ফাঁকা, *non_unique = ফাঁকা;
	কাঠামো llist *missing, *unique_pack_objects;

	pl = local_packs;
	যখন (pl) {
		যদি (pl->unique_objects->আকার_2)
			pack_list_ঢোকাও(&unique, pl);
		অন্যথা
			pack_list_ঢোকাও(&non_unique, pl);
		pl = pl->পরবর্তী;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	যখন (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->পরবর্তী;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	যদি (missing->আকার_2 == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		ফেরত;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	যখন (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->পরবর্তী;
	}

	যখন (non_unique) {
		কাঠামো pack_list *পরবর্তী;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		যদি (non_unique->remaining_objects->আকার_2 == 0)
			বিরতি;

		pack_list_ঢোকাও(min, non_unique);

		জন্য (pl = non_unique->পরবর্তী; pl && pl->remaining_objects->আকার_2 > 0;  pl = pl->পরবর্তী)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		পরবর্তী = non_unique->পরবর্তী;
		মুক্ত(non_unique);
		non_unique = পরবর্তী;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

স্থির শূন্য load_all_objects(শূন্য)
{
	কাঠামো pack_list *pl = local_packs;
	কাঠামো llist_item *hint, *l;

	llist_init(&all_objects);

	যখন (pl) {
		hint = ফাঁকা;
		l = pl->remaining_objects->front;
		যখন (l) {
			hint = llist_ঢোকাও_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->পরবর্তী;
		}
		pl = pl->পরবর্তী;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	যখন (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->পরবর্তী;
	}
}

/* this scales like O(n^2) */
স্থির শূন্য cmp_local_packs(শূন্য)
{
	কাঠামো pack_list *subset, *pl = local_packs;

	/* only one packfile */
	যদি (!pl->পরবর্তী) {
		llist_init(&pl->unique_objects);
		ফেরত;
	}

	যখন ((subset = pl)) {
		যখন ((subset = subset->পরবর্তী))
			cmp_two_packs(pl, subset);
		pl = pl->পরবর্তী;
	}
}

স্থির শূন্য scan_alt_odb_packs(শূন্য)
{
	কাঠামো pack_list *local, *alt;

	alt = altodb_packs;
	যখন (alt) {
		local = local_packs;
		যখন (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->পরবর্তী;
		}
		alt = alt->পরবর্তী;
	}
}

স্থির কাঠামো pack_list * add_pack(কাঠামো packed_git *p)
{
	কাঠামো pack_list l;
	আকার_t off = 0, step;
	ধ্রুবক unsigned অক্ষর *base;

	যদি (!p->pack_local && !(alt_odb || verbose))
		ফেরত ফাঁকা;

	l.pack = p;
	llist_init(&l.remaining_objects);

	যদি (open_pack_সূচক(p)) {
		llist_free(l.remaining_objects);
		ফেরত ফাঁকা;
	}

	base = p->সূচক_তথ্য;
	base += 256 * 4 + ((p->সূচক_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->সূচক_version < 2) ? 4 : 0);
	যখন (off < p->অঙ্ক_objects * step) {
		llist_ঢোকাও_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_আকার = l.remaining_objects->আকার_2;
	l.unique_objects = ফাঁকা;
	যদি (p->pack_local)
		ফেরত pack_list_ঢোকাও(&local_packs, &l);
	অন্যথা
		ফেরত pack_list_ঢোকাও(&altodb_packs, &l);
}

স্থির কাঠামো pack_list * add_pack_file(ধ্রুবক অক্ষর *filename)
{
	কাঠামো packed_git *p;

	যদি (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		যদি (strstr(p->pack_name, filename))
			ফেরত add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

স্থির শূন্য load_all(শূন্য)
{
	কাঠামো packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

পূর্ণসংখ্যা cmd_pack_redundant(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix UNUSED, কাঠামো repository *repo UNUSED) {
	পূর্ণসংখ্যা i; পূর্ণসংখ্যা i_still_use_this = 0; কাঠামো pack_list *min = ফাঁকা, *red, *pl;
	কাঠামো llist *ignore;
	কাঠামো strbuf idx_name = STRBUF_INIT;
	অক্ষর buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	জন্য (i = 1; i < argc; i++) {
		ধ্রুবক অক্ষর *arg = argv[i];
		যদি (!strcmp(arg, "--")) {
			i++;
			বিরতি;
		}
		যদি (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			চালু;
		}
		যদি (!strcmp(arg, "--verbose")) {
			verbose = 1;
			চালু;
		}
		যদি (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			চালু;
		}
		যদি (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			চালু;
		}
		যদি (*arg == '-')
			usage(pack_redundant_usage);
		অন্যথা
			বিরতি;
	}

	যদি (!i_still_use_this)
		you_still_use_that("git pack-redundant", ফাঁকা);

	যদি (load_all_packs)
		load_all();
	অন্যথা
		যখন (*(argv + i) != ফাঁকা)
			add_pack_file(*(argv + i++));

	যদি (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	যদি (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	যদি (!isatty(0)) {
		কাঠামো object_id oid;
		যখন (fgets(buf, আকার(buf), stdin)) {
			যদি (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_ঢোকাও_sorted_unique(ignore, &oid, ফাঁকা);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	যখন (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->পরবর্তী;
	}

	cmp_local_packs();

	minimize(&min);

	যদি (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_আকার(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		যখন (pl) {
			fprintf(stderr, "\t%s\n", pl->pack->pack_name);
			pl = pl->পরবর্তী;
		}
		fprintf(stderr, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(unsigned long)get_pack_redundancy(min),
			(unsigned long)pack_set_bytecount(min)/1024);
		fprintf(stderr, "A total of %lu unique objects were considered.\n",
			(unsigned long)all_objects->আকার_2);
		fprintf(stderr, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	যখন (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->পরবর্তী;
	}
	যদি (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	ফেরত 0;
}
