/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"

#అన్తర్భావ "packfile.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "strbuf.h"

#నిర్వచించు BLKSIZE 512

స్థిర స్థిరరాశి అక్షరం pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

స్థిర పూర్ణాంకం ఎక్కించు_all_packs, verbose, alt_odb;

నిర్మాణం llist_అంశం {
	నిర్మాణం llist_అంశం *తదుపరి;
	నిర్మాణం object_id oid;
};
స్థిర నిర్మాణం llist {
	నిర్మాణం llist_అంశం *front;
	నిర్మాణం llist_అంశం *వెనుక;
	కొలత_t కొలత;
} *all_objects; /* all objects which must be present in local packfiles */

స్థిర నిర్మాణం pack_list {
	నిర్మాణం pack_list *తదుపరి;
	నిర్మాణం packed_git *pack;
	నిర్మాణం llist *unique_objects;
	నిర్మాణం llist *మిగిలిన_objects;
	కొలత_t all_objects_కొలత;
} *local_packs = ఖాళీ, *altodb_packs = ఖాళీ;

స్థిర నిర్మాణం llist_అంశం *free_nodes;

స్థిర అంతఃవరుస శూన్య llist_అంశం_put(నిర్మాణం llist_అంశం *అంశం)
{
	అంశం->తదుపరి = free_nodes;
	free_nodes = అంశం;
}

స్థిర అంతఃవరుస నిర్మాణం llist_అంశం *llist_అంశం_get(శూన్య)
{
	నిర్మాణం llist_అంశం *new_అంశం;
	ఒకవేళ ( free_nodes ) {
		new_అంశం = free_nodes;
		free_nodes = free_nodes->తదుపరి;
	} లేకపోతే {
		పూర్ణాంకం i = 1;
		ALLOC_ARRAY(new_అంశం, BLKSIZE);
		కొరకు (; i < BLKSIZE; i++)
			llist_అంశం_put(&new_అంశం[i]);
	}
	తిరిగి new_అంశం;
}

స్థిర అంతఃవరుస శూన్య llist_init(నిర్మాణం llist **list)
{
	*list = xmalloc(పరిమాణం(నిర్మాణం llist));
	(*list)->front = (*list)->వెనుక = ఖాళీ;
	(*list)->కొలత = 0;
}

స్థిర శూన్య llist_free(నిర్మాణం llist *list)
{
	కొరకు (నిర్మాణం llist_అంశం *i = list->front, *తదుపరి; i; i = తదుపరి) {
		తదుపరి = i->తదుపరి;
		llist_అంశం_put(i);
	}
	విడుదల(list);
}

స్థిర నిర్మాణం llist * llist_copy(నిర్మాణం llist *list)
{
	నిర్మాణం llist *ret;
	నిర్మాణం llist_అంశం *new_అంశం, *old_అంశం, *prev;

	llist_init(&ret);

	ఒకవేళ ((ret->కొలత = list->కొలత) == 0)
		తిరిగి ret;

	new_అంశం = ret->front = llist_అంశం_get();
	new_అంశం->oid = list->front->oid;

	old_అంశం = list->front->తదుపరి;
	అయితే (old_అంశం) {
		prev = new_అంశం;
		new_అంశం = llist_అంశం_get();
		prev->తదుపరి = new_అంశం;
		new_అంశం->oid = old_అంశం->oid;
		old_అంశం = old_అంశం->తదుపరి;
	}
	new_అంశం->తదుపరి = ఖాళీ;
	ret->వెనుక = new_అంశం;

	తిరిగి ret;
}

స్థిర అంతఃవరుస నిర్మాణం llist_అంశం *llist_చొప్పించు(నిర్మాణం llist *list,
					      నిర్మాణం llist_అంశం *after,
					      స్థిరరాశి గుర్తులేని అక్షరం *oid)
{
	నిర్మాణం llist_అంశం *new_అంశం = llist_అంశం_get();
	oidread(&new_అంశం->oid, oid, the_repository->hash_algo);
	new_అంశం->తదుపరి = ఖాళీ;

	ఒకవేళ (after) {
		new_అంశం->తదుపరి = after->తదుపరి;
		after->తదుపరి = new_అంశం;
		ఒకవేళ (after == list->వెనుక)
			list->వెనుక = new_అంశం;
	} లేకపోతే {/* insert in front */
		ఒకవేళ (list->కొలత == 0)
			list->వెనుక = new_అంశం;
		లేకపోతే
			new_అంశం->తదుపరి = list->front;
		list->front = new_అంశం;
	}
	list->కొలత++;
	తిరిగి new_అంశం;
}

స్థిర అంతఃవరుస నిర్మాణం llist_అంశం *llist_చొప్పించు_వెనుక(నిర్మాణం llist *list,
						   స్థిరరాశి గుర్తులేని అక్షరం *oid)
{
	తిరిగి llist_చొప్పించు(list, list->వెనుక, oid);
}

స్థిర అంతఃవరుస నిర్మాణం llist_అంశం *llist_చొప్పించు_sorted_unique(నిర్మాణం llist *list,
			స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం llist_అంశం *hint)
{
	నిర్మాణం llist_అంశం *prev = ఖాళీ, *l;

	l = (hint == ఖాళీ) ? list->front : hint;
	అయితే (l) {
		పూర్ణాంకం cmp = oidcmp(&l->oid, oid);
		ఒకవేళ (cmp > 0) { /* we insert before this entry */
			తిరిగి llist_చొప్పించు(list, prev, oid->hash);
		}
		ఒకవేళ (!cmp) { /* already exists */
			తిరిగి l;
		}
		prev = l;
		l = l->తదుపరి;
	}
	/* insert at the end */
	తిరిగి llist_చొప్పించు_వెనుక(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
స్థిర అంతఃవరుస నిర్మాణం llist_అంశం * llist_sorted_remove(నిర్మాణం llist *list, స్థిరరాశి గుర్తులేని అక్షరం *oid, నిర్మాణం llist_అంశం *hint)
{
	నిర్మాణం llist_అంశం *prev, *l;

redo_from_ప్రారంభం:
	l = (hint == ఖాళీ) ? list->front : hint;
	prev = ఖాళీ;
	అయితే (l) {
		స్థిరరాశి పూర్ణాంకం cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		ఒకవేళ (cmp > 0) /* not in list, since sorted */
			తిరిగి prev;
		ఒకవేళ (!cmp) { /*దొరికింది*/
			ఒకవేళ (!prev) {
				ఒకవేళ (hint != ఖాళీ && hint != list->front) {
					/* we don't know the previous element */
					hint = ఖాళీ;
					వెళ్ళు redo_from_ప్రారంభం;
				}
				list->front = l->తదుపరి;
			} లేకపోతే
				prev->తదుపరి = l->తదుపరి;
			ఒకవేళ (l == list->వెనుక)
				list->వెనుక = prev;
			llist_అంశం_put(l);
			list->కొలత--;
			తిరిగి prev;
		}
		prev = l;
		l = l->తదుపరి;
	}
	తిరిగి prev;
}

/* computes A\B */
స్థిర శూన్య llist_sorted_difference_inplace(నిర్మాణం llist *A,
				     నిర్మాణం llist *B)
{
	నిర్మాణం llist_అంశం *hint, *b;

	hint = ఖాళీ;
	b = B->front;

	అయితే (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->తదుపరి;
	}
}

స్థిర అంతఃవరుస నిర్మాణం pack_list * pack_list_చొప్పించు(నిర్మాణం pack_list **pl,
					   నిర్మాణం pack_list *నమోదు_2)
{
	నిర్మాణం pack_list *p = xmalloc(పరిమాణం(నిర్మాణం pack_list));
	జ్ఞాపకం_నకలు(p, నమోదు_2, పరిమాణం(నిర్మాణం pack_list));
	p->తదుపరి = *pl;
	*pl = p;
	తిరిగి p;
}

స్థిర శూన్య pack_list_free(నిర్మాణం pack_list *pl)
{
	కొరకు (నిర్మాణం pack_list *తదుపరి; pl; pl = తదుపరి) {
		తదుపరి = pl->తదుపరి;
		విడుదల(pl);
	}
}

స్థిర అంతఃవరుస కొలత_t pack_list_కొలత(నిర్మాణం pack_list *pl)
{
	కొలత_t ret = 0;
	అయితే (pl) {
		ret++;
		pl = pl->తదుపరి;
	}
	తిరిగి ret;
}

స్థిర నిర్మాణం pack_list * pack_list_difference(స్థిరరాశి నిర్మాణం pack_list *A,
					       స్థిరరాశి నిర్మాణం pack_list *B)
{
	నిర్మాణం pack_list *ret;
	స్థిరరాశి నిర్మాణం pack_list *pl;

	ఒకవేళ (!A)
		తిరిగి ఖాళీ;

	pl = B;
	అయితే (pl != ఖాళీ) {
		ఒకవేళ (A->pack == pl->pack)
			తిరిగి pack_list_difference(A->తదుపరి, B);
		pl = pl->తదుపరి;
	}
	ret = xmalloc(పరిమాణం(నిర్మాణం pack_list));
	జ్ఞాపకం_నకలు(ret, A, పరిమాణం(నిర్మాణం pack_list));
	ret->తదుపరి = pack_list_difference(A->తదుపరి, B);
	తిరిగి ret;
}

స్థిర శూన్య cmp_two_packs(నిర్మాణం pack_list *p1, నిర్మాణం pack_list *p2)
{
	కొలత_t p1_off = 0, p2_off = 0, p1_అడుగు, p2_అడుగు;
	స్థిరరాశి గుర్తులేని అక్షరం *p1_పునాది, *p2_పునాది;
	నిర్మాణం llist_అంశం *p1_hint = ఖాళీ, *p2_hint = ఖాళీ;
	స్థిరరాశి గుర్తులేని పూర్ణాంకం hashsz = the_hash_algo->rawsz;

	ఒకవేళ (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->మిగిలిన_objects);
	ఒకవేళ (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->మిగిలిన_objects);

	p1_పునాది = p1->pack->సూచీ_దత్తాంశం;
	p2_పునాది = p2->pack->సూచీ_దత్తాంశం;
	p1_పునాది += 256 * 4 + ((p1->pack->సూచీ_సంస్కరణ < 2) ? 4 : 8);
	p2_పునాది += 256 * 4 + ((p2->pack->సూచీ_సంస్కరణ < 2) ? 4 : 8);
	p1_అడుగు = hashsz + ((p1->pack->సూచీ_సంస్కరణ < 2) ? 4 : 0);
	p2_అడుగు = hashsz + ((p2->pack->సూచీ_సంస్కరణ < 2) ? 4 : 0);

	అయితే (p1_off < p1->pack->అంకె_objects * p1_అడుగు &&
	       p2_off < p2->pack->అంకె_objects * p2_అడుగు)
	{
		స్థిరరాశి పూర్ణాంకం cmp = hashcmp(p1_పునాది + p1_off, p2_పునాది + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ఒకవేళ (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_పునాది + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_పునాది + p1_off,
					p2_hint);
			p1_off += p1_అడుగు;
			p2_off += p2_అడుగు;
			కొనసాగు;
		}
		ఒకవేళ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_అడుగు;
		} లేకపోతే { /* p2 has the object, p1 doesn't */
			p2_off += p2_అడుగు;
		}
	}
}

స్థిర కొలత_t sizeof_union(నిర్మాణం packed_git *p1, నిర్మాణం packed_git *p2)
{
	కొలత_t ret = 0;
	కొలత_t p1_off = 0, p2_off = 0, p1_అడుగు, p2_అడుగు;
	స్థిరరాశి గుర్తులేని అక్షరం *p1_పునాది, *p2_పునాది;
	స్థిరరాశి గుర్తులేని పూర్ణాంకం hashsz = the_hash_algo->rawsz;

	p1_పునాది = p1->సూచీ_దత్తాంశం;
	p2_పునాది = p2->సూచీ_దత్తాంశం;
	p1_పునాది += 256 * 4 + ((p1->సూచీ_సంస్కరణ < 2) ? 4 : 8);
	p2_పునాది += 256 * 4 + ((p2->సూచీ_సంస్కరణ < 2) ? 4 : 8);
	p1_అడుగు = hashsz + ((p1->సూచీ_సంస్కరణ < 2) ? 4 : 0);
	p2_అడుగు = hashsz + ((p2->సూచీ_సంస్కరణ < 2) ? 4 : 0);

	అయితే (p1_off < p1->అంకె_objects * p1_అడుగు &&
	       p2_off < p2->అంకె_objects * p2_అడుగు)
	{
		పూర్ణాంకం cmp = hashcmp(p1_పునాది + p1_off, p2_పునాది + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		ఒకవేళ (cmp == 0) {
			ret++;
			p1_off += p1_అడుగు;
			p2_off += p2_అడుగు;
			కొనసాగు;
		}
		ఒకవేళ (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_అడుగు;
		} లేకపోతే { /* p2 has the object, p1 doesn't */
			p2_off += p2_అడుగు;
		}
	}
	తిరిగి ret;
}

/* another O(n^2) function ... */
స్థిర కొలత_t get_pack_redundancy(నిర్మాణం pack_list *pl)
{
	నిర్మాణం pack_list *subset;
	కొలత_t ret = 0;

	ఒకవేళ (!pl)
		తిరిగి 0;

	అయితే ((subset = pl->తదుపరి)) {
		అయితే (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->తదుపరి;
		}
		pl = pl->తదుపరి;
	}
	తిరిగి ret;
}

స్థిర అంతఃవరుస off_t pack_సమితి_bytecount(నిర్మాణం pack_list *pl)
{
	off_t ret = 0;
	అయితే (pl) {
		ret += pl->pack->pack_కొలత;
		ret += pl->pack->సూచీ_కొలత;
		pl = pl->తదుపరి;
	}
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmp_మిగిలిన_objects(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
	నిర్మాణం pack_list *pl_a = *((నిర్మాణం pack_list **)a);
	నిర్మాణం pack_list *pl_b = *((నిర్మాణం pack_list **)b);

	ఒకవేళ (pl_a->మిగిలిన_objects->కొలత == pl_b->మిగిలిన_objects->కొలత) {
		/* have the same remaining_objects, big pack first */
		ఒకవేళ (pl_a->all_objects_కొలత == pl_b->all_objects_కొలత)
			తిరిగి 0;
		లేకపోతే ఒకవేళ (pl_a->all_objects_కొలత < pl_b->all_objects_కొలత)
			తిరిగి 1;
		లేకపోతే
			తిరిగి -1;
	} లేకపోతే ఒకవేళ (pl_a->మిగిలిన_objects->కొలత < pl_b->మిగిలిన_objects->కొలత) {
		/* sort by remaining objects, more objects first */
		తిరిగి 1;
	} లేకపోతే {
		తిరిగి -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
స్థిర శూన్య క్రమీకరించు_pack_list(నిర్మాణం pack_list **pl)
{
	నిర్మాణం pack_list **ary, *p;
	కొలత_t n = pack_list_కొలత(*pl);

	ఒకవేళ (n < 2)
		తిరిగి;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	కొరకు (n = 0, p = *pl; p; p = p->తదుపరి)
		ary[n++] = p;

	QSORT(ary, n, cmp_మిగిలిన_objects);

	/* link them back again */
	కొరకు (కొలత_t i = 0; i < n - 1; i++)
		ary[i]->తదుపరి = ary[i + 1];
	ary[n - 1]->తదుపరి = ఖాళీ;
	*pl = ary[0];

	విడుదల(ary);
}


స్థిర శూన్య minimize(నిర్మాణం pack_list **min)
{
	నిర్మాణం pack_list *pl, *unique = ఖాళీ, *non_unique = ఖాళీ;
	నిర్మాణం llist *missing, *unique_pack_objects;

	pl = local_packs;
	అయితే (pl) {
		ఒకవేళ (pl->unique_objects->కొలత)
			pack_list_చొప్పించు(&unique, pl);
		లేకపోతే
			pack_list_చొప్పించు(&non_unique, pl);
		pl = pl->తదుపరి;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	అయితే (pl) {
		llist_sorted_difference_inplace(missing, pl->మిగిలిన_objects);
		pl = pl->తదుపరి;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	ఒకవేళ (missing->కొలత == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		తిరిగి;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	అయితే (pl) {
		llist_sorted_difference_inplace(pl->మిగిలిన_objects, unique_pack_objects);
		pl = pl->తదుపరి;
	}

	అయితే (non_unique) {
		నిర్మాణం pack_list *తదుపరి;

		/* sort the non_unique packs, greater size of remaining_objects first */
		క్రమీకరించు_pack_list(&non_unique);
		ఒకవేళ (non_unique->మిగిలిన_objects->కొలత == 0)
			ఆపు;

		pack_list_చొప్పించు(min, non_unique);

		కొరకు (pl = non_unique->తదుపరి; pl && pl->మిగిలిన_objects->కొలత > 0;  pl = pl->తదుపరి)
			llist_sorted_difference_inplace(pl->మిగిలిన_objects, non_unique->మిగిలిన_objects);

		తదుపరి = non_unique->తదుపరి;
		విడుదల(non_unique);
		non_unique = తదుపరి;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

స్థిర శూన్య ఎక్కించు_all_objects(శూన్య)
{
	నిర్మాణం pack_list *pl = local_packs;
	నిర్మాణం llist_అంశం *hint, *l;

	llist_init(&all_objects);

	అయితే (pl) {
		hint = ఖాళీ;
		l = pl->మిగిలిన_objects->front;
		అయితే (l) {
			hint = llist_చొప్పించు_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->తదుపరి;
		}
		pl = pl->తదుపరి;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	అయితే (pl) {
		llist_sorted_difference_inplace(all_objects, pl->మిగిలిన_objects);
		pl = pl->తదుపరి;
	}
}

/* this scales like O(n^2) */
స్థిర శూన్య cmp_local_packs(శూన్య)
{
	నిర్మాణం pack_list *subset, *pl = local_packs;

	/* only one packfile */
	ఒకవేళ (!pl->తదుపరి) {
		llist_init(&pl->unique_objects);
		తిరిగి;
	}

	అయితే ((subset = pl)) {
		అయితే ((subset = subset->తదుపరి))
			cmp_two_packs(pl, subset);
		pl = pl->తదుపరి;
	}
}

స్థిర శూన్య పరిశీలన_alt_odb_packs(శూన్య)
{
	నిర్మాణం pack_list *local, *alt;

	alt = altodb_packs;
	అయితే (alt) {
		local = local_packs;
		అయితే (local) {
			llist_sorted_difference_inplace(local->మిగిలిన_objects,
							alt->మిగిలిన_objects);
			local = local->తదుపరి;
		}
		alt = alt->తదుపరి;
	}
}

స్థిర నిర్మాణం pack_list * కూడు_pack(నిర్మాణం packed_git *p)
{
	నిర్మాణం pack_list l;
	కొలత_t off = 0, అడుగు;
	స్థిరరాశి గుర్తులేని అక్షరం *పునాది;

	ఒకవేళ (!p->pack_local && !(alt_odb || verbose))
		తిరిగి ఖాళీ;

	l.pack = p;
	llist_init(&l.మిగిలిన_objects);

	ఒకవేళ (open_pack_సూచీ(p)) {
		llist_free(l.మిగిలిన_objects);
		తిరిగి ఖాళీ;
	}

	పునాది = p->సూచీ_దత్తాంశం;
	పునాది += 256 * 4 + ((p->సూచీ_సంస్కరణ < 2) ? 4 : 8);
	అడుగు = the_hash_algo->rawsz + ((p->సూచీ_సంస్కరణ < 2) ? 4 : 0);
	అయితే (off < p->అంకె_objects * అడుగు) {
		llist_చొప్పించు_వెనుక(l.మిగిలిన_objects, పునాది + off);
		off += అడుగు;
	}
	l.all_objects_కొలత = l.మిగిలిన_objects->కొలత;
	l.unique_objects = ఖాళీ;
	ఒకవేళ (p->pack_local)
		తిరిగి pack_list_చొప్పించు(&local_packs, &l);
	లేకపోతే
		తిరిగి pack_list_చొప్పించు(&altodb_packs, &l);
}

స్థిర నిర్మాణం pack_list * కూడు_pack_file(స్థిరరాశి అక్షరం *filename)
{
	నిర్మాణం packed_git *p;

	ఒకవేళ (అక్షరశ్రేణి_పొడవు(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		ఒకవేళ (అక్షరశ్రేణి_శోధన(p->pack_name, filename))
			తిరిగి కూడు_pack(p);
	die("Filename %s not found in packed_git", filename);
}

స్థిర శూన్య ఎక్కించు_all(శూన్య)
{
	నిర్మాణం packed_git *p;

	repo_for_each_pack(the_repository, p)
		కూడు_pack(p);
}

పూర్ణాంకం cmd_pack_redundant(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ వాడని, నిర్మాణం repository *repo వాడని) {
	పూర్ణాంకం i; పూర్ణాంకం i_still_use_this = 0; నిర్మాణం pack_list *min = ఖాళీ, *red, *pl;
	నిర్మాణం llist *ignore;
	నిర్మాణం strbuf idx_name = STRBUF_INIT;
	అక్షరం buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	కొరకు (i = 1; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];
		ఒకవేళ (!strcmp(arg, "--")) {
			i++;
			ఆపు;
		}
		ఒకవేళ (!strcmp(arg, "--all")) {
			ఎక్కించు_all_packs = 1;
			కొనసాగు;
		}
		ఒకవేళ (!strcmp(arg, "--verbose")) {
			verbose = 1;
			కొనసాగు;
		}
		ఒకవేళ (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			కొనసాగు;
		}
		ఒకవేళ (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			కొనసాగు;
		}
		ఒకవేళ (*arg == '-')
			usage(pack_redundant_usage);
		లేకపోతే
			ఆపు;
	}

	ఒకవేళ (!i_still_use_this)
		you_still_use_that("git pack-redundant", ఖాళీ);

	ఒకవేళ (ఎక్కించు_all_packs)
		ఎక్కించు_all();
	లేకపోతే
		అయితే (*(argv + i) != ఖాళీ)
			కూడు_pack_file(*(argv + i++));

	ఒకవేళ (!local_packs)
		die("Zero packs found!");

	ఎక్కించు_all_objects();

	ఒకవేళ (alt_odb)
		పరిశీలన_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	ఒకవేళ (!isatty(0)) {
		నిర్మాణం object_id oid;
		అయితే (fgets(buf, పరిమాణం(buf), stdin)) {
			ఒకవేళ (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_చొప్పించు_sorted_unique(ignore, &oid, ఖాళీ);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	అయితే (pl) {
		llist_sorted_difference_inplace(pl->మిగిలిన_objects, ignore);
		pl = pl->తదుపరి;
	}

	cmp_local_packs();

	minimize(&min);

	ఒకవేళ (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(గుర్తులేని దీర్ఘ)pack_list_కొలత(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		అయితే (pl) {
			fprintf(stderr, "\t%s\n", pl->pack->pack_name);
			pl = pl->తదుపరి;
		}
		fprintf(stderr, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(గుర్తులేని దీర్ఘ)get_pack_redundancy(min),
			(గుర్తులేని దీర్ఘ)pack_సమితి_bytecount(min)/1024);
		fprintf(stderr, "A total of %lu unique objects were considered.\n",
			(గుర్తులేని దీర్ఘ)all_objects->కొలత);
		fprintf(stderr, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	అయితే (pl) {
		రాయి("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->తదుపరి;
	}
	ఒకవేళ (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(గుర్తులేని దీర్ఘ)pack_సమితి_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	తిరిగి 0;
}
