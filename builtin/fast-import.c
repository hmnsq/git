#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "object.h"
#അന്തര്ഭാവ "blob.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "delta.h"
#അന്തര്ഭാവ "pack.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "csum-file.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "packfile.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "mem-pool.h"
#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "khash.h"
#അന്തര്ഭാവ "date.h"
#അന്തര്ഭാവ "gpg-interface.h"
#അന്തര്ഭാവ "parse-options.h"

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

ഘടന object_entry {
	ഘടന pack_idx_entry idx;
	ഘടന hashmap_entry ent;
	uint32_t type : TYPE_BITS,
		pack_id : PACK_ID_BITS,
		depth : DEPTH_BITS;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ object_entry_hashcmp(സ്ഥിരം ശൂന്യം *map_data UNUSED,
				സ്ഥിരം ഘടന hashmap_entry *eptr,
				സ്ഥിരം ഘടന hashmap_entry *entry_or_താക്കോൽ,
				സ്ഥിരം ശൂന്യം *keydata)
{
	സ്ഥിരം ഘടന object_id *oid = keydata;
	സ്ഥിരം ഘടന object_entry *e1, *e2;

	e1 = container_of(eptr, സ്ഥിരം ഘടന object_entry, ent);
	എങ്കിൽ (oid)
		തിരികെ oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_താക്കോൽ, സ്ഥിരം ഘടന object_entry, ent);
	തിരികെ oidcmp(&e1->idx.oid, &e2->idx.oid);
}

ഘടന object_entry_pool {
	ഘടന object_entry_pool *next_pool;
	ഘടന object_entry *next_free;
	ഘടന object_entry *end;
	ഘടന object_entry entries[FLEX_ARRAY]; /* more */
};

ഘടന mark_set {
	സംഘം {
		ഘടന object_id *oids[1024];
		ഘടന object_entry *marked[1024];
		ഘടന mark_set *sets[1024];
	} data;
	unsigned പൂർണ്ണസംഖ്യ shift;
};

ഘടന last_object {
	ഘടന strbuf data;
	off_t offset;
	unsigned പൂർണ്ണസംഖ്യ depth;
	unsigned no_swap : 1;
};

ഘടന atom_str {
	ഘടന atom_str *next_atom;
	unsigned short str_len;
	അക്ഷരം str_dat[FLEX_ARRAY]; /* more */
};

ഘടന വൃക്ഷം_content;
ഘടന വൃക്ഷം_entry {
	ഘടന വൃക്ഷം_content *വൃക്ഷം;
	ഘടന atom_str *name;
	ഘടന വൃക്ഷം_entry_ms {
		uint16_t mode;
		ഘടന object_id oid;
	} versions[2];
};

ഘടന വൃക്ഷം_content {
	unsigned പൂർണ്ണസംഖ്യ entry_capacity; /* must match avail_tree_content */
	unsigned പൂർണ്ണസംഖ്യ entry_എണ്ണം;
	unsigned പൂർണ്ണസംഖ്യ delta_depth;
	ഘടന വൃക്ഷം_entry *entries[FLEX_ARRAY]; /* more */
};

ഘടന avail_വൃക്ഷം_content {
	unsigned പൂർണ്ണസംഖ്യ entry_capacity; /* must match tree_content */
	ഘടന avail_വൃക്ഷം_content *next_avail;
};

ഘടന branch {
	ഘടന branch *table_next_branch;
	ഘടന branch *active_next_branch;
	സ്ഥിരം അക്ഷരം *name;
	ഘടന വൃക്ഷം_entry branch_വൃക്ഷം;
	uintmax_t last_commit;
	uintmax_t അക്കം_notes;
	unsigned active : 1;
	unsigned delete : 1;
	unsigned pack_id : PACK_ID_BITS;
	ഘടന object_id oid;
};

ഘടന tag {
	ഘടന tag *next_tag;
	സ്ഥിരം അക്ഷരം *name;
	unsigned പൂർണ്ണസംഖ്യ pack_id;
	ഘടന object_id oid;
};

ഘടന hash_list {
	ഘടന hash_list *next;
	ഘടന object_id oid;
};

തരനിർവചനം എണ്ണൽ {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

ഘടന recent_command {
	ഘടന recent_command *prev;
	ഘടന recent_command *next;
	അക്ഷരം *buf;
};

തരനിർവചനം ശൂന്യം (*mark_set_inserter_t)(ഘടന mark_set **s, ഘടന object_id *oid, uintmax_t mark);
തരനിർവചനം ശൂന്യം (*each_mark_fn_t)(uintmax_t mark, ശൂന്യം *obj, ശൂന്യം *cbp);

/* Configured limits on output */
സ്ഥിതം unsigned long max_depth = 50;
സ്ഥിതം off_t max_packsize;
സ്ഥിതം പൂർണ്ണസംഖ്യ unpack_limit = 100;
സ്ഥിതം പൂർണ്ണസംഖ്യ force_update;

/* Stats and misc. counters */
സ്ഥിതം uintmax_t alloc_എണ്ണം;
സ്ഥിതം uintmax_t marks_set_എണ്ണം;
സ്ഥിതം uintmax_t object_എണ്ണം_by_type[1 << TYPE_BITS];
സ്ഥിതം uintmax_t duplicate_എണ്ണം_by_type[1 << TYPE_BITS];
സ്ഥിതം uintmax_t delta_എണ്ണം_by_type[1 << TYPE_BITS];
സ്ഥിതം uintmax_t delta_എണ്ണം_attempts_by_type[1 << TYPE_BITS];
സ്ഥിതം unsigned long object_എണ്ണം;
സ്ഥിതം unsigned long branch_എണ്ണം;
സ്ഥിതം unsigned long branch_load_എണ്ണം;
സ്ഥിതം പൂർണ്ണസംഖ്യ failure;
സ്ഥിതം FILE *pack_edges;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_stats = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ quiet;
സ്ഥിതം എണ്ണൽ sign_mode signed_tag_mode = SIGN_VERBATIM;
സ്ഥിതം എണ്ണൽ sign_mode signed_commit_mode = SIGN_VERBATIM;
സ്ഥിതം സ്ഥിരം അക്ഷരം *signed_commit_keyid;
സ്ഥിതം സ്ഥിരം അക്ഷരം *signed_tag_keyid;

/* Memory pools */
സ്ഥിതം ഘടന mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - വലുപ്പം(ഘടന mp_block),
};

/* Atom management */
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ atom_table_sz = 4451;
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ atom_cnt;
സ്ഥിതം ഘടന atom_str **atom_table;

/* The .pack file being generated */
സ്ഥിതം ഘടന pack_idx_option pack_idx_opts;
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ pack_id;
സ്ഥിതം ഘടന hashfile *pack_file;
സ്ഥിതം ഘടന packed_git *pack_data;
സ്ഥിതം ഘടന packed_git **all_packs;
സ്ഥിതം off_t pack_size;

/* Table of objects we've written. */
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ object_entry_alloc = 5000;
സ്ഥിതം ഘടന object_entry_pool *blocks;
സ്ഥിതം ഘടന hashmap object_table;
സ്ഥിതം ഘടന mark_set *marks;
സ്ഥിതം അക്ഷരം *export_marks_file;
സ്ഥിതം അക്ഷരം *import_marks_file;
സ്ഥിതം പൂർണ്ണസംഖ്യ import_marks_file_from_stream;
സ്ഥിതം പൂർണ്ണസംഖ്യ import_marks_file_ignore_missing;
സ്ഥിതം പൂർണ്ണസംഖ്യ import_marks_file_done;
സ്ഥിതം പൂർണ്ണസംഖ്യ relative_marks_paths;

/* Our last blob */
സ്ഥിതം ഘടന last_object last_blob = {
	.data = STRBUF_INIT,
 };

/* Tree management */
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ വൃക്ഷം_entry_alloc = 1000;
സ്ഥിതം ശൂന്യം *avail_വൃക്ഷം_entry;
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ avail_വൃക്ഷം_table_sz = 100;
സ്ഥിതം ഘടന avail_വൃക്ഷം_content **avail_വൃക്ഷം_table;
സ്ഥിതം size_t വൃക്ഷം_entry_allocd;
സ്ഥിതം ഘടന strbuf old_വൃക്ഷം = STRBUF_INIT;
സ്ഥിതം ഘടന strbuf new_വൃക്ഷം = STRBUF_INIT;

/* Branch data */
സ്ഥിതം unsigned long max_active_branches = 5;
സ്ഥിതം unsigned long cur_active_branches;
സ്ഥിതം unsigned long branch_table_sz = 1039;
സ്ഥിതം ഘടന branch **branch_table;
സ്ഥിതം ഘടന branch *active_branches;

/* Tag data */
സ്ഥിതം ഘടന tag *first_tag;
സ്ഥിതം ഘടന tag *last_tag;

/* Input stream parsing */
സ്ഥിതം whenspec_type whenspec = WHENSPEC_RAW;
സ്ഥിതം ഘടന strbuf command_buf = STRBUF_INIT;
സ്ഥിതം പൂർണ്ണസംഖ്യ unread_command_buf;
സ്ഥിതം ഘടന recent_command cmd_hist = {
	.prev = &cmd_hist,
	.next = &cmd_hist,
};
സ്ഥിതം ഘടന recent_command *cmd_tail = &cmd_hist;
സ്ഥിതം ഘടന recent_command *rc_free;
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ cmd_save = 100;
സ്ഥിതം uintmax_t next_mark;
സ്ഥിതം ഘടന strbuf new_data = STRBUF_INIT;
സ്ഥിതം പൂർണ്ണസംഖ്യ require_explicit_termination;

/* Signal handling */
സ്ഥിതം അസ്ഥിര sig_atomic_t checkpoint_requested;

/* Submodule marks */
സ്ഥിതം ഘടന string_list sub_marks_from = STRING_LIST_INIT_DUP;
സ്ഥിതം ഘടന string_list sub_marks_to = STRING_LIST_INIT_DUP;
സ്ഥിതം kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
സ്ഥിതം പൂർണ്ണസംഖ്യ cat_blob_fd = STDOUT_FILENO;

/* Command state */
ഘടന fast_import_state {
	പൂർണ്ണസംഖ്യ argc;
	സ്ഥിരം അക്ഷരം **argv;
	സ്ഥിരം അക്ഷരം *prefix;
	പൂർണ്ണസംഖ്യ seen_data_command;
	പൂർണ്ണസംഖ്യ allow_unsafe_features;
	ഘടന option *option;
};

സ്ഥിതം ശൂന്യം fast_import_state_init(ഘടന fast_import_state *state,
				   പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				   സ്ഥിരം അക്ഷരം *prefix, ഘടന option *option)
{
	memset(state, 0, വലുപ്പം(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

സ്ഥിതം ശൂന്യം parse_argv(ഘടന fast_import_state *state);
സ്ഥിതം ശൂന്യം parse_get_mark(ഘടന fast_import_state *state, സ്ഥിരം അക്ഷരം *p);
സ്ഥിതം ശൂന്യം parse_cat_blob(ഘടന fast_import_state *state, സ്ഥിരം അക്ഷരം *p);
സ്ഥിതം ശൂന്യം parse_ls(ഘടന fast_import_state *state,
		     സ്ഥിരം അക്ഷരം *p,
		     ഘടന branch *b);

സ്ഥിതം ശൂന്യം for_each_mark(ഘടന mark_set *m, uintmax_t base, each_mark_fn_t callback, ശൂന്യം *p)
{
	uintmax_t k;
	എങ്കിൽ (m->shift) {
		വേണ്ടി (k = 0; k < 1024; k++) {
			എങ്കിൽ (m->data.sets[k])
				for_each_mark(m->data.sets[k], base + (k << m->shift), callback, p);
		}
	} അല്ലെങ്കിൽ {
		വേണ്ടി (k = 0; k < 1024; k++) {
			എങ്കിൽ (m->data.marked[k])
				callback(base + k, m->data.marked[k], p);
		}
	}
}

സ്ഥിതം ശൂന്യം dump_marks_fn(uintmax_t mark, ശൂന്യം *object, ശൂന്യം *cbp) {
	ഘടന object_entry *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

സ്ഥിതം ശൂന്യം write_branch_report(FILE *rpt, ഘടന branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	എങ്കിൽ (b->active)
		fputs(" active", rpt);
	എങ്കിൽ (b->branch_വൃക്ഷം.വൃക്ഷം)
		fputs(" loaded", rpt);
	എങ്കിൽ (is_null_oid(&b->branch_വൃക്ഷം.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_to_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_to_hex(&b->branch_വൃക്ഷം.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_to_hex(&b->branch_വൃക്ഷം.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->last_commit);

	fputs("  last pack   : ", rpt);
	എങ്കിൽ (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

സ്ഥിതം ശൂന്യം write_crash_report(സ്ഥിരം അക്ഷരം *err)
{
	അക്ഷരം *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	ഘടന branch *b;
	unsigned long lu;
	ഘടന recent_command *rc;

	എങ്കിൽ (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		സ്വതന്ത്രം(loc);
		തിരികെ;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(time(ശൂന്യ), 0, DATE_MODE(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	വേണ്ടി (rc = cmd_hist.next; rc != &cmd_hist; rc = rc->next) {
		എങ്കിൽ (rc->next == &cmd_hist)
			fputs("* ", rpt);
		അല്ലെങ്കിൽ
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
	വേണ്ടി (b = active_branches, lu = 0; b; b = b->active_next_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	വേണ്ടി (lu = 0; lu < branch_table_sz; lu++) {
		വേണ്ടി (b = branch_table[lu]; b; b = b->table_next_branch)
			write_branch_report(rpt, b);
	}

	എങ്കിൽ (first_tag) {
		ഘടന tag *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		വേണ്ടി (tg = first_tag; tg; tg = tg->next_tag) {
			fputs(oid_to_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	എങ്കിൽ (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	അല്ലെങ്കിൽ
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	സ്വതന്ത്രം(loc);
}

സ്ഥിതം ശൂന്യം end_packfile(ശൂന്യം);
സ്ഥിതം ശൂന്യം unkeep_all_packs(ശൂന്യം);
സ്ഥിതം ശൂന്യം dump_marks(ശൂന്യം);

സ്ഥിതം NORETURN ശൂന്യം die_nicely(സ്ഥിരം അക്ഷരം *err, va_list params)
{
	va_list cp;
	സ്ഥിതം പൂർണ്ണസംഖ്യ zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	എങ്കിൽ (!zombie) {
		അക്ഷരം message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, വലുപ്പം(message), err, cp);
		write_crash_report(message);
		end_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	exit(128);
}

#ifndef SIGUSR1	/* Windows, for example */

സ്ഥിതം ശൂന്യം set_checkpoint_signal(ശൂന്യം)
{
}

#അല്ലെങ്കിൽ

സ്ഥിതം ശൂന്യം checkpoint_signal(പൂർണ്ണസംഖ്യ signo UNUSED)
{
	checkpoint_requested = 1;
}

സ്ഥിതം ശൂന്യം set_checkpoint_signal(ശൂന്യം)
{
	ഘടന sigaction sa;

	memset(&sa, 0, വലുപ്പം(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, ശൂന്യ);
}

#endif

സ്ഥിതം ശൂന്യം alloc_objects(unsigned പൂർണ്ണസംഖ്യ cnt)
{
	ഘടന object_entry_pool *b;

	b = xmalloc(വലുപ്പം(ഘടന object_entry_pool)
		+ cnt * വലുപ്പം(ഘടന object_entry));
	b->next_pool = blocks;
	b->next_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_എണ്ണം += cnt;
}

സ്ഥിതം ഘടന object_entry *new_object(ഘടന object_id *oid)
{
	ഘടന object_entry *e;

	എങ്കിൽ (blocks->next_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->next_free++;
	oidcpy(&e->idx.oid, oid);
	തിരികെ e;
}

സ്ഥിതം ഘടന object_entry *find_object(ഘടന object_id *oid)
{
	തിരികെ hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   ഘടന object_entry, ent);
}

സ്ഥിതം ഘടന object_entry *തിരുകുക_object(ഘടന object_id *oid)
{
	ഘടന object_entry *e;
	unsigned പൂർണ്ണസംഖ്യ hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					ഘടന object_entry, ent);
	എങ്കിൽ (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	തിരികെ e;
}

സ്ഥിതം ശൂന്യം invalidate_pack_id(unsigned പൂർണ്ണസംഖ്യ id)
{
	unsigned long lu;
	ഘടന tag *t;
	ഘടന hashmap_iter iter;
	ഘടന object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		എങ്കിൽ (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	വേണ്ടി (lu = 0; lu < branch_table_sz; lu++) {
		ഘടന branch *b;

		വേണ്ടി (b = branch_table[lu]; b; b = b->table_next_branch)
			എങ്കിൽ (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	വേണ്ടി (t = first_tag; t; t = t->next_tag)
		എങ്കിൽ (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ hc_str(സ്ഥിരം അക്ഷരം *s, size_t len)
{
	unsigned പൂർണ്ണസംഖ്യ r = 0;
	ആകുമ്പോൾ (len-- > 0)
		r = r * 31 + *s++;
	തിരികെ r;
}

സ്ഥിതം ശൂന്യം തിരുകുക_mark(ഘടന mark_set **മുകൾ, uintmax_t idnum, ഘടന object_entry *oe)
{
	ഘടന mark_set *s = *മുകൾ;

	ആകുമ്പോൾ ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, വലുപ്പം(ഘടന mark_set));
		s->shift = (*മുകൾ)->shift + 10;
		s->data.sets[0] = *മുകൾ;
		*മുകൾ = s;
	}
	ആകുമ്പോൾ (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		എങ്കിൽ (!s->data.sets[i]) {
			s->data.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, വലുപ്പം(ഘടന mark_set));
			s->data.sets[i]->shift = s->shift - 10;
		}
		s = s->data.sets[i];
	}
	എങ്കിൽ (!s->data.marked[idnum])
		marks_set_എണ്ണം++;
	s->data.marked[idnum] = oe;
}

സ്ഥിതം ശൂന്യം *find_mark(ഘടന mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	ഘടന object_entry *oe = ശൂന്യ;
	എങ്കിൽ ((idnum >> s->shift) < 1024) {
		ആകുമ്പോൾ (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->data.sets[i];
		}
		എങ്കിൽ (s)
			oe = s->data.marked[idnum];
	}
	എങ്കിൽ (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	തിരികെ oe;
}

സ്ഥിതം ഘടന atom_str *to_atom(സ്ഥിരം അക്ഷരം *s, unsigned short len)
{
	unsigned പൂർണ്ണസംഖ്യ hc = hc_str(s, len) % atom_table_sz;
	ഘടന atom_str *c;

	വേണ്ടി (c = atom_table[hc]; c; c = c->next_atom)
		എങ്കിൽ (c->str_len == len && !strncmp(s, c->str_dat, len))
			തിരികെ c;

	c = mem_pool_alloc(&fi_mem_pool, വലുപ്പം(ഘടന atom_str) + len + 1);
	c->str_len = len;
	memcpy(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->next_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	തിരികെ c;
}

സ്ഥിതം ഘടന branch *lookup_branch(സ്ഥിരം അക്ഷരം *name)
{
	unsigned പൂർണ്ണസംഖ്യ hc = hc_str(name, strlen(name)) % branch_table_sz;
	ഘടന branch *b;

	വേണ്ടി (b = branch_table[hc]; b; b = b->table_next_branch)
		എങ്കിൽ (!strcmp(name, b->name))
			തിരികെ b;
	തിരികെ ശൂന്യ;
}

സ്ഥിതം ഘടന branch *new_branch(സ്ഥിരം അക്ഷരം *name)
{
	unsigned പൂർണ്ണസംഖ്യ hc = hc_str(name, strlen(name)) % branch_table_sz;
	ഘടന branch *b = lookup_branch(name);

	എങ്കിൽ (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	എങ്കിൽ (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, വലുപ്പം(ഘടന branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_next_branch = branch_table[hc];
	b->branch_വൃക്ഷം.versions[0].mode = S_IFDIR;
	b->branch_വൃക്ഷം.versions[1].mode = S_IFDIR;
	b->അക്കം_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_എണ്ണം++;
	തിരികെ b;
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ hc_entries(unsigned പൂർണ്ണസംഖ്യ cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	തിരികെ cnt < avail_വൃക്ഷം_table_sz ? cnt : avail_വൃക്ഷം_table_sz - 1;
}

സ്ഥിതം ഘടന വൃക്ഷം_content *new_വൃക്ഷം_content(unsigned പൂർണ്ണസംഖ്യ cnt)
{
	ഘടന avail_വൃക്ഷം_content *f, *l = ശൂന്യ;
	ഘടന വൃക്ഷം_content *t;
	unsigned പൂർണ്ണസംഖ്യ hc = hc_entries(cnt);

	വേണ്ടി (f = avail_വൃക്ഷം_table[hc]; f; l = f, f = f->next_avail)
		എങ്കിൽ (f->entry_capacity >= cnt)
			നിർത്തുക;

	എങ്കിൽ (f) {
		എങ്കിൽ (l)
			l->next_avail = f->next_avail;
		അല്ലെങ്കിൽ
			avail_വൃക്ഷം_table[hc] = f->next_avail;
	} അല്ലെങ്കിൽ {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, വലുപ്പം(*t) + വലുപ്പം(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (ഘടന വൃക്ഷം_content*)f;
	t->entry_എണ്ണം = 0;
	t->delta_depth = 0;
	തിരികെ t;
}

സ്ഥിതം ശൂന്യം release_വൃക്ഷം_entry(ഘടന വൃക്ഷം_entry *e);
സ്ഥിതം ശൂന്യം release_വൃക്ഷം_content(ഘടന വൃക്ഷം_content *t)
{
	ഘടന avail_വൃക്ഷം_content *f = (ഘടന avail_വൃക്ഷം_content*)t;
	unsigned പൂർണ്ണസംഖ്യ hc = hc_entries(f->entry_capacity);
	f->next_avail = avail_വൃക്ഷം_table[hc];
	avail_വൃക്ഷം_table[hc] = f;
}

സ്ഥിതം ശൂന്യം release_വൃക്ഷം_content_recursive(ഘടന വൃക്ഷം_content *t)
{
	unsigned പൂർണ്ണസംഖ്യ i;
	വേണ്ടി (i = 0; i < t->entry_എണ്ണം; i++)
		release_വൃക്ഷം_entry(t->entries[i]);
	release_വൃക്ഷം_content(t);
}

സ്ഥിതം ഘടന വൃക്ഷം_content *grow_വൃക്ഷം_content(
	ഘടന വൃക്ഷം_content *t,
	പൂർണ്ണസംഖ്യ amt)
{
	ഘടന വൃക്ഷം_content *r = new_വൃക്ഷം_content(t->entry_എണ്ണം + amt);
	r->entry_എണ്ണം = t->entry_എണ്ണം;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_എണ്ണം);
	release_വൃക്ഷം_content(t);
	തിരികെ r;
}

സ്ഥിതം ഘടന വൃക്ഷം_entry *new_വൃക്ഷം_entry(ശൂന്യം)
{
	ഘടന വൃക്ഷം_entry *e;

	എങ്കിൽ (!avail_വൃക്ഷം_entry) {
		unsigned പൂർണ്ണസംഖ്യ n = വൃക്ഷം_entry_alloc;
		വൃക്ഷം_entry_allocd += n * വലുപ്പം(ഘടന വൃക്ഷം_entry);
		ALLOC_ARRAY(e, n);
		avail_വൃക്ഷം_entry = e;
		ആകുമ്പോൾ (n-- > 1) {
			*((ശൂന്യം**)e) = e + 1;
			e++;
		}
		*((ശൂന്യം**)e) = ശൂന്യ;
	}

	e = avail_വൃക്ഷം_entry;
	avail_വൃക്ഷം_entry = *((ശൂന്യം**)e);
	തിരികെ e;
}

സ്ഥിതം ശൂന്യം release_വൃക്ഷം_entry(ഘടന വൃക്ഷം_entry *e)
{
	എങ്കിൽ (e->വൃക്ഷം)
		release_വൃക്ഷം_content_recursive(e->വൃക്ഷം);
	*((ശൂന്യം**)e) = avail_വൃക്ഷം_entry;
	avail_വൃക്ഷം_entry = e;
}

സ്ഥിതം ഘടന വൃക്ഷം_content *dup_വൃക്ഷം_content(ഘടന വൃക്ഷം_content *s)
{
	ഘടന വൃക്ഷം_content *d;
	ഘടന വൃക്ഷം_entry *a, *b;
	unsigned പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (!s)
		തിരികെ ശൂന്യ;
	d = new_വൃക്ഷം_content(s->entry_എണ്ണം);
	വേണ്ടി (i = 0; i < s->entry_എണ്ണം; i++) {
		a = s->entries[i];
		b = new_വൃക്ഷം_entry();
		memcpy(b, a, വലുപ്പം(*a));
		എങ്കിൽ (a->വൃക്ഷം && is_null_oid(&b->versions[1].oid))
			b->വൃക്ഷം = dup_വൃക്ഷം_content(a->വൃക്ഷം);
		അല്ലെങ്കിൽ
			b->വൃക്ഷം = ശൂന്യ;
		d->entries[i] = b;
	}
	d->entry_എണ്ണം = s->entry_എണ്ണം;
	d->delta_depth = s->delta_depth;

	തിരികെ d;
}

സ്ഥിതം ശൂന്യം start_packfile(ശൂന്യം)
{
	ഘടന strbuf tmp_file = STRBUF_INIT;
	ഘടന packed_git *p;
	പൂർണ്ണസംഖ്യ pack_fd;

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
	object_എണ്ണം = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *create_index(ശൂന്യം)
{
	സ്ഥിരം അക്ഷരം *tmpfile;
	ഘടന pack_idx_entry **idx, **c, **last;
	ഘടന object_entry *e;
	ഘടന object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_എണ്ണം);
	c = idx;
	വേണ്ടി (o = blocks; o; o = o->next_pool)
		വേണ്ടി (e = o->next_free; e-- != o->entries;)
			എങ്കിൽ (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_എണ്ണം;
	എങ്കിൽ (c != last)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, ശൂന്യ, idx, object_എണ്ണം,
				 &pack_idx_opts, pack_data->hash);
	സ്വതന്ത്രം(idx);
	തിരികെ tmpfile;
}

സ്ഥിതം അക്ഷരം *keep_pack(സ്ഥിരം അക്ഷരം *curr_index_name)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം *keep_സന്ദേശം = "fast-import";
	ഘടന strbuf name = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ keep_fd;

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_data->repo,
							    name.buf);
	എങ്കിൽ (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_സന്ദേശം, strlen(keep_സന്ദേശം));
	എങ്കിൽ (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "pack");
	എങ്കിൽ (finalize_object_file(pack_data->repo, pack_data->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "idx");
	എങ്കിൽ (finalize_object_file(pack_data->repo, curr_index_name, name.buf))
		die(_("cannot store index file"));
	സ്വതന്ത്രം((ശൂന്യം *)curr_index_name);
	തിരികെ strbuf_detach(&name, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം unkeep_all_packs(ശൂന്യം)
{
	ഘടന strbuf name = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ k;

	വേണ്ടി (k = 0; k < pack_id; k++) {
		ഘടന packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ loosen_small_pack(സ്ഥിരം ഘടന packed_git *p)
{
	ഘടന child_process unpack = CHILD_PROCESS_INIT;

	എങ്കിൽ (lseek(p->pack_fd, 0, SEEK_SET) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_തള്ളുക(&unpack.args, "unpack-objects");
	എങ്കിൽ (!show_stats)
		strvec_തള്ളുക(&unpack.args, "-q");

	തിരികെ run_command(&unpack);
}

സ്ഥിതം ശൂന്യം end_packfile(ശൂന്യം)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ running;

	എങ്കിൽ (running || !pack_data)
		തിരികെ;

	running = 1;
	clear_delta_base_cache();
	എങ്കിൽ (object_എണ്ണം) {
		ഘടന odb_source_files *files = odb_source_files_downcast(pack_data->repo->objects->source);
		ഘടന packed_git *new_p;
		ഘടന object_id cur_pack_oid;
		അക്ഷരം *idx_name;
		പൂർണ്ണസംഖ്യ i;
		ഘടന branch *b;
		ഘടന tag *t;

		close_pack_windows(pack_data);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_data->pack_fd,
					 pack_data->hash, pack_data->pack_name,
					 object_എണ്ണം, cur_pack_oid.hash,
					 pack_size);

		എങ്കിൽ (object_എണ്ണം <= unpack_limit) {
			എങ്കിൽ (!loosen_small_pack(pack_data)) {
				invalidate_pack_id(pack_id);
				പോകുക discard_pack;
			}
		}

		close(pack_data->pack_fd);
		idx_name = keep_pack(create_index());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		എങ്കിൽ (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		സ്വതന്ത്രം(idx_name);

		/* Print the boundary */
		എങ്കിൽ (pack_edges) {
			fprintf(pack_edges, "%s:", new_p->pack_name);
			വേണ്ടി (i = 0; i < branch_table_sz; i++) {
				വേണ്ടി (b = branch_table[i]; b; b = b->table_next_branch) {
					എങ്കിൽ (b->pack_id == pack_id)
						fprintf(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			വേണ്ടി (t = first_tag; t; t = t->next_tag) {
				എങ്കിൽ (t->pack_id == pack_id)
					fprintf(pack_edges, " %s",
						oid_to_hex(&t->oid));
			}
			fputc('\n', pack_edges);
			fflush(pack_edges);
		}

		pack_id++;
	}
	അല്ലെങ്കിൽ {
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

സ്ഥിതം ശൂന്യം cycle_packfile(ശൂന്യം)
{
	end_packfile();
	start_packfile();
}

സ്ഥിതം പൂർണ്ണസംഖ്യ store_object(
	എണ്ണൽ object_type type,
	ഘടന strbuf *dat,
	ഘടന last_object *last,
	ഘടന object_id *oidout,
	uintmax_t mark)
{
	ഘടന odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	ശൂന്യം *out, *delta;
	ഘടന object_entry *e;
	unsigned അക്ഷരം hdr[96];
	ഘടന object_id oid;
	unsigned long hdrlen, deltalen = 0;
	ഘടന git_hash_ctx c;
	git_zstream s;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	hdrlen = format_object_header((അക്ഷരം *)hdr, വലുപ്പം(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	എങ്കിൽ (oidout)
		oidcpy(oidout, &oid);

	e = തിരുകുക_object(&oid);
	എങ്കിൽ (mark)
		തിരുകുക_mark(&marks, mark, e);
	എങ്കിൽ (e->idx.offset) {
		duplicate_എണ്ണം_by_type[type]++;
		തിരികെ 1;
	}

	വേണ്ടി (ഘടന odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		എങ്കിൽ (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			തുടരുക;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_എണ്ണം_by_type[type]++;
		തിരികെ 1;
	}

	എങ്കിൽ (last && last->data.len && last->data.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		size_t deltalen_st;

		delta_എണ്ണം_attempts_by_type[type]++;
		delta = diff_delta(last->data.buf, last->data.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_size_t_to_ulong(deltalen_st);
	} അല്ലെങ്കിൽ
		delta = ശൂന്യ;

	git_deflate_init(&s, cfg->pack_compression_level);
	എങ്കിൽ (delta) {
		s.next_in = delta;
		s.avail_in = deltalen;
	} അല്ലെങ്കിൽ {
		s.next_in = (ശൂന്യം *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.next_out = out = xmalloc(s.avail_out);
	ആകുമ്പോൾ (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	എങ്കിൽ ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + s.total_out) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + s.total_out) < pack_size) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		എങ്കിൽ (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.next_in = (ശൂന്യം *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.next_out = out = xrealloc(out, s.avail_out);
			ആകുമ്പോൾ (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_end(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.offset = pack_size;
	object_എണ്ണം++;
	object_എണ്ണം_by_type[type]++;

	crc32_begin(pack_file);

	എങ്കിൽ (delta) {
		off_t ofs = e->idx.offset - last->offset;
		unsigned pos = വലുപ്പം(hdr) - 1;

		delta_എണ്ണം_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, വലുപ്പം(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;

		hdr[pos] = ofs & 127;
		ആകുമ്പോൾ (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, വലുപ്പം(hdr) - pos);
		pack_size += വലുപ്പം(hdr) - pos;
	} അല്ലെങ്കിൽ {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, വലുപ്പം(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;
	}

	hashwrite(pack_file, out, s.total_out);
	pack_size += s.total_out;

	e->idx.crc32 = crc32_end(pack_file);

	സ്വതന്ത്രം(out);
	സ്വതന്ത്രം(delta);
	എങ്കിൽ (last) {
		എങ്കിൽ (last->no_swap) {
			last->data = *dat;
		} അല്ലെങ്കിൽ {
			strbuf_swap(&last->data, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം truncate_pack(ഘടന hashfile_checkpoint *checkpoint)
{
	എങ്കിൽ (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_size = checkpoint->offset;
}

സ്ഥിതം ശൂന്യം stream_blob(uintmax_t len, ഘടന object_id *oidout, uintmax_t mark)
{
	ഘടന odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	size_t in_sz = 64 * 1024, out_sz = 64 * 1024;
	unsigned അക്ഷരം *in_buf = xmalloc(in_sz);
	unsigned അക്ഷരം *out_buf = xmalloc(out_sz);
	ഘടന object_entry *e;
	ഘടന object_id oid;
	unsigned long hdrlen;
	off_t offset;
	ഘടന git_hash_ctx c;
	git_zstream s;
	ഘടന hashfile_checkpoint checkpoint;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);
	പൂർണ്ണസംഖ്യ status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	എങ്കിൽ ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + len) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + len) < pack_size)
		cycle_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	offset = checkpoint.offset;

	hdrlen = format_object_header((അക്ഷരം *)out_buf, out_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, out_buf, hdrlen);

	crc32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_in_pack_object_header(out_buf, out_sz, OBJ_BLOB, len);

	s.next_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	ആകുമ്പോൾ (status != Z_STREAM_END) {
		എങ്കിൽ (0 < len && !s.avail_in) {
			size_t cnt = in_sz < len ? in_sz : (size_t)len;
			size_t n = fread(in_buf, 1, cnt, stdin);
			എങ്കിൽ (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.next_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		എങ്കിൽ (!s.avail_out || status == Z_STREAM_END) {
			size_t n = s.next_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_size += n;
			s.next_out = out_buf;
			s.avail_out = out_sz;
		}

		തിരഞ്ഞെടുപ്പ് (status) {
		അവസ്ഥ Z_OK:
		അവസ്ഥ Z_BUF_ERROR:
		അവസ്ഥ Z_STREAM_END:
			തുടരുക;
		സ്ഥിരസ്ഥിതി:
			die(_("unexpected deflate failure: %d"), status);
		}
	}
	git_deflate_end(&s);
	git_hash_final_oid(&oid, &c);

	എങ്കിൽ (oidout)
		oidcpy(oidout, &oid);

	e = തിരുകുക_object(&oid);

	എങ്കിൽ (mark)
		തിരുകുക_mark(&marks, mark, e);

	എങ്കിൽ (e->idx.offset) {
		duplicate_എണ്ണം_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		പോകുക out;
	}

	വേണ്ടി (ഘടന odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		എങ്കിൽ (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			തുടരുക;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_എണ്ണം_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		പോകുക out;
	}

	e->depth = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.offset = offset;
	e->idx.crc32 = crc32_end(pack_file);
	object_എണ്ണം++;
	object_എണ്ണം_by_type[OBJ_BLOB]++;

out:
	സ്വതന്ത്രം(in_buf);
	സ്വതന്ത്രം(out_buf);
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
സ്ഥിതം ശൂന്യം *gfi_unpack_entry(
	ഘടന object_entry *oe,
	unsigned long *sizep)
{
	എണ്ണൽ object_type type;
	size_t size_st = 0;
	ശൂന്യം *data;
	ഘടന packed_git *p = all_packs[oe->pack_id];
	എങ്കിൽ (p == pack_data && p->pack_size < (pack_size + the_hash_algo->rawsz)) {
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
	എങ്കിൽ (sizep)
		*sizep = cast_size_t_to_ulong(size_st);
	തിരികെ data;
}

സ്ഥിതം ശൂന്യം load_വൃക്ഷം(ഘടന വൃക്ഷം_entry *വേര്)
{
	ഘടന object_id *oid = &വേര്->versions[1].oid;
	ഘടന object_entry *myoe;
	ഘടന വൃക്ഷം_content *t;
	unsigned long size;
	അക്ഷരം *buf;
	സ്ഥിരം അക്ഷരം *c;

	വേര്->വൃക്ഷം = t = new_വൃക്ഷം_content(8);
	എങ്കിൽ (is_null_oid(oid))
		തിരികെ;

	myoe = find_object(oid);
	എങ്കിൽ (myoe && myoe->pack_id != MAX_PACK_ID) {
		എങ്കിൽ (myoe->type != OBJ_വൃക്ഷം)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &size);
		എങ്കിൽ (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} അല്ലെങ്കിൽ {
		എണ്ണൽ object_type type;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		എങ്കിൽ (!buf || type != OBJ_വൃക്ഷം)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	ആകുമ്പോൾ (c != (buf + size)) {
		ഘടന വൃക്ഷം_entry *e = new_വൃക്ഷം_entry();

		എങ്കിൽ (t->entry_എണ്ണം == t->entry_capacity)
			വേര്->വൃക്ഷം = t = grow_വൃക്ഷം_content(t, t->entry_എണ്ണം);
		t->entries[t->entry_എണ്ണം++] = e;

		e->വൃക്ഷം = ശൂന്യ;
		c = parse_mode(c, &e->versions[1].mode);
		എങ്കിൽ (!c)
			die(_("corrupt mode in %s"), oid_to_hex(oid));
		e->versions[0].mode = e->versions[1].mode;
		e->name = to_atom(c, strlen(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (unsigned അക്ഷരം *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (unsigned അക്ഷരം *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	സ്വതന്ത്രം(buf);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ tecmp0 (സ്ഥിരം ശൂന്യം *_a, സ്ഥിരം ശൂന്യം *_b)
{
	ഘടന വൃക്ഷം_entry *a = *((ഘടന വൃക്ഷം_entry**)_a);
	ഘടന വൃക്ഷം_entry *b = *((ഘടന വൃക്ഷം_entry**)_b);
	തിരികെ base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ tecmp1 (സ്ഥിരം ശൂന്യം *_a, സ്ഥിരം ശൂന്യം *_b)
{
	ഘടന വൃക്ഷം_entry *a = *((ഘടന വൃക്ഷം_entry**)_a);
	ഘടന വൃക്ഷം_entry *b = *((ഘടന വൃക്ഷം_entry**)_b);
	തിരികെ base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

സ്ഥിതം ശൂന്യം mktree(ഘടന വൃക്ഷം_content *t, പൂർണ്ണസംഖ്യ v, ഘടന strbuf *b)
{
	size_t maxlen = 0;
	unsigned പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (!v)
		QSORT(t->entries, t->entry_എണ്ണം, tecmp0);
	അല്ലെങ്കിൽ
		QSORT(t->entries, t->entry_എണ്ണം, tecmp1);

	വേണ്ടി (i = 0; i < t->entry_എണ്ണം; i++) {
		എങ്കിൽ (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	വേണ്ടി (i = 0; i < t->entry_എണ്ണം; i++) {
		ഘടന വൃക്ഷം_entry *e = t->entries[i];
		എങ്കിൽ (!e->versions[v].mode)
			തുടരുക;
		strbuf_addf(b, "%o %s%c",
			(unsigned പൂർണ്ണസംഖ്യ)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

സ്ഥിതം ശൂന്യം store_വൃക്ഷം(ഘടന വൃക്ഷം_entry *വേര്)
{
	ഘടന വൃക്ഷം_content *t;
	unsigned പൂർണ്ണസംഖ്യ i, j, del;
	ഘടന last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	ഘടന object_entry *le = ശൂന്യ;

	എങ്കിൽ (!is_null_oid(&വേര്->versions[1].oid))
		തിരികെ;

	എങ്കിൽ (!വേര്->വൃക്ഷം)
		load_വൃക്ഷം(വേര്);
	t = വേര്->വൃക്ഷം;

	വേണ്ടി (i = 0; i < t->entry_എണ്ണം; i++) {
		എങ്കിൽ (t->entries[i]->വൃക്ഷം)
			store_വൃക്ഷം(t->entries[i]);
	}

	എങ്കിൽ (!(വേര്->versions[0].mode & NO_DELTA))
		le = find_object(&വേര്->versions[0].oid);
	എങ്കിൽ (S_ISDIR(വേര്->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_വൃക്ഷം);
		lo.data = old_വൃക്ഷം;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_വൃക്ഷം);
	store_object(OBJ_വൃക്ഷം, &new_വൃക്ഷം, &lo, &വേര്->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	വേണ്ടി (i = 0, j = 0, del = 0; i < t->entry_എണ്ണം; i++) {
		ഘടന വൃക്ഷം_entry *e = t->entries[i];
		എങ്കിൽ (e->versions[1].mode) {
			e->versions[0].mode = e->versions[1].mode;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} അല്ലെങ്കിൽ {
			release_വൃക്ഷം_entry(e);
			del++;
		}
	}
	t->entry_എണ്ണം -= del;
}

സ്ഥിതം ശൂന്യം വൃക്ഷം_content_replace(
	ഘടന വൃക്ഷം_entry *വേര്,
	സ്ഥിരം ഘടന object_id *oid,
	സ്ഥിരം uint16_t mode,
	ഘടന വൃക്ഷം_content *newtree)
{
	എങ്കിൽ (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&വേര്->versions[0].oid, the_repository->hash_algo);
	oidcpy(&വേര്->versions[1].oid, oid);
	എങ്കിൽ (വേര്->വൃക്ഷം)
		release_വൃക്ഷം_content_recursive(വേര്->വൃക്ഷം);
	വേര്->വൃക്ഷം = newtree;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ വൃക്ഷം_content_set(
	ഘടന വൃക്ഷം_entry *വേര്,
	സ്ഥിരം അക്ഷരം *p,
	സ്ഥിരം ഘടന object_id *oid,
	സ്ഥിരം uint16_t mode,
	ഘടന വൃക്ഷം_content *subtree)
{
	ഘടന വൃക്ഷം_content *t;
	സ്ഥിരം അക്ഷരം *slash1;
	unsigned പൂർണ്ണസംഖ്യ i, n;
	ഘടന വൃക്ഷം_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	എങ്കിൽ (!n)
		die(_("empty path component found in input"));
	എങ്കിൽ (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	എങ്കിൽ (!വേര്->വൃക്ഷം)
		load_വൃക്ഷം(വേര്);
	t = വേര്->വൃക്ഷം;
	വേണ്ടി (i = 0; i < t->entry_എണ്ണം; i++) {
		e = t->entries[i];
		എങ്കിൽ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			എങ്കിൽ (!*slash1) {
				എങ്കിൽ (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					തിരികെ 0;
				e->versions[1].mode = mode;
				oidcpy(&e->versions[1].oid, oid);
				എങ്കിൽ (e->വൃക്ഷം)
					release_വൃക്ഷം_content_recursive(e->വൃക്ഷം);
				e->വൃക്ഷം = subtree;

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
				എങ്കിൽ (S_ISDIR(e->versions[0].mode))
					e->versions[0].mode |= NO_DELTA;

				oidclr(&വേര്->versions[1].oid, the_repository->hash_algo);
				തിരികെ 1;
			}
			എങ്കിൽ (!S_ISDIR(e->versions[1].mode)) {
				e->വൃക്ഷം = new_വൃക്ഷം_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			എങ്കിൽ (!e->വൃക്ഷം)
				load_വൃക്ഷം(e);
			എങ്കിൽ (വൃക്ഷം_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&വേര്->versions[1].oid, the_repository->hash_algo);
				തിരികെ 1;
			}
			തിരികെ 0;
		}
	}

	എങ്കിൽ (t->entry_എണ്ണം == t->entry_capacity)
		വേര്->വൃക്ഷം = t = grow_വൃക്ഷം_content(t, t->entry_എണ്ണം);
	e = new_വൃക്ഷം_entry();
	e->name = to_atom(p, n);
	e->versions[0].mode = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->entry_എണ്ണം++] = e;
	എങ്കിൽ (*slash1) {
		e->വൃക്ഷം = new_വൃക്ഷം_content(8);
		e->versions[1].mode = S_IFDIR;
		വൃക്ഷം_content_set(e, slash1 + 1, oid, mode, subtree);
	} അല്ലെങ്കിൽ {
		e->വൃക്ഷം = subtree;
		e->versions[1].mode = mode;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&വേര്->versions[1].oid, the_repository->hash_algo);
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ വൃക്ഷം_content_remove(
	ഘടന വൃക്ഷം_entry *വേര്,
	സ്ഥിരം അക്ഷരം *p,
	ഘടന വൃക്ഷം_entry *backup_leaf,
	പൂർണ്ണസംഖ്യ allow_വേര്)
{
	ഘടന വൃക്ഷം_content *t;
	സ്ഥിരം അക്ഷരം *slash1;
	unsigned പൂർണ്ണസംഖ്യ i, n;
	ഘടന വൃക്ഷം_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	എങ്കിൽ (!വേര്->വൃക്ഷം)
		load_വൃക്ഷം(വേര്);

	എങ്കിൽ (!*p && allow_വേര്) {
		e = വേര്;
		പോകുക del_entry;
	}

	t = വേര്->വൃക്ഷം;
	വേണ്ടി (i = 0; i < t->entry_എണ്ണം; i++) {
		e = t->entries[i];
		എങ്കിൽ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			എങ്കിൽ (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				തിരികെ 1;
			എങ്കിൽ (!*slash1 || !S_ISDIR(e->versions[1].mode))
				പോകുക del_entry;
			എങ്കിൽ (!e->വൃക്ഷം)
				load_വൃക്ഷം(e);
			എങ്കിൽ (വൃക്ഷം_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				വേണ്ടി (n = 0; n < e->വൃക്ഷം->entry_എണ്ണം; n++) {
					എങ്കിൽ (e->വൃക്ഷം->entries[n]->versions[1].mode) {
						oidclr(&വേര്->versions[1].oid,
						       the_repository->hash_algo);
						തിരികെ 1;
					}
				}
				backup_leaf = ശൂന്യ;
				പോകുക del_entry;
			}
			തിരികെ 0;
		}
	}
	തിരികെ 0;

del_entry:
	എങ്കിൽ (backup_leaf)
		memcpy(backup_leaf, e, വലുപ്പം(*backup_leaf));
	അല്ലെങ്കിൽ എങ്കിൽ (e->വൃക്ഷം)
		release_വൃക്ഷം_content_recursive(e->വൃക്ഷം);
	e->വൃക്ഷം = ശൂന്യ;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&വേര്->versions[1].oid, the_repository->hash_algo);
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ വൃക്ഷം_content_get(
	ഘടന വൃക്ഷം_entry *വേര്,
	സ്ഥിരം അക്ഷരം *p,
	ഘടന വൃക്ഷം_entry *leaf,
	പൂർണ്ണസംഖ്യ allow_വേര്)
{
	ഘടന വൃക്ഷം_content *t;
	സ്ഥിരം അക്ഷരം *slash1;
	unsigned പൂർണ്ണസംഖ്യ i, n;
	ഘടന വൃക്ഷം_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	എങ്കിൽ (!n && !allow_വേര്)
		die(_("empty path component found in input"));

	എങ്കിൽ (!വേര്->വൃക്ഷം)
		load_വൃക്ഷം(വേര്);

	എങ്കിൽ (!n) {
		e = വേര്;
		പോകുക found_entry;
	}

	t = വേര്->വൃക്ഷം;
	വേണ്ടി (i = 0; i < t->entry_എണ്ണം; i++) {
		e = t->entries[i];
		എങ്കിൽ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			എങ്കിൽ (!*slash1)
				പോകുക found_entry;
			എങ്കിൽ (!S_ISDIR(e->versions[1].mode))
				തിരികെ 0;
			എങ്കിൽ (!e->വൃക്ഷം)
				load_വൃക്ഷം(e);
			തിരികെ വൃക്ഷം_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	തിരികെ 0;

found_entry:
	memcpy(leaf, e, വലുപ്പം(*leaf));
	എങ്കിൽ (e->വൃക്ഷം && is_null_oid(&e->versions[1].oid))
		leaf->വൃക്ഷം = dup_വൃക്ഷം_content(e->വൃക്ഷം);
	അല്ലെങ്കിൽ
		leaf->വൃക്ഷം = ശൂന്യ;
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_branch(ഘടന branch *b)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം = "fast-import";
	ഘടന ref_transaction *transaction;
	ഘടന object_id old_oid;
	ഘടന strbuf err = STRBUF_INIT;
	സ്ഥിതം സ്ഥിരം അക്ഷരം *replace_prefix = "refs/replace/";

	എങ്കിൽ (starts_with(b->name, replace_prefix) &&
	    !strcmp(b->name + strlen(replace_prefix),
		    oid_to_hex(&b->oid))) {
		എങ്കിൽ (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				ശൂന്യ, b->name, ശൂന്യ, 0);
		തിരികെ 0;
	}
	എങ്കിൽ (is_null_oid(&b->oid)) {
		എങ്കിൽ (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					ശൂന്യ, b->name, ശൂന്യ, 0);
		തിരികെ 0;
	}
	എങ്കിൽ (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	എങ്കിൽ (!force_update && !is_null_oid(&old_oid)) {
		ഘടന commit *old_cmit, *new_cmit;
		പൂർണ്ണസംഖ്യ ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		എങ്കിൽ (!old_cmit || !new_cmit)
			തിരികെ error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		എങ്കിൽ (ret < 0)
			exit(128);
		എങ്കിൽ (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			തിരികെ -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	എങ്കിൽ (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   ശൂന്യ, ശൂന്യ, 0, സന്ദേശം, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		തിരികെ -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം dump_branches(ശൂന്യം)
{
	unsigned പൂർണ്ണസംഖ്യ i;
	ഘടന branch *b;

	വേണ്ടി (i = 0; i < branch_table_sz; i++) {
		വേണ്ടി (b = branch_table[i]; b; b = b->table_next_branch)
			failure |= update_branch(b);
	}
}

സ്ഥിതം ശൂന്യം dump_tags(ശൂന്യം)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം = "fast-import";
	ഘടന tag *t;
	ഘടന strbuf ref_name = STRBUF_INIT;
	ഘടന strbuf err = STRBUF_INIT;
	ഘടന ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	എങ്കിൽ (!transaction) {
		failure |= error("%s", err.buf);
		പോകുക cleanup;
	}
	വേണ്ടി (t = first_tag; t; t = t->next_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		എങ്കിൽ (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, ശൂന്യ, ശൂന്യ, ശൂന്യ,
					   0, സന്ദേശം, &err)) {
			failure |= error("%s", err.buf);
			പോകുക cleanup;
		}
	}
	എങ്കിൽ (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

സ്ഥിതം ശൂന്യം dump_marks(ശൂന്യം)
{
	ഘടന lock_file mark_lock = LOCK_INIT;
	FILE *f;

	എങ്കിൽ (!export_marks_file || (import_marks_file && !import_marks_file_done))
		തിരികെ;

	എങ്കിൽ (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		തിരികെ;
	}

	എങ്കിൽ (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		തിരികെ;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	എങ്കിൽ (!f) {
		പൂർണ്ണസംഖ്യ saved_errno = errno;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		തിരികെ;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	എങ്കിൽ (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		തിരികെ;
	}
}

സ്ഥിതം ശൂന്യം തിരുകുക_object_entry(ഘടന mark_set **s, ഘടന object_id *oid, uintmax_t mark)
{
	ഘടന object_entry *e;
	e = find_object(oid);
	എങ്കിൽ (!e) {
		എണ്ണൽ object_type type = odb_read_object_info(the_repository->objects,
							     oid, ശൂന്യ);
		എങ്കിൽ (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = തിരുകുക_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	തിരുകുക_mark(s, mark, e);
}

സ്ഥിതം ശൂന്യം തിരുകുക_oid_entry(ഘടന mark_set **s, ഘടന object_id *oid, uintmax_t mark)
{
	തിരുകുക_mark(s, mark, xmemdupz(oid, വലുപ്പം(*oid)));
}

സ്ഥിതം ശൂന്യം read_mark_file(ഘടന mark_set **s, FILE *f, mark_set_inserter_t inserter)
{
	അക്ഷരം വരി[512];
	ആകുമ്പോൾ (fgets(വരി, വലുപ്പം(വരി), f)) {
		uintmax_t mark;
		അക്ഷരം *end;
		ഘടന object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, വലുപ്പം(oid.hash));

		end = strchr(വരി, '\n');
		എങ്കിൽ (വരി[0] != ':' || !end)
			die(_("corrupt mark line: %s"), വരി);
		*end = 0;
		mark = strtoumax(വരി + 1, &end, 10);
		എങ്കിൽ (!mark || end == വരി + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), വരി);
		inserter(s, &oid, mark);
	}
}

സ്ഥിതം ശൂന്യം read_marks(ശൂന്യം)
{
	FILE *f = fopen(import_marks_file, "r");
	എങ്കിൽ (f)
		;
	അല്ലെങ്കിൽ എങ്കിൽ (import_marks_file_ignore_missing && errno == ENOENT)
		പോകുക done; /* Marks file does not exist */
	അല്ലെങ്കിൽ
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, തിരുകുക_object_entry);
	fclose(f);
done:
	import_marks_file_done = 1;
}


സ്ഥിതം പൂർണ്ണസംഖ്യ read_next_command(ഘടന fast_import_state *state)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ stdin_eof = 0;

	എങ്കിൽ (stdin_eof) {
		unread_command_buf = 0;
		തിരികെ EOF;
	}

	വേണ്ടി (;;) {
		എങ്കിൽ (unread_command_buf) {
			unread_command_buf = 0;
		} അല്ലെങ്കിൽ {
			ഘടന recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			എങ്കിൽ (stdin_eof)
				തിരികെ EOF;

			എങ്കിൽ (!state->seen_data_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			എങ്കിൽ (rc)
				rc_free = rc->next;
			അല്ലെങ്കിൽ {
				rc = cmd_hist.next;
				cmd_hist.next = rc->next;
				cmd_hist.next->prev = &cmd_hist;
				സ്വതന്ത്രം(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_tail;
			rc->next = cmd_hist.prev;
			rc->prev->next = rc;
			cmd_tail = rc;
		}
		എങ്കിൽ (command_buf.buf[0] == '#')
			തുടരുക;
		തിരികെ 0;
	}
}

സ്ഥിതം ശൂന്യം skip_optional_lf(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ term_char = fgetc(stdin);
	എങ്കിൽ (term_char != '\n' && term_char != EOF)
		ungetc(term_char, stdin);
}

സ്ഥിതം ശൂന്യം parse_mark(ഘടന fast_import_state *state)
{
	സ്ഥിരം അക്ഷരം *v;
	എങ്കിൽ (skip_prefix(command_buf.buf, "mark :", &v)) {
		next_mark = strtoumax(v, ശൂന്യ, 10);
		read_next_command(state);
	}
	അല്ലെങ്കിൽ
		next_mark = 0;
}

സ്ഥിതം ശൂന്യം parse_original_identifier(ഘടന fast_import_state *state)
{
	സ്ഥിരം അക്ഷരം *v;
	എങ്കിൽ (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_next_command(state);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_data(ഘടന strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	സ്ഥിരം അക്ഷരം *data;
	strbuf_reset(sb);

	എങ്കിൽ (!skip_prefix(command_buf.buf, "data ", &data))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	എങ്കിൽ (skip_prefix(data, "<<", &data)) {
		അക്ഷരം *term = xstrdup(data);
		size_t term_len = command_buf.len - (data - command_buf.buf);

		വേണ്ടി (;;) {
			എങ്കിൽ (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), term);
			എങ്കിൽ (term_len == command_buf.len
				&& !strcmp(term, command_buf.buf))
				നിർത്തുക;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		സ്വതന്ത്രം(term);
	}
	അല്ലെങ്കിൽ {
		uintmax_t len = strtoumax(data, ശൂന്യ, 10);
		size_t n = 0, length = (size_t)len;

		എങ്കിൽ (limit && limit < len) {
			*len_res = len;
			തിരികെ 0;
		}
		എങ്കിൽ (length < len)
			die(_("data is too large to use in this context"));

		ആകുമ്പോൾ (n < length) {
			size_t s = strbuf_fread(sb, length - n, stdin);
			എങ്കിൽ (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(unsigned long)(length - n));
			n += s;
		}
	}

	skip_optional_lf();
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ validate_raw_date(സ്ഥിരം അക്ഷരം *src, ഘടന strbuf *result, പൂർണ്ണസംഖ്യ strict)
{
	സ്ഥിരം അക്ഷരം *orig_src = src;
	അക്ഷരം *endp;
	unsigned long അക്കം;

	errno = 0;

	അക്കം = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	എങ്കിൽ (errno || endp == src || *endp != ' ')
		തിരികെ -1;

	src = endp + 1;
	എങ്കിൽ (*src != '-' && *src != '+')
		തിരികെ -1;

	അക്കം = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	എങ്കിൽ (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < അക്കം))             /* parsed a broken timezone */
	   )
		തിരികെ -1;

	strbuf_addstr(result, orig_src);
	തിരികെ 0;
}

സ്ഥിതം അക്ഷരം *parse_ident(സ്ഥിരം അക്ഷരം *buf)
{
	സ്ഥിരം അക്ഷരം *ltgt;
	size_t name_len;
	ഘടന strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	എങ്കിൽ (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	എങ്കിൽ (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	എങ്കിൽ (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	എങ്കിൽ (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	എങ്കിൽ (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_add(&ident, buf, name_len);

	തിരഞ്ഞെടുപ്പ് (whenspec) {
	അവസ്ഥ WHENSPEC_RAW:
		എങ്കിൽ (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		നിർത്തുക;
	അവസ്ഥ WHENSPEC_RAW_PERMISSIVE:
		എങ്കിൽ (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		നിർത്തുക;
	അവസ്ഥ WHENSPEC_RFC2822:
		എങ്കിൽ (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		നിർത്തുക;
	അവസ്ഥ WHENSPEC_NOW:
		എങ്കിൽ (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		നിർത്തുക;
	}

	തിരികെ strbuf_detach(&ident, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം parse_and_store_blob(
	ഘടന last_object *last,
	ഘടന object_id *oidout,
	uintmax_t mark)
{
	സ്ഥിതം ഘടന strbuf buf = STRBUF_INIT;
	uintmax_t len;

	എങ്കിൽ (parse_data(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	അല്ലെങ്കിൽ {
		എങ്കിൽ (last) {
			strbuf_release(&last->data);
			last->offset = 0;
			last->depth = 0;
		}
		stream_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

സ്ഥിതം ശൂന്യം parse_new_blob(ഘടന fast_import_state *state)
{
	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&last_blob, ശൂന്യ, next_mark);
}

സ്ഥിതം ശൂന്യം unload_one_branch(ശൂന്യം)
{
	ആകുമ്പോൾ (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = ULONG_MAX;
		ഘടന branch *e, *l = ശൂന്യ, *p = ശൂന്യ;

		വേണ്ടി (e = active_branches; e; e = e->active_next_branch) {
			എങ്കിൽ (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		എങ്കിൽ (p) {
			e = p->active_next_branch;
			p->active_next_branch = e->active_next_branch;
		} അല്ലെങ്കിൽ {
			e = active_branches;
			active_branches = e->active_next_branch;
		}
		e->active = 0;
		e->active_next_branch = ശൂന്യ;
		എങ്കിൽ (e->branch_വൃക്ഷം.വൃക്ഷം) {
			release_വൃക്ഷം_content_recursive(e->branch_വൃക്ഷം.വൃക്ഷം);
			e->branch_വൃക്ഷം.വൃക്ഷം = ശൂന്യ;
		}
		cur_active_branches--;
	}
}

സ്ഥിതം ശൂന്യം load_branch(ഘടന branch *b)
{
	load_വൃക്ഷം(&b->branch_വൃക്ഷം);
	എങ്കിൽ (!b->active) {
		b->active = 1;
		b->active_next_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_എണ്ണം++;
	}
}

സ്ഥിതം unsigned അക്ഷരം convert_അക്കം_notes_to_fanout(uintmax_t അക്കം_notes)
{
	unsigned അക്ഷരം fanout = 0;
	ആകുമ്പോൾ ((അക്കം_notes >>= 8))
		fanout++;
	തിരികെ fanout;
}

സ്ഥിതം ശൂന്യം construct_path_with_fanout(സ്ഥിരം അക്ഷരം *hex_sha1,
		unsigned അക്ഷരം fanout, അക്ഷരം *path)
{
	unsigned പൂർണ്ണസംഖ്യ i = 0, j = 0;
	എങ്കിൽ (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	ആകുമ്പോൾ (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	memcpy(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

സ്ഥിതം uintmax_t do_change_note_fanout(
		ഘടന വൃക്ഷം_entry *orig_വേര്, ഘടന വൃക്ഷം_entry *വേര്,
		അക്ഷരം *hex_oid, unsigned പൂർണ്ണസംഖ്യ hex_oid_len,
		അക്ഷരം *fullpath, unsigned പൂർണ്ണസംഖ്യ fullpath_len,
		unsigned അക്ഷരം fanout)
{
	ഘടന വൃക്ഷം_content *t;
	ഘടന വൃക്ഷം_entry *e, leaf;
	unsigned പൂർണ്ണസംഖ്യ i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t അക്കം_notes = 0;
	ഘടന object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	അക്ഷരം realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	സ്ഥിരം unsigned hexsz = the_hash_algo->hexsz;

	എങ്കിൽ (!വേര്->വൃക്ഷം)
		load_വൃക്ഷം(വേര്);
	t = വേര്->വൃക്ഷം;

	വേണ്ടി (i = 0; t && i < t->entry_എണ്ണം; i++) {
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
		എങ്കിൽ (!e->versions[1].mode ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			തുടരുക;

		/* This _may_ be a note entry, or a subdir containing notes */
		memcpy(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		എങ്കിൽ (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		memcpy(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		എങ്കിൽ (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			എങ്കിൽ (fanout == 0xff) {
				/* Counting mode, no rename */
				അക്കം_notes++;
				തുടരുക;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			എങ്കിൽ (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				അക്കം_notes++;
				തുടരുക;
			}

			/* Rename fullpath to realpath */
			എങ്കിൽ (!വൃക്ഷം_content_remove(orig_വേര്, fullpath, &leaf, 0))
				die(_("failed to remove path %s"), fullpath);
			വൃക്ഷം_content_set(orig_വേര്, realpath,
				&leaf.versions[1].oid,
				leaf.versions[1].mode,
				leaf.വൃക്ഷം);
		} അല്ലെങ്കിൽ എങ്കിൽ (S_ISDIR(e->versions[1].mode)) {
			/* This is a subdir that may contain note entries */
			അക്കം_notes += do_change_note_fanout(orig_വേര്, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = വേര്->വൃക്ഷം;
	}
	തിരികെ അക്കം_notes;
}

സ്ഥിതം uintmax_t change_note_fanout(ഘടന വൃക്ഷം_entry *വേര്,
		unsigned അക്ഷരം fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	അക്ഷരം hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	തിരികെ do_change_note_fanout(വേര്, വേര്, hex_oid, 0, path, 0, fanout);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_mapped_oid_hex(സ്ഥിരം അക്ഷരം *hex, ഘടന object_id *oid, സ്ഥിരം അക്ഷരം **end)
{
	പൂർണ്ണസംഖ്യ algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, വലുപ്പം(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	എങ്കിൽ (algo == GIT_HASH_UNKNOWN)
		തിരികെ -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	എങ്കിൽ (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		എങ്കിൽ (hash_algos[algo].format_id == the_hash_algo->format_id)
			തിരികെ 0;
		തിരികെ -1;
	}
	oidcpy(oid, kh_മൂല്യം(sub_oid_map, it));
	തിരികെ 0;
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
സ്ഥിതം uintmax_t parse_mark_ref(സ്ഥിരം അക്ഷരം *p, അക്ഷരം **endptr)
{
	uintmax_t mark;

	പ്രതിജ്ഞാ(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	എങ്കിൽ (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	തിരികെ mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
സ്ഥിതം uintmax_t parse_mark_ref_eol(സ്ഥിരം അക്ഷരം *p)
{
	അക്ഷരം *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	എങ്കിൽ (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	തിരികെ mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
സ്ഥിതം uintmax_t parse_mark_ref_space(സ്ഥിരം അക്ഷരം **p)
{
	uintmax_t mark;
	അക്ഷരം *end;

	mark = parse_mark_ref(*p, &end);
	എങ്കിൽ (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	തിരികെ mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
സ്ഥിതം ശൂന്യം parse_path(ഘടന strbuf *sb, സ്ഥിരം അക്ഷരം *p, സ്ഥിരം അക്ഷരം **endp,
		പൂർണ്ണസംഖ്യ is_last_field, സ്ഥിരം അക്ഷരം *field)
{
	എങ്കിൽ (*p == '"') {
		എങ്കിൽ (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), field, command_buf.buf);
		എങ്കിൽ (strlen(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), field, command_buf.buf);
	} അല്ലെങ്കിൽ {
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
സ്ഥിതം ശൂന്യം parse_path_eol(ഘടന strbuf *sb, സ്ഥിരം അക്ഷരം *p, സ്ഥിരം അക്ഷരം *field)
{
	സ്ഥിരം അക്ഷരം *end;

	parse_path(sb, p, &end, 1, field);
	എങ്കിൽ (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
സ്ഥിതം ശൂന്യം parse_path_space(ഘടന strbuf *sb, സ്ഥിരം അക്ഷരം *p,
		സ്ഥിരം അക്ഷരം **endp, സ്ഥിരം അക്ഷരം *field)
{
	parse_path(sb, p, endp, 0, field);
	എങ്കിൽ (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

സ്ഥിതം ശൂന്യം file_change_m(ഘടന fast_import_state *state,
			  സ്ഥിരം അക്ഷരം *p,
			  ഘടന branch *b)
{
	സ്ഥിതം ഘടന strbuf path = STRBUF_INIT;
	ഘടന object_entry *oe;
	ഘടന object_id oid;
	uint16_t mode, inline_data = 0;

	p = parse_mode(p, &mode);
	എങ്കിൽ (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	തിരഞ്ഞെടുപ്പ് (mode) {
	അവസ്ഥ 0644:
	അവസ്ഥ 0755:
		mode |= S_IFREG;
	അവസ്ഥ S_IFREG | 0644:
	അവസ്ഥ S_IFREG | 0755:
	അവസ്ഥ S_IFLNK:
	അവസ്ഥ S_IFDIR:
	അവസ്ഥ S_IFGITLINK:
		/* ok */
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	എങ്കിൽ (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = ശൂന്യ; /* not used with inline_data, but makes gcc happy */
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		എങ്കിൽ (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	എങ്കിൽ (S_ISDIR(mode) &&
	    is_empty_വൃക്ഷം_oid(&oid, the_repository->hash_algo) &&
	    *path.buf) {
		വൃക്ഷം_content_remove(&b->branch_വൃക്ഷം, path.buf, ശൂന്യ, 0);
		തിരികെ;
	}

	എങ്കിൽ (S_ISGITLINK(mode)) {
		എങ്കിൽ (inline_data)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		അല്ലെങ്കിൽ എങ്കിൽ (oe) {
			എങ്കിൽ (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} അല്ലെങ്കിൽ എങ്കിൽ (inline_data) {
		എങ്കിൽ (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		ആകുമ്പോൾ (read_next_command(state) != EOF) {
			സ്ഥിരം അക്ഷരം *v;
			എങ്കിൽ (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			അല്ലെങ്കിൽ {
				parse_and_store_blob(&last_blob, &oid, 0);
				നിർത്തുക;
			}
		}
	} അല്ലെങ്കിൽ {
		എണ്ണൽ object_type expected = S_ISDIR(mode) ?
						OBJ_വൃക്ഷം: OBJ_BLOB;
		എണ്ണൽ object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, ശൂന്യ);
		എങ്കിൽ (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(mode) ?  _("tree") : _("blob"),
			    command_buf.buf);
		എങ്കിൽ (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	എങ്കിൽ (!*path.buf) {
		വൃക്ഷം_content_replace(&b->branch_വൃക്ഷം, &oid, mode, ശൂന്യ);
		തിരികെ;
	}

	എങ്കിൽ (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	വൃക്ഷം_content_set(&b->branch_വൃക്ഷം, path.buf, &oid, mode, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം file_change_d(സ്ഥിരം അക്ഷരം *p, ഘടന branch *b)
{
	സ്ഥിതം ഘടന strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	വൃക്ഷം_content_remove(&b->branch_വൃക്ഷം, path.buf, ശൂന്യ, 1);
}

സ്ഥിതം ശൂന്യം file_change_cr(സ്ഥിരം അക്ഷരം *p, ഘടന branch *b, പൂർണ്ണസംഖ്യ rename)
{
	സ്ഥിതം ഘടന strbuf source = STRBUF_INIT;
	സ്ഥിതം ഘടന strbuf dest = STRBUF_INIT;
	ഘടന വൃക്ഷം_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	memset(&leaf, 0, വലുപ്പം(leaf));
	എങ്കിൽ (rename)
		വൃക്ഷം_content_remove(&b->branch_വൃക്ഷം, source.buf, &leaf, 1);
	അല്ലെങ്കിൽ
		വൃക്ഷം_content_get(&b->branch_വൃക്ഷം, source.buf, &leaf, 1);
	എങ്കിൽ (!leaf.versions[1].mode)
		die(_("path %s not in branch"), source.buf);
	എങ്കിൽ (!*dest.buf) {	/* C "path/to/subdir" "" */
		വൃക്ഷം_content_replace(&b->branch_വൃക്ഷം,
			&leaf.versions[1].oid,
			leaf.versions[1].mode,
			leaf.വൃക്ഷം);
		തിരികെ;
	}
	എങ്കിൽ (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	വൃക്ഷം_content_set(&b->branch_വൃക്ഷം, dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.വൃക്ഷം);
}

സ്ഥിതം ശൂന്യം note_change_n(ഘടന fast_import_state *state,
			  സ്ഥിരം അക്ഷരം *p,
			  ഘടന branch *b,
			  unsigned അക്ഷരം *old_fanout)
{
	ഘടന object_entry *oe;
	ഘടന branch *s;
	ഘടന object_id oid, commit_oid;
	അക്ഷരം path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_data = 0;
	unsigned അക്ഷരം new_fanout;

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
	എങ്കിൽ (b->അക്കം_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->അക്കം_notes = change_note_fanout(&b->branch_വൃക്ഷം, 0xff);
		*old_fanout = convert_അക്കം_notes_to_fanout(b->അക്കം_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	എങ്കിൽ (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = ശൂന്യ; /* not used with inline_data, but makes gcc happy */
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		എങ്കിൽ (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	എങ്കിൽ (s) {
		എങ്കിൽ (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} അല്ലെങ്കിൽ എങ്കിൽ (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		ഘടന object_entry *commit_oe = find_mark(marks, commit_mark);
		എങ്കിൽ (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} അല്ലെങ്കിൽ എങ്കിൽ (!repo_get_oid(the_repository, p, &commit_oid)) {
		size_t size;
		അക്ഷരം *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &size,
						   &commit_oid);
		എങ്കിൽ (!buf || size < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		സ്വതന്ത്രം(buf);
	} അല്ലെങ്കിൽ
		die(_("invalid ref name or SHA1 expression: %s"), p);

	എങ്കിൽ (inline_data) {
		read_next_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} അല്ലെങ്കിൽ എങ്കിൽ (oe) {
		എങ്കിൽ (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} അല്ലെങ്കിൽ എങ്കിൽ (!is_null_oid(&oid)) {
		എണ്ണൽ object_type type = odb_read_object_info(the_repository->objects, &oid,
							ശൂന്യ);
		എങ്കിൽ (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		എങ്കിൽ (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	എങ്കിൽ (വൃക്ഷം_content_remove(&b->branch_വൃക്ഷം, path, ശൂന്യ, 0))
		b->അക്കം_notes--;

	എങ്കിൽ (is_null_oid(&oid))
		തിരികെ; /* nothing to insert */

	b->അക്കം_notes++;
	new_fanout = convert_അക്കം_notes_to_fanout(b->അക്കം_notes);
	construct_path_with_fanout(oid_to_hex(&commit_oid), new_fanout, path);
	വൃക്ഷം_content_set(&b->branch_വൃക്ഷം, path, &oid, S_IFREG | 0644, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം file_change_deleteall(ഘടന branch *b)
{
	release_വൃക്ഷം_content_recursive(b->branch_വൃക്ഷം.വൃക്ഷം);
	oidclr(&b->branch_വൃക്ഷം.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_വൃക്ഷം.versions[1].oid, the_repository->hash_algo);
	load_വൃക്ഷം(&b->branch_വൃക്ഷം);
	b->അക്കം_notes = 0;
}

സ്ഥിതം ശൂന്യം parse_from_commit(ഘടന branch *b, അക്ഷരം *buf, unsigned long size)
{
	എങ്കിൽ (!buf || size < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	എങ്കിൽ (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_വൃക്ഷം.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_വൃക്ഷം.versions[0].oid,
	       &b->branch_വൃക്ഷം.versions[1].oid);
}

സ്ഥിതം ശൂന്യം parse_from_existing(ഘടന branch *b)
{
	എങ്കിൽ (is_null_oid(&b->oid)) {
		oidclr(&b->branch_വൃക്ഷം.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_വൃക്ഷം.versions[1].oid, the_repository->hash_algo);
	} അല്ലെങ്കിൽ {
		unsigned long size;
		size_t size_st = 0;
		അക്ഷരം *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &size_st, &b->oid);
		size = cast_size_t_to_ulong(size_st);
		parse_from_commit(b, buf, size);
		സ്വതന്ത്രം(buf);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_objectish(ഘടന fast_import_state *state,
			   ഘടന branch *b,
			   സ്ഥിരം അക്ഷരം *objectish)
{
	ഘടന branch *s;
	ഘടന object_id oid;

	oidcpy(&oid, &b->branch_വൃക്ഷം.versions[1].oid);

	s = lookup_branch(objectish);
	എങ്കിൽ (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	അല്ലെങ്കിൽ എങ്കിൽ (s) {
		ഘടന object_id *t = &s->branch_വൃക്ഷം.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_വൃക്ഷം.versions[0].oid, t);
		oidcpy(&b->branch_വൃക്ഷം.versions[1].oid, t);
	} അല്ലെങ്കിൽ എങ്കിൽ (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		ഘടന object_entry *oe = find_mark(marks, idnum);
		എങ്കിൽ (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		എങ്കിൽ (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			എങ്കിൽ (oe->pack_id != MAX_PACK_ID) {
				unsigned long size;
				അക്ഷരം *buf = gfi_unpack_entry(oe, &size);
				parse_from_commit(b, buf, size);
				സ്വതന്ത്രം(buf);
			} അല്ലെങ്കിൽ
				parse_from_existing(b);
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		എങ്കിൽ (is_null_oid(&b->oid))
			b->delete = 1;
	}
	അല്ലെങ്കിൽ
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	എങ്കിൽ (b->branch_വൃക്ഷം.വൃക്ഷം && !oideq(&oid, &b->branch_വൃക്ഷം.versions[1].oid)) {
		release_വൃക്ഷം_content_recursive(b->branch_വൃക്ഷം.വൃക്ഷം);
		b->branch_വൃക്ഷം.വൃക്ഷം = ശൂന്യ;
	}

	read_next_command(state);
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_from(ഘടന fast_import_state *state, ഘടന branch *b)
{
	സ്ഥിരം അക്ഷരം *from;

	എങ്കിൽ (!skip_prefix(command_buf.buf, "from ", &from))
		തിരികെ 0;

	തിരികെ parse_objectish(state, b, from);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_objectish_with_prefix(ഘടന fast_import_state *state,
				       ഘടന branch *b,
				       സ്ഥിരം അക്ഷരം *prefix)
{
	സ്ഥിരം അക്ഷരം *base;

	എങ്കിൽ (!skip_prefix(command_buf.buf, prefix, &base))
		തിരികെ 0;

	തിരികെ parse_objectish(state, b, base);
}

സ്ഥിതം ഘടന hash_list *parse_merge(ഘടന fast_import_state *state,
				     unsigned പൂർണ്ണസംഖ്യ *എണ്ണം)
{
	ഘടന hash_list *list = ശൂന്യ, **tail = &list, *n;
	സ്ഥിരം അക്ഷരം *from;
	ഘടന branch *s;

	*എണ്ണം = 0;
	ആകുമ്പോൾ (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(വലുപ്പം(*n));
		s = lookup_branch(from);
		എങ്കിൽ (s)
			oidcpy(&n->oid, &s->oid);
		അല്ലെങ്കിൽ എങ്കിൽ (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			ഘടന object_entry *oe = find_mark(marks, idnum);
			എങ്കിൽ (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} അല്ലെങ്കിൽ എങ്കിൽ (!repo_get_oid(the_repository, from, &n->oid)) {
			size_t size;
			അക്ഷരം *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &size, &n->oid);
			എങ്കിൽ (!buf || size < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			സ്വതന്ത്രം(buf);
		} അല്ലെങ്കിൽ
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->next = ശൂന്യ;
		*tail = n;
		tail = &n->next;

		(*എണ്ണം)++;
		read_next_command(state);
	}
	തിരികെ list;
}

ഘടന signature_data {
	അക്ഷരം *hash_algo;      /* "sha1" or "sha256" */
	അക്ഷരം *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	ഘടന strbuf data;   /* The actual signature data */
};

സ്ഥിതം ശൂന്യം parse_one_signature(ഘടന fast_import_state *state,
				ഘടന signature_data *sig,
				സ്ഥിരം അക്ഷരം *v)
{
	അക്ഷരം *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	അക്ഷരം *space = strchr(args, ' ');

	എങ്കിൽ (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), args);
	*space = '\0';

	sig->hash_algo = args;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	എങ്കിൽ (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	എങ്കിൽ (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	എങ്കിൽ (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_next_command(state);
	parse_data(&sig->data, 0, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം discard_one_signature(ഘടന fast_import_state *state)
{
	ഘടന strbuf data = STRBUF_INIT;

	read_next_command(state);
	parse_data(&data, 0, ശൂന്യ);
	strbuf_release(&data);
}

സ്ഥിതം ശൂന്യം add_gpgsig_to_commit(ഘടന strbuf *commit_data,
				 സ്ഥിരം അക്ഷരം *header,
				 ഘടന signature_data *sig)
{
	ഘടന string_list siglines = STRING_LIST_INIT_NODUP;

	എങ്കിൽ (!sig || !sig->hash_algo)
		തിരികെ;

	strbuf_addstr(commit_data, header);
	string_list_split_in_place(&siglines, sig->data.buf, "\n", -1);
	strbuf_add_separated_string_list(commit_data, "\n ", &siglines);
	strbuf_addch(commit_data, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->data);
	സ്വതന്ത്രം(sig->hash_algo);
}

സ്ഥിതം ശൂന്യം store_signature(ഘടന signature_data *stored_sig,
			    ഘടന signature_data *new_sig,
			    സ്ഥിരം അക്ഷരം *hash_type)
{
	എങ്കിൽ (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->data);
		സ്വതന്ത്രം(new_sig->hash_algo);
	} അല്ലെങ്കിൽ {
		*stored_sig = *new_sig;
	}
}

സ്ഥിതം ശൂന്യം import_one_signature(ഘടന fast_import_state *state,
				 ഘടന signature_data *sig_sha1,
				 ഘടന signature_data *sig_sha256,
				 സ്ഥിരം അക്ഷരം *v)
{
	ഘടന signature_data sig = { ശൂന്യ, ശൂന്യ, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	എങ്കിൽ (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	അല്ലെങ്കിൽ
		die(_("parse_one_signature() returned unknown hash algo"));
}

സ്ഥിതം ശൂന്യം finalize_commit_buffer(ഘടന strbuf *new_data,
				   ഘടന signature_data *sig_sha1,
				   ഘടന signature_data *sig_sha256,
				   ഘടന strbuf *സന്ദേശം)
{
	add_gpgsig_to_commit(new_data, "gpgsig ", sig_sha1);
	add_gpgsig_to_commit(new_data, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_data, '\n');
	strbuf_addbuf(new_data, സന്ദേശം);
}

സ്ഥിതം ശൂന്യം warn_invalid_signature(ഘടന signature_check *check,
				   സ്ഥിരം അക്ഷരം *സന്ദേശം, എണ്ണൽ sign_mode mode)
{
	സ്ഥിരം അക്ഷരം *signer = check->signer ? check->signer : _("unknown");
	സ്ഥിരം അക്ഷരം *subject;
	പൂർണ്ണസംഖ്യ subject_len = find_commit_subject(സന്ദേശം, &subject);

	തിരഞ്ഞെടുപ്പ് (mode) {
	അവസ്ഥ SIGN_STRIP_IF_INVALID:
		എങ്കിൽ (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		അല്ലെങ്കിൽ എങ്കിൽ (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		അല്ലെങ്കിൽ
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		നിർത്തുക;
	അവസ്ഥ SIGN_SIGN_IF_INVALID:
		എങ്കിൽ (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		അല്ലെങ്കിൽ എങ്കിൽ (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		അല്ലെങ്കിൽ
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("unsupported signing mode");
	}
}

സ്ഥിതം ശൂന്യം handle_signature_if_invalid(ഘടന strbuf *new_data,
					ഘടന signature_data *sig_sha1,
					ഘടന signature_data *sig_sha256,
					ഘടന strbuf *സന്ദേശം,
					എണ്ണൽ sign_mode mode)
{
	ഘടന strbuf tmp_buf = STRBUF_INIT;
	ഘടന signature_check signature_check = { 0 };
	പൂർണ്ണസംഖ്യ ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_data);
	finalize_commit_buffer(&tmp_buf, sig_sha1, sig_sha256, സന്ദേശം);
	ret = verify_commit_buffer(tmp_buf.buf, tmp_buf.len, &signature_check);

	എങ്കിൽ (ret) {
		എങ്കിൽ (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, സന്ദേശം->buf, mode);

		എങ്കിൽ (mode == SIGN_SIGN_IF_INVALID) {
			ഘടന strbuf signature = STRBUF_INIT;
			ഘടന strbuf payload = STRBUF_INIT;

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
			എങ്കിൽ (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			എങ്കിൽ (sign_buffer(&payload, &signature, signed_commit_keyid,
					SIGN_BUFFER_USE_DEFAULT_താക്കോൽ))
				die(_("failed to sign commit object"));
			add_header_signature(new_data, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_buffer(new_data, ശൂന്യ, ശൂന്യ, സന്ദേശം);
	} അല്ലെങ്കിൽ {
		strbuf_swap(new_data, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

സ്ഥിതം ശൂന്യം parse_new_commit(ഘടന fast_import_state *state, സ്ഥിരം അക്ഷരം *arg)
{
	സ്ഥിതം ഘടന strbuf സന്ദേശം = STRBUF_INIT;
	ഘടന signature_data sig_sha1 = { ശൂന്യ, ശൂന്യ, STRBUF_INIT };
	ഘടന signature_data sig_sha256 = { ശൂന്യ, ശൂന്യ, STRBUF_INIT };
	ഘടന branch *b;
	അക്ഷരം *author = ശൂന്യ;
	അക്ഷരം *committer = ശൂന്യ;
	അക്ഷരം *encoding = ശൂന്യ;
	ഘടന hash_list *merge_list = ശൂന്യ;
	unsigned പൂർണ്ണസംഖ്യ merge_എണ്ണം;
	unsigned അക്ഷരം prev_fanout, new_fanout;
	സ്ഥിരം അക്ഷരം *v;

	b = lookup_branch(arg);
	എങ്കിൽ (!b)
		b = new_branch(arg);

	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	എങ്കിൽ (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_next_command(state);
	}
	എങ്കിൽ (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_next_command(state);
	}
	എങ്കിൽ (!committer)
		die(_("expected committer but didn't get one"));

	ആകുമ്പോൾ (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		തിരഞ്ഞെടുപ്പ് (signed_commit_mode) {

		/* First, modes that don't need the signature to be parsed */
		അവസ്ഥ SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		അവസ്ഥ SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		അവസ്ഥ SIGN_STRIP:
			discard_one_signature(state);
			നിർത്തുക;

		/* Second, modes that parse the signature */
		അവസ്ഥ SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		അവസ്ഥ SIGN_VERBATIM:
		അവസ്ഥ SIGN_STRIP_IF_INVALID:
		അവസ്ഥ SIGN_SIGN_IF_INVALID:
		അവസ്ഥ SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			നിർത്തുക;

		/* Third, BUG */
		സ്ഥിരസ്ഥിതി:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_next_command(state);
	}

	എങ്കിൽ (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_next_command(state);
	}
	parse_data(&സന്ദേശം, 0, ശൂന്യ);
	read_next_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_എണ്ണം);

	/* ensure the branch is active/loaded */
	എങ്കിൽ (!b->branch_വൃക്ഷം.വൃക്ഷം || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	prev_fanout = convert_അക്കം_notes_to_fanout(b->അക്കം_notes);

	/* file_change* */
	ആകുമ്പോൾ (command_buf.len > 0) {
		എങ്കിൽ (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		അല്ലെങ്കിൽ {
			unread_command_buf = 1;
			നിർത്തുക;
		}
		എങ്കിൽ (read_next_command(state) == EOF)
			നിർത്തുക;
	}

	new_fanout = convert_അക്കം_notes_to_fanout(b->അക്കം_notes);
	എങ്കിൽ (new_fanout != prev_fanout)
		b->അക്കം_notes = change_note_fanout(&b->branch_വൃക്ഷം, new_fanout);

	/* build the tree and the commit */
	store_വൃക്ഷം(&b->branch_വൃക്ഷം);
	oidcpy(&b->branch_വൃക്ഷം.versions[0].oid,
	       &b->branch_വൃക്ഷം.versions[1].oid);

	strbuf_reset(&new_data);
	strbuf_addf(&new_data, "tree %s\n",
		oid_to_hex(&b->branch_വൃക്ഷം.versions[1].oid));
	എങ്കിൽ (!is_null_oid(&b->oid))
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&b->oid));
	ആകുമ്പോൾ (merge_list) {
		ഘടന hash_list *next = merge_list->next;
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&merge_list->oid));
		സ്വതന്ത്രം(merge_list);
		merge_list = next;
	}
	strbuf_addf(&new_data,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	എങ്കിൽ (encoding)
		strbuf_addf(&new_data,
			"encoding %s\n",
			encoding);

	എങ്കിൽ ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
	     signed_commit_mode == SIGN_SIGN_IF_INVALID ||
	     signed_commit_mode == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_data, &sig_sha1, &sig_sha256,
					    &സന്ദേശം, signed_commit_mode);
	അല്ലെങ്കിൽ
		finalize_commit_buffer(&new_data, &sig_sha1, &sig_sha256, &സന്ദേശം);

	സ്വതന്ത്രം(author);
	സ്വതന്ത്രം(committer);
	സ്വതന്ത്രം(encoding);

	എങ്കിൽ (!store_object(OBJ_COMMIT, &new_data, ശൂന്യ, &b->oid, next_mark))
		b->pack_id = pack_id;
	b->last_commit = object_എണ്ണം_by_type[OBJ_COMMIT];
}

സ്ഥിതം ശൂന്യം handle_tag_signature_if_invalid(ഘടന strbuf *buf,
					    ഘടന strbuf *സന്ദേശം,
					    size_t sig_offset)
{
	ഘടന strbuf signature = STRBUF_INIT;
	ഘടന strbuf payload = STRBUF_INIT;
	ഘടന signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_add(&payload, സന്ദേശം->buf, sig_offset);
	strbuf_add(&signature, സന്ദേശം->buf + sig_offset, സന്ദേശം->len - sig_offset);

	sigc.payload_type = SIGNATURE_PAYLOAD_TAG;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	എങ്കിൽ (!check_signature(&sigc, signature.buf, signature.len))
		പോകുക out;

	എങ്കിൽ (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(സന്ദേശം, sig_offset);

	എങ്കിൽ (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = ശൂന്യ;
		strbuf_reset(&signature);

		എങ്കിൽ (sign_buffer(&payload, &signature, signed_tag_keyid,
				SIGN_BUFFER_USE_DEFAULT_താക്കോൽ))
			die(_("failed to sign tag object"));

		strbuf_addbuf(സന്ദേശം, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

സ്ഥിതം ശൂന്യം handle_tag_signature(ഘടന strbuf *buf, ഘടന strbuf *സന്ദേശം, സ്ഥിരം അക്ഷരം *name)
{
	size_t sig_offset = parse_signed_buffer(സന്ദേശം->buf, സന്ദേശം->len);

	/* If there is no signature, there is nothing to do. */
	എങ്കിൽ (sig_offset >= സന്ദേശം->len)
		തിരികെ;

	തിരഞ്ഞെടുപ്പ് (signed_tag_mode) {

	/* First, modes that don't change anything */
	അവസ്ഥ SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	അവസ്ഥ SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		നിർത്തുക;

	/* Second, modes that remove the signature */
	അവസ്ഥ SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	അവസ്ഥ SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(സന്ദേശം, sig_offset);
		നിർത്തുക;
	അവസ്ഥ SIGN_ABORT_IF_INVALID:
	അവസ്ഥ SIGN_SIGN_IF_INVALID:
	അവസ്ഥ SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, സന്ദേശം, sig_offset);
		നിർത്തുക;

	/* Third, aborting modes */
	അവസ്ഥ SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	സ്ഥിരസ്ഥിതി:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

സ്ഥിതം ശൂന്യം parse_new_tag(ഘടന fast_import_state *state, സ്ഥിരം അക്ഷരം *arg)
{
	സ്ഥിതം ഘടന strbuf സന്ദേശം = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *from;
	അക്ഷരം *tagger;
	ഘടന branch *s;
	ഘടന tag *t;
	uintmax_t from_mark = 0;
	ഘടന object_id oid;
	എണ്ണൽ object_type type;
	സ്ഥിരം അക്ഷരം *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, വലുപ്പം(ഘടന tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	എങ്കിൽ (last_tag)
		last_tag->next_tag = t;
	അല്ലെങ്കിൽ
		first_tag = t;
	last_tag = t;
	read_next_command(state);
	parse_mark(state);

	/* from ... */
	എങ്കിൽ (!skip_prefix(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	എങ്കിൽ (s) {
		എങ്കിൽ (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} അല്ലെങ്കിൽ എങ്കിൽ (*from == ':') {
		ഘടന object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} അല്ലെങ്കിൽ എങ്കിൽ (!repo_get_oid(the_repository, from, &oid)) {
		ഘടന object_entry *oe = find_object(&oid);
		എങ്കിൽ (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, ശൂന്യ);
			എങ്കിൽ (type < 0)
				die(_("not a valid object: %s"), from);
		} അല്ലെങ്കിൽ
			type = oe->type;
	} അല്ലെങ്കിൽ
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_next_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	എങ്കിൽ (skip_prefix(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_next_command(state);
	} അല്ലെങ്കിൽ
		tagger = ശൂന്യ;

	/* tag payload/message */
	parse_data(&സന്ദേശം, 0, ശൂന്യ);

	/* build the tag object */
	strbuf_reset(&new_data);

	strbuf_addf(&new_data,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_to_hex(&oid), type_name(type), t->name);
	എങ്കിൽ (tagger)
		strbuf_addf(&new_data,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_data, &സന്ദേശം, t->name);

	strbuf_addch(&new_data, '\n');
	strbuf_addbuf(&new_data, &സന്ദേശം);
	സ്വതന്ത്രം(tagger);

	എങ്കിൽ (store_object(OBJ_TAG, &new_data, ശൂന്യ, &t->oid, next_mark))
		t->pack_id = MAX_PACK_ID;
	അല്ലെങ്കിൽ
		t->pack_id = pack_id;
}

സ്ഥിതം ശൂന്യം parse_reset_branch(ഘടന fast_import_state *state, സ്ഥിരം അക്ഷരം *arg)
{
	ഘടന branch *b;
	സ്ഥിരം അക്ഷരം *tag_name;

	b = lookup_branch(arg);
	എങ്കിൽ (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_വൃക്ഷം.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_വൃക്ഷം.versions[1].oid, the_repository->hash_algo);
		എങ്കിൽ (b->branch_വൃക്ഷം.വൃക്ഷം) {
			release_വൃക്ഷം_content_recursive(b->branch_വൃക്ഷം.വൃക്ഷം);
			b->branch_വൃക്ഷം.വൃക്ഷം = ശൂന്യ;
		}
	}
	അല്ലെങ്കിൽ
		b = new_branch(arg);
	read_next_command(state);
	parse_from(state, b);
	എങ്കിൽ (b->delete && skip_prefix(b->name, "refs/tags/", &tag_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		ഘടന tag *t, *prev = ശൂന്യ;
		വേണ്ടി (t = first_tag; t; t = t->next_tag) {
			എങ്കിൽ (!strcmp(t->name, tag_name))
				നിർത്തുക;
			prev = t;
		}
		എങ്കിൽ (t) {
			എങ്കിൽ (prev)
				prev->next_tag = t->next_tag;
			അല്ലെങ്കിൽ
				first_tag = t->next_tag;
			എങ്കിൽ (!t->next_tag)
				last_tag = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	എങ്കിൽ (command_buf.len > 0)
		unread_command_buf = 1;
}

സ്ഥിതം ശൂന്യം cat_blob_write(സ്ഥിരം അക്ഷരം *buf, unsigned long size)
{
	എങ്കിൽ (write_in_full(cat_blob_fd, buf, size) < 0)
		die_errno(_("write to frontend failed"));
}

സ്ഥിതം ശൂന്യം cat_blob(ഘടന object_entry *oe, ഘടന object_id *oid)
{
	ഘടന strbuf വരി = STRBUF_INIT;
	ഘടന iovec iov[3];
	unsigned long size;
	എണ്ണൽ object_type type = 0;
	അക്ഷരം *buf;

	എങ്കിൽ (!oe || oe->pack_id == MAX_PACK_ID) {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
	} അല്ലെങ്കിൽ {
		type = oe->type;
		buf = gfi_unpack_entry(oe, &size);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	എങ്കിൽ (type <= 0) {
		strbuf_reset(&വരി);
		strbuf_addf(&വരി, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(വരി.buf, വരി.len);
		strbuf_release(&വരി);
		സ്വതന്ത്രം(buf);
		തിരികെ;
	}
	എങ്കിൽ (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	എങ്കിൽ (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_to_hex(oid), type_name(type));
	strbuf_reset(&വരി);
	strbuf_addf(&വരി, "%s %s %"PRIuMAX"\n", oid_to_hex(oid),
		    type_name(type), (uintmax_t)size);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_base = വരി.buf;
	iov[0].iov_len = വരി.len;
	iov[1].iov_base = buf;
	iov[1].iov_len = size;
	iov[2].iov_base = (ശൂന്യം *) "\n";
	iov[2].iov_len = 1;

	എങ്കിൽ (writev_in_full(cat_blob_fd, iov, ARRAY_SIZE(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&വരി);
	എങ്കിൽ (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.data, buf, size, size + 1);
		last_blob.depth = oe->depth;
	} അല്ലെങ്കിൽ
		സ്വതന്ത്രം(buf);
}

സ്ഥിതം ശൂന്യം parse_get_mark(ഘടന fast_import_state *state UNUSED,
			   സ്ഥിരം അക്ഷരം *p)
{
	ഘടന object_entry *oe;
	അക്ഷരം output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	എങ്കിൽ (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	എങ്കിൽ (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, വലുപ്പം(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

സ്ഥിതം ശൂന്യം parse_cat_blob(ഘടന fast_import_state *state UNUSED,
			   സ്ഥിരം അക്ഷരം *p)
{
	ഘടന object_entry *oe;
	ഘടന object_id oid;

	/* cat-blob SP <object> LF */
	എങ്കിൽ (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		എങ്കിൽ (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		എങ്കിൽ (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

സ്ഥിതം ഘടന object_entry *dereference(ഘടന object_entry *oe,
					ഘടന object_id *oid)
{
	unsigned long size;
	അക്ഷരം *buf = ശൂന്യ;
	സ്ഥിരം unsigned hexsz = the_hash_algo->hexsz;

	എങ്കിൽ (!oe) {
		എണ്ണൽ object_type type = odb_read_object_info(the_repository->objects,
							     oid, ശൂന്യ);
		എങ്കിൽ (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = തിരുകുക_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	തിരഞ്ഞെടുപ്പ് (oe->type) {
	അവസ്ഥ OBJ_വൃക്ഷം:	/* easy case. */
		തിരികെ oe;
	അവസ്ഥ OBJ_COMMIT:
	അവസ്ഥ OBJ_TAG:
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	എങ്കിൽ (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &size);
	} അല്ലെങ്കിൽ {
		എണ്ണൽ object_type unused;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &size_st);
		size = cast_size_t_to_ulong(size_st);
	}
	എങ്കിൽ (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	തിരഞ്ഞെടുപ്പ് (oe->type) {
	അവസ്ഥ OBJ_TAG:
		എങ്കിൽ (size < hexsz + strlen("object ") ||
		    get_oid_hex(buf + strlen("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		നിർത്തുക;
	അവസ്ഥ OBJ_COMMIT:
		എങ്കിൽ (size < hexsz + strlen("tree ") ||
		    get_oid_hex(buf + strlen("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	സ്വതന്ത്രം(buf);
	തിരികെ find_object(oid);
}

സ്ഥിതം ശൂന്യം തിരുകുക_mapped_mark(uintmax_t mark, ശൂന്യം *object, ശൂന്യം *cbp)
{
	ഘടന object_id *fromoid = object;
	ഘടന object_id *tooid = find_mark(cbp, mark);
	പൂർണ്ണസംഖ്യ ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	എങ്കിൽ (ret == 0)
		തിരികെ;
	kh_മൂല്യം(sub_oid_map, it) = tooid;
}

സ്ഥിതം ശൂന്യം build_mark_map_one(ഘടന mark_set *from, ഘടന mark_set *to)
{
	for_each_mark(from, 0, തിരുകുക_mapped_mark, to);
}

സ്ഥിതം ശൂന്യം build_mark_map(ഘടന string_list *from, ഘടന string_list *to)
{
	ഘടന string_list_item *fromp, *മുകൾ;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		മുകൾ = string_list_lookup(to, fromp->string);
		എങ്കിൽ (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} അല്ലെങ്കിൽ എങ്കിൽ (!മുകൾ || !മുകൾ->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, മുകൾ->util);
	}
}

സ്ഥിതം ഘടന object_entry *parse_treeish_dataref(സ്ഥിരം അക്ഷരം **p)
{
	ഘടന object_id oid;
	ഘടന object_entry *e;

	എങ്കിൽ (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		എങ്കിൽ (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} അല്ലെങ്കിൽ {	/* <sha1> */
		എങ്കിൽ (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		എങ്കിൽ (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	ആകുമ്പോൾ (!e || e->type != OBJ_വൃക്ഷം)
		e = dereference(e, &oid);
	തിരികെ e;
}

സ്ഥിതം ശൂന്യം print_ls(പൂർണ്ണസംഖ്യ mode, സ്ഥിരം unsigned അക്ഷരം *hash, സ്ഥിരം അക്ഷരം *path)
{
	സ്ഥിതം ഘടന strbuf വരി = STRBUF_INIT;

	/* See show_tree(). */
	സ്ഥിരം അക്ഷരം *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? വൃക്ഷം_type :
		blob_type;

	എങ്കിൽ (!mode) {
		/* missing SP path LF */
		strbuf_reset(&വരി);
		strbuf_addstr(&വരി, "missing ");
		quote_c_style(path, &വരി, ശൂന്യ, 0);
		strbuf_addch(&വരി, '\n');
	} അല്ലെങ്കിൽ {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&വരി);
		strbuf_addf(&വരി, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &വരി, ശൂന്യ, 0);
		strbuf_addch(&വരി, '\n');
	}
	cat_blob_write(വരി.buf, വരി.len);
}

സ്ഥിതം ശൂന്യം parse_ls(ഘടന fast_import_state *state UNUSED,
		     സ്ഥിരം അക്ഷരം *p,
		     ഘടന branch *b)
{
	സ്ഥിതം ഘടന strbuf path = STRBUF_INIT;
	ഘടന വൃക്ഷം_entry *വേര് = ശൂന്യ;
	ഘടന വൃക്ഷം_entry leaf = {ശൂന്യ};

	/* ls SP (<tree-ish> SP)? <path> */
	എങ്കിൽ (*p == '"') {
		എങ്കിൽ (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		വേര് = &b->branch_വൃക്ഷം;
	} അല്ലെങ്കിൽ {
		ഘടന object_entry *e = parse_treeish_dataref(&p);
		വേര് = new_വൃക്ഷം_entry();
		oidcpy(&വേര്->versions[1].oid, &e->idx.oid);
		എങ്കിൽ (!is_null_oid(&വേര്->versions[1].oid))
			വേര്->versions[1].mode = S_IFDIR;
		load_വൃക്ഷം(വേര്);
	}
	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	വൃക്ഷം_content_get(വേര്, path.buf, &leaf, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	എങ്കിൽ (S_ISDIR(leaf.versions[1].mode))
		store_വൃക്ഷം(&leaf);

	print_ls(leaf.versions[1].mode, leaf.versions[1].oid.hash, path.buf);
	എങ്കിൽ (leaf.വൃക്ഷം)
		release_വൃക്ഷം_content_recursive(leaf.വൃക്ഷം);
	എങ്കിൽ (!b || വേര് != &b->branch_വൃക്ഷം)
		release_വൃക്ഷം_entry(വേര്);
}

സ്ഥിതം ശൂന്യം checkpoint(ശൂന്യം)
{
	checkpoint_requested = 0;
	എങ്കിൽ (object_എണ്ണം) {
		cycle_packfile();
	}
	dump_branches();
	dump_tags();
	dump_marks();
}

സ്ഥിതം ശൂന്യം parse_checkpoint(ഘടന fast_import_state *state UNUSED)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

സ്ഥിതം ശൂന്യം parse_progress(ഘടന fast_import_state *state UNUSED)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

സ്ഥിതം ശൂന്യം parse_alias(ഘടന fast_import_state *state)
{
	ഘടന object_entry *e;
	ഘടന branch b;

	skip_optional_lf();
	read_next_command(state);

	/* mark ... */
	parse_mark(state);
	എങ്കിൽ (!next_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, വലുപ്പം(b));
	എങ്കിൽ (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	പ്രതിജ്ഞാ(e);
	തിരുകുക_mark(&marks, next_mark, e);
}

സ്ഥിതം അക്ഷരം* make_fast_import_path(ഘടന fast_import_state *state,
				   സ്ഥിരം അക്ഷരം *path)
{
	എങ്കിൽ (!relative_marks_paths || is_absolute_path(path))
		തിരികെ prefix_filename(state->prefix, path);
	തിരികെ repo_git_path(the_repository, "info/fast-import/%s", path);
}

സ്ഥിതം ശൂന്യം option_import_marks(ഘടന fast_import_state *state,
				സ്ഥിരം അക്ഷരം *marks,
				പൂർണ്ണസംഖ്യ from_stream,
				പൂർണ്ണസംഖ്യ ignore_missing)
{
	എങ്കിൽ (import_marks_file) {
		എങ്കിൽ (from_stream)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		എങ്കിൽ(!import_marks_file_from_stream)
			read_marks();
	}

	സ്വതന്ത്രം(import_marks_file);
	import_marks_file = make_fast_import_path(state, marks);
	import_marks_file_from_stream = from_stream;
	import_marks_file_ignore_missing = ignore_missing;
}

സ്ഥിതം ശൂന്യം option_date_format(സ്ഥിരം അക്ഷരം *fmt)
{
	എങ്കിൽ (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	അല്ലെങ്കിൽ
		die(_("unknown --date-format argument %s"), fmt);
}

സ്ഥിതം unsigned long ulong_arg(സ്ഥിരം അക്ഷരം *option, സ്ഥിരം അക്ഷരം *arg)
{
	അക്ഷരം *endptr;
	unsigned long rv = strtoul(arg, &endptr, 0);
	എങ്കിൽ (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	തിരികെ rv;
}

സ്ഥിതം ശൂന്യം option_depth(സ്ഥിരം അക്ഷരം *depth)
{
	max_depth = ulong_arg("--depth", depth);
	എങ്കിൽ (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

സ്ഥിതം ശൂന്യം option_active_branches(സ്ഥിരം അക്ഷരം *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

സ്ഥിതം ശൂന്യം option_export_marks(ഘടന fast_import_state *state,
				സ്ഥിരം അക്ഷരം *marks)
{
	സ്വതന്ത്രം(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

സ്ഥിതം ശൂന്യം option_cat_blob_fd(ഘടന fast_import_state *state UNUSED,
			       സ്ഥിരം അക്ഷരം *fd)
{
	unsigned long n = ulong_arg("--cat-blob-fd", fd);
	എങ്കിൽ (n > (unsigned long) INT_MAX)
		die(_("--cat-blob-fd cannot exceed %d"), INT_MAX);
	cat_blob_fd = (പൂർണ്ണസംഖ്യ) n;
}

സ്ഥിതം ശൂന്യം option_export_pack_edges(ഘടന fast_import_state *state,
				     സ്ഥിരം അക്ഷരം *edges)
{
	അക്ഷരം *fn = prefix_filename(state->prefix, edges);
	എങ്കിൽ (pack_edges)
		fclose(pack_edges);
	pack_edges = xfopen(fn, "a");
	സ്വതന്ത്രം(fn);
}

സ്ഥിതം ശൂന്യം option_rewrite_submodules(ഘടന fast_import_state *state,
				      സ്ഥിരം അക്ഷരം *arg,
				      ഘടന string_list *list)
{
	ഘടന mark_set *ms;
	FILE *fp;
	അക്ഷരം *s = xstrdup(arg);
	അക്ഷരം *f = strchr(s, ':');
	എങ്കിൽ (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = prefix_filename(state->prefix, f);
	fp = fopen(f, "r");
	എങ്കിൽ (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, തിരുകുക_oid_entry);
	fclose(fp);
	സ്വതന്ത്രം(f);

	string_list_തിരുകുക(list, s)->util = ms;

	സ്വതന്ത്രം(s);
}

സ്ഥിതം ശൂന്യം option_max_pack_size(സ്ഥിരം അക്ഷരം *arg)
{
	unsigned long v;

	എങ്കിൽ (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	എങ്കിൽ (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} അല്ലെങ്കിൽ എങ്കിൽ (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

സ്ഥിതം ശൂന്യം option_big_file_threshold(സ്ഥിരം അക്ഷരം *arg)
{
	unsigned long v;

	എങ്കിൽ (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

സ്ഥിതം ശൂന്യം option_signed_commits(സ്ഥിരം അക്ഷരം *arg)
{
	എങ്കിൽ (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

സ്ഥിതം ശൂന്യം option_signed_tags(സ്ഥിരം അക്ഷരം *arg)
{
	എങ്കിൽ (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

സ്ഥിതം ശൂന്യം option_quiet(ശൂന്യം)
{
	show_stats = 0;
	quiet = 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_one_option(ഘടന fast_import_state *state, സ്ഥിരം അക്ഷരം *option)
{
	എങ്കിൽ (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_size(option);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(option, "big-file-threshold=", &option)) {
		option_big_file_threshold(option);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(option, "depth=", &option)) {
		option_depth(option);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(option, "active-branches=", &option)) {
		option_active_branches(option);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(option, "export-pack-edges=", &option)) {
		option_export_pack_edges(state, option);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(option, "signed-tags=", &option)) {
		option_signed_tags(option);
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(option, "quiet")) {
		option_quiet();
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(option, "stats")) {
		show_stats = 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} അല്ലെങ്കിൽ {
		തിരികെ 0;
	}

	തിരികെ 1;
}

സ്ഥിതം ശൂന്യം check_unsafe_feature(ഘടന fast_import_state *state,
				 സ്ഥിരം അക്ഷരം *feature)
{
	എങ്കിൽ (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_one_feature(ഘടന fast_import_state *state,
			     സ്ഥിരം അക്ഷരം *feature)
{
	സ്ഥിരം അക്ഷരം *arg;

	എങ്കിൽ (skip_prefix(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_to);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(feature, "force")) {
		force_update = 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} അല്ലെങ്കിൽ {
		തിരികെ 0;
	}

	തിരികെ 1;
}

സ്ഥിതം ശൂന്യം parse_feature(ഘടന fast_import_state *state, സ്ഥിരം അക്ഷരം *feature)
{
	എങ്കിൽ (state->seen_data_command)
		die(_("got feature command '%s' after data command"), feature);

	എങ്കിൽ (parse_one_feature(state, feature))
		തിരികെ;

	die(_("this version of fast-import does not support feature %s."), feature);
}

സ്ഥിതം ശൂന്യം parse_option(ഘടന fast_import_state *state, സ്ഥിരം അക്ഷരം *option)
{
	എങ്കിൽ (state->seen_data_command)
		die(_("got option command '%s' after data command"), option);

	എങ്കിൽ (parse_one_option(state, option))
		തിരികെ;

	die(_("this version of fast-import does not support option: %s"), option);
}

സ്ഥിതം ശൂന്യം git_pack_config(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ indexversion_മൂല്യം;
	പൂർണ്ണസംഖ്യ limit;
	unsigned long packsizelimit_മൂല്യം;

	എങ്കിൽ (!repo_config_get_ulong(the_repository, "pack.depth", &max_depth)) {
		എങ്കിൽ (max_depth > MAX_DEPTH)
			max_depth = MAX_DEPTH;
	}
	എങ്കിൽ (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_മൂല്യം)) {
		pack_idx_opts.version = indexversion_മൂല്യം;
		എങ്കിൽ (pack_idx_opts.version > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.version);
	}
	എങ്കിൽ (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_മൂല്യം))
		max_packsize = packsizelimit_മൂല്യം;

	എങ്കിൽ (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &limit))
		unpack_limit = limit;
	അല്ലെങ്കിൽ എങ്കിൽ (!repo_config_get_int(the_repository, "transfer.unpacklimit", &limit))
		unpack_limit = limit;

	repo_config(the_repository, git_default_config, ശൂന്യ);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	ശൂന്യ
};

സ്ഥിതം ശൂന്യം parse_argv(ഘടന fast_import_state *state)
{
	പൂർണ്ണസംഖ്യ argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	എങ്കിൽ (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_data_command = 1;
	എങ്കിൽ (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_date_format(സ്ഥിരം ഘടന option *opt UNUSED,
				    സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_export_pack_edges(സ്ഥിരം ഘടന option *opt,
					  സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->മൂല്യം, arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_max_pack_size(സ്ഥിരം ഘടന option *opt UNUSED,
				      സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_size(arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_big_file_threshold(സ്ഥിരം ഘടന option *opt UNUSED,
					   സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_signed_commits(സ്ഥിരം ഘടന option *opt UNUSED,
				       സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_signed_tags(സ്ഥിരം ഘടന option *opt UNUSED,
				    സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_rewrite_submodules_from(സ്ഥിരം ഘടന option *opt,
						സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->മൂല്യം, arg, &sub_marks_from);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_rewrite_submodules_to(സ്ഥിരം ഘടന option *opt,
					      സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->മൂല്യം, arg, &sub_marks_to);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_cat_blob_fd(സ്ഥിരം ഘടന option *opt,
				    സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->മൂല്യം, arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_import_marks(സ്ഥിരം ഘടന option *opt,
				     സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->മൂല്യം, arg, 0, 0);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_import_marks_if_exists(സ്ഥിരം ഘടന option *opt,
					       സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->മൂല്യം, arg, 0, 1);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_export_marks(സ്ഥിരം ഘടന option *opt,
				     സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->മൂല്യം, arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_depth(സ്ഥിരം ഘടന option *opt UNUSED,
			      സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_active_branches(സ്ഥിരം ഘടന option *opt UNUSED,
					സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_quiet(സ്ഥിരം ഘടന option *opt UNUSED,
			      സ്ഥിരം അക്ഷരം *arg UNUSED, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_fast_import(പൂർണ്ണസംഖ്യ argc,
		    സ്ഥിരം അക്ഷരം **argv,
		    സ്ഥിരം അക്ഷരം *prefix,
		    ഘടന repository *repo)
{
	ഘടന fast_import_state state;

	ഘടന option fast_import_options[] = {
		OPT_GROUP(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", ശൂന്യ, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", ശൂന്യ, ശൂന്യ,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", ശൂന്യ, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_size),
		OPT_CALLBACK_F(0, "big-file-threshold", ശൂന്യ, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_threshold),
		OPT_CALLBACK_F(0, "depth", ശൂന്യ, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_depth),
		OPT_CALLBACK_F(0, "active-branches", ശൂന്യ, N_("n"),
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
		OPT_CALLBACK_F(0, "signed-commits", ശൂന്യ, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", ശൂന്യ, N_("mode"),
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
	CALLOC_ARRAY(avail_വൃക്ഷം_table, avail_വൃക്ഷം_table_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, വലുപ്പം(ഘടന mark_set));

	hashmap_init(&object_table, object_entry_hashcmp, ശൂന്യ, 0);

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
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];
		എങ്കിൽ (*arg != '-' || !strcmp(arg, "--"))
			നിർത്തുക;
		എങ്കിൽ (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * വലുപ്പം(*rc_free));
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < (cmd_save - 1); i++)
		rc_free[i].next = &rc_free[i + 1];
	rc_free[cmd_save - 1].next = ശൂന്യ;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	ആകുമ്പോൾ (read_next_command(&state) != EOF) {
		സ്ഥിരം അക്ഷരം *v;
		എങ്കിൽ (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, ശൂന്യ);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp("done", command_buf.buf))
			നിർത്തുക;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		അല്ലെങ്കിൽ എങ്കിൽ (starts_with(command_buf.buf, "progress "))
			parse_progress(&state);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		അല്ലെങ്കിൽ എങ്കിൽ (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		അല്ലെങ്കിൽ
			die(_("unsupported command: %s"), command_buf.buf);

		എങ്കിൽ (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	എങ്കിൽ (!state.seen_data_command)
		parse_argv(&state);

	എങ്കിൽ (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	end_packfile();

	dump_branches();
	dump_tags();
	unkeep_all_packs();
	dump_marks();

	എങ്കിൽ (pack_edges)
		fclose(pack_edges);

	എങ്കിൽ (show_stats) {
		uintmax_t total_എണ്ണം = 0, duplicate_എണ്ണം = 0;
		വേണ്ടി (size_t i = 0; i < ARRAY_SIZE(object_എണ്ണം_by_type); i++)
			total_എണ്ണം += object_എണ്ണം_by_type[i];
		വേണ്ടി (size_t i = 0; i < ARRAY_SIZE(duplicate_എണ്ണം_by_type); i++)
			duplicate_എണ്ണം += duplicate_എണ്ണം_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_എണ്ണം);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", total_എണ്ണം, duplicate_എണ്ണം);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_എണ്ണം_by_type[OBJ_BLOB], duplicate_എണ്ണം_by_type[OBJ_BLOB], delta_എണ്ണം_by_type[OBJ_BLOB], delta_എണ്ണം_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_എണ്ണം_by_type[OBJ_വൃക്ഷം], duplicate_എണ്ണം_by_type[OBJ_വൃക്ഷം], delta_എണ്ണം_by_type[OBJ_വൃക്ഷം], delta_എണ്ണം_attempts_by_type[OBJ_വൃക്ഷം]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_എണ്ണം_by_type[OBJ_COMMIT], duplicate_എണ്ണം_by_type[OBJ_COMMIT], delta_എണ്ണം_by_type[OBJ_COMMIT], delta_എണ്ണം_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_എണ്ണം_by_type[OBJ_TAG], duplicate_എണ്ണം_by_type[OBJ_TAG], delta_എണ്ണം_by_type[OBJ_TAG], delta_എണ്ണം_attempts_by_type[OBJ_TAG]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_എണ്ണം, branch_load_എണ്ണം);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->shift) * 1024, marks_set_എണ്ണം);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (വൃക്ഷം_entry_allocd + fi_mem_pool.pool_alloc + alloc_എണ്ണം*വലുപ്പം(ഘടന object_entry))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (unsigned long)((വൃക്ഷം_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_എണ്ണം*വലുപ്പം(ഘടന object_entry))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	തിരികെ failure ? 1 : 0;
}
