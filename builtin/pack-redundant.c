/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hex.h"

#ضمن "packfile.h"
#ضمن "odb.h"
#ضمن "strbuf.h"

#تعریف_کرو BLKSIZE 512

ساكن ثابت حرف pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

ساكن صحيح لدو_all_packs, verbose, alt_odb;

بنية llist_شے {
	بنية llist_شے *اگلا;
	بنية object_id oid;
};
ساكن بنية llist {
	بنية llist_شے *front;
	بنية llist_شے *پٹھ;
	ناپ_t ناپ;
} *all_objects; /* all objects which must be present in local packfiles */

ساكن بنية pack_list {
	بنية pack_list *اگلا;
	بنية packed_git *pack;
	بنية llist *unique_objects;
	بنية llist *بچیا_objects;
	ناپ_t all_objects_ناپ;
} *local_packs = عدم, *altodb_packs = عدم;

ساكن بنية llist_شے *free_nodes;

ساكن مضمن فراغ llist_شے_put(بنية llist_شے *شے)
{
	شے->اگلا = free_nodes;
	free_nodes = شے;
}

ساكن مضمن بنية llist_شے *llist_شے_get(فراغ)
{
	بنية llist_شے *new_شے;
	إذا ( free_nodes ) {
		new_شے = free_nodes;
		free_nodes = free_nodes->اگلا;
	} وإلا {
		صحيح i = 1;
		ALLOC_ARRAY(new_شے, BLKSIZE);
		لكل (; i < BLKSIZE; i++)
			llist_شے_put(&new_شے[i]);
	}
	أرجع new_شے;
}

ساكن مضمن فراغ llist_init(بنية llist **list)
{
	*list = xmalloc(حجم(بنية llist));
	(*list)->front = (*list)->پٹھ = عدم;
	(*list)->ناپ = 0;
}

ساكن فراغ llist_free(بنية llist *list)
{
	لكل (بنية llist_شے *i = list->front, *اگلا; i; i = اگلا) {
		اگلا = i->اگلا;
		llist_شے_put(i);
	}
	حرر(list);
}

ساكن بنية llist * llist_copy(بنية llist *list)
{
	بنية llist *ret;
	بنية llist_شے *new_شے, *old_شے, *prev;

	llist_init(&ret);

	إذا ((ret->ناپ = list->ناپ) == 0)
		أرجع ret;

	new_شے = ret->front = llist_شے_get();
	new_شے->oid = list->front->oid;

	old_شے = list->front->اگلا;
	طالما (old_شے) {
		prev = new_شے;
		new_شے = llist_شے_get();
		prev->اگلا = new_شے;
		new_شے->oid = old_شے->oid;
		old_شے = old_شے->اگلا;
	}
	new_شے->اگلا = عدم;
	ret->پٹھ = new_شے;

	أرجع ret;
}

ساكن مضمن بنية llist_شے *llist_داخل_کرو(بنية llist *list,
					      بنية llist_شے *after,
					      ثابت بنانشانی حرف *oid)
{
	بنية llist_شے *new_شے = llist_شے_get();
	oidread(&new_شے->oid, oid, the_repository->hash_algo);
	new_شے->اگلا = عدم;

	إذا (after) {
		new_شے->اگلا = after->اگلا;
		after->اگلا = new_شے;
		إذا (after == list->پٹھ)
			list->پٹھ = new_شے;
	} وإلا {/* insert in front */
		إذا (list->ناپ == 0)
			list->پٹھ = new_شے;
		وإلا
			new_شے->اگلا = list->front;
		list->front = new_شے;
	}
	list->ناپ++;
	أرجع new_شے;
}

ساكن مضمن بنية llist_شے *llist_داخل_کرو_پٹھ(بنية llist *list,
						   ثابت بنانشانی حرف *oid)
{
	أرجع llist_داخل_کرو(list, list->پٹھ, oid);
}

ساكن مضمن بنية llist_شے *llist_داخل_کرو_sorted_unique(بنية llist *list,
			ثابت بنية object_id *oid, بنية llist_شے *hint)
{
	بنية llist_شے *prev = عدم, *l;

	l = (hint == عدم) ? list->front : hint;
	طالما (l) {
		صحيح cmp = oidcmp(&l->oid, oid);
		إذا (cmp > 0) { /* we insert before this entry */
			أرجع llist_داخل_کرو(list, prev, oid->hash);
		}
		إذا (!cmp) { /* already exists */
			أرجع l;
		}
		prev = l;
		l = l->اگلا;
	}
	/* insert at the end */
	أرجع llist_داخل_کرو_پٹھ(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
ساكن مضمن بنية llist_شے * llist_sorted_remove(بنية llist *list, ثابت بنانشانی حرف *oid, بنية llist_شے *hint)
{
	بنية llist_شے *prev, *l;

redo_from_شروع:
	l = (hint == عدم) ? list->front : hint;
	prev = عدم;
	طالما (l) {
		ثابت صحيح cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		إذا (cmp > 0) /* not in list, since sorted */
			أرجع prev;
		إذا (!cmp) { /*لبھیا*/
			إذا (!prev) {
				إذا (hint != عدم && hint != list->front) {
					/* we don't know the previous element */
					hint = عدم;
					اذهب redo_from_شروع;
				}
				list->front = l->اگلا;
			} وإلا
				prev->اگلا = l->اگلا;
			إذا (l == list->پٹھ)
				list->پٹھ = prev;
			llist_شے_put(l);
			list->ناپ--;
			أرجع prev;
		}
		prev = l;
		l = l->اگلا;
	}
	أرجع prev;
}

/* computes A\B */
ساكن فراغ llist_sorted_difference_inplace(بنية llist *A,
				     بنية llist *B)
{
	بنية llist_شے *hint, *b;

	hint = عدم;
	b = B->front;

	طالما (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->اگلا;
	}
}

ساكن مضمن بنية pack_list * pack_list_داخل_کرو(بنية pack_list **pl,
					   بنية pack_list *اندراج)
{
	بنية pack_list *p = xmalloc(حجم(بنية pack_list));
	حافظہ_نقل(p, اندراج, حجم(بنية pack_list));
	p->اگلا = *pl;
	*pl = p;
	أرجع p;
}

ساكن فراغ pack_list_free(بنية pack_list *pl)
{
	لكل (بنية pack_list *اگلا; pl; pl = اگلا) {
		اگلا = pl->اگلا;
		حرر(pl);
	}
}

ساكن مضمن ناپ_t pack_list_ناپ(بنية pack_list *pl)
{
	ناپ_t ret = 0;
	طالما (pl) {
		ret++;
		pl = pl->اگلا;
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
			أرجع pack_list_difference(A->اگلا, B);
		pl = pl->اگلا;
	}
	ret = xmalloc(حجم(بنية pack_list));
	حافظہ_نقل(ret, A, حجم(بنية pack_list));
	ret->اگلا = pack_list_difference(A->اگلا, B);
	أرجع ret;
}

ساكن فراغ cmp_two_packs(بنية pack_list *p1, بنية pack_list *p2)
{
	ناپ_t p1_off = 0, p2_off = 0, p1_قدم, p2_قدم;
	ثابت بنانشانی حرف *p1_ادھار, *p2_ادھار;
	بنية llist_شے *p1_hint = عدم, *p2_hint = عدم;
	ثابت بنانشانی صحيح hashsz = the_hash_algo->rawsz;

	إذا (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->بچیا_objects);
	إذا (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->بچیا_objects);

	p1_ادھار = p1->pack->اشاریہ_انکڑے;
	p2_ادھار = p2->pack->اشاریہ_انکڑے;
	p1_ادھار += 256 * 4 + ((p1->pack->اشاریہ_نسخہ < 2) ? 4 : 8);
	p2_ادھار += 256 * 4 + ((p2->pack->اشاریہ_نسخہ < 2) ? 4 : 8);
	p1_قدم = hashsz + ((p1->pack->اشاریہ_نسخہ < 2) ? 4 : 0);
	p2_قدم = hashsz + ((p2->pack->اشاریہ_نسخہ < 2) ? 4 : 0);

	طالما (p1_off < p1->pack->عدد_objects * p1_قدم &&
	       p2_off < p2->pack->عدد_objects * p2_قدم)
	{
		ثابت صحيح cmp = hashcmp(p1_ادھار + p1_off, p2_ادھار + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		إذا (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_ادھار + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_ادھار + p1_off,
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

ساكن ناپ_t sizeof_union(بنية packed_git *p1, بنية packed_git *p2)
{
	ناپ_t ret = 0;
	ناپ_t p1_off = 0, p2_off = 0, p1_قدم, p2_قدم;
	ثابت بنانشانی حرف *p1_ادھار, *p2_ادھار;
	ثابت بنانشانی صحيح hashsz = the_hash_algo->rawsz;

	p1_ادھار = p1->اشاریہ_انکڑے;
	p2_ادھار = p2->اشاریہ_انکڑے;
	p1_ادھار += 256 * 4 + ((p1->اشاریہ_نسخہ < 2) ? 4 : 8);
	p2_ادھار += 256 * 4 + ((p2->اشاریہ_نسخہ < 2) ? 4 : 8);
	p1_قدم = hashsz + ((p1->اشاریہ_نسخہ < 2) ? 4 : 0);
	p2_قدم = hashsz + ((p2->اشاریہ_نسخہ < 2) ? 4 : 0);

	طالما (p1_off < p1->عدد_objects * p1_قدم &&
	       p2_off < p2->عدد_objects * p2_قدم)
	{
		صحيح cmp = hashcmp(p1_ادھار + p1_off, p2_ادھار + p2_off,
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
ساكن ناپ_t get_pack_redundancy(بنية pack_list *pl)
{
	بنية pack_list *subset;
	ناپ_t ret = 0;

	إذا (!pl)
		أرجع 0;

	طالما ((subset = pl->اگلا)) {
		طالما (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->اگلا;
		}
		pl = pl->اگلا;
	}
	أرجع ret;
}

ساكن مضمن off_t pack_سیٹ_bytecount(بنية pack_list *pl)
{
	off_t ret = 0;
	طالما (pl) {
		ret += pl->pack->pack_ناپ;
		ret += pl->pack->اشاریہ_ناپ;
		pl = pl->اگلا;
	}
	أرجع ret;
}

ساكن صحيح cmp_بچیا_objects(ثابت فراغ *a, ثابت فراغ *b)
{
	بنية pack_list *pl_a = *((بنية pack_list **)a);
	بنية pack_list *pl_b = *((بنية pack_list **)b);

	إذا (pl_a->بچیا_objects->ناپ == pl_b->بچیا_objects->ناپ) {
		/* have the same remaining_objects, big pack first */
		إذا (pl_a->all_objects_ناپ == pl_b->all_objects_ناپ)
			أرجع 0;
		وإلا إذا (pl_a->all_objects_ناپ < pl_b->all_objects_ناپ)
			أرجع 1;
		وإلا
			أرجع -1;
	} وإلا إذا (pl_a->بچیا_objects->ناپ < pl_b->بچیا_objects->ناپ) {
		/* sort by remaining objects, more objects first */
		أرجع 1;
	} وإلا {
		أرجع -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
ساكن فراغ ترتیب_دیو_pack_list(بنية pack_list **pl)
{
	بنية pack_list **ary, *p;
	ناپ_t n = pack_list_ناپ(*pl);

	إذا (n < 2)
		أرجع;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	لكل (n = 0, p = *pl; p; p = p->اگلا)
		ary[n++] = p;

	QSORT(ary, n, cmp_بچیا_objects);

	/* link them back again */
	لكل (ناپ_t i = 0; i < n - 1; i++)
		ary[i]->اگلا = ary[i + 1];
	ary[n - 1]->اگلا = عدم;
	*pl = ary[0];

	حرر(ary);
}


ساكن فراغ minimize(بنية pack_list **min)
{
	بنية pack_list *pl, *unique = عدم, *non_unique = عدم;
	بنية llist *missing, *unique_pack_objects;

	pl = local_packs;
	طالما (pl) {
		إذا (pl->unique_objects->ناپ)
			pack_list_داخل_کرو(&unique, pl);
		وإلا
			pack_list_داخل_کرو(&non_unique, pl);
		pl = pl->اگلا;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	طالما (pl) {
		llist_sorted_difference_inplace(missing, pl->بچیا_objects);
		pl = pl->اگلا;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	إذا (missing->ناپ == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		أرجع;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	طالما (pl) {
		llist_sorted_difference_inplace(pl->بچیا_objects, unique_pack_objects);
		pl = pl->اگلا;
	}

	طالما (non_unique) {
		بنية pack_list *اگلا;

		/* sort the non_unique packs, greater size of remaining_objects first */
		ترتیب_دیو_pack_list(&non_unique);
		إذا (non_unique->بچیا_objects->ناپ == 0)
			اكسر;

		pack_list_داخل_کرو(min, non_unique);

		لكل (pl = non_unique->اگلا; pl && pl->بچیا_objects->ناپ > 0;  pl = pl->اگلا)
			llist_sorted_difference_inplace(pl->بچیا_objects, non_unique->بچیا_objects);

		اگلا = non_unique->اگلا;
		حرر(non_unique);
		non_unique = اگلا;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

ساكن فراغ لدو_all_objects(فراغ)
{
	بنية pack_list *pl = local_packs;
	بنية llist_شے *hint, *l;

	llist_init(&all_objects);

	طالما (pl) {
		hint = عدم;
		l = pl->بچیا_objects->front;
		طالما (l) {
			hint = llist_داخل_کرو_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->اگلا;
		}
		pl = pl->اگلا;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	طالما (pl) {
		llist_sorted_difference_inplace(all_objects, pl->بچیا_objects);
		pl = pl->اگلا;
	}
}

/* this scales like O(n^2) */
ساكن فراغ cmp_local_packs(فراغ)
{
	بنية pack_list *subset, *pl = local_packs;

	/* only one packfile */
	إذا (!pl->اگلا) {
		llist_init(&pl->unique_objects);
		أرجع;
	}

	طالما ((subset = pl)) {
		طالما ((subset = subset->اگلا))
			cmp_two_packs(pl, subset);
		pl = pl->اگلا;
	}
}

ساكن فراغ جانچ_alt_odb_packs(فراغ)
{
	بنية pack_list *local, *alt;

	alt = altodb_packs;
	طالما (alt) {
		local = local_packs;
		طالما (local) {
			llist_sorted_difference_inplace(local->بچیا_objects,
							alt->بچیا_objects);
			local = local->اگلا;
		}
		alt = alt->اگلا;
	}
}

ساكن بنية pack_list * جوڑو_pack(بنية packed_git *p)
{
	بنية pack_list l;
	ناپ_t off = 0, قدم;
	ثابت بنانشانی حرف *ادھار;

	إذا (!p->pack_local && !(alt_odb || verbose))
		أرجع عدم;

	l.pack = p;
	llist_init(&l.بچیا_objects);

	إذا (open_pack_اشاریہ(p)) {
		llist_free(l.بچیا_objects);
		أرجع عدم;
	}

	ادھار = p->اشاریہ_انکڑے;
	ادھار += 256 * 4 + ((p->اشاریہ_نسخہ < 2) ? 4 : 8);
	قدم = the_hash_algo->rawsz + ((p->اشاریہ_نسخہ < 2) ? 4 : 0);
	طالما (off < p->عدد_objects * قدم) {
		llist_داخل_کرو_پٹھ(l.بچیا_objects, ادھار + off);
		off += قدم;
	}
	l.all_objects_ناپ = l.بچیا_objects->ناپ;
	l.unique_objects = عدم;
	إذا (p->pack_local)
		أرجع pack_list_داخل_کرو(&local_packs, &l);
	وإلا
		أرجع pack_list_داخل_کرو(&altodb_packs, &l);
}

ساكن بنية pack_list * جوڑو_pack_file(ثابت حرف *filename)
{
	بنية packed_git *p;

	إذا (لڑی_لمبائی(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		إذا (لڑی_کھوج(p->pack_name, filename))
			أرجع جوڑو_pack(p);
	die("Filename %s not found in packed_git", filename);
}

ساكن فراغ لدو_all(فراغ)
{
	بنية packed_git *p;

	repo_for_each_pack(the_repository, p)
		جوڑو_pack(p);
}

صحيح cmd_pack_redundant(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر بن_ورتیا, بنية repository *repo بن_ورتیا) {
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
			لدو_all_packs = 1;
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

	إذا (لدو_all_packs)
		لدو_all();
	وإلا
		طالما (*(argv + i) != عدم)
			جوڑو_pack_file(*(argv + i++));

	إذا (!local_packs)
		die("Zero packs found!");

	لدو_all_objects();

	إذا (alt_odb)
		جانچ_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	إذا (!isatty(0)) {
		بنية object_id oid;
		طالما (fgets(buf, حجم(buf), stdin)) {
			إذا (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_داخل_کرو_sorted_unique(ignore, &oid, عدم);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	طالما (pl) {
		llist_sorted_difference_inplace(pl->بچیا_objects, ignore);
		pl = pl->اگلا;
	}

	cmp_local_packs();

	minimize(&min);

	إذا (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(بنانشانی لما)pack_list_ناپ(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		طالما (pl) {
			fprintf(stderr, "\t%s\n", pl->pack->pack_name);
			pl = pl->اگلا;
		}
		fprintf(stderr, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(بنانشانی لما)get_pack_redundancy(min),
			(بنانشانی لما)pack_سیٹ_bytecount(min)/1024);
		fprintf(stderr, "A total of %lu unique objects were considered.\n",
			(بنانشانی لما)all_objects->ناپ);
		fprintf(stderr, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	طالما (pl) {
		لکھو("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->اگلا;
	}
	إذا (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(بنانشانی لما)pack_سیٹ_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	أرجع 0;
}
