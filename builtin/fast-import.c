#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "config.h"
#شامل "lockfile.h"
#شامل "object.h"
#شامل "blob.h"
#شامل "tree.h"
#شامل "commit.h"
#شامل "delta.h"
#شامل "pack.h"
#شامل "path.h"
#شامل "read-cache-ll.h"
#شامل "refs.h"
#شامل "csum-file.h"
#شامل "quote.h"
#شامل "dir.h"
#شامل "run-command.h"
#شامل "packfile.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "mem-pool.h"
#شامل "commit-reach.h"
#شامل "khash.h"
#شامل "date.h"
#شامل "gpg-interface.h"
#شامل "parse-options.h"

#تعریف_کرو PACK_ID_بٹ 16
#تعریف_کرو MAX_PACK_ID ((1<<PACK_ID_بٹ)-1)
#تعریف_کرو گہرائی_بٹ 13
#تعریف_کرو MAX_گہرائی ((1<<گہرائی_بٹ)-1)

/*
 * We abuse the setuid bit on directories to mean "do not delta".
 */
#تعریف_کرو NO_DELTA S_ISUID

/*
 * The amount of additional space required in order to write an object into the
 * current pack. This is the hash lengths at the end of the pack, plus the
 * length of one object ID.
 */
#تعریف_کرو PACK_سائز_حد (the_hash_algo->rawsz * 3)

ساخت object_اندراج {
	ساخت pack_idx_اندراج idx;
	ساخت hashmap_اندراج ent;
	uint32_t type : TYPE_بٹ,
		pack_id : PACK_ID_بٹ,
		گہرائی : گہرائی_بٹ;
};

ساکن صحیح object_اندراج_hashcmp(ثابت خالی *map_ڈیٹا غیر_مستعمل,
				ثابت ساخت hashmap_اندراج *eptr,
				ثابت ساخت hashmap_اندراج *اندراج_or_کلید,
				ثابت خالی *keydata)
{
	ثابت ساخت object_id *oid = keydata;
	ثابت ساخت object_اندراج *e1, *e2;

	e1 = container_کا(eptr, ثابت ساخت object_اندراج, ent);
	اگر (oid)
		واپس oidcmp(&e1->idx.oid, oid);

	e2 = container_کا(اندراج_or_کلید, ثابت ساخت object_اندراج, ent);
	واپس oidcmp(&e1->idx.oid, &e2->idx.oid);
}

ساخت object_اندراج_pool {
	ساخت object_اندراج_pool *اگلا_pool;
	ساخت object_اندراج *اگلا_free;
	ساخت object_اندراج *انتہا;
	ساخت object_اندراج entries[FLEX_ARRAY]; /*مزید*/
};

ساخت mark_سیٹ {
	اتحاد {
		ساخت object_id *oids[1024];
		ساخت object_اندراج *marked[1024];
		ساخت mark_سیٹ *sets[1024];
	} ڈیٹا;
	غیرعلامتی صحیح سرکاؤ;
};

ساخت آخری_object {
	ساخت strbuf ڈیٹا;
	off_t انحراف;
	غیرعلامتی صحیح گہرائی;
	غیرعلامتی no_تبادلہ : 1;
};

ساخت atom_str {
	ساخت atom_str *اگلا_atom;
	غیرعلامتی مختصر str_len;
	حرف str_dat[FLEX_ARRAY]; /*مزید*/
};

ساخت درخت_content;
ساخت درخت_اندراج {
	ساخت درخت_content *درخت;
	ساخت atom_str *name;
	ساخت درخت_اندراج_ms {
		uint16_t منوال;
		ساخت object_id oid;
	} versions[2];
};

ساخت درخت_content {
	غیرعلامتی صحیح اندراج_استعداد; /* must match avail_tree_content */
	غیرعلامتی صحیح اندراج_گنتی;
	غیرعلامتی صحیح delta_گہرائی;
	ساخت درخت_اندراج *entries[FLEX_ARRAY]; /*مزید*/
};

ساخت avail_درخت_content {
	غیرعلامتی صحیح اندراج_استعداد; /* must match tree_content */
	ساخت avail_درخت_content *اگلا_avail;
};

ساخت branch {
	ساخت branch *جدول_اگلا_branch;
	ساخت branch *فعال_اگلا_branch;
	ثابت حرف *name;
	ساخت درخت_اندراج branch_درخت;
	uintmax_t آخری_commit;
	uintmax_t عدد_notes;
	غیرعلامتی فعال : 1;
	غیرعلامتی delete : 1;
	غیرعلامتی pack_id : PACK_ID_بٹ;
	ساخت object_id oid;
};

ساخت لیبل {
	ساخت لیبل *اگلا_لیبل;
	ثابت حرف *name;
	غیرعلامتی صحیح pack_id;
	ساخت object_id oid;
};

ساخت hash_list {
	ساخت hash_list *اگلا;
	ساخت object_id oid;
};

قسمتعریف شمار {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

ساخت recent_command {
	ساخت recent_command *پچھلا_2;
	ساخت recent_command *اگلا;
	حرف *buf;
};

قسمتعریف خالی (*mark_سیٹ_inserter_t)(ساخت mark_سیٹ **s, ساخت object_id *oid, uintmax_t mark);
قسمتعریف خالی (*each_mark_fn_t)(uintmax_t mark, خالی *obj, خالی *cbp);

/* Configured limits on output */
ساکن غیرعلامتی طویل max_گہرائی = 50;
ساکن off_t max_packsize;
ساکن صحیح unpack_حد_بندی = 100;
ساکن صحیح force_update;

/* Stats and misc. counters */
ساکن uintmax_t alloc_گنتی;
ساکن uintmax_t marks_سیٹ_گنتی;
ساکن uintmax_t object_گنتی_by_type[1 << TYPE_بٹ];
ساکن uintmax_t duplicate_گنتی_by_type[1 << TYPE_بٹ];
ساکن uintmax_t delta_گنتی_by_type[1 << TYPE_بٹ];
ساکن uintmax_t delta_گنتی_attempts_by_type[1 << TYPE_بٹ];
ساکن غیرعلامتی طویل object_گنتی;
ساکن غیرعلامتی طویل branch_گنتی;
ساکن غیرعلامتی طویل branch_بھرو_گنتی;
ساکن صحیح failure;
ساکن FILE *pack_کنارے;
ساکن صحیح show_stats = 1;
ساکن صحیح quiet;
ساکن شمار sign_منوال signed_لیبل_منوال = SIGN_VERBATIM;
ساکن شمار sign_منوال signed_commit_منوال = SIGN_VERBATIM;
ساکن ثابت حرف *signed_commit_keyid;
ساکن ثابت حرف *signed_لیبل_keyid;

/* Memory pools */
ساکن ساخت mem_pool fi_mem_pool = {
	.بلاک_alloc = 2*1024*1024 - حجم(ساخت mp_بلاک),
};

/* Atom management */
ساکن غیرعلامتی صحیح atom_جدول_sz = 4451;
ساکن غیرعلامتی صحیح atom_cnt;
ساکن ساخت atom_str **atom_جدول;

/* The .pack file being generated */
ساکن ساخت pack_idx_option pack_idx_opts;
ساکن غیرعلامتی صحیح pack_id;
ساکن ساخت hashfile *pack_file;
ساکن ساخت packed_git *pack_ڈیٹا;
ساکن ساخت packed_git **all_packs;
ساکن off_t pack_سائز;

/* Table of objects we've written. */
ساکن غیرعلامتی صحیح object_اندراج_alloc = 5000;
ساکن ساخت object_اندراج_pool *بلاکیں;
ساکن ساخت hashmap object_جدول;
ساکن ساخت mark_سیٹ *marks;
ساکن حرف *export_marks_file;
ساکن حرف *import_marks_file;
ساکن صحیح import_marks_file_from_دھارا;
ساکن صحیح import_marks_file_ignore_missing;
ساکن صحیح import_marks_file_مکمل;
ساکن صحیح relative_marks_paths;

/* Our last blob */
ساکن ساخت آخری_object آخری_blob = {
	.ڈیٹا = STRBUF_INIT,
 };

/* Tree management */
ساکن غیرعلامتی صحیح درخت_اندراج_alloc = 1000;
ساکن خالی *avail_درخت_اندراج;
ساکن غیرعلامتی صحیح avail_درخت_جدول_sz = 100;
ساکن ساخت avail_درخت_content **avail_درخت_جدول;
ساکن سائز_t درخت_اندراج_allocd;
ساکن ساخت strbuf old_درخت = STRBUF_INIT;
ساکن ساخت strbuf new_درخت = STRBUF_INIT;

/* Branch data */
ساکن غیرعلامتی طویل max_فعال_branches = 5;
ساکن غیرعلامتی طویل cur_فعال_branches;
ساکن غیرعلامتی طویل branch_جدول_sz = 1039;
ساکن ساخت branch **branch_جدول;
ساکن ساخت branch *فعال_branches;

/* Tag data */
ساکن ساخت لیبل *پہلا_لیبل;
ساکن ساخت لیبل *آخری_لیبل;

/* Input stream parsing */
ساکن whenspec_type whenspec = WHENSPEC_RAW;
ساکن ساخت strbuf command_buf = STRBUF_INIT;
ساکن صحیح unread_command_buf;
ساکن ساخت recent_command cmd_hist = {
	.پچھلا_2 = &cmd_hist,
	.اگلا = &cmd_hist,
};
ساکن ساخت recent_command *cmd_دم = &cmd_hist;
ساکن ساخت recent_command *rc_free;
ساکن غیرعلامتی صحیح cmd_save = 100;
ساکن uintmax_t اگلا_mark;
ساکن ساخت strbuf new_ڈیٹا = STRBUF_INIT;
ساکن صحیح require_explicit_termination;

/* Signal handling */
ساکن متلون sig_atomic_t checkpoint_requested;

/* Submodule marks */
ساکن ساخت string_list sub_marks_from = STRING_LIST_INIT_DUP;
ساکن ساخت string_list sub_marks_مقصد = STRING_LIST_INIT_DUP;
ساکن kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
ساکن صحیح cat_blob_fd = STDOUT_FILENO;

/* Command state */
ساخت fast_import_state {
	صحیح argc;
	ثابت حرف **argv;
	ثابت حرف *سابقہ;
	صحیح نظر_آیا_ڈیٹا_command;
	صحیح allow_unsafe_features;
	ساخت option *option;
};

ساکن خالی fast_import_state_init(ساخت fast_import_state *state,
				   صحیح argc, ثابت حرف **argv,
				   ثابت حرف *سابقہ, ساخت option *option)
{
	memset(state, 0, حجم(*state));
	state->argc = argc;
	state->argv = argv;
	state->سابقہ = سابقہ;
	state->option = option;
}

ساکن خالی parse_argv(ساخت fast_import_state *state);
ساکن خالی parse_get_mark(ساخت fast_import_state *state, ثابت حرف *p);
ساکن خالی parse_cat_blob(ساخت fast_import_state *state, ثابت حرف *p);
ساکن خالی parse_ls(ساخت fast_import_state *state,
		     ثابت حرف *p,
		     ساخت branch *b);

ساکن خالی for_each_mark(ساخت mark_سیٹ *m, uintmax_t بنیاد, each_mark_fn_t callback, خالی *p)
{
	uintmax_t k;
	اگر (m->سرکاؤ) {
		برائے (k = 0; k < 1024; k++) {
			اگر (m->ڈیٹا.sets[k])
				for_each_mark(m->ڈیٹا.sets[k], بنیاد + (k << m->سرکاؤ), callback, p);
		}
	} ورنہ {
		برائے (k = 0; k < 1024; k++) {
			اگر (m->ڈیٹا.marked[k])
				callback(بنیاد + k, m->ڈیٹا.marked[k], p);
		}
	}
}

ساکن خالی dump_marks_fn(uintmax_t mark, خالی *object, خالی *cbp) {
	ساخت object_اندراج *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_مقصد_hex(&e->idx.oid));
}

ساکن خالی write_branch_report(FILE *rpt, ساخت branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	اگر (b->فعال)
		fputs(" active", rpt);
	اگر (b->branch_درخت.درخت)
		fputs(" loaded", rpt);
	اگر (is_null_oid(&b->branch_درخت.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_مقصد_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_مقصد_hex(&b->branch_درخت.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_مقصد_hex(&b->branch_درخت.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->آخری_commit);

	fputs("  last pack   : ", rpt);
	اگر (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

ساکن خالی write_crash_report(ثابت حرف *err)
{
	حرف *loc = repo_git_راستہ(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	ساخت branch *b;
	غیرعلامتی طویل lu;
	ساخت recent_command *rc;

	اگر (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		آزاد(loc);
		واپس;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(وقت(تہی), 0, DATE_منوال(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	برائے (rc = cmd_hist.اگلا; rc != &cmd_hist; rc = rc->اگلا) {
		اگر (rc->اگلا == &cmd_hist)
			fputs("* ", rpt);
		ورنہ
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
	برائے (b = فعال_branches, lu = 0; b; b = b->فعال_اگلا_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->آخری_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	برائے (lu = 0; lu < branch_جدول_sz; lu++) {
		برائے (b = branch_جدول[lu]; b; b = b->جدول_اگلا_branch)
			write_branch_report(rpt, b);
	}

	اگر (پہلا_لیبل) {
		ساخت لیبل *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		برائے (tg = پہلا_لیبل; tg; tg = tg->اگلا_لیبل) {
			fputs(oid_مقصد_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	اگر (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	ورنہ
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	آزاد(loc);
}

ساکن خالی انتہا_packfile(خالی);
ساکن خالی unkeep_all_packs(خالی);
ساکن خالی dump_marks(خالی);

ساکن NORETURN خالی die_nicely(ثابت حرف *err, va_list params)
{
	va_list cp;
	ساکن صحیح zombie;
	report_fn die_پیغام_fn = get_die_پیغام_routine();

	va_copy(cp, params);
	die_پیغام_fn(err, params);

	اگر (!zombie) {
		حرف پیغام_2[2 * راستہ_MAX];

		zombie = 1;
		vsnprintf(پیغام_2, حجم(پیغام_2), err, cp);
		write_crash_report(پیغام_2);
		انتہا_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	باہر_نکلو(128);
}

#اگر_غیر_تعریف_شدہ SIGUSR1	/* Windows, for example */

ساکن خالی سیٹ_checkpoint_signal(خالی)
{
}

#ورنہ

ساکن خالی checkpoint_signal(صحیح signo غیر_مستعمل)
{
	checkpoint_requested = 1;
}

ساکن خالی سیٹ_checkpoint_signal(خالی)
{
	ساخت sigaction sa;

	memset(&sa, 0, حجم(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_نقاب);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, تہی);
}

#اگر_ختم

ساکن خالی alloc_objects(غیرعلامتی صحیح cnt)
{
	ساخت object_اندراج_pool *b;

	b = xmalloc(حجم(ساخت object_اندراج_pool)
		+ cnt * حجم(ساخت object_اندراج));
	b->اگلا_pool = بلاکیں;
	b->اگلا_free = b->entries;
	b->انتہا = b->entries + cnt;
	بلاکیں = b;
	alloc_گنتی += cnt;
}

ساکن ساخت object_اندراج *new_object(ساخت object_id *oid)
{
	ساخت object_اندراج *e;

	اگر (بلاکیں->اگلا_free == بلاکیں->انتہا)
		alloc_objects(object_اندراج_alloc);

	e = بلاکیں->اگلا_free++;
	oidcpy(&e->idx.oid, oid);
	واپس e;
}

ساکن ساخت object_اندراج *ڈھونڈو_object(ساخت object_id *oid)
{
	واپس hashmap_get_اندراج_from_hash(&object_جدول, oidhash(oid), oid,
					   ساخت object_اندراج, ent);
}

ساکن ساخت object_اندراج *داخل_کرو_object(ساخت object_id *oid)
{
	ساخت object_اندراج *e;
	غیرعلامتی صحیح hash = oidhash(oid);

	e = hashmap_get_اندراج_from_hash(&object_جدول, hash, oid,
					ساخت object_اندراج, ent);
	اگر (!e) {
		e = new_object(oid);
		e->idx.انحراف = 0;
		hashmap_اندراج_init(&e->ent, hash);
		hashmap_جوڑو(&object_جدول, &e->ent);
	}

	واپس e;
}

ساکن خالی invalidate_pack_id(غیرعلامتی صحیح id)
{
	غیرعلامتی طویل lu;
	ساخت لیبل *t;
	ساخت hashmap_iter iter;
	ساخت object_اندراج *e;

	hashmap_for_each_اندراج(&object_جدول, &iter, e, ent) {
		اگر (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	برائے (lu = 0; lu < branch_جدول_sz; lu++) {
		ساخت branch *b;

		برائے (b = branch_جدول[lu]; b; b = b->جدول_اگلا_branch)
			اگر (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	برائے (t = پہلا_لیبل; t; t = t->اگلا_لیبل)
		اگر (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

ساکن غیرعلامتی صحیح hc_str(ثابت حرف *s, سائز_t len)
{
	غیرعلامتی صحیح r = 0;
	جب (len-- > 0)
		r = r * 31 + *s++;
	واپس r;
}

ساکن خالی داخل_کرو_mark(ساخت mark_سیٹ **چوٹی, uintmax_t idnum, ساخت object_اندراج *oe)
{
	ساخت mark_سیٹ *s = *چوٹی;

	جب ((idnum >> s->سرکاؤ) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, حجم(ساخت mark_سیٹ));
		s->سرکاؤ = (*چوٹی)->سرکاؤ + 10;
		s->ڈیٹا.sets[0] = *چوٹی;
		*چوٹی = s;
	}
	جب (s->سرکاؤ) {
		uintmax_t i = idnum >> s->سرکاؤ;
		idnum -= i << s->سرکاؤ;
		اگر (!s->ڈیٹا.sets[i]) {
			s->ڈیٹا.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, حجم(ساخت mark_سیٹ));
			s->ڈیٹا.sets[i]->سرکاؤ = s->سرکاؤ - 10;
		}
		s = s->ڈیٹا.sets[i];
	}
	اگر (!s->ڈیٹا.marked[idnum])
		marks_سیٹ_گنتی++;
	s->ڈیٹا.marked[idnum] = oe;
}

ساکن خالی *ڈھونڈو_mark(ساخت mark_سیٹ *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	ساخت object_اندراج *oe = تہی;
	اگر ((idnum >> s->سرکاؤ) < 1024) {
		جب (s && s->سرکاؤ) {
			uintmax_t i = idnum >> s->سرکاؤ;
			idnum -= i << s->سرکاؤ;
			s = s->ڈیٹا.sets[i];
		}
		اگر (s)
			oe = s->ڈیٹا.marked[idnum];
	}
	اگر (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	واپس oe;
}

ساکن ساخت atom_str *مقصد_atom(ثابت حرف *s, غیرعلامتی مختصر len)
{
	غیرعلامتی صحیح hc = hc_str(s, len) % atom_جدول_sz;
	ساخت atom_str *c;

	برائے (c = atom_جدول[hc]; c; c = c->اگلا_atom)
		اگر (c->str_len == len && !لڑی_موازنہ(s, c->str_dat, len))
			واپس c;

	c = mem_pool_alloc(&fi_mem_pool, حجم(ساخت atom_str) + len + 1);
	c->str_len = len;
	حافظہ_نقل(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->اگلا_atom = atom_جدول[hc];
	atom_جدول[hc] = c;
	atom_cnt++;
	واپس c;
}

ساکن ساخت branch *lookup_branch(ثابت حرف *name)
{
	غیرعلامتی صحیح hc = hc_str(name, لڑی_لمبائی(name)) % branch_جدول_sz;
	ساخت branch *b;

	برائے (b = branch_جدول[hc]; b; b = b->جدول_اگلا_branch)
		اگر (!strcmp(name, b->name))
			واپس b;
	واپس تہی;
}

ساکن ساخت branch *new_branch(ثابت حرف *name)
{
	غیرعلامتی صحیح hc = hc_str(name, لڑی_لمبائی(name)) % branch_جدول_sz;
	ساخت branch *b = lookup_branch(name);

	اگر (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	اگر (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, حجم(ساخت branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->جدول_اگلا_branch = branch_جدول[hc];
	b->branch_درخت.versions[0].منوال = S_IFDIR;
	b->branch_درخت.versions[1].منوال = S_IFDIR;
	b->عدد_notes = 0;
	b->فعال = 0;
	b->pack_id = MAX_PACK_ID;
	branch_جدول[hc] = b;
	branch_گنتی++;
	واپس b;
}

ساکن غیرعلامتی صحیح hc_entries(غیرعلامتی صحیح cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	واپس cnt < avail_درخت_جدول_sz ? cnt : avail_درخت_جدول_sz - 1;
}

ساکن ساخت درخت_content *new_درخت_content(غیرعلامتی صحیح cnt)
{
	ساخت avail_درخت_content *f, *l = تہی;
	ساخت درخت_content *t;
	غیرعلامتی صحیح hc = hc_entries(cnt);

	برائے (f = avail_درخت_جدول[hc]; f; l = f, f = f->اگلا_avail)
		اگر (f->اندراج_استعداد >= cnt)
			توڑو;

	اگر (f) {
		اگر (l)
			l->اگلا_avail = f->اگلا_avail;
		ورنہ
			avail_درخت_جدول[hc] = f->اگلا_avail;
	} ورنہ {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, حجم(*t) + حجم(t->entries[0]) * cnt);
		f->اندراج_استعداد = cnt;
	}

	t = (ساخت درخت_content*)f;
	t->اندراج_گنتی = 0;
	t->delta_گہرائی = 0;
	واپس t;
}

ساکن خالی release_درخت_اندراج(ساخت درخت_اندراج *e);
ساکن خالی release_درخت_content(ساخت درخت_content *t)
{
	ساخت avail_درخت_content *f = (ساخت avail_درخت_content*)t;
	غیرعلامتی صحیح hc = hc_entries(f->اندراج_استعداد);
	f->اگلا_avail = avail_درخت_جدول[hc];
	avail_درخت_جدول[hc] = f;
}

ساکن خالی release_درخت_content_recursive(ساخت درخت_content *t)
{
	غیرعلامتی صحیح i;
	برائے (i = 0; i < t->اندراج_گنتی; i++)
		release_درخت_اندراج(t->entries[i]);
	release_درخت_content(t);
}

ساکن ساخت درخت_content *grow_درخت_content(
	ساخت درخت_content *t,
	صحیح amt)
{
	ساخت درخت_content *r = new_درخت_content(t->اندراج_گنتی + amt);
	r->اندراج_گنتی = t->اندراج_گنتی;
	r->delta_گہرائی = t->delta_گہرائی;
	COPY_ARRAY(r->entries, t->entries, t->اندراج_گنتی);
	release_درخت_content(t);
	واپس r;
}

ساکن ساخت درخت_اندراج *new_درخت_اندراج(خالی)
{
	ساخت درخت_اندراج *e;

	اگر (!avail_درخت_اندراج) {
		غیرعلامتی صحیح n = درخت_اندراج_alloc;
		درخت_اندراج_allocd += n * حجم(ساخت درخت_اندراج);
		ALLOC_ARRAY(e, n);
		avail_درخت_اندراج = e;
		جب (n-- > 1) {
			*((خالی**)e) = e + 1;
			e++;
		}
		*((خالی**)e) = تہی;
	}

	e = avail_درخت_اندراج;
	avail_درخت_اندراج = *((خالی**)e);
	واپس e;
}

ساکن خالی release_درخت_اندراج(ساخت درخت_اندراج *e)
{
	اگر (e->درخت)
		release_درخت_content_recursive(e->درخت);
	*((خالی**)e) = avail_درخت_اندراج;
	avail_درخت_اندراج = e;
}

ساکن ساخت درخت_content *dup_درخت_content(ساخت درخت_content *s)
{
	ساخت درخت_content *d;
	ساخت درخت_اندراج *a, *b;
	غیرعلامتی صحیح i;

	اگر (!s)
		واپس تہی;
	d = new_درخت_content(s->اندراج_گنتی);
	برائے (i = 0; i < s->اندراج_گنتی; i++) {
		a = s->entries[i];
		b = new_درخت_اندراج();
		حافظہ_نقل(b, a, حجم(*a));
		اگر (a->درخت && is_null_oid(&b->versions[1].oid))
			b->درخت = dup_درخت_content(a->درخت);
		ورنہ
			b->درخت = تہی;
		d->entries[i] = b;
	}
	d->اندراج_گنتی = s->اندراج_گنتی;
	d->delta_گہرائی = s->delta_گہرائی;

	واپس d;
}

ساکن خالی آغاز_packfile(خالی)
{
	ساخت strbuf tmp_file = STRBUF_INIT;
	ساخت packed_git *p;
	صحیح pack_fd;

	pack_fd = odb_mkstemp(the_repository->objects, &tmp_file,
			      "pack/tmp_pack_XXXXXX");
	FLEX_ALLOC_STR(p, pack_name, tmp_file.buf);
	strbuf_release(&tmp_file);

	p->pack_fd = pack_fd;
	p->do_not_close = 1;
	p->repo = the_repository;
	pack_file = hashfd(the_repository->hash_algo, pack_fd, p->pack_name);

	pack_ڈیٹا = p;
	pack_سائز = write_pack_سرخی(pack_file, 0);
	object_گنتی = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

ساکن ثابت حرف *create_اشاریہ(خالی)
{
	ثابت حرف *tmpfile;
	ساخت pack_idx_اندراج **idx, **c, **آخری;
	ساخت object_اندراج *e;
	ساخت object_اندراج_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_گنتی);
	c = idx;
	برائے (o = بلاکیں; o; o = o->اگلا_pool)
		برائے (e = o->اگلا_free; e-- != o->entries;)
			اگر (pack_id == e->pack_id)
				*c++ = &e->idx;
	آخری = idx + object_گنتی;
	اگر (c != آخری)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, تہی, idx, object_گنتی,
				 &pack_idx_opts, pack_ڈیٹا->hash);
	آزاد(idx);
	واپس tmpfile;
}

ساکن حرف *keep_pack(ثابت حرف *curr_اشاریہ_name)
{
	ساکن ثابت حرف *keep_پیغام = "fast-import";
	ساخت strbuf name = STRBUF_INIT;
	صحیح keep_fd;

	odb_pack_name(pack_ڈیٹا->repo, &name, pack_ڈیٹا->hash, "keep");
	keep_fd = محفوظ_create_file_with_leading_directories(pack_ڈیٹا->repo,
							    name.buf);
	اگر (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_پیغام, لڑی_لمبائی(keep_پیغام));
	اگر (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_ڈیٹا->repo, &name, pack_ڈیٹا->hash, "pack");
	اگر (finalize_object_file(pack_ڈیٹا->repo, pack_ڈیٹا->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_ڈیٹا->repo, &name, pack_ڈیٹا->hash, "idx");
	اگر (finalize_object_file(pack_ڈیٹا->repo, curr_اشاریہ_name, name.buf))
		die(_("cannot store index file"));
	آزاد((خالی *)curr_اشاریہ_name);
	واپس strbuf_detach(&name, تہی);
}

ساکن خالی unkeep_all_packs(خالی)
{
	ساخت strbuf name = STRBUF_INIT;
	صحیح k;

	برائے (k = 0; k < pack_id; k++) {
		ساخت packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

ساکن صحیح loosen_small_pack(ثابت ساخت packed_git *p)
{
	ساخت بچہ_process unpack = بچہ_PROCESS_INIT;

	اگر (lseek(p->pack_fd, 0, SEEK_سیٹ) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.اندر = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_مقصد_stderr = 1;
	strvec_ڈالو(&unpack.دلائل, "unpack-objects");
	اگر (!show_stats)
		strvec_ڈالو(&unpack.دلائل, "-q");

	واپس تسلسل_command(&unpack);
}

ساکن خالی انتہا_packfile(خالی)
{
	ساکن صحیح running;

	اگر (running || !pack_ڈیٹا)
		واپس;

	running = 1;
	clear_delta_بنیاد_cache();
	اگر (object_گنتی) {
		ساخت odb_ماخذ_files *files = odb_ماخذ_files_downcast(pack_ڈیٹا->repo->objects->ماخذ);
		ساخت packed_git *new_p;
		ساخت object_id cur_pack_oid;
		حرف *idx_name;
		صحیح i;
		ساخت branch *b;
		ساخت لیبل *t;

		close_pack_windows(pack_ڈیٹا);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_سرخی_footer(the_hash_algo, pack_ڈیٹا->pack_fd,
					 pack_ڈیٹا->hash, pack_ڈیٹا->pack_name,
					 object_گنتی, cur_pack_oid.hash,
					 pack_سائز);

		اگر (object_گنتی <= unpack_حد_بندی) {
			اگر (!loosen_small_pack(pack_ڈیٹا)) {
				invalidate_pack_id(pack_id);
				جاؤ discard_pack;
			}
		}

		close(pack_ڈیٹا->pack_fd);
		idx_name = keep_pack(create_اشاریہ());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_بھرو_pack(files->dirs->packed, idx_name, 1);
		اگر (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		آزاد(idx_name);

		/* Print the boundary */
		اگر (pack_کنارے) {
			fprintf(pack_کنارے, "%s:", new_p->pack_name);
			برائے (i = 0; i < branch_جدول_sz; i++) {
				برائے (b = branch_جدول[i]; b; b = b->جدول_اگلا_branch) {
					اگر (b->pack_id == pack_id)
						fprintf(pack_کنارے, " %s",
							oid_مقصد_hex(&b->oid));
				}
			}
			برائے (t = پہلا_لیبل; t; t = t->اگلا_لیبل) {
				اگر (t->pack_id == pack_id)
					fprintf(pack_کنارے, " %s",
						oid_مقصد_hex(&t->oid));
			}
			fputc('\n', pack_کنارے);
			fflush(pack_کنارے);
		}

		pack_id++;
	}
	ورنہ {
discard_pack:
		close(pack_ڈیٹا->pack_fd);
		unlink_or_warn(pack_ڈیٹا->pack_name);
	}
	FREE_AND_NULL(pack_ڈیٹا);
	running = 0;

	/* We can't carry a delta across packfiles. */
	strbuf_release(&آخری_blob.ڈیٹا);
	آخری_blob.انحراف = 0;
	آخری_blob.گہرائی = 0;
}

ساکن خالی چکر_packfile(خالی)
{
	انتہا_packfile();
	آغاز_packfile();
}

ساکن صحیح store_object(
	شمار object_type type,
	ساخت strbuf *dat,
	ساخت آخری_object *آخری,
	ساخت object_id *oidout,
	uintmax_t mark)
{
	ساخت odb_ماخذ_files *files = odb_ماخذ_files_downcast(the_repository->objects->ماخذ);
	خالی *برآمد, *delta;
	ساخت object_اندراج *e;
	غیرعلامتی حرف hdr[96];
	ساخت object_id oid;
	غیرعلامتی طویل hdrlen, deltalen = 0;
	ساخت git_hash_ctx c;
	git_zstream s;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	hdrlen = format_object_سرخی((حرف *)hdr, حجم(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	اگر (oidout)
		oidcpy(oidout, &oid);

	e = داخل_کرو_object(&oid);
	اگر (mark)
		داخل_کرو_mark(&marks, mark, e);
	اگر (e->idx.انحراف) {
		duplicate_گنتی_by_type[type]++;
		واپس 1;
	}

	برائے (ساخت odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
		اگر (!packfile_list_ڈھونڈو_oid(packfile_store_get_packs(dir->packed), &oid))
			جاری;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.انحراف = 1; /* just not zero! */
		duplicate_گنتی_by_type[type]++;
		واپس 1;
	}

	اگر (آخری && آخری->ڈیٹا.len && آخری->ڈیٹا.buf && آخری->گہرائی < max_گہرائی
		&& dat->len > the_hash_algo->rawsz) {
		سائز_t deltalen_st;

		delta_گنتی_attempts_by_type[type]++;
		delta = diff_delta(آخری->ڈیٹا.buf, آخری->ڈیٹا.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_سائز_t_مقصد_ulong(deltalen_st);
	} ورنہ
		delta = تہی;

	git_deflate_init(&s, cfg->pack_compression_level);
	اگر (delta) {
		s.اگلا_اندر = delta;
		s.avail_اندر = deltalen;
	} ورنہ {
		s.اگلا_اندر = (خالی *)dat->buf;
		s.avail_اندر = dat->len;
	}
	s.avail_برآمد = git_deflate_bound(&s, s.avail_اندر);
	s.اگلا_برآمد = برآمد = xmalloc(s.avail_برآمد);
	جب (git_deflate(&s, Z_اختتام) == Z_OK)
		; /* nothing */
	git_deflate_انتہا(&s);

	/* Determine if we should auto-checkpoint. */
	اگر ((max_packsize
		&& (pack_سائز + PACK_سائز_حد + s.کل_برآمد) > max_packsize)
		|| (pack_سائز + PACK_سائز_حد + s.کل_برآمد) < pack_سائز) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		چکر_packfile();

		/* We cannot carry a delta into the new pack. */
		اگر (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.اگلا_اندر = (خالی *)dat->buf;
			s.avail_اندر = dat->len;
			s.avail_برآمد = git_deflate_bound(&s, s.avail_اندر);
			s.اگلا_برآمد = برآمد = xrealloc(برآمد, s.avail_برآمد);
			جب (git_deflate(&s, Z_اختتام) == Z_OK)
				; /* nothing */
			git_deflate_انتہا(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.انحراف = pack_سائز;
	object_گنتی++;
	object_گنتی_by_type[type]++;

	چکری_جانچ_32_begin(pack_file);

	اگر (delta) {
		off_t ofs = e->idx.انحراف - آخری->انحراف;
		غیرعلامتی pos = حجم(hdr) - 1;

		delta_گنتی_by_type[type]++;
		e->گہرائی = آخری->گہرائی + 1;

		hdrlen = encode_اندر_pack_object_سرخی(hdr, حجم(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_سائز += hdrlen;

		hdr[pos] = ofs & 127;
		جب (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, حجم(hdr) - pos);
		pack_سائز += حجم(hdr) - pos;
	} ورنہ {
		e->گہرائی = 0;
		hdrlen = encode_اندر_pack_object_سرخی(hdr, حجم(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_سائز += hdrlen;
	}

	hashwrite(pack_file, برآمد, s.کل_برآمد);
	pack_سائز += s.کل_برآمد;

	e->idx.چکری_جانچ_32 = چکری_جانچ_32_انتہا(pack_file);

	آزاد(برآمد);
	آزاد(delta);
	اگر (آخری) {
		اگر (آخری->no_تبادلہ) {
			آخری->ڈیٹا = *dat;
		} ورنہ {
			strbuf_تبادلہ(&آخری->ڈیٹا, dat);
		}
		آخری->انحراف = e->idx.انحراف;
		آخری->گہرائی = e->گہرائی;
	}
	واپس 0;
}

ساکن خالی truncate_pack(ساخت hashfile_checkpoint *checkpoint)
{
	اگر (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_سائز = checkpoint->انحراف;
}

ساکن خالی دھارا_blob(uintmax_t len, ساخت object_id *oidout, uintmax_t mark)
{
	ساخت odb_ماخذ_files *files = odb_ماخذ_files_downcast(the_repository->objects->ماخذ);
	سائز_t اندر_sz = 64 * 1024, برآمد_sz = 64 * 1024;
	غیرعلامتی حرف *اندر_buf = xmalloc(اندر_sz);
	غیرعلامتی حرف *برآمد_buf = xmalloc(برآمد_sz);
	ساخت object_اندراج *e;
	ساخت object_id oid;
	غیرعلامتی طویل hdrlen;
	off_t انحراف;
	ساخت git_hash_ctx c;
	git_zstream s;
	ساخت hashfile_checkpoint checkpoint;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	صحیح حالت_2 = Z_OK;

	/* Determine if we should auto-checkpoint. */
	اگر ((max_packsize
		&& (pack_سائز + PACK_سائز_حد + len) > max_packsize)
		|| (pack_سائز + PACK_سائز_حد + len) < pack_سائز)
		چکر_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	انحراف = checkpoint.انحراف;

	hdrlen = format_object_سرخی((حرف *)برآمد_buf, برآمد_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, برآمد_buf, hdrlen);

	چکری_جانچ_32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_اندر_pack_object_سرخی(برآمد_buf, برآمد_sz, OBJ_BLOB, len);

	s.اگلا_برآمد = برآمد_buf + hdrlen;
	s.avail_برآمد = برآمد_sz - hdrlen;

	جب (حالت_2 != Z_دھارا_انتہا) {
		اگر (0 < len && !s.avail_اندر) {
			سائز_t cnt = اندر_sz < len ? اندر_sz : (سائز_t)len;
			سائز_t n = fread(اندر_buf, 1, cnt, stdin);
			اگر (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, اندر_buf, n);
			s.اگلا_اندر = اندر_buf;
			s.avail_اندر = n;
			len -= n;
		}

		حالت_2 = git_deflate(&s, len ? 0 : Z_اختتام);

		اگر (!s.avail_برآمد || حالت_2 == Z_دھارا_انتہا) {
			سائز_t n = s.اگلا_برآمد - برآمد_buf;
			hashwrite(pack_file, برآمد_buf, n);
			pack_سائز += n;
			s.اگلا_برآمد = برآمد_buf;
			s.avail_برآمد = برآمد_sz;
		}

		انتخاب (حالت_2) {
		حالت Z_OK:
		حالت Z_BUF_ERROR:
		حالت Z_دھارا_انتہا:
			جاری;
		طےشدہ:
			die(_("unexpected deflate failure: %d"), حالت_2);
		}
	}
	git_deflate_انتہا(&s);
	git_hash_final_oid(&oid, &c);

	اگر (oidout)
		oidcpy(oidout, &oid);

	e = داخل_کرو_object(&oid);

	اگر (mark)
		داخل_کرو_mark(&marks, mark, e);

	اگر (e->idx.انحراف) {
		duplicate_گنتی_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		جاؤ برآمد;
	}

	برائے (ساخت odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
		اگر (!packfile_list_ڈھونڈو_oid(packfile_store_get_packs(dir->packed), &oid))
			جاری;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.انحراف = 1; /* just not zero! */
		duplicate_گنتی_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		جاؤ برآمد;
	}

	e->گہرائی = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.انحراف = انحراف;
	e->idx.چکری_جانچ_32 = چکری_جانچ_32_انتہا(pack_file);
	object_گنتی++;
	object_گنتی_by_type[OBJ_BLOB]++;

برآمد:
	آزاد(اندر_buf);
	آزاد(برآمد_buf);
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
ساکن خالی *gfi_unpack_اندراج(
	ساخت object_اندراج *oe,
	غیرعلامتی طویل *sizep)
{
	شمار object_type type;
	سائز_t سائز_st = 0;
	خالی *ڈیٹا;
	ساخت packed_git *p = all_packs[oe->pack_id];
	اگر (p == pack_ڈیٹا && p->pack_سائز < (pack_سائز + the_hash_algo->rawsz)) {
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
		p->pack_سائز = pack_سائز + the_hash_algo->rawsz;
	}
	ڈیٹا = unpack_اندراج(the_repository, p, oe->idx.انحراف, &type, &سائز_st);
	اگر (sizep)
		*sizep = cast_سائز_t_مقصد_ulong(سائز_st);
	واپس ڈیٹا;
}

ساکن خالی بھرو_درخت(ساخت درخت_اندراج *جڑ)
{
	ساخت object_id *oid = &جڑ->versions[1].oid;
	ساخت object_اندراج *myoe;
	ساخت درخت_content *t;
	غیرعلامتی طویل سائز;
	حرف *buf;
	ثابت حرف *c;

	جڑ->درخت = t = new_درخت_content(8);
	اگر (is_null_oid(oid))
		واپس;

	myoe = ڈھونڈو_object(oid);
	اگر (myoe && myoe->pack_id != MAX_PACK_ID) {
		اگر (myoe->type != OBJ_درخت)
			die(_("not a tree: %s"), oid_مقصد_hex(oid));
		t->delta_گہرائی = myoe->گہرائی;
		buf = gfi_unpack_اندراج(myoe, &سائز);
		اگر (!buf)
			die(_("can't load tree %s"), oid_مقصد_hex(oid));
	} ورنہ {
		شمار object_type type;
		سائز_t سائز_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &سائز_st);
		سائز = cast_سائز_t_مقصد_ulong(سائز_st);
		اگر (!buf || type != OBJ_درخت)
			die(_("can't load tree %s"), oid_مقصد_hex(oid));
	}

	c = buf;
	جب (c != (buf + سائز)) {
		ساخت درخت_اندراج *e = new_درخت_اندراج();

		اگر (t->اندراج_گنتی == t->اندراج_استعداد)
			جڑ->درخت = t = grow_درخت_content(t, t->اندراج_گنتی);
		t->entries[t->اندراج_گنتی++] = e;

		e->درخت = تہی;
		c = parse_منوال(c, &e->versions[1].منوال);
		اگر (!c)
			die(_("corrupt mode in %s"), oid_مقصد_hex(oid));
		e->versions[0].منوال = e->versions[1].منوال;
		e->name = مقصد_atom(c, لڑی_لمبائی(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (غیرعلامتی حرف *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (غیرعلامتی حرف *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	آزاد(buf);
}

ساکن صحیح tecmp0 (ثابت خالی *_a, ثابت خالی *_b)
{
	ساخت درخت_اندراج *a = *((ساخت درخت_اندراج**)_a);
	ساخت درخت_اندراج *b = *((ساخت درخت_اندراج**)_b);
	واپس بنیاد_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].منوال,
		b->name->str_dat, b->name->str_len, b->versions[0].منوال);
}

ساکن صحیح tecmp1 (ثابت خالی *_a, ثابت خالی *_b)
{
	ساخت درخت_اندراج *a = *((ساخت درخت_اندراج**)_a);
	ساخت درخت_اندراج *b = *((ساخت درخت_اندراج**)_b);
	واپس بنیاد_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].منوال,
		b->name->str_dat, b->name->str_len, b->versions[1].منوال);
}

ساکن خالی mktree(ساخت درخت_content *t, صحیح v, ساخت strbuf *b)
{
	سائز_t maxlen = 0;
	غیرعلامتی صحیح i;

	اگر (!v)
		QSORT(t->entries, t->اندراج_گنتی, tecmp0);
	ورنہ
		QSORT(t->entries, t->اندراج_گنتی, tecmp1);

	برائے (i = 0; i < t->اندراج_گنتی; i++) {
		اگر (t->entries[i]->versions[v].منوال)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	برائے (i = 0; i < t->اندراج_گنتی; i++) {
		ساخت درخت_اندراج *e = t->entries[i];
		اگر (!e->versions[v].منوال)
			جاری;
		strbuf_addf(b, "%o %s%c",
			(غیرعلامتی صحیح)(e->versions[v].منوال & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_جوڑو(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

ساکن خالی store_درخت(ساخت درخت_اندراج *جڑ)
{
	ساخت درخت_content *t;
	غیرعلامتی صحیح i, j, del;
	ساخت آخری_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	ساخت object_اندراج *le = تہی;

	اگر (!is_null_oid(&جڑ->versions[1].oid))
		واپس;

	اگر (!جڑ->درخت)
		بھرو_درخت(جڑ);
	t = جڑ->درخت;

	برائے (i = 0; i < t->اندراج_گنتی; i++) {
		اگر (t->entries[i]->درخت)
			store_درخت(t->entries[i]);
	}

	اگر (!(جڑ->versions[0].منوال & NO_DELTA))
		le = ڈھونڈو_object(&جڑ->versions[0].oid);
	اگر (S_ISDIR(جڑ->versions[0].منوال) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_درخت);
		lo.ڈیٹا = old_درخت;
		lo.انحراف = le->idx.انحراف;
		lo.گہرائی = t->delta_گہرائی;
	}

	mktree(t, 1, &new_درخت);
	store_object(OBJ_درخت, &new_درخت, &lo, &جڑ->versions[1].oid, 0);

	t->delta_گہرائی = lo.گہرائی;
	برائے (i = 0, j = 0, del = 0; i < t->اندراج_گنتی; i++) {
		ساخت درخت_اندراج *e = t->entries[i];
		اگر (e->versions[1].منوال) {
			e->versions[0].منوال = e->versions[1].منوال;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} ورنہ {
			release_درخت_اندراج(e);
			del++;
		}
	}
	t->اندراج_گنتی -= del;
}

ساکن خالی درخت_content_replace(
	ساخت درخت_اندراج *جڑ,
	ثابت ساخت object_id *oid,
	ثابت uint16_t منوال,
	ساخت درخت_content *newtree)
{
	اگر (!S_ISDIR(منوال))
		die(_("root cannot be a non-directory"));
	oidclr(&جڑ->versions[0].oid, the_repository->hash_algo);
	oidcpy(&جڑ->versions[1].oid, oid);
	اگر (جڑ->درخت)
		release_درخت_content_recursive(جڑ->درخت);
	جڑ->درخت = newtree;
}

ساکن صحیح درخت_content_سیٹ(
	ساخت درخت_اندراج *جڑ,
	ثابت حرف *p,
	ثابت ساخت object_id *oid,
	ثابت uint16_t منوال,
	ساخت درخت_content *subtree)
{
	ساخت درخت_content *t;
	ثابت حرف *slash1;
	غیرعلامتی صحیح i, n;
	ساخت درخت_اندراج *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	اگر (!n)
		die(_("empty path component found in input"));
	اگر (!*slash1 && !S_ISDIR(منوال) && subtree)
		die(_("non-directories cannot have subtrees"));

	اگر (!جڑ->درخت)
		بھرو_درخت(جڑ);
	t = جڑ->درخت;
	برائے (i = 0; i < t->اندراج_گنتی; i++) {
		e = t->entries[i];
		اگر (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			اگر (!*slash1) {
				اگر (!S_ISDIR(منوال)
						&& e->versions[1].منوال == منوال
						&& oideq(&e->versions[1].oid, oid))
					واپس 0;
				e->versions[1].منوال = منوال;
				oidcpy(&e->versions[1].oid, oid);
				اگر (e->درخت)
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
				اگر (S_ISDIR(e->versions[0].منوال))
					e->versions[0].منوال |= NO_DELTA;

				oidclr(&جڑ->versions[1].oid, the_repository->hash_algo);
				واپس 1;
			}
			اگر (!S_ISDIR(e->versions[1].منوال)) {
				e->درخت = new_درخت_content(8);
				e->versions[1].منوال = S_IFDIR;
			}
			اگر (!e->درخت)
				بھرو_درخت(e);
			اگر (درخت_content_سیٹ(e, slash1 + 1, oid, منوال, subtree)) {
				oidclr(&جڑ->versions[1].oid, the_repository->hash_algo);
				واپس 1;
			}
			واپس 0;
		}
	}

	اگر (t->اندراج_گنتی == t->اندراج_استعداد)
		جڑ->درخت = t = grow_درخت_content(t, t->اندراج_گنتی);
	e = new_درخت_اندراج();
	e->name = مقصد_atom(p, n);
	e->versions[0].منوال = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->اندراج_گنتی++] = e;
	اگر (*slash1) {
		e->درخت = new_درخت_content(8);
		e->versions[1].منوال = S_IFDIR;
		درخت_content_سیٹ(e, slash1 + 1, oid, منوال, subtree);
	} ورنہ {
		e->درخت = subtree;
		e->versions[1].منوال = منوال;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&جڑ->versions[1].oid, the_repository->hash_algo);
	واپس 1;
}

ساکن صحیح درخت_content_remove(
	ساخت درخت_اندراج *جڑ,
	ثابت حرف *p,
	ساخت درخت_اندراج *backup_برگ,
	صحیح allow_جڑ)
{
	ساخت درخت_content *t;
	ثابت حرف *slash1;
	غیرعلامتی صحیح i, n;
	ساخت درخت_اندراج *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	اگر (!جڑ->درخت)
		بھرو_درخت(جڑ);

	اگر (!*p && allow_جڑ) {
		e = جڑ;
		جاؤ del_اندراج;
	}

	t = جڑ->درخت;
	برائے (i = 0; i < t->اندراج_گنتی; i++) {
		e = t->entries[i];
		اگر (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			اگر (*slash1 && !S_ISDIR(e->versions[1].منوال))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				واپس 1;
			اگر (!*slash1 || !S_ISDIR(e->versions[1].منوال))
				جاؤ del_اندراج;
			اگر (!e->درخت)
				بھرو_درخت(e);
			اگر (درخت_content_remove(e, slash1 + 1, backup_برگ, 0)) {
				برائے (n = 0; n < e->درخت->اندراج_گنتی; n++) {
					اگر (e->درخت->entries[n]->versions[1].منوال) {
						oidclr(&جڑ->versions[1].oid,
						       the_repository->hash_algo);
						واپس 1;
					}
				}
				backup_برگ = تہی;
				جاؤ del_اندراج;
			}
			واپس 0;
		}
	}
	واپس 0;

del_اندراج:
	اگر (backup_برگ)
		حافظہ_نقل(backup_برگ, e, حجم(*backup_برگ));
	ورنہ اگر (e->درخت)
		release_درخت_content_recursive(e->درخت);
	e->درخت = تہی;
	e->versions[1].منوال = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&جڑ->versions[1].oid, the_repository->hash_algo);
	واپس 1;
}

ساکن صحیح درخت_content_get(
	ساخت درخت_اندراج *جڑ,
	ثابت حرف *p,
	ساخت درخت_اندراج *برگ,
	صحیح allow_جڑ)
{
	ساخت درخت_content *t;
	ثابت حرف *slash1;
	غیرعلامتی صحیح i, n;
	ساخت درخت_اندراج *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	اگر (!n && !allow_جڑ)
		die(_("empty path component found in input"));

	اگر (!جڑ->درخت)
		بھرو_درخت(جڑ);

	اگر (!n) {
		e = جڑ;
		جاؤ ملا_اندراج;
	}

	t = جڑ->درخت;
	برائے (i = 0; i < t->اندراج_گنتی; i++) {
		e = t->entries[i];
		اگر (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			اگر (!*slash1)
				جاؤ ملا_اندراج;
			اگر (!S_ISDIR(e->versions[1].منوال))
				واپس 0;
			اگر (!e->درخت)
				بھرو_درخت(e);
			واپس درخت_content_get(e, slash1 + 1, برگ, 0);
		}
	}
	واپس 0;

ملا_اندراج:
	حافظہ_نقل(برگ, e, حجم(*برگ));
	اگر (e->درخت && is_null_oid(&e->versions[1].oid))
		برگ->درخت = dup_درخت_content(e->درخت);
	ورنہ
		برگ->درخت = تہی;
	واپس 1;
}

ساکن صحیح update_branch(ساخت branch *b)
{
	ساکن ثابت حرف *پیغام = "fast-import";
	ساخت ref_transaction *transaction;
	ساخت object_id old_oid;
	ساخت strbuf err = STRBUF_INIT;
	ساکن ثابت حرف *replace_سابقہ = "refs/replace/";

	اگر (starts_with(b->name, replace_سابقہ) &&
	    !strcmp(b->name + لڑی_لمبائی(replace_سابقہ),
		    oid_مقصد_hex(&b->oid))) {
		اگر (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_مقصد_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				تہی, b->name, تہی, 0);
		واپس 0;
	}
	اگر (is_null_oid(&b->oid)) {
		اگر (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					تہی, b->name, تہی, 0);
		واپس 0;
	}
	اگر (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	اگر (!force_update && !is_null_oid(&old_oid)) {
		ساخت commit *old_cmit, *new_cmit;
		صحیح ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		اگر (!old_cmit || !new_cmit)
			واپس error(_("branch %s is missing commits."), b->name);

		ret = repo_اندر_ملاؤ_bases(the_repository, old_cmit, new_cmit);
		اگر (ret < 0)
			باہر_نکلو(128);
		اگر (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_مقصد_hex(&b->oid),
				oid_مقصد_hex(&old_oid));
			واپس -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	اگر (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   تہی, تہی, 0, پیغام, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		واپس -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	واپس 0;
}

ساکن خالی dump_branches(خالی)
{
	غیرعلامتی صحیح i;
	ساخت branch *b;

	برائے (i = 0; i < branch_جدول_sz; i++) {
		برائے (b = branch_جدول[i]; b; b = b->جدول_اگلا_branch)
			failure |= update_branch(b);
	}
}

ساکن خالی dump_لیبل_فہرست(خالی)
{
	ساکن ثابت حرف *پیغام = "fast-import";
	ساخت لیبل *t;
	ساخت strbuf ref_name = STRBUF_INIT;
	ساخت strbuf err = STRBUF_INIT;
	ساخت ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	اگر (!transaction) {
		failure |= error("%s", err.buf);
		جاؤ cleanup;
	}
	برائے (t = پہلا_لیبل; t; t = t->اگلا_لیبل) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		اگر (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, تہی, تہی, تہی,
					   0, پیغام, &err)) {
			failure |= error("%s", err.buf);
			جاؤ cleanup;
		}
	}
	اگر (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

ساکن خالی dump_marks(خالی)
{
	ساخت تالا_file mark_تالا = تالا_INIT;
	FILE *f;

	اگر (!export_marks_file || (import_marks_file && !import_marks_file_مکمل))
		واپس;

	اگر (محفوظ_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		واپس;
	}

	اگر (hold_تالا_file_for_update(&mark_تالا, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		واپس;
	}

	f = fdopen_تالا_file(&mark_تالا, "w");
	اگر (!f) {
		صحیح saved_errno = errno;
		rollback_تالا_file(&mark_تالا);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		واپس;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	اگر (commit_تالا_file(&mark_تالا)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		واپس;
	}
}

ساکن خالی داخل_کرو_object_اندراج(ساخت mark_سیٹ **s, ساخت object_id *oid, uintmax_t mark)
{
	ساخت object_اندراج *e;
	e = ڈھونڈو_object(oid);
	اگر (!e) {
		شمار object_type type = odb_read_object_info(the_repository->objects,
							     oid, تہی);
		اگر (type < 0)
			die(_("object not found: %s"), oid_مقصد_hex(oid));
		e = داخل_کرو_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.انحراف = 1; /* just not zero! */
	}
	داخل_کرو_mark(s, mark, e);
}

ساکن خالی داخل_کرو_oid_اندراج(ساخت mark_سیٹ **s, ساخت object_id *oid, uintmax_t mark)
{
	داخل_کرو_mark(s, mark, xmemdupz(oid, حجم(*oid)));
}

ساکن خالی read_mark_file(ساخت mark_سیٹ **s, FILE *f, mark_سیٹ_inserter_t inserter)
{
	حرف سطر[512];
	جب (fgets(سطر, حجم(سطر), f)) {
		uintmax_t mark;
		حرف *انتہا;
		ساخت object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, حجم(oid.hash));

		انتہا = strchr(سطر, '\n');
		اگر (سطر[0] != ':' || !انتہا)
			die(_("corrupt mark line: %s"), سطر);
		*انتہا = 0;
		mark = strtoumax(سطر + 1, &انتہا, 10);
		اگر (!mark || انتہا == سطر + 1
			|| *انتہا != ' '
			|| get_oid_hex_any(انتہا + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), سطر);
		inserter(s, &oid, mark);
	}
}

ساکن خالی read_marks(خالی)
{
	FILE *f = fopen(import_marks_file, "r");
	اگر (f)
		;
	ورنہ اگر (import_marks_file_ignore_missing && errno == ENOENT)
		جاؤ مکمل; /* Marks file does not exist */
	ورنہ
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, داخل_کرو_object_اندراج);
	fclose(f);
مکمل:
	import_marks_file_مکمل = 1;
}


ساکن صحیح read_اگلا_command(ساخت fast_import_state *state)
{
	ساکن صحیح stdin_eof = 0;

	اگر (stdin_eof) {
		unread_command_buf = 0;
		واپس EOF;
	}

	برائے (;;) {
		اگر (unread_command_buf) {
			unread_command_buf = 0;
		} ورنہ {
			ساخت recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			اگر (stdin_eof)
				واپس EOF;

			اگر (!state->نظر_آیا_ڈیٹا_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			اگر (rc)
				rc_free = rc->اگلا;
			ورنہ {
				rc = cmd_hist.اگلا;
				cmd_hist.اگلا = rc->اگلا;
				cmd_hist.اگلا->پچھلا_2 = &cmd_hist;
				آزاد(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->پچھلا_2 = cmd_دم;
			rc->اگلا = cmd_hist.پچھلا_2;
			rc->پچھلا_2->اگلا = rc;
			cmd_دم = rc;
		}
		اگر (command_buf.buf[0] == '#')
			جاری;
		واپس 0;
	}
}

ساکن خالی skip_optional_lf(خالی)
{
	صحیح رکن_char = fgetc(stdin);
	اگر (رکن_char != '\n' && رکن_char != EOF)
		ungetc(رکن_char, stdin);
}

ساکن خالی parse_mark(ساخت fast_import_state *state)
{
	ثابت حرف *v;
	اگر (skip_سابقہ(command_buf.buf, "mark :", &v)) {
		اگلا_mark = strtoumax(v, تہی, 10);
		read_اگلا_command(state);
	}
	ورنہ
		اگلا_mark = 0;
}

ساکن خالی parse_original_identifier(ساخت fast_import_state *state)
{
	ثابت حرف *v;
	اگر (skip_سابقہ(command_buf.buf, "original-oid ", &v))
		read_اگلا_command(state);
}

ساکن صحیح parse_ڈیٹا(ساخت strbuf *sb, uintmax_t حد_بندی, uintmax_t *len_res)
{
	ثابت حرف *ڈیٹا;
	strbuf_reset(sb);

	اگر (!skip_سابقہ(command_buf.buf, "data ", &ڈیٹا))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	اگر (skip_سابقہ(ڈیٹا, "<<", &ڈیٹا)) {
		حرف *رکن = xstrdup(ڈیٹا);
		سائز_t رکن_len = command_buf.len - (ڈیٹا - command_buf.buf);

		برائے (;;) {
			اگر (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), رکن);
			اگر (رکن_len == command_buf.len
				&& !strcmp(رکن, command_buf.buf))
				توڑو;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		آزاد(رکن);
	}
	ورنہ {
		uintmax_t len = strtoumax(ڈیٹا, تہی, 10);
		سائز_t n = 0, لمبائی = (سائز_t)len;

		اگر (حد_بندی && حد_بندی < len) {
			*len_res = len;
			واپس 0;
		}
		اگر (لمبائی < len)
			die(_("data is too large to use in this context"));

		جب (n < لمبائی) {
			سائز_t s = strbuf_fread(sb, لمبائی - n, stdin);
			اگر (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(غیرعلامتی طویل)(لمبائی - n));
			n += s;
		}
	}

	skip_optional_lf();
	واپس 1;
}

ساکن صحیح validate_raw_date(ثابت حرف *src, ساخت strbuf *نتیجہ, صحیح strict)
{
	ثابت حرف *orig_src = src;
	حرف *endp;
	غیرعلامتی طویل عدد;

	errno = 0;

	عدد = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	اگر (errno || endp == src || *endp != ' ')
		واپس -1;

	src = endp + 1;
	اگر (*src != '-' && *src != '+')
		واپس -1;

	عدد = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	اگر (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < عدد))             /* parsed a broken timezone */
	   )
		واپس -1;

	strbuf_addstr(نتیجہ, orig_src);
	واپس 0;
}

ساکن حرف *parse_ident(ثابت حرف *buf)
{
	ثابت حرف *ltgt;
	سائز_t name_len;
	ساخت strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	اگر (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	اگر (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	اگر (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	اگر (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	اگر (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_جوڑو(&ident, buf, name_len);

	انتخاب (whenspec) {
	حالت WHENSPEC_RAW:
		اگر (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		توڑو;
	حالت WHENSPEC_RAW_PERMISSIVE:
		اگر (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		توڑو;
	حالت WHENSPEC_RFC2822:
		اگر (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		توڑو;
	حالت WHENSPEC_NOW:
		اگر (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		توڑو;
	}

	واپس strbuf_detach(&ident, تہی);
}

ساکن خالی parse_and_store_blob(
	ساخت آخری_object *آخری,
	ساخت object_id *oidout,
	uintmax_t mark)
{
	ساکن ساخت strbuf buf = STRBUF_INIT;
	uintmax_t len;

	اگر (parse_ڈیٹا(&buf, repo_settings_get_big_file_حد(the_repository), &len))
		store_object(OBJ_BLOB, &buf, آخری, oidout, mark);
	ورنہ {
		اگر (آخری) {
			strbuf_release(&آخری->ڈیٹا);
			آخری->انحراف = 0;
			آخری->گہرائی = 0;
		}
		دھارا_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

ساکن خالی parse_new_blob(ساخت fast_import_state *state)
{
	read_اگلا_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&آخری_blob, تہی, اگلا_mark);
}

ساکن خالی unload_one_branch(خالی)
{
	جب (cur_فعال_branches
		&& cur_فعال_branches >= max_فعال_branches) {
		uintmax_t min_commit = ULONG_MAX;
		ساخت branch *e, *l = تہی, *p = تہی;

		برائے (e = فعال_branches; e; e = e->فعال_اگلا_branch) {
			اگر (e->آخری_commit < min_commit) {
				p = l;
				min_commit = e->آخری_commit;
			}
			l = e;
		}

		اگر (p) {
			e = p->فعال_اگلا_branch;
			p->فعال_اگلا_branch = e->فعال_اگلا_branch;
		} ورنہ {
			e = فعال_branches;
			فعال_branches = e->فعال_اگلا_branch;
		}
		e->فعال = 0;
		e->فعال_اگلا_branch = تہی;
		اگر (e->branch_درخت.درخت) {
			release_درخت_content_recursive(e->branch_درخت.درخت);
			e->branch_درخت.درخت = تہی;
		}
		cur_فعال_branches--;
	}
}

ساکن خالی بھرو_branch(ساخت branch *b)
{
	بھرو_درخت(&b->branch_درخت);
	اگر (!b->فعال) {
		b->فعال = 1;
		b->فعال_اگلا_branch = فعال_branches;
		فعال_branches = b;
		cur_فعال_branches++;
		branch_بھرو_گنتی++;
	}
}

ساکن غیرعلامتی حرف convert_عدد_notes_مقصد_fanout(uintmax_t عدد_notes)
{
	غیرعلامتی حرف fanout = 0;
	جب ((عدد_notes >>= 8))
		fanout++;
	واپس fanout;
}

ساکن خالی construct_راستہ_with_fanout(ثابت حرف *hex_sha1,
		غیرعلامتی حرف fanout, حرف *راستہ)
{
	غیرعلامتی صحیح i = 0, j = 0;
	اگر (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	جب (fanout) {
		راستہ[i++] = hex_sha1[j++];
		راستہ[i++] = hex_sha1[j++];
		راستہ[i++] = '/';
		fanout--;
	}
	حافظہ_نقل(راستہ + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	راستہ[i + the_hash_algo->hexsz - j] = '\0';
}

ساکن uintmax_t do_change_note_fanout(
		ساخت درخت_اندراج *orig_جڑ, ساخت درخت_اندراج *جڑ,
		حرف *hex_oid, غیرعلامتی صحیح hex_oid_len,
		حرف *fullpath, غیرعلامتی صحیح fullpath_len,
		غیرعلامتی حرف fanout)
{
	ساخت درخت_content *t;
	ساخت درخت_اندراج *e, برگ;
	غیرعلامتی صحیح i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t عدد_notes = 0;
	ساخت object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	حرف realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ثابت غیرعلامتی hexsz = the_hash_algo->hexsz;

	اگر (!جڑ->درخت)
		بھرو_درخت(جڑ);
	t = جڑ->درخت;

	برائے (i = 0; t && i < t->اندراج_گنتی; i++) {
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
		اگر (!e->versions[1].منوال ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			جاری;

		/* This _may_ be a note entry, or a subdir containing notes */
		حافظہ_نقل(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		اگر (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		حافظہ_نقل(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		اگر (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			اگر (fanout == 0xff) {
				/* Counting mode, no rename */
				عدد_notes++;
				جاری;
			}
			construct_راستہ_with_fanout(hex_oid, fanout, realpath);
			اگر (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				عدد_notes++;
				جاری;
			}

			/* Rename fullpath to realpath */
			اگر (!درخت_content_remove(orig_جڑ, fullpath, &برگ, 0))
				die(_("failed to remove path %s"), fullpath);
			درخت_content_سیٹ(orig_جڑ, realpath,
				&برگ.versions[1].oid,
				برگ.versions[1].منوال,
				برگ.درخت);
		} ورنہ اگر (S_ISDIR(e->versions[1].منوال)) {
			/* This is a subdir that may contain note entries */
			عدد_notes += do_change_note_fanout(orig_جڑ, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = جڑ->درخت;
	}
	واپس عدد_notes;
}

ساکن uintmax_t change_note_fanout(ساخت درخت_اندراج *جڑ,
		غیرعلامتی حرف fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	حرف hex_oid[GIT_MAX_HEXSZ], راستہ[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	واپس do_change_note_fanout(جڑ, جڑ, hex_oid, 0, راستہ, 0, fanout);
}

ساکن صحیح parse_mapped_oid_hex(ثابت حرف *hex, ساخت object_id *oid, ثابت حرف **انتہا)
{
	صحیح algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, حجم(oid->hash));

	algo = parse_oid_hex_any(hex, oid, انتہا);
	اگر (algo == GIT_HASH_UNKNOWN)
		واپس -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	اگر (it == kh_انتہا(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		اگر (hash_algos[algo].format_id == the_hash_algo->format_id)
			واپس 0;
		واپس -1;
	}
	oidcpy(oid, kh_قدر(sub_oid_map, it));
	واپس 0;
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
ساکن uintmax_t parse_mark_ref(ثابت حرف *p, حرف **endptr)
{
	uintmax_t mark;

	أكد(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	اگر (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	واپس mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
ساکن uintmax_t parse_mark_ref_eol(ثابت حرف *p)
{
	حرف *انتہا;
	uintmax_t mark;

	mark = parse_mark_ref(p, &انتہا);
	اگر (*انتہا != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	واپس mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
ساکن uintmax_t parse_mark_ref_space(ثابت حرف **p)
{
	uintmax_t mark;
	حرف *انتہا;

	mark = parse_mark_ref(*p, &انتہا);
	اگر (*انتہا++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = انتہا;
	واپس mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
ساکن خالی parse_راستہ(ساخت strbuf *sb, ثابت حرف *p, ثابت حرف **endp,
		صحیح is_آخری_میدان, ثابت حرف *میدان)
{
	اگر (*p == '"') {
		اگر (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), میدان, command_buf.buf);
		اگر (لڑی_لمبائی(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), میدان, command_buf.buf);
	} ورنہ {
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
ساکن خالی parse_راستہ_eol(ساخت strbuf *sb, ثابت حرف *p, ثابت حرف *میدان)
{
	ثابت حرف *انتہا;

	parse_راستہ(sb, p, &انتہا, 1, میدان);
	اگر (*انتہا)
		die(_("garbage after %s: %s"), میدان, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
ساکن خالی parse_راستہ_space(ساخت strbuf *sb, ثابت حرف *p,
		ثابت حرف **endp, ثابت حرف *میدان)
{
	parse_راستہ(sb, p, endp, 0, میدان);
	اگر (**endp != ' ')
		die(_("missing space after %s: %s"), میدان, command_buf.buf);
	(*endp)++;
}

ساکن خالی file_change_m(ساخت fast_import_state *state,
			  ثابت حرف *p,
			  ساخت branch *b)
{
	ساکن ساخت strbuf راستہ = STRBUF_INIT;
	ساخت object_اندراج *oe;
	ساخت object_id oid;
	uint16_t منوال, inline_ڈیٹا = 0;

	p = parse_منوال(p, &منوال);
	اگر (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	انتخاب (منوال) {
	حالت 0644:
	حالت 0755:
		منوال |= S_IFREG;
	حالت S_IFREG | 0644:
	حالت S_IFREG | 0755:
	حالت S_IFLNK:
	حالت S_IFDIR:
	حالت S_IFGITLINK:
		/* ok */
		توڑو;
	طےشدہ:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	اگر (*p == ':') {
		oe = ڈھونڈو_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} ورنہ اگر (skip_سابقہ(p, "inline ", &p)) {
		inline_ڈیٹا = 1;
		oe = تہی; /* not used with inline_data, but makes gcc happy */
	} ورنہ {
		اگر (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = ڈھونڈو_object(&oid);
		اگر (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&راستہ);
	parse_راستہ_eol(&راستہ, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	اگر (S_ISDIR(منوال) &&
	    is_خالی_خانے_درخت_oid(&oid, the_repository->hash_algo) &&
	    *راستہ.buf) {
		درخت_content_remove(&b->branch_درخت, راستہ.buf, تہی, 0);
		واپس;
	}

	اگر (S_ISGITLINK(منوال)) {
		اگر (inline_ڈیٹا)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		ورنہ اگر (oe) {
			اگر (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} ورنہ اگر (inline_ڈیٹا) {
		اگر (S_ISDIR(منوال))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		جب (read_اگلا_command(state) != EOF) {
			ثابت حرف *v;
			اگر (skip_سابقہ(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			ورنہ {
				parse_and_store_blob(&آخری_blob, &oid, 0);
				توڑو;
			}
		}
	} ورنہ {
		شمار object_type expected = S_ISDIR(منوال) ?
						OBJ_درخت: OBJ_BLOB;
		شمار object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, تہی);
		اگر (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(منوال) ?  _("tree") : _("blob"),
			    command_buf.buf);
		اگر (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	اگر (!*راستہ.buf) {
		درخت_content_replace(&b->branch_درخت, &oid, منوال, تہی);
		واپس;
	}

	اگر (!verify_راستہ(راستہ.buf, منوال))
		die(_("invalid path '%s'"), راستہ.buf);
	درخت_content_سیٹ(&b->branch_درخت, راستہ.buf, &oid, منوال, تہی);
}

ساکن خالی file_change_d(ثابت حرف *p, ساخت branch *b)
{
	ساکن ساخت strbuf راستہ = STRBUF_INIT;

	strbuf_reset(&راستہ);
	parse_راستہ_eol(&راستہ, p, "path");
	درخت_content_remove(&b->branch_درخت, راستہ.buf, تہی, 1);
}

ساکن خالی file_change_cr(ثابت حرف *p, ساخت branch *b, صحیح rename)
{
	ساکن ساخت strbuf ماخذ = STRBUF_INIT;
	ساکن ساخت strbuf dest = STRBUF_INIT;
	ساخت درخت_اندراج برگ;

	strbuf_reset(&ماخذ);
	parse_راستہ_space(&ماخذ, p, &p, "source");
	strbuf_reset(&dest);
	parse_راستہ_eol(&dest, p, "dest");

	memset(&برگ, 0, حجم(برگ));
	اگر (rename)
		درخت_content_remove(&b->branch_درخت, ماخذ.buf, &برگ, 1);
	ورنہ
		درخت_content_get(&b->branch_درخت, ماخذ.buf, &برگ, 1);
	اگر (!برگ.versions[1].منوال)
		die(_("path %s not in branch"), ماخذ.buf);
	اگر (!*dest.buf) {	/* C "path/to/subdir" "" */
		درخت_content_replace(&b->branch_درخت,
			&برگ.versions[1].oid,
			برگ.versions[1].منوال,
			برگ.درخت);
		واپس;
	}
	اگر (!verify_راستہ(dest.buf, برگ.versions[1].منوال))
		die(_("invalid path '%s'"), dest.buf);
	درخت_content_سیٹ(&b->branch_درخت, dest.buf,
		&برگ.versions[1].oid,
		برگ.versions[1].منوال,
		برگ.درخت);
}

ساکن خالی note_change_n(ساخت fast_import_state *state,
			  ثابت حرف *p,
			  ساخت branch *b,
			  غیرعلامتی حرف *old_fanout)
{
	ساخت object_اندراج *oe;
	ساخت branch *s;
	ساخت object_id oid, commit_oid;
	حرف راستہ[GIT_MAX_RAWSZ * 3];
	uint16_t inline_ڈیٹا = 0;
	غیرعلامتی حرف new_fanout;

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
	اگر (b->عدد_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->عدد_notes = change_note_fanout(&b->branch_درخت, 0xff);
		*old_fanout = convert_عدد_notes_مقصد_fanout(b->عدد_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	اگر (*p == ':') {
		oe = ڈھونڈو_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} ورنہ اگر (skip_سابقہ(p, "inline ", &p)) {
		inline_ڈیٹا = 1;
		oe = تہی; /* not used with inline_data, but makes gcc happy */
	} ورنہ {
		اگر (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = ڈھونڈو_object(&oid);
		اگر (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	اگر (s) {
		اگر (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} ورنہ اگر (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		ساخت object_اندراج *commit_oe = ڈھونڈو_mark(marks, commit_mark);
		اگر (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} ورنہ اگر (!repo_get_oid(the_repository, p, &commit_oid)) {
		سائز_t سائز;
		حرف *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &سائز,
						   &commit_oid);
		اگر (!buf || سائز < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		آزاد(buf);
	} ورنہ
		die(_("invalid ref name or SHA1 expression: %s"), p);

	اگر (inline_ڈیٹا) {
		read_اگلا_command(state);
		parse_and_store_blob(&آخری_blob, &oid, 0);
	} ورنہ اگر (oe) {
		اگر (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} ورنہ اگر (!is_null_oid(&oid)) {
		شمار object_type type = odb_read_object_info(the_repository->objects, &oid,
							تہی);
		اگر (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		اگر (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_راستہ_with_fanout(oid_مقصد_hex(&commit_oid), *old_fanout, راستہ);
	اگر (درخت_content_remove(&b->branch_درخت, راستہ, تہی, 0))
		b->عدد_notes--;

	اگر (is_null_oid(&oid))
		واپس; /* nothing to insert */

	b->عدد_notes++;
	new_fanout = convert_عدد_notes_مقصد_fanout(b->عدد_notes);
	construct_راستہ_with_fanout(oid_مقصد_hex(&commit_oid), new_fanout, راستہ);
	درخت_content_سیٹ(&b->branch_درخت, راستہ, &oid, S_IFREG | 0644, تہی);
}

ساکن خالی file_change_deleteall(ساخت branch *b)
{
	release_درخت_content_recursive(b->branch_درخت.درخت);
	oidclr(&b->branch_درخت.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_درخت.versions[1].oid, the_repository->hash_algo);
	بھرو_درخت(&b->branch_درخت);
	b->عدد_notes = 0;
}

ساکن خالی parse_from_commit(ساخت branch *b, حرف *buf, غیرعلامتی طویل سائز)
{
	اگر (!buf || سائز < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_مقصد_hex(&b->oid));
	اگر (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_درخت.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_مقصد_hex(&b->oid));
	oidcpy(&b->branch_درخت.versions[0].oid,
	       &b->branch_درخت.versions[1].oid);
}

ساکن خالی parse_from_existing(ساخت branch *b)
{
	اگر (is_null_oid(&b->oid)) {
		oidclr(&b->branch_درخت.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_درخت.versions[1].oid, the_repository->hash_algo);
	} ورنہ {
		غیرعلامتی طویل سائز;
		سائز_t سائز_st = 0;
		حرف *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &سائز_st, &b->oid);
		سائز = cast_سائز_t_مقصد_ulong(سائز_st);
		parse_from_commit(b, buf, سائز);
		آزاد(buf);
	}
}

ساکن صحیح parse_objectish(ساخت fast_import_state *state,
			   ساخت branch *b,
			   ثابت حرف *objectish)
{
	ساخت branch *s;
	ساخت object_id oid;

	oidcpy(&oid, &b->branch_درخت.versions[1].oid);

	s = lookup_branch(objectish);
	اگر (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	ورنہ اگر (s) {
		ساخت object_id *t = &s->branch_درخت.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_درخت.versions[0].oid, t);
		oidcpy(&b->branch_درخت.versions[1].oid, t);
	} ورنہ اگر (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		ساخت object_اندراج *oe = ڈھونڈو_mark(marks, idnum);
		اگر (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		اگر (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			اگر (oe->pack_id != MAX_PACK_ID) {
				غیرعلامتی طویل سائز;
				حرف *buf = gfi_unpack_اندراج(oe, &سائز);
				parse_from_commit(b, buf, سائز);
				آزاد(buf);
			} ورنہ
				parse_from_existing(b);
		}
	} ورنہ اگر (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		اگر (is_null_oid(&b->oid))
			b->delete = 1;
	}
	ورنہ
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	اگر (b->branch_درخت.درخت && !oideq(&oid, &b->branch_درخت.versions[1].oid)) {
		release_درخت_content_recursive(b->branch_درخت.درخت);
		b->branch_درخت.درخت = تہی;
	}

	read_اگلا_command(state);
	واپس 1;
}

ساکن صحیح parse_from(ساخت fast_import_state *state, ساخت branch *b)
{
	ثابت حرف *from;

	اگر (!skip_سابقہ(command_buf.buf, "from ", &from))
		واپس 0;

	واپس parse_objectish(state, b, from);
}

ساکن صحیح parse_objectish_with_سابقہ(ساخت fast_import_state *state,
				       ساخت branch *b,
				       ثابت حرف *سابقہ)
{
	ثابت حرف *بنیاد;

	اگر (!skip_سابقہ(command_buf.buf, سابقہ, &بنیاد))
		واپس 0;

	واپس parse_objectish(state, b, بنیاد);
}

ساکن ساخت hash_list *parse_ملاؤ(ساخت fast_import_state *state,
				     غیرعلامتی صحیح *گنتی)
{
	ساخت hash_list *list = تہی, **دم = &list, *n;
	ثابت حرف *from;
	ساخت branch *s;

	*گنتی = 0;
	جب (skip_سابقہ(command_buf.buf, "merge ", &from)) {
		n = xmalloc(حجم(*n));
		s = lookup_branch(from);
		اگر (s)
			oidcpy(&n->oid, &s->oid);
		ورنہ اگر (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			ساخت object_اندراج *oe = ڈھونڈو_mark(marks, idnum);
			اگر (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} ورنہ اگر (!repo_get_oid(the_repository, from, &n->oid)) {
			سائز_t سائز;
			حرف *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &سائز, &n->oid);
			اگر (!buf || سائز < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			آزاد(buf);
		} ورنہ
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->اگلا = تہی;
		*دم = n;
		دم = &n->اگلا;

		(*گنتی)++;
		read_اگلا_command(state);
	}
	واپس list;
}

ساخت signature_ڈیٹا {
	حرف *hash_algo;      /* "sha1" or "sha256" */
	حرف *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	ساخت strbuf ڈیٹا;   /* The actual signature data */
};

ساکن خالی parse_one_signature(ساخت fast_import_state *state,
				ساخت signature_ڈیٹا *sig,
				ثابت حرف *v)
{
	حرف *دلائل = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	حرف *space = strchr(دلائل, ' ');

	اگر (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), دلائل);
	*space = '\0';

	sig->hash_algo = دلائل;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	اگر (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	اگر (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	اگر (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_اگلا_command(state);
	parse_ڈیٹا(&sig->ڈیٹا, 0, تہی);
}

ساکن خالی discard_one_signature(ساخت fast_import_state *state)
{
	ساخت strbuf ڈیٹا = STRBUF_INIT;

	read_اگلا_command(state);
	parse_ڈیٹا(&ڈیٹا, 0, تہی);
	strbuf_release(&ڈیٹا);
}

ساکن خالی جوڑو_gpgsig_مقصد_commit(ساخت strbuf *commit_ڈیٹا,
				 ثابت حرف *سرخی,
				 ساخت signature_ڈیٹا *sig)
{
	ساخت string_list siglines = STRING_LIST_INIT_NODUP;

	اگر (!sig || !sig->hash_algo)
		واپس;

	strbuf_addstr(commit_ڈیٹا, سرخی);
	string_list_بانٹو_اندر_رکھو(&siglines, sig->ڈیٹا.buf, "\n", -1);
	strbuf_جوڑو_separated_string_list(commit_ڈیٹا, "\n ", &siglines);
	strbuf_addch(commit_ڈیٹا, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->ڈیٹا);
	آزاد(sig->hash_algo);
}

ساکن خالی store_signature(ساخت signature_ڈیٹا *stored_sig,
			    ساخت signature_ڈیٹا *new_sig,
			    ثابت حرف *hash_type)
{
	اگر (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->ڈیٹا);
		آزاد(new_sig->hash_algo);
	} ورنہ {
		*stored_sig = *new_sig;
	}
}

ساکن خالی import_one_signature(ساخت fast_import_state *state,
				 ساخت signature_ڈیٹا *sig_sha1,
				 ساخت signature_ڈیٹا *sig_sha256,
				 ثابت حرف *v)
{
	ساخت signature_ڈیٹا sig = { تہی, تہی, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	اگر (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	ورنہ اگر (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	ورنہ
		die(_("parse_one_signature() returned unknown hash algo"));
}

ساکن خالی finalize_commit_بفر(ساخت strbuf *new_ڈیٹا,
				   ساخت signature_ڈیٹا *sig_sha1,
				   ساخت signature_ڈیٹا *sig_sha256,
				   ساخت strbuf *پیغام)
{
	جوڑو_gpgsig_مقصد_commit(new_ڈیٹا, "gpgsig ", sig_sha1);
	جوڑو_gpgsig_مقصد_commit(new_ڈیٹا, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_ڈیٹا, '\n');
	strbuf_addbuf(new_ڈیٹا, پیغام);
}

ساکن خالی warn_invalid_signature(ساخت signature_check *check,
				   ثابت حرف *پیغام, شمار sign_منوال منوال)
{
	ثابت حرف *signer = check->signer ? check->signer : _("unknown");
	ثابت حرف *subject;
	صحیح subject_len = ڈھونڈو_commit_subject(پیغام, &subject);

	انتخاب (منوال) {
	حالت SIGN_STRIP_IF_INVALID:
		اگر (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		ورنہ اگر (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		ورنہ
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		توڑو;
	حالت SIGN_SIGN_IF_INVALID:
		اگر (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		ورنہ اگر (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		ورنہ
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		توڑو;
	طےشدہ:
		BUG("unsupported signing mode");
	}
}

ساکن خالی handle_signature_if_invalid(ساخت strbuf *new_ڈیٹا,
					ساخت signature_ڈیٹا *sig_sha1,
					ساخت signature_ڈیٹا *sig_sha256,
					ساخت strbuf *پیغام,
					شمار sign_منوال منوال)
{
	ساخت strbuf tmp_buf = STRBUF_INIT;
	ساخت signature_check signature_check = { 0 };
	صحیح ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_ڈیٹا);
	finalize_commit_بفر(&tmp_buf, sig_sha1, sig_sha256, پیغام);
	ret = verify_commit_بفر(tmp_buf.buf, tmp_buf.len, &signature_check);

	اگر (ret) {
		اگر (منوال == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, پیغام->buf, منوال);

		اگر (منوال == SIGN_SIGN_IF_INVALID) {
			ساخت strbuf signature = STRBUF_INIT;
			ساخت strbuf payload = STRBUF_INIT;

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
			اگر (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			اگر (sign_بفر(&payload, &signature, signed_commit_keyid,
					SIGN_بفر_USE_DEFAULT_کلید))
				die(_("failed to sign commit object"));
			جوڑو_سرخی_signature(new_ڈیٹا, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_بفر(new_ڈیٹا, تہی, تہی, پیغام);
	} ورنہ {
		strbuf_تبادلہ(new_ڈیٹا, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

ساکن خالی parse_new_commit(ساخت fast_import_state *state, ثابت حرف *arg)
{
	ساکن ساخت strbuf پیغام = STRBUF_INIT;
	ساخت signature_ڈیٹا sig_sha1 = { تہی, تہی, STRBUF_INIT };
	ساخت signature_ڈیٹا sig_sha256 = { تہی, تہی, STRBUF_INIT };
	ساخت branch *b;
	حرف *author = تہی;
	حرف *committer = تہی;
	حرف *encoding = تہی;
	ساخت hash_list *ملاؤ_list = تہی;
	غیرعلامتی صحیح ملاؤ_گنتی;
	غیرعلامتی حرف پچھلا_fanout, new_fanout;
	ثابت حرف *v;

	b = lookup_branch(arg);
	اگر (!b)
		b = new_branch(arg);

	read_اگلا_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	اگر (skip_سابقہ(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_اگلا_command(state);
	}
	اگر (skip_سابقہ(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_اگلا_command(state);
	}
	اگر (!committer)
		die(_("expected committer but didn't get one"));

	جب (skip_سابقہ(command_buf.buf, "gpgsig ", &v)) {
		انتخاب (signed_commit_منوال) {

		/* First, modes that don't need the signature to be parsed */
		حالت SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		حالت SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		حالت SIGN_STRIP:
			discard_one_signature(state);
			توڑو;

		/* Second, modes that parse the signature */
		حالت SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		حالت SIGN_VERBATIM:
		حالت SIGN_STRIP_IF_INVALID:
		حالت SIGN_SIGN_IF_INVALID:
		حالت SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			توڑو;

		/* Third, BUG */
		طےشدہ:
			BUG("invalid signed_commit_mode value %d", signed_commit_منوال);
		}
		read_اگلا_command(state);
	}

	اگر (skip_سابقہ(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_اگلا_command(state);
	}
	parse_ڈیٹا(&پیغام, 0, تہی);
	read_اگلا_command(state);
	parse_from(state, b);
	ملاؤ_list = parse_ملاؤ(state, &ملاؤ_گنتی);

	/* ensure the branch is active/loaded */
	اگر (!b->branch_درخت.درخت || !max_فعال_branches) {
		unload_one_branch();
		بھرو_branch(b);
	}

	پچھلا_fanout = convert_عدد_notes_مقصد_fanout(b->عدد_notes);

	/* file_change* */
	جب (command_buf.len > 0) {
		اگر (skip_سابقہ(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &پچھلا_fanout);
		ورنہ اگر (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		ورنہ {
			unread_command_buf = 1;
			توڑو;
		}
		اگر (read_اگلا_command(state) == EOF)
			توڑو;
	}

	new_fanout = convert_عدد_notes_مقصد_fanout(b->عدد_notes);
	اگر (new_fanout != پچھلا_fanout)
		b->عدد_notes = change_note_fanout(&b->branch_درخت, new_fanout);

	/* build the tree and the commit */
	store_درخت(&b->branch_درخت);
	oidcpy(&b->branch_درخت.versions[0].oid,
	       &b->branch_درخت.versions[1].oid);

	strbuf_reset(&new_ڈیٹا);
	strbuf_addf(&new_ڈیٹا, "tree %s\n",
		oid_مقصد_hex(&b->branch_درخت.versions[1].oid));
	اگر (!is_null_oid(&b->oid))
		strbuf_addf(&new_ڈیٹا, "parent %s\n",
			    oid_مقصد_hex(&b->oid));
	جب (ملاؤ_list) {
		ساخت hash_list *اگلا = ملاؤ_list->اگلا;
		strbuf_addf(&new_ڈیٹا, "parent %s\n",
			    oid_مقصد_hex(&ملاؤ_list->oid));
		آزاد(ملاؤ_list);
		ملاؤ_list = اگلا;
	}
	strbuf_addf(&new_ڈیٹا,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	اگر (encoding)
		strbuf_addf(&new_ڈیٹا,
			"encoding %s\n",
			encoding);

	اگر ((signed_commit_منوال == SIGN_STRIP_IF_INVALID ||
	     signed_commit_منوال == SIGN_SIGN_IF_INVALID ||
	     signed_commit_منوال == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_ڈیٹا, &sig_sha1, &sig_sha256,
					    &پیغام, signed_commit_منوال);
	ورنہ
		finalize_commit_بفر(&new_ڈیٹا, &sig_sha1, &sig_sha256, &پیغام);

	آزاد(author);
	آزاد(committer);
	آزاد(encoding);

	اگر (!store_object(OBJ_COMMIT, &new_ڈیٹا, تہی, &b->oid, اگلا_mark))
		b->pack_id = pack_id;
	b->آخری_commit = object_گنتی_by_type[OBJ_COMMIT];
}

ساکن خالی handle_لیبل_signature_if_invalid(ساخت strbuf *buf,
					    ساخت strbuf *پیغام,
					    سائز_t sig_انحراف)
{
	ساخت strbuf signature = STRBUF_INIT;
	ساخت strbuf payload = STRBUF_INIT;
	ساخت signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_جوڑو(&payload, پیغام->buf, sig_انحراف);
	strbuf_جوڑو(&signature, پیغام->buf + sig_انحراف, پیغام->len - sig_انحراف);

	sigc.payload_type = SIGNATURE_PAYLOAD_لیبل;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	اگر (!check_signature(&sigc, signature.buf, signature.len))
		جاؤ برآمد;

	اگر (signed_لیبل_منوال == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(پیغام, sig_انحراف);

	اگر (signed_لیبل_منوال == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = تہی;
		strbuf_reset(&signature);

		اگر (sign_بفر(&payload, &signature, signed_لیبل_keyid,
				SIGN_بفر_USE_DEFAULT_کلید))
			die(_("failed to sign tag object"));

		strbuf_addbuf(پیغام, &signature);
	}

برآمد:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

ساکن خالی handle_لیبل_signature(ساخت strbuf *buf, ساخت strbuf *پیغام, ثابت حرف *name)
{
	سائز_t sig_انحراف = parse_signed_بفر(پیغام->buf, پیغام->len);

	/* If there is no signature, there is nothing to do. */
	اگر (sig_انحراف >= پیغام->len)
		واپس;

	انتخاب (signed_لیبل_منوال) {

	/* First, modes that don't change anything */
	حالت SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	حالت SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		توڑو;

	/* Second, modes that remove the signature */
	حالت SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	حالت SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(پیغام, sig_انحراف);
		توڑو;
	حالت SIGN_ABORT_IF_INVALID:
	حالت SIGN_SIGN_IF_INVALID:
	حالت SIGN_STRIP_IF_INVALID:
		handle_لیبل_signature_if_invalid(buf, پیغام, sig_انحراف);
		توڑو;

	/* Third, aborting modes */
	حالت SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	طےشدہ:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_لیبل_منوال, name);
	}
}

ساکن خالی parse_new_لیبل(ساخت fast_import_state *state, ثابت حرف *arg)
{
	ساکن ساخت strbuf پیغام = STRBUF_INIT;
	ثابت حرف *from;
	حرف *tagger;
	ساخت branch *s;
	ساخت لیبل *t;
	uintmax_t from_mark = 0;
	ساخت object_id oid;
	شمار object_type type;
	ثابت حرف *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, حجم(ساخت لیبل));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	اگر (آخری_لیبل)
		آخری_لیبل->اگلا_لیبل = t;
	ورنہ
		پہلا_لیبل = t;
	آخری_لیبل = t;
	read_اگلا_command(state);
	parse_mark(state);

	/* from ... */
	اگر (!skip_سابقہ(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	اگر (s) {
		اگر (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} ورنہ اگر (*from == ':') {
		ساخت object_اندراج *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = ڈھونڈو_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} ورنہ اگر (!repo_get_oid(the_repository, from, &oid)) {
		ساخت object_اندراج *oe = ڈھونڈو_object(&oid);
		اگر (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, تہی);
			اگر (type < 0)
				die(_("not a valid object: %s"), from);
		} ورنہ
			type = oe->type;
	} ورنہ
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_اگلا_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	اگر (skip_سابقہ(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_اگلا_command(state);
	} ورنہ
		tagger = تہی;

	/* tag payload/message */
	parse_ڈیٹا(&پیغام, 0, تہی);

	/* build the tag object */
	strbuf_reset(&new_ڈیٹا);

	strbuf_addf(&new_ڈیٹا,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_مقصد_hex(&oid), type_name(type), t->name);
	اگر (tagger)
		strbuf_addf(&new_ڈیٹا,
			    "tagger %s\n", tagger);

	handle_لیبل_signature(&new_ڈیٹا, &پیغام, t->name);

	strbuf_addch(&new_ڈیٹا, '\n');
	strbuf_addbuf(&new_ڈیٹا, &پیغام);
	آزاد(tagger);

	اگر (store_object(OBJ_لیبل, &new_ڈیٹا, تہی, &t->oid, اگلا_mark))
		t->pack_id = MAX_PACK_ID;
	ورنہ
		t->pack_id = pack_id;
}

ساکن خالی parse_reset_branch(ساخت fast_import_state *state, ثابت حرف *arg)
{
	ساخت branch *b;
	ثابت حرف *لیبل_name;

	b = lookup_branch(arg);
	اگر (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_درخت.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_درخت.versions[1].oid, the_repository->hash_algo);
		اگر (b->branch_درخت.درخت) {
			release_درخت_content_recursive(b->branch_درخت.درخت);
			b->branch_درخت.درخت = تہی;
		}
	}
	ورنہ
		b = new_branch(arg);
	read_اگلا_command(state);
	parse_from(state, b);
	اگر (b->delete && skip_سابقہ(b->name, "refs/tags/", &لیبل_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		ساخت لیبل *t, *پچھلا_2 = تہی;
		برائے (t = پہلا_لیبل; t; t = t->اگلا_لیبل) {
			اگر (!strcmp(t->name, لیبل_name))
				توڑو;
			پچھلا_2 = t;
		}
		اگر (t) {
			اگر (پچھلا_2)
				پچھلا_2->اگلا_لیبل = t->اگلا_لیبل;
			ورنہ
				پہلا_لیبل = t->اگلا_لیبل;
			اگر (!t->اگلا_لیبل)
				آخری_لیبل = پچھلا_2;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	اگر (command_buf.len > 0)
		unread_command_buf = 1;
}

ساکن خالی cat_blob_write(ثابت حرف *buf, غیرعلامتی طویل سائز)
{
	اگر (write_اندر_بھرا(cat_blob_fd, buf, سائز) < 0)
		die_errno(_("write to frontend failed"));
}

ساکن خالی cat_blob(ساخت object_اندراج *oe, ساخت object_id *oid)
{
	ساخت strbuf سطر = STRBUF_INIT;
	ساخت iovec iov[3];
	غیرعلامتی طویل سائز;
	شمار object_type type = 0;
	حرف *buf;

	اگر (!oe || oe->pack_id == MAX_PACK_ID) {
		سائز_t سائز_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &سائز_st);
		سائز = cast_سائز_t_مقصد_ulong(سائز_st);
	} ورنہ {
		type = oe->type;
		buf = gfi_unpack_اندراج(oe, &سائز);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	اگر (type <= 0) {
		strbuf_reset(&سطر);
		strbuf_addf(&سطر, "%s missing\n", oid_مقصد_hex(oid));
		cat_blob_write(سطر.buf, سطر.len);
		strbuf_release(&سطر);
		آزاد(buf);
		واپس;
	}
	اگر (!buf)
		die(_("can't read object %s"), oid_مقصد_hex(oid));
	اگر (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_مقصد_hex(oid), type_name(type));
	strbuf_reset(&سطر);
	strbuf_addf(&سطر, "%s %s %"PRIuMAX"\n", oid_مقصد_hex(oid),
		    type_name(type), (uintmax_t)سائز);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_بنیاد = سطر.buf;
	iov[0].iov_len = سطر.len;
	iov[1].iov_بنیاد = buf;
	iov[1].iov_len = سائز;
	iov[2].iov_بنیاد = (خالی *) "\n";
	iov[2].iov_len = 1;

	اگر (writev_اندر_بھرا(cat_blob_fd, iov, ARRAY_سائز(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&سطر);
	اگر (oe && oe->pack_id == pack_id) {
		آخری_blob.انحراف = oe->idx.انحراف;
		strbuf_attach(&آخری_blob.ڈیٹا, buf, سائز, سائز + 1);
		آخری_blob.گہرائی = oe->گہرائی;
	} ورنہ
		آزاد(buf);
}

ساکن خالی parse_get_mark(ساخت fast_import_state *state غیر_مستعمل,
			   ثابت حرف *p)
{
	ساخت object_اندراج *oe;
	حرف output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	اگر (*p != ':')
		die(_("not a mark: %s"), p);

	oe = ڈھونڈو_mark(marks, parse_mark_ref_eol(p));
	اگر (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, حجم(output), "%s\n", oid_مقصد_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

ساکن خالی parse_cat_blob(ساخت fast_import_state *state غیر_مستعمل,
			   ثابت حرف *p)
{
	ساخت object_اندراج *oe;
	ساخت object_id oid;

	/* cat-blob SP <object> LF */
	اگر (*p == ':') {
		oe = ڈھونڈو_mark(marks, parse_mark_ref_eol(p));
		اگر (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} ورنہ {
		اگر (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		اگر (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = ڈھونڈو_object(&oid);
	}

	cat_blob(oe, &oid);
}

ساکن ساخت object_اندراج *dereference(ساخت object_اندراج *oe,
					ساخت object_id *oid)
{
	غیرعلامتی طویل سائز;
	حرف *buf = تہی;
	ثابت غیرعلامتی hexsz = the_hash_algo->hexsz;

	اگر (!oe) {
		شمار object_type type = odb_read_object_info(the_repository->objects,
							     oid, تہی);
		اگر (type < 0)
			die(_("object not found: %s"), oid_مقصد_hex(oid));
		/* cache it! */
		oe = داخل_کرو_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.انحراف = 1;
	}
	انتخاب (oe->type) {
	حالت OBJ_درخت:	/* easy case. */
		واپس oe;
	حالت OBJ_COMMIT:
	حالت OBJ_لیبل:
		توڑو;
	طےشدہ:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	اگر (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_اندراج(oe, &سائز);
	} ورنہ {
		شمار object_type غیر_مستعمل_2;
		سائز_t سائز_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &غیر_مستعمل_2, &سائز_st);
		سائز = cast_سائز_t_مقصد_ulong(سائز_st);
	}
	اگر (!buf)
		die(_("can't load object %s"), oid_مقصد_hex(oid));

	/* Peel one layer. */
	انتخاب (oe->type) {
	حالت OBJ_لیبل:
		اگر (سائز < hexsz + لڑی_لمبائی("object ") ||
		    get_oid_hex(buf + لڑی_لمبائی("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		توڑو;
	حالت OBJ_COMMIT:
		اگر (سائز < hexsz + لڑی_لمبائی("tree ") ||
		    get_oid_hex(buf + لڑی_لمبائی("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	آزاد(buf);
	واپس ڈھونڈو_object(oid);
}

ساکن خالی داخل_کرو_mapped_mark(uintmax_t mark, خالی *object, خالی *cbp)
{
	ساخت object_id *fromoid = object;
	ساخت object_id *tooid = ڈھونڈو_mark(cbp, mark);
	صحیح ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	اگر (ret == 0)
		واپس;
	kh_قدر(sub_oid_map, it) = tooid;
}

ساکن خالی build_mark_map_one(ساخت mark_سیٹ *from, ساخت mark_سیٹ *مقصد)
{
	for_each_mark(from, 0, داخل_کرو_mapped_mark, مقصد);
}

ساکن خالی build_mark_map(ساخت string_list *from, ساخت string_list *مقصد)
{
	ساخت string_list_شے *fromp, *چوٹی;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_شے(fromp, from) {
		چوٹی = string_list_lookup(مقصد, fromp->string);
		اگر (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} ورنہ اگر (!چوٹی || !چوٹی->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, چوٹی->util);
	}
}

ساکن ساخت object_اندراج *parse_treeish_dataref(ثابت حرف **p)
{
	ساخت object_id oid;
	ساخت object_اندراج *e;

	اگر (**p == ':') {	/* <mark> */
		e = ڈھونڈو_mark(marks, parse_mark_ref_space(p));
		اگر (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} ورنہ {	/* <sha1> */
		اگر (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = ڈھونڈو_object(&oid);
		اگر (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	جب (!e || e->type != OBJ_درخت)
		e = dereference(e, &oid);
	واپس e;
}

ساکن خالی print_ls(صحیح منوال, ثابت غیرعلامتی حرف *hash, ثابت حرف *راستہ)
{
	ساکن ساخت strbuf سطر = STRBUF_INIT;

	/* See show_tree(). */
	ثابت حرف *type =
		S_ISGITLINK(منوال) ? commit_type :
		S_ISDIR(منوال) ? درخت_type :
		blob_type;

	اگر (!منوال) {
		/* missing SP path LF */
		strbuf_reset(&سطر);
		strbuf_addstr(&سطر, "missing ");
		quote_c_style(راستہ, &سطر, تہی, 0);
		strbuf_addch(&سطر, '\n');
	} ورنہ {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&سطر);
		strbuf_addf(&سطر, "%06o %s %s\t",
				منوال & ~NO_DELTA, type, hash_مقصد_hex(hash));
		quote_c_style(راستہ, &سطر, تہی, 0);
		strbuf_addch(&سطر, '\n');
	}
	cat_blob_write(سطر.buf, سطر.len);
}

ساکن خالی parse_ls(ساخت fast_import_state *state غیر_مستعمل,
		     ثابت حرف *p,
		     ساخت branch *b)
{
	ساکن ساخت strbuf راستہ = STRBUF_INIT;
	ساخت درخت_اندراج *جڑ = تہی;
	ساخت درخت_اندراج برگ = {تہی};

	/* ls SP (<tree-ish> SP)? <path> */
	اگر (*p == '"') {
		اگر (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		جڑ = &b->branch_درخت;
	} ورنہ {
		ساخت object_اندراج *e = parse_treeish_dataref(&p);
		جڑ = new_درخت_اندراج();
		oidcpy(&جڑ->versions[1].oid, &e->idx.oid);
		اگر (!is_null_oid(&جڑ->versions[1].oid))
			جڑ->versions[1].منوال = S_IFDIR;
		بھرو_درخت(جڑ);
	}
	strbuf_reset(&راستہ);
	parse_راستہ_eol(&راستہ, p, "path");
	درخت_content_get(جڑ, راستہ.buf, &برگ, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	اگر (S_ISDIR(برگ.versions[1].منوال))
		store_درخت(&برگ);

	print_ls(برگ.versions[1].منوال, برگ.versions[1].oid.hash, راستہ.buf);
	اگر (برگ.درخت)
		release_درخت_content_recursive(برگ.درخت);
	اگر (!b || جڑ != &b->branch_درخت)
		release_درخت_اندراج(جڑ);
}

ساکن خالی checkpoint(خالی)
{
	checkpoint_requested = 0;
	اگر (object_گنتی) {
		چکر_packfile();
	}
	dump_branches();
	dump_لیبل_فہرست();
	dump_marks();
}

ساکن خالی parse_checkpoint(ساخت fast_import_state *state غیر_مستعمل)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

ساکن خالی parse_پیش_رفت(ساخت fast_import_state *state غیر_مستعمل)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

ساکن خالی parse_alias(ساخت fast_import_state *state)
{
	ساخت object_اندراج *e;
	ساخت branch b;

	skip_optional_lf();
	read_اگلا_command(state);

	/* mark ... */
	parse_mark(state);
	اگر (!اگلا_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, حجم(b));
	اگر (!parse_objectish_with_سابقہ(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = ڈھونڈو_object(&b.oid);
	أكد(e);
	داخل_کرو_mark(&marks, اگلا_mark, e);
}

ساکن حرف* make_fast_import_راستہ(ساخت fast_import_state *state,
				   ثابت حرف *راستہ)
{
	اگر (!relative_marks_paths || is_absolute_راستہ(راستہ))
		واپس سابقہ_filename(state->سابقہ, راستہ);
	واپس repo_git_راستہ(the_repository, "info/fast-import/%s", راستہ);
}

ساکن خالی option_import_marks(ساخت fast_import_state *state,
				ثابت حرف *marks,
				صحیح from_دھارا,
				صحیح ignore_missing)
{
	اگر (import_marks_file) {
		اگر (from_دھارا)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		اگر(!import_marks_file_from_دھارا)
			read_marks();
	}

	آزاد(import_marks_file);
	import_marks_file = make_fast_import_راستہ(state, marks);
	import_marks_file_from_دھارا = from_دھارا;
	import_marks_file_ignore_missing = ignore_missing;
}

ساکن خالی option_date_format(ثابت حرف *fmt)
{
	اگر (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	ورنہ اگر (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	ورنہ اگر (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	ورنہ اگر (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	ورنہ
		die(_("unknown --date-format argument %s"), fmt);
}

ساکن غیرعلامتی طویل ulong_arg(ثابت حرف *option, ثابت حرف *arg)
{
	حرف *endptr;
	غیرعلامتی طویل rv = strtoul(arg, &endptr, 0);
	اگر (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	واپس rv;
}

ساکن خالی option_گہرائی(ثابت حرف *گہرائی)
{
	max_گہرائی = ulong_arg("--depth", گہرائی);
	اگر (max_گہرائی > MAX_گہرائی)
		die(_("--depth cannot exceed %u"), MAX_گہرائی);
}

ساکن خالی option_فعال_branches(ثابت حرف *branches)
{
	max_فعال_branches = ulong_arg("--active-branches", branches);
}

ساکن خالی option_export_marks(ساخت fast_import_state *state,
				ثابت حرف *marks)
{
	آزاد(export_marks_file);
	export_marks_file = make_fast_import_راستہ(state, marks);
}

ساکن خالی option_cat_blob_fd(ساخت fast_import_state *state غیر_مستعمل,
			       ثابت حرف *fd)
{
	غیرعلامتی طویل n = ulong_arg("--cat-blob-fd", fd);
	اگر (n > (غیرعلامتی طویل) زیادہ_سے_زیادہ_عدد)
		die(_("--cat-blob-fd cannot exceed %d"), زیادہ_سے_زیادہ_عدد);
	cat_blob_fd = (صحیح) n;
}

ساکن خالی option_export_pack_کنارے(ساخت fast_import_state *state,
				     ثابت حرف *کنارے)
{
	حرف *fn = سابقہ_filename(state->سابقہ, کنارے);
	اگر (pack_کنارے)
		fclose(pack_کنارے);
	pack_کنارے = xfopen(fn, "a");
	آزاد(fn);
}

ساکن خالی option_rewrite_submodules(ساخت fast_import_state *state,
				      ثابت حرف *arg,
				      ساخت string_list *list)
{
	ساخت mark_سیٹ *ms;
	FILE *fp;
	حرف *s = xstrdup(arg);
	حرف *f = strchr(s, ':');
	اگر (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = سابقہ_filename(state->سابقہ, f);
	fp = fopen(f, "r");
	اگر (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, داخل_کرو_oid_اندراج);
	fclose(fp);
	آزاد(f);

	string_list_داخل_کرو(list, s)->util = ms;

	آزاد(s);
}

ساکن خالی option_max_pack_سائز(ثابت حرف *arg)
{
	غیرعلامتی طویل v;

	اگر (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	اگر (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} ورنہ اگر (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

ساکن خالی option_big_file_حد(ثابت حرف *arg)
{
	غیرعلامتی طویل v;

	اگر (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_سیٹ_big_file_حد(the_repository, v);
}

ساکن خالی option_signed_commits(ثابت حرف *arg)
{
	اگر (parse_sign_منوال(arg, &signed_commit_منوال, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

ساکن خالی option_signed_لیبل_فہرست(ثابت حرف *arg)
{
	اگر (parse_sign_منوال(arg, &signed_لیبل_منوال, &signed_لیبل_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

ساکن خالی option_quiet(خالی)
{
	show_stats = 0;
	quiet = 1;
}

ساکن صحیح parse_one_option(ساخت fast_import_state *state, ثابت حرف *option)
{
	اگر (skip_سابقہ(option, "max-pack-size=", &option)) {
		option_max_pack_سائز(option);
	} ورنہ اگر (skip_سابقہ(option, "big-file-threshold=", &option)) {
		option_big_file_حد(option);
	} ورنہ اگر (skip_سابقہ(option, "depth=", &option)) {
		option_گہرائی(option);
	} ورنہ اگر (skip_سابقہ(option, "active-branches=", &option)) {
		option_فعال_branches(option);
	} ورنہ اگر (skip_سابقہ(option, "export-pack-edges=", &option)) {
		option_export_pack_کنارے(state, option);
	} ورنہ اگر (skip_سابقہ(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} ورنہ اگر (skip_سابقہ(option, "signed-tags=", &option)) {
		option_signed_لیبل_فہرست(option);
	} ورنہ اگر (!strcmp(option, "quiet")) {
		option_quiet();
	} ورنہ اگر (!strcmp(option, "stats")) {
		show_stats = 1;
	} ورنہ اگر (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} ورنہ {
		واپس 0;
	}

	واپس 1;
}

ساکن خالی check_unsafe_feature(ساخت fast_import_state *state,
				 ثابت حرف *feature)
{
	اگر (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

ساکن صحیح parse_one_feature(ساخت fast_import_state *state,
			     ثابت حرف *feature)
{
	ثابت حرف *arg;

	اگر (skip_سابقہ(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} ورنہ اگر (skip_سابقہ(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} ورنہ اگر (skip_سابقہ(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} ورنہ اگر (skip_سابقہ(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} ورنہ اگر (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} ورنہ اگر (skip_سابقہ(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_مقصد);
	} ورنہ اگر (skip_سابقہ(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} ورنہ اگر (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} ورنہ اگر (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} ورنہ اگر (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} ورنہ اگر (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} ورنہ اگر (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} ورنہ اگر (!strcmp(feature, "force")) {
		force_update = 1;
	} ورنہ اگر (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} ورنہ {
		واپس 0;
	}

	واپس 1;
}

ساکن خالی parse_feature(ساخت fast_import_state *state, ثابت حرف *feature)
{
	اگر (state->نظر_آیا_ڈیٹا_command)
		die(_("got feature command '%s' after data command"), feature);

	اگر (parse_one_feature(state, feature))
		واپس;

	die(_("this version of fast-import does not support feature %s."), feature);
}

ساکن خالی parse_option(ساخت fast_import_state *state, ثابت حرف *option)
{
	اگر (state->نظر_آیا_ڈیٹا_command)
		die(_("got option command '%s' after data command"), option);

	اگر (parse_one_option(state, option))
		واپس;

	die(_("this version of fast-import does not support option: %s"), option);
}

ساکن خالی git_pack_config(خالی)
{
	صحیح indexversion_قدر;
	صحیح حد_بندی;
	غیرعلامتی طویل packsizelimit_قدر;

	اگر (!repo_config_get_ulong(the_repository, "pack.depth", &max_گہرائی)) {
		اگر (max_گہرائی > MAX_گہرائی)
			max_گہرائی = MAX_گہرائی;
	}
	اگر (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_قدر)) {
		pack_idx_opts.نسخہ = indexversion_قدر;
		اگر (pack_idx_opts.نسخہ > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.نسخہ);
	}
	اگر (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_قدر))
		max_packsize = packsizelimit_قدر;

	اگر (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &حد_بندی))
		unpack_حد_بندی = حد_بندی;
	ورنہ اگر (!repo_config_get_int(the_repository, "transfer.unpacklimit", &حد_بندی))
		unpack_حد_بندی = حد_بندی;

	repo_config(the_repository, git_default_config, تہی);
}

ساکن ثابت حرف *ثابت fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	تہی
};

ساکن خالی parse_argv(ساخت fast_import_state *state)
{
	صحیح argc = parse_options(state->argc, state->argv, state->سابقہ,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	اگر (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->نظر_آیا_ڈیٹا_command = 1;
	اگر (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_مقصد);
}

ساکن صحیح option_parse_date_format(ثابت ساخت option *opt غیر_مستعمل,
				    ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	واپس 0;
}

ساکن صحیح option_parse_export_pack_کنارے(ثابت ساخت option *opt,
					  ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_کنارے(opt->قدر, arg);
	واپس 0;
}

ساکن صحیح option_parse_max_pack_سائز(ثابت ساخت option *opt غیر_مستعمل,
				      ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_سائز(arg);
	واپس 0;
}

ساکن صحیح option_parse_big_file_حد(ثابت ساخت option *opt غیر_مستعمل,
					   ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_حد(arg);
	واپس 0;
}

ساکن صحیح option_parse_signed_commits(ثابت ساخت option *opt غیر_مستعمل,
				       ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	واپس 0;
}

ساکن صحیح option_parse_signed_لیبل_فہرست(ثابت ساخت option *opt غیر_مستعمل,
				    ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_لیبل_فہرست(arg);
	واپس 0;
}

ساکن صحیح option_parse_rewrite_submodules_from(ثابت ساخت option *opt,
						ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->قدر, arg, &sub_marks_from);
	واپس 0;
}

ساکن صحیح option_parse_rewrite_submodules_مقصد(ثابت ساخت option *opt,
					      ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->قدر, arg, &sub_marks_مقصد);
	واپس 0;
}

ساکن صحیح option_parse_cat_blob_fd(ثابت ساخت option *opt,
				    ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->قدر, arg);
	واپس 0;
}

ساکن صحیح option_parse_import_marks(ثابت ساخت option *opt,
				     ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->قدر, arg, 0, 0);
	واپس 0;
}

ساکن صحیح option_parse_import_marks_if_exists(ثابت ساخت option *opt,
					       ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->قدر, arg, 0, 1);
	واپس 0;
}

ساکن صحیح option_parse_export_marks(ثابت ساخت option *opt,
				     ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->قدر, arg);
	واپس 0;
}

ساکن صحیح option_parse_گہرائی(ثابت ساخت option *opt غیر_مستعمل,
			      ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_گہرائی(arg);
	واپس 0;
}

ساکن صحیح option_parse_فعال_branches(ثابت ساخت option *opt غیر_مستعمل,
					ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_فعال_branches(arg);
	واپس 0;
}

ساکن صحیح option_parse_quiet(ثابت ساخت option *opt غیر_مستعمل,
			      ثابت حرف *arg غیر_مستعمل, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	واپس 0;
}

صحیح cmd_fast_import(صحیح argc,
		    ثابت حرف **argv,
		    ثابت حرف *سابقہ,
		    ساخت repository *repo)
{
	ساخت fast_import_state state;

	ساخت option fast_import_options[] = {
		OPT_گروہ(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", تہی, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", تہی, تہی,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", تہی, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_سائز),
		OPT_CALLBACK_F(0, "big-file-threshold", تہی, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_حد),
		OPT_CALLBACK_F(0, "depth", تہی, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_گہرائی),
		OPT_CALLBACK_F(0, "active-branches", تہی, N_("n"),
			       N_("maximum number of branches to maintain active"),
			       PARSE_OPT_NONEG, option_parse_فعال_branches),
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
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_مقصد),
		OPT_گروہ(N_("Signing")),
		OPT_CALLBACK_F(0, "signed-commits", تہی, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", تہی, N_("mode"),
			       N_("how to handle signed tags"),
			       PARSE_OPT_NONEG, option_parse_signed_لیبل_فہرست),
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
		OPT_انتہا()
	};

	show_usage_with_options_if_asked(argc, argv, fast_import_usage, fast_import_options);

	fast_import_state_init(&state, argc, argv, سابقہ, fast_import_options);

	reset_pack_idx_option(&pack_idx_opts);
	git_pack_config();

	alloc_objects(object_اندراج_alloc);
	strbuf_init(&command_buf, 0);
	CALLOC_ARRAY(atom_جدول, atom_جدول_sz);
	CALLOC_ARRAY(branch_جدول, branch_جدول_sz);
	CALLOC_ARRAY(avail_درخت_جدول, avail_درخت_جدول_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, حجم(ساخت mark_سیٹ));

	hashmap_init(&object_جدول, object_اندراج_hashcmp, تہی, 0);

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
	برائے (صحیح i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		اگر (*arg != '-' || !strcmp(arg, "--"))
			توڑو;
		اگر (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * حجم(*rc_free));
	برائے (غیرعلامتی صحیح i = 0; i < (cmd_save - 1); i++)
		rc_free[i].اگلا = &rc_free[i + 1];
	rc_free[cmd_save - 1].اگلا = تہی;

	آغاز_packfile();
	سیٹ_die_routine(die_nicely);
	سیٹ_checkpoint_signal();
	جب (read_اگلا_command(&state) != EOF) {
		ثابت حرف *v;
		اگر (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "tag ", &v))
			parse_new_لیبل(&state, v);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, تہی);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		ورنہ اگر (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		ورنہ اگر (!strcmp("done", command_buf.buf))
			توڑو;
		ورنہ اگر (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		ورنہ اگر (starts_with(command_buf.buf, "progress "))
			parse_پیش_رفت(&state);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		ورنہ اگر (skip_سابقہ(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		ورنہ اگر (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		ورنہ
			die(_("unsupported command: %s"), command_buf.buf);

		اگر (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	اگر (!state.نظر_آیا_ڈیٹا_command)
		parse_argv(&state);

	اگر (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	انتہا_packfile();

	dump_branches();
	dump_لیبل_فہرست();
	unkeep_all_packs();
	dump_marks();

	اگر (pack_کنارے)
		fclose(pack_کنارے);

	اگر (show_stats) {
		uintmax_t کل_گنتی = 0, duplicate_گنتی = 0;
		برائے (سائز_t i = 0; i < ARRAY_سائز(object_گنتی_by_type); i++)
			کل_گنتی += object_گنتی_by_type[i];
		برائے (سائز_t i = 0; i < ARRAY_سائز(duplicate_گنتی_by_type); i++)
			duplicate_گنتی += duplicate_گنتی_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_گنتی);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", کل_گنتی, duplicate_گنتی);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_گنتی_by_type[OBJ_BLOB], duplicate_گنتی_by_type[OBJ_BLOB], delta_گنتی_by_type[OBJ_BLOB], delta_گنتی_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_گنتی_by_type[OBJ_درخت], duplicate_گنتی_by_type[OBJ_درخت], delta_گنتی_by_type[OBJ_درخت], delta_گنتی_attempts_by_type[OBJ_درخت]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_گنتی_by_type[OBJ_COMMIT], duplicate_گنتی_by_type[OBJ_COMMIT], delta_گنتی_by_type[OBJ_COMMIT], delta_گنتی_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_گنتی_by_type[OBJ_لیبل], duplicate_گنتی_by_type[OBJ_لیبل], delta_گنتی_by_type[OBJ_لیبل], delta_گنتی_attempts_by_type[OBJ_لیبل]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_گنتی, branch_بھرو_گنتی);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->سرکاؤ) * 1024, marks_سیٹ_گنتی);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (درخت_اندراج_allocd + fi_mem_pool.pool_alloc + alloc_گنتی*حجم(ساخت object_اندراج))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (غیرعلامتی طویل)((درخت_اندراج_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_گنتی*حجم(ساخت object_اندراج))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	واپس failure ? 1 : 0;
}
