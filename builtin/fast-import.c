#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "config.h"
#ضمن "lockfile.h"
#ضمن "object.h"
#ضمن "blob.h"
#ضمن "tree.h"
#ضمن "commit.h"
#ضمن "delta.h"
#ضمن "pack.h"
#ضمن "path.h"
#ضمن "read-cache-ll.h"
#ضمن "refs.h"
#ضمن "csum-file.h"
#ضمن "quote.h"
#ضمن "dir.h"
#ضمن "run-command.h"
#ضمن "packfile.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "mem-pool.h"
#ضمن "commit-reach.h"
#ضمن "khash.h"
#ضمن "date.h"
#ضمن "gpg-interface.h"
#ضمن "parse-options.h"

#تعریف_کرو PACK_ID_بٹ 16
#تعریف_کرو MAX_PACK_ID ((1<<PACK_ID_بٹ)-1)
#تعریف_کرو ڈونگھائی_بٹ 13
#تعریف_کرو MAX_ڈونگھائی ((1<<ڈونگھائی_بٹ)-1)

/*
 * We abuse the setuid bit on directories to mean "do not delta".
 */
#تعریف_کرو NO_DELTA S_ISUID

/*
 * The amount of additional space required in order to write an object into the
 * current pack. This is the hash lengths at the end of the pack, plus the
 * length of one object ID.
 */
#تعریف_کرو PACK_ناپ_حد (the_hash_algo->rawsz * 3)

بنية object_اندراج {
	بنية pack_idx_اندراج idx;
	بنية hashmap_اندراج ent;
	uint32_t type : TYPE_بٹ,
		pack_id : PACK_ID_بٹ,
		ڈونگھائی : ڈونگھائی_بٹ;
};

ساكن صحيح object_اندراج_hashcmp(ثابت فراغ *map_انکڑے بن_ورتیا,
				ثابت بنية hashmap_اندراج *eptr,
				ثابت بنية hashmap_اندراج *اندراج_or_کلید,
				ثابت فراغ *keydata)
{
	ثابت بنية object_id *oid = keydata;
	ثابت بنية object_اندراج *e1, *e2;

	e1 = container_دا(eptr, ثابت بنية object_اندراج, ent);
	إذا (oid)
		أرجع oidcmp(&e1->idx.oid, oid);

	e2 = container_دا(اندراج_or_کلید, ثابت بنية object_اندراج, ent);
	أرجع oidcmp(&e1->idx.oid, &e2->idx.oid);
}

بنية object_اندراج_pool {
	بنية object_اندراج_pool *اگلا_pool;
	بنية object_اندراج *اگلا_free;
	بنية object_اندراج *سرا;
	بنية object_اندراج entries[FLEX_ARRAY]; /*ہور*/
};

بنية mark_سیٹ {
	اتحاد {
		بنية object_id *oids[1024];
		بنية object_اندراج *marked[1024];
		بنية mark_سیٹ *sets[1024];
	} انکڑے;
	بنانشانی صحيح کھسکاؤ;
};

بنية آخری_object {
	بنية strbuf انکڑے;
	off_t وتھ;
	بنانشانی صحيح ڈونگھائی;
	بنانشانی no_وٹاندرا : 1;
};

بنية atom_str {
	بنية atom_str *اگلا_atom;
	بنانشانی چھوٹا str_len;
	حرف str_dat[FLEX_ARRAY]; /*ہور*/
};

بنية درخت_content;
بنية درخت_اندراج {
	بنية درخت_content *درخت;
	بنية atom_str *name;
	بنية درخت_اندراج_ms {
		uint16_t منوال;
		بنية object_id oid;
	} versions[2];
};

بنية درخت_content {
	بنانشانی صحيح اندراج_سمرتھا; /* must match avail_tree_content */
	بنانشانی صحيح اندراج_گنتی;
	بنانشانی صحيح delta_ڈونگھائی;
	بنية درخت_اندراج *entries[FLEX_ARRAY]; /*ہور*/
};

بنية avail_درخت_content {
	بنانشانی صحيح اندراج_سمرتھا; /* must match tree_content */
	بنية avail_درخت_content *اگلا_avail;
};

بنية branch {
	بنية branch *جدول_اگلا_branch;
	بنية branch *سرگرم_اگلا_branch;
	ثابت حرف *name;
	بنية درخت_اندراج branch_درخت;
	uintmax_t آخری_commit;
	uintmax_t عدد_notes;
	بنانشانی سرگرم : 1;
	بنانشانی delete : 1;
	بنانشانی pack_id : PACK_ID_بٹ;
	بنية object_id oid;
};

بنية ٹیگ {
	بنية ٹیگ *اگلا_ٹیگ;
	ثابت حرف *name;
	بنانشانی صحيح pack_id;
	بنية object_id oid;
};

بنية hash_list {
	بنية hash_list *اگلا;
	بنية object_id oid;
};

تعريفنوع تعداد {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

بنية recent_command {
	بنية recent_command *prev;
	بنية recent_command *اگلا;
	حرف *buf;
};

تعريفنوع فراغ (*mark_سیٹ_inserter_t)(بنية mark_سیٹ **s, بنية object_id *oid, uintmax_t mark);
تعريفنوع فراغ (*each_mark_fn_t)(uintmax_t mark, فراغ *obj, فراغ *cbp);

/* Configured limits on output */
ساكن بنانشانی لما max_ڈونگھائی = 50;
ساكن off_t max_packsize;
ساكن صحيح unpack_حد_بندی = 100;
ساكن صحيح force_update;

/* Stats and misc. counters */
ساكن uintmax_t alloc_گنتی;
ساكن uintmax_t marks_سیٹ_گنتی;
ساكن uintmax_t object_گنتی_by_type[1 << TYPE_بٹ];
ساكن uintmax_t duplicate_گنتی_by_type[1 << TYPE_بٹ];
ساكن uintmax_t delta_گنتی_by_type[1 << TYPE_بٹ];
ساكن uintmax_t delta_گنتی_attempts_by_type[1 << TYPE_بٹ];
ساكن بنانشانی لما object_گنتی;
ساكن بنانشانی لما branch_گنتی;
ساكن بنانشانی لما branch_لدو_گنتی;
ساكن صحيح failure;
ساكن FILE *pack_کنارے;
ساكن صحيح show_stats = 1;
ساكن صحيح quiet;
ساكن تعداد sign_منوال signed_ٹیگ_منوال = SIGN_VERBATIM;
ساكن تعداد sign_منوال signed_commit_منوال = SIGN_VERBATIM;
ساكن ثابت حرف *signed_commit_keyid;
ساكن ثابت حرف *signed_ٹیگ_keyid;

/* Memory pools */
ساكن بنية mem_pool fi_mem_pool = {
	.بلاک_alloc = 2*1024*1024 - حجم(بنية mp_بلاک),
};

/* Atom management */
ساكن بنانشانی صحيح atom_جدول_sz = 4451;
ساكن بنانشانی صحيح atom_cnt;
ساكن بنية atom_str **atom_جدول;

/* The .pack file being generated */
ساكن بنية pack_idx_option pack_idx_opts;
ساكن بنانشانی صحيح pack_id;
ساكن بنية hashfile *pack_file;
ساكن بنية packed_git *pack_انکڑے;
ساكن بنية packed_git **all_packs;
ساكن off_t pack_ناپ;

/* Table of objects we've written. */
ساكن بنانشانی صحيح object_اندراج_alloc = 5000;
ساكن بنية object_اندراج_pool *بلاکاں;
ساكن بنية hashmap object_جدول;
ساكن بنية mark_سیٹ *marks;
ساكن حرف *export_marks_file;
ساكن حرف *import_marks_file;
ساكن صحيح import_marks_file_from_دھارا;
ساكن صحيح import_marks_file_ignore_missing;
ساكن صحيح import_marks_file_پورا;
ساكن صحيح relative_marks_paths;

/* Our last blob */
ساكن بنية آخری_object آخری_blob = {
	.انکڑے = STRBUF_INIT,
 };

/* Tree management */
ساكن بنانشانی صحيح درخت_اندراج_alloc = 1000;
ساكن فراغ *avail_درخت_اندراج;
ساكن بنانشانی صحيح avail_درخت_جدول_sz = 100;
ساكن بنية avail_درخت_content **avail_درخت_جدول;
ساكن ناپ_t درخت_اندراج_allocd;
ساكن بنية strbuf old_درخت = STRBUF_INIT;
ساكن بنية strbuf new_درخت = STRBUF_INIT;

/* Branch data */
ساكن بنانشانی لما max_سرگرم_branches = 5;
ساكن بنانشانی لما cur_سرگرم_branches;
ساكن بنانشانی لما branch_جدول_sz = 1039;
ساكن بنية branch **branch_جدول;
ساكن بنية branch *سرگرم_branches;

/* Tag data */
ساكن بنية ٹیگ *پہلا_ٹیگ;
ساكن بنية ٹیگ *آخری_ٹیگ;

/* Input stream parsing */
ساكن whenspec_type whenspec = WHENSPEC_RAW;
ساكن بنية strbuf command_buf = STRBUF_INIT;
ساكن صحيح unread_command_buf;
ساكن بنية recent_command cmd_hist = {
	.prev = &cmd_hist,
	.اگلا = &cmd_hist,
};
ساكن بنية recent_command *cmd_پوچھ = &cmd_hist;
ساكن بنية recent_command *rc_free;
ساكن بنانشانی صحيح cmd_save = 100;
ساكن uintmax_t اگلا_mark;
ساكن بنية strbuf new_انکڑے = STRBUF_INIT;
ساكن صحيح require_explicit_termination;

/* Signal handling */
ساكن متطاير sig_atomic_t checkpoint_requested;

/* Submodule marks */
ساكن بنية string_list sub_marks_from = STRING_LIST_INIT_DUP;
ساكن بنية string_list sub_marks_منزل = STRING_LIST_INIT_DUP;
ساكن kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
ساكن صحيح cat_blob_fd = STDOUT_FILENO;

/* Command state */
بنية fast_import_state {
	صحيح argc;
	ثابت حرف **argv;
	ثابت حرف *اگیتر;
	صحيح تکیا_انکڑے_command;
	صحيح allow_unsafe_features;
	بنية option *option;
};

ساكن فراغ fast_import_state_init(بنية fast_import_state *state,
				   صحيح argc, ثابت حرف **argv,
				   ثابت حرف *اگیتر, بنية option *option)
{
	memset(state, 0, حجم(*state));
	state->argc = argc;
	state->argv = argv;
	state->اگیتر = اگیتر;
	state->option = option;
}

ساكن فراغ parse_argv(بنية fast_import_state *state);
ساكن فراغ parse_get_mark(بنية fast_import_state *state, ثابت حرف *p);
ساكن فراغ parse_cat_blob(بنية fast_import_state *state, ثابت حرف *p);
ساكن فراغ parse_ls(بنية fast_import_state *state,
		     ثابت حرف *p,
		     بنية branch *b);

ساكن فراغ for_each_mark(بنية mark_سیٹ *m, uintmax_t ادھار, each_mark_fn_t callback, فراغ *p)
{
	uintmax_t k;
	إذا (m->کھسکاؤ) {
		لكل (k = 0; k < 1024; k++) {
			إذا (m->انکڑے.sets[k])
				for_each_mark(m->انکڑے.sets[k], ادھار + (k << m->کھسکاؤ), callback, p);
		}
	} وإلا {
		لكل (k = 0; k < 1024; k++) {
			إذا (m->انکڑے.marked[k])
				callback(ادھار + k, m->انکڑے.marked[k], p);
		}
	}
}

ساكن فراغ dump_marks_fn(uintmax_t mark, فراغ *object, فراغ *cbp) {
	بنية object_اندراج *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_منزل_hex(&e->idx.oid));
}

ساكن فراغ write_branch_report(FILE *rpt, بنية branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	إذا (b->سرگرم)
		fputs(" active", rpt);
	إذا (b->branch_درخت.درخت)
		fputs(" loaded", rpt);
	إذا (is_null_oid(&b->branch_درخت.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_منزل_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_منزل_hex(&b->branch_درخت.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_منزل_hex(&b->branch_درخت.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->آخری_commit);

	fputs("  last pack   : ", rpt);
	إذا (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

ساكن فراغ write_crash_report(ثابت حرف *err)
{
	حرف *loc = repo_git_رستہ(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	بنية branch *b;
	بنانشانی لما lu;
	بنية recent_command *rc;

	إذا (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		حرر(loc);
		أرجع;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(ویلا(عدم), 0, DATE_منوال(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	لكل (rc = cmd_hist.اگلا; rc != &cmd_hist; rc = rc->اگلا) {
		إذا (rc->اگلا == &cmd_hist)
			fputs("* ", rpt);
		وإلا
			fputs("  ", rpt);
		fputs(rc->buf, rpt);
		fputc('\n', rpt);
	}

	fputc('\n', rpt);
	fputs("Active Branch LRU\n", rpt);
	fputs("-----------------\n", rpt);
	fprintf(rpt, "    active_branches = %lu cur, %lu max\n",
		cur_سرگرم_branches,
		max_سرگرم_branches);
	fputc('\n', rpt);
	fputs("  pos  clock name\n", rpt);
	fputs("  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", rpt);
	لكل (b = سرگرم_branches, lu = 0; b; b = b->سرگرم_اگلا_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->آخری_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	لكل (lu = 0; lu < branch_جدول_sz; lu++) {
		لكل (b = branch_جدول[lu]; b; b = b->جدول_اگلا_branch)
			write_branch_report(rpt, b);
	}

	إذا (پہلا_ٹیگ) {
		بنية ٹیگ *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		لكل (tg = پہلا_ٹیگ; tg; tg = tg->اگلا_ٹیگ) {
			fputs(oid_منزل_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	إذا (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	وإلا
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	حرر(loc);
}

ساكن فراغ سرا_packfile(فراغ);
ساكن فراغ unkeep_all_packs(فراغ);
ساكن فراغ dump_marks(فراغ);

ساكن NORETURN فراغ die_nicely(ثابت حرف *err, va_list params)
{
	va_list cp;
	ساكن صحيح zombie;
	report_fn die_سنیہا_fn = get_die_سنیہا_routine();

	va_copy(cp, params);
	die_سنیہا_fn(err, params);

	إذا (!zombie) {
		حرف سنیہا[2 * رستہ_MAX];

		zombie = 1;
		vsnprintf(سنیہا, حجم(سنیہا), err, cp);
		write_crash_report(سنیہا);
		سرا_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	باہر_نکلو(128);
}

#جے_بنا_تعریف SIGUSR1	/* Windows, for example */

ساكن فراغ سیٹ_checkpoint_signal(فراغ)
{
}

#وإلا

ساكن فراغ checkpoint_signal(صحيح signo بن_ورتیا)
{
	checkpoint_requested = 1;
}

ساكن فراغ سیٹ_checkpoint_signal(فراغ)
{
	بنية sigaction sa;

	memset(&sa, 0, حجم(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_نقاب);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, عدم);
}

#جے_انت

ساكن فراغ alloc_objects(بنانشانی صحيح cnt)
{
	بنية object_اندراج_pool *b;

	b = xmalloc(حجم(بنية object_اندراج_pool)
		+ cnt * حجم(بنية object_اندراج));
	b->اگلا_pool = بلاکاں;
	b->اگلا_free = b->entries;
	b->سرا = b->entries + cnt;
	بلاکاں = b;
	alloc_گنتی += cnt;
}

ساكن بنية object_اندراج *new_object(بنية object_id *oid)
{
	بنية object_اندراج *e;

	إذا (بلاکاں->اگلا_free == بلاکاں->سرا)
		alloc_objects(object_اندراج_alloc);

	e = بلاکاں->اگلا_free++;
	oidcpy(&e->idx.oid, oid);
	أرجع e;
}

ساكن بنية object_اندراج *لبھو_object(بنية object_id *oid)
{
	أرجع hashmap_get_اندراج_from_hash(&object_جدول, oidhash(oid), oid,
					   بنية object_اندراج, ent);
}

ساكن بنية object_اندراج *داخل_کرو_object(بنية object_id *oid)
{
	بنية object_اندراج *e;
	بنانشانی صحيح hash = oidhash(oid);

	e = hashmap_get_اندراج_from_hash(&object_جدول, hash, oid,
					بنية object_اندراج, ent);
	إذا (!e) {
		e = new_object(oid);
		e->idx.وتھ = 0;
		hashmap_اندراج_init(&e->ent, hash);
		hashmap_جوڑو(&object_جدول, &e->ent);
	}

	أرجع e;
}

ساكن فراغ invalidate_pack_id(بنانشانی صحيح id)
{
	بنانشانی لما lu;
	بنية ٹیگ *t;
	بنية hashmap_iter iter;
	بنية object_اندراج *e;

	hashmap_for_each_اندراج(&object_جدول, &iter, e, ent) {
		إذا (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	لكل (lu = 0; lu < branch_جدول_sz; lu++) {
		بنية branch *b;

		لكل (b = branch_جدول[lu]; b; b = b->جدول_اگلا_branch)
			إذا (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	لكل (t = پہلا_ٹیگ; t; t = t->اگلا_ٹیگ)
		إذا (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

ساكن بنانشانی صحيح hc_str(ثابت حرف *s, ناپ_t len)
{
	بنانشانی صحيح r = 0;
	طالما (len-- > 0)
		r = r * 31 + *s++;
	أرجع r;
}

ساكن فراغ داخل_کرو_mark(بنية mark_سیٹ **چوٹی, uintmax_t idnum, بنية object_اندراج *oe)
{
	بنية mark_سیٹ *s = *چوٹی;

	طالما ((idnum >> s->کھسکاؤ) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_سیٹ));
		s->کھسکاؤ = (*چوٹی)->کھسکاؤ + 10;
		s->انکڑے.sets[0] = *چوٹی;
		*چوٹی = s;
	}
	طالما (s->کھسکاؤ) {
		uintmax_t i = idnum >> s->کھسکاؤ;
		idnum -= i << s->کھسکاؤ;
		إذا (!s->انکڑے.sets[i]) {
			s->انکڑے.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_سیٹ));
			s->انکڑے.sets[i]->کھسکاؤ = s->کھسکاؤ - 10;
		}
		s = s->انکڑے.sets[i];
	}
	إذا (!s->انکڑے.marked[idnum])
		marks_سیٹ_گنتی++;
	s->انکڑے.marked[idnum] = oe;
}

ساكن فراغ *لبھو_mark(بنية mark_سیٹ *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	بنية object_اندراج *oe = عدم;
	إذا ((idnum >> s->کھسکاؤ) < 1024) {
		طالما (s && s->کھسکاؤ) {
			uintmax_t i = idnum >> s->کھسکاؤ;
			idnum -= i << s->کھسکاؤ;
			s = s->انکڑے.sets[i];
		}
		إذا (s)
			oe = s->انکڑے.marked[idnum];
	}
	إذا (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	أرجع oe;
}

ساكن بنية atom_str *منزل_atom(ثابت حرف *s, بنانشانی چھوٹا len)
{
	بنانشانی صحيح hc = hc_str(s, len) % atom_جدول_sz;
	بنية atom_str *c;

	لكل (c = atom_جدول[hc]; c; c = c->اگلا_atom)
		إذا (c->str_len == len && !لڑی_مقابلہ(s, c->str_dat, len))
			أرجع c;

	c = mem_pool_alloc(&fi_mem_pool, حجم(بنية atom_str) + len + 1);
	c->str_len = len;
	حافظہ_نقل(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->اگلا_atom = atom_جدول[hc];
	atom_جدول[hc] = c;
	atom_cnt++;
	أرجع c;
}

ساكن بنية branch *lookup_branch(ثابت حرف *name)
{
	بنانشانی صحيح hc = hc_str(name, لڑی_لمبائی(name)) % branch_جدول_sz;
	بنية branch *b;

	لكل (b = branch_جدول[hc]; b; b = b->جدول_اگلا_branch)
		إذا (!strcmp(name, b->name))
			أرجع b;
	أرجع عدم;
}

ساكن بنية branch *new_branch(ثابت حرف *name)
{
	بنانشانی صحيح hc = hc_str(name, لڑی_لمبائی(name)) % branch_جدول_sz;
	بنية branch *b = lookup_branch(name);

	إذا (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	إذا (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->جدول_اگلا_branch = branch_جدول[hc];
	b->branch_درخت.versions[0].منوال = S_IFDIR;
	b->branch_درخت.versions[1].منوال = S_IFDIR;
	b->عدد_notes = 0;
	b->سرگرم = 0;
	b->pack_id = MAX_PACK_ID;
	branch_جدول[hc] = b;
	branch_گنتی++;
	أرجع b;
}

ساكن بنانشانی صحيح hc_entries(بنانشانی صحيح cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	أرجع cnt < avail_درخت_جدول_sz ? cnt : avail_درخت_جدول_sz - 1;
}

ساكن بنية درخت_content *new_درخت_content(بنانشانی صحيح cnt)
{
	بنية avail_درخت_content *f, *l = عدم;
	بنية درخت_content *t;
	بنانشانی صحيح hc = hc_entries(cnt);

	لكل (f = avail_درخت_جدول[hc]; f; l = f, f = f->اگلا_avail)
		إذا (f->اندراج_سمرتھا >= cnt)
			اكسر;

	إذا (f) {
		إذا (l)
			l->اگلا_avail = f->اگلا_avail;
		وإلا
			avail_درخت_جدول[hc] = f->اگلا_avail;
	} وإلا {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, حجم(*t) + حجم(t->entries[0]) * cnt);
		f->اندراج_سمرتھا = cnt;
	}

	t = (بنية درخت_content*)f;
	t->اندراج_گنتی = 0;
	t->delta_ڈونگھائی = 0;
	أرجع t;
}

ساكن فراغ release_درخت_اندراج(بنية درخت_اندراج *e);
ساكن فراغ release_درخت_content(بنية درخت_content *t)
{
	بنية avail_درخت_content *f = (بنية avail_درخت_content*)t;
	بنانشانی صحيح hc = hc_entries(f->اندراج_سمرتھا);
	f->اگلا_avail = avail_درخت_جدول[hc];
	avail_درخت_جدول[hc] = f;
}

ساكن فراغ release_درخت_content_recursive(بنية درخت_content *t)
{
	بنانشانی صحيح i;
	لكل (i = 0; i < t->اندراج_گنتی; i++)
		release_درخت_اندراج(t->entries[i]);
	release_درخت_content(t);
}

ساكن بنية درخت_content *grow_درخت_content(
	بنية درخت_content *t,
	صحيح amt)
{
	بنية درخت_content *r = new_درخت_content(t->اندراج_گنتی + amt);
	r->اندراج_گنتی = t->اندراج_گنتی;
	r->delta_ڈونگھائی = t->delta_ڈونگھائی;
	COPY_ARRAY(r->entries, t->entries, t->اندراج_گنتی);
	release_درخت_content(t);
	أرجع r;
}

ساكن بنية درخت_اندراج *new_درخت_اندراج(فراغ)
{
	بنية درخت_اندراج *e;

	إذا (!avail_درخت_اندراج) {
		بنانشانی صحيح n = درخت_اندراج_alloc;
		درخت_اندراج_allocd += n * حجم(بنية درخت_اندراج);
		ALLOC_ARRAY(e, n);
		avail_درخت_اندراج = e;
		طالما (n-- > 1) {
			*((فراغ**)e) = e + 1;
			e++;
		}
		*((فراغ**)e) = عدم;
	}

	e = avail_درخت_اندراج;
	avail_درخت_اندراج = *((فراغ**)e);
	أرجع e;
}

ساكن فراغ release_درخت_اندراج(بنية درخت_اندراج *e)
{
	إذا (e->درخت)
		release_درخت_content_recursive(e->درخت);
	*((فراغ**)e) = avail_درخت_اندراج;
	avail_درخت_اندراج = e;
}

ساكن بنية درخت_content *dup_درخت_content(بنية درخت_content *s)
{
	بنية درخت_content *d;
	بنية درخت_اندراج *a, *b;
	بنانشانی صحيح i;

	إذا (!s)
		أرجع عدم;
	d = new_درخت_content(s->اندراج_گنتی);
	لكل (i = 0; i < s->اندراج_گنتی; i++) {
		a = s->entries[i];
		b = new_درخت_اندراج();
		حافظہ_نقل(b, a, حجم(*a));
		إذا (a->درخت && is_null_oid(&b->versions[1].oid))
			b->درخت = dup_درخت_content(a->درخت);
		وإلا
			b->درخت = عدم;
		d->entries[i] = b;
	}
	d->اندراج_گنتی = s->اندراج_گنتی;
	d->delta_ڈونگھائی = s->delta_ڈونگھائی;

	أرجع d;
}

ساكن فراغ شروع_packfile(فراغ)
{
	بنية strbuf tmp_file = STRBUF_INIT;
	بنية packed_git *p;
	صحيح pack_fd;

	pack_fd = odb_mkstemp(the_repository->objects, &tmp_file,
			      "pack/tmp_pack_XXXXXX");
	FLEX_ALLOC_STR(p, pack_name, tmp_file.buf);
	strbuf_release(&tmp_file);

	p->pack_fd = pack_fd;
	p->do_not_close = 1;
	p->repo = the_repository;
	pack_file = hashfd(the_repository->hash_algo, pack_fd, p->pack_name);

	pack_انکڑے = p;
	pack_ناپ = write_pack_سرخی(pack_file, 0);
	object_گنتی = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

ساكن ثابت حرف *create_اشاریہ(فراغ)
{
	ثابت حرف *tmpfile;
	بنية pack_idx_اندراج **idx, **c, **آخری;
	بنية object_اندراج *e;
	بنية object_اندراج_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_گنتی);
	c = idx;
	لكل (o = بلاکاں; o; o = o->اگلا_pool)
		لكل (e = o->اگلا_free; e-- != o->entries;)
			إذا (pack_id == e->pack_id)
				*c++ = &e->idx;
	آخری = idx + object_گنتی;
	إذا (c != آخری)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, عدم, idx, object_گنتی,
				 &pack_idx_opts, pack_انکڑے->hash);
	حرر(idx);
	أرجع tmpfile;
}

ساكن حرف *keep_pack(ثابت حرف *curr_اشاریہ_name)
{
	ساكن ثابت حرف *keep_پیغام = "fast-import";
	بنية strbuf name = STRBUF_INIT;
	صحيح keep_fd;

	odb_pack_name(pack_انکڑے->repo, &name, pack_انکڑے->hash, "keep");
	keep_fd = محفوظ_create_file_with_leading_directories(pack_انکڑے->repo,
							    name.buf);
	إذا (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_پیغام, لڑی_لمبائی(keep_پیغام));
	إذا (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_انکڑے->repo, &name, pack_انکڑے->hash, "pack");
	إذا (finalize_object_file(pack_انکڑے->repo, pack_انکڑے->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_انکڑے->repo, &name, pack_انکڑے->hash, "idx");
	إذا (finalize_object_file(pack_انکڑے->repo, curr_اشاریہ_name, name.buf))
		die(_("cannot store index file"));
	حرر((فراغ *)curr_اشاریہ_name);
	أرجع strbuf_detach(&name, عدم);
}

ساكن فراغ unkeep_all_packs(فراغ)
{
	بنية strbuf name = STRBUF_INIT;
	صحيح k;

	لكل (k = 0; k < pack_id; k++) {
		بنية packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

ساكن صحيح loosen_small_pack(ثابت بنية packed_git *p)
{
	بنية بچہ_process unpack = بچہ_PROCESS_INIT;

	إذا (lseek(p->pack_fd, 0, SEEK_سیٹ) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.وچ = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_منزل_stderr = 1;
	strvec_ڈالو(&unpack.دلیلاں, "unpack-objects");
	إذا (!show_stats)
		strvec_ڈالو(&unpack.دلیلاں, "-q");

	أرجع لگاتار_command(&unpack);
}

ساكن فراغ سرا_packfile(فراغ)
{
	ساكن صحيح running;

	إذا (running || !pack_انکڑے)
		أرجع;

	running = 1;
	clear_delta_ادھار_cache();
	إذا (object_گنتی) {
		بنية odb_سروت_files *files = odb_سروت_files_downcast(pack_انکڑے->repo->objects->سروت);
		بنية packed_git *new_p;
		بنية object_id cur_pack_oid;
		حرف *idx_name;
		صحيح i;
		بنية branch *b;
		بنية ٹیگ *t;

		close_pack_windows(pack_انکڑے);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_سرخی_footer(the_hash_algo, pack_انکڑے->pack_fd,
					 pack_انکڑے->hash, pack_انکڑے->pack_name,
					 object_گنتی, cur_pack_oid.hash,
					 pack_ناپ);

		إذا (object_گنتی <= unpack_حد_بندی) {
			إذا (!loosen_small_pack(pack_انکڑے)) {
				invalidate_pack_id(pack_id);
				اذهب discard_pack;
			}
		}

		close(pack_انکڑے->pack_fd);
		idx_name = keep_pack(create_اشاریہ());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_لدو_pack(files->dirs->packed, idx_name, 1);
		إذا (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		حرر(idx_name);

		/* Print the boundary */
		إذا (pack_کنارے) {
			fprintf(pack_کنارے, "%s:", new_p->pack_name);
			لكل (i = 0; i < branch_جدول_sz; i++) {
				لكل (b = branch_جدول[i]; b; b = b->جدول_اگلا_branch) {
					إذا (b->pack_id == pack_id)
						fprintf(pack_کنارے, " %s",
							oid_منزل_hex(&b->oid));
				}
			}
			لكل (t = پہلا_ٹیگ; t; t = t->اگلا_ٹیگ) {
				إذا (t->pack_id == pack_id)
					fprintf(pack_کنارے, " %s",
						oid_منزل_hex(&t->oid));
			}
			fputc('\n', pack_کنارے);
			fflush(pack_کنارے);
		}

		pack_id++;
	}
	وإلا {
discard_pack:
		close(pack_انکڑے->pack_fd);
		unlink_or_warn(pack_انکڑے->pack_name);
	}
	FREE_AND_NULL(pack_انکڑے);
	running = 0;

	/* We can't carry a delta across packfiles. */
	strbuf_release(&آخری_blob.انکڑے);
	آخری_blob.وتھ = 0;
	آخری_blob.ڈونگھائی = 0;
}

ساكن فراغ چکر_packfile(فراغ)
{
	سرا_packfile();
	شروع_packfile();
}

ساكن صحيح store_object(
	تعداد object_type type,
	بنية strbuf *dat,
	بنية آخری_object *آخری,
	بنية object_id *oidout,
	uintmax_t mark)
{
	بنية odb_سروت_files *files = odb_سروت_files_downcast(the_repository->objects->سروت);
	فراغ *باہرلا, *delta;
	بنية object_اندراج *e;
	بنانشانی حرف hdr[96];
	بنية object_id oid;
	بنانشانی لما hdrlen, deltalen = 0;
	بنية git_hash_ctx c;
	git_zstream s;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	hdrlen = format_object_سرخی((حرف *)hdr, حجم(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	إذا (oidout)
		oidcpy(oidout, &oid);

	e = داخل_کرو_object(&oid);
	إذا (mark)
		داخل_کرو_mark(&marks, mark, e);
	إذا (e->idx.وتھ) {
		duplicate_گنتی_by_type[type]++;
		أرجع 1;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
		إذا (!packfile_list_لبھو_oid(packfile_store_get_packs(dir->packed), &oid))
			استمر;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.وتھ = 1; /* just not zero! */
		duplicate_گنتی_by_type[type]++;
		أرجع 1;
	}

	إذا (آخری && آخری->انکڑے.len && آخری->انکڑے.buf && آخری->ڈونگھائی < max_ڈونگھائی
		&& dat->len > the_hash_algo->rawsz) {
		ناپ_t deltalen_st;

		delta_گنتی_attempts_by_type[type]++;
		delta = diff_delta(آخری->انکڑے.buf, آخری->انکڑے.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_ناپ_t_منزل_ulong(deltalen_st);
	} وإلا
		delta = عدم;

	git_deflate_init(&s, cfg->pack_compression_level);
	إذا (delta) {
		s.اگلا_وچ = delta;
		s.avail_وچ = deltalen;
	} وإلا {
		s.اگلا_وچ = (فراغ *)dat->buf;
		s.avail_وچ = dat->len;
	}
	s.avail_باہرلا = git_deflate_bound(&s, s.avail_وچ);
	s.اگلا_باہرلا = باہرلا = xmalloc(s.avail_باہرلا);
	طالما (git_deflate(&s, Z_انت) == Z_OK)
		; /* nothing */
	git_deflate_سرا(&s);

	/* Determine if we should auto-checkpoint. */
	إذا ((max_packsize
		&& (pack_ناپ + PACK_ناپ_حد + s.کل_باہرلا) > max_packsize)
		|| (pack_ناپ + PACK_ناپ_حد + s.کل_باہرلا) < pack_ناپ) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		چکر_packfile();

		/* We cannot carry a delta into the new pack. */
		إذا (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.اگلا_وچ = (فراغ *)dat->buf;
			s.avail_وچ = dat->len;
			s.avail_باہرلا = git_deflate_bound(&s, s.avail_وچ);
			s.اگلا_باہرلا = باہرلا = xrealloc(باہرلا, s.avail_باہرلا);
			طالما (git_deflate(&s, Z_انت) == Z_OK)
				; /* nothing */
			git_deflate_سرا(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.وتھ = pack_ناپ;
	object_گنتی++;
	object_گنتی_by_type[type]++;

	چکری_جانچ_32_begin(pack_file);

	إذا (delta) {
		off_t ofs = e->idx.وتھ - آخری->وتھ;
		بنانشانی pos = حجم(hdr) - 1;

		delta_گنتی_by_type[type]++;
		e->ڈونگھائی = آخری->ڈونگھائی + 1;

		hdrlen = encode_وچ_pack_object_سرخی(hdr, حجم(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_ناپ += hdrlen;

		hdr[pos] = ofs & 127;
		طالما (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, حجم(hdr) - pos);
		pack_ناپ += حجم(hdr) - pos;
	} وإلا {
		e->ڈونگھائی = 0;
		hdrlen = encode_وچ_pack_object_سرخی(hdr, حجم(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_ناپ += hdrlen;
	}

	hashwrite(pack_file, باہرلا, s.کل_باہرلا);
	pack_ناپ += s.کل_باہرلا;

	e->idx.چکری_جانچ_32 = چکری_جانچ_32_سرا(pack_file);

	حرر(باہرلا);
	حرر(delta);
	إذا (آخری) {
		إذا (آخری->no_وٹاندرا) {
			آخری->انکڑے = *dat;
		} وإلا {
			strbuf_وٹاندرا(&آخری->انکڑے, dat);
		}
		آخری->وتھ = e->idx.وتھ;
		آخری->ڈونگھائی = e->ڈونگھائی;
	}
	أرجع 0;
}

ساكن فراغ truncate_pack(بنية hashfile_checkpoint *checkpoint)
{
	إذا (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_ناپ = checkpoint->وتھ;
}

ساكن فراغ دھارا_blob(uintmax_t len, بنية object_id *oidout, uintmax_t mark)
{
	بنية odb_سروت_files *files = odb_سروت_files_downcast(the_repository->objects->سروت);
	ناپ_t وچ_sz = 64 * 1024, باہرلا_sz = 64 * 1024;
	بنانشانی حرف *وچ_buf = xmalloc(وچ_sz);
	بنانشانی حرف *باہرلا_buf = xmalloc(باہرلا_sz);
	بنية object_اندراج *e;
	بنية object_id oid;
	بنانشانی لما hdrlen;
	off_t وتھ;
	بنية git_hash_ctx c;
	git_zstream s;
	بنية hashfile_checkpoint checkpoint;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	صحيح حالت = Z_OK;

	/* Determine if we should auto-checkpoint. */
	إذا ((max_packsize
		&& (pack_ناپ + PACK_ناپ_حد + len) > max_packsize)
		|| (pack_ناپ + PACK_ناپ_حد + len) < pack_ناپ)
		چکر_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	وتھ = checkpoint.وتھ;

	hdrlen = format_object_سرخی((حرف *)باہرلا_buf, باہرلا_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, باہرلا_buf, hdrlen);

	چکری_جانچ_32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_وچ_pack_object_سرخی(باہرلا_buf, باہرلا_sz, OBJ_BLOB, len);

	s.اگلا_باہرلا = باہرلا_buf + hdrlen;
	s.avail_باہرلا = باہرلا_sz - hdrlen;

	طالما (حالت != Z_دھارا_سرا) {
		إذا (0 < len && !s.avail_وچ) {
			ناپ_t cnt = وچ_sz < len ? وچ_sz : (ناپ_t)len;
			ناپ_t n = fread(وچ_buf, 1, cnt, stdin);
			إذا (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, وچ_buf, n);
			s.اگلا_وچ = وچ_buf;
			s.avail_وچ = n;
			len -= n;
		}

		حالت = git_deflate(&s, len ? 0 : Z_انت);

		إذا (!s.avail_باہرلا || حالت == Z_دھارا_سرا) {
			ناپ_t n = s.اگلا_باہرلا - باہرلا_buf;
			hashwrite(pack_file, باہرلا_buf, n);
			pack_ناپ += n;
			s.اگلا_باہرلا = باہرلا_buf;
			s.avail_باہرلا = باہرلا_sz;
		}

		بدل (حالت) {
		حالة Z_OK:
		حالة Z_BUF_ERROR:
		حالة Z_دھارا_سرا:
			استمر;
		افتراضي:
			die(_("unexpected deflate failure: %d"), حالت);
		}
	}
	git_deflate_سرا(&s);
	git_hash_final_oid(&oid, &c);

	إذا (oidout)
		oidcpy(oidout, &oid);

	e = داخل_کرو_object(&oid);

	إذا (mark)
		داخل_کرو_mark(&marks, mark, e);

	إذا (e->idx.وتھ) {
		duplicate_گنتی_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		اذهب باہرلا;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
		إذا (!packfile_list_لبھو_oid(packfile_store_get_packs(dir->packed), &oid))
			استمر;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.وتھ = 1; /* just not zero! */
		duplicate_گنتی_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		اذهب باہرلا;
	}

	e->ڈونگھائی = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.وتھ = وتھ;
	e->idx.چکری_جانچ_32 = چکری_جانچ_32_سرا(pack_file);
	object_گنتی++;
	object_گنتی_by_type[OBJ_BLOB]++;

باہرلا:
	حرر(وچ_buf);
	حرر(باہرلا_buf);
	hashfile_checkpoint_release(&checkpoint);
}

/* All calls must be guarded by find_object() or find_mark() to
 * ensure the 'struct object_entry' passed was written by this
 * process instance.  We unpack the entry by the offset, avoiding
 * the need for the corresponding .idx file.  This unpacking rule
 * works because we only use OBJ_REF_DELTA within the packfiles
 * created by fast-import.
 *
 * oe must not be NULL.  Such an oe usually comes from giving
 * an unknown SHA-1 to find_object() or an undefined mark to
 * find_mark().  Callers must test for this condition and use
 * the standard read_sha1_file() when it happens.
 *
 * oe->pack_id must not be MAX_PACK_ID.  Such an oe is usually from
 * find_mark(), where the mark was reloaded from an existing marks
 * file and is referencing an object that this fast-import process
 * instance did not write out to a packfile.  Callers must test for
 * this condition and use read_sha1_file() instead.
 */
ساكن فراغ *gfi_unpack_اندراج(
	بنية object_اندراج *oe,
	بنانشانی لما *sizep)
{
	تعداد object_type type;
	ناپ_t ناپ_st = 0;
	فراغ *انکڑے;
	بنية packed_git *p = all_packs[oe->pack_id];
	إذا (p == pack_انکڑے && p->pack_ناپ < (pack_ناپ + the_hash_algo->rawsz)) {
		/* The object is stored in the packfile we are writing to
		 * and we have modified it since the last time we scanned
		 * back to read a previously written object.  If an old
		 * window covered [p->pack_size, p->pack_size + rawsz) its
		 * data is stale and is not valid.  Closing all windows
		 * and updating the packfile length ensures we can read
		 * the newly written data.
		 */
		close_pack_windows(p);
		hashflush(pack_file);

		/* We have to offer rawsz bytes additional on the end of
		 * the packfile as the core unpacker code assumes the
		 * footer is present at the file end and must promise
		 * at least rawsz bytes within any window it maps.  But
		 * we don't actually create the footer here.
		 */
		p->pack_ناپ = pack_ناپ + the_hash_algo->rawsz;
	}
	انکڑے = unpack_اندراج(the_repository, p, oe->idx.وتھ, &type, &ناپ_st);
	إذا (sizep)
		*sizep = cast_ناپ_t_منزل_ulong(ناپ_st);
	أرجع انکڑے;
}

ساكن فراغ لدو_درخت(بنية درخت_اندراج *جڑ)
{
	بنية object_id *oid = &جڑ->versions[1].oid;
	بنية object_اندراج *myoe;
	بنية درخت_content *t;
	بنانشانی لما ناپ;
	حرف *buf;
	ثابت حرف *c;

	جڑ->درخت = t = new_درخت_content(8);
	إذا (is_null_oid(oid))
		أرجع;

	myoe = لبھو_object(oid);
	إذا (myoe && myoe->pack_id != MAX_PACK_ID) {
		إذا (myoe->type != OBJ_درخت)
			die(_("not a tree: %s"), oid_منزل_hex(oid));
		t->delta_ڈونگھائی = myoe->ڈونگھائی;
		buf = gfi_unpack_اندراج(myoe, &ناپ);
		إذا (!buf)
			die(_("can't load tree %s"), oid_منزل_hex(oid));
	} وإلا {
		تعداد object_type type;
		ناپ_t ناپ_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &ناپ_st);
		ناپ = cast_ناپ_t_منزل_ulong(ناپ_st);
		إذا (!buf || type != OBJ_درخت)
			die(_("can't load tree %s"), oid_منزل_hex(oid));
	}

	c = buf;
	طالما (c != (buf + ناپ)) {
		بنية درخت_اندراج *e = new_درخت_اندراج();

		إذا (t->اندراج_گنتی == t->اندراج_سمرتھا)
			جڑ->درخت = t = grow_درخت_content(t, t->اندراج_گنتی);
		t->entries[t->اندراج_گنتی++] = e;

		e->درخت = عدم;
		c = parse_منوال(c, &e->versions[1].منوال);
		إذا (!c)
			die(_("corrupt mode in %s"), oid_منزل_hex(oid));
		e->versions[0].منوال = e->versions[1].منوال;
		e->name = منزل_atom(c, لڑی_لمبائی(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (بنانشانی حرف *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (بنانشانی حرف *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	حرر(buf);
}

ساكن صحيح tecmp0 (ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية درخت_اندراج *a = *((بنية درخت_اندراج**)_a);
	بنية درخت_اندراج *b = *((بنية درخت_اندراج**)_b);
	أرجع ادھار_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].منوال,
		b->name->str_dat, b->name->str_len, b->versions[0].منوال);
}

ساكن صحيح tecmp1 (ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية درخت_اندراج *a = *((بنية درخت_اندراج**)_a);
	بنية درخت_اندراج *b = *((بنية درخت_اندراج**)_b);
	أرجع ادھار_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].منوال,
		b->name->str_dat, b->name->str_len, b->versions[1].منوال);
}

ساكن فراغ mktree(بنية درخت_content *t, صحيح v, بنية strbuf *b)
{
	ناپ_t maxlen = 0;
	بنانشانی صحيح i;

	إذا (!v)
		QSORT(t->entries, t->اندراج_گنتی, tecmp0);
	وإلا
		QSORT(t->entries, t->اندراج_گنتی, tecmp1);

	لكل (i = 0; i < t->اندراج_گنتی; i++) {
		إذا (t->entries[i]->versions[v].منوال)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	لكل (i = 0; i < t->اندراج_گنتی; i++) {
		بنية درخت_اندراج *e = t->entries[i];
		إذا (!e->versions[v].منوال)
			استمر;
		strbuf_addf(b, "%o %s%c",
			(بنانشانی صحيح)(e->versions[v].منوال & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_جوڑو(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

ساكن فراغ store_درخت(بنية درخت_اندراج *جڑ)
{
	بنية درخت_content *t;
	بنانشانی صحيح i, j, del;
	بنية آخری_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	بنية object_اندراج *le = عدم;

	إذا (!is_null_oid(&جڑ->versions[1].oid))
		أرجع;

	إذا (!جڑ->درخت)
		لدو_درخت(جڑ);
	t = جڑ->درخت;

	لكل (i = 0; i < t->اندراج_گنتی; i++) {
		إذا (t->entries[i]->درخت)
			store_درخت(t->entries[i]);
	}

	إذا (!(جڑ->versions[0].منوال & NO_DELTA))
		le = لبھو_object(&جڑ->versions[0].oid);
	إذا (S_ISDIR(جڑ->versions[0].منوال) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_درخت);
		lo.انکڑے = old_درخت;
		lo.وتھ = le->idx.وتھ;
		lo.ڈونگھائی = t->delta_ڈونگھائی;
	}

	mktree(t, 1, &new_درخت);
	store_object(OBJ_درخت, &new_درخت, &lo, &جڑ->versions[1].oid, 0);

	t->delta_ڈونگھائی = lo.ڈونگھائی;
	لكل (i = 0, j = 0, del = 0; i < t->اندراج_گنتی; i++) {
		بنية درخت_اندراج *e = t->entries[i];
		إذا (e->versions[1].منوال) {
			e->versions[0].منوال = e->versions[1].منوال;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} وإلا {
			release_درخت_اندراج(e);
			del++;
		}
	}
	t->اندراج_گنتی -= del;
}

ساكن فراغ درخت_content_replace(
	بنية درخت_اندراج *جڑ,
	ثابت بنية object_id *oid,
	ثابت uint16_t منوال,
	بنية درخت_content *newtree)
{
	إذا (!S_ISDIR(منوال))
		die(_("root cannot be a non-directory"));
	oidclr(&جڑ->versions[0].oid, the_repository->hash_algo);
	oidcpy(&جڑ->versions[1].oid, oid);
	إذا (جڑ->درخت)
		release_درخت_content_recursive(جڑ->درخت);
	جڑ->درخت = newtree;
}

ساكن صحيح درخت_content_سیٹ(
	بنية درخت_اندراج *جڑ,
	ثابت حرف *p,
	ثابت بنية object_id *oid,
	ثابت uint16_t منوال,
	بنية درخت_content *subtree)
{
	بنية درخت_content *t;
	ثابت حرف *slash1;
	بنانشانی صحيح i, n;
	بنية درخت_اندراج *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	إذا (!n)
		die(_("empty path component found in input"));
	إذا (!*slash1 && !S_ISDIR(منوال) && subtree)
		die(_("non-directories cannot have subtrees"));

	إذا (!جڑ->درخت)
		لدو_درخت(جڑ);
	t = جڑ->درخت;
	لكل (i = 0; i < t->اندراج_گنتی; i++) {
		e = t->entries[i];
		إذا (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			إذا (!*slash1) {
				إذا (!S_ISDIR(منوال)
						&& e->versions[1].منوال == منوال
						&& oideq(&e->versions[1].oid, oid))
					أرجع 0;
				e->versions[1].منوال = منوال;
				oidcpy(&e->versions[1].oid, oid);
				إذا (e->درخت)
					release_درخت_content_recursive(e->درخت);
				e->درخت = subtree;

				/*
				 * We need to leave e->versions[0].sha1 alone
				 * to avoid modifying the preimage tree used
				 * when writing out the parent directory.
				 * But after replacing the subdir with a
				 * completely different one, it's not a good
				 * delta base any more, and besides, we've
				 * thrown away the tree entries needed to
				 * make a delta against it.
				 *
				 * So let's just explicitly disable deltas
				 * for the subtree.
				 */
				إذا (S_ISDIR(e->versions[0].منوال))
					e->versions[0].منوال |= NO_DELTA;

				oidclr(&جڑ->versions[1].oid, the_repository->hash_algo);
				أرجع 1;
			}
			إذا (!S_ISDIR(e->versions[1].منوال)) {
				e->درخت = new_درخت_content(8);
				e->versions[1].منوال = S_IFDIR;
			}
			إذا (!e->درخت)
				لدو_درخت(e);
			إذا (درخت_content_سیٹ(e, slash1 + 1, oid, منوال, subtree)) {
				oidclr(&جڑ->versions[1].oid, the_repository->hash_algo);
				أرجع 1;
			}
			أرجع 0;
		}
	}

	إذا (t->اندراج_گنتی == t->اندراج_سمرتھا)
		جڑ->درخت = t = grow_درخت_content(t, t->اندراج_گنتی);
	e = new_درخت_اندراج();
	e->name = منزل_atom(p, n);
	e->versions[0].منوال = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->اندراج_گنتی++] = e;
	إذا (*slash1) {
		e->درخت = new_درخت_content(8);
		e->versions[1].منوال = S_IFDIR;
		درخت_content_سیٹ(e, slash1 + 1, oid, منوال, subtree);
	} وإلا {
		e->درخت = subtree;
		e->versions[1].منوال = منوال;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&جڑ->versions[1].oid, the_repository->hash_algo);
	أرجع 1;
}

ساكن صحيح درخت_content_remove(
	بنية درخت_اندراج *جڑ,
	ثابت حرف *p,
	بنية درخت_اندراج *backup_پتی,
	صحيح allow_جڑ)
{
	بنية درخت_content *t;
	ثابت حرف *slash1;
	بنانشانی صحيح i, n;
	بنية درخت_اندراج *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	إذا (!جڑ->درخت)
		لدو_درخت(جڑ);

	إذا (!*p && allow_جڑ) {
		e = جڑ;
		اذهب del_اندراج;
	}

	t = جڑ->درخت;
	لكل (i = 0; i < t->اندراج_گنتی; i++) {
		e = t->entries[i];
		إذا (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			إذا (*slash1 && !S_ISDIR(e->versions[1].منوال))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				أرجع 1;
			إذا (!*slash1 || !S_ISDIR(e->versions[1].منوال))
				اذهب del_اندراج;
			إذا (!e->درخت)
				لدو_درخت(e);
			إذا (درخت_content_remove(e, slash1 + 1, backup_پتی, 0)) {
				لكل (n = 0; n < e->درخت->اندراج_گنتی; n++) {
					إذا (e->درخت->entries[n]->versions[1].منوال) {
						oidclr(&جڑ->versions[1].oid,
						       the_repository->hash_algo);
						أرجع 1;
					}
				}
				backup_پتی = عدم;
				اذهب del_اندراج;
			}
			أرجع 0;
		}
	}
	أرجع 0;

del_اندراج:
	إذا (backup_پتی)
		حافظہ_نقل(backup_پتی, e, حجم(*backup_پتی));
	وإلا إذا (e->درخت)
		release_درخت_content_recursive(e->درخت);
	e->درخت = عدم;
	e->versions[1].منوال = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&جڑ->versions[1].oid, the_repository->hash_algo);
	أرجع 1;
}

ساكن صحيح درخت_content_get(
	بنية درخت_اندراج *جڑ,
	ثابت حرف *p,
	بنية درخت_اندراج *پتی,
	صحيح allow_جڑ)
{
	بنية درخت_content *t;
	ثابت حرف *slash1;
	بنانشانی صحيح i, n;
	بنية درخت_اندراج *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	إذا (!n && !allow_جڑ)
		die(_("empty path component found in input"));

	إذا (!جڑ->درخت)
		لدو_درخت(جڑ);

	إذا (!n) {
		e = جڑ;
		اذهب لبھیا_اندراج;
	}

	t = جڑ->درخت;
	لكل (i = 0; i < t->اندراج_گنتی; i++) {
		e = t->entries[i];
		إذا (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			إذا (!*slash1)
				اذهب لبھیا_اندراج;
			إذا (!S_ISDIR(e->versions[1].منوال))
				أرجع 0;
			إذا (!e->درخت)
				لدو_درخت(e);
			أرجع درخت_content_get(e, slash1 + 1, پتی, 0);
		}
	}
	أرجع 0;

لبھیا_اندراج:
	حافظہ_نقل(پتی, e, حجم(*پتی));
	إذا (e->درخت && is_null_oid(&e->versions[1].oid))
		پتی->درخت = dup_درخت_content(e->درخت);
	وإلا
		پتی->درخت = عدم;
	أرجع 1;
}

ساكن صحيح update_branch(بنية branch *b)
{
	ساكن ثابت حرف *پیغام = "fast-import";
	بنية ref_transaction *transaction;
	بنية object_id old_oid;
	بنية strbuf err = STRBUF_INIT;
	ساكن ثابت حرف *replace_اگیتر = "refs/replace/";

	إذا (starts_with(b->name, replace_اگیتر) &&
	    !strcmp(b->name + لڑی_لمبائی(replace_اگیتر),
		    oid_منزل_hex(&b->oid))) {
		إذا (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_منزل_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				عدم, b->name, عدم, 0);
		أرجع 0;
	}
	إذا (is_null_oid(&b->oid)) {
		إذا (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					عدم, b->name, عدم, 0);
		أرجع 0;
	}
	إذا (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	إذا (!force_update && !is_null_oid(&old_oid)) {
		بنية commit *old_cmit, *new_cmit;
		صحيح ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		إذا (!old_cmit || !new_cmit)
			أرجع error(_("branch %s is missing commits."), b->name);

		ret = repo_وچ_ملاؤ_bases(the_repository, old_cmit, new_cmit);
		إذا (ret < 0)
			باہر_نکلو(128);
		إذا (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_منزل_hex(&b->oid),
				oid_منزل_hex(&old_oid));
			أرجع -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	إذا (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   عدم, عدم, 0, پیغام, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		أرجع -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	أرجع 0;
}

ساكن فراغ dump_branches(فراغ)
{
	بنانشانی صحيح i;
	بنية branch *b;

	لكل (i = 0; i < branch_جدول_sz; i++) {
		لكل (b = branch_جدول[i]; b; b = b->جدول_اگلا_branch)
			failure |= update_branch(b);
	}
}

ساكن فراغ dump_ٹیگ_سوچی(فراغ)
{
	ساكن ثابت حرف *پیغام = "fast-import";
	بنية ٹیگ *t;
	بنية strbuf ref_name = STRBUF_INIT;
	بنية strbuf err = STRBUF_INIT;
	بنية ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	إذا (!transaction) {
		failure |= error("%s", err.buf);
		اذهب cleanup;
	}
	لكل (t = پہلا_ٹیگ; t; t = t->اگلا_ٹیگ) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		إذا (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, عدم, عدم, عدم,
					   0, پیغام, &err)) {
			failure |= error("%s", err.buf);
			اذهب cleanup;
		}
	}
	إذا (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

ساكن فراغ dump_marks(فراغ)
{
	بنية جندرا_file mark_جندرا = جندرا_INIT;
	FILE *f;

	إذا (!export_marks_file || (import_marks_file && !import_marks_file_پورا))
		أرجع;

	إذا (محفوظ_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		أرجع;
	}

	إذا (hold_جندرا_file_for_update(&mark_جندرا, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		أرجع;
	}

	f = fdopen_جندرا_file(&mark_جندرا, "w");
	إذا (!f) {
		صحيح saved_errno = errno;
		rollback_جندرا_file(&mark_جندرا);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		أرجع;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	إذا (commit_جندرا_file(&mark_جندرا)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		أرجع;
	}
}

ساكن فراغ داخل_کرو_object_اندراج(بنية mark_سیٹ **s, بنية object_id *oid, uintmax_t mark)
{
	بنية object_اندراج *e;
	e = لبھو_object(oid);
	إذا (!e) {
		تعداد object_type type = odb_read_object_info(the_repository->objects,
							     oid, عدم);
		إذا (type < 0)
			die(_("object not found: %s"), oid_منزل_hex(oid));
		e = داخل_کرو_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.وتھ = 1; /* just not zero! */
	}
	داخل_کرو_mark(s, mark, e);
}

ساكن فراغ داخل_کرو_oid_اندراج(بنية mark_سیٹ **s, بنية object_id *oid, uintmax_t mark)
{
	داخل_کرو_mark(s, mark, xmemdupz(oid, حجم(*oid)));
}

ساكن فراغ read_mark_file(بنية mark_سیٹ **s, FILE *f, mark_سیٹ_inserter_t inserter)
{
	حرف سطر[512];
	طالما (fgets(سطر, حجم(سطر), f)) {
		uintmax_t mark;
		حرف *سرا;
		بنية object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, حجم(oid.hash));

		سرا = strchr(سطر, '\n');
		إذا (سطر[0] != ':' || !سرا)
			die(_("corrupt mark line: %s"), سطر);
		*سرا = 0;
		mark = strtoumax(سطر + 1, &سرا, 10);
		إذا (!mark || سرا == سطر + 1
			|| *سرا != ' '
			|| get_oid_hex_any(سرا + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), سطر);
		inserter(s, &oid, mark);
	}
}

ساكن فراغ read_marks(فراغ)
{
	FILE *f = fopen(import_marks_file, "r");
	إذا (f)
		;
	وإلا إذا (import_marks_file_ignore_missing && errno == ENOENT)
		اذهب پورا; /* Marks file does not exist */
	وإلا
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, داخل_کرو_object_اندراج);
	fclose(f);
پورا:
	import_marks_file_پورا = 1;
}


ساكن صحيح read_اگلا_command(بنية fast_import_state *state)
{
	ساكن صحيح stdin_eof = 0;

	إذا (stdin_eof) {
		unread_command_buf = 0;
		أرجع EOF;
	}

	لكل (;;) {
		إذا (unread_command_buf) {
			unread_command_buf = 0;
		} وإلا {
			بنية recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			إذا (stdin_eof)
				أرجع EOF;

			إذا (!state->تکیا_انکڑے_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			إذا (rc)
				rc_free = rc->اگلا;
			وإلا {
				rc = cmd_hist.اگلا;
				cmd_hist.اگلا = rc->اگلا;
				cmd_hist.اگلا->prev = &cmd_hist;
				حرر(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_پوچھ;
			rc->اگلا = cmd_hist.prev;
			rc->prev->اگلا = rc;
			cmd_پوچھ = rc;
		}
		إذا (command_buf.buf[0] == '#')
			استمر;
		أرجع 0;
	}
}

ساكن فراغ skip_optional_lf(فراغ)
{
	صحيح رکن_char = fgetc(stdin);
	إذا (رکن_char != '\n' && رکن_char != EOF)
		ungetc(رکن_char, stdin);
}

ساكن فراغ parse_mark(بنية fast_import_state *state)
{
	ثابت حرف *v;
	إذا (skip_اگیتر(command_buf.buf, "mark :", &v)) {
		اگلا_mark = strtoumax(v, عدم, 10);
		read_اگلا_command(state);
	}
	وإلا
		اگلا_mark = 0;
}

ساكن فراغ parse_original_identifier(بنية fast_import_state *state)
{
	ثابت حرف *v;
	إذا (skip_اگیتر(command_buf.buf, "original-oid ", &v))
		read_اگلا_command(state);
}

ساكن صحيح parse_انکڑے(بنية strbuf *sb, uintmax_t حد_بندی, uintmax_t *len_res)
{
	ثابت حرف *انکڑے;
	strbuf_reset(sb);

	إذا (!skip_اگیتر(command_buf.buf, "data ", &انکڑے))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	إذا (skip_اگیتر(انکڑے, "<<", &انکڑے)) {
		حرف *رکن = xstrdup(انکڑے);
		ناپ_t رکن_len = command_buf.len - (انکڑے - command_buf.buf);

		لكل (;;) {
			إذا (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), رکن);
			إذا (رکن_len == command_buf.len
				&& !strcmp(رکن, command_buf.buf))
				اكسر;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		حرر(رکن);
	}
	وإلا {
		uintmax_t len = strtoumax(انکڑے, عدم, 10);
		ناپ_t n = 0, لمائی = (ناپ_t)len;

		إذا (حد_بندی && حد_بندی < len) {
			*len_res = len;
			أرجع 0;
		}
		إذا (لمائی < len)
			die(_("data is too large to use in this context"));

		طالما (n < لمائی) {
			ناپ_t s = strbuf_fread(sb, لمائی - n, stdin);
			إذا (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(بنانشانی لما)(لمائی - n));
			n += s;
		}
	}

	skip_optional_lf();
	أرجع 1;
}

ساكن صحيح validate_raw_date(ثابت حرف *src, بنية strbuf *نتیجہ, صحيح strict)
{
	ثابت حرف *orig_src = src;
	حرف *endp;
	بنانشانی لما عدد;

	errno = 0;

	عدد = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	إذا (errno || endp == src || *endp != ' ')
		أرجع -1;

	src = endp + 1;
	إذا (*src != '-' && *src != '+')
		أرجع -1;

	عدد = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	إذا (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < عدد))             /* parsed a broken timezone */
	   )
		أرجع -1;

	strbuf_addstr(نتیجہ, orig_src);
	أرجع 0;
}

ساكن حرف *parse_ident(ثابت حرف *buf)
{
	ثابت حرف *ltgt;
	ناپ_t name_len;
	بنية strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	إذا (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	إذا (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	إذا (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	إذا (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	إذا (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_جوڑو(&ident, buf, name_len);

	بدل (whenspec) {
	حالة WHENSPEC_RAW:
		إذا (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		اكسر;
	حالة WHENSPEC_RAW_PERMISSIVE:
		إذا (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		اكسر;
	حالة WHENSPEC_RFC2822:
		إذا (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		اكسر;
	حالة WHENSPEC_NOW:
		إذا (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		اكسر;
	}

	أرجع strbuf_detach(&ident, عدم);
}

ساكن فراغ parse_and_store_blob(
	بنية آخری_object *آخری,
	بنية object_id *oidout,
	uintmax_t mark)
{
	ساكن بنية strbuf buf = STRBUF_INIT;
	uintmax_t len;

	إذا (parse_انکڑے(&buf, repo_settings_get_big_file_حد(the_repository), &len))
		store_object(OBJ_BLOB, &buf, آخری, oidout, mark);
	وإلا {
		إذا (آخری) {
			strbuf_release(&آخری->انکڑے);
			آخری->وتھ = 0;
			آخری->ڈونگھائی = 0;
		}
		دھارا_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

ساكن فراغ parse_new_blob(بنية fast_import_state *state)
{
	read_اگلا_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&آخری_blob, عدم, اگلا_mark);
}

ساكن فراغ unload_one_branch(فراغ)
{
	طالما (cur_سرگرم_branches
		&& cur_سرگرم_branches >= max_سرگرم_branches) {
		uintmax_t min_commit = ULONG_MAX;
		بنية branch *e, *l = عدم, *p = عدم;

		لكل (e = سرگرم_branches; e; e = e->سرگرم_اگلا_branch) {
			إذا (e->آخری_commit < min_commit) {
				p = l;
				min_commit = e->آخری_commit;
			}
			l = e;
		}

		إذا (p) {
			e = p->سرگرم_اگلا_branch;
			p->سرگرم_اگلا_branch = e->سرگرم_اگلا_branch;
		} وإلا {
			e = سرگرم_branches;
			سرگرم_branches = e->سرگرم_اگلا_branch;
		}
		e->سرگرم = 0;
		e->سرگرم_اگلا_branch = عدم;
		إذا (e->branch_درخت.درخت) {
			release_درخت_content_recursive(e->branch_درخت.درخت);
			e->branch_درخت.درخت = عدم;
		}
		cur_سرگرم_branches--;
	}
}

ساكن فراغ لدو_branch(بنية branch *b)
{
	لدو_درخت(&b->branch_درخت);
	إذا (!b->سرگرم) {
		b->سرگرم = 1;
		b->سرگرم_اگلا_branch = سرگرم_branches;
		سرگرم_branches = b;
		cur_سرگرم_branches++;
		branch_لدو_گنتی++;
	}
}

ساكن بنانشانی حرف convert_عدد_notes_منزل_fanout(uintmax_t عدد_notes)
{
	بنانشانی حرف fanout = 0;
	طالما ((عدد_notes >>= 8))
		fanout++;
	أرجع fanout;
}

ساكن فراغ construct_رستہ_with_fanout(ثابت حرف *hex_sha1,
		بنانشانی حرف fanout, حرف *رستہ)
{
	بنانشانی صحيح i = 0, j = 0;
	إذا (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	طالما (fanout) {
		رستہ[i++] = hex_sha1[j++];
		رستہ[i++] = hex_sha1[j++];
		رستہ[i++] = '/';
		fanout--;
	}
	حافظہ_نقل(رستہ + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	رستہ[i + the_hash_algo->hexsz - j] = '\0';
}

ساكن uintmax_t do_change_note_fanout(
		بنية درخت_اندراج *orig_جڑ, بنية درخت_اندراج *جڑ,
		حرف *hex_oid, بنانشانی صحيح hex_oid_len,
		حرف *fullpath, بنانشانی صحيح fullpath_len,
		بنانشانی حرف fanout)
{
	بنية درخت_content *t;
	بنية درخت_اندراج *e, پتی;
	بنانشانی صحيح i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t عدد_notes = 0;
	بنية object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	حرف realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ثابت بنانشانی hexsz = the_hash_algo->hexsz;

	إذا (!جڑ->درخت)
		لدو_درخت(جڑ);
	t = جڑ->درخت;

	لكل (i = 0; t && i < t->اندراج_گنتی; i++) {
		e = t->entries[i];
		tmp_hex_oid_len = hex_oid_len + e->name->str_len;
		tmp_fullpath_len = fullpath_len;

		/*
		 * We're interested in EITHER existing note entries (entries
		 * with exactly 40 hex chars in path, not including directory
		 * separators), OR directory entries that may contain note
		 * entries (with < 40 hex chars in path).
		 * Also, each path component in a note entry must be a multiple
		 * of 2 chars.
		 */
		إذا (!e->versions[1].منوال ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			استمر;

		/* This _may_ be a note entry, or a subdir containing notes */
		حافظہ_نقل(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		إذا (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		حافظہ_نقل(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		إذا (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			إذا (fanout == 0xff) {
				/* Counting mode, no rename */
				عدد_notes++;
				استمر;
			}
			construct_رستہ_with_fanout(hex_oid, fanout, realpath);
			إذا (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				عدد_notes++;
				استمر;
			}

			/* Rename fullpath to realpath */
			إذا (!درخت_content_remove(orig_جڑ, fullpath, &پتی, 0))
				die(_("failed to remove path %s"), fullpath);
			درخت_content_سیٹ(orig_جڑ, realpath,
				&پتی.versions[1].oid,
				پتی.versions[1].منوال,
				پتی.درخت);
		} وإلا إذا (S_ISDIR(e->versions[1].منوال)) {
			/* This is a subdir that may contain note entries */
			عدد_notes += do_change_note_fanout(orig_جڑ, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = جڑ->درخت;
	}
	أرجع عدد_notes;
}

ساكن uintmax_t change_note_fanout(بنية درخت_اندراج *جڑ,
		بنانشانی حرف fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	حرف hex_oid[GIT_MAX_HEXSZ], رستہ[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	أرجع do_change_note_fanout(جڑ, جڑ, hex_oid, 0, رستہ, 0, fanout);
}

ساكن صحيح parse_mapped_oid_hex(ثابت حرف *hex, بنية object_id *oid, ثابت حرف **سرا)
{
	صحيح algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, حجم(oid->hash));

	algo = parse_oid_hex_any(hex, oid, سرا);
	إذا (algo == GIT_HASH_UNKNOWN)
		أرجع -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	إذا (it == kh_سرا(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		إذا (hash_algos[algo].format_id == the_hash_algo->format_id)
			أرجع 0;
		أرجع -1;
	}
	oidcpy(oid, kh_قدر(sub_oid_map, it));
	أرجع 0;
}

/*
 * Given a pointer into a string, parse a mark reference:
 *
 *   idnum ::= ':' bigint;
 *
 * Update *endptr to point to the first character after the value.
 *
 * Complain if the following character is not what is expected,
 * either a space or end of the string.
 */
ساكن uintmax_t parse_mark_ref(ثابت حرف *p, حرف **endptr)
{
	uintmax_t mark;

	أكد(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	إذا (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	أرجع mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
ساكن uintmax_t parse_mark_ref_eol(ثابت حرف *p)
{
	حرف *سرا;
	uintmax_t mark;

	mark = parse_mark_ref(p, &سرا);
	إذا (*سرا != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	أرجع mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
ساكن uintmax_t parse_mark_ref_space(ثابت حرف **p)
{
	uintmax_t mark;
	حرف *سرا;

	mark = parse_mark_ref(*p, &سرا);
	إذا (*سرا++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = سرا;
	أرجع mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
ساكن فراغ parse_رستہ(بنية strbuf *sb, ثابت حرف *p, ثابت حرف **endp,
		صحيح is_آخری_میدان, ثابت حرف *میدان)
{
	إذا (*p == '"') {
		إذا (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), میدان, command_buf.buf);
		إذا (لڑی_لمبائی(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), میدان, command_buf.buf);
	} وإلا {
		/*
		 * Unless we are parsing the last field of a line,
		 * SP is the end of this field.
		 */
		*endp = is_آخری_میدان
			? p + لڑی_لمبائی(p)
			: strchrnul(p, ' ');
		strbuf_جوڑو(sb, p, *endp - p);
	}
}

/*
 * Parse the path string into the strbuf, and complain if this is not the end of
 * the string. Unquoted strings may contain spaces.
 */
ساكن فراغ parse_رستہ_eol(بنية strbuf *sb, ثابت حرف *p, ثابت حرف *میدان)
{
	ثابت حرف *سرا;

	parse_رستہ(sb, p, &سرا, 1, میدان);
	إذا (*سرا)
		die(_("garbage after %s: %s"), میدان, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
ساكن فراغ parse_رستہ_space(بنية strbuf *sb, ثابت حرف *p,
		ثابت حرف **endp, ثابت حرف *میدان)
{
	parse_رستہ(sb, p, endp, 0, میدان);
	إذا (**endp != ' ')
		die(_("missing space after %s: %s"), میدان, command_buf.buf);
	(*endp)++;
}

ساكن فراغ file_change_m(بنية fast_import_state *state,
			  ثابت حرف *p,
			  بنية branch *b)
{
	ساكن بنية strbuf رستہ = STRBUF_INIT;
	بنية object_اندراج *oe;
	بنية object_id oid;
	uint16_t منوال, inline_انکڑے = 0;

	p = parse_منوال(p, &منوال);
	إذا (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	بدل (منوال) {
	حالة 0644:
	حالة 0755:
		منوال |= S_IFREG;
	حالة S_IFREG | 0644:
	حالة S_IFREG | 0755:
	حالة S_IFLNK:
	حالة S_IFDIR:
	حالة S_IFGITLINK:
		/* ok */
		اكسر;
	افتراضي:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	إذا (*p == ':') {
		oe = لبھو_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} وإلا إذا (skip_اگیتر(p, "inline ", &p)) {
		inline_انکڑے = 1;
		oe = عدم; /* not used with inline_data, but makes gcc happy */
	} وإلا {
		إذا (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = لبھو_object(&oid);
		إذا (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&رستہ);
	parse_رستہ_eol(&رستہ, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	إذا (S_ISDIR(منوال) &&
	    is_خالی_تھاواں_درخت_oid(&oid, the_repository->hash_algo) &&
	    *رستہ.buf) {
		درخت_content_remove(&b->branch_درخت, رستہ.buf, عدم, 0);
		أرجع;
	}

	إذا (S_ISGITLINK(منوال)) {
		إذا (inline_انکڑے)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		وإلا إذا (oe) {
			إذا (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} وإلا إذا (inline_انکڑے) {
		إذا (S_ISDIR(منوال))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		طالما (read_اگلا_command(state) != EOF) {
			ثابت حرف *v;
			إذا (skip_اگیتر(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			وإلا {
				parse_and_store_blob(&آخری_blob, &oid, 0);
				اكسر;
			}
		}
	} وإلا {
		تعداد object_type expected = S_ISDIR(منوال) ?
						OBJ_درخت: OBJ_BLOB;
		تعداد object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, عدم);
		إذا (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(منوال) ?  _("tree") : _("blob"),
			    command_buf.buf);
		إذا (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	إذا (!*رستہ.buf) {
		درخت_content_replace(&b->branch_درخت, &oid, منوال, عدم);
		أرجع;
	}

	إذا (!verify_رستہ(رستہ.buf, منوال))
		die(_("invalid path '%s'"), رستہ.buf);
	درخت_content_سیٹ(&b->branch_درخت, رستہ.buf, &oid, منوال, عدم);
}

ساكن فراغ file_change_d(ثابت حرف *p, بنية branch *b)
{
	ساكن بنية strbuf رستہ = STRBUF_INIT;

	strbuf_reset(&رستہ);
	parse_رستہ_eol(&رستہ, p, "path");
	درخت_content_remove(&b->branch_درخت, رستہ.buf, عدم, 1);
}

ساكن فراغ file_change_cr(ثابت حرف *p, بنية branch *b, صحيح rename)
{
	ساكن بنية strbuf سروت = STRBUF_INIT;
	ساكن بنية strbuf dest = STRBUF_INIT;
	بنية درخت_اندراج پتی;

	strbuf_reset(&سروت);
	parse_رستہ_space(&سروت, p, &p, "source");
	strbuf_reset(&dest);
	parse_رستہ_eol(&dest, p, "dest");

	memset(&پتی, 0, حجم(پتی));
	إذا (rename)
		درخت_content_remove(&b->branch_درخت, سروت.buf, &پتی, 1);
	وإلا
		درخت_content_get(&b->branch_درخت, سروت.buf, &پتی, 1);
	إذا (!پتی.versions[1].منوال)
		die(_("path %s not in branch"), سروت.buf);
	إذا (!*dest.buf) {	/* C "path/to/subdir" "" */
		درخت_content_replace(&b->branch_درخت,
			&پتی.versions[1].oid,
			پتی.versions[1].منوال,
			پتی.درخت);
		أرجع;
	}
	إذا (!verify_رستہ(dest.buf, پتی.versions[1].منوال))
		die(_("invalid path '%s'"), dest.buf);
	درخت_content_سیٹ(&b->branch_درخت, dest.buf,
		&پتی.versions[1].oid,
		پتی.versions[1].منوال,
		پتی.درخت);
}

ساكن فراغ note_change_n(بنية fast_import_state *state,
			  ثابت حرف *p,
			  بنية branch *b,
			  بنانشانی حرف *old_fanout)
{
	بنية object_اندراج *oe;
	بنية branch *s;
	بنية object_id oid, commit_oid;
	حرف رستہ[GIT_MAX_RAWSZ * 3];
	uint16_t inline_انکڑے = 0;
	بنانشانی حرف new_fanout;

	/*
	 * When loading a branch, we don't traverse its tree to count the real
	 * number of notes (too expensive to do this for all non-note refs).
	 * This means that recently loaded notes refs might incorrectly have
	 * b->num_notes == 0, and consequently, old_fanout might be wrong.
	 *
	 * Fix this by traversing the tree and counting the number of notes
	 * when b->num_notes == 0. If the notes tree is truly empty, the
	 * calculation should not take long.
	 */
	إذا (b->عدد_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->عدد_notes = change_note_fanout(&b->branch_درخت, 0xff);
		*old_fanout = convert_عدد_notes_منزل_fanout(b->عدد_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	إذا (*p == ':') {
		oe = لبھو_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} وإلا إذا (skip_اگیتر(p, "inline ", &p)) {
		inline_انکڑے = 1;
		oe = عدم; /* not used with inline_data, but makes gcc happy */
	} وإلا {
		إذا (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = لبھو_object(&oid);
		إذا (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	إذا (s) {
		إذا (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} وإلا إذا (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		بنية object_اندراج *commit_oe = لبھو_mark(marks, commit_mark);
		إذا (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} وإلا إذا (!repo_get_oid(the_repository, p, &commit_oid)) {
		ناپ_t ناپ;
		حرف *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &ناپ,
						   &commit_oid);
		إذا (!buf || ناپ < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		حرر(buf);
	} وإلا
		die(_("invalid ref name or SHA1 expression: %s"), p);

	إذا (inline_انکڑے) {
		read_اگلا_command(state);
		parse_and_store_blob(&آخری_blob, &oid, 0);
	} وإلا إذا (oe) {
		إذا (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} وإلا إذا (!is_null_oid(&oid)) {
		تعداد object_type type = odb_read_object_info(the_repository->objects, &oid,
							عدم);
		إذا (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		إذا (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_رستہ_with_fanout(oid_منزل_hex(&commit_oid), *old_fanout, رستہ);
	إذا (درخت_content_remove(&b->branch_درخت, رستہ, عدم, 0))
		b->عدد_notes--;

	إذا (is_null_oid(&oid))
		أرجع; /* nothing to insert */

	b->عدد_notes++;
	new_fanout = convert_عدد_notes_منزل_fanout(b->عدد_notes);
	construct_رستہ_with_fanout(oid_منزل_hex(&commit_oid), new_fanout, رستہ);
	درخت_content_سیٹ(&b->branch_درخت, رستہ, &oid, S_IFREG | 0644, عدم);
}

ساكن فراغ file_change_deleteall(بنية branch *b)
{
	release_درخت_content_recursive(b->branch_درخت.درخت);
	oidclr(&b->branch_درخت.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_درخت.versions[1].oid, the_repository->hash_algo);
	لدو_درخت(&b->branch_درخت);
	b->عدد_notes = 0;
}

ساكن فراغ parse_from_commit(بنية branch *b, حرف *buf, بنانشانی لما ناپ)
{
	إذا (!buf || ناپ < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_منزل_hex(&b->oid));
	إذا (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_درخت.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_منزل_hex(&b->oid));
	oidcpy(&b->branch_درخت.versions[0].oid,
	       &b->branch_درخت.versions[1].oid);
}

ساكن فراغ parse_from_existing(بنية branch *b)
{
	إذا (is_null_oid(&b->oid)) {
		oidclr(&b->branch_درخت.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_درخت.versions[1].oid, the_repository->hash_algo);
	} وإلا {
		بنانشانی لما ناپ;
		ناپ_t ناپ_st = 0;
		حرف *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &ناپ_st, &b->oid);
		ناپ = cast_ناپ_t_منزل_ulong(ناپ_st);
		parse_from_commit(b, buf, ناپ);
		حرر(buf);
	}
}

ساكن صحيح parse_objectish(بنية fast_import_state *state,
			   بنية branch *b,
			   ثابت حرف *objectish)
{
	بنية branch *s;
	بنية object_id oid;

	oidcpy(&oid, &b->branch_درخت.versions[1].oid);

	s = lookup_branch(objectish);
	إذا (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	وإلا إذا (s) {
		بنية object_id *t = &s->branch_درخت.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_درخت.versions[0].oid, t);
		oidcpy(&b->branch_درخت.versions[1].oid, t);
	} وإلا إذا (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		بنية object_اندراج *oe = لبھو_mark(marks, idnum);
		إذا (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		إذا (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			إذا (oe->pack_id != MAX_PACK_ID) {
				بنانشانی لما ناپ;
				حرف *buf = gfi_unpack_اندراج(oe, &ناپ);
				parse_from_commit(b, buf, ناپ);
				حرر(buf);
			} وإلا
				parse_from_existing(b);
		}
	} وإلا إذا (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		إذا (is_null_oid(&b->oid))
			b->delete = 1;
	}
	وإلا
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	إذا (b->branch_درخت.درخت && !oideq(&oid, &b->branch_درخت.versions[1].oid)) {
		release_درخت_content_recursive(b->branch_درخت.درخت);
		b->branch_درخت.درخت = عدم;
	}

	read_اگلا_command(state);
	أرجع 1;
}

ساكن صحيح parse_from(بنية fast_import_state *state, بنية branch *b)
{
	ثابت حرف *from;

	إذا (!skip_اگیتر(command_buf.buf, "from ", &from))
		أرجع 0;

	أرجع parse_objectish(state, b, from);
}

ساكن صحيح parse_objectish_with_اگیتر(بنية fast_import_state *state,
				       بنية branch *b,
				       ثابت حرف *اگیتر)
{
	ثابت حرف *ادھار;

	إذا (!skip_اگیتر(command_buf.buf, اگیتر, &ادھار))
		أرجع 0;

	أرجع parse_objectish(state, b, ادھار);
}

ساكن بنية hash_list *parse_ملاؤ(بنية fast_import_state *state,
				     بنانشانی صحيح *گنتی)
{
	بنية hash_list *list = عدم, **پوچھ = &list, *n;
	ثابت حرف *from;
	بنية branch *s;

	*گنتی = 0;
	طالما (skip_اگیتر(command_buf.buf, "merge ", &from)) {
		n = xmalloc(حجم(*n));
		s = lookup_branch(from);
		إذا (s)
			oidcpy(&n->oid, &s->oid);
		وإلا إذا (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			بنية object_اندراج *oe = لبھو_mark(marks, idnum);
			إذا (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} وإلا إذا (!repo_get_oid(the_repository, from, &n->oid)) {
			ناپ_t ناپ;
			حرف *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &ناپ, &n->oid);
			إذا (!buf || ناپ < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			حرر(buf);
		} وإلا
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->اگلا = عدم;
		*پوچھ = n;
		پوچھ = &n->اگلا;

		(*گنتی)++;
		read_اگلا_command(state);
	}
	أرجع list;
}

بنية signature_انکڑے {
	حرف *hash_algo;      /* "sha1" or "sha256" */
	حرف *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	بنية strbuf انکڑے;   /* The actual signature data */
};

ساكن فراغ parse_one_signature(بنية fast_import_state *state,
				بنية signature_انکڑے *sig,
				ثابت حرف *v)
{
	حرف *دلیلاں = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	حرف *space = strchr(دلیلاں, ' ');

	إذا (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), دلیلاں);
	*space = '\0';

	sig->hash_algo = دلیلاں;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	إذا (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	إذا (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	إذا (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_اگلا_command(state);
	parse_انکڑے(&sig->انکڑے, 0, عدم);
}

ساكن فراغ discard_one_signature(بنية fast_import_state *state)
{
	بنية strbuf انکڑے = STRBUF_INIT;

	read_اگلا_command(state);
	parse_انکڑے(&انکڑے, 0, عدم);
	strbuf_release(&انکڑے);
}

ساكن فراغ جوڑو_gpgsig_منزل_commit(بنية strbuf *commit_انکڑے,
				 ثابت حرف *سرخی,
				 بنية signature_انکڑے *sig)
{
	بنية string_list siglines = STRING_LIST_INIT_NODUP;

	إذا (!sig || !sig->hash_algo)
		أرجع;

	strbuf_addstr(commit_انکڑے, سرخی);
	string_list_ونڈو_وچ_رکھو(&siglines, sig->انکڑے.buf, "\n", -1);
	strbuf_جوڑو_separated_string_list(commit_انکڑے, "\n ", &siglines);
	strbuf_addch(commit_انکڑے, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->انکڑے);
	حرر(sig->hash_algo);
}

ساكن فراغ store_signature(بنية signature_انکڑے *stored_sig,
			    بنية signature_انکڑے *new_sig,
			    ثابت حرف *hash_type)
{
	إذا (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->انکڑے);
		حرر(new_sig->hash_algo);
	} وإلا {
		*stored_sig = *new_sig;
	}
}

ساكن فراغ import_one_signature(بنية fast_import_state *state,
				 بنية signature_انکڑے *sig_sha1,
				 بنية signature_انکڑے *sig_sha256,
				 ثابت حرف *v)
{
	بنية signature_انکڑے sig = { عدم, عدم, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	إذا (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	وإلا إذا (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	وإلا
		die(_("parse_one_signature() returned unknown hash algo"));
}

ساكن فراغ finalize_commit_بفر(بنية strbuf *new_انکڑے,
				   بنية signature_انکڑے *sig_sha1,
				   بنية signature_انکڑے *sig_sha256,
				   بنية strbuf *پیغام)
{
	جوڑو_gpgsig_منزل_commit(new_انکڑے, "gpgsig ", sig_sha1);
	جوڑو_gpgsig_منزل_commit(new_انکڑے, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_انکڑے, '\n');
	strbuf_addbuf(new_انکڑے, پیغام);
}

ساكن فراغ warn_invalid_signature(بنية signature_check *check,
				   ثابت حرف *پیغام, تعداد sign_منوال منوال)
{
	ثابت حرف *signer = check->signer ? check->signer : _("unknown");
	ثابت حرف *subject;
	صحيح subject_len = لبھو_commit_subject(پیغام, &subject);

	بدل (منوال) {
	حالة SIGN_STRIP_IF_INVALID:
		إذا (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		وإلا إذا (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		وإلا
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		اكسر;
	حالة SIGN_SIGN_IF_INVALID:
		إذا (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		وإلا إذا (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		وإلا
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		اكسر;
	افتراضي:
		BUG("unsupported signing mode");
	}
}

ساكن فراغ handle_signature_if_invalid(بنية strbuf *new_انکڑے,
					بنية signature_انکڑے *sig_sha1,
					بنية signature_انکڑے *sig_sha256,
					بنية strbuf *پیغام,
					تعداد sign_منوال منوال)
{
	بنية strbuf tmp_buf = STRBUF_INIT;
	بنية signature_check signature_check = { 0 };
	صحيح ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_انکڑے);
	finalize_commit_بفر(&tmp_buf, sig_sha1, sig_sha256, پیغام);
	ret = verify_commit_بفر(tmp_buf.buf, tmp_buf.len, &signature_check);

	إذا (ret) {
		إذا (منوال == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, پیغام->buf, منوال);

		إذا (منوال == SIGN_SIGN_IF_INVALID) {
			بنية strbuf signature = STRBUF_INIT;
			بنية strbuf payload = STRBUF_INIT;

			/*
			 * NEEDSWORK: To properly support interoperability mode
			 * when signing commit signatures, the commit buffer
			 * must be provided in both the repository and
			 * compatibility object formats. As currently
			 * implemented, only the repository object format is
			 * considered meaning compatibility signatures cannot be
			 * generated. Thus, attempting to sign commit signatures
			 * in interoperability mode is currently unsupported.
			 */
			إذا (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			إذا (sign_بفر(&payload, &signature, signed_commit_keyid,
					SIGN_بفر_USE_DEFAULT_کلید))
				die(_("failed to sign commit object"));
			جوڑو_سرخی_signature(new_انکڑے, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_بفر(new_انکڑے, عدم, عدم, پیغام);
	} وإلا {
		strbuf_وٹاندرا(new_انکڑے, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

ساكن فراغ parse_new_commit(بنية fast_import_state *state, ثابت حرف *arg)
{
	ساكن بنية strbuf پیغام = STRBUF_INIT;
	بنية signature_انکڑے sig_sha1 = { عدم, عدم, STRBUF_INIT };
	بنية signature_انکڑے sig_sha256 = { عدم, عدم, STRBUF_INIT };
	بنية branch *b;
	حرف *author = عدم;
	حرف *committer = عدم;
	حرف *encoding = عدم;
	بنية hash_list *ملاؤ_list = عدم;
	بنانشانی صحيح ملاؤ_گنتی;
	بنانشانی حرف prev_fanout, new_fanout;
	ثابت حرف *v;

	b = lookup_branch(arg);
	إذا (!b)
		b = new_branch(arg);

	read_اگلا_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	إذا (skip_اگیتر(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_اگلا_command(state);
	}
	إذا (skip_اگیتر(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_اگلا_command(state);
	}
	إذا (!committer)
		die(_("expected committer but didn't get one"));

	طالما (skip_اگیتر(command_buf.buf, "gpgsig ", &v)) {
		بدل (signed_commit_منوال) {

		/* First, modes that don't need the signature to be parsed */
		حالة SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		حالة SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		حالة SIGN_STRIP:
			discard_one_signature(state);
			اكسر;

		/* Second, modes that parse the signature */
		حالة SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		حالة SIGN_VERBATIM:
		حالة SIGN_STRIP_IF_INVALID:
		حالة SIGN_SIGN_IF_INVALID:
		حالة SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			اكسر;

		/* Third, BUG */
		افتراضي:
			BUG("invalid signed_commit_mode value %d", signed_commit_منوال);
		}
		read_اگلا_command(state);
	}

	إذا (skip_اگیتر(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_اگلا_command(state);
	}
	parse_انکڑے(&پیغام, 0, عدم);
	read_اگلا_command(state);
	parse_from(state, b);
	ملاؤ_list = parse_ملاؤ(state, &ملاؤ_گنتی);

	/* ensure the branch is active/loaded */
	إذا (!b->branch_درخت.درخت || !max_سرگرم_branches) {
		unload_one_branch();
		لدو_branch(b);
	}

	prev_fanout = convert_عدد_notes_منزل_fanout(b->عدد_notes);

	/* file_change* */
	طالما (command_buf.len > 0) {
		إذا (skip_اگیتر(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		وإلا إذا (skip_اگیتر(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		وإلا إذا (skip_اگیتر(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		وإلا إذا (skip_اگیتر(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		وإلا إذا (skip_اگیتر(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		وإلا إذا (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		وإلا إذا (skip_اگیتر(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		وإلا إذا (skip_اگیتر(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		وإلا {
			unread_command_buf = 1;
			اكسر;
		}
		إذا (read_اگلا_command(state) == EOF)
			اكسر;
	}

	new_fanout = convert_عدد_notes_منزل_fanout(b->عدد_notes);
	إذا (new_fanout != prev_fanout)
		b->عدد_notes = change_note_fanout(&b->branch_درخت, new_fanout);

	/* build the tree and the commit */
	store_درخت(&b->branch_درخت);
	oidcpy(&b->branch_درخت.versions[0].oid,
	       &b->branch_درخت.versions[1].oid);

	strbuf_reset(&new_انکڑے);
	strbuf_addf(&new_انکڑے, "tree %s\n",
		oid_منزل_hex(&b->branch_درخت.versions[1].oid));
	إذا (!is_null_oid(&b->oid))
		strbuf_addf(&new_انکڑے, "parent %s\n",
			    oid_منزل_hex(&b->oid));
	طالما (ملاؤ_list) {
		بنية hash_list *اگلا = ملاؤ_list->اگلا;
		strbuf_addf(&new_انکڑے, "parent %s\n",
			    oid_منزل_hex(&ملاؤ_list->oid));
		حرر(ملاؤ_list);
		ملاؤ_list = اگلا;
	}
	strbuf_addf(&new_انکڑے,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	إذا (encoding)
		strbuf_addf(&new_انکڑے,
			"encoding %s\n",
			encoding);

	إذا ((signed_commit_منوال == SIGN_STRIP_IF_INVALID ||
	     signed_commit_منوال == SIGN_SIGN_IF_INVALID ||
	     signed_commit_منوال == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_انکڑے, &sig_sha1, &sig_sha256,
					    &پیغام, signed_commit_منوال);
	وإلا
		finalize_commit_بفر(&new_انکڑے, &sig_sha1, &sig_sha256, &پیغام);

	حرر(author);
	حرر(committer);
	حرر(encoding);

	إذا (!store_object(OBJ_COMMIT, &new_انکڑے, عدم, &b->oid, اگلا_mark))
		b->pack_id = pack_id;
	b->آخری_commit = object_گنتی_by_type[OBJ_COMMIT];
}

ساكن فراغ handle_ٹیگ_signature_if_invalid(بنية strbuf *buf,
					    بنية strbuf *پیغام,
					    ناپ_t sig_وتھ)
{
	بنية strbuf signature = STRBUF_INIT;
	بنية strbuf payload = STRBUF_INIT;
	بنية signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_جوڑو(&payload, پیغام->buf, sig_وتھ);
	strbuf_جوڑو(&signature, پیغام->buf + sig_وتھ, پیغام->len - sig_وتھ);

	sigc.payload_type = SIGNATURE_PAYLOAD_ٹیگ;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	إذا (!check_signature(&sigc, signature.buf, signature.len))
		اذهب باہرلا;

	إذا (signed_ٹیگ_منوال == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(پیغام, sig_وتھ);

	إذا (signed_ٹیگ_منوال == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = عدم;
		strbuf_reset(&signature);

		إذا (sign_بفر(&payload, &signature, signed_ٹیگ_keyid,
				SIGN_بفر_USE_DEFAULT_کلید))
			die(_("failed to sign tag object"));

		strbuf_addbuf(پیغام, &signature);
	}

باہرلا:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

ساكن فراغ handle_ٹیگ_signature(بنية strbuf *buf, بنية strbuf *پیغام, ثابت حرف *name)
{
	ناپ_t sig_وتھ = parse_signed_بفر(پیغام->buf, پیغام->len);

	/* If there is no signature, there is nothing to do. */
	إذا (sig_وتھ >= پیغام->len)
		أرجع;

	بدل (signed_ٹیگ_منوال) {

	/* First, modes that don't change anything */
	حالة SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	حالة SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		اكسر;

	/* Second, modes that remove the signature */
	حالة SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	حالة SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(پیغام, sig_وتھ);
		اكسر;
	حالة SIGN_ABORT_IF_INVALID:
	حالة SIGN_SIGN_IF_INVALID:
	حالة SIGN_STRIP_IF_INVALID:
		handle_ٹیگ_signature_if_invalid(buf, پیغام, sig_وتھ);
		اكسر;

	/* Third, aborting modes */
	حالة SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	افتراضي:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_ٹیگ_منوال, name);
	}
}

ساكن فراغ parse_new_ٹیگ(بنية fast_import_state *state, ثابت حرف *arg)
{
	ساكن بنية strbuf پیغام = STRBUF_INIT;
	ثابت حرف *from;
	حرف *tagger;
	بنية branch *s;
	بنية ٹیگ *t;
	uintmax_t from_mark = 0;
	بنية object_id oid;
	تعداد object_type type;
	ثابت حرف *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية ٹیگ));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	إذا (آخری_ٹیگ)
		آخری_ٹیگ->اگلا_ٹیگ = t;
	وإلا
		پہلا_ٹیگ = t;
	آخری_ٹیگ = t;
	read_اگلا_command(state);
	parse_mark(state);

	/* from ... */
	إذا (!skip_اگیتر(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	إذا (s) {
		إذا (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} وإلا إذا (*from == ':') {
		بنية object_اندراج *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = لبھو_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} وإلا إذا (!repo_get_oid(the_repository, from, &oid)) {
		بنية object_اندراج *oe = لبھو_object(&oid);
		إذا (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, عدم);
			إذا (type < 0)
				die(_("not a valid object: %s"), from);
		} وإلا
			type = oe->type;
	} وإلا
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_اگلا_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	إذا (skip_اگیتر(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_اگلا_command(state);
	} وإلا
		tagger = عدم;

	/* tag payload/message */
	parse_انکڑے(&پیغام, 0, عدم);

	/* build the tag object */
	strbuf_reset(&new_انکڑے);

	strbuf_addf(&new_انکڑے,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_منزل_hex(&oid), type_name(type), t->name);
	إذا (tagger)
		strbuf_addf(&new_انکڑے,
			    "tagger %s\n", tagger);

	handle_ٹیگ_signature(&new_انکڑے, &پیغام, t->name);

	strbuf_addch(&new_انکڑے, '\n');
	strbuf_addbuf(&new_انکڑے, &پیغام);
	حرر(tagger);

	إذا (store_object(OBJ_ٹیگ, &new_انکڑے, عدم, &t->oid, اگلا_mark))
		t->pack_id = MAX_PACK_ID;
	وإلا
		t->pack_id = pack_id;
}

ساكن فراغ parse_reset_branch(بنية fast_import_state *state, ثابت حرف *arg)
{
	بنية branch *b;
	ثابت حرف *ٹیگ_name;

	b = lookup_branch(arg);
	إذا (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_درخت.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_درخت.versions[1].oid, the_repository->hash_algo);
		إذا (b->branch_درخت.درخت) {
			release_درخت_content_recursive(b->branch_درخت.درخت);
			b->branch_درخت.درخت = عدم;
		}
	}
	وإلا
		b = new_branch(arg);
	read_اگلا_command(state);
	parse_from(state, b);
	إذا (b->delete && skip_اگیتر(b->name, "refs/tags/", &ٹیگ_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		بنية ٹیگ *t, *prev = عدم;
		لكل (t = پہلا_ٹیگ; t; t = t->اگلا_ٹیگ) {
			إذا (!strcmp(t->name, ٹیگ_name))
				اكسر;
			prev = t;
		}
		إذا (t) {
			إذا (prev)
				prev->اگلا_ٹیگ = t->اگلا_ٹیگ;
			وإلا
				پہلا_ٹیگ = t->اگلا_ٹیگ;
			إذا (!t->اگلا_ٹیگ)
				آخری_ٹیگ = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	إذا (command_buf.len > 0)
		unread_command_buf = 1;
}

ساكن فراغ cat_blob_write(ثابت حرف *buf, بنانشانی لما ناپ)
{
	إذا (write_وچ_بھریا(cat_blob_fd, buf, ناپ) < 0)
		die_errno(_("write to frontend failed"));
}

ساكن فراغ cat_blob(بنية object_اندراج *oe, بنية object_id *oid)
{
	بنية strbuf سطر = STRBUF_INIT;
	بنية iovec iov[3];
	بنانشانی لما ناپ;
	تعداد object_type type = 0;
	حرف *buf;

	إذا (!oe || oe->pack_id == MAX_PACK_ID) {
		ناپ_t ناپ_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &ناپ_st);
		ناپ = cast_ناپ_t_منزل_ulong(ناپ_st);
	} وإلا {
		type = oe->type;
		buf = gfi_unpack_اندراج(oe, &ناپ);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	إذا (type <= 0) {
		strbuf_reset(&سطر);
		strbuf_addf(&سطر, "%s missing\n", oid_منزل_hex(oid));
		cat_blob_write(سطر.buf, سطر.len);
		strbuf_release(&سطر);
		حرر(buf);
		أرجع;
	}
	إذا (!buf)
		die(_("can't read object %s"), oid_منزل_hex(oid));
	إذا (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_منزل_hex(oid), type_name(type));
	strbuf_reset(&سطر);
	strbuf_addf(&سطر, "%s %s %"PRIuMAX"\n", oid_منزل_hex(oid),
		    type_name(type), (uintmax_t)ناپ);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_ادھار = سطر.buf;
	iov[0].iov_len = سطر.len;
	iov[1].iov_ادھار = buf;
	iov[1].iov_len = ناپ;
	iov[2].iov_ادھار = (فراغ *) "\n";
	iov[2].iov_len = 1;

	إذا (writev_وچ_بھریا(cat_blob_fd, iov, ARRAY_ناپ(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&سطر);
	إذا (oe && oe->pack_id == pack_id) {
		آخری_blob.وتھ = oe->idx.وتھ;
		strbuf_attach(&آخری_blob.انکڑے, buf, ناپ, ناپ + 1);
		آخری_blob.ڈونگھائی = oe->ڈونگھائی;
	} وإلا
		حرر(buf);
}

ساكن فراغ parse_get_mark(بنية fast_import_state *state بن_ورتیا,
			   ثابت حرف *p)
{
	بنية object_اندراج *oe;
	حرف output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	إذا (*p != ':')
		die(_("not a mark: %s"), p);

	oe = لبھو_mark(marks, parse_mark_ref_eol(p));
	إذا (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, حجم(output), "%s\n", oid_منزل_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

ساكن فراغ parse_cat_blob(بنية fast_import_state *state بن_ورتیا,
			   ثابت حرف *p)
{
	بنية object_اندراج *oe;
	بنية object_id oid;

	/* cat-blob SP <object> LF */
	إذا (*p == ':') {
		oe = لبھو_mark(marks, parse_mark_ref_eol(p));
		إذا (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} وإلا {
		إذا (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		إذا (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = لبھو_object(&oid);
	}

	cat_blob(oe, &oid);
}

ساكن بنية object_اندراج *dereference(بنية object_اندراج *oe,
					بنية object_id *oid)
{
	بنانشانی لما ناپ;
	حرف *buf = عدم;
	ثابت بنانشانی hexsz = the_hash_algo->hexsz;

	إذا (!oe) {
		تعداد object_type type = odb_read_object_info(the_repository->objects,
							     oid, عدم);
		إذا (type < 0)
			die(_("object not found: %s"), oid_منزل_hex(oid));
		/* cache it! */
		oe = داخل_کرو_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.وتھ = 1;
	}
	بدل (oe->type) {
	حالة OBJ_درخت:	/* easy case. */
		أرجع oe;
	حالة OBJ_COMMIT:
	حالة OBJ_ٹیگ:
		اكسر;
	افتراضي:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	إذا (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_اندراج(oe, &ناپ);
	} وإلا {
		تعداد object_type بن_ورتیا_2;
		ناپ_t ناپ_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &بن_ورتیا_2, &ناپ_st);
		ناپ = cast_ناپ_t_منزل_ulong(ناپ_st);
	}
	إذا (!buf)
		die(_("can't load object %s"), oid_منزل_hex(oid));

	/* Peel one layer. */
	بدل (oe->type) {
	حالة OBJ_ٹیگ:
		إذا (ناپ < hexsz + لڑی_لمبائی("object ") ||
		    get_oid_hex(buf + لڑی_لمبائی("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		اكسر;
	حالة OBJ_COMMIT:
		إذا (ناپ < hexsz + لڑی_لمبائی("tree ") ||
		    get_oid_hex(buf + لڑی_لمبائی("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	حرر(buf);
	أرجع لبھو_object(oid);
}

ساكن فراغ داخل_کرو_mapped_mark(uintmax_t mark, فراغ *object, فراغ *cbp)
{
	بنية object_id *fromoid = object;
	بنية object_id *tooid = لبھو_mark(cbp, mark);
	صحيح ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	إذا (ret == 0)
		أرجع;
	kh_قدر(sub_oid_map, it) = tooid;
}

ساكن فراغ build_mark_map_one(بنية mark_سیٹ *from, بنية mark_سیٹ *منزل)
{
	for_each_mark(from, 0, داخل_کرو_mapped_mark, منزل);
}

ساكن فراغ build_mark_map(بنية string_list *from, بنية string_list *منزل)
{
	بنية string_list_شے *fromp, *چوٹی;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_شے(fromp, from) {
		چوٹی = string_list_lookup(منزل, fromp->string);
		إذا (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} وإلا إذا (!چوٹی || !چوٹی->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, چوٹی->util);
	}
}

ساكن بنية object_اندراج *parse_treeish_dataref(ثابت حرف **p)
{
	بنية object_id oid;
	بنية object_اندراج *e;

	إذا (**p == ':') {	/* <mark> */
		e = لبھو_mark(marks, parse_mark_ref_space(p));
		إذا (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} وإلا {	/* <sha1> */
		إذا (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = لبھو_object(&oid);
		إذا (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	طالما (!e || e->type != OBJ_درخت)
		e = dereference(e, &oid);
	أرجع e;
}

ساكن فراغ print_ls(صحيح منوال, ثابت بنانشانی حرف *hash, ثابت حرف *رستہ)
{
	ساكن بنية strbuf سطر = STRBUF_INIT;

	/* See show_tree(). */
	ثابت حرف *type =
		S_ISGITLINK(منوال) ? commit_type :
		S_ISDIR(منوال) ? درخت_type :
		blob_type;

	إذا (!منوال) {
		/* missing SP path LF */
		strbuf_reset(&سطر);
		strbuf_addstr(&سطر, "missing ");
		quote_c_style(رستہ, &سطر, عدم, 0);
		strbuf_addch(&سطر, '\n');
	} وإلا {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&سطر);
		strbuf_addf(&سطر, "%06o %s %s\t",
				منوال & ~NO_DELTA, type, hash_منزل_hex(hash));
		quote_c_style(رستہ, &سطر, عدم, 0);
		strbuf_addch(&سطر, '\n');
	}
	cat_blob_write(سطر.buf, سطر.len);
}

ساكن فراغ parse_ls(بنية fast_import_state *state بن_ورتیا,
		     ثابت حرف *p,
		     بنية branch *b)
{
	ساكن بنية strbuf رستہ = STRBUF_INIT;
	بنية درخت_اندراج *جڑ = عدم;
	بنية درخت_اندراج پتی = {عدم};

	/* ls SP (<tree-ish> SP)? <path> */
	إذا (*p == '"') {
		إذا (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		جڑ = &b->branch_درخت;
	} وإلا {
		بنية object_اندراج *e = parse_treeish_dataref(&p);
		جڑ = new_درخت_اندراج();
		oidcpy(&جڑ->versions[1].oid, &e->idx.oid);
		إذا (!is_null_oid(&جڑ->versions[1].oid))
			جڑ->versions[1].منوال = S_IFDIR;
		لدو_درخت(جڑ);
	}
	strbuf_reset(&رستہ);
	parse_رستہ_eol(&رستہ, p, "path");
	درخت_content_get(جڑ, رستہ.buf, &پتی, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	إذا (S_ISDIR(پتی.versions[1].منوال))
		store_درخت(&پتی);

	print_ls(پتی.versions[1].منوال, پتی.versions[1].oid.hash, رستہ.buf);
	إذا (پتی.درخت)
		release_درخت_content_recursive(پتی.درخت);
	إذا (!b || جڑ != &b->branch_درخت)
		release_درخت_اندراج(جڑ);
}

ساكن فراغ checkpoint(فراغ)
{
	checkpoint_requested = 0;
	إذا (object_گنتی) {
		چکر_packfile();
	}
	dump_branches();
	dump_ٹیگ_سوچی();
	dump_marks();
}

ساكن فراغ parse_checkpoint(بنية fast_import_state *state بن_ورتیا)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

ساكن فراغ parse_ترقی(بنية fast_import_state *state بن_ورتیا)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

ساكن فراغ parse_alias(بنية fast_import_state *state)
{
	بنية object_اندراج *e;
	بنية branch b;

	skip_optional_lf();
	read_اگلا_command(state);

	/* mark ... */
	parse_mark(state);
	إذا (!اگلا_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, حجم(b));
	إذا (!parse_objectish_with_اگیتر(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = لبھو_object(&b.oid);
	أكد(e);
	داخل_کرو_mark(&marks, اگلا_mark, e);
}

ساكن حرف* make_fast_import_رستہ(بنية fast_import_state *state,
				   ثابت حرف *رستہ)
{
	إذا (!relative_marks_paths || is_absolute_رستہ(رستہ))
		أرجع اگیتر_filename(state->اگیتر, رستہ);
	أرجع repo_git_رستہ(the_repository, "info/fast-import/%s", رستہ);
}

ساكن فراغ option_import_marks(بنية fast_import_state *state,
				ثابت حرف *marks,
				صحيح from_دھارا,
				صحيح ignore_missing)
{
	إذا (import_marks_file) {
		إذا (from_دھارا)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		إذا(!import_marks_file_from_دھارا)
			read_marks();
	}

	حرر(import_marks_file);
	import_marks_file = make_fast_import_رستہ(state, marks);
	import_marks_file_from_دھارا = from_دھارا;
	import_marks_file_ignore_missing = ignore_missing;
}

ساكن فراغ option_date_format(ثابت حرف *fmt)
{
	إذا (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	وإلا إذا (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	وإلا إذا (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	وإلا إذا (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	وإلا
		die(_("unknown --date-format argument %s"), fmt);
}

ساكن بنانشانی لما ulong_arg(ثابت حرف *option, ثابت حرف *arg)
{
	حرف *endptr;
	بنانشانی لما rv = strtoul(arg, &endptr, 0);
	إذا (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	أرجع rv;
}

ساكن فراغ option_ڈونگھائی(ثابت حرف *ڈونگھائی)
{
	max_ڈونگھائی = ulong_arg("--depth", ڈونگھائی);
	إذا (max_ڈونگھائی > MAX_ڈونگھائی)
		die(_("--depth cannot exceed %u"), MAX_ڈونگھائی);
}

ساكن فراغ option_سرگرم_branches(ثابت حرف *branches)
{
	max_سرگرم_branches = ulong_arg("--active-branches", branches);
}

ساكن فراغ option_export_marks(بنية fast_import_state *state,
				ثابت حرف *marks)
{
	حرر(export_marks_file);
	export_marks_file = make_fast_import_رستہ(state, marks);
}

ساكن فراغ option_cat_blob_fd(بنية fast_import_state *state بن_ورتیا,
			       ثابت حرف *fd)
{
	بنانشانی لما n = ulong_arg("--cat-blob-fd", fd);
	إذا (n > (بنانشانی لما) ودھ_توں_ودھ_عدد)
		die(_("--cat-blob-fd cannot exceed %d"), ودھ_توں_ودھ_عدد);
	cat_blob_fd = (صحيح) n;
}

ساكن فراغ option_export_pack_کنارے(بنية fast_import_state *state,
				     ثابت حرف *کنارے)
{
	حرف *fn = اگیتر_filename(state->اگیتر, کنارے);
	إذا (pack_کنارے)
		fclose(pack_کنارے);
	pack_کنارے = xfopen(fn, "a");
	حرر(fn);
}

ساكن فراغ option_rewrite_submodules(بنية fast_import_state *state,
				      ثابت حرف *arg,
				      بنية string_list *list)
{
	بنية mark_سیٹ *ms;
	FILE *fp;
	حرف *s = xstrdup(arg);
	حرف *f = strchr(s, ':');
	إذا (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = اگیتر_filename(state->اگیتر, f);
	fp = fopen(f, "r");
	إذا (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, داخل_کرو_oid_اندراج);
	fclose(fp);
	حرر(f);

	string_list_داخل_کرو(list, s)->util = ms;

	حرر(s);
}

ساكن فراغ option_max_pack_ناپ(ثابت حرف *arg)
{
	بنانشانی لما v;

	إذا (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	إذا (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} وإلا إذا (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

ساكن فراغ option_big_file_حد(ثابت حرف *arg)
{
	بنانشانی لما v;

	إذا (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_سیٹ_big_file_حد(the_repository, v);
}

ساكن فراغ option_signed_commits(ثابت حرف *arg)
{
	إذا (parse_sign_منوال(arg, &signed_commit_منوال, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

ساكن فراغ option_signed_ٹیگ_سوچی(ثابت حرف *arg)
{
	إذا (parse_sign_منوال(arg, &signed_ٹیگ_منوال, &signed_ٹیگ_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

ساكن فراغ option_quiet(فراغ)
{
	show_stats = 0;
	quiet = 1;
}

ساكن صحيح parse_one_option(بنية fast_import_state *state, ثابت حرف *option)
{
	إذا (skip_اگیتر(option, "max-pack-size=", &option)) {
		option_max_pack_ناپ(option);
	} وإلا إذا (skip_اگیتر(option, "big-file-threshold=", &option)) {
		option_big_file_حد(option);
	} وإلا إذا (skip_اگیتر(option, "depth=", &option)) {
		option_ڈونگھائی(option);
	} وإلا إذا (skip_اگیتر(option, "active-branches=", &option)) {
		option_سرگرم_branches(option);
	} وإلا إذا (skip_اگیتر(option, "export-pack-edges=", &option)) {
		option_export_pack_کنارے(state, option);
	} وإلا إذا (skip_اگیتر(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} وإلا إذا (skip_اگیتر(option, "signed-tags=", &option)) {
		option_signed_ٹیگ_سوچی(option);
	} وإلا إذا (!strcmp(option, "quiet")) {
		option_quiet();
	} وإلا إذا (!strcmp(option, "stats")) {
		show_stats = 1;
	} وإلا إذا (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} وإلا {
		أرجع 0;
	}

	أرجع 1;
}

ساكن فراغ check_unsafe_feature(بنية fast_import_state *state,
				 ثابت حرف *feature)
{
	إذا (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

ساكن صحيح parse_one_feature(بنية fast_import_state *state,
			     ثابت حرف *feature)
{
	ثابت حرف *arg;

	إذا (skip_اگیتر(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} وإلا إذا (skip_اگیتر(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} وإلا إذا (skip_اگیتر(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} وإلا إذا (skip_اگیتر(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} وإلا إذا (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} وإلا إذا (skip_اگیتر(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_منزل);
	} وإلا إذا (skip_اگیتر(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} وإلا إذا (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} وإلا إذا (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} وإلا إذا (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} وإلا إذا (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} وإلا إذا (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} وإلا إذا (!strcmp(feature, "force")) {
		force_update = 1;
	} وإلا إذا (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} وإلا {
		أرجع 0;
	}

	أرجع 1;
}

ساكن فراغ parse_feature(بنية fast_import_state *state, ثابت حرف *feature)
{
	إذا (state->تکیا_انکڑے_command)
		die(_("got feature command '%s' after data command"), feature);

	إذا (parse_one_feature(state, feature))
		أرجع;

	die(_("this version of fast-import does not support feature %s."), feature);
}

ساكن فراغ parse_option(بنية fast_import_state *state, ثابت حرف *option)
{
	إذا (state->تکیا_انکڑے_command)
		die(_("got option command '%s' after data command"), option);

	إذا (parse_one_option(state, option))
		أرجع;

	die(_("this version of fast-import does not support option: %s"), option);
}

ساكن فراغ git_pack_config(فراغ)
{
	صحيح indexversion_قدر;
	صحيح حد_بندی;
	بنانشانی لما packsizelimit_قدر;

	إذا (!repo_config_get_ulong(the_repository, "pack.depth", &max_ڈونگھائی)) {
		إذا (max_ڈونگھائی > MAX_ڈونگھائی)
			max_ڈونگھائی = MAX_ڈونگھائی;
	}
	إذا (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_قدر)) {
		pack_idx_opts.نسخہ = indexversion_قدر;
		إذا (pack_idx_opts.نسخہ > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.نسخہ);
	}
	إذا (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_قدر))
		max_packsize = packsizelimit_قدر;

	إذا (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &حد_بندی))
		unpack_حد_بندی = حد_بندی;
	وإلا إذا (!repo_config_get_int(the_repository, "transfer.unpacklimit", &حد_بندی))
		unpack_حد_بندی = حد_بندی;

	repo_config(the_repository, git_default_config, عدم);
}

ساكن ثابت حرف *ثابت fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	عدم
};

ساكن فراغ parse_argv(بنية fast_import_state *state)
{
	صحيح argc = parse_options(state->argc, state->argv, state->اگیتر,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	إذا (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->تکیا_انکڑے_command = 1;
	إذا (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_منزل);
}

ساكن صحيح option_parse_date_format(ثابت بنية option *opt بن_ورتیا,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	أرجع 0;
}

ساكن صحيح option_parse_export_pack_کنارے(ثابت بنية option *opt,
					  ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_کنارے(opt->قدر, arg);
	أرجع 0;
}

ساكن صحيح option_parse_max_pack_ناپ(ثابت بنية option *opt بن_ورتیا,
				      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_ناپ(arg);
	أرجع 0;
}

ساكن صحيح option_parse_big_file_حد(ثابت بنية option *opt بن_ورتیا,
					   ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_حد(arg);
	أرجع 0;
}

ساكن صحيح option_parse_signed_commits(ثابت بنية option *opt بن_ورتیا,
				       ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	أرجع 0;
}

ساكن صحيح option_parse_signed_ٹیگ_سوچی(ثابت بنية option *opt بن_ورتیا,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_ٹیگ_سوچی(arg);
	أرجع 0;
}

ساكن صحيح option_parse_rewrite_submodules_from(ثابت بنية option *opt,
						ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->قدر, arg, &sub_marks_from);
	أرجع 0;
}

ساكن صحيح option_parse_rewrite_submodules_منزل(ثابت بنية option *opt,
					      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->قدر, arg, &sub_marks_منزل);
	أرجع 0;
}

ساكن صحيح option_parse_cat_blob_fd(ثابت بنية option *opt,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->قدر, arg);
	أرجع 0;
}

ساكن صحيح option_parse_import_marks(ثابت بنية option *opt,
				     ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->قدر, arg, 0, 0);
	أرجع 0;
}

ساكن صحيح option_parse_import_marks_if_exists(ثابت بنية option *opt,
					       ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->قدر, arg, 0, 1);
	أرجع 0;
}

ساكن صحيح option_parse_export_marks(ثابت بنية option *opt,
				     ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->قدر, arg);
	أرجع 0;
}

ساكن صحيح option_parse_ڈونگھائی(ثابت بنية option *opt بن_ورتیا,
			      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_ڈونگھائی(arg);
	أرجع 0;
}

ساكن صحيح option_parse_سرگرم_branches(ثابت بنية option *opt بن_ورتیا,
					ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_سرگرم_branches(arg);
	أرجع 0;
}

ساكن صحيح option_parse_quiet(ثابت بنية option *opt بن_ورتیا,
			      ثابت حرف *arg بن_ورتیا, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	أرجع 0;
}

صحيح cmd_fast_import(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *اگیتر,
		    بنية repository *repo)
{
	بنية fast_import_state state;

	بنية option fast_import_options[] = {
		OPT_گروہ(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", عدم, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", عدم, عدم,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", عدم, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_ناپ),
		OPT_CALLBACK_F(0, "big-file-threshold", عدم, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_حد),
		OPT_CALLBACK_F(0, "depth", عدم, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_ڈونگھائی),
		OPT_CALLBACK_F(0, "active-branches", عدم, N_("n"),
			       N_("maximum number of branches to maintain active"),
			       PARSE_OPT_NONEG, option_parse_سرگرم_branches),
		OPT_گروہ(N_("Marks")),
		OPT_CALLBACK_F(0, "import-marks", &state, N_("file"),
			       N_("import marks from <file>"),
			       PARSE_OPT_NONEG, option_parse_import_marks),
		OPT_CALLBACK_F(0, "import-marks-if-exists", &state, N_("file"),
			       N_("import marks from <file> if it exists"),
			       PARSE_OPT_NONEG, option_parse_import_marks_if_exists),
		OPT_CALLBACK_F(0, "export-marks", &state, N_("file"),
			       N_("dump marks to <file>"),
			       PARSE_OPT_NONEG, option_parse_export_marks),
		OPT_BOOL(0, "relative-marks", &relative_marks_paths,
			 N_("are --(import|export)-marks= paths relative to '.git/info/fast-import'?")),
		OPT_گروہ(N_("Submodule rewrite")),
		OPT_CALLBACK_F(0, "rewrite-submodules-from", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> from <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_from),
		OPT_CALLBACK_F(0, "rewrite-submodules-to", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> to <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_منزل),
		OPT_گروہ(N_("Signing")),
		OPT_CALLBACK_F(0, "signed-commits", عدم, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", عدم, N_("mode"),
			       N_("how to handle signed tags"),
			       PARSE_OPT_NONEG, option_parse_signed_ٹیگ_سوچی),
		OPT_HIDDEN_گروہ(N_("Advanced")),
		OPT_BOOL_F(0, "allow-unsafe-features", &state.allow_unsafe_features,
			   N_("allow unsafe mark commands from the stream"),
			   PARSE_OPT_HIDDEN | PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "export-pack-edges", &state, N_("file"),
			       N_("dump edge commits to <file>"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_export_pack_کنارے),
		OPT_CALLBACK_F(0, "cat-blob-fd", &state, N_("fd"),
			       N_("write some responses to <fd> instead of stdout"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_cat_blob_fd),
		OPT_سرا()
	};

	show_usage_with_options_if_asked(argc, argv, fast_import_usage, fast_import_options);

	fast_import_state_init(&state, argc, argv, اگیتر, fast_import_options);

	reset_pack_idx_option(&pack_idx_opts);
	git_pack_config();

	alloc_objects(object_اندراج_alloc);
	strbuf_init(&command_buf, 0);
	CALLOC_ARRAY(atom_جدول, atom_جدول_sz);
	CALLOC_ARRAY(branch_جدول, branch_جدول_sz);
	CALLOC_ARRAY(avail_درخت_جدول, avail_درخت_جدول_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_سیٹ));

	hashmap_init(&object_جدول, object_اندراج_hashcmp, عدم, 0);

	/*
	 * We don't parse most options until after we've seen the set of
	 * "feature" lines at the start of the stream (which allows the command
	 * line to override stream data). But we must do an early parse of any
	 * command-line options that impact how we interpret the feature lines.
	 *
	 * NEEDSWORK: This scan only matches the exact "--allow-unsafe-features"
	 * spelling and stops at the first argument that doesn't start with a
	 * dash. As parse_options() below also accepts unambiguous abbreviations
	 * and values separated by a space from their option, the two disagree
	 * for command lines like "--allow-unsafe" or "--depth 5
	 * --allow-unsafe-features": parse_options() accepts the option, but
	 * this scan doesn't see it, so unsafe features from the stream are
	 * still refused. This errs on the safe side, but should be fixed by
	 * teaching this scan about the options that take a value.
	 */
	لكل (صحيح i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		إذا (*arg != '-' || !strcmp(arg, "--"))
			اكسر;
		إذا (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * حجم(*rc_free));
	لكل (بنانشانی صحيح i = 0; i < (cmd_save - 1); i++)
		rc_free[i].اگلا = &rc_free[i + 1];
	rc_free[cmd_save - 1].اگلا = عدم;

	شروع_packfile();
	سیٹ_die_routine(die_nicely);
	سیٹ_checkpoint_signal();
	طالما (read_اگلا_command(&state) != EOF) {
		ثابت حرف *v;
		إذا (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		وإلا إذا (skip_اگیتر(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		وإلا إذا (skip_اگیتر(command_buf.buf, "tag ", &v))
			parse_new_ٹیگ(&state, v);
		وإلا إذا (skip_اگیتر(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		وإلا إذا (skip_اگیتر(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, عدم);
		وإلا إذا (skip_اگیتر(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		وإلا إذا (skip_اگیتر(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		وإلا إذا (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		وإلا إذا (!strcmp("done", command_buf.buf))
			اكسر;
		وإلا إذا (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		وإلا إذا (starts_with(command_buf.buf, "progress "))
			parse_ترقی(&state);
		وإلا إذا (skip_اگیتر(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		وإلا إذا (skip_اگیتر(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		وإلا إذا (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		وإلا
			die(_("unsupported command: %s"), command_buf.buf);

		إذا (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	إذا (!state.تکیا_انکڑے_command)
		parse_argv(&state);

	إذا (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	سرا_packfile();

	dump_branches();
	dump_ٹیگ_سوچی();
	unkeep_all_packs();
	dump_marks();

	إذا (pack_کنارے)
		fclose(pack_کنارے);

	إذا (show_stats) {
		uintmax_t کل_گنتی = 0, duplicate_گنتی = 0;
		لكل (ناپ_t i = 0; i < ARRAY_ناپ(object_گنتی_by_type); i++)
			کل_گنتی += object_گنتی_by_type[i];
		لكل (ناپ_t i = 0; i < ARRAY_ناپ(duplicate_گنتی_by_type); i++)
			duplicate_گنتی += duplicate_گنتی_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_گنتی);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", کل_گنتی, duplicate_گنتی);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_گنتی_by_type[OBJ_BLOB], duplicate_گنتی_by_type[OBJ_BLOB], delta_گنتی_by_type[OBJ_BLOB], delta_گنتی_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_گنتی_by_type[OBJ_درخت], duplicate_گنتی_by_type[OBJ_درخت], delta_گنتی_by_type[OBJ_درخت], delta_گنتی_attempts_by_type[OBJ_درخت]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_گنتی_by_type[OBJ_COMMIT], duplicate_گنتی_by_type[OBJ_COMMIT], delta_گنتی_by_type[OBJ_COMMIT], delta_گنتی_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_گنتی_by_type[OBJ_ٹیگ], duplicate_گنتی_by_type[OBJ_ٹیگ], delta_گنتی_by_type[OBJ_ٹیگ], delta_گنتی_attempts_by_type[OBJ_ٹیگ]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_گنتی, branch_لدو_گنتی);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->کھسکاؤ) * 1024, marks_سیٹ_گنتی);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (درخت_اندراج_allocd + fi_mem_pool.pool_alloc + alloc_گنتی*حجم(بنية object_اندراج))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (بنانشانی لما)((درخت_اندراج_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_گنتی*حجم(بنية object_اندراج))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	أرجع failure ? 1 : 0;
}
