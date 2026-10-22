/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "gettext.h"
#समावेश "hex.h"

#समावेश "packfile.h"
#समावेश "odb.h"
#समावेश "strbuf.h"

#घोषणा BLKSIZE 512

अटल स्थिर अक्षर pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

अटल पूर्णांक load_all_packs, verbose, alt_odb;

काष्ठा llist_item {
	काष्ठा llist_item *अग्र;
	काष्ठा object_id oid;
};
अटल काष्ठा llist {
	काष्ठा llist_item *front;
	काष्ठा llist_item *back;
	माप_प्रकार आकार;
} *all_objects; /* all objects which must be present in local packfiles */

अटल काष्ठा pack_list {
	काष्ठा pack_list *अग्र;
	काष्ठा packed_git *pack;
	काष्ठा llist *unique_objects;
	काष्ठा llist *remaining_objects;
	माप_प्रकार all_objects_आकार;
} *local_packs = शून्य, *altodb_packs = शून्य;

अटल काष्ठा llist_item *free_nodes;

अटल अंतरभूत व्योम llist_item_put(काष्ठा llist_item *item)
{
	item->अग्र = free_nodes;
	free_nodes = item;
}

अटल अंतरभूत काष्ठा llist_item *llist_item_get(व्योम)
{
	काष्ठा llist_item *new_item;
	अगर ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->अग्र;
	} अन्यथा {
		पूर्णांक i = 1;
		ALLOC_ARRAY(new_item, BLKSIZE);
		क्रम (; i < BLKSIZE; i++)
			llist_item_put(&new_item[i]);
	}
	वापस new_item;
}

अटल अंतरभूत व्योम llist_init(काष्ठा llist **list)
{
	*list = xmalloc(माप(काष्ठा llist));
	(*list)->front = (*list)->back = शून्य;
	(*list)->आकार = 0;
}

अटल व्योम llist_free(काष्ठा llist *list)
{
	क्रम (काष्ठा llist_item *i = list->front, *अग्र; i; i = अग्र) {
		अग्र = i->अग्र;
		llist_item_put(i);
	}
	मुक्त(list);
}

अटल काष्ठा llist * llist_copy(काष्ठा llist *list)
{
	काष्ठा llist *ret;
	काष्ठा llist_item *new_item, *old_item, *पूर्व;

	llist_init(&ret);

	अगर ((ret->आकार = list->आकार) == 0)
		वापस ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->अग्र;
	जबतक (old_item) {
		पूर्व = new_item;
		new_item = llist_item_get();
		पूर्व->अग्र = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->अग्र;
	}
	new_item->अग्र = शून्य;
	ret->back = new_item;

	वापस ret;
}

अटल अंतरभूत काष्ठा llist_item *llist_जोड़ो(काष्ठा llist *list,
					      काष्ठा llist_item *after,
					      स्थिर अचिन्हित अक्षर *oid)
{
	काष्ठा llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->अग्र = शून्य;

	अगर (after) {
		new_item->अग्र = after->अग्र;
		after->अग्र = new_item;
		अगर (after == list->back)
			list->back = new_item;
	} अन्यथा {/* insert in front */
		अगर (list->आकार == 0)
			list->back = new_item;
		अन्यथा
			new_item->अग्र = list->front;
		list->front = new_item;
	}
	list->आकार++;
	वापस new_item;
}

अटल अंतरभूत काष्ठा llist_item *llist_जोड़ो_back(काष्ठा llist *list,
						   स्थिर अचिन्हित अक्षर *oid)
{
	वापस llist_जोड़ो(list, list->back, oid);
}

अटल अंतरभूत काष्ठा llist_item *llist_जोड़ो_sorted_unique(काष्ठा llist *list,
			स्थिर काष्ठा object_id *oid, काष्ठा llist_item *hint)
{
	काष्ठा llist_item *पूर्व = शून्य, *l;

	l = (hint == शून्य) ? list->front : hint;
	जबतक (l) {
		पूर्णांक cmp = oidcmp(&l->oid, oid);
		अगर (cmp > 0) { /* we insert before this entry */
			वापस llist_जोड़ो(list, पूर्व, oid->hash);
		}
		अगर (!cmp) { /* already exists */
			वापस l;
		}
		पूर्व = l;
		l = l->अग्र;
	}
	/* insert at the end */
	वापस llist_जोड़ो_back(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
अटल अंतरभूत काष्ठा llist_item * llist_sorted_remove(काष्ठा llist *list, स्थिर अचिन्हित अक्षर *oid, काष्ठा llist_item *hint)
{
	काष्ठा llist_item *पूर्व, *l;

redo_from_start:
	l = (hint == शून्य) ? list->front : hint;
	पूर्व = शून्य;
	जबतक (l) {
		स्थिर पूर्णांक cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		अगर (cmp > 0) /* not in list, since sorted */
			वापस पूर्व;
		अगर (!cmp) { /* found */
			अगर (!पूर्व) {
				अगर (hint != शून्य && hint != list->front) {
					/* we don't know the previous element */
					hint = शून्य;
					जाओ redo_from_start;
				}
				list->front = l->अग्र;
			} अन्यथा
				पूर्व->अग्र = l->अग्र;
			अगर (l == list->back)
				list->back = पूर्व;
			llist_item_put(l);
			list->आकार--;
			वापस पूर्व;
		}
		पूर्व = l;
		l = l->अग्र;
	}
	वापस पूर्व;
}

/* computes A\B */
अटल व्योम llist_sorted_difference_inplace(काष्ठा llist *A,
				     काष्ठा llist *B)
{
	काष्ठा llist_item *hint, *b;

	hint = शून्य;
	b = B->front;

	जबतक (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->अग्र;
	}
}

अटल अंतरभूत काष्ठा pack_list * pack_list_जोड़ो(काष्ठा pack_list **pl,
					   काष्ठा pack_list *entry)
{
	काष्ठा pack_list *p = xmalloc(माप(काष्ठा pack_list));
	स_नकल(p, entry, माप(काष्ठा pack_list));
	p->अग्र = *pl;
	*pl = p;
	वापस p;
}

अटल व्योम pack_list_free(काष्ठा pack_list *pl)
{
	क्रम (काष्ठा pack_list *अग्र; pl; pl = अग्र) {
		अग्र = pl->अग्र;
		मुक्त(pl);
	}
}

अटल अंतरभूत माप_प्रकार pack_list_आकार(काष्ठा pack_list *pl)
{
	माप_प्रकार ret = 0;
	जबतक (pl) {
		ret++;
		pl = pl->अग्र;
	}
	वापस ret;
}

अटल काष्ठा pack_list * pack_list_difference(स्थिर काष्ठा pack_list *A,
					       स्थिर काष्ठा pack_list *B)
{
	काष्ठा pack_list *ret;
	स्थिर काष्ठा pack_list *pl;

	अगर (!A)
		वापस शून्य;

	pl = B;
	जबतक (pl != शून्य) {
		अगर (A->pack == pl->pack)
			वापस pack_list_difference(A->अग्र, B);
		pl = pl->अग्र;
	}
	ret = xmalloc(माप(काष्ठा pack_list));
	स_नकल(ret, A, माप(काष्ठा pack_list));
	ret->अग्र = pack_list_difference(A->अग्र, B);
	वापस ret;
}

अटल व्योम cmp_two_packs(काष्ठा pack_list *p1, काष्ठा pack_list *p2)
{
	माप_प्रकार p1_off = 0, p2_off = 0, p1_step, p2_step;
	स्थिर अचिन्हित अक्षर *p1_base, *p2_base;
	काष्ठा llist_item *p1_hint = शून्य, *p2_hint = शून्य;
	स्थिर अचिन्हित पूर्णांक hashsz = the_hash_algo->rawsz;

	अगर (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	अगर (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_base = p1->pack->सूचकांक_तथ्य;
	p2_base = p2->pack->सूचकांक_तथ्य;
	p1_base += 256 * 4 + ((p1->pack->सूचकांक_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->pack->सूचकांक_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->pack->सूचकांक_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->pack->सूचकांक_version < 2) ? 4 : 0);

	जबतक (p1_off < p1->pack->अंक_objects * p1_step &&
	       p2_off < p2->pack->अंक_objects * p2_step)
	{
		स्थिर पूर्णांक cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		अगर (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_base + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_base + p1_off,
					p2_hint);
			p1_off += p1_step;
			p2_off += p2_step;
			जारी;
		}
		अगर (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} अन्यथा { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
}

अटल माप_प्रकार sizeof_union(काष्ठा packed_git *p1, काष्ठा packed_git *p2)
{
	माप_प्रकार ret = 0;
	माप_प्रकार p1_off = 0, p2_off = 0, p1_step, p2_step;
	स्थिर अचिन्हित अक्षर *p1_base, *p2_base;
	स्थिर अचिन्हित पूर्णांक hashsz = the_hash_algo->rawsz;

	p1_base = p1->सूचकांक_तथ्य;
	p2_base = p2->सूचकांक_तथ्य;
	p1_base += 256 * 4 + ((p1->सूचकांक_version < 2) ? 4 : 8);
	p2_base += 256 * 4 + ((p2->सूचकांक_version < 2) ? 4 : 8);
	p1_step = hashsz + ((p1->सूचकांक_version < 2) ? 4 : 0);
	p2_step = hashsz + ((p2->सूचकांक_version < 2) ? 4 : 0);

	जबतक (p1_off < p1->अंक_objects * p1_step &&
	       p2_off < p2->अंक_objects * p2_step)
	{
		पूर्णांक cmp = hashcmp(p1_base + p1_off, p2_base + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		अगर (cmp == 0) {
			ret++;
			p1_off += p1_step;
			p2_off += p2_step;
			जारी;
		}
		अगर (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_step;
		} अन्यथा { /* p2 has the object, p1 doesn't */
			p2_off += p2_step;
		}
	}
	वापस ret;
}

/* another O(n^2) function ... */
अटल माप_प्रकार get_pack_redundancy(काष्ठा pack_list *pl)
{
	काष्ठा pack_list *subset;
	माप_प्रकार ret = 0;

	अगर (!pl)
		वापस 0;

	जबतक ((subset = pl->अग्र)) {
		जबतक (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->अग्र;
		}
		pl = pl->अग्र;
	}
	वापस ret;
}

अटल अंतरभूत off_t pack_set_bytecount(काष्ठा pack_list *pl)
{
	off_t ret = 0;
	जबतक (pl) {
		ret += pl->pack->pack_आकार;
		ret += pl->pack->सूचकांक_आकार;
		pl = pl->अग्र;
	}
	वापस ret;
}

अटल पूर्णांक cmp_remaining_objects(स्थिर व्योम *a, स्थिर व्योम *b)
{
	काष्ठा pack_list *pl_a = *((काष्ठा pack_list **)a);
	काष्ठा pack_list *pl_b = *((काष्ठा pack_list **)b);

	अगर (pl_a->remaining_objects->आकार == pl_b->remaining_objects->आकार) {
		/* have the same remaining_objects, big pack first */
		अगर (pl_a->all_objects_आकार == pl_b->all_objects_आकार)
			वापस 0;
		अन्यथा अगर (pl_a->all_objects_आकार < pl_b->all_objects_आकार)
			वापस 1;
		अन्यथा
			वापस -1;
	} अन्यथा अगर (pl_a->remaining_objects->आकार < pl_b->remaining_objects->आकार) {
		/* sort by remaining objects, more objects first */
		वापस 1;
	} अन्यथा {
		वापस -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
अटल व्योम sort_pack_list(काष्ठा pack_list **pl)
{
	काष्ठा pack_list **ary, *p;
	माप_प्रकार n = pack_list_आकार(*pl);

	अगर (n < 2)
		वापस;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	क्रम (n = 0, p = *pl; p; p = p->अग्र)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	क्रम (माप_प्रकार i = 0; i < n - 1; i++)
		ary[i]->अग्र = ary[i + 1];
	ary[n - 1]->अग्र = शून्य;
	*pl = ary[0];

	मुक्त(ary);
}


अटल व्योम minimize(काष्ठा pack_list **min)
{
	काष्ठा pack_list *pl, *unique = शून्य, *non_unique = शून्य;
	काष्ठा llist *missing, *unique_pack_objects;

	pl = local_packs;
	जबतक (pl) {
		अगर (pl->unique_objects->आकार)
			pack_list_जोड़ो(&unique, pl);
		अन्यथा
			pack_list_जोड़ो(&non_unique, pl);
		pl = pl->अग्र;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	जबतक (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->अग्र;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	अगर (missing->आकार == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		वापस;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	जबतक (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, unique_pack_objects);
		pl = pl->अग्र;
	}

	जबतक (non_unique) {
		काष्ठा pack_list *अग्र;

		/* sort the non_unique packs, greater size of remaining_objects first */
		sort_pack_list(&non_unique);
		अगर (non_unique->remaining_objects->आकार == 0)
			अवरोध;

		pack_list_जोड़ो(min, non_unique);

		क्रम (pl = non_unique->अग्र; pl && pl->remaining_objects->आकार > 0;  pl = pl->अग्र)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		अग्र = non_unique->अग्र;
		मुक्त(non_unique);
		non_unique = अग्र;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

अटल व्योम load_all_objects(व्योम)
{
	काष्ठा pack_list *pl = local_packs;
	काष्ठा llist_item *hint, *l;

	llist_init(&all_objects);

	जबतक (pl) {
		hint = शून्य;
		l = pl->remaining_objects->front;
		जबतक (l) {
			hint = llist_जोड़ो_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->अग्र;
		}
		pl = pl->अग्र;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	जबतक (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->अग्र;
	}
}

/* this scales like O(n^2) */
अटल व्योम cmp_local_packs(व्योम)
{
	काष्ठा pack_list *subset, *pl = local_packs;

	/* only one packfile */
	अगर (!pl->अग्र) {
		llist_init(&pl->unique_objects);
		वापस;
	}

	जबतक ((subset = pl)) {
		जबतक ((subset = subset->अग्र))
			cmp_two_packs(pl, subset);
		pl = pl->अग्र;
	}
}

अटल व्योम scan_alt_odb_packs(व्योम)
{
	काष्ठा pack_list *local, *alt;

	alt = altodb_packs;
	जबतक (alt) {
		local = local_packs;
		जबतक (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->अग्र;
		}
		alt = alt->अग्र;
	}
}

अटल काष्ठा pack_list * add_pack(काष्ठा packed_git *p)
{
	काष्ठा pack_list l;
	माप_प्रकार off = 0, step;
	स्थिर अचिन्हित अक्षर *base;

	अगर (!p->pack_local && !(alt_odb || verbose))
		वापस शून्य;

	l.pack = p;
	llist_init(&l.remaining_objects);

	अगर (open_pack_सूचकांक(p)) {
		llist_free(l.remaining_objects);
		वापस शून्य;
	}

	base = p->सूचकांक_तथ्य;
	base += 256 * 4 + ((p->सूचकांक_version < 2) ? 4 : 8);
	step = the_hash_algo->rawsz + ((p->सूचकांक_version < 2) ? 4 : 0);
	जबतक (off < p->अंक_objects * step) {
		llist_जोड़ो_back(l.remaining_objects, base + off);
		off += step;
	}
	l.all_objects_आकार = l.remaining_objects->आकार;
	l.unique_objects = शून्य;
	अगर (p->pack_local)
		वापस pack_list_जोड़ो(&local_packs, &l);
	अन्यथा
		वापस pack_list_जोड़ो(&altodb_packs, &l);
}

अटल काष्ठा pack_list * add_pack_file(स्थिर अक्षर *filename)
{
	काष्ठा packed_git *p;

	अगर (म_माप(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		अगर (म_माला(p->pack_name, filename))
			वापस add_pack(p);
	die("Filename %s not found in packed_git", filename);
}

अटल व्योम load_all(व्योम)
{
	काष्ठा packed_git *p;

	repo_for_each_pack(the_repository, p)
		add_pack(p);
}

पूर्णांक cmd_pack_redundant(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix UNUSED, काष्ठा repository *repo UNUSED) {
	पूर्णांक i; पूर्णांक i_still_use_this = 0; काष्ठा pack_list *min = शून्य, *red, *pl;
	काष्ठा llist *ignore;
	काष्ठा strbuf idx_name = STRBUF_INIT;
	अक्षर buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	क्रम (i = 1; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];
		अगर (!म_भेद(arg, "--")) {
			i++;
			अवरोध;
		}
		अगर (!म_भेद(arg, "--all")) {
			load_all_packs = 1;
			जारी;
		}
		अगर (!म_भेद(arg, "--verbose")) {
			verbose = 1;
			जारी;
		}
		अगर (!म_भेद(arg, "--alt-odb")) {
			alt_odb = 1;
			जारी;
		}
		अगर (!म_भेद(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			जारी;
		}
		अगर (*arg == '-')
			usage(pack_redundant_usage);
		अन्यथा
			अवरोध;
	}

	अगर (!i_still_use_this)
		you_still_use_that("git pack-redundant", शून्य);

	अगर (load_all_packs)
		load_all();
	अन्यथा
		जबतक (*(argv + i) != शून्य)
			add_pack_file(*(argv + i++));

	अगर (!local_packs)
		die("Zero packs found!");

	load_all_objects();

	अगर (alt_odb)
		scan_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	अगर (!isatty(0)) {
		काष्ठा object_id oid;
		जबतक (ख_माला_लो(buf, माप(buf), मानक_निवेश)) {
			अगर (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_जोड़ो_sorted_unique(ignore, &oid, शून्य);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	जबतक (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->अग्र;
	}

	cmp_local_packs();

	minimize(&min);

	अगर (verbose) {
		ख_लिखो(मानक_त्रुटि, "There are %lu packs available in alt-odbs.\n",
			(अचिन्हित दीर्घ)pack_list_आकार(altodb_packs));
		ख_लिखो(मानक_त्रुटि, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		जबतक (pl) {
			ख_लिखो(मानक_त्रुटि, "\t%s\n", pl->pack->pack_name);
			pl = pl->अग्र;
		}
		ख_लिखो(मानक_त्रुटि, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(अचिन्हित दीर्घ)get_pack_redundancy(min),
			(अचिन्हित दीर्घ)pack_set_bytecount(min)/1024);
		ख_लिखो(मानक_त्रुटि, "A total of %lu unique objects were considered.\n",
			(अचिन्हित दीर्घ)all_objects->आकार);
		ख_लिखो(मानक_त्रुटि, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	जबतक (pl) {
		म_लिखो("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->अग्र;
	}
	अगर (verbose)
		ख_लिखो(मानक_त्रुटि, "%luMB of redundant packs in total.\n",
			(अचिन्हित दीर्घ)pack_set_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	वापस 0;
}
