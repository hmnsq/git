#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "config.h"
#समावेश "lockfile.h"
#समावेश "object.h"
#समावेश "blob.h"
#समावेश "tree.h"
#समावेश "commit.h"
#समावेश "delta.h"
#समावेश "pack.h"
#समावेश "path.h"
#समावेश "read-cache-ll.h"
#समावेश "refs.h"
#समावेश "csum-file.h"
#समावेश "quote.h"
#समावेश "dir.h"
#समावेश "run-command.h"
#समावेश "packfile.h"
#समावेश "object-file.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "mem-pool.h"
#समावेश "commit-reach.h"
#समावेश "khash.h"
#समावेश "date.h"
#समावेश "gpg-interface.h"
#समावेश "parse-options.h"

#घोषणा PACK_ID_BITS 16
#घोषणा MAX_PACK_ID ((1<<PACK_ID_BITS)-1)
#घोषणा DEPTH_BITS 13
#घोषणा MAX_DEPTH ((1<<DEPTH_BITS)-1)

/*
 * We abuse the setuid bit on directories to mean "do not delta".
 */
#घोषणा NO_DELTA S_ISUID

/*
 * The amount of additional space required in order to write an object into the
 * current pack. This is the hash lengths at the end of the pack, plus the
 * length of one object ID.
 */
#घोषणा PACK_आकार_THRESHOLD (the_hash_algo->rawsz * 3)

काष्ठा object_entry {
	काष्ठा pack_idx_entry idx;
	काष्ठा hashmap_entry ent;
	uint32_t type : TYPE_BITS,
		pack_id : PACK_ID_BITS,
		depth : DEPTH_BITS;
};

अटल पूर्णांक object_entry_hashcmp(स्थिर व्योम *map_तथ्य UNUSED,
				स्थिर काष्ठा hashmap_entry *eptr,
				स्थिर काष्ठा hashmap_entry *entry_or_कुंजी,
				स्थिर व्योम *keydata)
{
	स्थिर काष्ठा object_id *oid = keydata;
	स्थिर काष्ठा object_entry *e1, *e2;

	e1 = container_of(eptr, स्थिर काष्ठा object_entry, ent);
	अगर (oid)
		वापस oidcmp(&e1->idx.oid, oid);

	e2 = container_of(entry_or_कुंजी, स्थिर काष्ठा object_entry, ent);
	वापस oidcmp(&e1->idx.oid, &e2->idx.oid);
}

काष्ठा object_entry_pool {
	काष्ठा object_entry_pool *अग्र_pool;
	काष्ठा object_entry *अग्र_free;
	काष्ठा object_entry *end;
	काष्ठा object_entry entries[FLEX_ARRAY]; /* more */
};

काष्ठा mark_set {
	जोड़ {
		काष्ठा object_id *oids[1024];
		काष्ठा object_entry *marked[1024];
		काष्ठा mark_set *sets[1024];
	} तथ्य;
	अचिन्हित पूर्णांक shift;
};

काष्ठा last_object {
	काष्ठा strbuf तथ्य;
	off_t offset;
	अचिन्हित पूर्णांक depth;
	अचिन्हित no_swap : 1;
};

काष्ठा atom_str {
	काष्ठा atom_str *अग्र_atom;
	अचिन्हित लघु str_len;
	अक्षर str_dat[FLEX_ARRAY]; /* more */
};

काष्ठा वृक्ष_content;
काष्ठा वृक्ष_entry {
	काष्ठा वृक्ष_content *वृक्ष;
	काष्ठा atom_str *name;
	काष्ठा वृक्ष_entry_ms {
		uint16_t mode;
		काष्ठा object_id oid;
	} versions[2];
};

काष्ठा वृक्ष_content {
	अचिन्हित पूर्णांक entry_capacity; /* must match avail_tree_content */
	अचिन्हित पूर्णांक entry_गिनती;
	अचिन्हित पूर्णांक delta_depth;
	काष्ठा वृक्ष_entry *entries[FLEX_ARRAY]; /* more */
};

काष्ठा avail_वृक्ष_content {
	अचिन्हित पूर्णांक entry_capacity; /* must match tree_content */
	काष्ठा avail_वृक्ष_content *अग्र_avail;
};

काष्ठा branch {
	काष्ठा branch *table_अग्र_branch;
	काष्ठा branch *active_अग्र_branch;
	स्थिर अक्षर *name;
	काष्ठा वृक्ष_entry branch_वृक्ष;
	uintmax_t last_commit;
	uintmax_t अंक_notes;
	अचिन्हित active : 1;
	अचिन्हित delete : 1;
	अचिन्हित pack_id : PACK_ID_BITS;
	काष्ठा object_id oid;
};

काष्ठा tag {
	काष्ठा tag *अग्र_tag;
	स्थिर अक्षर *name;
	अचिन्हित पूर्णांक pack_id;
	काष्ठा object_id oid;
};

काष्ठा hash_list {
	काष्ठा hash_list *अग्र;
	काष्ठा object_id oid;
};

प्रकार क्रमागत {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

काष्ठा recent_command {
	काष्ठा recent_command *पूर्व;
	काष्ठा recent_command *अग्र;
	अक्षर *buf;
};

प्रकार व्योम (*mark_set_inserter_t)(काष्ठा mark_set **s, काष्ठा object_id *oid, uintmax_t mark);
प्रकार व्योम (*each_mark_fn_t)(uintmax_t mark, व्योम *obj, व्योम *cbp);

/* Configured limits on output */
अटल अचिन्हित दीर्घ max_depth = 50;
अटल off_t max_packsize;
अटल पूर्णांक unpack_limit = 100;
अटल पूर्णांक force_update;

/* Stats and misc. counters */
अटल uintmax_t alloc_गिनती;
अटल uintmax_t marks_set_गिनती;
अटल uintmax_t object_गिनती_by_type[1 << TYPE_BITS];
अटल uintmax_t duplicate_गिनती_by_type[1 << TYPE_BITS];
अटल uintmax_t delta_गिनती_by_type[1 << TYPE_BITS];
अटल uintmax_t delta_गिनती_attempts_by_type[1 << TYPE_BITS];
अटल अचिन्हित दीर्घ object_गिनती;
अटल अचिन्हित दीर्घ branch_गिनती;
अटल अचिन्हित दीर्घ branch_load_गिनती;
अटल पूर्णांक failure;
अटल खाता *pack_edges;
अटल पूर्णांक show_stats = 1;
अटल पूर्णांक quiet;
अटल क्रमागत sign_mode signed_tag_mode = SIGN_VERBATIM;
अटल क्रमागत sign_mode signed_commit_mode = SIGN_VERBATIM;
अटल स्थिर अक्षर *signed_commit_keyid;
अटल स्थिर अक्षर *signed_tag_keyid;

/* Memory pools */
अटल काष्ठा mem_pool fi_mem_pool = {
	.block_alloc = 2*1024*1024 - माप(काष्ठा mp_block),
};

/* Atom management */
अटल अचिन्हित पूर्णांक atom_table_sz = 4451;
अटल अचिन्हित पूर्णांक atom_cnt;
अटल काष्ठा atom_str **atom_table;

/* The .pack file being generated */
अटल काष्ठा pack_idx_option pack_idx_opts;
अटल अचिन्हित पूर्णांक pack_id;
अटल काष्ठा hashfile *pack_file;
अटल काष्ठा packed_git *pack_तथ्य;
अटल काष्ठा packed_git **all_packs;
अटल off_t pack_आकार;

/* Table of objects we've written. */
अटल अचिन्हित पूर्णांक object_entry_alloc = 5000;
अटल काष्ठा object_entry_pool *blocks;
अटल काष्ठा hashmap object_table;
अटल काष्ठा mark_set *marks;
अटल अक्षर *export_marks_file;
अटल अक्षर *import_marks_file;
अटल पूर्णांक import_marks_file_from_stream;
अटल पूर्णांक import_marks_file_ignore_missing;
अटल पूर्णांक import_marks_file_done;
अटल पूर्णांक relative_marks_paths;

/* Our last blob */
अटल काष्ठा last_object last_blob = {
	.तथ्य = STRBUF_INIT,
 };

/* Tree management */
अटल अचिन्हित पूर्णांक वृक्ष_entry_alloc = 1000;
अटल व्योम *avail_वृक्ष_entry;
अटल अचिन्हित पूर्णांक avail_वृक्ष_table_sz = 100;
अटल काष्ठा avail_वृक्ष_content **avail_वृक्ष_table;
अटल माप_प्रकार वृक्ष_entry_allocd;
अटल काष्ठा strbuf old_वृक्ष = STRBUF_INIT;
अटल काष्ठा strbuf new_वृक्ष = STRBUF_INIT;

/* Branch data */
अटल अचिन्हित दीर्घ max_active_branches = 5;
अटल अचिन्हित दीर्घ cur_active_branches;
अटल अचिन्हित दीर्घ branch_table_sz = 1039;
अटल काष्ठा branch **branch_table;
अटल काष्ठा branch *active_branches;

/* Tag data */
अटल काष्ठा tag *first_tag;
अटल काष्ठा tag *last_tag;

/* Input stream parsing */
अटल whenspec_type whenspec = WHENSPEC_RAW;
अटल काष्ठा strbuf command_buf = STRBUF_INIT;
अटल पूर्णांक unread_command_buf;
अटल काष्ठा recent_command cmd_hist = {
	.पूर्व = &cmd_hist,
	.अग्र = &cmd_hist,
};
अटल काष्ठा recent_command *cmd_पुच्छ = &cmd_hist;
अटल काष्ठा recent_command *rc_free;
अटल अचिन्हित पूर्णांक cmd_save = 100;
अटल uintmax_t अग्र_mark;
अटल काष्ठा strbuf new_तथ्य = STRBUF_INIT;
अटल पूर्णांक require_explicit_termination;

/* Signal handling */
अटल अस्थिर संक_पूर्ण_प्रकार checkpoint_requested;

/* Submodule marks */
अटल काष्ठा string_list sub_marks_from = STRING_LIST_INIT_DUP;
अटल काष्ठा string_list sub_marks_to = STRING_LIST_INIT_DUP;
अटल kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
अटल पूर्णांक cat_blob_fd = STDOUT_FILENO;

/* Command state */
काष्ठा fast_import_state {
	पूर्णांक argc;
	स्थिर अक्षर **argv;
	स्थिर अक्षर *prefix;
	पूर्णांक seen_तथ्य_command;
	पूर्णांक allow_unsafe_features;
	काष्ठा option *option;
};

अटल व्योम fast_import_state_init(काष्ठा fast_import_state *state,
				   पूर्णांक argc, स्थिर अक्षर **argv,
				   स्थिर अक्षर *prefix, काष्ठा option *option)
{
	स_रखो(state, 0, माप(*state));
	state->argc = argc;
	state->argv = argv;
	state->prefix = prefix;
	state->option = option;
}

अटल व्योम parse_argv(काष्ठा fast_import_state *state);
अटल व्योम parse_get_mark(काष्ठा fast_import_state *state, स्थिर अक्षर *p);
अटल व्योम parse_cat_blob(काष्ठा fast_import_state *state, स्थिर अक्षर *p);
अटल व्योम parse_ls(काष्ठा fast_import_state *state,
		     स्थिर अक्षर *p,
		     काष्ठा branch *b);

अटल व्योम for_each_mark(काष्ठा mark_set *m, uintmax_t base, each_mark_fn_t callback, व्योम *p)
{
	uintmax_t k;
	अगर (m->shift) {
		क्रम (k = 0; k < 1024; k++) {
			अगर (m->तथ्य.sets[k])
				for_each_mark(m->तथ्य.sets[k], base + (k << m->shift), callback, p);
		}
	} अन्यथा {
		क्रम (k = 0; k < 1024; k++) {
			अगर (m->तथ्य.marked[k])
				callback(base + k, m->तथ्य.marked[k], p);
		}
	}
}

अटल व्योम dump_marks_fn(uintmax_t mark, व्योम *object, व्योम *cbp) {
	काष्ठा object_entry *e = object;
	खाता *f = cbp;

	ख_लिखो(f, ":%" PRIuMAX " %s\n", mark, oid_to_hex(&e->idx.oid));
}

अटल व्योम write_branch_report(खाता *rpt, काष्ठा branch *b)
{
	ख_लिखो(rpt, "%s:\n", b->name);

	ख_लिखो(rpt, "  status      :");
	अगर (b->active)
		ख_माला_दो(" active", rpt);
	अगर (b->branch_वृक्ष.वृक्ष)
		ख_माला_दो(" loaded", rpt);
	अगर (is_null_oid(&b->branch_वृक्ष.versions[1].oid))
		ख_माला_दो(" dirty", rpt);
	ख_अक्षर_दो('\n', rpt);

	ख_लिखो(rpt, "  tip commit  : %s\n", oid_to_hex(&b->oid));
	ख_लिखो(rpt, "  old tree    : %s\n",
		oid_to_hex(&b->branch_वृक्ष.versions[0].oid));
	ख_लिखो(rpt, "  cur tree    : %s\n",
		oid_to_hex(&b->branch_वृक्ष.versions[1].oid));
	ख_लिखो(rpt, "  commit clock: %" PRIuMAX "\n", b->last_commit);

	ख_माला_दो("  last pack   : ", rpt);
	अगर (b->pack_id < MAX_PACK_ID)
		ख_लिखो(rpt, "%u", b->pack_id);
	ख_अक्षर_दो('\n', rpt);

	ख_अक्षर_दो('\n', rpt);
}

अटल व्योम write_crash_report(स्थिर अक्षर *err)
{
	अक्षर *loc = repo_git_path(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	खाता *rpt = ख_खोलो(loc, "w");
	काष्ठा branch *b;
	अचिन्हित दीर्घ lu;
	काष्ठा recent_command *rc;

	अगर (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		मुक्त(loc);
		वापस;
	}

	ख_लिखो(मानक_त्रुटि, _("fast-import: dumping crash report to %s\n"), loc);

	ख_लिखो(rpt, "fast-import crash report:\n");
	ख_लिखो(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	ख_लिखो(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	ख_लिखो(rpt, "    at %s\n", show_date(समय(शून्य), 0, DATE_MODE(ISO8601)));
	ख_अक्षर_दो('\n', rpt);

	ख_माला_दो("fatal: ", rpt);
	ख_माला_दो(err, rpt);
	ख_अक्षर_दो('\n', rpt);

	ख_अक्षर_दो('\n', rpt);
	ख_माला_दो("Most Recent Commands Before Crash\n", rpt);
	ख_माला_दो("---------------------------------\n", rpt);
	क्रम (rc = cmd_hist.अग्र; rc != &cmd_hist; rc = rc->अग्र) {
		अगर (rc->अग्र == &cmd_hist)
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
	क्रम (b = active_branches, lu = 0; b; b = b->active_अग्र_branch)
		ख_लिखो(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->last_commit, b->name);

	ख_अक्षर_दो('\n', rpt);
	ख_माला_दो("Inactive Branches\n", rpt);
	ख_माला_दो("-----------------\n", rpt);
	क्रम (lu = 0; lu < branch_table_sz; lu++) {
		क्रम (b = branch_table[lu]; b; b = b->table_अग्र_branch)
			write_branch_report(rpt, b);
	}

	अगर (first_tag) {
		काष्ठा tag *tg;
		ख_अक्षर_दो('\n', rpt);
		ख_माला_दो("Annotated Tags\n", rpt);
		ख_माला_दो("--------------\n", rpt);
		क्रम (tg = first_tag; tg; tg = tg->अग्र_tag) {
			ख_माला_दो(oid_to_hex(&tg->oid), rpt);
			ख_अक्षर_दो(' ', rpt);
			ख_माला_दो(tg->name, rpt);
			ख_अक्षर_दो('\n', rpt);
		}
	}

	ख_अक्षर_दो('\n', rpt);
	ख_माला_दो("Marks\n", rpt);
	ख_माला_दो("-----\n", rpt);
	अगर (export_marks_file)
		ख_लिखो(rpt, "  exported to %s\n", export_marks_file);
	अन्यथा
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	ख_अक्षर_दो('\n', rpt);
	ख_माला_दो("-------------------\n", rpt);
	ख_माला_दो("END OF CRASH REPORT\n", rpt);
	ख_बंद(rpt);
	मुक्त(loc);
}

अटल व्योम end_packfile(व्योम);
अटल व्योम unkeep_all_packs(व्योम);
अटल व्योम dump_marks(व्योम);

अटल NORETURN व्योम die_nicely(स्थिर अक्षर *err, बहु_सूची params)
{
	बहु_सूची cp;
	अटल पूर्णांक zombie;
	report_fn die_message_fn = get_die_message_routine();

	va_copy(cp, params);
	die_message_fn(err, params);

	अगर (!zombie) {
		अक्षर message[2 * PATH_MAX];

		zombie = 1;
		vsnprintf(message, माप(message), err, cp);
		write_crash_report(message);
		end_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	निकास(128);
}

#अगर_अघोषित SIGUSR1	/* Windows, for example */

अटल व्योम set_checkpoint_signal(व्योम)
{
}

#अन्यथा

अटल व्योम checkpoint_signal(पूर्णांक signo UNUSED)
{
	checkpoint_requested = 1;
}

अटल व्योम set_checkpoint_signal(व्योम)
{
	काष्ठा sigaction sa;

	स_रखो(&sa, 0, माप(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, शून्य);
}

#पूर्ण_अगर

अटल व्योम alloc_objects(अचिन्हित पूर्णांक cnt)
{
	काष्ठा object_entry_pool *b;

	b = xmalloc(माप(काष्ठा object_entry_pool)
		+ cnt * माप(काष्ठा object_entry));
	b->अग्र_pool = blocks;
	b->अग्र_free = b->entries;
	b->end = b->entries + cnt;
	blocks = b;
	alloc_गिनती += cnt;
}

अटल काष्ठा object_entry *new_object(काष्ठा object_id *oid)
{
	काष्ठा object_entry *e;

	अगर (blocks->अग्र_free == blocks->end)
		alloc_objects(object_entry_alloc);

	e = blocks->अग्र_free++;
	oidcpy(&e->idx.oid, oid);
	वापस e;
}

अटल काष्ठा object_entry *find_object(काष्ठा object_id *oid)
{
	वापस hashmap_get_entry_from_hash(&object_table, oidhash(oid), oid,
					   काष्ठा object_entry, ent);
}

अटल काष्ठा object_entry *जोड़ो_object(काष्ठा object_id *oid)
{
	काष्ठा object_entry *e;
	अचिन्हित पूर्णांक hash = oidhash(oid);

	e = hashmap_get_entry_from_hash(&object_table, hash, oid,
					काष्ठा object_entry, ent);
	अगर (!e) {
		e = new_object(oid);
		e->idx.offset = 0;
		hashmap_entry_init(&e->ent, hash);
		hashmap_add(&object_table, &e->ent);
	}

	वापस e;
}

अटल व्योम invalidate_pack_id(अचिन्हित पूर्णांक id)
{
	अचिन्हित दीर्घ lu;
	काष्ठा tag *t;
	काष्ठा hashmap_iter iter;
	काष्ठा object_entry *e;

	hashmap_for_each_entry(&object_table, &iter, e, ent) {
		अगर (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	क्रम (lu = 0; lu < branch_table_sz; lu++) {
		काष्ठा branch *b;

		क्रम (b = branch_table[lu]; b; b = b->table_अग्र_branch)
			अगर (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	क्रम (t = first_tag; t; t = t->अग्र_tag)
		अगर (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

अटल अचिन्हित पूर्णांक hc_str(स्थिर अक्षर *s, माप_प्रकार len)
{
	अचिन्हित पूर्णांक r = 0;
	जबतक (len-- > 0)
		r = r * 31 + *s++;
	वापस r;
}

अटल व्योम जोड़ो_mark(काष्ठा mark_set **शीर्ष_2, uintmax_t idnum, काष्ठा object_entry *oe)
{
	काष्ठा mark_set *s = *शीर्ष_2;

	जबतक ((idnum >> s->shift) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, माप(काष्ठा mark_set));
		s->shift = (*शीर्ष_2)->shift + 10;
		s->तथ्य.sets[0] = *शीर्ष_2;
		*शीर्ष_2 = s;
	}
	जबतक (s->shift) {
		uintmax_t i = idnum >> s->shift;
		idnum -= i << s->shift;
		अगर (!s->तथ्य.sets[i]) {
			s->तथ्य.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, माप(काष्ठा mark_set));
			s->तथ्य.sets[i]->shift = s->shift - 10;
		}
		s = s->तथ्य.sets[i];
	}
	अगर (!s->तथ्य.marked[idnum])
		marks_set_गिनती++;
	s->तथ्य.marked[idnum] = oe;
}

अटल व्योम *find_mark(काष्ठा mark_set *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	काष्ठा object_entry *oe = शून्य;
	अगर ((idnum >> s->shift) < 1024) {
		जबतक (s && s->shift) {
			uintmax_t i = idnum >> s->shift;
			idnum -= i << s->shift;
			s = s->तथ्य.sets[i];
		}
		अगर (s)
			oe = s->तथ्य.marked[idnum];
	}
	अगर (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	वापस oe;
}

अटल काष्ठा atom_str *to_atom(स्थिर अक्षर *s, अचिन्हित लघु len)
{
	अचिन्हित पूर्णांक hc = hc_str(s, len) % atom_table_sz;
	काष्ठा atom_str *c;

	क्रम (c = atom_table[hc]; c; c = c->अग्र_atom)
		अगर (c->str_len == len && !म_भेदन(s, c->str_dat, len))
			वापस c;

	c = mem_pool_alloc(&fi_mem_pool, माप(काष्ठा atom_str) + len + 1);
	c->str_len = len;
	स_नकल(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->अग्र_atom = atom_table[hc];
	atom_table[hc] = c;
	atom_cnt++;
	वापस c;
}

अटल काष्ठा branch *lookup_branch(स्थिर अक्षर *name)
{
	अचिन्हित पूर्णांक hc = hc_str(name, म_माप(name)) % branch_table_sz;
	काष्ठा branch *b;

	क्रम (b = branch_table[hc]; b; b = b->table_अग्र_branch)
		अगर (!म_भेद(name, b->name))
			वापस b;
	वापस शून्य;
}

अटल काष्ठा branch *new_branch(स्थिर अक्षर *name)
{
	अचिन्हित पूर्णांक hc = hc_str(name, म_माप(name)) % branch_table_sz;
	काष्ठा branch *b = lookup_branch(name);

	अगर (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	अगर (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, माप(काष्ठा branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->table_अग्र_branch = branch_table[hc];
	b->branch_वृक्ष.versions[0].mode = S_IFDIR;
	b->branch_वृक्ष.versions[1].mode = S_IFDIR;
	b->अंक_notes = 0;
	b->active = 0;
	b->pack_id = MAX_PACK_ID;
	branch_table[hc] = b;
	branch_गिनती++;
	वापस b;
}

अटल अचिन्हित पूर्णांक hc_entries(अचिन्हित पूर्णांक cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	वापस cnt < avail_वृक्ष_table_sz ? cnt : avail_वृक्ष_table_sz - 1;
}

अटल काष्ठा वृक्ष_content *new_वृक्ष_content(अचिन्हित पूर्णांक cnt)
{
	काष्ठा avail_वृक्ष_content *f, *l = शून्य;
	काष्ठा वृक्ष_content *t;
	अचिन्हित पूर्णांक hc = hc_entries(cnt);

	क्रम (f = avail_वृक्ष_table[hc]; f; l = f, f = f->अग्र_avail)
		अगर (f->entry_capacity >= cnt)
			अवरोध;

	अगर (f) {
		अगर (l)
			l->अग्र_avail = f->अग्र_avail;
		अन्यथा
			avail_वृक्ष_table[hc] = f->अग्र_avail;
	} अन्यथा {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, माप(*t) + माप(t->entries[0]) * cnt);
		f->entry_capacity = cnt;
	}

	t = (काष्ठा वृक्ष_content*)f;
	t->entry_गिनती = 0;
	t->delta_depth = 0;
	वापस t;
}

अटल व्योम release_वृक्ष_entry(काष्ठा वृक्ष_entry *e);
अटल व्योम release_वृक्ष_content(काष्ठा वृक्ष_content *t)
{
	काष्ठा avail_वृक्ष_content *f = (काष्ठा avail_वृक्ष_content*)t;
	अचिन्हित पूर्णांक hc = hc_entries(f->entry_capacity);
	f->अग्र_avail = avail_वृक्ष_table[hc];
	avail_वृक्ष_table[hc] = f;
}

अटल व्योम release_वृक्ष_content_recursive(काष्ठा वृक्ष_content *t)
{
	अचिन्हित पूर्णांक i;
	क्रम (i = 0; i < t->entry_गिनती; i++)
		release_वृक्ष_entry(t->entries[i]);
	release_वृक्ष_content(t);
}

अटल काष्ठा वृक्ष_content *grow_वृक्ष_content(
	काष्ठा वृक्ष_content *t,
	पूर्णांक amt)
{
	काष्ठा वृक्ष_content *r = new_वृक्ष_content(t->entry_गिनती + amt);
	r->entry_गिनती = t->entry_गिनती;
	r->delta_depth = t->delta_depth;
	COPY_ARRAY(r->entries, t->entries, t->entry_गिनती);
	release_वृक्ष_content(t);
	वापस r;
}

अटल काष्ठा वृक्ष_entry *new_वृक्ष_entry(व्योम)
{
	काष्ठा वृक्ष_entry *e;

	अगर (!avail_वृक्ष_entry) {
		अचिन्हित पूर्णांक n = वृक्ष_entry_alloc;
		वृक्ष_entry_allocd += n * माप(काष्ठा वृक्ष_entry);
		ALLOC_ARRAY(e, n);
		avail_वृक्ष_entry = e;
		जबतक (n-- > 1) {
			*((व्योम**)e) = e + 1;
			e++;
		}
		*((व्योम**)e) = शून्य;
	}

	e = avail_वृक्ष_entry;
	avail_वृक्ष_entry = *((व्योम**)e);
	वापस e;
}

अटल व्योम release_वृक्ष_entry(काष्ठा वृक्ष_entry *e)
{
	अगर (e->वृक्ष)
		release_वृक्ष_content_recursive(e->वृक्ष);
	*((व्योम**)e) = avail_वृक्ष_entry;
	avail_वृक्ष_entry = e;
}

अटल काष्ठा वृक्ष_content *dup_वृक्ष_content(काष्ठा वृक्ष_content *s)
{
	काष्ठा वृक्ष_content *d;
	काष्ठा वृक्ष_entry *a, *b;
	अचिन्हित पूर्णांक i;

	अगर (!s)
		वापस शून्य;
	d = new_वृक्ष_content(s->entry_गिनती);
	क्रम (i = 0; i < s->entry_गिनती; i++) {
		a = s->entries[i];
		b = new_वृक्ष_entry();
		स_नकल(b, a, माप(*a));
		अगर (a->वृक्ष && is_null_oid(&b->versions[1].oid))
			b->वृक्ष = dup_वृक्ष_content(a->वृक्ष);
		अन्यथा
			b->वृक्ष = शून्य;
		d->entries[i] = b;
	}
	d->entry_गिनती = s->entry_गिनती;
	d->delta_depth = s->delta_depth;

	वापस d;
}

अटल व्योम start_packfile(व्योम)
{
	काष्ठा strbuf tmp_file = STRBUF_INIT;
	काष्ठा packed_git *p;
	पूर्णांक pack_fd;

	pack_fd = odb_mkstemp(the_repository->objects, &tmp_file,
			      "pack/tmp_pack_XXXXXX");
	FLEX_ALLOC_STR(p, pack_name, tmp_file.buf);
	strbuf_release(&tmp_file);

	p->pack_fd = pack_fd;
	p->do_not_close = 1;
	p->repo = the_repository;
	pack_file = hashfd(the_repository->hash_algo, pack_fd, p->pack_name);

	pack_तथ्य = p;
	pack_आकार = write_pack_header(pack_file, 0);
	object_गिनती = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

अटल स्थिर अक्षर *create_सूचकांक(व्योम)
{
	स्थिर अक्षर *क्षणिक_ख;
	काष्ठा pack_idx_entry **idx, **c, **last;
	काष्ठा object_entry *e;
	काष्ठा object_entry_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_गिनती);
	c = idx;
	क्रम (o = blocks; o; o = o->अग्र_pool)
		क्रम (e = o->अग्र_free; e-- != o->entries;)
			अगर (pack_id == e->pack_id)
				*c++ = &e->idx;
	last = idx + object_गिनती;
	अगर (c != last)
		die(_("internal consistency error creating the index"));

	क्षणिक_ख = write_idx_file(the_repository, शून्य, idx, object_गिनती,
				 &pack_idx_opts, pack_तथ्य->hash);
	मुक्त(idx);
	वापस क्षणिक_ख;
}

अटल अक्षर *keep_pack(स्थिर अक्षर *curr_सूचकांक_name)
{
	अटल स्थिर अक्षर *keep_संदेश = "fast-import";
	काष्ठा strbuf name = STRBUF_INIT;
	पूर्णांक keep_fd;

	odb_pack_name(pack_तथ्य->repo, &name, pack_तथ्य->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_तथ्य->repo,
							    name.buf);
	अगर (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_संदेश, म_माप(keep_संदेश));
	अगर (बंद(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_तथ्य->repo, &name, pack_तथ्य->hash, "pack");
	अगर (finalize_object_file(pack_तथ्य->repo, pack_तथ्य->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_तथ्य->repo, &name, pack_तथ्य->hash, "idx");
	अगर (finalize_object_file(pack_तथ्य->repo, curr_सूचकांक_name, name.buf))
		die(_("cannot store index file"));
	मुक्त((व्योम *)curr_सूचकांक_name);
	वापस strbuf_detach(&name, शून्य);
}

अटल व्योम unkeep_all_packs(व्योम)
{
	काष्ठा strbuf name = STRBUF_INIT;
	पूर्णांक k;

	क्रम (k = 0; k < pack_id; k++) {
		काष्ठा packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

अटल पूर्णांक loosen_small_pack(स्थिर काष्ठा packed_git *p)
{
	काष्ठा child_process unpack = CHILD_PROCESS_INIT;

	अगर (lseek(p->pack_fd, 0, शुरू_से) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.in = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_to_stderr = 1;
	strvec_धकेलो(&unpack.args, "unpack-objects");
	अगर (!show_stats)
		strvec_धकेलो(&unpack.args, "-q");

	वापस run_command(&unpack);
}

अटल व्योम end_packfile(व्योम)
{
	अटल पूर्णांक running;

	अगर (running || !pack_तथ्य)
		वापस;

	running = 1;
	clear_delta_base_cache();
	अगर (object_गिनती) {
		काष्ठा odb_source_files *files = odb_source_files_downcast(pack_तथ्य->repo->objects->source);
		काष्ठा packed_git *new_p;
		काष्ठा object_id cur_pack_oid;
		अक्षर *idx_name;
		पूर्णांक i;
		काष्ठा branch *b;
		काष्ठा tag *t;

		close_pack_windows(pack_तथ्य);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_header_footer(the_hash_algo, pack_तथ्य->pack_fd,
					 pack_तथ्य->hash, pack_तथ्य->pack_name,
					 object_गिनती, cur_pack_oid.hash,
					 pack_आकार);

		अगर (object_गिनती <= unpack_limit) {
			अगर (!loosen_small_pack(pack_तथ्य)) {
				invalidate_pack_id(pack_id);
				जाओ discard_pack;
			}
		}

		बंद(pack_तथ्य->pack_fd);
		idx_name = keep_pack(create_सूचकांक());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_load_pack(files->dirs->packed, idx_name, 1);
		अगर (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		मुक्त(idx_name);

		/* Print the boundary */
		अगर (pack_edges) {
			ख_लिखो(pack_edges, "%s:", new_p->pack_name);
			क्रम (i = 0; i < branch_table_sz; i++) {
				क्रम (b = branch_table[i]; b; b = b->table_अग्र_branch) {
					अगर (b->pack_id == pack_id)
						ख_लिखो(pack_edges, " %s",
							oid_to_hex(&b->oid));
				}
			}
			क्रम (t = first_tag; t; t = t->अग्र_tag) {
				अगर (t->pack_id == pack_id)
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
		बंद(pack_तथ्य->pack_fd);
		unlink_or_warn(pack_तथ्य->pack_name);
	}
	FREE_AND_NULL(pack_तथ्य);
	running = 0;

	/* We can't carry a delta across packfiles. */
	strbuf_release(&last_blob.तथ्य);
	last_blob.offset = 0;
	last_blob.depth = 0;
}

अटल व्योम cycle_packfile(व्योम)
{
	end_packfile();
	start_packfile();
}

अटल पूर्णांक store_object(
	क्रमागत object_type type,
	काष्ठा strbuf *dat,
	काष्ठा last_object *last,
	काष्ठा object_id *oidout,
	uintmax_t mark)
{
	काष्ठा odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	व्योम *out, *delta;
	काष्ठा object_entry *e;
	अचिन्हित अक्षर hdr[96];
	काष्ठा object_id oid;
	अचिन्हित दीर्घ hdrlen, deltalen = 0;
	काष्ठा git_hash_ctx c;
	git_zstream s;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	hdrlen = format_object_header((अक्षर *)hdr, माप(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	अगर (oidout)
		oidcpy(oidout, &oid);

	e = जोड़ो_object(&oid);
	अगर (mark)
		जोड़ो_mark(&marks, mark, e);
	अगर (e->idx.offset) {
		duplicate_गिनती_by_type[type]++;
		वापस 1;
	}

	क्रम (काष्ठा odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
		अगर (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			जारी;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_गिनती_by_type[type]++;
		वापस 1;
	}

	अगर (last && last->तथ्य.len && last->तथ्य.buf && last->depth < max_depth
		&& dat->len > the_hash_algo->rawsz) {
		माप_प्रकार deltalen_st;

		delta_गिनती_attempts_by_type[type]++;
		delta = diff_delta(last->तथ्य.buf, last->तथ्य.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_आकार_t_to_ulong(deltalen_st);
	} अन्यथा
		delta = शून्य;

	git_deflate_init(&s, cfg->pack_compression_level);
	अगर (delta) {
		s.अग्र_in = delta;
		s.avail_in = deltalen;
	} अन्यथा {
		s.अग्र_in = (व्योम *)dat->buf;
		s.avail_in = dat->len;
	}
	s.avail_out = git_deflate_bound(&s, s.avail_in);
	s.अग्र_out = out = xmalloc(s.avail_out);
	जबतक (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&s);

	/* Determine if we should auto-checkpoint. */
	अगर ((max_packsize
		&& (pack_आकार + PACK_आकार_THRESHOLD + s.योग_out) > max_packsize)
		|| (pack_आकार + PACK_आकार_THRESHOLD + s.योग_out) < pack_आकार) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		cycle_packfile();

		/* We cannot carry a delta into the new pack. */
		अगर (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.अग्र_in = (व्योम *)dat->buf;
			s.avail_in = dat->len;
			s.avail_out = git_deflate_bound(&s, s.avail_in);
			s.अग्र_out = out = xrealloc(out, s.avail_out);
			जबतक (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_end(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.offset = pack_आकार;
	object_गिनती++;
	object_गिनती_by_type[type]++;

	crc32_begin(pack_file);

	अगर (delta) {
		off_t ofs = e->idx.offset - last->offset;
		अचिन्हित pos = माप(hdr) - 1;

		delta_गिनती_by_type[type]++;
		e->depth = last->depth + 1;

		hdrlen = encode_in_pack_object_header(hdr, माप(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_आकार += hdrlen;

		hdr[pos] = ofs & 127;
		जबतक (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, माप(hdr) - pos);
		pack_आकार += माप(hdr) - pos;
	} अन्यथा {
		e->depth = 0;
		hdrlen = encode_in_pack_object_header(hdr, माप(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_आकार += hdrlen;
	}

	hashwrite(pack_file, out, s.योग_out);
	pack_आकार += s.योग_out;

	e->idx.crc32 = crc32_end(pack_file);

	मुक्त(out);
	मुक्त(delta);
	अगर (last) {
		अगर (last->no_swap) {
			last->तथ्य = *dat;
		} अन्यथा {
			strbuf_swap(&last->तथ्य, dat);
		}
		last->offset = e->idx.offset;
		last->depth = e->depth;
	}
	वापस 0;
}

अटल व्योम truncate_pack(काष्ठा hashfile_checkpoint *checkpoint)
{
	अगर (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_आकार = checkpoint->offset;
}

अटल व्योम stream_blob(uintmax_t len, काष्ठा object_id *oidout, uintmax_t mark)
{
	काष्ठा odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	माप_प्रकार in_sz = 64 * 1024, out_sz = 64 * 1024;
	अचिन्हित अक्षर *in_buf = xmalloc(in_sz);
	अचिन्हित अक्षर *out_buf = xmalloc(out_sz);
	काष्ठा object_entry *e;
	काष्ठा object_id oid;
	अचिन्हित दीर्घ hdrlen;
	off_t offset;
	काष्ठा git_hash_ctx c;
	git_zstream s;
	काष्ठा hashfile_checkpoint checkpoint;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	पूर्णांक status = Z_OK;

	/* Determine if we should auto-checkpoint. */
	अगर ((max_packsize
		&& (pack_आकार + PACK_आकार_THRESHOLD + len) > max_packsize)
		|| (pack_आकार + PACK_आकार_THRESHOLD + len) < pack_आकार)
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

	s.अग्र_out = out_buf + hdrlen;
	s.avail_out = out_sz - hdrlen;

	जबतक (status != Z_STREAM_END) {
		अगर (0 < len && !s.avail_in) {
			माप_प्रकार cnt = in_sz < len ? in_sz : (माप_प्रकार)len;
			माप_प्रकार n = ख_पढ़ो(in_buf, 1, cnt, मानक_निवेश);
			अगर (!n && ख_पूर्ण(मानक_निवेश))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, in_buf, n);
			s.अग्र_in = in_buf;
			s.avail_in = n;
			len -= n;
		}

		status = git_deflate(&s, len ? 0 : Z_FINISH);

		अगर (!s.avail_out || status == Z_STREAM_END) {
			माप_प्रकार n = s.अग्र_out - out_buf;
			hashwrite(pack_file, out_buf, n);
			pack_आकार += n;
			s.अग्र_out = out_buf;
			s.avail_out = out_sz;
		}

		चयन (status) {
		हाल Z_OK:
		हाल Z_BUF_ERROR:
		हाल Z_STREAM_END:
			जारी;
		शेष:
			die(_("unexpected deflate failure: %d"), status);
		}
	}
	git_deflate_end(&s);
	git_hash_final_oid(&oid, &c);

	अगर (oidout)
		oidcpy(oidout, &oid);

	e = जोड़ो_object(&oid);

	अगर (mark)
		जोड़ो_mark(&marks, mark, e);

	अगर (e->idx.offset) {
		duplicate_गिनती_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		जाओ out;
	}

	क्रम (काष्ठा odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
		अगर (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			जारी;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
		duplicate_गिनती_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		जाओ out;
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
अटल व्योम *gfi_unpack_entry(
	काष्ठा object_entry *oe,
	अचिन्हित दीर्घ *sizep)
{
	क्रमागत object_type type;
	माप_प्रकार आकार_st = 0;
	व्योम *तथ्य;
	काष्ठा packed_git *p = all_packs[oe->pack_id];
	अगर (p == pack_तथ्य && p->pack_आकार < (pack_आकार + the_hash_algo->rawsz)) {
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
		p->pack_आकार = pack_आकार + the_hash_algo->rawsz;
	}
	तथ्य = unpack_entry(the_repository, p, oe->idx.offset, &type, &आकार_st);
	अगर (sizep)
		*sizep = cast_आकार_t_to_ulong(आकार_st);
	वापस तथ्य;
}

अटल व्योम load_वृक्ष(काष्ठा वृक्ष_entry *मूल)
{
	काष्ठा object_id *oid = &मूल->versions[1].oid;
	काष्ठा object_entry *myoe;
	काष्ठा वृक्ष_content *t;
	अचिन्हित दीर्घ आकार;
	अक्षर *buf;
	स्थिर अक्षर *c;

	मूल->वृक्ष = t = new_वृक्ष_content(8);
	अगर (is_null_oid(oid))
		वापस;

	myoe = find_object(oid);
	अगर (myoe && myoe->pack_id != MAX_PACK_ID) {
		अगर (myoe->type != OBJ_वृक्ष)
			die(_("not a tree: %s"), oid_to_hex(oid));
		t->delta_depth = myoe->depth;
		buf = gfi_unpack_entry(myoe, &आकार);
		अगर (!buf)
			die(_("can't load tree %s"), oid_to_hex(oid));
	} अन्यथा {
		क्रमागत object_type type;
		माप_प्रकार आकार_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &आकार_st);
		आकार = cast_आकार_t_to_ulong(आकार_st);
		अगर (!buf || type != OBJ_वृक्ष)
			die(_("can't load tree %s"), oid_to_hex(oid));
	}

	c = buf;
	जबतक (c != (buf + आकार)) {
		काष्ठा वृक्ष_entry *e = new_वृक्ष_entry();

		अगर (t->entry_गिनती == t->entry_capacity)
			मूल->वृक्ष = t = grow_वृक्ष_content(t, t->entry_गिनती);
		t->entries[t->entry_गिनती++] = e;

		e->वृक्ष = शून्य;
		c = parse_mode(c, &e->versions[1].mode);
		अगर (!c)
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

अटल पूर्णांक tecmp0 (स्थिर व्योम *_a, स्थिर व्योम *_b)
{
	काष्ठा वृक्ष_entry *a = *((काष्ठा वृक्ष_entry**)_a);
	काष्ठा वृक्ष_entry *b = *((काष्ठा वृक्ष_entry**)_b);
	वापस base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].mode,
		b->name->str_dat, b->name->str_len, b->versions[0].mode);
}

अटल पूर्णांक tecmp1 (स्थिर व्योम *_a, स्थिर व्योम *_b)
{
	काष्ठा वृक्ष_entry *a = *((काष्ठा वृक्ष_entry**)_a);
	काष्ठा वृक्ष_entry *b = *((काष्ठा वृक्ष_entry**)_b);
	वापस base_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].mode,
		b->name->str_dat, b->name->str_len, b->versions[1].mode);
}

अटल व्योम mktree(काष्ठा वृक्ष_content *t, पूर्णांक v, काष्ठा strbuf *b)
{
	माप_प्रकार maxlen = 0;
	अचिन्हित पूर्णांक i;

	अगर (!v)
		QSORT(t->entries, t->entry_गिनती, tecmp0);
	अन्यथा
		QSORT(t->entries, t->entry_गिनती, tecmp1);

	क्रम (i = 0; i < t->entry_गिनती; i++) {
		अगर (t->entries[i]->versions[v].mode)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	क्रम (i = 0; i < t->entry_गिनती; i++) {
		काष्ठा वृक्ष_entry *e = t->entries[i];
		अगर (!e->versions[v].mode)
			जारी;
		strbuf_addf(b, "%o %s%c",
			(अचिन्हित पूर्णांक)(e->versions[v].mode & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_add(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

अटल व्योम store_वृक्ष(काष्ठा वृक्ष_entry *मूल)
{
	काष्ठा वृक्ष_content *t;
	अचिन्हित पूर्णांक i, j, del;
	काष्ठा last_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	काष्ठा object_entry *le = शून्य;

	अगर (!is_null_oid(&मूल->versions[1].oid))
		वापस;

	अगर (!मूल->वृक्ष)
		load_वृक्ष(मूल);
	t = मूल->वृक्ष;

	क्रम (i = 0; i < t->entry_गिनती; i++) {
		अगर (t->entries[i]->वृक्ष)
			store_वृक्ष(t->entries[i]);
	}

	अगर (!(मूल->versions[0].mode & NO_DELTA))
		le = find_object(&मूल->versions[0].oid);
	अगर (S_ISDIR(मूल->versions[0].mode) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_वृक्ष);
		lo.तथ्य = old_वृक्ष;
		lo.offset = le->idx.offset;
		lo.depth = t->delta_depth;
	}

	mktree(t, 1, &new_वृक्ष);
	store_object(OBJ_वृक्ष, &new_वृक्ष, &lo, &मूल->versions[1].oid, 0);

	t->delta_depth = lo.depth;
	क्रम (i = 0, j = 0, del = 0; i < t->entry_गिनती; i++) {
		काष्ठा वृक्ष_entry *e = t->entries[i];
		अगर (e->versions[1].mode) {
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

अटल व्योम वृक्ष_content_replace(
	काष्ठा वृक्ष_entry *मूल,
	स्थिर काष्ठा object_id *oid,
	स्थिर uint16_t mode,
	काष्ठा वृक्ष_content *newtree)
{
	अगर (!S_ISDIR(mode))
		die(_("root cannot be a non-directory"));
	oidclr(&मूल->versions[0].oid, the_repository->hash_algo);
	oidcpy(&मूल->versions[1].oid, oid);
	अगर (मूल->वृक्ष)
		release_वृक्ष_content_recursive(मूल->वृक्ष);
	मूल->वृक्ष = newtree;
}

अटल पूर्णांक वृक्ष_content_set(
	काष्ठा वृक्ष_entry *मूल,
	स्थिर अक्षर *p,
	स्थिर काष्ठा object_id *oid,
	स्थिर uint16_t mode,
	काष्ठा वृक्ष_content *subtree)
{
	काष्ठा वृक्ष_content *t;
	स्थिर अक्षर *slash1;
	अचिन्हित पूर्णांक i, n;
	काष्ठा वृक्ष_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	अगर (!n)
		die(_("empty path component found in input"));
	अगर (!*slash1 && !S_ISDIR(mode) && subtree)
		die(_("non-directories cannot have subtrees"));

	अगर (!मूल->वृक्ष)
		load_वृक्ष(मूल);
	t = मूल->वृक्ष;
	क्रम (i = 0; i < t->entry_गिनती; i++) {
		e = t->entries[i];
		अगर (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			अगर (!*slash1) {
				अगर (!S_ISDIR(mode)
						&& e->versions[1].mode == mode
						&& oideq(&e->versions[1].oid, oid))
					वापस 0;
				e->versions[1].mode = mode;
				oidcpy(&e->versions[1].oid, oid);
				अगर (e->वृक्ष)
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
				अगर (S_ISDIR(e->versions[0].mode))
					e->versions[0].mode |= NO_DELTA;

				oidclr(&मूल->versions[1].oid, the_repository->hash_algo);
				वापस 1;
			}
			अगर (!S_ISDIR(e->versions[1].mode)) {
				e->वृक्ष = new_वृक्ष_content(8);
				e->versions[1].mode = S_IFDIR;
			}
			अगर (!e->वृक्ष)
				load_वृक्ष(e);
			अगर (वृक्ष_content_set(e, slash1 + 1, oid, mode, subtree)) {
				oidclr(&मूल->versions[1].oid, the_repository->hash_algo);
				वापस 1;
			}
			वापस 0;
		}
	}

	अगर (t->entry_गिनती == t->entry_capacity)
		मूल->वृक्ष = t = grow_वृक्ष_content(t, t->entry_गिनती);
	e = new_वृक्ष_entry();
	e->name = to_atom(p, n);
	e->versions[0].mode = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->entry_गिनती++] = e;
	अगर (*slash1) {
		e->वृक्ष = new_वृक्ष_content(8);
		e->versions[1].mode = S_IFDIR;
		वृक्ष_content_set(e, slash1 + 1, oid, mode, subtree);
	} अन्यथा {
		e->वृक्ष = subtree;
		e->versions[1].mode = mode;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&मूल->versions[1].oid, the_repository->hash_algo);
	वापस 1;
}

अटल पूर्णांक वृक्ष_content_remove(
	काष्ठा वृक्ष_entry *मूल,
	स्थिर अक्षर *p,
	काष्ठा वृक्ष_entry *backup_leaf,
	पूर्णांक allow_मूल)
{
	काष्ठा वृक्ष_content *t;
	स्थिर अक्षर *slash1;
	अचिन्हित पूर्णांक i, n;
	काष्ठा वृक्ष_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	अगर (!मूल->वृक्ष)
		load_वृक्ष(मूल);

	अगर (!*p && allow_मूल) {
		e = मूल;
		जाओ del_entry;
	}

	t = मूल->वृक्ष;
	क्रम (i = 0; i < t->entry_गिनती; i++) {
		e = t->entries[i];
		अगर (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			अगर (*slash1 && !S_ISDIR(e->versions[1].mode))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				वापस 1;
			अगर (!*slash1 || !S_ISDIR(e->versions[1].mode))
				जाओ del_entry;
			अगर (!e->वृक्ष)
				load_वृक्ष(e);
			अगर (वृक्ष_content_remove(e, slash1 + 1, backup_leaf, 0)) {
				क्रम (n = 0; n < e->वृक्ष->entry_गिनती; n++) {
					अगर (e->वृक्ष->entries[n]->versions[1].mode) {
						oidclr(&मूल->versions[1].oid,
						       the_repository->hash_algo);
						वापस 1;
					}
				}
				backup_leaf = शून्य;
				जाओ del_entry;
			}
			वापस 0;
		}
	}
	वापस 0;

del_entry:
	अगर (backup_leaf)
		स_नकल(backup_leaf, e, माप(*backup_leaf));
	अन्यथा अगर (e->वृक्ष)
		release_वृक्ष_content_recursive(e->वृक्ष);
	e->वृक्ष = शून्य;
	e->versions[1].mode = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&मूल->versions[1].oid, the_repository->hash_algo);
	वापस 1;
}

अटल पूर्णांक वृक्ष_content_get(
	काष्ठा वृक्ष_entry *मूल,
	स्थिर अक्षर *p,
	काष्ठा वृक्ष_entry *leaf,
	पूर्णांक allow_मूल)
{
	काष्ठा वृक्ष_content *t;
	स्थिर अक्षर *slash1;
	अचिन्हित पूर्णांक i, n;
	काष्ठा वृक्ष_entry *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	अगर (!n && !allow_मूल)
		die(_("empty path component found in input"));

	अगर (!मूल->वृक्ष)
		load_वृक्ष(मूल);

	अगर (!n) {
		e = मूल;
		जाओ found_entry;
	}

	t = मूल->वृक्ष;
	क्रम (i = 0; i < t->entry_गिनती; i++) {
		e = t->entries[i];
		अगर (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			अगर (!*slash1)
				जाओ found_entry;
			अगर (!S_ISDIR(e->versions[1].mode))
				वापस 0;
			अगर (!e->वृक्ष)
				load_वृक्ष(e);
			वापस वृक्ष_content_get(e, slash1 + 1, leaf, 0);
		}
	}
	वापस 0;

found_entry:
	स_नकल(leaf, e, माप(*leaf));
	अगर (e->वृक्ष && is_null_oid(&e->versions[1].oid))
		leaf->वृक्ष = dup_वृक्ष_content(e->वृक्ष);
	अन्यथा
		leaf->वृक्ष = शून्य;
	वापस 1;
}

अटल पूर्णांक update_branch(काष्ठा branch *b)
{
	अटल स्थिर अक्षर *संदेश = "fast-import";
	काष्ठा ref_transaction *transaction;
	काष्ठा object_id old_oid;
	काष्ठा strbuf err = STRBUF_INIT;
	अटल स्थिर अक्षर *replace_prefix = "refs/replace/";

	अगर (starts_with(b->name, replace_prefix) &&
	    !म_भेद(b->name + म_माप(replace_prefix),
		    oid_to_hex(&b->oid))) {
		अगर (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_to_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				शून्य, b->name, शून्य, 0);
		वापस 0;
	}
	अगर (is_null_oid(&b->oid)) {
		अगर (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					शून्य, b->name, शून्य, 0);
		वापस 0;
	}
	अगर (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	अगर (!force_update && !is_null_oid(&old_oid)) {
		काष्ठा commit *old_cmit, *new_cmit;
		पूर्णांक ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		अगर (!old_cmit || !new_cmit)
			वापस error(_("branch %s is missing commits."), b->name);

		ret = repo_in_merge_bases(the_repository, old_cmit, new_cmit);
		अगर (ret < 0)
			निकास(128);
		अगर (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_to_hex(&b->oid),
				oid_to_hex(&old_oid));
			वापस -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	अगर (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   शून्य, शून्य, 0, संदेश, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		वापस -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	वापस 0;
}

अटल व्योम dump_branches(व्योम)
{
	अचिन्हित पूर्णांक i;
	काष्ठा branch *b;

	क्रम (i = 0; i < branch_table_sz; i++) {
		क्रम (b = branch_table[i]; b; b = b->table_अग्र_branch)
			failure |= update_branch(b);
	}
}

अटल व्योम dump_tags(व्योम)
{
	अटल स्थिर अक्षर *संदेश = "fast-import";
	काष्ठा tag *t;
	काष्ठा strbuf ref_name = STRBUF_INIT;
	काष्ठा strbuf err = STRBUF_INIT;
	काष्ठा ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	अगर (!transaction) {
		failure |= error("%s", err.buf);
		जाओ cleanup;
	}
	क्रम (t = first_tag; t; t = t->अग्र_tag) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		अगर (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, शून्य, शून्य, शून्य,
					   0, संदेश, &err)) {
			failure |= error("%s", err.buf);
			जाओ cleanup;
		}
	}
	अगर (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

अटल व्योम dump_marks(व्योम)
{
	काष्ठा lock_file mark_lock = LOCK_INIT;
	खाता *f;

	अगर (!export_marks_file || (import_marks_file && !import_marks_file_done))
		वापस;

	अगर (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		वापस;
	}

	अगर (hold_lock_file_for_update(&mark_lock, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		वापस;
	}

	f = fdopen_lock_file(&mark_lock, "w");
	अगर (!f) {
		पूर्णांक saved_errno = त्रुटि_सं;
		rollback_lock_file(&mark_lock);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, म_त्रुटि(saved_errno));
		वापस;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	अगर (commit_lock_file(&mark_lock)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		वापस;
	}
}

अटल व्योम जोड़ो_object_entry(काष्ठा mark_set **s, काष्ठा object_id *oid, uintmax_t mark)
{
	काष्ठा object_entry *e;
	e = find_object(oid);
	अगर (!e) {
		क्रमागत object_type type = odb_read_object_info(the_repository->objects,
							     oid, शून्य);
		अगर (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		e = जोड़ो_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.offset = 1; /* just not zero! */
	}
	जोड़ो_mark(s, mark, e);
}

अटल व्योम जोड़ो_oid_entry(काष्ठा mark_set **s, काष्ठा object_id *oid, uintmax_t mark)
{
	जोड़ो_mark(s, mark, xmemdupz(oid, माप(*oid)));
}

अटल व्योम read_mark_file(काष्ठा mark_set **s, खाता *f, mark_set_inserter_t inserter)
{
	अक्षर पंक्ति[512];
	जबतक (ख_माला_लो(पंक्ति, माप(पंक्ति), f)) {
		uintmax_t mark;
		अक्षर *end;
		काष्ठा object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		स_रखो(oid.hash, 0, माप(oid.hash));

		end = म_अक्षर(पंक्ति, '\n');
		अगर (पंक्ति[0] != ':' || !end)
			die(_("corrupt mark line: %s"), पंक्ति);
		*end = 0;
		mark = strtoumax(पंक्ति + 1, &end, 10);
		अगर (!mark || end == पंक्ति + 1
			|| *end != ' '
			|| get_oid_hex_any(end + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), पंक्ति);
		inserter(s, &oid, mark);
	}
}

अटल व्योम read_marks(व्योम)
{
	खाता *f = ख_खोलो(import_marks_file, "r");
	अगर (f)
		;
	अन्यथा अगर (import_marks_file_ignore_missing && त्रुटि_सं == ENOENT)
		जाओ done; /* Marks file does not exist */
	अन्यथा
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, जोड़ो_object_entry);
	ख_बंद(f);
done:
	import_marks_file_done = 1;
}


अटल पूर्णांक read_अग्र_command(काष्ठा fast_import_state *state)
{
	अटल पूर्णांक stdin_eof = 0;

	अगर (stdin_eof) {
		unread_command_buf = 0;
		वापस खातापूर्ण;
	}

	क्रम (;;) {
		अगर (unread_command_buf) {
			unread_command_buf = 0;
		} अन्यथा {
			काष्ठा recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, मानक_निवेश);
			अगर (stdin_eof)
				वापस खातापूर्ण;

			अगर (!state->seen_तथ्य_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			अगर (rc)
				rc_free = rc->अग्र;
			अन्यथा {
				rc = cmd_hist.अग्र;
				cmd_hist.अग्र = rc->अग्र;
				cmd_hist.अग्र->पूर्व = &cmd_hist;
				मुक्त(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->पूर्व = cmd_पुच्छ;
			rc->अग्र = cmd_hist.पूर्व;
			rc->पूर्व->अग्र = rc;
			cmd_पुच्छ = rc;
		}
		अगर (command_buf.buf[0] == '#')
			जारी;
		वापस 0;
	}
}

अटल व्योम skip_optional_lf(व्योम)
{
	पूर्णांक term_char = ख_अक्षर_लो(मानक_निवेश);
	अगर (term_char != '\n' && term_char != खातापूर्ण)
		अक्षर_वापस(term_char, मानक_निवेश);
}

अटल व्योम parse_mark(काष्ठा fast_import_state *state)
{
	स्थिर अक्षर *v;
	अगर (skip_prefix(command_buf.buf, "mark :", &v)) {
		अग्र_mark = strtoumax(v, शून्य, 10);
		read_अग्र_command(state);
	}
	अन्यथा
		अग्र_mark = 0;
}

अटल व्योम parse_original_identifier(काष्ठा fast_import_state *state)
{
	स्थिर अक्षर *v;
	अगर (skip_prefix(command_buf.buf, "original-oid ", &v))
		read_अग्र_command(state);
}

अटल पूर्णांक parse_तथ्य(काष्ठा strbuf *sb, uintmax_t limit, uintmax_t *len_res)
{
	स्थिर अक्षर *तथ्य;
	strbuf_reset(sb);

	अगर (!skip_prefix(command_buf.buf, "data ", &तथ्य))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	अगर (skip_prefix(तथ्य, "<<", &तथ्य)) {
		अक्षर *term = xstrdup(तथ्य);
		माप_प्रकार term_len = command_buf.len - (तथ्य - command_buf.buf);

		क्रम (;;) {
			अगर (strbuf_getline_lf(&command_buf, मानक_निवेश) == खातापूर्ण)
				die(_("EOF in data (terminator '%s' not found)"), term);
			अगर (term_len == command_buf.len
				&& !म_भेद(term, command_buf.buf))
				अवरोध;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		मुक्त(term);
	}
	अन्यथा {
		uintmax_t len = strtoumax(तथ्य, शून्य, 10);
		माप_प्रकार n = 0, लंबाई = (माप_प्रकार)len;

		अगर (limit && limit < len) {
			*len_res = len;
			वापस 0;
		}
		अगर (लंबाई < len)
			die(_("data is too large to use in this context"));

		जबतक (n < लंबाई) {
			माप_प्रकार s = strbuf_fread(sb, लंबाई - n, मानक_निवेश);
			अगर (!s && ख_पूर्ण(मानक_निवेश))
				die(_("EOF in data (%lu bytes remaining)"),
					(अचिन्हित दीर्घ)(लंबाई - n));
			n += s;
		}
	}

	skip_optional_lf();
	वापस 1;
}

अटल पूर्णांक validate_raw_date(स्थिर अक्षर *src, काष्ठा strbuf *परिणाम, पूर्णांक strict)
{
	स्थिर अक्षर *orig_src = src;
	अक्षर *endp;
	अचिन्हित दीर्घ अंक;

	त्रुटि_सं = 0;

	अंक = म_से_अदीर्घ(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	अगर (त्रुटि_सं || endp == src || *endp != ' ')
		वापस -1;

	src = endp + 1;
	अगर (*src != '-' && *src != '+')
		वापस -1;

	अंक = म_से_अदीर्घ(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	अगर (त्रुटि_सं || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < अंक))             /* parsed a broken timezone */
	   )
		वापस -1;

	strbuf_addstr(परिणाम, orig_src);
	वापस 0;
}

अटल अक्षर *parse_ident(स्थिर अक्षर *buf)
{
	स्थिर अक्षर *ltgt;
	माप_प्रकार name_len;
	काष्ठा strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	अगर (*buf == '<')
		--buf;

	ltgt = buf + म_खोज(buf, "<>");
	अगर (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	अगर (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + म_खोज(ltgt + 1, "<>");
	अगर (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	अगर (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_add(&ident, buf, name_len);

	चयन (whenspec) {
	हाल WHENSPEC_RAW:
		अगर (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		अवरोध;
	हाल WHENSPEC_RAW_PERMISSIVE:
		अगर (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		अवरोध;
	हाल WHENSPEC_RFC2822:
		अगर (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		अवरोध;
	हाल WHENSPEC_NOW:
		अगर (म_भेद("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		अवरोध;
	}

	वापस strbuf_detach(&ident, शून्य);
}

अटल व्योम parse_and_store_blob(
	काष्ठा last_object *last,
	काष्ठा object_id *oidout,
	uintmax_t mark)
{
	अटल काष्ठा strbuf buf = STRBUF_INIT;
	uintmax_t len;

	अगर (parse_तथ्य(&buf, repo_settings_get_big_file_threshold(the_repository), &len))
		store_object(OBJ_BLOB, &buf, last, oidout, mark);
	अन्यथा {
		अगर (last) {
			strbuf_release(&last->तथ्य);
			last->offset = 0;
			last->depth = 0;
		}
		stream_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

अटल व्योम parse_new_blob(काष्ठा fast_import_state *state)
{
	read_अग्र_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&last_blob, शून्य, अग्र_mark);
}

अटल व्योम unload_one_branch(व्योम)
{
	जबतक (cur_active_branches
		&& cur_active_branches >= max_active_branches) {
		uintmax_t min_commit = अच_दीर्घ_उच्च;
		काष्ठा branch *e, *l = शून्य, *p = शून्य;

		क्रम (e = active_branches; e; e = e->active_अग्र_branch) {
			अगर (e->last_commit < min_commit) {
				p = l;
				min_commit = e->last_commit;
			}
			l = e;
		}

		अगर (p) {
			e = p->active_अग्र_branch;
			p->active_अग्र_branch = e->active_अग्र_branch;
		} अन्यथा {
			e = active_branches;
			active_branches = e->active_अग्र_branch;
		}
		e->active = 0;
		e->active_अग्र_branch = शून्य;
		अगर (e->branch_वृक्ष.वृक्ष) {
			release_वृक्ष_content_recursive(e->branch_वृक्ष.वृक्ष);
			e->branch_वृक्ष.वृक्ष = शून्य;
		}
		cur_active_branches--;
	}
}

अटल व्योम load_branch(काष्ठा branch *b)
{
	load_वृक्ष(&b->branch_वृक्ष);
	अगर (!b->active) {
		b->active = 1;
		b->active_अग्र_branch = active_branches;
		active_branches = b;
		cur_active_branches++;
		branch_load_गिनती++;
	}
}

अटल अचिन्हित अक्षर convert_अंक_notes_to_fanout(uintmax_t अंक_notes)
{
	अचिन्हित अक्षर fanout = 0;
	जबतक ((अंक_notes >>= 8))
		fanout++;
	वापस fanout;
}

अटल व्योम construct_path_with_fanout(स्थिर अक्षर *hex_sha1,
		अचिन्हित अक्षर fanout, अक्षर *path)
{
	अचिन्हित पूर्णांक i = 0, j = 0;
	अगर (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	जबतक (fanout) {
		path[i++] = hex_sha1[j++];
		path[i++] = hex_sha1[j++];
		path[i++] = '/';
		fanout--;
	}
	स_नकल(path + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	path[i + the_hash_algo->hexsz - j] = '\0';
}

अटल uintmax_t do_change_note_fanout(
		काष्ठा वृक्ष_entry *orig_मूल, काष्ठा वृक्ष_entry *मूल,
		अक्षर *hex_oid, अचिन्हित पूर्णांक hex_oid_len,
		अक्षर *fullpath, अचिन्हित पूर्णांक fullpath_len,
		अचिन्हित अक्षर fanout)
{
	काष्ठा वृक्ष_content *t;
	काष्ठा वृक्ष_entry *e, leaf;
	अचिन्हित पूर्णांक i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t अंक_notes = 0;
	काष्ठा object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	अक्षर realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	स्थिर अचिन्हित hexsz = the_hash_algo->hexsz;

	अगर (!मूल->वृक्ष)
		load_वृक्ष(मूल);
	t = मूल->वृक्ष;

	क्रम (i = 0; t && i < t->entry_गिनती; i++) {
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
		अगर (!e->versions[1].mode ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			जारी;

		/* This _may_ be a note entry, or a subdir containing notes */
		स_नकल(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		अगर (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		स_नकल(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		अगर (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			अगर (fanout == 0xff) {
				/* Counting mode, no rename */
				अंक_notes++;
				जारी;
			}
			construct_path_with_fanout(hex_oid, fanout, realpath);
			अगर (!म_भेद(fullpath, realpath)) {
				/* Note entry is in correct location */
				अंक_notes++;
				जारी;
			}

			/* Rename fullpath to realpath */
			अगर (!वृक्ष_content_remove(orig_मूल, fullpath, &leaf, 0))
				die(_("failed to remove path %s"), fullpath);
			वृक्ष_content_set(orig_मूल, realpath,
				&leaf.versions[1].oid,
				leaf.versions[1].mode,
				leaf.वृक्ष);
		} अन्यथा अगर (S_ISDIR(e->versions[1].mode)) {
			/* This is a subdir that may contain note entries */
			अंक_notes += do_change_note_fanout(orig_मूल, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = मूल->वृक्ष;
	}
	वापस अंक_notes;
}

अटल uintmax_t change_note_fanout(काष्ठा वृक्ष_entry *मूल,
		अचिन्हित अक्षर fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	अक्षर hex_oid[GIT_MAX_HEXSZ], path[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	वापस do_change_note_fanout(मूल, मूल, hex_oid, 0, path, 0, fanout);
}

अटल पूर्णांक parse_mapped_oid_hex(स्थिर अक्षर *hex, काष्ठा object_id *oid, स्थिर अक्षर **end)
{
	पूर्णांक algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	स_रखो(oid->hash, 0, माप(oid->hash));

	algo = parse_oid_hex_any(hex, oid, end);
	अगर (algo == GIT_HASH_UNKNOWN)
		वापस -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	अगर (it == kh_end(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		अगर (hash_algos[algo].format_id == the_hash_algo->format_id)
			वापस 0;
		वापस -1;
	}
	oidcpy(oid, kh_मूल्य(sub_oid_map, it));
	वापस 0;
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
अटल uintmax_t parse_mark_ref(स्थिर अक्षर *p, अक्षर **endptr)
{
	uintmax_t mark;

	निश्चित(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	अगर (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	वापस mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
अटल uintmax_t parse_mark_ref_eol(स्थिर अक्षर *p)
{
	अक्षर *end;
	uintmax_t mark;

	mark = parse_mark_ref(p, &end);
	अगर (*end != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	वापस mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
अटल uintmax_t parse_mark_ref_space(स्थिर अक्षर **p)
{
	uintmax_t mark;
	अक्षर *end;

	mark = parse_mark_ref(*p, &end);
	अगर (*end++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = end;
	वापस mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
अटल व्योम parse_path(काष्ठा strbuf *sb, स्थिर अक्षर *p, स्थिर अक्षर **endp,
		पूर्णांक is_last_field, स्थिर अक्षर *field)
{
	अगर (*p == '"') {
		अगर (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), field, command_buf.buf);
		अगर (म_माप(sb->buf) != sb->len)
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
अटल व्योम parse_path_eol(काष्ठा strbuf *sb, स्थिर अक्षर *p, स्थिर अक्षर *field)
{
	स्थिर अक्षर *end;

	parse_path(sb, p, &end, 1, field);
	अगर (*end)
		die(_("garbage after %s: %s"), field, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
अटल व्योम parse_path_space(काष्ठा strbuf *sb, स्थिर अक्षर *p,
		स्थिर अक्षर **endp, स्थिर अक्षर *field)
{
	parse_path(sb, p, endp, 0, field);
	अगर (**endp != ' ')
		die(_("missing space after %s: %s"), field, command_buf.buf);
	(*endp)++;
}

अटल व्योम file_change_m(काष्ठा fast_import_state *state,
			  स्थिर अक्षर *p,
			  काष्ठा branch *b)
{
	अटल काष्ठा strbuf path = STRBUF_INIT;
	काष्ठा object_entry *oe;
	काष्ठा object_id oid;
	uint16_t mode, inline_तथ्य = 0;

	p = parse_mode(p, &mode);
	अगर (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	चयन (mode) {
	हाल 0644:
	हाल 0755:
		mode |= S_IFREG;
	हाल S_IFREG | 0644:
	हाल S_IFREG | 0755:
	हाल S_IFLNK:
	हाल S_IFDIR:
	हाल S_IFGITLINK:
		/* ok */
		अवरोध;
	शेष:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	अगर (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा अगर (skip_prefix(p, "inline ", &p)) {
		inline_तथ्य = 1;
		oe = शून्य; /* not used with inline_data, but makes gcc happy */
	} अन्यथा {
		अगर (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		अगर (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	अगर (S_ISDIR(mode) &&
	    is_empty_वृक्ष_oid(&oid, the_repository->hash_algo) &&
	    *path.buf) {
		वृक्ष_content_remove(&b->branch_वृक्ष, path.buf, शून्य, 0);
		वापस;
	}

	अगर (S_ISGITLINK(mode)) {
		अगर (inline_तथ्य)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		अन्यथा अगर (oe) {
			अगर (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} अन्यथा अगर (inline_तथ्य) {
		अगर (S_ISDIR(mode))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		जबतक (read_अग्र_command(state) != खातापूर्ण) {
			स्थिर अक्षर *v;
			अगर (skip_prefix(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			अन्यथा {
				parse_and_store_blob(&last_blob, &oid, 0);
				अवरोध;
			}
		}
	} अन्यथा {
		क्रमागत object_type expected = S_ISDIR(mode) ?
						OBJ_वृक्ष: OBJ_BLOB;
		क्रमागत object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, शून्य);
		अगर (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(mode) ?  _("tree") : _("blob"),
			    command_buf.buf);
		अगर (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	अगर (!*path.buf) {
		वृक्ष_content_replace(&b->branch_वृक्ष, &oid, mode, शून्य);
		वापस;
	}

	अगर (!verify_path(path.buf, mode))
		die(_("invalid path '%s'"), path.buf);
	वृक्ष_content_set(&b->branch_वृक्ष, path.buf, &oid, mode, शून्य);
}

अटल व्योम file_change_d(स्थिर अक्षर *p, काष्ठा branch *b)
{
	अटल काष्ठा strbuf path = STRBUF_INIT;

	strbuf_reset(&path);
	parse_path_eol(&path, p, "path");
	वृक्ष_content_remove(&b->branch_वृक्ष, path.buf, शून्य, 1);
}

अटल व्योम file_change_cr(स्थिर अक्षर *p, काष्ठा branch *b, पूर्णांक नाम)
{
	अटल काष्ठा strbuf source = STRBUF_INIT;
	अटल काष्ठा strbuf dest = STRBUF_INIT;
	काष्ठा वृक्ष_entry leaf;

	strbuf_reset(&source);
	parse_path_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_path_eol(&dest, p, "dest");

	स_रखो(&leaf, 0, माप(leaf));
	अगर (नाम)
		वृक्ष_content_remove(&b->branch_वृक्ष, source.buf, &leaf, 1);
	अन्यथा
		वृक्ष_content_get(&b->branch_वृक्ष, source.buf, &leaf, 1);
	अगर (!leaf.versions[1].mode)
		die(_("path %s not in branch"), source.buf);
	अगर (!*dest.buf) {	/* C "path/to/subdir" "" */
		वृक्ष_content_replace(&b->branch_वृक्ष,
			&leaf.versions[1].oid,
			leaf.versions[1].mode,
			leaf.वृक्ष);
		वापस;
	}
	अगर (!verify_path(dest.buf, leaf.versions[1].mode))
		die(_("invalid path '%s'"), dest.buf);
	वृक्ष_content_set(&b->branch_वृक्ष, dest.buf,
		&leaf.versions[1].oid,
		leaf.versions[1].mode,
		leaf.वृक्ष);
}

अटल व्योम note_change_n(काष्ठा fast_import_state *state,
			  स्थिर अक्षर *p,
			  काष्ठा branch *b,
			  अचिन्हित अक्षर *old_fanout)
{
	काष्ठा object_entry *oe;
	काष्ठा branch *s;
	काष्ठा object_id oid, commit_oid;
	अक्षर path[GIT_MAX_RAWSZ * 3];
	uint16_t inline_तथ्य = 0;
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
	अगर (b->अंक_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->अंक_notes = change_note_fanout(&b->branch_वृक्ष, 0xff);
		*old_fanout = convert_अंक_notes_to_fanout(b->अंक_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	अगर (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा अगर (skip_prefix(p, "inline ", &p)) {
		inline_तथ्य = 1;
		oe = शून्य; /* not used with inline_data, but makes gcc happy */
	} अन्यथा {
		अगर (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		अगर (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	अगर (s) {
		अगर (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} अन्यथा अगर (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		काष्ठा object_entry *commit_oe = find_mark(marks, commit_mark);
		अगर (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} अन्यथा अगर (!repo_get_oid(the_repository, p, &commit_oid)) {
		माप_प्रकार आकार;
		अक्षर *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &आकार,
						   &commit_oid);
		अगर (!buf || आकार < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		मुक्त(buf);
	} अन्यथा
		die(_("invalid ref name or SHA1 expression: %s"), p);

	अगर (inline_तथ्य) {
		read_अग्र_command(state);
		parse_and_store_blob(&last_blob, &oid, 0);
	} अन्यथा अगर (oe) {
		अगर (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} अन्यथा अगर (!is_null_oid(&oid)) {
		क्रमागत object_type type = odb_read_object_info(the_repository->objects, &oid,
							शून्य);
		अगर (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		अगर (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_path_with_fanout(oid_to_hex(&commit_oid), *old_fanout, path);
	अगर (वृक्ष_content_remove(&b->branch_वृक्ष, path, शून्य, 0))
		b->अंक_notes--;

	अगर (is_null_oid(&oid))
		वापस; /* nothing to insert */

	b->अंक_notes++;
	new_fanout = convert_अंक_notes_to_fanout(b->अंक_notes);
	construct_path_with_fanout(oid_to_hex(&commit_oid), new_fanout, path);
	वृक्ष_content_set(&b->branch_वृक्ष, path, &oid, S_IFREG | 0644, शून्य);
}

अटल व्योम file_change_deleteall(काष्ठा branch *b)
{
	release_वृक्ष_content_recursive(b->branch_वृक्ष.वृक्ष);
	oidclr(&b->branch_वृक्ष.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_वृक्ष.versions[1].oid, the_repository->hash_algo);
	load_वृक्ष(&b->branch_वृक्ष);
	b->अंक_notes = 0;
}

अटल व्योम parse_from_commit(काष्ठा branch *b, अक्षर *buf, अचिन्हित दीर्घ आकार)
{
	अगर (!buf || आकार < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_to_hex(&b->oid));
	अगर (स_भेद("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_वृक्ष.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_to_hex(&b->oid));
	oidcpy(&b->branch_वृक्ष.versions[0].oid,
	       &b->branch_वृक्ष.versions[1].oid);
}

अटल व्योम parse_from_existing(काष्ठा branch *b)
{
	अगर (is_null_oid(&b->oid)) {
		oidclr(&b->branch_वृक्ष.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_वृक्ष.versions[1].oid, the_repository->hash_algo);
	} अन्यथा {
		अचिन्हित दीर्घ आकार;
		माप_प्रकार आकार_st = 0;
		अक्षर *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &आकार_st, &b->oid);
		आकार = cast_आकार_t_to_ulong(आकार_st);
		parse_from_commit(b, buf, आकार);
		मुक्त(buf);
	}
}

अटल पूर्णांक parse_objectish(काष्ठा fast_import_state *state,
			   काष्ठा branch *b,
			   स्थिर अक्षर *objectish)
{
	काष्ठा branch *s;
	काष्ठा object_id oid;

	oidcpy(&oid, &b->branch_वृक्ष.versions[1].oid);

	s = lookup_branch(objectish);
	अगर (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	अन्यथा अगर (s) {
		काष्ठा object_id *t = &s->branch_वृक्ष.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_वृक्ष.versions[0].oid, t);
		oidcpy(&b->branch_वृक्ष.versions[1].oid, t);
	} अन्यथा अगर (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		काष्ठा object_entry *oe = find_mark(marks, idnum);
		अगर (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		अगर (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			अगर (oe->pack_id != MAX_PACK_ID) {
				अचिन्हित दीर्घ आकार;
				अक्षर *buf = gfi_unpack_entry(oe, &आकार);
				parse_from_commit(b, buf, आकार);
				मुक्त(buf);
			} अन्यथा
				parse_from_existing(b);
		}
	} अन्यथा अगर (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		अगर (is_null_oid(&b->oid))
			b->delete = 1;
	}
	अन्यथा
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	अगर (b->branch_वृक्ष.वृक्ष && !oideq(&oid, &b->branch_वृक्ष.versions[1].oid)) {
		release_वृक्ष_content_recursive(b->branch_वृक्ष.वृक्ष);
		b->branch_वृक्ष.वृक्ष = शून्य;
	}

	read_अग्र_command(state);
	वापस 1;
}

अटल पूर्णांक parse_from(काष्ठा fast_import_state *state, काष्ठा branch *b)
{
	स्थिर अक्षर *from;

	अगर (!skip_prefix(command_buf.buf, "from ", &from))
		वापस 0;

	वापस parse_objectish(state, b, from);
}

अटल पूर्णांक parse_objectish_with_prefix(काष्ठा fast_import_state *state,
				       काष्ठा branch *b,
				       स्थिर अक्षर *prefix)
{
	स्थिर अक्षर *base;

	अगर (!skip_prefix(command_buf.buf, prefix, &base))
		वापस 0;

	वापस parse_objectish(state, b, base);
}

अटल काष्ठा hash_list *parse_merge(काष्ठा fast_import_state *state,
				     अचिन्हित पूर्णांक *गिनती)
{
	काष्ठा hash_list *list = शून्य, **पुच्छ = &list, *n;
	स्थिर अक्षर *from;
	काष्ठा branch *s;

	*गिनती = 0;
	जबतक (skip_prefix(command_buf.buf, "merge ", &from)) {
		n = xmalloc(माप(*n));
		s = lookup_branch(from);
		अगर (s)
			oidcpy(&n->oid, &s->oid);
		अन्यथा अगर (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			काष्ठा object_entry *oe = find_mark(marks, idnum);
			अगर (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} अन्यथा अगर (!repo_get_oid(the_repository, from, &n->oid)) {
			माप_प्रकार आकार;
			अक्षर *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &आकार, &n->oid);
			अगर (!buf || आकार < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			मुक्त(buf);
		} अन्यथा
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->अग्र = शून्य;
		*पुच्छ = n;
		पुच्छ = &n->अग्र;

		(*गिनती)++;
		read_अग्र_command(state);
	}
	वापस list;
}

काष्ठा signature_तथ्य {
	अक्षर *hash_algo;      /* "sha1" or "sha256" */
	अक्षर *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	काष्ठा strbuf तथ्य;   /* The actual signature data */
};

अटल व्योम parse_one_signature(काष्ठा fast_import_state *state,
				काष्ठा signature_तथ्य *sig,
				स्थिर अक्षर *v)
{
	अक्षर *args = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	अक्षर *space = म_अक्षर(args, ' ');

	अगर (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), args);
	*space = '\0';

	sig->hash_algo = args;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	अगर (म_भेद(sig->hash_algo, "sha1") &&
	    म_भेद(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	अगर (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	अगर (!म_भेद(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_अग्र_command(state);
	parse_तथ्य(&sig->तथ्य, 0, शून्य);
}

अटल व्योम discard_one_signature(काष्ठा fast_import_state *state)
{
	काष्ठा strbuf तथ्य = STRBUF_INIT;

	read_अग्र_command(state);
	parse_तथ्य(&तथ्य, 0, शून्य);
	strbuf_release(&तथ्य);
}

अटल व्योम add_gpgsig_to_commit(काष्ठा strbuf *commit_तथ्य,
				 स्थिर अक्षर *header,
				 काष्ठा signature_तथ्य *sig)
{
	काष्ठा string_list siglines = STRING_LIST_INIT_NODUP;

	अगर (!sig || !sig->hash_algo)
		वापस;

	strbuf_addstr(commit_तथ्य, header);
	string_list_split_in_place(&siglines, sig->तथ्य.buf, "\n", -1);
	strbuf_add_separated_string_list(commit_तथ्य, "\n ", &siglines);
	strbuf_addch(commit_तथ्य, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->तथ्य);
	मुक्त(sig->hash_algo);
}

अटल व्योम store_signature(काष्ठा signature_तथ्य *stored_sig,
			    काष्ठा signature_तथ्य *new_sig,
			    स्थिर अक्षर *hash_type)
{
	अगर (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->तथ्य);
		मुक्त(new_sig->hash_algo);
	} अन्यथा {
		*stored_sig = *new_sig;
	}
}

अटल व्योम import_one_signature(काष्ठा fast_import_state *state,
				 काष्ठा signature_तथ्य *sig_sha1,
				 काष्ठा signature_तथ्य *sig_sha256,
				 स्थिर अक्षर *v)
{
	काष्ठा signature_तथ्य sig = { शून्य, शून्य, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	अगर (!म_भेद(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	अन्यथा अगर (!म_भेद(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	अन्यथा
		die(_("parse_one_signature() returned unknown hash algo"));
}

अटल व्योम finalize_commit_बफर(काष्ठा strbuf *new_तथ्य,
				   काष्ठा signature_तथ्य *sig_sha1,
				   काष्ठा signature_तथ्य *sig_sha256,
				   काष्ठा strbuf *संदेश)
{
	add_gpgsig_to_commit(new_तथ्य, "gpgsig ", sig_sha1);
	add_gpgsig_to_commit(new_तथ्य, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_तथ्य, '\n');
	strbuf_addbuf(new_तथ्य, संदेश);
}

अटल व्योम warn_invalid_signature(काष्ठा signature_check *check,
				   स्थिर अक्षर *संदेश, क्रमागत sign_mode mode)
{
	स्थिर अक्षर *signer = check->signer ? check->signer : _("unknown");
	स्थिर अक्षर *subject;
	पूर्णांक subject_len = find_commit_subject(संदेश, &subject);

	चयन (mode) {
	हाल SIGN_STRIP_IF_INVALID:
		अगर (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		अन्यथा अगर (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		अन्यथा
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		अवरोध;
	हाल SIGN_SIGN_IF_INVALID:
		अगर (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		अन्यथा अगर (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		अन्यथा
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		अवरोध;
	शेष:
		BUG("unsupported signing mode");
	}
}

अटल व्योम handle_signature_if_invalid(काष्ठा strbuf *new_तथ्य,
					काष्ठा signature_तथ्य *sig_sha1,
					काष्ठा signature_तथ्य *sig_sha256,
					काष्ठा strbuf *संदेश,
					क्रमागत sign_mode mode)
{
	काष्ठा strbuf tmp_buf = STRBUF_INIT;
	काष्ठा signature_check signature_check = { 0 };
	पूर्णांक ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_तथ्य);
	finalize_commit_बफर(&tmp_buf, sig_sha1, sig_sha256, संदेश);
	ret = verify_commit_बफर(tmp_buf.buf, tmp_buf.len, &signature_check);

	अगर (ret) {
		अगर (mode == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, संदेश->buf, mode);

		अगर (mode == SIGN_SIGN_IF_INVALID) {
			काष्ठा strbuf signature = STRBUF_INIT;
			काष्ठा strbuf payload = STRBUF_INIT;

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
			अगर (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			अगर (sign_बफर(&payload, &signature, signed_commit_keyid,
					SIGN_बफर_USE_DEFAULT_कुंजी))
				die(_("failed to sign commit object"));
			add_header_signature(new_तथ्य, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_बफर(new_तथ्य, शून्य, शून्य, संदेश);
	} अन्यथा {
		strbuf_swap(new_तथ्य, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

अटल व्योम parse_new_commit(काष्ठा fast_import_state *state, स्थिर अक्षर *arg)
{
	अटल काष्ठा strbuf संदेश = STRBUF_INIT;
	काष्ठा signature_तथ्य sig_sha1 = { शून्य, शून्य, STRBUF_INIT };
	काष्ठा signature_तथ्य sig_sha256 = { शून्य, शून्य, STRBUF_INIT };
	काष्ठा branch *b;
	अक्षर *author = शून्य;
	अक्षर *committer = शून्य;
	अक्षर *encoding = शून्य;
	काष्ठा hash_list *merge_list = शून्य;
	अचिन्हित पूर्णांक merge_गिनती;
	अचिन्हित अक्षर पूर्व_fanout, new_fanout;
	स्थिर अक्षर *v;

	b = lookup_branch(arg);
	अगर (!b)
		b = new_branch(arg);

	read_अग्र_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	अगर (skip_prefix(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_अग्र_command(state);
	}
	अगर (skip_prefix(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_अग्र_command(state);
	}
	अगर (!committer)
		die(_("expected committer but didn't get one"));

	जबतक (skip_prefix(command_buf.buf, "gpgsig ", &v)) {
		चयन (signed_commit_mode) {

		/* First, modes that don't need the signature to be parsed */
		हाल SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		हाल SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		हाल SIGN_STRIP:
			discard_one_signature(state);
			अवरोध;

		/* Second, modes that parse the signature */
		हाल SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		हाल SIGN_VERBATIM:
		हाल SIGN_STRIP_IF_INVALID:
		हाल SIGN_SIGN_IF_INVALID:
		हाल SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			अवरोध;

		/* Third, BUG */
		शेष:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		read_अग्र_command(state);
	}

	अगर (skip_prefix(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_अग्र_command(state);
	}
	parse_तथ्य(&संदेश, 0, शून्य);
	read_अग्र_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_गिनती);

	/* ensure the branch is active/loaded */
	अगर (!b->branch_वृक्ष.वृक्ष || !max_active_branches) {
		unload_one_branch();
		load_branch(b);
	}

	पूर्व_fanout = convert_अंक_notes_to_fanout(b->अंक_notes);

	/* file_change* */
	जबतक (command_buf.len > 0) {
		अगर (skip_prefix(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		अन्यथा अगर (skip_prefix(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		अन्यथा अगर (skip_prefix(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		अन्यथा अगर (skip_prefix(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		अन्यथा अगर (skip_prefix(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &पूर्व_fanout);
		अन्यथा अगर (!म_भेद("deleteall", command_buf.buf))
			file_change_deleteall(b);
		अन्यथा अगर (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		अन्यथा अगर (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		अन्यथा {
			unread_command_buf = 1;
			अवरोध;
		}
		अगर (read_अग्र_command(state) == खातापूर्ण)
			अवरोध;
	}

	new_fanout = convert_अंक_notes_to_fanout(b->अंक_notes);
	अगर (new_fanout != पूर्व_fanout)
		b->अंक_notes = change_note_fanout(&b->branch_वृक्ष, new_fanout);

	/* build the tree and the commit */
	store_वृक्ष(&b->branch_वृक्ष);
	oidcpy(&b->branch_वृक्ष.versions[0].oid,
	       &b->branch_वृक्ष.versions[1].oid);

	strbuf_reset(&new_तथ्य);
	strbuf_addf(&new_तथ्य, "tree %s\n",
		oid_to_hex(&b->branch_वृक्ष.versions[1].oid));
	अगर (!is_null_oid(&b->oid))
		strbuf_addf(&new_तथ्य, "parent %s\n",
			    oid_to_hex(&b->oid));
	जबतक (merge_list) {
		काष्ठा hash_list *अग्र = merge_list->अग्र;
		strbuf_addf(&new_तथ्य, "parent %s\n",
			    oid_to_hex(&merge_list->oid));
		मुक्त(merge_list);
		merge_list = अग्र;
	}
	strbuf_addf(&new_तथ्य,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	अगर (encoding)
		strbuf_addf(&new_तथ्य,
			"encoding %s\n",
			encoding);

	अगर ((signed_commit_mode == SIGN_STRIP_IF_INVALID ||
	     signed_commit_mode == SIGN_SIGN_IF_INVALID ||
	     signed_commit_mode == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_तथ्य, &sig_sha1, &sig_sha256,
					    &संदेश, signed_commit_mode);
	अन्यथा
		finalize_commit_बफर(&new_तथ्य, &sig_sha1, &sig_sha256, &संदेश);

	मुक्त(author);
	मुक्त(committer);
	मुक्त(encoding);

	अगर (!store_object(OBJ_COMMIT, &new_तथ्य, शून्य, &b->oid, अग्र_mark))
		b->pack_id = pack_id;
	b->last_commit = object_गिनती_by_type[OBJ_COMMIT];
}

अटल व्योम handle_tag_signature_if_invalid(काष्ठा strbuf *buf,
					    काष्ठा strbuf *संदेश,
					    माप_प्रकार sig_offset)
{
	काष्ठा strbuf signature = STRBUF_INIT;
	काष्ठा strbuf payload = STRBUF_INIT;
	काष्ठा signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_add(&payload, संदेश->buf, sig_offset);
	strbuf_add(&signature, संदेश->buf + sig_offset, संदेश->len - sig_offset);

	sigc.payload_type = SIGNATURE_PAYLOAD_TAG;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	अगर (!check_signature(&sigc, signature.buf, signature.len))
		जाओ out;

	अगर (signed_tag_mode == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(संदेश, sig_offset);

	अगर (signed_tag_mode == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = शून्य;
		strbuf_reset(&signature);

		अगर (sign_बफर(&payload, &signature, signed_tag_keyid,
				SIGN_बफर_USE_DEFAULT_कुंजी))
			die(_("failed to sign tag object"));

		strbuf_addbuf(संदेश, &signature);
	}

out:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

अटल व्योम handle_tag_signature(काष्ठा strbuf *buf, काष्ठा strbuf *संदेश, स्थिर अक्षर *name)
{
	माप_प्रकार sig_offset = parse_signed_बफर(संदेश->buf, संदेश->len);

	/* If there is no signature, there is nothing to do. */
	अगर (sig_offset >= संदेश->len)
		वापस;

	चयन (signed_tag_mode) {

	/* First, modes that don't change anything */
	हाल SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	हाल SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		अवरोध;

	/* Second, modes that remove the signature */
	हाल SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	हाल SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(संदेश, sig_offset);
		अवरोध;
	हाल SIGN_ABORT_IF_INVALID:
	हाल SIGN_SIGN_IF_INVALID:
	हाल SIGN_STRIP_IF_INVALID:
		handle_tag_signature_if_invalid(buf, संदेश, sig_offset);
		अवरोध;

	/* Third, aborting modes */
	हाल SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	शेष:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_tag_mode, name);
	}
}

अटल व्योम parse_new_tag(काष्ठा fast_import_state *state, स्थिर अक्षर *arg)
{
	अटल काष्ठा strbuf संदेश = STRBUF_INIT;
	स्थिर अक्षर *from;
	अक्षर *tagger;
	काष्ठा branch *s;
	काष्ठा tag *t;
	uintmax_t from_mark = 0;
	काष्ठा object_id oid;
	क्रमागत object_type type;
	स्थिर अक्षर *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, माप(काष्ठा tag));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	अगर (last_tag)
		last_tag->अग्र_tag = t;
	अन्यथा
		first_tag = t;
	last_tag = t;
	read_अग्र_command(state);
	parse_mark(state);

	/* from ... */
	अगर (!skip_prefix(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	अगर (s) {
		अगर (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} अन्यथा अगर (*from == ':') {
		काष्ठा object_entry *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा अगर (!repo_get_oid(the_repository, from, &oid)) {
		काष्ठा object_entry *oe = find_object(&oid);
		अगर (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, शून्य);
			अगर (type < 0)
				die(_("not a valid object: %s"), from);
		} अन्यथा
			type = oe->type;
	} अन्यथा
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_अग्र_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	अगर (skip_prefix(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_अग्र_command(state);
	} अन्यथा
		tagger = शून्य;

	/* tag payload/message */
	parse_तथ्य(&संदेश, 0, शून्य);

	/* build the tag object */
	strbuf_reset(&new_तथ्य);

	strbuf_addf(&new_तथ्य,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_to_hex(&oid), type_name(type), t->name);
	अगर (tagger)
		strbuf_addf(&new_तथ्य,
			    "tagger %s\n", tagger);

	handle_tag_signature(&new_तथ्य, &संदेश, t->name);

	strbuf_addch(&new_तथ्य, '\n');
	strbuf_addbuf(&new_तथ्य, &संदेश);
	मुक्त(tagger);

	अगर (store_object(OBJ_TAG, &new_तथ्य, शून्य, &t->oid, अग्र_mark))
		t->pack_id = MAX_PACK_ID;
	अन्यथा
		t->pack_id = pack_id;
}

अटल व्योम parse_reset_branch(काष्ठा fast_import_state *state, स्थिर अक्षर *arg)
{
	काष्ठा branch *b;
	स्थिर अक्षर *tag_name;

	b = lookup_branch(arg);
	अगर (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_वृक्ष.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_वृक्ष.versions[1].oid, the_repository->hash_algo);
		अगर (b->branch_वृक्ष.वृक्ष) {
			release_वृक्ष_content_recursive(b->branch_वृक्ष.वृक्ष);
			b->branch_वृक्ष.वृक्ष = शून्य;
		}
	}
	अन्यथा
		b = new_branch(arg);
	read_अग्र_command(state);
	parse_from(state, b);
	अगर (b->delete && skip_prefix(b->name, "refs/tags/", &tag_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		काष्ठा tag *t, *पूर्व = शून्य;
		क्रम (t = first_tag; t; t = t->अग्र_tag) {
			अगर (!म_भेद(t->name, tag_name))
				अवरोध;
			पूर्व = t;
		}
		अगर (t) {
			अगर (पूर्व)
				पूर्व->अग्र_tag = t->अग्र_tag;
			अन्यथा
				first_tag = t->अग्र_tag;
			अगर (!t->अग्र_tag)
				last_tag = पूर्व;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	अगर (command_buf.len > 0)
		unread_command_buf = 1;
}

अटल व्योम cat_blob_write(स्थिर अक्षर *buf, अचिन्हित दीर्घ आकार)
{
	अगर (write_in_full(cat_blob_fd, buf, आकार) < 0)
		die_errno(_("write to frontend failed"));
}

अटल व्योम cat_blob(काष्ठा object_entry *oe, काष्ठा object_id *oid)
{
	काष्ठा strbuf पंक्ति = STRBUF_INIT;
	काष्ठा iovec iov[3];
	अचिन्हित दीर्घ आकार;
	क्रमागत object_type type = 0;
	अक्षर *buf;

	अगर (!oe || oe->pack_id == MAX_PACK_ID) {
		माप_प्रकार आकार_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &आकार_st);
		आकार = cast_आकार_t_to_ulong(आकार_st);
	} अन्यथा {
		type = oe->type;
		buf = gfi_unpack_entry(oe, &आकार);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	अगर (type <= 0) {
		strbuf_reset(&पंक्ति);
		strbuf_addf(&पंक्ति, "%s missing\n", oid_to_hex(oid));
		cat_blob_write(पंक्ति.buf, पंक्ति.len);
		strbuf_release(&पंक्ति);
		मुक्त(buf);
		वापस;
	}
	अगर (!buf)
		die(_("can't read object %s"), oid_to_hex(oid));
	अगर (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_to_hex(oid), type_name(type));
	strbuf_reset(&पंक्ति);
	strbuf_addf(&पंक्ति, "%s %s %"PRIuMAX"\n", oid_to_hex(oid),
		    type_name(type), (uintmax_t)आकार);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_base = पंक्ति.buf;
	iov[0].iov_len = पंक्ति.len;
	iov[1].iov_base = buf;
	iov[1].iov_len = आकार;
	iov[2].iov_base = (व्योम *) "\n";
	iov[2].iov_len = 1;

	अगर (writev_in_full(cat_blob_fd, iov, ARRAY_आकार(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&पंक्ति);
	अगर (oe && oe->pack_id == pack_id) {
		last_blob.offset = oe->idx.offset;
		strbuf_attach(&last_blob.तथ्य, buf, आकार, आकार + 1);
		last_blob.depth = oe->depth;
	} अन्यथा
		मुक्त(buf);
}

अटल व्योम parse_get_mark(काष्ठा fast_import_state *state UNUSED,
			   स्थिर अक्षर *p)
{
	काष्ठा object_entry *oe;
	अक्षर output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	अगर (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	अगर (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, माप(output), "%s\n", oid_to_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

अटल व्योम parse_cat_blob(काष्ठा fast_import_state *state UNUSED,
			   स्थिर अक्षर *p)
{
	काष्ठा object_entry *oe;
	काष्ठा object_id oid;

	/* cat-blob SP <object> LF */
	अगर (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		अगर (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा {
		अगर (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		अगर (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

अटल काष्ठा object_entry *dereference(काष्ठा object_entry *oe,
					काष्ठा object_id *oid)
{
	अचिन्हित दीर्घ आकार;
	अक्षर *buf = शून्य;
	स्थिर अचिन्हित hexsz = the_hash_algo->hexsz;

	अगर (!oe) {
		क्रमागत object_type type = odb_read_object_info(the_repository->objects,
							     oid, शून्य);
		अगर (type < 0)
			die(_("object not found: %s"), oid_to_hex(oid));
		/* cache it! */
		oe = जोड़ो_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.offset = 1;
	}
	चयन (oe->type) {
	हाल OBJ_वृक्ष:	/* easy case. */
		वापस oe;
	हाल OBJ_COMMIT:
	हाल OBJ_TAG:
		अवरोध;
	शेष:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	अगर (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_entry(oe, &आकार);
	} अन्यथा {
		क्रमागत object_type unused;
		माप_प्रकार आकार_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &unused, &आकार_st);
		आकार = cast_आकार_t_to_ulong(आकार_st);
	}
	अगर (!buf)
		die(_("can't load object %s"), oid_to_hex(oid));

	/* Peel one layer. */
	चयन (oe->type) {
	हाल OBJ_TAG:
		अगर (आकार < hexsz + म_माप("object ") ||
		    get_oid_hex(buf + म_माप("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		अवरोध;
	हाल OBJ_COMMIT:
		अगर (आकार < hexsz + म_माप("tree ") ||
		    get_oid_hex(buf + म_माप("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	मुक्त(buf);
	वापस find_object(oid);
}

अटल व्योम जोड़ो_mapped_mark(uintmax_t mark, व्योम *object, व्योम *cbp)
{
	काष्ठा object_id *fromoid = object;
	काष्ठा object_id *tooid = find_mark(cbp, mark);
	पूर्णांक ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	अगर (ret == 0)
		वापस;
	kh_मूल्य(sub_oid_map, it) = tooid;
}

अटल व्योम build_mark_map_one(काष्ठा mark_set *from, काष्ठा mark_set *to)
{
	for_each_mark(from, 0, जोड़ो_mapped_mark, to);
}

अटल व्योम build_mark_map(काष्ठा string_list *from, काष्ठा string_list *to)
{
	काष्ठा string_list_item *fromp, *शीर्ष_2;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		शीर्ष_2 = string_list_lookup(to, fromp->string);
		अगर (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} अन्यथा अगर (!शीर्ष_2 || !शीर्ष_2->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, शीर्ष_2->util);
	}
}

अटल काष्ठा object_entry *parse_treeish_dataref(स्थिर अक्षर **p)
{
	काष्ठा object_id oid;
	काष्ठा object_entry *e;

	अगर (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		अगर (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} अन्यथा {	/* <sha1> */
		अगर (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		अगर (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	जबतक (!e || e->type != OBJ_वृक्ष)
		e = dereference(e, &oid);
	वापस e;
}

अटल व्योम print_ls(पूर्णांक mode, स्थिर अचिन्हित अक्षर *hash, स्थिर अक्षर *path)
{
	अटल काष्ठा strbuf पंक्ति = STRBUF_INIT;

	/* See show_tree(). */
	स्थिर अक्षर *type =
		S_ISGITLINK(mode) ? commit_type :
		S_ISDIR(mode) ? वृक्ष_type :
		blob_type;

	अगर (!mode) {
		/* missing SP path LF */
		strbuf_reset(&पंक्ति);
		strbuf_addstr(&पंक्ति, "missing ");
		quote_c_style(path, &पंक्ति, शून्य, 0);
		strbuf_addch(&पंक्ति, '\n');
	} अन्यथा {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&पंक्ति);
		strbuf_addf(&पंक्ति, "%06o %s %s\t",
				mode & ~NO_DELTA, type, hash_to_hex(hash));
		quote_c_style(path, &पंक्ति, शून्य, 0);
		strbuf_addch(&पंक्ति, '\n');
	}
	cat_blob_write(पंक्ति.buf, पंक्ति.len);
}

अटल व्योम parse_ls(काष्ठा fast_import_state *state UNUSED,
		     स्थिर अक्षर *p,
		     काष्ठा branch *b)
{
	अटल काष्ठा strbuf path = STRBUF_INIT;
	काष्ठा वृक्ष_entry *मूल = शून्य;
	काष्ठा वृक्ष_entry leaf = {शून्य};

	/* ls SP (<tree-ish> SP)? <path> */
	अगर (*p == '"') {
		अगर (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		मूल = &b->branch_वृक्ष;
	} अन्यथा {
		काष्ठा object_entry *e = parse_treeish_dataref(&p);
		मूल = new_वृक्ष_entry();
		oidcpy(&मूल->versions[1].oid, &e->idx.oid);
		अगर (!is_null_oid(&मूल->versions[1].oid))
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
	अगर (S_ISDIR(leaf.versions[1].mode))
		store_वृक्ष(&leaf);

	print_ls(leaf.versions[1].mode, leaf.versions[1].oid.hash, path.buf);
	अगर (leaf.वृक्ष)
		release_वृक्ष_content_recursive(leaf.वृक्ष);
	अगर (!b || मूल != &b->branch_वृक्ष)
		release_वृक्ष_entry(मूल);
}

अटल व्योम checkpoint(व्योम)
{
	checkpoint_requested = 0;
	अगर (object_गिनती) {
		cycle_packfile();
	}
	dump_branches();
	dump_tags();
	dump_marks();
}

अटल व्योम parse_checkpoint(काष्ठा fast_import_state *state UNUSED)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

अटल व्योम parse_progress(काष्ठा fast_import_state *state UNUSED)
{
	ख_डालो(command_buf.buf, 1, command_buf.len, मानक_निकास);
	ख_अक्षर_दो('\n', मानक_निकास);
	ख_साफ(मानक_निकास);
	skip_optional_lf();
}

अटल व्योम parse_alias(काष्ठा fast_import_state *state)
{
	काष्ठा object_entry *e;
	काष्ठा branch b;

	skip_optional_lf();
	read_अग्र_command(state);

	/* mark ... */
	parse_mark(state);
	अगर (!अग्र_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	स_रखो(&b, 0, माप(b));
	अगर (!parse_objectish_with_prefix(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	निश्चित(e);
	जोड़ो_mark(&marks, अग्र_mark, e);
}

अटल अक्षर* make_fast_import_path(काष्ठा fast_import_state *state,
				   स्थिर अक्षर *path)
{
	अगर (!relative_marks_paths || is_absolute_path(path))
		वापस prefix_filename(state->prefix, path);
	वापस repo_git_path(the_repository, "info/fast-import/%s", path);
}

अटल व्योम option_import_marks(काष्ठा fast_import_state *state,
				स्थिर अक्षर *marks,
				पूर्णांक from_stream,
				पूर्णांक ignore_missing)
{
	अगर (import_marks_file) {
		अगर (from_stream)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		अगर(!import_marks_file_from_stream)
			read_marks();
	}

	मुक्त(import_marks_file);
	import_marks_file = make_fast_import_path(state, marks);
	import_marks_file_from_stream = from_stream;
	import_marks_file_ignore_missing = ignore_missing;
}

अटल व्योम option_date_format(स्थिर अक्षर *fmt)
{
	अगर (!म_भेद(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	अन्यथा अगर (!म_भेद(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	अन्यथा अगर (!म_भेद(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	अन्यथा अगर (!म_भेद(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	अन्यथा
		die(_("unknown --date-format argument %s"), fmt);
}

अटल अचिन्हित दीर्घ ulong_arg(स्थिर अक्षर *option, स्थिर अक्षर *arg)
{
	अक्षर *endptr;
	अचिन्हित दीर्घ rv = म_से_अदीर्घ(arg, &endptr, 0);
	अगर (म_अक्षर(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	वापस rv;
}

अटल व्योम option_depth(स्थिर अक्षर *depth)
{
	max_depth = ulong_arg("--depth", depth);
	अगर (max_depth > MAX_DEPTH)
		die(_("--depth cannot exceed %u"), MAX_DEPTH);
}

अटल व्योम option_active_branches(स्थिर अक्षर *branches)
{
	max_active_branches = ulong_arg("--active-branches", branches);
}

अटल व्योम option_export_marks(काष्ठा fast_import_state *state,
				स्थिर अक्षर *marks)
{
	मुक्त(export_marks_file);
	export_marks_file = make_fast_import_path(state, marks);
}

अटल व्योम option_cat_blob_fd(काष्ठा fast_import_state *state UNUSED,
			       स्थिर अक्षर *fd)
{
	अचिन्हित दीर्घ n = ulong_arg("--cat-blob-fd", fd);
	अगर (n > (अचिन्हित दीर्घ) पूर्णांक_उच्च)
		die(_("--cat-blob-fd cannot exceed %d"), पूर्णांक_उच्च);
	cat_blob_fd = (पूर्णांक) n;
}

अटल व्योम option_export_pack_edges(काष्ठा fast_import_state *state,
				     स्थिर अक्षर *edges)
{
	अक्षर *fn = prefix_filename(state->prefix, edges);
	अगर (pack_edges)
		ख_बंद(pack_edges);
	pack_edges = xfopen(fn, "a");
	मुक्त(fn);
}

अटल व्योम option_rewrite_submodules(काष्ठा fast_import_state *state,
				      स्थिर अक्षर *arg,
				      काष्ठा string_list *list)
{
	काष्ठा mark_set *ms;
	खाता *fp;
	अक्षर *s = xstrdup(arg);
	अक्षर *f = म_अक्षर(s, ':');
	अगर (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = prefix_filename(state->prefix, f);
	fp = ख_खोलो(f, "r");
	अगर (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, जोड़ो_oid_entry);
	ख_बंद(fp);
	मुक्त(f);

	string_list_जोड़ो(list, s)->util = ms;

	मुक्त(s);
}

अटल व्योम option_max_pack_आकार(स्थिर अक्षर *arg)
{
	अचिन्हित दीर्घ v;

	अगर (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	अगर (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} अन्यथा अगर (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

अटल व्योम option_big_file_threshold(स्थिर अक्षर *arg)
{
	अचिन्हित दीर्घ v;

	अगर (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_set_big_file_threshold(the_repository, v);
}

अटल व्योम option_signed_commits(स्थिर अक्षर *arg)
{
	अगर (parse_sign_mode(arg, &signed_commit_mode, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

अटल व्योम option_signed_tags(स्थिर अक्षर *arg)
{
	अगर (parse_sign_mode(arg, &signed_tag_mode, &signed_tag_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

अटल व्योम option_quiet(व्योम)
{
	show_stats = 0;
	quiet = 1;
}

अटल पूर्णांक parse_one_option(काष्ठा fast_import_state *state, स्थिर अक्षर *option)
{
	अगर (skip_prefix(option, "max-pack-size=", &option)) {
		option_max_pack_आकार(option);
	} अन्यथा अगर (skip_prefix(option, "big-file-threshold=", &option)) {
		option_big_file_threshold(option);
	} अन्यथा अगर (skip_prefix(option, "depth=", &option)) {
		option_depth(option);
	} अन्यथा अगर (skip_prefix(option, "active-branches=", &option)) {
		option_active_branches(option);
	} अन्यथा अगर (skip_prefix(option, "export-pack-edges=", &option)) {
		option_export_pack_edges(state, option);
	} अन्यथा अगर (skip_prefix(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} अन्यथा अगर (skip_prefix(option, "signed-tags=", &option)) {
		option_signed_tags(option);
	} अन्यथा अगर (!म_भेद(option, "quiet")) {
		option_quiet();
	} अन्यथा अगर (!म_भेद(option, "stats")) {
		show_stats = 1;
	} अन्यथा अगर (!म_भेद(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} अन्यथा {
		वापस 0;
	}

	वापस 1;
}

अटल व्योम check_unsafe_feature(काष्ठा fast_import_state *state,
				 स्थिर अक्षर *feature)
{
	अगर (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

अटल पूर्णांक parse_one_feature(काष्ठा fast_import_state *state,
			     स्थिर अक्षर *feature)
{
	स्थिर अक्षर *arg;

	अगर (skip_prefix(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} अन्यथा अगर (skip_prefix(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} अन्यथा अगर (skip_prefix(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} अन्यथा अगर (skip_prefix(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} अन्यथा अगर (!म_भेद(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} अन्यथा अगर (skip_prefix(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_to);
	} अन्यथा अगर (skip_prefix(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} अन्यथा अगर (!म_भेद(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} अन्यथा अगर (!म_भेद(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} अन्यथा अगर (!म_भेद(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} अन्यथा अगर (!म_भेद(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} अन्यथा अगर (!म_भेद(feature, "done")) {
		require_explicit_termination = 1;
	} अन्यथा अगर (!म_भेद(feature, "force")) {
		force_update = 1;
	} अन्यथा अगर (!म_भेद(feature, "notes") || !म_भेद(feature, "ls")) {
		; /* do nothing; we have the feature */
	} अन्यथा {
		वापस 0;
	}

	वापस 1;
}

अटल व्योम parse_feature(काष्ठा fast_import_state *state, स्थिर अक्षर *feature)
{
	अगर (state->seen_तथ्य_command)
		die(_("got feature command '%s' after data command"), feature);

	अगर (parse_one_feature(state, feature))
		वापस;

	die(_("this version of fast-import does not support feature %s."), feature);
}

अटल व्योम parse_option(काष्ठा fast_import_state *state, स्थिर अक्षर *option)
{
	अगर (state->seen_तथ्य_command)
		die(_("got option command '%s' after data command"), option);

	अगर (parse_one_option(state, option))
		वापस;

	die(_("this version of fast-import does not support option: %s"), option);
}

अटल व्योम git_pack_config(व्योम)
{
	पूर्णांक indexversion_मूल्य;
	पूर्णांक limit;
	अचिन्हित दीर्घ packsizelimit_मूल्य;

	अगर (!repo_config_get_ulong(the_repository, "pack.depth", &max_depth)) {
		अगर (max_depth > MAX_DEPTH)
			max_depth = MAX_DEPTH;
	}
	अगर (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_मूल्य)) {
		pack_idx_opts.version = indexversion_मूल्य;
		अगर (pack_idx_opts.version > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.version);
	}
	अगर (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_मूल्य))
		max_packsize = packsizelimit_मूल्य;

	अगर (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &limit))
		unpack_limit = limit;
	अन्यथा अगर (!repo_config_get_int(the_repository, "transfer.unpacklimit", &limit))
		unpack_limit = limit;

	repo_config(the_repository, git_default_config, शून्य);
}

अटल स्थिर अक्षर *स्थिर fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	शून्य
};

अटल व्योम parse_argv(काष्ठा fast_import_state *state)
{
	पूर्णांक argc = parse_options(state->argc, state->argv, state->prefix,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	अगर (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->seen_तथ्य_command = 1;
	अगर (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_to);
}

अटल पूर्णांक option_parse_date_format(स्थिर काष्ठा option *opt UNUSED,
				    स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	वापस 0;
}

अटल पूर्णांक option_parse_export_pack_edges(स्थिर काष्ठा option *opt,
					  स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_edges(opt->मूल्य, arg);
	वापस 0;
}

अटल पूर्णांक option_parse_max_pack_आकार(स्थिर काष्ठा option *opt UNUSED,
				      स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_आकार(arg);
	वापस 0;
}

अटल पूर्णांक option_parse_big_file_threshold(स्थिर काष्ठा option *opt UNUSED,
					   स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_threshold(arg);
	वापस 0;
}

अटल पूर्णांक option_parse_signed_commits(स्थिर काष्ठा option *opt UNUSED,
				       स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	वापस 0;
}

अटल पूर्णांक option_parse_signed_tags(स्थिर काष्ठा option *opt UNUSED,
				    स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_tags(arg);
	वापस 0;
}

अटल पूर्णांक option_parse_rewrite_submodules_from(स्थिर काष्ठा option *opt,
						स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->मूल्य, arg, &sub_marks_from);
	वापस 0;
}

अटल पूर्णांक option_parse_rewrite_submodules_to(स्थिर काष्ठा option *opt,
					      स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->मूल्य, arg, &sub_marks_to);
	वापस 0;
}

अटल पूर्णांक option_parse_cat_blob_fd(स्थिर काष्ठा option *opt,
				    स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->मूल्य, arg);
	वापस 0;
}

अटल पूर्णांक option_parse_import_marks(स्थिर काष्ठा option *opt,
				     स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->मूल्य, arg, 0, 0);
	वापस 0;
}

अटल पूर्णांक option_parse_import_marks_if_exists(स्थिर काष्ठा option *opt,
					       स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->मूल्य, arg, 0, 1);
	वापस 0;
}

अटल पूर्णांक option_parse_export_marks(स्थिर काष्ठा option *opt,
				     स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->मूल्य, arg);
	वापस 0;
}

अटल पूर्णांक option_parse_depth(स्थिर काष्ठा option *opt UNUSED,
			      स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_depth(arg);
	वापस 0;
}

अटल पूर्णांक option_parse_active_branches(स्थिर काष्ठा option *opt UNUSED,
					स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_active_branches(arg);
	वापस 0;
}

अटल पूर्णांक option_parse_quiet(स्थिर काष्ठा option *opt UNUSED,
			      स्थिर अक्षर *arg UNUSED, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	वापस 0;
}

पूर्णांक cmd_fast_import(पूर्णांक argc,
		    स्थिर अक्षर **argv,
		    स्थिर अक्षर *prefix,
		    काष्ठा repository *repo)
{
	काष्ठा fast_import_state state;

	काष्ठा option fast_import_options[] = {
		OPT_GROUP(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", शून्य, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", शून्य, शून्य,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", शून्य, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_आकार),
		OPT_CALLBACK_F(0, "big-file-threshold", शून्य, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_threshold),
		OPT_CALLBACK_F(0, "depth", शून्य, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_depth),
		OPT_CALLBACK_F(0, "active-branches", शून्य, N_("n"),
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
		OPT_CALLBACK_F(0, "signed-commits", शून्य, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", शून्य, N_("mode"),
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
	marks = mem_pool_calloc(&fi_mem_pool, 1, माप(काष्ठा mark_set));

	hashmap_init(&object_table, object_entry_hashcmp, शून्य, 0);

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
	क्रम (पूर्णांक i = 1; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];
		अगर (*arg != '-' || !म_भेद(arg, "--"))
			अवरोध;
		अगर (!म_भेद(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * माप(*rc_free));
	क्रम (अचिन्हित पूर्णांक i = 0; i < (cmd_save - 1); i++)
		rc_free[i].अग्र = &rc_free[i + 1];
	rc_free[cmd_save - 1].अग्र = शून्य;

	start_packfile();
	set_die_routine(die_nicely);
	set_checkpoint_signal();
	जबतक (read_अग्र_command(&state) != खातापूर्ण) {
		स्थिर अक्षर *v;
		अगर (!म_भेद("blob", command_buf.buf))
			parse_new_blob(&state);
		अन्यथा अगर (skip_prefix(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		अन्यथा अगर (skip_prefix(command_buf.buf, "tag ", &v))
			parse_new_tag(&state, v);
		अन्यथा अगर (skip_prefix(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		अन्यथा अगर (skip_prefix(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, शून्य);
		अन्यथा अगर (skip_prefix(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		अन्यथा अगर (skip_prefix(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		अन्यथा अगर (!म_भेद("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		अन्यथा अगर (!म_भेद("done", command_buf.buf))
			अवरोध;
		अन्यथा अगर (!म_भेद("alias", command_buf.buf))
			parse_alias(&state);
		अन्यथा अगर (starts_with(command_buf.buf, "progress "))
			parse_progress(&state);
		अन्यथा अगर (skip_prefix(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		अन्यथा अगर (skip_prefix(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		अन्यथा अगर (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		अन्यथा
			die(_("unsupported command: %s"), command_buf.buf);

		अगर (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	अगर (!state.seen_तथ्य_command)
		parse_argv(&state);

	अगर (require_explicit_termination && ख_पूर्ण(मानक_निवेश))
		die(_("stream ends early"));

	end_packfile();

	dump_branches();
	dump_tags();
	unkeep_all_packs();
	dump_marks();

	अगर (pack_edges)
		ख_बंद(pack_edges);

	अगर (show_stats) {
		uintmax_t योग_गिनती = 0, duplicate_गिनती = 0;
		क्रम (माप_प्रकार i = 0; i < ARRAY_आकार(object_गिनती_by_type); i++)
			योग_गिनती += object_गिनती_by_type[i];
		क्रम (माप_प्रकार i = 0; i < ARRAY_आकार(duplicate_गिनती_by_type); i++)
			duplicate_गिनती += duplicate_गिनती_by_type[i];

		ख_लिखो(मानक_त्रुटि, "%s statistics:\n", argv[0]);
		ख_लिखो(मानक_त्रुटि, "---------------------------------------------------------------------\n");
		ख_लिखो(मानक_त्रुटि, "Alloc'd objects: %10" PRIuMAX "\n", alloc_गिनती);
		ख_लिखो(मानक_त्रुटि, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", योग_गिनती, duplicate_गिनती);
		ख_लिखो(मानक_त्रुटि, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_गिनती_by_type[OBJ_BLOB], duplicate_गिनती_by_type[OBJ_BLOB], delta_गिनती_by_type[OBJ_BLOB], delta_गिनती_attempts_by_type[OBJ_BLOB]);
		ख_लिखो(मानक_त्रुटि, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_गिनती_by_type[OBJ_वृक्ष], duplicate_गिनती_by_type[OBJ_वृक्ष], delta_गिनती_by_type[OBJ_वृक्ष], delta_गिनती_attempts_by_type[OBJ_वृक्ष]);
		ख_लिखो(मानक_त्रुटि, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_गिनती_by_type[OBJ_COMMIT], duplicate_गिनती_by_type[OBJ_COMMIT], delta_गिनती_by_type[OBJ_COMMIT], delta_गिनती_attempts_by_type[OBJ_COMMIT]);
		ख_लिखो(मानक_त्रुटि, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_गिनती_by_type[OBJ_TAG], duplicate_गिनती_by_type[OBJ_TAG], delta_गिनती_by_type[OBJ_TAG], delta_गिनती_attempts_by_type[OBJ_TAG]);
		ख_लिखो(मानक_त्रुटि, "Total branches:  %10lu (%10lu loads     )\n", branch_गिनती, branch_load_गिनती);
		ख_लिखो(मानक_त्रुटि, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->shift) * 1024, marks_set_गिनती);
		ख_लिखो(मानक_त्रुटि, "      atoms:     %10u\n", atom_cnt);
		ख_लिखो(मानक_त्रुटि, "Memory total:    %10" PRIuMAX " KiB\n", (वृक्ष_entry_allocd + fi_mem_pool.pool_alloc + alloc_गिनती*माप(काष्ठा object_entry))/1024);
		ख_लिखो(मानक_त्रुटि, "       pools:    %10lu KiB\n", (अचिन्हित दीर्घ)((वृक्ष_entry_allocd + fi_mem_pool.pool_alloc) /1024));
		ख_लिखो(मानक_त्रुटि, "     objects:    %10" PRIuMAX " KiB\n", (alloc_गिनती*माप(काष्ठा object_entry))/1024);
		ख_लिखो(मानक_त्रुटि, "---------------------------------------------------------------------\n");
		pack_report(repo);
		ख_लिखो(मानक_त्रुटि, "---------------------------------------------------------------------\n");
		ख_लिखो(मानक_त्रुटि, "\n");
	}

	वापस failure ? 1 : 0;
}
