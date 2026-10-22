#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

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

#تعریف_کړه PACK_ID_بټونه 16
#تعریف_کړه MAX_PACK_ID ((1<<PACK_ID_بټونه)-1)
#تعریف_کړه ژوروالی_بټونه 13
#تعریف_کړه MAX_ژوروالی ((1<<ژوروالی_بټونه)-1)

/*
 * We abuse the setuid bit on directories to mean "do not delta".
 */
#تعریف_کړه NO_DELTA S_ISUID

/*
 * The amount of additional space required in order to write an object into the
 * current pack. This is the hash lengths at the end of the pack, plus the
 * length of one object ID.
 */
#تعریف_کړه PACK_اندازه_پوله (the_hash_algo->rawsz * 3)

بنية object_ثبت {
	بنية pack_idx_ثبت idx;
	بنية hashmap_ثبت ent;
	uint32_t type : TYPE_بټونه,
		pack_id : PACK_ID_بټونه,
		ژوروالی : ژوروالی_بټونه;
};

ساكن صحيح object_ثبت_hashcmp(ثابت فراغ *map_معلومات ناکارول_شوی,
				ثابت بنية hashmap_ثبت *eptr,
				ثابت بنية hashmap_ثبت *ثبت_or_کلی,
				ثابت فراغ *keydata)
{
	ثابت بنية object_id *oid = keydata;
	ثابت بنية object_ثبت *e1, *e2;

	e1 = container_د(eptr, ثابت بنية object_ثبت, ent);
	إذا (oid)
		أرجع oidcmp(&e1->idx.oid, oid);

	e2 = container_د(ثبت_or_کلی, ثابت بنية object_ثبت, ent);
	أرجع oidcmp(&e1->idx.oid, &e2->idx.oid);
}

بنية object_ثبت_pool {
	بنية object_ثبت_pool *بل_pool;
	بنية object_ثبت *بل_free;
	بنية object_ثبت *پای_ټکی;
	بنية object_ثبت entries[FLEX_ARRAY]; /*نور*/
};

بنية mark_سېټ {
	اتحاد {
		بنية object_id *oids[1024];
		بنية object_ثبت *marked[1024];
		بنية mark_سېټ *sets[1024];
	} معلومات;
	بینښه صحيح ښويدنه;
};

بنية وروستی_object {
	بنية strbuf معلومات;
	off_t دننه_واټن;
	بینښه صحيح ژوروالی;
	بینښه no_بدلون : 1;
};

بنية atom_str {
	بنية atom_str *بل_atom;
	بینښه لنډ str_len;
	حرف str_dat[FLEX_ARRAY]; /*نور*/
};

بنية ونه_content;
بنية ونه_ثبت {
	بنية ونه_content *ونه;
	بنية atom_str *name;
	بنية ونه_ثبت_ms {
		uint16_t ډېرپېښ;
		بنية object_id oid;
	} versions[2];
};

بنية ونه_content {
	بینښه صحيح ثبت_ظرفیت; /* must match avail_tree_content */
	بینښه صحيح ثبت_شمېر;
	بینښه صحيح delta_ژوروالی;
	بنية ونه_ثبت *entries[FLEX_ARRAY]; /*نور*/
};

بنية avail_ونه_content {
	بینښه صحيح ثبت_ظرفیت; /* must match tree_content */
	بنية avail_ونه_content *بل_avail;
};

بنية branch {
	بنية branch *جدول_بل_branch;
	بنية branch *فعال_بل_branch;
	ثابت حرف *name;
	بنية ونه_ثبت branch_ونه;
	uintmax_t وروستی_commit;
	uintmax_t عدد_notes;
	بینښه فعال : 1;
	بینښه delete : 1;
	بینښه pack_id : PACK_ID_بټونه;
	بنية object_id oid;
};

بنية نښان {
	بنية نښان *بل_نښان;
	ثابت حرف *name;
	بینښه صحيح pack_id;
	بنية object_id oid;
};

بنية hash_list {
	بنية hash_list *بل;
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
	بنية recent_command *بل;
	حرف *buf;
};

تعريفنوع فراغ (*mark_سېټ_inserter_t)(بنية mark_سېټ **s, بنية object_id *oid, uintmax_t mark);
تعريفنوع فراغ (*each_mark_fn_t)(uintmax_t mark, فراغ *obj, فراغ *cbp);

/* Configured limits on output */
ساكن بینښه اوږد max_ژوروالی = 50;
ساكن off_t max_packsize;
ساكن صحيح unpack_حد = 100;
ساكن صحيح force_update;

/* Stats and misc. counters */
ساكن uintmax_t alloc_شمېر;
ساكن uintmax_t marks_سېټ_شمېر;
ساكن uintmax_t object_شمېر_by_type[1 << TYPE_بټونه];
ساكن uintmax_t duplicate_شمېر_by_type[1 << TYPE_بټونه];
ساكن uintmax_t delta_شمېر_by_type[1 << TYPE_بټونه];
ساكن uintmax_t delta_شمېر_attempts_by_type[1 << TYPE_بټونه];
ساكن بینښه اوږد object_شمېر;
ساكن بینښه اوږد branch_شمېر;
ساكن بینښه اوږد branch_بارول_شمېر;
ساكن صحيح failure;
ساكن FILE *pack_څنډې;
ساكن صحيح show_stats = 1;
ساكن صحيح quiet;
ساكن تعداد sign_ډېرپېښ signed_نښان_ډېرپېښ = SIGN_VERBATIM;
ساكن تعداد sign_ډېرپېښ signed_commit_ډېرپېښ = SIGN_VERBATIM;
ساكن ثابت حرف *signed_commit_keyid;
ساكن ثابت حرف *signed_نښان_keyid;

/* Memory pools */
ساكن بنية mem_pool fi_mem_pool = {
	.بلاک_alloc = 2*1024*1024 - حجم(بنية mp_بلاک),
};

/* Atom management */
ساكن بینښه صحيح atom_جدول_sz = 4451;
ساكن بینښه صحيح atom_cnt;
ساكن بنية atom_str **atom_جدول;

/* The .pack file being generated */
ساكن بنية pack_idx_option pack_idx_opts;
ساكن بینښه صحيح pack_id;
ساكن بنية hashfile *pack_file;
ساكن بنية packed_git *pack_معلومات;
ساكن بنية packed_git **all_packs;
ساكن off_t pack_اندازه;

/* Table of objects we've written. */
ساكن بینښه صحيح object_ثبت_alloc = 5000;
ساكن بنية object_ثبت_pool *بلاکونه;
ساكن بنية hashmap object_جدول;
ساكن بنية mark_سېټ *marks;
ساكن حرف *export_marks_file;
ساكن حرف *import_marks_file;
ساكن صحيح import_marks_file_from_بهیر;
ساكن صحيح import_marks_file_ignore_missing;
ساكن صحيح import_marks_file_بشپړ;
ساكن صحيح relative_marks_paths;

/* Our last blob */
ساكن بنية وروستی_object وروستی_blob = {
	.معلومات = STRBUF_INIT,
 };

/* Tree management */
ساكن بینښه صحيح ونه_ثبت_alloc = 1000;
ساكن فراغ *avail_ونه_ثبت;
ساكن بینښه صحيح avail_ونه_جدول_sz = 100;
ساكن بنية avail_ونه_content **avail_ونه_جدول;
ساكن اندازه_t ونه_ثبت_allocd;
ساكن بنية strbuf old_ونه = STRBUF_INIT;
ساكن بنية strbuf new_ونه = STRBUF_INIT;

/* Branch data */
ساكن بینښه اوږد max_فعال_branches = 5;
ساكن بینښه اوږد cur_فعال_branches;
ساكن بینښه اوږد branch_جدول_sz = 1039;
ساكن بنية branch **branch_جدول;
ساكن بنية branch *فعال_branches;

/* Tag data */
ساكن بنية نښان *لومړی_نښان;
ساكن بنية نښان *وروستی_نښان;

/* Input stream parsing */
ساكن whenspec_type whenspec = WHENSPEC_RAW;
ساكن بنية strbuf command_buf = STRBUF_INIT;
ساكن صحيح unread_command_buf;
ساكن بنية recent_command cmd_hist = {
	.prev = &cmd_hist,
	.بل = &cmd_hist,
};
ساكن بنية recent_command *cmd_لکۍ = &cmd_hist;
ساكن بنية recent_command *rc_free;
ساكن بینښه صحيح cmd_save = 100;
ساكن uintmax_t بل_mark;
ساكن بنية strbuf new_معلومات = STRBUF_INIT;
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
	ثابت حرف *مختاړی;
	صحيح کتل_شوی_معلومات_command;
	صحيح allow_unsafe_features;
	بنية option *option;
};

ساكن فراغ fast_import_state_init(بنية fast_import_state *state,
				   صحيح argc, ثابت حرف **argv,
				   ثابت حرف *مختاړی, بنية option *option)
{
	memset(state, 0, حجم(*state));
	state->argc = argc;
	state->argv = argv;
	state->مختاړی = مختاړی;
	state->option = option;
}

ساكن فراغ parse_argv(بنية fast_import_state *state);
ساكن فراغ parse_get_mark(بنية fast_import_state *state, ثابت حرف *p);
ساكن فراغ parse_cat_blob(بنية fast_import_state *state, ثابت حرف *p);
ساكن فراغ parse_ls(بنية fast_import_state *state,
		     ثابت حرف *p,
		     بنية branch *b);

ساكن فراغ for_each_mark(بنية mark_سېټ *m, uintmax_t بنسټ, each_mark_fn_t callback, فراغ *p)
{
	uintmax_t k;
	إذا (m->ښويدنه) {
		لكل (k = 0; k < 1024; k++) {
			إذا (m->معلومات.sets[k])
				for_each_mark(m->معلومات.sets[k], بنسټ + (k << m->ښويدنه), callback, p);
		}
	} وإلا {
		لكل (k = 0; k < 1024; k++) {
			إذا (m->معلومات.marked[k])
				callback(بنسټ + k, m->معلومات.marked[k], p);
		}
	}
}

ساكن فراغ dump_marks_fn(uintmax_t mark, فراغ *object, فراغ *cbp) {
	بنية object_ثبت *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_منزل_hex(&e->idx.oid));
}

ساكن فراغ write_branch_report(FILE *rpt, بنية branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	إذا (b->فعال)
		fputs(" active", rpt);
	إذا (b->branch_ونه.ونه)
		fputs(" loaded", rpt);
	إذا (is_null_oid(&b->branch_ونه.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_منزل_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_منزل_hex(&b->branch_ونه.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_منزل_hex(&b->branch_ونه.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->وروستی_commit);

	fputs("  last pack   : ", rpt);
	إذا (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

ساكن فراغ write_crash_report(ثابت حرف *err)
{
	حرف *loc = repo_git_لار(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	بنية branch *b;
	بینښه اوږد lu;
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
	fprintf(rpt, "    at %s\n", show_date(وخت(عدم), 0, DATE_ډېرپېښ(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	لكل (rc = cmd_hist.بل; rc != &cmd_hist; rc = rc->بل) {
		إذا (rc->بل == &cmd_hist)
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
	لكل (b = فعال_branches, lu = 0; b; b = b->فعال_بل_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->وروستی_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	لكل (lu = 0; lu < branch_جدول_sz; lu++) {
		لكل (b = branch_جدول[lu]; b; b = b->جدول_بل_branch)
			write_branch_report(rpt, b);
	}

	إذا (لومړی_نښان) {
		بنية نښان *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		لكل (tg = لومړی_نښان; tg; tg = tg->بل_نښان) {
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

ساكن فراغ پای_ټکی_packfile(فراغ);
ساكن فراغ unkeep_all_packs(فراغ);
ساكن فراغ dump_marks(فراغ);

ساكن NORETURN فراغ die_nicely(ثابت حرف *err, va_list params)
{
	va_list cp;
	ساكن صحيح zombie;
	report_fn die_پیغام_fn = get_die_پیغام_routine();

	va_copy(cp, params);
	die_پیغام_fn(err, params);

	إذا (!zombie) {
		حرف پیغام_2[2 * لار_MAX];

		zombie = 1;
		vsnprintf(پیغام_2, حجم(پیغام_2), err, cp);
		write_crash_report(پیغام_2);
		پای_ټکی_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	وتل(128);
}

#که_نه_تعریف_شوی SIGUSR1	/* Windows, for example */

ساكن فراغ سېټ_checkpoint_signal(فراغ)
{
}

#وإلا

ساكن فراغ checkpoint_signal(صحيح signo ناکارول_شوی)
{
	checkpoint_requested = 1;
}

ساكن فراغ سېټ_checkpoint_signal(فراغ)
{
	بنية sigaction sa;

	memset(&sa, 0, حجم(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_ماسک);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, عدم);
}

#د_که_پای

ساكن فراغ alloc_objects(بینښه صحيح cnt)
{
	بنية object_ثبت_pool *b;

	b = xmalloc(حجم(بنية object_ثبت_pool)
		+ cnt * حجم(بنية object_ثبت));
	b->بل_pool = بلاکونه;
	b->بل_free = b->entries;
	b->پای_ټکی = b->entries + cnt;
	بلاکونه = b;
	alloc_شمېر += cnt;
}

ساكن بنية object_ثبت *new_object(بنية object_id *oid)
{
	بنية object_ثبت *e;

	إذا (بلاکونه->بل_free == بلاکونه->پای_ټکی)
		alloc_objects(object_ثبت_alloc);

	e = بلاکونه->بل_free++;
	oidcpy(&e->idx.oid, oid);
	أرجع e;
}

ساكن بنية object_ثبت *پیدا_کړه_object(بنية object_id *oid)
{
	أرجع hashmap_get_ثبت_from_hash(&object_جدول, oidhash(oid), oid,
					   بنية object_ثبت, ent);
}

ساكن بنية object_ثبت *ورننباسه_object(بنية object_id *oid)
{
	بنية object_ثبت *e;
	بینښه صحيح hash = oidhash(oid);

	e = hashmap_get_ثبت_from_hash(&object_جدول, hash, oid,
					بنية object_ثبت, ent);
	إذا (!e) {
		e = new_object(oid);
		e->idx.دننه_واټن = 0;
		hashmap_ثبت_init(&e->ent, hash);
		hashmap_جمع_کړه(&object_جدول, &e->ent);
	}

	أرجع e;
}

ساكن فراغ invalidate_pack_id(بینښه صحيح id)
{
	بینښه اوږد lu;
	بنية نښان *t;
	بنية hashmap_iter iter;
	بنية object_ثبت *e;

	hashmap_for_each_ثبت(&object_جدول, &iter, e, ent) {
		إذا (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	لكل (lu = 0; lu < branch_جدول_sz; lu++) {
		بنية branch *b;

		لكل (b = branch_جدول[lu]; b; b = b->جدول_بل_branch)
			إذا (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	لكل (t = لومړی_نښان; t; t = t->بل_نښان)
		إذا (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

ساكن بینښه صحيح hc_str(ثابت حرف *s, اندازه_t len)
{
	بینښه صحيح r = 0;
	طالما (len-- > 0)
		r = r * 31 + *s++;
	أرجع r;
}

ساكن فراغ ورننباسه_mark(بنية mark_سېټ **سر_2, uintmax_t idnum, بنية object_ثبت *oe)
{
	بنية mark_سېټ *s = *سر_2;

	طالما ((idnum >> s->ښويدنه) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_سېټ));
		s->ښويدنه = (*سر_2)->ښويدنه + 10;
		s->معلومات.sets[0] = *سر_2;
		*سر_2 = s;
	}
	طالما (s->ښويدنه) {
		uintmax_t i = idnum >> s->ښويدنه;
		idnum -= i << s->ښويدنه;
		إذا (!s->معلومات.sets[i]) {
			s->معلومات.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_سېټ));
			s->معلومات.sets[i]->ښويدنه = s->ښويدنه - 10;
		}
		s = s->معلومات.sets[i];
	}
	إذا (!s->معلومات.marked[idnum])
		marks_سېټ_شمېر++;
	s->معلومات.marked[idnum] = oe;
}

ساكن فراغ *پیدا_کړه_mark(بنية mark_سېټ *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	بنية object_ثبت *oe = عدم;
	إذا ((idnum >> s->ښويدنه) < 1024) {
		طالما (s && s->ښويدنه) {
			uintmax_t i = idnum >> s->ښويدنه;
			idnum -= i << s->ښويدنه;
			s = s->معلومات.sets[i];
		}
		إذا (s)
			oe = s->معلومات.marked[idnum];
	}
	إذا (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	أرجع oe;
}

ساكن بنية atom_str *منزل_atom(ثابت حرف *s, بینښه لنډ len)
{
	بینښه صحيح hc = hc_str(s, len) % atom_جدول_sz;
	بنية atom_str *c;

	لكل (c = atom_جدول[hc]; c; c = c->بل_atom)
		إذا (c->str_len == len && !مزي_پرتله(s, c->str_dat, len))
			أرجع c;

	c = mem_pool_alloc(&fi_mem_pool, حجم(بنية atom_str) + len + 1);
	c->str_len = len;
	حافظه_نقل(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->بل_atom = atom_جدول[hc];
	atom_جدول[hc] = c;
	atom_cnt++;
	أرجع c;
}

ساكن بنية branch *lookup_branch(ثابت حرف *name)
{
	بینښه صحيح hc = hc_str(name, مزي_اوږدوالی(name)) % branch_جدول_sz;
	بنية branch *b;

	لكل (b = branch_جدول[hc]; b; b = b->جدول_بل_branch)
		إذا (!strcmp(name, b->name))
			أرجع b;
	أرجع عدم;
}

ساكن بنية branch *new_branch(ثابت حرف *name)
{
	بینښه صحيح hc = hc_str(name, مزي_اوږدوالی(name)) % branch_جدول_sz;
	بنية branch *b = lookup_branch(name);

	إذا (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	إذا (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->جدول_بل_branch = branch_جدول[hc];
	b->branch_ونه.versions[0].ډېرپېښ = S_IFDIR;
	b->branch_ونه.versions[1].ډېرپېښ = S_IFDIR;
	b->عدد_notes = 0;
	b->فعال = 0;
	b->pack_id = MAX_PACK_ID;
	branch_جدول[hc] = b;
	branch_شمېر++;
	أرجع b;
}

ساكن بینښه صحيح hc_entries(بینښه صحيح cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	أرجع cnt < avail_ونه_جدول_sz ? cnt : avail_ونه_جدول_sz - 1;
}

ساكن بنية ونه_content *new_ونه_content(بینښه صحيح cnt)
{
	بنية avail_ونه_content *f, *l = عدم;
	بنية ونه_content *t;
	بینښه صحيح hc = hc_entries(cnt);

	لكل (f = avail_ونه_جدول[hc]; f; l = f, f = f->بل_avail)
		إذا (f->ثبت_ظرفیت >= cnt)
			اكسر;

	إذا (f) {
		إذا (l)
			l->بل_avail = f->بل_avail;
		وإلا
			avail_ونه_جدول[hc] = f->بل_avail;
	} وإلا {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, حجم(*t) + حجم(t->entries[0]) * cnt);
		f->ثبت_ظرفیت = cnt;
	}

	t = (بنية ونه_content*)f;
	t->ثبت_شمېر = 0;
	t->delta_ژوروالی = 0;
	أرجع t;
}

ساكن فراغ release_ونه_ثبت(بنية ونه_ثبت *e);
ساكن فراغ release_ونه_content(بنية ونه_content *t)
{
	بنية avail_ونه_content *f = (بنية avail_ونه_content*)t;
	بینښه صحيح hc = hc_entries(f->ثبت_ظرفیت);
	f->بل_avail = avail_ونه_جدول[hc];
	avail_ونه_جدول[hc] = f;
}

ساكن فراغ release_ونه_content_recursive(بنية ونه_content *t)
{
	بینښه صحيح i;
	لكل (i = 0; i < t->ثبت_شمېر; i++)
		release_ونه_ثبت(t->entries[i]);
	release_ونه_content(t);
}

ساكن بنية ونه_content *grow_ونه_content(
	بنية ونه_content *t,
	صحيح amt)
{
	بنية ونه_content *r = new_ونه_content(t->ثبت_شمېر + amt);
	r->ثبت_شمېر = t->ثبت_شمېر;
	r->delta_ژوروالی = t->delta_ژوروالی;
	COPY_ARRAY(r->entries, t->entries, t->ثبت_شمېر);
	release_ونه_content(t);
	أرجع r;
}

ساكن بنية ونه_ثبت *new_ونه_ثبت(فراغ)
{
	بنية ونه_ثبت *e;

	إذا (!avail_ونه_ثبت) {
		بینښه صحيح n = ونه_ثبت_alloc;
		ونه_ثبت_allocd += n * حجم(بنية ونه_ثبت);
		ALLOC_ARRAY(e, n);
		avail_ونه_ثبت = e;
		طالما (n-- > 1) {
			*((فراغ**)e) = e + 1;
			e++;
		}
		*((فراغ**)e) = عدم;
	}

	e = avail_ونه_ثبت;
	avail_ونه_ثبت = *((فراغ**)e);
	أرجع e;
}

ساكن فراغ release_ونه_ثبت(بنية ونه_ثبت *e)
{
	إذا (e->ونه)
		release_ونه_content_recursive(e->ونه);
	*((فراغ**)e) = avail_ونه_ثبت;
	avail_ونه_ثبت = e;
}

ساكن بنية ونه_content *dup_ونه_content(بنية ونه_content *s)
{
	بنية ونه_content *d;
	بنية ونه_ثبت *a, *b;
	بینښه صحيح i;

	إذا (!s)
		أرجع عدم;
	d = new_ونه_content(s->ثبت_شمېر);
	لكل (i = 0; i < s->ثبت_شمېر; i++) {
		a = s->entries[i];
		b = new_ونه_ثبت();
		حافظه_نقل(b, a, حجم(*a));
		إذا (a->ونه && is_null_oid(&b->versions[1].oid))
			b->ونه = dup_ونه_content(a->ونه);
		وإلا
			b->ونه = عدم;
		d->entries[i] = b;
	}
	d->ثبت_شمېر = s->ثبت_شمېر;
	d->delta_ژوروالی = s->delta_ژوروالی;

	أرجع d;
}

ساكن فراغ پیل_packfile(فراغ)
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

	pack_معلومات = p;
	pack_اندازه = write_pack_سرلیک(pack_file, 0);
	object_شمېر = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

ساكن ثابت حرف *create_شاخص(فراغ)
{
	ثابت حرف *tmpfile;
	بنية pack_idx_ثبت **idx, **c, **وروستی;
	بنية object_ثبت *e;
	بنية object_ثبت_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_شمېر);
	c = idx;
	لكل (o = بلاکونه; o; o = o->بل_pool)
		لكل (e = o->بل_free; e-- != o->entries;)
			إذا (pack_id == e->pack_id)
				*c++ = &e->idx;
	وروستی = idx + object_شمېر;
	إذا (c != وروستی)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, عدم, idx, object_شمېر,
				 &pack_idx_opts, pack_معلومات->hash);
	حرر(idx);
	أرجع tmpfile;
}

ساكن حرف *keep_pack(ثابت حرف *curr_شاخص_name)
{
	ساكن ثابت حرف *keep_پیغام = "fast-import";
	بنية strbuf name = STRBUF_INIT;
	صحيح keep_fd;

	odb_pack_name(pack_معلومات->repo, &name, pack_معلومات->hash, "keep");
	keep_fd = خوندي_create_file_with_leading_directories(pack_معلومات->repo,
							    name.buf);
	إذا (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_پیغام, مزي_اوږدوالی(keep_پیغام));
	إذا (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_معلومات->repo, &name, pack_معلومات->hash, "pack");
	إذا (finalize_object_file(pack_معلومات->repo, pack_معلومات->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_معلومات->repo, &name, pack_معلومات->hash, "idx");
	إذا (finalize_object_file(pack_معلومات->repo, curr_شاخص_name, name.buf))
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
	بنية ماشوم_process unpack = ماشوم_PROCESS_INIT;

	إذا (lseek(p->pack_fd, 0, SEEK_سېټ) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.دننه = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_منزل_stderr = 1;
	strvec_ټېل(&unpack.دلیلونه, "unpack-objects");
	إذا (!show_stats)
		strvec_ټېل(&unpack.دلیلونه, "-q");

	أرجع پرلپسې_command(&unpack);
}

ساكن فراغ پای_ټکی_packfile(فراغ)
{
	ساكن صحيح running;

	إذا (running || !pack_معلومات)
		أرجع;

	running = 1;
	clear_delta_بنسټ_cache();
	إذا (object_شمېر) {
		بنية odb_سرچینه_files *files = odb_سرچینه_files_downcast(pack_معلومات->repo->objects->سرچینه);
		بنية packed_git *new_p;
		بنية object_id cur_pack_oid;
		حرف *idx_name;
		صحيح i;
		بنية branch *b;
		بنية نښان *t;

		close_pack_windows(pack_معلومات);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_سرلیک_footer(the_hash_algo, pack_معلومات->pack_fd,
					 pack_معلومات->hash, pack_معلومات->pack_name,
					 object_شمېر, cur_pack_oid.hash,
					 pack_اندازه);

		إذا (object_شمېر <= unpack_حد) {
			إذا (!loosen_small_pack(pack_معلومات)) {
				invalidate_pack_id(pack_id);
				اذهب discard_pack;
			}
		}

		close(pack_معلومات->pack_fd);
		idx_name = keep_pack(create_شاخص());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_بارول_pack(files->dirs->packed, idx_name, 1);
		إذا (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		حرر(idx_name);

		/* Print the boundary */
		إذا (pack_څنډې) {
			fprintf(pack_څنډې, "%s:", new_p->pack_name);
			لكل (i = 0; i < branch_جدول_sz; i++) {
				لكل (b = branch_جدول[i]; b; b = b->جدول_بل_branch) {
					إذا (b->pack_id == pack_id)
						fprintf(pack_څنډې, " %s",
							oid_منزل_hex(&b->oid));
				}
			}
			لكل (t = لومړی_نښان; t; t = t->بل_نښان) {
				إذا (t->pack_id == pack_id)
					fprintf(pack_څنډې, " %s",
						oid_منزل_hex(&t->oid));
			}
			fputc('\n', pack_څنډې);
			fflush(pack_څنډې);
		}

		pack_id++;
	}
	وإلا {
discard_pack:
		close(pack_معلومات->pack_fd);
		unlink_or_warn(pack_معلومات->pack_name);
	}
	FREE_AND_NULL(pack_معلومات);
	running = 0;

	/* We can't carry a delta across packfiles. */
	strbuf_release(&وروستی_blob.معلومات);
	وروستی_blob.دننه_واټن = 0;
	وروستی_blob.ژوروالی = 0;
}

ساكن فراغ دوره_packfile(فراغ)
{
	پای_ټکی_packfile();
	پیل_packfile();
}

ساكن صحيح store_object(
	تعداد object_type type,
	بنية strbuf *dat,
	بنية وروستی_object *وروستی,
	بنية object_id *oidout,
	uintmax_t mark)
{
	بنية odb_سرچینه_files *files = odb_سرچینه_files_downcast(the_repository->objects->سرچینه);
	فراغ *بهر, *delta;
	بنية object_ثبت *e;
	بینښه حرف hdr[96];
	بنية object_id oid;
	بینښه اوږد hdrlen, deltalen = 0;
	بنية git_hash_ctx c;
	git_zstream s;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	hdrlen = format_object_سرلیک((حرف *)hdr, حجم(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	إذا (oidout)
		oidcpy(oidout, &oid);

	e = ورننباسه_object(&oid);
	إذا (mark)
		ورننباسه_mark(&marks, mark, e);
	إذا (e->idx.دننه_واټن) {
		duplicate_شمېر_by_type[type]++;
		أرجع 1;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بل) {
		إذا (!packfile_list_پیدا_کړه_oid(packfile_store_get_packs(dir->packed), &oid))
			استمر;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.دننه_واټن = 1; /* just not zero! */
		duplicate_شمېر_by_type[type]++;
		أرجع 1;
	}

	إذا (وروستی && وروستی->معلومات.len && وروستی->معلومات.buf && وروستی->ژوروالی < max_ژوروالی
		&& dat->len > the_hash_algo->rawsz) {
		اندازه_t deltalen_st;

		delta_شمېر_attempts_by_type[type]++;
		delta = diff_delta(وروستی->معلومات.buf, وروستی->معلومات.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_اندازه_t_منزل_ulong(deltalen_st);
	} وإلا
		delta = عدم;

	git_deflate_init(&s, cfg->pack_compression_level);
	إذا (delta) {
		s.بل_دننه = delta;
		s.avail_دننه = deltalen;
	} وإلا {
		s.بل_دننه = (فراغ *)dat->buf;
		s.avail_دننه = dat->len;
	}
	s.avail_بهر = git_deflate_bound(&s, s.avail_دننه);
	s.بل_بهر = بهر = xmalloc(s.avail_بهر);
	طالما (git_deflate(&s, Z_پای) == Z_OK)
		; /* nothing */
	git_deflate_پای_ټکی(&s);

	/* Determine if we should auto-checkpoint. */
	إذا ((max_packsize
		&& (pack_اندازه + PACK_اندازه_پوله + s.ټول_بهر) > max_packsize)
		|| (pack_اندازه + PACK_اندازه_پوله + s.ټول_بهر) < pack_اندازه) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		دوره_packfile();

		/* We cannot carry a delta into the new pack. */
		إذا (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.بل_دننه = (فراغ *)dat->buf;
			s.avail_دننه = dat->len;
			s.avail_بهر = git_deflate_bound(&s, s.avail_دننه);
			s.بل_بهر = بهر = xrealloc(بهر, s.avail_بهر);
			طالما (git_deflate(&s, Z_پای) == Z_OK)
				; /* nothing */
			git_deflate_پای_ټکی(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.دننه_واټن = pack_اندازه;
	object_شمېر++;
	object_شمېر_by_type[type]++;

	دوراني_ازموینه_32_begin(pack_file);

	إذا (delta) {
		off_t ofs = e->idx.دننه_واټن - وروستی->دننه_واټن;
		بینښه pos = حجم(hdr) - 1;

		delta_شمېر_by_type[type]++;
		e->ژوروالی = وروستی->ژوروالی + 1;

		hdrlen = encode_دننه_pack_object_سرلیک(hdr, حجم(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_اندازه += hdrlen;

		hdr[pos] = ofs & 127;
		طالما (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, حجم(hdr) - pos);
		pack_اندازه += حجم(hdr) - pos;
	} وإلا {
		e->ژوروالی = 0;
		hdrlen = encode_دننه_pack_object_سرلیک(hdr, حجم(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_اندازه += hdrlen;
	}

	hashwrite(pack_file, بهر, s.ټول_بهر);
	pack_اندازه += s.ټول_بهر;

	e->idx.دوراني_ازموینه_32 = دوراني_ازموینه_32_پای_ټکی(pack_file);

	حرر(بهر);
	حرر(delta);
	إذا (وروستی) {
		إذا (وروستی->no_بدلون) {
			وروستی->معلومات = *dat;
		} وإلا {
			strbuf_بدلون(&وروستی->معلومات, dat);
		}
		وروستی->دننه_واټن = e->idx.دننه_واټن;
		وروستی->ژوروالی = e->ژوروالی;
	}
	أرجع 0;
}

ساكن فراغ truncate_pack(بنية hashfile_checkpoint *checkpoint)
{
	إذا (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_اندازه = checkpoint->دننه_واټن;
}

ساكن فراغ بهیر_blob(uintmax_t len, بنية object_id *oidout, uintmax_t mark)
{
	بنية odb_سرچینه_files *files = odb_سرچینه_files_downcast(the_repository->objects->سرچینه);
	اندازه_t دننه_sz = 64 * 1024, بهر_sz = 64 * 1024;
	بینښه حرف *دننه_buf = xmalloc(دننه_sz);
	بینښه حرف *بهر_buf = xmalloc(بهر_sz);
	بنية object_ثبت *e;
	بنية object_id oid;
	بینښه اوږد hdrlen;
	off_t دننه_واټن;
	بنية git_hash_ctx c;
	git_zstream s;
	بنية hashfile_checkpoint checkpoint;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);
	صحيح حالت = Z_OK;

	/* Determine if we should auto-checkpoint. */
	إذا ((max_packsize
		&& (pack_اندازه + PACK_اندازه_پوله + len) > max_packsize)
		|| (pack_اندازه + PACK_اندازه_پوله + len) < pack_اندازه)
		دوره_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	دننه_واټن = checkpoint.دننه_واټن;

	hdrlen = format_object_سرلیک((حرف *)بهر_buf, بهر_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, بهر_buf, hdrlen);

	دوراني_ازموینه_32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_دننه_pack_object_سرلیک(بهر_buf, بهر_sz, OBJ_BLOB, len);

	s.بل_بهر = بهر_buf + hdrlen;
	s.avail_بهر = بهر_sz - hdrlen;

	طالما (حالت != Z_بهیر_پای_ټکی) {
		إذا (0 < len && !s.avail_دننه) {
			اندازه_t cnt = دننه_sz < len ? دننه_sz : (اندازه_t)len;
			اندازه_t n = fread(دننه_buf, 1, cnt, stdin);
			إذا (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, دننه_buf, n);
			s.بل_دننه = دننه_buf;
			s.avail_دننه = n;
			len -= n;
		}

		حالت = git_deflate(&s, len ? 0 : Z_پای);

		إذا (!s.avail_بهر || حالت == Z_بهیر_پای_ټکی) {
			اندازه_t n = s.بل_بهر - بهر_buf;
			hashwrite(pack_file, بهر_buf, n);
			pack_اندازه += n;
			s.بل_بهر = بهر_buf;
			s.avail_بهر = بهر_sz;
		}

		بدل (حالت) {
		حالة Z_OK:
		حالة Z_BUF_ERROR:
		حالة Z_بهیر_پای_ټکی:
			استمر;
		افتراضي:
			die(_("unexpected deflate failure: %d"), حالت);
		}
	}
	git_deflate_پای_ټکی(&s);
	git_hash_final_oid(&oid, &c);

	إذا (oidout)
		oidcpy(oidout, &oid);

	e = ورننباسه_object(&oid);

	إذا (mark)
		ورننباسه_mark(&marks, mark, e);

	إذا (e->idx.دننه_واټن) {
		duplicate_شمېر_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		اذهب بهر;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بل) {
		إذا (!packfile_list_پیدا_کړه_oid(packfile_store_get_packs(dir->packed), &oid))
			استمر;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.دننه_واټن = 1; /* just not zero! */
		duplicate_شمېر_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		اذهب بهر;
	}

	e->ژوروالی = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.دننه_واټن = دننه_واټن;
	e->idx.دوراني_ازموینه_32 = دوراني_ازموینه_32_پای_ټکی(pack_file);
	object_شمېر++;
	object_شمېر_by_type[OBJ_BLOB]++;

بهر:
	حرر(دننه_buf);
	حرر(بهر_buf);
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
ساكن فراغ *gfi_unpack_ثبت(
	بنية object_ثبت *oe,
	بینښه اوږد *sizep)
{
	تعداد object_type type;
	اندازه_t اندازه_st = 0;
	فراغ *معلومات;
	بنية packed_git *p = all_packs[oe->pack_id];
	إذا (p == pack_معلومات && p->pack_اندازه < (pack_اندازه + the_hash_algo->rawsz)) {
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
	معلومات = unpack_ثبت(the_repository, p, oe->idx.دننه_واټن, &type, &اندازه_st);
	إذا (sizep)
		*sizep = cast_اندازه_t_منزل_ulong(اندازه_st);
	أرجع معلومات;
}

ساكن فراغ بارول_ونه(بنية ونه_ثبت *ریښه)
{
	بنية object_id *oid = &ریښه->versions[1].oid;
	بنية object_ثبت *myoe;
	بنية ونه_content *t;
	بینښه اوږد اندازه;
	حرف *buf;
	ثابت حرف *c;

	ریښه->ونه = t = new_ونه_content(8);
	إذا (is_null_oid(oid))
		أرجع;

	myoe = پیدا_کړه_object(oid);
	إذا (myoe && myoe->pack_id != MAX_PACK_ID) {
		إذا (myoe->type != OBJ_ونه)
			die(_("not a tree: %s"), oid_منزل_hex(oid));
		t->delta_ژوروالی = myoe->ژوروالی;
		buf = gfi_unpack_ثبت(myoe, &اندازه);
		إذا (!buf)
			die(_("can't load tree %s"), oid_منزل_hex(oid));
	} وإلا {
		تعداد object_type type;
		اندازه_t اندازه_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &اندازه_st);
		اندازه = cast_اندازه_t_منزل_ulong(اندازه_st);
		إذا (!buf || type != OBJ_ونه)
			die(_("can't load tree %s"), oid_منزل_hex(oid));
	}

	c = buf;
	طالما (c != (buf + اندازه)) {
		بنية ونه_ثبت *e = new_ونه_ثبت();

		إذا (t->ثبت_شمېر == t->ثبت_ظرفیت)
			ریښه->ونه = t = grow_ونه_content(t, t->ثبت_شمېر);
		t->entries[t->ثبت_شمېر++] = e;

		e->ونه = عدم;
		c = parse_ډېرپېښ(c, &e->versions[1].ډېرپېښ);
		إذا (!c)
			die(_("corrupt mode in %s"), oid_منزل_hex(oid));
		e->versions[0].ډېرپېښ = e->versions[1].ډېرپېښ;
		e->name = منزل_atom(c, مزي_اوږدوالی(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (بینښه حرف *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (بینښه حرف *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	حرر(buf);
}

ساكن صحيح tecmp0 (ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية ونه_ثبت *a = *((بنية ونه_ثبت**)_a);
	بنية ونه_ثبت *b = *((بنية ونه_ثبت**)_b);
	أرجع بنسټ_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].ډېرپېښ,
		b->name->str_dat, b->name->str_len, b->versions[0].ډېرپېښ);
}

ساكن صحيح tecmp1 (ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية ونه_ثبت *a = *((بنية ونه_ثبت**)_a);
	بنية ونه_ثبت *b = *((بنية ونه_ثبت**)_b);
	أرجع بنسټ_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].ډېرپېښ,
		b->name->str_dat, b->name->str_len, b->versions[1].ډېرپېښ);
}

ساكن فراغ mktree(بنية ونه_content *t, صحيح v, بنية strbuf *b)
{
	اندازه_t maxlen = 0;
	بینښه صحيح i;

	إذا (!v)
		QSORT(t->entries, t->ثبت_شمېر, tecmp0);
	وإلا
		QSORT(t->entries, t->ثبت_شمېر, tecmp1);

	لكل (i = 0; i < t->ثبت_شمېر; i++) {
		إذا (t->entries[i]->versions[v].ډېرپېښ)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	لكل (i = 0; i < t->ثبت_شمېر; i++) {
		بنية ونه_ثبت *e = t->entries[i];
		إذا (!e->versions[v].ډېرپېښ)
			استمر;
		strbuf_addf(b, "%o %s%c",
			(بینښه صحيح)(e->versions[v].ډېرپېښ & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_جمع_کړه(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

ساكن فراغ store_ونه(بنية ونه_ثبت *ریښه)
{
	بنية ونه_content *t;
	بینښه صحيح i, j, del;
	بنية وروستی_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	بنية object_ثبت *le = عدم;

	إذا (!is_null_oid(&ریښه->versions[1].oid))
		أرجع;

	إذا (!ریښه->ونه)
		بارول_ونه(ریښه);
	t = ریښه->ونه;

	لكل (i = 0; i < t->ثبت_شمېر; i++) {
		إذا (t->entries[i]->ونه)
			store_ونه(t->entries[i]);
	}

	إذا (!(ریښه->versions[0].ډېرپېښ & NO_DELTA))
		le = پیدا_کړه_object(&ریښه->versions[0].oid);
	إذا (S_ISDIR(ریښه->versions[0].ډېرپېښ) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_ونه);
		lo.معلومات = old_ونه;
		lo.دننه_واټن = le->idx.دننه_واټن;
		lo.ژوروالی = t->delta_ژوروالی;
	}

	mktree(t, 1, &new_ونه);
	store_object(OBJ_ونه, &new_ونه, &lo, &ریښه->versions[1].oid, 0);

	t->delta_ژوروالی = lo.ژوروالی;
	لكل (i = 0, j = 0, del = 0; i < t->ثبت_شمېر; i++) {
		بنية ونه_ثبت *e = t->entries[i];
		إذا (e->versions[1].ډېرپېښ) {
			e->versions[0].ډېرپېښ = e->versions[1].ډېرپېښ;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} وإلا {
			release_ونه_ثبت(e);
			del++;
		}
	}
	t->ثبت_شمېر -= del;
}

ساكن فراغ ونه_content_replace(
	بنية ونه_ثبت *ریښه,
	ثابت بنية object_id *oid,
	ثابت uint16_t ډېرپېښ,
	بنية ونه_content *newtree)
{
	إذا (!S_ISDIR(ډېرپېښ))
		die(_("root cannot be a non-directory"));
	oidclr(&ریښه->versions[0].oid, the_repository->hash_algo);
	oidcpy(&ریښه->versions[1].oid, oid);
	إذا (ریښه->ونه)
		release_ونه_content_recursive(ریښه->ونه);
	ریښه->ونه = newtree;
}

ساكن صحيح ونه_content_سېټ(
	بنية ونه_ثبت *ریښه,
	ثابت حرف *p,
	ثابت بنية object_id *oid,
	ثابت uint16_t ډېرپېښ,
	بنية ونه_content *subtree)
{
	بنية ونه_content *t;
	ثابت حرف *slash1;
	بینښه صحيح i, n;
	بنية ونه_ثبت *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	إذا (!n)
		die(_("empty path component found in input"));
	إذا (!*slash1 && !S_ISDIR(ډېرپېښ) && subtree)
		die(_("non-directories cannot have subtrees"));

	إذا (!ریښه->ونه)
		بارول_ونه(ریښه);
	t = ریښه->ونه;
	لكل (i = 0; i < t->ثبت_شمېر; i++) {
		e = t->entries[i];
		إذا (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			إذا (!*slash1) {
				إذا (!S_ISDIR(ډېرپېښ)
						&& e->versions[1].ډېرپېښ == ډېرپېښ
						&& oideq(&e->versions[1].oid, oid))
					أرجع 0;
				e->versions[1].ډېرپېښ = ډېرپېښ;
				oidcpy(&e->versions[1].oid, oid);
				إذا (e->ونه)
					release_ونه_content_recursive(e->ونه);
				e->ونه = subtree;

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
				إذا (S_ISDIR(e->versions[0].ډېرپېښ))
					e->versions[0].ډېرپېښ |= NO_DELTA;

				oidclr(&ریښه->versions[1].oid, the_repository->hash_algo);
				أرجع 1;
			}
			إذا (!S_ISDIR(e->versions[1].ډېرپېښ)) {
				e->ونه = new_ونه_content(8);
				e->versions[1].ډېرپېښ = S_IFDIR;
			}
			إذا (!e->ونه)
				بارول_ونه(e);
			إذا (ونه_content_سېټ(e, slash1 + 1, oid, ډېرپېښ, subtree)) {
				oidclr(&ریښه->versions[1].oid, the_repository->hash_algo);
				أرجع 1;
			}
			أرجع 0;
		}
	}

	إذا (t->ثبت_شمېر == t->ثبت_ظرفیت)
		ریښه->ونه = t = grow_ونه_content(t, t->ثبت_شمېر);
	e = new_ونه_ثبت();
	e->name = منزل_atom(p, n);
	e->versions[0].ډېرپېښ = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->ثبت_شمېر++] = e;
	إذا (*slash1) {
		e->ونه = new_ونه_content(8);
		e->versions[1].ډېرپېښ = S_IFDIR;
		ونه_content_سېټ(e, slash1 + 1, oid, ډېرپېښ, subtree);
	} وإلا {
		e->ونه = subtree;
		e->versions[1].ډېرپېښ = ډېرپېښ;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&ریښه->versions[1].oid, the_repository->hash_algo);
	أرجع 1;
}

ساكن صحيح ونه_content_remove(
	بنية ونه_ثبت *ریښه,
	ثابت حرف *p,
	بنية ونه_ثبت *backup_ونې_پاڼه,
	صحيح allow_ریښه)
{
	بنية ونه_content *t;
	ثابت حرف *slash1;
	بینښه صحيح i, n;
	بنية ونه_ثبت *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	إذا (!ریښه->ونه)
		بارول_ونه(ریښه);

	إذا (!*p && allow_ریښه) {
		e = ریښه;
		اذهب del_ثبت;
	}

	t = ریښه->ونه;
	لكل (i = 0; i < t->ثبت_شمېر; i++) {
		e = t->entries[i];
		إذا (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			إذا (*slash1 && !S_ISDIR(e->versions[1].ډېرپېښ))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				أرجع 1;
			إذا (!*slash1 || !S_ISDIR(e->versions[1].ډېرپېښ))
				اذهب del_ثبت;
			إذا (!e->ونه)
				بارول_ونه(e);
			إذا (ونه_content_remove(e, slash1 + 1, backup_ونې_پاڼه, 0)) {
				لكل (n = 0; n < e->ونه->ثبت_شمېر; n++) {
					إذا (e->ونه->entries[n]->versions[1].ډېرپېښ) {
						oidclr(&ریښه->versions[1].oid,
						       the_repository->hash_algo);
						أرجع 1;
					}
				}
				backup_ونې_پاڼه = عدم;
				اذهب del_ثبت;
			}
			أرجع 0;
		}
	}
	أرجع 0;

del_ثبت:
	إذا (backup_ونې_پاڼه)
		حافظه_نقل(backup_ونې_پاڼه, e, حجم(*backup_ونې_پاڼه));
	وإلا إذا (e->ونه)
		release_ونه_content_recursive(e->ونه);
	e->ونه = عدم;
	e->versions[1].ډېرپېښ = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&ریښه->versions[1].oid, the_repository->hash_algo);
	أرجع 1;
}

ساكن صحيح ونه_content_get(
	بنية ونه_ثبت *ریښه,
	ثابت حرف *p,
	بنية ونه_ثبت *ونې_پاڼه,
	صحيح allow_ریښه)
{
	بنية ونه_content *t;
	ثابت حرف *slash1;
	بینښه صحيح i, n;
	بنية ونه_ثبت *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	إذا (!n && !allow_ریښه)
		die(_("empty path component found in input"));

	إذا (!ریښه->ونه)
		بارول_ونه(ریښه);

	إذا (!n) {
		e = ریښه;
		اذهب وموندل_ثبت;
	}

	t = ریښه->ونه;
	لكل (i = 0; i < t->ثبت_شمېر; i++) {
		e = t->entries[i];
		إذا (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			إذا (!*slash1)
				اذهب وموندل_ثبت;
			إذا (!S_ISDIR(e->versions[1].ډېرپېښ))
				أرجع 0;
			إذا (!e->ونه)
				بارول_ونه(e);
			أرجع ونه_content_get(e, slash1 + 1, ونې_پاڼه, 0);
		}
	}
	أرجع 0;

وموندل_ثبت:
	حافظه_نقل(ونې_پاڼه, e, حجم(*ونې_پاڼه));
	إذا (e->ونه && is_null_oid(&e->versions[1].oid))
		ونې_پاڼه->ونه = dup_ونه_content(e->ونه);
	وإلا
		ونې_پاڼه->ونه = عدم;
	أرجع 1;
}

ساكن صحيح update_branch(بنية branch *b)
{
	ساكن ثابت حرف *پیغام = "fast-import";
	بنية ref_transaction *transaction;
	بنية object_id old_oid;
	بنية strbuf err = STRBUF_INIT;
	ساكن ثابت حرف *replace_مختاړی = "refs/replace/";

	إذا (starts_with(b->name, replace_مختاړی) &&
	    !strcmp(b->name + مزي_اوږدوالی(replace_مختاړی),
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

		ret = repo_دننه_یوځای_bases(the_repository, old_cmit, new_cmit);
		إذا (ret < 0)
			وتل(128);
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
	بینښه صحيح i;
	بنية branch *b;

	لكل (i = 0; i < branch_جدول_sz; i++) {
		لكل (b = branch_جدول[i]; b; b = b->جدول_بل_branch)
			failure |= update_branch(b);
	}
}

ساكن فراغ dump_نښانونه(فراغ)
{
	ساكن ثابت حرف *پیغام = "fast-import";
	بنية نښان *t;
	بنية strbuf ref_name = STRBUF_INIT;
	بنية strbuf err = STRBUF_INIT;
	بنية ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	إذا (!transaction) {
		failure |= error("%s", err.buf);
		اذهب cleanup;
	}
	لكل (t = لومړی_نښان; t; t = t->بل_نښان) {
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
	بنية قلف_file mark_قلف = قلف_INIT;
	FILE *f;

	إذا (!export_marks_file || (import_marks_file && !import_marks_file_بشپړ))
		أرجع;

	إذا (خوندي_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		أرجع;
	}

	إذا (hold_قلف_file_for_update(&mark_قلف, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		أرجع;
	}

	f = fdopen_قلف_file(&mark_قلف, "w");
	إذا (!f) {
		صحيح saved_errno = errno;
		rollback_قلف_file(&mark_قلف);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		أرجع;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	إذا (commit_قلف_file(&mark_قلف)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		أرجع;
	}
}

ساكن فراغ ورننباسه_object_ثبت(بنية mark_سېټ **s, بنية object_id *oid, uintmax_t mark)
{
	بنية object_ثبت *e;
	e = پیدا_کړه_object(oid);
	إذا (!e) {
		تعداد object_type type = odb_read_object_info(the_repository->objects,
							     oid, عدم);
		إذا (type < 0)
			die(_("object not found: %s"), oid_منزل_hex(oid));
		e = ورننباسه_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.دننه_واټن = 1; /* just not zero! */
	}
	ورننباسه_mark(s, mark, e);
}

ساكن فراغ ورننباسه_oid_ثبت(بنية mark_سېټ **s, بنية object_id *oid, uintmax_t mark)
{
	ورننباسه_mark(s, mark, xmemdupz(oid, حجم(*oid)));
}

ساكن فراغ read_mark_file(بنية mark_سېټ **s, FILE *f, mark_سېټ_inserter_t inserter)
{
	حرف کرښه[512];
	طالما (fgets(کرښه, حجم(کرښه), f)) {
		uintmax_t mark;
		حرف *پای_ټکی;
		بنية object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, حجم(oid.hash));

		پای_ټکی = strchr(کرښه, '\n');
		إذا (کرښه[0] != ':' || !پای_ټکی)
			die(_("corrupt mark line: %s"), کرښه);
		*پای_ټکی = 0;
		mark = strtoumax(کرښه + 1, &پای_ټکی, 10);
		إذا (!mark || پای_ټکی == کرښه + 1
			|| *پای_ټکی != ' '
			|| get_oid_hex_any(پای_ټکی + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), کرښه);
		inserter(s, &oid, mark);
	}
}

ساكن فراغ read_marks(فراغ)
{
	FILE *f = fopen(import_marks_file, "r");
	إذا (f)
		;
	وإلا إذا (import_marks_file_ignore_missing && errno == ENOENT)
		اذهب بشپړ; /* Marks file does not exist */
	وإلا
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, ورننباسه_object_ثبت);
	fclose(f);
بشپړ:
	import_marks_file_بشپړ = 1;
}


ساكن صحيح read_بل_command(بنية fast_import_state *state)
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

			إذا (!state->کتل_شوی_معلومات_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			إذا (rc)
				rc_free = rc->بل;
			وإلا {
				rc = cmd_hist.بل;
				cmd_hist.بل = rc->بل;
				cmd_hist.بل->prev = &cmd_hist;
				حرر(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_لکۍ;
			rc->بل = cmd_hist.prev;
			rc->prev->بل = rc;
			cmd_لکۍ = rc;
		}
		إذا (command_buf.buf[0] == '#')
			استمر;
		أرجع 0;
	}
}

ساكن فراغ skip_optional_lf(فراغ)
{
	صحيح غړی_char = fgetc(stdin);
	إذا (غړی_char != '\n' && غړی_char != EOF)
		ungetc(غړی_char, stdin);
}

ساكن فراغ parse_mark(بنية fast_import_state *state)
{
	ثابت حرف *v;
	إذا (skip_مختاړی(command_buf.buf, "mark :", &v)) {
		بل_mark = strtoumax(v, عدم, 10);
		read_بل_command(state);
	}
	وإلا
		بل_mark = 0;
}

ساكن فراغ parse_original_identifier(بنية fast_import_state *state)
{
	ثابت حرف *v;
	إذا (skip_مختاړی(command_buf.buf, "original-oid ", &v))
		read_بل_command(state);
}

ساكن صحيح parse_معلومات(بنية strbuf *sb, uintmax_t حد, uintmax_t *len_res)
{
	ثابت حرف *معلومات;
	strbuf_reset(sb);

	إذا (!skip_مختاړی(command_buf.buf, "data ", &معلومات))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	إذا (skip_مختاړی(معلومات, "<<", &معلومات)) {
		حرف *غړی = xstrdup(معلومات);
		اندازه_t غړی_len = command_buf.len - (معلومات - command_buf.buf);

		لكل (;;) {
			إذا (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), غړی);
			إذا (غړی_len == command_buf.len
				&& !strcmp(غړی, command_buf.buf))
				اكسر;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		حرر(غړی);
	}
	وإلا {
		uintmax_t len = strtoumax(معلومات, عدم, 10);
		اندازه_t n = 0, اوږدتیا = (اندازه_t)len;

		إذا (حد && حد < len) {
			*len_res = len;
			أرجع 0;
		}
		إذا (اوږدتیا < len)
			die(_("data is too large to use in this context"));

		طالما (n < اوږدتیا) {
			اندازه_t s = strbuf_fread(sb, اوږدتیا - n, stdin);
			إذا (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(بینښه اوږد)(اوږدتیا - n));
			n += s;
		}
	}

	skip_optional_lf();
	أرجع 1;
}

ساكن صحيح validate_raw_date(ثابت حرف *src, بنية strbuf *پایله, صحيح strict)
{
	ثابت حرف *orig_src = src;
	حرف *endp;
	بینښه اوږد عدد;

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

	strbuf_addstr(پایله, orig_src);
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
	strbuf_جمع_کړه(&ident, buf, name_len);

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
	بنية وروستی_object *وروستی,
	بنية object_id *oidout,
	uintmax_t mark)
{
	ساكن بنية strbuf buf = STRBUF_INIT;
	uintmax_t len;

	إذا (parse_معلومات(&buf, repo_settings_get_big_file_پوله(the_repository), &len))
		store_object(OBJ_BLOB, &buf, وروستی, oidout, mark);
	وإلا {
		إذا (وروستی) {
			strbuf_release(&وروستی->معلومات);
			وروستی->دننه_واټن = 0;
			وروستی->ژوروالی = 0;
		}
		بهیر_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

ساكن فراغ parse_new_blob(بنية fast_import_state *state)
{
	read_بل_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&وروستی_blob, عدم, بل_mark);
}

ساكن فراغ unload_one_branch(فراغ)
{
	طالما (cur_فعال_branches
		&& cur_فعال_branches >= max_فعال_branches) {
		uintmax_t min_commit = ULONG_MAX;
		بنية branch *e, *l = عدم, *p = عدم;

		لكل (e = فعال_branches; e; e = e->فعال_بل_branch) {
			إذا (e->وروستی_commit < min_commit) {
				p = l;
				min_commit = e->وروستی_commit;
			}
			l = e;
		}

		إذا (p) {
			e = p->فعال_بل_branch;
			p->فعال_بل_branch = e->فعال_بل_branch;
		} وإلا {
			e = فعال_branches;
			فعال_branches = e->فعال_بل_branch;
		}
		e->فعال = 0;
		e->فعال_بل_branch = عدم;
		إذا (e->branch_ونه.ونه) {
			release_ونه_content_recursive(e->branch_ونه.ونه);
			e->branch_ونه.ونه = عدم;
		}
		cur_فعال_branches--;
	}
}

ساكن فراغ بارول_branch(بنية branch *b)
{
	بارول_ونه(&b->branch_ونه);
	إذا (!b->فعال) {
		b->فعال = 1;
		b->فعال_بل_branch = فعال_branches;
		فعال_branches = b;
		cur_فعال_branches++;
		branch_بارول_شمېر++;
	}
}

ساكن بینښه حرف convert_عدد_notes_منزل_fanout(uintmax_t عدد_notes)
{
	بینښه حرف fanout = 0;
	طالما ((عدد_notes >>= 8))
		fanout++;
	أرجع fanout;
}

ساكن فراغ construct_لار_with_fanout(ثابت حرف *hex_sha1,
		بینښه حرف fanout, حرف *لار)
{
	بینښه صحيح i = 0, j = 0;
	إذا (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	طالما (fanout) {
		لار[i++] = hex_sha1[j++];
		لار[i++] = hex_sha1[j++];
		لار[i++] = '/';
		fanout--;
	}
	حافظه_نقل(لار + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	لار[i + the_hash_algo->hexsz - j] = '\0';
}

ساكن uintmax_t do_change_note_fanout(
		بنية ونه_ثبت *orig_ریښه, بنية ونه_ثبت *ریښه,
		حرف *hex_oid, بینښه صحيح hex_oid_len,
		حرف *fullpath, بینښه صحيح fullpath_len,
		بینښه حرف fanout)
{
	بنية ونه_content *t;
	بنية ونه_ثبت *e, ونې_پاڼه;
	بینښه صحيح i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t عدد_notes = 0;
	بنية object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	حرف realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ثابت بینښه hexsz = the_hash_algo->hexsz;

	إذا (!ریښه->ونه)
		بارول_ونه(ریښه);
	t = ریښه->ونه;

	لكل (i = 0; t && i < t->ثبت_شمېر; i++) {
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
		إذا (!e->versions[1].ډېرپېښ ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			استمر;

		/* This _may_ be a note entry, or a subdir containing notes */
		حافظه_نقل(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		إذا (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		حافظه_نقل(fullpath + tmp_fullpath_len, e->name->str_dat,
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
			construct_لار_with_fanout(hex_oid, fanout, realpath);
			إذا (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				عدد_notes++;
				استمر;
			}

			/* Rename fullpath to realpath */
			إذا (!ونه_content_remove(orig_ریښه, fullpath, &ونې_پاڼه, 0))
				die(_("failed to remove path %s"), fullpath);
			ونه_content_سېټ(orig_ریښه, realpath,
				&ونې_پاڼه.versions[1].oid,
				ونې_پاڼه.versions[1].ډېرپېښ,
				ونې_پاڼه.ونه);
		} وإلا إذا (S_ISDIR(e->versions[1].ډېرپېښ)) {
			/* This is a subdir that may contain note entries */
			عدد_notes += do_change_note_fanout(orig_ریښه, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = ریښه->ونه;
	}
	أرجع عدد_notes;
}

ساكن uintmax_t change_note_fanout(بنية ونه_ثبت *ریښه,
		بینښه حرف fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	حرف hex_oid[GIT_MAX_HEXSZ], لار[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	أرجع do_change_note_fanout(ریښه, ریښه, hex_oid, 0, لار, 0, fanout);
}

ساكن صحيح parse_mapped_oid_hex(ثابت حرف *hex, بنية object_id *oid, ثابت حرف **پای_ټکی)
{
	صحيح algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, حجم(oid->hash));

	algo = parse_oid_hex_any(hex, oid, پای_ټکی);
	إذا (algo == GIT_HASH_UNKNOWN)
		أرجع -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	إذا (it == kh_پای_ټکی(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		إذا (hash_algos[algo].format_id == the_hash_algo->format_id)
			أرجع 0;
		أرجع -1;
	}
	oidcpy(oid, kh_ارزښت(sub_oid_map, it));
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
	حرف *پای_ټکی;
	uintmax_t mark;

	mark = parse_mark_ref(p, &پای_ټکی);
	إذا (*پای_ټکی != '\0')
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
	حرف *پای_ټکی;

	mark = parse_mark_ref(*p, &پای_ټکی);
	إذا (*پای_ټکی++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = پای_ټکی;
	أرجع mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
ساكن فراغ parse_لار(بنية strbuf *sb, ثابت حرف *p, ثابت حرف **endp,
		صحيح is_وروستی_ډګر, ثابت حرف *ډګر)
{
	إذا (*p == '"') {
		إذا (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), ډګر, command_buf.buf);
		إذا (مزي_اوږدوالی(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), ډګر, command_buf.buf);
	} وإلا {
		/*
		 * Unless we are parsing the last field of a line,
		 * SP is the end of this field.
		 */
		*endp = is_وروستی_ډګر
			? p + مزي_اوږدوالی(p)
			: strchrnul(p, ' ');
		strbuf_جمع_کړه(sb, p, *endp - p);
	}
}

/*
 * Parse the path string into the strbuf, and complain if this is not the end of
 * the string. Unquoted strings may contain spaces.
 */
ساكن فراغ parse_لار_eol(بنية strbuf *sb, ثابت حرف *p, ثابت حرف *ډګر)
{
	ثابت حرف *پای_ټکی;

	parse_لار(sb, p, &پای_ټکی, 1, ډګر);
	إذا (*پای_ټکی)
		die(_("garbage after %s: %s"), ډګر, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
ساكن فراغ parse_لار_space(بنية strbuf *sb, ثابت حرف *p,
		ثابت حرف **endp, ثابت حرف *ډګر)
{
	parse_لار(sb, p, endp, 0, ډګر);
	إذا (**endp != ' ')
		die(_("missing space after %s: %s"), ډګر, command_buf.buf);
	(*endp)++;
}

ساكن فراغ file_change_m(بنية fast_import_state *state,
			  ثابت حرف *p,
			  بنية branch *b)
{
	ساكن بنية strbuf لار = STRBUF_INIT;
	بنية object_ثبت *oe;
	بنية object_id oid;
	uint16_t ډېرپېښ, inline_معلومات = 0;

	p = parse_ډېرپېښ(p, &ډېرپېښ);
	إذا (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	بدل (ډېرپېښ) {
	حالة 0644:
	حالة 0755:
		ډېرپېښ |= S_IFREG;
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
		oe = پیدا_کړه_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} وإلا إذا (skip_مختاړی(p, "inline ", &p)) {
		inline_معلومات = 1;
		oe = عدم; /* not used with inline_data, but makes gcc happy */
	} وإلا {
		إذا (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = پیدا_کړه_object(&oid);
		إذا (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&لار);
	parse_لار_eol(&لار, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	إذا (S_ISDIR(ډېرپېښ) &&
	    is_تشه_ونه_oid(&oid, the_repository->hash_algo) &&
	    *لار.buf) {
		ونه_content_remove(&b->branch_ونه, لار.buf, عدم, 0);
		أرجع;
	}

	إذا (S_ISGITLINK(ډېرپېښ)) {
		إذا (inline_معلومات)
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
	} وإلا إذا (inline_معلومات) {
		إذا (S_ISDIR(ډېرپېښ))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		طالما (read_بل_command(state) != EOF) {
			ثابت حرف *v;
			إذا (skip_مختاړی(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			وإلا {
				parse_and_store_blob(&وروستی_blob, &oid, 0);
				اكسر;
			}
		}
	} وإلا {
		تعداد object_type expected = S_ISDIR(ډېرپېښ) ?
						OBJ_ونه: OBJ_BLOB;
		تعداد object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, عدم);
		إذا (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(ډېرپېښ) ?  _("tree") : _("blob"),
			    command_buf.buf);
		إذا (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	إذا (!*لار.buf) {
		ونه_content_replace(&b->branch_ونه, &oid, ډېرپېښ, عدم);
		أرجع;
	}

	إذا (!verify_لار(لار.buf, ډېرپېښ))
		die(_("invalid path '%s'"), لار.buf);
	ونه_content_سېټ(&b->branch_ونه, لار.buf, &oid, ډېرپېښ, عدم);
}

ساكن فراغ file_change_d(ثابت حرف *p, بنية branch *b)
{
	ساكن بنية strbuf لار = STRBUF_INIT;

	strbuf_reset(&لار);
	parse_لار_eol(&لار, p, "path");
	ونه_content_remove(&b->branch_ونه, لار.buf, عدم, 1);
}

ساكن فراغ file_change_cr(ثابت حرف *p, بنية branch *b, صحيح rename)
{
	ساكن بنية strbuf سرچینه = STRBUF_INIT;
	ساكن بنية strbuf dest = STRBUF_INIT;
	بنية ونه_ثبت ونې_پاڼه;

	strbuf_reset(&سرچینه);
	parse_لار_space(&سرچینه, p, &p, "source");
	strbuf_reset(&dest);
	parse_لار_eol(&dest, p, "dest");

	memset(&ونې_پاڼه, 0, حجم(ونې_پاڼه));
	إذا (rename)
		ونه_content_remove(&b->branch_ونه, سرچینه.buf, &ونې_پاڼه, 1);
	وإلا
		ونه_content_get(&b->branch_ونه, سرچینه.buf, &ونې_پاڼه, 1);
	إذا (!ونې_پاڼه.versions[1].ډېرپېښ)
		die(_("path %s not in branch"), سرچینه.buf);
	إذا (!*dest.buf) {	/* C "path/to/subdir" "" */
		ونه_content_replace(&b->branch_ونه,
			&ونې_پاڼه.versions[1].oid,
			ونې_پاڼه.versions[1].ډېرپېښ,
			ونې_پاڼه.ونه);
		أرجع;
	}
	إذا (!verify_لار(dest.buf, ونې_پاڼه.versions[1].ډېرپېښ))
		die(_("invalid path '%s'"), dest.buf);
	ونه_content_سېټ(&b->branch_ونه, dest.buf,
		&ونې_پاڼه.versions[1].oid,
		ونې_پاڼه.versions[1].ډېرپېښ,
		ونې_پاڼه.ونه);
}

ساكن فراغ note_change_n(بنية fast_import_state *state,
			  ثابت حرف *p,
			  بنية branch *b,
			  بینښه حرف *old_fanout)
{
	بنية object_ثبت *oe;
	بنية branch *s;
	بنية object_id oid, commit_oid;
	حرف لار[GIT_MAX_RAWSZ * 3];
	uint16_t inline_معلومات = 0;
	بینښه حرف new_fanout;

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
		b->عدد_notes = change_note_fanout(&b->branch_ونه, 0xff);
		*old_fanout = convert_عدد_notes_منزل_fanout(b->عدد_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	إذا (*p == ':') {
		oe = پیدا_کړه_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} وإلا إذا (skip_مختاړی(p, "inline ", &p)) {
		inline_معلومات = 1;
		oe = عدم; /* not used with inline_data, but makes gcc happy */
	} وإلا {
		إذا (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = پیدا_کړه_object(&oid);
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
		بنية object_ثبت *commit_oe = پیدا_کړه_mark(marks, commit_mark);
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

	إذا (inline_معلومات) {
		read_بل_command(state);
		parse_and_store_blob(&وروستی_blob, &oid, 0);
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

	construct_لار_with_fanout(oid_منزل_hex(&commit_oid), *old_fanout, لار);
	إذا (ونه_content_remove(&b->branch_ونه, لار, عدم, 0))
		b->عدد_notes--;

	إذا (is_null_oid(&oid))
		أرجع; /* nothing to insert */

	b->عدد_notes++;
	new_fanout = convert_عدد_notes_منزل_fanout(b->عدد_notes);
	construct_لار_with_fanout(oid_منزل_hex(&commit_oid), new_fanout, لار);
	ونه_content_سېټ(&b->branch_ونه, لار, &oid, S_IFREG | 0644, عدم);
}

ساكن فراغ file_change_deleteall(بنية branch *b)
{
	release_ونه_content_recursive(b->branch_ونه.ونه);
	oidclr(&b->branch_ونه.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_ونه.versions[1].oid, the_repository->hash_algo);
	بارول_ونه(&b->branch_ونه);
	b->عدد_notes = 0;
}

ساكن فراغ parse_from_commit(بنية branch *b, حرف *buf, بینښه اوږد اندازه)
{
	إذا (!buf || اندازه < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_منزل_hex(&b->oid));
	إذا (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_ونه.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_منزل_hex(&b->oid));
	oidcpy(&b->branch_ونه.versions[0].oid,
	       &b->branch_ونه.versions[1].oid);
}

ساكن فراغ parse_from_existing(بنية branch *b)
{
	إذا (is_null_oid(&b->oid)) {
		oidclr(&b->branch_ونه.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_ونه.versions[1].oid, the_repository->hash_algo);
	} وإلا {
		بینښه اوږد اندازه;
		اندازه_t اندازه_st = 0;
		حرف *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &اندازه_st, &b->oid);
		اندازه = cast_اندازه_t_منزل_ulong(اندازه_st);
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

	oidcpy(&oid, &b->branch_ونه.versions[1].oid);

	s = lookup_branch(objectish);
	إذا (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	وإلا إذا (s) {
		بنية object_id *t = &s->branch_ونه.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_ونه.versions[0].oid, t);
		oidcpy(&b->branch_ونه.versions[1].oid, t);
	} وإلا إذا (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		بنية object_ثبت *oe = پیدا_کړه_mark(marks, idnum);
		إذا (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		إذا (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			إذا (oe->pack_id != MAX_PACK_ID) {
				بینښه اوږد اندازه;
				حرف *buf = gfi_unpack_ثبت(oe, &اندازه);
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

	إذا (b->branch_ونه.ونه && !oideq(&oid, &b->branch_ونه.versions[1].oid)) {
		release_ونه_content_recursive(b->branch_ونه.ونه);
		b->branch_ونه.ونه = عدم;
	}

	read_بل_command(state);
	أرجع 1;
}

ساكن صحيح parse_from(بنية fast_import_state *state, بنية branch *b)
{
	ثابت حرف *from;

	إذا (!skip_مختاړی(command_buf.buf, "from ", &from))
		أرجع 0;

	أرجع parse_objectish(state, b, from);
}

ساكن صحيح parse_objectish_with_مختاړی(بنية fast_import_state *state,
				       بنية branch *b,
				       ثابت حرف *مختاړی)
{
	ثابت حرف *بنسټ;

	إذا (!skip_مختاړی(command_buf.buf, مختاړی, &بنسټ))
		أرجع 0;

	أرجع parse_objectish(state, b, بنسټ);
}

ساكن بنية hash_list *parse_یوځای(بنية fast_import_state *state,
				     بینښه صحيح *شمېر)
{
	بنية hash_list *list = عدم, **لکۍ = &list, *n;
	ثابت حرف *from;
	بنية branch *s;

	*شمېر = 0;
	طالما (skip_مختاړی(command_buf.buf, "merge ", &from)) {
		n = xmalloc(حجم(*n));
		s = lookup_branch(from);
		إذا (s)
			oidcpy(&n->oid, &s->oid);
		وإلا إذا (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			بنية object_ثبت *oe = پیدا_کړه_mark(marks, idnum);
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

		n->بل = عدم;
		*لکۍ = n;
		لکۍ = &n->بل;

		(*شمېر)++;
		read_بل_command(state);
	}
	أرجع list;
}

بنية signature_معلومات {
	حرف *hash_algo;      /* "sha1" or "sha256" */
	حرف *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	بنية strbuf معلومات;   /* The actual signature data */
};

ساكن فراغ parse_one_signature(بنية fast_import_state *state,
				بنية signature_معلومات *sig,
				ثابت حرف *v)
{
	حرف *دلیلونه = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	حرف *space = strchr(دلیلونه, ' ');

	إذا (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), دلیلونه);
	*space = '\0';

	sig->hash_algo = دلیلونه;
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
	read_بل_command(state);
	parse_معلومات(&sig->معلومات, 0, عدم);
}

ساكن فراغ discard_one_signature(بنية fast_import_state *state)
{
	بنية strbuf معلومات = STRBUF_INIT;

	read_بل_command(state);
	parse_معلومات(&معلومات, 0, عدم);
	strbuf_release(&معلومات);
}

ساكن فراغ جمع_کړه_gpgsig_منزل_commit(بنية strbuf *commit_معلومات,
				 ثابت حرف *سرلیک,
				 بنية signature_معلومات *sig)
{
	بنية string_list siglines = STRING_LIST_INIT_NODUP;

	إذا (!sig || !sig->hash_algo)
		أرجع;

	strbuf_addstr(commit_معلومات, سرلیک);
	string_list_وېشل_دننه_ځای_پرځای_کړه(&siglines, sig->معلومات.buf, "\n", -1);
	strbuf_جمع_کړه_separated_string_list(commit_معلومات, "\n ", &siglines);
	strbuf_addch(commit_معلومات, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->معلومات);
	حرر(sig->hash_algo);
}

ساكن فراغ store_signature(بنية signature_معلومات *stored_sig,
			    بنية signature_معلومات *new_sig,
			    ثابت حرف *hash_type)
{
	إذا (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->معلومات);
		حرر(new_sig->hash_algo);
	} وإلا {
		*stored_sig = *new_sig;
	}
}

ساكن فراغ import_one_signature(بنية fast_import_state *state,
				 بنية signature_معلومات *sig_sha1,
				 بنية signature_معلومات *sig_sha256,
				 ثابت حرف *v)
{
	بنية signature_معلومات sig = { عدم, عدم, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	إذا (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	وإلا إذا (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	وإلا
		die(_("parse_one_signature() returned unknown hash algo"));
}

ساكن فراغ finalize_commit_بفر(بنية strbuf *new_معلومات,
				   بنية signature_معلومات *sig_sha1,
				   بنية signature_معلومات *sig_sha256,
				   بنية strbuf *پیغام)
{
	جمع_کړه_gpgsig_منزل_commit(new_معلومات, "gpgsig ", sig_sha1);
	جمع_کړه_gpgsig_منزل_commit(new_معلومات, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_معلومات, '\n');
	strbuf_addbuf(new_معلومات, پیغام);
}

ساكن فراغ warn_invalid_signature(بنية signature_check *check,
				   ثابت حرف *پیغام, تعداد sign_ډېرپېښ ډېرپېښ)
{
	ثابت حرف *signer = check->signer ? check->signer : _("unknown");
	ثابت حرف *subject;
	صحيح subject_len = پیدا_کړه_commit_subject(پیغام, &subject);

	بدل (ډېرپېښ) {
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

ساكن فراغ handle_signature_if_invalid(بنية strbuf *new_معلومات,
					بنية signature_معلومات *sig_sha1,
					بنية signature_معلومات *sig_sha256,
					بنية strbuf *پیغام,
					تعداد sign_ډېرپېښ ډېرپېښ)
{
	بنية strbuf tmp_buf = STRBUF_INIT;
	بنية signature_check signature_check = { 0 };
	صحيح ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_معلومات);
	finalize_commit_بفر(&tmp_buf, sig_sha1, sig_sha256, پیغام);
	ret = verify_commit_بفر(tmp_buf.buf, tmp_buf.len, &signature_check);

	إذا (ret) {
		إذا (ډېرپېښ == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, پیغام->buf, ډېرپېښ);

		إذا (ډېرپېښ == SIGN_SIGN_IF_INVALID) {
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
					SIGN_بفر_USE_DEFAULT_کلی))
				die(_("failed to sign commit object"));
			جمع_کړه_سرلیک_signature(new_معلومات, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_بفر(new_معلومات, عدم, عدم, پیغام);
	} وإلا {
		strbuf_بدلون(new_معلومات, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

ساكن فراغ parse_new_commit(بنية fast_import_state *state, ثابت حرف *arg)
{
	ساكن بنية strbuf پیغام = STRBUF_INIT;
	بنية signature_معلومات sig_sha1 = { عدم, عدم, STRBUF_INIT };
	بنية signature_معلومات sig_sha256 = { عدم, عدم, STRBUF_INIT };
	بنية branch *b;
	حرف *author = عدم;
	حرف *committer = عدم;
	حرف *encoding = عدم;
	بنية hash_list *یوځای_list = عدم;
	بینښه صحيح یوځای_شمېر;
	بینښه حرف prev_fanout, new_fanout;
	ثابت حرف *v;

	b = lookup_branch(arg);
	إذا (!b)
		b = new_branch(arg);

	read_بل_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	إذا (skip_مختاړی(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_بل_command(state);
	}
	إذا (skip_مختاړی(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_بل_command(state);
	}
	إذا (!committer)
		die(_("expected committer but didn't get one"));

	طالما (skip_مختاړی(command_buf.buf, "gpgsig ", &v)) {
		بدل (signed_commit_ډېرپېښ) {

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
			BUG("invalid signed_commit_mode value %d", signed_commit_ډېرپېښ);
		}
		read_بل_command(state);
	}

	إذا (skip_مختاړی(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_بل_command(state);
	}
	parse_معلومات(&پیغام, 0, عدم);
	read_بل_command(state);
	parse_from(state, b);
	یوځای_list = parse_یوځای(state, &یوځای_شمېر);

	/* ensure the branch is active/loaded */
	إذا (!b->branch_ونه.ونه || !max_فعال_branches) {
		unload_one_branch();
		بارول_branch(b);
	}

	prev_fanout = convert_عدد_notes_منزل_fanout(b->عدد_notes);

	/* file_change* */
	طالما (command_buf.len > 0) {
		إذا (skip_مختاړی(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		وإلا إذا (skip_مختاړی(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		وإلا إذا (skip_مختاړی(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		وإلا إذا (skip_مختاړی(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		وإلا إذا (skip_مختاړی(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		وإلا إذا (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		وإلا إذا (skip_مختاړی(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		وإلا إذا (skip_مختاړی(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		وإلا {
			unread_command_buf = 1;
			اكسر;
		}
		إذا (read_بل_command(state) == EOF)
			اكسر;
	}

	new_fanout = convert_عدد_notes_منزل_fanout(b->عدد_notes);
	إذا (new_fanout != prev_fanout)
		b->عدد_notes = change_note_fanout(&b->branch_ونه, new_fanout);

	/* build the tree and the commit */
	store_ونه(&b->branch_ونه);
	oidcpy(&b->branch_ونه.versions[0].oid,
	       &b->branch_ونه.versions[1].oid);

	strbuf_reset(&new_معلومات);
	strbuf_addf(&new_معلومات, "tree %s\n",
		oid_منزل_hex(&b->branch_ونه.versions[1].oid));
	إذا (!is_null_oid(&b->oid))
		strbuf_addf(&new_معلومات, "parent %s\n",
			    oid_منزل_hex(&b->oid));
	طالما (یوځای_list) {
		بنية hash_list *بل = یوځای_list->بل;
		strbuf_addf(&new_معلومات, "parent %s\n",
			    oid_منزل_hex(&یوځای_list->oid));
		حرر(یوځای_list);
		یوځای_list = بل;
	}
	strbuf_addf(&new_معلومات,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	إذا (encoding)
		strbuf_addf(&new_معلومات,
			"encoding %s\n",
			encoding);

	إذا ((signed_commit_ډېرپېښ == SIGN_STRIP_IF_INVALID ||
	     signed_commit_ډېرپېښ == SIGN_SIGN_IF_INVALID ||
	     signed_commit_ډېرپېښ == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_معلومات, &sig_sha1, &sig_sha256,
					    &پیغام, signed_commit_ډېرپېښ);
	وإلا
		finalize_commit_بفر(&new_معلومات, &sig_sha1, &sig_sha256, &پیغام);

	حرر(author);
	حرر(committer);
	حرر(encoding);

	إذا (!store_object(OBJ_COMMIT, &new_معلومات, عدم, &b->oid, بل_mark))
		b->pack_id = pack_id;
	b->وروستی_commit = object_شمېر_by_type[OBJ_COMMIT];
}

ساكن فراغ handle_نښان_signature_if_invalid(بنية strbuf *buf,
					    بنية strbuf *پیغام,
					    اندازه_t sig_دننه_واټن)
{
	بنية strbuf signature = STRBUF_INIT;
	بنية strbuf payload = STRBUF_INIT;
	بنية signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_جمع_کړه(&payload, پیغام->buf, sig_دننه_واټن);
	strbuf_جمع_کړه(&signature, پیغام->buf + sig_دننه_واټن, پیغام->len - sig_دننه_واټن);

	sigc.payload_type = SIGNATURE_PAYLOAD_نښان;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	إذا (!check_signature(&sigc, signature.buf, signature.len))
		اذهب بهر;

	إذا (signed_نښان_ډېرپېښ == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(پیغام, sig_دننه_واټن);

	إذا (signed_نښان_ډېرپېښ == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = عدم;
		strbuf_reset(&signature);

		إذا (sign_بفر(&payload, &signature, signed_نښان_keyid,
				SIGN_بفر_USE_DEFAULT_کلی))
			die(_("failed to sign tag object"));

		strbuf_addbuf(پیغام, &signature);
	}

بهر:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

ساكن فراغ handle_نښان_signature(بنية strbuf *buf, بنية strbuf *پیغام, ثابت حرف *name)
{
	اندازه_t sig_دننه_واټن = parse_signed_بفر(پیغام->buf, پیغام->len);

	/* If there is no signature, there is nothing to do. */
	إذا (sig_دننه_واټن >= پیغام->len)
		أرجع;

	بدل (signed_نښان_ډېرپېښ) {

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
		strbuf_setlen(پیغام, sig_دننه_واټن);
		اكسر;
	حالة SIGN_ABORT_IF_INVALID:
	حالة SIGN_SIGN_IF_INVALID:
	حالة SIGN_STRIP_IF_INVALID:
		handle_نښان_signature_if_invalid(buf, پیغام, sig_دننه_واټن);
		اكسر;

	/* Third, aborting modes */
	حالة SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	افتراضي:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_نښان_ډېرپېښ, name);
	}
}

ساكن فراغ parse_new_نښان(بنية fast_import_state *state, ثابت حرف *arg)
{
	ساكن بنية strbuf پیغام = STRBUF_INIT;
	ثابت حرف *from;
	حرف *tagger;
	بنية branch *s;
	بنية نښان *t;
	uintmax_t from_mark = 0;
	بنية object_id oid;
	تعداد object_type type;
	ثابت حرف *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية نښان));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	إذا (وروستی_نښان)
		وروستی_نښان->بل_نښان = t;
	وإلا
		لومړی_نښان = t;
	وروستی_نښان = t;
	read_بل_command(state);
	parse_mark(state);

	/* from ... */
	إذا (!skip_مختاړی(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	إذا (s) {
		إذا (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} وإلا إذا (*from == ':') {
		بنية object_ثبت *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = پیدا_کړه_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} وإلا إذا (!repo_get_oid(the_repository, from, &oid)) {
		بنية object_ثبت *oe = پیدا_کړه_object(&oid);
		إذا (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, عدم);
			إذا (type < 0)
				die(_("not a valid object: %s"), from);
		} وإلا
			type = oe->type;
	} وإلا
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_بل_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	إذا (skip_مختاړی(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_بل_command(state);
	} وإلا
		tagger = عدم;

	/* tag payload/message */
	parse_معلومات(&پیغام, 0, عدم);

	/* build the tag object */
	strbuf_reset(&new_معلومات);

	strbuf_addf(&new_معلومات,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_منزل_hex(&oid), type_name(type), t->name);
	إذا (tagger)
		strbuf_addf(&new_معلومات,
			    "tagger %s\n", tagger);

	handle_نښان_signature(&new_معلومات, &پیغام, t->name);

	strbuf_addch(&new_معلومات, '\n');
	strbuf_addbuf(&new_معلومات, &پیغام);
	حرر(tagger);

	إذا (store_object(OBJ_نښان, &new_معلومات, عدم, &t->oid, بل_mark))
		t->pack_id = MAX_PACK_ID;
	وإلا
		t->pack_id = pack_id;
}

ساكن فراغ parse_reset_branch(بنية fast_import_state *state, ثابت حرف *arg)
{
	بنية branch *b;
	ثابت حرف *نښان_name;

	b = lookup_branch(arg);
	إذا (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_ونه.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_ونه.versions[1].oid, the_repository->hash_algo);
		إذا (b->branch_ونه.ونه) {
			release_ونه_content_recursive(b->branch_ونه.ونه);
			b->branch_ونه.ونه = عدم;
		}
	}
	وإلا
		b = new_branch(arg);
	read_بل_command(state);
	parse_from(state, b);
	إذا (b->delete && skip_مختاړی(b->name, "refs/tags/", &نښان_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		بنية نښان *t, *prev = عدم;
		لكل (t = لومړی_نښان; t; t = t->بل_نښان) {
			إذا (!strcmp(t->name, نښان_name))
				اكسر;
			prev = t;
		}
		إذا (t) {
			إذا (prev)
				prev->بل_نښان = t->بل_نښان;
			وإلا
				لومړی_نښان = t->بل_نښان;
			إذا (!t->بل_نښان)
				وروستی_نښان = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	إذا (command_buf.len > 0)
		unread_command_buf = 1;
}

ساكن فراغ cat_blob_write(ثابت حرف *buf, بینښه اوږد اندازه)
{
	إذا (write_دننه_ډک(cat_blob_fd, buf, اندازه) < 0)
		die_errno(_("write to frontend failed"));
}

ساكن فراغ cat_blob(بنية object_ثبت *oe, بنية object_id *oid)
{
	بنية strbuf کرښه = STRBUF_INIT;
	بنية iovec iov[3];
	بینښه اوږد اندازه;
	تعداد object_type type = 0;
	حرف *buf;

	إذا (!oe || oe->pack_id == MAX_PACK_ID) {
		اندازه_t اندازه_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &اندازه_st);
		اندازه = cast_اندازه_t_منزل_ulong(اندازه_st);
	} وإلا {
		type = oe->type;
		buf = gfi_unpack_ثبت(oe, &اندازه);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	إذا (type <= 0) {
		strbuf_reset(&کرښه);
		strbuf_addf(&کرښه, "%s missing\n", oid_منزل_hex(oid));
		cat_blob_write(کرښه.buf, کرښه.len);
		strbuf_release(&کرښه);
		حرر(buf);
		أرجع;
	}
	إذا (!buf)
		die(_("can't read object %s"), oid_منزل_hex(oid));
	إذا (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_منزل_hex(oid), type_name(type));
	strbuf_reset(&کرښه);
	strbuf_addf(&کرښه, "%s %s %"PRIuMAX"\n", oid_منزل_hex(oid),
		    type_name(type), (uintmax_t)اندازه);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_بنسټ = کرښه.buf;
	iov[0].iov_len = کرښه.len;
	iov[1].iov_بنسټ = buf;
	iov[1].iov_len = اندازه;
	iov[2].iov_بنسټ = (فراغ *) "\n";
	iov[2].iov_len = 1;

	إذا (writev_دننه_ډک(cat_blob_fd, iov, ARRAY_اندازه(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&کرښه);
	إذا (oe && oe->pack_id == pack_id) {
		وروستی_blob.دننه_واټن = oe->idx.دننه_واټن;
		strbuf_attach(&وروستی_blob.معلومات, buf, اندازه, اندازه + 1);
		وروستی_blob.ژوروالی = oe->ژوروالی;
	} وإلا
		حرر(buf);
}

ساكن فراغ parse_get_mark(بنية fast_import_state *state ناکارول_شوی,
			   ثابت حرف *p)
{
	بنية object_ثبت *oe;
	حرف output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	إذا (*p != ':')
		die(_("not a mark: %s"), p);

	oe = پیدا_کړه_mark(marks, parse_mark_ref_eol(p));
	إذا (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, حجم(output), "%s\n", oid_منزل_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

ساكن فراغ parse_cat_blob(بنية fast_import_state *state ناکارول_شوی,
			   ثابت حرف *p)
{
	بنية object_ثبت *oe;
	بنية object_id oid;

	/* cat-blob SP <object> LF */
	إذا (*p == ':') {
		oe = پیدا_کړه_mark(marks, parse_mark_ref_eol(p));
		إذا (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} وإلا {
		إذا (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		إذا (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = پیدا_کړه_object(&oid);
	}

	cat_blob(oe, &oid);
}

ساكن بنية object_ثبت *dereference(بنية object_ثبت *oe,
					بنية object_id *oid)
{
	بینښه اوږد اندازه;
	حرف *buf = عدم;
	ثابت بینښه hexsz = the_hash_algo->hexsz;

	إذا (!oe) {
		تعداد object_type type = odb_read_object_info(the_repository->objects,
							     oid, عدم);
		إذا (type < 0)
			die(_("object not found: %s"), oid_منزل_hex(oid));
		/* cache it! */
		oe = ورننباسه_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.دننه_واټن = 1;
	}
	بدل (oe->type) {
	حالة OBJ_ونه:	/* easy case. */
		أرجع oe;
	حالة OBJ_COMMIT:
	حالة OBJ_نښان:
		اكسر;
	افتراضي:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	إذا (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_ثبت(oe, &اندازه);
	} وإلا {
		تعداد object_type ناکارول_شوی_2;
		اندازه_t اندازه_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &ناکارول_شوی_2, &اندازه_st);
		اندازه = cast_اندازه_t_منزل_ulong(اندازه_st);
	}
	إذا (!buf)
		die(_("can't load object %s"), oid_منزل_hex(oid));

	/* Peel one layer. */
	بدل (oe->type) {
	حالة OBJ_نښان:
		إذا (اندازه < hexsz + مزي_اوږدوالی("object ") ||
		    get_oid_hex(buf + مزي_اوږدوالی("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		اكسر;
	حالة OBJ_COMMIT:
		إذا (اندازه < hexsz + مزي_اوږدوالی("tree ") ||
		    get_oid_hex(buf + مزي_اوږدوالی("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	حرر(buf);
	أرجع پیدا_کړه_object(oid);
}

ساكن فراغ ورننباسه_mapped_mark(uintmax_t mark, فراغ *object, فراغ *cbp)
{
	بنية object_id *fromoid = object;
	بنية object_id *tooid = پیدا_کړه_mark(cbp, mark);
	صحيح ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	إذا (ret == 0)
		أرجع;
	kh_ارزښت(sub_oid_map, it) = tooid;
}

ساكن فراغ build_mark_map_one(بنية mark_سېټ *from, بنية mark_سېټ *منزل)
{
	for_each_mark(from, 0, ورننباسه_mapped_mark, منزل);
}

ساكن فراغ build_mark_map(بنية string_list *from, بنية string_list *منزل)
{
	بنية string_list_توکی *fromp, *سر_2;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_توکی(fromp, from) {
		سر_2 = string_list_lookup(منزل, fromp->string);
		إذا (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} وإلا إذا (!سر_2 || !سر_2->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, سر_2->util);
	}
}

ساكن بنية object_ثبت *parse_treeish_dataref(ثابت حرف **p)
{
	بنية object_id oid;
	بنية object_ثبت *e;

	إذا (**p == ':') {	/* <mark> */
		e = پیدا_کړه_mark(marks, parse_mark_ref_space(p));
		إذا (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} وإلا {	/* <sha1> */
		إذا (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = پیدا_کړه_object(&oid);
		إذا (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	طالما (!e || e->type != OBJ_ونه)
		e = dereference(e, &oid);
	أرجع e;
}

ساكن فراغ print_ls(صحيح ډېرپېښ, ثابت بینښه حرف *hash, ثابت حرف *لار)
{
	ساكن بنية strbuf کرښه = STRBUF_INIT;

	/* See show_tree(). */
	ثابت حرف *type =
		S_ISGITLINK(ډېرپېښ) ? commit_type :
		S_ISDIR(ډېرپېښ) ? ونه_type :
		blob_type;

	إذا (!ډېرپېښ) {
		/* missing SP path LF */
		strbuf_reset(&کرښه);
		strbuf_addstr(&کرښه, "missing ");
		quote_c_style(لار, &کرښه, عدم, 0);
		strbuf_addch(&کرښه, '\n');
	} وإلا {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&کرښه);
		strbuf_addf(&کرښه, "%06o %s %s\t",
				ډېرپېښ & ~NO_DELTA, type, hash_منزل_hex(hash));
		quote_c_style(لار, &کرښه, عدم, 0);
		strbuf_addch(&کرښه, '\n');
	}
	cat_blob_write(کرښه.buf, کرښه.len);
}

ساكن فراغ parse_ls(بنية fast_import_state *state ناکارول_شوی,
		     ثابت حرف *p,
		     بنية branch *b)
{
	ساكن بنية strbuf لار = STRBUF_INIT;
	بنية ونه_ثبت *ریښه = عدم;
	بنية ونه_ثبت ونې_پاڼه = {عدم};

	/* ls SP (<tree-ish> SP)? <path> */
	إذا (*p == '"') {
		إذا (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		ریښه = &b->branch_ونه;
	} وإلا {
		بنية object_ثبت *e = parse_treeish_dataref(&p);
		ریښه = new_ونه_ثبت();
		oidcpy(&ریښه->versions[1].oid, &e->idx.oid);
		إذا (!is_null_oid(&ریښه->versions[1].oid))
			ریښه->versions[1].ډېرپېښ = S_IFDIR;
		بارول_ونه(ریښه);
	}
	strbuf_reset(&لار);
	parse_لار_eol(&لار, p, "path");
	ونه_content_get(ریښه, لار.buf, &ونې_پاڼه, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	إذا (S_ISDIR(ونې_پاڼه.versions[1].ډېرپېښ))
		store_ونه(&ونې_پاڼه);

	print_ls(ونې_پاڼه.versions[1].ډېرپېښ, ونې_پاڼه.versions[1].oid.hash, لار.buf);
	إذا (ونې_پاڼه.ونه)
		release_ونه_content_recursive(ونې_پاڼه.ونه);
	إذا (!b || ریښه != &b->branch_ونه)
		release_ونه_ثبت(ریښه);
}

ساكن فراغ checkpoint(فراغ)
{
	checkpoint_requested = 0;
	إذا (object_شمېر) {
		دوره_packfile();
	}
	dump_branches();
	dump_نښانونه();
	dump_marks();
}

ساكن فراغ parse_checkpoint(بنية fast_import_state *state ناکارول_شوی)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

ساكن فراغ parse_پرمختګ(بنية fast_import_state *state ناکارول_شوی)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

ساكن فراغ parse_alias(بنية fast_import_state *state)
{
	بنية object_ثبت *e;
	بنية branch b;

	skip_optional_lf();
	read_بل_command(state);

	/* mark ... */
	parse_mark(state);
	إذا (!بل_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, حجم(b));
	إذا (!parse_objectish_with_مختاړی(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = پیدا_کړه_object(&b.oid);
	أكد(e);
	ورننباسه_mark(&marks, بل_mark, e);
}

ساكن حرف* make_fast_import_لار(بنية fast_import_state *state,
				   ثابت حرف *لار)
{
	إذا (!relative_marks_paths || is_absolute_لار(لار))
		أرجع مختاړی_filename(state->مختاړی, لار);
	أرجع repo_git_لار(the_repository, "info/fast-import/%s", لار);
}

ساكن فراغ option_import_marks(بنية fast_import_state *state,
				ثابت حرف *marks,
				صحيح from_بهیر,
				صحيح ignore_missing)
{
	إذا (import_marks_file) {
		إذا (from_بهیر)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		إذا(!import_marks_file_from_بهیر)
			read_marks();
	}

	حرر(import_marks_file);
	import_marks_file = make_fast_import_لار(state, marks);
	import_marks_file_from_بهیر = from_بهیر;
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

ساكن بینښه اوږد ulong_arg(ثابت حرف *option, ثابت حرف *arg)
{
	حرف *endptr;
	بینښه اوږد rv = strtoul(arg, &endptr, 0);
	إذا (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	أرجع rv;
}

ساكن فراغ option_ژوروالی(ثابت حرف *ژوروالی)
{
	max_ژوروالی = ulong_arg("--depth", ژوروالی);
	إذا (max_ژوروالی > MAX_ژوروالی)
		die(_("--depth cannot exceed %u"), MAX_ژوروالی);
}

ساكن فراغ option_فعال_branches(ثابت حرف *branches)
{
	max_فعال_branches = ulong_arg("--active-branches", branches);
}

ساكن فراغ option_export_marks(بنية fast_import_state *state,
				ثابت حرف *marks)
{
	حرر(export_marks_file);
	export_marks_file = make_fast_import_لار(state, marks);
}

ساكن فراغ option_cat_blob_fd(بنية fast_import_state *state ناکارول_شوی,
			       ثابت حرف *fd)
{
	بینښه اوږد n = ulong_arg("--cat-blob-fd", fd);
	إذا (n > (بینښه اوږد) لوی_عدد)
		die(_("--cat-blob-fd cannot exceed %d"), لوی_عدد);
	cat_blob_fd = (صحيح) n;
}

ساكن فراغ option_export_pack_څنډې(بنية fast_import_state *state,
				     ثابت حرف *څنډې)
{
	حرف *fn = مختاړی_filename(state->مختاړی, څنډې);
	إذا (pack_څنډې)
		fclose(pack_څنډې);
	pack_څنډې = xfopen(fn, "a");
	حرر(fn);
}

ساكن فراغ option_rewrite_submodules(بنية fast_import_state *state,
				      ثابت حرف *arg,
				      بنية string_list *list)
{
	بنية mark_سېټ *ms;
	FILE *fp;
	حرف *s = xstrdup(arg);
	حرف *f = strchr(s, ':');
	إذا (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = مختاړی_filename(state->مختاړی, f);
	fp = fopen(f, "r");
	إذا (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, ورننباسه_oid_ثبت);
	fclose(fp);
	حرر(f);

	string_list_ورننباسه(list, s)->util = ms;

	حرر(s);
}

ساكن فراغ option_max_pack_اندازه(ثابت حرف *arg)
{
	بینښه اوږد v;

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

ساكن فراغ option_big_file_پوله(ثابت حرف *arg)
{
	بینښه اوږد v;

	إذا (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_سېټ_big_file_پوله(the_repository, v);
}

ساكن فراغ option_signed_commits(ثابت حرف *arg)
{
	إذا (parse_sign_ډېرپېښ(arg, &signed_commit_ډېرپېښ, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

ساكن فراغ option_signed_نښانونه(ثابت حرف *arg)
{
	إذا (parse_sign_ډېرپېښ(arg, &signed_نښان_ډېرپېښ, &signed_نښان_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

ساكن فراغ option_quiet(فراغ)
{
	show_stats = 0;
	quiet = 1;
}

ساكن صحيح parse_one_option(بنية fast_import_state *state, ثابت حرف *option)
{
	إذا (skip_مختاړی(option, "max-pack-size=", &option)) {
		option_max_pack_اندازه(option);
	} وإلا إذا (skip_مختاړی(option, "big-file-threshold=", &option)) {
		option_big_file_پوله(option);
	} وإلا إذا (skip_مختاړی(option, "depth=", &option)) {
		option_ژوروالی(option);
	} وإلا إذا (skip_مختاړی(option, "active-branches=", &option)) {
		option_فعال_branches(option);
	} وإلا إذا (skip_مختاړی(option, "export-pack-edges=", &option)) {
		option_export_pack_څنډې(state, option);
	} وإلا إذا (skip_مختاړی(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} وإلا إذا (skip_مختاړی(option, "signed-tags=", &option)) {
		option_signed_نښانونه(option);
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

	إذا (skip_مختاړی(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} وإلا إذا (skip_مختاړی(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} وإلا إذا (skip_مختاړی(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} وإلا إذا (skip_مختاړی(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} وإلا إذا (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} وإلا إذا (skip_مختاړی(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_منزل);
	} وإلا إذا (skip_مختاړی(feature, "rewrite-submodules-from=", &arg)) {
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
	إذا (state->کتل_شوی_معلومات_command)
		die(_("got feature command '%s' after data command"), feature);

	إذا (parse_one_feature(state, feature))
		أرجع;

	die(_("this version of fast-import does not support feature %s."), feature);
}

ساكن فراغ parse_option(بنية fast_import_state *state, ثابت حرف *option)
{
	إذا (state->کتل_شوی_معلومات_command)
		die(_("got option command '%s' after data command"), option);

	إذا (parse_one_option(state, option))
		أرجع;

	die(_("this version of fast-import does not support option: %s"), option);
}

ساكن فراغ git_pack_config(فراغ)
{
	صحيح indexversion_ارزښت;
	صحيح حد;
	بینښه اوږد packsizelimit_ارزښت;

	إذا (!repo_config_get_ulong(the_repository, "pack.depth", &max_ژوروالی)) {
		إذا (max_ژوروالی > MAX_ژوروالی)
			max_ژوروالی = MAX_ژوروالی;
	}
	إذا (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_ارزښت)) {
		pack_idx_opts.نسخه = indexversion_ارزښت;
		إذا (pack_idx_opts.نسخه > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.نسخه);
	}
	إذا (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_ارزښت))
		max_packsize = packsizelimit_ارزښت;

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
	صحيح argc = parse_options(state->argc, state->argv, state->مختاړی,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	إذا (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->کتل_شوی_معلومات_command = 1;
	إذا (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_منزل);
}

ساكن صحيح option_parse_date_format(ثابت بنية option *opt ناکارول_شوی,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	أرجع 0;
}

ساكن صحيح option_parse_export_pack_څنډې(ثابت بنية option *opt,
					  ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_څنډې(opt->ارزښت, arg);
	أرجع 0;
}

ساكن صحيح option_parse_max_pack_اندازه(ثابت بنية option *opt ناکارول_شوی,
				      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_اندازه(arg);
	أرجع 0;
}

ساكن صحيح option_parse_big_file_پوله(ثابت بنية option *opt ناکارول_شوی,
					   ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_پوله(arg);
	أرجع 0;
}

ساكن صحيح option_parse_signed_commits(ثابت بنية option *opt ناکارول_شوی,
				       ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	أرجع 0;
}

ساكن صحيح option_parse_signed_نښانونه(ثابت بنية option *opt ناکارول_شوی,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_نښانونه(arg);
	أرجع 0;
}

ساكن صحيح option_parse_rewrite_submodules_from(ثابت بنية option *opt,
						ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->ارزښت, arg, &sub_marks_from);
	أرجع 0;
}

ساكن صحيح option_parse_rewrite_submodules_منزل(ثابت بنية option *opt,
					      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->ارزښت, arg, &sub_marks_منزل);
	أرجع 0;
}

ساكن صحيح option_parse_cat_blob_fd(ثابت بنية option *opt,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->ارزښت, arg);
	أرجع 0;
}

ساكن صحيح option_parse_import_marks(ثابت بنية option *opt,
				     ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->ارزښت, arg, 0, 0);
	أرجع 0;
}

ساكن صحيح option_parse_import_marks_if_exists(ثابت بنية option *opt,
					       ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->ارزښت, arg, 0, 1);
	أرجع 0;
}

ساكن صحيح option_parse_export_marks(ثابت بنية option *opt,
				     ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->ارزښت, arg);
	أرجع 0;
}

ساكن صحيح option_parse_ژوروالی(ثابت بنية option *opt ناکارول_شوی,
			      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_ژوروالی(arg);
	أرجع 0;
}

ساكن صحيح option_parse_فعال_branches(ثابت بنية option *opt ناکارول_شوی,
					ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_فعال_branches(arg);
	أرجع 0;
}

ساكن صحيح option_parse_quiet(ثابت بنية option *opt ناکارول_شوی,
			      ثابت حرف *arg ناکارول_شوی, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	أرجع 0;
}

صحيح cmd_fast_import(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *مختاړی,
		    بنية repository *repo)
{
	بنية fast_import_state state;

	بنية option fast_import_options[] = {
		OPT_ډله(N_("Common")),
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
			       PARSE_OPT_NONEG, option_parse_big_file_پوله),
		OPT_CALLBACK_F(0, "depth", عدم, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_ژوروالی),
		OPT_CALLBACK_F(0, "active-branches", عدم, N_("n"),
			       N_("maximum number of branches to maintain active"),
			       PARSE_OPT_NONEG, option_parse_فعال_branches),
		OPT_ډله(N_("Marks")),
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
		OPT_ډله(N_("Submodule rewrite")),
		OPT_CALLBACK_F(0, "rewrite-submodules-from", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> from <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_from),
		OPT_CALLBACK_F(0, "rewrite-submodules-to", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> to <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_منزل),
		OPT_ډله(N_("Signing")),
		OPT_CALLBACK_F(0, "signed-commits", عدم, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", عدم, N_("mode"),
			       N_("how to handle signed tags"),
			       PARSE_OPT_NONEG, option_parse_signed_نښانونه),
		OPT_HIDDEN_ډله(N_("Advanced")),
		OPT_BOOL_F(0, "allow-unsafe-features", &state.allow_unsafe_features,
			   N_("allow unsafe mark commands from the stream"),
			   PARSE_OPT_HIDDEN | PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "export-pack-edges", &state, N_("file"),
			       N_("dump edge commits to <file>"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_export_pack_څنډې),
		OPT_CALLBACK_F(0, "cat-blob-fd", &state, N_("fd"),
			       N_("write some responses to <fd> instead of stdout"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_cat_blob_fd),
		OPT_پای_ټکی()
	};

	show_usage_with_options_if_asked(argc, argv, fast_import_usage, fast_import_options);

	fast_import_state_init(&state, argc, argv, مختاړی, fast_import_options);

	reset_pack_idx_option(&pack_idx_opts);
	git_pack_config();

	alloc_objects(object_ثبت_alloc);
	strbuf_init(&command_buf, 0);
	CALLOC_ARRAY(atom_جدول, atom_جدول_sz);
	CALLOC_ARRAY(branch_جدول, branch_جدول_sz);
	CALLOC_ARRAY(avail_ونه_جدول, avail_ونه_جدول_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_سېټ));

	hashmap_init(&object_جدول, object_ثبت_hashcmp, عدم, 0);

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
	لكل (بینښه صحيح i = 0; i < (cmd_save - 1); i++)
		rc_free[i].بل = &rc_free[i + 1];
	rc_free[cmd_save - 1].بل = عدم;

	پیل_packfile();
	سېټ_die_routine(die_nicely);
	سېټ_checkpoint_signal();
	طالما (read_بل_command(&state) != EOF) {
		ثابت حرف *v;
		إذا (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		وإلا إذا (skip_مختاړی(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		وإلا إذا (skip_مختاړی(command_buf.buf, "tag ", &v))
			parse_new_نښان(&state, v);
		وإلا إذا (skip_مختاړی(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		وإلا إذا (skip_مختاړی(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, عدم);
		وإلا إذا (skip_مختاړی(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		وإلا إذا (skip_مختاړی(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		وإلا إذا (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		وإلا إذا (!strcmp("done", command_buf.buf))
			اكسر;
		وإلا إذا (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		وإلا إذا (starts_with(command_buf.buf, "progress "))
			parse_پرمختګ(&state);
		وإلا إذا (skip_مختاړی(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		وإلا إذا (skip_مختاړی(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		وإلا إذا (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		وإلا
			die(_("unsupported command: %s"), command_buf.buf);

		إذا (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	إذا (!state.کتل_شوی_معلومات_command)
		parse_argv(&state);

	إذا (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	پای_ټکی_packfile();

	dump_branches();
	dump_نښانونه();
	unkeep_all_packs();
	dump_marks();

	إذا (pack_څنډې)
		fclose(pack_څنډې);

	إذا (show_stats) {
		uintmax_t ټول_شمېر = 0, duplicate_شمېر = 0;
		لكل (اندازه_t i = 0; i < ARRAY_اندازه(object_شمېر_by_type); i++)
			ټول_شمېر += object_شمېر_by_type[i];
		لكل (اندازه_t i = 0; i < ARRAY_اندازه(duplicate_شمېر_by_type); i++)
			duplicate_شمېر += duplicate_شمېر_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_شمېر);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", ټول_شمېر, duplicate_شمېر);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_شمېر_by_type[OBJ_BLOB], duplicate_شمېر_by_type[OBJ_BLOB], delta_شمېر_by_type[OBJ_BLOB], delta_شمېر_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_شمېر_by_type[OBJ_ونه], duplicate_شمېر_by_type[OBJ_ونه], delta_شمېر_by_type[OBJ_ونه], delta_شمېر_attempts_by_type[OBJ_ونه]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_شمېر_by_type[OBJ_COMMIT], duplicate_شمېر_by_type[OBJ_COMMIT], delta_شمېر_by_type[OBJ_COMMIT], delta_شمېر_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_شمېر_by_type[OBJ_نښان], duplicate_شمېر_by_type[OBJ_نښان], delta_شمېر_by_type[OBJ_نښان], delta_شمېر_attempts_by_type[OBJ_نښان]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_شمېر, branch_بارول_شمېر);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->ښويدنه) * 1024, marks_سېټ_شمېر);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (ونه_ثبت_allocd + fi_mem_pool.pool_alloc + alloc_شمېر*حجم(بنية object_ثبت))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (بینښه اوږد)((ونه_ثبت_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_شمېر*حجم(بنية object_ثبت))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	أرجع failure ? 1 : 0;
}
