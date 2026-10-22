#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "object.h"
#অন্তর্ভাব "blob.h"
#অন্তর্ভাব "tree.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "delta.h"
#অন্তর্ভাব "pack.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "csum-file.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "packfile.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "mem-pool.h"
#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "khash.h"
#অন্তর্ভাব "date.h"
#অন্তর্ভাব "gpg-interface.h"
#অন্তর্ভাব "parse-options.h"

#define PACK_ID_BITS 16
#define MAX_PACK_ID ((1<<PACK_ID_BITS)-1)
#define DEPTH_BITS 13
#define MAX_DEPTH ((1<<DEPTH_BITS)-1)

/*
 * We abuse the setuid bit on directories to mean "do not delta".
 */
#define NO_DELTA S_ISUID

/*
 * The amount of additional space required in order to write an object into the
 * current pack. This is the hash lengths at the end of the pack, plus the
 * length of one object ID.
 */
#define PACK_আকার_THRESHOLD (the_hash_algo->rawsz * 3)

কাঠামো object_entry {
	কাঠামো pack_idx_entry idx;
	কাঠামো hashmap_entry ent;
	uint32_t type : TYPE_BITS,
		pack_id : PACK_ID_BITS,
		depth : DEPTH_BITS;
};

স্থির পূর্ণসংখ্যা object_entry_hashcmp(ধ্রুবক শূন্য *map_তথ্য UNUSED,
				ধ্রুবক কাঠামো hashmap_entry *eptr,
				ধ্রুবক কাঠামো hashmap_entry *entry_or_চাবি,
				ধ্রুবক শূন্য *keydata)
{
	ধ্রুবক কাঠামো object_id *oid = keydata;
	ধ্রুবক কাঠামো object_entry *e1, *e2;

	e1 = container_of(eptr, ধ্রুবক কাঠামো object_entry, ent);
	যদি (oid)
		ফেরত oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_চাবি, ধ্রুবক কাঠামো object_entry, ent);
	ফেরত oidcmp(&e1->idx.oid, &e2->idx.oid);
}

কাঠামো object_entry_pool {
	কাঠামো object_entry_pool *পরবর্তী_pool;
	কাঠামো object_entry *পরবর্তী_free;
	কাঠামো object_entry *end;
	কাঠামো object_entry entries[FLEX_ARRAY]; /* more */
};

কাঠামো mark_set {
	সংঘ {
		কাঠামো object_id *oids[1024];
		কাঠামো object_entry *marked[1024];
		কাঠামো mark_set *sets[1024];
	} তথ্য;
	unsigned পূর্ণসংখ্যা shift;
};

কাঠামো last_object {
	কাঠামো strbuf তথ্য;
	off_t offset;
	unsigned পূর্ণসংখ্যা depth;
	unsigned no_swap : 1;
};

কাঠামো atom_str {
	কাঠামো atom_str *পরবর্তী_atom;
	unsigned short str_len;
	অক্ষর str_dat[FLEX_ARRAY]; /* more */
};

কাঠামো বৃক্ষ_content;
কাঠামো বৃক্ষ_entry {
	কাঠামো বৃক্ষ_content *বৃক্ষ;
	কাঠামো atom_str *name;
	কাঠামো বৃক্ষ_entry_ms {
		uint16_t mode;
		কাঠামো object_id oid;
	} versions[2];
};

কাঠামো বৃক্ষ_content {
	unsigned পূর্ণসংখ্যা entry_capacity; /* must match avail_tree_content */
	unsigned পূর্ণসংখ্যা entry_গুনতি;
	unsigned পূর্ণসংখ্যা delta_depth;
	কাঠামো বৃক্ষ_entry *entries[FLEX_ARRAY]; /* more */
};

কাঠামো avail_বৃক্ষ_content {
	unsigned পূর্ণসংখ্যা entry_capacity; /* must match tree_content */
	কাঠামো avail_বৃক্ষ_content *পরবর্তী_avail;
};

কাঠামো branch {
	কাঠামো branch *table_পরবর্তী_branch;
	কাঠামো branch *active_পরবর্তী_branch;
	ধ্রুবক অক্ষর *name;
	কাঠামো বৃক্ষ_entry branch_বৃক্ষ;
	uintmax_t last_commit;
	uintmax_t অঙ্ক_notes;
	unsigned active : 1;
	unsigned delete : 1;
	unsigned pack_id : PACK_ID_BITS;
	কাঠামো object_id oid;
};

কাঠামো tag {
	কাঠামো tag *পরবর্তী_tag;
	ধ্রুবক অক্ষর *name;
	unsigned পূর্ণসংখ্যা pack_id;
	কাঠামো object_id oid;
};

কাঠামো hash_list {
	কাঠামো hash_list *পরবর্তী;
	কাঠামো object_id oid;
};

প্রকারলেখ গণনা {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

কাঠামো recent_command {
	কাঠামো recent_command *পূর্ব;
	কাঠামো recent_command *পরবর্তী;
	অক্ষর *buf;
};

প্রকারলেখ শূন্য (*mark_set_inserter_t)(কাঠামো mark_set **s, কাঠামো object_id *oid, uintmax_t mark);
প্রকারলেখ শূন্য (*each_mark_fn_t)(uintmax_t mark, শূন্য *obj, শূন্য *cbp);

/* Configured limits on output */
স্থির unsigned long max_depth = 50;
স্থির off_t max_packsize;
স্থির পূর্ণসংখ্যা unpack_limit = 100;
স্থির পূর্ণসংখ্যা force_update;

/* Stats and misc. counters */
স্থির uintmax_t alloc_গুনতি;
স্থির uintmax_t marks_set_গুনতি;
স্থির uintmax_t object_গুনতি_by_type[1 << TYPE_BITS];
স্থির uintmax_t duplicate_গুনতি_by_type[1 << TYPE_BITS];
স্থির uintmax_t delta_গুনতি_by_type[1 << TYPE_BITS];
স্থির uintmax_t delta_গুনতি_attempts_by_type[1 << TYPE_BITS];
স্থির unsigned long object_গুনতি;
স্থির unsigned long branch_গুনতি;
স্থির unsigned long branch_load_গুনতি;
স্থির পূর্ণসংখ্যা failure;
স্থির FILE *pack_edges;
স্থির পূর্ণসংখ্যা show_stats = 1;
স্থির পূর্ণসংখ্যা quiet;
স্থির গণনা sign_mode signed_tag_mode = SIGN_VERBATIM;
স্থির গণনা sign_mode signed_commit_mode = SIGN_VERBATIM;
স্থির ধ্রুবক অক্ষর *signed_commit_keyid;
স্থির ধ্রুবক অক্ষর *signed_tag_keyid;

/* Memory pools */
স্থির কাঠামো mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - আকার(কাঠামো mp_block),
};

/* Atom management */
স্থির unsigned পূর্ণসংখ্যা atom_table_sz = 4451;
স্থির unsigned পূর্ণসংখ্যা atom_cnt;
স্থির কাঠামো atom_str **atom_table;

/* The .pack file being generated */
স্থির কাঠামো pack_idx_option pack_idx_opts;
স্থির unsigned পূর্ণসংখ্যা pack_id;
স্থির কাঠামো hashfile *pack_file;
স্থির কাঠামো packed_git *pack_তথ্য;
স্থির কাঠামো packed_git **all_packs;
স্থির off_t pack_আকার;

/* Table of objects we've written. */
স্থির unsigned পূর্ণসংখ্যা object_entry_alloc = 5000;
স্থির কাঠামো object_entry_pool *blocks;
স্থির কাঠামো hashmap object_table;
স্থির কাঠামো mark_set *marks;
স্থির অক্ষর *export_marks_file;
স্থির অক্ষর *import_marks_file;
স্থির পূর্ণসংখ্যা import_marks_file_from_stream;
স্থির পূর্ণসংখ্যা import_marks_file_ignore_missing;
স্থির পূর্ণসংখ্যা import_marks_file_done;
স্থির পূর্ণসংখ্যা relative_marks_paths;

/* Our last blob */
স্থির কাঠামো last_object last_blob = {
	.তথ্য = STRBUF_INIT,
 };

/* Tree management */
স্থির unsigned পূর্ণসংখ্যা বৃক্ষ_entry_alloc = 1000;
স্থির শূন্য *avail_বৃক্ষ_entry;
স্থির unsigned পূর্ণসংখ্যা avail_বৃক্ষ_table_sz = 100;
স্থির কাঠামো avail_বৃক্ষ_content **avail_বৃক্ষ_table;
স্থির আকার_t বৃক্ষ_entry_allocd;
স্থির কাঠামো strbuf old_বৃক্ষ = STRBUF_INIT;
স্থির কাঠামো strbuf new_বৃক্ষ = STRBUF_INIT;

/* Branch data */
স্থির unsigned long max_active_branches = 5;
স্থির unsigned long cur_active_branches;
স্থির unsigned long branch_table_sz = 1039;
স্থির কাঠামো branch **branch_table;
স্থির কাঠামো branch *active_branches;

/* Tag data */
স্থির কাঠামো tag *first_tag;
স্থির কাঠামো tag *last_tag;

/* Input stream parsing */
স্থির whenspec_type whenspec = WHENSPEC_RAW;
স্থির কাঠামো strbuf command_buf = STRBUF_INIT;
স্থির পূর্ণসংখ্যা unread_command_buf;
স্থির কাঠামো recent_command cmd_hist = {
	.পূর্ব = &cmd_hist,
	.পরবর্তী = &cmd_hist,
};
স্থির কাঠামো recent_command *cmd_লেজ = &cmd_hist;
স্থির কাঠামো recent_command *rc_free;
স্থির unsigned পূর্ণসংখ্যা cmd_save = 100;
স্থির uintmax_t পরবর্তী_mark;
স্থির কাঠামো strbuf new_তথ্য = STRBUF_INIT;
স্থির পূর্ণসংখ্যা require_explicit_termination;

/* Signal handling */
স্থির অস্থির sig_atomic_t checkpoint_requested;

/* Submodule marks */
স্থির কাঠামো string_list sub_marks_from = STRING_LIST_INIT_DUP;
স্থির কাঠামো string_list sub_marks_to = STRING_LIST_INIT_DUP;
স্থির kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
স্থির পূর্ণসংখ্যা cat_blob_fd = STDOUT_FILENO;

/* Command state */
কাঠামো fast_import_state {
	পূর্ণসংখ্যা argc;
	ধ্রুবক অক্ষর **argv;
	ধ্রুবক অক্ষর *prefix;
	পূর্ণসংখ্যা seen_তথ্য_command;
	পূর্ণসংখ্যা allow_unsafe_features;
	কাঠামো option *option;
};

স্থির শূন্য fast_import_state_init(কাঠামো fast_import_state *state,
				   পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv,
				   ধ্রুবক অক্ষর *prefix, কাঠামো option *option)
{
	memset(state, 0, আকার(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

স্থির শূন্য parse_argv(কাঠামো fast_import_state *state);
স্থির শূন্য parse_get_mark(কাঠামো fast_import_state *state, ধ্রুবক অক্ষর *p);
স্থির শূন্য parse_cat_blob(কাঠামো fast_import_state *state, ধ্রুবক অক্ষর *p);
স্থির শূন্য parse_ls(কাঠামো fast_import_state *state,
		     ধ্রুবক অক্ষর *p,
		     কাঠামো branch *b);

স্থির শূন্য for_each_mark(কাঠামো mark_set *m, uintmax_t base, each_mark_fn_t callback, শূন্য *p)
{
	uintmax_t k;
	যদি (m->shift) {
		জন্য (k = 0; k < 1024; k++) {
			যদি (m->তথ্য.sets[k])
				for_each_mark(m->তথ্য.sets[k], base + (k << m->shift), callback, p);
		}
	} অন্যথা {
		জন্য (k = 0; k < 1024; k++) {
			যদি (m->তথ্য.marked[k])
				callback(base + k, m->তথ্য.marked[k], p);
		}
	}
}

স্থির শূন্য dump_marks_fn(uintmax_t mark, শূন্য *object, শূন্য *cbp) {
	কাঠামো object_entry *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

স্থির শূন্য write_branch_report(FILE *rpt, কাঠামো branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	যদি (b->active)
		fputs(" active", rpt);
	যদি (b->branch_বৃক্ষ.বৃক্ষ)
		fputs(" loaded", rpt);
	যদি (is_null_oid(&b->branch_বৃক্ষ.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_to_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_to_hex(&b->branch_বৃক্ষ.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_to_hex(&b->branch_বৃক্ষ.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->last_commit);

	fputs("  last pack   : ", rpt);
	যদি (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

স্থির শূন্য write_crash_report(ধ্রুবক অক্ষর *err)
{
	অক্ষর *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	কাঠামো branch *b;
	unsigned long lu;
	কাঠামো recent_command *rc;

	যদি (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		মুক্ত(loc);
		ফেরত;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(time(ফাঁকা), 0, DATE_MODE(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	জন্য (rc = cmd_hist.পরবর্তী; rc != &cmd_hist; rc = rc->পরবর্তী) {
		যদি (rc->পরবর্তী == &cmd_hist)
			fputs("* ", rpt);
		অন্যথা
			fputs("  ", rpt);
		fputs(rc->buf, rpt);
		fputc('\n', rpt);
	}

	fputc('\n', rpt);
	fputs("Active Branch LRU\n", rpt);
	fputs("-----------------\n", rpt);
	fprintf(rpt, "    active_branches = %lu cur, %lu max\n",
		cur_active_branches,
		max_active_branches);
	fputc('\n', rpt);
	fputs("  pos  clock name\n", rpt);
	fputs("  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", rpt);
	জন্য (b = active_branches, lu = 0; b; b = b->active_পরবর্তী_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	জন্য (lu = 0; lu < branch_table_sz; lu++) {
		জন্য (b = branch_table[lu]; b; b = b->table_পরবর্তী_branch)
			write_branch_report(rpt, b);
	}

	যদি (first_tag) {
		কাঠামো tag *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		জন্য (tg = first_tag; tg; tg = tg->পরবর্তী_tag) {
			fputs(oid_to_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	যদি (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	অন্যথা
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	মুক্ত(loc);
}

স্থির শূন্য end_packfile(শূন্য);
স্থির শূন্য unkeep_all_packs(শূন্য);
স্থির শূন্য dump_marks(শূন্য);

স্থির NORETURN শূন্য die_nicely(ধ্রুবক অক্ষর *err, va_list params)
{
	va_list cp;
	স্থির পূর্ণসংখ্যা zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	যদি (!zombie) {
		অক্ষর message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, আকার(message), err, cp);
		write_crash_report(message);
		end_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	exit(128);
}

#ifndef SIGUSR1	/* Windows, for example */

স্থির শূন্য set_checkpoint_signal(শূন্য)
{
}

#অন্যথা

স্থির শূন্য checkpoint_signal(পূর্ণসংখ্যা signo UNUSED)
{
	checkpoint_requested = 1;
}

স্থির শূন্য set_checkpoint_signal(শূন্য)
{
	কাঠামো sigaction sa;

	memset(&sa, 0, আকার(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, ফাঁকা);
}

#endif

স্থির শূন্য alloc_objects(unsigned পূর্ণসংখ্যা cnt)
{
	কাঠামো object_entry_pool *b;

	b = xmalloc(আকার(কাঠামো object_entry_pool)
		+ cnt * আকার(কাঠামো object_entry));
	b->পরবর্তী_pool = blocks;
	b->পরবর্তী_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_গুনতি += cnt;
}

স্থির কাঠামো object_entry *new_object(কাঠামো object_id *oid)
{
	কাঠামো object_entry *e;

	যদি (blocks->পরবর্তী_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->পরবর্তী_free++;
	oidcpy(&e->idx.oid, oid);
	ফেরত e;
}

স্থির কাঠামো object_entry *find_object(কাঠামো object_id *oid)
{
	ফেরত hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   কাঠামো object_entry, ent);
}

স্থির কাঠামো object_entry *ঢোকাও_object(কাঠামো object_id *oid)
{
	কাঠামো object_entry *e;
	unsigned পূর্ণসংখ্যা hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					কাঠামো object_entry, ent);
	যদি (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	ফেরত e;
}

স্থির শূন্য invalidate_pack_id(unsigned পূর্ণসংখ্যা id)
{
	unsigned long lu;
	কাঠামো tag *t;
	কাঠামো hashmap_iter iter;
	কাঠামো object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		যদি (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	জন্য (lu = 0; lu < branch_table_sz; lu++) {
		কাঠামো branch *b;

		জন্য (b = branch_table[lu]; b; b = b->table_পরবর্তী_branch)
			যদি (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	জন্য (t = first_tag; t; t = t->পরবর্তী_tag)
		যদি (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

স্থির unsigned পূর্ণসংখ্যা hc_str(ধ্রুবক অক্ষর *s, আকার_t len)
{
	unsigned পূর্ণসংখ্যা r = 0;
	যখন (len-- > 0)
		r = r * 31 + *s++;
	ফেরত r;
}

স্থির শূন্য ঢোকাও_mark(কাঠামো mark_set **শীর্ষ_2, uintmax_t idnum, কাঠামো object_entry *oe)
{
	কাঠামো mark_set *s = *শীর্ষ_2;

	যখন ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, আকার(কাঠামো mark_set));
		s->shift = (*শীর্ষ_2)->shift + 10;
		s->তথ্য.sets[0] = *শীর্ষ_2;
		*শীর্ষ_2 = s;
	}
	যখন (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		যদি (!s->তথ্য.sets[i]) {
			s->তথ্য.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, আকার(কাঠামো mark_set));
			s->তথ্য.sets[i]->shift = s->shift - 10;
		}
		s = s->তথ্য.sets[i];
	}
	যদি (!s->তথ্য.marked[idnum])
		marks_set_গুনতি++;
	s->তথ্য.marked[idnum] = oe;
}

স্থির শূন্য *find_mark(কাঠামো mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	কাঠামো object_entry *oe = ফাঁকা;
	যদি ((idnum >> s->shift) < 1024) {
		যখন (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->তথ্য.sets[i];
		}
		যদি (s)
			oe = s->তথ্য.marked[idnum];
	}
	যদি (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	ফেরত oe;
}

স্থির কাঠামো atom_str *to_atom(ধ্রুবক অক্ষর *s, unsigned short len)
{
	unsigned পূর্ণসংখ্যা hc = hc_str(s, len) % atom_table_sz;
	কাঠামো atom_str *c;

	জন্য (c = atom_table[hc]; c; c = c->পরবর্তী_atom)
		যদি (c->str_len == len && !strncmp(s, c->str_dat, len))
			ফেরত c;

	c = mem_pool_alloc(&fi_mem_pool, আকার(কাঠামো atom_str) + len + 1);
	c->str_len = len;
	memcpy(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->পরবর্তী_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	ফেরত c;
}

স্থির কাঠামো branch *lookup_branch(ধ্রুবক অক্ষর *name)
{
	unsigned পূর্ণসংখ্যা hc = hc_str(name, strlen(name)) % branch_table_sz;
	কাঠামো branch *b;

	জন্য (b = branch_table[hc]; b; b = b->table_পরবর্তী_branch)
		যদি (!strcmp(name, b->name))
			ফেরত b;
	ফেরত ফাঁকা;
}

স্থির কাঠামো branch *new_branch(ধ্রুবক অক্ষর *name)
{
	unsigned পূর্ণসংখ্যা hc = hc_str(name, strlen(name)) % branch_table_sz;
	কাঠামো branch *b = lookup_branch(name);

	যদি (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	যদি (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, আকার(কাঠামো branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_পরবর্তী_branch = branch_table[hc];
	b->branch_বৃক্ষ.versions[0].mode = S_IFDIR;
	b->branch_বৃক্ষ.versions[1].mode = S_IFDIR;
	b->অঙ্ক_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_গুনতি++;
	ফেরত b;
}

স্থির unsigned পূর্ণসংখ্যা hc_entries(unsigned পূর্ণসংখ্যা cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	ফেরত cnt < avail_বৃক্ষ_table_sz ? cnt : avail_বৃক্ষ_table_sz - 1;
}

স্থির কাঠামো বৃক্ষ_content *new_বৃক্ষ_content(unsigned পূর্ণসংখ্যা cnt)
{
	কাঠামো avail_বৃক্ষ_content *f, *l = ফাঁকা;
	কাঠামো বৃক্ষ_content *t;
	unsigned পূর্ণসংখ্যা hc = hc_entries(cnt);

	জন্য (f = avail_বৃক্ষ_table[hc]; f; l = f, f = f->পরবর্তী_avail)
		যদি (f->entry_capacity >= cnt)
			বিরতি;

	যদি (f) {
		যদি (l)
			l->পরবর্তী_avail = f->পরবর্তী_avail;
		অন্যথা
			avail_বৃক্ষ_table[hc] = f->পরবর্তী_avail;
	} অন্যথা {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, আকার(*t) + আকার(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (কাঠামো বৃক্ষ_content*)f;
	t->entry_গুনতি = 0;
	t->delta_depth = 0;
	ফেরত t;
}

স্থির শূন্য release_বৃক্ষ_entry(কাঠামো বৃক্ষ_entry *e);
স্থির শূন্য release_বৃক্ষ_content(কাঠামো বৃক্ষ_content *t)
{
	কাঠামো avail_বৃক্ষ_content *f = (কাঠামো avail_বৃক্ষ_content*)t;
	unsigned পূর্ণসংখ্যা hc = hc_entries(f->entry_capacity);
	f->পরবর্তী_avail = avail_বৃক্ষ_table[hc];
	avail_বৃক্ষ_table[hc] = f;
}

স্থির শূন্য release_বৃক্ষ_content_recursive(কাঠামো বৃক্ষ_content *t)
{
	unsigned পূর্ণসংখ্যা i;
	জন্য (i = 0; i < t->entry_গুনতি; i++)
		release_বৃক্ষ_entry(t->entries[i]);
	release_বৃক্ষ_content(t);
}

স্থির কাঠামো বৃক্ষ_content *grow_বৃক্ষ_content(
	কাঠামো বৃক্ষ_content *t,
	পূর্ণসংখ্যা amt)
{
	কাঠামো বৃক্ষ_content *r = new_বৃক্ষ_content(t->entry_গুনতি + amt);
	r->entry_গুনতি = t->entry_গুনতি;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_গুনতি);
	release_বৃক্ষ_content(t);
	ফেরত r;
}

স্থির কাঠামো বৃক্ষ_entry *new_বৃক্ষ_entry(শূন্য)
{
	কাঠামো বৃক্ষ_entry *e;

	যদি (!avail_বৃক্ষ_entry) {
		unsigned পূর্ণসংখ্যা n = বৃক্ষ_entry_alloc;
		বৃক্ষ_entry_allocd += n * আকার(কাঠামো বৃক্ষ_entry);
		ALLOC_ARRAY(e, n);
		avail_বৃক্ষ_entry = e;
		যখন (n-- > 1) {
			*((শূন্য**)e) = e + 1;
			e++;
		}
		*((শূন্য**)e) = ফাঁকা;
	}

	e = avail_বৃক্ষ_entry;
	avail_বৃক্ষ_entry = *((শূন্য**)e);
	ফেরত e;
}

স্থির শূন্য release_বৃক্ষ_entry(কাঠামো বৃক্ষ_entry *e)
{
	যদি (e->বৃক্ষ)
		release_বৃক্ষ_content_recursive(e->বৃক্ষ);
	*((শূন্য**)e) = avail_বৃক্ষ_entry;
	avail_বৃক্ষ_entry = e;
}

স্থির কাঠামো বৃক্ষ_content *dup_বৃক্ষ_content(কাঠামো বৃক্ষ_content *s)
{
	কাঠামো বৃক্ষ_content *d;
	কাঠামো বৃক্ষ_entry *a, *b;
	unsigned পূর্ণসংখ্যা i;

	যদি (!s)
		ফেরত ফাঁকা;
	d = new_বৃক্ষ_content(s->entry_গুনতি);
	জন্য (i = 0; i < s->entry_গুনতি; i++) {
		a = s->entries[i];
		b = new_বৃক্ষ_entry();
		memcpy(b, a, আকার(*a));
		যদি (a->বৃক্ষ && is_null_oid(&b->versions[1].oid))
			b->বৃক্ষ = dup_বৃক্ষ_content(a->বৃক্ষ);
		অন্যথা
			b->বৃক্ষ = ফাঁকা;
		d->entries[i] = b;
	}
	d->entry_গুনতি = s->entry_গুনতি;
	d->delta_depth = s->delta_depth;

	ফেরত d;
}

স্থির শূন্য start_packfile(শূন্য)
{
	কাঠামো strbuf tmp_file = STRBUF_INIT;
	কাঠামো packed_git *p;
	পূর্ণসংখ্যা pack_fd;

	pack_fd = odb_mkstemp(the_repository->objects, &tmp_file,
			      "pack/tmp_pack_XXXXXX");
	FLEX_ALLOC_STR(p, pack_name, tmp_file.buf);
	strbuf_release(&tmp_file);

	p->pack_fd = pack_fd;
	p->do_not_close = 1;
	p->repo = the_repository;
	pack_file = hashfd(the_repository->hash_algo, pack_fd, p->pack_name);

	pack_তথ্য = p;
	pack_আকার = write_pack_header(pack_file, 0);
	object_গুনতি = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

স্থির ধ্রুবক অক্ষর *create_সূচক(শূন্য)
{
	ধ্রুবক অক্ষর *tmpfile;
	কাঠামো pack_idx_entry **idx, **c, **last;
	কাঠামো object_entry *e;
	কাঠামো object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_গুনতি);
	c = idx;
	জন্য (o = blocks; o; o = o->পরবর্তী_pool)
		জন্য (e = o->পরবর্তী_free; e-- != o->entries;)
			যদি (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_গুনতি;
	যদি (c != last)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, ফাঁকা, idx, object_গুনতি,
				 &pack_idx_opts, pack_তথ্য->hash);
	মুক্ত(idx);
	ফেরত tmpfile;
}

স্থির অক্ষর *keep_pack(ধ্রুবক অক্ষর *curr_সূচক_name)
{
	স্থির ধ্রুবক অক্ষর *keep_বার্তা = "fast-import";
	কাঠামো strbuf name = STRBUF_INIT;
	পূর্ণসংখ্যা keep_fd;

	odb_pack_name(pack_তথ্য->repo, &name, pack_তথ্য->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_তথ্য->repo,
							    name.buf);
	যদি (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_বার্তা, strlen(keep_বার্তা));
	যদি (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_তথ্য->repo, &name, pack_তথ্য->hash, "pack");
	যদি (finalize_object_file(pack_তথ্য->repo, pack_তথ্য->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_তথ্য->repo, &name, pack_তথ্য->hash, "idx");
	যদি (finalize_object_file(pack_তথ্য->repo, curr_সূচক_name, name.buf))
		die(_("cannot store index file"));
	মুক্ত((শূন্য *)curr_সূচক_name);
	ফেরত strbuf_detach(&name, ফাঁকা);
}

স্থির শূন্য unkeep_all_packs(শূন্য)
{
	কাঠামো strbuf name = STRBUF_INIT;
	পূর্ণসংখ্যা k;

	জন্য (k = 0; k < pack_id; k++) {
		কাঠামো packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

স্থির পূর্ণসংখ্যা loosen_small_pack(ধ্রুবক কাঠামো packed_git *p)
{
	কাঠামো child_process unpack = CHILD_PROCESS_INIT;

	যদি (lseek(p->pack_fd, 0, SEEK_SET) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_ঠেলো(&unpack.args, "unpack-objects");
	যদি (!show_stats)
		strvec_ঠেলো(&unpack.args, "-q");

	ফেরত run_command(&unpack);
}

স্থির শূন্য end_packfile(শূন্য)
{
	স্থির পূর্ণসংখ্যা running;

	যদি (running || !pack_তথ্য)
		ফেরত;

	running = 1;
	clear_delta_base_cache();
	যদি (object_গুনতি) {
		কাঠামো odb_source_files *files = odb_source_files_downcast(pack_তথ্য->repo->objects->source);
		কাঠামো packed_git *new_p;
		কাঠামো object_id cur_pack_oid;
		অক্ষর *idx_name;
		পূর্ণসংখ্যা i;
		কাঠামো branch *b;
		কাঠামো tag *t;

		close_pack_windows(pack_তথ্য);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_তথ্য->pack_fd,
					 pack_তথ্য->hash, pack_তথ্য->pack_name,
					 object_গুনতি, cur_pack_oid.hash,
					 pack_আকার);

		যদি (object_গুনতি <= unpack_limit) {
			যদি (!loosen_small_pack(pack_তথ্য)) {
				invalidate_pack_id(pack_id);
				যাও discard_pack;
			}
		}

		close(pack_তথ্য->pack_fd);
		idx_name = keep_pack(create_সূচক());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		যদি (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		মুক্ত(idx_name);

		/* Print the boundary */
		যদি (pack_edges) {
			fprintf(pack_edges, "%s:", new_p->pack_name);
			জন্য (i = 0; i < branch_table_sz; i++) {
				জন্য (b = branch_table[i]; b; b = b->table_পরবর্তী_branch) {
					যদি (b->pack_id == pack_id)
						fprintf(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			জন্য (t = first_tag; t; t = t->পরবর্তী_tag) {
				যদি (t->pack_id == pack_id)
					fprintf(pack_edges, " %s",
						oid_to_hex(&t->oid));
			}
			fputc('\n', pack_edges);
			fflush(pack_edges);
		}

		pack_id++;
	}
	অন্যথা {
discard_pack:
		close(pack_তথ্য->pack_fd);
		unlink_or_warn(pack_তথ্য->pack_name);
	}
	FREE_AND_NULL(pack_তথ্য);
	running = 0;

	/* We can't carry a delta across packfiles. */
	strbuf_release(&last_blob.তথ্য);
	last_blob.offset = 0;
	last_blob.depth = 0;
}

স্থির শূন্য cycle_packfile(শূন্য)
{
	end_packfile();
	start_packfile();
}

স্থির পূর্ণসংখ্যা store_object(
	গণনা object_type type,
	কাঠামো strbuf *dat,
	কাঠামো last_object *last,
	কাঠামো object_id *oidout,
	uintmax_t mark)
{
	কাঠামো odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	শূন্য *out, *delta;
	কাঠামো object_entry *e;
	unsigned অক্ষর hdr[96];
	কাঠামো object_id oid;
	unsigned long hdrlen, deltalen = 0;
	কাঠামো git_hash_ctx c;
	git_zstream s;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	hdrlen = format_object_header((অক্ষর *)hdr, আকার(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	যদি (oidout)
		oidcpy(oidout, &oid);

	e = ঢোকাও_object(&oid);
	যদি (mark)
		ঢোকাও_mark(&marks, mark, e);
	যদি (e->idx.offset) {
		duplicate_গুনতি_by_type[type]++;
		ফেরত 1;
	}

	জন্য (কাঠামো odb_files_dir *dir = files->dirs; dir; dir = dir->পরবর্তী) {
		যদি (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			চালু;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_গুনতি_by_type[type]++;
		ফেরত 1;
	}

	যদি (last && last->তথ্য.len && last->তথ্য.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		আকার_t deltalen_st;

		delta_গুনতি_attempts_by_type[type]++;
		delta = diff_delta(last->তথ্য.buf, last->তথ্য.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_আকার_t_to_ulong(deltalen_st);
	} অন্যথা
		delta = ফাঁকা;

	git_deflate_init(&s, cfg->pack_compression_level);
	যদি (delta) {
		s.পরবর্তী_in = delta;
		s.avail_in = deltalen;
	} অন্যথা {
		s.পরবর্তী_in = (শূন্য *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.পরবর্তী_out = out = xmalloc(s.avail_out);
	যখন (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	যদি ((max_packsize
		&& (pack_আকার + PACK_আকার_THRESHOLD + s.যোগ_out) > max_packsize)
		|| (pack_আকার + PACK_আকার_THRESHOLD + s.যোগ_out) < pack_আকার) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		যদি (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.পরবর্তী_in = (শূন্য *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.পরবর্তী_out = out = xrealloc(out, s.avail_out);
			যখন (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_end(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.offset = pack_আকার;
	object_গুনতি++;
	object_গুনতি_by_type[type]++;

	crc32_begin(pack_file);

	যদি (delta) {
		off_t ofs = e->idx.offset - last->offset;
		unsigned pos = আকার(hdr) - 1;

		delta_গুনতি_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, আকার(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_আকার += hdrlen;

		hdr[pos] = ofs & 127;
		যখন (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, আকার(hdr) - pos);
		pack_আকার += আকার(hdr) - pos;
	} অন্যথা {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, আকার(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_আকার += hdrlen;
	}

	hashwrite(pack_file, out, s.যোগ_out);
	pack_আকার += s.যোগ_out;

	e->idx.crc32 = crc32_end(pack_file);

	মুক্ত(out);
	মুক্ত(delta);
	যদি (last) {
		যদি (last->no_swap) {
			last->তথ্য = *dat;
		} অন্যথা {
			strbuf_swap(&last->তথ্য, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	ফেরত 0;
}

স্থির শূন্য truncate_pack(কাঠামো hashfile_checkpoint *checkpoint)
{
	যদি (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_আকার = checkpoint->offset;
}

স্থির শূন্য stream_blob(uintmax_t len, কাঠামো object_id *oidout, uintmax_t mark)
{
	কাঠামো odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	আকার_t in_sz = 64 * 1024, out_sz = 64 * 1024;
	unsigned অক্ষর *in_buf = xmalloc(in_sz);
	unsigned অক্ষর *out_buf = xmalloc(out_sz);
	কাঠামো object_entry *e;
	কাঠামো object_id oid;
	unsigned long hdrlen;
	off_t offset;
	কাঠামো git_hash_ctx c;
	git_zstream s;
	কাঠামো hashfile_checkpoint checkpoint;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	পূর্ণসংখ্যা status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	যদি ((max_packsize
		&& (pack_আকার + PACK_আকার_THRESHOLD + len) > max_packsize)
		|| (pack_আকার + PACK_আকার_THRESHOLD + len) < pack_আকার)
		cycle_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	offset = checkpoint.offset;

	hdrlen = format_object_header((অক্ষর *)out_buf, out_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, out_buf, hdrlen);

	crc32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_in_pack_object_header(out_buf, out_sz, OBJ_BLOB, len);

	s.পরবর্তী_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	যখন (status != Z_STREAM_END) {
		যদি (0 < len && !s.avail_in) {
			আকার_t cnt = in_sz < len ? in_sz : (আকার_t)len;
			আকার_t n = fread(in_buf, 1, cnt, stdin);
			যদি (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.পরবর্তী_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		যদি (!s.avail_out || status == Z_STREAM_END) {
			আকার_t n = s.পরবর্তী_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_আকার += n;
			s.পরবর্তী_out = out_buf;
			s.avail_out = out_sz;
		}

		নির্বাচন (status) {
		ক্ষেত্র Z_OK:
		ক্ষেত্র Z_BUF_ERROR:
		ক্ষেত্র Z_STREAM_END:
			চালু;
		পূর্বনির্ধারিত:
			die(_("unexpected deflate failure: %d"), status);
		}
	}
	git_deflate_end(&s);
	git_hash_final_oid(&oid, &c);

	যদি (oidout)
		oidcpy(oidout, &oid);

	e = ঢোকাও_object(&oid);

	যদি (mark)
		ঢোকাও_mark(&marks, mark, e);

	যদি (e->idx.offset) {
		duplicate_গুনতি_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		যাও out;
	}

	জন্য (কাঠামো odb_files_dir *dir = files->dirs; dir; dir = dir->পরবর্তী) {
		যদি (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			চালু;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_গুনতি_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		যাও out;
	}

	e->depth = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.offset = offset;
	e->idx.crc32 = crc32_end(pack_file);
	object_গুনতি++;
	object_গুনতি_by_type[OBJ_BLOB]++;

out:
	মুক্ত(in_buf);
	মুক্ত(out_buf);
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
স্থির শূন্য *gfi_unpack_entry(
	কাঠামো object_entry *oe,
	unsigned long *sizep)
{
	গণনা object_type type;
	আকার_t আকার_st = 0;
	শূন্য *তথ্য;
	কাঠামো packed_git *p = all_packs[oe->pack_id];
	যদি (p == pack_তথ্য && p->pack_আকার < (pack_আকার + the_hash_algo->rawsz)) {
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
		p->pack_আকার = pack_আকার + the_hash_algo->rawsz;
	}
	তথ্য = unpack_entry(the_repository, p, oe->idx.offset, &type, &আকার_st);
	যদি (sizep)
		*sizep = cast_আকার_t_to_ulong(আকার_st);
	ফেরত তথ্য;
}

স্থির শূন্য load_বৃক্ষ(কাঠামো বৃক্ষ_entry *মূল)
{
	কাঠামো object_id *oid = &মূল->versions[1].oid;
	কাঠামো object_entry *myoe;
	কাঠামো বৃক্ষ_content *t;
	unsigned long আকার_2;
	অক্ষর *buf;
	ধ্রুবক অক্ষর *c;

	মূল->বৃক্ষ = t = new_বৃক্ষ_content(8);
	যদি (is_null_oid(oid))
		ফেরত;

	myoe = find_object(oid);
	যদি (myoe && myoe->pack_id != MAX_PACK_ID) {
		যদি (myoe->type != OBJ_বৃক্ষ)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &আকার_2);
		যদি (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} অন্যথা {
		গণনা object_type type;
		আকার_t আকার_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &আকার_st);
		আকার_2 = cast_আকার_t_to_ulong(আকার_st);
		যদি (!buf || type != OBJ_বৃক্ষ)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	যখন (c != (buf + আকার_2)) {
		কাঠামো বৃক্ষ_entry *e = new_বৃক্ষ_entry();

		যদি (t->entry_গুনতি == t->entry_capacity)
			মূল->বৃক্ষ = t = grow_বৃক্ষ_content(t, t->entry_গুনতি);
		t->entries[t->entry_গুনতি++] = e;

		e->বৃক্ষ = ফাঁকা;
		c = parse_mode(c, &e->versions[1].mode);
		যদি (!c)
			die(_("corrupt mode in %s"), oid_to_hex(oid));
		e->versions[0].mode = e->versions[1].mode;
		e->name = to_atom(c, strlen(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (unsigned অক্ষর *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (unsigned অক্ষর *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	মুক্ত(buf);
}

স্থির পূর্ণসংখ্যা tecmp0 (ধ্রুবক শূন্য *_a, ধ্রুবক শূন্য *_b)
{
	কাঠামো বৃক্ষ_entry *a = *((কাঠামো বৃক্ষ_entry**)_a);
	কাঠামো বৃক্ষ_entry *b = *((কাঠামো বৃক্ষ_entry**)_b);
	ফেরত base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

স্থির পূর্ণসংখ্যা tecmp1 (ধ্রুবক শূন্য *_a, ধ্রুবক শূন্য *_b)
{
	কাঠামো বৃক্ষ_entry *a = *((কাঠামো বৃক্ষ_entry**)_a);
	কাঠামো বৃক্ষ_entry *b = *((কাঠামো বৃক্ষ_entry**)_b);
	ফেরত base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

স্থির শূন্য mktree(কাঠামো বৃক্ষ_content *t, পূর্ণসংখ্যা v, কাঠামো strbuf *b)
{
	আকার_t maxlen = 0;
	unsigned পূর্ণসংখ্যা i;

	যদি (!v)
		QSORT(t->entries, t->entry_গুনতি, tecmp0);
	অন্যথা
		QSORT(t->entries, t->entry_গুনতি, tecmp1);

	জন্য (i = 0; i < t->entry_গুনতি; i++) {
		যদি (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	জন্য (i = 0; i < t->entry_গুনতি; i++) {
		কাঠামো বৃক্ষ_entry *e = t->entries[i];
		যদি (!e->versions[v].mode)
			চালু;
		strbuf_addf(b, "%o %s%c",
			(unsigned পূর্ণসংখ্যা)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

স্থির শূন্য store_বৃক্ষ(কাঠামো বৃক্ষ_entry *মূল)
{
	কাঠামো বৃক্ষ_content *t;
	unsigned পূর্ণসংখ্যা i, j, del;
	কাঠামো last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	কাঠামো object_entry *le = ফাঁকা;

	যদি (!is_null_oid(&মূল->versions[1].oid))
		ফেরত;

	যদি (!মূল->বৃক্ষ)
		load_বৃক্ষ(মূল);
	t = মূল->বৃক্ষ;

	জন্য (i = 0; i < t->entry_গুনতি; i++) {
		যদি (t->entries[i]->বৃক্ষ)
			store_বৃক্ষ(t->entries[i]);
	}

	যদি (!(মূল->versions[0].mode & NO_DELTA))
		le = find_object(&মূল->versions[0].oid);
	যদি (S_ISDIR(মূল->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_বৃক্ষ);
		lo.তথ্য = old_বৃক্ষ;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_বৃক্ষ);
	store_object(OBJ_বৃক্ষ, &new_বৃক্ষ, &lo, &মূল->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	জন্য (i = 0, j = 0, del = 0; i < t->entry_গুনতি; i++) {
		কাঠামো বৃক্ষ_entry *e = t->entries[i];
		যদি (e->versions[1].mode) {
			e->versions[0].mode = e->versions[1].mode;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} অন্যথা {
			release_বৃক্ষ_entry(e);
			del++;
		}
	}
	t->entry_গুনতি -= del;
}

স্থির শূন্য বৃক্ষ_content_replace(
	কাঠামো বৃক্ষ_entry *মূল,
	ধ্রুবক কাঠামো object_id *oid,
	ধ্রুবক uint16_t mode,
	কাঠামো বৃক্ষ_content *newtree)
{
	যদি (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&মূল->versions[0].oid, the_repository->hash_algo);
	oidcpy(&মূল->versions[1].oid, oid);
	যদি (মূল->বৃক্ষ)
		release_বৃক্ষ_content_recursive(মূল->বৃক্ষ);
	মূল->বৃক্ষ = newtree;
}

স্থির পূর্ণসংখ্যা বৃক্ষ_content_set(
	কাঠামো বৃক্ষ_entry *মূল,
	ধ্রুবক অক্ষর *p,
	ধ্রুবক কাঠামো object_id *oid,
	ধ্রুবক uint16_t mode,
	কাঠামো বৃক্ষ_content *subtree)
{
	কাঠামো বৃক্ষ_content *t;
	ধ্রুবক অক্ষর *slash1;
	unsigned পূর্ণসংখ্যা i, n;
	কাঠামো বৃক্ষ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	যদি (!n)
		die(_("empty path component found in input"));
	যদি (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	যদি (!মূল->বৃক্ষ)
		load_বৃক্ষ(মূল);
	t = মূল->বৃক্ষ;
	জন্য (i = 0; i < t->entry_গুনতি; i++) {
		e = t->entries[i];
		যদি (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			যদি (!*slash1) {
				যদি (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					ফেরত 0;
				e->versions[1].mode = mode;
				oidcpy(&e->versions[1].oid, oid);
				যদি (e->বৃক্ষ)
					release_বৃক্ষ_content_recursive(e->বৃক্ষ);
				e->বৃক্ষ = subtree;

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
				যদি (S_ISDIR(e->versions[0].mode))
					e->versions[0].mode |= NO_DELTA;

				oidclr(&মূল->versions[1].oid, the_repository->hash_algo);
				ফেরত 1;
			}
			যদি (!S_ISDIR(e->versions[1].mode)) {
				e->বৃক্ষ = new_বৃক্ষ_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			যদি (!e->বৃক্ষ)
				load_বৃক্ষ(e);
			যদি (বৃক্ষ_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&মূল->versions[1].oid, the_repository->hash_algo);
				ফেরত 1;
			}
			ফেরত 0;
		}
	}

	যদি (t->entry_গুনতি == t->entry_capacity)
		মূল->বৃক্ষ = t = grow_বৃক্ষ_content(t, t->entry_গুনতি);
	e = new_বৃক্ষ_entry();
	e->name = to_atom(p, n);
	e->versions[0].mode = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->entry_গুনতি++] = e;
	যদি (*slash1) {
		e->বৃক্ষ = new_বৃক্ষ_content(8);
		e->versions[1].mode = S_IFDIR;
		বৃক্ষ_content_set(e, slash1 + 1, oid, mode, subtree);
	} অন্যথা {
		e->বৃক্ষ = subtree;
		e->versions[1].mode = mode;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&মূল->versions[1].oid, the_repository->hash_algo);
	ফেরত 1;
}

স্থির পূর্ণসংখ্যা বৃক্ষ_content_remove(
	কাঠামো বৃক্ষ_entry *মূল,
	ধ্রুবক অক্ষর *p,
	কাঠামো বৃক্ষ_entry *backup_leaf,
	পূর্ণসংখ্যা allow_মূল)
{
	কাঠামো বৃক্ষ_content *t;
	ধ্রুবক অক্ষর *slash1;
	unsigned পূর্ণসংখ্যা i, n;
	কাঠামো বৃক্ষ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	যদি (!মূল->বৃক্ষ)
		load_বৃক্ষ(মূল);

	যদি (!*p && allow_মূল) {
		e = মূল;
		যাও del_entry;
	}

	t = মূল->বৃক্ষ;
	জন্য (i = 0; i < t->entry_গুনতি; i++) {
		e = t->entries[i];
		যদি (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			যদি (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				ফেরত 1;
			যদি (!*slash1 || !S_ISDIR(e->versions[1].mode))
				যাও del_entry;
			যদি (!e->বৃক্ষ)
				load_বৃক্ষ(e);
			যদি (বৃক্ষ_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				জন্য (n = 0; n < e->বৃক্ষ->entry_গুনতি; n++) {
					যদি (e->বৃক্ষ->entries[n]->versions[1].mode) {
						oidclr(&মূল->versions[1].oid,
						       the_repository->hash_algo);
						ফেরত 1;
					}
				}
				backup_leaf = ফাঁকা;
				যাও del_entry;
			}
			ফেরত 0;
		}
	}
	ফেরত 0;

del_entry:
	যদি (backup_leaf)
		memcpy(backup_leaf, e, আকার(*backup_leaf));
	অন্যথা যদি (e->বৃক্ষ)
		release_বৃক্ষ_content_recursive(e->বৃক্ষ);
	e->বৃক্ষ = ফাঁকা;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&মূল->versions[1].oid, the_repository->hash_algo);
	ফেরত 1;
}

স্থির পূর্ণসংখ্যা বৃক্ষ_content_get(
	কাঠামো বৃক্ষ_entry *মূল,
	ধ্রুবক অক্ষর *p,
	কাঠামো বৃক্ষ_entry *leaf,
	পূর্ণসংখ্যা allow_মূল)
{
	কাঠামো বৃক্ষ_content *t;
	ধ্রুবক অক্ষর *slash1;
	unsigned পূর্ণসংখ্যা i, n;
	কাঠামো বৃক্ষ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	যদি (!n && !allow_মূল)
		die(_("empty path component found in input"));

	যদি (!মূল->বৃক্ষ)
		load_বৃক্ষ(মূল);

	যদি (!n) {
		e = মূল;
		যাও found_entry;
	}

	t = মূল->বৃক্ষ;
	জন্য (i = 0; i < t->entry_গুনতি; i++) {
		e = t->entries[i];
		যদি (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			যদি (!*slash1)
				যাও found_entry;
			যদি (!S_ISDIR(e->versions[1].mode))
				ফেরত 0;
			যদি (!e->বৃক্ষ)
				load_বৃক্ষ(e);
			ফেরত বৃক্ষ_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	ফেরত 0;

found_entry:
	memcpy(leaf, e, আকার(*leaf));
	যদি (e->বৃক্ষ && is_null_oid(&e->versions[1].oid))
		leaf->বৃক্ষ = dup_বৃক্ষ_content(e->বৃক্ষ);
	অন্যথা
		leaf->বৃক্ষ = ফাঁকা;
	ফেরত 1;
}

স্থির পূর্ণসংখ্যা update_branch(কাঠামো branch *b)
{
	স্থির ধ্রুবক অক্ষর *বার্তা = "fast-import";
	কাঠামো ref_transaction *transaction;
	কাঠামো object_id old_oid;
	কাঠামো strbuf err = STRBUF_INIT;
	স্থির ধ্রুবক অক্ষর *replace_prefix = "refs/replace/";

	যদি (starts_with(b->name, replace_prefix) &&
	    !strcmp(b->name + strlen(replace_prefix),
		    oid_to_hex(&b->oid))) {
		যদি (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				ফাঁকা, b->name, ফাঁকা, 0);
		ফেরত 0;
	}
	যদি (is_null_oid(&b->oid)) {
		যদি (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					ফাঁকা, b->name, ফাঁকা, 0);
		ফেরত 0;
	}
	যদি (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	যদি (!force_update && !is_null_oid(&old_oid)) {
		কাঠামো commit *old_cmit, *new_cmit;
		পূর্ণসংখ্যা ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		যদি (!old_cmit || !new_cmit)
			ফেরত error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		যদি (ret < 0)
			exit(128);
		যদি (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			ফেরত -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	যদি (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   ফাঁকা, ফাঁকা, 0, বার্তা, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		ফেরত -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ফেরত 0;
}

স্থির শূন্য dump_branches(শূন্য)
{
	unsigned পূর্ণসংখ্যা i;
	কাঠামো branch *b;

	জন্য (i = 0; i < branch_table_sz; i++) {
		জন্য (b = branch_table[i]; b; b = b->table_পরবর্তী_branch)
			failure |= update_branch(b);
	}
}

স্থির শূন্য dump_tags(শূন্য)
{
	স্থির ধ্রুবক অক্ষর *বার্তা = "fast-import";
	কাঠামো tag *t;
	কাঠামো strbuf ref_name = STRBUF_INIT;
	কাঠামো strbuf err = STRBUF_INIT;
	কাঠামো ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	যদি (!transaction) {
		failure |= error("%s", err.buf);
		যাও cleanup;
	}
	জন্য (t = first_tag; t; t = t->পরবর্তী_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		যদি (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, ফাঁকা, ফাঁকা, ফাঁকা,
					   0, বার্তা, &err)) {
			failure |= error("%s", err.buf);
			যাও cleanup;
		}
	}
	যদি (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

স্থির শূন্য dump_marks(শূন্য)
{
	কাঠামো lock_file mark_lock = LOCK_INIT;
	FILE *f;

	যদি (!export_marks_file || (import_marks_file && !import_marks_file_done))
		ফেরত;

	যদি (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		ফেরত;
	}

	যদি (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		ফেরত;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	যদি (!f) {
		পূর্ণসংখ্যা saved_errno = errno;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		ফেরত;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	যদি (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		ফেরত;
	}
}

স্থির শূন্য ঢোকাও_object_entry(কাঠামো mark_set **s, কাঠামো object_id *oid, uintmax_t mark)
{
	কাঠামো object_entry *e;
	e = find_object(oid);
	যদি (!e) {
		গণনা object_type type = odb_read_object_info(the_repository->objects,
							     oid, ফাঁকা);
		যদি (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = ঢোকাও_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	ঢোকাও_mark(s, mark, e);
}

স্থির শূন্য ঢোকাও_oid_entry(কাঠামো mark_set **s, কাঠামো object_id *oid, uintmax_t mark)
{
	ঢোকাও_mark(s, mark, xmemdupz(oid, আকার(*oid)));
}

স্থির শূন্য read_mark_file(কাঠামো mark_set **s, FILE *f, mark_set_inserter_t inserter)
{
	অক্ষর সারি[512];
	যখন (fgets(সারি, আকার(সারি), f)) {
		uintmax_t mark;
		অক্ষর *end;
		কাঠামো object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, আকার(oid.hash));

		end = strchr(সারি, '\n');
		যদি (সারি[0] != ':' || !end)
			die(_("corrupt mark line: %s"), সারি);
		*end = 0;
		mark = strtoumax(সারি + 1, &end, 10);
		যদি (!mark || end == সারি + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), সারি);
		inserter(s, &oid, mark);
	}
}

স্থির শূন্য read_marks(শূন্য)
{
	FILE *f = fopen(import_marks_file, "r");
	যদি (f)
		;
	অন্যথা যদি (import_marks_file_ignore_missing && errno == ENOENT)
		যাও done; /* Marks file does not exist */
	অন্যথা
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, ঢোকাও_object_entry);
	fclose(f);
done:
	import_marks_file_done = 1;
}


স্থির পূর্ণসংখ্যা read_পরবর্তী_command(কাঠামো fast_import_state *state)
{
	স্থির পূর্ণসংখ্যা stdin_eof = 0;

	যদি (stdin_eof) {
		unread_command_buf = 0;
		ফেরত EOF;
	}

	জন্য (;;) {
		যদি (unread_command_buf) {
			unread_command_buf = 0;
		} অন্যথা {
			কাঠামো recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			যদি (stdin_eof)
				ফেরত EOF;

			যদি (!state->seen_তথ্য_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			যদি (rc)
				rc_free = rc->পরবর্তী;
			অন্যথা {
				rc = cmd_hist.পরবর্তী;
				cmd_hist.পরবর্তী = rc->পরবর্তী;
				cmd_hist.পরবর্তী->পূর্ব = &cmd_hist;
				মুক্ত(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->পূর্ব = cmd_লেজ;
			rc->পরবর্তী = cmd_hist.পূর্ব;
			rc->পূর্ব->পরবর্তী = rc;
			cmd_লেজ = rc;
		}
		যদি (command_buf.buf[0] == '#')
			চালু;
		ফেরত 0;
	}
}

স্থির শূন্য skip_optional_lf(শূন্য)
{
	পূর্ণসংখ্যা term_char = fgetc(stdin);
	যদি (term_char != '\n' && term_char != EOF)
		ungetc(term_char, stdin);
}

স্থির শূন্য parse_mark(কাঠামো fast_import_state *state)
{
	ধ্রুবক অক্ষর *v;
	যদি (skip_prefix(command_buf.buf, "mark :", &v)) {
		পরবর্তী_mark = strtoumax(v, ফাঁকা, 10);
		read_পরবর্তী_command(state);
	}
	অন্যথা
		পরবর্তী_mark = 0;
}

স্থির শূন্য parse_original_identifier(কাঠামো fast_import_state *state)
{
	ধ্রুবক অক্ষর *v;
	যদি (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_পরবর্তী_command(state);
}

স্থির পূর্ণসংখ্যা parse_তথ্য(কাঠামো strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	ধ্রুবক অক্ষর *তথ্য;
	strbuf_reset(sb);

	যদি (!skip_prefix(command_buf.buf, "data ", &তথ্য))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	যদি (skip_prefix(তথ্য, "<<", &তথ্য)) {
		অক্ষর *term = xstrdup(তথ্য);
		আকার_t term_len = command_buf.len - (তথ্য - command_buf.buf);

		জন্য (;;) {
			যদি (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), term);
			যদি (term_len == command_buf.len
				&& !strcmp(term, command_buf.buf))
				বিরতি;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		মুক্ত(term);
	}
	অন্যথা {
		uintmax_t len = strtoumax(তথ্য, ফাঁকা, 10);
		আকার_t n = 0, দৈর্ঘ্য = (আকার_t)len;

		যদি (limit && limit < len) {
			*len_res = len;
			ফেরত 0;
		}
		যদি (দৈর্ঘ্য < len)
			die(_("data is too large to use in this context"));

		যখন (n < দৈর্ঘ্য) {
			আকার_t s = strbuf_fread(sb, দৈর্ঘ্য - n, stdin);
			যদি (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(unsigned long)(দৈর্ঘ্য - n));
			n += s;
		}
	}

	skip_optional_lf();
	ফেরত 1;
}

স্থির পূর্ণসংখ্যা validate_raw_date(ধ্রুবক অক্ষর *src, কাঠামো strbuf *ফলাফল, পূর্ণসংখ্যা strict)
{
	ধ্রুবক অক্ষর *orig_src = src;
	অক্ষর *endp;
	unsigned long অঙ্ক;

	errno = 0;

	অঙ্ক = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	যদি (errno || endp == src || *endp != ' ')
		ফেরত -1;

	src = endp + 1;
	যদি (*src != '-' && *src != '+')
		ফেরত -1;

	অঙ্ক = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	যদি (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < অঙ্ক))             /* parsed a broken timezone */
	   )
		ফেরত -1;

	strbuf_addstr(ফলাফল, orig_src);
	ফেরত 0;
}

স্থির অক্ষর *parse_ident(ধ্রুবক অক্ষর *buf)
{
	ধ্রুবক অক্ষর *ltgt;
	আকার_t name_len;
	কাঠামো strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	যদি (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	যদি (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	যদি (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	যদি (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	যদি (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_add(&ident, buf, name_len);

	নির্বাচন (whenspec) {
	ক্ষেত্র WHENSPEC_RAW:
		যদি (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		বিরতি;
	ক্ষেত্র WHENSPEC_RAW_PERMISSIVE:
		যদি (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		বিরতি;
	ক্ষেত্র WHENSPEC_RFC2822:
		যদি (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		বিরতি;
	ক্ষেত্র WHENSPEC_NOW:
		যদি (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		বিরতি;
	}

	ফেরত strbuf_detach(&ident, ফাঁকা);
}

স্থির শূন্য parse_and_store_blob(
	কাঠামো last_object *last,
	কাঠামো object_id *oidout,
	uintmax_t mark)
{
	স্থির কাঠামো strbuf buf = STRBUF_INIT;
	uintmax_t len;

	যদি (parse_তথ্য(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	অন্যথা {
		যদি (last) {
			strbuf_release(&last->তথ্য);
			last->offset = 0;
			last->depth = 0;
		}
		stream_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

স্থির শূন্য parse_new_blob(কাঠামো fast_import_state *state)
{
	read_পরবর্তী_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&last_blob, ফাঁকা, পরবর্তী_mark);
}

স্থির শূন্য unload_one_branch(শূন্য)
{
	যখন (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = ULONG_MAX;
		কাঠামো branch *e, *l = ফাঁকা, *p = ফাঁকা;

		জন্য (e = active_branches; e; e = e->active_পরবর্তী_branch) {
			যদি (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		যদি (p) {
			e = p->active_পরবর্তী_branch;
			p->active_পরবর্তী_branch = e->active_পরবর্তী_branch;
		} অন্যথা {
			e = active_branches;
			active_branches = e->active_পরবর্তী_branch;
		}
		e->active = 0;
		e->active_পরবর্তী_branch = ফাঁকা;
		যদি (e->branch_বৃক্ষ.বৃক্ষ) {
			release_বৃক্ষ_content_recursive(e->branch_বৃক্ষ.বৃক্ষ);
			e->branch_বৃক্ষ.বৃক্ষ = ফাঁকা;
		}
		cur_active_branches--;
	}
}

স্থির শূন্য load_branch(কাঠামো branch *b)
{
	load_বৃক্ষ(&b->branch_বৃক্ষ);
	যদি (!b->active) {
		b->active = 1;
		b->active_পরবর্তী_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_গুনতি++;
	}
}

স্থির unsigned অক্ষর convert_অঙ্ক_notes_to_fanout(uintmax_t অঙ্ক_notes)
{
	unsigned অক্ষর fanout = 0;
	যখন ((অঙ্ক_notes >>= 8))
		fanout++;
	ফেরত fanout;
}

স্থির শূন্য construct_path_with_fanout(ধ্রুবক অক্ষর *hex_sha1,
		unsigned অক্ষর fanout, অক্ষর *path)
{
	unsigned পূর্ণসংখ্যা i = 0, j = 0;
	যদি (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	যখন (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	memcpy(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

স্থির uintmax_t do_change_note_fanout(
		কাঠামো বৃক্ষ_entry *orig_মূল, কাঠামো বৃক্ষ_entry *মূল,
		অক্ষর *hex_oid, unsigned পূর্ণসংখ্যা hex_oid_len,
		অক্ষর *fullpath, unsigned পূর্ণসংখ্যা fullpath_len,
		unsigned অক্ষর fanout)
{
	কাঠামো বৃক্ষ_content *t;
	কাঠামো বৃক্ষ_entry *e, leaf;
	unsigned পূর্ণসংখ্যা i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t অঙ্ক_notes = 0;
	কাঠামো object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	অক্ষর realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ধ্রুবক unsigned hexsz = the_hash_algo->hexsz;

	যদি (!মূল->বৃক্ষ)
		load_বৃক্ষ(মূল);
	t = মূল->বৃক্ষ;

	জন্য (i = 0; t && i < t->entry_গুনতি; i++) {
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
		যদি (!e->versions[1].mode ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			চালু;

		/* This _may_ be a note entry, or a subdir containing notes */
		memcpy(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		যদি (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		memcpy(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		যদি (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			যদি (fanout == 0xff) {
				/* Counting mode, no rename */
				অঙ্ক_notes++;
				চালু;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			যদি (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				অঙ্ক_notes++;
				চালু;
			}

			/* Rename fullpath to realpath */
			যদি (!বৃক্ষ_content_remove(orig_মূল, fullpath, &leaf, 0))
				die(_("failed to remove path %s"), fullpath);
			বৃক্ষ_content_set(orig_মূল, realpath,
				&leaf.versions[1].oid,
				leaf.versions[1].mode,
				leaf.বৃক্ষ);
		} অন্যথা যদি (S_ISDIR(e->versions[1].mode)) {
			/* This is a subdir that may contain note entries */
			অঙ্ক_notes += do_change_note_fanout(orig_মূল, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = মূল->বৃক্ষ;
	}
	ফেরত অঙ্ক_notes;
}

স্থির uintmax_t change_note_fanout(কাঠামো বৃক্ষ_entry *মূল,
		unsigned অক্ষর fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	অক্ষর hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	ফেরত do_change_note_fanout(মূল, মূল, hex_oid, 0, path, 0, fanout);
}

স্থির পূর্ণসংখ্যা parse_mapped_oid_hex(ধ্রুবক অক্ষর *hex, কাঠামো object_id *oid, ধ্রুবক অক্ষর **end)
{
	পূর্ণসংখ্যা algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, আকার(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	যদি (algo == GIT_HASH_UNKNOWN)
		ফেরত -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	যদি (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		যদি (hash_algos[algo].format_id == the_hash_algo->format_id)
			ফেরত 0;
		ফেরত -1;
	}
	oidcpy(oid, kh_মান(sub_oid_map, it));
	ফেরত 0;
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
স্থির uintmax_t parse_mark_ref(ধ্রুবক অক্ষর *p, অক্ষর **endptr)
{
	uintmax_t mark;

	প্রতিজ্ঞা(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	যদি (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	ফেরত mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
স্থির uintmax_t parse_mark_ref_eol(ধ্রুবক অক্ষর *p)
{
	অক্ষর *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	যদি (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	ফেরত mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
স্থির uintmax_t parse_mark_ref_space(ধ্রুবক অক্ষর **p)
{
	uintmax_t mark;
	অক্ষর *end;

	mark = parse_mark_ref(*p, &end);
	যদি (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	ফেরত mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
স্থির শূন্য parse_path(কাঠামো strbuf *sb, ধ্রুবক অক্ষর *p, ধ্রুবক অক্ষর **endp,
		পূর্ণসংখ্যা is_last_field, ধ্রুবক অক্ষর *field)
{
	যদি (*p == '"') {
		যদি (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), field, command_buf.buf);
		যদি (strlen(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), field, command_buf.buf);
	} অন্যথা {
		/*
		 * Unless we are parsing the last field of a line,
		 * SP is the end of this field.
		 */
		*endp = is_last_field
			? p + strlen(p)
			: strchrnul(p, ' ');
		strbuf_add(sb, p, *endp - p);
	}
}

/*
 * Parse the path string into the strbuf, and complain if this is not the end of
 * the string. Unquoted strings may contain spaces.
 */
স্থির শূন্য parse_path_eol(কাঠামো strbuf *sb, ধ্রুবক অক্ষর *p, ধ্রুবক অক্ষর *field)
{
	ধ্রুবক অক্ষর *end;

	parse_path(sb, p, &end, 1, field);
	যদি (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
স্থির শূন্য parse_path_space(কাঠামো strbuf *sb, ধ্রুবক অক্ষর *p,
		ধ্রুবক অক্ষর **endp, ধ্রুবক অক্ষর *field)
{
	parse_path(sb, p, endp, 0, field);
	যদি (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

স্থির শূন্য file_change_m(কাঠামো fast_import_state *state,
			  ধ্রুবক অক্ষর *p,
			  কাঠামো branch *b)
{
	স্থির কাঠামো strbuf path = STRBUF_INIT;
	কাঠামো object_entry *oe;
	কাঠামো object_id oid;
	uint16_t mode, inline_তথ্য = 0;

	p = parse_mode(p, &mode);
	যদি (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	নির্বাচন (mode) {
	ক্ষেত্র 0644:
	ক্ষেত্র 0755:
		mode |= S_IFREG;
	ক্ষেত্র S_IFREG | 0644:
	ক্ষেত্র S_IFREG | 0755:
	ক্ষেত্র S_IFLNK:
	ক্ষেত্র S_IFDIR:
	ক্ষেত্র S_IFGITLINK:
		/* ok */
		বিরতি;
	পূর্বনির্ধারিত:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	যদি (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} অন্যথা যদি (skip_prefix(p, "inline ", &p)) {
		inline_তথ্য = 1;
		oe = ফাঁকা; /* not used with inline_data, but makes gcc happy */
	} অন্যথা {
		যদি (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		যদি (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	যদি (S_ISDIR(mode) &&
	    is_empty_বৃক্ষ_oid(&oid, the_repository->hash_algo) &&
	    *path.buf) {
		বৃক্ষ_content_remove(&b->branch_বৃক্ষ, path.buf, ফাঁকা, 0);
		ফেরত;
	}

	যদি (S_ISGITLINK(mode)) {
		যদি (inline_তথ্য)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		অন্যথা যদি (oe) {
			যদি (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} অন্যথা যদি (inline_তথ্য) {
		যদি (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		যখন (read_পরবর্তী_command(state) != EOF) {
			ধ্রুবক অক্ষর *v;
			যদি (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			অন্যথা {
				parse_and_store_blob(&last_blob, &oid, 0);
				বিরতি;
			}
		}
	} অন্যথা {
		গণনা object_type expected = S_ISDIR(mode) ?
						OBJ_বৃক্ষ: OBJ_BLOB;
		গণনা object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, ফাঁকা);
		যদি (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(mode) ?  _("tree") : _("blob"),
			    command_buf.buf);
		যদি (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	যদি (!*path.buf) {
		বৃক্ষ_content_replace(&b->branch_বৃক্ষ, &oid, mode, ফাঁকা);
		ফেরত;
	}

	যদি (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	বৃক্ষ_content_set(&b->branch_বৃক্ষ, path.buf, &oid, mode, ফাঁকা);
}

স্থির শূন্য file_change_d(ধ্রুবক অক্ষর *p, কাঠামো branch *b)
{
	স্থির কাঠামো strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	বৃক্ষ_content_remove(&b->branch_বৃক্ষ, path.buf, ফাঁকা, 1);
}

স্থির শূন্য file_change_cr(ধ্রুবক অক্ষর *p, কাঠামো branch *b, পূর্ণসংখ্যা rename)
{
	স্থির কাঠামো strbuf source = STRBUF_INIT;
	স্থির কাঠামো strbuf dest = STRBUF_INIT;
	কাঠামো বৃক্ষ_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	memset(&leaf, 0, আকার(leaf));
	যদি (rename)
		বৃক্ষ_content_remove(&b->branch_বৃক্ষ, source.buf, &leaf, 1);
	অন্যথা
		বৃক্ষ_content_get(&b->branch_বৃক্ষ, source.buf, &leaf, 1);
	যদি (!leaf.versions[1].mode)
		die(_("path %s not in branch"), source.buf);
	যদি (!*dest.buf) {	/* C "path/to/subdir" "" */
		বৃক্ষ_content_replace(&b->branch_বৃক্ষ,
			&leaf.versions[1].oid,
			leaf.versions[1].mode,
			leaf.বৃক্ষ);
		ফেরত;
	}
	যদি (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	বৃক্ষ_content_set(&b->branch_বৃক্ষ, dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.বৃক্ষ);
}

স্থির শূন্য note_change_n(কাঠামো fast_import_state *state,
			  ধ্রুবক অক্ষর *p,
			  কাঠামো branch *b,
			  unsigned অক্ষর *old_fanout)
{
	কাঠামো object_entry *oe;
	কাঠামো branch *s;
	কাঠামো object_id oid, commit_oid;
	অক্ষর path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_তথ্য = 0;
	unsigned অক্ষর new_fanout;

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
	যদি (b->অঙ্ক_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->অঙ্ক_notes = change_note_fanout(&b->branch_বৃক্ষ, 0xff);
		*old_fanout = convert_অঙ্ক_notes_to_fanout(b->অঙ্ক_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	যদি (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} অন্যথা যদি (skip_prefix(p, "inline ", &p)) {
		inline_তথ্য = 1;
		oe = ফাঁকা; /* not used with inline_data, but makes gcc happy */
	} অন্যথা {
		যদি (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		যদি (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	যদি (s) {
		যদি (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} অন্যথা যদি (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		কাঠামো object_entry *commit_oe = find_mark(marks, commit_mark);
		যদি (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} অন্যথা যদি (!repo_get_oid(the_repository, p, &commit_oid)) {
		আকার_t আকার_2;
		অক্ষর *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &আকার_2,
						   &commit_oid);
		যদি (!buf || আকার_2 < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		মুক্ত(buf);
	} অন্যথা
		die(_("invalid ref name or SHA1 expression: %s"), p);

	যদি (inline_তথ্য) {
		read_পরবর্তী_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} অন্যথা যদি (oe) {
		যদি (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} অন্যথা যদি (!is_null_oid(&oid)) {
		গণনা object_type type = odb_read_object_info(the_repository->objects, &oid,
							ফাঁকা);
		যদি (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		যদি (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	যদি (বৃক্ষ_content_remove(&b->branch_বৃক্ষ, path, ফাঁকা, 0))
		b->অঙ্ক_notes--;

	যদি (is_null_oid(&oid))
		ফেরত; /* nothing to insert */

	b->অঙ্ক_notes++;
	new_fanout = convert_অঙ্ক_notes_to_fanout(b->অঙ্ক_notes);
	construct_path_with_fanout(oid_to_hex(&commit_oid), new_fanout, path);
	বৃক্ষ_content_set(&b->branch_বৃক্ষ, path, &oid, S_IFREG | 0644, ফাঁকা);
}

স্থির শূন্য file_change_deleteall(কাঠামো branch *b)
{
	release_বৃক্ষ_content_recursive(b->branch_বৃক্ষ.বৃক্ষ);
	oidclr(&b->branch_বৃক্ষ.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_বৃক্ষ.versions[1].oid, the_repository->hash_algo);
	load_বৃক্ষ(&b->branch_বৃক্ষ);
	b->অঙ্ক_notes = 0;
}

স্থির শূন্য parse_from_commit(কাঠামো branch *b, অক্ষর *buf, unsigned long আকার_2)
{
	যদি (!buf || আকার_2 < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	যদি (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_বৃক্ষ.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_বৃক্ষ.versions[0].oid,
	       &b->branch_বৃক্ষ.versions[1].oid);
}

স্থির শূন্য parse_from_existing(কাঠামো branch *b)
{
	যদি (is_null_oid(&b->oid)) {
		oidclr(&b->branch_বৃক্ষ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_বৃক্ষ.versions[1].oid, the_repository->hash_algo);
	} অন্যথা {
		unsigned long আকার_2;
		আকার_t আকার_st = 0;
		অক্ষর *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &আকার_st, &b->oid);
		আকার_2 = cast_আকার_t_to_ulong(আকার_st);
		parse_from_commit(b, buf, আকার_2);
		মুক্ত(buf);
	}
}

স্থির পূর্ণসংখ্যা parse_objectish(কাঠামো fast_import_state *state,
			   কাঠামো branch *b,
			   ধ্রুবক অক্ষর *objectish)
{
	কাঠামো branch *s;
	কাঠামো object_id oid;

	oidcpy(&oid, &b->branch_বৃক্ষ.versions[1].oid);

	s = lookup_branch(objectish);
	যদি (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	অন্যথা যদি (s) {
		কাঠামো object_id *t = &s->branch_বৃক্ষ.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_বৃক্ষ.versions[0].oid, t);
		oidcpy(&b->branch_বৃক্ষ.versions[1].oid, t);
	} অন্যথা যদি (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		কাঠামো object_entry *oe = find_mark(marks, idnum);
		যদি (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		যদি (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			যদি (oe->pack_id != MAX_PACK_ID) {
				unsigned long আকার_2;
				অক্ষর *buf = gfi_unpack_entry(oe, &আকার_2);
				parse_from_commit(b, buf, আকার_2);
				মুক্ত(buf);
			} অন্যথা
				parse_from_existing(b);
		}
	} অন্যথা যদি (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		যদি (is_null_oid(&b->oid))
			b->delete = 1;
	}
	অন্যথা
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	যদি (b->branch_বৃক্ষ.বৃক্ষ && !oideq(&oid, &b->branch_বৃক্ষ.versions[1].oid)) {
		release_বৃক্ষ_content_recursive(b->branch_বৃক্ষ.বৃক্ষ);
		b->branch_বৃক্ষ.বৃক্ষ = ফাঁকা;
	}

	read_পরবর্তী_command(state);
	ফেরত 1;
}

স্থির পূর্ণসংখ্যা parse_from(কাঠামো fast_import_state *state, কাঠামো branch *b)
{
	ধ্রুবক অক্ষর *from;

	যদি (!skip_prefix(command_buf.buf, "from ", &from))
		ফেরত 0;

	ফেরত parse_objectish(state, b, from);
}

স্থির পূর্ণসংখ্যা parse_objectish_with_prefix(কাঠামো fast_import_state *state,
				       কাঠামো branch *b,
				       ধ্রুবক অক্ষর *prefix)
{
	ধ্রুবক অক্ষর *base;

	যদি (!skip_prefix(command_buf.buf, prefix, &base))
		ফেরত 0;

	ফেরত parse_objectish(state, b, base);
}

স্থির কাঠামো hash_list *parse_merge(কাঠামো fast_import_state *state,
				     unsigned পূর্ণসংখ্যা *গুনতি)
{
	কাঠামো hash_list *list = ফাঁকা, **লেজ = &list, *n;
	ধ্রুবক অক্ষর *from;
	কাঠামো branch *s;

	*গুনতি = 0;
	যখন (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(আকার(*n));
		s = lookup_branch(from);
		যদি (s)
			oidcpy(&n->oid, &s->oid);
		অন্যথা যদি (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			কাঠামো object_entry *oe = find_mark(marks, idnum);
			যদি (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} অন্যথা যদি (!repo_get_oid(the_repository, from, &n->oid)) {
			আকার_t আকার_2;
			অক্ষর *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &আকার_2, &n->oid);
			যদি (!buf || আকার_2 < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			মুক্ত(buf);
		} অন্যথা
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->পরবর্তী = ফাঁকা;
		*লেজ = n;
		লেজ = &n->পরবর্তী;

		(*গুনতি)++;
		read_পরবর্তী_command(state);
	}
	ফেরত list;
}

কাঠামো signature_তথ্য {
	অক্ষর *hash_algo;      /* "sha1" or "sha256" */
	অক্ষর *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	কাঠামো strbuf তথ্য;   /* The actual signature data */
};

স্থির শূন্য parse_one_signature(কাঠামো fast_import_state *state,
				কাঠামো signature_তথ্য *sig,
				ধ্রুবক অক্ষর *v)
{
	অক্ষর *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	অক্ষর *space = strchr(args, ' ');

	যদি (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), args);
	*space = '\0';

	sig->hash_algo = args;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	যদি (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	যদি (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	যদি (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_পরবর্তী_command(state);
	parse_তথ্য(&sig->তথ্য, 0, ফাঁকা);
}

স্থির শূন্য discard_one_signature(কাঠামো fast_import_state *state)
{
	কাঠামো strbuf তথ্য = STRBUF_INIT;

	read_পরবর্তী_command(state);
	parse_তথ্য(&তথ্য, 0, ফাঁকা);
	strbuf_release(&তথ্য);
}

স্থির শূন্য add_gpgsig_to_commit(কাঠামো strbuf *commit_তথ্য,
				 ধ্রুবক অক্ষর *header,
				 কাঠামো signature_তথ্য *sig)
{
	কাঠামো string_list siglines = STRING_LIST_INIT_NODUP;

	যদি (!sig || !sig->hash_algo)
		ফেরত;

	strbuf_addstr(commit_তথ্য, header);
	string_list_split_in_place(&siglines, sig->তথ্য.buf, "\n", -1);
	strbuf_add_separated_string_list(commit_তথ্য, "\n ", &siglines);
	strbuf_addch(commit_তথ্য, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->তথ্য);
	মুক্ত(sig->hash_algo);
}

স্থির শূন্য store_signature(কাঠামো signature_তথ্য *stored_sig,
			    কাঠামো signature_তথ্য *new_sig,
			    ধ্রুবক অক্ষর *hash_type)
{
	যদি (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->তথ্য);
		মুক্ত(new_sig->hash_algo);
	} অন্যথা {
		*stored_sig = *new_sig;
	}
}

স্থির শূন্য import_one_signature(কাঠামো fast_import_state *state,
				 কাঠামো signature_তথ্য *sig_sha1,
				 কাঠামো signature_তথ্য *sig_sha256,
				 ধ্রুবক অক্ষর *v)
{
	কাঠামো signature_তথ্য sig = { ফাঁকা, ফাঁকা, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	যদি (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	অন্যথা যদি (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	অন্যথা
		die(_("parse_one_signature() returned unknown hash algo"));
}

স্থির শূন্য finalize_commit_বাফার(কাঠামো strbuf *new_তথ্য,
				   কাঠামো signature_তথ্য *sig_sha1,
				   কাঠামো signature_তথ্য *sig_sha256,
				   কাঠামো strbuf *বার্তা)
{
	add_gpgsig_to_commit(new_তথ্য, "gpgsig ", sig_sha1);
	add_gpgsig_to_commit(new_তথ্য, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_তথ্য, '\n');
	strbuf_addbuf(new_তথ্য, বার্তা);
}

স্থির শূন্য warn_invalid_signature(কাঠামো signature_check *check,
				   ধ্রুবক অক্ষর *বার্তা, গণনা sign_mode mode)
{
	ধ্রুবক অক্ষর *signer = check->signer ? check->signer : _("unknown");
	ধ্রুবক অক্ষর *subject;
	পূর্ণসংখ্যা subject_len = find_commit_subject(বার্তা, &subject);

	নির্বাচন (mode) {
	ক্ষেত্র SIGN_STRIP_IF_INVALID:
		যদি (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		অন্যথা যদি (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		অন্যথা
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		বিরতি;
	ক্ষেত্র SIGN_SIGN_IF_INVALID:
		যদি (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		অন্যথা যদি (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		অন্যথা
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("unsupported signing mode");
	}
}

স্থির শূন্য handle_signature_if_invalid(কাঠামো strbuf *new_তথ্য,
					কাঠামো signature_তথ্য *sig_sha1,
					কাঠামো signature_তথ্য *sig_sha256,
					কাঠামো strbuf *বার্তা,
					গণনা sign_mode mode)
{
	কাঠামো strbuf tmp_buf = STRBUF_INIT;
	কাঠামো signature_check signature_check = { 0 };
	পূর্ণসংখ্যা ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_তথ্য);
	finalize_commit_বাফার(&tmp_buf, sig_sha1, sig_sha256, বার্তা);
	ret = verify_commit_বাফার(tmp_buf.buf, tmp_buf.len, &signature_check);

	যদি (ret) {
		যদি (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, বার্তা->buf, mode);

		যদি (mode == SIGN_SIGN_IF_INVALID) {
			কাঠামো strbuf signature = STRBUF_INIT;
			কাঠামো strbuf payload = STRBUF_INIT;

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
			যদি (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			যদি (sign_বাফার(&payload, &signature, signed_commit_keyid,
					SIGN_বাফার_USE_DEFAULT_চাবি))
				die(_("failed to sign commit object"));
			add_header_signature(new_তথ্য, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_বাফার(new_তথ্য, ফাঁকা, ফাঁকা, বার্তা);
	} অন্যথা {
		strbuf_swap(new_তথ্য, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

স্থির শূন্য parse_new_commit(কাঠামো fast_import_state *state, ধ্রুবক অক্ষর *arg)
{
	স্থির কাঠামো strbuf বার্তা = STRBUF_INIT;
	কাঠামো signature_তথ্য sig_sha1 = { ফাঁকা, ফাঁকা, STRBUF_INIT };
	কাঠামো signature_তথ্য sig_sha256 = { ফাঁকা, ফাঁকা, STRBUF_INIT };
	কাঠামো branch *b;
	অক্ষর *author = ফাঁকা;
	অক্ষর *committer = ফাঁকা;
	অক্ষর *encoding = ফাঁকা;
	কাঠামো hash_list *merge_list = ফাঁকা;
	unsigned পূর্ণসংখ্যা merge_গুনতি;
	unsigned অক্ষর পূর্ব_fanout, new_fanout;
	ধ্রুবক অক্ষর *v;

	b = lookup_branch(arg);
	যদি (!b)
		b = new_branch(arg);

	read_পরবর্তী_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	যদি (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_পরবর্তী_command(state);
	}
	যদি (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_পরবর্তী_command(state);
	}
	যদি (!committer)
		die(_("expected committer but didn't get one"));

	যখন (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		নির্বাচন (signed_commit_mode) {

		/* First, modes that don't need the signature to be parsed */
		ক্ষেত্র SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		ক্ষেত্র SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		ক্ষেত্র SIGN_STRIP:
			discard_one_signature(state);
			বিরতি;

		/* Second, modes that parse the signature */
		ক্ষেত্র SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		ক্ষেত্র SIGN_VERBATIM:
		ক্ষেত্র SIGN_STRIP_IF_INVALID:
		ক্ষেত্র SIGN_SIGN_IF_INVALID:
		ক্ষেত্র SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			বিরতি;

		/* Third, BUG */
		পূর্বনির্ধারিত:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_পরবর্তী_command(state);
	}

	যদি (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_পরবর্তী_command(state);
	}
	parse_তথ্য(&বার্তা, 0, ফাঁকা);
	read_পরবর্তী_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_গুনতি);

	/* ensure the branch is active/loaded */
	যদি (!b->branch_বৃক্ষ.বৃক্ষ || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	পূর্ব_fanout = convert_অঙ্ক_notes_to_fanout(b->অঙ্ক_notes);

	/* file_change* */
	যখন (command_buf.len > 0) {
		যদি (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		অন্যথা যদি (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		অন্যথা যদি (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		অন্যথা যদি (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		অন্যথা যদি (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &পূর্ব_fanout);
		অন্যথা যদি (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		অন্যথা যদি (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		অন্যথা যদি (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		অন্যথা {
			unread_command_buf = 1;
			বিরতি;
		}
		যদি (read_পরবর্তী_command(state) == EOF)
			বিরতি;
	}

	new_fanout = convert_অঙ্ক_notes_to_fanout(b->অঙ্ক_notes);
	যদি (new_fanout != পূর্ব_fanout)
		b->অঙ্ক_notes = change_note_fanout(&b->branch_বৃক্ষ, new_fanout);

	/* build the tree and the commit */
	store_বৃক্ষ(&b->branch_বৃক্ষ);
	oidcpy(&b->branch_বৃক্ষ.versions[0].oid,
	       &b->branch_বৃক্ষ.versions[1].oid);

	strbuf_reset(&new_তথ্য);
	strbuf_addf(&new_তথ্য, "tree %s\n",
		oid_to_hex(&b->branch_বৃক্ষ.versions[1].oid));
	যদি (!is_null_oid(&b->oid))
		strbuf_addf(&new_তথ্য, "parent %s\n",
			    oid_to_hex(&b->oid));
	যখন (merge_list) {
		কাঠামো hash_list *পরবর্তী = merge_list->পরবর্তী;
		strbuf_addf(&new_তথ্য, "parent %s\n",
			    oid_to_hex(&merge_list->oid));
		মুক্ত(merge_list);
		merge_list = পরবর্তী;
	}
	strbuf_addf(&new_তথ্য,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	যদি (encoding)
		strbuf_addf(&new_তথ্য,
			"encoding %s\n",
			encoding);

	যদি ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
	     signed_commit_mode == SIGN_SIGN_IF_INVALID ||
	     signed_commit_mode == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_তথ্য, &sig_sha1, &sig_sha256,
					    &বার্তা, signed_commit_mode);
	অন্যথা
		finalize_commit_বাফার(&new_তথ্য, &sig_sha1, &sig_sha256, &বার্তা);

	মুক্ত(author);
	মুক্ত(committer);
	মুক্ত(encoding);

	যদি (!store_object(OBJ_COMMIT, &new_তথ্য, ফাঁকা, &b->oid, পরবর্তী_mark))
		b->pack_id = pack_id;
	b->last_commit = object_গুনতি_by_type[OBJ_COMMIT];
}

স্থির শূন্য handle_tag_signature_if_invalid(কাঠামো strbuf *buf,
					    কাঠামো strbuf *বার্তা,
					    আকার_t sig_offset)
{
	কাঠামো strbuf signature = STRBUF_INIT;
	কাঠামো strbuf payload = STRBUF_INIT;
	কাঠামো signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_add(&payload, বার্তা->buf, sig_offset);
	strbuf_add(&signature, বার্তা->buf + sig_offset, বার্তা->len - sig_offset);

	sigc.payload_type = SIGNATURE_PAYLOAD_TAG;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	যদি (!check_signature(&sigc, signature.buf, signature.len))
		যাও out;

	যদি (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(বার্তা, sig_offset);

	যদি (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = ফাঁকা;
		strbuf_reset(&signature);

		যদি (sign_বাফার(&payload, &signature, signed_tag_keyid,
				SIGN_বাফার_USE_DEFAULT_চাবি))
			die(_("failed to sign tag object"));

		strbuf_addbuf(বার্তা, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

স্থির শূন্য handle_tag_signature(কাঠামো strbuf *buf, কাঠামো strbuf *বার্তা, ধ্রুবক অক্ষর *name)
{
	আকার_t sig_offset = parse_signed_বাফার(বার্তা->buf, বার্তা->len);

	/* If there is no signature, there is nothing to do. */
	যদি (sig_offset >= বার্তা->len)
		ফেরত;

	নির্বাচন (signed_tag_mode) {

	/* First, modes that don't change anything */
	ক্ষেত্র SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	ক্ষেত্র SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		বিরতি;

	/* Second, modes that remove the signature */
	ক্ষেত্র SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	ক্ষেত্র SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(বার্তা, sig_offset);
		বিরতি;
	ক্ষেত্র SIGN_ABORT_IF_INVALID:
	ক্ষেত্র SIGN_SIGN_IF_INVALID:
	ক্ষেত্র SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, বার্তা, sig_offset);
		বিরতি;

	/* Third, aborting modes */
	ক্ষেত্র SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	পূর্বনির্ধারিত:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

স্থির শূন্য parse_new_tag(কাঠামো fast_import_state *state, ধ্রুবক অক্ষর *arg)
{
	স্থির কাঠামো strbuf বার্তা = STRBUF_INIT;
	ধ্রুবক অক্ষর *from;
	অক্ষর *tagger;
	কাঠামো branch *s;
	কাঠামো tag *t;
	uintmax_t from_mark = 0;
	কাঠামো object_id oid;
	গণনা object_type type;
	ধ্রুবক অক্ষর *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, আকার(কাঠামো tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	যদি (last_tag)
		last_tag->পরবর্তী_tag = t;
	অন্যথা
		first_tag = t;
	last_tag = t;
	read_পরবর্তী_command(state);
	parse_mark(state);

	/* from ... */
	যদি (!skip_prefix(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	যদি (s) {
		যদি (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} অন্যথা যদি (*from == ':') {
		কাঠামো object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} অন্যথা যদি (!repo_get_oid(the_repository, from, &oid)) {
		কাঠামো object_entry *oe = find_object(&oid);
		যদি (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, ফাঁকা);
			যদি (type < 0)
				die(_("not a valid object: %s"), from);
		} অন্যথা
			type = oe->type;
	} অন্যথা
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_পরবর্তী_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	যদি (skip_prefix(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_পরবর্তী_command(state);
	} অন্যথা
		tagger = ফাঁকা;

	/* tag payload/message */
	parse_তথ্য(&বার্তা, 0, ফাঁকা);

	/* build the tag object */
	strbuf_reset(&new_তথ্য);

	strbuf_addf(&new_তথ্য,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_to_hex(&oid), type_name(type), t->name);
	যদি (tagger)
		strbuf_addf(&new_তথ্য,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_তথ্য, &বার্তা, t->name);

	strbuf_addch(&new_তথ্য, '\n');
	strbuf_addbuf(&new_তথ্য, &বার্তা);
	মুক্ত(tagger);

	যদি (store_object(OBJ_TAG, &new_তথ্য, ফাঁকা, &t->oid, পরবর্তী_mark))
		t->pack_id = MAX_PACK_ID;
	অন্যথা
		t->pack_id = pack_id;
}

স্থির শূন্য parse_reset_branch(কাঠামো fast_import_state *state, ধ্রুবক অক্ষর *arg)
{
	কাঠামো branch *b;
	ধ্রুবক অক্ষর *tag_name;

	b = lookup_branch(arg);
	যদি (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_বৃক্ষ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_বৃক্ষ.versions[1].oid, the_repository->hash_algo);
		যদি (b->branch_বৃক্ষ.বৃক্ষ) {
			release_বৃক্ষ_content_recursive(b->branch_বৃক্ষ.বৃক্ষ);
			b->branch_বৃক্ষ.বৃক্ষ = ফাঁকা;
		}
	}
	অন্যথা
		b = new_branch(arg);
	read_পরবর্তী_command(state);
	parse_from(state, b);
	যদি (b->delete && skip_prefix(b->name, "refs/tags/", &tag_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		কাঠামো tag *t, *পূর্ব = ফাঁকা;
		জন্য (t = first_tag; t; t = t->পরবর্তী_tag) {
			যদি (!strcmp(t->name, tag_name))
				বিরতি;
			পূর্ব = t;
		}
		যদি (t) {
			যদি (পূর্ব)
				পূর্ব->পরবর্তী_tag = t->পরবর্তী_tag;
			অন্যথা
				first_tag = t->পরবর্তী_tag;
			যদি (!t->পরবর্তী_tag)
				last_tag = পূর্ব;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	যদি (command_buf.len > 0)
		unread_command_buf = 1;
}

স্থির শূন্য cat_blob_write(ধ্রুবক অক্ষর *buf, unsigned long আকার_2)
{
	যদি (write_in_full(cat_blob_fd, buf, আকার_2) < 0)
		die_errno(_("write to frontend failed"));
}

স্থির শূন্য cat_blob(কাঠামো object_entry *oe, কাঠামো object_id *oid)
{
	কাঠামো strbuf সারি = STRBUF_INIT;
	কাঠামো iovec iov[3];
	unsigned long আকার_2;
	গণনা object_type type = 0;
	অক্ষর *buf;

	যদি (!oe || oe->pack_id == MAX_PACK_ID) {
		আকার_t আকার_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &আকার_st);
		আকার_2 = cast_আকার_t_to_ulong(আকার_st);
	} অন্যথা {
		type = oe->type;
		buf = gfi_unpack_entry(oe, &আকার_2);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	যদি (type <= 0) {
		strbuf_reset(&সারি);
		strbuf_addf(&সারি, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(সারি.buf, সারি.len);
		strbuf_release(&সারি);
		মুক্ত(buf);
		ফেরত;
	}
	যদি (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	যদি (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_to_hex(oid), type_name(type));
	strbuf_reset(&সারি);
	strbuf_addf(&সারি, "%s %s %"PRIuMAX"\n", oid_to_hex(oid),
		    type_name(type), (uintmax_t)আকার_2);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_base = সারি.buf;
	iov[0].iov_len = সারি.len;
	iov[1].iov_base = buf;
	iov[1].iov_len = আকার_2;
	iov[2].iov_base = (শূন্য *) "\n";
	iov[2].iov_len = 1;

	যদি (writev_in_full(cat_blob_fd, iov, ARRAY_আকার(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&সারি);
	যদি (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.তথ্য, buf, আকার_2, আকার_2 + 1);
		last_blob.depth = oe->depth;
	} অন্যথা
		মুক্ত(buf);
}

স্থির শূন্য parse_get_mark(কাঠামো fast_import_state *state UNUSED,
			   ধ্রুবক অক্ষর *p)
{
	কাঠামো object_entry *oe;
	অক্ষর output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	যদি (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	যদি (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, আকার(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

স্থির শূন্য parse_cat_blob(কাঠামো fast_import_state *state UNUSED,
			   ধ্রুবক অক্ষর *p)
{
	কাঠামো object_entry *oe;
	কাঠামো object_id oid;

	/* cat-blob SP <object> LF */
	যদি (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		যদি (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} অন্যথা {
		যদি (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		যদি (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

স্থির কাঠামো object_entry *dereference(কাঠামো object_entry *oe,
					কাঠামো object_id *oid)
{
	unsigned long আকার_2;
	অক্ষর *buf = ফাঁকা;
	ধ্রুবক unsigned hexsz = the_hash_algo->hexsz;

	যদি (!oe) {
		গণনা object_type type = odb_read_object_info(the_repository->objects,
							     oid, ফাঁকা);
		যদি (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = ঢোকাও_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	নির্বাচন (oe->type) {
	ক্ষেত্র OBJ_বৃক্ষ:	/* easy case. */
		ফেরত oe;
	ক্ষেত্র OBJ_COMMIT:
	ক্ষেত্র OBJ_TAG:
		বিরতি;
	পূর্বনির্ধারিত:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	যদি (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &আকার_2);
	} অন্যথা {
		গণনা object_type unused;
		আকার_t আকার_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &আকার_st);
		আকার_2 = cast_আকার_t_to_ulong(আকার_st);
	}
	যদি (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	নির্বাচন (oe->type) {
	ক্ষেত্র OBJ_TAG:
		যদি (আকার_2 < hexsz + strlen("object ") ||
		    get_oid_hex(buf + strlen("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		বিরতি;
	ক্ষেত্র OBJ_COMMIT:
		যদি (আকার_2 < hexsz + strlen("tree ") ||
		    get_oid_hex(buf + strlen("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	মুক্ত(buf);
	ফেরত find_object(oid);
}

স্থির শূন্য ঢোকাও_mapped_mark(uintmax_t mark, শূন্য *object, শূন্য *cbp)
{
	কাঠামো object_id *fromoid = object;
	কাঠামো object_id *tooid = find_mark(cbp, mark);
	পূর্ণসংখ্যা ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	যদি (ret == 0)
		ফেরত;
	kh_মান(sub_oid_map, it) = tooid;
}

স্থির শূন্য build_mark_map_one(কাঠামো mark_set *from, কাঠামো mark_set *to)
{
	for_each_mark(from, 0, ঢোকাও_mapped_mark, to);
}

স্থির শূন্য build_mark_map(কাঠামো string_list *from, কাঠামো string_list *to)
{
	কাঠামো string_list_item *fromp, *শীর্ষ_2;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		শীর্ষ_2 = string_list_lookup(to, fromp->string);
		যদি (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} অন্যথা যদি (!শীর্ষ_2 || !শীর্ষ_2->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, শীর্ষ_2->util);
	}
}

স্থির কাঠামো object_entry *parse_treeish_dataref(ধ্রুবক অক্ষর **p)
{
	কাঠামো object_id oid;
	কাঠামো object_entry *e;

	যদি (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		যদি (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} অন্যথা {	/* <sha1> */
		যদি (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		যদি (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	যখন (!e || e->type != OBJ_বৃক্ষ)
		e = dereference(e, &oid);
	ফেরত e;
}

স্থির শূন্য print_ls(পূর্ণসংখ্যা mode, ধ্রুবক unsigned অক্ষর *hash, ধ্রুবক অক্ষর *path)
{
	স্থির কাঠামো strbuf সারি = STRBUF_INIT;

	/* See show_tree(). */
	ধ্রুবক অক্ষর *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? বৃক্ষ_type :
		blob_type;

	যদি (!mode) {
		/* missing SP path LF */
		strbuf_reset(&সারি);
		strbuf_addstr(&সারি, "missing ");
		quote_c_style(path, &সারি, ফাঁকা, 0);
		strbuf_addch(&সারি, '\n');
	} অন্যথা {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&সারি);
		strbuf_addf(&সারি, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &সারি, ফাঁকা, 0);
		strbuf_addch(&সারি, '\n');
	}
	cat_blob_write(সারি.buf, সারি.len);
}

স্থির শূন্য parse_ls(কাঠামো fast_import_state *state UNUSED,
		     ধ্রুবক অক্ষর *p,
		     কাঠামো branch *b)
{
	স্থির কাঠামো strbuf path = STRBUF_INIT;
	কাঠামো বৃক্ষ_entry *মূল = ফাঁকা;
	কাঠামো বৃক্ষ_entry leaf = {ফাঁকা};

	/* ls SP (<tree-ish> SP)? <path> */
	যদি (*p == '"') {
		যদি (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		মূল = &b->branch_বৃক্ষ;
	} অন্যথা {
		কাঠামো object_entry *e = parse_treeish_dataref(&p);
		মূল = new_বৃক্ষ_entry();
		oidcpy(&মূল->versions[1].oid, &e->idx.oid);
		যদি (!is_null_oid(&মূল->versions[1].oid))
			মূল->versions[1].mode = S_IFDIR;
		load_বৃক্ষ(মূল);
	}
	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	বৃক্ষ_content_get(মূল, path.buf, &leaf, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	যদি (S_ISDIR(leaf.versions[1].mode))
		store_বৃক্ষ(&leaf);

	print_ls(leaf.versions[1].mode, leaf.versions[1].oid.hash, path.buf);
	যদি (leaf.বৃক্ষ)
		release_বৃক্ষ_content_recursive(leaf.বৃক্ষ);
	যদি (!b || মূল != &b->branch_বৃক্ষ)
		release_বৃক্ষ_entry(মূল);
}

স্থির শূন্য checkpoint(শূন্য)
{
	checkpoint_requested = 0;
	যদি (object_গুনতি) {
		cycle_packfile();
	}
	dump_branches();
	dump_tags();
	dump_marks();
}

স্থির শূন্য parse_checkpoint(কাঠামো fast_import_state *state UNUSED)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

স্থির শূন্য parse_progress(কাঠামো fast_import_state *state UNUSED)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

স্থির শূন্য parse_alias(কাঠামো fast_import_state *state)
{
	কাঠামো object_entry *e;
	কাঠামো branch b;

	skip_optional_lf();
	read_পরবর্তী_command(state);

	/* mark ... */
	parse_mark(state);
	যদি (!পরবর্তী_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, আকার(b));
	যদি (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	প্রতিজ্ঞা(e);
	ঢোকাও_mark(&marks, পরবর্তী_mark, e);
}

স্থির অক্ষর* make_fast_import_path(কাঠামো fast_import_state *state,
				   ধ্রুবক অক্ষর *path)
{
	যদি (!relative_marks_paths || is_absolute_path(path))
		ফেরত prefix_filename(state->prefix, path);
	ফেরত repo_git_path(the_repository, "info/fast-import/%s", path);
}

স্থির শূন্য option_import_marks(কাঠামো fast_import_state *state,
				ধ্রুবক অক্ষর *marks,
				পূর্ণসংখ্যা from_stream,
				পূর্ণসংখ্যা ignore_missing)
{
	যদি (import_marks_file) {
		যদি (from_stream)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		যদি(!import_marks_file_from_stream)
			read_marks();
	}

	মুক্ত(import_marks_file);
	import_marks_file = make_fast_import_path(state, marks);
	import_marks_file_from_stream = from_stream;
	import_marks_file_ignore_missing = ignore_missing;
}

স্থির শূন্য option_date_format(ধ্রুবক অক্ষর *fmt)
{
	যদি (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	অন্যথা যদি (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	অন্যথা যদি (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	অন্যথা যদি (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	অন্যথা
		die(_("unknown --date-format argument %s"), fmt);
}

স্থির unsigned long ulong_arg(ধ্রুবক অক্ষর *option, ধ্রুবক অক্ষর *arg)
{
	অক্ষর *endptr;
	unsigned long rv = strtoul(arg, &endptr, 0);
	যদি (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	ফেরত rv;
}

স্থির শূন্য option_depth(ধ্রুবক অক্ষর *depth)
{
	max_depth = ulong_arg("--depth", depth);
	যদি (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

স্থির শূন্য option_active_branches(ধ্রুবক অক্ষর *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

স্থির শূন্য option_export_marks(কাঠামো fast_import_state *state,
				ধ্রুবক অক্ষর *marks)
{
	মুক্ত(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

স্থির শূন্য option_cat_blob_fd(কাঠামো fast_import_state *state UNUSED,
			       ধ্রুবক অক্ষর *fd)
{
	unsigned long n = ulong_arg("--cat-blob-fd", fd);
	যদি (n > (unsigned long) INT_MAX)
		die(_("--cat-blob-fd cannot exceed %d"), INT_MAX);
	cat_blob_fd = (পূর্ণসংখ্যা) n;
}

স্থির শূন্য option_export_pack_edges(কাঠামো fast_import_state *state,
				     ধ্রুবক অক্ষর *edges)
{
	অক্ষর *fn = prefix_filename(state->prefix, edges);
	যদি (pack_edges)
		fclose(pack_edges);
	pack_edges = xfopen(fn, "a");
	মুক্ত(fn);
}

স্থির শূন্য option_rewrite_submodules(কাঠামো fast_import_state *state,
				      ধ্রুবক অক্ষর *arg,
				      কাঠামো string_list *list)
{
	কাঠামো mark_set *ms;
	FILE *fp;
	অক্ষর *s = xstrdup(arg);
	অক্ষর *f = strchr(s, ':');
	যদি (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = prefix_filename(state->prefix, f);
	fp = fopen(f, "r");
	যদি (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, ঢোকাও_oid_entry);
	fclose(fp);
	মুক্ত(f);

	string_list_ঢোকাও(list, s)->util = ms;

	মুক্ত(s);
}

স্থির শূন্য option_max_pack_আকার(ধ্রুবক অক্ষর *arg)
{
	unsigned long v;

	যদি (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	যদি (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} অন্যথা যদি (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

স্থির শূন্য option_big_file_threshold(ধ্রুবক অক্ষর *arg)
{
	unsigned long v;

	যদি (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

স্থির শূন্য option_signed_commits(ধ্রুবক অক্ষর *arg)
{
	যদি (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

স্থির শূন্য option_signed_tags(ধ্রুবক অক্ষর *arg)
{
	যদি (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

স্থির শূন্য option_quiet(শূন্য)
{
	show_stats = 0;
	quiet = 1;
}

স্থির পূর্ণসংখ্যা parse_one_option(কাঠামো fast_import_state *state, ধ্রুবক অক্ষর *option)
{
	যদি (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_আকার(option);
	} অন্যথা যদি (skip_prefix(option, "big-file-threshold=", &option)) {
		option_big_file_threshold(option);
	} অন্যথা যদি (skip_prefix(option, "depth=", &option)) {
		option_depth(option);
	} অন্যথা যদি (skip_prefix(option, "active-branches=", &option)) {
		option_active_branches(option);
	} অন্যথা যদি (skip_prefix(option, "export-pack-edges=", &option)) {
		option_export_pack_edges(state, option);
	} অন্যথা যদি (skip_prefix(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} অন্যথা যদি (skip_prefix(option, "signed-tags=", &option)) {
		option_signed_tags(option);
	} অন্যথা যদি (!strcmp(option, "quiet")) {
		option_quiet();
	} অন্যথা যদি (!strcmp(option, "stats")) {
		show_stats = 1;
	} অন্যথা যদি (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} অন্যথা {
		ফেরত 0;
	}

	ফেরত 1;
}

স্থির শূন্য check_unsafe_feature(কাঠামো fast_import_state *state,
				 ধ্রুবক অক্ষর *feature)
{
	যদি (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

স্থির পূর্ণসংখ্যা parse_one_feature(কাঠামো fast_import_state *state,
			     ধ্রুবক অক্ষর *feature)
{
	ধ্রুবক অক্ষর *arg;

	যদি (skip_prefix(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} অন্যথা যদি (skip_prefix(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} অন্যথা যদি (skip_prefix(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} অন্যথা যদি (skip_prefix(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} অন্যথা যদি (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} অন্যথা যদি (skip_prefix(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_to);
	} অন্যথা যদি (skip_prefix(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} অন্যথা যদি (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} অন্যথা যদি (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} অন্যথা যদি (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} অন্যথা যদি (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} অন্যথা যদি (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} অন্যথা যদি (!strcmp(feature, "force")) {
		force_update = 1;
	} অন্যথা যদি (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} অন্যথা {
		ফেরত 0;
	}

	ফেরত 1;
}

স্থির শূন্য parse_feature(কাঠামো fast_import_state *state, ধ্রুবক অক্ষর *feature)
{
	যদি (state->seen_তথ্য_command)
		die(_("got feature command '%s' after data command"), feature);

	যদি (parse_one_feature(state, feature))
		ফেরত;

	die(_("this version of fast-import does not support feature %s."), feature);
}

স্থির শূন্য parse_option(কাঠামো fast_import_state *state, ধ্রুবক অক্ষর *option)
{
	যদি (state->seen_তথ্য_command)
		die(_("got option command '%s' after data command"), option);

	যদি (parse_one_option(state, option))
		ফেরত;

	die(_("this version of fast-import does not support option: %s"), option);
}

স্থির শূন্য git_pack_config(শূন্য)
{
	পূর্ণসংখ্যা indexversion_মান;
	পূর্ণসংখ্যা limit;
	unsigned long packsizelimit_মান;

	যদি (!repo_config_get_ulong(the_repository, "pack.depth", &max_depth)) {
		যদি (max_depth > MAX_DEPTH)
			max_depth = MAX_DEPTH;
	}
	যদি (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_মান)) {
		pack_idx_opts.version = indexversion_মান;
		যদি (pack_idx_opts.version > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.version);
	}
	যদি (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_মান))
		max_packsize = packsizelimit_মান;

	যদি (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &limit))
		unpack_limit = limit;
	অন্যথা যদি (!repo_config_get_int(the_repository, "transfer.unpacklimit", &limit))
		unpack_limit = limit;

	repo_config(the_repository, git_default_config, ফাঁকা);
}

স্থির ধ্রুবক অক্ষর *ধ্রুবক fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	ফাঁকা
};

স্থির শূন্য parse_argv(কাঠামো fast_import_state *state)
{
	পূর্ণসংখ্যা argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	যদি (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_তথ্য_command = 1;
	যদি (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

স্থির পূর্ণসংখ্যা option_parse_date_format(ধ্রুবক কাঠামো option *opt UNUSED,
				    ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_export_pack_edges(ধ্রুবক কাঠামো option *opt,
					  ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->মান, arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_max_pack_আকার(ধ্রুবক কাঠামো option *opt UNUSED,
				      ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_আকার(arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_big_file_threshold(ধ্রুবক কাঠামো option *opt UNUSED,
					   ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_signed_commits(ধ্রুবক কাঠামো option *opt UNUSED,
				       ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_signed_tags(ধ্রুবক কাঠামো option *opt UNUSED,
				    ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_rewrite_submodules_from(ধ্রুবক কাঠামো option *opt,
						ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->মান, arg, &sub_marks_from);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_rewrite_submodules_to(ধ্রুবক কাঠামো option *opt,
					      ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->মান, arg, &sub_marks_to);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_cat_blob_fd(ধ্রুবক কাঠামো option *opt,
				    ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->মান, arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_import_marks(ধ্রুবক কাঠামো option *opt,
				     ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->মান, arg, 0, 0);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_import_marks_if_exists(ধ্রুবক কাঠামো option *opt,
					       ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->মান, arg, 0, 1);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_export_marks(ধ্রুবক কাঠামো option *opt,
				     ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->মান, arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_depth(ধ্রুবক কাঠামো option *opt UNUSED,
			      ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_active_branches(ধ্রুবক কাঠামো option *opt UNUSED,
					ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_quiet(ধ্রুবক কাঠামো option *opt UNUSED,
			      ধ্রুবক অক্ষর *arg UNUSED, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	ফেরত 0;
}

পূর্ণসংখ্যা cmd_fast_import(পূর্ণসংখ্যা argc,
		    ধ্রুবক অক্ষর **argv,
		    ধ্রুবক অক্ষর *prefix,
		    কাঠামো repository *repo)
{
	কাঠামো fast_import_state state;

	কাঠামো option fast_import_options[] = {
		OPT_GROUP(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", ফাঁকা, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", ফাঁকা, ফাঁকা,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", ফাঁকা, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_আকার),
		OPT_CALLBACK_F(0, "big-file-threshold", ফাঁকা, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_threshold),
		OPT_CALLBACK_F(0, "depth", ফাঁকা, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_depth),
		OPT_CALLBACK_F(0, "active-branches", ফাঁকা, N_("n"),
			       N_("maximum number of branches to maintain active"),
			       PARSE_OPT_NONEG, option_parse_active_branches),
		OPT_GROUP(N_("Marks")),
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
		OPT_GROUP(N_("Submodule rewrite")),
		OPT_CALLBACK_F(0, "rewrite-submodules-from", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> from <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_from),
		OPT_CALLBACK_F(0, "rewrite-submodules-to", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> to <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_to),
		OPT_GROUP(N_("Signing")),
		OPT_CALLBACK_F(0, "signed-commits", ফাঁকা, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", ফাঁকা, N_("mode"),
			       N_("how to handle signed tags"),
			       PARSE_OPT_NONEG, option_parse_signed_tags),
		OPT_HIDDEN_GROUP(N_("Advanced")),
		OPT_BOOL_F(0, "allow-unsafe-features", &state.allow_unsafe_features,
			   N_("allow unsafe mark commands from the stream"),
			   PARSE_OPT_HIDDEN | PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "export-pack-edges", &state, N_("file"),
			       N_("dump edge commits to <file>"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_export_pack_edges),
		OPT_CALLBACK_F(0, "cat-blob-fd", &state, N_("fd"),
			       N_("write some responses to <fd> instead of stdout"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_cat_blob_fd),
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv, fast_import_usage, fast_import_options);

	fast_import_state_init(&state, argc, argv, prefix, fast_import_options);

	reset_pack_idx_option(&pack_idx_opts);
	git_pack_config();

	alloc_objects(object_entry_alloc);
	strbuf_init(&command_buf, 0);
	CALLOC_ARRAY(atom_table, atom_table_sz);
	CALLOC_ARRAY(branch_table, branch_table_sz);
	CALLOC_ARRAY(avail_বৃক্ষ_table, avail_বৃক্ষ_table_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, আকার(কাঠামো mark_set));

	hashmap_init(&object_table, object_entry_hashcmp, ফাঁকা, 0);

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
	জন্য (পূর্ণসংখ্যা i = 1; i < argc; i++) {
		ধ্রুবক অক্ষর *arg = argv[i];
		যদি (*arg != '-' || !strcmp(arg, "--"))
			বিরতি;
		যদি (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * আকার(*rc_free));
	জন্য (unsigned পূর্ণসংখ্যা i = 0; i < (cmd_save - 1); i++)
		rc_free[i].পরবর্তী = &rc_free[i + 1];
	rc_free[cmd_save - 1].পরবর্তী = ফাঁকা;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	যখন (read_পরবর্তী_command(&state) != EOF) {
		ধ্রুবক অক্ষর *v;
		যদি (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		অন্যথা যদি (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		অন্যথা যদি (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		অন্যথা যদি (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		অন্যথা যদি (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, ফাঁকা);
		অন্যথা যদি (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		অন্যথা যদি (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		অন্যথা যদি (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		অন্যথা যদি (!strcmp("done", command_buf.buf))
			বিরতি;
		অন্যথা যদি (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		অন্যথা যদি (starts_with(command_buf.buf, "progress "))
			parse_progress(&state);
		অন্যথা যদি (skip_prefix(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		অন্যথা যদি (skip_prefix(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		অন্যথা যদি (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		অন্যথা
			die(_("unsupported command: %s"), command_buf.buf);

		যদি (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	যদি (!state.seen_তথ্য_command)
		parse_argv(&state);

	যদি (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	end_packfile();

	dump_branches();
	dump_tags();
	unkeep_all_packs();
	dump_marks();

	যদি (pack_edges)
		fclose(pack_edges);

	যদি (show_stats) {
		uintmax_t যোগ_গুনতি = 0, duplicate_গুনতি = 0;
		জন্য (আকার_t i = 0; i < ARRAY_আকার(object_গুনতি_by_type); i++)
			যোগ_গুনতি += object_গুনতি_by_type[i];
		জন্য (আকার_t i = 0; i < ARRAY_আকার(duplicate_গুনতি_by_type); i++)
			duplicate_গুনতি += duplicate_গুনতি_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_গুনতি);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", যোগ_গুনতি, duplicate_গুনতি);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_গুনতি_by_type[OBJ_BLOB], duplicate_গুনতি_by_type[OBJ_BLOB], delta_গুনতি_by_type[OBJ_BLOB], delta_গুনতি_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_গুনতি_by_type[OBJ_বৃক্ষ], duplicate_গুনতি_by_type[OBJ_বৃক্ষ], delta_গুনতি_by_type[OBJ_বৃক্ষ], delta_গুনতি_attempts_by_type[OBJ_বৃক্ষ]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_গুনতি_by_type[OBJ_COMMIT], duplicate_গুনতি_by_type[OBJ_COMMIT], delta_গুনতি_by_type[OBJ_COMMIT], delta_গুনতি_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_গুনতি_by_type[OBJ_TAG], duplicate_গুনতি_by_type[OBJ_TAG], delta_গুনতি_by_type[OBJ_TAG], delta_গুনতি_attempts_by_type[OBJ_TAG]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_গুনতি, branch_load_গুনতি);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->shift) * 1024, marks_set_গুনতি);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (বৃক্ষ_entry_allocd + fi_mem_pool.pool_alloc + alloc_গুনতি*আকার(কাঠামো object_entry))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (unsigned long)((বৃক্ষ_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_গুনতি*আকার(কাঠামো object_entry))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	ফেরত failure ? 1 : 0;
}
