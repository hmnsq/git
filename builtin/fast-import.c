#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "blob.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "delta.h"
#அந்தர்பாவ "pack.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "csum-file.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "packfile.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "mem-pool.h"
#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "khash.h"
#அந்தர்பாவ "date.h"
#அந்தர்பாவ "gpg-interface.h"
#அந்தர்பாவ "parse-options.h"

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

கட்டமைப்பு object_entry {
	கட்டமைப்பு pack_idx_entry idx;
	கட்டமைப்பு hashmap_entry ent;
	uint32_t type : TYPE_BITS,
		pack_id : PACK_ID_BITS,
		depth : DEPTH_BITS;
};

நிலையான முழு object_entry_hashcmp(மாறிலி வெறுமை *map_data UNUSED,
				மாறிலி கட்டமைப்பு hashmap_entry *eptr,
				மாறிலி கட்டமைப்பு hashmap_entry *entry_or_சாவி,
				மாறிலி வெறுமை *keydata)
{
	மாறிலி கட்டமைப்பு object_id *oid = keydata;
	மாறிலி கட்டமைப்பு object_entry *e1, *e2;

	e1 = container_of(eptr, மாறிலி கட்டமைப்பு object_entry, ent);
	எனில் (oid)
		திரும்பு oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_சாவி, மாறிலி கட்டமைப்பு object_entry, ent);
	திரும்பு oidcmp(&e1->idx.oid, &e2->idx.oid);
}

கட்டமைப்பு object_entry_pool {
	கட்டமைப்பு object_entry_pool *next_pool;
	கட்டமைப்பு object_entry *next_free;
	கட்டமைப்பு object_entry *end;
	கட்டமைப்பு object_entry entries[FLEX_ARRAY]; /* more */
};

கட்டமைப்பு mark_set {
	ஒன்றிப்பு {
		கட்டமைப்பு object_id *oids[1024];
		கட்டமைப்பு object_entry *marked[1024];
		கட்டமைப்பு mark_set *sets[1024];
	} data;
	unsigned முழு shift;
};

கட்டமைப்பு last_object {
	கட்டமைப்பு strbuf data;
	off_t offset;
	unsigned முழு depth;
	unsigned no_swap : 1;
};

கட்டமைப்பு atom_str {
	கட்டமைப்பு atom_str *next_atom;
	unsigned short str_len;
	எழுத்து str_dat[FLEX_ARRAY]; /* more */
};

கட்டமைப்பு மரம்_content;
கட்டமைப்பு மரம்_entry {
	கட்டமைப்பு மரம்_content *மரம்;
	கட்டமைப்பு atom_str *name;
	கட்டமைப்பு மரம்_entry_ms {
		uint16_t mode;
		கட்டமைப்பு object_id oid;
	} versions[2];
};

கட்டமைப்பு மரம்_content {
	unsigned முழு entry_capacity; /* must match avail_tree_content */
	unsigned முழு entry_மொத்தம்;
	unsigned முழு delta_depth;
	கட்டமைப்பு மரம்_entry *entries[FLEX_ARRAY]; /* more */
};

கட்டமைப்பு avail_மரம்_content {
	unsigned முழு entry_capacity; /* must match tree_content */
	கட்டமைப்பு avail_மரம்_content *next_avail;
};

கட்டமைப்பு branch {
	கட்டமைப்பு branch *table_next_branch;
	கட்டமைப்பு branch *active_next_branch;
	மாறிலி எழுத்து *name;
	கட்டமைப்பு மரம்_entry branch_மரம்;
	uintmax_t last_commit;
	uintmax_t இலக்கம்_notes;
	unsigned active : 1;
	unsigned delete : 1;
	unsigned pack_id : PACK_ID_BITS;
	கட்டமைப்பு object_id oid;
};

கட்டமைப்பு tag {
	கட்டமைப்பு tag *next_tag;
	மாறிலி எழுத்து *name;
	unsigned முழு pack_id;
	கட்டமைப்பு object_id oid;
};

கட்டமைப்பு hash_list {
	கட்டமைப்பு hash_list *next;
	கட்டமைப்பு object_id oid;
};

வகைவரை எண்ணிக்கை {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

கட்டமைப்பு recent_command {
	கட்டமைப்பு recent_command *prev;
	கட்டமைப்பு recent_command *next;
	எழுத்து *buf;
};

வகைவரை வெறுமை (*mark_set_inserter_t)(கட்டமைப்பு mark_set **s, கட்டமைப்பு object_id *oid, uintmax_t mark);
வகைவரை வெறுமை (*each_mark_fn_t)(uintmax_t mark, வெறுமை *obj, வெறுமை *cbp);

/* Configured limits on output */
நிலையான unsigned long max_depth = 50;
நிலையான off_t max_packsize;
நிலையான முழு unpack_limit = 100;
நிலையான முழு force_update;

/* Stats and misc. counters */
நிலையான uintmax_t alloc_மொத்தம்;
நிலையான uintmax_t marks_set_மொத்தம்;
நிலையான uintmax_t object_மொத்தம்_by_type[1 << TYPE_BITS];
நிலையான uintmax_t duplicate_மொத்தம்_by_type[1 << TYPE_BITS];
நிலையான uintmax_t delta_மொத்தம்_by_type[1 << TYPE_BITS];
நிலையான uintmax_t delta_மொத்தம்_attempts_by_type[1 << TYPE_BITS];
நிலையான unsigned long object_மொத்தம்;
நிலையான unsigned long branch_மொத்தம்;
நிலையான unsigned long branch_load_மொத்தம்;
நிலையான முழு failure;
நிலையான FILE *pack_edges;
நிலையான முழு show_stats = 1;
நிலையான முழு quiet;
நிலையான எண்ணிக்கை sign_mode signed_tag_mode = SIGN_VERBATIM;
நிலையான எண்ணிக்கை sign_mode signed_commit_mode = SIGN_VERBATIM;
நிலையான மாறிலி எழுத்து *signed_commit_keyid;
நிலையான மாறிலி எழுத்து *signed_tag_keyid;

/* Memory pools */
நிலையான கட்டமைப்பு mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - அளவு(கட்டமைப்பு mp_block),
};

/* Atom management */
நிலையான unsigned முழு atom_table_sz = 4451;
நிலையான unsigned முழு atom_cnt;
நிலையான கட்டமைப்பு atom_str **atom_table;

/* The .pack file being generated */
நிலையான கட்டமைப்பு pack_idx_option pack_idx_opts;
நிலையான unsigned முழு pack_id;
நிலையான கட்டமைப்பு hashfile *pack_file;
நிலையான கட்டமைப்பு packed_git *pack_data;
நிலையான கட்டமைப்பு packed_git **all_packs;
நிலையான off_t pack_size;

/* Table of objects we've written. */
நிலையான unsigned முழு object_entry_alloc = 5000;
நிலையான கட்டமைப்பு object_entry_pool *blocks;
நிலையான கட்டமைப்பு hashmap object_table;
நிலையான கட்டமைப்பு mark_set *marks;
நிலையான எழுத்து *export_marks_file;
நிலையான எழுத்து *import_marks_file;
நிலையான முழு import_marks_file_from_stream;
நிலையான முழு import_marks_file_ignore_missing;
நிலையான முழு import_marks_file_done;
நிலையான முழு relative_marks_paths;

/* Our last blob */
நிலையான கட்டமைப்பு last_object last_blob = {
	.data = STRBUF_INIT,
 };

/* Tree management */
நிலையான unsigned முழு மரம்_entry_alloc = 1000;
நிலையான வெறுமை *avail_மரம்_entry;
நிலையான unsigned முழு avail_மரம்_table_sz = 100;
நிலையான கட்டமைப்பு avail_மரம்_content **avail_மரம்_table;
நிலையான size_t மரம்_entry_allocd;
நிலையான கட்டமைப்பு strbuf old_மரம் = STRBUF_INIT;
நிலையான கட்டமைப்பு strbuf new_மரம் = STRBUF_INIT;

/* Branch data */
நிலையான unsigned long max_active_branches = 5;
நிலையான unsigned long cur_active_branches;
நிலையான unsigned long branch_table_sz = 1039;
நிலையான கட்டமைப்பு branch **branch_table;
நிலையான கட்டமைப்பு branch *active_branches;

/* Tag data */
நிலையான கட்டமைப்பு tag *first_tag;
நிலையான கட்டமைப்பு tag *last_tag;

/* Input stream parsing */
நிலையான whenspec_type whenspec = WHENSPEC_RAW;
நிலையான கட்டமைப்பு strbuf command_buf = STRBUF_INIT;
நிலையான முழு unread_command_buf;
நிலையான கட்டமைப்பு recent_command cmd_hist = {
	.prev = &cmd_hist,
	.next = &cmd_hist,
};
நிலையான கட்டமைப்பு recent_command *cmd_tail = &cmd_hist;
நிலையான கட்டமைப்பு recent_command *rc_free;
நிலையான unsigned முழு cmd_save = 100;
நிலையான uintmax_t next_mark;
நிலையான கட்டமைப்பு strbuf new_data = STRBUF_INIT;
நிலையான முழு require_explicit_termination;

/* Signal handling */
நிலையான நிலையற்ற sig_atomic_t checkpoint_requested;

/* Submodule marks */
நிலையான கட்டமைப்பு string_list sub_marks_from = STRING_LIST_INIT_DUP;
நிலையான கட்டமைப்பு string_list sub_marks_to = STRING_LIST_INIT_DUP;
நிலையான kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
நிலையான முழு cat_blob_fd = STDOUT_FILENO;

/* Command state */
கட்டமைப்பு fast_import_state {
	முழு argc;
	மாறிலி எழுத்து **argv;
	மாறிலி எழுத்து *prefix;
	முழு seen_data_command;
	முழு allow_unsafe_features;
	கட்டமைப்பு option *option;
};

நிலையான வெறுமை fast_import_state_init(கட்டமைப்பு fast_import_state *state,
				   முழு argc, மாறிலி எழுத்து **argv,
				   மாறிலி எழுத்து *prefix, கட்டமைப்பு option *option)
{
	memset(state, 0, அளவு(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

நிலையான வெறுமை parse_argv(கட்டமைப்பு fast_import_state *state);
நிலையான வெறுமை parse_get_mark(கட்டமைப்பு fast_import_state *state, மாறிலி எழுத்து *p);
நிலையான வெறுமை parse_cat_blob(கட்டமைப்பு fast_import_state *state, மாறிலி எழுத்து *p);
நிலையான வெறுமை parse_ls(கட்டமைப்பு fast_import_state *state,
		     மாறிலி எழுத்து *p,
		     கட்டமைப்பு branch *b);

நிலையான வெறுமை for_each_mark(கட்டமைப்பு mark_set *m, uintmax_t base, each_mark_fn_t callback, வெறுமை *p)
{
	uintmax_t k;
	எனில் (m->shift) {
		க்கு (k = 0; k < 1024; k++) {
			எனில் (m->data.sets[k])
				for_each_mark(m->data.sets[k], base + (k << m->shift), callback, p);
		}
	} இல்லையெனில் {
		க்கு (k = 0; k < 1024; k++) {
			எனில் (m->data.marked[k])
				callback(base + k, m->data.marked[k], p);
		}
	}
}

நிலையான வெறுமை dump_marks_fn(uintmax_t mark, வெறுமை *object, வெறுமை *cbp) {
	கட்டமைப்பு object_entry *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

நிலையான வெறுமை write_branch_report(FILE *rpt, கட்டமைப்பு branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	எனில் (b->active)
		fputs(" active", rpt);
	எனில் (b->branch_மரம்.மரம்)
		fputs(" loaded", rpt);
	எனில் (is_null_oid(&b->branch_மரம்.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_to_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_to_hex(&b->branch_மரம்.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_to_hex(&b->branch_மரம்.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->last_commit);

	fputs("  last pack   : ", rpt);
	எனில் (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

நிலையான வெறுமை write_crash_report(மாறிலி எழுத்து *err)
{
	எழுத்து *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	கட்டமைப்பு branch *b;
	unsigned long lu;
	கட்டமைப்பு recent_command *rc;

	எனில் (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		விடுவி(loc);
		திரும்பு;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(time(வெற்று), 0, DATE_MODE(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	க்கு (rc = cmd_hist.next; rc != &cmd_hist; rc = rc->next) {
		எனில் (rc->next == &cmd_hist)
			fputs("* ", rpt);
		இல்லையெனில்
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
	க்கு (b = active_branches, lu = 0; b; b = b->active_next_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	க்கு (lu = 0; lu < branch_table_sz; lu++) {
		க்கு (b = branch_table[lu]; b; b = b->table_next_branch)
			write_branch_report(rpt, b);
	}

	எனில் (first_tag) {
		கட்டமைப்பு tag *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		க்கு (tg = first_tag; tg; tg = tg->next_tag) {
			fputs(oid_to_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	எனில் (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	இல்லையெனில்
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	விடுவி(loc);
}

நிலையான வெறுமை end_packfile(வெறுமை);
நிலையான வெறுமை unkeep_all_packs(வெறுமை);
நிலையான வெறுமை dump_marks(வெறுமை);

நிலையான NORETURN வெறுமை die_nicely(மாறிலி எழுத்து *err, va_list params)
{
	va_list cp;
	நிலையான முழு zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	எனில் (!zombie) {
		எழுத்து message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, அளவு(message), err, cp);
		write_crash_report(message);
		end_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	exit(128);
}

#ifndef SIGUSR1	/* Windows, for example */

நிலையான வெறுமை set_checkpoint_signal(வெறுமை)
{
}

#இல்லையெனில்

நிலையான வெறுமை checkpoint_signal(முழு signo UNUSED)
{
	checkpoint_requested = 1;
}

நிலையான வெறுமை set_checkpoint_signal(வெறுமை)
{
	கட்டமைப்பு sigaction sa;

	memset(&sa, 0, அளவு(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, வெற்று);
}

#endif

நிலையான வெறுமை alloc_objects(unsigned முழு cnt)
{
	கட்டமைப்பு object_entry_pool *b;

	b = xmalloc(அளவு(கட்டமைப்பு object_entry_pool)
		+ cnt * அளவு(கட்டமைப்பு object_entry));
	b->next_pool = blocks;
	b->next_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_மொத்தம் += cnt;
}

நிலையான கட்டமைப்பு object_entry *new_object(கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு object_entry *e;

	எனில் (blocks->next_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->next_free++;
	oidcpy(&e->idx.oid, oid);
	திரும்பு e;
}

நிலையான கட்டமைப்பு object_entry *find_object(கட்டமைப்பு object_id *oid)
{
	திரும்பு hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   கட்டமைப்பு object_entry, ent);
}

நிலையான கட்டமைப்பு object_entry *செருகு_object(கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு object_entry *e;
	unsigned முழு hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					கட்டமைப்பு object_entry, ent);
	எனில் (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	திரும்பு e;
}

நிலையான வெறுமை invalidate_pack_id(unsigned முழு id)
{
	unsigned long lu;
	கட்டமைப்பு tag *t;
	கட்டமைப்பு hashmap_iter iter;
	கட்டமைப்பு object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		எனில் (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	க்கு (lu = 0; lu < branch_table_sz; lu++) {
		கட்டமைப்பு branch *b;

		க்கு (b = branch_table[lu]; b; b = b->table_next_branch)
			எனில் (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	க்கு (t = first_tag; t; t = t->next_tag)
		எனில் (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

நிலையான unsigned முழு hc_str(மாறிலி எழுத்து *s, size_t len)
{
	unsigned முழு r = 0;
	போது (len-- > 0)
		r = r * 31 + *s++;
	திரும்பு r;
}

நிலையான வெறுமை செருகு_mark(கட்டமைப்பு mark_set **உச்சி, uintmax_t idnum, கட்டமைப்பு object_entry *oe)
{
	கட்டமைப்பு mark_set *s = *உச்சி;

	போது ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, அளவு(கட்டமைப்பு mark_set));
		s->shift = (*உச்சி)->shift + 10;
		s->data.sets[0] = *உச்சி;
		*உச்சி = s;
	}
	போது (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		எனில் (!s->data.sets[i]) {
			s->data.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, அளவு(கட்டமைப்பு mark_set));
			s->data.sets[i]->shift = s->shift - 10;
		}
		s = s->data.sets[i];
	}
	எனில் (!s->data.marked[idnum])
		marks_set_மொத்தம்++;
	s->data.marked[idnum] = oe;
}

நிலையான வெறுமை *find_mark(கட்டமைப்பு mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	கட்டமைப்பு object_entry *oe = வெற்று;
	எனில் ((idnum >> s->shift) < 1024) {
		போது (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->data.sets[i];
		}
		எனில் (s)
			oe = s->data.marked[idnum];
	}
	எனில் (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	திரும்பு oe;
}

நிலையான கட்டமைப்பு atom_str *to_atom(மாறிலி எழுத்து *s, unsigned short len)
{
	unsigned முழு hc = hc_str(s, len) % atom_table_sz;
	கட்டமைப்பு atom_str *c;

	க்கு (c = atom_table[hc]; c; c = c->next_atom)
		எனில் (c->str_len == len && !strncmp(s, c->str_dat, len))
			திரும்பு c;

	c = mem_pool_alloc(&fi_mem_pool, அளவு(கட்டமைப்பு atom_str) + len + 1);
	c->str_len = len;
	memcpy(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->next_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	திரும்பு c;
}

நிலையான கட்டமைப்பு branch *lookup_branch(மாறிலி எழுத்து *name)
{
	unsigned முழு hc = hc_str(name, strlen(name)) % branch_table_sz;
	கட்டமைப்பு branch *b;

	க்கு (b = branch_table[hc]; b; b = b->table_next_branch)
		எனில் (!strcmp(name, b->name))
			திரும்பு b;
	திரும்பு வெற்று;
}

நிலையான கட்டமைப்பு branch *new_branch(மாறிலி எழுத்து *name)
{
	unsigned முழு hc = hc_str(name, strlen(name)) % branch_table_sz;
	கட்டமைப்பு branch *b = lookup_branch(name);

	எனில் (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	எனில் (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, அளவு(கட்டமைப்பு branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_next_branch = branch_table[hc];
	b->branch_மரம்.versions[0].mode = S_IFDIR;
	b->branch_மரம்.versions[1].mode = S_IFDIR;
	b->இலக்கம்_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_மொத்தம்++;
	திரும்பு b;
}

நிலையான unsigned முழு hc_entries(unsigned முழு cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	திரும்பு cnt < avail_மரம்_table_sz ? cnt : avail_மரம்_table_sz - 1;
}

நிலையான கட்டமைப்பு மரம்_content *new_மரம்_content(unsigned முழு cnt)
{
	கட்டமைப்பு avail_மரம்_content *f, *l = வெற்று;
	கட்டமைப்பு மரம்_content *t;
	unsigned முழு hc = hc_entries(cnt);

	க்கு (f = avail_மரம்_table[hc]; f; l = f, f = f->next_avail)
		எனில் (f->entry_capacity >= cnt)
			நிறுத்து;

	எனில் (f) {
		எனில் (l)
			l->next_avail = f->next_avail;
		இல்லையெனில்
			avail_மரம்_table[hc] = f->next_avail;
	} இல்லையெனில் {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, அளவு(*t) + அளவு(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (கட்டமைப்பு மரம்_content*)f;
	t->entry_மொத்தம் = 0;
	t->delta_depth = 0;
	திரும்பு t;
}

நிலையான வெறுமை release_மரம்_entry(கட்டமைப்பு மரம்_entry *e);
நிலையான வெறுமை release_மரம்_content(கட்டமைப்பு மரம்_content *t)
{
	கட்டமைப்பு avail_மரம்_content *f = (கட்டமைப்பு avail_மரம்_content*)t;
	unsigned முழு hc = hc_entries(f->entry_capacity);
	f->next_avail = avail_மரம்_table[hc];
	avail_மரம்_table[hc] = f;
}

நிலையான வெறுமை release_மரம்_content_recursive(கட்டமைப்பு மரம்_content *t)
{
	unsigned முழு i;
	க்கு (i = 0; i < t->entry_மொத்தம்; i++)
		release_மரம்_entry(t->entries[i]);
	release_மரம்_content(t);
}

நிலையான கட்டமைப்பு மரம்_content *grow_மரம்_content(
	கட்டமைப்பு மரம்_content *t,
	முழு amt)
{
	கட்டமைப்பு மரம்_content *r = new_மரம்_content(t->entry_மொத்தம் + amt);
	r->entry_மொத்தம் = t->entry_மொத்தம்;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_மொத்தம்);
	release_மரம்_content(t);
	திரும்பு r;
}

நிலையான கட்டமைப்பு மரம்_entry *new_மரம்_entry(வெறுமை)
{
	கட்டமைப்பு மரம்_entry *e;

	எனில் (!avail_மரம்_entry) {
		unsigned முழு n = மரம்_entry_alloc;
		மரம்_entry_allocd += n * அளவு(கட்டமைப்பு மரம்_entry);
		ALLOC_ARRAY(e, n);
		avail_மரம்_entry = e;
		போது (n-- > 1) {
			*((வெறுமை**)e) = e + 1;
			e++;
		}
		*((வெறுமை**)e) = வெற்று;
	}

	e = avail_மரம்_entry;
	avail_மரம்_entry = *((வெறுமை**)e);
	திரும்பு e;
}

நிலையான வெறுமை release_மரம்_entry(கட்டமைப்பு மரம்_entry *e)
{
	எனில் (e->மரம்)
		release_மரம்_content_recursive(e->மரம்);
	*((வெறுமை**)e) = avail_மரம்_entry;
	avail_மரம்_entry = e;
}

நிலையான கட்டமைப்பு மரம்_content *dup_மரம்_content(கட்டமைப்பு மரம்_content *s)
{
	கட்டமைப்பு மரம்_content *d;
	கட்டமைப்பு மரம்_entry *a, *b;
	unsigned முழு i;

	எனில் (!s)
		திரும்பு வெற்று;
	d = new_மரம்_content(s->entry_மொத்தம்);
	க்கு (i = 0; i < s->entry_மொத்தம்; i++) {
		a = s->entries[i];
		b = new_மரம்_entry();
		memcpy(b, a, அளவு(*a));
		எனில் (a->மரம் && is_null_oid(&b->versions[1].oid))
			b->மரம் = dup_மரம்_content(a->மரம்);
		இல்லையெனில்
			b->மரம் = வெற்று;
		d->entries[i] = b;
	}
	d->entry_மொத்தம் = s->entry_மொத்தம்;
	d->delta_depth = s->delta_depth;

	திரும்பு d;
}

நிலையான வெறுமை start_packfile(வெறுமை)
{
	கட்டமைப்பு strbuf tmp_file = STRBUF_INIT;
	கட்டமைப்பு packed_git *p;
	முழு pack_fd;

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
	object_மொத்தம் = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

நிலையான மாறிலி எழுத்து *create_index(வெறுமை)
{
	மாறிலி எழுத்து *tmpfile;
	கட்டமைப்பு pack_idx_entry **idx, **c, **last;
	கட்டமைப்பு object_entry *e;
	கட்டமைப்பு object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_மொத்தம்);
	c = idx;
	க்கு (o = blocks; o; o = o->next_pool)
		க்கு (e = o->next_free; e-- != o->entries;)
			எனில் (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_மொத்தம்;
	எனில் (c != last)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, வெற்று, idx, object_மொத்தம்,
				 &pack_idx_opts, pack_data->hash);
	விடுவி(idx);
	திரும்பு tmpfile;
}

நிலையான எழுத்து *keep_pack(மாறிலி எழுத்து *curr_index_name)
{
	நிலையான மாறிலி எழுத்து *keep_செய்தி = "fast-import";
	கட்டமைப்பு strbuf name = STRBUF_INIT;
	முழு keep_fd;

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_data->repo,
							    name.buf);
	எனில் (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_செய்தி, strlen(keep_செய்தி));
	எனில் (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "pack");
	எனில் (finalize_object_file(pack_data->repo, pack_data->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "idx");
	எனில் (finalize_object_file(pack_data->repo, curr_index_name, name.buf))
		die(_("cannot store index file"));
	விடுவி((வெறுமை *)curr_index_name);
	திரும்பு strbuf_detach(&name, வெற்று);
}

நிலையான வெறுமை unkeep_all_packs(வெறுமை)
{
	கட்டமைப்பு strbuf name = STRBUF_INIT;
	முழு k;

	க்கு (k = 0; k < pack_id; k++) {
		கட்டமைப்பு packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

நிலையான முழு loosen_small_pack(மாறிலி கட்டமைப்பு packed_git *p)
{
	கட்டமைப்பு child_process unpack = CHILD_PROCESS_INIT;

	எனில் (lseek(p->pack_fd, 0, SEEK_SET) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_தள்ளு(&unpack.args, "unpack-objects");
	எனில் (!show_stats)
		strvec_தள்ளு(&unpack.args, "-q");

	திரும்பு run_command(&unpack);
}

நிலையான வெறுமை end_packfile(வெறுமை)
{
	நிலையான முழு running;

	எனில் (running || !pack_data)
		திரும்பு;

	running = 1;
	clear_delta_base_cache();
	எனில் (object_மொத்தம்) {
		கட்டமைப்பு odb_source_files *files = odb_source_files_downcast(pack_data->repo->objects->source);
		கட்டமைப்பு packed_git *new_p;
		கட்டமைப்பு object_id cur_pack_oid;
		எழுத்து *idx_name;
		முழு i;
		கட்டமைப்பு branch *b;
		கட்டமைப்பு tag *t;

		close_pack_windows(pack_data);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_data->pack_fd,
					 pack_data->hash, pack_data->pack_name,
					 object_மொத்தம், cur_pack_oid.hash,
					 pack_size);

		எனில் (object_மொத்தம் <= unpack_limit) {
			எனில் (!loosen_small_pack(pack_data)) {
				invalidate_pack_id(pack_id);
				செல் discard_pack;
			}
		}

		close(pack_data->pack_fd);
		idx_name = keep_pack(create_index());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		எனில் (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		விடுவி(idx_name);

		/* Print the boundary */
		எனில் (pack_edges) {
			fprintf(pack_edges, "%s:", new_p->pack_name);
			க்கு (i = 0; i < branch_table_sz; i++) {
				க்கு (b = branch_table[i]; b; b = b->table_next_branch) {
					எனில் (b->pack_id == pack_id)
						fprintf(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			க்கு (t = first_tag; t; t = t->next_tag) {
				எனில் (t->pack_id == pack_id)
					fprintf(pack_edges, " %s",
						oid_to_hex(&t->oid));
			}
			fputc('\n', pack_edges);
			fflush(pack_edges);
		}

		pack_id++;
	}
	இல்லையெனில் {
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

நிலையான வெறுமை cycle_packfile(வெறுமை)
{
	end_packfile();
	start_packfile();
}

நிலையான முழு store_object(
	எண்ணிக்கை object_type type,
	கட்டமைப்பு strbuf *dat,
	கட்டமைப்பு last_object *last,
	கட்டமைப்பு object_id *oidout,
	uintmax_t mark)
{
	கட்டமைப்பு odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	வெறுமை *out, *delta;
	கட்டமைப்பு object_entry *e;
	unsigned எழுத்து hdr[96];
	கட்டமைப்பு object_id oid;
	unsigned long hdrlen, deltalen = 0;
	கட்டமைப்பு git_hash_ctx c;
	git_zstream s;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	hdrlen = format_object_header((எழுத்து *)hdr, அளவு(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	எனில் (oidout)
		oidcpy(oidout, &oid);

	e = செருகு_object(&oid);
	எனில் (mark)
		செருகு_mark(&marks, mark, e);
	எனில் (e->idx.offset) {
		duplicate_மொத்தம்_by_type[type]++;
		திரும்பு 1;
	}

	க்கு (கட்டமைப்பு odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		எனில் (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			தொடர்;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_மொத்தம்_by_type[type]++;
		திரும்பு 1;
	}

	எனில் (last && last->data.len && last->data.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		size_t deltalen_st;

		delta_மொத்தம்_attempts_by_type[type]++;
		delta = diff_delta(last->data.buf, last->data.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_size_t_to_ulong(deltalen_st);
	} இல்லையெனில்
		delta = வெற்று;

	git_deflate_init(&s, cfg->pack_compression_level);
	எனில் (delta) {
		s.next_in = delta;
		s.avail_in = deltalen;
	} இல்லையெனில் {
		s.next_in = (வெறுமை *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.next_out = out = xmalloc(s.avail_out);
	போது (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	எனில் ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + s.total_out) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + s.total_out) < pack_size) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		எனில் (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.next_in = (வெறுமை *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.next_out = out = xrealloc(out, s.avail_out);
			போது (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_end(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.offset = pack_size;
	object_மொத்தம்++;
	object_மொத்தம்_by_type[type]++;

	crc32_begin(pack_file);

	எனில் (delta) {
		off_t ofs = e->idx.offset - last->offset;
		unsigned pos = அளவு(hdr) - 1;

		delta_மொத்தம்_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, அளவு(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;

		hdr[pos] = ofs & 127;
		போது (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, அளவு(hdr) - pos);
		pack_size += அளவு(hdr) - pos;
	} இல்லையெனில் {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, அளவு(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;
	}

	hashwrite(pack_file, out, s.total_out);
	pack_size += s.total_out;

	e->idx.crc32 = crc32_end(pack_file);

	விடுவி(out);
	விடுவி(delta);
	எனில் (last) {
		எனில் (last->no_swap) {
			last->data = *dat;
		} இல்லையெனில் {
			strbuf_swap(&last->data, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	திரும்பு 0;
}

நிலையான வெறுமை truncate_pack(கட்டமைப்பு hashfile_checkpoint *checkpoint)
{
	எனில் (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_size = checkpoint->offset;
}

நிலையான வெறுமை stream_blob(uintmax_t len, கட்டமைப்பு object_id *oidout, uintmax_t mark)
{
	கட்டமைப்பு odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	size_t in_sz = 64 * 1024, out_sz = 64 * 1024;
	unsigned எழுத்து *in_buf = xmalloc(in_sz);
	unsigned எழுத்து *out_buf = xmalloc(out_sz);
	கட்டமைப்பு object_entry *e;
	கட்டமைப்பு object_id oid;
	unsigned long hdrlen;
	off_t offset;
	கட்டமைப்பு git_hash_ctx c;
	git_zstream s;
	கட்டமைப்பு hashfile_checkpoint checkpoint;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);
	முழு status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	எனில் ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + len) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + len) < pack_size)
		cycle_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	offset = checkpoint.offset;

	hdrlen = format_object_header((எழுத்து *)out_buf, out_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, out_buf, hdrlen);

	crc32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_in_pack_object_header(out_buf, out_sz, OBJ_BLOB, len);

	s.next_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	போது (status != Z_STREAM_END) {
		எனில் (0 < len && !s.avail_in) {
			size_t cnt = in_sz < len ? in_sz : (size_t)len;
			size_t n = fread(in_buf, 1, cnt, stdin);
			எனில் (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.next_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		எனில் (!s.avail_out || status == Z_STREAM_END) {
			size_t n = s.next_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_size += n;
			s.next_out = out_buf;
			s.avail_out = out_sz;
		}

		தேர்வு (status) {
		நிலை Z_OK:
		நிலை Z_BUF_ERROR:
		நிலை Z_STREAM_END:
			தொடர்;
		இயல்பு:
			die(_("unexpected deflate failure: %d"), status);
		}
	}
	git_deflate_end(&s);
	git_hash_final_oid(&oid, &c);

	எனில் (oidout)
		oidcpy(oidout, &oid);

	e = செருகு_object(&oid);

	எனில் (mark)
		செருகு_mark(&marks, mark, e);

	எனில் (e->idx.offset) {
		duplicate_மொத்தம்_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		செல் out;
	}

	க்கு (கட்டமைப்பு odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		எனில் (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			தொடர்;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_மொத்தம்_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		செல் out;
	}

	e->depth = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.offset = offset;
	e->idx.crc32 = crc32_end(pack_file);
	object_மொத்தம்++;
	object_மொத்தம்_by_type[OBJ_BLOB]++;

out:
	விடுவி(in_buf);
	விடுவி(out_buf);
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
நிலையான வெறுமை *gfi_unpack_entry(
	கட்டமைப்பு object_entry *oe,
	unsigned long *sizep)
{
	எண்ணிக்கை object_type type;
	size_t size_st = 0;
	வெறுமை *data;
	கட்டமைப்பு packed_git *p = all_packs[oe->pack_id];
	எனில் (p == pack_data && p->pack_size < (pack_size + the_hash_algo->rawsz)) {
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
	எனில் (sizep)
		*sizep = cast_size_t_to_ulong(size_st);
	திரும்பு data;
}

நிலையான வெறுமை load_மரம்(கட்டமைப்பு மரம்_entry *வேர்)
{
	கட்டமைப்பு object_id *oid = &வேர்->versions[1].oid;
	கட்டமைப்பு object_entry *myoe;
	கட்டமைப்பு மரம்_content *t;
	unsigned long size;
	எழுத்து *buf;
	மாறிலி எழுத்து *c;

	வேர்->மரம் = t = new_மரம்_content(8);
	எனில் (is_null_oid(oid))
		திரும்பு;

	myoe = find_object(oid);
	எனில் (myoe && myoe->pack_id != MAX_PACK_ID) {
		எனில் (myoe->type != OBJ_மரம்)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &size);
		எனில் (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} இல்லையெனில் {
		எண்ணிக்கை object_type type;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		எனில் (!buf || type != OBJ_மரம்)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	போது (c != (buf + size)) {
		கட்டமைப்பு மரம்_entry *e = new_மரம்_entry();

		எனில் (t->entry_மொத்தம் == t->entry_capacity)
			வேர்->மரம் = t = grow_மரம்_content(t, t->entry_மொத்தம்);
		t->entries[t->entry_மொத்தம்++] = e;

		e->மரம் = வெற்று;
		c = parse_mode(c, &e->versions[1].mode);
		எனில் (!c)
			die(_("corrupt mode in %s"), oid_to_hex(oid));
		e->versions[0].mode = e->versions[1].mode;
		e->name = to_atom(c, strlen(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (unsigned எழுத்து *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (unsigned எழுத்து *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	விடுவி(buf);
}

நிலையான முழு tecmp0 (மாறிலி வெறுமை *_a, மாறிலி வெறுமை *_b)
{
	கட்டமைப்பு மரம்_entry *a = *((கட்டமைப்பு மரம்_entry**)_a);
	கட்டமைப்பு மரம்_entry *b = *((கட்டமைப்பு மரம்_entry**)_b);
	திரும்பு base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

நிலையான முழு tecmp1 (மாறிலி வெறுமை *_a, மாறிலி வெறுமை *_b)
{
	கட்டமைப்பு மரம்_entry *a = *((கட்டமைப்பு மரம்_entry**)_a);
	கட்டமைப்பு மரம்_entry *b = *((கட்டமைப்பு மரம்_entry**)_b);
	திரும்பு base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

நிலையான வெறுமை mktree(கட்டமைப்பு மரம்_content *t, முழு v, கட்டமைப்பு strbuf *b)
{
	size_t maxlen = 0;
	unsigned முழு i;

	எனில் (!v)
		QSORT(t->entries, t->entry_மொத்தம், tecmp0);
	இல்லையெனில்
		QSORT(t->entries, t->entry_மொத்தம், tecmp1);

	க்கு (i = 0; i < t->entry_மொத்தம்; i++) {
		எனில் (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	க்கு (i = 0; i < t->entry_மொத்தம்; i++) {
		கட்டமைப்பு மரம்_entry *e = t->entries[i];
		எனில் (!e->versions[v].mode)
			தொடர்;
		strbuf_addf(b, "%o %s%c",
			(unsigned முழு)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

நிலையான வெறுமை store_மரம்(கட்டமைப்பு மரம்_entry *வேர்)
{
	கட்டமைப்பு மரம்_content *t;
	unsigned முழு i, j, del;
	கட்டமைப்பு last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	கட்டமைப்பு object_entry *le = வெற்று;

	எனில் (!is_null_oid(&வேர்->versions[1].oid))
		திரும்பு;

	எனில் (!வேர்->மரம்)
		load_மரம்(வேர்);
	t = வேர்->மரம்;

	க்கு (i = 0; i < t->entry_மொத்தம்; i++) {
		எனில் (t->entries[i]->மரம்)
			store_மரம்(t->entries[i]);
	}

	எனில் (!(வேர்->versions[0].mode & NO_DELTA))
		le = find_object(&வேர்->versions[0].oid);
	எனில் (S_ISDIR(வேர்->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_மரம்);
		lo.data = old_மரம்;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_மரம்);
	store_object(OBJ_மரம், &new_மரம், &lo, &வேர்->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	க்கு (i = 0, j = 0, del = 0; i < t->entry_மொத்தம்; i++) {
		கட்டமைப்பு மரம்_entry *e = t->entries[i];
		எனில் (e->versions[1].mode) {
			e->versions[0].mode = e->versions[1].mode;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} இல்லையெனில் {
			release_மரம்_entry(e);
			del++;
		}
	}
	t->entry_மொத்தம் -= del;
}

நிலையான வெறுமை மரம்_content_replace(
	கட்டமைப்பு மரம்_entry *வேர்,
	மாறிலி கட்டமைப்பு object_id *oid,
	மாறிலி uint16_t mode,
	கட்டமைப்பு மரம்_content *newtree)
{
	எனில் (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&வேர்->versions[0].oid, the_repository->hash_algo);
	oidcpy(&வேர்->versions[1].oid, oid);
	எனில் (வேர்->மரம்)
		release_மரம்_content_recursive(வேர்->மரம்);
	வேர்->மரம் = newtree;
}

நிலையான முழு மரம்_content_set(
	கட்டமைப்பு மரம்_entry *வேர்,
	மாறிலி எழுத்து *p,
	மாறிலி கட்டமைப்பு object_id *oid,
	மாறிலி uint16_t mode,
	கட்டமைப்பு மரம்_content *subtree)
{
	கட்டமைப்பு மரம்_content *t;
	மாறிலி எழுத்து *slash1;
	unsigned முழு i, n;
	கட்டமைப்பு மரம்_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	எனில் (!n)
		die(_("empty path component found in input"));
	எனில் (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	எனில் (!வேர்->மரம்)
		load_மரம்(வேர்);
	t = வேர்->மரம்;
	க்கு (i = 0; i < t->entry_மொத்தம்; i++) {
		e = t->entries[i];
		எனில் (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			எனில் (!*slash1) {
				எனில் (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					திரும்பு 0;
				e->versions[1].mode = mode;
				oidcpy(&e->versions[1].oid, oid);
				எனில் (e->மரம்)
					release_மரம்_content_recursive(e->மரம்);
				e->மரம் = subtree;

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
				எனில் (S_ISDIR(e->versions[0].mode))
					e->versions[0].mode |= NO_DELTA;

				oidclr(&வேர்->versions[1].oid, the_repository->hash_algo);
				திரும்பு 1;
			}
			எனில் (!S_ISDIR(e->versions[1].mode)) {
				e->மரம் = new_மரம்_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			எனில் (!e->மரம்)
				load_மரம்(e);
			எனில் (மரம்_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&வேர்->versions[1].oid, the_repository->hash_algo);
				திரும்பு 1;
			}
			திரும்பு 0;
		}
	}

	எனில் (t->entry_மொத்தம் == t->entry_capacity)
		வேர்->மரம் = t = grow_மரம்_content(t, t->entry_மொத்தம்);
	e = new_மரம்_entry();
	e->name = to_atom(p, n);
	e->versions[0].mode = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->entry_மொத்தம்++] = e;
	எனில் (*slash1) {
		e->மரம் = new_மரம்_content(8);
		e->versions[1].mode = S_IFDIR;
		மரம்_content_set(e, slash1 + 1, oid, mode, subtree);
	} இல்லையெனில் {
		e->மரம் = subtree;
		e->versions[1].mode = mode;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&வேர்->versions[1].oid, the_repository->hash_algo);
	திரும்பு 1;
}

நிலையான முழு மரம்_content_remove(
	கட்டமைப்பு மரம்_entry *வேர்,
	மாறிலி எழுத்து *p,
	கட்டமைப்பு மரம்_entry *backup_leaf,
	முழு allow_வேர்)
{
	கட்டமைப்பு மரம்_content *t;
	மாறிலி எழுத்து *slash1;
	unsigned முழு i, n;
	கட்டமைப்பு மரம்_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	எனில் (!வேர்->மரம்)
		load_மரம்(வேர்);

	எனில் (!*p && allow_வேர்) {
		e = வேர்;
		செல் del_entry;
	}

	t = வேர்->மரம்;
	க்கு (i = 0; i < t->entry_மொத்தம்; i++) {
		e = t->entries[i];
		எனில் (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			எனில் (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				திரும்பு 1;
			எனில் (!*slash1 || !S_ISDIR(e->versions[1].mode))
				செல் del_entry;
			எனில் (!e->மரம்)
				load_மரம்(e);
			எனில் (மரம்_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				க்கு (n = 0; n < e->மரம்->entry_மொத்தம்; n++) {
					எனில் (e->மரம்->entries[n]->versions[1].mode) {
						oidclr(&வேர்->versions[1].oid,
						       the_repository->hash_algo);
						திரும்பு 1;
					}
				}
				backup_leaf = வெற்று;
				செல் del_entry;
			}
			திரும்பு 0;
		}
	}
	திரும்பு 0;

del_entry:
	எனில் (backup_leaf)
		memcpy(backup_leaf, e, அளவு(*backup_leaf));
	இல்லையெனில் எனில் (e->மரம்)
		release_மரம்_content_recursive(e->மரம்);
	e->மரம் = வெற்று;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&வேர்->versions[1].oid, the_repository->hash_algo);
	திரும்பு 1;
}

நிலையான முழு மரம்_content_get(
	கட்டமைப்பு மரம்_entry *வேர்,
	மாறிலி எழுத்து *p,
	கட்டமைப்பு மரம்_entry *leaf,
	முழு allow_வேர்)
{
	கட்டமைப்பு மரம்_content *t;
	மாறிலி எழுத்து *slash1;
	unsigned முழு i, n;
	கட்டமைப்பு மரம்_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	எனில் (!n && !allow_வேர்)
		die(_("empty path component found in input"));

	எனில் (!வேர்->மரம்)
		load_மரம்(வேர்);

	எனில் (!n) {
		e = வேர்;
		செல் found_entry;
	}

	t = வேர்->மரம்;
	க்கு (i = 0; i < t->entry_மொத்தம்; i++) {
		e = t->entries[i];
		எனில் (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			எனில் (!*slash1)
				செல் found_entry;
			எனில் (!S_ISDIR(e->versions[1].mode))
				திரும்பு 0;
			எனில் (!e->மரம்)
				load_மரம்(e);
			திரும்பு மரம்_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	திரும்பு 0;

found_entry:
	memcpy(leaf, e, அளவு(*leaf));
	எனில் (e->மரம் && is_null_oid(&e->versions[1].oid))
		leaf->மரம் = dup_மரம்_content(e->மரம்);
	இல்லையெனில்
		leaf->மரம் = வெற்று;
	திரும்பு 1;
}

நிலையான முழு update_branch(கட்டமைப்பு branch *b)
{
	நிலையான மாறிலி எழுத்து *செய்தி = "fast-import";
	கட்டமைப்பு ref_transaction *transaction;
	கட்டமைப்பு object_id old_oid;
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	நிலையான மாறிலி எழுத்து *replace_prefix = "refs/replace/";

	எனில் (starts_with(b->name, replace_prefix) &&
	    !strcmp(b->name + strlen(replace_prefix),
		    oid_to_hex(&b->oid))) {
		எனில் (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				வெற்று, b->name, வெற்று, 0);
		திரும்பு 0;
	}
	எனில் (is_null_oid(&b->oid)) {
		எனில் (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					வெற்று, b->name, வெற்று, 0);
		திரும்பு 0;
	}
	எனில் (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	எனில் (!force_update && !is_null_oid(&old_oid)) {
		கட்டமைப்பு commit *old_cmit, *new_cmit;
		முழு ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		எனில் (!old_cmit || !new_cmit)
			திரும்பு error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		எனில் (ret < 0)
			exit(128);
		எனில் (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			திரும்பு -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	எனில் (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   வெற்று, வெற்று, 0, செய்தி, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		திரும்பு -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	திரும்பு 0;
}

நிலையான வெறுமை dump_branches(வெறுமை)
{
	unsigned முழு i;
	கட்டமைப்பு branch *b;

	க்கு (i = 0; i < branch_table_sz; i++) {
		க்கு (b = branch_table[i]; b; b = b->table_next_branch)
			failure |= update_branch(b);
	}
}

நிலையான வெறுமை dump_tags(வெறுமை)
{
	நிலையான மாறிலி எழுத்து *செய்தி = "fast-import";
	கட்டமைப்பு tag *t;
	கட்டமைப்பு strbuf ref_name = STRBUF_INIT;
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	கட்டமைப்பு ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	எனில் (!transaction) {
		failure |= error("%s", err.buf);
		செல் cleanup;
	}
	க்கு (t = first_tag; t; t = t->next_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		எனில் (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, வெற்று, வெற்று, வெற்று,
					   0, செய்தி, &err)) {
			failure |= error("%s", err.buf);
			செல் cleanup;
		}
	}
	எனில் (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

நிலையான வெறுமை dump_marks(வெறுமை)
{
	கட்டமைப்பு lock_file mark_lock = LOCK_INIT;
	FILE *f;

	எனில் (!export_marks_file || (import_marks_file && !import_marks_file_done))
		திரும்பு;

	எனில் (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		திரும்பு;
	}

	எனில் (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		திரும்பு;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	எனில் (!f) {
		முழு saved_errno = errno;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		திரும்பு;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	எனில் (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		திரும்பு;
	}
}

நிலையான வெறுமை செருகு_object_entry(கட்டமைப்பு mark_set **s, கட்டமைப்பு object_id *oid, uintmax_t mark)
{
	கட்டமைப்பு object_entry *e;
	e = find_object(oid);
	எனில் (!e) {
		எண்ணிக்கை object_type type = odb_read_object_info(the_repository->objects,
							     oid, வெற்று);
		எனில் (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = செருகு_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	செருகு_mark(s, mark, e);
}

நிலையான வெறுமை செருகு_oid_entry(கட்டமைப்பு mark_set **s, கட்டமைப்பு object_id *oid, uintmax_t mark)
{
	செருகு_mark(s, mark, xmemdupz(oid, அளவு(*oid)));
}

நிலையான வெறுமை read_mark_file(கட்டமைப்பு mark_set **s, FILE *f, mark_set_inserter_t inserter)
{
	எழுத்து வரி[512];
	போது (fgets(வரி, அளவு(வரி), f)) {
		uintmax_t mark;
		எழுத்து *end;
		கட்டமைப்பு object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, அளவு(oid.hash));

		end = strchr(வரி, '\n');
		எனில் (வரி[0] != ':' || !end)
			die(_("corrupt mark line: %s"), வரி);
		*end = 0;
		mark = strtoumax(வரி + 1, &end, 10);
		எனில் (!mark || end == வரி + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), வரி);
		inserter(s, &oid, mark);
	}
}

நிலையான வெறுமை read_marks(வெறுமை)
{
	FILE *f = fopen(import_marks_file, "r");
	எனில் (f)
		;
	இல்லையெனில் எனில் (import_marks_file_ignore_missing && errno == ENOENT)
		செல் done; /* Marks file does not exist */
	இல்லையெனில்
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, செருகு_object_entry);
	fclose(f);
done:
	import_marks_file_done = 1;
}


நிலையான முழு read_next_command(கட்டமைப்பு fast_import_state *state)
{
	நிலையான முழு stdin_eof = 0;

	எனில் (stdin_eof) {
		unread_command_buf = 0;
		திரும்பு EOF;
	}

	க்கு (;;) {
		எனில் (unread_command_buf) {
			unread_command_buf = 0;
		} இல்லையெனில் {
			கட்டமைப்பு recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			எனில் (stdin_eof)
				திரும்பு EOF;

			எனில் (!state->seen_data_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			எனில் (rc)
				rc_free = rc->next;
			இல்லையெனில் {
				rc = cmd_hist.next;
				cmd_hist.next = rc->next;
				cmd_hist.next->prev = &cmd_hist;
				விடுவி(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_tail;
			rc->next = cmd_hist.prev;
			rc->prev->next = rc;
			cmd_tail = rc;
		}
		எனில் (command_buf.buf[0] == '#')
			தொடர்;
		திரும்பு 0;
	}
}

நிலையான வெறுமை skip_optional_lf(வெறுமை)
{
	முழு term_char = fgetc(stdin);
	எனில் (term_char != '\n' && term_char != EOF)
		ungetc(term_char, stdin);
}

நிலையான வெறுமை parse_mark(கட்டமைப்பு fast_import_state *state)
{
	மாறிலி எழுத்து *v;
	எனில் (skip_prefix(command_buf.buf, "mark :", &v)) {
		next_mark = strtoumax(v, வெற்று, 10);
		read_next_command(state);
	}
	இல்லையெனில்
		next_mark = 0;
}

நிலையான வெறுமை parse_original_identifier(கட்டமைப்பு fast_import_state *state)
{
	மாறிலி எழுத்து *v;
	எனில் (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_next_command(state);
}

நிலையான முழு parse_data(கட்டமைப்பு strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	மாறிலி எழுத்து *data;
	strbuf_reset(sb);

	எனில் (!skip_prefix(command_buf.buf, "data ", &data))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	எனில் (skip_prefix(data, "<<", &data)) {
		எழுத்து *term = xstrdup(data);
		size_t term_len = command_buf.len - (data - command_buf.buf);

		க்கு (;;) {
			எனில் (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), term);
			எனில் (term_len == command_buf.len
				&& !strcmp(term, command_buf.buf))
				நிறுத்து;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		விடுவி(term);
	}
	இல்லையெனில் {
		uintmax_t len = strtoumax(data, வெற்று, 10);
		size_t n = 0, length = (size_t)len;

		எனில் (limit && limit < len) {
			*len_res = len;
			திரும்பு 0;
		}
		எனில் (length < len)
			die(_("data is too large to use in this context"));

		போது (n < length) {
			size_t s = strbuf_fread(sb, length - n, stdin);
			எனில் (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(unsigned long)(length - n));
			n += s;
		}
	}

	skip_optional_lf();
	திரும்பு 1;
}

நிலையான முழு validate_raw_date(மாறிலி எழுத்து *src, கட்டமைப்பு strbuf *result, முழு strict)
{
	மாறிலி எழுத்து *orig_src = src;
	எழுத்து *endp;
	unsigned long இலக்கம்;

	errno = 0;

	இலக்கம் = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	எனில் (errno || endp == src || *endp != ' ')
		திரும்பு -1;

	src = endp + 1;
	எனில் (*src != '-' && *src != '+')
		திரும்பு -1;

	இலக்கம் = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	எனில் (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < இலக்கம்))             /* parsed a broken timezone */
	   )
		திரும்பு -1;

	strbuf_addstr(result, orig_src);
	திரும்பு 0;
}

நிலையான எழுத்து *parse_ident(மாறிலி எழுத்து *buf)
{
	மாறிலி எழுத்து *ltgt;
	size_t name_len;
	கட்டமைப்பு strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	எனில் (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	எனில் (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	எனில் (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	எனில் (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	எனில் (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_add(&ident, buf, name_len);

	தேர்வு (whenspec) {
	நிலை WHENSPEC_RAW:
		எனில் (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		நிறுத்து;
	நிலை WHENSPEC_RAW_PERMISSIVE:
		எனில் (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		நிறுத்து;
	நிலை WHENSPEC_RFC2822:
		எனில் (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		நிறுத்து;
	நிலை WHENSPEC_NOW:
		எனில் (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		நிறுத்து;
	}

	திரும்பு strbuf_detach(&ident, வெற்று);
}

நிலையான வெறுமை parse_and_store_blob(
	கட்டமைப்பு last_object *last,
	கட்டமைப்பு object_id *oidout,
	uintmax_t mark)
{
	நிலையான கட்டமைப்பு strbuf buf = STRBUF_INIT;
	uintmax_t len;

	எனில் (parse_data(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	இல்லையெனில் {
		எனில் (last) {
			strbuf_release(&last->data);
			last->offset = 0;
			last->depth = 0;
		}
		stream_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

நிலையான வெறுமை parse_new_blob(கட்டமைப்பு fast_import_state *state)
{
	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&last_blob, வெற்று, next_mark);
}

நிலையான வெறுமை unload_one_branch(வெறுமை)
{
	போது (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = ULONG_MAX;
		கட்டமைப்பு branch *e, *l = வெற்று, *p = வெற்று;

		க்கு (e = active_branches; e; e = e->active_next_branch) {
			எனில் (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		எனில் (p) {
			e = p->active_next_branch;
			p->active_next_branch = e->active_next_branch;
		} இல்லையெனில் {
			e = active_branches;
			active_branches = e->active_next_branch;
		}
		e->active = 0;
		e->active_next_branch = வெற்று;
		எனில் (e->branch_மரம்.மரம்) {
			release_மரம்_content_recursive(e->branch_மரம்.மரம்);
			e->branch_மரம்.மரம் = வெற்று;
		}
		cur_active_branches--;
	}
}

நிலையான வெறுமை load_branch(கட்டமைப்பு branch *b)
{
	load_மரம்(&b->branch_மரம்);
	எனில் (!b->active) {
		b->active = 1;
		b->active_next_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_மொத்தம்++;
	}
}

நிலையான unsigned எழுத்து convert_இலக்கம்_notes_to_fanout(uintmax_t இலக்கம்_notes)
{
	unsigned எழுத்து fanout = 0;
	போது ((இலக்கம்_notes >>= 8))
		fanout++;
	திரும்பு fanout;
}

நிலையான வெறுமை construct_path_with_fanout(மாறிலி எழுத்து *hex_sha1,
		unsigned எழுத்து fanout, எழுத்து *path)
{
	unsigned முழு i = 0, j = 0;
	எனில் (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	போது (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	memcpy(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

நிலையான uintmax_t do_change_note_fanout(
		கட்டமைப்பு மரம்_entry *orig_வேர், கட்டமைப்பு மரம்_entry *வேர்,
		எழுத்து *hex_oid, unsigned முழு hex_oid_len,
		எழுத்து *fullpath, unsigned முழு fullpath_len,
		unsigned எழுத்து fanout)
{
	கட்டமைப்பு மரம்_content *t;
	கட்டமைப்பு மரம்_entry *e, leaf;
	unsigned முழு i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t இலக்கம்_notes = 0;
	கட்டமைப்பு object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	எழுத்து realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	மாறிலி unsigned hexsz = the_hash_algo->hexsz;

	எனில் (!வேர்->மரம்)
		load_மரம்(வேர்);
	t = வேர்->மரம்;

	க்கு (i = 0; t && i < t->entry_மொத்தம்; i++) {
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
		எனில் (!e->versions[1].mode ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			தொடர்;

		/* This _may_ be a note entry, or a subdir containing notes */
		memcpy(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		எனில் (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		memcpy(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		எனில் (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			எனில் (fanout == 0xff) {
				/* Counting mode, no rename */
				இலக்கம்_notes++;
				தொடர்;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			எனில் (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				இலக்கம்_notes++;
				தொடர்;
			}

			/* Rename fullpath to realpath */
			எனில் (!மரம்_content_remove(orig_வேர், fullpath, &leaf, 0))
				die(_("failed to remove path %s"), fullpath);
			மரம்_content_set(orig_வேர், realpath,
				&leaf.versions[1].oid,
				leaf.versions[1].mode,
				leaf.மரம்);
		} இல்லையெனில் எனில் (S_ISDIR(e->versions[1].mode)) {
			/* This is a subdir that may contain note entries */
			இலக்கம்_notes += do_change_note_fanout(orig_வேர், e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = வேர்->மரம்;
	}
	திரும்பு இலக்கம்_notes;
}

நிலையான uintmax_t change_note_fanout(கட்டமைப்பு மரம்_entry *வேர்,
		unsigned எழுத்து fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	எழுத்து hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	திரும்பு do_change_note_fanout(வேர், வேர், hex_oid, 0, path, 0, fanout);
}

நிலையான முழு parse_mapped_oid_hex(மாறிலி எழுத்து *hex, கட்டமைப்பு object_id *oid, மாறிலி எழுத்து **end)
{
	முழு algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, அளவு(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	எனில் (algo == GIT_HASH_UNKNOWN)
		திரும்பு -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	எனில் (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		எனில் (hash_algos[algo].format_id == the_hash_algo->format_id)
			திரும்பு 0;
		திரும்பு -1;
	}
	oidcpy(oid, kh_மதிப்பு(sub_oid_map, it));
	திரும்பு 0;
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
நிலையான uintmax_t parse_mark_ref(மாறிலி எழுத்து *p, எழுத்து **endptr)
{
	uintmax_t mark;

	ப்ரதிஜ்ஞா(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	எனில் (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	திரும்பு mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
நிலையான uintmax_t parse_mark_ref_eol(மாறிலி எழுத்து *p)
{
	எழுத்து *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	எனில் (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	திரும்பு mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
நிலையான uintmax_t parse_mark_ref_space(மாறிலி எழுத்து **p)
{
	uintmax_t mark;
	எழுத்து *end;

	mark = parse_mark_ref(*p, &end);
	எனில் (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	திரும்பு mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
நிலையான வெறுமை parse_path(கட்டமைப்பு strbuf *sb, மாறிலி எழுத்து *p, மாறிலி எழுத்து **endp,
		முழு is_last_field, மாறிலி எழுத்து *field)
{
	எனில் (*p == '"') {
		எனில் (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), field, command_buf.buf);
		எனில் (strlen(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), field, command_buf.buf);
	} இல்லையெனில் {
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
நிலையான வெறுமை parse_path_eol(கட்டமைப்பு strbuf *sb, மாறிலி எழுத்து *p, மாறிலி எழுத்து *field)
{
	மாறிலி எழுத்து *end;

	parse_path(sb, p, &end, 1, field);
	எனில் (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
நிலையான வெறுமை parse_path_space(கட்டமைப்பு strbuf *sb, மாறிலி எழுத்து *p,
		மாறிலி எழுத்து **endp, மாறிலி எழுத்து *field)
{
	parse_path(sb, p, endp, 0, field);
	எனில் (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

நிலையான வெறுமை file_change_m(கட்டமைப்பு fast_import_state *state,
			  மாறிலி எழுத்து *p,
			  கட்டமைப்பு branch *b)
{
	நிலையான கட்டமைப்பு strbuf path = STRBUF_INIT;
	கட்டமைப்பு object_entry *oe;
	கட்டமைப்பு object_id oid;
	uint16_t mode, inline_data = 0;

	p = parse_mode(p, &mode);
	எனில் (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	தேர்வு (mode) {
	நிலை 0644:
	நிலை 0755:
		mode |= S_IFREG;
	நிலை S_IFREG | 0644:
	நிலை S_IFREG | 0755:
	நிலை S_IFLNK:
	நிலை S_IFDIR:
	நிலை S_IFGITLINK:
		/* ok */
		நிறுத்து;
	இயல்பு:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	எனில் (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} இல்லையெனில் எனில் (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = வெற்று; /* not used with inline_data, but makes gcc happy */
	} இல்லையெனில் {
		எனில் (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		எனில் (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	எனில் (S_ISDIR(mode) &&
	    is_empty_மரம்_oid(&oid, the_repository->hash_algo) &&
	    *path.buf) {
		மரம்_content_remove(&b->branch_மரம், path.buf, வெற்று, 0);
		திரும்பு;
	}

	எனில் (S_ISGITLINK(mode)) {
		எனில் (inline_data)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		இல்லையெனில் எனில் (oe) {
			எனில் (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} இல்லையெனில் எனில் (inline_data) {
		எனில் (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		போது (read_next_command(state) != EOF) {
			மாறிலி எழுத்து *v;
			எனில் (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			இல்லையெனில் {
				parse_and_store_blob(&last_blob, &oid, 0);
				நிறுத்து;
			}
		}
	} இல்லையெனில் {
		எண்ணிக்கை object_type expected = S_ISDIR(mode) ?
						OBJ_மரம்: OBJ_BLOB;
		எண்ணிக்கை object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, வெற்று);
		எனில் (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(mode) ?  _("tree") : _("blob"),
			    command_buf.buf);
		எனில் (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	எனில் (!*path.buf) {
		மரம்_content_replace(&b->branch_மரம், &oid, mode, வெற்று);
		திரும்பு;
	}

	எனில் (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	மரம்_content_set(&b->branch_மரம், path.buf, &oid, mode, வெற்று);
}

நிலையான வெறுமை file_change_d(மாறிலி எழுத்து *p, கட்டமைப்பு branch *b)
{
	நிலையான கட்டமைப்பு strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	மரம்_content_remove(&b->branch_மரம், path.buf, வெற்று, 1);
}

நிலையான வெறுமை file_change_cr(மாறிலி எழுத்து *p, கட்டமைப்பு branch *b, முழு rename)
{
	நிலையான கட்டமைப்பு strbuf source = STRBUF_INIT;
	நிலையான கட்டமைப்பு strbuf dest = STRBUF_INIT;
	கட்டமைப்பு மரம்_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	memset(&leaf, 0, அளவு(leaf));
	எனில் (rename)
		மரம்_content_remove(&b->branch_மரம், source.buf, &leaf, 1);
	இல்லையெனில்
		மரம்_content_get(&b->branch_மரம், source.buf, &leaf, 1);
	எனில் (!leaf.versions[1].mode)
		die(_("path %s not in branch"), source.buf);
	எனில் (!*dest.buf) {	/* C "path/to/subdir" "" */
		மரம்_content_replace(&b->branch_மரம்,
			&leaf.versions[1].oid,
			leaf.versions[1].mode,
			leaf.மரம்);
		திரும்பு;
	}
	எனில் (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	மரம்_content_set(&b->branch_மரம், dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.மரம்);
}

நிலையான வெறுமை note_change_n(கட்டமைப்பு fast_import_state *state,
			  மாறிலி எழுத்து *p,
			  கட்டமைப்பு branch *b,
			  unsigned எழுத்து *old_fanout)
{
	கட்டமைப்பு object_entry *oe;
	கட்டமைப்பு branch *s;
	கட்டமைப்பு object_id oid, commit_oid;
	எழுத்து path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_data = 0;
	unsigned எழுத்து new_fanout;

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
	எனில் (b->இலக்கம்_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->இலக்கம்_notes = change_note_fanout(&b->branch_மரம், 0xff);
		*old_fanout = convert_இலக்கம்_notes_to_fanout(b->இலக்கம்_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	எனில் (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} இல்லையெனில் எனில் (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = வெற்று; /* not used with inline_data, but makes gcc happy */
	} இல்லையெனில் {
		எனில் (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		எனில் (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	எனில் (s) {
		எனில் (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} இல்லையெனில் எனில் (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		கட்டமைப்பு object_entry *commit_oe = find_mark(marks, commit_mark);
		எனில் (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} இல்லையெனில் எனில் (!repo_get_oid(the_repository, p, &commit_oid)) {
		size_t size;
		எழுத்து *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &size,
						   &commit_oid);
		எனில் (!buf || size < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		விடுவி(buf);
	} இல்லையெனில்
		die(_("invalid ref name or SHA1 expression: %s"), p);

	எனில் (inline_data) {
		read_next_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} இல்லையெனில் எனில் (oe) {
		எனில் (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} இல்லையெனில் எனில் (!is_null_oid(&oid)) {
		எண்ணிக்கை object_type type = odb_read_object_info(the_repository->objects, &oid,
							வெற்று);
		எனில் (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		எனில் (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	எனில் (மரம்_content_remove(&b->branch_மரம், path, வெற்று, 0))
		b->இலக்கம்_notes--;

	எனில் (is_null_oid(&oid))
		திரும்பு; /* nothing to insert */

	b->இலக்கம்_notes++;
	new_fanout = convert_இலக்கம்_notes_to_fanout(b->இலக்கம்_notes);
	construct_path_with_fanout(oid_to_hex(&commit_oid), new_fanout, path);
	மரம்_content_set(&b->branch_மரம், path, &oid, S_IFREG | 0644, வெற்று);
}

நிலையான வெறுமை file_change_deleteall(கட்டமைப்பு branch *b)
{
	release_மரம்_content_recursive(b->branch_மரம்.மரம்);
	oidclr(&b->branch_மரம்.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_மரம்.versions[1].oid, the_repository->hash_algo);
	load_மரம்(&b->branch_மரம்);
	b->இலக்கம்_notes = 0;
}

நிலையான வெறுமை parse_from_commit(கட்டமைப்பு branch *b, எழுத்து *buf, unsigned long size)
{
	எனில் (!buf || size < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	எனில் (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_மரம்.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_மரம்.versions[0].oid,
	       &b->branch_மரம்.versions[1].oid);
}

நிலையான வெறுமை parse_from_existing(கட்டமைப்பு branch *b)
{
	எனில் (is_null_oid(&b->oid)) {
		oidclr(&b->branch_மரம்.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_மரம்.versions[1].oid, the_repository->hash_algo);
	} இல்லையெனில் {
		unsigned long size;
		size_t size_st = 0;
		எழுத்து *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &size_st, &b->oid);
		size = cast_size_t_to_ulong(size_st);
		parse_from_commit(b, buf, size);
		விடுவி(buf);
	}
}

நிலையான முழு parse_objectish(கட்டமைப்பு fast_import_state *state,
			   கட்டமைப்பு branch *b,
			   மாறிலி எழுத்து *objectish)
{
	கட்டமைப்பு branch *s;
	கட்டமைப்பு object_id oid;

	oidcpy(&oid, &b->branch_மரம்.versions[1].oid);

	s = lookup_branch(objectish);
	எனில் (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	இல்லையெனில் எனில் (s) {
		கட்டமைப்பு object_id *t = &s->branch_மரம்.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_மரம்.versions[0].oid, t);
		oidcpy(&b->branch_மரம்.versions[1].oid, t);
	} இல்லையெனில் எனில் (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		கட்டமைப்பு object_entry *oe = find_mark(marks, idnum);
		எனில் (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		எனில் (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			எனில் (oe->pack_id != MAX_PACK_ID) {
				unsigned long size;
				எழுத்து *buf = gfi_unpack_entry(oe, &size);
				parse_from_commit(b, buf, size);
				விடுவி(buf);
			} இல்லையெனில்
				parse_from_existing(b);
		}
	} இல்லையெனில் எனில் (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		எனில் (is_null_oid(&b->oid))
			b->delete = 1;
	}
	இல்லையெனில்
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	எனில் (b->branch_மரம்.மரம் && !oideq(&oid, &b->branch_மரம்.versions[1].oid)) {
		release_மரம்_content_recursive(b->branch_மரம்.மரம்);
		b->branch_மரம்.மரம் = வெற்று;
	}

	read_next_command(state);
	திரும்பு 1;
}

நிலையான முழு parse_from(கட்டமைப்பு fast_import_state *state, கட்டமைப்பு branch *b)
{
	மாறிலி எழுத்து *from;

	எனில் (!skip_prefix(command_buf.buf, "from ", &from))
		திரும்பு 0;

	திரும்பு parse_objectish(state, b, from);
}

நிலையான முழு parse_objectish_with_prefix(கட்டமைப்பு fast_import_state *state,
				       கட்டமைப்பு branch *b,
				       மாறிலி எழுத்து *prefix)
{
	மாறிலி எழுத்து *base;

	எனில் (!skip_prefix(command_buf.buf, prefix, &base))
		திரும்பு 0;

	திரும்பு parse_objectish(state, b, base);
}

நிலையான கட்டமைப்பு hash_list *parse_merge(கட்டமைப்பு fast_import_state *state,
				     unsigned முழு *மொத்தம்)
{
	கட்டமைப்பு hash_list *list = வெற்று, **tail = &list, *n;
	மாறிலி எழுத்து *from;
	கட்டமைப்பு branch *s;

	*மொத்தம் = 0;
	போது (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(அளவு(*n));
		s = lookup_branch(from);
		எனில் (s)
			oidcpy(&n->oid, &s->oid);
		இல்லையெனில் எனில் (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			கட்டமைப்பு object_entry *oe = find_mark(marks, idnum);
			எனில் (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} இல்லையெனில் எனில் (!repo_get_oid(the_repository, from, &n->oid)) {
			size_t size;
			எழுத்து *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &size, &n->oid);
			எனில் (!buf || size < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			விடுவி(buf);
		} இல்லையெனில்
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->next = வெற்று;
		*tail = n;
		tail = &n->next;

		(*மொத்தம்)++;
		read_next_command(state);
	}
	திரும்பு list;
}

கட்டமைப்பு signature_data {
	எழுத்து *hash_algo;      /* "sha1" or "sha256" */
	எழுத்து *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	கட்டமைப்பு strbuf data;   /* The actual signature data */
};

நிலையான வெறுமை parse_one_signature(கட்டமைப்பு fast_import_state *state,
				கட்டமைப்பு signature_data *sig,
				மாறிலி எழுத்து *v)
{
	எழுத்து *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	எழுத்து *space = strchr(args, ' ');

	எனில் (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), args);
	*space = '\0';

	sig->hash_algo = args;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	எனில் (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	எனில் (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	எனில் (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_next_command(state);
	parse_data(&sig->data, 0, வெற்று);
}

நிலையான வெறுமை discard_one_signature(கட்டமைப்பு fast_import_state *state)
{
	கட்டமைப்பு strbuf data = STRBUF_INIT;

	read_next_command(state);
	parse_data(&data, 0, வெற்று);
	strbuf_release(&data);
}

நிலையான வெறுமை add_gpgsig_to_commit(கட்டமைப்பு strbuf *commit_data,
				 மாறிலி எழுத்து *header,
				 கட்டமைப்பு signature_data *sig)
{
	கட்டமைப்பு string_list siglines = STRING_LIST_INIT_NODUP;

	எனில் (!sig || !sig->hash_algo)
		திரும்பு;

	strbuf_addstr(commit_data, header);
	string_list_split_in_place(&siglines, sig->data.buf, "\n", -1);
	strbuf_add_separated_string_list(commit_data, "\n ", &siglines);
	strbuf_addch(commit_data, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->data);
	விடுவி(sig->hash_algo);
}

நிலையான வெறுமை store_signature(கட்டமைப்பு signature_data *stored_sig,
			    கட்டமைப்பு signature_data *new_sig,
			    மாறிலி எழுத்து *hash_type)
{
	எனில் (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->data);
		விடுவி(new_sig->hash_algo);
	} இல்லையெனில் {
		*stored_sig = *new_sig;
	}
}

நிலையான வெறுமை import_one_signature(கட்டமைப்பு fast_import_state *state,
				 கட்டமைப்பு signature_data *sig_sha1,
				 கட்டமைப்பு signature_data *sig_sha256,
				 மாறிலி எழுத்து *v)
{
	கட்டமைப்பு signature_data sig = { வெற்று, வெற்று, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	எனில் (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	இல்லையெனில் எனில் (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	இல்லையெனில்
		die(_("parse_one_signature() returned unknown hash algo"));
}

நிலையான வெறுமை finalize_commit_buffer(கட்டமைப்பு strbuf *new_data,
				   கட்டமைப்பு signature_data *sig_sha1,
				   கட்டமைப்பு signature_data *sig_sha256,
				   கட்டமைப்பு strbuf *செய்தி)
{
	add_gpgsig_to_commit(new_data, "gpgsig ", sig_sha1);
	add_gpgsig_to_commit(new_data, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_data, '\n');
	strbuf_addbuf(new_data, செய்தி);
}

நிலையான வெறுமை warn_invalid_signature(கட்டமைப்பு signature_check *check,
				   மாறிலி எழுத்து *செய்தி, எண்ணிக்கை sign_mode mode)
{
	மாறிலி எழுத்து *signer = check->signer ? check->signer : _("unknown");
	மாறிலி எழுத்து *subject;
	முழு subject_len = find_commit_subject(செய்தி, &subject);

	தேர்வு (mode) {
	நிலை SIGN_STRIP_IF_INVALID:
		எனில் (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		இல்லையெனில் எனில் (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		இல்லையெனில்
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		நிறுத்து;
	நிலை SIGN_SIGN_IF_INVALID:
		எனில் (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		இல்லையெனில் எனில் (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		இல்லையெனில்
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		நிறுத்து;
	இயல்பு:
		BUG("unsupported signing mode");
	}
}

நிலையான வெறுமை handle_signature_if_invalid(கட்டமைப்பு strbuf *new_data,
					கட்டமைப்பு signature_data *sig_sha1,
					கட்டமைப்பு signature_data *sig_sha256,
					கட்டமைப்பு strbuf *செய்தி,
					எண்ணிக்கை sign_mode mode)
{
	கட்டமைப்பு strbuf tmp_buf = STRBUF_INIT;
	கட்டமைப்பு signature_check signature_check = { 0 };
	முழு ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_data);
	finalize_commit_buffer(&tmp_buf, sig_sha1, sig_sha256, செய்தி);
	ret = verify_commit_buffer(tmp_buf.buf, tmp_buf.len, &signature_check);

	எனில் (ret) {
		எனில் (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, செய்தி->buf, mode);

		எனில் (mode == SIGN_SIGN_IF_INVALID) {
			கட்டமைப்பு strbuf signature = STRBUF_INIT;
			கட்டமைப்பு strbuf payload = STRBUF_INIT;

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
			எனில் (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			எனில் (sign_buffer(&payload, &signature, signed_commit_keyid,
					SIGN_BUFFER_USE_DEFAULT_சாவி))
				die(_("failed to sign commit object"));
			add_header_signature(new_data, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_buffer(new_data, வெற்று, வெற்று, செய்தி);
	} இல்லையெனில் {
		strbuf_swap(new_data, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

நிலையான வெறுமை parse_new_commit(கட்டமைப்பு fast_import_state *state, மாறிலி எழுத்து *arg)
{
	நிலையான கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
	கட்டமைப்பு signature_data sig_sha1 = { வெற்று, வெற்று, STRBUF_INIT };
	கட்டமைப்பு signature_data sig_sha256 = { வெற்று, வெற்று, STRBUF_INIT };
	கட்டமைப்பு branch *b;
	எழுத்து *author = வெற்று;
	எழுத்து *committer = வெற்று;
	எழுத்து *encoding = வெற்று;
	கட்டமைப்பு hash_list *merge_list = வெற்று;
	unsigned முழு merge_மொத்தம்;
	unsigned எழுத்து prev_fanout, new_fanout;
	மாறிலி எழுத்து *v;

	b = lookup_branch(arg);
	எனில் (!b)
		b = new_branch(arg);

	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	எனில் (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_next_command(state);
	}
	எனில் (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_next_command(state);
	}
	எனில் (!committer)
		die(_("expected committer but didn't get one"));

	போது (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		தேர்வு (signed_commit_mode) {

		/* First, modes that don't need the signature to be parsed */
		நிலை SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		நிலை SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		நிலை SIGN_STRIP:
			discard_one_signature(state);
			நிறுத்து;

		/* Second, modes that parse the signature */
		நிலை SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		நிலை SIGN_VERBATIM:
		நிலை SIGN_STRIP_IF_INVALID:
		நிலை SIGN_SIGN_IF_INVALID:
		நிலை SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			நிறுத்து;

		/* Third, BUG */
		இயல்பு:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_next_command(state);
	}

	எனில் (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_next_command(state);
	}
	parse_data(&செய்தி, 0, வெற்று);
	read_next_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_மொத்தம்);

	/* ensure the branch is active/loaded */
	எனில் (!b->branch_மரம்.மரம் || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	prev_fanout = convert_இலக்கம்_notes_to_fanout(b->இலக்கம்_notes);

	/* file_change* */
	போது (command_buf.len > 0) {
		எனில் (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		இல்லையெனில் எனில் (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		இல்லையெனில் {
			unread_command_buf = 1;
			நிறுத்து;
		}
		எனில் (read_next_command(state) == EOF)
			நிறுத்து;
	}

	new_fanout = convert_இலக்கம்_notes_to_fanout(b->இலக்கம்_notes);
	எனில் (new_fanout != prev_fanout)
		b->இலக்கம்_notes = change_note_fanout(&b->branch_மரம், new_fanout);

	/* build the tree and the commit */
	store_மரம்(&b->branch_மரம்);
	oidcpy(&b->branch_மரம்.versions[0].oid,
	       &b->branch_மரம்.versions[1].oid);

	strbuf_reset(&new_data);
	strbuf_addf(&new_data, "tree %s\n",
		oid_to_hex(&b->branch_மரம்.versions[1].oid));
	எனில் (!is_null_oid(&b->oid))
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&b->oid));
	போது (merge_list) {
		கட்டமைப்பு hash_list *next = merge_list->next;
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&merge_list->oid));
		விடுவி(merge_list);
		merge_list = next;
	}
	strbuf_addf(&new_data,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	எனில் (encoding)
		strbuf_addf(&new_data,
			"encoding %s\n",
			encoding);

	எனில் ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
	     signed_commit_mode == SIGN_SIGN_IF_INVALID ||
	     signed_commit_mode == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_data, &sig_sha1, &sig_sha256,
					    &செய்தி, signed_commit_mode);
	இல்லையெனில்
		finalize_commit_buffer(&new_data, &sig_sha1, &sig_sha256, &செய்தி);

	விடுவி(author);
	விடுவி(committer);
	விடுவி(encoding);

	எனில் (!store_object(OBJ_COMMIT, &new_data, வெற்று, &b->oid, next_mark))
		b->pack_id = pack_id;
	b->last_commit = object_மொத்தம்_by_type[OBJ_COMMIT];
}

நிலையான வெறுமை handle_tag_signature_if_invalid(கட்டமைப்பு strbuf *buf,
					    கட்டமைப்பு strbuf *செய்தி,
					    size_t sig_offset)
{
	கட்டமைப்பு strbuf signature = STRBUF_INIT;
	கட்டமைப்பு strbuf payload = STRBUF_INIT;
	கட்டமைப்பு signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_add(&payload, செய்தி->buf, sig_offset);
	strbuf_add(&signature, செய்தி->buf + sig_offset, செய்தி->len - sig_offset);

	sigc.payload_type = SIGNATURE_PAYLOAD_TAG;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	எனில் (!check_signature(&sigc, signature.buf, signature.len))
		செல் out;

	எனில் (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(செய்தி, sig_offset);

	எனில் (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = வெற்று;
		strbuf_reset(&signature);

		எனில் (sign_buffer(&payload, &signature, signed_tag_keyid,
				SIGN_BUFFER_USE_DEFAULT_சாவி))
			die(_("failed to sign tag object"));

		strbuf_addbuf(செய்தி, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

நிலையான வெறுமை handle_tag_signature(கட்டமைப்பு strbuf *buf, கட்டமைப்பு strbuf *செய்தி, மாறிலி எழுத்து *name)
{
	size_t sig_offset = parse_signed_buffer(செய்தி->buf, செய்தி->len);

	/* If there is no signature, there is nothing to do. */
	எனில் (sig_offset >= செய்தி->len)
		திரும்பு;

	தேர்வு (signed_tag_mode) {

	/* First, modes that don't change anything */
	நிலை SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	நிலை SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		நிறுத்து;

	/* Second, modes that remove the signature */
	நிலை SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	நிலை SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(செய்தி, sig_offset);
		நிறுத்து;
	நிலை SIGN_ABORT_IF_INVALID:
	நிலை SIGN_SIGN_IF_INVALID:
	நிலை SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, செய்தி, sig_offset);
		நிறுத்து;

	/* Third, aborting modes */
	நிலை SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	இயல்பு:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

நிலையான வெறுமை parse_new_tag(கட்டமைப்பு fast_import_state *state, மாறிலி எழுத்து *arg)
{
	நிலையான கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
	மாறிலி எழுத்து *from;
	எழுத்து *tagger;
	கட்டமைப்பு branch *s;
	கட்டமைப்பு tag *t;
	uintmax_t from_mark = 0;
	கட்டமைப்பு object_id oid;
	எண்ணிக்கை object_type type;
	மாறிலி எழுத்து *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, அளவு(கட்டமைப்பு tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	எனில் (last_tag)
		last_tag->next_tag = t;
	இல்லையெனில்
		first_tag = t;
	last_tag = t;
	read_next_command(state);
	parse_mark(state);

	/* from ... */
	எனில் (!skip_prefix(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	எனில் (s) {
		எனில் (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} இல்லையெனில் எனில் (*from == ':') {
		கட்டமைப்பு object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} இல்லையெனில் எனில் (!repo_get_oid(the_repository, from, &oid)) {
		கட்டமைப்பு object_entry *oe = find_object(&oid);
		எனில் (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, வெற்று);
			எனில் (type < 0)
				die(_("not a valid object: %s"), from);
		} இல்லையெனில்
			type = oe->type;
	} இல்லையெனில்
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_next_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	எனில் (skip_prefix(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_next_command(state);
	} இல்லையெனில்
		tagger = வெற்று;

	/* tag payload/message */
	parse_data(&செய்தி, 0, வெற்று);

	/* build the tag object */
	strbuf_reset(&new_data);

	strbuf_addf(&new_data,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_to_hex(&oid), type_name(type), t->name);
	எனில் (tagger)
		strbuf_addf(&new_data,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_data, &செய்தி, t->name);

	strbuf_addch(&new_data, '\n');
	strbuf_addbuf(&new_data, &செய்தி);
	விடுவி(tagger);

	எனில் (store_object(OBJ_TAG, &new_data, வெற்று, &t->oid, next_mark))
		t->pack_id = MAX_PACK_ID;
	இல்லையெனில்
		t->pack_id = pack_id;
}

நிலையான வெறுமை parse_reset_branch(கட்டமைப்பு fast_import_state *state, மாறிலி எழுத்து *arg)
{
	கட்டமைப்பு branch *b;
	மாறிலி எழுத்து *tag_name;

	b = lookup_branch(arg);
	எனில் (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_மரம்.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_மரம்.versions[1].oid, the_repository->hash_algo);
		எனில் (b->branch_மரம்.மரம்) {
			release_மரம்_content_recursive(b->branch_மரம்.மரம்);
			b->branch_மரம்.மரம் = வெற்று;
		}
	}
	இல்லையெனில்
		b = new_branch(arg);
	read_next_command(state);
	parse_from(state, b);
	எனில் (b->delete && skip_prefix(b->name, "refs/tags/", &tag_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		கட்டமைப்பு tag *t, *prev = வெற்று;
		க்கு (t = first_tag; t; t = t->next_tag) {
			எனில் (!strcmp(t->name, tag_name))
				நிறுத்து;
			prev = t;
		}
		எனில் (t) {
			எனில் (prev)
				prev->next_tag = t->next_tag;
			இல்லையெனில்
				first_tag = t->next_tag;
			எனில் (!t->next_tag)
				last_tag = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	எனில் (command_buf.len > 0)
		unread_command_buf = 1;
}

நிலையான வெறுமை cat_blob_write(மாறிலி எழுத்து *buf, unsigned long size)
{
	எனில் (write_in_full(cat_blob_fd, buf, size) < 0)
		die_errno(_("write to frontend failed"));
}

நிலையான வெறுமை cat_blob(கட்டமைப்பு object_entry *oe, கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு strbuf வரி = STRBUF_INIT;
	கட்டமைப்பு iovec iov[3];
	unsigned long size;
	எண்ணிக்கை object_type type = 0;
	எழுத்து *buf;

	எனில் (!oe || oe->pack_id == MAX_PACK_ID) {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
	} இல்லையெனில் {
		type = oe->type;
		buf = gfi_unpack_entry(oe, &size);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	எனில் (type <= 0) {
		strbuf_reset(&வரி);
		strbuf_addf(&வரி, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(வரி.buf, வரி.len);
		strbuf_release(&வரி);
		விடுவி(buf);
		திரும்பு;
	}
	எனில் (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	எனில் (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_to_hex(oid), type_name(type));
	strbuf_reset(&வரி);
	strbuf_addf(&வரி, "%s %s %"PRIuMAX"\n", oid_to_hex(oid),
		    type_name(type), (uintmax_t)size);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_base = வரி.buf;
	iov[0].iov_len = வரி.len;
	iov[1].iov_base = buf;
	iov[1].iov_len = size;
	iov[2].iov_base = (வெறுமை *) "\n";
	iov[2].iov_len = 1;

	எனில் (writev_in_full(cat_blob_fd, iov, ARRAY_SIZE(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&வரி);
	எனில் (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.data, buf, size, size + 1);
		last_blob.depth = oe->depth;
	} இல்லையெனில்
		விடுவி(buf);
}

நிலையான வெறுமை parse_get_mark(கட்டமைப்பு fast_import_state *state UNUSED,
			   மாறிலி எழுத்து *p)
{
	கட்டமைப்பு object_entry *oe;
	எழுத்து output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	எனில் (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	எனில் (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, அளவு(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

நிலையான வெறுமை parse_cat_blob(கட்டமைப்பு fast_import_state *state UNUSED,
			   மாறிலி எழுத்து *p)
{
	கட்டமைப்பு object_entry *oe;
	கட்டமைப்பு object_id oid;

	/* cat-blob SP <object> LF */
	எனில் (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		எனில் (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} இல்லையெனில் {
		எனில் (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		எனில் (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

நிலையான கட்டமைப்பு object_entry *dereference(கட்டமைப்பு object_entry *oe,
					கட்டமைப்பு object_id *oid)
{
	unsigned long size;
	எழுத்து *buf = வெற்று;
	மாறிலி unsigned hexsz = the_hash_algo->hexsz;

	எனில் (!oe) {
		எண்ணிக்கை object_type type = odb_read_object_info(the_repository->objects,
							     oid, வெற்று);
		எனில் (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = செருகு_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	தேர்வு (oe->type) {
	நிலை OBJ_மரம்:	/* easy case. */
		திரும்பு oe;
	நிலை OBJ_COMMIT:
	நிலை OBJ_TAG:
		நிறுத்து;
	இயல்பு:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	எனில் (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &size);
	} இல்லையெனில் {
		எண்ணிக்கை object_type unused;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &size_st);
		size = cast_size_t_to_ulong(size_st);
	}
	எனில் (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	தேர்வு (oe->type) {
	நிலை OBJ_TAG:
		எனில் (size < hexsz + strlen("object ") ||
		    get_oid_hex(buf + strlen("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		நிறுத்து;
	நிலை OBJ_COMMIT:
		எனில் (size < hexsz + strlen("tree ") ||
		    get_oid_hex(buf + strlen("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	விடுவி(buf);
	திரும்பு find_object(oid);
}

நிலையான வெறுமை செருகு_mapped_mark(uintmax_t mark, வெறுமை *object, வெறுமை *cbp)
{
	கட்டமைப்பு object_id *fromoid = object;
	கட்டமைப்பு object_id *tooid = find_mark(cbp, mark);
	முழு ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	எனில் (ret == 0)
		திரும்பு;
	kh_மதிப்பு(sub_oid_map, it) = tooid;
}

நிலையான வெறுமை build_mark_map_one(கட்டமைப்பு mark_set *from, கட்டமைப்பு mark_set *to)
{
	for_each_mark(from, 0, செருகு_mapped_mark, to);
}

நிலையான வெறுமை build_mark_map(கட்டமைப்பு string_list *from, கட்டமைப்பு string_list *to)
{
	கட்டமைப்பு string_list_item *fromp, *உச்சி;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		உச்சி = string_list_lookup(to, fromp->string);
		எனில் (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} இல்லையெனில் எனில் (!உச்சி || !உச்சி->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, உச்சி->util);
	}
}

நிலையான கட்டமைப்பு object_entry *parse_treeish_dataref(மாறிலி எழுத்து **p)
{
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு object_entry *e;

	எனில் (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		எனில் (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} இல்லையெனில் {	/* <sha1> */
		எனில் (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		எனில் (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	போது (!e || e->type != OBJ_மரம்)
		e = dereference(e, &oid);
	திரும்பு e;
}

நிலையான வெறுமை print_ls(முழு mode, மாறிலி unsigned எழுத்து *hash, மாறிலி எழுத்து *path)
{
	நிலையான கட்டமைப்பு strbuf வரி = STRBUF_INIT;

	/* See show_tree(). */
	மாறிலி எழுத்து *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? மரம்_type :
		blob_type;

	எனில் (!mode) {
		/* missing SP path LF */
		strbuf_reset(&வரி);
		strbuf_addstr(&வரி, "missing ");
		quote_c_style(path, &வரி, வெற்று, 0);
		strbuf_addch(&வரி, '\n');
	} இல்லையெனில் {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&வரி);
		strbuf_addf(&வரி, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &வரி, வெற்று, 0);
		strbuf_addch(&வரி, '\n');
	}
	cat_blob_write(வரி.buf, வரி.len);
}

நிலையான வெறுமை parse_ls(கட்டமைப்பு fast_import_state *state UNUSED,
		     மாறிலி எழுத்து *p,
		     கட்டமைப்பு branch *b)
{
	நிலையான கட்டமைப்பு strbuf path = STRBUF_INIT;
	கட்டமைப்பு மரம்_entry *வேர் = வெற்று;
	கட்டமைப்பு மரம்_entry leaf = {வெற்று};

	/* ls SP (<tree-ish> SP)? <path> */
	எனில் (*p == '"') {
		எனில் (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		வேர் = &b->branch_மரம்;
	} இல்லையெனில் {
		கட்டமைப்பு object_entry *e = parse_treeish_dataref(&p);
		வேர் = new_மரம்_entry();
		oidcpy(&வேர்->versions[1].oid, &e->idx.oid);
		எனில் (!is_null_oid(&வேர்->versions[1].oid))
			வேர்->versions[1].mode = S_IFDIR;
		load_மரம்(வேர்);
	}
	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	மரம்_content_get(வேர், path.buf, &leaf, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	எனில் (S_ISDIR(leaf.versions[1].mode))
		store_மரம்(&leaf);

	print_ls(leaf.versions[1].mode, leaf.versions[1].oid.hash, path.buf);
	எனில் (leaf.மரம்)
		release_மரம்_content_recursive(leaf.மரம்);
	எனில் (!b || வேர் != &b->branch_மரம்)
		release_மரம்_entry(வேர்);
}

நிலையான வெறுமை checkpoint(வெறுமை)
{
	checkpoint_requested = 0;
	எனில் (object_மொத்தம்) {
		cycle_packfile();
	}
	dump_branches();
	dump_tags();
	dump_marks();
}

நிலையான வெறுமை parse_checkpoint(கட்டமைப்பு fast_import_state *state UNUSED)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

நிலையான வெறுமை parse_progress(கட்டமைப்பு fast_import_state *state UNUSED)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

நிலையான வெறுமை parse_alias(கட்டமைப்பு fast_import_state *state)
{
	கட்டமைப்பு object_entry *e;
	கட்டமைப்பு branch b;

	skip_optional_lf();
	read_next_command(state);

	/* mark ... */
	parse_mark(state);
	எனில் (!next_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, அளவு(b));
	எனில் (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	ப்ரதிஜ்ஞா(e);
	செருகு_mark(&marks, next_mark, e);
}

நிலையான எழுத்து* make_fast_import_path(கட்டமைப்பு fast_import_state *state,
				   மாறிலி எழுத்து *path)
{
	எனில் (!relative_marks_paths || is_absolute_path(path))
		திரும்பு prefix_filename(state->prefix, path);
	திரும்பு repo_git_path(the_repository, "info/fast-import/%s", path);
}

நிலையான வெறுமை option_import_marks(கட்டமைப்பு fast_import_state *state,
				மாறிலி எழுத்து *marks,
				முழு from_stream,
				முழு ignore_missing)
{
	எனில் (import_marks_file) {
		எனில் (from_stream)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		எனில்(!import_marks_file_from_stream)
			read_marks();
	}

	விடுவி(import_marks_file);
	import_marks_file = make_fast_import_path(state, marks);
	import_marks_file_from_stream = from_stream;
	import_marks_file_ignore_missing = ignore_missing;
}

நிலையான வெறுமை option_date_format(மாறிலி எழுத்து *fmt)
{
	எனில் (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	இல்லையெனில் எனில் (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	இல்லையெனில் எனில் (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	இல்லையெனில் எனில் (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	இல்லையெனில்
		die(_("unknown --date-format argument %s"), fmt);
}

நிலையான unsigned long ulong_arg(மாறிலி எழுத்து *option, மாறிலி எழுத்து *arg)
{
	எழுத்து *endptr;
	unsigned long rv = strtoul(arg, &endptr, 0);
	எனில் (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	திரும்பு rv;
}

நிலையான வெறுமை option_depth(மாறிலி எழுத்து *depth)
{
	max_depth = ulong_arg("--depth", depth);
	எனில் (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

நிலையான வெறுமை option_active_branches(மாறிலி எழுத்து *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

நிலையான வெறுமை option_export_marks(கட்டமைப்பு fast_import_state *state,
				மாறிலி எழுத்து *marks)
{
	விடுவி(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

நிலையான வெறுமை option_cat_blob_fd(கட்டமைப்பு fast_import_state *state UNUSED,
			       மாறிலி எழுத்து *fd)
{
	unsigned long n = ulong_arg("--cat-blob-fd", fd);
	எனில் (n > (unsigned long) INT_MAX)
		die(_("--cat-blob-fd cannot exceed %d"), INT_MAX);
	cat_blob_fd = (முழு) n;
}

நிலையான வெறுமை option_export_pack_edges(கட்டமைப்பு fast_import_state *state,
				     மாறிலி எழுத்து *edges)
{
	எழுத்து *fn = prefix_filename(state->prefix, edges);
	எனில் (pack_edges)
		fclose(pack_edges);
	pack_edges = xfopen(fn, "a");
	விடுவி(fn);
}

நிலையான வெறுமை option_rewrite_submodules(கட்டமைப்பு fast_import_state *state,
				      மாறிலி எழுத்து *arg,
				      கட்டமைப்பு string_list *list)
{
	கட்டமைப்பு mark_set *ms;
	FILE *fp;
	எழுத்து *s = xstrdup(arg);
	எழுத்து *f = strchr(s, ':');
	எனில் (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = prefix_filename(state->prefix, f);
	fp = fopen(f, "r");
	எனில் (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, செருகு_oid_entry);
	fclose(fp);
	விடுவி(f);

	string_list_செருகு(list, s)->util = ms;

	விடுவி(s);
}

நிலையான வெறுமை option_max_pack_size(மாறிலி எழுத்து *arg)
{
	unsigned long v;

	எனில் (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	எனில் (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} இல்லையெனில் எனில் (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

நிலையான வெறுமை option_big_file_threshold(மாறிலி எழுத்து *arg)
{
	unsigned long v;

	எனில் (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

நிலையான வெறுமை option_signed_commits(மாறிலி எழுத்து *arg)
{
	எனில் (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

நிலையான வெறுமை option_signed_tags(மாறிலி எழுத்து *arg)
{
	எனில் (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

நிலையான வெறுமை option_quiet(வெறுமை)
{
	show_stats = 0;
	quiet = 1;
}

நிலையான முழு parse_one_option(கட்டமைப்பு fast_import_state *state, மாறிலி எழுத்து *option)
{
	எனில் (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_size(option);
	} இல்லையெனில் எனில் (skip_prefix(option, "big-file-threshold=", &option)) {
		option_big_file_threshold(option);
	} இல்லையெனில் எனில் (skip_prefix(option, "depth=", &option)) {
		option_depth(option);
	} இல்லையெனில் எனில் (skip_prefix(option, "active-branches=", &option)) {
		option_active_branches(option);
	} இல்லையெனில் எனில் (skip_prefix(option, "export-pack-edges=", &option)) {
		option_export_pack_edges(state, option);
	} இல்லையெனில் எனில் (skip_prefix(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} இல்லையெனில் எனில் (skip_prefix(option, "signed-tags=", &option)) {
		option_signed_tags(option);
	} இல்லையெனில் எனில் (!strcmp(option, "quiet")) {
		option_quiet();
	} இல்லையெனில் எனில் (!strcmp(option, "stats")) {
		show_stats = 1;
	} இல்லையெனில் எனில் (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} இல்லையெனில் {
		திரும்பு 0;
	}

	திரும்பு 1;
}

நிலையான வெறுமை check_unsafe_feature(கட்டமைப்பு fast_import_state *state,
				 மாறிலி எழுத்து *feature)
{
	எனில் (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

நிலையான முழு parse_one_feature(கட்டமைப்பு fast_import_state *state,
			     மாறிலி எழுத்து *feature)
{
	மாறிலி எழுத்து *arg;

	எனில் (skip_prefix(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} இல்லையெனில் எனில் (skip_prefix(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} இல்லையெனில் எனில் (skip_prefix(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} இல்லையெனில் எனில் (skip_prefix(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} இல்லையெனில் எனில் (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} இல்லையெனில் எனில் (skip_prefix(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_to);
	} இல்லையெனில் எனில் (skip_prefix(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} இல்லையெனில் எனில் (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} இல்லையெனில் எனில் (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} இல்லையெனில் எனில் (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} இல்லையெனில் எனில் (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} இல்லையெனில் எனில் (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} இல்லையெனில் எனில் (!strcmp(feature, "force")) {
		force_update = 1;
	} இல்லையெனில் எனில் (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} இல்லையெனில் {
		திரும்பு 0;
	}

	திரும்பு 1;
}

நிலையான வெறுமை parse_feature(கட்டமைப்பு fast_import_state *state, மாறிலி எழுத்து *feature)
{
	எனில் (state->seen_data_command)
		die(_("got feature command '%s' after data command"), feature);

	எனில் (parse_one_feature(state, feature))
		திரும்பு;

	die(_("this version of fast-import does not support feature %s."), feature);
}

நிலையான வெறுமை parse_option(கட்டமைப்பு fast_import_state *state, மாறிலி எழுத்து *option)
{
	எனில் (state->seen_data_command)
		die(_("got option command '%s' after data command"), option);

	எனில் (parse_one_option(state, option))
		திரும்பு;

	die(_("this version of fast-import does not support option: %s"), option);
}

நிலையான வெறுமை git_pack_config(வெறுமை)
{
	முழு indexversion_மதிப்பு;
	முழு limit;
	unsigned long packsizelimit_மதிப்பு;

	எனில் (!repo_config_get_ulong(the_repository, "pack.depth", &max_depth)) {
		எனில் (max_depth > MAX_DEPTH)
			max_depth = MAX_DEPTH;
	}
	எனில் (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_மதிப்பு)) {
		pack_idx_opts.version = indexversion_மதிப்பு;
		எனில் (pack_idx_opts.version > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.version);
	}
	எனில் (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_மதிப்பு))
		max_packsize = packsizelimit_மதிப்பு;

	எனில் (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &limit))
		unpack_limit = limit;
	இல்லையெனில் எனில் (!repo_config_get_int(the_repository, "transfer.unpacklimit", &limit))
		unpack_limit = limit;

	repo_config(the_repository, git_default_config, வெற்று);
}

நிலையான மாறிலி எழுத்து *மாறிலி fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	வெற்று
};

நிலையான வெறுமை parse_argv(கட்டமைப்பு fast_import_state *state)
{
	முழு argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	எனில் (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_data_command = 1;
	எனில் (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

நிலையான முழு option_parse_date_format(மாறிலி கட்டமைப்பு option *opt UNUSED,
				    மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	திரும்பு 0;
}

நிலையான முழு option_parse_export_pack_edges(மாறிலி கட்டமைப்பு option *opt,
					  மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->மதிப்பு, arg);
	திரும்பு 0;
}

நிலையான முழு option_parse_max_pack_size(மாறிலி கட்டமைப்பு option *opt UNUSED,
				      மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_size(arg);
	திரும்பு 0;
}

நிலையான முழு option_parse_big_file_threshold(மாறிலி கட்டமைப்பு option *opt UNUSED,
					   மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	திரும்பு 0;
}

நிலையான முழு option_parse_signed_commits(மாறிலி கட்டமைப்பு option *opt UNUSED,
				       மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	திரும்பு 0;
}

நிலையான முழு option_parse_signed_tags(மாறிலி கட்டமைப்பு option *opt UNUSED,
				    மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	திரும்பு 0;
}

நிலையான முழு option_parse_rewrite_submodules_from(மாறிலி கட்டமைப்பு option *opt,
						மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->மதிப்பு, arg, &sub_marks_from);
	திரும்பு 0;
}

நிலையான முழு option_parse_rewrite_submodules_to(மாறிலி கட்டமைப்பு option *opt,
					      மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->மதிப்பு, arg, &sub_marks_to);
	திரும்பு 0;
}

நிலையான முழு option_parse_cat_blob_fd(மாறிலி கட்டமைப்பு option *opt,
				    மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->மதிப்பு, arg);
	திரும்பு 0;
}

நிலையான முழு option_parse_import_marks(மாறிலி கட்டமைப்பு option *opt,
				     மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->மதிப்பு, arg, 0, 0);
	திரும்பு 0;
}

நிலையான முழு option_parse_import_marks_if_exists(மாறிலி கட்டமைப்பு option *opt,
					       மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->மதிப்பு, arg, 0, 1);
	திரும்பு 0;
}

நிலையான முழு option_parse_export_marks(மாறிலி கட்டமைப்பு option *opt,
				     மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->மதிப்பு, arg);
	திரும்பு 0;
}

நிலையான முழு option_parse_depth(மாறிலி கட்டமைப்பு option *opt UNUSED,
			      மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	திரும்பு 0;
}

நிலையான முழு option_parse_active_branches(மாறிலி கட்டமைப்பு option *opt UNUSED,
					மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	திரும்பு 0;
}

நிலையான முழு option_parse_quiet(மாறிலி கட்டமைப்பு option *opt UNUSED,
			      மாறிலி எழுத்து *arg UNUSED, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	திரும்பு 0;
}

முழு cmd_fast_import(முழு argc,
		    மாறிலி எழுத்து **argv,
		    மாறிலி எழுத்து *prefix,
		    கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு fast_import_state state;

	கட்டமைப்பு option fast_import_options[] = {
		OPT_GROUP(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", வெற்று, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", வெற்று, வெற்று,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", வெற்று, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_size),
		OPT_CALLBACK_F(0, "big-file-threshold", வெற்று, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_threshold),
		OPT_CALLBACK_F(0, "depth", வெற்று, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_depth),
		OPT_CALLBACK_F(0, "active-branches", வெற்று, N_("n"),
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
		OPT_CALLBACK_F(0, "signed-commits", வெற்று, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", வெற்று, N_("mode"),
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
	CALLOC_ARRAY(avail_மரம்_table, avail_மரம்_table_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, அளவு(கட்டமைப்பு mark_set));

	hashmap_init(&object_table, object_entry_hashcmp, வெற்று, 0);

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
	க்கு (முழு i = 1; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];
		எனில் (*arg != '-' || !strcmp(arg, "--"))
			நிறுத்து;
		எனில் (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * அளவு(*rc_free));
	க்கு (unsigned முழு i = 0; i < (cmd_save - 1); i++)
		rc_free[i].next = &rc_free[i + 1];
	rc_free[cmd_save - 1].next = வெற்று;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	போது (read_next_command(&state) != EOF) {
		மாறிலி எழுத்து *v;
		எனில் (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, வெற்று);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		இல்லையெனில் எனில் (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		இல்லையெனில் எனில் (!strcmp("done", command_buf.buf))
			நிறுத்து;
		இல்லையெனில் எனில் (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		இல்லையெனில் எனில் (starts_with(command_buf.buf, "progress "))
			parse_progress(&state);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		இல்லையெனில் எனில் (skip_prefix(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		இல்லையெனில் எனில் (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		இல்லையெனில்
			die(_("unsupported command: %s"), command_buf.buf);

		எனில் (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	எனில் (!state.seen_data_command)
		parse_argv(&state);

	எனில் (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	end_packfile();

	dump_branches();
	dump_tags();
	unkeep_all_packs();
	dump_marks();

	எனில் (pack_edges)
		fclose(pack_edges);

	எனில் (show_stats) {
		uintmax_t total_மொத்தம் = 0, duplicate_மொத்தம் = 0;
		க்கு (size_t i = 0; i < ARRAY_SIZE(object_மொத்தம்_by_type); i++)
			total_மொத்தம் += object_மொத்தம்_by_type[i];
		க்கு (size_t i = 0; i < ARRAY_SIZE(duplicate_மொத்தம்_by_type); i++)
			duplicate_மொத்தம் += duplicate_மொத்தம்_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_மொத்தம்);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", total_மொத்தம், duplicate_மொத்தம்);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_மொத்தம்_by_type[OBJ_BLOB], duplicate_மொத்தம்_by_type[OBJ_BLOB], delta_மொத்தம்_by_type[OBJ_BLOB], delta_மொத்தம்_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_மொத்தம்_by_type[OBJ_மரம்], duplicate_மொத்தம்_by_type[OBJ_மரம்], delta_மொத்தம்_by_type[OBJ_மரம்], delta_மொத்தம்_attempts_by_type[OBJ_மரம்]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_மொத்தம்_by_type[OBJ_COMMIT], duplicate_மொத்தம்_by_type[OBJ_COMMIT], delta_மொத்தம்_by_type[OBJ_COMMIT], delta_மொத்தம்_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_மொத்தம்_by_type[OBJ_TAG], duplicate_மொத்தம்_by_type[OBJ_TAG], delta_மொத்தம்_by_type[OBJ_TAG], delta_மொத்தம்_attempts_by_type[OBJ_TAG]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_மொத்தம், branch_load_மொத்தம்);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->shift) * 1024, marks_set_மொத்தம்);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (மரம்_entry_allocd + fi_mem_pool.pool_alloc + alloc_மொத்தம்*அளவு(கட்டமைப்பு object_entry))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (unsigned long)((மரம்_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_மொத்தம்*அளவு(கட்டமைப்பு object_entry))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	திரும்பு failure ? 1 : 0;
}
