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

स्थिर पूर्णांक object_entry_hashcmp(स्थिरांक शून्य *map_data UNUSED,
				स्थिरांक संरचना hashmap_entry *eptr,
				स्थिरांक संरचना hashmap_entry *entry_or_किल्ली,
				स्थिरांक शून्य *keydata)
{
	स्थिरांक संरचना object_id *oid = keydata;
	स्थिरांक संरचना object_entry *e1, *e2;

	e1 = container_of(eptr, स्थिरांक संरचना object_entry, ent);
	जर (oid)
		परत oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_किल्ली, स्थिरांक संरचना object_entry, ent);
	परत oidcmp(&e1->idx.oid, &e2->idx.oid);
}

संरचना object_entry_pool {
	संरचना object_entry_pool *next_pool;
	संरचना object_entry *next_free;
	संरचना object_entry *end;
	संरचना object_entry entries[FLEX_ARRAY]; /* more */
};

संरचना mark_set {
	संघ {
		संरचना object_id *oids[1024];
		संरचना object_entry *marked[1024];
		संरचना mark_set *sets[1024];
	} data;
	अचिन्हित पूर्णांक shift;
};

संरचना last_object {
	संरचना strbuf data;
	off_t offset;
	अचिन्हित पूर्णांक depth;
	अचिन्हित no_swap : 1;
};

संरचना atom_str {
	संरचना atom_str *next_atom;
	अचिन्हित लघु str_len;
	अक्षर str_dat[FLEX_ARRAY]; /* more */
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
	अचिन्हित पूर्णांक entry_capacity; /* must match avail_tree_content */
	अचिन्हित पूर्णांक entry_मोजणी;
	अचिन्हित पूर्णांक delta_depth;
	संरचना वृक्ष_entry *entries[FLEX_ARRAY]; /* more */
};

संरचना avail_वृक्ष_content {
	अचिन्हित पूर्णांक entry_capacity; /* must match tree_content */
	संरचना avail_वृक्ष_content *next_avail;
};

संरचना branch {
	संरचना branch *table_next_branch;
	संरचना branch *active_next_branch;
	स्थिरांक अक्षर *name;
	संरचना वृक्ष_entry branch_वृक्ष;
	uintmax_t last_commit;
	uintmax_t अंक_notes;
	अचिन्हित active : 1;
	अचिन्हित delete : 1;
	अचिन्हित pack_id : PACK_ID_BITS;
	संरचना object_id oid;
};

संरचना tag {
	संरचना tag *next_tag;
	स्थिरांक अक्षर *name;
	अचिन्हित पूर्णांक pack_id;
	संरचना object_id oid;
};

संरचना hash_list {
	संरचना hash_list *next;
	संरचना object_id oid;
};

प्रकारव्याख्या गणना {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

संरचना recent_command {
	संरचना recent_command *prev;
	संरचना recent_command *next;
	अक्षर *buf;
};

प्रकारव्याख्या शून्य (*mark_set_inserter_t)(संरचना mark_set **s, संरचना object_id *oid, uintmax_t mark);
प्रकारव्याख्या शून्य (*each_mark_fn_t)(uintmax_t mark, शून्य *obj, शून्य *cbp);

/* Configured limits on output */
स्थिर अचिन्हित दीर्घ max_depth = 50;
स्थिर off_t max_packsize;
स्थिर पूर्णांक unpack_limit = 100;
स्थिर पूर्णांक force_update;

/* Stats and misc. counters */
स्थिर uintmax_t alloc_मोजणी;
स्थिर uintmax_t marks_set_मोजणी;
स्थिर uintmax_t object_मोजणी_by_type[1 << TYPE_BITS];
स्थिर uintmax_t duplicate_मोजणी_by_type[1 << TYPE_BITS];
स्थिर uintmax_t delta_मोजणी_by_type[1 << TYPE_BITS];
स्थिर uintmax_t delta_मोजणी_attempts_by_type[1 << TYPE_BITS];
स्थिर अचिन्हित दीर्घ object_मोजणी;
स्थिर अचिन्हित दीर्घ branch_मोजणी;
स्थिर अचिन्हित दीर्घ branch_load_मोजणी;
स्थिर पूर्णांक failure;
स्थिर खाता *pack_edges;
स्थिर पूर्णांक show_stats = 1;
स्थिर पूर्णांक quiet;
स्थिर गणना sign_mode signed_tag_mode = SIGN_VERBATIM;
स्थिर गणना sign_mode signed_commit_mode = SIGN_VERBATIM;
स्थिर स्थिरांक अक्षर *signed_commit_keyid;
स्थिर स्थिरांक अक्षर *signed_tag_keyid;

/* Memory pools */
स्थिर संरचना mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - आकार(संरचना mp_block),
};

/* Atom management */
स्थिर अचिन्हित पूर्णांक atom_table_sz = 4451;
स्थिर अचिन्हित पूर्णांक atom_cnt;
स्थिर संरचना atom_str **atom_table;

/* The .pack file being generated */
स्थिर संरचना pack_idx_option pack_idx_opts;
स्थिर अचिन्हित पूर्णांक pack_id;
स्थिर संरचना hashfile *pack_file;
स्थिर संरचना packed_git *pack_data;
स्थिर संरचना packed_git **all_packs;
स्थिर off_t pack_size;

/* Table of objects we've written. */
स्थिर अचिन्हित पूर्णांक object_entry_alloc = 5000;
स्थिर संरचना object_entry_pool *blocks;
स्थिर संरचना hashmap object_table;
स्थिर संरचना mark_set *marks;
स्थिर अक्षर *export_marks_file;
स्थिर अक्षर *import_marks_file;
स्थिर पूर्णांक import_marks_file_from_stream;
स्थिर पूर्णांक import_marks_file_ignore_missing;
स्थिर पूर्णांक import_marks_file_done;
स्थिर पूर्णांक relative_marks_paths;

/* Our last blob */
स्थिर संरचना last_object last_blob = {
	.data = STRBUF_INIT,
 };

/* Tree management */
स्थिर अचिन्हित पूर्णांक वृक्ष_entry_alloc = 1000;
स्थिर शून्य *avail_वृक्ष_entry;
स्थिर अचिन्हित पूर्णांक avail_वृक्ष_table_sz = 100;
स्थिर संरचना avail_वृक्ष_content **avail_वृक्ष_table;
स्थिर माप_प्रकार वृक्ष_entry_allocd;
स्थिर संरचना strbuf old_वृक्ष = STRBUF_INIT;
स्थिर संरचना strbuf new_वृक्ष = STRBUF_INIT;

/* Branch data */
स्थिर अचिन्हित दीर्घ max_active_branches = 5;
स्थिर अचिन्हित दीर्घ cur_active_branches;
स्थिर अचिन्हित दीर्घ branch_table_sz = 1039;
स्थिर संरचना branch **branch_table;
स्थिर संरचना branch *active_branches;

/* Tag data */
स्थिर संरचना tag *first_tag;
स्थिर संरचना tag *last_tag;

/* Input stream parsing */
स्थिर whenspec_type whenspec = WHENSPEC_RAW;
स्थिर संरचना strbuf command_buf = STRBUF_INIT;
स्थिर पूर्णांक unread_command_buf;
स्थिर संरचना recent_command cmd_hist = {
	.prev = &cmd_hist,
	.next = &cmd_hist,
};
स्थिर संरचना recent_command *cmd_tail = &cmd_hist;
स्थिर संरचना recent_command *rc_free;
स्थिर अचिन्हित पूर्णांक cmd_save = 100;
स्थिर uintmax_t next_mark;
स्थिर संरचना strbuf new_data = STRBUF_INIT;
स्थिर पूर्णांक require_explicit_termination;

/* Signal handling */
स्थिर अस्थिर संक_पूर्ण_प्रकार checkpoint_requested;

/* Submodule marks */
स्थिर संरचना string_list sub_marks_from = STRING_LIST_INIT_DUP;
स्थिर संरचना string_list sub_marks_to = STRING_LIST_INIT_DUP;
स्थिर kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
स्थिर पूर्णांक cat_blob_fd = STDOUT_FILENO;

/* Command state */
संरचना fast_import_state {
	पूर्णांक argc;
	स्थिरांक अक्षर **argv;
	स्थिरांक अक्षर *prefix;
	पूर्णांक seen_data_command;
	पूर्णांक allow_unsafe_features;
	संरचना option *option;
};

स्थिर शून्य fast_import_state_init(संरचना fast_import_state *state,
				   पूर्णांक argc, स्थिरांक अक्षर **argv,
				   स्थिरांक अक्षर *prefix, संरचना option *option)
{
	स_रखो(state, 0, आकार(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

स्थिर शून्य parse_argv(संरचना fast_import_state *state);
स्थिर शून्य parse_get_mark(संरचना fast_import_state *state, स्थिरांक अक्षर *p);
स्थिर शून्य parse_cat_blob(संरचना fast_import_state *state, स्थिरांक अक्षर *p);
स्थिर शून्य parse_ls(संरचना fast_import_state *state,
		     स्थिरांक अक्षर *p,
		     संरचना branch *b);

स्थिर शून्य for_each_mark(संरचना mark_set *m, uintmax_t base, each_mark_fn_t callback, शून्य *p)
{
	uintmax_t k;
	जर (m->shift) {
		साठी (k = 0; k < 1024; k++) {
			जर (m->data.sets[k])
				for_each_mark(m->data.sets[k], base + (k << m->shift), callback, p);
		}
	} अन्यथा {
		साठी (k = 0; k < 1024; k++) {
			जर (m->data.marked[k])
				callback(base + k, m->data.marked[k], p);
		}
	}
}

स्थिर शून्य dump_marks_fn(uintmax_t mark, शून्य *object, शून्य *cbp) {
	संरचना object_entry *e = object;
	खाता *f = cbp;

	ख_लिखो(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

स्थिर शून्य write_branch_report(खाता *rpt, संरचना branch *b)
{
	ख_लिखो(rpt, "%s:\n", b->name);

	ख_लिखो(rpt, "  status      :");
	जर (b->active)
		ख_माला_दो(" active", rpt);
	जर (b->branch_वृक्ष.वृक्ष)
		ख_माला_दो(" loaded", rpt);
	जर (is_null_oid(&b->branch_वृक्ष.versions[1].oid))
		ख_माला_दो(" dirty", rpt);
	ख_अक्षर_दो('\n', rpt);

	ख_लिखो(rpt, "  tip commit  : %s\n", oid_to_hex(&b->oid));
	ख_लिखो(rpt, "  old tree    : %s\n",
		oid_to_hex(&b->branch_वृक्ष.versions[0].oid));
	ख_लिखो(rpt, "  cur tree    : %s\n",
		oid_to_hex(&b->branch_वृक्ष.versions[1].oid));
	ख_लिखो(rpt, "  commit clock: %" PRIuMAX "\n", b->last_commit);

	ख_माला_दो("  last pack   : ", rpt);
	जर (b->pack_id < MAX_PACK_ID)
		ख_लिखो(rpt, "%u", b->pack_id);
	ख_अक्षर_दो('\n', rpt);

	ख_अक्षर_दो('\n', rpt);
}

स्थिर शून्य write_crash_report(स्थिरांक अक्षर *err)
{
	अक्षर *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	खाता *rpt = ख_खोलो(loc, "w");
	संरचना branch *b;
	अचिन्हित दीर्घ lu;
	संरचना recent_command *rc;

	जर (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		मुक्त(loc);
		परत;
	}

	ख_लिखो(मानक_त्रुटि, _("fast-import: dumping crash report to %s\n"), loc);

	ख_लिखो(rpt, "fast-import crash report:\n");
	ख_लिखो(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	ख_लिखो(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	ख_लिखो(rpt, "    at %s\n", show_date(समय(रिक्त), 0, DATE_MODE(ISO8601)));
	ख_अक्षर_दो('\n', rpt);

	ख_माला_दो("fatal: ", rpt);
	ख_माला_दो(err, rpt);
	ख_अक्षर_दो('\n', rpt);

	ख_अक्षर_दो('\n', rpt);
	ख_माला_दो("Most Recent Commands Before Crash\n", rpt);
	ख_माला_दो("---------------------------------\n", rpt);
	साठी (rc = cmd_hist.next; rc != &cmd_hist; rc = rc->next) {
		जर (rc->next == &cmd_hist)
			ख_माला_दो("* ", rpt);
		अन्यथा
			ख_माला_दो("  ", rpt);
		ख_माला_दो(rc->buf, rpt);
		ख_अक्षर_दो('\n', rpt);
	}

	ख_अक्षर_दो('\n', rpt);
	ख_माला_दो("Active Branch LRU\n", rpt);
	ख_माला_दो("-----------------\n", rpt);
	ख_लिखो(rpt, "    active_branches = %lu cur, %lu max\n",
		cur_active_branches,
		max_active_branches);
	ख_अक्षर_दो('\n', rpt);
	ख_माला_दो("  pos  clock name\n", rpt);
	ख_माला_दो("  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", rpt);
	साठी (b = active_branches, lu = 0; b; b = b->active_next_branch)
		ख_लिखो(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	ख_अक्षर_दो('\n', rpt);
	ख_माला_दो("Inactive Branches\n", rpt);
	ख_माला_दो("-----------------\n", rpt);
	साठी (lu = 0; lu < branch_table_sz; lu++) {
		साठी (b = branch_table[lu]; b; b = b->table_next_branch)
			write_branch_report(rpt, b);
	}

	जर (first_tag) {
		संरचना tag *tg;
		ख_अक्षर_दो('\n', rpt);
		ख_माला_दो("Annotated Tags\n", rpt);
		ख_माला_दो("--------------\n", rpt);
		साठी (tg = first_tag; tg; tg = tg->next_tag) {
			ख_माला_दो(oid_to_hex(&tg->oid), rpt);
			ख_अक्षर_दो(' ', rpt);
			ख_माला_दो(tg->name, rpt);
			ख_अक्षर_दो('\n', rpt);
		}
	}

	ख_अक्षर_दो('\n', rpt);
	ख_माला_दो("Marks\n", rpt);
	ख_माला_दो("-----\n", rpt);
	जर (export_marks_file)
		ख_लिखो(rpt, "  exported to %s\n", export_marks_file);
	अन्यथा
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	ख_अक्षर_दो('\n', rpt);
	ख_माला_दो("-------------------\n", rpt);
	ख_माला_दो("END OF CRASH REPORT\n", rpt);
	ख_बंद(rpt);
	मुक्त(loc);
}

स्थिर शून्य end_packfile(शून्य);
स्थिर शून्य unkeep_all_packs(शून्य);
स्थिर शून्य dump_marks(शून्य);

स्थिर NORETURN शून्य die_nicely(स्थिरांक अक्षर *err, बहु_सूची params)
{
	बहु_सूची cp;
	स्थिर पूर्णांक zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	जर (!zombie) {
		अक्षर message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, आकार(message), err, cp);
		write_crash_report(message);
		end_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	निकास(128);
}

#ifndef SIGUSR1	/* Windows, for example */

स्थिर शून्य set_checkpoint_signal(शून्य)
{
}

#अन्यथा

स्थिर शून्य checkpoint_signal(पूर्णांक signo UNUSED)
{
	checkpoint_requested = 1;
}

स्थिर शून्य set_checkpoint_signal(शून्य)
{
	संरचना sigaction sa;

	स_रखो(&sa, 0, आकार(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, रिक्त);
}

#endif

स्थिर शून्य alloc_objects(अचिन्हित पूर्णांक cnt)
{
	संरचना object_entry_pool *b;

	b = xmalloc(आकार(संरचना object_entry_pool)
		+ cnt * आकार(संरचना object_entry));
	b->next_pool = blocks;
	b->next_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_मोजणी += cnt;
}

स्थिर संरचना object_entry *new_object(संरचना object_id *oid)
{
	संरचना object_entry *e;

	जर (blocks->next_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->next_free++;
	oidcpy(&e->idx.oid, oid);
	परत e;
}

स्थिर संरचना object_entry *find_object(संरचना object_id *oid)
{
	परत hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   संरचना object_entry, ent);
}

स्थिर संरचना object_entry *घाला_object(संरचना object_id *oid)
{
	संरचना object_entry *e;
	अचिन्हित पूर्णांक hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					संरचना object_entry, ent);
	जर (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	परत e;
}

स्थिर शून्य invalidate_pack_id(अचिन्हित पूर्णांक id)
{
	अचिन्हित दीर्घ lu;
	संरचना tag *t;
	संरचना hashmap_iter iter;
	संरचना object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		जर (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	साठी (lu = 0; lu < branch_table_sz; lu++) {
		संरचना branch *b;

		साठी (b = branch_table[lu]; b; b = b->table_next_branch)
			जर (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	साठी (t = first_tag; t; t = t->next_tag)
		जर (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

स्थिर अचिन्हित पूर्णांक hc_str(स्थिरांक अक्षर *s, माप_प्रकार len)
{
	अचिन्हित पूर्णांक r = 0;
	पर्यंत (len-- > 0)
		r = r * 31 + *s++;
	परत r;
}

स्थिर शून्य घाला_mark(संरचना mark_set **शीर्ष, uintmax_t idnum, संरचना object_entry *oe)
{
	संरचना mark_set *s = *शीर्ष;

	पर्यंत ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, आकार(संरचना mark_set));
		s->shift = (*शीर्ष)->shift + 10;
		s->data.sets[0] = *शीर्ष;
		*शीर्ष = s;
	}
	पर्यंत (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		जर (!s->data.sets[i]) {
			s->data.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, आकार(संरचना mark_set));
			s->data.sets[i]->shift = s->shift - 10;
		}
		s = s->data.sets[i];
	}
	जर (!s->data.marked[idnum])
		marks_set_मोजणी++;
	s->data.marked[idnum] = oe;
}

स्थिर शून्य *find_mark(संरचना mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	संरचना object_entry *oe = रिक्त;
	जर ((idnum >> s->shift) < 1024) {
		पर्यंत (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->data.sets[i];
		}
		जर (s)
			oe = s->data.marked[idnum];
	}
	जर (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	परत oe;
}

स्थिर संरचना atom_str *to_atom(स्थिरांक अक्षर *s, अचिन्हित लघु len)
{
	अचिन्हित पूर्णांक hc = hc_str(s, len) % atom_table_sz;
	संरचना atom_str *c;

	साठी (c = atom_table[hc]; c; c = c->next_atom)
		जर (c->str_len == len && !म_भेदन(s, c->str_dat, len))
			परत c;

	c = mem_pool_alloc(&fi_mem_pool, आकार(संरचना atom_str) + len + 1);
	c->str_len = len;
	स_नकल(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->next_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	परत c;
}

स्थिर संरचना branch *lookup_branch(स्थिरांक अक्षर *name)
{
	अचिन्हित पूर्णांक hc = hc_str(name, म_माप(name)) % branch_table_sz;
	संरचना branch *b;

	साठी (b = branch_table[hc]; b; b = b->table_next_branch)
		जर (!म_भेद(name, b->name))
			परत b;
	परत रिक्त;
}

स्थिर संरचना branch *new_branch(स्थिरांक अक्षर *name)
{
	अचिन्हित पूर्णांक hc = hc_str(name, म_माप(name)) % branch_table_sz;
	संरचना branch *b = lookup_branch(name);

	जर (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	जर (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, आकार(संरचना branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_next_branch = branch_table[hc];
	b->branch_वृक्ष.versions[0].mode = S_IFDIR;
	b->branch_वृक्ष.versions[1].mode = S_IFDIR;
	b->अंक_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_मोजणी++;
	परत b;
}

स्थिर अचिन्हित पूर्णांक hc_entries(अचिन्हित पूर्णांक cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	परत cnt < avail_वृक्ष_table_sz ? cnt : avail_वृक्ष_table_sz - 1;
}

स्थिर संरचना वृक्ष_content *new_वृक्ष_content(अचिन्हित पूर्णांक cnt)
{
	संरचना avail_वृक्ष_content *f, *l = रिक्त;
	संरचना वृक्ष_content *t;
	अचिन्हित पूर्णांक hc = hc_entries(cnt);

	साठी (f = avail_वृक्ष_table[hc]; f; l = f, f = f->next_avail)
		जर (f->entry_capacity >= cnt)
			थांबा;

	जर (f) {
		जर (l)
			l->next_avail = f->next_avail;
		अन्यथा
			avail_वृक्ष_table[hc] = f->next_avail;
	} अन्यथा {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, आकार(*t) + आकार(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (संरचना वृक्ष_content*)f;
	t->entry_मोजणी = 0;
	t->delta_depth = 0;
	परत t;
}

स्थिर शून्य release_वृक्ष_entry(संरचना वृक्ष_entry *e);
स्थिर शून्य release_वृक्ष_content(संरचना वृक्ष_content *t)
{
	संरचना avail_वृक्ष_content *f = (संरचना avail_वृक्ष_content*)t;
	अचिन्हित पूर्णांक hc = hc_entries(f->entry_capacity);
	f->next_avail = avail_वृक्ष_table[hc];
	avail_वृक्ष_table[hc] = f;
}

स्थिर शून्य release_वृक्ष_content_recursive(संरचना वृक्ष_content *t)
{
	अचिन्हित पूर्णांक i;
	साठी (i = 0; i < t->entry_मोजणी; i++)
		release_वृक्ष_entry(t->entries[i]);
	release_वृक्ष_content(t);
}

स्थिर संरचना वृक्ष_content *grow_वृक्ष_content(
	संरचना वृक्ष_content *t,
	पूर्णांक amt)
{
	संरचना वृक्ष_content *r = new_वृक्ष_content(t->entry_मोजणी + amt);
	r->entry_मोजणी = t->entry_मोजणी;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_मोजणी);
	release_वृक्ष_content(t);
	परत r;
}

स्थिर संरचना वृक्ष_entry *new_वृक्ष_entry(शून्य)
{
	संरचना वृक्ष_entry *e;

	जर (!avail_वृक्ष_entry) {
		अचिन्हित पूर्णांक n = वृक्ष_entry_alloc;
		वृक्ष_entry_allocd += n * आकार(संरचना वृक्ष_entry);
		ALLOC_ARRAY(e, n);
		avail_वृक्ष_entry = e;
		पर्यंत (n-- > 1) {
			*((शून्य**)e) = e + 1;
			e++;
		}
		*((शून्य**)e) = रिक्त;
	}

	e = avail_वृक्ष_entry;
	avail_वृक्ष_entry = *((शून्य**)e);
	परत e;
}

स्थिर शून्य release_वृक्ष_entry(संरचना वृक्ष_entry *e)
{
	जर (e->वृक्ष)
		release_वृक्ष_content_recursive(e->वृक्ष);
	*((शून्य**)e) = avail_वृक्ष_entry;
	avail_वृक्ष_entry = e;
}

स्थिर संरचना वृक्ष_content *dup_वृक्ष_content(संरचना वृक्ष_content *s)
{
	संरचना वृक्ष_content *d;
	संरचना वृक्ष_entry *a, *b;
	अचिन्हित पूर्णांक i;

	जर (!s)
		परत रिक्त;
	d = new_वृक्ष_content(s->entry_मोजणी);
	साठी (i = 0; i < s->entry_मोजणी; i++) {
		a = s->entries[i];
		b = new_वृक्ष_entry();
		स_नकल(b, a, आकार(*a));
		जर (a->वृक्ष && is_null_oid(&b->versions[1].oid))
			b->वृक्ष = dup_वृक्ष_content(a->वृक्ष);
		अन्यथा
			b->वृक्ष = रिक्त;
		d->entries[i] = b;
	}
	d->entry_मोजणी = s->entry_मोजणी;
	d->delta_depth = s->delta_depth;

	परत d;
}

स्थिर शून्य start_packfile(शून्य)
{
	संरचना strbuf tmp_file = STRBUF_INIT;
	संरचना packed_git *p;
	पूर्णांक pack_fd;

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
	object_मोजणी = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

स्थिर स्थिरांक अक्षर *create_index(शून्य)
{
	स्थिरांक अक्षर *क्षणिक_ख;
	संरचना pack_idx_entry **idx, **c, **last;
	संरचना object_entry *e;
	संरचना object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_मोजणी);
	c = idx;
	साठी (o = blocks; o; o = o->next_pool)
		साठी (e = o->next_free; e-- != o->entries;)
			जर (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_मोजणी;
	जर (c != last)
		die(_("internal consistency error creating the index"));

	क्षणिक_ख = write_idx_file(the_repository, रिक्त, idx, object_मोजणी,
				 &pack_idx_opts, pack_data->hash);
	मुक्त(idx);
	परत क्षणिक_ख;
}

स्थिर अक्षर *keep_pack(स्थिरांक अक्षर *curr_index_name)
{
	स्थिर स्थिरांक अक्षर *keep_संदेश = "fast-import";
	संरचना strbuf name = STRBUF_INIT;
	पूर्णांक keep_fd;

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_data->repo,
							    name.buf);
	जर (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_संदेश, म_माप(keep_संदेश));
	जर (बंद(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "pack");
	जर (finalize_object_file(pack_data->repo, pack_data->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_data->repo, &name, pack_data->hash, "idx");
	जर (finalize_object_file(pack_data->repo, curr_index_name, name.buf))
		die(_("cannot store index file"));
	मुक्त((शून्य *)curr_index_name);
	परत strbuf_detach(&name, रिक्त);
}

स्थिर शून्य unkeep_all_packs(शून्य)
{
	संरचना strbuf name = STRBUF_INIT;
	पूर्णांक k;

	साठी (k = 0; k < pack_id; k++) {
		संरचना packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

स्थिर पूर्णांक loosen_small_pack(स्थिरांक संरचना packed_git *p)
{
	संरचना child_process unpack = CHILD_PROCESS_INIT;

	जर (lseek(p->pack_fd, 0, शुरू_से) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_ढकला(&unpack.args, "unpack-objects");
	जर (!show_stats)
		strvec_ढकला(&unpack.args, "-q");

	परत run_command(&unpack);
}

स्थिर शून्य end_packfile(शून्य)
{
	स्थिर पूर्णांक running;

	जर (running || !pack_data)
		परत;

	running = 1;
	clear_delta_base_cache();
	जर (object_मोजणी) {
		संरचना odb_source_files *files = odb_source_files_downcast(pack_data->repo->objects->source);
		संरचना packed_git *new_p;
		संरचना object_id cur_pack_oid;
		अक्षर *idx_name;
		पूर्णांक i;
		संरचना branch *b;
		संरचना tag *t;

		close_pack_windows(pack_data);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_data->pack_fd,
					 pack_data->hash, pack_data->pack_name,
					 object_मोजणी, cur_pack_oid.hash,
					 pack_size);

		जर (object_मोजणी <= unpack_limit) {
			जर (!loosen_small_pack(pack_data)) {
				invalidate_pack_id(pack_id);
				जा discard_pack;
			}
		}

		बंद(pack_data->pack_fd);
		idx_name = keep_pack(create_index());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		जर (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		मुक्त(idx_name);

		/* Print the boundary */
		जर (pack_edges) {
			ख_लिखो(pack_edges, "%s:", new_p->pack_name);
			साठी (i = 0; i < branch_table_sz; i++) {
				साठी (b = branch_table[i]; b; b = b->table_next_branch) {
					जर (b->pack_id == pack_id)
						ख_लिखो(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			साठी (t = first_tag; t; t = t->next_tag) {
				जर (t->pack_id == pack_id)
					ख_लिखो(pack_edges, " %s",
						oid_to_hex(&t->oid));
			}
			ख_अक्षर_दो('\n', pack_edges);
			ख_साफ(pack_edges);
		}

		pack_id++;
	}
	अन्यथा {
discard_pack:
		बंद(pack_data->pack_fd);
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

स्थिर पूर्णांक store_object(
	गणना object_type type,
	संरचना strbuf *dat,
	संरचना last_object *last,
	संरचना object_id *oidout,
	uintmax_t mark)
{
	संरचना odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	शून्य *out, *delta;
	संरचना object_entry *e;
	अचिन्हित अक्षर hdr[96];
	संरचना object_id oid;
	अचिन्हित दीर्घ hdrlen, deltalen = 0;
	संरचना git_hash_ctx c;
	git_zstream s;
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);

	hdrlen = format_object_header((अक्षर *)hdr, आकार(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	जर (oidout)
		oidcpy(oidout, &oid);

	e = घाला_object(&oid);
	जर (mark)
		घाला_mark(&marks, mark, e);
	जर (e->idx.offset) {
		duplicate_मोजणी_by_type[type]++;
		परत 1;
	}

	साठी (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		जर (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			सुरूठेवा;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_मोजणी_by_type[type]++;
		परत 1;
	}

	जर (last && last->data.len && last->data.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		माप_प्रकार deltalen_st;

		delta_मोजणी_attempts_by_type[type]++;
		delta = diff_delta(last->data.buf, last->data.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_size_t_to_ulong(deltalen_st);
	} अन्यथा
		delta = रिक्त;

	git_deflate_init(&s, cfg->pack_compression_level);
	जर (delta) {
		s.next_in = delta;
		s.avail_in = deltalen;
	} अन्यथा {
		s.next_in = (शून्य *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.next_out = out = xmalloc(s.avail_out);
	पर्यंत (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	जर ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + s.total_out) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + s.total_out) < pack_size) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		जर (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.next_in = (शून्य *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.next_out = out = xrealloc(out, s.avail_out);
			पर्यंत (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_end(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.offset = pack_size;
	object_मोजणी++;
	object_मोजणी_by_type[type]++;

	crc32_begin(pack_file);

	जर (delta) {
		off_t ofs = e->idx.offset - last->offset;
		अचिन्हित pos = आकार(hdr) - 1;

		delta_मोजणी_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, आकार(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;

		hdr[pos] = ofs & 127;
		पर्यंत (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, आकार(hdr) - pos);
		pack_size += आकार(hdr) - pos;
	} अन्यथा {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, आकार(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_size += hdrlen;
	}

	hashwrite(pack_file, out, s.total_out);
	pack_size += s.total_out;

	e->idx.crc32 = crc32_end(pack_file);

	मुक्त(out);
	मुक्त(delta);
	जर (last) {
		जर (last->no_swap) {
			last->data = *dat;
		} अन्यथा {
			strbuf_swap(&last->data, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	परत 0;
}

स्थिर शून्य truncate_pack(संरचना hashfile_checkpoint *checkpoint)
{
	जर (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_size = checkpoint->offset;
}

स्थिर शून्य stream_blob(uintmax_t len, संरचना object_id *oidout, uintmax_t mark)
{
	संरचना odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	माप_प्रकार in_sz = 64 * 1024, out_sz = 64 * 1024;
	अचिन्हित अक्षर *in_buf = xmalloc(in_sz);
	अचिन्हित अक्षर *out_buf = xmalloc(out_sz);
	संरचना object_entry *e;
	संरचना object_id oid;
	अचिन्हित दीर्घ hdrlen;
	off_t offset;
	संरचना git_hash_ctx c;
	git_zstream s;
	संरचना hashfile_checkpoint checkpoint;
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);
	पूर्णांक status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	जर ((max_packsize
		&& (pack_size + PACK_SIZE_THRESHOLD + len) > max_packsize)
		|| (pack_size + PACK_SIZE_THRESHOLD + len) < pack_size)
		cycle_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	offset = checkpoint.offset;

	hdrlen = format_object_header((अक्षर *)out_buf, out_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, out_buf, hdrlen);

	crc32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_in_pack_object_header(out_buf, out_sz, OBJ_BLOB, len);

	s.next_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	पर्यंत (status != Z_STREAM_END) {
		जर (0 < len && !s.avail_in) {
			माप_प्रकार cnt = in_sz < len ? in_sz : (माप_प्रकार)len;
			माप_प्रकार n = ख_पढ़ो(in_buf, 1, cnt, मानक_निवेश);
			जर (!n && ख_पूर्ण(मानक_निवेश))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.next_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		जर (!s.avail_out || status == Z_STREAM_END) {
			माप_प्रकार n = s.next_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_size += n;
			s.next_out = out_buf;
			s.avail_out = out_sz;
		}

		निवड (status) {
		स्थिती Z_OK:
		स्थिती Z_BUF_ERROR:
		स्थिती Z_STREAM_END:
			सुरूठेवा;
		पूर्वनिर्धारित:
			die(_("unexpected deflate failure: %d"), status);
		}
	}
	git_deflate_end(&s);
	git_hash_final_oid(&oid, &c);

	जर (oidout)
		oidcpy(oidout, &oid);

	e = घाला_object(&oid);

	जर (mark)
		घाला_mark(&marks, mark, e);

	जर (e->idx.offset) {
		duplicate_मोजणी_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		जा out;
	}

	साठी (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		जर (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			सुरूठेवा;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_मोजणी_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		जा out;
	}

	e->depth = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.offset = offset;
	e->idx.crc32 = crc32_end(pack_file);
	object_मोजणी++;
	object_मोजणी_by_type[OBJ_BLOB]++;

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
	अचिन्हित दीर्घ *sizep)
{
	गणना object_type type;
	माप_प्रकार size_st = 0;
	शून्य *data;
	संरचना packed_git *p = all_packs[oe->pack_id];
	जर (p == pack_data && p->pack_size < (pack_size + the_hash_algo->rawsz)) {
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
	जर (sizep)
		*sizep = cast_size_t_to_ulong(size_st);
	परत data;
}

स्थिर शून्य load_वृक्ष(संरचना वृक्ष_entry *मूळ)
{
	संरचना object_id *oid = &मूळ->versions[1].oid;
	संरचना object_entry *myoe;
	संरचना वृक्ष_content *t;
	अचिन्हित दीर्घ size;
	अक्षर *buf;
	स्थिरांक अक्षर *c;

	मूळ->वृक्ष = t = new_वृक्ष_content(8);
	जर (is_null_oid(oid))
		परत;

	myoe = find_object(oid);
	जर (myoe && myoe->pack_id != MAX_PACK_ID) {
		जर (myoe->type != OBJ_वृक्ष)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &size);
		जर (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} अन्यथा {
		गणना object_type type;
		माप_प्रकार size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		जर (!buf || type != OBJ_वृक्ष)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	पर्यंत (c != (buf + size)) {
		संरचना वृक्ष_entry *e = new_वृक्ष_entry();

		जर (t->entry_मोजणी == t->entry_capacity)
			मूळ->वृक्ष = t = grow_वृक्ष_content(t, t->entry_मोजणी);
		t->entries[t->entry_मोजणी++] = e;

		e->वृक्ष = रिक्त;
		c = parse_mode(c, &e->versions[1].mode);
		जर (!c)
			die(_("corrupt mode in %s"), oid_to_hex(oid));
		e->versions[0].mode = e->versions[1].mode;
		e->name = to_atom(c, म_माप(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (अचिन्हित अक्षर *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (अचिन्हित अक्षर *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	मुक्त(buf);
}

स्थिर पूर्णांक tecmp0 (स्थिरांक शून्य *_a, स्थिरांक शून्य *_b)
{
	संरचना वृक्ष_entry *a = *((संरचना वृक्ष_entry**)_a);
	संरचना वृक्ष_entry *b = *((संरचना वृक्ष_entry**)_b);
	परत base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

स्थिर पूर्णांक tecmp1 (स्थिरांक शून्य *_a, स्थिरांक शून्य *_b)
{
	संरचना वृक्ष_entry *a = *((संरचना वृक्ष_entry**)_a);
	संरचना वृक्ष_entry *b = *((संरचना वृक्ष_entry**)_b);
	परत base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

स्थिर शून्य mktree(संरचना वृक्ष_content *t, पूर्णांक v, संरचना strbuf *b)
{
	माप_प्रकार maxlen = 0;
	अचिन्हित पूर्णांक i;

	जर (!v)
		QSORT(t->entries, t->entry_मोजणी, tecmp0);
	अन्यथा
		QSORT(t->entries, t->entry_मोजणी, tecmp1);

	साठी (i = 0; i < t->entry_मोजणी; i++) {
		जर (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	साठी (i = 0; i < t->entry_मोजणी; i++) {
		संरचना वृक्ष_entry *e = t->entries[i];
		जर (!e->versions[v].mode)
			सुरूठेवा;
		strbuf_addf(b, "%o %s%c",
			(अचिन्हित पूर्णांक)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

स्थिर शून्य store_वृक्ष(संरचना वृक्ष_entry *मूळ)
{
	संरचना वृक्ष_content *t;
	अचिन्हित पूर्णांक i, j, del;
	संरचना last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	संरचना object_entry *le = रिक्त;

	जर (!is_null_oid(&मूळ->versions[1].oid))
		परत;

	जर (!मूळ->वृक्ष)
		load_वृक्ष(मूळ);
	t = मूळ->वृक्ष;

	साठी (i = 0; i < t->entry_मोजणी; i++) {
		जर (t->entries[i]->वृक्ष)
			store_वृक्ष(t->entries[i]);
	}

	जर (!(मूळ->versions[0].mode & NO_DELTA))
		le = find_object(&मूळ->versions[0].oid);
	जर (S_ISDIR(मूळ->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_वृक्ष);
		lo.data = old_वृक्ष;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_वृक्ष);
	store_object(OBJ_वृक्ष, &new_वृक्ष, &lo, &मूळ->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	साठी (i = 0, j = 0, del = 0; i < t->entry_मोजणी; i++) {
		संरचना वृक्ष_entry *e = t->entries[i];
		जर (e->versions[1].mode) {
			e->versions[0].mode = e->versions[1].mode;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} अन्यथा {
			release_वृक्ष_entry(e);
			del++;
		}
	}
	t->entry_मोजणी -= del;
}

स्थिर शून्य वृक्ष_content_replace(
	संरचना वृक्ष_entry *मूळ,
	स्थिरांक संरचना object_id *oid,
	स्थिरांक uint16_t mode,
	संरचना वृक्ष_content *newtree)
{
	जर (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&मूळ->versions[0].oid, the_repository->hash_algo);
	oidcpy(&मूळ->versions[1].oid, oid);
	जर (मूळ->वृक्ष)
		release_वृक्ष_content_recursive(मूळ->वृक्ष);
	मूळ->वृक्ष = newtree;
}

स्थिर पूर्णांक वृक्ष_content_set(
	संरचना वृक्ष_entry *मूळ,
	स्थिरांक अक्षर *p,
	स्थिरांक संरचना object_id *oid,
	स्थिरांक uint16_t mode,
	संरचना वृक्ष_content *subtree)
{
	संरचना वृक्ष_content *t;
	स्थिरांक अक्षर *slash1;
	अचिन्हित पूर्णांक i, n;
	संरचना वृक्ष_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	जर (!n)
		die(_("empty path component found in input"));
	जर (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	जर (!मूळ->वृक्ष)
		load_वृक्ष(मूळ);
	t = मूळ->वृक्ष;
	साठी (i = 0; i < t->entry_मोजणी; i++) {
		e = t->entries[i];
		जर (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			जर (!*slash1) {
				जर (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					परत 0;
				e->versions[1].mode = mode;
				oidcpy(&e->versions[1].oid, oid);
				जर (e->वृक्ष)
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
				जर (S_ISDIR(e->versions[0].mode))
					e->versions[0].mode |= NO_DELTA;

				oidclr(&मूळ->versions[1].oid, the_repository->hash_algo);
				परत 1;
			}
			जर (!S_ISDIR(e->versions[1].mode)) {
				e->वृक्ष = new_वृक्ष_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			जर (!e->वृक्ष)
				load_वृक्ष(e);
			जर (वृक्ष_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&मूळ->versions[1].oid, the_repository->hash_algo);
				परत 1;
			}
			परत 0;
		}
	}

	जर (t->entry_मोजणी == t->entry_capacity)
		मूळ->वृक्ष = t = grow_वृक्ष_content(t, t->entry_मोजणी);
	e = new_वृक्ष_entry();
	e->name = to_atom(p, n);
	e->versions[0].mode = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->entry_मोजणी++] = e;
	जर (*slash1) {
		e->वृक्ष = new_वृक्ष_content(8);
		e->versions[1].mode = S_IFDIR;
		वृक्ष_content_set(e, slash1 + 1, oid, mode, subtree);
	} अन्यथा {
		e->वृक्ष = subtree;
		e->versions[1].mode = mode;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&मूळ->versions[1].oid, the_repository->hash_algo);
	परत 1;
}

स्थिर पूर्णांक वृक्ष_content_remove(
	संरचना वृक्ष_entry *मूळ,
	स्थिरांक अक्षर *p,
	संरचना वृक्ष_entry *backup_leaf,
	पूर्णांक allow_मूळ)
{
	संरचना वृक्ष_content *t;
	स्थिरांक अक्षर *slash1;
	अचिन्हित पूर्णांक i, n;
	संरचना वृक्ष_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	जर (!मूळ->वृक्ष)
		load_वृक्ष(मूळ);

	जर (!*p && allow_मूळ) {
		e = मूळ;
		जा del_entry;
	}

	t = मूळ->वृक्ष;
	साठी (i = 0; i < t->entry_मोजणी; i++) {
		e = t->entries[i];
		जर (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			जर (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				परत 1;
			जर (!*slash1 || !S_ISDIR(e->versions[1].mode))
				जा del_entry;
			जर (!e->वृक्ष)
				load_वृक्ष(e);
			जर (वृक्ष_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				साठी (n = 0; n < e->वृक्ष->entry_मोजणी; n++) {
					जर (e->वृक्ष->entries[n]->versions[1].mode) {
						oidclr(&मूळ->versions[1].oid,
						       the_repository->hash_algo);
						परत 1;
					}
				}
				backup_leaf = रिक्त;
				जा del_entry;
			}
			परत 0;
		}
	}
	परत 0;

del_entry:
	जर (backup_leaf)
		स_नकल(backup_leaf, e, आकार(*backup_leaf));
	अन्यथा जर (e->वृक्ष)
		release_वृक्ष_content_recursive(e->वृक्ष);
	e->वृक्ष = रिक्त;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&मूळ->versions[1].oid, the_repository->hash_algo);
	परत 1;
}

स्थिर पूर्णांक वृक्ष_content_get(
	संरचना वृक्ष_entry *मूळ,
	स्थिरांक अक्षर *p,
	संरचना वृक्ष_entry *leaf,
	पूर्णांक allow_मूळ)
{
	संरचना वृक्ष_content *t;
	स्थिरांक अक्षर *slash1;
	अचिन्हित पूर्णांक i, n;
	संरचना वृक्ष_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	जर (!n && !allow_मूळ)
		die(_("empty path component found in input"));

	जर (!मूळ->वृक्ष)
		load_वृक्ष(मूळ);

	जर (!n) {
		e = मूळ;
		जा found_entry;
	}

	t = मूळ->वृक्ष;
	साठी (i = 0; i < t->entry_मोजणी; i++) {
		e = t->entries[i];
		जर (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			जर (!*slash1)
				जा found_entry;
			जर (!S_ISDIR(e->versions[1].mode))
				परत 0;
			जर (!e->वृक्ष)
				load_वृक्ष(e);
			परत वृक्ष_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	परत 0;

found_entry:
	स_नकल(leaf, e, आकार(*leaf));
	जर (e->वृक्ष && is_null_oid(&e->versions[1].oid))
		leaf->वृक्ष = dup_वृक्ष_content(e->वृक्ष);
	अन्यथा
		leaf->वृक्ष = रिक्त;
	परत 1;
}

स्थिर पूर्णांक update_branch(संरचना branch *b)
{
	स्थिर स्थिरांक अक्षर *संदेश = "fast-import";
	संरचना ref_transaction *transaction;
	संरचना object_id old_oid;
	संरचना strbuf err = STRBUF_INIT;
	स्थिर स्थिरांक अक्षर *replace_prefix = "refs/replace/";

	जर (starts_with(b->name, replace_prefix) &&
	    !म_भेद(b->name + म_माप(replace_prefix),
		    oid_to_hex(&b->oid))) {
		जर (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				रिक्त, b->name, रिक्त, 0);
		परत 0;
	}
	जर (is_null_oid(&b->oid)) {
		जर (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					रिक्त, b->name, रिक्त, 0);
		परत 0;
	}
	जर (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	जर (!force_update && !is_null_oid(&old_oid)) {
		संरचना commit *old_cmit, *new_cmit;
		पूर्णांक ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		जर (!old_cmit || !new_cmit)
			परत error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		जर (ret < 0)
			निकास(128);
		जर (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			परत -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	जर (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   रिक्त, रिक्त, 0, संदेश, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		परत -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	परत 0;
}

स्थिर शून्य dump_branches(शून्य)
{
	अचिन्हित पूर्णांक i;
	संरचना branch *b;

	साठी (i = 0; i < branch_table_sz; i++) {
		साठी (b = branch_table[i]; b; b = b->table_next_branch)
			failure |= update_branch(b);
	}
}

स्थिर शून्य dump_tags(शून्य)
{
	स्थिर स्थिरांक अक्षर *संदेश = "fast-import";
	संरचना tag *t;
	संरचना strbuf ref_name = STRBUF_INIT;
	संरचना strbuf err = STRBUF_INIT;
	संरचना ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	जर (!transaction) {
		failure |= error("%s", err.buf);
		जा cleanup;
	}
	साठी (t = first_tag; t; t = t->next_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		जर (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, रिक्त, रिक्त, रिक्त,
					   0, संदेश, &err)) {
			failure |= error("%s", err.buf);
			जा cleanup;
		}
	}
	जर (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

स्थिर शून्य dump_marks(शून्य)
{
	संरचना lock_file mark_lock = LOCK_INIT;
	खाता *f;

	जर (!export_marks_file || (import_marks_file && !import_marks_file_done))
		परत;

	जर (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		परत;
	}

	जर (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		परत;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	जर (!f) {
		पूर्णांक saved_errno = त्रुटि_सं;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, म_त्रुटि(saved_errno));
		परत;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	जर (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		परत;
	}
}

स्थिर शून्य घाला_object_entry(संरचना mark_set **s, संरचना object_id *oid, uintmax_t mark)
{
	संरचना object_entry *e;
	e = find_object(oid);
	जर (!e) {
		गणना object_type type = odb_read_object_info(the_repository->objects,
							     oid, रिक्त);
		जर (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = घाला_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	घाला_mark(s, mark, e);
}

स्थिर शून्य घाला_oid_entry(संरचना mark_set **s, संरचना object_id *oid, uintmax_t mark)
{
	घाला_mark(s, mark, xmemdupz(oid, आकार(*oid)));
}

स्थिर शून्य read_mark_file(संरचना mark_set **s, खाता *f, mark_set_inserter_t inserter)
{
	अक्षर ओळ[512];
	पर्यंत (ख_माला_लो(ओळ, आकार(ओळ), f)) {
		uintmax_t mark;
		अक्षर *end;
		संरचना object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		स_रखो(oid.hash, 0, आकार(oid.hash));

		end = म_अक्षर(ओळ, '\n');
		जर (ओळ[0] != ':' || !end)
			die(_("corrupt mark line: %s"), ओळ);
		*end = 0;
		mark = strtoumax(ओळ + 1, &end, 10);
		जर (!mark || end == ओळ + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), ओळ);
		inserter(s, &oid, mark);
	}
}

स्थिर शून्य read_marks(शून्य)
{
	खाता *f = ख_खोलो(import_marks_file, "r");
	जर (f)
		;
	अन्यथा जर (import_marks_file_ignore_missing && त्रुटि_सं == ENOENT)
		जा done; /* Marks file does not exist */
	अन्यथा
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, घाला_object_entry);
	ख_बंद(f);
done:
	import_marks_file_done = 1;
}


स्थिर पूर्णांक read_next_command(संरचना fast_import_state *state)
{
	स्थिर पूर्णांक stdin_eof = 0;

	जर (stdin_eof) {
		unread_command_buf = 0;
		परत खातापूर्ण;
	}

	साठी (;;) {
		जर (unread_command_buf) {
			unread_command_buf = 0;
		} अन्यथा {
			संरचना recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, मानक_निवेश);
			जर (stdin_eof)
				परत खातापूर्ण;

			जर (!state->seen_data_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			जर (rc)
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
		जर (command_buf.buf[0] == '#')
			सुरूठेवा;
		परत 0;
	}
}

स्थिर शून्य skip_optional_lf(शून्य)
{
	पूर्णांक term_char = ख_अक्षर_लो(मानक_निवेश);
	जर (term_char != '\n' && term_char != खातापूर्ण)
		अक्षर_वापस(term_char, मानक_निवेश);
}

स्थिर शून्य parse_mark(संरचना fast_import_state *state)
{
	स्थिरांक अक्षर *v;
	जर (skip_prefix(command_buf.buf, "mark :", &v)) {
		next_mark = strtoumax(v, रिक्त, 10);
		read_next_command(state);
	}
	अन्यथा
		next_mark = 0;
}

स्थिर शून्य parse_original_identifier(संरचना fast_import_state *state)
{
	स्थिरांक अक्षर *v;
	जर (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_next_command(state);
}

स्थिर पूर्णांक parse_data(संरचना strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	स्थिरांक अक्षर *data;
	strbuf_reset(sb);

	जर (!skip_prefix(command_buf.buf, "data ", &data))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	जर (skip_prefix(data, "<<", &data)) {
		अक्षर *term = xstrdup(data);
		माप_प्रकार term_len = command_buf.len - (data - command_buf.buf);

		साठी (;;) {
			जर (strbuf_getline_lf(&command_buf, मानक_निवेश) == खातापूर्ण)
				die(_("EOF in data (terminator '%s' not found)"), term);
			जर (term_len == command_buf.len
				&& !म_भेद(term, command_buf.buf))
				थांबा;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		मुक्त(term);
	}
	अन्यथा {
		uintmax_t len = strtoumax(data, रिक्त, 10);
		माप_प्रकार n = 0, length = (माप_प्रकार)len;

		जर (limit && limit < len) {
			*len_res = len;
			परत 0;
		}
		जर (length < len)
			die(_("data is too large to use in this context"));

		पर्यंत (n < length) {
			माप_प्रकार s = strbuf_fread(sb, length - n, मानक_निवेश);
			जर (!s && ख_पूर्ण(मानक_निवेश))
				die(_("EOF in data (%lu bytes remaining)"),
					(अचिन्हित दीर्घ)(length - n));
			n += s;
		}
	}

	skip_optional_lf();
	परत 1;
}

स्थिर पूर्णांक validate_raw_date(स्थिरांक अक्षर *src, संरचना strbuf *result, पूर्णांक strict)
{
	स्थिरांक अक्षर *orig_src = src;
	अक्षर *endp;
	अचिन्हित दीर्घ अंक;

	त्रुटि_सं = 0;

	अंक = म_से_अदीर्घ(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	जर (त्रुटि_सं || endp == src || *endp != ' ')
		परत -1;

	src = endp + 1;
	जर (*src != '-' && *src != '+')
		परत -1;

	अंक = म_से_अदीर्घ(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	जर (त्रुटि_सं || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < अंक))             /* parsed a broken timezone */
	   )
		परत -1;

	strbuf_addstr(result, orig_src);
	परत 0;
}

स्थिर अक्षर *parse_ident(स्थिरांक अक्षर *buf)
{
	स्थिरांक अक्षर *ltgt;
	माप_प्रकार name_len;
	संरचना strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	जर (*buf == '<')
		--buf;

	ltgt = buf + म_खोज(buf, "<>");
	जर (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	जर (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + म_खोज(ltgt + 1, "<>");
	जर (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	जर (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_add(&ident, buf, name_len);

	निवड (whenspec) {
	स्थिती WHENSPEC_RAW:
		जर (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		थांबा;
	स्थिती WHENSPEC_RAW_PERMISSIVE:
		जर (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		थांबा;
	स्थिती WHENSPEC_RFC2822:
		जर (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		थांबा;
	स्थिती WHENSPEC_NOW:
		जर (म_भेद("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		थांबा;
	}

	परत strbuf_detach(&ident, रिक्त);
}

स्थिर शून्य parse_and_store_blob(
	संरचना last_object *last,
	संरचना object_id *oidout,
	uintmax_t mark)
{
	स्थिर संरचना strbuf buf = STRBUF_INIT;
	uintmax_t len;

	जर (parse_data(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	अन्यथा {
		जर (last) {
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
	पर्यंत (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = अच_दीर्घ_उच्च;
		संरचना branch *e, *l = रिक्त, *p = रिक्त;

		साठी (e = active_branches; e; e = e->active_next_branch) {
			जर (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		जर (p) {
			e = p->active_next_branch;
			p->active_next_branch = e->active_next_branch;
		} अन्यथा {
			e = active_branches;
			active_branches = e->active_next_branch;
		}
		e->active = 0;
		e->active_next_branch = रिक्त;
		जर (e->branch_वृक्ष.वृक्ष) {
			release_वृक्ष_content_recursive(e->branch_वृक्ष.वृक्ष);
			e->branch_वृक्ष.वृक्ष = रिक्त;
		}
		cur_active_branches--;
	}
}

स्थिर शून्य load_branch(संरचना branch *b)
{
	load_वृक्ष(&b->branch_वृक्ष);
	जर (!b->active) {
		b->active = 1;
		b->active_next_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_मोजणी++;
	}
}

स्थिर अचिन्हित अक्षर convert_अंक_notes_to_fanout(uintmax_t अंक_notes)
{
	अचिन्हित अक्षर fanout = 0;
	पर्यंत ((अंक_notes >>= 8))
		fanout++;
	परत fanout;
}

स्थिर शून्य construct_path_with_fanout(स्थिरांक अक्षर *hex_sha1,
		अचिन्हित अक्षर fanout, अक्षर *path)
{
	अचिन्हित पूर्णांक i = 0, j = 0;
	जर (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	पर्यंत (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	स_नकल(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

स्थिर uintmax_t do_change_note_fanout(
		संरचना वृक्ष_entry *orig_मूळ, संरचना वृक्ष_entry *मूळ,
		अक्षर *hex_oid, अचिन्हित पूर्णांक hex_oid_len,
		अक्षर *fullpath, अचिन्हित पूर्णांक fullpath_len,
		अचिन्हित अक्षर fanout)
{
	संरचना वृक्ष_content *t;
	संरचना वृक्ष_entry *e, leaf;
	अचिन्हित पूर्णांक i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t अंक_notes = 0;
	संरचना object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	अक्षर realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	स्थिरांक अचिन्हित hexsz = the_hash_algo->hexsz;

	जर (!मूळ->वृक्ष)
		load_वृक्ष(मूळ);
	t = मूळ->वृक्ष;

	साठी (i = 0; t && i < t->entry_मोजणी; i++) {
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
		जर (!e->versions[1].mode ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			सुरूठेवा;

		/* This _may_ be a note entry, or a subdir containing notes */
		स_नकल(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		जर (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		स_नकल(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		जर (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			जर (fanout == 0xff) {
				/* Counting mode, no rename */
				अंक_notes++;
				सुरूठेवा;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			जर (!म_भेद(fullpath, realpath)) {
				/* Note entry is in correct location */
				अंक_notes++;
				सुरूठेवा;
			}

			/* Rename fullpath to realpath */
			जर (!वृक्ष_content_remove(orig_मूळ, fullpath, &leaf, 0))
				die(_("failed to remove path %s"), fullpath);
			वृक्ष_content_set(orig_मूळ, realpath,
				&leaf.versions[1].oid,
				leaf.versions[1].mode,
				leaf.वृक्ष);
		} अन्यथा जर (S_ISDIR(e->versions[1].mode)) {
			/* This is a subdir that may contain note entries */
			अंक_notes += do_change_note_fanout(orig_मूळ, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = मूळ->वृक्ष;
	}
	परत अंक_notes;
}

स्थिर uintmax_t change_note_fanout(संरचना वृक्ष_entry *मूळ,
		अचिन्हित अक्षर fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	अक्षर hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	परत do_change_note_fanout(मूळ, मूळ, hex_oid, 0, path, 0, fanout);
}

स्थिर पूर्णांक parse_mapped_oid_hex(स्थिरांक अक्षर *hex, संरचना object_id *oid, स्थिरांक अक्षर **end)
{
	पूर्णांक algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	स_रखो(oid->hash, 0, आकार(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	जर (algo == GIT_HASH_UNKNOWN)
		परत -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	जर (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		जर (hash_algos[algo].format_id == the_hash_algo->format_id)
			परत 0;
		परत -1;
	}
	oidcpy(oid, kh_मूल्य(sub_oid_map, it));
	परत 0;
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
स्थिर uintmax_t parse_mark_ref(स्थिरांक अक्षर *p, अक्षर **endptr)
{
	uintmax_t mark;

	प्रतिज्ञा(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	जर (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	परत mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
स्थिर uintmax_t parse_mark_ref_eol(स्थिरांक अक्षर *p)
{
	अक्षर *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	जर (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	परत mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
स्थिर uintmax_t parse_mark_ref_space(स्थिरांक अक्षर **p)
{
	uintmax_t mark;
	अक्षर *end;

	mark = parse_mark_ref(*p, &end);
	जर (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	परत mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
स्थिर शून्य parse_path(संरचना strbuf *sb, स्थिरांक अक्षर *p, स्थिरांक अक्षर **endp,
		पूर्णांक is_last_field, स्थिरांक अक्षर *field)
{
	जर (*p == '"') {
		जर (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), field, command_buf.buf);
		जर (म_माप(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), field, command_buf.buf);
	} अन्यथा {
		/*
		 * Unless we are parsing the last field of a line,
		 * SP is the end of this field.
		 */
		*endp = is_last_field
			? p + म_माप(p)
			: strchrnul(p, ' ');
		strbuf_add(sb, p, *endp - p);
	}
}

/*
 * Parse the path string into the strbuf, and complain if this is not the end of
 * the string. Unquoted strings may contain spaces.
 */
स्थिर शून्य parse_path_eol(संरचना strbuf *sb, स्थिरांक अक्षर *p, स्थिरांक अक्षर *field)
{
	स्थिरांक अक्षर *end;

	parse_path(sb, p, &end, 1, field);
	जर (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
स्थिर शून्य parse_path_space(संरचना strbuf *sb, स्थिरांक अक्षर *p,
		स्थिरांक अक्षर **endp, स्थिरांक अक्षर *field)
{
	parse_path(sb, p, endp, 0, field);
	जर (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

स्थिर शून्य file_change_m(संरचना fast_import_state *state,
			  स्थिरांक अक्षर *p,
			  संरचना branch *b)
{
	स्थिर संरचना strbuf path = STRBUF_INIT;
	संरचना object_entry *oe;
	संरचना object_id oid;
	uint16_t mode, inline_data = 0;

	p = parse_mode(p, &mode);
	जर (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	निवड (mode) {
	स्थिती 0644:
	स्थिती 0755:
		mode |= S_IFREG;
	स्थिती S_IFREG | 0644:
	स्थिती S_IFREG | 0755:
	स्थिती S_IFLNK:
	स्थिती S_IFDIR:
	स्थिती S_IFGITLINK:
		/* ok */
		थांबा;
	पूर्वनिर्धारित:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	जर (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा जर (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = रिक्त; /* not used with inline_data, but makes gcc happy */
	} अन्यथा {
		जर (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		जर (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	जर (S_ISDIR(mode) &&
	    is_empty_वृक्ष_oid(&oid, the_repository->hash_algo) &&
	    *path.buf) {
		वृक्ष_content_remove(&b->branch_वृक्ष, path.buf, रिक्त, 0);
		परत;
	}

	जर (S_ISGITLINK(mode)) {
		जर (inline_data)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		अन्यथा जर (oe) {
			जर (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} अन्यथा जर (inline_data) {
		जर (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		पर्यंत (read_next_command(state) != खातापूर्ण) {
			स्थिरांक अक्षर *v;
			जर (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			अन्यथा {
				parse_and_store_blob(&last_blob, &oid, 0);
				थांबा;
			}
		}
	} अन्यथा {
		गणना object_type expected = S_ISDIR(mode) ?
						OBJ_वृक्ष: OBJ_BLOB;
		गणना object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, रिक्त);
		जर (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(mode) ?  _("tree") : _("blob"),
			    command_buf.buf);
		जर (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	जर (!*path.buf) {
		वृक्ष_content_replace(&b->branch_वृक्ष, &oid, mode, रिक्त);
		परत;
	}

	जर (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	वृक्ष_content_set(&b->branch_वृक्ष, path.buf, &oid, mode, रिक्त);
}

स्थिर शून्य file_change_d(स्थिरांक अक्षर *p, संरचना branch *b)
{
	स्थिर संरचना strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	वृक्ष_content_remove(&b->branch_वृक्ष, path.buf, रिक्त, 1);
}

स्थिर शून्य file_change_cr(स्थिरांक अक्षर *p, संरचना branch *b, पूर्णांक नाम)
{
	स्थिर संरचना strbuf source = STRBUF_INIT;
	स्थिर संरचना strbuf dest = STRBUF_INIT;
	संरचना वृक्ष_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	स_रखो(&leaf, 0, आकार(leaf));
	जर (नाम)
		वृक्ष_content_remove(&b->branch_वृक्ष, source.buf, &leaf, 1);
	अन्यथा
		वृक्ष_content_get(&b->branch_वृक्ष, source.buf, &leaf, 1);
	जर (!leaf.versions[1].mode)
		die(_("path %s not in branch"), source.buf);
	जर (!*dest.buf) {	/* C "path/to/subdir" "" */
		वृक्ष_content_replace(&b->branch_वृक्ष,
			&leaf.versions[1].oid,
			leaf.versions[1].mode,
			leaf.वृक्ष);
		परत;
	}
	जर (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	वृक्ष_content_set(&b->branch_वृक्ष, dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.वृक्ष);
}

स्थिर शून्य note_change_n(संरचना fast_import_state *state,
			  स्थिरांक अक्षर *p,
			  संरचना branch *b,
			  अचिन्हित अक्षर *old_fanout)
{
	संरचना object_entry *oe;
	संरचना branch *s;
	संरचना object_id oid, commit_oid;
	अक्षर path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_data = 0;
	अचिन्हित अक्षर new_fanout;

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
	जर (b->अंक_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->अंक_notes = change_note_fanout(&b->branch_वृक्ष, 0xff);
		*old_fanout = convert_अंक_notes_to_fanout(b->अंक_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	जर (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा जर (skip_prefix(p, "inline ", &p)) {
		inline_data = 1;
		oe = रिक्त; /* not used with inline_data, but makes gcc happy */
	} अन्यथा {
		जर (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		जर (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	जर (s) {
		जर (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} अन्यथा जर (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		संरचना object_entry *commit_oe = find_mark(marks, commit_mark);
		जर (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} अन्यथा जर (!repo_get_oid(the_repository, p, &commit_oid)) {
		माप_प्रकार size;
		अक्षर *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &size,
						   &commit_oid);
		जर (!buf || size < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		मुक्त(buf);
	} अन्यथा
		die(_("invalid ref name or SHA1 expression: %s"), p);

	जर (inline_data) {
		read_next_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} अन्यथा जर (oe) {
		जर (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} अन्यथा जर (!is_null_oid(&oid)) {
		गणना object_type type = odb_read_object_info(the_repository->objects, &oid,
							रिक्त);
		जर (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		जर (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	जर (वृक्ष_content_remove(&b->branch_वृक्ष, path, रिक्त, 0))
		b->अंक_notes--;

	जर (is_null_oid(&oid))
		परत; /* nothing to insert */

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

स्थिर शून्य parse_from_commit(संरचना branch *b, अक्षर *buf, अचिन्हित दीर्घ size)
{
	जर (!buf || size < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	जर (स_भेद("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_वृक्ष.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_वृक्ष.versions[0].oid,
	       &b->branch_वृक्ष.versions[1].oid);
}

स्थिर शून्य parse_from_existing(संरचना branch *b)
{
	जर (is_null_oid(&b->oid)) {
		oidclr(&b->branch_वृक्ष.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_वृक्ष.versions[1].oid, the_repository->hash_algo);
	} अन्यथा {
		अचिन्हित दीर्घ size;
		माप_प्रकार size_st = 0;
		अक्षर *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &size_st, &b->oid);
		size = cast_size_t_to_ulong(size_st);
		parse_from_commit(b, buf, size);
		मुक्त(buf);
	}
}

स्थिर पूर्णांक parse_objectish(संरचना fast_import_state *state,
			   संरचना branch *b,
			   स्थिरांक अक्षर *objectish)
{
	संरचना branch *s;
	संरचना object_id oid;

	oidcpy(&oid, &b->branch_वृक्ष.versions[1].oid);

	s = lookup_branch(objectish);
	जर (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	अन्यथा जर (s) {
		संरचना object_id *t = &s->branch_वृक्ष.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_वृक्ष.versions[0].oid, t);
		oidcpy(&b->branch_वृक्ष.versions[1].oid, t);
	} अन्यथा जर (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		संरचना object_entry *oe = find_mark(marks, idnum);
		जर (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		जर (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			जर (oe->pack_id != MAX_PACK_ID) {
				अचिन्हित दीर्घ size;
				अक्षर *buf = gfi_unpack_entry(oe, &size);
				parse_from_commit(b, buf, size);
				मुक्त(buf);
			} अन्यथा
				parse_from_existing(b);
		}
	} अन्यथा जर (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		जर (is_null_oid(&b->oid))
			b->delete = 1;
	}
	अन्यथा
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	जर (b->branch_वृक्ष.वृक्ष && !oideq(&oid, &b->branch_वृक्ष.versions[1].oid)) {
		release_वृक्ष_content_recursive(b->branch_वृक्ष.वृक्ष);
		b->branch_वृक्ष.वृक्ष = रिक्त;
	}

	read_next_command(state);
	परत 1;
}

स्थिर पूर्णांक parse_from(संरचना fast_import_state *state, संरचना branch *b)
{
	स्थिरांक अक्षर *from;

	जर (!skip_prefix(command_buf.buf, "from ", &from))
		परत 0;

	परत parse_objectish(state, b, from);
}

स्थिर पूर्णांक parse_objectish_with_prefix(संरचना fast_import_state *state,
				       संरचना branch *b,
				       स्थिरांक अक्षर *prefix)
{
	स्थिरांक अक्षर *base;

	जर (!skip_prefix(command_buf.buf, prefix, &base))
		परत 0;

	परत parse_objectish(state, b, base);
}

स्थिर संरचना hash_list *parse_merge(संरचना fast_import_state *state,
				     अचिन्हित पूर्णांक *मोजणी)
{
	संरचना hash_list *list = रिक्त, **tail = &list, *n;
	स्थिरांक अक्षर *from;
	संरचना branch *s;

	*मोजणी = 0;
	पर्यंत (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(आकार(*n));
		s = lookup_branch(from);
		जर (s)
			oidcpy(&n->oid, &s->oid);
		अन्यथा जर (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			संरचना object_entry *oe = find_mark(marks, idnum);
			जर (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} अन्यथा जर (!repo_get_oid(the_repository, from, &n->oid)) {
			माप_प्रकार size;
			अक्षर *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &size, &n->oid);
			जर (!buf || size < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			मुक्त(buf);
		} अन्यथा
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->next = रिक्त;
		*tail = n;
		tail = &n->next;

		(*मोजणी)++;
		read_next_command(state);
	}
	परत list;
}

संरचना signature_data {
	अक्षर *hash_algo;      /* "sha1" or "sha256" */
	अक्षर *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	संरचना strbuf data;   /* The actual signature data */
};

स्थिर शून्य parse_one_signature(संरचना fast_import_state *state,
				संरचना signature_data *sig,
				स्थिरांक अक्षर *v)
{
	अक्षर *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	अक्षर *space = म_अक्षर(args, ' ');

	जर (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), args);
	*space = '\0';

	sig->hash_algo = args;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	जर (म_भेद(sig->hash_algo, "sha1") &&
	    म_भेद(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	जर (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	जर (!म_भेद(sig->sig_format, "unknown"))
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
				 स्थिरांक अक्षर *header,
				 संरचना signature_data *sig)
{
	संरचना string_list siglines = STRING_LIST_INIT_NODUP;

	जर (!sig || !sig->hash_algo)
		परत;

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
			    स्थिरांक अक्षर *hash_type)
{
	जर (stored_sig->hash_algo) {
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
				 स्थिरांक अक्षर *v)
{
	संरचना signature_data sig = { रिक्त, रिक्त, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	जर (!म_भेद(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	अन्यथा जर (!म_भेद(sig.hash_algo, "sha256"))
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
				   स्थिरांक अक्षर *संदेश, गणना sign_mode mode)
{
	स्थिरांक अक्षर *signer = check->signer ? check->signer : _("unknown");
	स्थिरांक अक्षर *subject;
	पूर्णांक subject_len = find_commit_subject(संदेश, &subject);

	निवड (mode) {
	स्थिती SIGN_STRIP_IF_INVALID:
		जर (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		अन्यथा जर (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		अन्यथा
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		थांबा;
	स्थिती SIGN_SIGN_IF_INVALID:
		जर (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		अन्यथा जर (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		अन्यथा
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		थांबा;
	पूर्वनिर्धारित:
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
	पूर्णांक ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_data);
	finalize_commit_buffer(&tmp_buf, sig_sha1, sig_sha256, संदेश);
	ret = verify_commit_buffer(tmp_buf.buf, tmp_buf.len, &signature_check);

	जर (ret) {
		जर (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, संदेश->buf, mode);

		जर (mode == SIGN_SIGN_IF_INVALID) {
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
			जर (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			जर (sign_buffer(&payload, &signature, signed_commit_keyid,
					SIGN_BUFFER_USE_DEFAULT_किल्ली))
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

स्थिर शून्य parse_new_commit(संरचना fast_import_state *state, स्थिरांक अक्षर *arg)
{
	स्थिर संरचना strbuf संदेश = STRBUF_INIT;
	संरचना signature_data sig_sha1 = { रिक्त, रिक्त, STRBUF_INIT };
	संरचना signature_data sig_sha256 = { रिक्त, रिक्त, STRBUF_INIT };
	संरचना branch *b;
	अक्षर *author = रिक्त;
	अक्षर *committer = रिक्त;
	अक्षर *encoding = रिक्त;
	संरचना hash_list *merge_list = रिक्त;
	अचिन्हित पूर्णांक merge_मोजणी;
	अचिन्हित अक्षर prev_fanout, new_fanout;
	स्थिरांक अक्षर *v;

	b = lookup_branch(arg);
	जर (!b)
		b = new_branch(arg);

	read_next_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	जर (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_next_command(state);
	}
	जर (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_next_command(state);
	}
	जर (!committer)
		die(_("expected committer but didn't get one"));

	पर्यंत (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		निवड (signed_commit_mode) {

		/* First, modes that don't need the signature to be parsed */
		स्थिती SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		स्थिती SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		स्थिती SIGN_STRIP:
			discard_one_signature(state);
			थांबा;

		/* Second, modes that parse the signature */
		स्थिती SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		स्थिती SIGN_VERBATIM:
		स्थिती SIGN_STRIP_IF_INVALID:
		स्थिती SIGN_SIGN_IF_INVALID:
		स्थिती SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			थांबा;

		/* Third, BUG */
		पूर्वनिर्धारित:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_next_command(state);
	}

	जर (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_next_command(state);
	}
	parse_data(&संदेश, 0, रिक्त);
	read_next_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_मोजणी);

	/* ensure the branch is active/loaded */
	जर (!b->branch_वृक्ष.वृक्ष || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	prev_fanout = convert_अंक_notes_to_fanout(b->अंक_notes);

	/* file_change* */
	पर्यंत (command_buf.len > 0) {
		जर (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		अन्यथा जर (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		अन्यथा जर (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		अन्यथा जर (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		अन्यथा जर (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		अन्यथा जर (!म_भेद("deleteall", command_buf.buf))
			file_change_deleteall(b);
		अन्यथा जर (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		अन्यथा जर (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		अन्यथा {
			unread_command_buf = 1;
			थांबा;
		}
		जर (read_next_command(state) == खातापूर्ण)
			थांबा;
	}

	new_fanout = convert_अंक_notes_to_fanout(b->अंक_notes);
	जर (new_fanout != prev_fanout)
		b->अंक_notes = change_note_fanout(&b->branch_वृक्ष, new_fanout);

	/* build the tree and the commit */
	store_वृक्ष(&b->branch_वृक्ष);
	oidcpy(&b->branch_वृक्ष.versions[0].oid,
	       &b->branch_वृक्ष.versions[1].oid);

	strbuf_reset(&new_data);
	strbuf_addf(&new_data, "tree %s\n",
		oid_to_hex(&b->branch_वृक्ष.versions[1].oid));
	जर (!is_null_oid(&b->oid))
		strbuf_addf(&new_data, "parent %s\n",
			    oid_to_hex(&b->oid));
	पर्यंत (merge_list) {
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
	जर (encoding)
		strbuf_addf(&new_data,
			"encoding %s\n",
			encoding);

	जर ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
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

	जर (!store_object(OBJ_COMMIT, &new_data, रिक्त, &b->oid, next_mark))
		b->pack_id = pack_id;
	b->last_commit = object_मोजणी_by_type[OBJ_COMMIT];
}

स्थिर शून्य handle_tag_signature_if_invalid(संरचना strbuf *buf,
					    संरचना strbuf *संदेश,
					    माप_प्रकार sig_offset)
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

	जर (!check_signature(&sigc, signature.buf, signature.len))
		जा out;

	जर (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(संदेश, sig_offset);

	जर (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = रिक्त;
		strbuf_reset(&signature);

		जर (sign_buffer(&payload, &signature, signed_tag_keyid,
				SIGN_BUFFER_USE_DEFAULT_किल्ली))
			die(_("failed to sign tag object"));

		strbuf_addbuf(संदेश, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

स्थिर शून्य handle_tag_signature(संरचना strbuf *buf, संरचना strbuf *संदेश, स्थिरांक अक्षर *name)
{
	माप_प्रकार sig_offset = parse_signed_buffer(संदेश->buf, संदेश->len);

	/* If there is no signature, there is nothing to do. */
	जर (sig_offset >= संदेश->len)
		परत;

	निवड (signed_tag_mode) {

	/* First, modes that don't change anything */
	स्थिती SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	स्थिती SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		थांबा;

	/* Second, modes that remove the signature */
	स्थिती SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	स्थिती SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(संदेश, sig_offset);
		थांबा;
	स्थिती SIGN_ABORT_IF_INVALID:
	स्थिती SIGN_SIGN_IF_INVALID:
	स्थिती SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, संदेश, sig_offset);
		थांबा;

	/* Third, aborting modes */
	स्थिती SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	पूर्वनिर्धारित:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

स्थिर शून्य parse_new_tag(संरचना fast_import_state *state, स्थिरांक अक्षर *arg)
{
	स्थिर संरचना strbuf संदेश = STRBUF_INIT;
	स्थिरांक अक्षर *from;
	अक्षर *tagger;
	संरचना branch *s;
	संरचना tag *t;
	uintmax_t from_mark = 0;
	संरचना object_id oid;
	गणना object_type type;
	स्थिरांक अक्षर *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, आकार(संरचना tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	जर (last_tag)
		last_tag->next_tag = t;
	अन्यथा
		first_tag = t;
	last_tag = t;
	read_next_command(state);
	parse_mark(state);

	/* from ... */
	जर (!skip_prefix(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	जर (s) {
		जर (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} अन्यथा जर (*from == ':') {
		संरचना object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा जर (!repo_get_oid(the_repository, from, &oid)) {
		संरचना object_entry *oe = find_object(&oid);
		जर (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, रिक्त);
			जर (type < 0)
				die(_("not a valid object: %s"), from);
		} अन्यथा
			type = oe->type;
	} अन्यथा
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_next_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	जर (skip_prefix(command_buf.buf, "tagger ", &v)) {
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
	जर (tagger)
		strbuf_addf(&new_data,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_data, &संदेश, t->name);

	strbuf_addch(&new_data, '\n');
	strbuf_addbuf(&new_data, &संदेश);
	मुक्त(tagger);

	जर (store_object(OBJ_TAG, &new_data, रिक्त, &t->oid, next_mark))
		t->pack_id = MAX_PACK_ID;
	अन्यथा
		t->pack_id = pack_id;
}

स्थिर शून्य parse_reset_branch(संरचना fast_import_state *state, स्थिरांक अक्षर *arg)
{
	संरचना branch *b;
	स्थिरांक अक्षर *tag_name;

	b = lookup_branch(arg);
	जर (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_वृक्ष.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_वृक्ष.versions[1].oid, the_repository->hash_algo);
		जर (b->branch_वृक्ष.वृक्ष) {
			release_वृक्ष_content_recursive(b->branch_वृक्ष.वृक्ष);
			b->branch_वृक्ष.वृक्ष = रिक्त;
		}
	}
	अन्यथा
		b = new_branch(arg);
	read_next_command(state);
	parse_from(state, b);
	जर (b->delete && skip_prefix(b->name, "refs/tags/", &tag_name)) {
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
		साठी (t = first_tag; t; t = t->next_tag) {
			जर (!म_भेद(t->name, tag_name))
				थांबा;
			prev = t;
		}
		जर (t) {
			जर (prev)
				prev->next_tag = t->next_tag;
			अन्यथा
				first_tag = t->next_tag;
			जर (!t->next_tag)
				last_tag = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	जर (command_buf.len > 0)
		unread_command_buf = 1;
}

स्थिर शून्य cat_blob_write(स्थिरांक अक्षर *buf, अचिन्हित दीर्घ size)
{
	जर (write_in_full(cat_blob_fd, buf, size) < 0)
		die_errno(_("write to frontend failed"));
}

स्थिर शून्य cat_blob(संरचना object_entry *oe, संरचना object_id *oid)
{
	संरचना strbuf ओळ = STRBUF_INIT;
	संरचना iovec iov[3];
	अचिन्हित दीर्घ size;
	गणना object_type type = 0;
	अक्षर *buf;

	जर (!oe || oe->pack_id == MAX_PACK_ID) {
		माप_प्रकार size_st = 0;
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
	जर (type <= 0) {
		strbuf_reset(&ओळ);
		strbuf_addf(&ओळ, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(ओळ.buf, ओळ.len);
		strbuf_release(&ओळ);
		मुक्त(buf);
		परत;
	}
	जर (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	जर (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_to_hex(oid), type_name(type));
	strbuf_reset(&ओळ);
	strbuf_addf(&ओळ, "%s %s %"PRIuMAX"\n", oid_to_hex(oid),
		    type_name(type), (uintmax_t)size);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_base = ओळ.buf;
	iov[0].iov_len = ओळ.len;
	iov[1].iov_base = buf;
	iov[1].iov_len = size;
	iov[2].iov_base = (शून्य *) "\n";
	iov[2].iov_len = 1;

	जर (writev_in_full(cat_blob_fd, iov, ARRAY_SIZE(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&ओळ);
	जर (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.data, buf, size, size + 1);
		last_blob.depth = oe->depth;
	} अन्यथा
		मुक्त(buf);
}

स्थिर शून्य parse_get_mark(संरचना fast_import_state *state UNUSED,
			   स्थिरांक अक्षर *p)
{
	संरचना object_entry *oe;
	अक्षर output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	जर (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	जर (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, आकार(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

स्थिर शून्य parse_cat_blob(संरचना fast_import_state *state UNUSED,
			   स्थिरांक अक्षर *p)
{
	संरचना object_entry *oe;
	संरचना object_id oid;

	/* cat-blob SP <object> LF */
	जर (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		जर (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा {
		जर (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		जर (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

स्थिर संरचना object_entry *dereference(संरचना object_entry *oe,
					संरचना object_id *oid)
{
	अचिन्हित दीर्घ size;
	अक्षर *buf = रिक्त;
	स्थिरांक अचिन्हित hexsz = the_hash_algo->hexsz;

	जर (!oe) {
		गणना object_type type = odb_read_object_info(the_repository->objects,
							     oid, रिक्त);
		जर (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = घाला_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	निवड (oe->type) {
	स्थिती OBJ_वृक्ष:	/* easy case. */
		परत oe;
	स्थिती OBJ_COMMIT:
	स्थिती OBJ_TAG:
		थांबा;
	पूर्वनिर्धारित:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	जर (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &size);
	} अन्यथा {
		गणना object_type unused;
		माप_प्रकार size_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &size_st);
		size = cast_size_t_to_ulong(size_st);
	}
	जर (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	निवड (oe->type) {
	स्थिती OBJ_TAG:
		जर (size < hexsz + म_माप("object ") ||
		    get_oid_hex(buf + म_माप("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		थांबा;
	स्थिती OBJ_COMMIT:
		जर (size < hexsz + म_माप("tree ") ||
		    get_oid_hex(buf + म_माप("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	मुक्त(buf);
	परत find_object(oid);
}

स्थिर शून्य घाला_mapped_mark(uintmax_t mark, शून्य *object, शून्य *cbp)
{
	संरचना object_id *fromoid = object;
	संरचना object_id *tooid = find_mark(cbp, mark);
	पूर्णांक ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	जर (ret == 0)
		परत;
	kh_मूल्य(sub_oid_map, it) = tooid;
}

स्थिर शून्य build_mark_map_one(संरचना mark_set *from, संरचना mark_set *to)
{
	for_each_mark(from, 0, घाला_mapped_mark, to);
}

स्थिर शून्य build_mark_map(संरचना string_list *from, संरचना string_list *to)
{
	संरचना string_list_item *fromp, *शीर्ष;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		शीर्ष = string_list_lookup(to, fromp->string);
		जर (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} अन्यथा जर (!शीर्ष || !शीर्ष->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, शीर्ष->util);
	}
}

स्थिर संरचना object_entry *parse_treeish_dataref(स्थिरांक अक्षर **p)
{
	संरचना object_id oid;
	संरचना object_entry *e;

	जर (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		जर (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} अन्यथा {	/* <sha1> */
		जर (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		जर (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	पर्यंत (!e || e->type != OBJ_वृक्ष)
		e = dereference(e, &oid);
	परत e;
}

स्थिर शून्य print_ls(पूर्णांक mode, स्थिरांक अचिन्हित अक्षर *hash, स्थिरांक अक्षर *path)
{
	स्थिर संरचना strbuf ओळ = STRBUF_INIT;

	/* See show_tree(). */
	स्थिरांक अक्षर *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? वृक्ष_type :
		blob_type;

	जर (!mode) {
		/* missing SP path LF */
		strbuf_reset(&ओळ);
		strbuf_addstr(&ओळ, "missing ");
		quote_c_style(path, &ओळ, रिक्त, 0);
		strbuf_addch(&ओळ, '\n');
	} अन्यथा {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&ओळ);
		strbuf_addf(&ओळ, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &ओळ, रिक्त, 0);
		strbuf_addch(&ओळ, '\n');
	}
	cat_blob_write(ओळ.buf, ओळ.len);
}

स्थिर शून्य parse_ls(संरचना fast_import_state *state UNUSED,
		     स्थिरांक अक्षर *p,
		     संरचना branch *b)
{
	स्थिर संरचना strbuf path = STRBUF_INIT;
	संरचना वृक्ष_entry *मूळ = रिक्त;
	संरचना वृक्ष_entry leaf = {रिक्त};

	/* ls SP (<tree-ish> SP)? <path> */
	जर (*p == '"') {
		जर (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		मूळ = &b->branch_वृक्ष;
	} अन्यथा {
		संरचना object_entry *e = parse_treeish_dataref(&p);
		मूळ = new_वृक्ष_entry();
		oidcpy(&मूळ->versions[1].oid, &e->idx.oid);
		जर (!is_null_oid(&मूळ->versions[1].oid))
			मूळ->versions[1].mode = S_IFDIR;
		load_वृक्ष(मूळ);
	}
	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	वृक्ष_content_get(मूळ, path.buf, &leaf, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	जर (S_ISDIR(leaf.versions[1].mode))
		store_वृक्ष(&leaf);

	print_ls(leaf.versions[1].mode, leaf.versions[1].oid.hash, path.buf);
	जर (leaf.वृक्ष)
		release_वृक्ष_content_recursive(leaf.वृक्ष);
	जर (!b || मूळ != &b->branch_वृक्ष)
		release_वृक्ष_entry(मूळ);
}

स्थिर शून्य checkpoint(शून्य)
{
	checkpoint_requested = 0;
	जर (object_मोजणी) {
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
	ख_डालो(command_buf.buf, 1, command_buf.len, मानक_निकास);
	ख_अक्षर_दो('\n', मानक_निकास);
	ख_साफ(मानक_निकास);
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
	जर (!next_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	स_रखो(&b, 0, आकार(b));
	जर (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	प्रतिज्ञा(e);
	घाला_mark(&marks, next_mark, e);
}

स्थिर अक्षर* make_fast_import_path(संरचना fast_import_state *state,
				   स्थिरांक अक्षर *path)
{
	जर (!relative_marks_paths || is_absolute_path(path))
		परत prefix_filename(state->prefix, path);
	परत repo_git_path(the_repository, "info/fast-import/%s", path);
}

स्थिर शून्य option_import_marks(संरचना fast_import_state *state,
				स्थिरांक अक्षर *marks,
				पूर्णांक from_stream,
				पूर्णांक ignore_missing)
{
	जर (import_marks_file) {
		जर (from_stream)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		जर(!import_marks_file_from_stream)
			read_marks();
	}

	मुक्त(import_marks_file);
	import_marks_file = make_fast_import_path(state, marks);
	import_marks_file_from_stream = from_stream;
	import_marks_file_ignore_missing = ignore_missing;
}

स्थिर शून्य option_date_format(स्थिरांक अक्षर *fmt)
{
	जर (!म_भेद(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	अन्यथा जर (!म_भेद(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	अन्यथा जर (!म_भेद(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	अन्यथा जर (!म_भेद(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	अन्यथा
		die(_("unknown --date-format argument %s"), fmt);
}

स्थिर अचिन्हित दीर्घ ulong_arg(स्थिरांक अक्षर *option, स्थिरांक अक्षर *arg)
{
	अक्षर *endptr;
	अचिन्हित दीर्घ rv = म_से_अदीर्घ(arg, &endptr, 0);
	जर (म_अक्षर(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	परत rv;
}

स्थिर शून्य option_depth(स्थिरांक अक्षर *depth)
{
	max_depth = ulong_arg("--depth", depth);
	जर (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

स्थिर शून्य option_active_branches(स्थिरांक अक्षर *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

स्थिर शून्य option_export_marks(संरचना fast_import_state *state,
				स्थिरांक अक्षर *marks)
{
	मुक्त(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

स्थिर शून्य option_cat_blob_fd(संरचना fast_import_state *state UNUSED,
			       स्थिरांक अक्षर *fd)
{
	अचिन्हित दीर्घ n = ulong_arg("--cat-blob-fd", fd);
	जर (n > (अचिन्हित दीर्घ) पूर्णांक_उच्च)
		die(_("--cat-blob-fd cannot exceed %d"), पूर्णांक_उच्च);
	cat_blob_fd = (पूर्णांक) n;
}

स्थिर शून्य option_export_pack_edges(संरचना fast_import_state *state,
				     स्थिरांक अक्षर *edges)
{
	अक्षर *fn = prefix_filename(state->prefix, edges);
	जर (pack_edges)
		ख_बंद(pack_edges);
	pack_edges = xfopen(fn, "a");
	मुक्त(fn);
}

स्थिर शून्य option_rewrite_submodules(संरचना fast_import_state *state,
				      स्थिरांक अक्षर *arg,
				      संरचना string_list *list)
{
	संरचना mark_set *ms;
	खाता *fp;
	अक्षर *s = xstrdup(arg);
	अक्षर *f = म_अक्षर(s, ':');
	जर (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = prefix_filename(state->prefix, f);
	fp = ख_खोलो(f, "r");
	जर (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, घाला_oid_entry);
	ख_बंद(fp);
	मुक्त(f);

	string_list_घाला(list, s)->util = ms;

	मुक्त(s);
}

स्थिर शून्य option_max_pack_size(स्थिरांक अक्षर *arg)
{
	अचिन्हित दीर्घ v;

	जर (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	जर (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} अन्यथा जर (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

स्थिर शून्य option_big_file_threshold(स्थिरांक अक्षर *arg)
{
	अचिन्हित दीर्घ v;

	जर (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

स्थिर शून्य option_signed_commits(स्थिरांक अक्षर *arg)
{
	जर (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

स्थिर शून्य option_signed_tags(स्थिरांक अक्षर *arg)
{
	जर (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

स्थिर शून्य option_quiet(शून्य)
{
	show_stats = 0;
	quiet = 1;
}

स्थिर पूर्णांक parse_one_option(संरचना fast_import_state *state, स्थिरांक अक्षर *option)
{
	जर (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_size(option);
	} अन्यथा जर (skip_prefix(option, "big-file-threshold=", &option)) {
		option_big_file_threshold(option);
	} अन्यथा जर (skip_prefix(option, "depth=", &option)) {
		option_depth(option);
	} अन्यथा जर (skip_prefix(option, "active-branches=", &option)) {
		option_active_branches(option);
	} अन्यथा जर (skip_prefix(option, "export-pack-edges=", &option)) {
		option_export_pack_edges(state, option);
	} अन्यथा जर (skip_prefix(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} अन्यथा जर (skip_prefix(option, "signed-tags=", &option)) {
		option_signed_tags(option);
	} अन्यथा जर (!म_भेद(option, "quiet")) {
		option_quiet();
	} अन्यथा जर (!म_भेद(option, "stats")) {
		show_stats = 1;
	} अन्यथा जर (!म_भेद(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} अन्यथा {
		परत 0;
	}

	परत 1;
}

स्थिर शून्य check_unsafe_feature(संरचना fast_import_state *state,
				 स्थिरांक अक्षर *feature)
{
	जर (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

स्थिर पूर्णांक parse_one_feature(संरचना fast_import_state *state,
			     स्थिरांक अक्षर *feature)
{
	स्थिरांक अक्षर *arg;

	जर (skip_prefix(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} अन्यथा जर (skip_prefix(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} अन्यथा जर (skip_prefix(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} अन्यथा जर (skip_prefix(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} अन्यथा जर (!म_भेद(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} अन्यथा जर (skip_prefix(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_to);
	} अन्यथा जर (skip_prefix(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} अन्यथा जर (!म_भेद(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} अन्यथा जर (!म_भेद(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} अन्यथा जर (!म_भेद(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} अन्यथा जर (!म_भेद(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} अन्यथा जर (!म_भेद(feature, "done")) {
		require_explicit_termination = 1;
	} अन्यथा जर (!म_भेद(feature, "force")) {
		force_update = 1;
	} अन्यथा जर (!म_भेद(feature, "notes") || !म_भेद(feature, "ls")) {
		; /* do nothing; we have the feature */
	} अन्यथा {
		परत 0;
	}

	परत 1;
}

स्थिर शून्य parse_feature(संरचना fast_import_state *state, स्थिरांक अक्षर *feature)
{
	जर (state->seen_data_command)
		die(_("got feature command '%s' after data command"), feature);

	जर (parse_one_feature(state, feature))
		परत;

	die(_("this version of fast-import does not support feature %s."), feature);
}

स्थिर शून्य parse_option(संरचना fast_import_state *state, स्थिरांक अक्षर *option)
{
	जर (state->seen_data_command)
		die(_("got option command '%s' after data command"), option);

	जर (parse_one_option(state, option))
		परत;

	die(_("this version of fast-import does not support option: %s"), option);
}

स्थिर शून्य git_pack_config(शून्य)
{
	पूर्णांक indexversion_मूल्य;
	पूर्णांक limit;
	अचिन्हित दीर्घ packsizelimit_मूल्य;

	जर (!repo_config_get_ulong(the_repository, "pack.depth", &max_depth)) {
		जर (max_depth > MAX_DEPTH)
			max_depth = MAX_DEPTH;
	}
	जर (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_मूल्य)) {
		pack_idx_opts.version = indexversion_मूल्य;
		जर (pack_idx_opts.version > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.version);
	}
	जर (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_मूल्य))
		max_packsize = packsizelimit_मूल्य;

	जर (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &limit))
		unpack_limit = limit;
	अन्यथा जर (!repo_config_get_int(the_repository, "transfer.unpacklimit", &limit))
		unpack_limit = limit;

	repo_config(the_repository, git_default_config, रिक्त);
}

स्थिर स्थिरांक अक्षर *स्थिरांक fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	रिक्त
};

स्थिर शून्य parse_argv(संरचना fast_import_state *state)
{
	पूर्णांक argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	जर (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_data_command = 1;
	जर (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

स्थिर पूर्णांक option_parse_date_format(स्थिरांक संरचना option *opt UNUSED,
				    स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	परत 0;
}

स्थिर पूर्णांक option_parse_export_pack_edges(स्थिरांक संरचना option *opt,
					  स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->मूल्य, arg);
	परत 0;
}

स्थिर पूर्णांक option_parse_max_pack_size(स्थिरांक संरचना option *opt UNUSED,
				      स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_size(arg);
	परत 0;
}

स्थिर पूर्णांक option_parse_big_file_threshold(स्थिरांक संरचना option *opt UNUSED,
					   स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	परत 0;
}

स्थिर पूर्णांक option_parse_signed_commits(स्थिरांक संरचना option *opt UNUSED,
				       स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	परत 0;
}

स्थिर पूर्णांक option_parse_signed_tags(स्थिरांक संरचना option *opt UNUSED,
				    स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	परत 0;
}

स्थिर पूर्णांक option_parse_rewrite_submodules_from(स्थिरांक संरचना option *opt,
						स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->मूल्य, arg, &sub_marks_from);
	परत 0;
}

स्थिर पूर्णांक option_parse_rewrite_submodules_to(स्थिरांक संरचना option *opt,
					      स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->मूल्य, arg, &sub_marks_to);
	परत 0;
}

स्थिर पूर्णांक option_parse_cat_blob_fd(स्थिरांक संरचना option *opt,
				    स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->मूल्य, arg);
	परत 0;
}

स्थिर पूर्णांक option_parse_import_marks(स्थिरांक संरचना option *opt,
				     स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->मूल्य, arg, 0, 0);
	परत 0;
}

स्थिर पूर्णांक option_parse_import_marks_if_exists(स्थिरांक संरचना option *opt,
					       स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->मूल्य, arg, 0, 1);
	परत 0;
}

स्थिर पूर्णांक option_parse_export_marks(स्थिरांक संरचना option *opt,
				     स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->मूल्य, arg);
	परत 0;
}

स्थिर पूर्णांक option_parse_depth(स्थिरांक संरचना option *opt UNUSED,
			      स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	परत 0;
}

स्थिर पूर्णांक option_parse_active_branches(स्थिरांक संरचना option *opt UNUSED,
					स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	परत 0;
}

स्थिर पूर्णांक option_parse_quiet(स्थिरांक संरचना option *opt UNUSED,
			      स्थिरांक अक्षर *arg UNUSED, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	परत 0;
}

पूर्णांक cmd_fast_import(पूर्णांक argc,
		    स्थिरांक अक्षर **argv,
		    स्थिरांक अक्षर *prefix,
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
	marks = mem_pool_calloc(&fi_mem_pool, 1, आकार(संरचना mark_set));

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
	साठी (पूर्णांक i = 1; i < argc; i++) {
		स्थिरांक अक्षर *arg = argv[i];
		जर (*arg != '-' || !म_भेद(arg, "--"))
			थांबा;
		जर (!म_भेद(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * आकार(*rc_free));
	साठी (अचिन्हित पूर्णांक i = 0; i < (cmd_save - 1); i++)
		rc_free[i].next = &rc_free[i + 1];
	rc_free[cmd_save - 1].next = रिक्त;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	पर्यंत (read_next_command(&state) != खातापूर्ण) {
		स्थिरांक अक्षर *v;
		जर (!म_भेद("blob", command_buf.buf))
			parse_new_blob(&state);
		अन्यथा जर (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		अन्यथा जर (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		अन्यथा जर (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		अन्यथा जर (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, रिक्त);
		अन्यथा जर (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		अन्यथा जर (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		अन्यथा जर (!म_भेद("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		अन्यथा जर (!म_भेद("done", command_buf.buf))
			थांबा;
		अन्यथा जर (!म_भेद("alias", command_buf.buf))
			parse_alias(&state);
		अन्यथा जर (starts_with(command_buf.buf, "progress "))
			parse_progress(&state);
		अन्यथा जर (skip_prefix(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		अन्यथा जर (skip_prefix(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		अन्यथा जर (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		अन्यथा
			die(_("unsupported command: %s"), command_buf.buf);

		जर (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	जर (!state.seen_data_command)
		parse_argv(&state);

	जर (require_explicit_termination && ख_पूर्ण(मानक_निवेश))
		die(_("stream ends early"));

	end_packfile();

	dump_branches();
	dump_tags();
	unkeep_all_packs();
	dump_marks();

	जर (pack_edges)
		ख_बंद(pack_edges);

	जर (show_stats) {
		uintmax_t total_मोजणी = 0, duplicate_मोजणी = 0;
		साठी (माप_प्रकार i = 0; i < ARRAY_SIZE(object_मोजणी_by_type); i++)
			total_मोजणी += object_मोजणी_by_type[i];
		साठी (माप_प्रकार i = 0; i < ARRAY_SIZE(duplicate_मोजणी_by_type); i++)
			duplicate_मोजणी += duplicate_मोजणी_by_type[i];

		ख_लिखो(मानक_त्रुटि, "%s statistics:\n", argv[0]);
		ख_लिखो(मानक_त्रुटि, "---------------------------------------------------------------------\n");
		ख_लिखो(मानक_त्रुटि, "Alloc'd objects: %10" PRIuMAX "\n", alloc_मोजणी);
		ख_लिखो(मानक_त्रुटि, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", total_मोजणी, duplicate_मोजणी);
		ख_लिखो(मानक_त्रुटि, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_मोजणी_by_type[OBJ_BLOB], duplicate_मोजणी_by_type[OBJ_BLOB], delta_मोजणी_by_type[OBJ_BLOB], delta_मोजणी_attempts_by_type[OBJ_BLOB]);
		ख_लिखो(मानक_त्रुटि, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_मोजणी_by_type[OBJ_वृक्ष], duplicate_मोजणी_by_type[OBJ_वृक्ष], delta_मोजणी_by_type[OBJ_वृक्ष], delta_मोजणी_attempts_by_type[OBJ_वृक्ष]);
		ख_लिखो(मानक_त्रुटि, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_मोजणी_by_type[OBJ_COMMIT], duplicate_मोजणी_by_type[OBJ_COMMIT], delta_मोजणी_by_type[OBJ_COMMIT], delta_मोजणी_attempts_by_type[OBJ_COMMIT]);
		ख_लिखो(मानक_त्रुटि, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_मोजणी_by_type[OBJ_TAG], duplicate_मोजणी_by_type[OBJ_TAG], delta_मोजणी_by_type[OBJ_TAG], delta_मोजणी_attempts_by_type[OBJ_TAG]);
		ख_लिखो(मानक_त्रुटि, "Total branches:  %10lu (%10lu loads     )\n", branch_मोजणी, branch_load_मोजणी);
		ख_लिखो(मानक_त्रुटि, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->shift) * 1024, marks_set_मोजणी);
		ख_लिखो(मानक_त्रुटि, "      atoms:     %10u\n", atom_cnt);
		ख_लिखो(मानक_त्रुटि, "Memory total:    %10" PRIuMAX " KiB\n", (वृक्ष_entry_allocd + fi_mem_pool.pool_alloc + alloc_मोजणी*आकार(संरचना object_entry))/1024);
		ख_लिखो(मानक_त्रुटि, "       pools:    %10lu KiB\n", (अचिन्हित दीर्घ)((वृक्ष_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		ख_लिखो(मानक_त्रुटि, "     objects:    %10" PRIuMAX " KiB\n", (alloc_मोजणी*आकार(संरचना object_entry))/1024);
		ख_लिखो(मानक_त्रुटि, "---------------------------------------------------------------------\n");
		pack_report(repo);
		ख_लिखो(मानक_त्रुटि, "---------------------------------------------------------------------\n");
		ख_लिखो(मानक_त्रुटि, "\n");
	}

	परत failure ? 1 : 0;
}
