/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"

#અન્તર્ભાવ "packfile.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "strbuf.h"

#define BLKSIZE 512

સ્થિર અચલ અક્ષર pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

સ્થિર પૂર્ણાંક load_all_packs, verbose, alt_odb;

સંરચના llist_item {
	સંરચના llist_item *next;
	સંરચના object_id oid;
};
સ્થિર સંરચના llist {
	સંરચના llist_item *front;
	સંરચના llist_item *back;
	size_t size;
} *all_objects; /* all objects which must be present in local packfiles */

સ્થિર સંરચના pack_list {
	સંરચના pack_list *next;
	સંરચના packed_git *pack;
	સંરચના llist *unique_objects;
	સંરચના llist *remaining_objects;
	size_t all_objects_size;
} *local_packs = ખાલી, *altodb_packs = ખાલી;

સ્થિર સંરચના llist_item *free_nodes;

સ્થિર અંતઃરેખ શૂન્ય llist_item_put(સંરચના llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

સ્થિર અંતઃરેખ સંરચના llist_item *llist_item_get(શૂન્ય)
{
	સંરચના llist_item *new_item;
	જો ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} નહીંતર {
		પૂર્ણાંક i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		માટે (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	પાછા new_item;
}

સ્થિર અંતઃરેખ શૂન્ય llist_init(સંરચના llist **list)
{
	*list = xmalloc(કદ(સંરચના llist));
	(*list)->front = (*list)->back = ખાલી;
	(*list)->size = 0;
}

સ્થિર શૂન્ય llist_free(સંરચના llist *list)
{
	માટે (સંરચના llist_item *i = list->front, *next; i; i = next) {
		next = i->next;
		llist_item_put(i);
	}
	મુક્ત(list);
}

સ્થિર સંરચના llist * llist_copy(સંરચના llist *list)
{
	સંરચના llist *ret;
	સંરચના llist_item *new_item, *old_item, *prev;

	llist_init(&ret);

	જો ((ret->size = list->size) == 0)
		પાછા ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	જ્યારે (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = ખાલી;
	ret->back = new_item;

	પાછા ret;
}

સ્થિર અંતઃરેખ સંરચના llist_item *llist_ઉમેરો(સંરચના llist *list,
					      સંરચના llist_item *after,
					      અચલ unsigned અક્ષર *oid)
{
	સંરચના llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = ખાલી;

	જો (after) {
		new_item->next = after->next;
		after->next = new_item;
		જો (after == list->back)
			list->back = new_item;
	} નહીંતર {/* insert in front */
		જો (list->size == 0)
			list->back = new_item;
		નહીંતર
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	પાછા new_item;
}

સ્થિર અંતઃરેખ સંરચના llist_item *llist_ઉમેરો_back(સંરચના llist *list,
						   અચલ unsigned અક્ષર *oid)
{
	પાછા llist_ઉમેરો(list, list->back, oid);
}

સ્થિર અંતઃરેખ સંરચના llist_item *llist_ઉમેરો_sorted_unique(સંરચના llist *list,
			અચલ સંરચના object_id *oid, સંરચના llist_item *hint)
{
	સંરચના llist_item *prev = ખાલી, *l;

	l = (hint == ખાલી) ? list->front : hint;
	જ્યારે (l) {
		પૂર્ણાંક cmp = oidcmp(&l->oid, oid);
		જો (cmp > 0) { /* we insert before this entry */
			પાછા llist_ઉમેરો(list, prev, oid->hash);
		}
		જો (!cmp) { /* already exists */
			પાછા l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	પાછા llist_ઉમેરો_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
સ્થિર અંતઃરેખ સંરચના llist_item * llist_sorted_remove(સંરચના llist *list, અચલ unsigned અક્ષર *oid, સંરચના llist_item *hint)
{
	સંરચના llist_item *prev, *l;

redo_from_start:
	l = (hint == ખાલી) ? list->front : hint;
	prev = ખાલી;
	જ્યારે (l) {
		અચલ પૂર્ણાંક cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		જો (cmp > 0) /* not in list, since sorted */
			પાછા prev;
		જો (!cmp) { /* found */
			જો (!prev) {
				જો (hint != ખાલી && hint != list->front) {
					/* we don't know the previous element */
					hint = ખાલી;
					જાઓ redo_from_start;
				}
				list->front = l->next;
			} નહીંતર
				prev->next = l->next;
			જો (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			પાછા prev;
		}
		prev = l;
		l = l->next;
	}
	પાછા prev;
}

/* computes A\B */
સ્થિર શૂન્ય llist_sorted_difference_inplace(સંરચના llist *A,
				     સંરચના llist *B)
{
	સંરચના llist_item *hint, *b;

	hint = ખાલી;
	b = B->front;

	જ્યારે (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

સ્થિર અંતઃરેખ સંરચના pack_list * pack_list_ઉમેરો(સંરચના pack_list **pl,
					   સંરચના pack_list *entry)
{
	સંરચના pack_list *p = xmalloc(કદ(સંરચના pack_list));
	memcpy(p, entry, કદ(સંરચના pack_list));
	p->next = *pl;
	*pl = p;
	પાછા p;
}

સ્થિર શૂન્ય pack_list_free(સંરચના pack_list *pl)
{
	માટે (સંરચના pack_list *next; pl; pl = next) {
		next = pl->next;
		મુક્ત(pl);
	}
}

સ્થિર અંતઃરેખ size_t pack_list_size(સંરચના pack_list *pl)
{
	size_t ret = 0;
	જ્યારે (pl) {
		ret++;
		pl = pl->next;
	}
	પાછા ret;
}

સ્થિર સંરચના pack_list * pack_list_difference(અચલ સંરચના pack_list *A,
					       અચલ સંરચના pack_list *B)
{
	સંરચના pack_list *ret;
	અચલ સંરચના pack_list *pl;

	જો (!A)
		પાછા ખાલી;

	pl = B;
	જ્યારે (pl != ખાલી) {
		જો (A->pack == pl->pack)
			પાછા pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(કદ(સંરચના pack_list));
	memcpy(ret, A, કદ(સંરચના pack_list));
	ret->next = pack_list_difference(A->next, B);
	પાછા ret;
}

સ્થિર શૂન્ય cmp_two_packs(સંરચના pack_list *p1, સંરચના pack_list *p2)
{
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	અચલ unsigned અક્ષર *p1_base, *p2_base;
	સંરચના llist_item *p1_hint = ખાલી, *p2_hint = ખાલી;
	અચલ unsigned પૂર્ણાંક hashsz = the_hash_algo->rawsz;

	જો (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	જો (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	જ્યારે (p1_off < p1->pack->અંક_objects * p1_step &&
	       p2_off < p2->pack->અંક_objects * p2_step)
	{
		અચલ પૂર્ણાંક cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		જો (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			ચાલુ;
		}
		જો (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} નહીંતર { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

સ્થિર size_t sizeof_union(સંરચના packed_git *p1, સંરચના packed_git *p2)
{
	size_t ret = 0;
	size_t p1_off = 0, p2_off = 0, p1_step, p2_step;
	અચલ unsigned અક્ષર *p1_base, *p2_base;
	અચલ unsigned પૂર્ણાંક hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	જ્યારે (p1_off < p1->અંક_objects * p1_step &&
	       p2_off < p2->અંક_objects * p2_step)
	{
		પૂર્ણાંક cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		જો (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			ચાલુ;
		}
		જો (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} નહીંતર { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	પાછા ret;
}

/* another O(n^2) function ... */
સ્થિર size_t get_pack_redundancy(સંરચના pack_list *pl)
{
	સંરચના pack_list *subset;
	size_t ret = 0;

	જો (!pl)
		પાછા 0;

	જ્યારે ((subset = pl->next)) {
		જ્યારે (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	પાછા ret;
}

સ્થિર અંતઃરેખ off_t pack_set_bytecount(સંરચના pack_list *pl)
{
	off_t ret = 0;
	જ્યારે (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	પાછા ret;
}

સ્થિર પૂર્ણાંક cmp_remaining_objects(અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
	સંરચના pack_list *pl_a = *((સંરચના pack_list **)a);
	સંરચના pack_list *pl_b = *((સંરચના pack_list **)b);

	જો (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		જો (pl_a->all_objects_size == pl_b->all_objects_size)
			પાછા 0;
		નહીંતર જો (pl_a->all_objects_size < pl_b->all_objects_size)
			પાછા 1;
		નહીંતર
			પાછા -1;
	} નહીંતર જો (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		પાછા 1;
	} નહીંતર {
		પાછા -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
સ્થિર શૂન્ય sort_pack_list(સંરચના pack_list **pl)
{
	સંરચના pack_list **ary, *p;
	size_t n = pack_list_size(*pl);

	જો (n < 2)
		પાછા;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	માટે (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	માટે (size_t i = 0; i < n - 1; i++)
		ary[i]->next = ary[i + 1];
	ary[n - 1]->next = ખાલી;
	*pl = ary[0];

	મુક્ત(ary);
}


સ્થિર શૂન્ય minimize(સંરચના pack_list **min)
{
	સંરચના pack_list *pl, *unique = ખાલી, *non_unique = ખાલી;
	સંરચના llist *missing, *unique_pack_objects;

	pl = local_packs;
	જ્યારે (pl) {
		જો (pl->unique_objects->size)
			pack_list_ઉમેરો(&unique, pl);
		નહીંતર
			pack_list_ઉમેરો(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	જ્યારે (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	જો (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		પાછા;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	જ્યારે (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	જ્યારે (non_unique) {
		સંરચના pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		જો (non_unique->remaining_objects->size == 0)
			તોડો;

		pack_list_ઉમેરો(min, non_unique);

		માટે (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		next = non_unique->next;
		મુક્ત(non_unique);
		non_unique = next;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

સ્થિર શૂન્ય load_all_objects(શૂન્ય)
{
	સંરચના pack_list *pl = local_packs;
	સંરચના llist_item *hint, *l;

	llist_init(&all_objects);

	જ્યારે (pl) {
		hint = ખાલી;
		l = pl->remaining_objects->front;
		જ્યારે (l) {
			hint = llist_ઉમેરો_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	જ્યારે (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
સ્થિર શૂન્ય cmp_local_packs(શૂન્ય)
{
	સંરચના pack_list *subset, *pl = local_packs;

	/* only one packfile */
	જો (!pl->next) {
		llist_init(&pl->unique_objects);
		પાછા;
	}

	જ્યારે ((subset = pl)) {
		જ્યારે ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

સ્થિર શૂન્ય scan_alt_odb_packs(શૂન્ય)
{
	સંરચના pack_list *local, *alt;

	alt = altodb_packs;
	જ્યારે (alt) {
		local = local_packs;
		જ્યારે (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->next;
		}
		alt = alt->next;
	}
}

સ્થિર સંરચના pack_list * add_pack(સંરચના packed_git *p)
{
	સંરચના pack_list l;
	size_t off = 0, step;
	અચલ unsigned અક્ષર *base;

	જો (!p->pack_local && !(alt_odb || verbose))
		પાછા ખાલી;

	l.pack = p;
	llist_init(&l.remaining_objects);

	જો (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		પાછા ખાલી;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	જ્યારે (off < p->અંક_objects * step) {
		llist_ઉમેરો_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = ખાલી;
	જો (p->pack_local)
		પાછા pack_list_ઉમેરો(&local_packs, &l);
	નહીંતર
		પાછા pack_list_ઉમેરો(&altodb_packs, &l);
}

સ્થિર સંરચના pack_list * add_pack_file(અચલ અક્ષર *filename)
{
	સંરચના packed_git *p;

	જો (strlen(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		જો (strstr(p->pack_name, filename))
			પાછા add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

સ્થિર શૂન્ય load_all(શૂન્ય)
{
	સંરચના packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

પૂર્ણાંક cmd_pack_redundant(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix UNUSED, સંરચના repository *repo UNUSED) {
	પૂર્ણાંક i; પૂર્ણાંક i_still_use_this = 0; સંરચના pack_list *min = ખાલી, *red, *pl;
	સંરચના llist *ignore;
	સંરચના strbuf idx_name = STRBUF_INIT;
	અક્ષર buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	માટે (i = 1; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];
		જો (!strcmp(arg, "--")) {
			i++;
			તોડો;
		}
		જો (!strcmp(arg, "--all")) {
			load_all_packs = 1;
			ચાલુ;
		}
		જો (!strcmp(arg, "--verbose")) {
			verbose = 1;
			ચાલુ;
		}
		જો (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			ચાલુ;
		}
		જો (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			ચાલુ;
		}
		જો (*arg == '-')
			usage(pack_redundant_usage);
		નહીંતર
			તોડો;
	}

	જો (!i_still_use_this)
		you_still_use_that("git pack-redundant", ખાલી);

	જો (load_all_packs)
		load_all();
	નહીંતર
		જ્યારે (*(argv + i) != ખાલી)
			add_pack_file(*(argv + i++));

	જો (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	જો (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	જો (!isatty(0)) {
		સંરચના object_id oid;
		જ્યારે (fgets(buf, કદ(buf), stdin)) {
			જો (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_ઉમેરો_sorted_unique(ignore, &oid, ખાલી);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	જ્યારે (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	જો (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(unsigned long)pack_list_size(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		જ્યારે (pl) {
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
	જ્યારે (pl) {
		printf("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	જો (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(unsigned long)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	પાછા 0;
}
