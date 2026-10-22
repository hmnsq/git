/*
*
* Copyright 2005, Lukas Sandstrom <lukass@etek.chalmers.se>
*
* This file is licensed under the GPL v2.
*
*/

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "gettext.h"
#شامل "hex.h"

#شامل "packfile.h"
#شامل "odb.h"
#شامل "strbuf.h"

#تعریف_کرو BLKSIZE 512

ساکن ثابت حرف pack_redundant_usage[] =
"git pack-redundant [--verbose] [--alt-odb] (--all | <pack-filename>...)";

ساکن صحیح بھرو_all_packs, verbose, alt_odb;

ساخت llist_شے {
	ساخت llist_شے *اگلا;
	ساخت object_id oid;
};
ساکن ساخت llist {
	ساخت llist_شے *front;
	ساخت llist_شے *عقب;
	سائز_t سائز;
} *all_objects; /* all objects which must be present in local packfiles */

ساکن ساخت pack_list {
	ساخت pack_list *اگلا;
	ساخت packed_git *pack;
	ساخت llist *unique_objects;
	ساخت llist *بقایا_objects;
	سائز_t all_objects_سائز;
} *local_packs = تہی, *altodb_packs = تہی;

ساکن ساخت llist_شے *free_nodes;

ساکن اندرونی خالی llist_شے_put(ساخت llist_شے *شے)
{
	شے->اگلا = free_nodes;
	free_nodes = شے;
}

ساکن اندرونی ساخت llist_شے *llist_شے_get(خالی)
{
	ساخت llist_شے *new_شے;
	اگر ( free_nodes ) {
		new_شے = free_nodes;
		free_nodes = free_nodes->اگلا;
	} ورنہ {
		صحیح i = 1;
		ALLOC_ARRAY(new_شے, BLKSIZE);
		برائے (; i < BLKSIZE; i++)
			llist_شے_put(&new_شے[i]);
	}
	واپس new_شے;
}

ساکن اندرونی خالی llist_init(ساخت llist **list)
{
	*list = xmalloc(حجم(ساخت llist));
	(*list)->front = (*list)->عقب = تہی;
	(*list)->سائز = 0;
}

ساکن خالی llist_free(ساخت llist *list)
{
	برائے (ساخت llist_شے *i = list->front, *اگلا; i; i = اگلا) {
		اگلا = i->اگلا;
		llist_شے_put(i);
	}
	آزاد(list);
}

ساکن ساخت llist * llist_copy(ساخت llist *list)
{
	ساخت llist *ret;
	ساخت llist_شے *new_شے, *old_شے, *پچھلا_2;

	llist_init(&ret);

	اگر ((ret->سائز = list->سائز) == 0)
		واپس ret;

	new_شے = ret->front = llist_شے_get();
	new_شے->oid = list->front->oid;

	old_شے = list->front->اگلا;
	جب (old_شے) {
		پچھلا_2 = new_شے;
		new_شے = llist_شے_get();
		پچھلا_2->اگلا = new_شے;
		new_شے->oid = old_شے->oid;
		old_شے = old_شے->اگلا;
	}
	new_شے->اگلا = تہی;
	ret->عقب = new_شے;

	واپس ret;
}

ساکن اندرونی ساخت llist_شے *llist_داخل_کرو(ساخت llist *list,
					      ساخت llist_شے *after,
					      ثابت غیرعلامتی حرف *oid)
{
	ساخت llist_شے *new_شے = llist_شے_get();
	oidread(&new_شے->oid, oid, the_repository->hash_algo);
	new_شے->اگلا = تہی;

	اگر (after) {
		new_شے->اگلا = after->اگلا;
		after->اگلا = new_شے;
		اگر (after == list->عقب)
			list->عقب = new_شے;
	} ورنہ {/* insert in front */
		اگر (list->سائز == 0)
			list->عقب = new_شے;
		ورنہ
			new_شے->اگلا = list->front;
		list->front = new_شے;
	}
	list->سائز++;
	واپس new_شے;
}

ساکن اندرونی ساخت llist_شے *llist_داخل_کرو_عقب(ساخت llist *list,
						   ثابت غیرعلامتی حرف *oid)
{
	واپس llist_داخل_کرو(list, list->عقب, oid);
}

ساکن اندرونی ساخت llist_شے *llist_داخل_کرو_sorted_unique(ساخت llist *list,
			ثابت ساخت object_id *oid, ساخت llist_شے *hint)
{
	ساخت llist_شے *پچھلا_2 = تہی, *l;

	l = (hint == تہی) ? list->front : hint;
	جب (l) {
		صحیح cmp = oidcmp(&l->oid, oid);
		اگر (cmp > 0) { /* we insert before this entry */
			واپس llist_داخل_کرو(list, پچھلا_2, oid->hash);
		}
		اگر (!cmp) { /* already exists */
			واپس l;
		}
		پچھلا_2 = l;
		l = l->اگلا;
	}
	/* insert at the end */
	واپس llist_داخل_کرو_عقب(list, oid->hash);
}

/* returns a pointer to an item in front of sha1 */
ساکن اندرونی ساخت llist_شے * llist_sorted_remove(ساخت llist *list, ثابت غیرعلامتی حرف *oid, ساخت llist_شے *hint)
{
	ساخت llist_شے *پچھلا_2, *l;

redo_from_آغاز:
	l = (hint == تہی) ? list->front : hint;
	پچھلا_2 = تہی;
	جب (l) {
		ثابت صحیح cmp = hashcmp(l->oid.hash, oid, the_repository->hash_algo);
		اگر (cmp > 0) /* not in list, since sorted */
			واپس پچھلا_2;
		اگر (!cmp) { /*ملا*/
			اگر (!پچھلا_2) {
				اگر (hint != تہی && hint != list->front) {
					/* we don't know the previous element */
					hint = تہی;
					جاؤ redo_from_آغاز;
				}
				list->front = l->اگلا;
			} ورنہ
				پچھلا_2->اگلا = l->اگلا;
			اگر (l == list->عقب)
				list->عقب = پچھلا_2;
			llist_شے_put(l);
			list->سائز--;
			واپس پچھلا_2;
		}
		پچھلا_2 = l;
		l = l->اگلا;
	}
	واپس پچھلا_2;
}

/* computes A\B */
ساکن خالی llist_sorted_difference_inplace(ساخت llist *A,
				     ساخت llist *B)
{
	ساخت llist_شے *hint, *b;

	hint = تہی;
	b = B->front;

	جب (b) {
		hint = llist_sorted_remove(A, b->oid.hash, hint);
		b = b->اگلا;
	}
}

ساکن اندرونی ساخت pack_list * pack_list_داخل_کرو(ساخت pack_list **pl,
					   ساخت pack_list *اندراج_2)
{
	ساخت pack_list *p = xmalloc(حجم(ساخت pack_list));
	حافظہ_نقل(p, اندراج_2, حجم(ساخت pack_list));
	p->اگلا = *pl;
	*pl = p;
	واپس p;
}

ساکن خالی pack_list_free(ساخت pack_list *pl)
{
	برائے (ساخت pack_list *اگلا; pl; pl = اگلا) {
		اگلا = pl->اگلا;
		آزاد(pl);
	}
}

ساکن اندرونی سائز_t pack_list_سائز(ساخت pack_list *pl)
{
	سائز_t ret = 0;
	جب (pl) {
		ret++;
		pl = pl->اگلا;
	}
	واپس ret;
}

ساکن ساخت pack_list * pack_list_difference(ثابت ساخت pack_list *A,
					       ثابت ساخت pack_list *B)
{
	ساخت pack_list *ret;
	ثابت ساخت pack_list *pl;

	اگر (!A)
		واپس تہی;

	pl = B;
	جب (pl != تہی) {
		اگر (A->pack == pl->pack)
			واپس pack_list_difference(A->اگلا, B);
		pl = pl->اگلا;
	}
	ret = xmalloc(حجم(ساخت pack_list));
	حافظہ_نقل(ret, A, حجم(ساخت pack_list));
	ret->اگلا = pack_list_difference(A->اگلا, B);
	واپس ret;
}

ساکن خالی cmp_two_packs(ساخت pack_list *p1, ساخت pack_list *p2)
{
	سائز_t p1_off = 0, p2_off = 0, p1_قدم, p2_قدم;
	ثابت غیرعلامتی حرف *p1_بنیاد, *p2_بنیاد;
	ساخت llist_شے *p1_hint = تہی, *p2_hint = تہی;
	ثابت غیرعلامتی صحیح hashsz = the_hash_algo->rawsz;

	اگر (!p1->unique_objects)
		p1->unique_objects = llist_copy(p1->بقایا_objects);
	اگر (!p2->unique_objects)
		p2->unique_objects = llist_copy(p2->بقایا_objects);

	p1_بنیاد = p1->pack->اشاریہ_ڈیٹا;
	p2_بنیاد = p2->pack->اشاریہ_ڈیٹا;
	p1_بنیاد += 256 * 4 + ((p1->pack->اشاریہ_نسخہ < 2) ? 4 : 8);
	p2_بنیاد += 256 * 4 + ((p2->pack->اشاریہ_نسخہ < 2) ? 4 : 8);
	p1_قدم = hashsz + ((p1->pack->اشاریہ_نسخہ < 2) ? 4 : 0);
	p2_قدم = hashsz + ((p2->pack->اشاریہ_نسخہ < 2) ? 4 : 0);

	جب (p1_off < p1->pack->عدد_objects * p1_قدم &&
	       p2_off < p2->pack->عدد_objects * p2_قدم)
	{
		ثابت صحیح cmp = hashcmp(p1_بنیاد + p1_off, p2_بنیاد + p2_off,
					the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		اگر (cmp == 0) {
			p1_hint = llist_sorted_remove(p1->unique_objects,
					p1_بنیاد + p1_off,
					p1_hint);
			p2_hint = llist_sorted_remove(p2->unique_objects,
					p1_بنیاد + p1_off,
					p2_hint);
			p1_off += p1_قدم;
			p2_off += p2_قدم;
			جاری;
		}
		اگر (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_قدم;
		} ورنہ { /* p2 has the object, p1 doesn't */
			p2_off += p2_قدم;
		}
	}
}

ساکن سائز_t sizeof_union(ساخت packed_git *p1, ساخت packed_git *p2)
{
	سائز_t ret = 0;
	سائز_t p1_off = 0, p2_off = 0, p1_قدم, p2_قدم;
	ثابت غیرعلامتی حرف *p1_بنیاد, *p2_بنیاد;
	ثابت غیرعلامتی صحیح hashsz = the_hash_algo->rawsz;

	p1_بنیاد = p1->اشاریہ_ڈیٹا;
	p2_بنیاد = p2->اشاریہ_ڈیٹا;
	p1_بنیاد += 256 * 4 + ((p1->اشاریہ_نسخہ < 2) ? 4 : 8);
	p2_بنیاد += 256 * 4 + ((p2->اشاریہ_نسخہ < 2) ? 4 : 8);
	p1_قدم = hashsz + ((p1->اشاریہ_نسخہ < 2) ? 4 : 0);
	p2_قدم = hashsz + ((p2->اشاریہ_نسخہ < 2) ? 4 : 0);

	جب (p1_off < p1->عدد_objects * p1_قدم &&
	       p2_off < p2->عدد_objects * p2_قدم)
	{
		صحیح cmp = hashcmp(p1_بنیاد + p1_off, p2_بنیاد + p2_off,
				  the_repository->hash_algo);
		/* cmp ~ p1 - p2 */
		اگر (cmp == 0) {
			ret++;
			p1_off += p1_قدم;
			p2_off += p2_قدم;
			جاری;
		}
		اگر (cmp < 0) { /* p1 has the object, p2 doesn't */
			p1_off += p1_قدم;
		} ورنہ { /* p2 has the object, p1 doesn't */
			p2_off += p2_قدم;
		}
	}
	واپس ret;
}

/* another O(n^2) function ... */
ساکن سائز_t get_pack_redundancy(ساخت pack_list *pl)
{
	ساخت pack_list *subset;
	سائز_t ret = 0;

	اگر (!pl)
		واپس 0;

	جب ((subset = pl->اگلا)) {
		جب (subset) {
			ret += sizeof_union(pl->pack, subset->pack);
			subset = subset->اگلا;
		}
		pl = pl->اگلا;
	}
	واپس ret;
}

ساکن اندرونی off_t pack_سیٹ_bytecount(ساخت pack_list *pl)
{
	off_t ret = 0;
	جب (pl) {
		ret += pl->pack->pack_سائز;
		ret += pl->pack->اشاریہ_سائز;
		pl = pl->اگلا;
	}
	واپس ret;
}

ساکن صحیح cmp_بقایا_objects(ثابت خالی *a, ثابت خالی *b)
{
	ساخت pack_list *pl_a = *((ساخت pack_list **)a);
	ساخت pack_list *pl_b = *((ساخت pack_list **)b);

	اگر (pl_a->بقایا_objects->سائز == pl_b->بقایا_objects->سائز) {
		/* have the same remaining_objects, big pack first */
		اگر (pl_a->all_objects_سائز == pl_b->all_objects_سائز)
			واپس 0;
		ورنہ اگر (pl_a->all_objects_سائز < pl_b->all_objects_سائز)
			واپس 1;
		ورنہ
			واپس -1;
	} ورنہ اگر (pl_a->بقایا_objects->سائز < pl_b->بقایا_objects->سائز) {
		/* sort by remaining objects, more objects first */
		واپس 1;
	} ورنہ {
		واپس -1;
	}
}

/* Sort pack_list, greater size of remaining_objects first */
ساکن خالی ترتیب_دو_pack_list(ساخت pack_list **pl)
{
	ساخت pack_list **ary, *p;
	سائز_t n = pack_list_سائز(*pl);

	اگر (n < 2)
		واپس;

	/* prepare an array of packed_list for easier sorting */
	CALLOC_ARRAY(ary, n);
	برائے (n = 0, p = *pl; p; p = p->اگلا)
		ary[n++] = p;

	QSORT(ary, n, cmp_بقایا_objects);

	/* link them back again */
	برائے (سائز_t i = 0; i < n - 1; i++)
		ary[i]->اگلا = ary[i + 1];
	ary[n - 1]->اگلا = تہی;
	*pl = ary[0];

	آزاد(ary);
}


ساکن خالی minimize(ساخت pack_list **min)
{
	ساخت pack_list *pl, *unique = تہی, *non_unique = تہی;
	ساخت llist *missing, *unique_pack_objects;

	pl = local_packs;
	جب (pl) {
		اگر (pl->unique_objects->سائز)
			pack_list_داخل_کرو(&unique, pl);
		ورنہ
			pack_list_داخل_کرو(&non_unique, pl);
		pl = pl->اگلا;
	}
	/* find out which objects are missing from the set of unique packs */
	missing = llist_copy(all_objects);
	pl = unique;
	جب (pl) {
		llist_sorted_difference_inplace(missing, pl->بقایا_objects);
		pl = pl->اگلا;
	}

	*min = unique;

	/* return if there are no objects missing from the unique set */
	اگر (missing->سائز == 0) {
		llist_free(missing);
		pack_list_free(non_unique);
		واپس;
	}

	unique_pack_objects = llist_copy(all_objects);
	llist_sorted_difference_inplace(unique_pack_objects, missing);

	/* remove unique pack objects from the non_unique packs */
	pl = non_unique;
	جب (pl) {
		llist_sorted_difference_inplace(pl->بقایا_objects, unique_pack_objects);
		pl = pl->اگلا;
	}

	جب (non_unique) {
		ساخت pack_list *اگلا;

		/* sort the non_unique packs, greater size of remaining_objects first */
		ترتیب_دو_pack_list(&non_unique);
		اگر (non_unique->بقایا_objects->سائز == 0)
			توڑو;

		pack_list_داخل_کرو(min, non_unique);

		برائے (pl = non_unique->اگلا; pl && pl->بقایا_objects->سائز > 0;  pl = pl->اگلا)
			llist_sorted_difference_inplace(pl->بقایا_objects, non_unique->بقایا_objects);

		اگلا = non_unique->اگلا;
		آزاد(non_unique);
		non_unique = اگلا;
	}

	pack_list_free(non_unique);
	llist_free(unique_pack_objects);
	llist_free(missing);
}

ساکن خالی بھرو_all_objects(خالی)
{
	ساخت pack_list *pl = local_packs;
	ساخت llist_شے *hint, *l;

	llist_init(&all_objects);

	جب (pl) {
		hint = تہی;
		l = pl->بقایا_objects->front;
		جب (l) {
			hint = llist_داخل_کرو_sorted_unique(all_objects,
							  &l->oid, hint);
			l = l->اگلا;
		}
		pl = pl->اگلا;
	}
	/* remove objects present in remote packs */
	pl = altodb_packs;
	جب (pl) {
		llist_sorted_difference_inplace(all_objects, pl->بقایا_objects);
		pl = pl->اگلا;
	}
}

/* this scales like O(n^2) */
ساکن خالی cmp_local_packs(خالی)
{
	ساخت pack_list *subset, *pl = local_packs;

	/* only one packfile */
	اگر (!pl->اگلا) {
		llist_init(&pl->unique_objects);
		واپس;
	}

	جب ((subset = pl)) {
		جب ((subset = subset->اگلا))
			cmp_two_packs(pl, subset);
		pl = pl->اگلا;
	}
}

ساکن خالی جائزہ_alt_odb_packs(خالی)
{
	ساخت pack_list *local, *alt;

	alt = altodb_packs;
	جب (alt) {
		local = local_packs;
		جب (local) {
			llist_sorted_difference_inplace(local->بقایا_objects,
							alt->بقایا_objects);
			local = local->اگلا;
		}
		alt = alt->اگلا;
	}
}

ساکن ساخت pack_list * جوڑو_pack(ساخت packed_git *p)
{
	ساخت pack_list l;
	سائز_t off = 0, قدم;
	ثابت غیرعلامتی حرف *بنیاد;

	اگر (!p->pack_local && !(alt_odb || verbose))
		واپس تہی;

	l.pack = p;
	llist_init(&l.بقایا_objects);

	اگر (open_pack_اشاریہ(p)) {
		llist_free(l.بقایا_objects);
		واپس تہی;
	}

	بنیاد = p->اشاریہ_ڈیٹا;
	بنیاد += 256 * 4 + ((p->اشاریہ_نسخہ < 2) ? 4 : 8);
	قدم = the_hash_algo->rawsz + ((p->اشاریہ_نسخہ < 2) ? 4 : 0);
	جب (off < p->عدد_objects * قدم) {
		llist_داخل_کرو_عقب(l.بقایا_objects, بنیاد + off);
		off += قدم;
	}
	l.all_objects_سائز = l.بقایا_objects->سائز;
	l.unique_objects = تہی;
	اگر (p->pack_local)
		واپس pack_list_داخل_کرو(&local_packs, &l);
	ورنہ
		واپس pack_list_داخل_کرو(&altodb_packs, &l);
}

ساکن ساخت pack_list * جوڑو_pack_file(ثابت حرف *filename)
{
	ساخت packed_git *p;

	اگر (لڑی_لمبائی(filename) < 40)
		die("Bad pack filename: %s", filename);

	repo_for_each_pack(the_repository, p)
		اگر (لڑی_تلاش(p->pack_name, filename))
			واپس جوڑو_pack(p);
	die("Filename %s not found in packed_git", filename);
}

ساکن خالی بھرو_all(خالی)
{
	ساخت packed_git *p;

	repo_for_each_pack(the_repository, p)
		جوڑو_pack(p);
}

صحیح cmd_pack_redundant(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ غیر_مستعمل, ساخت repository *repo غیر_مستعمل) {
	صحیح i; صحیح i_still_use_this = 0; ساخت pack_list *min = تہی, *red, *pl;
	ساخت llist *ignore;
	ساخت strbuf idx_name = STRBUF_INIT;
	حرف buf[GIT_MAX_HEXSZ + 2]; /* hex hash + \n + \0 */

	show_usage_if_asked(argc, argv, pack_redundant_usage);

	برائے (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		اگر (!strcmp(arg, "--")) {
			i++;
			توڑو;
		}
		اگر (!strcmp(arg, "--all")) {
			بھرو_all_packs = 1;
			جاری;
		}
		اگر (!strcmp(arg, "--verbose")) {
			verbose = 1;
			جاری;
		}
		اگر (!strcmp(arg, "--alt-odb")) {
			alt_odb = 1;
			جاری;
		}
		اگر (!strcmp(arg, "--i-still-use-this")) {
			i_still_use_this = 1;
			جاری;
		}
		اگر (*arg == '-')
			usage(pack_redundant_usage);
		ورنہ
			توڑو;
	}

	اگر (!i_still_use_this)
		you_still_use_that("git pack-redundant", تہی);

	اگر (بھرو_all_packs)
		بھرو_all();
	ورنہ
		جب (*(argv + i) != تہی)
			جوڑو_pack_file(*(argv + i++));

	اگر (!local_packs)
		die("Zero packs found!");

	بھرو_all_objects();

	اگر (alt_odb)
		جائزہ_alt_odb_packs();

	/* ignore objects given on stdin */
	llist_init(&ignore);
	اگر (!isatty(0)) {
		ساخت object_id oid;
		جب (fgets(buf, حجم(buf), stdin)) {
			اگر (get_oid_hex(buf, &oid))
				die("Bad object ID on stdin: %s", buf);
			llist_داخل_کرو_sorted_unique(ignore, &oid, تہی);
		}
	}
	llist_sorted_difference_inplace(all_objects, ignore);
	pl = local_packs;
	جب (pl) {
		llist_sorted_difference_inplace(pl->بقایا_objects, ignore);
		pl = pl->اگلا;
	}

	cmp_local_packs();

	minimize(&min);

	اگر (verbose) {
		fprintf(stderr, "There are %lu packs available in alt-odbs.\n",
			(غیرعلامتی طویل)pack_list_سائز(altodb_packs));
		fprintf(stderr, "The smallest (bytewise) set of packs is:\n");
		pl = min;
		جب (pl) {
			fprintf(stderr, "\t%s\n", pl->pack->pack_name);
			pl = pl->اگلا;
		}
		fprintf(stderr, "containing %lu duplicate objects "
				"with a total size of %lukb.\n",
			(غیرعلامتی طویل)get_pack_redundancy(min),
			(غیرعلامتی طویل)pack_سیٹ_bytecount(min)/1024);
		fprintf(stderr, "A total of %lu unique objects were considered.\n",
			(غیرعلامتی طویل)all_objects->سائز);
		fprintf(stderr, "Redundant packs (with indexes):\n");
	}
	pl = red = pack_list_difference(local_packs, min);
	جب (pl) {
		لکھو("%s\n%s\n",
		       odb_pack_name(pl->pack->repo, &idx_name, pl->pack->hash, "idx"),
		       pl->pack->pack_name);
		pl = pl->اگلا;
	}
	اگر (verbose)
		fprintf(stderr, "%luMB of redundant packs in total.\n",
			(غیرعلامتی طویل)pack_سیٹ_bytecount(red)/(1024*1024));

	pack_list_free(red);
	pack_list_free(min);
	llist_free(ignore);
	strbuf_release(&idx_name);
	واپس 0;
}
