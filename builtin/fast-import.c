#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "abspath.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "config.h"
#include "lockfile.h"
#include "object.h"
#include "blob.h"
#include "tree.h"
#include "commit.h"
#include "delta.h"
#include "pack.h"
#include "path.h"
#include "read-cache-ll.h"
#include "refs.h"
#include "csum-file.h"
#include "quote.h"
#include "dir.h"
#include "run-command.h"
#include "packfile.h"
#include "object-file.h"
#include "object-name.h"
#include "odb.h"
#include "mem-pool.h"
#include "commit-reach.h"
#include "khash.h"
#include "date.h"
#include "gpg-interface.h"
#include "parse-options.h"

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

sa_nracanaa object_entry {
	sa_nracanaa pack_idx_entry idx;
	sa_nracanaa hashmap_entry ent;
	uint32_t type : TYPE_BITS,
		pack_id : PACK_ID_BITS,
		depth : DEPTH_BITS;
};

static ཧྲིལ object_entry_hashcmp(const void *map_data UNUSED,
				const sa_nracanaa hashmap_entry *eptr,
				const sa_nracanaa hashmap_entry *entry_or_key,
				const void *keydata)
{
	const sa_nracanaa object_id *oid = keydata;
	const sa_nracanaa object_entry *e1, *e2;

	e1 = container_of(eptr, const sa_nracanaa object_entry, ent);
	གལ་སྲིད (oid)
		ལོག oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_key, const sa_nracanaa object_entry, ent);
	ལོག oidcmp(&e1->idx.oid, &e2->idx.oid);
}

sa_nracanaa object_entry_pool {
	sa_nracanaa object_entry_pool *next_pool;
	sa_nracanaa object_entry *next_free;
	sa_nracanaa object_entry *end;
	sa_nracanaa object_entry entries[FLEX_ARRAY]; /* more */
};

sa_nracanaa mark_set {
	sa_ngha {
		sa_nracanaa object_id *oids[1024];
		sa_nracanaa object_entry *marked[1024];
		sa_nracanaa mark_set *sets[1024];
	} data;
	unsigned ཧྲིལ shift;
};

sa_nracanaa last_object {
	sa_nracanaa strbuf data;
	off_t offset;
	unsigned ཧྲིལ depth;
	unsigned no_swap : 1;
};

sa_nracanaa atom_str {
	sa_nracanaa atom_str *next_atom;
	unsigned short str_len;
	var_na str_dat[FLEX_ARRAY]; /* more */
};

sa_nracanaa tree_content;
sa_nracanaa tree_entry {
	sa_nracanaa tree_content *tree;
	sa_nracanaa atom_str *name;
	sa_nracanaa tree_entry_ms {
		uint16_t mode;
		sa_nracanaa object_id oid;
	} versions[2];
};

sa_nracanaa tree_content {
	unsigned ཧྲིལ entry_capacity; /* must match avail_tree_content */
	unsigned ཧྲིལ entry_count;
	unsigned ཧྲིལ delta_depth;
	sa_nracanaa tree_entry *entries[FLEX_ARRAY]; /* more */
};

sa_nracanaa avail_tree_content {
	unsigned ཧྲིལ entry_capacity; /* must match tree_content */
	sa_nracanaa avail_tree_content *next_avail;
};

sa_nracanaa branch {
	sa_nracanaa branch *table_next_branch;
	sa_nracanaa branch *active_next_branch;
	const var_na *name;
	sa_nracanaa tree_entry branch_tree;
	uintmax_t last_commit;
	uintmax_t num_notes;
	unsigned active : 1;
	unsigned delete : 1;
	unsigned pack_id : PACK_ID_BITS;
	sa_nracanaa object_id oid;
};

sa_nracanaa tag {
	sa_nracanaa tag *next_tag;
	const var_na *name;
	unsigned ཧྲིལ pack_id;
	sa_nracanaa object_id oid;
};

sa_nracanaa hash_list {
	sa_nracanaa hash_list *next;
	sa_nracanaa object_id oid;
};

typedef ga_nanaa {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

sa_nracanaa recent_command {
	sa_nracanaa recent_command *prev;
	sa_nracanaa recent_command *next;
	var_na *buf;
};

typedef void (*mark_set_inserter_t)(sa_nracanaa mark_set **s, sa_nracanaa object_id *oid, uintmax_t mark);
typedef void (*each_mark_fn_t)(uintmax_t mark, void *obj, void *cbp);

/* Configured limits on output */
static unsigned long max_depth = 50;
static off_t max_packsize;
static ཧྲིལ unpack_limit = 100;
static ཧྲིལ force_update;

/* Stats and misc. counters */
static uintmax_t alloc_count;
static uintmax_t marks_set_count;
static uintmax_t object_count_by_type[1 << TYPE_BITS];
static uintmax_t duplicate_count_by_type[1 << TYPE_BITS];
static uintmax_t delta_count_by_type[1 << TYPE_BITS];
static uintmax_t delta_count_attempts_by_type[1 << TYPE_BITS];
static unsigned long object_count;
static unsigned long branch_count;
static unsigned long branch_load_count;
static ཧྲིལ failure;
static FILE *pack_edges;
static ཧྲིལ show_stats = 1;
static ཧྲིལ quiet;
static ga_nanaa sign_mode signed_tag_mode = SIGN_VERBATIM;
static ga_nanaa sign_mode signed_commit_mode = SIGN_VERBATIM;
static const var_na *signed_commit_keyid;
static const var_na *signed_tag_keyid;

/* Memory pools */
static sa_nracanaa mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - parimaa_na(sa_nracanaa mp_block),
};

/* Atom management */
static unsigned ཧྲིལ atom_table_sz = 4451;
static unsigned ཧྲིལ atom_cnt;
static sa_nracanaa atom_str **atom_table;

/* The .pack file being generated */
static sa_nracanaa pack_idx_option pack_idx_opts;
static unsigned ཧྲིལ pack_id;
static sa_nracanaa hashfile *pack_file;
static sa_nracanaa packed_git *pack_data;
static sa_nracanaa packed_git **all_packs;
static off_t pack_size;

/* Table of objects we've written. */
static unsigned ཧྲིལ object_entry_alloc = 5000;
static sa_nracanaa object_entry_pool *blocks;
static sa_nracanaa hashmap object_table;
static sa_nracanaa mark_set *marks;
static var_na *export_marks_file;
static var_na *import_marks_file;
static ཧྲིལ import_marks_file_from_stream;
static ཧྲིལ import_marks_file_ignore_missing;
static ཧྲིལ import_marks_file_done;
static ཧྲིལ relative_marks_paths;

/* Our last blob */
static sa_nracanaa last_object last_blob = {
	.data = STRBUF_INIT,
 };

/* Tree management */
static unsigned ཧྲིལ tree_entry_alloc = 1000;
static void *avail_tree_entry;
static unsigned ཧྲིལ avail_tree_table_sz = 100;
static sa_nracanaa avail_tree_content **avail_tree_table;
static size_t tree_entry_allocd;
static sa_nracanaa strbuf old_tree = STRBUF_INIT;
static sa_nracanaa strbuf new_tree = STRBUF_INIT;

/* Branch data */
static unsigned long max_active_branches = 5;
static unsigned long cur_active_branches;
static unsigned long branch_table_sz = 1039;
static sa_nracanaa branch **branch_table;
static sa_nracanaa branch *active_branches;

/* Tag data */
static sa_nracanaa tag *first_tag;
static sa_nracanaa tag *last_tag;

/* Input stream parsing */
static whenspec_type whenspec = WHENSPEC_RAW;
static sa_nracanaa strbuf command_buf = STRBUF_INIT;
static ཧྲིལ unread_command_buf;
static sa_nracanaa recent_command cmd_hist = {
	.prev = &cmd_hist,
	.next = &cmd_hist,
};
static sa_nracanaa recent_command *cmd_tail = &cmd_hist;
static sa_nracanaa recent_command *rc_free;
static unsigned ཧྲིལ cmd_save = 100;
static uintmax_t next_mark;
static sa_nracanaa strbuf new_data = STRBUF_INIT;
static ཧྲིལ require_explicit_termination;

/* Signal handling */
static ca_ncala sig_atomic_t checkpoint_requested;

/* Submodule marks */
static sa_nracanaa string_list sub_marks_from = STRING_LIST_INIT_DUP;
static sa_nracanaa string_list sub_marks_to = STRING_LIST_INIT_DUP;
static kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
static ཧྲིལ cat_blob_fd = STDOUT_FILENO;

/* Command state */
sa_nracanaa fast_import_state {
	ཧྲིལ argc;
	const var_na **argv;
	const var_na *prefix;
	ཧྲིལ seen_data_command;
	ཧྲིལ allow_unsafe_features;
	sa_nracanaa option *option;
};

static void fast_import_state_init(sa_nracanaa fast_import_state *state,
				   ཧྲིལ argc, const var_na **argv,
				   const var_na *prefix, sa_nracanaa option *option)
{
	memset(state, 0, parimaa_na(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

static void parse_argv(sa_nracanaa fast_import_state *state);
static void parse_get_mark(sa_nracanaa fast_import_state *state, const var_na *p);
static void parse_cat_blob(sa_nracanaa fast_import_state *state, const var_na *p);
static void parse_ls(sa_nracanaa fast_import_state *state,
		     const var_na *p,
		     sa_nracanaa branch *b);

static void for_each_mark(sa_nracanaa mark_set *m, uintmax_t base, each_mark_fn_t callback, void *p)
{
	uintmax_t k;
	གལ་སྲིད (m->shift) {
		དོན་ལུ (k = 0; k < 1024; k++) {
			གལ་སྲིད (m->data.sets[k])
				for_each_mark(m->data.sets[k], base + (k << m->shift), callback, p);
		}
	} དེ་མིན {
		དོན་ལུ (k = 0; k < 1024; k++) {
			གལ་སྲིད (m->data.marked[k])
				callback(base + k, m->data.marked[k], p);
		}
	}
}

static void dump_marks_fn(uintmax_t mark, void *object, void *cbp) {
	sa_nracanaa object_entry *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

static void write_branch_report(FILE *rpt, sa_nracanaa branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	གལ་སྲིད (b->active)
		fputs(" active", rpt);
	གལ་སྲིད (b->branch_tree.tree)
		fputs(" loaded", rpt);
	གལ་སྲིད (is_null_oid(&b->branch_tree.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_to_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_to_hex(&b->branch_tree.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_to_hex(&b->branch_tree.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->last_commit);

	fputs("  last pack   : ", rpt);
	གལ་སྲིད (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

static void write_crash_report(const var_na *err)
{
	var_na *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	sa_nracanaa branch *b;
	unsigned long lu;
	sa_nracanaa recent_command *rc;

	གལ་སྲིད (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		free(loc);
		ལོག;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(time(NULL), 0, DATE_MODE(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	དོན་ལུ (rc = cmd_hist.next; rc != &cmd_hist; rc = rc->next) {
		གལ་སྲིད (rc->next == &cmd_hist)
			fputs("* ", rpt);
		དེ་མིན
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
	དོན་ལུ (b = active_branches, lu = 0; b; b = b->active_next_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	དོན་ལུ (lu = 0; lu < branch_table_sz; lu++) {
		དོན་ལུ (b = branch_table[lu]; b; b = b->table_next_branch)
			write_branch_report(rpt, b);
	}

	གལ་སྲིད (first_tag) {
		sa_nracanaa tag *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		དོན་ལུ (tg = first_tag; tg; tg = tg->next_tag) {
			fputs(oid_to_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	གལ་སྲིད (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	དེ་མིན
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	free(loc);
}

static void end_packfile(void);
static void unkeep_all_packs(void);
static void dump_marks(void);

static NORETURN void die_nicely(const var_na *err, va_list params)
{
	va_list cp;
	static ཧྲིལ zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	གལ་སྲིད (!zombie) {
		var_na message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, parimaa_na(message), err, cp);
		write_crash_report(message);
		end_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	exit(128);
}

#ifndef SIGUSR1	/* Windows, for example */

static void set_checkpoint_signal(void)
{
}

#དེ་མིན

static void checkpoint_signal(ཧྲིལ signo UNUSED)
{
	checkpoint_requested = 1;
}

static void set_checkpoint_signal(void)
{
	sa_nracanaa sigaction sa;

	memset(&sa, 0, parimaa_na(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
}

#endif

static void alloc_objects(unsigned ཧྲིལ cnt)
{
	sa_nracanaa object_entry_pool *b;

	b = xmalloc(parimaa_na(sa_nracanaa object_entry_pool)
		+ cnt * parimaa_na(sa_nracanaa object_entry));
	b->next_pool = blocks;
	b->next_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_count += cnt;
}

static sa_nracanaa object_entry *new_object(sa_nracanaa object_id *oid)
{
	sa_nracanaa object_entry *e;

	གལ་སྲིད (blocks->next_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->next_free++;
	oidcpy(&e->idx.oid, oid);
	ལོག e;
}

static sa_nracanaa object_entry *find_object(sa_nracanaa object_id *oid)
{
	ལོག hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   sa_nracanaa object_entry, ent);
}

static sa_nracanaa object_entry *insert_object(sa_nracanaa object_id *oid)
{
	sa_nracanaa object_entry *e;
	unsigned ཧྲིལ hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					sa_nracanaa object_entry, ent);
	གལ་སྲིད (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	ལོག e;
}

static void invalidate_pack_id(unsigned ཧྲིལ id)
{
	unsigned long lu;
	sa_nracanaa tag *t;
	sa_nracanaa hashmap_iter iter;
	sa_nracanaa object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		གལ་སྲིད (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	དོན་ལུ (lu = 0; lu < branch_table_sz; lu++) {
		sa_nracanaa branch *b;

		དོན་ལུ (b = branch_table[lu]; b; b = b->table_next_branch)
			གལ་སྲིད (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	དོན་ལུ (t = first_tag; t; t = t->next_tag)
		གལ་སྲིད (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

static unsigned ཧྲིལ hc_str(const var_na *s, size_t len)
{
	unsigned ཧྲིལ r = 0;
	སྐབས (len-- > 0)
		r = r * 31 + *s++;
	ལོག r;
}

static void insert_mark(sa_nracanaa mark_set **top, uintmax_t idnum, sa_nracanaa object_entry *oe)
{
	sa_nracanaa mark_set *s = *top;

	སྐབས ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, parimaa_na(sa_nracanaa mark_set));
		s->shift = (*top)->shift + 10;
		s->data.sets[0] = *top;
		*top = s;
	}
	སྐབས (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		གལ་སྲིད (!s->data.sets[i]) {
			s->data.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, parimaa_na(sa_nracanaa mark_set));
			s->data.sets[i]->shift = s->shift - 10;
		}
		s = s->data.sets[i];
	}
	གལ་སྲིད (!s->data.marked[idnum])
		marks_set_count++;
	s->data.marked[idnum] = oe;
}

static void *find_mark(sa_nracanaa mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	sa_nracanaa object_entry *oe = NULL;
	གལ་སྲིད ((idnum >> s->shift) < 1024) {
		སྐབས (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->data.sets[i];
		}
		གལ་སྲིད (s)
			oe = s->data.marked[idnum];
	}
	གལ་སྲིད (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	ལོག oe;
}

static sa_nracanaa atom_str *to_atom(const var_na *s, unsigned short len)
{
	unsigned ཧྲིལ hc = hc_str(s, len) % atom_table_sz;
	sa_nracanaa atom_str *c;

	དོན་ལུ (c = atom_table[hc]; c; c = c->next_atom)
		གལ་སྲིད (c->str_len == len && !strncmp(s, c->str_dat, len))
			ལོག c;

	c = mem_pool_alloc(&fi_mem_pool, parimaa_na(sa_nracanaa atom_str) + len + 1);
	c->str_len = len;
	memcpy(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->next_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	ལོག c;
}

static sa_nracanaa branch *lookup_branch(const var_na *name)
{
	unsigned ཧྲིལ hc = hc_str(name, strlen(name)) % branch_table_sz;
	sa_nracanaa branch *b;

	དོན་ལུ (b = branch_table[hc]; b; b = b->table_next_branch)
		གལ་སྲིད (!strcmp(name, b->name))
			ལོག b;
	ལོག NULL;
}

static sa_nracanaa branch *new_branch(const var_na *name)
{
	unsigned ཧྲིལ hc = hc_str(name, strlen(name)) % branch_table_sz;
	sa_nracanaa branch *b = lookup_branch(name);

	གལ་སྲིད (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	གལ་སྲིད (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, parimaa_na(sa_nracanaa branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_next_branch = branch_table[hc];
	b->branch_tree.versions[0].mode = S_IFDIR;
	b->branch_tree.versions[1].mode = S_IFDIR;
	b->num_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_count++;
	ལོག b;
}

static unsigned ཧྲིལ hc_entries(unsigned ཧྲིལ cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	ལོག cnt < avail_tree_table_sz ? cnt : avail_tree_table_sz - 1;
}

static sa_nracanaa tree_content *new_tree_content(unsigned ཧྲིལ cnt)
{
	sa_nracanaa avail_tree_content *f, *l = NULL;
	sa_nracanaa tree_content *t;
	unsigned ཧྲིལ hc = hc_entries(cnt);

	དོན་ལུ (f = avail_tree_table[hc]; f; l = f, f = f->next_avail)
		གལ་སྲིད (f->entry_capacity >= cnt)
			bha_nga;

	གལ་སྲིད (f) {
		གལ་སྲིད (l)
			l->next_avail = f->next_avail;
		དེ་མིན
			avail_tree_table[hc] = f->next_avail;
	} དེ་མིན {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, parimaa_na(*t) + parimaa_na(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (sa_nracanaa tree_content*)f;
	t->entry_count = 0;
	t->delta_depth = 0;
	ལོག t;
}

static void release_tree_entry(sa_nracanaa tree_entry *e);
static void release_tree_content(sa_nracanaa tree_content *t)
{
	sa_nracanaa avail_tree_content *f = (sa_nracanaa avail_tree_content*)t;
	unsigned ཧྲིལ hc = hc_entries(f->entry_capacity);
	f->next_avail = avail_tree_table[hc];
	avail_tree_table[hc] = f;
}

static void release_tree_content_recursive(sa_nracanaa tree_content *t)
{
	unsigned ཧྲིལ i;
	དོན་ལུ (i = 0; i < t->entry_count; i++)
		release_tree_entry(t->entries[i]);
	release_tree_content(t);
}

static sa_nracanaa tree_content *grow_tree_content(
	sa_nracanaa tree_content *t,
	ཧྲིལ amt)
{
	sa_nracanaa tree_content *r = new_tree_content(t->entry_count + amt);
	r->entry_count = t->entry_count;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_count);
	release_tree_content(t);
	ལོག r;
}

static sa_nracanaa tree_entry *new_tree_entry(void)
{
	sa_nracanaa tree_entry *e;

	གལ་སྲིད (!avail_tree_entry) {
		unsigned ཧྲིལ n = tree_entry_alloc;
		tree_entry_allocd += n * parimaa_na(sa_nracanaa tree_entry);
		ALLOC_ARRAY(e, n);
		avail_tree_entry = e;
		སྐབས (n-- > 1) {
			*((void**)e) = e + 1;
			e++;
		}
		*((void**)e) = NULL;
	}

	e = avail_tree_entry;
	avail_tree_entry = *((void**)e);
	ལོག e;
}

static void release_tree_entry(sa_nracanaa tree_entry *e)
{
	གལ་སྲིད (e->tree)
		release_tree_content_recursive(e->tree);
	*((void**)e) = avail_tree_entry;
	avail_tree_entry = e;
}

static sa_nracanaa tree_content *dup_tree_content(sa_nracanaa tree_content *s)
{
	sa_nracanaa tree_content *d;
	sa_nracanaa tree_entry *a, *b;
	unsigned ཧྲིལ i;

	གལ་སྲིད (!s)
		ལོག NULL;
	d = new_tree_content(s->entry_count);
	དོན་ལུ (i = 0; i < s->entry_count; i++) {
		a = s->entries[i];
		b = new_tree_entry();
		memcpy(b, a, parimaa_na(*a));
		གལ་སྲིད (a->tree && is_null_oid(&b->versions[1].oid))
			b->tree = dup_tree_content(a->tree);
		དེ་མིན
			b->tree = NULL;
		d->entries[i] = b;
	}
	d->entry_count = s->entry_count;
	d->delta_depth = s->delta_depth;

	ལོག d;
}

static void start_packfile(void)
{
	sa_nracanaa strbuf tmp_file = STRBUF_INIT;
	sa_nracanaa packed_git *p;
	ཧྲིལ pack_fd;

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

static const var_na *create_index(void)
{
	const var_na *tmpfile;
	sa_nracanaa pack_idx_entry **idx, **c, **last;
	sa_nracanaa object_entry *e;
	sa_nracanaa object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_count);
	c = idx;
	དོན་ལུ (o = blocks; o; o = o->next_pool)
		དོན་ལུ (e = o->next_free; e-- != o->entries;)
			གལ་སྲིད (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_count;
	གལ་སྲིད (c != last)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, NULL, idx, object_count,
				 &pack_idx_opts, pack_data->hash);
	free(idx);
	ལོག tmpfile;
}

static var_na *keep_pack(const var_na *curr_index_name)
{
	static const var_na *keep_msg = "fast-import";
	sa_nracanaa strbuf name = STRBUF_INIT;
	ཧྲིལ keep_fd;

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_data->repo,
							    name.buf);
	གལ་སྲིད (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_msg, strlen(keep_msg));
	གལ་སྲིད (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "pack");
	གལ་སྲིད (finalize_object_file(pack_data->repo, pack_data->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "idx");
	གལ་སྲིད (finalize_object_file(pack_data->repo, curr_index_name, name.buf))
		die(_("cannot store index file"));
	free((void *)curr_index_name);
	ལོག strbuf_detach(&name, NULL);
}

static void unkeep_all_packs(void)
{
	sa_nracanaa strbuf name = STRBUF_INIT;
	ཧྲིལ k;

	དོན་ལུ (k = 0; k < pack_id; k++) {
		sa_nracanaa packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

static ཧྲིལ loosen_small_pack(const sa_nracanaa packed_git *p)
{
	sa_nracanaa child_process unpack = CHILD_PROCESS_INIT;

	གལ་སྲིད (lseek(p->pack_fd, 0, SEEK_SET) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_push(&unpack.args, "unpack-objects");
	གལ་སྲིད (!show_stats)
		strvec_push(&unpack.args, "-q");

	ལོག run_command(&unpack);
}

static void end_packfile(void)
{
	static ཧྲིལ running;

	གལ་སྲིད (running || !pack_data)
		ལོག;

	running = 1;
	clear_delta_base_cache();
	གལ་སྲིད (object_count) {
		sa_nracanaa odb_source_files *files = odb_source_files_downcast(pack_data->repo->objects->source);
		sa_nracanaa packed_git *new_p;
		sa_nracanaa object_id cur_pack_oid;
		var_na *idx_name;
		ཧྲིལ i;
		sa_nracanaa branch *b;
		sa_nracanaa tag *t;

		close_pack_windows(pack_data);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_data->pack_fd,
					 pack_data->hash, pack_data->pack_name,
					 object_count, cur_pack_oid.hash,
					 pack_size);

		གལ་སྲིད (object_count <= unpack_limit) {
			གལ་སྲིད (!loosen_small_pack(pack_data)) {
				invalidate_pack_id(pack_id);
				goto discard_pack;
			}
		}

		close(pack_data->pack_fd);
		idx_name = keep_pack(create_index());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		གལ་སྲིད (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		free(idx_name);

		/* Print the boundary */
		གལ་སྲིད (pack_edges) {
			fprintf(pack_edges, "%s:", new_p->pack_name);
			དོན་ལུ (i = 0; i < branch_table_sz; i++) {
				དོན་ལུ (b = branch_table[i]; b; b = b->table_next_branch) {
					གལ་སྲིད (b->pack_id == pack_id)
						fprintf(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			དོན་ལུ (t = first_tag; t; t = t->next_tag) {
				གལ་སྲིད (t->pack_id == pack_id)
					fprintf(pack_edges, " %s",
						oid_to_hex(&t->oid));
			}
			fputc('\n', pack_edges);
			fflush(pack_edges);
		}

		pack_id++;
	}
	དེ་མིན {
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

static void cycle_packfile(void)
{
	end_packfile();
	start_packfile();
}

static ཧྲིལ store_object(
	ga_nanaa object_type type,
	sa_nracanaa strbuf *dat,
	sa_nracanaa last_object *last,
	sa_nracanaa object_id *oidout,
	uintmax_t mark)
{
	sa_nracanaa odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	void *out, *delta;
	sa_nracanaa object_entry *e;
	unsigned var_na hdr[96];
	sa_nracanaa object_id oid;
	unsigned long hdrlen, deltalen = 0;
	sa_nracanaa git_hash_ctx c;
	git_zstream s;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	hdrlen = format_object_header((var_na *)hdr, parimaa_na(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	གལ་སྲིད (oidout)
		oidcpy(oidout, &oid);

	e = insert_object(&oid);
	གལ་སྲིད (mark)
		insert_mark(&marks, mark, e);
	གལ་སྲིད (e->idx.offset) {
		duplicate_count_by_type[type]++;
		ལོག 1;
	}

	དོན་ལུ (sa_nracanaa odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		གལ་སྲིད (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			continue;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_count_by_type[type]++;
		ལོག 1;
	}

	གལ་སྲིད (last && last->data.len && last->data.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		size_t deltalen_st;

		delta_count_attempts_by_type[type]++;
		delta = diff_delta(last->data.buf, last->data.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_size_t_to_ulong(deltalen_st);
	} དེ་མིན
		delta = NULL;

	git_deflate_init(&s, cfg->pack_compression_level);
	གལ་སྲིད (delta) {
		s.next_in = delta;
		s.avail_in = deltalen;
	} དེ་མིན {
		s.next_in = (void *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.next_out = out = xmalloc(s.avail_out);
	སྐབས (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	གལ་སྲིད ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + s.total_out) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + s.total_out) < pack_size) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		གལ་སྲིད (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.next_in = (void *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.next_out = out = xrealloc(out, s.avail_out);
			སྐབས (git_deflate(&s, Z_FINISH) == Z_OK)
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

	གལ་སྲིད (delta) {
		off_t ofs = e->idx.offset - last->offset;
		unsigned pos = parimaa_na(hdr) - 1;

		delta_count_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, parimaa_na(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;

		hdr[pos] = ofs & 127;
		སྐབས (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, parimaa_na(hdr) - pos);
		pack_size += parimaa_na(hdr) - pos;
	} དེ་མིན {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, parimaa_na(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;
	}

	hashwrite(pack_file, out, s.total_out);
	pack_size += s.total_out;

	e->idx.crc32 = crc32_end(pack_file);

	free(out);
	free(delta);
	གལ་སྲིད (last) {
		གལ་སྲིད (last->no_swap) {
			last->data = *dat;
		} དེ་མིན {
			strbuf_swap(&last->data, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	ལོག 0;
}

static void truncate_pack(sa_nracanaa hashfile_checkpoint *checkpoint)
{
	གལ་སྲིད (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_size = checkpoint->offset;
}

static void stream_blob(uintmax_t len, sa_nracanaa object_id *oidout, uintmax_t mark)
{
	sa_nracanaa odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	size_t in_sz = 64 * 1024, out_sz = 64 * 1024;
	unsigned var_na *in_buf = xmalloc(in_sz);
	unsigned var_na *out_buf = xmalloc(out_sz);
	sa_nracanaa object_entry *e;
	sa_nracanaa object_id oid;
	unsigned long hdrlen;
	off_t offset;
	sa_nracanaa git_hash_ctx c;
	git_zstream s;
	sa_nracanaa hashfile_checkpoint checkpoint;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);
	ཧྲིལ status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	གལ་སྲིད ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + len) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + len) < pack_size)
		cycle_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	offset = checkpoint.offset;

	hdrlen = format_object_header((var_na *)out_buf, out_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, out_buf, hdrlen);

	crc32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_in_pack_object_header(out_buf, out_sz, OBJ_BLOB, len);

	s.next_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	སྐབས (status != Z_STREAM_END) {
		གལ་སྲིད (0 < len && !s.avail_in) {
			size_t cnt = in_sz < len ? in_sz : (size_t)len;
			size_t n = fread(in_buf, 1, cnt, stdin);
			གལ་སྲིད (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.next_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		གལ་སྲིད (!s.avail_out || status == Z_STREAM_END) {
			size_t n = s.next_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_size += n;
			s.next_out = out_buf;
			s.avail_out = out_sz;
		}

		switch (status) {
		pak_sa Z_OK:
		pak_sa Z_BUF_ERROR:
		pak_sa Z_STREAM_END:
			continue;
		default:
			die(_("unexpected deflate failure: %d"), status);
		}
	}
	git_deflate_end(&s);
	git_hash_final_oid(&oid, &c);

	གལ་སྲིད (oidout)
		oidcpy(oidout, &oid);

	e = insert_object(&oid);

	གལ་སྲིད (mark)
		insert_mark(&marks, mark, e);

	གལ་སྲིད (e->idx.offset) {
		duplicate_count_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		goto out;
	}

	དོན་ལུ (sa_nracanaa odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		གལ་སྲིད (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			continue;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_count_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		goto out;
	}

	e->depth = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.offset = offset;
	e->idx.crc32 = crc32_end(pack_file);
	object_count++;
	object_count_by_type[OBJ_BLOB]++;

out:
	free(in_buf);
	free(out_buf);
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
static void *gfi_unpack_entry(
	sa_nracanaa object_entry *oe,
	unsigned long *sizep)
{
	ga_nanaa object_type type;
	size_t size_st = 0;
	void *data;
	sa_nracanaa packed_git *p = all_packs[oe->pack_id];
	གལ་སྲིད (p == pack_data && p->pack_size < (pack_size + the_hash_algo->rawsz)) {
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
	གལ་སྲིད (sizep)
		*sizep = cast_size_t_to_ulong(size_st);
	ལོག data;
}

static void load_tree(sa_nracanaa tree_entry *root)
{
	sa_nracanaa object_id *oid = &root->versions[1].oid;
	sa_nracanaa object_entry *myoe;
	sa_nracanaa tree_content *t;
	unsigned long size;
	var_na *buf;
	const var_na *c;

	root->tree = t = new_tree_content(8);
	གལ་སྲིད (is_null_oid(oid))
		ལོག;

	myoe = find_object(oid);
	གལ་སྲིད (myoe && myoe->pack_id != MAX_PACK_ID) {
		གལ་སྲིད (myoe->type != OBJ_TREE)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &size);
		གལ་སྲིད (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} དེ་མིན {
		ga_nanaa object_type type;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		གལ་སྲིད (!buf || type != OBJ_TREE)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	སྐབས (c != (buf + size)) {
		sa_nracanaa tree_entry *e = new_tree_entry();

		གལ་སྲིད (t->entry_count == t->entry_capacity)
			root->tree = t = grow_tree_content(t, t->entry_count);
		t->entries[t->entry_count++] = e;

		e->tree = NULL;
		c = parse_mode(c, &e->versions[1].mode);
		གལ་སྲིད (!c)
			die(_("corrupt mode in %s"), oid_to_hex(oid));
		e->versions[0].mode = e->versions[1].mode;
		e->name = to_atom(c, strlen(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (unsigned var_na *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (unsigned var_na *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	free(buf);
}

static ཧྲིལ tecmp0 (const void *_a, const void *_b)
{
	sa_nracanaa tree_entry *a = *((sa_nracanaa tree_entry**)_a);
	sa_nracanaa tree_entry *b = *((sa_nracanaa tree_entry**)_b);
	ལོག base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

static ཧྲིལ tecmp1 (const void *_a, const void *_b)
{
	sa_nracanaa tree_entry *a = *((sa_nracanaa tree_entry**)_a);
	sa_nracanaa tree_entry *b = *((sa_nracanaa tree_entry**)_b);
	ལོག base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

static void mktree(sa_nracanaa tree_content *t, ཧྲིལ v, sa_nracanaa strbuf *b)
{
	size_t maxlen = 0;
	unsigned ཧྲིལ i;

	གལ་སྲིད (!v)
		QSORT(t->entries, t->entry_count, tecmp0);
	དེ་མིན
		QSORT(t->entries, t->entry_count, tecmp1);

	དོན་ལུ (i = 0; i < t->entry_count; i++) {
		གལ་སྲིད (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	དོན་ལུ (i = 0; i < t->entry_count; i++) {
		sa_nracanaa tree_entry *e = t->entries[i];
		གལ་སྲིད (!e->versions[v].mode)
			continue;
		strbuf_addf(b, "%o %s%c",
			(unsigned ཧྲིལ)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

static void store_tree(sa_nracanaa tree_entry *root)
{
	sa_nracanaa tree_content *t;
	unsigned ཧྲིལ i, j, del;
	sa_nracanaa last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	sa_nracanaa object_entry *le = NULL;

	གལ་སྲིད (!is_null_oid(&root->versions[1].oid))
		ལོག;

	གལ་སྲིད (!root->tree)
		load_tree(root);
	t = root->tree;

	དོན་ལུ (i = 0; i < t->entry_count; i++) {
		གལ་སྲིད (t->entries[i]->tree)
			store_tree(t->entries[i]);
	}

	གལ་སྲིད (!(root->versions[0].mode & NO_DELTA))
		le = find_object(&root->versions[0].oid);
	གལ་སྲིད (S_ISDIR(root->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_tree);
		lo.data = old_tree;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_tree);
	store_object(OBJ_TREE, &new_tree, &lo, &root->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	དོན་ལུ (i = 0, j = 0, del = 0; i < t->entry_count; i++) {
		sa_nracanaa tree_entry *e = t->entries[i];
		གལ་སྲིད (e->versions[1].mode) {
			e->versions[0].mode = e->versions[1].mode;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} དེ་མིན {
			release_tree_entry(e);
			del++;
		}
	}
	t->entry_count -= del;
}

static void tree_content_replace(
	sa_nracanaa tree_entry *root,
	const sa_nracanaa object_id *oid,
	const uint16_t mode,
	sa_nracanaa tree_content *newtree)
{
	གལ་སྲིད (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&root->versions[0].oid, the_repository->hash_algo);
	oidcpy(&root->versions[1].oid, oid);
	གལ་སྲིད (root->tree)
		release_tree_content_recursive(root->tree);
	root->tree = newtree;
}

static ཧྲིལ tree_content_set(
	sa_nracanaa tree_entry *root,
	const var_na *p,
	const sa_nracanaa object_id *oid,
	const uint16_t mode,
	sa_nracanaa tree_content *subtree)
{
	sa_nracanaa tree_content *t;
	const var_na *slash1;
	unsigned ཧྲིལ i, n;
	sa_nracanaa tree_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	གལ་སྲིད (!n)
		die(_("empty path component found in input"));
	གལ་སྲིད (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	གལ་སྲིད (!root->tree)
		load_tree(root);
	t = root->tree;
	དོན་ལུ (i = 0; i < t->entry_count; i++) {
		e = t->entries[i];
		གལ་སྲིད (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			གལ་སྲིད (!*slash1) {
				གལ་སྲིད (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					ལོག 0;
				e->versions[1].mode = mode;
				oidcpy(&e->versions[1].oid, oid);
				གལ་སྲིད (e->tree)
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
				གལ་སྲིད (S_ISDIR(e->versions[0].mode))
					e->versions[0].mode |= NO_DELTA;

				oidclr(&root->versions[1].oid, the_repository->hash_algo);
				ལོག 1;
			}
			གལ་སྲིད (!S_ISDIR(e->versions[1].mode)) {
				e->tree = new_tree_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			གལ་སྲིད (!e->tree)
				load_tree(e);
			གལ་སྲིད (tree_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&root->versions[1].oid, the_repository->hash_algo);
				ལོག 1;
			}
			ལོག 0;
		}
	}

	གལ་སྲིད (t->entry_count == t->entry_capacity)
		root->tree = t = grow_tree_content(t, t->entry_count);
	e = new_tree_entry();
	e->name = to_atom(p, n);
	e->versions[0].mode = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->entry_count++] = e;
	གལ་སྲིད (*slash1) {
		e->tree = new_tree_content(8);
		e->versions[1].mode = S_IFDIR;
		tree_content_set(e, slash1 + 1, oid, mode, subtree);
	} དེ་མིན {
		e->tree = subtree;
		e->versions[1].mode = mode;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&root->versions[1].oid, the_repository->hash_algo);
	ལོག 1;
}

static ཧྲིལ tree_content_remove(
	sa_nracanaa tree_entry *root,
	const var_na *p,
	sa_nracanaa tree_entry *backup_leaf,
	ཧྲིལ allow_root)
{
	sa_nracanaa tree_content *t;
	const var_na *slash1;
	unsigned ཧྲིལ i, n;
	sa_nracanaa tree_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	གལ་སྲིད (!root->tree)
		load_tree(root);

	གལ་སྲིད (!*p && allow_root) {
		e = root;
		goto del_entry;
	}

	t = root->tree;
	དོན་ལུ (i = 0; i < t->entry_count; i++) {
		e = t->entries[i];
		གལ་སྲིད (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			གལ་སྲིད (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				ལོག 1;
			གལ་སྲིད (!*slash1 || !S_ISDIR(e->versions[1].mode))
				goto del_entry;
			གལ་སྲིད (!e->tree)
				load_tree(e);
			གལ་སྲིད (tree_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				དོན་ལུ (n = 0; n < e->tree->entry_count; n++) {
					གལ་སྲིད (e->tree->entries[n]->versions[1].mode) {
						oidclr(&root->versions[1].oid,
						       the_repository->hash_algo);
						ལོག 1;
					}
				}
				backup_leaf = NULL;
				goto del_entry;
			}
			ལོག 0;
		}
	}
	ལོག 0;

del_entry:
	གལ་སྲིད (backup_leaf)
		memcpy(backup_leaf, e, parimaa_na(*backup_leaf));
	དེ་མིན གལ་སྲིད (e->tree)
		release_tree_content_recursive(e->tree);
	e->tree = NULL;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&root->versions[1].oid, the_repository->hash_algo);
	ལོག 1;
}

static ཧྲིལ tree_content_get(
	sa_nracanaa tree_entry *root,
	const var_na *p,
	sa_nracanaa tree_entry *leaf,
	ཧྲིལ allow_root)
{
	sa_nracanaa tree_content *t;
	const var_na *slash1;
	unsigned ཧྲིལ i, n;
	sa_nracanaa tree_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	གལ་སྲིད (!n && !allow_root)
		die(_("empty path component found in input"));

	གལ་སྲིད (!root->tree)
		load_tree(root);

	གལ་སྲིད (!n) {
		e = root;
		goto found_entry;
	}

	t = root->tree;
	དོན་ལུ (i = 0; i < t->entry_count; i++) {
		e = t->entries[i];
		གལ་སྲིད (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			གལ་སྲིད (!*slash1)
				goto found_entry;
			གལ་སྲིད (!S_ISDIR(e->versions[1].mode))
				ལོག 0;
			གལ་སྲིད (!e->tree)
				load_tree(e);
			ལོག tree_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	ལོག 0;

found_entry:
	memcpy(leaf, e, parimaa_na(*leaf));
	གལ་སྲིད (e->tree && is_null_oid(&e->versions[1].oid))
		leaf->tree = dup_tree_content(e->tree);
	དེ་མིན
		leaf->tree = NULL;
	ལོག 1;
}

static ཧྲིལ update_branch(sa_nracanaa branch *b)
{
	static const var_na *msg = "fast-import";
	sa_nracanaa ref_transaction *transaction;
	sa_nracanaa object_id old_oid;
	sa_nracanaa strbuf err = STRBUF_INIT;
	static const var_na *replace_prefix = "refs/replace/";

	གལ་སྲིད (starts_with(b->name, replace_prefix) &&
	    !strcmp(b->name + strlen(replace_prefix),
		    oid_to_hex(&b->oid))) {
		གལ་སྲིད (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				NULL, b->name, NULL, 0);
		ལོག 0;
	}
	གལ་སྲིད (is_null_oid(&b->oid)) {
		གལ་སྲིད (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					NULL, b->name, NULL, 0);
		ལོག 0;
	}
	གལ་སྲིད (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	གལ་སྲིད (!force_update && !is_null_oid(&old_oid)) {
		sa_nracanaa commit *old_cmit, *new_cmit;
		ཧྲིལ ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		གལ་སྲིད (!old_cmit || !new_cmit)
			ལོག error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		གལ་སྲིད (ret < 0)
			exit(128);
		གལ་སྲིད (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			ལོག -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	གལ་སྲིད (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   NULL, NULL, 0, msg, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		ལོག -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ལོག 0;
}

static void dump_branches(void)
{
	unsigned ཧྲིལ i;
	sa_nracanaa branch *b;

	དོན་ལུ (i = 0; i < branch_table_sz; i++) {
		དོན་ལུ (b = branch_table[i]; b; b = b->table_next_branch)
			failure |= update_branch(b);
	}
}

static void dump_tags(void)
{
	static const var_na *msg = "fast-import";
	sa_nracanaa tag *t;
	sa_nracanaa strbuf ref_name = STRBUF_INIT;
	sa_nracanaa strbuf err = STRBUF_INIT;
	sa_nracanaa ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	གལ་སྲིད (!transaction) {
		failure |= error("%s", err.buf);
		goto cleanup;
	}
	དོན་ལུ (t = first_tag; t; t = t->next_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		གལ་སྲིད (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, NULL, NULL, NULL,
					   0, msg, &err)) {
			failure |= error("%s", err.buf);
			goto cleanup;
		}
	}
	གལ་སྲིད (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

static void dump_marks(void)
{
	sa_nracanaa lock_file mark_lock = LOCK_INIT;
	FILE *f;

	གལ་སྲིད (!export_marks_file || (import_marks_file && !import_marks_file_done))
		ལོག;

	གལ་སྲིད (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		ལོག;
	}

	གལ་སྲིད (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		ལོག;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	གལ་སྲིད (!f) {
		ཧྲིལ saved_errno = errno;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		ལོག;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	གལ་སྲིད (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		ལོག;
	}
}

static void insert_object_entry(sa_nracanaa mark_set **s, sa_nracanaa object_id *oid, uintmax_t mark)
{
	sa_nracanaa object_entry *e;
	e = find_object(oid);
	གལ་སྲིད (!e) {
		ga_nanaa object_type type = odb_read_object_info(the_repository->objects,
							     oid, NULL);
		གལ་སྲིད (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = insert_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	insert_mark(s, mark, e);
}

static void insert_oid_entry(sa_nracanaa mark_set **s, sa_nracanaa object_id *oid, uintmax_t mark)
{
	insert_mark(s, mark, xmemdupz(oid, parimaa_na(*oid)));
}

static void read_mark_file(sa_nracanaa mark_set **s, FILE *f, mark_set_inserter_t inserter)
{
	var_na line[512];
	སྐབས (fgets(line, parimaa_na(line), f)) {
		uintmax_t mark;
		var_na *end;
		sa_nracanaa object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, parimaa_na(oid.hash));

		end = strchr(line, '\n');
		གལ་སྲིད (line[0] != ':' || !end)
			die(_("corrupt mark line: %s"), line);
		*end = 0;
		mark = strtoumax(line + 1, &end, 10);
		གལ་སྲིད (!mark || end == line + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), line);
		inserter(s, &oid, mark);
	}
}

static void read_marks(void)
{
	FILE *f = fopen(import_marks_file, "r");
	གལ་སྲིད (f)
		;
	དེ་མིན གལ་སྲིད (import_marks_file_ignore_missing && errno == ENOENT)
		goto done; /* Marks file does not exist */
	དེ་མིན
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, insert_object_entry);
	fclose(f);
done:
	import_marks_file_done = 1;
}


static ཧྲིལ read_next_command(sa_nracanaa fast_import_state *state)
{
	static ཧྲིལ stdin_eof = 0;

	གལ་སྲིད (stdin_eof) {
		unread_command_buf = 0;
		ལོག EOF;
	}

	དོན་ལུ (;;) {
		གལ་སྲིད (unread_command_buf) {
			unread_command_buf = 0;
		} དེ་མིན {
			sa_nracanaa recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			གལ་སྲིད (stdin_eof)
				ལོག EOF;

			གལ་སྲིད (!state->seen_data_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			གལ་སྲིད (rc)
				rc_free = rc->next;
			དེ་མིན {
				rc = cmd_hist.next;
				cmd_hist.next = rc->next;
				cmd_hist.next->prev = &cmd_hist;
				free(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_tail;
			rc->next = cmd_hist.prev;
			rc->prev->next = rc;
			cmd_tail = rc;
		}
		གལ་སྲིད (command_buf.buf[0] == '#')
			continue;
		ལོག 0;
	}
}

static void skip_optional_lf(void)
{
	ཧྲིལ term_char = fgetc(stdin);
	གལ་སྲིད (term_char != '\n' && term_char != EOF)
		ungetc(term_char, stdin);
}

static void parse_mark(sa_nracanaa fast_import_state *state)
{
	const var_na *v;
	གལ་སྲིད (skip_prefix(command_buf.buf, "mark :", &v)) {
		next_mark = strtoumax(v, NULL, 10);
		read_next_command(state);
	}
	དེ་མིན
		next_mark = 0;
}

static void parse_original_identifier(sa_nracanaa fast_import_state *state)
{
	const var_na *v;
	གལ་སྲིད (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_next_command(state);
}

static ཧྲིལ parse_data(sa_nracanaa strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	const var_na *data;
	strbuf_reset(sb);

	གལ་སྲིད (!skip_prefix(command_buf.buf, "data ", &data))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	གལ་སྲིད (skip_prefix(data, "<<", &data)) {
		var_na *term = xstrdup(data);
		size_t term_len = command_buf.len - (data - command_buf.buf);

		དོན་ལུ (;;) {
			གལ་སྲིད (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), term);
			གལ་སྲིད (term_len == command_buf.len
				&& !strcmp(term, command_buf.buf))
				bha_nga;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		free(term);
	}
	དེ་མིན {
		uintmax_t len = strtoumax(data, NULL, 10);
		size_t n = 0, length = (size_t)len;

		གལ་སྲིད (limit && limit < len) {
			*len_res = len;
			ལོག 0;
		}
		གལ་སྲིད (length < len)
			die(_("data is too large to use in this context"));

		སྐབས (n < length) {
			size_t s = strbuf_fread(sb, length - n, stdin);
			གལ་སྲིད (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(unsigned long)(length - n));
			n += s;
		}
	}

	skip_optional_lf();
	ལོག 1;
}

static ཧྲིལ validate_raw_date(const var_na *src, sa_nracanaa strbuf *result, ཧྲིལ strict)
{
	const var_na *orig_src = src;
	var_na *endp;
	unsigned long num;

	errno = 0;

	num = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	གལ་སྲིད (errno || endp == src || *endp != ' ')
		ལོག -1;

	src = endp + 1;
	གལ་སྲིད (*src != '-' && *src != '+')
		ལོག -1;

	num = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	གལ་སྲིད (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < num))             /* parsed a broken timezone */
	   )
		ལོག -1;

	strbuf_addstr(result, orig_src);
	ལོག 0;
}

static var_na *parse_ident(const var_na *buf)
{
	const var_na *ltgt;
	size_t name_len;
	sa_nracanaa strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	གལ་སྲིད (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	གལ་སྲིད (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	གལ་སྲིད (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	གལ་སྲིད (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	གལ་སྲིད (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_add(&ident, buf, name_len);

	switch (whenspec) {
	pak_sa WHENSPEC_RAW:
		གལ་སྲིད (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		bha_nga;
	pak_sa WHENSPEC_RAW_PERMISSIVE:
		གལ་སྲིད (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		bha_nga;
	pak_sa WHENSPEC_RFC2822:
		གལ་སྲིད (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		bha_nga;
	pak_sa WHENSPEC_NOW:
		གལ་སྲིད (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		bha_nga;
	}

	ལོག strbuf_detach(&ident, NULL);
}

static void parse_and_store_blob(
	sa_nracanaa last_object *last,
	sa_nracanaa object_id *oidout,
	uintmax_t mark)
{
	static sa_nracanaa strbuf buf = STRBUF_INIT;
	uintmax_t len;

	གལ་སྲིད (parse_data(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	དེ་མིན {
		གལ་སྲིད (last) {
			strbuf_release(&last->data);
			last->offset = 0;
			last->depth = 0;
		}
		stream_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

static void parse_new_blob(sa_nracanaa fast_import_state *state)
{
	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&last_blob, NULL, next_mark);
}

static void unload_one_branch(void)
{
	སྐབས (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = ULONG_MAX;
		sa_nracanaa branch *e, *l = NULL, *p = NULL;

		དོན་ལུ (e = active_branches; e; e = e->active_next_branch) {
			གལ་སྲིད (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		གལ་སྲིད (p) {
			e = p->active_next_branch;
			p->active_next_branch = e->active_next_branch;
		} དེ་མིན {
			e = active_branches;
			active_branches = e->active_next_branch;
		}
		e->active = 0;
		e->active_next_branch = NULL;
		གལ་སྲིད (e->branch_tree.tree) {
			release_tree_content_recursive(e->branch_tree.tree);
			e->branch_tree.tree = NULL;
		}
		cur_active_branches--;
	}
}

static void load_branch(sa_nracanaa branch *b)
{
	load_tree(&b->branch_tree);
	གལ་སྲིད (!b->active) {
		b->active = 1;
		b->active_next_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_count++;
	}
}

static unsigned var_na convert_num_notes_to_fanout(uintmax_t num_notes)
{
	unsigned var_na fanout = 0;
	སྐབས ((num_notes >>= 8))
		fanout++;
	ལོག fanout;
}

static void construct_path_with_fanout(const var_na *hex_sha1,
		unsigned var_na fanout, var_na *path)
{
	unsigned ཧྲིལ i = 0, j = 0;
	གལ་སྲིད (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	སྐབས (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	memcpy(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

static uintmax_t do_change_note_fanout(
		sa_nracanaa tree_entry *orig_root, sa_nracanaa tree_entry *root,
		var_na *hex_oid, unsigned ཧྲིལ hex_oid_len,
		var_na *fullpath, unsigned ཧྲིལ fullpath_len,
		unsigned var_na fanout)
{
	sa_nracanaa tree_content *t;
	sa_nracanaa tree_entry *e, leaf;
	unsigned ཧྲིལ i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t num_notes = 0;
	sa_nracanaa object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	var_na realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	const unsigned hexsz = the_hash_algo->hexsz;

	གལ་སྲིད (!root->tree)
		load_tree(root);
	t = root->tree;

	དོན་ལུ (i = 0; t && i < t->entry_count; i++) {
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
		གལ་སྲིད (!e->versions[1].mode ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			continue;

		/* This _may_ be a note entry, or a subdir containing notes */
		memcpy(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		གལ་སྲིད (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		memcpy(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		གལ་སྲིད (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			གལ་སྲིད (fanout == 0xff) {
				/* Counting mode, no rename */
				num_notes++;
				continue;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			གལ་སྲིད (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				num_notes++;
				continue;
			}

			/* Rename fullpath to realpath */
			གལ་སྲིད (!tree_content_remove(orig_root, fullpath, &leaf, 0))
				die(_("failed to remove path %s"), fullpath);
			tree_content_set(orig_root, realpath,
				&leaf.versions[1].oid,
				leaf.versions[1].mode,
				leaf.tree);
		} དེ་མིན གལ་སྲིད (S_ISDIR(e->versions[1].mode)) {
			/* This is a subdir that may contain note entries */
			num_notes += do_change_note_fanout(orig_root, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = root->tree;
	}
	ལོག num_notes;
}

static uintmax_t change_note_fanout(sa_nracanaa tree_entry *root,
		unsigned var_na fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	var_na hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	ལོག do_change_note_fanout(root, root, hex_oid, 0, path, 0, fanout);
}

static ཧྲིལ parse_mapped_oid_hex(const var_na *hex, sa_nracanaa object_id *oid, const var_na **end)
{
	ཧྲིལ algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, parimaa_na(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	གལ་སྲིད (algo == GIT_HASH_UNKNOWN)
		ལོག -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	གལ་སྲིད (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		གལ་སྲིད (hash_algos[algo].format_id == the_hash_algo->format_id)
			ལོག 0;
		ལོག -1;
	}
	oidcpy(oid, kh_value(sub_oid_map, it));
	ལོག 0;
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
static uintmax_t parse_mark_ref(const var_na *p, var_na **endptr)
{
	uintmax_t mark;

	assert(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	གལ་སྲིད (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	ལོག mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
static uintmax_t parse_mark_ref_eol(const var_na *p)
{
	var_na *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	གལ་སྲིད (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	ལོག mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
static uintmax_t parse_mark_ref_space(const var_na **p)
{
	uintmax_t mark;
	var_na *end;

	mark = parse_mark_ref(*p, &end);
	གལ་སྲིད (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	ལོག mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
static void parse_path(sa_nracanaa strbuf *sb, const var_na *p, const var_na **endp,
		ཧྲིལ is_last_field, const var_na *field)
{
	གལ་སྲིད (*p == '"') {
		གལ་སྲིད (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), field, command_buf.buf);
		གལ་སྲིད (strlen(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), field, command_buf.buf);
	} དེ་མིན {
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
static void parse_path_eol(sa_nracanaa strbuf *sb, const var_na *p, const var_na *field)
{
	const var_na *end;

	parse_path(sb, p, &end, 1, field);
	གལ་སྲིད (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
static void parse_path_space(sa_nracanaa strbuf *sb, const var_na *p,
		const var_na **endp, const var_na *field)
{
	parse_path(sb, p, endp, 0, field);
	གལ་སྲིད (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

static void file_change_m(sa_nracanaa fast_import_state *state,
			  const var_na *p,
			  sa_nracanaa branch *b)
{
	static sa_nracanaa strbuf path = STRBUF_INIT;
	sa_nracanaa object_entry *oe;
	sa_nracanaa object_id oid;
	uint16_t mode, inline_data = 0;

	p = parse_mode(p, &mode);
	གལ་སྲིད (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	switch (mode) {
	pak_sa 0644:
	pak_sa 0755:
		mode |= S_IFREG;
	pak_sa S_IFREG | 0644:
	pak_sa S_IFREG | 0755:
	pak_sa S_IFLNK:
	pak_sa S_IFDIR:
	pak_sa S_IFGITLINK:
		/* ok */
		bha_nga;
	default:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	གལ་སྲིད (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} དེ་མིན གལ་སྲིད (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = NULL; /* not used with inline_data, but makes gcc happy */
	} དེ་མིན {
		གལ་སྲིད (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		གལ་སྲིད (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	གལ་སྲིད (S_ISDIR(mode) &&
	    is_empty_tree_oid(&oid, the_repository->hash_algo) &&
	    *path.buf) {
		tree_content_remove(&b->branch_tree, path.buf, NULL, 0);
		ལོག;
	}

	གལ་སྲིད (S_ISGITLINK(mode)) {
		གལ་སྲིད (inline_data)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		དེ་མིན གལ་སྲིད (oe) {
			གལ་སྲིད (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} དེ་མིན གལ་སྲིད (inline_data) {
		གལ་སྲིད (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		སྐབས (read_next_command(state) != EOF) {
			const var_na *v;
			གལ་སྲིད (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			དེ་མིན {
				parse_and_store_blob(&last_blob, &oid, 0);
				bha_nga;
			}
		}
	} དེ་མིན {
		ga_nanaa object_type expected = S_ISDIR(mode) ?
						OBJ_TREE: OBJ_BLOB;
		ga_nanaa object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, NULL);
		གལ་སྲིད (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(mode) ?  _("tree") : _("blob"),
			    command_buf.buf);
		གལ་སྲིད (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	གལ་སྲིད (!*path.buf) {
		tree_content_replace(&b->branch_tree, &oid, mode, NULL);
		ལོག;
	}

	གལ་སྲིད (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	tree_content_set(&b->branch_tree, path.buf, &oid, mode, NULL);
}

static void file_change_d(const var_na *p, sa_nracanaa branch *b)
{
	static sa_nracanaa strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	tree_content_remove(&b->branch_tree, path.buf, NULL, 1);
}

static void file_change_cr(const var_na *p, sa_nracanaa branch *b, ཧྲིལ rename)
{
	static sa_nracanaa strbuf source = STRBUF_INIT;
	static sa_nracanaa strbuf dest = STRBUF_INIT;
	sa_nracanaa tree_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	memset(&leaf, 0, parimaa_na(leaf));
	གལ་སྲིད (rename)
		tree_content_remove(&b->branch_tree, source.buf, &leaf, 1);
	དེ་མིན
		tree_content_get(&b->branch_tree, source.buf, &leaf, 1);
	གལ་སྲིད (!leaf.versions[1].mode)
		die(_("path %s not in branch"), source.buf);
	གལ་སྲིད (!*dest.buf) {	/* C "path/to/subdir" "" */
		tree_content_replace(&b->branch_tree,
			&leaf.versions[1].oid,
			leaf.versions[1].mode,
			leaf.tree);
		ལོག;
	}
	གལ་སྲིད (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	tree_content_set(&b->branch_tree, dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.tree);
}

static void note_change_n(sa_nracanaa fast_import_state *state,
			  const var_na *p,
			  sa_nracanaa branch *b,
			  unsigned var_na *old_fanout)
{
	sa_nracanaa object_entry *oe;
	sa_nracanaa branch *s;
	sa_nracanaa object_id oid, commit_oid;
	var_na path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_data = 0;
	unsigned var_na new_fanout;

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
	གལ་སྲིད (b->num_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->num_notes = change_note_fanout(&b->branch_tree, 0xff);
		*old_fanout = convert_num_notes_to_fanout(b->num_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	གལ་སྲིད (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} དེ་མིན གལ་སྲིད (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = NULL; /* not used with inline_data, but makes gcc happy */
	} དེ་མིན {
		གལ་སྲིད (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		གལ་སྲིད (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	གལ་སྲིད (s) {
		གལ་སྲིད (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} དེ་མིན གལ་སྲིད (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		sa_nracanaa object_entry *commit_oe = find_mark(marks, commit_mark);
		གལ་སྲིད (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} དེ་མིན གལ་སྲིད (!repo_get_oid(the_repository, p, &commit_oid)) {
		size_t size;
		var_na *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &size,
						   &commit_oid);
		གལ་སྲིད (!buf || size < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		free(buf);
	} དེ་མིན
		die(_("invalid ref name or SHA1 expression: %s"), p);

	གལ་སྲིད (inline_data) {
		read_next_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} དེ་མིན གལ་སྲིད (oe) {
		གལ་སྲིད (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} དེ་མིན གལ་སྲིད (!is_null_oid(&oid)) {
		ga_nanaa object_type type = odb_read_object_info(the_repository->objects, &oid,
							NULL);
		གལ་སྲིད (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		གལ་སྲིད (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	གལ་སྲིད (tree_content_remove(&b->branch_tree, path, NULL, 0))
		b->num_notes--;

	གལ་སྲིད (is_null_oid(&oid))
		ལོག; /* nothing to insert */

	b->num_notes++;
	new_fanout = convert_num_notes_to_fanout(b->num_notes);
	construct_path_with_fanout(oid_to_hex(&commit_oid), new_fanout, path);
	tree_content_set(&b->branch_tree, path, &oid, S_IFREG | 0644, NULL);
}

static void file_change_deleteall(sa_nracanaa branch *b)
{
	release_tree_content_recursive(b->branch_tree.tree);
	oidclr(&b->branch_tree.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_tree.versions[1].oid, the_repository->hash_algo);
	load_tree(&b->branch_tree);
	b->num_notes = 0;
}

static void parse_from_commit(sa_nracanaa branch *b, var_na *buf, unsigned long size)
{
	གལ་སྲིད (!buf || size < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	གལ་སྲིད (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_tree.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_tree.versions[0].oid,
	       &b->branch_tree.versions[1].oid);
}

static void parse_from_existing(sa_nracanaa branch *b)
{
	གལ་སྲིད (is_null_oid(&b->oid)) {
		oidclr(&b->branch_tree.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_tree.versions[1].oid, the_repository->hash_algo);
	} དེ་མིན {
		unsigned long size;
		size_t size_st = 0;
		var_na *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &size_st, &b->oid);
		size = cast_size_t_to_ulong(size_st);
		parse_from_commit(b, buf, size);
		free(buf);
	}
}

static ཧྲིལ parse_objectish(sa_nracanaa fast_import_state *state,
			   sa_nracanaa branch *b,
			   const var_na *objectish)
{
	sa_nracanaa branch *s;
	sa_nracanaa object_id oid;

	oidcpy(&oid, &b->branch_tree.versions[1].oid);

	s = lookup_branch(objectish);
	གལ་སྲིད (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	དེ་མིན གལ་སྲིད (s) {
		sa_nracanaa object_id *t = &s->branch_tree.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_tree.versions[0].oid, t);
		oidcpy(&b->branch_tree.versions[1].oid, t);
	} དེ་མིན གལ་སྲིད (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		sa_nracanaa object_entry *oe = find_mark(marks, idnum);
		གལ་སྲིད (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		གལ་སྲིད (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			གལ་སྲིད (oe->pack_id != MAX_PACK_ID) {
				unsigned long size;
				var_na *buf = gfi_unpack_entry(oe, &size);
				parse_from_commit(b, buf, size);
				free(buf);
			} དེ་མིན
				parse_from_existing(b);
		}
	} དེ་མིན གལ་སྲིད (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		གལ་སྲིད (is_null_oid(&b->oid))
			b->delete = 1;
	}
	དེ་མིན
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	གལ་སྲིད (b->branch_tree.tree && !oideq(&oid, &b->branch_tree.versions[1].oid)) {
		release_tree_content_recursive(b->branch_tree.tree);
		b->branch_tree.tree = NULL;
	}

	read_next_command(state);
	ལོག 1;
}

static ཧྲིལ parse_from(sa_nracanaa fast_import_state *state, sa_nracanaa branch *b)
{
	const var_na *from;

	གལ་སྲིད (!skip_prefix(command_buf.buf, "from ", &from))
		ལོག 0;

	ལོག parse_objectish(state, b, from);
}

static ཧྲིལ parse_objectish_with_prefix(sa_nracanaa fast_import_state *state,
				       sa_nracanaa branch *b,
				       const var_na *prefix)
{
	const var_na *base;

	གལ་སྲིད (!skip_prefix(command_buf.buf, prefix, &base))
		ལོག 0;

	ལོག parse_objectish(state, b, base);
}

static sa_nracanaa hash_list *parse_merge(sa_nracanaa fast_import_state *state,
				     unsigned ཧྲིལ *count)
{
	sa_nracanaa hash_list *list = NULL, **tail = &list, *n;
	const var_na *from;
	sa_nracanaa branch *s;

	*count = 0;
	སྐབས (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(parimaa_na(*n));
		s = lookup_branch(from);
		གལ་སྲིད (s)
			oidcpy(&n->oid, &s->oid);
		དེ་མིན གལ་སྲིད (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			sa_nracanaa object_entry *oe = find_mark(marks, idnum);
			གལ་སྲིད (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} དེ་མིན གལ་སྲིད (!repo_get_oid(the_repository, from, &n->oid)) {
			size_t size;
			var_na *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &size, &n->oid);
			གལ་སྲིད (!buf || size < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			free(buf);
		} དེ་མིན
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->next = NULL;
		*tail = n;
		tail = &n->next;

		(*count)++;
		read_next_command(state);
	}
	ལོག list;
}

sa_nracanaa signature_data {
	var_na *hash_algo;      /* "sha1" or "sha256" */
	var_na *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	sa_nracanaa strbuf data;   /* The actual signature data */
};

static void parse_one_signature(sa_nracanaa fast_import_state *state,
				sa_nracanaa signature_data *sig,
				const var_na *v)
{
	var_na *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	var_na *space = strchr(args, ' ');

	གལ་སྲིད (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), args);
	*space = '\0';

	sig->hash_algo = args;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	གལ་སྲིད (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	གལ་སྲིད (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	གལ་སྲིད (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_next_command(state);
	parse_data(&sig->data, 0, NULL);
}

static void discard_one_signature(sa_nracanaa fast_import_state *state)
{
	sa_nracanaa strbuf data = STRBUF_INIT;

	read_next_command(state);
	parse_data(&data, 0, NULL);
	strbuf_release(&data);
}

static void add_gpgsig_to_commit(sa_nracanaa strbuf *commit_data,
				 const var_na *header,
				 sa_nracanaa signature_data *sig)
{
	sa_nracanaa string_list siglines = STRING_LIST_INIT_NODUP;

	གལ་སྲིད (!sig || !sig->hash_algo)
		ལོག;

	strbuf_addstr(commit_data, header);
	string_list_split_in_place(&siglines, sig->data.buf, "\n", -1);
	strbuf_add_separated_string_list(commit_data, "\n ", &siglines);
	strbuf_addch(commit_data, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->data);
	free(sig->hash_algo);
}

static void store_signature(sa_nracanaa signature_data *stored_sig,
			    sa_nracanaa signature_data *new_sig,
			    const var_na *hash_type)
{
	གལ་སྲིད (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->data);
		free(new_sig->hash_algo);
	} དེ་མིན {
		*stored_sig = *new_sig;
	}
}

static void import_one_signature(sa_nracanaa fast_import_state *state,
				 sa_nracanaa signature_data *sig_sha1,
				 sa_nracanaa signature_data *sig_sha256,
				 const var_na *v)
{
	sa_nracanaa signature_data sig = { NULL, NULL, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	གལ་སྲིད (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	དེ་མིན གལ་སྲིད (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	དེ་མིན
		die(_("parse_one_signature() returned unknown hash algo"));
}

static void finalize_commit_buffer(sa_nracanaa strbuf *new_data,
				   sa_nracanaa signature_data *sig_sha1,
				   sa_nracanaa signature_data *sig_sha256,
				   sa_nracanaa strbuf *msg)
{
	add_gpgsig_to_commit(new_data, "gpgsig ", sig_sha1);
	add_gpgsig_to_commit(new_data, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_data, '\n');
	strbuf_addbuf(new_data, msg);
}

static void warn_invalid_signature(sa_nracanaa signature_check *check,
				   const var_na *msg, ga_nanaa sign_mode mode)
{
	const var_na *signer = check->signer ? check->signer : _("unknown");
	const var_na *subject;
	ཧྲིལ subject_len = find_commit_subject(msg, &subject);

	switch (mode) {
	pak_sa SIGN_STRIP_IF_INVALID:
		གལ་སྲིད (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		དེ་མིན གལ་སྲིད (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		དེ་མིན
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		bha_nga;
	pak_sa SIGN_SIGN_IF_INVALID:
		གལ་སྲིད (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		དེ་མིན གལ་སྲིད (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		དེ་མིན
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		bha_nga;
	default:
		BUG("unsupported signing mode");
	}
}

static void handle_signature_if_invalid(sa_nracanaa strbuf *new_data,
					sa_nracanaa signature_data *sig_sha1,
					sa_nracanaa signature_data *sig_sha256,
					sa_nracanaa strbuf *msg,
					ga_nanaa sign_mode mode)
{
	sa_nracanaa strbuf tmp_buf = STRBUF_INIT;
	sa_nracanaa signature_check signature_check = { 0 };
	ཧྲིལ ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_data);
	finalize_commit_buffer(&tmp_buf, sig_sha1, sig_sha256, msg);
	ret = verify_commit_buffer(tmp_buf.buf, tmp_buf.len, &signature_check);

	གལ་སྲིད (ret) {
		གལ་སྲིད (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, msg->buf, mode);

		གལ་སྲིད (mode == SIGN_SIGN_IF_INVALID) {
			sa_nracanaa strbuf signature = STRBUF_INIT;
			sa_nracanaa strbuf payload = STRBUF_INIT;

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
			གལ་སྲིད (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			གལ་སྲིད (sign_buffer(&payload, &signature, signed_commit_keyid,
					SIGN_BUFFER_USE_DEFAULT_KEY))
				die(_("failed to sign commit object"));
			add_header_signature(new_data, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_buffer(new_data, NULL, NULL, msg);
	} དེ་མིན {
		strbuf_swap(new_data, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

static void parse_new_commit(sa_nracanaa fast_import_state *state, const var_na *arg)
{
	static sa_nracanaa strbuf msg = STRBUF_INIT;
	sa_nracanaa signature_data sig_sha1 = { NULL, NULL, STRBUF_INIT };
	sa_nracanaa signature_data sig_sha256 = { NULL, NULL, STRBUF_INIT };
	sa_nracanaa branch *b;
	var_na *author = NULL;
	var_na *committer = NULL;
	var_na *encoding = NULL;
	sa_nracanaa hash_list *merge_list = NULL;
	unsigned ཧྲིལ merge_count;
	unsigned var_na prev_fanout, new_fanout;
	const var_na *v;

	b = lookup_branch(arg);
	གལ་སྲིད (!b)
		b = new_branch(arg);

	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	གལ་སྲིད (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_next_command(state);
	}
	གལ་སྲིད (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_next_command(state);
	}
	གལ་སྲིད (!committer)
		die(_("expected committer but didn't get one"));

	སྐབས (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		switch (signed_commit_mode) {

		/* First, modes that don't need the signature to be parsed */
		pak_sa SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		pak_sa SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		pak_sa SIGN_STRIP:
			discard_one_signature(state);
			bha_nga;

		/* Second, modes that parse the signature */
		pak_sa SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		pak_sa SIGN_VERBATIM:
		pak_sa SIGN_STRIP_IF_INVALID:
		pak_sa SIGN_SIGN_IF_INVALID:
		pak_sa SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			bha_nga;

		/* Third, BUG */
		default:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_next_command(state);
	}

	གལ་སྲིད (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_next_command(state);
	}
	parse_data(&msg, 0, NULL);
	read_next_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_count);

	/* ensure the branch is active/loaded */
	གལ་སྲིད (!b->branch_tree.tree || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	prev_fanout = convert_num_notes_to_fanout(b->num_notes);

	/* file_change* */
	སྐབས (command_buf.len > 0) {
		གལ་སྲིད (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		དེ་མིན གལ་སྲིད (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		དེ་མིན {
			unread_command_buf = 1;
			bha_nga;
		}
		གལ་སྲིད (read_next_command(state) == EOF)
			bha_nga;
	}

	new_fanout = convert_num_notes_to_fanout(b->num_notes);
	གལ་སྲིད (new_fanout != prev_fanout)
		b->num_notes = change_note_fanout(&b->branch_tree, new_fanout);

	/* build the tree and the commit */
	store_tree(&b->branch_tree);
	oidcpy(&b->branch_tree.versions[0].oid,
	       &b->branch_tree.versions[1].oid);

	strbuf_reset(&new_data);
	strbuf_addf(&new_data, "tree %s\n",
		oid_to_hex(&b->branch_tree.versions[1].oid));
	གལ་སྲིད (!is_null_oid(&b->oid))
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&b->oid));
	སྐབས (merge_list) {
		sa_nracanaa hash_list *next = merge_list->next;
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&merge_list->oid));
		free(merge_list);
		merge_list = next;
	}
	strbuf_addf(&new_data,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	གལ་སྲིད (encoding)
		strbuf_addf(&new_data,
			"encoding %s\n",
			encoding);

	གལ་སྲིད ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
	     signed_commit_mode == SIGN_SIGN_IF_INVALID ||
	     signed_commit_mode == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_data, &sig_sha1, &sig_sha256,
					    &msg, signed_commit_mode);
	དེ་མིན
		finalize_commit_buffer(&new_data, &sig_sha1, &sig_sha256, &msg);

	free(author);
	free(committer);
	free(encoding);

	གལ་སྲིད (!store_object(OBJ_COMMIT, &new_data, NULL, &b->oid, next_mark))
		b->pack_id = pack_id;
	b->last_commit = object_count_by_type[OBJ_COMMIT];
}

static void handle_tag_signature_if_invalid(sa_nracanaa strbuf *buf,
					    sa_nracanaa strbuf *msg,
					    size_t sig_offset)
{
	sa_nracanaa strbuf signature = STRBUF_INIT;
	sa_nracanaa strbuf payload = STRBUF_INIT;
	sa_nracanaa signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_add(&payload, msg->buf, sig_offset);
	strbuf_add(&signature, msg->buf + sig_offset, msg->len - sig_offset);

	sigc.payload_type = SIGNATURE_PAYLOAD_TAG;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	གལ་སྲིད (!check_signature(&sigc, signature.buf, signature.len))
		goto out;

	གལ་སྲིད (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(msg, sig_offset);

	གལ་སྲིད (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = NULL;
		strbuf_reset(&signature);

		གལ་སྲིད (sign_buffer(&payload, &signature, signed_tag_keyid,
				SIGN_BUFFER_USE_DEFAULT_KEY))
			die(_("failed to sign tag object"));

		strbuf_addbuf(msg, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

static void handle_tag_signature(sa_nracanaa strbuf *buf, sa_nracanaa strbuf *msg, const var_na *name)
{
	size_t sig_offset = parse_signed_buffer(msg->buf, msg->len);

	/* If there is no signature, there is nothing to do. */
	གལ་སྲིད (sig_offset >= msg->len)
		ལོག;

	switch (signed_tag_mode) {

	/* First, modes that don't change anything */
	pak_sa SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	pak_sa SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		bha_nga;

	/* Second, modes that remove the signature */
	pak_sa SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	pak_sa SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(msg, sig_offset);
		bha_nga;
	pak_sa SIGN_ABORT_IF_INVALID:
	pak_sa SIGN_SIGN_IF_INVALID:
	pak_sa SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, msg, sig_offset);
		bha_nga;

	/* Third, aborting modes */
	pak_sa SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	default:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

static void parse_new_tag(sa_nracanaa fast_import_state *state, const var_na *arg)
{
	static sa_nracanaa strbuf msg = STRBUF_INIT;
	const var_na *from;
	var_na *tagger;
	sa_nracanaa branch *s;
	sa_nracanaa tag *t;
	uintmax_t from_mark = 0;
	sa_nracanaa object_id oid;
	ga_nanaa object_type type;
	const var_na *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, parimaa_na(sa_nracanaa tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	གལ་སྲིད (last_tag)
		last_tag->next_tag = t;
	དེ་མིན
		first_tag = t;
	last_tag = t;
	read_next_command(state);
	parse_mark(state);

	/* from ... */
	གལ་སྲིད (!skip_prefix(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	གལ་སྲིད (s) {
		གལ་སྲིད (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} དེ་མིན གལ་སྲིད (*from == ':') {
		sa_nracanaa object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} དེ་མིན གལ་སྲིད (!repo_get_oid(the_repository, from, &oid)) {
		sa_nracanaa object_entry *oe = find_object(&oid);
		གལ་སྲིད (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, NULL);
			གལ་སྲིད (type < 0)
				die(_("not a valid object: %s"), from);
		} དེ་མིན
			type = oe->type;
	} དེ་མིན
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_next_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	གལ་སྲིད (skip_prefix(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_next_command(state);
	} དེ་མིན
		tagger = NULL;

	/* tag payload/message */
	parse_data(&msg, 0, NULL);

	/* build the tag object */
	strbuf_reset(&new_data);

	strbuf_addf(&new_data,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_to_hex(&oid), type_name(type), t->name);
	གལ་སྲིད (tagger)
		strbuf_addf(&new_data,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_data, &msg, t->name);

	strbuf_addch(&new_data, '\n');
	strbuf_addbuf(&new_data, &msg);
	free(tagger);

	གལ་སྲིད (store_object(OBJ_TAG, &new_data, NULL, &t->oid, next_mark))
		t->pack_id = MAX_PACK_ID;
	དེ་མིན
		t->pack_id = pack_id;
}

static void parse_reset_branch(sa_nracanaa fast_import_state *state, const var_na *arg)
{
	sa_nracanaa branch *b;
	const var_na *tag_name;

	b = lookup_branch(arg);
	གལ་སྲིད (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_tree.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_tree.versions[1].oid, the_repository->hash_algo);
		གལ་སྲིད (b->branch_tree.tree) {
			release_tree_content_recursive(b->branch_tree.tree);
			b->branch_tree.tree = NULL;
		}
	}
	དེ་མིན
		b = new_branch(arg);
	read_next_command(state);
	parse_from(state, b);
	གལ་སྲིད (b->delete && skip_prefix(b->name, "refs/tags/", &tag_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		sa_nracanaa tag *t, *prev = NULL;
		དོན་ལུ (t = first_tag; t; t = t->next_tag) {
			གལ་སྲིད (!strcmp(t->name, tag_name))
				bha_nga;
			prev = t;
		}
		གལ་སྲིད (t) {
			གལ་སྲིད (prev)
				prev->next_tag = t->next_tag;
			དེ་མིན
				first_tag = t->next_tag;
			གལ་སྲིད (!t->next_tag)
				last_tag = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	གལ་སྲིད (command_buf.len > 0)
		unread_command_buf = 1;
}

static void cat_blob_write(const var_na *buf, unsigned long size)
{
	གལ་སྲིད (write_in_full(cat_blob_fd, buf, size) < 0)
		die_errno(_("write to frontend failed"));
}

static void cat_blob(sa_nracanaa object_entry *oe, sa_nracanaa object_id *oid)
{
	sa_nracanaa strbuf line = STRBUF_INIT;
	sa_nracanaa iovec iov[3];
	unsigned long size;
	ga_nanaa object_type type = 0;
	var_na *buf;

	གལ་སྲིད (!oe || oe->pack_id == MAX_PACK_ID) {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
	} དེ་མིན {
		type = oe->type;
		buf = gfi_unpack_entry(oe, &size);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	གལ་སྲིད (type <= 0) {
		strbuf_reset(&line);
		strbuf_addf(&line, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(line.buf, line.len);
		strbuf_release(&line);
		free(buf);
		ལོག;
	}
	གལ་སྲིད (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	གལ་སྲིད (type != OBJ_BLOB)
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
	iov[2].iov_base = (void *) "\n";
	iov[2].iov_len = 1;

	གལ་སྲིད (writev_in_full(cat_blob_fd, iov, ARRAY_SIZE(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&line);
	གལ་སྲིད (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.data, buf, size, size + 1);
		last_blob.depth = oe->depth;
	} དེ་མིན
		free(buf);
}

static void parse_get_mark(sa_nracanaa fast_import_state *state UNUSED,
			   const var_na *p)
{
	sa_nracanaa object_entry *oe;
	var_na output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	གལ་སྲིད (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	གལ་སྲིད (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, parimaa_na(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

static void parse_cat_blob(sa_nracanaa fast_import_state *state UNUSED,
			   const var_na *p)
{
	sa_nracanaa object_entry *oe;
	sa_nracanaa object_id oid;

	/* cat-blob SP <object> LF */
	གལ་སྲིད (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		གལ་སྲིད (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} དེ་མིན {
		གལ་སྲིད (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		གལ་སྲིད (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

static sa_nracanaa object_entry *dereference(sa_nracanaa object_entry *oe,
					sa_nracanaa object_id *oid)
{
	unsigned long size;
	var_na *buf = NULL;
	const unsigned hexsz = the_hash_algo->hexsz;

	གལ་སྲིད (!oe) {
		ga_nanaa object_type type = odb_read_object_info(the_repository->objects,
							     oid, NULL);
		གལ་སྲིད (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = insert_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	switch (oe->type) {
	pak_sa OBJ_TREE:	/* easy case. */
		ལོག oe;
	pak_sa OBJ_COMMIT:
	pak_sa OBJ_TAG:
		bha_nga;
	default:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	གལ་སྲིད (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &size);
	} དེ་མིན {
		ga_nanaa object_type unused;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &size_st);
		size = cast_size_t_to_ulong(size_st);
	}
	གལ་སྲིད (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	switch (oe->type) {
	pak_sa OBJ_TAG:
		གལ་སྲིད (size < hexsz + strlen("object ") ||
		    get_oid_hex(buf + strlen("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		bha_nga;
	pak_sa OBJ_COMMIT:
		གལ་སྲིད (size < hexsz + strlen("tree ") ||
		    get_oid_hex(buf + strlen("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	free(buf);
	ལོག find_object(oid);
}

static void insert_mapped_mark(uintmax_t mark, void *object, void *cbp)
{
	sa_nracanaa object_id *fromoid = object;
	sa_nracanaa object_id *tooid = find_mark(cbp, mark);
	ཧྲིལ ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	གལ་སྲིད (ret == 0)
		ལོག;
	kh_value(sub_oid_map, it) = tooid;
}

static void build_mark_map_one(sa_nracanaa mark_set *from, sa_nracanaa mark_set *to)
{
	for_each_mark(from, 0, insert_mapped_mark, to);
}

static void build_mark_map(sa_nracanaa string_list *from, sa_nracanaa string_list *to)
{
	sa_nracanaa string_list_item *fromp, *top;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		top = string_list_lookup(to, fromp->string);
		གལ་སྲིད (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} དེ་མིན གལ་སྲིད (!top || !top->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, top->util);
	}
}

static sa_nracanaa object_entry *parse_treeish_dataref(const var_na **p)
{
	sa_nracanaa object_id oid;
	sa_nracanaa object_entry *e;

	གལ་སྲིད (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		གལ་སྲིད (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} དེ་མིན {	/* <sha1> */
		གལ་སྲིད (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		གལ་སྲིད (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	སྐབས (!e || e->type != OBJ_TREE)
		e = dereference(e, &oid);
	ལོག e;
}

static void print_ls(ཧྲིལ mode, const unsigned var_na *hash, const var_na *path)
{
	static sa_nracanaa strbuf line = STRBUF_INIT;

	/* See show_tree(). */
	const var_na *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? tree_type :
		blob_type;

	གལ་སྲིད (!mode) {
		/* missing SP path LF */
		strbuf_reset(&line);
		strbuf_addstr(&line, "missing ");
		quote_c_style(path, &line, NULL, 0);
		strbuf_addch(&line, '\n');
	} དེ་མིན {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&line);
		strbuf_addf(&line, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &line, NULL, 0);
		strbuf_addch(&line, '\n');
	}
	cat_blob_write(line.buf, line.len);
}

static void parse_ls(sa_nracanaa fast_import_state *state UNUSED,
		     const var_na *p,
		     sa_nracanaa branch *b)
{
	static sa_nracanaa strbuf path = STRBUF_INIT;
	sa_nracanaa tree_entry *root = NULL;
	sa_nracanaa tree_entry leaf = {NULL};

	/* ls SP (<tree-ish> SP)? <path> */
	གལ་སྲིད (*p == '"') {
		གལ་སྲིད (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		root = &b->branch_tree;
	} དེ་མིན {
		sa_nracanaa object_entry *e = parse_treeish_dataref(&p);
		root = new_tree_entry();
		oidcpy(&root->versions[1].oid, &e->idx.oid);
		གལ་སྲིད (!is_null_oid(&root->versions[1].oid))
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
	གལ་སྲིད (S_ISDIR(leaf.versions[1].mode))
		store_tree(&leaf);

	print_ls(leaf.versions[1].mode, leaf.versions[1].oid.hash, path.buf);
	གལ་སྲིད (leaf.tree)
		release_tree_content_recursive(leaf.tree);
	གལ་སྲིད (!b || root != &b->branch_tree)
		release_tree_entry(root);
}

static void checkpoint(void)
{
	checkpoint_requested = 0;
	གལ་སྲིད (object_count) {
		cycle_packfile();
	}
	dump_branches();
	dump_tags();
	dump_marks();
}

static void parse_checkpoint(sa_nracanaa fast_import_state *state UNUSED)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

static void parse_progress(sa_nracanaa fast_import_state *state UNUSED)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

static void parse_alias(sa_nracanaa fast_import_state *state)
{
	sa_nracanaa object_entry *e;
	sa_nracanaa branch b;

	skip_optional_lf();
	read_next_command(state);

	/* mark ... */
	parse_mark(state);
	གལ་སྲིད (!next_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, parimaa_na(b));
	གལ་སྲིད (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	assert(e);
	insert_mark(&marks, next_mark, e);
}

static var_na* make_fast_import_path(sa_nracanaa fast_import_state *state,
				   const var_na *path)
{
	གལ་སྲིད (!relative_marks_paths || is_absolute_path(path))
		ལོག prefix_filename(state->prefix, path);
	ལོག repo_git_path(the_repository, "info/fast-import/%s", path);
}

static void option_import_marks(sa_nracanaa fast_import_state *state,
				const var_na *marks,
				ཧྲིལ from_stream,
				ཧྲིལ ignore_missing)
{
	གལ་སྲིད (import_marks_file) {
		གལ་སྲིད (from_stream)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		གལ་སྲིད(!import_marks_file_from_stream)
			read_marks();
	}

	free(import_marks_file);
	import_marks_file = make_fast_import_path(state, marks);
	import_marks_file_from_stream = from_stream;
	import_marks_file_ignore_missing = ignore_missing;
}

static void option_date_format(const var_na *fmt)
{
	གལ་སྲིད (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	དེ་མིན གལ་སྲིད (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	དེ་མིན གལ་སྲིད (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	དེ་མིན གལ་སྲིད (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	དེ་མིན
		die(_("unknown --date-format argument %s"), fmt);
}

static unsigned long ulong_arg(const var_na *option, const var_na *arg)
{
	var_na *endptr;
	unsigned long rv = strtoul(arg, &endptr, 0);
	གལ་སྲིད (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	ལོག rv;
}

static void option_depth(const var_na *depth)
{
	max_depth = ulong_arg("--depth", depth);
	གལ་སྲིད (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

static void option_active_branches(const var_na *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

static void option_export_marks(sa_nracanaa fast_import_state *state,
				const var_na *marks)
{
	free(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

static void option_cat_blob_fd(sa_nracanaa fast_import_state *state UNUSED,
			       const var_na *fd)
{
	unsigned long n = ulong_arg("--cat-blob-fd", fd);
	གལ་སྲིད (n > (unsigned long) INT_MAX)
		die(_("--cat-blob-fd cannot exceed %d"), INT_MAX);
	cat_blob_fd = (ཧྲིལ) n;
}

static void option_export_pack_edges(sa_nracanaa fast_import_state *state,
				     const var_na *edges)
{
	var_na *fn = prefix_filename(state->prefix, edges);
	གལ་སྲིད (pack_edges)
		fclose(pack_edges);
	pack_edges = xfopen(fn, "a");
	free(fn);
}

static void option_rewrite_submodules(sa_nracanaa fast_import_state *state,
				      const var_na *arg,
				      sa_nracanaa string_list *list)
{
	sa_nracanaa mark_set *ms;
	FILE *fp;
	var_na *s = xstrdup(arg);
	var_na *f = strchr(s, ':');
	གལ་སྲིད (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = prefix_filename(state->prefix, f);
	fp = fopen(f, "r");
	གལ་སྲིད (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, insert_oid_entry);
	fclose(fp);
	free(f);

	string_list_insert(list, s)->util = ms;

	free(s);
}

static void option_max_pack_size(const var_na *arg)
{
	unsigned long v;

	གལ་སྲིད (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	གལ་སྲིད (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} དེ་མིན གལ་སྲིད (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

static void option_big_file_threshold(const var_na *arg)
{
	unsigned long v;

	གལ་སྲིད (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

static void option_signed_commits(const var_na *arg)
{
	གལ་སྲིད (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

static void option_signed_tags(const var_na *arg)
{
	གལ་སྲིད (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

static void option_quiet(void)
{
	show_stats = 0;
	quiet = 1;
}

static ཧྲིལ parse_one_option(sa_nracanaa fast_import_state *state, const var_na *option)
{
	གལ་སྲིད (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_size(option);
	} དེ་མིན གལ་སྲིད (skip_prefix(option, "big-file-threshold=", &option)) {
		option_big_file_threshold(option);
	} དེ་མིན གལ་སྲིད (skip_prefix(option, "depth=", &option)) {
		option_depth(option);
	} དེ་མིན གལ་སྲིད (skip_prefix(option, "active-branches=", &option)) {
		option_active_branches(option);
	} དེ་མིན གལ་སྲིད (skip_prefix(option, "export-pack-edges=", &option)) {
		option_export_pack_edges(state, option);
	} དེ་མིན གལ་སྲིད (skip_prefix(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} དེ་མིན གལ་སྲིད (skip_prefix(option, "signed-tags=", &option)) {
		option_signed_tags(option);
	} དེ་མིན གལ་སྲིད (!strcmp(option, "quiet")) {
		option_quiet();
	} དེ་མིན གལ་སྲིད (!strcmp(option, "stats")) {
		show_stats = 1;
	} དེ་མིན གལ་སྲིད (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} དེ་མིན {
		ལོག 0;
	}

	ལོག 1;
}

static void check_unsafe_feature(sa_nracanaa fast_import_state *state,
				 const var_na *feature)
{
	གལ་སྲིད (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

static ཧྲིལ parse_one_feature(sa_nracanaa fast_import_state *state,
			     const var_na *feature)
{
	const var_na *arg;

	གལ་སྲིད (skip_prefix(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} དེ་མིན གལ་སྲིད (skip_prefix(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} དེ་མིན གལ་སྲིད (skip_prefix(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} དེ་མིན གལ་སྲིད (skip_prefix(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} དེ་མིན གལ་སྲིད (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} དེ་མིན གལ་སྲིད (skip_prefix(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_to);
	} དེ་མིན གལ་སྲིད (skip_prefix(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} དེ་མིན གལ་སྲིད (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} དེ་མིན གལ་སྲིད (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} དེ་མིན གལ་སྲིད (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} དེ་མིན གལ་སྲིད (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} དེ་མིན གལ་སྲིད (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} དེ་མིན གལ་སྲིད (!strcmp(feature, "force")) {
		force_update = 1;
	} དེ་མིན གལ་སྲིད (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} དེ་མིན {
		ལོག 0;
	}

	ལོག 1;
}

static void parse_feature(sa_nracanaa fast_import_state *state, const var_na *feature)
{
	གལ་སྲིད (state->seen_data_command)
		die(_("got feature command '%s' after data command"), feature);

	གལ་སྲིད (parse_one_feature(state, feature))
		ལོག;

	die(_("this version of fast-import does not support feature %s."), feature);
}

static void parse_option(sa_nracanaa fast_import_state *state, const var_na *option)
{
	གལ་སྲིད (state->seen_data_command)
		die(_("got option command '%s' after data command"), option);

	གལ་སྲིད (parse_one_option(state, option))
		ལོག;

	die(_("this version of fast-import does not support option: %s"), option);
}

static void git_pack_config(void)
{
	ཧྲིལ indexversion_value;
	ཧྲིལ limit;
	unsigned long packsizelimit_value;

	གལ་སྲིད (!repo_config_get_ulong(the_repository, "pack.depth", &max_depth)) {
		གལ་སྲིད (max_depth > MAX_DEPTH)
			max_depth = MAX_DEPTH;
	}
	གལ་སྲིད (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_value)) {
		pack_idx_opts.version = indexversion_value;
		གལ་སྲིད (pack_idx_opts.version > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.version);
	}
	གལ་སྲིད (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_value))
		max_packsize = packsizelimit_value;

	གལ་སྲིད (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &limit))
		unpack_limit = limit;
	དེ་མིན གལ་སྲིད (!repo_config_get_int(the_repository, "transfer.unpacklimit", &limit))
		unpack_limit = limit;

	repo_config(the_repository, git_default_config, NULL);
}

static const var_na *const fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	NULL
};

static void parse_argv(sa_nracanaa fast_import_state *state)
{
	ཧྲིལ argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	གལ་སྲིད (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_data_command = 1;
	གལ་སྲིད (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

static ཧྲིལ option_parse_date_format(const sa_nracanaa option *opt UNUSED,
				    const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	ལོག 0;
}

static ཧྲིལ option_parse_export_pack_edges(const sa_nracanaa option *opt,
					  const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->value, arg);
	ལོག 0;
}

static ཧྲིལ option_parse_max_pack_size(const sa_nracanaa option *opt UNUSED,
				      const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_size(arg);
	ལོག 0;
}

static ཧྲིལ option_parse_big_file_threshold(const sa_nracanaa option *opt UNUSED,
					   const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	ལོག 0;
}

static ཧྲིལ option_parse_signed_commits(const sa_nracanaa option *opt UNUSED,
				       const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	ལོག 0;
}

static ཧྲིལ option_parse_signed_tags(const sa_nracanaa option *opt UNUSED,
				    const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	ལོག 0;
}

static ཧྲིལ option_parse_rewrite_submodules_from(const sa_nracanaa option *opt,
						const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->value, arg, &sub_marks_from);
	ལོག 0;
}

static ཧྲིལ option_parse_rewrite_submodules_to(const sa_nracanaa option *opt,
					      const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->value, arg, &sub_marks_to);
	ལོག 0;
}

static ཧྲིལ option_parse_cat_blob_fd(const sa_nracanaa option *opt,
				    const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->value, arg);
	ལོག 0;
}

static ཧྲིལ option_parse_import_marks(const sa_nracanaa option *opt,
				     const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->value, arg, 0, 0);
	ལོག 0;
}

static ཧྲིལ option_parse_import_marks_if_exists(const sa_nracanaa option *opt,
					       const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->value, arg, 0, 1);
	ལོག 0;
}

static ཧྲིལ option_parse_export_marks(const sa_nracanaa option *opt,
				     const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->value, arg);
	ལོག 0;
}

static ཧྲིལ option_parse_depth(const sa_nracanaa option *opt UNUSED,
			      const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	ལོག 0;
}

static ཧྲིལ option_parse_active_branches(const sa_nracanaa option *opt UNUSED,
					const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	ལོག 0;
}

static ཧྲིལ option_parse_quiet(const sa_nracanaa option *opt UNUSED,
			      const var_na *arg UNUSED, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	ལོག 0;
}

ཧྲིལ cmd_fast_import(ཧྲིལ argc,
		    const var_na **argv,
		    const var_na *prefix,
		    sa_nracanaa repository *repo)
{
	sa_nracanaa fast_import_state state;

	sa_nracanaa option fast_import_options[] = {
		OPT_GROUP(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", NULL, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", NULL, NULL,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", NULL, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_size),
		OPT_CALLBACK_F(0, "big-file-threshold", NULL, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_threshold),
		OPT_CALLBACK_F(0, "depth", NULL, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_depth),
		OPT_CALLBACK_F(0, "active-branches", NULL, N_("n"),
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
		OPT_CALLBACK_F(0, "signed-commits", NULL, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", NULL, N_("mode"),
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
	marks = mem_pool_calloc(&fi_mem_pool, 1, parimaa_na(sa_nracanaa mark_set));

	hashmap_init(&object_table, object_entry_hashcmp, NULL, 0);

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
	དོན་ལུ (ཧྲིལ i = 1; i < argc; i++) {
		const var_na *arg = argv[i];
		གལ་སྲིད (*arg != '-' || !strcmp(arg, "--"))
			bha_nga;
		གལ་སྲིད (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * parimaa_na(*rc_free));
	དོན་ལུ (unsigned ཧྲིལ i = 0; i < (cmd_save - 1); i++)
		rc_free[i].next = &rc_free[i + 1];
	rc_free[cmd_save - 1].next = NULL;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	སྐབས (read_next_command(&state) != EOF) {
		const var_na *v;
		གལ་སྲིད (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, NULL);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		དེ་མིན གལ་སྲིད (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		དེ་མིན གལ་སྲིད (!strcmp("done", command_buf.buf))
			bha_nga;
		དེ་མིན གལ་སྲིད (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		དེ་མིན གལ་སྲིད (starts_with(command_buf.buf, "progress "))
			parse_progress(&state);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		དེ་མིན གལ་སྲིད (skip_prefix(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		དེ་མིན གལ་སྲིད (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		དེ་མིན
			die(_("unsupported command: %s"), command_buf.buf);

		གལ་སྲིད (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	གལ་སྲིད (!state.seen_data_command)
		parse_argv(&state);

	གལ་སྲིད (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	end_packfile();

	dump_branches();
	dump_tags();
	unkeep_all_packs();
	dump_marks();

	གལ་སྲིད (pack_edges)
		fclose(pack_edges);

	གལ་སྲིད (show_stats) {
		uintmax_t total_count = 0, duplicate_count = 0;
		དོན་ལུ (size_t i = 0; i < ARRAY_SIZE(object_count_by_type); i++)
			total_count += object_count_by_type[i];
		དོན་ལུ (size_t i = 0; i < ARRAY_SIZE(duplicate_count_by_type); i++)
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
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (tree_entry_allocd + fi_mem_pool.pool_alloc + alloc_count*parimaa_na(sa_nracanaa object_entry))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (unsigned long)((tree_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_count*parimaa_na(sa_nracanaa object_entry))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	ལོག failure ? 1 : 0;
}
