#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "object.h"
#ଅନ୍ତର୍ଭାଵ "blob.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "delta.h"
#ଅନ୍ତର୍ଭାଵ "pack.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "csum-file.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "packfile.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "mem-pool.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"
#ଅନ୍ତର୍ଭାଵ "khash.h"
#ଅନ୍ତର୍ଭାଵ "date.h"
#ଅନ୍ତର୍ଭାଵ "gpg-interface.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"

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

ସଂରଚନା object_entry {
	ସଂରଚନା pack_idx_entry idx;
	ସଂରଚନା hashmap_entry ent;
	uint32_t type : TYPE_BITS,
		pack_id : PACK_ID_BITS,
		depth : DEPTH_BITS;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା object_entry_hashcmp(ଅଚଳ ଶୂନ୍ୟ *map_data UNUSED,
				ଅଚଳ ସଂରଚନା hashmap_entry *eptr,
				ଅଚଳ ସଂରଚନା hashmap_entry *entry_or_ଚାବି,
				ଅଚଳ ଶୂନ୍ୟ *keydata)
{
	ଅଚଳ ସଂରଚନା object_id *oid = keydata;
	ଅଚଳ ସଂରଚନା object_entry *e1, *e2;

	e1 = container_of(eptr, ଅଚଳ ସଂରଚନା object_entry, ent);
	ଯଦି (oid)
		ଫେରନ୍ତୁ oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_ଚାବି, ଅଚଳ ସଂରଚନା object_entry, ent);
	ଫେରନ୍ତୁ oidcmp(&e1->idx.oid, &e2->idx.oid);
}

ସଂରଚନା object_entry_pool {
	ସଂରଚନା object_entry_pool *next_pool;
	ସଂରଚନା object_entry *next_free;
	ସଂରଚନା object_entry *end;
	ସଂରଚନା object_entry entries[FLEX_ARRAY]; /* more */
};

ସଂରଚନା mark_set {
	ସଂଘ {
		ସଂରଚନା object_id *oids[1024];
		ସଂରଚନା object_entry *marked[1024];
		ସଂରଚନା mark_set *sets[1024];
	} data;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା shift;
};

ସଂରଚନା last_object {
	ସଂରଚନା strbuf data;
	off_t offset;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth;
	unsigned no_swap : 1;
};

ସଂରଚନା atom_str {
	ସଂରଚନା atom_str *next_atom;
	unsigned short str_len;
	ଅକ୍ଷର str_dat[FLEX_ARRAY]; /* more */
};

ସଂରଚନା ବୃକ୍ଷ_content;
ସଂରଚନା ବୃକ୍ଷ_entry {
	ସଂରଚନା ବୃକ୍ଷ_content *ବୃକ୍ଷ;
	ସଂରଚନା atom_str *name;
	ସଂରଚନା ବୃକ୍ଷ_entry_ms {
		uint16_t mode;
		ସଂରଚନା object_id oid;
	} versions[2];
};

ସଂରଚନା ବୃକ୍ଷ_content {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା entry_capacity; /* must match avail_tree_content */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା entry_ଗଣନା;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta_depth;
	ସଂରଚନା ବୃକ୍ଷ_entry *entries[FLEX_ARRAY]; /* more */
};

ସଂରଚନା avail_ବୃକ୍ଷ_content {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା entry_capacity; /* must match tree_content */
	ସଂରଚନା avail_ବୃକ୍ଷ_content *next_avail;
};

ସଂରଚନା branch {
	ସଂରଚନା branch *table_next_branch;
	ସଂରଚନା branch *active_next_branch;
	ଅଚଳ ଅକ୍ଷର *name;
	ସଂରଚନା ବୃକ୍ଷ_entry branch_ବୃକ୍ଷ;
	uintmax_t last_commit;
	uintmax_t ଅଙ୍କ_notes;
	unsigned active : 1;
	unsigned delete : 1;
	unsigned pack_id : PACK_ID_BITS;
	ସଂରଚନା object_id oid;
};

ସଂରଚନା tag {
	ସଂରଚନା tag *next_tag;
	ଅଚଳ ଅକ୍ଷର *name;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pack_id;
	ସଂରଚନା object_id oid;
};

ସଂରଚନା hash_list {
	ସଂରଚନା hash_list *next;
	ସଂରଚନା object_id oid;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

ସଂରଚନା recent_command {
	ସଂରଚନା recent_command *prev;
	ସଂରଚନା recent_command *next;
	ଅକ୍ଷର *buf;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*mark_set_inserter_t)(ସଂରଚନା mark_set **s, ସଂରଚନା object_id *oid, uintmax_t mark);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*each_mark_fn_t)(uintmax_t mark, ଶୂନ୍ୟ *obj, ଶୂନ୍ୟ *cbp);

/* Configured limits on output */
ସ୍ଥିର unsigned long max_depth = 50;
ସ୍ଥିର off_t max_packsize;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା unpack_limit = 100;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_update;

/* Stats and misc. counters */
ସ୍ଥିର uintmax_t alloc_ଗଣନା;
ସ୍ଥିର uintmax_t marks_set_ଗଣନା;
ସ୍ଥିର uintmax_t object_ଗଣନା_by_type[1 << TYPE_BITS];
ସ୍ଥିର uintmax_t duplicate_ଗଣନା_by_type[1 << TYPE_BITS];
ସ୍ଥିର uintmax_t delta_ଗଣନା_by_type[1 << TYPE_BITS];
ସ୍ଥିର uintmax_t delta_ଗଣନା_attempts_by_type[1 << TYPE_BITS];
ସ୍ଥିର unsigned long object_ଗଣନା;
ସ୍ଥିର unsigned long branch_ଗଣନା;
ସ୍ଥିର unsigned long branch_load_ଗଣନା;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା failure;
ସ୍ଥିର FILE *pack_edges;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_stats = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet;
ସ୍ଥିର ଗଣନା sign_mode signed_tag_mode = SIGN_VERBATIM;
ସ୍ଥିର ଗଣନା sign_mode signed_commit_mode = SIGN_VERBATIM;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *signed_commit_keyid;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *signed_tag_keyid;

/* Memory pools */
ସ୍ଥିର ସଂରଚନା mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - ଆକାର(ସଂରଚନା mp_block),
};

/* Atom management */
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା atom_table_sz = 4451;
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା atom_cnt;
ସ୍ଥିର ସଂରଚନା atom_str **atom_table;

/* The .pack file being generated */
ସ୍ଥିର ସଂରଚନା pack_idx_option pack_idx_opts;
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pack_id;
ସ୍ଥିର ସଂରଚନା hashfile *pack_file;
ସ୍ଥିର ସଂରଚନା packed_git *pack_data;
ସ୍ଥିର ସଂରଚନା packed_git **all_packs;
ସ୍ଥିର off_t pack_size;

/* Table of objects we've written. */
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା object_entry_alloc = 5000;
ସ୍ଥିର ସଂରଚନା object_entry_pool *blocks;
ସ୍ଥିର ସଂରଚନା hashmap object_table;
ସ୍ଥିର ସଂରଚନା mark_set *marks;
ସ୍ଥିର ଅକ୍ଷର *export_marks_file;
ସ୍ଥିର ଅକ୍ଷର *import_marks_file;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା import_marks_file_from_stream;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା import_marks_file_ignore_missing;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା import_marks_file_done;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା relative_marks_paths;

/* Our last blob */
ସ୍ଥିର ସଂରଚନା last_object last_blob = {
	.data = STRBUF_INIT,
 };

/* Tree management */
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବୃକ୍ଷ_entry_alloc = 1000;
ସ୍ଥିର ଶୂନ୍ୟ *avail_ବୃକ୍ଷ_entry;
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା avail_ବୃକ୍ଷ_table_sz = 100;
ସ୍ଥିର ସଂରଚନା avail_ବୃକ୍ଷ_content **avail_ବୃକ୍ଷ_table;
ସ୍ଥିର size_t ବୃକ୍ଷ_entry_allocd;
ସ୍ଥିର ସଂରଚନା strbuf old_ବୃକ୍ଷ = STRBUF_INIT;
ସ୍ଥିର ସଂରଚନା strbuf new_ବୃକ୍ଷ = STRBUF_INIT;

/* Branch data */
ସ୍ଥିର unsigned long max_active_branches = 5;
ସ୍ଥିର unsigned long cur_active_branches;
ସ୍ଥିର unsigned long branch_table_sz = 1039;
ସ୍ଥିର ସଂରଚନା branch **branch_table;
ସ୍ଥିର ସଂରଚନା branch *active_branches;

/* Tag data */
ସ୍ଥିର ସଂରଚନା tag *first_tag;
ସ୍ଥିର ସଂରଚନା tag *last_tag;

/* Input stream parsing */
ସ୍ଥିର whenspec_type whenspec = WHENSPEC_RAW;
ସ୍ଥିର ସଂରଚନା strbuf command_buf = STRBUF_INIT;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା unread_command_buf;
ସ୍ଥିର ସଂରଚନା recent_command cmd_hist = {
	.prev = &cmd_hist,
	.next = &cmd_hist,
};
ସ୍ଥିର ସଂରଚନା recent_command *cmd_tail = &cmd_hist;
ସ୍ଥିର ସଂରଚନା recent_command *rc_free;
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_save = 100;
ସ୍ଥିର uintmax_t next_mark;
ସ୍ଥିର ସଂରଚନା strbuf new_data = STRBUF_INIT;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା require_explicit_termination;

/* Signal handling */
ସ୍ଥିର ଅସ୍ଥିର sig_atomic_t checkpoint_requested;

/* Submodule marks */
ସ୍ଥିର ସଂରଚନା string_list sub_marks_from = STRING_LIST_INIT_DUP;
ସ୍ଥିର ସଂରଚନା string_list sub_marks_to = STRING_LIST_INIT_DUP;
ସ୍ଥିର kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cat_blob_fd = STDOUT_FILENO;

/* Command state */
ସଂରଚନା fast_import_state {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc;
	ଅଚଳ ଅକ୍ଷର **argv;
	ଅଚଳ ଅକ୍ଷର *prefix;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା seen_data_command;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_unsafe_features;
	ସଂରଚନା option *option;
};

ସ୍ଥିର ଶୂନ୍ୟ fast_import_state_init(ସଂରଚନା fast_import_state *state,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				   ଅଚଳ ଅକ୍ଷର *prefix, ସଂରଚନା option *option)
{
	memset(state, 0, ଆକାର(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

ସ୍ଥିର ଶୂନ୍ୟ parse_argv(ସଂରଚନା fast_import_state *state);
ସ୍ଥିର ଶୂନ୍ୟ parse_get_mark(ସଂରଚନା fast_import_state *state, ଅଚଳ ଅକ୍ଷର *p);
ସ୍ଥିର ଶୂନ୍ୟ parse_cat_blob(ସଂରଚନା fast_import_state *state, ଅଚଳ ଅକ୍ଷର *p);
ସ୍ଥିର ଶୂନ୍ୟ parse_ls(ସଂରଚନା fast_import_state *state,
		     ଅଚଳ ଅକ୍ଷର *p,
		     ସଂରଚନା branch *b);

ସ୍ଥିର ଶୂନ୍ୟ for_each_mark(ସଂରଚନା mark_set *m, uintmax_t base, each_mark_fn_t callback, ଶୂନ୍ୟ *p)
{
	uintmax_t k;
	ଯଦି (m->shift) {
		ପାଇଁ (k = 0; k < 1024; k++) {
			ଯଦି (m->data.sets[k])
				for_each_mark(m->data.sets[k], base + (k << m->shift), callback, p);
		}
	} ଅନ୍ୟଥା {
		ପାଇଁ (k = 0; k < 1024; k++) {
			ଯଦି (m->data.marked[k])
				callback(base + k, m->data.marked[k], p);
		}
	}
}

ସ୍ଥିର ଶୂନ୍ୟ dump_marks_fn(uintmax_t mark, ଶୂନ୍ୟ *object, ଶୂନ୍ୟ *cbp) {
	ସଂରଚନା object_entry *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

ସ୍ଥିର ଶୂନ୍ୟ write_branch_report(FILE *rpt, ସଂରଚନା branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	ଯଦି (b->active)
		fputs(" active", rpt);
	ଯଦି (b->branch_ବୃକ୍ଷ.ବୃକ୍ଷ)
		fputs(" loaded", rpt);
	ଯଦି (is_null_oid(&b->branch_ବୃକ୍ଷ.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_to_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_to_hex(&b->branch_ବୃକ୍ଷ.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_to_hex(&b->branch_ବୃକ୍ଷ.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->last_commit);

	fputs("  last pack   : ", rpt);
	ଯଦି (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

ସ୍ଥିର ଶୂନ୍ୟ write_crash_report(ଅଚଳ ଅକ୍ଷର *err)
{
	ଅକ୍ଷର *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	ସଂରଚନା branch *b;
	unsigned long lu;
	ସଂରଚନା recent_command *rc;

	ଯଦି (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		ମୁକ୍ତ(loc);
		ଫେରନ୍ତୁ;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(time(ଖାଲି), 0, DATE_MODE(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	ପାଇଁ (rc = cmd_hist.next; rc != &cmd_hist; rc = rc->next) {
		ଯଦି (rc->next == &cmd_hist)
			fputs("* ", rpt);
		ଅନ୍ୟଥା
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
	ପାଇଁ (b = active_branches, lu = 0; b; b = b->active_next_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	ପାଇଁ (lu = 0; lu < branch_table_sz; lu++) {
		ପାଇଁ (b = branch_table[lu]; b; b = b->table_next_branch)
			write_branch_report(rpt, b);
	}

	ଯଦି (first_tag) {
		ସଂରଚନା tag *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		ପାଇଁ (tg = first_tag; tg; tg = tg->next_tag) {
			fputs(oid_to_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	ଯଦି (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	ଅନ୍ୟଥା
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	ମୁକ୍ତ(loc);
}

ସ୍ଥିର ଶୂନ୍ୟ end_packfile(ଶୂନ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ unkeep_all_packs(ଶୂନ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ dump_marks(ଶୂନ୍ୟ);

ସ୍ଥିର NORETURN ଶୂନ୍ୟ die_nicely(ଅଚଳ ଅକ୍ଷର *err, va_list params)
{
	va_list cp;
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	ଯଦି (!zombie) {
		ଅକ୍ଷର message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, ଆକାର(message), err, cp);
		write_crash_report(message);
		end_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	exit(128);
}

#ifndef SIGUSR1	/* Windows, for example */

ସ୍ଥିର ଶୂନ୍ୟ set_checkpoint_signal(ଶୂନ୍ୟ)
{
}

#ଅନ୍ୟଥା

ସ୍ଥିର ଶୂନ୍ୟ checkpoint_signal(ପୂର୍ଣ୍ଣସଂଖ୍ୟା signo UNUSED)
{
	checkpoint_requested = 1;
}

ସ୍ଥିର ଶୂନ୍ୟ set_checkpoint_signal(ଶୂନ୍ୟ)
{
	ସଂରଚନା sigaction sa;

	memset(&sa, 0, ଆକାର(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, ଖାଲି);
}

#endif

ସ୍ଥିର ଶୂନ୍ୟ alloc_objects(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnt)
{
	ସଂରଚନା object_entry_pool *b;

	b = xmalloc(ଆକାର(ସଂରଚନା object_entry_pool)
		+ cnt * ଆକାର(ସଂରଚନା object_entry));
	b->next_pool = blocks;
	b->next_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_ଗଣନା += cnt;
}

ସ୍ଥିର ସଂରଚନା object_entry *new_object(ସଂରଚନା object_id *oid)
{
	ସଂରଚନା object_entry *e;

	ଯଦି (blocks->next_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->next_free++;
	oidcpy(&e->idx.oid, oid);
	ଫେରନ୍ତୁ e;
}

ସ୍ଥିର ସଂରଚନା object_entry *find_object(ସଂରଚନା object_id *oid)
{
	ଫେରନ୍ତୁ hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   ସଂରଚନା object_entry, ent);
}

ସ୍ଥିର ସଂରଚନା object_entry *ଭର୍ତ୍ତି_object(ସଂରଚନା object_id *oid)
{
	ସଂରଚନା object_entry *e;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					ସଂରଚନା object_entry, ent);
	ଯଦି (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	ଫେରନ୍ତୁ e;
}

ସ୍ଥିର ଶୂନ୍ୟ invalidate_pack_id(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା id)
{
	unsigned long lu;
	ସଂରଚନା tag *t;
	ସଂରଚନା hashmap_iter iter;
	ସଂରଚନା object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		ଯଦି (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	ପାଇଁ (lu = 0; lu < branch_table_sz; lu++) {
		ସଂରଚନା branch *b;

		ପାଇଁ (b = branch_table[lu]; b; b = b->table_next_branch)
			ଯଦି (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	ପାଇଁ (t = first_tag; t; t = t->next_tag)
		ଯଦି (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hc_str(ଅଚଳ ଅକ୍ଷର *s, size_t len)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = 0;
	ଯେତେବେଳେ (len-- > 0)
		r = r * 31 + *s++;
	ଫେରନ୍ତୁ r;
}

ସ୍ଥିର ଶୂନ୍ୟ ଭର୍ତ୍ତି_mark(ସଂରଚନା mark_set **ଶୀର୍ଷ, uintmax_t idnum, ସଂରଚନା object_entry *oe)
{
	ସଂରଚନା mark_set *s = *ଶୀର୍ଷ;

	ଯେତେବେଳେ ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, ଆକାର(ସଂରଚନା mark_set));
		s->shift = (*ଶୀର୍ଷ)->shift + 10;
		s->data.sets[0] = *ଶୀର୍ଷ;
		*ଶୀର୍ଷ = s;
	}
	ଯେତେବେଳେ (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		ଯଦି (!s->data.sets[i]) {
			s->data.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, ଆକାର(ସଂରଚନା mark_set));
			s->data.sets[i]->shift = s->shift - 10;
		}
		s = s->data.sets[i];
	}
	ଯଦି (!s->data.marked[idnum])
		marks_set_ଗଣନା++;
	s->data.marked[idnum] = oe;
}

ସ୍ଥିର ଶୂନ୍ୟ *find_mark(ସଂରଚନା mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	ସଂରଚନା object_entry *oe = ଖାଲି;
	ଯଦି ((idnum >> s->shift) < 1024) {
		ଯେତେବେଳେ (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->data.sets[i];
		}
		ଯଦି (s)
			oe = s->data.marked[idnum];
	}
	ଯଦି (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	ଫେରନ୍ତୁ oe;
}

ସ୍ଥିର ସଂରଚନା atom_str *to_atom(ଅଚଳ ଅକ୍ଷର *s, unsigned short len)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hc = hc_str(s, len) % atom_table_sz;
	ସଂରଚନା atom_str *c;

	ପାଇଁ (c = atom_table[hc]; c; c = c->next_atom)
		ଯଦି (c->str_len == len && !strncmp(s, c->str_dat, len))
			ଫେରନ୍ତୁ c;

	c = mem_pool_alloc(&fi_mem_pool, ଆକାର(ସଂରଚନା atom_str) + len + 1);
	c->str_len = len;
	memcpy(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->next_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	ଫେରନ୍ତୁ c;
}

ସ୍ଥିର ସଂରଚନା branch *lookup_branch(ଅଚଳ ଅକ୍ଷର *name)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hc = hc_str(name, strlen(name)) % branch_table_sz;
	ସଂରଚନା branch *b;

	ପାଇଁ (b = branch_table[hc]; b; b = b->table_next_branch)
		ଯଦି (!strcmp(name, b->name))
			ଫେରନ୍ତୁ b;
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ସଂରଚନା branch *new_branch(ଅଚଳ ଅକ୍ଷର *name)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hc = hc_str(name, strlen(name)) % branch_table_sz;
	ସଂରଚନା branch *b = lookup_branch(name);

	ଯଦି (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	ଯଦି (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, ଆକାର(ସଂରଚନା branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_next_branch = branch_table[hc];
	b->branch_ବୃକ୍ଷ.versions[0].mode = S_IFDIR;
	b->branch_ବୃକ୍ଷ.versions[1].mode = S_IFDIR;
	b->ଅଙ୍କ_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_ଗଣନା++;
	ଫେରନ୍ତୁ b;
}

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hc_entries(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	ଫେରନ୍ତୁ cnt < avail_ବୃକ୍ଷ_table_sz ? cnt : avail_ବୃକ୍ଷ_table_sz - 1;
}

ସ୍ଥିର ସଂରଚନା ବୃକ୍ଷ_content *new_ବୃକ୍ଷ_content(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnt)
{
	ସଂରଚନା avail_ବୃକ୍ଷ_content *f, *l = ଖାଲି;
	ସଂରଚନା ବୃକ୍ଷ_content *t;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hc = hc_entries(cnt);

	ପାଇଁ (f = avail_ବୃକ୍ଷ_table[hc]; f; l = f, f = f->next_avail)
		ଯଦି (f->entry_capacity >= cnt)
			ଭାଙ୍ଗ;

	ଯଦି (f) {
		ଯଦି (l)
			l->next_avail = f->next_avail;
		ଅନ୍ୟଥା
			avail_ବୃକ୍ଷ_table[hc] = f->next_avail;
	} ଅନ୍ୟଥା {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, ଆକାର(*t) + ଆକାର(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (ସଂରଚନା ବୃକ୍ଷ_content*)f;
	t->entry_ଗଣନା = 0;
	t->delta_depth = 0;
	ଫେରନ୍ତୁ t;
}

ସ୍ଥିର ଶୂନ୍ୟ release_ବୃକ୍ଷ_entry(ସଂରଚନା ବୃକ୍ଷ_entry *e);
ସ୍ଥିର ଶୂନ୍ୟ release_ବୃକ୍ଷ_content(ସଂରଚନା ବୃକ୍ଷ_content *t)
{
	ସଂରଚନା avail_ବୃକ୍ଷ_content *f = (ସଂରଚନା avail_ବୃକ୍ଷ_content*)t;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hc = hc_entries(f->entry_capacity);
	f->next_avail = avail_ବୃକ୍ଷ_table[hc];
	avail_ବୃକ୍ଷ_table[hc] = f;
}

ସ୍ଥିର ଶୂନ୍ୟ release_ବୃକ୍ଷ_content_recursive(ସଂରଚନା ବୃକ୍ଷ_content *t)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପାଇଁ (i = 0; i < t->entry_ଗଣନା; i++)
		release_ବୃକ୍ଷ_entry(t->entries[i]);
	release_ବୃକ୍ଷ_content(t);
}

ସ୍ଥିର ସଂରଚନା ବୃକ୍ଷ_content *grow_ବୃକ୍ଷ_content(
	ସଂରଚନା ବୃକ୍ଷ_content *t,
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା amt)
{
	ସଂରଚନା ବୃକ୍ଷ_content *r = new_ବୃକ୍ଷ_content(t->entry_ଗଣନା + amt);
	r->entry_ଗଣନା = t->entry_ଗଣନା;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_ଗଣନା);
	release_ବୃକ୍ଷ_content(t);
	ଫେରନ୍ତୁ r;
}

ସ୍ଥିର ସଂରଚନା ବୃକ୍ଷ_entry *new_ବୃକ୍ଷ_entry(ଶୂନ୍ୟ)
{
	ସଂରଚନା ବୃକ୍ଷ_entry *e;

	ଯଦି (!avail_ବୃକ୍ଷ_entry) {
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = ବୃକ୍ଷ_entry_alloc;
		ବୃକ୍ଷ_entry_allocd += n * ଆକାର(ସଂରଚନା ବୃକ୍ଷ_entry);
		ALLOC_ARRAY(e, n);
		avail_ବୃକ୍ଷ_entry = e;
		ଯେତେବେଳେ (n-- > 1) {
			*((ଶୂନ୍ୟ**)e) = e + 1;
			e++;
		}
		*((ଶୂନ୍ୟ**)e) = ଖାଲି;
	}

	e = avail_ବୃକ୍ଷ_entry;
	avail_ବୃକ୍ଷ_entry = *((ଶୂନ୍ୟ**)e);
	ଫେରନ୍ତୁ e;
}

ସ୍ଥିର ଶୂନ୍ୟ release_ବୃକ୍ଷ_entry(ସଂରଚନା ବୃକ୍ଷ_entry *e)
{
	ଯଦି (e->ବୃକ୍ଷ)
		release_ବୃକ୍ଷ_content_recursive(e->ବୃକ୍ଷ);
	*((ଶୂନ୍ୟ**)e) = avail_ବୃକ୍ଷ_entry;
	avail_ବୃକ୍ଷ_entry = e;
}

ସ୍ଥିର ସଂରଚନା ବୃକ୍ଷ_content *dup_ବୃକ୍ଷ_content(ସଂରଚନା ବୃକ୍ଷ_content *s)
{
	ସଂରଚନା ବୃକ୍ଷ_content *d;
	ସଂରଚନା ବୃକ୍ଷ_entry *a, *b;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (!s)
		ଫେରନ୍ତୁ ଖାଲି;
	d = new_ବୃକ୍ଷ_content(s->entry_ଗଣନା);
	ପାଇଁ (i = 0; i < s->entry_ଗଣନା; i++) {
		a = s->entries[i];
		b = new_ବୃକ୍ଷ_entry();
		memcpy(b, a, ଆକାର(*a));
		ଯଦି (a->ବୃକ୍ଷ && is_null_oid(&b->versions[1].oid))
			b->ବୃକ୍ଷ = dup_ବୃକ୍ଷ_content(a->ବୃକ୍ଷ);
		ଅନ୍ୟଥା
			b->ବୃକ୍ଷ = ଖାଲି;
		d->entries[i] = b;
	}
	d->entry_ଗଣନା = s->entry_ଗଣନା;
	d->delta_depth = s->delta_depth;

	ଫେରନ୍ତୁ d;
}

ସ୍ଥିର ଶୂନ୍ୟ start_packfile(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf tmp_file = STRBUF_INIT;
	ସଂରଚନା packed_git *p;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pack_fd;

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
	object_ଗଣନା = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *create_index(ଶୂନ୍ୟ)
{
	ଅଚଳ ଅକ୍ଷର *tmpfile;
	ସଂରଚନା pack_idx_entry **idx, **c, **last;
	ସଂରଚନା object_entry *e;
	ସଂରଚନା object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_ଗଣନା);
	c = idx;
	ପାଇଁ (o = blocks; o; o = o->next_pool)
		ପାଇଁ (e = o->next_free; e-- != o->entries;)
			ଯଦି (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_ଗଣନା;
	ଯଦି (c != last)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, ଖାଲି, idx, object_ଗଣନା,
				 &pack_idx_opts, pack_data->hash);
	ମୁକ୍ତ(idx);
	ଫେରନ୍ତୁ tmpfile;
}

ସ୍ଥିର ଅକ୍ଷର *keep_pack(ଅଚଳ ଅକ୍ଷର *curr_index_name)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *keep_ସନ୍ଦେଶ = "fast-import";
	ସଂରଚନା strbuf name = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_fd;

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_data->repo,
							    name.buf);
	ଯଦି (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_ସନ୍ଦେଶ, strlen(keep_ସନ୍ଦେଶ));
	ଯଦି (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "pack");
	ଯଦି (finalize_object_file(pack_data->repo, pack_data->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "idx");
	ଯଦି (finalize_object_file(pack_data->repo, curr_index_name, name.buf))
		die(_("cannot store index file"));
	ମୁକ୍ତ((ଶୂନ୍ୟ *)curr_index_name);
	ଫେରନ୍ତୁ strbuf_detach(&name, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ unkeep_all_packs(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf name = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା k;

	ପାଇଁ (k = 0; k < pack_id; k++) {
		ସଂରଚନା packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା loosen_small_pack(ଅଚଳ ସଂରଚନା packed_git *p)
{
	ସଂରଚନା child_process unpack = CHILD_PROCESS_INIT;

	ଯଦି (lseek(p->pack_fd, 0, SEEK_SET) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_ଠେଲ(&unpack.args, "unpack-objects");
	ଯଦି (!show_stats)
		strvec_ଠେଲ(&unpack.args, "-q");

	ଫେରନ୍ତୁ run_command(&unpack);
}

ସ୍ଥିର ଶୂନ୍ୟ end_packfile(ଶୂନ୍ୟ)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା running;

	ଯଦି (running || !pack_data)
		ଫେରନ୍ତୁ;

	running = 1;
	clear_delta_base_cache();
	ଯଦି (object_ଗଣନା) {
		ସଂରଚନା odb_source_files *files = odb_source_files_downcast(pack_data->repo->objects->source);
		ସଂରଚନା packed_git *new_p;
		ସଂରଚନା object_id cur_pack_oid;
		ଅକ୍ଷର *idx_name;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
		ସଂରଚନା branch *b;
		ସଂରଚନା tag *t;

		close_pack_windows(pack_data);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_data->pack_fd,
					 pack_data->hash, pack_data->pack_name,
					 object_ଗଣନା, cur_pack_oid.hash,
					 pack_size);

		ଯଦି (object_ଗଣନା <= unpack_limit) {
			ଯଦି (!loosen_small_pack(pack_data)) {
				invalidate_pack_id(pack_id);
				ଯାଅ discard_pack;
			}
		}

		close(pack_data->pack_fd);
		idx_name = keep_pack(create_index());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		ଯଦି (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		ମୁକ୍ତ(idx_name);

		/* Print the boundary */
		ଯଦି (pack_edges) {
			fprintf(pack_edges, "%s:", new_p->pack_name);
			ପାଇଁ (i = 0; i < branch_table_sz; i++) {
				ପାଇଁ (b = branch_table[i]; b; b = b->table_next_branch) {
					ଯଦି (b->pack_id == pack_id)
						fprintf(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			ପାଇଁ (t = first_tag; t; t = t->next_tag) {
				ଯଦି (t->pack_id == pack_id)
					fprintf(pack_edges, " %s",
						oid_to_hex(&t->oid));
			}
			fputc('\n', pack_edges);
			fflush(pack_edges);
		}

		pack_id++;
	}
	ଅନ୍ୟଥା {
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

ସ୍ଥିର ଶୂନ୍ୟ cycle_packfile(ଶୂନ୍ୟ)
{
	end_packfile();
	start_packfile();
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା store_object(
	ଗଣନା object_type type,
	ସଂରଚନା strbuf *dat,
	ସଂରଚନା last_object *last,
	ସଂରଚନା object_id *oidout,
	uintmax_t mark)
{
	ସଂରଚନା odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	ଶୂନ୍ୟ *out, *delta;
	ସଂରଚନା object_entry *e;
	unsigned ଅକ୍ଷର hdr[96];
	ସଂରଚନା object_id oid;
	unsigned long hdrlen, deltalen = 0;
	ସଂରଚନା git_hash_ctx c;
	git_zstream s;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	hdrlen = format_object_header((ଅକ୍ଷର *)hdr, ଆକାର(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	ଯଦି (oidout)
		oidcpy(oidout, &oid);

	e = ଭର୍ତ୍ତି_object(&oid);
	ଯଦି (mark)
		ଭର୍ତ୍ତି_mark(&marks, mark, e);
	ଯଦି (e->idx.offset) {
		duplicate_ଗଣନା_by_type[type]++;
		ଫେରନ୍ତୁ 1;
	}

	ପାଇଁ (ସଂରଚନା odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ଯଦି (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			ଜାରି;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_ଗଣନା_by_type[type]++;
		ଫେରନ୍ତୁ 1;
	}

	ଯଦି (last && last->data.len && last->data.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		size_t deltalen_st;

		delta_ଗଣନା_attempts_by_type[type]++;
		delta = diff_delta(last->data.buf, last->data.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_size_t_to_ulong(deltalen_st);
	} ଅନ୍ୟଥା
		delta = ଖାଲି;

	git_deflate_init(&s, cfg->pack_compression_level);
	ଯଦି (delta) {
		s.next_in = delta;
		s.avail_in = deltalen;
	} ଅନ୍ୟଥା {
		s.next_in = (ଶୂନ୍ୟ *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.next_out = out = xmalloc(s.avail_out);
	ଯେତେବେଳେ (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	ଯଦି ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + s.total_out) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + s.total_out) < pack_size) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		ଯଦି (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.next_in = (ଶୂନ୍ୟ *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.next_out = out = xrealloc(out, s.avail_out);
			ଯେତେବେଳେ (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_end(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.offset = pack_size;
	object_ଗଣନା++;
	object_ଗଣନା_by_type[type]++;

	crc32_begin(pack_file);

	ଯଦି (delta) {
		off_t ofs = e->idx.offset - last->offset;
		unsigned pos = ଆକାର(hdr) - 1;

		delta_ଗଣନା_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, ଆକାର(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;

		hdr[pos] = ofs & 127;
		ଯେତେବେଳେ (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, ଆକାର(hdr) - pos);
		pack_size += ଆକାର(hdr) - pos;
	} ଅନ୍ୟଥା {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, ଆକାର(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;
	}

	hashwrite(pack_file, out, s.total_out);
	pack_size += s.total_out;

	e->idx.crc32 = crc32_end(pack_file);

	ମୁକ୍ତ(out);
	ମୁକ୍ତ(delta);
	ଯଦି (last) {
		ଯଦି (last->no_swap) {
			last->data = *dat;
		} ଅନ୍ୟଥା {
			strbuf_swap(&last->data, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ truncate_pack(ସଂରଚନା hashfile_checkpoint *checkpoint)
{
	ଯଦି (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_size = checkpoint->offset;
}

ସ୍ଥିର ଶୂନ୍ୟ stream_blob(uintmax_t len, ସଂରଚନା object_id *oidout, uintmax_t mark)
{
	ସଂରଚନା odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	size_t in_sz = 64 * 1024, out_sz = 64 * 1024;
	unsigned ଅକ୍ଷର *in_buf = xmalloc(in_sz);
	unsigned ଅକ୍ଷର *out_buf = xmalloc(out_sz);
	ସଂରଚନା object_entry *e;
	ସଂରଚନା object_id oid;
	unsigned long hdrlen;
	off_t offset;
	ସଂରଚନା git_hash_ctx c;
	git_zstream s;
	ସଂରଚନା hashfile_checkpoint checkpoint;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	ଯଦି ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + len) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + len) < pack_size)
		cycle_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	offset = checkpoint.offset;

	hdrlen = format_object_header((ଅକ୍ଷର *)out_buf, out_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, out_buf, hdrlen);

	crc32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_in_pack_object_header(out_buf, out_sz, OBJ_BLOB, len);

	s.next_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	ଯେତେବେଳେ (status != Z_STREAM_END) {
		ଯଦି (0 < len && !s.avail_in) {
			size_t cnt = in_sz < len ? in_sz : (size_t)len;
			size_t n = fread(in_buf, 1, cnt, stdin);
			ଯଦି (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.next_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		ଯଦି (!s.avail_out || status == Z_STREAM_END) {
			size_t n = s.next_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_size += n;
			s.next_out = out_buf;
			s.avail_out = out_sz;
		}

		ବାଛ (status) {
		ସ୍ଥିତି Z_OK:
		ସ୍ଥିତି Z_BUF_ERROR:
		ସ୍ଥିତି Z_STREAM_END:
			ଜାରି;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			die(_("unexpected deflate failure: %d"), status);
		}
	}
	git_deflate_end(&s);
	git_hash_final_oid(&oid, &c);

	ଯଦି (oidout)
		oidcpy(oidout, &oid);

	e = ଭର୍ତ୍ତି_object(&oid);

	ଯଦି (mark)
		ଭର୍ତ୍ତି_mark(&marks, mark, e);

	ଯଦି (e->idx.offset) {
		duplicate_ଗଣନା_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		ଯାଅ out;
	}

	ପାଇଁ (ସଂରଚନା odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ଯଦି (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			ଜାରି;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_ଗଣନା_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		ଯାଅ out;
	}

	e->depth = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.offset = offset;
	e->idx.crc32 = crc32_end(pack_file);
	object_ଗଣନା++;
	object_ଗଣନା_by_type[OBJ_BLOB]++;

out:
	ମୁକ୍ତ(in_buf);
	ମୁକ୍ତ(out_buf);
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
ସ୍ଥିର ଶୂନ୍ୟ *gfi_unpack_entry(
	ସଂରଚନା object_entry *oe,
	unsigned long *sizep)
{
	ଗଣନା object_type type;
	size_t size_st = 0;
	ଶୂନ୍ୟ *data;
	ସଂରଚନା packed_git *p = all_packs[oe->pack_id];
	ଯଦି (p == pack_data && p->pack_size < (pack_size + the_hash_algo->rawsz)) {
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
	ଯଦି (sizep)
		*sizep = cast_size_t_to_ulong(size_st);
	ଫେରନ୍ତୁ data;
}

ସ୍ଥିର ଶୂନ୍ୟ load_ବୃକ୍ଷ(ସଂରଚନା ବୃକ୍ଷ_entry *ମୂଳ)
{
	ସଂରଚନା object_id *oid = &ମୂଳ->versions[1].oid;
	ସଂରଚନା object_entry *myoe;
	ସଂରଚନା ବୃକ୍ଷ_content *t;
	unsigned long size;
	ଅକ୍ଷର *buf;
	ଅଚଳ ଅକ୍ଷର *c;

	ମୂଳ->ବୃକ୍ଷ = t = new_ବୃକ୍ଷ_content(8);
	ଯଦି (is_null_oid(oid))
		ଫେରନ୍ତୁ;

	myoe = find_object(oid);
	ଯଦି (myoe && myoe->pack_id != MAX_PACK_ID) {
		ଯଦି (myoe->type != OBJ_ବୃକ୍ଷ)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &size);
		ଯଦି (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} ଅନ୍ୟଥା {
		ଗଣନା object_type type;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		ଯଦି (!buf || type != OBJ_ବୃକ୍ଷ)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	ଯେତେବେଳେ (c != (buf + size)) {
		ସଂରଚନା ବୃକ୍ଷ_entry *e = new_ବୃକ୍ଷ_entry();

		ଯଦି (t->entry_ଗଣନା == t->entry_capacity)
			ମୂଳ->ବୃକ୍ଷ = t = grow_ବୃକ୍ଷ_content(t, t->entry_ଗଣନା);
		t->entries[t->entry_ଗଣନା++] = e;

		e->ବୃକ୍ଷ = ଖାଲି;
		c = parse_mode(c, &e->versions[1].mode);
		ଯଦି (!c)
			die(_("corrupt mode in %s"), oid_to_hex(oid));
		e->versions[0].mode = e->versions[1].mode;
		e->name = to_atom(c, strlen(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (unsigned ଅକ୍ଷର *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (unsigned ଅକ୍ଷର *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	ମୁକ୍ତ(buf);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tecmp0 (ଅଚଳ ଶୂନ୍ୟ *_a, ଅଚଳ ଶୂନ୍ୟ *_b)
{
	ସଂରଚନା ବୃକ୍ଷ_entry *a = *((ସଂରଚନା ବୃକ୍ଷ_entry**)_a);
	ସଂରଚନା ବୃକ୍ଷ_entry *b = *((ସଂରଚନା ବୃକ୍ଷ_entry**)_b);
	ଫେରନ୍ତୁ base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tecmp1 (ଅଚଳ ଶୂନ୍ୟ *_a, ଅଚଳ ଶୂନ୍ୟ *_b)
{
	ସଂରଚନା ବୃକ୍ଷ_entry *a = *((ସଂରଚନା ବୃକ୍ଷ_entry**)_a);
	ସଂରଚନା ବୃକ୍ଷ_entry *b = *((ସଂରଚନା ବୃକ୍ଷ_entry**)_b);
	ଫେରନ୍ତୁ base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

ସ୍ଥିର ଶୂନ୍ୟ mktree(ସଂରଚନା ବୃକ୍ଷ_content *t, ପୂର୍ଣ୍ଣସଂଖ୍ୟା v, ସଂରଚନା strbuf *b)
{
	size_t maxlen = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (!v)
		QSORT(t->entries, t->entry_ଗଣନା, tecmp0);
	ଅନ୍ୟଥା
		QSORT(t->entries, t->entry_ଗଣନା, tecmp1);

	ପାଇଁ (i = 0; i < t->entry_ଗଣନା; i++) {
		ଯଦି (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	ପାଇଁ (i = 0; i < t->entry_ଗଣନା; i++) {
		ସଂରଚନା ବୃକ୍ଷ_entry *e = t->entries[i];
		ଯଦି (!e->versions[v].mode)
			ଜାରି;
		strbuf_addf(b, "%o %s%c",
			(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ store_ବୃକ୍ଷ(ସଂରଚନା ବୃକ୍ଷ_entry *ମୂଳ)
{
	ସଂରଚନା ବୃକ୍ଷ_content *t;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, del;
	ସଂରଚନା last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	ସଂରଚନା object_entry *le = ଖାଲି;

	ଯଦି (!is_null_oid(&ମୂଳ->versions[1].oid))
		ଫେରନ୍ତୁ;

	ଯଦି (!ମୂଳ->ବୃକ୍ଷ)
		load_ବୃକ୍ଷ(ମୂଳ);
	t = ମୂଳ->ବୃକ୍ଷ;

	ପାଇଁ (i = 0; i < t->entry_ଗଣନା; i++) {
		ଯଦି (t->entries[i]->ବୃକ୍ଷ)
			store_ବୃକ୍ଷ(t->entries[i]);
	}

	ଯଦି (!(ମୂଳ->versions[0].mode & NO_DELTA))
		le = find_object(&ମୂଳ->versions[0].oid);
	ଯଦି (S_ISDIR(ମୂଳ->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_ବୃକ୍ଷ);
		lo.data = old_ବୃକ୍ଷ;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_ବୃକ୍ଷ);
	store_object(OBJ_ବୃକ୍ଷ, &new_ବୃକ୍ଷ, &lo, &ମୂଳ->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	ପାଇଁ (i = 0, j = 0, del = 0; i < t->entry_ଗଣନା; i++) {
		ସଂରଚନା ବୃକ୍ଷ_entry *e = t->entries[i];
		ଯଦି (e->versions[1].mode) {
			e->versions[0].mode = e->versions[1].mode;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} ଅନ୍ୟଥା {
			release_ବୃକ୍ଷ_entry(e);
			del++;
		}
	}
	t->entry_ଗଣନା -= del;
}

ସ୍ଥିର ଶୂନ୍ୟ ବୃକ୍ଷ_content_replace(
	ସଂରଚନା ବୃକ୍ଷ_entry *ମୂଳ,
	ଅଚଳ ସଂରଚନା object_id *oid,
	ଅଚଳ uint16_t mode,
	ସଂରଚନା ବୃକ୍ଷ_content *newtree)
{
	ଯଦି (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&ମୂଳ->versions[0].oid, the_repository->hash_algo);
	oidcpy(&ମୂଳ->versions[1].oid, oid);
	ଯଦି (ମୂଳ->ବୃକ୍ଷ)
		release_ବୃକ୍ଷ_content_recursive(ମୂଳ->ବୃକ୍ଷ);
	ମୂଳ->ବୃକ୍ଷ = newtree;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବୃକ୍ଷ_content_set(
	ସଂରଚନା ବୃକ୍ଷ_entry *ମୂଳ,
	ଅଚଳ ଅକ୍ଷର *p,
	ଅଚଳ ସଂରଚନା object_id *oid,
	ଅଚଳ uint16_t mode,
	ସଂରଚନା ବୃକ୍ଷ_content *subtree)
{
	ସଂରଚନା ବୃକ୍ଷ_content *t;
	ଅଚଳ ଅକ୍ଷର *slash1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, n;
	ସଂରଚନା ବୃକ୍ଷ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	ଯଦି (!n)
		die(_("empty path component found in input"));
	ଯଦି (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	ଯଦି (!ମୂଳ->ବୃକ୍ଷ)
		load_ବୃକ୍ଷ(ମୂଳ);
	t = ମୂଳ->ବୃକ୍ଷ;
	ପାଇଁ (i = 0; i < t->entry_ଗଣନା; i++) {
		e = t->entries[i];
		ଯଦି (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ଯଦି (!*slash1) {
				ଯଦି (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					ଫେରନ୍ତୁ 0;
				e->versions[1].mode = mode;
				oidcpy(&e->versions[1].oid, oid);
				ଯଦି (e->ବୃକ୍ଷ)
					release_ବୃକ୍ଷ_content_recursive(e->ବୃକ୍ଷ);
				e->ବୃକ୍ଷ = subtree;

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
				ଯଦି (S_ISDIR(e->versions[0].mode))
					e->versions[0].mode |= NO_DELTA;

				oidclr(&ମୂଳ->versions[1].oid, the_repository->hash_algo);
				ଫେରନ୍ତୁ 1;
			}
			ଯଦି (!S_ISDIR(e->versions[1].mode)) {
				e->ବୃକ୍ଷ = new_ବୃକ୍ଷ_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			ଯଦି (!e->ବୃକ୍ଷ)
				load_ବୃକ୍ଷ(e);
			ଯଦି (ବୃକ୍ଷ_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&ମୂଳ->versions[1].oid, the_repository->hash_algo);
				ଫେରନ୍ତୁ 1;
			}
			ଫେରନ୍ତୁ 0;
		}
	}

	ଯଦି (t->entry_ଗଣନା == t->entry_capacity)
		ମୂଳ->ବୃକ୍ଷ = t = grow_ବୃକ୍ଷ_content(t, t->entry_ଗଣନା);
	e = new_ବୃକ୍ଷ_entry();
	e->name = to_atom(p, n);
	e->versions[0].mode = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->entry_ଗଣନା++] = e;
	ଯଦି (*slash1) {
		e->ବୃକ୍ଷ = new_ବୃକ୍ଷ_content(8);
		e->versions[1].mode = S_IFDIR;
		ବୃକ୍ଷ_content_set(e, slash1 + 1, oid, mode, subtree);
	} ଅନ୍ୟଥା {
		e->ବୃକ୍ଷ = subtree;
		e->versions[1].mode = mode;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&ମୂଳ->versions[1].oid, the_repository->hash_algo);
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବୃକ୍ଷ_content_remove(
	ସଂରଚନା ବୃକ୍ଷ_entry *ମୂଳ,
	ଅଚଳ ଅକ୍ଷର *p,
	ସଂରଚନା ବୃକ୍ଷ_entry *backup_leaf,
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_ମୂଳ)
{
	ସଂରଚନା ବୃକ୍ଷ_content *t;
	ଅଚଳ ଅକ୍ଷର *slash1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, n;
	ସଂରଚନା ବୃକ୍ଷ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	ଯଦି (!ମୂଳ->ବୃକ୍ଷ)
		load_ବୃକ୍ଷ(ମୂଳ);

	ଯଦି (!*p && allow_ମୂଳ) {
		e = ମୂଳ;
		ଯାଅ del_entry;
	}

	t = ମୂଳ->ବୃକ୍ଷ;
	ପାଇଁ (i = 0; i < t->entry_ଗଣନା; i++) {
		e = t->entries[i];
		ଯଦି (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ଯଦି (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				ଫେରନ୍ତୁ 1;
			ଯଦି (!*slash1 || !S_ISDIR(e->versions[1].mode))
				ଯାଅ del_entry;
			ଯଦି (!e->ବୃକ୍ଷ)
				load_ବୃକ୍ଷ(e);
			ଯଦି (ବୃକ୍ଷ_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				ପାଇଁ (n = 0; n < e->ବୃକ୍ଷ->entry_ଗଣନା; n++) {
					ଯଦି (e->ବୃକ୍ଷ->entries[n]->versions[1].mode) {
						oidclr(&ମୂଳ->versions[1].oid,
						       the_repository->hash_algo);
						ଫେରନ୍ତୁ 1;
					}
				}
				backup_leaf = ଖାଲି;
				ଯାଅ del_entry;
			}
			ଫେରନ୍ତୁ 0;
		}
	}
	ଫେରନ୍ତୁ 0;

del_entry:
	ଯଦି (backup_leaf)
		memcpy(backup_leaf, e, ଆକାର(*backup_leaf));
	ଅନ୍ୟଥା ଯଦି (e->ବୃକ୍ଷ)
		release_ବୃକ୍ଷ_content_recursive(e->ବୃକ୍ଷ);
	e->ବୃକ୍ଷ = ଖାଲି;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&ମୂଳ->versions[1].oid, the_repository->hash_algo);
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବୃକ୍ଷ_content_get(
	ସଂରଚନା ବୃକ୍ଷ_entry *ମୂଳ,
	ଅଚଳ ଅକ୍ଷର *p,
	ସଂରଚନା ବୃକ୍ଷ_entry *leaf,
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_ମୂଳ)
{
	ସଂରଚନା ବୃକ୍ଷ_content *t;
	ଅଚଳ ଅକ୍ଷର *slash1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, n;
	ସଂରଚନା ବୃକ୍ଷ_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	ଯଦି (!n && !allow_ମୂଳ)
		die(_("empty path component found in input"));

	ଯଦି (!ମୂଳ->ବୃକ୍ଷ)
		load_ବୃକ୍ଷ(ମୂଳ);

	ଯଦି (!n) {
		e = ମୂଳ;
		ଯାଅ found_entry;
	}

	t = ମୂଳ->ବୃକ୍ଷ;
	ପାଇଁ (i = 0; i < t->entry_ଗଣନା; i++) {
		e = t->entries[i];
		ଯଦି (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ଯଦି (!*slash1)
				ଯାଅ found_entry;
			ଯଦି (!S_ISDIR(e->versions[1].mode))
				ଫେରନ୍ତୁ 0;
			ଯଦି (!e->ବୃକ୍ଷ)
				load_ବୃକ୍ଷ(e);
			ଫେରନ୍ତୁ ବୃକ୍ଷ_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	ଫେରନ୍ତୁ 0;

found_entry:
	memcpy(leaf, e, ଆକାର(*leaf));
	ଯଦି (e->ବୃକ୍ଷ && is_null_oid(&e->versions[1].oid))
		leaf->ବୃକ୍ଷ = dup_ବୃକ୍ଷ_content(e->ବୃକ୍ଷ);
	ଅନ୍ୟଥା
		leaf->ବୃକ୍ଷ = ଖାଲି;
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_branch(ସଂରଚନା branch *b)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ = "fast-import";
	ସଂରଚନା ref_transaction *transaction;
	ସଂରଚନା object_id old_oid;
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *replace_prefix = "refs/replace/";

	ଯଦି (starts_with(b->name, replace_prefix) &&
	    !strcmp(b->name + strlen(replace_prefix),
		    oid_to_hex(&b->oid))) {
		ଯଦି (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				ଖାଲି, b->name, ଖାଲି, 0);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (is_null_oid(&b->oid)) {
		ଯଦି (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					ଖାଲି, b->name, ଖାଲି, 0);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	ଯଦି (!force_update && !is_null_oid(&old_oid)) {
		ସଂରଚନା commit *old_cmit, *new_cmit;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		ଯଦି (!old_cmit || !new_cmit)
			ଫେରନ୍ତୁ error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		ଯଦି (ret < 0)
			exit(128);
		ଯଦି (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			ଫେରନ୍ତୁ -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ଯଦି (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   ଖାଲି, ଖାଲି, 0, ସନ୍ଦେଶ, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		ଫେରନ୍ତୁ -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ dump_branches(ଶୂନ୍ୟ)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା branch *b;

	ପାଇଁ (i = 0; i < branch_table_sz; i++) {
		ପାଇଁ (b = branch_table[i]; b; b = b->table_next_branch)
			failure |= update_branch(b);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ dump_tags(ଶୂନ୍ୟ)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ = "fast-import";
	ସଂରଚନା tag *t;
	ସଂରଚନା strbuf ref_name = STRBUF_INIT;
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ସଂରଚନା ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ଯଦି (!transaction) {
		failure |= error("%s", err.buf);
		ଯାଅ cleanup;
	}
	ପାଇଁ (t = first_tag; t; t = t->next_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		ଯଦି (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, ଖାଲି, ଖାଲି, ଖାଲି,
					   0, ସନ୍ଦେଶ, &err)) {
			failure |= error("%s", err.buf);
			ଯାଅ cleanup;
		}
	}
	ଯଦି (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

ସ୍ଥିର ଶୂନ୍ୟ dump_marks(ଶୂନ୍ୟ)
{
	ସଂରଚନା lock_file mark_lock = LOCK_INIT;
	FILE *f;

	ଯଦି (!export_marks_file || (import_marks_file && !import_marks_file_done))
		ଫେରନ୍ତୁ;

	ଯଦି (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		ଫେରନ୍ତୁ;
	}

	ଯଦି (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		ଫେରନ୍ତୁ;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	ଯଦି (!f) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		ଫେରନ୍ତୁ;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	ଯଦି (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		ଫେରନ୍ତୁ;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ ଭର୍ତ୍ତି_object_entry(ସଂରଚନା mark_set **s, ସଂରଚନା object_id *oid, uintmax_t mark)
{
	ସଂରଚନା object_entry *e;
	e = find_object(oid);
	ଯଦି (!e) {
		ଗଣନା object_type type = odb_read_object_info(the_repository->objects,
							     oid, ଖାଲି);
		ଯଦି (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = ଭର୍ତ୍ତି_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	ଭର୍ତ୍ତି_mark(s, mark, e);
}

ସ୍ଥିର ଶୂନ୍ୟ ଭର୍ତ୍ତି_oid_entry(ସଂରଚନା mark_set **s, ସଂରଚନା object_id *oid, uintmax_t mark)
{
	ଭର୍ତ୍ତି_mark(s, mark, xmemdupz(oid, ଆକାର(*oid)));
}

ସ୍ଥିର ଶୂନ୍ୟ read_mark_file(ସଂରଚନା mark_set **s, FILE *f, mark_set_inserter_t inserter)
{
	ଅକ୍ଷର ଧାଡ଼ି[512];
	ଯେତେବେଳେ (fgets(ଧାଡ଼ି, ଆକାର(ଧାଡ଼ି), f)) {
		uintmax_t mark;
		ଅକ୍ଷର *end;
		ସଂରଚନା object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, ଆକାର(oid.hash));

		end = strchr(ଧାଡ଼ି, '\n');
		ଯଦି (ଧାଡ଼ି[0] != ':' || !end)
			die(_("corrupt mark line: %s"), ଧାଡ଼ି);
		*end = 0;
		mark = strtoumax(ଧାଡ଼ି + 1, &end, 10);
		ଯଦି (!mark || end == ଧାଡ଼ି + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), ଧାଡ଼ି);
		inserter(s, &oid, mark);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ read_marks(ଶୂନ୍ୟ)
{
	FILE *f = fopen(import_marks_file, "r");
	ଯଦି (f)
		;
	ଅନ୍ୟଥା ଯଦି (import_marks_file_ignore_missing && errno == ENOENT)
		ଯାଅ done; /* Marks file does not exist */
	ଅନ୍ୟଥା
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, ଭର୍ତ୍ତି_object_entry);
	fclose(f);
done:
	import_marks_file_done = 1;
}


ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_next_command(ସଂରଚନା fast_import_state *state)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stdin_eof = 0;

	ଯଦି (stdin_eof) {
		unread_command_buf = 0;
		ଫେରନ୍ତୁ EOF;
	}

	ପାଇଁ (;;) {
		ଯଦି (unread_command_buf) {
			unread_command_buf = 0;
		} ଅନ୍ୟଥା {
			ସଂରଚନା recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			ଯଦି (stdin_eof)
				ଫେରନ୍ତୁ EOF;

			ଯଦି (!state->seen_data_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			ଯଦି (rc)
				rc_free = rc->next;
			ଅନ୍ୟଥା {
				rc = cmd_hist.next;
				cmd_hist.next = rc->next;
				cmd_hist.next->prev = &cmd_hist;
				ମୁକ୍ତ(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_tail;
			rc->next = cmd_hist.prev;
			rc->prev->next = rc;
			cmd_tail = rc;
		}
		ଯଦି (command_buf.buf[0] == '#')
			ଜାରି;
		ଫେରନ୍ତୁ 0;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ skip_optional_lf(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା term_char = fgetc(stdin);
	ଯଦି (term_char != '\n' && term_char != EOF)
		ungetc(term_char, stdin);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_mark(ସଂରଚନା fast_import_state *state)
{
	ଅଚଳ ଅକ୍ଷର *v;
	ଯଦି (skip_prefix(command_buf.buf, "mark :", &v)) {
		next_mark = strtoumax(v, ଖାଲି, 10);
		read_next_command(state);
	}
	ଅନ୍ୟଥା
		next_mark = 0;
}

ସ୍ଥିର ଶୂନ୍ୟ parse_original_identifier(ସଂରଚନା fast_import_state *state)
{
	ଅଚଳ ଅକ୍ଷର *v;
	ଯଦି (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_next_command(state);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_data(ସଂରଚନା strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	ଅଚଳ ଅକ୍ଷର *data;
	strbuf_reset(sb);

	ଯଦି (!skip_prefix(command_buf.buf, "data ", &data))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	ଯଦି (skip_prefix(data, "<<", &data)) {
		ଅକ୍ଷର *term = xstrdup(data);
		size_t term_len = command_buf.len - (data - command_buf.buf);

		ପାଇଁ (;;) {
			ଯଦି (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), term);
			ଯଦି (term_len == command_buf.len
				&& !strcmp(term, command_buf.buf))
				ଭାଙ୍ଗ;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		ମୁକ୍ତ(term);
	}
	ଅନ୍ୟଥା {
		uintmax_t len = strtoumax(data, ଖାଲି, 10);
		size_t n = 0, length = (size_t)len;

		ଯଦି (limit && limit < len) {
			*len_res = len;
			ଫେରନ୍ତୁ 0;
		}
		ଯଦି (length < len)
			die(_("data is too large to use in this context"));

		ଯେତେବେଳେ (n < length) {
			size_t s = strbuf_fread(sb, length - n, stdin);
			ଯଦି (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(unsigned long)(length - n));
			n += s;
		}
	}

	skip_optional_lf();
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା validate_raw_date(ଅଚଳ ଅକ୍ଷର *src, ସଂରଚନା strbuf *result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା strict)
{
	ଅଚଳ ଅକ୍ଷର *orig_src = src;
	ଅକ୍ଷର *endp;
	unsigned long ଅଙ୍କ;

	errno = 0;

	ଅଙ୍କ = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	ଯଦି (errno || endp == src || *endp != ' ')
		ଫେରନ୍ତୁ -1;

	src = endp + 1;
	ଯଦି (*src != '-' && *src != '+')
		ଫେରନ୍ତୁ -1;

	ଅଙ୍କ = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	ଯଦି (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < ଅଙ୍କ))             /* parsed a broken timezone */
	   )
		ଫେରନ୍ତୁ -1;

	strbuf_addstr(result, orig_src);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅକ୍ଷର *parse_ident(ଅଚଳ ଅକ୍ଷର *buf)
{
	ଅଚଳ ଅକ୍ଷର *ltgt;
	size_t name_len;
	ସଂରଚନା strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	ଯଦି (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	ଯଦି (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	ଯଦି (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	ଯଦି (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	ଯଦି (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_add(&ident, buf, name_len);

	ବାଛ (whenspec) {
	ସ୍ଥିତି WHENSPEC_RAW:
		ଯଦି (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି WHENSPEC_RAW_PERMISSIVE:
		ଯଦି (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି WHENSPEC_RFC2822:
		ଯଦି (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି WHENSPEC_NOW:
		ଯଦି (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		ଭାଙ୍ଗ;
	}

	ଫେରନ୍ତୁ strbuf_detach(&ident, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_and_store_blob(
	ସଂରଚନା last_object *last,
	ସଂରଚନା object_id *oidout,
	uintmax_t mark)
{
	ସ୍ଥିର ସଂରଚନା strbuf buf = STRBUF_INIT;
	uintmax_t len;

	ଯଦି (parse_data(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	ଅନ୍ୟଥା {
		ଯଦି (last) {
			strbuf_release(&last->data);
			last->offset = 0;
			last->depth = 0;
		}
		stream_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

ସ୍ଥିର ଶୂନ୍ୟ parse_new_blob(ସଂରଚନା fast_import_state *state)
{
	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&last_blob, ଖାଲି, next_mark);
}

ସ୍ଥିର ଶୂନ୍ୟ unload_one_branch(ଶୂନ୍ୟ)
{
	ଯେତେବେଳେ (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = ULONG_MAX;
		ସଂରଚନା branch *e, *l = ଖାଲି, *p = ଖାଲି;

		ପାଇଁ (e = active_branches; e; e = e->active_next_branch) {
			ଯଦି (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		ଯଦି (p) {
			e = p->active_next_branch;
			p->active_next_branch = e->active_next_branch;
		} ଅନ୍ୟଥା {
			e = active_branches;
			active_branches = e->active_next_branch;
		}
		e->active = 0;
		e->active_next_branch = ଖାଲି;
		ଯଦି (e->branch_ବୃକ୍ଷ.ବୃକ୍ଷ) {
			release_ବୃକ୍ଷ_content_recursive(e->branch_ବୃକ୍ଷ.ବୃକ୍ଷ);
			e->branch_ବୃକ୍ଷ.ବୃକ୍ଷ = ଖାଲି;
		}
		cur_active_branches--;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ load_branch(ସଂରଚନା branch *b)
{
	load_ବୃକ୍ଷ(&b->branch_ବୃକ୍ଷ);
	ଯଦି (!b->active) {
		b->active = 1;
		b->active_next_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_ଗଣନା++;
	}
}

ସ୍ଥିର unsigned ଅକ୍ଷର convert_ଅଙ୍କ_notes_to_fanout(uintmax_t ଅଙ୍କ_notes)
{
	unsigned ଅକ୍ଷର fanout = 0;
	ଯେତେବେଳେ ((ଅଙ୍କ_notes >>= 8))
		fanout++;
	ଫେରନ୍ତୁ fanout;
}

ସ୍ଥିର ଶୂନ୍ୟ construct_path_with_fanout(ଅଚଳ ଅକ୍ଷର *hex_sha1,
		unsigned ଅକ୍ଷର fanout, ଅକ୍ଷର *path)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0, j = 0;
	ଯଦି (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	ଯେତେବେଳେ (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	memcpy(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

ସ୍ଥିର uintmax_t do_change_note_fanout(
		ସଂରଚନା ବୃକ୍ଷ_entry *orig_ମୂଳ, ସଂରଚନା ବୃକ୍ଷ_entry *ମୂଳ,
		ଅକ୍ଷର *hex_oid, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hex_oid_len,
		ଅକ୍ଷର *fullpath, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fullpath_len,
		unsigned ଅକ୍ଷର fanout)
{
	ସଂରଚନା ବୃକ୍ଷ_content *t;
	ସଂରଚନା ବୃକ୍ଷ_entry *e, leaf;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t ଅଙ୍କ_notes = 0;
	ସଂରଚନା object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	ଅକ୍ଷର realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ଅଚଳ unsigned hexsz = the_hash_algo->hexsz;

	ଯଦି (!ମୂଳ->ବୃକ୍ଷ)
		load_ବୃକ୍ଷ(ମୂଳ);
	t = ମୂଳ->ବୃକ୍ଷ;

	ପାଇଁ (i = 0; t && i < t->entry_ଗଣନା; i++) {
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
		ଯଦି (!e->versions[1].mode ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			ଜାରି;

		/* This _may_ be a note entry, or a subdir containing notes */
		memcpy(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		ଯଦି (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		memcpy(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		ଯଦି (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			ଯଦି (fanout == 0xff) {
				/* Counting mode, no rename */
				ଅଙ୍କ_notes++;
				ଜାରି;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			ଯଦି (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				ଅଙ୍କ_notes++;
				ଜାରି;
			}

			/* Rename fullpath to realpath */
			ଯଦି (!ବୃକ୍ଷ_content_remove(orig_ମୂଳ, fullpath, &leaf, 0))
				die(_("failed to remove path %s"), fullpath);
			ବୃକ୍ଷ_content_set(orig_ମୂଳ, realpath,
				&leaf.versions[1].oid,
				leaf.versions[1].mode,
				leaf.ବୃକ୍ଷ);
		} ଅନ୍ୟଥା ଯଦି (S_ISDIR(e->versions[1].mode)) {
			/* This is a subdir that may contain note entries */
			ଅଙ୍କ_notes += do_change_note_fanout(orig_ମୂଳ, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = ମୂଳ->ବୃକ୍ଷ;
	}
	ଫେରନ୍ତୁ ଅଙ୍କ_notes;
}

ସ୍ଥିର uintmax_t change_note_fanout(ସଂରଚନା ବୃକ୍ଷ_entry *ମୂଳ,
		unsigned ଅକ୍ଷର fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	ଅକ୍ଷର hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	ଫେରନ୍ତୁ do_change_note_fanout(ମୂଳ, ମୂଳ, hex_oid, 0, path, 0, fanout);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_mapped_oid_hex(ଅଚଳ ଅକ୍ଷର *hex, ସଂରଚନା object_id *oid, ଅଚଳ ଅକ୍ଷର **end)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, ଆକାର(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	ଯଦି (algo == GIT_HASH_UNKNOWN)
		ଫେରନ୍ତୁ -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	ଯଦି (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		ଯଦି (hash_algos[algo].format_id == the_hash_algo->format_id)
			ଫେରନ୍ତୁ 0;
		ଫେରନ୍ତୁ -1;
	}
	oidcpy(oid, kh_ମୂଲ୍ୟ(sub_oid_map, it));
	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର uintmax_t parse_mark_ref(ଅଚଳ ଅକ୍ଷର *p, ଅକ୍ଷର **endptr)
{
	uintmax_t mark;

	ପ୍ରତିଜ୍ଞା(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	ଯଦି (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	ଫେରନ୍ତୁ mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
ସ୍ଥିର uintmax_t parse_mark_ref_eol(ଅଚଳ ଅକ୍ଷର *p)
{
	ଅକ୍ଷର *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	ଯଦି (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	ଫେରନ୍ତୁ mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
ସ୍ଥିର uintmax_t parse_mark_ref_space(ଅଚଳ ଅକ୍ଷର **p)
{
	uintmax_t mark;
	ଅକ୍ଷର *end;

	mark = parse_mark_ref(*p, &end);
	ଯଦି (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	ଫେରନ୍ତୁ mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
ସ୍ଥିର ଶୂନ୍ୟ parse_path(ସଂରଚନା strbuf *sb, ଅଚଳ ଅକ୍ଷର *p, ଅଚଳ ଅକ୍ଷର **endp,
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_last_field, ଅଚଳ ଅକ୍ଷର *field)
{
	ଯଦି (*p == '"') {
		ଯଦି (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), field, command_buf.buf);
		ଯଦି (strlen(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), field, command_buf.buf);
	} ଅନ୍ୟଥା {
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
ସ୍ଥିର ଶୂନ୍ୟ parse_path_eol(ସଂରଚନା strbuf *sb, ଅଚଳ ଅକ୍ଷର *p, ଅଚଳ ଅକ୍ଷର *field)
{
	ଅଚଳ ଅକ୍ଷର *end;

	parse_path(sb, p, &end, 1, field);
	ଯଦି (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
ସ୍ଥିର ଶୂନ୍ୟ parse_path_space(ସଂରଚନା strbuf *sb, ଅଚଳ ଅକ୍ଷର *p,
		ଅଚଳ ଅକ୍ଷର **endp, ଅଚଳ ଅକ୍ଷର *field)
{
	parse_path(sb, p, endp, 0, field);
	ଯଦି (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

ସ୍ଥିର ଶୂନ୍ୟ file_change_m(ସଂରଚନା fast_import_state *state,
			  ଅଚଳ ଅକ୍ଷର *p,
			  ସଂରଚନା branch *b)
{
	ସ୍ଥିର ସଂରଚନା strbuf path = STRBUF_INIT;
	ସଂରଚନା object_entry *oe;
	ସଂରଚନା object_id oid;
	uint16_t mode, inline_data = 0;

	p = parse_mode(p, &mode);
	ଯଦି (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	ବାଛ (mode) {
	ସ୍ଥିତି 0644:
	ସ୍ଥିତି 0755:
		mode |= S_IFREG;
	ସ୍ଥିତି S_IFREG | 0644:
	ସ୍ଥିତି S_IFREG | 0755:
	ସ୍ଥିତି S_IFLNK:
	ସ୍ଥିତି S_IFDIR:
	ସ୍ଥିତି S_IFGITLINK:
		/* ok */
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	ଯଦି (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = ଖାଲି; /* not used with inline_data, but makes gcc happy */
	} ଅନ୍ୟଥା {
		ଯଦି (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		ଯଦି (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	ଯଦି (S_ISDIR(mode) &&
	    is_empty_ବୃକ୍ଷ_oid(&oid, the_repository->hash_algo) &&
	    *path.buf) {
		ବୃକ୍ଷ_content_remove(&b->branch_ବୃକ୍ଷ, path.buf, ଖାଲି, 0);
		ଫେରନ୍ତୁ;
	}

	ଯଦି (S_ISGITLINK(mode)) {
		ଯଦି (inline_data)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		ଅନ୍ୟଥା ଯଦି (oe) {
			ଯଦି (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} ଅନ୍ୟଥା ଯଦି (inline_data) {
		ଯଦି (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		ଯେତେବେଳେ (read_next_command(state) != EOF) {
			ଅଚଳ ଅକ୍ଷର *v;
			ଯଦି (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			ଅନ୍ୟଥା {
				parse_and_store_blob(&last_blob, &oid, 0);
				ଭାଙ୍ଗ;
			}
		}
	} ଅନ୍ୟଥା {
		ଗଣନା object_type expected = S_ISDIR(mode) ?
						OBJ_ବୃକ୍ଷ: OBJ_BLOB;
		ଗଣନା object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, ଖାଲି);
		ଯଦି (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(mode) ?  _("tree") : _("blob"),
			    command_buf.buf);
		ଯଦି (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	ଯଦି (!*path.buf) {
		ବୃକ୍ଷ_content_replace(&b->branch_ବୃକ୍ଷ, &oid, mode, ଖାଲି);
		ଫେରନ୍ତୁ;
	}

	ଯଦି (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	ବୃକ୍ଷ_content_set(&b->branch_ବୃକ୍ଷ, path.buf, &oid, mode, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ file_change_d(ଅଚଳ ଅକ୍ଷର *p, ସଂରଚନା branch *b)
{
	ସ୍ଥିର ସଂରଚନା strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	ବୃକ୍ଷ_content_remove(&b->branch_ବୃକ୍ଷ, path.buf, ଖାଲି, 1);
}

ସ୍ଥିର ଶୂନ୍ୟ file_change_cr(ଅଚଳ ଅକ୍ଷର *p, ସଂରଚନା branch *b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rename)
{
	ସ୍ଥିର ସଂରଚନା strbuf source = STRBUF_INIT;
	ସ୍ଥିର ସଂରଚନା strbuf dest = STRBUF_INIT;
	ସଂରଚନା ବୃକ୍ଷ_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	memset(&leaf, 0, ଆକାର(leaf));
	ଯଦି (rename)
		ବୃକ୍ଷ_content_remove(&b->branch_ବୃକ୍ଷ, source.buf, &leaf, 1);
	ଅନ୍ୟଥା
		ବୃକ୍ଷ_content_get(&b->branch_ବୃକ୍ଷ, source.buf, &leaf, 1);
	ଯଦି (!leaf.versions[1].mode)
		die(_("path %s not in branch"), source.buf);
	ଯଦି (!*dest.buf) {	/* C "path/to/subdir" "" */
		ବୃକ୍ଷ_content_replace(&b->branch_ବୃକ୍ଷ,
			&leaf.versions[1].oid,
			leaf.versions[1].mode,
			leaf.ବୃକ୍ଷ);
		ଫେରନ୍ତୁ;
	}
	ଯଦି (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	ବୃକ୍ଷ_content_set(&b->branch_ବୃକ୍ଷ, dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.ବୃକ୍ଷ);
}

ସ୍ଥିର ଶୂନ୍ୟ note_change_n(ସଂରଚନା fast_import_state *state,
			  ଅଚଳ ଅକ୍ଷର *p,
			  ସଂରଚନା branch *b,
			  unsigned ଅକ୍ଷର *old_fanout)
{
	ସଂରଚନା object_entry *oe;
	ସଂରଚନା branch *s;
	ସଂରଚନା object_id oid, commit_oid;
	ଅକ୍ଷର path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_data = 0;
	unsigned ଅକ୍ଷର new_fanout;

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
	ଯଦି (b->ଅଙ୍କ_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->ଅଙ୍କ_notes = change_note_fanout(&b->branch_ବୃକ୍ଷ, 0xff);
		*old_fanout = convert_ଅଙ୍କ_notes_to_fanout(b->ଅଙ୍କ_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	ଯଦି (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = ଖାଲି; /* not used with inline_data, but makes gcc happy */
	} ଅନ୍ୟଥା {
		ଯଦି (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		ଯଦି (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	ଯଦି (s) {
		ଯଦି (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} ଅନ୍ୟଥା ଯଦି (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		ସଂରଚନା object_entry *commit_oe = find_mark(marks, commit_mark);
		ଯଦି (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} ଅନ୍ୟଥା ଯଦି (!repo_get_oid(the_repository, p, &commit_oid)) {
		size_t size;
		ଅକ୍ଷର *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &size,
						   &commit_oid);
		ଯଦି (!buf || size < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		ମୁକ୍ତ(buf);
	} ଅନ୍ୟଥା
		die(_("invalid ref name or SHA1 expression: %s"), p);

	ଯଦି (inline_data) {
		read_next_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} ଅନ୍ୟଥା ଯଦି (oe) {
		ଯଦି (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} ଅନ୍ୟଥା ଯଦି (!is_null_oid(&oid)) {
		ଗଣନା object_type type = odb_read_object_info(the_repository->objects, &oid,
							ଖାଲି);
		ଯଦି (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		ଯଦି (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	ଯଦି (ବୃକ୍ଷ_content_remove(&b->branch_ବୃକ୍ଷ, path, ଖାଲି, 0))
		b->ଅଙ୍କ_notes--;

	ଯଦି (is_null_oid(&oid))
		ଫେରନ୍ତୁ; /* nothing to insert */

	b->ଅଙ୍କ_notes++;
	new_fanout = convert_ଅଙ୍କ_notes_to_fanout(b->ଅଙ୍କ_notes);
	construct_path_with_fanout(oid_to_hex(&commit_oid), new_fanout, path);
	ବୃକ୍ଷ_content_set(&b->branch_ବୃକ୍ଷ, path, &oid, S_IFREG | 0644, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ file_change_deleteall(ସଂରଚନା branch *b)
{
	release_ବୃକ୍ଷ_content_recursive(b->branch_ବୃକ୍ଷ.ବୃକ୍ଷ);
	oidclr(&b->branch_ବୃକ୍ଷ.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_ବୃକ୍ଷ.versions[1].oid, the_repository->hash_algo);
	load_ବୃକ୍ଷ(&b->branch_ବୃକ୍ଷ);
	b->ଅଙ୍କ_notes = 0;
}

ସ୍ଥିର ଶୂନ୍ୟ parse_from_commit(ସଂରଚନା branch *b, ଅକ୍ଷର *buf, unsigned long size)
{
	ଯଦି (!buf || size < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	ଯଦି (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_ବୃକ୍ଷ.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_ବୃକ୍ଷ.versions[0].oid,
	       &b->branch_ବୃକ୍ଷ.versions[1].oid);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_from_existing(ସଂରଚନା branch *b)
{
	ଯଦି (is_null_oid(&b->oid)) {
		oidclr(&b->branch_ବୃକ୍ଷ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_ବୃକ୍ଷ.versions[1].oid, the_repository->hash_algo);
	} ଅନ୍ୟଥା {
		unsigned long size;
		size_t size_st = 0;
		ଅକ୍ଷର *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &size_st, &b->oid);
		size = cast_size_t_to_ulong(size_st);
		parse_from_commit(b, buf, size);
		ମୁକ୍ତ(buf);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_objectish(ସଂରଚନା fast_import_state *state,
			   ସଂରଚନା branch *b,
			   ଅଚଳ ଅକ୍ଷର *objectish)
{
	ସଂରଚନା branch *s;
	ସଂରଚନା object_id oid;

	oidcpy(&oid, &b->branch_ବୃକ୍ଷ.versions[1].oid);

	s = lookup_branch(objectish);
	ଯଦି (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	ଅନ୍ୟଥା ଯଦି (s) {
		ସଂରଚନା object_id *t = &s->branch_ବୃକ୍ଷ.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_ବୃକ୍ଷ.versions[0].oid, t);
		oidcpy(&b->branch_ବୃକ୍ଷ.versions[1].oid, t);
	} ଅନ୍ୟଥା ଯଦି (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		ସଂରଚନା object_entry *oe = find_mark(marks, idnum);
		ଯଦି (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		ଯଦି (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			ଯଦି (oe->pack_id != MAX_PACK_ID) {
				unsigned long size;
				ଅକ୍ଷର *buf = gfi_unpack_entry(oe, &size);
				parse_from_commit(b, buf, size);
				ମୁକ୍ତ(buf);
			} ଅନ୍ୟଥା
				parse_from_existing(b);
		}
	} ଅନ୍ୟଥା ଯଦି (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		ଯଦି (is_null_oid(&b->oid))
			b->delete = 1;
	}
	ଅନ୍ୟଥା
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	ଯଦି (b->branch_ବୃକ୍ଷ.ବୃକ୍ଷ && !oideq(&oid, &b->branch_ବୃକ୍ଷ.versions[1].oid)) {
		release_ବୃକ୍ଷ_content_recursive(b->branch_ବୃକ୍ଷ.ବୃକ୍ଷ);
		b->branch_ବୃକ୍ଷ.ବୃକ୍ଷ = ଖାଲି;
	}

	read_next_command(state);
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_from(ସଂରଚନା fast_import_state *state, ସଂରଚନା branch *b)
{
	ଅଚଳ ଅକ୍ଷର *from;

	ଯଦି (!skip_prefix(command_buf.buf, "from ", &from))
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ parse_objectish(state, b, from);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_objectish_with_prefix(ସଂରଚନା fast_import_state *state,
				       ସଂରଚନା branch *b,
				       ଅଚଳ ଅକ୍ଷର *prefix)
{
	ଅଚଳ ଅକ୍ଷର *base;

	ଯଦି (!skip_prefix(command_buf.buf, prefix, &base))
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ parse_objectish(state, b, base);
}

ସ୍ଥିର ସଂରଚନା hash_list *parse_merge(ସଂରଚନା fast_import_state *state,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଗଣନା_2)
{
	ସଂରଚନା hash_list *list = ଖାଲି, **tail = &list, *n;
	ଅଚଳ ଅକ୍ଷର *from;
	ସଂରଚନା branch *s;

	*ଗଣନା_2 = 0;
	ଯେତେବେଳେ (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(ଆକାର(*n));
		s = lookup_branch(from);
		ଯଦି (s)
			oidcpy(&n->oid, &s->oid);
		ଅନ୍ୟଥା ଯଦି (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			ସଂରଚନା object_entry *oe = find_mark(marks, idnum);
			ଯଦି (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} ଅନ୍ୟଥା ଯଦି (!repo_get_oid(the_repository, from, &n->oid)) {
			size_t size;
			ଅକ୍ଷର *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &size, &n->oid);
			ଯଦି (!buf || size < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			ମୁକ୍ତ(buf);
		} ଅନ୍ୟଥା
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->next = ଖାଲି;
		*tail = n;
		tail = &n->next;

		(*ଗଣନା_2)++;
		read_next_command(state);
	}
	ଫେରନ୍ତୁ list;
}

ସଂରଚନା signature_data {
	ଅକ୍ଷର *hash_algo;      /* "sha1" or "sha256" */
	ଅକ୍ଷର *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	ସଂରଚନା strbuf data;   /* The actual signature data */
};

ସ୍ଥିର ଶୂନ୍ୟ parse_one_signature(ସଂରଚନା fast_import_state *state,
				ସଂରଚନା signature_data *sig,
				ଅଚଳ ଅକ୍ଷର *v)
{
	ଅକ୍ଷର *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	ଅକ୍ଷର *space = strchr(args, ' ');

	ଯଦି (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), args);
	*space = '\0';

	sig->hash_algo = args;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	ଯଦି (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	ଯଦି (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	ଯଦି (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_next_command(state);
	parse_data(&sig->data, 0, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ discard_one_signature(ସଂରଚନା fast_import_state *state)
{
	ସଂରଚନା strbuf data = STRBUF_INIT;

	read_next_command(state);
	parse_data(&data, 0, ଖାଲି);
	strbuf_release(&data);
}

ସ୍ଥିର ଶୂନ୍ୟ add_gpgsig_to_commit(ସଂରଚନା strbuf *commit_data,
				 ଅଚଳ ଅକ୍ଷର *header,
				 ସଂରଚନା signature_data *sig)
{
	ସଂରଚନା string_list siglines = STRING_LIST_INIT_NODUP;

	ଯଦି (!sig || !sig->hash_algo)
		ଫେରନ୍ତୁ;

	strbuf_addstr(commit_data, header);
	string_list_split_in_place(&siglines, sig->data.buf, "\n", -1);
	strbuf_add_separated_string_list(commit_data, "\n ", &siglines);
	strbuf_addch(commit_data, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->data);
	ମୁକ୍ତ(sig->hash_algo);
}

ସ୍ଥିର ଶୂନ୍ୟ store_signature(ସଂରଚନା signature_data *stored_sig,
			    ସଂରଚନା signature_data *new_sig,
			    ଅଚଳ ଅକ୍ଷର *hash_type)
{
	ଯଦି (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->data);
		ମୁକ୍ତ(new_sig->hash_algo);
	} ଅନ୍ୟଥା {
		*stored_sig = *new_sig;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ import_one_signature(ସଂରଚନା fast_import_state *state,
				 ସଂରଚନା signature_data *sig_sha1,
				 ସଂରଚନା signature_data *sig_sha256,
				 ଅଚଳ ଅକ୍ଷର *v)
{
	ସଂରଚନା signature_data sig = { ଖାଲି, ଖାଲି, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	ଯଦି (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	ଅନ୍ୟଥା ଯଦି (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	ଅନ୍ୟଥା
		die(_("parse_one_signature() returned unknown hash algo"));
}

ସ୍ଥିର ଶୂନ୍ୟ finalize_commit_buffer(ସଂରଚନା strbuf *new_data,
				   ସଂରଚନା signature_data *sig_sha1,
				   ସଂରଚନା signature_data *sig_sha256,
				   ସଂରଚନା strbuf *ସନ୍ଦେଶ)
{
	add_gpgsig_to_commit(new_data, "gpgsig ", sig_sha1);
	add_gpgsig_to_commit(new_data, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_data, '\n');
	strbuf_addbuf(new_data, ସନ୍ଦେଶ);
}

ସ୍ଥିର ଶୂନ୍ୟ warn_invalid_signature(ସଂରଚନା signature_check *check,
				   ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ଗଣନା sign_mode mode)
{
	ଅଚଳ ଅକ୍ଷର *signer = check->signer ? check->signer : _("unknown");
	ଅଚଳ ଅକ୍ଷର *subject;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା subject_len = find_commit_subject(ସନ୍ଦେଶ, &subject);

	ବାଛ (mode) {
	ସ୍ଥିତି SIGN_STRIP_IF_INVALID:
		ଯଦି (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		ଅନ୍ୟଥା ଯଦି (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		ଅନ୍ୟଥା
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SIGN_SIGN_IF_INVALID:
		ଯଦି (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		ଅନ୍ୟଥା ଯଦି (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		ଅନ୍ୟଥା
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unsupported signing mode");
	}
}

ସ୍ଥିର ଶୂନ୍ୟ handle_signature_if_invalid(ସଂରଚନା strbuf *new_data,
					ସଂରଚନା signature_data *sig_sha1,
					ସଂରଚନା signature_data *sig_sha256,
					ସଂରଚନା strbuf *ସନ୍ଦେଶ,
					ଗଣନା sign_mode mode)
{
	ସଂରଚନା strbuf tmp_buf = STRBUF_INIT;
	ସଂରଚନା signature_check signature_check = { 0 };
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_data);
	finalize_commit_buffer(&tmp_buf, sig_sha1, sig_sha256, ସନ୍ଦେଶ);
	ret = verify_commit_buffer(tmp_buf.buf, tmp_buf.len, &signature_check);

	ଯଦି (ret) {
		ଯଦି (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, ସନ୍ଦେଶ->buf, mode);

		ଯଦି (mode == SIGN_SIGN_IF_INVALID) {
			ସଂରଚନା strbuf signature = STRBUF_INIT;
			ସଂରଚନା strbuf payload = STRBUF_INIT;

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
			ଯଦି (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			ଯଦି (sign_buffer(&payload, &signature, signed_commit_keyid,
					SIGN_BUFFER_USE_DEFAULT_ଚାବି))
				die(_("failed to sign commit object"));
			add_header_signature(new_data, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_buffer(new_data, ଖାଲି, ଖାଲି, ସନ୍ଦେଶ);
	} ଅନ୍ୟଥା {
		strbuf_swap(new_data, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_new_commit(ସଂରଚନା fast_import_state *state, ଅଚଳ ଅକ୍ଷର *arg)
{
	ସ୍ଥିର ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା signature_data sig_sha1 = { ଖାଲି, ଖାଲି, STRBUF_INIT };
	ସଂରଚନା signature_data sig_sha256 = { ଖାଲି, ଖାଲି, STRBUF_INIT };
	ସଂରଚନା branch *b;
	ଅକ୍ଷର *author = ଖାଲି;
	ଅକ୍ଷର *committer = ଖାଲି;
	ଅକ୍ଷର *encoding = ଖାଲି;
	ସଂରଚନା hash_list *merge_list = ଖାଲି;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା merge_ଗଣନା;
	unsigned ଅକ୍ଷର prev_fanout, new_fanout;
	ଅଚଳ ଅକ୍ଷର *v;

	b = lookup_branch(arg);
	ଯଦି (!b)
		b = new_branch(arg);

	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	ଯଦି (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_next_command(state);
	}
	ଯଦି (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_next_command(state);
	}
	ଯଦି (!committer)
		die(_("expected committer but didn't get one"));

	ଯେତେବେଳେ (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		ବାଛ (signed_commit_mode) {

		/* First, modes that don't need the signature to be parsed */
		ସ୍ଥିତି SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		ସ୍ଥିତି SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		ସ୍ଥିତି SIGN_STRIP:
			discard_one_signature(state);
			ଭାଙ୍ଗ;

		/* Second, modes that parse the signature */
		ସ୍ଥିତି SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		ସ୍ଥିତି SIGN_VERBATIM:
		ସ୍ଥିତି SIGN_STRIP_IF_INVALID:
		ସ୍ଥିତି SIGN_SIGN_IF_INVALID:
		ସ୍ଥିତି SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			ଭାଙ୍ଗ;

		/* Third, BUG */
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_next_command(state);
	}

	ଯଦି (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_next_command(state);
	}
	parse_data(&ସନ୍ଦେଶ, 0, ଖାଲି);
	read_next_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_ଗଣନା);

	/* ensure the branch is active/loaded */
	ଯଦି (!b->branch_ବୃକ୍ଷ.ବୃକ୍ଷ || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	prev_fanout = convert_ଅଙ୍କ_notes_to_fanout(b->ଅଙ୍କ_notes);

	/* file_change* */
	ଯେତେବେଳେ (command_buf.len > 0) {
		ଯଦି (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		ଅନ୍ୟଥା ଯଦି (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		ଅନ୍ୟଥା {
			unread_command_buf = 1;
			ଭାଙ୍ଗ;
		}
		ଯଦି (read_next_command(state) == EOF)
			ଭାଙ୍ଗ;
	}

	new_fanout = convert_ଅଙ୍କ_notes_to_fanout(b->ଅଙ୍କ_notes);
	ଯଦି (new_fanout != prev_fanout)
		b->ଅଙ୍କ_notes = change_note_fanout(&b->branch_ବୃକ୍ଷ, new_fanout);

	/* build the tree and the commit */
	store_ବୃକ୍ଷ(&b->branch_ବୃକ୍ଷ);
	oidcpy(&b->branch_ବୃକ୍ଷ.versions[0].oid,
	       &b->branch_ବୃକ୍ଷ.versions[1].oid);

	strbuf_reset(&new_data);
	strbuf_addf(&new_data, "tree %s\n",
		oid_to_hex(&b->branch_ବୃକ୍ଷ.versions[1].oid));
	ଯଦି (!is_null_oid(&b->oid))
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&b->oid));
	ଯେତେବେଳେ (merge_list) {
		ସଂରଚନା hash_list *next = merge_list->next;
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&merge_list->oid));
		ମୁକ୍ତ(merge_list);
		merge_list = next;
	}
	strbuf_addf(&new_data,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	ଯଦି (encoding)
		strbuf_addf(&new_data,
			"encoding %s\n",
			encoding);

	ଯଦି ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
	     signed_commit_mode == SIGN_SIGN_IF_INVALID ||
	     signed_commit_mode == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_data, &sig_sha1, &sig_sha256,
					    &ସନ୍ଦେଶ, signed_commit_mode);
	ଅନ୍ୟଥା
		finalize_commit_buffer(&new_data, &sig_sha1, &sig_sha256, &ସନ୍ଦେଶ);

	ମୁକ୍ତ(author);
	ମୁକ୍ତ(committer);
	ମୁକ୍ତ(encoding);

	ଯଦି (!store_object(OBJ_COMMIT, &new_data, ଖାଲି, &b->oid, next_mark))
		b->pack_id = pack_id;
	b->last_commit = object_ଗଣନା_by_type[OBJ_COMMIT];
}

ସ୍ଥିର ଶୂନ୍ୟ handle_tag_signature_if_invalid(ସଂରଚନା strbuf *buf,
					    ସଂରଚନା strbuf *ସନ୍ଦେଶ,
					    size_t sig_offset)
{
	ସଂରଚନା strbuf signature = STRBUF_INIT;
	ସଂରଚନା strbuf payload = STRBUF_INIT;
	ସଂରଚନା signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_add(&payload, ସନ୍ଦେଶ->buf, sig_offset);
	strbuf_add(&signature, ସନ୍ଦେଶ->buf + sig_offset, ସନ୍ଦେଶ->len - sig_offset);

	sigc.payload_type = SIGNATURE_PAYLOAD_TAG;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	ଯଦି (!check_signature(&sigc, signature.buf, signature.len))
		ଯାଅ out;

	ଯଦି (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(ସନ୍ଦେଶ, sig_offset);

	ଯଦି (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = ଖାଲି;
		strbuf_reset(&signature);

		ଯଦି (sign_buffer(&payload, &signature, signed_tag_keyid,
				SIGN_BUFFER_USE_DEFAULT_ଚାବି))
			die(_("failed to sign tag object"));

		strbuf_addbuf(ସନ୍ଦେଶ, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

ସ୍ଥିର ଶୂନ୍ୟ handle_tag_signature(ସଂରଚନା strbuf *buf, ସଂରଚନା strbuf *ସନ୍ଦେଶ, ଅଚଳ ଅକ୍ଷର *name)
{
	size_t sig_offset = parse_signed_buffer(ସନ୍ଦେଶ->buf, ସନ୍ଦେଶ->len);

	/* If there is no signature, there is nothing to do. */
	ଯଦି (sig_offset >= ସନ୍ଦେଶ->len)
		ଫେରନ୍ତୁ;

	ବାଛ (signed_tag_mode) {

	/* First, modes that don't change anything */
	ସ୍ଥିତି SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	ସ୍ଥିତି SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		ଭାଙ୍ଗ;

	/* Second, modes that remove the signature */
	ସ୍ଥିତି SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	ସ୍ଥିତି SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(ସନ୍ଦେଶ, sig_offset);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SIGN_ABORT_IF_INVALID:
	ସ୍ଥିତି SIGN_SIGN_IF_INVALID:
	ସ୍ଥିତି SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, ସନ୍ଦେଶ, sig_offset);
		ଭାଙ୍ଗ;

	/* Third, aborting modes */
	ସ୍ଥିତି SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ parse_new_tag(ସଂରଚନା fast_import_state *state, ଅଚଳ ଅକ୍ଷର *arg)
{
	ସ୍ଥିର ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *from;
	ଅକ୍ଷର *tagger;
	ସଂରଚନା branch *s;
	ସଂରଚନା tag *t;
	uintmax_t from_mark = 0;
	ସଂରଚନା object_id oid;
	ଗଣନା object_type type;
	ଅଚଳ ଅକ୍ଷର *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, ଆକାର(ସଂରଚନା tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	ଯଦି (last_tag)
		last_tag->next_tag = t;
	ଅନ୍ୟଥା
		first_tag = t;
	last_tag = t;
	read_next_command(state);
	parse_mark(state);

	/* from ... */
	ଯଦି (!skip_prefix(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	ଯଦି (s) {
		ଯଦି (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} ଅନ୍ୟଥା ଯଦି (*from == ':') {
		ସଂରଚନା object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} ଅନ୍ୟଥା ଯଦି (!repo_get_oid(the_repository, from, &oid)) {
		ସଂରଚନା object_entry *oe = find_object(&oid);
		ଯଦି (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, ଖାଲି);
			ଯଦି (type < 0)
				die(_("not a valid object: %s"), from);
		} ଅନ୍ୟଥା
			type = oe->type;
	} ଅନ୍ୟଥା
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_next_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	ଯଦି (skip_prefix(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_next_command(state);
	} ଅନ୍ୟଥା
		tagger = ଖାଲି;

	/* tag payload/message */
	parse_data(&ସନ୍ଦେଶ, 0, ଖାଲି);

	/* build the tag object */
	strbuf_reset(&new_data);

	strbuf_addf(&new_data,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_to_hex(&oid), type_name(type), t->name);
	ଯଦି (tagger)
		strbuf_addf(&new_data,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_data, &ସନ୍ଦେଶ, t->name);

	strbuf_addch(&new_data, '\n');
	strbuf_addbuf(&new_data, &ସନ୍ଦେଶ);
	ମୁକ୍ତ(tagger);

	ଯଦି (store_object(OBJ_TAG, &new_data, ଖାଲି, &t->oid, next_mark))
		t->pack_id = MAX_PACK_ID;
	ଅନ୍ୟଥା
		t->pack_id = pack_id;
}

ସ୍ଥିର ଶୂନ୍ୟ parse_reset_branch(ସଂରଚନା fast_import_state *state, ଅଚଳ ଅକ୍ଷର *arg)
{
	ସଂରଚନା branch *b;
	ଅଚଳ ଅକ୍ଷର *tag_name;

	b = lookup_branch(arg);
	ଯଦି (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_ବୃକ୍ଷ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_ବୃକ୍ଷ.versions[1].oid, the_repository->hash_algo);
		ଯଦି (b->branch_ବୃକ୍ଷ.ବୃକ୍ଷ) {
			release_ବୃକ୍ଷ_content_recursive(b->branch_ବୃକ୍ଷ.ବୃକ୍ଷ);
			b->branch_ବୃକ୍ଷ.ବୃକ୍ଷ = ଖାଲି;
		}
	}
	ଅନ୍ୟଥା
		b = new_branch(arg);
	read_next_command(state);
	parse_from(state, b);
	ଯଦି (b->delete && skip_prefix(b->name, "refs/tags/", &tag_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		ସଂରଚନା tag *t, *prev = ଖାଲି;
		ପାଇଁ (t = first_tag; t; t = t->next_tag) {
			ଯଦି (!strcmp(t->name, tag_name))
				ଭାଙ୍ଗ;
			prev = t;
		}
		ଯଦି (t) {
			ଯଦି (prev)
				prev->next_tag = t->next_tag;
			ଅନ୍ୟଥା
				first_tag = t->next_tag;
			ଯଦି (!t->next_tag)
				last_tag = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	ଯଦି (command_buf.len > 0)
		unread_command_buf = 1;
}

ସ୍ଥିର ଶୂନ୍ୟ cat_blob_write(ଅଚଳ ଅକ୍ଷର *buf, unsigned long size)
{
	ଯଦି (write_in_full(cat_blob_fd, buf, size) < 0)
		die_errno(_("write to frontend failed"));
}

ସ୍ଥିର ଶୂନ୍ୟ cat_blob(ସଂରଚନା object_entry *oe, ସଂରଚନା object_id *oid)
{
	ସଂରଚନା strbuf ଧାଡ଼ି = STRBUF_INIT;
	ସଂରଚନା iovec iov[3];
	unsigned long size;
	ଗଣନା object_type type = 0;
	ଅକ୍ଷର *buf;

	ଯଦି (!oe || oe->pack_id == MAX_PACK_ID) {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
	} ଅନ୍ୟଥା {
		type = oe->type;
		buf = gfi_unpack_entry(oe, &size);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	ଯଦି (type <= 0) {
		strbuf_reset(&ଧାଡ଼ି);
		strbuf_addf(&ଧାଡ଼ି, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(ଧାଡ଼ି.buf, ଧାଡ଼ି.len);
		strbuf_release(&ଧାଡ଼ି);
		ମୁକ୍ତ(buf);
		ଫେରନ୍ତୁ;
	}
	ଯଦି (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	ଯଦି (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_to_hex(oid), type_name(type));
	strbuf_reset(&ଧାଡ଼ି);
	strbuf_addf(&ଧାଡ଼ି, "%s %s %"PRIuMAX"\n", oid_to_hex(oid),
		    type_name(type), (uintmax_t)size);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_base = ଧାଡ଼ି.buf;
	iov[0].iov_len = ଧାଡ଼ି.len;
	iov[1].iov_base = buf;
	iov[1].iov_len = size;
	iov[2].iov_base = (ଶୂନ୍ୟ *) "\n";
	iov[2].iov_len = 1;

	ଯଦି (writev_in_full(cat_blob_fd, iov, ARRAY_SIZE(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&ଧାଡ଼ି);
	ଯଦି (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.data, buf, size, size + 1);
		last_blob.depth = oe->depth;
	} ଅନ୍ୟଥା
		ମୁକ୍ତ(buf);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_get_mark(ସଂରଚନା fast_import_state *state UNUSED,
			   ଅଚଳ ଅକ୍ଷର *p)
{
	ସଂରଚନା object_entry *oe;
	ଅକ୍ଷର output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	ଯଦି (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	ଯଦି (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, ଆକାର(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cat_blob(ସଂରଚନା fast_import_state *state UNUSED,
			   ଅଚଳ ଅକ୍ଷର *p)
{
	ସଂରଚନା object_entry *oe;
	ସଂରଚନା object_id oid;

	/* cat-blob SP <object> LF */
	ଯଦି (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		ଯଦି (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} ଅନ୍ୟଥା {
		ଯଦି (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		ଯଦି (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

ସ୍ଥିର ସଂରଚନା object_entry *dereference(ସଂରଚନା object_entry *oe,
					ସଂରଚନା object_id *oid)
{
	unsigned long size;
	ଅକ୍ଷର *buf = ଖାଲି;
	ଅଚଳ unsigned hexsz = the_hash_algo->hexsz;

	ଯଦି (!oe) {
		ଗଣନା object_type type = odb_read_object_info(the_repository->objects,
							     oid, ଖାଲି);
		ଯଦି (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = ଭର୍ତ୍ତି_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	ବାଛ (oe->type) {
	ସ୍ଥିତି OBJ_ବୃକ୍ଷ:	/* easy case. */
		ଫେରନ୍ତୁ oe;
	ସ୍ଥିତି OBJ_COMMIT:
	ସ୍ଥିତି OBJ_TAG:
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	ଯଦି (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &size);
	} ଅନ୍ୟଥା {
		ଗଣନା object_type unused;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &size_st);
		size = cast_size_t_to_ulong(size_st);
	}
	ଯଦି (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	ବାଛ (oe->type) {
	ସ୍ଥିତି OBJ_TAG:
		ଯଦି (size < hexsz + strlen("object ") ||
		    get_oid_hex(buf + strlen("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି OBJ_COMMIT:
		ଯଦି (size < hexsz + strlen("tree ") ||
		    get_oid_hex(buf + strlen("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	ମୁକ୍ତ(buf);
	ଫେରନ୍ତୁ find_object(oid);
}

ସ୍ଥିର ଶୂନ୍ୟ ଭର୍ତ୍ତି_mapped_mark(uintmax_t mark, ଶୂନ୍ୟ *object, ଶୂନ୍ୟ *cbp)
{
	ସଂରଚନା object_id *fromoid = object;
	ସଂରଚନା object_id *tooid = find_mark(cbp, mark);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	ଯଦି (ret == 0)
		ଫେରନ୍ତୁ;
	kh_ମୂଲ୍ୟ(sub_oid_map, it) = tooid;
}

ସ୍ଥିର ଶୂନ୍ୟ build_mark_map_one(ସଂରଚନା mark_set *from, ସଂରଚନା mark_set *to)
{
	for_each_mark(from, 0, ଭର୍ତ୍ତି_mapped_mark, to);
}

ସ୍ଥିର ଶୂନ୍ୟ build_mark_map(ସଂରଚନା string_list *from, ସଂରଚନା string_list *to)
{
	ସଂରଚନା string_list_item *fromp, *ଶୀର୍ଷ;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		ଶୀର୍ଷ = string_list_lookup(to, fromp->string);
		ଯଦି (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} ଅନ୍ୟଥା ଯଦି (!ଶୀର୍ଷ || !ଶୀର୍ଷ->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, ଶୀର୍ଷ->util);
	}
}

ସ୍ଥିର ସଂରଚନା object_entry *parse_treeish_dataref(ଅଚଳ ଅକ୍ଷର **p)
{
	ସଂରଚନା object_id oid;
	ସଂରଚନା object_entry *e;

	ଯଦି (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		ଯଦି (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} ଅନ୍ୟଥା {	/* <sha1> */
		ଯଦି (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		ଯଦି (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	ଯେତେବେଳେ (!e || e->type != OBJ_ବୃକ୍ଷ)
		e = dereference(e, &oid);
	ଫେରନ୍ତୁ e;
}

ସ୍ଥିର ଶୂନ୍ୟ print_ls(ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, ଅଚଳ unsigned ଅକ୍ଷର *hash, ଅଚଳ ଅକ୍ଷର *path)
{
	ସ୍ଥିର ସଂରଚନା strbuf ଧାଡ଼ି = STRBUF_INIT;

	/* See show_tree(). */
	ଅଚଳ ଅକ୍ଷର *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? ବୃକ୍ଷ_type :
		blob_type;

	ଯଦି (!mode) {
		/* missing SP path LF */
		strbuf_reset(&ଧାଡ଼ି);
		strbuf_addstr(&ଧାଡ଼ି, "missing ");
		quote_c_style(path, &ଧାଡ଼ି, ଖାଲି, 0);
		strbuf_addch(&ଧାଡ଼ି, '\n');
	} ଅନ୍ୟଥା {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&ଧାଡ଼ି);
		strbuf_addf(&ଧାଡ଼ି, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &ଧାଡ଼ି, ଖାଲି, 0);
		strbuf_addch(&ଧାଡ଼ି, '\n');
	}
	cat_blob_write(ଧାଡ଼ି.buf, ଧାଡ଼ି.len);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_ls(ସଂରଚନା fast_import_state *state UNUSED,
		     ଅଚଳ ଅକ୍ଷର *p,
		     ସଂରଚନା branch *b)
{
	ସ୍ଥିର ସଂରଚନା strbuf path = STRBUF_INIT;
	ସଂରଚନା ବୃକ୍ଷ_entry *ମୂଳ = ଖାଲି;
	ସଂରଚନା ବୃକ୍ଷ_entry leaf = {ଖାଲି};

	/* ls SP (<tree-ish> SP)? <path> */
	ଯଦି (*p == '"') {
		ଯଦି (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		ମୂଳ = &b->branch_ବୃକ୍ଷ;
	} ଅନ୍ୟଥା {
		ସଂରଚନା object_entry *e = parse_treeish_dataref(&p);
		ମୂଳ = new_ବୃକ୍ଷ_entry();
		oidcpy(&ମୂଳ->versions[1].oid, &e->idx.oid);
		ଯଦି (!is_null_oid(&ମୂଳ->versions[1].oid))
			ମୂଳ->versions[1].mode = S_IFDIR;
		load_ବୃକ୍ଷ(ମୂଳ);
	}
	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	ବୃକ୍ଷ_content_get(ମୂଳ, path.buf, &leaf, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	ଯଦି (S_ISDIR(leaf.versions[1].mode))
		store_ବୃକ୍ଷ(&leaf);

	print_ls(leaf.versions[1].mode, leaf.versions[1].oid.hash, path.buf);
	ଯଦି (leaf.ବୃକ୍ଷ)
		release_ବୃକ୍ଷ_content_recursive(leaf.ବୃକ୍ଷ);
	ଯଦି (!b || ମୂଳ != &b->branch_ବୃକ୍ଷ)
		release_ବୃକ୍ଷ_entry(ମୂଳ);
}

ସ୍ଥିର ଶୂନ୍ୟ checkpoint(ଶୂନ୍ୟ)
{
	checkpoint_requested = 0;
	ଯଦି (object_ଗଣନା) {
		cycle_packfile();
	}
	dump_branches();
	dump_tags();
	dump_marks();
}

ସ୍ଥିର ଶୂନ୍ୟ parse_checkpoint(ସଂରଚନା fast_import_state *state UNUSED)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

ସ୍ଥିର ଶୂନ୍ୟ parse_progress(ସଂରଚନା fast_import_state *state UNUSED)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

ସ୍ଥିର ଶୂନ୍ୟ parse_alias(ସଂରଚନା fast_import_state *state)
{
	ସଂରଚନା object_entry *e;
	ସଂରଚନା branch b;

	skip_optional_lf();
	read_next_command(state);

	/* mark ... */
	parse_mark(state);
	ଯଦି (!next_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, ଆକାର(b));
	ଯଦି (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	ପ୍ରତିଜ୍ଞା(e);
	ଭର୍ତ୍ତି_mark(&marks, next_mark, e);
}

ସ୍ଥିର ଅକ୍ଷର* make_fast_import_path(ସଂରଚନା fast_import_state *state,
				   ଅଚଳ ଅକ୍ଷର *path)
{
	ଯଦି (!relative_marks_paths || is_absolute_path(path))
		ଫେରନ୍ତୁ prefix_filename(state->prefix, path);
	ଫେରନ୍ତୁ repo_git_path(the_repository, "info/fast-import/%s", path);
}

ସ୍ଥିର ଶୂନ୍ୟ option_import_marks(ସଂରଚନା fast_import_state *state,
				ଅଚଳ ଅକ୍ଷର *marks,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା from_stream,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_missing)
{
	ଯଦି (import_marks_file) {
		ଯଦି (from_stream)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		ଯଦି(!import_marks_file_from_stream)
			read_marks();
	}

	ମୁକ୍ତ(import_marks_file);
	import_marks_file = make_fast_import_path(state, marks);
	import_marks_file_from_stream = from_stream;
	import_marks_file_ignore_missing = ignore_missing;
}

ସ୍ଥିର ଶୂନ୍ୟ option_date_format(ଅଚଳ ଅକ୍ଷର *fmt)
{
	ଯଦି (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	ଅନ୍ୟଥା ଯଦି (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	ଅନ୍ୟଥା ଯଦି (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	ଅନ୍ୟଥା ଯଦି (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	ଅନ୍ୟଥା
		die(_("unknown --date-format argument %s"), fmt);
}

ସ୍ଥିର unsigned long ulong_arg(ଅଚଳ ଅକ୍ଷର *option, ଅଚଳ ଅକ୍ଷର *arg)
{
	ଅକ୍ଷର *endptr;
	unsigned long rv = strtoul(arg, &endptr, 0);
	ଯଦି (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	ଫେରନ୍ତୁ rv;
}

ସ୍ଥିର ଶୂନ୍ୟ option_depth(ଅଚଳ ଅକ୍ଷର *depth)
{
	max_depth = ulong_arg("--depth", depth);
	ଯଦି (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

ସ୍ଥିର ଶୂନ୍ୟ option_active_branches(ଅଚଳ ଅକ୍ଷର *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

ସ୍ଥିର ଶୂନ୍ୟ option_export_marks(ସଂରଚନା fast_import_state *state,
				ଅଚଳ ଅକ୍ଷର *marks)
{
	ମୁକ୍ତ(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

ସ୍ଥିର ଶୂନ୍ୟ option_cat_blob_fd(ସଂରଚନା fast_import_state *state UNUSED,
			       ଅଚଳ ଅକ୍ଷର *fd)
{
	unsigned long n = ulong_arg("--cat-blob-fd", fd);
	ଯଦି (n > (unsigned long) INT_MAX)
		die(_("--cat-blob-fd cannot exceed %d"), INT_MAX);
	cat_blob_fd = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) n;
}

ସ୍ଥିର ଶୂନ୍ୟ option_export_pack_edges(ସଂରଚନା fast_import_state *state,
				     ଅଚଳ ଅକ୍ଷର *edges)
{
	ଅକ୍ଷର *fn = prefix_filename(state->prefix, edges);
	ଯଦି (pack_edges)
		fclose(pack_edges);
	pack_edges = xfopen(fn, "a");
	ମୁକ୍ତ(fn);
}

ସ୍ଥିର ଶୂନ୍ୟ option_rewrite_submodules(ସଂରଚନା fast_import_state *state,
				      ଅଚଳ ଅକ୍ଷର *arg,
				      ସଂରଚନା string_list *list)
{
	ସଂରଚନା mark_set *ms;
	FILE *fp;
	ଅକ୍ଷର *s = xstrdup(arg);
	ଅକ୍ଷର *f = strchr(s, ':');
	ଯଦି (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = prefix_filename(state->prefix, f);
	fp = fopen(f, "r");
	ଯଦି (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, ଭର୍ତ୍ତି_oid_entry);
	fclose(fp);
	ମୁକ୍ତ(f);

	string_list_ଭର୍ତ୍ତି(list, s)->util = ms;

	ମୁକ୍ତ(s);
}

ସ୍ଥିର ଶୂନ୍ୟ option_max_pack_size(ଅଚଳ ଅକ୍ଷର *arg)
{
	unsigned long v;

	ଯଦି (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	ଯଦି (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} ଅନ୍ୟଥା ଯଦି (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

ସ୍ଥିର ଶୂନ୍ୟ option_big_file_threshold(ଅଚଳ ଅକ୍ଷର *arg)
{
	unsigned long v;

	ଯଦି (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

ସ୍ଥିର ଶୂନ୍ୟ option_signed_commits(ଅଚଳ ଅକ୍ଷର *arg)
{
	ଯଦି (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

ସ୍ଥିର ଶୂନ୍ୟ option_signed_tags(ଅଚଳ ଅକ୍ଷର *arg)
{
	ଯଦି (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

ସ୍ଥିର ଶୂନ୍ୟ option_quiet(ଶୂନ୍ୟ)
{
	show_stats = 0;
	quiet = 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_one_option(ସଂରଚନା fast_import_state *state, ଅଚଳ ଅକ୍ଷର *option)
{
	ଯଦି (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_size(option);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(option, "big-file-threshold=", &option)) {
		option_big_file_threshold(option);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(option, "depth=", &option)) {
		option_depth(option);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(option, "active-branches=", &option)) {
		option_active_branches(option);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(option, "export-pack-edges=", &option)) {
		option_export_pack_edges(state, option);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(option, "signed-tags=", &option)) {
		option_signed_tags(option);
	} ଅନ୍ୟଥା ଯଦି (!strcmp(option, "quiet")) {
		option_quiet();
	} ଅନ୍ୟଥା ଯଦି (!strcmp(option, "stats")) {
		show_stats = 1;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} ଅନ୍ୟଥା {
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଶୂନ୍ୟ check_unsafe_feature(ସଂରଚନା fast_import_state *state,
				 ଅଚଳ ଅକ୍ଷର *feature)
{
	ଯଦି (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_one_feature(ସଂରଚନା fast_import_state *state,
			     ଅଚଳ ଅକ୍ଷର *feature)
{
	ଅଚଳ ଅକ୍ଷର *arg;

	ଯଦି (skip_prefix(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} ଅନ୍ୟଥା ଯଦି (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_to);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} ଅନ୍ୟଥା ଯଦି (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} ଅନ୍ୟଥା ଯଦି (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} ଅନ୍ୟଥା ଯଦି (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(feature, "force")) {
		force_update = 1;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} ଅନ୍ୟଥା {
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଶୂନ୍ୟ parse_feature(ସଂରଚନା fast_import_state *state, ଅଚଳ ଅକ୍ଷର *feature)
{
	ଯଦି (state->seen_data_command)
		die(_("got feature command '%s' after data command"), feature);

	ଯଦି (parse_one_feature(state, feature))
		ଫେରନ୍ତୁ;

	die(_("this version of fast-import does not support feature %s."), feature);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_option(ସଂରଚନା fast_import_state *state, ଅଚଳ ଅକ୍ଷର *option)
{
	ଯଦି (state->seen_data_command)
		die(_("got option command '%s' after data command"), option);

	ଯଦି (parse_one_option(state, option))
		ଫେରନ୍ତୁ;

	die(_("this version of fast-import does not support option: %s"), option);
}

ସ୍ଥିର ଶୂନ୍ୟ git_pack_config(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା indexversion_ମୂଲ୍ୟ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା limit;
	unsigned long packsizelimit_ମୂଲ୍ୟ;

	ଯଦି (!repo_config_get_ulong(the_repository, "pack.depth", &max_depth)) {
		ଯଦି (max_depth > MAX_DEPTH)
			max_depth = MAX_DEPTH;
	}
	ଯଦି (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_ମୂଲ୍ୟ)) {
		pack_idx_opts.version = indexversion_ମୂଲ୍ୟ;
		ଯଦି (pack_idx_opts.version > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.version);
	}
	ଯଦି (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_ମୂଲ୍ୟ))
		max_packsize = packsizelimit_ମୂଲ୍ୟ;

	ଯଦି (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &limit))
		unpack_limit = limit;
	ଅନ୍ୟଥା ଯଦି (!repo_config_get_int(the_repository, "transfer.unpacklimit", &limit))
		unpack_limit = limit;

	repo_config(the_repository, git_default_config, ଖାଲି);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	ଖାଲି
};

ସ୍ଥିର ଶୂନ୍ୟ parse_argv(ସଂରଚନା fast_import_state *state)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	ଯଦି (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_data_command = 1;
	ଯଦି (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_date_format(ଅଚଳ ସଂରଚନା option *opt UNUSED,
				    ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_export_pack_edges(ଅଚଳ ସଂରଚନା option *opt,
					  ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->ମୂଲ୍ୟ, arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_max_pack_size(ଅଚଳ ସଂରଚନା option *opt UNUSED,
				      ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_size(arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_big_file_threshold(ଅଚଳ ସଂରଚନା option *opt UNUSED,
					   ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_signed_commits(ଅଚଳ ସଂରଚନା option *opt UNUSED,
				       ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_signed_tags(ଅଚଳ ସଂରଚନା option *opt UNUSED,
				    ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_rewrite_submodules_from(ଅଚଳ ସଂରଚନା option *opt,
						ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->ମୂଲ୍ୟ, arg, &sub_marks_from);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_rewrite_submodules_to(ଅଚଳ ସଂରଚନା option *opt,
					      ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->ମୂଲ୍ୟ, arg, &sub_marks_to);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_cat_blob_fd(ଅଚଳ ସଂରଚନା option *opt,
				    ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->ମୂଲ୍ୟ, arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_import_marks(ଅଚଳ ସଂରଚନା option *opt,
				     ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->ମୂଲ୍ୟ, arg, 0, 0);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_import_marks_if_exists(ଅଚଳ ସଂରଚନା option *opt,
					       ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->ମୂଲ୍ୟ, arg, 0, 1);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_export_marks(ଅଚଳ ସଂରଚନା option *opt,
				     ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->ମୂଲ୍ୟ, arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_depth(ଅଚଳ ସଂରଚନା option *opt UNUSED,
			      ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_active_branches(ଅଚଳ ସଂରଚନା option *opt UNUSED,
					ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_quiet(ଅଚଳ ସଂରଚନା option *opt UNUSED,
			      ଅଚଳ ଅକ୍ଷର *arg UNUSED, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_fast_import(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		    ଅଚଳ ଅକ୍ଷର **argv,
		    ଅଚଳ ଅକ୍ଷର *prefix,
		    ସଂରଚନା repository *repo)
{
	ସଂରଚନା fast_import_state state;

	ସଂରଚନା option fast_import_options[] = {
		OPT_GROUP(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", ଖାଲି, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", ଖାଲି, ଖାଲି,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", ଖାଲି, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_size),
		OPT_CALLBACK_F(0, "big-file-threshold", ଖାଲି, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_threshold),
		OPT_CALLBACK_F(0, "depth", ଖାଲି, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_depth),
		OPT_CALLBACK_F(0, "active-branches", ଖାଲି, N_("n"),
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
		OPT_CALLBACK_F(0, "signed-commits", ଖାଲି, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", ଖାଲି, N_("mode"),
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
	CALLOC_ARRAY(avail_ବୃକ୍ଷ_table, avail_ବୃକ୍ଷ_table_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, ଆକାର(ସଂରଚନା mark_set));

	hashmap_init(&object_table, object_entry_hashcmp, ଖାଲି, 0);

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
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];
		ଯଦି (*arg != '-' || !strcmp(arg, "--"))
			ଭାଙ୍ଗ;
		ଯଦି (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * ଆକାର(*rc_free));
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < (cmd_save - 1); i++)
		rc_free[i].next = &rc_free[i + 1];
	rc_free[cmd_save - 1].next = ଖାଲି;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	ଯେତେବେଳେ (read_next_command(&state) != EOF) {
		ଅଚଳ ଅକ୍ଷର *v;
		ଯଦି (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, ଖାଲି);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		ଅନ୍ୟଥା ଯଦି (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		ଅନ୍ୟଥା ଯଦି (!strcmp("done", command_buf.buf))
			ଭାଙ୍ଗ;
		ଅନ୍ୟଥା ଯଦି (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		ଅନ୍ୟଥା ଯଦି (starts_with(command_buf.buf, "progress "))
			parse_progress(&state);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		ଅନ୍ୟଥା ଯଦି (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		ଅନ୍ୟଥା
			die(_("unsupported command: %s"), command_buf.buf);

		ଯଦି (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	ଯଦି (!state.seen_data_command)
		parse_argv(&state);

	ଯଦି (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	end_packfile();

	dump_branches();
	dump_tags();
	unkeep_all_packs();
	dump_marks();

	ଯଦି (pack_edges)
		fclose(pack_edges);

	ଯଦି (show_stats) {
		uintmax_t total_ଗଣନା = 0, duplicate_ଗଣନା = 0;
		ପାଇଁ (size_t i = 0; i < ARRAY_SIZE(object_ଗଣନା_by_type); i++)
			total_ଗଣନା += object_ଗଣନା_by_type[i];
		ପାଇଁ (size_t i = 0; i < ARRAY_SIZE(duplicate_ଗଣନା_by_type); i++)
			duplicate_ଗଣନା += duplicate_ଗଣନା_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_ଗଣନା);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", total_ଗଣନା, duplicate_ଗଣନା);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ଗଣନା_by_type[OBJ_BLOB], duplicate_ଗଣନା_by_type[OBJ_BLOB], delta_ଗଣନା_by_type[OBJ_BLOB], delta_ଗଣନା_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ଗଣନା_by_type[OBJ_ବୃକ୍ଷ], duplicate_ଗଣନା_by_type[OBJ_ବୃକ୍ଷ], delta_ଗଣନା_by_type[OBJ_ବୃକ୍ଷ], delta_ଗଣନା_attempts_by_type[OBJ_ବୃକ୍ଷ]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ଗଣନା_by_type[OBJ_COMMIT], duplicate_ଗଣନା_by_type[OBJ_COMMIT], delta_ଗଣନା_by_type[OBJ_COMMIT], delta_ଗଣନା_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ଗଣନା_by_type[OBJ_TAG], duplicate_ଗଣନା_by_type[OBJ_TAG], delta_ଗଣନା_by_type[OBJ_TAG], delta_ଗଣନା_attempts_by_type[OBJ_TAG]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_ଗଣନା, branch_load_ଗଣନା);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->shift) * 1024, marks_set_ଗଣନା);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (ବୃକ୍ଷ_entry_allocd + fi_mem_pool.pool_alloc + alloc_ଗଣନା*ଆକାର(ସଂରଚନା object_entry))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (unsigned long)((ବୃକ୍ଷ_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_ଗଣନା*ଆକାର(ସଂରଚନା object_entry))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	ଫେରନ୍ତୁ failure ? 1 : 0;
}
