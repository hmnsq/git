/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"

#अन्तर्भाव "packfile.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "strbuf.h"

#define BLKSIZE 512

स्थिर ध्रुव वर्ण pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

स्थिर पूर्णाङ्क load_all_packs, verbose, alt_odb;

संरचना llist_item {
	संरचना llist_item *next;
	संरचना object_id oid;
};
स्थिर संरचना llist {
	संरचना llist_item *front;
	संरचना llist_item *back;
	माप_प्रकार size;
} *all_objects; /* all objects which must be present in local packfiles */

स्थिर संरचना pack_list {
	संरचना pack_list *next;
	संरचना packed_git *pack;
	संरचना llist *unique_objects;
	संरचना llist *remaining_objects;
	माप_प्रकार all_objects_size;
} *local_packs = रिक्त, *altodb_packs = रिक्त;

स्थिर संरचना llist_item *free_nodes;

स्थिर अन्तःस्थ शून्य llist_item_put(संरचना llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

स्थिर अन्तःस्थ संरचना llist_item *llist_item_get(शून्य)
{
	संरचना llist_item *new_item;
	यदि ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} अन्यथा {
		पूर्णाङ्क i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		प्रति (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	प्रत्यागम new_item;
}

स्थिर अन्तःस्थ शून्य llist_init(संरचना llist **list)
{
	*list = xmalloc(परिमाण(संरचना llist));
	(*list)->front = (*list)->back = रिक्त;
	(*list)->size = 0;
}

स्थिर शून्य llist_free(संरचना llist *list)
{
	प्रति (संरचना llist_item *i = list->front, *next; i; i = next) {
		next = i->next;
		llist_item_put(i);
	}
	मुक्त(list);
}

स्थिर संरचना llist * llist_copy(संरचना llist *list)
{
	संरचना llist *ret;
	संरचना llist_item *new_item, *old_item, *prev;

	llist_init(&ret);

	यदि ((ret->size = list->size) == 0)
		प्रत्यागम ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	यावत् (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = रिक्त;
	ret->back = new_item;

	प्रत्यागम ret;
}

स्थिर अन्तःस्थ संरचना llist_item *llist_जोड़ो(संरचना llist *list,
					      संरचना llist_item *after,
					      ध्रुव अचिन्हित वर्ण *oid)
{
	संरचना llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = रिक्त;

	यदि (after) {
		new_item->next = after->next;
		after->next = new_item;
		यदि (after == list->back)
			list->back = new_item;
	} अन्यथा {/* insert in front */
		यदि (list->size == 0)
			list->back = new_item;
		अन्यथा
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	प्रत्यागम new_item;
}

स्थिर अन्तःस्थ संरचना llist_item *llist_जोड़ो_back(संरचना llist *list,
						   ध्रुव अचिन्हित वर्ण *oid)
{
	प्रत्यागम llist_जोड़ो(list, list->back, oid);
}

स्थिर अन्तःस्थ संरचना llist_item *llist_जोड़ो_sorted_unique(संरचना llist *list,
			ध्रुव संरचना object_id *oid, संरचना llist_item *hint)
{
	संरचना llist_item *prev = रिक्त, *l;

	l = (hint == रिक्त) ? list->front : hint;
	यावत् (l) {
		पूर्णाङ्क cmp = oidcmp(&l->oid, oid);
		यदि (cmp > 0) { /* we insert before this entry */
			प्रत्यागम llist_जोड़ो(list, prev, oid->hash);
		}
		यदि (!cmp) { /* already exists */
			प्रत्यागम l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	प्रत्यागम llist_जोड़ो_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
स्थिर अन्तःस्थ संरचना llist_item * llist_sorted_remove(संरचना llist *list, ध्रुव अचिन्हित वर्ण *oid, संरचना llist_item *hint)
{
	संरचना llist_item *prev, *l;

redo_from_start:
	l = (hint == रिक्त) ? list->front : hint;
	prev = रिक्त;
	यावत् (l) {
		ध्रुव पूर्णाङ्क cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		यदि (cmp > 0) /* not in list, since sorted */
			प्रत्यागम prev;
		यदि (!cmp) { /* found */
			यदि (!prev) {
				यदि (hint != रिक्त && hint != list->front) {
					/* we don't know the previous element */
					hint = रिक्त;
					गच्छ redo_from_start;
				}
				list->front = l->next;
			} अन्यथा
				prev->next = l->next;
			यदि (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			प्रत्यागम prev;
		}
		prev = l;
		l = l->next;
	}
	प्रत्यागम prev;
}

/* computes A\B */
स्थिर शून्य llist_sorted_difference_inplace(संरचना llist *A,
				     संरचना llist *B)
{
	संरचना llist_item *hint, *b;

	hint = रिक्त;
	b = B->front;

	यावत् (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

स्थिर अन्तःस्थ संरचना pack_list * pack_list_जोड़ो(संरचना pack_list **pl,
					   संरचना pack_list *entry)
{
	संरचना pack_list *p = xmalloc(परिमाण(संरचना pack_list));
	स_नकल(p, entry, परिमाण(संरचना pack_list));
	p->next = *pl;
	*pl = p;
	प्रत्यागम p;
}

स्थिर शून्य pack_list_free(संरचना pack_list *pl)
{
	प्रति (संरचना pack_list *next; pl; pl = next) {
		next = pl->next;
		मुक्त(pl);
	}
}

स्थिर अन्तःस्थ माप_प्रकार pack_list_size(संरचना pack_list *pl)
{
	माप_प्रकार ret = 0;
	यावत् (pl) {
		ret++;
		pl = pl->next;
	}
	प्रत्यागम ret;
}

स्थिर संरचना pack_list * pack_list_difference(ध्रुव संरचना pack_list *A,
					       ध्रुव संरचना pack_list *B)
{
	संरचना pack_list *ret;
	ध्रुव संरचना pack_list *pl;

	यदि (!A)
		प्रत्यागम रिक्त;

	pl = B;
	यावत् (pl != रिक्त) {
		यदि (A->pack == pl->pack)
			प्रत्यागम pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(परिमाण(संरचना pack_list));
	स_नकल(ret, A, परिमाण(संरचना pack_list));
	ret->next = pack_list_difference(A->next, B);
	प्रत्यागम ret;
}

स्थिर शून्य cmp_two_packs(संरचना pack_list *p1, संरचना pack_list *p2)
{
	माप_प्रकार p1_off = 0, p2_off = 0, p1_step, p2_step;
	ध्रुव अचिन्हित वर्ण *p1_base, *p2_base;
	संरचना llist_item *p1_hint = रिक्त, *p2_hint = रिक्त;
	ध्रुव अचिन्हित पूर्णाङ्क hashsz = the_hash_algo->rawsz;

	यदि (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	यदि (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	यावत् (p1_off < p1->pack->अंक_objects * p1_step &&
	       p2_off < p2->pack->अंक_objects * p2_step)
	{
		ध्रुव पूर्णाङ्क cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		यदि (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			अनुवर्त;
		}
		यदि (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} अन्यथा { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

स्थिर माप_प्रकार sizeof_union(संरचना packed_git *p1, संरचना packed_git *p2)
{
	माप_प्रकार ret = 0;
	माप_प्रकार p1_off = 0, p2_off = 0, p1_step, p2_step;
	ध्रुव अचिन्हित वर्ण *p1_base, *p2_base;
	ध्रुव अचिन्हित पूर्णाङ्क hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	यावत् (p1_off < p1->अंक_objects * p1_step &&
	       p2_off < p2->अंक_objects * p2_step)
	{
		पूर्णाङ्क cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		यदि (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			अनुवर्त;
		}
		यदि (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} अन्यथा { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	प्रत्यागम ret;
}

/* another O(n^2) function ... */
स्थिर माप_प्रकार get_pack_redundancy(संरचना pack_list *pl)
{
	संरचना pack_list *subset;
	माप_प्रकार ret = 0;

	यदि (!pl)
		प्रत्यागम 0;

	यावत् ((subset = pl->next)) {
		यावत् (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	प्रत्यागम ret;
}

स्थिर अन्तःस्थ off_t pack_set_bytecount(संरचना pack_list *pl)
{
	off_t ret = 0;
	यावत् (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmp_remaining_objects(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	संरचना pack_list *pl_a = *((संरचना pack_list **)a);
	संरचना pack_list *pl_b = *((संरचना pack_list **)b);

	यदि (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		यदि (pl_a->all_objects_size == pl_b->all_objects_size)
			प्रत्यागम 0;
		अन्यथा यदि (pl_a->all_objects_size < pl_b->all_objects_size)
			प्रत्यागम 1;
		अन्यथा
			प्रत्यागम -1;
	} अन्यथा यदि (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		प्रत्यागम 1;
	} अन्यथा {
		प्रत्यागम -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
स्थिर शून्य sort_pack_list(संरचना pack_list **pl)
{
	संरचना pack_list **ary, *p;
	माप_प्रकार n = pack_list_size(*pl);

	यदि (n < 2)
		प्रत्यागम;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	प्रति (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	प्रति (माप_प्रकार i = 0; i < n - 1; i++)
		ary[i]->next = ary[i + 1];
	ary[n - 1]->next = रिक्त;
	*pl = ary[0];

	मुक्त(ary);
}


स्थिर शून्य minimize(संरचना pack_list **min)
{
	संरचना pack_list *pl, *unique = रिक्त, *non_unique = रिक्त;
	संरचना llist *missing, *unique_pack_objects;

	pl = local_packs;
	यावत् (pl) {
		यदि (pl->unique_objects->size)
			pack_list_जोड़ो(&unique, pl);
		अन्यथा
			pack_list_जोड़ो(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	यावत् (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	यदि (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		प्रत्यागम;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	यावत् (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	यावत् (non_unique) {
		संरचना pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		यदि (non_unique->remaining_objects->size == 0)
			भङ्ग;

		pack_list_जोड़ो(min, non_unique);

		प्रति (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		next = non_unique->next;
		मुक्त(non_unique);
		non_unique = next;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

स्थिर शून्य load_all_objects(शून्य)
{
	संरचना pack_list *pl = local_packs;
	संरचना llist_item *hint, *l;

	llist_init(&all_objects);

	यावत् (pl) {
		hint = रिक्त;
		l = pl->remaining_objects->front;
		यावत् (l) {
			hint = llist_जोड़ो_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	यावत् (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
स्थिर शून्य cmp_local_packs(शून्य)
{
	संरचना pack_list *subset, *pl = local_packs;

	/* only one packfile */
	यदि (!pl->next) {
		llist_init(&pl->unique_objects);
		प्रत्यागम;
	}

	यावत् ((subset = pl)) {
		यावत् ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

स्थिर शून्य scan_alt_odb_packs(शून्य)
{
	संरचना pack_list *local, *alt;

	alt = altodb_packs;
	यावत् (alt) {
		local = local_packs;
		यावत् (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->next;
		}
		alt = alt->next;
	}
}

स्थिर संरचना pack_list * add_pack(संरचना packed_git *p)
{
	संरचना pack_list l;
	माप_प्रकार off = 0, step;
	ध्रुव अचिन्हित वर्ण *base;

	यदि (!p->pack_local && !(alt_odb || verbose))
		प्रत्यागम रिक्त;

	l.pack = p;
	llist_init(&l.remaining_objects);

	यदि (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		प्रत्यागम रिक्त;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	यावत् (off < p->अंक_objects * step) {
		llist_जोड़ो_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = रिक्त;
	यदि (p->pack_local)
		प्रत्यागम pack_list_जोड़ो(&local_packs, &l);
	अन्यथा
		प्रत्यागम pack_list_जोड़ो(&altodb_packs, &l);
}

स्थिर संरचना pack_list * add_pack_file(ध्रुव वर्ण *filename)
{
	संरचना packed_git *p;

	यदि (म_माप(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		यदि (म_माला(p->pack_name, filename))
			प्रत्यागम add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

स्थिर शून्य load_all(शून्य)
{
	संरचना packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

पूर्णाङ्क cmd_pack_redundant(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix UNUSED, संरचना repository *repo UNUSED) {
	पूर्णाङ्क i; पूर्णाङ्क i_still_use_this = 0; संरचना pack_list *min = रिक्त, *red, *pl;
	संरचना llist *ignore;
	संरचना strbuf idx_name = STRBUF_INIT;
	वर्ण buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	प्रति (i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		यदि (!म_भेद(arg, "--")) {
			i++;
			भङ्ग;
		}
		यदि (!म_भेद(arg, "--all")) {
			load_all_packs = 1;
			अनुवर्त;
		}
		यदि (!म_भेद(arg, "--verbose")) {
			verbose = 1;
			अनुवर्त;
		}
		यदि (!म_भेद(arg, "--alt-odb")) {
			alt_odb = 1;
			अनुवर्त;
		}
		यदि (!म_भेद(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			अनुवर्त;
		}
		यदि (*arg == '-')
			usage(pack_redundant_usage);
		अन्यथा
			भङ्ग;
	}

	यदि (!i_still_use_this)
		you_still_use_that("git pack-redundant", रिक्त);

	यदि (load_all_packs)
		load_all();
	अन्यथा
		यावत् (*(argv + i) != रिक्त)
			add_pack_file(*(argv + i++));

	यदि (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	यदि (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	यदि (!isatty(0)) {
		संरचना object_id oid;
		यावत् (ख_माला_लो(buf, परिमाण(buf), मानक_निवेश)) {
			यदि (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_जोड़ो_sorted_unique(ignore, &oid, रिक्त);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	यावत् (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	यदि (verbose) {
		ख_लिखो(मानक_त्रुटि, "There are %lu packs available in alt-odbs.\n",
			(अचिन्हित दीर्घ)pack_list_size(altodb_packs));
		ख_लिखो(मानक_त्रुटि, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		यावत् (pl) {
			ख_लिखो(मानक_त्रुटि, "\t%s\n", pl->pack->pack_name);
			pl = pl->next;
		}
		ख_लिखो(मानक_त्रुटि, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(अचिन्हित दीर्घ)get_pack_redundancy(min),
			(अचिन्हित दीर्घ)pack_set_bytecount(min)/1024);
		ख_लिखो(मानक_त्रुटि, "A total of %lu unique objects were considered.\n",
			(अचिन्हित दीर्घ)all_objects->size);
		ख_लिखो(मानक_त्रुटि, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	यावत् (pl) {
		म_लिखो("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	यदि (verbose)
		ख_लिखो(मानक_त्रुटि, "%luMB of redundant packs in total.\n",
			(अचिन्हित दीर्घ)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	प्रत्यागम 0;
}
