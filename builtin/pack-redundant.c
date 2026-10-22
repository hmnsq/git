/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "gettext.h"
#شامل "hex.h"

#شامل "packfile.h"
#شامل "odb.h"
#شامل "strbuf.h"

#define BLKSIZE 512

ایستا ثابت نویسه pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

ایستا صحیح load_all_packs, verbose, alt_odb;

ساختار llist_item {
	ساختار llist_item *next;
	ساختار object_id oid;
};
ایستا ساختار llist {
	ساختار llist_item *front;
	ساختار llist_item *back;
	size_t size;
} *all_objects; /* all objects which must be present in local packfiles */

ایستا ساختار pack_list {
	ساختار pack_list *next;
	ساختار packed_git *pack;
	ساختار llist *unique_objects;
	ساختار llist *remaining_objects;
	size_t all_objects_size;
} *local_packs = پوچ, *altodb_packs = پوچ;

ایستا ساختار llist_item *free_nodes;

ایستا درونخطی تهی llist_item_put(ساختار llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

ایستا درونخطی ساختار llist_item *llist_item_get(تهی)
{
	ساختار llist_item *new_item;
	اگر ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} وگرنه {
		صحیح i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		برای (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	بازگشت new_item;
}

ایستا درونخطی تهی llist_init(ساختار llist **list)
{
	*list = xmalloc(اندازه(ساختار llist));
	(*list)->front = (*list)->back = پوچ;
	(*list)->size = 0;
}

ایستا تهی llist_free(ساختار llist *list)
{
	برای (ساختار llist_item *i = list->front, *next; i; i = next) {
		next = i->next;
		llist_item_put(i);
	}
	آزاد(list);
}

ایستا ساختار llist * llist_copy(ساختار llist *list)
{
	ساختار llist *ret;
	ساختار llist_item *new_item, *old_item, *prev;

	llist_init(&ret);

	اگر ((ret->size = list->size) == 0)
		بازگشت ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	وقتی (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = پوچ;
	ret->back = new_item;

	بازگشت ret;
}

ایستا درونخطی ساختار llist_item *llist_درج(ساختار llist *list,
					      ساختار llist_item *after,
					      ثابت unsigned نویسه *oid)
{
	ساختار llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = پوچ;

	اگر (after) {
		new_item->next = after->next;
		after->next = new_item;
		اگر (after == list->back)
			list->back = new_item;
	} وگرنه {/* insert in front */
		اگر (list->size == 0)
			list->back = new_item;
		وگرنه
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	بازگشت new_item;
}

ایستا درونخطی ساختار llist_item *llist_درج_back(ساختار llist *list,
						   ثابت unsigned نویسه *oid)
{
	بازگشت llist_درج(list, list->back, oid);
}

ایستا درونخطی ساختار llist_item *llist_درج_sorted_unique(ساختار llist *list,
			ثابت ساختار object_id *oid, ساختار llist_item *hint)
{
	ساختار llist_item *prev = پوچ, *l;

	l = (hint == پوچ) ? list->front : hint;
	وقتی (l) {
		صحیح cmp = oidcmp(&l->oid, oid);
		اگر (cmp > 0) { /* we insert before this entry */
			بازگشت llist_درج(list, prev, oid->hash);
		}
		اگر (!cmp) { /* already exists */
			بازگشت l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	بازگشت llist_درج_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
ایستا درونخطی ساختار llist_item * llist_sorted_remove(ساختار llist *list, ثابت unsigned نویسه *oid, ساختار llist_item *hint)
{
	ساختار llist_item *prev, *l;

redo_from_start:
	l = (hint == پوچ) ? list->front : hint;
	prev = پوچ;
	وقتی (l) {
		ثابت صحیح cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		اگر (cmp > 0) /* not in list, since sorted */
			بازگشت prev;
		اگر (!cmp) { /* found */
			اگر (!prev) {
				اگر (hint != پوچ && hint != list->front) {
					/* we don't know the previous element */
					hint = پوچ;
					برو redo_from_start;
				}
				list->front = l->next;
			} وگرنه
				prev->next = l->next;
			اگر (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			بازگشت prev;
		}
		prev = l;
		l = l->next;
	}
	بازگشت prev;
}

/* computes A\B */
ایستا تهی llist_sorted_difference_inplace(ساختار llist *A,
				     ساختار llist *B)
{
	ساختار llist_item *hint, *b;

	hint = پوچ;
	b = B->front;

	وقتی (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

ایستا درونخطی ساختار pack_list * pack_list_درج(ساختار pack_list **pl,
					   ساختار pack_list *entry)
{
	ساختار pack_list *p = xmalloc(اندازه(ساختار pack_list));
	memcpy(p, entry, اندازه(ساختار pack_list));
	p->next = *pl;
	*pl = p;
	بازگشت p;
}

ایستا تهی pack_list_free(ساختار pack_list *pl)
{
	برای (ساختار pack_list *next; pl; pl = next) {
		next = pl->next;
		آزاد(pl);
	}
}

ایستا درونخطی size_t pack_list_size(ساختار pack_list *pl)
{
	size_t ret = 0;
	وقتی (pl) {
		ret++;
		pl = pl->next;
	}
	بازگشت ret;
}

ایستا ساختار pack_list * pack_list_difference(ثابت ساختار pack_list *A,
					       ثابت ساختار pack_list *B)
{
	ساختار pack_list *ret;
	ثابت ساختار pack_list *pl;

	اگر (!A)
		بازگشت پوچ;

	pl = B;
	وقتی (pl != پوچ) {
		اگر (A->pack == pl->pack)
			بازگشت pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(اندازه(ساختار pack_list));
	memcpy(ret, A, اندازه(ساختار pack_list));
	ret->next = pack_list_difference(A->next, B);
	بازگشت ret;
}

ایستا تهی cmp_two_packs(ساختار pack_list *p1, ساختار pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ثابت unsigned نویسه *p1_base, *p2_base;
	ساختار llist_item *p1_hint = پوچ, *p2_hint = پوچ;
	ثابت unsigned صحیح hashsz = the_hash_algo->rawsz;

	اگر (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	اگر (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	وقتی (p1_off < p1->pack->رقم_objects * p1_step &&
	       p2_off < p2->pack->رقم_objects * p2_step)
	{
		ثابت صحیح cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		اگر (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			ادامه;
		}
		اگر (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} وگرنه { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

ایستا size_t sizeof_union(ساختار packed_git *p1, ساختار packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ثابت unsigned نویسه *p1_base, *p2_base;
	ثابت unsigned صحیح hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	وقتی (p1_off < p1->رقم_objects * p1_step &&
	       p2_off < p2->رقم_objects * p2_step)
	{
		صحیح cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		اگر (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			ادامه;
		}
		اگر (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} وگرنه { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	بازگشت ret;
}

/* another O(n^2) function ... */
ایستا size_t get_pack_redundancy(ساختار pack_list *pl)
{
	ساختار pack_list *subset;
	size_t ret = 0;

	اگر (!pl)
		بازگشت 0;

	وقتی ((subset = pl->next)) {
		وقتی (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	بازگشت ret;
}

ایستا درونخطی off_t pack_set_bytecount(ساختار pack_list *pl)
{
	off_t ret = 0;
	وقتی (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	بازگشت ret;
}

ایستا صحیح cmp_remaining_objects(ثابت تهی *a, ثابت تهی *b)
{
	ساختار pack_list *pl_a = *((ساختار pack_list **)a);
	ساختار pack_list *pl_b = *((ساختار pack_list **)b);

	اگر (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		اگر (pl_a->all_objects_size == pl_b->all_objects_size)
			بازگشت 0;
		وگرنه اگر (pl_a->all_objects_size < pl_b->all_objects_size)
			بازگشت 1;
		وگرنه
			بازگشت -1;
	} وگرنه اگر (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		بازگشت 1;
	} وگرنه {
		بازگشت -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
ایستا تهی sort_pack_list(ساختار pack_list **pl)
{
	ساختار pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	اگر (n < 2)
		بازگشت;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	برای (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	برای (size_t i = 0; i < n - 1; i++)
		ary[i]->next = ary[i + 1];
	ary[n - 1]->next = پوچ;
	*pl = ary[0];

	آزاد(ary);
}


ایستا تهی minimize(ساختار pack_list **min)
{
	ساختار pack_list *pl, *unique = پوچ, *non_unique = پوچ;
	ساختار llist *missing, *unique_pack_objects;

	pl = local_packs;
	وقتی (pl) {
		اگر (pl->unique_objects->size)
			pack_list_درج(&unique, pl);
		وگرنه
			pack_list_درج(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	وقتی (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	اگر (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		بازگشت;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	وقتی (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	وقتی (non_unique) {
		ساختار pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		اگر (non_unique->remaining_objects->size == 0)
			بشکن;

		pack_list_درج(min, non_unique);

		برای (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		next = non_unique->next;
		آزاد(non_unique);
		non_unique = next;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

ایستا تهی load_all_objects(تهی)
{
	ساختار pack_list *pl = local_packs;
	ساختار llist_item *hint, *l;

	llist_init(&all_objects);

	وقتی (pl) {
		hint = پوچ;
		l = pl->remaining_objects->front;
		وقتی (l) {
			hint = llist_درج_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	وقتی (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
ایستا تهی cmp_local_packs(تهی)
{
	ساختار pack_list *subset, *pl = local_packs;

	/* only one packfile */
	اگر (!pl->next) {
		llist_init(&pl->unique_objects);
		بازگشت;
	}

	وقتی ((subset = pl)) {
		وقتی ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

ایستا تهی scan_alt_odb_packs(تهی)
{
	ساختار pack_list *local, *alt;

	alt = altodb_packs;
	وقتی (alt) {
		local = local_packs;
		وقتی (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->next;
		}
		alt = alt->next;
	}
}

ایستا ساختار pack_list * add_pack(ساختار packed_git *p)
{
	ساختار pack_list l;
	size_t off = 0, step;
	ثابت unsigned نویسه *base;

	اگر (!p->pack_local && !(alt_odb || verbose))
		بازگشت پوچ;

	l.pack = p;
	llist_init(&l.remaining_objects);

	اگر (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		بازگشت پوچ;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	وقتی (off < p->رقم_objects * step) {
		llist_درج_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = پوچ;
	اگر (p->pack_local)
		بازگشت pack_list_درج(&local_packs, &l);
	وگرنه
		بازگشت pack_list_درج(&altodb_packs, &l);
}

ایستا ساختار pack_list * add_pack_file(ثابت نویسه *filename)
{
	ساختار packed_git *p;

	اگر (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		اگر (strstr(p->pack_name, filename))
			بازگشت add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

ایستا تهی load_all(تهی)
{
	ساختار packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

صحیح cmd_pack_redundant(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix UNUSED, ساختار repository *repo UNUSED) {
	صحیح i; صحیح i_still_use_this = 0; ساختار pack_list *min = پوچ, *red, *pl;
	ساختار llist *ignore;
	ساختار strbuf idx_name = STRBUF_INIT;
	نویسه buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	برای (i = 1; i < argc; i++) {
		ثابت نویسه *arg = argv[i];
		اگر (!strcmp(arg, "--")) {
			i++;
			بشکن;
		}
		اگر (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			ادامه;
		}
		اگر (!strcmp(arg, "--verbose")) {
			verbose = 1;
			ادامه;
		}
		اگر (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			ادامه;
		}
		اگر (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			ادامه;
		}
		اگر (*arg == '-')
			usage(pack_redundant_usage);
		وگرنه
			بشکن;
	}

	اگر (!i_still_use_this)
		you_still_use_that("git pack-redundant", پوچ);

	اگر (load_all_packs)
		load_all();
	وگرنه
		وقتی (*(argv + i) != پوچ)
			add_pack_file(*(argv + i++));

	اگر (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	اگر (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	اگر (!isatty(0)) {
		ساختار object_id oid;
		وقتی (fgets(buf, اندازه(buf), stdin)) {
			اگر (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_درج_sorted_unique(ignore, &oid, پوچ);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	وقتی (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	اگر (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		وقتی (pl) {
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
	وقتی (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	اگر (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	بازگشت 0;
}
