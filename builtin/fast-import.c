#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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

#تعریف_کن PACK_ID_بیتها 16
#تعریف_کن MAX_PACK_ID ((1<<PACK_ID_بیتها)-1)
#تعریف_کن عمق_بیتها 13
#تعریف_کن MAX_عمق ((1<<عمق_بیتها)-1)

/*
 * We abuse the setuid bit on directories to mean "do not delta".
 */
#تعریف_کن NO_DELTA S_ISUID

/*
 * The amount of additional space required in order to write an object into the
 * current pack. This is the hash lengths at the end of the pack, plus the
 * length of one object ID.
 */
#تعریف_کن PACK_اندازه_آستانه (the_hash_algo->rawsz * 3)

بنية object_مدخل {
	بنية pack_idx_مدخل idx;
	بنية hashmap_مدخل ent;
	uint32_t type : TYPE_بیتها,
		pack_id : PACK_ID_بیتها,
		عمق : عمق_بیتها;
};

ساكن صحيح object_مدخل_hashcmp(ثابت فراغ *map_داده استفادهنشده,
				ثابت بنية hashmap_مدخل *eptr,
				ثابت بنية hashmap_مدخل *مدخل_or_کلید,
				ثابت فراغ *keydata)
{
	ثابت بنية object_id *oid = keydata;
	ثابت بنية object_مدخل *e1, *e2;

	e1 = container_از(eptr, ثابت بنية object_مدخل, ent);
	إذا (oid)
		أرجع oidcmp(&e1->idx.oid, oid);

	e2 = container_از(مدخل_or_کلید, ثابت بنية object_مدخل, ent);
	أرجع oidcmp(&e1->idx.oid, &e2->idx.oid);
}

بنية object_مدخل_pool {
	بنية object_مدخل_pool *بعدی_pool;
	بنية object_مدخل *بعدی_free;
	بنية object_مدخل *انتها;
	بنية object_مدخل entries[FLEX_ARRAY]; /*بیشتر*/
};

بنية mark_مجموعه {
	اتحاد {
		بنية object_id *oids[1024];
		بنية object_مدخل *marked[1024];
		بنية mark_مجموعه *sets[1024];
	} داده;
	بیعلامت صحيح لغزش;
};

بنية آخرین_object {
	بنية strbuf داده;
	off_t جابه_جایی;
	بیعلامت صحيح عمق;
	بیعلامت no_جابجا : 1;
};

بنية atom_str {
	بنية atom_str *بعدی_atom;
	بیعلامت کوتاه str_len;
	حرف str_dat[FLEX_ARRAY]; /*بیشتر*/
};

بنية درخت_content;
بنية درخت_مدخل {
	بنية درخت_content *درخت;
	بنية atom_str *name;
	بنية درخت_مدخل_ms {
		uint16_t نما;
		بنية object_id oid;
	} versions[2];
};

بنية درخت_content {
	بیعلامت صحيح مدخل_ظرفیت; /* must match avail_tree_content */
	بیعلامت صحيح مدخل_شمار;
	بیعلامت صحيح delta_عمق;
	بنية درخت_مدخل *entries[FLEX_ARRAY]; /*بیشتر*/
};

بنية avail_درخت_content {
	بیعلامت صحيح مدخل_ظرفیت; /* must match tree_content */
	بنية avail_درخت_content *بعدی_avail;
};

بنية branch {
	بنية branch *جدول_بعدی_branch;
	بنية branch *فعال_بعدی_branch;
	ثابت حرف *name;
	بنية درخت_مدخل branch_درخت;
	uintmax_t آخرین_commit;
	uintmax_t رقم_notes;
	بیعلامت فعال : 1;
	بیعلامت delete : 1;
	بیعلامت pack_id : PACK_ID_بیتها;
	بنية object_id oid;
};

بنية برچسب {
	بنية برچسب *بعدی_برچسب;
	ثابت حرف *name;
	بیعلامت صحيح pack_id;
	بنية object_id oid;
};

بنية hash_list {
	بنية hash_list *بعدی;
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
	بنية recent_command *بعدی;
	حرف *buf;
};

تعريفنوع فراغ (*mark_مجموعه_inserter_t)(بنية mark_مجموعه **s, بنية object_id *oid, uintmax_t mark);
تعريفنوع فراغ (*each_mark_fn_t)(uintmax_t mark, فراغ *obj, فراغ *cbp);

/* Configured limits on output */
ساكن بیعلامت دراز max_عمق = 50;
ساكن off_t max_packsize;
ساكن صحيح unpack_حد = 100;
ساكن صحيح force_update;

/* Stats and misc. counters */
ساكن uintmax_t alloc_شمار;
ساكن uintmax_t marks_مجموعه_شمار;
ساكن uintmax_t object_شمار_by_type[1 << TYPE_بیتها];
ساكن uintmax_t duplicate_شمار_by_type[1 << TYPE_بیتها];
ساكن uintmax_t delta_شمار_by_type[1 << TYPE_بیتها];
ساكن uintmax_t delta_شمار_attempts_by_type[1 << TYPE_بیتها];
ساكن بیعلامت دراز object_شمار;
ساكن بیعلامت دراز branch_شمار;
ساكن بیعلامت دراز branch_بارگیری_شمار;
ساكن صحيح failure;
ساكن FILE *pack_یالها;
ساكن صحيح show_stats = 1;
ساكن صحيح quiet;
ساكن تعداد sign_نما signed_برچسب_نما = SIGN_VERBATIM;
ساكن تعداد sign_نما signed_commit_نما = SIGN_VERBATIM;
ساكن ثابت حرف *signed_commit_keyid;
ساكن ثابت حرف *signed_برچسب_keyid;

/* Memory pools */
ساكن بنية mem_pool fi_mem_pool = {
	.بلوک_alloc = 2*1024*1024 - حجم(بنية mp_بلوک),
};

/* Atom management */
ساكن بیعلامت صحيح atom_جدول_sz = 4451;
ساكن بیعلامت صحيح atom_cnt;
ساكن بنية atom_str **atom_جدول;

/* The .pack file being generated */
ساكن بنية pack_idx_option pack_idx_opts;
ساكن بیعلامت صحيح pack_id;
ساكن بنية hashfile *pack_file;
ساكن بنية packed_git *pack_داده;
ساكن بنية packed_git **all_packs;
ساكن off_t pack_اندازه;

/* Table of objects we've written. */
ساكن بیعلامت صحيح object_مدخل_alloc = 5000;
ساكن بنية object_مدخل_pool *بلوکها;
ساكن بنية hashmap object_جدول;
ساكن بنية mark_مجموعه *marks;
ساكن حرف *export_marks_file;
ساكن حرف *import_marks_file;
ساكن صحيح import_marks_file_from_جریان;
ساكن صحيح import_marks_file_ignore_missing;
ساكن صحيح import_marks_file_انجامشده;
ساكن صحيح relative_marks_paths;

/* Our last blob */
ساكن بنية آخرین_object آخرین_blob = {
	.داده = STRBUF_INIT,
 };

/* Tree management */
ساكن بیعلامت صحيح درخت_مدخل_alloc = 1000;
ساكن فراغ *avail_درخت_مدخل;
ساكن بیعلامت صحيح avail_درخت_جدول_sz = 100;
ساكن بنية avail_درخت_content **avail_درخت_جدول;
ساكن اندازه_t درخت_مدخل_allocd;
ساكن بنية strbuf old_درخت = STRBUF_INIT;
ساكن بنية strbuf new_درخت = STRBUF_INIT;

/* Branch data */
ساكن بیعلامت دراز max_فعال_branches = 5;
ساكن بیعلامت دراز cur_فعال_branches;
ساكن بیعلامت دراز branch_جدول_sz = 1039;
ساكن بنية branch **branch_جدول;
ساكن بنية branch *فعال_branches;

/* Tag data */
ساكن بنية برچسب *اولی_برچسب;
ساكن بنية برچسب *آخرین_برچسب;

/* Input stream parsing */
ساكن whenspec_type whenspec = WHENSPEC_RAW;
ساكن بنية strbuf command_buf = STRBUF_INIT;
ساكن صحيح unread_command_buf;
ساكن بنية recent_command cmd_hist = {
	.prev = &cmd_hist,
	.بعدی = &cmd_hist,
};
ساكن بنية recent_command *cmd_دم = &cmd_hist;
ساكن بنية recent_command *rc_free;
ساكن بیعلامت صحيح cmd_save = 100;
ساكن uintmax_t بعدی_mark;
ساكن بنية strbuf new_داده = STRBUF_INIT;
ساكن صحيح require_explicit_termination;

/* Signal handling */
ساكن متطاير sig_atomic_t checkpoint_requested;

/* Submodule marks */
ساكن بنية string_list sub_marks_from = STRING_LIST_INIT_DUP;
ساكن بنية string_list sub_marks_مقصد = STRING_LIST_INIT_DUP;
ساكن kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
ساكن صحيح cat_blob_fd = STDOUT_FILENO;

/* Command state */
بنية fast_import_state {
	صحيح argc;
	ثابت حرف **argv;
	ثابت حرف *پیشوند;
	صحيح دیده_داده_command;
	صحيح allow_unsafe_features;
	بنية option *option;
};

ساكن فراغ fast_import_state_init(بنية fast_import_state *state,
				   صحيح argc, ثابت حرف **argv,
				   ثابت حرف *پیشوند, بنية option *option)
{
	memset(state, 0, حجم(*state));
	state->argc = argc;
	state->argv = argv;
	state->پیشوند = پیشوند;
	state->option = option;
}

ساكن فراغ parse_argv(بنية fast_import_state *state);
ساكن فراغ parse_get_mark(بنية fast_import_state *state, ثابت حرف *p);
ساكن فراغ parse_cat_blob(بنية fast_import_state *state, ثابت حرف *p);
ساكن فراغ parse_ls(بنية fast_import_state *state,
		     ثابت حرف *p,
		     بنية branch *b);

ساكن فراغ for_each_mark(بنية mark_مجموعه *m, uintmax_t پایه, each_mark_fn_t callback, فراغ *p)
{
	uintmax_t k;
	إذا (m->لغزش) {
		لكل (k = 0; k < 1024; k++) {
			إذا (m->داده.sets[k])
				for_each_mark(m->داده.sets[k], پایه + (k << m->لغزش), callback, p);
		}
	} وإلا {
		لكل (k = 0; k < 1024; k++) {
			إذا (m->داده.marked[k])
				callback(پایه + k, m->داده.marked[k], p);
		}
	}
}

ساكن فراغ dump_marks_fn(uintmax_t mark, فراغ *object, فراغ *cbp) {
	بنية object_مدخل *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_مقصد_hex(&e->idx.oid));
}

ساكن فراغ write_branch_report(FILE *rpt, بنية branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	إذا (b->فعال)
		fputs(" active", rpt);
	إذا (b->branch_درخت.درخت)
		fputs(" loaded", rpt);
	إذا (is_null_oid(&b->branch_درخت.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_مقصد_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_مقصد_hex(&b->branch_درخت.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_مقصد_hex(&b->branch_درخت.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->آخرین_commit);

	fputs("  last pack   : ", rpt);
	إذا (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

ساكن فراغ write_crash_report(ثابت حرف *err)
{
	حرف *loc = repo_git_مسیر(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	بنية branch *b;
	بیعلامت دراز lu;
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
	fprintf(rpt, "    at %s\n", show_date(زمان(عدم), 0, DATE_نما(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	لكل (rc = cmd_hist.بعدی; rc != &cmd_hist; rc = rc->بعدی) {
		إذا (rc->بعدی == &cmd_hist)
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
		cur_فعال_branches,
		max_فعال_branches);
	fputc('\n', rpt);
	fputs("  pos  clock name\n", rpt);
	fputs("  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", rpt);
	لكل (b = فعال_branches, lu = 0; b; b = b->فعال_بعدی_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->آخرین_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	لكل (lu = 0; lu < branch_جدول_sz; lu++) {
		لكل (b = branch_جدول[lu]; b; b = b->جدول_بعدی_branch)
			write_branch_report(rpt, b);
	}

	إذا (اولی_برچسب) {
		بنية برچسب *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		لكل (tg = اولی_برچسب; tg; tg = tg->بعدی_برچسب) {
			fputs(oid_مقصد_hex(&tg->oid), rpt);
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

ساكن فراغ انتها_packfile(فراغ);
ساكن فراغ unkeep_all_packs(فراغ);
ساكن فراغ dump_marks(فراغ);

ساكن NORETURN فراغ die_nicely(ثابت حرف *err, va_list params)
{
	va_list cp;
	ساكن صحيح zombie;
	report_fn die_پیام_fn = get_die_پیام_routine();

	va_copy(cp, params);
	die_پیام_fn(err, params);

	إذا (!zombie) {
		حرف پیام_2[2 * مسیر_MAX];

		zombie = 1;
		vsnprintf(پیام_2, حجم(پیام_2), err, cp);
		write_crash_report(پیام_2);
		انتها_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	خروج(128);
}

#اگر_تعریف_نشده SIGUSR1	/* Windows, for example */

ساكن فراغ مجموعه_checkpoint_signal(فراغ)
{
}

#وإلا

ساكن فراغ checkpoint_signal(صحيح signo استفادهنشده)
{
	checkpoint_requested = 1;
}

ساكن فراغ مجموعه_checkpoint_signal(فراغ)
{
	بنية sigaction sa;

	memset(&sa, 0, حجم(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_ماسک);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, عدم);
}

#پایان_اگر

ساكن فراغ alloc_objects(بیعلامت صحيح cnt)
{
	بنية object_مدخل_pool *b;

	b = xmalloc(حجم(بنية object_مدخل_pool)
		+ cnt * حجم(بنية object_مدخل));
	b->بعدی_pool = بلوکها;
	b->بعدی_free = b->entries;
	b->انتها = b->entries + cnt;
	بلوکها = b;
	alloc_شمار += cnt;
}

ساكن بنية object_مدخل *new_object(بنية object_id *oid)
{
	بنية object_مدخل *e;

	إذا (بلوکها->بعدی_free == بلوکها->انتها)
		alloc_objects(object_مدخل_alloc);

	e = بلوکها->بعدی_free++;
	oidcpy(&e->idx.oid, oid);
	أرجع e;
}

ساكن بنية object_مدخل *پیدا_کن_object(بنية object_id *oid)
{
	أرجع hashmap_get_مدخل_from_hash(&object_جدول, oidhash(oid), oid,
					   بنية object_مدخل, ent);
}

ساكن بنية object_مدخل *درج_object(بنية object_id *oid)
{
	بنية object_مدخل *e;
	بیعلامت صحيح hash = oidhash(oid);

	e = hashmap_get_مدخل_from_hash(&object_جدول, hash, oid,
					بنية object_مدخل, ent);
	إذا (!e) {
		e = new_object(oid);
		e->idx.جابه_جایی = 0;
		hashmap_مدخل_init(&e->ent, hash);
		hashmap_جمع_کن(&object_جدول, &e->ent);
	}

	أرجع e;
}

ساكن فراغ invalidate_pack_id(بیعلامت صحيح id)
{
	بیعلامت دراز lu;
	بنية برچسب *t;
	بنية hashmap_iter iter;
	بنية object_مدخل *e;

	hashmap_for_each_مدخل(&object_جدول, &iter, e, ent) {
		إذا (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	لكل (lu = 0; lu < branch_جدول_sz; lu++) {
		بنية branch *b;

		لكل (b = branch_جدول[lu]; b; b = b->جدول_بعدی_branch)
			إذا (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	لكل (t = اولی_برچسب; t; t = t->بعدی_برچسب)
		إذا (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

ساكن بیعلامت صحيح hc_str(ثابت حرف *s, اندازه_t len)
{
	بیعلامت صحيح r = 0;
	طالما (len-- > 0)
		r = r * 31 + *s++;
	أرجع r;
}

ساكن فراغ درج_mark(بنية mark_مجموعه **بالاترین, uintmax_t idnum, بنية object_مدخل *oe)
{
	بنية mark_مجموعه *s = *بالاترین;

	طالما ((idnum >> s->لغزش) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_مجموعه));
		s->لغزش = (*بالاترین)->لغزش + 10;
		s->داده.sets[0] = *بالاترین;
		*بالاترین = s;
	}
	طالما (s->لغزش) {
		uintmax_t i = idnum >> s->لغزش;
		idnum -= i << s->لغزش;
		إذا (!s->داده.sets[i]) {
			s->داده.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_مجموعه));
			s->داده.sets[i]->لغزش = s->لغزش - 10;
		}
		s = s->داده.sets[i];
	}
	إذا (!s->داده.marked[idnum])
		marks_مجموعه_شمار++;
	s->داده.marked[idnum] = oe;
}

ساكن فراغ *پیدا_کن_mark(بنية mark_مجموعه *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	بنية object_مدخل *oe = عدم;
	إذا ((idnum >> s->لغزش) < 1024) {
		طالما (s && s->لغزش) {
			uintmax_t i = idnum >> s->لغزش;
			idnum -= i << s->لغزش;
			s = s->داده.sets[i];
		}
		إذا (s)
			oe = s->داده.marked[idnum];
	}
	إذا (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	أرجع oe;
}

ساكن بنية atom_str *مقصد_atom(ثابت حرف *s, بیعلامت کوتاه len)
{
	بیعلامت صحيح hc = hc_str(s, len) % atom_جدول_sz;
	بنية atom_str *c;

	لكل (c = atom_جدول[hc]; c; c = c->بعدی_atom)
		إذا (c->str_len == len && !مقایسه_رشته(s, c->str_dat, len))
			أرجع c;

	c = mem_pool_alloc(&fi_mem_pool, حجم(بنية atom_str) + len + 1);
	c->str_len = len;
	کپی_حافظه(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->بعدی_atom = atom_جدول[hc];
	atom_جدول[hc] = c;
	atom_cnt++;
	أرجع c;
}

ساكن بنية branch *lookup_branch(ثابت حرف *name)
{
	بیعلامت صحيح hc = hc_str(name, طول_رشته(name)) % branch_جدول_sz;
	بنية branch *b;

	لكل (b = branch_جدول[hc]; b; b = b->جدول_بعدی_branch)
		إذا (!strcmp(name, b->name))
			أرجع b;
	أرجع عدم;
}

ساكن بنية branch *new_branch(ثابت حرف *name)
{
	بیعلامت صحيح hc = hc_str(name, طول_رشته(name)) % branch_جدول_sz;
	بنية branch *b = lookup_branch(name);

	إذا (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	إذا (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->جدول_بعدی_branch = branch_جدول[hc];
	b->branch_درخت.versions[0].نما = S_IFDIR;
	b->branch_درخت.versions[1].نما = S_IFDIR;
	b->رقم_notes = 0;
	b->فعال = 0;
	b->pack_id = MAX_PACK_ID;
	branch_جدول[hc] = b;
	branch_شمار++;
	أرجع b;
}

ساكن بیعلامت صحيح hc_entries(بیعلامت صحيح cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	أرجع cnt < avail_درخت_جدول_sz ? cnt : avail_درخت_جدول_sz - 1;
}

ساكن بنية درخت_content *new_درخت_content(بیعلامت صحيح cnt)
{
	بنية avail_درخت_content *f, *l = عدم;
	بنية درخت_content *t;
	بیعلامت صحيح hc = hc_entries(cnt);

	لكل (f = avail_درخت_جدول[hc]; f; l = f, f = f->بعدی_avail)
		إذا (f->مدخل_ظرفیت >= cnt)
			اكسر;

	إذا (f) {
		إذا (l)
			l->بعدی_avail = f->بعدی_avail;
		وإلا
			avail_درخت_جدول[hc] = f->بعدی_avail;
	} وإلا {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, حجم(*t) + حجم(t->entries[0]) * cnt);
		f->مدخل_ظرفیت = cnt;
	}

	t = (بنية درخت_content*)f;
	t->مدخل_شمار = 0;
	t->delta_عمق = 0;
	أرجع t;
}

ساكن فراغ release_درخت_مدخل(بنية درخت_مدخل *e);
ساكن فراغ release_درخت_content(بنية درخت_content *t)
{
	بنية avail_درخت_content *f = (بنية avail_درخت_content*)t;
	بیعلامت صحيح hc = hc_entries(f->مدخل_ظرفیت);
	f->بعدی_avail = avail_درخت_جدول[hc];
	avail_درخت_جدول[hc] = f;
}

ساكن فراغ release_درخت_content_recursive(بنية درخت_content *t)
{
	بیعلامت صحيح i;
	لكل (i = 0; i < t->مدخل_شمار; i++)
		release_درخت_مدخل(t->entries[i]);
	release_درخت_content(t);
}

ساكن بنية درخت_content *grow_درخت_content(
	بنية درخت_content *t,
	صحيح amt)
{
	بنية درخت_content *r = new_درخت_content(t->مدخل_شمار + amt);
	r->مدخل_شمار = t->مدخل_شمار;
	r->delta_عمق = t->delta_عمق;
	COPY_ARRAY(r->entries, t->entries, t->مدخل_شمار);
	release_درخت_content(t);
	أرجع r;
}

ساكن بنية درخت_مدخل *new_درخت_مدخل(فراغ)
{
	بنية درخت_مدخل *e;

	إذا (!avail_درخت_مدخل) {
		بیعلامت صحيح n = درخت_مدخل_alloc;
		درخت_مدخل_allocd += n * حجم(بنية درخت_مدخل);
		ALLOC_ARRAY(e, n);
		avail_درخت_مدخل = e;
		طالما (n-- > 1) {
			*((فراغ**)e) = e + 1;
			e++;
		}
		*((فراغ**)e) = عدم;
	}

	e = avail_درخت_مدخل;
	avail_درخت_مدخل = *((فراغ**)e);
	أرجع e;
}

ساكن فراغ release_درخت_مدخل(بنية درخت_مدخل *e)
{
	إذا (e->درخت)
		release_درخت_content_recursive(e->درخت);
	*((فراغ**)e) = avail_درخت_مدخل;
	avail_درخت_مدخل = e;
}

ساكن بنية درخت_content *dup_درخت_content(بنية درخت_content *s)
{
	بنية درخت_content *d;
	بنية درخت_مدخل *a, *b;
	بیعلامت صحيح i;

	إذا (!s)
		أرجع عدم;
	d = new_درخت_content(s->مدخل_شمار);
	لكل (i = 0; i < s->مدخل_شمار; i++) {
		a = s->entries[i];
		b = new_درخت_مدخل();
		کپی_حافظه(b, a, حجم(*a));
		إذا (a->درخت && is_null_oid(&b->versions[1].oid))
			b->درخت = dup_درخت_content(a->درخت);
		وإلا
			b->درخت = عدم;
		d->entries[i] = b;
	}
	d->مدخل_شمار = s->مدخل_شمار;
	d->delta_عمق = s->delta_عمق;

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

	pack_داده = p;
	pack_اندازه = write_pack_سرآیند(pack_file, 0);
	object_شمار = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

ساكن ثابت حرف *create_شاخص(فراغ)
{
	ثابت حرف *tmpfile;
	بنية pack_idx_مدخل **idx, **c, **آخرین;
	بنية object_مدخل *e;
	بنية object_مدخل_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_شمار);
	c = idx;
	لكل (o = بلوکها; o; o = o->بعدی_pool)
		لكل (e = o->بعدی_free; e-- != o->entries;)
			إذا (pack_id == e->pack_id)
				*c++ = &e->idx;
	آخرین = idx + object_شمار;
	إذا (c != آخرین)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, عدم, idx, object_شمار,
				 &pack_idx_opts, pack_داده->hash);
	حرر(idx);
	أرجع tmpfile;
}

ساكن حرف *keep_pack(ثابت حرف *curr_شاخص_name)
{
	ساكن ثابت حرف *keep_پیام = "fast-import";
	بنية strbuf name = STRBUF_INIT;
	صحيح keep_fd;

	odb_pack_name(pack_داده->repo, &name, pack_داده->hash, "keep");
	keep_fd = امن_create_file_with_leading_directories(pack_داده->repo,
							    name.buf);
	إذا (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_پیام, طول_رشته(keep_پیام));
	إذا (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_داده->repo, &name, pack_داده->hash, "pack");
	إذا (finalize_object_file(pack_داده->repo, pack_داده->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_داده->repo, &name, pack_داده->hash, "idx");
	إذا (finalize_object_file(pack_داده->repo, curr_شاخص_name, name.buf))
		die(_("cannot store index file"));
	حرر((فراغ *)curr_شاخص_name);
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
	بنية فرزند_process unpack = فرزند_PROCESS_INIT;

	إذا (lseek(p->pack_fd, 0, SEEK_مجموعه) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.درون = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_مقصد_stderr = 1;
	strvec_افزودن(&unpack.آرگومانها, "unpack-objects");
	إذا (!show_stats)
		strvec_افزودن(&unpack.آرگومانها, "-q");

	أرجع پیاپی_command(&unpack);
}

ساكن فراغ انتها_packfile(فراغ)
{
	ساكن صحيح running;

	إذا (running || !pack_داده)
		أرجع;

	running = 1;
	clear_delta_پایه_cache();
	إذا (object_شمار) {
		بنية odb_منبع_files *files = odb_منبع_files_downcast(pack_داده->repo->objects->منبع);
		بنية packed_git *new_p;
		بنية object_id cur_pack_oid;
		حرف *idx_name;
		صحيح i;
		بنية branch *b;
		بنية برچسب *t;

		close_pack_windows(pack_داده);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_سرآیند_footer(the_hash_algo, pack_داده->pack_fd,
					 pack_داده->hash, pack_داده->pack_name,
					 object_شمار, cur_pack_oid.hash,
					 pack_اندازه);

		إذا (object_شمار <= unpack_حد) {
			إذا (!loosen_small_pack(pack_داده)) {
				invalidate_pack_id(pack_id);
				اذهب discard_pack;
			}
		}

		close(pack_داده->pack_fd);
		idx_name = keep_pack(create_شاخص());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_بارگیری_pack(files->dirs->packed, idx_name, 1);
		إذا (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		حرر(idx_name);

		/* Print the boundary */
		إذا (pack_یالها) {
			fprintf(pack_یالها, "%s:", new_p->pack_name);
			لكل (i = 0; i < branch_جدول_sz; i++) {
				لكل (b = branch_جدول[i]; b; b = b->جدول_بعدی_branch) {
					إذا (b->pack_id == pack_id)
						fprintf(pack_یالها, " %s",
							oid_مقصد_hex(&b->oid));
				}
			}
			لكل (t = اولی_برچسب; t; t = t->بعدی_برچسب) {
				إذا (t->pack_id == pack_id)
					fprintf(pack_یالها, " %s",
						oid_مقصد_hex(&t->oid));
			}
			fputc('\n', pack_یالها);
			fflush(pack_یالها);
		}

		pack_id++;
	}
	وإلا {
discard_pack:
		close(pack_داده->pack_fd);
		unlink_or_warn(pack_داده->pack_name);
	}
	FREE_AND_NULL(pack_داده);
	running = 0;

	/* We can't carry a delta across packfiles. */
	strbuf_release(&آخرین_blob.داده);
	آخرین_blob.جابه_جایی = 0;
	آخرین_blob.عمق = 0;
}

ساكن فراغ چرخه_packfile(فراغ)
{
	انتها_packfile();
	شروع_packfile();
}

ساكن صحيح store_object(
	تعداد object_type type,
	بنية strbuf *dat,
	بنية آخرین_object *آخرین,
	بنية object_id *oidout,
	uintmax_t mark)
{
	بنية odb_منبع_files *files = odb_منبع_files_downcast(the_repository->objects->منبع);
	فراغ *بیرون, *delta;
	بنية object_مدخل *e;
	بیعلامت حرف hdr[96];
	بنية object_id oid;
	بیعلامت دراز hdrlen, deltalen = 0;
	بنية git_hash_ctx c;
	git_zstream s;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	hdrlen = format_object_سرآیند((حرف *)hdr, حجم(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	إذا (oidout)
		oidcpy(oidout, &oid);

	e = درج_object(&oid);
	إذا (mark)
		درج_mark(&marks, mark, e);
	إذا (e->idx.جابه_جایی) {
		duplicate_شمار_by_type[type]++;
		أرجع 1;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بعدی) {
		إذا (!packfile_list_پیدا_کن_oid(packfile_store_get_packs(dir->packed), &oid))
			استمر;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.جابه_جایی = 1; /* just not zero! */
		duplicate_شمار_by_type[type]++;
		أرجع 1;
	}

	إذا (آخرین && آخرین->داده.len && آخرین->داده.buf && آخرین->عمق < max_عمق
		&& dat->len > the_hash_algo->rawsz) {
		اندازه_t deltalen_st;

		delta_شمار_attempts_by_type[type]++;
		delta = diff_delta(آخرین->داده.buf, آخرین->داده.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_اندازه_t_مقصد_ulong(deltalen_st);
	} وإلا
		delta = عدم;

	git_deflate_init(&s, cfg->pack_compression_level);
	إذا (delta) {
		s.بعدی_درون = delta;
		s.avail_درون = deltalen;
	} وإلا {
		s.بعدی_درون = (فراغ *)dat->buf;
		s.avail_درون = dat->len;
	}
	s.avail_بیرون = git_deflate_bound(&s, s.avail_درون);
	s.بعدی_بیرون = بیرون = xmalloc(s.avail_بیرون);
	طالما (git_deflate(&s, Z_پایان) == Z_OK)
		; /* nothing */
	git_deflate_انتها(&s);

	/* Determine if we should auto-checkpoint. */
	إذا ((max_packsize
		&& (pack_اندازه + PACK_اندازه_آستانه + s.مجموع_بیرون) > max_packsize)
		|| (pack_اندازه + PACK_اندازه_آستانه + s.مجموع_بیرون) < pack_اندازه) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		چرخه_packfile();

		/* We cannot carry a delta into the new pack. */
		إذا (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.بعدی_درون = (فراغ *)dat->buf;
			s.avail_درون = dat->len;
			s.avail_بیرون = git_deflate_bound(&s, s.avail_درون);
			s.بعدی_بیرون = بیرون = xrealloc(بیرون, s.avail_بیرون);
			طالما (git_deflate(&s, Z_پایان) == Z_OK)
				; /* nothing */
			git_deflate_انتها(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.جابه_جایی = pack_اندازه;
	object_شمار++;
	object_شمار_by_type[type]++;

	بررسی_چرخهای_32_begin(pack_file);

	إذا (delta) {
		off_t ofs = e->idx.جابه_جایی - آخرین->جابه_جایی;
		بیعلامت pos = حجم(hdr) - 1;

		delta_شمار_by_type[type]++;
		e->عمق = آخرین->عمق + 1;

		hdrlen = encode_درون_pack_object_سرآیند(hdr, حجم(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_اندازه += hdrlen;

		hdr[pos] = ofs & 127;
		طالما (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, حجم(hdr) - pos);
		pack_اندازه += حجم(hdr) - pos;
	} وإلا {
		e->عمق = 0;
		hdrlen = encode_درون_pack_object_سرآیند(hdr, حجم(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_اندازه += hdrlen;
	}

	hashwrite(pack_file, بیرون, s.مجموع_بیرون);
	pack_اندازه += s.مجموع_بیرون;

	e->idx.بررسی_چرخهای_32 = بررسی_چرخهای_32_انتها(pack_file);

	حرر(بیرون);
	حرر(delta);
	إذا (آخرین) {
		إذا (آخرین->no_جابجا) {
			آخرین->داده = *dat;
		} وإلا {
			strbuf_جابجا(&آخرین->داده, dat);
		}
		آخرین->جابه_جایی = e->idx.جابه_جایی;
		آخرین->عمق = e->عمق;
	}
	أرجع 0;
}

ساكن فراغ truncate_pack(بنية hashfile_checkpoint *checkpoint)
{
	إذا (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_اندازه = checkpoint->جابه_جایی;
}

ساكن فراغ جریان_blob(uintmax_t len, بنية object_id *oidout, uintmax_t mark)
{
	بنية odb_منبع_files *files = odb_منبع_files_downcast(the_repository->objects->منبع);
	اندازه_t درون_sz = 64 * 1024, بیرون_sz = 64 * 1024;
	بیعلامت حرف *درون_buf = xmalloc(درون_sz);
	بیعلامت حرف *بیرون_buf = xmalloc(بیرون_sz);
	بنية object_مدخل *e;
	بنية object_id oid;
	بیعلامت دراز hdrlen;
	off_t جابه_جایی;
	بنية git_hash_ctx c;
	git_zstream s;
	بنية hashfile_checkpoint checkpoint;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	صحيح وضعیت = Z_OK;

	/* Determine if we should auto-checkpoint. */
	إذا ((max_packsize
		&& (pack_اندازه + PACK_اندازه_آستانه + len) > max_packsize)
		|| (pack_اندازه + PACK_اندازه_آستانه + len) < pack_اندازه)
		چرخه_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	جابه_جایی = checkpoint.جابه_جایی;

	hdrlen = format_object_سرآیند((حرف *)بیرون_buf, بیرون_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, بیرون_buf, hdrlen);

	بررسی_چرخهای_32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_درون_pack_object_سرآیند(بیرون_buf, بیرون_sz, OBJ_BLOB, len);

	s.بعدی_بیرون = بیرون_buf + hdrlen;
	s.avail_بیرون = بیرون_sz - hdrlen;

	طالما (وضعیت != Z_جریان_انتها) {
		إذا (0 < len && !s.avail_درون) {
			اندازه_t cnt = درون_sz < len ? درون_sz : (اندازه_t)len;
			اندازه_t n = fread(درون_buf, 1, cnt, stdin);
			إذا (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, درون_buf, n);
			s.بعدی_درون = درون_buf;
			s.avail_درون = n;
			len -= n;
		}

		وضعیت = git_deflate(&s, len ? 0 : Z_پایان);

		إذا (!s.avail_بیرون || وضعیت == Z_جریان_انتها) {
			اندازه_t n = s.بعدی_بیرون - بیرون_buf;
			hashwrite(pack_file, بیرون_buf, n);
			pack_اندازه += n;
			s.بعدی_بیرون = بیرون_buf;
			s.avail_بیرون = بیرون_sz;
		}

		بدل (وضعیت) {
		حالة Z_OK:
		حالة Z_BUF_ERROR:
		حالة Z_جریان_انتها:
			استمر;
		افتراضي:
			die(_("unexpected deflate failure: %d"), وضعیت);
		}
	}
	git_deflate_انتها(&s);
	git_hash_final_oid(&oid, &c);

	إذا (oidout)
		oidcpy(oidout, &oid);

	e = درج_object(&oid);

	إذا (mark)
		درج_mark(&marks, mark, e);

	إذا (e->idx.جابه_جایی) {
		duplicate_شمار_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		اذهب بیرون;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بعدی) {
		إذا (!packfile_list_پیدا_کن_oid(packfile_store_get_packs(dir->packed), &oid))
			استمر;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.جابه_جایی = 1; /* just not zero! */
		duplicate_شمار_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		اذهب بیرون;
	}

	e->عمق = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.جابه_جایی = جابه_جایی;
	e->idx.بررسی_چرخهای_32 = بررسی_چرخهای_32_انتها(pack_file);
	object_شمار++;
	object_شمار_by_type[OBJ_BLOB]++;

بیرون:
	حرر(درون_buf);
	حرر(بیرون_buf);
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
ساكن فراغ *gfi_unpack_مدخل(
	بنية object_مدخل *oe,
	بیعلامت دراز *sizep)
{
	تعداد object_type type;
	اندازه_t اندازه_st = 0;
	فراغ *داده;
	بنية packed_git *p = all_packs[oe->pack_id];
	إذا (p == pack_داده && p->pack_اندازه < (pack_اندازه + the_hash_algo->rawsz)) {
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
		p->pack_اندازه = pack_اندازه + the_hash_algo->rawsz;
	}
	داده = unpack_مدخل(the_repository, p, oe->idx.جابه_جایی, &type, &اندازه_st);
	إذا (sizep)
		*sizep = cast_اندازه_t_مقصد_ulong(اندازه_st);
	أرجع داده;
}

ساكن فراغ بارگیری_درخت(بنية درخت_مدخل *ریشه)
{
	بنية object_id *oid = &ریشه->versions[1].oid;
	بنية object_مدخل *myoe;
	بنية درخت_content *t;
	بیعلامت دراز اندازه;
	حرف *buf;
	ثابت حرف *c;

	ریشه->درخت = t = new_درخت_content(8);
	إذا (is_null_oid(oid))
		أرجع;

	myoe = پیدا_کن_object(oid);
	إذا (myoe && myoe->pack_id != MAX_PACK_ID) {
		إذا (myoe->type != OBJ_درخت)
			die(_("not a tree: %s"), oid_مقصد_hex(oid));
		t->delta_عمق = myoe->عمق;
		buf = gfi_unpack_مدخل(myoe, &اندازه);
		إذا (!buf)
			die(_("can't load tree %s"), oid_مقصد_hex(oid));
	} وإلا {
		تعداد object_type type;
		اندازه_t اندازه_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &اندازه_st);
		اندازه = cast_اندازه_t_مقصد_ulong(اندازه_st);
		إذا (!buf || type != OBJ_درخت)
			die(_("can't load tree %s"), oid_مقصد_hex(oid));
	}

	c = buf;
	طالما (c != (buf + اندازه)) {
		بنية درخت_مدخل *e = new_درخت_مدخل();

		إذا (t->مدخل_شمار == t->مدخل_ظرفیت)
			ریشه->درخت = t = grow_درخت_content(t, t->مدخل_شمار);
		t->entries[t->مدخل_شمار++] = e;

		e->درخت = عدم;
		c = parse_نما(c, &e->versions[1].نما);
		إذا (!c)
			die(_("corrupt mode in %s"), oid_مقصد_hex(oid));
		e->versions[0].نما = e->versions[1].نما;
		e->name = مقصد_atom(c, طول_رشته(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (بیعلامت حرف *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (بیعلامت حرف *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	حرر(buf);
}

ساكن صحيح tecmp0 (ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية درخت_مدخل *a = *((بنية درخت_مدخل**)_a);
	بنية درخت_مدخل *b = *((بنية درخت_مدخل**)_b);
	أرجع پایه_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].نما,
		b->name->str_dat, b->name->str_len, b->versions[0].نما);
}

ساكن صحيح tecmp1 (ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية درخت_مدخل *a = *((بنية درخت_مدخل**)_a);
	بنية درخت_مدخل *b = *((بنية درخت_مدخل**)_b);
	أرجع پایه_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].نما,
		b->name->str_dat, b->name->str_len, b->versions[1].نما);
}

ساكن فراغ mktree(بنية درخت_content *t, صحيح v, بنية strbuf *b)
{
	اندازه_t maxlen = 0;
	بیعلامت صحيح i;

	إذا (!v)
		QSORT(t->entries, t->مدخل_شمار, tecmp0);
	وإلا
		QSORT(t->entries, t->مدخل_شمار, tecmp1);

	لكل (i = 0; i < t->مدخل_شمار; i++) {
		إذا (t->entries[i]->versions[v].نما)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	لكل (i = 0; i < t->مدخل_شمار; i++) {
		بنية درخت_مدخل *e = t->entries[i];
		إذا (!e->versions[v].نما)
			استمر;
		strbuf_addf(b, "%o %s%c",
			(بیعلامت صحيح)(e->versions[v].نما & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_جمع_کن(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

ساكن فراغ store_درخت(بنية درخت_مدخل *ریشه)
{
	بنية درخت_content *t;
	بیعلامت صحيح i, j, del;
	بنية آخرین_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	بنية object_مدخل *le = عدم;

	إذا (!is_null_oid(&ریشه->versions[1].oid))
		أرجع;

	إذا (!ریشه->درخت)
		بارگیری_درخت(ریشه);
	t = ریشه->درخت;

	لكل (i = 0; i < t->مدخل_شمار; i++) {
		إذا (t->entries[i]->درخت)
			store_درخت(t->entries[i]);
	}

	إذا (!(ریشه->versions[0].نما & NO_DELTA))
		le = پیدا_کن_object(&ریشه->versions[0].oid);
	إذا (S_ISDIR(ریشه->versions[0].نما) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_درخت);
		lo.داده = old_درخت;
		lo.جابه_جایی = le->idx.جابه_جایی;
		lo.عمق = t->delta_عمق;
	}

	mktree(t, 1, &new_درخت);
	store_object(OBJ_درخت, &new_درخت, &lo, &ریشه->versions[1].oid, 0);

	t->delta_عمق = lo.عمق;
	لكل (i = 0, j = 0, del = 0; i < t->مدخل_شمار; i++) {
		بنية درخت_مدخل *e = t->entries[i];
		إذا (e->versions[1].نما) {
			e->versions[0].نما = e->versions[1].نما;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} وإلا {
			release_درخت_مدخل(e);
			del++;
		}
	}
	t->مدخل_شمار -= del;
}

ساكن فراغ درخت_content_replace(
	بنية درخت_مدخل *ریشه,
	ثابت بنية object_id *oid,
	ثابت uint16_t نما,
	بنية درخت_content *newtree)
{
	إذا (!S_ISDIR(نما))
		die(_("root cannot be a non-directory"));
	oidclr(&ریشه->versions[0].oid, the_repository->hash_algo);
	oidcpy(&ریشه->versions[1].oid, oid);
	إذا (ریشه->درخت)
		release_درخت_content_recursive(ریشه->درخت);
	ریشه->درخت = newtree;
}

ساكن صحيح درخت_content_مجموعه(
	بنية درخت_مدخل *ریشه,
	ثابت حرف *p,
	ثابت بنية object_id *oid,
	ثابت uint16_t نما,
	بنية درخت_content *subtree)
{
	بنية درخت_content *t;
	ثابت حرف *slash1;
	بیعلامت صحيح i, n;
	بنية درخت_مدخل *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	إذا (!n)
		die(_("empty path component found in input"));
	إذا (!*slash1 && !S_ISDIR(نما) && subtree)
		die(_("non-directories cannot have subtrees"));

	إذا (!ریشه->درخت)
		بارگیری_درخت(ریشه);
	t = ریشه->درخت;
	لكل (i = 0; i < t->مدخل_شمار; i++) {
		e = t->entries[i];
		إذا (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			إذا (!*slash1) {
				إذا (!S_ISDIR(نما)
						&& e->versions[1].نما == نما
						&& oideq(&e->versions[1].oid, oid))
					أرجع 0;
				e->versions[1].نما = نما;
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
				إذا (S_ISDIR(e->versions[0].نما))
					e->versions[0].نما |= NO_DELTA;

				oidclr(&ریشه->versions[1].oid, the_repository->hash_algo);
				أرجع 1;
			}
			إذا (!S_ISDIR(e->versions[1].نما)) {
				e->درخت = new_درخت_content(8);
				e->versions[1].نما = S_IFDIR;
			}
			إذا (!e->درخت)
				بارگیری_درخت(e);
			إذا (درخت_content_مجموعه(e, slash1 + 1, oid, نما, subtree)) {
				oidclr(&ریشه->versions[1].oid, the_repository->hash_algo);
				أرجع 1;
			}
			أرجع 0;
		}
	}

	إذا (t->مدخل_شمار == t->مدخل_ظرفیت)
		ریشه->درخت = t = grow_درخت_content(t, t->مدخل_شمار);
	e = new_درخت_مدخل();
	e->name = مقصد_atom(p, n);
	e->versions[0].نما = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->مدخل_شمار++] = e;
	إذا (*slash1) {
		e->درخت = new_درخت_content(8);
		e->versions[1].نما = S_IFDIR;
		درخت_content_مجموعه(e, slash1 + 1, oid, نما, subtree);
	} وإلا {
		e->درخت = subtree;
		e->versions[1].نما = نما;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&ریشه->versions[1].oid, the_repository->hash_algo);
	أرجع 1;
}

ساكن صحيح درخت_content_remove(
	بنية درخت_مدخل *ریشه,
	ثابت حرف *p,
	بنية درخت_مدخل *backup_برگ,
	صحيح allow_ریشه)
{
	بنية درخت_content *t;
	ثابت حرف *slash1;
	بیعلامت صحيح i, n;
	بنية درخت_مدخل *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	إذا (!ریشه->درخت)
		بارگیری_درخت(ریشه);

	إذا (!*p && allow_ریشه) {
		e = ریشه;
		اذهب del_مدخل;
	}

	t = ریشه->درخت;
	لكل (i = 0; i < t->مدخل_شمار; i++) {
		e = t->entries[i];
		إذا (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			إذا (*slash1 && !S_ISDIR(e->versions[1].نما))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				أرجع 1;
			إذا (!*slash1 || !S_ISDIR(e->versions[1].نما))
				اذهب del_مدخل;
			إذا (!e->درخت)
				بارگیری_درخت(e);
			إذا (درخت_content_remove(e, slash1 + 1, backup_برگ, 0)) {
				لكل (n = 0; n < e->درخت->مدخل_شمار; n++) {
					إذا (e->درخت->entries[n]->versions[1].نما) {
						oidclr(&ریشه->versions[1].oid,
						       the_repository->hash_algo);
						أرجع 1;
					}
				}
				backup_برگ = عدم;
				اذهب del_مدخل;
			}
			أرجع 0;
		}
	}
	أرجع 0;

del_مدخل:
	إذا (backup_برگ)
		کپی_حافظه(backup_برگ, e, حجم(*backup_برگ));
	وإلا إذا (e->درخت)
		release_درخت_content_recursive(e->درخت);
	e->درخت = عدم;
	e->versions[1].نما = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&ریشه->versions[1].oid, the_repository->hash_algo);
	أرجع 1;
}

ساكن صحيح درخت_content_get(
	بنية درخت_مدخل *ریشه,
	ثابت حرف *p,
	بنية درخت_مدخل *برگ,
	صحيح allow_ریشه)
{
	بنية درخت_content *t;
	ثابت حرف *slash1;
	بیعلامت صحيح i, n;
	بنية درخت_مدخل *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	إذا (!n && !allow_ریشه)
		die(_("empty path component found in input"));

	إذا (!ریشه->درخت)
		بارگیری_درخت(ریشه);

	إذا (!n) {
		e = ریشه;
		اذهب یافت_مدخل;
	}

	t = ریشه->درخت;
	لكل (i = 0; i < t->مدخل_شمار; i++) {
		e = t->entries[i];
		إذا (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			إذا (!*slash1)
				اذهب یافت_مدخل;
			إذا (!S_ISDIR(e->versions[1].نما))
				أرجع 0;
			إذا (!e->درخت)
				بارگیری_درخت(e);
			أرجع درخت_content_get(e, slash1 + 1, برگ, 0);
		}
	}
	أرجع 0;

یافت_مدخل:
	کپی_حافظه(برگ, e, حجم(*برگ));
	إذا (e->درخت && is_null_oid(&e->versions[1].oid))
		برگ->درخت = dup_درخت_content(e->درخت);
	وإلا
		برگ->درخت = عدم;
	أرجع 1;
}

ساكن صحيح update_branch(بنية branch *b)
{
	ساكن ثابت حرف *پیام = "fast-import";
	بنية ref_transaction *transaction;
	بنية object_id old_oid;
	بنية strbuf err = STRBUF_INIT;
	ساكن ثابت حرف *replace_پیشوند = "refs/replace/";

	إذا (starts_with(b->name, replace_پیشوند) &&
	    !strcmp(b->name + طول_رشته(replace_پیشوند),
		    oid_مقصد_hex(&b->oid))) {
		إذا (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_مقصد_hex(&b->oid));
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

		ret = repo_درون_ادغام_bases(the_repository, old_cmit, new_cmit);
		إذا (ret < 0)
			خروج(128);
		إذا (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_مقصد_hex(&b->oid),
				oid_مقصد_hex(&old_oid));
			أرجع -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	إذا (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   عدم, عدم, 0, پیام, &err) ||
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
	بیعلامت صحيح i;
	بنية branch *b;

	لكل (i = 0; i < branch_جدول_sz; i++) {
		لكل (b = branch_جدول[i]; b; b = b->جدول_بعدی_branch)
			failure |= update_branch(b);
	}
}

ساكن فراغ dump_برچسبها(فراغ)
{
	ساكن ثابت حرف *پیام = "fast-import";
	بنية برچسب *t;
	بنية strbuf ref_name = STRBUF_INIT;
	بنية strbuf err = STRBUF_INIT;
	بنية ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	إذا (!transaction) {
		failure |= error("%s", err.buf);
		اذهب cleanup;
	}
	لكل (t = اولی_برچسب; t; t = t->بعدی_برچسب) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		إذا (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, عدم, عدم, عدم,
					   0, پیام, &err)) {
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
	بنية قفل_file mark_قفل = قفل_INIT;
	FILE *f;

	إذا (!export_marks_file || (import_marks_file && !import_marks_file_انجامشده))
		أرجع;

	إذا (امن_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		أرجع;
	}

	إذا (hold_قفل_file_for_update(&mark_قفل, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		أرجع;
	}

	f = fdopen_قفل_file(&mark_قفل, "w");
	إذا (!f) {
		صحيح saved_errno = errno;
		rollback_قفل_file(&mark_قفل);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		أرجع;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	إذا (commit_قفل_file(&mark_قفل)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		أرجع;
	}
}

ساكن فراغ درج_object_مدخل(بنية mark_مجموعه **s, بنية object_id *oid, uintmax_t mark)
{
	بنية object_مدخل *e;
	e = پیدا_کن_object(oid);
	إذا (!e) {
		تعداد object_type type = odb_read_object_info(the_repository->objects,
							     oid, عدم);
		إذا (type < 0)
			die(_("object not found: %s"), oid_مقصد_hex(oid));
		e = درج_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.جابه_جایی = 1; /* just not zero! */
	}
	درج_mark(s, mark, e);
}

ساكن فراغ درج_oid_مدخل(بنية mark_مجموعه **s, بنية object_id *oid, uintmax_t mark)
{
	درج_mark(s, mark, xmemdupz(oid, حجم(*oid)));
}

ساكن فراغ read_mark_file(بنية mark_مجموعه **s, FILE *f, mark_مجموعه_inserter_t inserter)
{
	حرف سطر[512];
	طالما (fgets(سطر, حجم(سطر), f)) {
		uintmax_t mark;
		حرف *انتها;
		بنية object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, حجم(oid.hash));

		انتها = strchr(سطر, '\n');
		إذا (سطر[0] != ':' || !انتها)
			die(_("corrupt mark line: %s"), سطر);
		*انتها = 0;
		mark = strtoumax(سطر + 1, &انتها, 10);
		إذا (!mark || انتها == سطر + 1
			|| *انتها != ' '
			|| get_oid_hex_any(انتها + 1, &oid) == GIT_HASH_UNKNOWN)
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
		اذهب انجامشده; /* Marks file does not exist */
	وإلا
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, درج_object_مدخل);
	fclose(f);
انجامشده:
	import_marks_file_انجامشده = 1;
}


ساكن صحيح read_بعدی_command(بنية fast_import_state *state)
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

			إذا (!state->دیده_داده_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			إذا (rc)
				rc_free = rc->بعدی;
			وإلا {
				rc = cmd_hist.بعدی;
				cmd_hist.بعدی = rc->بعدی;
				cmd_hist.بعدی->prev = &cmd_hist;
				حرر(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_دم;
			rc->بعدی = cmd_hist.prev;
			rc->prev->بعدی = rc;
			cmd_دم = rc;
		}
		إذا (command_buf.buf[0] == '#')
			استمر;
		أرجع 0;
	}
}

ساكن فراغ skip_optional_lf(فراغ)
{
	صحيح جمله_char = fgetc(stdin);
	إذا (جمله_char != '\n' && جمله_char != EOF)
		ungetc(جمله_char, stdin);
}

ساكن فراغ parse_mark(بنية fast_import_state *state)
{
	ثابت حرف *v;
	إذا (skip_پیشوند(command_buf.buf, "mark :", &v)) {
		بعدی_mark = strtoumax(v, عدم, 10);
		read_بعدی_command(state);
	}
	وإلا
		بعدی_mark = 0;
}

ساكن فراغ parse_original_identifier(بنية fast_import_state *state)
{
	ثابت حرف *v;
	إذا (skip_پیشوند(command_buf.buf, "original-oid ", &v))
		read_بعدی_command(state);
}

ساكن صحيح parse_داده(بنية strbuf *sb, uintmax_t حد, uintmax_t *len_res)
{
	ثابت حرف *داده;
	strbuf_reset(sb);

	إذا (!skip_پیشوند(command_buf.buf, "data ", &داده))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	إذا (skip_پیشوند(داده, "<<", &داده)) {
		حرف *جمله = xstrdup(داده);
		اندازه_t جمله_len = command_buf.len - (داده - command_buf.buf);

		لكل (;;) {
			إذا (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), جمله);
			إذا (جمله_len == command_buf.len
				&& !strcmp(جمله, command_buf.buf))
				اكسر;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		حرر(جمله);
	}
	وإلا {
		uintmax_t len = strtoumax(داده, عدم, 10);
		اندازه_t n = 0, درازی = (اندازه_t)len;

		إذا (حد && حد < len) {
			*len_res = len;
			أرجع 0;
		}
		إذا (درازی < len)
			die(_("data is too large to use in this context"));

		طالما (n < درازی) {
			اندازه_t s = strbuf_fread(sb, درازی - n, stdin);
			إذا (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(بیعلامت دراز)(درازی - n));
			n += s;
		}
	}

	skip_optional_lf();
	أرجع 1;
}

ساكن صحيح validate_raw_date(ثابت حرف *src, بنية strbuf *نتیجه, صحيح strict)
{
	ثابت حرف *orig_src = src;
	حرف *endp;
	بیعلامت دراز رقم;

	errno = 0;

	رقم = strtoul(src, &endp, 10);
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

	رقم = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	إذا (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < رقم))             /* parsed a broken timezone */
	   )
		أرجع -1;

	strbuf_addstr(نتیجه, orig_src);
	أرجع 0;
}

ساكن حرف *parse_ident(ثابت حرف *buf)
{
	ثابت حرف *ltgt;
	اندازه_t name_len;
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
	strbuf_جمع_کن(&ident, buf, name_len);

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
	بنية آخرین_object *آخرین,
	بنية object_id *oidout,
	uintmax_t mark)
{
	ساكن بنية strbuf buf = STRBUF_INIT;
	uintmax_t len;

	إذا (parse_داده(&buf, repo_settings_get_big_file_آستانه(the_repository), &len))
		store_object(OBJ_BLOB, &buf, آخرین, oidout, mark);
	وإلا {
		إذا (آخرین) {
			strbuf_release(&آخرین->داده);
			آخرین->جابه_جایی = 0;
			آخرین->عمق = 0;
		}
		جریان_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

ساكن فراغ parse_new_blob(بنية fast_import_state *state)
{
	read_بعدی_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&آخرین_blob, عدم, بعدی_mark);
}

ساكن فراغ unload_one_branch(فراغ)
{
	طالما (cur_فعال_branches
		&& cur_فعال_branches >= max_فعال_branches) {
		uintmax_t min_commit = ULONG_MAX;
		بنية branch *e, *l = عدم, *p = عدم;

		لكل (e = فعال_branches; e; e = e->فعال_بعدی_branch) {
			إذا (e->آخرین_commit < min_commit) {
				p = l;
				min_commit = e->آخرین_commit;
			}
			l = e;
		}

		إذا (p) {
			e = p->فعال_بعدی_branch;
			p->فعال_بعدی_branch = e->فعال_بعدی_branch;
		} وإلا {
			e = فعال_branches;
			فعال_branches = e->فعال_بعدی_branch;
		}
		e->فعال = 0;
		e->فعال_بعدی_branch = عدم;
		إذا (e->branch_درخت.درخت) {
			release_درخت_content_recursive(e->branch_درخت.درخت);
			e->branch_درخت.درخت = عدم;
		}
		cur_فعال_branches--;
	}
}

ساكن فراغ بارگیری_branch(بنية branch *b)
{
	بارگیری_درخت(&b->branch_درخت);
	إذا (!b->فعال) {
		b->فعال = 1;
		b->فعال_بعدی_branch = فعال_branches;
		فعال_branches = b;
		cur_فعال_branches++;
		branch_بارگیری_شمار++;
	}
}

ساكن بیعلامت حرف convert_رقم_notes_مقصد_fanout(uintmax_t رقم_notes)
{
	بیعلامت حرف fanout = 0;
	طالما ((رقم_notes >>= 8))
		fanout++;
	أرجع fanout;
}

ساكن فراغ construct_مسیر_with_fanout(ثابت حرف *hex_sha1,
		بیعلامت حرف fanout, حرف *مسیر)
{
	بیعلامت صحيح i = 0, j = 0;
	إذا (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	طالما (fanout) {
		مسیر[i++] = hex_sha1[j++];
		مسیر[i++] = hex_sha1[j++];
		مسیر[i++] = '/';
		fanout--;
	}
	کپی_حافظه(مسیر + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	مسیر[i + the_hash_algo->hexsz - j] = '\0';
}

ساكن uintmax_t do_change_note_fanout(
		بنية درخت_مدخل *orig_ریشه, بنية درخت_مدخل *ریشه,
		حرف *hex_oid, بیعلامت صحيح hex_oid_len,
		حرف *fullpath, بیعلامت صحيح fullpath_len,
		بیعلامت حرف fanout)
{
	بنية درخت_content *t;
	بنية درخت_مدخل *e, برگ;
	بیعلامت صحيح i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t رقم_notes = 0;
	بنية object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	حرف realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ثابت بیعلامت hexsz = the_hash_algo->hexsz;

	إذا (!ریشه->درخت)
		بارگیری_درخت(ریشه);
	t = ریشه->درخت;

	لكل (i = 0; t && i < t->مدخل_شمار; i++) {
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
		إذا (!e->versions[1].نما ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			استمر;

		/* This _may_ be a note entry, or a subdir containing notes */
		کپی_حافظه(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		إذا (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		کپی_حافظه(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		إذا (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			إذا (fanout == 0xff) {
				/* Counting mode, no rename */
				رقم_notes++;
				استمر;
			}
			construct_مسیر_with_fanout(hex_oid, fanout, realpath);
			إذا (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				رقم_notes++;
				استمر;
			}

			/* Rename fullpath to realpath */
			إذا (!درخت_content_remove(orig_ریشه, fullpath, &برگ, 0))
				die(_("failed to remove path %s"), fullpath);
			درخت_content_مجموعه(orig_ریشه, realpath,
				&برگ.versions[1].oid,
				برگ.versions[1].نما,
				برگ.درخت);
		} وإلا إذا (S_ISDIR(e->versions[1].نما)) {
			/* This is a subdir that may contain note entries */
			رقم_notes += do_change_note_fanout(orig_ریشه, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = ریشه->درخت;
	}
	أرجع رقم_notes;
}

ساكن uintmax_t change_note_fanout(بنية درخت_مدخل *ریشه,
		بیعلامت حرف fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	حرف hex_oid[GIT_MAX_HEXSZ], مسیر[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	أرجع do_change_note_fanout(ریشه, ریشه, hex_oid, 0, مسیر, 0, fanout);
}

ساكن صحيح parse_mapped_oid_hex(ثابت حرف *hex, بنية object_id *oid, ثابت حرف **انتها)
{
	صحيح algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, حجم(oid->hash));

	algo = parse_oid_hex_any(hex, oid, انتها);
	إذا (algo == GIT_HASH_UNKNOWN)
		أرجع -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	إذا (it == kh_انتها(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		إذا (hash_algos[algo].format_id == the_hash_algo->format_id)
			أرجع 0;
		أرجع -1;
	}
	oidcpy(oid, kh_مقدار(sub_oid_map, it));
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
	حرف *انتها;
	uintmax_t mark;

	mark = parse_mark_ref(p, &انتها);
	إذا (*انتها != '\0')
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
	حرف *انتها;

	mark = parse_mark_ref(*p, &انتها);
	إذا (*انتها++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = انتها;
	أرجع mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
ساكن فراغ parse_مسیر(بنية strbuf *sb, ثابت حرف *p, ثابت حرف **endp,
		صحيح is_آخرین_فیلد, ثابت حرف *فیلد)
{
	إذا (*p == '"') {
		إذا (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), فیلد, command_buf.buf);
		إذا (طول_رشته(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), فیلد, command_buf.buf);
	} وإلا {
		/*
		 * Unless we are parsing the last field of a line,
		 * SP is the end of this field.
		 */
		*endp = is_آخرین_فیلد
			? p + طول_رشته(p)
			: strchrnul(p, ' ');
		strbuf_جمع_کن(sb, p, *endp - p);
	}
}

/*
 * Parse the path string into the strbuf, and complain if this is not the end of
 * the string. Unquoted strings may contain spaces.
 */
ساكن فراغ parse_مسیر_eol(بنية strbuf *sb, ثابت حرف *p, ثابت حرف *فیلد)
{
	ثابت حرف *انتها;

	parse_مسیر(sb, p, &انتها, 1, فیلد);
	إذا (*انتها)
		die(_("garbage after %s: %s"), فیلد, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
ساكن فراغ parse_مسیر_space(بنية strbuf *sb, ثابت حرف *p,
		ثابت حرف **endp, ثابت حرف *فیلد)
{
	parse_مسیر(sb, p, endp, 0, فیلد);
	إذا (**endp != ' ')
		die(_("missing space after %s: %s"), فیلد, command_buf.buf);
	(*endp)++;
}

ساكن فراغ file_change_m(بنية fast_import_state *state,
			  ثابت حرف *p,
			  بنية branch *b)
{
	ساكن بنية strbuf مسیر = STRBUF_INIT;
	بنية object_مدخل *oe;
	بنية object_id oid;
	uint16_t نما, inline_داده = 0;

	p = parse_نما(p, &نما);
	إذا (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	بدل (نما) {
	حالة 0644:
	حالة 0755:
		نما |= S_IFREG;
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
		oe = پیدا_کن_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} وإلا إذا (skip_پیشوند(p, "inline ", &p)) {
		inline_داده = 1;
		oe = عدم; /* not used with inline_data, but makes gcc happy */
	} وإلا {
		إذا (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = پیدا_کن_object(&oid);
		إذا (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&مسیر);
	parse_مسیر_eol(&مسیر, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	إذا (S_ISDIR(نما) &&
	    is_خالی_درخت_oid(&oid, the_repository->hash_algo) &&
	    *مسیر.buf) {
		درخت_content_remove(&b->branch_درخت, مسیر.buf, عدم, 0);
		أرجع;
	}

	إذا (S_ISGITLINK(نما)) {
		إذا (inline_داده)
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
	} وإلا إذا (inline_داده) {
		إذا (S_ISDIR(نما))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		طالما (read_بعدی_command(state) != EOF) {
			ثابت حرف *v;
			إذا (skip_پیشوند(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			وإلا {
				parse_and_store_blob(&آخرین_blob, &oid, 0);
				اكسر;
			}
		}
	} وإلا {
		تعداد object_type expected = S_ISDIR(نما) ?
						OBJ_درخت: OBJ_BLOB;
		تعداد object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, عدم);
		إذا (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(نما) ?  _("tree") : _("blob"),
			    command_buf.buf);
		إذا (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	إذا (!*مسیر.buf) {
		درخت_content_replace(&b->branch_درخت, &oid, نما, عدم);
		أرجع;
	}

	إذا (!verify_مسیر(مسیر.buf, نما))
		die(_("invalid path '%s'"), مسیر.buf);
	درخت_content_مجموعه(&b->branch_درخت, مسیر.buf, &oid, نما, عدم);
}

ساكن فراغ file_change_d(ثابت حرف *p, بنية branch *b)
{
	ساكن بنية strbuf مسیر = STRBUF_INIT;

	strbuf_reset(&مسیر);
	parse_مسیر_eol(&مسیر, p, "path");
	درخت_content_remove(&b->branch_درخت, مسیر.buf, عدم, 1);
}

ساكن فراغ file_change_cr(ثابت حرف *p, بنية branch *b, صحيح rename)
{
	ساكن بنية strbuf منبع = STRBUF_INIT;
	ساكن بنية strbuf dest = STRBUF_INIT;
	بنية درخت_مدخل برگ;

	strbuf_reset(&منبع);
	parse_مسیر_space(&منبع, p, &p, "source");
	strbuf_reset(&dest);
	parse_مسیر_eol(&dest, p, "dest");

	memset(&برگ, 0, حجم(برگ));
	إذا (rename)
		درخت_content_remove(&b->branch_درخت, منبع.buf, &برگ, 1);
	وإلا
		درخت_content_get(&b->branch_درخت, منبع.buf, &برگ, 1);
	إذا (!برگ.versions[1].نما)
		die(_("path %s not in branch"), منبع.buf);
	إذا (!*dest.buf) {	/* C "path/to/subdir" "" */
		درخت_content_replace(&b->branch_درخت,
			&برگ.versions[1].oid,
			برگ.versions[1].نما,
			برگ.درخت);
		أرجع;
	}
	إذا (!verify_مسیر(dest.buf, برگ.versions[1].نما))
		die(_("invalid path '%s'"), dest.buf);
	درخت_content_مجموعه(&b->branch_درخت, dest.buf,
		&برگ.versions[1].oid,
		برگ.versions[1].نما,
		برگ.درخت);
}

ساكن فراغ note_change_n(بنية fast_import_state *state,
			  ثابت حرف *p,
			  بنية branch *b,
			  بیعلامت حرف *old_fanout)
{
	بنية object_مدخل *oe;
	بنية branch *s;
	بنية object_id oid, commit_oid;
	حرف مسیر[GIT_MAX_RAWSZ * 3];
	uint16_t inline_داده = 0;
	بیعلامت حرف new_fanout;

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
	إذا (b->رقم_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->رقم_notes = change_note_fanout(&b->branch_درخت, 0xff);
		*old_fanout = convert_رقم_notes_مقصد_fanout(b->رقم_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	إذا (*p == ':') {
		oe = پیدا_کن_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} وإلا إذا (skip_پیشوند(p, "inline ", &p)) {
		inline_داده = 1;
		oe = عدم; /* not used with inline_data, but makes gcc happy */
	} وإلا {
		إذا (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = پیدا_کن_object(&oid);
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
		بنية object_مدخل *commit_oe = پیدا_کن_mark(marks, commit_mark);
		إذا (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} وإلا إذا (!repo_get_oid(the_repository, p, &commit_oid)) {
		اندازه_t اندازه;
		حرف *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &اندازه,
						   &commit_oid);
		إذا (!buf || اندازه < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		حرر(buf);
	} وإلا
		die(_("invalid ref name or SHA1 expression: %s"), p);

	إذا (inline_داده) {
		read_بعدی_command(state);
		parse_and_store_blob(&آخرین_blob, &oid, 0);
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

	construct_مسیر_with_fanout(oid_مقصد_hex(&commit_oid), *old_fanout, مسیر);
	إذا (درخت_content_remove(&b->branch_درخت, مسیر, عدم, 0))
		b->رقم_notes--;

	إذا (is_null_oid(&oid))
		أرجع; /* nothing to insert */

	b->رقم_notes++;
	new_fanout = convert_رقم_notes_مقصد_fanout(b->رقم_notes);
	construct_مسیر_with_fanout(oid_مقصد_hex(&commit_oid), new_fanout, مسیر);
	درخت_content_مجموعه(&b->branch_درخت, مسیر, &oid, S_IFREG | 0644, عدم);
}

ساكن فراغ file_change_deleteall(بنية branch *b)
{
	release_درخت_content_recursive(b->branch_درخت.درخت);
	oidclr(&b->branch_درخت.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_درخت.versions[1].oid, the_repository->hash_algo);
	بارگیری_درخت(&b->branch_درخت);
	b->رقم_notes = 0;
}

ساكن فراغ parse_from_commit(بنية branch *b, حرف *buf, بیعلامت دراز اندازه)
{
	إذا (!buf || اندازه < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_مقصد_hex(&b->oid));
	إذا (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_درخت.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_مقصد_hex(&b->oid));
	oidcpy(&b->branch_درخت.versions[0].oid,
	       &b->branch_درخت.versions[1].oid);
}

ساكن فراغ parse_from_existing(بنية branch *b)
{
	إذا (is_null_oid(&b->oid)) {
		oidclr(&b->branch_درخت.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_درخت.versions[1].oid, the_repository->hash_algo);
	} وإلا {
		بیعلامت دراز اندازه;
		اندازه_t اندازه_st = 0;
		حرف *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &اندازه_st, &b->oid);
		اندازه = cast_اندازه_t_مقصد_ulong(اندازه_st);
		parse_from_commit(b, buf, اندازه);
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
		بنية object_مدخل *oe = پیدا_کن_mark(marks, idnum);
		إذا (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		إذا (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			إذا (oe->pack_id != MAX_PACK_ID) {
				بیعلامت دراز اندازه;
				حرف *buf = gfi_unpack_مدخل(oe, &اندازه);
				parse_from_commit(b, buf, اندازه);
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

	read_بعدی_command(state);
	أرجع 1;
}

ساكن صحيح parse_from(بنية fast_import_state *state, بنية branch *b)
{
	ثابت حرف *from;

	إذا (!skip_پیشوند(command_buf.buf, "from ", &from))
		أرجع 0;

	أرجع parse_objectish(state, b, from);
}

ساكن صحيح parse_objectish_with_پیشوند(بنية fast_import_state *state,
				       بنية branch *b,
				       ثابت حرف *پیشوند)
{
	ثابت حرف *پایه;

	إذا (!skip_پیشوند(command_buf.buf, پیشوند, &پایه))
		أرجع 0;

	أرجع parse_objectish(state, b, پایه);
}

ساكن بنية hash_list *parse_ادغام(بنية fast_import_state *state,
				     بیعلامت صحيح *شمار)
{
	بنية hash_list *list = عدم, **دم = &list, *n;
	ثابت حرف *from;
	بنية branch *s;

	*شمار = 0;
	طالما (skip_پیشوند(command_buf.buf, "merge ", &from)) {
		n = xmalloc(حجم(*n));
		s = lookup_branch(from);
		إذا (s)
			oidcpy(&n->oid, &s->oid);
		وإلا إذا (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			بنية object_مدخل *oe = پیدا_کن_mark(marks, idnum);
			إذا (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} وإلا إذا (!repo_get_oid(the_repository, from, &n->oid)) {
			اندازه_t اندازه;
			حرف *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &اندازه, &n->oid);
			إذا (!buf || اندازه < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			حرر(buf);
		} وإلا
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->بعدی = عدم;
		*دم = n;
		دم = &n->بعدی;

		(*شمار)++;
		read_بعدی_command(state);
	}
	أرجع list;
}

بنية signature_داده {
	حرف *hash_algo;      /* "sha1" or "sha256" */
	حرف *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	بنية strbuf داده;   /* The actual signature data */
};

ساكن فراغ parse_one_signature(بنية fast_import_state *state,
				بنية signature_داده *sig,
				ثابت حرف *v)
{
	حرف *آرگومانها = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	حرف *space = strchr(آرگومانها, ' ');

	إذا (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), آرگومانها);
	*space = '\0';

	sig->hash_algo = آرگومانها;
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
	read_بعدی_command(state);
	parse_داده(&sig->داده, 0, عدم);
}

ساكن فراغ discard_one_signature(بنية fast_import_state *state)
{
	بنية strbuf داده = STRBUF_INIT;

	read_بعدی_command(state);
	parse_داده(&داده, 0, عدم);
	strbuf_release(&داده);
}

ساكن فراغ جمع_کن_gpgsig_مقصد_commit(بنية strbuf *commit_داده,
				 ثابت حرف *سرآیند,
				 بنية signature_داده *sig)
{
	بنية string_list siglines = STRING_LIST_INIT_NODUP;

	إذا (!sig || !sig->hash_algo)
		أرجع;

	strbuf_addstr(commit_داده, سرآیند);
	string_list_تقسیم_درون_قراربده(&siglines, sig->داده.buf, "\n", -1);
	strbuf_جمع_کن_separated_string_list(commit_داده, "\n ", &siglines);
	strbuf_addch(commit_داده, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->داده);
	حرر(sig->hash_algo);
}

ساكن فراغ store_signature(بنية signature_داده *stored_sig,
			    بنية signature_داده *new_sig,
			    ثابت حرف *hash_type)
{
	إذا (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->داده);
		حرر(new_sig->hash_algo);
	} وإلا {
		*stored_sig = *new_sig;
	}
}

ساكن فراغ import_one_signature(بنية fast_import_state *state,
				 بنية signature_داده *sig_sha1,
				 بنية signature_داده *sig_sha256,
				 ثابت حرف *v)
{
	بنية signature_داده sig = { عدم, عدم, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	إذا (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	وإلا إذا (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	وإلا
		die(_("parse_one_signature() returned unknown hash algo"));
}

ساكن فراغ finalize_commit_میانگیر(بنية strbuf *new_داده,
				   بنية signature_داده *sig_sha1,
				   بنية signature_داده *sig_sha256,
				   بنية strbuf *پیام)
{
	جمع_کن_gpgsig_مقصد_commit(new_داده, "gpgsig ", sig_sha1);
	جمع_کن_gpgsig_مقصد_commit(new_داده, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_داده, '\n');
	strbuf_addbuf(new_داده, پیام);
}

ساكن فراغ warn_invalid_signature(بنية signature_check *check,
				   ثابت حرف *پیام, تعداد sign_نما نما)
{
	ثابت حرف *signer = check->signer ? check->signer : _("unknown");
	ثابت حرف *subject;
	صحيح subject_len = پیدا_کن_commit_subject(پیام, &subject);

	بدل (نما) {
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

ساكن فراغ handle_signature_if_invalid(بنية strbuf *new_داده,
					بنية signature_داده *sig_sha1,
					بنية signature_داده *sig_sha256,
					بنية strbuf *پیام,
					تعداد sign_نما نما)
{
	بنية strbuf tmp_buf = STRBUF_INIT;
	بنية signature_check signature_check = { 0 };
	صحيح ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_داده);
	finalize_commit_میانگیر(&tmp_buf, sig_sha1, sig_sha256, پیام);
	ret = verify_commit_میانگیر(tmp_buf.buf, tmp_buf.len, &signature_check);

	إذا (ret) {
		إذا (نما == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, پیام->buf, نما);

		إذا (نما == SIGN_SIGN_IF_INVALID) {
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
			إذا (sign_میانگیر(&payload, &signature, signed_commit_keyid,
					SIGN_میانگیر_USE_DEFAULT_کلید))
				die(_("failed to sign commit object"));
			جمع_کن_سرآیند_signature(new_داده, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_میانگیر(new_داده, عدم, عدم, پیام);
	} وإلا {
		strbuf_جابجا(new_داده, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

ساكن فراغ parse_new_commit(بنية fast_import_state *state, ثابت حرف *arg)
{
	ساكن بنية strbuf پیام = STRBUF_INIT;
	بنية signature_داده sig_sha1 = { عدم, عدم, STRBUF_INIT };
	بنية signature_داده sig_sha256 = { عدم, عدم, STRBUF_INIT };
	بنية branch *b;
	حرف *author = عدم;
	حرف *committer = عدم;
	حرف *encoding = عدم;
	بنية hash_list *ادغام_list = عدم;
	بیعلامت صحيح ادغام_شمار;
	بیعلامت حرف prev_fanout, new_fanout;
	ثابت حرف *v;

	b = lookup_branch(arg);
	إذا (!b)
		b = new_branch(arg);

	read_بعدی_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	إذا (skip_پیشوند(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_بعدی_command(state);
	}
	إذا (skip_پیشوند(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_بعدی_command(state);
	}
	إذا (!committer)
		die(_("expected committer but didn't get one"));

	طالما (skip_پیشوند(command_buf.buf, "gpgsig ", &v)) {
		بدل (signed_commit_نما) {

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
			BUG("invalid signed_commit_mode value %d", signed_commit_نما);
		}
		read_بعدی_command(state);
	}

	إذا (skip_پیشوند(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_بعدی_command(state);
	}
	parse_داده(&پیام, 0, عدم);
	read_بعدی_command(state);
	parse_from(state, b);
	ادغام_list = parse_ادغام(state, &ادغام_شمار);

	/* ensure the branch is active/loaded */
	إذا (!b->branch_درخت.درخت || !max_فعال_branches) {
		unload_one_branch();
		بارگیری_branch(b);
	}

	prev_fanout = convert_رقم_notes_مقصد_fanout(b->رقم_notes);

	/* file_change* */
	طالما (command_buf.len > 0) {
		إذا (skip_پیشوند(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		وإلا إذا (skip_پیشوند(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		وإلا إذا (skip_پیشوند(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		وإلا إذا (skip_پیشوند(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		وإلا إذا (skip_پیشوند(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		وإلا إذا (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		وإلا إذا (skip_پیشوند(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		وإلا إذا (skip_پیشوند(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		وإلا {
			unread_command_buf = 1;
			اكسر;
		}
		إذا (read_بعدی_command(state) == EOF)
			اكسر;
	}

	new_fanout = convert_رقم_notes_مقصد_fanout(b->رقم_notes);
	إذا (new_fanout != prev_fanout)
		b->رقم_notes = change_note_fanout(&b->branch_درخت, new_fanout);

	/* build the tree and the commit */
	store_درخت(&b->branch_درخت);
	oidcpy(&b->branch_درخت.versions[0].oid,
	       &b->branch_درخت.versions[1].oid);

	strbuf_reset(&new_داده);
	strbuf_addf(&new_داده, "tree %s\n",
		oid_مقصد_hex(&b->branch_درخت.versions[1].oid));
	إذا (!is_null_oid(&b->oid))
		strbuf_addf(&new_داده, "parent %s\n",
			    oid_مقصد_hex(&b->oid));
	طالما (ادغام_list) {
		بنية hash_list *بعدی = ادغام_list->بعدی;
		strbuf_addf(&new_داده, "parent %s\n",
			    oid_مقصد_hex(&ادغام_list->oid));
		حرر(ادغام_list);
		ادغام_list = بعدی;
	}
	strbuf_addf(&new_داده,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	إذا (encoding)
		strbuf_addf(&new_داده,
			"encoding %s\n",
			encoding);

	إذا ((signed_commit_نما == SIGN_STRIP_IF_INVALID ||
	     signed_commit_نما == SIGN_SIGN_IF_INVALID ||
	     signed_commit_نما == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_داده, &sig_sha1, &sig_sha256,
					    &پیام, signed_commit_نما);
	وإلا
		finalize_commit_میانگیر(&new_داده, &sig_sha1, &sig_sha256, &پیام);

	حرر(author);
	حرر(committer);
	حرر(encoding);

	إذا (!store_object(OBJ_COMMIT, &new_داده, عدم, &b->oid, بعدی_mark))
		b->pack_id = pack_id;
	b->آخرین_commit = object_شمار_by_type[OBJ_COMMIT];
}

ساكن فراغ handle_برچسب_signature_if_invalid(بنية strbuf *buf,
					    بنية strbuf *پیام,
					    اندازه_t sig_جابه_جایی)
{
	بنية strbuf signature = STRBUF_INIT;
	بنية strbuf payload = STRBUF_INIT;
	بنية signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_جمع_کن(&payload, پیام->buf, sig_جابه_جایی);
	strbuf_جمع_کن(&signature, پیام->buf + sig_جابه_جایی, پیام->len - sig_جابه_جایی);

	sigc.payload_type = SIGNATURE_PAYLOAD_برچسب;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	إذا (!check_signature(&sigc, signature.buf, signature.len))
		اذهب بیرون;

	إذا (signed_برچسب_نما == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(پیام, sig_جابه_جایی);

	إذا (signed_برچسب_نما == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = عدم;
		strbuf_reset(&signature);

		إذا (sign_میانگیر(&payload, &signature, signed_برچسب_keyid,
				SIGN_میانگیر_USE_DEFAULT_کلید))
			die(_("failed to sign tag object"));

		strbuf_addbuf(پیام, &signature);
	}

بیرون:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

ساكن فراغ handle_برچسب_signature(بنية strbuf *buf, بنية strbuf *پیام, ثابت حرف *name)
{
	اندازه_t sig_جابه_جایی = parse_signed_میانگیر(پیام->buf, پیام->len);

	/* If there is no signature, there is nothing to do. */
	إذا (sig_جابه_جایی >= پیام->len)
		أرجع;

	بدل (signed_برچسب_نما) {

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
		strbuf_setlen(پیام, sig_جابه_جایی);
		اكسر;
	حالة SIGN_ABORT_IF_INVALID:
	حالة SIGN_SIGN_IF_INVALID:
	حالة SIGN_STRIP_IF_INVALID:
		handle_برچسب_signature_if_invalid(buf, پیام, sig_جابه_جایی);
		اكسر;

	/* Third, aborting modes */
	حالة SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	افتراضي:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_برچسب_نما, name);
	}
}

ساكن فراغ parse_new_برچسب(بنية fast_import_state *state, ثابت حرف *arg)
{
	ساكن بنية strbuf پیام = STRBUF_INIT;
	ثابت حرف *from;
	حرف *tagger;
	بنية branch *s;
	بنية برچسب *t;
	uintmax_t from_mark = 0;
	بنية object_id oid;
	تعداد object_type type;
	ثابت حرف *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية برچسب));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	إذا (آخرین_برچسب)
		آخرین_برچسب->بعدی_برچسب = t;
	وإلا
		اولی_برچسب = t;
	آخرین_برچسب = t;
	read_بعدی_command(state);
	parse_mark(state);

	/* from ... */
	إذا (!skip_پیشوند(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	إذا (s) {
		إذا (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} وإلا إذا (*from == ':') {
		بنية object_مدخل *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = پیدا_کن_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} وإلا إذا (!repo_get_oid(the_repository, from, &oid)) {
		بنية object_مدخل *oe = پیدا_کن_object(&oid);
		إذا (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, عدم);
			إذا (type < 0)
				die(_("not a valid object: %s"), from);
		} وإلا
			type = oe->type;
	} وإلا
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_بعدی_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	إذا (skip_پیشوند(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_بعدی_command(state);
	} وإلا
		tagger = عدم;

	/* tag payload/message */
	parse_داده(&پیام, 0, عدم);

	/* build the tag object */
	strbuf_reset(&new_داده);

	strbuf_addf(&new_داده,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_مقصد_hex(&oid), type_name(type), t->name);
	إذا (tagger)
		strbuf_addf(&new_داده,
			    "tagger %s\n", tagger);

	handle_برچسب_signature(&new_داده, &پیام, t->name);

	strbuf_addch(&new_داده, '\n');
	strbuf_addbuf(&new_داده, &پیام);
	حرر(tagger);

	إذا (store_object(OBJ_برچسب, &new_داده, عدم, &t->oid, بعدی_mark))
		t->pack_id = MAX_PACK_ID;
	وإلا
		t->pack_id = pack_id;
}

ساكن فراغ parse_reset_branch(بنية fast_import_state *state, ثابت حرف *arg)
{
	بنية branch *b;
	ثابت حرف *برچسب_name;

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
	read_بعدی_command(state);
	parse_from(state, b);
	إذا (b->delete && skip_پیشوند(b->name, "refs/tags/", &برچسب_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		بنية برچسب *t, *prev = عدم;
		لكل (t = اولی_برچسب; t; t = t->بعدی_برچسب) {
			إذا (!strcmp(t->name, برچسب_name))
				اكسر;
			prev = t;
		}
		إذا (t) {
			إذا (prev)
				prev->بعدی_برچسب = t->بعدی_برچسب;
			وإلا
				اولی_برچسب = t->بعدی_برچسب;
			إذا (!t->بعدی_برچسب)
				آخرین_برچسب = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	إذا (command_buf.len > 0)
		unread_command_buf = 1;
}

ساكن فراغ cat_blob_write(ثابت حرف *buf, بیعلامت دراز اندازه)
{
	إذا (write_درون_پر(cat_blob_fd, buf, اندازه) < 0)
		die_errno(_("write to frontend failed"));
}

ساكن فراغ cat_blob(بنية object_مدخل *oe, بنية object_id *oid)
{
	بنية strbuf سطر = STRBUF_INIT;
	بنية iovec iov[3];
	بیعلامت دراز اندازه;
	تعداد object_type type = 0;
	حرف *buf;

	إذا (!oe || oe->pack_id == MAX_PACK_ID) {
		اندازه_t اندازه_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &اندازه_st);
		اندازه = cast_اندازه_t_مقصد_ulong(اندازه_st);
	} وإلا {
		type = oe->type;
		buf = gfi_unpack_مدخل(oe, &اندازه);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	إذا (type <= 0) {
		strbuf_reset(&سطر);
		strbuf_addf(&سطر, "%s missing\n", oid_مقصد_hex(oid));
		cat_blob_write(سطر.buf, سطر.len);
		strbuf_release(&سطر);
		حرر(buf);
		أرجع;
	}
	إذا (!buf)
		die(_("can't read object %s"), oid_مقصد_hex(oid));
	إذا (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_مقصد_hex(oid), type_name(type));
	strbuf_reset(&سطر);
	strbuf_addf(&سطر, "%s %s %"PRIuMAX"\n", oid_مقصد_hex(oid),
		    type_name(type), (uintmax_t)اندازه);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_پایه = سطر.buf;
	iov[0].iov_len = سطر.len;
	iov[1].iov_پایه = buf;
	iov[1].iov_len = اندازه;
	iov[2].iov_پایه = (فراغ *) "\n";
	iov[2].iov_len = 1;

	إذا (writev_درون_پر(cat_blob_fd, iov, ARRAY_اندازه(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&سطر);
	إذا (oe && oe->pack_id == pack_id) {
		آخرین_blob.جابه_جایی = oe->idx.جابه_جایی;
		strbuf_attach(&آخرین_blob.داده, buf, اندازه, اندازه + 1);
		آخرین_blob.عمق = oe->عمق;
	} وإلا
		حرر(buf);
}

ساكن فراغ parse_get_mark(بنية fast_import_state *state استفادهنشده,
			   ثابت حرف *p)
{
	بنية object_مدخل *oe;
	حرف output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	إذا (*p != ':')
		die(_("not a mark: %s"), p);

	oe = پیدا_کن_mark(marks, parse_mark_ref_eol(p));
	إذا (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, حجم(output), "%s\n", oid_مقصد_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

ساكن فراغ parse_cat_blob(بنية fast_import_state *state استفادهنشده,
			   ثابت حرف *p)
{
	بنية object_مدخل *oe;
	بنية object_id oid;

	/* cat-blob SP <object> LF */
	إذا (*p == ':') {
		oe = پیدا_کن_mark(marks, parse_mark_ref_eol(p));
		إذا (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} وإلا {
		إذا (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		إذا (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = پیدا_کن_object(&oid);
	}

	cat_blob(oe, &oid);
}

ساكن بنية object_مدخل *dereference(بنية object_مدخل *oe,
					بنية object_id *oid)
{
	بیعلامت دراز اندازه;
	حرف *buf = عدم;
	ثابت بیعلامت hexsz = the_hash_algo->hexsz;

	إذا (!oe) {
		تعداد object_type type = odb_read_object_info(the_repository->objects,
							     oid, عدم);
		إذا (type < 0)
			die(_("object not found: %s"), oid_مقصد_hex(oid));
		/* cache it! */
		oe = درج_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.جابه_جایی = 1;
	}
	بدل (oe->type) {
	حالة OBJ_درخت:	/* easy case. */
		أرجع oe;
	حالة OBJ_COMMIT:
	حالة OBJ_برچسب:
		اكسر;
	افتراضي:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	إذا (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_مدخل(oe, &اندازه);
	} وإلا {
		تعداد object_type استفادهنشده_2;
		اندازه_t اندازه_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &استفادهنشده_2, &اندازه_st);
		اندازه = cast_اندازه_t_مقصد_ulong(اندازه_st);
	}
	إذا (!buf)
		die(_("can't load object %s"), oid_مقصد_hex(oid));

	/* Peel one layer. */
	بدل (oe->type) {
	حالة OBJ_برچسب:
		إذا (اندازه < hexsz + طول_رشته("object ") ||
		    get_oid_hex(buf + طول_رشته("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		اكسر;
	حالة OBJ_COMMIT:
		إذا (اندازه < hexsz + طول_رشته("tree ") ||
		    get_oid_hex(buf + طول_رشته("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	حرر(buf);
	أرجع پیدا_کن_object(oid);
}

ساكن فراغ درج_mapped_mark(uintmax_t mark, فراغ *object, فراغ *cbp)
{
	بنية object_id *fromoid = object;
	بنية object_id *tooid = پیدا_کن_mark(cbp, mark);
	صحيح ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	إذا (ret == 0)
		أرجع;
	kh_مقدار(sub_oid_map, it) = tooid;
}

ساكن فراغ build_mark_map_one(بنية mark_مجموعه *from, بنية mark_مجموعه *مقصد)
{
	for_each_mark(from, 0, درج_mapped_mark, مقصد);
}

ساكن فراغ build_mark_map(بنية string_list *from, بنية string_list *مقصد)
{
	بنية string_list_قلم *fromp, *بالاترین;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_قلم(fromp, from) {
		بالاترین = string_list_lookup(مقصد, fromp->string);
		إذا (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} وإلا إذا (!بالاترین || !بالاترین->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, بالاترین->util);
	}
}

ساكن بنية object_مدخل *parse_treeish_dataref(ثابت حرف **p)
{
	بنية object_id oid;
	بنية object_مدخل *e;

	إذا (**p == ':') {	/* <mark> */
		e = پیدا_کن_mark(marks, parse_mark_ref_space(p));
		إذا (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} وإلا {	/* <sha1> */
		إذا (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = پیدا_کن_object(&oid);
		إذا (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	طالما (!e || e->type != OBJ_درخت)
		e = dereference(e, &oid);
	أرجع e;
}

ساكن فراغ print_ls(صحيح نما, ثابت بیعلامت حرف *hash, ثابت حرف *مسیر)
{
	ساكن بنية strbuf سطر = STRBUF_INIT;

	/* See show_tree(). */
	ثابت حرف *type =
		S_ISGITLINK(نما) ? commit_type :
		S_ISDIR(نما) ? درخت_type :
		blob_type;

	إذا (!نما) {
		/* missing SP path LF */
		strbuf_reset(&سطر);
		strbuf_addstr(&سطر, "missing ");
		quote_c_style(مسیر, &سطر, عدم, 0);
		strbuf_addch(&سطر, '\n');
	} وإلا {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&سطر);
		strbuf_addf(&سطر, "%06o %s %s\t",
				نما & ~NO_DELTA, type, hash_مقصد_hex(hash));
		quote_c_style(مسیر, &سطر, عدم, 0);
		strbuf_addch(&سطر, '\n');
	}
	cat_blob_write(سطر.buf, سطر.len);
}

ساكن فراغ parse_ls(بنية fast_import_state *state استفادهنشده,
		     ثابت حرف *p,
		     بنية branch *b)
{
	ساكن بنية strbuf مسیر = STRBUF_INIT;
	بنية درخت_مدخل *ریشه = عدم;
	بنية درخت_مدخل برگ = {عدم};

	/* ls SP (<tree-ish> SP)? <path> */
	إذا (*p == '"') {
		إذا (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		ریشه = &b->branch_درخت;
	} وإلا {
		بنية object_مدخل *e = parse_treeish_dataref(&p);
		ریشه = new_درخت_مدخل();
		oidcpy(&ریشه->versions[1].oid, &e->idx.oid);
		إذا (!is_null_oid(&ریشه->versions[1].oid))
			ریشه->versions[1].نما = S_IFDIR;
		بارگیری_درخت(ریشه);
	}
	strbuf_reset(&مسیر);
	parse_مسیر_eol(&مسیر, p, "path");
	درخت_content_get(ریشه, مسیر.buf, &برگ, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	إذا (S_ISDIR(برگ.versions[1].نما))
		store_درخت(&برگ);

	print_ls(برگ.versions[1].نما, برگ.versions[1].oid.hash, مسیر.buf);
	إذا (برگ.درخت)
		release_درخت_content_recursive(برگ.درخت);
	إذا (!b || ریشه != &b->branch_درخت)
		release_درخت_مدخل(ریشه);
}

ساكن فراغ checkpoint(فراغ)
{
	checkpoint_requested = 0;
	إذا (object_شمار) {
		چرخه_packfile();
	}
	dump_branches();
	dump_برچسبها();
	dump_marks();
}

ساكن فراغ parse_checkpoint(بنية fast_import_state *state استفادهنشده)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

ساكن فراغ parse_پیشرفت(بنية fast_import_state *state استفادهنشده)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

ساكن فراغ parse_alias(بنية fast_import_state *state)
{
	بنية object_مدخل *e;
	بنية branch b;

	skip_optional_lf();
	read_بعدی_command(state);

	/* mark ... */
	parse_mark(state);
	إذا (!بعدی_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, حجم(b));
	إذا (!parse_objectish_with_پیشوند(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = پیدا_کن_object(&b.oid);
	أكد(e);
	درج_mark(&marks, بعدی_mark, e);
}

ساكن حرف* make_fast_import_مسیر(بنية fast_import_state *state,
				   ثابت حرف *مسیر)
{
	إذا (!relative_marks_paths || is_absolute_مسیر(مسیر))
		أرجع پیشوند_filename(state->پیشوند, مسیر);
	أرجع repo_git_مسیر(the_repository, "info/fast-import/%s", مسیر);
}

ساكن فراغ option_import_marks(بنية fast_import_state *state,
				ثابت حرف *marks,
				صحيح from_جریان,
				صحيح ignore_missing)
{
	إذا (import_marks_file) {
		إذا (from_جریان)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		إذا(!import_marks_file_from_جریان)
			read_marks();
	}

	حرر(import_marks_file);
	import_marks_file = make_fast_import_مسیر(state, marks);
	import_marks_file_from_جریان = from_جریان;
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

ساكن بیعلامت دراز ulong_arg(ثابت حرف *option, ثابت حرف *arg)
{
	حرف *endptr;
	بیعلامت دراز rv = strtoul(arg, &endptr, 0);
	إذا (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	أرجع rv;
}

ساكن فراغ option_عمق(ثابت حرف *عمق)
{
	max_عمق = ulong_arg("--depth", عمق);
	إذا (max_عمق > MAX_عمق)
		die(_("--depth cannot exceed %u"), MAX_عمق);
}

ساكن فراغ option_فعال_branches(ثابت حرف *branches)
{
	max_فعال_branches = ulong_arg("--active-branches", branches);
}

ساكن فراغ option_export_marks(بنية fast_import_state *state,
				ثابت حرف *marks)
{
	حرر(export_marks_file);
	export_marks_file = make_fast_import_مسیر(state, marks);
}

ساكن فراغ option_cat_blob_fd(بنية fast_import_state *state استفادهنشده,
			       ثابت حرف *fd)
{
	بیعلامت دراز n = ulong_arg("--cat-blob-fd", fd);
	إذا (n > (بیعلامت دراز) بیشترین_صحیح)
		die(_("--cat-blob-fd cannot exceed %d"), بیشترین_صحیح);
	cat_blob_fd = (صحيح) n;
}

ساكن فراغ option_export_pack_یالها(بنية fast_import_state *state,
				     ثابت حرف *یالها)
{
	حرف *fn = پیشوند_filename(state->پیشوند, یالها);
	إذا (pack_یالها)
		fclose(pack_یالها);
	pack_یالها = xfopen(fn, "a");
	حرر(fn);
}

ساكن فراغ option_rewrite_submodules(بنية fast_import_state *state,
				      ثابت حرف *arg,
				      بنية string_list *list)
{
	بنية mark_مجموعه *ms;
	FILE *fp;
	حرف *s = xstrdup(arg);
	حرف *f = strchr(s, ':');
	إذا (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = پیشوند_filename(state->پیشوند, f);
	fp = fopen(f, "r");
	إذا (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, درج_oid_مدخل);
	fclose(fp);
	حرر(f);

	string_list_درج(list, s)->util = ms;

	حرر(s);
}

ساكن فراغ option_max_pack_اندازه(ثابت حرف *arg)
{
	بیعلامت دراز v;

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

ساكن فراغ option_big_file_آستانه(ثابت حرف *arg)
{
	بیعلامت دراز v;

	إذا (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_مجموعه_big_file_آستانه(the_repository, v);
}

ساكن فراغ option_signed_commits(ثابت حرف *arg)
{
	إذا (parse_sign_نما(arg, &signed_commit_نما, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

ساكن فراغ option_signed_برچسبها(ثابت حرف *arg)
{
	إذا (parse_sign_نما(arg, &signed_برچسب_نما, &signed_برچسب_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

ساكن فراغ option_quiet(فراغ)
{
	show_stats = 0;
	quiet = 1;
}

ساكن صحيح parse_one_option(بنية fast_import_state *state, ثابت حرف *option)
{
	إذا (skip_پیشوند(option, "max-pack-size=", &option)) {
		option_max_pack_اندازه(option);
	} وإلا إذا (skip_پیشوند(option, "big-file-threshold=", &option)) {
		option_big_file_آستانه(option);
	} وإلا إذا (skip_پیشوند(option, "depth=", &option)) {
		option_عمق(option);
	} وإلا إذا (skip_پیشوند(option, "active-branches=", &option)) {
		option_فعال_branches(option);
	} وإلا إذا (skip_پیشوند(option, "export-pack-edges=", &option)) {
		option_export_pack_یالها(state, option);
	} وإلا إذا (skip_پیشوند(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} وإلا إذا (skip_پیشوند(option, "signed-tags=", &option)) {
		option_signed_برچسبها(option);
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

	إذا (skip_پیشوند(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} وإلا إذا (skip_پیشوند(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} وإلا إذا (skip_پیشوند(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} وإلا إذا (skip_پیشوند(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} وإلا إذا (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} وإلا إذا (skip_پیشوند(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_مقصد);
	} وإلا إذا (skip_پیشوند(feature, "rewrite-submodules-from=", &arg)) {
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
	إذا (state->دیده_داده_command)
		die(_("got feature command '%s' after data command"), feature);

	إذا (parse_one_feature(state, feature))
		أرجع;

	die(_("this version of fast-import does not support feature %s."), feature);
}

ساكن فراغ parse_option(بنية fast_import_state *state, ثابت حرف *option)
{
	إذا (state->دیده_داده_command)
		die(_("got option command '%s' after data command"), option);

	إذا (parse_one_option(state, option))
		أرجع;

	die(_("this version of fast-import does not support option: %s"), option);
}

ساكن فراغ git_pack_config(فراغ)
{
	صحيح indexversion_مقدار;
	صحيح حد;
	بیعلامت دراز packsizelimit_مقدار;

	إذا (!repo_config_get_ulong(the_repository, "pack.depth", &max_عمق)) {
		إذا (max_عمق > MAX_عمق)
			max_عمق = MAX_عمق;
	}
	إذا (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_مقدار)) {
		pack_idx_opts.نسخه = indexversion_مقدار;
		إذا (pack_idx_opts.نسخه > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.نسخه);
	}
	إذا (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_مقدار))
		max_packsize = packsizelimit_مقدار;

	إذا (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &حد))
		unpack_حد = حد;
	وإلا إذا (!repo_config_get_int(the_repository, "transfer.unpacklimit", &حد))
		unpack_حد = حد;

	repo_config(the_repository, git_default_config, عدم);
}

ساكن ثابت حرف *ثابت fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	عدم
};

ساكن فراغ parse_argv(بنية fast_import_state *state)
{
	صحيح argc = parse_options(state->argc, state->argv, state->پیشوند,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	إذا (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->دیده_داده_command = 1;
	إذا (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_مقصد);
}

ساكن صحيح option_parse_date_format(ثابت بنية option *opt استفادهنشده,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	أرجع 0;
}

ساكن صحيح option_parse_export_pack_یالها(ثابت بنية option *opt,
					  ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_یالها(opt->مقدار, arg);
	أرجع 0;
}

ساكن صحيح option_parse_max_pack_اندازه(ثابت بنية option *opt استفادهنشده,
				      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_اندازه(arg);
	أرجع 0;
}

ساكن صحيح option_parse_big_file_آستانه(ثابت بنية option *opt استفادهنشده,
					   ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_آستانه(arg);
	أرجع 0;
}

ساكن صحيح option_parse_signed_commits(ثابت بنية option *opt استفادهنشده,
				       ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	أرجع 0;
}

ساكن صحيح option_parse_signed_برچسبها(ثابت بنية option *opt استفادهنشده,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_برچسبها(arg);
	أرجع 0;
}

ساكن صحيح option_parse_rewrite_submodules_from(ثابت بنية option *opt,
						ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->مقدار, arg, &sub_marks_from);
	أرجع 0;
}

ساكن صحيح option_parse_rewrite_submodules_مقصد(ثابت بنية option *opt,
					      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->مقدار, arg, &sub_marks_مقصد);
	أرجع 0;
}

ساكن صحيح option_parse_cat_blob_fd(ثابت بنية option *opt,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->مقدار, arg);
	أرجع 0;
}

ساكن صحيح option_parse_import_marks(ثابت بنية option *opt,
				     ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->مقدار, arg, 0, 0);
	أرجع 0;
}

ساكن صحيح option_parse_import_marks_if_exists(ثابت بنية option *opt,
					       ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->مقدار, arg, 0, 1);
	أرجع 0;
}

ساكن صحيح option_parse_export_marks(ثابت بنية option *opt,
				     ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->مقدار, arg);
	أرجع 0;
}

ساكن صحيح option_parse_عمق(ثابت بنية option *opt استفادهنشده,
			      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_عمق(arg);
	أرجع 0;
}

ساكن صحيح option_parse_فعال_branches(ثابت بنية option *opt استفادهنشده,
					ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_فعال_branches(arg);
	أرجع 0;
}

ساكن صحيح option_parse_quiet(ثابت بنية option *opt استفادهنشده,
			      ثابت حرف *arg استفادهنشده, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	أرجع 0;
}

صحيح cmd_fast_import(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *پیشوند,
		    بنية repository *repo)
{
	بنية fast_import_state state;

	بنية option fast_import_options[] = {
		OPT_گروه(N_("Common")),
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
			       PARSE_OPT_NONEG, option_parse_max_pack_اندازه),
		OPT_CALLBACK_F(0, "big-file-threshold", عدم, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_آستانه),
		OPT_CALLBACK_F(0, "depth", عدم, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_عمق),
		OPT_CALLBACK_F(0, "active-branches", عدم, N_("n"),
			       N_("maximum number of branches to maintain active"),
			       PARSE_OPT_NONEG, option_parse_فعال_branches),
		OPT_گروه(N_("Marks")),
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
		OPT_گروه(N_("Submodule rewrite")),
		OPT_CALLBACK_F(0, "rewrite-submodules-from", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> from <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_from),
		OPT_CALLBACK_F(0, "rewrite-submodules-to", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> to <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_مقصد),
		OPT_گروه(N_("Signing")),
		OPT_CALLBACK_F(0, "signed-commits", عدم, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", عدم, N_("mode"),
			       N_("how to handle signed tags"),
			       PARSE_OPT_NONEG, option_parse_signed_برچسبها),
		OPT_HIDDEN_گروه(N_("Advanced")),
		OPT_BOOL_F(0, "allow-unsafe-features", &state.allow_unsafe_features,
			   N_("allow unsafe mark commands from the stream"),
			   PARSE_OPT_HIDDEN | PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "export-pack-edges", &state, N_("file"),
			       N_("dump edge commits to <file>"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_export_pack_یالها),
		OPT_CALLBACK_F(0, "cat-blob-fd", &state, N_("fd"),
			       N_("write some responses to <fd> instead of stdout"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_cat_blob_fd),
		OPT_انتها()
	};

	show_usage_with_options_if_asked(argc, argv, fast_import_usage, fast_import_options);

	fast_import_state_init(&state, argc, argv, پیشوند, fast_import_options);

	reset_pack_idx_option(&pack_idx_opts);
	git_pack_config();

	alloc_objects(object_مدخل_alloc);
	strbuf_init(&command_buf, 0);
	CALLOC_ARRAY(atom_جدول, atom_جدول_sz);
	CALLOC_ARRAY(branch_جدول, branch_جدول_sz);
	CALLOC_ARRAY(avail_درخت_جدول, avail_درخت_جدول_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_مجموعه));

	hashmap_init(&object_جدول, object_مدخل_hashcmp, عدم, 0);

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
	لكل (بیعلامت صحيح i = 0; i < (cmd_save - 1); i++)
		rc_free[i].بعدی = &rc_free[i + 1];
	rc_free[cmd_save - 1].بعدی = عدم;

	شروع_packfile();
	مجموعه_die_routine(die_nicely);
	مجموعه_checkpoint_signal();
	طالما (read_بعدی_command(&state) != EOF) {
		ثابت حرف *v;
		إذا (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		وإلا إذا (skip_پیشوند(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		وإلا إذا (skip_پیشوند(command_buf.buf, "tag ", &v))
			parse_new_برچسب(&state, v);
		وإلا إذا (skip_پیشوند(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		وإلا إذا (skip_پیشوند(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, عدم);
		وإلا إذا (skip_پیشوند(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		وإلا إذا (skip_پیشوند(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		وإلا إذا (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		وإلا إذا (!strcmp("done", command_buf.buf))
			اكسر;
		وإلا إذا (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		وإلا إذا (starts_with(command_buf.buf, "progress "))
			parse_پیشرفت(&state);
		وإلا إذا (skip_پیشوند(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		وإلا إذا (skip_پیشوند(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		وإلا إذا (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		وإلا
			die(_("unsupported command: %s"), command_buf.buf);

		إذا (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	إذا (!state.دیده_داده_command)
		parse_argv(&state);

	إذا (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	انتها_packfile();

	dump_branches();
	dump_برچسبها();
	unkeep_all_packs();
	dump_marks();

	إذا (pack_یالها)
		fclose(pack_یالها);

	إذا (show_stats) {
		uintmax_t مجموع_شمار = 0, duplicate_شمار = 0;
		لكل (اندازه_t i = 0; i < ARRAY_اندازه(object_شمار_by_type); i++)
			مجموع_شمار += object_شمار_by_type[i];
		لكل (اندازه_t i = 0; i < ARRAY_اندازه(duplicate_شمار_by_type); i++)
			duplicate_شمار += duplicate_شمار_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_شمار);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", مجموع_شمار, duplicate_شمار);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_شمار_by_type[OBJ_BLOB], duplicate_شمار_by_type[OBJ_BLOB], delta_شمار_by_type[OBJ_BLOB], delta_شمار_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_شمار_by_type[OBJ_درخت], duplicate_شمار_by_type[OBJ_درخت], delta_شمار_by_type[OBJ_درخت], delta_شمار_attempts_by_type[OBJ_درخت]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_شمار_by_type[OBJ_COMMIT], duplicate_شمار_by_type[OBJ_COMMIT], delta_شمار_by_type[OBJ_COMMIT], delta_شمار_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_شمار_by_type[OBJ_برچسب], duplicate_شمار_by_type[OBJ_برچسب], delta_شمار_by_type[OBJ_برچسب], delta_شمار_attempts_by_type[OBJ_برچسب]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_شمار, branch_بارگیری_شمار);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->لغزش) * 1024, marks_مجموعه_شمار);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (درخت_مدخل_allocd + fi_mem_pool.pool_alloc + alloc_شمار*حجم(بنية object_مدخل))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (بیعلامت دراز)((درخت_مدخل_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_شمار*حجم(بنية object_مدخل))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	أرجع failure ? 1 : 0;
}
