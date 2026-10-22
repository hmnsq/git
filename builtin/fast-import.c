#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "delta.h"
#अन्तर्भाव "pack.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "csum-file.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "mem-pool.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "khash.h"
#अन्तर्भाव "date.h"
#अन्तर्भाव "gpg-interface.h"
#अन्तर्भाव "parse-options.h"

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

संरचना object_entry {
	संरचना pack_idx_entry idx;
	संरचना hashmap_entry ent;
	uint32_t type : TYPE_BITS,
		pack_id : PACK_ID_BITS,
		depth : DEPTH_BITS;
};

स्थिर पूर्णाङ्क object_entry_hashcmp(ध्रुव शून्य *map_data UNUSED,
				ध्रुव संरचना hashmap_entry *eptr,
				ध्रुव संरचना hashmap_entry *entry_or_कुंजी,
				ध्रुव शून्य *keydata)
{
	ध्रुव संरचना object_id *oid = keydata;
	ध्रुव संरचना object_entry *e1, *e2;

	e1 = container_of(eptr, ध्रुव संरचना object_entry, ent);
	यदि (oid)
		प्रत्यागम oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_कुंजी, ध्रुव संरचना object_entry, ent);
	प्रत्यागम oidcmp(&e1->idx.oid, &e2->idx.oid);
}

संरचना object_entry_pool {
	संरचना object_entry_pool *next_pool;
	संरचना object_entry *next_free;
	संरचना object_entry *end;
	संरचना object_entry entries[FLEX_ARRAY]; /* more */
};

संरचना mark_set {
	सङ्घ {
		संरचना object_id *oids[1024];
		संरचना object_entry *marked[1024];
		संरचना mark_set *sets[1024];
	} data;
	unsigned पूर्णाङ्क shift;
};

संरचना last_object {
	संरचना strbuf data;
	off_t offset;
	unsigned पूर्णाङ्क depth;
	unsigned no_swap : 1;
};

संरचना atom_str {
	संरचना atom_str *next_atom;
	unsigned short str_len;
	वर्ण str_dat[FLEX_ARRAY]; /* more */
};

संरचना वृक्ष_content;
संरचना वृक्ष_entry {
	संरचना वृक्ष_content *वृक्ष;
	संरचना atom_str *name;
	संरचना वृक्ष_entry_ms {
		uint16_t mode;
		संरचना object_id oid;
	} versions[2];
};

संरचना वृक्ष_content {
	unsigned पूर्णाङ्क entry_capacity; /* must match avail_tree_content */
	unsigned पूर्णाङ्क entry_गिनती;
	unsigned पूर्णाङ्क delta_depth;
	संरचना वृक्ष_entry *entries[FLEX_ARRAY]; /* more */
};

संरचना avail_वृक्ष_content {
	unsigned पूर्णाङ्क entry_capacity; /* must match tree_content */
	संरचना avail_वृक्ष_content *next_avail;
};

संरचना branch {
	संरचना branch *table_next_branch;
	संरचना branch *active_next_branch;
	ध्रुव वर्ण *name;
	संरचना वृक्ष_entry branch_वृक्ष;
	uintmax_t last_commit;
	uintmax_t अंक_notes;
	unsigned active : 1;
	unsigned delete : 1;
	unsigned pack_id : PACK_ID_BITS;
	संरचना object_id oid;
};

संरचना tag {
	संरचना tag *next_tag;
	ध्रुव वर्ण *name;
	unsigned पूर्णाङ्क pack_id;
	संरचना object_id oid;
};

संरचना hash_list {
	संरचना hash_list *next;
	संरचना object_id oid;
};

प्रकारनिर्देश गणना {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

संरचना recent_command {
	संरचना recent_command *prev;
	संरचना recent_command *next;
	वर्ण *buf;
};

प्रकारनिर्देश शून्य (*mark_set_inserter_t)(संरचना mark_set **s, संरचना object_id *oid, uintmax_t mark);
प्रकारनिर्देश शून्य (*each_mark_fn_t)(uintmax_t mark, शून्य *obj, शून्य *cbp);

/* Configured limits on output */
स्थिर unsigned long max_depth = 50;
स्थिर off_t max_packsize;
स्थिर पूर्णाङ्क unpack_limit = 100;
स्थिर पूर्णाङ्क force_update;

/* Stats and misc. counters */
स्थिर uintmax_t alloc_गिनती;
स्थिर uintmax_t marks_set_गिनती;
स्थिर uintmax_t object_गिनती_by_type[1 << TYPE_BITS];
स्थिर uintmax_t duplicate_गिनती_by_type[1 << TYPE_BITS];
स्थिर uintmax_t delta_गिनती_by_type[1 << TYPE_BITS];
स्थिर uintmax_t delta_गिनती_attempts_by_type[1 << TYPE_BITS];
स्थिर unsigned long object_गिनती;
स्थिर unsigned long branch_गिनती;
स्थिर unsigned long branch_load_गिनती;
स्थिर पूर्णाङ्क failure;
स्थिर FILE *pack_edges;
स्थिर पूर्णाङ्क show_stats = 1;
स्थिर पूर्णाङ्क quiet;
स्थिर गणना sign_mode signed_tag_mode = SIGN_VERBATIM;
स्थिर गणना sign_mode signed_commit_mode = SIGN_VERBATIM;
स्थिर ध्रुव वर्ण *signed_commit_keyid;
स्थिर ध्रुव वर्ण *signed_tag_keyid;

/* Memory pools */
स्थिर संरचना mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - परिमाण(संरचना mp_block),
};

/* Atom management */
स्थिर unsigned पूर्णाङ्क atom_table_sz = 4451;
स्थिर unsigned पूर्णाङ्क atom_cnt;
स्थिर संरचना atom_str **atom_table;

/* The .pack file being generated */
स्थिर संरचना pack_idx_option pack_idx_opts;
स्थिर unsigned पूर्णाङ्क pack_id;
स्थिर संरचना hashfile *pack_file;
स्थिर संरचना packed_git *pack_data;
स्थिर संरचना packed_git **all_packs;
स्थिर off_t pack_size;

/* Table of objects we've written. */
स्थिर unsigned पूर्णाङ्क object_entry_alloc = 5000;
स्थिर संरचना object_entry_pool *blocks;
स्थिर संरचना hashmap object_table;
स्थिर संरचना mark_set *marks;
स्थिर वर्ण *export_marks_file;
स्थिर वर्ण *import_marks_file;
स्थिर पूर्णाङ्क import_marks_file_from_stream;
स्थिर पूर्णाङ्क import_marks_file_ignore_missing;
स्थिर पूर्णाङ्क import_marks_file_done;
स्थिर पूर्णाङ्क relative_marks_paths;

/* Our last blob */
स्थिर संरचना last_object last_blob = {
	.data = STRBUF_INIT,
 };

/* Tree management */
स्थिर unsigned पूर्णाङ्क वृक्ष_entry_alloc = 1000;
स्थिर शून्य *avail_वृक्ष_entry;
स्थिर unsigned पूर्णाङ्क avail_वृक्ष_table_sz = 100;
स्थिर संरचना avail_वृक्ष_content **avail_वृक्ष_table;
स्थिर size_t वृक्ष_entry_allocd;
स्थिर संरचना strbuf old_वृक्ष = STRBUF_INIT;
स्थिर संरचना strbuf new_वृक्ष = STRBUF_INIT;

/* Branch data */
स्थिर unsigned long max_active_branches = 5;
स्थिर unsigned long cur_active_branches;
स्थिर unsigned long branch_table_sz = 1039;
स्थिर संरचना branch **branch_table;
स्थिर संरचना branch *active_branches;

/* Tag data */
स्थिर संरचना tag *first_tag;
स्थिर संरचना tag *last_tag;

/* Input stream parsing */
स्थिर whenspec_type whenspec = WHENSPEC_RAW;
स्थिर संरचना strbuf command_buf = STRBUF_INIT;
स्थिर पूर्णाङ्क unread_command_buf;
स्थिर संरचना recent_command cmd_hist = {
	.prev = &cmd_hist,
	.next = &cmd_hist,
};
स्थिर संरचना recent_command *cmd_tail = &cmd_hist;
स्थिर संरचना recent_command *rc_free;
स्थिर unsigned पूर्णाङ्क cmd_save = 100;
स्थिर uintmax_t next_mark;
स्थिर संरचना strbuf new_data = STRBUF_INIT;
स्थिर पूर्णाङ्क require_explicit_termination;

/* Signal handling */
स्थिर चञ्चल sig_atomic_t checkpoint_requested;

/* Submodule marks */
स्थिर संरचना string_list sub_marks_from = STRING_LIST_INIT_DUP;
स्थिर संरचना string_list sub_marks_to = STRING_LIST_INIT_DUP;
स्थिर kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
स्थिर पूर्णाङ्क cat_blob_fd = STDOUT_FILENO;

/* Command state */
संरचना fast_import_state {
	पूर्णाङ्क argc;
	ध्रुव वर्ण **argv;
	ध्रुव वर्ण *prefix;
	पूर्णाङ्क seen_data_command;
	पूर्णाङ्क allow_unsafe_features;
	संरचना option *option;
};

स्थिर शून्य fast_import_state_init(संरचना fast_import_state *state,
				   पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				   ध्रुव वर्ण *prefix, संरचना option *option)
{
	memset(state, 0, परिमाण(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

स्थिर शून्य parse_argv(संरचना fast_import_state *state);
स्थिर शून्य parse_get_mark(संरचना fast_import_state *state, ध्रुव वर्ण *p);
स्थिर शून्य parse_cat_blob(संरचना fast_import_state *state, ध्रुव वर्ण *p);
स्थिर शून्य parse_ls(संरचना fast_import_state *state,
		     ध्रुव वर्ण *p,
		     संरचना branch *b);

स्थिर शून्य for_each_mark(संरचना mark_set *m, uintmax_t base, each_mark_fn_t callback, शून्य *p)
{
	uintmax_t k;
	यदि (m->shift) {
		प्रति (k = 0; k < 1024; k++) {
			यदि (m->data.sets[k])
				for_each_mark(m->data.sets[k], base + (k << m->shift), callback, p);
		}
	} अन्यथा {
		प्रति (k = 0; k < 1024; k++) {
			यदि (m->data.marked[k])
				callback(base + k, m->data.marked[k], p);
		}
	}
}

स्थिर शून्य dump_marks_fn(uintmax_t mark, शून्य *object, शून्य *cbp) {
	संरचना object_entry *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

स्थिर शून्य write_branch_report(FILE *rpt, संरचना branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	यदि (b->active)
		fputs(" active", rpt);
	यदि (b->branch_वृक्ष.वृक्ष)
		fputs(" loaded", rpt);
	यदि (is_null_oid(&b->branch_वृक्ष.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_to_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_to_hex(&b->branch_वृक्ष.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_to_hex(&b->branch_वृक्ष.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->last_commit);

	fputs("  last pack   : ", rpt);
	यदि (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

स्थिर शून्य write_crash_report(ध्रुव वर्ण *err)
{
	वर्ण *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	संरचना branch *b;
	unsigned long lu;
	संरचना recent_command *rc;

	यदि (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		मुक्त(loc);
		प्रत्यागम;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(time(रिक्त), 0, DATE_MODE(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	प्रति (rc = cmd_hist.next; rc != &cmd_hist; rc = rc->next) {
		यदि (rc->next == &cmd_hist)
			fputs("* ", rpt);
		अन्यथा
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
	प्रति (b = active_branches, lu = 0; b; b = b->active_next_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	प्रति (lu = 0; lu < branch_table_sz; lu++) {
		प्रति (b = branch_table[lu]; b; b = b->table_next_branch)
			write_branch_report(rpt, b);
	}

	यदि (first_tag) {
		संरचना tag *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		प्रति (tg = first_tag; tg; tg = tg->next_tag) {
			fputs(oid_to_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	यदि (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	अन्यथा
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	मुक्त(loc);
}

स्थिर शून्य end_packfile(शून्य);
स्थिर शून्य unkeep_all_packs(शून्य);
स्थिर शून्य dump_marks(शून्य);

स्थिर NORETURN शून्य die_nicely(ध्रुव वर्ण *err, va_list params)
{
	va_list cp;
	स्थिर पूर्णाङ्क zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	यदि (!zombie) {
		वर्ण message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, परिमाण(message), err, cp);
		write_crash_report(message);
		end_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	exit(128);
}

#ifndef SIGUSR1	/* Windows, for example */

स्थिर शून्य set_checkpoint_signal(शून्य)
{
}

#अन्यथा

स्थिर शून्य checkpoint_signal(पूर्णाङ्क signo UNUSED)
{
	checkpoint_requested = 1;
}

स्थिर शून्य set_checkpoint_signal(शून्य)
{
	संरचना sigaction sa;

	memset(&sa, 0, परिमाण(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, रिक्त);
}

#endif

स्थिर शून्य alloc_objects(unsigned पूर्णाङ्क cnt)
{
	संरचना object_entry_pool *b;

	b = xmalloc(परिमाण(संरचना object_entry_pool)
		+ cnt * परिमाण(संरचना object_entry));
	b->next_pool = blocks;
	b->next_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_गिनती += cnt;
}

स्थिर संरचना object_entry *new_object(संरचना object_id *oid)
{
	संरचना object_entry *e;

	यदि (blocks->next_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->next_free++;
	oidcpy(&e->idx.oid, oid);
	प्रत्यागम e;
}

स्थिर संरचना object_entry *find_object(संरचना object_id *oid)
{
	प्रत्यागम hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   संरचना object_entry, ent);
}

स्थिर संरचना object_entry *जोड़ो_object(संरचना object_id *oid)
{
	संरचना object_entry *e;
	unsigned पूर्णाङ्क hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					संरचना object_entry, ent);
	यदि (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	प्रत्यागम e;
}

स्थिर शून्य invalidate_pack_id(unsigned पूर्णाङ्क id)
{
	unsigned long lu;
	संरचना tag *t;
	संरचना hashmap_iter iter;
	संरचना object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		यदि (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	प्रति (lu = 0; lu < branch_table_sz; lu++) {
		संरचना branch *b;

		प्रति (b = branch_table[lu]; b; b = b->table_next_branch)
			यदि (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	प्रति (t = first_tag; t; t = t->next_tag)
		यदि (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

स्थिर unsigned पूर्णाङ्क hc_str(ध्रुव वर्ण *s, size_t len)
{
	unsigned पूर्णाङ्क r = 0;
	यावत् (len-- > 0)
		r = r * 31 + *s++;
	प्रत्यागम r;
}

स्थिर शून्य जोड़ो_mark(संरचना mark_set **शीर्ष, uintmax_t idnum, संरचना object_entry *oe)
{
	संरचना mark_set *s = *शीर्ष;

	यावत् ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, परिमाण(संरचना mark_set));
		s->shift = (*शीर्ष)->shift + 10;
		s->data.sets[0] = *शीर्ष;
		*शीर्ष = s;
	}
	यावत् (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		यदि (!s->data.sets[i]) {
			s->data.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, परिमाण(संरचना mark_set));
			s->data.sets[i]->shift = s->shift - 10;
		}
		s = s->data.sets[i];
	}
	यदि (!s->data.marked[idnum])
		marks_set_गिनती++;
	s->data.marked[idnum] = oe;
}

स्थिर शून्य *find_mark(संरचना mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	संरचना object_entry *oe = रिक्त;
	यदि ((idnum >> s->shift) < 1024) {
		यावत् (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->data.sets[i];
		}
		यदि (s)
			oe = s->data.marked[idnum];
	}
	यदि (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	प्रत्यागम oe;
}

स्थिर संरचना atom_str *to_atom(ध्रुव वर्ण *s, unsigned short len)
{
	unsigned पूर्णाङ्क hc = hc_str(s, len) % atom_table_sz;
	संरचना atom_str *c;

	प्रति (c = atom_table[hc]; c; c = c->next_atom)
		यदि (c->str_len == len && !strncmp(s, c->str_dat, len))
			प्रत्यागम c;

	c = mem_pool_alloc(&fi_mem_pool, परिमाण(संरचना atom_str) + len + 1);
	c->str_len = len;
	memcpy(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->next_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	प्रत्यागम c;
}

स्थिर संरचना branch *lookup_branch(ध्रुव वर्ण *name)
{
	unsigned पूर्णाङ्क hc = hc_str(name, strlen(name)) % branch_table_sz;
	संरचना branch *b;

	प्रति (b = branch_table[hc]; b; b = b->table_next_branch)
		यदि (!strcmp(name, b->name))
			प्रत्यागम b;
	प्रत्यागम रिक्त;
}

स्थिर संरचना branch *new_branch(ध्रुव वर्ण *name)
{
	unsigned पूर्णाङ्क hc = hc_str(name, strlen(name)) % branch_table_sz;
	संरचना branch *b = lookup_branch(name);

	यदि (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	यदि (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, परिमाण(संरचना branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_next_branch = branch_table[hc];
	b->branch_वृक्ष.versions[0].mode = S_IFDIR;
	b->branch_वृक्ष.versions[1].mode = S_IFDIR;
	b->अंक_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_गिनती++;
	प्रत्यागम b;
}

स्थिर unsigned पूर्णाङ्क hc_entries(unsigned पूर्णाङ्क cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	प्रत्यागम cnt < avail_वृक्ष_table_sz ? cnt : avail_वृक्ष_table_sz - 1;
}

स्थिर संरचना वृक्ष_content *new_वृक्ष_content(unsigned पूर्णाङ्क cnt)
{
	संरचना avail_वृक्ष_content *f, *l = रिक्त;
	संरचना वृक्ष_content *t;
	unsigned पूर्णाङ्क hc = hc_entries(cnt);

	प्रति (f = avail_वृक्ष_table[hc]; f; l = f, f = f->next_avail)
		यदि (f->entry_capacity >= cnt)
			भङ्ग;

	यदि (f) {
		यदि (l)
			l->next_avail = f->next_avail;
		अन्यथा
			avail_वृक्ष_table[hc] = f->next_avail;
	} अन्यथा {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, परिमाण(*t) + परिमाण(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (संरचना वृक्ष_content*)f;
	t->entry_गिनती = 0;
	t->delta_depth = 0;
	प्रत्यागम t;
}

स्थिर शून्य release_वृक्ष_entry(संरचना वृक्ष_entry *e);
स्थिर शून्य release_वृक्ष_content(संरचना वृक्ष_content *t)
{
	संरचना avail_वृक्ष_content *f = (संरचना avail_वृक्ष_content*)t;
	unsigned पूर्णाङ्क hc = hc_entries(f->entry_capacity);
	f->next_avail = avail_वृक्ष_table[hc];
	avail_वृक्ष_table[hc] = f;
}

स्थिर शून्य release_वृक्ष_content_recursive(संरचना वृक्ष_content *t)
{
	unsigned पूर्णाङ्क i;
	प्रति (i = 0; i < t->entry_गिनती; i++)
		release_वृक्ष_entry(t->entries[i]);
	release_वृक्ष_content(t);
}

स्थिर संरचना वृक्ष_content *grow_वृक्ष_content(
	संरचना वृक्ष_content *t,
	पूर्णाङ्क amt)
{
	संरचना वृक्ष_content *r = new_वृक्ष_content(t->entry_गिनती + amt);
	r->entry_गिनती = t->entry_गिनती;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_गिनती);
	release_वृक्ष_content(t);
	प्रत्यागम r;
}

स्थिर संरचना वृक्ष_entry *new_वृक्ष_entry(शून्य)
{
	संरचना वृक्ष_entry *e;

	यदि (!avail_वृक्ष_entry) {
		unsigned पूर्णाङ्क n = वृक्ष_entry_alloc;
		वृक्ष_entry_allocd += n * परिमाण(संरचना वृक्ष_entry);
		ALLOC_ARRAY(e, n);
		avail_वृक्ष_entry = e;
		यावत् (n-- > 1) {
			*((शून्य**)e) = e + 1;
			e++;
		}
		*((शून्य**)e) = रिक्त;
	}

	e = avail_वृक्ष_entry;
	avail_वृक्ष_entry = *((शून्य**)e);
	प्रत्यागम e;
}

स्थिर शून्य release_वृक्ष_entry(संरचना वृक्ष_entry *e)
{
	यदि (e->वृक्ष)
		release_वृक्ष_content_recursive(e->वृक्ष);
	*((शून्य**)e) = avail_वृक्ष_entry;
	avail_वृक्ष_entry = e;
}

स्थिर संरचना वृक्ष_content *dup_वृक्ष_content(संरचना वृक्ष_content *s)
{
	संरचना वृक्ष_content *d;
	संरचना वृक्ष_entry *a, *b;
	unsigned पूर्णाङ्क i;

	यदि (!s)
		प्रत्यागम रिक्त;
	d = new_वृक्ष_content(s->entry_गिनती);
	प्रति (i = 0; i < s->entry_गिनती; i++) {
		a = s->entries[i];
		b = new_वृक्ष_entry();
		memcpy(b, a, परिमाण(*a));
		यदि (a->वृक्ष && is_null_oid(&b->versions[1].oid))
			b->वृक्ष = dup_वृक्ष_content(a->वृक्ष);
		अन्यथा
			b->वृक्ष = रिक्त;
		d->entries[i] = b;
	}
	d->entry_गिनती = s->entry_गिनती;
	d->delta_depth = s->delta_depth;

	प्रत्यागम d;
}

स्थिर शून्य start_packfile(शून्य)
{
	संरचना strbuf tmp_file = STRBUF_INIT;
	संरचना packed_git *p;
	पूर्णाङ्क pack_fd;

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
	object_गिनती = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

स्थिर ध्रुव वर्ण *create_index(शून्य)
{
	ध्रुव वर्ण *tmpfile;
	संरचना pack_idx_entry **idx, **c, **last;
	संरचना object_entry *e;
	संरचना object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_गिनती);
	c = idx;
	प्रति (o = blocks; o; o = o->next_pool)
		प्रति (e = o->next_free; e-- != o->entries;)
			यदि (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_गिनती;
	यदि (c != last)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, रिक्त, idx, object_गिनती,
				 &pack_idx_opts, pack_data->hash);
	मुक्त(idx);
	प्रत्यागम tmpfile;
}

स्थिर वर्ण *keep_pack(ध्रुव वर्ण *curr_index_name)
{
	स्थिर ध्रुव वर्ण *keep_संदेश = "fast-import";
	संरचना strbuf name = STRBUF_INIT;
	पूर्णाङ्क keep_fd;

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_data->repo,
							    name.buf);
	यदि (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_संदेश, strlen(keep_संदेश));
	यदि (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "pack");
	यदि (finalize_object_file(pack_data->repo, pack_data->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "idx");
	यदि (finalize_object_file(pack_data->repo, curr_index_name, name.buf))
		die(_("cannot store index file"));
	मुक्त((शून्य *)curr_index_name);
	प्रत्यागम strbuf_detach(&name, रिक्त);
}

स्थिर शून्य unkeep_all_packs(शून्य)
{
	संरचना strbuf name = STRBUF_INIT;
	पूर्णाङ्क k;

	प्रति (k = 0; k < pack_id; k++) {
		संरचना packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

स्थिर पूर्णाङ्क loosen_small_pack(ध्रुव संरचना packed_git *p)
{
	संरचना child_process unpack = CHILD_PROCESS_INIT;

	यदि (lseek(p->pack_fd, 0, SEEK_SET) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_धकेलो(&unpack.args, "unpack-objects");
	यदि (!show_stats)
		strvec_धकेलो(&unpack.args, "-q");

	प्रत्यागम run_command(&unpack);
}

स्थिर शून्य end_packfile(शून्य)
{
	स्थिर पूर्णाङ्क running;

	यदि (running || !pack_data)
		प्रत्यागम;

	running = 1;
	clear_delta_base_cache();
	यदि (object_गिनती) {
		संरचना odb_source_files *files = odb_source_files_downcast(pack_data->repo->objects->source);
		संरचना packed_git *new_p;
		संरचना object_id cur_pack_oid;
		वर्ण *idx_name;
		पूर्णाङ्क i;
		संरचना branch *b;
		संरचना tag *t;

		close_pack_windows(pack_data);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_data->pack_fd,
					 pack_data->hash, pack_data->pack_name,
					 object_गिनती, cur_pack_oid.hash,
					 pack_size);

		यदि (object_गिनती <= unpack_limit) {
			यदि (!loosen_small_pack(pack_data)) {
				invalidate_pack_id(pack_id);
				गच्छ discard_pack;
			}
		}

		close(pack_data->pack_fd);
		idx_name = keep_pack(create_index());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		यदि (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		मुक्त(idx_name);

		/* Print the boundary */
		यदि (pack_edges) {
			fprintf(pack_edges, "%s:", new_p->pack_name);
			प्रति (i = 0; i < branch_table_sz; i++) {
				प्रति (b = branch_table[i]; b; b = b->table_next_branch) {
					यदि (b->pack_id == pack_id)
						fprintf(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			प्रति (t = first_tag; t; t = t->next_tag) {
				यदि (t->pack_id == pack_id)
					fprintf(pack_edges, " %s",
						oid_to_hex(&t->oid));
			}
			fputc('\n', pack_edges);
			fflush(pack_edges);
		}

		pack_id++;
	}
	अन्यथा {
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

स्थिर शून्य cycle_packfile(शून्य)
{
	end_packfile();
	start_packfile();
}

स्थिर पूर्णाङ्क store_object(
	गणना object_type type,
	संरचना strbuf *dat,
	संरचना last_object *last,
	संरचना object_id *oidout,
	uintmax_t mark)
{
	संरचना odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	शून्य *out, *delta;
	संरचना object_entry *e;
	unsigned वर्ण hdr[96];
	संरचना object_id oid;
	unsigned long hdrlen, deltalen = 0;
	संरचना git_hash_ctx c;
	git_zstream s;
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	hdrlen = format_object_header((वर्ण *)hdr, परिमाण(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	यदि (oidout)
		oidcpy(oidout, &oid);

	e = जोड़ो_object(&oid);
	यदि (mark)
		जोड़ो_mark(&marks, mark, e);
	यदि (e->idx.offset) {
		duplicate_गिनती_by_type[type]++;
		प्रत्यागम 1;
	}

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		यदि (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			अनुवर्त;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_गिनती_by_type[type]++;
		प्रत्यागम 1;
	}

	यदि (last && last->data.len && last->data.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		size_t deltalen_st;

		delta_गिनती_attempts_by_type[type]++;
		delta = diff_delta(last->data.buf, last->data.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_size_t_to_ulong(deltalen_st);
	} अन्यथा
		delta = रिक्त;

	git_deflate_init(&s, cfg->pack_compression_level);
	यदि (delta) {
		s.next_in = delta;
		s.avail_in = deltalen;
	} अन्यथा {
		s.next_in = (शून्य *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.next_out = out = xmalloc(s.avail_out);
	यावत् (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	यदि ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + s.total_out) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + s.total_out) < pack_size) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		यदि (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.next_in = (शून्य *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.next_out = out = xrealloc(out, s.avail_out);
			यावत् (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_end(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.offset = pack_size;
	object_गिनती++;
	object_गिनती_by_type[type]++;

	crc32_begin(pack_file);

	यदि (delta) {
		off_t ofs = e->idx.offset - last->offset;
		unsigned pos = परिमाण(hdr) - 1;

		delta_गिनती_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, परिमाण(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;

		hdr[pos] = ofs & 127;
		यावत् (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, परिमाण(hdr) - pos);
		pack_size += परिमाण(hdr) - pos;
	} अन्यथा {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, परिमाण(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;
	}

	hashwrite(pack_file, out, s.total_out);
	pack_size += s.total_out;

	e->idx.crc32 = crc32_end(pack_file);

	मुक्त(out);
	मुक्त(delta);
	यदि (last) {
		यदि (last->no_swap) {
			last->data = *dat;
		} अन्यथा {
			strbuf_swap(&last->data, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	प्रत्यागम 0;
}

स्थिर शून्य truncate_pack(संरचना hashfile_checkpoint *checkpoint)
{
	यदि (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_size = checkpoint->offset;
}

स्थिर शून्य stream_blob(uintmax_t len, संरचना object_id *oidout, uintmax_t mark)
{
	संरचना odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	size_t in_sz = 64 * 1024, out_sz = 64 * 1024;
	unsigned वर्ण *in_buf = xmalloc(in_sz);
	unsigned वर्ण *out_buf = xmalloc(out_sz);
	संरचना object_entry *e;
	संरचना object_id oid;
	unsigned long hdrlen;
	off_t offset;
	संरचना git_hash_ctx c;
	git_zstream s;
	संरचना hashfile_checkpoint checkpoint;
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	पूर्णाङ्क status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	यदि ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + len) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + len) < pack_size)
		cycle_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	offset = checkpoint.offset;

	hdrlen = format_object_header((वर्ण *)out_buf, out_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, out_buf, hdrlen);

	crc32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_in_pack_object_header(out_buf, out_sz, OBJ_BLOB, len);

	s.next_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	यावत् (status != Z_STREAM_END) {
		यदि (0 < len && !s.avail_in) {
			size_t cnt = in_sz < len ? in_sz : (size_t)len;
			size_t n = fread(in_buf, 1, cnt, stdin);
			यदि (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.next_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		यदि (!s.avail_out || status == Z_STREAM_END) {
			size_t n = s.next_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_size += n;
			s.next_out = out_buf;
			s.avail_out = out_sz;
		}

		विकल्प (status) {
		पक्ष Z_OK:
		पक्ष Z_BUF_ERROR:
		पक्ष Z_STREAM_END:
			अनुवर्त;
		स्वभाव:
			die(_("unexpected deflate failure: %d"), status);
		}
	}
	git_deflate_end(&s);
	git_hash_final_oid(&oid, &c);

	यदि (oidout)
		oidcpy(oidout, &oid);

	e = जोड़ो_object(&oid);

	यदि (mark)
		जोड़ो_mark(&marks, mark, e);

	यदि (e->idx.offset) {
		duplicate_गिनती_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		गच्छ out;
	}

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		यदि (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			अनुवर्त;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_गिनती_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		गच्छ out;
	}

	e->depth = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.offset = offset;
	e->idx.crc32 = crc32_end(pack_file);
	object_गिनती++;
	object_गिनती_by_type[OBJ_BLOB]++;

out:
	मुक्त(in_buf);
	मुक्त(out_buf);
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
स्थिर शून्य *gfi_unpack_entry(
	संरचना object_entry *oe,
	unsigned long *sizep)
{
	गणना object_type type;
	size_t size_st = 0;
	शून्य *data;
	संरचना packed_git *p = all_packs[oe->pack_id];
	यदि (p == pack_data && p->pack_size < (pack_size + the_hash_algo->rawsz)) {
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
	यदि (sizep)
		*sizep = cast_size_t_to_ulong(size_st);
	प्रत्यागम data;
}

स्थिर शून्य load_वृक्ष(संरचना वृक्ष_entry *मूल)
{
	संरचना object_id *oid = &मूल->versions[1].oid;
	संरचना object_entry *myoe;
	संरचना वृक्ष_content *t;
	unsigned long size;
	वर्ण *buf;
	ध्रुव वर्ण *c;

	मूल->वृक्ष = t = new_वृक्ष_content(8);
	यदि (is_null_oid(oid))
		प्रत्यागम;

	myoe = find_object(oid);
	यदि (myoe && myoe->pack_id != MAX_PACK_ID) {
		यदि (myoe->type != OBJ_वृक्ष)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &size);
		यदि (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} अन्यथा {
		गणना object_type type;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		यदि (!buf || type != OBJ_वृक्ष)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	यावत् (c != (buf + size)) {
		संरचना वृक्ष_entry *e = new_वृक्ष_entry();

		यदि (t->entry_गिनती == t->entry_capacity)
			मूल->वृक्ष = t = grow_वृक्ष_content(t, t->entry_गिनती);
		t->entries[t->entry_गिनती++] = e;

		e->वृक्ष = रिक्त;
		c = parse_mode(c, &e->versions[1].mode);
		यदि (!c)
			die(_("corrupt mode in %s"), oid_to_hex(oid));
		e->versions[0].mode = e->versions[1].mode;
		e->name = to_atom(c, strlen(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (unsigned वर्ण *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (unsigned वर्ण *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	मुक्त(buf);
}

स्थिर पूर्णाङ्क tecmp0 (ध्रुव शून्य *_a, ध्रुव शून्य *_b)
{
	संरचना वृक्ष_entry *a = *((संरचना वृक्ष_entry**)_a);
	संरचना वृक्ष_entry *b = *((संरचना वृक्ष_entry**)_b);
	प्रत्यागम base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

स्थिर पूर्णाङ्क tecmp1 (ध्रुव शून्य *_a, ध्रुव शून्य *_b)
{
	संरचना वृक्ष_entry *a = *((संरचना वृक्ष_entry**)_a);
	संरचना वृक्ष_entry *b = *((संरचना वृक्ष_entry**)_b);
	प्रत्यागम base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

स्थिर शून्य mktree(संरचना वृक्ष_content *t, पूर्णाङ्क v, संरचना strbuf *b)
{
	size_t maxlen = 0;
	unsigned पूर्णाङ्क i;

	यदि (!v)
		QSORT(t->entries, t->entry_गिनती, tecmp0);
	अन्यथा
		QSORT(t->entries, t->entry_गिनती, tecmp1);

	प्रति (i = 0; i < t->entry_गिनती; i++) {
		यदि (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	प्रति (i = 0; i < t->entry_गिनती; i++) {
		संरचना वृक्ष_entry *e = t->entries[i];
		यदि (!e->versions[v].mode)
			अनुवर्त;
		strbuf_addf(b, "%o %s%c",
			(unsigned पूर्णाङ्क)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

स्थिर शून्य store_वृक्ष(संरचना वृक्ष_entry *मूल)
{
	संरचना वृक्ष_content *t;
	unsigned पूर्णाङ्क i, j, del;
	संरचना last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	संरचना object_entry *le = रिक्त;

	यदि (!is_null_oid(&मूल->versions[1].oid))
		प्रत्यागम;

	यदि (!मूल->वृक्ष)
		load_वृक्ष(मूल);
	t = मूल->वृक्ष;

	प्रति (i = 0; i < t->entry_गिनती; i++) {
		यदि (t->entries[i]->वृक्ष)
			store_वृक्ष(t->entries[i]);
	}

	यदि (!(मूल->versions[0].mode & NO_DELTA))
		le = find_object(&मूल->versions[0].oid);
	यदि (S_ISDIR(मूल->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_वृक्ष);
		lo.data = old_वृक्ष;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_वृक्ष);
	store_object(OBJ_वृक्ष, &new_वृक्ष, &lo, &मूल->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	प्रति (i = 0, j = 0, del = 0; i < t->entry_गिनती; i++) {
		संरचना वृक्ष_entry *e = t->entries[i];
		यदि (e->versions[1].mode) {
			e->versions[0].mode = e->versions[1].mode;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} अन्यथा {
			release_वृक्ष_entry(e);
			del++;
		}
	}
	t->entry_गिनती -= del;
}

स्थिर शून्य वृक्ष_content_replace(
	संरचना वृक्ष_entry *मूल,
	ध्रुव संरचना object_id *oid,
	ध्रुव uint16_t mode,
	संरचना वृक्ष_content *newtree)
{
	यदि (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&मूल->versions[0].oid, the_repository->hash_algo);
	oidcpy(&मूल->versions[1].oid, oid);
	यदि (मूल->वृक्ष)
		release_वृक्ष_content_recursive(मूल->वृक्ष);
	मूल->वृक्ष = newtree;
}

स्थिर पूर्णाङ्क वृक्ष_content_set(
	संरचना वृक्ष_entry *मूल,
	ध्रुव वर्ण *p,
	ध्रुव संरचना object_id *oid,
	ध्रुव uint16_t mode,
	संरचना वृक्ष_content *subtree)
{
	संरचना वृक्ष_content *t;
	ध्रुव वर्ण *slash1;
	unsigned पूर्णाङ्क i, n;
	संरचना वृक्ष_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	यदि (!n)
		die(_("empty path component found in input"));
	यदि (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	यदि (!मूल->वृक्ष)
		load_वृक्ष(मूल);
	t = मूल->वृक्ष;
	प्रति (i = 0; i < t->entry_गिनती; i++) {
		e = t->entries[i];
		यदि (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			यदि (!*slash1) {
				यदि (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					प्रत्यागम 0;
				e->versions[1].mode = mode;
				oidcpy(&e->versions[1].oid, oid);
				यदि (e->वृक्ष)
					release_वृक्ष_content_recursive(e->वृक्ष);
				e->वृक्ष = subtree;

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
				यदि (S_ISDIR(e->versions[0].mode))
					e->versions[0].mode |= NO_DELTA;

				oidclr(&मूल->versions[1].oid, the_repository->hash_algo);
				प्रत्यागम 1;
			}
			यदि (!S_ISDIR(e->versions[1].mode)) {
				e->वृक्ष = new_वृक्ष_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			यदि (!e->वृक्ष)
				load_वृक्ष(e);
			यदि (वृक्ष_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&मूल->versions[1].oid, the_repository->hash_algo);
				प्रत्यागम 1;
			}
			प्रत्यागम 0;
		}
	}

	यदि (t->entry_गिनती == t->entry_capacity)
		मूल->वृक्ष = t = grow_वृक्ष_content(t, t->entry_गिनती);
	e = new_वृक्ष_entry();
	e->name = to_atom(p, n);
	e->versions[0].mode = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->entry_गिनती++] = e;
	यदि (*slash1) {
		e->वृक्ष = new_वृक्ष_content(8);
		e->versions[1].mode = S_IFDIR;
		वृक्ष_content_set(e, slash1 + 1, oid, mode, subtree);
	} अन्यथा {
		e->वृक्ष = subtree;
		e->versions[1].mode = mode;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&मूल->versions[1].oid, the_repository->hash_algo);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क वृक्ष_content_remove(
	संरचना वृक्ष_entry *मूल,
	ध्रुव वर्ण *p,
	संरचना वृक्ष_entry *backup_leaf,
	पूर्णाङ्क allow_मूल)
{
	संरचना वृक्ष_content *t;
	ध्रुव वर्ण *slash1;
	unsigned पूर्णाङ्क i, n;
	संरचना वृक्ष_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	यदि (!मूल->वृक्ष)
		load_वृक्ष(मूल);

	यदि (!*p && allow_मूल) {
		e = मूल;
		गच्छ del_entry;
	}

	t = मूल->वृक्ष;
	प्रति (i = 0; i < t->entry_गिनती; i++) {
		e = t->entries[i];
		यदि (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			यदि (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				प्रत्यागम 1;
			यदि (!*slash1 || !S_ISDIR(e->versions[1].mode))
				गच्छ del_entry;
			यदि (!e->वृक्ष)
				load_वृक्ष(e);
			यदि (वृक्ष_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				प्रति (n = 0; n < e->वृक्ष->entry_गिनती; n++) {
					यदि (e->वृक्ष->entries[n]->versions[1].mode) {
						oidclr(&मूल->versions[1].oid,
						       the_repository->hash_algo);
						प्रत्यागम 1;
					}
				}
				backup_leaf = रिक्त;
				गच्छ del_entry;
			}
			प्रत्यागम 0;
		}
	}
	प्रत्यागम 0;

del_entry:
	यदि (backup_leaf)
		memcpy(backup_leaf, e, परिमाण(*backup_leaf));
	अन्यथा यदि (e->वृक्ष)
		release_वृक्ष_content_recursive(e->वृक्ष);
	e->वृक्ष = रिक्त;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&मूल->versions[1].oid, the_repository->hash_algo);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क वृक्ष_content_get(
	संरचना वृक्ष_entry *मूल,
	ध्रुव वर्ण *p,
	संरचना वृक्ष_entry *leaf,
	पूर्णाङ्क allow_मूल)
{
	संरचना वृक्ष_content *t;
	ध्रुव वर्ण *slash1;
	unsigned पूर्णाङ्क i, n;
	संरचना वृक्ष_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	यदि (!n && !allow_मूल)
		die(_("empty path component found in input"));

	यदि (!मूल->वृक्ष)
		load_वृक्ष(मूल);

	यदि (!n) {
		e = मूल;
		गच्छ found_entry;
	}

	t = मूल->वृक्ष;
	प्रति (i = 0; i < t->entry_गिनती; i++) {
		e = t->entries[i];
		यदि (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			यदि (!*slash1)
				गच्छ found_entry;
			यदि (!S_ISDIR(e->versions[1].mode))
				प्रत्यागम 0;
			यदि (!e->वृक्ष)
				load_वृक्ष(e);
			प्रत्यागम वृक्ष_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	प्रत्यागम 0;

found_entry:
	memcpy(leaf, e, परिमाण(*leaf));
	यदि (e->वृक्ष && is_null_oid(&e->versions[1].oid))
		leaf->वृक्ष = dup_वृक्ष_content(e->वृक्ष);
	अन्यथा
		leaf->वृक्ष = रिक्त;
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क update_branch(संरचना branch *b)
{
	स्थिर ध्रुव वर्ण *संदेश = "fast-import";
	संरचना ref_transaction *transaction;
	संरचना object_id old_oid;
	संरचना strbuf err = STRBUF_INIT;
	स्थिर ध्रुव वर्ण *replace_prefix = "refs/replace/";

	यदि (starts_with(b->name, replace_prefix) &&
	    !strcmp(b->name + strlen(replace_prefix),
		    oid_to_hex(&b->oid))) {
		यदि (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				रिक्त, b->name, रिक्त, 0);
		प्रत्यागम 0;
	}
	यदि (is_null_oid(&b->oid)) {
		यदि (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					रिक्त, b->name, रिक्त, 0);
		प्रत्यागम 0;
	}
	यदि (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	यदि (!force_update && !is_null_oid(&old_oid)) {
		संरचना commit *old_cmit, *new_cmit;
		पूर्णाङ्क ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		यदि (!old_cmit || !new_cmit)
			प्रत्यागम error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		यदि (ret < 0)
			exit(128);
		यदि (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			प्रत्यागम -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	यदि (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   रिक्त, रिक्त, 0, संदेश, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		प्रत्यागम -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	प्रत्यागम 0;
}

स्थिर शून्य dump_branches(शून्य)
{
	unsigned पूर्णाङ्क i;
	संरचना branch *b;

	प्रति (i = 0; i < branch_table_sz; i++) {
		प्रति (b = branch_table[i]; b; b = b->table_next_branch)
			failure |= update_branch(b);
	}
}

स्थिर शून्य dump_tags(शून्य)
{
	स्थिर ध्रुव वर्ण *संदेश = "fast-import";
	संरचना tag *t;
	संरचना strbuf ref_name = STRBUF_INIT;
	संरचना strbuf err = STRBUF_INIT;
	संरचना ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	यदि (!transaction) {
		failure |= error("%s", err.buf);
		गच्छ cleanup;
	}
	प्रति (t = first_tag; t; t = t->next_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		यदि (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, रिक्त, रिक्त, रिक्त,
					   0, संदेश, &err)) {
			failure |= error("%s", err.buf);
			गच्छ cleanup;
		}
	}
	यदि (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

स्थिर शून्य dump_marks(शून्य)
{
	संरचना lock_file mark_lock = LOCK_INIT;
	FILE *f;

	यदि (!export_marks_file || (import_marks_file && !import_marks_file_done))
		प्रत्यागम;

	यदि (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		प्रत्यागम;
	}

	यदि (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		प्रत्यागम;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	यदि (!f) {
		पूर्णाङ्क saved_errno = errno;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		प्रत्यागम;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	यदि (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		प्रत्यागम;
	}
}

स्थिर शून्य जोड़ो_object_entry(संरचना mark_set **s, संरचना object_id *oid, uintmax_t mark)
{
	संरचना object_entry *e;
	e = find_object(oid);
	यदि (!e) {
		गणना object_type type = odb_read_object_info(the_repository->objects,
							     oid, रिक्त);
		यदि (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = जोड़ो_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	जोड़ो_mark(s, mark, e);
}

स्थिर शून्य जोड़ो_oid_entry(संरचना mark_set **s, संरचना object_id *oid, uintmax_t mark)
{
	जोड़ो_mark(s, mark, xmemdupz(oid, परिमाण(*oid)));
}

स्थिर शून्य read_mark_file(संरचना mark_set **s, FILE *f, mark_set_inserter_t inserter)
{
	वर्ण पंक्ति[512];
	यावत् (fgets(पंक्ति, परिमाण(पंक्ति), f)) {
		uintmax_t mark;
		वर्ण *end;
		संरचना object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, परिमाण(oid.hash));

		end = strchr(पंक्ति, '\n');
		यदि (पंक्ति[0] != ':' || !end)
			die(_("corrupt mark line: %s"), पंक्ति);
		*end = 0;
		mark = strtoumax(पंक्ति + 1, &end, 10);
		यदि (!mark || end == पंक्ति + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), पंक्ति);
		inserter(s, &oid, mark);
	}
}

स्थिर शून्य read_marks(शून्य)
{
	FILE *f = fopen(import_marks_file, "r");
	यदि (f)
		;
	अन्यथा यदि (import_marks_file_ignore_missing && errno == ENOENT)
		गच्छ done; /* Marks file does not exist */
	अन्यथा
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, जोड़ो_object_entry);
	fclose(f);
done:
	import_marks_file_done = 1;
}


स्थिर पूर्णाङ्क read_next_command(संरचना fast_import_state *state)
{
	स्थिर पूर्णाङ्क stdin_eof = 0;

	यदि (stdin_eof) {
		unread_command_buf = 0;
		प्रत्यागम EOF;
	}

	प्रति (;;) {
		यदि (unread_command_buf) {
			unread_command_buf = 0;
		} अन्यथा {
			संरचना recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			यदि (stdin_eof)
				प्रत्यागम EOF;

			यदि (!state->seen_data_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			यदि (rc)
				rc_free = rc->next;
			अन्यथा {
				rc = cmd_hist.next;
				cmd_hist.next = rc->next;
				cmd_hist.next->prev = &cmd_hist;
				मुक्त(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_tail;
			rc->next = cmd_hist.prev;
			rc->prev->next = rc;
			cmd_tail = rc;
		}
		यदि (command_buf.buf[0] == '#')
			अनुवर्त;
		प्रत्यागम 0;
	}
}

स्थिर शून्य skip_optional_lf(शून्य)
{
	पूर्णाङ्क term_char = fgetc(stdin);
	यदि (term_char != '\n' && term_char != EOF)
		ungetc(term_char, stdin);
}

स्थिर शून्य parse_mark(संरचना fast_import_state *state)
{
	ध्रुव वर्ण *v;
	यदि (skip_prefix(command_buf.buf, "mark :", &v)) {
		next_mark = strtoumax(v, रिक्त, 10);
		read_next_command(state);
	}
	अन्यथा
		next_mark = 0;
}

स्थिर शून्य parse_original_identifier(संरचना fast_import_state *state)
{
	ध्रुव वर्ण *v;
	यदि (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_next_command(state);
}

स्थिर पूर्णाङ्क parse_data(संरचना strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	ध्रुव वर्ण *data;
	strbuf_reset(sb);

	यदि (!skip_prefix(command_buf.buf, "data ", &data))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	यदि (skip_prefix(data, "<<", &data)) {
		वर्ण *term = xstrdup(data);
		size_t term_len = command_buf.len - (data - command_buf.buf);

		प्रति (;;) {
			यदि (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), term);
			यदि (term_len == command_buf.len
				&& !strcmp(term, command_buf.buf))
				भङ्ग;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		मुक्त(term);
	}
	अन्यथा {
		uintmax_t len = strtoumax(data, रिक्त, 10);
		size_t n = 0, length = (size_t)len;

		यदि (limit && limit < len) {
			*len_res = len;
			प्रत्यागम 0;
		}
		यदि (length < len)
			die(_("data is too large to use in this context"));

		यावत् (n < length) {
			size_t s = strbuf_fread(sb, length - n, stdin);
			यदि (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(unsigned long)(length - n));
			n += s;
		}
	}

	skip_optional_lf();
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क validate_raw_date(ध्रुव वर्ण *src, संरचना strbuf *result, पूर्णाङ्क strict)
{
	ध्रुव वर्ण *orig_src = src;
	वर्ण *endp;
	unsigned long अंक;

	errno = 0;

	अंक = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	यदि (errno || endp == src || *endp != ' ')
		प्रत्यागम -1;

	src = endp + 1;
	यदि (*src != '-' && *src != '+')
		प्रत्यागम -1;

	अंक = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	यदि (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < अंक))             /* parsed a broken timezone */
	   )
		प्रत्यागम -1;

	strbuf_addstr(result, orig_src);
	प्रत्यागम 0;
}

स्थिर वर्ण *parse_ident(ध्रुव वर्ण *buf)
{
	ध्रुव वर्ण *ltgt;
	size_t name_len;
	संरचना strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	यदि (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	यदि (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	यदि (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	यदि (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	यदि (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_add(&ident, buf, name_len);

	विकल्प (whenspec) {
	पक्ष WHENSPEC_RAW:
		यदि (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		भङ्ग;
	पक्ष WHENSPEC_RAW_PERMISSIVE:
		यदि (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		भङ्ग;
	पक्ष WHENSPEC_RFC2822:
		यदि (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		भङ्ग;
	पक्ष WHENSPEC_NOW:
		यदि (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		भङ्ग;
	}

	प्रत्यागम strbuf_detach(&ident, रिक्त);
}

स्थिर शून्य parse_and_store_blob(
	संरचना last_object *last,
	संरचना object_id *oidout,
	uintmax_t mark)
{
	स्थिर संरचना strbuf buf = STRBUF_INIT;
	uintmax_t len;

	यदि (parse_data(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	अन्यथा {
		यदि (last) {
			strbuf_release(&last->data);
			last->offset = 0;
			last->depth = 0;
		}
		stream_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

स्थिर शून्य parse_new_blob(संरचना fast_import_state *state)
{
	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&last_blob, रिक्त, next_mark);
}

स्थिर शून्य unload_one_branch(शून्य)
{
	यावत् (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = ULONG_MAX;
		संरचना branch *e, *l = रिक्त, *p = रिक्त;

		प्रति (e = active_branches; e; e = e->active_next_branch) {
			यदि (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		यदि (p) {
			e = p->active_next_branch;
			p->active_next_branch = e->active_next_branch;
		} अन्यथा {
			e = active_branches;
			active_branches = e->active_next_branch;
		}
		e->active = 0;
		e->active_next_branch = रिक्त;
		यदि (e->branch_वृक्ष.वृक्ष) {
			release_वृक्ष_content_recursive(e->branch_वृक्ष.वृक्ष);
			e->branch_वृक्ष.वृक्ष = रिक्त;
		}
		cur_active_branches--;
	}
}

स्थिर शून्य load_branch(संरचना branch *b)
{
	load_वृक्ष(&b->branch_वृक्ष);
	यदि (!b->active) {
		b->active = 1;
		b->active_next_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_गिनती++;
	}
}

स्थिर unsigned वर्ण convert_अंक_notes_to_fanout(uintmax_t अंक_notes)
{
	unsigned वर्ण fanout = 0;
	यावत् ((अंक_notes >>= 8))
		fanout++;
	प्रत्यागम fanout;
}

स्थिर शून्य construct_path_with_fanout(ध्रुव वर्ण *hex_sha1,
		unsigned वर्ण fanout, वर्ण *path)
{
	unsigned पूर्णाङ्क i = 0, j = 0;
	यदि (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	यावत् (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	memcpy(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

स्थिर uintmax_t do_change_note_fanout(
		संरचना वृक्ष_entry *orig_मूल, संरचना वृक्ष_entry *मूल,
		वर्ण *hex_oid, unsigned पूर्णाङ्क hex_oid_len,
		वर्ण *fullpath, unsigned पूर्णाङ्क fullpath_len,
		unsigned वर्ण fanout)
{
	संरचना वृक्ष_content *t;
	संरचना वृक्ष_entry *e, leaf;
	unsigned पूर्णाङ्क i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t अंक_notes = 0;
	संरचना object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	वर्ण realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ध्रुव unsigned hexsz = the_hash_algo->hexsz;

	यदि (!मूल->वृक्ष)
		load_वृक्ष(मूल);
	t = मूल->वृक्ष;

	प्रति (i = 0; t && i < t->entry_गिनती; i++) {
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
		यदि (!e->versions[1].mode ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			अनुवर्त;

		/* This _may_ be a note entry, or a subdir containing notes */
		memcpy(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		यदि (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		memcpy(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		यदि (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			यदि (fanout == 0xff) {
				/* Counting mode, no rename */
				अंक_notes++;
				अनुवर्त;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			यदि (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				अंक_notes++;
				अनुवर्त;
			}

			/* Rename fullpath to realpath */
			यदि (!वृक्ष_content_remove(orig_मूल, fullpath, &leaf, 0))
				die(_("failed to remove path %s"), fullpath);
			वृक्ष_content_set(orig_मूल, realpath,
				&leaf.versions[1].oid,
				leaf.versions[1].mode,
				leaf.वृक्ष);
		} अन्यथा यदि (S_ISDIR(e->versions[1].mode)) {
			/* This is a subdir that may contain note entries */
			अंक_notes += do_change_note_fanout(orig_मूल, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = मूल->वृक्ष;
	}
	प्रत्यागम अंक_notes;
}

स्थिर uintmax_t change_note_fanout(संरचना वृक्ष_entry *मूल,
		unsigned वर्ण fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	वर्ण hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	प्रत्यागम do_change_note_fanout(मूल, मूल, hex_oid, 0, path, 0, fanout);
}

स्थिर पूर्णाङ्क parse_mapped_oid_hex(ध्रुव वर्ण *hex, संरचना object_id *oid, ध्रुव वर्ण **end)
{
	पूर्णाङ्क algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, परिमाण(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	यदि (algo == GIT_HASH_UNKNOWN)
		प्रत्यागम -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	यदि (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		यदि (hash_algos[algo].format_id == the_hash_algo->format_id)
			प्रत्यागम 0;
		प्रत्यागम -1;
	}
	oidcpy(oid, kh_मूल्य(sub_oid_map, it));
	प्रत्यागम 0;
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
स्थिर uintmax_t parse_mark_ref(ध्रुव वर्ण *p, वर्ण **endptr)
{
	uintmax_t mark;

	प्रतिज्ञा(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	यदि (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	प्रत्यागम mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
स्थिर uintmax_t parse_mark_ref_eol(ध्रुव वर्ण *p)
{
	वर्ण *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	यदि (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	प्रत्यागम mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
स्थिर uintmax_t parse_mark_ref_space(ध्रुव वर्ण **p)
{
	uintmax_t mark;
	वर्ण *end;

	mark = parse_mark_ref(*p, &end);
	यदि (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	प्रत्यागम mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
स्थिर शून्य parse_path(संरचना strbuf *sb, ध्रुव वर्ण *p, ध्रुव वर्ण **endp,
		पूर्णाङ्क is_last_field, ध्रुव वर्ण *field)
{
	यदि (*p == '"') {
		यदि (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), field, command_buf.buf);
		यदि (strlen(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), field, command_buf.buf);
	} अन्यथा {
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
स्थिर शून्य parse_path_eol(संरचना strbuf *sb, ध्रुव वर्ण *p, ध्रुव वर्ण *field)
{
	ध्रुव वर्ण *end;

	parse_path(sb, p, &end, 1, field);
	यदि (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
स्थिर शून्य parse_path_space(संरचना strbuf *sb, ध्रुव वर्ण *p,
		ध्रुव वर्ण **endp, ध्रुव वर्ण *field)
{
	parse_path(sb, p, endp, 0, field);
	यदि (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

स्थिर शून्य file_change_m(संरचना fast_import_state *state,
			  ध्रुव वर्ण *p,
			  संरचना branch *b)
{
	स्थिर संरचना strbuf path = STRBUF_INIT;
	संरचना object_entry *oe;
	संरचना object_id oid;
	uint16_t mode, inline_data = 0;

	p = parse_mode(p, &mode);
	यदि (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	विकल्प (mode) {
	पक्ष 0644:
	पक्ष 0755:
		mode |= S_IFREG;
	पक्ष S_IFREG | 0644:
	पक्ष S_IFREG | 0755:
	पक्ष S_IFLNK:
	पक्ष S_IFDIR:
	पक्ष S_IFGITLINK:
		/* ok */
		भङ्ग;
	स्वभाव:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	यदि (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा यदि (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = रिक्त; /* not used with inline_data, but makes gcc happy */
	} अन्यथा {
		यदि (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		यदि (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	यदि (S_ISDIR(mode) &&
	    is_empty_वृक्ष_oid(&oid, the_repository->hash_algo) &&
	    *path.buf) {
		वृक्ष_content_remove(&b->branch_वृक्ष, path.buf, रिक्त, 0);
		प्रत्यागम;
	}

	यदि (S_ISGITLINK(mode)) {
		यदि (inline_data)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		अन्यथा यदि (oe) {
			यदि (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} अन्यथा यदि (inline_data) {
		यदि (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		यावत् (read_next_command(state) != EOF) {
			ध्रुव वर्ण *v;
			यदि (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			अन्यथा {
				parse_and_store_blob(&last_blob, &oid, 0);
				भङ्ग;
			}
		}
	} अन्यथा {
		गणना object_type expected = S_ISDIR(mode) ?
						OBJ_वृक्ष: OBJ_BLOB;
		गणना object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, रिक्त);
		यदि (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(mode) ?  _("tree") : _("blob"),
			    command_buf.buf);
		यदि (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	यदि (!*path.buf) {
		वृक्ष_content_replace(&b->branch_वृक्ष, &oid, mode, रिक्त);
		प्रत्यागम;
	}

	यदि (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	वृक्ष_content_set(&b->branch_वृक्ष, path.buf, &oid, mode, रिक्त);
}

स्थिर शून्य file_change_d(ध्रुव वर्ण *p, संरचना branch *b)
{
	स्थिर संरचना strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	वृक्ष_content_remove(&b->branch_वृक्ष, path.buf, रिक्त, 1);
}

स्थिर शून्य file_change_cr(ध्रुव वर्ण *p, संरचना branch *b, पूर्णाङ्क rename)
{
	स्थिर संरचना strbuf source = STRBUF_INIT;
	स्थिर संरचना strbuf dest = STRBUF_INIT;
	संरचना वृक्ष_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	memset(&leaf, 0, परिमाण(leaf));
	यदि (rename)
		वृक्ष_content_remove(&b->branch_वृक्ष, source.buf, &leaf, 1);
	अन्यथा
		वृक्ष_content_get(&b->branch_वृक्ष, source.buf, &leaf, 1);
	यदि (!leaf.versions[1].mode)
		die(_("path %s not in branch"), source.buf);
	यदि (!*dest.buf) {	/* C "path/to/subdir" "" */
		वृक्ष_content_replace(&b->branch_वृक्ष,
			&leaf.versions[1].oid,
			leaf.versions[1].mode,
			leaf.वृक्ष);
		प्रत्यागम;
	}
	यदि (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	वृक्ष_content_set(&b->branch_वृक्ष, dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.वृक्ष);
}

स्थिर शून्य note_change_n(संरचना fast_import_state *state,
			  ध्रुव वर्ण *p,
			  संरचना branch *b,
			  unsigned वर्ण *old_fanout)
{
	संरचना object_entry *oe;
	संरचना branch *s;
	संरचना object_id oid, commit_oid;
	वर्ण path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_data = 0;
	unsigned वर्ण new_fanout;

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
	यदि (b->अंक_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->अंक_notes = change_note_fanout(&b->branch_वृक्ष, 0xff);
		*old_fanout = convert_अंक_notes_to_fanout(b->अंक_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	यदि (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा यदि (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = रिक्त; /* not used with inline_data, but makes gcc happy */
	} अन्यथा {
		यदि (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		यदि (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	यदि (s) {
		यदि (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} अन्यथा यदि (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		संरचना object_entry *commit_oe = find_mark(marks, commit_mark);
		यदि (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} अन्यथा यदि (!repo_get_oid(the_repository, p, &commit_oid)) {
		size_t size;
		वर्ण *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &size,
						   &commit_oid);
		यदि (!buf || size < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		मुक्त(buf);
	} अन्यथा
		die(_("invalid ref name or SHA1 expression: %s"), p);

	यदि (inline_data) {
		read_next_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} अन्यथा यदि (oe) {
		यदि (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} अन्यथा यदि (!is_null_oid(&oid)) {
		गणना object_type type = odb_read_object_info(the_repository->objects, &oid,
							रिक्त);
		यदि (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		यदि (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	यदि (वृक्ष_content_remove(&b->branch_वृक्ष, path, रिक्त, 0))
		b->अंक_notes--;

	यदि (is_null_oid(&oid))
		प्रत्यागम; /* nothing to insert */

	b->अंक_notes++;
	new_fanout = convert_अंक_notes_to_fanout(b->अंक_notes);
	construct_path_with_fanout(oid_to_hex(&commit_oid), new_fanout, path);
	वृक्ष_content_set(&b->branch_वृक्ष, path, &oid, S_IFREG | 0644, रिक्त);
}

स्थिर शून्य file_change_deleteall(संरचना branch *b)
{
	release_वृक्ष_content_recursive(b->branch_वृक्ष.वृक्ष);
	oidclr(&b->branch_वृक्ष.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_वृक्ष.versions[1].oid, the_repository->hash_algo);
	load_वृक्ष(&b->branch_वृक्ष);
	b->अंक_notes = 0;
}

स्थिर शून्य parse_from_commit(संरचना branch *b, वर्ण *buf, unsigned long size)
{
	यदि (!buf || size < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	यदि (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_वृक्ष.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_वृक्ष.versions[0].oid,
	       &b->branch_वृक्ष.versions[1].oid);
}

स्थिर शून्य parse_from_existing(संरचना branch *b)
{
	यदि (is_null_oid(&b->oid)) {
		oidclr(&b->branch_वृक्ष.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_वृक्ष.versions[1].oid, the_repository->hash_algo);
	} अन्यथा {
		unsigned long size;
		size_t size_st = 0;
		वर्ण *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &size_st, &b->oid);
		size = cast_size_t_to_ulong(size_st);
		parse_from_commit(b, buf, size);
		मुक्त(buf);
	}
}

स्थिर पूर्णाङ्क parse_objectish(संरचना fast_import_state *state,
			   संरचना branch *b,
			   ध्रुव वर्ण *objectish)
{
	संरचना branch *s;
	संरचना object_id oid;

	oidcpy(&oid, &b->branch_वृक्ष.versions[1].oid);

	s = lookup_branch(objectish);
	यदि (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	अन्यथा यदि (s) {
		संरचना object_id *t = &s->branch_वृक्ष.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_वृक्ष.versions[0].oid, t);
		oidcpy(&b->branch_वृक्ष.versions[1].oid, t);
	} अन्यथा यदि (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		संरचना object_entry *oe = find_mark(marks, idnum);
		यदि (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		यदि (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			यदि (oe->pack_id != MAX_PACK_ID) {
				unsigned long size;
				वर्ण *buf = gfi_unpack_entry(oe, &size);
				parse_from_commit(b, buf, size);
				मुक्त(buf);
			} अन्यथा
				parse_from_existing(b);
		}
	} अन्यथा यदि (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		यदि (is_null_oid(&b->oid))
			b->delete = 1;
	}
	अन्यथा
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	यदि (b->branch_वृक्ष.वृक्ष && !oideq(&oid, &b->branch_वृक्ष.versions[1].oid)) {
		release_वृक्ष_content_recursive(b->branch_वृक्ष.वृक्ष);
		b->branch_वृक्ष.वृक्ष = रिक्त;
	}

	read_next_command(state);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क parse_from(संरचना fast_import_state *state, संरचना branch *b)
{
	ध्रुव वर्ण *from;

	यदि (!skip_prefix(command_buf.buf, "from ", &from))
		प्रत्यागम 0;

	प्रत्यागम parse_objectish(state, b, from);
}

स्थिर पूर्णाङ्क parse_objectish_with_prefix(संरचना fast_import_state *state,
				       संरचना branch *b,
				       ध्रुव वर्ण *prefix)
{
	ध्रुव वर्ण *base;

	यदि (!skip_prefix(command_buf.buf, prefix, &base))
		प्रत्यागम 0;

	प्रत्यागम parse_objectish(state, b, base);
}

स्थिर संरचना hash_list *parse_merge(संरचना fast_import_state *state,
				     unsigned पूर्णाङ्क *गिनती)
{
	संरचना hash_list *list = रिक्त, **tail = &list, *n;
	ध्रुव वर्ण *from;
	संरचना branch *s;

	*गिनती = 0;
	यावत् (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(परिमाण(*n));
		s = lookup_branch(from);
		यदि (s)
			oidcpy(&n->oid, &s->oid);
		अन्यथा यदि (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			संरचना object_entry *oe = find_mark(marks, idnum);
			यदि (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} अन्यथा यदि (!repo_get_oid(the_repository, from, &n->oid)) {
			size_t size;
			वर्ण *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &size, &n->oid);
			यदि (!buf || size < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			मुक्त(buf);
		} अन्यथा
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->next = रिक्त;
		*tail = n;
		tail = &n->next;

		(*गिनती)++;
		read_next_command(state);
	}
	प्रत्यागम list;
}

संरचना signature_data {
	वर्ण *hash_algo;      /* "sha1" or "sha256" */
	वर्ण *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	संरचना strbuf data;   /* The actual signature data */
};

स्थिर शून्य parse_one_signature(संरचना fast_import_state *state,
				संरचना signature_data *sig,
				ध्रुव वर्ण *v)
{
	वर्ण *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	वर्ण *space = strchr(args, ' ');

	यदि (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), args);
	*space = '\0';

	sig->hash_algo = args;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	यदि (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	यदि (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	यदि (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_next_command(state);
	parse_data(&sig->data, 0, रिक्त);
}

स्थिर शून्य discard_one_signature(संरचना fast_import_state *state)
{
	संरचना strbuf data = STRBUF_INIT;

	read_next_command(state);
	parse_data(&data, 0, रिक्त);
	strbuf_release(&data);
}

स्थिर शून्य add_gpgsig_to_commit(संरचना strbuf *commit_data,
				 ध्रुव वर्ण *header,
				 संरचना signature_data *sig)
{
	संरचना string_list siglines = STRING_LIST_INIT_NODUP;

	यदि (!sig || !sig->hash_algo)
		प्रत्यागम;

	strbuf_addstr(commit_data, header);
	string_list_split_in_place(&siglines, sig->data.buf, "\n", -1);
	strbuf_add_separated_string_list(commit_data, "\n ", &siglines);
	strbuf_addch(commit_data, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->data);
	मुक्त(sig->hash_algo);
}

स्थिर शून्य store_signature(संरचना signature_data *stored_sig,
			    संरचना signature_data *new_sig,
			    ध्रुव वर्ण *hash_type)
{
	यदि (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->data);
		मुक्त(new_sig->hash_algo);
	} अन्यथा {
		*stored_sig = *new_sig;
	}
}

स्थिर शून्य import_one_signature(संरचना fast_import_state *state,
				 संरचना signature_data *sig_sha1,
				 संरचना signature_data *sig_sha256,
				 ध्रुव वर्ण *v)
{
	संरचना signature_data sig = { रिक्त, रिक्त, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	यदि (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	अन्यथा यदि (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	अन्यथा
		die(_("parse_one_signature() returned unknown hash algo"));
}

स्थिर शून्य finalize_commit_buffer(संरचना strbuf *new_data,
				   संरचना signature_data *sig_sha1,
				   संरचना signature_data *sig_sha256,
				   संरचना strbuf *संदेश)
{
	add_gpgsig_to_commit(new_data, "gpgsig ", sig_sha1);
	add_gpgsig_to_commit(new_data, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_data, '\n');
	strbuf_addbuf(new_data, संदेश);
}

स्थिर शून्य warn_invalid_signature(संरचना signature_check *check,
				   ध्रुव वर्ण *संदेश, गणना sign_mode mode)
{
	ध्रुव वर्ण *signer = check->signer ? check->signer : _("unknown");
	ध्रुव वर्ण *subject;
	पूर्णाङ्क subject_len = find_commit_subject(संदेश, &subject);

	विकल्प (mode) {
	पक्ष SIGN_STRIP_IF_INVALID:
		यदि (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		अन्यथा यदि (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		अन्यथा
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		भङ्ग;
	पक्ष SIGN_SIGN_IF_INVALID:
		यदि (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		अन्यथा यदि (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		अन्यथा
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		भङ्ग;
	स्वभाव:
		BUG("unsupported signing mode");
	}
}

स्थिर शून्य handle_signature_if_invalid(संरचना strbuf *new_data,
					संरचना signature_data *sig_sha1,
					संरचना signature_data *sig_sha256,
					संरचना strbuf *संदेश,
					गणना sign_mode mode)
{
	संरचना strbuf tmp_buf = STRBUF_INIT;
	संरचना signature_check signature_check = { 0 };
	पूर्णाङ्क ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_data);
	finalize_commit_buffer(&tmp_buf, sig_sha1, sig_sha256, संदेश);
	ret = verify_commit_buffer(tmp_buf.buf, tmp_buf.len, &signature_check);

	यदि (ret) {
		यदि (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, संदेश->buf, mode);

		यदि (mode == SIGN_SIGN_IF_INVALID) {
			संरचना strbuf signature = STRBUF_INIT;
			संरचना strbuf payload = STRBUF_INIT;

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
			यदि (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			यदि (sign_buffer(&payload, &signature, signed_commit_keyid,
					SIGN_BUFFER_USE_DEFAULT_कुंजी))
				die(_("failed to sign commit object"));
			add_header_signature(new_data, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_buffer(new_data, रिक्त, रिक्त, संदेश);
	} अन्यथा {
		strbuf_swap(new_data, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

स्थिर शून्य parse_new_commit(संरचना fast_import_state *state, ध्रुव वर्ण *arg)
{
	स्थिर संरचना strbuf संदेश = STRBUF_INIT;
	संरचना signature_data sig_sha1 = { रिक्त, रिक्त, STRBUF_INIT };
	संरचना signature_data sig_sha256 = { रिक्त, रिक्त, STRBUF_INIT };
	संरचना branch *b;
	वर्ण *author = रिक्त;
	वर्ण *committer = रिक्त;
	वर्ण *encoding = रिक्त;
	संरचना hash_list *merge_list = रिक्त;
	unsigned पूर्णाङ्क merge_गिनती;
	unsigned वर्ण prev_fanout, new_fanout;
	ध्रुव वर्ण *v;

	b = lookup_branch(arg);
	यदि (!b)
		b = new_branch(arg);

	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	यदि (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_next_command(state);
	}
	यदि (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_next_command(state);
	}
	यदि (!committer)
		die(_("expected committer but didn't get one"));

	यावत् (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		विकल्प (signed_commit_mode) {

		/* First, modes that don't need the signature to be parsed */
		पक्ष SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		पक्ष SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		पक्ष SIGN_STRIP:
			discard_one_signature(state);
			भङ्ग;

		/* Second, modes that parse the signature */
		पक्ष SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		पक्ष SIGN_VERBATIM:
		पक्ष SIGN_STRIP_IF_INVALID:
		पक्ष SIGN_SIGN_IF_INVALID:
		पक्ष SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			भङ्ग;

		/* Third, BUG */
		स्वभाव:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_next_command(state);
	}

	यदि (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_next_command(state);
	}
	parse_data(&संदेश, 0, रिक्त);
	read_next_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_गिनती);

	/* ensure the branch is active/loaded */
	यदि (!b->branch_वृक्ष.वृक्ष || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	prev_fanout = convert_अंक_notes_to_fanout(b->अंक_notes);

	/* file_change* */
	यावत् (command_buf.len > 0) {
		यदि (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		अन्यथा यदि (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		अन्यथा यदि (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		अन्यथा यदि (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		अन्यथा यदि (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		अन्यथा यदि (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		अन्यथा यदि (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		अन्यथा यदि (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		अन्यथा {
			unread_command_buf = 1;
			भङ्ग;
		}
		यदि (read_next_command(state) == EOF)
			भङ्ग;
	}

	new_fanout = convert_अंक_notes_to_fanout(b->अंक_notes);
	यदि (new_fanout != prev_fanout)
		b->अंक_notes = change_note_fanout(&b->branch_वृक्ष, new_fanout);

	/* build the tree and the commit */
	store_वृक्ष(&b->branch_वृक्ष);
	oidcpy(&b->branch_वृक्ष.versions[0].oid,
	       &b->branch_वृक्ष.versions[1].oid);

	strbuf_reset(&new_data);
	strbuf_addf(&new_data, "tree %s\n",
		oid_to_hex(&b->branch_वृक्ष.versions[1].oid));
	यदि (!is_null_oid(&b->oid))
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&b->oid));
	यावत् (merge_list) {
		संरचना hash_list *next = merge_list->next;
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&merge_list->oid));
		मुक्त(merge_list);
		merge_list = next;
	}
	strbuf_addf(&new_data,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	यदि (encoding)
		strbuf_addf(&new_data,
			"encoding %s\n",
			encoding);

	यदि ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
	     signed_commit_mode == SIGN_SIGN_IF_INVALID ||
	     signed_commit_mode == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_data, &sig_sha1, &sig_sha256,
					    &संदेश, signed_commit_mode);
	अन्यथा
		finalize_commit_buffer(&new_data, &sig_sha1, &sig_sha256, &संदेश);

	मुक्त(author);
	मुक्त(committer);
	मुक्त(encoding);

	यदि (!store_object(OBJ_COMMIT, &new_data, रिक्त, &b->oid, next_mark))
		b->pack_id = pack_id;
	b->last_commit = object_गिनती_by_type[OBJ_COMMIT];
}

स्थिर शून्य handle_tag_signature_if_invalid(संरचना strbuf *buf,
					    संरचना strbuf *संदेश,
					    size_t sig_offset)
{
	संरचना strbuf signature = STRBUF_INIT;
	संरचना strbuf payload = STRBUF_INIT;
	संरचना signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_add(&payload, संदेश->buf, sig_offset);
	strbuf_add(&signature, संदेश->buf + sig_offset, संदेश->len - sig_offset);

	sigc.payload_type = SIGNATURE_PAYLOAD_TAG;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	यदि (!check_signature(&sigc, signature.buf, signature.len))
		गच्छ out;

	यदि (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(संदेश, sig_offset);

	यदि (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = रिक्त;
		strbuf_reset(&signature);

		यदि (sign_buffer(&payload, &signature, signed_tag_keyid,
				SIGN_BUFFER_USE_DEFAULT_कुंजी))
			die(_("failed to sign tag object"));

		strbuf_addbuf(संदेश, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

स्थिर शून्य handle_tag_signature(संरचना strbuf *buf, संरचना strbuf *संदेश, ध्रुव वर्ण *name)
{
	size_t sig_offset = parse_signed_buffer(संदेश->buf, संदेश->len);

	/* If there is no signature, there is nothing to do. */
	यदि (sig_offset >= संदेश->len)
		प्रत्यागम;

	विकल्प (signed_tag_mode) {

	/* First, modes that don't change anything */
	पक्ष SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	पक्ष SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		भङ्ग;

	/* Second, modes that remove the signature */
	पक्ष SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	पक्ष SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(संदेश, sig_offset);
		भङ्ग;
	पक्ष SIGN_ABORT_IF_INVALID:
	पक्ष SIGN_SIGN_IF_INVALID:
	पक्ष SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, संदेश, sig_offset);
		भङ्ग;

	/* Third, aborting modes */
	पक्ष SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	स्वभाव:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

स्थिर शून्य parse_new_tag(संरचना fast_import_state *state, ध्रुव वर्ण *arg)
{
	स्थिर संरचना strbuf संदेश = STRBUF_INIT;
	ध्रुव वर्ण *from;
	वर्ण *tagger;
	संरचना branch *s;
	संरचना tag *t;
	uintmax_t from_mark = 0;
	संरचना object_id oid;
	गणना object_type type;
	ध्रुव वर्ण *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, परिमाण(संरचना tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	यदि (last_tag)
		last_tag->next_tag = t;
	अन्यथा
		first_tag = t;
	last_tag = t;
	read_next_command(state);
	parse_mark(state);

	/* from ... */
	यदि (!skip_prefix(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	यदि (s) {
		यदि (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} अन्यथा यदि (*from == ':') {
		संरचना object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा यदि (!repo_get_oid(the_repository, from, &oid)) {
		संरचना object_entry *oe = find_object(&oid);
		यदि (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, रिक्त);
			यदि (type < 0)
				die(_("not a valid object: %s"), from);
		} अन्यथा
			type = oe->type;
	} अन्यथा
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_next_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	यदि (skip_prefix(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_next_command(state);
	} अन्यथा
		tagger = रिक्त;

	/* tag payload/message */
	parse_data(&संदेश, 0, रिक्त);

	/* build the tag object */
	strbuf_reset(&new_data);

	strbuf_addf(&new_data,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_to_hex(&oid), type_name(type), t->name);
	यदि (tagger)
		strbuf_addf(&new_data,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_data, &संदेश, t->name);

	strbuf_addch(&new_data, '\n');
	strbuf_addbuf(&new_data, &संदेश);
	मुक्त(tagger);

	यदि (store_object(OBJ_TAG, &new_data, रिक्त, &t->oid, next_mark))
		t->pack_id = MAX_PACK_ID;
	अन्यथा
		t->pack_id = pack_id;
}

स्थिर शून्य parse_reset_branch(संरचना fast_import_state *state, ध्रुव वर्ण *arg)
{
	संरचना branch *b;
	ध्रुव वर्ण *tag_name;

	b = lookup_branch(arg);
	यदि (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_वृक्ष.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_वृक्ष.versions[1].oid, the_repository->hash_algo);
		यदि (b->branch_वृक्ष.वृक्ष) {
			release_वृक्ष_content_recursive(b->branch_वृक्ष.वृक्ष);
			b->branch_वृक्ष.वृक्ष = रिक्त;
		}
	}
	अन्यथा
		b = new_branch(arg);
	read_next_command(state);
	parse_from(state, b);
	यदि (b->delete && skip_prefix(b->name, "refs/tags/", &tag_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		संरचना tag *t, *prev = रिक्त;
		प्रति (t = first_tag; t; t = t->next_tag) {
			यदि (!strcmp(t->name, tag_name))
				भङ्ग;
			prev = t;
		}
		यदि (t) {
			यदि (prev)
				prev->next_tag = t->next_tag;
			अन्यथा
				first_tag = t->next_tag;
			यदि (!t->next_tag)
				last_tag = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	यदि (command_buf.len > 0)
		unread_command_buf = 1;
}

स्थिर शून्य cat_blob_write(ध्रुव वर्ण *buf, unsigned long size)
{
	यदि (write_in_full(cat_blob_fd, buf, size) < 0)
		die_errno(_("write to frontend failed"));
}

स्थिर शून्य cat_blob(संरचना object_entry *oe, संरचना object_id *oid)
{
	संरचना strbuf पंक्ति = STRBUF_INIT;
	संरचना iovec iov[3];
	unsigned long size;
	गणना object_type type = 0;
	वर्ण *buf;

	यदि (!oe || oe->pack_id == MAX_PACK_ID) {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
	} अन्यथा {
		type = oe->type;
		buf = gfi_unpack_entry(oe, &size);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	यदि (type <= 0) {
		strbuf_reset(&पंक्ति);
		strbuf_addf(&पंक्ति, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(पंक्ति.buf, पंक्ति.len);
		strbuf_release(&पंक्ति);
		मुक्त(buf);
		प्रत्यागम;
	}
	यदि (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	यदि (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_to_hex(oid), type_name(type));
	strbuf_reset(&पंक्ति);
	strbuf_addf(&पंक्ति, "%s %s %"PRIuMAX"\n", oid_to_hex(oid),
		    type_name(type), (uintmax_t)size);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_base = पंक्ति.buf;
	iov[0].iov_len = पंक्ति.len;
	iov[1].iov_base = buf;
	iov[1].iov_len = size;
	iov[2].iov_base = (शून्य *) "\n";
	iov[2].iov_len = 1;

	यदि (writev_in_full(cat_blob_fd, iov, ARRAY_SIZE(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&पंक्ति);
	यदि (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.data, buf, size, size + 1);
		last_blob.depth = oe->depth;
	} अन्यथा
		मुक्त(buf);
}

स्थिर शून्य parse_get_mark(संरचना fast_import_state *state UNUSED,
			   ध्रुव वर्ण *p)
{
	संरचना object_entry *oe;
	वर्ण output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	यदि (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	यदि (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, परिमाण(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

स्थिर शून्य parse_cat_blob(संरचना fast_import_state *state UNUSED,
			   ध्रुव वर्ण *p)
{
	संरचना object_entry *oe;
	संरचना object_id oid;

	/* cat-blob SP <object> LF */
	यदि (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		यदि (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा {
		यदि (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		यदि (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

स्थिर संरचना object_entry *dereference(संरचना object_entry *oe,
					संरचना object_id *oid)
{
	unsigned long size;
	वर्ण *buf = रिक्त;
	ध्रुव unsigned hexsz = the_hash_algo->hexsz;

	यदि (!oe) {
		गणना object_type type = odb_read_object_info(the_repository->objects,
							     oid, रिक्त);
		यदि (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = जोड़ो_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	विकल्प (oe->type) {
	पक्ष OBJ_वृक्ष:	/* easy case. */
		प्रत्यागम oe;
	पक्ष OBJ_COMMIT:
	पक्ष OBJ_TAG:
		भङ्ग;
	स्वभाव:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	यदि (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &size);
	} अन्यथा {
		गणना object_type unused;
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &size_st);
		size = cast_size_t_to_ulong(size_st);
	}
	यदि (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	विकल्प (oe->type) {
	पक्ष OBJ_TAG:
		यदि (size < hexsz + strlen("object ") ||
		    get_oid_hex(buf + strlen("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		भङ्ग;
	पक्ष OBJ_COMMIT:
		यदि (size < hexsz + strlen("tree ") ||
		    get_oid_hex(buf + strlen("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	मुक्त(buf);
	प्रत्यागम find_object(oid);
}

स्थिर शून्य जोड़ो_mapped_mark(uintmax_t mark, शून्य *object, शून्य *cbp)
{
	संरचना object_id *fromoid = object;
	संरचना object_id *tooid = find_mark(cbp, mark);
	पूर्णाङ्क ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	यदि (ret == 0)
		प्रत्यागम;
	kh_मूल्य(sub_oid_map, it) = tooid;
}

स्थिर शून्य build_mark_map_one(संरचना mark_set *from, संरचना mark_set *to)
{
	for_each_mark(from, 0, जोड़ो_mapped_mark, to);
}

स्थिर शून्य build_mark_map(संरचना string_list *from, संरचना string_list *to)
{
	संरचना string_list_item *fromp, *शीर्ष;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		शीर्ष = string_list_lookup(to, fromp->string);
		यदि (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} अन्यथा यदि (!शीर्ष || !शीर्ष->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, शीर्ष->util);
	}
}

स्थिर संरचना object_entry *parse_treeish_dataref(ध्रुव वर्ण **p)
{
	संरचना object_id oid;
	संरचना object_entry *e;

	यदि (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		यदि (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} अन्यथा {	/* <sha1> */
		यदि (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		यदि (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	यावत् (!e || e->type != OBJ_वृक्ष)
		e = dereference(e, &oid);
	प्रत्यागम e;
}

स्थिर शून्य print_ls(पूर्णाङ्क mode, ध्रुव unsigned वर्ण *hash, ध्रुव वर्ण *path)
{
	स्थिर संरचना strbuf पंक्ति = STRBUF_INIT;

	/* See show_tree(). */
	ध्रुव वर्ण *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? वृक्ष_type :
		blob_type;

	यदि (!mode) {
		/* missing SP path LF */
		strbuf_reset(&पंक्ति);
		strbuf_addstr(&पंक्ति, "missing ");
		quote_c_style(path, &पंक्ति, रिक्त, 0);
		strbuf_addch(&पंक्ति, '\n');
	} अन्यथा {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&पंक्ति);
		strbuf_addf(&पंक्ति, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &पंक्ति, रिक्त, 0);
		strbuf_addch(&पंक्ति, '\n');
	}
	cat_blob_write(पंक्ति.buf, पंक्ति.len);
}

स्थिर शून्य parse_ls(संरचना fast_import_state *state UNUSED,
		     ध्रुव वर्ण *p,
		     संरचना branch *b)
{
	स्थिर संरचना strbuf path = STRBUF_INIT;
	संरचना वृक्ष_entry *मूल = रिक्त;
	संरचना वृक्ष_entry leaf = {रिक्त};

	/* ls SP (<tree-ish> SP)? <path> */
	यदि (*p == '"') {
		यदि (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		मूल = &b->branch_वृक्ष;
	} अन्यथा {
		संरचना object_entry *e = parse_treeish_dataref(&p);
		मूल = new_वृक्ष_entry();
		oidcpy(&मूल->versions[1].oid, &e->idx.oid);
		यदि (!is_null_oid(&मूल->versions[1].oid))
			मूल->versions[1].mode = S_IFDIR;
		load_वृक्ष(मूल);
	}
	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	वृक्ष_content_get(मूल, path.buf, &leaf, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	यदि (S_ISDIR(leaf.versions[1].mode))
		store_वृक्ष(&leaf);

	print_ls(leaf.versions[1].mode, leaf.versions[1].oid.hash, path.buf);
	यदि (leaf.वृक्ष)
		release_वृक्ष_content_recursive(leaf.वृक्ष);
	यदि (!b || मूल != &b->branch_वृक्ष)
		release_वृक्ष_entry(मूल);
}

स्थिर शून्य checkpoint(शून्य)
{
	checkpoint_requested = 0;
	यदि (object_गिनती) {
		cycle_packfile();
	}
	dump_branches();
	dump_tags();
	dump_marks();
}

स्थिर शून्य parse_checkpoint(संरचना fast_import_state *state UNUSED)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

स्थिर शून्य parse_progress(संरचना fast_import_state *state UNUSED)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

स्थिर शून्य parse_alias(संरचना fast_import_state *state)
{
	संरचना object_entry *e;
	संरचना branch b;

	skip_optional_lf();
	read_next_command(state);

	/* mark ... */
	parse_mark(state);
	यदि (!next_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, परिमाण(b));
	यदि (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	प्रतिज्ञा(e);
	जोड़ो_mark(&marks, next_mark, e);
}

स्थिर वर्ण* make_fast_import_path(संरचना fast_import_state *state,
				   ध्रुव वर्ण *path)
{
	यदि (!relative_marks_paths || is_absolute_path(path))
		प्रत्यागम prefix_filename(state->prefix, path);
	प्रत्यागम repo_git_path(the_repository, "info/fast-import/%s", path);
}

स्थिर शून्य option_import_marks(संरचना fast_import_state *state,
				ध्रुव वर्ण *marks,
				पूर्णाङ्क from_stream,
				पूर्णाङ्क ignore_missing)
{
	यदि (import_marks_file) {
		यदि (from_stream)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		यदि(!import_marks_file_from_stream)
			read_marks();
	}

	मुक्त(import_marks_file);
	import_marks_file = make_fast_import_path(state, marks);
	import_marks_file_from_stream = from_stream;
	import_marks_file_ignore_missing = ignore_missing;
}

स्थिर शून्य option_date_format(ध्रुव वर्ण *fmt)
{
	यदि (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	अन्यथा यदि (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	अन्यथा यदि (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	अन्यथा यदि (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	अन्यथा
		die(_("unknown --date-format argument %s"), fmt);
}

स्थिर unsigned long ulong_arg(ध्रुव वर्ण *option, ध्रुव वर्ण *arg)
{
	वर्ण *endptr;
	unsigned long rv = strtoul(arg, &endptr, 0);
	यदि (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	प्रत्यागम rv;
}

स्थिर शून्य option_depth(ध्रुव वर्ण *depth)
{
	max_depth = ulong_arg("--depth", depth);
	यदि (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

स्थिर शून्य option_active_branches(ध्रुव वर्ण *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

स्थिर शून्य option_export_marks(संरचना fast_import_state *state,
				ध्रुव वर्ण *marks)
{
	मुक्त(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

स्थिर शून्य option_cat_blob_fd(संरचना fast_import_state *state UNUSED,
			       ध्रुव वर्ण *fd)
{
	unsigned long n = ulong_arg("--cat-blob-fd", fd);
	यदि (n > (unsigned long) INT_MAX)
		die(_("--cat-blob-fd cannot exceed %d"), INT_MAX);
	cat_blob_fd = (पूर्णाङ्क) n;
}

स्थिर शून्य option_export_pack_edges(संरचना fast_import_state *state,
				     ध्रुव वर्ण *edges)
{
	वर्ण *fn = prefix_filename(state->prefix, edges);
	यदि (pack_edges)
		fclose(pack_edges);
	pack_edges = xfopen(fn, "a");
	मुक्त(fn);
}

स्थिर शून्य option_rewrite_submodules(संरचना fast_import_state *state,
				      ध्रुव वर्ण *arg,
				      संरचना string_list *list)
{
	संरचना mark_set *ms;
	FILE *fp;
	वर्ण *s = xstrdup(arg);
	वर्ण *f = strchr(s, ':');
	यदि (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = prefix_filename(state->prefix, f);
	fp = fopen(f, "r");
	यदि (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, जोड़ो_oid_entry);
	fclose(fp);
	मुक्त(f);

	string_list_जोड़ो(list, s)->util = ms;

	मुक्त(s);
}

स्थिर शून्य option_max_pack_size(ध्रुव वर्ण *arg)
{
	unsigned long v;

	यदि (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	यदि (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} अन्यथा यदि (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

स्थिर शून्य option_big_file_threshold(ध्रुव वर्ण *arg)
{
	unsigned long v;

	यदि (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

स्थिर शून्य option_signed_commits(ध्रुव वर्ण *arg)
{
	यदि (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

स्थिर शून्य option_signed_tags(ध्रुव वर्ण *arg)
{
	यदि (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

स्थिर शून्य option_quiet(शून्य)
{
	show_stats = 0;
	quiet = 1;
}

स्थिर पूर्णाङ्क parse_one_option(संरचना fast_import_state *state, ध्रुव वर्ण *option)
{
	यदि (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_size(option);
	} अन्यथा यदि (skip_prefix(option, "big-file-threshold=", &option)) {
		option_big_file_threshold(option);
	} अन्यथा यदि (skip_prefix(option, "depth=", &option)) {
		option_depth(option);
	} अन्यथा यदि (skip_prefix(option, "active-branches=", &option)) {
		option_active_branches(option);
	} अन्यथा यदि (skip_prefix(option, "export-pack-edges=", &option)) {
		option_export_pack_edges(state, option);
	} अन्यथा यदि (skip_prefix(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} अन्यथा यदि (skip_prefix(option, "signed-tags=", &option)) {
		option_signed_tags(option);
	} अन्यथा यदि (!strcmp(option, "quiet")) {
		option_quiet();
	} अन्यथा यदि (!strcmp(option, "stats")) {
		show_stats = 1;
	} अन्यथा यदि (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} अन्यथा {
		प्रत्यागम 0;
	}

	प्रत्यागम 1;
}

स्थिर शून्य check_unsafe_feature(संरचना fast_import_state *state,
				 ध्रुव वर्ण *feature)
{
	यदि (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

स्थिर पूर्णाङ्क parse_one_feature(संरचना fast_import_state *state,
			     ध्रुव वर्ण *feature)
{
	ध्रुव वर्ण *arg;

	यदि (skip_prefix(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} अन्यथा यदि (skip_prefix(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} अन्यथा यदि (skip_prefix(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} अन्यथा यदि (skip_prefix(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} अन्यथा यदि (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} अन्यथा यदि (skip_prefix(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_to);
	} अन्यथा यदि (skip_prefix(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} अन्यथा यदि (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} अन्यथा यदि (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} अन्यथा यदि (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} अन्यथा यदि (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} अन्यथा यदि (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} अन्यथा यदि (!strcmp(feature, "force")) {
		force_update = 1;
	} अन्यथा यदि (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} अन्यथा {
		प्रत्यागम 0;
	}

	प्रत्यागम 1;
}

स्थिर शून्य parse_feature(संरचना fast_import_state *state, ध्रुव वर्ण *feature)
{
	यदि (state->seen_data_command)
		die(_("got feature command '%s' after data command"), feature);

	यदि (parse_one_feature(state, feature))
		प्रत्यागम;

	die(_("this version of fast-import does not support feature %s."), feature);
}

स्थिर शून्य parse_option(संरचना fast_import_state *state, ध्रुव वर्ण *option)
{
	यदि (state->seen_data_command)
		die(_("got option command '%s' after data command"), option);

	यदि (parse_one_option(state, option))
		प्रत्यागम;

	die(_("this version of fast-import does not support option: %s"), option);
}

स्थिर शून्य git_pack_config(शून्य)
{
	पूर्णाङ्क indexversion_मूल्य;
	पूर्णाङ्क limit;
	unsigned long packsizelimit_मूल्य;

	यदि (!repo_config_get_ulong(the_repository, "pack.depth", &max_depth)) {
		यदि (max_depth > MAX_DEPTH)
			max_depth = MAX_DEPTH;
	}
	यदि (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_मूल्य)) {
		pack_idx_opts.version = indexversion_मूल्य;
		यदि (pack_idx_opts.version > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.version);
	}
	यदि (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_मूल्य))
		max_packsize = packsizelimit_मूल्य;

	यदि (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &limit))
		unpack_limit = limit;
	अन्यथा यदि (!repo_config_get_int(the_repository, "transfer.unpacklimit", &limit))
		unpack_limit = limit;

	repo_config(the_repository, git_default_config, रिक्त);
}

स्थिर ध्रुव वर्ण *ध्रुव fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	रिक्त
};

स्थिर शून्य parse_argv(संरचना fast_import_state *state)
{
	पूर्णाङ्क argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	यदि (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_data_command = 1;
	यदि (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

स्थिर पूर्णाङ्क option_parse_date_format(ध्रुव संरचना option *opt UNUSED,
				    ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_export_pack_edges(ध्रुव संरचना option *opt,
					  ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->मूल्य, arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_max_pack_size(ध्रुव संरचना option *opt UNUSED,
				      ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_size(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_big_file_threshold(ध्रुव संरचना option *opt UNUSED,
					   ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_signed_commits(ध्रुव संरचना option *opt UNUSED,
				       ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_signed_tags(ध्रुव संरचना option *opt UNUSED,
				    ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_rewrite_submodules_from(ध्रुव संरचना option *opt,
						ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->मूल्य, arg, &sub_marks_from);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_rewrite_submodules_to(ध्रुव संरचना option *opt,
					      ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->मूल्य, arg, &sub_marks_to);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_cat_blob_fd(ध्रुव संरचना option *opt,
				    ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->मूल्य, arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_import_marks(ध्रुव संरचना option *opt,
				     ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->मूल्य, arg, 0, 0);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_import_marks_if_exists(ध्रुव संरचना option *opt,
					       ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->मूल्य, arg, 0, 1);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_export_marks(ध्रुव संरचना option *opt,
				     ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->मूल्य, arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_depth(ध्रुव संरचना option *opt UNUSED,
			      ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_active_branches(ध्रुव संरचना option *opt UNUSED,
					ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_quiet(ध्रुव संरचना option *opt UNUSED,
			      ध्रुव वर्ण *arg UNUSED, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_fast_import(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *prefix,
		    संरचना repository *repo)
{
	संरचना fast_import_state state;

	संरचना option fast_import_options[] = {
		OPT_GROUP(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", रिक्त, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", रिक्त, रिक्त,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", रिक्त, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_size),
		OPT_CALLBACK_F(0, "big-file-threshold", रिक्त, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_threshold),
		OPT_CALLBACK_F(0, "depth", रिक्त, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_depth),
		OPT_CALLBACK_F(0, "active-branches", रिक्त, N_("n"),
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
		OPT_CALLBACK_F(0, "signed-commits", रिक्त, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", रिक्त, N_("mode"),
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
	CALLOC_ARRAY(avail_वृक्ष_table, avail_वृक्ष_table_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, परिमाण(संरचना mark_set));

	hashmap_init(&object_table, object_entry_hashcmp, रिक्त, 0);

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
	प्रति (पूर्णाङ्क i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		यदि (*arg != '-' || !strcmp(arg, "--"))
			भङ्ग;
		यदि (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * परिमाण(*rc_free));
	प्रति (unsigned पूर्णाङ्क i = 0; i < (cmd_save - 1); i++)
		rc_free[i].next = &rc_free[i + 1];
	rc_free[cmd_save - 1].next = रिक्त;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	यावत् (read_next_command(&state) != EOF) {
		ध्रुव वर्ण *v;
		यदि (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		अन्यथा यदि (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		अन्यथा यदि (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		अन्यथा यदि (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		अन्यथा यदि (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, रिक्त);
		अन्यथा यदि (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		अन्यथा यदि (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		अन्यथा यदि (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		अन्यथा यदि (!strcmp("done", command_buf.buf))
			भङ्ग;
		अन्यथा यदि (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		अन्यथा यदि (starts_with(command_buf.buf, "progress "))
			parse_progress(&state);
		अन्यथा यदि (skip_prefix(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		अन्यथा यदि (skip_prefix(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		अन्यथा यदि (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		अन्यथा
			die(_("unsupported command: %s"), command_buf.buf);

		यदि (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	यदि (!state.seen_data_command)
		parse_argv(&state);

	यदि (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	end_packfile();

	dump_branches();
	dump_tags();
	unkeep_all_packs();
	dump_marks();

	यदि (pack_edges)
		fclose(pack_edges);

	यदि (show_stats) {
		uintmax_t total_गिनती = 0, duplicate_गिनती = 0;
		प्रति (size_t i = 0; i < ARRAY_SIZE(object_गिनती_by_type); i++)
			total_गिनती += object_गिनती_by_type[i];
		प्रति (size_t i = 0; i < ARRAY_SIZE(duplicate_गिनती_by_type); i++)
			duplicate_गिनती += duplicate_गिनती_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_गिनती);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", total_गिनती, duplicate_गिनती);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_गिनती_by_type[OBJ_BLOB], duplicate_गिनती_by_type[OBJ_BLOB], delta_गिनती_by_type[OBJ_BLOB], delta_गिनती_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_गिनती_by_type[OBJ_वृक्ष], duplicate_गिनती_by_type[OBJ_वृक्ष], delta_गिनती_by_type[OBJ_वृक्ष], delta_गिनती_attempts_by_type[OBJ_वृक्ष]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_गिनती_by_type[OBJ_COMMIT], duplicate_गिनती_by_type[OBJ_COMMIT], delta_गिनती_by_type[OBJ_COMMIT], delta_गिनती_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_गिनती_by_type[OBJ_TAG], duplicate_गिनती_by_type[OBJ_TAG], delta_गिनती_by_type[OBJ_TAG], delta_गिनती_attempts_by_type[OBJ_TAG]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_गिनती, branch_load_गिनती);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->shift) * 1024, marks_set_गिनती);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (वृक्ष_entry_allocd + fi_mem_pool.pool_alloc + alloc_गिनती*परिमाण(संरचना object_entry))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (unsigned long)((वृक्ष_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_गिनती*परिमाण(संरचना object_entry))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	प्रत्यागम failure ? 1 : 0;
}
