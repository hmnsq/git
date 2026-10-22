#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "object.h"
#અન્તર્ભાવ "blob.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "delta.h"
#અન્તર્ભાવ "pack.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "csum-file.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "packfile.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "mem-pool.h"
#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "khash.h"
#અન્તર્ભાવ "date.h"
#અન્તર્ભાવ "gpg-interface.h"
#અન્તર્ભાવ "parse-options.h"

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

સંરચના object_entry {
	સંરચના pack_idx_entry idx;
	સંરચના hashmap_entry ent;
	uint32_t type : TYPE_BITS,
		pack_id : PACK_ID_BITS,
		depth : DEPTH_BITS;
};

સ્થિર પૂર્ણાંક object_entry_hashcmp(અચલ શૂન્ય *map_data UNUSED,
				અચલ સંરચના hashmap_entry *eptr,
				અચલ સંરચના hashmap_entry *entry_or_ચાવી,
				અચલ શૂન્ય *keydata)
{
	અચલ સંરચના object_id *oid = keydata;
	અચલ સંરચના object_entry *e1, *e2;

	e1 = container_of(eptr, અચલ સંરચના object_entry, ent);
	જો (oid)
		પાછા oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_ચાવી, અચલ સંરચના object_entry, ent);
	પાછા oidcmp(&e1->idx.oid, &e2->idx.oid);
}

સંરચના object_entry_pool {
	સંરચના object_entry_pool *next_pool;
	સંરચના object_entry *next_free;
	સંરચના object_entry *end;
	સંરચના object_entry entries[FLEX_ARRAY]; /* more */
};

સંરચના mark_set {
	સંઘ {
		સંરચના object_id *oids[1024];
		સંરચના object_entry *marked[1024];
		સંરચના mark_set *sets[1024];
	} data;
	unsigned પૂર્ણાંક shift;
};

સંરચના last_object {
	સંરચના strbuf data;
	off_t offset;
	unsigned પૂર્ણાંક depth;
	unsigned no_swap : 1;
};

સંરચના atom_str {
	સંરચના atom_str *next_atom;
	unsigned short str_len;
	અક્ષર str_dat[FLEX_ARRAY]; /* more */
};

સંરચના વૃક્ષ_content;
સંરચના વૃક્ષ_entry {
	સંરચના વૃક્ષ_content *વૃક્ષ;
	સંરચના atom_str *name;
	સંરચના વૃક્ષ_entry_ms {
		uint16_t mode;
		સંરચના object_id oid;
	} versions[2];
};

સંરચના વૃક્ષ_content {
	unsigned પૂર્ણાંક entry_capacity; /* must match avail_tree_content */
	unsigned પૂર્ણાંક entry_ગણતરી;
	unsigned પૂર્ણાંક delta_depth;
	સંરચના વૃક્ષ_entry *entries[FLEX_ARRAY]; /* more */
};

સંરચના avail_વૃક્ષ_content {
	unsigned પૂર્ણાંક entry_capacity; /* must match tree_content */
	સંરચના avail_વૃક્ષ_content *next_avail;
};

સંરચના branch {
	સંરચના branch *table_next_branch;
	સંરચના branch *active_next_branch;
	અચલ અક્ષર *name;
	સંરચના વૃક્ષ_entry branch_વૃક્ષ;
	uintmax_t last_commit;
	uintmax_t અંક_notes;
	unsigned active : 1;
	unsigned delete : 1;
	unsigned pack_id : PACK_ID_BITS;
	સંરચના object_id oid;
};

સંરચના tag {
	સંરચના tag *next_tag;
	અચલ અક્ષર *name;
	unsigned પૂર્ણાંક pack_id;
	સંરચના object_id oid;
};

સંરચના hash_list {
	સંરચના hash_list *next;
	સંરચના object_id oid;
};

પ્રકારવ્યાખ્યા ગણના {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

સંરચના recent_command {
	સંરચના recent_command *prev;
	સંરચના recent_command *next;
	અક્ષર *buf;
};

પ્રકારવ્યાખ્યા શૂન્ય (*mark_set_inserter_t)(સંરચના mark_set **s, સંરચના object_id *oid, uintmax_t mark);
પ્રકારવ્યાખ્યા શૂન્ય (*each_mark_fn_t)(uintmax_t mark, શૂન્ય *obj, શૂન્ય *cbp);

/* Configured limits on output */
સ્થિર unsigned long max_depth = 50;
સ્થિર off_t max_packsize;
સ્થિર પૂર્ણાંક unpack_limit = 100;
સ્થિર પૂર્ણાંક force_update;

/* Stats and misc. counters */
સ્થિર uintmax_t alloc_ગણતરી;
સ્થિર uintmax_t marks_set_ગણતરી;
સ્થિર uintmax_t object_ગણતરી_by_type[1 << TYPE_BITS];
સ્થિર uintmax_t duplicate_ગણતરી_by_type[1 << TYPE_BITS];
સ્થિર uintmax_t delta_ગણતરી_by_type[1 << TYPE_BITS];
સ્થિર uintmax_t delta_ગણતરી_attempts_by_type[1 << TYPE_BITS];
સ્થિર unsigned long object_ગણતરી;
સ્થિર unsigned long branch_ગણતરી;
સ્થિર unsigned long branch_load_ગણતરી;
સ્થિર પૂર્ણાંક failure;
સ્થિર FILE *pack_edges;
સ્થિર પૂર્ણાંક show_stats = 1;
સ્થિર પૂર્ણાંક quiet;
સ્થિર ગણના sign_mode signed_tag_mode = SIGN_VERBATIM;
સ્થિર ગણના sign_mode signed_commit_mode = SIGN_VERBATIM;
સ્થિર અચલ અક્ષર *signed_commit_keyid;
સ્થિર અચલ અક્ષર *signed_tag_keyid;

/* Memory pools */
સ્થિર સંરચના mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - કદ(સંરચના mp_block),
};

/* Atom management */
સ્થિર unsigned પૂર્ણાંક atom_table_sz = 4451;
સ્થિર unsigned પૂર્ણાંક atom_cnt;
સ્થિર સંરચના atom_str **atom_table;

/* The .pack file being generated */
સ્થિર સંરચના pack_idx_option pack_idx_opts;
સ્થિર unsigned પૂર્ણાંક pack_id;
સ્થિર સંરચના hashfile *pack_file;
સ્થિર સંરચના packed_git *pack_data;
સ્થિર સંરચના packed_git **all_packs;
સ્થિર off_t pack_size;

/* Table of objects we've written. */
સ્થિર unsigned પૂર્ણાંક object_entry_alloc = 5000;
સ્થિર સંરચના object_entry_pool *blocks;
સ્થિર સંરચના hashmap object_table;
સ્થિર સંરચના mark_set *marks;
સ્થિર અક્ષર *export_marks_file;
સ્થિર અક્ષર *import_marks_file;
સ્થિર પૂર્ણાંક import_marks_file_from_stream;
સ્થિર પૂર્ણાંક import_marks_file_ignore_missing;
સ્થિર પૂર્ણાંક import_marks_file_done;
સ્થિર પૂર્ણાંક relative_marks_paths;

/* Our last blob */
સ્થિર સંરચના last_object last_blob = {
	.data = STRBUF_INIT,
 };

/* Tree management */
સ્થિર unsigned પૂર્ણાંક વૃક્ષ_entry_alloc = 1000;
સ્થિર શૂન્ય *avail_વૃક્ષ_entry;
સ્થિર unsigned પૂર્ણાંક avail_વૃક્ષ_table_sz = 100;
સ્થિર સંરચના avail_વૃક્ષ_content **avail_વૃક્ષ_table;
સ્થિર size_t વૃક્ષ_entry_allocd;
સ્થિર સંરચના strbuf old_વૃક્ષ = STRBUF_INIT;
સ્થિર સંરચના strbuf new_વૃક્ષ = STRBUF_INIT;

/* Branch data */
સ્થિર unsigned long max_active_branches = 5;
સ્થિર unsigned long cur_active_branches;
સ્થિર unsigned long branch_table_sz = 1039;
સ્થિર સંરચના branch **branch_table;
સ્થિર સંરચના branch *active_branches;

/* Tag data */
સ્થિર સંરચના tag *first_tag;
સ્થિર સંરચના tag *last_tag;

/* Input stream parsing */
સ્થિર whenspec_type whenspec = WHENSPEC_RAW;
સ્થિર સંરચના strbuf command_buf = STRBUF_INIT;
સ્થિર પૂર્ણાંક unread_command_buf;
સ્થિર સંરચના recent_command cmd_hist = {
	.prev = &cmd_hist,
	.next = &cmd_hist,
};
સ્થિર સંરચના recent_command *cmd_tail = &cmd_hist;
સ્થિર સંરચના recent_command *rc_free;
સ્થિર unsigned પૂર્ણાંક cmd_save = 100;
સ્થિર uintmax_t next_mark;
સ્થિર સંરચના strbuf new_data = STRBUF_INIT;
સ્થિર પૂર્ણાંક require_explicit_termination;

/* Signal handling */
સ્થિર અસ્થિર sig_atomic_t checkpoint_requested;

/* Submodule marks */
સ્થિર સંરચના string_list sub_marks_from = STRING_LIST_INIT_DUP;
સ્થિર સંરચના string_list sub_marks_to = STRING_LIST_INIT_DUP;
સ્થિર kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
સ્થિર પૂર્ણાંક cat_blob_fd = STDOUT_FILENO;

/* Command state */
સંરચના fast_import_state {
	પૂર્ણાંક argc;
	અચલ અક્ષર **argv;
	અચલ અક્ષર *prefix;
	પૂર્ણાંક seen_data_command;
	પૂર્ણાંક allow_unsafe_features;
	સંરચના option *option;
};

સ્થિર શૂન્ય fast_import_state_init(સંરચના fast_import_state *state,
				   પૂર્ણાંક argc, અચલ અક્ષર **argv,
				   અચલ અક્ષર *prefix, સંરચના option *option)
{
	memset(state, 0, કદ(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

સ્થિર શૂન્ય parse_argv(સંરચના fast_import_state *state);
સ્થિર શૂન્ય parse_get_mark(સંરચના fast_import_state *state, અચલ અક્ષર *p);
સ્થિર શૂન્ય parse_cat_blob(સંરચના fast_import_state *state, અચલ અક્ષર *p);
સ્થિર શૂન્ય parse_ls(સંરચના fast_import_state *state,
		     અચલ અક્ષર *p,
		     સંરચના branch *b);

સ્થિર શૂન્ય for_each_mark(સંરચના mark_set *m, uintmax_t base, each_mark_fn_t callback, શૂન્ય *p)
{
	uintmax_t k;
	જો (m->shift) {
		માટે (k = 0; k < 1024; k++) {
			જો (m->data.sets[k])
				for_each_mark(m->data.sets[k], base + (k << m->shift), callback, p);
		}
	} નહીંતર {
		માટે (k = 0; k < 1024; k++) {
			જો (m->data.marked[k])
				callback(base + k, m->data.marked[k], p);
		}
	}
}

સ્થિર શૂન્ય dump_marks_fn(uintmax_t mark, શૂન્ય *object, શૂન્ય *cbp) {
	સંરચના object_entry *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

સ્થિર શૂન્ય write_branch_report(FILE *rpt, સંરચના branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	જો (b->active)
		fputs(" active", rpt);
	જો (b->branch_વૃક્ષ.વૃક્ષ)
		fputs(" loaded", rpt);
	જો (is_null_oid(&b->branch_વૃક્ષ.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_to_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_to_hex(&b->branch_વૃક્ષ.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_to_hex(&b->branch_વૃક્ષ.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->last_commit);

	fputs("  last pack   : ", rpt);
	જો (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

સ્થિર શૂન્ય write_crash_report(અચલ અક્ષર *err)
{
	અક્ષર *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	સંરચના branch *b;
	unsigned long lu;
	સંરચના recent_command *rc;

	જો (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		મુક્ત(loc);
		પાછા;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(time(ખાલી), 0, DATE_MODE(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	માટે (rc = cmd_hist.next; rc != &cmd_hist; rc = rc->next) {
		જો (rc->next == &cmd_hist)
			fputs("* ", rpt);
		નહીંતર
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
	માટે (b = active_branches, lu = 0; b; b = b->active_next_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	માટે (lu = 0; lu < branch_table_sz; lu++) {
		માટે (b = branch_table[lu]; b; b = b->table_next_branch)
			write_branch_report(rpt, b);
	}

	જો (first_tag) {
		સંરચના tag *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		માટે (tg = first_tag; tg; tg = tg->next_tag) {
			fputs(oid_to_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	જો (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	નહીંતર
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	મુક્ત(loc);
}

સ્થિર શૂન્ય end_packfile(શૂન્ય);
સ્થિર શૂન્ય unkeep_all_packs(શૂન્ય);
સ્થિર શૂન્ય dump_marks(શૂન્ય);

સ્થિર NORETURN શૂન્ય die_nicely(અચલ અક્ષર *err, va_list params)
{
	va_list cp;
	સ્થિર પૂર્ણાંક zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	જો (!zombie) {
		અક્ષર message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, કદ(message), err, cp);
		write_crash_report(message);
		end_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	exit(128);
}

#ifndef SIGUSR1	/* Windows, for example */

સ્થિર શૂન્ય set_checkpoint_signal(શૂન્ય)
{
}

#નહીંતર

સ્થિર શૂન્ય checkpoint_signal(પૂર્ણાંક signo UNUSED)
{
	checkpoint_requested = 1;
}

સ્થિર શૂન્ય set_checkpoint_signal(શૂન્ય)
{
	સંરચના sigaction sa;

	memset(&sa, 0, કદ(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, ખાલી);
}

#endif

સ્થિર શૂન્ય alloc_objects(unsigned પૂર્ણાંક cnt)
{
	સંરચના object_entry_pool *b;

	b = xmalloc(કદ(સંરચના object_entry_pool)
		+ cnt * કદ(સંરચના object_entry));
	b->next_pool = blocks;
	b->next_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_ગણતરી += cnt;
}

સ્થિર સંરચના object_entry *new_object(સંરચના object_id *oid)
{
	સંરચના object_entry *e;

	જો (blocks->next_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->next_free++;
	oidcpy(&e->idx.oid, oid);
	પાછા e;
}

સ્થિર સંરચના object_entry *find_object(સંરચના object_id *oid)
{
	પાછા hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   સંરચના object_entry, ent);
}

સ્થિર સંરચના object_entry *ઉમેરો_object(સંરચના object_id *oid)
{
	સંરચના object_entry *e;
	unsigned પૂર્ણાંક hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					સંરચના object_entry, ent);
	જો (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	પાછા e;
}

સ્થિર શૂન્ય invalidate_pack_id(unsigned પૂર્ણાંક id)
{
	unsigned long lu;
	સંરચના tag *t;
	સંરચના hashmap_iter iter;
	સંરચના object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		જો (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	માટે (lu = 0; lu < branch_table_sz; lu++) {
		સંરચના branch *b;

		માટે (b = branch_table[lu]; b; b = b->table_next_branch)
			જો (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	માટે (t = first_tag; t; t = t->next_tag)
		જો (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

સ્થિર unsigned પૂર્ણાંક hc_str(અચલ અક્ષર *s, size_t len)
{
	unsigned પૂર્ણાંક r = 0;
	જ્યારે (len-- > 0)
		r = r * 31 + *s++;
	પાછા r;
}

સ્થિર શૂન્ય ઉમેરો_mark(સંરચના mark_set **ટોચ, uintmax_t idnum, સંરચના object_entry *oe)
{
	સંરચના mark_set *s = *ટોચ;

	જ્યારે ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, કદ(સંરચના mark_set));
		s->shift = (*ટોચ)->shift + 10;
		s->data.sets[0] = *ટોચ;
		*ટોચ = s;
	}
	જ્યારે (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		જો (!s->data.sets[i]) {
			s->data.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, કદ(સંરચના mark_set));
			s->data.sets[i]->shift = s->shift - 10;
		}
		s = s->data.sets[i];
	}
	જો (!s->data.marked[idnum])
		marks_set_ગણતરી++;
	s->data.marked[idnum] = oe;
}

સ્થિર શૂન્ય *find_mark(સંરચના mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	સંરચના object_entry *oe = ખાલી;
	જો ((idnum >> s->shift) < 1024) {
		જ્યારે (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->data.sets[i];
		}
		જો (s)
			oe = s->data.marked[idnum];
	}
	જો (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	પાછા oe;
}

સ્થિર સંરચના atom_str *to_atom(અચલ અક્ષર *s, unsigned short len)
{
	unsigned પૂર્ણાંક hc = hc_str(s, len) % atom_table_sz;
	સંરચના atom_str *c;

	માટે (c = atom_table[hc]; c; c = c->next_atom)
		જો (c->str_len == len && !strncmp(s, c->str_dat, len))
			પાછા c;

	c = mem_pool_alloc(&fi_mem_pool, કદ(સંરચના atom_str) + len + 1);
	c->str_len = len;
	memcpy(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->next_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	પાછા c;
}

સ્થિર સંરચના branch *lookup_branch(અચલ અક્ષર *name)
{
	unsigned પૂર્ણાંક hc = hc_str(name, strlen(name)) % branch_table_sz;
	સંરચના branch *b;

	માટે (b = branch_table[hc]; b; b = b->table_next_branch)
		જો (!strcmp(name, b->name))
			પાછા b;
	પાછા ખાલી;
}

સ્થિર સંરચના branch *new_branch(અચલ અક્ષર *name)
{
	unsigned પૂર્ણાંક hc = hc_str(name, strlen(name)) % branch_table_sz;
	સંરચના branch *b = lookup_branch(name);

	જો (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	જો (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, કદ(સંરચના branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_next_branch = branch_table[hc];
	b->branch_વૃક્ષ.versions[0].mode = S_IFDIR;
	b->branch_વૃક્ષ.versions[1].mode = S_IFDIR;
	b->અંક_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_ગણતરી++;
	પાછા b;
}

સ્થિર unsigned પૂર્ણાંક hc_entries(unsigned પૂર્ણાંક cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	પાછા cnt < avail_વૃક્ષ_table_sz ? cnt : avail_વૃક્ષ_table_sz - 1;
}

સ્થિર સંરચના વૃક્ષ_content *new_વૃક્ષ_content(unsigned પૂર્ણાંક cnt)
{
	સંરચના avail_વૃક્ષ_content *f, *l = ખાલી;
	સંરચના વૃક્ષ_content *t;
	unsigned પૂર્ણાંક hc = hc_entries(cnt);

	માટે (f = avail_વૃક્ષ_table[hc]; f; l = f, f = f->next_avail)
		જો (f->entry_capacity >= cnt)
			તોડો;

	જો (f) {
		જો (l)
			l->next_avail = f->next_avail;
		નહીંતર
			avail_વૃક્ષ_table[hc] = f->next_avail;
	} નહીંતર {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, કદ(*t) + કદ(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (સંરચના વૃક્ષ_content*)f;
	t->entry_ગણતરી = 0;
	t->delta_depth = 0;
	પાછા t;
}

સ્થિર શૂન્ય release_વૃક્ષ_entry(સંરચના વૃક્ષ_entry *e);
સ્થિર શૂન્ય release_વૃક્ષ_content(સંરચના વૃક્ષ_content *t)
{
	સંરચના avail_વૃક્ષ_content *f = (સંરચના avail_વૃક્ષ_content*)t;
	unsigned પૂર્ણાંક hc = hc_entries(f->entry_capacity);
	f->next_avail = avail_વૃક્ષ_table[hc];
	avail_વૃક્ષ_table[hc] = f;
}

સ્થિર શૂન્ય release_વૃક્ષ_content_recursive(સંરચના વૃક્ષ_content *t)
{
	unsigned પૂર્ણાંક i;
	માટે (i = 0; i < t->entry_ગણતરી; i++)
		release_વૃક્ષ_entry(t->entries[i]);
	release_વૃક્ષ_content(t);
}

સ્થિર સંરચના વૃક્ષ_content *grow_વૃક્ષ_content(
	સંરચના વૃક્ષ_content *t,
	પૂર્ણાંક amt)
{
	સંરચના વૃક્ષ_content *r = new_વૃક્ષ_content(t->entry_ગણતરી + amt);
	r->entry_ગણતરી = t->entry_ગણતરી;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_ગણતરી);
	release_વૃક્ષ_content(t);
	પાછા r;
}

સ્થિર સંરચના વૃક્ષ_entry *new_વૃક્ષ_entry(શૂન્ય)
{
	સંરચના વૃક્ષ_entry *e;

	જો (!avail_વૃક્ષ_entry) {
		unsigned પૂર્ણાંક n = વૃક્ષ_entry_alloc;
		વૃક્ષ_entry_allocd += n * કદ(સંરચના વૃક્ષ_entry);
		ALLOC_ARRAY(e, n);
		avail_વૃક્ષ_entry = e;
		જ્યારે (n-- > 1) {
			*((શૂન્ય**)e) = e + 1;
			e++;
		}
		*((શૂન્ય**)e) = ખાલી;
	}

	e = avail_વૃક્ષ_entry;
	avail_વૃક્ષ_entry = *((શૂન્ય**)e);
	પાછા e;
}

સ્થિર શૂન્ય release_વૃક્ષ_entry(સંરચના વૃક્ષ_entry *e)
{
	જો (e->વૃક્ષ)
		release_વૃક્ષ_content_recursive(e->વૃક્ષ);
	*((શૂન્ય**)e) = avail_વૃક્ષ_entry;
	avail_વૃક્ષ_entry = e;
}

સ્થિર સંરચના વૃક્ષ_content *dup_વૃક્ષ_content(સંરચના વૃક્ષ_content *s)
{
	સંરચના વૃક્ષ_content *d;
	સંરચના વૃક્ષ_entry *a, *b;
	unsigned પૂર્ણાંક i;

	જો (!s)
		પાછા ખાલી;
	d = new_વૃક્ષ_content(s->entry_ગણતરી);
	માટે (i = 0; i < s->entry_ગણતરી; i++) {
		a = s->entries[i];
		b = new_વૃક્ષ_entry();
		memcpy(b, a, કદ(*a));
		જો (a->વૃક્ષ && is_null_oid(&b->versions[1].oid))
			b->વૃક્ષ = dup_વૃક્ષ_content(a->વૃક્ષ);
		નહીંતર
			b->વૃક્ષ = ખાલી;
		d->entries[i] = b;
	}
	d->entry_ગણતરી = s->entry_ગણતરી;
	d->delta_depth = s->delta_depth;

	પાછા d;
}

સ્થિર શૂન્ય start_packfile(શૂન્ય)
{
	સંરચના strbuf tmp_file = STRBUF_INIT;
	સંરચના packed_git *p;
	પૂર્ણાંક pack_fd;

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
	object_ગણતરી = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

સ્થિર અચલ અક્ષર *create_index(શૂન્ય)
{
	અચલ અક્ષર *tmpfile;
	સંરચના pack_idx_entry **idx, **c, **last;
	સંરચના object_entry *e;
	સંરચના object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_ગણતરી);
	c = idx;
	માટે (o = blocks; o; o = o->next_pool)
		માટે (e = o->next_free; e-- != o->entries;)
			જો (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_ગણતરી;
	જો (c != last)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, ખાલી, idx, object_ગણતરી,
				 &pack_idx_opts, pack_data->hash);
	મુક્ત(idx);
	પાછા tmpfile;
}

સ્થિર અક્ષર *keep_pack(અચલ અક્ષર *curr_index_name)
{
	સ્થિર અચલ અક્ષર *keep_સંદેશ = "fast-import";
	સંરચના strbuf name = STRBUF_INIT;
	પૂર્ણાંક keep_fd;

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_data->repo,
							    name.buf);
	જો (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_સંદેશ, strlen(keep_સંદેશ));
	જો (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "pack");
	જો (finalize_object_file(pack_data->repo, pack_data->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "idx");
	જો (finalize_object_file(pack_data->repo, curr_index_name, name.buf))
		die(_("cannot store index file"));
	મુક્ત((શૂન્ય *)curr_index_name);
	પાછા strbuf_detach(&name, ખાલી);
}

સ્થિર શૂન્ય unkeep_all_packs(શૂન્ય)
{
	સંરચના strbuf name = STRBUF_INIT;
	પૂર્ણાંક k;

	માટે (k = 0; k < pack_id; k++) {
		સંરચના packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

સ્થિર પૂર્ણાંક loosen_small_pack(અચલ સંરચના packed_git *p)
{
	સંરચના child_process unpack = CHILD_PROCESS_INIT;

	જો (lseek(p->pack_fd, 0, SEEK_SET) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_ધકેલો(&unpack.args, "unpack-objects");
	જો (!show_stats)
		strvec_ધકેલો(&unpack.args, "-q");

	પાછા run_command(&unpack);
}

સ્થિર શૂન્ય end_packfile(શૂન્ય)
{
	સ્થિર પૂર્ણાંક running;

	જો (running || !pack_data)
		પાછા;

	running = 1;
	clear_delta_base_cache();
	જો (object_ગણતરી) {
		સંરચના odb_source_files *files = odb_source_files_downcast(pack_data->repo->objects->source);
		સંરચના packed_git *new_p;
		સંરચના object_id cur_pack_oid;
		અક્ષર *idx_name;
		પૂર્ણાંક i;
		સંરચના branch *b;
		સંરચના tag *t;

		close_pack_windows(pack_data);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_data->pack_fd,
					 pack_data->hash, pack_data->pack_name,
					 object_ગણતરી, cur_pack_oid.hash,
					 pack_size);

		જો (object_ગણતરી <= unpack_limit) {
			જો (!loosen_small_pack(pack_data)) {
				invalidate_pack_id(pack_id);
				જાઓ discard_pack;
			}
		}

		close(pack_data->pack_fd);
		idx_name = keep_pack(create_index());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		જો (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		મુક્ત(idx_name);

		/* Print the boundary */
		જો (pack_edges) {
			fprintf(pack_edges, "%s:", new_p->pack_name);
			માટે (i = 0; i < branch_table_sz; i++) {
				માટે (b = branch_table[i]; b; b = b->table_next_branch) {
					જો (b->pack_id == pack_id)
						fprintf(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			માટે (t = first_tag; t; t = t->next_tag) {
				જો (t->pack_id == pack_id)
					fprintf(pack_edges, " %s",
						oid_to_hex(&t->oid));
			}
			fputc('\n', pack_edges);
			fflush(pack_edges);
		}

		pack_id++;
	}
	નહીંતર {
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

સ્થિર શૂન્ય cycle_packfile(શૂન્ય)
{
	end_packfile();
	start_packfile();
}

સ્થિર પૂર્ણાંક store_object(
	ગણના object_type type,
	સંરચના strbuf *dat,
	સંરચના last_object *last,
	સંરચના object_id *oidout,
	uintmax_t mark)
{
	સંરચના odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	શૂન્ય *out, *delta;
	સંરચના object_entry *e;
	unsigned અક્ષર hdr[96];
	સંરચના object_id oid;
	unsigned long hdrlen, deltalen = 0;
	સંરચના git_hash_ctx c;
	git_zstream s;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	hdrlen = format_object_header((અક્ષર *)hdr, કદ(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	જો (oidout)
		oidcpy(oidout, &oid);

	e = ઉમેરો_object(&oid);
	જો (mark)
		ઉમેરો_mark(&marks, mark, e);
	જો (e->idx.offset) {
		duplicate_ગણતરી_by_type[type]++;
		પાછા 1;
	}

	માટે (સંરચના odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		જો (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			ચાલુ;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_ગણતરી_by_type[type]++;
		પાછા 1;
	}

	જો (last && last->data.len && last->data.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		size_t deltalen_st;

		delta_ગણતરી_attempts_by_type[type]++;
		delta = diff_delta(last->data.buf, last->data.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_size_t_to_ulong(deltalen_st);
	} નહીંતર
		delta = ખાલી;

	git_deflate_init(&s, cfg->pack_compression_level);
	જો (delta) {
		s.next_in = delta;
		s.avail_in = deltalen;
	} નહીંતર {
		s.next_in = (શૂન્ય *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.next_out = out = xmalloc(s.avail_out);
	જ્યારે (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	જો ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + s.total_out) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + s.total_out) < pack_size) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		જો (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.next_in = (શૂન્ય *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.next_out = out = xrealloc(out, s.avail_out);
			જ્યારે (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_end(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.offset = pack_size;
	object_ગણતરી++;
	object_ગણતરી_by_type[type]++;

	crc32_begin(pack_file);

	જો (delta) {
		off_t ofs = e->idx.offset - last->offset;
		unsigned pos = કદ(hdr) - 1;

		delta_ગણતરી_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, કદ(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;

		hdr[pos] = ofs & 127;
		જ્યારે (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, કદ(hdr) - pos);
		pack_size += કદ(hdr) - pos;
	} નહીંતર {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, કદ(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;
	}

	hashwrite(pack_file, out, s.total_out);
	pack_size += s.total_out;

	e->idx.crc32 = crc32_end(pack_file);

	મુક્ત(out);
	મુક્ત(delta);
	જો (last) {
		જો (last->no_swap) {
			last->data = *dat;
		} નહીંતર {
			strbuf_swap(&last->data, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	પાછા 0;
}

સ્થિર શૂન્ય truncate_pack(સંરચના hashfile_checkpoint *checkpoint)
{
	જો (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_size = checkpoint->offset;
}

સ્થિર શૂન્ય stream_blob(uintmax_t len, સંરચના object_id *oidout, uintmax_t mark)
{
	સંરચના odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	size_t in_sz = 64 * 1024, out_sz = 64 * 1024;
	unsigned અક્ષર *in_buf = xmalloc(in_sz);
	unsigned અક્ષર *out_buf = xmalloc(out_sz);
	સંરચના object_entry *e;
	સંરચના object_id oid;
	unsigned long hdrlen;
	off_t offset;
	સંરચના git_hash_ctx c;
	git_zstream s;
	સંરચના hashfile_checkpoint checkpoint;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);
	પૂર્ણાંક status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	જો ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + len) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + len) < pack_size)
		cycle_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	offset = checkpoint.offset;

	hdrlen = format_object_header((અક્ષર *)out_buf, out_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, out_buf, hdrlen);

	crc32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_in_pack_object_header(out_buf, out_sz, OBJ_BLOB, len);

	s.next_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	જ્યારે (status != Z_STREAM_END) {
		જો (0 < len && !s.avail_in) {
			size_t cnt = in_sz < len ? in_sz : (size_t)len;
			size_t n = fread(in_buf, 1, cnt, stdin);
			જો (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.next_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		જો (!s.avail_out || status == Z_STREAM_END) {
			size_t n = s.next_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_size += n;
			s.next_out = out_buf;
			s.avail_out = out_sz;
		}

		પસંદગી (status) {
		સ્થિતિ Z_OK:
		સ્થિતિ Z_BUF_ERROR:
		સ્થિતિ Z_STREAM_END:
			ચાલુ;
		મૂળભૂત:
			die(_("unexpected deflate failure: %d"), status);
		}
	}
	git_deflate_end(&s);
	git_hash_final_oid(&oid, &c);

	જો (oidout)
		oidcpy(oidout, &oid);

	e = ઉમેરો_object(&oid);

	જો (mark)
		ઉમેરો_mark(&marks, mark, e);

	જો (e->idx.offset) {
		duplicate_ગણતરી_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		જાઓ out;
	}

	માટે (સંરચના odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		જો (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			ચાલુ;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_ગણતરી_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		જાઓ out;
	}

	e->depth = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.offset = offset;
	e->idx.crc32 = crc32_end(pack_file);
	object_ગણતરી++;
	object_ગણતરી_by_type[OBJ_BLOB]++;

out:
	મુક્ત(in_buf);
	મુક્ત(out_buf);
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
સ્થિર શૂન્ય *gfi_unpack_entry(
	સંરચના object_entry *oe,
	unsigned long *sizep)
{
	ગણના object_type type;
	size_t size_st = 0;
	શૂન્ય *data;
	સંરચના packed_git *p = all_packs[oe->pack_id];
	જો (p == pack_data && p->pack_size < (pack_size + the_hash_algo->rawsz)) {
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
	જો (sizep)
		*sizep = cast_size_t_to_ulong(size_st);
	પાછા data;
}

સ્થિર શૂન્ય load_વૃક્ષ(સંરચના વૃક્ષ_entry *મૂળ)
{
	સંરચના object_id *oid = &મૂળ->versions[1].oid;
	સંરચના object_entry *myoe;
	સંરચના વૃક્ષ_content *t;
	unsigned long size;
	અક્ષર *buf;
	અચલ અક્ષર *c;

	મૂળ->વૃક્ષ = t = new_વૃક્ષ_content(8);
	જો (is_null_oid(oid))
		પાછા;

	myoe = find_object(oid);
	જો (myoe && myoe->pack_id != MAX_PACK_ID) {
		જો (myoe->type != OBJ_વૃક્ષ)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &size);
		જો (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} નહીંતર {
		ગણના object_type type;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		જો (!buf || type != OBJ_વૃક્ષ)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	જ્યારે (c != (buf + size)) {
		સંરચના વૃક્ષ_entry *e = new_વૃક્ષ_entry();

		જો (t->entry_ગણતરી == t->entry_capacity)
			મૂળ->વૃક્ષ = t = grow_વૃક્ષ_content(t, t->entry_ગણતરી);
		t->entries[t->entry_ગણતરી++] = e;

		e->વૃક્ષ = ખાલી;
		c = parse_mode(c, &e->versions[1].mode);
		જો (!c)
			die(_("corrupt mode in %s"), oid_to_hex(oid));
		e->versions[0].mode = e->versions[1].mode;
		e->name = to_atom(c, strlen(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (unsigned અક્ષર *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (unsigned અક્ષર *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	મુક્ત(buf);
}

સ્થિર પૂર્ણાંક tecmp0 (અચલ શૂન્ય *_a, અચલ શૂન્ય *_b)
{
	સંરચના વૃક્ષ_entry *a = *((સંરચના વૃક્ષ_entry**)_a);
	સંરચના વૃક્ષ_entry *b = *((સંરચના વૃક્ષ_entry**)_b);
	પાછા base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

સ્થિર પૂર્ણાંક tecmp1 (અચલ શૂન્ય *_a, અચલ શૂન્ય *_b)
{
	સંરચના વૃક્ષ_entry *a = *((સંરચના વૃક્ષ_entry**)_a);
	સંરચના વૃક્ષ_entry *b = *((સંરચના વૃક્ષ_entry**)_b);
	પાછા base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

સ્થિર શૂન્ય mktree(સંરચના વૃક્ષ_content *t, પૂર્ણાંક v, સંરચના strbuf *b)
{
	size_t maxlen = 0;
	unsigned પૂર્ણાંક i;

	જો (!v)
		QSORT(t->entries, t->entry_ગણતરી, tecmp0);
	નહીંતર
		QSORT(t->entries, t->entry_ગણતરી, tecmp1);

	માટે (i = 0; i < t->entry_ગણતરી; i++) {
		જો (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	માટે (i = 0; i < t->entry_ગણતરી; i++) {
		સંરચના વૃક્ષ_entry *e = t->entries[i];
		જો (!e->versions[v].mode)
			ચાલુ;
		strbuf_addf(b, "%o %s%c",
			(unsigned પૂર્ણાંક)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

સ્થિર શૂન્ય store_વૃક્ષ(સંરચના વૃક્ષ_entry *મૂળ)
{
	સંરચના વૃક્ષ_content *t;
	unsigned પૂર્ણાંક i, j, del;
	સંરચના last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	સંરચના object_entry *le = ખાલી;

	જો (!is_null_oid(&મૂળ->versions[1].oid))
		પાછા;

	જો (!મૂળ->વૃક્ષ)
		load_વૃક્ષ(મૂળ);
	t = મૂળ->વૃક્ષ;

	માટે (i = 0; i < t->entry_ગણતરી; i++) {
		જો (t->entries[i]->વૃક્ષ)
			store_વૃક્ષ(t->entries[i]);
	}

	જો (!(મૂળ->versions[0].mode & NO_DELTA))
		le = find_object(&મૂળ->versions[0].oid);
	જો (S_ISDIR(મૂળ->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_વૃક્ષ);
		lo.data = old_વૃક્ષ;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_વૃક્ષ);
	store_object(OBJ_વૃક્ષ, &new_વૃક્ષ, &lo, &મૂળ->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	માટે (i = 0, j = 0, del = 0; i < t->entry_ગણતરી; i++) {
		સંરચના વૃક્ષ_entry *e = t->entries[i];
		જો (e->versions[1].mode) {
			e->versions[0].mode = e->versions[1].mode;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} નહીંતર {
			release_વૃક્ષ_entry(e);
			del++;
		}
	}
	t->entry_ગણતરી -= del;
}

સ્થિર શૂન્ય વૃક્ષ_content_replace(
	સંરચના વૃક્ષ_entry *મૂળ,
	અચલ સંરચના object_id *oid,
	અચલ uint16_t mode,
	સંરચના વૃક્ષ_content *newtree)
{
	જો (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&મૂળ->versions[0].oid, the_repository->hash_algo);
	oidcpy(&મૂળ->versions[1].oid, oid);
	જો (મૂળ->વૃક્ષ)
		release_વૃક્ષ_content_recursive(મૂળ->વૃક્ષ);
	મૂળ->વૃક્ષ = newtree;
}

સ્થિર પૂર્ણાંક વૃક્ષ_content_set(
	સંરચના વૃક્ષ_entry *મૂળ,
	અચલ અક્ષર *p,
	અચલ સંરચના object_id *oid,
	અચલ uint16_t mode,
	સંરચના વૃક્ષ_content *subtree)
{
	સંરચના વૃક્ષ_content *t;
	અચલ અક્ષર *slash1;
	unsigned પૂર્ણાંક i, n;
	સંરચના વૃક્ષ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	જો (!n)
		die(_("empty path component found in input"));
	જો (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	જો (!મૂળ->વૃક્ષ)
		load_વૃક્ષ(મૂળ);
	t = મૂળ->વૃક્ષ;
	માટે (i = 0; i < t->entry_ગણતરી; i++) {
		e = t->entries[i];
		જો (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			જો (!*slash1) {
				જો (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					પાછા 0;
				e->versions[1].mode = mode;
				oidcpy(&e->versions[1].oid, oid);
				જો (e->વૃક્ષ)
					release_વૃક્ષ_content_recursive(e->વૃક્ષ);
				e->વૃક્ષ = subtree;

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
				જો (S_ISDIR(e->versions[0].mode))
					e->versions[0].mode |= NO_DELTA;

				oidclr(&મૂળ->versions[1].oid, the_repository->hash_algo);
				પાછા 1;
			}
			જો (!S_ISDIR(e->versions[1].mode)) {
				e->વૃક્ષ = new_વૃક્ષ_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			જો (!e->વૃક્ષ)
				load_વૃક્ષ(e);
			જો (વૃક્ષ_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&મૂળ->versions[1].oid, the_repository->hash_algo);
				પાછા 1;
			}
			પાછા 0;
		}
	}

	જો (t->entry_ગણતરી == t->entry_capacity)
		મૂળ->વૃક્ષ = t = grow_વૃક્ષ_content(t, t->entry_ગણતરી);
	e = new_વૃક્ષ_entry();
	e->name = to_atom(p, n);
	e->versions[0].mode = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->entry_ગણતરી++] = e;
	જો (*slash1) {
		e->વૃક્ષ = new_વૃક્ષ_content(8);
		e->versions[1].mode = S_IFDIR;
		વૃક્ષ_content_set(e, slash1 + 1, oid, mode, subtree);
	} નહીંતર {
		e->વૃક્ષ = subtree;
		e->versions[1].mode = mode;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&મૂળ->versions[1].oid, the_repository->hash_algo);
	પાછા 1;
}

સ્થિર પૂર્ણાંક વૃક્ષ_content_remove(
	સંરચના વૃક્ષ_entry *મૂળ,
	અચલ અક્ષર *p,
	સંરચના વૃક્ષ_entry *backup_leaf,
	પૂર્ણાંક allow_મૂળ)
{
	સંરચના વૃક્ષ_content *t;
	અચલ અક્ષર *slash1;
	unsigned પૂર્ણાંક i, n;
	સંરચના વૃક્ષ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	જો (!મૂળ->વૃક્ષ)
		load_વૃક્ષ(મૂળ);

	જો (!*p && allow_મૂળ) {
		e = મૂળ;
		જાઓ del_entry;
	}

	t = મૂળ->વૃક્ષ;
	માટે (i = 0; i < t->entry_ગણતરી; i++) {
		e = t->entries[i];
		જો (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			જો (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				પાછા 1;
			જો (!*slash1 || !S_ISDIR(e->versions[1].mode))
				જાઓ del_entry;
			જો (!e->વૃક્ષ)
				load_વૃક્ષ(e);
			જો (વૃક્ષ_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				માટે (n = 0; n < e->વૃક્ષ->entry_ગણતરી; n++) {
					જો (e->વૃક્ષ->entries[n]->versions[1].mode) {
						oidclr(&મૂળ->versions[1].oid,
						       the_repository->hash_algo);
						પાછા 1;
					}
				}
				backup_leaf = ખાલી;
				જાઓ del_entry;
			}
			પાછા 0;
		}
	}
	પાછા 0;

del_entry:
	જો (backup_leaf)
		memcpy(backup_leaf, e, કદ(*backup_leaf));
	નહીંતર જો (e->વૃક્ષ)
		release_વૃક્ષ_content_recursive(e->વૃક્ષ);
	e->વૃક્ષ = ખાલી;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&મૂળ->versions[1].oid, the_repository->hash_algo);
	પાછા 1;
}

સ્થિર પૂર્ણાંક વૃક્ષ_content_get(
	સંરચના વૃક્ષ_entry *મૂળ,
	અચલ અક્ષર *p,
	સંરચના વૃક્ષ_entry *leaf,
	પૂર્ણાંક allow_મૂળ)
{
	સંરચના વૃક્ષ_content *t;
	અચલ અક્ષર *slash1;
	unsigned પૂર્ણાંક i, n;
	સંરચના વૃક્ષ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	જો (!n && !allow_મૂળ)
		die(_("empty path component found in input"));

	જો (!મૂળ->વૃક્ષ)
		load_વૃક્ષ(મૂળ);

	જો (!n) {
		e = મૂળ;
		જાઓ found_entry;
	}

	t = મૂળ->વૃક્ષ;
	માટે (i = 0; i < t->entry_ગણતરી; i++) {
		e = t->entries[i];
		જો (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			જો (!*slash1)
				જાઓ found_entry;
			જો (!S_ISDIR(e->versions[1].mode))
				પાછા 0;
			જો (!e->વૃક્ષ)
				load_વૃક્ષ(e);
			પાછા વૃક્ષ_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	પાછા 0;

found_entry:
	memcpy(leaf, e, કદ(*leaf));
	જો (e->વૃક્ષ && is_null_oid(&e->versions[1].oid))
		leaf->વૃક્ષ = dup_વૃક્ષ_content(e->વૃક્ષ);
	નહીંતર
		leaf->વૃક્ષ = ખાલી;
	પાછા 1;
}

સ્થિર પૂર્ણાંક update_branch(સંરચના branch *b)
{
	સ્થિર અચલ અક્ષર *સંદેશ = "fast-import";
	સંરચના ref_transaction *transaction;
	સંરચના object_id old_oid;
	સંરચના strbuf err = STRBUF_INIT;
	સ્થિર અચલ અક્ષર *replace_prefix = "refs/replace/";

	જો (starts_with(b->name, replace_prefix) &&
	    !strcmp(b->name + strlen(replace_prefix),
		    oid_to_hex(&b->oid))) {
		જો (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				ખાલી, b->name, ખાલી, 0);
		પાછા 0;
	}
	જો (is_null_oid(&b->oid)) {
		જો (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					ખાલી, b->name, ખાલી, 0);
		પાછા 0;
	}
	જો (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	જો (!force_update && !is_null_oid(&old_oid)) {
		સંરચના commit *old_cmit, *new_cmit;
		પૂર્ણાંક ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		જો (!old_cmit || !new_cmit)
			પાછા error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		જો (ret < 0)
			exit(128);
		જો (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			પાછા -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	જો (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   ખાલી, ખાલી, 0, સંદેશ, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		પાછા -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	પાછા 0;
}

સ્થિર શૂન્ય dump_branches(શૂન્ય)
{
	unsigned પૂર્ણાંક i;
	સંરચના branch *b;

	માટે (i = 0; i < branch_table_sz; i++) {
		માટે (b = branch_table[i]; b; b = b->table_next_branch)
			failure |= update_branch(b);
	}
}

સ્થિર શૂન્ય dump_tags(શૂન્ય)
{
	સ્થિર અચલ અક્ષર *સંદેશ = "fast-import";
	સંરચના tag *t;
	સંરચના strbuf ref_name = STRBUF_INIT;
	સંરચના strbuf err = STRBUF_INIT;
	સંરચના ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	જો (!transaction) {
		failure |= error("%s", err.buf);
		જાઓ cleanup;
	}
	માટે (t = first_tag; t; t = t->next_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		જો (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, ખાલી, ખાલી, ખાલી,
					   0, સંદેશ, &err)) {
			failure |= error("%s", err.buf);
			જાઓ cleanup;
		}
	}
	જો (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

સ્થિર શૂન્ય dump_marks(શૂન્ય)
{
	સંરચના lock_file mark_lock = LOCK_INIT;
	FILE *f;

	જો (!export_marks_file || (import_marks_file && !import_marks_file_done))
		પાછા;

	જો (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		પાછા;
	}

	જો (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		પાછા;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	જો (!f) {
		પૂર્ણાંક saved_errno = errno;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		પાછા;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	જો (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		પાછા;
	}
}

સ્થિર શૂન્ય ઉમેરો_object_entry(સંરચના mark_set **s, સંરચના object_id *oid, uintmax_t mark)
{
	સંરચના object_entry *e;
	e = find_object(oid);
	જો (!e) {
		ગણના object_type type = odb_read_object_info(the_repository->objects,
							     oid, ખાલી);
		જો (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = ઉમેરો_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	ઉમેરો_mark(s, mark, e);
}

સ્થિર શૂન્ય ઉમેરો_oid_entry(સંરચના mark_set **s, સંરચના object_id *oid, uintmax_t mark)
{
	ઉમેરો_mark(s, mark, xmemdupz(oid, કદ(*oid)));
}

સ્થિર શૂન્ય read_mark_file(સંરચના mark_set **s, FILE *f, mark_set_inserter_t inserter)
{
	અક્ષર લીટી[512];
	જ્યારે (fgets(લીટી, કદ(લીટી), f)) {
		uintmax_t mark;
		અક્ષર *end;
		સંરચના object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, કદ(oid.hash));

		end = strchr(લીટી, '\n');
		જો (લીટી[0] != ':' || !end)
			die(_("corrupt mark line: %s"), લીટી);
		*end = 0;
		mark = strtoumax(લીટી + 1, &end, 10);
		જો (!mark || end == લીટી + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), લીટી);
		inserter(s, &oid, mark);
	}
}

સ્થિર શૂન્ય read_marks(શૂન્ય)
{
	FILE *f = fopen(import_marks_file, "r");
	જો (f)
		;
	નહીંતર જો (import_marks_file_ignore_missing && errno == ENOENT)
		જાઓ done; /* Marks file does not exist */
	નહીંતર
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, ઉમેરો_object_entry);
	fclose(f);
done:
	import_marks_file_done = 1;
}


સ્થિર પૂર્ણાંક read_next_command(સંરચના fast_import_state *state)
{
	સ્થિર પૂર્ણાંક stdin_eof = 0;

	જો (stdin_eof) {
		unread_command_buf = 0;
		પાછા EOF;
	}

	માટે (;;) {
		જો (unread_command_buf) {
			unread_command_buf = 0;
		} નહીંતર {
			સંરચના recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			જો (stdin_eof)
				પાછા EOF;

			જો (!state->seen_data_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			જો (rc)
				rc_free = rc->next;
			નહીંતર {
				rc = cmd_hist.next;
				cmd_hist.next = rc->next;
				cmd_hist.next->prev = &cmd_hist;
				મુક્ત(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_tail;
			rc->next = cmd_hist.prev;
			rc->prev->next = rc;
			cmd_tail = rc;
		}
		જો (command_buf.buf[0] == '#')
			ચાલુ;
		પાછા 0;
	}
}

સ્થિર શૂન્ય skip_optional_lf(શૂન્ય)
{
	પૂર્ણાંક term_char = fgetc(stdin);
	જો (term_char != '\n' && term_char != EOF)
		ungetc(term_char, stdin);
}

સ્થિર શૂન્ય parse_mark(સંરચના fast_import_state *state)
{
	અચલ અક્ષર *v;
	જો (skip_prefix(command_buf.buf, "mark :", &v)) {
		next_mark = strtoumax(v, ખાલી, 10);
		read_next_command(state);
	}
	નહીંતર
		next_mark = 0;
}

સ્થિર શૂન્ય parse_original_identifier(સંરચના fast_import_state *state)
{
	અચલ અક્ષર *v;
	જો (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_next_command(state);
}

સ્થિર પૂર્ણાંક parse_data(સંરચના strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	અચલ અક્ષર *data;
	strbuf_reset(sb);

	જો (!skip_prefix(command_buf.buf, "data ", &data))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	જો (skip_prefix(data, "<<", &data)) {
		અક્ષર *term = xstrdup(data);
		size_t term_len = command_buf.len - (data - command_buf.buf);

		માટે (;;) {
			જો (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), term);
			જો (term_len == command_buf.len
				&& !strcmp(term, command_buf.buf))
				તોડો;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		મુક્ત(term);
	}
	નહીંતર {
		uintmax_t len = strtoumax(data, ખાલી, 10);
		size_t n = 0, length = (size_t)len;

		જો (limit && limit < len) {
			*len_res = len;
			પાછા 0;
		}
		જો (length < len)
			die(_("data is too large to use in this context"));

		જ્યારે (n < length) {
			size_t s = strbuf_fread(sb, length - n, stdin);
			જો (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(unsigned long)(length - n));
			n += s;
		}
	}

	skip_optional_lf();
	પાછા 1;
}

સ્થિર પૂર્ણાંક validate_raw_date(અચલ અક્ષર *src, સંરચના strbuf *result, પૂર્ણાંક strict)
{
	અચલ અક્ષર *orig_src = src;
	અક્ષર *endp;
	unsigned long અંક;

	errno = 0;

	અંક = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	જો (errno || endp == src || *endp != ' ')
		પાછા -1;

	src = endp + 1;
	જો (*src != '-' && *src != '+')
		પાછા -1;

	અંક = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	જો (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < અંક))             /* parsed a broken timezone */
	   )
		પાછા -1;

	strbuf_addstr(result, orig_src);
	પાછા 0;
}

સ્થિર અક્ષર *parse_ident(અચલ અક્ષર *buf)
{
	અચલ અક્ષર *ltgt;
	size_t name_len;
	સંરચના strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	જો (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	જો (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	જો (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	જો (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	જો (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_add(&ident, buf, name_len);

	પસંદગી (whenspec) {
	સ્થિતિ WHENSPEC_RAW:
		જો (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		તોડો;
	સ્થિતિ WHENSPEC_RAW_PERMISSIVE:
		જો (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		તોડો;
	સ્થિતિ WHENSPEC_RFC2822:
		જો (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		તોડો;
	સ્થિતિ WHENSPEC_NOW:
		જો (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		તોડો;
	}

	પાછા strbuf_detach(&ident, ખાલી);
}

સ્થિર શૂન્ય parse_and_store_blob(
	સંરચના last_object *last,
	સંરચના object_id *oidout,
	uintmax_t mark)
{
	સ્થિર સંરચના strbuf buf = STRBUF_INIT;
	uintmax_t len;

	જો (parse_data(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	નહીંતર {
		જો (last) {
			strbuf_release(&last->data);
			last->offset = 0;
			last->depth = 0;
		}
		stream_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

સ્થિર શૂન્ય parse_new_blob(સંરચના fast_import_state *state)
{
	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&last_blob, ખાલી, next_mark);
}

સ્થિર શૂન્ય unload_one_branch(શૂન્ય)
{
	જ્યારે (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = ULONG_MAX;
		સંરચના branch *e, *l = ખાલી, *p = ખાલી;

		માટે (e = active_branches; e; e = e->active_next_branch) {
			જો (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		જો (p) {
			e = p->active_next_branch;
			p->active_next_branch = e->active_next_branch;
		} નહીંતર {
			e = active_branches;
			active_branches = e->active_next_branch;
		}
		e->active = 0;
		e->active_next_branch = ખાલી;
		જો (e->branch_વૃક્ષ.વૃક્ષ) {
			release_વૃક્ષ_content_recursive(e->branch_વૃક્ષ.વૃક્ષ);
			e->branch_વૃક્ષ.વૃક્ષ = ખાલી;
		}
		cur_active_branches--;
	}
}

સ્થિર શૂન્ય load_branch(સંરચના branch *b)
{
	load_વૃક્ષ(&b->branch_વૃક્ષ);
	જો (!b->active) {
		b->active = 1;
		b->active_next_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_ગણતરી++;
	}
}

સ્થિર unsigned અક્ષર convert_અંક_notes_to_fanout(uintmax_t અંક_notes)
{
	unsigned અક્ષર fanout = 0;
	જ્યારે ((અંક_notes >>= 8))
		fanout++;
	પાછા fanout;
}

સ્થિર શૂન્ય construct_path_with_fanout(અચલ અક્ષર *hex_sha1,
		unsigned અક્ષર fanout, અક્ષર *path)
{
	unsigned પૂર્ણાંક i = 0, j = 0;
	જો (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	જ્યારે (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	memcpy(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

સ્થિર uintmax_t do_change_note_fanout(
		સંરચના વૃક્ષ_entry *orig_મૂળ, સંરચના વૃક્ષ_entry *મૂળ,
		અક્ષર *hex_oid, unsigned પૂર્ણાંક hex_oid_len,
		અક્ષર *fullpath, unsigned પૂર્ણાંક fullpath_len,
		unsigned અક્ષર fanout)
{
	સંરચના વૃક્ષ_content *t;
	સંરચના વૃક્ષ_entry *e, leaf;
	unsigned પૂર્ણાંક i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t અંક_notes = 0;
	સંરચના object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	અક્ષર realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	અચલ unsigned hexsz = the_hash_algo->hexsz;

	જો (!મૂળ->વૃક્ષ)
		load_વૃક્ષ(મૂળ);
	t = મૂળ->વૃક્ષ;

	માટે (i = 0; t && i < t->entry_ગણતરી; i++) {
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
		જો (!e->versions[1].mode ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			ચાલુ;

		/* This _may_ be a note entry, or a subdir containing notes */
		memcpy(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		જો (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		memcpy(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		જો (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			જો (fanout == 0xff) {
				/* Counting mode, no rename */
				અંક_notes++;
				ચાલુ;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			જો (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				અંક_notes++;
				ચાલુ;
			}

			/* Rename fullpath to realpath */
			જો (!વૃક્ષ_content_remove(orig_મૂળ, fullpath, &leaf, 0))
				die(_("failed to remove path %s"), fullpath);
			વૃક્ષ_content_set(orig_મૂળ, realpath,
				&leaf.versions[1].oid,
				leaf.versions[1].mode,
				leaf.વૃક્ષ);
		} નહીંતર જો (S_ISDIR(e->versions[1].mode)) {
			/* This is a subdir that may contain note entries */
			અંક_notes += do_change_note_fanout(orig_મૂળ, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = મૂળ->વૃક્ષ;
	}
	પાછા અંક_notes;
}

સ્થિર uintmax_t change_note_fanout(સંરચના વૃક્ષ_entry *મૂળ,
		unsigned અક્ષર fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	અક્ષર hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	પાછા do_change_note_fanout(મૂળ, મૂળ, hex_oid, 0, path, 0, fanout);
}

સ્થિર પૂર્ણાંક parse_mapped_oid_hex(અચલ અક્ષર *hex, સંરચના object_id *oid, અચલ અક્ષર **end)
{
	પૂર્ણાંક algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, કદ(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	જો (algo == GIT_HASH_UNKNOWN)
		પાછા -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	જો (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		જો (hash_algos[algo].format_id == the_hash_algo->format_id)
			પાછા 0;
		પાછા -1;
	}
	oidcpy(oid, kh_કિંમત(sub_oid_map, it));
	પાછા 0;
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
સ્થિર uintmax_t parse_mark_ref(અચલ અક્ષર *p, અક્ષર **endptr)
{
	uintmax_t mark;

	પ્રતિજ્ઞા(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	જો (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	પાછા mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
સ્થિર uintmax_t parse_mark_ref_eol(અચલ અક્ષર *p)
{
	અક્ષર *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	જો (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	પાછા mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
સ્થિર uintmax_t parse_mark_ref_space(અચલ અક્ષર **p)
{
	uintmax_t mark;
	અક્ષર *end;

	mark = parse_mark_ref(*p, &end);
	જો (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	પાછા mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
સ્થિર શૂન્ય parse_path(સંરચના strbuf *sb, અચલ અક્ષર *p, અચલ અક્ષર **endp,
		પૂર્ણાંક is_last_field, અચલ અક્ષર *field)
{
	જો (*p == '"') {
		જો (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), field, command_buf.buf);
		જો (strlen(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), field, command_buf.buf);
	} નહીંતર {
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
સ્થિર શૂન્ય parse_path_eol(સંરચના strbuf *sb, અચલ અક્ષર *p, અચલ અક્ષર *field)
{
	અચલ અક્ષર *end;

	parse_path(sb, p, &end, 1, field);
	જો (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
સ્થિર શૂન્ય parse_path_space(સંરચના strbuf *sb, અચલ અક્ષર *p,
		અચલ અક્ષર **endp, અચલ અક્ષર *field)
{
	parse_path(sb, p, endp, 0, field);
	જો (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

સ્થિર શૂન્ય file_change_m(સંરચના fast_import_state *state,
			  અચલ અક્ષર *p,
			  સંરચના branch *b)
{
	સ્થિર સંરચના strbuf path = STRBUF_INIT;
	સંરચના object_entry *oe;
	સંરચના object_id oid;
	uint16_t mode, inline_data = 0;

	p = parse_mode(p, &mode);
	જો (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	પસંદગી (mode) {
	સ્થિતિ 0644:
	સ્થિતિ 0755:
		mode |= S_IFREG;
	સ્થિતિ S_IFREG | 0644:
	સ્થિતિ S_IFREG | 0755:
	સ્થિતિ S_IFLNK:
	સ્થિતિ S_IFDIR:
	સ્થિતિ S_IFGITLINK:
		/* ok */
		તોડો;
	મૂળભૂત:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	જો (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} નહીંતર જો (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = ખાલી; /* not used with inline_data, but makes gcc happy */
	} નહીંતર {
		જો (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		જો (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	જો (S_ISDIR(mode) &&
	    is_empty_વૃક્ષ_oid(&oid, the_repository->hash_algo) &&
	    *path.buf) {
		વૃક્ષ_content_remove(&b->branch_વૃક્ષ, path.buf, ખાલી, 0);
		પાછા;
	}

	જો (S_ISGITLINK(mode)) {
		જો (inline_data)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		નહીંતર જો (oe) {
			જો (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} નહીંતર જો (inline_data) {
		જો (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		જ્યારે (read_next_command(state) != EOF) {
			અચલ અક્ષર *v;
			જો (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			નહીંતર {
				parse_and_store_blob(&last_blob, &oid, 0);
				તોડો;
			}
		}
	} નહીંતર {
		ગણના object_type expected = S_ISDIR(mode) ?
						OBJ_વૃક્ષ: OBJ_BLOB;
		ગણના object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, ખાલી);
		જો (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(mode) ?  _("tree") : _("blob"),
			    command_buf.buf);
		જો (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	જો (!*path.buf) {
		વૃક્ષ_content_replace(&b->branch_વૃક્ષ, &oid, mode, ખાલી);
		પાછા;
	}

	જો (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	વૃક્ષ_content_set(&b->branch_વૃક્ષ, path.buf, &oid, mode, ખાલી);
}

સ્થિર શૂન્ય file_change_d(અચલ અક્ષર *p, સંરચના branch *b)
{
	સ્થિર સંરચના strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	વૃક્ષ_content_remove(&b->branch_વૃક્ષ, path.buf, ખાલી, 1);
}

સ્થિર શૂન્ય file_change_cr(અચલ અક્ષર *p, સંરચના branch *b, પૂર્ણાંક rename)
{
	સ્થિર સંરચના strbuf source = STRBUF_INIT;
	સ્થિર સંરચના strbuf dest = STRBUF_INIT;
	સંરચના વૃક્ષ_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	memset(&leaf, 0, કદ(leaf));
	જો (rename)
		વૃક્ષ_content_remove(&b->branch_વૃક્ષ, source.buf, &leaf, 1);
	નહીંતર
		વૃક્ષ_content_get(&b->branch_વૃક્ષ, source.buf, &leaf, 1);
	જો (!leaf.versions[1].mode)
		die(_("path %s not in branch"), source.buf);
	જો (!*dest.buf) {	/* C "path/to/subdir" "" */
		વૃક્ષ_content_replace(&b->branch_વૃક્ષ,
			&leaf.versions[1].oid,
			leaf.versions[1].mode,
			leaf.વૃક્ષ);
		પાછા;
	}
	જો (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	વૃક્ષ_content_set(&b->branch_વૃક્ષ, dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.વૃક્ષ);
}

સ્થિર શૂન્ય note_change_n(સંરચના fast_import_state *state,
			  અચલ અક્ષર *p,
			  સંરચના branch *b,
			  unsigned અક્ષર *old_fanout)
{
	સંરચના object_entry *oe;
	સંરચના branch *s;
	સંરચના object_id oid, commit_oid;
	અક્ષર path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_data = 0;
	unsigned અક્ષર new_fanout;

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
	જો (b->અંક_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->અંક_notes = change_note_fanout(&b->branch_વૃક્ષ, 0xff);
		*old_fanout = convert_અંક_notes_to_fanout(b->અંક_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	જો (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} નહીંતર જો (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = ખાલી; /* not used with inline_data, but makes gcc happy */
	} નહીંતર {
		જો (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		જો (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	જો (s) {
		જો (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} નહીંતર જો (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		સંરચના object_entry *commit_oe = find_mark(marks, commit_mark);
		જો (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} નહીંતર જો (!repo_get_oid(the_repository, p, &commit_oid)) {
		size_t size;
		અક્ષર *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &size,
						   &commit_oid);
		જો (!buf || size < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		મુક્ત(buf);
	} નહીંતર
		die(_("invalid ref name or SHA1 expression: %s"), p);

	જો (inline_data) {
		read_next_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} નહીંતર જો (oe) {
		જો (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} નહીંતર જો (!is_null_oid(&oid)) {
		ગણના object_type type = odb_read_object_info(the_repository->objects, &oid,
							ખાલી);
		જો (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		જો (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	જો (વૃક્ષ_content_remove(&b->branch_વૃક્ષ, path, ખાલી, 0))
		b->અંક_notes--;

	જો (is_null_oid(&oid))
		પાછા; /* nothing to insert */

	b->અંક_notes++;
	new_fanout = convert_અંક_notes_to_fanout(b->અંક_notes);
	construct_path_with_fanout(oid_to_hex(&commit_oid), new_fanout, path);
	વૃક્ષ_content_set(&b->branch_વૃક્ષ, path, &oid, S_IFREG | 0644, ખાલી);
}

સ્થિર શૂન્ય file_change_deleteall(સંરચના branch *b)
{
	release_વૃક્ષ_content_recursive(b->branch_વૃક્ષ.વૃક્ષ);
	oidclr(&b->branch_વૃક્ષ.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_વૃક્ષ.versions[1].oid, the_repository->hash_algo);
	load_વૃક્ષ(&b->branch_વૃક્ષ);
	b->અંક_notes = 0;
}

સ્થિર શૂન્ય parse_from_commit(સંરચના branch *b, અક્ષર *buf, unsigned long size)
{
	જો (!buf || size < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	જો (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_વૃક્ષ.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_વૃક્ષ.versions[0].oid,
	       &b->branch_વૃક્ષ.versions[1].oid);
}

સ્થિર શૂન્ય parse_from_existing(સંરચના branch *b)
{
	જો (is_null_oid(&b->oid)) {
		oidclr(&b->branch_વૃક્ષ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_વૃક્ષ.versions[1].oid, the_repository->hash_algo);
	} નહીંતર {
		unsigned long size;
		size_t size_st = 0;
		અક્ષર *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &size_st, &b->oid);
		size = cast_size_t_to_ulong(size_st);
		parse_from_commit(b, buf, size);
		મુક્ત(buf);
	}
}

સ્થિર પૂર્ણાંક parse_objectish(સંરચના fast_import_state *state,
			   સંરચના branch *b,
			   અચલ અક્ષર *objectish)
{
	સંરચના branch *s;
	સંરચના object_id oid;

	oidcpy(&oid, &b->branch_વૃક્ષ.versions[1].oid);

	s = lookup_branch(objectish);
	જો (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	નહીંતર જો (s) {
		સંરચના object_id *t = &s->branch_વૃક્ષ.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_વૃક્ષ.versions[0].oid, t);
		oidcpy(&b->branch_વૃક્ષ.versions[1].oid, t);
	} નહીંતર જો (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		સંરચના object_entry *oe = find_mark(marks, idnum);
		જો (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		જો (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			જો (oe->pack_id != MAX_PACK_ID) {
				unsigned long size;
				અક્ષર *buf = gfi_unpack_entry(oe, &size);
				parse_from_commit(b, buf, size);
				મુક્ત(buf);
			} નહીંતર
				parse_from_existing(b);
		}
	} નહીંતર જો (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		જો (is_null_oid(&b->oid))
			b->delete = 1;
	}
	નહીંતર
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	જો (b->branch_વૃક્ષ.વૃક્ષ && !oideq(&oid, &b->branch_વૃક્ષ.versions[1].oid)) {
		release_વૃક્ષ_content_recursive(b->branch_વૃક્ષ.વૃક્ષ);
		b->branch_વૃક્ષ.વૃક્ષ = ખાલી;
	}

	read_next_command(state);
	પાછા 1;
}

સ્થિર પૂર્ણાંક parse_from(સંરચના fast_import_state *state, સંરચના branch *b)
{
	અચલ અક્ષર *from;

	જો (!skip_prefix(command_buf.buf, "from ", &from))
		પાછા 0;

	પાછા parse_objectish(state, b, from);
}

સ્થિર પૂર્ણાંક parse_objectish_with_prefix(સંરચના fast_import_state *state,
				       સંરચના branch *b,
				       અચલ અક્ષર *prefix)
{
	અચલ અક્ષર *base;

	જો (!skip_prefix(command_buf.buf, prefix, &base))
		પાછા 0;

	પાછા parse_objectish(state, b, base);
}

સ્થિર સંરચના hash_list *parse_merge(સંરચના fast_import_state *state,
				     unsigned પૂર્ણાંક *ગણતરી)
{
	સંરચના hash_list *list = ખાલી, **tail = &list, *n;
	અચલ અક્ષર *from;
	સંરચના branch *s;

	*ગણતરી = 0;
	જ્યારે (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(કદ(*n));
		s = lookup_branch(from);
		જો (s)
			oidcpy(&n->oid, &s->oid);
		નહીંતર જો (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			સંરચના object_entry *oe = find_mark(marks, idnum);
			જો (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} નહીંતર જો (!repo_get_oid(the_repository, from, &n->oid)) {
			size_t size;
			અક્ષર *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &size, &n->oid);
			જો (!buf || size < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			મુક્ત(buf);
		} નહીંતર
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->next = ખાલી;
		*tail = n;
		tail = &n->next;

		(*ગણતરી)++;
		read_next_command(state);
	}
	પાછા list;
}

સંરચના signature_data {
	અક્ષર *hash_algo;      /* "sha1" or "sha256" */
	અક્ષર *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	સંરચના strbuf data;   /* The actual signature data */
};

સ્થિર શૂન્ય parse_one_signature(સંરચના fast_import_state *state,
				સંરચના signature_data *sig,
				અચલ અક્ષર *v)
{
	અક્ષર *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	અક્ષર *space = strchr(args, ' ');

	જો (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), args);
	*space = '\0';

	sig->hash_algo = args;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	જો (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	જો (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	જો (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_next_command(state);
	parse_data(&sig->data, 0, ખાલી);
}

સ્થિર શૂન્ય discard_one_signature(સંરચના fast_import_state *state)
{
	સંરચના strbuf data = STRBUF_INIT;

	read_next_command(state);
	parse_data(&data, 0, ખાલી);
	strbuf_release(&data);
}

સ્થિર શૂન્ય add_gpgsig_to_commit(સંરચના strbuf *commit_data,
				 અચલ અક્ષર *header,
				 સંરચના signature_data *sig)
{
	સંરચના string_list siglines = STRING_LIST_INIT_NODUP;

	જો (!sig || !sig->hash_algo)
		પાછા;

	strbuf_addstr(commit_data, header);
	string_list_split_in_place(&siglines, sig->data.buf, "\n", -1);
	strbuf_add_separated_string_list(commit_data, "\n ", &siglines);
	strbuf_addch(commit_data, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->data);
	મુક્ત(sig->hash_algo);
}

સ્થિર શૂન્ય store_signature(સંરચના signature_data *stored_sig,
			    સંરચના signature_data *new_sig,
			    અચલ અક્ષર *hash_type)
{
	જો (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->data);
		મુક્ત(new_sig->hash_algo);
	} નહીંતર {
		*stored_sig = *new_sig;
	}
}

સ્થિર શૂન્ય import_one_signature(સંરચના fast_import_state *state,
				 સંરચના signature_data *sig_sha1,
				 સંરચના signature_data *sig_sha256,
				 અચલ અક્ષર *v)
{
	સંરચના signature_data sig = { ખાલી, ખાલી, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	જો (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	નહીંતર જો (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	નહીંતર
		die(_("parse_one_signature() returned unknown hash algo"));
}

સ્થિર શૂન્ય finalize_commit_buffer(સંરચના strbuf *new_data,
				   સંરચના signature_data *sig_sha1,
				   સંરચના signature_data *sig_sha256,
				   સંરચના strbuf *સંદેશ)
{
	add_gpgsig_to_commit(new_data, "gpgsig ", sig_sha1);
	add_gpgsig_to_commit(new_data, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_data, '\n');
	strbuf_addbuf(new_data, સંદેશ);
}

સ્થિર શૂન્ય warn_invalid_signature(સંરચના signature_check *check,
				   અચલ અક્ષર *સંદેશ, ગણના sign_mode mode)
{
	અચલ અક્ષર *signer = check->signer ? check->signer : _("unknown");
	અચલ અક્ષર *subject;
	પૂર્ણાંક subject_len = find_commit_subject(સંદેશ, &subject);

	પસંદગી (mode) {
	સ્થિતિ SIGN_STRIP_IF_INVALID:
		જો (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		નહીંતર જો (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		નહીંતર
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		તોડો;
	સ્થિતિ SIGN_SIGN_IF_INVALID:
		જો (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		નહીંતર જો (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		નહીંતર
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		તોડો;
	મૂળભૂત:
		BUG("unsupported signing mode");
	}
}

સ્થિર શૂન્ય handle_signature_if_invalid(સંરચના strbuf *new_data,
					સંરચના signature_data *sig_sha1,
					સંરચના signature_data *sig_sha256,
					સંરચના strbuf *સંદેશ,
					ગણના sign_mode mode)
{
	સંરચના strbuf tmp_buf = STRBUF_INIT;
	સંરચના signature_check signature_check = { 0 };
	પૂર્ણાંક ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_data);
	finalize_commit_buffer(&tmp_buf, sig_sha1, sig_sha256, સંદેશ);
	ret = verify_commit_buffer(tmp_buf.buf, tmp_buf.len, &signature_check);

	જો (ret) {
		જો (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, સંદેશ->buf, mode);

		જો (mode == SIGN_SIGN_IF_INVALID) {
			સંરચના strbuf signature = STRBUF_INIT;
			સંરચના strbuf payload = STRBUF_INIT;

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
			જો (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			જો (sign_buffer(&payload, &signature, signed_commit_keyid,
					SIGN_BUFFER_USE_DEFAULT_ચાવી))
				die(_("failed to sign commit object"));
			add_header_signature(new_data, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_buffer(new_data, ખાલી, ખાલી, સંદેશ);
	} નહીંતર {
		strbuf_swap(new_data, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

સ્થિર શૂન્ય parse_new_commit(સંરચના fast_import_state *state, અચલ અક્ષર *arg)
{
	સ્થિર સંરચના strbuf સંદેશ = STRBUF_INIT;
	સંરચના signature_data sig_sha1 = { ખાલી, ખાલી, STRBUF_INIT };
	સંરચના signature_data sig_sha256 = { ખાલી, ખાલી, STRBUF_INIT };
	સંરચના branch *b;
	અક્ષર *author = ખાલી;
	અક્ષર *committer = ખાલી;
	અક્ષર *encoding = ખાલી;
	સંરચના hash_list *merge_list = ખાલી;
	unsigned પૂર્ણાંક merge_ગણતરી;
	unsigned અક્ષર prev_fanout, new_fanout;
	અચલ અક્ષર *v;

	b = lookup_branch(arg);
	જો (!b)
		b = new_branch(arg);

	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	જો (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_next_command(state);
	}
	જો (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_next_command(state);
	}
	જો (!committer)
		die(_("expected committer but didn't get one"));

	જ્યારે (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		પસંદગી (signed_commit_mode) {

		/* First, modes that don't need the signature to be parsed */
		સ્થિતિ SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		સ્થિતિ SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		સ્થિતિ SIGN_STRIP:
			discard_one_signature(state);
			તોડો;

		/* Second, modes that parse the signature */
		સ્થિતિ SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		સ્થિતિ SIGN_VERBATIM:
		સ્થિતિ SIGN_STRIP_IF_INVALID:
		સ્થિતિ SIGN_SIGN_IF_INVALID:
		સ્થિતિ SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			તોડો;

		/* Third, BUG */
		મૂળભૂત:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_next_command(state);
	}

	જો (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_next_command(state);
	}
	parse_data(&સંદેશ, 0, ખાલી);
	read_next_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_ગણતરી);

	/* ensure the branch is active/loaded */
	જો (!b->branch_વૃક્ષ.વૃક્ષ || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	prev_fanout = convert_અંક_notes_to_fanout(b->અંક_notes);

	/* file_change* */
	જ્યારે (command_buf.len > 0) {
		જો (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		નહીંતર જો (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		નહીંતર જો (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		નહીંતર જો (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		નહીંતર જો (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		નહીંતર જો (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		નહીંતર જો (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		નહીંતર જો (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		નહીંતર {
			unread_command_buf = 1;
			તોડો;
		}
		જો (read_next_command(state) == EOF)
			તોડો;
	}

	new_fanout = convert_અંક_notes_to_fanout(b->અંક_notes);
	જો (new_fanout != prev_fanout)
		b->અંક_notes = change_note_fanout(&b->branch_વૃક્ષ, new_fanout);

	/* build the tree and the commit */
	store_વૃક્ષ(&b->branch_વૃક્ષ);
	oidcpy(&b->branch_વૃક્ષ.versions[0].oid,
	       &b->branch_વૃક્ષ.versions[1].oid);

	strbuf_reset(&new_data);
	strbuf_addf(&new_data, "tree %s\n",
		oid_to_hex(&b->branch_વૃક્ષ.versions[1].oid));
	જો (!is_null_oid(&b->oid))
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&b->oid));
	જ્યારે (merge_list) {
		સંરચના hash_list *next = merge_list->next;
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&merge_list->oid));
		મુક્ત(merge_list);
		merge_list = next;
	}
	strbuf_addf(&new_data,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	જો (encoding)
		strbuf_addf(&new_data,
			"encoding %s\n",
			encoding);

	જો ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
	     signed_commit_mode == SIGN_SIGN_IF_INVALID ||
	     signed_commit_mode == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_data, &sig_sha1, &sig_sha256,
					    &સંદેશ, signed_commit_mode);
	નહીંતર
		finalize_commit_buffer(&new_data, &sig_sha1, &sig_sha256, &સંદેશ);

	મુક્ત(author);
	મુક્ત(committer);
	મુક્ત(encoding);

	જો (!store_object(OBJ_COMMIT, &new_data, ખાલી, &b->oid, next_mark))
		b->pack_id = pack_id;
	b->last_commit = object_ગણતરી_by_type[OBJ_COMMIT];
}

સ્થિર શૂન્ય handle_tag_signature_if_invalid(સંરચના strbuf *buf,
					    સંરચના strbuf *સંદેશ,
					    size_t sig_offset)
{
	સંરચના strbuf signature = STRBUF_INIT;
	સંરચના strbuf payload = STRBUF_INIT;
	સંરચના signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_add(&payload, સંદેશ->buf, sig_offset);
	strbuf_add(&signature, સંદેશ->buf + sig_offset, સંદેશ->len - sig_offset);

	sigc.payload_type = SIGNATURE_PAYLOAD_TAG;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	જો (!check_signature(&sigc, signature.buf, signature.len))
		જાઓ out;

	જો (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(સંદેશ, sig_offset);

	જો (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = ખાલી;
		strbuf_reset(&signature);

		જો (sign_buffer(&payload, &signature, signed_tag_keyid,
				SIGN_BUFFER_USE_DEFAULT_ચાવી))
			die(_("failed to sign tag object"));

		strbuf_addbuf(સંદેશ, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

સ્થિર શૂન્ય handle_tag_signature(સંરચના strbuf *buf, સંરચના strbuf *સંદેશ, અચલ અક્ષર *name)
{
	size_t sig_offset = parse_signed_buffer(સંદેશ->buf, સંદેશ->len);

	/* If there is no signature, there is nothing to do. */
	જો (sig_offset >= સંદેશ->len)
		પાછા;

	પસંદગી (signed_tag_mode) {

	/* First, modes that don't change anything */
	સ્થિતિ SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	સ્થિતિ SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		તોડો;

	/* Second, modes that remove the signature */
	સ્થિતિ SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	સ્થિતિ SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(સંદેશ, sig_offset);
		તોડો;
	સ્થિતિ SIGN_ABORT_IF_INVALID:
	સ્થિતિ SIGN_SIGN_IF_INVALID:
	સ્થિતિ SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, સંદેશ, sig_offset);
		તોડો;

	/* Third, aborting modes */
	સ્થિતિ SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	મૂળભૂત:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

સ્થિર શૂન્ય parse_new_tag(સંરચના fast_import_state *state, અચલ અક્ષર *arg)
{
	સ્થિર સંરચના strbuf સંદેશ = STRBUF_INIT;
	અચલ અક્ષર *from;
	અક્ષર *tagger;
	સંરચના branch *s;
	સંરચના tag *t;
	uintmax_t from_mark = 0;
	સંરચના object_id oid;
	ગણના object_type type;
	અચલ અક્ષર *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, કદ(સંરચના tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	જો (last_tag)
		last_tag->next_tag = t;
	નહીંતર
		first_tag = t;
	last_tag = t;
	read_next_command(state);
	parse_mark(state);

	/* from ... */
	જો (!skip_prefix(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	જો (s) {
		જો (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} નહીંતર જો (*from == ':') {
		સંરચના object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} નહીંતર જો (!repo_get_oid(the_repository, from, &oid)) {
		સંરચના object_entry *oe = find_object(&oid);
		જો (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, ખાલી);
			જો (type < 0)
				die(_("not a valid object: %s"), from);
		} નહીંતર
			type = oe->type;
	} નહીંતર
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_next_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	જો (skip_prefix(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_next_command(state);
	} નહીંતર
		tagger = ખાલી;

	/* tag payload/message */
	parse_data(&સંદેશ, 0, ખાલી);

	/* build the tag object */
	strbuf_reset(&new_data);

	strbuf_addf(&new_data,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_to_hex(&oid), type_name(type), t->name);
	જો (tagger)
		strbuf_addf(&new_data,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_data, &સંદેશ, t->name);

	strbuf_addch(&new_data, '\n');
	strbuf_addbuf(&new_data, &સંદેશ);
	મુક્ત(tagger);

	જો (store_object(OBJ_TAG, &new_data, ખાલી, &t->oid, next_mark))
		t->pack_id = MAX_PACK_ID;
	નહીંતર
		t->pack_id = pack_id;
}

સ્થિર શૂન્ય parse_reset_branch(સંરચના fast_import_state *state, અચલ અક્ષર *arg)
{
	સંરચના branch *b;
	અચલ અક્ષર *tag_name;

	b = lookup_branch(arg);
	જો (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_વૃક્ષ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_વૃક્ષ.versions[1].oid, the_repository->hash_algo);
		જો (b->branch_વૃક્ષ.વૃક્ષ) {
			release_વૃક્ષ_content_recursive(b->branch_વૃક્ષ.વૃક્ષ);
			b->branch_વૃક્ષ.વૃક્ષ = ખાલી;
		}
	}
	નહીંતર
		b = new_branch(arg);
	read_next_command(state);
	parse_from(state, b);
	જો (b->delete && skip_prefix(b->name, "refs/tags/", &tag_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		સંરચના tag *t, *prev = ખાલી;
		માટે (t = first_tag; t; t = t->next_tag) {
			જો (!strcmp(t->name, tag_name))
				તોડો;
			prev = t;
		}
		જો (t) {
			જો (prev)
				prev->next_tag = t->next_tag;
			નહીંતર
				first_tag = t->next_tag;
			જો (!t->next_tag)
				last_tag = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	જો (command_buf.len > 0)
		unread_command_buf = 1;
}

સ્થિર શૂન્ય cat_blob_write(અચલ અક્ષર *buf, unsigned long size)
{
	જો (write_in_full(cat_blob_fd, buf, size) < 0)
		die_errno(_("write to frontend failed"));
}

સ્થિર શૂન્ય cat_blob(સંરચના object_entry *oe, સંરચના object_id *oid)
{
	સંરચના strbuf લીટી = STRBUF_INIT;
	સંરચના iovec iov[3];
	unsigned long size;
	ગણના object_type type = 0;
	અક્ષર *buf;

	જો (!oe || oe->pack_id == MAX_PACK_ID) {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
	} નહીંતર {
		type = oe->type;
		buf = gfi_unpack_entry(oe, &size);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	જો (type <= 0) {
		strbuf_reset(&લીટી);
		strbuf_addf(&લીટી, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(લીટી.buf, લીટી.len);
		strbuf_release(&લીટી);
		મુક્ત(buf);
		પાછા;
	}
	જો (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	જો (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_to_hex(oid), type_name(type));
	strbuf_reset(&લીટી);
	strbuf_addf(&લીટી, "%s %s %"PRIuMAX"\n", oid_to_hex(oid),
		    type_name(type), (uintmax_t)size);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_base = લીટી.buf;
	iov[0].iov_len = લીટી.len;
	iov[1].iov_base = buf;
	iov[1].iov_len = size;
	iov[2].iov_base = (શૂન્ય *) "\n";
	iov[2].iov_len = 1;

	જો (writev_in_full(cat_blob_fd, iov, ARRAY_SIZE(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&લીટી);
	જો (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.data, buf, size, size + 1);
		last_blob.depth = oe->depth;
	} નહીંતર
		મુક્ત(buf);
}

સ્થિર શૂન્ય parse_get_mark(સંરચના fast_import_state *state UNUSED,
			   અચલ અક્ષર *p)
{
	સંરચના object_entry *oe;
	અક્ષર output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	જો (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	જો (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, કદ(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

સ્થિર શૂન્ય parse_cat_blob(સંરચના fast_import_state *state UNUSED,
			   અચલ અક્ષર *p)
{
	સંરચના object_entry *oe;
	સંરચના object_id oid;

	/* cat-blob SP <object> LF */
	જો (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		જો (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} નહીંતર {
		જો (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		જો (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

સ્થિર સંરચના object_entry *dereference(સંરચના object_entry *oe,
					સંરચના object_id *oid)
{
	unsigned long size;
	અક્ષર *buf = ખાલી;
	અચલ unsigned hexsz = the_hash_algo->hexsz;

	જો (!oe) {
		ગણના object_type type = odb_read_object_info(the_repository->objects,
							     oid, ખાલી);
		જો (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = ઉમેરો_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	પસંદગી (oe->type) {
	સ્થિતિ OBJ_વૃક્ષ:	/* easy case. */
		પાછા oe;
	સ્થિતિ OBJ_COMMIT:
	સ્થિતિ OBJ_TAG:
		તોડો;
	મૂળભૂત:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	જો (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &size);
	} નહીંતર {
		ગણના object_type unused;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &size_st);
		size = cast_size_t_to_ulong(size_st);
	}
	જો (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	પસંદગી (oe->type) {
	સ્થિતિ OBJ_TAG:
		જો (size < hexsz + strlen("object ") ||
		    get_oid_hex(buf + strlen("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		તોડો;
	સ્થિતિ OBJ_COMMIT:
		જો (size < hexsz + strlen("tree ") ||
		    get_oid_hex(buf + strlen("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	મુક્ત(buf);
	પાછા find_object(oid);
}

સ્થિર શૂન્ય ઉમેરો_mapped_mark(uintmax_t mark, શૂન્ય *object, શૂન્ય *cbp)
{
	સંરચના object_id *fromoid = object;
	સંરચના object_id *tooid = find_mark(cbp, mark);
	પૂર્ણાંક ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	જો (ret == 0)
		પાછા;
	kh_કિંમત(sub_oid_map, it) = tooid;
}

સ્થિર શૂન્ય build_mark_map_one(સંરચના mark_set *from, સંરચના mark_set *to)
{
	for_each_mark(from, 0, ઉમેરો_mapped_mark, to);
}

સ્થિર શૂન્ય build_mark_map(સંરચના string_list *from, સંરચના string_list *to)
{
	સંરચના string_list_item *fromp, *ટોચ;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		ટોચ = string_list_lookup(to, fromp->string);
		જો (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} નહીંતર જો (!ટોચ || !ટોચ->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, ટોચ->util);
	}
}

સ્થિર સંરચના object_entry *parse_treeish_dataref(અચલ અક્ષર **p)
{
	સંરચના object_id oid;
	સંરચના object_entry *e;

	જો (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		જો (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} નહીંતર {	/* <sha1> */
		જો (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		જો (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	જ્યારે (!e || e->type != OBJ_વૃક્ષ)
		e = dereference(e, &oid);
	પાછા e;
}

સ્થિર શૂન્ય print_ls(પૂર્ણાંક mode, અચલ unsigned અક્ષર *hash, અચલ અક્ષર *path)
{
	સ્થિર સંરચના strbuf લીટી = STRBUF_INIT;

	/* See show_tree(). */
	અચલ અક્ષર *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? વૃક્ષ_type :
		blob_type;

	જો (!mode) {
		/* missing SP path LF */
		strbuf_reset(&લીટી);
		strbuf_addstr(&લીટી, "missing ");
		quote_c_style(path, &લીટી, ખાલી, 0);
		strbuf_addch(&લીટી, '\n');
	} નહીંતર {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&લીટી);
		strbuf_addf(&લીટી, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &લીટી, ખાલી, 0);
		strbuf_addch(&લીટી, '\n');
	}
	cat_blob_write(લીટી.buf, લીટી.len);
}

સ્થિર શૂન્ય parse_ls(સંરચના fast_import_state *state UNUSED,
		     અચલ અક્ષર *p,
		     સંરચના branch *b)
{
	સ્થિર સંરચના strbuf path = STRBUF_INIT;
	સંરચના વૃક્ષ_entry *મૂળ = ખાલી;
	સંરચના વૃક્ષ_entry leaf = {ખાલી};

	/* ls SP (<tree-ish> SP)? <path> */
	જો (*p == '"') {
		જો (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		મૂળ = &b->branch_વૃક્ષ;
	} નહીંતર {
		સંરચના object_entry *e = parse_treeish_dataref(&p);
		મૂળ = new_વૃક્ષ_entry();
		oidcpy(&મૂળ->versions[1].oid, &e->idx.oid);
		જો (!is_null_oid(&મૂળ->versions[1].oid))
			મૂળ->versions[1].mode = S_IFDIR;
		load_વૃક્ષ(મૂળ);
	}
	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	વૃક્ષ_content_get(મૂળ, path.buf, &leaf, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	જો (S_ISDIR(leaf.versions[1].mode))
		store_વૃક્ષ(&leaf);

	print_ls(leaf.versions[1].mode, leaf.versions[1].oid.hash, path.buf);
	જો (leaf.વૃક્ષ)
		release_વૃક્ષ_content_recursive(leaf.વૃક્ષ);
	જો (!b || મૂળ != &b->branch_વૃક્ષ)
		release_વૃક્ષ_entry(મૂળ);
}

સ્થિર શૂન્ય checkpoint(શૂન્ય)
{
	checkpoint_requested = 0;
	જો (object_ગણતરી) {
		cycle_packfile();
	}
	dump_branches();
	dump_tags();
	dump_marks();
}

સ્થિર શૂન્ય parse_checkpoint(સંરચના fast_import_state *state UNUSED)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

સ્થિર શૂન્ય parse_progress(સંરચના fast_import_state *state UNUSED)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

સ્થિર શૂન્ય parse_alias(સંરચના fast_import_state *state)
{
	સંરચના object_entry *e;
	સંરચના branch b;

	skip_optional_lf();
	read_next_command(state);

	/* mark ... */
	parse_mark(state);
	જો (!next_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, કદ(b));
	જો (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	પ્રતિજ્ઞા(e);
	ઉમેરો_mark(&marks, next_mark, e);
}

સ્થિર અક્ષર* make_fast_import_path(સંરચના fast_import_state *state,
				   અચલ અક્ષર *path)
{
	જો (!relative_marks_paths || is_absolute_path(path))
		પાછા prefix_filename(state->prefix, path);
	પાછા repo_git_path(the_repository, "info/fast-import/%s", path);
}

સ્થિર શૂન્ય option_import_marks(સંરચના fast_import_state *state,
				અચલ અક્ષર *marks,
				પૂર્ણાંક from_stream,
				પૂર્ણાંક ignore_missing)
{
	જો (import_marks_file) {
		જો (from_stream)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		જો(!import_marks_file_from_stream)
			read_marks();
	}

	મુક્ત(import_marks_file);
	import_marks_file = make_fast_import_path(state, marks);
	import_marks_file_from_stream = from_stream;
	import_marks_file_ignore_missing = ignore_missing;
}

સ્થિર શૂન્ય option_date_format(અચલ અક્ષર *fmt)
{
	જો (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	નહીંતર જો (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	નહીંતર જો (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	નહીંતર જો (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	નહીંતર
		die(_("unknown --date-format argument %s"), fmt);
}

સ્થિર unsigned long ulong_arg(અચલ અક્ષર *option, અચલ અક્ષર *arg)
{
	અક્ષર *endptr;
	unsigned long rv = strtoul(arg, &endptr, 0);
	જો (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	પાછા rv;
}

સ્થિર શૂન્ય option_depth(અચલ અક્ષર *depth)
{
	max_depth = ulong_arg("--depth", depth);
	જો (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

સ્થિર શૂન્ય option_active_branches(અચલ અક્ષર *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

સ્થિર શૂન્ય option_export_marks(સંરચના fast_import_state *state,
				અચલ અક્ષર *marks)
{
	મુક્ત(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

સ્થિર શૂન્ય option_cat_blob_fd(સંરચના fast_import_state *state UNUSED,
			       અચલ અક્ષર *fd)
{
	unsigned long n = ulong_arg("--cat-blob-fd", fd);
	જો (n > (unsigned long) INT_MAX)
		die(_("--cat-blob-fd cannot exceed %d"), INT_MAX);
	cat_blob_fd = (પૂર્ણાંક) n;
}

સ્થિર શૂન્ય option_export_pack_edges(સંરચના fast_import_state *state,
				     અચલ અક્ષર *edges)
{
	અક્ષર *fn = prefix_filename(state->prefix, edges);
	જો (pack_edges)
		fclose(pack_edges);
	pack_edges = xfopen(fn, "a");
	મુક્ત(fn);
}

સ્થિર શૂન્ય option_rewrite_submodules(સંરચના fast_import_state *state,
				      અચલ અક્ષર *arg,
				      સંરચના string_list *list)
{
	સંરચના mark_set *ms;
	FILE *fp;
	અક્ષર *s = xstrdup(arg);
	અક્ષર *f = strchr(s, ':');
	જો (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = prefix_filename(state->prefix, f);
	fp = fopen(f, "r");
	જો (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, ઉમેરો_oid_entry);
	fclose(fp);
	મુક્ત(f);

	string_list_ઉમેરો(list, s)->util = ms;

	મુક્ત(s);
}

સ્થિર શૂન્ય option_max_pack_size(અચલ અક્ષર *arg)
{
	unsigned long v;

	જો (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	જો (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} નહીંતર જો (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

સ્થિર શૂન્ય option_big_file_threshold(અચલ અક્ષર *arg)
{
	unsigned long v;

	જો (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

સ્થિર શૂન્ય option_signed_commits(અચલ અક્ષર *arg)
{
	જો (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

સ્થિર શૂન્ય option_signed_tags(અચલ અક્ષર *arg)
{
	જો (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

સ્થિર શૂન્ય option_quiet(શૂન્ય)
{
	show_stats = 0;
	quiet = 1;
}

સ્થિર પૂર્ણાંક parse_one_option(સંરચના fast_import_state *state, અચલ અક્ષર *option)
{
	જો (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_size(option);
	} નહીંતર જો (skip_prefix(option, "big-file-threshold=", &option)) {
		option_big_file_threshold(option);
	} નહીંતર જો (skip_prefix(option, "depth=", &option)) {
		option_depth(option);
	} નહીંતર જો (skip_prefix(option, "active-branches=", &option)) {
		option_active_branches(option);
	} નહીંતર જો (skip_prefix(option, "export-pack-edges=", &option)) {
		option_export_pack_edges(state, option);
	} નહીંતર જો (skip_prefix(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} નહીંતર જો (skip_prefix(option, "signed-tags=", &option)) {
		option_signed_tags(option);
	} નહીંતર જો (!strcmp(option, "quiet")) {
		option_quiet();
	} નહીંતર જો (!strcmp(option, "stats")) {
		show_stats = 1;
	} નહીંતર જો (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} નહીંતર {
		પાછા 0;
	}

	પાછા 1;
}

સ્થિર શૂન્ય check_unsafe_feature(સંરચના fast_import_state *state,
				 અચલ અક્ષર *feature)
{
	જો (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

સ્થિર પૂર્ણાંક parse_one_feature(સંરચના fast_import_state *state,
			     અચલ અક્ષર *feature)
{
	અચલ અક્ષર *arg;

	જો (skip_prefix(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} નહીંતર જો (skip_prefix(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} નહીંતર જો (skip_prefix(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} નહીંતર જો (skip_prefix(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} નહીંતર જો (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} નહીંતર જો (skip_prefix(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_to);
	} નહીંતર જો (skip_prefix(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} નહીંતર જો (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} નહીંતર જો (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} નહીંતર જો (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} નહીંતર જો (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} નહીંતર જો (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} નહીંતર જો (!strcmp(feature, "force")) {
		force_update = 1;
	} નહીંતર જો (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} નહીંતર {
		પાછા 0;
	}

	પાછા 1;
}

સ્થિર શૂન્ય parse_feature(સંરચના fast_import_state *state, અચલ અક્ષર *feature)
{
	જો (state->seen_data_command)
		die(_("got feature command '%s' after data command"), feature);

	જો (parse_one_feature(state, feature))
		પાછા;

	die(_("this version of fast-import does not support feature %s."), feature);
}

સ્થિર શૂન્ય parse_option(સંરચના fast_import_state *state, અચલ અક્ષર *option)
{
	જો (state->seen_data_command)
		die(_("got option command '%s' after data command"), option);

	જો (parse_one_option(state, option))
		પાછા;

	die(_("this version of fast-import does not support option: %s"), option);
}

સ્થિર શૂન્ય git_pack_config(શૂન્ય)
{
	પૂર્ણાંક indexversion_કિંમત;
	પૂર્ણાંક limit;
	unsigned long packsizelimit_કિંમત;

	જો (!repo_config_get_ulong(the_repository, "pack.depth", &max_depth)) {
		જો (max_depth > MAX_DEPTH)
			max_depth = MAX_DEPTH;
	}
	જો (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_કિંમત)) {
		pack_idx_opts.version = indexversion_કિંમત;
		જો (pack_idx_opts.version > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.version);
	}
	જો (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_કિંમત))
		max_packsize = packsizelimit_કિંમત;

	જો (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &limit))
		unpack_limit = limit;
	નહીંતર જો (!repo_config_get_int(the_repository, "transfer.unpacklimit", &limit))
		unpack_limit = limit;

	repo_config(the_repository, git_default_config, ખાલી);
}

સ્થિર અચલ અક્ષર *અચલ fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	ખાલી
};

સ્થિર શૂન્ય parse_argv(સંરચના fast_import_state *state)
{
	પૂર્ણાંક argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	જો (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_data_command = 1;
	જો (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

સ્થિર પૂર્ણાંક option_parse_date_format(અચલ સંરચના option *opt UNUSED,
				    અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_export_pack_edges(અચલ સંરચના option *opt,
					  અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->કિંમત, arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_max_pack_size(અચલ સંરચના option *opt UNUSED,
				      અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_size(arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_big_file_threshold(અચલ સંરચના option *opt UNUSED,
					   અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_signed_commits(અચલ સંરચના option *opt UNUSED,
				       અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_signed_tags(અચલ સંરચના option *opt UNUSED,
				    અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_rewrite_submodules_from(અચલ સંરચના option *opt,
						અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->કિંમત, arg, &sub_marks_from);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_rewrite_submodules_to(અચલ સંરચના option *opt,
					      અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->કિંમત, arg, &sub_marks_to);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_cat_blob_fd(અચલ સંરચના option *opt,
				    અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->કિંમત, arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_import_marks(અચલ સંરચના option *opt,
				     અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->કિંમત, arg, 0, 0);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_import_marks_if_exists(અચલ સંરચના option *opt,
					       અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->કિંમત, arg, 0, 1);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_export_marks(અચલ સંરચના option *opt,
				     અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->કિંમત, arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_depth(અચલ સંરચના option *opt UNUSED,
			      અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_active_branches(અચલ સંરચના option *opt UNUSED,
					અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_quiet(અચલ સંરચના option *opt UNUSED,
			      અચલ અક્ષર *arg UNUSED, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	પાછા 0;
}

પૂર્ણાંક cmd_fast_import(પૂર્ણાંક argc,
		    અચલ અક્ષર **argv,
		    અચલ અક્ષર *prefix,
		    સંરચના repository *repo)
{
	સંરચના fast_import_state state;

	સંરચના option fast_import_options[] = {
		OPT_GROUP(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", ખાલી, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", ખાલી, ખાલી,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", ખાલી, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_size),
		OPT_CALLBACK_F(0, "big-file-threshold", ખાલી, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_threshold),
		OPT_CALLBACK_F(0, "depth", ખાલી, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_depth),
		OPT_CALLBACK_F(0, "active-branches", ખાલી, N_("n"),
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
		OPT_CALLBACK_F(0, "signed-commits", ખાલી, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", ખાલી, N_("mode"),
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
	CALLOC_ARRAY(avail_વૃક્ષ_table, avail_વૃક્ષ_table_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, કદ(સંરચના mark_set));

	hashmap_init(&object_table, object_entry_hashcmp, ખાલી, 0);

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
	માટે (પૂર્ણાંક i = 1; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];
		જો (*arg != '-' || !strcmp(arg, "--"))
			તોડો;
		જો (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * કદ(*rc_free));
	માટે (unsigned પૂર્ણાંક i = 0; i < (cmd_save - 1); i++)
		rc_free[i].next = &rc_free[i + 1];
	rc_free[cmd_save - 1].next = ખાલી;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	જ્યારે (read_next_command(&state) != EOF) {
		અચલ અક્ષર *v;
		જો (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		નહીંતર જો (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		નહીંતર જો (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		નહીંતર જો (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		નહીંતર જો (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, ખાલી);
		નહીંતર જો (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		નહીંતર જો (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		નહીંતર જો (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		નહીંતર જો (!strcmp("done", command_buf.buf))
			તોડો;
		નહીંતર જો (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		નહીંતર જો (starts_with(command_buf.buf, "progress "))
			parse_progress(&state);
		નહીંતર જો (skip_prefix(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		નહીંતર જો (skip_prefix(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		નહીંતર જો (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		નહીંતર
			die(_("unsupported command: %s"), command_buf.buf);

		જો (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	જો (!state.seen_data_command)
		parse_argv(&state);

	જો (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	end_packfile();

	dump_branches();
	dump_tags();
	unkeep_all_packs();
	dump_marks();

	જો (pack_edges)
		fclose(pack_edges);

	જો (show_stats) {
		uintmax_t total_ગણતરી = 0, duplicate_ગણતરી = 0;
		માટે (size_t i = 0; i < ARRAY_SIZE(object_ગણતરી_by_type); i++)
			total_ગણતરી += object_ગણતરી_by_type[i];
		માટે (size_t i = 0; i < ARRAY_SIZE(duplicate_ગણતરી_by_type); i++)
			duplicate_ગણતરી += duplicate_ગણતરી_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_ગણતરી);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", total_ગણતરી, duplicate_ગણતરી);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ગણતરી_by_type[OBJ_BLOB], duplicate_ગણતરી_by_type[OBJ_BLOB], delta_ગણતરી_by_type[OBJ_BLOB], delta_ગણતરી_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ગણતરી_by_type[OBJ_વૃક્ષ], duplicate_ગણતરી_by_type[OBJ_વૃક્ષ], delta_ગણતરી_by_type[OBJ_વૃક્ષ], delta_ગણતરી_attempts_by_type[OBJ_વૃક્ષ]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ગણતરી_by_type[OBJ_COMMIT], duplicate_ગણતરી_by_type[OBJ_COMMIT], delta_ગણતરી_by_type[OBJ_COMMIT], delta_ગણતરી_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ગણતરી_by_type[OBJ_TAG], duplicate_ગણતરી_by_type[OBJ_TAG], delta_ગણતરી_by_type[OBJ_TAG], delta_ગણતરી_attempts_by_type[OBJ_TAG]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_ગણતરી, branch_load_ગણતરી);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->shift) * 1024, marks_set_ગણતરી);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (વૃક્ષ_entry_allocd + fi_mem_pool.pool_alloc + alloc_ગણતરી*કદ(સંરચના object_entry))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (unsigned long)((વૃક્ષ_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_ગણતરી*કદ(સંરચના object_entry))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	પાછા failure ? 1 : 0;
}
