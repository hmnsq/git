/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hex.h"

#ضمن "packfile.h"
#ضمن "odb.h"
#ضمن "strbuf.h"

#تعریف_کړه BLKSIZE 512

ساكن ثابت حرف pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

ساكن صحيح بارول_all_packs, verbose, alt_odb;

بنية llist_توکی {
	بنية llist_توکی *بل;
	بنية object_id oid;
};
ساكن بنية llist {
	بنية llist_توکی *front;
	بنية llist_توکی *شا;
	اندازه_t اندازه;
} *all_objects; /* all objects which must be present in local packfiles */

ساكن بنية pack_list {
	بنية pack_list *بل;
	بنية packed_git *pack;
	بنية llist *unique_objects;
	بنية llist *پاتې_شوی_objects;
	اندازه_t all_objects_اندازه;
} *local_packs = عدم, *altodb_packs = عدم;

ساكن بنية llist_توکی *free_nodes;

ساكن مضمن فراغ llist_توکی_put(بنية llist_توکی *توکی)
{
	توکی->بل = free_nodes;
	free_nodes = توکی;
}

ساكن مضمن بنية llist_توکی *llist_توکی_get(فراغ)
{
	بنية llist_توکی *new_توکی;
	إذا ( free_nodes ) {
		new_توکی = free_nodes;
		free_nodes = free_nodes->بل;
	} وإلا {
		صحيح i = 1;
		ALLOC_ARRAY(new_توکی, BLKSIZE);
		لكل (; i < BLKSIZE; i++)
			llist_توکی_put(&new_توکی[i]);
	}
	أرجع new_توکی;
}

ساكن مضمن فراغ llist_init(بنية llist **list)
{
	*list = xmalloc(حجم(بنية llist));
	(*list)->front = (*list)->شا = عدم;
	(*list)->اندازه = 0;
}

ساكن فراغ llist_free(بنية llist *list)
{
	لكل (بنية llist_توکی *i = list->front, *بل; i; i = بل) {
		بل = i->بل;
		llist_توکی_put(i);
	}
	حرر(list);
}

ساكن بنية llist * llist_copy(بنية llist *list)
{
	بنية llist *ret;
	بنية llist_توکی *new_توکی, *old_توکی, *prev;

	llist_init(&ret);

	إذا ((ret->اندازه = list->اندازه) == 0)
		أرجع ret;

	new_توکی = ret->front = llist_توکی_get();
	new_توکی->oid = list->front->oid;

	old_توکی = list->front->بل;
	طالما (old_توکی) {
		prev = new_توکی;
		new_توکی = llist_توکی_get();
		prev->بل = new_توکی;
		new_توکی->oid = old_توکی->oid;
		old_توکی = old_توکی->بل;
	}
	new_توکی->بل = عدم;
	ret->شا = new_توکی;

	أرجع ret;
}

ساكن مضمن بنية llist_توکی *llist_ورننباسه(بنية llist *list,
					      بنية llist_توکی *after,
					      ثابت بینښه حرف *oid)
{
	بنية llist_توکی *new_توکی = llist_توکی_get();
	oidread(&new_توکی->oid, oid, the_repository->hash_algo);
	new_توکی->بل = عدم;

	إذا (after) {
		new_توکی->بل = after->بل;
		after->بل = new_توکی;
		إذا (after == list->شا)
			list->شا = new_توکی;
	} وإلا {/* insert in front */
		إذا (list->اندازه == 0)
			list->شا = new_توکی;
		وإلا
			new_توکی->بل = list->front;
		list->front = new_توکی;
	}
	list->اندازه++;
	أرجع new_توکی;
}

ساكن مضمن بنية llist_توکی *llist_ورننباسه_شا(بنية llist *list,
						   ثابت بینښه حرف *oid)
{
	أرجع llist_ورننباسه(list, list->شا, oid);
}

ساكن مضمن بنية llist_توکی *llist_ورننباسه_sorted_unique(بنية llist *list,
			ثابت بنية object_id *oid, بنية llist_توکی *hint)
{
	بنية llist_توکی *prev = عدم, *l;

	l = (hint == عدم) ? list->front : hint;
	طالما (l) {
		صحيح cmp = oidcmp(&l->oid, oid);
		إذا (cmp > 0) { /* we insert before this entry */
			أرجع llist_ورننباسه(list, prev, oid->hash);
		}
		إذا (!cmp) { /* already exists */
			أرجع l;
		}
		prev = l;
		l = l->بل;
	}
	/* insert at the end */
	أرجع llist_ورننباسه_شا(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
ساكن مضمن بنية llist_توکی * llist_sorted_remove(بنية llist *list, ثابت بینښه حرف *oid, بنية llist_توکی *hint)
{
	بنية llist_توکی *prev, *l;

redo_from_پیل:
	l = (hint == عدم) ? list->front : hint;
	prev = عدم;
	طالما (l) {
		ثابت صحيح cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		إذا (cmp > 0) /* not in list, since sorted */
			أرجع prev;
		إذا (!cmp) { /*وموندل*/
			إذا (!prev) {
				إذا (hint != عدم && hint != list->front) {
					/* we don't know the previous element */
					hint = عدم;
					اذهب redo_from_پیل;
				}
				list->front = l->بل;
			} وإلا
				prev->بل = l->بل;
			إذا (l == list->شا)
				list->شا = prev;
			llist_توکی_put(l);
			list->اندازه--;
			أرجع prev;
		}
		prev = l;
		l = l->بل;
	}
	أرجع prev;
}

/* computes A\B */
ساكن فراغ llist_sorted_difference_inplace(بنية llist *A,
				     بنية llist *B)
{
	بنية llist_توکی *hint, *b;

	hint = عدم;
	b = B->front;

	طالما (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->بل;
	}
}

ساكن مضمن بنية pack_list * pack_list_ورننباسه(بنية pack_list **pl,
					   بنية pack_list *ثبت)
{
	بنية pack_list *p = xmalloc(حجم(بنية pack_list));
	حافظه_نقل(p, ثبت, حجم(بنية pack_list));
	p->بل = *pl;
	*pl = p;
	أرجع p;
}

ساكن فراغ pack_list_free(بنية pack_list *pl)
{
	لكل (بنية pack_list *بل; pl; pl = بل) {
		بل = pl->بل;
		حرر(pl);
	}
}

ساكن مضمن اندازه_t pack_list_اندازه(بنية pack_list *pl)
{
	اندازه_t ret = 0;
	طالما (pl) {
		ret++;
		pl = pl->بل;
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
			أرجع pack_list_difference(A->بل, B);
		pl = pl->بل;
	}
	ret = xmalloc(حجم(بنية pack_list));
	حافظه_نقل(ret, A, حجم(بنية pack_list));
	ret->بل = pack_list_difference(A->بل, B);
	أرجع ret;
}

ساكن فراغ cmp_two_packs(بنية pack_list *p1, بنية pack_list *p2)
{
	اندازه_t p1_off = 0, p2_off = 0, p1_ګام, p2_ګام;
	ثابت بینښه حرف *p1_بنسټ, *p2_بنسټ;
	بنية llist_توکی *p1_hint = عدم, *p2_hint = عدم;
	ثابت بینښه صحيح hashsz = the_hash_algo->rawsz;

	إذا (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->پاتې_شوی_objects);
	إذا (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->پاتې_شوی_objects);

	p1_بنسټ = p1->pack->شاخص_معلومات;
	p2_بنسټ = p2->pack->شاخص_معلومات;
	p1_بنسټ += 256 * 4 + ((p1->pack->شاخص_نسخه < 2) ? 4 : 8);
	p2_بنسټ += 256 * 4 + ((p2->pack->شاخص_نسخه < 2) ? 4 : 8);
	p1_ګام = hashsz + ((p1->pack->شاخص_نسخه < 2) ? 4 : 0);
	p2_ګام = hashsz + ((p2->pack->شاخص_نسخه < 2) ? 4 : 0);

	طالما (p1_off < p1->pack->عدد_objects * p1_ګام &&
	       p2_off < p2->pack->عدد_objects * p2_ګام)
	{
		ثابت صحيح cmp = hashcmp(p1_بنسټ + p1_off, p2_بنسټ + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		إذا (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_بنسټ + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_بنسټ + p1_off,
					p2_hint);
			p1_off += p1_ګام;
			p2_off += p2_ګام;
			استمر;
		}
		إذا (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_ګام;
		} وإلا { /* p2 has the object, p1 doesn't */
			p2_off += p2_ګام;
		}
	}
}

ساكن اندازه_t sizeof_union(بنية packed_git *p1, بنية packed_git *p2)
{
	اندازه_t ret = 0;
	اندازه_t p1_off = 0, p2_off = 0, p1_ګام, p2_ګام;
	ثابت بینښه حرف *p1_بنسټ, *p2_بنسټ;
	ثابت بینښه صحيح hashsz = the_hash_algo->rawsz;

	p1_بنسټ = p1->شاخص_معلومات;
	p2_بنسټ = p2->شاخص_معلومات;
	p1_بنسټ += 256 * 4 + ((p1->شاخص_نسخه < 2) ? 4 : 8);
	p2_بنسټ += 256 * 4 + ((p2->شاخص_نسخه < 2) ? 4 : 8);
	p1_ګام = hashsz + ((p1->شاخص_نسخه < 2) ? 4 : 0);
	p2_ګام = hashsz + ((p2->شاخص_نسخه < 2) ? 4 : 0);

	طالما (p1_off < p1->عدد_objects * p1_ګام &&
	       p2_off < p2->عدد_objects * p2_ګام)
	{
		صحيح cmp = hashcmp(p1_بنسټ + p1_off, p2_بنسټ + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		إذا (cmp == 0) {
			ret++;
			p1_off += p1_ګام;
			p2_off += p2_ګام;
			استمر;
		}
		إذا (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_ګام;
		} وإلا { /* p2 has the object, p1 doesn't */
			p2_off += p2_ګام;
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

	طالما ((subset = pl->بل)) {
		طالما (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->بل;
		}
		pl = pl->بل;
	}
	أرجع ret;
}

ساكن مضمن off_t pack_سېټ_bytecount(بنية pack_list *pl)
{
	off_t ret = 0;
	طالما (pl) {
		ret += pl->pack->pack_اندازه;
		ret += pl->pack->شاخص_اندازه;
		pl = pl->بل;
	}
	أرجع ret;
}

ساكن صحيح cmp_پاتې_شوی_objects(ثابت فراغ *a, ثابت فراغ *b)
{
	بنية pack_list *pl_a = *((بنية pack_list **)a);
	بنية pack_list *pl_b = *((بنية pack_list **)b);

	إذا (pl_a->پاتې_شوی_objects->اندازه == pl_b->پاتې_شوی_objects->اندازه) {
		/* have the same remaining_objects, big pack first */
		إذا (pl_a->all_objects_اندازه == pl_b->all_objects_اندازه)
			أرجع 0;
		وإلا إذا (pl_a->all_objects_اندازه < pl_b->all_objects_اندازه)
			أرجع 1;
		وإلا
			أرجع -1;
	} وإلا إذا (pl_a->پاتې_شوی_objects->اندازه < pl_b->پاتې_شوی_objects->اندازه) {
		/* sort by remaining objects, more objects first */
		أرجع 1;
	} وإلا {
		أرجع -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
ساكن فراغ ترتیبول_pack_list(بنية pack_list **pl)
{
	بنية pack_list **ary, *p;
	اندازه_t n = pack_list_اندازه(*pl);

	إذا (n < 2)
		أرجع;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	لكل (n = 0, p = *pl; p; p = p->بل)
		ary[n++] = p;

	QSORT(ary, n, cmp_پاتې_شوی_objects);

	/* link them back again */
	لكل (اندازه_t i = 0; i < n - 1; i++)
		ary[i]->بل = ary[i + 1];
	ary[n - 1]->بل = عدم;
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
			pack_list_ورننباسه(&unique, pl);
		وإلا
			pack_list_ورننباسه(&non_unique, pl);
		pl = pl->بل;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	طالما (pl) {
		llist_sorted_difference_inplace(missing, pl->پاتې_شوی_objects);
		pl = pl->بل;
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
		llist_sorted_difference_inplace(pl->پاتې_شوی_objects, unique_pack_objects);
		pl = pl->بل;
	}

	طالما (non_unique) {
		بنية pack_list *بل;

		/* sort the non_unique packs, greater size of remaining_objects first */
		ترتیبول_pack_list(&non_unique);
		إذا (non_unique->پاتې_شوی_objects->اندازه == 0)
			اكسر;

		pack_list_ورننباسه(min, non_unique);

		لكل (pl = non_unique->بل; pl && pl->پاتې_شوی_objects->اندازه > 0;  pl = pl->بل)
			llist_sorted_difference_inplace(pl->پاتې_شوی_objects, non_unique->پاتې_شوی_objects);

		بل = non_unique->بل;
		حرر(non_unique);
		non_unique = بل;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

ساكن فراغ بارول_all_objects(فراغ)
{
	بنية pack_list *pl = local_packs;
	بنية llist_توکی *hint, *l;

	llist_init(&all_objects);

	طالما (pl) {
		hint = عدم;
		l = pl->پاتې_شوی_objects->front;
		طالما (l) {
			hint = llist_ورننباسه_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->بل;
		}
		pl = pl->بل;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	طالما (pl) {
		llist_sorted_difference_inplace(all_objects, pl->پاتې_شوی_objects);
		pl = pl->بل;
	}
}

/* this scales like O(n^2) */
ساكن فراغ cmp_local_packs(فراغ)
{
	بنية pack_list *subset, *pl = local_packs;

	/* only one packfile */
	إذا (!pl->بل) {
		llist_init(&pl->unique_objects);
		أرجع;
	}

	طالما ((subset = pl)) {
		طالما ((subset = subset->بل))
			cmp_two_packs(pl, subset);
		pl = pl->بل;
	}
}

ساكن فراغ لټون_alt_odb_packs(فراغ)
{
	بنية pack_list *local, *alt;

	alt = altodb_packs;
	طالما (alt) {
		local = local_packs;
		طالما (local) {
			llist_sorted_difference_inplace(local->پاتې_شوی_objects,
							alt->پاتې_شوی_objects);
			local = local->بل;
		}
		alt = alt->بل;
	}
}

ساكن بنية pack_list * جمع_کړه_pack(بنية packed_git *p)
{
	بنية pack_list l;
	اندازه_t off = 0, ګام;
	ثابت بینښه حرف *بنسټ;

	إذا (!p->pack_local && !(alt_odb || verbose))
		أرجع عدم;

	l.pack = p;
	llist_init(&l.پاتې_شوی_objects);

	إذا (open_pack_شاخص(p)) {
		llist_free(l.پاتې_شوی_objects);
		أرجع عدم;
	}

	بنسټ = p->شاخص_معلومات;
	بنسټ += 256 * 4 + ((p->شاخص_نسخه < 2) ? 4 : 8);
	ګام = the_hash_algo->rawsz + ((p->شاخص_نسخه < 2) ? 4 : 0);
	طالما (off < p->عدد_objects * ګام) {
		llist_ورننباسه_شا(l.پاتې_شوی_objects, بنسټ + off);
		off += ګام;
	}
	l.all_objects_اندازه = l.پاتې_شوی_objects->اندازه;
	l.unique_objects = عدم;
	إذا (p->pack_local)
		أرجع pack_list_ورننباسه(&local_packs, &l);
	وإلا
		أرجع pack_list_ورننباسه(&altodb_packs, &l);
}

ساكن بنية pack_list * جمع_کړه_pack_file(ثابت حرف *filename)
{
	بنية packed_git *p;

	إذا (مزي_اوږدوالی(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		إذا (مزي_پلټنه(p->pack_name, filename))
			أرجع جمع_کړه_pack(p);
	die("Filename %s not found in packed_git", filename);
}

ساكن فراغ بارول_all(فراغ)
{
	بنية packed_git *p;

	repo_for_each_pack(the_repository, p)
		جمع_کړه_pack(p);
}

صحيح cmd_pack_redundant(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی ناکارول_شوی, بنية repository *repo ناکارول_شوی) {
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
			بارول_all_packs = 1;
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

	إذا (بارول_all_packs)
		بارول_all();
	وإلا
		طالما (*(argv + i) != عدم)
			جمع_کړه_pack_file(*(argv + i++));

	إذا (!local_packs)
		die("Zero packs found!");

	بارول_all_objects();

	إذا (alt_odb)
		لټون_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	إذا (!isatty(0)) {
		بنية object_id oid;
		طالما (fgets(buf, حجم(buf), stdin)) {
			إذا (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_ورننباسه_sorted_unique(ignore, &oid, عدم);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	طالما (pl) {
		llist_sorted_difference_inplace(pl->پاتې_شوی_objects, ignore);
		pl = pl->بل;
	}

	cmp_local_packs();

	minimize(&min);

	إذا (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(بینښه اوږد)pack_list_اندازه(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		طالما (pl) {
			fprintf(stderr, "\t%s\n", pl->pack->pack_name);
			pl = pl->بل;
		}
		fprintf(stderr, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(بینښه اوږد)get_pack_redundancy(min),
			(بینښه اوږد)pack_سېټ_bytecount(min)/1024);
		fprintf(stderr, "A total of %lu unique objects were considered.\n",
			(بینښه اوږد)all_objects->اندازه);
		fprintf(stderr, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	طالما (pl) {
		چاپ_کړه("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->بل;
	}
	إذا (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(بینښه اوږد)pack_سېټ_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	أرجع 0;
}
