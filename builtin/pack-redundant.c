/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hex.h"

#ضمن "packfile.h"
#ضمن "odb.h"
#ضمن "strbuf.h"

#وصف_ڪريو BLKSIZE 512

ساكن ثابت حرف pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

ساكن صحيح ڀريو_all_packs, verbose, alt_odb;

بنية llist_شي {
	بنية llist_شي *ايندڙ;
	بنية object_id oid;
};
ساكن بنية llist {
	بنية llist_شي *front;
	بنية llist_شي *پٺ;
	سائيز_t سائيز;
} *all_objects; /* all objects which must be present in local packfiles */

ساكن بنية pack_list {
	بنية pack_list *ايندڙ;
	بنية packed_git *pack;
	بنية llist *unique_objects;
	بنية llist *بچيل_objects;
	سائيز_t all_objects_سائيز;
} *local_packs = عدم, *altodb_packs = عدم;

ساكن بنية llist_شي *free_nodes;

ساكن مضمن فراغ llist_شي_put(بنية llist_شي *شي)
{
	شي->ايندڙ = free_nodes;
	free_nodes = شي;
}

ساكن مضمن بنية llist_شي *llist_شي_get(فراغ)
{
	بنية llist_شي *new_شي;
	إذا ( free_nodes ) {
		new_شي = free_nodes;
		free_nodes = free_nodes->ايندڙ;
	} وإلا {
		صحيح i = 1;
		ALLOC_ARRAY(new_شي, BLKSIZE);
		لكل (; i < BLKSIZE; i++)
			llist_شي_put(&new_شي[i]);
	}
	أرجع new_شي;
}

ساكن مضمن فراغ llist_init(بنية llist **list)
{
	*list = xmalloc(حجم(بنية llist));
	(*list)->front = (*list)->پٺ = عدم;
	(*list)->سائيز = 0;
}

ساكن فراغ llist_free(بنية llist *list)
{
	لكل (بنية llist_شي *i = list->front, *ايندڙ; i; i = ايندڙ) {
		ايندڙ = i->ايندڙ;
		llist_شي_put(i);
	}
	حرر(list);
}

ساكن بنية llist * llist_copy(بنية llist *list)
{
	بنية llist *ret;
	بنية llist_شي *new_شي, *old_شي, *prev;

	llist_init(&ret);

	إذا ((ret->سائيز = list->سائيز) == 0)
		أرجع ret;

	new_شي = ret->front = llist_شي_get();
	new_شي->oid = list->front->oid;

	old_شي = list->front->ايندڙ;
	طالما (old_شي) {
		prev = new_شي;
		new_شي = llist_شي_get();
		prev->ايندڙ = new_شي;
		new_شي->oid = old_شي->oid;
		old_شي = old_شي->ايندڙ;
	}
	new_شي->ايندڙ = عدم;
	ret->پٺ = new_شي;

	أرجع ret;
}

ساكن مضمن بنية llist_شي *llist_وجهو(بنية llist *list,
					      بنية llist_شي *after,
					      ثابت بنانشان حرف *oid)
{
	بنية llist_شي *new_شي = llist_شي_get();
	oidread(&new_شي->oid, oid, the_repository->hash_algo);
	new_شي->ايندڙ = عدم;

	إذا (after) {
		new_شي->ايندڙ = after->ايندڙ;
		after->ايندڙ = new_شي;
		إذا (after == list->پٺ)
			list->پٺ = new_شي;
	} وإلا {/* insert in front */
		إذا (list->سائيز == 0)
			list->پٺ = new_شي;
		وإلا
			new_شي->ايندڙ = list->front;
		list->front = new_شي;
	}
	list->سائيز++;
	أرجع new_شي;
}

ساكن مضمن بنية llist_شي *llist_وجهو_پٺ(بنية llist *list,
						   ثابت بنانشان حرف *oid)
{
	أرجع llist_وجهو(list, list->پٺ, oid);
}

ساكن مضمن بنية llist_شي *llist_وجهو_sorted_unique(بنية llist *list,
			ثابت بنية object_id *oid, بنية llist_شي *hint)
{
	بنية llist_شي *prev = عدم, *l;

	l = (hint == عدم) ? list->front : hint;
	طالما (l) {
		صحيح cmp = oidcmp(&l->oid, oid);
		إذا (cmp > 0) { /* we insert before this entry */
			أرجع llist_وجهو(list, prev, oid->hash);
		}
		إذا (!cmp) { /* already exists */
			أرجع l;
		}
		prev = l;
		l = l->ايندڙ;
	}
	/* insert at the end */
	أرجع llist_وجهو_پٺ(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
ساكن مضمن بنية llist_شي * llist_sorted_remove(بنية llist *list, ثابت بنانشان حرف *oid, بنية llist_شي *hint)
{
	بنية llist_شي *prev, *l;

redo_from_شروعات:
	l = (hint == عدم) ? list->front : hint;
	prev = عدم;
	طالما (l) {
		ثابت صحيح cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		إذا (cmp > 0) /* not in list, since sorted */
			أرجع prev;
		إذا (!cmp) { /*مليو*/
			إذا (!prev) {
				إذا (hint != عدم && hint != list->front) {
					/* we don't know the previous element */
					hint = عدم;
					اذهب redo_from_شروعات;
				}
				list->front = l->ايندڙ;
			} وإلا
				prev->ايندڙ = l->ايندڙ;
			إذا (l == list->پٺ)
				list->پٺ = prev;
			llist_شي_put(l);
			list->سائيز--;
			أرجع prev;
		}
		prev = l;
		l = l->ايندڙ;
	}
	أرجع prev;
}

/* computes A\B */
ساكن فراغ llist_sorted_difference_inplace(بنية llist *A,
				     بنية llist *B)
{
	بنية llist_شي *hint, *b;

	hint = عدم;
	b = B->front;

	طالما (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->ايندڙ;
	}
}

ساكن مضمن بنية pack_list * pack_list_وجهو(بنية pack_list **pl,
					   بنية pack_list *داخلا)
{
	بنية pack_list *p = xmalloc(حجم(بنية pack_list));
	يادگيري_نقل(p, داخلا, حجم(بنية pack_list));
	p->ايندڙ = *pl;
	*pl = p;
	أرجع p;
}

ساكن فراغ pack_list_free(بنية pack_list *pl)
{
	لكل (بنية pack_list *ايندڙ; pl; pl = ايندڙ) {
		ايندڙ = pl->ايندڙ;
		حرر(pl);
	}
}

ساكن مضمن سائيز_t pack_list_سائيز(بنية pack_list *pl)
{
	سائيز_t ret = 0;
	طالما (pl) {
		ret++;
		pl = pl->ايندڙ;
	}
	أرجع ret;
}

ساكن بنية pack_list * pack_list_difference(ثابت بنية pack_list *A,
					       ثابت بنية pack_list *B)
{
	بنية pack_list *ret;
	ثابت بنية pack_list *pl;

	إذا (!A)
		أرجع عدم;

	pl = B;
	طالما (pl != عدم) {
		إذا (A->pack == pl->pack)
			أرجع pack_list_difference(A->ايندڙ, B);
		pl = pl->ايندڙ;
	}
	ret = xmalloc(حجم(بنية pack_list));
	يادگيري_نقل(ret, A, حجم(بنية pack_list));
	ret->ايندڙ = pack_list_difference(A->ايندڙ, B);
	أرجع ret;
}

ساكن فراغ cmp_two_packs(بنية pack_list *p1, بنية pack_list *p2)
{
	سائيز_t p1_off = 0, p2_off = 0, p1_قدم, p2_قدم;
	ثابت بنانشان حرف *p1_بنياد, *p2_بنياد;
	بنية llist_شي *p1_hint = عدم, *p2_hint = عدم;
	ثابت بنانشان صحيح hashsz = the_hash_algo->rawsz;

	إذا (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->بچيل_objects);
	إذا (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->بچيل_objects);

	p1_بنياد = p1->pack->اشاريو_معلومات;
	p2_بنياد = p2->pack->اشاريو_معلومات;
	p1_بنياد += 256 * 4 + ((p1->pack->اشاريو_نسخو < 2) ? 4 : 8);
	p2_بنياد += 256 * 4 + ((p2->pack->اشاريو_نسخو < 2) ? 4 : 8);
	p1_قدم = hashsz + ((p1->pack->اشاريو_نسخو < 2) ? 4 : 0);
	p2_قدم = hashsz + ((p2->pack->اشاريو_نسخو < 2) ? 4 : 0);

	طالما (p1_off < p1->pack->انگ_objects * p1_قدم &&
	       p2_off < p2->pack->انگ_objects * p2_قدم)
	{
		ثابت صحيح cmp = hashcmp(p1_بنياد + p1_off, p2_بنياد + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		إذا (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_بنياد + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_بنياد + p1_off,
					p2_hint);
			p1_off += p1_قدم;
			p2_off += p2_قدم;
			استمر;
		}
		إذا (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_قدم;
		} وإلا { /* p2 has the object, p1 doesn't */
			p2_off += p2_قدم;
		}
	}
}

ساكن سائيز_t sizeof_union(بنية packed_git *p1, بنية packed_git *p2)
{
	سائيز_t ret = 0;
	سائيز_t p1_off = 0, p2_off = 0, p1_قدم, p2_قدم;
	ثابت بنانشان حرف *p1_بنياد, *p2_بنياد;
	ثابت بنانشان صحيح hashsz = the_hash_algo->rawsz;

	p1_بنياد = p1->اشاريو_معلومات;
	p2_بنياد = p2->اشاريو_معلومات;
	p1_بنياد += 256 * 4 + ((p1->اشاريو_نسخو < 2) ? 4 : 8);
	p2_بنياد += 256 * 4 + ((p2->اشاريو_نسخو < 2) ? 4 : 8);
	p1_قدم = hashsz + ((p1->اشاريو_نسخو < 2) ? 4 : 0);
	p2_قدم = hashsz + ((p2->اشاريو_نسخو < 2) ? 4 : 0);

	طالما (p1_off < p1->انگ_objects * p1_قدم &&
	       p2_off < p2->انگ_objects * p2_قدم)
	{
		صحيح cmp = hashcmp(p1_بنياد + p1_off, p2_بنياد + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		إذا (cmp == 0) {
			ret++;
			p1_off += p1_قدم;
			p2_off += p2_قدم;
			استمر;
		}
		إذا (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_قدم;
		} وإلا { /* p2 has the object, p1 doesn't */
			p2_off += p2_قدم;
		}
	}
	أرجع ret;
}

/* another O(n^2) function ... */
ساكن سائيز_t get_pack_redundancy(بنية pack_list *pl)
{
	بنية pack_list *subset;
	سائيز_t ret = 0;

	إذا (!pl)
		أرجع 0;

	طالما ((subset = pl->ايندڙ)) {
		طالما (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->ايندڙ;
		}
		pl = pl->ايندڙ;
	}
	أرجع ret;
}

ساكن مضمن off_t pack_سيٽ_bytecount(بنية pack_list *pl)
{
	off_t ret = 0;
	طالما (pl) {
		ret += pl->pack->pack_سائيز;
		ret += pl->pack->اشاريو_سائيز;
		pl = pl->ايندڙ;
	}
	أرجع ret;
}

ساكن صحيح cmp_بچيل_objects(ثابت فراغ *a, ثابت فراغ *b)
{
	بنية pack_list *pl_a = *((بنية pack_list **)a);
	بنية pack_list *pl_b = *((بنية pack_list **)b);

	إذا (pl_a->بچيل_objects->سائيز == pl_b->بچيل_objects->سائيز) {
		/* have the same remaining_objects, big pack first */
		إذا (pl_a->all_objects_سائيز == pl_b->all_objects_سائيز)
			أرجع 0;
		وإلا إذا (pl_a->all_objects_سائيز < pl_b->all_objects_سائيز)
			أرجع 1;
		وإلا
			أرجع -1;
	} وإلا إذا (pl_a->بچيل_objects->سائيز < pl_b->بچيل_objects->سائيز) {
		/* sort by remaining objects, more objects first */
		أرجع 1;
	} وإلا {
		أرجع -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
ساكن فراغ ترتيب_ڏيو_pack_list(بنية pack_list **pl)
{
	بنية pack_list **ary, *p;
	سائيز_t n = pack_list_سائيز(*pl);

	إذا (n < 2)
		أرجع;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	لكل (n = 0, p = *pl; p; p = p->ايندڙ)
		ary[n++] = p;

	QSORT(ary, n, cmp_بچيل_objects);

	/* link them back again */
	لكل (سائيز_t i = 0; i < n - 1; i++)
		ary[i]->ايندڙ = ary[i + 1];
	ary[n - 1]->ايندڙ = عدم;
	*pl = ary[0];

	حرر(ary);
}


ساكن فراغ minimize(بنية pack_list **min)
{
	بنية pack_list *pl, *unique = عدم, *non_unique = عدم;
	بنية llist *missing, *unique_pack_objects;

	pl = local_packs;
	طالما (pl) {
		إذا (pl->unique_objects->سائيز)
			pack_list_وجهو(&unique, pl);
		وإلا
			pack_list_وجهو(&non_unique, pl);
		pl = pl->ايندڙ;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	طالما (pl) {
		llist_sorted_difference_inplace(missing, pl->بچيل_objects);
		pl = pl->ايندڙ;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	إذا (missing->سائيز == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		أرجع;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	طالما (pl) {
		llist_sorted_difference_inplace(pl->بچيل_objects, unique_pack_objects);
		pl = pl->ايندڙ;
	}

	طالما (non_unique) {
		بنية pack_list *ايندڙ;

		/* sort the non_unique packs, greater size of remaining_objects first */
		ترتيب_ڏيو_pack_list(&non_unique);
		إذا (non_unique->بچيل_objects->سائيز == 0)
			اكسر;

		pack_list_وجهو(min, non_unique);

		لكل (pl = non_unique->ايندڙ; pl && pl->بچيل_objects->سائيز > 0;  pl = pl->ايندڙ)
			llist_sorted_difference_inplace(pl->بچيل_objects, non_unique->بچيل_objects);

		ايندڙ = non_unique->ايندڙ;
		حرر(non_unique);
		non_unique = ايندڙ;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

ساكن فراغ ڀريو_all_objects(فراغ)
{
	بنية pack_list *pl = local_packs;
	بنية llist_شي *hint, *l;

	llist_init(&all_objects);

	طالما (pl) {
		hint = عدم;
		l = pl->بچيل_objects->front;
		طالما (l) {
			hint = llist_وجهو_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->ايندڙ;
		}
		pl = pl->ايندڙ;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	طالما (pl) {
		llist_sorted_difference_inplace(all_objects, pl->بچيل_objects);
		pl = pl->ايندڙ;
	}
}

/* this scales like O(n^2) */
ساكن فراغ cmp_local_packs(فراغ)
{
	بنية pack_list *subset, *pl = local_packs;

	/* only one packfile */
	إذا (!pl->ايندڙ) {
		llist_init(&pl->unique_objects);
		أرجع;
	}

	طالما ((subset = pl)) {
		طالما ((subset = subset->ايندڙ))
			cmp_two_packs(pl, subset);
		pl = pl->ايندڙ;
	}
}

ساكن فراغ جاچ_alt_odb_packs(فراغ)
{
	بنية pack_list *local, *alt;

	alt = altodb_packs;
	طالما (alt) {
		local = local_packs;
		طالما (local) {
			llist_sorted_difference_inplace(local->بچيل_objects,
							alt->بچيل_objects);
			local = local->ايندڙ;
		}
		alt = alt->ايندڙ;
	}
}

ساكن بنية pack_list * جوڙيو_pack(بنية packed_git *p)
{
	بنية pack_list l;
	سائيز_t off = 0, قدم;
	ثابت بنانشان حرف *بنياد;

	إذا (!p->pack_local && !(alt_odb || verbose))
		أرجع عدم;

	l.pack = p;
	llist_init(&l.بچيل_objects);

	إذا (open_pack_اشاريو(p)) {
		llist_free(l.بچيل_objects);
		أرجع عدم;
	}

	بنياد = p->اشاريو_معلومات;
	بنياد += 256 * 4 + ((p->اشاريو_نسخو < 2) ? 4 : 8);
	قدم = the_hash_algo->rawsz + ((p->اشاريو_نسخو < 2) ? 4 : 0);
	طالما (off < p->انگ_objects * قدم) {
		llist_وجهو_پٺ(l.بچيل_objects, بنياد + off);
		off += قدم;
	}
	l.all_objects_سائيز = l.بچيل_objects->سائيز;
	l.unique_objects = عدم;
	إذا (p->pack_local)
		أرجع pack_list_وجهو(&local_packs, &l);
	وإلا
		أرجع pack_list_وجهو(&altodb_packs, &l);
}

ساكن بنية pack_list * جوڙيو_pack_file(ثابت حرف *filename)
{
	بنية packed_git *p;

	إذا (لڙي_ڊيگهه(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		إذا (لڙي_ڳولا(p->pack_name, filename))
			أرجع جوڙيو_pack(p);
	die("Filename %s not found in packed_git", filename);
}

ساكن فراغ ڀريو_all(فراغ)
{
	بنية packed_git *p;

	repo_for_each_pack(the_repository, p)
		جوڙيو_pack(p);
}

صحيح cmd_pack_redundant(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل, بنية repository *repo اڻ_استعمال_ٿيل) {
	صحيح i; صحيح i_still_use_this = 0; بنية pack_list *min = عدم, *red, *pl;
	بنية llist *ignore;
	بنية strbuf idx_name = STRBUF_INIT;
	حرف buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	لكل (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		إذا (!strcmp(arg, "--")) {
			i++;
			اكسر;
		}
		إذا (!strcmp(arg, "--all")) {
			ڀريو_all_packs = 1;
			استمر;
		}
		إذا (!strcmp(arg, "--verbose")) {
			verbose = 1;
			استمر;
		}
		إذا (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			استمر;
		}
		إذا (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			استمر;
		}
		إذا (*arg == '-')
			usage(pack_redundant_usage);
		وإلا
			اكسر;
	}

	إذا (!i_still_use_this)
		you_still_use_that("git pack-redundant", عدم);

	إذا (ڀريو_all_packs)
		ڀريو_all();
	وإلا
		طالما (*(argv + i) != عدم)
			جوڙيو_pack_file(*(argv + i++));

	إذا (!local_packs)
		die("Zero packs found!");

	ڀريو_all_objects();

	إذا (alt_odb)
		جاچ_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	إذا (!isatty(0)) {
		بنية object_id oid;
		طالما (fgets(buf, حجم(buf), stdin)) {
			إذا (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_وجهو_sorted_unique(ignore, &oid, عدم);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	طالما (pl) {
		llist_sorted_difference_inplace(pl->بچيل_objects, ignore);
		pl = pl->ايندڙ;
	}

	cmp_local_packs();

	minimize(&min);

	إذا (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(بنانشان ڊگهو)pack_list_سائيز(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		طالما (pl) {
			fprintf(stderr, "\t%s\n", pl->pack->pack_name);
			pl = pl->ايندڙ;
		}
		fprintf(stderr, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(بنانشان ڊگهو)get_pack_redundancy(min),
			(بنانشان ڊگهو)pack_سيٽ_bytecount(min)/1024);
		fprintf(stderr, "A total of %lu unique objects were considered.\n",
			(بنانشان ڊگهو)all_objects->سائيز);
		fprintf(stderr, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	طالما (pl) {
		لکو("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->ايندڙ;
	}
	إذا (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(بنانشان ڊگهو)pack_سيٽ_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	أرجع 0;
}
