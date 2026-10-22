#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "blob.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "delta.h"
#ಅನ್ತರ್ಭಾವ "pack.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "csum-file.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "mem-pool.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "khash.h"
#ಅನ್ತರ್ಭಾವ "date.h"
#ಅನ್ತರ್ಭಾವ "gpg-interface.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"

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

ಸಂರಚನಾ object_entry {
	ಸಂರಚನಾ pack_idx_entry idx;
	ಸಂರಚನಾ hashmap_entry ent;
	uint32_t type : TYPE_BITS,
		pack_id : PACK_ID_BITS,
		depth : DEPTH_BITS;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ object_entry_hashcmp(ಧ್ರುವ ಶೂನ್ಯ *map_data UNUSED,
				ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *eptr,
				ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *entry_or_ಕೀಲಿ,
				ಧ್ರುವ ಶೂನ್ಯ *keydata)
{
	ಧ್ರುವ ಸಂರಚನಾ object_id *oid = keydata;
	ಧ್ರುವ ಸಂರಚನಾ object_entry *e1, *e2;

	e1 = container_of(eptr, ಧ್ರುವ ಸಂರಚನಾ object_entry, ent);
	ಯದಿ (oid)
		ಪ್ರತ್ಯಾಗಮ oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_ಕೀಲಿ, ಧ್ರುವ ಸಂರಚನಾ object_entry, ent);
	ಪ್ರತ್ಯಾಗಮ oidcmp(&e1->idx.oid, &e2->idx.oid);
}

ಸಂರಚನಾ object_entry_pool {
	ಸಂರಚನಾ object_entry_pool *next_pool;
	ಸಂರಚನಾ object_entry *next_free;
	ಸಂರಚನಾ object_entry *end;
	ಸಂರಚನಾ object_entry entries[FLEX_ARRAY]; /* more */
};

ಸಂರಚನಾ mark_set {
	ಸಙ್ಘ {
		ಸಂರಚನಾ object_id *oids[1024];
		ಸಂರಚನಾ object_entry *marked[1024];
		ಸಂರಚನಾ mark_set *sets[1024];
	} data;
	unsigned ಪೂರ್ಣಾಙ್ಕ shift;
};

ಸಂರಚನಾ last_object {
	ಸಂರಚನಾ strbuf data;
	off_t offset;
	unsigned ಪೂರ್ಣಾಙ್ಕ depth;
	unsigned no_swap : 1;
};

ಸಂರಚನಾ atom_str {
	ಸಂರಚನಾ atom_str *next_atom;
	unsigned short str_len;
	ವರ್ಣ str_dat[FLEX_ARRAY]; /* more */
};

ಸಂರಚನಾ ವೃಕ್ಷ_content;
ಸಂರಚನಾ ವೃಕ್ಷ_entry {
	ಸಂರಚನಾ ವೃಕ್ಷ_content *ವೃಕ್ಷ;
	ಸಂರಚನಾ atom_str *name;
	ಸಂರಚನಾ ವೃಕ್ಷ_entry_ms {
		uint16_t mode;
		ಸಂರಚನಾ object_id oid;
	} versions[2];
};

ಸಂರಚನಾ ವೃಕ್ಷ_content {
	unsigned ಪೂರ್ಣಾಙ್ಕ entry_capacity; /* must match avail_tree_content */
	unsigned ಪೂರ್ಣಾಙ್ಕ entry_ಎಣಿಕೆ;
	unsigned ಪೂರ್ಣಾಙ್ಕ delta_depth;
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *entries[FLEX_ARRAY]; /* more */
};

ಸಂರಚನಾ avail_ವೃಕ್ಷ_content {
	unsigned ಪೂರ್ಣಾಙ್ಕ entry_capacity; /* must match tree_content */
	ಸಂರಚನಾ avail_ವೃಕ್ಷ_content *next_avail;
};

ಸಂರಚನಾ branch {
	ಸಂರಚನಾ branch *table_next_branch;
	ಸಂರಚನಾ branch *active_next_branch;
	ಧ್ರುವ ವರ್ಣ *name;
	ಸಂರಚನಾ ವೃಕ್ಷ_entry branch_ವೃಕ್ಷ;
	uintmax_t last_commit;
	uintmax_t ಅಂಕೆ_notes;
	unsigned active : 1;
	unsigned delete : 1;
	unsigned pack_id : PACK_ID_BITS;
	ಸಂರಚನಾ object_id oid;
};

ಸಂರಚನಾ tag {
	ಸಂರಚನಾ tag *next_tag;
	ಧ್ರುವ ವರ್ಣ *name;
	unsigned ಪೂರ್ಣಾಙ್ಕ pack_id;
	ಸಂರಚನಾ object_id oid;
};

ಸಂರಚನಾ hash_list {
	ಸಂರಚನಾ hash_list *next;
	ಸಂರಚನಾ object_id oid;
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

ಸಂರಚನಾ recent_command {
	ಸಂರಚನಾ recent_command *prev;
	ಸಂರಚನಾ recent_command *next;
	ವರ್ಣ *buf;
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*mark_set_inserter_t)(ಸಂರಚನಾ mark_set **s, ಸಂರಚನಾ object_id *oid, uintmax_t mark);
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*each_mark_fn_t)(uintmax_t mark, ಶೂನ್ಯ *obj, ಶೂನ್ಯ *cbp);

/* Configured limits on output */
ಸ್ಥಿರ unsigned long max_depth = 50;
ಸ್ಥಿರ off_t max_packsize;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ unpack_limit = 100;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ force_update;

/* Stats and misc. counters */
ಸ್ಥಿರ uintmax_t alloc_ಎಣಿಕೆ;
ಸ್ಥಿರ uintmax_t marks_set_ಎಣಿಕೆ;
ಸ್ಥಿರ uintmax_t object_ಎಣಿಕೆ_by_type[1 << TYPE_BITS];
ಸ್ಥಿರ uintmax_t duplicate_ಎಣಿಕೆ_by_type[1 << TYPE_BITS];
ಸ್ಥಿರ uintmax_t delta_ಎಣಿಕೆ_by_type[1 << TYPE_BITS];
ಸ್ಥಿರ uintmax_t delta_ಎಣಿಕೆ_attempts_by_type[1 << TYPE_BITS];
ಸ್ಥಿರ unsigned long object_ಎಣಿಕೆ;
ಸ್ಥಿರ unsigned long branch_ಎಣಿಕೆ;
ಸ್ಥಿರ unsigned long branch_load_ಎಣಿಕೆ;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ failure;
ಸ್ಥಿರ FILE *pack_edges;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_stats = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ quiet;
ಸ್ಥಿರ ಗಣನಾ sign_mode signed_tag_mode = SIGN_VERBATIM;
ಸ್ಥಿರ ಗಣನಾ sign_mode signed_commit_mode = SIGN_VERBATIM;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *signed_commit_keyid;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *signed_tag_keyid;

/* Memory pools */
ಸ್ಥಿರ ಸಂರಚನಾ mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - ಪರಿಮಾಣ(ಸಂರಚನಾ mp_block),
};

/* Atom management */
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ atom_table_sz = 4451;
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ atom_cnt;
ಸ್ಥಿರ ಸಂರಚನಾ atom_str **atom_table;

/* The .pack file being generated */
ಸ್ಥಿರ ಸಂರಚನಾ pack_idx_option pack_idx_opts;
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ pack_id;
ಸ್ಥಿರ ಸಂರಚನಾ hashfile *pack_file;
ಸ್ಥಿರ ಸಂರಚನಾ packed_git *pack_data;
ಸ್ಥಿರ ಸಂರಚನಾ packed_git **all_packs;
ಸ್ಥಿರ off_t pack_size;

/* Table of objects we've written. */
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ object_entry_alloc = 5000;
ಸ್ಥಿರ ಸಂರಚನಾ object_entry_pool *blocks;
ಸ್ಥಿರ ಸಂರಚನಾ hashmap object_table;
ಸ್ಥಿರ ಸಂರಚನಾ mark_set *marks;
ಸ್ಥಿರ ವರ್ಣ *export_marks_file;
ಸ್ಥಿರ ವರ್ಣ *import_marks_file;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ import_marks_file_from_stream;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ import_marks_file_ignore_missing;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ import_marks_file_done;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ relative_marks_paths;

/* Our last blob */
ಸ್ಥಿರ ಸಂರಚನಾ last_object last_blob = {
	.data = STRBUF_INIT,
 };

/* Tree management */
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ ವೃಕ್ಷ_entry_alloc = 1000;
ಸ್ಥಿರ ಶೂನ್ಯ *avail_ವೃಕ್ಷ_entry;
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ avail_ವೃಕ್ಷ_table_sz = 100;
ಸ್ಥಿರ ಸಂರಚನಾ avail_ವೃಕ್ಷ_content **avail_ವೃಕ್ಷ_table;
ಸ್ಥಿರ size_t ವೃಕ್ಷ_entry_allocd;
ಸ್ಥಿರ ಸಂರಚನಾ strbuf old_ವೃಕ್ಷ = STRBUF_INIT;
ಸ್ಥಿರ ಸಂರಚನಾ strbuf new_ವೃಕ್ಷ = STRBUF_INIT;

/* Branch data */
ಸ್ಥಿರ unsigned long max_active_branches = 5;
ಸ್ಥಿರ unsigned long cur_active_branches;
ಸ್ಥಿರ unsigned long branch_table_sz = 1039;
ಸ್ಥಿರ ಸಂರಚನಾ branch **branch_table;
ಸ್ಥಿರ ಸಂರಚನಾ branch *active_branches;

/* Tag data */
ಸ್ಥಿರ ಸಂರಚನಾ tag *first_tag;
ಸ್ಥಿರ ಸಂರಚನಾ tag *last_tag;

/* Input stream parsing */
ಸ್ಥಿರ whenspec_type whenspec = WHENSPEC_RAW;
ಸ್ಥಿರ ಸಂರಚನಾ strbuf command_buf = STRBUF_INIT;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ unread_command_buf;
ಸ್ಥಿರ ಸಂರಚನಾ recent_command cmd_hist = {
	.prev = &cmd_hist,
	.next = &cmd_hist,
};
ಸ್ಥಿರ ಸಂರಚನಾ recent_command *cmd_tail = &cmd_hist;
ಸ್ಥಿರ ಸಂರಚನಾ recent_command *rc_free;
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ cmd_save = 100;
ಸ್ಥಿರ uintmax_t next_mark;
ಸ್ಥಿರ ಸಂರಚನಾ strbuf new_data = STRBUF_INIT;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ require_explicit_termination;

/* Signal handling */
ಸ್ಥಿರ ಚಞ್ಚಲ sig_atomic_t checkpoint_requested;

/* Submodule marks */
ಸ್ಥಿರ ಸಂರಚನಾ string_list sub_marks_from = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ ಸಂರಚನಾ string_list sub_marks_to = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cat_blob_fd = STDOUT_FILENO;

/* Command state */
ಸಂರಚನಾ fast_import_state {
	ಪೂರ್ಣಾಙ್ಕ argc;
	ಧ್ರುವ ವರ್ಣ **argv;
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಪೂರ್ಣಾಙ್ಕ seen_data_command;
	ಪೂರ್ಣಾಙ್ಕ allow_unsafe_features;
	ಸಂರಚನಾ option *option;
};

ಸ್ಥಿರ ಶೂನ್ಯ fast_import_state_init(ಸಂರಚನಾ fast_import_state *state,
				   ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
				   ಧ್ರುವ ವರ್ಣ *prefix, ಸಂರಚನಾ option *option)
{
	memset(state, 0, ಪರಿಮಾಣ(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_argv(ಸಂರಚನಾ fast_import_state *state);
ಸ್ಥಿರ ಶೂನ್ಯ parse_get_mark(ಸಂರಚನಾ fast_import_state *state, ಧ್ರುವ ವರ್ಣ *p);
ಸ್ಥಿರ ಶೂನ್ಯ parse_cat_blob(ಸಂರಚನಾ fast_import_state *state, ಧ್ರುವ ವರ್ಣ *p);
ಸ್ಥಿರ ಶೂನ್ಯ parse_ls(ಸಂರಚನಾ fast_import_state *state,
		     ಧ್ರುವ ವರ್ಣ *p,
		     ಸಂರಚನಾ branch *b);

ಸ್ಥಿರ ಶೂನ್ಯ for_each_mark(ಸಂರಚನಾ mark_set *m, uintmax_t base, each_mark_fn_t callback, ಶೂನ್ಯ *p)
{
	uintmax_t k;
	ಯದಿ (m->shift) {
		ಪ್ರತಿ (k = 0; k < 1024; k++) {
			ಯದಿ (m->data.sets[k])
				for_each_mark(m->data.sets[k], base + (k << m->shift), callback, p);
		}
	} ಅನ್ಯಥಾ {
		ಪ್ರತಿ (k = 0; k < 1024; k++) {
			ಯದಿ (m->data.marked[k])
				callback(base + k, m->data.marked[k], p);
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_marks_fn(uintmax_t mark, ಶೂನ್ಯ *object, ಶೂನ್ಯ *cbp) {
	ಸಂರಚನಾ object_entry *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

ಸ್ಥಿರ ಶೂನ್ಯ write_branch_report(FILE *rpt, ಸಂರಚನಾ branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	ಯದಿ (b->active)
		fputs(" active", rpt);
	ಯದಿ (b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ)
		fputs(" loaded", rpt);
	ಯದಿ (is_null_oid(&b->branch_ವೃಕ್ಷ.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_to_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_to_hex(&b->branch_ವೃಕ್ಷ.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_to_hex(&b->branch_ವೃಕ್ಷ.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->last_commit);

	fputs("  last pack   : ", rpt);
	ಯದಿ (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_crash_report(ಧ್ರುವ ವರ್ಣ *err)
{
	ವರ್ಣ *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	ಸಂರಚನಾ branch *b;
	unsigned long lu;
	ಸಂರಚನಾ recent_command *rc;

	ಯದಿ (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		ಮುಕ್ತ(loc);
		ಪ್ರತ್ಯಾಗಮ;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(time(ರಿಕ್ತ), 0, DATE_MODE(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	ಪ್ರತಿ (rc = cmd_hist.next; rc != &cmd_hist; rc = rc->next) {
		ಯದಿ (rc->next == &cmd_hist)
			fputs("* ", rpt);
		ಅನ್ಯಥಾ
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
	ಪ್ರತಿ (b = active_branches, lu = 0; b; b = b->active_next_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	ಪ್ರತಿ (lu = 0; lu < branch_table_sz; lu++) {
		ಪ್ರತಿ (b = branch_table[lu]; b; b = b->table_next_branch)
			write_branch_report(rpt, b);
	}

	ಯದಿ (first_tag) {
		ಸಂರಚನಾ tag *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		ಪ್ರತಿ (tg = first_tag; tg; tg = tg->next_tag) {
			fputs(oid_to_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	ಯದಿ (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	ಅನ್ಯಥಾ
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	ಮುಕ್ತ(loc);
}

ಸ್ಥಿರ ಶೂನ್ಯ end_packfile(ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ unkeep_all_packs(ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ dump_marks(ಶೂನ್ಯ);

ಸ್ಥಿರ NORETURN ಶೂನ್ಯ die_nicely(ಧ್ರುವ ವರ್ಣ *err, va_list params)
{
	va_list cp;
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	ಯದಿ (!zombie) {
		ವರ್ಣ message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, ಪರಿಮಾಣ(message), err, cp);
		write_crash_report(message);
		end_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	exit(128);
}

#ifndef SIGUSR1	/* Windows, for example */

ಸ್ಥಿರ ಶೂನ್ಯ set_checkpoint_signal(ಶೂನ್ಯ)
{
}

#ಅನ್ಯಥಾ

ಸ್ಥಿರ ಶೂನ್ಯ checkpoint_signal(ಪೂರ್ಣಾಙ್ಕ signo UNUSED)
{
	checkpoint_requested = 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ set_checkpoint_signal(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ sigaction sa;

	memset(&sa, 0, ಪರಿಮಾಣ(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, ರಿಕ್ತ);
}

#endif

ಸ್ಥಿರ ಶೂನ್ಯ alloc_objects(unsigned ಪೂರ್ಣಾಙ್ಕ cnt)
{
	ಸಂರಚನಾ object_entry_pool *b;

	b = xmalloc(ಪರಿಮಾಣ(ಸಂರಚನಾ object_entry_pool)
		+ cnt * ಪರಿಮಾಣ(ಸಂರಚನಾ object_entry));
	b->next_pool = blocks;
	b->next_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_ಎಣಿಕೆ += cnt;
}

ಸ್ಥಿರ ಸಂರಚನಾ object_entry *new_object(ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ object_entry *e;

	ಯದಿ (blocks->next_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->next_free++;
	oidcpy(&e->idx.oid, oid);
	ಪ್ರತ್ಯಾಗಮ e;
}

ಸ್ಥಿರ ಸಂರಚನಾ object_entry *find_object(ಸಂರಚನಾ object_id *oid)
{
	ಪ್ರತ್ಯಾಗಮ hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   ಸಂರಚನಾ object_entry, ent);
}

ಸ್ಥಿರ ಸಂರಚನಾ object_entry *ಸೇರಿಸು_object(ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ object_entry *e;
	unsigned ಪೂರ್ಣಾಙ್ಕ hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					ಸಂರಚನಾ object_entry, ent);
	ಯದಿ (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	ಪ್ರತ್ಯಾಗಮ e;
}

ಸ್ಥಿರ ಶೂನ್ಯ invalidate_pack_id(unsigned ಪೂರ್ಣಾಙ್ಕ id)
{
	unsigned long lu;
	ಸಂರಚನಾ tag *t;
	ಸಂರಚನಾ hashmap_iter iter;
	ಸಂರಚನಾ object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		ಯದಿ (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	ಪ್ರತಿ (lu = 0; lu < branch_table_sz; lu++) {
		ಸಂರಚನಾ branch *b;

		ಪ್ರತಿ (b = branch_table[lu]; b; b = b->table_next_branch)
			ಯದಿ (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	ಪ್ರತಿ (t = first_tag; t; t = t->next_tag)
		ಯದಿ (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ hc_str(ಧ್ರುವ ವರ್ಣ *s, size_t len)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ r = 0;
	ಯಾವತ್ (len-- > 0)
		r = r * 31 + *s++;
	ಪ್ರತ್ಯಾಗಮ r;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_mark(ಸಂರಚನಾ mark_set **ತುದಿ, uintmax_t idnum, ಸಂರಚನಾ object_entry *oe)
{
	ಸಂರಚನಾ mark_set *s = *ತುದಿ;

	ಯಾವತ್ ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, ಪರಿಮಾಣ(ಸಂರಚನಾ mark_set));
		s->shift = (*ತುದಿ)->shift + 10;
		s->data.sets[0] = *ತುದಿ;
		*ತುದಿ = s;
	}
	ಯಾವತ್ (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		ಯದಿ (!s->data.sets[i]) {
			s->data.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, ಪರಿಮಾಣ(ಸಂರಚನಾ mark_set));
			s->data.sets[i]->shift = s->shift - 10;
		}
		s = s->data.sets[i];
	}
	ಯದಿ (!s->data.marked[idnum])
		marks_set_ಎಣಿಕೆ++;
	s->data.marked[idnum] = oe;
}

ಸ್ಥಿರ ಶೂನ್ಯ *find_mark(ಸಂರಚನಾ mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	ಸಂರಚನಾ object_entry *oe = ರಿಕ್ತ;
	ಯದಿ ((idnum >> s->shift) < 1024) {
		ಯಾವತ್ (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->data.sets[i];
		}
		ಯದಿ (s)
			oe = s->data.marked[idnum];
	}
	ಯದಿ (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	ಪ್ರತ್ಯಾಗಮ oe;
}

ಸ್ಥಿರ ಸಂರಚನಾ atom_str *to_atom(ಧ್ರುವ ವರ್ಣ *s, unsigned short len)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ hc = hc_str(s, len) % atom_table_sz;
	ಸಂರಚನಾ atom_str *c;

	ಪ್ರತಿ (c = atom_table[hc]; c; c = c->next_atom)
		ಯದಿ (c->str_len == len && !strncmp(s, c->str_dat, len))
			ಪ್ರತ್ಯಾಗಮ c;

	c = mem_pool_alloc(&fi_mem_pool, ಪರಿಮಾಣ(ಸಂರಚನಾ atom_str) + len + 1);
	c->str_len = len;
	memcpy(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->next_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	ಪ್ರತ್ಯಾಗಮ c;
}

ಸ್ಥಿರ ಸಂರಚನಾ branch *lookup_branch(ಧ್ರುವ ವರ್ಣ *name)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ hc = hc_str(name, strlen(name)) % branch_table_sz;
	ಸಂರಚನಾ branch *b;

	ಪ್ರತಿ (b = branch_table[hc]; b; b = b->table_next_branch)
		ಯದಿ (!strcmp(name, b->name))
			ಪ್ರತ್ಯಾಗಮ b;
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಸಂರಚನಾ branch *new_branch(ಧ್ರುವ ವರ್ಣ *name)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ hc = hc_str(name, strlen(name)) % branch_table_sz;
	ಸಂರಚನಾ branch *b = lookup_branch(name);

	ಯದಿ (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	ಯದಿ (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, ಪರಿಮಾಣ(ಸಂರಚನಾ branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_next_branch = branch_table[hc];
	b->branch_ವೃಕ್ಷ.versions[0].mode = S_IFDIR;
	b->branch_ವೃಕ್ಷ.versions[1].mode = S_IFDIR;
	b->ಅಂಕೆ_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_ಎಣಿಕೆ++;
	ಪ್ರತ್ಯಾಗಮ b;
}

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ hc_entries(unsigned ಪೂರ್ಣಾಙ್ಕ cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	ಪ್ರತ್ಯಾಗಮ cnt < avail_ವೃಕ್ಷ_table_sz ? cnt : avail_ವೃಕ್ಷ_table_sz - 1;
}

ಸ್ಥಿರ ಸಂರಚನಾ ವೃಕ್ಷ_content *new_ವೃಕ್ಷ_content(unsigned ಪೂರ್ಣಾಙ್ಕ cnt)
{
	ಸಂರಚನಾ avail_ವೃಕ್ಷ_content *f, *l = ರಿಕ್ತ;
	ಸಂರಚನಾ ವೃಕ್ಷ_content *t;
	unsigned ಪೂರ್ಣಾಙ್ಕ hc = hc_entries(cnt);

	ಪ್ರತಿ (f = avail_ವೃಕ್ಷ_table[hc]; f; l = f, f = f->next_avail)
		ಯದಿ (f->entry_capacity >= cnt)
			ಭಙ್ಗ;

	ಯದಿ (f) {
		ಯದಿ (l)
			l->next_avail = f->next_avail;
		ಅನ್ಯಥಾ
			avail_ವೃಕ್ಷ_table[hc] = f->next_avail;
	} ಅನ್ಯಥಾ {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, ಪರಿಮಾಣ(*t) + ಪರಿಮಾಣ(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (ಸಂರಚನಾ ವೃಕ್ಷ_content*)f;
	t->entry_ಎಣಿಕೆ = 0;
	t->delta_depth = 0;
	ಪ್ರತ್ಯಾಗಮ t;
}

ಸ್ಥಿರ ಶೂನ್ಯ release_ವೃಕ್ಷ_entry(ಸಂರಚನಾ ವೃಕ್ಷ_entry *e);
ಸ್ಥಿರ ಶೂನ್ಯ release_ವೃಕ್ಷ_content(ಸಂರಚನಾ ವೃಕ್ಷ_content *t)
{
	ಸಂರಚನಾ avail_ವೃಕ್ಷ_content *f = (ಸಂರಚನಾ avail_ವೃಕ್ಷ_content*)t;
	unsigned ಪೂರ್ಣಾಙ್ಕ hc = hc_entries(f->entry_capacity);
	f->next_avail = avail_ವೃಕ್ಷ_table[hc];
	avail_ವೃಕ್ಷ_table[hc] = f;
}

ಸ್ಥಿರ ಶೂನ್ಯ release_ವೃಕ್ಷ_content_recursive(ಸಂರಚನಾ ವೃಕ್ಷ_content *t)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ i;
	ಪ್ರತಿ (i = 0; i < t->entry_ಎಣಿಕೆ; i++)
		release_ವೃಕ್ಷ_entry(t->entries[i]);
	release_ವೃಕ್ಷ_content(t);
}

ಸ್ಥಿರ ಸಂರಚನಾ ವೃಕ್ಷ_content *grow_ವೃಕ್ಷ_content(
	ಸಂರಚನಾ ವೃಕ್ಷ_content *t,
	ಪೂರ್ಣಾಙ್ಕ amt)
{
	ಸಂರಚನಾ ವೃಕ್ಷ_content *r = new_ವೃಕ್ಷ_content(t->entry_ಎಣಿಕೆ + amt);
	r->entry_ಎಣಿಕೆ = t->entry_ಎಣಿಕೆ;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_ಎಣಿಕೆ);
	release_ವೃಕ್ಷ_content(t);
	ಪ್ರತ್ಯಾಗಮ r;
}

ಸ್ಥಿರ ಸಂರಚನಾ ವೃಕ್ಷ_entry *new_ವೃಕ್ಷ_entry(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *e;

	ಯದಿ (!avail_ವೃಕ್ಷ_entry) {
		unsigned ಪೂರ್ಣಾಙ್ಕ n = ವೃಕ್ಷ_entry_alloc;
		ವೃಕ್ಷ_entry_allocd += n * ಪರಿಮಾಣ(ಸಂರಚನಾ ವೃಕ್ಷ_entry);
		ALLOC_ARRAY(e, n);
		avail_ವೃಕ್ಷ_entry = e;
		ಯಾವತ್ (n-- > 1) {
			*((ಶೂನ್ಯ**)e) = e + 1;
			e++;
		}
		*((ಶೂನ್ಯ**)e) = ರಿಕ್ತ;
	}

	e = avail_ವೃಕ್ಷ_entry;
	avail_ವೃಕ್ಷ_entry = *((ಶೂನ್ಯ**)e);
	ಪ್ರತ್ಯಾಗಮ e;
}

ಸ್ಥಿರ ಶೂನ್ಯ release_ವೃಕ್ಷ_entry(ಸಂರಚನಾ ವೃಕ್ಷ_entry *e)
{
	ಯದಿ (e->ವೃಕ್ಷ)
		release_ವೃಕ್ಷ_content_recursive(e->ವೃಕ್ಷ);
	*((ಶೂನ್ಯ**)e) = avail_ವೃಕ್ಷ_entry;
	avail_ವೃಕ್ಷ_entry = e;
}

ಸ್ಥಿರ ಸಂರಚನಾ ವೃಕ್ಷ_content *dup_ವೃಕ್ಷ_content(ಸಂರಚನಾ ವೃಕ್ಷ_content *s)
{
	ಸಂರಚನಾ ವೃಕ್ಷ_content *d;
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *a, *b;
	unsigned ಪೂರ್ಣಾಙ್ಕ i;

	ಯದಿ (!s)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	d = new_ವೃಕ್ಷ_content(s->entry_ಎಣಿಕೆ);
	ಪ್ರತಿ (i = 0; i < s->entry_ಎಣಿಕೆ; i++) {
		a = s->entries[i];
		b = new_ವೃಕ್ಷ_entry();
		memcpy(b, a, ಪರಿಮಾಣ(*a));
		ಯದಿ (a->ವೃಕ್ಷ && is_null_oid(&b->versions[1].oid))
			b->ವೃಕ್ಷ = dup_ವೃಕ್ಷ_content(a->ವೃಕ್ಷ);
		ಅನ್ಯಥಾ
			b->ವೃಕ್ಷ = ರಿಕ್ತ;
		d->entries[i] = b;
	}
	d->entry_ಎಣಿಕೆ = s->entry_ಎಣಿಕೆ;
	d->delta_depth = s->delta_depth;

	ಪ್ರತ್ಯಾಗಮ d;
}

ಸ್ಥಿರ ಶೂನ್ಯ start_packfile(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ strbuf tmp_file = STRBUF_INIT;
	ಸಂರಚನಾ packed_git *p;
	ಪೂರ್ಣಾಙ್ಕ pack_fd;

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
	object_ಎಣಿಕೆ = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *create_index(ಶೂನ್ಯ)
{
	ಧ್ರುವ ವರ್ಣ *tmpfile;
	ಸಂರಚನಾ pack_idx_entry **idx, **c, **last;
	ಸಂರಚನಾ object_entry *e;
	ಸಂರಚನಾ object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_ಎಣಿಕೆ);
	c = idx;
	ಪ್ರತಿ (o = blocks; o; o = o->next_pool)
		ಪ್ರತಿ (e = o->next_free; e-- != o->entries;)
			ಯದಿ (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_ಎಣಿಕೆ;
	ಯದಿ (c != last)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, ರಿಕ್ತ, idx, object_ಎಣಿಕೆ,
				 &pack_idx_opts, pack_data->hash);
	ಮುಕ್ತ(idx);
	ಪ್ರತ್ಯಾಗಮ tmpfile;
}

ಸ್ಥಿರ ವರ್ಣ *keep_pack(ಧ್ರುವ ವರ್ಣ *curr_index_name)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *keep_ಸಂದೇಶ = "fast-import";
	ಸಂರಚನಾ strbuf name = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ keep_fd;

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_data->repo,
							    name.buf);
	ಯದಿ (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_ಸಂದೇಶ, strlen(keep_ಸಂದೇಶ));
	ಯದಿ (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "pack");
	ಯದಿ (finalize_object_file(pack_data->repo, pack_data->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "idx");
	ಯದಿ (finalize_object_file(pack_data->repo, curr_index_name, name.buf))
		die(_("cannot store index file"));
	ಮುಕ್ತ((ಶೂನ್ಯ *)curr_index_name);
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&name, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ unkeep_all_packs(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ strbuf name = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ k;

	ಪ್ರತಿ (k = 0; k < pack_id; k++) {
		ಸಂರಚನಾ packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ loosen_small_pack(ಧ್ರುವ ಸಂರಚನಾ packed_git *p)
{
	ಸಂರಚನಾ child_process unpack = CHILD_PROCESS_INIT;

	ಯದಿ (lseek(p->pack_fd, 0, SEEK_SET) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_ತಳ್ಳು(&unpack.args, "unpack-objects");
	ಯದಿ (!show_stats)
		strvec_ತಳ್ಳು(&unpack.args, "-q");

	ಪ್ರತ್ಯಾಗಮ run_command(&unpack);
}

ಸ್ಥಿರ ಶೂನ್ಯ end_packfile(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ running;

	ಯದಿ (running || !pack_data)
		ಪ್ರತ್ಯಾಗಮ;

	running = 1;
	clear_delta_base_cache();
	ಯದಿ (object_ಎಣಿಕೆ) {
		ಸಂರಚನಾ odb_source_files *files = odb_source_files_downcast(pack_data->repo->objects->source);
		ಸಂರಚನಾ packed_git *new_p;
		ಸಂರಚನಾ object_id cur_pack_oid;
		ವರ್ಣ *idx_name;
		ಪೂರ್ಣಾಙ್ಕ i;
		ಸಂರಚನಾ branch *b;
		ಸಂರಚನಾ tag *t;

		close_pack_windows(pack_data);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_data->pack_fd,
					 pack_data->hash, pack_data->pack_name,
					 object_ಎಣಿಕೆ, cur_pack_oid.hash,
					 pack_size);

		ಯದಿ (object_ಎಣಿಕೆ <= unpack_limit) {
			ಯದಿ (!loosen_small_pack(pack_data)) {
				invalidate_pack_id(pack_id);
				ಗಚ್ಛ discard_pack;
			}
		}

		close(pack_data->pack_fd);
		idx_name = keep_pack(create_index());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		ಯದಿ (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		ಮುಕ್ತ(idx_name);

		/* Print the boundary */
		ಯದಿ (pack_edges) {
			fprintf(pack_edges, "%s:", new_p->pack_name);
			ಪ್ರತಿ (i = 0; i < branch_table_sz; i++) {
				ಪ್ರತಿ (b = branch_table[i]; b; b = b->table_next_branch) {
					ಯದಿ (b->pack_id == pack_id)
						fprintf(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			ಪ್ರತಿ (t = first_tag; t; t = t->next_tag) {
				ಯದಿ (t->pack_id == pack_id)
					fprintf(pack_edges, " %s",
						oid_to_hex(&t->oid));
			}
			fputc('\n', pack_edges);
			fflush(pack_edges);
		}

		pack_id++;
	}
	ಅನ್ಯಥಾ {
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

ಸ್ಥಿರ ಶೂನ್ಯ cycle_packfile(ಶೂನ್ಯ)
{
	end_packfile();
	start_packfile();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ store_object(
	ಗಣನಾ object_type type,
	ಸಂರಚನಾ strbuf *dat,
	ಸಂರಚನಾ last_object *last,
	ಸಂರಚನಾ object_id *oidout,
	uintmax_t mark)
{
	ಸಂರಚನಾ odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	ಶೂನ್ಯ *out, *delta;
	ಸಂರಚನಾ object_entry *e;
	unsigned ವರ್ಣ hdr[96];
	ಸಂರಚನಾ object_id oid;
	unsigned long hdrlen, deltalen = 0;
	ಸಂರಚನಾ git_hash_ctx c;
	git_zstream s;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	hdrlen = format_object_header((ವರ್ಣ *)hdr, ಪರಿಮಾಣ(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	ಯದಿ (oidout)
		oidcpy(oidout, &oid);

	e = ಸೇರಿಸು_object(&oid);
	ಯದಿ (mark)
		ಸೇರಿಸು_mark(&marks, mark, e);
	ಯದಿ (e->idx.offset) {
		duplicate_ಎಣಿಕೆ_by_type[type]++;
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಪ್ರತಿ (ಸಂರಚನಾ odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ಯದಿ (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			ಅನುವರ್ತ;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_ಎಣಿಕೆ_by_type[type]++;
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಯದಿ (last && last->data.len && last->data.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		size_t deltalen_st;

		delta_ಎಣಿಕೆ_attempts_by_type[type]++;
		delta = diff_delta(last->data.buf, last->data.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_size_t_to_ulong(deltalen_st);
	} ಅನ್ಯಥಾ
		delta = ರಿಕ್ತ;

	git_deflate_init(&s, cfg->pack_compression_level);
	ಯದಿ (delta) {
		s.next_in = delta;
		s.avail_in = deltalen;
	} ಅನ್ಯಥಾ {
		s.next_in = (ಶೂನ್ಯ *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.next_out = out = xmalloc(s.avail_out);
	ಯಾವತ್ (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	ಯದಿ ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + s.total_out) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + s.total_out) < pack_size) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		ಯದಿ (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.next_in = (ಶೂನ್ಯ *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.next_out = out = xrealloc(out, s.avail_out);
			ಯಾವತ್ (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_end(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.offset = pack_size;
	object_ಎಣಿಕೆ++;
	object_ಎಣಿಕೆ_by_type[type]++;

	crc32_begin(pack_file);

	ಯದಿ (delta) {
		off_t ofs = e->idx.offset - last->offset;
		unsigned pos = ಪರಿಮಾಣ(hdr) - 1;

		delta_ಎಣಿಕೆ_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, ಪರಿಮಾಣ(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;

		hdr[pos] = ofs & 127;
		ಯಾವತ್ (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, ಪರಿಮಾಣ(hdr) - pos);
		pack_size += ಪರಿಮಾಣ(hdr) - pos;
	} ಅನ್ಯಥಾ {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, ಪರಿಮಾಣ(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;
	}

	hashwrite(pack_file, out, s.total_out);
	pack_size += s.total_out;

	e->idx.crc32 = crc32_end(pack_file);

	ಮುಕ್ತ(out);
	ಮುಕ್ತ(delta);
	ಯದಿ (last) {
		ಯದಿ (last->no_swap) {
			last->data = *dat;
		} ಅನ್ಯಥಾ {
			strbuf_swap(&last->data, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ truncate_pack(ಸಂರಚನಾ hashfile_checkpoint *checkpoint)
{
	ಯದಿ (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_size = checkpoint->offset;
}

ಸ್ಥಿರ ಶೂನ್ಯ stream_blob(uintmax_t len, ಸಂರಚನಾ object_id *oidout, uintmax_t mark)
{
	ಸಂರಚನಾ odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	size_t in_sz = 64 * 1024, out_sz = 64 * 1024;
	unsigned ವರ್ಣ *in_buf = xmalloc(in_sz);
	unsigned ವರ್ಣ *out_buf = xmalloc(out_sz);
	ಸಂರಚನಾ object_entry *e;
	ಸಂರಚನಾ object_id oid;
	unsigned long hdrlen;
	off_t offset;
	ಸಂರಚನಾ git_hash_ctx c;
	git_zstream s;
	ಸಂರಚನಾ hashfile_checkpoint checkpoint;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ಪೂರ್ಣಾಙ್ಕ status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	ಯದಿ ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + len) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + len) < pack_size)
		cycle_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	offset = checkpoint.offset;

	hdrlen = format_object_header((ವರ್ಣ *)out_buf, out_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, out_buf, hdrlen);

	crc32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_in_pack_object_header(out_buf, out_sz, OBJ_BLOB, len);

	s.next_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	ಯಾವತ್ (status != Z_STREAM_END) {
		ಯದಿ (0 < len && !s.avail_in) {
			size_t cnt = in_sz < len ? in_sz : (size_t)len;
			size_t n = fread(in_buf, 1, cnt, stdin);
			ಯದಿ (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.next_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		ಯದಿ (!s.avail_out || status == Z_STREAM_END) {
			size_t n = s.next_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_size += n;
			s.next_out = out_buf;
			s.avail_out = out_sz;
		}

		ವಿಕಲ್ಪ (status) {
		ಪಕ್ಷ Z_OK:
		ಪಕ್ಷ Z_BUF_ERROR:
		ಪಕ್ಷ Z_STREAM_END:
			ಅನುವರ್ತ;
		ಸ್ವಭಾವ:
			die(_("unexpected deflate failure: %d"), status);
		}
	}
	git_deflate_end(&s);
	git_hash_final_oid(&oid, &c);

	ಯದಿ (oidout)
		oidcpy(oidout, &oid);

	e = ಸೇರಿಸು_object(&oid);

	ಯದಿ (mark)
		ಸೇರಿಸು_mark(&marks, mark, e);

	ಯದಿ (e->idx.offset) {
		duplicate_ಎಣಿಕೆ_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		ಗಚ್ಛ out;
	}

	ಪ್ರತಿ (ಸಂರಚನಾ odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ಯದಿ (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			ಅನುವರ್ತ;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_ಎಣಿಕೆ_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		ಗಚ್ಛ out;
	}

	e->depth = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.offset = offset;
	e->idx.crc32 = crc32_end(pack_file);
	object_ಎಣಿಕೆ++;
	object_ಎಣಿಕೆ_by_type[OBJ_BLOB]++;

out:
	ಮುಕ್ತ(in_buf);
	ಮುಕ್ತ(out_buf);
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
ಸ್ಥಿರ ಶೂನ್ಯ *gfi_unpack_entry(
	ಸಂರಚನಾ object_entry *oe,
	unsigned long *sizep)
{
	ಗಣನಾ object_type type;
	size_t size_st = 0;
	ಶೂನ್ಯ *data;
	ಸಂರಚನಾ packed_git *p = all_packs[oe->pack_id];
	ಯದಿ (p == pack_data && p->pack_size < (pack_size + the_hash_algo->rawsz)) {
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
	ಯದಿ (sizep)
		*sizep = cast_size_t_to_ulong(size_st);
	ಪ್ರತ್ಯಾಗಮ data;
}

ಸ್ಥಿರ ಶೂನ್ಯ load_ವೃಕ್ಷ(ಸಂರಚನಾ ವೃಕ್ಷ_entry *ಬೇರು)
{
	ಸಂರಚನಾ object_id *oid = &ಬೇರು->versions[1].oid;
	ಸಂರಚನಾ object_entry *myoe;
	ಸಂರಚನಾ ವೃಕ್ಷ_content *t;
	unsigned long size;
	ವರ್ಣ *buf;
	ಧ್ರುವ ವರ್ಣ *c;

	ಬೇರು->ವೃಕ್ಷ = t = new_ವೃಕ್ಷ_content(8);
	ಯದಿ (is_null_oid(oid))
		ಪ್ರತ್ಯಾಗಮ;

	myoe = find_object(oid);
	ಯದಿ (myoe && myoe->pack_id != MAX_PACK_ID) {
		ಯದಿ (myoe->type != OBJ_ವೃಕ್ಷ)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &size);
		ಯದಿ (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} ಅನ್ಯಥಾ {
		ಗಣನಾ object_type type;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		ಯದಿ (!buf || type != OBJ_ವೃಕ್ಷ)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	ಯಾವತ್ (c != (buf + size)) {
		ಸಂರಚನಾ ವೃಕ್ಷ_entry *e = new_ವೃಕ್ಷ_entry();

		ಯದಿ (t->entry_ಎಣಿಕೆ == t->entry_capacity)
			ಬೇರು->ವೃಕ್ಷ = t = grow_ವೃಕ್ಷ_content(t, t->entry_ಎಣಿಕೆ);
		t->entries[t->entry_ಎಣಿಕೆ++] = e;

		e->ವೃಕ್ಷ = ರಿಕ್ತ;
		c = parse_mode(c, &e->versions[1].mode);
		ಯದಿ (!c)
			die(_("corrupt mode in %s"), oid_to_hex(oid));
		e->versions[0].mode = e->versions[1].mode;
		e->name = to_atom(c, strlen(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (unsigned ವರ್ಣ *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (unsigned ವರ್ಣ *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	ಮುಕ್ತ(buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tecmp0 (ಧ್ರುವ ಶೂನ್ಯ *_a, ಧ್ರುವ ಶೂನ್ಯ *_b)
{
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *a = *((ಸಂರಚನಾ ವೃಕ್ಷ_entry**)_a);
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *b = *((ಸಂರಚನಾ ವೃಕ್ಷ_entry**)_b);
	ಪ್ರತ್ಯಾಗಮ base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tecmp1 (ಧ್ರುವ ಶೂನ್ಯ *_a, ಧ್ರುವ ಶೂನ್ಯ *_b)
{
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *a = *((ಸಂರಚನಾ ವೃಕ್ಷ_entry**)_a);
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *b = *((ಸಂರಚನಾ ವೃಕ್ಷ_entry**)_b);
	ಪ್ರತ್ಯಾಗಮ base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

ಸ್ಥಿರ ಶೂನ್ಯ mktree(ಸಂರಚನಾ ವೃಕ್ಷ_content *t, ಪೂರ್ಣಾಙ್ಕ v, ಸಂರಚನಾ strbuf *b)
{
	size_t maxlen = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ i;

	ಯದಿ (!v)
		QSORT(t->entries, t->entry_ಎಣಿಕೆ, tecmp0);
	ಅನ್ಯಥಾ
		QSORT(t->entries, t->entry_ಎಣಿಕೆ, tecmp1);

	ಪ್ರತಿ (i = 0; i < t->entry_ಎಣಿಕೆ; i++) {
		ಯದಿ (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	ಪ್ರತಿ (i = 0; i < t->entry_ಎಣಿಕೆ; i++) {
		ಸಂರಚನಾ ವೃಕ್ಷ_entry *e = t->entries[i];
		ಯದಿ (!e->versions[v].mode)
			ಅನುವರ್ತ;
		strbuf_addf(b, "%o %s%c",
			(unsigned ಪೂರ್ಣಾಙ್ಕ)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ store_ವೃಕ್ಷ(ಸಂರಚನಾ ವೃಕ್ಷ_entry *ಬೇರು)
{
	ಸಂರಚನಾ ವೃಕ್ಷ_content *t;
	unsigned ಪೂರ್ಣಾಙ್ಕ i, j, del;
	ಸಂರಚನಾ last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	ಸಂರಚನಾ object_entry *le = ರಿಕ್ತ;

	ಯದಿ (!is_null_oid(&ಬೇರು->versions[1].oid))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (!ಬೇರು->ವೃಕ್ಷ)
		load_ವೃಕ್ಷ(ಬೇರು);
	t = ಬೇರು->ವೃಕ್ಷ;

	ಪ್ರತಿ (i = 0; i < t->entry_ಎಣಿಕೆ; i++) {
		ಯದಿ (t->entries[i]->ವೃಕ್ಷ)
			store_ವೃಕ್ಷ(t->entries[i]);
	}

	ಯದಿ (!(ಬೇರು->versions[0].mode & NO_DELTA))
		le = find_object(&ಬೇರು->versions[0].oid);
	ಯದಿ (S_ISDIR(ಬೇರು->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_ವೃಕ್ಷ);
		lo.data = old_ವೃಕ್ಷ;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_ವೃಕ್ಷ);
	store_object(OBJ_ವೃಕ್ಷ, &new_ವೃಕ್ಷ, &lo, &ಬೇರು->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	ಪ್ರತಿ (i = 0, j = 0, del = 0; i < t->entry_ಎಣಿಕೆ; i++) {
		ಸಂರಚನಾ ವೃಕ್ಷ_entry *e = t->entries[i];
		ಯದಿ (e->versions[1].mode) {
			e->versions[0].mode = e->versions[1].mode;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} ಅನ್ಯಥಾ {
			release_ವೃಕ್ಷ_entry(e);
			del++;
		}
	}
	t->entry_ಎಣಿಕೆ -= del;
}

ಸ್ಥಿರ ಶೂನ್ಯ ವೃಕ್ಷ_content_replace(
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *ಬೇರು,
	ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
	ಧ್ರುವ uint16_t mode,
	ಸಂರಚನಾ ವೃಕ್ಷ_content *newtree)
{
	ಯದಿ (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&ಬೇರು->versions[0].oid, the_repository->hash_algo);
	oidcpy(&ಬೇರು->versions[1].oid, oid);
	ಯದಿ (ಬೇರು->ವೃಕ್ಷ)
		release_ವೃಕ್ಷ_content_recursive(ಬೇರು->ವೃಕ್ಷ);
	ಬೇರು->ವೃಕ್ಷ = newtree;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ವೃಕ್ಷ_content_set(
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *ಬೇರು,
	ಧ್ರುವ ವರ್ಣ *p,
	ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
	ಧ್ರುವ uint16_t mode,
	ಸಂರಚನಾ ವೃಕ್ಷ_content *subtree)
{
	ಸಂರಚನಾ ವೃಕ್ಷ_content *t;
	ಧ್ರುವ ವರ್ಣ *slash1;
	unsigned ಪೂರ್ಣಾಙ್ಕ i, n;
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	ಯದಿ (!n)
		die(_("empty path component found in input"));
	ಯದಿ (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	ಯದಿ (!ಬೇರು->ವೃಕ್ಷ)
		load_ವೃಕ್ಷ(ಬೇರು);
	t = ಬೇರು->ವೃಕ್ಷ;
	ಪ್ರತಿ (i = 0; i < t->entry_ಎಣಿಕೆ; i++) {
		e = t->entries[i];
		ಯದಿ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ಯದಿ (!*slash1) {
				ಯದಿ (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					ಪ್ರತ್ಯಾಗಮ 0;
				e->versions[1].mode = mode;
				oidcpy(&e->versions[1].oid, oid);
				ಯದಿ (e->ವೃಕ್ಷ)
					release_ವೃಕ್ಷ_content_recursive(e->ವೃಕ್ಷ);
				e->ವೃಕ್ಷ = subtree;

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
				ಯದಿ (S_ISDIR(e->versions[0].mode))
					e->versions[0].mode |= NO_DELTA;

				oidclr(&ಬೇರು->versions[1].oid, the_repository->hash_algo);
				ಪ್ರತ್ಯಾಗಮ 1;
			}
			ಯದಿ (!S_ISDIR(e->versions[1].mode)) {
				e->ವೃಕ್ಷ = new_ವೃಕ್ಷ_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			ಯದಿ (!e->ವೃಕ್ಷ)
				load_ವೃಕ್ಷ(e);
			ಯದಿ (ವೃಕ್ಷ_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&ಬೇರು->versions[1].oid, the_repository->hash_algo);
				ಪ್ರತ್ಯಾಗಮ 1;
			}
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	ಯದಿ (t->entry_ಎಣಿಕೆ == t->entry_capacity)
		ಬೇರು->ವೃಕ್ಷ = t = grow_ವೃಕ್ಷ_content(t, t->entry_ಎಣಿಕೆ);
	e = new_ವೃಕ್ಷ_entry();
	e->name = to_atom(p, n);
	e->versions[0].mode = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->entry_ಎಣಿಕೆ++] = e;
	ಯದಿ (*slash1) {
		e->ವೃಕ್ಷ = new_ವೃಕ್ಷ_content(8);
		e->versions[1].mode = S_IFDIR;
		ವೃಕ್ಷ_content_set(e, slash1 + 1, oid, mode, subtree);
	} ಅನ್ಯಥಾ {
		e->ವೃಕ್ಷ = subtree;
		e->versions[1].mode = mode;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&ಬೇರು->versions[1].oid, the_repository->hash_algo);
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ವೃಕ್ಷ_content_remove(
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *ಬೇರು,
	ಧ್ರುವ ವರ್ಣ *p,
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *backup_leaf,
	ಪೂರ್ಣಾಙ್ಕ allow_ಬೇರು)
{
	ಸಂರಚನಾ ವೃಕ್ಷ_content *t;
	ಧ್ರುವ ವರ್ಣ *slash1;
	unsigned ಪೂರ್ಣಾಙ್ಕ i, n;
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	ಯದಿ (!ಬೇರು->ವೃಕ್ಷ)
		load_ವೃಕ್ಷ(ಬೇರು);

	ಯದಿ (!*p && allow_ಬೇರು) {
		e = ಬೇರು;
		ಗಚ್ಛ del_entry;
	}

	t = ಬೇರು->ವೃಕ್ಷ;
	ಪ್ರತಿ (i = 0; i < t->entry_ಎಣಿಕೆ; i++) {
		e = t->entries[i];
		ಯದಿ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ಯದಿ (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				ಪ್ರತ್ಯಾಗಮ 1;
			ಯದಿ (!*slash1 || !S_ISDIR(e->versions[1].mode))
				ಗಚ್ಛ del_entry;
			ಯದಿ (!e->ವೃಕ್ಷ)
				load_ವೃಕ್ಷ(e);
			ಯದಿ (ವೃಕ್ಷ_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				ಪ್ರತಿ (n = 0; n < e->ವೃಕ್ಷ->entry_ಎಣಿಕೆ; n++) {
					ಯದಿ (e->ವೃಕ್ಷ->entries[n]->versions[1].mode) {
						oidclr(&ಬೇರು->versions[1].oid,
						       the_repository->hash_algo);
						ಪ್ರತ್ಯಾಗಮ 1;
					}
				}
				backup_leaf = ರಿಕ್ತ;
				ಗಚ್ಛ del_entry;
			}
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;

del_entry:
	ಯದಿ (backup_leaf)
		memcpy(backup_leaf, e, ಪರಿಮಾಣ(*backup_leaf));
	ಅನ್ಯಥಾ ಯದಿ (e->ವೃಕ್ಷ)
		release_ವೃಕ್ಷ_content_recursive(e->ವೃಕ್ಷ);
	e->ವೃಕ್ಷ = ರಿಕ್ತ;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&ಬೇರು->versions[1].oid, the_repository->hash_algo);
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ವೃಕ್ಷ_content_get(
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *ಬೇರು,
	ಧ್ರುವ ವರ್ಣ *p,
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *leaf,
	ಪೂರ್ಣಾಙ್ಕ allow_ಬೇರು)
{
	ಸಂರಚನಾ ವೃಕ್ಷ_content *t;
	ಧ್ರುವ ವರ್ಣ *slash1;
	unsigned ಪೂರ್ಣಾಙ್ಕ i, n;
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	ಯದಿ (!n && !allow_ಬೇರು)
		die(_("empty path component found in input"));

	ಯದಿ (!ಬೇರು->ವೃಕ್ಷ)
		load_ವೃಕ್ಷ(ಬೇರು);

	ಯದಿ (!n) {
		e = ಬೇರು;
		ಗಚ್ಛ found_entry;
	}

	t = ಬೇರು->ವೃಕ್ಷ;
	ಪ್ರತಿ (i = 0; i < t->entry_ಎಣಿಕೆ; i++) {
		e = t->entries[i];
		ಯದಿ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ಯದಿ (!*slash1)
				ಗಚ್ಛ found_entry;
			ಯದಿ (!S_ISDIR(e->versions[1].mode))
				ಪ್ರತ್ಯಾಗಮ 0;
			ಯದಿ (!e->ವೃಕ್ಷ)
				load_ವೃಕ್ಷ(e);
			ಪ್ರತ್ಯಾಗಮ ವೃಕ್ಷ_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;

found_entry:
	memcpy(leaf, e, ಪರಿಮಾಣ(*leaf));
	ಯದಿ (e->ವೃಕ್ಷ && is_null_oid(&e->versions[1].oid))
		leaf->ವೃಕ್ಷ = dup_ವೃಕ್ಷ_content(e->ವೃಕ್ಷ);
	ಅನ್ಯಥಾ
		leaf->ವೃಕ್ಷ = ರಿಕ್ತ;
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_branch(ಸಂರಚನಾ branch *b)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ = "fast-import";
	ಸಂರಚನಾ ref_transaction *transaction;
	ಸಂರಚನಾ object_id old_oid;
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *replace_prefix = "refs/replace/";

	ಯದಿ (starts_with(b->name, replace_prefix) &&
	    !strcmp(b->name + strlen(replace_prefix),
		    oid_to_hex(&b->oid))) {
		ಯದಿ (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				ರಿಕ್ತ, b->name, ರಿಕ್ತ, 0);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (is_null_oid(&b->oid)) {
		ಯದಿ (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					ರಿಕ್ತ, b->name, ರಿಕ್ತ, 0);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	ಯದಿ (!force_update && !is_null_oid(&old_oid)) {
		ಸಂರಚನಾ commit *old_cmit, *new_cmit;
		ಪೂರ್ಣಾಙ್ಕ ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		ಯದಿ (!old_cmit || !new_cmit)
			ಪ್ರತ್ಯಾಗಮ error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		ಯದಿ (ret < 0)
			exit(128);
		ಯದಿ (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			ಪ್ರತ್ಯಾಗಮ -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ಯದಿ (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   ರಿಕ್ತ, ರಿಕ್ತ, 0, ಸಂದೇಶ, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_branches(ಶೂನ್ಯ)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ branch *b;

	ಪ್ರತಿ (i = 0; i < branch_table_sz; i++) {
		ಪ್ರತಿ (b = branch_table[i]; b; b = b->table_next_branch)
			failure |= update_branch(b);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_tags(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ = "fast-import";
	ಸಂರಚನಾ tag *t;
	ಸಂರಚನಾ strbuf ref_name = STRBUF_INIT;
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಸಂರಚನಾ ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ಯದಿ (!transaction) {
		failure |= error("%s", err.buf);
		ಗಚ್ಛ cleanup;
	}
	ಪ್ರತಿ (t = first_tag; t; t = t->next_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		ಯದಿ (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ,
					   0, ಸಂದೇಶ, &err)) {
			failure |= error("%s", err.buf);
			ಗಚ್ಛ cleanup;
		}
	}
	ಯದಿ (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_marks(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ lock_file mark_lock = LOCK_INIT;
	FILE *f;

	ಯದಿ (!export_marks_file || (import_marks_file && !import_marks_file_done))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		ಪ್ರತ್ಯಾಗಮ;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	ಯದಿ (!f) {
		ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		ಪ್ರತ್ಯಾಗಮ;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	ಯದಿ (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		ಪ್ರತ್ಯಾಗಮ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_object_entry(ಸಂರಚನಾ mark_set **s, ಸಂರಚನಾ object_id *oid, uintmax_t mark)
{
	ಸಂರಚನಾ object_entry *e;
	e = find_object(oid);
	ಯದಿ (!e) {
		ಗಣನಾ object_type type = odb_read_object_info(the_repository->objects,
							     oid, ರಿಕ್ತ);
		ಯದಿ (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = ಸೇರಿಸು_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	ಸೇರಿಸು_mark(s, mark, e);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_oid_entry(ಸಂರಚನಾ mark_set **s, ಸಂರಚನಾ object_id *oid, uintmax_t mark)
{
	ಸೇರಿಸು_mark(s, mark, xmemdupz(oid, ಪರಿಮಾಣ(*oid)));
}

ಸ್ಥಿರ ಶೂನ್ಯ read_mark_file(ಸಂರಚನಾ mark_set **s, FILE *f, mark_set_inserter_t inserter)
{
	ವರ್ಣ ಸಾಲು[512];
	ಯಾವತ್ (fgets(ಸಾಲು, ಪರಿಮಾಣ(ಸಾಲು), f)) {
		uintmax_t mark;
		ವರ್ಣ *end;
		ಸಂರಚನಾ object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, ಪರಿಮಾಣ(oid.hash));

		end = strchr(ಸಾಲು, '\n');
		ಯದಿ (ಸಾಲು[0] != ':' || !end)
			die(_("corrupt mark line: %s"), ಸಾಲು);
		*end = 0;
		mark = strtoumax(ಸಾಲು + 1, &end, 10);
		ಯದಿ (!mark || end == ಸಾಲು + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), ಸಾಲು);
		inserter(s, &oid, mark);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ read_marks(ಶೂನ್ಯ)
{
	FILE *f = fopen(import_marks_file, "r");
	ಯದಿ (f)
		;
	ಅನ್ಯಥಾ ಯದಿ (import_marks_file_ignore_missing && errno == ENOENT)
		ಗಚ್ಛ done; /* Marks file does not exist */
	ಅನ್ಯಥಾ
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, ಸೇರಿಸು_object_entry);
	fclose(f);
done:
	import_marks_file_done = 1;
}


ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ read_next_command(ಸಂರಚನಾ fast_import_state *state)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stdin_eof = 0;

	ಯದಿ (stdin_eof) {
		unread_command_buf = 0;
		ಪ್ರತ್ಯಾಗಮ EOF;
	}

	ಪ್ರತಿ (;;) {
		ಯದಿ (unread_command_buf) {
			unread_command_buf = 0;
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			ಯದಿ (stdin_eof)
				ಪ್ರತ್ಯಾಗಮ EOF;

			ಯದಿ (!state->seen_data_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			ಯದಿ (rc)
				rc_free = rc->next;
			ಅನ್ಯಥಾ {
				rc = cmd_hist.next;
				cmd_hist.next = rc->next;
				cmd_hist.next->prev = &cmd_hist;
				ಮುಕ್ತ(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_tail;
			rc->next = cmd_hist.prev;
			rc->prev->next = rc;
			cmd_tail = rc;
		}
		ಯದಿ (command_buf.buf[0] == '#')
			ಅನುವರ್ತ;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ skip_optional_lf(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ term_char = fgetc(stdin);
	ಯದಿ (term_char != '\n' && term_char != EOF)
		ungetc(term_char, stdin);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_mark(ಸಂರಚನಾ fast_import_state *state)
{
	ಧ್ರುವ ವರ್ಣ *v;
	ಯದಿ (skip_prefix(command_buf.buf, "mark :", &v)) {
		next_mark = strtoumax(v, ರಿಕ್ತ, 10);
		read_next_command(state);
	}
	ಅನ್ಯಥಾ
		next_mark = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_original_identifier(ಸಂರಚನಾ fast_import_state *state)
{
	ಧ್ರುವ ವರ್ಣ *v;
	ಯದಿ (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_next_command(state);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_data(ಸಂರಚನಾ strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	ಧ್ರುವ ವರ್ಣ *data;
	strbuf_reset(sb);

	ಯದಿ (!skip_prefix(command_buf.buf, "data ", &data))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	ಯದಿ (skip_prefix(data, "<<", &data)) {
		ವರ್ಣ *term = xstrdup(data);
		size_t term_len = command_buf.len - (data - command_buf.buf);

		ಪ್ರತಿ (;;) {
			ಯದಿ (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), term);
			ಯದಿ (term_len == command_buf.len
				&& !strcmp(term, command_buf.buf))
				ಭಙ್ಗ;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		ಮುಕ್ತ(term);
	}
	ಅನ್ಯಥಾ {
		uintmax_t len = strtoumax(data, ರಿಕ್ತ, 10);
		size_t n = 0, length = (size_t)len;

		ಯದಿ (limit && limit < len) {
			*len_res = len;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		ಯದಿ (length < len)
			die(_("data is too large to use in this context"));

		ಯಾವತ್ (n < length) {
			size_t s = strbuf_fread(sb, length - n, stdin);
			ಯದಿ (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(unsigned long)(length - n));
			n += s;
		}
	}

	skip_optional_lf();
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ validate_raw_date(ಧ್ರುವ ವರ್ಣ *src, ಸಂರಚನಾ strbuf *result, ಪೂರ್ಣಾಙ್ಕ strict)
{
	ಧ್ರುವ ವರ್ಣ *orig_src = src;
	ವರ್ಣ *endp;
	unsigned long ಅಂಕೆ;

	errno = 0;

	ಅಂಕೆ = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	ಯದಿ (errno || endp == src || *endp != ' ')
		ಪ್ರತ್ಯಾಗಮ -1;

	src = endp + 1;
	ಯದಿ (*src != '-' && *src != '+')
		ಪ್ರತ್ಯಾಗಮ -1;

	ಅಂಕೆ = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	ಯದಿ (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < ಅಂಕೆ))             /* parsed a broken timezone */
	   )
		ಪ್ರತ್ಯಾಗಮ -1;

	strbuf_addstr(result, orig_src);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ವರ್ಣ *parse_ident(ಧ್ರುವ ವರ್ಣ *buf)
{
	ಧ್ರುವ ವರ್ಣ *ltgt;
	size_t name_len;
	ಸಂರಚನಾ strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	ಯದಿ (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	ಯದಿ (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	ಯದಿ (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	ಯದಿ (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	ಯದಿ (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_add(&ident, buf, name_len);

	ವಿಕಲ್ಪ (whenspec) {
	ಪಕ್ಷ WHENSPEC_RAW:
		ಯದಿ (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		ಭಙ್ಗ;
	ಪಕ್ಷ WHENSPEC_RAW_PERMISSIVE:
		ಯದಿ (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		ಭಙ್ಗ;
	ಪಕ್ಷ WHENSPEC_RFC2822:
		ಯದಿ (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		ಭಙ್ಗ;
	ಪಕ್ಷ WHENSPEC_NOW:
		ಯದಿ (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		ಭಙ್ಗ;
	}

	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&ident, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_and_store_blob(
	ಸಂರಚನಾ last_object *last,
	ಸಂರಚನಾ object_id *oidout,
	uintmax_t mark)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	uintmax_t len;

	ಯದಿ (parse_data(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	ಅನ್ಯಥಾ {
		ಯದಿ (last) {
			strbuf_release(&last->data);
			last->offset = 0;
			last->depth = 0;
		}
		stream_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_new_blob(ಸಂರಚನಾ fast_import_state *state)
{
	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&last_blob, ರಿಕ್ತ, next_mark);
}

ಸ್ಥಿರ ಶೂನ್ಯ unload_one_branch(ಶೂನ್ಯ)
{
	ಯಾವತ್ (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = ULONG_MAX;
		ಸಂರಚನಾ branch *e, *l = ರಿಕ್ತ, *p = ರಿಕ್ತ;

		ಪ್ರತಿ (e = active_branches; e; e = e->active_next_branch) {
			ಯದಿ (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		ಯದಿ (p) {
			e = p->active_next_branch;
			p->active_next_branch = e->active_next_branch;
		} ಅನ್ಯಥಾ {
			e = active_branches;
			active_branches = e->active_next_branch;
		}
		e->active = 0;
		e->active_next_branch = ರಿಕ್ತ;
		ಯದಿ (e->branch_ವೃಕ್ಷ.ವೃಕ್ಷ) {
			release_ವೃಕ್ಷ_content_recursive(e->branch_ವೃಕ್ಷ.ವೃಕ್ಷ);
			e->branch_ವೃಕ್ಷ.ವೃಕ್ಷ = ರಿಕ್ತ;
		}
		cur_active_branches--;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ load_branch(ಸಂರಚನಾ branch *b)
{
	load_ವೃಕ್ಷ(&b->branch_ವೃಕ್ಷ);
	ಯದಿ (!b->active) {
		b->active = 1;
		b->active_next_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_ಎಣಿಕೆ++;
	}
}

ಸ್ಥಿರ unsigned ವರ್ಣ convert_ಅಂಕೆ_notes_to_fanout(uintmax_t ಅಂಕೆ_notes)
{
	unsigned ವರ್ಣ fanout = 0;
	ಯಾವತ್ ((ಅಂಕೆ_notes >>= 8))
		fanout++;
	ಪ್ರತ್ಯಾಗಮ fanout;
}

ಸ್ಥಿರ ಶೂನ್ಯ construct_path_with_fanout(ಧ್ರುವ ವರ್ಣ *hex_sha1,
		unsigned ವರ್ಣ fanout, ವರ್ಣ *path)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ i = 0, j = 0;
	ಯದಿ (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	ಯಾವತ್ (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	memcpy(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

ಸ್ಥಿರ uintmax_t do_change_note_fanout(
		ಸಂರಚನಾ ವೃಕ್ಷ_entry *orig_ಬೇರು, ಸಂರಚನಾ ವೃಕ್ಷ_entry *ಬೇರು,
		ವರ್ಣ *hex_oid, unsigned ಪೂರ್ಣಾಙ್ಕ hex_oid_len,
		ವರ್ಣ *fullpath, unsigned ಪೂರ್ಣಾಙ್ಕ fullpath_len,
		unsigned ವರ್ಣ fanout)
{
	ಸಂರಚನಾ ವೃಕ್ಷ_content *t;
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *e, leaf;
	unsigned ಪೂರ್ಣಾಙ್ಕ i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t ಅಂಕೆ_notes = 0;
	ಸಂರಚನಾ object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	ವರ್ಣ realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ಧ್ರುವ unsigned hexsz = the_hash_algo->hexsz;

	ಯದಿ (!ಬೇರು->ವೃಕ್ಷ)
		load_ವೃಕ್ಷ(ಬೇರು);
	t = ಬೇರು->ವೃಕ್ಷ;

	ಪ್ರತಿ (i = 0; t && i < t->entry_ಎಣಿಕೆ; i++) {
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
		ಯದಿ (!e->versions[1].mode ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			ಅನುವರ್ತ;

		/* This _may_ be a note entry, or a subdir containing notes */
		memcpy(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		ಯದಿ (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		memcpy(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		ಯದಿ (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			ಯದಿ (fanout == 0xff) {
				/* Counting mode, no rename */
				ಅಂಕೆ_notes++;
				ಅನುವರ್ತ;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			ಯದಿ (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				ಅಂಕೆ_notes++;
				ಅನುವರ್ತ;
			}

			/* Rename fullpath to realpath */
			ಯದಿ (!ವೃಕ್ಷ_content_remove(orig_ಬೇರು, fullpath, &leaf, 0))
				die(_("failed to remove path %s"), fullpath);
			ವೃಕ್ಷ_content_set(orig_ಬೇರು, realpath,
				&leaf.versions[1].oid,
				leaf.versions[1].mode,
				leaf.ವೃಕ್ಷ);
		} ಅನ್ಯಥಾ ಯದಿ (S_ISDIR(e->versions[1].mode)) {
			/* This is a subdir that may contain note entries */
			ಅಂಕೆ_notes += do_change_note_fanout(orig_ಬೇರು, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = ಬೇರು->ವೃಕ್ಷ;
	}
	ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_notes;
}

ಸ್ಥಿರ uintmax_t change_note_fanout(ಸಂರಚನಾ ವೃಕ್ಷ_entry *ಬೇರು,
		unsigned ವರ್ಣ fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	ವರ್ಣ hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	ಪ್ರತ್ಯಾಗಮ do_change_note_fanout(ಬೇರು, ಬೇರು, hex_oid, 0, path, 0, fanout);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_mapped_oid_hex(ಧ್ರುವ ವರ್ಣ *hex, ಸಂರಚನಾ object_id *oid, ಧ್ರುವ ವರ್ಣ **end)
{
	ಪೂರ್ಣಾಙ್ಕ algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, ಪರಿಮಾಣ(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	ಯದಿ (algo == GIT_HASH_UNKNOWN)
		ಪ್ರತ್ಯಾಗಮ -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	ಯದಿ (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		ಯದಿ (hash_algos[algo].format_id == the_hash_algo->format_id)
			ಪ್ರತ್ಯಾಗಮ 0;
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	oidcpy(oid, kh_ಮೌಲ್ಯ(sub_oid_map, it));
	ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ uintmax_t parse_mark_ref(ಧ್ರುವ ವರ್ಣ *p, ವರ್ಣ **endptr)
{
	uintmax_t mark;

	ಪ್ರತಿಜ್ಞಾ(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	ಯದಿ (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	ಪ್ರತ್ಯಾಗಮ mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
ಸ್ಥಿರ uintmax_t parse_mark_ref_eol(ಧ್ರುವ ವರ್ಣ *p)
{
	ವರ್ಣ *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	ಯದಿ (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	ಪ್ರತ್ಯಾಗಮ mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
ಸ್ಥಿರ uintmax_t parse_mark_ref_space(ಧ್ರುವ ವರ್ಣ **p)
{
	uintmax_t mark;
	ವರ್ಣ *end;

	mark = parse_mark_ref(*p, &end);
	ಯದಿ (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	ಪ್ರತ್ಯಾಗಮ mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
ಸ್ಥಿರ ಶೂನ್ಯ parse_path(ಸಂರಚನಾ strbuf *sb, ಧ್ರುವ ವರ್ಣ *p, ಧ್ರುವ ವರ್ಣ **endp,
		ಪೂರ್ಣಾಙ್ಕ is_last_field, ಧ್ರುವ ವರ್ಣ *field)
{
	ಯದಿ (*p == '"') {
		ಯದಿ (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), field, command_buf.buf);
		ಯದಿ (strlen(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), field, command_buf.buf);
	} ಅನ್ಯಥಾ {
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
ಸ್ಥಿರ ಶೂನ್ಯ parse_path_eol(ಸಂರಚನಾ strbuf *sb, ಧ್ರುವ ವರ್ಣ *p, ಧ್ರುವ ವರ್ಣ *field)
{
	ಧ್ರುವ ವರ್ಣ *end;

	parse_path(sb, p, &end, 1, field);
	ಯದಿ (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
ಸ್ಥಿರ ಶೂನ್ಯ parse_path_space(ಸಂರಚನಾ strbuf *sb, ಧ್ರುವ ವರ್ಣ *p,
		ಧ್ರುವ ವರ್ಣ **endp, ಧ್ರುವ ವರ್ಣ *field)
{
	parse_path(sb, p, endp, 0, field);
	ಯದಿ (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

ಸ್ಥಿರ ಶೂನ್ಯ file_change_m(ಸಂರಚನಾ fast_import_state *state,
			  ಧ್ರುವ ವರ್ಣ *p,
			  ಸಂರಚನಾ branch *b)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf path = STRBUF_INIT;
	ಸಂರಚನಾ object_entry *oe;
	ಸಂರಚನಾ object_id oid;
	uint16_t mode, inline_data = 0;

	p = parse_mode(p, &mode);
	ಯದಿ (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	ವಿಕಲ್ಪ (mode) {
	ಪಕ್ಷ 0644:
	ಪಕ್ಷ 0755:
		mode |= S_IFREG;
	ಪಕ್ಷ S_IFREG | 0644:
	ಪಕ್ಷ S_IFREG | 0755:
	ಪಕ್ಷ S_IFLNK:
	ಪಕ್ಷ S_IFDIR:
	ಪಕ್ಷ S_IFGITLINK:
		/* ok */
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	ಯದಿ (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = ರಿಕ್ತ; /* not used with inline_data, but makes gcc happy */
	} ಅನ್ಯಥಾ {
		ಯದಿ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		ಯದಿ (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	ಯದಿ (S_ISDIR(mode) &&
	    is_empty_ವೃಕ್ಷ_oid(&oid, the_repository->hash_algo) &&
	    *path.buf) {
		ವೃಕ್ಷ_content_remove(&b->branch_ವೃಕ್ಷ, path.buf, ರಿಕ್ತ, 0);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (S_ISGITLINK(mode)) {
		ಯದಿ (inline_data)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		ಅನ್ಯಥಾ ಯದಿ (oe) {
			ಯದಿ (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} ಅನ್ಯಥಾ ಯದಿ (inline_data) {
		ಯದಿ (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		ಯಾವತ್ (read_next_command(state) != EOF) {
			ಧ್ರುವ ವರ್ಣ *v;
			ಯದಿ (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			ಅನ್ಯಥಾ {
				parse_and_store_blob(&last_blob, &oid, 0);
				ಭಙ್ಗ;
			}
		}
	} ಅನ್ಯಥಾ {
		ಗಣನಾ object_type expected = S_ISDIR(mode) ?
						OBJ_ವೃಕ್ಷ: OBJ_BLOB;
		ಗಣನಾ object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, ರಿಕ್ತ);
		ಯದಿ (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(mode) ?  _("tree") : _("blob"),
			    command_buf.buf);
		ಯದಿ (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	ಯದಿ (!*path.buf) {
		ವೃಕ್ಷ_content_replace(&b->branch_ವೃಕ್ಷ, &oid, mode, ರಿಕ್ತ);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	ವೃಕ್ಷ_content_set(&b->branch_ವೃಕ್ಷ, path.buf, &oid, mode, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ file_change_d(ಧ್ರುವ ವರ್ಣ *p, ಸಂರಚನಾ branch *b)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	ವೃಕ್ಷ_content_remove(&b->branch_ವೃಕ್ಷ, path.buf, ರಿಕ್ತ, 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ file_change_cr(ಧ್ರುವ ವರ್ಣ *p, ಸಂರಚನಾ branch *b, ಪೂರ್ಣಾಙ್ಕ rename)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf source = STRBUF_INIT;
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf dest = STRBUF_INIT;
	ಸಂರಚನಾ ವೃಕ್ಷ_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	memset(&leaf, 0, ಪರಿಮಾಣ(leaf));
	ಯದಿ (rename)
		ವೃಕ್ಷ_content_remove(&b->branch_ವೃಕ್ಷ, source.buf, &leaf, 1);
	ಅನ್ಯಥಾ
		ವೃಕ್ಷ_content_get(&b->branch_ವೃಕ್ಷ, source.buf, &leaf, 1);
	ಯದಿ (!leaf.versions[1].mode)
		die(_("path %s not in branch"), source.buf);
	ಯದಿ (!*dest.buf) {	/* C "path/to/subdir" "" */
		ವೃಕ್ಷ_content_replace(&b->branch_ವೃಕ್ಷ,
			&leaf.versions[1].oid,
			leaf.versions[1].mode,
			leaf.ವೃಕ್ಷ);
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಯದಿ (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	ವೃಕ್ಷ_content_set(&b->branch_ವೃಕ್ಷ, dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.ವೃಕ್ಷ);
}

ಸ್ಥಿರ ಶೂನ್ಯ note_change_n(ಸಂರಚನಾ fast_import_state *state,
			  ಧ್ರುವ ವರ್ಣ *p,
			  ಸಂರಚನಾ branch *b,
			  unsigned ವರ್ಣ *old_fanout)
{
	ಸಂರಚನಾ object_entry *oe;
	ಸಂರಚನಾ branch *s;
	ಸಂರಚನಾ object_id oid, commit_oid;
	ವರ್ಣ path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_data = 0;
	unsigned ವರ್ಣ new_fanout;

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
	ಯದಿ (b->ಅಂಕೆ_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->ಅಂಕೆ_notes = change_note_fanout(&b->branch_ವೃಕ್ಷ, 0xff);
		*old_fanout = convert_ಅಂಕೆ_notes_to_fanout(b->ಅಂಕೆ_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	ಯದಿ (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = ರಿಕ್ತ; /* not used with inline_data, but makes gcc happy */
	} ಅನ್ಯಥಾ {
		ಯದಿ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		ಯದಿ (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	ಯದಿ (s) {
		ಯದಿ (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} ಅನ್ಯಥಾ ಯದಿ (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		ಸಂರಚನಾ object_entry *commit_oe = find_mark(marks, commit_mark);
		ಯದಿ (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} ಅನ್ಯಥಾ ಯದಿ (!repo_get_oid(the_repository, p, &commit_oid)) {
		size_t size;
		ವರ್ಣ *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &size,
						   &commit_oid);
		ಯದಿ (!buf || size < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		ಮುಕ್ತ(buf);
	} ಅನ್ಯಥಾ
		die(_("invalid ref name or SHA1 expression: %s"), p);

	ಯದಿ (inline_data) {
		read_next_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} ಅನ್ಯಥಾ ಯದಿ (oe) {
		ಯದಿ (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} ಅನ್ಯಥಾ ಯದಿ (!is_null_oid(&oid)) {
		ಗಣನಾ object_type type = odb_read_object_info(the_repository->objects, &oid,
							ರಿಕ್ತ);
		ಯದಿ (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		ಯದಿ (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	ಯದಿ (ವೃಕ್ಷ_content_remove(&b->branch_ವೃಕ್ಷ, path, ರಿಕ್ತ, 0))
		b->ಅಂಕೆ_notes--;

	ಯದಿ (is_null_oid(&oid))
		ಪ್ರತ್ಯಾಗಮ; /* nothing to insert */

	b->ಅಂಕೆ_notes++;
	new_fanout = convert_ಅಂಕೆ_notes_to_fanout(b->ಅಂಕೆ_notes);
	construct_path_with_fanout(oid_to_hex(&commit_oid), new_fanout, path);
	ವೃಕ್ಷ_content_set(&b->branch_ವೃಕ್ಷ, path, &oid, S_IFREG | 0644, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ file_change_deleteall(ಸಂರಚನಾ branch *b)
{
	release_ವೃಕ್ಷ_content_recursive(b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ);
	oidclr(&b->branch_ವೃಕ್ಷ.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_ವೃಕ್ಷ.versions[1].oid, the_repository->hash_algo);
	load_ವೃಕ್ಷ(&b->branch_ವೃಕ್ಷ);
	b->ಅಂಕೆ_notes = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_from_commit(ಸಂರಚನಾ branch *b, ವರ್ಣ *buf, unsigned long size)
{
	ಯದಿ (!buf || size < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	ಯದಿ (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_ವೃಕ್ಷ.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_ವೃಕ್ಷ.versions[0].oid,
	       &b->branch_ವೃಕ್ಷ.versions[1].oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_from_existing(ಸಂರಚನಾ branch *b)
{
	ಯದಿ (is_null_oid(&b->oid)) {
		oidclr(&b->branch_ವೃಕ್ಷ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_ವೃಕ್ಷ.versions[1].oid, the_repository->hash_algo);
	} ಅನ್ಯಥಾ {
		unsigned long size;
		size_t size_st = 0;
		ವರ್ಣ *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &size_st, &b->oid);
		size = cast_size_t_to_ulong(size_st);
		parse_from_commit(b, buf, size);
		ಮುಕ್ತ(buf);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_objectish(ಸಂರಚನಾ fast_import_state *state,
			   ಸಂರಚನಾ branch *b,
			   ಧ್ರುವ ವರ್ಣ *objectish)
{
	ಸಂರಚನಾ branch *s;
	ಸಂರಚನಾ object_id oid;

	oidcpy(&oid, &b->branch_ವೃಕ್ಷ.versions[1].oid);

	s = lookup_branch(objectish);
	ಯದಿ (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	ಅನ್ಯಥಾ ಯದಿ (s) {
		ಸಂರಚನಾ object_id *t = &s->branch_ವೃಕ್ಷ.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_ವೃಕ್ಷ.versions[0].oid, t);
		oidcpy(&b->branch_ವೃಕ್ಷ.versions[1].oid, t);
	} ಅನ್ಯಥಾ ಯದಿ (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		ಸಂರಚನಾ object_entry *oe = find_mark(marks, idnum);
		ಯದಿ (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		ಯದಿ (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			ಯದಿ (oe->pack_id != MAX_PACK_ID) {
				unsigned long size;
				ವರ್ಣ *buf = gfi_unpack_entry(oe, &size);
				parse_from_commit(b, buf, size);
				ಮುಕ್ತ(buf);
			} ಅನ್ಯಥಾ
				parse_from_existing(b);
		}
	} ಅನ್ಯಥಾ ಯದಿ (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		ಯದಿ (is_null_oid(&b->oid))
			b->delete = 1;
	}
	ಅನ್ಯಥಾ
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	ಯದಿ (b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ && !oideq(&oid, &b->branch_ವೃಕ್ಷ.versions[1].oid)) {
		release_ವೃಕ್ಷ_content_recursive(b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ);
		b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ = ರಿಕ್ತ;
	}

	read_next_command(state);
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_from(ಸಂರಚನಾ fast_import_state *state, ಸಂರಚನಾ branch *b)
{
	ಧ್ರುವ ವರ್ಣ *from;

	ಯದಿ (!skip_prefix(command_buf.buf, "from ", &from))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ parse_objectish(state, b, from);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_objectish_with_prefix(ಸಂರಚನಾ fast_import_state *state,
				       ಸಂರಚನಾ branch *b,
				       ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಧ್ರುವ ವರ್ಣ *base;

	ಯದಿ (!skip_prefix(command_buf.buf, prefix, &base))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ parse_objectish(state, b, base);
}

ಸ್ಥಿರ ಸಂರಚನಾ hash_list *parse_merge(ಸಂರಚನಾ fast_import_state *state,
				     unsigned ಪೂರ್ಣಾಙ್ಕ *ಎಣಿಕೆ)
{
	ಸಂರಚನಾ hash_list *list = ರಿಕ್ತ, **tail = &list, *n;
	ಧ್ರುವ ವರ್ಣ *from;
	ಸಂರಚನಾ branch *s;

	*ಎಣಿಕೆ = 0;
	ಯಾವತ್ (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(ಪರಿಮಾಣ(*n));
		s = lookup_branch(from);
		ಯದಿ (s)
			oidcpy(&n->oid, &s->oid);
		ಅನ್ಯಥಾ ಯದಿ (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			ಸಂರಚನಾ object_entry *oe = find_mark(marks, idnum);
			ಯದಿ (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} ಅನ್ಯಥಾ ಯದಿ (!repo_get_oid(the_repository, from, &n->oid)) {
			size_t size;
			ವರ್ಣ *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &size, &n->oid);
			ಯದಿ (!buf || size < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			ಮುಕ್ತ(buf);
		} ಅನ್ಯಥಾ
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->next = ರಿಕ್ತ;
		*tail = n;
		tail = &n->next;

		(*ಎಣಿಕೆ)++;
		read_next_command(state);
	}
	ಪ್ರತ್ಯಾಗಮ list;
}

ಸಂರಚನಾ signature_data {
	ವರ್ಣ *hash_algo;      /* "sha1" or "sha256" */
	ವರ್ಣ *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	ಸಂರಚನಾ strbuf data;   /* The actual signature data */
};

ಸ್ಥಿರ ಶೂನ್ಯ parse_one_signature(ಸಂರಚನಾ fast_import_state *state,
				ಸಂರಚನಾ signature_data *sig,
				ಧ್ರುವ ವರ್ಣ *v)
{
	ವರ್ಣ *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	ವರ್ಣ *space = strchr(args, ' ');

	ಯದಿ (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), args);
	*space = '\0';

	sig->hash_algo = args;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	ಯದಿ (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	ಯದಿ (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	ಯದಿ (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_next_command(state);
	parse_data(&sig->data, 0, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ discard_one_signature(ಸಂರಚನಾ fast_import_state *state)
{
	ಸಂರಚನಾ strbuf data = STRBUF_INIT;

	read_next_command(state);
	parse_data(&data, 0, ರಿಕ್ತ);
	strbuf_release(&data);
}

ಸ್ಥಿರ ಶೂನ್ಯ add_gpgsig_to_commit(ಸಂರಚನಾ strbuf *commit_data,
				 ಧ್ರುವ ವರ್ಣ *header,
				 ಸಂರಚನಾ signature_data *sig)
{
	ಸಂರಚನಾ string_list siglines = STRING_LIST_INIT_NODUP;

	ಯದಿ (!sig || !sig->hash_algo)
		ಪ್ರತ್ಯಾಗಮ;

	strbuf_addstr(commit_data, header);
	string_list_split_in_place(&siglines, sig->data.buf, "\n", -1);
	strbuf_add_separated_string_list(commit_data, "\n ", &siglines);
	strbuf_addch(commit_data, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->data);
	ಮುಕ್ತ(sig->hash_algo);
}

ಸ್ಥಿರ ಶೂನ್ಯ store_signature(ಸಂರಚನಾ signature_data *stored_sig,
			    ಸಂರಚನಾ signature_data *new_sig,
			    ಧ್ರುವ ವರ್ಣ *hash_type)
{
	ಯದಿ (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->data);
		ಮುಕ್ತ(new_sig->hash_algo);
	} ಅನ್ಯಥಾ {
		*stored_sig = *new_sig;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ import_one_signature(ಸಂರಚನಾ fast_import_state *state,
				 ಸಂರಚನಾ signature_data *sig_sha1,
				 ಸಂರಚನಾ signature_data *sig_sha256,
				 ಧ್ರುವ ವರ್ಣ *v)
{
	ಸಂರಚನಾ signature_data sig = { ರಿಕ್ತ, ರಿಕ್ತ, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	ಯದಿ (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	ಅನ್ಯಥಾ
		die(_("parse_one_signature() returned unknown hash algo"));
}

ಸ್ಥಿರ ಶೂನ್ಯ finalize_commit_buffer(ಸಂರಚನಾ strbuf *new_data,
				   ಸಂರಚನಾ signature_data *sig_sha1,
				   ಸಂರಚನಾ signature_data *sig_sha256,
				   ಸಂರಚನಾ strbuf *ಸಂದೇಶ)
{
	add_gpgsig_to_commit(new_data, "gpgsig ", sig_sha1);
	add_gpgsig_to_commit(new_data, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_data, '\n');
	strbuf_addbuf(new_data, ಸಂದೇಶ);
}

ಸ್ಥಿರ ಶೂನ್ಯ warn_invalid_signature(ಸಂರಚನಾ signature_check *check,
				   ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ಗಣನಾ sign_mode mode)
{
	ಧ್ರುವ ವರ್ಣ *signer = check->signer ? check->signer : _("unknown");
	ಧ್ರುವ ವರ್ಣ *subject;
	ಪೂರ್ಣಾಙ್ಕ subject_len = find_commit_subject(ಸಂದೇಶ, &subject);

	ವಿಕಲ್ಪ (mode) {
	ಪಕ್ಷ SIGN_STRIP_IF_INVALID:
		ಯದಿ (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		ಅನ್ಯಥಾ ಯದಿ (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		ಅನ್ಯಥಾ
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		ಭಙ್ಗ;
	ಪಕ್ಷ SIGN_SIGN_IF_INVALID:
		ಯದಿ (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		ಅನ್ಯಥಾ ಯದಿ (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		ಅನ್ಯಥಾ
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("unsupported signing mode");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_signature_if_invalid(ಸಂರಚನಾ strbuf *new_data,
					ಸಂರಚನಾ signature_data *sig_sha1,
					ಸಂರಚನಾ signature_data *sig_sha256,
					ಸಂರಚನಾ strbuf *ಸಂದೇಶ,
					ಗಣನಾ sign_mode mode)
{
	ಸಂರಚನಾ strbuf tmp_buf = STRBUF_INIT;
	ಸಂರಚನಾ signature_check signature_check = { 0 };
	ಪೂರ್ಣಾಙ್ಕ ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_data);
	finalize_commit_buffer(&tmp_buf, sig_sha1, sig_sha256, ಸಂದೇಶ);
	ret = verify_commit_buffer(tmp_buf.buf, tmp_buf.len, &signature_check);

	ಯದಿ (ret) {
		ಯದಿ (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, ಸಂದೇಶ->buf, mode);

		ಯದಿ (mode == SIGN_SIGN_IF_INVALID) {
			ಸಂರಚನಾ strbuf signature = STRBUF_INIT;
			ಸಂರಚನಾ strbuf payload = STRBUF_INIT;

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
			ಯದಿ (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			ಯದಿ (sign_buffer(&payload, &signature, signed_commit_keyid,
					SIGN_BUFFER_USE_DEFAULT_ಕೀಲಿ))
				die(_("failed to sign commit object"));
			add_header_signature(new_data, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_buffer(new_data, ರಿಕ್ತ, ರಿಕ್ತ, ಸಂದೇಶ);
	} ಅನ್ಯಥಾ {
		strbuf_swap(new_data, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_new_commit(ಸಂರಚನಾ fast_import_state *state, ಧ್ರುವ ವರ್ಣ *arg)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ signature_data sig_sha1 = { ರಿಕ್ತ, ರಿಕ್ತ, STRBUF_INIT };
	ಸಂರಚನಾ signature_data sig_sha256 = { ರಿಕ್ತ, ರಿಕ್ತ, STRBUF_INIT };
	ಸಂರಚನಾ branch *b;
	ವರ್ಣ *author = ರಿಕ್ತ;
	ವರ್ಣ *committer = ರಿಕ್ತ;
	ವರ್ಣ *encoding = ರಿಕ್ತ;
	ಸಂರಚನಾ hash_list *merge_list = ರಿಕ್ತ;
	unsigned ಪೂರ್ಣಾಙ್ಕ merge_ಎಣಿಕೆ;
	unsigned ವರ್ಣ prev_fanout, new_fanout;
	ಧ್ರುವ ವರ್ಣ *v;

	b = lookup_branch(arg);
	ಯದಿ (!b)
		b = new_branch(arg);

	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	ಯದಿ (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_next_command(state);
	}
	ಯದಿ (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_next_command(state);
	}
	ಯದಿ (!committer)
		die(_("expected committer but didn't get one"));

	ಯಾವತ್ (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		ವಿಕಲ್ಪ (signed_commit_mode) {

		/* First, modes that don't need the signature to be parsed */
		ಪಕ್ಷ SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		ಪಕ್ಷ SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		ಪಕ್ಷ SIGN_STRIP:
			discard_one_signature(state);
			ಭಙ್ಗ;

		/* Second, modes that parse the signature */
		ಪಕ್ಷ SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		ಪಕ್ಷ SIGN_VERBATIM:
		ಪಕ್ಷ SIGN_STRIP_IF_INVALID:
		ಪಕ್ಷ SIGN_SIGN_IF_INVALID:
		ಪಕ್ಷ SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			ಭಙ್ಗ;

		/* Third, BUG */
		ಸ್ವಭಾವ:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_next_command(state);
	}

	ಯದಿ (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_next_command(state);
	}
	parse_data(&ಸಂದೇಶ, 0, ರಿಕ್ತ);
	read_next_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_ಎಣಿಕೆ);

	/* ensure the branch is active/loaded */
	ಯದಿ (!b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	prev_fanout = convert_ಅಂಕೆ_notes_to_fanout(b->ಅಂಕೆ_notes);

	/* file_change* */
	ಯಾವತ್ (command_buf.len > 0) {
		ಯದಿ (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		ಅನ್ಯಥಾ ಯದಿ (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		ಅನ್ಯಥಾ {
			unread_command_buf = 1;
			ಭಙ್ಗ;
		}
		ಯದಿ (read_next_command(state) == EOF)
			ಭಙ್ಗ;
	}

	new_fanout = convert_ಅಂಕೆ_notes_to_fanout(b->ಅಂಕೆ_notes);
	ಯದಿ (new_fanout != prev_fanout)
		b->ಅಂಕೆ_notes = change_note_fanout(&b->branch_ವೃಕ್ಷ, new_fanout);

	/* build the tree and the commit */
	store_ವೃಕ್ಷ(&b->branch_ವೃಕ್ಷ);
	oidcpy(&b->branch_ವೃಕ್ಷ.versions[0].oid,
	       &b->branch_ವೃಕ್ಷ.versions[1].oid);

	strbuf_reset(&new_data);
	strbuf_addf(&new_data, "tree %s\n",
		oid_to_hex(&b->branch_ವೃಕ್ಷ.versions[1].oid));
	ಯದಿ (!is_null_oid(&b->oid))
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&b->oid));
	ಯಾವತ್ (merge_list) {
		ಸಂರಚನಾ hash_list *next = merge_list->next;
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&merge_list->oid));
		ಮುಕ್ತ(merge_list);
		merge_list = next;
	}
	strbuf_addf(&new_data,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	ಯದಿ (encoding)
		strbuf_addf(&new_data,
			"encoding %s\n",
			encoding);

	ಯದಿ ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
	     signed_commit_mode == SIGN_SIGN_IF_INVALID ||
	     signed_commit_mode == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_data, &sig_sha1, &sig_sha256,
					    &ಸಂದೇಶ, signed_commit_mode);
	ಅನ್ಯಥಾ
		finalize_commit_buffer(&new_data, &sig_sha1, &sig_sha256, &ಸಂದೇಶ);

	ಮುಕ್ತ(author);
	ಮುಕ್ತ(committer);
	ಮುಕ್ತ(encoding);

	ಯದಿ (!store_object(OBJ_COMMIT, &new_data, ರಿಕ್ತ, &b->oid, next_mark))
		b->pack_id = pack_id;
	b->last_commit = object_ಎಣಿಕೆ_by_type[OBJ_COMMIT];
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_tag_signature_if_invalid(ಸಂರಚನಾ strbuf *buf,
					    ಸಂರಚನಾ strbuf *ಸಂದೇಶ,
					    size_t sig_offset)
{
	ಸಂರಚನಾ strbuf signature = STRBUF_INIT;
	ಸಂರಚನಾ strbuf payload = STRBUF_INIT;
	ಸಂರಚನಾ signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_add(&payload, ಸಂದೇಶ->buf, sig_offset);
	strbuf_add(&signature, ಸಂದೇಶ->buf + sig_offset, ಸಂದೇಶ->len - sig_offset);

	sigc.payload_type = SIGNATURE_PAYLOAD_TAG;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	ಯದಿ (!check_signature(&sigc, signature.buf, signature.len))
		ಗಚ್ಛ out;

	ಯದಿ (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(ಸಂದೇಶ, sig_offset);

	ಯದಿ (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = ರಿಕ್ತ;
		strbuf_reset(&signature);

		ಯದಿ (sign_buffer(&payload, &signature, signed_tag_keyid,
				SIGN_BUFFER_USE_DEFAULT_ಕೀಲಿ))
			die(_("failed to sign tag object"));

		strbuf_addbuf(ಸಂದೇಶ, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_tag_signature(ಸಂರಚನಾ strbuf *buf, ಸಂರಚನಾ strbuf *ಸಂದೇಶ, ಧ್ರುವ ವರ್ಣ *name)
{
	size_t sig_offset = parse_signed_buffer(ಸಂದೇಶ->buf, ಸಂದೇಶ->len);

	/* If there is no signature, there is nothing to do. */
	ಯದಿ (sig_offset >= ಸಂದೇಶ->len)
		ಪ್ರತ್ಯಾಗಮ;

	ವಿಕಲ್ಪ (signed_tag_mode) {

	/* First, modes that don't change anything */
	ಪಕ್ಷ SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	ಪಕ್ಷ SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		ಭಙ್ಗ;

	/* Second, modes that remove the signature */
	ಪಕ್ಷ SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	ಪಕ್ಷ SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(ಸಂದೇಶ, sig_offset);
		ಭಙ್ಗ;
	ಪಕ್ಷ SIGN_ABORT_IF_INVALID:
	ಪಕ್ಷ SIGN_SIGN_IF_INVALID:
	ಪಕ್ಷ SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, ಸಂದೇಶ, sig_offset);
		ಭಙ್ಗ;

	/* Third, aborting modes */
	ಪಕ್ಷ SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	ಸ್ವಭಾವ:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_new_tag(ಸಂರಚನಾ fast_import_state *state, ಧ್ರುವ ವರ್ಣ *arg)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *from;
	ವರ್ಣ *tagger;
	ಸಂರಚನಾ branch *s;
	ಸಂರಚನಾ tag *t;
	uintmax_t from_mark = 0;
	ಸಂರಚನಾ object_id oid;
	ಗಣನಾ object_type type;
	ಧ್ರುವ ವರ್ಣ *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, ಪರಿಮಾಣ(ಸಂರಚನಾ tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	ಯದಿ (last_tag)
		last_tag->next_tag = t;
	ಅನ್ಯಥಾ
		first_tag = t;
	last_tag = t;
	read_next_command(state);
	parse_mark(state);

	/* from ... */
	ಯದಿ (!skip_prefix(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	ಯದಿ (s) {
		ಯದಿ (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} ಅನ್ಯಥಾ ಯದಿ (*from == ':') {
		ಸಂರಚನಾ object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} ಅನ್ಯಥಾ ಯದಿ (!repo_get_oid(the_repository, from, &oid)) {
		ಸಂರಚನಾ object_entry *oe = find_object(&oid);
		ಯದಿ (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, ರಿಕ್ತ);
			ಯದಿ (type < 0)
				die(_("not a valid object: %s"), from);
		} ಅನ್ಯಥಾ
			type = oe->type;
	} ಅನ್ಯಥಾ
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_next_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	ಯದಿ (skip_prefix(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_next_command(state);
	} ಅನ್ಯಥಾ
		tagger = ರಿಕ್ತ;

	/* tag payload/message */
	parse_data(&ಸಂದೇಶ, 0, ರಿಕ್ತ);

	/* build the tag object */
	strbuf_reset(&new_data);

	strbuf_addf(&new_data,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_to_hex(&oid), type_name(type), t->name);
	ಯದಿ (tagger)
		strbuf_addf(&new_data,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_data, &ಸಂದೇಶ, t->name);

	strbuf_addch(&new_data, '\n');
	strbuf_addbuf(&new_data, &ಸಂದೇಶ);
	ಮುಕ್ತ(tagger);

	ಯದಿ (store_object(OBJ_TAG, &new_data, ರಿಕ್ತ, &t->oid, next_mark))
		t->pack_id = MAX_PACK_ID;
	ಅನ್ಯಥಾ
		t->pack_id = pack_id;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_reset_branch(ಸಂರಚನಾ fast_import_state *state, ಧ್ರುವ ವರ್ಣ *arg)
{
	ಸಂರಚನಾ branch *b;
	ಧ್ರುವ ವರ್ಣ *tag_name;

	b = lookup_branch(arg);
	ಯದಿ (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_ವೃಕ್ಷ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_ವೃಕ್ಷ.versions[1].oid, the_repository->hash_algo);
		ಯದಿ (b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ) {
			release_ವೃಕ್ಷ_content_recursive(b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ);
			b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ = ರಿಕ್ತ;
		}
	}
	ಅನ್ಯಥಾ
		b = new_branch(arg);
	read_next_command(state);
	parse_from(state, b);
	ಯದಿ (b->delete && skip_prefix(b->name, "refs/tags/", &tag_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		ಸಂರಚನಾ tag *t, *prev = ರಿಕ್ತ;
		ಪ್ರತಿ (t = first_tag; t; t = t->next_tag) {
			ಯದಿ (!strcmp(t->name, tag_name))
				ಭಙ್ಗ;
			prev = t;
		}
		ಯದಿ (t) {
			ಯದಿ (prev)
				prev->next_tag = t->next_tag;
			ಅನ್ಯಥಾ
				first_tag = t->next_tag;
			ಯದಿ (!t->next_tag)
				last_tag = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	ಯದಿ (command_buf.len > 0)
		unread_command_buf = 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ cat_blob_write(ಧ್ರುವ ವರ್ಣ *buf, unsigned long size)
{
	ಯದಿ (write_in_full(cat_blob_fd, buf, size) < 0)
		die_errno(_("write to frontend failed"));
}

ಸ್ಥಿರ ಶೂನ್ಯ cat_blob(ಸಂರಚನಾ object_entry *oe, ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ strbuf ಸಾಲು = STRBUF_INIT;
	ಸಂರಚನಾ iovec iov[3];
	unsigned long size;
	ಗಣನಾ object_type type = 0;
	ವರ್ಣ *buf;

	ಯದಿ (!oe || oe->pack_id == MAX_PACK_ID) {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
	} ಅನ್ಯಥಾ {
		type = oe->type;
		buf = gfi_unpack_entry(oe, &size);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	ಯದಿ (type <= 0) {
		strbuf_reset(&ಸಾಲು);
		strbuf_addf(&ಸಾಲು, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(ಸಾಲು.buf, ಸಾಲು.len);
		strbuf_release(&ಸಾಲು);
		ಮುಕ್ತ(buf);
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಯದಿ (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	ಯದಿ (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_to_hex(oid), type_name(type));
	strbuf_reset(&ಸಾಲು);
	strbuf_addf(&ಸಾಲು, "%s %s %"PRIuMAX"\n", oid_to_hex(oid),
		    type_name(type), (uintmax_t)size);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_base = ಸಾಲು.buf;
	iov[0].iov_len = ಸಾಲು.len;
	iov[1].iov_base = buf;
	iov[1].iov_len = size;
	iov[2].iov_base = (ಶೂನ್ಯ *) "\n";
	iov[2].iov_len = 1;

	ಯದಿ (writev_in_full(cat_blob_fd, iov, ARRAY_SIZE(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&ಸಾಲು);
	ಯದಿ (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.data, buf, size, size + 1);
		last_blob.depth = oe->depth;
	} ಅನ್ಯಥಾ
		ಮುಕ್ತ(buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_get_mark(ಸಂರಚನಾ fast_import_state *state UNUSED,
			   ಧ್ರುವ ವರ್ಣ *p)
{
	ಸಂರಚನಾ object_entry *oe;
	ವರ್ಣ output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	ಯದಿ (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	ಯದಿ (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, ಪರಿಮಾಣ(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cat_blob(ಸಂರಚನಾ fast_import_state *state UNUSED,
			   ಧ್ರುವ ವರ್ಣ *p)
{
	ಸಂರಚನಾ object_entry *oe;
	ಸಂರಚನಾ object_id oid;

	/* cat-blob SP <object> LF */
	ಯದಿ (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		ಯದಿ (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} ಅನ್ಯಥಾ {
		ಯದಿ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		ಯದಿ (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

ಸ್ಥಿರ ಸಂರಚನಾ object_entry *dereference(ಸಂರಚನಾ object_entry *oe,
					ಸಂರಚನಾ object_id *oid)
{
	unsigned long size;
	ವರ್ಣ *buf = ರಿಕ್ತ;
	ಧ್ರುವ unsigned hexsz = the_hash_algo->hexsz;

	ಯದಿ (!oe) {
		ಗಣನಾ object_type type = odb_read_object_info(the_repository->objects,
							     oid, ರಿಕ್ತ);
		ಯದಿ (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = ಸೇರಿಸು_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	ವಿಕಲ್ಪ (oe->type) {
	ಪಕ್ಷ OBJ_ವೃಕ್ಷ:	/* easy case. */
		ಪ್ರತ್ಯಾಗಮ oe;
	ಪಕ್ಷ OBJ_COMMIT:
	ಪಕ್ಷ OBJ_TAG:
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	ಯದಿ (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &size);
	} ಅನ್ಯಥಾ {
		ಗಣನಾ object_type unused;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &size_st);
		size = cast_size_t_to_ulong(size_st);
	}
	ಯದಿ (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	ವಿಕಲ್ಪ (oe->type) {
	ಪಕ್ಷ OBJ_TAG:
		ಯದಿ (size < hexsz + strlen("object ") ||
		    get_oid_hex(buf + strlen("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		ಭಙ್ಗ;
	ಪಕ್ಷ OBJ_COMMIT:
		ಯದಿ (size < hexsz + strlen("tree ") ||
		    get_oid_hex(buf + strlen("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	ಮುಕ್ತ(buf);
	ಪ್ರತ್ಯಾಗಮ find_object(oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_mapped_mark(uintmax_t mark, ಶೂನ್ಯ *object, ಶೂನ್ಯ *cbp)
{
	ಸಂರಚನಾ object_id *fromoid = object;
	ಸಂರಚನಾ object_id *tooid = find_mark(cbp, mark);
	ಪೂರ್ಣಾಙ್ಕ ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	ಯದಿ (ret == 0)
		ಪ್ರತ್ಯಾಗಮ;
	kh_ಮೌಲ್ಯ(sub_oid_map, it) = tooid;
}

ಸ್ಥಿರ ಶೂನ್ಯ build_mark_map_one(ಸಂರಚನಾ mark_set *from, ಸಂರಚನಾ mark_set *to)
{
	for_each_mark(from, 0, ಸೇರಿಸು_mapped_mark, to);
}

ಸ್ಥಿರ ಶೂನ್ಯ build_mark_map(ಸಂರಚನಾ string_list *from, ಸಂರಚನಾ string_list *to)
{
	ಸಂರಚನಾ string_list_item *fromp, *ತುದಿ;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		ತುದಿ = string_list_lookup(to, fromp->string);
		ಯದಿ (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} ಅನ್ಯಥಾ ಯದಿ (!ತುದಿ || !ತುದಿ->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, ತುದಿ->util);
	}
}

ಸ್ಥಿರ ಸಂರಚನಾ object_entry *parse_treeish_dataref(ಧ್ರುವ ವರ್ಣ **p)
{
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ object_entry *e;

	ಯದಿ (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		ಯದಿ (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} ಅನ್ಯಥಾ {	/* <sha1> */
		ಯದಿ (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		ಯದಿ (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	ಯಾವತ್ (!e || e->type != OBJ_ವೃಕ್ಷ)
		e = dereference(e, &oid);
	ಪ್ರತ್ಯಾಗಮ e;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_ls(ಪೂರ್ಣಾಙ್ಕ mode, ಧ್ರುವ unsigned ವರ್ಣ *hash, ಧ್ರುವ ವರ್ಣ *path)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf ಸಾಲು = STRBUF_INIT;

	/* See show_tree(). */
	ಧ್ರುವ ವರ್ಣ *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? ವೃಕ್ಷ_type :
		blob_type;

	ಯದಿ (!mode) {
		/* missing SP path LF */
		strbuf_reset(&ಸಾಲು);
		strbuf_addstr(&ಸಾಲು, "missing ");
		quote_c_style(path, &ಸಾಲು, ರಿಕ್ತ, 0);
		strbuf_addch(&ಸಾಲು, '\n');
	} ಅನ್ಯಥಾ {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&ಸಾಲು);
		strbuf_addf(&ಸಾಲು, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &ಸಾಲು, ರಿಕ್ತ, 0);
		strbuf_addch(&ಸಾಲು, '\n');
	}
	cat_blob_write(ಸಾಲು.buf, ಸಾಲು.len);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_ls(ಸಂರಚನಾ fast_import_state *state UNUSED,
		     ಧ್ರುವ ವರ್ಣ *p,
		     ಸಂರಚನಾ branch *b)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf path = STRBUF_INIT;
	ಸಂರಚನಾ ವೃಕ್ಷ_entry *ಬೇರು = ರಿಕ್ತ;
	ಸಂರಚನಾ ವೃಕ್ಷ_entry leaf = {ರಿಕ್ತ};

	/* ls SP (<tree-ish> SP)? <path> */
	ಯದಿ (*p == '"') {
		ಯದಿ (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		ಬೇರು = &b->branch_ವೃಕ್ಷ;
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ object_entry *e = parse_treeish_dataref(&p);
		ಬೇರು = new_ವೃಕ್ಷ_entry();
		oidcpy(&ಬೇರು->versions[1].oid, &e->idx.oid);
		ಯದಿ (!is_null_oid(&ಬೇರು->versions[1].oid))
			ಬೇರು->versions[1].mode = S_IFDIR;
		load_ವೃಕ್ಷ(ಬೇರು);
	}
	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	ವೃಕ್ಷ_content_get(ಬೇರು, path.buf, &leaf, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	ಯದಿ (S_ISDIR(leaf.versions[1].mode))
		store_ವೃಕ್ಷ(&leaf);

	print_ls(leaf.versions[1].mode, leaf.versions[1].oid.hash, path.buf);
	ಯದಿ (leaf.ವೃಕ್ಷ)
		release_ವೃಕ್ಷ_content_recursive(leaf.ವೃಕ್ಷ);
	ಯದಿ (!b || ಬೇರು != &b->branch_ವೃಕ್ಷ)
		release_ವೃಕ್ಷ_entry(ಬೇರು);
}

ಸ್ಥಿರ ಶೂನ್ಯ checkpoint(ಶೂನ್ಯ)
{
	checkpoint_requested = 0;
	ಯದಿ (object_ಎಣಿಕೆ) {
		cycle_packfile();
	}
	dump_branches();
	dump_tags();
	dump_marks();
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_checkpoint(ಸಂರಚನಾ fast_import_state *state UNUSED)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_progress(ಸಂರಚನಾ fast_import_state *state UNUSED)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_alias(ಸಂರಚನಾ fast_import_state *state)
{
	ಸಂರಚನಾ object_entry *e;
	ಸಂರಚನಾ branch b;

	skip_optional_lf();
	read_next_command(state);

	/* mark ... */
	parse_mark(state);
	ಯದಿ (!next_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, ಪರಿಮಾಣ(b));
	ಯದಿ (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	ಪ್ರತಿಜ್ಞಾ(e);
	ಸೇರಿಸು_mark(&marks, next_mark, e);
}

ಸ್ಥಿರ ವರ್ಣ* make_fast_import_path(ಸಂರಚನಾ fast_import_state *state,
				   ಧ್ರುವ ವರ್ಣ *path)
{
	ಯದಿ (!relative_marks_paths || is_absolute_path(path))
		ಪ್ರತ್ಯಾಗಮ prefix_filename(state->prefix, path);
	ಪ್ರತ್ಯಾಗಮ repo_git_path(the_repository, "info/fast-import/%s", path);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_import_marks(ಸಂರಚನಾ fast_import_state *state,
				ಧ್ರುವ ವರ್ಣ *marks,
				ಪೂರ್ಣಾಙ್ಕ from_stream,
				ಪೂರ್ಣಾಙ್ಕ ignore_missing)
{
	ಯದಿ (import_marks_file) {
		ಯದಿ (from_stream)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		ಯದಿ(!import_marks_file_from_stream)
			read_marks();
	}

	ಮುಕ್ತ(import_marks_file);
	import_marks_file = make_fast_import_path(state, marks);
	import_marks_file_from_stream = from_stream;
	import_marks_file_ignore_missing = ignore_missing;
}

ಸ್ಥಿರ ಶೂನ್ಯ option_date_format(ಧ್ರುವ ವರ್ಣ *fmt)
{
	ಯದಿ (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	ಅನ್ಯಥಾ
		die(_("unknown --date-format argument %s"), fmt);
}

ಸ್ಥಿರ unsigned long ulong_arg(ಧ್ರುವ ವರ್ಣ *option, ಧ್ರುವ ವರ್ಣ *arg)
{
	ವರ್ಣ *endptr;
	unsigned long rv = strtoul(arg, &endptr, 0);
	ಯದಿ (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	ಪ್ರತ್ಯಾಗಮ rv;
}

ಸ್ಥಿರ ಶೂನ್ಯ option_depth(ಧ್ರುವ ವರ್ಣ *depth)
{
	max_depth = ulong_arg("--depth", depth);
	ಯದಿ (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_active_branches(ಧ್ರುವ ವರ್ಣ *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_export_marks(ಸಂರಚನಾ fast_import_state *state,
				ಧ್ರುವ ವರ್ಣ *marks)
{
	ಮುಕ್ತ(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_cat_blob_fd(ಸಂರಚನಾ fast_import_state *state UNUSED,
			       ಧ್ರುವ ವರ್ಣ *fd)
{
	unsigned long n = ulong_arg("--cat-blob-fd", fd);
	ಯದಿ (n > (unsigned long) INT_MAX)
		die(_("--cat-blob-fd cannot exceed %d"), INT_MAX);
	cat_blob_fd = (ಪೂರ್ಣಾಙ್ಕ) n;
}

ಸ್ಥಿರ ಶೂನ್ಯ option_export_pack_edges(ಸಂರಚನಾ fast_import_state *state,
				     ಧ್ರುವ ವರ್ಣ *edges)
{
	ವರ್ಣ *fn = prefix_filename(state->prefix, edges);
	ಯದಿ (pack_edges)
		fclose(pack_edges);
	pack_edges = xfopen(fn, "a");
	ಮುಕ್ತ(fn);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_rewrite_submodules(ಸಂರಚನಾ fast_import_state *state,
				      ಧ್ರುವ ವರ್ಣ *arg,
				      ಸಂರಚನಾ string_list *list)
{
	ಸಂರಚನಾ mark_set *ms;
	FILE *fp;
	ವರ್ಣ *s = xstrdup(arg);
	ವರ್ಣ *f = strchr(s, ':');
	ಯದಿ (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = prefix_filename(state->prefix, f);
	fp = fopen(f, "r");
	ಯದಿ (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, ಸೇರಿಸು_oid_entry);
	fclose(fp);
	ಮುಕ್ತ(f);

	string_list_ಸೇರಿಸು(list, s)->util = ms;

	ಮುಕ್ತ(s);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_max_pack_size(ಧ್ರುವ ವರ್ಣ *arg)
{
	unsigned long v;

	ಯದಿ (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	ಯದಿ (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} ಅನ್ಯಥಾ ಯದಿ (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

ಸ್ಥಿರ ಶೂನ್ಯ option_big_file_threshold(ಧ್ರುವ ವರ್ಣ *arg)
{
	unsigned long v;

	ಯದಿ (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_signed_commits(ಧ್ರುವ ವರ್ಣ *arg)
{
	ಯದಿ (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_signed_tags(ಧ್ರುವ ವರ್ಣ *arg)
{
	ಯದಿ (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_quiet(ಶೂನ್ಯ)
{
	show_stats = 0;
	quiet = 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_one_option(ಸಂರಚನಾ fast_import_state *state, ಧ್ರುವ ವರ್ಣ *option)
{
	ಯದಿ (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_size(option);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(option, "big-file-threshold=", &option)) {
		option_big_file_threshold(option);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(option, "depth=", &option)) {
		option_depth(option);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(option, "active-branches=", &option)) {
		option_active_branches(option);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(option, "export-pack-edges=", &option)) {
		option_export_pack_edges(state, option);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(option, "signed-tags=", &option)) {
		option_signed_tags(option);
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(option, "quiet")) {
		option_quiet();
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(option, "stats")) {
		show_stats = 1;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} ಅನ್ಯಥಾ {
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_unsafe_feature(ಸಂರಚನಾ fast_import_state *state,
				 ಧ್ರುವ ವರ್ಣ *feature)
{
	ಯದಿ (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_one_feature(ಸಂರಚನಾ fast_import_state *state,
			     ಧ್ರುವ ವರ್ಣ *feature)
{
	ಧ್ರುವ ವರ್ಣ *arg;

	ಯದಿ (skip_prefix(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_to);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(feature, "force")) {
		force_update = 1;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} ಅನ್ಯಥಾ {
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_feature(ಸಂರಚನಾ fast_import_state *state, ಧ್ರುವ ವರ್ಣ *feature)
{
	ಯದಿ (state->seen_data_command)
		die(_("got feature command '%s' after data command"), feature);

	ಯದಿ (parse_one_feature(state, feature))
		ಪ್ರತ್ಯಾಗಮ;

	die(_("this version of fast-import does not support feature %s."), feature);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_option(ಸಂರಚನಾ fast_import_state *state, ಧ್ರುವ ವರ್ಣ *option)
{
	ಯದಿ (state->seen_data_command)
		die(_("got option command '%s' after data command"), option);

	ಯದಿ (parse_one_option(state, option))
		ಪ್ರತ್ಯಾಗಮ;

	die(_("this version of fast-import does not support option: %s"), option);
}

ಸ್ಥಿರ ಶೂನ್ಯ git_pack_config(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ indexversion_ಮೌಲ್ಯ;
	ಪೂರ್ಣಾಙ್ಕ limit;
	unsigned long packsizelimit_ಮೌಲ್ಯ;

	ಯದಿ (!repo_config_get_ulong(the_repository, "pack.depth", &max_depth)) {
		ಯದಿ (max_depth > MAX_DEPTH)
			max_depth = MAX_DEPTH;
	}
	ಯದಿ (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_ಮೌಲ್ಯ)) {
		pack_idx_opts.version = indexversion_ಮೌಲ್ಯ;
		ಯದಿ (pack_idx_opts.version > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.version);
	}
	ಯದಿ (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_ಮೌಲ್ಯ))
		max_packsize = packsizelimit_ಮೌಲ್ಯ;

	ಯದಿ (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &limit))
		unpack_limit = limit;
	ಅನ್ಯಥಾ ಯದಿ (!repo_config_get_int(the_repository, "transfer.unpacklimit", &limit))
		unpack_limit = limit;

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಶೂನ್ಯ parse_argv(ಸಂರಚನಾ fast_import_state *state)
{
	ಪೂರ್ಣಾಙ್ಕ argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	ಯದಿ (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_data_command = 1;
	ಯದಿ (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_date_format(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
				    ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_export_pack_edges(ಧ್ರುವ ಸಂರಚನಾ option *opt,
					  ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->ಮೌಲ್ಯ, arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_max_pack_size(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
				      ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_size(arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_big_file_threshold(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
					   ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_signed_commits(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
				       ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_signed_tags(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
				    ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_rewrite_submodules_from(ಧ್ರುವ ಸಂರಚನಾ option *opt,
						ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->ಮೌಲ್ಯ, arg, &sub_marks_from);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_rewrite_submodules_to(ಧ್ರುವ ಸಂರಚನಾ option *opt,
					      ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->ಮೌಲ್ಯ, arg, &sub_marks_to);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_cat_blob_fd(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				    ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->ಮೌಲ್ಯ, arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_import_marks(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				     ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->ಮೌಲ್ಯ, arg, 0, 0);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_import_marks_if_exists(ಧ್ರುವ ಸಂರಚನಾ option *opt,
					       ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->ಮೌಲ್ಯ, arg, 0, 1);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_export_marks(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				     ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->ಮೌಲ್ಯ, arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_depth(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
			      ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_active_branches(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
					ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_quiet(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
			      ಧ್ರುವ ವರ್ಣ *arg UNUSED, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_fast_import(ಪೂರ್ಣಾಙ್ಕ argc,
		    ಧ್ರುವ ವರ್ಣ **argv,
		    ಧ್ರುವ ವರ್ಣ *prefix,
		    ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ fast_import_state state;

	ಸಂರಚನಾ option fast_import_options[] = {
		OPT_GROUP(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", ರಿಕ್ತ, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", ರಿಕ್ತ, ರಿಕ್ತ,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", ರಿಕ್ತ, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_size),
		OPT_CALLBACK_F(0, "big-file-threshold", ರಿಕ್ತ, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_threshold),
		OPT_CALLBACK_F(0, "depth", ರಿಕ್ತ, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_depth),
		OPT_CALLBACK_F(0, "active-branches", ರಿಕ್ತ, N_("n"),
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
		OPT_CALLBACK_F(0, "signed-commits", ರಿಕ್ತ, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", ರಿಕ್ತ, N_("mode"),
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
	CALLOC_ARRAY(avail_ವೃಕ್ಷ_table, avail_ವೃಕ್ಷ_table_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, ಪರಿಮಾಣ(ಸಂರಚನಾ mark_set));

	hashmap_init(&object_table, object_entry_hashcmp, ರಿಕ್ತ, 0);

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
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[i];
		ಯದಿ (*arg != '-' || !strcmp(arg, "--"))
			ಭಙ್ಗ;
		ಯದಿ (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * ಪರಿಮಾಣ(*rc_free));
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < (cmd_save - 1); i++)
		rc_free[i].next = &rc_free[i + 1];
	rc_free[cmd_save - 1].next = ರಿಕ್ತ;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	ಯಾವತ್ (read_next_command(&state) != EOF) {
		ಧ್ರುವ ವರ್ಣ *v;
		ಯದಿ (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, ರಿಕ್ತ);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		ಅನ್ಯಥಾ ಯದಿ (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		ಅನ್ಯಥಾ ಯದಿ (!strcmp("done", command_buf.buf))
			ಭಙ್ಗ;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		ಅನ್ಯಥಾ ಯದಿ (starts_with(command_buf.buf, "progress "))
			parse_progress(&state);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		ಅನ್ಯಥಾ ಯದಿ (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		ಅನ್ಯಥಾ
			die(_("unsupported command: %s"), command_buf.buf);

		ಯದಿ (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	ಯದಿ (!state.seen_data_command)
		parse_argv(&state);

	ಯದಿ (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	end_packfile();

	dump_branches();
	dump_tags();
	unkeep_all_packs();
	dump_marks();

	ಯದಿ (pack_edges)
		fclose(pack_edges);

	ಯದಿ (show_stats) {
		uintmax_t total_ಎಣಿಕೆ = 0, duplicate_ಎಣಿಕೆ = 0;
		ಪ್ರತಿ (size_t i = 0; i < ARRAY_SIZE(object_ಎಣಿಕೆ_by_type); i++)
			total_ಎಣಿಕೆ += object_ಎಣಿಕೆ_by_type[i];
		ಪ್ರತಿ (size_t i = 0; i < ARRAY_SIZE(duplicate_ಎಣಿಕೆ_by_type); i++)
			duplicate_ಎಣಿಕೆ += duplicate_ಎಣಿಕೆ_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_ಎಣಿಕೆ);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", total_ಎಣಿಕೆ, duplicate_ಎಣಿಕೆ);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ಎಣಿಕೆ_by_type[OBJ_BLOB], duplicate_ಎಣಿಕೆ_by_type[OBJ_BLOB], delta_ಎಣಿಕೆ_by_type[OBJ_BLOB], delta_ಎಣಿಕೆ_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ಎಣಿಕೆ_by_type[OBJ_ವೃಕ್ಷ], duplicate_ಎಣಿಕೆ_by_type[OBJ_ವೃಕ್ಷ], delta_ಎಣಿಕೆ_by_type[OBJ_ವೃಕ್ಷ], delta_ಎಣಿಕೆ_attempts_by_type[OBJ_ವೃಕ್ಷ]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ಎಣಿಕೆ_by_type[OBJ_COMMIT], duplicate_ಎಣಿಕೆ_by_type[OBJ_COMMIT], delta_ಎಣಿಕೆ_by_type[OBJ_COMMIT], delta_ಎಣಿಕೆ_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ಎಣಿಕೆ_by_type[OBJ_TAG], duplicate_ಎಣಿಕೆ_by_type[OBJ_TAG], delta_ಎಣಿಕೆ_by_type[OBJ_TAG], delta_ಎಣಿಕೆ_attempts_by_type[OBJ_TAG]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_ಎಣಿಕೆ, branch_load_ಎಣಿಕೆ);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->shift) * 1024, marks_set_ಎಣಿಕೆ);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (ವೃಕ್ಷ_entry_allocd + fi_mem_pool.pool_alloc + alloc_ಎಣಿಕೆ*ಪರಿಮಾಣ(ಸಂರಚನಾ object_entry))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (unsigned long)((ವೃಕ್ಷ_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_ಎಣಿಕೆ*ಪರಿಮಾಣ(ಸಂರಚನಾ object_entry))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	ಪ್ರತ್ಯಾಗಮ failure ? 1 : 0;
}
