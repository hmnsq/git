#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "abspath.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "lockfile.h"
#অন্তর্ভাৱ "object.h"
#অন্তর্ভাৱ "blob.h"
#অন্তর্ভাৱ "tree.h"
#অন্তর্ভাৱ "commit.h"
#অন্তর্ভাৱ "delta.h"
#অন্তর্ভাৱ "pack.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "read-cache-ll.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "csum-file.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "dir.h"
#অন্তর্ভাৱ "run-command.h"
#অন্তর্ভাৱ "packfile.h"
#অন্তর্ভাৱ "object-file.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "mem-pool.h"
#অন্তর্ভাৱ "commit-reach.h"
#অন্তর্ভাৱ "khash.h"
#অন্তর্ভাৱ "date.h"
#অন্তর্ভাৱ "gpg-interface.h"
#অন্তর্ভাৱ "parse-options.h"

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
#define PACK_SIZE_THRESHOLD (the_hash_algo->rawsz * 3)

সংরচনা object_entry {
	সংরচনা pack_idx_entry idx;
	সংরচনা hashmap_entry ent;
	uint32_t type : TYPE_BITS,
		pack_id : PACK_ID_BITS,
		depth : DEPTH_BITS;
};

স্থির পূর্ণাঙ্ক object_entry_hashcmp(ধ্রুৱ শূন্য *map_data UNUSED,
				ধ্রুৱ সংরচনা hashmap_entry *eptr,
				ধ্রুৱ সংরচনা hashmap_entry *entry_or_চাবি,
				ধ্রুৱ শূন্য *keydata)
{
	ধ্রুৱ সংরচনা object_id *oid = keydata;
	ধ্রুৱ সংরচনা object_entry *e1, *e2;

	e1 = container_of(eptr, ধ্রুৱ সংরচনা object_entry, ent);
	যদি (oid)
		ঘূৰাই oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_চাবি, ধ্রুৱ সংরচনা object_entry, ent);
	ঘূৰাই oidcmp(&e1->idx.oid, &e2->idx.oid);
}

সংরচনা object_entry_pool {
	সংরচনা object_entry_pool *next_pool;
	সংরচনা object_entry *next_free;
	সংরচনা object_entry *end;
	সংরচনা object_entry entries[FLEX_ARRAY]; /* more */
};

সংরচনা mark_set {
	সঙ্ঘ {
		সংরচনা object_id *oids[1024];
		সংরচনা object_entry *marked[1024];
		সংরচনা mark_set *sets[1024];
	} data;
	unsigned পূর্ণাঙ্ক shift;
};

সংরচনা last_object {
	সংরচনা strbuf data;
	off_t offset;
	unsigned পূর্ণাঙ্ক depth;
	unsigned no_swap : 1;
};

সংরচনা atom_str {
	সংরচনা atom_str *next_atom;
	unsigned short str_len;
	ৱর্ণ str_dat[FLEX_ARRAY]; /* more */
};

সংরচনা বৃক্ষ_content;
সংরচনা বৃক্ষ_entry {
	সংরচনা বৃক্ষ_content *বৃক্ষ;
	সংরচনা atom_str *name;
	সংরচনা বৃক্ষ_entry_ms {
		uint16_t mode;
		সংরচনা object_id oid;
	} versions[2];
};

সংরচনা বৃক্ষ_content {
	unsigned পূর্ণাঙ্ক entry_capacity; /* must match avail_tree_content */
	unsigned পূর্ণাঙ্ক entry_গুনতি;
	unsigned পূর্ণাঙ্ক delta_depth;
	সংরচনা বৃক্ষ_entry *entries[FLEX_ARRAY]; /* more */
};

সংরচনা avail_বৃক্ষ_content {
	unsigned পূর্ণাঙ্ক entry_capacity; /* must match tree_content */
	সংরচনা avail_বৃক্ষ_content *next_avail;
};

সংরচনা branch {
	সংরচনা branch *table_next_branch;
	সংরচনা branch *active_next_branch;
	ধ্রুৱ ৱর্ণ *name;
	সংরচনা বৃক্ষ_entry branch_বৃক্ষ;
	uintmax_t last_commit;
	uintmax_t অঙ্ক_notes;
	unsigned active : 1;
	unsigned delete : 1;
	unsigned pack_id : PACK_ID_BITS;
	সংরচনা object_id oid;
};

সংরচনা tag {
	সংরচনা tag *next_tag;
	ধ্রুৱ ৱর্ণ *name;
	unsigned পূর্ণাঙ্ক pack_id;
	সংরচনা object_id oid;
};

সংরচনা hash_list {
	সংরচনা hash_list *next;
	সংরচনা object_id oid;
};

প্রকারনির্দেশ গণনা {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

সংরচনা recent_command {
	সংরচনা recent_command *prev;
	সংরচনা recent_command *next;
	ৱর্ণ *buf;
};

প্রকারনির্দেশ শূন্য (*mark_set_inserter_t)(সংরচনা mark_set **s, সংরচনা object_id *oid, uintmax_t mark);
প্রকারনির্দেশ শূন্য (*each_mark_fn_t)(uintmax_t mark, শূন্য *obj, শূন্য *cbp);

/* Configured limits on output */
স্থির unsigned long max_depth = 50;
স্থির off_t max_packsize;
স্থির পূর্ণাঙ্ক unpack_limit = 100;
স্থির পূর্ণাঙ্ক force_update;

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
স্থির পূর্ণাঙ্ক failure;
স্থির FILE *pack_edges;
স্থির পূর্ণাঙ্ক show_stats = 1;
স্থির পূর্ণাঙ্ক quiet;
স্থির গণনা sign_mode signed_tag_mode = SIGN_VERBATIM;
স্থির গণনা sign_mode signed_commit_mode = SIGN_VERBATIM;
স্থির ধ্রুৱ ৱর্ণ *signed_commit_keyid;
স্থির ধ্রুৱ ৱর্ণ *signed_tag_keyid;

/* Memory pools */
স্থির সংরচনা mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - পরিমাণ(সংরচনা mp_block),
};

/* Atom management */
স্থির unsigned পূর্ণাঙ্ক atom_table_sz = 4451;
স্থির unsigned পূর্ণাঙ্ক atom_cnt;
স্থির সংরচনা atom_str **atom_table;

/* The .pack file being generated */
স্থির সংরচনা pack_idx_option pack_idx_opts;
স্থির unsigned পূর্ণাঙ্ক pack_id;
স্থির সংরচনা hashfile *pack_file;
স্থির সংরচনা packed_git *pack_data;
স্থির সংরচনা packed_git **all_packs;
স্থির off_t pack_size;

/* Table of objects we've written. */
স্থির unsigned পূর্ণাঙ্ক object_entry_alloc = 5000;
স্থির সংরচনা object_entry_pool *blocks;
স্থির সংরচনা hashmap object_table;
স্থির সংরচনা mark_set *marks;
স্থির ৱর্ণ *export_marks_file;
স্থির ৱর্ণ *import_marks_file;
স্থির পূর্ণাঙ্ক import_marks_file_from_stream;
স্থির পূর্ণাঙ্ক import_marks_file_ignore_missing;
স্থির পূর্ণাঙ্ক import_marks_file_done;
স্থির পূর্ণাঙ্ক relative_marks_paths;

/* Our last blob */
স্থির সংরচনা last_object last_blob = {
	.data = STRBUF_INIT,
 };

/* Tree management */
স্থির unsigned পূর্ণাঙ্ক বৃক্ষ_entry_alloc = 1000;
স্থির শূন্য *avail_বৃক্ষ_entry;
স্থির unsigned পূর্ণাঙ্ক avail_বৃক্ষ_table_sz = 100;
স্থির সংরচনা avail_বৃক্ষ_content **avail_বৃক্ষ_table;
স্থির size_t বৃক্ষ_entry_allocd;
স্থির সংরচনা strbuf old_বৃক্ষ = STRBUF_INIT;
স্থির সংরচনা strbuf new_বৃক্ষ = STRBUF_INIT;

/* Branch data */
স্থির unsigned long max_active_branches = 5;
স্থির unsigned long cur_active_branches;
স্থির unsigned long branch_table_sz = 1039;
স্থির সংরচনা branch **branch_table;
স্থির সংরচনা branch *active_branches;

/* Tag data */
স্থির সংরচনা tag *first_tag;
স্থির সংরচনা tag *last_tag;

/* Input stream parsing */
স্থির whenspec_type whenspec = WHENSPEC_RAW;
স্থির সংরচনা strbuf command_buf = STRBUF_INIT;
স্থির পূর্ণাঙ্ক unread_command_buf;
স্থির সংরচনা recent_command cmd_hist = {
	.prev = &cmd_hist,
	.next = &cmd_hist,
};
স্থির সংরচনা recent_command *cmd_tail = &cmd_hist;
স্থির সংরচনা recent_command *rc_free;
স্থির unsigned পূর্ণাঙ্ক cmd_save = 100;
স্থির uintmax_t next_mark;
স্থির সংরচনা strbuf new_data = STRBUF_INIT;
স্থির পূর্ণাঙ্ক require_explicit_termination;

/* Signal handling */
স্থির চঞ্চল sig_atomic_t checkpoint_requested;

/* Submodule marks */
স্থির সংরচনা string_list sub_marks_from = STRING_LIST_INIT_DUP;
স্থির সংরচনা string_list sub_marks_to = STRING_LIST_INIT_DUP;
স্থির kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
স্থির পূর্ণাঙ্ক cat_blob_fd = STDOUT_FILENO;

/* Command state */
সংরচনা fast_import_state {
	পূর্ণাঙ্ক argc;
	ধ্রুৱ ৱর্ণ **argv;
	ধ্রুৱ ৱর্ণ *prefix;
	পূর্ণাঙ্ক seen_data_command;
	পূর্ণাঙ্ক allow_unsafe_features;
	সংরচনা option *option;
};

স্থির শূন্য fast_import_state_init(সংরচনা fast_import_state *state,
				   পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv,
				   ধ্রুৱ ৱর্ণ *prefix, সংরচনা option *option)
{
	memset(state, 0, পরিমাণ(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

স্থির শূন্য parse_argv(সংরচনা fast_import_state *state);
স্থির শূন্য parse_get_mark(সংরচনা fast_import_state *state, ধ্রুৱ ৱর্ণ *p);
স্থির শূন্য parse_cat_blob(সংরচনা fast_import_state *state, ধ্রুৱ ৱর্ণ *p);
স্থির শূন্য parse_ls(সংরচনা fast_import_state *state,
		     ধ্রুৱ ৱর্ণ *p,
		     সংরচনা branch *b);

স্থির শূন্য for_each_mark(সংরচনা mark_set *m, uintmax_t base, each_mark_fn_t callback, শূন্য *p)
{
	uintmax_t k;
	যদি (m->shift) {
		বাবে (k = 0; k < 1024; k++) {
			যদি (m->data.sets[k])
				for_each_mark(m->data.sets[k], base + (k << m->shift), callback, p);
		}
	} অন্যথা {
		বাবে (k = 0; k < 1024; k++) {
			যদি (m->data.marked[k])
				callback(base + k, m->data.marked[k], p);
		}
	}
}

স্থির শূন্য dump_marks_fn(uintmax_t mark, শূন্য *object, শূন্য *cbp) {
	সংরচনা object_entry *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

স্থির শূন্য write_branch_report(FILE *rpt, সংরচনা branch *b)
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

স্থির শূন্য write_crash_report(ধ্রুৱ ৱর্ণ *err)
{
	ৱর্ণ *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	সংরচনা branch *b;
	unsigned long lu;
	সংরচনা recent_command *rc;

	যদি (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		মুক্ত(loc);
		ঘূৰাই;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(time(রিক্ত), 0, DATE_MODE(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	বাবে (rc = cmd_hist.next; rc != &cmd_hist; rc = rc->next) {
		যদি (rc->next == &cmd_hist)
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
	বাবে (b = active_branches, lu = 0; b; b = b->active_next_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	বাবে (lu = 0; lu < branch_table_sz; lu++) {
		বাবে (b = branch_table[lu]; b; b = b->table_next_branch)
			write_branch_report(rpt, b);
	}

	যদি (first_tag) {
		সংরচনা tag *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		বাবে (tg = first_tag; tg; tg = tg->next_tag) {
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

স্থির NORETURN শূন্য die_nicely(ধ্রুৱ ৱর্ণ *err, va_list params)
{
	va_list cp;
	স্থির পূর্ণাঙ্ক zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	যদি (!zombie) {
		ৱর্ণ message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, পরিমাণ(message), err, cp);
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

স্থির শূন্য checkpoint_signal(পূর্ণাঙ্ক signo UNUSED)
{
	checkpoint_requested = 1;
}

স্থির শূন্য set_checkpoint_signal(শূন্য)
{
	সংরচনা sigaction sa;

	memset(&sa, 0, পরিমাণ(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, রিক্ত);
}

#endif

স্থির শূন্য alloc_objects(unsigned পূর্ণাঙ্ক cnt)
{
	সংরচনা object_entry_pool *b;

	b = xmalloc(পরিমাণ(সংরচনা object_entry_pool)
		+ cnt * পরিমাণ(সংরচনা object_entry));
	b->next_pool = blocks;
	b->next_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_গুনতি += cnt;
}

স্থির সংরচনা object_entry *new_object(সংরচনা object_id *oid)
{
	সংরচনা object_entry *e;

	যদি (blocks->next_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->next_free++;
	oidcpy(&e->idx.oid, oid);
	ঘূৰাই e;
}

স্থির সংরচনা object_entry *find_object(সংরচনা object_id *oid)
{
	ঘূৰাই hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   সংরচনা object_entry, ent);
}

স্থির সংরচনা object_entry *ঢোকাও_object(সংরচনা object_id *oid)
{
	সংরচনা object_entry *e;
	unsigned পূর্ণাঙ্ক hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					সংরচনা object_entry, ent);
	যদি (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	ঘূৰাই e;
}

স্থির শূন্য invalidate_pack_id(unsigned পূর্ণাঙ্ক id)
{
	unsigned long lu;
	সংরচনা tag *t;
	সংরচনা hashmap_iter iter;
	সংরচনা object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		যদি (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	বাবে (lu = 0; lu < branch_table_sz; lu++) {
		সংরচনা branch *b;

		বাবে (b = branch_table[lu]; b; b = b->table_next_branch)
			যদি (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	বাবে (t = first_tag; t; t = t->next_tag)
		যদি (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

স্থির unsigned পূর্ণাঙ্ক hc_str(ধ্রুৱ ৱর্ণ *s, size_t len)
{
	unsigned পূর্ণাঙ্ক r = 0;
	যেতিয়ালৈ (len-- > 0)
		r = r * 31 + *s++;
	ঘূৰাই r;
}

স্থির শূন্য ঢোকাও_mark(সংরচনা mark_set **শীৰ্ষ, uintmax_t idnum, সংরচনা object_entry *oe)
{
	সংরচনা mark_set *s = *শীৰ্ষ;

	যেতিয়ালৈ ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, পরিমাণ(সংরচনা mark_set));
		s->shift = (*শীৰ্ষ)->shift + 10;
		s->data.sets[0] = *শীৰ্ষ;
		*শীৰ্ষ = s;
	}
	যেতিয়ালৈ (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		যদি (!s->data.sets[i]) {
			s->data.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, পরিমাণ(সংরচনা mark_set));
			s->data.sets[i]->shift = s->shift - 10;
		}
		s = s->data.sets[i];
	}
	যদি (!s->data.marked[idnum])
		marks_set_গুনতি++;
	s->data.marked[idnum] = oe;
}

স্থির শূন্য *find_mark(সংরচনা mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	সংরচনা object_entry *oe = রিক্ত;
	যদি ((idnum >> s->shift) < 1024) {
		যেতিয়ালৈ (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->data.sets[i];
		}
		যদি (s)
			oe = s->data.marked[idnum];
	}
	যদি (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	ঘূৰাই oe;
}

স্থির সংরচনা atom_str *to_atom(ধ্রুৱ ৱর্ণ *s, unsigned short len)
{
	unsigned পূর্ণাঙ্ক hc = hc_str(s, len) % atom_table_sz;
	সংরচনা atom_str *c;

	বাবে (c = atom_table[hc]; c; c = c->next_atom)
		যদি (c->str_len == len && !strncmp(s, c->str_dat, len))
			ঘূৰাই c;

	c = mem_pool_alloc(&fi_mem_pool, পরিমাণ(সংরচনা atom_str) + len + 1);
	c->str_len = len;
	memcpy(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->next_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	ঘূৰাই c;
}

স্থির সংরচনা branch *lookup_branch(ধ্রুৱ ৱর্ণ *name)
{
	unsigned পূর্ণাঙ্ক hc = hc_str(name, strlen(name)) % branch_table_sz;
	সংরচনা branch *b;

	বাবে (b = branch_table[hc]; b; b = b->table_next_branch)
		যদি (!strcmp(name, b->name))
			ঘূৰাই b;
	ঘূৰাই রিক্ত;
}

স্থির সংরচনা branch *new_branch(ধ্রুৱ ৱর্ণ *name)
{
	unsigned পূর্ণাঙ্ক hc = hc_str(name, strlen(name)) % branch_table_sz;
	সংরচনা branch *b = lookup_branch(name);

	যদি (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	যদি (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, পরিমাণ(সংরচনা branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_next_branch = branch_table[hc];
	b->branch_বৃক্ষ.versions[0].mode = S_IFDIR;
	b->branch_বৃক্ষ.versions[1].mode = S_IFDIR;
	b->অঙ্ক_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_গুনতি++;
	ঘূৰাই b;
}

স্থির unsigned পূর্ণাঙ্ক hc_entries(unsigned পূর্ণাঙ্ক cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	ঘূৰাই cnt < avail_বৃক্ষ_table_sz ? cnt : avail_বৃক্ষ_table_sz - 1;
}

স্থির সংরচনা বৃক্ষ_content *new_বৃক্ষ_content(unsigned পূর্ণাঙ্ক cnt)
{
	সংরচনা avail_বৃক্ষ_content *f, *l = রিক্ত;
	সংরচনা বৃক্ষ_content *t;
	unsigned পূর্ণাঙ্ক hc = hc_entries(cnt);

	বাবে (f = avail_বৃক্ষ_table[hc]; f; l = f, f = f->next_avail)
		যদি (f->entry_capacity >= cnt)
			ভঙ্গ;

	যদি (f) {
		যদি (l)
			l->next_avail = f->next_avail;
		অন্যথা
			avail_বৃক্ষ_table[hc] = f->next_avail;
	} অন্যথা {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, পরিমাণ(*t) + পরিমাণ(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (সংরচনা বৃক্ষ_content*)f;
	t->entry_গুনতি = 0;
	t->delta_depth = 0;
	ঘূৰাই t;
}

স্থির শূন্য release_বৃক্ষ_entry(সংরচনা বৃক্ষ_entry *e);
স্থির শূন্য release_বৃক্ষ_content(সংরচনা বৃক্ষ_content *t)
{
	সংরচনা avail_বৃক্ষ_content *f = (সংরচনা avail_বৃক্ষ_content*)t;
	unsigned পূর্ণাঙ্ক hc = hc_entries(f->entry_capacity);
	f->next_avail = avail_বৃক্ষ_table[hc];
	avail_বৃক্ষ_table[hc] = f;
}

স্থির শূন্য release_বৃক্ষ_content_recursive(সংরচনা বৃক্ষ_content *t)
{
	unsigned পূর্ণাঙ্ক i;
	বাবে (i = 0; i < t->entry_গুনতি; i++)
		release_বৃক্ষ_entry(t->entries[i]);
	release_বৃক্ষ_content(t);
}

স্থির সংরচনা বৃক্ষ_content *grow_বৃক্ষ_content(
	সংরচনা বৃক্ষ_content *t,
	পূর্ণাঙ্ক amt)
{
	সংরচনা বৃক্ষ_content *r = new_বৃক্ষ_content(t->entry_গুনতি + amt);
	r->entry_গুনতি = t->entry_গুনতি;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_গুনতি);
	release_বৃক্ষ_content(t);
	ঘূৰাই r;
}

স্থির সংরচনা বৃক্ষ_entry *new_বৃক্ষ_entry(শূন্য)
{
	সংরচনা বৃক্ষ_entry *e;

	যদি (!avail_বৃক্ষ_entry) {
		unsigned পূর্ণাঙ্ক n = বৃক্ষ_entry_alloc;
		বৃক্ষ_entry_allocd += n * পরিমাণ(সংরচনা বৃক্ষ_entry);
		ALLOC_ARRAY(e, n);
		avail_বৃক্ষ_entry = e;
		যেতিয়ালৈ (n-- > 1) {
			*((শূন্য**)e) = e + 1;
			e++;
		}
		*((শূন্য**)e) = রিক্ত;
	}

	e = avail_বৃক্ষ_entry;
	avail_বৃক্ষ_entry = *((শূন্য**)e);
	ঘূৰাই e;
}

স্থির শূন্য release_বৃক্ষ_entry(সংরচনা বৃক্ষ_entry *e)
{
	যদি (e->বৃক্ষ)
		release_বৃক্ষ_content_recursive(e->বৃক্ষ);
	*((শূন্য**)e) = avail_বৃক্ষ_entry;
	avail_বৃক্ষ_entry = e;
}

স্থির সংরচনা বৃক্ষ_content *dup_বৃক্ষ_content(সংরচনা বৃক্ষ_content *s)
{
	সংরচনা বৃক্ষ_content *d;
	সংরচনা বৃক্ষ_entry *a, *b;
	unsigned পূর্ণাঙ্ক i;

	যদি (!s)
		ঘূৰাই রিক্ত;
	d = new_বৃক্ষ_content(s->entry_গুনতি);
	বাবে (i = 0; i < s->entry_গুনতি; i++) {
		a = s->entries[i];
		b = new_বৃক্ষ_entry();
		memcpy(b, a, পরিমাণ(*a));
		যদি (a->বৃক্ষ && is_null_oid(&b->versions[1].oid))
			b->বৃক্ষ = dup_বৃক্ষ_content(a->বৃক্ষ);
		অন্যথা
			b->বৃক্ষ = রিক্ত;
		d->entries[i] = b;
	}
	d->entry_গুনতি = s->entry_গুনতি;
	d->delta_depth = s->delta_depth;

	ঘূৰাই d;
}

স্থির শূন্য start_packfile(শূন্য)
{
	সংরচনা strbuf tmp_file = STRBUF_INIT;
	সংরচনা packed_git *p;
	পূর্ণাঙ্ক pack_fd;

	pack_fd = odb_mkstemp(the_repository->objects, &tmp_file,
			      "pack/tmp_pack_XXXXXX");
	FLEX_ALLOC_STR(p, pack_name, tmp_file.buf);
	strbuf_release(&tmp_file);

	p->pack_fd = pack_fd;
	p->do_not_close = 1;
	p->repo = the_repository;
	pack_file = hashfd(the_repository->hash_algo, pack_fd, p->pack_name);

	pack_data = p;
	pack_size = write_pack_header(pack_file, 0);
	object_গুনতি = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

স্থির ধ্রুৱ ৱর্ণ *create_index(শূন্য)
{
	ধ্রুৱ ৱর্ণ *tmpfile;
	সংরচনা pack_idx_entry **idx, **c, **last;
	সংরচনা object_entry *e;
	সংরচনা object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_গুনতি);
	c = idx;
	বাবে (o = blocks; o; o = o->next_pool)
		বাবে (e = o->next_free; e-- != o->entries;)
			যদি (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_গুনতি;
	যদি (c != last)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, রিক্ত, idx, object_গুনতি,
				 &pack_idx_opts, pack_data->hash);
	মুক্ত(idx);
	ঘূৰাই tmpfile;
}

স্থির ৱর্ণ *keep_pack(ধ্রুৱ ৱর্ণ *curr_index_name)
{
	স্থির ধ্রুৱ ৱর্ণ *keep_বাৰ্তা = "fast-import";
	সংরচনা strbuf name = STRBUF_INIT;
	পূর্ণাঙ্ক keep_fd;

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_data->repo,
							    name.buf);
	যদি (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_বাৰ্তা, strlen(keep_বাৰ্তা));
	যদি (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "pack");
	যদি (finalize_object_file(pack_data->repo, pack_data->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "idx");
	যদি (finalize_object_file(pack_data->repo, curr_index_name, name.buf))
		die(_("cannot store index file"));
	মুক্ত((শূন্য *)curr_index_name);
	ঘূৰাই strbuf_detach(&name, রিক্ত);
}

স্থির শূন্য unkeep_all_packs(শূন্য)
{
	সংরচনা strbuf name = STRBUF_INIT;
	পূর্ণাঙ্ক k;

	বাবে (k = 0; k < pack_id; k++) {
		সংরচনা packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

স্থির পূর্ণাঙ্ক loosen_small_pack(ধ্রুৱ সংরচনা packed_git *p)
{
	সংরচনা child_process unpack = CHILD_PROCESS_INIT;

	যদি (lseek(p->pack_fd, 0, SEEK_SET) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_ঠেলো(&unpack.args, "unpack-objects");
	যদি (!show_stats)
		strvec_ঠেলো(&unpack.args, "-q");

	ঘূৰাই run_command(&unpack);
}

স্থির শূন্য end_packfile(শূন্য)
{
	স্থির পূর্ণাঙ্ক running;

	যদি (running || !pack_data)
		ঘূৰাই;

	running = 1;
	clear_delta_base_cache();
	যদি (object_গুনতি) {
		সংরচনা odb_source_files *files = odb_source_files_downcast(pack_data->repo->objects->source);
		সংরচনা packed_git *new_p;
		সংরচনা object_id cur_pack_oid;
		ৱর্ণ *idx_name;
		পূর্ণাঙ্ক i;
		সংরচনা branch *b;
		সংরচনা tag *t;

		close_pack_windows(pack_data);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_data->pack_fd,
					 pack_data->hash, pack_data->pack_name,
					 object_গুনতি, cur_pack_oid.hash,
					 pack_size);

		যদি (object_গুনতি <= unpack_limit) {
			যদি (!loosen_small_pack(pack_data)) {
				invalidate_pack_id(pack_id);
				গচ্ছ discard_pack;
			}
		}

		close(pack_data->pack_fd);
		idx_name = keep_pack(create_index());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		যদি (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		মুক্ত(idx_name);

		/* Print the boundary */
		যদি (pack_edges) {
			fprintf(pack_edges, "%s:", new_p->pack_name);
			বাবে (i = 0; i < branch_table_sz; i++) {
				বাবে (b = branch_table[i]; b; b = b->table_next_branch) {
					যদি (b->pack_id == pack_id)
						fprintf(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			বাবে (t = first_tag; t; t = t->next_tag) {
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
		close(pack_data->pack_fd);
		unlink_or_warn(pack_data->pack_name);
	}
	FREE_AND_NULL(pack_data);
	running = 0;

	/* We can't carry a delta across packfiles. */
	strbuf_release(&last_blob.data);
	last_blob.offset = 0;
	last_blob.depth = 0;
}

স্থির শূন্য cycle_packfile(শূন্য)
{
	end_packfile();
	start_packfile();
}

স্থির পূর্ণাঙ্ক store_object(
	গণনা object_type type,
	সংরচনা strbuf *dat,
	সংরচনা last_object *last,
	সংরচনা object_id *oidout,
	uintmax_t mark)
{
	সংরচনা odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	শূন্য *out, *delta;
	সংরচনা object_entry *e;
	unsigned ৱর্ণ hdr[96];
	সংরচনা object_id oid;
	unsigned long hdrlen, deltalen = 0;
	সংরচনা git_hash_ctx c;
	git_zstream s;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	hdrlen = format_object_header((ৱর্ণ *)hdr, পরিমাণ(hdr), type,
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
		ঘূৰাই 1;
	}

	বাবে (সংরচনা odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		যদি (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			অনুৱর্ত;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_গুনতি_by_type[type]++;
		ঘূৰাই 1;
	}

	যদি (last && last->data.len && last->data.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		size_t deltalen_st;

		delta_গুনতি_attempts_by_type[type]++;
		delta = diff_delta(last->data.buf, last->data.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_size_t_to_ulong(deltalen_st);
	} অন্যথা
		delta = রিক্ত;

	git_deflate_init(&s, cfg->pack_compression_level);
	যদি (delta) {
		s.next_in = delta;
		s.avail_in = deltalen;
	} অন্যথা {
		s.next_in = (শূন্য *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.next_out = out = xmalloc(s.avail_out);
	যেতিয়ালৈ (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	যদি ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + s.total_out) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + s.total_out) < pack_size) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		যদি (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.next_in = (শূন্য *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.next_out = out = xrealloc(out, s.avail_out);
			যেতিয়ালৈ (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_end(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.offset = pack_size;
	object_গুনতি++;
	object_গুনতি_by_type[type]++;

	crc32_begin(pack_file);

	যদি (delta) {
		off_t ofs = e->idx.offset - last->offset;
		unsigned pos = পরিমাণ(hdr) - 1;

		delta_গুনতি_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, পরিমাণ(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;

		hdr[pos] = ofs & 127;
		যেতিয়ালৈ (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, পরিমাণ(hdr) - pos);
		pack_size += পরিমাণ(hdr) - pos;
	} অন্যথা {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, পরিমাণ(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;
	}

	hashwrite(pack_file, out, s.total_out);
	pack_size += s.total_out;

	e->idx.crc32 = crc32_end(pack_file);

	মুক্ত(out);
	মুক্ত(delta);
	যদি (last) {
		যদি (last->no_swap) {
			last->data = *dat;
		} অন্যথা {
			strbuf_swap(&last->data, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	ঘূৰাই 0;
}

স্থির শূন্য truncate_pack(সংরচনা hashfile_checkpoint *checkpoint)
{
	যদি (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_size = checkpoint->offset;
}

স্থির শূন্য stream_blob(uintmax_t len, সংরচনা object_id *oidout, uintmax_t mark)
{
	সংরচনা odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	size_t in_sz = 64 * 1024, out_sz = 64 * 1024;
	unsigned ৱর্ণ *in_buf = xmalloc(in_sz);
	unsigned ৱর্ণ *out_buf = xmalloc(out_sz);
	সংরচনা object_entry *e;
	সংরচনা object_id oid;
	unsigned long hdrlen;
	off_t offset;
	সংরচনা git_hash_ctx c;
	git_zstream s;
	সংরচনা hashfile_checkpoint checkpoint;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	পূর্ণাঙ্ক status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	যদি ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + len) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + len) < pack_size)
		cycle_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	offset = checkpoint.offset;

	hdrlen = format_object_header((ৱর্ণ *)out_buf, out_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, out_buf, hdrlen);

	crc32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_in_pack_object_header(out_buf, out_sz, OBJ_BLOB, len);

	s.next_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	যেতিয়ালৈ (status != Z_STREAM_END) {
		যদি (0 < len && !s.avail_in) {
			size_t cnt = in_sz < len ? in_sz : (size_t)len;
			size_t n = fread(in_buf, 1, cnt, stdin);
			যদি (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.next_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		যদি (!s.avail_out || status == Z_STREAM_END) {
			size_t n = s.next_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_size += n;
			s.next_out = out_buf;
			s.avail_out = out_sz;
		}

		ৱিকল্প (status) {
		পক্ষ Z_OK:
		পক্ষ Z_BUF_ERROR:
		পক্ষ Z_STREAM_END:
			অনুৱর্ত;
		স্ৱভাৱ:
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
		গচ্ছ out;
	}

	বাবে (সংরচনা odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		যদি (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			অনুৱর্ত;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_গুনতি_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		গচ্ছ out;
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
	সংরচনা object_entry *oe,
	unsigned long *sizep)
{
	গণনা object_type type;
	size_t size_st = 0;
	শূন্য *data;
	সংরচনা packed_git *p = all_packs[oe->pack_id];
	যদি (p == pack_data && p->pack_size < (pack_size + the_hash_algo->rawsz)) {
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
		p->pack_size = pack_size + the_hash_algo->rawsz;
	}
	data = unpack_entry(the_repository, p, oe->idx.offset, &type, &size_st);
	যদি (sizep)
		*sizep = cast_size_t_to_ulong(size_st);
	ঘূৰাই data;
}

স্থির শূন্য load_বৃক্ষ(সংরচনা বৃক্ষ_entry *মূল)
{
	সংরচনা object_id *oid = &মূল->versions[1].oid;
	সংরচনা object_entry *myoe;
	সংরচনা বৃক্ষ_content *t;
	unsigned long size;
	ৱর্ণ *buf;
	ধ্রুৱ ৱর্ণ *c;

	মূল->বৃক্ষ = t = new_বৃক্ষ_content(8);
	যদি (is_null_oid(oid))
		ঘূৰাই;

	myoe = find_object(oid);
	যদি (myoe && myoe->pack_id != MAX_PACK_ID) {
		যদি (myoe->type != OBJ_বৃক্ষ)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &size);
		যদি (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} অন্যথা {
		গণনা object_type type;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		যদি (!buf || type != OBJ_বৃক্ষ)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	যেতিয়ালৈ (c != (buf + size)) {
		সংরচনা বৃক্ষ_entry *e = new_বৃক্ষ_entry();

		যদি (t->entry_গুনতি == t->entry_capacity)
			মূল->বৃক্ষ = t = grow_বৃক্ষ_content(t, t->entry_গুনতি);
		t->entries[t->entry_গুনতি++] = e;

		e->বৃক্ষ = রিক্ত;
		c = parse_mode(c, &e->versions[1].mode);
		যদি (!c)
			die(_("corrupt mode in %s"), oid_to_hex(oid));
		e->versions[0].mode = e->versions[1].mode;
		e->name = to_atom(c, strlen(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (unsigned ৱর্ণ *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (unsigned ৱর্ণ *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	মুক্ত(buf);
}

স্থির পূর্ণাঙ্ক tecmp0 (ধ্রুৱ শূন্য *_a, ধ্রুৱ শূন্য *_b)
{
	সংরচনা বৃক্ষ_entry *a = *((সংরচনা বৃক্ষ_entry**)_a);
	সংরচনা বৃক্ষ_entry *b = *((সংরচনা বৃক্ষ_entry**)_b);
	ঘূৰাই base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

স্থির পূর্ণাঙ্ক tecmp1 (ধ্রুৱ শূন্য *_a, ধ্রুৱ শূন্য *_b)
{
	সংরচনা বৃক্ষ_entry *a = *((সংরচনা বৃক্ষ_entry**)_a);
	সংরচনা বৃক্ষ_entry *b = *((সংরচনা বৃক্ষ_entry**)_b);
	ঘূৰাই base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

স্থির শূন্য mktree(সংরচনা বৃক্ষ_content *t, পূর্ণাঙ্ক v, সংরচনা strbuf *b)
{
	size_t maxlen = 0;
	unsigned পূর্ণাঙ্ক i;

	যদি (!v)
		QSORT(t->entries, t->entry_গুনতি, tecmp0);
	অন্যথা
		QSORT(t->entries, t->entry_গুনতি, tecmp1);

	বাবে (i = 0; i < t->entry_গুনতি; i++) {
		যদি (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	বাবে (i = 0; i < t->entry_গুনতি; i++) {
		সংরচনা বৃক্ষ_entry *e = t->entries[i];
		যদি (!e->versions[v].mode)
			অনুৱর্ত;
		strbuf_addf(b, "%o %s%c",
			(unsigned পূর্ণাঙ্ক)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

স্থির শূন্য store_বৃক্ষ(সংরচনা বৃক্ষ_entry *মূল)
{
	সংরচনা বৃক্ষ_content *t;
	unsigned পূর্ণাঙ্ক i, j, del;
	সংরচনা last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	সংরচনা object_entry *le = রিক্ত;

	যদি (!is_null_oid(&মূল->versions[1].oid))
		ঘূৰাই;

	যদি (!মূল->বৃক্ষ)
		load_বৃক্ষ(মূল);
	t = মূল->বৃক্ষ;

	বাবে (i = 0; i < t->entry_গুনতি; i++) {
		যদি (t->entries[i]->বৃক্ষ)
			store_বৃক্ষ(t->entries[i]);
	}

	যদি (!(মূল->versions[0].mode & NO_DELTA))
		le = find_object(&মূল->versions[0].oid);
	যদি (S_ISDIR(মূল->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_বৃক্ষ);
		lo.data = old_বৃক্ষ;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_বৃক্ষ);
	store_object(OBJ_বৃক্ষ, &new_বৃক্ষ, &lo, &মূল->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	বাবে (i = 0, j = 0, del = 0; i < t->entry_গুনতি; i++) {
		সংরচনা বৃক্ষ_entry *e = t->entries[i];
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
	সংরচনা বৃক্ষ_entry *মূল,
	ধ্রুৱ সংরচনা object_id *oid,
	ধ্রুৱ uint16_t mode,
	সংরচনা বৃক্ষ_content *newtree)
{
	যদি (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&মূল->versions[0].oid, the_repository->hash_algo);
	oidcpy(&মূল->versions[1].oid, oid);
	যদি (মূল->বৃক্ষ)
		release_বৃক্ষ_content_recursive(মূল->বৃক্ষ);
	মূল->বৃক্ষ = newtree;
}

স্থির পূর্ণাঙ্ক বৃক্ষ_content_set(
	সংরচনা বৃক্ষ_entry *মূল,
	ধ্রুৱ ৱর্ণ *p,
	ধ্রুৱ সংরচনা object_id *oid,
	ধ্রুৱ uint16_t mode,
	সংরচনা বৃক্ষ_content *subtree)
{
	সংরচনা বৃক্ষ_content *t;
	ধ্রুৱ ৱর্ণ *slash1;
	unsigned পূর্ণাঙ্ক i, n;
	সংরচনা বৃক্ষ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	যদি (!n)
		die(_("empty path component found in input"));
	যদি (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	যদি (!মূল->বৃক্ষ)
		load_বৃক্ষ(মূল);
	t = মূল->বৃক্ষ;
	বাবে (i = 0; i < t->entry_গুনতি; i++) {
		e = t->entries[i];
		যদি (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			যদি (!*slash1) {
				যদি (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					ঘূৰাই 0;
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
				ঘূৰাই 1;
			}
			যদি (!S_ISDIR(e->versions[1].mode)) {
				e->বৃক্ষ = new_বৃক্ষ_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			যদি (!e->বৃক্ষ)
				load_বৃক্ষ(e);
			যদি (বৃক্ষ_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&মূল->versions[1].oid, the_repository->hash_algo);
				ঘূৰাই 1;
			}
			ঘূৰাই 0;
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
	ঘূৰাই 1;
}

স্থির পূর্ণাঙ্ক বৃক্ষ_content_remove(
	সংরচনা বৃক্ষ_entry *মূল,
	ধ্রুৱ ৱর্ণ *p,
	সংরচনা বৃক্ষ_entry *backup_leaf,
	পূর্ণাঙ্ক allow_মূল)
{
	সংরচনা বৃক্ষ_content *t;
	ধ্রুৱ ৱর্ণ *slash1;
	unsigned পূর্ণাঙ্ক i, n;
	সংরচনা বৃক্ষ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	যদি (!মূল->বৃক্ষ)
		load_বৃক্ষ(মূল);

	যদি (!*p && allow_মূল) {
		e = মূল;
		গচ্ছ del_entry;
	}

	t = মূল->বৃক্ষ;
	বাবে (i = 0; i < t->entry_গুনতি; i++) {
		e = t->entries[i];
		যদি (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			যদি (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				ঘূৰাই 1;
			যদি (!*slash1 || !S_ISDIR(e->versions[1].mode))
				গচ্ছ del_entry;
			যদি (!e->বৃক্ষ)
				load_বৃক্ষ(e);
			যদি (বৃক্ষ_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				বাবে (n = 0; n < e->বৃক্ষ->entry_গুনতি; n++) {
					যদি (e->বৃক্ষ->entries[n]->versions[1].mode) {
						oidclr(&মূল->versions[1].oid,
						       the_repository->hash_algo);
						ঘূৰাই 1;
					}
				}
				backup_leaf = রিক্ত;
				গচ্ছ del_entry;
			}
			ঘূৰাই 0;
		}
	}
	ঘূৰাই 0;

del_entry:
	যদি (backup_leaf)
		memcpy(backup_leaf, e, পরিমাণ(*backup_leaf));
	অন্যথা যদি (e->বৃক্ষ)
		release_বৃক্ষ_content_recursive(e->বৃক্ষ);
	e->বৃক্ষ = রিক্ত;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&মূল->versions[1].oid, the_repository->hash_algo);
	ঘূৰাই 1;
}

স্থির পূর্ণাঙ্ক বৃক্ষ_content_get(
	সংরচনা বৃক্ষ_entry *মূল,
	ধ্রুৱ ৱর্ণ *p,
	সংরচনা বৃক্ষ_entry *leaf,
	পূর্ণাঙ্ক allow_মূল)
{
	সংরচনা বৃক্ষ_content *t;
	ধ্রুৱ ৱর্ণ *slash1;
	unsigned পূর্ণাঙ্ক i, n;
	সংরচনা বৃক্ষ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	যদি (!n && !allow_মূল)
		die(_("empty path component found in input"));

	যদি (!মূল->বৃক্ষ)
		load_বৃক্ষ(মূল);

	যদি (!n) {
		e = মূল;
		গচ্ছ found_entry;
	}

	t = মূল->বৃক্ষ;
	বাবে (i = 0; i < t->entry_গুনতি; i++) {
		e = t->entries[i];
		যদি (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			যদি (!*slash1)
				গচ্ছ found_entry;
			যদি (!S_ISDIR(e->versions[1].mode))
				ঘূৰাই 0;
			যদি (!e->বৃক্ষ)
				load_বৃক্ষ(e);
			ঘূৰাই বৃক্ষ_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	ঘূৰাই 0;

found_entry:
	memcpy(leaf, e, পরিমাণ(*leaf));
	যদি (e->বৃক্ষ && is_null_oid(&e->versions[1].oid))
		leaf->বৃক্ষ = dup_বৃক্ষ_content(e->বৃক্ষ);
	অন্যথা
		leaf->বৃক্ষ = রিক্ত;
	ঘূৰাই 1;
}

স্থির পূর্ণাঙ্ক update_branch(সংরচনা branch *b)
{
	স্থির ধ্রুৱ ৱর্ণ *বাৰ্তা = "fast-import";
	সংরচনা ref_transaction *transaction;
	সংরচনা object_id old_oid;
	সংরচনা strbuf err = STRBUF_INIT;
	স্থির ধ্রুৱ ৱর্ণ *replace_prefix = "refs/replace/";

	যদি (starts_with(b->name, replace_prefix) &&
	    !strcmp(b->name + strlen(replace_prefix),
		    oid_to_hex(&b->oid))) {
		যদি (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				রিক্ত, b->name, রিক্ত, 0);
		ঘূৰাই 0;
	}
	যদি (is_null_oid(&b->oid)) {
		যদি (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					রিক্ত, b->name, রিক্ত, 0);
		ঘূৰাই 0;
	}
	যদি (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	যদি (!force_update && !is_null_oid(&old_oid)) {
		সংরচনা commit *old_cmit, *new_cmit;
		পূর্ণাঙ্ক ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		যদি (!old_cmit || !new_cmit)
			ঘূৰাই error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		যদি (ret < 0)
			exit(128);
		যদি (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			ঘূৰাই -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	যদি (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   রিক্ত, রিক্ত, 0, বাৰ্তা, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		ঘূৰাই -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ঘূৰাই 0;
}

স্থির শূন্য dump_branches(শূন্য)
{
	unsigned পূর্ণাঙ্ক i;
	সংরচনা branch *b;

	বাবে (i = 0; i < branch_table_sz; i++) {
		বাবে (b = branch_table[i]; b; b = b->table_next_branch)
			failure |= update_branch(b);
	}
}

স্থির শূন্য dump_tags(শূন্য)
{
	স্থির ধ্রুৱ ৱর্ণ *বাৰ্তা = "fast-import";
	সংরচনা tag *t;
	সংরচনা strbuf ref_name = STRBUF_INIT;
	সংরচনা strbuf err = STRBUF_INIT;
	সংরচনা ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	যদি (!transaction) {
		failure |= error("%s", err.buf);
		গচ্ছ cleanup;
	}
	বাবে (t = first_tag; t; t = t->next_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		যদি (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, রিক্ত, রিক্ত, রিক্ত,
					   0, বাৰ্তা, &err)) {
			failure |= error("%s", err.buf);
			গচ্ছ cleanup;
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
	সংরচনা lock_file mark_lock = LOCK_INIT;
	FILE *f;

	যদি (!export_marks_file || (import_marks_file && !import_marks_file_done))
		ঘূৰাই;

	যদি (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		ঘূৰাই;
	}

	যদি (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		ঘূৰাই;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	যদি (!f) {
		পূর্ণাঙ্ক saved_errno = errno;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		ঘূৰাই;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	যদি (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		ঘূৰাই;
	}
}

স্থির শূন্য ঢোকাও_object_entry(সংরচনা mark_set **s, সংরচনা object_id *oid, uintmax_t mark)
{
	সংরচনা object_entry *e;
	e = find_object(oid);
	যদি (!e) {
		গণনা object_type type = odb_read_object_info(the_repository->objects,
							     oid, রিক্ত);
		যদি (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = ঢোকাও_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	ঢোকাও_mark(s, mark, e);
}

স্থির শূন্য ঢোকাও_oid_entry(সংরচনা mark_set **s, সংরচনা object_id *oid, uintmax_t mark)
{
	ঢোকাও_mark(s, mark, xmemdupz(oid, পরিমাণ(*oid)));
}

স্থির শূন্য read_mark_file(সংরচনা mark_set **s, FILE *f, mark_set_inserter_t inserter)
{
	ৱর্ণ সাৰি[512];
	যেতিয়ালৈ (fgets(সাৰি, পরিমাণ(সাৰি), f)) {
		uintmax_t mark;
		ৱর্ণ *end;
		সংরচনা object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, পরিমাণ(oid.hash));

		end = strchr(সাৰি, '\n');
		যদি (সাৰি[0] != ':' || !end)
			die(_("corrupt mark line: %s"), সাৰি);
		*end = 0;
		mark = strtoumax(সাৰি + 1, &end, 10);
		যদি (!mark || end == সাৰি + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), সাৰি);
		inserter(s, &oid, mark);
	}
}

স্থির শূন্য read_marks(শূন্য)
{
	FILE *f = fopen(import_marks_file, "r");
	যদি (f)
		;
	অন্যথা যদি (import_marks_file_ignore_missing && errno == ENOENT)
		গচ্ছ done; /* Marks file does not exist */
	অন্যথা
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, ঢোকাও_object_entry);
	fclose(f);
done:
	import_marks_file_done = 1;
}


স্থির পূর্ণাঙ্ক read_next_command(সংরচনা fast_import_state *state)
{
	স্থির পূর্ণাঙ্ক stdin_eof = 0;

	যদি (stdin_eof) {
		unread_command_buf = 0;
		ঘূৰাই EOF;
	}

	বাবে (;;) {
		যদি (unread_command_buf) {
			unread_command_buf = 0;
		} অন্যথা {
			সংরচনা recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			যদি (stdin_eof)
				ঘূৰাই EOF;

			যদি (!state->seen_data_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			যদি (rc)
				rc_free = rc->next;
			অন্যথা {
				rc = cmd_hist.next;
				cmd_hist.next = rc->next;
				cmd_hist.next->prev = &cmd_hist;
				মুক্ত(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_tail;
			rc->next = cmd_hist.prev;
			rc->prev->next = rc;
			cmd_tail = rc;
		}
		যদি (command_buf.buf[0] == '#')
			অনুৱর্ত;
		ঘূৰাই 0;
	}
}

স্থির শূন্য skip_optional_lf(শূন্য)
{
	পূর্ণাঙ্ক term_char = fgetc(stdin);
	যদি (term_char != '\n' && term_char != EOF)
		ungetc(term_char, stdin);
}

স্থির শূন্য parse_mark(সংরচনা fast_import_state *state)
{
	ধ্রুৱ ৱর্ণ *v;
	যদি (skip_prefix(command_buf.buf, "mark :", &v)) {
		next_mark = strtoumax(v, রিক্ত, 10);
		read_next_command(state);
	}
	অন্যথা
		next_mark = 0;
}

স্থির শূন্য parse_original_identifier(সংরচনা fast_import_state *state)
{
	ধ্রুৱ ৱর্ণ *v;
	যদি (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_next_command(state);
}

স্থির পূর্ণাঙ্ক parse_data(সংরচনা strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	ধ্রুৱ ৱর্ণ *data;
	strbuf_reset(sb);

	যদি (!skip_prefix(command_buf.buf, "data ", &data))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	যদি (skip_prefix(data, "<<", &data)) {
		ৱর্ণ *term = xstrdup(data);
		size_t term_len = command_buf.len - (data - command_buf.buf);

		বাবে (;;) {
			যদি (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), term);
			যদি (term_len == command_buf.len
				&& !strcmp(term, command_buf.buf))
				ভঙ্গ;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		মুক্ত(term);
	}
	অন্যথা {
		uintmax_t len = strtoumax(data, রিক্ত, 10);
		size_t n = 0, length = (size_t)len;

		যদি (limit && limit < len) {
			*len_res = len;
			ঘূৰাই 0;
		}
		যদি (length < len)
			die(_("data is too large to use in this context"));

		যেতিয়ালৈ (n < length) {
			size_t s = strbuf_fread(sb, length - n, stdin);
			যদি (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(unsigned long)(length - n));
			n += s;
		}
	}

	skip_optional_lf();
	ঘূৰাই 1;
}

স্থির পূর্ণাঙ্ক validate_raw_date(ধ্রুৱ ৱর্ণ *src, সংরচনা strbuf *result, পূর্ণাঙ্ক strict)
{
	ধ্রুৱ ৱর্ণ *orig_src = src;
	ৱর্ণ *endp;
	unsigned long অঙ্ক;

	errno = 0;

	অঙ্ক = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	যদি (errno || endp == src || *endp != ' ')
		ঘূৰাই -1;

	src = endp + 1;
	যদি (*src != '-' && *src != '+')
		ঘূৰাই -1;

	অঙ্ক = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	যদি (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < অঙ্ক))             /* parsed a broken timezone */
	   )
		ঘূৰাই -1;

	strbuf_addstr(result, orig_src);
	ঘূৰাই 0;
}

স্থির ৱর্ণ *parse_ident(ধ্রুৱ ৱর্ণ *buf)
{
	ধ্রুৱ ৱর্ণ *ltgt;
	size_t name_len;
	সংরচনা strbuf ident = STRBUF_INIT;

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

	ৱিকল্প (whenspec) {
	পক্ষ WHENSPEC_RAW:
		যদি (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		ভঙ্গ;
	পক্ষ WHENSPEC_RAW_PERMISSIVE:
		যদি (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		ভঙ্গ;
	পক্ষ WHENSPEC_RFC2822:
		যদি (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		ভঙ্গ;
	পক্ষ WHENSPEC_NOW:
		যদি (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		ভঙ্গ;
	}

	ঘূৰাই strbuf_detach(&ident, রিক্ত);
}

স্থির শূন্য parse_and_store_blob(
	সংরচনা last_object *last,
	সংরচনা object_id *oidout,
	uintmax_t mark)
{
	স্থির সংরচনা strbuf buf = STRBUF_INIT;
	uintmax_t len;

	যদি (parse_data(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	অন্যথা {
		যদি (last) {
			strbuf_release(&last->data);
			last->offset = 0;
			last->depth = 0;
		}
		stream_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

স্থির শূন্য parse_new_blob(সংরচনা fast_import_state *state)
{
	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&last_blob, রিক্ত, next_mark);
}

স্থির শূন্য unload_one_branch(শূন্য)
{
	যেতিয়ালৈ (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = ULONG_MAX;
		সংরচনা branch *e, *l = রিক্ত, *p = রিক্ত;

		বাবে (e = active_branches; e; e = e->active_next_branch) {
			যদি (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		যদি (p) {
			e = p->active_next_branch;
			p->active_next_branch = e->active_next_branch;
		} অন্যথা {
			e = active_branches;
			active_branches = e->active_next_branch;
		}
		e->active = 0;
		e->active_next_branch = রিক্ত;
		যদি (e->branch_বৃক্ষ.বৃক্ষ) {
			release_বৃক্ষ_content_recursive(e->branch_বৃক্ষ.বৃক্ষ);
			e->branch_বৃক্ষ.বৃক্ষ = রিক্ত;
		}
		cur_active_branches--;
	}
}

স্থির শূন্য load_branch(সংরচনা branch *b)
{
	load_বৃক্ষ(&b->branch_বৃক্ষ);
	যদি (!b->active) {
		b->active = 1;
		b->active_next_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_গুনতি++;
	}
}

স্থির unsigned ৱর্ণ convert_অঙ্ক_notes_to_fanout(uintmax_t অঙ্ক_notes)
{
	unsigned ৱর্ণ fanout = 0;
	যেতিয়ালৈ ((অঙ্ক_notes >>= 8))
		fanout++;
	ঘূৰাই fanout;
}

স্থির শূন্য construct_path_with_fanout(ধ্রুৱ ৱর্ণ *hex_sha1,
		unsigned ৱর্ণ fanout, ৱর্ণ *path)
{
	unsigned পূর্ণাঙ্ক i = 0, j = 0;
	যদি (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	যেতিয়ালৈ (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	memcpy(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

স্থির uintmax_t do_change_note_fanout(
		সংরচনা বৃক্ষ_entry *orig_মূল, সংরচনা বৃক্ষ_entry *মূল,
		ৱর্ণ *hex_oid, unsigned পূর্ণাঙ্ক hex_oid_len,
		ৱর্ণ *fullpath, unsigned পূর্ণাঙ্ক fullpath_len,
		unsigned ৱর্ণ fanout)
{
	সংরচনা বৃক্ষ_content *t;
	সংরচনা বৃক্ষ_entry *e, leaf;
	unsigned পূর্ণাঙ্ক i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t অঙ্ক_notes = 0;
	সংরচনা object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	ৱর্ণ realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ধ্রুৱ unsigned hexsz = the_hash_algo->hexsz;

	যদি (!মূল->বৃক্ষ)
		load_বৃক্ষ(মূল);
	t = মূল->বৃক্ষ;

	বাবে (i = 0; t && i < t->entry_গুনতি; i++) {
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
			অনুৱর্ত;

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
				অনুৱর্ত;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			যদি (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				অঙ্ক_notes++;
				অনুৱর্ত;
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
	ঘূৰাই অঙ্ক_notes;
}

স্থির uintmax_t change_note_fanout(সংরচনা বৃক্ষ_entry *মূল,
		unsigned ৱর্ণ fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	ৱর্ণ hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	ঘূৰাই do_change_note_fanout(মূল, মূল, hex_oid, 0, path, 0, fanout);
}

স্থির পূর্ণাঙ্ক parse_mapped_oid_hex(ধ্রুৱ ৱর্ণ *hex, সংরচনা object_id *oid, ধ্রুৱ ৱর্ণ **end)
{
	পূর্ণাঙ্ক algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, পরিমাণ(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	যদি (algo == GIT_HASH_UNKNOWN)
		ঘূৰাই -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	যদি (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		যদি (hash_algos[algo].format_id == the_hash_algo->format_id)
			ঘূৰাই 0;
		ঘূৰাই -1;
	}
	oidcpy(oid, kh_মান(sub_oid_map, it));
	ঘূৰাই 0;
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
স্থির uintmax_t parse_mark_ref(ধ্রুৱ ৱর্ণ *p, ৱর্ণ **endptr)
{
	uintmax_t mark;

	প্রতিজ্ঞা(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	যদি (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	ঘূৰাই mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
স্থির uintmax_t parse_mark_ref_eol(ধ্রুৱ ৱর্ণ *p)
{
	ৱর্ণ *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	যদি (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	ঘূৰাই mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
স্থির uintmax_t parse_mark_ref_space(ধ্রুৱ ৱর্ণ **p)
{
	uintmax_t mark;
	ৱর্ণ *end;

	mark = parse_mark_ref(*p, &end);
	যদি (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	ঘূৰাই mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
স্থির শূন্য parse_path(সংরচনা strbuf *sb, ধ্রুৱ ৱর্ণ *p, ধ্রুৱ ৱর্ণ **endp,
		পূর্ণাঙ্ক is_last_field, ধ্রুৱ ৱর্ণ *field)
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
স্থির শূন্য parse_path_eol(সংরচনা strbuf *sb, ধ্রুৱ ৱর্ণ *p, ধ্রুৱ ৱর্ণ *field)
{
	ধ্রুৱ ৱর্ণ *end;

	parse_path(sb, p, &end, 1, field);
	যদি (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
স্থির শূন্য parse_path_space(সংরচনা strbuf *sb, ধ্রুৱ ৱর্ণ *p,
		ধ্রুৱ ৱর্ণ **endp, ধ্রুৱ ৱর্ণ *field)
{
	parse_path(sb, p, endp, 0, field);
	যদি (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

স্থির শূন্য file_change_m(সংরচনা fast_import_state *state,
			  ধ্রুৱ ৱর্ণ *p,
			  সংরচনা branch *b)
{
	স্থির সংরচনা strbuf path = STRBUF_INIT;
	সংরচনা object_entry *oe;
	সংরচনা object_id oid;
	uint16_t mode, inline_data = 0;

	p = parse_mode(p, &mode);
	যদি (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	ৱিকল্প (mode) {
	পক্ষ 0644:
	পক্ষ 0755:
		mode |= S_IFREG;
	পক্ষ S_IFREG | 0644:
	পক্ষ S_IFREG | 0755:
	পক্ষ S_IFLNK:
	পক্ষ S_IFDIR:
	পক্ষ S_IFGITLINK:
		/* ok */
		ভঙ্গ;
	স্ৱভাৱ:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	যদি (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} অন্যথা যদি (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = রিক্ত; /* not used with inline_data, but makes gcc happy */
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
		বৃক্ষ_content_remove(&b->branch_বৃক্ষ, path.buf, রিক্ত, 0);
		ঘূৰাই;
	}

	যদি (S_ISGITLINK(mode)) {
		যদি (inline_data)
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
	} অন্যথা যদি (inline_data) {
		যদি (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		যেতিয়ালৈ (read_next_command(state) != EOF) {
			ধ্রুৱ ৱর্ণ *v;
			যদি (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			অন্যথা {
				parse_and_store_blob(&last_blob, &oid, 0);
				ভঙ্গ;
			}
		}
	} অন্যথা {
		গণনা object_type expected = S_ISDIR(mode) ?
						OBJ_বৃক্ষ: OBJ_BLOB;
		গণনা object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, রিক্ত);
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
		বৃক্ষ_content_replace(&b->branch_বৃক্ষ, &oid, mode, রিক্ত);
		ঘূৰাই;
	}

	যদি (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	বৃক্ষ_content_set(&b->branch_বৃক্ষ, path.buf, &oid, mode, রিক্ত);
}

স্থির শূন্য file_change_d(ধ্রুৱ ৱর্ণ *p, সংরচনা branch *b)
{
	স্থির সংরচনা strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	বৃক্ষ_content_remove(&b->branch_বৃক্ষ, path.buf, রিক্ত, 1);
}

স্থির শূন্য file_change_cr(ধ্রুৱ ৱর্ণ *p, সংরচনা branch *b, পূর্ণাঙ্ক rename)
{
	স্থির সংরচনা strbuf source = STRBUF_INIT;
	স্থির সংরচনা strbuf dest = STRBUF_INIT;
	সংরচনা বৃক্ষ_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	memset(&leaf, 0, পরিমাণ(leaf));
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
		ঘূৰাই;
	}
	যদি (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	বৃক্ষ_content_set(&b->branch_বৃক্ষ, dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.বৃক্ষ);
}

স্থির শূন্য note_change_n(সংরচনা fast_import_state *state,
			  ধ্রুৱ ৱর্ণ *p,
			  সংরচনা branch *b,
			  unsigned ৱর্ণ *old_fanout)
{
	সংরচনা object_entry *oe;
	সংরচনা branch *s;
	সংরচনা object_id oid, commit_oid;
	ৱর্ণ path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_data = 0;
	unsigned ৱর্ণ new_fanout;

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
		inline_data = 1;
		oe = রিক্ত; /* not used with inline_data, but makes gcc happy */
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
		সংরচনা object_entry *commit_oe = find_mark(marks, commit_mark);
		যদি (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} অন্যথা যদি (!repo_get_oid(the_repository, p, &commit_oid)) {
		size_t size;
		ৱর্ণ *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &size,
						   &commit_oid);
		যদি (!buf || size < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		মুক্ত(buf);
	} অন্যথা
		die(_("invalid ref name or SHA1 expression: %s"), p);

	যদি (inline_data) {
		read_next_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} অন্যথা যদি (oe) {
		যদি (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} অন্যথা যদি (!is_null_oid(&oid)) {
		গণনা object_type type = odb_read_object_info(the_repository->objects, &oid,
							রিক্ত);
		যদি (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		যদি (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	যদি (বৃক্ষ_content_remove(&b->branch_বৃক্ষ, path, রিক্ত, 0))
		b->অঙ্ক_notes--;

	যদি (is_null_oid(&oid))
		ঘূৰাই; /* nothing to insert */

	b->অঙ্ক_notes++;
	new_fanout = convert_অঙ্ক_notes_to_fanout(b->অঙ্ক_notes);
	construct_path_with_fanout(oid_to_hex(&commit_oid), new_fanout, path);
	বৃক্ষ_content_set(&b->branch_বৃক্ষ, path, &oid, S_IFREG | 0644, রিক্ত);
}

স্থির শূন্য file_change_deleteall(সংরচনা branch *b)
{
	release_বৃক্ষ_content_recursive(b->branch_বৃক্ষ.বৃক্ষ);
	oidclr(&b->branch_বৃক্ষ.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_বৃক্ষ.versions[1].oid, the_repository->hash_algo);
	load_বৃক্ষ(&b->branch_বৃক্ষ);
	b->অঙ্ক_notes = 0;
}

স্থির শূন্য parse_from_commit(সংরচনা branch *b, ৱর্ণ *buf, unsigned long size)
{
	যদি (!buf || size < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	যদি (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_বৃক্ষ.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_বৃক্ষ.versions[0].oid,
	       &b->branch_বৃক্ষ.versions[1].oid);
}

স্থির শূন্য parse_from_existing(সংরচনা branch *b)
{
	যদি (is_null_oid(&b->oid)) {
		oidclr(&b->branch_বৃক্ষ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_বৃক্ষ.versions[1].oid, the_repository->hash_algo);
	} অন্যথা {
		unsigned long size;
		size_t size_st = 0;
		ৱর্ণ *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &size_st, &b->oid);
		size = cast_size_t_to_ulong(size_st);
		parse_from_commit(b, buf, size);
		মুক্ত(buf);
	}
}

স্থির পূর্ণাঙ্ক parse_objectish(সংরচনা fast_import_state *state,
			   সংরচনা branch *b,
			   ধ্রুৱ ৱর্ণ *objectish)
{
	সংরচনা branch *s;
	সংরচনা object_id oid;

	oidcpy(&oid, &b->branch_বৃক্ষ.versions[1].oid);

	s = lookup_branch(objectish);
	যদি (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	অন্যথা যদি (s) {
		সংরচনা object_id *t = &s->branch_বৃক্ষ.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_বৃক্ষ.versions[0].oid, t);
		oidcpy(&b->branch_বৃক্ষ.versions[1].oid, t);
	} অন্যথা যদি (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		সংরচনা object_entry *oe = find_mark(marks, idnum);
		যদি (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		যদি (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			যদি (oe->pack_id != MAX_PACK_ID) {
				unsigned long size;
				ৱর্ণ *buf = gfi_unpack_entry(oe, &size);
				parse_from_commit(b, buf, size);
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
		b->branch_বৃক্ষ.বৃক্ষ = রিক্ত;
	}

	read_next_command(state);
	ঘূৰাই 1;
}

স্থির পূর্ণাঙ্ক parse_from(সংরচনা fast_import_state *state, সংরচনা branch *b)
{
	ধ্রুৱ ৱর্ণ *from;

	যদি (!skip_prefix(command_buf.buf, "from ", &from))
		ঘূৰাই 0;

	ঘূৰাই parse_objectish(state, b, from);
}

স্থির পূর্ণাঙ্ক parse_objectish_with_prefix(সংরচনা fast_import_state *state,
				       সংরচনা branch *b,
				       ধ্রুৱ ৱর্ণ *prefix)
{
	ধ্রুৱ ৱর্ণ *base;

	যদি (!skip_prefix(command_buf.buf, prefix, &base))
		ঘূৰাই 0;

	ঘূৰাই parse_objectish(state, b, base);
}

স্থির সংরচনা hash_list *parse_merge(সংরচনা fast_import_state *state,
				     unsigned পূর্ণাঙ্ক *গুনতি)
{
	সংরচনা hash_list *list = রিক্ত, **tail = &list, *n;
	ধ্রুৱ ৱর্ণ *from;
	সংরচনা branch *s;

	*গুনতি = 0;
	যেতিয়ালৈ (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(পরিমাণ(*n));
		s = lookup_branch(from);
		যদি (s)
			oidcpy(&n->oid, &s->oid);
		অন্যথা যদি (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			সংরচনা object_entry *oe = find_mark(marks, idnum);
			যদি (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} অন্যথা যদি (!repo_get_oid(the_repository, from, &n->oid)) {
			size_t size;
			ৱর্ণ *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &size, &n->oid);
			যদি (!buf || size < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			মুক্ত(buf);
		} অন্যথা
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->next = রিক্ত;
		*tail = n;
		tail = &n->next;

		(*গুনতি)++;
		read_next_command(state);
	}
	ঘূৰাই list;
}

সংরচনা signature_data {
	ৱর্ণ *hash_algo;      /* "sha1" or "sha256" */
	ৱর্ণ *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	সংরচনা strbuf data;   /* The actual signature data */
};

স্থির শূন্য parse_one_signature(সংরচনা fast_import_state *state,
				সংরচনা signature_data *sig,
				ধ্রুৱ ৱর্ণ *v)
{
	ৱর্ণ *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	ৱর্ণ *space = strchr(args, ' ');

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
	read_next_command(state);
	parse_data(&sig->data, 0, রিক্ত);
}

স্থির শূন্য discard_one_signature(সংরচনা fast_import_state *state)
{
	সংরচনা strbuf data = STRBUF_INIT;

	read_next_command(state);
	parse_data(&data, 0, রিক্ত);
	strbuf_release(&data);
}

স্থির শূন্য add_gpgsig_to_commit(সংরচনা strbuf *commit_data,
				 ধ্রুৱ ৱর্ণ *header,
				 সংরচনা signature_data *sig)
{
	সংরচনা string_list siglines = STRING_LIST_INIT_NODUP;

	যদি (!sig || !sig->hash_algo)
		ঘূৰাই;

	strbuf_addstr(commit_data, header);
	string_list_split_in_place(&siglines, sig->data.buf, "\n", -1);
	strbuf_add_separated_string_list(commit_data, "\n ", &siglines);
	strbuf_addch(commit_data, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->data);
	মুক্ত(sig->hash_algo);
}

স্থির শূন্য store_signature(সংরচনা signature_data *stored_sig,
			    সংরচনা signature_data *new_sig,
			    ধ্রুৱ ৱর্ণ *hash_type)
{
	যদি (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->data);
		মুক্ত(new_sig->hash_algo);
	} অন্যথা {
		*stored_sig = *new_sig;
	}
}

স্থির শূন্য import_one_signature(সংরচনা fast_import_state *state,
				 সংরচনা signature_data *sig_sha1,
				 সংরচনা signature_data *sig_sha256,
				 ধ্রুৱ ৱর্ণ *v)
{
	সংরচনা signature_data sig = { রিক্ত, রিক্ত, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	যদি (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	অন্যথা যদি (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	অন্যথা
		die(_("parse_one_signature() returned unknown hash algo"));
}

স্থির শূন্য finalize_commit_buffer(সংরচনা strbuf *new_data,
				   সংরচনা signature_data *sig_sha1,
				   সংরচনা signature_data *sig_sha256,
				   সংরচনা strbuf *বাৰ্তা)
{
	add_gpgsig_to_commit(new_data, "gpgsig ", sig_sha1);
	add_gpgsig_to_commit(new_data, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_data, '\n');
	strbuf_addbuf(new_data, বাৰ্তা);
}

স্থির শূন্য warn_invalid_signature(সংরচনা signature_check *check,
				   ধ্রুৱ ৱর্ণ *বাৰ্তা, গণনা sign_mode mode)
{
	ধ্রুৱ ৱর্ণ *signer = check->signer ? check->signer : _("unknown");
	ধ্রুৱ ৱর্ণ *subject;
	পূর্ণাঙ্ক subject_len = find_commit_subject(বাৰ্তা, &subject);

	ৱিকল্প (mode) {
	পক্ষ SIGN_STRIP_IF_INVALID:
		যদি (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		অন্যথা যদি (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		অন্যথা
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		ভঙ্গ;
	পক্ষ SIGN_SIGN_IF_INVALID:
		যদি (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		অন্যথা যদি (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		অন্যথা
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		ভঙ্গ;
	স্ৱভাৱ:
		BUG("unsupported signing mode");
	}
}

স্থির শূন্য handle_signature_if_invalid(সংরচনা strbuf *new_data,
					সংরচনা signature_data *sig_sha1,
					সংরচনা signature_data *sig_sha256,
					সংরচনা strbuf *বাৰ্তা,
					গণনা sign_mode mode)
{
	সংরচনা strbuf tmp_buf = STRBUF_INIT;
	সংরচনা signature_check signature_check = { 0 };
	পূর্ণাঙ্ক ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_data);
	finalize_commit_buffer(&tmp_buf, sig_sha1, sig_sha256, বাৰ্তা);
	ret = verify_commit_buffer(tmp_buf.buf, tmp_buf.len, &signature_check);

	যদি (ret) {
		যদি (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, বাৰ্তা->buf, mode);

		যদি (mode == SIGN_SIGN_IF_INVALID) {
			সংরচনা strbuf signature = STRBUF_INIT;
			সংরচনা strbuf payload = STRBUF_INIT;

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
			যদি (sign_buffer(&payload, &signature, signed_commit_keyid,
					SIGN_BUFFER_USE_DEFAULT_চাবি))
				die(_("failed to sign commit object"));
			add_header_signature(new_data, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_buffer(new_data, রিক্ত, রিক্ত, বাৰ্তা);
	} অন্যথা {
		strbuf_swap(new_data, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

স্থির শূন্য parse_new_commit(সংরচনা fast_import_state *state, ধ্রুৱ ৱর্ণ *arg)
{
	স্থির সংরচনা strbuf বাৰ্তা = STRBUF_INIT;
	সংরচনা signature_data sig_sha1 = { রিক্ত, রিক্ত, STRBUF_INIT };
	সংরচনা signature_data sig_sha256 = { রিক্ত, রিক্ত, STRBUF_INIT };
	সংরচনা branch *b;
	ৱর্ণ *author = রিক্ত;
	ৱর্ণ *committer = রিক্ত;
	ৱর্ণ *encoding = রিক্ত;
	সংরচনা hash_list *merge_list = রিক্ত;
	unsigned পূর্ণাঙ্ক merge_গুনতি;
	unsigned ৱর্ণ prev_fanout, new_fanout;
	ধ্রুৱ ৱর্ণ *v;

	b = lookup_branch(arg);
	যদি (!b)
		b = new_branch(arg);

	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	যদি (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_next_command(state);
	}
	যদি (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_next_command(state);
	}
	যদি (!committer)
		die(_("expected committer but didn't get one"));

	যেতিয়ালৈ (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		ৱিকল্প (signed_commit_mode) {

		/* First, modes that don't need the signature to be parsed */
		পক্ষ SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		পক্ষ SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		পক্ষ SIGN_STRIP:
			discard_one_signature(state);
			ভঙ্গ;

		/* Second, modes that parse the signature */
		পক্ষ SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		পক্ষ SIGN_VERBATIM:
		পক্ষ SIGN_STRIP_IF_INVALID:
		পক্ষ SIGN_SIGN_IF_INVALID:
		পক্ষ SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			ভঙ্গ;

		/* Third, BUG */
		স্ৱভাৱ:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_next_command(state);
	}

	যদি (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_next_command(state);
	}
	parse_data(&বাৰ্তা, 0, রিক্ত);
	read_next_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_গুনতি);

	/* ensure the branch is active/loaded */
	যদি (!b->branch_বৃক্ষ.বৃক্ষ || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	prev_fanout = convert_অঙ্ক_notes_to_fanout(b->অঙ্ক_notes);

	/* file_change* */
	যেতিয়ালৈ (command_buf.len > 0) {
		যদি (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		অন্যথা যদি (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		অন্যথা যদি (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		অন্যথা যদি (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		অন্যথা যদি (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		অন্যথা যদি (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		অন্যথা যদি (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		অন্যথা যদি (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		অন্যথা {
			unread_command_buf = 1;
			ভঙ্গ;
		}
		যদি (read_next_command(state) == EOF)
			ভঙ্গ;
	}

	new_fanout = convert_অঙ্ক_notes_to_fanout(b->অঙ্ক_notes);
	যদি (new_fanout != prev_fanout)
		b->অঙ্ক_notes = change_note_fanout(&b->branch_বৃক্ষ, new_fanout);

	/* build the tree and the commit */
	store_বৃক্ষ(&b->branch_বৃক্ষ);
	oidcpy(&b->branch_বৃক্ষ.versions[0].oid,
	       &b->branch_বৃক্ষ.versions[1].oid);

	strbuf_reset(&new_data);
	strbuf_addf(&new_data, "tree %s\n",
		oid_to_hex(&b->branch_বৃক্ষ.versions[1].oid));
	যদি (!is_null_oid(&b->oid))
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&b->oid));
	যেতিয়ালৈ (merge_list) {
		সংরচনা hash_list *next = merge_list->next;
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&merge_list->oid));
		মুক্ত(merge_list);
		merge_list = next;
	}
	strbuf_addf(&new_data,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	যদি (encoding)
		strbuf_addf(&new_data,
			"encoding %s\n",
			encoding);

	যদি ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
	     signed_commit_mode == SIGN_SIGN_IF_INVALID ||
	     signed_commit_mode == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_data, &sig_sha1, &sig_sha256,
					    &বাৰ্তা, signed_commit_mode);
	অন্যথা
		finalize_commit_buffer(&new_data, &sig_sha1, &sig_sha256, &বাৰ্তা);

	মুক্ত(author);
	মুক্ত(committer);
	মুক্ত(encoding);

	যদি (!store_object(OBJ_COMMIT, &new_data, রিক্ত, &b->oid, next_mark))
		b->pack_id = pack_id;
	b->last_commit = object_গুনতি_by_type[OBJ_COMMIT];
}

স্থির শূন্য handle_tag_signature_if_invalid(সংরচনা strbuf *buf,
					    সংরচনা strbuf *বাৰ্তা,
					    size_t sig_offset)
{
	সংরচনা strbuf signature = STRBUF_INIT;
	সংরচনা strbuf payload = STRBUF_INIT;
	সংরচনা signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_add(&payload, বাৰ্তা->buf, sig_offset);
	strbuf_add(&signature, বাৰ্তা->buf + sig_offset, বাৰ্তা->len - sig_offset);

	sigc.payload_type = SIGNATURE_PAYLOAD_TAG;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	যদি (!check_signature(&sigc, signature.buf, signature.len))
		গচ্ছ out;

	যদি (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(বাৰ্তা, sig_offset);

	যদি (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = রিক্ত;
		strbuf_reset(&signature);

		যদি (sign_buffer(&payload, &signature, signed_tag_keyid,
				SIGN_BUFFER_USE_DEFAULT_চাবি))
			die(_("failed to sign tag object"));

		strbuf_addbuf(বাৰ্তা, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

স্থির শূন্য handle_tag_signature(সংরচনা strbuf *buf, সংরচনা strbuf *বাৰ্তা, ধ্রুৱ ৱর্ণ *name)
{
	size_t sig_offset = parse_signed_buffer(বাৰ্তা->buf, বাৰ্তা->len);

	/* If there is no signature, there is nothing to do. */
	যদি (sig_offset >= বাৰ্তা->len)
		ঘূৰাই;

	ৱিকল্প (signed_tag_mode) {

	/* First, modes that don't change anything */
	পক্ষ SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	পক্ষ SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		ভঙ্গ;

	/* Second, modes that remove the signature */
	পক্ষ SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	পক্ষ SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(বাৰ্তা, sig_offset);
		ভঙ্গ;
	পক্ষ SIGN_ABORT_IF_INVALID:
	পক্ষ SIGN_SIGN_IF_INVALID:
	পক্ষ SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, বাৰ্তা, sig_offset);
		ভঙ্গ;

	/* Third, aborting modes */
	পক্ষ SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	স্ৱভাৱ:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

স্থির শূন্য parse_new_tag(সংরচনা fast_import_state *state, ধ্রুৱ ৱর্ণ *arg)
{
	স্থির সংরচনা strbuf বাৰ্তা = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *from;
	ৱর্ণ *tagger;
	সংরচনা branch *s;
	সংরচনা tag *t;
	uintmax_t from_mark = 0;
	সংরচনা object_id oid;
	গণনা object_type type;
	ধ্রুৱ ৱর্ণ *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, পরিমাণ(সংরচনা tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	যদি (last_tag)
		last_tag->next_tag = t;
	অন্যথা
		first_tag = t;
	last_tag = t;
	read_next_command(state);
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
		সংরচনা object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} অন্যথা যদি (!repo_get_oid(the_repository, from, &oid)) {
		সংরচনা object_entry *oe = find_object(&oid);
		যদি (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, রিক্ত);
			যদি (type < 0)
				die(_("not a valid object: %s"), from);
		} অন্যথা
			type = oe->type;
	} অন্যথা
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_next_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	যদি (skip_prefix(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_next_command(state);
	} অন্যথা
		tagger = রিক্ত;

	/* tag payload/message */
	parse_data(&বাৰ্তা, 0, রিক্ত);

	/* build the tag object */
	strbuf_reset(&new_data);

	strbuf_addf(&new_data,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_to_hex(&oid), type_name(type), t->name);
	যদি (tagger)
		strbuf_addf(&new_data,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_data, &বাৰ্তা, t->name);

	strbuf_addch(&new_data, '\n');
	strbuf_addbuf(&new_data, &বাৰ্তা);
	মুক্ত(tagger);

	যদি (store_object(OBJ_TAG, &new_data, রিক্ত, &t->oid, next_mark))
		t->pack_id = MAX_PACK_ID;
	অন্যথা
		t->pack_id = pack_id;
}

স্থির শূন্য parse_reset_branch(সংরচনা fast_import_state *state, ধ্রুৱ ৱর্ণ *arg)
{
	সংরচনা branch *b;
	ধ্রুৱ ৱর্ণ *tag_name;

	b = lookup_branch(arg);
	যদি (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_বৃক্ষ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_বৃক্ষ.versions[1].oid, the_repository->hash_algo);
		যদি (b->branch_বৃক্ষ.বৃক্ষ) {
			release_বৃক্ষ_content_recursive(b->branch_বৃক্ষ.বৃক্ষ);
			b->branch_বৃক্ষ.বৃক্ষ = রিক্ত;
		}
	}
	অন্যথা
		b = new_branch(arg);
	read_next_command(state);
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
		সংরচনা tag *t, *prev = রিক্ত;
		বাবে (t = first_tag; t; t = t->next_tag) {
			যদি (!strcmp(t->name, tag_name))
				ভঙ্গ;
			prev = t;
		}
		যদি (t) {
			যদি (prev)
				prev->next_tag = t->next_tag;
			অন্যথা
				first_tag = t->next_tag;
			যদি (!t->next_tag)
				last_tag = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	যদি (command_buf.len > 0)
		unread_command_buf = 1;
}

স্থির শূন্য cat_blob_write(ধ্রুৱ ৱর্ণ *buf, unsigned long size)
{
	যদি (write_in_full(cat_blob_fd, buf, size) < 0)
		die_errno(_("write to frontend failed"));
}

স্থির শূন্য cat_blob(সংরচনা object_entry *oe, সংরচনা object_id *oid)
{
	সংরচনা strbuf সাৰি = STRBUF_INIT;
	সংরচনা iovec iov[3];
	unsigned long size;
	গণনা object_type type = 0;
	ৱর্ণ *buf;

	যদি (!oe || oe->pack_id == MAX_PACK_ID) {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
	} অন্যথা {
		type = oe->type;
		buf = gfi_unpack_entry(oe, &size);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	যদি (type <= 0) {
		strbuf_reset(&সাৰি);
		strbuf_addf(&সাৰি, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(সাৰি.buf, সাৰি.len);
		strbuf_release(&সাৰি);
		মুক্ত(buf);
		ঘূৰাই;
	}
	যদি (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	যদি (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_to_hex(oid), type_name(type));
	strbuf_reset(&সাৰি);
	strbuf_addf(&সাৰি, "%s %s %"PRIuMAX"\n", oid_to_hex(oid),
		    type_name(type), (uintmax_t)size);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_base = সাৰি.buf;
	iov[0].iov_len = সাৰি.len;
	iov[1].iov_base = buf;
	iov[1].iov_len = size;
	iov[2].iov_base = (শূন্য *) "\n";
	iov[2].iov_len = 1;

	যদি (writev_in_full(cat_blob_fd, iov, ARRAY_SIZE(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&সাৰি);
	যদি (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.data, buf, size, size + 1);
		last_blob.depth = oe->depth;
	} অন্যথা
		মুক্ত(buf);
}

স্থির শূন্য parse_get_mark(সংরচনা fast_import_state *state UNUSED,
			   ধ্রুৱ ৱর্ণ *p)
{
	সংরচনা object_entry *oe;
	ৱর্ণ output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	যদি (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	যদি (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, পরিমাণ(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

স্থির শূন্য parse_cat_blob(সংরচনা fast_import_state *state UNUSED,
			   ধ্রুৱ ৱর্ণ *p)
{
	সংরচনা object_entry *oe;
	সংরচনা object_id oid;

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

স্থির সংরচনা object_entry *dereference(সংরচনা object_entry *oe,
					সংরচনা object_id *oid)
{
	unsigned long size;
	ৱর্ণ *buf = রিক্ত;
	ধ্রুৱ unsigned hexsz = the_hash_algo->hexsz;

	যদি (!oe) {
		গণনা object_type type = odb_read_object_info(the_repository->objects,
							     oid, রিক্ত);
		যদি (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = ঢোকাও_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	ৱিকল্প (oe->type) {
	পক্ষ OBJ_বৃক্ষ:	/* easy case. */
		ঘূৰাই oe;
	পক্ষ OBJ_COMMIT:
	পক্ষ OBJ_TAG:
		ভঙ্গ;
	স্ৱভাৱ:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	যদি (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &size);
	} অন্যথা {
		গণনা object_type unused;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &size_st);
		size = cast_size_t_to_ulong(size_st);
	}
	যদি (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	ৱিকল্প (oe->type) {
	পক্ষ OBJ_TAG:
		যদি (size < hexsz + strlen("object ") ||
		    get_oid_hex(buf + strlen("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		ভঙ্গ;
	পক্ষ OBJ_COMMIT:
		যদি (size < hexsz + strlen("tree ") ||
		    get_oid_hex(buf + strlen("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	মুক্ত(buf);
	ঘূৰাই find_object(oid);
}

স্থির শূন্য ঢোকাও_mapped_mark(uintmax_t mark, শূন্য *object, শূন্য *cbp)
{
	সংরচনা object_id *fromoid = object;
	সংরচনা object_id *tooid = find_mark(cbp, mark);
	পূর্ণাঙ্ক ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	যদি (ret == 0)
		ঘূৰাই;
	kh_মান(sub_oid_map, it) = tooid;
}

স্থির শূন্য build_mark_map_one(সংরচনা mark_set *from, সংরচনা mark_set *to)
{
	for_each_mark(from, 0, ঢোকাও_mapped_mark, to);
}

স্থির শূন্য build_mark_map(সংরচনা string_list *from, সংরচনা string_list *to)
{
	সংরচনা string_list_item *fromp, *শীৰ্ষ;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		শীৰ্ষ = string_list_lookup(to, fromp->string);
		যদি (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} অন্যথা যদি (!শীৰ্ষ || !শীৰ্ষ->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, শীৰ্ষ->util);
	}
}

স্থির সংরচনা object_entry *parse_treeish_dataref(ধ্রুৱ ৱর্ণ **p)
{
	সংরচনা object_id oid;
	সংরচনা object_entry *e;

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

	যেতিয়ালৈ (!e || e->type != OBJ_বৃক্ষ)
		e = dereference(e, &oid);
	ঘূৰাই e;
}

স্থির শূন্য print_ls(পূর্ণাঙ্ক mode, ধ্রুৱ unsigned ৱর্ণ *hash, ধ্রুৱ ৱর্ণ *path)
{
	স্থির সংরচনা strbuf সাৰি = STRBUF_INIT;

	/* See show_tree(). */
	ধ্রুৱ ৱর্ণ *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? বৃক্ষ_type :
		blob_type;

	যদি (!mode) {
		/* missing SP path LF */
		strbuf_reset(&সাৰি);
		strbuf_addstr(&সাৰি, "missing ");
		quote_c_style(path, &সাৰি, রিক্ত, 0);
		strbuf_addch(&সাৰি, '\n');
	} অন্যথা {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&সাৰি);
		strbuf_addf(&সাৰি, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &সাৰি, রিক্ত, 0);
		strbuf_addch(&সাৰি, '\n');
	}
	cat_blob_write(সাৰি.buf, সাৰি.len);
}

স্থির শূন্য parse_ls(সংরচনা fast_import_state *state UNUSED,
		     ধ্রুৱ ৱর্ণ *p,
		     সংরচনা branch *b)
{
	স্থির সংরচনা strbuf path = STRBUF_INIT;
	সংরচনা বৃক্ষ_entry *মূল = রিক্ত;
	সংরচনা বৃক্ষ_entry leaf = {রিক্ত};

	/* ls SP (<tree-ish> SP)? <path> */
	যদি (*p == '"') {
		যদি (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		মূল = &b->branch_বৃক্ষ;
	} অন্যথা {
		সংরচনা object_entry *e = parse_treeish_dataref(&p);
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

স্থির শূন্য parse_checkpoint(সংরচনা fast_import_state *state UNUSED)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

স্থির শূন্য parse_progress(সংরচনা fast_import_state *state UNUSED)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

স্থির শূন্য parse_alias(সংরচনা fast_import_state *state)
{
	সংরচনা object_entry *e;
	সংরচনা branch b;

	skip_optional_lf();
	read_next_command(state);

	/* mark ... */
	parse_mark(state);
	যদি (!next_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, পরিমাণ(b));
	যদি (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	প্রতিজ্ঞা(e);
	ঢোকাও_mark(&marks, next_mark, e);
}

স্থির ৱর্ণ* make_fast_import_path(সংরচনা fast_import_state *state,
				   ধ্রুৱ ৱর্ণ *path)
{
	যদি (!relative_marks_paths || is_absolute_path(path))
		ঘূৰাই prefix_filename(state->prefix, path);
	ঘূৰাই repo_git_path(the_repository, "info/fast-import/%s", path);
}

স্থির শূন্য option_import_marks(সংরচনা fast_import_state *state,
				ধ্রুৱ ৱর্ণ *marks,
				পূর্ণাঙ্ক from_stream,
				পূর্ণাঙ্ক ignore_missing)
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

স্থির শূন্য option_date_format(ধ্রুৱ ৱর্ণ *fmt)
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

স্থির unsigned long ulong_arg(ধ্রুৱ ৱর্ণ *option, ধ্রুৱ ৱর্ণ *arg)
{
	ৱর্ণ *endptr;
	unsigned long rv = strtoul(arg, &endptr, 0);
	যদি (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	ঘূৰাই rv;
}

স্থির শূন্য option_depth(ধ্রুৱ ৱর্ণ *depth)
{
	max_depth = ulong_arg("--depth", depth);
	যদি (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

স্থির শূন্য option_active_branches(ধ্রুৱ ৱর্ণ *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

স্থির শূন্য option_export_marks(সংরচনা fast_import_state *state,
				ধ্রুৱ ৱর্ণ *marks)
{
	মুক্ত(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

স্থির শূন্য option_cat_blob_fd(সংরচনা fast_import_state *state UNUSED,
			       ধ্রুৱ ৱর্ণ *fd)
{
	unsigned long n = ulong_arg("--cat-blob-fd", fd);
	যদি (n > (unsigned long) INT_MAX)
		die(_("--cat-blob-fd cannot exceed %d"), INT_MAX);
	cat_blob_fd = (পূর্ণাঙ্ক) n;
}

স্থির শূন্য option_export_pack_edges(সংরচনা fast_import_state *state,
				     ধ্রুৱ ৱর্ণ *edges)
{
	ৱর্ণ *fn = prefix_filename(state->prefix, edges);
	যদি (pack_edges)
		fclose(pack_edges);
	pack_edges = xfopen(fn, "a");
	মুক্ত(fn);
}

স্থির শূন্য option_rewrite_submodules(সংরচনা fast_import_state *state,
				      ধ্রুৱ ৱর্ণ *arg,
				      সংরচনা string_list *list)
{
	সংরচনা mark_set *ms;
	FILE *fp;
	ৱর্ণ *s = xstrdup(arg);
	ৱর্ণ *f = strchr(s, ':');
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

স্থির শূন্য option_max_pack_size(ধ্রুৱ ৱর্ণ *arg)
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

স্থির শূন্য option_big_file_threshold(ধ্রুৱ ৱর্ণ *arg)
{
	unsigned long v;

	যদি (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

স্থির শূন্য option_signed_commits(ধ্রুৱ ৱর্ণ *arg)
{
	যদি (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

স্থির শূন্য option_signed_tags(ধ্রুৱ ৱর্ণ *arg)
{
	যদি (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

স্থির শূন্য option_quiet(শূন্য)
{
	show_stats = 0;
	quiet = 1;
}

স্থির পূর্ণাঙ্ক parse_one_option(সংরচনা fast_import_state *state, ধ্রুৱ ৱর্ণ *option)
{
	যদি (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_size(option);
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
		ঘূৰাই 0;
	}

	ঘূৰাই 1;
}

স্থির শূন্য check_unsafe_feature(সংরচনা fast_import_state *state,
				 ধ্রুৱ ৱর্ণ *feature)
{
	যদি (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

স্থির পূর্ণাঙ্ক parse_one_feature(সংরচনা fast_import_state *state,
			     ধ্রুৱ ৱর্ণ *feature)
{
	ধ্রুৱ ৱর্ণ *arg;

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
		ঘূৰাই 0;
	}

	ঘূৰাই 1;
}

স্থির শূন্য parse_feature(সংরচনা fast_import_state *state, ধ্রুৱ ৱর্ণ *feature)
{
	যদি (state->seen_data_command)
		die(_("got feature command '%s' after data command"), feature);

	যদি (parse_one_feature(state, feature))
		ঘূৰাই;

	die(_("this version of fast-import does not support feature %s."), feature);
}

স্থির শূন্য parse_option(সংরচনা fast_import_state *state, ধ্রুৱ ৱর্ণ *option)
{
	যদি (state->seen_data_command)
		die(_("got option command '%s' after data command"), option);

	যদি (parse_one_option(state, option))
		ঘূৰাই;

	die(_("this version of fast-import does not support option: %s"), option);
}

স্থির শূন্য git_pack_config(শূন্য)
{
	পূর্ণাঙ্ক indexversion_মান;
	পূর্ণাঙ্ক limit;
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

	repo_config(the_repository, git_default_config, রিক্ত);
}

স্থির ধ্রুৱ ৱর্ণ *ধ্রুৱ fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	রিক্ত
};

স্থির শূন্য parse_argv(সংরচনা fast_import_state *state)
{
	পূর্ণাঙ্ক argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	যদি (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_data_command = 1;
	যদি (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

স্থির পূর্ণাঙ্ক option_parse_date_format(ধ্রুৱ সংরচনা option *opt UNUSED,
				    ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_export_pack_edges(ধ্রুৱ সংরচনা option *opt,
					  ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->মান, arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_max_pack_size(ধ্রুৱ সংরচনা option *opt UNUSED,
				      ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_size(arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_big_file_threshold(ধ্রুৱ সংরচনা option *opt UNUSED,
					   ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_signed_commits(ধ্রুৱ সংরচনা option *opt UNUSED,
				       ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_signed_tags(ধ্রুৱ সংরচনা option *opt UNUSED,
				    ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_rewrite_submodules_from(ধ্রুৱ সংরচনা option *opt,
						ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->মান, arg, &sub_marks_from);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_rewrite_submodules_to(ধ্রুৱ সংরচনা option *opt,
					      ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->মান, arg, &sub_marks_to);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_cat_blob_fd(ধ্রুৱ সংরচনা option *opt,
				    ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->মান, arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_import_marks(ধ্রুৱ সংরচনা option *opt,
				     ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->মান, arg, 0, 0);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_import_marks_if_exists(ধ্রুৱ সংরচনা option *opt,
					       ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->মান, arg, 0, 1);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_export_marks(ধ্রুৱ সংরচনা option *opt,
				     ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->মান, arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_depth(ধ্রুৱ সংরচনা option *opt UNUSED,
			      ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_active_branches(ধ্রুৱ সংরচনা option *opt UNUSED,
					ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_quiet(ধ্রুৱ সংরচনা option *opt UNUSED,
			      ধ্রুৱ ৱর্ণ *arg UNUSED, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	ঘূৰাই 0;
}

পূর্ণাঙ্ক cmd_fast_import(পূর্ণাঙ্ক argc,
		    ধ্রুৱ ৱর্ণ **argv,
		    ধ্রুৱ ৱর্ণ *prefix,
		    সংরচনা repository *repo)
{
	সংরচনা fast_import_state state;

	সংরচনা option fast_import_options[] = {
		OPT_GROUP(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", রিক্ত, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", রিক্ত, রিক্ত,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", রিক্ত, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_size),
		OPT_CALLBACK_F(0, "big-file-threshold", রিক্ত, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_threshold),
		OPT_CALLBACK_F(0, "depth", রিক্ত, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_depth),
		OPT_CALLBACK_F(0, "active-branches", রিক্ত, N_("n"),
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
		OPT_CALLBACK_F(0, "signed-commits", রিক্ত, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", রিক্ত, N_("mode"),
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
	marks = mem_pool_calloc(&fi_mem_pool, 1, পরিমাণ(সংরচনা mark_set));

	hashmap_init(&object_table, object_entry_hashcmp, রিক্ত, 0);

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
	বাবে (পূর্ণাঙ্ক i = 1; i < argc; i++) {
		ধ্রুৱ ৱর্ণ *arg = argv[i];
		যদি (*arg != '-' || !strcmp(arg, "--"))
			ভঙ্গ;
		যদি (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * পরিমাণ(*rc_free));
	বাবে (unsigned পূর্ণাঙ্ক i = 0; i < (cmd_save - 1); i++)
		rc_free[i].next = &rc_free[i + 1];
	rc_free[cmd_save - 1].next = রিক্ত;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	যেতিয়ালৈ (read_next_command(&state) != EOF) {
		ধ্রুৱ ৱর্ণ *v;
		যদি (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		অন্যথা যদি (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		অন্যথা যদি (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		অন্যথা যদি (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		অন্যথা যদি (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, রিক্ত);
		অন্যথা যদি (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		অন্যথা যদি (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		অন্যথা যদি (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		অন্যথা যদি (!strcmp("done", command_buf.buf))
			ভঙ্গ;
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
	যদি (!state.seen_data_command)
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
		uintmax_t total_গুনতি = 0, duplicate_গুনতি = 0;
		বাবে (size_t i = 0; i < ARRAY_SIZE(object_গুনতি_by_type); i++)
			total_গুনতি += object_গুনতি_by_type[i];
		বাবে (size_t i = 0; i < ARRAY_SIZE(duplicate_গুনতি_by_type); i++)
			duplicate_গুনতি += duplicate_গুনতি_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_গুনতি);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", total_গুনতি, duplicate_গুনতি);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_গুনতি_by_type[OBJ_BLOB], duplicate_গুনতি_by_type[OBJ_BLOB], delta_গুনতি_by_type[OBJ_BLOB], delta_গুনতি_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_গুনতি_by_type[OBJ_বৃক্ষ], duplicate_গুনতি_by_type[OBJ_বৃক্ষ], delta_গুনতি_by_type[OBJ_বৃক্ষ], delta_গুনতি_attempts_by_type[OBJ_বৃক্ষ]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_গুনতি_by_type[OBJ_COMMIT], duplicate_গুনতি_by_type[OBJ_COMMIT], delta_গুনতি_by_type[OBJ_COMMIT], delta_গুনতি_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_গুনতি_by_type[OBJ_TAG], duplicate_গুনতি_by_type[OBJ_TAG], delta_গুনতি_by_type[OBJ_TAG], delta_গুনতি_attempts_by_type[OBJ_TAG]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_গুনতি, branch_load_গুনতি);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->shift) * 1024, marks_set_গুনতি);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (বৃক্ষ_entry_allocd + fi_mem_pool.pool_alloc + alloc_গুনতি*পরিমাণ(সংরচনা object_entry))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (unsigned long)((বৃক্ষ_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_গুনতি*পরিমাণ(সংরচনা object_entry))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	ঘূৰাই failure ? 1 : 0;
}
