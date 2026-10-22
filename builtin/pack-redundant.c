/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#تعریف_کن USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hex.h"

#ضمن "packfile.h"
#ضمن "odb.h"
#ضمن "strbuf.h"

#تعریف_کن BLKSIZE 512

ساكن ثابت حرف pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

ساكن صحيح بارگیری_all_packs, verbose, alt_odb;

بنية llist_قلم {
	بنية llist_قلم *بعدی;
	بنية object_id oid;
};
ساكن بنية llist {
	بنية llist_قلم *front;
	بنية llist_قلم *پشت;
	اندازه_t اندازه;
} *all_objects; /* all objects which must be present in local packfiles */

ساكن بنية pack_list {
	بنية pack_list *بعدی;
	بنية packed_git *pack;
	بنية llist *unique_objects;
	بنية llist *مانده_objects;
	اندازه_t all_objects_اندازه;
} *local_packs = عدم, *altodb_packs = عدم;

ساكن بنية llist_قلم *free_nodes;

ساكن مضمن فراغ llist_قلم_put(بنية llist_قلم *قلم)
{
	قلم->بعدی = free_nodes;
	free_nodes = قلم;
}

ساكن مضمن بنية llist_قلم *llist_قلم_get(فراغ)
{
	بنية llist_قلم *new_قلم;
	إذا ( free_nodes ) {
		new_قلم = free_nodes;
		free_nodes = free_nodes->بعدی;
	} وإلا {
		صحيح i = 1;
		ALLOC_ARRAY(new_قلم, BLKSIZE);
		لكل (; i < BLKSIZE; i++)
			llist_قلم_put(&new_قلم[i]);
	}
	أرجع new_قلم;
}

ساكن مضمن فراغ llist_init(بنية llist **list)
{
	*list = xmalloc(حجم(بنية llist));
	(*list)->front = (*list)->پشت = عدم;
	(*list)->اندازه = 0;
}

ساكن فراغ llist_free(بنية llist *list)
{
	لكل (بنية llist_قلم *i = list->front, *بعدی; i; i = بعدی) {
		بعدی = i->بعدی;
		llist_قلم_put(i);
	}
	حرر(list);
}

ساكن بنية llist * llist_copy(بنية llist *list)
{
	بنية llist *ret;
	بنية llist_قلم *new_قلم, *old_قلم, *prev;

	llist_init(&ret);

	إذا ((ret->اندازه = list->اندازه) == 0)
		أرجع ret;

	new_قلم = ret->front = llist_قلم_get();
	new_قلم->oid = list->front->oid;

	old_قلم = list->front->بعدی;
	طالما (old_قلم) {
		prev = new_قلم;
		new_قلم = llist_قلم_get();
		prev->بعدی = new_قلم;
		new_قلم->oid = old_قلم->oid;
		old_قلم = old_قلم->بعدی;
	}
	new_قلم->بعدی = عدم;
	ret->پشت = new_قلم;

	أرجع ret;
}

ساكن مضمن بنية llist_قلم *llist_درج(بنية llist *list,
					      بنية llist_قلم *after,
					      ثابت بیعلامت حرف *oid)
{
	بنية llist_قلم *new_قلم = llist_قلم_get();
	oidread(&new_قلم->oid, oid, the_repository->hash_algo);
	new_قلم->بعدی = عدم;

	إذا (after) {
		new_قلم->بعدی = after->بعدی;
		after->بعدی = new_قلم;
		إذا (after == list->پشت)
			list->پشت = new_قلم;
	} وإلا {/* insert in front */
		إذا (list->اندازه == 0)
			list->پشت = new_قلم;
		وإلا
			new_قلم->بعدی = list->front;
		list->front = new_قلم;
	}
	list->اندازه++;
	أرجع new_قلم;
}

ساكن مضمن بنية llist_قلم *llist_درج_پشت(بنية llist *list,
						   ثابت بیعلامت حرف *oid)
{
	أرجع llist_درج(list, list->پشت, oid);
}

ساكن مضمن بنية llist_قلم *llist_درج_sorted_unique(بنية llist *list,
			ثابت بنية object_id *oid, بنية llist_قلم *hint)
{
	بنية llist_قلم *prev = عدم, *l;

	l = (hint == عدم) ? list->front : hint;
	طالما (l) {
		صحيح cmp = oidcmp(&l->oid, oid);
		إذا (cmp > 0) { /* we insert before this entry */
			أرجع llist_درج(list, prev, oid->hash);
		}
		إذا (!cmp) { /* already exists */
			أرجع l;
		}
		prev = l;
		l = l->بعدی;
	}
	/* insert at the end */
	أرجع llist_درج_پشت(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
ساكن مضمن بنية llist_قلم * llist_sorted_remove(بنية llist *list, ثابت بیعلامت حرف *oid, بنية llist_قلم *hint)
{
	بنية llist_قلم *prev, *l;

redo_from_شروع:
	l = (hint == عدم) ? list->front : hint;
	prev = عدم;
	طالما (l) {
		ثابت صحيح cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		إذا (cmp > 0) /* not in list, since sorted */
			أرجع prev;
		إذا (!cmp) { /*یافت*/
			إذا (!prev) {
				إذا (hint != عدم && hint != list->front) {
					/* we don't know the previous element */
					hint = عدم;
					اذهب redo_from_شروع;
				}
				list->front = l->بعدی;
			} وإلا
				prev->بعدی = l->بعدی;
			إذا (l == list->پشت)
				list->پشت = prev;
			llist_قلم_put(l);
			list->اندازه--;
			أرجع prev;
		}
		prev = l;
		l = l->بعدی;
	}
	أرجع prev;
}

/* computes A\B */
ساكن فراغ llist_sorted_difference_inplace(بنية llist *A,
				     بنية llist *B)
{
	بنية llist_قلم *hint, *b;

	hint = عدم;
	b = B->front;

	طالما (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->بعدی;
	}
}

ساكن مضمن بنية pack_list * pack_list_درج(بنية pack_list **pl,
					   بنية pack_list *مدخل)
{
	بنية pack_list *p = xmalloc(حجم(بنية pack_list));
	کپی_حافظه(p, مدخل, حجم(بنية pack_list));
	p->بعدی = *pl;
	*pl = p;
	أرجع p;
}

ساكن فراغ pack_list_free(بنية pack_list *pl)
{
	لكل (بنية pack_list *بعدی; pl; pl = بعدی) {
		بعدی = pl->بعدی;
		حرر(pl);
	}
}

ساكن مضمن اندازه_t pack_list_اندازه(بنية pack_list *pl)
{
	اندازه_t ret = 0;
	طالما (pl) {
		ret++;
		pl = pl->بعدی;
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
			أرجع pack_list_difference(A->بعدی, B);
		pl = pl->بعدی;
	}
	ret = xmalloc(حجم(بنية pack_list));
	کپی_حافظه(ret, A, حجم(بنية pack_list));
	ret->بعدی = pack_list_difference(A->بعدی, B);
	أرجع ret;
}

ساكن فراغ cmp_two_packs(بنية pack_list *p1, بنية pack_list *p2)
{
	اندازه_t p1_off = 0, p2_off = 0, p1_گام, p2_گام;
	ثابت بیعلامت حرف *p1_پایه, *p2_پایه;
	بنية llist_قلم *p1_hint = عدم, *p2_hint = عدم;
	ثابت بیعلامت صحيح hashsz = the_hash_algo->rawsz;

	إذا (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->مانده_objects);
	إذا (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->مانده_objects);

	p1_پایه = p1->pack->شاخص_داده;
	p2_پایه = p2->pack->شاخص_داده;
	p1_پایه += 256 * 4 + ((p1->pack->شاخص_نسخه < 2) ? 4 : 8);
	p2_پایه += 256 * 4 + ((p2->pack->شاخص_نسخه < 2) ? 4 : 8);
	p1_گام = hashsz + ((p1->pack->شاخص_نسخه < 2) ? 4 : 0);
	p2_گام = hashsz + ((p2->pack->شاخص_نسخه < 2) ? 4 : 0);

	طالما (p1_off < p1->pack->رقم_objects * p1_گام &&
	       p2_off < p2->pack->رقم_objects * p2_گام)
	{
		ثابت صحيح cmp = hashcmp(p1_پایه + p1_off, p2_پایه + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		إذا (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_پایه + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_پایه + p1_off,
					p2_hint);
			p1_off += p1_گام;
			p2_off += p2_گام;
			استمر;
		}
		إذا (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_گام;
		} وإلا { /* p2 has the object, p1 doesn't */
			p2_off += p2_گام;
		}
	}
}

ساكن اندازه_t sizeof_union(بنية packed_git *p1, بنية packed_git *p2)
{
	اندازه_t ret = 0;
	اندازه_t p1_off = 0, p2_off = 0, p1_گام, p2_گام;
	ثابت بیعلامت حرف *p1_پایه, *p2_پایه;
	ثابت بیعلامت صحيح hashsz = the_hash_algo->rawsz;

	p1_پایه = p1->شاخص_داده;
	p2_پایه = p2->شاخص_داده;
	p1_پایه += 256 * 4 + ((p1->شاخص_نسخه < 2) ? 4 : 8);
	p2_پایه += 256 * 4 + ((p2->شاخص_نسخه < 2) ? 4 : 8);
	p1_گام = hashsz + ((p1->شاخص_نسخه < 2) ? 4 : 0);
	p2_گام = hashsz + ((p2->شاخص_نسخه < 2) ? 4 : 0);

	طالما (p1_off < p1->رقم_objects * p1_گام &&
	       p2_off < p2->رقم_objects * p2_گام)
	{
		صحيح cmp = hashcmp(p1_پایه + p1_off, p2_پایه + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		إذا (cmp == 0) {
			ret++;
			p1_off += p1_گام;
			p2_off += p2_گام;
			استمر;
		}
		إذا (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_گام;
		} وإلا { /* p2 has the object, p1 doesn't */
			p2_off += p2_گام;
		}
	}
	أرجع ret;
}

/* another O(n^2) function ... */
ساكن اندازه_t get_pack_redundancy(بنية pack_list *pl)
{
	بنية pack_list *subset;
	اندازه_t ret = 0;

	إذا (!pl)
		أرجع 0;

	طالما ((subset = pl->بعدی)) {
		طالما (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->بعدی;
		}
		pl = pl->بعدی;
	}
	أرجع ret;
}

ساكن مضمن off_t pack_مجموعه_bytecount(بنية pack_list *pl)
{
	off_t ret = 0;
	طالما (pl) {
		ret += pl->pack->pack_اندازه;
		ret += pl->pack->شاخص_اندازه;
		pl = pl->بعدی;
	}
	أرجع ret;
}

ساكن صحيح cmp_مانده_objects(ثابت فراغ *a, ثابت فراغ *b)
{
	بنية pack_list *pl_a = *((بنية pack_list **)a);
	بنية pack_list *pl_b = *((بنية pack_list **)b);

	إذا (pl_a->مانده_objects->اندازه == pl_b->مانده_objects->اندازه) {
		/* have the same remaining_objects, big pack first */
		إذا (pl_a->all_objects_اندازه == pl_b->all_objects_اندازه)
			أرجع 0;
		وإلا إذا (pl_a->all_objects_اندازه < pl_b->all_objects_اندازه)
			أرجع 1;
		وإلا
			أرجع -1;
	} وإلا إذا (pl_a->مانده_objects->اندازه < pl_b->مانده_objects->اندازه) {
		/* sort by remaining objects, more objects first */
		أرجع 1;
	} وإلا {
		أرجع -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
ساكن فراغ مرتب_سازی_pack_list(بنية pack_list **pl)
{
	بنية pack_list **ary, *p;
	اندازه_t n = pack_list_اندازه(*pl);

	إذا (n < 2)
		أرجع;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	لكل (n = 0, p = *pl; p; p = p->بعدی)
		ary[n++] = p;

	QSORT(ary, n, cmp_مانده_objects);

	/* link them back again */
	لكل (اندازه_t i = 0; i < n - 1; i++)
		ary[i]->بعدی = ary[i + 1];
	ary[n - 1]->بعدی = عدم;
	*pl = ary[0];

	حرر(ary);
}


ساكن فراغ minimize(بنية pack_list **min)
{
	بنية pack_list *pl, *unique = عدم, *non_unique = عدم;
	بنية llist *missing, *unique_pack_objects;

	pl = local_packs;
	طالما (pl) {
		إذا (pl->unique_objects->اندازه)
			pack_list_درج(&unique, pl);
		وإلا
			pack_list_درج(&non_unique, pl);
		pl = pl->بعدی;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	طالما (pl) {
		llist_sorted_difference_inplace(missing, pl->مانده_objects);
		pl = pl->بعدی;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	إذا (missing->اندازه == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		أرجع;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	طالما (pl) {
		llist_sorted_difference_inplace(pl->مانده_objects, unique_pack_objects);
		pl = pl->بعدی;
	}

	طالما (non_unique) {
		بنية pack_list *بعدی;

		/* sort the non_unique packs, greater size of remaining_objects first */
		مرتب_سازی_pack_list(&non_unique);
		إذا (non_unique->مانده_objects->اندازه == 0)
			اكسر;

		pack_list_درج(min, non_unique);

		لكل (pl = non_unique->بعدی; pl && pl->مانده_objects->اندازه > 0;  pl = pl->بعدی)
			llist_sorted_difference_inplace(pl->مانده_objects, non_unique->مانده_objects);

		بعدی = non_unique->بعدی;
		حرر(non_unique);
		non_unique = بعدی;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

ساكن فراغ بارگیری_all_objects(فراغ)
{
	بنية pack_list *pl = local_packs;
	بنية llist_قلم *hint, *l;

	llist_init(&all_objects);

	طالما (pl) {
		hint = عدم;
		l = pl->مانده_objects->front;
		طالما (l) {
			hint = llist_درج_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->بعدی;
		}
		pl = pl->بعدی;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	طالما (pl) {
		llist_sorted_difference_inplace(all_objects, pl->مانده_objects);
		pl = pl->بعدی;
	}
}

/* this scales like O(n^2) */
ساكن فراغ cmp_local_packs(فراغ)
{
	بنية pack_list *subset, *pl = local_packs;

	/* only one packfile */
	إذا (!pl->بعدی) {
		llist_init(&pl->unique_objects);
		أرجع;
	}

	طالما ((subset = pl)) {
		طالما ((subset = subset->بعدی))
			cmp_two_packs(pl, subset);
		pl = pl->بعدی;
	}
}

ساكن فراغ پیمایش_alt_odb_packs(فراغ)
{
	بنية pack_list *local, *alt;

	alt = altodb_packs;
	طالما (alt) {
		local = local_packs;
		طالما (local) {
			llist_sorted_difference_inplace(local->مانده_objects,
							alt->مانده_objects);
			local = local->بعدی;
		}
		alt = alt->بعدی;
	}
}

ساكن بنية pack_list * جمع_کن_pack(بنية packed_git *p)
{
	بنية pack_list l;
	اندازه_t off = 0, گام;
	ثابت بیعلامت حرف *پایه;

	إذا (!p->pack_local && !(alt_odb || verbose))
		أرجع عدم;

	l.pack = p;
	llist_init(&l.مانده_objects);

	إذا (open_pack_شاخص(p)) {
		llist_free(l.مانده_objects);
		أرجع عدم;
	}

	پایه = p->شاخص_داده;
	پایه += 256 * 4 + ((p->شاخص_نسخه < 2) ? 4 : 8);
	گام = the_hash_algo->rawsz + ((p->شاخص_نسخه < 2) ? 4 : 0);
	طالما (off < p->رقم_objects * گام) {
		llist_درج_پشت(l.مانده_objects, پایه + off);
		off += گام;
	}
	l.all_objects_اندازه = l.مانده_objects->اندازه;
	l.unique_objects = عدم;
	إذا (p->pack_local)
		أرجع pack_list_درج(&local_packs, &l);
	وإلا
		أرجع pack_list_درج(&altodb_packs, &l);
}

ساكن بنية pack_list * جمع_کن_pack_file(ثابت حرف *filename)
{
	بنية packed_git *p;

	إذا (طول_رشته(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		إذا (جستجوی_رشته(p->pack_name, filename))
			أرجع جمع_کن_pack(p);
	die("Filename %s not found in packed_git", filename);
}

ساكن فراغ بارگیری_all(فراغ)
{
	بنية packed_git *p;

	repo_for_each_pack(the_repository, p)
		جمع_کن_pack(p);
}

صحيح cmd_pack_redundant(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند استفادهنشده, بنية repository *repo استفادهنشده) {
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
			بارگیری_all_packs = 1;
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

	إذا (بارگیری_all_packs)
		بارگیری_all();
	وإلا
		طالما (*(argv + i) != عدم)
			جمع_کن_pack_file(*(argv + i++));

	إذا (!local_packs)
		die("Zero packs found!");

	بارگیری_all_objects();

	إذا (alt_odb)
		پیمایش_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	إذا (!isatty(0)) {
		بنية object_id oid;
		طالما (fgets(buf, حجم(buf), stdin)) {
			إذا (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_درج_sorted_unique(ignore, &oid, عدم);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	طالما (pl) {
		llist_sorted_difference_inplace(pl->مانده_objects, ignore);
		pl = pl->بعدی;
	}

	cmp_local_packs();

	minimize(&min);

	إذا (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(بیعلامت دراز)pack_list_اندازه(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		طالما (pl) {
			fprintf(stderr, "\t%s\n", pl->pack->pack_name);
			pl = pl->بعدی;
		}
		fprintf(stderr, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(بیعلامت دراز)get_pack_redundancy(min),
			(بیعلامت دراز)pack_مجموعه_bytecount(min)/1024);
		fprintf(stderr, "A total of %lu unique objects were considered.\n",
			(بیعلامت دراز)all_objects->اندازه);
		fprintf(stderr, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	طالما (pl) {
		چاپ_کن("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->بعدی;
	}
	إذا (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(بیعلامت دراز)pack_مجموعه_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	أرجع 0;
}
