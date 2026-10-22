#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

بنية object_entry {
	بنية pack_idx_entry idx;
	بنية hashmap_entry ent;
	uint32_t type : TYPE_BITS,
		pack_id : PACK_ID_BITS,
		depth : DEPTH_BITS;
};

ساكن صحيح object_entry_hashcmp(ثابت فراغ *map_data UNUSED,
				ثابت بنية hashmap_entry *eptr,
				ثابت بنية hashmap_entry *entry_or_key,
				ثابت فراغ *keydata)
{
	ثابت بنية object_id *oid = keydata;
	ثابت بنية object_entry *e1, *e2;

	e1 = container_of(eptr, ثابت بنية object_entry, ent);
	ܐܢ (oid)
		ܦܢܐ oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_key, ثابت بنية object_entry, ent);
	ܦܢܐ oidcmp(&e1->idx.oid, &e2->idx.oid);
}

بنية object_entry_pool {
	بنية object_entry_pool *next_pool;
	بنية object_entry *next_free;
	بنية object_entry *end;
	بنية object_entry entries[FLEX_ARRAY]; /* more */
};

بنية mark_set {
	اتحاد {
		بنية object_id *oids[1024];
		بنية object_entry *marked[1024];
		بنية mark_set *sets[1024];
	} data;
	unsigned صحيح shift;
};

بنية last_object {
	بنية strbuf data;
	off_t offset;
	unsigned صحيح depth;
	unsigned no_swap : 1;
};

بنية atom_str {
	بنية atom_str *next_atom;
	unsigned short str_len;
	حرف str_dat[FLEX_ARRAY]; /* more */
};

بنية tree_content;
بنية tree_entry {
	بنية tree_content *tree;
	بنية atom_str *name;
	بنية tree_entry_ms {
		uint16_t mode;
		بنية object_id oid;
	} versions[2];
};

بنية tree_content {
	unsigned صحيح entry_capacity; /* must match avail_tree_content */
	unsigned صحيح entry_count;
	unsigned صحيح delta_depth;
	بنية tree_entry *entries[FLEX_ARRAY]; /* more */
};

بنية avail_tree_content {
	unsigned صحيح entry_capacity; /* must match tree_content */
	بنية avail_tree_content *next_avail;
};

بنية branch {
	بنية branch *table_next_branch;
	بنية branch *active_next_branch;
	ثابت حرف *name;
	بنية tree_entry branch_tree;
	uintmax_t last_commit;
	uintmax_t num_notes;
	unsigned active : 1;
	unsigned delete : 1;
	unsigned pack_id : PACK_ID_BITS;
	بنية object_id oid;
};

بنية tag {
	بنية tag *next_tag;
	ثابت حرف *name;
	unsigned صحيح pack_id;
	بنية object_id oid;
};

بنية hash_list {
	بنية hash_list *next;
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
	بنية recent_command *next;
	حرف *buf;
};

تعريفنوع فراغ (*mark_set_inserter_t)(بنية mark_set **s, بنية object_id *oid, uintmax_t mark);
تعريفنوع فراغ (*each_mark_fn_t)(uintmax_t mark, فراغ *obj, فراغ *cbp);

/* Configured limits on output */
ساكن unsigned long max_depth = 50;
ساكن off_t max_packsize;
ساكن صحيح unpack_limit = 100;
ساكن صحيح force_update;

/* Stats and misc. counters */
ساكن uintmax_t alloc_count;
ساكن uintmax_t marks_set_count;
ساكن uintmax_t object_count_by_type[1 << TYPE_BITS];
ساكن uintmax_t duplicate_count_by_type[1 << TYPE_BITS];
ساكن uintmax_t delta_count_by_type[1 << TYPE_BITS];
ساكن uintmax_t delta_count_attempts_by_type[1 << TYPE_BITS];
ساكن unsigned long object_count;
ساكن unsigned long branch_count;
ساكن unsigned long branch_load_count;
ساكن صحيح failure;
ساكن FILE *pack_edges;
ساكن صحيح show_stats = 1;
ساكن صحيح quiet;
ساكن تعداد sign_mode signed_tag_mode = SIGN_VERBATIM;
ساكن تعداد sign_mode signed_commit_mode = SIGN_VERBATIM;
ساكن ثابت حرف *signed_commit_keyid;
ساكن ثابت حرف *signed_tag_keyid;

/* Memory pools */
ساكن بنية mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - حجم(بنية mp_block),
};

/* Atom management */
ساكن unsigned صحيح atom_table_sz = 4451;
ساكن unsigned صحيح atom_cnt;
ساكن بنية atom_str **atom_table;

/* The .pack file being generated */
ساكن بنية pack_idx_option pack_idx_opts;
ساكن unsigned صحيح pack_id;
ساكن بنية hashfile *pack_file;
ساكن بنية packed_git *pack_data;
ساكن بنية packed_git **all_packs;
ساكن off_t pack_size;

/* Table of objects we've written. */
ساكن unsigned صحيح object_entry_alloc = 5000;
ساكن بنية object_entry_pool *blocks;
ساكن بنية hashmap object_table;
ساكن بنية mark_set *marks;
ساكن حرف *export_marks_file;
ساكن حرف *import_marks_file;
ساكن صحيح import_marks_file_from_stream;
ساكن صحيح import_marks_file_ignore_missing;
ساكن صحيح import_marks_file_done;
ساكن صحيح relative_marks_paths;

/* Our last blob */
ساكن بنية last_object last_blob = {
	.data = STRBUF_INIT,
 };

/* Tree management */
ساكن unsigned صحيح tree_entry_alloc = 1000;
ساكن فراغ *avail_tree_entry;
ساكن unsigned صحيح avail_tree_table_sz = 100;
ساكن بنية avail_tree_content **avail_tree_table;
ساكن size_t tree_entry_allocd;
ساكن بنية strbuf old_tree = STRBUF_INIT;
ساكن بنية strbuf new_tree = STRBUF_INIT;

/* Branch data */
ساكن unsigned long max_active_branches = 5;
ساكن unsigned long cur_active_branches;
ساكن unsigned long branch_table_sz = 1039;
ساكن بنية branch **branch_table;
ساكن بنية branch *active_branches;

/* Tag data */
ساكن بنية tag *first_tag;
ساكن بنية tag *last_tag;

/* Input stream parsing */
ساكن whenspec_type whenspec = WHENSPEC_RAW;
ساكن بنية strbuf command_buf = STRBUF_INIT;
ساكن صحيح unread_command_buf;
ساكن بنية recent_command cmd_hist = {
	.prev = &cmd_hist,
	.next = &cmd_hist,
};
ساكن بنية recent_command *cmd_tail = &cmd_hist;
ساكن بنية recent_command *rc_free;
ساكن unsigned صحيح cmd_save = 100;
ساكن uintmax_t next_mark;
ساكن بنية strbuf new_data = STRBUF_INIT;
ساكن صحيح require_explicit_termination;

/* Signal handling */
ساكن متطاير sig_atomic_t checkpoint_requested;

/* Submodule marks */
ساكن بنية string_list sub_marks_from = STRING_LIST_INIT_DUP;
ساكن بنية string_list sub_marks_to = STRING_LIST_INIT_DUP;
ساكن kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
ساكن صحيح cat_blob_fd = STDOUT_FILENO;

/* Command state */
بنية fast_import_state {
	صحيح argc;
	ثابت حرف **argv;
	ثابت حرف *prefix;
	صحيح seen_data_command;
	صحيح allow_unsafe_features;
	بنية option *option;
};

ساكن فراغ fast_import_state_init(بنية fast_import_state *state,
				   صحيح argc, ثابت حرف **argv,
				   ثابت حرف *prefix, بنية option *option)
{
	memset(state, 0, حجم(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

ساكن فراغ parse_argv(بنية fast_import_state *state);
ساكن فراغ parse_get_mark(بنية fast_import_state *state, ثابت حرف *p);
ساكن فراغ parse_cat_blob(بنية fast_import_state *state, ثابت حرف *p);
ساكن فراغ parse_ls(بنية fast_import_state *state,
		     ثابت حرف *p,
		     بنية branch *b);

ساكن فراغ for_each_mark(بنية mark_set *m, uintmax_t base, each_mark_fn_t callback, فراغ *p)
{
	uintmax_t k;
	ܐܢ (m->shift) {
		لكل (k = 0; k < 1024; k++) {
			ܐܢ (m->data.sets[k])
				for_each_mark(m->data.sets[k], base + (k << m->shift), callback, p);
		}
	} ܐܚܪܢܐ {
		لكل (k = 0; k < 1024; k++) {
			ܐܢ (m->data.marked[k])
				callback(base + k, m->data.marked[k], p);
		}
	}
}

ساكن فراغ dump_marks_fn(uintmax_t mark, فراغ *object, فراغ *cbp) {
	بنية object_entry *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

ساكن فراغ write_branch_report(FILE *rpt, بنية branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	ܐܢ (b->active)
		fputs(" active", rpt);
	ܐܢ (b->branch_tree.tree)
		fputs(" loaded", rpt);
	ܐܢ (is_null_oid(&b->branch_tree.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_to_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_to_hex(&b->branch_tree.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_to_hex(&b->branch_tree.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->last_commit);

	fputs("  last pack   : ", rpt);
	ܐܢ (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

ساكن فراغ write_crash_report(ثابت حرف *err)
{
	حرف *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	بنية branch *b;
	unsigned long lu;
	بنية recent_command *rc;

	ܐܢ (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		حرر(loc);
		ܦܢܐ;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(time(عدم), 0, DATE_MODE(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	لكل (rc = cmd_hist.next; rc != &cmd_hist; rc = rc->next) {
		ܐܢ (rc->next == &cmd_hist)
			fputs("* ", rpt);
		ܐܚܪܢܐ
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
	لكل (b = active_branches, lu = 0; b; b = b->active_next_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	لكل (lu = 0; lu < branch_table_sz; lu++) {
		لكل (b = branch_table[lu]; b; b = b->table_next_branch)
			write_branch_report(rpt, b);
	}

	ܐܢ (first_tag) {
		بنية tag *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		لكل (tg = first_tag; tg; tg = tg->next_tag) {
			fputs(oid_to_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	ܐܢ (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	ܐܚܪܢܐ
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	حرر(loc);
}

ساكن فراغ end_packfile(فراغ);
ساكن فراغ unkeep_all_packs(فراغ);
ساكن فراغ dump_marks(فراغ);

ساكن NORETURN فراغ die_nicely(ثابت حرف *err, va_list params)
{
	va_list cp;
	ساكن صحيح zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	ܐܢ (!zombie) {
		حرف message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, حجم(message), err, cp);
		write_crash_report(message);
		end_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	exit(128);
}

#ifndef SIGUSR1	/* Windows, for example */

ساكن فراغ set_checkpoint_signal(فراغ)
{
}

#ܐܚܪܢܐ

ساكن فراغ checkpoint_signal(صحيح signo UNUSED)
{
	checkpoint_requested = 1;
}

ساكن فراغ set_checkpoint_signal(فراغ)
{
	بنية sigaction sa;

	memset(&sa, 0, حجم(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, عدم);
}

#endif

ساكن فراغ alloc_objects(unsigned صحيح cnt)
{
	بنية object_entry_pool *b;

	b = xmalloc(حجم(بنية object_entry_pool)
		+ cnt * حجم(بنية object_entry));
	b->next_pool = blocks;
	b->next_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_count += cnt;
}

ساكن بنية object_entry *new_object(بنية object_id *oid)
{
	بنية object_entry *e;

	ܐܢ (blocks->next_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->next_free++;
	oidcpy(&e->idx.oid, oid);
	ܦܢܐ e;
}

ساكن بنية object_entry *find_object(بنية object_id *oid)
{
	ܦܢܐ hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   بنية object_entry, ent);
}

ساكن بنية object_entry *insert_object(بنية object_id *oid)
{
	بنية object_entry *e;
	unsigned صحيح hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					بنية object_entry, ent);
	ܐܢ (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	ܦܢܐ e;
}

ساكن فراغ invalidate_pack_id(unsigned صحيح id)
{
	unsigned long lu;
	بنية tag *t;
	بنية hashmap_iter iter;
	بنية object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		ܐܢ (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	لكل (lu = 0; lu < branch_table_sz; lu++) {
		بنية branch *b;

		لكل (b = branch_table[lu]; b; b = b->table_next_branch)
			ܐܢ (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	لكل (t = first_tag; t; t = t->next_tag)
		ܐܢ (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

ساكن unsigned صحيح hc_str(ثابت حرف *s, size_t len)
{
	unsigned صحيح r = 0;
	ܟܡܐ (len-- > 0)
		r = r * 31 + *s++;
	ܦܢܐ r;
}

ساكن فراغ insert_mark(بنية mark_set **top, uintmax_t idnum, بنية object_entry *oe)
{
	بنية mark_set *s = *top;

	ܟܡܐ ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_set));
		s->shift = (*top)->shift + 10;
		s->data.sets[0] = *top;
		*top = s;
	}
	ܟܡܐ (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		ܐܢ (!s->data.sets[i]) {
			s->data.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_set));
			s->data.sets[i]->shift = s->shift - 10;
		}
		s = s->data.sets[i];
	}
	ܐܢ (!s->data.marked[idnum])
		marks_set_count++;
	s->data.marked[idnum] = oe;
}

ساكن فراغ *find_mark(بنية mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	بنية object_entry *oe = عدم;
	ܐܢ ((idnum >> s->shift) < 1024) {
		ܟܡܐ (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->data.sets[i];
		}
		ܐܢ (s)
			oe = s->data.marked[idnum];
	}
	ܐܢ (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	ܦܢܐ oe;
}

ساكن بنية atom_str *to_atom(ثابت حرف *s, unsigned short len)
{
	unsigned صحيح hc = hc_str(s, len) % atom_table_sz;
	بنية atom_str *c;

	لكل (c = atom_table[hc]; c; c = c->next_atom)
		ܐܢ (c->str_len == len && !strncmp(s, c->str_dat, len))
			ܦܢܐ c;

	c = mem_pool_alloc(&fi_mem_pool, حجم(بنية atom_str) + len + 1);
	c->str_len = len;
	memcpy(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->next_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	ܦܢܐ c;
}

ساكن بنية branch *lookup_branch(ثابت حرف *name)
{
	unsigned صحيح hc = hc_str(name, strlen(name)) % branch_table_sz;
	بنية branch *b;

	لكل (b = branch_table[hc]; b; b = b->table_next_branch)
		ܐܢ (!strcmp(name, b->name))
			ܦܢܐ b;
	ܦܢܐ عدم;
}

ساكن بنية branch *new_branch(ثابت حرف *name)
{
	unsigned صحيح hc = hc_str(name, strlen(name)) % branch_table_sz;
	بنية branch *b = lookup_branch(name);

	ܐܢ (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	ܐܢ (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_next_branch = branch_table[hc];
	b->branch_tree.versions[0].mode = S_IFDIR;
	b->branch_tree.versions[1].mode = S_IFDIR;
	b->num_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_count++;
	ܦܢܐ b;
}

ساكن unsigned صحيح hc_entries(unsigned صحيح cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	ܦܢܐ cnt < avail_tree_table_sz ? cnt : avail_tree_table_sz - 1;
}

ساكن بنية tree_content *new_tree_content(unsigned صحيح cnt)
{
	بنية avail_tree_content *f, *l = عدم;
	بنية tree_content *t;
	unsigned صحيح hc = hc_entries(cnt);

	لكل (f = avail_tree_table[hc]; f; l = f, f = f->next_avail)
		ܐܢ (f->entry_capacity >= cnt)
			اكسر;

	ܐܢ (f) {
		ܐܢ (l)
			l->next_avail = f->next_avail;
		ܐܚܪܢܐ
			avail_tree_table[hc] = f->next_avail;
	} ܐܚܪܢܐ {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, حجم(*t) + حجم(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (بنية tree_content*)f;
	t->entry_count = 0;
	t->delta_depth = 0;
	ܦܢܐ t;
}

ساكن فراغ release_tree_entry(بنية tree_entry *e);
ساكن فراغ release_tree_content(بنية tree_content *t)
{
	بنية avail_tree_content *f = (بنية avail_tree_content*)t;
	unsigned صحيح hc = hc_entries(f->entry_capacity);
	f->next_avail = avail_tree_table[hc];
	avail_tree_table[hc] = f;
}

ساكن فراغ release_tree_content_recursive(بنية tree_content *t)
{
	unsigned صحيح i;
	لكل (i = 0; i < t->entry_count; i++)
		release_tree_entry(t->entries[i]);
	release_tree_content(t);
}

ساكن بنية tree_content *grow_tree_content(
	بنية tree_content *t,
	صحيح amt)
{
	بنية tree_content *r = new_tree_content(t->entry_count + amt);
	r->entry_count = t->entry_count;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_count);
	release_tree_content(t);
	ܦܢܐ r;
}

ساكن بنية tree_entry *new_tree_entry(فراغ)
{
	بنية tree_entry *e;

	ܐܢ (!avail_tree_entry) {
		unsigned صحيح n = tree_entry_alloc;
		tree_entry_allocd += n * حجم(بنية tree_entry);
		ALLOC_ARRAY(e, n);
		avail_tree_entry = e;
		ܟܡܐ (n-- > 1) {
			*((فراغ**)e) = e + 1;
			e++;
		}
		*((فراغ**)e) = عدم;
	}

	e = avail_tree_entry;
	avail_tree_entry = *((فراغ**)e);
	ܦܢܐ e;
}

ساكن فراغ release_tree_entry(بنية tree_entry *e)
{
	ܐܢ (e->tree)
		release_tree_content_recursive(e->tree);
	*((فراغ**)e) = avail_tree_entry;
	avail_tree_entry = e;
}

ساكن بنية tree_content *dup_tree_content(بنية tree_content *s)
{
	بنية tree_content *d;
	بنية tree_entry *a, *b;
	unsigned صحيح i;

	ܐܢ (!s)
		ܦܢܐ عدم;
	d = new_tree_content(s->entry_count);
	لكل (i = 0; i < s->entry_count; i++) {
		a = s->entries[i];
		b = new_tree_entry();
		memcpy(b, a, حجم(*a));
		ܐܢ (a->tree && is_null_oid(&b->versions[1].oid))
			b->tree = dup_tree_content(a->tree);
		ܐܚܪܢܐ
			b->tree = عدم;
		d->entries[i] = b;
	}
	d->entry_count = s->entry_count;
	d->delta_depth = s->delta_depth;

	ܦܢܐ d;
}

ساكن فراغ start_packfile(فراغ)
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

	pack_data = p;
	pack_size = write_pack_header(pack_file, 0);
	object_count = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

ساكن ثابت حرف *create_index(فراغ)
{
	ثابت حرف *tmpfile;
	بنية pack_idx_entry **idx, **c, **last;
	بنية object_entry *e;
	بنية object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_count);
	c = idx;
	لكل (o = blocks; o; o = o->next_pool)
		لكل (e = o->next_free; e-- != o->entries;)
			ܐܢ (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_count;
	ܐܢ (c != last)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, عدم, idx, object_count,
				 &pack_idx_opts, pack_data->hash);
	حرر(idx);
	ܦܢܐ tmpfile;
}

ساكن حرف *keep_pack(ثابت حرف *curr_index_name)
{
	ساكن ثابت حرف *keep_msg = "fast-import";
	بنية strbuf name = STRBUF_INIT;
	صحيح keep_fd;

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_data->repo,
							    name.buf);
	ܐܢ (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_msg, strlen(keep_msg));
	ܐܢ (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "pack");
	ܐܢ (finalize_object_file(pack_data->repo, pack_data->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "idx");
	ܐܢ (finalize_object_file(pack_data->repo, curr_index_name, name.buf))
		die(_("cannot store index file"));
	حرر((فراغ *)curr_index_name);
	ܦܢܐ strbuf_detach(&name, عدم);
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
	بنية child_process unpack = CHILD_PROCESS_INIT;

	ܐܢ (lseek(p->pack_fd, 0, SEEK_SET) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_push(&unpack.args, "unpack-objects");
	ܐܢ (!show_stats)
		strvec_push(&unpack.args, "-q");

	ܦܢܐ run_command(&unpack);
}

ساكن فراغ end_packfile(فراغ)
{
	ساكن صحيح running;

	ܐܢ (running || !pack_data)
		ܦܢܐ;

	running = 1;
	clear_delta_base_cache();
	ܐܢ (object_count) {
		بنية odb_source_files *files = odb_source_files_downcast(pack_data->repo->objects->source);
		بنية packed_git *new_p;
		بنية object_id cur_pack_oid;
		حرف *idx_name;
		صحيح i;
		بنية branch *b;
		بنية tag *t;

		close_pack_windows(pack_data);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_data->pack_fd,
					 pack_data->hash, pack_data->pack_name,
					 object_count, cur_pack_oid.hash,
					 pack_size);

		ܐܢ (object_count <= unpack_limit) {
			ܐܢ (!loosen_small_pack(pack_data)) {
				invalidate_pack_id(pack_id);
				اذهب discard_pack;
			}
		}

		close(pack_data->pack_fd);
		idx_name = keep_pack(create_index());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		ܐܢ (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		حرر(idx_name);

		/* Print the boundary */
		ܐܢ (pack_edges) {
			fprintf(pack_edges, "%s:", new_p->pack_name);
			لكل (i = 0; i < branch_table_sz; i++) {
				لكل (b = branch_table[i]; b; b = b->table_next_branch) {
					ܐܢ (b->pack_id == pack_id)
						fprintf(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			لكل (t = first_tag; t; t = t->next_tag) {
				ܐܢ (t->pack_id == pack_id)
					fprintf(pack_edges, " %s",
						oid_to_hex(&t->oid));
			}
			fputc('\n', pack_edges);
			fflush(pack_edges);
		}

		pack_id++;
	}
	ܐܚܪܢܐ {
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

ساكن فراغ cycle_packfile(فراغ)
{
	end_packfile();
	start_packfile();
}

ساكن صحيح store_object(
	تعداد object_type type,
	بنية strbuf *dat,
	بنية last_object *last,
	بنية object_id *oidout,
	uintmax_t mark)
{
	بنية odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	فراغ *out, *delta;
	بنية object_entry *e;
	unsigned حرف hdr[96];
	بنية object_id oid;
	unsigned long hdrlen, deltalen = 0;
	بنية git_hash_ctx c;
	git_zstream s;
	بنية repo_config_values *cfg = repo_config_values(the_repository);

	hdrlen = format_object_header((حرف *)hdr, حجم(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	ܐܢ (oidout)
		oidcpy(oidout, &oid);

	e = insert_object(&oid);
	ܐܢ (mark)
		insert_mark(&marks, mark, e);
	ܐܢ (e->idx.offset) {
		duplicate_count_by_type[type]++;
		ܦܢܐ 1;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ܐܢ (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			استمر;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_count_by_type[type]++;
		ܦܢܐ 1;
	}

	ܐܢ (last && last->data.len && last->data.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		size_t deltalen_st;

		delta_count_attempts_by_type[type]++;
		delta = diff_delta(last->data.buf, last->data.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_size_t_to_ulong(deltalen_st);
	} ܐܚܪܢܐ
		delta = عدم;

	git_deflate_init(&s, cfg->pack_compression_level);
	ܐܢ (delta) {
		s.next_in = delta;
		s.avail_in = deltalen;
	} ܐܚܪܢܐ {
		s.next_in = (فراغ *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.next_out = out = xmalloc(s.avail_out);
	ܟܡܐ (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	ܐܢ ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + s.total_out) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + s.total_out) < pack_size) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		ܐܢ (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.next_in = (فراغ *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.next_out = out = xrealloc(out, s.avail_out);
			ܟܡܐ (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_end(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.offset = pack_size;
	object_count++;
	object_count_by_type[type]++;

	crc32_begin(pack_file);

	ܐܢ (delta) {
		off_t ofs = e->idx.offset - last->offset;
		unsigned pos = حجم(hdr) - 1;

		delta_count_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, حجم(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;

		hdr[pos] = ofs & 127;
		ܟܡܐ (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, حجم(hdr) - pos);
		pack_size += حجم(hdr) - pos;
	} ܐܚܪܢܐ {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, حجم(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;
	}

	hashwrite(pack_file, out, s.total_out);
	pack_size += s.total_out;

	e->idx.crc32 = crc32_end(pack_file);

	حرر(out);
	حرر(delta);
	ܐܢ (last) {
		ܐܢ (last->no_swap) {
			last->data = *dat;
		} ܐܚܪܢܐ {
			strbuf_swap(&last->data, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	ܦܢܐ 0;
}

ساكن فراغ truncate_pack(بنية hashfile_checkpoint *checkpoint)
{
	ܐܢ (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_size = checkpoint->offset;
}

ساكن فراغ stream_blob(uintmax_t len, بنية object_id *oidout, uintmax_t mark)
{
	بنية odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	size_t in_sz = 64 * 1024, out_sz = 64 * 1024;
	unsigned حرف *in_buf = xmalloc(in_sz);
	unsigned حرف *out_buf = xmalloc(out_sz);
	بنية object_entry *e;
	بنية object_id oid;
	unsigned long hdrlen;
	off_t offset;
	بنية git_hash_ctx c;
	git_zstream s;
	بنية hashfile_checkpoint checkpoint;
	بنية repo_config_values *cfg = repo_config_values(the_repository);
	صحيح status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	ܐܢ ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + len) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + len) < pack_size)
		cycle_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	offset = checkpoint.offset;

	hdrlen = format_object_header((حرف *)out_buf, out_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, out_buf, hdrlen);

	crc32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_in_pack_object_header(out_buf, out_sz, OBJ_BLOB, len);

	s.next_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	ܟܡܐ (status != Z_STREAM_END) {
		ܐܢ (0 < len && !s.avail_in) {
			size_t cnt = in_sz < len ? in_sz : (size_t)len;
			size_t n = fread(in_buf, 1, cnt, stdin);
			ܐܢ (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.next_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		ܐܢ (!s.avail_out || status == Z_STREAM_END) {
			size_t n = s.next_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_size += n;
			s.next_out = out_buf;
			s.avail_out = out_sz;
		}

		بدل (status) {
		حالة Z_OK:
		حالة Z_BUF_ERROR:
		حالة Z_STREAM_END:
			استمر;
		افتراضي:
			die(_("unexpected deflate failure: %d"), status);
		}
	}
	git_deflate_end(&s);
	git_hash_final_oid(&oid, &c);

	ܐܢ (oidout)
		oidcpy(oidout, &oid);

	e = insert_object(&oid);

	ܐܢ (mark)
		insert_mark(&marks, mark, e);

	ܐܢ (e->idx.offset) {
		duplicate_count_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		اذهب out;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ܐܢ (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			استمر;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_count_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		اذهب out;
	}

	e->depth = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.offset = offset;
	e->idx.crc32 = crc32_end(pack_file);
	object_count++;
	object_count_by_type[OBJ_BLOB]++;

out:
	حرر(in_buf);
	حرر(out_buf);
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
ساكن فراغ *gfi_unpack_entry(
	بنية object_entry *oe,
	unsigned long *sizep)
{
	تعداد object_type type;
	size_t size_st = 0;
	فراغ *data;
	بنية packed_git *p = all_packs[oe->pack_id];
	ܐܢ (p == pack_data && p->pack_size < (pack_size + the_hash_algo->rawsz)) {
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
	ܐܢ (sizep)
		*sizep = cast_size_t_to_ulong(size_st);
	ܦܢܐ data;
}

ساكن فراغ load_tree(بنية tree_entry *root)
{
	بنية object_id *oid = &root->versions[1].oid;
	بنية object_entry *myoe;
	بنية tree_content *t;
	unsigned long size;
	حرف *buf;
	ثابت حرف *c;

	root->tree = t = new_tree_content(8);
	ܐܢ (is_null_oid(oid))
		ܦܢܐ;

	myoe = find_object(oid);
	ܐܢ (myoe && myoe->pack_id != MAX_PACK_ID) {
		ܐܢ (myoe->type != OBJ_TREE)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &size);
		ܐܢ (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} ܐܚܪܢܐ {
		تعداد object_type type;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		ܐܢ (!buf || type != OBJ_TREE)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	ܟܡܐ (c != (buf + size)) {
		بنية tree_entry *e = new_tree_entry();

		ܐܢ (t->entry_count == t->entry_capacity)
			root->tree = t = grow_tree_content(t, t->entry_count);
		t->entries[t->entry_count++] = e;

		e->tree = عدم;
		c = parse_mode(c, &e->versions[1].mode);
		ܐܢ (!c)
			die(_("corrupt mode in %s"), oid_to_hex(oid));
		e->versions[0].mode = e->versions[1].mode;
		e->name = to_atom(c, strlen(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (unsigned حرف *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (unsigned حرف *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	حرر(buf);
}

ساكن صحيح tecmp0 (ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية tree_entry *a = *((بنية tree_entry**)_a);
	بنية tree_entry *b = *((بنية tree_entry**)_b);
	ܦܢܐ base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

ساكن صحيح tecmp1 (ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية tree_entry *a = *((بنية tree_entry**)_a);
	بنية tree_entry *b = *((بنية tree_entry**)_b);
	ܦܢܐ base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

ساكن فراغ mktree(بنية tree_content *t, صحيح v, بنية strbuf *b)
{
	size_t maxlen = 0;
	unsigned صحيح i;

	ܐܢ (!v)
		QSORT(t->entries, t->entry_count, tecmp0);
	ܐܚܪܢܐ
		QSORT(t->entries, t->entry_count, tecmp1);

	لكل (i = 0; i < t->entry_count; i++) {
		ܐܢ (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	لكل (i = 0; i < t->entry_count; i++) {
		بنية tree_entry *e = t->entries[i];
		ܐܢ (!e->versions[v].mode)
			استمر;
		strbuf_addf(b, "%o %s%c",
			(unsigned صحيح)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

ساكن فراغ store_tree(بنية tree_entry *root)
{
	بنية tree_content *t;
	unsigned صحيح i, j, del;
	بنية last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	بنية object_entry *le = عدم;

	ܐܢ (!is_null_oid(&root->versions[1].oid))
		ܦܢܐ;

	ܐܢ (!root->tree)
		load_tree(root);
	t = root->tree;

	لكل (i = 0; i < t->entry_count; i++) {
		ܐܢ (t->entries[i]->tree)
			store_tree(t->entries[i]);
	}

	ܐܢ (!(root->versions[0].mode & NO_DELTA))
		le = find_object(&root->versions[0].oid);
	ܐܢ (S_ISDIR(root->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_tree);
		lo.data = old_tree;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_tree);
	store_object(OBJ_TREE, &new_tree, &lo, &root->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	لكل (i = 0, j = 0, del = 0; i < t->entry_count; i++) {
		بنية tree_entry *e = t->entries[i];
		ܐܢ (e->versions[1].mode) {
			e->versions[0].mode = e->versions[1].mode;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} ܐܚܪܢܐ {
			release_tree_entry(e);
			del++;
		}
	}
	t->entry_count -= del;
}

ساكن فراغ tree_content_replace(
	بنية tree_entry *root,
	ثابت بنية object_id *oid,
	ثابت uint16_t mode,
	بنية tree_content *newtree)
{
	ܐܢ (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&root->versions[0].oid, the_repository->hash_algo);
	oidcpy(&root->versions[1].oid, oid);
	ܐܢ (root->tree)
		release_tree_content_recursive(root->tree);
	root->tree = newtree;
}

ساكن صحيح tree_content_set(
	بنية tree_entry *root,
	ثابت حرف *p,
	ثابت بنية object_id *oid,
	ثابت uint16_t mode,
	بنية tree_content *subtree)
{
	بنية tree_content *t;
	ثابت حرف *slash1;
	unsigned صحيح i, n;
	بنية tree_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	ܐܢ (!n)
		die(_("empty path component found in input"));
	ܐܢ (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	ܐܢ (!root->tree)
		load_tree(root);
	t = root->tree;
	لكل (i = 0; i < t->entry_count; i++) {
		e = t->entries[i];
		ܐܢ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ܐܢ (!*slash1) {
				ܐܢ (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					ܦܢܐ 0;
				e->versions[1].mode = mode;
				oidcpy(&e->versions[1].oid, oid);
				ܐܢ (e->tree)
					release_tree_content_recursive(e->tree);
				e->tree = subtree;

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
				ܐܢ (S_ISDIR(e->versions[0].mode))
					e->versions[0].mode |= NO_DELTA;

				oidclr(&root->versions[1].oid, the_repository->hash_algo);
				ܦܢܐ 1;
			}
			ܐܢ (!S_ISDIR(e->versions[1].mode)) {
				e->tree = new_tree_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			ܐܢ (!e->tree)
				load_tree(e);
			ܐܢ (tree_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&root->versions[1].oid, the_repository->hash_algo);
				ܦܢܐ 1;
			}
			ܦܢܐ 0;
		}
	}

	ܐܢ (t->entry_count == t->entry_capacity)
		root->tree = t = grow_tree_content(t, t->entry_count);
	e = new_tree_entry();
	e->name = to_atom(p, n);
	e->versions[0].mode = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->entry_count++] = e;
	ܐܢ (*slash1) {
		e->tree = new_tree_content(8);
		e->versions[1].mode = S_IFDIR;
		tree_content_set(e, slash1 + 1, oid, mode, subtree);
	} ܐܚܪܢܐ {
		e->tree = subtree;
		e->versions[1].mode = mode;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&root->versions[1].oid, the_repository->hash_algo);
	ܦܢܐ 1;
}

ساكن صحيح tree_content_remove(
	بنية tree_entry *root,
	ثابت حرف *p,
	بنية tree_entry *backup_leaf,
	صحيح allow_root)
{
	بنية tree_content *t;
	ثابت حرف *slash1;
	unsigned صحيح i, n;
	بنية tree_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	ܐܢ (!root->tree)
		load_tree(root);

	ܐܢ (!*p && allow_root) {
		e = root;
		اذهب del_entry;
	}

	t = root->tree;
	لكل (i = 0; i < t->entry_count; i++) {
		e = t->entries[i];
		ܐܢ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ܐܢ (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				ܦܢܐ 1;
			ܐܢ (!*slash1 || !S_ISDIR(e->versions[1].mode))
				اذهب del_entry;
			ܐܢ (!e->tree)
				load_tree(e);
			ܐܢ (tree_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				لكل (n = 0; n < e->tree->entry_count; n++) {
					ܐܢ (e->tree->entries[n]->versions[1].mode) {
						oidclr(&root->versions[1].oid,
						       the_repository->hash_algo);
						ܦܢܐ 1;
					}
				}
				backup_leaf = عدم;
				اذهب del_entry;
			}
			ܦܢܐ 0;
		}
	}
	ܦܢܐ 0;

del_entry:
	ܐܢ (backup_leaf)
		memcpy(backup_leaf, e, حجم(*backup_leaf));
	ܐܚܪܢܐ ܐܢ (e->tree)
		release_tree_content_recursive(e->tree);
	e->tree = عدم;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&root->versions[1].oid, the_repository->hash_algo);
	ܦܢܐ 1;
}

ساكن صحيح tree_content_get(
	بنية tree_entry *root,
	ثابت حرف *p,
	بنية tree_entry *leaf,
	صحيح allow_root)
{
	بنية tree_content *t;
	ثابت حرف *slash1;
	unsigned صحيح i, n;
	بنية tree_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	ܐܢ (!n && !allow_root)
		die(_("empty path component found in input"));

	ܐܢ (!root->tree)
		load_tree(root);

	ܐܢ (!n) {
		e = root;
		اذهب found_entry;
	}

	t = root->tree;
	لكل (i = 0; i < t->entry_count; i++) {
		e = t->entries[i];
		ܐܢ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ܐܢ (!*slash1)
				اذهب found_entry;
			ܐܢ (!S_ISDIR(e->versions[1].mode))
				ܦܢܐ 0;
			ܐܢ (!e->tree)
				load_tree(e);
			ܦܢܐ tree_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	ܦܢܐ 0;

found_entry:
	memcpy(leaf, e, حجم(*leaf));
	ܐܢ (e->tree && is_null_oid(&e->versions[1].oid))
		leaf->tree = dup_tree_content(e->tree);
	ܐܚܪܢܐ
		leaf->tree = عدم;
	ܦܢܐ 1;
}

ساكن صحيح update_branch(بنية branch *b)
{
	ساكن ثابت حرف *msg = "fast-import";
	بنية ref_transaction *transaction;
	بنية object_id old_oid;
	بنية strbuf err = STRBUF_INIT;
	ساكن ثابت حرف *replace_prefix = "refs/replace/";

	ܐܢ (starts_with(b->name, replace_prefix) &&
	    !strcmp(b->name + strlen(replace_prefix),
		    oid_to_hex(&b->oid))) {
		ܐܢ (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				عدم, b->name, عدم, 0);
		ܦܢܐ 0;
	}
	ܐܢ (is_null_oid(&b->oid)) {
		ܐܢ (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					عدم, b->name, عدم, 0);
		ܦܢܐ 0;
	}
	ܐܢ (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	ܐܢ (!force_update && !is_null_oid(&old_oid)) {
		بنية commit *old_cmit, *new_cmit;
		صحيح ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		ܐܢ (!old_cmit || !new_cmit)
			ܦܢܐ error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		ܐܢ (ret < 0)
			exit(128);
		ܐܢ (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			ܦܢܐ -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ܐܢ (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   عدم, عدم, 0, msg, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		ܦܢܐ -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ܦܢܐ 0;
}

ساكن فراغ dump_branches(فراغ)
{
	unsigned صحيح i;
	بنية branch *b;

	لكل (i = 0; i < branch_table_sz; i++) {
		لكل (b = branch_table[i]; b; b = b->table_next_branch)
			failure |= update_branch(b);
	}
}

ساكن فراغ dump_tags(فراغ)
{
	ساكن ثابت حرف *msg = "fast-import";
	بنية tag *t;
	بنية strbuf ref_name = STRBUF_INIT;
	بنية strbuf err = STRBUF_INIT;
	بنية ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ܐܢ (!transaction) {
		failure |= error("%s", err.buf);
		اذهب cleanup;
	}
	لكل (t = first_tag; t; t = t->next_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		ܐܢ (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, عدم, عدم, عدم,
					   0, msg, &err)) {
			failure |= error("%s", err.buf);
			اذهب cleanup;
		}
	}
	ܐܢ (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

ساكن فراغ dump_marks(فراغ)
{
	بنية lock_file mark_lock = LOCK_INIT;
	FILE *f;

	ܐܢ (!export_marks_file || (import_marks_file && !import_marks_file_done))
		ܦܢܐ;

	ܐܢ (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		ܦܢܐ;
	}

	ܐܢ (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		ܦܢܐ;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	ܐܢ (!f) {
		صحيح saved_errno = errno;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		ܦܢܐ;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	ܐܢ (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		ܦܢܐ;
	}
}

ساكن فراغ insert_object_entry(بنية mark_set **s, بنية object_id *oid, uintmax_t mark)
{
	بنية object_entry *e;
	e = find_object(oid);
	ܐܢ (!e) {
		تعداد object_type type = odb_read_object_info(the_repository->objects,
							     oid, عدم);
		ܐܢ (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = insert_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	insert_mark(s, mark, e);
}

ساكن فراغ insert_oid_entry(بنية mark_set **s, بنية object_id *oid, uintmax_t mark)
{
	insert_mark(s, mark, xmemdupz(oid, حجم(*oid)));
}

ساكن فراغ read_mark_file(بنية mark_set **s, FILE *f, mark_set_inserter_t inserter)
{
	حرف line[512];
	ܟܡܐ (fgets(line, حجم(line), f)) {
		uintmax_t mark;
		حرف *end;
		بنية object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, حجم(oid.hash));

		end = strchr(line, '\n');
		ܐܢ (line[0] != ':' || !end)
			die(_("corrupt mark line: %s"), line);
		*end = 0;
		mark = strtoumax(line + 1, &end, 10);
		ܐܢ (!mark || end == line + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), line);
		inserter(s, &oid, mark);
	}
}

ساكن فراغ read_marks(فراغ)
{
	FILE *f = fopen(import_marks_file, "r");
	ܐܢ (f)
		;
	ܐܚܪܢܐ ܐܢ (import_marks_file_ignore_missing && errno == ENOENT)
		اذهب done; /* Marks file does not exist */
	ܐܚܪܢܐ
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, insert_object_entry);
	fclose(f);
done:
	import_marks_file_done = 1;
}


ساكن صحيح read_next_command(بنية fast_import_state *state)
{
	ساكن صحيح stdin_eof = 0;

	ܐܢ (stdin_eof) {
		unread_command_buf = 0;
		ܦܢܐ EOF;
	}

	لكل (;;) {
		ܐܢ (unread_command_buf) {
			unread_command_buf = 0;
		} ܐܚܪܢܐ {
			بنية recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			ܐܢ (stdin_eof)
				ܦܢܐ EOF;

			ܐܢ (!state->seen_data_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			ܐܢ (rc)
				rc_free = rc->next;
			ܐܚܪܢܐ {
				rc = cmd_hist.next;
				cmd_hist.next = rc->next;
				cmd_hist.next->prev = &cmd_hist;
				حرر(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_tail;
			rc->next = cmd_hist.prev;
			rc->prev->next = rc;
			cmd_tail = rc;
		}
		ܐܢ (command_buf.buf[0] == '#')
			استمر;
		ܦܢܐ 0;
	}
}

ساكن فراغ skip_optional_lf(فراغ)
{
	صحيح term_char = fgetc(stdin);
	ܐܢ (term_char != '\n' && term_char != EOF)
		ungetc(term_char, stdin);
}

ساكن فراغ parse_mark(بنية fast_import_state *state)
{
	ثابت حرف *v;
	ܐܢ (skip_prefix(command_buf.buf, "mark :", &v)) {
		next_mark = strtoumax(v, عدم, 10);
		read_next_command(state);
	}
	ܐܚܪܢܐ
		next_mark = 0;
}

ساكن فراغ parse_original_identifier(بنية fast_import_state *state)
{
	ثابت حرف *v;
	ܐܢ (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_next_command(state);
}

ساكن صحيح parse_data(بنية strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	ثابت حرف *data;
	strbuf_reset(sb);

	ܐܢ (!skip_prefix(command_buf.buf, "data ", &data))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	ܐܢ (skip_prefix(data, "<<", &data)) {
		حرف *term = xstrdup(data);
		size_t term_len = command_buf.len - (data - command_buf.buf);

		لكل (;;) {
			ܐܢ (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), term);
			ܐܢ (term_len == command_buf.len
				&& !strcmp(term, command_buf.buf))
				اكسر;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		حرر(term);
	}
	ܐܚܪܢܐ {
		uintmax_t len = strtoumax(data, عدم, 10);
		size_t n = 0, length = (size_t)len;

		ܐܢ (limit && limit < len) {
			*len_res = len;
			ܦܢܐ 0;
		}
		ܐܢ (length < len)
			die(_("data is too large to use in this context"));

		ܟܡܐ (n < length) {
			size_t s = strbuf_fread(sb, length - n, stdin);
			ܐܢ (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(unsigned long)(length - n));
			n += s;
		}
	}

	skip_optional_lf();
	ܦܢܐ 1;
}

ساكن صحيح validate_raw_date(ثابت حرف *src, بنية strbuf *result, صحيح strict)
{
	ثابت حرف *orig_src = src;
	حرف *endp;
	unsigned long num;

	errno = 0;

	num = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	ܐܢ (errno || endp == src || *endp != ' ')
		ܦܢܐ -1;

	src = endp + 1;
	ܐܢ (*src != '-' && *src != '+')
		ܦܢܐ -1;

	num = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	ܐܢ (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < num))             /* parsed a broken timezone */
	   )
		ܦܢܐ -1;

	strbuf_addstr(result, orig_src);
	ܦܢܐ 0;
}

ساكن حرف *parse_ident(ثابت حرف *buf)
{
	ثابت حرف *ltgt;
	size_t name_len;
	بنية strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	ܐܢ (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	ܐܢ (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	ܐܢ (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	ܐܢ (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	ܐܢ (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_add(&ident, buf, name_len);

	بدل (whenspec) {
	حالة WHENSPEC_RAW:
		ܐܢ (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		اكسر;
	حالة WHENSPEC_RAW_PERMISSIVE:
		ܐܢ (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		اكسر;
	حالة WHENSPEC_RFC2822:
		ܐܢ (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		اكسر;
	حالة WHENSPEC_NOW:
		ܐܢ (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		اكسر;
	}

	ܦܢܐ strbuf_detach(&ident, عدم);
}

ساكن فراغ parse_and_store_blob(
	بنية last_object *last,
	بنية object_id *oidout,
	uintmax_t mark)
{
	ساكن بنية strbuf buf = STRBUF_INIT;
	uintmax_t len;

	ܐܢ (parse_data(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	ܐܚܪܢܐ {
		ܐܢ (last) {
			strbuf_release(&last->data);
			last->offset = 0;
			last->depth = 0;
		}
		stream_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

ساكن فراغ parse_new_blob(بنية fast_import_state *state)
{
	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&last_blob, عدم, next_mark);
}

ساكن فراغ unload_one_branch(فراغ)
{
	ܟܡܐ (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = ULONG_MAX;
		بنية branch *e, *l = عدم, *p = عدم;

		لكل (e = active_branches; e; e = e->active_next_branch) {
			ܐܢ (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		ܐܢ (p) {
			e = p->active_next_branch;
			p->active_next_branch = e->active_next_branch;
		} ܐܚܪܢܐ {
			e = active_branches;
			active_branches = e->active_next_branch;
		}
		e->active = 0;
		e->active_next_branch = عدم;
		ܐܢ (e->branch_tree.tree) {
			release_tree_content_recursive(e->branch_tree.tree);
			e->branch_tree.tree = عدم;
		}
		cur_active_branches--;
	}
}

ساكن فراغ load_branch(بنية branch *b)
{
	load_tree(&b->branch_tree);
	ܐܢ (!b->active) {
		b->active = 1;
		b->active_next_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_count++;
	}
}

ساكن unsigned حرف convert_num_notes_to_fanout(uintmax_t num_notes)
{
	unsigned حرف fanout = 0;
	ܟܡܐ ((num_notes >>= 8))
		fanout++;
	ܦܢܐ fanout;
}

ساكن فراغ construct_path_with_fanout(ثابت حرف *hex_sha1,
		unsigned حرف fanout, حرف *path)
{
	unsigned صحيح i = 0, j = 0;
	ܐܢ (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	ܟܡܐ (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	memcpy(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

ساكن uintmax_t do_change_note_fanout(
		بنية tree_entry *orig_root, بنية tree_entry *root,
		حرف *hex_oid, unsigned صحيح hex_oid_len,
		حرف *fullpath, unsigned صحيح fullpath_len,
		unsigned حرف fanout)
{
	بنية tree_content *t;
	بنية tree_entry *e, leaf;
	unsigned صحيح i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t num_notes = 0;
	بنية object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	حرف realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ثابت unsigned hexsz = the_hash_algo->hexsz;

	ܐܢ (!root->tree)
		load_tree(root);
	t = root->tree;

	لكل (i = 0; t && i < t->entry_count; i++) {
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
		ܐܢ (!e->versions[1].mode ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			استمر;

		/* This _may_ be a note entry, or a subdir containing notes */
		memcpy(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		ܐܢ (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		memcpy(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		ܐܢ (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			ܐܢ (fanout == 0xff) {
				/* Counting mode, no rename */
				num_notes++;
				استمر;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			ܐܢ (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				num_notes++;
				استمر;
			}

			/* Rename fullpath to realpath */
			ܐܢ (!tree_content_remove(orig_root, fullpath, &leaf, 0))
				die(_("failed to remove path %s"), fullpath);
			tree_content_set(orig_root, realpath,
				&leaf.versions[1].oid,
				leaf.versions[1].mode,
				leaf.tree);
		} ܐܚܪܢܐ ܐܢ (S_ISDIR(e->versions[1].mode)) {
			/* This is a subdir that may contain note entries */
			num_notes += do_change_note_fanout(orig_root, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = root->tree;
	}
	ܦܢܐ num_notes;
}

ساكن uintmax_t change_note_fanout(بنية tree_entry *root,
		unsigned حرف fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	حرف hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	ܦܢܐ do_change_note_fanout(root, root, hex_oid, 0, path, 0, fanout);
}

ساكن صحيح parse_mapped_oid_hex(ثابت حرف *hex, بنية object_id *oid, ثابت حرف **end)
{
	صحيح algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, حجم(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	ܐܢ (algo == GIT_HASH_UNKNOWN)
		ܦܢܐ -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	ܐܢ (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		ܐܢ (hash_algos[algo].format_id == the_hash_algo->format_id)
			ܦܢܐ 0;
		ܦܢܐ -1;
	}
	oidcpy(oid, kh_value(sub_oid_map, it));
	ܦܢܐ 0;
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
	ܐܢ (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	ܦܢܐ mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
ساكن uintmax_t parse_mark_ref_eol(ثابت حرف *p)
{
	حرف *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	ܐܢ (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	ܦܢܐ mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
ساكن uintmax_t parse_mark_ref_space(ثابت حرف **p)
{
	uintmax_t mark;
	حرف *end;

	mark = parse_mark_ref(*p, &end);
	ܐܢ (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	ܦܢܐ mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
ساكن فراغ parse_path(بنية strbuf *sb, ثابت حرف *p, ثابت حرف **endp,
		صحيح is_last_field, ثابت حرف *field)
{
	ܐܢ (*p == '"') {
		ܐܢ (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), field, command_buf.buf);
		ܐܢ (strlen(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), field, command_buf.buf);
	} ܐܚܪܢܐ {
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
ساكن فراغ parse_path_eol(بنية strbuf *sb, ثابت حرف *p, ثابت حرف *field)
{
	ثابت حرف *end;

	parse_path(sb, p, &end, 1, field);
	ܐܢ (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
ساكن فراغ parse_path_space(بنية strbuf *sb, ثابت حرف *p,
		ثابت حرف **endp, ثابت حرف *field)
{
	parse_path(sb, p, endp, 0, field);
	ܐܢ (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

ساكن فراغ file_change_m(بنية fast_import_state *state,
			  ثابت حرف *p,
			  بنية branch *b)
{
	ساكن بنية strbuf path = STRBUF_INIT;
	بنية object_entry *oe;
	بنية object_id oid;
	uint16_t mode, inline_data = 0;

	p = parse_mode(p, &mode);
	ܐܢ (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	بدل (mode) {
	حالة 0644:
	حالة 0755:
		mode |= S_IFREG;
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

	ܐܢ (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = عدم; /* not used with inline_data, but makes gcc happy */
	} ܐܚܪܢܐ {
		ܐܢ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		ܐܢ (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	ܐܢ (S_ISDIR(mode) &&
	    is_empty_tree_oid(&oid, the_repository->hash_algo) &&
	    *path.buf) {
		tree_content_remove(&b->branch_tree, path.buf, عدم, 0);
		ܦܢܐ;
	}

	ܐܢ (S_ISGITLINK(mode)) {
		ܐܢ (inline_data)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		ܐܚܪܢܐ ܐܢ (oe) {
			ܐܢ (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} ܐܚܪܢܐ ܐܢ (inline_data) {
		ܐܢ (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		ܟܡܐ (read_next_command(state) != EOF) {
			ثابت حرف *v;
			ܐܢ (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			ܐܚܪܢܐ {
				parse_and_store_blob(&last_blob, &oid, 0);
				اكسر;
			}
		}
	} ܐܚܪܢܐ {
		تعداد object_type expected = S_ISDIR(mode) ?
						OBJ_TREE: OBJ_BLOB;
		تعداد object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, عدم);
		ܐܢ (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(mode) ?  _("tree") : _("blob"),
			    command_buf.buf);
		ܐܢ (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	ܐܢ (!*path.buf) {
		tree_content_replace(&b->branch_tree, &oid, mode, عدم);
		ܦܢܐ;
	}

	ܐܢ (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	tree_content_set(&b->branch_tree, path.buf, &oid, mode, عدم);
}

ساكن فراغ file_change_d(ثابت حرف *p, بنية branch *b)
{
	ساكن بنية strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	tree_content_remove(&b->branch_tree, path.buf, عدم, 1);
}

ساكن فراغ file_change_cr(ثابت حرف *p, بنية branch *b, صحيح rename)
{
	ساكن بنية strbuf source = STRBUF_INIT;
	ساكن بنية strbuf dest = STRBUF_INIT;
	بنية tree_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	memset(&leaf, 0, حجم(leaf));
	ܐܢ (rename)
		tree_content_remove(&b->branch_tree, source.buf, &leaf, 1);
	ܐܚܪܢܐ
		tree_content_get(&b->branch_tree, source.buf, &leaf, 1);
	ܐܢ (!leaf.versions[1].mode)
		die(_("path %s not in branch"), source.buf);
	ܐܢ (!*dest.buf) {	/* C "path/to/subdir" "" */
		tree_content_replace(&b->branch_tree,
			&leaf.versions[1].oid,
			leaf.versions[1].mode,
			leaf.tree);
		ܦܢܐ;
	}
	ܐܢ (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	tree_content_set(&b->branch_tree, dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.tree);
}

ساكن فراغ note_change_n(بنية fast_import_state *state,
			  ثابت حرف *p,
			  بنية branch *b,
			  unsigned حرف *old_fanout)
{
	بنية object_entry *oe;
	بنية branch *s;
	بنية object_id oid, commit_oid;
	حرف path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_data = 0;
	unsigned حرف new_fanout;

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
	ܐܢ (b->num_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->num_notes = change_note_fanout(&b->branch_tree, 0xff);
		*old_fanout = convert_num_notes_to_fanout(b->num_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	ܐܢ (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = عدم; /* not used with inline_data, but makes gcc happy */
	} ܐܚܪܢܐ {
		ܐܢ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		ܐܢ (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	ܐܢ (s) {
		ܐܢ (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} ܐܚܪܢܐ ܐܢ (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		بنية object_entry *commit_oe = find_mark(marks, commit_mark);
		ܐܢ (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} ܐܚܪܢܐ ܐܢ (!repo_get_oid(the_repository, p, &commit_oid)) {
		size_t size;
		حرف *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &size,
						   &commit_oid);
		ܐܢ (!buf || size < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		حرر(buf);
	} ܐܚܪܢܐ
		die(_("invalid ref name or SHA1 expression: %s"), p);

	ܐܢ (inline_data) {
		read_next_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} ܐܚܪܢܐ ܐܢ (oe) {
		ܐܢ (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} ܐܚܪܢܐ ܐܢ (!is_null_oid(&oid)) {
		تعداد object_type type = odb_read_object_info(the_repository->objects, &oid,
							عدم);
		ܐܢ (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		ܐܢ (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	ܐܢ (tree_content_remove(&b->branch_tree, path, عدم, 0))
		b->num_notes--;

	ܐܢ (is_null_oid(&oid))
		ܦܢܐ; /* nothing to insert */

	b->num_notes++;
	new_fanout = convert_num_notes_to_fanout(b->num_notes);
	construct_path_with_fanout(oid_to_hex(&commit_oid), new_fanout, path);
	tree_content_set(&b->branch_tree, path, &oid, S_IFREG | 0644, عدم);
}

ساكن فراغ file_change_deleteall(بنية branch *b)
{
	release_tree_content_recursive(b->branch_tree.tree);
	oidclr(&b->branch_tree.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_tree.versions[1].oid, the_repository->hash_algo);
	load_tree(&b->branch_tree);
	b->num_notes = 0;
}

ساكن فراغ parse_from_commit(بنية branch *b, حرف *buf, unsigned long size)
{
	ܐܢ (!buf || size < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	ܐܢ (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_tree.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_tree.versions[0].oid,
	       &b->branch_tree.versions[1].oid);
}

ساكن فراغ parse_from_existing(بنية branch *b)
{
	ܐܢ (is_null_oid(&b->oid)) {
		oidclr(&b->branch_tree.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_tree.versions[1].oid, the_repository->hash_algo);
	} ܐܚܪܢܐ {
		unsigned long size;
		size_t size_st = 0;
		حرف *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &size_st, &b->oid);
		size = cast_size_t_to_ulong(size_st);
		parse_from_commit(b, buf, size);
		حرر(buf);
	}
}

ساكن صحيح parse_objectish(بنية fast_import_state *state,
			   بنية branch *b,
			   ثابت حرف *objectish)
{
	بنية branch *s;
	بنية object_id oid;

	oidcpy(&oid, &b->branch_tree.versions[1].oid);

	s = lookup_branch(objectish);
	ܐܢ (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	ܐܚܪܢܐ ܐܢ (s) {
		بنية object_id *t = &s->branch_tree.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_tree.versions[0].oid, t);
		oidcpy(&b->branch_tree.versions[1].oid, t);
	} ܐܚܪܢܐ ܐܢ (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		بنية object_entry *oe = find_mark(marks, idnum);
		ܐܢ (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		ܐܢ (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			ܐܢ (oe->pack_id != MAX_PACK_ID) {
				unsigned long size;
				حرف *buf = gfi_unpack_entry(oe, &size);
				parse_from_commit(b, buf, size);
				حرر(buf);
			} ܐܚܪܢܐ
				parse_from_existing(b);
		}
	} ܐܚܪܢܐ ܐܢ (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		ܐܢ (is_null_oid(&b->oid))
			b->delete = 1;
	}
	ܐܚܪܢܐ
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	ܐܢ (b->branch_tree.tree && !oideq(&oid, &b->branch_tree.versions[1].oid)) {
		release_tree_content_recursive(b->branch_tree.tree);
		b->branch_tree.tree = عدم;
	}

	read_next_command(state);
	ܦܢܐ 1;
}

ساكن صحيح parse_from(بنية fast_import_state *state, بنية branch *b)
{
	ثابت حرف *from;

	ܐܢ (!skip_prefix(command_buf.buf, "from ", &from))
		ܦܢܐ 0;

	ܦܢܐ parse_objectish(state, b, from);
}

ساكن صحيح parse_objectish_with_prefix(بنية fast_import_state *state,
				       بنية branch *b,
				       ثابت حرف *prefix)
{
	ثابت حرف *base;

	ܐܢ (!skip_prefix(command_buf.buf, prefix, &base))
		ܦܢܐ 0;

	ܦܢܐ parse_objectish(state, b, base);
}

ساكن بنية hash_list *parse_merge(بنية fast_import_state *state,
				     unsigned صحيح *count)
{
	بنية hash_list *list = عدم, **tail = &list, *n;
	ثابت حرف *from;
	بنية branch *s;

	*count = 0;
	ܟܡܐ (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(حجم(*n));
		s = lookup_branch(from);
		ܐܢ (s)
			oidcpy(&n->oid, &s->oid);
		ܐܚܪܢܐ ܐܢ (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			بنية object_entry *oe = find_mark(marks, idnum);
			ܐܢ (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} ܐܚܪܢܐ ܐܢ (!repo_get_oid(the_repository, from, &n->oid)) {
			size_t size;
			حرف *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &size, &n->oid);
			ܐܢ (!buf || size < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			حرر(buf);
		} ܐܚܪܢܐ
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->next = عدم;
		*tail = n;
		tail = &n->next;

		(*count)++;
		read_next_command(state);
	}
	ܦܢܐ list;
}

بنية signature_data {
	حرف *hash_algo;      /* "sha1" or "sha256" */
	حرف *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	بنية strbuf data;   /* The actual signature data */
};

ساكن فراغ parse_one_signature(بنية fast_import_state *state,
				بنية signature_data *sig,
				ثابت حرف *v)
{
	حرف *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	حرف *space = strchr(args, ' ');

	ܐܢ (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), args);
	*space = '\0';

	sig->hash_algo = args;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	ܐܢ (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	ܐܢ (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	ܐܢ (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_next_command(state);
	parse_data(&sig->data, 0, عدم);
}

ساكن فراغ discard_one_signature(بنية fast_import_state *state)
{
	بنية strbuf data = STRBUF_INIT;

	read_next_command(state);
	parse_data(&data, 0, عدم);
	strbuf_release(&data);
}

ساكن فراغ add_gpgsig_to_commit(بنية strbuf *commit_data,
				 ثابت حرف *header,
				 بنية signature_data *sig)
{
	بنية string_list siglines = STRING_LIST_INIT_NODUP;

	ܐܢ (!sig || !sig->hash_algo)
		ܦܢܐ;

	strbuf_addstr(commit_data, header);
	string_list_split_in_place(&siglines, sig->data.buf, "\n", -1);
	strbuf_add_separated_string_list(commit_data, "\n ", &siglines);
	strbuf_addch(commit_data, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->data);
	حرر(sig->hash_algo);
}

ساكن فراغ store_signature(بنية signature_data *stored_sig,
			    بنية signature_data *new_sig,
			    ثابت حرف *hash_type)
{
	ܐܢ (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->data);
		حرر(new_sig->hash_algo);
	} ܐܚܪܢܐ {
		*stored_sig = *new_sig;
	}
}

ساكن فراغ import_one_signature(بنية fast_import_state *state,
				 بنية signature_data *sig_sha1,
				 بنية signature_data *sig_sha256,
				 ثابت حرف *v)
{
	بنية signature_data sig = { عدم, عدم, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	ܐܢ (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	ܐܚܪܢܐ ܐܢ (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	ܐܚܪܢܐ
		die(_("parse_one_signature() returned unknown hash algo"));
}

ساكن فراغ finalize_commit_buffer(بنية strbuf *new_data,
				   بنية signature_data *sig_sha1,
				   بنية signature_data *sig_sha256,
				   بنية strbuf *msg)
{
	add_gpgsig_to_commit(new_data, "gpgsig ", sig_sha1);
	add_gpgsig_to_commit(new_data, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_data, '\n');
	strbuf_addbuf(new_data, msg);
}

ساكن فراغ warn_invalid_signature(بنية signature_check *check,
				   ثابت حرف *msg, تعداد sign_mode mode)
{
	ثابت حرف *signer = check->signer ? check->signer : _("unknown");
	ثابت حرف *subject;
	صحيح subject_len = find_commit_subject(msg, &subject);

	بدل (mode) {
	حالة SIGN_STRIP_IF_INVALID:
		ܐܢ (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		ܐܚܪܢܐ ܐܢ (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		ܐܚܪܢܐ
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		اكسر;
	حالة SIGN_SIGN_IF_INVALID:
		ܐܢ (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		ܐܚܪܢܐ ܐܢ (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		ܐܚܪܢܐ
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		اكسر;
	افتراضي:
		BUG("unsupported signing mode");
	}
}

ساكن فراغ handle_signature_if_invalid(بنية strbuf *new_data,
					بنية signature_data *sig_sha1,
					بنية signature_data *sig_sha256,
					بنية strbuf *msg,
					تعداد sign_mode mode)
{
	بنية strbuf tmp_buf = STRBUF_INIT;
	بنية signature_check signature_check = { 0 };
	صحيح ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_data);
	finalize_commit_buffer(&tmp_buf, sig_sha1, sig_sha256, msg);
	ret = verify_commit_buffer(tmp_buf.buf, tmp_buf.len, &signature_check);

	ܐܢ (ret) {
		ܐܢ (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, msg->buf, mode);

		ܐܢ (mode == SIGN_SIGN_IF_INVALID) {
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
			ܐܢ (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			ܐܢ (sign_buffer(&payload, &signature, signed_commit_keyid,
					SIGN_BUFFER_USE_DEFAULT_KEY))
				die(_("failed to sign commit object"));
			add_header_signature(new_data, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_buffer(new_data, عدم, عدم, msg);
	} ܐܚܪܢܐ {
		strbuf_swap(new_data, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

ساكن فراغ parse_new_commit(بنية fast_import_state *state, ثابت حرف *arg)
{
	ساكن بنية strbuf msg = STRBUF_INIT;
	بنية signature_data sig_sha1 = { عدم, عدم, STRBUF_INIT };
	بنية signature_data sig_sha256 = { عدم, عدم, STRBUF_INIT };
	بنية branch *b;
	حرف *author = عدم;
	حرف *committer = عدم;
	حرف *encoding = عدم;
	بنية hash_list *merge_list = عدم;
	unsigned صحيح merge_count;
	unsigned حرف prev_fanout, new_fanout;
	ثابت حرف *v;

	b = lookup_branch(arg);
	ܐܢ (!b)
		b = new_branch(arg);

	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	ܐܢ (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_next_command(state);
	}
	ܐܢ (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_next_command(state);
	}
	ܐܢ (!committer)
		die(_("expected committer but didn't get one"));

	ܟܡܐ (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		بدل (signed_commit_mode) {

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
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_next_command(state);
	}

	ܐܢ (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_next_command(state);
	}
	parse_data(&msg, 0, عدم);
	read_next_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_count);

	/* ensure the branch is active/loaded */
	ܐܢ (!b->branch_tree.tree || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	prev_fanout = convert_num_notes_to_fanout(b->num_notes);

	/* file_change* */
	ܟܡܐ (command_buf.len > 0) {
		ܐܢ (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		ܐܚܪܢܐ ܐܢ (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		ܐܚܪܢܐ {
			unread_command_buf = 1;
			اكسر;
		}
		ܐܢ (read_next_command(state) == EOF)
			اكسر;
	}

	new_fanout = convert_num_notes_to_fanout(b->num_notes);
	ܐܢ (new_fanout != prev_fanout)
		b->num_notes = change_note_fanout(&b->branch_tree, new_fanout);

	/* build the tree and the commit */
	store_tree(&b->branch_tree);
	oidcpy(&b->branch_tree.versions[0].oid,
	       &b->branch_tree.versions[1].oid);

	strbuf_reset(&new_data);
	strbuf_addf(&new_data, "tree %s\n",
		oid_to_hex(&b->branch_tree.versions[1].oid));
	ܐܢ (!is_null_oid(&b->oid))
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&b->oid));
	ܟܡܐ (merge_list) {
		بنية hash_list *next = merge_list->next;
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&merge_list->oid));
		حرر(merge_list);
		merge_list = next;
	}
	strbuf_addf(&new_data,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	ܐܢ (encoding)
		strbuf_addf(&new_data,
			"encoding %s\n",
			encoding);

	ܐܢ ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
	     signed_commit_mode == SIGN_SIGN_IF_INVALID ||
	     signed_commit_mode == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_data, &sig_sha1, &sig_sha256,
					    &msg, signed_commit_mode);
	ܐܚܪܢܐ
		finalize_commit_buffer(&new_data, &sig_sha1, &sig_sha256, &msg);

	حرر(author);
	حرر(committer);
	حرر(encoding);

	ܐܢ (!store_object(OBJ_COMMIT, &new_data, عدم, &b->oid, next_mark))
		b->pack_id = pack_id;
	b->last_commit = object_count_by_type[OBJ_COMMIT];
}

ساكن فراغ handle_tag_signature_if_invalid(بنية strbuf *buf,
					    بنية strbuf *msg,
					    size_t sig_offset)
{
	بنية strbuf signature = STRBUF_INIT;
	بنية strbuf payload = STRBUF_INIT;
	بنية signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_add(&payload, msg->buf, sig_offset);
	strbuf_add(&signature, msg->buf + sig_offset, msg->len - sig_offset);

	sigc.payload_type = SIGNATURE_PAYLOAD_TAG;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	ܐܢ (!check_signature(&sigc, signature.buf, signature.len))
		اذهب out;

	ܐܢ (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(msg, sig_offset);

	ܐܢ (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = عدم;
		strbuf_reset(&signature);

		ܐܢ (sign_buffer(&payload, &signature, signed_tag_keyid,
				SIGN_BUFFER_USE_DEFAULT_KEY))
			die(_("failed to sign tag object"));

		strbuf_addbuf(msg, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

ساكن فراغ handle_tag_signature(بنية strbuf *buf, بنية strbuf *msg, ثابت حرف *name)
{
	size_t sig_offset = parse_signed_buffer(msg->buf, msg->len);

	/* If there is no signature, there is nothing to do. */
	ܐܢ (sig_offset >= msg->len)
		ܦܢܐ;

	بدل (signed_tag_mode) {

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
		strbuf_setlen(msg, sig_offset);
		اكسر;
	حالة SIGN_ABORT_IF_INVALID:
	حالة SIGN_SIGN_IF_INVALID:
	حالة SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, msg, sig_offset);
		اكسر;

	/* Third, aborting modes */
	حالة SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	افتراضي:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

ساكن فراغ parse_new_tag(بنية fast_import_state *state, ثابت حرف *arg)
{
	ساكن بنية strbuf msg = STRBUF_INIT;
	ثابت حرف *from;
	حرف *tagger;
	بنية branch *s;
	بنية tag *t;
	uintmax_t from_mark = 0;
	بنية object_id oid;
	تعداد object_type type;
	ثابت حرف *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	ܐܢ (last_tag)
		last_tag->next_tag = t;
	ܐܚܪܢܐ
		first_tag = t;
	last_tag = t;
	read_next_command(state);
	parse_mark(state);

	/* from ... */
	ܐܢ (!skip_prefix(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	ܐܢ (s) {
		ܐܢ (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} ܐܚܪܢܐ ܐܢ (*from == ':') {
		بنية object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} ܐܚܪܢܐ ܐܢ (!repo_get_oid(the_repository, from, &oid)) {
		بنية object_entry *oe = find_object(&oid);
		ܐܢ (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, عدم);
			ܐܢ (type < 0)
				die(_("not a valid object: %s"), from);
		} ܐܚܪܢܐ
			type = oe->type;
	} ܐܚܪܢܐ
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_next_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	ܐܢ (skip_prefix(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_next_command(state);
	} ܐܚܪܢܐ
		tagger = عدم;

	/* tag payload/message */
	parse_data(&msg, 0, عدم);

	/* build the tag object */
	strbuf_reset(&new_data);

	strbuf_addf(&new_data,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_to_hex(&oid), type_name(type), t->name);
	ܐܢ (tagger)
		strbuf_addf(&new_data,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_data, &msg, t->name);

	strbuf_addch(&new_data, '\n');
	strbuf_addbuf(&new_data, &msg);
	حرر(tagger);

	ܐܢ (store_object(OBJ_TAG, &new_data, عدم, &t->oid, next_mark))
		t->pack_id = MAX_PACK_ID;
	ܐܚܪܢܐ
		t->pack_id = pack_id;
}

ساكن فراغ parse_reset_branch(بنية fast_import_state *state, ثابت حرف *arg)
{
	بنية branch *b;
	ثابت حرف *tag_name;

	b = lookup_branch(arg);
	ܐܢ (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_tree.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_tree.versions[1].oid, the_repository->hash_algo);
		ܐܢ (b->branch_tree.tree) {
			release_tree_content_recursive(b->branch_tree.tree);
			b->branch_tree.tree = عدم;
		}
	}
	ܐܚܪܢܐ
		b = new_branch(arg);
	read_next_command(state);
	parse_from(state, b);
	ܐܢ (b->delete && skip_prefix(b->name, "refs/tags/", &tag_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		بنية tag *t, *prev = عدم;
		لكل (t = first_tag; t; t = t->next_tag) {
			ܐܢ (!strcmp(t->name, tag_name))
				اكسر;
			prev = t;
		}
		ܐܢ (t) {
			ܐܢ (prev)
				prev->next_tag = t->next_tag;
			ܐܚܪܢܐ
				first_tag = t->next_tag;
			ܐܢ (!t->next_tag)
				last_tag = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	ܐܢ (command_buf.len > 0)
		unread_command_buf = 1;
}

ساكن فراغ cat_blob_write(ثابت حرف *buf, unsigned long size)
{
	ܐܢ (write_in_full(cat_blob_fd, buf, size) < 0)
		die_errno(_("write to frontend failed"));
}

ساكن فراغ cat_blob(بنية object_entry *oe, بنية object_id *oid)
{
	بنية strbuf line = STRBUF_INIT;
	بنية iovec iov[3];
	unsigned long size;
	تعداد object_type type = 0;
	حرف *buf;

	ܐܢ (!oe || oe->pack_id == MAX_PACK_ID) {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
	} ܐܚܪܢܐ {
		type = oe->type;
		buf = gfi_unpack_entry(oe, &size);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	ܐܢ (type <= 0) {
		strbuf_reset(&line);
		strbuf_addf(&line, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(line.buf, line.len);
		strbuf_release(&line);
		حرر(buf);
		ܦܢܐ;
	}
	ܐܢ (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	ܐܢ (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_to_hex(oid), type_name(type));
	strbuf_reset(&line);
	strbuf_addf(&line, "%s %s %"PRIuMAX"\n", oid_to_hex(oid),
		    type_name(type), (uintmax_t)size);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_base = line.buf;
	iov[0].iov_len = line.len;
	iov[1].iov_base = buf;
	iov[1].iov_len = size;
	iov[2].iov_base = (فراغ *) "\n";
	iov[2].iov_len = 1;

	ܐܢ (writev_in_full(cat_blob_fd, iov, ARRAY_SIZE(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&line);
	ܐܢ (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.data, buf, size, size + 1);
		last_blob.depth = oe->depth;
	} ܐܚܪܢܐ
		حرر(buf);
}

ساكن فراغ parse_get_mark(بنية fast_import_state *state UNUSED,
			   ثابت حرف *p)
{
	بنية object_entry *oe;
	حرف output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	ܐܢ (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	ܐܢ (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, حجم(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

ساكن فراغ parse_cat_blob(بنية fast_import_state *state UNUSED,
			   ثابت حرف *p)
{
	بنية object_entry *oe;
	بنية object_id oid;

	/* cat-blob SP <object> LF */
	ܐܢ (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		ܐܢ (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} ܐܚܪܢܐ {
		ܐܢ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		ܐܢ (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

ساكن بنية object_entry *dereference(بنية object_entry *oe,
					بنية object_id *oid)
{
	unsigned long size;
	حرف *buf = عدم;
	ثابت unsigned hexsz = the_hash_algo->hexsz;

	ܐܢ (!oe) {
		تعداد object_type type = odb_read_object_info(the_repository->objects,
							     oid, عدم);
		ܐܢ (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = insert_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	بدل (oe->type) {
	حالة OBJ_TREE:	/* easy case. */
		ܦܢܐ oe;
	حالة OBJ_COMMIT:
	حالة OBJ_TAG:
		اكسر;
	افتراضي:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	ܐܢ (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &size);
	} ܐܚܪܢܐ {
		تعداد object_type unused;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &size_st);
		size = cast_size_t_to_ulong(size_st);
	}
	ܐܢ (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	بدل (oe->type) {
	حالة OBJ_TAG:
		ܐܢ (size < hexsz + strlen("object ") ||
		    get_oid_hex(buf + strlen("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		اكسر;
	حالة OBJ_COMMIT:
		ܐܢ (size < hexsz + strlen("tree ") ||
		    get_oid_hex(buf + strlen("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	حرر(buf);
	ܦܢܐ find_object(oid);
}

ساكن فراغ insert_mapped_mark(uintmax_t mark, فراغ *object, فراغ *cbp)
{
	بنية object_id *fromoid = object;
	بنية object_id *tooid = find_mark(cbp, mark);
	صحيح ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	ܐܢ (ret == 0)
		ܦܢܐ;
	kh_value(sub_oid_map, it) = tooid;
}

ساكن فراغ build_mark_map_one(بنية mark_set *from, بنية mark_set *to)
{
	for_each_mark(from, 0, insert_mapped_mark, to);
}

ساكن فراغ build_mark_map(بنية string_list *from, بنية string_list *to)
{
	بنية string_list_item *fromp, *top;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		top = string_list_lookup(to, fromp->string);
		ܐܢ (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} ܐܚܪܢܐ ܐܢ (!top || !top->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, top->util);
	}
}

ساكن بنية object_entry *parse_treeish_dataref(ثابت حرف **p)
{
	بنية object_id oid;
	بنية object_entry *e;

	ܐܢ (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		ܐܢ (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} ܐܚܪܢܐ {	/* <sha1> */
		ܐܢ (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		ܐܢ (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	ܟܡܐ (!e || e->type != OBJ_TREE)
		e = dereference(e, &oid);
	ܦܢܐ e;
}

ساكن فراغ print_ls(صحيح mode, ثابت unsigned حرف *hash, ثابت حرف *path)
{
	ساكن بنية strbuf line = STRBUF_INIT;

	/* See show_tree(). */
	ثابت حرف *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? tree_type :
		blob_type;

	ܐܢ (!mode) {
		/* missing SP path LF */
		strbuf_reset(&line);
		strbuf_addstr(&line, "missing ");
		quote_c_style(path, &line, عدم, 0);
		strbuf_addch(&line, '\n');
	} ܐܚܪܢܐ {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&line);
		strbuf_addf(&line, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &line, عدم, 0);
		strbuf_addch(&line, '\n');
	}
	cat_blob_write(line.buf, line.len);
}

ساكن فراغ parse_ls(بنية fast_import_state *state UNUSED,
		     ثابت حرف *p,
		     بنية branch *b)
{
	ساكن بنية strbuf path = STRBUF_INIT;
	بنية tree_entry *root = عدم;
	بنية tree_entry leaf = {عدم};

	/* ls SP (<tree-ish> SP)? <path> */
	ܐܢ (*p == '"') {
		ܐܢ (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		root = &b->branch_tree;
	} ܐܚܪܢܐ {
		بنية object_entry *e = parse_treeish_dataref(&p);
		root = new_tree_entry();
		oidcpy(&root->versions[1].oid, &e->idx.oid);
		ܐܢ (!is_null_oid(&root->versions[1].oid))
			root->versions[1].mode = S_IFDIR;
		load_tree(root);
	}
	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	tree_content_get(root, path.buf, &leaf, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	ܐܢ (S_ISDIR(leaf.versions[1].mode))
		store_tree(&leaf);

	print_ls(leaf.versions[1].mode, leaf.versions[1].oid.hash, path.buf);
	ܐܢ (leaf.tree)
		release_tree_content_recursive(leaf.tree);
	ܐܢ (!b || root != &b->branch_tree)
		release_tree_entry(root);
}

ساكن فراغ checkpoint(فراغ)
{
	checkpoint_requested = 0;
	ܐܢ (object_count) {
		cycle_packfile();
	}
	dump_branches();
	dump_tags();
	dump_marks();
}

ساكن فراغ parse_checkpoint(بنية fast_import_state *state UNUSED)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

ساكن فراغ parse_progress(بنية fast_import_state *state UNUSED)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

ساكن فراغ parse_alias(بنية fast_import_state *state)
{
	بنية object_entry *e;
	بنية branch b;

	skip_optional_lf();
	read_next_command(state);

	/* mark ... */
	parse_mark(state);
	ܐܢ (!next_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, حجم(b));
	ܐܢ (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	أكد(e);
	insert_mark(&marks, next_mark, e);
}

ساكن حرف* make_fast_import_path(بنية fast_import_state *state,
				   ثابت حرف *path)
{
	ܐܢ (!relative_marks_paths || is_absolute_path(path))
		ܦܢܐ prefix_filename(state->prefix, path);
	ܦܢܐ repo_git_path(the_repository, "info/fast-import/%s", path);
}

ساكن فراغ option_import_marks(بنية fast_import_state *state,
				ثابت حرف *marks,
				صحيح from_stream,
				صحيح ignore_missing)
{
	ܐܢ (import_marks_file) {
		ܐܢ (from_stream)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		ܐܢ(!import_marks_file_from_stream)
			read_marks();
	}

	حرر(import_marks_file);
	import_marks_file = make_fast_import_path(state, marks);
	import_marks_file_from_stream = from_stream;
	import_marks_file_ignore_missing = ignore_missing;
}

ساكن فراغ option_date_format(ثابت حرف *fmt)
{
	ܐܢ (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	ܐܚܪܢܐ ܐܢ (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	ܐܚܪܢܐ ܐܢ (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	ܐܚܪܢܐ ܐܢ (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	ܐܚܪܢܐ
		die(_("unknown --date-format argument %s"), fmt);
}

ساكن unsigned long ulong_arg(ثابت حرف *option, ثابت حرف *arg)
{
	حرف *endptr;
	unsigned long rv = strtoul(arg, &endptr, 0);
	ܐܢ (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	ܦܢܐ rv;
}

ساكن فراغ option_depth(ثابت حرف *depth)
{
	max_depth = ulong_arg("--depth", depth);
	ܐܢ (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

ساكن فراغ option_active_branches(ثابت حرف *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

ساكن فراغ option_export_marks(بنية fast_import_state *state,
				ثابت حرف *marks)
{
	حرر(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

ساكن فراغ option_cat_blob_fd(بنية fast_import_state *state UNUSED,
			       ثابت حرف *fd)
{
	unsigned long n = ulong_arg("--cat-blob-fd", fd);
	ܐܢ (n > (unsigned long) INT_MAX)
		die(_("--cat-blob-fd cannot exceed %d"), INT_MAX);
	cat_blob_fd = (صحيح) n;
}

ساكن فراغ option_export_pack_edges(بنية fast_import_state *state,
				     ثابت حرف *edges)
{
	حرف *fn = prefix_filename(state->prefix, edges);
	ܐܢ (pack_edges)
		fclose(pack_edges);
	pack_edges = xfopen(fn, "a");
	حرر(fn);
}

ساكن فراغ option_rewrite_submodules(بنية fast_import_state *state,
				      ثابت حرف *arg,
				      بنية string_list *list)
{
	بنية mark_set *ms;
	FILE *fp;
	حرف *s = xstrdup(arg);
	حرف *f = strchr(s, ':');
	ܐܢ (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = prefix_filename(state->prefix, f);
	fp = fopen(f, "r");
	ܐܢ (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, insert_oid_entry);
	fclose(fp);
	حرر(f);

	string_list_insert(list, s)->util = ms;

	حرر(s);
}

ساكن فراغ option_max_pack_size(ثابت حرف *arg)
{
	unsigned long v;

	ܐܢ (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	ܐܢ (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} ܐܚܪܢܐ ܐܢ (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

ساكن فراغ option_big_file_threshold(ثابت حرف *arg)
{
	unsigned long v;

	ܐܢ (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

ساكن فراغ option_signed_commits(ثابت حرف *arg)
{
	ܐܢ (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

ساكن فراغ option_signed_tags(ثابت حرف *arg)
{
	ܐܢ (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

ساكن فراغ option_quiet(فراغ)
{
	show_stats = 0;
	quiet = 1;
}

ساكن صحيح parse_one_option(بنية fast_import_state *state, ثابت حرف *option)
{
	ܐܢ (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_size(option);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(option, "big-file-threshold=", &option)) {
		option_big_file_threshold(option);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(option, "depth=", &option)) {
		option_depth(option);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(option, "active-branches=", &option)) {
		option_active_branches(option);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(option, "export-pack-edges=", &option)) {
		option_export_pack_edges(state, option);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(option, "signed-tags=", &option)) {
		option_signed_tags(option);
	} ܐܚܪܢܐ ܐܢ (!strcmp(option, "quiet")) {
		option_quiet();
	} ܐܚܪܢܐ ܐܢ (!strcmp(option, "stats")) {
		show_stats = 1;
	} ܐܚܪܢܐ ܐܢ (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} ܐܚܪܢܐ {
		ܦܢܐ 0;
	}

	ܦܢܐ 1;
}

ساكن فراغ check_unsafe_feature(بنية fast_import_state *state,
				 ثابت حرف *feature)
{
	ܐܢ (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

ساكن صحيح parse_one_feature(بنية fast_import_state *state,
			     ثابت حرف *feature)
{
	ثابت حرف *arg;

	ܐܢ (skip_prefix(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} ܐܚܪܢܐ ܐܢ (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} ܐܚܪܢܐ ܐܢ (skip_prefix(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_to);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} ܐܚܪܢܐ ܐܢ (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} ܐܚܪܢܐ ܐܢ (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} ܐܚܪܢܐ ܐܢ (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} ܐܚܪܢܐ ܐܢ (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} ܐܚܪܢܐ ܐܢ (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} ܐܚܪܢܐ ܐܢ (!strcmp(feature, "force")) {
		force_update = 1;
	} ܐܚܪܢܐ ܐܢ (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} ܐܚܪܢܐ {
		ܦܢܐ 0;
	}

	ܦܢܐ 1;
}

ساكن فراغ parse_feature(بنية fast_import_state *state, ثابت حرف *feature)
{
	ܐܢ (state->seen_data_command)
		die(_("got feature command '%s' after data command"), feature);

	ܐܢ (parse_one_feature(state, feature))
		ܦܢܐ;

	die(_("this version of fast-import does not support feature %s."), feature);
}

ساكن فراغ parse_option(بنية fast_import_state *state, ثابت حرف *option)
{
	ܐܢ (state->seen_data_command)
		die(_("got option command '%s' after data command"), option);

	ܐܢ (parse_one_option(state, option))
		ܦܢܐ;

	die(_("this version of fast-import does not support option: %s"), option);
}

ساكن فراغ git_pack_config(فراغ)
{
	صحيح indexversion_value;
	صحيح limit;
	unsigned long packsizelimit_value;

	ܐܢ (!repo_config_get_ulong(the_repository, "pack.depth", &max_depth)) {
		ܐܢ (max_depth > MAX_DEPTH)
			max_depth = MAX_DEPTH;
	}
	ܐܢ (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_value)) {
		pack_idx_opts.version = indexversion_value;
		ܐܢ (pack_idx_opts.version > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.version);
	}
	ܐܢ (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_value))
		max_packsize = packsizelimit_value;

	ܐܢ (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &limit))
		unpack_limit = limit;
	ܐܚܪܢܐ ܐܢ (!repo_config_get_int(the_repository, "transfer.unpacklimit", &limit))
		unpack_limit = limit;

	repo_config(the_repository, git_default_config, عدم);
}

ساكن ثابت حرف *ثابت fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	عدم
};

ساكن فراغ parse_argv(بنية fast_import_state *state)
{
	صحيح argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	ܐܢ (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_data_command = 1;
	ܐܢ (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

ساكن صحيح option_parse_date_format(ثابت بنية option *opt UNUSED,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_export_pack_edges(ثابت بنية option *opt,
					  ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->value, arg);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_max_pack_size(ثابت بنية option *opt UNUSED,
				      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_size(arg);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_big_file_threshold(ثابت بنية option *opt UNUSED,
					   ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_signed_commits(ثابت بنية option *opt UNUSED,
				       ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_signed_tags(ثابت بنية option *opt UNUSED,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_rewrite_submodules_from(ثابت بنية option *opt,
						ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->value, arg, &sub_marks_from);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_rewrite_submodules_to(ثابت بنية option *opt,
					      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->value, arg, &sub_marks_to);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_cat_blob_fd(ثابت بنية option *opt,
				    ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->value, arg);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_import_marks(ثابت بنية option *opt,
				     ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->value, arg, 0, 0);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_import_marks_if_exists(ثابت بنية option *opt,
					       ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->value, arg, 0, 1);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_export_marks(ثابت بنية option *opt,
				     ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->value, arg);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_depth(ثابت بنية option *opt UNUSED,
			      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_active_branches(ثابت بنية option *opt UNUSED,
					ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	ܦܢܐ 0;
}

ساكن صحيح option_parse_quiet(ثابت بنية option *opt UNUSED,
			      ثابت حرف *arg UNUSED, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	ܦܢܐ 0;
}

صحيح cmd_fast_import(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *prefix,
		    بنية repository *repo)
{
	بنية fast_import_state state;

	بنية option fast_import_options[] = {
		OPT_GROUP(N_("Common")),
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
			       PARSE_OPT_NONEG, option_parse_max_pack_size),
		OPT_CALLBACK_F(0, "big-file-threshold", عدم, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_threshold),
		OPT_CALLBACK_F(0, "depth", عدم, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_depth),
		OPT_CALLBACK_F(0, "active-branches", عدم, N_("n"),
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
		OPT_CALLBACK_F(0, "signed-commits", عدم, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", عدم, N_("mode"),
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
	CALLOC_ARRAY(avail_tree_table, avail_tree_table_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, حجم(بنية mark_set));

	hashmap_init(&object_table, object_entry_hashcmp, عدم, 0);

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
		ܐܢ (*arg != '-' || !strcmp(arg, "--"))
			اكسر;
		ܐܢ (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * حجم(*rc_free));
	لكل (unsigned صحيح i = 0; i < (cmd_save - 1); i++)
		rc_free[i].next = &rc_free[i + 1];
	rc_free[cmd_save - 1].next = عدم;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	ܟܡܐ (read_next_command(&state) != EOF) {
		ثابت حرف *v;
		ܐܢ (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, عدم);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		ܐܚܪܢܐ ܐܢ (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		ܐܚܪܢܐ ܐܢ (!strcmp("done", command_buf.buf))
			اكسر;
		ܐܚܪܢܐ ܐܢ (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		ܐܚܪܢܐ ܐܢ (starts_with(command_buf.buf, "progress "))
			parse_progress(&state);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		ܐܚܪܢܐ ܐܢ (skip_prefix(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		ܐܚܪܢܐ ܐܢ (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		ܐܚܪܢܐ
			die(_("unsupported command: %s"), command_buf.buf);

		ܐܢ (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	ܐܢ (!state.seen_data_command)
		parse_argv(&state);

	ܐܢ (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	end_packfile();

	dump_branches();
	dump_tags();
	unkeep_all_packs();
	dump_marks();

	ܐܢ (pack_edges)
		fclose(pack_edges);

	ܐܢ (show_stats) {
		uintmax_t total_count = 0, duplicate_count = 0;
		لكل (size_t i = 0; i < ARRAY_SIZE(object_count_by_type); i++)
			total_count += object_count_by_type[i];
		لكل (size_t i = 0; i < ARRAY_SIZE(duplicate_count_by_type); i++)
			duplicate_count += duplicate_count_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_count);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", total_count, duplicate_count);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_count_by_type[OBJ_BLOB], duplicate_count_by_type[OBJ_BLOB], delta_count_by_type[OBJ_BLOB], delta_count_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_count_by_type[OBJ_TREE], duplicate_count_by_type[OBJ_TREE], delta_count_by_type[OBJ_TREE], delta_count_attempts_by_type[OBJ_TREE]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_count_by_type[OBJ_COMMIT], duplicate_count_by_type[OBJ_COMMIT], delta_count_by_type[OBJ_COMMIT], delta_count_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_count_by_type[OBJ_TAG], duplicate_count_by_type[OBJ_TAG], delta_count_by_type[OBJ_TAG], delta_count_attempts_by_type[OBJ_TAG]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_count, branch_load_count);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->shift) * 1024, marks_set_count);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (tree_entry_allocd + fi_mem_pool.pool_alloc + alloc_count*حجم(بنية object_entry))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (unsigned long)((tree_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_count*حجم(بنية object_entry))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	ܦܢܐ failure ? 1 : 0;
}
