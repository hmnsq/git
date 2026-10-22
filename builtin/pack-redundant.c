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

स्थिर स्थिरांक अक्षर pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

स्थिर पूर्णांक load_all_packs, verbose, alt_odb;

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

स्थिर ओळीत शून्य llist_item_put(संरचना llist_item *item)
{
	item->next = free_nodes;
	free_nodes = item;
}

स्थिर ओळीत संरचना llist_item *llist_item_get(शून्य)
{
	संरचना llist_item *new_item;
	जर ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->next;
	} अन्यथा {
		पूर्णांक i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		साठी (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	परत new_item;
}

स्थिर ओळीत शून्य llist_init(संरचना llist **list)
{
	*list = xmalloc(आकार(संरचना llist));
	(*list)->front = (*list)->back = रिक्त;
	(*list)->size = 0;
}

स्थिर शून्य llist_free(संरचना llist *list)
{
	साठी (संरचना llist_item *i = list->front, *next; i; i = next) {
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

	जर ((ret->size = list->size) == 0)
		परत ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->next;
	पर्यंत (old_item) {
		prev = new_item;
		new_item = llist_item_get();
		prev->next = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->next;
	}
	new_item->next = रिक्त;
	ret->back = new_item;

	परत ret;
}

स्थिर ओळीत संरचना llist_item *llist_घाला(संरचना llist *list,
					      संरचना llist_item *after,
					      स्थिरांक अचिन्हित अक्षर *oid)
{
	संरचना llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->next = रिक्त;

	जर (after) {
		new_item->next = after->next;
		after->next = new_item;
		जर (after == list->back)
			list->back = new_item;
	} अन्यथा {/* insert in front */
		जर (list->size == 0)
			list->back = new_item;
		अन्यथा
			new_item->next = list->front;
		list->front = new_item;
	}
	list->size++;
	परत new_item;
}

स्थिर ओळीत संरचना llist_item *llist_घाला_back(संरचना llist *list,
						   स्थिरांक अचिन्हित अक्षर *oid)
{
	परत llist_घाला(list, list->back, oid);
}

स्थिर ओळीत संरचना llist_item *llist_घाला_sorted_unique(संरचना llist *list,
			स्थिरांक संरचना object_id *oid, संरचना llist_item *hint)
{
	संरचना llist_item *prev = रिक्त, *l;

	l = (hint == रिक्त) ? list->front : hint;
	पर्यंत (l) {
		पूर्णांक cmp = oidcmp(&l->oid, oid);
		जर (cmp > 0) { /* we insert before this entry */
			परत llist_घाला(list, prev, oid->hash);
		}
		जर (!cmp) { /* already exists */
			परत l;
		}
		prev = l;
		l = l->next;
	}
	/* insert at the end */
	परत llist_घाला_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
स्थिर ओळीत संरचना llist_item * llist_sorted_remove(संरचना llist *list, स्थिरांक अचिन्हित अक्षर *oid, संरचना llist_item *hint)
{
	संरचना llist_item *prev, *l;

redo_from_start:
	l = (hint == रिक्त) ? list->front : hint;
	prev = रिक्त;
	पर्यंत (l) {
		स्थिरांक पूर्णांक cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		जर (cmp > 0) /* not in list, since sorted */
			परत prev;
		जर (!cmp) { /* found */
			जर (!prev) {
				जर (hint != रिक्त && hint != list->front) {
					/* we don't know the previous element */
					hint = रिक्त;
					जा redo_from_start;
				}
				list->front = l->next;
			} अन्यथा
				prev->next = l->next;
			जर (l == list->back)
				list->back = prev;
			llist_item_put(l);
			list->size--;
			परत prev;
		}
		prev = l;
		l = l->next;
	}
	परत prev;
}

/* computes A\B */
स्थिर शून्य llist_sorted_difference_inplace(संरचना llist *A,
				     संरचना llist *B)
{
	संरचना llist_item *hint, *b;

	hint = रिक्त;
	b = B->front;

	पर्यंत (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->next;
	}
}

स्थिर ओळीत संरचना pack_list * pack_list_घाला(संरचना pack_list **pl,
					   संरचना pack_list *entry)
{
	संरचना pack_list *p = xmalloc(आकार(संरचना pack_list));
	स_नकल(p, entry, आकार(संरचना pack_list));
	p->next = *pl;
	*pl = p;
	परत p;
}

स्थिर शून्य pack_list_free(संरचना pack_list *pl)
{
	साठी (संरचना pack_list *next; pl; pl = next) {
		next = pl->next;
		मुक्त(pl);
	}
}

स्थिर ओळीत माप_प्रकार pack_list_size(संरचना pack_list *pl)
{
	माप_प्रकार ret = 0;
	पर्यंत (pl) {
		ret++;
		pl = pl->next;
	}
	परत ret;
}

स्थिर संरचना pack_list * pack_list_difference(स्थिरांक संरचना pack_list *A,
					       स्थिरांक संरचना pack_list *B)
{
	संरचना pack_list *ret;
	स्थिरांक संरचना pack_list *pl;

	जर (!A)
		परत रिक्त;

	pl = B;
	पर्यंत (pl != रिक्त) {
		जर (A->pack == pl->pack)
			परत pack_list_difference(A->next, B);
		pl = pl->next;
	}
	ret = xmalloc(आकार(संरचना pack_list));
	स_नकल(ret, A, आकार(संरचना pack_list));
	ret->next = pack_list_difference(A->next, B);
	परत ret;
}

स्थिर शून्य cmp_two_packs(संरचना pack_list *p1, संरचना pack_list *p2)
{
	माप_प्रकार p1_off = 0, p2_off = 0, p1_step, p2_step;
	स्थिरांक अचिन्हित अक्षर *p1_base, *p2_base;
	संरचना llist_item *p1_hint = रिक्त, *p2_hint = रिक्त;
	स्थिरांक अचिन्हित पूर्णांक hashsz = the_hash_algo->rawsz;

	जर (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	जर (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->index_data;
	p2_base = p2->pack->index_data;
	p1_base += 256 * 4 + ((p1->pack->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->index_version < 2) ? 4 : 0);

	पर्यंत (p1_off < p1->pack->अंक_objects * p1_step &&
	       p2_off < p2->pack->अंक_objects * p2_step)
	{
		स्थिरांक पूर्णांक cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		जर (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			सुरूठेवा;
		}
		जर (cmp < 0) { /* p1 has the object, p2 doesn't */
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
	स्थिरांक अचिन्हित अक्षर *p1_base, *p2_base;
	स्थिरांक अचिन्हित पूर्णांक hashsz = the_hash_algo->rawsz;

	p1_base = p1->index_data;
	p2_base = p2->index_data;
	p1_base += 256 * 4 + ((p1->index_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->index_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->index_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->index_version < 2) ? 4 : 0);

	पर्यंत (p1_off < p1->अंक_objects * p1_step &&
	       p2_off < p2->अंक_objects * p2_step)
	{
		पूर्णांक cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		जर (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			सुरूठेवा;
		}
		जर (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} अन्यथा { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	परत ret;
}

/* another O(n^2) function ... */
स्थिर माप_प्रकार get_pack_redundancy(संरचना pack_list *pl)
{
	संरचना pack_list *subset;
	माप_प्रकार ret = 0;

	जर (!pl)
		परत 0;

	पर्यंत ((subset = pl->next)) {
		पर्यंत (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->next;
		}
		pl = pl->next;
	}
	परत ret;
}

स्थिर ओळीत off_t pack_set_bytecount(संरचना pack_list *pl)
{
	off_t ret = 0;
	पर्यंत (pl) {
		ret += pl->pack->pack_size;
		ret += pl->pack->index_size;
		pl = pl->next;
	}
	परत ret;
}

स्थिर पूर्णांक cmp_remaining_objects(स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
	संरचना pack_list *pl_a = *((संरचना pack_list **)a);
	संरचना pack_list *pl_b = *((संरचना pack_list **)b);

	जर (pl_a->remaining_objects->size == pl_b->remaining_objects->size) {
		/* have the same remaining_objects, big pack first */
		जर (pl_a->all_objects_size == pl_b->all_objects_size)
			परत 0;
		अन्यथा जर (pl_a->all_objects_size < pl_b->all_objects_size)
			परत 1;
		अन्यथा
			परत -1;
	} अन्यथा जर (pl_a->remaining_objects->size < pl_b->remaining_objects->size) {
		/* sort by remaining objects, more objects first */
		परत 1;
	} अन्यथा {
		परत -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
स्थिर शून्य sort_pack_list(संरचना pack_list **pl)
{
	संरचना pack_list **ary, *p;
	माप_प्रकार n = pack_list_size(*pl);

	जर (n < 2)
		परत;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	साठी (n = 0, p = *pl; p; p = p->next)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	साठी (माप_प्रकार i = 0; i < n - 1; i++)
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
	पर्यंत (pl) {
		जर (pl->unique_objects->size)
			pack_list_घाला(&unique, pl);
		अन्यथा
			pack_list_घाला(&non_unique, pl);
		pl = pl->next;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	पर्यंत (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->next;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	जर (missing->size == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		परत;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	पर्यंत (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->next;
	}

	पर्यंत (non_unique) {
		संरचना pack_list *next;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		जर (non_unique->remaining_objects->size == 0)
			थांबा;

		pack_list_घाला(min, non_unique);

		साठी (pl = non_unique->next; pl && pl->remaining_objects->size > 0;  pl = pl->next)
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

	पर्यंत (pl) {
		hint = रिक्त;
		l = pl->remaining_objects->front;
		पर्यंत (l) {
			hint = llist_घाला_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->next;
		}
		pl = pl->next;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	पर्यंत (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->next;
	}
}

/* this scales like O(n^2) */
स्थिर शून्य cmp_local_packs(शून्य)
{
	संरचना pack_list *subset, *pl = local_packs;

	/* only one packfile */
	जर (!pl->next) {
		llist_init(&pl->unique_objects);
		परत;
	}

	पर्यंत ((subset = pl)) {
		पर्यंत ((subset = subset->next))
			cmp_two_packs(pl, subset);
		pl = pl->next;
	}
}

स्थिर शून्य scan_alt_odb_packs(शून्य)
{
	संरचना pack_list *local, *alt;

	alt = altodb_packs;
	पर्यंत (alt) {
		local = local_packs;
		पर्यंत (local) {
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
	स्थिरांक अचिन्हित अक्षर *base;

	जर (!p->pack_local && !(alt_odb || verbose))
		परत रिक्त;

	l.pack = p;
	llist_init(&l.remaining_objects);

	जर (open_pack_index(p)) {
		llist_free(l.remaining_objects);
		परत रिक्त;
	}

	base = p->index_data;
	base += 256 * 4 + ((p->index_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->index_version < 2) ? 4 : 0);
	पर्यंत (off < p->अंक_objects * step) {
		llist_घाला_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_size = l.remaining_objects->size;
	l.unique_objects = रिक्त;
	जर (p->pack_local)
		परत pack_list_घाला(&local_packs, &l);
	अन्यथा
		परत pack_list_घाला(&altodb_packs, &l);
}

स्थिर संरचना pack_list * add_pack_file(स्थिरांक अक्षर *filename)
{
	संरचना packed_git *p;

	जर (म_माप(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		जर (म_माला(p->pack_name, filename))
			परत add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

स्थिर शून्य load_all(शून्य)
{
	संरचना packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

पूर्णांक cmd_pack_redundant(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix UNUSED, संरचना repository *repo UNUSED) {
	पूर्णांक i; पूर्णांक i_still_use_this = 0; संरचना pack_list *min = रिक्त, *red, *pl;
	संरचना llist *ignore;
	संरचना strbuf idx_name = STRBUF_INIT;
	अक्षर buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	साठी (i = 1; i < argc; i++) {
		स्थिरांक अक्षर *arg = argv[i];
		जर (!म_भेद(arg, "--")) {
			i++;
			थांबा;
		}
		जर (!म_भेद(arg, "--all")) {
			load_all_packs = 1;
			सुरूठेवा;
		}
		जर (!म_भेद(arg, "--verbose")) {
			verbose = 1;
			सुरूठेवा;
		}
		जर (!म_भेद(arg, "--alt-odb")) {
			alt_odb = 1;
			सुरूठेवा;
		}
		जर (!म_भेद(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			सुरूठेवा;
		}
		जर (*arg == '-')
			usage(pack_redundant_usage);
		अन्यथा
			थांबा;
	}

	जर (!i_still_use_this)
		you_still_use_that("git pack-redundant", रिक्त);

	जर (load_all_packs)
		load_all();
	अन्यथा
		पर्यंत (*(argv + i) != रिक्त)
			add_pack_file(*(argv + i++));

	जर (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	जर (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	जर (!isatty(0)) {
		संरचना object_id oid;
		पर्यंत (ख_माला_लो(buf, आकार(buf), मानक_निवेश)) {
			जर (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_घाला_sorted_unique(ignore, &oid, रिक्त);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	पर्यंत (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->next;
	}

	cmp_local_packs();

	minimize(&min);

	जर (verbose) {
		ख_लिखो(मानक_त्रुटि, "There are %lu packs available in alt-odbs.\n",
			(अचिन्हित दीर्घ)pack_list_size(altodb_packs));
		ख_लिखो(मानक_त्रुटि, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		पर्यंत (pl) {
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
	पर्यंत (pl) {
		म_लिखो("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->next;
	}
	जर (verbose)
		ख_लिखो(मानक_त्रुटि, "%luMB of redundant packs in total.\n",
			(अचिन्हित दीर्घ)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	परत 0;
}
