#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

#وصف_ڪريو PACK_ID_بٽ 16
#وصف_ڪريو MAX_PACK_ID ((1<<PACK_ID_بٽ)-1)
#وصف_ڪريو اونهائي_بٽ 13
#وصف_ڪريو MAX_اونهائي ((1<<اونهائي_بٽ)-1)

/*
 * We abuse the setuid bit on directories to mean "do not delta".
 */
#وصف_ڪريو NO_DELTA S_ISUID

/*
 * The amount of additional space required in order to write an object into the
 * current pack. This is the hash lengths at the end of the pack, plus the
 * length of one object ID.
 */
#وصف_ڪريو PACK_سائيز_حد (the_hash_algo->rawsz * 3)

بنية object_داخلا {
	بنية pack_idx_داخلا idx;
	بنية hashmap_داخلا ent;
	uint32_t type : TYPE_بٽ,
		pack_id : PACK_ID_بٽ,
		اونهائي : اونهائي_بٽ;
};

ساكن صحيح object_داخلا_hashcmp(ثابت فراغ *map_معلومات اڻ_استعمال_ٿيل,
				ثابت بنية hashmap_داخلا *eptr,
				ثابت بنية hashmap_داخلا *داخلا_or_ڪنجي,
				ثابت فراغ *keydata)
{
	ثابت بنية object_id *oid = keydata;
	ثابت بنية object_داخلا *e1, *e2;

	e1 = container_جو(eptr, ثابت بنية object_داخلا, ent);
	إذا (oid)
		أرجع oidcmp(&e1->idx.oid, oid);

	e2 = container_جو(داخلا_or_ڪنجي, ثابت بنية object_داخلا, ent);
	أرجع oidcmp(&e1->idx.oid, &e2->idx.oid);
}

بنية object_داخلا_pool {
	بنية object_داخلا_pool *ايندڙ_pool;
	بنية object_داخلا *ايندڙ_free;
	بنية object_داخلا *پڇاڙي;
	بنية object_داخلا entries[FLEX_ARRAY]; /*وڌيڪ*/
};

بنية mark_سيٽ {
	اتحاد {
		بنية object_id *oids[1024];
		بنية object_داخلا *marked[1024];
		بنية mark_سيٽ *sets[1024];
	} معلومات;
	بنانشان صحيح سرڪايو;
};

بنية آخري_object {
	بنية strbuf معلومات;
	off_t وٿي;
	بنانشان صحيح اونهائي;
	بنانشان no_مٽا_سٽا : 1;
};

بنية atom_str {
	بنية atom_str *ايندڙ_atom;
	بنانشان ننڍو str_len;
	حرف str_dat[FLEX_ARRAY]; /*وڌيڪ*/
};

بنية وڻ_content;
بنية وڻ_داخلا {
	بنية وڻ_content *وڻ;
	بنية atom_str *name;
	بنية وڻ_داخلا_ms {
		uint16_t گهڻائي;
		بنية object_id oid;
	} versions[2];
};

بنية وڻ_content {
	بنانشان صحيح داخلا_صلاحيت; /* must match avail_tree_content */
	بنانشان صحيح داخلا_ڳڻپ;
	بنانشان صحيح delta_اونهائي;
	بنية وڻ_داخلا *entries[FLEX_ARRAY]; /*وڌيڪ*/
};

بنية avail_وڻ_content {
	بنانشان صحيح داخلا_صلاحيت; /* must match tree_content */
	بنية avail_وڻ_content *ايندڙ_avail;
};

بنية branch {
	بنية branch *جدول_ايندڙ_branch;
	بنية branch *سرگرم_ايندڙ_branch;
	ثابت حرف *name;
	بنية وڻ_داخلا branch_وڻ;
	uintmax_t آخري_commit;
	uintmax_t انگ_notes;
	بنانشان سرگرم : 1;
	بنانشان delete : 1;
	بنانشان pack_id : PACK_ID_بٽ;
	بنية object_id oid;
};

بنية ٽيگ {
	بنية ٽيگ *ايندڙ_ٽيگ;
	ثابت حرف *name;
	بنانشان صحيح pack_id;
	بنية object_id oid;
};

بنية hash_list {
	بنية hash_list *ايندڙ;
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
	بنية recent_command *ايندڙ;
	حرف *buf;
};

تعريفنوع فراغ (*mark_سيٽ_inserter_t)(بنية mark_سيٽ **s, بنية object_id *oid, uintmax_t mark);
تعريفنوع فراغ (*each_mark_fn_t)(uintmax_t mark, فراغ *obj, فراغ *cbp);

/* Configured limits on output */
ساكن بنانشان ڊگهو max_اونهائي = 50;
ساكن off_t max_packsize;
ساكن صحيح unpack_حد_بندي = 100;
ساكن صحيح force_update;

/* Stats and misc. counters */
ساكن uintmax_t alloc_ڳڻپ;
ساكن uintmax_t marks_سيٽ_ڳڻپ;
ساكن uintmax_t object_ڳڻپ_by_type[1 << TYPE_بٽ];
ساكن uintmax_t duplicate_ڳڻپ_by_type[1 << TYPE_بٽ];
ساكن uintmax_t delta_ڳڻپ_by_type[1 << TYPE_بٽ];
ساكن uintmax_t delta_ڳڻپ_attempts_by_type[1 << TYPE_بٽ];
ساكن بنانشان ڊگهو object_ڳڻپ;
ساكن بنانشان ڊگهو branch_ڳڻپ;
ساكن بنانشان ڊگهو branch_ڀريو_ڳڻپ;
ساكن صحيح failure;
ساكن FILE *pack_ڪنارا;
ساكن صحيح show_stats = 1;
ساكن صحيح quiet;
ساكن تعداد sign_گهڻائي signed_ٽيگ_گهڻائي = SIGN_VERBATIM;
ساكن تعداد sign_گهڻائي signed_commit_گهڻائي = SIGN_VERBATIM;
ساكن ثابت حرف *signed_commit_keyid;
ساكن ثابت حرف *signed_ٽيگ_keyid;

/* Memory pools */
ساكن بنية mem_pool fi_mem_pool = {
	.بلاڪ_alloc = 2*1024*1024 - حجم(بنية mp_بلاڪ),
};

/* Atom management */
ساكن بنانشان صحيح atom_جدول_sz = 4451;
ساكن بنانشان صحيح atom_cnt;
ساكن بنية atom_str **atom_جدول;

/* The .pack file being generated */
ساكن بنية pack_idx_option pack_idx_opts;
ساكن بنانشان صحيح pack_id;
ساكن بنية hashfile *pack_file;
ساكن بنية packed_git *pack_معلومات;
ساكن بنية packed_git **all_packs;
ساكن off_t pack_سائيز;

/* Table of objects we've written. */
ساكن بنانشان صحيح object_داخلا_alloc = 5000;
ساكن بنية object_داخلا_pool *بلاڪون;
ساكن بنية hashmap object_جدول;
ساكن بنية mark_سيٽ *marks;
ساكن حرف *export_marks_file;
ساكن حرف *import_marks_file;
ساكن صحيح import_marks_file_from_وهڪرو;
ساكن صحيح import_marks_file_ignore_missing;
ساكن صحيح import_marks_file_پورو;
ساكن صحيح relative_marks_paths;

/* Our last blob */
ساكن بنية آخري_object آخري_blob = {
	.معلومات = STRBUF_INIT,
 };

/* Tree management */
ساكن بنانشان صحيح وڻ_داخلا_alloc = 1000;
ساكن فراغ *avail_وڻ_داخلا;
ساكن بنانشان صحيح avail_وڻ_جدول_sz = 100;
ساكن بنية avail_وڻ_content **avail_وڻ_جدول;
ساكن سائيز_t وڻ_داخلا_allocd;
ساكن بنية strbuf old_وڻ = STRBUF_INIT;
ساكن بنية strbuf new_وڻ = STRBUF_INIT;

/* Branch data */
ساكن بنانشان ڊگهو max_سرگرم_branches = 5;
ساكن بنانشان ڊگهو cur_سرگرم_branches;
ساكن بنانشان ڊگهو branch_جدول_sz = 1039;
ساكن بنية branch **branch_جدول;
ساكن بنية branch *سرگرم_branches;

/* Tag data */
ساكن بنية ٽيگ *پهريون_ٽيگ;
ساكن بنية ٽيگ *آخري_ٽيگ;

/* Input stream parsing */
ساكن whenspec_type whenspec = WHENSPEC_RAW;
ساكن بنية strbuf command_buf = STRBUF_INIT;
ساكن صحيح unread_command_buf;
ساكن بنية recent_command cmd_hist = {
	.prev = &cmd_hist,
	.ايندڙ = &cmd_hist,
};
ساكن بنية recent_command *cmd_پڇ = &cmd_hist;
ساكن بنية recent_command *rc_free;
ساكن بنانشان صحيح cmd_save = 100;
ساكن uintmax_t ايندڙ_mark;
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
	ثابت حرف *اڳياڙي;
	صحيح ڏٺل_معلومات_command;
	صحيح allow_unsafe_features;
	بنية option *option;
};

ساكن فراغ fast_import_state_init(بنية fast_import_state *state,
				   صحيح argc, ثابت حرف **argv,
				   ثابت حرف *اڳياڙي, بنية option *option)
{
	memset(state, 0, حجم(*state));
	state->argc = argc;
	state->argv = argv;
	state->اڳياڙي = اڳياڙي;
	state->option = option;
}

ساكن فراغ parse_argv(بنية fast_import_state *state);
ساكن فراغ parse_get_mark(بنية fast_import_state *state, ثابت حرف *p);
ساكن فراغ parse_cat_blob(بنية fast_import_state *state, ثابت حرف *p);
ساكن فراغ parse_ls(بنية fast_import_state *state,
		     ثابت حرف *p,
		     بنية branch *b);

ساكن فراغ for_each_mark(بنية mark_سيٽ *m, uintmax_t بنياد, each_mark_fn_t callback, فراغ *p)
{
	uintmax_t k;
	إذا (m->سرڪايو) {
		لكل (k = 0; k < 1024; k++) {
			إذا (m->معلومات.sets[k])
				for_each_mark(m->معلومات.sets[k], بنياد + (k << m->سرڪايو), callback, p);
		}
	} وإلا {
		لكل (k = 0; k < 1024; k++) {
			إذا (m->معلومات.marked[k])
				callback(بنياد + k, m->معلومات.marked[k], p);
		}
	}
}

ساكن فراغ dump_marks_fn(uintmax_t mark, فراغ *object, فراغ *cbp) {
	بنية object_داخلا *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_منزل_hex(&e->idx.oid));
}

ساكن فراغ write_branch_report(FILE *rpt, بنية branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	إذا (b->سرگرم)
		fputs(" active", rpt);
	إذا (b->branch_وڻ.وڻ)
		fputs(" loaded", rpt);
	إذا (is_null_oid(&b->branch_وڻ.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_منزل_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_منزل_hex(&b->branch_وڻ.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_منزل_hex(&b->branch_وڻ.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->آخري_commit);

	fputs("  last pack   : ", rpt);
	إذا (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

ساكن فراغ write_crash_report(ثابت حرف *err)
{
	حرف *loc = repo_git_رستو(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	بنية branch *b;
	بنانشان ڊگهو lu;
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
	fprintf(rpt, "    at %s\n", show_date(وقت(عدم), 0, DATE_گهڻائي(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	لكل (rc = cmd_hist.ايندڙ; rc != &cmd_hist; rc = rc->ايندڙ) {
		إذا (rc->ايندڙ == &cmd_hist)
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
	لكل (b = سرگرم_branches, lu = 0; b; b = b->سرگرم_ايندڙ_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->آخري_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	لكل (lu = 0; lu < branch_جدول_sz; lu++) {
		لكل (b = branch_جدول[lu]; b; b = b->جدول_ايندڙ_branch)
			write_branch_report(rpt, b);
	}

	إذا (پهريون_ٽيگ) {
		بنية ٽيگ *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		لكل (tg = پهريون_ٽيگ; tg; tg = tg->ايندڙ_ٽيگ) {
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

ساكن فراغ پڇاڙي_packfile(فراغ);
ساكن فراغ unkeep_all_packs(فراغ);
ساكن فراغ dump_marks(فراغ);

ساكن NORETURN فراغ die_nicely(ثابت حرف *err, va_list params)
{
	va_list cp;
	ساكن صحيح zombie;
	report_fn die_پيغام_fn = get_die_پيغام_routine();

	va_copy(cp, params);
	die_پيغام_fn(err, params);

	إذا (!zombie) {
		حرف پيغام[2 * رستو_MAX];

		zombie = 1;
		vsnprintf(پيغام, حجم(پيغام), err, cp);
		write_crash_report(پيغام);
		پڇاڙي_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	ٻاهر_نڪرو(128);
}

#جيڪڏهن_وصف_نه_ٿيل SIGUSR1	/* Windows, for example */

ساكن فراغ سيٽ_checkpoint_signal(فراغ)
{
}

#وإلا

ساكن فراغ checkpoint_signal(صحيح signo اڻ_استعمال_ٿيل)
{
	checkpoint_requested = 1;
}

ساكن فراغ سيٽ_checkpoint_signal(فراغ)
{
	بنية sigaction sa;

	memset(&sa, 0, حجم(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_نقاب);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, عدم);
}

#جيڪڏهن_پورو

ساكن فراغ alloc_objects(بنانشان صحيح cnt)
{
	بنية object_داخلا_pool *b;

	b = xmalloc(حجم(بنية object_داخلا_pool)
		+ cnt * حجم(بنية object_داخلا));
	b->ايندڙ_pool = بلاڪون;
	b->ايندڙ_free = b->entries;
	b->پڇاڙي = b->entries + cnt;
	بلاڪون = b;
	alloc_ڳڻپ += cnt;
}

ساكن بنية object_داخلا *new_object(بنية object_id *oid)
{
	بنية object_داخلا *e;

	إذا (بلاڪون->ايندڙ_free == بلاڪون->پڇاڙي)
		alloc_objects(object_داخلا_alloc);

	e = بلاڪون->ايندڙ_free++;
	oidcpy(&e->idx.oid, oid);
	أرجع e;
}

ساكن بنية object_داخلا *ڳوليو_object(بنية object_id *oid)
{
	أرجع hashmap_get_داخلا_from_hash(&object_جدول, oidhash(oid), oid,
					   بنية object_داخلا, ent);
}

ساكن بنية object_داخلا *وجهو_object(بنية object_id *oid)
{
	بنية object_داخلا *e;
	بنانشان صحيح hash = oidhash(oid);

	e = hashmap_get_داخلا_from_hash(&object_جدول, hash, oid,
					بنية object_داخلا, ent);
	إذا (!e) {
		e = new_object(oid);
		e->idx.وٿي = 0;
		hashmap_داخلا_init(&e->ent, hash);
		hashmap_جوڙيو(&object_جدول, &e->ent);
	}

	أرجع e;
}

ساكن فراغ invalidate_pack_id(بنانشان صحيح id)
{
	بنانشان ڊگهو lu;
	بنية ٽيگ *t;
	بنية hashmap_iter iter;
	بنية object_داخلا *e;

	hashmap_for_each_داخلا(&object_جدول, &iter, e, ent) {
		إذا (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	لكل (lu = 0; lu < branch_جدول_sz; lu++) {
		بنية branch *b;

		لكل (b = branch_جدول[lu]; b; b = b->جدول_ايندڙ_branch)
			إذا (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	لكل (t = پهريون_ٽيگ; t; t = t->ايندڙ_ٽيگ)
		إذا (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

ساكن بنانشان صحيح hc_str(ثابت حرف *s, سائيز_t len)
{
	بنانشان صحيح r = 0;
	طالما (len-- > 0)
		r = r * 31 + *s++;
	أرجع r;
}

ساكن فراغ وجهو_mark(بنية mark_سيٽ **چوٽي, uintmax_t idnum, بنية object_داخلا *oe)
{
	بنية mark_سيٽ *s = *چوٽي;

	طالما ((idnum >> s->سرڪايو) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_سيٽ));
		s->سرڪايو = (*چوٽي)->سرڪايو + 10;
		s->معلومات.sets[0] = *چوٽي;
		*چوٽي = s;
	}
	طالما (s->سرڪايو) {
		uintmax_t i = idnum >> s->سرڪايو;
		idnum -= i << s->سرڪايو;
		إذا (!s->معلومات.sets[i]) {
			s->معلومات.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_سيٽ));
			s->معلومات.sets[i]->سرڪايو = s->سرڪايو - 10;
		}
		s = s->معلومات.sets[i];
	}
	إذا (!s->معلومات.marked[idnum])
		marks_سيٽ_ڳڻپ++;
	s->معلومات.marked[idnum] = oe;
}

ساكن فراغ *ڳوليو_mark(بنية mark_سيٽ *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	بنية object_داخلا *oe = عدم;
	إذا ((idnum >> s->سرڪايو) < 1024) {
		طالما (s && s->سرڪايو) {
			uintmax_t i = idnum >> s->سرڪايو;
			idnum -= i << s->سرڪايو;
			s = s->معلومات.sets[i];
		}
		إذا (s)
			oe = s->معلومات.marked[idnum];
	}
	إذا (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	أرجع oe;
}

ساكن بنية atom_str *منزل_atom(ثابت حرف *s, بنانشان ننڍو len)
{
	بنانشان صحيح hc = hc_str(s, len) % atom_جدول_sz;
	بنية atom_str *c;

	لكل (c = atom_جدول[hc]; c; c = c->ايندڙ_atom)
		إذا (c->str_len == len && !لڙي_ڀيٽ(s, c->str_dat, len))
			أرجع c;

	c = mem_pool_alloc(&fi_mem_pool, حجم(بنية atom_str) + len + 1);
	c->str_len = len;
	يادگيري_نقل(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->ايندڙ_atom = atom_جدول[hc];
	atom_جدول[hc] = c;
	atom_cnt++;
	أرجع c;
}

ساكن بنية branch *lookup_branch(ثابت حرف *name)
{
	بنانشان صحيح hc = hc_str(name, لڙي_ڊيگهه(name)) % branch_جدول_sz;
	بنية branch *b;

	لكل (b = branch_جدول[hc]; b; b = b->جدول_ايندڙ_branch)
		إذا (!strcmp(name, b->name))
			أرجع b;
	أرجع عدم;
}

ساكن بنية branch *new_branch(ثابت حرف *name)
{
	بنانشان صحيح hc = hc_str(name, لڙي_ڊيگهه(name)) % branch_جدول_sz;
	بنية branch *b = lookup_branch(name);

	إذا (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	إذا (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->جدول_ايندڙ_branch = branch_جدول[hc];
	b->branch_وڻ.versions[0].گهڻائي = S_IFDIR;
	b->branch_وڻ.versions[1].گهڻائي = S_IFDIR;
	b->انگ_notes = 0;
	b->سرگرم = 0;
	b->pack_id = MAX_PACK_ID;
	branch_جدول[hc] = b;
	branch_ڳڻپ++;
	أرجع b;
}

ساكن بنانشان صحيح hc_entries(بنانشان صحيح cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	أرجع cnt < avail_وڻ_جدول_sz ? cnt : avail_وڻ_جدول_sz - 1;
}

ساكن بنية وڻ_content *new_وڻ_content(بنانشان صحيح cnt)
{
	بنية avail_وڻ_content *f, *l = عدم;
	بنية وڻ_content *t;
	بنانشان صحيح hc = hc_entries(cnt);

	لكل (f = avail_وڻ_جدول[hc]; f; l = f, f = f->ايندڙ_avail)
		إذا (f->داخلا_صلاحيت >= cnt)
			اكسر;

	إذا (f) {
		إذا (l)
			l->ايندڙ_avail = f->ايندڙ_avail;
		وإلا
			avail_وڻ_جدول[hc] = f->ايندڙ_avail;
	} وإلا {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, حجم(*t) + حجم(t->entries[0]) * cnt);
		f->داخلا_صلاحيت = cnt;
	}

	t = (بنية وڻ_content*)f;
	t->داخلا_ڳڻپ = 0;
	t->delta_اونهائي = 0;
	أرجع t;
}

ساكن فراغ release_وڻ_داخلا(بنية وڻ_داخلا *e);
ساكن فراغ release_وڻ_content(بنية وڻ_content *t)
{
	بنية avail_وڻ_content *f = (بنية avail_وڻ_content*)t;
	بنانشان صحيح hc = hc_entries(f->داخلا_صلاحيت);
	f->ايندڙ_avail = avail_وڻ_جدول[hc];
	avail_وڻ_جدول[hc] = f;
}

ساكن فراغ release_وڻ_content_recursive(بنية وڻ_content *t)
{
	بنانشان صحيح i;
	لكل (i = 0; i < t->داخلا_ڳڻپ; i++)
		release_وڻ_داخلا(t->entries[i]);
	release_وڻ_content(t);
}

ساكن بنية وڻ_content *grow_وڻ_content(
	بنية وڻ_content *t,
	صحيح amt)
{
	بنية وڻ_content *r = new_وڻ_content(t->داخلا_ڳڻپ + amt);
	r->داخلا_ڳڻپ = t->داخلا_ڳڻپ;
	r->delta_اونهائي = t->delta_اونهائي;
	COPY_ARRAY(r->entries, t->entries, t->داخلا_ڳڻپ);
	release_وڻ_content(t);
	أرجع r;
}

ساكن بنية وڻ_داخلا *new_وڻ_داخلا(فراغ)
{
	بنية وڻ_داخلا *e;

	إذا (!avail_وڻ_داخلا) {
		بنانشان صحيح n = وڻ_داخلا_alloc;
		وڻ_داخلا_allocd += n * حجم(بنية وڻ_داخلا);
		ALLOC_ARRAY(e, n);
		avail_وڻ_داخلا = e;
		طالما (n-- > 1) {
			*((فراغ**)e) = e + 1;
			e++;
		}
		*((فراغ**)e) = عدم;
	}

	e = avail_وڻ_داخلا;
	avail_وڻ_داخلا = *((فراغ**)e);
	أرجع e;
}

ساكن فراغ release_وڻ_داخلا(بنية وڻ_داخلا *e)
{
	إذا (e->وڻ)
		release_وڻ_content_recursive(e->وڻ);
	*((فراغ**)e) = avail_وڻ_داخلا;
	avail_وڻ_داخلا = e;
}

ساكن بنية وڻ_content *dup_وڻ_content(بنية وڻ_content *s)
{
	بنية وڻ_content *d;
	بنية وڻ_داخلا *a, *b;
	بنانشان صحيح i;

	إذا (!s)
		أرجع عدم;
	d = new_وڻ_content(s->داخلا_ڳڻپ);
	لكل (i = 0; i < s->داخلا_ڳڻپ; i++) {
		a = s->entries[i];
		b = new_وڻ_داخلا();
		يادگيري_نقل(b, a, حجم(*a));
		إذا (a->وڻ && is_null_oid(&b->versions[1].oid))
			b->وڻ = dup_وڻ_content(a->وڻ);
		وإلا
			b->وڻ = عدم;
		d->entries[i] = b;
	}
	d->داخلا_ڳڻپ = s->داخلا_ڳڻپ;
	d->delta_اونهائي = s->delta_اونهائي;

	أرجع d;
}

ساكن فراغ شروعات_packfile(فراغ)
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
	pack_سائيز = write_pack_سرو(pack_file, 0);
	object_ڳڻپ = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

ساكن ثابت حرف *create_اشاريو(فراغ)
{
	ثابت حرف *tmpfile;
	بنية pack_idx_داخلا **idx, **c, **آخري;
	بنية object_داخلا *e;
	بنية object_داخلا_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_ڳڻپ);
	c = idx;
	لكل (o = بلاڪون; o; o = o->ايندڙ_pool)
		لكل (e = o->ايندڙ_free; e-- != o->entries;)
			إذا (pack_id == e->pack_id)
				*c++ = &e->idx;
	آخري = idx + object_ڳڻپ;
	إذا (c != آخري)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, عدم, idx, object_ڳڻپ,
				 &pack_idx_opts, pack_معلومات->hash);
	حرر(idx);
	أرجع tmpfile;
}

ساكن حرف *keep_pack(ثابت حرف *curr_اشاريو_name)
{
	ساكن ثابت حرف *keep_نياپو = "fast-import";
	بنية strbuf name = STRBUF_INIT;
	صحيح keep_fd;

	odb_pack_name(pack_معلومات->repo, &name, pack_معلومات->hash, "keep");
	keep_fd = محفوظ_create_file_with_leading_directories(pack_معلومات->repo,
							    name.buf);
	إذا (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_نياپو, لڙي_ڊيگهه(keep_نياپو));
	إذا (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_معلومات->repo, &name, pack_معلومات->hash, "pack");
	إذا (finalize_object_file(pack_معلومات->repo, pack_معلومات->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_معلومات->repo, &name, pack_معلومات->hash, "idx");
	إذا (finalize_object_file(pack_معلومات->repo, curr_اشاريو_name, name.buf))
		die(_("cannot store index file"));
	حرر((فراغ *)curr_اشاريو_name);
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
	بنية ٻار_process unpack = ٻار_PROCESS_INIT;

	إذا (lseek(p->pack_fd, 0, SEEK_سيٽ) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.اندريون = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_منزل_stderr = 1;
	strvec_ڌڪو(&unpack.دليلون, "unpack-objects");
	إذا (!show_stats)
		strvec_ڌڪو(&unpack.دليلون, "-q");

	أرجع لاڳيتو_command(&unpack);
}

ساكن فراغ پڇاڙي_packfile(فراغ)
{
	ساكن صحيح running;

	إذا (running || !pack_معلومات)
		أرجع;

	running = 1;
	clear_delta_بنياد_cache();
	إذا (object_ڳڻپ) {
		بنية odb_ذريعو_files *files = odb_ذريعو_files_downcast(pack_معلومات->repo->objects->ذريعو);
		بنية packed_git *new_p;
		بنية object_id cur_pack_oid;
		حرف *idx_name;
		صحيح i;
		بنية branch *b;
		بنية ٽيگ *t;

		close_pack_windows(pack_معلومات);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_سرو_footer(the_hash_algo, pack_معلومات->pack_fd,
					 pack_معلومات->hash, pack_معلومات->pack_name,
					 object_ڳڻپ, cur_pack_oid.hash,
					 pack_سائيز);

		إذا (object_ڳڻپ <= unpack_حد_بندي) {
			إذا (!loosen_small_pack(pack_معلومات)) {
				invalidate_pack_id(pack_id);
				اذهب discard_pack;
			}
		}

		close(pack_معلومات->pack_fd);
		idx_name = keep_pack(create_اشاريو());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_ڀريو_pack(files->dirs->packed, idx_name, 1);
		إذا (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		حرر(idx_name);

		/* Print the boundary */
		إذا (pack_ڪنارا) {
			fprintf(pack_ڪنارا, "%s:", new_p->pack_name);
			لكل (i = 0; i < branch_جدول_sz; i++) {
				لكل (b = branch_جدول[i]; b; b = b->جدول_ايندڙ_branch) {
					إذا (b->pack_id == pack_id)
						fprintf(pack_ڪنارا, " %s",
							oid_منزل_hex(&b->oid));
				}
			}
			لكل (t = پهريون_ٽيگ; t; t = t->ايندڙ_ٽيگ) {
				إذا (t->pack_id == pack_id)
					fprintf(pack_ڪنارا, " %s",
						oid_منزل_hex(&t->oid));
			}
			fputc('\n', pack_ڪنارا);
			fflush(pack_ڪنارا);
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
	strbuf_release(&آخري_blob.معلومات);
	آخري_blob.وٿي = 0;
	آخري_blob.اونهائي = 0;
}

ساكن فراغ چڪر_packfile(فراغ)
{
	پڇاڙي_packfile();
	شروعات_packfile();
}

ساكن صحيح store_object(
	تعداد object_type type,
	بنية strbuf *dat,
	بنية آخري_object *آخري,
	بنية object_id *oidout,
	uintmax_t mark)
{
	بنية odb_ذريعو_files *files = odb_ذريعو_files_downcast(the_repository->objects->ذريعو);
	فراغ *ٻاهريون, *delta;
	بنية object_داخلا *e;
	بنانشان حرف hdr[96];
	بنية object_id oid;
	بنانشان ڊگهو hdrlen, deltalen = 0;
	بنية git_hash_ctx c;
	git_zstream s;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	hdrlen = format_object_سرو((حرف *)hdr, حجم(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	إذا (oidout)
		oidcpy(oidout, &oid);

	e = وجهو_object(&oid);
	إذا (mark)
		وجهو_mark(&marks, mark, e);
	إذا (e->idx.وٿي) {
		duplicate_ڳڻپ_by_type[type]++;
		أرجع 1;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->ايندڙ) {
		إذا (!packfile_list_ڳوليو_oid(packfile_store_get_packs(dir->packed), &oid))
			استمر;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.وٿي = 1; /* just not zero! */
		duplicate_ڳڻپ_by_type[type]++;
		أرجع 1;
	}

	إذا (آخري && آخري->معلومات.len && آخري->معلومات.buf && آخري->اونهائي < max_اونهائي
		&& dat->len > the_hash_algo->rawsz) {
		سائيز_t deltalen_st;

		delta_ڳڻپ_attempts_by_type[type]++;
		delta = diff_delta(آخري->معلومات.buf, آخري->معلومات.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_سائيز_t_منزل_ulong(deltalen_st);
	} وإلا
		delta = عدم;

	git_deflate_init(&s, cfg->pack_compression_level);
	إذا (delta) {
		s.ايندڙ_اندريون = delta;
		s.avail_اندريون = deltalen;
	} وإلا {
		s.ايندڙ_اندريون = (فراغ *)dat->buf;
		s.avail_اندريون = dat->len;
	}
	s.avail_ٻاهريون = git_deflate_bound(&s, s.avail_اندريون);
	s.ايندڙ_ٻاهريون = ٻاهريون = xmalloc(s.avail_ٻاهريون);
	طالما (git_deflate(&s, Z_پڄاڻي) == Z_OK)
		; /* nothing */
	git_deflate_پڇاڙي(&s);

	/* Determine if we should auto-checkpoint. */
	إذا ((max_packsize
		&& (pack_سائيز + PACK_سائيز_حد + s.ڪل_ٻاهريون) > max_packsize)
		|| (pack_سائيز + PACK_سائيز_حد + s.ڪل_ٻاهريون) < pack_سائيز) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		چڪر_packfile();

		/* We cannot carry a delta into the new pack. */
		إذا (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.ايندڙ_اندريون = (فراغ *)dat->buf;
			s.avail_اندريون = dat->len;
			s.avail_ٻاهريون = git_deflate_bound(&s, s.avail_اندريون);
			s.ايندڙ_ٻاهريون = ٻاهريون = xrealloc(ٻاهريون, s.avail_ٻاهريون);
			طالما (git_deflate(&s, Z_پڄاڻي) == Z_OK)
				; /* nothing */
			git_deflate_پڇاڙي(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.وٿي = pack_سائيز;
	object_ڳڻپ++;
	object_ڳڻپ_by_type[type]++;

	چڪري_جاچ_32_begin(pack_file);

	إذا (delta) {
		off_t ofs = e->idx.وٿي - آخري->وٿي;
		بنانشان pos = حجم(hdr) - 1;

		delta_ڳڻپ_by_type[type]++;
		e->اونهائي = آخري->اونهائي + 1;

		hdrlen = encode_اندريون_pack_object_سرو(hdr, حجم(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_سائيز += hdrlen;

		hdr[pos] = ofs & 127;
		طالما (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, حجم(hdr) - pos);
		pack_سائيز += حجم(hdr) - pos;
	} وإلا {
		e->اونهائي = 0;
		hdrlen = encode_اندريون_pack_object_سرو(hdr, حجم(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_سائيز += hdrlen;
	}

	hashwrite(pack_file, ٻاهريون, s.ڪل_ٻاهريون);
	pack_سائيز += s.ڪل_ٻاهريون;

	e->idx.چڪري_جاچ_32 = چڪري_جاچ_32_پڇاڙي(pack_file);

	حرر(ٻاهريون);
	حرر(delta);
	إذا (آخري) {
		إذا (آخري->no_مٽا_سٽا) {
			آخري->معلومات = *dat;
		} وإلا {
			strbuf_مٽا_سٽا(&آخري->معلومات, dat);
		}
		آخري->وٿي = e->idx.وٿي;
		آخري->اونهائي = e->اونهائي;
	}
	أرجع 0;
}

ساكن فراغ truncate_pack(بنية hashfile_checkpoint *checkpoint)
{
	إذا (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_سائيز = checkpoint->وٿي;
}

ساكن فراغ وهڪرو_blob(uintmax_t len, بنية object_id *oidout, uintmax_t mark)
{
	بنية odb_ذريعو_files *files = odb_ذريعو_files_downcast(the_repository->objects->ذريعو);
	سائيز_t اندريون_sz = 64 * 1024, ٻاهريون_sz = 64 * 1024;
	بنانشان حرف *اندريون_buf = xmalloc(اندريون_sz);
	بنانشان حرف *ٻاهريون_buf = xmalloc(ٻاهريون_sz);
	بنية object_داخلا *e;
	بنية object_id oid;
	بنانشان ڊگهو hdrlen;
	off_t وٿي;
	بنية git_hash_ctx c;
	git_zstream s;
	بنية hashfile_checkpoint checkpoint;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);
	صحيح حالت = Z_OK;

	/* Determine if we should auto-checkpoint. */
	إذا ((max_packsize
		&& (pack_سائيز + PACK_سائيز_حد + len) > max_packsize)
		|| (pack_سائيز + PACK_سائيز_حد + len) < pack_سائيز)
		چڪر_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	وٿي = checkpoint.وٿي;

	hdrlen = format_object_سرو((حرف *)ٻاهريون_buf, ٻاهريون_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, ٻاهريون_buf, hdrlen);

	چڪري_جاچ_32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_اندريون_pack_object_سرو(ٻاهريون_buf, ٻاهريون_sz, OBJ_BLOB, len);

	s.ايندڙ_ٻاهريون = ٻاهريون_buf + hdrlen;
	s.avail_ٻاهريون = ٻاهريون_sz - hdrlen;

	طالما (حالت != Z_وهڪرو_پڇاڙي) {
		إذا (0 < len && !s.avail_اندريون) {
			سائيز_t cnt = اندريون_sz < len ? اندريون_sz : (سائيز_t)len;
			سائيز_t n = fread(اندريون_buf, 1, cnt, stdin);
			إذا (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, اندريون_buf, n);
			s.ايندڙ_اندريون = اندريون_buf;
			s.avail_اندريون = n;
			len -= n;
		}

		حالت = git_deflate(&s, len ? 0 : Z_پڄاڻي);

		إذا (!s.avail_ٻاهريون || حالت == Z_وهڪرو_پڇاڙي) {
			سائيز_t n = s.ايندڙ_ٻاهريون - ٻاهريون_buf;
			hashwrite(pack_file, ٻاهريون_buf, n);
			pack_سائيز += n;
			s.ايندڙ_ٻاهريون = ٻاهريون_buf;
			s.avail_ٻاهريون = ٻاهريون_sz;
		}

		بدل (حالت) {
		حالة Z_OK:
		حالة Z_BUF_ERROR:
		حالة Z_وهڪرو_پڇاڙي:
			استمر;
		افتراضي:
			die(_("unexpected deflate failure: %d"), حالت);
		}
	}
	git_deflate_پڇاڙي(&s);
	git_hash_final_oid(&oid, &c);

	إذا (oidout)
		oidcpy(oidout, &oid);

	e = وجهو_object(&oid);

	إذا (mark)
		وجهو_mark(&marks, mark, e);

	إذا (e->idx.وٿي) {
		duplicate_ڳڻپ_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		اذهب ٻاهريون;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->ايندڙ) {
		إذا (!packfile_list_ڳوليو_oid(packfile_store_get_packs(dir->packed), &oid))
			استمر;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.وٿي = 1; /* just not zero! */
		duplicate_ڳڻپ_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		اذهب ٻاهريون;
	}

	e->اونهائي = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.وٿي = وٿي;
	e->idx.چڪري_جاچ_32 = چڪري_جاچ_32_پڇاڙي(pack_file);
	object_ڳڻپ++;
	object_ڳڻپ_by_type[OBJ_BLOB]++;

ٻاهريون:
	حرر(اندريون_buf);
	حرر(ٻاهريون_buf);
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
ساكن فراغ *gfi_unpack_داخلا(
	بنية object_داخلا *oe,
	بنانشان ڊگهو *sizep)
{
	تعداد object_type type;
	سائيز_t سائيز_st = 0;
	فراغ *معلومات;
	بنية packed_git *p = all_packs[oe->pack_id];
	إذا (p == pack_معلومات && p->pack_سائيز < (pack_سائيز + the_hash_algo->rawsz)) {
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
		p->pack_سائيز = pack_سائيز + the_hash_algo->rawsz;
	}
	معلومات = unpack_داخلا(the_repository, p, oe->idx.وٿي, &type, &سائيز_st);
	إذا (sizep)
		*sizep = cast_سائيز_t_منزل_ulong(سائيز_st);
	أرجع معلومات;
}

ساكن فراغ ڀريو_وڻ(بنية وڻ_داخلا *پاڙ)
{
	بنية object_id *oid = &پاڙ->versions[1].oid;
	بنية object_داخلا *myoe;
	بنية وڻ_content *t;
	بنانشان ڊگهو سائيز;
	حرف *buf;
	ثابت حرف *c;

	پاڙ->وڻ = t = new_وڻ_content(8);
	إذا (is_null_oid(oid))
		أرجع;

	myoe = ڳوليو_object(oid);
	إذا (myoe && myoe->pack_id != MAX_PACK_ID) {
		إذا (myoe->type != OBJ_وڻ)
			die(_("not a tree: %s"), oid_منزل_hex(oid));
		t->delta_اونهائي = myoe->اونهائي;
		buf = gfi_unpack_داخلا(myoe, &سائيز);
		إذا (!buf)
			die(_("can't load tree %s"), oid_منزل_hex(oid));
	} وإلا {
		تعداد object_type type;
		سائيز_t سائيز_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &سائيز_st);
		سائيز = cast_سائيز_t_منزل_ulong(سائيز_st);
		إذا (!buf || type != OBJ_وڻ)
			die(_("can't load tree %s"), oid_منزل_hex(oid));
	}

	c = buf;
	طالما (c != (buf + سائيز)) {
		بنية وڻ_داخلا *e = new_وڻ_داخلا();

		إذا (t->داخلا_ڳڻپ == t->داخلا_صلاحيت)
			پاڙ->وڻ = t = grow_وڻ_content(t, t->داخلا_ڳڻپ);
		t->entries[t->داخلا_ڳڻپ++] = e;

		e->وڻ = عدم;
		c = parse_گهڻائي(c, &e->versions[1].گهڻائي);
		إذا (!c)
			die(_("corrupt mode in %s"), oid_منزل_hex(oid));
		e->versions[0].گهڻائي = e->versions[1].گهڻائي;
		e->name = منزل_atom(c, لڙي_ڊيگهه(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (بنانشان حرف *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (بنانشان حرف *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	حرر(buf);
}

ساكن صحيح tecmp0 (ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية وڻ_داخلا *a = *((بنية وڻ_داخلا**)_a);
	بنية وڻ_داخلا *b = *((بنية وڻ_داخلا**)_b);
	أرجع بنياد_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].گهڻائي,
		b->name->str_dat, b->name->str_len, b->versions[0].گهڻائي);
}

ساكن صحيح tecmp1 (ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية وڻ_داخلا *a = *((بنية وڻ_داخلا**)_a);
	بنية وڻ_داخلا *b = *((بنية وڻ_داخلا**)_b);
	أرجع بنياد_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].گهڻائي,
		b->name->str_dat, b->name->str_len, b->versions[1].گهڻائي);
}

ساكن فراغ mktree(بنية وڻ_content *t, صحيح v, بنية strbuf *b)
{
	سائيز_t maxlen = 0;
	بنانشان صحيح i;

	إذا (!v)
		QSORT(t->entries, t->داخلا_ڳڻپ, tecmp0);
	وإلا
		QSORT(t->entries, t->داخلا_ڳڻپ, tecmp1);

	لكل (i = 0; i < t->داخلا_ڳڻپ; i++) {
		إذا (t->entries[i]->versions[v].گهڻائي)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	لكل (i = 0; i < t->داخلا_ڳڻپ; i++) {
		بنية وڻ_داخلا *e = t->entries[i];
		إذا (!e->versions[v].گهڻائي)
			استمر;
		strbuf_addf(b, "%o %s%c",
			(بنانشان صحيح)(e->versions[v].گهڻائي & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_جوڙيو(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

ساكن فراغ store_وڻ(بنية وڻ_داخلا *پاڙ)
{
	بنية وڻ_content *t;
	بنانشان صحيح i, j, del;
	بنية آخري_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	بنية object_داخلا *le = عدم;

	إذا (!is_null_oid(&پاڙ->versions[1].oid))
		أرجع;

	إذا (!پاڙ->وڻ)
		ڀريو_وڻ(پاڙ);
	t = پاڙ->وڻ;

	لكل (i = 0; i < t->داخلا_ڳڻپ; i++) {
		إذا (t->entries[i]->وڻ)
			store_وڻ(t->entries[i]);
	}

	إذا (!(پاڙ->versions[0].گهڻائي & NO_DELTA))
		le = ڳوليو_object(&پاڙ->versions[0].oid);
	إذا (S_ISDIR(پاڙ->versions[0].گهڻائي) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_وڻ);
		lo.معلومات = old_وڻ;
		lo.وٿي = le->idx.وٿي;
		lo.اونهائي = t->delta_اونهائي;
	}

	mktree(t, 1, &new_وڻ);
	store_object(OBJ_وڻ, &new_وڻ, &lo, &پاڙ->versions[1].oid, 0);

	t->delta_اونهائي = lo.اونهائي;
	لكل (i = 0, j = 0, del = 0; i < t->داخلا_ڳڻپ; i++) {
		بنية وڻ_داخلا *e = t->entries[i];
		إذا (e->versions[1].گهڻائي) {
			e->versions[0].گهڻائي = e->versions[1].گهڻائي;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} وإلا {
			release_وڻ_داخلا(e);
			del++;
		}
	}
	t->داخلا_ڳڻپ -= del;
}

ساكن فراغ وڻ_content_replace(
	بنية وڻ_داخلا *پاڙ,
	ثابت بنية object_id *oid,
	ثابت uint16_t گهڻائي,
	بنية وڻ_content *newtree)
{
	إذا (!S_ISDIR(گهڻائي))
		die(_("root cannot be a non-directory"));
	oidclr(&پاڙ->versions[0].oid, the_repository->hash_algo);
	oidcpy(&پاڙ->versions[1].oid, oid);
	إذا (پاڙ->وڻ)
		release_وڻ_content_recursive(پاڙ->وڻ);
	پاڙ->وڻ = newtree;
}

ساكن صحيح وڻ_content_سيٽ(
	بنية وڻ_داخلا *پاڙ,
	ثابت حرف *p,
	ثابت بنية object_id *oid,
	ثابت uint16_t گهڻائي,
	بنية وڻ_content *subtree)
{
	بنية وڻ_content *t;
	ثابت حرف *slash1;
	بنانشان صحيح i, n;
	بنية وڻ_داخلا *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	إذا (!n)
		die(_("empty path component found in input"));
	إذا (!*slash1 && !S_ISDIR(گهڻائي) && subtree)
		die(_("non-directories cannot have subtrees"));

	إذا (!پاڙ->وڻ)
		ڀريو_وڻ(پاڙ);
	t = پاڙ->وڻ;
	لكل (i = 0; i < t->داخلا_ڳڻپ; i++) {
		e = t->entries[i];
		إذا (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			إذا (!*slash1) {
				إذا (!S_ISDIR(گهڻائي)
						&& e->versions[1].گهڻائي == گهڻائي
						&& oideq(&e->versions[1].oid, oid))
					أرجع 0;
				e->versions[1].گهڻائي = گهڻائي;
				oidcpy(&e->versions[1].oid, oid);
				إذا (e->وڻ)
					release_وڻ_content_recursive(e->وڻ);
				e->وڻ = subtree;

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
				إذا (S_ISDIR(e->versions[0].گهڻائي))
					e->versions[0].گهڻائي |= NO_DELTA;

				oidclr(&پاڙ->versions[1].oid, the_repository->hash_algo);
				أرجع 1;
			}
			إذا (!S_ISDIR(e->versions[1].گهڻائي)) {
				e->وڻ = new_وڻ_content(8);
				e->versions[1].گهڻائي = S_IFDIR;
			}
			إذا (!e->وڻ)
				ڀريو_وڻ(e);
			إذا (وڻ_content_سيٽ(e, slash1 + 1, oid, گهڻائي, subtree)) {
				oidclr(&پاڙ->versions[1].oid, the_repository->hash_algo);
				أرجع 1;
			}
			أرجع 0;
		}
	}

	إذا (t->داخلا_ڳڻپ == t->داخلا_صلاحيت)
		پاڙ->وڻ = t = grow_وڻ_content(t, t->داخلا_ڳڻپ);
	e = new_وڻ_داخلا();
	e->name = منزل_atom(p, n);
	e->versions[0].گهڻائي = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->داخلا_ڳڻپ++] = e;
	إذا (*slash1) {
		e->وڻ = new_وڻ_content(8);
		e->versions[1].گهڻائي = S_IFDIR;
		وڻ_content_سيٽ(e, slash1 + 1, oid, گهڻائي, subtree);
	} وإلا {
		e->وڻ = subtree;
		e->versions[1].گهڻائي = گهڻائي;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&پاڙ->versions[1].oid, the_repository->hash_algo);
	أرجع 1;
}

ساكن صحيح وڻ_content_remove(
	بنية وڻ_داخلا *پاڙ,
	ثابت حرف *p,
	بنية وڻ_داخلا *backup_پن,
	صحيح allow_پاڙ)
{
	بنية وڻ_content *t;
	ثابت حرف *slash1;
	بنانشان صحيح i, n;
	بنية وڻ_داخلا *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	إذا (!پاڙ->وڻ)
		ڀريو_وڻ(پاڙ);

	إذا (!*p && allow_پاڙ) {
		e = پاڙ;
		اذهب del_داخلا;
	}

	t = پاڙ->وڻ;
	لكل (i = 0; i < t->داخلا_ڳڻپ; i++) {
		e = t->entries[i];
		إذا (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			إذا (*slash1 && !S_ISDIR(e->versions[1].گهڻائي))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				أرجع 1;
			إذا (!*slash1 || !S_ISDIR(e->versions[1].گهڻائي))
				اذهب del_داخلا;
			إذا (!e->وڻ)
				ڀريو_وڻ(e);
			إذا (وڻ_content_remove(e, slash1 + 1, backup_پن, 0)) {
				لكل (n = 0; n < e->وڻ->داخلا_ڳڻپ; n++) {
					إذا (e->وڻ->entries[n]->versions[1].گهڻائي) {
						oidclr(&پاڙ->versions[1].oid,
						       the_repository->hash_algo);
						أرجع 1;
					}
				}
				backup_پن = عدم;
				اذهب del_داخلا;
			}
			أرجع 0;
		}
	}
	أرجع 0;

del_داخلا:
	إذا (backup_پن)
		يادگيري_نقل(backup_پن, e, حجم(*backup_پن));
	وإلا إذا (e->وڻ)
		release_وڻ_content_recursive(e->وڻ);
	e->وڻ = عدم;
	e->versions[1].گهڻائي = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&پاڙ->versions[1].oid, the_repository->hash_algo);
	أرجع 1;
}

ساكن صحيح وڻ_content_get(
	بنية وڻ_داخلا *پاڙ,
	ثابت حرف *p,
	بنية وڻ_داخلا *پن,
	صحيح allow_پاڙ)
{
	بنية وڻ_content *t;
	ثابت حرف *slash1;
	بنانشان صحيح i, n;
	بنية وڻ_داخلا *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	إذا (!n && !allow_پاڙ)
		die(_("empty path component found in input"));

	إذا (!پاڙ->وڻ)
		ڀريو_وڻ(پاڙ);

	إذا (!n) {
		e = پاڙ;
		اذهب مليو_داخلا;
	}

	t = پاڙ->وڻ;
	لكل (i = 0; i < t->داخلا_ڳڻپ; i++) {
		e = t->entries[i];
		إذا (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			إذا (!*slash1)
				اذهب مليو_داخلا;
			إذا (!S_ISDIR(e->versions[1].گهڻائي))
				أرجع 0;
			إذا (!e->وڻ)
				ڀريو_وڻ(e);
			أرجع وڻ_content_get(e, slash1 + 1, پن, 0);
		}
	}
	أرجع 0;

مليو_داخلا:
	يادگيري_نقل(پن, e, حجم(*پن));
	إذا (e->وڻ && is_null_oid(&e->versions[1].oid))
		پن->وڻ = dup_وڻ_content(e->وڻ);
	وإلا
		پن->وڻ = عدم;
	أرجع 1;
}

ساكن صحيح update_branch(بنية branch *b)
{
	ساكن ثابت حرف *نياپو = "fast-import";
	بنية ref_transaction *transaction;
	بنية object_id old_oid;
	بنية strbuf err = STRBUF_INIT;
	ساكن ثابت حرف *replace_اڳياڙي = "refs/replace/";

	إذا (starts_with(b->name, replace_اڳياڙي) &&
	    !strcmp(b->name + لڙي_ڊيگهه(replace_اڳياڙي),
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

		ret = repo_اندريون_ملايو_bases(the_repository, old_cmit, new_cmit);
		إذا (ret < 0)
			ٻاهر_نڪرو(128);
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
				   عدم, عدم, 0, نياپو, &err) ||
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
	بنانشان صحيح i;
	بنية branch *b;

	لكل (i = 0; i < branch_جدول_sz; i++) {
		لكل (b = branch_جدول[i]; b; b = b->جدول_ايندڙ_branch)
			failure |= update_branch(b);
	}
}

ساكن فراغ dump_ٽيگ_فهرست(فراغ)
{
	ساكن ثابت حرف *نياپو = "fast-import";
	بنية ٽيگ *t;
	بنية strbuf ref_name = STRBUF_INIT;
	بنية strbuf err = STRBUF_INIT;
	بنية ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	إذا (!transaction) {
		failure |= error("%s", err.buf);
		اذهب cleanup;
	}
	لكل (t = پهريون_ٽيگ; t; t = t->ايندڙ_ٽيگ) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		إذا (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, عدم, عدم, عدم,
					   0, نياپو, &err)) {
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
	بنية تالو_file mark_تالو = تالو_INIT;
	FILE *f;

	إذا (!export_marks_file || (import_marks_file && !import_marks_file_پورو))
		أرجع;

	إذا (محفوظ_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		أرجع;
	}

	إذا (hold_تالو_file_for_update(&mark_تالو, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		أرجع;
	}

	f = fdopen_تالو_file(&mark_تالو, "w");
	إذا (!f) {
		صحيح saved_errno = errno;
		rollback_تالو_file(&mark_تالو);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		أرجع;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	إذا (commit_تالو_file(&mark_تالو)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		أرجع;
	}
}

ساكن فراغ وجهو_object_داخلا(بنية mark_سيٽ **s, بنية object_id *oid, uintmax_t mark)
{
	بنية object_داخلا *e;
	e = ڳوليو_object(oid);
	إذا (!e) {
		تعداد object_type type = odb_read_object_info(the_repository->objects,
							     oid, عدم);
		إذا (type < 0)
			die(_("object not found: %s"), oid_منزل_hex(oid));
		e = وجهو_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.وٿي = 1; /* just not zero! */
	}
	وجهو_mark(s, mark, e);
}

ساكن فراغ وجهو_oid_داخلا(بنية mark_سيٽ **s, بنية object_id *oid, uintmax_t mark)
{
	وجهو_mark(s, mark, xmemdupz(oid, حجم(*oid)));
}

ساكن فراغ read_mark_file(بنية mark_سيٽ **s, FILE *f, mark_سيٽ_inserter_t inserter)
{
	حرف سٽ[512];
	طالما (fgets(سٽ, حجم(سٽ), f)) {
		uintmax_t mark;
		حرف *پڇاڙي;
		بنية object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, حجم(oid.hash));

		پڇاڙي = strchr(سٽ, '\n');
		إذا (سٽ[0] != ':' || !پڇاڙي)
			die(_("corrupt mark line: %s"), سٽ);
		*پڇاڙي = 0;
		mark = strtoumax(سٽ + 1, &پڇاڙي, 10);
		إذا (!mark || پڇاڙي == سٽ + 1
			|| *پڇاڙي != ' '
			|| get_oid_hex_any(پڇاڙي + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), سٽ);
		inserter(s, &oid, mark);
	}
}

ساكن فراغ read_marks(فراغ)
{
	FILE *f = fopen(import_marks_file, "r");
	إذا (f)
		;
	وإلا إذا (import_marks_file_ignore_missing && errno == ENOENT)
		اذهب پورو; /* Marks file does not exist */
	وإلا
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, وجهو_object_داخلا);
	fclose(f);
پورو:
	import_marks_file_پورو = 1;
}


ساكن صحيح read_ايندڙ_command(بنية fast_import_state *state)
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

			إذا (!state->ڏٺل_معلومات_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			إذا (rc)
				rc_free = rc->ايندڙ;
			وإلا {
				rc = cmd_hist.ايندڙ;
				cmd_hist.ايندڙ = rc->ايندڙ;
				cmd_hist.ايندڙ->prev = &cmd_hist;
				حرر(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_پڇ;
			rc->ايندڙ = cmd_hist.prev;
			rc->prev->ايندڙ = rc;
			cmd_پڇ = rc;
		}
		إذا (command_buf.buf[0] == '#')
			استمر;
		أرجع 0;
	}
}

ساكن فراغ skip_optional_lf(فراغ)
{
	صحيح رڪن_char = fgetc(stdin);
	إذا (رڪن_char != '\n' && رڪن_char != EOF)
		ungetc(رڪن_char, stdin);
}

ساكن فراغ parse_mark(بنية fast_import_state *state)
{
	ثابت حرف *v;
	إذا (skip_اڳياڙي(command_buf.buf, "mark :", &v)) {
		ايندڙ_mark = strtoumax(v, عدم, 10);
		read_ايندڙ_command(state);
	}
	وإلا
		ايندڙ_mark = 0;
}

ساكن فراغ parse_original_identifier(بنية fast_import_state *state)
{
	ثابت حرف *v;
	إذا (skip_اڳياڙي(command_buf.buf, "original-oid ", &v))
		read_ايندڙ_command(state);
}

ساكن صحيح parse_معلومات(بنية strbuf *sb, uintmax_t حد_بندي, uintmax_t *len_res)
{
	ثابت حرف *معلومات;
	strbuf_reset(sb);

	إذا (!skip_اڳياڙي(command_buf.buf, "data ", &معلومات))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	إذا (skip_اڳياڙي(معلومات, "<<", &معلومات)) {
		حرف *رڪن = xstrdup(معلومات);
		سائيز_t رڪن_len = command_buf.len - (معلومات - command_buf.buf);

		لكل (;;) {
			إذا (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), رڪن);
			إذا (رڪن_len == command_buf.len
				&& !strcmp(رڪن, command_buf.buf))
				اكسر;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		حرر(رڪن);
	}
	وإلا {
		uintmax_t len = strtoumax(معلومات, عدم, 10);
		سائيز_t n = 0, ڊيگهائي = (سائيز_t)len;

		إذا (حد_بندي && حد_بندي < len) {
			*len_res = len;
			أرجع 0;
		}
		إذا (ڊيگهائي < len)
			die(_("data is too large to use in this context"));

		طالما (n < ڊيگهائي) {
			سائيز_t s = strbuf_fread(sb, ڊيگهائي - n, stdin);
			إذا (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(بنانشان ڊگهو)(ڊيگهائي - n));
			n += s;
		}
	}

	skip_optional_lf();
	أرجع 1;
}

ساكن صحيح validate_raw_date(ثابت حرف *src, بنية strbuf *نتيجو, صحيح strict)
{
	ثابت حرف *orig_src = src;
	حرف *endp;
	بنانشان ڊگهو انگ;

	errno = 0;

	انگ = strtoul(src, &endp, 10);
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

	انگ = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	إذا (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < انگ))             /* parsed a broken timezone */
	   )
		أرجع -1;

	strbuf_addstr(نتيجو, orig_src);
	أرجع 0;
}

ساكن حرف *parse_ident(ثابت حرف *buf)
{
	ثابت حرف *ltgt;
	سائيز_t name_len;
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
	strbuf_جوڙيو(&ident, buf, name_len);

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
	بنية آخري_object *آخري,
	بنية object_id *oidout,
	uintmax_t mark)
{
	ساكن بنية strbuf buf = STRBUF_INIT;
	uintmax_t len;

	إذا (parse_معلومات(&buf, repo_settings_get_big_file_حد(the_repository), &len))
		store_object(OBJ_BLOB, &buf, آخري, oidout, mark);
	وإلا {
		إذا (آخري) {
			strbuf_release(&آخري->معلومات);
			آخري->وٿي = 0;
			آخري->اونهائي = 0;
		}
		وهڪرو_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

ساكن فراغ parse_new_blob(بنية fast_import_state *state)
{
	read_ايندڙ_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&آخري_blob, عدم, ايندڙ_mark);
}

ساكن فراغ unload_one_branch(فراغ)
{
	طالما (cur_سرگرم_branches
		&& cur_سرگرم_branches >= max_سرگرم_branches) {
		uintmax_t min_commit = ULONG_MAX;
		بنية branch *e, *l = عدم, *p = عدم;

		لكل (e = سرگرم_branches; e; e = e->سرگرم_ايندڙ_branch) {
			إذا (e->آخري_commit < min_commit) {
				p = l;
				min_commit = e->آخري_commit;
			}
			l = e;
		}

		إذا (p) {
			e = p->سرگرم_ايندڙ_branch;
			p->سرگرم_ايندڙ_branch = e->سرگرم_ايندڙ_branch;
		} وإلا {
			e = سرگرم_branches;
			سرگرم_branches = e->سرگرم_ايندڙ_branch;
		}
		e->سرگرم = 0;
		e->سرگرم_ايندڙ_branch = عدم;
		إذا (e->branch_وڻ.وڻ) {
			release_وڻ_content_recursive(e->branch_وڻ.وڻ);
			e->branch_وڻ.وڻ = عدم;
		}
		cur_سرگرم_branches--;
	}
}

ساكن فراغ ڀريو_branch(بنية branch *b)
{
	ڀريو_وڻ(&b->branch_وڻ);
	إذا (!b->سرگرم) {
		b->سرگرم = 1;
		b->سرگرم_ايندڙ_branch = سرگرم_branches;
		سرگرم_branches = b;
		cur_سرگرم_branches++;
		branch_ڀريو_ڳڻپ++;
	}
}

ساكن بنانشان حرف convert_انگ_notes_منزل_fanout(uintmax_t انگ_notes)
{
	بنانشان حرف fanout = 0;
	طالما ((انگ_notes >>= 8))
		fanout++;
	أرجع fanout;
}

ساكن فراغ construct_رستو_with_fanout(ثابت حرف *hex_sha1,
		بنانشان حرف fanout, حرف *رستو)
{
	بنانشان صحيح i = 0, j = 0;
	إذا (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	طالما (fanout) {
		رستو[i++] = hex_sha1[j++];
		رستو[i++] = hex_sha1[j++];
		رستو[i++] = '/';
		fanout--;
	}
	يادگيري_نقل(رستو + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	رستو[i + the_hash_algo->hexsz - j] = '\0';
}

ساكن uintmax_t do_change_note_fanout(
		بنية وڻ_داخلا *orig_پاڙ, بنية وڻ_داخلا *پاڙ,
		حرف *hex_oid, بنانشان صحيح hex_oid_len,
		حرف *fullpath, بنانشان صحيح fullpath_len,
		بنانشان حرف fanout)
{
	بنية وڻ_content *t;
	بنية وڻ_داخلا *e, پن;
	بنانشان صحيح i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t انگ_notes = 0;
	بنية object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	حرف realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ثابت بنانشان hexsz = the_hash_algo->hexsz;

	إذا (!پاڙ->وڻ)
		ڀريو_وڻ(پاڙ);
	t = پاڙ->وڻ;

	لكل (i = 0; t && i < t->داخلا_ڳڻپ; i++) {
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
		إذا (!e->versions[1].گهڻائي ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			استمر;

		/* This _may_ be a note entry, or a subdir containing notes */
		يادگيري_نقل(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		إذا (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		يادگيري_نقل(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		إذا (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			إذا (fanout == 0xff) {
				/* Counting mode, no rename */
				انگ_notes++;
				استمر;
			}
			construct_رستو_with_fanout(hex_oid, fanout, realpath);
			إذا (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				انگ_notes++;
				استمر;
			}

			/* Rename fullpath to realpath */
			إذا (!وڻ_content_remove(orig_پاڙ, fullpath, &پن, 0))
				die(_("failed to remove path %s"), fullpath);
			وڻ_content_سيٽ(orig_پاڙ, realpath,
				&پن.versions[1].oid,
				پن.versions[1].گهڻائي,
				پن.وڻ);
		} وإلا إذا (S_ISDIR(e->versions[1].گهڻائي)) {
			/* This is a subdir that may contain note entries */
			انگ_notes += do_change_note_fanout(orig_پاڙ, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = پاڙ->وڻ;
	}
	أرجع انگ_notes;
}

ساكن uintmax_t change_note_fanout(بنية وڻ_داخلا *پاڙ,
		بنانشان حرف fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	حرف hex_oid[GIT_MAX_HEXSZ], رستو[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	أرجع do_change_note_fanout(پاڙ, پاڙ, hex_oid, 0, رستو, 0, fanout);
}

ساكن صحيح parse_mapped_oid_hex(ثابت حرف *hex, بنية object_id *oid, ثابت حرف **پڇاڙي)
{
	صحيح algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, حجم(oid->hash));

	algo = parse_oid_hex_any(hex, oid, پڇاڙي);
	إذا (algo == GIT_HASH_UNKNOWN)
		أرجع -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	إذا (it == kh_پڇاڙي(sub_oid_map)) {
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
	حرف *پڇاڙي;
	uintmax_t mark;

	mark = parse_mark_ref(p, &پڇاڙي);
	إذا (*پڇاڙي != '\0')
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
	حرف *پڇاڙي;

	mark = parse_mark_ref(*p, &پڇاڙي);
	إذا (*پڇاڙي++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = پڇاڙي;
	أرجع mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
ساكن فراغ parse_رستو(بنية strbuf *sb, ثابت حرف *p, ثابت حرف **endp,
		صحيح is_آخري_ميدان, ثابت حرف *ميدان)
{
	إذا (*p == '"') {
		إذا (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), ميدان, command_buf.buf);
		إذا (لڙي_ڊيگهه(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), ميدان, command_buf.buf);
	} وإلا {
		/*
		 * Unless we are parsing the last field of a line,
		 * SP is the end of this field.
		 */
		*endp = is_آخري_ميدان
			? p + لڙي_ڊيگهه(p)
			: strchrnul(p, ' ');
		strbuf_جوڙيو(sb, p, *endp - p);
	}
}

/*
 * Parse the path string into the strbuf, and complain if this is not the end of
 * the string. Unquoted strings may contain spaces.
 */
ساكن فراغ parse_رستو_eol(بنية strbuf *sb, ثابت حرف *p, ثابت حرف *ميدان)
{
	ثابت حرف *پڇاڙي;

	parse_رستو(sb, p, &پڇاڙي, 1, ميدان);
	إذا (*پڇاڙي)
		die(_("garbage after %s: %s"), ميدان, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
ساكن فراغ parse_رستو_space(بنية strbuf *sb, ثابت حرف *p,
		ثابت حرف **endp, ثابت حرف *ميدان)
{
	parse_رستو(sb, p, endp, 0, ميدان);
	إذا (**endp != ' ')
		die(_("missing space after %s: %s"), ميدان, command_buf.buf);
	(*endp)++;
}

ساكن فراغ file_change_m(بنية fast_import_state *state,
			  ثابت حرف *p,
			  بنية branch *b)
{
	ساكن بنية strbuf رستو = STRBUF_INIT;
	بنية object_داخلا *oe;
	بنية object_id oid;
	uint16_t گهڻائي, inline_معلومات = 0;

	p = parse_گهڻائي(p, &گهڻائي);
	إذا (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	بدل (گهڻائي) {
	حالة 0644:
	حالة 0755:
		گهڻائي |= S_IFREG;
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
		oe = ڳوليو_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} وإلا إذا (skip_اڳياڙي(p, "inline ", &p)) {
		inline_معلومات = 1;
		oe = عدم; /* not used with inline_data, but makes gcc happy */
	} وإلا {
		إذا (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = ڳوليو_object(&oid);
		إذا (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&رستو);
	parse_رستو_eol(&رستو, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	إذا (S_ISDIR(گهڻائي) &&
	    is_خالي_جايون_وڻ_oid(&oid, the_repository->hash_algo) &&
	    *رستو.buf) {
		وڻ_content_remove(&b->branch_وڻ, رستو.buf, عدم, 0);
		أرجع;
	}

	إذا (S_ISGITLINK(گهڻائي)) {
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
		إذا (S_ISDIR(گهڻائي))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		طالما (read_ايندڙ_command(state) != EOF) {
			ثابت حرف *v;
			إذا (skip_اڳياڙي(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			وإلا {
				parse_and_store_blob(&آخري_blob, &oid, 0);
				اكسر;
			}
		}
	} وإلا {
		تعداد object_type expected = S_ISDIR(گهڻائي) ?
						OBJ_وڻ: OBJ_BLOB;
		تعداد object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, عدم);
		إذا (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(گهڻائي) ?  _("tree") : _("blob"),
			    command_buf.buf);
		إذا (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	إذا (!*رستو.buf) {
		وڻ_content_replace(&b->branch_وڻ, &oid, گهڻائي, عدم);
		أرجع;
	}

	إذا (!verify_رستو(رستو.buf, گهڻائي))
		die(_("invalid path '%s'"), رستو.buf);
	وڻ_content_سيٽ(&b->branch_وڻ, رستو.buf, &oid, گهڻائي, عدم);
}

ساكن فراغ file_change_d(ثابت حرف *p, بنية branch *b)
{
	ساكن بنية strbuf رستو = STRBUF_INIT;

	strbuf_reset(&رستو);
	parse_رستو_eol(&رستو, p, "path");
	وڻ_content_remove(&b->branch_وڻ, رستو.buf, عدم, 1);
}

ساكن فراغ file_change_cr(ثابت حرف *p, بنية branch *b, صحيح rename)
{
	ساكن بنية strbuf ذريعو = STRBUF_INIT;
	ساكن بنية strbuf dest = STRBUF_INIT;
	بنية وڻ_داخلا پن;

	strbuf_reset(&ذريعو);
	parse_رستو_space(&ذريعو, p, &p, "source");
	strbuf_reset(&dest);
	parse_رستو_eol(&dest, p, "dest");

	memset(&پن, 0, حجم(پن));
	إذا (rename)
		وڻ_content_remove(&b->branch_وڻ, ذريعو.buf, &پن, 1);
	وإلا
		وڻ_content_get(&b->branch_وڻ, ذريعو.buf, &پن, 1);
	إذا (!پن.versions[1].گهڻائي)
		die(_("path %s not in branch"), ذريعو.buf);
	إذا (!*dest.buf) {	/* C "path/to/subdir" "" */
		وڻ_content_replace(&b->branch_وڻ,
			&پن.versions[1].oid,
			پن.versions[1].گهڻائي,
			پن.وڻ);
		أرجع;
	}
	إذا (!verify_رستو(dest.buf, پن.versions[1].گهڻائي))
		die(_("invalid path '%s'"), dest.buf);
	وڻ_content_سيٽ(&b->branch_وڻ, dest.buf,
		&پن.versions[1].oid,
		پن.versions[1].گهڻائي,
		پن.وڻ);
}

ساكن فراغ note_change_n(بنية fast_import_state *state,
			  ثابت حرف *p,
			  بنية branch *b,
			  بنانشان حرف *old_fanout)
{
	بنية object_داخلا *oe;
	بنية branch *s;
	بنية object_id oid, commit_oid;
	حرف رستو[GIT_MAX_RAWSZ * 3];
	uint16_t inline_معلومات = 0;
	بنانشان حرف new_fanout;

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
	إذا (b->انگ_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->انگ_notes = change_note_fanout(&b->branch_وڻ, 0xff);
		*old_fanout = convert_انگ_notes_منزل_fanout(b->انگ_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	إذا (*p == ':') {
		oe = ڳوليو_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} وإلا إذا (skip_اڳياڙي(p, "inline ", &p)) {
		inline_معلومات = 1;
		oe = عدم; /* not used with inline_data, but makes gcc happy */
	} وإلا {
		إذا (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = ڳوليو_object(&oid);
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
		بنية object_داخلا *commit_oe = ڳوليو_mark(marks, commit_mark);
		إذا (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} وإلا إذا (!repo_get_oid(the_repository, p, &commit_oid)) {
		سائيز_t سائيز;
		حرف *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &سائيز,
						   &commit_oid);
		إذا (!buf || سائيز < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		حرر(buf);
	} وإلا
		die(_("invalid ref name or SHA1 expression: %s"), p);

	إذا (inline_معلومات) {
		read_ايندڙ_command(state);
		parse_and_store_blob(&آخري_blob, &oid, 0);
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

	construct_رستو_with_fanout(oid_منزل_hex(&commit_oid), *old_fanout, رستو);
	إذا (وڻ_content_remove(&b->branch_وڻ, رستو, عدم, 0))
		b->انگ_notes--;

	إذا (is_null_oid(&oid))
		أرجع; /* nothing to insert */

	b->انگ_notes++;
	new_fanout = convert_انگ_notes_منزل_fanout(b->انگ_notes);
	construct_رستو_with_fanout(oid_منزل_hex(&commit_oid), new_fanout, رستو);
	وڻ_content_سيٽ(&b->branch_وڻ, رستو, &oid, S_IFREG | 0644, عدم);
}

ساكن فراغ file_change_deleteall(بنية branch *b)
{
	release_وڻ_content_recursive(b->branch_وڻ.وڻ);
	oidclr(&b->branch_وڻ.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_وڻ.versions[1].oid, the_repository->hash_algo);
	ڀريو_وڻ(&b->branch_وڻ);
	b->انگ_notes = 0;
}

ساكن فراغ parse_from_commit(بنية branch *b, حرف *buf, بنانشان ڊگهو سائيز)
{
	إذا (!buf || سائيز < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_منزل_hex(&b->oid));
	إذا (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_وڻ.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_منزل_hex(&b->oid));
	oidcpy(&b->branch_وڻ.versions[0].oid,
	       &b->branch_وڻ.versions[1].oid);
}

ساكن فراغ parse_from_existing(بنية branch *b)
{
	إذا (is_null_oid(&b->oid)) {
		oidclr(&b->branch_وڻ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_وڻ.versions[1].oid, the_repository->hash_algo);
	} وإلا {
		بنانشان ڊگهو سائيز;
		سائيز_t سائيز_st = 0;
		حرف *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &سائيز_st, &b->oid);
		سائيز = cast_سائيز_t_منزل_ulong(سائيز_st);
		parse_from_commit(b, buf, سائيز);
		حرر(buf);
	}
}

ساكن صحيح parse_objectish(بنية fast_import_state *state,
			   بنية branch *b,
			   ثابت حرف *objectish)
{
	بنية branch *s;
	بنية object_id oid;

	oidcpy(&oid, &b->branch_وڻ.versions[1].oid);

	s = lookup_branch(objectish);
	إذا (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	وإلا إذا (s) {
		بنية object_id *t = &s->branch_وڻ.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_وڻ.versions[0].oid, t);
		oidcpy(&b->branch_وڻ.versions[1].oid, t);
	} وإلا إذا (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		بنية object_داخلا *oe = ڳوليو_mark(marks, idnum);
		إذا (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		إذا (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			إذا (oe->pack_id != MAX_PACK_ID) {
				بنانشان ڊگهو سائيز;
				حرف *buf = gfi_unpack_داخلا(oe, &سائيز);
				parse_from_commit(b, buf, سائيز);
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

	إذا (b->branch_وڻ.وڻ && !oideq(&oid, &b->branch_وڻ.versions[1].oid)) {
		release_وڻ_content_recursive(b->branch_وڻ.وڻ);
		b->branch_وڻ.وڻ = عدم;
	}

	read_ايندڙ_command(state);
	أرجع 1;
}

ساكن صحيح parse_from(بنية fast_import_state *state, بنية branch *b)
{
	ثابت حرف *from;

	إذا (!skip_اڳياڙي(command_buf.buf, "from ", &from))
		أرجع 0;

	أرجع parse_objectish(state, b, from);
}

ساكن صحيح parse_objectish_with_اڳياڙي(بنية fast_import_state *state,
				       بنية branch *b,
				       ثابت حرف *اڳياڙي)
{
	ثابت حرف *بنياد;

	إذا (!skip_اڳياڙي(command_buf.buf, اڳياڙي, &بنياد))
		أرجع 0;

	أرجع parse_objectish(state, b, بنياد);
}

ساكن بنية hash_list *parse_ملايو(بنية fast_import_state *state,
				     بنانشان صحيح *ڳڻپ)
{
	بنية hash_list *list = عدم, **پڇ = &list, *n;
	ثابت حرف *from;
	بنية branch *s;

	*ڳڻپ = 0;
	طالما (skip_اڳياڙي(command_buf.buf, "merge ", &from)) {
		n = xmalloc(حجم(*n));
		s = lookup_branch(from);
		إذا (s)
			oidcpy(&n->oid, &s->oid);
		وإلا إذا (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			بنية object_داخلا *oe = ڳوليو_mark(marks, idnum);
			إذا (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} وإلا إذا (!repo_get_oid(the_repository, from, &n->oid)) {
			سائيز_t سائيز;
			حرف *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &سائيز, &n->oid);
			إذا (!buf || سائيز < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			حرر(buf);
		} وإلا
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->ايندڙ = عدم;
		*پڇ = n;
		پڇ = &n->ايندڙ;

		(*ڳڻپ)++;
		read_ايندڙ_command(state);
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
	حرف *دليلون = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	حرف *space = strchr(دليلون, ' ');

	إذا (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), دليلون);
	*space = '\0';

	sig->hash_algo = دليلون;
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
	read_ايندڙ_command(state);
	parse_معلومات(&sig->معلومات, 0, عدم);
}

ساكن فراغ discard_one_signature(بنية fast_import_state *state)
{
	بنية strbuf معلومات = STRBUF_INIT;

	read_ايندڙ_command(state);
	parse_معلومات(&معلومات, 0, عدم);
	strbuf_release(&معلومات);
}

ساكن فراغ جوڙيو_gpgsig_منزل_commit(بنية strbuf *commit_معلومات,
				 ثابت حرف *سرو,
				 بنية signature_معلومات *sig)
{
	بنية string_list siglines = STRING_LIST_INIT_NODUP;

	إذا (!sig || !sig->hash_algo)
		أرجع;

	strbuf_addstr(commit_معلومات, سرو);
	string_list_ورهايو_اندريون_رکو(&siglines, sig->معلومات.buf, "\n", -1);
	strbuf_جوڙيو_separated_string_list(commit_معلومات, "\n ", &siglines);
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
				   بنية strbuf *نياپو)
{
	جوڙيو_gpgsig_منزل_commit(new_معلومات, "gpgsig ", sig_sha1);
	جوڙيو_gpgsig_منزل_commit(new_معلومات, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_معلومات, '\n');
	strbuf_addbuf(new_معلومات, نياپو);
}

ساكن فراغ warn_invalid_signature(بنية signature_check *check,
				   ثابت حرف *نياپو, تعداد sign_گهڻائي گهڻائي)
{
	ثابت حرف *signer = check->signer ? check->signer : _("unknown");
	ثابت حرف *subject;
	صحيح subject_len = ڳوليو_commit_subject(نياپو, &subject);

	بدل (گهڻائي) {
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
					بنية strbuf *نياپو,
					تعداد sign_گهڻائي گهڻائي)
{
	بنية strbuf tmp_buf = STRBUF_INIT;
	بنية signature_check signature_check = { 0 };
	صحيح ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_معلومات);
	finalize_commit_بفر(&tmp_buf, sig_sha1, sig_sha256, نياپو);
	ret = verify_commit_بفر(tmp_buf.buf, tmp_buf.len, &signature_check);

	إذا (ret) {
		إذا (گهڻائي == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, نياپو->buf, گهڻائي);

		إذا (گهڻائي == SIGN_SIGN_IF_INVALID) {
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
					SIGN_بفر_USE_DEFAULT_ڪنجي))
				die(_("failed to sign commit object"));
			جوڙيو_سرو_signature(new_معلومات, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_بفر(new_معلومات, عدم, عدم, نياپو);
	} وإلا {
		strbuf_مٽا_سٽا(new_معلومات, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

ساكن فراغ parse_new_commit(بنية fast_import_state *state, ثابت حرف *arg)
{
	ساكن بنية strbuf نياپو = STRBUF_INIT;
	بنية signature_معلومات sig_sha1 = { عدم, عدم, STRBUF_INIT };
	بنية signature_معلومات sig_sha256 = { عدم, عدم, STRBUF_INIT };
	بنية branch *b;
	حرف *author = عدم;
	حرف *committer = عدم;
	حرف *encoding = عدم;
	بنية hash_list *ملايو_list = عدم;
	بنانشان صحيح ملايو_ڳڻپ;
	بنانشان حرف prev_fanout, new_fanout;
	ثابت حرف *v;

	b = lookup_branch(arg);
	إذا (!b)
		b = new_branch(arg);

	read_ايندڙ_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	إذا (skip_اڳياڙي(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_ايندڙ_command(state);
	}
	إذا (skip_اڳياڙي(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_ايندڙ_command(state);
	}
	إذا (!committer)
		die(_("expected committer but didn't get one"));

	طالما (skip_اڳياڙي(command_buf.buf, "gpgsig ", &v)) {
		بدل (signed_commit_گهڻائي) {

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
			BUG("invalid signed_commit_mode value %d", signed_commit_گهڻائي);
		}
		read_ايندڙ_command(state);
	}

	إذا (skip_اڳياڙي(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_ايندڙ_command(state);
	}
	parse_معلومات(&نياپو, 0, عدم);
	read_ايندڙ_command(state);
	parse_from(state, b);
	ملايو_list = parse_ملايو(state, &ملايو_ڳڻپ);

	/* ensure the branch is active/loaded */
	إذا (!b->branch_وڻ.وڻ || !max_سرگرم_branches) {
		unload_one_branch();
		ڀريو_branch(b);
	}

	prev_fanout = convert_انگ_notes_منزل_fanout(b->انگ_notes);

	/* file_change* */
	طالما (command_buf.len > 0) {
		إذا (skip_اڳياڙي(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		وإلا إذا (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		وإلا {
			unread_command_buf = 1;
			اكسر;
		}
		إذا (read_ايندڙ_command(state) == EOF)
			اكسر;
	}

	new_fanout = convert_انگ_notes_منزل_fanout(b->انگ_notes);
	إذا (new_fanout != prev_fanout)
		b->انگ_notes = change_note_fanout(&b->branch_وڻ, new_fanout);

	/* build the tree and the commit */
	store_وڻ(&b->branch_وڻ);
	oidcpy(&b->branch_وڻ.versions[0].oid,
	       &b->branch_وڻ.versions[1].oid);

	strbuf_reset(&new_معلومات);
	strbuf_addf(&new_معلومات, "tree %s\n",
		oid_منزل_hex(&b->branch_وڻ.versions[1].oid));
	إذا (!is_null_oid(&b->oid))
		strbuf_addf(&new_معلومات, "parent %s\n",
			    oid_منزل_hex(&b->oid));
	طالما (ملايو_list) {
		بنية hash_list *ايندڙ = ملايو_list->ايندڙ;
		strbuf_addf(&new_معلومات, "parent %s\n",
			    oid_منزل_hex(&ملايو_list->oid));
		حرر(ملايو_list);
		ملايو_list = ايندڙ;
	}
	strbuf_addf(&new_معلومات,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	إذا (encoding)
		strbuf_addf(&new_معلومات,
			"encoding %s\n",
			encoding);

	إذا ((signed_commit_گهڻائي == SIGN_STRIP_IF_INVALID ||
	     signed_commit_گهڻائي == SIGN_SIGN_IF_INVALID ||
	     signed_commit_گهڻائي == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_معلومات, &sig_sha1, &sig_sha256,
					    &نياپو, signed_commit_گهڻائي);
	وإلا
		finalize_commit_بفر(&new_معلومات, &sig_sha1, &sig_sha256, &نياپو);

	حرر(author);
	حرر(committer);
	حرر(encoding);

	إذا (!store_object(OBJ_COMMIT, &new_معلومات, عدم, &b->oid, ايندڙ_mark))
		b->pack_id = pack_id;
	b->آخري_commit = object_ڳڻپ_by_type[OBJ_COMMIT];
}

ساكن فراغ handle_ٽيگ_signature_if_invalid(بنية strbuf *buf,
					    بنية strbuf *نياپو,
					    سائيز_t sig_وٿي)
{
	بنية strbuf signature = STRBUF_INIT;
	بنية strbuf payload = STRBUF_INIT;
	بنية signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_جوڙيو(&payload, نياپو->buf, sig_وٿي);
	strbuf_جوڙيو(&signature, نياپو->buf + sig_وٿي, نياپو->len - sig_وٿي);

	sigc.payload_type = SIGNATURE_PAYLOAD_ٽيگ;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	إذا (!check_signature(&sigc, signature.buf, signature.len))
		اذهب ٻاهريون;

	إذا (signed_ٽيگ_گهڻائي == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(نياپو, sig_وٿي);

	إذا (signed_ٽيگ_گهڻائي == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = عدم;
		strbuf_reset(&signature);

		إذا (sign_بفر(&payload, &signature, signed_ٽيگ_keyid,
				SIGN_بفر_USE_DEFAULT_ڪنجي))
			die(_("failed to sign tag object"));

		strbuf_addbuf(نياپو, &signature);
	}

ٻاهريون:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

ساكن فراغ handle_ٽيگ_signature(بنية strbuf *buf, بنية strbuf *نياپو, ثابت حرف *name)
{
	سائيز_t sig_وٿي = parse_signed_بفر(نياپو->buf, نياپو->len);

	/* If there is no signature, there is nothing to do. */
	إذا (sig_وٿي >= نياپو->len)
		أرجع;

	بدل (signed_ٽيگ_گهڻائي) {

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
		strbuf_setlen(نياپو, sig_وٿي);
		اكسر;
	حالة SIGN_ABORT_IF_INVALID:
	حالة SIGN_SIGN_IF_INVALID:
	حالة SIGN_STRIP_IF_INVALID:
		handle_ٽيگ_signature_if_invalid(buf, نياپو, sig_وٿي);
		اكسر;

	/* Third, aborting modes */
	حالة SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	افتراضي:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_ٽيگ_گهڻائي, name);
	}
}

ساكن فراغ parse_new_ٽيگ(بنية fast_import_state *state, ثابت حرف *arg)
{
	ساكن بنية strbuf نياپو = STRBUF_INIT;
	ثابت حرف *from;
	حرف *tagger;
	بنية branch *s;
	بنية ٽيگ *t;
	uintmax_t from_mark = 0;
	بنية object_id oid;
	تعداد object_type type;
	ثابت حرف *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية ٽيگ));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	إذا (آخري_ٽيگ)
		آخري_ٽيگ->ايندڙ_ٽيگ = t;
	وإلا
		پهريون_ٽيگ = t;
	آخري_ٽيگ = t;
	read_ايندڙ_command(state);
	parse_mark(state);

	/* from ... */
	إذا (!skip_اڳياڙي(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	إذا (s) {
		إذا (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} وإلا إذا (*from == ':') {
		بنية object_داخلا *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = ڳوليو_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} وإلا إذا (!repo_get_oid(the_repository, from, &oid)) {
		بنية object_داخلا *oe = ڳوليو_object(&oid);
		إذا (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, عدم);
			إذا (type < 0)
				die(_("not a valid object: %s"), from);
		} وإلا
			type = oe->type;
	} وإلا
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_ايندڙ_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	إذا (skip_اڳياڙي(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_ايندڙ_command(state);
	} وإلا
		tagger = عدم;

	/* tag payload/message */
	parse_معلومات(&نياپو, 0, عدم);

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

	handle_ٽيگ_signature(&new_معلومات, &نياپو, t->name);

	strbuf_addch(&new_معلومات, '\n');
	strbuf_addbuf(&new_معلومات, &نياپو);
	حرر(tagger);

	إذا (store_object(OBJ_ٽيگ, &new_معلومات, عدم, &t->oid, ايندڙ_mark))
		t->pack_id = MAX_PACK_ID;
	وإلا
		t->pack_id = pack_id;
}

ساكن فراغ parse_reset_branch(بنية fast_import_state *state, ثابت حرف *arg)
{
	بنية branch *b;
	ثابت حرف *ٽيگ_name;

	b = lookup_branch(arg);
	إذا (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_وڻ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_وڻ.versions[1].oid, the_repository->hash_algo);
		إذا (b->branch_وڻ.وڻ) {
			release_وڻ_content_recursive(b->branch_وڻ.وڻ);
			b->branch_وڻ.وڻ = عدم;
		}
	}
	وإلا
		b = new_branch(arg);
	read_ايندڙ_command(state);
	parse_from(state, b);
	إذا (b->delete && skip_اڳياڙي(b->name, "refs/tags/", &ٽيگ_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		بنية ٽيگ *t, *prev = عدم;
		لكل (t = پهريون_ٽيگ; t; t = t->ايندڙ_ٽيگ) {
			إذا (!strcmp(t->name, ٽيگ_name))
				اكسر;
			prev = t;
		}
		إذا (t) {
			إذا (prev)
				prev->ايندڙ_ٽيگ = t->ايندڙ_ٽيگ;
			وإلا
				پهريون_ٽيگ = t->ايندڙ_ٽيگ;
			إذا (!t->ايندڙ_ٽيگ)
				آخري_ٽيگ = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	إذا (command_buf.len > 0)
		unread_command_buf = 1;
}

ساكن فراغ cat_blob_write(ثابت حرف *buf, بنانشان ڊگهو سائيز)
{
	إذا (write_اندريون_ڀريل(cat_blob_fd, buf, سائيز) < 0)
		die_errno(_("write to frontend failed"));
}

ساكن فراغ cat_blob(بنية object_داخلا *oe, بنية object_id *oid)
{
	بنية strbuf سٽ = STRBUF_INIT;
	بنية iovec iov[3];
	بنانشان ڊگهو سائيز;
	تعداد object_type type = 0;
	حرف *buf;

	إذا (!oe || oe->pack_id == MAX_PACK_ID) {
		سائيز_t سائيز_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &سائيز_st);
		سائيز = cast_سائيز_t_منزل_ulong(سائيز_st);
	} وإلا {
		type = oe->type;
		buf = gfi_unpack_داخلا(oe, &سائيز);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	إذا (type <= 0) {
		strbuf_reset(&سٽ);
		strbuf_addf(&سٽ, "%s missing\n", oid_منزل_hex(oid));
		cat_blob_write(سٽ.buf, سٽ.len);
		strbuf_release(&سٽ);
		حرر(buf);
		أرجع;
	}
	إذا (!buf)
		die(_("can't read object %s"), oid_منزل_hex(oid));
	إذا (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_منزل_hex(oid), type_name(type));
	strbuf_reset(&سٽ);
	strbuf_addf(&سٽ, "%s %s %"PRIuMAX"\n", oid_منزل_hex(oid),
		    type_name(type), (uintmax_t)سائيز);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_بنياد = سٽ.buf;
	iov[0].iov_len = سٽ.len;
	iov[1].iov_بنياد = buf;
	iov[1].iov_len = سائيز;
	iov[2].iov_بنياد = (فراغ *) "\n";
	iov[2].iov_len = 1;

	إذا (writev_اندريون_ڀريل(cat_blob_fd, iov, ARRAY_سائيز(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&سٽ);
	إذا (oe && oe->pack_id == pack_id) {
		آخري_blob.وٿي = oe->idx.وٿي;
		strbuf_attach(&آخري_blob.معلومات, buf, سائيز, سائيز + 1);
		آخري_blob.اونهائي = oe->اونهائي;
	} وإلا
		حرر(buf);
}

ساكن فراغ parse_get_mark(بنية fast_import_state *state اڻ_استعمال_ٿيل,
			   ثابت حرف *p)
{
	بنية object_داخلا *oe;
	حرف output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	إذا (*p != ':')
		die(_("not a mark: %s"), p);

	oe = ڳوليو_mark(marks, parse_mark_ref_eol(p));
	إذا (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, حجم(output), "%s\n", oid_منزل_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

ساكن فراغ parse_cat_blob(بنية fast_import_state *state اڻ_استعمال_ٿيل,
			   ثابت حرف *p)
{
	بنية object_داخلا *oe;
	بنية object_id oid;

	/* cat-blob SP <object> LF */
	إذا (*p == ':') {
		oe = ڳوليو_mark(marks, parse_mark_ref_eol(p));
		إذا (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} وإلا {
		إذا (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		إذا (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = ڳوليو_object(&oid);
	}

	cat_blob(oe, &oid);
}

ساكن بنية object_داخلا *dereference(بنية object_داخلا *oe,
					بنية object_id *oid)
{
	بنانشان ڊگهو سائيز;
	حرف *buf = عدم;
	ثابت بنانشان hexsz = the_hash_algo->hexsz;

	إذا (!oe) {
		تعداد object_type type = odb_read_object_info(the_repository->objects,
							     oid, عدم);
		إذا (type < 0)
			die(_("object not found: %s"), oid_منزل_hex(oid));
		/* cache it! */
		oe = وجهو_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.وٿي = 1;
	}
	بدل (oe->type) {
	حالة OBJ_وڻ:	/* easy case. */
		أرجع oe;
	حالة OBJ_COMMIT:
	حالة OBJ_ٽيگ:
		اكسر;
	افتراضي:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	إذا (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_داخلا(oe, &سائيز);
	} وإلا {
		تعداد object_type اڻ_استعمال_ٿيل_2;
		سائيز_t سائيز_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &اڻ_استعمال_ٿيل_2, &سائيز_st);
		سائيز = cast_سائيز_t_منزل_ulong(سائيز_st);
	}
	إذا (!buf)
		die(_("can't load object %s"), oid_منزل_hex(oid));

	/* Peel one layer. */
	بدل (oe->type) {
	حالة OBJ_ٽيگ:
		إذا (سائيز < hexsz + لڙي_ڊيگهه("object ") ||
		    get_oid_hex(buf + لڙي_ڊيگهه("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		اكسر;
	حالة OBJ_COMMIT:
		إذا (سائيز < hexsz + لڙي_ڊيگهه("tree ") ||
		    get_oid_hex(buf + لڙي_ڊيگهه("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	حرر(buf);
	أرجع ڳوليو_object(oid);
}

ساكن فراغ وجهو_mapped_mark(uintmax_t mark, فراغ *object, فراغ *cbp)
{
	بنية object_id *fromoid = object;
	بنية object_id *tooid = ڳوليو_mark(cbp, mark);
	صحيح ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	إذا (ret == 0)
		أرجع;
	kh_قدر(sub_oid_map, it) = tooid;
}

ساكن فراغ build_mark_map_one(بنية mark_سيٽ *from, بنية mark_سيٽ *منزل)
{
	for_each_mark(from, 0, وجهو_mapped_mark, منزل);
}

ساكن فراغ build_mark_map(بنية string_list *from, بنية string_list *منزل)
{
	بنية string_list_شي *fromp, *چوٽي;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_شي(fromp, from) {
		چوٽي = string_list_lookup(منزل, fromp->string);
		إذا (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} وإلا إذا (!چوٽي || !چوٽي->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, چوٽي->util);
	}
}

ساكن بنية object_داخلا *parse_treeish_dataref(ثابت حرف **p)
{
	بنية object_id oid;
	بنية object_داخلا *e;

	إذا (**p == ':') {	/* <mark> */
		e = ڳوليو_mark(marks, parse_mark_ref_space(p));
		إذا (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} وإلا {	/* <sha1> */
		إذا (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = ڳوليو_object(&oid);
		إذا (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	طالما (!e || e->type != OBJ_وڻ)
		e = dereference(e, &oid);
	أرجع e;
}

ساكن فراغ print_ls(صحيح گهڻائي, ثابت بنانشان حرف *hash, ثابت حرف *رستو)
{
	ساكن بنية strbuf سٽ = STRBUF_INIT;

	/* See show_tree(). */
	ثابت حرف *type =
		S_ISGITLINK(گهڻائي) ? commit_type :
		S_ISDIR(گهڻائي) ? وڻ_type :
		blob_type;

	إذا (!گهڻائي) {
		/* missing SP path LF */
		strbuf_reset(&سٽ);
		strbuf_addstr(&سٽ, "missing ");
		quote_c_style(رستو, &سٽ, عدم, 0);
		strbuf_addch(&سٽ, '\n');
	} وإلا {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&سٽ);
		strbuf_addf(&سٽ, "%06o %s %s\t",
				گهڻائي & ~NO_DELTA, type, hash_منزل_hex(hash));
		quote_c_style(رستو, &سٽ, عدم, 0);
		strbuf_addch(&سٽ, '\n');
	}
	cat_blob_write(سٽ.buf, سٽ.len);
}

ساكن فراغ parse_ls(بنية fast_import_state *state اڻ_استعمال_ٿيل,
		     ثابت حرف *p,
		     بنية branch *b)
{
	ساكن بنية strbuf رستو = STRBUF_INIT;
	بنية وڻ_داخلا *پاڙ = عدم;
	بنية وڻ_داخلا پن = {عدم};

	/* ls SP (<tree-ish> SP)? <path> */
	إذا (*p == '"') {
		إذا (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		پاڙ = &b->branch_وڻ;
	} وإلا {
		بنية object_داخلا *e = parse_treeish_dataref(&p);
		پاڙ = new_وڻ_داخلا();
		oidcpy(&پاڙ->versions[1].oid, &e->idx.oid);
		إذا (!is_null_oid(&پاڙ->versions[1].oid))
			پاڙ->versions[1].گهڻائي = S_IFDIR;
		ڀريو_وڻ(پاڙ);
	}
	strbuf_reset(&رستو);
	parse_رستو_eol(&رستو, p, "path");
	وڻ_content_get(پاڙ, رستو.buf, &پن, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	إذا (S_ISDIR(پن.versions[1].گهڻائي))
		store_وڻ(&پن);

	print_ls(پن.versions[1].گهڻائي, پن.versions[1].oid.hash, رستو.buf);
	إذا (پن.وڻ)
		release_وڻ_content_recursive(پن.وڻ);
	إذا (!b || پاڙ != &b->branch_وڻ)
		release_وڻ_داخلا(پاڙ);
}

ساكن فراغ checkpoint(فراغ)
{
	checkpoint_requested = 0;
	إذا (object_ڳڻپ) {
		چڪر_packfile();
	}
	dump_branches();
	dump_ٽيگ_فهرست();
	dump_marks();
}

ساكن فراغ parse_checkpoint(بنية fast_import_state *state اڻ_استعمال_ٿيل)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

ساكن فراغ parse_ترقي(بنية fast_import_state *state اڻ_استعمال_ٿيل)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

ساكن فراغ parse_alias(بنية fast_import_state *state)
{
	بنية object_داخلا *e;
	بنية branch b;

	skip_optional_lf();
	read_ايندڙ_command(state);

	/* mark ... */
	parse_mark(state);
	إذا (!ايندڙ_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, حجم(b));
	إذا (!parse_objectish_with_اڳياڙي(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = ڳوليو_object(&b.oid);
	أكد(e);
	وجهو_mark(&marks, ايندڙ_mark, e);
}

ساكن حرف* make_fast_import_رستو(بنية fast_import_state *state,
				   ثابت حرف *رستو)
{
	إذا (!relative_marks_paths || is_absolute_رستو(رستو))
		أرجع اڳياڙي_filename(state->اڳياڙي, رستو);
	أرجع repo_git_رستو(the_repository, "info/fast-import/%s", رستو);
}

ساكن فراغ option_import_marks(بنية fast_import_state *state,
				ثابت حرف *marks,
				صحيح from_وهڪرو,
				صحيح ignore_missing)
{
	إذا (import_marks_file) {
		إذا (from_وهڪرو)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		إذا(!import_marks_file_from_وهڪرو)
			read_marks();
	}

	حرر(import_marks_file);
	import_marks_file = make_fast_import_رستو(state, marks);
	import_marks_file_from_وهڪرو = from_وهڪرو;
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

ساكن بنانشان ڊگهو ulong_arg(ثابت حرف *option, ثابت حرف *arg)
{
	حرف *endptr;
	بنانشان ڊگهو rv = strtoul(arg, &endptr, 0);
	إذا (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	أرجع rv;
}

ساكن فراغ option_اونهائي(ثابت حرف *اونهائي)
{
	max_اونهائي = ulong_arg("--depth", اونهائي);
	إذا (max_اونهائي > MAX_اونهائي)
		die(_("--depth cannot exceed %u"), MAX_اونهائي);
}

ساكن فراغ option_سرگرم_branches(ثابت حرف *branches)
{
	max_سرگرم_branches = ulong_arg("--active-branches", branches);
}

ساكن فراغ option_export_marks(بنية fast_import_state *state,
				ثابت حرف *marks)
{
	حرر(export_marks_file);
	export_marks_file = make_fast_import_رستو(state, marks);
}

ساكن فراغ option_cat_blob_fd(بنية fast_import_state *state اڻ_استعمال_ٿيل,
			       ثابت حرف *fd)
{
	بنانشان ڊگهو n = ulong_arg("--cat-blob-fd", fd);
	إذا (n > (بنانشان ڊگهو) وڌ_کان_وڌ_عدد)
		die(_("--cat-blob-fd cannot exceed %d"), وڌ_کان_وڌ_عدد);
	cat_blob_fd = (صحيح) n;
}

ساكن فراغ option_export_pack_ڪنارا(بنية fast_import_state *state,
				     ثابت حرف *ڪنارا)
{
	حرف *fn = اڳياڙي_filename(state->اڳياڙي, ڪنارا);
	إذا (pack_ڪنارا)
		fclose(pack_ڪنارا);
	pack_ڪنارا = xfopen(fn, "a");
	حرر(fn);
}

ساكن فراغ option_rewrite_submodules(بنية fast_import_state *state,
				      ثابت حرف *arg,
				      بنية string_list *list)
{
	بنية mark_سيٽ *ms;
	FILE *fp;
	حرف *s = xstrdup(arg);
	حرف *f = strchr(s, ':');
	إذا (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = اڳياڙي_filename(state->اڳياڙي, f);
	fp = fopen(f, "r");
	إذا (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, وجهو_oid_داخلا);
	fclose(fp);
	حرر(f);

	string_list_وجهو(list, s)->util = ms;

	حرر(s);
}

ساكن فراغ option_max_pack_سائيز(ثابت حرف *arg)
{
	بنانشان ڊگهو v;

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
	بنانشان ڊگهو v;

	إذا (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_سيٽ_big_file_حد(the_repository, v);
}

ساكن فراغ option_signed_commits(ثابت حرف *arg)
{
	إذا (parse_sign_گهڻائي(arg, &signed_commit_گهڻائي, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

ساكن فراغ option_signed_ٽيگ_فهرست(ثابت حرف *arg)
{
	إذا (parse_sign_گهڻائي(arg, &signed_ٽيگ_گهڻائي, &signed_ٽيگ_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

ساكن فراغ option_quiet(فراغ)
{
	show_stats = 0;
	quiet = 1;
}

ساكن صحيح parse_one_option(بنية fast_import_state *state, ثابت حرف *option)
{
	إذا (skip_اڳياڙي(option, "max-pack-size=", &option)) {
		option_max_pack_سائيز(option);
	} وإلا إذا (skip_اڳياڙي(option, "big-file-threshold=", &option)) {
		option_big_file_حد(option);
	} وإلا إذا (skip_اڳياڙي(option, "depth=", &option)) {
		option_اونهائي(option);
	} وإلا إذا (skip_اڳياڙي(option, "active-branches=", &option)) {
		option_سرگرم_branches(option);
	} وإلا إذا (skip_اڳياڙي(option, "export-pack-edges=", &option)) {
		option_export_pack_ڪنارا(state, option);
	} وإلا إذا (skip_اڳياڙي(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} وإلا إذا (skip_اڳياڙي(option, "signed-tags=", &option)) {
		option_signed_ٽيگ_فهرست(option);
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

	إذا (skip_اڳياڙي(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} وإلا إذا (skip_اڳياڙي(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} وإلا إذا (skip_اڳياڙي(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} وإلا إذا (skip_اڳياڙي(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} وإلا إذا (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} وإلا إذا (skip_اڳياڙي(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_منزل);
	} وإلا إذا (skip_اڳياڙي(feature, "rewrite-submodules-from=", &arg)) {
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
	إذا (state->ڏٺل_معلومات_command)
		die(_("got feature command '%s' after data command"), feature);

	إذا (parse_one_feature(state, feature))
		أرجع;

	die(_("this version of fast-import does not support feature %s."), feature);
}

ساكن فراغ parse_option(بنية fast_import_state *state, ثابت حرف *option)
{
	إذا (state->ڏٺل_معلومات_command)
		die(_("got option command '%s' after data command"), option);

	إذا (parse_one_option(state, option))
		أرجع;

	die(_("this version of fast-import does not support option: %s"), option);
}

ساكن فراغ git_pack_config(فراغ)
{
	صحيح indexversion_قدر;
	صحيح حد_بندي;
	بنانشان ڊگهو packsizelimit_قدر;

	إذا (!repo_config_get_ulong(the_repository, "pack.depth", &max_اونهائي)) {
		إذا (max_اونهائي > MAX_اونهائي)
			max_اونهائي = MAX_اونهائي;
	}
	إذا (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_قدر)) {
		pack_idx_opts.نسخو = indexversion_قدر;
		إذا (pack_idx_opts.نسخو > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.نسخو);
	}
	إذا (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_قدر))
		max_packsize = packsizelimit_قدر;

	إذا (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &حد_بندي))
		unpack_حد_بندي = حد_بندي;
	وإلا إذا (!repo_config_get_int(the_repository, "transfer.unpacklimit", &حد_بندي))
		unpack_حد_بندي = حد_بندي;

	repo_config(the_repository, git_default_config, عدم);
}

ساكن ثابت حرف *ثابت fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	عدم
};

ساكن فراغ parse_argv(بنية fast_import_state *state)
{
	صحيح argc = parse_options(state->argc, state->argv, state->اڳياڙي,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	إذا (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->ڏٺل_معلومات_command = 1;
	إذا (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_منزل);
}

ساكن صحيح option_parse_date_format(ثابت بنية option *opt اڻ_استعمال_ٿيل,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	أرجع 0;
}

ساكن صحيح option_parse_export_pack_ڪنارا(ثابت بنية option *opt,
					  ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_ڪنارا(opt->قدر, arg);
	أرجع 0;
}

ساكن صحيح option_parse_max_pack_سائيز(ثابت بنية option *opt اڻ_استعمال_ٿيل,
				      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_سائيز(arg);
	أرجع 0;
}

ساكن صحيح option_parse_big_file_حد(ثابت بنية option *opt اڻ_استعمال_ٿيل,
					   ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_حد(arg);
	أرجع 0;
}

ساكن صحيح option_parse_signed_commits(ثابت بنية option *opt اڻ_استعمال_ٿيل,
				       ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	أرجع 0;
}

ساكن صحيح option_parse_signed_ٽيگ_فهرست(ثابت بنية option *opt اڻ_استعمال_ٿيل,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_ٽيگ_فهرست(arg);
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

ساكن صحيح option_parse_اونهائي(ثابت بنية option *opt اڻ_استعمال_ٿيل,
			      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_اونهائي(arg);
	أرجع 0;
}

ساكن صحيح option_parse_سرگرم_branches(ثابت بنية option *opt اڻ_استعمال_ٿيل,
					ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_سرگرم_branches(arg);
	أرجع 0;
}

ساكن صحيح option_parse_quiet(ثابت بنية option *opt اڻ_استعمال_ٿيل,
			      ثابت حرف *arg اڻ_استعمال_ٿيل, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	أرجع 0;
}

صحيح cmd_fast_import(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *اڳياڙي,
		    بنية repository *repo)
{
	بنية fast_import_state state;

	بنية option fast_import_options[] = {
		OPT_گروهه(N_("Common")),
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
			       PARSE_OPT_NONEG, option_parse_max_pack_سائيز),
		OPT_CALLBACK_F(0, "big-file-threshold", عدم, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_حد),
		OPT_CALLBACK_F(0, "depth", عدم, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_اونهائي),
		OPT_CALLBACK_F(0, "active-branches", عدم, N_("n"),
			       N_("maximum number of branches to maintain active"),
			       PARSE_OPT_NONEG, option_parse_سرگرم_branches),
		OPT_گروهه(N_("Marks")),
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
		OPT_گروهه(N_("Submodule rewrite")),
		OPT_CALLBACK_F(0, "rewrite-submodules-from", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> from <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_from),
		OPT_CALLBACK_F(0, "rewrite-submodules-to", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> to <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_منزل),
		OPT_گروهه(N_("Signing")),
		OPT_CALLBACK_F(0, "signed-commits", عدم, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", عدم, N_("mode"),
			       N_("how to handle signed tags"),
			       PARSE_OPT_NONEG, option_parse_signed_ٽيگ_فهرست),
		OPT_HIDDEN_گروهه(N_("Advanced")),
		OPT_BOOL_F(0, "allow-unsafe-features", &state.allow_unsafe_features,
			   N_("allow unsafe mark commands from the stream"),
			   PARSE_OPT_HIDDEN | PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "export-pack-edges", &state, N_("file"),
			       N_("dump edge commits to <file>"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_export_pack_ڪنارا),
		OPT_CALLBACK_F(0, "cat-blob-fd", &state, N_("fd"),
			       N_("write some responses to <fd> instead of stdout"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_cat_blob_fd),
		OPT_پڇاڙي()
	};

	show_usage_with_options_if_asked(argc, argv, fast_import_usage, fast_import_options);

	fast_import_state_init(&state, argc, argv, اڳياڙي, fast_import_options);

	reset_pack_idx_option(&pack_idx_opts);
	git_pack_config();

	alloc_objects(object_داخلا_alloc);
	strbuf_init(&command_buf, 0);
	CALLOC_ARRAY(atom_جدول, atom_جدول_sz);
	CALLOC_ARRAY(branch_جدول, branch_جدول_sz);
	CALLOC_ARRAY(avail_وڻ_جدول, avail_وڻ_جدول_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_سيٽ));

	hashmap_init(&object_جدول, object_داخلا_hashcmp, عدم, 0);

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
	لكل (بنانشان صحيح i = 0; i < (cmd_save - 1); i++)
		rc_free[i].ايندڙ = &rc_free[i + 1];
	rc_free[cmd_save - 1].ايندڙ = عدم;

	شروعات_packfile();
	سيٽ_die_routine(die_nicely);
	سيٽ_checkpoint_signal();
	طالما (read_ايندڙ_command(&state) != EOF) {
		ثابت حرف *v;
		إذا (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "tag ", &v))
			parse_new_ٽيگ(&state, v);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, عدم);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		وإلا إذا (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		وإلا إذا (!strcmp("done", command_buf.buf))
			اكسر;
		وإلا إذا (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		وإلا إذا (starts_with(command_buf.buf, "progress "))
			parse_ترقي(&state);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		وإلا إذا (skip_اڳياڙي(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		وإلا إذا (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		وإلا
			die(_("unsupported command: %s"), command_buf.buf);

		إذا (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	إذا (!state.ڏٺل_معلومات_command)
		parse_argv(&state);

	إذا (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	پڇاڙي_packfile();

	dump_branches();
	dump_ٽيگ_فهرست();
	unkeep_all_packs();
	dump_marks();

	إذا (pack_ڪنارا)
		fclose(pack_ڪنارا);

	إذا (show_stats) {
		uintmax_t ڪل_ڳڻپ = 0, duplicate_ڳڻپ = 0;
		لكل (سائيز_t i = 0; i < ARRAY_سائيز(object_ڳڻپ_by_type); i++)
			ڪل_ڳڻپ += object_ڳڻپ_by_type[i];
		لكل (سائيز_t i = 0; i < ARRAY_سائيز(duplicate_ڳڻپ_by_type); i++)
			duplicate_ڳڻپ += duplicate_ڳڻپ_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_ڳڻپ);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", ڪل_ڳڻپ, duplicate_ڳڻپ);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ڳڻپ_by_type[OBJ_BLOB], duplicate_ڳڻپ_by_type[OBJ_BLOB], delta_ڳڻپ_by_type[OBJ_BLOB], delta_ڳڻپ_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ڳڻپ_by_type[OBJ_وڻ], duplicate_ڳڻپ_by_type[OBJ_وڻ], delta_ڳڻپ_by_type[OBJ_وڻ], delta_ڳڻپ_attempts_by_type[OBJ_وڻ]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ڳڻپ_by_type[OBJ_COMMIT], duplicate_ڳڻپ_by_type[OBJ_COMMIT], delta_ڳڻپ_by_type[OBJ_COMMIT], delta_ڳڻپ_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ڳڻپ_by_type[OBJ_ٽيگ], duplicate_ڳڻپ_by_type[OBJ_ٽيگ], delta_ڳڻپ_by_type[OBJ_ٽيگ], delta_ڳڻپ_attempts_by_type[OBJ_ٽيگ]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_ڳڻپ, branch_ڀريو_ڳڻپ);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->سرڪايو) * 1024, marks_سيٽ_ڳڻپ);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (وڻ_داخلا_allocd + fi_mem_pool.pool_alloc + alloc_ڳڻپ*حجم(بنية object_داخلا))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (بنانشان ڊگهو)((وڻ_داخلا_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_ڳڻپ*حجم(بنية object_داخلا))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	أرجع failure ? 1 : 0;
}
