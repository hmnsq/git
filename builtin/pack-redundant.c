/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#परिभाषय USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"

#अन्तर्भाव "packfile.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "strbuf.h"

#परिभाषय BLKSIZE 512

स्थिर ध्रुव वर्ण pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

स्थिर पूर्णाङ्क आरोपय_all_packs, verbose, alt_odb;

संरचना llist_item {
	संरचना llist_item *अग्र;
	संरचना object_id oid;
};
स्थिर संरचना llist {
	संरचना llist_item *front;
	संरचना llist_item *पृष्ठभागः;
	आकार_t आकार;
} *all_objects; /* all objects which must be present in local packfiles */

स्थिर संरचना pack_list {
	संरचना pack_list *अग्र;
	संरचना packed_git *pack;
	संरचना llist *unique_objects;
	संरचना llist *remaining_objects;
	आकार_t all_objects_आकार;
} *local_packs = रिक्त, *altodb_packs = रिक्त;

स्थिर संरचना llist_item *free_nodes;

स्थिर अन्तःस्थ शून्य llist_item_put(संरचना llist_item *item)
{
	item->अग्र = free_nodes;
	free_nodes = item;
}

स्थिर अन्तःस्थ संरचना llist_item *llist_item_get(शून्य)
{
	संरचना llist_item *new_item;
	यदि ( free_nodes ) {
		new_item = free_nodes;
		free_nodes = free_nodes->अग्र;
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
	(*list)->front = (*list)->पृष्ठभागः = रिक्त;
	(*list)->आकार = 0;
}

स्थिर शून्य llist_free(संरचना llist *list)
{
	प्रति (संरचना llist_item *i = list->front, *अग्र; i; i = अग्र) {
		अग्र = i->अग्र;
		llist_item_put(i);
	}
	मुक्त(list);
}

स्थिर संरचना llist * llist_copy(संरचना llist *list)
{
	संरचना llist *ret;
	संरचना llist_item *new_item, *old_item, *पूर्व;

	llist_init(&ret);

	यदि ((ret->आकार = list->आकार) == 0)
		प्रत्यागम ret;

	new_item = ret->front = llist_item_get();
	new_item->oid = list->front->oid;

	old_item = list->front->अग्र;
	यावत् (old_item) {
		पूर्व = new_item;
		new_item = llist_item_get();
		पूर्व->अग्र = new_item;
		new_item->oid = old_item->oid;
		old_item = old_item->अग्र;
	}
	new_item->अग्र = रिक्त;
	ret->पृष्ठभागः = new_item;

	प्रत्यागम ret;
}

स्थिर अन्तःस्थ संरचना llist_item *llist_निवेशय(संरचना llist *list,
					      संरचना llist_item *after,
					      ध्रुव अचिह्नित वर्ण *oid)
{
	संरचना llist_item *new_item = llist_item_get();
	oidread(&new_item->oid, oid, the_repository->hash_algo);
	new_item->अग्र = रिक्त;

	यदि (after) {
		new_item->अग्र = after->अग्र;
		after->अग्र = new_item;
		यदि (after == list->पृष्ठभागः)
			list->पृष्ठभागः = new_item;
	} अन्यथा {/* insert in front */
		यदि (list->आकार == 0)
			list->पृष्ठभागः = new_item;
		अन्यथा
			new_item->अग्र = list->front;
		list->front = new_item;
	}
	list->आकार++;
	प्रत्यागम new_item;
}

स्थिर अन्तःस्थ संरचना llist_item *llist_निवेशय_पृष्ठभागः(संरचना llist *list,
						   ध्रुव अचिह्नित वर्ण *oid)
{
	प्रत्यागम llist_निवेशय(list, list->पृष्ठभागः, oid);
}

स्थिर अन्तःस्थ संरचना llist_item *llist_निवेशय_sorted_unique(संरचना llist *list,
			ध्रुव संरचना object_id *oid, संरचना llist_item *hint)
{
	संरचना llist_item *पूर्व = रिक्त, *l;

	l = (hint == रिक्त) ? list->front : hint;
	यावत् (l) {
		पूर्णाङ्क cmp = oidcmp(&l->oid, oid);
		यदि (cmp > 0) { /* we insert before this entry */
			प्रत्यागम llist_निवेशय(list, पूर्व, oid->hash);
		}
		यदि (!cmp) { /* already exists */
			प्रत्यागम l;
		}
		पूर्व = l;
		l = l->अग्र;
	}
	/* insert at the end */
	प्रत्यागम llist_निवेशय_पृष्ठभागः(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
स्थिर अन्तःस्थ संरचना llist_item * llist_sorted_remove(संरचना llist *list, ध्रुव अचिह्नित वर्ण *oid, संरचना llist_item *hint)
{
	संरचना llist_item *पूर्व, *l;

redo_from_start:
	l = (hint == रिक्त) ? list->front : hint;
	पूर्व = रिक्त;
	यावत् (l) {
		ध्रुव पूर्णाङ्क cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		यदि (cmp > 0) /* not in list, since sorted */
			प्रत्यागम पूर्व;
		यदि (!cmp) { /*प्राप्तम्*/
			यदि (!पूर्व) {
				यदि (hint != रिक्त && hint != list->front) {
					/* we don't know the previous element */
					hint = रिक्त;
					गच्छ redo_from_start;
				}
				list->front = l->अग्र;
			} अन्यथा
				पूर्व->अग्र = l->अग्र;
			यदि (l == list->पृष्ठभागः)
				list->पृष्ठभागः = पूर्व;
			llist_item_put(l);
			list->आकार--;
			प्रत्यागम पूर्व;
		}
		पूर्व = l;
		l = l->अग्र;
	}
	प्रत्यागम पूर्व;
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
		b = b->अग्र;
	}
}

स्थिर अन्तःस्थ संरचना pack_list * pack_list_निवेशय(संरचना pack_list **pl,
					   संरचना pack_list *प्रविष्टिः)
{
	संरचना pack_list *p = xmalloc(परिमाण(संरचना pack_list));
	स्मृतिं_प्रतिलिख(p, प्रविष्टिः, परिमाण(संरचना pack_list));
	p->अग्र = *pl;
	*pl = p;
	प्रत्यागम p;
}

स्थिर शून्य pack_list_free(संरचना pack_list *pl)
{
	प्रति (संरचना pack_list *अग्र; pl; pl = अग्र) {
		अग्र = pl->अग्र;
		मुक्त(pl);
	}
}

स्थिर अन्तःस्थ आकार_t pack_list_आकार(संरचना pack_list *pl)
{
	आकार_t ret = 0;
	यावत् (pl) {
		ret++;
		pl = pl->अग्र;
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
			प्रत्यागम pack_list_difference(A->अग्र, B);
		pl = pl->अग्र;
	}
	ret = xmalloc(परिमाण(संरचना pack_list));
	स्मृतिं_प्रतिलिख(ret, A, परिमाण(संरचना pack_list));
	ret->अग्र = pack_list_difference(A->अग्र, B);
	प्रत्यागम ret;
}

स्थिर शून्य cmp_two_packs(संरचना pack_list *p1, संरचना pack_list *p2)
{
	आकार_t p1_off = 0, p2_off = 0, p1_चरणः, p2_चरणः;
	ध्रुव अचिह्नित वर्ण *p1_आधारः, *p2_आधारः;
	संरचना llist_item *p1_hint = रिक्त, *p2_hint = रिक्त;
	ध्रुव अचिह्नित पूर्णाङ्क hashsz = the_hash_algo->rawsz;

	यदि (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->remaining_objects);
	यदि (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->remaining_objects);

	p1_आधारः = p1->pack->सूचकांक_दत्त;
	p2_आधारः = p2->pack->सूचकांक_दत्त;
	p1_आधारः += 256 * 4 + ((p1->pack->सूचकांक_संस्करणम् < 2) ? 4 : 8);
	p2_आधारः += 256 * 4 + ((p2->pack->सूचकांक_संस्करणम् < 2) ? 4 : 8);
	p1_चरणः = hashsz + ((p1->pack->सूचकांक_संस्करणम् < 2) ? 4 : 0);
	p2_चरणः = hashsz + ((p2->pack->सूचकांक_संस्करणम् < 2) ? 4 : 0);

	यावत् (p1_off < p1->pack->अङ्कः_objects * p1_चरणः &&
	       p2_off < p2->pack->अङ्कः_objects * p2_चरणः)
	{
		ध्रुव पूर्णाङ्क cmp = hashcmp(p1_आधारः + p1_off, p2_आधारः + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		यदि (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_आधारः + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_आधारः + p1_off,
					p2_hint);
			p1_off += p1_चरणः;
			p2_off += p2_चरणः;
			अनुवर्त;
		}
		यदि (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_चरणः;
		} अन्यथा { /* p2 has the object, p1 doesn't */
			p2_off += p2_चरणः;
		}
	}
}

स्थिर आकार_t sizeof_union(संरचना packed_git *p1, संरचना packed_git *p2)
{
	आकार_t ret = 0;
	आकार_t p1_off = 0, p2_off = 0, p1_चरणः, p2_चरणः;
	ध्रुव अचिह्नित वर्ण *p1_आधारः, *p2_आधारः;
	ध्रुव अचिह्नित पूर्णाङ्क hashsz = the_hash_algo->rawsz;

	p1_आधारः = p1->सूचकांक_दत्त;
	p2_आधारः = p2->सूचकांक_दत्त;
	p1_आधारः += 256 * 4 + ((p1->सूचकांक_संस्करणम् < 2) ? 4 : 8);
	p2_आधारः += 256 * 4 + ((p2->सूचकांक_संस्करणम् < 2) ? 4 : 8);
	p1_चरणः = hashsz + ((p1->सूचकांक_संस्करणम् < 2) ? 4 : 0);
	p2_चरणः = hashsz + ((p2->सूचकांक_संस्करणम् < 2) ? 4 : 0);

	यावत् (p1_off < p1->अङ्कः_objects * p1_चरणः &&
	       p2_off < p2->अङ्कः_objects * p2_चरणः)
	{
		पूर्णाङ्क cmp = hashcmp(p1_आधारः + p1_off, p2_आधारः + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		यदि (cmp == 0) {
			ret++;
			p1_off += p1_चरणः;
			p2_off += p2_चरणः;
			अनुवर्त;
		}
		यदि (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_चरणः;
		} अन्यथा { /* p2 has the object, p1 doesn't */
			p2_off += p2_चरणः;
		}
	}
	प्रत्यागम ret;
}

/* another O(n^2) function ... */
स्थिर आकार_t get_pack_redundancy(संरचना pack_list *pl)
{
	संरचना pack_list *subset;
	आकार_t ret = 0;

	यदि (!pl)
		प्रत्यागम 0;

	यावत् ((subset = pl->अग्र)) {
		यावत् (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->अग्र;
		}
		pl = pl->अग्र;
	}
	प्रत्यागम ret;
}

स्थिर अन्तःस्थ off_t pack_समुच्चयः_bytecount(संरचना pack_list *pl)
{
	off_t ret = 0;
	यावत् (pl) {
		ret += pl->pack->pack_आकार;
		ret += pl->pack->सूचकांक_आकार;
		pl = pl->अग्र;
	}
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmp_remaining_objects(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	संरचना pack_list *pl_a = *((संरचना pack_list **)a);
	संरचना pack_list *pl_b = *((संरचना pack_list **)b);

	यदि (pl_a->remaining_objects->आकार == pl_b->remaining_objects->आकार) {
		/* have the same remaining_objects, big pack first */
		यदि (pl_a->all_objects_आकार == pl_b->all_objects_आकार)
			प्रत्यागम 0;
		अन्यथा यदि (pl_a->all_objects_आकार < pl_b->all_objects_आकार)
			प्रत्यागम 1;
		अन्यथा
			प्रत्यागम -1;
	} अन्यथा यदि (pl_a->remaining_objects->आकार < pl_b->remaining_objects->आकार) {
		/* sort by remaining objects, more objects first */
		प्रत्यागम 1;
	} अन्यथा {
		प्रत्यागम -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
स्थिर शून्य क्रमय_pack_list(संरचना pack_list **pl)
{
	संरचना pack_list **ary, *p;
	आकार_t n = pack_list_आकार(*pl);

	यदि (n < 2)
		प्रत्यागम;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	प्रति (n = 0, p = *pl; p; p = p->अग्र)
		ary[n++] = p;

	QSORT(ary, n, cmp_remaining_objects);

	/* link them back again */
	प्रति (आकार_t i = 0; i < n - 1; i++)
		ary[i]->अग्र = ary[i + 1];
	ary[n - 1]->अग्र = रिक्त;
	*pl = ary[0];

	मुक्त(ary);
}


स्थिर शून्य minimize(संरचना pack_list **min)
{
	संरचना pack_list *pl, *unique = रिक्त, *non_unique = रिक्त;
	संरचना llist *missing, *unique_pack_objects;

	pl = local_packs;
	यावत् (pl) {
		यदि (pl->unique_objects->आकार)
			pack_list_निवेशय(&unique, pl);
		अन्यथा
			pack_list_निवेशय(&non_unique, pl);
		pl = pl->अग्र;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	यावत् (pl) {
		llist_sorted_difference_inplace(missing, pl->remaining_objects);
		pl = pl->अग्र;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	यदि (missing->आकार == 0) {
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
		pl = pl->अग्र;
	}

	यावत् (non_unique) {
		संरचना pack_list *अग्र;

		/* sort the non_unique packs, greater size of remaining_objects first */
		क्रमय_pack_list(&non_unique);
		यदि (non_unique->remaining_objects->आकार == 0)
			भङ्ग;

		pack_list_निवेशय(min, non_unique);

		प्रति (pl = non_unique->अग्र; pl && pl->remaining_objects->आकार > 0;  pl = pl->अग्र)
			llist_sorted_difference_inplace(pl->remaining_objects, non_unique->remaining_objects);

		अग्र = non_unique->अग्र;
		मुक्त(non_unique);
		non_unique = अग्र;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

स्थिर शून्य आरोपय_all_objects(शून्य)
{
	संरचना pack_list *pl = local_packs;
	संरचना llist_item *hint, *l;

	llist_init(&all_objects);

	यावत् (pl) {
		hint = रिक्त;
		l = pl->remaining_objects->front;
		यावत् (l) {
			hint = llist_निवेशय_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->अग्र;
		}
		pl = pl->अग्र;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	यावत् (pl) {
		llist_sorted_difference_inplace(all_objects, pl->remaining_objects);
		pl = pl->अग्र;
	}
}

/* this scales like O(n^2) */
स्थिर शून्य cmp_local_packs(शून्य)
{
	संरचना pack_list *subset, *pl = local_packs;

	/* only one packfile */
	यदि (!pl->अग्र) {
		llist_init(&pl->unique_objects);
		प्रत्यागम;
	}

	यावत् ((subset = pl)) {
		यावत् ((subset = subset->अग्र))
			cmp_two_packs(pl, subset);
		pl = pl->अग्र;
	}
}

स्थिर शून्य अवलोकनम्_alt_odb_packs(शून्य)
{
	संरचना pack_list *local, *alt;

	alt = altodb_packs;
	यावत् (alt) {
		local = local_packs;
		यावत् (local) {
			llist_sorted_difference_inplace(local->remaining_objects,
							alt->remaining_objects);
			local = local->अग्र;
		}
		alt = alt->अग्र;
	}
}

स्थिर संरचना pack_list * सङ्कलय_pack(संरचना packed_git *p)
{
	संरचना pack_list l;
	आकार_t off = 0, चरणः;
	ध्रुव अचिह्नित वर्ण *आधारः;

	यदि (!p->pack_local && !(alt_odb || verbose))
		प्रत्यागम रिक्त;

	l.pack = p;
	llist_init(&l.remaining_objects);

	यदि (open_pack_सूचकांक(p)) {
		llist_free(l.remaining_objects);
		प्रत्यागम रिक्त;
	}

	आधारः = p->सूचकांक_दत्त;
	आधारः += 256 * 4 + ((p->सूचकांक_संस्करणम् < 2) ? 4 : 8);
	चरणः = the_hash_algo->rawsz + ((p->सूचकांक_संस्करणम् < 2) ? 4 : 0);
	यावत् (off < p->अङ्कः_objects * चरणः) {
		llist_निवेशय_पृष्ठभागः(l.remaining_objects, आधारः + off);
		off += चरणः;
	}
	l.all_objects_आकार = l.remaining_objects->आकार;
	l.unique_objects = रिक्त;
	यदि (p->pack_local)
		प्रत्यागम pack_list_निवेशय(&local_packs, &l);
	अन्यथा
		प्रत्यागम pack_list_निवेशय(&altodb_packs, &l);
}

स्थिर संरचना pack_list * सङ्कलय_pack_file(ध्रुव वर्ण *filename)
{
	संरचना packed_git *p;

	यदि (वाक्यदैर्घ्यम्(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		यदि (वाक्ये_अन्विष(p->pack_name, filename))
			प्रत्यागम सङ्कलय_pack(p);
	die("Filename %s not found in packed_git", filename);
}

स्थिर शून्य आरोपय_all(शून्य)
{
	संरचना packed_git *p;

	repo_for_each_pack(the_repository, p)
		सङ्कलय_pack(p);
}

पूर्णाङ्क cmd_pack_redundant(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्, संरचना repository *repo अप्रयुक्तम्) {
	पूर्णाङ्क i; पूर्णाङ्क i_still_use_this = 0; संरचना pack_list *min = रिक्त, *red, *pl;
	संरचना llist *ignore;
	संरचना strbuf idx_name = STRBUF_INIT;
	वर्ण buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	प्रति (i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		यदि (!वाक्यं_तुलय(arg, "--")) {
			i++;
			भङ्ग;
		}
		यदि (!वाक्यं_तुलय(arg, "--all")) {
			आरोपय_all_packs = 1;
			अनुवर्त;
		}
		यदि (!वाक्यं_तुलय(arg, "--verbose")) {
			verbose = 1;
			अनुवर्त;
		}
		यदि (!वाक्यं_तुलय(arg, "--alt-odb")) {
			alt_odb = 1;
			अनुवर्त;
		}
		यदि (!वाक्यं_तुलय(arg, "--i-still-use-this")) {
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

	यदि (आरोपय_all_packs)
		आरोपय_all();
	अन्यथा
		यावत् (*(argv + i) != रिक्त)
			सङ्कलय_pack_file(*(argv + i++));

	यदि (!local_packs)
		die("Zero packs found!");

	आरोपय_all_objects();

	यदि (alt_odb)
		अवलोकनम्_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	यदि (!isatty(0)) {
		संरचना object_id oid;
		यावत् (fgets(buf, परिमाण(buf), stdin)) {
			यदि (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_निवेशय_sorted_unique(ignore, &oid, रिक्त);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	यावत् (pl) {
		llist_sorted_difference_inplace(pl->remaining_objects, ignore);
		pl = pl->अग्र;
	}

	cmp_local_packs();

	minimize(&min);

	यदि (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(अचिह्नित दीर्घ)pack_list_आकार(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		यावत् (pl) {
			fprintf(stderr, "\t%s\n", pl->pack->pack_name);
			pl = pl->अग्र;
		}
		fprintf(stderr, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(अचिह्नित दीर्घ)get_pack_redundancy(min),
			(अचिह्नित दीर्घ)pack_समुच्चयः_bytecount(min)/1024);
		fprintf(stderr, "A total of %lu unique objects were considered.\n",
			(अचिह्नित दीर्घ)all_objects->आकार);
		fprintf(stderr, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	यावत् (pl) {
		प्रारूप_लिख("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->अग्र;
	}
	यदि (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(अचिह्नित दीर्घ)pack_समुच्चयः_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	प्रत्यागम 0;
}
