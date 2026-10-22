/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#define USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hex.h"

#ضمن "packfile.h"
#ضمن "odb.h"
#ضمن "strbuf.h"

#define BLKSIZE 512

ساكن ثابت حرف pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

ساكن صحيح load_all_packs, verbose, alt_odb;

بنية llist_item {
	بنية llist_item *next;
	بنية object_id oid;
};
ساكن بنية llist {
	بنية llist_item *front;
	بنية llist_item *back;
	size_t size;
} *all_objects; /* all objects which must be present in local packfiles */

ساكن بنية pack_list {
	بنية pack_list *next;
	بنية packed_git *pack;
	بنية llist *unique_objects;
	بنية llist *remaining_objects;
	size_t all_objects_size;
} *local_packs = عدم, *altodb_packs = عدم;

ساكن بنية llist_item *free_nodes;

ساكن مضمن فراغ llist_item_put(بنية llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

ساكن مضمن بنية llist_item *llist_item_get(فراغ)
{
	بنية llist_item *new_item;
	ܐܢ ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} ܐܚܪܢܐ {
		صحيح i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		لكل (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	ܦܢܐ new_item;
}

ساكن مضمن فراغ llist_init(بنية llist **list)
{
	*list = xmalloc(حجم(بنية llist));
	(*list)->front = (*list)->back = عدم;
	(*list)->size = 0;
}

ساكن فراغ llist_free(بنية llist *list)
{
	لكل (بنية llist_item *i = list->front, *next; i; i = next) {
		next = i->next;
		llist_item_put(i);
	}
	حرر(list);
}

ساكن بنية llist * llist_copy(بنية llist *list)
{
	بنية llist *ret;
	بنية llist_item *new_item, *old_item, *prev;

	llist_init(&ret);

	ܐܢ ((ret->size = list->size) == 0)
		ܦܢܐ ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	ܟܡܐ (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = عدم;
	ret->back = new_item;

	ܦܢܐ ret;
}

ساكن مضمن بنية llist_item *llist_insert(بنية llist *list,
					      بنية llist_item *after,
					      ثابت unsigned حرف *oid)
{
	بنية llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = عدم;

	ܐܢ (after) {
		new_item->next = after->next;
		after->next = new_item;
		ܐܢ (after == list->back)
			list->back = new_item;
	} ܐܚܪܢܐ {/* insert in front */
		ܐܢ (list->size == 0)
			list->back = new_item;
		ܐܚܪܢܐ
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	ܦܢܐ new_item;
}

ساكن مضمن بنية llist_item *llist_insert_back(بنية llist *list,
						   ثابت unsigned حرف *oid)
{
	ܦܢܐ llist_insert(list, list->back, oid);
}

ساكن مضمن بنية llist_item *llist_insert_sorted_unique(بنية llist *list,
			ثابت بنية object_id *oid, بنية llist_item *hint)
{
	بنية llist_item *prev = عدم, *l;

	l = (hint == عدم) ? list->front : hint;
	ܟܡܐ (l) {
		صحيح cmp = oidcmp(&l->oid, oid);
		ܐܢ (cmp > 0) { /* we insert before this entry */
			ܦܢܐ llist_insert(list, prev, oid->hash);
		}
		ܐܢ (!cmp) { /* already exists */
			ܦܢܐ l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	ܦܢܐ llist_insert_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
ساكن مضمن بنية llist_item * llist_sorted_remove(بنية llist *list, ثابت unsigned حرف *oid, بنية llist_item *hint)
{
	بنية llist_item *prev, *l;

redo_from_start:
	l = (hint == عدم) ? list->front : hint;
	prev = عدم;
	ܟܡܐ (l) {
		ثابت صحيح cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		ܐܢ (cmp > 0) /* not in list, since sorted */
			ܦܢܐ prev;
		ܐܢ (!cmp) { /* found */
			ܐܢ (!prev) {
				ܐܢ (hint != عدم && hint != list->front) {
					/* we don't know the previous element */
					hint = عدم;
					اذهب redo_from_start;
				}
				list->front = l->next;
			} ܐܚܪܢܐ
				prev->next = l->next;
			ܐܢ (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			ܦܢܐ prev;
		}
		prev = l;
		l = l->next;
	}
	ܦܢܐ prev;
}

/* computes A\B */
ساكن فراغ llist_sorted_difference_inplace(بنية llist *A,
				     بنية llist *B)
{
	بنية llist_item *hint, *b;

	hint = عدم;
	b = B->front;

	ܟܡܐ (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

ساكن مضمن بنية pack_list * pack_list_insert(بنية pack_list **pl,
					   بنية pack_list *entry)
{
	بنية pack_list *p = xmalloc(حجم(بنية pack_list));
	memcpy(p, entry, حجم(بنية pack_list));
	p->next = *pl;
	*pl = p;
	ܦܢܐ p;
}

ساكن فراغ pack_list_free(بنية pack_list *pl)
{
	لكل (بنية pack_list *next; pl; pl = next) {
		next = pl->next;
		حرر(pl);
	}
}

ساكن مضمن size_t pack_list_size(بنية pack_list *pl)
{
	size_t ret = 0;
	ܟܡܐ (pl) {
		ret++;
		pl = pl->next;
	}
	ܦܢܐ ret;
}

ساكن بنية pack_list * pack_list_difference(ثابت بنية pack_list *A,
					       ثابت بنية pack_list *B)
{
	بنية pack_list *ret;
	ثابت بنية pack_list *pl;

	ܐܢ (!A)
		ܦܢܐ عدم;

	pl = B;
	ܟܡܐ (pl != عدم) {
		ܐܢ (A->pack == pl->pack)
			ܦܢܐ pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(حجم(بنية pack_list));
	memcpy(ret, A, حجم(بنية pack_list));
	ret->next = pack_list_difference(A->next, B);
	ܦܢܐ ret;
}

ساكن فراغ cmp_two_packs(بنية pack_list *p1, بنية pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ثابت unsigned حرف *p1_base, *p2_base;
	بنية llist_item *p1_hint = عدم, *p2_hint = عدم;
	ثابت unsigned صحيح hashsz = the_hash_algo->rawsz;

	ܐܢ (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	ܐܢ (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	ܟܡܐ (p1_off < p1->pack->num_objects * p1_step &&
	       p2_off < p2->pack->num_objects * p2_step)
	{
		ثابت صحيح cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ܐܢ (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			استمر;
		}
		ܐܢ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} ܐܚܪܢܐ { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

ساكن size_t sizeof_union(بنية packed_git *p1, بنية packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	ثابت unsigned حرف *p1_base, *p2_base;
	ثابت unsigned صحيح hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	ܟܡܐ (p1_off < p1->num_objects * p1_step &&
	       p2_off < p2->num_objects * p2_step)
	{
		صحيح cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ܐܢ (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			استمر;
		}
		ܐܢ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} ܐܚܪܢܐ { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	ܦܢܐ ret;
}

/* another O(n^2) function ... */
ساكن size_t get_pack_redundancy(بنية pack_list *pl)
{
	بنية pack_list *subset;
	size_t ret = 0;

	ܐܢ (!pl)
		ܦܢܐ 0;

	ܟܡܐ ((subset = pl->next)) {
		ܟܡܐ (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	ܦܢܐ ret;
}

ساكن مضمن off_t pack_set_bytecount(بنية pack_list *pl)
{
	off_t ret = 0;
	ܟܡܐ (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	ܦܢܐ ret;
}

ساكن صحيح cmp_remaining_objects(ثابت فراغ *a, ثابت فراغ *b)
{
	بنية pack_list *pl_a = *((بنية pack_list **)a);
	بنية pack_list *pl_b = *((بنية pack_list **)b);

	ܐܢ (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		ܐܢ (pl_a->all_objects_size == pl_b->all_objects_size)
			ܦܢܐ 0;
		ܐܚܪܢܐ ܐܢ (pl_a->all_objects_size < pl_b->all_objects_size)
			ܦܢܐ 1;
		ܐܚܪܢܐ
			ܦܢܐ -1;
	} ܐܚܪܢܐ ܐܢ (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		ܦܢܐ 1;
	} ܐܚܪܢܐ {
		ܦܢܐ -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
ساكن فراغ sort_pack_list(بنية pack_list **pl)
{
	بنية pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	ܐܢ (n < 2)
		ܦܢܐ;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	لكل (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	لكل (size_t i = 0; i < n - 1; i++)
		ary[i]->next = ary[i + 1];
	ary[n - 1]->next = عدم;
	*pl = ary[0];

	حرر(ary);
}


ساكن فراغ minimize(بنية pack_list **min)
{
	بنية pack_list *pl, *unique = عدم, *non_unique = عدم;
	بنية llist *missing, *unique_pack_objects;

	pl = local_packs;
	ܟܡܐ (pl) {
		ܐܢ (pl->unique_objects->size)
			pack_list_insert(&unique, pl);
		ܐܚܪܢܐ
			pack_list_insert(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	ܟܡܐ (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	ܐܢ (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		ܦܢܐ;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	ܟܡܐ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	ܟܡܐ (non_unique) {
		بنية pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		ܐܢ (non_unique->remaining_objects->size == 0)
			اكسر;

		pack_list_insert(min, non_unique);

		لكل (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		next = non_unique->next;
		حرر(non_unique);
		non_unique = next;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

ساكن فراغ load_all_objects(فراغ)
{
	بنية pack_list *pl = local_packs;
	بنية llist_item *hint, *l;

	llist_init(&all_objects);

	ܟܡܐ (pl) {
		hint = عدم;
		l = pl->remaining_objects->front;
		ܟܡܐ (l) {
			hint = llist_insert_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	ܟܡܐ (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
ساكن فراغ cmp_local_packs(فراغ)
{
	بنية pack_list *subset, *pl = local_packs;

	/* only one packfile */
	ܐܢ (!pl->next) {
		llist_init(&pl->unique_objects);
		ܦܢܐ;
	}

	ܟܡܐ ((subset = pl)) {
		ܟܡܐ ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

ساكن فراغ scan_alt_odb_packs(فراغ)
{
	بنية pack_list *local, *alt;

	alt = altodb_packs;
	ܟܡܐ (alt) {
		local = local_packs;
		ܟܡܐ (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->next;
		}
		alt = alt->next;
	}
}

ساكن بنية pack_list * add_pack(بنية packed_git *p)
{
	بنية pack_list l;
	size_t off = 0, step;
	ثابت unsigned حرف *base;

	ܐܢ (!p->pack_local && !(alt_odb || verbose))
		ܦܢܐ عدم;

	l.pack = p;
	llist_init(&l.remaining_objects);

	ܐܢ (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		ܦܢܐ عدم;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	ܟܡܐ (off < p->num_objects * step) {
		llist_insert_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = عدم;
	ܐܢ (p->pack_local)
		ܦܢܐ pack_list_insert(&local_packs, &l);
	ܐܚܪܢܐ
		ܦܢܐ pack_list_insert(&altodb_packs, &l);
}

ساكن بنية pack_list * add_pack_file(ثابت حرف *filename)
{
	بنية packed_git *p;

	ܐܢ (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		ܐܢ (strstr(p->pack_name, filename))
			ܦܢܐ add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

ساكن فراغ load_all(فراغ)
{
	بنية packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

صحيح cmd_pack_redundant(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED, بنية repository *repo UNUSED) {
	صحيح i; صحيح i_still_use_this = 0; بنية pack_list *min = عدم, *red, *pl;
	بنية llist *ignore;
	بنية strbuf idx_name = STRBUF_INIT;
	حرف buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	لكل (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		ܐܢ (!strcmp(arg, "--")) {
			i++;
			اكسر;
		}
		ܐܢ (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			استمر;
		}
		ܐܢ (!strcmp(arg, "--verbose")) {
			verbose = 1;
			استمر;
		}
		ܐܢ (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			استمر;
		}
		ܐܢ (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			استمر;
		}
		ܐܢ (*arg == '-')
			usage(pack_redundant_usage);
		ܐܚܪܢܐ
			اكسر;
	}

	ܐܢ (!i_still_use_this)
		you_still_use_that("git pack-redundant", عدم);

	ܐܢ (load_all_packs)
		load_all();
	ܐܚܪܢܐ
		ܟܡܐ (*(argv + i) != عدم)
			add_pack_file(*(argv + i++));

	ܐܢ (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	ܐܢ (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	ܐܢ (!isatty(0)) {
		بنية object_id oid;
		ܟܡܐ (fgets(buf, حجم(buf), stdin)) {
			ܐܢ (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_insert_sorted_unique(ignore, &oid, عدم);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	ܟܡܐ (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	ܐܢ (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		ܟܡܐ (pl) {
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
	ܟܡܐ (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	ܐܢ (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	ܦܢܐ 0;
}
