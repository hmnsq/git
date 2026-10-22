#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

ساختار object_entry {
	ساختار pack_idx_entry idx;
	ساختار hashmap_entry ent;
	uint32_t type : TYPE_BITS,
		pack_id : PACK_ID_BITS,
		depth : DEPTH_BITS;
};

ایستا صحیح object_entry_hashcmp(ثابت تهی *map_data UNUSED,
				ثابت ساختار hashmap_entry *eptr,
				ثابت ساختار hashmap_entry *entry_or_کلید,
				ثابت تهی *keydata)
{
	ثابت ساختار object_id *oid = keydata;
	ثابت ساختار object_entry *e1, *e2;

	e1 = container_of(eptr, ثابت ساختار object_entry, ent);
	اگر (oid)
		بازگشت oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_کلید, ثابت ساختار object_entry, ent);
	بازگشت oidcmp(&e1->idx.oid, &e2->idx.oid);
}

ساختار object_entry_pool {
	ساختار object_entry_pool *next_pool;
	ساختار object_entry *next_free;
	ساختار object_entry *end;
	ساختار object_entry entries[FLEX_ARRAY]; /* more */
};

ساختار mark_set {
	اجتماع {
		ساختار object_id *oids[1024];
		ساختار object_entry *marked[1024];
		ساختار mark_set *sets[1024];
	} data;
	unsigned صحیح shift;
};

ساختار last_object {
	ساختار strbuf data;
	off_t offset;
	unsigned صحیح depth;
	unsigned no_swap : 1;
};

ساختار atom_str {
	ساختار atom_str *next_atom;
	unsigned short str_len;
	نویسه str_dat[FLEX_ARRAY]; /* more */
};

ساختار درخت_content;
ساختار درخت_entry {
	ساختار درخت_content *درخت;
	ساختار atom_str *name;
	ساختار درخت_entry_ms {
		uint16_t mode;
		ساختار object_id oid;
	} versions[2];
};

ساختار درخت_content {
	unsigned صحیح entry_capacity; /* must match avail_tree_content */
	unsigned صحیح entry_شمار;
	unsigned صحیح delta_depth;
	ساختار درخت_entry *entries[FLEX_ARRAY]; /* more */
};

ساختار avail_درخت_content {
	unsigned صحیح entry_capacity; /* must match tree_content */
	ساختار avail_درخت_content *next_avail;
};

ساختار branch {
	ساختار branch *table_next_branch;
	ساختار branch *active_next_branch;
	ثابت نویسه *name;
	ساختار درخت_entry branch_درخت;
	uintmax_t last_commit;
	uintmax_t رقم_notes;
	unsigned active : 1;
	unsigned delete : 1;
	unsigned pack_id : PACK_ID_BITS;
	ساختار object_id oid;
};

ساختار tag {
	ساختار tag *next_tag;
	ثابت نویسه *name;
	unsigned صحیح pack_id;
	ساختار object_id oid;
};

ساختار hash_list {
	ساختار hash_list *next;
	ساختار object_id oid;
};

تعریفنوع شمارش {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

ساختار recent_command {
	ساختار recent_command *prev;
	ساختار recent_command *next;
	نویسه *buf;
};

تعریفنوع تهی (*mark_set_inserter_t)(ساختار mark_set **s, ساختار object_id *oid, uintmax_t mark);
تعریفنوع تهی (*each_mark_fn_t)(uintmax_t mark, تهی *obj, تهی *cbp);

/* Configured limits on output */
ایستا unsigned long max_depth = 50;
ایستا off_t max_packsize;
ایستا صحیح unpack_limit = 100;
ایستا صحیح force_update;

/* Stats and misc. counters */
ایستا uintmax_t alloc_شمار;
ایستا uintmax_t marks_set_شمار;
ایستا uintmax_t object_شمار_by_type[1 << TYPE_BITS];
ایستا uintmax_t duplicate_شمار_by_type[1 << TYPE_BITS];
ایستا uintmax_t delta_شمار_by_type[1 << TYPE_BITS];
ایستا uintmax_t delta_شمار_attempts_by_type[1 << TYPE_BITS];
ایستا unsigned long object_شمار;
ایستا unsigned long branch_شمار;
ایستا unsigned long branch_load_شمار;
ایستا صحیح failure;
ایستا FILE *pack_edges;
ایستا صحیح show_stats = 1;
ایستا صحیح quiet;
ایستا شمارش sign_mode signed_tag_mode = SIGN_VERBATIM;
ایستا شمارش sign_mode signed_commit_mode = SIGN_VERBATIM;
ایستا ثابت نویسه *signed_commit_keyid;
ایستا ثابت نویسه *signed_tag_keyid;

/* Memory pools */
ایستا ساختار mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - اندازه(ساختار mp_block),
};

/* Atom management */
ایستا unsigned صحیح atom_table_sz = 4451;
ایستا unsigned صحیح atom_cnt;
ایستا ساختار atom_str **atom_table;

/* The .pack file being generated */
ایستا ساختار pack_idx_option pack_idx_opts;
ایستا unsigned صحیح pack_id;
ایستا ساختار hashfile *pack_file;
ایستا ساختار packed_git *pack_data;
ایستا ساختار packed_git **all_packs;
ایستا off_t pack_size;

/* Table of objects we've written. */
ایستا unsigned صحیح object_entry_alloc = 5000;
ایستا ساختار object_entry_pool *blocks;
ایستا ساختار hashmap object_table;
ایستا ساختار mark_set *marks;
ایستا نویسه *export_marks_file;
ایستا نویسه *import_marks_file;
ایستا صحیح import_marks_file_from_stream;
ایستا صحیح import_marks_file_ignore_missing;
ایستا صحیح import_marks_file_done;
ایستا صحیح relative_marks_paths;

/* Our last blob */
ایستا ساختار last_object last_blob = {
	.data = STRBUF_INIT,
 };

/* Tree management */
ایستا unsigned صحیح درخت_entry_alloc = 1000;
ایستا تهی *avail_درخت_entry;
ایستا unsigned صحیح avail_درخت_table_sz = 100;
ایستا ساختار avail_درخت_content **avail_درخت_table;
ایستا size_t درخت_entry_allocd;
ایستا ساختار strbuf old_درخت = STRBUF_INIT;
ایستا ساختار strbuf new_درخت = STRBUF_INIT;

/* Branch data */
ایستا unsigned long max_active_branches = 5;
ایستا unsigned long cur_active_branches;
ایستا unsigned long branch_table_sz = 1039;
ایستا ساختار branch **branch_table;
ایستا ساختار branch *active_branches;

/* Tag data */
ایستا ساختار tag *first_tag;
ایستا ساختار tag *last_tag;

/* Input stream parsing */
ایستا whenspec_type whenspec = WHENSPEC_RAW;
ایستا ساختار strbuf command_buf = STRBUF_INIT;
ایستا صحیح unread_command_buf;
ایستا ساختار recent_command cmd_hist = {
	.prev = &cmd_hist,
	.next = &cmd_hist,
};
ایستا ساختار recent_command *cmd_tail = &cmd_hist;
ایستا ساختار recent_command *rc_free;
ایستا unsigned صحیح cmd_save = 100;
ایستا uintmax_t next_mark;
ایستا ساختار strbuf new_data = STRBUF_INIT;
ایستا صحیح require_explicit_termination;

/* Signal handling */
ایستا فرار sig_atomic_t checkpoint_requested;

/* Submodule marks */
ایستا ساختار string_list sub_marks_from = STRING_LIST_INIT_DUP;
ایستا ساختار string_list sub_marks_to = STRING_LIST_INIT_DUP;
ایستا kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
ایستا صحیح cat_blob_fd = STDOUT_FILENO;

/* Command state */
ساختار fast_import_state {
	صحیح argc;
	ثابت نویسه **argv;
	ثابت نویسه *prefix;
	صحیح seen_data_command;
	صحیح allow_unsafe_features;
	ساختار option *option;
};

ایستا تهی fast_import_state_init(ساختار fast_import_state *state,
				   صحیح argc, ثابت نویسه **argv,
				   ثابت نویسه *prefix, ساختار option *option)
{
	memset(state, 0, اندازه(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

ایستا تهی parse_argv(ساختار fast_import_state *state);
ایستا تهی parse_get_mark(ساختار fast_import_state *state, ثابت نویسه *p);
ایستا تهی parse_cat_blob(ساختار fast_import_state *state, ثابت نویسه *p);
ایستا تهی parse_ls(ساختار fast_import_state *state,
		     ثابت نویسه *p,
		     ساختار branch *b);

ایستا تهی for_each_mark(ساختار mark_set *m, uintmax_t base, each_mark_fn_t callback, تهی *p)
{
	uintmax_t k;
	اگر (m->shift) {
		برای (k = 0; k < 1024; k++) {
			اگر (m->data.sets[k])
				for_each_mark(m->data.sets[k], base + (k << m->shift), callback, p);
		}
	} وگرنه {
		برای (k = 0; k < 1024; k++) {
			اگر (m->data.marked[k])
				callback(base + k, m->data.marked[k], p);
		}
	}
}

ایستا تهی dump_marks_fn(uintmax_t mark, تهی *object, تهی *cbp) {
	ساختار object_entry *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

ایستا تهی write_branch_report(FILE *rpt, ساختار branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	اگر (b->active)
		fputs(" active", rpt);
	اگر (b->branch_درخت.درخت)
		fputs(" loaded", rpt);
	اگر (is_null_oid(&b->branch_درخت.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_to_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_to_hex(&b->branch_درخت.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_to_hex(&b->branch_درخت.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->last_commit);

	fputs("  last pack   : ", rpt);
	اگر (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

ایستا تهی write_crash_report(ثابت نویسه *err)
{
	نویسه *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	ساختار branch *b;
	unsigned long lu;
	ساختار recent_command *rc;

	اگر (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		آزاد(loc);
		بازگشت;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(time(پوچ), 0, DATE_MODE(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	برای (rc = cmd_hist.next; rc != &cmd_hist; rc = rc->next) {
		اگر (rc->next == &cmd_hist)
			fputs("* ", rpt);
		وگرنه
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
	برای (b = active_branches, lu = 0; b; b = b->active_next_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	برای (lu = 0; lu < branch_table_sz; lu++) {
		برای (b = branch_table[lu]; b; b = b->table_next_branch)
			write_branch_report(rpt, b);
	}

	اگر (first_tag) {
		ساختار tag *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		برای (tg = first_tag; tg; tg = tg->next_tag) {
			fputs(oid_to_hex(&tg->oid), rpt);
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
	وگرنه
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	آزاد(loc);
}

ایستا تهی end_packfile(تهی);
ایستا تهی unkeep_all_packs(تهی);
ایستا تهی dump_marks(تهی);

ایستا NORETURN تهی die_nicely(ثابت نویسه *err, va_list params)
{
	va_list cp;
	ایستا صحیح zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	اگر (!zombie) {
		نویسه message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, اندازه(message), err, cp);
		write_crash_report(message);
		end_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	exit(128);
}

#ifndef SIGUSR1	/* Windows, for example */

ایستا تهی set_checkpoint_signal(تهی)
{
}

#وگرنه

ایستا تهی checkpoint_signal(صحیح signo UNUSED)
{
	checkpoint_requested = 1;
}

ایستا تهی set_checkpoint_signal(تهی)
{
	ساختار sigaction sa;

	memset(&sa, 0, اندازه(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, پوچ);
}

#endif

ایستا تهی alloc_objects(unsigned صحیح cnt)
{
	ساختار object_entry_pool *b;

	b = xmalloc(اندازه(ساختار object_entry_pool)
		+ cnt * اندازه(ساختار object_entry));
	b->next_pool = blocks;
	b->next_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_شمار += cnt;
}

ایستا ساختار object_entry *new_object(ساختار object_id *oid)
{
	ساختار object_entry *e;

	اگر (blocks->next_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->next_free++;
	oidcpy(&e->idx.oid, oid);
	بازگشت e;
}

ایستا ساختار object_entry *find_object(ساختار object_id *oid)
{
	بازگشت hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   ساختار object_entry, ent);
}

ایستا ساختار object_entry *درج_object(ساختار object_id *oid)
{
	ساختار object_entry *e;
	unsigned صحیح hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					ساختار object_entry, ent);
	اگر (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	بازگشت e;
}

ایستا تهی invalidate_pack_id(unsigned صحیح id)
{
	unsigned long lu;
	ساختار tag *t;
	ساختار hashmap_iter iter;
	ساختار object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		اگر (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	برای (lu = 0; lu < branch_table_sz; lu++) {
		ساختار branch *b;

		برای (b = branch_table[lu]; b; b = b->table_next_branch)
			اگر (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	برای (t = first_tag; t; t = t->next_tag)
		اگر (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

ایستا unsigned صحیح hc_str(ثابت نویسه *s, size_t len)
{
	unsigned صحیح r = 0;
	وقتی (len-- > 0)
		r = r * 31 + *s++;
	بازگشت r;
}

ایستا تهی درج_mark(ساختار mark_set **بالاترین, uintmax_t idnum, ساختار object_entry *oe)
{
	ساختار mark_set *s = *بالاترین;

	وقتی ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, اندازه(ساختار mark_set));
		s->shift = (*بالاترین)->shift + 10;
		s->data.sets[0] = *بالاترین;
		*بالاترین = s;
	}
	وقتی (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		اگر (!s->data.sets[i]) {
			s->data.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, اندازه(ساختار mark_set));
			s->data.sets[i]->shift = s->shift - 10;
		}
		s = s->data.sets[i];
	}
	اگر (!s->data.marked[idnum])
		marks_set_شمار++;
	s->data.marked[idnum] = oe;
}

ایستا تهی *find_mark(ساختار mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	ساختار object_entry *oe = پوچ;
	اگر ((idnum >> s->shift) < 1024) {
		وقتی (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->data.sets[i];
		}
		اگر (s)
			oe = s->data.marked[idnum];
	}
	اگر (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	بازگشت oe;
}

ایستا ساختار atom_str *to_atom(ثابت نویسه *s, unsigned short len)
{
	unsigned صحیح hc = hc_str(s, len) % atom_table_sz;
	ساختار atom_str *c;

	برای (c = atom_table[hc]; c; c = c->next_atom)
		اگر (c->str_len == len && !strncmp(s, c->str_dat, len))
			بازگشت c;

	c = mem_pool_alloc(&fi_mem_pool, اندازه(ساختار atom_str) + len + 1);
	c->str_len = len;
	memcpy(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->next_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	بازگشت c;
}

ایستا ساختار branch *lookup_branch(ثابت نویسه *name)
{
	unsigned صحیح hc = hc_str(name, strlen(name)) % branch_table_sz;
	ساختار branch *b;

	برای (b = branch_table[hc]; b; b = b->table_next_branch)
		اگر (!strcmp(name, b->name))
			بازگشت b;
	بازگشت پوچ;
}

ایستا ساختار branch *new_branch(ثابت نویسه *name)
{
	unsigned صحیح hc = hc_str(name, strlen(name)) % branch_table_sz;
	ساختار branch *b = lookup_branch(name);

	اگر (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	اگر (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, اندازه(ساختار branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_next_branch = branch_table[hc];
	b->branch_درخت.versions[0].mode = S_IFDIR;
	b->branch_درخت.versions[1].mode = S_IFDIR;
	b->رقم_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_شمار++;
	بازگشت b;
}

ایستا unsigned صحیح hc_entries(unsigned صحیح cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	بازگشت cnt < avail_درخت_table_sz ? cnt : avail_درخت_table_sz - 1;
}

ایستا ساختار درخت_content *new_درخت_content(unsigned صحیح cnt)
{
	ساختار avail_درخت_content *f, *l = پوچ;
	ساختار درخت_content *t;
	unsigned صحیح hc = hc_entries(cnt);

	برای (f = avail_درخت_table[hc]; f; l = f, f = f->next_avail)
		اگر (f->entry_capacity >= cnt)
			بشکن;

	اگر (f) {
		اگر (l)
			l->next_avail = f->next_avail;
		وگرنه
			avail_درخت_table[hc] = f->next_avail;
	} وگرنه {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, اندازه(*t) + اندازه(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (ساختار درخت_content*)f;
	t->entry_شمار = 0;
	t->delta_depth = 0;
	بازگشت t;
}

ایستا تهی release_درخت_entry(ساختار درخت_entry *e);
ایستا تهی release_درخت_content(ساختار درخت_content *t)
{
	ساختار avail_درخت_content *f = (ساختار avail_درخت_content*)t;
	unsigned صحیح hc = hc_entries(f->entry_capacity);
	f->next_avail = avail_درخت_table[hc];
	avail_درخت_table[hc] = f;
}

ایستا تهی release_درخت_content_recursive(ساختار درخت_content *t)
{
	unsigned صحیح i;
	برای (i = 0; i < t->entry_شمار; i++)
		release_درخت_entry(t->entries[i]);
	release_درخت_content(t);
}

ایستا ساختار درخت_content *grow_درخت_content(
	ساختار درخت_content *t,
	صحیح amt)
{
	ساختار درخت_content *r = new_درخت_content(t->entry_شمار + amt);
	r->entry_شمار = t->entry_شمار;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_شمار);
	release_درخت_content(t);
	بازگشت r;
}

ایستا ساختار درخت_entry *new_درخت_entry(تهی)
{
	ساختار درخت_entry *e;

	اگر (!avail_درخت_entry) {
		unsigned صحیح n = درخت_entry_alloc;
		درخت_entry_allocd += n * اندازه(ساختار درخت_entry);
		ALLOC_ARRAY(e, n);
		avail_درخت_entry = e;
		وقتی (n-- > 1) {
			*((تهی**)e) = e + 1;
			e++;
		}
		*((تهی**)e) = پوچ;
	}

	e = avail_درخت_entry;
	avail_درخت_entry = *((تهی**)e);
	بازگشت e;
}

ایستا تهی release_درخت_entry(ساختار درخت_entry *e)
{
	اگر (e->درخت)
		release_درخت_content_recursive(e->درخت);
	*((تهی**)e) = avail_درخت_entry;
	avail_درخت_entry = e;
}

ایستا ساختار درخت_content *dup_درخت_content(ساختار درخت_content *s)
{
	ساختار درخت_content *d;
	ساختار درخت_entry *a, *b;
	unsigned صحیح i;

	اگر (!s)
		بازگشت پوچ;
	d = new_درخت_content(s->entry_شمار);
	برای (i = 0; i < s->entry_شمار; i++) {
		a = s->entries[i];
		b = new_درخت_entry();
		memcpy(b, a, اندازه(*a));
		اگر (a->درخت && is_null_oid(&b->versions[1].oid))
			b->درخت = dup_درخت_content(a->درخت);
		وگرنه
			b->درخت = پوچ;
		d->entries[i] = b;
	}
	d->entry_شمار = s->entry_شمار;
	d->delta_depth = s->delta_depth;

	بازگشت d;
}

ایستا تهی start_packfile(تهی)
{
	ساختار strbuf tmp_file = STRBUF_INIT;
	ساختار packed_git *p;
	صحیح pack_fd;

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
	object_شمار = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

ایستا ثابت نویسه *create_index(تهی)
{
	ثابت نویسه *tmpfile;
	ساختار pack_idx_entry **idx, **c, **last;
	ساختار object_entry *e;
	ساختار object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_شمار);
	c = idx;
	برای (o = blocks; o; o = o->next_pool)
		برای (e = o->next_free; e-- != o->entries;)
			اگر (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_شمار;
	اگر (c != last)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, پوچ, idx, object_شمار,
				 &pack_idx_opts, pack_data->hash);
	آزاد(idx);
	بازگشت tmpfile;
}

ایستا نویسه *keep_pack(ثابت نویسه *curr_index_name)
{
	ایستا ثابت نویسه *keep_پیام = "fast-import";
	ساختار strbuf name = STRBUF_INIT;
	صحیح keep_fd;

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_data->repo,
							    name.buf);
	اگر (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_پیام, strlen(keep_پیام));
	اگر (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "pack");
	اگر (finalize_object_file(pack_data->repo, pack_data->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "idx");
	اگر (finalize_object_file(pack_data->repo, curr_index_name, name.buf))
		die(_("cannot store index file"));
	آزاد((تهی *)curr_index_name);
	بازگشت strbuf_detach(&name, پوچ);
}

ایستا تهی unkeep_all_packs(تهی)
{
	ساختار strbuf name = STRBUF_INIT;
	صحیح k;

	برای (k = 0; k < pack_id; k++) {
		ساختار packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

ایستا صحیح loosen_small_pack(ثابت ساختار packed_git *p)
{
	ساختار child_process unpack = CHILD_PROCESS_INIT;

	اگر (lseek(p->pack_fd, 0, SEEK_SET) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_افزودن(&unpack.args, "unpack-objects");
	اگر (!show_stats)
		strvec_افزودن(&unpack.args, "-q");

	بازگشت run_command(&unpack);
}

ایستا تهی end_packfile(تهی)
{
	ایستا صحیح running;

	اگر (running || !pack_data)
		بازگشت;

	running = 1;
	clear_delta_base_cache();
	اگر (object_شمار) {
		ساختار odb_source_files *files = odb_source_files_downcast(pack_data->repo->objects->source);
		ساختار packed_git *new_p;
		ساختار object_id cur_pack_oid;
		نویسه *idx_name;
		صحیح i;
		ساختار branch *b;
		ساختار tag *t;

		close_pack_windows(pack_data);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_data->pack_fd,
					 pack_data->hash, pack_data->pack_name,
					 object_شمار, cur_pack_oid.hash,
					 pack_size);

		اگر (object_شمار <= unpack_limit) {
			اگر (!loosen_small_pack(pack_data)) {
				invalidate_pack_id(pack_id);
				برو discard_pack;
			}
		}

		close(pack_data->pack_fd);
		idx_name = keep_pack(create_index());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		اگر (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		آزاد(idx_name);

		/* Print the boundary */
		اگر (pack_edges) {
			fprintf(pack_edges, "%s:", new_p->pack_name);
			برای (i = 0; i < branch_table_sz; i++) {
				برای (b = branch_table[i]; b; b = b->table_next_branch) {
					اگر (b->pack_id == pack_id)
						fprintf(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			برای (t = first_tag; t; t = t->next_tag) {
				اگر (t->pack_id == pack_id)
					fprintf(pack_edges, " %s",
						oid_to_hex(&t->oid));
			}
			fputc('\n', pack_edges);
			fflush(pack_edges);
		}

		pack_id++;
	}
	وگرنه {
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

ایستا تهی cycle_packfile(تهی)
{
	end_packfile();
	start_packfile();
}

ایستا صحیح store_object(
	شمارش object_type type,
	ساختار strbuf *dat,
	ساختار last_object *last,
	ساختار object_id *oidout,
	uintmax_t mark)
{
	ساختار odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	تهی *out, *delta;
	ساختار object_entry *e;
	unsigned نویسه hdr[96];
	ساختار object_id oid;
	unsigned long hdrlen, deltalen = 0;
	ساختار git_hash_ctx c;
	git_zstream s;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	hdrlen = format_object_header((نویسه *)hdr, اندازه(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	اگر (oidout)
		oidcpy(oidout, &oid);

	e = درج_object(&oid);
	اگر (mark)
		درج_mark(&marks, mark, e);
	اگر (e->idx.offset) {
		duplicate_شمار_by_type[type]++;
		بازگشت 1;
	}

	برای (ساختار odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		اگر (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			ادامه;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_شمار_by_type[type]++;
		بازگشت 1;
	}

	اگر (last && last->data.len && last->data.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		size_t deltalen_st;

		delta_شمار_attempts_by_type[type]++;
		delta = diff_delta(last->data.buf, last->data.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_size_t_to_ulong(deltalen_st);
	} وگرنه
		delta = پوچ;

	git_deflate_init(&s, cfg->pack_compression_level);
	اگر (delta) {
		s.next_in = delta;
		s.avail_in = deltalen;
	} وگرنه {
		s.next_in = (تهی *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.next_out = out = xmalloc(s.avail_out);
	وقتی (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	اگر ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + s.total_out) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + s.total_out) < pack_size) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		اگر (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.next_in = (تهی *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.next_out = out = xrealloc(out, s.avail_out);
			وقتی (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_end(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.offset = pack_size;
	object_شمار++;
	object_شمار_by_type[type]++;

	crc32_begin(pack_file);

	اگر (delta) {
		off_t ofs = e->idx.offset - last->offset;
		unsigned pos = اندازه(hdr) - 1;

		delta_شمار_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, اندازه(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;

		hdr[pos] = ofs & 127;
		وقتی (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, اندازه(hdr) - pos);
		pack_size += اندازه(hdr) - pos;
	} وگرنه {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, اندازه(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;
	}

	hashwrite(pack_file, out, s.total_out);
	pack_size += s.total_out;

	e->idx.crc32 = crc32_end(pack_file);

	آزاد(out);
	آزاد(delta);
	اگر (last) {
		اگر (last->no_swap) {
			last->data = *dat;
		} وگرنه {
			strbuf_swap(&last->data, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	بازگشت 0;
}

ایستا تهی truncate_pack(ساختار hashfile_checkpoint *checkpoint)
{
	اگر (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_size = checkpoint->offset;
}

ایستا تهی stream_blob(uintmax_t len, ساختار object_id *oidout, uintmax_t mark)
{
	ساختار odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	size_t in_sz = 64 * 1024, out_sz = 64 * 1024;
	unsigned نویسه *in_buf = xmalloc(in_sz);
	unsigned نویسه *out_buf = xmalloc(out_sz);
	ساختار object_entry *e;
	ساختار object_id oid;
	unsigned long hdrlen;
	off_t offset;
	ساختار git_hash_ctx c;
	git_zstream s;
	ساختار hashfile_checkpoint checkpoint;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	صحیح status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	اگر ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + len) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + len) < pack_size)
		cycle_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	offset = checkpoint.offset;

	hdrlen = format_object_header((نویسه *)out_buf, out_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, out_buf, hdrlen);

	crc32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_in_pack_object_header(out_buf, out_sz, OBJ_BLOB, len);

	s.next_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	وقتی (status != Z_STREAM_END) {
		اگر (0 < len && !s.avail_in) {
			size_t cnt = in_sz < len ? in_sz : (size_t)len;
			size_t n = fread(in_buf, 1, cnt, stdin);
			اگر (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.next_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		اگر (!s.avail_out || status == Z_STREAM_END) {
			size_t n = s.next_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_size += n;
			s.next_out = out_buf;
			s.avail_out = out_sz;
		}

		انتخاب (status) {
		حالت Z_OK:
		حالت Z_BUF_ERROR:
		حالت Z_STREAM_END:
			ادامه;
		پیشفرض:
			die(_("unexpected deflate failure: %d"), status);
		}
	}
	git_deflate_end(&s);
	git_hash_final_oid(&oid, &c);

	اگر (oidout)
		oidcpy(oidout, &oid);

	e = درج_object(&oid);

	اگر (mark)
		درج_mark(&marks, mark, e);

	اگر (e->idx.offset) {
		duplicate_شمار_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		برو out;
	}

	برای (ساختار odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		اگر (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			ادامه;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_شمار_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		برو out;
	}

	e->depth = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.offset = offset;
	e->idx.crc32 = crc32_end(pack_file);
	object_شمار++;
	object_شمار_by_type[OBJ_BLOB]++;

out:
	آزاد(in_buf);
	آزاد(out_buf);
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
ایستا تهی *gfi_unpack_entry(
	ساختار object_entry *oe,
	unsigned long *sizep)
{
	شمارش object_type type;
	size_t size_st = 0;
	تهی *data;
	ساختار packed_git *p = all_packs[oe->pack_id];
	اگر (p == pack_data && p->pack_size < (pack_size + the_hash_algo->rawsz)) {
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
	اگر (sizep)
		*sizep = cast_size_t_to_ulong(size_st);
	بازگشت data;
}

ایستا تهی load_درخت(ساختار درخت_entry *ریشه)
{
	ساختار object_id *oid = &ریشه->versions[1].oid;
	ساختار object_entry *myoe;
	ساختار درخت_content *t;
	unsigned long size;
	نویسه *buf;
	ثابت نویسه *c;

	ریشه->درخت = t = new_درخت_content(8);
	اگر (is_null_oid(oid))
		بازگشت;

	myoe = find_object(oid);
	اگر (myoe && myoe->pack_id != MAX_PACK_ID) {
		اگر (myoe->type != OBJ_درخت)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &size);
		اگر (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} وگرنه {
		شمارش object_type type;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		اگر (!buf || type != OBJ_درخت)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	وقتی (c != (buf + size)) {
		ساختار درخت_entry *e = new_درخت_entry();

		اگر (t->entry_شمار == t->entry_capacity)
			ریشه->درخت = t = grow_درخت_content(t, t->entry_شمار);
		t->entries[t->entry_شمار++] = e;

		e->درخت = پوچ;
		c = parse_mode(c, &e->versions[1].mode);
		اگر (!c)
			die(_("corrupt mode in %s"), oid_to_hex(oid));
		e->versions[0].mode = e->versions[1].mode;
		e->name = to_atom(c, strlen(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (unsigned نویسه *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (unsigned نویسه *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	آزاد(buf);
}

ایستا صحیح tecmp0 (ثابت تهی *_a, ثابت تهی *_b)
{
	ساختار درخت_entry *a = *((ساختار درخت_entry**)_a);
	ساختار درخت_entry *b = *((ساختار درخت_entry**)_b);
	بازگشت base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

ایستا صحیح tecmp1 (ثابت تهی *_a, ثابت تهی *_b)
{
	ساختار درخت_entry *a = *((ساختار درخت_entry**)_a);
	ساختار درخت_entry *b = *((ساختار درخت_entry**)_b);
	بازگشت base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

ایستا تهی mktree(ساختار درخت_content *t, صحیح v, ساختار strbuf *b)
{
	size_t maxlen = 0;
	unsigned صحیح i;

	اگر (!v)
		QSORT(t->entries, t->entry_شمار, tecmp0);
	وگرنه
		QSORT(t->entries, t->entry_شمار, tecmp1);

	برای (i = 0; i < t->entry_شمار; i++) {
		اگر (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	برای (i = 0; i < t->entry_شمار; i++) {
		ساختار درخت_entry *e = t->entries[i];
		اگر (!e->versions[v].mode)
			ادامه;
		strbuf_addf(b, "%o %s%c",
			(unsigned صحیح)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

ایستا تهی store_درخت(ساختار درخت_entry *ریشه)
{
	ساختار درخت_content *t;
	unsigned صحیح i, j, del;
	ساختار last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	ساختار object_entry *le = پوچ;

	اگر (!is_null_oid(&ریشه->versions[1].oid))
		بازگشت;

	اگر (!ریشه->درخت)
		load_درخت(ریشه);
	t = ریشه->درخت;

	برای (i = 0; i < t->entry_شمار; i++) {
		اگر (t->entries[i]->درخت)
			store_درخت(t->entries[i]);
	}

	اگر (!(ریشه->versions[0].mode & NO_DELTA))
		le = find_object(&ریشه->versions[0].oid);
	اگر (S_ISDIR(ریشه->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_درخت);
		lo.data = old_درخت;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_درخت);
	store_object(OBJ_درخت, &new_درخت, &lo, &ریشه->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	برای (i = 0, j = 0, del = 0; i < t->entry_شمار; i++) {
		ساختار درخت_entry *e = t->entries[i];
		اگر (e->versions[1].mode) {
			e->versions[0].mode = e->versions[1].mode;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} وگرنه {
			release_درخت_entry(e);
			del++;
		}
	}
	t->entry_شمار -= del;
}

ایستا تهی درخت_content_replace(
	ساختار درخت_entry *ریشه,
	ثابت ساختار object_id *oid,
	ثابت uint16_t mode,
	ساختار درخت_content *newtree)
{
	اگر (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&ریشه->versions[0].oid, the_repository->hash_algo);
	oidcpy(&ریشه->versions[1].oid, oid);
	اگر (ریشه->درخت)
		release_درخت_content_recursive(ریشه->درخت);
	ریشه->درخت = newtree;
}

ایستا صحیح درخت_content_set(
	ساختار درخت_entry *ریشه,
	ثابت نویسه *p,
	ثابت ساختار object_id *oid,
	ثابت uint16_t mode,
	ساختار درخت_content *subtree)
{
	ساختار درخت_content *t;
	ثابت نویسه *slash1;
	unsigned صحیح i, n;
	ساختار درخت_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	اگر (!n)
		die(_("empty path component found in input"));
	اگر (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	اگر (!ریشه->درخت)
		load_درخت(ریشه);
	t = ریشه->درخت;
	برای (i = 0; i < t->entry_شمار; i++) {
		e = t->entries[i];
		اگر (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			اگر (!*slash1) {
				اگر (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					بازگشت 0;
				e->versions[1].mode = mode;
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
				اگر (S_ISDIR(e->versions[0].mode))
					e->versions[0].mode |= NO_DELTA;

				oidclr(&ریشه->versions[1].oid, the_repository->hash_algo);
				بازگشت 1;
			}
			اگر (!S_ISDIR(e->versions[1].mode)) {
				e->درخت = new_درخت_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			اگر (!e->درخت)
				load_درخت(e);
			اگر (درخت_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&ریشه->versions[1].oid, the_repository->hash_algo);
				بازگشت 1;
			}
			بازگشت 0;
		}
	}

	اگر (t->entry_شمار == t->entry_capacity)
		ریشه->درخت = t = grow_درخت_content(t, t->entry_شمار);
	e = new_درخت_entry();
	e->name = to_atom(p, n);
	e->versions[0].mode = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->entry_شمار++] = e;
	اگر (*slash1) {
		e->درخت = new_درخت_content(8);
		e->versions[1].mode = S_IFDIR;
		درخت_content_set(e, slash1 + 1, oid, mode, subtree);
	} وگرنه {
		e->درخت = subtree;
		e->versions[1].mode = mode;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&ریشه->versions[1].oid, the_repository->hash_algo);
	بازگشت 1;
}

ایستا صحیح درخت_content_remove(
	ساختار درخت_entry *ریشه,
	ثابت نویسه *p,
	ساختار درخت_entry *backup_leaf,
	صحیح allow_ریشه)
{
	ساختار درخت_content *t;
	ثابت نویسه *slash1;
	unsigned صحیح i, n;
	ساختار درخت_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	اگر (!ریشه->درخت)
		load_درخت(ریشه);

	اگر (!*p && allow_ریشه) {
		e = ریشه;
		برو del_entry;
	}

	t = ریشه->درخت;
	برای (i = 0; i < t->entry_شمار; i++) {
		e = t->entries[i];
		اگر (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			اگر (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				بازگشت 1;
			اگر (!*slash1 || !S_ISDIR(e->versions[1].mode))
				برو del_entry;
			اگر (!e->درخت)
				load_درخت(e);
			اگر (درخت_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				برای (n = 0; n < e->درخت->entry_شمار; n++) {
					اگر (e->درخت->entries[n]->versions[1].mode) {
						oidclr(&ریشه->versions[1].oid,
						       the_repository->hash_algo);
						بازگشت 1;
					}
				}
				backup_leaf = پوچ;
				برو del_entry;
			}
			بازگشت 0;
		}
	}
	بازگشت 0;

del_entry:
	اگر (backup_leaf)
		memcpy(backup_leaf, e, اندازه(*backup_leaf));
	وگرنه اگر (e->درخت)
		release_درخت_content_recursive(e->درخت);
	e->درخت = پوچ;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&ریشه->versions[1].oid, the_repository->hash_algo);
	بازگشت 1;
}

ایستا صحیح درخت_content_get(
	ساختار درخت_entry *ریشه,
	ثابت نویسه *p,
	ساختار درخت_entry *leaf,
	صحیح allow_ریشه)
{
	ساختار درخت_content *t;
	ثابت نویسه *slash1;
	unsigned صحیح i, n;
	ساختار درخت_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	اگر (!n && !allow_ریشه)
		die(_("empty path component found in input"));

	اگر (!ریشه->درخت)
		load_درخت(ریشه);

	اگر (!n) {
		e = ریشه;
		برو found_entry;
	}

	t = ریشه->درخت;
	برای (i = 0; i < t->entry_شمار; i++) {
		e = t->entries[i];
		اگر (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			اگر (!*slash1)
				برو found_entry;
			اگر (!S_ISDIR(e->versions[1].mode))
				بازگشت 0;
			اگر (!e->درخت)
				load_درخت(e);
			بازگشت درخت_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	بازگشت 0;

found_entry:
	memcpy(leaf, e, اندازه(*leaf));
	اگر (e->درخت && is_null_oid(&e->versions[1].oid))
		leaf->درخت = dup_درخت_content(e->درخت);
	وگرنه
		leaf->درخت = پوچ;
	بازگشت 1;
}

ایستا صحیح update_branch(ساختار branch *b)
{
	ایستا ثابت نویسه *پیام = "fast-import";
	ساختار ref_transaction *transaction;
	ساختار object_id old_oid;
	ساختار strbuf err = STRBUF_INIT;
	ایستا ثابت نویسه *replace_prefix = "refs/replace/";

	اگر (starts_with(b->name, replace_prefix) &&
	    !strcmp(b->name + strlen(replace_prefix),
		    oid_to_hex(&b->oid))) {
		اگر (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				پوچ, b->name, پوچ, 0);
		بازگشت 0;
	}
	اگر (is_null_oid(&b->oid)) {
		اگر (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					پوچ, b->name, پوچ, 0);
		بازگشت 0;
	}
	اگر (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	اگر (!force_update && !is_null_oid(&old_oid)) {
		ساختار commit *old_cmit, *new_cmit;
		صحیح ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		اگر (!old_cmit || !new_cmit)
			بازگشت error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		اگر (ret < 0)
			exit(128);
		اگر (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			بازگشت -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	اگر (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   پوچ, پوچ, 0, پیام, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		بازگشت -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	بازگشت 0;
}

ایستا تهی dump_branches(تهی)
{
	unsigned صحیح i;
	ساختار branch *b;

	برای (i = 0; i < branch_table_sz; i++) {
		برای (b = branch_table[i]; b; b = b->table_next_branch)
			failure |= update_branch(b);
	}
}

ایستا تهی dump_tags(تهی)
{
	ایستا ثابت نویسه *پیام = "fast-import";
	ساختار tag *t;
	ساختار strbuf ref_name = STRBUF_INIT;
	ساختار strbuf err = STRBUF_INIT;
	ساختار ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	اگر (!transaction) {
		failure |= error("%s", err.buf);
		برو cleanup;
	}
	برای (t = first_tag; t; t = t->next_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		اگر (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, پوچ, پوچ, پوچ,
					   0, پیام, &err)) {
			failure |= error("%s", err.buf);
			برو cleanup;
		}
	}
	اگر (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

ایستا تهی dump_marks(تهی)
{
	ساختار lock_file mark_lock = LOCK_INIT;
	FILE *f;

	اگر (!export_marks_file || (import_marks_file && !import_marks_file_done))
		بازگشت;

	اگر (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		بازگشت;
	}

	اگر (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		بازگشت;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	اگر (!f) {
		صحیح saved_errno = errno;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		بازگشت;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	اگر (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		بازگشت;
	}
}

ایستا تهی درج_object_entry(ساختار mark_set **s, ساختار object_id *oid, uintmax_t mark)
{
	ساختار object_entry *e;
	e = find_object(oid);
	اگر (!e) {
		شمارش object_type type = odb_read_object_info(the_repository->objects,
							     oid, پوچ);
		اگر (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = درج_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	درج_mark(s, mark, e);
}

ایستا تهی درج_oid_entry(ساختار mark_set **s, ساختار object_id *oid, uintmax_t mark)
{
	درج_mark(s, mark, xmemdupz(oid, اندازه(*oid)));
}

ایستا تهی read_mark_file(ساختار mark_set **s, FILE *f, mark_set_inserter_t inserter)
{
	نویسه سطر[512];
	وقتی (fgets(سطر, اندازه(سطر), f)) {
		uintmax_t mark;
		نویسه *end;
		ساختار object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, اندازه(oid.hash));

		end = strchr(سطر, '\n');
		اگر (سطر[0] != ':' || !end)
			die(_("corrupt mark line: %s"), سطر);
		*end = 0;
		mark = strtoumax(سطر + 1, &end, 10);
		اگر (!mark || end == سطر + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), سطر);
		inserter(s, &oid, mark);
	}
}

ایستا تهی read_marks(تهی)
{
	FILE *f = fopen(import_marks_file, "r");
	اگر (f)
		;
	وگرنه اگر (import_marks_file_ignore_missing && errno == ENOENT)
		برو done; /* Marks file does not exist */
	وگرنه
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, درج_object_entry);
	fclose(f);
done:
	import_marks_file_done = 1;
}


ایستا صحیح read_next_command(ساختار fast_import_state *state)
{
	ایستا صحیح stdin_eof = 0;

	اگر (stdin_eof) {
		unread_command_buf = 0;
		بازگشت EOF;
	}

	برای (;;) {
		اگر (unread_command_buf) {
			unread_command_buf = 0;
		} وگرنه {
			ساختار recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			اگر (stdin_eof)
				بازگشت EOF;

			اگر (!state->seen_data_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			اگر (rc)
				rc_free = rc->next;
			وگرنه {
				rc = cmd_hist.next;
				cmd_hist.next = rc->next;
				cmd_hist.next->prev = &cmd_hist;
				آزاد(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_tail;
			rc->next = cmd_hist.prev;
			rc->prev->next = rc;
			cmd_tail = rc;
		}
		اگر (command_buf.buf[0] == '#')
			ادامه;
		بازگشت 0;
	}
}

ایستا تهی skip_optional_lf(تهی)
{
	صحیح term_char = fgetc(stdin);
	اگر (term_char != '\n' && term_char != EOF)
		ungetc(term_char, stdin);
}

ایستا تهی parse_mark(ساختار fast_import_state *state)
{
	ثابت نویسه *v;
	اگر (skip_prefix(command_buf.buf, "mark :", &v)) {
		next_mark = strtoumax(v, پوچ, 10);
		read_next_command(state);
	}
	وگرنه
		next_mark = 0;
}

ایستا تهی parse_original_identifier(ساختار fast_import_state *state)
{
	ثابت نویسه *v;
	اگر (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_next_command(state);
}

ایستا صحیح parse_data(ساختار strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	ثابت نویسه *data;
	strbuf_reset(sb);

	اگر (!skip_prefix(command_buf.buf, "data ", &data))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	اگر (skip_prefix(data, "<<", &data)) {
		نویسه *term = xstrdup(data);
		size_t term_len = command_buf.len - (data - command_buf.buf);

		برای (;;) {
			اگر (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), term);
			اگر (term_len == command_buf.len
				&& !strcmp(term, command_buf.buf))
				بشکن;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		آزاد(term);
	}
	وگرنه {
		uintmax_t len = strtoumax(data, پوچ, 10);
		size_t n = 0, length = (size_t)len;

		اگر (limit && limit < len) {
			*len_res = len;
			بازگشت 0;
		}
		اگر (length < len)
			die(_("data is too large to use in this context"));

		وقتی (n < length) {
			size_t s = strbuf_fread(sb, length - n, stdin);
			اگر (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(unsigned long)(length - n));
			n += s;
		}
	}

	skip_optional_lf();
	بازگشت 1;
}

ایستا صحیح validate_raw_date(ثابت نویسه *src, ساختار strbuf *result, صحیح strict)
{
	ثابت نویسه *orig_src = src;
	نویسه *endp;
	unsigned long رقم;

	errno = 0;

	رقم = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	اگر (errno || endp == src || *endp != ' ')
		بازگشت -1;

	src = endp + 1;
	اگر (*src != '-' && *src != '+')
		بازگشت -1;

	رقم = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	اگر (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < رقم))             /* parsed a broken timezone */
	   )
		بازگشت -1;

	strbuf_addstr(result, orig_src);
	بازگشت 0;
}

ایستا نویسه *parse_ident(ثابت نویسه *buf)
{
	ثابت نویسه *ltgt;
	size_t name_len;
	ساختار strbuf ident = STRBUF_INIT;

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
	strbuf_add(&ident, buf, name_len);

	انتخاب (whenspec) {
	حالت WHENSPEC_RAW:
		اگر (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		بشکن;
	حالت WHENSPEC_RAW_PERMISSIVE:
		اگر (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		بشکن;
	حالت WHENSPEC_RFC2822:
		اگر (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		بشکن;
	حالت WHENSPEC_NOW:
		اگر (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		بشکن;
	}

	بازگشت strbuf_detach(&ident, پوچ);
}

ایستا تهی parse_and_store_blob(
	ساختار last_object *last,
	ساختار object_id *oidout,
	uintmax_t mark)
{
	ایستا ساختار strbuf buf = STRBUF_INIT;
	uintmax_t len;

	اگر (parse_data(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	وگرنه {
		اگر (last) {
			strbuf_release(&last->data);
			last->offset = 0;
			last->depth = 0;
		}
		stream_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

ایستا تهی parse_new_blob(ساختار fast_import_state *state)
{
	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&last_blob, پوچ, next_mark);
}

ایستا تهی unload_one_branch(تهی)
{
	وقتی (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = ULONG_MAX;
		ساختار branch *e, *l = پوچ, *p = پوچ;

		برای (e = active_branches; e; e = e->active_next_branch) {
			اگر (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		اگر (p) {
			e = p->active_next_branch;
			p->active_next_branch = e->active_next_branch;
		} وگرنه {
			e = active_branches;
			active_branches = e->active_next_branch;
		}
		e->active = 0;
		e->active_next_branch = پوچ;
		اگر (e->branch_درخت.درخت) {
			release_درخت_content_recursive(e->branch_درخت.درخت);
			e->branch_درخت.درخت = پوچ;
		}
		cur_active_branches--;
	}
}

ایستا تهی load_branch(ساختار branch *b)
{
	load_درخت(&b->branch_درخت);
	اگر (!b->active) {
		b->active = 1;
		b->active_next_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_شمار++;
	}
}

ایستا unsigned نویسه convert_رقم_notes_to_fanout(uintmax_t رقم_notes)
{
	unsigned نویسه fanout = 0;
	وقتی ((رقم_notes >>= 8))
		fanout++;
	بازگشت fanout;
}

ایستا تهی construct_path_with_fanout(ثابت نویسه *hex_sha1,
		unsigned نویسه fanout, نویسه *path)
{
	unsigned صحیح i = 0, j = 0;
	اگر (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	وقتی (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	memcpy(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

ایستا uintmax_t do_change_note_fanout(
		ساختار درخت_entry *orig_ریشه, ساختار درخت_entry *ریشه,
		نویسه *hex_oid, unsigned صحیح hex_oid_len,
		نویسه *fullpath, unsigned صحیح fullpath_len,
		unsigned نویسه fanout)
{
	ساختار درخت_content *t;
	ساختار درخت_entry *e, leaf;
	unsigned صحیح i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t رقم_notes = 0;
	ساختار object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	نویسه realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ثابت unsigned hexsz = the_hash_algo->hexsz;

	اگر (!ریشه->درخت)
		load_درخت(ریشه);
	t = ریشه->درخت;

	برای (i = 0; t && i < t->entry_شمار; i++) {
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
		اگر (!e->versions[1].mode ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			ادامه;

		/* This _may_ be a note entry, or a subdir containing notes */
		memcpy(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		اگر (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		memcpy(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		اگر (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			اگر (fanout == 0xff) {
				/* Counting mode, no rename */
				رقم_notes++;
				ادامه;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			اگر (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				رقم_notes++;
				ادامه;
			}

			/* Rename fullpath to realpath */
			اگر (!درخت_content_remove(orig_ریشه, fullpath, &leaf, 0))
				die(_("failed to remove path %s"), fullpath);
			درخت_content_set(orig_ریشه, realpath,
				&leaf.versions[1].oid,
				leaf.versions[1].mode,
				leaf.درخت);
		} وگرنه اگر (S_ISDIR(e->versions[1].mode)) {
			/* This is a subdir that may contain note entries */
			رقم_notes += do_change_note_fanout(orig_ریشه, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = ریشه->درخت;
	}
	بازگشت رقم_notes;
}

ایستا uintmax_t change_note_fanout(ساختار درخت_entry *ریشه,
		unsigned نویسه fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	نویسه hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	بازگشت do_change_note_fanout(ریشه, ریشه, hex_oid, 0, path, 0, fanout);
}

ایستا صحیح parse_mapped_oid_hex(ثابت نویسه *hex, ساختار object_id *oid, ثابت نویسه **end)
{
	صحیح algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, اندازه(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	اگر (algo == GIT_HASH_UNKNOWN)
		بازگشت -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	اگر (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		اگر (hash_algos[algo].format_id == the_hash_algo->format_id)
			بازگشت 0;
		بازگشت -1;
	}
	oidcpy(oid, kh_مقدار(sub_oid_map, it));
	بازگشت 0;
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
ایستا uintmax_t parse_mark_ref(ثابت نویسه *p, نویسه **endptr)
{
	uintmax_t mark;

	أكد(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	اگر (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	بازگشت mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
ایستا uintmax_t parse_mark_ref_eol(ثابت نویسه *p)
{
	نویسه *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	اگر (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	بازگشت mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
ایستا uintmax_t parse_mark_ref_space(ثابت نویسه **p)
{
	uintmax_t mark;
	نویسه *end;

	mark = parse_mark_ref(*p, &end);
	اگر (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	بازگشت mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
ایستا تهی parse_path(ساختار strbuf *sb, ثابت نویسه *p, ثابت نویسه **endp,
		صحیح is_last_field, ثابت نویسه *field)
{
	اگر (*p == '"') {
		اگر (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), field, command_buf.buf);
		اگر (strlen(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), field, command_buf.buf);
	} وگرنه {
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
ایستا تهی parse_path_eol(ساختار strbuf *sb, ثابت نویسه *p, ثابت نویسه *field)
{
	ثابت نویسه *end;

	parse_path(sb, p, &end, 1, field);
	اگر (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
ایستا تهی parse_path_space(ساختار strbuf *sb, ثابت نویسه *p,
		ثابت نویسه **endp, ثابت نویسه *field)
{
	parse_path(sb, p, endp, 0, field);
	اگر (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

ایستا تهی file_change_m(ساختار fast_import_state *state,
			  ثابت نویسه *p,
			  ساختار branch *b)
{
	ایستا ساختار strbuf path = STRBUF_INIT;
	ساختار object_entry *oe;
	ساختار object_id oid;
	uint16_t mode, inline_data = 0;

	p = parse_mode(p, &mode);
	اگر (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	انتخاب (mode) {
	حالت 0644:
	حالت 0755:
		mode |= S_IFREG;
	حالت S_IFREG | 0644:
	حالت S_IFREG | 0755:
	حالت S_IFLNK:
	حالت S_IFDIR:
	حالت S_IFGITLINK:
		/* ok */
		بشکن;
	پیشفرض:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	اگر (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} وگرنه اگر (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = پوچ; /* not used with inline_data, but makes gcc happy */
	} وگرنه {
		اگر (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		اگر (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	اگر (S_ISDIR(mode) &&
	    is_empty_درخت_oid(&oid, the_repository->hash_algo) &&
	    *path.buf) {
		درخت_content_remove(&b->branch_درخت, path.buf, پوچ, 0);
		بازگشت;
	}

	اگر (S_ISGITLINK(mode)) {
		اگر (inline_data)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		وگرنه اگر (oe) {
			اگر (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} وگرنه اگر (inline_data) {
		اگر (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		وقتی (read_next_command(state) != EOF) {
			ثابت نویسه *v;
			اگر (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			وگرنه {
				parse_and_store_blob(&last_blob, &oid, 0);
				بشکن;
			}
		}
	} وگرنه {
		شمارش object_type expected = S_ISDIR(mode) ?
						OBJ_درخت: OBJ_BLOB;
		شمارش object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, پوچ);
		اگر (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(mode) ?  _("tree") : _("blob"),
			    command_buf.buf);
		اگر (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	اگر (!*path.buf) {
		درخت_content_replace(&b->branch_درخت, &oid, mode, پوچ);
		بازگشت;
	}

	اگر (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	درخت_content_set(&b->branch_درخت, path.buf, &oid, mode, پوچ);
}

ایستا تهی file_change_d(ثابت نویسه *p, ساختار branch *b)
{
	ایستا ساختار strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	درخت_content_remove(&b->branch_درخت, path.buf, پوچ, 1);
}

ایستا تهی file_change_cr(ثابت نویسه *p, ساختار branch *b, صحیح rename)
{
	ایستا ساختار strbuf source = STRBUF_INIT;
	ایستا ساختار strbuf dest = STRBUF_INIT;
	ساختار درخت_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	memset(&leaf, 0, اندازه(leaf));
	اگر (rename)
		درخت_content_remove(&b->branch_درخت, source.buf, &leaf, 1);
	وگرنه
		درخت_content_get(&b->branch_درخت, source.buf, &leaf, 1);
	اگر (!leaf.versions[1].mode)
		die(_("path %s not in branch"), source.buf);
	اگر (!*dest.buf) {	/* C "path/to/subdir" "" */
		درخت_content_replace(&b->branch_درخت,
			&leaf.versions[1].oid,
			leaf.versions[1].mode,
			leaf.درخت);
		بازگشت;
	}
	اگر (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	درخت_content_set(&b->branch_درخت, dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.درخت);
}

ایستا تهی note_change_n(ساختار fast_import_state *state,
			  ثابت نویسه *p,
			  ساختار branch *b,
			  unsigned نویسه *old_fanout)
{
	ساختار object_entry *oe;
	ساختار branch *s;
	ساختار object_id oid, commit_oid;
	نویسه path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_data = 0;
	unsigned نویسه new_fanout;

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
	اگر (b->رقم_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->رقم_notes = change_note_fanout(&b->branch_درخت, 0xff);
		*old_fanout = convert_رقم_notes_to_fanout(b->رقم_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	اگر (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} وگرنه اگر (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = پوچ; /* not used with inline_data, but makes gcc happy */
	} وگرنه {
		اگر (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		اگر (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	اگر (s) {
		اگر (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} وگرنه اگر (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		ساختار object_entry *commit_oe = find_mark(marks, commit_mark);
		اگر (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} وگرنه اگر (!repo_get_oid(the_repository, p, &commit_oid)) {
		size_t size;
		نویسه *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &size,
						   &commit_oid);
		اگر (!buf || size < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		آزاد(buf);
	} وگرنه
		die(_("invalid ref name or SHA1 expression: %s"), p);

	اگر (inline_data) {
		read_next_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} وگرنه اگر (oe) {
		اگر (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} وگرنه اگر (!is_null_oid(&oid)) {
		شمارش object_type type = odb_read_object_info(the_repository->objects, &oid,
							پوچ);
		اگر (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		اگر (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	اگر (درخت_content_remove(&b->branch_درخت, path, پوچ, 0))
		b->رقم_notes--;

	اگر (is_null_oid(&oid))
		بازگشت; /* nothing to insert */

	b->رقم_notes++;
	new_fanout = convert_رقم_notes_to_fanout(b->رقم_notes);
	construct_path_with_fanout(oid_to_hex(&commit_oid), new_fanout, path);
	درخت_content_set(&b->branch_درخت, path, &oid, S_IFREG | 0644, پوچ);
}

ایستا تهی file_change_deleteall(ساختار branch *b)
{
	release_درخت_content_recursive(b->branch_درخت.درخت);
	oidclr(&b->branch_درخت.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_درخت.versions[1].oid, the_repository->hash_algo);
	load_درخت(&b->branch_درخت);
	b->رقم_notes = 0;
}

ایستا تهی parse_from_commit(ساختار branch *b, نویسه *buf, unsigned long size)
{
	اگر (!buf || size < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	اگر (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_درخت.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_درخت.versions[0].oid,
	       &b->branch_درخت.versions[1].oid);
}

ایستا تهی parse_from_existing(ساختار branch *b)
{
	اگر (is_null_oid(&b->oid)) {
		oidclr(&b->branch_درخت.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_درخت.versions[1].oid, the_repository->hash_algo);
	} وگرنه {
		unsigned long size;
		size_t size_st = 0;
		نویسه *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &size_st, &b->oid);
		size = cast_size_t_to_ulong(size_st);
		parse_from_commit(b, buf, size);
		آزاد(buf);
	}
}

ایستا صحیح parse_objectish(ساختار fast_import_state *state,
			   ساختار branch *b,
			   ثابت نویسه *objectish)
{
	ساختار branch *s;
	ساختار object_id oid;

	oidcpy(&oid, &b->branch_درخت.versions[1].oid);

	s = lookup_branch(objectish);
	اگر (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	وگرنه اگر (s) {
		ساختار object_id *t = &s->branch_درخت.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_درخت.versions[0].oid, t);
		oidcpy(&b->branch_درخت.versions[1].oid, t);
	} وگرنه اگر (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		ساختار object_entry *oe = find_mark(marks, idnum);
		اگر (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		اگر (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			اگر (oe->pack_id != MAX_PACK_ID) {
				unsigned long size;
				نویسه *buf = gfi_unpack_entry(oe, &size);
				parse_from_commit(b, buf, size);
				آزاد(buf);
			} وگرنه
				parse_from_existing(b);
		}
	} وگرنه اگر (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		اگر (is_null_oid(&b->oid))
			b->delete = 1;
	}
	وگرنه
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	اگر (b->branch_درخت.درخت && !oideq(&oid, &b->branch_درخت.versions[1].oid)) {
		release_درخت_content_recursive(b->branch_درخت.درخت);
		b->branch_درخت.درخت = پوچ;
	}

	read_next_command(state);
	بازگشت 1;
}

ایستا صحیح parse_from(ساختار fast_import_state *state, ساختار branch *b)
{
	ثابت نویسه *from;

	اگر (!skip_prefix(command_buf.buf, "from ", &from))
		بازگشت 0;

	بازگشت parse_objectish(state, b, from);
}

ایستا صحیح parse_objectish_with_prefix(ساختار fast_import_state *state,
				       ساختار branch *b,
				       ثابت نویسه *prefix)
{
	ثابت نویسه *base;

	اگر (!skip_prefix(command_buf.buf, prefix, &base))
		بازگشت 0;

	بازگشت parse_objectish(state, b, base);
}

ایستا ساختار hash_list *parse_merge(ساختار fast_import_state *state,
				     unsigned صحیح *شمار)
{
	ساختار hash_list *list = پوچ, **tail = &list, *n;
	ثابت نویسه *from;
	ساختار branch *s;

	*شمار = 0;
	وقتی (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(اندازه(*n));
		s = lookup_branch(from);
		اگر (s)
			oidcpy(&n->oid, &s->oid);
		وگرنه اگر (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			ساختار object_entry *oe = find_mark(marks, idnum);
			اگر (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} وگرنه اگر (!repo_get_oid(the_repository, from, &n->oid)) {
			size_t size;
			نویسه *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &size, &n->oid);
			اگر (!buf || size < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			آزاد(buf);
		} وگرنه
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->next = پوچ;
		*tail = n;
		tail = &n->next;

		(*شمار)++;
		read_next_command(state);
	}
	بازگشت list;
}

ساختار signature_data {
	نویسه *hash_algo;      /* "sha1" or "sha256" */
	نویسه *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	ساختار strbuf data;   /* The actual signature data */
};

ایستا تهی parse_one_signature(ساختار fast_import_state *state,
				ساختار signature_data *sig,
				ثابت نویسه *v)
{
	نویسه *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	نویسه *space = strchr(args, ' ');

	اگر (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), args);
	*space = '\0';

	sig->hash_algo = args;
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
	read_next_command(state);
	parse_data(&sig->data, 0, پوچ);
}

ایستا تهی discard_one_signature(ساختار fast_import_state *state)
{
	ساختار strbuf data = STRBUF_INIT;

	read_next_command(state);
	parse_data(&data, 0, پوچ);
	strbuf_release(&data);
}

ایستا تهی add_gpgsig_to_commit(ساختار strbuf *commit_data,
				 ثابت نویسه *header,
				 ساختار signature_data *sig)
{
	ساختار string_list siglines = STRING_LIST_INIT_NODUP;

	اگر (!sig || !sig->hash_algo)
		بازگشت;

	strbuf_addstr(commit_data, header);
	string_list_split_in_place(&siglines, sig->data.buf, "\n", -1);
	strbuf_add_separated_string_list(commit_data, "\n ", &siglines);
	strbuf_addch(commit_data, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->data);
	آزاد(sig->hash_algo);
}

ایستا تهی store_signature(ساختار signature_data *stored_sig,
			    ساختار signature_data *new_sig,
			    ثابت نویسه *hash_type)
{
	اگر (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->data);
		آزاد(new_sig->hash_algo);
	} وگرنه {
		*stored_sig = *new_sig;
	}
}

ایستا تهی import_one_signature(ساختار fast_import_state *state,
				 ساختار signature_data *sig_sha1,
				 ساختار signature_data *sig_sha256,
				 ثابت نویسه *v)
{
	ساختار signature_data sig = { پوچ, پوچ, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	اگر (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	وگرنه اگر (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	وگرنه
		die(_("parse_one_signature() returned unknown hash algo"));
}

ایستا تهی finalize_commit_buffer(ساختار strbuf *new_data,
				   ساختار signature_data *sig_sha1,
				   ساختار signature_data *sig_sha256,
				   ساختار strbuf *پیام)
{
	add_gpgsig_to_commit(new_data, "gpgsig ", sig_sha1);
	add_gpgsig_to_commit(new_data, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_data, '\n');
	strbuf_addbuf(new_data, پیام);
}

ایستا تهی warn_invalid_signature(ساختار signature_check *check,
				   ثابت نویسه *پیام, شمارش sign_mode mode)
{
	ثابت نویسه *signer = check->signer ? check->signer : _("unknown");
	ثابت نویسه *subject;
	صحیح subject_len = find_commit_subject(پیام, &subject);

	انتخاب (mode) {
	حالت SIGN_STRIP_IF_INVALID:
		اگر (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		وگرنه اگر (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		وگرنه
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		بشکن;
	حالت SIGN_SIGN_IF_INVALID:
		اگر (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		وگرنه اگر (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		وگرنه
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		بشکن;
	پیشفرض:
		BUG("unsupported signing mode");
	}
}

ایستا تهی handle_signature_if_invalid(ساختار strbuf *new_data,
					ساختار signature_data *sig_sha1,
					ساختار signature_data *sig_sha256,
					ساختار strbuf *پیام,
					شمارش sign_mode mode)
{
	ساختار strbuf tmp_buf = STRBUF_INIT;
	ساختار signature_check signature_check = { 0 };
	صحیح ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_data);
	finalize_commit_buffer(&tmp_buf, sig_sha1, sig_sha256, پیام);
	ret = verify_commit_buffer(tmp_buf.buf, tmp_buf.len, &signature_check);

	اگر (ret) {
		اگر (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, پیام->buf, mode);

		اگر (mode == SIGN_SIGN_IF_INVALID) {
			ساختار strbuf signature = STRBUF_INIT;
			ساختار strbuf payload = STRBUF_INIT;

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
			اگر (sign_buffer(&payload, &signature, signed_commit_keyid,
					SIGN_BUFFER_USE_DEFAULT_کلید))
				die(_("failed to sign commit object"));
			add_header_signature(new_data, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_buffer(new_data, پوچ, پوچ, پیام);
	} وگرنه {
		strbuf_swap(new_data, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

ایستا تهی parse_new_commit(ساختار fast_import_state *state, ثابت نویسه *arg)
{
	ایستا ساختار strbuf پیام = STRBUF_INIT;
	ساختار signature_data sig_sha1 = { پوچ, پوچ, STRBUF_INIT };
	ساختار signature_data sig_sha256 = { پوچ, پوچ, STRBUF_INIT };
	ساختار branch *b;
	نویسه *author = پوچ;
	نویسه *committer = پوچ;
	نویسه *encoding = پوچ;
	ساختار hash_list *merge_list = پوچ;
	unsigned صحیح merge_شمار;
	unsigned نویسه prev_fanout, new_fanout;
	ثابت نویسه *v;

	b = lookup_branch(arg);
	اگر (!b)
		b = new_branch(arg);

	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	اگر (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_next_command(state);
	}
	اگر (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_next_command(state);
	}
	اگر (!committer)
		die(_("expected committer but didn't get one"));

	وقتی (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		انتخاب (signed_commit_mode) {

		/* First, modes that don't need the signature to be parsed */
		حالت SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		حالت SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		حالت SIGN_STRIP:
			discard_one_signature(state);
			بشکن;

		/* Second, modes that parse the signature */
		حالت SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		حالت SIGN_VERBATIM:
		حالت SIGN_STRIP_IF_INVALID:
		حالت SIGN_SIGN_IF_INVALID:
		حالت SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			بشکن;

		/* Third, BUG */
		پیشفرض:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_next_command(state);
	}

	اگر (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_next_command(state);
	}
	parse_data(&پیام, 0, پوچ);
	read_next_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_شمار);

	/* ensure the branch is active/loaded */
	اگر (!b->branch_درخت.درخت || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	prev_fanout = convert_رقم_notes_to_fanout(b->رقم_notes);

	/* file_change* */
	وقتی (command_buf.len > 0) {
		اگر (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		وگرنه اگر (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		وگرنه اگر (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		وگرنه اگر (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		وگرنه اگر (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		وگرنه اگر (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		وگرنه اگر (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		وگرنه اگر (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		وگرنه {
			unread_command_buf = 1;
			بشکن;
		}
		اگر (read_next_command(state) == EOF)
			بشکن;
	}

	new_fanout = convert_رقم_notes_to_fanout(b->رقم_notes);
	اگر (new_fanout != prev_fanout)
		b->رقم_notes = change_note_fanout(&b->branch_درخت, new_fanout);

	/* build the tree and the commit */
	store_درخت(&b->branch_درخت);
	oidcpy(&b->branch_درخت.versions[0].oid,
	       &b->branch_درخت.versions[1].oid);

	strbuf_reset(&new_data);
	strbuf_addf(&new_data, "tree %s\n",
		oid_to_hex(&b->branch_درخت.versions[1].oid));
	اگر (!is_null_oid(&b->oid))
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&b->oid));
	وقتی (merge_list) {
		ساختار hash_list *next = merge_list->next;
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&merge_list->oid));
		آزاد(merge_list);
		merge_list = next;
	}
	strbuf_addf(&new_data,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	اگر (encoding)
		strbuf_addf(&new_data,
			"encoding %s\n",
			encoding);

	اگر ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
	     signed_commit_mode == SIGN_SIGN_IF_INVALID ||
	     signed_commit_mode == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_data, &sig_sha1, &sig_sha256,
					    &پیام, signed_commit_mode);
	وگرنه
		finalize_commit_buffer(&new_data, &sig_sha1, &sig_sha256, &پیام);

	آزاد(author);
	آزاد(committer);
	آزاد(encoding);

	اگر (!store_object(OBJ_COMMIT, &new_data, پوچ, &b->oid, next_mark))
		b->pack_id = pack_id;
	b->last_commit = object_شمار_by_type[OBJ_COMMIT];
}

ایستا تهی handle_tag_signature_if_invalid(ساختار strbuf *buf,
					    ساختار strbuf *پیام,
					    size_t sig_offset)
{
	ساختار strbuf signature = STRBUF_INIT;
	ساختار strbuf payload = STRBUF_INIT;
	ساختار signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_add(&payload, پیام->buf, sig_offset);
	strbuf_add(&signature, پیام->buf + sig_offset, پیام->len - sig_offset);

	sigc.payload_type = SIGNATURE_PAYLOAD_TAG;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	اگر (!check_signature(&sigc, signature.buf, signature.len))
		برو out;

	اگر (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(پیام, sig_offset);

	اگر (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = پوچ;
		strbuf_reset(&signature);

		اگر (sign_buffer(&payload, &signature, signed_tag_keyid,
				SIGN_BUFFER_USE_DEFAULT_کلید))
			die(_("failed to sign tag object"));

		strbuf_addbuf(پیام, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

ایستا تهی handle_tag_signature(ساختار strbuf *buf, ساختار strbuf *پیام, ثابت نویسه *name)
{
	size_t sig_offset = parse_signed_buffer(پیام->buf, پیام->len);

	/* If there is no signature, there is nothing to do. */
	اگر (sig_offset >= پیام->len)
		بازگشت;

	انتخاب (signed_tag_mode) {

	/* First, modes that don't change anything */
	حالت SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	حالت SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		بشکن;

	/* Second, modes that remove the signature */
	حالت SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	حالت SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(پیام, sig_offset);
		بشکن;
	حالت SIGN_ABORT_IF_INVALID:
	حالت SIGN_SIGN_IF_INVALID:
	حالت SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, پیام, sig_offset);
		بشکن;

	/* Third, aborting modes */
	حالت SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	پیشفرض:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

ایستا تهی parse_new_tag(ساختار fast_import_state *state, ثابت نویسه *arg)
{
	ایستا ساختار strbuf پیام = STRBUF_INIT;
	ثابت نویسه *from;
	نویسه *tagger;
	ساختار branch *s;
	ساختار tag *t;
	uintmax_t from_mark = 0;
	ساختار object_id oid;
	شمارش object_type type;
	ثابت نویسه *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, اندازه(ساختار tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	اگر (last_tag)
		last_tag->next_tag = t;
	وگرنه
		first_tag = t;
	last_tag = t;
	read_next_command(state);
	parse_mark(state);

	/* from ... */
	اگر (!skip_prefix(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	اگر (s) {
		اگر (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} وگرنه اگر (*from == ':') {
		ساختار object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} وگرنه اگر (!repo_get_oid(the_repository, from, &oid)) {
		ساختار object_entry *oe = find_object(&oid);
		اگر (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, پوچ);
			اگر (type < 0)
				die(_("not a valid object: %s"), from);
		} وگرنه
			type = oe->type;
	} وگرنه
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_next_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	اگر (skip_prefix(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_next_command(state);
	} وگرنه
		tagger = پوچ;

	/* tag payload/message */
	parse_data(&پیام, 0, پوچ);

	/* build the tag object */
	strbuf_reset(&new_data);

	strbuf_addf(&new_data,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_to_hex(&oid), type_name(type), t->name);
	اگر (tagger)
		strbuf_addf(&new_data,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_data, &پیام, t->name);

	strbuf_addch(&new_data, '\n');
	strbuf_addbuf(&new_data, &پیام);
	آزاد(tagger);

	اگر (store_object(OBJ_TAG, &new_data, پوچ, &t->oid, next_mark))
		t->pack_id = MAX_PACK_ID;
	وگرنه
		t->pack_id = pack_id;
}

ایستا تهی parse_reset_branch(ساختار fast_import_state *state, ثابت نویسه *arg)
{
	ساختار branch *b;
	ثابت نویسه *tag_name;

	b = lookup_branch(arg);
	اگر (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_درخت.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_درخت.versions[1].oid, the_repository->hash_algo);
		اگر (b->branch_درخت.درخت) {
			release_درخت_content_recursive(b->branch_درخت.درخت);
			b->branch_درخت.درخت = پوچ;
		}
	}
	وگرنه
		b = new_branch(arg);
	read_next_command(state);
	parse_from(state, b);
	اگر (b->delete && skip_prefix(b->name, "refs/tags/", &tag_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		ساختار tag *t, *prev = پوچ;
		برای (t = first_tag; t; t = t->next_tag) {
			اگر (!strcmp(t->name, tag_name))
				بشکن;
			prev = t;
		}
		اگر (t) {
			اگر (prev)
				prev->next_tag = t->next_tag;
			وگرنه
				first_tag = t->next_tag;
			اگر (!t->next_tag)
				last_tag = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	اگر (command_buf.len > 0)
		unread_command_buf = 1;
}

ایستا تهی cat_blob_write(ثابت نویسه *buf, unsigned long size)
{
	اگر (write_in_full(cat_blob_fd, buf, size) < 0)
		die_errno(_("write to frontend failed"));
}

ایستا تهی cat_blob(ساختار object_entry *oe, ساختار object_id *oid)
{
	ساختار strbuf سطر = STRBUF_INIT;
	ساختار iovec iov[3];
	unsigned long size;
	شمارش object_type type = 0;
	نویسه *buf;

	اگر (!oe || oe->pack_id == MAX_PACK_ID) {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
	} وگرنه {
		type = oe->type;
		buf = gfi_unpack_entry(oe, &size);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	اگر (type <= 0) {
		strbuf_reset(&سطر);
		strbuf_addf(&سطر, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(سطر.buf, سطر.len);
		strbuf_release(&سطر);
		آزاد(buf);
		بازگشت;
	}
	اگر (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	اگر (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_to_hex(oid), type_name(type));
	strbuf_reset(&سطر);
	strbuf_addf(&سطر, "%s %s %"PRIuMAX"\n", oid_to_hex(oid),
		    type_name(type), (uintmax_t)size);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_base = سطر.buf;
	iov[0].iov_len = سطر.len;
	iov[1].iov_base = buf;
	iov[1].iov_len = size;
	iov[2].iov_base = (تهی *) "\n";
	iov[2].iov_len = 1;

	اگر (writev_in_full(cat_blob_fd, iov, ARRAY_SIZE(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&سطر);
	اگر (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.data, buf, size, size + 1);
		last_blob.depth = oe->depth;
	} وگرنه
		آزاد(buf);
}

ایستا تهی parse_get_mark(ساختار fast_import_state *state UNUSED,
			   ثابت نویسه *p)
{
	ساختار object_entry *oe;
	نویسه output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	اگر (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	اگر (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, اندازه(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

ایستا تهی parse_cat_blob(ساختار fast_import_state *state UNUSED,
			   ثابت نویسه *p)
{
	ساختار object_entry *oe;
	ساختار object_id oid;

	/* cat-blob SP <object> LF */
	اگر (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		اگر (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} وگرنه {
		اگر (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		اگر (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

ایستا ساختار object_entry *dereference(ساختار object_entry *oe,
					ساختار object_id *oid)
{
	unsigned long size;
	نویسه *buf = پوچ;
	ثابت unsigned hexsz = the_hash_algo->hexsz;

	اگر (!oe) {
		شمارش object_type type = odb_read_object_info(the_repository->objects,
							     oid, پوچ);
		اگر (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = درج_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	انتخاب (oe->type) {
	حالت OBJ_درخت:	/* easy case. */
		بازگشت oe;
	حالت OBJ_COMMIT:
	حالت OBJ_TAG:
		بشکن;
	پیشفرض:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	اگر (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &size);
	} وگرنه {
		شمارش object_type unused;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &size_st);
		size = cast_size_t_to_ulong(size_st);
	}
	اگر (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	انتخاب (oe->type) {
	حالت OBJ_TAG:
		اگر (size < hexsz + strlen("object ") ||
		    get_oid_hex(buf + strlen("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		بشکن;
	حالت OBJ_COMMIT:
		اگر (size < hexsz + strlen("tree ") ||
		    get_oid_hex(buf + strlen("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	آزاد(buf);
	بازگشت find_object(oid);
}

ایستا تهی درج_mapped_mark(uintmax_t mark, تهی *object, تهی *cbp)
{
	ساختار object_id *fromoid = object;
	ساختار object_id *tooid = find_mark(cbp, mark);
	صحیح ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	اگر (ret == 0)
		بازگشت;
	kh_مقدار(sub_oid_map, it) = tooid;
}

ایستا تهی build_mark_map_one(ساختار mark_set *from, ساختار mark_set *to)
{
	for_each_mark(from, 0, درج_mapped_mark, to);
}

ایستا تهی build_mark_map(ساختار string_list *from, ساختار string_list *to)
{
	ساختار string_list_item *fromp, *بالاترین;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		بالاترین = string_list_lookup(to, fromp->string);
		اگر (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} وگرنه اگر (!بالاترین || !بالاترین->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, بالاترین->util);
	}
}

ایستا ساختار object_entry *parse_treeish_dataref(ثابت نویسه **p)
{
	ساختار object_id oid;
	ساختار object_entry *e;

	اگر (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		اگر (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} وگرنه {	/* <sha1> */
		اگر (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		اگر (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	وقتی (!e || e->type != OBJ_درخت)
		e = dereference(e, &oid);
	بازگشت e;
}

ایستا تهی print_ls(صحیح mode, ثابت unsigned نویسه *hash, ثابت نویسه *path)
{
	ایستا ساختار strbuf سطر = STRBUF_INIT;

	/* See show_tree(). */
	ثابت نویسه *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? درخت_type :
		blob_type;

	اگر (!mode) {
		/* missing SP path LF */
		strbuf_reset(&سطر);
		strbuf_addstr(&سطر, "missing ");
		quote_c_style(path, &سطر, پوچ, 0);
		strbuf_addch(&سطر, '\n');
	} وگرنه {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&سطر);
		strbuf_addf(&سطر, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &سطر, پوچ, 0);
		strbuf_addch(&سطر, '\n');
	}
	cat_blob_write(سطر.buf, سطر.len);
}

ایستا تهی parse_ls(ساختار fast_import_state *state UNUSED,
		     ثابت نویسه *p,
		     ساختار branch *b)
{
	ایستا ساختار strbuf path = STRBUF_INIT;
	ساختار درخت_entry *ریشه = پوچ;
	ساختار درخت_entry leaf = {پوچ};

	/* ls SP (<tree-ish> SP)? <path> */
	اگر (*p == '"') {
		اگر (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		ریشه = &b->branch_درخت;
	} وگرنه {
		ساختار object_entry *e = parse_treeish_dataref(&p);
		ریشه = new_درخت_entry();
		oidcpy(&ریشه->versions[1].oid, &e->idx.oid);
		اگر (!is_null_oid(&ریشه->versions[1].oid))
			ریشه->versions[1].mode = S_IFDIR;
		load_درخت(ریشه);
	}
	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	درخت_content_get(ریشه, path.buf, &leaf, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	اگر (S_ISDIR(leaf.versions[1].mode))
		store_درخت(&leaf);

	print_ls(leaf.versions[1].mode, leaf.versions[1].oid.hash, path.buf);
	اگر (leaf.درخت)
		release_درخت_content_recursive(leaf.درخت);
	اگر (!b || ریشه != &b->branch_درخت)
		release_درخت_entry(ریشه);
}

ایستا تهی checkpoint(تهی)
{
	checkpoint_requested = 0;
	اگر (object_شمار) {
		cycle_packfile();
	}
	dump_branches();
	dump_tags();
	dump_marks();
}

ایستا تهی parse_checkpoint(ساختار fast_import_state *state UNUSED)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

ایستا تهی parse_progress(ساختار fast_import_state *state UNUSED)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

ایستا تهی parse_alias(ساختار fast_import_state *state)
{
	ساختار object_entry *e;
	ساختار branch b;

	skip_optional_lf();
	read_next_command(state);

	/* mark ... */
	parse_mark(state);
	اگر (!next_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, اندازه(b));
	اگر (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	أكد(e);
	درج_mark(&marks, next_mark, e);
}

ایستا نویسه* make_fast_import_path(ساختار fast_import_state *state,
				   ثابت نویسه *path)
{
	اگر (!relative_marks_paths || is_absolute_path(path))
		بازگشت prefix_filename(state->prefix, path);
	بازگشت repo_git_path(the_repository, "info/fast-import/%s", path);
}

ایستا تهی option_import_marks(ساختار fast_import_state *state,
				ثابت نویسه *marks,
				صحیح from_stream,
				صحیح ignore_missing)
{
	اگر (import_marks_file) {
		اگر (from_stream)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		اگر(!import_marks_file_from_stream)
			read_marks();
	}

	آزاد(import_marks_file);
	import_marks_file = make_fast_import_path(state, marks);
	import_marks_file_from_stream = from_stream;
	import_marks_file_ignore_missing = ignore_missing;
}

ایستا تهی option_date_format(ثابت نویسه *fmt)
{
	اگر (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	وگرنه اگر (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	وگرنه اگر (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	وگرنه اگر (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	وگرنه
		die(_("unknown --date-format argument %s"), fmt);
}

ایستا unsigned long ulong_arg(ثابت نویسه *option, ثابت نویسه *arg)
{
	نویسه *endptr;
	unsigned long rv = strtoul(arg, &endptr, 0);
	اگر (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	بازگشت rv;
}

ایستا تهی option_depth(ثابت نویسه *depth)
{
	max_depth = ulong_arg("--depth", depth);
	اگر (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

ایستا تهی option_active_branches(ثابت نویسه *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

ایستا تهی option_export_marks(ساختار fast_import_state *state,
				ثابت نویسه *marks)
{
	آزاد(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

ایستا تهی option_cat_blob_fd(ساختار fast_import_state *state UNUSED,
			       ثابت نویسه *fd)
{
	unsigned long n = ulong_arg("--cat-blob-fd", fd);
	اگر (n > (unsigned long) INT_MAX)
		die(_("--cat-blob-fd cannot exceed %d"), INT_MAX);
	cat_blob_fd = (صحیح) n;
}

ایستا تهی option_export_pack_edges(ساختار fast_import_state *state,
				     ثابت نویسه *edges)
{
	نویسه *fn = prefix_filename(state->prefix, edges);
	اگر (pack_edges)
		fclose(pack_edges);
	pack_edges = xfopen(fn, "a");
	آزاد(fn);
}

ایستا تهی option_rewrite_submodules(ساختار fast_import_state *state,
				      ثابت نویسه *arg,
				      ساختار string_list *list)
{
	ساختار mark_set *ms;
	FILE *fp;
	نویسه *s = xstrdup(arg);
	نویسه *f = strchr(s, ':');
	اگر (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = prefix_filename(state->prefix, f);
	fp = fopen(f, "r");
	اگر (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, درج_oid_entry);
	fclose(fp);
	آزاد(f);

	string_list_درج(list, s)->util = ms;

	آزاد(s);
}

ایستا تهی option_max_pack_size(ثابت نویسه *arg)
{
	unsigned long v;

	اگر (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	اگر (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} وگرنه اگر (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

ایستا تهی option_big_file_threshold(ثابت نویسه *arg)
{
	unsigned long v;

	اگر (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

ایستا تهی option_signed_commits(ثابت نویسه *arg)
{
	اگر (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

ایستا تهی option_signed_tags(ثابت نویسه *arg)
{
	اگر (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

ایستا تهی option_quiet(تهی)
{
	show_stats = 0;
	quiet = 1;
}

ایستا صحیح parse_one_option(ساختار fast_import_state *state, ثابت نویسه *option)
{
	اگر (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_size(option);
	} وگرنه اگر (skip_prefix(option, "big-file-threshold=", &option)) {
		option_big_file_threshold(option);
	} وگرنه اگر (skip_prefix(option, "depth=", &option)) {
		option_depth(option);
	} وگرنه اگر (skip_prefix(option, "active-branches=", &option)) {
		option_active_branches(option);
	} وگرنه اگر (skip_prefix(option, "export-pack-edges=", &option)) {
		option_export_pack_edges(state, option);
	} وگرنه اگر (skip_prefix(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} وگرنه اگر (skip_prefix(option, "signed-tags=", &option)) {
		option_signed_tags(option);
	} وگرنه اگر (!strcmp(option, "quiet")) {
		option_quiet();
	} وگرنه اگر (!strcmp(option, "stats")) {
		show_stats = 1;
	} وگرنه اگر (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} وگرنه {
		بازگشت 0;
	}

	بازگشت 1;
}

ایستا تهی check_unsafe_feature(ساختار fast_import_state *state,
				 ثابت نویسه *feature)
{
	اگر (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

ایستا صحیح parse_one_feature(ساختار fast_import_state *state,
			     ثابت نویسه *feature)
{
	ثابت نویسه *arg;

	اگر (skip_prefix(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} وگرنه اگر (skip_prefix(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} وگرنه اگر (skip_prefix(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} وگرنه اگر (skip_prefix(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} وگرنه اگر (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} وگرنه اگر (skip_prefix(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_to);
	} وگرنه اگر (skip_prefix(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} وگرنه اگر (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} وگرنه اگر (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} وگرنه اگر (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} وگرنه اگر (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} وگرنه اگر (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} وگرنه اگر (!strcmp(feature, "force")) {
		force_update = 1;
	} وگرنه اگر (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} وگرنه {
		بازگشت 0;
	}

	بازگشت 1;
}

ایستا تهی parse_feature(ساختار fast_import_state *state, ثابت نویسه *feature)
{
	اگر (state->seen_data_command)
		die(_("got feature command '%s' after data command"), feature);

	اگر (parse_one_feature(state, feature))
		بازگشت;

	die(_("this version of fast-import does not support feature %s."), feature);
}

ایستا تهی parse_option(ساختار fast_import_state *state, ثابت نویسه *option)
{
	اگر (state->seen_data_command)
		die(_("got option command '%s' after data command"), option);

	اگر (parse_one_option(state, option))
		بازگشت;

	die(_("this version of fast-import does not support option: %s"), option);
}

ایستا تهی git_pack_config(تهی)
{
	صحیح indexversion_مقدار;
	صحیح limit;
	unsigned long packsizelimit_مقدار;

	اگر (!repo_config_get_ulong(the_repository, "pack.depth", &max_depth)) {
		اگر (max_depth > MAX_DEPTH)
			max_depth = MAX_DEPTH;
	}
	اگر (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_مقدار)) {
		pack_idx_opts.version = indexversion_مقدار;
		اگر (pack_idx_opts.version > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.version);
	}
	اگر (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_مقدار))
		max_packsize = packsizelimit_مقدار;

	اگر (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &limit))
		unpack_limit = limit;
	وگرنه اگر (!repo_config_get_int(the_repository, "transfer.unpacklimit", &limit))
		unpack_limit = limit;

	repo_config(the_repository, git_default_config, پوچ);
}

ایستا ثابت نویسه *ثابت fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	پوچ
};

ایستا تهی parse_argv(ساختار fast_import_state *state)
{
	صحیح argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	اگر (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_data_command = 1;
	اگر (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

ایستا صحیح option_parse_date_format(ثابت ساختار option *opt UNUSED,
				    ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	بازگشت 0;
}

ایستا صحیح option_parse_export_pack_edges(ثابت ساختار option *opt,
					  ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->مقدار, arg);
	بازگشت 0;
}

ایستا صحیح option_parse_max_pack_size(ثابت ساختار option *opt UNUSED,
				      ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_size(arg);
	بازگشت 0;
}

ایستا صحیح option_parse_big_file_threshold(ثابت ساختار option *opt UNUSED,
					   ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	بازگشت 0;
}

ایستا صحیح option_parse_signed_commits(ثابت ساختار option *opt UNUSED,
				       ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	بازگشت 0;
}

ایستا صحیح option_parse_signed_tags(ثابت ساختار option *opt UNUSED,
				    ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	بازگشت 0;
}

ایستا صحیح option_parse_rewrite_submodules_from(ثابت ساختار option *opt,
						ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->مقدار, arg, &sub_marks_from);
	بازگشت 0;
}

ایستا صحیح option_parse_rewrite_submodules_to(ثابت ساختار option *opt,
					      ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->مقدار, arg, &sub_marks_to);
	بازگشت 0;
}

ایستا صحیح option_parse_cat_blob_fd(ثابت ساختار option *opt,
				    ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->مقدار, arg);
	بازگشت 0;
}

ایستا صحیح option_parse_import_marks(ثابت ساختار option *opt,
				     ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->مقدار, arg, 0, 0);
	بازگشت 0;
}

ایستا صحیح option_parse_import_marks_if_exists(ثابت ساختار option *opt,
					       ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->مقدار, arg, 0, 1);
	بازگشت 0;
}

ایستا صحیح option_parse_export_marks(ثابت ساختار option *opt,
				     ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->مقدار, arg);
	بازگشت 0;
}

ایستا صحیح option_parse_depth(ثابت ساختار option *opt UNUSED,
			      ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	بازگشت 0;
}

ایستا صحیح option_parse_active_branches(ثابت ساختار option *opt UNUSED,
					ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	بازگشت 0;
}

ایستا صحیح option_parse_quiet(ثابت ساختار option *opt UNUSED,
			      ثابت نویسه *arg UNUSED, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	بازگشت 0;
}

صحیح cmd_fast_import(صحیح argc,
		    ثابت نویسه **argv,
		    ثابت نویسه *prefix,
		    ساختار repository *repo)
{
	ساختار fast_import_state state;

	ساختار option fast_import_options[] = {
		OPT_GROUP(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", پوچ, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", پوچ, پوچ,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", پوچ, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_size),
		OPT_CALLBACK_F(0, "big-file-threshold", پوچ, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_threshold),
		OPT_CALLBACK_F(0, "depth", پوچ, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_depth),
		OPT_CALLBACK_F(0, "active-branches", پوچ, N_("n"),
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
		OPT_CALLBACK_F(0, "signed-commits", پوچ, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", پوچ, N_("mode"),
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
	CALLOC_ARRAY(avail_درخت_table, avail_درخت_table_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, اندازه(ساختار mark_set));

	hashmap_init(&object_table, object_entry_hashcmp, پوچ, 0);

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
	برای (صحیح i = 1; i < argc; i++) {
		ثابت نویسه *arg = argv[i];
		اگر (*arg != '-' || !strcmp(arg, "--"))
			بشکن;
		اگر (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * اندازه(*rc_free));
	برای (unsigned صحیح i = 0; i < (cmd_save - 1); i++)
		rc_free[i].next = &rc_free[i + 1];
	rc_free[cmd_save - 1].next = پوچ;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	وقتی (read_next_command(&state) != EOF) {
		ثابت نویسه *v;
		اگر (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		وگرنه اگر (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		وگرنه اگر (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		وگرنه اگر (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		وگرنه اگر (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, پوچ);
		وگرنه اگر (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		وگرنه اگر (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		وگرنه اگر (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		وگرنه اگر (!strcmp("done", command_buf.buf))
			بشکن;
		وگرنه اگر (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		وگرنه اگر (starts_with(command_buf.buf, "progress "))
			parse_progress(&state);
		وگرنه اگر (skip_prefix(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		وگرنه اگر (skip_prefix(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		وگرنه اگر (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		وگرنه
			die(_("unsupported command: %s"), command_buf.buf);

		اگر (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	اگر (!state.seen_data_command)
		parse_argv(&state);

	اگر (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	end_packfile();

	dump_branches();
	dump_tags();
	unkeep_all_packs();
	dump_marks();

	اگر (pack_edges)
		fclose(pack_edges);

	اگر (show_stats) {
		uintmax_t total_شمار = 0, duplicate_شمار = 0;
		برای (size_t i = 0; i < ARRAY_SIZE(object_شمار_by_type); i++)
			total_شمار += object_شمار_by_type[i];
		برای (size_t i = 0; i < ARRAY_SIZE(duplicate_شمار_by_type); i++)
			duplicate_شمار += duplicate_شمار_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_شمار);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", total_شمار, duplicate_شمار);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_شمار_by_type[OBJ_BLOB], duplicate_شمار_by_type[OBJ_BLOB], delta_شمار_by_type[OBJ_BLOB], delta_شمار_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_شمار_by_type[OBJ_درخت], duplicate_شمار_by_type[OBJ_درخت], delta_شمار_by_type[OBJ_درخت], delta_شمار_attempts_by_type[OBJ_درخت]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_شمار_by_type[OBJ_COMMIT], duplicate_شمار_by_type[OBJ_COMMIT], delta_شمار_by_type[OBJ_COMMIT], delta_شمار_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_شمار_by_type[OBJ_TAG], duplicate_شمار_by_type[OBJ_TAG], delta_شمار_by_type[OBJ_TAG], delta_شمار_attempts_by_type[OBJ_TAG]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_شمار, branch_load_شمار);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->shift) * 1024, marks_set_شمار);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (درخت_entry_allocd + fi_mem_pool.pool_alloc + alloc_شمار*اندازه(ساختار object_entry))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (unsigned long)((درخت_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_شمار*اندازه(ساختار object_entry))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	بازگشت failure ? 1 : 0;
}
